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
@file		zwscn_scene_api.c  
  
                Implementation of Scenes API public
  
@author		Arif Mohammand
 
@version	1.0 2013-12-05  Initial release
*/
#include <stdio.h>
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
#include "zwscn_statelog.h"
#include "zwscn_scene.h"
#include "zwscn_scene_events.h"
#include "zwscn_scene_schedule.h"
#include "zwscn_persistence.h"
#include "zwscn_scene_status_update.h"
#include "zwscn_notification_handler.h"
#include "zwscn_notification_processor.h"
#include "zwscn_scene_execute.h"
#include "zwscn_setup_op.h"
#include "zwscn_security_scene_api.h"
#include "zwscn_security_scene.h"
#include "zwscn_security_scene_events.h"
#include "zwscn_security_scene_notifications.h"
#include "zwscn_persistence_security_scene.h"
#include "zwscn_util_zwave_eval.h"

ZWSCN_STATUS_T zwscn_init() {
	zwscn_global_data_p global_db;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	global_db = zwscn_get_global_db();
	if(!global_db) {
		goto l_err;
	}
	if(global_db->initialized == ZWSCN_TRUE) {
		goto l_err_init;
	}
	if(global_db->exiting == ZWSCN_TRUE) {
		goto l_err_init;
	}
	
	zwscn_message_log_init();
	global_db->timer_subsystem_data = zwscn_timer_subsystem_init();
	if(global_db->timer_subsystem_data == NULL) {
		goto l_err_memory;
	}
	global_db->unavailable_network_hash_mutex = zwscn_rec_mutex_new(); //recursive mutex
	if(global_db->unavailable_network_hash_mutex == NULL) {
		goto l_err_memory;
	}
	global_db->network_hash_mutex = zwscn_rec_mutex_new(); //recursive mutex
	if(global_db->network_hash_mutex == NULL) {
		goto l_err_memory_2;
	}
	zwscn_unavailable_network_hash_lock();
	global_db->unavailable_network_hash = zwscn_hash_new();
	zwscn_unavailable_network_hash_unlock();
	zwscn_global_hash_lock();
	global_db->network_hash = zwscn_hash_new();
	zwscn_global_hash_unlock();
	zwscn_atomic_set(&(global_db->initialized), ZWSCN_TRUE); //Now the system is initialized
	
	//TODO do other init stuff
	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;
l_err:
	RETURN(ZWSCN_ERR_GENERIC);
l_err_init:
	RETURN(ZWSCN_ERR_INIT);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_memory_2:
	if(global_db && global_db->unavailable_network_hash_mutex) {
		zwscn_rec_mutex_delete(global_db->unavailable_network_hash_mutex);
		global_db->unavailable_network_hash_mutex = NULL;
	}
	RETURN(ZWSCN_ERR_MEMORY);
}

ZWSCN_STATUS_T zwscn_init_network(zwscn_init_input_p scene_init_args, zwscn_init_response_p *out_scene_init_response) {
	zwscn_init_p init_data;
	zwscn_init_response_p init_response;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if(!scene_init_args) {
		zwscn_message_log_warning("Input init_args is NULL.\n");
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("Initializing net=%p...\n", scene_init_args->net);
	if(zwscn_does_net_exist(scene_init_args->net)) {
		zwscn_message_log_warning("Network net=%p exists.\n", scene_init_args->net);
		goto l_err_init;
	}
	
	if(!(zwscn_timezone_is_valid((char *)scene_init_args->client_timezone))) {
		zwscn_message_log_warning("Input timezone is invalid for net=%p\n", scene_init_args->net);
		goto l_err_timezone;
	}
	
	init_data = (zwscn_init_p ) calloc(1, sizeof(zwscn_init_t));
	if(!init_data){
		goto l_err_memory;
	}
	init_data->init_input = (zwscn_init_input_p ) calloc(1, sizeof(zwscn_init_input_t));
	if(!(init_data->init_input)){
		goto l_err_memory_1;
	}
	//memcpy(init_data->init_input, scene_init_args, sizeof(zwscn_init_input_t));
	*(init_data->init_input) = *(scene_init_args); // C compiler in-built structure copy
	
//	init_data->net_mutex = zwscn_mutex_new();
//	if(init_data->net_mutex == NULL) {
//		goto l_err_memory_2;
//	}
	init_data->hash_device_report_cache = zwscn_hash_new();
	init_data->hash_pending_postset_poll_cb = zwscn_hash_new();
	
	init_data->net_rwlock = zwscn_rwlock_new();
	if(init_data->net_rwlock == NULL) {
		goto l_err_memory_3;
	}
	init_data->mqueue_mutex = zwscn_mutex_new();
	if(init_data->mqueue_mutex == NULL) {
		goto l_err_memory_4;
	}
	init_data->statelog_mutex = zwscn_mutex_new();
	if(init_data->statelog_mutex == NULL) {
		goto l_err_memory_5;
	}
	init_data->mutex_abort_status_update_op = zwscn_mutex_new();
	if(init_data->mutex_abort_status_update_op == NULL) {
		goto l_err_memory_6;
	}
	init_data->mutex_device_report_cache = zwscn_mutex_new();
	if(init_data->mutex_device_report_cache == NULL) {
		goto l_err_memory_7;
	}
	init_data->mutex_pending_report_status = zwscn_mutex_new();
	if(init_data->mutex_pending_report_status == NULL) {
		goto l_err_memory_8;
	}
	
	init_data->mqueue_thread_events = zwscn_mqueue_new(zwscn_free_mqueue_message);
	init_data->thread_events = zwscn_thread_new(ZWSCN_THREAD_NAME_EVENTS, zwscn_events_report_handler_thread, init_data->mqueue_thread_events);
	
	init_data->mqueue_thread_execute_request_handler = zwscn_mqueue_new(zwscn_free_mqueue_message);
	init_data->thread_execute_request_handler = zwscn_thread_new(ZWSCN_THREAD_NAME_EXECUTE_REQUEST_HANDLER, zwscn_execute_request_handler_thread, init_data->mqueue_thread_execute_request_handler);
	
	init_data->mqueue_thread_status_update_request_handler = zwscn_mqueue_new(zwscn_free_mqueue_message);
	init_data->thread_status_update_request_handler = zwscn_thread_new(ZWSCN_THREAD_NAME_STATUS_UPDATE_REQUEST_HANDLER, zwscn_status_update_request_handler_thread, init_data->mqueue_thread_status_update_request_handler);
	
	init_data->mqueue_thread_status_update_report_handler = zwscn_mqueue_new(zwscn_free_mqueue_message);
	init_data->thread_status_update_report_handler = zwscn_thread_new(ZWSCN_THREAD_NAME_STATUS_UPDATE_REPORT_HANDLER, zwscn_status_update_report_handler_thread, init_data->mqueue_thread_status_update_report_handler);

	init_data->mqueue_thread_notification_processor = zwscn_mqueue_new(zwscn_free_mqueue_message);
	init_data->thread_notification_processor = zwscn_thread_new(ZWSCN_THREAD_NAME_NOTIFICATION_PROCESSOR, zwscn_notification_processor_thread, init_data->mqueue_thread_notification_processor);
	
	init_data->mqueue_thread_security_events = zwscn_mqueue_new(zwscn_free_mqueue_message);
	init_data->thread_security_events = zwscn_thread_new(ZWSCN_THREAD_NAME_SECURITY_EVENTS, zwscn_security_events_report_handler_thread, init_data->mqueue_thread_security_events);
	
	init_data->mqueue_thread_security_scene_notification_request_handler = zwscn_mqueue_new(zwscn_free_mqueue_message);
	init_data->thread_security_scene_notification_request_handler = zwscn_thread_new(ZWSCN_THREAD_NAME_SECURITY_SECENE_NOTIFICATIONS, zwscn_security_scene_notification_request_handler_thread, init_data->mqueue_thread_security_scene_notification_request_handler);
	
	zwscn_unavailable_network_hash_lock();
	zwscn_hash_remove(zwscn_get_unavailable_network_hash(), init_data->init_input->net);
	zwscn_unavailable_network_hash_unlock();
	zwscn_global_hash_lock();
	zwscn_hash_insert(zwscn_get_network_hash(), init_data->init_input->net, init_data);
	zwscn_global_hash_unlock();
	
	if(zwscn_persistence_load_configuration(init_data->init_input->net) != ZWSCN_ERR_NONE) {
		//do nothing
	}
	if(zwscn_persistence_load_all_scenes(init_data->init_input->net, &(init_data->list_scenes)) != ZWSCN_ERR_NONE) {
		goto l_err_persistence; //At this point network is added but the persistent data could not be loaded, so the caller should call zwscn_exit_network(), if needed
	}
	if(zwscn_persistence_load_all_security_scenes(init_data->init_input->net, &(init_data->list_security_scenes)) != ZWSCN_ERR_NONE) {
		goto l_err_persistence; //At this point network is added but the persistent data could not be loaded, so the caller should call zwscn_exit_network(), if needed
	}
	
	init_response = calloc(1, sizeof(zwscn_init_response_t));
	if(init_response == NULL) {
		goto l_err_init_response; //At this point network is added but the system ran out of memory, so the caller should call zwscn_exit_network() 
	}
	init_response->zwscn_function_network_notify_handler = zwscn_network_notification_handler;
	init_response->zwscn_function_node_notify_handler = zwscn_node_notification_handler;
	init_response->zwscn_function_report_notify_handler = zwscn_device_notification_handler;
	
	(*out_scene_init_response) = init_response;

	ZWSCN_MESSAGE_LOG_DEBUG("Initializing net=%p. DONE!\n", scene_init_args->net);
	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_init:
	RETURN(ZWSCN_ERR_INIT);
l_err_timezone:
	RETURN(ZWSCN_ERR_TIMEZONE);
l_err_memory_8:
	zwscn_mutex_delete(init_data->mutex_device_report_cache);
l_err_memory_7:
	zwscn_mutex_delete(init_data->mutex_abort_status_update_op);
l_err_memory_6:
	zwscn_mutex_delete(init_data->statelog_mutex);
l_err_memory_5:
	zwscn_mutex_delete(init_data->mqueue_mutex);
l_err_memory_4:
	zwscn_mutex_delete(init_data->net_rwlock);
//	zwscn_mutex_delete(init_data->net_mutex);
l_err_memory_3:
//	zwscn_mutex_delete(init_data->net_mutex);
//l_err_memory_2:
//	free(init_data->init_input);
l_err_memory_1:
	free(init_data);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_init_response:
	RETURN(ZWSCN_ERR_MEMORY_INIT_RESPONSE);
l_err_persistence:
	RETURN(ZWSCN_ERR_PERSISTENT_DATA);
}

ZWSCN_STATUS_T zwscn_exit_network(void *net) {
	zwscn_init_p init_data_ptr = NULL;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net) {
		zwscn_message_log_warning("Input net is NULL\n");
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("Exiting net=%p...\n", net);
	if (!zwscn_does_net_exist(net)) {
		zwscn_message_log_warning("Network net=%p does not exist in the list initialized networks\n", net);
		goto l_err_exit;
	}
	zwscn_unavailable_network_hash_lock();
	zwscn_hash_add(zwscn_get_unavailable_network_hash(), net);
	zwscn_unavailable_network_hash_unlock();

	init_data_ptr = zwscn_get_init_data(net);
	if(init_data_ptr == NULL) {
		goto l_err_exit;
	}
	zwscn_free_net(init_data_ptr); //This function removes all the memory associated by this net and its children
	zwscn_global_hash_lock();
	if(!zwscn_hash_remove(zwscn_get_network_hash(), net)) { //If value is removed successfully, TRUE is returned
		goto l_err;
	}
	zwscn_global_hash_unlock();
	
	zwscn_unavailable_network_hash_lock();
	zwscn_hash_remove(zwscn_get_unavailable_network_hash(), net);
	zwscn_unavailable_network_hash_unlock();

	ZWSCN_MESSAGE_LOG_DEBUG("Exiting net=%p. DONE!\n", net);
	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_exit:
	RETURN(ZWSCN_ERR_GENERIC);
l_err:
	zwscn_global_hash_unlock();
	RETURN(ZWSCN_ERR_GENERIC);
}

ZWSCN_STATUS_T zwscn_shutdown() {
	zwscn_global_data_p global_db;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	global_db = zwscn_get_global_db();
	if(!global_db) {
		goto l_err;
	}
	if(global_db->initialized != ZWSCN_TRUE) {
		goto l_err_init;
	}
	if(global_db->exiting == ZWSCN_TRUE) {
		goto l_err_init;
	}
	zwscn_atomic_set(&(global_db->exiting), ZWSCN_TRUE); //Now the system shutdown is in progress
	
	zwscn_hash_foreach_remove(zwscn_get_network_hash(), zwscn_free_net_foreach_callback, NULL);
	
	zwscn_atomic_set(&(global_db->initialized), ZWSCN_FALSE); //Now the system is un-initialized
	
	zwscn_global_hash_lock();
	zwscn_hash_remove_all(global_db->network_hash); //Remove any remaining entries
	zwscn_hash_destroy(global_db->network_hash); //Free memory allocated for hash
	zwscn_global_hash_unlock();
	
	zwscn_unavailable_network_hash_lock();
	zwscn_hash_remove_all(global_db->unavailable_network_hash); //Remove any remaining entries
	zwscn_hash_destroy(global_db->unavailable_network_hash); //Free memory allocated for hash
	zwscn_unavailable_network_hash_unlock();
	
	if(global_db && global_db->network_hash_mutex) {
		zwscn_rec_mutex_delete(global_db->network_hash_mutex);
		global_db->network_hash_mutex = NULL;
	}
	if(global_db && global_db->unavailable_network_hash_mutex) {
		zwscn_rec_mutex_delete(global_db->unavailable_network_hash_mutex);
		global_db->unavailable_network_hash_mutex = NULL;
	}
	zwscn_timer_subsystem_exit(global_db->timer_subsystem_data);

	//TODO perform other cleanup before complete shutdown
	zwscn_atomic_set(&(global_db->exiting), ZWSCN_FALSE); //Now the system shutdown is finished

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err:
	RETURN(ZWSCN_ERR_GENERIC);
l_err_init:
	RETURN(ZWSCN_ERR_INIT);
}

ZWSCN_STATUS_T zwscn_add_scene(void *net, zwscn_scene_p scene, uint32_t *out_id) {
	zwscn_list_t *scene_list;
	zwscn_scene_cont_p scene_container_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if ((net == NULL) || (scene == NULL) || (out_id == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_name=%s\n", net, scene->name);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	scene_container_ptr = (zwscn_scene_cont_p) calloc(1, sizeof(zwscn_scene_cont_t));
	if(scene_container_ptr == NULL) {
		goto l_err_memory;
	}
	scene_container_ptr->net = net;
	scene_container_ptr->mutex_status_update_timer_info = zwscn_mutex_new();
	if(scene_container_ptr->mutex_status_update_timer_info == NULL) {
		goto l_err_memory_mutex;
	}
	scene_container_ptr->hash_status_update_timer_info = zwscn_hash_new();
	scene_container_ptr->scene = (zwscn_scene_p) calloc(1, sizeof(zwscn_scene_t));
	if(scene_container_ptr->scene == NULL) {
		goto l_err_memory_1;
	}
	*(scene_container_ptr->scene) = *(scene); // C compiler in-built structure copy
	
	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	zwscn_set_statelog_start_op_add(net, 0, ZWSCN_STATELOG_TRIGGER_MANUAL);
	if(zwscn_find_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	if(zwscn_is_list_full_scene(net, (*scene_list))) {
		goto l_err_list_full;
	}
	if((scene_container_ptr->scene->id = zwscn_generate_scene_id(net)) == 0) {
		goto l_err;
	}
	if(((*scene_list) = zwscn_list_add((*scene_list), scene_container_ptr)) == NULL) {
		goto l_err;
	}
	if(zwscn_persistence_save_scene(net, scene_container_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_after_add;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_name=%s returned id=%u\n", net, scene->name, scene_container_ptr->scene->id);
	(*out_id) = scene_container_ptr->scene->id;
	zwscn_set_statelog_end_op_add(net, scene_container_ptr->scene->id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	
	
	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory_1:
	zwscn_hash_destroy(scene_container_ptr->hash_status_update_timer_info);
	zwscn_mutex_delete(scene_container_ptr->mutex_status_update_timer_info);
l_err_memory_mutex:
	free(scene_container_ptr);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_list_full:
	zwscn_network_unlock_writer(net);
	zwscn_hash_destroy(scene_container_ptr->hash_status_update_timer_info);
	zwscn_mutex_delete(scene_container_ptr->mutex_status_update_timer_info);
	free(scene_container_ptr->scene);
	free(scene_container_ptr);
	RETURN(ZWSCN_ERR_LIST_FULL);
l_err:
	zwscn_set_statelog_end_op_add(net, 0, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	zwscn_hash_destroy(scene_container_ptr->hash_status_update_timer_info);
	zwscn_mutex_delete(scene_container_ptr->mutex_status_update_timer_info);
	free(scene_container_ptr->scene);
	free(scene_container_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_after_add:
	zwscn_set_statelog_end_op_add(net, 0, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_add_action(void *net, uint32_t scene_id, zwscn_action_p action, uint32_t *out_id) {
	zwscn_list_t *action_list;
	zwscn_action_p action_ptr;
	zwscn_action_p action_ptr_temp;
	zwscn_event_p event_ptr_temp;
	uint8_t node_id, ep_id;
	uint16_t cls_id;
	zwscn_init_p init_data;
	int result=0;
	zwscn_scene_cont_p scene_ptr = NULL;
	ZWSCN_BOOL_T is_device_args_valid = ZWSCN_FALSE;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if ((net == NULL) || (scene_id == 0) || (action == NULL) || (out_id == NULL)) {
		goto l_err_args;
	}
	if((action->device_args == NULL) && (action->device_args_size > 0)) {
		zwscn_message_log_warning("zwscn_add_action: device_args_size(%u) is not zero while device_args is NULL\n", action->device_args_size);
		goto l_err_args;
	}
	if(action->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
		zwscn_message_log_warning("zwscn_add_action: device_args_size(%u) is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%u)\n", action->device_args_size, ZWSCN_DEVICE_ARGS_MAX_SIZE);
		goto l_err_args;
	}
	if((zwscn_eval_device_args(action->device_args, action->id, action->cmd, &is_device_args_valid) != ZWSCN_ERR_NONE) || (is_device_args_valid != ZWSCN_TRUE)) {
		zwscn_message_log_warning("zwscn_add_action: device_args are not valid\n");
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	action_ptr = (zwscn_action_p) calloc(1, sizeof(zwscn_action_t));
	if(action_ptr == NULL) {
		goto l_err_memory;
	}
	*(action_ptr) = *(action); // C compiler in-built structure copy
	if(action->device_args && (action->device_args_size != 0)) {
		action_ptr->device_args = calloc(1, action->device_args_size);
		if(!action_ptr->device_args) {
			goto l_err_memory_2;
		}
		memcpy(action_ptr->device_args, action->device_args, action->device_args_size);
	} else {
		action_ptr->device_args = NULL;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_action(net, scene_id, action->id, &action_ptr_temp) == ZWSCN_ERR_NONE) { //Action exists with the same id
		goto l_err_duplicate_id_action;
	}
#if ZWSCN_CHECK_DEVICE_INTF_IN_ACTIONS_AND_EVENSTS
	if(zwscn_find_event(net, scene_id, action->id, &event_ptr_temp) == ZWSCN_ERR_NONE) { //Event exists with the same id
		goto l_err_duplicate_id_event;
	}
#endif
	node_id = zwscn_util_combo_id_to_node_id(action->id);
	ep_id = zwscn_util_combo_id_to_ep_id(action->id);
	cls_id = zwscn_util_combo_id_to_cls_id(action->id);
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->init_input && init_data->init_input->zwscn_fn_is_valid_handler) {
		result = init_data->init_input->zwscn_fn_is_valid_handler(net, ZWSCN_CHECK_NODE_EP_CLS, node_id, ep_id, cls_id);
		zwscn_message_log_debug("[3] zwscn_fn_is_valid_handler returned %d for node=%d ep=%d cls=%d\n", result, node_id, ep_id, cls_id);
		if(!result) {
			goto l_err_device_invalid;
		}
	} else {
		zwscn_message_log_warning("Function zwscn_fn_is_valid_handler is NULL for net=%p Scene_id=%d\n", net, scene_id);
		goto l_err_callback_null;
	}
	zwscn_set_statelog_start_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	if(zwscn_find_action_list(net, scene_id, &action_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	if(zwscn_is_list_full_action(net, (*action_list))) {
		goto l_err_list_full;
	}
	if(((*action_list) = zwscn_list_add((*action_list), action_ptr)) == NULL) {
		goto l_err;
	}
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_after_add;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	if(zwscn_persistence_save_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_after_add;
	}
	if(zwscn_setup_op_request(net, ZWSCN_TYPE_ACTION, action_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_after_add;
	}
////	zwscn_set_statelog_start_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM); 
//	if(zwscn_status_update_scene_req_start(net, scene_ptr, ZWSCN_TRUE) != ZWSCN_ERR_NONE) {
//		goto l_err_status;
//	}
////	zwscn_set_statelog_end_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM); 
	result = zwscn_status_update_request_message_send(net, scene_id, action_ptr->id, ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_SCENE_ON_CHANGE);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("(%d) retuned by zwscn_status_update_request_message_send\n", result);
		//goto l_err_status; //Ignore as this is not a fatal error
	}
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	
	(*out_id) = action->id;

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_memory_2:
	free(action_ptr);
	RETURN(ZWSCN_ERR_MEMORY);
l_err_duplicate_id_action:
	zwscn_network_unlock_writer(net);
	free(action_ptr->device_args);
	free(action_ptr);
	RETURN(ZWSCN_ERR_ACTION_EXISTS);
l_err_duplicate_id_event:
	zwscn_network_unlock_writer(net);
	free(action_ptr->device_args);
	free(action_ptr);
	RETURN(ZWSCN_ERR_EVENT_EXISTS);
l_err_list_full:
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	free(action_ptr->device_args);
	free(action_ptr);
	zwscn_message_log_critical("l_err_list_full\n");
	RETURN(ZWSCN_ERR_LIST_FULL);
l_err:
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	free(action_ptr->device_args);
	free(action_ptr);
	zwscn_message_log_critical("l_err\n");
	RETURN(ZWSCN_ERR_GENERIC);
//l_err_status:
////	zwscn_set_statelog_end_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
//	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
//	zwscn_network_unlock_writer(net);
//	//free(action_ptr->device_args);
//	//free(action_ptr);
//	RETURN(ZWSCN_ERR_GENERIC);
l_err_device_invalid:
	zwscn_network_unlock_writer(net);
	free(action_ptr->device_args);
	free(action_ptr);
	RETURN(ZWSCN_ERR_INVALID_DEVICE);
l_err_callback_null:
	zwscn_network_unlock_writer(net);
	free(action_ptr->device_args);
	free(action_ptr);
	RETURN(ZWSCN_ERR_CALLBACK_NULL);
l_err_after_add:
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	zwscn_message_log_critical("l_err_after_add\n");
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_add_schedule(void *net, uint32_t scene_id, zwscn_schedule_p schedule, uint32_t *out_id) {
	zwscn_list_t *schedule_list;
	zwscn_schedule_p schedule_ptr;
	zwscn_scene_cont_p scene_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if ((net == NULL) || (schedule == NULL) || (scene_id == 0) || (out_id == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	if(zwscn_schedule_is_data_valid(schedule) == ZWSCN_FALSE) {
		zwscn_message_log_warning("Schedule value for day bitmask(0x%02x) or hour(%d) or minute(%d) is invalid\n", schedule->day, schedule->hour, schedule->minute);
		goto l_err_args;
	}
	
	schedule_ptr = (zwscn_schedule_p) calloc(1, sizeof(zwscn_schedule_t));
	if(schedule_ptr == NULL) {
		goto l_err_memory;
	}
	*(schedule_ptr) = *(schedule); // C compiler in-built structure copy
	schedule_ptr->timer_id = 0;				//Reset private data value
	schedule_ptr->timer_user_data = NULL;	//Reset private data value
	
	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	zwscn_set_statelog_start_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	if(zwscn_find_schedule_list(net, scene_id, &schedule_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	if(zwscn_is_list_full_schedule(net, (*schedule_list))) {
		goto l_err_list_full;
	}
	if((schedule_ptr->id = zwscn_generate_scene_schedule_id(net, scene_id)) == 0) {
		goto l_err;
	}
	if(((*schedule_list) = zwscn_list_add((*schedule_list), schedule_ptr)) == NULL) {
		goto l_err;
	}
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_persistence;
	}
	if(zwscn_persistence_save_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_persistence;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	if(zwscn_schedule_handle_update(net, scene_ptr, schedule_ptr->id, ZWSCN_TRUE) != ZWSCN_ERR_NONE) { //ZWSCN_TRUE is for checking the schedule from today
		goto l_err_schedule;
	}
	(*out_id) = schedule_ptr->id;
	zwscn_scene_triggers_info_update(net, scene_ptr);
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	
	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_list_full:
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	free(schedule_ptr);
	RETURN(ZWSCN_ERR_LIST_FULL);
l_err:
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	free(schedule_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_persistence:
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_schedule:
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_add_event(void *net, uint32_t scene_id, zwscn_event_p event, uint32_t *out_id) {
	zwscn_list_t *event_list;
	zwscn_event_p event_ptr;
	zwscn_action_p action_ptr_temp;
	zwscn_event_p event_ptr_temp;
	uint8_t node_id, ep_id;
	uint16_t cls_id;
	zwscn_init_p init_data;
	zwscn_scene_cont_p scene_ptr;
	int result=0;
	ZWSCN_BOOL_T is_device_args_valid = ZWSCN_FALSE;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if ((net == NULL) || (event == NULL) || (scene_id == 0) || (out_id == NULL)) {
		goto l_err_args;
	}
	if((event->device_args == NULL) && (event->device_args_size > 0)) {
		zwscn_message_log_warning("zwscn_add_event: device_args_size(%u) is not zero while device_args is NULL\n", event->device_args_size);
		goto l_err_args;
	}
	if(event->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
		zwscn_message_log_warning("zwscn_add_event: device_args_size(%u) is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%u)\n", event->device_args_size, ZWSCN_DEVICE_ARGS_MAX_SIZE);
		goto l_err_args;
	}
	if((zwscn_eval_device_args(event->device_args, event->id, event->cmd, &is_device_args_valid) != ZWSCN_ERR_NONE) || (is_device_args_valid != ZWSCN_TRUE)) {
		zwscn_message_log_warning("zwscn_add_event: device_args are not valid\n");
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	event_ptr = (zwscn_event_p) calloc(1, sizeof(zwscn_event_t));
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
	if(zwscn_find_event(net, scene_id, event->id, &event_ptr_temp) == ZWSCN_ERR_NONE) { //Event exists with the same id
		goto l_err_duplicate_id_event;
	}
#if ZWSCN_CHECK_DEVICE_INTF_IN_ACTIONS_AND_EVENSTS
	if(zwscn_find_action(net, scene_id, event->id, &action_ptr_temp) == ZWSCN_ERR_NONE) { //Action exists with the same id
		goto l_err_duplicate_id_action;
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
	zwscn_set_statelog_start_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	if(zwscn_find_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	if(zwscn_is_list_full_event(net, (*event_list))) {
		goto l_err_list_full;
	}

	if(((*event_list) = zwscn_list_add((*event_list), event_ptr)) == NULL) {
		goto l_err;
	}
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_after_add;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	if(zwscn_persistence_save_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_after_add;
	}
	zwscn_scene_triggers_info_update(net, scene_ptr);
	if(zwscn_setup_op_request(net, ZWSCN_TYPE_EVENT, event_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_after_add;
	}
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
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
l_err_duplicate_id_action:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_ACTION_EXISTS);
l_err_duplicate_id_event:
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_EVENT_EXISTS);
l_err_list_full:
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	free(event_ptr->device_args);
	free(event_ptr);
	RETURN(ZWSCN_ERR_LIST_FULL);
l_err:
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
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
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_scene(void *net, uint32_t scene_id) {
	zwscn_list_t *scene_list;
	zwscn_scene_cont_p scene_ptr;

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
	zwscn_set_statelog_start_op_del(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	if(zwscn_find_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	scene_ptr = zwscn_list_get_scene_by_id((*scene_list), scene_id);
	if(scene_ptr == NULL) {
		goto l_err_invalid_id;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	if(zwscn_persistence_delete_scene(net, scene_id) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	(*scene_list) = zwscn_list_remove((*scene_list), scene_ptr);
	//zwscn_status_update_remove_pending_reqs_on_scene_delete(net, scene_ptr); //Not needed actions level deletion should take care of it
	zwscn_free_scene(scene_ptr);
	zwscn_set_statelog_end_op_del(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	
	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_set_statelog_end_op_del(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_invalid_id:
	zwscn_set_statelog_end_op_del(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_action(void *net, uint32_t scene_id, uint32_t action_id) {
	zwscn_list_t *action_list;
	zwscn_action_p action_ptr;
	zwscn_scene_cont_p scene_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d, action_id=0x%x\n", net, scene_id, action_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	zwscn_set_statelog_start_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	if(zwscn_find_action_list(net, scene_id, &action_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	action_ptr = zwscn_list_get_action_by_id((*action_list), action_id);
	if(action_ptr == NULL) {
		goto l_err_invalid_id;
	}
	(*action_list) = zwscn_list_remove((*action_list), action_ptr);
	zwscn_status_update_remove_pending_reqs_on_action_delete(net, scene_ptr, action_ptr);
	zwscn_free_action(action_ptr);
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s action_id=0x%x\n", net, scene_id, scene_ptr->scene->name, action_id);
	if(zwscn_persistence_save_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_invalid_id:
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_schedule(void *net, uint32_t scene_id, uint32_t schedule_id) {
	zwscn_list_t *schedule_list;
	zwscn_schedule_p schedule_ptr;
	zwscn_scene_cont_p scene_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d schedule_id=%d\n", net, scene_id, schedule_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	zwscn_set_statelog_start_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	if(zwscn_find_schedule_list(net, scene_id, &schedule_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	schedule_ptr = zwscn_list_get_schedule_by_id((*schedule_list), schedule_id);
	if(schedule_ptr == NULL) {
		goto l_err_invalid_id;
	}
	(*schedule_list) = zwscn_list_remove((*schedule_list), schedule_ptr);
	zwscn_free_schedule(schedule_ptr);
	
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_scene_triggers_info_update(net, scene_ptr);
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s schedule_id=%d\n", net, scene_id, scene_ptr->scene->name, schedule_id);
	if(zwscn_persistence_save_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_invalid_id:
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_event(void *net, uint32_t scene_id, uint32_t event_id) {
	zwscn_list_t *event_list;
	zwscn_event_p event_ptr;
	zwscn_scene_cont_p scene_ptr;

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
	zwscn_set_statelog_start_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	if(zwscn_find_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	event_ptr = zwscn_list_get_event_by_id((*event_list), event_id);
	if(event_ptr == NULL) {
		goto l_err_invalid_id;
	}
	(*event_list) = zwscn_list_remove((*event_list), event_ptr);
	zwscn_free_event(event_ptr);
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_scene_triggers_info_update(net, scene_ptr);
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s event_id=0x%x\n", net, scene_id, scene_ptr->scene->name, event_id);
	if(zwscn_persistence_save_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_invalid_id:
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_scene_by_index(void *net, uint32_t index) {
	zwscn_list_t *scene_list;
	zwscn_scene_cont_p scene_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene index=%d\n", net, index);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	zwscn_set_statelog_start_op_del(net, 0, ZWSCN_STATELOG_TRIGGER_MANUAL);
	if(zwscn_find_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	scene_ptr = zwscn_list_get_by_index((*scene_list), index);
	if(scene_ptr == NULL) {
		goto l_err_invalid_index;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene index=%d scene_id=%d scene_name=%s\n", net, index, scene_ptr->scene->id, scene_ptr->scene->name);
	if(zwscn_persistence_delete_scene(net, scene_ptr->scene->id) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	(*scene_list) = zwscn_list_remove((*scene_list), scene_ptr);
	//zwscn_status_update_remove_pending_reqs_on_scene_delete(net, scene_ptr); //Not needed actions level deletion should take care of it
	zwscn_free_scene(scene_ptr);
	zwscn_set_statelog_end_op_del(net, 0, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	
	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_set_statelog_end_op_del(net, 0, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_invalid_index:
	zwscn_set_statelog_end_op_del(net, 0, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_action_by_index(void *net, uint32_t scene_id, uint32_t index) {
	zwscn_list_t *action_list;
	zwscn_action_p action_ptr;
	zwscn_scene_cont_p scene_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d action index=%d\n", net, scene_id, index);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	zwscn_set_statelog_start_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	if(zwscn_find_action_list(net, scene_id, &action_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	action_ptr = zwscn_list_get_by_index((*action_list), index);
	if(action_ptr == NULL) {
		goto l_err_invalid_index;
	}
	(*action_list) = zwscn_list_remove((*action_list), action_ptr);
	zwscn_status_update_remove_pending_reqs_on_action_delete(net, scene_ptr, action_ptr);
	zwscn_free_action(action_ptr);
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s action index=%d\n", net, scene_id, scene_ptr->scene->name, index);
	if(zwscn_persistence_save_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_invalid_index:
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_schedule_by_index(void *net, uint32_t scene_id, uint32_t index) {
	zwscn_list_t *schedule_list;
	zwscn_schedule_p schedule_ptr;
	zwscn_scene_cont_p scene_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d schedule index=%d\n", net, scene_id, index);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	zwscn_set_statelog_start_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	if(zwscn_find_schedule_list(net, scene_id, &schedule_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	schedule_ptr = zwscn_list_get_by_index((*schedule_list), index);
	if(schedule_ptr == NULL) {
		goto l_err_invalid_index;
	}
	(*schedule_list) = zwscn_list_remove((*schedule_list), schedule_ptr);
	zwscn_free_schedule(schedule_ptr);
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_scene_triggers_info_update(net, scene_ptr);
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s schedule index=%d\n", net, scene_id, scene_ptr->scene->name, index);
	if(zwscn_persistence_save_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_invalid_index:
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_event_by_index(void *net, uint32_t scene_id, uint32_t index) {
	zwscn_list_t *event_list;
	zwscn_event_p event_ptr;
	zwscn_scene_cont_p scene_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d event index=%d\n", net, scene_id, index);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	zwscn_set_statelog_start_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	if(zwscn_find_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	event_ptr = zwscn_list_get_by_index((*event_list), index);
	if(event_ptr == NULL) {
		goto l_err_invalid_index;
	}
	(*event_list) = zwscn_list_remove((*event_list), event_ptr);
	zwscn_free_event(event_ptr);
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_scene_triggers_info_update(net, scene_ptr);
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s event index=%d\n", net, scene_id, scene_ptr->scene->name, index);
	if(zwscn_persistence_save_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_invalid_index:
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_all_scenes(void *net) {
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
	zwscn_set_statelog_start_op_del(net, 0, ZWSCN_STATELOG_TRIGGER_MANUAL);
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		goto l_err;
	}
	
	if(init_data->list_scenes != NULL) {
		scene_list = &(init_data->list_scenes);
		if(zwscn_persistence_delete_all_scenes(net, (*scene_list)) != ZWSCN_ERR_NONE) {
			//goto l_err; //Don't exit as we have to free the next list
		}
		zwscn_list_free_scene_list(scene_list); //this deletes and free all the member items and their child nodes
	}
	if(init_data->list_scenes_invalid != NULL) {
		scene_list = &(init_data->list_scenes_invalid);
		if(zwscn_persistence_delete_all_scenes(net, (*scene_list)) != ZWSCN_ERR_NONE) {
			//goto l_err; //Don't exit as we have to free the list
		}
		zwscn_list_free_scene_list(scene_list); //this deletes and free all the member items and their child nodes
	}
	
	zwscn_set_statelog_end_op_del(net, 0, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	
	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_set_statelog_end_op_del(net, 0, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_all_actions(void *net, uint32_t scene_id) {
//	zwscn_list_t *action_list;
	zwscn_scene_cont_p scene_ptr;

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
	zwscn_set_statelog_start_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	if(scene_ptr->list_actions != NULL) {
		zwscn_status_update_remove_pending_reqs_on_action_delete_all(net, scene_ptr);
		zwscn_list_free_action_list(&(scene_ptr->list_actions)); //this deletes and free all the member items and their child nodes
	}
	if(scene_ptr->list_actions_invalid != NULL) {
		zwscn_list_free_action_list(&(scene_ptr->list_actions_invalid)); //this deletes and free all the member items and their child nodes
	}
	if(zwscn_persistence_save_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_all_schedules(void *net, uint32_t scene_id) {
	zwscn_list_t *schedule_list;
	zwscn_scene_cont_p scene_ptr;

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
	zwscn_set_statelog_start_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	if(zwscn_find_schedule_list(net, scene_id, &schedule_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	if(schedule_list != NULL) {
		zwscn_list_free_schedule_list(schedule_list); //this deletes and free all the member items and their child nodes
	}
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_scene_triggers_info_update(net, scene_ptr);
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	if(zwscn_persistence_save_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_del_all_events(void *net, uint32_t scene_id) {
//	zwscn_list_t *event_list;
	zwscn_scene_cont_p scene_ptr;
	
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
	zwscn_set_statelog_start_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	if(scene_ptr->list_events != NULL) {
		zwscn_list_free_event_list(&(scene_ptr->list_events)); //this deletes and free all the member items and their child nodes
	}
	if(scene_ptr->list_events_invalid != NULL) {
		zwscn_list_free_event_list(&(scene_ptr->list_events_invalid)); //this deletes and free all the member items and their child nodes
	}
	if(zwscn_persistence_save_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_scene_triggers_info_update(net, scene_ptr);
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_set_statelog_end_op_del_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_get_scene(void *net, uint32_t scene_id, zwscn_scene_p *out_scene) {
	zwscn_scene_cont_p scene_ptr;
	zwscn_scene_p output_scene;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (out_scene == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	output_scene = (zwscn_scene_p) calloc(1, sizeof(zwscn_scene_t));
	if(!output_scene) {
		goto l_err_memory;
	}
	
	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	scene_ptr->scene->flag_updating = (scene_ptr->pending_report_reqs == 0) ? 0 : 1; //set the flag based on the condition whether we are waiting for any reports
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s flag_updating=%u\n", net, scene_id, scene_ptr->scene->name, scene_ptr->scene->flag_updating);
	*(output_scene) = *(scene_ptr->scene); // C compiler in-built structure copy
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

ZWSCN_STATUS_T zwscn_get_action(void *net, uint32_t scene_id, uint32_t action_id, zwscn_action_p *out_scene_action) {
//	zwscn_list_t action_list;
	zwscn_action_p action_ptr;
	zwscn_action_p output_action_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (action_id == 0) || (out_scene_action == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d action_id=0x%x\n", net, scene_id, action_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	output_action_ptr = (zwscn_action_p) calloc(1, sizeof(zwscn_action_t));
	if(!output_action_ptr) {
		goto l_err_memory;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_action(net, scene_id, action_id, &action_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	*(output_action_ptr) = *(action_ptr); // C compiler in-built structure copy
	if((action_ptr->device_args != NULL) && (action_ptr->device_args_size != 0)) {
		output_action_ptr->device_args = malloc(action_ptr->device_args_size);
		if(output_action_ptr->device_args == NULL) {
			goto l_err_memory_2;
		}
		memcpy(output_action_ptr->device_args, action_ptr->device_args, action_ptr->device_args_size);
	} else {
		output_action_ptr->device_args = NULL;
		output_action_ptr->device_args_size = 0;
	}
	zwscn_network_unlock_reader(net);
	
	*(out_scene_action) = output_action_ptr; //NOTE: this must be freed by the application 

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_memory_2:
	zwscn_network_unlock_reader(net);
	free(output_action_ptr);
	RETURN(ZWSCN_ERR_MEMORY);
l_err:
	zwscn_network_unlock_reader(net);
	free(output_action_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_get_schedule(void *net, uint32_t scene_id, uint32_t schedule_id, zwscn_schedule_p *out_scene_schedule) {
//	zwscn_list_t schedule_list;
	zwscn_schedule_p schedule_ptr;
	zwscn_schedule_p output_schedule_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (schedule_id == 0) || (out_scene_schedule == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d schedule_id=%d\n", net, scene_id, schedule_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	output_schedule_ptr = (zwscn_schedule_p) calloc(1, sizeof(zwscn_schedule_t));
	if(!output_schedule_ptr) {
		goto l_err_memory;
	}
	
	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_schedule(net, scene_id, schedule_id, &schedule_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}

	*(output_schedule_ptr) = *(schedule_ptr); // C compiler in-built structure copy
	output_schedule_ptr->timer_id = 0;				//Reset private data value
	output_schedule_ptr->timer_user_data = NULL;	//Reset private data value
	zwscn_network_unlock_reader(net);
	
	*(out_scene_schedule) = output_schedule_ptr; //NOTE: this must be freed by the application 

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err:
	zwscn_network_unlock_reader(net);
	free(output_schedule_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_get_event(void *net, uint32_t scene_id, uint32_t event_id, zwscn_event_p *out_scene_event) {
//	zwscn_list_t event_list;
	zwscn_event_p event_ptr;
	zwscn_event_p output_event_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (event_id == 0) || (out_scene_event == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d event_id=0x%x\n", net, scene_id, event_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	output_event_ptr = (zwscn_event_p) calloc(1, sizeof(zwscn_event_t));
	if(!output_event_ptr) {
		goto l_err_memory;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_event(net, scene_id, event_id, &event_ptr) != ZWSCN_ERR_NONE) {
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

ZWSCN_STATUS_T zwscn_get_scene_by_index(void *net, uint32_t index, zwscn_scene_p *out_scene) {
	zwscn_list_t *scene_list;
	zwscn_scene_cont_p scene_ptr;
	zwscn_scene_p output_scene;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if ((!net) || (out_scene == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene index=%d\n", net, index);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	output_scene = (zwscn_scene_p) calloc(1, sizeof(zwscn_scene_t));
	if(!output_scene) {
		goto l_err_memory;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	scene_ptr = zwscn_list_get_by_index((*scene_list), index);
	if(scene_ptr == NULL) {
		goto l_err;
	}
	scene_ptr->scene->flag_updating = (scene_ptr->pending_report_reqs == 0) ? 0 : 1; //set the flag based on the condition whether we are waiting for any reports
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene index=%d scene_id=%d scene_name=%s flag_updating=%u\n", net, index, scene_ptr->scene->id, scene_ptr->scene->name, scene_ptr->scene->flag_updating);
	*(output_scene) = *(scene_ptr->scene); // C compiler in-built structure copy
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

ZWSCN_STATUS_T zwscn_get_action_by_index(void *net, uint32_t scene_id, uint32_t index, zwscn_action_p *out_scene_action) {
	zwscn_list_t *action_list;
	zwscn_action_p action_ptr;
	zwscn_action_p output_action;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (out_scene_action == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d index=%d\n", net, scene_id, index);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	output_action = (zwscn_action_p) calloc(1, sizeof(zwscn_action_t));
	if(!output_action) {
		goto l_err_memory;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_action_list(net, scene_id, &action_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	action_ptr = zwscn_list_get_by_index((*action_list), index);
	if(action_ptr == NULL) {
		goto l_err_invalid_index;
	}
	
	*(output_action) = *(action_ptr); // C compiler in-built structure copy
	if((action_ptr->device_args != NULL) && (action_ptr->device_args_size != 0)) {
		output_action->device_args = malloc(action_ptr->device_args_size);
		if(output_action->device_args == NULL) {
			goto l_err_memory_2;
		}
		memcpy(output_action->device_args, action_ptr->device_args, action_ptr->device_args_size);
	} else {
		output_action->device_args = NULL;
		output_action->device_args_size = 0;
	}
	zwscn_network_unlock_reader(net);
	
	*(out_scene_action) = output_action; //NOTE: this must be freed by the application 

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_memory_2:
	zwscn_network_unlock_reader(net);
	free(output_action);
	RETURN(ZWSCN_ERR_MEMORY);
l_err_invalid_index:
	zwscn_network_unlock_reader(net);
	free(output_action);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err:
	zwscn_network_unlock_reader(net);
	free(output_action);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_get_schedule_by_index(void *net, uint32_t scene_id, uint32_t index, zwscn_schedule_p *out_scene_schedule) {
	zwscn_list_t *schedule_list;
	zwscn_schedule_p schedule_ptr;
	zwscn_schedule_p output_schedule;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (out_scene_schedule == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d index=%d\n", net, scene_id, index);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	output_schedule = (zwscn_schedule_p) calloc(1, sizeof(zwscn_schedule_t));
	if(!output_schedule) {
		goto l_err_memory;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_schedule_list(net, scene_id, &schedule_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	schedule_ptr = zwscn_list_get_by_index((*schedule_list), index);
	if(schedule_ptr == NULL) {
		goto l_err_invalid_index;
	}
	*(output_schedule) = *(schedule_ptr); // C compiler in-built structure copy
	output_schedule->timer_id = 0;				//Reset private data value
	output_schedule->timer_user_data = NULL;	//Reset private data value
	zwscn_network_unlock_reader(net);
	
	*(out_scene_schedule) = output_schedule; //NOTE: this must be freed by the application 

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_invalid_index:
	zwscn_network_unlock_reader(net);
	free(output_schedule);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err:
	zwscn_network_unlock_reader(net);
	free(output_schedule);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_get_event_by_index(void *net, uint32_t scene_id, uint32_t index, zwscn_event_p *out_scene_event) {
	zwscn_list_t *event_list;
	zwscn_event_p event_ptr;
	zwscn_event_p output_event;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (out_scene_event == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d index=%d\n", net, scene_id, index);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	output_event = (zwscn_event_p) calloc(1, sizeof(zwscn_event_t));
	if(!output_event) {
		goto l_err_memory;
	}

	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
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

ZWSCN_STATUS_T zwscn_get_network_persistent_context(void *net, char **out_context) {
	zwscn_init_p scene_init_data;
	char *persistent_context = NULL;
	uint32_t str_size = 0;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (out_context == NULL)) {
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
		str_size = sizeof(scene_init_data->init_input->context_persistent);
		persistent_context = calloc(1, str_size);
		if(persistent_context == NULL) {
			goto l_err_memory;
		}
		zwscn_util_strcpy(persistent_context, scene_init_data->init_input->context_persistent, str_size);
	}
	zwscn_network_unlock_reader(net);
	(*out_context) = persistent_context;
	
	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;
	
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_memory:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_MEMORY);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_get_network_timezone(void *net, char **out_timezone) {
	zwscn_init_p scene_init_data;
	char *timezone = NULL;
	uint32_t str_size = 0;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (out_timezone == NULL)) {
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
		str_size = sizeof(scene_init_data->init_input->client_timezone);
		timezone = calloc(1, str_size);
		if(timezone == NULL) {
			goto l_err_memory;
		}
		zwscn_util_strcpy(timezone, scene_init_data->init_input->client_timezone, str_size);
	}
	zwscn_network_unlock_reader(net);
	(*out_timezone) = timezone;
	
	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;
	
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_memory:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_MEMORY);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

int zwscn_count_scenes(void *net){
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
	if(zwscn_find_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
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

int zwscn_count_actions(void *net, uint32_t scene_id){
	zwscn_list_t *action_list;
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
	if(zwscn_find_action_list(net, scene_id, &action_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	count = zwscn_list_size((*action_list));
	zwscn_network_unlock_reader(net);
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d actions_count=%d\n", net, scene_id, count);
	
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

int zwscn_count_schedules(void *net, uint32_t scene_id){
	zwscn_list_t *schedule_list;
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
	if(zwscn_find_schedule_list(net, scene_id, &schedule_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	count = zwscn_list_size((*schedule_list));
	zwscn_network_unlock_reader(net);
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d schedules_count=%d\n", net, scene_id, count);
	
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

int zwscn_count_events(void *net, uint32_t scene_id){
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
	if(zwscn_find_event_list(net, scene_id, &event_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	count = zwscn_list_size((*event_list));
	zwscn_network_unlock_reader(net);
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d events_count=%d\n", net, scene_id, count);
	
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

int zwscn_get_max_num_scenes(void *net){
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
		max_limit = (scene_init_data->init_input->num_max_scenes < 0) ? ZWSCN_MAX_SCENES : scene_init_data->init_input->num_max_scenes;
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

int zwscn_get_max_num_actions(void *net){
	zwscn_init_p init_data;
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
	if((init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	if(init_data->init_input) {
		max_limit = (init_data->init_input->num_max_actions_per_scene < 0) ? ZWSCN_MAX_ACTIONS : init_data->init_input->num_max_actions_per_scene;
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

int zwscn_get_max_num_schedules(void *net){
	zwscn_init_p init_data;
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
	if((init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	if(init_data->init_input) {
		max_limit = (init_data->init_input->num_max_schedules_per_scene < 0) ? ZWSCN_MAX_SCHEDULES : init_data->init_input->num_max_schedules_per_scene;
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

int zwscn_get_max_num_events(void *net){
	zwscn_init_p init_data;
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
	if((init_data = zwscn_get_init_data(net)) == NULL) {
		goto l_err;
	}
	if(init_data->init_input) {
		max_limit = (init_data->init_input->num_max_events_per_scene < 0) ? ZWSCN_MAX_EVENTS : init_data->init_input->num_max_events_per_scene;
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

int zwscn_get_default_max_num_scenes(){
	return ZWSCN_MAX_SCENES;
}

int zwscn_get_default_max_num_actions(){
	return ZWSCN_MAX_ACTIONS;
}

int zwscn_get_default_max_num_schedules(){
	return ZWSCN_MAX_SCHEDULES;
}

int zwscn_get_default_max_num_events(){
	return ZWSCN_MAX_EVENTS;
}
 
ZWSCN_STATUS_T zwscn_edit_scene(void *net, uint32_t scene_id, zwscn_scene_p scene) {
	zwscn_scene_cont_p scene_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (scene == NULL)) {
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
	zwscn_set_statelog_start_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d old scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d new scene_name=%s\n", net, scene_id, scene->name);
	// Copy the editable data
	zwscn_util_strcpy(scene_ptr->scene->name, scene->name, sizeof(scene_ptr->scene->name));
	scene_ptr->scene->active = scene->active;
	if(zwscn_persistence_save_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_edit_action(void *net, uint32_t scene_id, uint32_t action_id, zwscn_action_p action) {
	zwscn_action_p action_ptr;
	void *temp_ptr;
	zwscn_scene_cont_p scene_ptr = NULL;
	ZWSCN_STATUS_T result = ZWSCN_ERR_NONE;
	ZWSCN_BOOL_T is_device_args_valid = ZWSCN_FALSE;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (action == NULL)) {
		goto l_err_args;
	}
	if((action->device_args == NULL) && (action->device_args_size > 0)) {
		zwscn_message_log_warning("zwscn_edit_action: device_args_size(%d) is not zero while device_args is NULL\n", action->device_args_size);
		goto l_err_args;
	}
	if(action->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
		zwscn_message_log_warning("zwscn_edit_action: device_args_size is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%d)\n", ZWSCN_DEVICE_ARGS_MAX_SIZE);
		goto l_err_args;
	}
	if((zwscn_eval_device_args(action->device_args, action->id, action->cmd, &is_device_args_valid) != ZWSCN_ERR_NONE) || (is_device_args_valid != ZWSCN_TRUE)) {
		zwscn_message_log_warning("zwscn_edit_action: device_args are not valid\n");
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d action_id=0x%x\n", net, scene_id, action_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	zwscn_set_statelog_start_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	if(zwscn_find_action(net, scene_id, action_id, &action_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	// Copy the editable data
	if(action->device_args_size == 0) {
		free(action_ptr->device_args);
		action_ptr->device_args = NULL;
		action_ptr->device_args_size = action->device_args_size;
	} else if(action_ptr->device_args_size >= action->device_args_size) {
		memcpy(action_ptr->device_args, action->device_args, action->device_args_size);
		action_ptr->device_args_size = action->device_args_size;
	} else {
		temp_ptr = realloc(action_ptr->device_args, action->device_args_size);
		if(temp_ptr == NULL) {
			goto l_err_memory;
		}
		action_ptr->device_args = temp_ptr;
		memcpy(action_ptr->device_args, action->device_args, action->device_args_size);
		action_ptr->device_args_size = action->device_args_size;
	}
	action_ptr->type = action->type; //Copying this last as the other paramerts may result in memory error
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s action_id=0x%x\n", net, scene_id, scene_ptr->scene->name, action_id);
	if(zwscn_persistence_save_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}	
////	zwscn_set_statelog_start_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM); 
//	if(zwscn_status_update_scene_req_start(net, scene_ptr, ZWSCN_TRUE) != ZWSCN_ERR_NONE) {
//		goto l_err_status;
//	}
////	zwscn_set_statelog_end_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
	result = zwscn_status_update_request_message_send(net, scene_id, action_id, ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_SCENE_ON_CHANGE);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("(%d) retuned by zwscn_status_update_request_message_send\n", result);
		//goto l_err_status; //This is not a fatal error so ignore it
	}
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_MEMORY);
l_err:
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
//l_err_status:
////	zwscn_set_statelog_end_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
//	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
//	zwscn_network_unlock_writer(net);
//	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_edit_schedule(void *net, uint32_t scene_id, uint32_t schedule_id, zwscn_schedule_p schedule) {
	zwscn_schedule_p schedule_ptr;
//	void *temp_ptr;
	zwscn_scene_cont_p scene_ptr;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (schedule == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d schedule_id=%d\n", net, scene_id, schedule_id);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	if(zwscn_schedule_is_data_valid(schedule) == ZWSCN_FALSE) {
		zwscn_message_log_warning("Schedule value for day bitmask(0x%02x) or hour(%d) or minute(%d) is invalid\n", schedule->day, schedule->hour, schedule->minute);
		goto l_err_args;
	}

	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	zwscn_set_statelog_start_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	if(zwscn_find_schedule(net, scene_id, schedule_id, &schedule_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	// Copy the editable data
	schedule_ptr->type = schedule->type;
	schedule_ptr->day =  schedule->day;
	schedule_ptr->hour = schedule->hour;
	schedule_ptr->minute = schedule->minute;
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s schedule_id=%d\n", net, scene_id, scene_ptr->scene->name, schedule_id);
	if(zwscn_persistence_save_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	if(zwscn_schedule_handle_update(net, scene_ptr, schedule_ptr->id, ZWSCN_TRUE) != ZWSCN_ERR_NONE) { //ZWSCN_TRUE is for checking the schedule from today
		goto l_err;
	}
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err:
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_edit_event(void *net, uint32_t scene_id, uint32_t event_id, zwscn_event_p event) {
	zwscn_event_p event_ptr;
	void *temp_ptr;
	zwscn_scene_cont_p scene_ptr;
	ZWSCN_BOOL_T is_device_args_valid = ZWSCN_FALSE;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0) || (event == NULL)) {
		goto l_err_args;
	}
	if((event->device_args == NULL) && (event->device_args_size > 0)) {
		zwscn_message_log_warning("zwscn_edit_event: device_args_size(%d) is not zero while device_args is NULL\n", event->device_args_size);
		goto l_err_args;
	}
	if(event->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
		zwscn_message_log_warning("zwscn_edit_event: device_args_size is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%d)\n", ZWSCN_DEVICE_ARGS_MAX_SIZE);
		goto l_err_args;
	}
	if((zwscn_eval_device_args(event->device_args, event->id, event->cmd, &is_device_args_valid) != ZWSCN_ERR_NONE) || (is_device_args_valid != ZWSCN_TRUE)) {
		zwscn_message_log_warning("zwscn_edit_event: device_args are not valid\n");
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
	zwscn_set_statelog_start_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	if(zwscn_find_event(net, scene_id, event_id, &event_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
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
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s event_id=0x%x\n", net, scene_id, scene_ptr->scene->name, event_id);
	if(zwscn_persistence_save_scene(net, scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_memory:
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_MEMORY);
l_err:
	zwscn_set_statelog_end_op_add_modify(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_writer(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_execute_scene(void *net, uint32_t scene_id) {
//	zwscn_mqueue_t mqueue_execute_request_handler;
//	zwscn_mqueue_message_p message;
	ZWSCN_STATUS_T result = ZWSCN_ERR_NONE;

	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);

	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}

//	zwscn_network_lock_reader(net); //TODO - Check if execution require writers lock 
//	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
//		goto l_err_network_invalid;
//	}
//	zwscn_set_statelog_start_op_exec(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
//	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
//		goto l_err;
//	}
//
//	if(zwscn_execute_scene_private(net, scene_ptr, ZWSCN_STATELOG_TRIGGER_MANUAL) != ZWSCN_ERR_NONE) {
//		goto l_err;
//	}
//	
////	zwscn_set_statelog_start_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM); 
//	if(zwscn_status_update_scene_req_start(net, scene_ptr, ZWSCN_TRUE) != ZWSCN_ERR_NONE) {
//		goto l_err_status;
//	}
////	zwscn_set_statelog_end_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM); 
//	zwscn_set_statelog_end_op_exec(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
//	zwscn_network_unlock_reader(net);

//	message = zwscn_mqueue_message_new(NULL);
//	if(message ==  NULL) {
//		goto l_err_message;
//	}
//	message->net = net;
//	message->type = ZWSCN_MESSAGE_TYPE_EXECUTE_SCENE;
//	message->sub_type = scene_id; //Using field sub-type to store scene_id so that the there is no need to create a new field
//	if(zwscn_find_mqueue_thread_execute_request_handler(net, &mqueue_execute_request_handler) == ZWSCN_ERR_NONE) {
//		if(mqueue_execute_request_handler && (zwscn_mqueue_length(mqueue_execute_request_handler) < ZWSCN_MQUEUE_LENGTH_MAX_EXECUTE_REQUEST_HANDLER)) {
//			ZWSCN_MESSAGE_LOG_DEBUG("sending message to execute request handler thread for net=%p scene id=%d\n", net, scene_id);
//			zwscn_mqueue_message_push(mqueue_execute_request_handler, message); //Send it to execute request handler thread
//		} else {
//			zwscn_message_log_message("MQUEUE for Execute Request Handler is full. dropping new message...\n");
//			zwscn_free_mqueue_message(message);
//		}
//	}
	
	result = zwscn_execute_request_message_send(net, scene_id, ZWSCN_MESSAGE_TYPE_EXECUTE_SCENE);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("(%d) retuned by zwscn_execute_request_message_send\n", result);
		goto l_err;
	}

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;

l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
//l_err_message:
//	RETURN(ZWSCN_ERR_MEMORY);
l_err:
	RETURN(ZWSCN_ERR_GENERIC);
//l_err:
//	zwscn_set_statelog_end_op_exec(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
//	zwscn_network_unlock_reader(net);
//	RETURN(ZWSCN_ERR_GENERIC);
//l_err_status:
////	zwscn_set_statelog_end_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
//	zwscn_set_statelog_end_op_exec(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
//	zwscn_network_unlock_reader(net);
//	RETURN(ZWSCN_ERR_GENERIC);
//l_err_network_invalid:
//	zwscn_network_unlock_reader(net);
//	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

uint32_t zwscn_get_last_executed_scene(void *net) {
	uint32_t result = 0;

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
	result = zwscn_find_last_executed_scene(net);
	zwscn_network_unlock_reader(net);
	
	return result;
l_err_args:
	return result;
//l_err:
//	zwscn_network_unlock_reader(net);
//	return result;
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_status_update_scene(void *net, uint32_t scene_id) {
//	zwscn_mqueue_t mqueue_status_update_request_handler;
//	zwscn_mqueue_message_p message;
	ZWSCN_STATUS_T result = ZWSCN_ERR_NONE;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if (!net || (scene_id == 0)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d\n", net, scene_id);

	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
//	zwscn_network_lock_reader(net);
//	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
//		goto l_err_network_invalid;
//	}
//	zwscn_set_statelog_start_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL); 
//	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
//		goto l_err;
//	}
//	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_id=%d scene_name=%s\n", net, scene_id, scene_ptr->scene->name);
//	if(zwscn_status_update_scene_req_start(net, scene_ptr, ZWSCN_FALSE) != ZWSCN_ERR_NONE) {
//		goto l_err;
//	}
//	zwscn_set_statelog_end_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
//	zwscn_network_unlock_reader(net);

//	message = zwscn_mqueue_message_new(NULL);
//	if(message ==  NULL) {
//		goto l_err_message;
//	}
//	message->net = net;
//	message->type = ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_SCENE;
//	message->sub_type = scene_id; //Using field sub-type to store scene_id so that the there is no need to create a new field
//	if(zwscn_find_mqueue_thread_status_update_request_handler(net, &mqueue_status_update_request_handler) == ZWSCN_ERR_NONE) {
//		if(mqueue_status_update_request_handler && (zwscn_mqueue_length(mqueue_status_update_request_handler) < ZWSCN_MQUEUE_LENGTH_MAX_STATUS_UPDATE_REQUEST_HANDLER)) {
//			ZWSCN_MESSAGE_LOG_DEBUG("sending message to status update request handler thread for net=%p scene id=%d\n", net, scene_id);
//			zwscn_mqueue_message_push(mqueue_status_update_request_handler, message); //Send it to status update request handler thread
//		} else {
//			zwscn_message_log_message("MQUEUE for Status Update Request Handler is full. dropping new message...\n");
//			zwscn_free_mqueue_message(message);
//		}
//	}
#if 1	
	result = zwscn_status_update_request_message_send(net, scene_id, 0, ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_SCENE); //0 - update all actions
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("(%d) retuned by zwscn_status_update_request_message_send\n", result);
		goto l_err;
	}
#endif

	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
//l_err_message:
//	RETURN(ZWSCN_ERR_MEMORY);
l_err:
	RETURN(ZWSCN_ERR_GENERIC);
//l_err:
//	zwscn_set_statelog_end_op_status_update(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
//	zwscn_network_unlock_reader(net);
//	RETURN(ZWSCN_ERR_GENERIC);
//l_err_network_invalid:
//	zwscn_network_unlock_reader(net);
//	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_status_update_all_scenes(void *net) {
//	zwscn_mqueue_t mqueue_status_update_request_handler;
//	zwscn_mqueue_message_p message;
	zwscn_list_t  *scene_list;
	zwscn_scene_cont_p scene_ptr = NULL;
	uint32_t scene_id = 0;
	int count = 0;
	int index = 0;
	ZWSCN_STATUS_T result = ZWSCN_ERR_NONE;
	
	 
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
//	zwscn_set_statelog_start_op_status_update_all(net);
	if(zwscn_find_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	count = zwscn_list_size((*scene_list));
	for(index=0; index < count; ++index) {
		if (!zwscn_does_net_exist(net)) { //Special case - If network was removed when this thread was looping then we should just exit
			goto l_err_network_invalid;
		}
		scene_ptr = zwscn_list_get_by_index((*scene_list), index);
//		zwscn_set_statelog_start_op_status_update(net, scene_ptr->scene->id, ZWSCN_STATELOG_TRIGGER_MANUAL);
//		if(zwscn_status_update_scene_req_start(net, scene_ptr, ZWSCN_FALSE) != ZWSCN_ERR_NONE) {
//			goto l_err_2;
//		}
		scene_id = scene_ptr->scene->id;
		result = zwscn_status_update_request_message_send(net, scene_id, 0, ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_SCENE); //0 - update all actions
		if(result != ZWSCN_ERR_NONE) {
			zwscn_message_log_warning("(%d) retuned by zwscn_status_update_request_message_send\n", result);
			goto l_err;
		}
//		zwscn_set_statelog_end_op_status_update(net, scene_ptr->scene->id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	}
//	zwscn_set_statelog_end_op_status_update_all(net);
	zwscn_network_unlock_reader(net);

//	message = zwscn_mqueue_message_new(NULL);
//	if(message ==  NULL) {
//		goto l_err_message;
//	}
//	message->net = net;
//	message->type = ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_ALL;
//	message->sub_type = 0; //Using field sub-type to store scene_id which is set to 0 as it is not required for this operation
//	if(zwscn_find_mqueue_thread_status_update_request_handler(net, &mqueue_status_update_request_handler) == ZWSCN_ERR_NONE) {
//		if(mqueue_status_update_request_handler && (zwscn_mqueue_length(mqueue_status_update_request_handler) < ZWSCN_MQUEUE_LENGTH_MAX_STATUS_UPDATE_REQUEST_HANDLER)) {
//			ZWSCN_MESSAGE_LOG_DEBUG("sending message to status update request handler thread for net=%p\n", net);
//			zwscn_mqueue_message_push(mqueue_status_update_request_handler, message); //Send it to status update request handler thread
//		} else {
//			zwscn_message_log_message("MQUEUE for Status Update Request Handler is full. dropping new message...\n");
//			zwscn_free_mqueue_message(message);
//		}
//	}

//#if 1	
//	result = zwscn_status_update_request_message_send(net, 0, ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_ALL);
//	if(result != ZWSCN_ERR_NONE) {
//		zwscn_message_log_warning("(%d) retuned by zwscn_status_update_request_message_send\n", result);
//		goto l_err;
//	}
//#endif
	
	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
//l_err_message:
//	RETURN(ZWSCN_ERR_MEMORY);
//l_err:
//	RETURN(ZWSCN_ERR_GENERIC);
	
//l_err_2:
////	zwscn_set_statelog_end_op_status_update(net, 0, ZWSCN_STATELOG_TRIGGER_MANUAL);
l_err:
//	zwscn_set_statelog_end_op_status_update_all(net);
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

ZWSCN_STATUS_T zwscn_get_statelog(void *net, uint32_t category, uint32_t subcategory, uint64_t *value) {
	ZWSCN_STATUS_T result = ZWSCN_ERR_NONE;
	
	if ((!net) || (value == NULL)) {
		goto l_err_args;
	}

	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	zwscn_statelog_lock(net); //NOTE: This is different mutex lock as statelog API should not wait for other calls to finish
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	result = zwscn_statelog_get(net, category, subcategory, value);
	zwscn_statelog_unlock(net);
	
	
	return result;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
//l_err:
//	zwscn_statelog_unlock(net);
//	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_statelog_unlock(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);
}

static ZWSCN_STATUS_T zwscn_save_action(void *net, zwscn_scene_cont_p scene_container_ptr, zwscn_action_p action) {
	zwscn_action_p action_ptr;
	uint8_t node_id, ep_id;
	uint16_t cls_id;
	zwscn_init_p init_data;
	int result = 0;
	ZWSCN_BOOL_T is_device_args_valid = ZWSCN_FALSE;
	
	if ((net == NULL) || (scene_container_ptr == NULL)  || (scene_container_ptr->scene == NULL) || (action == NULL)) {
		goto l_err_args;
	}
	zwscn_message_log_debug("SAVE net=%p scene_name=%s action=0x%x\n", net, scene_container_ptr->scene->name, action->id);
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	//check device args
	if((action->device_args == NULL) && (action->device_args_size > 0)) {
		zwscn_message_log_warning("zwscn_save_scene: action device_args_size(%u) is not zero while device_args is NULL\n", action->device_args_size);
		goto l_err_args_actions;
	}
	if(action->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
		zwscn_message_log_warning("zwscn_save_scene: action device_args_size(%u) is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%u)\n", action->device_args_size, ZWSCN_DEVICE_ARGS_MAX_SIZE);
		goto l_err_args_actions;
	}
	if((zwscn_eval_device_args(action->device_args, action->id, action->cmd, &is_device_args_valid) != ZWSCN_ERR_NONE) || (is_device_args_valid != ZWSCN_TRUE)) {
		zwscn_message_log_warning("zwscn_save_scene: action device_args are not valid\n");
		goto l_err_args;
	}
	//check for duplicate
	if(zwscn_list_get_action_by_id((scene_container_ptr->list_actions), action->id) != NULL) {
		goto l_err_duplicate_action;
	}
	if(zwscn_list_get_event_by_id((scene_container_ptr->list_events), action->id) != NULL) {
		goto l_err_duplicate_event;
	}
	//check for device
	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	node_id = zwscn_util_combo_id_to_node_id(action->id);
	ep_id = zwscn_util_combo_id_to_ep_id(action->id);
	cls_id = zwscn_util_combo_id_to_cls_id(action->id);
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
	
	action_ptr = (zwscn_action_p) calloc(1, sizeof(zwscn_action_t));
	if(action_ptr == NULL) {
		goto l_err_memory;
	}
	*(action_ptr) = *(action); // C compiler in-built structure copy
	if(action->device_args && (action->device_args_size != 0)) {
		action_ptr->device_args = calloc(1, action->device_args_size);
		if(!action_ptr->device_args) {
			goto l_err_memory_1;
		}
		memcpy(action_ptr->device_args, action->device_args, action->device_args_size);
	} else {
		action_ptr->device_args = NULL;
	}
	
	if((scene_container_ptr->list_actions = zwscn_list_add(scene_container_ptr->list_actions, action_ptr)) == NULL) {
		goto l_err_action_add;
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_args_actions:
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_duplicate_action:
	RETURN(ZWSCN_ERR_ACTION_EXISTS);
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
	free(action_ptr);
	RETURN(ZWSCN_ERR_MEMORY);
l_err_action_add:
	free(action_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);	
}

static ZWSCN_STATUS_T zwscn_save_schedule(void *net, zwscn_scene_cont_p scene_container_ptr, zwscn_schedule_p schedule) {
	zwscn_schedule_p schedule_ptr;
	
	if ((net == NULL) || (scene_container_ptr == NULL)  || (scene_container_ptr->scene == NULL) || (schedule == NULL)) {
		goto l_err_args;
	}
	zwscn_message_log_debug("SAVE net=%p scene_name=%s schedule day=0x%02x hour=%d minute=%d\n", net, scene_container_ptr->scene->name, schedule->day, schedule->hour, schedule->minute);
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	if(zwscn_schedule_is_data_valid(schedule) == ZWSCN_FALSE) {
		zwscn_message_log_warning("Schedule value for day bitmask(0x%02x) or hour(%d) or minute(%d) is invalid\n", schedule->day, schedule->hour, schedule->minute);
		goto l_err_args_schedules;
	}
	
	schedule_ptr = (zwscn_schedule_p) calloc(1, sizeof(zwscn_schedule_t));
	if(schedule_ptr == NULL) {
		goto l_err_memory;
	}
	*(schedule_ptr) = *(schedule); // C compiler in-built structure copy
	
	schedule_ptr->timer_id = 0;				//Reset private data value
	schedule_ptr->timer_user_data = NULL;	//Reset private data value
	schedule_ptr->id = zwscn_get_new_schedule_id(scene_container_ptr);
	if((scene_container_ptr->list_schedules = zwscn_list_add(scene_container_ptr->list_schedules, schedule_ptr)) == NULL) {
		goto l_err_schedule_add;
	}

	return ZWSCN_ERR_NONE;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_args_schedules:
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_schedule_add:
	free(schedule_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
}

static ZWSCN_STATUS_T zwscn_save_event(void *net, zwscn_scene_cont_p scene_container_ptr, zwscn_event_p event) {
	zwscn_event_p event_ptr;
	uint8_t node_id, ep_id;
	uint16_t cls_id;
	zwscn_init_p init_data;
	int result = 0;
	ZWSCN_BOOL_T is_device_args_valid = ZWSCN_FALSE;
	
	if ((net == NULL) || (scene_container_ptr == NULL)  || (scene_container_ptr->scene == NULL) || (event == NULL)) {
		goto l_err_args;
	}
	zwscn_message_log_debug("SAVE net=%p scene_name=%s event=0x%x\n", net, scene_container_ptr->scene->name, event->id);
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	//check device args
	if((event->device_args == NULL) && (event->device_args_size > 0)) {
		zwscn_message_log_warning("zwscn_save_scene: event device_args_size(%u) is not zero while device_args is NULL\n", event->device_args_size);
		goto l_err_args_events;
	}
	if(event->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
		zwscn_message_log_warning("zwscn_save_scene: event device_args_size(%u) is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%u)\n", event->device_args_size, ZWSCN_DEVICE_ARGS_MAX_SIZE);
		goto l_err_args_events;
	}
	if((zwscn_eval_device_args(event->device_args, event->id, event->cmd, &is_device_args_valid) != ZWSCN_ERR_NONE) || (is_device_args_valid != ZWSCN_TRUE)) {
		zwscn_message_log_warning("zwscn_save_scene: event device_args are not valid\n");
		goto l_err_args;
	}
	//check for duplicate
	if(zwscn_list_get_event_by_id((scene_container_ptr->list_events), event->id) != NULL) {
		goto l_err_duplicate_event;
	}
	if(zwscn_list_get_action_by_id((scene_container_ptr->list_actions), event->id) != NULL) {
		goto l_err_duplicate_action;
	}
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
	
	event_ptr = (zwscn_event_p) calloc(1, sizeof(zwscn_event_t));
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
	
	if((scene_container_ptr->list_events = zwscn_list_add(scene_container_ptr->list_events, event_ptr)) == NULL) {
		goto l_err_event_add;
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_args_events:
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_duplicate_action:
	RETURN(ZWSCN_ERR_ACTION_EXISTS);
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
	zwscn_free_event(event_ptr);
	RETURN(ZWSCN_ERR_MEMORY);
l_err_event_add:
	zwscn_free_event(event_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_network_invalid:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_INVALID_NETWORK);	
}

ZWSCN_STATUS_T zwscn_save_scene(void *net, zwscn_scene_p scene, 
								zwscn_action_t *array_actions, int num_actions,
								zwscn_schedule_t *array_schedules, int num_schedules, 
								zwscn_event_t *array_events, int num_events, uint32_t *out_id) {
	zwscn_scene_cont_p scene_container_ptr;
	zwscn_scene_cont_p scene_ptr;
	zwscn_action_p action_temp;
	zwscn_schedule_p schedule_temp;
	zwscn_event_p event_temp;
	zwscn_list_t *scene_list;
	uint32_t scene_id=0;
	uint32_t schedule_id=0;
	int max_actions=0;
	int max_schedules=0;
	int max_events=0;
	int index=0;
	int result=0;
	int i=0;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER_TIME();
	if ((net == NULL) || (scene == NULL) || 
		((num_actions > 0) && (array_actions == NULL)) || 
		((num_schedules > 0) && (array_schedules == NULL)) || 
		((num_events > 0) && (array_events == NULL)) || 
		(out_id == NULL)) {
		goto l_err_args;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene id=%d name=%s\n", net, scene->id, scene->name);
	
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	zwscn_network_lock_reader(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid_1;
	}
	max_actions = zwscn_get_max_actions(net);
	max_schedules = zwscn_get_max_schedules(net);
	max_events = zwscn_get_max_events(net);
	if(num_actions > max_actions) {
		zwscn_message_log_warning("Input Scene has more actions (%d) than allowed (%d)\n", num_actions, max_actions);
		goto l_err_num_actions;
	}
	if(num_schedules > max_schedules) {
		zwscn_message_log_warning("Input Scene has more schedules (%d) than allowed (%d)\n", num_schedules, max_schedules);
		goto l_err_num_schedules;
	}
	if(num_events > max_events) {
		zwscn_message_log_warning("Input Scene has more events (%d) than allowed (%d)\n", num_events, max_events);
		goto l_err_num_events;
	}
	zwscn_network_unlock_reader(net);
		
	//scene metadata
	scene_container_ptr = (zwscn_scene_cont_p) calloc(1, sizeof(zwscn_scene_cont_t));
	if(scene_container_ptr == NULL) {
		goto l_err_memory;
	}
	scene_container_ptr->net = net;
	scene_container_ptr->mutex_status_update_timer_info = zwscn_mutex_new();
	if(scene_container_ptr->mutex_status_update_timer_info == NULL) {
		goto l_err_memory_mutex;
	}
	scene_container_ptr->hash_status_update_timer_info = zwscn_hash_new();
	scene_container_ptr->scene = (zwscn_scene_p) calloc(1, sizeof(zwscn_scene_t));
	if(scene_container_ptr->scene == NULL) {
		goto l_err_memory_1;
	}
	*(scene_container_ptr->scene) = *(scene); // C compiler in-built structure copy
	
	//Add Actions
	for(i=0; i<num_actions; ++i) {
		action_temp = &(array_actions[i]);
		if(action_temp == NULL) {
			goto l_err_args_actions;
		}
		result = zwscn_save_action(net, scene_container_ptr, action_temp);
		if(result != ZWSCN_ERR_NONE) {
			zwscn_message_log_warning("Error (%d) saving action=0x%x for net=%p scene=%d\n", result, action_temp->id, net, scene_container_ptr->scene->id);
			goto l_err_action;
		}
	}
	//Add Schedules
	for(i=0; i<num_schedules; ++i) {
		schedule_temp = &(array_schedules[i]);
		if(schedule_temp == NULL) {
			goto l_err_args_schedules;
		}
		result = zwscn_save_schedule(net, scene_container_ptr, schedule_temp);
		if(result != ZWSCN_ERR_NONE) {
			zwscn_message_log_warning("Error (%d) saving schedule=%d for net=%p scene=%d\n", result, i, net, scene_container_ptr->scene->id);
			goto l_err_schedule;
		}
	}
	//Add Events
	for(i=0; i<num_events; ++i) {
		event_temp = &(array_events[i]);
		if(event_temp == NULL) {
			goto l_err_args_events;
		}
		result = zwscn_save_event(net, scene_container_ptr, event_temp);
		if(result != ZWSCN_ERR_NONE) {
			zwscn_message_log_warning("Error (%d) saving event=0x%x for net=%p scene=%d\n", result, event_temp->id, net, scene_container_ptr->scene->id);
			goto l_err_event;
		}
	}
	
	//Update triggers info
	zwscn_scene_triggers_info_update(net, scene_container_ptr);
	
	zwscn_network_lock_writer(net);
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid_2;
	}
	scene_id = scene_container_ptr->scene->id;
	if(zwscn_find_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	if(scene_id == 0) { //new scene
		if(zwscn_is_list_full_scene(net, (*scene_list))) {
			goto l_err_list_full;
		}
		if((scene_id = zwscn_generate_scene_id(net)) == 0) {
			goto l_err;
		}
		scene_container_ptr->scene->id = scene_id;
		if(((*scene_list) = zwscn_list_add((*scene_list), scene_container_ptr)) == NULL) {
			goto l_err;
		}
	} else { //Edit existing scene - in this API we'll just replace the old scene
		if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
			goto l_err;
		}
		index = zwscn_list_get_index((*scene_list), scene_ptr);
		if(index == -1) {
			goto l_err;
		}
		scene_container_ptr->scene->status = scene_ptr->scene->status;
		scene_container_ptr->scene->sutime = scene_ptr->scene->sutime;
		scene_container_ptr->scene->letrigger = scene_ptr->scene->letrigger;
		scene_container_ptr->scene->letime = scene_ptr->scene->letime;
		scene_container_ptr->scene->lkstatus = scene_ptr->scene->lkstatus;
		scene_container_ptr->scene->lksutime = scene_ptr->scene->lksutime;
		if(((*scene_list) = zwscn_list_insert((*scene_list), scene_container_ptr, index)) == NULL) {
			goto l_err;
		}
		(*scene_list) = zwscn_list_remove((*scene_list), scene_ptr);
		zwscn_free_scene(scene_ptr);
	}
	
	//Update Schedules
	for(i=0; i<num_schedules; ++i) {
		schedule_temp = zwscn_list_get_by_index(scene_container_ptr->list_schedules, i);
		if(schedule_temp == NULL) {
			zwscn_message_log_warning("Schedule ptr is NULL at index %d\n", i);
			(*scene_list) = zwscn_list_remove((*scene_list), scene_container_ptr);
			goto l_err_schedule_update;
		}
		schedule_id = schedule_temp->id;
		result = zwscn_schedule_handle_update(net, scene_container_ptr, schedule_id, ZWSCN_TRUE);
		if(result != ZWSCN_ERR_NONE) { //ZWSCN_TRUE is for checking the schedule from today
			zwscn_message_log_warning("Error(%d) returned by zwscn_schedule_handle_update\n", result);
			(*scene_list) = zwscn_list_remove((*scene_list), scene_container_ptr);
			goto l_err_schedule_update;
		}
	}
	if(zwscn_persistence_save_scene(net, scene_container_ptr) != ZWSCN_ERR_NONE) {
		goto l_err_persistence;
	}
	ZWSCN_MESSAGE_LOG_DEBUG("net=%p scene_name=%s returned id=%u\n", net, scene->name, scene_container_ptr->scene->id);
	(*out_id) = scene_container_ptr->scene->id;
	zwscn_network_unlock_writer(net);
	
	//Send status update request
	result = zwscn_status_update_request_message_send(net, scene_id, 0, ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_SCENE_ON_CHANGE); //0 - update all actions
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("(%d) retuned by zwscn_status_update_request_message_send\n", result);
		//don't return an error here
	}
	//update statelog
	zwscn_set_statelog_start_op_add(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	zwscn_set_statelog_end_op_add(net, scene_id, ZWSCN_STATELOG_TRIGGER_MANUAL);
	
	ZWSCN_MESSAGE_LOG_DEBUG_EXIT_TIME();
	return ZWSCN_ERR_NONE;
l_err_args:
	RETURN(ZWSCN_ERR_INVALID_ARGS);
l_err_num_actions:
l_err_num_schedules:
l_err_num_events:
	zwscn_network_unlock_reader(net);
	RETURN(ZWSCN_ERR_LIST_FULL);
l_err_memory_1:
	zwscn_hash_destroy(scene_container_ptr->hash_status_update_timer_info);
	zwscn_mutex_delete(scene_container_ptr->mutex_status_update_timer_info);
l_err_memory_mutex:
	free(scene_container_ptr);
l_err_memory:
	RETURN(ZWSCN_ERR_MEMORY);
l_err_args_actions:
l_err_args_schedules:
l_err_args_events:
	zwscn_free_new_scene(scene_container_ptr);
	RETURN(ZWSCN_ERR_INVALID_DATA);
l_err_action:
l_err_schedule:
l_err_event:
	zwscn_free_new_scene(scene_container_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_list_full:
	zwscn_network_unlock_writer(net);
	zwscn_free_new_scene(scene_container_ptr);
	RETURN(ZWSCN_ERR_LIST_FULL);
l_err:
	zwscn_network_unlock_writer(net);
	zwscn_free_new_scene(scene_container_ptr);
	RETURN(ZWSCN_ERR_GENERIC);
l_err_schedule_update:
	zwscn_network_unlock_writer(net);
	zwscn_free_new_scene(scene_container_ptr);
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
