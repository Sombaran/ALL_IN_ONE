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
@file		zwscn_scene.c  
  
                Implentation for genral functions for Scene core functionality
  
@author		Arif Mohammand
 
@version	1.0 2013-12-05  Initial release
*/

#include <stdlib.h>
#include <stdint.h>
#include "zwscn_defines.h"
#include "zwscn_util_hash.h"
#include "zwscn_util_list.h"
#include "zwscn_util_mqueue.h"
#include "zwscn_util_thread.h"
#include "zwscn_util_mutex.h"
#include "zwscn_util_rwlock.h"
#include "zwscn_util_timer.h"
#include "zwscn_scene_api.h"
#include "zwscn_statelog.h"
#include "zwscn_scene.h"
#include "zwscn_util_message_log.h"

zwscn_global_data_t zwscn_global_db;

static ZWSCN_STATUS_T zwscn_find_list_by_type(void *net, uint32_t scene_id, ZWSCN_MEMBERS_TYPE_T list_type, zwscn_list_t **list_ptr);

void zwscn_global_hash_lock() {
	zwscn_global_data_p global_db;
	
	//zwscn_debug("global_hash_lock\n");
	global_db = zwscn_get_global_db();
	if(global_db && (global_db->initialized) && global_db->network_hash_mutex) {
		zwscn_rec_mutex_lock(global_db->network_hash_mutex);
	}
}

void zwscn_global_hash_unlock() {
	zwscn_global_data_p global_db;
	
	//zwscn_debug("global_hash_unlock\n");
	global_db = zwscn_get_global_db();
	if(global_db && (global_db->initialized) && global_db->network_hash_mutex) {
		zwscn_rec_mutex_unlock(global_db->network_hash_mutex);
	}
}

void zwscn_unavailable_network_hash_lock() {
	zwscn_global_data_p global_db;
	
	//zwscn_debug("unavailable_network_hash_lock\n");
	global_db = zwscn_get_global_db();
	if(global_db && (global_db->initialized) && global_db->unavailable_network_hash_mutex) {
		zwscn_rec_mutex_lock(global_db->unavailable_network_hash_mutex);
	}
}

void zwscn_unavailable_network_hash_unlock() {
	zwscn_global_data_p global_db;
	
	//zwscn_debug("unavailable_network_hash_unlock\n");
	global_db = zwscn_get_global_db();
	if(global_db && (global_db->initialized) && global_db->unavailable_network_hash_mutex) {
		zwscn_rec_mutex_unlock(global_db->unavailable_network_hash_mutex);
	}
}

//void zwscn_network_lock(void *net) {
//	zwscn_init_p init_data = NULL;
//	
//	if(net == NULL) {
//		return;
//	}
//	init_data = zwscn_get_init_data(net);
//	if(init_data && init_data->net_mutex) {
//		zwscn_mutex_lock(init_data->net_mutex);
//	}
//}
//
//void zwscn_network_unlock(void *net) {
//	zwscn_init_p init_data = NULL;
//	
//	if(net == NULL) {
//		return;
//	}
//	init_data = zwscn_get_init_data(net);
//	if(init_data && init_data->net_mutex) {
//		zwscn_mutex_unlock(init_data->net_mutex);
//	}
//}

void zwscn_network_lock_reader(void *net) {
	zwscn_init_p init_data = NULL;
	
	if(net == NULL) {
		return;
	}
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->net_rwlock) {
		zwscn_rwlock_reader_lock(init_data->net_rwlock);
	}
}

void zwscn_network_unlock_reader(void *net) {
	zwscn_init_p init_data = NULL;
	
	if(net == NULL) {
		return;
	}
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->net_rwlock) {
		zwscn_rwlock_reader_unlock(init_data->net_rwlock);
	}
}

void zwscn_network_lock_writer(void *net) {
	zwscn_init_p init_data = NULL;
	
	if(net == NULL) {
		return;
	}
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->net_rwlock) {
		zwscn_rwlock_writer_lock(init_data->net_rwlock);
	}
}

void zwscn_network_unlock_writer(void *net) {
	zwscn_init_p init_data = NULL;
	
	if(net == NULL) {
		return;
	}
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->net_rwlock) {
		zwscn_rwlock_writer_unlock(init_data->net_rwlock);
	}
}

void zwscn_mqueue_lock(void *net) {
	zwscn_init_p init_data = NULL;
	
	if(net == NULL) {
		return;
	}
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->mqueue_mutex) {
		zwscn_mutex_lock(init_data->mqueue_mutex);
	}
}

void zwscn_mqueue_unlock(void *net) {
	zwscn_init_p init_data = NULL;
	
	if(net == NULL) {
		return;
	}
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->mqueue_mutex) {
		zwscn_mutex_unlock(init_data->mqueue_mutex);
	}
}

void zwscn_statelog_lock(void *net) {
	zwscn_init_p init_data = NULL;
	
	if(net == NULL) {
		return;
	}
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->statelog_mutex) {
		zwscn_mutex_lock(init_data->statelog_mutex);
	}
}

void zwscn_statelog_unlock(void *net) {
	zwscn_init_p init_data = NULL;
	
	if(net == NULL) {
		return;
	}
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->statelog_mutex) {
		zwscn_mutex_unlock(init_data->statelog_mutex);
	}
}

zwscn_global_data_p zwscn_get_global_db() {
	return &zwscn_global_db;
}

zwscn_hash_t zwscn_get_network_hash() {
	zwscn_global_data_p global_db;
	
	global_db = zwscn_get_global_db();
	if(global_db && (global_db->initialized))
		return global_db->network_hash;
		
	return NULL;
}

zwscn_hash_t zwscn_get_unavailable_network_hash() {
	zwscn_global_data_p global_db;
	
	global_db = zwscn_get_global_db();
	if(global_db && (global_db->initialized))
		return global_db->unavailable_network_hash;
		
	return NULL;
}


zwscn_init_p zwscn_get_init_data(void *net) {
	zwscn_init_p init_data = NULL;
	
	//zwscn_debug("ENTER zwscn_get_init_data\n");
	if(net == NULL) {
		goto l_err;
	}
	zwscn_global_hash_lock();
	init_data = zwscn_hash_get_value(zwscn_get_network_hash(), net);
	zwscn_global_hash_unlock();
	
	//zwscn_debug("RETURN zwscn_get_init_data\n");
	return init_data;
l_err:
	//zwscn_debug("RETURN ERROR zwscn_get_init_data\n");
	return NULL;
}

zwscn_statelog_p zwscn_get_statelog_data(void *net, uint32_t category) {
	zwscn_statelog_p statelog_scene_data_ptr = NULL;
	zwscn_init_p scene_init_data;
	
	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	
	switch(category) {
		case ZWSCN_STATELOG_CATEGORY_STATUS:
			statelog_scene_data_ptr = &(scene_init_data->statelog_scene_status_data);
			break;
		case ZWSCN_STATELOG_CATEGORY_ADD:
			statelog_scene_data_ptr = &(scene_init_data->statelog_scene_add_data);
			break;
		case ZWSCN_STATELOG_CATEGORY_DELETE:
			statelog_scene_data_ptr = &(scene_init_data->statelog_scene_delete_data);
			break;
		case ZWSCN_STATELOG_CATEGORY_EXEC:
			statelog_scene_data_ptr = &(scene_init_data->statelog_scene_exec_data);
			break;
		default:
			zwscn_message_log_warning("Statelog category %d not found\n", category);
	}
	
	return statelog_scene_data_ptr;
l_err:
	return NULL;
}

int zwscn_does_net_exist(void *net) {
	int result = 0;
	zwscn_global_data_p global_db;
	
	global_db = zwscn_get_global_db();
	if((global_db == NULL) || (global_db->initialized != ZWSCN_TRUE)  || (global_db->exiting == ZWSCN_TRUE)|| (net == NULL)) {
		goto l_err;
	}
	zwscn_unavailable_network_hash_lock();
	result = zwscn_hash_contains_key(global_db->unavailable_network_hash, net);
	zwscn_unavailable_network_hash_unlock();
	if(result) { //network is in the list of unavailable networks (maybe exit is in progress)
		goto l_err;
	}
	zwscn_global_hash_lock();
	result = zwscn_hash_contains_key(zwscn_get_network_hash(), net);
	zwscn_global_hash_unlock();
	
	return result;
l_err:
	return 0;
}

//this funtion is called from zwscn_shutdown before freeing each network.
//Here we already know that global_db->exiting == ZWSCN_TRUE, so we do not care about it
//We only want to make sure that this network is not being deleted parallely by some other function.
int zwscn_does_net_exist_2(void *net) { 
	int result = 0;
	zwscn_global_data_p global_db;
	
	global_db = zwscn_get_global_db();
	if((global_db == NULL) || (global_db->initialized != ZWSCN_TRUE) || (net == NULL)) {
		goto l_err;
	}
	zwscn_unavailable_network_hash_lock();
	result = zwscn_hash_contains_key(global_db->unavailable_network_hash, net);
	zwscn_unavailable_network_hash_unlock();
	if(result) { //network is in the list of unavailable networks (maybe exit is in progress)
		goto l_err;
	}
	zwscn_global_hash_lock();
	result = zwscn_hash_contains_key(zwscn_get_network_hash(), net);
	zwscn_global_hash_unlock();
	
	return result;
l_err:
	return 0;
}

int zwscn_is_net_unavailable(void *net) {
	int result = 1; //by default all networks are unavailable
	zwscn_global_data_p global_db;
	
	global_db = zwscn_get_global_db();
	if((global_db == NULL) || (global_db->initialized != ZWSCN_TRUE)  || (global_db->exiting == ZWSCN_TRUE)|| (net == NULL)) {
		goto l_err;
	}
	zwscn_unavailable_network_hash_lock();
	result = zwscn_hash_contains_key(zwscn_get_unavailable_network_hash(), net);
	zwscn_unavailable_network_hash_unlock();
	
	return result;
l_err:
	return 1; //by default all networks are unavailable
}

uint32_t zwscn_generate_scene_id(void *net){
	zwscn_init_p scene_init_data;
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}
	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	++(scene_init_data->pool_scene_id);
	if((scene_init_data->pool_scene_id < ZWSCN_SCENE_ID_MIN) || (scene_init_data->pool_scene_id > ZWSCN_SCENE_ID_MAX)) {
		scene_init_data->pool_scene_id = ZWSCN_SCENE_ID_MIN;
	}
	
	return scene_init_data->pool_scene_id;
l_err:
	return 0; //invalid scene id
}

uint32_t zwscn_get_scene_id_pool_index(void *net) {
	zwscn_init_p scene_init_data;
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}
	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	
	return scene_init_data->pool_scene_id;
l_err:
	return 0; //invalid scene id
}

void zwscn_set_scene_id_pool_index(void *net, uint32_t index) {
	zwscn_init_p scene_init_data;
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}
	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	
	scene_init_data->pool_scene_id = index;
l_err:
	return;
}

uint32_t zwscn_generate_scene_schedule_id(void *net, uint32_t scene_id) {
	zwscn_scene_cont_p scene_container_ptr;
	
	if (!zwscn_does_net_exist(net)) {
		zwscn_message_log_critical("[Error] net=%p does not exist\n", net);
		goto l_err;
	}
	if(zwscn_find_scene(net, scene_id, &scene_container_ptr) != ZWSCN_ERR_NONE) {
		zwscn_message_log_critical("[Error] Could not find Scene id=%d\n", scene_id);
		goto l_err;
	}
//	++(scene_container_ptr->pool_schedule_id);
//	
//	zwscn_message_log_debug("Generated schedule id=%d\n", scene_container_ptr->pool_schedule_id);
//	return scene_container_ptr->pool_schedule_id;

	return zwscn_get_new_schedule_id(scene_container_ptr);
l_err:
	return 0; //invalid scene id
}

uint32_t zwscn_get_new_schedule_id(zwscn_scene_cont_p scene_container_ptr) {
	++(scene_container_ptr->pool_schedule_id);
	zwscn_message_log_debug("Generated schedule id=%d\n", scene_container_ptr->pool_schedule_id);
	
	return scene_container_ptr->pool_schedule_id;
}

ZWSCN_STATUS_T zwscn_find_thread_events(void *net, zwscn_thread_t *thread_ptr) {
	zwscn_init_p scene_init_data;

	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}

	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}

	(*thread_ptr) = scene_init_data->thread_events;

	return ZWSCN_ERR_NONE;

l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_thread_execute_request_handler(void *net, zwscn_thread_t *thread_ptr) {
	zwscn_init_p scene_init_data;

	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}

	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}

	(*thread_ptr) = scene_init_data->thread_execute_request_handler;

	return ZWSCN_ERR_NONE;

l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_thread_status_update_request_handler(void *net, zwscn_thread_t *thread_ptr) {
	zwscn_init_p scene_init_data;

	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}

	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}

	(*thread_ptr) = scene_init_data->thread_status_update_request_handler;

	return ZWSCN_ERR_NONE;

l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_thread_status_update_report_handler(void *net, zwscn_thread_t *thread_ptr) {
	zwscn_init_p scene_init_data;

	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}

	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}

	(*thread_ptr) = scene_init_data->thread_status_update_report_handler;

	return ZWSCN_ERR_NONE;

l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_thread_notification_processor(void *net, zwscn_thread_t *thread_ptr) {
	zwscn_init_p scene_init_data;

	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}

	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}

	(*thread_ptr) = scene_init_data->thread_notification_processor;

	return ZWSCN_ERR_NONE;

l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_mqueue_thread_events(void *net, zwscn_mqueue_t *mqueue_ptr) {
	zwscn_init_p scene_init_data;

	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}

	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	
	if(scene_init_data->mqueue_thread_events == NULL) {
		goto l_err;
	}

	(*mqueue_ptr) = scene_init_data->mqueue_thread_events;

	return ZWSCN_ERR_NONE;

l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_mqueue_thread_execute_request_handler(void *net, zwscn_mqueue_t *mqueue_ptr) {
	zwscn_init_p scene_init_data;

	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}

	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	
	if(scene_init_data->mqueue_thread_execute_request_handler == NULL) {
		goto l_err;
	}

	(*mqueue_ptr) = scene_init_data->mqueue_thread_execute_request_handler;

	return ZWSCN_ERR_NONE;

l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_mqueue_thread_status_update_request_handler(void *net, zwscn_mqueue_t *mqueue_ptr) {
	zwscn_init_p scene_init_data;

	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}

	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	
	if(scene_init_data->mqueue_thread_status_update_request_handler == NULL) {
		goto l_err;
	}

	(*mqueue_ptr) = scene_init_data->mqueue_thread_status_update_request_handler;

	return ZWSCN_ERR_NONE;

l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_mqueue_thread_status_update_report_handler(void *net, zwscn_mqueue_t *mqueue_ptr) {
	zwscn_init_p scene_init_data;

	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}

	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	
	if(scene_init_data->mqueue_thread_status_update_report_handler == NULL) {
		goto l_err;
	}

	(*mqueue_ptr) = scene_init_data->mqueue_thread_status_update_report_handler;

	return ZWSCN_ERR_NONE;

l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_mqueue_thread_notification_processor(void *net, zwscn_mqueue_t *mqueue_ptr) {
	zwscn_init_p scene_init_data;

	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}

	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	
	if(scene_init_data->mqueue_thread_notification_processor == NULL) {
		goto l_err;
	}

	(*mqueue_ptr) = scene_init_data->mqueue_thread_notification_processor;

	return ZWSCN_ERR_NONE;

l_err:
	return ZWSCN_ERR_GENERIC;
}


ZWSCN_STATUS_T zwscn_find_scene_list(void *net, zwscn_list_t **scene_list_ptr) {
	zwscn_init_p scene_init_data;

	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}

	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}

	(*scene_list_ptr) = &(scene_init_data->list_scenes);

	return ZWSCN_ERR_NONE;

l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_action_list(void *net, uint32_t scene_id, zwscn_list_t **action_list_ptr) {
	return zwscn_find_list_by_type(net, scene_id, ZWSCN_TYPE_ACTION, action_list_ptr);
}

ZWSCN_STATUS_T zwscn_find_schedule_list(void *net, uint32_t scene_id, zwscn_list_t **schedule_list_ptr) {
	return zwscn_find_list_by_type(net, scene_id, ZWSCN_TYPE_SCHEDULE, schedule_list_ptr);
}

ZWSCN_STATUS_T zwscn_find_event_list(void *net, uint32_t scene_id, zwscn_list_t **event_list_ptr) {
	return zwscn_find_list_by_type(net, scene_id, ZWSCN_TYPE_EVENT, event_list_ptr);
}

ZWSCN_STATUS_T zwscn_find_scene(void *net, uint32_t scene_id, zwscn_scene_cont_p *scene_ptr) {
	zwscn_list_t *scene_list;

	if((net == NULL) || (scene_id == 0) || (scene_ptr == NULL)) {
		goto l_err_args;
	}
	
	if(zwscn_find_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}

	if(((*scene_ptr) = zwscn_list_get_scene_by_id((*scene_list), scene_id)) == NULL) {
		goto l_err;
	}

	return ZWSCN_ERR_NONE;

l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_action(void *net, uint32_t scene_id, uint32_t id, zwscn_action_p *out_action) {
	zwscn_list_t *action_list;
//	zwscn_action_p action;

	if(zwscn_find_action_list(net, scene_id, &action_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}


	if(((*out_action) = zwscn_list_get_action_by_id((*action_list), id)) == NULL) {
		goto l_err;
	}
	

	return ZWSCN_ERR_NONE;

l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_schedule(void *net, uint32_t scene_id, uint32_t id, zwscn_schedule_p *out_schedule) {
	zwscn_list_t *schedule_list;
//	zwscn_schedule_p schedule;

	if(zwscn_find_schedule_list(net, scene_id, &schedule_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}


	if(((*out_schedule) = zwscn_list_get_schedule_by_id((*schedule_list), id)) == NULL) {
		goto l_err;
	}
	

	return ZWSCN_ERR_NONE;

l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_find_event(void *net, uint32_t scene_id, uint32_t id, zwscn_event_p *out_event) {
	zwscn_list_t *event_list;
//	zwscn_event_p event;

	if(zwscn_find_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}


	if(((*out_event) = zwscn_list_get_event_by_id((*event_list), id)) == NULL) {
		goto l_err;
	}
	

	return ZWSCN_ERR_NONE;

l_err:
	return ZWSCN_ERR_GENERIC;
}

static ZWSCN_STATUS_T zwscn_find_list_by_type(void *net, uint32_t scene_id, ZWSCN_MEMBERS_TYPE_T list_type, zwscn_list_t **list_ptr) {
	zwscn_list_t *scene_list;
	zwscn_scene_cont_p scene_ptr;

	if(zwscn_find_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}

	if((scene_ptr = zwscn_list_get_scene_by_id((*scene_list), scene_id)) == NULL) {
		goto l_err;
	}

	if(list_type == ZWSCN_TYPE_ACTION) {
		*(list_ptr) = &(scene_ptr->list_actions);
	} else if (list_type == ZWSCN_TYPE_SCHEDULE) {
		*(list_ptr) = &(scene_ptr->list_schedules);
	} else if (list_type == ZWSCN_TYPE_EVENT) {
		*(list_ptr) = &(scene_ptr->list_events);
	}

	return ZWSCN_ERR_NONE;

l_err:
	return ZWSCN_ERR_GENERIC;
}

uint32_t zwscn_find_last_executed_scene(void *net) {
	zwscn_init_p init_data;
	uint32_t result = 0;
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err;
	}

	if((init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	
	result = init_data->last_executed_scene_id;
	
	return result;
l_err:
	return 0;
}

ZWSCN_BOOL_T zwscn_is_last_executed_scene(void *net, zwscn_scene_cont_p scene_ptr) {
	ZWSCN_BOOL_T result = ZWSCN_FALSE;
	
	if((scene_ptr == NULL) || (scene_ptr->scene == NULL) || (scene_ptr->scene->id == 0)) {
		goto l_err_args;
	}
	
	if (zwscn_find_last_executed_scene(net) == scene_ptr->scene->id) {
		result = ZWSCN_TRUE;
	}
	
	return result;
l_err_args:
	return ZWSCN_FALSE;
}

int zwscn_get_max_scenes(void *net) {
	zwscn_init_p scene_init_data;
	int max_limit=0;
	
	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	if(scene_init_data->init_input) {
		max_limit = (scene_init_data->init_input->num_max_scenes < 0) ? ZWSCN_MAX_SCENES : scene_init_data->init_input->num_max_scenes;
	}
	
	return max_limit;
l_err:
	return 0;
}

int zwscn_get_max_actions(void *net) {
	zwscn_init_p scene_init_data;
	int max_limit=0;
	
	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	if(scene_init_data->init_input) {
		max_limit = (scene_init_data->init_input->num_max_actions_per_scene < 0) ? ZWSCN_MAX_ACTIONS : scene_init_data->init_input->num_max_actions_per_scene;
	}
	
	return max_limit;
l_err:
	return 0;
}

int zwscn_get_max_schedules(void *net) {
	zwscn_init_p scene_init_data;
	int max_limit=0;
	
	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	if(scene_init_data->init_input) {
		max_limit = (scene_init_data->init_input->num_max_schedules_per_scene < 0) ? ZWSCN_MAX_SCHEDULES : scene_init_data->init_input->num_max_schedules_per_scene;
	}
	
	return max_limit;
l_err:
	return 0;
}

int zwscn_get_max_events(void *net) {
	zwscn_init_p scene_init_data;
	int max_limit=0;
	
	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	if(scene_init_data->init_input) {
		max_limit = (scene_init_data->init_input->num_max_events_per_scene < 0) ? ZWSCN_MAX_EVENTS : scene_init_data->init_input->num_max_events_per_scene;
	}
	
	return max_limit;
l_err:
	return 0;
}

ZWSCN_BOOL_T zwscn_is_list_full_scene(void *net, zwscn_list_t scene_list) {
	int list_length=0;
	int max_limit=0;
//	zwscn_init_p scene_init_data;
	
//	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
//		goto l_err;
//	}
	list_length = zwscn_list_size(scene_list);
//	if(scene_init_data->init_input) {
//		max_limit = (scene_init_data->init_input->num_max_scenes < 0) ? ZWSCN_MAX_SCENES : scene_init_data->init_input->num_max_scenes;
//	}
	max_limit = zwscn_get_max_scenes(net);
	if(list_length < max_limit) {
		return ZWSCN_FALSE;
	}
	
	return ZWSCN_TRUE;
	
//l_err:
//	return ZWSCN_TRUE; //In case of error we assume that list is full
}

ZWSCN_BOOL_T zwscn_is_list_full_action(void *net, zwscn_list_t action_list) {
	int list_length=0;
	int max_limit=0;
//	zwscn_init_p scene_init_data;
	
//	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
//		goto l_err;
//	}
	list_length = zwscn_list_size(action_list);
//	if(scene_init_data->init_input) {
//		max_limit = (scene_init_data->init_input->num_max_actions_per_scene < 0) ? ZWSCN_MAX_ACTIONS : scene_init_data->init_input->num_max_actions_per_scene;
//	}
	max_limit = zwscn_get_max_actions(net);
	if(list_length < max_limit) {
		return ZWSCN_FALSE;
	}
	
	return ZWSCN_TRUE;
	
//l_err:
//	return ZWSCN_TRUE; //In case of error we assume that list is full
}

ZWSCN_BOOL_T zwscn_is_list_full_schedule(void *net, zwscn_list_t schedule_list) {
	int list_length=0;
	int max_limit=0;
//	zwscn_init_p scene_init_data;
	
//	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
//		goto l_err;
//	}
	list_length = zwscn_list_size(schedule_list);
//	if(scene_init_data->init_input) {
//		max_limit = (scene_init_data->init_input->num_max_schedules_per_scene < 0) ? ZWSCN_MAX_SCHEDULES : scene_init_data->init_input->num_max_schedules_per_scene;
//	}
	max_limit = zwscn_get_max_schedules(net);
	if(list_length < max_limit) {
		return ZWSCN_FALSE;
	}
	
	return ZWSCN_TRUE;
	
//l_err:
//	return ZWSCN_TRUE; //In case of error we assume that list is full
}

ZWSCN_BOOL_T zwscn_is_list_full_event(void *net, zwscn_list_t event_list) {
	int list_length=0;
	int max_limit=0;
//	zwscn_init_p scene_init_data;
	
//	if((scene_init_data = zwscn_get_init_data(net)) == NULL) {
//		goto l_err;
//	}
	list_length = zwscn_list_size(event_list);
//	if(scene_init_data->init_input) {
//		max_limit = (scene_init_data->init_input->num_max_events_per_scene < 0) ? ZWSCN_MAX_EVENTS : scene_init_data->init_input->num_max_events_per_scene;
//	}
	max_limit = zwscn_get_max_events(net);
	if(list_length < max_limit) {
		return ZWSCN_FALSE;
	}
	
	return ZWSCN_TRUE;
	
//l_err:
//	return ZWSCN_TRUE; //In case of error we assume that list is full
}

ZWSCN_STATUS_T zwscn_scene_triggers_info_update(void *net, zwscn_scene_cont_p scene_ptr) {
	ZWSCN_STATUS_T  result = ZWSCN_ERR_NONE;
	int count = 0;
	
	if((net == NULL) || (scene_ptr == NULL)) {
		goto l_err_args;
	}
	
	count = zwscn_list_size(scene_ptr->list_schedules);
	if(count > 0) {
		scene_ptr->scene->triggers |= ZWSCN_SCENE_TRIGGER_SCHEDULE;
	} else {
		scene_ptr->scene->triggers &= ~(ZWSCN_SCENE_TRIGGER_SCHEDULE);
	}
	
	count = zwscn_list_size(scene_ptr->list_events);
	if(count > 0) {
		scene_ptr->scene->triggers |= ZWSCN_SCENE_TRIGGER_EVENT;
	} else {
		scene_ptr->scene->triggers &= ~(ZWSCN_SCENE_TRIGGER_EVENT);
	}
	
	return result;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
//l_err:
//	return result;
}
