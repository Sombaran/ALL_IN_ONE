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
@file		zwscn_device_report_cache.c  
  
                Functions for caching the timestamp of the last received device report
  
@author		Arif Mohammand
 
@version	1.0 2014-04-22  Initial release
*/

#include <stdlib.h>
#include <stdint.h>
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
#include "zwscn_device_report_cache.h"
#include "zwscn_util.h"
#include "zwscn_util_zwave.h"
#include "zwscn_util_time.h"


void zwscn_device_report_cache_add(void *net, uint8_t node_id, uint8_t ep_id, uint16_t cls_id, uint8_t cmd, void *report, uint32_t report_size) {
	zwscn_time_t *report_timestamp_old = NULL;
	zwscn_time_t *report_timestamp = NULL;
	uint32_t combo_id = 0;
	zwscn_init_p init_data = NULL;
	void *temp = NULL;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if((net == NULL) || (node_id == 0) || (report == NULL) || (report_size == 0)) {
		goto l_err_args;
	}
	
	report_timestamp = calloc(1, sizeof(zwscn_time_t));
	if(report_timestamp == NULL) {
		goto l_err_memory;
	}
	
	if(zwscn_zwave_get_device_report_timestamp(cls_id, report, report_size, report_timestamp) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	
	combo_id = zwscn_util_make_combo_id(node_id, ep_id, cls_id);
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		goto l_err2;
	}
	
	ZWSCN_MESSAGE_LOG_DEBUG("Adding Device report timestamp for net=%p device id=0x%X.\n", net, combo_id);
	zwscn_mutex_lock(init_data->mutex_device_report_cache);
	//Free the old value if exists
	temp = zwscn_uint_to_pointer(combo_id);
	report_timestamp_old = zwscn_hash_get_value(init_data->hash_device_report_cache, temp); //if value is not present a NULL is returned
	free(report_timestamp_old);
	//insert the new value
	temp = zwscn_uint_to_pointer(combo_id);
	zwscn_hash_insert(init_data->hash_device_report_cache, temp, report_timestamp);
	zwscn_mutex_unlock(init_data->mutex_device_report_cache);
	
	return;
l_err_args:	
l_err_memory:
	return;
l_err:
l_err2:
	free(report_timestamp);
	return;
}

void zwscn_device_report_cache_del(void *net, uint32_t id) {
	zwscn_time_t *report_timestamp = NULL;
	zwscn_init_p init_data = NULL;
	void *temp = NULL;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if((net == NULL) || (id == 0)) {
		goto l_err_args;
	}
	
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		goto l_err;
	}
	
	ZWSCN_MESSAGE_LOG_DEBUG("Deleting Device report timestamp for net=%p device id=0x%X.\n", net, id);
	zwscn_mutex_lock(init_data->mutex_device_report_cache);
	temp = zwscn_uint_to_pointer(id);
	report_timestamp = zwscn_hash_get_value(init_data->hash_device_report_cache, temp);
	zwscn_hash_remove(init_data->hash_device_report_cache, temp);	
	free(report_timestamp);
	zwscn_mutex_unlock(init_data->mutex_device_report_cache);
	
	return;
l_err_args:	
l_err:
	return;
}

ZWSCN_BOOL_T zwscn_device_report_cache_is_fresh(void *net, uint32_t id) {
	ZWSCN_BOOL_T result = ZWSCN_FALSE;
	zwscn_time_t time_now = 0;
	zwscn_time_t *report_timestamp = NULL;
	zwscn_init_p init_data = NULL;
	void *temp = NULL;
	
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	if((net == NULL) || (id == 0)) {
		goto l_err_args;
	}
	
	time_now = ZWSCN_TIME_NOW();
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		goto l_err;
	}
	
	zwscn_mutex_lock(init_data->mutex_device_report_cache);
	temp = zwscn_uint_to_pointer(id);
	report_timestamp = zwscn_hash_get_value(init_data->hash_device_report_cache, temp);
	if(report_timestamp == NULL) {
		ZWSCN_MESSAGE_LOG_DEBUG("Device report timestamp not found in cache for net=%p device id=0x%X\n", net, id);
		goto l_err;
	}
	if((time_now >= (*report_timestamp)) && ((time_now - (*report_timestamp)) < ZWSCN_DEVICE_REPORT_FRESHNESS_TIMEOUT)) {
		ZWSCN_MESSAGE_LOG_DEBUG("Device report timestamp is fresh for net=%p device id=0x%X\n", net, id);
		result = ZWSCN_TRUE;
	} else {
		temp = zwscn_uint_to_pointer(id);
		zwscn_hash_remove(init_data->hash_device_report_cache, temp);
		free(report_timestamp);
		ZWSCN_MESSAGE_LOG_DEBUG("Device report timestamp is NOT fresh for net=%p device id=0x%X. Deleting it...\n", net, id);
	}
	zwscn_mutex_unlock(init_data->mutex_device_report_cache);
	
	return result;
l_err_args:
	return ZWSCN_FALSE;
l_err:
	zwscn_mutex_unlock(init_data->mutex_device_report_cache);
	return ZWSCN_FALSE;
}
