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
@file		zwscn_persistence_security_scene.c - Security Scenes persistent storage
  
                To provide support for persistent storage for security scenes
  
@author		Arif Mohammand
 
@version	1.0 2015-04-07  Initial release
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "zwscn_defines.h"
#include "zwscn_util_hash.h"
#include "zwscn_util_list.h"
#include "zwscn_util_mqueue.h"
#include "zwscn_util_mqueue_message.h"
#include "zwscn_util_thread.h"
#include "zwscn_util_mutex.h"
#include "zwscn_util_rwlock.h"
#include "zwscn_util_timer.h"
#include "zwscn_util_filesys.h"
#include "zwscn_util_mem.h"
#include "zwscn_util_message_log.h"
#include "zwscn_scene_api.h"
#include "zwscn_security_scene_api.h"
#include "zwscn_statelog.h"
#include "zwscn_scene.h"
#include "zwscn_setup_op.h"
#include "zwscn_util.h"
#include "zwscn_util_zwave.h"
#include "zwscn_security_scene.h"
#include "zwscn_persistence.h"
#include "zwscn_persistence_format.h"
#include "zwscn_persistence_format.h"
#include "zwscn_persistence_security_scene.h"
#include "zwscn_scene_schedule.h"
#include "zwscn_security_scene_events.h"
#include "zwscn_xml.h"
#include "zwscn_serializer_functions.h"
#include "zwscn_deserializer_functions.h"

//Prototypes
ZWSCN_STATUS_T	zwscn_persistence_save_security_scene_to_file(const char *filename, zwscn_security_scene_cont_p scene_ptr);
void			zwscn_persistence_save_tag_security_scene(FILE *file, zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr);
void			zwscn_persistence_save_tag_arm_security_event(FILE *file, zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr);
void			zwscn_persistence_save_tag_disarm_security_event(FILE *file, zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr);
void			zwscn_persistence_save_tag_alarm_security_event(FILE *file, zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr);
void			zwscn_persistence_prepare_tag_security_scene(zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr);
void			zwscn_persistence_prepare_tag_arm_security_event(zwscn_tlv_tag_p tag, zwscn_security_event_p action_ptr);
void			zwscn_persistence_prepare_tag_disarm_security_event(zwscn_tlv_tag_p tag, zwscn_security_event_p schedule_ptr);
void			zwscn_persistence_prepare_tag_alarm_security_event(zwscn_tlv_tag_p tag, zwscn_security_event_p event_ptr);

ZWSCN_STATUS_T	zwscn_persistence_load_security_scene_from_file(void *net, const char *filename, zwscn_list_t *out_list);
ZWSCN_STATUS_T	zwscn_persistence_load_security_scene(void *net, FILE *file, zwscn_security_scene_cont_p scene_ptr);
ZWSCN_STATUS_T	zwscn_persistence_load_tag_security_scene(void *net, zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr);
ZWSCN_STATUS_T	zwscn_persistence_load_tag_arm_security_event(void *net, zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr);
ZWSCN_STATUS_T	zwscn_persistence_load_tag_disarm_security_event(void *net, zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr);
ZWSCN_STATUS_T	zwscn_persistence_load_tag_alarm_security_event(void *net, zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr);
ZWSCN_STATUS_T	zwscn_persistence_parse_tag_security_scene(zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr);
ZWSCN_STATUS_T	zwscn_persistence_parse_tag_generic_security_event(zwscn_tlv_tag_p tag, uint8_t **subtagbuf_ptr, zwscn_security_event_p event_ptr);
ZWSCN_STATUS_T	zwscn_persistence_parse_tag_arm_security_event(zwscn_tlv_tag_p tag, zwscn_security_event_p event_ptr);
ZWSCN_STATUS_T	zwscn_persistence_parse_tag_disarm_security_event(zwscn_tlv_tag_p tag, zwscn_security_event_p event_ptr);
ZWSCN_STATUS_T	zwscn_persistence_parse_tag_alarm_security_event(zwscn_tlv_tag_p tag, zwscn_security_event_p event_ptr);

ZWSCN_STATUS_T zwscn_persistence_get_data_dir_valid_items_for_security_scenes(void *net, zwscn_list_t *out_list);
ZWSCN_STATUS_T	zwscn_persistence_get_absolute_filename_from_security_scene_id(void *net, uint32_t scene_id, char **out_absolute_filename);

ZWSCN_STATUS_T zwscn_persistence_save_security_scene(void *net, zwscn_security_scene_cont_p scene_ptr) {
	char scene_filename[ZWSCN_FILENAME_PATH_LEN]={0};
	char *ptr_filename = NULL;
	uint32_t scene_id;
	ZWSCN_STATUS_T result = ZWSCN_ERR_NONE;
	
	if ((net == NULL) || (scene_ptr == NULL) || (scene_ptr->scene == NULL)) {
		goto l_err_args;
	}
	scene_id = scene_ptr->scene->id;
	zwscn_message_log_debug("Save Security Scene net=%p id=%d\n", net, scene_id);
	
	ptr_filename = scene_filename;
	if(zwscn_persistence_get_absolute_filename_from_security_scene_id(net, scene_id, &ptr_filename) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	if(scene_filename[0] == 0) { //empty string
		goto l_err;
	}
	
	
	result = zwscn_persistence_save_security_scene_to_file(scene_filename, scene_ptr);
	
	return result;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;
}

void zwscn_persistence_save_security_scene_to_xml_file(const char* filename, zwscn_security_scene_cont_p scene_ptr) {
	zwscn_serializer_functions_register_all_types();
	xml_writer_settings_t settings;
	settings.encoding = XML_ENCODING_ASCII;
	settings.indent = 1;
	settings.new_line_on_attributes = 0;
	settings.omit_xml_decleration = 0;
	xml_writer_t* writer = xml_create_writer(filename, settings);
	if (writer) {
		xml_writer_write_start_element(writer, "zwave");
		xml_writer_write_attribute(writer, "version", "1.0");
		XML_SERIALIZER_SERIALIZE(writer, zwscn_security_scene_cont_t, scene_ptr);
		xml_writer_write_end_element(writer);
		xml_writer_close(writer);
	}
}

ZWSCN_STATUS_T zwscn_persistence_save_security_scene_to_file(const char *filename, zwscn_security_scene_cont_p scene_ptr) {
#if ZWSCN_PERSISTENCE_ENABLE_XML
	zwscn_persistence_save_security_scene_to_xml_file(filename, scene_ptr);
	return ZWSCN_ERR_NONE;
#else
	FILE *file;
	zwscn_tlv_tag_p tag;

	if((filename == NULL) || (scene_ptr == NULL)) {
		goto l_err_args;
	}

	file = zwscn_persistence_open_file(filename, ZWSCN_FILEMODE_WRITE);
	if (file == NULL) {
		zwscn_message_log_critical("[Error] SCENE_ERROR_FILE_OPEN\n");
		goto l_err_file_open;
	}

	//Allocate memory for tag
    tag = (zwscn_tlv_tag_p)calloc(1, sizeof(zwscn_tlv_tag_t) + ZWSCN_TLV_SUBTAGS_SIZE_MAX);
	if (!tag) {
        goto l_err_memory;
    }
	tag->len = 0; //though calloc takes care of initializing the value to 0, setting the tag len explicitly for a reminder

	zwscn_persistence_save_tag_security_scene(file, tag, scene_ptr);
	zwscn_persistence_save_tag_arm_security_event(file, tag, scene_ptr);
	zwscn_persistence_save_tag_disarm_security_event(file, tag, scene_ptr);
	zwscn_persistence_save_tag_alarm_security_event(file, tag, scene_ptr);

	free(tag);
	fclose(file);

	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err_file_open:
	return ZWSCN_ERR_FILE_OPEN;
l_err_memory:
	fclose(file);
	return ZWSCN_ERR_MEMORY;
#endif
}

void zwscn_persistence_save_tag_security_scene(FILE *file, zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr) {
	zwscn_persistence_prepare_tag_security_scene(tag, scene_ptr);
	zwscn_message_log_debug("writing tag SECURITY SECNE...\n");
	zwscn_message_log_debug("SECURITY SECNE id=%u name=%s active=%u\n", scene_ptr->scene->id, scene_ptr->scene->name, scene_ptr->scene->active);
	zwscn_tlv_tag_wr(file, tag);
}

void zwscn_persistence_save_tag_arm_security_event(FILE *file, zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr) {
	zwscn_list_t event_list;
	zwscn_security_event_p event_ptr;
	int count=0;
	int i;
	
	event_list = scene_ptr->list_arm_events;
	count = zwscn_list_size(event_list); //If list is NULL, count should be 0
	for(i=0; i < count; ++i) {
		event_ptr = zwscn_list_get_by_index(event_list, i);
		//zwscn_message_log_debug("writing tag event id=%d...\n", event_ptr->id);
		zwscn_persistence_prepare_tag_arm_security_event(tag, event_ptr);
		zwscn_tlv_tag_wr(file, tag);
	}
	
	event_list = scene_ptr->list_arm_events_invalid;
	count = zwscn_list_size(event_list); //If list is NULL, count should be 0
	for(i=0; i < count; ++i) {
		event_ptr = zwscn_list_get_by_index(event_list, i);
		//zwscn_message_log_debug("writing tag event id=%d...\n", event_ptr->id);
		zwscn_persistence_prepare_tag_arm_security_event(tag, event_ptr);
		zwscn_tlv_tag_wr(file, tag);
	}
}

void zwscn_persistence_save_tag_disarm_security_event(FILE *file, zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr) {
	zwscn_list_t event_list;
	zwscn_security_event_p event_ptr;
	int count=0;
	int i;
	
	event_list = scene_ptr->list_disarm_events;
	count = zwscn_list_size(event_list); //If list is NULL, count should be 0
	for(i=0; i < count; ++i) {
		event_ptr = zwscn_list_get_by_index(event_list, i);
		//zwscn_message_log_debug("writing tag event id=%d...\n", event_ptr->id);
		zwscn_persistence_prepare_tag_disarm_security_event(tag, event_ptr);
		zwscn_tlv_tag_wr(file, tag);
	}
	
	event_list = scene_ptr->list_disarm_events_invalid;
	count = zwscn_list_size(event_list); //If list is NULL, count should be 0
	for(i=0; i < count; ++i) {
		event_ptr = zwscn_list_get_by_index(event_list, i);
		//zwscn_message_log_debug("writing tag event id=%d...\n", event_ptr->id);
		zwscn_persistence_prepare_tag_disarm_security_event(tag, event_ptr);
		zwscn_tlv_tag_wr(file, tag);
	}
}

void zwscn_persistence_save_tag_alarm_security_event(FILE *file, zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr) {
	zwscn_list_t event_list;
	zwscn_security_event_p event_ptr;
	int count=0;
	int i;
	
	event_list = scene_ptr->list_alarm_events;
	count = zwscn_list_size(event_list); //If list is NULL, count should be 0
	for(i=0; i < count; ++i) {
		event_ptr = zwscn_list_get_by_index(event_list, i);
		//zwscn_message_log_debug("writing tag event id=%d...\n", event_ptr->id);
		zwscn_persistence_prepare_tag_alarm_security_event(tag, event_ptr);
		zwscn_tlv_tag_wr(file, tag);
	}
	
	event_list = scene_ptr->list_alarm_events_invalid;
	count = zwscn_list_size(event_list); //If list is NULL, count should be 0
	for(i=0; i < count; ++i) {
		event_ptr = zwscn_list_get_by_index(event_list, i);
		//zwscn_message_log_debug("writing tag event id=%d...\n", event_ptr->id);
		zwscn_persistence_prepare_tag_alarm_security_event(tag, event_ptr);
		zwscn_tlv_tag_wr(file, tag);
	}
}


void zwscn_persistence_prepare_tag_security_scene(zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr) {
	uint8_t *subtagbuf;
	uint16_t strlength;
	
	tag->id = ZWSCN_TAG_SECURITY_SCENE;
	tag->len = 0;
	subtagbuf = tag->data;
	zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_SCENE_ID, scene_ptr->scene->id, &subtagbuf, &tag->len);
	strlength = strlen(scene_ptr->scene->name);
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_NAME length=%d value=%s\n", strlength, scene_ptr->scene->name);
	zwscn_tlv_subtag_wr(ZWSCN_SUBTAG_SCENE_NAME, (uint8_t *) scene_ptr->scene->name, strlength, &subtagbuf, &tag->len);
	
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_ACTIVE value=%u\n", scene_ptr->scene->active);
	zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_SCENE_ACTIVE, scene_ptr->scene->active, &subtagbuf, &tag->len);
	
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_IS_ARMED value=%u\n", scene_ptr->scene->is_armed);
	zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_SCENE_IS_ARMED, scene_ptr->scene->is_armed, &subtagbuf, &tag->len);
	
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_IS_ALARMED value=%u\n", scene_ptr->scene->is_alarmed);
	zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_SCENE_IS_ALARMED, scene_ptr->scene->is_alarmed, &subtagbuf, &tag->len);
	
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_ID_AT_ARM value=%u\n", scene_ptr->scene->scene_id_at_arm);
	zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_SCENE_ID_AT_ARM, scene_ptr->scene->scene_id_at_arm, &subtagbuf, &tag->len);
	
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_ID_AT_DISARM value=%u\n", scene_ptr->scene->scene_id_at_disarm);
	zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_SCENE_ID_AT_DISARM, scene_ptr->scene->scene_id_at_disarm, &subtagbuf, &tag->len);
	
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_ID_AT_ALARM value=%u\n", scene_ptr->scene->scene_id_at_alarm);
	zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_SCENE_ID_AT_ALARM, scene_ptr->scene->scene_id_at_alarm, &subtagbuf, &tag->len);
	
//	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_LATRIGGER_TYPE value=%u\n", scene_ptr->scene->latrigger_type);
//	zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_SCENE_LATRIGGER_TYPE, scene_ptr->scene->latrigger_type, &subtagbuf, &tag->len);
//	
//	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_LATRIGGER_EVENT_ID value=%u\n", scene_ptr->scene->latrigger_event_id);
//	zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_SCENE_LATRIGGER_EVENT_ID, scene_ptr->scene->latrigger_event_id, &subtagbuf, &tag->len);
//	
//	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_LATRIGGER_TIME value=%llu\n", scene_ptr->scene->latrigger_time);
//	zwscn_tlv_subtag_wr64(ZWSCN_SUBTAG_SCENE_LATRIGGER_TIME, scene_ptr->scene->latrigger_time, &subtagbuf, &tag->len);
	
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_IS_NOTIFICATION_ON value=%u\n", scene_ptr->scene->is_notification_on);
	zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_SCENE_IS_NOTIFICATION_ON, scene_ptr->scene->is_notification_on, &subtagbuf, &tag->len);
	
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_IS_NOTIFICATION_BY_SMS_ON value=%u\n", scene_ptr->scene->is_notification_by_sms_on);
	zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_SCENE_IS_NOTIFICATION_BY_SMS_ON, scene_ptr->scene->is_notification_by_sms_on, &subtagbuf, &tag->len);
	
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_IS_NOTIFICATION_BY_EMAIL_ON value=%u\n", scene_ptr->scene->is_notification_by_email_on);
	zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_SCENE_IS_NOTIFICATION_BY_EMAIL_ON, scene_ptr->scene->is_notification_by_email_on, &subtagbuf, &tag->len);
	
	strlength = strlen(scene_ptr->scene->notification_sms_number);
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_NOTIFICATION_SMS_NUMBER length=%d value=%s\n", strlength, scene_ptr->scene->notification_sms_number);
	zwscn_tlv_subtag_wr(ZWSCN_SUBTAG_SCENE_NOTIFICATION_SMS_NUMBER, (uint8_t *) scene_ptr->scene->notification_sms_number, strlength, &subtagbuf, &tag->len); 
	
	strlength = strlen(scene_ptr->scene->notification_email);
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_NOTIFICATION_EMAIL length=%d value=%s\n", strlength, scene_ptr->scene->notification_email);
	zwscn_tlv_subtag_wr(ZWSCN_SUBTAG_SCENE_NOTIFICATION_EMAIL, (uint8_t *) scene_ptr->scene->notification_email, strlength, &subtagbuf, &tag->len); 
	
	//Last trigger arm
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_LT_ARM_TYPE value=%u\n", scene_ptr->scene->lt_arm.type);
	zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_SCENE_LT_ARM_TYPE, scene_ptr->scene->lt_arm.type, &subtagbuf, &tag->len);
	
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_LT_ARM_TIME value=%llu\n", scene_ptr->scene->lt_arm.time);
	zwscn_tlv_subtag_wr64(ZWSCN_SUBTAG_SCENE_LT_ARM_TIME, scene_ptr->scene->lt_arm.time, &subtagbuf, &tag->len);
	
	if((scene_ptr->scene->lt_arm.type == ZWSCN_STATELOG_TRIGGER_EVENT) && (scene_ptr->scene->lt_arm.event != NULL)) {
		zwscn_security_event_p event_ptr = scene_ptr->scene->lt_arm.event;
		
		zwscn_message_log_debug("PERSISTENCE write LT_ARM EVENT type=%u, id=%u, cmd=%u, args_size=%u\n", event_ptr->type, event_ptr->id, event_ptr->cmd, event_ptr->device_args_size);
		zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_EVENT_TYPE, event_ptr->type, &subtagbuf, &tag->len);
		zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_EVENT_ID, event_ptr->id, &subtagbuf, &tag->len);
		zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_EVENT_CMD, event_ptr->cmd, &subtagbuf, &tag->len);
		if(event_ptr->device_args_size > 0) {
			zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE, event_ptr->device_args_size, &subtagbuf, &tag->len);
			zwscn_tlv_subtag_wr(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS, (uint8_t *) event_ptr->device_args, event_ptr->device_args_size, &subtagbuf, &tag->len); 
		}
	}
	
	//Last trigger disarm
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_LT_DISARM_TYPE value=%u\n", scene_ptr->scene->lt_disarm.type);
	zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_SCENE_LT_DISARM_TYPE, scene_ptr->scene->lt_disarm.type, &subtagbuf, &tag->len);
	
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_LT_DISARM_TIME value=%llu\n", scene_ptr->scene->lt_disarm.time);
	zwscn_tlv_subtag_wr64(ZWSCN_SUBTAG_SCENE_LT_DISARM_TIME, scene_ptr->scene->lt_disarm.time, &subtagbuf, &tag->len);
	
	if((scene_ptr->scene->lt_disarm.type == ZWSCN_STATELOG_TRIGGER_EVENT) && (scene_ptr->scene->lt_disarm.event != NULL)) {
		zwscn_security_event_p event_ptr = scene_ptr->scene->lt_disarm.event;
		
		zwscn_message_log_debug("PERSISTENCE write LT_DISARM EVENT type=%u, id=%u, cmd=%u, args_size=%u\n", event_ptr->type, event_ptr->id, event_ptr->cmd, event_ptr->device_args_size);
		zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_EVENT_TYPE, event_ptr->type, &subtagbuf, &tag->len);
		zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_EVENT_ID, event_ptr->id, &subtagbuf, &tag->len);
		zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_EVENT_CMD, event_ptr->cmd, &subtagbuf, &tag->len);
		if(event_ptr->device_args_size > 0) {
			zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE, event_ptr->device_args_size, &subtagbuf, &tag->len);
			zwscn_tlv_subtag_wr(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS, (uint8_t *) event_ptr->device_args, event_ptr->device_args_size, &subtagbuf, &tag->len); 
		}
	}
	
	//Last trigger alarm_on
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_LT_ALARM_ON_TYPE value=%u\n", scene_ptr->scene->lt_alarm_on.type);
	zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_SCENE_LT_ALARM_ON_TYPE, scene_ptr->scene->lt_alarm_on.type, &subtagbuf, &tag->len);
	
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_LT_ALARM_ON_TIME value=%llu\n", scene_ptr->scene->lt_alarm_on.time);
	zwscn_tlv_subtag_wr64(ZWSCN_SUBTAG_SCENE_LT_ALARM_ON_TIME, scene_ptr->scene->lt_alarm_on.time, &subtagbuf, &tag->len);
	
	if((scene_ptr->scene->lt_alarm_on.type == ZWSCN_STATELOG_TRIGGER_EVENT) && (scene_ptr->scene->lt_alarm_on.event != NULL)) {
		zwscn_security_event_p event_ptr = scene_ptr->scene->lt_alarm_on.event;
		
		zwscn_message_log_debug("PERSISTENCE write LT_ALARM_ON EVENT type=%u, id=%u, cmd=%u, args_size=%u\n", event_ptr->type, event_ptr->id, event_ptr->cmd, event_ptr->device_args_size);
		zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_EVENT_TYPE, event_ptr->type, &subtagbuf, &tag->len);
		zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_EVENT_ID, event_ptr->id, &subtagbuf, &tag->len);
		zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_EVENT_CMD, event_ptr->cmd, &subtagbuf, &tag->len);
		if(event_ptr->device_args_size > 0) {
			zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE, event_ptr->device_args_size, &subtagbuf, &tag->len);
			zwscn_tlv_subtag_wr(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS, (uint8_t *) event_ptr->device_args, event_ptr->device_args_size, &subtagbuf, &tag->len); 
		}
	}
	
	//Last trigger alarm_off
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_LT_ALARM_OFF_TYPE value=%u\n", scene_ptr->scene->lt_alarm_off.type);
	zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_SCENE_LT_ALARM_OFF_TYPE, scene_ptr->scene->lt_alarm_off.type, &subtagbuf, &tag->len);
	
	zwscn_message_log_debug("PERSISTENCE write ZWSCN_SUBTAG_SCENE_LT_ALARM_OFF_TIME value=%llu\n", scene_ptr->scene->lt_alarm_off.time);
	zwscn_tlv_subtag_wr64(ZWSCN_SUBTAG_SCENE_LT_ALARM_OFF_TIME, scene_ptr->scene->lt_alarm_off.time, &subtagbuf, &tag->len);
	
	if((scene_ptr->scene->lt_alarm_off.type == ZWSCN_STATELOG_TRIGGER_EVENT) && (scene_ptr->scene->lt_alarm_off.event != NULL)) {
		zwscn_security_event_p event_ptr = scene_ptr->scene->lt_alarm_off.event;
		
		zwscn_message_log_debug("PERSISTENCE write LT_ALARM_OFF EVENT type=%u, id=%u, cmd=%u, args_size=%u\n", event_ptr->type, event_ptr->id, event_ptr->cmd, event_ptr->device_args_size);
		zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_EVENT_TYPE, event_ptr->type, &subtagbuf, &tag->len);
		zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_EVENT_ID, event_ptr->id, &subtagbuf, &tag->len);
		zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_EVENT_CMD, event_ptr->cmd, &subtagbuf, &tag->len);
		if(event_ptr->device_args_size > 0) {
			zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE, event_ptr->device_args_size, &subtagbuf, &tag->len);
			zwscn_tlv_subtag_wr(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS, (uint8_t *) event_ptr->device_args, event_ptr->device_args_size, &subtagbuf, &tag->len); 
		}
	}
}

void zwscn_persistence_prepare_tag_arm_security_event(zwscn_tlv_tag_p tag, zwscn_security_event_p event_ptr) {
	uint8_t *subtagbuf;
//	uint16_t strlength;
	
	tag->id = ZWSCN_TAG_ARM_EVENT;
	tag->len = 0;
	subtagbuf = tag->data;
	zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_EVENT_TYPE, event_ptr->type, &subtagbuf, &tag->len);
	zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_EVENT_ID, event_ptr->id, &subtagbuf, &tag->len);
	zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_EVENT_CMD, event_ptr->cmd, &subtagbuf, &tag->len);
	if(event_ptr->device_args_size > 0) {
		zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE, event_ptr->device_args_size, &subtagbuf, &tag->len);
		zwscn_tlv_subtag_wr(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS, (uint8_t *) event_ptr->device_args, event_ptr->device_args_size, &subtagbuf, &tag->len); 
	}
}

void zwscn_persistence_prepare_tag_disarm_security_event(zwscn_tlv_tag_p tag, zwscn_security_event_p event_ptr) {
	uint8_t *subtagbuf;
//	uint16_t strlength;
	
	tag->id = ZWSCN_TAG_DISARM_EVENT;
	tag->len = 0;
	subtagbuf = tag->data;
	zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_EVENT_TYPE, event_ptr->type, &subtagbuf, &tag->len);
	zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_EVENT_ID, event_ptr->id, &subtagbuf, &tag->len);
	zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_EVENT_CMD, event_ptr->cmd, &subtagbuf, &tag->len);
	if(event_ptr->device_args_size > 0) {
		zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE, event_ptr->device_args_size, &subtagbuf, &tag->len);
		zwscn_tlv_subtag_wr(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS, (uint8_t *) event_ptr->device_args, event_ptr->device_args_size, &subtagbuf, &tag->len); 
	}
}

void zwscn_persistence_prepare_tag_alarm_security_event(zwscn_tlv_tag_p tag, zwscn_security_event_p event_ptr) {
	uint8_t *subtagbuf;
//	uint16_t strlength;
	
	tag->id = ZWSCN_TAG_ALARM_EVENT;
	tag->len = 0;
	subtagbuf = tag->data;
	zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_EVENT_TYPE, event_ptr->type, &subtagbuf, &tag->len);
	zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_EVENT_ID, event_ptr->id, &subtagbuf, &tag->len);
	zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_EVENT_CMD, event_ptr->cmd, &subtagbuf, &tag->len);
	if(event_ptr->device_args_size > 0) {
		zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE, event_ptr->device_args_size, &subtagbuf, &tag->len);
		zwscn_tlv_subtag_wr(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS, (uint8_t *) event_ptr->device_args, event_ptr->device_args_size, &subtagbuf, &tag->len); 
	}
}
/*************************************************/

ZWSCN_STATUS_T zwscn_persistence_load_all_security_scenes(void *net, zwscn_list_t *out_list) {
	zwscn_list_t file_list = NULL;
	ZWSCN_STATUS_T result;
	int count;
	int i;
	const char *filename = NULL;
	char scene_filename[ZWSCN_FILENAME_PATH_LEN] = {0};
	char *scene_filename_ptr = NULL;
	 
	if(net == NULL) {
		goto l_err_args;
	}
	
	result = zwscn_persistence_get_data_dir_valid_items_for_security_scenes(net, &file_list);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("zwscn_persistence_get_data_dir_valid_items_for_security_scenes  returned %d\n", result);
		goto l_err;
	}
	
	//zwscn_persistence_print_file_list(file_list);
	
	scene_filename_ptr = scene_filename;
	count = zwscn_list_size(file_list); //If list is NULL, count should be 0
	for(i=0; i < count; ++i) {
		memset(scene_filename, 0, sizeof(scene_filename)); //zero out the temp buffer
		filename = zwscn_list_get_by_index(file_list, i);
		zwscn_message_log_debug("Loading: %s\n", filename);
		if(zwscn_persistence_get_absolute_filename(net, filename, &scene_filename_ptr) != ZWSCN_ERR_NONE) { //using scene_filename_ptr instead of scene_filename because the funtion takes char **
			goto l_err;
		}
		if(scene_filename[0] == 0) { //empty string
			goto l_err;
		}
		result = zwscn_persistence_load_security_scene_from_file(net, scene_filename, out_list);
		if(result != ZWSCN_ERR_NONE) {
			zwscn_message_log_warning("zwscn_load_security_scene_from_file returned %d\n", result);
			continue; //try to load other files
		}
	}
	
	zwscn_list_free(&file_list); //delete all items and free memory their memory

	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;
}


ZWSCN_STATUS_T zwscn_pesistence_load_security_scene_from_xml_file(const char* filename, void* net) {
    zwscn_security_scene_cont_p scene_ptr;
    ZWSCN_STATUS_T result;

    if ((net == NULL) || (filename == NULL)) goto l_err_args;

    scene_ptr = calloc(1, sizeof(zwscn_security_scene_cont_t));

    if (!scene_ptr) goto l_err_memory;

    scene_ptr->net = net;
    zwscn_deserializer_functions_register_all_types();
    xml_reader_t* reader = xml_reader_new(filename);
    result = XML_DESERIALIZER_DESERIALIZE(reader, zwscn_security_scene_cont_t, scene_ptr);
    xml_reader_close(reader);

    if (result != ZWSCN_ERR_NONE) {
        zwscn_free_scene_lists(scene_ptr);
        return result;
    }

    return ZWSCN_ERR_NONE;
    l_err_args: return ZWSCN_ERR_INVALID_ARGS;
    l_err_memory: return ZWSCN_ERR_MEMORY;
}

ZWSCN_STATUS_T zwscn_persistence_load_security_scene_from_file(void *net, const char *filename, zwscn_list_t *out_list) {
#if ZWSCN_PERSISTENCE_ENABLE_XML
    return zwscn_pesistence_load_security_scene_from_xml_file(filename, net);
#else
	FILE * file;
	zwscn_security_scene_cont_p scene_ptr;
	ZWSCN_STATUS_T result;

	if((net == NULL) || (filename == NULL)) {
		goto l_err_args;
	}

	file = zwscn_persistence_open_file(filename, ZWSCN_FILEMODE_READ);
	if (file == NULL) {
		zwscn_message_log_critical("SCENE_ERROR_FILE_OPEN\n");
		goto l_err_file_open;
	}
	//zwscn_message_log_debug("File: %s opened\n", filename);

	scene_ptr = (zwscn_security_scene_cont_p) calloc(1, sizeof(zwscn_security_scene_cont_t));
	if(!scene_ptr) {
		goto l_err_memory;
	}
	//scene_init_security_scene_ptr(scene_ptr);
	scene_ptr->net = net;
	//TODO init other fields

	result = zwscn_persistence_load_security_scene(net, file, scene_ptr);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_warning("Error code (%d) returned by zwscn_load_security_scene\n", result);
		goto l_err;
	}

	fclose(file);

	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err_file_open:
	return ZWSCN_ERR_FILE_OPEN;
l_err_memory:
	fclose(file);
	return ZWSCN_ERR_MEMORY;
//l_err_memory_mutex:
//	free(scene_ptr);
//	fclose(file);
//	return ZWSCN_ERR_MEMORY;
//l_err_list_full:
//	free(scene_ptr);
//	fclose(file);
//	return ZWSCN_ERR_LIST_FULL;
l_err:
	free(scene_ptr);
	fclose(file);
	return ZWSCN_ERR_GENERIC;
#endif
}

ZWSCN_STATUS_T zwscn_persistence_load_security_scene(void *net, FILE *file, zwscn_security_scene_cont_p scene_ptr) {
	int32_t result;
	zwscn_tlv_tag_p tag;
	ZWSCN_STATUS_T function_result;
	zwscn_list_t *scene_list;
	int count;
	zwscn_init_p init_data;
	
	if((net == NULL) || (file == NULL) || (scene_ptr == NULL)) {
		goto l_err_args;
	}
	
	tag = (zwscn_tlv_tag_p) calloc(1, sizeof (zwscn_tlv_tag_t) + ZWSCN_TLV_SUBTAGS_SIZE_MAX);
	if (!tag) {
		goto l_err_memory;
	}
	tag->len = ZWSCN_TLV_SUBTAGS_SIZE_MAX; //while reading, tag len must be initialized 

	while ((result = zwscn_tlv_tag_rd(file, tag)) == ZWSCN_TLV_ERR_NONE) {
		switch (tag->id) {
			case ZWSCN_TAG_SECURITY_SCENE:
				zwscn_message_log_debug("Found Tag=SECURITY SCENE\n");
				function_result = zwscn_persistence_load_tag_security_scene(net, tag, scene_ptr);
				if(function_result != ZWSCN_ERR_NONE) {
					zwscn_free_security_scene_lists(scene_ptr);
					goto l_err;
				}
				break;
			case ZWSCN_TAG_ARM_EVENT:
				zwscn_message_log_debug("Found Tag=ARM EVENT\n");
				zwscn_persistence_load_tag_arm_security_event(net, tag, scene_ptr);
				break;
			case ZWSCN_TAG_DISARM_EVENT:
				zwscn_message_log_debug("Found Tag=DISARM EVENT\n");
				zwscn_persistence_load_tag_disarm_security_event(net, tag, scene_ptr);
				break;
			case ZWSCN_TAG_ALARM_EVENT:
				zwscn_message_log_debug("Found Tag=ALARM EVENT\n");
				zwscn_persistence_load_tag_alarm_security_event(net, tag, scene_ptr);
				break;
			default:
				zwscn_message_log_debug("Found Unknown Tag=%d\n", tag->id);
				break;
		}
		memset(tag, 0, (sizeof (zwscn_tlv_tag_t) + ZWSCN_TLV_SUBTAGS_SIZE_MAX)); //zero out the tag variable as we are going to reuse it
	}
	
	free(tag);
	
	if(scene_ptr->scene == NULL) {
		zwscn_free_security_scene_lists(scene_ptr);
		goto l_err_2;
	}

#ifndef ZWSCN_LOAD_SECURITY_SCENE_WITH_NO_ALARM_EVENTS
	count = zwscn_list_size(scene_ptr->list_alarm_events);
	zwscn_message_log_debug("Scene id=%d has %d alarm events loaded\n", scene_ptr->scene->id, count);
	if(count <= 0) { //There is no action in this Scene
		init_data = zwscn_get_init_data(net);
		if(init_data == NULL) {
			goto l_err_2;
		}
		scene_list = &(init_data->list_security_scenes);
		(*scene_list) = zwscn_list_remove((*scene_list), scene_ptr); //remove the Scene from the list as it was added to the list already otherwise we'd have had an error earlier
		
		count = zwscn_list_size(scene_ptr->list_alarm_events_invalid);
		zwscn_message_log_debug("Scene id=%d has %d invalid alarm events\n", scene_ptr->scene->id, count);
		if(count > 0) { 											//it has some invalid actions, so it should be moved to invalid scene list instead of unloading from the memory
			scene_list = &(init_data->list_security_scenes_invalid);
			(*scene_list) = zwscn_list_add((*scene_list), scene_ptr); 
			zwscn_message_log_debug("Moved Security Scene id=%d to invalid scene list\n", scene_ptr->scene->id);
		} else {
			zwscn_free_security_scene(scene_ptr);							//free all the memory allocated by the Scene and its children
			zwscn_message_log_debug("Unloaded Security Scene without any alarm events\n");
			//Continue as normal without throwing an error because if an error is thrown the calling function will try to free scene_ptr which we have already freed here
		}
	}
	
#endif
	
	return ZWSCN_ERR_NONE; 
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err_memory:
	return ZWSCN_ERR_MEMORY;
l_err:
	free(tag);
	//return ZWSCN_ERR_GENERIC;
	return function_result;
l_err_2:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_persistence_load_tag_security_scene(void *net, zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr) {
	zwscn_list_t *scene_list;
	uint32_t scene_global_id;
	ZWSCN_STATUS_T result;
	
	scene_ptr->scene = calloc(1, sizeof(zwscn_security_scene_t));
	if(scene_ptr->scene == NULL) {
		goto l_err_memory;
	}
	
	result = zwscn_persistence_parse_tag_security_scene(tag, scene_ptr);
	if(result != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	if((scene_ptr->scene->is_notification_by_sms_on) && (zwscn_util_sms_number_is_valid(scene_ptr->scene->notification_sms_number, ZWSCN_TRUE) != ZWSCN_TRUE)) {
		ZWSCN_MESSAGE_LOG_DEBUG("SMS NUMBER: %s is invalid\n", scene_ptr->scene->notification_sms_number);
		goto l_err_invalid_data;
	}
	
	if((scene_ptr->scene->is_notification_by_email_on) && (zwscn_util_email_is_valid(scene_ptr->scene->notification_email, ZWSCN_TRUE) != ZWSCN_TRUE)) {
		ZWSCN_MESSAGE_LOG_DEBUG("Email: %s is invalid\n", scene_ptr->scene->notification_email);
		goto l_err_invalid_data;
	}

	scene_global_id = zwscn_get_security_scene_id_pool_index(net);
	if(scene_ptr->scene->id > scene_global_id) {
		zwscn_set_security_scene_id_pool_index(net, scene_ptr->scene->id);
	}
	
	if(zwscn_find_security_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	if(zwscn_is_list_full_security_scene(net, (*scene_list))) {
		goto l_err_list_full;
	}
	if(((*scene_list) = zwscn_list_add((*scene_list), scene_ptr)) == NULL) {
		goto l_err;
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_ADD, scene_ptr->scene->id);
	
	return ZWSCN_ERR_NONE;
l_err_memory:
	return ZWSCN_ERR_MEMORY;
l_err_list_full:
	zwscn_free_security_scene_struct(scene_ptr->scene);
	scene_ptr->scene = NULL;
	return ZWSCN_ERR_LIST_FULL;
l_err_invalid_data:
	zwscn_free_security_scene_struct(scene_ptr->scene);
	scene_ptr->scene = NULL;
	return ZWSCN_ERR_INVALID_DATA;
l_err:
	zwscn_free_security_scene_struct(scene_ptr->scene);
	scene_ptr->scene = NULL;
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_persistence_load_tag_arm_security_event(void *net, zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr) {
	zwscn_list_t event_list;
	zwscn_security_event_p event_ptr;
	zwscn_security_event_p event_ptr_temp;
	int32_t result;
	uint8_t node_id, ep_id;
	uint16_t cls_id;
	zwscn_init_p init_data;
	uint32_t scene_id;
	
	if((net == NULL) || (scene_ptr == NULL)) {
		goto l_err_args;
	}
	event_ptr = (zwscn_security_event_p) calloc(1, sizeof (zwscn_security_event_t));
	if (event_ptr == NULL) {
		goto l_err_memory;
	}
	result = zwscn_persistence_parse_tag_arm_security_event(tag, event_ptr);
	if(result != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	cls_id = zwscn_util_combo_id_to_cls_id(event_ptr->id);
	if(zwscn_zwave_is_cc_for_arm_security_event(cls_id, event_ptr->cmd) != ZWSCN_TRUE) {
		zwscn_message_log_warning("Z-Wave Command Class 0x%x is not valid for Arm events\n", cls_id);
		goto l_err_device_invalid;
	}
	scene_id = scene_ptr->scene->id;
#if ZWSCN_CHECK_DUPLICATE_IN_SAME_SECURITY_EVENSTS
	if(zwscn_find_arm_security_event(net, scene_id, event_ptr->id, &event_ptr_temp) == ZWSCN_ERR_NONE) { //Event exists with the same id
		goto l_err_duplicate_id;
	}
#endif
#if ZWSCN_CHECK_DUPLICATE_IN_OTHER_SECURITY_EVENSTS
	if(zwscn_security_scene_has_same_disarm_event(scene_ptr, event_ptr) == ZWSCN_TRUE) {
		goto l_err_duplicate_id;
	}
	if(zwscn_security_scene_has_same_alarm_event(scene_ptr, event_ptr) == ZWSCN_TRUE) {
		goto l_err_duplicate_id;
	}
#endif
	node_id = zwscn_util_combo_id_to_node_id(event_ptr->id);
	ep_id = zwscn_util_combo_id_to_ep_id(event_ptr->id);
	cls_id = zwscn_util_combo_id_to_cls_id(event_ptr->id);
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->init_input && init_data->init_input->zwscn_fn_is_valid_handler) {
		result = init_data->init_input->zwscn_fn_is_valid_handler(net, ZWSCN_CHECK_NODE_EP_CLS, node_id, ep_id, cls_id);
		if(!result) {
			//goto l_err_device_invalid;
			event_list = scene_ptr->list_arm_events_invalid;
			if((scene_ptr->list_arm_events_invalid = zwscn_list_add(event_list, event_ptr)) == NULL) {
				goto l_err;
			}
		} else {
			event_list = scene_ptr->list_arm_events;
			if(zwscn_is_list_full_arm_security_event(net, event_list)) {
				goto l_err_list_full;
			}
			if((scene_ptr->list_arm_events = zwscn_list_add(event_list, event_ptr)) == NULL) {
				goto l_err;
			}
		}
	} else {
		zwscn_message_log_warning("Function zwscn_fn_is_valid_handler is NULL for net=%p\n", net);
		goto l_err_callback_null;
	}
	if(zwscn_setup_op_request(net, ZWSCN_TYPE_EVENT, event_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_ptr->scene->id);
	
	return ZWSCN_ERR_NONE; 
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err_memory:
	return ZWSCN_ERR_MEMORY;
//l_err_parse_tag:
//	free(event_ptr);
//	return ZWSCN_ERR_INVALID_DATA;
l_err_duplicate_id:
	free(event_ptr->device_args);
	free(event_ptr);
	return ZWSCN_ERR_ITEM_IGNORED;
l_err_list_full:
	free(event_ptr->device_args);
	free(event_ptr);
	return ZWSCN_ERR_LIST_FULL;
l_err:
	free(event_ptr->device_args);
	free(event_ptr);
	return ZWSCN_ERR_GENERIC;
l_err_device_invalid:
	free(event_ptr->device_args);
	free(event_ptr);
	return ZWSCN_ERR_INVALID_DEVICE;
l_err_callback_null:
	free(event_ptr->device_args);
	free(event_ptr);
	return ZWSCN_ERR_CALLBACK_NULL;
}

ZWSCN_STATUS_T zwscn_persistence_load_tag_disarm_security_event(void *net, zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr) {
	zwscn_list_t event_list;
	zwscn_security_event_p event_ptr;
	zwscn_security_event_p event_ptr_temp;
	int32_t result;
	uint8_t node_id, ep_id;
	uint16_t cls_id;
	zwscn_init_p init_data;
	uint32_t scene_id;
	
	if((net == NULL) || (scene_ptr == NULL)) {
		goto l_err_args;
	}
	event_ptr = (zwscn_security_event_p) calloc(1, sizeof (zwscn_security_event_t));
	if (event_ptr == NULL) {
		goto l_err_memory;
	}
	result = zwscn_persistence_parse_tag_disarm_security_event(tag, event_ptr);
	if(result != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	cls_id = zwscn_util_combo_id_to_cls_id(event_ptr->id);
	if(zwscn_zwave_is_cc_for_arm_security_event(cls_id, event_ptr->cmd) != ZWSCN_TRUE) { //Same function works for Arm and Disarm events
		zwscn_message_log_warning("Z-Wave Command Class 0x%x is not valid for Disarm events\n", cls_id);
		goto l_err_device_invalid;
	}
	scene_id = scene_ptr->scene->id;
#if ZWSCN_CHECK_DUPLICATE_IN_SAME_SECURITY_EVENSTS
	if(zwscn_find_disarm_security_event(net, scene_id, event_ptr->id, &event_ptr_temp) == ZWSCN_ERR_NONE) { //Event exists with the same id
		goto l_err_duplicate_id;
	}
#endif
#if ZWSCN_CHECK_DUPLICATE_IN_OTHER_SECURITY_EVENSTS
	if(zwscn_security_scene_has_same_arm_event(scene_ptr, event_ptr) == ZWSCN_TRUE) {
		goto l_err_duplicate_id;
	}
	if(zwscn_security_scene_has_same_alarm_event(scene_ptr, event_ptr) == ZWSCN_TRUE) {
		goto l_err_duplicate_id;
	}
#endif
	node_id = zwscn_util_combo_id_to_node_id(event_ptr->id);
	ep_id = zwscn_util_combo_id_to_ep_id(event_ptr->id);
	cls_id = zwscn_util_combo_id_to_cls_id(event_ptr->id);
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->init_input && init_data->init_input->zwscn_fn_is_valid_handler) {
		result = init_data->init_input->zwscn_fn_is_valid_handler(net, ZWSCN_CHECK_NODE_EP_CLS, node_id, ep_id, cls_id);
		if(!result) {
			//goto l_err_device_invalid;
			event_list = scene_ptr->list_disarm_events_invalid;
			if((scene_ptr->list_disarm_events_invalid = zwscn_list_add(event_list, event_ptr)) == NULL) {
				goto l_err;
			}
		} else {
			event_list = scene_ptr->list_disarm_events;
			if(zwscn_is_list_full_disarm_security_event(net, event_list)) {
				goto l_err_list_full;
			}
			if((scene_ptr->list_disarm_events = zwscn_list_add(event_list, event_ptr)) == NULL) {
				goto l_err;
			}
		}
	} else {
		zwscn_message_log_warning("Function zwscn_fn_is_valid_handler is NULL for net=%p\n", net);
		goto l_err_callback_null;
	}
	if(zwscn_setup_op_request(net, ZWSCN_TYPE_EVENT, event_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_ptr->scene->id);
	
	return ZWSCN_ERR_NONE; 
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err_memory:
	return ZWSCN_ERR_MEMORY;
//l_err_parse_tag:
//	free(event_ptr);
//	return ZWSCN_ERR_INVALID_DATA;
l_err_duplicate_id:
	free(event_ptr->device_args);
	free(event_ptr);
	return ZWSCN_ERR_ITEM_IGNORED;
l_err_list_full:
	free(event_ptr->device_args);
	free(event_ptr);
	return ZWSCN_ERR_LIST_FULL;
l_err:
	free(event_ptr->device_args);
	free(event_ptr);
	return ZWSCN_ERR_GENERIC;
l_err_device_invalid:
	free(event_ptr->device_args);
	free(event_ptr);
	return ZWSCN_ERR_INVALID_DEVICE;
l_err_callback_null:
	free(event_ptr->device_args);
	free(event_ptr);
	return ZWSCN_ERR_CALLBACK_NULL;
}

ZWSCN_STATUS_T zwscn_persistence_load_tag_alarm_security_event(void *net, zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr) {
	zwscn_list_t event_list;
	zwscn_security_event_p event_ptr;
//	zwscn_security_event_p event_ptr_temp;
	int32_t result;
	uint8_t node_id, ep_id;
	uint16_t cls_id;
	zwscn_init_p init_data;
//	uint32_t scene_id;
	
	if((net == NULL) || (scene_ptr == NULL)) {
		goto l_err_args;
	}
	event_ptr = (zwscn_security_event_p) calloc(1, sizeof (zwscn_security_event_t));
	if (event_ptr == NULL) {
		goto l_err_memory;
	}
	result = zwscn_persistence_parse_tag_alarm_security_event(tag, event_ptr);
	if(result != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	cls_id = zwscn_util_combo_id_to_cls_id(event_ptr->id);
	if(zwscn_zwave_is_cc_for_alarm_security_event(cls_id, event_ptr->cmd) != ZWSCN_TRUE) {
		zwscn_message_log_warning("Z-Wave Command Class 0x%x is not valid for Alarm events\n", cls_id);
		goto l_err_device_invalid;
	}
//	scene_id = scene_ptr->scene->id;
#if ZWSCN_CHECK_DUPLICATE_IN_SAME_SECURITY_EVENSTS
	if(zwscn_security_scene_has_same_alarm_event(scene_ptr, event_ptr) == ZWSCN_TRUE) {
		goto l_err_duplicate_id;
	}
#endif
#if ZWSCN_CHECK_DUPLICATE_IN_OTHER_SECURITY_EVENSTS
	if(zwscn_security_scene_has_same_arm_event(scene_ptr, event_ptr) == ZWSCN_TRUE) {
		goto l_err_duplicate_id;
	}
	if(zwscn_security_scene_has_same_disarm_event(scene_ptr, event_ptr) == ZWSCN_TRUE) {
		goto l_err_duplicate_id;
	}
#endif
	node_id = zwscn_util_combo_id_to_node_id(event_ptr->id);
	ep_id = zwscn_util_combo_id_to_ep_id(event_ptr->id);
	cls_id = zwscn_util_combo_id_to_cls_id(event_ptr->id);
	init_data = zwscn_get_init_data(net);
	if(init_data && init_data->init_input && init_data->init_input->zwscn_fn_is_valid_handler) {
		result = init_data->init_input->zwscn_fn_is_valid_handler(net, ZWSCN_CHECK_NODE_EP_CLS, node_id, ep_id, cls_id);
		if(!result) {
			//goto l_err_device_invalid;
			event_list = scene_ptr->list_alarm_events_invalid;
			if((scene_ptr->list_alarm_events_invalid = zwscn_list_add(event_list, event_ptr)) == NULL) {
				goto l_err;
			}
		} else {
			event_list = scene_ptr->list_alarm_events;
			if(zwscn_is_list_full_alarm_security_event(net, event_list)) {
				goto l_err_list_full;
			}
			if((scene_ptr->list_alarm_events = zwscn_list_add(event_list, event_ptr)) == NULL) {
				goto l_err;
			}
		}
	} else {
		zwscn_message_log_warning("Function zwscn_fn_is_valid_handler is NULL for net=%p\n", net);
		goto l_err_callback_null;
	}
	if(zwscn_setup_op_request(net, ZWSCN_TYPE_EVENT, event_ptr) != ZWSCN_ERR_NONE) {
		goto l_err;
	}
	zwscn_security_scene_update_timestamp(scene_ptr);
	zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, scene_ptr->scene->id);
	
	return ZWSCN_ERR_NONE; 
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err_memory:
	return ZWSCN_ERR_MEMORY;
//l_err_parse_tag:
//	free(event_ptr);
//	return ZWSCN_ERR_INVALID_DATA;
l_err_duplicate_id:
	free(event_ptr->device_args);
	free(event_ptr);
	return ZWSCN_ERR_ITEM_IGNORED;
l_err_list_full:
	free(event_ptr->device_args);
	free(event_ptr);
	return ZWSCN_ERR_LIST_FULL;
l_err:
	free(event_ptr->device_args);
	free(event_ptr);
	return ZWSCN_ERR_GENERIC;
l_err_device_invalid:
	free(event_ptr->device_args);
	free(event_ptr);
	return ZWSCN_ERR_INVALID_DEVICE;
l_err_callback_null:
	free(event_ptr->device_args);
	free(event_ptr);
	return ZWSCN_ERR_CALLBACK_NULL;
}

ZWSCN_STATUS_T zwscn_persistence_parse_tag_security_scene(zwscn_tlv_tag_p tag, zwscn_security_scene_cont_p scene_ptr) {
	uint8_t *subtagbuf;
	int32_t result;
	
	subtagbuf = tag->data;
	
	result = zwscn_tlv_subtag_rd32(ZWSCN_SUBTAG_SCENE_ID, &(scene_ptr->scene->id), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_ID\n", result);
		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_ID=%u\n", scene_ptr->scene->id);
	}
	
	result = zwscn_tlv_subtag_rd(ZWSCN_SUBTAG_SCENE_NAME, (uint8_t *) (scene_ptr->scene->name), sizeof(scene_ptr->scene->name), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_NAME\n", result);
		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_NAME=%s\n", scene_ptr->scene->name);
	}
	
	result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_SCENE_ACTIVE, &(scene_ptr->scene->active), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_ACTIVE\n", result);
		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_ACTIVE=%u\n", scene_ptr->scene->active);
	}
	
	result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_SCENE_IS_ARMED, &(scene_ptr->scene->is_armed), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_IS_ARMED\n", result);
		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_IS_ARMED=%u\n", scene_ptr->scene->is_armed);
	}
	
	result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_SCENE_IS_ALARMED, &(scene_ptr->scene->is_alarmed), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_IS_ALARMED\n", result);
		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_IS_ALARMED=%u\n", scene_ptr->scene->is_alarmed);
	}
	
	result = zwscn_tlv_subtag_rd32(ZWSCN_SUBTAG_SCENE_ID_AT_ARM, &(scene_ptr->scene->scene_id_at_arm), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_ID_AT_ARM\n", result);
		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_ID_AT_ARM=%u\n", scene_ptr->scene->scene_id_at_arm);
	}
	
	result = zwscn_tlv_subtag_rd32(ZWSCN_SUBTAG_SCENE_ID_AT_DISARM, &(scene_ptr->scene->scene_id_at_disarm), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_ID_AT_DISARM\n", result);
		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_ID_AT_DISARM=%u\n", scene_ptr->scene->scene_id_at_disarm);
	}
	
	result = zwscn_tlv_subtag_rd32(ZWSCN_SUBTAG_SCENE_ID_AT_ALARM, &(scene_ptr->scene->scene_id_at_alarm), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_ID_AT_ALARM\n", result);
		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_ID_AT_ALARM=%u\n", scene_ptr->scene->scene_id_at_alarm);
	}
	
//	result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_SCENE_LATRIGGER_TYPE, &(scene_ptr->scene->latrigger_type), &subtagbuf, &tag->len);
//	if(result != ZWSCN_TLV_ERR_NONE) {
//		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_LATRIGGER_TYPE\n", result);
//		goto l_err;
//	} else {
//		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_LATRIGGER_TYPE=%u\n", scene_ptr->scene->latrigger_type);
//	}
//	
//	result = zwscn_tlv_subtag_rd32(ZWSCN_SUBTAG_SCENE_LATRIGGER_EVENT_ID, &(scene_ptr->scene->latrigger_event_id), &subtagbuf, &tag->len);
//	if(result != ZWSCN_TLV_ERR_NONE) {
//		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_LATRIGGER_EVENT_ID\n", result);
//		goto l_err;
//	} else {
//		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_LATRIGGER_EVENT_ID=%u\n", scene_ptr->scene->latrigger_event_id);
//	}
//	
//	result = zwscn_tlv_subtag_rd64(ZWSCN_SUBTAG_SCENE_LATRIGGER_TIME, &(scene_ptr->scene->latrigger_time), &subtagbuf, &tag->len);
//	if(result != ZWSCN_TLV_ERR_NONE) {
//		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_LATRIGGER_TIME\n", result);
//		goto l_err;
//	} else {
//		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_LATRIGGER_TIME=%llu\n", scene_ptr->scene->latrigger_time);
//	}
	
	result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_SCENE_IS_NOTIFICATION_ON, &(scene_ptr->scene->is_notification_on), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_IS_NOTIFICATION_ON\n", result);
		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_IS_NOTIFICATION_ON=%u\n", scene_ptr->scene->is_notification_on);
	}
	
	result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_SCENE_IS_NOTIFICATION_BY_SMS_ON, &(scene_ptr->scene->is_notification_by_sms_on), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_IS_NOTIFICATION_BY_SMS_ON\n", result);
		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_IS_NOTIFICATION_BY_SMS_ON=%u\n", scene_ptr->scene->is_notification_by_sms_on);
	}
	
	result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_SCENE_IS_NOTIFICATION_BY_EMAIL_ON, &(scene_ptr->scene->is_notification_by_email_on), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_IS_NOTIFICATION_BY_EMAIL_ON\n", result);
		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_IS_NOTIFICATION_BY_EMAIL_ON=%u\n", scene_ptr->scene->is_notification_by_email_on);
	}
	
	result = zwscn_tlv_subtag_rd(ZWSCN_SUBTAG_SCENE_NOTIFICATION_SMS_NUMBER, (uint8_t *) (scene_ptr->scene->notification_sms_number), sizeof(scene_ptr->scene->notification_sms_number), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_NOTIFICATION_SMS_NUMBER. Ignoring it as this subtag is optional.\n", result);
//		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_NOTIFICATION_SMS_NUMBER=%s\n", scene_ptr->scene->notification_sms_number);
	}
	
	result = zwscn_tlv_subtag_rd(ZWSCN_SUBTAG_SCENE_NOTIFICATION_EMAIL, (uint8_t *) (scene_ptr->scene->notification_email), sizeof(scene_ptr->scene->notification_email), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_NOTIFICATION_EMAIL. Ignoring it as this subtag is optional.\n", result);
//		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_NOTIFICATION_EMAIL=%s\n", scene_ptr->scene->notification_email);
	}
	
	//Last trigger - Arm
	result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_SCENE_LT_ARM_TYPE, &(scene_ptr->scene->lt_arm.type), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_LT_ARM_TYPE\n", result);
		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_LT_ARM_TYPE=%u\n", scene_ptr->scene->lt_arm.type);
	}
	result = zwscn_tlv_subtag_rd64(ZWSCN_SUBTAG_SCENE_LT_ARM_TIME, &(scene_ptr->scene->lt_arm.time), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_LT_ARM_TIME\n", result);
		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_LT_ARM_TIME =%llu\n", scene_ptr->scene->lt_arm.time);
	}
	if(scene_ptr->scene->lt_arm.type == ZWSCN_STATELOG_TRIGGER_EVENT) {
		zwscn_security_event_p event_ptr = NULL;
		event_ptr = calloc(1, sizeof(zwscn_security_event_t));
		if(event_ptr != NULL) {
			result = zwscn_persistence_parse_tag_generic_security_event(tag, &subtagbuf, event_ptr);
			if(result != ZWSCN_ERR_NONE) {
				zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_LT_ARM EVENT. Ignoring it and moving on...\n", result);
				zwscn_free_security_event(event_ptr);
				event_ptr = NULL;
			}
			scene_ptr->scene->lt_arm.event = event_ptr;
			zwscn_message_log_debug("PERSISTENCE read LT_ARM EVENT type=%u, id=%u, cmd=%u, args_size=%u\n", event_ptr->type, event_ptr->id, event_ptr->cmd, event_ptr->device_args_size);
		}
	}
	
	//Last trigger - Disarm
	result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_SCENE_LT_DISARM_TYPE, &(scene_ptr->scene->lt_disarm.type), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_LT_DISARM_TYPE\n", result);
		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_LT_DISARM_TYPE=%u\n", scene_ptr->scene->lt_disarm.type);
	}
	result = zwscn_tlv_subtag_rd64(ZWSCN_SUBTAG_SCENE_LT_DISARM_TIME, &(scene_ptr->scene->lt_disarm.time), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_LT_DISARM_TIME\n", result);
		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_LT_DISARM_TIME =%llu\n", scene_ptr->scene->lt_disarm.time);
	}
	if(scene_ptr->scene->lt_disarm.type == ZWSCN_STATELOG_TRIGGER_EVENT) {
		zwscn_security_event_p event_ptr = NULL;
		event_ptr = calloc(1, sizeof(zwscn_security_event_t));
		if(event_ptr != NULL) {
			result = zwscn_persistence_parse_tag_generic_security_event(tag, &subtagbuf, event_ptr);
			if(result != ZWSCN_ERR_NONE) {
				zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_LT_DISARM EVENT. Ignoring it and moving on...\n", result);
				zwscn_free_security_event(event_ptr);
				event_ptr = NULL;
			}
			scene_ptr->scene->lt_disarm.event = event_ptr;
			zwscn_message_log_debug("PERSISTENCE read LT_DISARM EVENT type=%u, id=%u, cmd=%u, args_size=%u\n", event_ptr->type, event_ptr->id, event_ptr->cmd, event_ptr->device_args_size);
		}
	}
	
	//Last trigger - Alarm On
	result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_SCENE_LT_ALARM_ON_TYPE, &(scene_ptr->scene->lt_alarm_on.type), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_LT_ALARM_ON_TYPE\n", result);
		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_LT_ALARM_ON_TYPE=%u\n", scene_ptr->scene->lt_alarm_on.type);
	}
	result = zwscn_tlv_subtag_rd64(ZWSCN_SUBTAG_SCENE_LT_ALARM_ON_TIME, &(scene_ptr->scene->lt_alarm_on.time), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_LT_ALARM_ON_TIME\n", result);
		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_LT_ALARM_ON_TIME =%llu\n", scene_ptr->scene->lt_alarm_on.time);
	}
	if(scene_ptr->scene->lt_alarm_on.type == ZWSCN_STATELOG_TRIGGER_EVENT) {
		zwscn_security_event_p event_ptr = NULL;
		event_ptr = calloc(1, sizeof(zwscn_security_event_t));
		if(event_ptr != NULL) {
			result = zwscn_persistence_parse_tag_generic_security_event(tag, &subtagbuf, event_ptr);
			if(result != ZWSCN_ERR_NONE) {
				zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_LT_ALARM_ON EVENT. Ignoring it and moving on...\n", result);
				zwscn_free_security_event(event_ptr);
				event_ptr = NULL;
			}
			scene_ptr->scene->lt_alarm_on.event = event_ptr;
			zwscn_message_log_debug("PERSISTENCE read LT_ALARM_ON EVENT type=%u, id=%u, cmd=%u, args_size=%u\n", event_ptr->type, event_ptr->id, event_ptr->cmd, event_ptr->device_args_size);
		}
	}
	
	//Last trigger - Alarm Off
	result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_SCENE_LT_ALARM_OFF_TYPE, &(scene_ptr->scene->lt_alarm_off.type), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_LT_ALARM_OFF_TYPE\n", result);
		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_LT_ALARM_OFF_TYPE=%u\n", scene_ptr->scene->lt_alarm_off.type);
	}
	result = zwscn_tlv_subtag_rd64(ZWSCN_SUBTAG_SCENE_LT_ALARM_OFF_TIME, &(scene_ptr->scene->lt_alarm_off.time), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_LT_ALARM_OFF_TIME\n", result);
		goto l_err;
	} else {
		zwscn_message_log_debug("PERSISTENCE read ZWSCN_SUBTAG_SCENE_LT_ALARM_OFF_TIME =%llu\n", scene_ptr->scene->lt_alarm_off.time);
	}
	if(scene_ptr->scene->lt_alarm_off.type == ZWSCN_STATELOG_TRIGGER_EVENT) {
		zwscn_security_event_p event_ptr = NULL;
		event_ptr = calloc(1, sizeof(zwscn_security_event_t));
		if(event_ptr != NULL) {
			result = zwscn_persistence_parse_tag_generic_security_event(tag, &subtagbuf, event_ptr);
			if(result != ZWSCN_ERR_NONE) {
				zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_SCENE_LT_ALARM_OFF EVENT. Ignoring it and moving on...\n", result);
				zwscn_free_security_event(event_ptr);
				event_ptr = NULL;
			}
			scene_ptr->scene->lt_alarm_off.event = event_ptr;
			zwscn_message_log_debug("PERSISTENCE read LT_ALARM_OFF EVENT type=%u, id=%u, cmd=%u, args_size=%u\n", event_ptr->type, event_ptr->id, event_ptr->cmd, event_ptr->device_args_size);
		}
	}
	
	return ZWSCN_ERR_NONE;
l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_persistence_parse_tag_generic_security_event(zwscn_tlv_tag_p tag, uint8_t **subtagbuf_ptr, zwscn_security_event_p event_ptr) {
	int32_t result;
	
	result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_EVENT_TYPE, &(event_ptr->type), subtagbuf_ptr, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_TYPE\n", result);
		goto l_err;
	} else {
		//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_TYPE=%u\n", event_ptr->type);
	}
	
	result = zwscn_tlv_subtag_rd32(ZWSCN_SUBTAG_EVENT_ID, &(event_ptr->id), subtagbuf_ptr, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_ID\n", result);
		goto l_err;
	} else {
		//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_ID=%u\n", event_ptr->id);
	}
	
	result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_EVENT_CMD, &(event_ptr->cmd), subtagbuf_ptr, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_CMD\n", result);
		goto l_err;
	} else {
		//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_CMD=%u\n", event_ptr->cmd);
	}
	
	result = zwscn_tlv_subtag_rd32(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE, &(event_ptr->device_args_size), subtagbuf_ptr, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE\n", result);
		goto l_err;
	} else {
		//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE=%u\n", event_ptr->device_args_size);
		if(event_ptr->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
			zwscn_message_log_warning("zwscn_persistence_parse_tag_arm_security_event: device_args_size(%u) is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%u)\n", event_ptr->device_args_size, ZWSCN_DEVICE_ARGS_MAX_SIZE);
			goto l_err_device_args_size;
		}
		event_ptr->device_args = malloc(event_ptr->device_args_size);
		if(event_ptr->device_args == NULL) {
			goto l_err_memory;
		}
		result = zwscn_tlv_subtag_rd(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS, (uint8_t *) (event_ptr->device_args), event_ptr->device_args_size, subtagbuf_ptr, &tag->len);
		if(result != ZWSCN_TLV_ERR_NONE) {
			zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_DEVICE_ARGS\n", result);
			goto l_err_device_args;
		} else {
			//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_DEVICE_ARGS\n");
		}
	}
	
	return ZWSCN_ERR_NONE;
l_err:
	return ZWSCN_ERR_GENERIC;
l_err_memory:
	return ZWSCN_ERR_MEMORY;
l_err_device_args:
	free(event_ptr->device_args);
l_err_device_args_size:
	event_ptr->device_args = NULL;
	event_ptr->device_args_size = 0;
	return ZWSCN_ERR_INVALID_DATA;
}

ZWSCN_STATUS_T zwscn_persistence_parse_tag_arm_security_event(zwscn_tlv_tag_p tag, zwscn_security_event_p event_ptr) {
	uint8_t *subtagbuf;
	int32_t result;
	
	subtagbuf = tag->data;
	
	result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_EVENT_TYPE, &(event_ptr->type), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_TYPE\n", result);
		goto l_err;
	} else {
		//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_TYPE=%u\n", event_ptr->type);
	}
	
	result = zwscn_tlv_subtag_rd32(ZWSCN_SUBTAG_EVENT_ID, &(event_ptr->id), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_ID\n", result);
		goto l_err;
	} else {
		//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_ID=%u\n", event_ptr->id);
	}
	
	result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_EVENT_CMD, &(event_ptr->cmd), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_CMD\n", result);
		goto l_err;
	} else {
		//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_CMD=%u\n", event_ptr->cmd);
	}
	
	result = zwscn_tlv_subtag_rd32(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE, &(event_ptr->device_args_size), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE\n", result);
		goto l_err;
	} else {
		//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE=%u\n", event_ptr->device_args_size);
		if(event_ptr->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
			zwscn_message_log_warning("zwscn_persistence_parse_tag_arm_security_event: device_args_size(%u) is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%u)\n", event_ptr->device_args_size, ZWSCN_DEVICE_ARGS_MAX_SIZE);
			goto l_err_device_args_size;
		}
		event_ptr->device_args = malloc(event_ptr->device_args_size);
		if(event_ptr->device_args == NULL) {
			goto l_err_memory;
		}
		result = zwscn_tlv_subtag_rd(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS, (uint8_t *) (event_ptr->device_args), event_ptr->device_args_size, &subtagbuf, &tag->len);
		if(result != ZWSCN_TLV_ERR_NONE) {
			zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_DEVICE_ARGS\n", result);
			goto l_err_device_args;
		} else {
			//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_DEVICE_ARGS\n");
		}
	}
	
	return ZWSCN_ERR_NONE;
l_err:
	return ZWSCN_ERR_GENERIC;
l_err_memory:
	return ZWSCN_ERR_MEMORY;
l_err_device_args:
	free(event_ptr->device_args);
l_err_device_args_size:
	event_ptr->device_args = NULL;
	event_ptr->device_args_size = 0;
	return ZWSCN_ERR_INVALID_DATA;
}

ZWSCN_STATUS_T zwscn_persistence_parse_tag_disarm_security_event(zwscn_tlv_tag_p tag, zwscn_security_event_p event_ptr) {
	uint8_t *subtagbuf;
	int32_t result;
	
	subtagbuf = tag->data;
	
	result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_EVENT_TYPE, &(event_ptr->type), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_TYPE\n", result);
		goto l_err;
	} else {
		//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_TYPE=%u\n", event_ptr->type);
	}
	
	result = zwscn_tlv_subtag_rd32(ZWSCN_SUBTAG_EVENT_ID, &(event_ptr->id), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_ID\n", result);
		goto l_err;
	} else {
		//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_ID=%u\n", event_ptr->id);
	}
	
	result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_EVENT_CMD, &(event_ptr->cmd), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_CMD\n", result);
		goto l_err;
	} else {
		//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_CMD=%u\n", event_ptr->cmd);
	}
	
	result = zwscn_tlv_subtag_rd32(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE, &(event_ptr->device_args_size), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE\n", result);
		goto l_err;
	} else {
		//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE=%u\n", event_ptr->device_args_size);
		if(event_ptr->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
			zwscn_message_log_warning("zwscn_persistence_parse_tag_disarm_security_event: device_args_size(%u) is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%u)\n", event_ptr->device_args_size, ZWSCN_DEVICE_ARGS_MAX_SIZE);
			goto l_err_device_args_size;
		}
		event_ptr->device_args = malloc(event_ptr->device_args_size);
		if(event_ptr->device_args == NULL) {
			goto l_err_memory;
		}
		result = zwscn_tlv_subtag_rd(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS, (uint8_t *) (event_ptr->device_args), event_ptr->device_args_size, &subtagbuf, &tag->len);
		if(result != ZWSCN_TLV_ERR_NONE) {
			zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_DEVICE_ARGS\n", result);
			goto l_err_device_args;
		} else {
			//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_DEVICE_ARGS\n");
		}
	}
	
	return ZWSCN_ERR_NONE;
l_err:
	return ZWSCN_ERR_GENERIC;
l_err_memory:
	return ZWSCN_ERR_MEMORY;
l_err_device_args:
	free(event_ptr->device_args);
l_err_device_args_size:
	event_ptr->device_args = NULL;
	event_ptr->device_args_size = 0;
	return ZWSCN_ERR_INVALID_DATA;
}

ZWSCN_STATUS_T zwscn_persistence_parse_tag_alarm_security_event(zwscn_tlv_tag_p tag, zwscn_security_event_p event_ptr) {
	uint8_t *subtagbuf;
	int32_t result;
	
	subtagbuf = tag->data;
	
	result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_EVENT_TYPE, &(event_ptr->type), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_TYPE\n", result);
		goto l_err;
	} else {
		//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_TYPE=%u\n", event_ptr->type);
	}
	
	result = zwscn_tlv_subtag_rd32(ZWSCN_SUBTAG_EVENT_ID, &(event_ptr->id), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_ID\n", result);
		goto l_err;
	} else {
		//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_ID=%u\n", event_ptr->id);
	}
	
	result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_EVENT_CMD, &(event_ptr->cmd), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_CMD\n", result);
		goto l_err;
	} else {
		//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_CMD=%u\n", event_ptr->cmd);
	}
	
	result = zwscn_tlv_subtag_rd32(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE, &(event_ptr->device_args_size), &subtagbuf, &tag->len);
	if(result != ZWSCN_TLV_ERR_NONE) {
		zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE\n", result);
		goto l_err;
	} else {
		//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE=%u\n", event_ptr->device_args_size);
		if(event_ptr->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
			zwscn_message_log_warning("zwscn_persistence_parse_tag_alarm_security_event: device_args_size(%u) is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%u)\n", event_ptr->device_args_size, ZWSCN_DEVICE_ARGS_MAX_SIZE);
			goto l_err_device_args_size;
		}
		event_ptr->device_args = malloc(event_ptr->device_args_size);
		if(event_ptr->device_args == NULL) {
			goto l_err_memory;
		}
		result = zwscn_tlv_subtag_rd(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS, (uint8_t *) (event_ptr->device_args), event_ptr->device_args_size, &subtagbuf, &tag->len);
		if(result != ZWSCN_TLV_ERR_NONE) {
			zwscn_message_log_debug("Error code (%d) returned while reading ZWSCN_SUBTAG_EVENT_DEVICE_ARGS\n", result);
			goto l_err_device_args;
		} else {
			//zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_DEVICE_ARGS\n");
		}
	}
	
	return ZWSCN_ERR_NONE;
l_err:
	return ZWSCN_ERR_GENERIC;
l_err_memory:
	return ZWSCN_ERR_MEMORY;
l_err_device_args:
	free(event_ptr->device_args);
l_err_device_args_size:
	event_ptr->device_args = NULL;
	event_ptr->device_args_size = 0;
	return ZWSCN_ERR_INVALID_DATA;
}

/*************************************************/

ZWSCN_STATUS_T zwscn_persistence_delete_all_security_scenes(void *net, zwscn_list_t scene_list) {
	zwscn_scene_cont_p scene_ptr;
	uint32_t scene_id;
	int count=0;
	int i=0;
	
	if((net == NULL) || (scene_list == NULL)) {
		goto l_err_args;
	}
	
	count = zwscn_list_size(scene_list); //If list is NULL, count should be 0
	for(i=0; i < count; ++i) {
		scene_ptr = zwscn_list_get_by_index(scene_list, i);
		if(scene_ptr == NULL) {
			goto l_err;
		}
		scene_id = scene_ptr->scene->id;
		if(zwscn_persistence_delete_security_scene(net, scene_id) != ZWSCN_ERR_NONE) {
			goto l_err;
		}
		zwscn_statelog_security_scene_set(net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_DELETE, scene_id);
	}
	
		return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_persistence_delete_security_scene(void *net, uint32_t scene_id) {
	char scene_filename[ZWSCN_FILENAME_PATH_LEN];
	char *ptr_filename;
	int32_t result;
	
	if((net == NULL) || (scene_id == 0)) {
		goto l_err_args;
	}
	
	//zwscn_message_log_debug("Going to delete scene id=%d from persistent storage\n", scene_id);
	
	ptr_filename = scene_filename;
	zwscn_persistence_get_absolute_filename_from_security_scene_id(net, scene_id, &ptr_filename);
	zwscn_message_log_debug("Deleting file %s\n", ptr_filename);
	
	result = zwscn_persistence_delete_file(ptr_filename);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_debug("[Error] Can't delete file error=(%d)\n", result);
		goto l_err_file;
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err_file:
	return ZWSCN_ERR_FILE;
}

ZWSCN_STATUS_T zwscn_persistence_get_absolute_filename_from_security_scene_id(void *net, uint32_t scene_id, char **out_absolute_filename)
{
	zwscn_init_p init_data;
	char *dir_path;
	
	if((net == NULL) || (scene_id == 0) || ((*out_absolute_filename) == NULL)) {
		goto l_err_args;
	}
	
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		goto l_err;
	}
	
	dir_path = init_data->init_input->persistent_data_path;
	if((dir_path == NULL) || (strlen(dir_path) == 0)) {
		dir_path = "."; //current directory
	}
	snprintf((*out_absolute_filename), ZWSCN_FILENAME_PATH_LEN, "%s/%s%08d%s", dir_path, init_data->init_input->context_persistent, scene_id, ZWSCN_SECURITY_SCENE_FILENAME_EXT);
	//zwscn_message_log_debug("Absolute filename = %s for scene id = %d\n", (*out_absolute_filename), scene_id);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_persistence_get_data_dir_valid_items_for_security_scenes(void *net, zwscn_list_t *out_list) {
	zwscn_init_p init_data;
    zwscn_dir_t dp;
	char *dir_path;
	const char *filename;
	char *filename_new;

	if(net == NULL) {
		goto l_err_args;
	}
	
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		goto l_err;
	}
	dir_path = init_data->init_input->persistent_data_path;
	if((dir_path == NULL) || (strlen(dir_path) == 0)) {
		dir_path = "."; //current directory
	}
	dp = zwscn_filesys_open_dir(dir_path);
    if(dp == NULL) {
        zwscn_message_log_critical("Could not open dir %s\n", dir_path);
		goto l_err_dir;
	}
    
    while((filename = zwscn_filesys_read_dir_next_content_name(dp)) != NULL)
    {
		
		if((strlen(filename) != ZWSCN_FILENAME_EXACT_LEN) || (strstr(filename, ZWSCN_SECURITY_SCENE_FILENAME_EXT) == NULL)) {
			//zwscn_message_log_debug("File %s does not seem to be a zwave scene file. Ignoring it...\n", filename);
			continue;
		} else {
			if(strncmp(filename, init_data->init_input->context_persistent, (ZWSCN_LEN_CONTEXT_PERSISTENT - 1))) {
				//zwscn_message_log_debug("File %s differs with current network context (%s). Ignoring this file.\n", filename, init_data->init_input->context_persistent);
				continue;
			} else {
				//zwscn_message_log_debug("File %s found\n", filename);
			}
		}
		
		filename_new = malloc(strlen(filename) + 1);
		if(filename_new == NULL) {
			goto l_err_memory;
		}
		zwscn_util_strcpy(filename_new, filename, (strlen(filename) + 1));
		(*out_list) = zwscn_list_insert_sorted_strings((*out_list), (void *)filename_new);
    }
	
	zwscn_filesys_close_dir(dp);
	
	return ZWSCN_ERR_NONE;
 l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err:
	return ZWSCN_ERR_GENERIC;
l_err_dir:
	return ZWSCN_ERR_FILE_OPEN;
l_err_memory:
	return ZWSCN_ERR_MEMORY;
}
