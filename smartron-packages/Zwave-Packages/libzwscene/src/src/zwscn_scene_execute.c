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
@file		zwscn_scene_execute.c  
  
                To provide support for Scene execution (activation)
  
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
#include "zwscn_util_time.h"
#include "zwscn_statelog.h"
#include "zwscn_util_mem.h"
#include "zwscn_scene.h"
#include "zwscn_scene_status_update.h"
#include "zwscn_util_zwave.h"
#include "zwscn_scene_execute.h"
#include "zwscn_notification_handler.h"

static void zwscn_execute_scene_info_update(zwscn_scene_cont_p scene_ptr, uint8_t last_execution_trigger);
static ZWSCN_STATUS_T zwscn_execute_send_set_op_requests(void *net, zwscn_list_t list_set_requests);
static void zwscn_execute_send_status_update_requests(void *net, zwscn_list_t list_status_update_requests);

ZWSCN_STATUS_T zwscn_execute_request_message_send(void *net, uint32_t scene_id, uint8_t message_type) {
	zwscn_mqueue_t mqueue_execute_request_handler;
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
	zwscn_mqueue_lock(net);
	if(zwscn_find_mqueue_thread_execute_request_handler(net, &mqueue_execute_request_handler) == ZWSCN_ERR_NONE) {
		if(mqueue_execute_request_handler && (zwscn_mqueue_length(mqueue_execute_request_handler) < ZWSCN_MQUEUE_LENGTH_MAX_EXECUTE_REQUEST_HANDLER)) {
			ZWSCN_MESSAGE_LOG_DEBUG("sending message to execute request handler thread for net=%p scene id=%d\n", net, scene_id);
			zwscn_mqueue_message_push(mqueue_execute_request_handler, message); //Send it to execute request handler thread
		} else {
			zwscn_message_log_message("MQUEUE for Execute Request Handler is full. dropping new message...\n");
			zwscn_free_mqueue_message(message);
		}
	} else {
		zwscn_message_log_message("MQUEUE for Execute Request Handler is not found. dropping new message...\n");
		zwscn_free_mqueue_message(message);
	}
	zwscn_mqueue_unlock(net);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err_message:
	return ZWSCN_ERR_MEMORY;
//l_err_queue_full:
//	return ZWSCN_ERR_QUEUE_FULL;
}

void zwscn_execute_request_handler_thread(void *data) {
	zwscn_mqueue_t mqueue_execute_request_handler = NULL;
	zwscn_mqueue_message_p message = NULL;
	uint32_t message_type = 0;
	void *net = NULL;
	uint32_t scene_id = 0;
	zwscn_scene_cont_p scene_ptr = NULL;
	ZWSCN_STATUS_T result = ZWSCN_ERR_NONE;
	
	if(data == NULL) {
		zwscn_message_log_error("message queue is NULL. Could not start Execute thread\n");
		return;
	}
	zwscn_message_log_info("THREAD_EXECUTE started\n");
	mqueue_execute_request_handler = (zwscn_mqueue_t) data;
	
	while(1) {
		message = zwscn_mqueue_message_pop(mqueue_execute_request_handler); //Blocking call
		if(message != NULL) {
			zwscn_message_log_debug("EXECUTE Thread received message with refcount=%d type=%d priority=%d\n", message->refcount, message->type, message->priority);
			if(message->type == ZWSCN_MESSAGE_TYPE_EXIT) {
				if(zwscn_mqueue_message_refcount_dec_and_test(message)) {
					zwscn_free_mqueue_message(message);
				}
				zwscn_message_log_info("EXECUTE Thread exiting...\n");
				zwscn_thread_exit(NULL);
			} else {
				zwscn_list_t list_set_requests = NULL;
				zwscn_list_t list_status_update_requests = NULL;
				
				net = message->net;
				message_type = message->type;
				scene_id = message->sub_type;
				zwscn_free_mqueue_message(message); //message is not needed anymore
				zwscn_set_statelog_start_op_status_update_pending(net);
				zwscn_message_log_debug("EXECUTE Thread checking message type %d for net=%p scene=%d\n", message_type, net, scene_id);
				zwscn_network_lock_reader(net); //TODO - Check if status update require writers lock
				if (zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should not process the message
					if(message_type == ZWSCN_MESSAGE_TYPE_EXECUTE_SCENE) {
						if((scene_id != 0) && (zwscn_find_scene(net, scene_id, &scene_ptr) == ZWSCN_ERR_NONE)) {
							result = zwscn_execute_and_update_scene(net, scene_ptr, ZWSCN_STATELOG_TRIGGER_MANUAL, &list_set_requests, &list_status_update_requests);
							if(result != ZWSCN_ERR_NONE) {
								zwscn_message_log_critical("(%d) returned by zwscn_execute_and_update_scene\n", result);
							}
						} else {
							zwscn_message_log_warning("Scene execute thread could not find scene for scene id=%d\n", scene_id);
						}
					} else if(message_type == ZWSCN_MESSAGE_TYPE_EXECUTE_SCENE_ON_SCHEDULE) {
						if((scene_id != 0) && (zwscn_find_scene(net, scene_id, &scene_ptr) == ZWSCN_ERR_NONE)) {
							result = zwscn_execute_and_update_scene(net, scene_ptr, ZWSCN_STATELOG_TRIGGER_SCHEDULE, &list_set_requests, &list_status_update_requests);
							if(result != ZWSCN_ERR_NONE) {
								zwscn_message_log_critical("(%d) returned by zwscn_execute_and_update_scene\n", result);
							}
						} else {
							zwscn_message_log_warning("Scene execute thread could not find scene for scene id=%d\n", scene_id);
						}
					} else if(message_type == ZWSCN_MESSAGE_TYPE_EXECUTE_SCENE_ON_EVENT) {
						if((scene_id != 0) && (zwscn_find_scene(net, scene_id, &scene_ptr) == ZWSCN_ERR_NONE)) {
							result = zwscn_execute_and_update_scene(net, scene_ptr, ZWSCN_STATELOG_TRIGGER_EVENT, &list_set_requests, &list_status_update_requests);
							if(result != ZWSCN_ERR_NONE) {
								zwscn_message_log_critical("(%d) returned by zwscn_execute_and_update_scene\n", result);
							}
						} else {
							zwscn_message_log_warning("Scene execute thread could not find scene for scene id=%d\n", scene_id);
						}
					}
				} else {
					zwscn_message_log_warning("net=%p does not exist\n", net);
				}
				zwscn_network_unlock_reader(net);
				
				if(list_set_requests != NULL) {
					if(zwscn_execute_send_set_op_requests(net, list_set_requests) == ZWSCN_ERR_CALLBACK_RESULT) { //We have to free args memory if callback error
						zwscn_set_op_request_p op_set_request = NULL;
						int count = 0;
						int i = 0;
//						ZWSCN_BOOL_T new_status;
//						zwscn_time_t sutime;
						
						count = zwscn_list_size(list_set_requests); //If list is NULL, count should be 0
//						if(count > 0) {
//							zwscn_network_lock_writer(net);
//							if (zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should not process the message
//								if((scene_id != 0) && (zwscn_find_scene(net, scene_id, &scene_ptr) == ZWSCN_ERR_NONE)) {
//									for(i=0; i < count; ++i) {
//										op_set_request = (zwscn_set_op_request_p) zwscn_list_get_by_index(list_set_requests, i);
//										if(op_set_request) {
//											if(zwscn_find_action(net, scene_id, op_set_request->action_id, &action_ptr) == ZWSCN_ERR_NONE) {
//												if(op_set_request->result == 0) { 
//													//set action status to active
//													new_status = ZWSCN_STATUS_ACTIVE;
//													sutime = ZWSCN_TIME_NOW();
//													zwscn_status_update_action(action_ptr, new_status, sutime);
//												} else {//error occurred while performing SET op
//													//TODO handle ZW_ERR_QUEUED status (for sleeping and Flirs devices)
//													
//													//set action status to unknown
//													new_status = ZWSCN_STATUS_UNKNOWN;
//													sutime = ZWSCN_TIME_NOW();
//													zwscn_status_update_action(action_ptr, new_status, sutime);
//												}
//												//TODO update other scenes which have the same action
//											}
//										}
//									}
//									//update scene based on its actions' status
//									zwscn_status_update_scene_for_current_action_status(net, scene_ptr);
//								}
//							}
//							zwscn_network_unlock_writer(net);
							
							
							for(i=0; i < count; ++i) {
								op_set_request = (zwscn_set_op_request_p) zwscn_list_get_by_index(list_set_requests, i);
								if(op_set_request) {
									free(op_set_request->args);
								}
							}
//						}
					}
					zwscn_list_free(&list_set_requests);
				} else {
					zwscn_message_log_debug("list_set_requests is NULL\n");
				}
				
				if(list_status_update_requests != NULL) {
					//zwscn_network_lock_reader(net);
					zwscn_execute_send_status_update_requests(net, list_status_update_requests);
					//zwscn_network_unlock_reader(net);
					zwscn_list_free(&list_status_update_requests);
				} else {
					zwscn_message_log_debug("list_status_update_requests is NULL\n");
					zwscn_status_update_reset_pending_flag(net);
				}
			}
		}
	}
}

ZWSCN_STATUS_T zwscn_execute_and_update_scene(void *net, zwscn_scene_cont_p scene_ptr, uint8_t exec_trigger_type, zwscn_list_t *list_set_requests_ptr, zwscn_list_t *list_status_update_requests_ptr) {
	uint32_t scene_id = 0;
	uint32_t *scene_id_ptr = NULL;
//	ZWSCN_STATUS_T result = ZWSCN_ERR_NONE;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if((net == NULL) || (scene_ptr == NULL)) {
		goto l_err_args;
	} 
	
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene id=%d trigger=%d\n", net, scene_ptr->scene->id, exec_trigger_type);
	scene_id = scene_ptr->scene->id;
	zwscn_set_statelog_start_op_exec(net, scene_id, exec_trigger_type);
	if(zwscn_execute_scene_private(net, scene_ptr, exec_trigger_type, list_set_requests_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
//	zwscn_set_statelog_start_op_status_update(net, scene_id, exec_trigger_type); 
//	if(zwscn_status_update_scene_req_start(net, scene_ptr, ZWSCN_TRUE, ZWSCN_FALSE) != ZWSCN_ERR_NONE) { //ZWSCN_TRUE = polling yes, ZWSCN_FALSE = cache no
//		goto l_err_status;
//	}

//	result = zwscn_status_update_request_message_send(net, scene_id, ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_SCENE_ON_EXECUTE);
//	if(result != ZWSCN_ERR_NONE) {
//		zwscn_message_log_warning("(%d) retuned by zwscn_status_update_request_message_send\n", result);
//		goto l_err_status;
//	}
	scene_id_ptr = malloc(sizeof(scene_id));
	if(scene_id_ptr == NULL) {
		goto l_err_memory;
	}
	(*scene_id_ptr) = scene_id;
	(*list_status_update_requests_ptr) = zwscn_list_add((*list_status_update_requests_ptr), scene_id_ptr);
//	zwscn_set_statelog_end_op_status_update(net, scene_id, exec_trigger_type); 
	zwscn_set_statelog_end_op_exec(net, scene_id, exec_trigger_type);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	zwscn_set_statelog_end_op_exec(net, scene_id, exec_trigger_type);
	return ZWSCN_ERR_GENERIC;
//l_err_status:
////	zwscn_set_statelog_end_op_status_update(net, scene_id, exec_trigger_type);
//	zwscn_set_statelog_end_op_exec(net, scene_id, exec_trigger_type);
//	return ZWSCN_ERR_GENERIC;
l_err_memory:
	zwscn_set_statelog_end_op_exec(net, scene_id, exec_trigger_type);
	return ZWSCN_ERR_MEMORY;
}

ZWSCN_STATUS_T zwscn_execute_scene_private(void *net, zwscn_scene_cont_p scene_ptr, uint8_t exec_trigger_type, zwscn_list_t *list_set_requests_ptr) {
	zwscn_list_t action_list;
	zwscn_action_p action_ptr;
	int count=0;
	int i=0;
	ZWSCN_STATUS_T result =  ZWSCN_ERR_NONE;
	zwscn_init_p init_data;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if((net == NULL) || (scene_ptr == NULL)) {
		goto l_err_args;
	} 
	
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene id=%d trigger=%d\n", net, scene_ptr->scene->id, exec_trigger_type);
	action_list = scene_ptr->list_actions;
	count = zwscn_list_size(action_list); //If list is NULL, count should be 0
	ZWSCN_MESSAGE_LOG_DEBUG("executing %d actions of scene=%d\n", count, scene_ptr->scene->id);
	for(i=0; i < count; ++i) {
		action_ptr = zwscn_list_get_by_index(action_list, i);
		result = zwscn_execute_action_private(net, scene_ptr, action_ptr, list_set_requests_ptr);
		if(result == ZWSCN_ERR_CALLBACK_NULL) { //Callback is null so no need to try other actions
			break;
		}
	}
	
	if(result != ZWSCN_ERR_CALLBACK_NULL) {
		zwscn_execute_scene_info_update(scene_ptr, exec_trigger_type);
		init_data = zwscn_get_init_data(net);
		if(init_data) {
			init_data->last_executed_scene_id = scene_ptr->scene->id;
		}
	}

	return result;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
}

ZWSCN_STATUS_T zwscn_execute_action_private(void *net, zwscn_scene_cont_p scene_ptr, zwscn_action_p action_ptr, zwscn_list_t *list_set_requests_ptr) {
	zwscn_init_p init_data;
//	uint8_t node_id, ep_id;
	uint8_t cmd;
//	uint16_t cls_id;
	void *args=NULL;
//	int result;
	zwscn_set_op_request_p op_set_request = NULL;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if((net == NULL) || (scene_ptr == NULL) || (action_ptr == NULL)) {
		goto l_err_args;
	}
	
//	node_id = zwscn_util_combo_id_to_node_id(action_ptr->id);
//	ep_id = zwscn_util_combo_id_to_ep_id(action_ptr->id);
//	cls_id = zwscn_util_combo_id_to_cls_id(action_ptr->id);
	cmd = action_ptr->cmd;
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->init_input && init_data->init_input->zwscn_function_set_op_handler) {
		args = zwscn_zwave_prepare_set_op_args(net, action_ptr);
//		result = init_data->init_input->zwscn_function_set_op_handler(net, node_id, ep_id, cls_id, cmd, args);
//		if(result < 0) {
//			zwscn_message_log_critical("Function set_op_handler return error (%d) for Scene_id=%d Action_id=%d\n", result, scene_ptr->scene->id, action_ptr->id);
//			goto l_err_callback;
//		}
		////////////////////////////
		op_set_request = calloc(1, sizeof(zwscn_set_op_request_t));
		if(op_set_request == NULL) {
			goto l_err_memory;
		}
		op_set_request->scene_id = scene_ptr->scene->id;
		op_set_request->action_id = action_ptr->id;
		op_set_request->cmd = cmd;
		op_set_request->args = args;
		op_set_request->func_set_op = init_data->init_input->zwscn_function_set_op_handler;
		
		(*list_set_requests_ptr) = zwscn_list_add((*list_set_requests_ptr), op_set_request);
		////////////////////////////
	} else {
		zwscn_message_log_warning("Function set_op_handler is NULL for Scene_id=%d Action_id=%d\n", scene_ptr->scene->id, action_ptr->id);
		goto l_err_callback_null;
	}
	
	return ZWSCN_ERR_NONE;
	
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
//l_err:
//	return ZWSCN_ERR_GENERIC;
//l_err_callback:
//	return ZWSCN_ERR_CALLBACK_RESULT;
l_err_callback_null:
	return ZWSCN_ERR_CALLBACK_NULL;
l_err_memory:
	return ZWSCN_ERR_MEMORY;
}

static void zwscn_execute_scene_info_update(zwscn_scene_cont_p scene_ptr, uint8_t last_execution_trigger) {
	ZWSCN_MESSAGE_LOG_DEBUG("Scene id=%d trigger=%d\n", scene_ptr->scene->id, last_execution_trigger);
	scene_ptr->scene->letrigger = last_execution_trigger;
	scene_ptr->scene->letime = ZWSCN_TIME_NOW();
}

static ZWSCN_STATUS_T zwscn_execute_send_set_op_requests(void *net, zwscn_list_t list_set_requests) {
	zwscn_set_op_request_p op_set_request = NULL;
	zwscn_fn_set_op_t func_set_op = NULL;
	zwscn_init_p init_data = NULL;
//	void *args = NULL;
	uint8_t node_id, ep_id, cmd;
	uint16_t cls_id;
	uint32_t action_id = 0;
	int count = 0;
	int i = 0;
	int result = 0;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if((net == NULL) || (list_set_requests == NULL)) {
		goto l_err_args;
	}
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		goto l_err;
	}
	
	count = zwscn_list_size(list_set_requests); //If list is NULL, count should be 0
	ZWSCN_MESSAGE_LOG_DEBUG("Calling SET op for %d actions\n", count);
	for(i=0; i < count; ++i) {
		op_set_request = (zwscn_set_op_request_p) zwscn_list_get_by_index(list_set_requests, i);
		if(op_set_request != NULL) {
			if(zwscn_does_net_exist(net)) {
				action_id = op_set_request->action_id;
				node_id = zwscn_util_combo_id_to_node_id(op_set_request->action_id);
				ep_id = zwscn_util_combo_id_to_ep_id(op_set_request->action_id);
				cls_id = zwscn_util_combo_id_to_cls_id(op_set_request->action_id);
				cmd = op_set_request->cmd;
				func_set_op = op_set_request->func_set_op;
				if(func_set_op != NULL) {
					ZWSCN_MESSAGE_LOG_DEBUG("Calling SET op on scene=%u action=0x%X\n", op_set_request->scene_id, op_set_request->action_id);
					if(zwscn_zwave_cc_has_postset_poll(cls_id, cmd)) {
						zwscn_message_log_debug("POSTSETPOLL adding action_id:0x%x\n", action_id);
						zwscn_mutex_lock(init_data->mutex_pending_report_status);
						zwscn_hash_add(init_data->hash_pending_postset_poll_cb, zwscn_uint_to_pointer(action_id));
						zwscn_mutex_unlock(init_data->mutex_pending_report_status);
						result = func_set_op(net, node_id, ep_id, cls_id, cmd, op_set_request->args, zwscn_postset_poll_notification_cb);
					} else {
						result = func_set_op(net, node_id, ep_id, cls_id, cmd, op_set_request->args, NULL);
					}
					op_set_request->result = result;
					if(result < 0) {
						zwscn_message_log_warning("Function set_op_handler return error (%d) for Scene_id=%d Action_id=0x%X\n", result, op_set_request->scene_id, op_set_request->action_id);
//						op_set_request->args = NULL; //freed by hcaal_set (MUX layer)
//						goto l_err_callback;
//						Print the warning but move on to the next action as the next action may be for a different device which may not return error
					}
					op_set_request->args = NULL; //freed by hcaal_set (MUX layer)
				} else {
					free(op_set_request->args);
				}
			} else {
				zwscn_message_log_debug("net does not exist\n");
				free(op_set_request->args);
			}
		} else {
			zwscn_message_log_debug("op_set_request is NULL\n");
		}
	}
	
	return ZWSCN_ERR_NONE;

l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_INVALID_NETWORK;
//l_err_callback:
//	return ZWSCN_ERR_CALLBACK_RESULT;
}

static void zwscn_execute_send_status_update_requests(void *net, zwscn_list_t list_status_update_requests) {
	uint32_t *scene_id_ptr = NULL;
	int count = 0;
	int i = 0;
	int result = 0;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if((net == NULL) || (list_status_update_requests == NULL)) {
		goto l_err_args;
	}
	
	count = zwscn_list_size(list_status_update_requests); //If list is NULL, count should be 0
	ZWSCN_MESSAGE_LOG_DEBUG("Requesting Status Update for %d scenes\n", count);
	for(i=0; i < count; ++i) {
		scene_id_ptr = (uint32_t *) zwscn_list_get_by_index(list_status_update_requests, i);
		if(scene_id_ptr != NULL) {
			if(zwscn_does_net_exist(net)) {
				ZWSCN_MESSAGE_LOG_DEBUG("Requesting Status Update for scene=%u\n", (*scene_id_ptr));
				result = zwscn_status_update_request_message_send(net, (*scene_id_ptr), 0, ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_SCENE_ON_EXECUTE);
				if(result != ZWSCN_ERR_NONE) {
					zwscn_message_log_warning("(%d) retuned by zwscn_status_update_request_message_send\n", result);
					//goto l_err_status; //ignore error and process next request
				}
			} else {
				zwscn_message_log_debug("net does not exist\n");
			}
			//free(scene_id_ptr); // done by list_free
		} else {
			zwscn_message_log_debug("scene_id_ptr is NULL\n");
		}
	}
	
	return;

l_err_args:
//l_err_callback:
	return;
}
