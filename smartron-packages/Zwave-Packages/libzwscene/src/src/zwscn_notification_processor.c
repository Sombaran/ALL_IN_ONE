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
@file		zwscn_notification_processor.c  
  
                To provide support for processing network and node notifications
  
@author		Arif Mohammand
 
@version	1.0 2014-02-26  Initial release
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
#include "zwscn_security_scene_api.h"
#include "zwscn_statelog.h"
#include "zwscn_scene.h"
#include "zwscn_security_scene.h"
#include "zwscn_scene_status_update.h"
#include "zwscn_persistence.h"
#include "zwscn_util.h"
#include "zwscn_util_mem.h"
#include "zwscn_zwpoll.h"
#include "zwscn_util_mem.h"
#include "zwscn_notification_handler.h"
#include "zwscn_notification_processor.h"
#include "zwscn_notification_processor_security_scene.h"
#include "zwscn_device_report_cache.h"

void zwscn_notification_processor_thread(void *data) {
	zwscn_mqueue_t mqueue_events;
	zwscn_mqueue_message_p message;
	void *net;
	uint8_t node_id;
	uint32_t message_type;
	uint32_t notification_type;
	ZWSCN_STATUS_T result;
	
	if(data == NULL) {
		zwscn_message_log_error("message queue is NULL. Could not start EVENTS thread\n");
		return;
	}
	zwscn_message_log_info("THREAD_NOTIFICATIONS started\n");
	mqueue_events = (zwscn_mqueue_t) data;
	
	while(1) {
		message = zwscn_mqueue_message_pop(mqueue_events); //Blocking call
		if(message != NULL) {
			zwscn_message_log_debug("NOTIFICATIONS Thread received message with refcount=%d type=%d priority=%d\n", message->refcount, message->type, message->priority);
			if(message->type == ZWSCN_MESSAGE_TYPE_EXIT) {
				if(zwscn_mqueue_message_refcount_dec_and_test(message)) {
					zwscn_free_mqueue_message(message);
				}
				zwscn_message_log_info("NOTIFICATIONS Thread exiting...\n");
				zwscn_thread_exit(NULL);
			} else {
				net = message->net;
				message_type = message->type;
				notification_type = message->sub_type;
				if(message->data_size == sizeof(node_id)) {
					node_id = *((uint8_t *) message->data);
				} else {
					node_id = 0;
				}
				
				//Network 
				if(message_type == ZWSCN_MESSAGE_TYPE_NOTIFICATION_NETWORK) {
					if(notification_type == ZWSCN_NOTIFICATION_NETWORK_RESET) {
						result = zwscn_network_reset_notification_processor(net, message->data, message->data_size); //data = new persistent context
						if(result != ZWSCN_ERR_NONE) {
							zwscn_message_log_warning("zwscn_network_reset_notification_processor returned %d\n", result);
						}
					} else if(notification_type == ZWSCN_NOTIFICATION_NETWORK_UPDATE) {
						result = zwscn_network_node_update_notification_processor(net, 0); //node_id == 0 means network operation
						if(result != ZWSCN_ERR_NONE) {
							zwscn_message_log_warning("zwscn_network_node_update_notification_processor returned %d\n", result);
						}
					} else if(notification_type == ZWSCN_NOTIFICATION_NETWORK_REPLACE_FAILED_NODE) {
						result = zwscn_network_node_update_notification_processor(net, node_id);
						if(result != ZWSCN_ERR_NONE) {
							zwscn_message_log_warning("zwscn_network_node_update_notification_processor returned %d\n", result);
						}
					} 
				}
				
				//Node
				if(message_type == ZWSCN_MESSAGE_TYPE_NOTIFICATION_NODE) {
					if(notification_type == ZWSCN_NOTIFICATION_NODE_REMOVED) {
						result = zwscn_node_removed_notification_processor(net, node_id);
						if(result != ZWSCN_ERR_NONE) {
							zwscn_message_log_warning("zwscn_node_removed_notification_processor returned %d\n", result);
						}
					} else if(notification_type == ZWSCN_NOTIFICATION_NODE_UPDATE) {
						result = zwscn_network_node_update_notification_processor(net, node_id);
						if(result != ZWSCN_ERR_NONE) {
							zwscn_message_log_warning("zwscn_network_node_update_notification_processor returned %d\n", result);
						}
					}
				}
				
				//ZWPoll
				if(message_type == ZWSCN_MESSAGE_TYPE_NOTIFICATION_ZWPOLL) {
					uint16_t handle = notification_type;
					uint32_t token = 0;
					if(message->data_size == sizeof(token)) {
						token = *((uint32_t *) message->data);
					}
					
					result = zwscn_zwpoll_callback_notification_processor(net, token, handle);
					if(result != ZWSCN_ERR_NONE) {
						zwscn_message_log_warning("zwscn_zwpoll_callback_notification_processor returned %d\n", result);
					}
				}
				
				//Post-Set Poll
				if(message_type == ZWSCN_MESSAGE_TYPE_NOTIFICATION_POSTSETPOLL) {
					int reason = notification_type;
					uint32_t action_id = 0;
					if(message->data_size == sizeof(action_id)) {
						action_id = *((uint32_t *) message->data);
					}
					
					result = zwscn_postset_poll_callback_notification_processor(net, action_id, reason);
					if(result != ZWSCN_ERR_NONE) {
						zwscn_message_log_warning("zwscn_postset_poll_callback_notification_processor returned %d\n", result);
					}
				}
				
				if(zwscn_mqueue_message_refcount_dec_and_test(message)) {
					zwscn_free_mqueue_message(message);
				}
			}
		}
	}
}

//Function for ZWPOll callback notification
ZWSCN_STATUS_T zwscn_zwpoll_callback_notification_processor(void *net, uint32_t token, uint16_t handle) {
	//information we have - handle, token (==  action id)
	//ideally only one zwpoll should be running for one token i.e.
	//before starting a new poll for the same token, all previous polls should be cancelled 
	//otherwise we'd be polling a device too aggressively
	ZWSCN_STATUS_T result = ZWSCN_ERR_NONE;
	
	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	result =  zwscn_zwpoll_clear_pending_reports_count(net, token, handle); //handle 0 - is ignored as we are working based on token
	zwscn_network_unlock_reader(net);
	
	return result;
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	return ZWSCN_ERR_INVALID_NETWORK;
}

//Function for Post-Set POll callback notification
ZWSCN_STATUS_T zwscn_postset_poll_callback_notification_processor(void *net, uint32_t action_id, int reason) {
	ZWSCN_STATUS_T result = ZWSCN_ERR_NONE;
	zwscn_init_p init_data = NULL;
	
	zwscn_message_log_debug("zwscn_postset_poll_callback_notification_processor: net:%p action_id:0x%x reason:%d\n", net, action_id, reason);
	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		goto l_err;
	}
	
	zwscn_message_log_debug("POSTSETPOLL removing action_id:0x%x\n", action_id);
	zwscn_mutex_lock(init_data->mutex_pending_report_status);
	zwscn_hash_remove(init_data->hash_pending_postset_poll_cb, zwscn_uint_to_pointer(action_id));
	zwscn_mutex_unlock(init_data->mutex_pending_report_status);
	zwscn_status_update_reset_pending_flag(net); //this function locks mutex_pending_report_status
	
	zwscn_network_unlock_reader(net);
	
	return result;
l_err:
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	return ZWSCN_ERR_INVALID_NETWORK;
}


//Function for Network Reset
ZWSCN_STATUS_T zwscn_network_reset_notification_processor(void *net, void *buf, uint32_t buf_size) {
	ZWSCN_STATUS_T result;
	zwscn_init_p init_data;
	uint32_t temp_size;
//	uint8_t node_id;
	
	
	//TODO abort all pending tasks including status update
	
	result = zwscn_del_all_scenes(net); //NOTE: calling API function that takes care of locking - unlocking the network
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("Error (%d) returned by zwscn_del_all_scenes\n", result);
	}
	result = zwscn_del_all_security_scenes(net); //NOTE: calling API function that takes care of locking - unlocking the network
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("Error (%d) returned by zwscn_del_all_security_scene\n", result);
	}
	if((buf == NULL) || (buf_size == 0)) {
		goto l_err_args;
	}
	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		goto l_err;
	}
	
	init_data->pool_scene_id = 0;
	init_data->last_executed_scene_id = 0;
	init_data->list_scenes = NULL;
	init_data->list_security_scenes = NULL;
	zwscn_mqueue_clear(init_data->mqueue_thread_events);
	zwscn_mqueue_clear(init_data->mqueue_thread_execute_request_handler);
	zwscn_mqueue_clear(init_data->mqueue_thread_status_update_request_handler);
	zwscn_mqueue_clear(init_data->mqueue_thread_status_update_report_handler);
	zwscn_mqueue_clear(init_data->mqueue_thread_notification_processor);
	zwscn_mqueue_clear(init_data->mqueue_thread_security_events);
	zwscn_mqueue_clear(init_data->mqueue_thread_security_scene_notification_request_handler);
	zwscn_free_hash_device_report_cache(init_data); //Note: this funtion internally uses mutex_device_report_cache
	zwscn_mutex_lock(init_data->mutex_pending_report_status);
	zwscn_hash_remove_all(init_data->hash_pending_postset_poll_cb);
	zwscn_mutex_unlock(init_data->mutex_pending_report_status);
	zwscn_message_log_debug("OLD context_persistent=%s\n", init_data->init_input->context_persistent);
	//TODO make sure mutexes are in right state
	memset(init_data->init_input->context_persistent, 0, sizeof(init_data->init_input->context_persistent));
	temp_size = ((sizeof(init_data->init_input->context_persistent)) > buf_size) ? buf_size : (sizeof(init_data->init_input->context_persistent));
	zwscn_util_strcpy((char *)init_data->init_input->context_persistent, (char *)buf, temp_size);
	zwscn_message_log_debug("NEW context_persistent=%s\n", init_data->init_input->context_persistent);
	
	zwscn_network_unlock_writer(net);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	zwscn_network_unlock_writer(net);
	return ZWSCN_ERR_GENERIC;
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	return ZWSCN_ERR_INVALID_NETWORK;
}

//Function for Node Removed
ZWSCN_STATUS_T zwscn_node_removed_notification_processor(void *net, uint8_t node_id) {
	
	if ((net == NULL) || (node_id == 0)) {
		goto l_err_args;
	}
	
	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_remove_scenes_by_node_id(net, node_id) != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("zwscn_remove_scenes_by_node_id return error\n");
		goto l_err;
	}
	if(zwscn_remove_security_scenes_by_node_id(net, node_id) != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("zwscn_remove_security_scenes_by_node_id return error\n");
		goto l_err;
	}
	zwscn_network_unlock_writer(net);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	zwscn_network_unlock_writer(net);
	return ZWSCN_ERR_GENERIC;
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	return ZWSCN_ERR_INVALID_NETWORK;
}

ZWSCN_STATUS_T zwscn_remove_scenes_by_node_id(void *net, uint8_t node_id) {
	zwscn_list_t *scene_list_ptr;
	zwscn_list_t scene_list;
	zwscn_list_foreach_data_t list_foreach_data;
	
	zwscn_message_log_debug("ENTER zwscn_remove_scene_by_node_id net=%p node_id=%d\n", net, node_id);
	if ((net == NULL) || (node_id == 0)) {
		zwscn_message_log_critical("zwscn_remove_scene_by_node_id input args NULL\n");
		goto l_err_args;
	}
	
	if(zwscn_find_scene_list(net, &scene_list_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	list_foreach_data.net = net;
	list_foreach_data.node_id = node_id;
	list_foreach_data.list_ptr = scene_list_ptr;
	scene_list = (*scene_list_ptr);
	zwscn_list_foreach(scene_list, zwscn_remove_scenes_by_node_id_list_callback, &list_foreach_data);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;
}

void zwscn_remove_scenes_by_node_id_list_callback(void *list_item, void *user_data) {
	void *net;
	zwscn_list_t *scene_list_ptr;
	zwscn_scene_cont_p scene_ptr;
	uint8_t node_id;
	uint32_t scene_id;
	int initial_count_actions;
	int count_actions;
	zwscn_list_foreach_data_t *list_foreach_data_ptr;
	ZWSCN_STATUS_T result;
	
	zwscn_message_log_debug("ENTER zwscn_remove_scenes_by_node_id_list_callback\n");
	if((list_item == NULL) || (user_data == NULL)) {
		zwscn_message_log_critical("zwscn_remove_scenes_by_node_id_list_callback input args NULL\n");
		goto l_err;
	}
	
	scene_ptr = (zwscn_scene_cont_p) list_item;
	list_foreach_data_ptr = (zwscn_list_foreach_data_t *) user_data;
	net = list_foreach_data_ptr->net;
	node_id = list_foreach_data_ptr->node_id;
	scene_list_ptr = list_foreach_data_ptr->list_ptr;
	
	initial_count_actions = zwscn_list_size(scene_ptr->list_actions);
	result = zwscn_remove_actions_by_node_id(net, node_id, scene_ptr);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_critical("[Warning] (%d) returned by zwscn_remove_actions_by_node_id for scene id=%d\n", result, scene_ptr->scene->id);
		goto l_err;
	}
	
	result = zwscn_remove_events_by_node_id(net, node_id, scene_ptr);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_critical("[Warning] (%d) returned by zwscn_remove_events_by_node_id for scene id=%d\n", result, scene_ptr->scene->id);
		goto l_err;
	}
	
	count_actions = zwscn_list_size(scene_ptr->list_actions);
	zwscn_message_log_debug("Scene id=%d has %d actions left\n", scene_ptr->scene->id, count_actions);
	if((initial_count_actions == 0) || (count_actions > 0)) { //If the Actions count was zero initially for some reason then we don't delete this Scene
		zwscn_message_log_debug("Saving Scene...\n");
		zwscn_persistence_save_scene(net, scene_ptr);
	} else {
		scene_id = scene_ptr->scene->id;
		zwscn_message_log_debug("Removing Scene...\n");
		zwscn_set_statelog_start_op_del(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
		zwscn_persistence_delete_scene(net, scene_id);
		(*scene_list_ptr) = zwscn_list_remove((*scene_list_ptr), scene_ptr);
		zwscn_free_scene(scene_ptr);
		zwscn_message_log_debug("Scene Removed\n");
		zwscn_set_statelog_end_op_del(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
	}
	
	return;
l_err:
	return;
}

ZWSCN_STATUS_T zwscn_remove_actions_by_node_id(void *net, uint8_t node_id, zwscn_scene_cont_p scene_ptr) {
	zwscn_list_t action_list;
	zwscn_list_foreach_data_t list_foreach_data;
	uint32_t scene_id;
	
	zwscn_message_log_debug("ENTER zwscn_remove_actions_by_node_id net=%p node_id=%d\n", net, node_id);
	if ((net == NULL) || (node_id == 0) || (scene_ptr == NULL)) {
		zwscn_message_log_critical("zwscn_remove_actions_by_node_id input args NULL\n");
		goto l_err_args;
	}
	
	list_foreach_data.net = net;
	list_foreach_data.scene_id = scene_ptr->scene->id;
	list_foreach_data.node_id = node_id;
	list_foreach_data.list_ptr = &(scene_ptr->list_actions);
	action_list = scene_ptr->list_actions;
	scene_id = scene_ptr->scene->id;
	zwscn_set_statelog_start_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
	zwscn_list_foreach(action_list, zwscn_remove_actions_by_node_id_list_callback, &list_foreach_data);
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
}

void zwscn_remove_actions_by_node_id_list_callback(void *list_item, void *user_data) {
	void *net;
	zwscn_scene_cont_p scene_ptr;
	zwscn_list_t *action_list_ptr;
	zwscn_action_p action_ptr;
	uint8_t action_node_id;
	uint32_t scene_id;
	uint8_t node_id;
	zwscn_list_foreach_data_t *list_foreach_data_ptr;
	
	zwscn_message_log_debug("ENTER zzwscn_remove_actions_by_node_id_list_callback\n");
	if((list_item == NULL) || (user_data == NULL)) {
		zwscn_message_log_critical("zwscn_remove_actions_by_node_id_list_callback input args NULL\n");
		goto l_err;
	}
	
	action_ptr = (zwscn_action_p) list_item;
	list_foreach_data_ptr = (zwscn_list_foreach_data_t *) user_data;
	net = list_foreach_data_ptr->net;
	scene_id = list_foreach_data_ptr->scene_id;
	node_id = list_foreach_data_ptr->node_id;
	action_list_ptr = list_foreach_data_ptr->list_ptr;
	if(action_list_ptr == NULL) {
		zwscn_message_log_critical("zwscn_remove_actions_by_node_id_list_callback action_list_ptr NULL\n");
		goto l_err;
	}
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	action_node_id = zwscn_util_combo_id_to_node_id(action_ptr->id);
	zwscn_message_log_debug("checking action id=%d action_node_id=%d node_id=%d\n", action_ptr->id, action_node_id, node_id);
	if(action_node_id == node_id) {
		(*action_list_ptr) = zwscn_list_remove((*action_list_ptr), action_ptr);
		zwscn_device_report_cache_del(net, action_ptr->id);
		zwscn_status_update_remove_pending_reqs_on_action_delete(net, scene_ptr, action_ptr);
		zwscn_free_action(action_ptr);
		zwscn_message_log_debug("Action Removed\n");
	}
	
l_err:
	return;
}

ZWSCN_STATUS_T zwscn_remove_events_by_node_id(void *net, uint8_t node_id, zwscn_scene_cont_p scene_ptr) {
	zwscn_list_t event_list;
	zwscn_list_foreach_data_t list_foreach_data;
	uint32_t scene_id;
	
	zwscn_message_log_debug("ENTER zwscn_remove_events_by_node_id net=%p node_id=%d\n", net, node_id);
	if ((net == NULL) || (node_id == 0) || (scene_ptr == NULL)) {
		zwscn_message_log_critical("zwscn_remove_events_by_node_id input args NULL\n");
		goto l_err_args;
	}
	
	list_foreach_data.node_id = node_id;
	list_foreach_data.list_ptr = &(scene_ptr->list_events);
	event_list = scene_ptr->list_events;
	scene_id = scene_ptr->scene->id;
	zwscn_set_statelog_start_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
	zwscn_list_foreach(event_list, zwscn_remove_events_by_node_id_list_callback, &list_foreach_data);
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
}

void zwscn_remove_events_by_node_id_list_callback(void *list_item, void *user_data) {
	zwscn_list_t *event_list_ptr;
	zwscn_event_p event_ptr;
	uint8_t event_node_id;
	uint8_t node_id;
	zwscn_list_foreach_data_t *list_foreach_data_ptr;
	
	zwscn_message_log_debug("ENTER zzwscn_remove_events_by_node_id_list_callback\n");
	if((list_item == NULL) || (user_data == NULL)) {
		zwscn_message_log_critical("zwscn_remove_events_by_node_id_list_callback input args NULL\n");
		goto l_err;
	}
	
	event_ptr = (zwscn_event_p) list_item;
	list_foreach_data_ptr = (zwscn_list_foreach_data_t *) user_data;
	node_id = list_foreach_data_ptr->node_id;
	event_list_ptr = list_foreach_data_ptr->list_ptr;
	if(event_list_ptr == NULL) {
		zwscn_message_log_critical("zwscn_remove_events_by_node_id_list_callback event_list_ptr NULL\n");
		goto l_err;
	}
	
	event_node_id = zwscn_util_combo_id_to_node_id(event_ptr->id);
	zwscn_message_log_debug("checking event id=%d event_node_id=%d node_id=%d\n", event_ptr->id, event_node_id, node_id);
	if(event_node_id == node_id) {
		(*event_list_ptr) = zwscn_list_remove((*event_list_ptr), event_ptr);
		zwscn_free_event(event_ptr);
		zwscn_message_log_debug("Event Removed\n");
	}
	
l_err:
	return;
}

//Function for Node Update, Network Update and Replace Failed node
ZWSCN_STATUS_T zwscn_network_node_update_notification_processor(void *net, uint8_t node_id) {
	
	if (net == NULL) { //if node_id == 0 then it is a network level notification
		goto l_err_args;
	}
	
	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_validate_scenes_on_notification(net, node_id) != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("zwscn_validate_scenes_on_notification return error\n");
		goto l_err;
	}
	if(zwscn_validate_security_scenes_on_notification(net, node_id) != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("zwscn_validate_security_scenes_on_notification return error\n");
		goto l_err;
	}
	zwscn_network_unlock_writer(net);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	zwscn_network_unlock_writer(net);
	return ZWSCN_ERR_GENERIC;
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	return ZWSCN_ERR_INVALID_NETWORK;
}

ZWSCN_STATUS_T zwscn_validate_scenes_on_notification(void *net, uint8_t node_id) {
//	zwscn_list_t *scene_list_ptr;
	zwscn_list_t scene_list;
	zwscn_list_foreach_data_t list_foreach_data;
	zwscn_init_p init_data;
	
	zwscn_message_log_debug("ENTER zwscn_validate_scenes_on_notification net=%p node_id=%d\n", net, node_id);
	if (net == NULL) { //if node_id == 0 then it is a network level notification
		zwscn_message_log_critical("zwscn_validate_scenes_on_notification input args NULL\n");
		goto l_err_args;
	}
	
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		goto l_err;
	}
	
	list_foreach_data.net = net;
	list_foreach_data.node_id = node_id;
	
	list_foreach_data.list_ptr = &(init_data->list_scenes);
	list_foreach_data.list_ptr_dest = &(init_data->list_scenes_invalid);
	scene_list = init_data->list_scenes;
	zwscn_list_foreach(scene_list, zwscn_validate_scenes_from_valid_list_callback, &list_foreach_data);
	
	list_foreach_data.list_ptr = &(init_data->list_scenes_invalid);
	list_foreach_data.list_ptr_dest = &(init_data->list_scenes);
	scene_list = init_data->list_scenes_invalid;
	zwscn_list_foreach(scene_list, zwscn_validate_scenes_from_invalid_list_callback, &list_foreach_data);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;
}

void zwscn_validate_scenes_from_valid_list_callback(void *list_item, void *user_data) {
	void *net;
	zwscn_list_t *scene_list_ptr;
	zwscn_list_t *invalid_scene_list_ptr;
	zwscn_scene_cont_p scene_ptr;
	uint8_t node_id;
	uint32_t scene_id;
	int initial_count_actions;
	int count_actions;
	zwscn_list_foreach_data_t *list_foreach_data_ptr;
	ZWSCN_STATUS_T result;
	
	zwscn_message_log_debug("ENTER zwscn_validate_scenes_from_valid_list_callback\n");
	if((list_item == NULL) || (user_data == NULL)) {
		zwscn_message_log_critical("zwscn_validate_scenes_from_valid_list_callback input args NULL\n");
		goto l_err;
	}
	
	scene_ptr = (zwscn_scene_cont_p) list_item;
	list_foreach_data_ptr = (zwscn_list_foreach_data_t *) user_data;
	net = list_foreach_data_ptr->net;
	node_id = list_foreach_data_ptr->node_id;
	scene_list_ptr = list_foreach_data_ptr->list_ptr;
	invalid_scene_list_ptr = list_foreach_data_ptr->list_ptr_dest;
	
	initial_count_actions = zwscn_list_size(scene_ptr->list_actions);
	result = zwscn_validate_actions(net, node_id, scene_ptr);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_critical("[Warning] (%d) returned by zwscn_validate_actions for scene id=%d\n", result, scene_ptr->scene->id);
		goto l_err;
	}
	
	result = zwscn_validate_events(net, node_id, scene_ptr);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_critical("[Warning] (%d) returned by zwscn_validate_events for scene id=%d\n", result, scene_ptr->scene->id);
		goto l_err;
	}
	
	scene_id = scene_ptr->scene->id;
	count_actions = zwscn_list_size(scene_ptr->list_actions);
	zwscn_message_log_debug("Scene id=%d has %d valid actions left\n", scene_ptr->scene->id, count_actions);
	if((initial_count_actions == 0) || (count_actions > 0)) { //If the Actions count was zero initially for some reason then we don't move this Scene
		if(count_actions > initial_count_actions) {
//			zwscn_set_statelog_start_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM); 
//			if(zwscn_status_update_scene_req_start(net, scene_ptr, ZWSCN_FALSE, ZWSCN_FALSE) != ZWSCN_ERR_NONE) { //ZWSCN_FALSE = polling no, ZWSCN_FALSE =  cache no
//				goto l_err_status;
//			}
			result = zwscn_status_update_request_message_send(net, scene_id, 0, ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_SCENE_ON_CHANGE);
			if(result != ZWSCN_ERR_NONE) {
				zwscn_message_log_warning("(%d) retuned by zwscn_status_update_request_message_send\n", result);
				goto l_err_status;
			}
//			zwscn_set_statelog_end_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
		} else {
			//do nothing
		}
	} else {
		zwscn_message_log_debug("Moving Scene %d to invalid list...\n", scene_id);
		zwscn_set_statelog_start_op_del(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
		(*scene_list_ptr) = zwscn_list_remove((*scene_list_ptr), scene_ptr);
		(*invalid_scene_list_ptr) = zwscn_list_add((*invalid_scene_list_ptr), scene_ptr);
		zwscn_message_log_debug("Scene moved to invalid list\n");
		zwscn_set_statelog_end_op_del(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
	}
	
	return;
l_err:
	return;
l_err_status:
//	zwscn_set_statelog_end_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
	return;
}

void zwscn_validate_scenes_from_invalid_list_callback(void *list_item, void *user_data) {
	void *net;
	zwscn_list_t *scene_list_ptr;
	zwscn_list_t *invalid_scene_list_ptr;
	zwscn_scene_cont_p scene_ptr;
	uint8_t node_id;
	uint32_t scene_id;
//	int initial_count_actions;
	int count_actions;
	zwscn_list_foreach_data_t *list_foreach_data_ptr;
	ZWSCN_STATUS_T result;
	
	zwscn_message_log_debug("ENTER zwscn_validate_scenes_from_invalid_list_callback\n");
	if((list_item == NULL) || (user_data == NULL)) {
		zwscn_message_log_critical("zwscn_validate_scenes_from_invalid_list_callback input args NULL\n");
		goto l_err;
	}
	
	scene_ptr = (zwscn_scene_cont_p) list_item;
	list_foreach_data_ptr = (zwscn_list_foreach_data_t *) user_data;
	net = list_foreach_data_ptr->net;
	node_id = list_foreach_data_ptr->node_id;
	scene_list_ptr = list_foreach_data_ptr->list_ptr_dest;
	invalid_scene_list_ptr = list_foreach_data_ptr->list_ptr;
	
//	initial_count_actions = zwscn_list_size(scene_ptr->list_actions);
	result = zwscn_validate_actions(net, node_id, scene_ptr);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_critical("[Warning] (%d) returned by zwscn_validate_actions for scene id=%d\n", result, scene_ptr->scene->id);
		goto l_err;
	}
	
	result = zwscn_validate_events(net, node_id, scene_ptr);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_critical("[Warning] (%d) returned by zwscn_validate_events for scene id=%d\n", result, scene_ptr->scene->id);
		goto l_err;
	}
	
	count_actions = zwscn_list_size(scene_ptr->list_actions);
	zwscn_message_log_debug("Scene id=%d has %d valid actions left\n", scene_ptr->scene->id, count_actions);
	if(count_actions > 0) {
		scene_id = scene_ptr->scene->id;
		if(zwscn_is_list_full_scene(net, (*scene_list_ptr))) {
			zwscn_message_log_warning("net=%p has its Scene list full. Can't move valid Scenes from invalid list to valid list\n", net);
			goto l_err_list_full;
		}
		zwscn_message_log_debug("Moving Scene %d to valid list...\n", scene_id);
		zwscn_set_statelog_start_op_add(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
		(*invalid_scene_list_ptr) = zwscn_list_remove((*invalid_scene_list_ptr), scene_ptr);
		(*scene_list_ptr) = zwscn_list_add((*scene_list_ptr), scene_ptr);
		zwscn_message_log_debug("Scene moved to valid list\n");
		zwscn_set_statelog_end_op_add(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
//		zwscn_set_statelog_start_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM); 
//		if(zwscn_status_update_scene_req_start(net, scene_ptr, ZWSCN_FALSE, ZWSCN_FALSE) != ZWSCN_ERR_NONE) { //ZWSCN_FALSE = polling no, ZWSCN_FALSE =  cache no
//			goto l_err_status;
//		}
		result = zwscn_status_update_request_message_send(net, scene_id, 0, ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_SCENE_ON_CHANGE);
		if(result != ZWSCN_ERR_NONE) {
			zwscn_message_log_warning("(%d) retuned by zwscn_status_update_request_message_send\n", result);
			goto l_err_status;
		}
//		zwscn_set_statelog_end_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
	}
	
	return;
l_err:
l_err_list_full:
	return;
l_err_status:
//	zwscn_set_statelog_end_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
	return;
}

ZWSCN_STATUS_T zwscn_validate_actions(void *net, uint8_t node_id, zwscn_scene_cont_p scene_ptr) {
	zwscn_list_t action_list;
	zwscn_list_foreach_data_t list_foreach_data;
//	uint32_t scene_id;
	zwscn_init_p init_data;
	
	zwscn_message_log_debug("ENTER zwscn_validate_actions net=%p node_id=%d\n", net, node_id);
	if ((net == NULL) || (scene_ptr == NULL)) {
		zwscn_message_log_critical("zwscn_validate_actions input args NULL\n");
		goto l_err_args;
	}
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->init_input && init_data->init_input->zwscn_fn_is_valid_handler) {
		list_foreach_data.zwscn_fn_is_valid = init_data->init_input->zwscn_fn_is_valid_handler;
	} else {
		goto l_err;
	}
	
	list_foreach_data.net = net;
	list_foreach_data.scene_id = scene_ptr->scene->id;
	list_foreach_data.node_id = node_id;
	
	list_foreach_data.list_ptr = &(scene_ptr->list_actions);
	list_foreach_data.list_ptr_dest = &(scene_ptr->list_actions_invalid);
	action_list = scene_ptr->list_actions;
	zwscn_list_foreach(action_list, zwscn_validate_actions_from_valid_list_callback, &list_foreach_data);

	list_foreach_data.list_ptr = &(scene_ptr->list_actions_invalid);
	list_foreach_data.list_ptr_dest = &(scene_ptr->list_actions);
	action_list = scene_ptr->list_actions_invalid;
	zwscn_list_foreach(action_list, zwscn_validate_actions_from_invalid_list_callback, &list_foreach_data);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;
}

void zwscn_validate_actions_from_valid_list_callback(void *list_item, void *user_data) {
	void *net;
	zwscn_scene_cont_p scene_ptr;
	zwscn_list_t *action_list_ptr;
	zwscn_list_t *invalid_action_list_ptr;
	zwscn_action_p action_ptr;
	uint8_t action_node_id;
	uint8_t action_ep_id;
	uint16_t action_cls_id;
	uint32_t scene_id;
	uint8_t node_id;
	zwscn_list_foreach_data_t *list_foreach_data_ptr;
	zwscn_fn_is_valid_t	func_is_valid;
	int result;
	
	zwscn_message_log_debug("ENTER zwscn_validate_actions_from_valid_list_callback\n");
	if((list_item == NULL) || (user_data == NULL)) {
		zwscn_message_log_critical("zwscn_validate_actions_from_valid_list_callback input args NULL\n");
		goto l_err;
	}
	
	action_ptr = (zwscn_action_p) list_item;
	list_foreach_data_ptr = (zwscn_list_foreach_data_t *) user_data;
	net = list_foreach_data_ptr->net;
	scene_id = list_foreach_data_ptr->scene_id;
	node_id = list_foreach_data_ptr->node_id;
	action_list_ptr = list_foreach_data_ptr->list_ptr;
	invalid_action_list_ptr = list_foreach_data_ptr->list_ptr_dest;
	func_is_valid = list_foreach_data_ptr->zwscn_fn_is_valid;
	if((action_list_ptr == NULL) || (invalid_action_list_ptr == NULL) || (func_is_valid == NULL)) {
		zwscn_message_log_critical("zwscn_validate_actions_from_valid_list_callback list_foreach_data_ptr members NULL\n");
		goto l_err;
	}
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	action_node_id = zwscn_util_combo_id_to_node_id(action_ptr->id);
	action_ep_id = zwscn_util_combo_id_to_ep_id(action_ptr->id);
	action_cls_id = zwscn_util_combo_id_to_cls_id(action_ptr->id);
	zwscn_message_log_debug("checking action id=%d action_node_id=%d node_id=%d\n", action_ptr->id, action_node_id, node_id);
	if((node_id == 0) || (action_node_id == node_id)) {
		result = func_is_valid(net, ZWSCN_CHECK_NODE_EP_CLS, action_node_id, action_ep_id, action_cls_id);
		zwscn_message_log_debug("[2] zwscn_fn_is_valid_handler returned %d for node=%d ep=%d cls=%d\n", result, action_node_id, action_ep_id, action_cls_id);
		if(result == ZWSCN_FALSE) {
			zwscn_set_statelog_start_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
			(*action_list_ptr) = zwscn_list_remove((*action_list_ptr), action_ptr);
			zwscn_status_update_remove_pending_reqs_on_action_delete(net, scene_ptr, action_ptr);
			(*invalid_action_list_ptr) = zwscn_list_add((*invalid_action_list_ptr), action_ptr);
			zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
		}
	}
	
l_err:
	return;
}

void zwscn_validate_actions_from_invalid_list_callback(void *list_item, void *user_data) {
	void *net;
	zwscn_list_t *action_list_ptr;
	zwscn_list_t *invalid_action_list_ptr;
	zwscn_action_p action_ptr;
	uint8_t action_node_id;
	uint8_t action_ep_id;
	uint16_t action_cls_id;
	uint32_t scene_id;
	uint8_t node_id;
	zwscn_list_foreach_data_t *list_foreach_data_ptr;
	zwscn_fn_is_valid_t	func_is_valid;
	int result;
	
	zwscn_message_log_debug("ENTER zwscn_validate_actions_from_invalid_list_callback\n");
	if((list_item == NULL) || (user_data == NULL)) {
		zwscn_message_log_critical("zwscn_validate_actions_from_invalid_list_callback input args NULL\n");
		goto l_err;
	}
	
	action_ptr = (zwscn_action_p) list_item;
	list_foreach_data_ptr = (zwscn_list_foreach_data_t *) user_data;
	net = list_foreach_data_ptr->net;
	scene_id = list_foreach_data_ptr->scene_id;
	node_id = list_foreach_data_ptr->node_id;
	action_list_ptr = list_foreach_data_ptr->list_ptr_dest;
	invalid_action_list_ptr = list_foreach_data_ptr->list_ptr;
	func_is_valid = list_foreach_data_ptr->zwscn_fn_is_valid;
	if((action_list_ptr == NULL) || (invalid_action_list_ptr == NULL) || (func_is_valid == NULL)) {
		zwscn_message_log_critical("zwscn_validate_actions_from_invalid_list_callback list_foreach_data_ptr members NULL\n");
		goto l_err;
	}
	
	action_node_id = zwscn_util_combo_id_to_node_id(action_ptr->id);
	action_ep_id = zwscn_util_combo_id_to_ep_id(action_ptr->id);
	action_cls_id = zwscn_util_combo_id_to_cls_id(action_ptr->id);
	zwscn_message_log_debug("checking action id=%d action_node_id=%d node_id=%d\n", action_ptr->id, action_node_id, node_id);
	if((node_id == 0) || (action_node_id == node_id)) {
		result = func_is_valid(net, ZWSCN_CHECK_NODE_EP_CLS, action_node_id, action_ep_id, action_cls_id);
		zwscn_message_log_debug("[3] zwscn_fn_is_valid_handler returned %d for node=%d ep=%d cls=%d\n", result, action_node_id, action_ep_id, action_cls_id);
		if(result == ZWSCN_TRUE) {
			if(zwscn_is_list_full_action(net, (*action_list_ptr))) {
				zwscn_message_log_warning("Scene id=%d action list is full. Can't move valid actions from invalid list to valid list\n", scene_id);
				goto l_err_list_full;
			}
			zwscn_set_statelog_start_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
			(*invalid_action_list_ptr) = zwscn_list_remove((*invalid_action_list_ptr), action_ptr);
			(*action_list_ptr) = zwscn_list_add((*action_list_ptr), action_ptr);
			zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
		}
	}
	
l_err:
l_err_list_full:
	return;

}

ZWSCN_STATUS_T zwscn_validate_events(void *net, uint8_t node_id, zwscn_scene_cont_p scene_ptr) {
	zwscn_list_t event_list;
	zwscn_list_foreach_data_t list_foreach_data;
//	uint32_t scene_id;
	zwscn_init_p init_data;
	
	zwscn_message_log_debug("ENTER zwscn_validate_events net=%p node_id=%d\n", net, node_id);
	if ((net == NULL) || (scene_ptr == NULL)) {
		zwscn_message_log_critical("zwscn_validate_events input args NULL\n");
		goto l_err_args;
	}
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->init_input && init_data->init_input->zwscn_fn_is_valid_handler) {
		list_foreach_data.zwscn_fn_is_valid = init_data->init_input->zwscn_fn_is_valid_handler;
	} else {
		goto l_err;
	}
	
	list_foreach_data.net = net;
	list_foreach_data.scene_id = scene_ptr->scene->id;
	list_foreach_data.node_id = node_id;
	
	list_foreach_data.list_ptr = &(scene_ptr->list_events);
	list_foreach_data.list_ptr_dest = &(scene_ptr->list_events_invalid);
	event_list = scene_ptr->list_events;
	zwscn_list_foreach(event_list, zwscn_validate_events_from_valid_list_callback, &list_foreach_data);

	list_foreach_data.list_ptr = &(scene_ptr->list_events_invalid);
	list_foreach_data.list_ptr_dest = &(scene_ptr->list_events);
	event_list = scene_ptr->list_events_invalid;
	zwscn_list_foreach(event_list, zwscn_validate_events_from_invalid_list_callback, &list_foreach_data);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;
}

void zwscn_validate_events_from_valid_list_callback(void *list_item, void *user_data) {
	void *net;
	zwscn_list_t *event_list_ptr;
	zwscn_list_t *invalid_event_list_ptr;
	zwscn_event_p event_ptr;
	uint8_t event_node_id;
	uint8_t event_ep_id;
	uint16_t event_cls_id;
	uint32_t scene_id;
	uint8_t node_id;
	zwscn_list_foreach_data_t *list_foreach_data_ptr;
	zwscn_fn_is_valid_t	func_is_valid;
	int result;
	
	zwscn_message_log_debug("ENTER zwscn_validate_events_from_valid_list_callback\n");
	if((list_item == NULL) || (user_data == NULL)) {
		zwscn_message_log_critical("zwscn_validate_events_from_valid_list_callback input args NULL\n");
		goto l_err;
	}
	
	event_ptr = (zwscn_event_p) list_item;
	list_foreach_data_ptr = (zwscn_list_foreach_data_t *) user_data;
	net = list_foreach_data_ptr->net;
	scene_id = list_foreach_data_ptr->scene_id;
	node_id = list_foreach_data_ptr->node_id;
	event_list_ptr = list_foreach_data_ptr->list_ptr;
	invalid_event_list_ptr = list_foreach_data_ptr->list_ptr_dest;
	func_is_valid = list_foreach_data_ptr->zwscn_fn_is_valid;
	if((event_list_ptr == NULL) || (invalid_event_list_ptr == NULL) || (func_is_valid == NULL)) {
		zwscn_message_log_critical("zwscn_validate_events_from_valid_list_callback list_foreach_data_ptr members NULL\n");
		goto l_err;
	}
	
	event_node_id = zwscn_util_combo_id_to_node_id(event_ptr->id);
	event_ep_id = zwscn_util_combo_id_to_ep_id(event_ptr->id);
	event_cls_id = zwscn_util_combo_id_to_cls_id(event_ptr->id);
	zwscn_message_log_debug("checking event id=%d event_node_id=%d node_id=%d\n", event_ptr->id, event_node_id, node_id);
	if((node_id == 0) || (event_node_id == node_id)) {
		result = func_is_valid(net, ZWSCN_CHECK_NODE_EP_CLS, event_node_id, event_ep_id, event_cls_id);
		if(result == ZWSCN_FALSE) {
			zwscn_set_statelog_start_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
			(*event_list_ptr) = zwscn_list_remove((*event_list_ptr), event_ptr);
			(*invalid_event_list_ptr) = zwscn_list_add((*invalid_event_list_ptr), event_ptr);
			zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
		}
	}
	
l_err:
	return;
}

void zwscn_validate_events_from_invalid_list_callback(void *list_item, void *user_data) {
	void *net;
	zwscn_list_t *event_list_ptr;
	zwscn_list_t *invalid_event_list_ptr;
	zwscn_event_p event_ptr;
	uint8_t event_node_id;
	uint8_t event_ep_id;
	uint16_t event_cls_id;
	uint32_t scene_id;
	uint8_t node_id;
	zwscn_list_foreach_data_t *list_foreach_data_ptr;
	zwscn_fn_is_valid_t	func_is_valid;
	int result;
	
	zwscn_message_log_debug("ENTER zwscn_validate_events_from_invalid_list_callback\n");
	if((list_item == NULL) || (user_data == NULL)) {
		zwscn_message_log_critical("zwscn_validate_events_from_invalid_list_callback input args NULL\n");
		goto l_err;
	}
	
	event_ptr = (zwscn_event_p) list_item;
	list_foreach_data_ptr = (zwscn_list_foreach_data_t *) user_data;
	net = list_foreach_data_ptr->net;
	scene_id = list_foreach_data_ptr->scene_id;
	node_id = list_foreach_data_ptr->node_id;
	event_list_ptr = list_foreach_data_ptr->list_ptr_dest;
	invalid_event_list_ptr = list_foreach_data_ptr->list_ptr;
	func_is_valid = list_foreach_data_ptr->zwscn_fn_is_valid;
	if((event_list_ptr == NULL) || (invalid_event_list_ptr == NULL) || (func_is_valid == NULL)) {
		zwscn_message_log_critical("zwscn_validate_events_from_invalid_list_callback list_foreach_data_ptr members NULL\n");
		goto l_err;
	}
	
	event_node_id = zwscn_util_combo_id_to_node_id(event_ptr->id);
	event_ep_id = zwscn_util_combo_id_to_ep_id(event_ptr->id);
	event_cls_id = zwscn_util_combo_id_to_cls_id(event_ptr->id);
	zwscn_message_log_debug("checking event id=%d event_node_id=%d node_id=%d\n", event_ptr->id, event_node_id, node_id);
	if((node_id == 0) || (event_node_id == node_id)) {
		result = func_is_valid(net, ZWSCN_CHECK_NODE_EP_CLS, event_node_id, event_ep_id, event_cls_id);
		if(result == ZWSCN_TRUE) {
			if(zwscn_is_list_full_event(net, (*event_list_ptr))) {
				zwscn_message_log_warning("Scene id=%d event list is full. Can't move valid events from invalid list to valid list\n", scene_id);
				goto l_err_list_full;
			}
			zwscn_set_statelog_start_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
			(*invalid_event_list_ptr) = zwscn_list_remove((*invalid_event_list_ptr), event_ptr);
			(*event_list_ptr) = zwscn_list_add((*event_list_ptr), event_ptr);
			zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
		}
	}
	
l_err:
l_err_list_full:
	return;
}
