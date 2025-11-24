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
@file		zwave-scenes-statelog.c  
  
                To provide support for scenes state logging which may be used by the client to update the UI
  
@author		Arif Mohammand
 
@version	1.0 2013-12-03  Initial release
*/
#include <stdlib.h>
#include <stdint.h>
#include "zwscn_defines.h"
#include "zwscn_util_hash.h"
#include "zwscn_util_list.h"
#include "zwscn_util_message_log.h"
#include "zwscn_util_mqueue.h"
#include "zwscn_util_thread.h"
#include "zwscn_util_mutex.h"
#include "zwscn_util_rwlock.h"
#include "zwscn_util_timer.h"
#include "zwscn_scene_api.h"
#include "zwscn_security_scene_api.h"
#include "zwscn_util_time.h"
#include "zwscn_statelog.h"
#include "zwscn_scene.h"
#include "zwscn_security_scene.h"

void zwscn_statelog_set(void *net, uint32_t category, uint32_t subcategory, uint64_t value) {
	zwscn_statelog_p statelog_scene_data_ptr = NULL;
	
	//zwscn_debug("StateLog SET Category:%d Subcategory:%d value:%d\n", category, subcategory, value);
	
	statelog_scene_data_ptr = zwscn_get_statelog_data(net, category);
	
	if(statelog_scene_data_ptr != NULL) {
		if(subcategory == ZWSCN_STATELOG_SUBCATEGORY_CURR_OP){
			statelog_scene_data_ptr->curr_op = value;
		} else if(subcategory == ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_SCENED){
			statelog_scene_data_ptr->curr_op_scened = value;
		} else if(subcategory == ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_TRIGGER){
			statelog_scene_data_ptr->curr_op_trigger = value;
		} else if(subcategory == ZWSCN_STATELOG_SUBCATEGORY_LAST_OP){
			statelog_scene_data_ptr->last_op = value;
		} else if(subcategory == ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_SCENED){
			statelog_scene_data_ptr->last_op_scened = value;
		} else if(subcategory == ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_TRIGGER){
			statelog_scene_data_ptr->last_op_trigger = value;
		} else if(subcategory == ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_TIME){
			statelog_scene_data_ptr->last_op_time = value;
		} 
	}
}

ZWSCN_STATUS_T zwscn_statelog_get(void *net, uint32_t category, uint32_t subcategory, uint64_t *value) {
	zwscn_statelog_p statelog_scene_data_ptr = NULL;
	
	if(value == NULL) {
		goto l_err_args;
	}
	
	statelog_scene_data_ptr = zwscn_get_statelog_data(net, category);
	
	if(statelog_scene_data_ptr != NULL){
		if(subcategory == ZWSCN_STATELOG_SUBCATEGORY_CURR_OP){
			(*value) = statelog_scene_data_ptr->curr_op;
		} else if(subcategory == ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_SCENED){
			(*value) = statelog_scene_data_ptr->curr_op_scened;
		} else if(subcategory == ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_TRIGGER){
			(*value) = statelog_scene_data_ptr->curr_op_trigger;
		} else if(subcategory == ZWSCN_STATELOG_SUBCATEGORY_LAST_OP){
			(*value) = statelog_scene_data_ptr->last_op;
		} else if(subcategory == ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_SCENED){
			(*value) = statelog_scene_data_ptr->last_op_scened;
		} else if(subcategory == ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_TRIGGER){
			(*value) = statelog_scene_data_ptr->last_op_trigger;
		} else if(subcategory == ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_TIME){
			(*value) = statelog_scene_data_ptr->last_op_time;
		} 
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
}

void zwscn_set_statelog_start_op(void *net, uint32_t scene_id, uint8_t trigger, uint8_t category, uint8_t op) {
	zwscn_statelog_lock(net);
	zwscn_statelog_set(net, category, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP, op);
	zwscn_statelog_set(net, category, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_SCENED, scene_id);
	zwscn_statelog_set(net, category, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_TRIGGER, trigger);
	zwscn_statelog_unlock(net);
}

void zwscn_set_statelog_end_op(void *net, uint32_t scene_id, uint8_t trigger, uint8_t category, uint8_t op) {
	zwscn_statelog_lock(net);
	zwscn_time_t time_now = ZWSCN_TIME_NOW();
	zwscn_time_t time_last = 0;
	
	zwscn_statelog_set(net, category, ZWSCN_STATELOG_SUBCATEGORY_LAST_OP, op);
	zwscn_statelog_set(net, category, ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_SCENED, scene_id);
	zwscn_statelog_set(net, category, ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_TRIGGER, trigger);
	if(category == ZWSCN_STATELOG_CATEGORY_STATUS) {
		zwscn_statelog_get(net, category, ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_TIME, &time_last);
//		ZWSCN_MESSAGE_LOG_DEBUG("STATELOG_CATEGORY_STATUS time_last=%llu time_now=%llu diff=%llu\n", time_last, time_now, (time_now-time_last));
		if(time_last >= time_now) {
			zwscn_statelog_set(net, category, ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_TIME, (time_last + 1)); //if last op time is the same then UI might get confused, so add 1
		} else {
			zwscn_statelog_set(net, category, ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_TIME, time_now);
		}
	} else {
		zwscn_statelog_set(net, category, ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_TIME, time_now);
	}
	
	zwscn_statelog_set(net, category, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP, ZWSCN_STATELOG_OP_NONE);
	zwscn_statelog_set(net, category, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_SCENED, 0);
	zwscn_statelog_set(net, category, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_TRIGGER, ZWSCN_STATELOG_TRIGGER_NONE);
	zwscn_statelog_unlock(net);
}

void zwscn_set_statelog_end_op_without_last_op(void *net, uint32_t scene_id, uint8_t trigger, uint8_t category, uint8_t op) {
	zwscn_statelog_lock(net);
	zwscn_statelog_set(net, category, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP, ZWSCN_STATELOG_OP_NONE);
	zwscn_statelog_set(net, category, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_SCENED, 0);
	zwscn_statelog_set(net, category, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_TRIGGER, ZWSCN_STATELOG_TRIGGER_NONE);
	zwscn_statelog_unlock(net);
}

void zwscn_set_statelog_start_op_add(void *net, uint32_t scene_id, uint8_t trigger) {
	zwscn_set_statelog_start_op(net, scene_id, trigger, ZWSCN_STATELOG_CATEGORY_ADD, ZWSCN_STATELOG_OP_SCENE_ADD);
}

void zwscn_set_statelog_end_op_add(void *net, uint32_t scene_id, uint8_t trigger) {
	zwscn_set_statelog_end_op(net, scene_id, trigger, ZWSCN_STATELOG_CATEGORY_ADD, ZWSCN_STATELOG_OP_SCENE_ADD);
}

void zwscn_set_statelog_start_op_add_modify(void *net, uint32_t scene_id, uint8_t trigger) {
	zwscn_set_statelog_start_op(net, scene_id, trigger, ZWSCN_STATELOG_CATEGORY_ADD, ZWSCN_STATELOG_OP_SCENE_ADD_MODIFY);
}

void zwscn_set_statelog_end_op_add_modify(void *net, uint32_t scene_id, uint8_t trigger) {
	zwscn_set_statelog_end_op(net, scene_id, trigger, ZWSCN_STATELOG_CATEGORY_ADD, ZWSCN_STATELOG_OP_SCENE_ADD_MODIFY);
}

void zwscn_set_statelog_start_op_del(void *net, uint32_t scene_id, uint8_t trigger) {
	zwscn_set_statelog_start_op(net, scene_id, trigger, ZWSCN_STATELOG_CATEGORY_DELETE, ZWSCN_STATELOG_OP_SCENE_DELETE);
}

void zwscn_set_statelog_end_op_del(void *net, uint32_t scene_id, uint8_t trigger) {
	zwscn_set_statelog_end_op(net, scene_id, trigger, ZWSCN_STATELOG_CATEGORY_DELETE, ZWSCN_STATELOG_OP_SCENE_DELETE);
}

void zwscn_set_statelog_start_op_del_modify(void *net, uint32_t scene_id, uint8_t trigger) {
	zwscn_set_statelog_start_op(net, scene_id, trigger, ZWSCN_STATELOG_CATEGORY_DELETE, ZWSCN_STATELOG_OP_SCENE_DELETE_MODIFY);
}

void zwscn_set_statelog_end_op_del_modify(void *net, uint32_t scene_id, uint8_t trigger) {
	zwscn_set_statelog_end_op(net, scene_id, trigger, ZWSCN_STATELOG_CATEGORY_DELETE, ZWSCN_STATELOG_OP_SCENE_DELETE_MODIFY);
}

void zwscn_set_statelog_start_op_exec(void *net, uint32_t scene_id, uint8_t trigger) {
	zwscn_set_statelog_start_op(net, scene_id, trigger, ZWSCN_STATELOG_CATEGORY_EXEC, ZWSCN_STATELOG_OP_SCENE_EXEC);
}

void zwscn_set_statelog_end_op_exec(void *net, uint32_t scene_id, uint8_t trigger) {
	zwscn_set_statelog_end_op(net, scene_id, trigger, ZWSCN_STATELOG_CATEGORY_EXEC, ZWSCN_STATELOG_OP_SCENE_EXEC);
}

void zwscn_set_statelog_start_op_status_update(void *net, uint32_t scene_id, uint8_t trigger) {
	uint64_t value=0;
	
	zwscn_statelog_lock(net);
	zwscn_statelog_get(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP, &value);
	if((value == ZWSCN_STATELOG_OP_SCENE_STATUS_UPDATE_ALL) || (value == ZWSCN_STATELOG_OP_SCENE_STATUS_UPDATE_PENDING)) {
		zwscn_statelog_set(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_SCENED, scene_id);
		zwscn_statelog_set(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_TRIGGER, trigger);
		zwscn_statelog_unlock(net);
	} else {
		zwscn_statelog_unlock(net);
		zwscn_set_statelog_start_op(net, scene_id, trigger, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_OP_SCENE_STATUS_UPDATE_SINGLE);
	}
}

void zwscn_set_statelog_end_op_status_update(void *net, uint32_t scene_id, uint8_t trigger) {
	uint64_t value=0;
	
	zwscn_statelog_lock(net);
	zwscn_statelog_get(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP, &value);
	if((value == ZWSCN_STATELOG_OP_SCENE_STATUS_UPDATE_ALL) || (value == ZWSCN_STATELOG_OP_SCENE_STATUS_UPDATE_PENDING)) {
		//The last op information is updated when Scene is actually updated after report is received
//		zwscn_statelog_set(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_LAST_OP, ZWSCN_STATELOG_OP_SCENE_STATUS_UPDATE_ALL);
//		zwscn_statelog_set(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_SCENED, scene_id);
//		zwscn_statelog_set(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_TRIGGER, trigger);
//		zwscn_statelog_set(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_TIME, ZWSCN_TIME_NOW());
		
		//Curr_op is not reset as it will be done by Status Update All funtion
		zwscn_statelog_set(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_SCENED, 0);
		zwscn_statelog_set(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_TRIGGER, ZWSCN_STATELOG_TRIGGER_NONE);
		zwscn_statelog_unlock(net);
	} else {
		zwscn_statelog_unlock(net);
		zwscn_set_statelog_end_op_without_last_op(net, scene_id, trigger, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_OP_SCENE_STATUS_UPDATE_SINGLE);
	}
}

void zwscn_set_statelog_start_op_status_update_on_report(void *net, uint32_t scene_id, uint8_t trigger) {
	zwscn_set_statelog_start_op_status_update(net, scene_id, trigger);
}

void zwscn_set_statelog_end_op_status_update_on_report(void *net, uint32_t scene_id, uint8_t trigger) {
	uint64_t value=0;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	zwscn_statelog_lock(net);
	zwscn_statelog_get(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP, &value);
	if((value == ZWSCN_STATELOG_OP_SCENE_STATUS_UPDATE_ALL) || (value == ZWSCN_STATELOG_OP_SCENE_STATUS_UPDATE_PENDING)) {
		zwscn_statelog_set(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_LAST_OP, ZWSCN_STATELOG_OP_SCENE_STATUS_UPDATE_ALL);
		zwscn_statelog_set(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_SCENED, scene_id);
		zwscn_statelog_set(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_TRIGGER, trigger);
		zwscn_statelog_set(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_TIME, ZWSCN_TIME_NOW());
		
		//Curr_op is not reset as it will be done by Status Update All funtion
		zwscn_statelog_set(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_SCENED, 0);
		zwscn_statelog_set(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_TRIGGER, ZWSCN_STATELOG_TRIGGER_NONE);
		zwscn_statelog_unlock(net);
	} else {
		zwscn_statelog_unlock(net);
		zwscn_set_statelog_end_op(net, scene_id, trigger, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_OP_SCENE_STATUS_UPDATE_SINGLE);
	}
}

void zwscn_set_statelog_start_op_status_update_all(void *net) {
	zwscn_statelog_lock(net);
	zwscn_statelog_set(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP, ZWSCN_STATELOG_OP_SCENE_STATUS_UPDATE_ALL);
	zwscn_statelog_unlock(net);
}

void zwscn_set_statelog_end_op_status_update_all(void *net) {
	zwscn_statelog_lock(net);
	//The last op information is updated when Scene is actually updated after report is received
	//zwscn_statelog_set(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_TIME, ZWSCN_TIME_NOW());
	zwscn_statelog_set(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP, ZWSCN_STATELOG_OP_NONE);
	zwscn_statelog_unlock(net);
}

void zwscn_set_statelog_start_op_status_update_pending(void *net) {
	zwscn_statelog_lock(net);
	zwscn_statelog_set(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP, ZWSCN_STATELOG_OP_SCENE_STATUS_UPDATE_PENDING);
	zwscn_statelog_unlock(net);
}

void zwscn_set_statelog_end_op_status_update_pending(void *net) {
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	zwscn_statelog_lock(net);
	//The last op information is updated when Scene is actually updated after report is received
	zwscn_statelog_set(net, ZWSCN_STATELOG_CATEGORY_STATUS, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP, ZWSCN_STATELOG_OP_NONE);
	zwscn_statelog_unlock(net);
}


void zwscn_statelog_security_scene_set(void *net, uint32_t category, uint32_t scened) {
	zwscn_statelog_security_scene_p statelog_scene_data_ptr = NULL;
	
	//zwscn_debug("StateLog SET Category:%d Subcategory:%d value:%d\n", category, subcategory, value);
	zwscn_statelog_lock(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	statelog_scene_data_ptr = zwscn_security_scene_get_statelog_data(net, category);
	
	if(statelog_scene_data_ptr != NULL) {
		statelog_scene_data_ptr->scened = scened;
		statelog_scene_data_ptr->counter += 1;
	}
	zwscn_statelog_unlock(net);
	
	return;
l_err_network_invalid:
	zwscn_statelog_unlock(net);
	return;
}

ZWSCN_STATUS_T zwscn_statelog_security_scene_get(void *net, uint32_t category, uint32_t *counter_ptr, uint32_t *scened_ptr) {
	zwscn_statelog_security_scene_p statelog_scene_data_ptr = NULL;
	
	if((net == NULL) || (counter_ptr == NULL) || (scened_ptr == NULL)){
		goto l_err_args;
	}
	
	zwscn_statelog_lock(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	statelog_scene_data_ptr = zwscn_security_scene_get_statelog_data(net, category);
	
	if(statelog_scene_data_ptr != NULL){
		(*counter_ptr) = statelog_scene_data_ptr->counter;
		(*scened_ptr) = statelog_scene_data_ptr->scened;
	}
	zwscn_statelog_unlock(net);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err_network_invalid:
	zwscn_statelog_unlock(net);
	return ZWSCN_ERR_INVALID_NETWORK;
}


