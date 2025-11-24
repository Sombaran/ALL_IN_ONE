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
 @file		zwscn_persistence.c - scenes persistent storage

 To provide support for persistent storage for scenes

 @author		Arif Mohammand
 
 @version	1.0 2014-01-23  Initial release
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
#include "zwscn_util_config_file.h"
#include "zwscn_scene_api.h"
#include "zwscn_security_scene_api.h"
#include "zwscn_statelog.h"
#include "zwscn_util.h"
#include "zwscn_util_mem.h"
#include "zwscn_util_message_log.h"
#include "zwscn_scene.h"
#include "zwscn_setup_op.h"
#include "zwscn_persistence.h"
#include "zwscn_persistence_format.h"
#include "zwscn_scene_schedule.h"
#include "zwscn_security_scene.h"
#include "zwscn_notification_processor_security_scene.h"
#include "zwscn_xml.h"
#include "zwscn_serializer_functions.h"
#include "zwscn_deserializer_functions.h"

//Global declarations 
//LIST(scene_file_list);
//typedef struct scene_file_list_item {
//	struct scene_file_list_item *next;
//	char *filename;
//} zwscn_file_list_item_t;

//Prototypes
ZWSCN_STATUS_T zwscn_persistence_save_scene_to_file(const char *filename, zwscn_scene_cont_p scene_ptr);
void zwscn_persistence_save_tag_scene(FILE *file, zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr);
void zwscn_persistence_save_tag_action(FILE *file, zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr);
void zwscn_persistence_save_tag_schedule(FILE *file, zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr);
void zwscn_persistence_save_tag_event(FILE *file, zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr);
void zwscn_persistence_prepare_tag_scene(zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr);
void zwscn_persistence_prepare_tag_action(zwscn_tlv_tag_p tag, zwscn_action_p action_ptr);
void zwscn_persistence_prepare_tag_schedule(zwscn_tlv_tag_p tag, zwscn_schedule_p schedule_ptr);
void zwscn_persistence_prepare_tag_event(zwscn_tlv_tag_p tag, zwscn_event_p event_ptr);

ZWSCN_STATUS_T zwscn_persistence_load_scene_from_xml_file(const char* filename, void* net);
ZWSCN_STATUS_T zwscn_persistence_load_scene_from_file(void *net, const char *filename, zwscn_list_t *out_list);
ZWSCN_STATUS_T zwscn_persistence_load_scene(void *net, FILE *file, zwscn_scene_cont_p scene_ptr);
ZWSCN_STATUS_T zwscn_persistence_load_tag_scene(void *net, zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr);
ZWSCN_STATUS_T zwscn_persistence_load_tag_action(void *net, zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr);
ZWSCN_STATUS_T zwscn_persistence_load_tag_schedule(void *net, zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr);
ZWSCN_STATUS_T zwscn_persistence_load_tag_event(void *net, zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr);
ZWSCN_STATUS_T zwscn_persistence_parse_tag_scene(zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr);
ZWSCN_STATUS_T zwscn_persistence_parse_tag_action(zwscn_tlv_tag_p tag, zwscn_action_p action_ptr);
ZWSCN_STATUS_T zwscn_persistence_parse_tag_schedule(zwscn_tlv_tag_p tag, zwscn_schedule_p schedule_ptr);
ZWSCN_STATUS_T zwscn_persistence_parse_tag_event(zwscn_tlv_tag_p tag, zwscn_event_p event_ptr);

ZWSCN_STATUS_T zwscn_persistence_get_data_dir_valid_items(void *net, zwscn_list_t *out_list);
ZWSCN_STATUS_T zwscn_persistence_get_absolute_filename_from_scene_id(void *net, uint32_t scene_id, char **out_absolute_filename);

void zwscn_persistence_save_scene_to_xml_file(const char* filename, zwscn_scene_cont_p scene_ptr) {
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
        XML_SERIALIZER_SERIALIZE(writer, zwscn_scene_cont_t, scene_ptr);
        xml_writer_write_end_element(writer);
        xml_writer_close(writer);
    }
}

ZWSCN_STATUS_T zwscn_persistence_save_scene(void *net, zwscn_scene_cont_p scene_ptr) {
    char scene_filename[ZWSCN_FILENAME_PATH_LEN] = { 0 };
    char *ptr_filename = NULL;
    uint32_t scene_id;
    ZWSCN_STATUS_T result = ZWSCN_ERR_NONE;

    if ((net == NULL) || (scene_ptr == NULL) || (scene_ptr->scene == NULL)) {
        goto l_err_args;
    }
    scene_id = scene_ptr->scene->id;
    //zwscn_message_log_debug("Save Scene id=%d\n", scene_id);

    ptr_filename = scene_filename;
    if (zwscn_persistence_get_absolute_filename_from_scene_id(net, scene_id, &ptr_filename) != ZWSCN_ERR_NONE) {
        goto l_err;
    }
    if (scene_filename[0] == 0) { //empty string
        goto l_err;
    }

    result = zwscn_persistence_save_scene_to_file(scene_filename, scene_ptr);

    return result;
    l_err_args: return ZWSCN_ERR_INVALID_ARGS;
    l_err: return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_persistence_save_scene_to_file(const char *filename, zwscn_scene_cont_p scene_ptr) {
#if ZWSCN_PERSISTENCE_ENABLE_XML
    zwscn_persistence_save_scene_to_xml_file(filename, scene_ptr);
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

	zwscn_persistence_save_tag_scene(file, tag, scene_ptr);
	zwscn_persistence_save_tag_action(file, tag, scene_ptr);
	zwscn_persistence_save_tag_schedule(file, tag, scene_ptr);
	zwscn_persistence_save_tag_event(file, tag, scene_ptr);

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

void zwscn_persistence_save_tag_scene(FILE *file, zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr) {
    zwscn_persistence_prepare_tag_scene(tag, scene_ptr);
    //zwscn_message_log_debug("writing tag scene...\n");
    //zwscn_message_log_debug("scene id=%u name=%s active=%u\n", scene_ptr->scene->id, scene_ptr->scene->name, scene_ptr->scene->active);
    zwscn_tlv_tag_wr(file, tag);
}

void zwscn_persistence_save_tag_action(FILE *file, zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr) {
    zwscn_list_t action_list;
    zwscn_action_p action_ptr;
    int count = 0;
    int i;

    action_list = scene_ptr->list_actions;
    count = zwscn_list_size(action_list); //If list is NULL, count should be 0
    for (i = 0; i < count; ++i) {
        action_ptr = zwscn_list_get_by_index(action_list, i);
        //zwscn_message_log_debug("writing tag action id=%d...\n", action_ptr->id);
        zwscn_persistence_prepare_tag_action(tag, action_ptr);
        zwscn_tlv_tag_wr(file, tag);
    }

    action_list = scene_ptr->list_actions_invalid;
    count = zwscn_list_size(action_list); //If list is NULL, count should be 0
    for (i = 0; i < count; ++i) {
        action_ptr = zwscn_list_get_by_index(action_list, i);
        //zwscn_message_log_debug("writing tag action id=%d...\n", action_ptr->id);
        zwscn_persistence_prepare_tag_action(tag, action_ptr);
        zwscn_tlv_tag_wr(file, tag);
    }
}

void zwscn_persistence_save_tag_schedule(FILE *file, zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr) {
    zwscn_list_t schedule_list;
    zwscn_schedule_p schedule_ptr;
    int count = 0;
    int i;

    schedule_list = scene_ptr->list_schedules;
    count = zwscn_list_size(schedule_list); //If list is NULL, count should be 0
    for (i = 0; i < count; ++i) {
        schedule_ptr = zwscn_list_get_by_index(schedule_list, i);
        //zwscn_message_log_debug("writing tag schedule id=%d...\n", schedule_ptr->id);
        zwscn_persistence_prepare_tag_schedule(tag, schedule_ptr);
        zwscn_tlv_tag_wr(file, tag);
    }
}

void zwscn_persistence_save_tag_event(FILE *file, zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr) {
    zwscn_list_t event_list;
    zwscn_event_p event_ptr;
    int count = 0;
    int i;

    event_list = scene_ptr->list_events;
    count = zwscn_list_size(event_list); //If list is NULL, count should be 0
    for (i = 0; i < count; ++i) {
        event_ptr = zwscn_list_get_by_index(event_list, i);
        //zwscn_message_log_debug("writing tag event id=%d...\n", event_ptr->id);
        zwscn_persistence_prepare_tag_event(tag, event_ptr);
        zwscn_tlv_tag_wr(file, tag);
    }

    event_list = scene_ptr->list_events_invalid;
    count = zwscn_list_size(event_list); //If list is NULL, count should be 0
    for (i = 0; i < count; ++i) {
        event_ptr = zwscn_list_get_by_index(event_list, i);
        //zwscn_message_log_debug("writing tag event id=%d...\n", event_ptr->id);
        zwscn_persistence_prepare_tag_event(tag, event_ptr);
        zwscn_tlv_tag_wr(file, tag);
    }
}

void zwscn_persistence_prepare_tag_scene(zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr) {
    uint8_t *subtagbuf;
    uint16_t strlength;

    tag->id = ZWSCN_TAG_SCENE;
    tag->len = 0;
    subtagbuf = tag->data;
    zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_SCENE_ID, scene_ptr->scene->id, &subtagbuf, &tag->len);
    strlength = strlen((char *) scene_ptr->scene->name);
    //zwscn_message_log_debug("Subtag: Name. length=%d\n", strlength);
    zwscn_tlv_subtag_wr(ZWSCN_SUBTAG_SCENE_NAME, (uint8_t *) scene_ptr->scene->name, strlength, &subtagbuf, &tag->len);
    zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_SCENE_ACTIVE, scene_ptr->scene->active, &subtagbuf, &tag->len);
}

void zwscn_persistence_prepare_tag_action(zwscn_tlv_tag_p tag, zwscn_action_p action_ptr) {
    uint8_t *subtagbuf;
//	uint16_t strlength;

    tag->id = ZWSCN_TAG_ACTION;
    tag->len = 0;
    subtagbuf = tag->data;
    zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_ACTION_TYPE, action_ptr->type, &subtagbuf, &tag->len);
    zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_ACTION_ID, action_ptr->id, &subtagbuf, &tag->len);
    zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_ACTION_CMD, action_ptr->cmd, &subtagbuf, &tag->len);
    if (action_ptr->device_args_size > 0) {
        zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_ACTION_DEVICE_ARGS_SIZE, action_ptr->device_args_size, &subtagbuf, &tag->len);
        zwscn_tlv_subtag_wr(ZWSCN_SUBTAG_ACTION_DEVICE_ARGS, (uint8_t *) action_ptr->device_args, action_ptr->device_args_size, &subtagbuf, &tag->len);
    }
}

void zwscn_persistence_prepare_tag_schedule(zwscn_tlv_tag_p tag, zwscn_schedule_p schedule_ptr) {
    uint8_t *subtagbuf;

    tag->id = ZWSCN_TAG_SCHEDULE;
    tag->len = 0;
    subtagbuf = tag->data;
    zwscn_message_log_debug("Writing schedule type=%d day=%d hour=%d minute=%d\n", schedule_ptr->type, schedule_ptr->day, schedule_ptr->hour,
            schedule_ptr->minute);
    zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_SCHEDULE_TYPE, schedule_ptr->type, &subtagbuf, &tag->len);
    zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_SCHEDULE_DAY, schedule_ptr->day, &subtagbuf, &tag->len);
    zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_SCHEDULE_HOUR, schedule_ptr->hour, &subtagbuf, &tag->len);
    zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_SCHEDULE_MINUTE, schedule_ptr->minute, &subtagbuf, &tag->len);
    //zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_SCHEDULE_ID, schedule_ptr->id, &subtagbuf, &tag->len); //id will be generated when scene is loaded
}

void zwscn_persistence_prepare_tag_event(zwscn_tlv_tag_p tag, zwscn_event_p event_ptr) {
    uint8_t *subtagbuf;
//	uint16_t strlength;

    tag->id = ZWSCN_TAG_EVENT;
    tag->len = 0;
    subtagbuf = tag->data;
    zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_EVENT_TYPE, event_ptr->type, &subtagbuf, &tag->len);
    zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_EVENT_ID, event_ptr->id, &subtagbuf, &tag->len);
    zwscn_tlv_subtag_wr8(ZWSCN_SUBTAG_EVENT_CMD, event_ptr->cmd, &subtagbuf, &tag->len);
    if (event_ptr->device_args_size > 0) {
        zwscn_tlv_subtag_wr32(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE, event_ptr->device_args_size, &subtagbuf, &tag->len);
        zwscn_tlv_subtag_wr(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS, (uint8_t *) event_ptr->device_args, event_ptr->device_args_size, &subtagbuf, &tag->len);
    }
}

ZWSCN_STATUS_T zwscn_persistence_is_xml_file(const char* filename){
    xml_exception_t err;
    xml_reader_t* reader = xml_reader_new(filename);
    if(reader){
        uint32_t xml_name_hash = zwscn_util_str_hash("xml");
        uint32_t zwave_name_hash = zwscn_util_str_hash("zwave");
        err = xml_reader_next(reader);
        if(!err){
            uint32_t name_hash = zwscn_util_str_hash(xml_reader_node_name(reader));
            switch(xml_reader_node_type(reader)){
            case XML_NODE_TYPE_PI:
            case XML_NODE_TYPE_START_ELEMENT:
                if(name_hash == zwave_name_hash || name_hash == xml_name_hash){
                    xml_reader_close(reader);
                    return ZWSCN_ERR_NONE;
                }
                break;
            default:
                break;
            }
        }
    }
    xml_reader_close(reader);
    return ZWSCN_ERR_FILE_OPEN;
}

ZWSCN_STATUS_T zwscn_persistence_load_all_scenes(void *net, zwscn_list_t *out_list) {
    zwscn_list_t file_list = NULL;
    ZWSCN_STATUS_T result;
    int count;
    int i;
    const char *filename = NULL;
    char scene_filename[ZWSCN_FILENAME_PATH_LEN] = { 0 };
    char *scene_filename_ptr = NULL;

    if (net == NULL) {
        goto l_err_args;
    }

    result = zwscn_persistence_get_data_dir_valid_items(net, &file_list);
    if (result != ZWSCN_ERR_NONE) {
        zwscn_message_log_debug("[Warning] zwscn_persistence_get_data_dir_valid_items returned %d\n", result);
        goto l_err;
    }

    //zwscn_persistence_print_file_list(file_list);

    scene_filename_ptr = scene_filename;
    count = zwscn_list_size(file_list); //If list is NULL, count should be 0
    for (i = 0; i < count; ++i) {
        memset(scene_filename, 0, sizeof(scene_filename)); //zero out the temp buffer
        filename = zwscn_list_get_by_index(file_list, i);
        zwscn_message_log_debug("Loading: %s\n", filename);
        if (zwscn_persistence_get_absolute_filename(net, filename, &scene_filename_ptr) != ZWSCN_ERR_NONE) { //using scene_filename_ptr instead of scene_filename because the funtion takes char **
            goto l_err;
        }
        if (scene_filename[0] == 0) { //empty string
            goto l_err;
        }
#if ZWSCN_PERSISTENCE_ENABLE_XML
        if(zwscn_persistence_is_xml_file(scene_filename) == ZWSCN_ERR_NONE){
#else
        if(0){
#endif
            result = zwscn_persistence_load_scene_from_xml_file(scene_filename, net);
        }
        else {
            result = zwscn_persistence_load_scene_from_file(net, scene_filename, out_list);
        }
        if (result != ZWSCN_ERR_NONE) {
            zwscn_message_log_debug("[Warning] zwscn_load_scene_from_file returned %d\n", result);
            continue; //try to load other files
        }
    }

    zwscn_list_free(&file_list); //delete all items and free memory their memory

    return ZWSCN_ERR_NONE;
    l_err_args: return ZWSCN_ERR_INVALID_ARGS;
    l_err: return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_persistence_load_scene_from_xml_file(const char* filename, void* net) {
    zwscn_scene_cont_p scene_ptr;
    ZWSCN_STATUS_T result;
    xml_reader_t* reader;

    if ((net == NULL) || (filename == NULL)) goto l_err_args;

    scene_ptr = calloc(1, sizeof(zwscn_scene_cont_t));

    if (!scene_ptr) goto l_err_memory;

    scene_ptr->net = net;
    scene_ptr->mutex_status_update_timer_info = zwscn_mutex_new();
    if (scene_ptr->mutex_status_update_timer_info == NULL) {
        goto l_err_memory_mutex;
    }
    scene_ptr->hash_status_update_timer_info = zwscn_hash_new();
    zwscn_deserializer_functions_register_all_types();
    reader = xml_reader_new(filename);
    result = XML_DESERIALIZER_DESERIALIZE(reader, zwscn_scene_cont_t, scene_ptr);
    xml_reader_close(reader);

    if (result != ZWSCN_ERR_NONE) {
        zwscn_free_scene_lists(scene_ptr);
        return result;
    }

    return ZWSCN_ERR_NONE;
    l_err_args: return ZWSCN_ERR_INVALID_ARGS;
    l_err_memory_mutex: free(scene_ptr);
    l_err_memory: return ZWSCN_ERR_MEMORY;
}

ZWSCN_STATUS_T zwscn_persistence_load_scene_from_file(void *net, const char *filename, zwscn_list_t *out_list) {
	FILE * file;
	zwscn_scene_cont_p scene_ptr;
	ZWSCN_STATUS_T result;
	//uint32_t scene_global_id;
	//zwscn_list_t *scene_list;

	if((net == NULL) || (filename == NULL)) {
		goto l_err_args;
	}

	file = zwscn_persistence_open_file(filename, ZWSCN_FILEMODE_READ);
	if (file == NULL) {
		zwscn_message_log_critical("[Error] SCENE_ERROR_FILE_OPEN\n");
		goto l_err_file_open;
	}
	//zwscn_message_log_debug("File: %s opened\n", filename);

	scene_ptr = (zwscn_scene_cont_p) calloc(1, sizeof(zwscn_scene_cont_t));
	if(!scene_ptr) {
		goto l_err_memory;
	}
	//scene_init_scene_ptr(scene_ptr);
	scene_ptr->net = net;
	scene_ptr->mutex_status_update_timer_info = zwscn_mutex_new();
	if(scene_ptr->mutex_status_update_timer_info == NULL) {
		goto l_err_memory_mutex;
	}
	scene_ptr->hash_status_update_timer_info = zwscn_hash_new();

	result = zwscn_persistence_load_scene(net, file, scene_ptr);
	if(result != ZWSCN_ERR_NONE) {
		zwscn_message_log_debug("[Warning](%d) returned by zwscn_load_scene\n", result);
		goto l_err;
	}
//	scene_global_id = zwscn_get_scene_id_pool_index(net);
//	//zwscn_message_log_debug("Global scene id=%d curr scene id=%d\n", scene_global_id, scene_ptr->scene->id);
//	if(scene_ptr->scene->id > scene_global_id) {
//		zwscn_set_scene_id_pool_index(net, scene_ptr->scene->id);
//	}
//
//	zwscn_set_statelog_start_op_add(net, scene_ptr->scene->id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
////	if(zwscn_find_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
////		goto l_err;
////	}
//	if(zwscn_is_list_full_scene(net, (*out_list))) {
//		goto l_err_list_full;
//	}
//	if(((*out_list) = zwscn_list_add((*out_list), scene_ptr)) == NULL) {
//		goto l_err;
//	}
//	zwscn_set_statelog_end_op_add(net, scene_ptr->scene->id, ZWSCN_STATELOG_TRIGGER_SYSTEM);

	fclose(file);

	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err_file_open:
	return ZWSCN_ERR_FILE_OPEN;
l_err_memory:
	fclose(file);
	return ZWSCN_ERR_MEMORY;
l_err_memory_mutex:
	free(scene_ptr);
	fclose(file);
	return ZWSCN_ERR_MEMORY;
//l_err_list_full:
//	free(scene_ptr);
//	fclose(file);
//	return ZWSCN_ERR_LIST_FULL;
l_err:
	zwscn_hash_destroy(scene_ptr->hash_status_update_timer_info);
	zwscn_mutex_delete(scene_ptr->mutex_status_update_timer_info);
	free(scene_ptr);
	fclose(file);
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_persistence_load_scene(void *net, FILE *file, zwscn_scene_cont_p scene_ptr) {
    int32_t result;
    zwscn_tlv_tag_p tag;
    ZWSCN_STATUS_T function_result;
    zwscn_list_t *scene_list;
    int count;
    zwscn_init_p init_data;

    if ((net == NULL) || (file == NULL) || (scene_ptr == NULL)) {
        goto l_err_args;
    }

    tag = (zwscn_tlv_tag_p) calloc(1, sizeof(zwscn_tlv_tag_t) + ZWSCN_TLV_SUBTAGS_SIZE_MAX);
    if (!tag) {
        goto l_err_memory;
    }
    tag->len = ZWSCN_TLV_SUBTAGS_SIZE_MAX; //while reading, tag len must be initialized

    while ((result = zwscn_tlv_tag_rd(file, tag)) == ZWSCN_TLV_ERR_NONE) {
        switch (tag->id) {
        case ZWSCN_TAG_SCENE:
            //zwscn_message_log_debug("Found Tag=scene\n");
            function_result = zwscn_persistence_load_tag_scene(net, tag, scene_ptr);
            if (function_result != ZWSCN_ERR_NONE) {
                zwscn_free_scene_lists(scene_ptr);
                goto l_err;
            }
            break;
        case ZWSCN_TAG_ACTION:
            //zwscn_message_log_debug("Found Tag=action\n");
            zwscn_persistence_load_tag_action(net, tag, scene_ptr);
            break;
        case ZWSCN_TAG_SCHEDULE:
            //zwscn_message_log_debug("Found Tag=schedule\n");
            zwscn_persistence_load_tag_schedule(net, tag, scene_ptr);
            break;
        case ZWSCN_TAG_EVENT:
            //zwscn_message_log_debug("Found Tag=event\n");
            zwscn_persistence_load_tag_event(net, tag, scene_ptr);
            break;
        default:
            zwscn_message_log_debug("Tag: Unknown id=%d\n", tag->id);
            break;
        }
        memset(tag, 0, (sizeof(zwscn_tlv_tag_t) + ZWSCN_TLV_SUBTAGS_SIZE_MAX)); //zero out the tag variable as we are going to reuse it
    }

    free(tag);

    if (scene_ptr->scene == NULL) {
        zwscn_free_scene_lists(scene_ptr);
        goto l_err_2;
    }

#ifndef ZWSCN_LOAD_SCENE_WITH_NO_ACTION
    count = zwscn_list_size(scene_ptr->list_actions);
    zwscn_message_log_debug("Scene id=%d has %d actions loaded\n", scene_ptr->scene->id, count);
    if (count <= 0) { //There is no action in this Scene
        init_data = zwscn_get_init_data(net);
        if (init_data == NULL) {
            goto l_err_2;
        }
        scene_list = &(init_data->list_scenes);
        (*scene_list) = zwscn_list_remove((*scene_list), scene_ptr); //remove the Scene from the list as it was added to the list already otherwise we'd have had an error earlier

        count = zwscn_list_size(scene_ptr->list_actions_invalid);
        zwscn_message_log_debug("Scene id=%d has %d invalid actions\n", scene_ptr->scene->id, count);
        if (count > 0) { 						//it has some invalid actions, so it should be moved to invalid scene list instead of unloading from the memory
            scene_list = &(init_data->list_scenes_invalid);
            (*scene_list) = zwscn_list_add((*scene_list), scene_ptr);
            zwscn_message_log_debug("Moved Scene id=%d to invalid scene list\n", scene_ptr->scene->id);
        }
        else {
            zwscn_free_scene(scene_ptr);							//free all the memory allocated by the Scene and its children
            zwscn_message_log_debug("Unloaded Scene without any action\n");
            //Continue as normal without throwing an error because if an error is thrown the calling function will try to free scene_ptr which we have already freed here
        }
    }

#endif

    return ZWSCN_ERR_NONE;
    l_err_args: return ZWSCN_ERR_INVALID_ARGS;
    l_err_memory: return ZWSCN_ERR_MEMORY;
    l_err: free(tag);
    //return ZWSCN_ERR_GENERIC;
    return function_result;
    l_err_2: return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_persistence_load_tag_scene(void *net, zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr) {
    zwscn_list_t *scene_list;
    uint32_t scene_global_id;
    ZWSCN_STATUS_T result;

    //zwscn_message_log_debug("FILEREAD sizeof(zwscn_scene_t)=%u\n", sizeof(zwscn_scene_t));
    scene_ptr->scene = calloc(1, sizeof(zwscn_scene_t));
    if (scene_ptr->scene == NULL) {
        goto l_err_memory;
    }

//	zwscn_message_log_debug("FILEREAD scene_ptr->scene=%p\n", scene_ptr->scene);
    result = zwscn_persistence_parse_tag_scene(tag, scene_ptr);
    if (result != ZWSCN_ERR_NONE) {
        goto l_err;
    }

    scene_global_id = zwscn_get_scene_id_pool_index(net);
    //zwscn_message_log_debug("Global scene id=%d curr scene id=%d\n", scene_global_id, scene_ptr->scene->id);
    if (scene_ptr->scene->id > scene_global_id) {
        zwscn_set_scene_id_pool_index(net, scene_ptr->scene->id);
    }

    zwscn_set_statelog_start_op_add(net, scene_ptr->scene->id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
    if (zwscn_find_scene_list(net, &scene_list) != ZWSCN_ERR_NONE) {
        goto l_err;
    }
    if (zwscn_is_list_full_scene(net, (*scene_list))) {
        goto l_err_list_full;
    }
    if (((*scene_list) = zwscn_list_add((*scene_list), scene_ptr)) == NULL) {
        goto l_err;
    }
    zwscn_set_statelog_end_op_add(net, scene_ptr->scene->id, ZWSCN_STATELOG_TRIGGER_SYSTEM);

    return ZWSCN_ERR_NONE;
    l_err_memory: return ZWSCN_ERR_MEMORY;
    l_err_list_full: free(scene_ptr->scene);
    scene_ptr->scene = NULL;
    return ZWSCN_ERR_LIST_FULL;
    l_err: free(scene_ptr->scene);
    scene_ptr->scene = NULL;
    return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_persistence_load_tag_action(void *net, zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr) {
    zwscn_list_t action_list;
    zwscn_action_p action_ptr;
    zwscn_action_p action_ptr_temp;
    zwscn_event_p event_ptr_temp;
    int32_t result;
    uint8_t node_id, ep_id;
    uint16_t cls_id;
    zwscn_init_p init_data;

    action_ptr = (zwscn_action_p) calloc(1, sizeof(zwscn_action_t));
    if (action_ptr == NULL) {
        goto l_err_memory;
    }
    result = zwscn_persistence_parse_tag_action(tag, action_ptr);
    if (result != ZWSCN_ERR_NONE) {
        goto l_err_parse_tag;
    }
    if (zwscn_find_action(net, scene_ptr->scene->id, action_ptr->id, &action_ptr_temp) == ZWSCN_ERR_NONE) { //Action exists with the same id
        goto l_err_duplicate_id;
    }
#if ZWSCN_CHECK_DEVICE_INTF_IN_ACTIONS_AND_EVENSTS
    if (zwscn_find_event(net, scene_ptr->scene->id, action_ptr->id, &event_ptr_temp) == ZWSCN_ERR_NONE) { //Event exists with the same id
        goto l_err_duplicate_id;
    }
#endif
    node_id = zwscn_util_combo_id_to_node_id(action_ptr->id);
    ep_id = zwscn_util_combo_id_to_ep_id(action_ptr->id);
    cls_id = zwscn_util_combo_id_to_cls_id(action_ptr->id);
    init_data = zwscn_get_init_data(net);
    if (init_data && init_data->init_input && init_data->init_input->zwscn_fn_is_valid_handler) {
        result = init_data->init_input->zwscn_fn_is_valid_handler(net, ZWSCN_CHECK_NODE_EP_CLS, node_id, ep_id, cls_id);
        zwscn_message_log_debug("[1] zwscn_fn_is_valid_handler returned %d for node=%d ep=%d cls=%d\n", result, node_id, ep_id, cls_id);
        if (!result) {
            //goto l_err_device_invalid;
            action_list = scene_ptr->list_actions_invalid;
            if ((scene_ptr->list_actions_invalid = zwscn_list_add(action_list, action_ptr)) == NULL) {
                goto l_err;
            }
        }
        else {
            action_list = scene_ptr->list_actions;
            if (zwscn_is_list_full_action(net, action_list)) {
                goto l_err_list_full;
            }
            if ((scene_ptr->list_actions = zwscn_list_add(action_list, action_ptr)) == NULL) {
                goto l_err;
            }
        }
    }
    else {
        zwscn_message_log_warning("Function zwscn_fn_is_valid_handler is NULL for net=%p\n", net);
        goto l_err_callback_null;
    }

    if (zwscn_setup_op_request(net, ZWSCN_TYPE_ACTION, action_ptr) != ZWSCN_ERR_NONE) {
        goto l_err;
    }

    return ZWSCN_ERR_NONE;
//l_err_args:
//	return ZWSCN_ERR_INVALID_ARGS;
    l_err_memory: return ZWSCN_ERR_MEMORY;
    l_err_parse_tag: free(action_ptr);
    return ZWSCN_ERR_INVALID_DATA;
    l_err_duplicate_id: free(action_ptr->device_args);
    free(action_ptr);
    return ZWSCN_ERR_ITEM_IGNORED;
    l_err: free(action_ptr->device_args);
    free(action_ptr);
    return ZWSCN_ERR_GENERIC;
    l_err_list_full: free(action_ptr->device_args);
    free(action_ptr);
    return ZWSCN_ERR_LIST_FULL;
//l_err_device_invalid:
//	free(action_ptr->device_args);
//	free(action_ptr);
//	return ZWSCN_ERR_INVALID_DEVICE;
    l_err_callback_null: free(action_ptr->device_args);
    free(action_ptr);
    return ZWSCN_ERR_CALLBACK_NULL;
}

ZWSCN_STATUS_T zwscn_persistence_load_tag_schedule(void *net, zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr) {
    zwscn_list_t schedule_list;
    zwscn_schedule_p schedule_ptr;
    int32_t result;

    schedule_ptr = (zwscn_schedule_p) calloc(1, sizeof(zwscn_schedule_t));
    if (schedule_ptr == NULL) {
        goto l_err_memory;
    }
    result = zwscn_persistence_parse_tag_schedule(tag, schedule_ptr);
    if (result != ZWSCN_ERR_NONE) {
        goto l_err;
    }
    if (zwscn_schedule_is_data_valid(schedule_ptr) == ZWSCN_FALSE) {
        zwscn_message_log_warning("Schedule value for day bitmask(0x%02x) or hour(%d) or minute(%d) is invalid\n", schedule_ptr->day, schedule_ptr->hour,
                schedule_ptr->minute);
        goto l_err;
    }
    if ((schedule_ptr->id = zwscn_generate_scene_schedule_id(net, scene_ptr->scene->id)) == 0) {
        goto l_err;
    }
    schedule_list = scene_ptr->list_schedules;
    if (zwscn_is_list_full_schedule(net, schedule_list)) {
        goto l_err_list_full;
    }
    if ((scene_ptr->list_schedules = zwscn_list_add(schedule_list, schedule_ptr)) == NULL) {
        goto l_err;
    }
    zwscn_scene_triggers_info_update(net, scene_ptr);
    if (zwscn_schedule_handle_update(net, scene_ptr, schedule_ptr->id, ZWSCN_TRUE) != ZWSCN_ERR_NONE) { //ZWSCN_TRUE is for checking the schedule from today
        goto l_err_schedule;
    }

    return ZWSCN_ERR_NONE;
//l_err_args:
//	return ZWSCN_ERR_INVALID_ARGS;
    l_err_memory: return ZWSCN_ERR_MEMORY;
    l_err_list_full: free(schedule_ptr);
    return ZWSCN_ERR_LIST_FULL;
    l_err: free(schedule_ptr);
    return ZWSCN_ERR_GENERIC;
    l_err_schedule: return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_persistence_load_tag_event(void *net, zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr) {
    zwscn_list_t event_list;
    zwscn_event_p event_ptr;
    zwscn_action_p action_ptr_temp;
    zwscn_event_p event_ptr_temp;
    int32_t result;
    uint8_t node_id, ep_id;
    uint16_t cls_id;
    zwscn_init_p init_data;

    event_ptr = (zwscn_event_p) calloc(1, sizeof(zwscn_event_t));
    if (event_ptr == NULL) {
        goto l_err_memory;
    }
    result = zwscn_persistence_parse_tag_event(tag, event_ptr);
    if (result != ZWSCN_ERR_NONE) {
        goto l_err;
    }
    if (zwscn_find_event(net, scene_ptr->scene->id, event_ptr->id, &event_ptr_temp) == ZWSCN_ERR_NONE) { //Event exists with the same id
        goto l_err_duplicate_id;
    }
#if ZWSCN_CHECK_DEVICE_INTF_IN_ACTIONS_AND_EVENSTS
    if (zwscn_find_action(net, scene_ptr->scene->id, event_ptr->id, &action_ptr_temp) == ZWSCN_ERR_NONE) { //Action exists with the same id
        goto l_err_duplicate_id;
    }
#endif
    node_id = zwscn_util_combo_id_to_node_id(event_ptr->id);
    ep_id = zwscn_util_combo_id_to_ep_id(event_ptr->id);
    cls_id = zwscn_util_combo_id_to_cls_id(event_ptr->id);
    init_data = zwscn_get_init_data(net);
    if (init_data && init_data->init_input && init_data->init_input->zwscn_fn_is_valid_handler) {
        result = init_data->init_input->zwscn_fn_is_valid_handler(net, ZWSCN_CHECK_NODE_EP_CLS, node_id, ep_id, cls_id);
        if (!result) {
            //goto l_err_device_invalid;
            event_list = scene_ptr->list_events_invalid;
            if ((scene_ptr->list_events_invalid = zwscn_list_add(event_list, event_ptr)) == NULL) {
                goto l_err;
            }
        }
        else {
            event_list = scene_ptr->list_events;
            if (zwscn_is_list_full_event(net, event_list)) {
                goto l_err_list_full;
            }
            if ((scene_ptr->list_events = zwscn_list_add(event_list, event_ptr)) == NULL) {
                goto l_err;
            }
        }
    }
    else {
        zwscn_message_log_warning("Function zwscn_fn_is_valid_handler is NULL for net=%p\n", net);
        goto l_err_callback_null;
    }
    zwscn_scene_triggers_info_update(net, scene_ptr);
    if (zwscn_setup_op_request(net, ZWSCN_TYPE_EVENT, event_ptr) != ZWSCN_ERR_NONE) {
        goto l_err;
    }

    return ZWSCN_ERR_NONE;
//l_err_args:
//	return ZWSCN_ERR_INVALID_ARGS;
    l_err_memory: return ZWSCN_ERR_MEMORY;
//l_err_parse_tag:
//	free(event_ptr);
//	return ZWSCN_ERR_INVALID_DATA;
    l_err_duplicate_id: free(event_ptr->device_args);
    free(event_ptr);
    return ZWSCN_ERR_ITEM_IGNORED;
    l_err_list_full: free(event_ptr->device_args);
    free(event_ptr);
    return ZWSCN_ERR_LIST_FULL;
    l_err: free(event_ptr->device_args);
    free(event_ptr);
    return ZWSCN_ERR_GENERIC;
//l_err_device_invalid:
//	free(event_ptr->device_args);
//	free(event_ptr);
//	return ZWSCN_ERR_INVALID_DEVICE;
    l_err_callback_null: free(event_ptr->device_args);
    free(event_ptr);
    return ZWSCN_ERR_CALLBACK_NULL;
}

ZWSCN_STATUS_T zwscn_persistence_parse_tag_scene(zwscn_tlv_tag_p tag, zwscn_scene_cont_p scene_ptr) {
    uint8_t *subtagbuf;
    int32_t result;

    subtagbuf = tag->data;

    result = zwscn_tlv_subtag_rd32(ZWSCN_SUBTAG_SCENE_ID, &(scene_ptr->scene->id), &subtagbuf, &tag->len);
    if (result != ZWSCN_TLV_ERR_NONE) {
        zwscn_message_log_debug("[Error](%d) reading ZWSCN_SUBTAG_SCENE_ID\n", result);
        goto l_err;
    }
    else {
        zwscn_message_log_debug("Found ZWSCN_SUBTAG_SCENE_ID=%u\n", scene_ptr->scene->id);
    }

    result = zwscn_tlv_subtag_rd(ZWSCN_SUBTAG_SCENE_NAME, (uint8_t *) (scene_ptr->scene->name), sizeof(scene_ptr->scene->name), &subtagbuf, &tag->len);
    if (result != ZWSCN_TLV_ERR_NONE) {
        zwscn_message_log_debug("[Error](%d) reading ZWSCN_SUBTAG_SCENE_NAME\n", result);
        goto l_err;
    }
    else {
        //zwscn_message_log_debug("Found ZWSCN_SUBTAG_SCENE_NAME=%s\n", scene_ptr->scene->name);
    }

    result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_SCENE_ACTIVE, &(scene_ptr->scene->active), &subtagbuf, &tag->len);
    if (result != ZWSCN_TLV_ERR_NONE) {
        zwscn_message_log_debug("[Error](%d) reading ZWSCN_SUBTAG_SCENE_ACTIVE\n", result);
        goto l_err;
    }
    else {
        //zwscn_message_log_debug("Found ZWSCN_SUBTAG_SCENE_ACTIVE=%u\n", scene_ptr->scene->active);
    }

    return ZWSCN_ERR_NONE;
    l_err: return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_persistence_parse_tag_action(zwscn_tlv_tag_p tag, zwscn_action_p action_ptr) {
    uint8_t *subtagbuf;
    int32_t result;

    subtagbuf = tag->data;

    result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_ACTION_TYPE, &(action_ptr->type), &subtagbuf, &tag->len);
    if (result != ZWSCN_TLV_ERR_NONE) {
        zwscn_message_log_debug("[Error](%d) reading ZWSCN_SUBTAG_ACTION_TYPE\n", result);
        goto l_err;
    }
    else {
        //zwscn_message_log_debug("Found ZWSCN_SUBTAG_ACTION_TYPE=%u\n", action_ptr->type);
    }

    result = zwscn_tlv_subtag_rd32(ZWSCN_SUBTAG_ACTION_ID, &(action_ptr->id), &subtagbuf, &tag->len);
    if (result != ZWSCN_TLV_ERR_NONE) {
        zwscn_message_log_debug("[Error](%d) reading ZWSCN_SUBTAG_ACTION_ID\n", result);
        goto l_err;
    }
    else {
        //zwscn_message_log_debug("Found ZWSCN_SUBTAG_ACTION_ID=%u\n", action_ptr->id);
    }

    result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_ACTION_CMD, &(action_ptr->cmd), &subtagbuf, &tag->len);
    if (result != ZWSCN_TLV_ERR_NONE) {
        zwscn_message_log_debug("[Error](%d) reading ZWSCN_SUBTAG_ACTION_CMD\n", result);
        goto l_err;
    }
    else {
        //zwscn_message_log_debug("Found ZWSCN_SUBTAG_ACTION_CMD=%u\n", action_ptr->cmd);
    }

    result = zwscn_tlv_subtag_rd32(ZWSCN_SUBTAG_ACTION_DEVICE_ARGS_SIZE, &(action_ptr->device_args_size), &subtagbuf, &tag->len);
    if (result != ZWSCN_TLV_ERR_NONE) {
        zwscn_message_log_debug("[Error](%d) reading ZWSCN_SUBTAG_ACTION_DEVICE_ARGS_SIZE\n", result);
        goto l_err;
    }
    else {
        //zwscn_message_log_debug("Found ZWSCN_SUBTAG_ACTION_DEVICE_ARGS_SIZE=%u\n", action_ptr->device_args_size);
        if (action_ptr->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
            zwscn_message_log_warning("zwscn_persistence_parse_tag_action: device_args_size(%u) is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%u)\n",
                    action_ptr->device_args_size, ZWSCN_DEVICE_ARGS_MAX_SIZE);
            goto l_err_device_args_size;
        }
        action_ptr->device_args = malloc(action_ptr->device_args_size);
        if (action_ptr->device_args == NULL) {
            goto l_err_memory;
        }
        result = zwscn_tlv_subtag_rd(ZWSCN_SUBTAG_ACTION_DEVICE_ARGS, (uint8_t *) (action_ptr->device_args), action_ptr->device_args_size, &subtagbuf,
                &tag->len);
        if (result != ZWSCN_TLV_ERR_NONE) {
            zwscn_message_log_debug("[Error](%d) reading ZWSCN_SUBTAG_ACTION_DEVICE_ARGS\n", result);
            goto l_err_device_args;
        }
        else {
            //zwscn_message_log_debug("Found ZWSCN_SUBTAG_ACTION_DEVICE_ARGS\n");
        }
    }

    return ZWSCN_ERR_NONE;
    l_err: return ZWSCN_ERR_GENERIC;
    l_err_memory: return ZWSCN_ERR_MEMORY;
    l_err_device_args: free(action_ptr->device_args);
    l_err_device_args_size: action_ptr->device_args = NULL;
    action_ptr->device_args_size = 0;
    return ZWSCN_ERR_INVALID_DATA;
}

ZWSCN_STATUS_T zwscn_persistence_parse_tag_schedule(zwscn_tlv_tag_p tag, zwscn_schedule_p schedule_ptr) {
    uint8_t *subtagbuf;
    int32_t result;

    subtagbuf = tag->data;

    result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_SCHEDULE_TYPE, &(schedule_ptr->type), &subtagbuf, &tag->len);
    if (result != ZWSCN_TLV_ERR_NONE) {
        zwscn_message_log_debug("[Error](%d) reading ZWSCN_SUBTAG_SCHEDULE_TYPE\n", result);
        goto l_err;
    }
    else {
        zwscn_message_log_debug("Found ZWSCN_SUBTAG_SCHEDULE_TYPE=%u\n", schedule_ptr->type);
    }

    result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_SCHEDULE_DAY, &(schedule_ptr->day), &subtagbuf, &tag->len);
    if (result != ZWSCN_TLV_ERR_NONE) {
        zwscn_message_log_debug("[Error](%d) reading ZWSCN_SUBTAG_SCHEDULE_DAY\n", result);
        goto l_err;
    }
    else {
        zwscn_message_log_debug("Found ZWSCN_SUBTAG_SCHEDULE_DAY=%u\n", schedule_ptr->day);
    }

    result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_SCHEDULE_HOUR, &(schedule_ptr->hour), &subtagbuf, &tag->len);
    if (result != ZWSCN_TLV_ERR_NONE) {
        zwscn_message_log_debug("[Error](%d) reading ZWSCN_SUBTAG_SCHEDULE_HOUR\n", result);
        goto l_err;
    }
    else {
        zwscn_message_log_debug("Found ZWSCN_SUBTAG_SCHEDULE_HOUR=%u\n", schedule_ptr->hour);
    }

    result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_SCHEDULE_MINUTE, &(schedule_ptr->minute), &subtagbuf, &tag->len);
    if (result != ZWSCN_TLV_ERR_NONE) {
        zwscn_message_log_debug("[Error](%d) reading ZWSCN_SUBTAG_SCHEDULE_MINUTE\n", result);
        goto l_err;
    }
    else {
        zwscn_message_log_debug("Found ZWSCN_SUBTAG_SCHEDULE_MINUTE=%u\n", schedule_ptr->minute);
    }

//	result = zwscn_tlv_subtag_rd32(ZWSCN_SUBTAG_SCHEDULE_ID, &(schedule_ptr->id), &subtagbuf, &tag->len);
//	if(result != ZWSCN_TLV_ERR_NONE) {
//		zwscn_message_log_debug("[Error](%d) reading ZWSCN_SUBTAG_SCHEDULE_ID\n", result);
//		goto l_err;
//	} else {
//		zwscn_message_log_debug("Found ZWSCN_SUBTAG_SCHEDULE_ID=%u\n", schedule_ptr->id);
//	}
//regenerate id after loading the schedule

    return ZWSCN_ERR_NONE;
    l_err: return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_persistence_parse_tag_event(zwscn_tlv_tag_p tag, zwscn_event_p event_ptr) {
    uint8_t *subtagbuf;
    int32_t result;

    subtagbuf = tag->data;

    result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_EVENT_TYPE, &(event_ptr->type), &subtagbuf, &tag->len);
    if (result != ZWSCN_TLV_ERR_NONE) {
        zwscn_message_log_debug("[Error](%d) reading ZWSCN_SUBTAG_EVENT_TYPE\n", result);
        goto l_err;
    }
    else {
        //zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_TYPE=%u\n", event_ptr->type);
    }

    result = zwscn_tlv_subtag_rd32(ZWSCN_SUBTAG_EVENT_ID, &(event_ptr->id), &subtagbuf, &tag->len);
    if (result != ZWSCN_TLV_ERR_NONE) {
        zwscn_message_log_debug("[Error](%d) reading ZWSCN_SUBTAG_EVENT_ID\n", result);
        goto l_err;
    }
    else {
        //zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_ID=%u\n", event_ptr->id);
    }

    result = zwscn_tlv_subtag_rd8(ZWSCN_SUBTAG_EVENT_CMD, &(event_ptr->cmd), &subtagbuf, &tag->len);
    if (result != ZWSCN_TLV_ERR_NONE) {
        zwscn_message_log_debug("[Error](%d) reading ZWSCN_SUBTAG_EVENT_CMD\n", result);
        goto l_err;
    }
    else {
        //zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_CMD=%u\n", event_ptr->cmd);
    }

    result = zwscn_tlv_subtag_rd32(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE, &(event_ptr->device_args_size), &subtagbuf, &tag->len);
    if (result != ZWSCN_TLV_ERR_NONE) {
        zwscn_message_log_debug("[Error](%d) reading ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE\n", result);
        goto l_err;
    }
    else {
        //zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE=%u\n", event_ptr->device_args_size);
        if (event_ptr->device_args_size > ZWSCN_DEVICE_ARGS_MAX_SIZE) {
            zwscn_message_log_warning("zwscn_persistence_parse_tag_event: device_args_size(%u) is more than ZWSCN_DEVICE_ARGS_MAX_SIZE(%u)\n",
                    event_ptr->device_args_size, ZWSCN_DEVICE_ARGS_MAX_SIZE);
            goto l_err_device_args_size;
        }
        event_ptr->device_args = malloc(event_ptr->device_args_size);
        if (event_ptr->device_args == NULL) {
            goto l_err_memory;
        }
        result = zwscn_tlv_subtag_rd(ZWSCN_SUBTAG_EVENT_DEVICE_ARGS, (uint8_t *) (event_ptr->device_args), event_ptr->device_args_size, &subtagbuf, &tag->len);
        if (result != ZWSCN_TLV_ERR_NONE) {
            zwscn_message_log_debug("[Error](%d) reading ZWSCN_SUBTAG_EVENT_DEVICE_ARGS\n", result);
            goto l_err_device_args;
        }
        else {
            //zwscn_message_log_debug("Found ZWSCN_SUBTAG_EVENT_DEVICE_ARGS\n");
        }
    }

    return ZWSCN_ERR_NONE;
    l_err: return ZWSCN_ERR_GENERIC;
    l_err_memory: return ZWSCN_ERR_MEMORY;
    l_err_device_args: free(event_ptr->device_args);
    l_err_device_args_size: event_ptr->device_args = NULL;
    event_ptr->device_args_size = 0;
    return ZWSCN_ERR_INVALID_DATA;
}

ZWSCN_STATUS_T zwscn_persistence_get_data_dir_valid_items(void *net, zwscn_list_t *out_list) {
    zwscn_init_p init_data;
    zwscn_dir_t dp;
    char *dir_path;
    const char *filename;
    char *filename_new;

    if (net == NULL) {
        goto l_err_args;
    }

    init_data = zwscn_get_init_data(net);
    if (init_data == NULL) {
        goto l_err;
    }
    dir_path = (char *) init_data->init_input->persistent_data_path;
    if ((dir_path == NULL) || (strlen(dir_path) == 0)) {
        dir_path = "."; //current directory
    }
    dp = zwscn_filesys_open_dir(dir_path);
    if (dp == NULL) {
        zwscn_message_log_critical("Could not open dir %s\n", dir_path);
        goto l_err_dir;
    }

    while ((filename = zwscn_filesys_read_dir_next_content_name(dp)) != NULL) {

        if ((strlen(filename) != ZWSCN_FILENAME_EXACT_LEN) || (strstr(filename, ZWSCN_FILENAME_EXT) == NULL)) {
            //zwscn_message_log_debug("File %s does not seem to be a zwave scene file. Ignoring it...\n", filename);
            continue;
        }
        else {
            if (strncmp(filename, init_data->init_input->context_persistent, (ZWSCN_LEN_CONTEXT_PERSISTENT - 1))) {
                //zwscn_message_log_debug("File %s differs with current network context (%s). Ignoring this file.\n", filename, init_data->init_input->context_persistent);
                continue;
            }
            else {
                //zwscn_message_log_debug("File %s found\n", filename);
            }
        }

        filename_new = malloc(strlen(filename) + 1);
        if (filename_new == NULL) {
            goto l_err_memory;
        }
        zwscn_util_strcpy(filename_new, filename, (strlen(filename) + 1));
        (*out_list) = zwscn_list_insert_sorted_strings((*out_list), (void *) filename_new);
    }

    zwscn_filesys_close_dir(dp);

    return ZWSCN_ERR_NONE;
    l_err_args: return ZWSCN_ERR_INVALID_ARGS;
    l_err: return ZWSCN_ERR_GENERIC;
    l_err_dir: return ZWSCN_ERR_FILE_OPEN;
    l_err_memory: return ZWSCN_ERR_MEMORY;
}

ZWSCN_STATUS_T zwscn_persistence_delete_all_scenes(void *net, zwscn_list_t scene_list) {
    zwscn_scene_cont_p scene_ptr;
    uint32_t scene_id;
    int count = 0;
    int i = 0;

    if ((net == NULL) || (scene_list == NULL)) {
        goto l_err_args;
    }

    count = zwscn_list_size(scene_list); //If list is NULL, count should be 0
    for (i = 0; i < count; ++i) {
        scene_ptr = zwscn_list_get_by_index(scene_list, i);
        if (scene_ptr == NULL) {
            goto l_err;
        }
        scene_id = scene_ptr->scene->id;
        if (zwscn_persistence_delete_scene(net, scene_id) != ZWSCN_ERR_NONE) {
            goto l_err;
        }
    }

    return ZWSCN_ERR_NONE;
    l_err_args: return ZWSCN_ERR_INVALID_ARGS;
    l_err: return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_persistence_delete_scene(void *net, uint32_t scene_id) {
    char scene_filename[ZWSCN_FILENAME_PATH_LEN];
    char *ptr_filename;
    int32_t result;

    if ((net == NULL) || (scene_id == 0)) {
        goto l_err_args;
    }

    //zwscn_message_log_debug("Going to delete scene id=%d from persistent storage\n", scene_id);

    ptr_filename = scene_filename;
    zwscn_persistence_get_absolute_filename_from_scene_id(net, scene_id, &ptr_filename);
    zwscn_message_log_debug("Deleting file %s\n", ptr_filename);

    result = zwscn_persistence_delete_file(ptr_filename);
    if (result != ZWSCN_ERR_NONE) {
        zwscn_message_log_debug("[Error] Can't delete file error=(%d)\n", result);
        goto l_err_file;
    }
    //Update Security Scenes if it contains scene_id of the deleted Scene
    zwscn_security_scene_update_on_scene_deletion(net, scene_id);

    return ZWSCN_ERR_NONE;
    l_err_args: return ZWSCN_ERR_INVALID_ARGS;
    l_err_file: return ZWSCN_ERR_FILE;
}

ZWSCN_STATUS_T zwscn_persistence_get_absolute_filename_from_scene_id(void *net, uint32_t scene_id, char **out_absolute_filename) {
    zwscn_init_p init_data;
    char *dir_path;

    if ((net == NULL) || (scene_id == 0) || ((*out_absolute_filename) == NULL)) {
        goto l_err_args;
    }

    init_data = zwscn_get_init_data(net);
    if (init_data == NULL) {
        goto l_err;
    }

    dir_path = (char *) init_data->init_input->persistent_data_path;
    if ((dir_path == NULL) || (strlen(dir_path) == 0)) {
        dir_path = "."; //current directory
    }
    snprintf((*out_absolute_filename), ZWSCN_FILENAME_PATH_LEN, "%s/%s%08d%s", dir_path, init_data->init_input->context_persistent, scene_id,
            ZWSCN_FILENAME_EXT);
    //zwscn_message_log_debug("Absolute filename = %s for scene id = %d\n", (*out_absolute_filename), scene_id);

    return ZWSCN_ERR_NONE;
    l_err_args: return ZWSCN_ERR_INVALID_ARGS;
    l_err: return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_persistence_get_absolute_filename(void *net, const char *filename, char **out_absolute_filename) {
    zwscn_init_p init_data;
    char *dir_path;

    if ((net == NULL) || (filename == NULL) || ((*out_absolute_filename) == NULL)) {
        goto l_err_args;
    }

    init_data = zwscn_get_init_data(net);
    if (init_data == NULL) {
        goto l_err;
    }

    dir_path = (char *) init_data->init_input->persistent_data_path;
    if ((dir_path == NULL) || (strlen(dir_path) == 0)) {
        dir_path = "."; //current directory
    }
    snprintf((*out_absolute_filename), ZWSCN_FILENAME_PATH_LEN, "%s/%s", dir_path, filename);
    //zwscn_message_log_debug("Absolute filename = %s\n", (*out_absolute_filename));

    return ZWSCN_ERR_NONE;
    l_err_args: return ZWSCN_ERR_INVALID_ARGS;
    l_err: return ZWSCN_ERR_GENERIC;
}

FILE *zwscn_persistence_open_file(const char *filename, uint8_t filemode) {
    FILE *file;
//	int result;

    if (!filename) {
        return NULL;
    }

    if (filemode == ZWSCN_FILEMODE_READ) {
        file = fopen(filename, "rb");
    }
    else {
        file = fopen(filename, "wb");
    }

    return file;
}

int zwscn_persistence_delete_file(char *filename) {
    return remove(filename);
}

void zwscn_persistence_print_file_list(zwscn_list_t list) {
    int count;
    int i;
    const char *filename;

    count = zwscn_list_size(list); //If list is NULL, count should be 0
    for (i = 0; i < count; ++i) {
        filename = zwscn_list_get_by_index(list, i);
        zwscn_message_log_debug("File[%d]: %s\n", i, filename);
    }
}

ZWSCN_STATUS_T zwscn_persistence_load_configuration(void *net) {
    zwscn_init_p init_data;
    char file_path[ZWSCN_FILENAME_PATH_LEN] = { 0 };
    zwscn_config_file_t config_file = NULL;
    char *dir_path = NULL;

    ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
    if (net == NULL) {
        goto l_err_args;
    }

    init_data = zwscn_get_init_data(net);
    if (init_data == NULL) {
        goto l_err;
    }
    dir_path = (char *) init_data->init_input->persistent_data_path;
    if ((dir_path == NULL) || (strlen(dir_path) == 0)) {
        dir_path = "."; //current directory
    }
    zwscn_util_strcat(file_path, dir_path, sizeof(file_path));
    zwscn_util_strcat(file_path, "/", sizeof(file_path));
    zwscn_util_strcat(file_path, ZWSCN_CONFIG_FILENAME, sizeof(file_path));
    zwscn_message_log_debug("CONFIGFILE Loading config file: %s\n", file_path);
    config_file = zwscn_util_config_file_load(file_path);
    if (config_file == NULL) {
        zwscn_message_log_critical("CONFIGFILE Could not open config file %s\n", file_path);
        goto l_err_invalid_file;
    }
    //Email notification
    zwscn_message_log_debug("CONFIGFILE Loading group: %s, key: %s\t", ZWSCN_CONFIG_GROUP_SS_NOTIFICATION_EMAIL, ZWSCN_CONFIG_KEY_ENABLE);
    if (zwscn_util_config_file_has_key(config_file, ZWSCN_CONFIG_GROUP_SS_NOTIFICATION_EMAIL, ZWSCN_CONFIG_KEY_ENABLE)) {
        init_data->config.security_scene_notification_email_enable = zwscn_util_config_file_get_value_boolean(config_file,
                ZWSCN_CONFIG_GROUP_SS_NOTIFICATION_EMAIL, ZWSCN_CONFIG_KEY_ENABLE);
        zwscn_message_log_debug("found value: %u\n", init_data->config.security_scene_notification_email_enable);
    }
    else {
        init_data->config.security_scene_notification_email_enable = ZWSCN_FALSE;
        zwscn_message_log_debug("NOT found\n");
    }
    zwscn_message_log_debug("CONFIGFILE Loading group: %s, key: %s\t", ZWSCN_CONFIG_GROUP_SS_NOTIFICATION_EMAIL, ZWSCN_CONFIG_KEY_SENDER_EMAIL_ADDR);
    if (zwscn_util_config_file_has_key(config_file, ZWSCN_CONFIG_GROUP_SS_NOTIFICATION_EMAIL, ZWSCN_CONFIG_KEY_SENDER_EMAIL_ADDR)) {
        init_data->config.security_scene_notification_email_sender_addr = zwscn_util_config_file_get_value_string(config_file,
                ZWSCN_CONFIG_GROUP_SS_NOTIFICATION_EMAIL, ZWSCN_CONFIG_KEY_SENDER_EMAIL_ADDR);
        zwscn_message_log_debug("found value: %s\n", init_data->config.security_scene_notification_email_sender_addr);
    }
    else {
        init_data->config.security_scene_notification_email_sender_addr = ZWSCN_SENDER_EMAIL_ADDR;
        zwscn_message_log_debug("NOT found\n");
    }
    //SMS notification
    zwscn_message_log_debug("CONFIGFILE Loading group: %s, key: %s\t", ZWSCN_CONFIG_GROUP_SS_NOTIFICATION_SMS, ZWSCN_CONFIG_KEY_ENABLE);
    if (zwscn_util_config_file_has_key(config_file, ZWSCN_CONFIG_GROUP_SS_NOTIFICATION_SMS, ZWSCN_CONFIG_KEY_ENABLE)) {
        init_data->config.security_scene_notification_sms_enable = zwscn_util_config_file_get_value_boolean(config_file, ZWSCN_CONFIG_GROUP_SS_NOTIFICATION_SMS,
                ZWSCN_CONFIG_KEY_ENABLE);
        zwscn_message_log_debug("found value: %u\n", init_data->config.security_scene_notification_sms_enable);
    }
    else {
        init_data->config.security_scene_notification_sms_enable = ZWSCN_FALSE;
        zwscn_message_log_debug("NOT found\n");
    }
    zwscn_message_log_debug("CONFIGFILE Loading group: %s, key: %s\t", ZWSCN_CONFIG_GROUP_SS_NOTIFICATION_SMS, ZWSCN_CONFIG_KEY_SENDER_EMAIL_ADDR);
    if (zwscn_util_config_file_has_key(config_file, ZWSCN_CONFIG_GROUP_SS_NOTIFICATION_SMS, ZWSCN_CONFIG_KEY_SENDER_EMAIL_ADDR)) {
        init_data->config.security_scene_notification_sms_sender_addr = zwscn_util_config_file_get_value_string(config_file,
                ZWSCN_CONFIG_GROUP_SS_NOTIFICATION_SMS, ZWSCN_CONFIG_KEY_SENDER_EMAIL_ADDR);
        zwscn_message_log_debug("found value: %s\n", init_data->config.security_scene_notification_sms_sender_addr);
    }
    else {
        init_data->config.security_scene_notification_sms_sender_addr = ZWSCN_SENDER_EMAIL_ADDR_FOR_SMS;
        zwscn_message_log_debug("NOT found\n");
    }
    zwscn_message_log_debug("CONFIGFILE Loading group: %s, key: %s\t", ZWSCN_CONFIG_GROUP_SS_NOTIFICATION_SMS, ZWSCN_CONFIG_KEY_SMS_GATEWAY);
    if (zwscn_util_config_file_has_key(config_file, ZWSCN_CONFIG_GROUP_SS_NOTIFICATION_SMS, ZWSCN_CONFIG_KEY_SMS_GATEWAY)) {
        init_data->config.security_scene_notification_sms_gateway = zwscn_util_config_file_get_value_string(config_file, ZWSCN_CONFIG_GROUP_SS_NOTIFICATION_SMS,
                ZWSCN_CONFIG_KEY_SMS_GATEWAY);
        zwscn_message_log_debug("found value: %s\n", init_data->config.security_scene_notification_sms_gateway);
    }
    else {
        init_data->config.security_scene_notification_sms_enable = ZWSCN_FALSE;
        zwscn_message_log_debug("NOT found\n");
    }
    //SMTP settings for email
    zwscn_message_log_debug("CONFIGFILE Loading group: %s, key: %s\t", ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_ENABLE);
    if (zwscn_util_config_file_has_key(config_file, ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_ENABLE)) {
        init_data->config.smtp_enable = zwscn_util_config_file_get_value_boolean(config_file, ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_ENABLE);
        zwscn_message_log_debug("found value: %u\n", init_data->config.smtp_enable);
    }
    else {
        init_data->config.smtp_enable = ZWSCN_FALSE;
        zwscn_message_log_debug("NOT found\n");
    }
    zwscn_message_log_debug("CONFIGFILE Loading group: %s, key: %s\t", ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_AUTH_ENABLE);
    if (zwscn_util_config_file_has_key(config_file, ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_AUTH_ENABLE)) {
        init_data->config.smtp_auth_enable = zwscn_util_config_file_get_value_boolean(config_file, ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_AUTH_ENABLE);
        zwscn_message_log_debug("found value: %u\n", init_data->config.smtp_auth_enable);
    }
    else {
        init_data->config.smtp_auth_enable = ZWSCN_FALSE;
        zwscn_message_log_debug("NOT found\n");
    }
    zwscn_message_log_debug("CONFIGFILE Loading group: %s, key: %s\t", ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_USERNAME);
    if (zwscn_util_config_file_has_key(config_file, ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_USERNAME)) {
        init_data->config.smtp_username = zwscn_util_config_file_get_value_string(config_file, ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_USERNAME);
        zwscn_message_log_debug("found value: %s\n", init_data->config.smtp_username);
    }
    else {
        init_data->config.smtp_username = NULL;
        zwscn_message_log_debug("NOT found\n");
    }
    zwscn_message_log_debug("CONFIGFILE Loading group: %s, key: %s\t", ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_PASSWORD);
    if (zwscn_util_config_file_has_key(config_file, ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_PASSWORD)) {
        init_data->config.smtp_password = zwscn_util_config_file_get_value_string(config_file, ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_PASSWORD);
        zwscn_message_log_debug("found value: %s\n", init_data->config.smtp_password);
    }
    else {
        init_data->config.smtp_password = NULL;
        zwscn_message_log_debug("NOT found\n");
    }
    zwscn_message_log_debug("CONFIGFILE Loading group: %s, key: %s\t", ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_SERVER_HOSTNAME);
    if (zwscn_util_config_file_has_key(config_file, ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_SERVER_HOSTNAME)) {
        init_data->config.smtp_server_hostname = zwscn_util_config_file_get_value_string(config_file, ZWSCN_CONFIG_GROUP_SMTP,
                ZWSCN_CONFIG_KEY_SERVER_HOSTNAME);
        zwscn_message_log_debug("found value: %s\n", init_data->config.smtp_server_hostname);
    }
    else {
        init_data->config.smtp_server_hostname = NULL;
        zwscn_message_log_debug("NOT found\n");
    }
    zwscn_message_log_debug("CONFIGFILE Loading group: %s, key: %s\t", ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_SERVER_PORT);
    if (zwscn_util_config_file_has_key(config_file, ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_SERVER_PORT)) {
        init_data->config.smtp_server_port = zwscn_util_config_file_get_value_string(config_file, ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_SERVER_PORT);
        zwscn_message_log_debug("found value: %s\n", init_data->config.smtp_server_port);
    }
    else {
        init_data->config.smtp_server_port = NULL;
        zwscn_message_log_debug("NOT found\n");
    }
    zwscn_message_log_debug("CONFIGFILE Loading group: %s, key: %s\t", ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_SECURE_METHOD);
    if (zwscn_util_config_file_has_key(config_file, ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_SECURE_METHOD)) {
        init_data->config.smtp_secure_method = zwscn_util_config_file_get_value_string(config_file, ZWSCN_CONFIG_GROUP_SMTP, ZWSCN_CONFIG_KEY_SECURE_METHOD);
        zwscn_message_log_debug("found value: %s\n", init_data->config.smtp_secure_method);
    }
    else {
        init_data->config.smtp_secure_method = NULL;
        zwscn_message_log_debug("NOT found\n");
    }
	//COMMON
	zwscn_message_log_debug("CONFIGFILE Loading group: %s, key: %s\t", ZWSCN_CONFIG_GROUP_COMMON, ZWSCN_CONFIG_KEY_ZWAVE_SAME_REPORT_IGNORE_INTERVAL);
    if (zwscn_util_config_file_has_key(config_file, ZWSCN_CONFIG_GROUP_COMMON, ZWSCN_CONFIG_KEY_ZWAVE_SAME_REPORT_IGNORE_INTERVAL)) {
        init_data->config.zwave_same_report_ignore_interval = zwscn_util_config_file_get_value_integer(config_file, ZWSCN_CONFIG_GROUP_COMMON, ZWSCN_CONFIG_KEY_ZWAVE_SAME_REPORT_IGNORE_INTERVAL);
        zwscn_message_log_debug("found value: %u\n", init_data->config.zwave_same_report_ignore_interval);
    }
    else {
        init_data->config.zwave_same_report_ignore_interval = ZWSCN_ZWAVE_SAME_REPORT_IGNORE_INTERVAL;
        zwscn_message_log_debug("NOT found\n");
    }

    zwscn_util_config_file_free(config_file);

    return ZWSCN_ERR_NONE;
    l_err_args: return ZWSCN_ERR_INVALID_ARGS;
    l_err: return ZWSCN_ERR_GENERIC;
    l_err_invalid_file: return ZWSCN_ERR_FILE;
}
