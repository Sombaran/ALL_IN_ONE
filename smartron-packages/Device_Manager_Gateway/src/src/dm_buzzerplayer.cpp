#include <iostream>
#include <gstreamer-1.0/gst/gst.h>
#include <glib-object.h>
#include <thread>
#include <unistd.h>
#include <mutex>
#include <sys/syslog.h>

std::mutex m1;
GstElement *pipeline;
GstBus *bus;
GstMessage *msg;
GMainLoop *main_loop;
guint bus_watch_id_prim;
bool player_state = false;
typedef enum {
	USER_STATE_PAUSE, USER_STATE_PLAY
} USER_STATE;

USER_STATE us = USER_STATE_PAUSE;

void Audio_play() {
	std::lock_guard<std::mutex> l1(m1);
	GstStateChangeReturn ret;
	if (player_state == true) {
		ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
		if (ret == GST_STATE_CHANGE_FAILURE) {
			syslog(LOG_ERR,
			        "[DM]:GPRIM unable to set the pipeline_prim to the playing state");
			gst_element_set_state(pipeline, GST_STATE_NULL);
			g_source_remove(bus_watch_id_prim);
			bus_watch_id_prim = 0;
			return;
		}
	}
	us = USER_STATE_PLAY;
}

void Audio_pause() {
	std::lock_guard<std::mutex> l1(m1);
	GstStateChangeReturn ret;
	if (player_state == true) {
		ret = gst_element_set_state(pipeline, GST_STATE_READY);
		if (ret == GST_STATE_CHANGE_FAILURE) {
			syslog(LOG_ERR,
			        "[DM]:GPRIM unable to set the pipeline_prim to the pause state");
			gst_element_set_state(pipeline, GST_STATE_NULL);
			g_source_remove(bus_watch_id_prim);
			bus_watch_id_prim = 0;
			return;
		}
	}
	us = USER_STATE_PAUSE;
}

static gboolean bus_message(GstBus * bus, GstMessage * message,
		void *priv_data) {

	GstState state;
	GstState pending;
	gboolean ret = TRUE;

	std::lock_guard<std::mutex> l1(m1);
	//syslog(LOG_INFO, "VA:GPRIM got message %s, %d",gst_message_type_get_name (GST_MESSAGE_TYPE (message)),GST_MESSAGE_TYPE (message));

	switch (GST_MESSAGE_TYPE(message)) {
	case GST_MESSAGE_ERROR:
		g_main_loop_quit(main_loop);
		break;

	case GST_MESSAGE_EOS:

		gst_element_set_state(pipeline, GST_STATE_READY);
		g_object_set(pipeline, "uri", "file:///exe_dir/buzzer.mp3", NULL);
		if (us == USER_STATE_PLAY) {
			gst_element_set_state(pipeline, GST_STATE_PLAYING);
		}
		break;

	case GST_MESSAGE_STATE_CHANGED:

		if (GST_MESSAGE_SRC(message) == GST_OBJECT(pipeline)) {
			GstState old_state, new_state, pending_state;
			gst_message_parse_state_changed(message, &old_state, &new_state,
					&pending_state);
			//syslog(LOG_INFO,"Pipeline state changed from %s to %s:",	gst_element_state_get_name (old_state), gst_element_state_get_name (new_state));
			ret = TRUE;
		}

		break;

	default:
		break;
	}

	return ret;
}

void AudioPlayer() {
	gst_init(NULL, NULL);
	GstStateChangeReturn ret;
	std::unique_lock<std::mutex> lck(m1, std::defer_lock);
	while (1) {
		lck.lock();
		pipeline = gst_element_factory_make("playbin", "playbin");
		if (pipeline == NULL) {
			return;
		}
		g_object_set(pipeline, "uri", "file:///exe_dir/buzzer.mp3", NULL);
		bus = gst_element_get_bus(pipeline);

		bus_watch_id_prim = gst_bus_add_watch(bus, (GstBusFunc) bus_message,
				pipeline);
		gst_object_unref(bus);
		if (us == USER_STATE_PLAY) {
			ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
			if (ret == GST_STATE_CHANGE_FAILURE) {
				syslog(LOG_ERR,
			        "[DM]:GPRIM unable to set the pipeline_prim to the playing state");
				gst_element_set_state(pipeline, GST_STATE_NULL);
				g_source_remove(bus_watch_id_prim);
				bus_watch_id_prim = 0;
				return;
			}

		} else {

			ret = gst_element_set_state(pipeline, GST_STATE_READY);
			if (ret == GST_STATE_CHANGE_FAILURE) {
				syslog(LOG_ERR,
			        "[DM]:GPRIM unable to set the pipeline_prim to the ready state");
				gst_element_set_state(pipeline, GST_STATE_NULL);
				g_source_remove(bus_watch_id_prim);
				bus_watch_id_prim = 0;
				return;
			}
		}
		player_state = true;
		main_loop = g_main_loop_new(NULL, FALSE);
		lck.unlock();
		g_main_loop_run(main_loop);
		lck.lock();
		gst_element_set_state(pipeline, GST_STATE_NULL);
		gst_object_unref(pipeline);
		g_main_loop_unref(main_loop);
		player_state = false;
		lck.unlock();

	}
}
