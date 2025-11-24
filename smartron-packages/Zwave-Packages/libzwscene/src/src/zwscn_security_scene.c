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
@file		zwscn_security_scene.c  
  
                Implentation for genral functions for Security Scene core functionality
  
@author		Arif Mohammand
 
@version	1.0 2015-04-16  Initial release
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "zwscn_defines.h"
#include "zwscn_util_hash.h"
#include "zwscn_util_list.h"
#include "zwscn_util_mqueue.h"
#include "zwscn_util_thread.h"
#include "zwscn_util_mutex.h"
#include "zwscn_util_rwlock.h"
#include "zwscn_util_timer.h"
#include "zwscn_scene_api.h"
#include "zwscn_security_scene_api.h"
#include "zwscn_statelog.h"
#include "zwscn_scene.h"
#include "zwscn_scene_execute.h"
#include "zwscn_util_time.h"
#include "zwscn_util_mem.h"
#include "zwscn_util_message_log.h"
#include "zwscn_security_scene.h"
#include "zwscn_security_scene_notifications.h"
#include "zwscn_persistence_security_scene.h"
#include "zwscn_util_zwave.h"

static ZWSCN_STATUS_T zwscn_security_scene_execute_scene(void *net, uint32_t scene_id);

ZWSCN_STATUS_T zwscn_find_mqueue_thread_security_events(void *net, zwscn_mqueue_t *mqueue_ptr) {
	zwscn_init_p scene_init_data;

	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}

	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	
	if(scene_init_data->mqueue_thread_security_events == NULL) {
		goto l_err;
	}

	(*mqueue_ptr) = scene_init_data->mqueue_thread_security_events;

	return ZWSCN_ERR_NONE;

l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_mqueue_thread_security_scene_notification_request_handler(void *net, zwscn_mqueue_t *mqueue_ptr) {
	zwscn_init_p scene_init_data;

	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}

	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	
	if(scene_init_data->mqueue_thread_security_events == NULL) {
		goto l_err;
	}

	(*mqueue_ptr) = scene_init_data->mqueue_thread_security_scene_notification_request_handler;

	return ZWSCN_ERR_NONE;

l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_security_scene_list(void *net, zwscn_list_t **scene_list_ptr) {
	zwscn_init_p scene_init_data;

	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}

	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}

	(*scene_list_ptr) = &(scene_init_data->list_security_scenes);

	return ZWSCN_ERR_NONE;

l_err:
	return ZWSCN_ERR_GENERIC;
}


ZWSCN_STATUS_T zwscn_find_arm_security_event_list(void *net, uint32_t scene_id, zwscn_list_t **list_ptr) {
	zwscn_list_t *scene_list;
	zwscn_security_scene_cont_p scene_ptr;

	if(zwscn_find_security_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}

	if((scene_ptr = zwscn_list_get_security_scene_by_id((*scene_list), scene_id)) == NULL) {
		goto l_err;
	}
	
	*(list_ptr) = &(scene_ptr->list_arm_events);
	
	return ZWSCN_ERR_NONE;
l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_disarm_security_event_list(void *net, uint32_t scene_id, zwscn_list_t **list_ptr) {
	zwscn_list_t *scene_list;
	zwscn_security_scene_cont_p scene_ptr;

	if(zwscn_find_security_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}

	if((scene_ptr = zwscn_list_get_security_scene_by_id((*scene_list), scene_id)) == NULL) {
		goto l_err;
	}
	
	*(list_ptr) = &(scene_ptr->list_disarm_events);
	
	return ZWSCN_ERR_NONE;
l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_alarm_security_event_list(void *net, uint32_t scene_id, zwscn_list_t **list_ptr) {
	zwscn_list_t *scene_list;
	zwscn_security_scene_cont_p scene_ptr;

	if(zwscn_find_security_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}

	if((scene_ptr = zwscn_list_get_security_scene_by_id((*scene_list), scene_id)) == NULL) {
		goto l_err;
	}
	
	*(list_ptr) = &(scene_ptr->list_alarm_events);
	
	return ZWSCN_ERR_NONE;
l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_security_scene(void *net, uint32_t scene_id, zwscn_security_scene_cont_p *scene_ptr) {
	zwscn_list_t *scene_list;

	if(zwscn_find_security_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}

	if(((*scene_ptr) = zwscn_list_get_security_scene_by_id((*scene_list), scene_id)) == NULL) {
		goto l_err;
	}

	return ZWSCN_ERR_NONE;
//l_err_args:
//	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_arm_security_event(void *net, uint32_t scene_id, uint32_t id, zwscn_event_p *out_event) {
	zwscn_list_t *event_list;
//	zwscn_security_event_p event;

	if(zwscn_find_arm_security_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}


	if(((*out_event) = zwscn_list_get_security_event_by_id((*event_list), id)) == NULL) {
		goto l_err;
	}
	

	return ZWSCN_ERR_NONE;
l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_disarm_security_event(void *net, uint32_t scene_id, uint32_t id, zwscn_event_p *out_event) {
	zwscn_list_t *event_list;
//	zwscn_security_event_p event;

	if(zwscn_find_disarm_security_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}


	if(((*out_event) = zwscn_list_get_security_event_by_id((*event_list), id)) == NULL) {
		goto l_err;
	}
	

	return ZWSCN_ERR_NONE;
l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_alarm_security_event(void *net, uint32_t scene_id, uint32_t id, zwscn_event_p *out_event) {
	zwscn_list_t *event_list;
//	zwscn_security_event_p event;

	if(zwscn_find_alarm_security_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}


	if(((*out_event) = zwscn_list_get_security_event_by_id((*event_list), id)) == NULL) {
		goto l_err;
	}
	

	return ZWSCN_ERR_NONE;
l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_BOOL_T zwscn_is_list_full_security_scene(void *net, zwscn_list_t scene_list) {
	int list_length=0;
	int max_limit=0;
	zwscn_init_p scene_init_data;
	
	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	list_length = zwscn_list_size(scene_list);
	if(scene_init_data->init_input) {
		max_limit = (scene_init_data->init_input->num_max_security_scenes < 0) ? ZWSCN_MAX_SECURITY_SCENES : scene_init_data->init_input->num_max_security_scenes;
	}
	if(list_length < max_limit) {
		return ZWSCN_FALSE;
	}
	
	return ZWSCN_TRUE;
	
l_err:
	return ZWSCN_TRUE; //In case of error we assume that list is full
}

ZWSCN_BOOL_T zwscn_is_list_full_arm_security_event(void *net, zwscn_list_t event_list) {
	int list_length=0;
	int max_limit=0;
	zwscn_init_p scene_init_data;
	
	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	list_length = zwscn_list_size(event_list);
	if(scene_init_data->init_input) {
		max_limit = (scene_init_data->init_input->num_max_arm_security_events_per_scene < 0) ? ZWSCN_MAX_ARM_SECURITY_EVENTS : scene_init_data->init_input->num_max_arm_security_events_per_scene;
	}
	if(list_length < max_limit) {
		return ZWSCN_FALSE;
	}
	
	return ZWSCN_TRUE;
	
l_err:
	return ZWSCN_TRUE; //In case of error we assume that list is full
}

ZWSCN_BOOL_T zwscn_is_list_full_disarm_security_event(void *net, zwscn_list_t event_list) {
	int list_length=0;
	int max_limit=0;
	zwscn_init_p scene_init_data;
	
	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	list_length = zwscn_list_size(event_list);
	if(scene_init_data->init_input) {
		max_limit = (scene_init_data->init_input->num_max_disarm_security_events_per_scene < 0) ? ZWSCN_MAX_DISARM_SECURITY_EVENTS : scene_init_data->init_input->num_max_disarm_security_events_per_scene;
	}
	if(list_length < max_limit) {
		return ZWSCN_FALSE;
	}
	
	return ZWSCN_TRUE;
	
l_err:
	return ZWSCN_TRUE; //In case of error we assume that list is full
}

ZWSCN_BOOL_T zwscn_is_list_full_alarm_security_event(void *net, zwscn_list_t event_list) {
	int list_length=0;
	int max_limit=0;
	zwscn_init_p scene_init_data;
	
	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	list_length = zwscn_list_size(event_list);
	if(scene_init_data->init_input) {
		max_limit = (scene_init_data->init_input->num_max_alarm_security_events_per_scene < 0) ? ZWSCN_MAX_ALARM_SECURITY_EVENTS : scene_init_data->init_input->num_max_alarm_security_events_per_scene;
	}
	if(list_length < max_limit) {
		return ZWSCN_FALSE;
	}
	
	return ZWSCN_TRUE;
	
l_err:
	return ZWSCN_TRUE; //In case of error we assume that list is full
}

uint32_t zwscn_generate_security_scene_id(void *net){
	zwscn_init_p scene_init_data;
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}
	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	++(scene_init_data->pool_security_scene_id);
	if((scene_init_data->pool_security_scene_id < ZWSCN_SECURITY_SCENE_ID_MIN) || (scene_init_data->pool_security_scene_id > ZWSCN_SECURITY_SCENE_ID_MAX)) {
		scene_init_data->pool_security_scene_id = ZWSCN_SECURITY_SCENE_ID_MIN;
	}
	
	return scene_init_data->pool_security_scene_id;
l_err:
	return 0; //invalid scene id
}

uint32_t zwscn_get_security_scene_id_pool_index(void *net) {
	zwscn_init_p scene_init_data;
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}
	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	
	return scene_init_data->pool_security_scene_id;
l_err:
	return 0; //invalid scene id
}

void zwscn_set_security_scene_id_pool_index(void *net, uint32_t index) {
	zwscn_init_p scene_init_data;
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}
	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	
	scene_init_data->pool_security_scene_id = index;
l_err:
	return;
}

void zwscn_security_scene_arm_state_set(void *net, zwscn_security_scene_cont_p scene_ptr, ZWSCN_BOOL_T flag_arm_disarm_state, uint8_t trigger_type, zwscn_security_event_p event) {
	uint32_t scene_id;
	ZWSCN_BOOL_T last_val;
	
	//TODO protect data using mutex
	last_val = scene_ptr->scene->is_armed;
	scene_ptr->scene->is_armed = flag_arm_disarm_state;
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_ARM, scene_ptr->scene->id);
	
	if(flag_arm_disarm_state == ZWSCN_TRUE) {
		scene_id = scene_ptr->scene->scene_id_at_arm;
		
		scene_ptr->scene->lt_arm.type = trigger_type;
		scene_ptr->scene->lt_arm.time = ZWSCN_TIME_NOW();
		zwscn_free_security_event(scene_ptr->scene->lt_arm.event);
		scene_ptr->scene->lt_arm.event = zwscn_security_scene_copy_event(event);
		
	} else {
		scene_id = scene_ptr->scene->scene_id_at_disarm;
		
		scene_ptr->scene->lt_disarm.type = trigger_type;
		scene_ptr->scene->lt_disarm.time = ZWSCN_TIME_NOW();
		zwscn_free_security_event(scene_ptr->scene->lt_disarm.event);		
		scene_ptr->scene->lt_disarm.event = zwscn_security_scene_copy_event(event);
	}
	if(scene_id != 0) {
		zwscn_security_scene_execute_scene(net, scene_id);
	} else {
		zwscn_message_log_debug("Ignoring execution of Scene with id 0 on Arm/Disarm event");
	}
	if(last_val != flag_arm_disarm_state) { //Save the Security Scene is the state has changed
		if(zwscn_persistence_save_security_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
			goto l_err;
		}
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
	
	return;
l_err:
	return; //Nothing to do until we change return value
}

ZWSCN_BOOL_T zwscn_security_scene_arm_state_get(zwscn_security_scene_cont_p scene_ptr) {
	return scene_ptr->scene->is_armed;
}

void zwscn_security_scene_alarm_state_set(void *net, zwscn_security_scene_cont_p scene_ptr, ZWSCN_BOOL_T flag_alarm_state, uint8_t trigger_type, zwscn_security_event_p event) {
	uint32_t scene_id;
	ZWSCN_BOOL_T last_val;
	zwscn_security_scene_p scene_copy = NULL;
	
	//TODO protect data using mutex
	last_val = scene_ptr->scene->is_alarmed;
	scene_ptr->scene->is_alarmed = flag_alarm_state;
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_ALARM, scene_ptr->scene->id);
	
	if(flag_alarm_state == ZWSCN_TRUE) {
		scene_ptr->scene->lt_alarm_on.type = trigger_type;
		scene_ptr->scene->lt_alarm_on.time = ZWSCN_TIME_NOW();
		zwscn_free_security_event(scene_ptr->scene->lt_alarm_on.event);
		scene_ptr->scene->lt_alarm_on.event = zwscn_security_scene_copy_event(event);
				
		scene_id = scene_ptr->scene->scene_id_at_alarm;
		if(scene_id != 0) {
			zwscn_security_scene_execute_scene(net, scene_id);
		} else {
			zwscn_message_log_debug("Ignoring execution of Scene with id 0 on Alarm event");
		}
		scene_copy = calloc(1, sizeof(zwscn_security_scene_t));
		if(scene_copy == NULL) {
			zwscn_message_log_critical("Could not allocate memory needed for security scene notification request\n");
		} else {
			(*scene_copy) = (*(scene_ptr->scene)); //C structure copy
			scene_copy->lt_arm.event = NULL;
			scene_copy->lt_disarm.event = NULL;
			scene_copy->lt_alarm_on.event = NULL;
			scene_copy->lt_alarm_off.event = NULL;
			scene_copy->lt_alarm_on.event = zwscn_security_scene_copy_event(event);
			zwscn_security_scene_notification_request_message_send(net, scene_copy, ZWSCN_MESSAGE_TYPE_SECURITY_SCENE_NOTIFICATION_ALARM);
		}
	} else {
		scene_ptr->scene->lt_alarm_off.type = trigger_type;
		scene_ptr->scene->lt_alarm_off.time = ZWSCN_TIME_NOW();
		zwscn_free_security_event(scene_ptr->scene->lt_alarm_off.event);
		scene_ptr->scene->lt_alarm_off.event = zwscn_security_scene_copy_event(event); 
	}
	
	if(last_val != flag_alarm_state) { //Save the Security Scene as the state has changed
		if(zwscn_persistence_save_security_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
			goto l_err;
		}
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
	
	return;
l_err:
	return; //Nothing to do until we change return value
}

ZWSCN_BOOL_T zwscn_security_scene_alarm_state_get(zwscn_security_scene_cont_p scene_ptr) {
	return scene_ptr->scene->is_alarmed;
}

static ZWSCN_STATUS_T zwscn_security_scene_execute_scene(void *net, uint32_t scene_id) {
	ZWSCN_STATUS_T  result = ZWSCN_ERR_NONE;
	
	zwscn_message_log_debug("SECURITY EVENT Sending a request to execute scene %d\n", scene_id);
	//if(zwscn_is_valid_scene_id()) //better check at scene creation, edit and load from persistent storage 
	result = zwscn_execute_request_message_send(net, scene_id, ZWSCN_MESSAGE_TYPE_EXECUTE_SCENE_ON_EVENT); //TODO use different message type
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("(%d) retuned by zwscn_execute_request_message_send\n", result);
		zwscn_message_log_critical("Scene execution failed on SECURITY EVENT trigger.\n");
		//Even though there was an error, we ignore it and continue
	}
	
	return result;
}

zwscn_statelog_security_scene_p zwscn_security_scene_get_statelog_data(void *net, uint32_t category) {
	zwscn_statelog_security_scene_p statelog_data_ptr = NULL;
	zwscn_init_p scene_init_data;
	
	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	
	switch(category) {
		case ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_ADD:
			statelog_data_ptr = &(scene_init_data->statelog_security_scene_add_data);
			break;
		case ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_DELETE:
			statelog_data_ptr = &(scene_init_data->statelog_security_scene_delete_data);
			break;
		case ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT:
			statelog_data_ptr = &(scene_init_data->statelog_security_scene_edit_data);
			break;
		case ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_ARM:
			statelog_data_ptr = &(scene_init_data->statelog_security_scene_arm_data);
			break;
		case ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_ALARM:
			statelog_data_ptr = &(scene_init_data->statelog_security_scene_alarm_data);
			break;
		default:
			zwscn_message_log_warning("Statelog category %d not found\n", category);
	}
	
	return statelog_data_ptr;
l_err:
	return NULL;
}

zwscn_security_event_p zwscn_security_scene_copy_event(zwscn_security_event_p event) {
	zwscn_security_event_p event_ptr = NULL;
	
	if(event == NULL) {
		goto l_err_args;
	}
	event_ptr = (zwscn_security_event_p) calloc(1, sizeof(zwscn_security_event_t));
	if(event_ptr == NULL) {
		goto l_err_memory;
	}
	*(event_ptr) = *(event); // C compiler in-built structure copy
	if(event->device_args && (event->device_args_size != 0)) {
		if (zwscn_copy_device_arg_2_lt(event, event_ptr) != ZWSCN_ERR_NONE)
			goto l_err_memory_2;
	} else {
		event_ptr->device_args = NULL;
	}
	
	return event_ptr;
l_err_memory_2:
	free(event_ptr);
l_err_memory:
l_err_args:
	return NULL;
}

void zwscn_security_scene_update_timestamp(zwscn_security_scene_cont_p scene_ptr) {
	if((scene_ptr != NULL) && (scene_ptr->scene != NULL)) {
		scene_ptr->scene->utime = ZWSCN_TIME_NOW();
	}
}

int zwscn_get_max_arm_events(void *net) {
	zwscn_init_p init_data;
	int max_limit=0;
	
	if((init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	if(init_data->init_input) {
		max_limit = (init_data->init_input->num_max_arm_security_events_per_scene < 0) ? ZWSCN_MAX_ARM_SECURITY_EVENTS : init_data->init_input->num_max_arm_security_events_per_scene;
	}
	
	return max_limit;
l_err:
	return 0;
}

int zwscn_get_max_disarm_events(void *net) {
	zwscn_init_p init_data;
	int max_limit=0;
	
	if((init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	if(init_data->init_input) {
		max_limit = (init_data->init_input->num_max_disarm_security_events_per_scene < 0) ? ZWSCN_MAX_DISARM_SECURITY_EVENTS : init_data->init_input->num_max_disarm_security_events_per_scene;
	}
	
	return max_limit;
l_err:
	return 0;
}

int zwscn_get_max_alarm_events(void *net) {
	zwscn_init_p init_data;
	int max_limit=0;
	
	if((init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	if(init_data->init_input) {
		max_limit = (init_data->init_input->num_max_alarm_security_events_per_scene < 0) ? ZWSCN_MAX_ALARM_SECURITY_EVENTS : init_data->init_input->num_max_alarm_security_events_per_scene;
	}
	
	return max_limit;
l_err:
	return 0;
}
