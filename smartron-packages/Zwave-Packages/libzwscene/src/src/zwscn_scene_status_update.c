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
@file		zwscn_scene_status_update.c  
  
                To provide support for Scene status update
  
@author		Arif Mohammand
 
@version	1.0 2013-12-05  Initial release
*/

#include <stdlib.h>
#include <stdint.h>
#include "zwscn_defines.h"
#include "zwscn_scene_api.h"
#include "zwscn_security_scene_api.h"
#include "zwscn_util.h"
#include "zwscn_util_hash.h"
#include "zwscn_util_list.h"
#include "zwscn_util_message_log.h"
#include "zwscn_util_mqueue.h"
#include "zwscn_util_mqueue_message.h"
#include "zwscn_util_thread.h"
#include "zwscn_util_mutex.h"
#include "zwscn_util_rwlock.h"
#include "zwscn_util_timer.h"
#include "zwscn_statelog.h"
#include "zwscn_scene.h"
#include "zwscn_util_mem.h"
#include "zwscn_scene_status_update.h"
#include "zwscn_util_zwave.h"
#include "zwscn_zwpoll.h"
#include "zwscn_util_time.h"
#include "zwscn_device_report_cache.h"

static ZWSCN_STATUS_T zwscn_status_update_send_get_op_requests(void *net, zwscn_list_t list_get_requests);

ZWSCN_STATUS_T zwscn_status_update_request_message_send(void *net, uint32_t scene_id, uint32_t action_id, uint8_t message_type) {
	zwscn_mqueue_t mqueue_status_update_request_handler;
	zwscn_mqueue_message_p message;
	
	if(net == NULL) {
		goto l_err_args;
	} 
	
	message = zwscn_mqueue_message_new(NULL);
	if(message ==  NULL) {
		goto l_err_message;
	}
	message->net = net;
	message->type = message_type;
	message->sub_type = scene_id; //Using field sub-type to store scene_id so that the there is no need to create a new field
	if(action_id > 0) {
		uint32_t *tmp = malloc(sizeof(uint32_t));
		if(tmp == NULL) {
			zwscn_message_log_critical("Memory allocation failed\n");
			zwscn_free_mqueue_message(message);
			goto l_err_message;
		}
		(*tmp) = action_id;
		message->data = tmp;
		message->data_size = sizeof(uint32_t);
	}
	
	zwscn_mqueue_lock(net);
	if(zwscn_find_mqueue_thread_status_update_request_handler(net, &mqueue_status_update_request_handler) == ZWSCN_ERR_NONE) {
		if(mqueue_status_update_request_handler && (zwscn_mqueue_length(mqueue_status_update_request_handler) < ZWSCN_MQUEUE_LENGTH_MAX_STATUS_UPDATE_REQUEST_HANDLER)) {
			ZWSCN_MESSAGE_LOG_DEBUG("sending message to status update request handler thread for net=%p scene_id=%u action_id=%u type=%d\n", net, scene_id, action_id, message_type);
			if(message_type == ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_SCENE_ON_EXECUTE) {
				message->priority = ZWSCN_MESSAGE_PRIORITY_STATUS_UPDATE_ON_EXECUTE;
				zwscn_mqueue_message_add_by_priority(mqueue_status_update_request_handler, message, zwscn_mqueue_message_priority_sort_handler);
			} else {
				zwscn_mqueue_message_push(mqueue_status_update_request_handler, message); //Send it to status update request handler thread
			}
		} else {
			zwscn_message_log_message("MQUEUE for Status Update Request Handler is full. dropping new message...\n");
			zwscn_free_mqueue_message(message);
			zwscn_mqueue_unlock(net);
			goto l_err_queue_full;
		}
	} else {
		zwscn_message_log_message("MQUEUE for Status Update Request Handler is not found. dropping new message...\n");
		zwscn_free_mqueue_message(message);
		zwscn_mqueue_unlock(net);
		goto l_err_not_found;
	}
	zwscn_mqueue_unlock(net);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err_message:
	return ZWSCN_ERR_MEMORY;
l_err_queue_full:
	return ZWSCN_ERR_QUEUE_FULL;
l_err_not_found:
	return ZWSCN_ERR_NOT_FOUND;
}

//NOTE: this function uses zwscn_network_lock and zwscn_network_unlock
void zwscn_status_update_request_handler_thread(void *data) {
	zwscn_mqueue_t mqueue_status_update_request = NULL;
	zwscn_mqueue_message_p message = NULL;
	uint32_t message_type = 0;
	void *net = NULL;
	uint32_t scene_id = 0;
	uint32_t action_id = 0;
	zwscn_scene_cont_p scene_ptr = NULL;
	ZWSCN_STATUS_T result = ZWSCN_ERR_NONE;
	
	if(data == NULL) {
		zwscn_message_log_error("message queue is NULL. Could not start STATUS Update request handler thread\n");
		return;
	}
	zwscn_message_log_info("THREAD_STATUS_UPDATE_REQUEST_HANDLER started\n");
	mqueue_status_update_request = (zwscn_mqueue_t) data;
	
	while(1) {
		message = zwscn_mqueue_message_pop(mqueue_status_update_request); //Blocking call
		if(message != NULL) {
			zwscn_message_log_debug("STATUS Thread request handler received message with refcount=%d type=%d priority=%d\n", message->refcount, message->type, message->priority);
			if(message->type == ZWSCN_MESSAGE_TYPE_EXIT) {
				if(zwscn_mqueue_message_refcount_dec_and_test(message)) {
					zwscn_free_mqueue_message(message);
				}
				zwscn_message_log_info("STATUS Request Handler Thread exiting...\n");
				zwscn_thread_exit(NULL);
			} else {
				zwscn_list_t list_get_requests = NULL;
				
				net = message->net;
				message_type = message->type;
				scene_id = message->sub_type;
				if((message->data_size > 0) && (message->data != NULL)) {
					action_id = (*((uint32_t *)message->data));
				} else {
					action_id = 0;
				}
				zwscn_free_mqueue_message(message); //message is not needed anymore
				zwscn_set_statelog_start_op_status_update_pending(net);
				zwscn_message_log_debug("STATUS Thread request handler checking message type %d for net=%p scene=%u action_id=%u\n", message_type, net, scene_id, action_id);
				zwscn_network_lock_reader(net); //TODO - Check if status update require writers lock
				if (zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should not process the message
					if(message_type == ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_SCENE) {
						if((scene_id != 0) && (zwscn_find_scene(net, scene_id, &scene_ptr) == ZWSCN_ERR_NONE)) {
							result = zwscn_status_update_scene_req_start(net, scene_ptr, action_id, ZWSCN_FALSE, ZWSCN_TRUE, &list_get_requests); //ZWSCN_FALSE =  polling no, ZWSCN_TRUE = cache yes
							if(result != ZWSCN_ERR_NONE) {
								zwscn_message_log_critical("(%d) returned by zwscn_status_update_scene_req_start\n", result);
							}
						} else {
							zwscn_message_log_warning("Scene status update request handler could not find scene for scene id=%d\n", scene_id);
						}
					} else if(message_type == ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_SCENE_ON_CHANGE) {
						if((scene_id != 0) && (zwscn_find_scene(net, scene_id, &scene_ptr) == ZWSCN_ERR_NONE)) {
							result = zwscn_status_update_scene_req_start(net, scene_ptr, action_id, ZWSCN_FALSE, ZWSCN_FALSE, &list_get_requests); //ZWSCN_FALSE =  polling no, ZWSCN_TRUE = cache no
							if(result != ZWSCN_ERR_NONE) {
								zwscn_message_log_critical("(%d) returned by zwscn_status_update_scene_req_start\n", result);
							}
						} else {
							zwscn_message_log_warning("Scene status update request handler could not find scene for scene id=%d\n", scene_id);
						}
					} else if(message_type == ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_SCENE_ON_EXECUTE) {
						if((scene_id != 0) && (zwscn_find_scene(net, scene_id, &scene_ptr) == ZWSCN_ERR_NONE)) {
//							result = zwscn_status_update_scene_req_start(net, scene_ptr, action_id, ZWSCN_TRUE, ZWSCN_FALSE, &list_get_requests); //ZWSCN_TRUE =  polling yes, ZWSCN_TRUE = cache no
							result = zwscn_status_update_scene_req_start(net, scene_ptr, action_id, ZWSCN_FALSE, ZWSCN_FALSE, &list_get_requests); //ZWSCN_FALSE =  polling no, ZWSCN_TRUE = cache no - lower layers do the polling now
							if(result != ZWSCN_ERR_NONE) {
								zwscn_message_log_critical("(%d) returned by zwscn_status_update_scene_req_start\n", result);
							}
						} else {
							zwscn_message_log_warning("Scene status update request handler could not find scene for scene id=%d\n", scene_id);
						}
					} else if(message_type == ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_ALL) {
						result = zwscn_status_update_all_req_start(net, &list_get_requests);
						if(result != ZWSCN_ERR_NONE) {
							zwscn_message_log_critical("(%d) returned by zwscn_status_update_all_req_start\n", result);
						}
					}
				} else {
					zwscn_message_log_warning("net=%p does not exist\n", net);
				}
				zwscn_network_unlock_reader(net);
				
				if(list_get_requests != NULL) {
					if(zwscn_status_update_send_get_op_requests(net, list_get_requests) == ZWSCN_ERR_CALLBACK_RESULT) { //We have to free args memory if callback error
						zwscn_get_op_request_p op_get_request = NULL;
						int count = 0;
						int i = 0;
						
						count = zwscn_list_size(list_get_requests); //If list is NULL, count should be 0
						for(i=0; i < count; ++i) {
							op_get_request = (zwscn_get_op_request_p) zwscn_list_get_by_index(list_get_requests, i);
							if(op_get_request) {
								free(op_get_request->args);
							}
						}
					}
					zwscn_list_free(&list_get_requests);
				}
				zwscn_status_update_reset_pending_flag(net);
			}
		}
	}
}

ZWSCN_STATUS_T zwscn_status_update_all_req_start(void *net, zwscn_list_t *list_get_requests_ptr) {
	zwscn_list_t *scene_list;
	zwscn_scene_cont_p scene_ptr = NULL;
	int count = 0;
	int index = 0;
	ZWSCN_STATUS_T result =  ZWSCN_ERR_NONE;
	 
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if(net == NULL) {
		goto l_err_args;
	}
	
	if(zwscn_find_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	count = zwscn_list_size((*scene_list));
	zwscn_message_log_debug("sending status update request for %d scenes for net=%p\n", count, net);
	for(index=0; index < count; ++index) {
		scene_ptr = zwscn_list_get_by_index((*scene_list), index);
		result = zwscn_status_update_scene_req_start(net, scene_ptr, 0, ZWSCN_FALSE, ZWSCN_TRUE, list_get_requests_ptr); //ZWSCN_FALSE =  polling no, ZWSCN_TRUE = cache yes
		if(result != ZWSCN_ERR_NONE) {
			zwscn_message_log_warning("(%d) returned by zwscn_status_update_scene_req_start\n");
			//Continue to the next scene
		}
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_status_update_scene_req_start(void *net, zwscn_scene_cont_p scene_ptr, uint32_t action_id, ZWSCN_BOOL_T flag_is_poll_req, ZWSCN_BOOL_T flag_use_cache, zwscn_list_t *list_get_requests_ptr) {
	zwscn_list_t action_list;
	zwscn_action_p action_ptr = NULL;
	int count = 0;
	int i = 0;
	int count_reqs = 0;
	ZWSCN_STATUS_T result =  ZWSCN_ERR_NONE;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if((net == NULL) || (scene_ptr == NULL)) {
		goto l_err_args;
	} 
	
	action_list = scene_ptr->list_actions;
	count = zwscn_list_size(action_list);
	zwscn_message_log_debug("sending status update request for %d actions for net=%p scene_id=%d\n", count, net, scene_ptr->scene->id);
	//Moved after for loop
//	if(count > 0) {
//		zwscn_status_update_start_timer(net, scene_ptr, (ZWSCN_STATUS_UPDATE_REQ_TIMEOUT + (count * ZWSCN_STATUS_UPDATE_REQ_TIMEOUT_PER_ACTION)));
//	}
	for(i=0; i < count; ++i) {
		//TODO Abort functionality
		action_ptr = zwscn_list_get_by_index(action_list, i);
		
		if((action_id == 0) || (action_id == action_ptr->id)) {
			if((flag_is_poll_req == ZWSCN_FALSE) && (flag_use_cache == ZWSCN_TRUE) && zwscn_device_report_cache_is_fresh(net, action_ptr->id)) {
				action_ptr->sutime = ZWSCN_TIME_NOW(); //update the timestamp (sutime = NOW()) so that timer callback does not mark it as Unknown
				continue; //continue with the next action
			}
			 
			result = zwscn_status_update_action_req_start(net, scene_ptr, action_ptr, flag_is_poll_req, list_get_requests_ptr);
			if(result == ZWSCN_ERR_CALLBACK_NULL) { //Callback is null so no need to try other actions
				break;
			} else if(result != ZWSCN_ERR_NONE) {
				zwscn_message_log_warning("(%d) returned by zwscn_status_update_action_req_start for net=%p scene=%d action=0x%X\n", net, scene_ptr->scene->id, action_ptr->id);
				continue; //continue with the next action
			}
			
			++count_reqs;
		}
	}

	if(count_reqs > 0) { 
		zwscn_message_log_debug("Starting status_update_start_timer with net=%p scene_id=%u action_id=%u count_reqs=%d\n", net, scene_ptr->scene->id, action_id, count_reqs);
		//zwscn_status_update_start_timer(net, scene_ptr, action_id, (ZWSCN_STATUS_UPDATE_REQ_TIMEOUT + (count_reqs * ZWSCN_STATUS_UPDATE_REQ_TIMEOUT_PER_ACTION)));
		zwscn_status_update_start_timer(net, scene_ptr, action_id, count_reqs);
	}

	return result;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
}

ZWSCN_STATUS_T zwscn_status_update_action_req_start(void *net, zwscn_scene_cont_p scene_ptr, zwscn_action_p action_ptr, ZWSCN_BOOL_T flag_is_poll_req, zwscn_list_t *list_get_requests_ptr) {
//	uint8_t node_id, ep_id;
	uint8_t cmd;
//	uint16_t cls_id;
	zwscn_device_zwpoll_req_t *zwscn_zwpoll_ptr = NULL;
	zwscn_init_p init_data;
	void *args=NULL;
//	int result;
	zwscn_get_op_request_p op_get_report_request = NULL;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if((net == NULL) || (scene_ptr == NULL) || (action_ptr == NULL)) {
		goto l_err_args;
	}
	
//	node_id = zwscn_util_combo_id_to_node_id(action_ptr->id);
//	ep_id = zwscn_util_combo_id_to_ep_id(action_ptr->id);
//	cls_id = zwscn_util_combo_id_to_cls_id(action_ptr->id);
	//cmd = zwscn_zwave_find_cmd_get(cls_id, action_ptr->cmd); //The conversion is done by the get_op_handler now
	cmd = action_ptr->cmd;
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->init_input && init_data->init_input->zwscn_function_get_op_handler) {
		args = zwscn_zwave_prepare_get_op_args(net, action_ptr);
		zwscn_zwpoll_ptr = (flag_is_poll_req == ZWSCN_TRUE) ? zwscn_zwpoll_prepare_req(net, scene_ptr, action_ptr) : NULL;
//		result = init_data->init_input->zwscn_function_get_op_handler(net, node_id, ep_id, cls_id, cmd, args, zwscn_zwpoll_ptr);
//		if(result < 0) {
//			zwscn_message_log_critical("Function get_op_handler return error (%d) for Scene_id=%d Action_id=0x%X\n", result, scene_ptr->scene->id, action_ptr->id);
//			goto l_err_callback;
//		}
		////////////////////////////
		op_get_report_request = calloc(1, sizeof(zwscn_get_op_request_t));
		if(op_get_report_request == NULL) {
			goto l_err_memory;
		}
		op_get_report_request->scene_id = scene_ptr->scene->id;
		op_get_report_request->action_id = action_ptr->id;
		op_get_report_request->cmd = cmd;
		op_get_report_request->args = args;
		op_get_report_request->zwscn_zwpoll_ptr = zwscn_zwpoll_ptr;
		op_get_report_request->func_get_op = init_data->init_input->zwscn_function_get_op_handler;
		
		(*list_get_requests_ptr) = zwscn_list_add((*list_get_requests_ptr), op_get_report_request);
		////////////////////////////

		if(zwscn_zwpoll_ptr != NULL) {
			zwscn_zwpoll_remove_req(net, scene_ptr, action_ptr); //Remove all other polls for thos action_id (token) as we are about to start a new one
			zwscn_mutex_lock(init_data->mutex_pending_report_status);
			action_ptr->pending_zwpoll_reqs += zwscn_zwpoll_ptr->poll_cnt;
			scene_ptr->pending_report_reqs += zwscn_zwpoll_ptr->poll_cnt;
			init_data->pending_report_reqs += zwscn_zwpoll_ptr->poll_cnt;
			zwscn_mutex_unlock(init_data->mutex_pending_report_status);
			ZWSCN_MESSAGE_LOG_DEBUG("REPORTCOUNT 1 action r=%u zr=%u\tscene[%d]=%u\tnetwork=%u\n", action_ptr->pending_report_reqs, action_ptr->pending_zwpoll_reqs, scene_ptr->scene->id, scene_ptr->pending_report_reqs, init_data->pending_report_reqs);
		} else {
			zwscn_mutex_lock(init_data->mutex_pending_report_status); 
			++(action_ptr->pending_report_reqs);
			++(scene_ptr->pending_report_reqs);
			++(init_data->pending_report_reqs);
			zwscn_mutex_unlock(init_data->mutex_pending_report_status);
			ZWSCN_MESSAGE_LOG_DEBUG("REPORTCOUNT 2 action r=%u zr=%u\tscene[%d]=%u\tnetwork=%u\n", action_ptr->pending_report_reqs, action_ptr->pending_zwpoll_reqs, scene_ptr->scene->id, scene_ptr->pending_report_reqs, init_data->pending_report_reqs);
		}
	} else {
		zwscn_message_log_warning("Function get_op_handler is NULL for Scene_id=%d Action_id=0x%X\n", scene_ptr->scene->id, action_ptr->id);
		goto l_err_callback_null;
	}
	
	//TODO do something with the returned handle value in zwscn_zwpoll_ptr
	//free(zwscn_zwpoll_ptr); //Passing NULL to free is OK according to C standard
	
	return ZWSCN_ERR_NONE;

l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
//l_err_callback:
//	return ZWSCN_ERR_CALLBACK_RESULT;
l_err_callback_null:
	return ZWSCN_ERR_CALLBACK_NULL;
l_err_memory:
	return ZWSCN_ERR_MEMORY;
}

//NOTE: this function uses zwscn_network_lock and zwscn_network_unlock
void zwscn_status_update_report_handler_thread(void *data) {
	zwscn_mqueue_t mqueue_status_update;
	zwscn_mqueue_message_p message;
	void *net;
	ZWSCN_STATUS_T result;
	
	if(data == NULL) {
		zwscn_message_log_error("message queue is NULL. Could not start STATUS Update thread\n");
		return;
	}
	zwscn_message_log_info("THREAD_STATUS_UPDATE_REPORT_HANDLER started\n");
	mqueue_status_update = (zwscn_mqueue_t) data;
	
	while(1) {
		message = zwscn_mqueue_message_pop(mqueue_status_update); //Blocking call
		if(message != NULL) {
			zwscn_message_log_debug("STATUS Thread received message with refcount=%d type=%d priority=%d\n", message->refcount, message->type, message->priority);
			if(message->type == ZWSCN_MESSAGE_TYPE_EXIT) {
				if(zwscn_mqueue_message_refcount_dec_and_test(message)) {
					zwscn_free_mqueue_message(message);
				}
				zwscn_message_log_info("STATUS Thread exiting...\n");
				zwscn_thread_exit(NULL);
			} else {
				net = message->net;
				zwscn_network_lock_reader(net); //TODO - Check if status update require writers lock
				if (zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should not process the message
					result = zwscn_status_update_process_report(net, message->type, message->sub_type, message->data);
					if(result != ZWSCN_ERR_NONE) {
						zwscn_message_log_critical("[Warning] (%d) returned by zwscn_status_update_process_report\n", result);
					}
				}
				zwscn_network_unlock_reader(net);
				if(zwscn_mqueue_message_refcount_dec_and_test(message)) {
					zwscn_free_mqueue_message(message);
				}
				zwscn_status_update_abort_flag_clear(net); //Clear the flag before goign to block again for the message
			}
		}
	}
}

//NOTE: this function uses zwscn_network_lock and zwscn_network_unlock
int zwscn_status_update_on_timeout_callback(void *user_data) {
	zwscn_status_update_timer_info_p su_timer_info_ptr = NULL;
	zwscn_mqueue_message_p message;
	void *net;
	uint32_t scene_id = 0;
	zwscn_scene_cont_p scene_ptr = NULL;
	zwscn_time_t time_now;
	ZWSCN_STATUS_T result;
//	zwscn_list_t action_list;
//	int count = 0;
	int count_reqs = 0;
	long interval = 0;
	long diff = 0;
	uint32_t action_id = 0;
	zwscn_time_t status_update_req_start_time = 0;
	void *temp = NULL;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if(user_data == NULL) {
		goto l_err_args;
	}
	if(zwscn_timer_is_destroyed()) {
		goto l_err_timer;
	}
	
	message = (zwscn_mqueue_message_p) user_data;
	zwscn_mqueue_message_refcount_inc(message);
	net = message->net;
	scene_id = message->type; //Using type as scene_id to avoid creating new fields
	if((message->data_size > 0) && (message->data != NULL)) {
		action_id = (*((uint32_t *)message->data));
	} else {
		action_id = 0;
	}
	if(zwscn_mqueue_message_refcount_dec_and_test(message)) {
		zwscn_free_mqueue_message(message); // Free the message as no other thread is going to
		message = NULL;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("Status update timeout is for net%p scene=%d\n", net, scene_id);
	if (!zwscn_does_net_exist(net)) {
		goto l_err_network_invalid_1;
	}
	
	zwscn_network_lock_reader(net); //TODO - Check if status update require writer's lock
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	zwscn_mutex_lock(scene_ptr->mutex_status_update_timer_info);
	temp = zwscn_uint_to_pointer((ZWSCN_STATUS_UPDATE_HASH_KEY_BASE + scene_id + action_id));
	su_timer_info_ptr = zwscn_hash_get_value(scene_ptr->hash_status_update_timer_info, temp);
	if(su_timer_info_ptr) {
		su_timer_info_ptr->timer_id = 0;
		su_timer_info_ptr->timer_user_data = NULL;
		zwscn_free_mqueue_message(message); // Free the message as no other thread is going to
	}
	zwscn_mutex_unlock(scene_ptr->mutex_status_update_timer_info);
	
	if(su_timer_info_ptr != NULL) {
		status_update_req_start_time = su_timer_info_ptr->status_update_req_start_time;
		count_reqs = su_timer_info_ptr->count_reqs;
		//zwscn_status_update_cancel_timer(scene_ptr, action_id, su_timer_info_ptr); //Cancel any existing timer first
		interval = (ZWSCN_STATUS_UPDATE_REQ_TIMEOUT + (count_reqs * ZWSCN_STATUS_UPDATE_REQ_TIMEOUT_PER_ACTION));
		ZWSCN_MESSAGE_LOG_DEBUG("Found action_id=%u status_update_req_start_time=%llu\n", action_id, status_update_req_start_time);
	} else {
		ZWSCN_MESSAGE_LOG_DEBUG("NOT Found action_id=%u status_update_req_start_time=%llu\n", action_id, status_update_req_start_time);
		goto l_err;
	}
	
//	action_list = scene_ptr->list_actions;
//	count = zwscn_list_size(action_list);
//	zwscn_message_log_debug("STATUS timeout net=%p scene_id=%d actions_count=%d\n", net, scene_id, count);
//	if(count > 0) {
//		if(action_id == 0) {
//			count_reqs = count;
//		} else {
//			count_reqs = 1; //there was single action update requested
//		}
//		interval = (ZWSCN_STATUS_UPDATE_REQ_TIMEOUT + (count_reqs * ZWSCN_STATUS_UPDATE_REQ_TIMEOUT_PER_ACTION)); //(count * ZWSCN_STATUS_UPDATE_REQ_TIMEOUT_PER_ACTION);
//	}

	zwscn_message_log_debug("STATUS timeout net=%p scene_id=%d count_reqs=%d\n", net, scene_id, count_reqs);

	time_now = ZWSCN_TIME_NOW();
	diff = time_now - status_update_req_start_time;
	zwscn_message_log_debug("STATUS interval=%ld time diff=%ld \n", interval, diff);
	if((status_update_req_start_time == 0) || (interval > diff)) {
		zwscn_message_log_warning("Exiting without status update on timeout because of time condition\n");
		goto l_err;
	}
	result = zwscn_status_update_scene_on_timeout(net, scene_ptr, action_id, ZWSCN_TRUE); //flag_update_other_scenes = true
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_critical("(%d) returned by zwscn_status_update_scene_on_timeout\n", result);
		goto l_err;
	}
//	scene_ptr->status_update_req_start_time = 0;
//	scene_ptr->timer_user_data = NULL;
//	scene_ptr->timer_id = 0;
	zwscn_network_unlock_reader(net);
	
	return 0; //Dismiss the timer
l_err_args:
l_err_timer:
l_err_network_invalid_1:
	return 0; //Dismiss the timer
l_err:
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	return 0; //Dismiss the timer
}

ZWSCN_STATUS_T zwscn_status_update_process_report(void *net, uint32_t id, uint32_t cmd, void *report) {
	zwscn_list_t *scene_list;
	zwscn_scene_cont_p scene_ptr;
	int count=0;
	int i=0;
	ZWSCN_STATUS_T result;
	
	if(net == NULL) {
		goto l_err_args;
	} 
	
	if(zwscn_find_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	count = zwscn_list_size((*scene_list));
	for(i=0; i < count; ++i) {
		if(zwscn_status_update_abort_flag_get(net)) {
			ZWSCN_MESSAGE_LOG_DEBUG("Aborting status update operation net=%p\n", net);
			goto l_err_abort;
		}
		
		scene_ptr = zwscn_list_get_by_index((*scene_list), i);
		result = zwscn_status_update_scene_on_report(net, scene_ptr, id, cmd, report);
		if(result != ZWSCN_ERR_NONE) {
			zwscn_message_log_critical("(%d) returned by zwscn_status_update_scene_on_report for scene id=%d\n", result, scene_ptr->scene->id);
		}
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;	
l_err_abort:
	return ZWSCN_ERR_ABORT;
}

ZWSCN_STATUS_T zwscn_status_update_scene_on_report(void *net, zwscn_scene_cont_p scene_ptr, uint32_t id, uint32_t cmd, void *report) {
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	return zwscn_status_update_scene_common(net, scene_ptr, id, cmd, report, ZWSCN_TRUE, ZWSCN_FALSE); //flag_status_update_on_report = ZWSCN_TRUE, flag_status_update_on_report
}

ZWSCN_STATUS_T zwscn_status_update_scene_on_timeout(void *net, zwscn_scene_cont_p scene_ptr, uint32_t action_id, ZWSCN_BOOL_T flag_update_other_scenes) {
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	return zwscn_status_update_scene_common(net, scene_ptr, action_id, 0, NULL, ZWSCN_FALSE, flag_update_other_scenes); //flag_status_update_on_report = ZWSCN_FALSE
}

ZWSCN_STATUS_T zwscn_status_update_scene_common(void *net, zwscn_scene_cont_p scene_ptr, uint32_t id, uint32_t cmd, void *report, ZWSCN_BOOL_T flag_status_update_on_report, ZWSCN_BOOL_T flag_update_other_scenes) {
	ZWSCN_STATUS_T result;
	zwscn_list_t action_list;
	zwscn_action_p action_ptr;
	int count=0;
	int i=0;
	ZWSCN_BOOL_T scene_has_updated_action = ZWSCN_FALSE;
	
	if((net == NULL) || (scene_ptr == NULL)) {
		goto l_err_args;
	} 
	
	action_list = scene_ptr->list_actions;
	count = zwscn_list_size(action_list); //If list is NULL, count should be 0
	//zwscn_message_log_debug("STATUS checking scene id=%d with %d actions\n", scene_ptr->scene->id, count);
	for(i=0; i < count; ++i) {
		if(zwscn_status_update_abort_flag_get(net)) {
			ZWSCN_MESSAGE_LOG_DEBUG("Aborting status update operation net=%p\n", net);
			goto l_err_abort;
		}
		
		action_ptr = zwscn_list_get_by_index(action_list, i);
		if(action_ptr) { 
			zwscn_message_log_debug("STATUS updating scene=%d action=0x%X cmd=%d\n", scene_ptr->scene->id, action_ptr->id, action_ptr->cmd);
			if(flag_status_update_on_report == ZWSCN_TRUE) {
				if(action_ptr->id == id) {
					zwscn_message_log_debug("STATUS processing action=0x%X cmd=%d for report=0x%X\n", action_ptr->id, action_ptr->cmd, id);
					result = zwscn_status_update_action_on_report(net, scene_ptr, action_ptr, id, cmd, report);
				} else {
					//Action does not match
					zwscn_message_log_debug("STATUS ignoring action=0x%X cmd=%d for report=0x%X\n", action_ptr->id, action_ptr->cmd, id);
					continue; //check the next action
				}
			} else {
				if((id == 0) || (action_ptr->id == id)) {
					result = zwscn_status_update_action_on_timeout(net, scene_ptr, action_ptr, id, flag_update_other_scenes);
				} else {
					//Action does not match
					zwscn_message_log_debug("STATUS ignoring action=0x%X for timeout\n", action_ptr->id);
					continue; //check the next action
				}
			}
			if(result != ZWSCN_ERR_NONE) {
				zwscn_message_log_debug("zwscn_status_update_action_on_xxxx return (%d)\n", result);
				continue; //check the next action
			} 
			scene_has_updated_action = ZWSCN_TRUE;
			//Action is ACTIVE - if it is the last executed scene, we may remove any existing ZWPoll for this ifd
			if((action_ptr->status == ZWSCN_STATUS_ACTIVE) && (zwscn_is_last_executed_scene(net, scene_ptr))) {
				zwscn_message_log_debug("Scene id=%d Action id=0x%X is ACTIVE. Removing exsiting ZWPoll for this action.\n", scene_ptr->scene->id, action_ptr->id);
				zwscn_zwpoll_remove_req(net, scene_ptr, action_ptr);
			}
		} else {
			zwscn_message_log_critical("[Warning] Action %d is NULL in scene=%u\n", i, scene_ptr->scene->id);
		}
	}
	
	if(scene_has_updated_action == ZWSCN_TRUE) {
		zwscn_status_update_scene_for_current_action_status(net, scene_ptr);
	} else {
		zwscn_message_log_debug("STATUS scene \"%s\" not updated. Retaining old status\n", scene_ptr->scene->name);
	}
	zwscn_message_log_debug("\tstatus=%d sutime=%llu\n", scene_ptr->scene->status, scene_ptr->scene->sutime);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
//l_err:
//	return ZWSCN_ERR_GENERIC;	
l_err_abort:
	return ZWSCN_ERR_ABORT;
}


ZWSCN_STATUS_T zwscn_status_update_action_on_report(void *net, zwscn_scene_cont_p scene_ptr, zwscn_action_p action_ptr, uint32_t id, uint32_t cmd, void *report) {
	ZWSCN_BOOL_T new_status;
	zwscn_time_t sutime = 0;
	ZWSCN_STATUS_T result;
	zwscn_init_p init_data = NULL;
	 
	if((net == NULL) || (scene_ptr == NULL) || (action_ptr == NULL) || (report == NULL)) {
		goto l_err_args;
	}
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		goto l_err;
	}
	
	 result = zwscn_zwave_compare_cc(net, action_ptr->device_args, id, cmd, report, &new_status, &sutime);
	 if(result == ZWSCN_ERR_NONE) {
//		if(action_ptr->status != ZWSCN_STATUS_UNKNOWN) {
//			action_ptr->lkstatus = action_ptr->status;
//			action_ptr->lksutime = action_ptr->sutime;
//		}
//		action_ptr->status = new_status;
//		action_ptr->sutime = sutime;
		zwscn_status_update_action(action_ptr, new_status, sutime);
		ZWSCN_MESSAGE_LOG_DEBUG("scene=%d action=0x%X status=%d sutime=%llu\n", scene_ptr->scene->id, action_ptr->id, action_ptr->status, action_ptr->sutime);
		if(action_ptr->pending_zwpoll_reqs > 0) {
			zwscn_mutex_lock(init_data->mutex_pending_report_status);
			--(action_ptr->pending_zwpoll_reqs);
			if(scene_ptr->pending_report_reqs > 0) {
				--(scene_ptr->pending_report_reqs);
			}
			if(init_data->pending_report_reqs > 0) {
				--(init_data->pending_report_reqs);
			}
			zwscn_mutex_unlock(init_data->mutex_pending_report_status);
			zwscn_status_update_reset_pending_flag(net); //this function locks mutex_pending_report_status
			ZWSCN_MESSAGE_LOG_DEBUG("REPORTCOUNT 3 action r=%u zr=%u\tscene[%d]=%u\tnetwork=%u\n", action_ptr->pending_report_reqs, action_ptr->pending_zwpoll_reqs, scene_ptr->scene->id, scene_ptr->pending_report_reqs, init_data->pending_report_reqs);			
		} else if(action_ptr->pending_report_reqs > 0) {
			zwscn_mutex_lock(init_data->mutex_pending_report_status);
			--(action_ptr->pending_report_reqs);
			if(scene_ptr->pending_report_reqs > 0) {
				--(scene_ptr->pending_report_reqs);
			}
			if(init_data->pending_report_reqs > 0) {
				--(init_data->pending_report_reqs);
			}
			zwscn_mutex_unlock(init_data->mutex_pending_report_status);
			zwscn_status_update_reset_pending_flag(net); //this function locks mutex_pending_report_status
			ZWSCN_MESSAGE_LOG_DEBUG("REPORTCOUNT 4 action r=%u zr=%u\tscene[%d]=%u\tnetwork=%u\n", action_ptr->pending_report_reqs, action_ptr->pending_zwpoll_reqs, scene_ptr->scene->id, scene_ptr->pending_report_reqs, init_data->pending_report_reqs);			
		}
	 } else if (result == ZWSCN_ERR_NOT_FOUND) {
		 goto l_err_not_found;
	 } else {
		 zwscn_message_log_info("(%d) returned by zwscn_zwave_status_update_action\n", result);
		 action_ptr->status = ZWSCN_STATUS_UNKNOWN;
		 action_ptr->sutime = ZWSCN_TIME_NOW();
		 goto l_err;
	 }
	 if(sutime == 0){
		zwscn_message_log_debug("Action sutime=%llu. Reverting to ZWSCN_STATUS_UNKNOWN for status\n", sutime);
		action_ptr->status = ZWSCN_STATUS_UNKNOWN;
		action_ptr->sutime = ZWSCN_TIME_NOW();
		goto l_err;
	}
	 
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;
l_err_not_found:
	return ZWSCN_ERR_NOT_FOUND;
}

ZWSCN_STATUS_T zwscn_status_update_action_on_timeout(void *net, zwscn_scene_cont_p scene_ptr, zwscn_action_p action_ptr, uint32_t hash_key_action_id, ZWSCN_BOOL_T flag_update_other_scenes) {
	zwscn_status_update_timer_info_p su_timer_info_ptr = NULL;
	zwscn_time_t status_update_req_start_time = 0;
	ZWSCN_BOOL_T new_status;
	zwscn_time_t sutime = 0;
//	ZWSCN_STATUS_T result;
	zwscn_init_p init_data = NULL;
	void *temp = NULL;

	if((net == NULL) || (scene_ptr == NULL) || (scene_ptr->scene == NULL) || (action_ptr == NULL)) {
		goto l_err_args;
	}
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		goto l_err;
	}
	 
	zwscn_mutex_lock(scene_ptr->mutex_status_update_timer_info);
	temp = zwscn_uint_to_pointer((ZWSCN_STATUS_UPDATE_HASH_KEY_BASE + scene_ptr->scene->id +(hash_key_action_id)));
	su_timer_info_ptr = zwscn_hash_get_value(scene_ptr->hash_status_update_timer_info, temp);
	zwscn_mutex_unlock(scene_ptr->mutex_status_update_timer_info);
	if(su_timer_info_ptr != NULL) {
		status_update_req_start_time = su_timer_info_ptr->status_update_req_start_time;
		ZWSCN_MESSAGE_LOG_DEBUG("Found action_id=%u status_update_req_start_time=%llu\n", hash_key_action_id, status_update_req_start_time);
	} else {
		ZWSCN_MESSAGE_LOG_DEBUG("NOT Found action_id=%u status_update_req_start_time=%llu\n", hash_key_action_id, status_update_req_start_time);
	}
	 ZWSCN_MESSAGE_LOG_DEBUG("scene=%d scene_update_req_start_time=%llu\n", scene_ptr->scene->id, status_update_req_start_time);
	 ZWSCN_MESSAGE_LOG_DEBUG("action=0x%X action_sutime=%llu\n", action_ptr->id, action_ptr->sutime);
	 if((action_ptr->sutime < status_update_req_start_time) || (flag_update_other_scenes != ZWSCN_TRUE)) { //when called by zwscn_status_update_other_scenes_for_same_action(), flag_update_other_scenes == ZWSCN_FALSE
		 new_status = ZWSCN_STATUS_UNKNOWN;
		 sutime = ZWSCN_TIME_NOW();
	 
//		if(action_ptr->status != ZWSCN_STATUS_UNKNOWN) {
//			action_ptr->lkstatus = action_ptr->status;
//			action_ptr->lksutime = action_ptr->sutime;
//		}
//		action_ptr->status = new_status;
//		action_ptr->sutime = sutime;
		zwscn_status_update_action(action_ptr, new_status, sutime);
		ZWSCN_MESSAGE_LOG_DEBUG("scene=%d action=0x%X status=%d sutime=%llu\n", scene_ptr->scene->id, action_ptr->id, action_ptr->status, action_ptr->sutime);
		if(action_ptr->pending_report_reqs > 0) {
			ZWSCN_MESSAGE_LOG_DEBUG("REPORTCOUNT 14 clearing r=%u for action=0x%x\tscene_id=%d\tnetwork=0x%x\n", action_ptr->pending_report_reqs, action_ptr->id, scene_ptr->scene->id, net); 
			zwscn_mutex_lock(init_data->mutex_pending_report_status);
			if(scene_ptr->pending_report_reqs > action_ptr->pending_report_reqs) {
				scene_ptr->pending_report_reqs -= action_ptr->pending_report_reqs;
			} else {
				scene_ptr->pending_report_reqs = 0;
			}
			if(init_data->pending_report_reqs > action_ptr->pending_report_reqs) {
				init_data->pending_report_reqs -= action_ptr->pending_report_reqs;
			} else {
				init_data->pending_report_reqs = 0;
			}
			action_ptr->pending_report_reqs = 0; //timeout occured so we make action unknown and do not wait for normal reports
			zwscn_mutex_unlock(init_data->mutex_pending_report_status);
			zwscn_status_update_reset_pending_flag(net); //this function locks mutex_pending_report_status
			ZWSCN_MESSAGE_LOG_DEBUG("REPORTCOUNT 5 action r=%u zr=%u\tscene[%d]=%u\tnetwork=%u\n", action_ptr->pending_report_reqs, action_ptr->pending_zwpoll_reqs, scene_ptr->scene->id, scene_ptr->pending_report_reqs, init_data->pending_report_reqs);			
		}
		
		if(flag_update_other_scenes == ZWSCN_TRUE) {
			zwscn_status_update_other_scenes_for_same_action(net, scene_ptr, action_ptr);
		}
	 } else {
		//Even if the action was updated after this timer was started, we still need to clear pending reports because of this timeout
		if(action_ptr->pending_report_reqs > 0) {
			ZWSCN_MESSAGE_LOG_DEBUG("REPORTCOUNT 15 clearing r=%u for action=0x%x\tscene_id=%d\tnetwork=0x%x\n", action_ptr->pending_report_reqs, action_ptr->id, scene_ptr->scene->id, net); 			
			zwscn_mutex_lock(init_data->mutex_pending_report_status);
			if(scene_ptr->pending_report_reqs > action_ptr->pending_report_reqs) {
				scene_ptr->pending_report_reqs -= action_ptr->pending_report_reqs;
			} else {
				scene_ptr->pending_report_reqs = 0;
			}
			if(init_data->pending_report_reqs > action_ptr->pending_report_reqs) {
				init_data->pending_report_reqs -= action_ptr->pending_report_reqs;
			} else {
				init_data->pending_report_reqs = 0;
			}
			action_ptr->pending_report_reqs = 0; //timeout occured so we make action unknown and do not wait for normal reports
			zwscn_mutex_unlock(init_data->mutex_pending_report_status);
			zwscn_status_update_reset_pending_flag(net); //this function locks mutex_pending_report_status
			ZWSCN_MESSAGE_LOG_DEBUG("REPORTCOUNT 9 action r=%u zr=%u\tscene[%d]=%u\tnetwork=%u\n", action_ptr->pending_report_reqs, action_ptr->pending_zwpoll_reqs, scene_ptr->scene->id, scene_ptr->pending_report_reqs, init_data->pending_report_reqs);			
		}
		goto l_err_ignored;
	 }
	 
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err_ignored:
	return ZWSCN_ERR_ITEM_IGNORED;
l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_status_update_action(zwscn_action_p action_ptr, ZWSCN_BOOL_T new_status, zwscn_time_t sutime) {
	if(action_ptr == NULL) {
		goto l_err_args;
	}
	
	if(action_ptr->status != ZWSCN_STATUS_UNKNOWN) {
		action_ptr->lkstatus = action_ptr->status;
		action_ptr->lksutime = action_ptr->sutime;
	}
	action_ptr->status = new_status;
	action_ptr->sutime = sutime;
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
}

ZWSCN_STATUS_T zwscn_status_update_other_scenes_for_same_action(void *net, zwscn_scene_cont_p scene_ptr, zwscn_action_p action_ptr) {
	zwscn_list_t *scene_list = NULL;
	zwscn_scene_cont_p scene_ptr_temp = NULL;
	int index=0;
	int count=0;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if((net == NULL) || (scene_ptr == NULL) || (scene_ptr->scene == NULL) || (action_ptr == NULL)) {
		goto l_err_args;
	}
	
	if(zwscn_find_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	count = zwscn_list_size((*scene_list));
	ZWSCN_MESSAGE_LOG_DEBUG("Found %d scenes\n", count);
	for(index=0; index < count; ++index) {
		scene_ptr_temp = zwscn_list_get_by_index((*scene_list), index);
		if((scene_ptr_temp != NULL) && (scene_ptr_temp->scene != NULL) && (scene_ptr_temp != scene_ptr)) {
			ZWSCN_MESSAGE_LOG_DEBUG("Checking Scene[%u]=\"%s\" for action_id=%u\n", scene_ptr_temp->scene->id, scene_ptr_temp->scene->name, action_ptr->id);
			zwscn_status_update_scene_on_timeout(net, scene_ptr_temp, action_ptr->id, ZWSCN_FALSE); //flag_update_other_scenes = false
		}
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
//l_err_ignored:
//	return ZWSCN_ERR_ITEM_IGNORED;
l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_status_update_scene_for_current_action_status(void *net, zwscn_scene_cont_p scene_ptr) {
//	ZWSCN_STATUS_T result;
	zwscn_list_t action_list;
	zwscn_action_p action_ptr;
	ZWSCN_BOOL_T scene_has_unknown_action = ZWSCN_FALSE;
	int scene_status = ZWSCN_STATUS_UNKNOWN;
	int count=0;
	int i=0;
	
	if((net == NULL) || (scene_ptr == NULL)) {
		goto l_err_args;
	} 
	
	action_list = scene_ptr->list_actions;
	count = zwscn_list_size(action_list); //If list is NULL, count should be 0
	zwscn_message_log_debug("STATUS checking scene id=%d with %d actions\n", scene_ptr->scene->id, count);
	for(i=0; i < count; ++i) {
		action_ptr = zwscn_list_get_by_index(action_list, i);
		
		ZWSCN_MESSAGE_LOG_DEBUG("scene=%d action=0x%X status=%d sutime=%llu\n", scene_ptr->scene->id, action_ptr->id, action_ptr->status, action_ptr->sutime);
		if (action_ptr->status == ZWSCN_STATUS_ACTIVE) {
			if (scene_status != ZWSCN_STATUS_INACTIVE) { //if it is already false, do not reset it
				scene_status = ZWSCN_STATUS_ACTIVE;
			}
		} else if (action_ptr->status == ZWSCN_STATUS_INACTIVE) {
			scene_status = ZWSCN_STATUS_INACTIVE;
		} else if (action_ptr->status == ZWSCN_STATUS_UNKNOWN) {
			if (scene_status != ZWSCN_STATUS_INACTIVE) { //if it is already false, do not reset it
				scene_status = ZWSCN_STATUS_UNKNOWN;
			}
			scene_has_unknown_action =  ZWSCN_TRUE;
		}

		if((scene_has_unknown_action == ZWSCN_TRUE) && (scene_status != ZWSCN_STATUS_INACTIVE)) {
			scene_status = ZWSCN_STATUS_UNKNOWN;
		}
	}
	
	zwscn_message_log_debug("STATUS updating status of scene \"%s\"\n", scene_ptr->scene->name);
	zwscn_set_statelog_start_op_status_update_on_report(net, 0, ZWSCN_STATELOG_TRIGGER_SYSTEM);
	zwscn_set_statelog_end_op_status_update_on_report(net, 0, ZWSCN_STATELOG_TRIGGER_SYSTEM);
	zwscn_set_statelog_start_op_status_update_on_report(net, scene_ptr->scene->id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
	if(scene_ptr->scene->status != ZWSCN_STATUS_UNKNOWN) {
		scene_ptr->scene->lkstatus = scene_ptr->scene->status;
		scene_ptr->scene->lksutime = scene_ptr->scene->sutime;
	}
	scene_ptr->scene->status = scene_status;
	scene_ptr->scene->sutime = ZWSCN_TIME_NOW();
	ZWSCN_MESSAGE_LOG_DEBUG("scene=%d status=%d sutime=%llu\n", scene_ptr->scene->id, scene_ptr->scene->status, scene_ptr->scene->sutime);
	zwscn_set_statelog_end_op_status_update_on_report(net, scene_ptr->scene->id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
}

ZWSCN_STATUS_T zwscn_status_update_start_timer(void *net, zwscn_scene_cont_p scene_ptr, uint32_t action_id, uint32_t count_reqs) {
	zwscn_mqueue_message_p message;
	zwscn_status_update_timer_info_p su_timer_info_ptr = NULL;
	uint32_t time_interval = 0;
	void *temp = NULL;
	
	zwscn_message_log_debug("ENTER zwscn_status_update_start_timer\n");
	if((net == NULL) || (scene_ptr == NULL) || (scene_ptr->scene == NULL) || (count_reqs == 0)) {
		goto l_err_args;
	}
	
	zwscn_mutex_lock(scene_ptr->mutex_status_update_timer_info);
	temp = zwscn_uint_to_pointer((ZWSCN_STATUS_UPDATE_HASH_KEY_BASE + scene_ptr->scene->id + action_id));
	su_timer_info_ptr = zwscn_hash_get_value(scene_ptr->hash_status_update_timer_info, temp);
//	temp = zwscn_uint_to_pointer((ZWSCN_STATUS_UPDATE_HASH_KEY_BASE + scene_ptr->scene->id + action_id));
	zwscn_hash_remove(scene_ptr->hash_status_update_timer_info, temp);
	zwscn_mutex_unlock(scene_ptr->mutex_status_update_timer_info);
	
	if(su_timer_info_ptr != NULL) {
		zwscn_status_update_cancel_timer(scene_ptr, action_id, su_timer_info_ptr); //Cancel any existing timer first
	}
	
	su_timer_info_ptr = calloc(1, sizeof(zwscn_status_update_timer_info_t));
	if(su_timer_info_ptr == NULL) {
		goto l_err_memory;
	}
	
	message = zwscn_mqueue_message_new(NULL);
	if(message ==  NULL) {
		goto l_err_memory_2;
	}
	zwscn_mqueue_message_refcount_inc(message);
	message->net = net;
	message->type = scene_ptr->scene->id; //Using type as scene_id to avoid creating new fields
	if(action_id > 0) {
		uint32_t *tmp = malloc(sizeof(uint32_t));
		if(tmp == NULL) {
			zwscn_message_log_critical("Memory allocation failed\n");
			zwscn_free_mqueue_message(message);
			goto l_err_memory;
		}
		(*tmp) = action_id;
		message->data = tmp;
		message->data_size = sizeof(uint32_t);
	}
	
	time_interval = (ZWSCN_STATUS_UPDATE_REQ_TIMEOUT + (count_reqs * ZWSCN_STATUS_UPDATE_REQ_TIMEOUT_PER_ACTION));
	su_timer_info_ptr->status_update_req_start_time = ZWSCN_TIME_NOW();
	su_timer_info_ptr->count_reqs = count_reqs;
	su_timer_info_ptr->timer_user_data = message;
	su_timer_info_ptr->timer_id = zwscn_timer_new(time_interval, zwscn_status_update_on_timeout_callback, message);
	zwscn_mutex_lock(scene_ptr->mutex_status_update_timer_info);
	temp = zwscn_uint_to_pointer((ZWSCN_STATUS_UPDATE_HASH_KEY_BASE + scene_ptr->scene->id + action_id));
	zwscn_hash_insert(scene_ptr->hash_status_update_timer_info, temp, su_timer_info_ptr);
	zwscn_mutex_unlock(scene_ptr->mutex_status_update_timer_info);
	zwscn_message_log_debug("New timer started with id=%d for scene_id=%d action_id=%u at status_update_req_start_time=%llu\n", su_timer_info_ptr->timer_id, scene_ptr->scene->id, action_id, su_timer_info_ptr->status_update_req_start_time);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err_memory:
	return ZWSCN_ERR_MEMORY;
l_err_memory_2:
	free(su_timer_info_ptr);
	return ZWSCN_ERR_MEMORY;
}

ZWSCN_STATUS_T zwscn_status_update_cancel_timer(zwscn_scene_cont_p scene_ptr, uint32_t action_id, zwscn_status_update_timer_info_p status_update_timer_info_ptr) {
	zwscn_mqueue_message_p message = NULL;
	
	zwscn_message_log_debug("ENTER zwscn_status_update_cancel_timer\n");
	if(status_update_timer_info_ptr == NULL) {
		goto l_err_args;
	}
	
	if(status_update_timer_info_ptr->timer_id) {
		zwscn_message_log_debug("Scene_id=%d action_id=%u has timer id=%d. Cancelling it...\n", scene_ptr->scene->id, action_id, status_update_timer_info_ptr->timer_id);
		zwscn_timer_delete(status_update_timer_info_ptr->timer_id);
		message = (zwscn_mqueue_message_p) status_update_timer_info_ptr->timer_user_data;
		if(message && zwscn_mqueue_message_refcount_dec_and_test(message)) {
			zwscn_free_mqueue_message(message); // Free the message as no other thread is going to
//			message = NULL; 
		}
		status_update_timer_info_ptr->status_update_req_start_time = 0;
		status_update_timer_info_ptr->timer_user_data = NULL;
		status_update_timer_info_ptr->timer_id = 0;
	}
	free(status_update_timer_info_ptr);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
}

ZWSCN_STATUS_T zwscn_status_update_abort_flag_set(void *net) {
	zwscn_init_p init_data;
	
	if(net == NULL) {
		goto l_err_args;
	}
	
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		goto l_err;
	}
	
	if(zwscn_status_update_abort_flag_set_2(init_data) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_status_update_abort_flag_set_2(zwscn_init_p init_data) {
	if(init_data == NULL) {
		goto l_err_args;
	}
	
	zwscn_mutex_lock(init_data->mutex_abort_status_update_op);
	init_data->flag_abort_status_update_op = ZWSCN_TRUE;
	zwscn_mutex_unlock(init_data->mutex_abort_status_update_op);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
}

ZWSCN_STATUS_T zwscn_status_update_abort_flag_clear(void *net) {
	zwscn_init_p init_data;
	
	if(net == NULL) {
		goto l_err_args;
	}
	
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		goto l_err;
	}
	
	zwscn_mutex_lock(init_data->mutex_abort_status_update_op);
	init_data->flag_abort_status_update_op = ZWSCN_FALSE;
	zwscn_mutex_unlock(init_data->mutex_abort_status_update_op);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_BOOL_T zwscn_status_update_abort_flag_get(void *net) {
	zwscn_init_p init_data;
	ZWSCN_BOOL_T result = ZWSCN_FALSE;
	
	if(net == NULL) {
		goto l_err_args;
	}
	
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		goto l_err;
	}
	
	zwscn_mutex_lock(init_data->mutex_abort_status_update_op);
	result = init_data->flag_abort_status_update_op;
	zwscn_mutex_unlock(init_data->mutex_abort_status_update_op);
	
	return result;
l_err_args:
l_err:
	return ZWSCN_FALSE; //return FALSE on error
}

static ZWSCN_STATUS_T zwscn_status_update_send_get_op_requests(void *net, zwscn_list_t list_get_requests) {
	zwscn_get_op_request_p op_get_request = NULL;
	zwscn_fn_get_op_t func_get_op = NULL;
//	void *args = NULL;
	uint8_t node_id, ep_id, cmd;
	uint16_t cls_id;
	int count = 0;
	int i = 0;
	int result = 0;
	
	
	if((net == NULL) || (list_get_requests == NULL)) {
		goto l_err_args;
	}
	
	count = zwscn_list_size(list_get_requests); //If list is NULL, count should be 0
	for(i=0; i < count; ++i) {
		op_get_request = (zwscn_get_op_request_p) zwscn_list_get_by_index(list_get_requests, i);
		if(op_get_request != NULL) {
			if(zwscn_does_net_exist(net)) {
				node_id = zwscn_util_combo_id_to_node_id(op_get_request->action_id);
				ep_id = zwscn_util_combo_id_to_ep_id(op_get_request->action_id);
				cls_id = zwscn_util_combo_id_to_cls_id(op_get_request->action_id);
				cmd = op_get_request->cmd;
				func_get_op = op_get_request->func_get_op;
				if(func_get_op != NULL) {
					result = func_get_op(net, node_id, ep_id, cls_id, cmd, op_get_request->args, op_get_request->zwscn_zwpoll_ptr);
					if(result < 0) {
						zwscn_message_log_warning("Function get_op_handler return error (%d) for Scene_id=%d Action_id=0x%X\n", result, op_get_request->scene_id, op_get_request->action_id);
//						op_get_request->args = NULL; //freed by hcaal_get (MUX layer)
//						goto l_err_callback;
//						Print the warning but move on to the next action as the next action may be for a different device which may not return error
					}
					op_get_request->args = NULL; //freed by hcaal_get (MUX layer)
				} else {
					free(op_get_request->args);
				}
			} else {
				free(op_get_request->args);
			}
			if(op_get_request && op_get_request->zwscn_zwpoll_ptr) {
				ZWSCN_MESSAGE_LOG_DEBUG("token=%u returned handle=%u", op_get_request->zwscn_zwpoll_ptr->usr_token, op_get_request->zwscn_zwpoll_ptr->handle);
			} 
			free(op_get_request->zwscn_zwpoll_ptr);
		}
	}
	
	return ZWSCN_ERR_NONE;

l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
//l_err_callback:
//	return ZWSCN_ERR_CALLBACK_RESULT;
}

//void zwscn_status_update_remove_pending_reqs_on_scene_delete(void *net, zwscn_scene_cont_p scene_ptr) {
//	zwscn_init_p init_data = NULL;
//	
//	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
//	if((net == NULL) || (scene_ptr == NULL)) {
//		goto l_err_args;
//	}
//	
//	if((init_data = zwscn_get_init_data(net)) == NULL) {
//		goto l_err_invalid_data;
//	}
//	
//	if(scene_ptr->pending_report_reqs > 0) {
//		zwscn_mutex_lock(init_data->mutex_pending_report_status);
//		if(init_data->pending_report_reqs > scene_ptr->pending_report_reqs) {
//			init_data->pending_report_reqs -= scene_ptr->pending_report_reqs;
//		} else {
//			init_data->pending_report_reqs = 0;
//		}
//		scene_ptr->pending_report_reqs = 0;
//		zwscn_mutex_unlock(init_data->mutex_pending_report_status);
//		zwscn_status_update_reset_pending_flag(net); //this function locks mutex_pending_report_status
//	}
//	
//	return;
//	
//l_err_args:
//l_err_invalid_data:
//	return;	
//}

void zwscn_status_update_remove_pending_reqs_on_action_delete_all(void *net, zwscn_scene_cont_p scene_ptr) {
	zwscn_list_t action_list;
	zwscn_action_p action_ptr;
	int count=0;
	int i=0;
	
	action_list = scene_ptr->list_actions;
	count = zwscn_list_size(action_list); //If list is NULL, count should be 0
	for(i=0; i < count; ++i) {
		action_ptr = zwscn_list_get_by_index(action_list, i);
		zwscn_status_update_remove_pending_reqs_on_action_delete(net, scene_ptr, action_ptr);
	} 
}

void zwscn_status_update_remove_pending_reqs_on_action_delete(void *net, zwscn_scene_cont_p scene_ptr, zwscn_action_p action_ptr) {
	zwscn_init_p init_data = NULL;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if((net == NULL) || (scene_ptr == NULL) || (action_ptr == NULL)) {
		goto l_err_args;
	}
	
	if((init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err_invalid_data;
	}
	
	if(action_ptr->pending_zwpoll_reqs > 0) {
		ZWSCN_MESSAGE_LOG_DEBUG("REPORTCOUNT 12 clearing zr=%u for action=0x%x\tscene_id=%d\tnetwork=0x%x\n", action_ptr->pending_zwpoll_reqs, action_ptr->id, scene_ptr->scene->id, net); 
		zwscn_mutex_lock(init_data->mutex_pending_report_status);
		if(scene_ptr->pending_report_reqs > action_ptr->pending_zwpoll_reqs) {
			scene_ptr->pending_report_reqs -= action_ptr->pending_zwpoll_reqs;
		} else {
			scene_ptr->pending_report_reqs = 0;
		}
		if(init_data->pending_report_reqs > action_ptr->pending_zwpoll_reqs) {
			init_data->pending_report_reqs -= action_ptr->pending_zwpoll_reqs;
		} else {
			init_data->pending_report_reqs = 0;
		}
		action_ptr->pending_zwpoll_reqs = 0;
		zwscn_mutex_unlock(init_data->mutex_pending_report_status);
		zwscn_status_update_reset_pending_flag(net); //this function locks mutex_pending_report_status
		ZWSCN_MESSAGE_LOG_DEBUG("REPORTCOUNT 7 action r=%u zr=%u\tscene[%d]=%u\tnetwork=%u\n", action_ptr->pending_report_reqs, action_ptr->pending_zwpoll_reqs, scene_ptr->scene->id, scene_ptr->pending_report_reqs, init_data->pending_report_reqs);
	}
	
	if(action_ptr->pending_report_reqs > 0) {
		ZWSCN_MESSAGE_LOG_DEBUG("REPORTCOUNT 13 clearing r=%u for action=0x%x\tscene_id=%d\tnetwork=0x%x\n", action_ptr->pending_report_reqs, action_ptr->id, scene_ptr->scene->id, net); 
		zwscn_mutex_lock(init_data->mutex_pending_report_status);
		if(scene_ptr->pending_report_reqs > action_ptr->pending_report_reqs) {
			scene_ptr->pending_report_reqs -= action_ptr->pending_report_reqs;
		} else {
			scene_ptr->pending_report_reqs = 0;
		}
		if(init_data->pending_report_reqs > action_ptr->pending_report_reqs) {
			init_data->pending_report_reqs -= action_ptr->pending_report_reqs;
		} else {
			init_data->pending_report_reqs = 0;
		}
		action_ptr->pending_report_reqs = 0;
		zwscn_mutex_unlock(init_data->mutex_pending_report_status);
		zwscn_status_update_reset_pending_flag(net); //this function locks mutex_pending_report_status
		ZWSCN_MESSAGE_LOG_DEBUG("REPORTCOUNT 8 action r=%u zr=%u\tscene[%d]=%u\tnetwork=%u\n", action_ptr->pending_report_reqs, action_ptr->pending_zwpoll_reqs, scene_ptr->scene->id, scene_ptr->pending_report_reqs, init_data->pending_report_reqs);		
	}
	
	return;
	
l_err_args:
l_err_invalid_data:
	return;	
}

void zwscn_status_update_reset_pending_flag(void *net) {
	zwscn_init_p init_data = NULL;
	int exec_reqs = 0;
	int status_reqs = 0;
	int pending_postset_poll_cbs = 0;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if(net == NULL) {
		goto l_err_args;
	}
	if (!zwscn_does_net_exist(net)) {
		goto l_err_network_invalid;
	}
	
	if((init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err_invalid_data;
	}
	
	zwscn_mqueue_lock(net);
	if (!zwscn_does_net_exist(net)) {
		goto l_err_network_invalid_1;
	}
	exec_reqs = zwscn_mqueue_length(init_data->mqueue_thread_execute_request_handler);
	status_reqs = zwscn_mqueue_length(init_data->mqueue_thread_status_update_request_handler);
	zwscn_mqueue_unlock(net);
	
	zwscn_mutex_lock(init_data->mutex_pending_report_status);
	if (!zwscn_does_net_exist(net)) {
		goto l_err_network_invalid_2;
	}
	pending_postset_poll_cbs = zwscn_hash_size(init_data->hash_pending_postset_poll_cb);
	ZWSCN_MESSAGE_LOG_DEBUG("exec_reqs=%d status_reqs=%d pending_report_reqs=%d pending_postset_poll_cbs=%d\n", exec_reqs, status_reqs, init_data->pending_report_reqs, pending_postset_poll_cbs);
	if((exec_reqs <= 0) && (status_reqs <= 0) && (init_data->pending_report_reqs == 0) && (pending_postset_poll_cbs == 0)) {
		zwscn_set_statelog_end_op_status_update_pending(net);
	}
	zwscn_mutex_unlock(init_data->mutex_pending_report_status);

	return;
	
l_err_args:
l_err_invalid_data:
l_err_network_invalid:
	return;
l_err_network_invalid_1:
	zwscn_mqueue_unlock(net);
	return;
l_err_network_invalid_2:
	zwscn_mutex_unlock(init_data->mutex_pending_report_status);
	return;	
}

