/** @copyright
© 2014 SIGMA DESIGNS, INC. THIS IS AN UNPUBLISHED WORK PROTECTED BY SIGMA 
DESIGNS, INC. AS A TRADE SECRET, AND IS NOT TO BE USED OR DISCLOSED EXCEPT AS 
PROVIDED Z-WAVE CONTROLLER DEVELOPMENT KIT LIMITED LICENSE AGREEMENT. ALL 
RIGHTS RESERVED.
@par
NOTICE: ALL INFORMATION CONTAINED HEREIN IS CONFIDENTIAL AND/OR PROPRIETARY TO 
SIGMA DESIGNS AND MAY BE COVERED BY U.S. AND FOREIGN PATENTS, PATENTS IN 
PROCESS, AND ARE PROTECTED BY TRADE SECRET OR COPYRIGHT LAW. DISSEMINATION OR 
REPRODUCTION OF THE SOURCE CODE CONTAINED HEREIN IS EXPRESSLY FORBIDDEN TO 
ANYONE EXCEPT LICENSEES OF SIGMA DESIGNS  WHO HAVE EXECUTED A SIGMA DESIGNS’ 
Z-WAVE CONTROLLER DEVELOPMENT KIT LIMITED LICENSE AGREEMENT. THE COPYRIGHT 
NOTICE ABOVE IS NOT EVIDENCE OF ANY ACTUAL OR INTENDED PUBLICATION OF THE 
SOURCE CODE. THE RECEIPT OR POSSESSION OF  THIS SOURCE CODE AND/OR RELATED 
INFORMATION DOES NOT CONVEY OR IMPLY ANY RIGHTS  TO REPRODUCE, DISCLOSE OR 
DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL A PRODUCT THAT IT  MAY 
DESCRIBE.
@par
THE SIGMA PROGRAM AND ANY RELATED DOCUMENTATION OR TOOLS IS PROVIDED TO COMPANY 
"AS IS" AND "WITH ALL FAULTS", WITHOUT WARRANTY OF ANY KIND FROM SIGMA. COMPANY 
ASSUMES ALL RISKS THAT LICENSED MATERIALS ARE SUITABLE OR ACCURATE FOR 
COMPANY’S NEEDS AND COMPANY’S USE OF THE SIGMA PROGRAM IS AT COMPANY’S 
OWN DISCRETION AND RISK. SIGMA DOES NOT GUARANTEE THAT THE USE OF THE SIGMA 
PROGRAM IN A THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES ENVIRONMENT WILL 
BE: (A) PERFORMED ERROR-FREE OR UNINTERRUPTED; (B) THAT SIGMA WILL CORRECT ANY 
THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT ERRORS; (C) THE 
THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT WILL OPERATE IN 
COMBINATION WITH COMPANY’S CONTENT OR COMPANY APPLICATIONS THAT UTILIZE THE 
SIGMA PROGRAM; (D) OR WITH ANY OTHER HARDWARE, SOFTWARE, SYSTEMS, SERVICES OR 
DATA NOT PROVIDED BY SIGMA. COMPANY ACKNOWLEDGES THAT SIGMA DOES NOT CONTROL 
THE TRANSFER OF DATA OVER COMMUNICATIONS FACILITIES, INCLUDING THE INTERNET, 
AND THAT THE SERVICES MAY BE SUBJECT TO LIMITATIONS, DELAYS, AND OTHER PROBLEMS 
INHERENT IN THE USE OF SUCH COMMUNICATIONS FACILITIES. SIGMA IS NOT RESPONSIBLE 
FOR ANY DELAYS, DELIVERY FAILURES, OR OTHER DAMAGE RESULTING FROM SUCH ISSUES. 
SIGMA IS NOT RESPONSIBLE FOR ANY ISSUES RELATED TO THE PERFORMANCE, OPERATION 
OR SECURITY OF THE THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES 
ENVIRONMENT THAT ARISE FROM COMPANY CONTENT, COMPANY APPLICATIONS OR THIRD 
PARTY CONTENT. SIGMA DOES NOT MAKE ANY REPRESENTATION OR WARRANTY REGARDING THE 
RELIABILITY, ACCURACY, COMPLETENESS, CORRECTNESS, OR USEFULNESS OF THIRD PARTY 
CONTENT OR SERVICE OR THE SIGMA PROGRAM, AND DISCLAIMS ALL LIABILITIES ARISING 
FROM OR RELATED TO THE SIGMA PROGRAM OR THIRD PARTY CONTENT OR SERVICES. TO THE 
EXTENT NOT PROHIBITED BY LAW, THESE WARRANTIES ARE EXCLUSIVE. SIGMA OFFERS NO 
WARRANTY OF NON-INFRINGEMENT, TITLE, OR QUIET ENJOYMENT. NEITHER SIGMA NOR ITS 
SUPPLIERS OR LICENSORS SHALL BE LIABLE FOR ANY INDIRECT, SPECIAL, INCIDENTAL OR 
CONSEQUENTIAL DAMAGES OR LOSS (INCLUDING DAMAGES FOR LOSS OF BUSINESS, LOSS OF 
PROFITS, OR THE LIKE), ARISING OUT OF THIS AGREEMENT WHETHER BASED ON BREACH OF 
CONTRACT, INTELLECTUAL PROPERTY INFRINGEMENT, TORT (INCLUDING NEGLIGENCE), 
STRICT LIABILITY, PRODUCT LIABILITY OR OTHERWISE, EVEN IF SIGMA OR ITS 
REPRESENTATIVES HAVE BEEN ADVISED OF OR OTHERWISE SHOULD KNOW ABOUT THE 
POSSIBILITY OF SUCH DAMAGES. THERE ARE NO OTHER EXPRESS OR IMPLIED WARRANTIES 
OR CONDITIONS INCLUDING FOR SOFTWARE, HARDWARE, SYSTEMS, NETWORKS OR 
ENVIRONMENTS OR FOR MERCHANTABILITY, NONINFRINGEMENT, SATISFACTORY QUALITY AND 
FITNESS FOR A PARTICULAR PURPOSE.
@par
The Sigma Program  is not fault-tolerant and is not designed, manufactured or 
intended for use or resale as on-line control equipment in hazardous 
environments requiring fail-safe performance, such as in the operation of 
nuclear facilities, aircraft navigation or communication systems, air traffic 
control, direct life support machines, or weapons systems, in which the failure 
of the Sigma Program, or Company Applications created using the Sigma Program, 
could lead directly to death, personal injury, or severe physical or 
environmental damage ("High Risk Activities").  Sigma and its suppliers 
specifically disclaim any express or implied warranty of fitness for High Risk 
Activities.Without limiting Sigma’s obligation of confidentiality as further 
described in the Z-Wave Controller Development Kit Limited License Agreement, 
Sigma has no obligation to establish and maintain a data privacy and 
information security program with regard to Company’s use of any Third Party 
Service Environment or Cloud Service Environment. For the avoidance of doubt, 
Sigma shall not be responsible for physical, technical, security, 
administrative, and/or organizational safeguards that are designed to ensure 
the security and confidentiality of the Company Content or Company Application 
in any Third Party Service Environment or Cloud Service Environment that 
Company chooses to utilize.
*/
/**
@file		zwscn_util_mem.c  
  
                Implementaion of functions to allocate/deallocate memory
  
@author		Arif Mohammand
 
@version	1.0 2013-12-05  Initial release
*/

#include <stdlib.h>
#include <stdint.h>
#include "zwscn_defines.h"
#include "zwscn_util_hash.h"
#include "zwscn_util_list.h"
#include "zwscn_util_message_log.h"
#include "zwscn_util_mqueue.h"
#include "zwscn_util_mqueue_message.h"
#include "zwscn_util_thread.h"
#include "zwscn_util_mutex.h"
#include "zwscn_util_rwlock.h"
#include "zwscn_util_timer.h"
#include "zwscn_scene_api.h"
#include "zwscn_statelog.h"
#include "zwscn_scene.h"
#include "zwscn_util_mem.h"
#include "zwscn_util.h"
#include "zwscn_scene_schedule.h"
#include "zwscn_scene_status_update.h"
#include "zwscn_security_scene_api.h"
#include "zwscn_security_scene.h"


int zwscn_free_hash_status_update_timer_info_foreach_callback(void *id, void *status_update_timer_data, void *user_data) {
	zwscn_status_update_timer_info_p status_update_timer_ptr = (zwscn_status_update_timer_info_p) status_update_timer_data;
	zwscn_scene_cont_p scene_ptr = (zwscn_scene_cont_p) user_data;
	uint32_t temp = 0;
	
	if((scene_ptr == NULL) || (scene_ptr->scene == NULL)) {
		goto l_err_args;
	}
	
	temp = zwscn_pointer_to_uint(id);
	zwscn_status_update_cancel_timer(scene_ptr, (temp - ZWSCN_STATUS_UPDATE_HASH_KEY_BASE - scene_ptr->scene->id), status_update_timer_ptr);
	
	return ZWSCN_TRUE; //Returning TRUE removes key-value pair from hash
l_err_args:
	return ZWSCN_TRUE; //Returning TRUE removes key-value pair from hash
}

void zwscn_free_hash_status_update_timer_info(void *scene_ptr) {
	zwscn_scene_cont_p scene_cont_ptr = (zwscn_scene_cont_p) scene_ptr;
	
	zwscn_mutex_lock(scene_cont_ptr->mutex_status_update_timer_info);
	zwscn_hash_foreach_remove(scene_cont_ptr->hash_status_update_timer_info, zwscn_free_hash_status_update_timer_info_foreach_callback, scene_cont_ptr);
	zwscn_mutex_unlock(scene_cont_ptr->mutex_status_update_timer_info);
}

int zwscn_free_hash_device_report_cache_foreach_callback(void *id, void *report_timestamp, void *user_data) {
	free(report_timestamp);
	
	return ZWSCN_TRUE; //Returning TRUE removes key-value pair from hash
}

void zwscn_free_hash_device_report_cache(void *init_data_ptr) {
	zwscn_init_p init_data = (zwscn_init_p) init_data_ptr;
	
	zwscn_mutex_lock(init_data->mutex_device_report_cache);
	zwscn_hash_foreach_remove(init_data->hash_device_report_cache, zwscn_free_hash_device_report_cache_foreach_callback, NULL);
	zwscn_mutex_unlock(init_data->mutex_device_report_cache);
}

int zwscn_free_net_foreach_callback(void *net, void *init_data, void *user_data) {
	if(zwscn_does_net_exist_2(net)) {
		zwscn_free_net(init_data);
	}
	
	return ZWSCN_TRUE; //Returning TRUE removes key-value pair from hash
}

void zwscn_free_net(void *init_data_ptr) {
	zwscn_init_p init_data = (zwscn_init_p) init_data_ptr;
	zwscn_mqueue_t mqueue_events;
	zwscn_mqueue_t mqueue_execute_request_handler;
	zwscn_mqueue_t mqueue_status_update_request_handler;
	zwscn_mqueue_t mqueue_status_update_report_handler;
	zwscn_mqueue_t mqueue_notification_processor;
	zwscn_mqueue_t mqueue_security_events;
	zwscn_mqueue_t mqueue_security_scene_notifications;
	zwscn_thread_t thread_events;
	zwscn_thread_t thread_execute_request_handler;
	zwscn_thread_t thread_status_update_request_handler;
	zwscn_thread_t thread_status_update_report_handler;
	zwscn_thread_t thread_notification_processor;
	zwscn_thread_t thread_security_events;
	zwscn_thread_t thread_security_scene_notifications;
	
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if(!init_data) {
		zwscn_message_log_warning("Input init_data is NULL.\n");
		return;
	}
	
	mqueue_events 							= init_data->mqueue_thread_events;
	mqueue_execute_request_handler			= init_data->mqueue_thread_execute_request_handler;
	mqueue_status_update_request_handler 	= init_data->mqueue_thread_status_update_request_handler;
	mqueue_status_update_report_handler 	= init_data->mqueue_thread_status_update_report_handler;	
	mqueue_notification_processor 			= init_data->mqueue_thread_notification_processor;
	mqueue_security_events 					= init_data->mqueue_thread_security_events;
	mqueue_security_scene_notifications 	= init_data->mqueue_thread_security_scene_notification_request_handler;
	
	thread_events 							= init_data->thread_events;
	thread_execute_request_handler			= init_data->thread_execute_request_handler;
	thread_status_update_request_handler	= init_data->thread_status_update_request_handler;
	thread_status_update_report_handler		= init_data->thread_status_update_report_handler;
	thread_notification_processor 			= init_data->thread_notification_processor;
	thread_security_events 					= init_data->thread_security_events;
	thread_security_scene_notifications		= init_data->thread_security_scene_notification_request_handler;
	
	zwscn_message_log_debug("Free threads\n");
	zwscn_mqueue_send_exit_message(mqueue_events);
	zwscn_thread_join(thread_events);
	
	zwscn_mqueue_send_exit_message(mqueue_execute_request_handler);
	zwscn_thread_join(thread_execute_request_handler);
	
	zwscn_mqueue_send_exit_message(mqueue_status_update_request_handler);
	zwscn_thread_join(thread_status_update_request_handler);
	
	zwscn_mqueue_send_exit_message(mqueue_status_update_report_handler);
	zwscn_status_update_abort_flag_set_2(init_data);
	zwscn_thread_join(thread_status_update_report_handler);
	
	zwscn_mqueue_send_exit_message(mqueue_notification_processor);
	zwscn_thread_join(thread_notification_processor);
	
	zwscn_mqueue_send_exit_message(mqueue_security_events);
	zwscn_thread_join(thread_security_events);
	
	zwscn_mqueue_send_exit_message(mqueue_security_scene_notifications);
	zwscn_thread_join(thread_security_scene_notifications);
	
	zwscn_message_log_debug("Free message queues\n");
	zwscn_mutex_lock(init_data->mqueue_mutex); //lock the mutex for mqueue access
	zwscn_mqueue_delete(mqueue_events);
	zwscn_mqueue_delete(mqueue_execute_request_handler);
	zwscn_mqueue_delete(mqueue_status_update_request_handler);
	zwscn_mqueue_delete(mqueue_status_update_report_handler);
	zwscn_mqueue_delete(mqueue_notification_processor);
	zwscn_mqueue_delete(mqueue_security_events);
	zwscn_mqueue_delete(mqueue_security_scene_notifications);
	
	init_data->mqueue_thread_events = NULL;
	init_data->mqueue_thread_execute_request_handler = NULL;
	init_data->mqueue_thread_status_update_request_handler = NULL;
	init_data->mqueue_thread_status_update_report_handler = NULL;
	init_data->mqueue_thread_notification_processor = NULL;
	init_data->mqueue_thread_security_events = NULL;
	init_data->mqueue_thread_security_scene_notification_request_handler = NULL;
	
	init_data->thread_events = NULL;
	init_data->thread_execute_request_handler = NULL;
	init_data->thread_status_update_request_handler = NULL;
	init_data->thread_status_update_report_handler = NULL;
	init_data->thread_notification_processor = NULL;
	init_data->thread_security_events = NULL;
	init_data->thread_security_scene_notification_request_handler = NULL;
	zwscn_mutex_unlock(init_data->mqueue_mutex); //unlock the mutex for mqueue access
	
	zwscn_free_init_data(init_data_ptr);
}

void zwscn_free_init_data(void *init_data_ptr) {
	zwscn_init_p init_data = (zwscn_init_p) init_data_ptr;
	
	//zwscn_debug("ENTER zwscn_free_init_data\n");
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if(!init_data) {
		return;
	}
//	zwscn_mutex_lock(init_data->net_mutex); 		//Wait until current ops are finished - This is valid only if zwscn_network_[un]lock() functions are used
	zwscn_rwlock_writer_lock(init_data->net_rwlock); //Wait until current ops are finished
	zwscn_message_log_debug("Free Net [%p]\n", init_data->init_input->net);
	zwscn_list_free_scene_list(&(init_data->list_scenes));
	zwscn_list_free_scene_list(&(init_data->list_scenes_invalid));
	zwscn_list_free_security_scene_list(&(init_data->list_security_scenes));
	zwscn_list_free_security_scene_list(&(init_data->list_security_scenes_invalid));
	init_data->list_scenes = NULL;
	init_data->list_scenes_invalid = NULL;
	init_data->list_security_scenes = NULL;
	init_data->list_security_scenes_invalid = NULL;
	free(init_data->init_input);
	init_data->init_input = NULL;
	
	//NOTE: this has to be done before freeing mutex_device_report_cache
	zwscn_free_hash_device_report_cache(init_data);
	zwscn_hash_destroy(init_data->hash_device_report_cache);
	init_data->hash_device_report_cache = NULL;
	zwscn_hash_destroy(init_data->hash_pending_postset_poll_cb);
	init_data->hash_pending_postset_poll_cb = NULL;
	
//	zwscn_mutex_unlock(init_data->net_mutex);
//	zwscn_mutex_delete(init_data->net_mutex);
//	init_data->net_mutex = NULL;
	zwscn_rwlock_writer_unlock(init_data->net_rwlock);
	zwscn_rwlock_delete(init_data->net_rwlock);
	init_data->net_rwlock = NULL;
	
	zwscn_mutex_lock(init_data->mqueue_mutex); //Wait until current ops are finished
	zwscn_mutex_unlock(init_data->mqueue_mutex);
	zwscn_mutex_delete(init_data->mqueue_mutex);
	init_data->mqueue_mutex = NULL;
	
	zwscn_mutex_lock(init_data->statelog_mutex); //Wait until current ops are finished
	zwscn_mutex_unlock(init_data->statelog_mutex);
	zwscn_mutex_delete(init_data->statelog_mutex);
	init_data->statelog_mutex = NULL;
	
	zwscn_mutex_lock(init_data->mutex_abort_status_update_op); //Wait until current ops are finished
	zwscn_mutex_unlock(init_data->mutex_abort_status_update_op);
	zwscn_mutex_delete(init_data->mutex_abort_status_update_op);
	init_data->mutex_abort_status_update_op = NULL;
	
	zwscn_mutex_lock(init_data->mutex_device_report_cache); //Wait until current ops are finished
	zwscn_mutex_unlock(init_data->mutex_device_report_cache);
	zwscn_mutex_delete(init_data->mutex_device_report_cache);
	init_data->mutex_device_report_cache = NULL;
	
	zwscn_mutex_lock(init_data->mutex_pending_report_status); //Wait until current ops are finished
	zwscn_mutex_unlock(init_data->mutex_pending_report_status);
	zwscn_mutex_delete(init_data->mutex_pending_report_status);
	init_data->mutex_pending_report_status = NULL;
	
	free(init_data->config.security_scene_notification_email_sender_addr);
	free(init_data->config.security_scene_notification_sms_sender_addr);
	free(init_data->config.security_scene_notification_sms_gateway);
	free(init_data->config.smtp_username);
	free(init_data->config.smtp_password);
	free(init_data->config.smtp_server_hostname);
	free(init_data->config.smtp_server_port);
	free(init_data->config.smtp_secure_method);
	
	free(init_data);
}

void zwscn_free_new_scene(void *scene_ptr) {
	zwscn_scene_cont_p scene_cont = (zwscn_scene_cont_p) scene_ptr;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if(!scene_cont) {
		return;
	}
	
	zwscn_hash_destroy(scene_cont->hash_status_update_timer_info);
	zwscn_mutex_lock(scene_cont->mutex_status_update_timer_info); //Wait until current ops are finished
	zwscn_mutex_unlock(scene_cont->mutex_status_update_timer_info);
	zwscn_mutex_delete(scene_cont->mutex_status_update_timer_info);
	scene_cont->hash_status_update_timer_info = NULL;
	scene_cont->mutex_status_update_timer_info = NULL;
	if(scene_cont->list_actions) {
		zwscn_list_free_action_list(&(scene_cont->list_actions));
	}
	if(scene_cont->list_schedules) {
		zwscn_list_free_schedule_list(&(scene_cont->list_schedules));
	}
	if(scene_cont->list_events) {
		zwscn_list_free_event_list(&(scene_cont->list_events));
	}
	if(scene_cont->list_actions_invalid) {
		zwscn_list_free_action_list(&(scene_cont->list_actions_invalid));
	}
	if(scene_cont->list_events_invalid) {
		zwscn_list_free_event_list(&(scene_cont->list_events_invalid));
	}
	scene_cont->list_actions = NULL;
	scene_cont->list_schedules = NULL;
	scene_cont->list_events = NULL;
	scene_cont->list_actions_invalid = NULL;
	scene_cont->list_events_invalid = NULL;
	
	if(scene_cont->scene) {
//		zwscn_message_log_debug("\tFree Scene [%d %s]\n", scene_cont->scene->id, scene_cont->scene->name);
		free(scene_cont->scene);
		scene_cont->scene = NULL; // Not needed but written for completeness
	}
	
	free(scene_cont);
}

void zwscn_free_scene(void *scene_ptr) {
	zwscn_scene_cont_p scene_cont = (zwscn_scene_cont_p) scene_ptr;
	
	//zwscn_debug("ENTER zwscn_free_scene\n");
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if(!scene_cont) {
		return;
	}
	
	//zwscn_status_update_cancel_timer(scene_ptr);
	zwscn_free_hash_status_update_timer_info(scene_cont);
	zwscn_hash_destroy(scene_cont->hash_status_update_timer_info);
	zwscn_mutex_lock(scene_cont->mutex_status_update_timer_info); //Wait until current ops are finished
	zwscn_mutex_unlock(scene_cont->mutex_status_update_timer_info);
	zwscn_mutex_delete(scene_cont->mutex_status_update_timer_info);
	scene_cont->hash_status_update_timer_info = NULL;
	scene_cont->mutex_status_update_timer_info = NULL;
	
//	zwscn_list_free_action_list(&(scene_cont->list_actions));
//	zwscn_list_free_schedule_list(&(scene_cont->list_schedules));
//	zwscn_list_free_event_list(&(scene_cont->list_events));
//	zwscn_list_free_action_list(&(scene_cont->list_actions_invalid));
//	zwscn_list_free_event_list(&(scene_cont->list_events_invalid));
//	scene_cont->list_actions = NULL;
//	scene_cont->list_schedules = NULL;
//	scene_cont->list_events = NULL;
//	scene_cont->list_actions_invalid = NULL;
//	scene_cont->list_events_invalid = NULL;
	zwscn_free_scene_lists(scene_ptr);
	
	if(scene_cont->scene) {
		zwscn_message_log_debug("\tFree Scene [%d %s]\n", scene_cont->scene->id, scene_cont->scene->name);
		free(scene_cont->scene);
		scene_cont->scene = NULL; // Not needed but written for completeness
	}
	
	free(scene_cont);
}

void zwscn_free_scene_lists(void *scene_ptr) {
	zwscn_scene_cont_p scene_cont = (zwscn_scene_cont_p) scene_ptr;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if(!scene_cont) {
		return;
	}
	
	if(scene_cont->list_actions) {
		zwscn_status_update_remove_pending_reqs_on_action_delete_all(scene_cont->net, scene_cont); //This function calls zwscn_get_init_data() but has not negative effect if obtains NULL
		zwscn_list_free_action_list(&(scene_cont->list_actions));
	}
	if(scene_cont->list_schedules) {
		zwscn_list_free_schedule_list(&(scene_cont->list_schedules));
	}
	if(scene_cont->list_events) {
		zwscn_list_free_event_list(&(scene_cont->list_events));
	}
	if(scene_cont->list_actions_invalid) {
		zwscn_list_free_action_list(&(scene_cont->list_actions_invalid));
	}
	if(scene_cont->list_events_invalid) {
		zwscn_list_free_event_list(&(scene_cont->list_events_invalid));
	}
	scene_cont->list_actions = NULL;
	scene_cont->list_schedules = NULL;
	scene_cont->list_events = NULL;
	scene_cont->list_actions_invalid = NULL;
	scene_cont->list_events_invalid = NULL;
}

void zwscn_free_action(void *action_ptr) {
	zwscn_action_p action = (zwscn_action_p) action_ptr;
	
	//zwscn_debug("ENTER zwscn_free_action\n");
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if(!action) {
		return;
	}
	
	free(action->device_args);
	action->device_args = NULL; // Not needed but written for completeness
	
	zwscn_message_log_debug("\t\tFree Action [id=%u]\n", action->id);
	free(action);
}

void zwscn_free_schedule(void *schedule_ptr) {
	zwscn_schedule_p schedule = (zwscn_schedule_p) schedule_ptr;
	
	//zwscn_debug("ENTER zwscn_free_schedule\n");
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if(!schedule) {
		return;
	}
	
	zwscn_schedule_cancel_timer(schedule);
	zwscn_message_log_debug("\t\tFree Schedule [id=%u]\n", schedule->id);
	free(schedule);
}

void zwscn_free_event(void *event_ptr) {
	zwscn_event_p event = (zwscn_event_p) event_ptr;
	
	//zwscn_debug("ENTER zwscn_free_event\n");
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if(!event) {
		return;
	}
	
	free(event->device_args);
	event->device_args = NULL; // Not needed but written for completeness
	
	zwscn_message_log_debug("\t\tFree Event [id=%u]\n", event->id);
	free(event);
}

void zwscn_free_mqueue_message(void *message_ptr) {
	zwscn_mqueue_message_p message = (zwscn_mqueue_message_p) message_ptr;
	
	//zwscn_message_log_debug("ENTER zwscn_free_mqueue_message\n");
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if(!message) {
		return;
	}
	
	free(message->data);
	message->data = NULL; // Not needed but written for completeness
		
	free(message);
}

//Security Secene
void zwscn_free_security_scene(void *scene_ptr) {
	zwscn_security_scene_cont_p scene_cont = (zwscn_security_scene_cont_p) scene_ptr;
	
	//zwscn_debug("ENTER zwscn_free_scene\n");
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if(!scene_cont) {
		return;
	}

	zwscn_free_security_scene_lists(scene_ptr);
	if(scene_cont->scene) {
		zwscn_message_log_debug("\tFree Security Scene [%d %s]\n", scene_cont->scene->id, scene_cont->scene->name);
		zwscn_free_security_scene_struct(scene_cont->scene);
		scene_cont->scene = NULL; // Not needed but written for completeness
	}
	
	free(scene_cont);
}

void zwscn_free_security_scene_struct(void *scene) {
	zwscn_security_scene_p scene_ptr = (zwscn_security_scene_p) scene;
	
	if(scene_ptr) {
		if(scene_ptr->lt_arm.event) {
			zwscn_free_security_event(scene_ptr->lt_arm.event);
			scene_ptr->lt_arm.event = NULL;
		}
		if(scene_ptr->lt_disarm.event) {
			zwscn_free_security_event(scene_ptr->lt_disarm.event);
			scene_ptr->lt_disarm.event = NULL;
		}
		if(scene_ptr->lt_alarm_on.event) {
			zwscn_free_security_event(scene_ptr->lt_alarm_on.event);
			scene_ptr->lt_alarm_on.event = NULL;
		}
		if(scene_ptr->lt_alarm_off.event) {
			zwscn_free_security_event(scene_ptr->lt_alarm_off.event);
			scene_ptr->lt_alarm_off.event = NULL;
		}
		free(scene_ptr);
	}
}

void zwscn_free_security_scene_lists(void *scene_ptr) {
	zwscn_security_scene_cont_p scene_cont = (zwscn_security_scene_cont_p) scene_ptr;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if(!scene_cont) {
		return;
	}
	
	if(scene_cont->list_arm_events) {
		zwscn_list_free_arm_security_event_list(&(scene_cont->list_arm_events));
	}
	if(scene_cont->list_arm_events_invalid) {
		zwscn_list_free_arm_security_event_list(&(scene_cont->list_arm_events_invalid));
	}
	if(scene_cont->list_disarm_events) {
		zwscn_list_free_disarm_security_event_list(&(scene_cont->list_disarm_events));
	}
	if(scene_cont->list_disarm_events_invalid) {
		zwscn_list_free_disarm_security_event_list(&(scene_cont->list_disarm_events_invalid));
	}
	if(scene_cont->list_alarm_events) {
		zwscn_list_free_alarm_security_event_list(&(scene_cont->list_alarm_events));
	}
	if(scene_cont->list_alarm_events_invalid) {
		zwscn_list_free_alarm_security_event_list(&(scene_cont->list_alarm_events_invalid));
	}
	scene_cont->list_arm_events = NULL;
	scene_cont->list_disarm_events = NULL;
	scene_cont->list_alarm_events = NULL;
	scene_cont->list_arm_events_invalid = NULL;
	scene_cont->list_disarm_events_invalid = NULL;
	scene_cont->list_alarm_events_invalid = NULL;
}

void zwscn_free_security_event(void *event_ptr) {
	zwscn_security_event_p event = (zwscn_security_event_p) event_ptr;
	
	//zwscn_debug("ENTER zwscn_free_event\n");
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if(!event) {
		return;
	}
	
	free(event->device_args);
	event->device_args = NULL; // Not needed but written for completeness
	
	zwscn_message_log_debug("\t\tFree Security Event [id=%u]\n", event->id);
	free(event);
}

