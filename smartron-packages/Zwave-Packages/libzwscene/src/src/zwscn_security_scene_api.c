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
@file		zwscn_security_scene_api.c  
  
                Implementation of Security Scenes API public
  
@author		Arif Mohammand
 
@version	1.0 2015-03-13  Initial release
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h> // memcpy
#include "zwscn_defines.h"
#include "zwscn_scene_api.h"
#include "zwscn_util.h"
#include "zwscn_util_hash.h"
#include "zwscn_util_list.h"
#include "zwscn_util_mem.h"
#include "zwscn_util_message_log.h"
#include "zwscn_util_mqueue.h"
#include "zwscn_util_mqueue_message.h"
#include "zwscn_util_thread.h"
#include "zwscn_util_mutex.h"
#include "zwscn_util_rwlock.h"
#include "zwscn_util_time.h"
#include "zwscn_util_timer.h"
#include "zwscn_util_zwave.h"
#include "zwscn_util_email.h"
#include "zwscn_statelog.h"
#include "zwscn_scene.h"
#include "zwscn_scene_events.h"
#include "zwscn_scene_status_update.h"
#include "zwscn_notification_handler.h"
#include "zwscn_notification_processor.h"
#include "zwscn_scene_execute.h"
#include "zwscn_setup_op.h"
#include "zwscn_security_scene_api.h"
#include "zwscn_security_scene.h"
#include "zwscn_persistence_security_scene.h"
#include "zwscn_security_scene_events.h"
#include "zwscn_security_scene_notifications.h"

ZWSCN_STATUS_T zwscn_add_security_scene(void* net, zwscn_security_scene_p scene, uint32_t* out_id) {
	zwscn_list_t *scene_list;
	zwscn_security_scene_cont_p scene_container_ptr;
	zwscn_security_scene_p scene_copy = NULL;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if ((net == NULL) || (scene == NULL) || (out_id == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_name=%s\n", net, scene->name);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	//TODO verify arm, disarm and alarm scenes' ids to be in the range of normal scenes
	scene_container_ptr = (zwscn_security_scene_cont_p) calloc(1, sizeof(zwscn_security_scene_cont_t));
	if(scene_container_ptr == NULL) {
		goto l_err_memory;
	}
	scene_container_ptr->net = net;
	scene_container_ptr->scene = (zwscn_security_scene_p) calloc(1, sizeof(zwscn_security_scene_t));
	if(scene_container_ptr->scene == NULL) {
		goto l_err_memory_1;
	}
	*(scene_container_ptr->scene) = *(scene); // C compiler in-built structure copy
	scene_container_ptr->scene->name[ZWSCN_LEN_SCENE_NAME - 1] = '\0';
	scene_container_ptr->scene->notification_sms_number[ZWSCN_LEN_SMS_NUMBER - 1] = '\0';
	scene_container_ptr->scene->notification_email[ZWSCN_LEN_EMAIL - 1] = '\0';
	scene_container_ptr->scene->lt_arm.event = NULL;
	scene_container_ptr->scene->lt_disarm.event = NULL;
	scene_container_ptr->scene->lt_alarm_on.event = NULL;
	scene_container_ptr->scene->lt_alarm_off.event = NULL;
	if((scene_container_ptr->scene->is_notification_by_sms_on) && (zwscn_util_sms_number_is_valid(scene_container_ptr->scene->notification_sms_number, ZWSCN_TRUE) != ZWSCN_TRUE)) {
		ZWSCN_MESSAGE_LOG_DEBUG("SMS NUMBER: %s is invalid\n", scene_container_ptr->scene->notification_sms_number);
		goto l_err_invalid_data;
	}
	if((scene_container_ptr->scene->is_notification_by_email_on) && (zwscn_util_email_is_valid(scene_container_ptr->scene->notification_email, ZWSCN_TRUE) != ZWSCN_TRUE)) {
		ZWSCN_MESSAGE_LOG_DEBUG("Email: %s is invalid\n", scene_container_ptr->scene->notification_email);
		goto l_err_invalid_data;
	}
	
	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	
	if(zwscn_find_security_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	if(zwscn_is_list_full_security_scene(net, (*scene_list))) {
		goto l_err_list_full;
	}
	if((scene_container_ptr->scene->id = zwscn_generate_security_scene_id(net)) == 0) {
		goto l_err;
	}
	if(((*scene_list) = zwscn_list_add((*scene_list), scene_container_ptr)) == NULL) {
		goto l_err;
	}
	if(zwscn_persistence_save_security_scene(net, scene_container_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_after_add;
	}
	zwscn_security_scene_update_timestamp(scene_container_ptr);
	//Send Security Scene Add notification
	scene_copy = calloc(1, sizeof(zwscn_security_scene_t));
	if(scene_copy == NULL) {
		zwscn_message_log_critical("Could not allocate memory needed for security scene notification request\n");
	} else {
		*(scene_copy) = *(scene_container_ptr->scene); //C structure copy
		scene_copy->lt_arm.event = NULL;
		scene_copy->lt_disarm.event = NULL;
		scene_copy->lt_alarm_on.event = NULL;
		scene_copy->lt_alarm_off.event = NULL;
		zwscn_security_scene_notification_request_message_send(net, scene_copy, ZWSCN_MESSAGE_TYPE_SECURITY_SCENE_NOTIFICATION_ADD);
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_name=%s returned id=%u\n", net, scene->name, scene_container_ptr->scene->id);
	(*out_id) = scene_container_ptr->scene->id;
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_ADD, scene_container_ptr->scene->id);
	zwscn_network_unlock_writer(net);
	
	
	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory_1:
	free(scene_container_ptr);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_invalid_data:
	free(scene_container_ptr->scene);
	free(scene_container_ptr);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_list_full:
	zwscn_network_unlock_writer(net);
	free(scene_container_ptr->scene);
	free(scene_container_ptr);
	RETURN(ZWSCN_ERR_LIST_FULL);
l_err:
	zwscn_network_unlock_writer(net);
	free(scene_container_ptr->scene);
	free(scene_container_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_after_add:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_add_arm_security_event(void* net, uint32_t scene_id, zwscn_security_event_p event, uint32_t* out_id) {
	zwscn_list_t *event_list;
	zwscn_security_event_p event_ptr;
	zwscn_security_event_p event_ptr_temp;
	uint8_t node_id, ep_id;
	uint16_t cls_id;
	zwscn_init_p init_data;
	zwscn_security_scene_cont_p scene_ptr;
	int result=0;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if ((net == NULL) || (event == NULL) || (scene_id == 0) || (out_id == NULL)) {
		goto l_err_args;
	}
	if((event->device_args == NULL) && (event->device_args_size > 0)) {
		zwscn_message_log_warning("zwscn_add_arm_security_event: device_args_size(%u) is not zero while device_args is NULL\n", event->device_args_size);
		goto l_err_args;
	}
	if(event->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
		zwscn_message_log_warning("zwscn_add_arm_security_event: device_args_size(%u) is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%u)\n", event->device_args_size, ZWSCN_DEVICE_ARGS_MAX_SIZE);
		goto l_err_args;
	}
	cls_id = zwscn_util_combo_id_to_cls_id(event->id);
	if(zwscn_zwave_is_cc_for_arm_security_event(cls_id, event->cmd) != ZWSCN_TRUE) {
		zwscn_message_log_warning("zwscn_add_arm_security_event: Z-Wave Command Class 0x%x is not valid for Arm events\n", cls_id);
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	event_ptr = (zwscn_security_event_p) calloc(1, sizeof(zwscn_security_event_t));
	if(event_ptr == NULL) {
		goto l_err_memory;
	}
	*(event_ptr) = *(event); // C compiler in-built structure copy
	if(event->device_args && (event->device_args_size != 0)) {
		event_ptr->device_args = calloc(1, event->device_args_size);
		if(!event_ptr->device_args) {
			goto l_err_memory_2;
		}
		memcpy(event_ptr->device_args, event->device_args, event->device_args_size);
	} else {
		event_ptr->device_args = NULL;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_security_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_scene_not_found;
	}
#if ZWSCN_CHECK_DUPLICATE_IN_SAME_SECURITY_EVENSTS
	if(zwscn_find_arm_security_event(net, scene_id, event->id, &event_ptr_temp) == ZWSCN_ERR_NONE) { //Event exists with the same id
		goto l_err_duplicate_id_event;
	}
#endif
#if ZWSCN_CHECK_DUPLICATE_IN_OTHER_SECURITY_EVENSTS
	if(zwscn_security_scene_has_same_disarm_event(scene_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_id_event;
	}
	if(zwscn_security_scene_has_same_alarm_event(scene_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_id_event;
	}
#endif
	node_id = zwscn_util_combo_id_to_node_id(event->id);
	ep_id = zwscn_util_combo_id_to_ep_id(event->id);
	cls_id = zwscn_util_combo_id_to_cls_id(event->id);
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->init_input && init_data->init_input->zwscn_fn_is_valid_handler) {
		result = init_data->init_input->zwscn_fn_is_valid_handler(net, ZWSCN_CHECK_NODE_EP_CLS, node_id, ep_id, cls_id);
		if(!result) {
			goto l_err_device_invalid;
		}
	} else {
		zwscn_message_log_warning("Function zwscn_fn_is_valid_handler is NULL for net=%p Scene_id=%d\n", net, scene_id);
		goto l_err_callback_null;
	}
	if(zwscn_find_arm_security_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	if(zwscn_is_list_full_arm_security_event(net, (*event_list))) {
		goto l_err_list_full;
	}

	if(((*event_list) = zwscn_list_add((*event_list), event_ptr)) == NULL) {
		goto l_err;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	if(zwscn_persistence_save_security_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_after_add;
	}
	if(zwscn_setup_op_request(net, ZWSCN_TYPE_ARM_SECURITY_EVENT, event_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_after_add;
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_ptr->scene->id);
	zwscn_network_unlock_writer(net);
	
	(*out_id) = event->id;

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_memory_2:
	free(event_ptr);
	RETURN(ZWSCN_ERR_MEMORY);
l_err_scene_not_found:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_duplicate_id_event:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_EVENT_EXISTS);
l_err_list_full:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_LIST_FULL);
l_err:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_device_invalid:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_INVALID_DEVICE);
l_err_callback_null:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_CALLBACK_NULL);
l_err_after_add:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_add_disarm_security_event(void* net, uint32_t scene_id, zwscn_security_event_p event, uint32_t* out_id) {
	zwscn_list_t *event_list;
	zwscn_security_event_p event_ptr;
	zwscn_security_event_p event_ptr_temp;
	uint8_t node_id, ep_id;
	uint16_t cls_id;
	zwscn_init_p init_data;
	zwscn_security_scene_cont_p scene_ptr;
	int result=0;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if ((net == NULL) || (event == NULL) || (scene_id == 0) || (out_id == NULL)) {
		goto l_err_args;
	}
	if((event->device_args == NULL) && (event->device_args_size > 0)) {
		zwscn_message_log_warning("zwscn_add_disarm_security_event: device_args_size(%u) is not zero while device_args is NULL\n", event->device_args_size);
		goto l_err_args;
	}
	if(event->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
		zwscn_message_log_warning("zwscn_add_disarm_security_event: device_args_size(%u) is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%u)\n", event->device_args_size, ZWSCN_DEVICE_ARGS_MAX_SIZE);
		goto l_err_args;
	}
	cls_id = zwscn_util_combo_id_to_cls_id(event->id);
	if(zwscn_zwave_is_cc_for_arm_security_event(cls_id, event->cmd) != ZWSCN_TRUE) { //The same function works for Arm and Disarm events
		zwscn_message_log_warning("zwscn_add_disarm_security_event: Z-Wave Command Class 0x%x is not valid for Disarm events\n", cls_id);
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	event_ptr = (zwscn_security_event_p) calloc(1, sizeof(zwscn_security_event_t));
	if(event_ptr == NULL) {
		goto l_err_memory;
	}
	*(event_ptr) = *(event); // C compiler in-built structure copy
	if(event->device_args && (event->device_args_size != 0)) {
		event_ptr->device_args = calloc(1, event->device_args_size);
		if(!event_ptr->device_args) {
			goto l_err_memory_2;
		}
		memcpy(event_ptr->device_args, event->device_args, event->device_args_size);
	} else {
		event_ptr->device_args = NULL;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_security_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_scene_not_found;
	}
#if ZWSCN_CHECK_DUPLICATE_IN_SAME_SECURITY_EVENSTS
	if(zwscn_find_disarm_security_event(net, scene_id, event->id, &event_ptr_temp) == ZWSCN_ERR_NONE) { //Event exists with the same id
		goto l_err_duplicate_id_event;
	}
#endif
#if ZWSCN_CHECK_DUPLICATE_IN_OTHER_SECURITY_EVENSTS
	if(zwscn_security_scene_has_same_arm_event(scene_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_id_event;
	}
	if(zwscn_security_scene_has_same_alarm_event(scene_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_id_event;
	}
#endif
	node_id = zwscn_util_combo_id_to_node_id(event->id);
	ep_id = zwscn_util_combo_id_to_ep_id(event->id);
	cls_id = zwscn_util_combo_id_to_cls_id(event->id);
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->init_input && init_data->init_input->zwscn_fn_is_valid_handler) {
		result = init_data->init_input->zwscn_fn_is_valid_handler(net, ZWSCN_CHECK_NODE_EP_CLS, node_id, ep_id, cls_id);
		if(!result) {
			goto l_err_device_invalid;
		}
	} else {
		zwscn_message_log_warning("Function zwscn_fn_is_valid_handler is NULL for net=%p Scene_id=%d\n", net, scene_id);
		goto l_err_callback_null;
	}
	if(zwscn_find_disarm_security_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	if(zwscn_is_list_full_disarm_security_event(net, (*event_list))) {
		goto l_err_list_full;
	}

	if(((*event_list) = zwscn_list_add((*event_list), event_ptr)) == NULL) {
		goto l_err;
	}
	
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	if(zwscn_persistence_save_security_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_after_add;
	}
	if(zwscn_setup_op_request(net, ZWSCN_TYPE_DISARM_SECURITY_EVENT, event_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_after_add;
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_ptr->scene->id);
	zwscn_network_unlock_writer(net);
	
	(*out_id) = event->id;

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_memory_2:
	free(event_ptr);
	RETURN(ZWSCN_ERR_MEMORY);
l_err_scene_not_found:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_duplicate_id_event:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_EVENT_EXISTS);
l_err_list_full:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_LIST_FULL);
l_err:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_device_invalid:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_INVALID_DEVICE);
l_err_callback_null:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_CALLBACK_NULL);
l_err_after_add:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_add_alarm_security_event(void* net, uint32_t scene_id, zwscn_security_event_p event, uint32_t* out_id) {
	zwscn_list_t *event_list;
	zwscn_security_event_p event_ptr;
//	zwscn_security_event_p event_ptr_temp;
	uint8_t node_id, ep_id;
	uint16_t cls_id;
	zwscn_init_p init_data;
	zwscn_security_scene_cont_p scene_ptr;
	int result=0;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if ((net == NULL) || (event == NULL) || (scene_id == 0) || (out_id == NULL)) {
		goto l_err_args;
	}
	if((event->device_args == NULL) && (event->device_args_size > 0)) {
		zwscn_message_log_warning("zwscn_add_alarm_security_event: device_args_size(%u) is not zero while device_args is NULL\n", event->device_args_size);
		goto l_err_args;
	}
	if(event->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
		zwscn_message_log_warning("zwscn_add_alarm_security_event: device_args_size(%u) is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%u)\n", event->device_args_size, ZWSCN_DEVICE_ARGS_MAX_SIZE);
		goto l_err_args;
	}
	cls_id = zwscn_util_combo_id_to_cls_id(event->id);
	if(zwscn_zwave_is_cc_for_alarm_security_event(cls_id, event->cmd) != ZWSCN_TRUE) {
		zwscn_message_log_warning("zwscn_add_alarm_security_event: Z-Wave Command Class 0x%x is not valid for Alarm events\n", cls_id);
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	event_ptr = (zwscn_security_event_p) calloc(1, sizeof(zwscn_security_event_t));
	if(event_ptr == NULL) {
		goto l_err_memory;
	}
	*(event_ptr) = *(event); // C compiler in-built structure copy
	if(event->device_args && (event->device_args_size != 0)) {
		event_ptr->device_args = calloc(1, event->device_args_size);
		if(!event_ptr->device_args) {
			goto l_err_memory_2;
		}
		memcpy(event_ptr->device_args, event->device_args, event->device_args_size);
	} else {
		event_ptr->device_args = NULL;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_security_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_scene_not_found;
	}
#if ZWSCN_CHECK_DUPLICATE_IN_SAME_SECURITY_EVENSTS
	if(zwscn_security_scene_has_same_alarm_event(scene_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_id_event;
	}
#endif
#if ZWSCN_CHECK_DUPLICATE_IN_OTHER_SECURITY_EVENSTS
	if(zwscn_security_scene_has_same_arm_event(scene_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_id_event;
	}
	if(zwscn_security_scene_has_same_disarm_event(scene_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_id_event;
	}
#endif
	node_id = zwscn_util_combo_id_to_node_id(event->id);
	ep_id = zwscn_util_combo_id_to_ep_id(event->id);
	cls_id = zwscn_util_combo_id_to_cls_id(event->id);
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->init_input && init_data->init_input->zwscn_fn_is_valid_handler) {
		result = init_data->init_input->zwscn_fn_is_valid_handler(net, ZWSCN_CHECK_NODE_EP_CLS, node_id, ep_id, cls_id);
		if(!result) {
			goto l_err_device_invalid;
		}
	} else {
		zwscn_message_log_warning("Function zwscn_fn_is_valid_handler is NULL for net=%p Scene_id=%d\n", net, scene_id);
		goto l_err_callback_null;
	}
	if(zwscn_find_alarm_security_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	if(zwscn_is_list_full_alarm_security_event(net, (*event_list))) {
		goto l_err_list_full;
	}

	if(((*event_list) = zwscn_list_add((*event_list), event_ptr)) == NULL) {
		goto l_err;
	}
	
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	if(zwscn_persistence_save_security_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_after_add;
	}
	if(zwscn_setup_op_request(net, ZWSCN_TYPE_ALARM_SECURITY_EVENT, event_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_after_add;
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_ptr->scene->id);
	zwscn_network_unlock_writer(net);
	
	(*out_id) = event->id;

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_memory_2:
	free(event_ptr);
	RETURN(ZWSCN_ERR_MEMORY);
l_err_scene_not_found:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_duplicate_id_event:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_EVENT_EXISTS);
l_err_list_full:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_LIST_FULL);
l_err:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_device_invalid:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_INVALID_DEVICE);
l_err_callback_null:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_CALLBACK_NULL);
l_err_after_add:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_security_scene(void* net, uint32_t scene_id) {
	zwscn_list_t *scene_list;
	zwscn_security_scene_cont_p scene_ptr=NULL;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_security_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	scene_ptr = zwscn_list_get_security_scene_by_id((*scene_list), scene_id);
	if(scene_ptr == NULL) {
		goto l_err_invalid_id;;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	if(zwscn_persistence_delete_security_scene(net, scene_id) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	(*scene_list) = zwscn_list_remove((*scene_list), scene_ptr);
	zwscn_free_security_scene(scene_ptr);
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_DELETE, scene_id);
	zwscn_network_unlock_writer(net);
	
	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_invalid_id:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_arm_security_event(void* net, uint32_t scene_id, uint32_t event_id) {
	zwscn_list_t *event_list;
	zwscn_security_event_p event_ptr=NULL;
	zwscn_security_scene_cont_p scene_ptr=NULL;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d, event_id=0x%x\n", net, scene_id, event_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_arm_security_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	event_ptr = zwscn_list_get_security_event_by_id((*event_list), event_id);
	if(event_ptr == NULL) {
		goto l_err_invalid_id;
	}
	(*event_list) = zwscn_list_remove((*event_list), event_ptr);
	zwscn_free_security_event(event_ptr);
	if(zwscn_find_security_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
//	zwscn_scene_triggers_info_update(net, scene_ptr); //TODO
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s event_id=0x%x\n", net, scene_id, scene_ptr->scene->name, event_id);
	if(zwscn_persistence_save_security_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_id);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_invalid_id:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_disarm_security_event(void* net, uint32_t scene_id, uint32_t event_id) {
	zwscn_list_t *event_list;
	zwscn_security_event_p event_ptr=NULL;
	zwscn_security_scene_cont_p scene_ptr=NULL;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d, event_id=0x%x\n", net, scene_id, event_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_disarm_security_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	event_ptr = zwscn_list_get_security_event_by_id((*event_list), event_id);
	if(event_ptr == NULL) {
		goto l_err_invalid_id;
	}
	(*event_list) = zwscn_list_remove((*event_list), event_ptr);
	zwscn_free_security_event(event_ptr);
	if(zwscn_find_security_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
//	zwscn_scene_triggers_info_update(net, scene_ptr); //TODO
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s event_id=0x%x\n", net, scene_id, scene_ptr->scene->name, event_id);
	if(zwscn_persistence_save_security_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_id);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_invalid_id:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_alarm_security_event(void* net, uint32_t scene_id, uint32_t event_id) {
	zwscn_list_t *event_list;
	zwscn_security_event_p event_ptr=NULL;
	zwscn_security_scene_cont_p scene_ptr=NULL;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d, event_id=0x%x\n", net, scene_id, event_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_alarm_security_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	event_ptr = zwscn_list_get_security_event_by_id((*event_list), event_id);
	if(event_ptr == NULL) {
		goto l_err_invalid_id;
	}
	(*event_list) = zwscn_list_remove((*event_list), event_ptr);
	zwscn_free_security_event(event_ptr);
	if(zwscn_find_security_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
//	zwscn_scene_triggers_info_update(net, scene_ptr); //TODO
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s event_id=0x%x\n", net, scene_id, scene_ptr->scene->name, event_id);
	if(zwscn_persistence_save_security_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
 	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_id);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_invalid_id:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

int zwscn_get_default_max_num_security_scenes() {
	return ZWSCN_MAX_SECURITY_SCENES;
}

int zwscn_get_default_max_num_arm_security_events() {
	return ZWSCN_MAX_ARM_SECURITY_EVENTS;
}

int zwscn_get_default_max_num_disarm_security_events() {
	return ZWSCN_MAX_DISARM_SECURITY_EVENTS;
}

int zwscn_get_default_max_num_alarm_security_events() {
	return ZWSCN_MAX_ALARM_SECURITY_EVENTS;
}

int zwscn_get_max_num_security_scenes(void* net) {
	zwscn_init_p scene_init_data;
	int max_limit=0;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p\n", net);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	if(scene_init_data->init_input) {
		max_limit = (scene_init_data->init_input->num_max_security_scenes < 0) ? ZWSCN_MAX_SECURITY_SCENES : scene_init_data->init_input->num_max_security_scenes;
	}
	zwscn_network_unlock_reader(net);
	
	return max_limit;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

int zwscn_get_max_num_arm_security_events(void* net) {
//	zwscn_init_p init_data;
	int max_limit=0;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p\n", net);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
//	if((init_data = zwscn_get_init_data(net)) == NULL) {
//		goto l_err;
//	}
//	if(init_data->init_input) {
//		max_limit = (init_data->init_input->num_max_arm_security_events_per_scene < 0) ? ZWSCN_MAX_ARM_SECURITY_EVENTS : init_data->init_input->num_max_arm_security_events_per_scene;
//	}
	max_limit = zwscn_get_max_arm_events(net);
	zwscn_network_unlock_reader(net);
	
	return max_limit;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
//l_err:
//	zwscn_network_unlock_reader(net);
//	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

int zwscn_get_max_num_disarm_security_events(void* net) {
//	zwscn_init_p init_data;
	int max_limit=0;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p\n", net);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
//	if((init_data = zwscn_get_init_data(net)) == NULL) {
//		goto l_err;
//	}
//	if(init_data->init_input) {
//		max_limit = (init_data->init_input->num_max_disarm_security_events_per_scene < 0) ? ZWSCN_MAX_DISARM_SECURITY_EVENTS : init_data->init_input->num_max_disarm_security_events_per_scene;
//	}
	max_limit = zwscn_get_max_disarm_events(net);
	zwscn_network_unlock_reader(net);
	
	return max_limit;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
//l_err:
//	zwscn_network_unlock_reader(net);
//	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

int zwscn_get_max_num_alarm_security_events(void* net) {
//	zwscn_init_p init_data;
	int max_limit=0;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p\n", net);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
//	if((init_data = zwscn_get_init_data(net)) == NULL) {
//		goto l_err;
//	}
//	if(init_data->init_input) {
//		max_limit = (init_data->init_input->num_max_alarm_security_events_per_scene < 0) ? ZWSCN_MAX_ALARM_SECURITY_EVENTS : init_data->init_input->num_max_alarm_security_events_per_scene;
//	}
	max_limit = zwscn_get_max_alarm_events(net);
	zwscn_network_unlock_reader(net);
	
	return max_limit;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
//l_err:
//	zwscn_network_unlock_reader(net);
//	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

int zwscn_count_security_scenes(void* net) {
	zwscn_list_t *scene_list;
	int count;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p\n", net);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_security_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	count = zwscn_list_size((*scene_list));
	zwscn_network_unlock_reader(net);
	
	return count;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

int zwscn_count_arm_security_events(void* net, uint32_t scene_id) {
	zwscn_list_t *event_list;
	int count;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_arm_security_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	count = zwscn_list_size((*event_list));
	zwscn_network_unlock_reader(net);
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d arm_events_count=%d\n", net, scene_id, count);
	
	return count;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

int zwscn_count_disarm_security_events(void* net, uint32_t scene_id) {
	zwscn_list_t *event_list;
	int count;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_disarm_security_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	count = zwscn_list_size((*event_list));
	zwscn_network_unlock_reader(net);
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d disarm_events_count=%d\n", net, scene_id, count);
	
	return count;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

int zwscn_count_alarm_security_events(void* net, uint32_t scene_id) {
	zwscn_list_t *event_list;
	int count;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_alarm_security_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	count = zwscn_list_size((*event_list));
	zwscn_network_unlock_reader(net);
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d alarm_events_count=%d\n", net, scene_id, count);
	
	return count;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_get_security_scene(void* net, uint32_t scene_id, zwscn_security_scene_p* out_scene) {
	zwscn_security_scene_cont_p scene_ptr;
	zwscn_security_scene_p output_scene;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (out_scene == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	output_scene = (zwscn_security_scene_p) calloc(1, sizeof(zwscn_security_scene_t));
	if(!output_scene) {
		goto l_err_memory;
	}
	
	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_security_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	//*(output_scene) = *(scene_ptr->scene); // C compiler in-built structure copy
	zwscn_security_scene_copy(output_scene, scene_ptr->scene);
	zwscn_network_unlock_reader(net);
	
	*(out_scene) = output_scene; //NOTE: this must be freed by the application 

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err:
	zwscn_network_unlock_reader(net);
	free(output_scene);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_get_arm_security_event(void* net, uint32_t scene_id, uint32_t event_id, zwscn_security_event_p* out_scene_event) {
//	zwscn_list_t event_list;
	zwscn_security_event_p event_ptr;
	zwscn_security_event_p output_event_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (event_id == 0) || (out_scene_event == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d event_id=0x%x\n", net, scene_id, event_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	output_event_ptr = (zwscn_security_event_p) calloc(1, sizeof(zwscn_security_event_t));
	if(!output_event_ptr) {
		goto l_err_memory;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_arm_security_event(net, scene_id, event_id, &event_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}

	*(output_event_ptr) = *(event_ptr); // C compiler in-built structure copy
	if((event_ptr->device_args != NULL) && (event_ptr->device_args_size != 0)) {
		output_event_ptr->device_args = malloc(event_ptr->device_args_size);
		if(output_event_ptr->device_args == NULL) {
			goto l_err_memory_2;
		}
		memcpy(output_event_ptr->device_args, event_ptr->device_args, event_ptr->device_args_size);
	} else {
		output_event_ptr->device_args = NULL;
		output_event_ptr->device_args_size = 0;
	}
	zwscn_network_unlock_reader(net);
	
	*(out_scene_event) = output_event_ptr; //NOTE: this must be freed by the application 

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_memory_2:
	zwscn_network_unlock_reader(net);
	free(output_event_ptr);
	RETURN(ZWSCN_ERR_MEMORY);
l_err:
	zwscn_network_unlock_reader(net);
	free(output_event_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_get_disarm_security_event(void* net, uint32_t scene_id, uint32_t event_id, zwscn_security_event_p* out_scene_event) {
//	zwscn_list_t event_list;
	zwscn_security_event_p event_ptr;
	zwscn_security_event_p output_event_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (event_id == 0) || (out_scene_event == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d event_id=0x%x\n", net, scene_id, event_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	output_event_ptr = (zwscn_security_event_p) calloc(1, sizeof(zwscn_security_event_t));
	if(!output_event_ptr) {
		goto l_err_memory;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_disarm_security_event(net, scene_id, event_id, &event_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}

	*(output_event_ptr) = *(event_ptr); // C compiler in-built structure copy
	if((event_ptr->device_args != NULL) && (event_ptr->device_args_size != 0)) {
		output_event_ptr->device_args = malloc(event_ptr->device_args_size);
		if(output_event_ptr->device_args == NULL) {
			goto l_err_memory_2;
		}
		memcpy(output_event_ptr->device_args, event_ptr->device_args, event_ptr->device_args_size);
	} else {
		output_event_ptr->device_args = NULL;
		output_event_ptr->device_args_size = 0;
	}
	zwscn_network_unlock_reader(net);
	
	*(out_scene_event) = output_event_ptr; //NOTE: this must be freed by the application 

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_memory_2:
	zwscn_network_unlock_reader(net);
	free(output_event_ptr);
	RETURN(ZWSCN_ERR_MEMORY);
l_err:
	zwscn_network_unlock_reader(net);
	free(output_event_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_get_alarm_security_event(void* net, uint32_t scene_id, uint32_t event_id, zwscn_security_event_p* out_scene_event) {
//	zwscn_list_t event_list;
	zwscn_security_event_p event_ptr;
	zwscn_security_event_p output_event_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (event_id == 0) || (out_scene_event == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d event_id=0x%x\n", net, scene_id, event_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	output_event_ptr = (zwscn_security_event_p) calloc(1, sizeof(zwscn_security_event_t));
	if(!output_event_ptr) {
		goto l_err_memory;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_alarm_security_event(net, scene_id, event_id, &event_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}

	*(output_event_ptr) = *(event_ptr); // C compiler in-built structure copy
	if((event_ptr->device_args != NULL) && (event_ptr->device_args_size != 0)) {
		output_event_ptr->device_args = malloc(event_ptr->device_args_size);
		if(output_event_ptr->device_args == NULL) {
			goto l_err_memory_2;
		}
		memcpy(output_event_ptr->device_args, event_ptr->device_args, event_ptr->device_args_size);
	} else {
		output_event_ptr->device_args = NULL;
		output_event_ptr->device_args_size = 0;
	}
	zwscn_network_unlock_reader(net);
	
	*(out_scene_event) = output_event_ptr; //NOTE: this must be freed by the application 

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_memory_2:
	zwscn_network_unlock_reader(net);
	free(output_event_ptr);
	RETURN(ZWSCN_ERR_MEMORY);
l_err:
	zwscn_network_unlock_reader(net);
	free(output_event_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_get_security_scene_by_index(void* net, uint32_t index, zwscn_security_scene_p* out_scene) {
	zwscn_list_t *scene_list;
	zwscn_security_scene_cont_p scene_ptr;
	zwscn_security_scene_p output_scene;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if ((!net) || (out_scene == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene index=%d\n", net, index);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	output_scene = (zwscn_security_scene_p) calloc(1, sizeof(zwscn_security_scene_t));
	if(!output_scene) {
		goto l_err_memory;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_security_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	scene_ptr = zwscn_list_get_by_index((*scene_list), index);
	if(scene_ptr == NULL) {
		goto l_err;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene index=%d scene_id=%d scene_name=%s\n", net, index, scene_ptr->scene->id, scene_ptr->scene->name);
	//*(output_scene) = *(scene_ptr->scene); // C compiler in-built structure copy
	zwscn_security_scene_copy(output_scene, scene_ptr->scene);
	zwscn_network_unlock_reader(net);
	
	*(out_scene) = output_scene; //NOTE: this must be freed by the application 

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err:
	zwscn_network_unlock_reader(net);
	free(output_scene);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_get_arm_security_event_by_index(void* net, uint32_t scene_id, uint32_t index, zwscn_security_event_p* out_scene_event) {
	zwscn_list_t *event_list;
	zwscn_security_event_p event_ptr;
	zwscn_security_event_p output_event;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (out_scene_event == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d index=%d\n", net, scene_id, index);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	output_event = (zwscn_security_event_p) calloc(1, sizeof(zwscn_security_event_t));
	if(!output_event) {
		goto l_err_memory;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_arm_security_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	event_ptr = zwscn_list_get_by_index((*event_list), index);
	if(event_ptr == NULL) {
		goto l_err_invalid_index;
	}
	*(output_event) = *(event_ptr); // C compiler in-built structure copy
	if((event_ptr->device_args != NULL) && (event_ptr->device_args_size != 0)) {
		output_event->device_args = malloc(event_ptr->device_args_size);
		if(output_event->device_args == NULL) {
			goto l_err_memory_2;
		}
		memcpy(output_event->device_args, event_ptr->device_args, event_ptr->device_args_size);
	} else {
		output_event->device_args = NULL;
		output_event->device_args_size = 0;
	}
	zwscn_network_unlock_reader(net);
	
	*(out_scene_event) = output_event; //NOTE: this must be freed by the application 

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_memory_2:
	zwscn_network_unlock_reader(net);
	free(output_event);
	RETURN(ZWSCN_ERR_MEMORY);
l_err_invalid_index:
	zwscn_network_unlock_reader(net);
	free(output_event);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err:
	zwscn_network_unlock_reader(net);
	free(output_event);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_get_disarm_security_event_by_index(void* net, uint32_t scene_id, uint32_t index, zwscn_security_event_p* out_scene_event) {
	zwscn_list_t *event_list;
	zwscn_security_event_p event_ptr;
	zwscn_security_event_p output_event;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (out_scene_event == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d index=%d\n", net, scene_id, index);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	output_event = (zwscn_security_event_p) calloc(1, sizeof(zwscn_security_event_t));
	if(!output_event) {
		goto l_err_memory;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_disarm_security_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	event_ptr = zwscn_list_get_by_index((*event_list), index);
	if(event_ptr == NULL) {
		goto l_err_invalid_index;
	}
	*(output_event) = *(event_ptr); // C compiler in-built structure copy
	if((event_ptr->device_args != NULL) && (event_ptr->device_args_size != 0)) {
		output_event->device_args = malloc(event_ptr->device_args_size);
		if(output_event->device_args == NULL) {
			goto l_err_memory_2;
		}
		memcpy(output_event->device_args, event_ptr->device_args, event_ptr->device_args_size);
	} else {
		output_event->device_args = NULL;
		output_event->device_args_size = 0;
	}
	zwscn_network_unlock_reader(net);
	
	*(out_scene_event) = output_event; //NOTE: this must be freed by the application 

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_memory_2:
	zwscn_network_unlock_reader(net);
	free(output_event);
	RETURN(ZWSCN_ERR_MEMORY);
l_err_invalid_index:
	zwscn_network_unlock_reader(net);
	free(output_event);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err:
	zwscn_network_unlock_reader(net);
	free(output_event);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_get_alarm_security_event_by_index(void* net, uint32_t scene_id, uint32_t index, zwscn_security_event_p* out_scene_event) {
	zwscn_list_t *event_list;
	zwscn_security_event_p event_ptr;
	zwscn_security_event_p output_event;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (out_scene_event == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d index=%d\n", net, scene_id, index);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	output_event = (zwscn_security_event_p) calloc(1, sizeof(zwscn_security_event_t));
	if(!output_event) {
		goto l_err_memory;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_alarm_security_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	event_ptr = zwscn_list_get_by_index((*event_list), index);
	if(event_ptr == NULL) {
		goto l_err_invalid_index;
	}
	*(output_event) = *(event_ptr); // C compiler in-built structure copy
	if((event_ptr->device_args != NULL) && (event_ptr->device_args_size != 0)) {
		output_event->device_args = malloc(event_ptr->device_args_size);
		if(output_event->device_args == NULL) {
			goto l_err_memory_2;
		}
		memcpy(output_event->device_args, event_ptr->device_args, event_ptr->device_args_size);
	} else {
		output_event->device_args = NULL;
		output_event->device_args_size = 0;
	}
	zwscn_network_unlock_reader(net);
	
	*(out_scene_event) = output_event; //NOTE: this must be freed by the application 

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_memory_2:
	zwscn_network_unlock_reader(net);
	free(output_event);
	RETURN(ZWSCN_ERR_MEMORY);
l_err_invalid_index:
	zwscn_network_unlock_reader(net);
	free(output_event);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err:
	zwscn_network_unlock_reader(net);
	free(output_event);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_all_security_scenes(void* net) {
	zwscn_list_t *scene_list;
	zwscn_init_p init_data;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p\n", net);
	
	if (!zwscn_does_net_exist(net)) {
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
	
	if(init_data->list_security_scenes != NULL) {
		scene_list = &(init_data->list_security_scenes);
		if(zwscn_persistence_delete_all_security_scenes(net, (*scene_list)) != ZWSCN_ERR_NONE) {
			//goto l_err; //Don't exit as we have to free the next list
		}
		zwscn_list_free_security_scene_list(scene_list); //this deletes and free all the member items and their child nodes
	}
	if(init_data->list_security_scenes_invalid != NULL) {
		scene_list = &(init_data->list_security_scenes_invalid);
		if(zwscn_persistence_delete_all_security_scenes(net, (*scene_list)) != ZWSCN_ERR_NONE) {
			//goto l_err; //Don't exit as we have to free the list
		}
		zwscn_list_free_security_scene_list(scene_list); //this deletes and free all the member items and their child nodes
	}
	//NOTE: zwscn_persistence_delete_all_security_scenes() calls zwscn_statelog_security_scene_set() multiple times
	zwscn_network_unlock_writer(net);
	
	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_all_security_events(void* net, uint32_t scene_id) {
//	zwscn_list_t *event_list;
	zwscn_security_scene_cont_p scene_ptr;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_security_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	if(scene_ptr->list_arm_events != NULL) {
		zwscn_list_free_arm_security_event_list(&(scene_ptr->list_arm_events)); //this deletes and free all the member items and their child nodes
	}
	if(scene_ptr->list_arm_events_invalid != NULL) {
		zwscn_list_free_arm_security_event_list(&(scene_ptr->list_arm_events_invalid)); //this deletes and free all the member items and their child nodes
	}
	if(scene_ptr->list_disarm_events != NULL) {
		zwscn_list_free_disarm_security_event_list(&(scene_ptr->list_disarm_events)); //this deletes and free all the member items and their child nodes
	}
	if(scene_ptr->list_disarm_events_invalid != NULL) {
		zwscn_list_free_disarm_security_event_list(&(scene_ptr->list_disarm_events_invalid)); //this deletes and free all the member items and their child nodes
	}
	if(scene_ptr->list_alarm_events != NULL) {
		zwscn_list_free_alarm_security_event_list(&(scene_ptr->list_alarm_events)); //this deletes and free all the member items and their child nodes
	}
	if(scene_ptr->list_alarm_events_invalid != NULL) {
		zwscn_list_free_alarm_security_event_list(&(scene_ptr->list_alarm_events_invalid)); //this deletes and free all the member items and their child nodes
	}
	if(zwscn_persistence_save_security_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_ptr->scene->id);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_all_arm_security_events(void* net, uint32_t scene_id) {
//	zwscn_list_t *event_list;
	zwscn_security_scene_cont_p scene_ptr;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_security_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	if(scene_ptr->list_arm_events != NULL) {
		zwscn_list_free_arm_security_event_list(&(scene_ptr->list_arm_events)); //this deletes and free all the member items and their child nodes
	}
	if(scene_ptr->list_arm_events_invalid != NULL) {
		zwscn_list_free_arm_security_event_list(&(scene_ptr->list_arm_events_invalid)); //this deletes and free all the member items and their child nodes
	}
	if(zwscn_persistence_save_security_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_ptr->scene->id);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_all_disarm_security_events(void* net, uint32_t scene_id) {
//	zwscn_list_t *event_list;
	zwscn_security_scene_cont_p scene_ptr;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_security_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	if(scene_ptr->list_disarm_events != NULL) {
		zwscn_list_free_disarm_security_event_list(&(scene_ptr->list_disarm_events)); //this deletes and free all the member items and their child nodes
	}
	if(scene_ptr->list_disarm_events_invalid != NULL) {
		zwscn_list_free_disarm_security_event_list(&(scene_ptr->list_disarm_events_invalid)); //this deletes and free all the member items and their child nodes
	}
	if(zwscn_persistence_save_security_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_ptr->scene->id);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_all_alarm_security_events(void* net, uint32_t scene_id) {
//	zwscn_list_t *event_list;
	zwscn_security_scene_cont_p scene_ptr;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_security_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	if(scene_ptr->list_alarm_events != NULL) {
		zwscn_list_free_alarm_security_event_list(&(scene_ptr->list_alarm_events)); //this deletes and free all the member items and their child nodes
	}
	if(scene_ptr->list_alarm_events_invalid != NULL) {
		zwscn_list_free_alarm_security_event_list(&(scene_ptr->list_alarm_events_invalid)); //this deletes and free all the member items and their child nodes
	}
	if(zwscn_persistence_save_security_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_ptr->scene->id);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_edit_security_scene(void* net, uint32_t scene_id, zwscn_security_scene_p scene) {
	zwscn_security_scene_cont_p scene_ptr;
	zwscn_security_scene_p scene_copy = NULL;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (scene == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);
	if((scene->is_notification_by_sms_on) && (zwscn_util_sms_number_is_valid(scene->notification_sms_number, ZWSCN_TRUE) != ZWSCN_TRUE)) {
		ZWSCN_MESSAGE_LOG_DEBUG("SMS NUMBER: %s is invalid\n", scene->notification_sms_number);
		goto l_err_args;
	}
	if((scene->is_notification_by_email_on) && (zwscn_util_email_is_valid(scene->notification_email, ZWSCN_TRUE) != ZWSCN_TRUE)) {
		ZWSCN_MESSAGE_LOG_DEBUG("Email: %s is invalid\n", scene->notification_email);
		goto l_err_args;
	}
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_security_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d old scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d new scene_name=%s\n", net, scene_id, scene->name);
	// Copy the editable data
	zwscn_util_strcpy(scene_ptr->scene->name, scene->name, sizeof(scene_ptr->scene->name));
	scene_ptr->scene->active = scene->active;
//	scene_ptr->scene->is_armed = scene->is_armed;			//NOTE: please use zwscn_security_scene_set_arm_state() separately
//	scene_ptr->scene->is_alarmed = scene->is_alarmed;		//NOTE: please use zwscn_security_scene_set_alarm_state() separately
	scene_ptr->scene->scene_id_at_arm = scene->scene_id_at_arm;
	scene_ptr->scene->scene_id_at_disarm = scene->scene_id_at_disarm;
	scene_ptr->scene->scene_id_at_alarm = scene->scene_id_at_alarm;
	scene_ptr->scene->is_notification_on = scene->is_notification_on;
	scene_ptr->scene->is_notification_by_sms_on = scene->is_notification_by_sms_on;
	scene_ptr->scene->is_notification_by_email_on = scene->is_notification_by_email_on;
	zwscn_util_strcpy(scene_ptr->scene->notification_sms_number, scene->notification_sms_number, sizeof(scene_ptr->scene->notification_sms_number));
	zwscn_util_strcpy(scene_ptr->scene->notification_email, scene->notification_email, sizeof(scene_ptr->scene->notification_email));
	
	if(zwscn_persistence_save_security_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
	//Send Security Scene Edit notification
	scene_copy = calloc(1, sizeof(zwscn_security_scene_t));
	if(scene_copy == NULL) {
		zwscn_message_log_critical("Could not allocate memory needed for security scene notification request\n");
	} else {
		*(scene_copy) = *(scene_ptr->scene); //C structure copy
		scene_copy->lt_arm.event = NULL;
		scene_copy->lt_disarm.event = NULL;
		scene_copy->lt_alarm_on.event = NULL;
		scene_copy->lt_alarm_off.event = NULL;
		zwscn_security_scene_notification_request_message_send(net, scene_copy, ZWSCN_MESSAGE_TYPE_SECURITY_SCENE_NOTIFICATION_EDIT);
	}
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_ptr->scene->id);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_edit_arm_security_event(void* net, uint32_t scene_id, uint32_t event_id, zwscn_security_event_p event) {
	zwscn_security_event_p event_ptr;
	void *temp_ptr;
	zwscn_security_scene_cont_p scene_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (event == NULL)) {
		goto l_err_args;
	}
	if((event->device_args == NULL) && (event->device_args_size > 0)) {
		zwscn_message_log_warning("zwscn_edit_arm_security_event: device_args_size(%d) is not zero while device_args is NULL\n", event->device_args_size);
		goto l_err_args;
	}
	if(event->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
		zwscn_message_log_warning("zwscn_edit_arm_security_event: device_args_size is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%d)\n", ZWSCN_DEVICE_ARGS_MAX_SIZE);
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d event_id=0x%x\n", net, scene_id, event_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_arm_security_event(net, scene_id, event_id, &event_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	if(zwscn_find_security_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_scene_not_found;
	}
#if ZWSCN_CHECK_DUPLICATE_IN_OTHER_SECURITY_EVENSTS
	if(zwscn_security_scene_has_same_disarm_event(scene_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_id_event;
	}
	if(zwscn_security_scene_has_same_alarm_event(scene_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_id_event;
	}
#endif
	// Copy the editable data
	if(event->device_args_size == 0) {
		free(event_ptr->device_args);
		event_ptr->device_args = NULL;
		event_ptr->device_args_size = event->device_args_size;
	} else if(event_ptr->device_args_size >= event->device_args_size) {
		memcpy(event_ptr->device_args, event->device_args, event->device_args_size);
		event_ptr->device_args_size = event->device_args_size;
	} else {
		temp_ptr = realloc(event_ptr->device_args, event->device_args_size);
		if(temp_ptr == NULL) {
			goto l_err_memory;
		}
		event_ptr->device_args = temp_ptr;
		memcpy(event_ptr->device_args, event->device_args, event->device_args_size);
		event_ptr->device_args_size = event->device_args_size;
	}
	event_ptr->type = event->type; //Copying this last as the other paramerts may result in memory error
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s event_id=0x%x\n", net, scene_id, scene_ptr->scene->name, event_id);
	if(zwscn_persistence_save_security_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_ptr->scene->id);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_MEMORY);
l_err:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_scene_not_found:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_duplicate_id_event:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_EVENT_EXISTS);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_edit_disarm_security_event(void* net, uint32_t scene_id, uint32_t event_id, zwscn_security_event_p event) {
	zwscn_security_event_p event_ptr;
	void *temp_ptr;
	zwscn_security_scene_cont_p scene_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (event == NULL)) {
		goto l_err_args;
	}
	if((event->device_args == NULL) && (event->device_args_size > 0)) {
		zwscn_message_log_warning("zwscn_edit_disarm_security_event: device_args_size(%d) is not zero while device_args is NULL\n", event->device_args_size);
		goto l_err_args;
	}
	if(event->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
		zwscn_message_log_warning("zwscn_edit_disarm_security_event: device_args_size is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%d)\n", ZWSCN_DEVICE_ARGS_MAX_SIZE);
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d event_id=0x%x\n", net, scene_id, event_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_disarm_security_event(net, scene_id, event_id, &event_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	if(zwscn_find_security_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_scene_not_found;
	}
#if ZWSCN_CHECK_DUPLICATE_IN_OTHER_SECURITY_EVENSTS
	if(zwscn_security_scene_has_same_arm_event(scene_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_id_event;
	}
	if(zwscn_security_scene_has_same_alarm_event(scene_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_id_event;
	}
#endif
	// Copy the editable data
	if(event->device_args_size == 0) {
		free(event_ptr->device_args);
		event_ptr->device_args = NULL;
		event_ptr->device_args_size = event->device_args_size;
	} else if(event_ptr->device_args_size >= event->device_args_size) {
		memcpy(event_ptr->device_args, event->device_args, event->device_args_size);
		event_ptr->device_args_size = event->device_args_size;
	} else {
		temp_ptr = realloc(event_ptr->device_args, event->device_args_size);
		if(temp_ptr == NULL) {
			goto l_err_memory;
		}
		event_ptr->device_args = temp_ptr;
		memcpy(event_ptr->device_args, event->device_args, event->device_args_size);
		event_ptr->device_args_size = event->device_args_size;
	}
	event_ptr->type = event->type; //Copying this last as the other paramerts may result in memory error
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s event_id=0x%x\n", net, scene_id, scene_ptr->scene->name, event_id);
	if(zwscn_persistence_save_security_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_ptr->scene->id);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_MEMORY);
l_err:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_scene_not_found:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_duplicate_id_event:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_EVENT_EXISTS);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_edit_alarm_security_event(void* net, uint32_t scene_id, uint32_t event_id, zwscn_security_event_p event) {
	zwscn_security_event_p event_ptr;
	void *temp_ptr;
	zwscn_security_scene_cont_p scene_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (event == NULL)) {
		goto l_err_args;
	}
	if((event->device_args == NULL) && (event->device_args_size > 0)) {
		zwscn_message_log_warning("zwscn_edit_alarm_security_event: device_args_size(%d) is not zero while device_args is NULL\n", event->device_args_size);
		goto l_err_args;
	}
	if(event->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
		zwscn_message_log_warning("zwscn_edit_alarm_security_event: device_args_size is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%d)\n", ZWSCN_DEVICE_ARGS_MAX_SIZE);
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d event_id=0x%x\n", net, scene_id, event_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_alarm_security_event(net, scene_id, event_id, &event_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	if(zwscn_find_security_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_scene_not_found;
	}
#if ZWSCN_CHECK_DUPLICATE_IN_OTHER_SECURITY_EVENSTS
	if(zwscn_security_scene_has_same_arm_event(scene_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_id_event;
	}
	if(zwscn_security_scene_has_same_disarm_event(scene_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_id_event;
	}
#endif
	// Copy the editable data
	if(event->device_args_size == 0) {
		free(event_ptr->device_args);
		event_ptr->device_args = NULL;
		event_ptr->device_args_size = event->device_args_size;
	} else if(event_ptr->device_args_size >= event->device_args_size) {
		memcpy(event_ptr->device_args, event->device_args, event->device_args_size);
		event_ptr->device_args_size = event->device_args_size;
	} else {
		temp_ptr = realloc(event_ptr->device_args, event->device_args_size);
		if(temp_ptr == NULL) {
			goto l_err_memory;
		}
		event_ptr->device_args = temp_ptr;
		memcpy(event_ptr->device_args, event->device_args, event->device_args_size);
		event_ptr->device_args_size = event->device_args_size;
	}
	event_ptr->type = event->type; //Copying this last as the other paramerts may result in memory error
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s event_id=0x%x\n", net, scene_id, scene_ptr->scene->name, event_id);
	if(zwscn_persistence_save_security_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_ptr->scene->id);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_MEMORY);
l_err:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_scene_not_found:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_duplicate_id_event:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_EVENT_EXISTS);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_security_scene_set_arm_state(void* net, uint32_t scene_id, ZWSCN_BOOL_T flag_arm_disarm_state) {
	zwscn_security_scene_cont_p scene_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_security_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d old scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	zwscn_security_scene_arm_state_set(net, scene_ptr, flag_arm_disarm_state, ZWSCN_STATELOG_TRIGGER_MANUAL, NULL); //passing event = NULL
	
//	if(zwscn_persistence_save_security_scene(net, scene_ptr) != ZWSCN_ERR_NONE) { //Moved to zwscn_security_scene_arm_state_set
//		goto l_err;
//	}
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_security_scene_set_alarm_state(void* net, uint32_t scene_id, ZWSCN_BOOL_T flag_alarm_state) {
	zwscn_security_scene_cont_p scene_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_security_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d old scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	zwscn_security_scene_alarm_state_set(net, scene_ptr, flag_alarm_state, ZWSCN_STATELOG_TRIGGER_MANUAL, NULL); //passing event = NULL
	
//	if(zwscn_persistence_save_security_scene(net, scene_ptr) != ZWSCN_ERR_NONE) { //Moved to zwscn_security_scene_alarm_state_set
//		goto l_err;
//	}
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_security_scene_get_statelog(void *net, uint32_t category, uint32_t* counter, uint32_t *scened) {
	ZWSCN_STATUS_T result = ZWSCN_ERR_NONE;
	
	if ((!net) || (counter == NULL) || (scened == NULL)) {
		goto l_err_args;
	}

	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	result = zwscn_statelog_security_scene_get(net, category, counter, scened); //statelog mutex is used inside this function
	
	return result;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
}

//Util Functions
ZWSCN_STATUS_T zwscn_security_scene_copy(zwscn_security_scene_p dest_scene_ptr, zwscn_security_scene_p source_scene_ptr) {
	if((dest_scene_ptr == NULL) || (source_scene_ptr == NULL)) {
		goto l_err_args;
	}
	
	*(dest_scene_ptr) = *(source_scene_ptr); //Shallow copy using C structure copying
	
	dest_scene_ptr->name[sizeof(dest_scene_ptr->name) -1] = '\0';
	dest_scene_ptr->notification_sms_number[sizeof(dest_scene_ptr->notification_sms_number) -1] = '\0';
	dest_scene_ptr->notification_email[sizeof(dest_scene_ptr->notification_email) -1] = '\0';
	
	dest_scene_ptr->lt_arm.event = NULL;
	dest_scene_ptr->lt_disarm.event = NULL;
	dest_scene_ptr->lt_alarm_on.event = NULL;
	dest_scene_ptr->lt_alarm_off.event = NULL;
	dest_scene_ptr->lt_arm.event = zwscn_security_scene_copy_event(source_scene_ptr->lt_arm.event);
	dest_scene_ptr->lt_disarm.event = zwscn_security_scene_copy_event(source_scene_ptr->lt_disarm.event);
	dest_scene_ptr->lt_alarm_on.event = zwscn_security_scene_copy_event(source_scene_ptr->lt_alarm_on.event);
	dest_scene_ptr->lt_alarm_off.event = zwscn_security_scene_copy_event(source_scene_ptr->lt_alarm_off.event);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
}

ZWSCN_STATUS_T zwscn_security_scene_free(zwscn_security_scene_p scene_ptr) {
	if(scene_ptr == NULL) {
		goto l_err_args;
	}
	
	zwscn_free_security_scene_struct(scene_ptr);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
}


static ZWSCN_STATUS_T zwscn_save_arm_event(void *net, zwscn_security_scene_cont_p scene_container_ptr, zwscn_security_event_p event) {
	zwscn_security_event_p event_ptr;
	uint8_t node_id, ep_id;
	uint16_t cls_id;
	zwscn_init_p init_data;
	int result = 0;
	
	if ((net == NULL) || (scene_container_ptr == NULL)  || (scene_container_ptr->scene == NULL) || (event == NULL)) {
		goto l_err_args;
	}
	zwscn_message_log_debug("SAVE net=%p Security Scene name=%s id=%d event=0x%x\n", net, scene_container_ptr->scene->name, scene_container_ptr->scene->id, event->id);
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	//check device args
	if((event->device_args == NULL) && (event->device_args_size > 0)) {
		zwscn_message_log_warning("zwscn_save_security_scene: event device_args_size(%u) is not zero while device_args is NULL\n", event->device_args_size);
		goto l_err_args_events;
	}
	if(event->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
		zwscn_message_log_warning("zwscn_save_security_scene: event device_args_size(%u) is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%u)\n", event->device_args_size, ZWSCN_DEVICE_ARGS_MAX_SIZE);
		goto l_err_args_events;
	}
	cls_id = zwscn_util_combo_id_to_cls_id(event->id);
	if(zwscn_zwave_is_cc_for_arm_security_event(cls_id, event->cmd) != ZWSCN_TRUE) {
		zwscn_message_log_warning("zwscn_save_security_scene: Z-Wave Command Class 0x%x is not valid for Arm events\n", cls_id);
		goto l_err_args_events;
	}
	
	//check for duplicate
#if ZWSCN_CHECK_DUPLICATE_IN_SAME_SECURITY_EVENSTS
	if(zwscn_list_get_security_event_by_id((scene_container_ptr->list_arm_events), event->id) != NULL) {
		goto l_err_duplicate_event;
	}
#endif
#if ZWSCN_CHECK_DUPLICATE_IN_OTHER_SECURITY_EVENSTS
	if(zwscn_security_scene_has_same_disarm_event(scene_container_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_event;
	}
	if(zwscn_security_scene_has_same_alarm_event(scene_container_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_event;
	}
#endif
	
	//check for device
	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	node_id = zwscn_util_combo_id_to_node_id(event->id);
	ep_id = zwscn_util_combo_id_to_ep_id(event->id);
	cls_id = zwscn_util_combo_id_to_cls_id(event->id);
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->init_input && init_data->init_input->zwscn_fn_is_valid_handler) {
		result = init_data->init_input->zwscn_fn_is_valid_handler(net, ZWSCN_CHECK_NODE_EP_CLS, node_id, ep_id, cls_id);
		if(!result) {
			zwscn_message_log_warning("zwscn_fn_is_valid_handler returned %d for net=%p node=%d ep=%d cls=%d\n", result, net, node_id, ep_id, cls_id);
			goto l_err_device_invalid;
		}
	} else {
		zwscn_message_log_warning("Function zwscn_fn_is_valid_handler is NULL for net=%p\n", net);
		goto l_err_callback_null;
	}
	zwscn_network_unlock_reader(net);
	
	event_ptr = (zwscn_security_event_p) calloc(1, sizeof(zwscn_security_event_t));
	if(event_ptr == NULL) {
		goto l_err_memory;
	}
	*(event_ptr) = *(event); // C compiler in-built structure copy
	event_ptr->device_args = NULL;
	if(event->device_args && (event->device_args_size != 0)) {
		event_ptr->device_args = calloc(1, event->device_args_size);
		if(!event_ptr->device_args) {
			goto l_err_memory_1;
		}
		memcpy(event_ptr->device_args, event->device_args, event->device_args_size);
	}
	
	if((scene_container_ptr->list_arm_events = zwscn_list_add(scene_container_ptr->list_arm_events, event_ptr)) == NULL) {
		goto l_err_event_add;
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_args_events:
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_duplicate_event:
	RETURN(ZWSCN_ERR_EVENT_EXISTS);
l_err_device_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_DEVICE);
l_err_callback_null:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_CALLBACK_NULL);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_memory_1:
	zwscn_free_security_event(event_ptr);
	RETURN(ZWSCN_ERR_MEMORY);
l_err_event_add:
	zwscn_free_security_event(event_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);	
}

static ZWSCN_STATUS_T zwscn_save_disarm_event(void *net, zwscn_security_scene_cont_p scene_container_ptr, zwscn_security_event_p event) {
	zwscn_security_event_p event_ptr;
	uint8_t node_id, ep_id;
	uint16_t cls_id;
	zwscn_init_p init_data;
	int result = 0;
	
	if ((net == NULL) || (scene_container_ptr == NULL)  || (scene_container_ptr->scene == NULL) || (event == NULL)) {
		goto l_err_args;
	}
	zwscn_message_log_debug("SAVE net=%p Security Scene name=%s id=%d event=0x%x\n", net, scene_container_ptr->scene->name, scene_container_ptr->scene->id, event->id);
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	//check device args
	if((event->device_args == NULL) && (event->device_args_size > 0)) {
		zwscn_message_log_warning("zwscn_save_security_scene: event device_args_size(%u) is not zero while device_args is NULL\n", event->device_args_size);
		goto l_err_args_events;
	}
	if(event->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
		zwscn_message_log_warning("zwscn_save_security_scene: event device_args_size(%u) is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%u)\n", event->device_args_size, ZWSCN_DEVICE_ARGS_MAX_SIZE);
		goto l_err_args_events;
	}
	cls_id = zwscn_util_combo_id_to_cls_id(event->id);
	if(zwscn_zwave_is_cc_for_arm_security_event(cls_id, event->cmd) != ZWSCN_TRUE) {
		zwscn_message_log_warning("zwscn_save_security_scene: Z-Wave Command Class 0x%x is not valid for Disarm events\n", cls_id);
		goto l_err_args_events;
	}
	
	//check for duplicate
#if ZWSCN_CHECK_DUPLICATE_IN_SAME_SECURITY_EVENSTS
	if(zwscn_list_get_security_event_by_id((scene_container_ptr->list_disarm_events), event->id) != NULL) {
		goto l_err_duplicate_event;
	}
#endif
#if ZWSCN_CHECK_DUPLICATE_IN_OTHER_SECURITY_EVENSTS
	if(zwscn_security_scene_has_same_arm_event(scene_container_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_event;
	}
	if(zwscn_security_scene_has_same_alarm_event(scene_container_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_event;
	}
#endif
	
	//check for device
	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	node_id = zwscn_util_combo_id_to_node_id(event->id);
	ep_id = zwscn_util_combo_id_to_ep_id(event->id);
	cls_id = zwscn_util_combo_id_to_cls_id(event->id);
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->init_input && init_data->init_input->zwscn_fn_is_valid_handler) {
		result = init_data->init_input->zwscn_fn_is_valid_handler(net, ZWSCN_CHECK_NODE_EP_CLS, node_id, ep_id, cls_id);
		if(!result) {
			zwscn_message_log_warning("zwscn_fn_is_valid_handler returned %d for net=%p node=%d ep=%d cls=%d\n", result, net, node_id, ep_id, cls_id);
			goto l_err_device_invalid;
		}
	} else {
		zwscn_message_log_warning("Function zwscn_fn_is_valid_handler is NULL for net=%p\n", net);
		goto l_err_callback_null;
	}
	zwscn_network_unlock_reader(net);
	
	event_ptr = (zwscn_security_event_p) calloc(1, sizeof(zwscn_security_event_t));
	if(event_ptr == NULL) {
		goto l_err_memory;
	}
	*(event_ptr) = *(event); // C compiler in-built structure copy
	event_ptr->device_args = NULL;
	if(event->device_args && (event->device_args_size != 0)) {
		event_ptr->device_args = calloc(1, event->device_args_size);
		if(!event_ptr->device_args) {
			goto l_err_memory_1;
		}
		memcpy(event_ptr->device_args, event->device_args, event->device_args_size);
	}
	
	if((scene_container_ptr->list_disarm_events = zwscn_list_add(scene_container_ptr->list_disarm_events, event_ptr)) == NULL) {
		goto l_err_event_add;
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_args_events:
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_duplicate_event:
	RETURN(ZWSCN_ERR_EVENT_EXISTS);
l_err_device_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_DEVICE);
l_err_callback_null:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_CALLBACK_NULL);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_memory_1:
	zwscn_free_security_event(event_ptr);
	RETURN(ZWSCN_ERR_MEMORY);
l_err_event_add:
	zwscn_free_security_event(event_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);	
}

static ZWSCN_STATUS_T zwscn_save_alarm_event(void *net, zwscn_security_scene_cont_p scene_container_ptr, zwscn_security_event_p event) {
	zwscn_security_event_p event_ptr;
	uint8_t node_id, ep_id;
	uint16_t cls_id;
	zwscn_init_p init_data;
	int result = 0;
	
	if ((net == NULL) || (scene_container_ptr == NULL)  || (scene_container_ptr->scene == NULL) || (event == NULL)) {
		goto l_err_args;
	}
	zwscn_message_log_debug("SAVE net=%p Security Scene name=%s id=%d event=0x%x\n", net, scene_container_ptr->scene->name, scene_container_ptr->scene->id, event->id);
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	//check device args
	if((event->device_args == NULL) && (event->device_args_size > 0)) {
		zwscn_message_log_warning("zwscn_save_security_scene: event device_args_size(%u) is not zero while device_args is NULL\n", event->device_args_size);
		goto l_err_args_events;
	}
	if(event->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
		zwscn_message_log_warning("zwscn_save_security_scene: event device_args_size(%u) is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%u)\n", event->device_args_size, ZWSCN_DEVICE_ARGS_MAX_SIZE);
		goto l_err_args_events;
	}
	cls_id = zwscn_util_combo_id_to_cls_id(event->id);
	if(zwscn_zwave_is_cc_for_alarm_security_event(cls_id, event->cmd) != ZWSCN_TRUE) {
		zwscn_message_log_warning("zwscn_save_security_scene: Z-Wave Command Class 0x%x is not valid for Alalarm events\n", cls_id);
		goto l_err_args_events;
	}
	
	//check for duplicate
#if ZWSCN_CHECK_DUPLICATE_IN_SAME_SECURITY_EVENSTS
	if(zwscn_security_scene_has_same_alarm_event(scene_container_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_event;
	}
#endif
#if ZWSCN_CHECK_DUPLICATE_IN_OTHER_SECURITY_EVENSTS
	if(zwscn_security_scene_has_same_arm_event(scene_container_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_event;
	}
	if(zwscn_security_scene_has_same_disarm_event(scene_container_ptr, event) == ZWSCN_TRUE) {
		goto l_err_duplicate_event;
	}
#endif
	
	//check for device
	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	node_id = zwscn_util_combo_id_to_node_id(event->id);
	ep_id = zwscn_util_combo_id_to_ep_id(event->id);
	cls_id = zwscn_util_combo_id_to_cls_id(event->id);
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->init_input && init_data->init_input->zwscn_fn_is_valid_handler) {
		result = init_data->init_input->zwscn_fn_is_valid_handler(net, ZWSCN_CHECK_NODE_EP_CLS, node_id, ep_id, cls_id);
		if(!result) {
			zwscn_message_log_warning("zwscn_fn_is_valid_handler returned %d for net=%p node=%d ep=%d cls=%d\n", result, net, node_id, ep_id, cls_id);
			goto l_err_device_invalid;
		}
	} else {
		zwscn_message_log_warning("Function zwscn_fn_is_valid_handler is NULL for net=%p\n", net);
		goto l_err_callback_null;
	}
	zwscn_network_unlock_reader(net);
	
	event_ptr = (zwscn_security_event_p) calloc(1, sizeof(zwscn_security_event_t));
	if(event_ptr == NULL) {
		goto l_err_memory;
	}
	*(event_ptr) = *(event); // C compiler in-built structure copy
	event_ptr->device_args = NULL;
	if(event->device_args && (event->device_args_size != 0)) {
		event_ptr->device_args = calloc(1, event->device_args_size);
		if(!event_ptr->device_args) {
			goto l_err_memory_1;
		}
		memcpy(event_ptr->device_args, event->device_args, event->device_args_size);
	}
	
	if((scene_container_ptr->list_alarm_events = zwscn_list_add(scene_container_ptr->list_alarm_events, event_ptr)) == NULL) {
		goto l_err_event_add;
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_args_events:
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_duplicate_event:
	RETURN(ZWSCN_ERR_EVENT_EXISTS);
l_err_device_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_DEVICE);
l_err_callback_null:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_CALLBACK_NULL);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_memory_1:
	zwscn_free_security_event(event_ptr);
	RETURN(ZWSCN_ERR_MEMORY);
l_err_event_add:
	zwscn_free_security_event(event_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);	
}

ZWSCN_STATUS_T zwscn_save_security_scene(void *net, zwscn_security_scene_p scene, 
								zwscn_security_event_t *array_arm_events, int num_arm_events,
								zwscn_security_event_t *array_disarm_events, int num_disarm_events, 
								zwscn_security_event_t *array_alarm_events, int num_alarm_events, uint32_t *out_id) {
	zwscn_list_t *scene_list;
	zwscn_security_scene_cont_p scene_container_ptr;
	zwscn_security_scene_cont_p scene_ptr;
	zwscn_security_scene_p scene_copy = NULL;
	zwscn_security_event_p event_temp;
	uint32_t scene_id=0;
	int max_arm_events=0;
	int max_disarm_events=0;
	int max_alarm_events=0;
	int index=0;
	int result=0;
	int i=0;
	int bNewScene = 0;
								
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if ((net == NULL) || (scene == NULL) || 
		((num_arm_events > 0) && (array_arm_events == NULL)) || 
		((num_disarm_events > 0) && (array_disarm_events == NULL)) || 
		((num_alarm_events > 0) && (array_alarm_events == NULL)) || 
		(out_id == NULL)) {
		goto l_err_args;
	}
	
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene id=%d name=%s\n", net, scene->id, scene->name);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	if((scene->is_notification_by_sms_on) && (zwscn_util_sms_number_is_valid(scene->notification_sms_number, ZWSCN_TRUE) != ZWSCN_TRUE)) {
		ZWSCN_MESSAGE_LOG_DEBUG("SMS NUMBER: %s is invalid\n", scene->notification_sms_number);
		goto l_err_args;
	}
	if((scene->is_notification_by_email_on) && (zwscn_util_email_is_valid(scene->notification_email, ZWSCN_TRUE) != ZWSCN_TRUE)) {
		ZWSCN_MESSAGE_LOG_DEBUG("Email: %s is invalid\n", scene->notification_email);
		goto l_err_args;
	}
	
	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid_1;
	}
	max_arm_events = zwscn_get_max_arm_events(net);
	max_disarm_events = zwscn_get_max_disarm_events(net);
	max_alarm_events = zwscn_get_max_alarm_events(net);
	if(num_arm_events > max_arm_events) {
		zwscn_message_log_warning("Input Security Scene has more Arm events (%d) than allowed (%d)\n", num_arm_events, max_arm_events);
		goto l_err_num_arm_events;
	}
	if(num_disarm_events > max_disarm_events) {
		zwscn_message_log_warning("Input Security Scene has more Disarm events (%d) than allowed (%d)\n", num_disarm_events, max_disarm_events);
		goto l_err_num_disarm_events;
	}
	if(num_alarm_events > max_alarm_events) {
		zwscn_message_log_warning("Input Security Scene has more Alarm events (%d) than allowed (%d)\n", num_alarm_events, max_alarm_events);
		goto l_err_num_alarm_events;
	}
	zwscn_network_unlock_reader(net);
	
	//scene metadata
	scene_container_ptr = (zwscn_security_scene_cont_p) calloc(1, sizeof(zwscn_security_scene_cont_t));
	if(scene_container_ptr == NULL) {
		goto l_err_memory;
	}
	scene_container_ptr->scene = (zwscn_security_scene_p) calloc(1, sizeof(zwscn_security_scene_t));
	if(scene_container_ptr->scene == NULL) {
		goto l_err_memory_1;
	}
	if(zwscn_security_scene_copy(scene_container_ptr->scene, scene) != ZWSCN_ERR_NONE) {
		goto l_err_scene_copy;
	}
	
	//Add Arm Events
	for(i=0; i<num_arm_events; ++i) {
		event_temp = &(array_arm_events[i]);
		if(event_temp == NULL) {
			goto l_err_args_arm_events;
		}
		result = zwscn_save_arm_event(net, scene_container_ptr, event_temp);
		if(result != ZWSCN_ERR_NONE) {
			zwscn_message_log_warning("Error (%d) saving Arm event=0x%x for net=%p scene=%d\n", result, event_temp->id, net, scene_container_ptr->scene->id);
			goto l_err_arm_events;
		}
	}
	
	//Add Disarm Events
	for(i=0; i<num_disarm_events; ++i) {
		event_temp = &(array_disarm_events[i]);
		if(event_temp == NULL) {
			goto l_err_args_disarm_events;
		}
		result = zwscn_save_disarm_event(net, scene_container_ptr, event_temp);
		if(result != ZWSCN_ERR_NONE) {
			zwscn_message_log_warning("Error (%d) saving Disarm event=0x%x for net=%p scene=%d\n", result, event_temp->id, net, scene_container_ptr->scene->id);
			goto l_err_disarm_events;
		}
	}
	
	//Add Alarm Events
	for(i=0; i<num_alarm_events; ++i) {
		event_temp = &(array_alarm_events[i]);
		if(event_temp == NULL) {
			goto l_err_args_alarm_events;
		}
		result = zwscn_save_alarm_event(net, scene_container_ptr, event_temp);
		if(result != ZWSCN_ERR_NONE) {
			zwscn_message_log_warning("Error (%d) saving Alarm event=0x%x for net=%p scene=%d\n", result, event_temp->id, net, scene_container_ptr->scene->id);
			goto l_err_alarm_events;
		}
	}
	
	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid_2;
	}
	if(zwscn_find_security_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	scene_id = scene->id;
	if(scene_id == 0) { //new scene
		bNewScene = 1;
		if(zwscn_is_list_full_security_scene(net, (*scene_list))) {
			goto l_err_list_full;
		}
		if((scene_id = zwscn_generate_security_scene_id(net)) == 0) {
			goto l_err;
		}
		scene_container_ptr->scene->id = scene_id;
		if(((*scene_list) = zwscn_list_add((*scene_list), scene_container_ptr)) == NULL) {
			goto l_err;
		}
	} else { //Edit existing scene - in this API we'll just replace the old scene
		bNewScene = 0;
		if(zwscn_find_security_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
			goto l_err;
		}
		index = zwscn_list_get_index((*scene_list), scene_ptr);
		if(index == -1) {
			goto l_err;
		}
		scene_container_ptr->scene->id = scene_id;
		scene_container_ptr->scene->is_armed = scene_ptr->scene->is_armed;
		scene_container_ptr->scene->is_alarmed = scene_ptr->scene->is_alarmed;
		scene_container_ptr->scene->lt_arm = scene_ptr->scene->lt_arm;
		scene_container_ptr->scene->lt_disarm = scene_ptr->scene->lt_disarm;
		scene_container_ptr->scene->lt_alarm_on = scene_ptr->scene->lt_alarm_on;
		scene_container_ptr->scene->lt_alarm_off = scene_ptr->scene->lt_alarm_off;
		scene_container_ptr->scene->lt_arm.event = zwscn_security_scene_copy_event(scene_ptr->scene->lt_arm.event);
		scene_container_ptr->scene->lt_disarm.event = zwscn_security_scene_copy_event(scene_ptr->scene->lt_disarm.event);
		scene_container_ptr->scene->lt_alarm_on.event = zwscn_security_scene_copy_event(scene_ptr->scene->lt_alarm_on.event);
		scene_container_ptr->scene->lt_alarm_off.event = zwscn_security_scene_copy_event(scene_ptr->scene->lt_alarm_off.event);
		if(((*scene_list) = zwscn_list_insert((*scene_list), scene_container_ptr, index)) == NULL) {
			goto l_err;
		}
		(*scene_list) = zwscn_list_remove((*scene_list), scene_ptr);
		zwscn_free_security_scene(scene_ptr);
	}
	if(zwscn_persistence_save_security_scene(net, scene_container_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_persistence;
	}
	zwscn_security_scene_update_timestamp(scene_container_ptr);
	//Send Security Scene Add notification
	scene_copy = calloc(1, sizeof(zwscn_security_scene_t));
	if(scene_copy == NULL) {
		zwscn_message_log_critical("Could not allocate memory needed for security scene notification request\n");
	} else {
		*(scene_copy) = *(scene_container_ptr->scene); //C structure copy
		scene_copy->lt_arm.event = NULL;
		scene_copy->lt_disarm.event = NULL;
		scene_copy->lt_alarm_on.event = NULL;
		scene_copy->lt_alarm_off.event = NULL;
		zwscn_security_scene_notification_request_message_send(net, scene_copy, ZWSCN_MESSAGE_TYPE_SECURITY_SCENE_NOTIFICATION_ADD);
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_name=%s returned id=%u\n", net, scene->name, scene_id);
	(*out_id) = scene_id;

	if(bNewScene == 1)
		zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_ADD, scene_id);
	else 
		zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_id);

	zwscn_network_unlock_writer(net);
	
	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_num_arm_events:
l_err_num_disarm_events:
l_err_num_alarm_events:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_LIST_FULL);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_memory_1:
	free(scene_container_ptr);
	RETURN(ZWSCN_ERR_MEMORY);
l_err_scene_copy:
	zwscn_free_security_scene(scene_container_ptr);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_args_arm_events:
l_err_args_disarm_events:
l_err_args_alarm_events:
	zwscn_free_security_scene(scene_container_ptr);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_arm_events:
l_err_disarm_events:
l_err_alarm_events:
	zwscn_free_security_scene(scene_container_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_list_full:
	zwscn_network_unlock_writer(net);
	zwscn_free_security_scene(scene_container_ptr);
	RETURN(ZWSCN_ERR_LIST_FULL);
l_err:
	zwscn_network_unlock_writer(net);
	zwscn_free_security_scene(scene_container_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_persistence:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_PERSISTENT_DATA);
l_err_network_invalid_1:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
l_err_network_invalid_2:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}