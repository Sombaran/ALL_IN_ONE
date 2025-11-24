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
@file		zwscn_zwpoll.c  
  
                To provide support for preparing ZWPOLL request
  
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
#include "zwscn_util_message_log.h"
#include "zwscn_util_mqueue.h"
#include "zwscn_util_mqueue_message.h"
#include "zwscn_util_thread.h"
#include "zwscn_util_mutex.h"
#include "zwscn_util_rwlock.h"
#include "zwscn_util_timer.h"
#include "zwscn_statelog.h"
#include "zwscn_scene.h"
#include "zwscn_scene_status_update.h"
#include "zwscn_util_zwave.h"
#include "zwscn_zwpoll.h"
#include "zwscn_notification_handler.h"

///**
//callback on polling completion
//@param[in]	net	        Network runtime context
//@param[in]	handle	    Handle of the polling request
//*/
//static void zwscn_device_zwpoll_cb(void *net, uint16_t handle, void *user_param)
//{
//	zwscn_message_log_debug("ZWPoll requests completed for handle=%u, net=%p\n", (unsigned)handle, net);
//}


zwscn_device_zwpoll_req_t *zwscn_zwpoll_prepare_req(void *net, zwscn_scene_cont_p scene_ptr, zwscn_action_p action_ptr) {
	zwscn_device_zwpoll_req_t *zwscn_zwpoll_ptr = NULL;
//	zwscn_zwpoll_user_data_p zwpoll_user_data_ptr = NULL;
	
	if((net == NULL) || (scene_ptr == NULL) || (scene_ptr->scene == NULL) || (action_ptr == NULL)) {
		ZWSCN_MESSAGE_LOG_DEBUG("NULL arguments error. Exiting...\n");
		goto l_err_args;
	}
	zwscn_zwpoll_ptr = calloc(1, sizeof(zwscn_device_zwpoll_req_t));
	if(zwscn_zwpoll_ptr == NULL) {
		zwscn_message_log_critical("Out of memory\n");
		goto l_err_memory;
	}
	zwscn_zwpoll_ptr->usr_token = zwscn_zwpoll_get_token(action_ptr);
	zwscn_zwpoll_ptr->interval = zwscn_zwpoll_get_interval(action_ptr);
	zwscn_zwpoll_ptr->poll_cnt = zwscn_zwpoll_get_count(action_ptr);

//	zwpoll_user_data_ptr = malloc(sizeof(zwscn_zwpoll_user_data_t));
//	if(zwpoll_user_data_ptr == NULL) {
//		zwscn_message_log_critical("Out of memory\n");
//		goto l_err_memory2;
//	}
//	zwpoll_user_data_ptr->scene_id = scene_ptr->scene->id;
//	zwpoll_user_data_ptr->action_id = action_ptr->id;
//	zwscn_zwpoll_ptr->usr_param = zwpoll_user_data_ptr;
	
	// This param is already used by HCAPI AAL - zwscn_zwpoll_ptr->usr_param = (void *) zwscn_zwpoll_ptr->usr_token; //using void *usr_param to store uint32_t value
    zwscn_zwpoll_ptr->cmplt_cb = zwscn_zwpoll_notification_cb;
    //Note: If not using callback, set cmplt_cb to NULL as followed:
    //zwscn_zwpoll_ptr->cmplt_cb = NULL;

	return zwscn_zwpoll_ptr;

l_err_args:
	return NULL;
l_err_memory:
	return NULL;
//l_err_memory2:
//	free(zwscn_zwpoll_ptr);
//	return NULL;
}

int zwscn_zwpoll_remove_req(void *net, zwscn_scene_cont_p scene_ptr, zwscn_action_p action_ptr) {
	uint32_t token;
	int result;
	zwscn_init_p init_data;
	
	if((net == NULL) || (scene_ptr == NULL) || (action_ptr == NULL)) {
		goto l_err_args;
	}
	
	token = zwscn_zwpoll_get_token(action_ptr);
	zwscn_message_log_debug("Removing existing ZWPoll requests for token=%d\n", token);
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->init_input && init_data->init_input->zwscn_function_zwpoll_rm_mul_handler) {
		result = init_data->init_input->zwscn_function_zwpoll_rm_mul_handler(net, token);
		if(result < 0) {
			zwscn_message_log_warning("Function zwpoll_rm_mul_handler returned (%d) for net=%p scene=%d action=%d\n", result, net, scene_ptr->scene->id, action_ptr->id);
			goto l_err_callback;
		}
		//Lock network - it's already locked by the calling functions
		zwscn_zwpoll_clear_pending_reports_count(net, token, 0); //handle 0 - is ignored as we are working based on token
		//Unlock network - it'll be unlocked by the calling functions
	} else {
		zwscn_message_log_warning("Function zwpoll_rm_mul_handler is NULL for net=%p scene=%d action=%d\n", net, scene_ptr->scene->id, action_ptr->id);
		goto l_err_callback_null;
	}
	
	return result;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err_callback:
	return ZWSCN_ERR_CALLBACK_RESULT;
l_err_callback_null:
	return ZWSCN_ERR_CALLBACK_NULL;
}

ZWSCN_STATUS_T zwscn_zwpoll_clear_pending_reports_count(void *net, uint32_t token, uint16_t handle) {
	zwscn_list_t *scene_list;
	zwscn_init_p init_data;
	zwscn_scene_cont_p scene_ptr = NULL;
	zwscn_action_p action_ptr = NULL;
	uint32_t action_id = 0;
	int count = 0;
	int index = 0;
//	ZWSCN_STATUS_T result =  ZWSCN_ERR_NONE;
	 
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if((net == NULL) || (token == 0)) {
		goto l_err_args;
	}
	
	action_id = token; //TODO use conversion funtion if there is no one-to-one conversion
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		goto l_err;
	}
	if(zwscn_find_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	count = zwscn_list_size((*scene_list));
	zwscn_message_log_debug("zwpoll CB reponse - check %d scenes for net=%p\n", count, net);
	for(index=0; index < count; ++index) {
		scene_ptr = zwscn_list_get_by_index((*scene_list), index);
		if(scene_ptr && scene_ptr->scene) {
			if(zwscn_find_action(net, scene_ptr->scene->id, action_id, &action_ptr) != ZWSCN_ERR_NONE) {
				continue; // got o next action
			}
			if(action_ptr->pending_zwpoll_reqs > 0) {
				ZWSCN_MESSAGE_LOG_DEBUG("REPORTCOUNT 11 clearing zr=%u for action=0x%x\tscene_id=%d\tnetwork=0x%x\n", action_ptr->pending_zwpoll_reqs, action_ptr->id, scene_ptr->scene->id, net); 
				zwscn_mutex_lock(init_data->mutex_pending_report_status);
				if(scene_ptr->pending_report_reqs > action_ptr->pending_zwpoll_reqs) {
					scene_ptr->pending_report_reqs -= action_ptr->pending_zwpoll_reqs;
				} else {
					scene_ptr->pending_report_reqs = 0;
				}
				if(init_data->pending_report_reqs > action_ptr->pending_zwpoll_reqs) {
					init_data->pending_report_reqs -= action_ptr->pending_zwpoll_reqs;
				} else {
					init_data->pending_report_reqs = 0;
				}
				action_ptr->pending_zwpoll_reqs = 0;
				zwscn_mutex_unlock(init_data->mutex_pending_report_status);
				zwscn_status_update_reset_pending_flag(net); //this function locks mutex_pending_report_status
				ZWSCN_MESSAGE_LOG_DEBUG("REPORTCOUNT 6 action r=%u zr=%u\tscene[%d]=%u\tnetwork=%u\n", action_ptr->pending_report_reqs, action_ptr->pending_zwpoll_reqs, scene_ptr->scene->id, scene_ptr->pending_report_reqs, init_data->pending_report_reqs);
			}
		}
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;
}

uint32_t zwscn_zwpoll_get_token(zwscn_action_p action_ptr) {
	uint32_t result = 0;
	
	result = action_ptr->id;
	
	return result;
}

uint16_t zwscn_zwpoll_get_interval(zwscn_action_p action_ptr) {
	return ZWSCN_ZWPOLL_INTERVAL_DEFAULT; //one interval for all CC for now
}

uint16_t zwscn_zwpoll_get_count(zwscn_action_p action_ptr) {
	uint16_t result = ZWSCN_ZWPOLL_COUNT_DEFAULT; 
	
	if(action_ptr == NULL) {
		return result;
	}
	
	result = zwscn_zwave_get_zwpoll_count(action_ptr);
	
	return result;
}
