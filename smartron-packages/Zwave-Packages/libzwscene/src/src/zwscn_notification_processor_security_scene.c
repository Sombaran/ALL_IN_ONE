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
@file		zwscn_notification_processor_security_scene.c  
  
                To provide support for processing network and node notifications in Security Scenes
  
@author		Arif Mohammand
 
@version	1.0 2015-04-21  Initial release
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "zwscn_defines.h"
#include "zwscn_scene_api.h"
#include "zwscn_security_scene_api.h"
#include "zwscn_util.h"
#include "zwscn_util_hash.h"
#include "zwscn_util_list.h"
#include "zwscn_util_message_log.h"
#include "zwscn_util_mem.h"
#include "zwscn_util_mqueue.h"
#include "zwscn_util_mqueue_message.h"
#include "zwscn_util_thread.h"
#include "zwscn_util_mutex.h"
#include "zwscn_util_rwlock.h"
#include "zwscn_util_timer.h"
#include "zwscn_statelog.h"
#include "zwscn_scene.h"
#include "zwscn_security_scene.h"
#include "zwscn_notification_handler.h"
#include "zwscn_notification_processor.h"
#include "zwscn_device_report_cache.h"
#include "zwscn_notification_processor_security_scene.h"
#include "zwscn_persistence_security_scene.h"

ZWSCN_STATUS_T zwscn_remove_security_scenes_by_node_id(void *net, uint8_t node_id) {
	zwscn_list_t *scene_list_ptr;
	zwscn_list_t scene_list;
	zwscn_list_foreach_data_t list_foreach_data;
	
	zwscn_message_log_debug("ENTER zwscn_remove_security_scene_by_node_id net=%p node_id=%d\n", net, node_id);
	if ((net == NULL) || (node_id == 0)) {
		zwscn_message_log_critical("zwscn_remove_security_scene_by_node_id input args NULL\n");
		goto l_err_args;
	}
	
	if(zwscn_find_security_scene_list(net, &scene_list_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	list_foreach_data.net = net;
	list_foreach_data.node_id = node_id;
	list_foreach_data.list_ptr = scene_list_ptr;
	scene_list = (*scene_list_ptr);
	zwscn_list_foreach(scene_list, zwscn_remove_security_scenes_by_node_id_list_callback, &list_foreach_data);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;
}

void zwscn_remove_security_scenes_by_node_id_list_callback(void *list_item, void *user_data) {
	void *net;
	zwscn_list_t *scene_list_ptr;
	zwscn_security_scene_cont_p scene_ptr;
	uint8_t node_id;
	uint32_t scene_id;
	int initial_count_alarm_security_events;
	int count_alarm_security_events;
	zwscn_list_foreach_data_t *list_foreach_data_ptr;
	ZWSCN_STATUS_T result;
	
	zwscn_message_log_debug("ENTER zwscn_remove_security_scenes_by_node_id_list_callback\n");
	if((list_item == NULL) || (user_data == NULL)) {
		zwscn_message_log_critical("zwscn_remove_security_scenes_by_node_id_list_callback input args NULL\n");
		goto l_err;
	}
	
	scene_ptr = (zwscn_security_scene_cont_p) list_item;
	list_foreach_data_ptr = (zwscn_list_foreach_data_t *) user_data;
	net = list_foreach_data_ptr->net;
	node_id = list_foreach_data_ptr->node_id;
	scene_list_ptr = list_foreach_data_ptr->list_ptr;
	
	initial_count_alarm_security_events = zwscn_list_size(scene_ptr->list_alarm_events);
	//Check Arm, Disarm and Alarm events
	result = zwscn_remove_security_events_by_node_id(net, node_id, scene_ptr, &(scene_ptr->list_arm_events));
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("Error (%d) returned by zwscn_remove_security_events_by_node_id for scene id=%d for Arm events\n", result, scene_ptr->scene->id);
//		goto l_err;
	}
	result = zwscn_remove_security_events_by_node_id(net, node_id, scene_ptr, &(scene_ptr->list_disarm_events));
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("Error (%d) returned by zwscn_remove_security_events_by_node_id for scene id=%d for Disarm events\n", result, scene_ptr->scene->id);
//		goto l_err;
	}
	result = zwscn_remove_security_events_by_node_id(net, node_id, scene_ptr, &(scene_ptr->list_alarm_events));
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("Error (%d) returned by zwscn_remove_security_events_by_node_id for scene id=%d for Alarm events\n", result, scene_ptr->scene->id);
//		goto l_err;
	}
	
	count_alarm_security_events = zwscn_list_size(scene_ptr->list_alarm_events);
	zwscn_message_log_debug("Security Scene id=%d has %d alarm events left\n", scene_ptr->scene->id, count_alarm_security_events);
	if((initial_count_alarm_security_events == 0) || (count_alarm_security_events > 0)) { //If the Alarm event count was zero initially for some reason then we don't delete this Scene
		zwscn_message_log_debug("Saving Security Scene...\n");
		zwscn_persistence_save_security_scene(net, scene_ptr);
	} else {
		scene_id = scene_ptr->scene->id;
		zwscn_message_log_debug("Removing Security Scene...\n");
		zwscn_persistence_delete_security_scene(net, scene_id);
		(*scene_list_ptr) = zwscn_list_remove((*scene_list_ptr), scene_ptr);
		zwscn_free_security_scene(scene_ptr);
		zwscn_message_log_debug("Security Scene Removed\n");
		zwscn_security_scene_update_timestamp(scene_ptr);
		zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_DELETE, scene_id);
	}
	
	return;
l_err:
	return;
}

ZWSCN_STATUS_T zwscn_remove_security_events_by_node_id(void *net, uint8_t node_id, zwscn_security_scene_cont_p scene_ptr, zwscn_list_t *event_list) {
	zwscn_list_foreach_data_t list_foreach_data;
	uint32_t scene_id;
	
	zwscn_message_log_debug("ENTER zwscn_remove_events_by_node_id net=%p node_id=%d\n", net, node_id);
	if ((net == NULL) || (node_id == 0) || (scene_ptr == NULL)) {
		zwscn_message_log_critical("zwscn_remove_events_by_node_id input args NULL\n");
		goto l_err_args;
	}
	
	scene_id = scene_ptr->scene->id;
	list_foreach_data.net = net;
	list_foreach_data.node_id = node_id;
	list_foreach_data.list_ptr = event_list;
	list_foreach_data.scene_id = scene_id;
	
	zwscn_list_foreach((*event_list), zwscn_remove_security_events_by_node_id_list_callback, &list_foreach_data);
//	zwscn_security_scene_update_timestamp(scene_ptr);
//	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_id);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
}

void zwscn_remove_security_events_by_node_id_list_callback(void *list_item, void *user_data) {
	void *net;
	zwscn_list_t *event_list_ptr;
	zwscn_event_p event_ptr;
	uint8_t event_node_id;
	uint8_t node_id;
	uint32_t scene_id;
	zwscn_list_foreach_data_t *list_foreach_data_ptr;
	zwscn_security_scene_cont_t *scene_ptr = NULL;
	
	zwscn_message_log_debug("ENTER zwscn_remove_security_events_by_node_id_list_callback\n");
	if((list_item == NULL) || (user_data == NULL)) {
		zwscn_message_log_critical("zwscn_remove_security_events_by_node_id_list_callback input args NULL\n");
		goto l_err;
	}
	
	event_ptr = (zwscn_event_p) list_item;
	list_foreach_data_ptr = (zwscn_list_foreach_data_t *) user_data;
	net = list_foreach_data_ptr->net;
	node_id = list_foreach_data_ptr->node_id;
	scene_id = list_foreach_data_ptr->scene_id;
	event_list_ptr = list_foreach_data_ptr->list_ptr;
	if(event_list_ptr == NULL) {
		zwscn_message_log_critical("zwscn_remove_security_events_by_node_id_list_callback event_list_ptr NULL\n");
		goto l_err;
	}
	
	event_node_id = zwscn_util_combo_id_to_node_id(event_ptr->id);
	zwscn_message_log_debug("checking security event id=%d event_node_id=%d node_id=%d\n", event_ptr->id, event_node_id, node_id);
	if(event_node_id == node_id) {
		(*event_list_ptr) = zwscn_list_remove((*event_list_ptr), event_ptr);
		zwscn_free_event(event_ptr);
		zwscn_message_log_debug("Security Event Removed\n");
		if((zwscn_find_security_scene(net, scene_id, &scene_ptr) == ZWSCN_ERR_NONE) && (scene_ptr != NULL)) {
			zwscn_security_scene_update_timestamp(scene_ptr);
		}
		zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_id);
	}
	
l_err:
	return;
}

//Function for Node Update, Network Update and Replace Failed node
ZWSCN_STATUS_T zwscn_validate_security_scenes_on_notification(void *net, uint8_t node_id) {
//	zwscn_list_t *scene_list_ptr;
	zwscn_list_t scene_list;
	zwscn_list_foreach_data_t list_foreach_data;
	zwscn_init_p init_data;
	
	zwscn_message_log_debug("ENTER zwscn_validate_security_scenes_on_notification net=%p node_id=%d\n", net, node_id);
	if (net == NULL) { //if node_id == 0 then it is a network level notification
		zwscn_message_log_critical("zwscn_validate_security_scenes_on_notification input args NULL\n");
		goto l_err_args;
	}
	
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		goto l_err;
	}
	
	list_foreach_data.net = net;
	list_foreach_data.node_id = node_id;
	
	list_foreach_data.list_ptr = &(init_data->list_security_scenes);
	list_foreach_data.list_ptr_dest = &(init_data->list_security_scenes_invalid);
	scene_list = init_data->list_security_scenes;
	zwscn_list_foreach(scene_list, zwscn_validate_security_scenes_from_valid_list_callback, &list_foreach_data);
	
	list_foreach_data.list_ptr = &(init_data->list_security_scenes_invalid);
	list_foreach_data.list_ptr_dest = &(init_data->list_security_scenes);
	scene_list = init_data->list_security_scenes_invalid;
	zwscn_list_foreach(scene_list, zwscn_validate_security_scenes_from_invalid_list_callback, &list_foreach_data);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;
}

void zwscn_validate_security_scenes_from_valid_list_callback(void *list_item, void *user_data) {
	void *net;
	zwscn_list_t *scene_list_ptr;
	zwscn_list_t *invalid_security_scene_list_ptr;
	zwscn_security_scene_cont_p scene_ptr;
	uint8_t node_id;
	uint32_t scene_id;
	int initial_count_alarm_events;
	int count_alarm_events;
	zwscn_list_foreach_data_t *list_foreach_data_ptr;
	ZWSCN_STATUS_T result;
	
	zwscn_message_log_debug("ENTER zwscn_validate_security_scenes_from_valid_list_callback\n");
	if((list_item == NULL) || (user_data == NULL)) {
		zwscn_message_log_critical("zwscn_validate_security_scenes_from_valid_list_callback input args NULL\n");
		goto l_err;
	}
	
	scene_ptr = (zwscn_security_scene_cont_p) list_item;
	list_foreach_data_ptr = (zwscn_list_foreach_data_t *) user_data;
	net = list_foreach_data_ptr->net;
	node_id = list_foreach_data_ptr->node_id;
	scene_list_ptr = list_foreach_data_ptr->list_ptr;
	invalid_security_scene_list_ptr = list_foreach_data_ptr->list_ptr_dest;
	
	initial_count_alarm_events = zwscn_list_size(scene_ptr->list_alarm_events);
	//Validate Arm, Disarm and Alarm events	
	result = zwscn_validate_security_events(net, node_id, scene_ptr, &(scene_ptr->list_arm_events), &(scene_ptr->list_arm_events_invalid), ZWSCN_TYPE_ARM_SECURITY_EVENT);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("Error (%d) returned by zwscn_validate_security_events for scene id=%d for Arm events\n", result, scene_ptr->scene->id);
//		goto l_err;
	}
	result = zwscn_validate_security_events(net, node_id, scene_ptr, &(scene_ptr->list_disarm_events), &(scene_ptr->list_disarm_events_invalid), ZWSCN_TYPE_DISARM_SECURITY_EVENT);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("Error (%d) returned by zwscn_validate_security_events for scene id=%d for Disrm events\n", result, scene_ptr->scene->id);
//		goto l_err;
	}
	result = zwscn_validate_security_events(net, node_id, scene_ptr, &(scene_ptr->list_alarm_events), &(scene_ptr->list_alarm_events_invalid), ZWSCN_TYPE_ALARM_SECURITY_EVENT);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("Error (%d) returned by zwscn_validate_security_events for scene id=%d for Alarm events\n", result, scene_ptr->scene->id);
//		goto l_err;
	}
	
	scene_id = scene_ptr->scene->id;
	count_alarm_events = zwscn_list_size(scene_ptr->list_alarm_events);
	zwscn_message_log_debug("Scene id=%d has %d valid alarm events left\n", scene_ptr->scene->id, count_alarm_events);
	if((initial_count_alarm_events == 0) || (count_alarm_events > 0)) { //If the Alarm events count was zero initially for some reason then we don't move this Scene
		//do nothing
	} else {
		zwscn_message_log_debug("Moving Security Scene %d to invalid list...\n", scene_id);
		(*scene_list_ptr) = zwscn_list_remove((*scene_list_ptr), scene_ptr);
		(*invalid_security_scene_list_ptr) = zwscn_list_add((*invalid_security_scene_list_ptr), scene_ptr);
		zwscn_message_log_debug("Security Scene moved to invalid list\n");
		zwscn_security_scene_update_timestamp(scene_ptr);
		zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_DELETE, scene_id);
	}
	
	return;
l_err:
	return;
//l_err_status:
//	return;
}

void zwscn_validate_security_scenes_from_invalid_list_callback(void *list_item, void *user_data) {
	void *net;
	zwscn_list_t *scene_list_ptr;
	zwscn_list_t *invalid_security_scene_list_ptr;
	zwscn_security_scene_cont_p scene_ptr;
	uint8_t node_id;
	uint32_t scene_id;
//	int initial_count_alarm_events;
	int count_alarm_events;
	zwscn_list_foreach_data_t *list_foreach_data_ptr;
	ZWSCN_STATUS_T result;
	
	zwscn_message_log_debug("ENTER zwscn_validate_security_scenes_from_invalid_list_callback\n");
	if((list_item == NULL) || (user_data == NULL)) {
		zwscn_message_log_critical("zwscn_validate_security_scenes_from_invalid_list_callback input args NULL\n");
		goto l_err;
	}
	
	scene_ptr = (zwscn_security_scene_cont_p) list_item;
	list_foreach_data_ptr = (zwscn_list_foreach_data_t *) user_data;
	net = list_foreach_data_ptr->net;
	node_id = list_foreach_data_ptr->node_id;
	scene_list_ptr = list_foreach_data_ptr->list_ptr_dest;
	invalid_security_scene_list_ptr = list_foreach_data_ptr->list_ptr;
	
//	initial_count_alarm_events = zwscn_list_size(scene_ptr->list_alarm_events);
	//Validate Arm, Disarm and Alarm events	
	result = zwscn_validate_security_events(net, node_id, scene_ptr, &(scene_ptr->list_arm_events), &(scene_ptr->list_arm_events_invalid), ZWSCN_TYPE_ARM_SECURITY_EVENT);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("Error (%d) returned by zwscn_validate_security_events for scene id=%d for Arm events\n", result, scene_ptr->scene->id);
//		goto l_err;
	}
	result = zwscn_validate_security_events(net, node_id, scene_ptr, &(scene_ptr->list_disarm_events), &(scene_ptr->list_disarm_events_invalid), ZWSCN_TYPE_DISARM_SECURITY_EVENT);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("Error (%d) returned by zwscn_validate_security_events for scene id=%d for Disrm events\n", result, scene_ptr->scene->id);
//		goto l_err;
	}
	result = zwscn_validate_security_events(net, node_id, scene_ptr, &(scene_ptr->list_alarm_events), &(scene_ptr->list_alarm_events_invalid), ZWSCN_TYPE_ALARM_SECURITY_EVENT);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("Error (%d) returned by zwscn_validate_security_events for scene id=%d for Alarm events\n", result, scene_ptr->scene->id);
//		goto l_err;
	}
	
	count_alarm_events = zwscn_list_size(scene_ptr->list_alarm_events);
	zwscn_message_log_debug("Scene id=%d has %d valid Alarm events left\n", scene_ptr->scene->id, count_alarm_events);
	if(count_alarm_events > 0) {
		scene_id = scene_ptr->scene->id;
		if(zwscn_is_list_full_security_scene(net, (*scene_list_ptr))) {
			zwscn_message_log_warning("net=%p has its Security Scene list full. Can't move valid Security Scenes from invalid list to valid list\n", net);
			goto l_err_list_full;
		}
		zwscn_message_log_debug("Moving Security Scene %d to valid list...\n", scene_id);
		(*invalid_security_scene_list_ptr) = zwscn_list_remove((*invalid_security_scene_list_ptr), scene_ptr);
		(*scene_list_ptr) = zwscn_list_add((*scene_list_ptr), scene_ptr);
		zwscn_message_log_debug("Security Scene moved to valid list\n");
		zwscn_security_scene_update_timestamp(scene_ptr);
		zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_ADD, scene_id);
	}
	
	return;
l_err:
l_err_list_full:
	return;
//l_err_status:
//	return;
}

ZWSCN_STATUS_T zwscn_validate_security_events(void *net, uint8_t node_id, zwscn_security_scene_cont_p scene_ptr, zwscn_list_t *list_valid, zwscn_list_t *list_invalid, uint8_t security_event_type) {
	zwscn_list_t event_list;
	zwscn_list_foreach_data_t list_foreach_data;
//	uint32_t scene_id;
	zwscn_init_p init_data;
	
	zwscn_message_log_debug("ENTER zwscn_validate_security_events net=%p node_id=%d\n", net, node_id);
	if ((net == NULL) || (scene_ptr == NULL)) {
		zwscn_message_log_critical("zwscn_validate_security_events input args NULL\n");
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
	list_foreach_data.type = security_event_type;
	
	list_foreach_data.list_ptr = list_valid;
	list_foreach_data.list_ptr_dest = list_invalid;
	event_list = (*list_valid);
	zwscn_list_foreach(event_list, zwscn_validate_security_events_from_valid_list_callback, &list_foreach_data);

	list_foreach_data.list_ptr = list_invalid;
	list_foreach_data.list_ptr_dest = list_valid;
	event_list = (*list_invalid);
	zwscn_list_foreach(event_list, zwscn_validate_security_events_from_invalid_list_callback, &list_foreach_data);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;
}

void zwscn_validate_security_events_from_valid_list_callback(void *list_item, void *user_data) {
	void *net;
	zwscn_list_t *event_list_ptr;
	zwscn_list_t *invalid_event_list_ptr;
	zwscn_security_event_p event_ptr;
	zwscn_security_scene_cont_t *scene_ptr = NULL;
	uint8_t event_node_id;
	uint8_t event_ep_id;
	uint16_t event_cls_id;
	uint32_t scene_id;
	uint8_t node_id;
	zwscn_list_foreach_data_t *list_foreach_data_ptr;
	zwscn_fn_is_valid_t	func_is_valid;
	int result;
	
	zwscn_message_log_debug("ENTER zwscn_validate_security_events_from_valid_list_callback\n");
	if((list_item == NULL) || (user_data == NULL)) {
		zwscn_message_log_critical("zwscn_validate_security_events_from_valid_list_callback input args NULL\n");
		goto l_err;
	}
	
	event_ptr = (zwscn_security_event_p) list_item;
	list_foreach_data_ptr = (zwscn_list_foreach_data_t *) user_data;
	net = list_foreach_data_ptr->net;
	scene_id = list_foreach_data_ptr->scene_id;
	node_id = list_foreach_data_ptr->node_id;
	event_list_ptr = list_foreach_data_ptr->list_ptr;
	invalid_event_list_ptr = list_foreach_data_ptr->list_ptr_dest;
	func_is_valid = list_foreach_data_ptr->zwscn_fn_is_valid;
	if((event_list_ptr == NULL) || (invalid_event_list_ptr == NULL) || (func_is_valid == NULL)) {
		zwscn_message_log_critical("zwscn_validate_security_events_from_valid_list_callback list_foreach_data_ptr members NULL\n");
		goto l_err;
	}
	
	event_node_id = zwscn_util_combo_id_to_node_id(event_ptr->id);
	event_ep_id = zwscn_util_combo_id_to_ep_id(event_ptr->id);
	event_cls_id = zwscn_util_combo_id_to_cls_id(event_ptr->id);
	zwscn_message_log_debug("checking event id=%d event_node_id=%d node_id=%d\n", event_ptr->id, event_node_id, node_id);
	if((node_id == 0) || (event_node_id == node_id)) {
		result = func_is_valid(net, ZWSCN_CHECK_NODE_EP_CLS, event_node_id, event_ep_id, event_cls_id);
		if(result == ZWSCN_FALSE) {
			(*event_list_ptr) = zwscn_list_remove((*event_list_ptr), event_ptr);
			(*invalid_event_list_ptr) = zwscn_list_add((*invalid_event_list_ptr), event_ptr);
			if((zwscn_find_security_scene(net, scene_id, &scene_ptr) == ZWSCN_ERR_NONE) && (scene_ptr != NULL)) {
				zwscn_security_scene_update_timestamp(scene_ptr);
			}
			zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_id);
		}
	}
	
l_err:
	return;
}

void zwscn_validate_security_events_from_invalid_list_callback(void *list_item, void *user_data) {
	void *net;
	zwscn_list_t *event_list_ptr;
	zwscn_list_t *invalid_event_list_ptr;
	zwscn_security_event_p event_ptr;
	zwscn_security_scene_cont_t *scene_ptr = NULL;
	uint8_t event_node_id;
	uint8_t event_ep_id;
	uint16_t event_cls_id;
	uint32_t scene_id;
	uint8_t node_id;
	uint8_t type;
	zwscn_list_foreach_data_t *list_foreach_data_ptr;
	zwscn_fn_is_valid_t	func_is_valid;
	int result;
	
	zwscn_message_log_debug("ENTER zwscn_validate_security_events_from_invalid_list_callback\n");
	if((list_item == NULL) || (user_data == NULL)) {
		zwscn_message_log_critical("zwscn_validate_security_events_from_invalid_list_callback input args NULL\n");
		goto l_err;
	}
	
	event_ptr = (zwscn_security_event_p) list_item;
	list_foreach_data_ptr = (zwscn_list_foreach_data_t *) user_data;
	net = list_foreach_data_ptr->net;
	scene_id = list_foreach_data_ptr->scene_id;
	node_id = list_foreach_data_ptr->node_id;
	event_list_ptr = list_foreach_data_ptr->list_ptr_dest;
	invalid_event_list_ptr = list_foreach_data_ptr->list_ptr;
	func_is_valid = list_foreach_data_ptr->zwscn_fn_is_valid;
	type = list_foreach_data_ptr->type;
	if((event_list_ptr == NULL) || (invalid_event_list_ptr == NULL) || (func_is_valid == NULL)) {
		zwscn_message_log_critical("zwscn_validate_security_events_from_invalid_list_callback list_foreach_data_ptr members NULL\n");
		goto l_err;
	}
	
	event_node_id = zwscn_util_combo_id_to_node_id(event_ptr->id);
	event_ep_id = zwscn_util_combo_id_to_ep_id(event_ptr->id);
	event_cls_id = zwscn_util_combo_id_to_cls_id(event_ptr->id);
	zwscn_message_log_debug("checking security event id=%d event_node_id=%d node_id=%d\n", event_ptr->id, event_node_id, node_id);
	if((node_id == 0) || (event_node_id == node_id)) {
		result = func_is_valid(net, ZWSCN_CHECK_NODE_EP_CLS, event_node_id, event_ep_id, event_cls_id);
		if(result == ZWSCN_TRUE) {
			if(type == ZWSCN_TYPE_ARM_SECURITY_EVENT) {
				if(zwscn_is_list_full_arm_security_event(net, (*event_list_ptr))) {
					zwscn_message_log_warning("Security Scene id=%d Arm event list is full. Can't move valid events from invalid list to valid list\n", scene_id);
					goto l_err_list_full;
				}
			} else if(type == ZWSCN_TYPE_DISARM_SECURITY_EVENT) {
				if(zwscn_is_list_full_disarm_security_event(net, (*event_list_ptr))) {
					zwscn_message_log_warning("Security Scene id=%d Disarm event list is full. Can't move valid events from invalid list to valid list\n", scene_id);
					goto l_err_list_full;
				}
			} else if(type == ZWSCN_TYPE_ALARM_SECURITY_EVENT) {
				if(zwscn_is_list_full_alarm_security_event(net, (*event_list_ptr))) {
					zwscn_message_log_warning("Security Scene id=%d Alarm event list is full. Can't move valid events from invalid list to valid list\n", scene_id);
					goto l_err_list_full;
				}
			}
			(*invalid_event_list_ptr) = zwscn_list_remove((*invalid_event_list_ptr), event_ptr);
			(*event_list_ptr) = zwscn_list_add((*event_list_ptr), event_ptr);
			if((zwscn_find_security_scene(net, scene_id, &scene_ptr) == ZWSCN_ERR_NONE) && (scene_ptr != NULL)) {
				zwscn_security_scene_update_timestamp(scene_ptr);
			}
			zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_id);
		}
	}
	
l_err:
l_err_list_full:
	return;
}


//Normal Scene Delete notification
ZWSCN_STATUS_T zwscn_security_scene_update_on_scene_deletion(void *net, uint32_t scene_id) {
	zwscn_list_t *scene_list;
	zwscn_security_scene_cont_p scene_ptr;
	int count=0;
	int i=0;
	int scene_modified = 0;
	
	if((net == NULL) || (scene_id == 0)) {
		goto l_err_args;
	} 
	
	if(zwscn_find_security_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	count = zwscn_list_size((*scene_list));
	for(i=0; i < count; ++i) {
		//TODO ABORT_OP function
		scene_modified = 0;
		scene_ptr = zwscn_list_get_by_index((*scene_list), i);
		if(scene_ptr->scene->scene_id_at_arm == scene_id) {
			scene_ptr->scene->scene_id_at_arm = 0;
			++scene_modified;
		}
		if(scene_ptr->scene->scene_id_at_disarm == scene_id) {
			scene_ptr->scene->scene_id_at_disarm = 0;
			++scene_modified;
		}
		if(scene_ptr->scene->scene_id_at_alarm == scene_id) {
			scene_ptr->scene->scene_id_at_alarm = 0;
			++scene_modified;
		}
		if(scene_modified) {
			zwscn_security_scene_update_timestamp(scene_ptr);
			zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_ptr->scene->id);
		}
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;	
}
