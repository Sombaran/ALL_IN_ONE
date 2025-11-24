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
@file		zwscn_security_scene_notifications.c  
  
                To provide support for sending Security Scene notifications
  
@author		Arif Mohammand
 
@version	1.0 2015-04-20  Initial release
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "zwscn_defines.h"
#include "zwscn_scene_api.h"
#include "zwscn_util.h"
#include "zwscn_util_config.h"
#include "zwscn_util_email.h"
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
#include "zwscn_util_zwave.h"
#include "zwscn_statelog.h"
#include "zwscn_scene.h"
#include "zwscn_security_scene_api.h"
#include "zwscn_security_scene.h"
#include "zwscn_security_scene_notifications.h"
#include "zwscn_util_mem.h"

static ZWSCN_STATUS_T zwscn_security_scene_notification_send(void *net, zwscn_security_scene_p scene_ptr, uint32_t message_type);
static ZWSCN_STATUS_T zwscn_security_scene_notification_send_by_sms(void *net, zwscn_security_scene_p scene_ptr, uint32_t message_type, const char *email_from, const char *sms_gateway);
static ZWSCN_STATUS_T zwscn_security_scene_notification_send_by_email(void *net, zwscn_security_scene_p scene_ptr, uint32_t message_type, const char *email_from);
static ZWSCN_STATUS_T zwscn_security_scene_send_email(void *net, zwscn_security_scene_p scene_ptr, uint32_t message_type, const char *email_to, const char *email_from);
static void zwscn_security_scene_notification_get_message_strings(void *net, zwscn_security_scene_p scene_ptr, uint32_t message_type, char *email_subject, uint32_t email_subject_len, char *email_body_text, uint32_t email_body_text_len);

ZWSCN_STATUS_T zwscn_security_scene_notification_request_message_send(void *net, zwscn_security_scene_p scene_ptr, uint8_t message_type) {
	zwscn_mqueue_t mqueue_security_scene_notification_request_handler;
	zwscn_mqueue_message_p message;
	uint32_t scene_id = 0;
	
	if((net == NULL) || (scene_ptr == NULL)) {
		goto l_err_args;
	} 
	
	scene_id = scene_ptr->id;
	message = zwscn_mqueue_message_new(scene_ptr);
	if(message ==  NULL) {
		goto l_err_message;
	}
	message->net = net;
	message->type = message_type;
	zwscn_mqueue_lock(net);
	if(zwscn_find_mqueue_thread_security_scene_notification_request_handler(net, &mqueue_security_scene_notification_request_handler) == ZWSCN_ERR_NONE) {
		if(mqueue_security_scene_notification_request_handler && (zwscn_mqueue_length(mqueue_security_scene_notification_request_handler) < ZWSCN_MQUEUE_LENGTH_MAX_SECURITY_SCENE_NOTIFICATIONS)) {
			ZWSCN_MESSAGE_LOG_DEBUG("sending message to security scene notification request handler thread for net=%p scene id=%d\n", net, scene_id);
			zwscn_mqueue_message_push(mqueue_security_scene_notification_request_handler, message); //Send it to security scene notification request handler thread
		} else {
			zwscn_message_log_message("MQUEUE for Security Scene Notification Request Handler is full. dropping new message...\n");
			zwscn_free_security_scene_struct(message->data);
			message->data = NULL;
			zwscn_free_mqueue_message(message);
		}
	} else {
		zwscn_message_log_message("MQUEUE for Security Scene Notification Request Handler is not found. dropping new message...\n");
		zwscn_free_security_scene_struct(message->data);
		message->data = NULL;
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

void zwscn_security_scene_notification_request_handler_thread(void *data) {
	zwscn_mqueue_t mqueue_security_scene_notification_request_handler = NULL;
	zwscn_mqueue_message_p message = NULL;
	uint32_t message_type = 0;
	void *net = NULL;
	uint32_t scene_id = 0;
	zwscn_security_scene_p scene_ptr = NULL;
//	ZWSCN_STATUS_T result = ZWSCN_ERR_NONE;
	
	if(data == NULL) {
		zwscn_message_log_error("message queue is NULL. Could not start Security Scene Notification thread\n");
		return;
	}
	zwscn_message_log_info("THREAD_SECURITY_SCENE_NOTIFICATIONS started\n");
	mqueue_security_scene_notification_request_handler = (zwscn_mqueue_t) data;
	
	while(1) {
		message = zwscn_mqueue_message_pop(mqueue_security_scene_notification_request_handler); //Blocking call
		if(message != NULL) {
			zwscn_message_log_debug("SECURITY_SCENE_NOTIFICATIONS Thread received message with refcount=%d type=%d priority=%d\n", message->refcount, message->type, message->priority);
			if(message->type == ZWSCN_MESSAGE_TYPE_EXIT) {
				if(zwscn_mqueue_message_refcount_dec_and_test(message)) {
					zwscn_free_mqueue_message(message);
				}
				zwscn_message_log_info("SECURITY_SCENE_NOTIFICATIONS Thread exiting...\n");
				zwscn_thread_exit(NULL);
			} else {
				net = message->net;
				message_type = message->type;
				scene_ptr = message->data;
				scene_id = scene_ptr->id;
				zwscn_message_log_debug("SECURITY_SCENE_NOTIFICATIONS Thread checking message type %d for net=%p scene=%d\n", message_type, net, scene_id);
//				zwscn_network_lock_reader(net); //TODO - Check if status update require writers lock
				if (zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should not process the message
					if((message_type == ZWSCN_MESSAGE_TYPE_SECURITY_SCENE_NOTIFICATION_ALARM)	||
						(message_type == ZWSCN_MESSAGE_TYPE_SECURITY_SCENE_NOTIFICATION_ADD)	||
						(message_type == ZWSCN_MESSAGE_TYPE_SECURITY_SCENE_NOTIFICATION_EDIT)) {
						if(scene_id != 0) {
							zwscn_security_scene_notification_send(net, scene_ptr, message_type);
						} else {
							zwscn_message_log_warning("SECURITY_SCENE_NOTIFICATIONS thread could not find scene for scene id=%d\n", scene_id);
						}
					}
				} else {
					zwscn_message_log_warning("net=%p does not exist\n", net);
				}
//				zwscn_network_unlock_reader(net);
				zwscn_free_security_scene_struct(message->data);
				message->data = NULL;
				zwscn_free_mqueue_message(message);
			}
		}
	}
}

static ZWSCN_STATUS_T zwscn_security_scene_notification_send(void *net, zwscn_security_scene_p scene_ptr, uint32_t message_type) {
	uint32_t scene_id = 0;
	ZWSCN_STATUS_T result = ZWSCN_ERR_NONE;
	ZWSCN_BOOL_T security_scene_notification_sms_enable = ZWSCN_FALSE;
	ZWSCN_BOOL_T security_scene_notification_email_enable = ZWSCN_FALSE;
	char *security_scene_notification_sms_sender_addr = NULL;
	char *security_scene_notification_sms_gateway = NULL;
	char *security_scene_notification_email_sender_addr = NULL;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if((net == NULL) || (scene_ptr == NULL)) {
		goto l_err_args;
	}
	
	security_scene_notification_sms_enable = zwscn_config_sms_is_enabled(net);
	security_scene_notification_email_enable = zwscn_config_email_is_enabled(net);
	security_scene_notification_sms_sender_addr = zwscn_config_sms_get_sender_address(net);
	security_scene_notification_sms_gateway = zwscn_config_sms_get_gateway(net);
	security_scene_notification_email_sender_addr = zwscn_config_email_get_sender_address(net);
	
	scene_id = scene_ptr->id;
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene id=%d\n", net, scene_id);
	ZWSCN_MESSAGE_LOG_DEBUG("Event type=%d time=%llu\n", scene_ptr->lt_alarm_on.type, scene_ptr->lt_alarm_on.time);
	
	if(scene_ptr->is_notification_on) {
		if((security_scene_notification_sms_enable) && (scene_ptr->is_notification_by_sms_on)) {
			if(message_type == ZWSCN_MESSAGE_TYPE_SECURITY_SCENE_NOTIFICATION_ALARM) {
				result = zwscn_security_scene_notification_send_by_sms(net, scene_ptr, message_type, security_scene_notification_sms_sender_addr, security_scene_notification_sms_gateway);
				if(result != ZWSCN_ERR_NONE) {
					zwscn_message_log_warning("Error (%d) returned by zwscn_security_scene_notification_send_by_sms\n", result);
				}
			} else {
				ZWSCN_MESSAGE_LOG_DEBUG("Only allowed to send SMS for Alarm events. ignoring %u message type\n", message_type);
			}
		}
		if((security_scene_notification_email_enable) && (scene_ptr->is_notification_by_email_on)) {
			result = zwscn_security_scene_notification_send_by_email(net, scene_ptr, message_type, security_scene_notification_email_sender_addr);
			if(result != ZWSCN_ERR_NONE) {
				zwscn_message_log_warning("Error (%d) returned by zwscn_security_scene_notification_send_by_email\n", result);
			}
		}
	}
	free(security_scene_notification_sms_sender_addr);
	free(security_scene_notification_sms_gateway);
	free(security_scene_notification_email_sender_addr);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
//l_err:
//	return ZWSCN_ERR_GENERIC;	
}

static ZWSCN_STATUS_T zwscn_security_scene_notification_send_by_sms(void *net, zwscn_security_scene_p scene_ptr, uint32_t message_type, const char *email_from, const char *sms_gateway) {
	uint32_t send_target_size = 0;
//	const char email_from[] = ZWSCN_SENDER_EMAIL_ADDR_FOR_SMS;
//	const char sms_gateway[] = ZWSCN_SMS_GATEWAY;
	char email_to[ZWSCN_TEMP_STR_MAX] = {0};
	char *notification_sms_number_decoded = NULL;
	
	if((net == NULL) || (scene_ptr == NULL)) {
		goto l_err_args;
	}
	
	send_target_size = strlen(scene_ptr->notification_sms_number);
	if(send_target_size > 0) {
		ZWSCN_MESSAGE_LOG_DEBUG("SECURITY SCENE NOTIFICATIONS sending SMS to %s (URL encoded)\n", scene_ptr->notification_sms_number);
		notification_sms_number_decoded = zwscn_util_unescape_uri(scene_ptr->notification_sms_number);
		if(notification_sms_number_decoded == NULL) {
			goto l_err_memory;
		}
		ZWSCN_MESSAGE_LOG_DEBUG("SECURITY SCENE NOTIFICATIONS sending SMS to %s (URL decoded)\n", notification_sms_number_decoded);
		zwscn_util_strcat(email_to, notification_sms_number_decoded, sizeof(email_to));
		zwscn_util_strcat(email_to, "@", sizeof(email_to));
		zwscn_util_strcat(email_to, sms_gateway, sizeof(email_to));
		if(zwscn_security_scene_send_email(net, scene_ptr, message_type, email_to, email_from) != ZWSCN_ERR_NONE) {
			zwscn_message_log_warning("Could not send Security Scene notification SMS (by email)");
			goto l_err;
		}
		free(notification_sms_number_decoded);
	} else {
		zwscn_message_log_warning("SECURITY SCENE NOTIFICATIONS by SMS is enabled but the target number is blank\n");
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err_memory:
	return ZWSCN_ERR_MEMORY;
l_err:
	free(notification_sms_number_decoded);
	return ZWSCN_ERR_GENERIC;
}

static ZWSCN_STATUS_T zwscn_security_scene_notification_send_by_email(void *net, zwscn_security_scene_p scene_ptr, uint32_t message_type, const char *email_from) {
	uint32_t send_target_size = 0;
//	const char email_from[] = ZWSCN_SENDER_EMAIL_ADDR;
	char *email_to = NULL;
	
	if((net == NULL) || (scene_ptr == NULL)) {
		goto l_err_args;
	}
	
	send_target_size = strlen(scene_ptr->notification_email);
	if(send_target_size > 0) {
		ZWSCN_MESSAGE_LOG_DEBUG("SECURITY SCENE NOTIFICATIONS sending email to %s (URL encoded)\n", scene_ptr->notification_email);
		email_to = zwscn_util_unescape_uri(scene_ptr->notification_email);
		if(email_to == NULL) {
			goto l_err_memory;
		}
		ZWSCN_MESSAGE_LOG_DEBUG("SECURITY SCENE NOTIFICATIONS sending email to %s (URL decoded)\n", email_to);
		if(zwscn_security_scene_send_email(net, scene_ptr, message_type, email_to, email_from) != ZWSCN_ERR_NONE) {
			zwscn_message_log_warning("Could not send Security Scene notification email");
			goto l_err;
		}
		free(email_to);
	} else {
		zwscn_message_log_warning("SECURITY SCENE NOTIFICATIONS by email is enabled but the target address is blank\n");
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err_memory:
	return ZWSCN_ERR_MEMORY;
l_err:
	free(email_to);
	return ZWSCN_ERR_GENERIC;
}

static ZWSCN_STATUS_T zwscn_security_scene_send_email(void *net, zwscn_security_scene_p scene_ptr, uint32_t message_type, const char *email_to, const char *email_from) {
	char email_subject[ZWSCN_TEMP_STR_MAX] = {0};
	char email_body_text[ZWSCN_TEMP_STR_MAX] = {0};
	
	ZWSCN_MESSAGE_LOG_DEBUG("SECURITY SCENE NOTIFICATIONS sending email to %s\n", email_to);
	zwscn_security_scene_notification_get_message_strings(net, scene_ptr, message_type, email_subject, sizeof(email_subject), email_body_text, sizeof(email_body_text));
	if(zwscn_util_email_send(net, email_to, email_from, email_subject, email_body_text) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	return ZWSCN_ERR_NONE;
l_err:
	return ZWSCN_ERR_GENERIC;
}

static void zwscn_security_scene_notification_get_message_strings(void *net, zwscn_security_scene_p scene_ptr, uint32_t message_type, char *email_subject, uint32_t email_subject_len, char *email_body_text, uint32_t email_body_text_len) {
//	const char *event_cc_string = NULL;
	uint32_t event_id = 0;
//	uint16_t cls_id = 0;
	char *scene_name_string = NULL;
	char *time_string = NULL;
	char trigger_string[ZWSCN_TEMP_STR_MAX] = {0};
	
	scene_name_string = zwscn_util_unescape_uri(scene_ptr->name);
	switch(message_type) {
		case ZWSCN_MESSAGE_TYPE_SECURITY_SCENE_NOTIFICATION_ALARM:
		{
			if((scene_ptr->lt_alarm_on.type == ZWSCN_STATELOG_TRIGGER_EVENT) && (scene_ptr->lt_alarm_on.event != NULL)) {
				event_id =  scene_ptr->lt_alarm_on.event->id;
//				cls_id = zwscn_util_combo_id_to_cls_id(event_id);
				zwscn_zwave_get_trigger_string(net, scene_ptr->lt_alarm_on.event, trigger_string, sizeof(trigger_string));
			}
			time_string = zwscn_timezone_get_network_time_from_timestamp(net, scene_ptr->lt_alarm_on.time);
			snprintf(email_subject, email_subject_len, "Z-Ware: Alarm! Scene: %s, Trigger: %s", scene_name_string, trigger_string);
			snprintf(email_body_text, email_body_text_len, "Security Scene: \"%s\" (%u)\nState: Alarmed\nTrigger: \"%s\" (%u)\nTime: %s\n", 
			scene_name_string,
			scene_ptr->id, 
			trigger_string,
			event_id,
			time_string);
		}
		break;
		case ZWSCN_MESSAGE_TYPE_SECURITY_SCENE_NOTIFICATION_ADD:
		{
			time_string = zwscn_timezone_get_network_time_from_timestamp(net, scene_ptr->utime);
			snprintf(email_subject, email_subject_len, "Z-Ware: Info! Scene: %s CREATED at %s", scene_name_string, time_string);
			snprintf(email_body_text, email_body_text_len, "Security Scene: \"%s\" (%u)\nOperation: Create\nTime: %s\n", 
			scene_name_string,
			scene_ptr->id,
			time_string);
		}
		break;
		case ZWSCN_MESSAGE_TYPE_SECURITY_SCENE_NOTIFICATION_EDIT:
		{
			time_string = zwscn_timezone_get_network_time_from_timestamp(net, scene_ptr->utime);
			snprintf(email_subject, email_subject_len, "Z-Ware: Info! Scene: %s EDITED at %s", scene_name_string, time_string);
			snprintf(email_body_text, email_body_text_len, "Security Scene: \"%s\" (%u)\nOperation: Edit\nTime: %s\n", 
			scene_name_string,
			scene_ptr->id,
			time_string);
		}
		break;
	}
	free(scene_name_string);
	zwscn_timezone_free_network_time_string(time_string);
	
}

ZWSCN_STATUS_T zwscn_zwave_get_trigger_string(void *net, zwscn_security_event_p event, char *output_buf, size_t output_buf_size) {
	uint8_t nodeid;
	uint8_t epid;
	uint16_t cls;
	char str_1[ZWSCN_NODE_STR_MAX] = {0};
	const char *str_2 = NULL;
	char str_3[ZWSCN_TEMP_STR_MAX] = {0};
	char name[ZWSCN_LOC_STR_MAX + 1] = {0};
    char loc[ZWSCN_LOC_STR_MAX + 1] = {0};
	zwscn_init_p init_data;
	char *separator = "-";
	int result = 0;
	
	if((net == NULL) || (event == NULL) || (output_buf == NULL) || (output_buf_size == 0)) {
		goto l_err_args;
	}
	
	nodeid = zwscn_util_combo_id_to_node_id(event->id);
	epid = zwscn_util_combo_id_to_ep_id(event->id);
	cls = zwscn_util_combo_id_to_cls_id(event->id);
	
	if(epid == 0) {
		snprintf(str_1, sizeof(str_1), "Node %u", nodeid);
	} else {
		snprintf(str_1, sizeof(str_1), "Node %u/%u", nodeid, epid);
	}
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->init_input && init_data->init_input->zwscn_function_get_nameloc_handler) {
		result = init_data->init_input->zwscn_function_get_nameloc_handler(net, nodeid, epid, cls, name, sizeof(name), loc, sizeof(loc));
		if(result < 0) {
			zwscn_message_log_warning("Function get_nameloc_handler return error (%d)\n", result);
		}
	}
	str_2 = zwscn_zwave_get_cc_string(cls);
	zwscn_zwave_get_cc_args_string(cls, event->device_args, str_3, sizeof(str_3));
	
	if((strlen(str_1) > 0)) {
		zwscn_util_strcat(output_buf, str_1, output_buf_size);
		
	}
	if((strlen(name) > 0)) {
		zwscn_util_strcat(output_buf, separator, output_buf_size);
		zwscn_util_strcat(output_buf, name, output_buf_size);
		
	}
	if((strlen(loc) > 0)) {
		zwscn_util_strcat(output_buf, separator, output_buf_size);
		zwscn_util_strcat(output_buf, loc, output_buf_size);
		
	}
	if(str_2 && (strlen(str_2) > 0)) {
		zwscn_util_strcat(output_buf, separator, output_buf_size);
		zwscn_util_strcat(output_buf, str_2, output_buf_size);
	}
	if((strlen(str_3) > 0)) {
		zwscn_util_strcat(output_buf, separator, output_buf_size);
		zwscn_util_strcat(output_buf, str_3, output_buf_size);
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
}
