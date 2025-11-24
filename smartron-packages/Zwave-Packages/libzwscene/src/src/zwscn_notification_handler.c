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
@file		zwscn_notification_handler.c  
  
                To provide support for handling notification and device reports
  
@author		Arif Mohammand
 
@version	1.0 2013-12-05  Initial release
*/

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
#include "zwscn_util_zwave.h"
#include "zwscn_util.h"
#include "zwscn_util_mem.h"
#include "zwscn_notification_handler.h"
#include "zwscn_notification_processor.h"
#include "zwscn_device_report_cache.h"

void zwscn_network_notification_handler(void *net, uint8_t op, uint16_t status, void *buf, uint32_t buf_size) {
	zwscn_mqueue_t mqueue_notification_processor_thread = NULL;
	zwscn_mqueue_message_p message;
	void *buf_copy = NULL;
	uint32_t notification_type;
	
	if (!net) {
		goto l_err_args;
	}
	if((buf == NULL) && (buf_size > 0)) {
		zwscn_message_log_warning("zwscn_network_notification_handler: buf_size(%u) is not zero while buf is NULL\n", buf_size);
		goto l_err_args;
	}
	if(buf_size > ZWSCN_NOTIFICATION_MAX_SIZE) {
		zwscn_message_log_warning("zwscn_network_notification_handler: buf_size(%u) is more than ZWSCN_NOTIFICATION_MAX_SIZE(%u)\n", buf_size, ZWSCN_NOTIFICATION_MAX_SIZE);
		goto l_err_args;
	}
	
	if (!zwscn_does_net_exist(net)) {
		zwscn_message_log_warning("zwscn_network_notification_handler: net=%p does not exist\n", net);
		goto l_err_exit;
	}
	
	zwscn_message_log_debug("NOTIFICATION NETWORK: net=%p op=%u status=%u buf_size=%u\n", net, op, status, buf_size);
	
	if((op == 8) && (status == 0)) { //#define ZWNET_OP_RESET          8   /**< Restore to factory default setting*/
		notification_type = ZWSCN_NOTIFICATION_NETWORK_RESET;
	} else if((op == 7) && (status == 0)) { //#define ZWNET_OP_UPDATE         7   /**< Update network topology from the SUC/SIS*/
		notification_type = ZWSCN_NOTIFICATION_NETWORK_UPDATE;
	} else if((op == 4) && (status == 0)) { //#define ZWNET_OP_RP_NODE        4   /**< Replace failed node operation*/
		notification_type = ZWSCN_NOTIFICATION_NETWORK_REPLACE_FAILED_NODE;
	} else {
		zwscn_message_log_message("Unknown NOTIFICATION NETWORK op=%u status=%u\n", op, status);
		goto l_err_exit;
	}
	
	if(buf_size > 0) {
		buf_copy = malloc(buf_size);
		if(buf_copy == NULL) {
			goto l_err_memory;
		}
		memcpy(buf_copy, buf, buf_size);
	}
	
	message = zwscn_mqueue_message_new(buf_copy);
	if(message ==  NULL) {
		goto l_err_message;
	}
	message->net = net;
	message->type = ZWSCN_MESSAGE_TYPE_NOTIFICATION_NETWORK;
	message->sub_type = notification_type;
	message->data_size = buf_size;
	zwscn_mqueue_message_refcount_inc(message);
	
	zwscn_mqueue_lock(net);
	if(zwscn_find_mqueue_thread_notification_processor(net, &mqueue_notification_processor_thread) == ZWSCN_ERR_NONE) {
		if(mqueue_notification_processor_thread && (zwscn_mqueue_length(mqueue_notification_processor_thread) < ZWSCN_MQUEUE_LENGTH_MAX_NOTIFICATIONS)) {
			zwscn_mqueue_message_refcount_inc(message);
			zwscn_mqueue_message_push(mqueue_notification_processor_thread, message); //Send it to notification_processor thread
		} else {
			zwscn_message_log_message("MQUEUE for Notifications is full. dropping new message...\n");
		}
	}
	zwscn_mqueue_unlock(net);
	
	if(zwscn_mqueue_message_refcount_dec_and_test(message)) { //There was an error in sending message to any of the threads
		zwscn_free_mqueue_message(message); // Free the message as no other thread is going to 
	}
	
	return;
l_err_args:
l_err_exit:
//l_err:
l_err_memory:
	return;
l_err_message:
	free(buf_copy);
	return;
}

void zwscn_node_notification_handler(void *net, uint8_t node_id, ZWSCN_NODE_NOTIFY_MODE_T mode) {
	zwscn_mqueue_t mqueue_notification_processor_thread;
	zwscn_mqueue_message_p message;
	void *node_id_copy = NULL;
	uint32_t notification_type;
//	ZWSCN_STATUS_T result;
	
	if ((net == NULL) || (node_id == 0)) {
		zwscn_message_log_warning("zwscn_node_notification_handler argument error\n");
		goto l_err_args;
	} 

	if (!zwscn_does_net_exist(net)) {
		zwscn_message_log_warning("zwscn_node_notification_handler: net=%p does not exist\n", net);
		goto l_err_exit;
	}
	
	zwscn_message_log_debug("NOTIFICATION NODE: net=%p id=%u mode=%d\n", net, node_id, mode);
	if(mode == ZWSCN_NODE_REMOVED) {
		notification_type = ZWSCN_NOTIFICATION_NODE_REMOVED;
	} else if(mode == ZWSCN_NODE_UPDATED) {
		notification_type = ZWSCN_NOTIFICATION_NODE_UPDATE;
	} else {
		zwscn_message_log_message("Unknown NOTIFICATION NODE id=%u mode=%d\n",node_id, mode);
		goto l_err_exit;
	}
	
	node_id_copy = malloc(sizeof(node_id));
	if(node_id_copy == NULL) {
		goto l_err_memory;
	}
	memcpy(node_id_copy, &node_id, sizeof(node_id));
	
	message = zwscn_mqueue_message_new(node_id_copy);
	if(message ==  NULL) {
		goto l_err_message;
	}
	message->net = net;
	message->type = ZWSCN_MESSAGE_TYPE_NOTIFICATION_NODE;
	message->sub_type = notification_type;
	message->data_size = sizeof(node_id);
	zwscn_mqueue_message_refcount_inc(message);
	
	zwscn_mqueue_lock(net);
	if(zwscn_find_mqueue_thread_notification_processor(net, &mqueue_notification_processor_thread) == ZWSCN_ERR_NONE) {
		if(mqueue_notification_processor_thread && (zwscn_mqueue_length(mqueue_notification_processor_thread) < ZWSCN_MQUEUE_LENGTH_MAX_NOTIFICATIONS)) {
			zwscn_mqueue_message_refcount_inc(message);
			zwscn_mqueue_message_push(mqueue_notification_processor_thread, message); //Send it to notification_processor thread
		} else {
			zwscn_message_log_message("MQUEUE for Notifications is full. dropping new message...\n");
		}
	}
	zwscn_mqueue_unlock(net);
	
	if(zwscn_mqueue_message_refcount_dec_and_test(message)) { //There was an error in sending message to any of the threads
		zwscn_free_mqueue_message(message); // Free the message as no other thread is going to 
	}
	
	return;
l_err_args:
l_err_exit:
//l_err:
l_err_memory:
	return;
l_err_message:
	free(node_id_copy);
	return;
}

int zwscn_device_notification_handler(void *net, uint8_t node_id, uint8_t ep_id, uint16_t cls_id, uint8_t cmd, void *report, uint32_t report_size) {
	zwscn_mqueue_t mqueue_status_update_report_handler;
	zwscn_mqueue_t mqueue_events;
	zwscn_mqueue_message_p message;
	void *report_copy = NULL;
	
	if ((net == NULL) || (report == NULL)) {
		zwscn_message_log_warning("zwscn_device_notification_handler: network or report is NULL\n");
		goto l_err_args;
	}
	if((report == NULL) && (report_size > 0)) {
		zwscn_message_log_warning("zwscn_device_notification_handler: report_size(%u) is not zero while report is NULL\n", report_size);
		goto l_err_args;
	}
	if(report_size > ZWSCN_DEVICE_REPORT_MAX_SIZE) {
		zwscn_message_log_warning("zwscn_device_notification_handler: report_size(%u) is more than ZWSCN_DEVICE_REPORT_MAX_SIZE(%u)\n", report_size, ZWSCN_DEVICE_REPORT_MAX_SIZE);
		goto l_err_args;
	}

	if (!zwscn_does_net_exist(net)) {
		zwscn_message_log_warning("zwscn_device_notification_handler: net=%p does not exist\n", net);
		goto l_err_exit;
	}
	
	zwscn_message_log_debug("NOTIFICATION REPORT: Received a report from net=%p node=%u ep=%u cls=%u cmd=%u report_size=%u\n", net, node_id, ep_id, cls_id, cmd, report_size);
	if(report_size > 0) {
		report_copy = malloc(report_size);
		if(report_copy == NULL) {
			goto l_err_memory;
		}
		memcpy(report_copy, report, report_size);
	}
	
	message = zwscn_mqueue_message_new(report_copy);
	if(message ==  NULL) {
		goto l_err_message;
	}
	message->net = net;
	message->type = zwscn_util_make_combo_id(node_id, ep_id, cls_id);
	message->sub_type = cmd;
	message->data_size = report_size;
	zwscn_mqueue_message_refcount_inc(message);
	
	if(zwscn_zwave_is_cc_for_action(cls_id, cmd) == ZWSCN_TRUE) {
		zwscn_mqueue_lock(net);
		if(zwscn_find_mqueue_thread_status_update_report_handler(net, &mqueue_status_update_report_handler) == ZWSCN_ERR_NONE) {
			if(mqueue_status_update_report_handler && (zwscn_mqueue_length(mqueue_status_update_report_handler) < ZWSCN_MQUEUE_LENGTH_MAX_STATUS_UPDATE_REPORT_HANDLER)) {
				zwscn_mqueue_message_refcount_inc(message);
				zwscn_mqueue_message_push(mqueue_status_update_report_handler, message); //Send it to status update thread
				
				//Add cls_id (key) and report_time (value) into the hash
				zwscn_device_report_cache_add(net, node_id, ep_id, cls_id, cmd, report, report_size);
			} else {
				zwscn_message_log_message("MQUEUE for Status Update is full. dropping new message...\n");
			}
		}
		zwscn_mqueue_unlock(net);
	}
	
	if(zwscn_zwave_is_cc_for_event(cls_id, cmd) == ZWSCN_TRUE) {
		zwscn_mqueue_lock(net);
		if(zwscn_find_mqueue_thread_events(net, &mqueue_events) == ZWSCN_ERR_NONE) {
			if(mqueue_events && (zwscn_mqueue_length(mqueue_events) < ZWSCN_MQUEUE_LENGTH_MAX_EVENTS)) {
				zwscn_mqueue_message_refcount_inc(message);
				zwscn_mqueue_message_push(mqueue_events, message); //Send it to events thread
			} else {
				zwscn_message_log_message("MQUEUE for Events is full. dropping new message...\n");
			}
		}
		zwscn_mqueue_unlock(net);
	}
	
	if((zwscn_zwave_is_cc_for_arm_security_event(cls_id, cmd) == ZWSCN_TRUE) || 
		(zwscn_zwave_is_cc_for_alarm_security_event(cls_id, cmd) == ZWSCN_TRUE)) {
		zwscn_mqueue_lock(net);
		if(zwscn_find_mqueue_thread_security_events(net, &mqueue_events) == ZWSCN_ERR_NONE) {
			if(mqueue_events && (zwscn_mqueue_length(mqueue_events) < ZWSCN_MQUEUE_LENGTH_MAX_SECURITY_SCENE_EVENTS)) {
				zwscn_mqueue_message_refcount_inc(message);
				zwscn_mqueue_message_push(mqueue_events, message); //Send it to events thread
			} else {
				zwscn_message_log_message("MQUEUE for Security Events is full. dropping new message...\n");
			}
		}
		zwscn_mqueue_unlock(net);
	}
	
	if(zwscn_mqueue_message_refcount_dec_and_test(message)) { //if the message was not sent to any of the threads
		zwscn_free_mqueue_message(message); // Free the message as no other thread is going to 
	}
	
	return (int)ZWSCN_ERR_NONE;
l_err_args:
	return (int)ZWSCN_ERR_INVALID_ARGS;
l_err_exit:
	return (int)ZWSCN_ERR_GENERIC;
l_err_memory:
	return (int)ZWSCN_ERR_MEMORY;
l_err_message:
	free(report_copy);
	return (int)ZWSCN_ERR_MEMORY;
}

//ZWPoll callback
void zwscn_zwpoll_notification_cb(void *net, uint16_t handle, uint32_t token)
{
	zwscn_mqueue_t mqueue_notification_processor_thread;
	zwscn_mqueue_message_p message;
	uint32_t *token_copy = NULL;
	
	if ((net == NULL) || (token == 0)) {
		zwscn_message_log_warning("zwscn_zwpoll_notification_cb argument error\n");
		goto l_err_args;
	} 

	if (!zwscn_does_net_exist(net)) {
		zwscn_message_log_warning("zwscn_zwpoll_notification_cb: net=%p does not exist\n", net);
		goto l_err_exit;
	}
	zwscn_message_log_debug("NOTIFICATION ZWPoll: requests completed for net=%p handle=%u token=%u\n", net, handle, token);
	
	token_copy = malloc(sizeof(token));
	if(token_copy == NULL) {
		goto l_err_memory;
	}
	memcpy(token_copy, &token, sizeof(token));
	message = zwscn_mqueue_message_new(token_copy);
	if(message ==  NULL) {
		goto l_err_message;
	}
	message->net = net;
	message->type = ZWSCN_MESSAGE_TYPE_NOTIFICATION_ZWPOLL;
	message->sub_type = handle; //using subtype to store handle value
	message->data_size = sizeof(token);
	zwscn_mqueue_message_refcount_inc(message);
	
	zwscn_mqueue_lock(net);
	if(zwscn_find_mqueue_thread_notification_processor(net, &mqueue_notification_processor_thread) == ZWSCN_ERR_NONE) {
		if(mqueue_notification_processor_thread && (zwscn_mqueue_length(mqueue_notification_processor_thread) < ZWSCN_MQUEUE_LENGTH_MAX_NOTIFICATIONS)) {
			zwscn_mqueue_message_refcount_inc(message);
			zwscn_mqueue_message_push(mqueue_notification_processor_thread, message); //Send it to notification_processor thread
		} else {
			zwscn_message_log_message("MQUEUE for Notifications is full. dropping new message...\n");
		}
	}
	zwscn_mqueue_unlock(net);
	
	if(zwscn_mqueue_message_refcount_dec_and_test(message)) { //There was an error in sending message to any of the threads
		zwscn_free_mqueue_message(message); // Free the message as no other thread is going to 
	}
	
	return;
l_err_args:
l_err_exit:
//l_err:
l_err_memory:
	return;
l_err_message:
	free(token_copy);
	return;
}

//Post-Set poll callback
void zwscn_postset_poll_notification_cb(void *net, uint8_t node_id, uint8_t ep_id, uint16_t cls_id, int reason)
{
	zwscn_mqueue_t mqueue_notification_processor_thread;
	zwscn_mqueue_message_p message;
	uint32_t action_id = 0;
	uint32_t *action_id_ptr = NULL;
	
	zwscn_message_log_debug("NOTIFICATION POSTSETPoll: net:%p node:%u, ep:%u cls:%u reason:%u\n", net, node_id, ep_id, cls_id, reason);
	if ((net == NULL) || (node_id == 0)) {
		zwscn_message_log_warning("zwscn_postset_poll_notification_cb argument error\n");
		goto l_err_args;
	} 

	if (!zwscn_does_net_exist(net)) {
		zwscn_message_log_warning("zwscn_postset_poll_notification_cb: net=%p does not exist\n", net);
		goto l_err_exit;
	}
	
	action_id = zwscn_util_make_combo_id(node_id, ep_id, cls_id);
	action_id_ptr = malloc(sizeof(action_id));
	if(action_id_ptr == NULL) {
		goto l_err_memory;
	}
	memcpy(action_id_ptr, &action_id, sizeof(action_id));
	message = zwscn_mqueue_message_new(action_id_ptr);
	if(message ==  NULL) {
		goto l_err_message;
	}
	message->net = net;
	message->type = ZWSCN_MESSAGE_TYPE_NOTIFICATION_POSTSETPOLL;
	message->sub_type = reason; //using subtype to store 'reason' value
	message->data_size = sizeof(action_id);
	zwscn_mqueue_message_refcount_inc(message);
	
	zwscn_mqueue_lock(net);
	if(zwscn_find_mqueue_thread_notification_processor(net, &mqueue_notification_processor_thread) == ZWSCN_ERR_NONE) {
		if(mqueue_notification_processor_thread && (zwscn_mqueue_length(mqueue_notification_processor_thread) < ZWSCN_MQUEUE_LENGTH_MAX_NOTIFICATIONS)) {
			zwscn_mqueue_message_refcount_inc(message);
			zwscn_mqueue_message_push(mqueue_notification_processor_thread, message); //Send it to notification_processor thread
		} else {
			zwscn_message_log_message("MQUEUE for Notifications is full. dropping new message...\n");
		}
	}
	zwscn_mqueue_unlock(net);
	
	if(zwscn_mqueue_message_refcount_dec_and_test(message)) { //There was an error in sending message to any of the threads
		zwscn_free_mqueue_message(message); // Free the message as no other thread is going to 
	}
	
	return;
l_err_args:
l_err_exit:
//l_err:
l_err_memory:
	return;
l_err_message:
	free(action_id_ptr);
	return;
}

