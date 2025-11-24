#include <unistd.h>
#include "sm_iface.h"
#include <json-c/json.h>

#define SM_JSON_FILE_PATH	"/etc/sm_config.json"


const char *stat_net = NULL;

static unsigned long ticks = 0;
bool intf_obj = true;
bool leave_obj;
static int reconnection_timeout = 0;
static int max_count = 0;

void cb(evutil_socket_t fd, short what, void *arg);
void signal_cb(evutil_socket_t fd, short event, void *arg);
int ip_notify(struct ubus_context *ctx, struct ubus_object *obj,
		struct ubus_request_data *req, const char *method,
		struct blob_attr *msg);
void remove_connection(struct ubus_context *ctx, struct ubus_subscriber *obj,
		uint32_t id);
void reconnection_timer(struct uloop_timeout *);
void reconnect_ip_obj(struct uloop_timeout *, unsigned int);

struct uloop_timeout timeout = { .cb = reconnection_timer };

static void read_config() {
	json_object *local_obj = json_object_from_file(SM_JSON_FILE_PATH);
	json_object *obj;
	if (local_obj != NULL) {
		json_object_object_get_ex(local_obj, "max_count", &obj);
		max_count = json_object_get_int(obj);
//		printf ("\n The json object loop_timeout: %d \n",max_count);

		json_object_object_get_ex(local_obj, "reconnection_timeout", &obj);
		reconnection_timeout = json_object_get_int(obj);
//		printf ("\n The json object loop_timeout: %d \n",reconnection_timeout);

		syslog(LOG_INFO, "[SM]:read_config success in sm_iface\n");

	}

}

void reconnect_ip_obj(struct uloop_timeout *t, unsigned int delay) {

	int ret;

	if (!leave_obj) {
		syslog(LOG_INFO, "[SM]: Trying to reconnect to network event\n");

		ret = ubus_lookup_id(ctx, "sm_interface", &obj_id);
		if (ret) {
			syslog(LOG_ERR,
					"[SM]:Failed to lookup for object in reconnection : %s\n",
					ubus_strerror(ret));
		} else {
			syslog(LOG_INFO,
					"[SM]:lookup object added in reconnection \n");
			ret = ubus_subscribe(ctx, &network_event, obj_id);

			if (ret) {
				syslog(LOG_ERR,
						"[SM]:Failed to re-subscribe to network event: %s\n",
						ubus_strerror(ret));
			} else {
				syslog(LOG_INFO,
						"[SM]:Re-subscribed to network event with Object id: %08x \n",
						obj_id);
				leave_obj = true;
			}
		}

	}

	uloop_timeout_set(t, delay);
	uloop_timeout_add(t);
	return;

}

void reconnection_timer(struct uloop_timeout *t) {
	ticks++;
	syslog(LOG_INFO, "[SM]: %s : TRIAL = %lu\n", __FUNCTION__, ticks);
	if (ticks == max_count || leave_obj) {
		ticks = 0;
		return;
	} else
		reconnect_ip_obj(t, reconnection_timeout);
}

int ip_notify(struct ubus_context *ctx, struct ubus_object *obj,
		struct ubus_request_data *req, const char *method,
		struct blob_attr *msg) {
	char *str = NULL;

	str = blobmsg_format_json(msg, true);
	intf_obj = true;
//	syslog(fac|LOG_INFO,"[SM]:Received notification '%s': %s   \n", method, str);
	stat_net = str;

	return 0;
}

void remove_connection(struct ubus_context *ctx, struct ubus_subscriber *obj,
		uint32_t id) {

	intf_obj = false;
	leave_obj = false;
	syslog(LOG_INFO, "[SM]:Object %08x went away\n", id);
	reconnect_ip_obj(&timeout, reconnection_timeout);

}

int sm_register_obj(char *obj_name) {
	int ret;

	network_event.cb = ip_notify;
	network_event.remove_cb = remove_connection;

	ret = ubus_register_subscriber(ctx, &network_event);
	if (ret) {
		syslog(LOG_ERR, "[SM]:Failed to register network_event: %s\n",
				ubus_strerror(ret));
		return UBUS_STATUS_NOT_FOUND;
	}

	ret = ubus_lookup_id(ctx, obj_name, &obj_id);
	if (ret) {
		syslog(LOG_ERR, "[SM]:Failed to lookup for object %s: %s\n",
				obj_name, ubus_strerror(ret));
		return UBUS_STATUS_NOT_FOUND;
	}

	ret = ubus_subscribe(ctx, &network_event, obj_id);
	syslog(LOG_INFO,
			"[SM]: Subscribed to network event with Object id :%08x \n",
			obj_id);
	if (ret) {
		syslog(LOG_ERR,
				"[SM]: Failed to subscribe to network event : %s\n",
				ubus_strerror(ret));
		return UBUS_STATUS_METHOD_NOT_FOUND;
	}
	return 0;

}

/********for event based loop**************************/
void cb(evutil_socket_t fd, short what, void *arg) {
	struct ubus_context *ctx = arg;
	ubus_handle_event(ctx);
}
void signal_cb(evutil_socket_t fd, short event, void *arg) {
	struct event_base *evloop = arg;
	event_base_loopbreak(evloop);
}

int sm_socket_init() {

	const char *ubus_socket = NULL;

	read_config();
	uloop_init();

	ctx = ubus_connect(ubus_socket);
	if (!ctx) {
		syslog(LOG_ERR, "[SM]:Failed to connect to ubus\n");
		return UBUS_STATUS_CONNECTION_FAILED;
	}
	ubus_add_uloop(ctx);
	return 0;
}

void sm_event_call() {
	struct event_base *evloop = event_base_new();

	struct event *signal_int = evsignal_new(evloop, SIGINT, signal_cb, evloop);
	event_add(signal_int, NULL);

	struct event *e = event_new(evloop, ctx->sock.fd, EV_READ | EV_PERSIST, cb,
			ctx);
	event_add(e, NULL);

//	event_base_dispatch(evloop);
	event_base_loop(evloop, EVLOOP_ONCE);

	event_free(signal_int);
	event_free(e);

	event_base_free(evloop);
}

