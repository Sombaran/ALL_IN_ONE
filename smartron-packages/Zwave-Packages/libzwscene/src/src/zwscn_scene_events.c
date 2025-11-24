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
@file		zwscn_scene_events.c  
  
                To provide support for handling device report for Scene Events trigger
  
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
#include "zwscn_util_mqueue.h"
#include "zwscn_util_mqueue_message.h"
#include "zwscn_util_thread.h"
#include "zwscn_util_mutex.h"
#include "zwscn_util_rwlock.h"
#include "zwscn_util_timer.h"
#include "zwscn_statelog.h"
#include "zwscn_scene.h"
#include "zwscn_scene_events.h"
#include "zwscn_util_zwave.h"
#include "zwscn_scene_execute.h"
#include "zwscn_util_mem.h"
#include "zwscn_util_message_log.h"

void zwscn_events_report_handler_thread(void *data) {
	zwscn_mqueue_t mqueue_events;
	zwscn_mqueue_message_p message;
	void *net;
	ZWSCN_STATUS_T result;
	
	if(data == NULL) {
		zwscn_message_log_error("message queue is NULL. Could not start EVENTS thread\n");
		return;
	}
	zwscn_message_log_info("THREAD_EVENTS started\n");
	mqueue_events = (zwscn_mqueue_t) data;
	
	while(1) {
		message = zwscn_mqueue_message_pop(mqueue_events); //Blocking call
		if(message != NULL) {
			zwscn_message_log_debug("EVENTS Thread received message with refcount=%d type=%d priority=%d\n", message->refcount, message->type, message->priority);
			if(message->type == ZWSCN_MESSAGE_TYPE_EXIT) {
				if(zwscn_mqueue_message_refcount_dec_and_test(message)) {
					zwscn_free_mqueue_message(message);
				}
				zwscn_message_log_info("EVENTS Thread exiting...\n");
				zwscn_thread_exit(NULL);
			} else {
				net = message->net;
				zwscn_network_lock_reader(net); //TODO - Check if scene execution or status update require writers lock
				if (zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should not process the message
					result = zwscn_events_process_report(net, message->type, message->sub_type, message->data);
					if(result != ZWSCN_ERR_NONE) {
						zwscn_message_log_critical("[Warning] (%d) returned by zwscn_events_process_report\n", result);
					}
				}
				zwscn_network_unlock_reader(net);
				if(zwscn_mqueue_message_refcount_dec_and_test(message)) {
					zwscn_free_mqueue_message(message);
				}
			}
		}
	}
}

ZWSCN_STATUS_T zwscn_events_process_report(void *net, uint32_t id, uint32_t cmd, void *report) {
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
		//TODO ABORT_OP function
		scene_ptr = zwscn_list_get_by_index((*scene_list), i);
		result = zwscn_events_trigger_scene_on_report(net, scene_ptr, id, cmd, report);
		if(result != ZWSCN_ERR_NONE) {
			zwscn_message_log_critical("[Warning] (%d) returned by zwscn_events_trigger_scene_on_report for scene id=%d\n", result, scene_ptr->scene->id);
		}
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;	
}

ZWSCN_STATUS_T zwscn_events_trigger_scene_on_report(void *net, zwscn_scene_cont_p scene_ptr, uint32_t id, uint32_t cmd, void *report) {
	ZWSCN_STATUS_T result;
	zwscn_list_t event_list;
	zwscn_event_p event_ptr;
	int count=0;
	int i=0;
	uint32_t scene_id;
	
	if(scene_ptr == NULL) {
		goto l_err_args;
	} 
	
	event_list = scene_ptr->list_events;
	count = zwscn_list_size(event_list); //If list is NULL, count should be 0
	zwscn_message_log_debug("EVENTS checking scene id=%d with %d events\n", scene_ptr->scene->id, count);
	for(i=0; i < count; ++i) {
		//TODO ABORT_OP function
		
		event_ptr = zwscn_list_get_by_index(event_list, i);
		if(event_ptr && (event_ptr->id == id)) { 
			zwscn_message_log_debug("EVENTS processing Event id=%d cmd=%d\n", event_ptr->id, event_ptr->cmd);
			if(zwscn_is_event_valid(net, event_ptr, id, cmd, report) == ZWSCN_TRUE) {
				scene_id = scene_ptr->scene->id;
//				zwscn_set_statelog_start_op_exec(net, scene_id, ZWSCN_STATELOG_TRIGGER_EVENT);
//				if(zwscn_execute_scene_private(net, scene_ptr, ZWSCN_STATELOG_TRIGGER_EVENT) != ZWSCN_ERR_NONE) {
//					goto l_err;
//				}
////				zwscn_set_statelog_start_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_EVENT); 
//				if(zwscn_status_update_scene_req_start(net, scene_ptr, ZWSCN_TRUE, ZWSCN_TRUE) != ZWSCN_ERR_NONE) {
//					goto l_err_status;
//				}
////				zwscn_set_statelog_end_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_EVENT); 
//				zwscn_set_statelog_end_op_exec(net, scene_id, ZWSCN_STATELOG_TRIGGER_EVENT);

//				if(zwscn_execute_and_update_scene(net, scene_ptr, ZWSCN_STATELOG_TRIGGER_EVENT) != ZWSCN_ERR_NONE) {
//					zwscn_message_log_critical("Scene execution failed on EVENT trigger.\n");
//					//Even though there was an error, we ignore it and continue
//				}
				result = zwscn_execute_request_message_send(net, scene_id, ZWSCN_MESSAGE_TYPE_EXECUTE_SCENE_ON_EVENT);
				if(result != ZWSCN_ERR_NONE) {
					zwscn_message_log_warning("(%d) retuned by zwscn_execute_request_message_send\n", result);
					zwscn_message_log_critical("Scene execution failed on EVENT trigger.\n");
					//Even though there was an error, we ignore it and continue
				}
			}
			
		}
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
//l_err:
//	zwscn_set_statelog_end_op_exec(net, scene_id, ZWSCN_STATELOG_TRIGGER_EVENT);
//	return ZWSCN_ERR_GENERIC;
//l_err_status:
////	zwscn_set_statelog_end_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_EVENT);
//	zwscn_set_statelog_end_op_exec(net, scene_id, ZWSCN_STATELOG_TRIGGER_EVENT);
//	return ZWSCN_ERR_GENERIC;
}

ZWSCN_BOOL_T zwscn_is_event_valid(void *net, zwscn_event_p event_ptr, uint32_t id, uint32_t cmd, void *report) {
	ZWSCN_BOOL_T new_status;
	zwscn_time_t sutime = 0;
	ZWSCN_STATUS_T result;
		
	if((event_ptr == NULL) || (event_ptr->device_args == NULL) || (report == NULL)) {
		goto l_err_args;
	} 
	
	if(event_ptr->type == ZWSCN_EVENT_DISABLED) {
		zwscn_message_log_info("SCENE: Event is disabled. Skipping...\n");
		goto l_err_event_disabled;
	}
	
	result = zwscn_zwave_compare_cc(net, event_ptr->device_args, id, cmd, report, &new_status, &sutime);
	if(result != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	event_ptr->status = new_status;
	event_ptr->lrtime = sutime;
	
	return new_status;
l_err_args:
l_err_event_disabled:
l_err:
	return ZWSCN_FALSE;
}
