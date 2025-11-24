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
@file		zwscn_scene_schedule.c  
  
                To provide support for Scene schedule trigger
  
@author		Arif Mohammand
 
@version	1.0 2014-02-20  Initial release
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
#include "zwscn_util_time.h"
#include "zwscn_util_timer.h"
#include "zwscn_statelog.h"
#include "zwscn_scene.h"
#include "zwscn_util_mem.h"
#include "zwscn_scene_execute.h"
#include "zwscn_util_message_log.h"
#include "zwscn_scene_schedule.h"

ZWSCN_STATUS_T zwscn_schedule_handle_update(void *net, zwscn_scene_cont_p scene_ptr, uint32_t schedule_id, ZWSCN_BOOL_T flag_check_schedule_today) {
	zwscn_schedule_p schedule_ptr;
	zwscn_mqueue_message_p message;
	zwscn_init_p init_data;
	const char *tz_name;
	uint8_t 	schedule_day_bitmask;
	uint8_t		schedule_hour;
	uint8_t		schedule_minute;
	zwscn_timespan_t diff_today;
 	zwscn_timespan_t diff;
	char *temp_str;
	ZWSCN_STATUS_T result = ZWSCN_ERR_NONE;
	uint32_t scene_id = 0;
	
	zwscn_message_log_debug("ENTER zwscn_schedule_handle_update schedule_id=%d flag=%d\n", schedule_id, flag_check_schedule_today);
	if((net == NULL) || (scene_ptr == NULL)) {
		zwscn_message_log_critical("[Error] Input args\n");
		goto l_err_args;
	}
	
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		zwscn_message_log_critical("Could not find init_data for net=%p\n", net);
		goto l_err_args;
	}
	if(zwscn_find_schedule(net, scene_ptr->scene->id, schedule_id, &schedule_ptr) != ZWSCN_ERR_NONE) {
		zwscn_message_log_critical("Could not find schedule id=%d\n", schedule_id);
		goto l_err;
	}
	
	if(schedule_ptr->type == ZWSCN_SCHEDULE_DISABLED) {
		zwscn_message_log_info("zwscn_schedule_handle_update Scene id=%d schedule id=%d is disabled. Ignoring it and cancelling any existing timers...\n", scene_ptr->scene->id, schedule_id);
		zwscn_schedule_cancel_timer(schedule_ptr); //Cancel any existing timer first
		goto l_err_disabled;
	}
	
	tz_name = init_data->init_input->client_timezone;
	schedule_day_bitmask = schedule_ptr->day;
	schedule_hour = schedule_ptr->hour;
	schedule_minute = schedule_ptr->minute;
	zwscn_message_log_debug("SCHEDULE day_bitmask=0x%02x hour=%d minute=%d\n", schedule_day_bitmask, schedule_hour, schedule_minute);
	temp_str = zwscn_timezone_get_network_time(net);
	zwscn_message_log_debug("CURRENT %s\n", temp_str);
	zwscn_timezone_free_network_time_string(temp_str);
	if(flag_check_schedule_today && (zwscn_timezone_is_schedule_enabled_today(tz_name, schedule_day_bitmask))) {
		diff_today = zwscn_timezone_get_microseconds_to_schedule_today(tz_name, schedule_day_bitmask, schedule_hour, schedule_minute);
		if(diff_today == 0) {
			temp_str = zwscn_timezone_get_network_time(net);
			zwscn_message_log_debug("[1] EXECUTE THE SCENE NOW net=%p scene=%d current time %s\n", net, scene_ptr->scene->id, temp_str);
			zwscn_timezone_free_network_time_string(temp_str);
			
//			if(zwscn_execute_and_update_scene(net, scene_ptr, ZWSCN_STATELOG_TRIGGER_SCHEDULE) != ZWSCN_ERR_NONE) {
//				zwscn_message_log_critical("[Error] Scene execution failed on Schedule trigger.\n");
//				//Even though there was an error, we ignore it and continue to set other Scene Schedule triggers
//			}
			scene_id = scene_ptr->scene->id;
			result = zwscn_execute_request_message_send(net, scene_id, ZWSCN_MESSAGE_TYPE_EXECUTE_SCENE_ON_SCHEDULE);
			if(result != ZWSCN_ERR_NONE) {
				zwscn_message_log_warning("(%d) retuned by zwscn_execute_request_message_send\n", result);
				zwscn_message_log_critical("Scene execution failed on Schedule trigger.\n");
				//Even though there was an error, we ignore it and continue to set other Scene Schedule triggers
			}
			
			
			diff = zwscn_timezone_get_microseconds_to_next_schedule(tz_name, schedule_day_bitmask, schedule_hour, schedule_minute);
		} else if(diff_today > 0) {
			//printf("Schedule is today after %016llX microseconds or %u milisecons\n", diff, (unsigned int)(diff/1000));
			diff = diff_today;
		} else {
			diff = zwscn_timezone_get_microseconds_to_next_schedule(tz_name, schedule_day_bitmask, schedule_hour, schedule_minute); 
		}
	} else {
		diff = zwscn_timezone_get_microseconds_to_next_schedule(tz_name, schedule_day_bitmask, schedule_hour, schedule_minute);
	}
	
	zwscn_schedule_cancel_timer(schedule_ptr); //Cancel any existing timer first
	message = zwscn_mqueue_message_new(NULL);
	if(message ==  NULL) {
		goto l_err_memory;
	}
	zwscn_mqueue_message_refcount_inc(message);
	message->net = net;
	message->type = scene_ptr->scene->id; 	//Using type as scene_id to avoid creating new fields
	message->sub_type = schedule_id; 		//Using subtype as schedule_id to avoid creating new fields
	schedule_ptr->lctime = ZWSCN_TIME_NOW();
	schedule_ptr->timer_user_data = message;
	//schedule_ptr->timer_id = zwscn_timer_new(ZWSCN_TIME_MICRO2SEC(diff), zwscn_schedule_timer_callback, message);
	schedule_ptr->timer_id = zwscn_timer_milliseconds_new(ZWSCN_TIME_MICRO2MILLI_SEC(diff), zwscn_schedule_timer_callback, message);
	zwscn_message_log_debug("New timer started with id=%d interval=%lu sec\n", schedule_ptr->timer_id, (unsigned long)ZWSCN_TIME_MICRO2SEC(diff));
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err_memory:
	return ZWSCN_ERR_MEMORY;
l_err:
	return ZWSCN_ERR_GENERIC;
l_err_disabled:
	//return ZWSCN_ERR_ITEM_IGNORED;
	return ZWSCN_ERR_NONE; //disabled schedule is not an error
}

ZWSCN_STATUS_T zwscn_schedule_cancel_timer(zwscn_schedule_p schedule_ptr) {
	zwscn_mqueue_message_p message;
	
	zwscn_message_log_debug("ENTER zwscn_schedule_cancel_timer\n");
	if(schedule_ptr == NULL) {
		goto l_err_args;
	}
	
	if(schedule_ptr->timer_id) {
		zwscn_message_log_debug("Scene Schedule id=%d has timer id=%d. Cancelling it...\n", schedule_ptr->id, schedule_ptr->timer_id);
		zwscn_timer_delete(schedule_ptr->timer_id);
		message = (zwscn_mqueue_message_p) schedule_ptr->timer_user_data;
		if(zwscn_mqueue_message_refcount_dec_and_test(message)) {
			zwscn_free_mqueue_message(message); // Free the message as no other thread is going to
			message = NULL; 
		}
		schedule_ptr->timer_user_data = NULL;
		schedule_ptr->timer_id = 0;
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
}

//NOTE: this function uses zwscn_network_lock and zwscn_network_unlock
int zwscn_schedule_timer_callback(void *user_data) {
	zwscn_mqueue_message_p message;
	void *net;
	uint32_t scene_id = 0;
	uint32_t schedule_id = 0;
	zwscn_scene_cont_p scene_ptr = NULL;
	zwscn_schedule_p schedule_ptr = NULL;
//	zwscn_time_t time_now;
	char *temp_str;
	ZWSCN_STATUS_T result = ZWSCN_ERR_NONE;
	
	zwscn_message_log_debug("ENTER zwscn_schedule_timer_callback\n");
	if(user_data == NULL) {
		goto l_err_args;
	}
	if(zwscn_timer_is_destroyed()) {
		goto l_err_timer;
	}
	
	message = (zwscn_mqueue_message_p) user_data;
	zwscn_mqueue_message_refcount_inc(message);
	net = message->net;
	scene_id = message->type; 			//Using type as scene_id to avoid creating new fields
	schedule_id = message->sub_type; 	//Using subtype as schedule_id to avoid creating new fields
	if(zwscn_mqueue_message_refcount_dec_and_test(message)) {
		zwscn_free_mqueue_message(message); // Free the message as no other thread is going to 
	}
	if (!zwscn_does_net_exist(net)) {
		goto l_err_args;
	}
	
	zwscn_network_lock_reader(net); //TODO - Check if status update require writers lock
	if (!zwscn_does_net_exist(net)) { //If network was removed when this thread was waiting for the mutex then we should just exit
		goto l_err_network_invalid;
	}
	if(zwscn_find_scene(net, scene_id, &scene_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	if(zwscn_find_schedule(net, scene_id, schedule_id, &schedule_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
//	schedule_ptr->timer_user_data = NULL;
	if(schedule_ptr->type == ZWSCN_SCHEDULE_DISABLED) {
		zwscn_message_log_info("schedule_timer_callback Scene id=%d schedule id=%d is disabled. Ignoring it...\n", scene_id, schedule_id);
		goto l_err_disabled;
	}
	
	temp_str = zwscn_timezone_get_network_time(net);
	zwscn_message_log_debug("[2] EXECUTE THE SCENE NOW net=%p scene=%d current time %s\n", net, scene_ptr->scene->id, temp_str);
	zwscn_timezone_free_network_time_string(temp_str);
	
//	if(zwscn_execute_and_update_scene(net, scene_ptr, ZWSCN_STATELOG_TRIGGER_SCHEDULE) != ZWSCN_ERR_NONE) {
//		zwscn_message_log_critical("[Error] Scene execution failed on Schedule trigger.\n");
//		//Even though there was an error, we ignore it and continue to set other Scene Schedule triggers
//	}
	result = zwscn_execute_request_message_send(net, scene_id, ZWSCN_MESSAGE_TYPE_EXECUTE_SCENE_ON_SCHEDULE);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("(%d) retuned by zwscn_execute_request_message_send\n", result);
		zwscn_message_log_critical("Scene execution failed on Schedule trigger.\n");
		//Even though there was an error, we ignore it and continue to set other Scene Schedule triggers
	}
	
	if(zwscn_schedule_handle_update(net, scene_ptr, schedule_id, ZWSCN_FALSE) != ZWSCN_ERR_NONE) { //ZWSCN_FALSE is for NOT checking today's schedule as it is already executed above
		zwscn_message_log_critical("[Error] Schedule timer CB could not set the next schedule trigger timer.\n");
		goto l_err;
	}
	zwscn_network_unlock_reader(net);
	
	return 0; //Dismiss the timer
l_err_args:
l_err_timer:
	return 0; //Dismiss the timer
l_err:
l_err_network_invalid:
l_err_disabled:
	zwscn_network_unlock_reader(net);
	return 0; //Dismiss the timer
}

ZWSCN_BOOL_T zwscn_schedule_is_data_valid(zwscn_schedule_p schedule_ptr) {
	ZWSCN_BOOL_T result = ZWSCN_FALSE;
	
	if(schedule_ptr == NULL) {
		goto l_err_args;
	}
	
	if((schedule_ptr->day >= ZWSCN_SCHEDULE_DAY_BITMASK_MIN) && (schedule_ptr->day <= ZWSCN_SCHEDULE_DAY_BITMASK_MAX) && 
		(schedule_ptr->hour >= ZWSCN_SCHEDULE_HOUR_MIN) && (schedule_ptr->hour <= ZWSCN_SCHEDULE_HOUR_MAX) && 
		(schedule_ptr->minute >= ZWSCN_SCHEDULE_MINUTE_MIN) && (schedule_ptr->minute <= ZWSCN_SCHEDULE_MINUTE_MAX)) {
		result = ZWSCN_TRUE;
	}
	
	return result;
l_err_args:
	return ZWSCN_FALSE;
}
