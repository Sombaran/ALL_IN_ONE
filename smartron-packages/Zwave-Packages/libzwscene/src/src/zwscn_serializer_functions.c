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
 @file       zwscn_serializer_functions.c - serializer functions for classes

 To provide serialization functions for known classes

 @author     Mafahir Fairoze

 @version    1.0 2016-01-18  Initial release
 */

#include <stdlib.h>
#include "zwscn_scene_api.h"
#include "zip_api.h"
#include "zwscn_device_args.h"
#include "ZW_classcmd.h"
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
#include "zwscn_serializer_functions.h"
#include "zwscn_xml.h"

static char strbuf[sizeof(uint32_t) * 10 + 1];

void zwscn_args_basic_set_serializer(xml_writer_t* writer, void* value) {
    zwscn_args_basic_set_t* param = value;
    if (!param) return;
    xml_writer_write_start_element(writer, "basic_set");
    xml_writer_write_attribute(writer, "value", xml_uint8_to_string(strbuf, param->value));
    xml_writer_write_end_element(writer);
}

void zwscn_args_switch_binary_set_serializer(xml_writer_t* writer, void* value) {
    zwscn_args_switch_binary_set_t* param = value;
    if (!param) return;
    xml_writer_write_start_element(writer, "switch_binary_set");
    xml_writer_write_attribute(writer, "value", xml_uint8_to_string(strbuf, param->value));
    xml_writer_write_end_element(writer);
}

void zwscn_args_switch_multilevel_set_serializer(xml_writer_t* writer, void* value) {
    zwscn_args_switch_multilevel_set_t* param = value;
    if (!param) return;
    xml_writer_write_start_element(writer, "switch_multilevel_set");
    xml_writer_write_attribute(writer, "value", xml_uint8_to_string(strbuf, param->value));
    xml_writer_write_end_element(writer);
}

void zwscn_args_door_lock_operation_set_serializer(xml_writer_t* writer, void* value) {
    zwscn_args_door_lock_operation_set_t* param = value;
    if (!param) return;
    xml_writer_write_start_element(writer, "door_lock_operation_set");
    xml_writer_write_attribute(writer, "mode", xml_uint8_to_string(strbuf, param->mode));
    xml_writer_write_end_element(writer);
}

void zwscn_args_thermostat_setpoint_set_serializer(xml_writer_t* writer, void* value) {
    zwscn_args_thermostat_setpoint_set_t* param = value;
    if (!param) return;
    xml_writer_write_start_element(writer, "thermostat_setpoint_set");
    XML_SERIALIZER_SERIALIZE(writer, zwsetp_t, &param->setpoint);
    xml_writer_write_end_element(writer);
}

void zwsetp_serializer(xml_writer_t* writer, void* value) {
    zwsetp_t* param = value;
    if (!param) return;
    xml_writer_write_start_element(writer, "setp");
    xml_writer_write_attribute(writer, "unit", xml_uint8_to_string(strbuf, param->unit));
    xml_writer_write_attribute(writer, "type", xml_uint8_to_string(strbuf, param->type));
    xml_writer_write_attribute(writer, "precision", xml_uint8_to_string(strbuf, param->precision));
    xml_writer_write_attribute(writer, "size", xml_uint8_to_string(strbuf, param->size));
    xml_writer_write_attribute(writer, "data", xml_int32_to_string(strbuf, zwscn_util_data2value(param->size, param->data)));
    xml_writer_write_end_element(writer);
}

void zwscn_args_sensor_binary_serializer(xml_writer_t* writer, void* value) {
    zwscn_args_sensor_binary_t* param = value;
    if (!param) return;
    xml_writer_write_start_element(writer, "sensor_binary");
    xml_writer_write_attribute(writer, "type", xml_uint8_to_string(strbuf, param->type));
    xml_writer_write_attribute(writer, "value", xml_uint8_to_string(strbuf, param->sensor_value));
    xml_writer_write_end_element(writer);
}

void zwscn_args_sensor_multilevel_serializer(xml_writer_t* writer, void* value) {
    zwscn_args_sensor_multilevel_t* param = value;
    if (!param) return;
    xml_writer_write_start_element(writer, "sensor_multilevel");
    xml_writer_write_start_element(writer, "value_low");
    XML_SERIALIZER_SERIALIZE(writer, zwsensor_t, &param->value_low);
    xml_writer_write_end_element(writer);
    xml_writer_write_start_element(writer, "value_high");
    XML_SERIALIZER_SERIALIZE(writer, zwsensor_t, &param->value_high);
    xml_writer_write_end_element(writer);
    xml_writer_write_end_element(writer);
}

void zwsensor_serializer(xml_writer_t* writer, void* value) {
    zwsensor_t* param = value;
    if (!param) return;
    xml_writer_write_start_element(writer, "sensor");
    xml_writer_write_attribute(writer, "unit", xml_uint8_to_string(strbuf, param->unit));
    xml_writer_write_attribute(writer, "type", xml_uint8_to_string(strbuf, param->type));
    xml_writer_write_attribute(writer, "precision", xml_uint8_to_string(strbuf, param->precision));
    xml_writer_write_attribute(writer, "size", xml_uint8_to_string(strbuf, param->size));
    xml_writer_write_attribute(writer, "data", xml_int32_to_string(strbuf, zwscn_util_data2value(param->size, param->data)));
    xml_writer_write_end_element(writer);
}

void zwscn_args_alarm_serializer(xml_writer_t* writer, void* value) {
    zwscn_args_alarm_t* param = value;
    size_t i = 0;
    if (!param) return;
    xml_writer_write_start_element(writer, "alarm");
    xml_writer_write_attribute(writer, "va_level", xml_uint8_to_string(strbuf, param->va_level));
    xml_writer_write_attribute(writer, "va_type", xml_uint8_to_string(strbuf, param->va_type));
    xml_writer_write_start_element(writer, "za_events");
    xml_writer_write_attribute(writer, "za_type", xml_uint8_to_string(strbuf, param->za_type));
    xml_writer_write_attribute(writer, "za_event_count", xml_uint32_to_string(strbuf, param->za_event_count));
    for (i = 0; i < param->za_event_count; ++i) {
        xml_writer_write_start_element(writer, "za_event");
        xml_writer_write_attribute(writer, "value", xml_uint8_to_string(strbuf, param->za_event[i]));
        xml_writer_write_end_element(writer);
    }
    xml_writer_write_end_element(writer);
    xml_writer_write_end_element(writer);
}

void zwscn_args_basic_event_serializer(xml_writer_t* writer, void* value) {
    zwscn_args_basic_event_t* param = value;
    if (!param) return;
    xml_writer_write_start_element(writer, "basic_event");
    xml_writer_write_attribute(writer, "value_low", xml_uint8_to_string(strbuf, param->value_low));
    xml_writer_write_attribute(writer, "value_high", xml_uint8_to_string(strbuf, param->value_high));
    xml_writer_write_end_element(writer);
}

void zwscn_args_central_scene_serializer(xml_writer_t* writer, void* value) {
    zwscn_args_central_scene_t* param = value;
    if (!param) return;
    xml_writer_write_start_element(writer, "central_scene");
    xml_writer_write_attribute(writer, "central_scene_key_attribute", xml_uint8_to_string(strbuf, param->central_scene_key_attribute));
    xml_writer_write_attribute(writer, "central_scene_key_number", xml_uint8_to_string(strbuf, param->central_scene_key_number));
    xml_writer_write_end_element(writer);
}

void zwscn_serializer_device_args(xml_writer_t* writer, void *device_args, uint32_t id, uint32_t cmd) {
    if (device_args) {
        uint16_t cls_id = zwscn_util_combo_id_to_cls_id(id);
        switch (cls_id) {
        case COMMAND_CLASS_BASIC:
            if (cmd == BASIC_SET)
                XML_SERIALIZER_SERIALIZE(writer, zwscn_args_basic_event_t, device_args);
            else
            XML_SERIALIZER_SERIALIZE(writer, zwscn_args_basic_set_t, device_args);
            break;
        case COMMAND_CLASS_SWITCH_BINARY:
            XML_SERIALIZER_SERIALIZE(writer, zwscn_args_switch_binary_set_t, device_args);
            break;
        case COMMAND_CLASS_SWITCH_MULTILEVEL:
            XML_SERIALIZER_SERIALIZE(writer, zwscn_args_switch_multilevel_set_t, device_args);
            break;
        case COMMAND_CLASS_DOOR_LOCK:
            XML_SERIALIZER_SERIALIZE(writer, zwscn_args_door_lock_operation_set_t, device_args);
            break;
        case COMMAND_CLASS_THERMOSTAT_SETPOINT:
            XML_SERIALIZER_SERIALIZE(writer, zwscn_args_thermostat_setpoint_set_t, device_args);
            break;
        case COMMAND_CLASS_SENSOR_BINARY:
            XML_SERIALIZER_SERIALIZE(writer, zwscn_args_sensor_binary_t, device_args);
            break;
        case COMMAND_CLASS_SENSOR_MULTILEVEL:
            XML_SERIALIZER_SERIALIZE(writer, zwscn_args_sensor_multilevel_t, device_args);
            break;
        case COMMAND_CLASS_ALARM:
            XML_SERIALIZER_SERIALIZE(writer, zwscn_args_alarm_t, device_args);
            break;
        case COMMAND_CLASS_CENTRAL_SCENE:
            XML_SERIALIZER_SERIALIZE(writer, zwscn_args_central_scene_t, device_args);
            break;
        }
    }
}

void zwscn_scene_cont_serializer(xml_writer_t* writer, void* value) {
    zwscn_scene_cont_t* param = value;
    size_t i, count = 0;
    if (!param) return;
    xml_writer_write_start_element(writer, "scene");
    //XML_SERIALIZER_SERIALIZE(writer, zwscn_scene_t, param->scene);
    xml_writer_write_attribute(writer, "id", xml_uint32_to_string(strbuf, param->scene->id));
    xml_writer_write_attribute(writer, "name", zwscn_util_unescape_uri(param->scene->name));
    xml_writer_write_attribute(writer, "active", xml_uint8_to_string(strbuf, param->scene->active));
    xml_writer_write_start_element(writer, "actions");
    count = zwscn_list_size(param->list_actions);
    for (i = 0; i < count; ++i) {
        zwscn_action_t* action = zwscn_list_get_by_index(param->list_actions, i);
        XML_SERIALIZER_SERIALIZE(writer, zwscn_action_t, action);
    }
    count = zwscn_list_size(param->list_actions_invalid);
    for (i = 0; i < count; ++i) {
        zwscn_action_t* action = zwscn_list_get_by_index(param->list_actions_invalid, i);
        XML_SERIALIZER_SERIALIZE(writer, zwscn_action_t, action);
    }
    xml_writer_write_end_element(writer);
    xml_writer_write_start_element(writer, "events");
    count = zwscn_list_size(param->list_events);
    for (i = 0; i < count; ++i) {
        zwscn_event_t* event = zwscn_list_get_by_index(param->list_events, i);
        XML_SERIALIZER_SERIALIZE(writer, zwscn_event_t, event);
    }
    count = zwscn_list_size(param->list_events_invalid);
    for (i = 0; i < count; ++i) {
        zwscn_event_t* event = zwscn_list_get_by_index(param->list_events_invalid, i);
        XML_SERIALIZER_SERIALIZE(writer, zwscn_event_t, event);
    }
    xml_writer_write_end_element(writer);
    xml_writer_write_start_element(writer, "schedules");
    count = zwscn_list_size(param->list_schedules);
    for (i = 0; i < count; ++i) {
        zwscn_schedule_t* schedule = zwscn_list_get_by_index(param->list_schedules, i);
        XML_SERIALIZER_SERIALIZE(writer, zwscn_schedule_t, schedule);
    }
    xml_writer_write_end_element(writer);
    xml_writer_write_end_element(writer);
}

void zwscn_scene_serializer(xml_writer_t* writer, void* value) {
    zwscn_scene_t* param = value;
    char * str = NULL;
    if (!param) return;
    xml_writer_write_start_element(writer, "scene");
    xml_writer_write_attribute(writer, "id", xml_uint32_to_string(strbuf, param->id));
    str = zwscn_util_unescape_uri(param->name);
    xml_writer_write_attribute(writer, "name", str);
    free(str);
    xml_writer_write_attribute(writer, "active", xml_uint8_to_string(strbuf, param->active));
    xml_writer_write_end_element(writer);
}

void zwscn_action_serializer(xml_writer_t* writer, void* value) {
    zwscn_action_t* param = value;
    if (!param) return;
    xml_writer_write_start_element(writer, "action");
    xml_writer_write_attribute(writer, "id", xml_uint32_to_string(strbuf, param->id));
    xml_writer_write_attribute(writer, "type", xml_uint8_to_string(strbuf, param->type));
    xml_writer_write_attribute(writer, "cmd", xml_uint8_to_string(strbuf, param->cmd));
    zwscn_serializer_device_args(writer, param->device_args, param->id, param->cmd);
    xml_writer_write_end_element(writer);
}

void zwscn_event_serializer(xml_writer_t* writer, void* value) {
    zwscn_event_t* param = value;
    if (!param) return;
    xml_writer_write_start_element(writer, "event");
    xml_writer_write_attribute(writer, "id", xml_uint32_to_string(strbuf, param->id));
    xml_writer_write_attribute(writer, "type", xml_uint8_to_string(strbuf, param->type));
    xml_writer_write_attribute(writer, "cmd", xml_uint8_to_string(strbuf, param->cmd));
    zwscn_serializer_device_args(writer, param->device_args, param->id, param->cmd);
    xml_writer_write_end_element(writer);
}

void zwscn_schedule_serializer(xml_writer_t* writer, void* value) {
    zwscn_schedule_t* param = value;
    if (!param) return;
    xml_writer_write_start_element(writer, "schedule");
    xml_writer_write_attribute(writer, "id", xml_uint32_to_string(strbuf, param->id));
    xml_writer_write_attribute(writer, "type", xml_uint8_to_string(strbuf, param->type));
    xml_writer_write_attribute(writer, "day", xml_uint8_to_string(strbuf, param->day));
    xml_writer_write_attribute(writer, "hour", xml_uint8_to_string(strbuf, param->hour));
    xml_writer_write_attribute(writer, "minute", xml_uint8_to_string(strbuf, param->minute));
    xml_writer_write_end_element(writer);
}

void zwscn_security_scene_cont_serializer(xml_writer_t* writer, void* value) {
    zwscn_security_scene_cont_t* param = value;
    size_t i, count = 0;
    if (!param) return;
    xml_writer_write_start_element(writer, "security_scene");
    XML_SERIALIZER_SERIALIZE(writer, zwscn_security_scene_t, param->scene);

    xml_writer_write_start_element(writer, "list_arm_events");
    count = zwscn_list_size(param->list_arm_events);
    for (i = 0; i < count; ++i) {
        zwscn_security_event_t* event = zwscn_list_get_by_index(param->list_arm_events, i);
        XML_SERIALIZER_SERIALIZE(writer, zwscn_security_event_t, event);
    }
    count = zwscn_list_size(param->list_arm_events_invalid);
    for (i = 0; i < count; ++i) {
        zwscn_security_event_t* event = zwscn_list_get_by_index(param->list_arm_events_invalid, i);
        XML_SERIALIZER_SERIALIZE(writer, zwscn_security_event_t, event);
    }
    xml_writer_write_end_element(writer);

    xml_writer_write_start_element(writer, "list_disarm_events");
    count = zwscn_list_size(param->list_disarm_events);
    for (i = 0; i < count; ++i) {
        zwscn_security_event_t* event = zwscn_list_get_by_index(param->list_disarm_events, i);
        XML_SERIALIZER_SERIALIZE(writer, zwscn_security_event_t, event);
    }
    count = zwscn_list_size(param->list_disarm_events_invalid);
    for (i = 0; i < count; ++i) {
        zwscn_security_event_t* event = zwscn_list_get_by_index(param->list_disarm_events_invalid, i);
        XML_SERIALIZER_SERIALIZE(writer, zwscn_security_event_t, event);
    }
    xml_writer_write_end_element(writer);

    xml_writer_write_start_element(writer, "list_alarm_events");
    count = zwscn_list_size(param->list_alarm_events);
    for (i = 0; i < count; ++i) {
        zwscn_security_event_t* event = zwscn_list_get_by_index(param->list_alarm_events, i);
        XML_SERIALIZER_SERIALIZE(writer, zwscn_security_event_t, event);
    }
    count = zwscn_list_size(param->list_alarm_events_invalid);
    for (i = 0; i < count; ++i) {
        zwscn_security_event_t* event = zwscn_list_get_by_index(param->list_alarm_events_invalid, i);
        XML_SERIALIZER_SERIALIZE(writer, zwscn_security_event_t, event);
    }
    xml_writer_write_end_element(writer);

    xml_writer_write_end_element(writer);
}

void zwscn_security_scene_serializer(xml_writer_t* writer, void* value) {
    zwscn_security_scene_t* param = value;
    char* str = NULL;
    if (!param) return;
    xml_writer_write_attribute(writer, "id", xml_uint32_to_string(strbuf, param->id));
    str = zwscn_util_unescape_uri(param->name);
    xml_writer_write_attribute(writer, "name", str);
    free(str);
    xml_writer_write_attribute(writer, "active", xml_uint8_to_string(strbuf, param->active));
    xml_writer_write_attribute(writer, "is_armed", xml_uint8_to_string(strbuf, param->is_armed));
    xml_writer_write_attribute(writer, "is_alarmed", xml_uint8_to_string(strbuf, param->is_alarmed));
    xml_writer_write_attribute(writer, "scene_id_at_arm", xml_uint32_to_string(strbuf, param->scene_id_at_arm));
    xml_writer_write_attribute(writer, "scene_id_at_disarm", xml_uint32_to_string(strbuf, param->scene_id_at_disarm));
    xml_writer_write_attribute(writer, "scene_id_at_alarm", xml_uint32_to_string(strbuf, param->scene_id_at_alarm));
    xml_writer_write_attribute(writer, "is_notification_on", xml_uint8_to_string(strbuf, param->is_notification_on));
    xml_writer_write_attribute(writer, "is_notification_by_sms_on", xml_uint8_to_string(strbuf, param->is_notification_by_sms_on));
    xml_writer_write_attribute(writer, "is_notification_by_email_on", xml_uint8_to_string(strbuf, param->is_notification_by_email_on));
    str = zwscn_util_unescape_uri(param->notification_sms_number);
    xml_writer_write_attribute(writer, "notification_sms_number", str);
    free(str);
    str = zwscn_util_unescape_uri(param->notification_email);
    xml_writer_write_attribute(writer, "notification_email", str);
    free(str);
    xml_writer_write_attribute(writer, "utime", xml_uint64_to_string(strbuf, param->utime));
    xml_writer_write_start_element(writer, "lt_arm");
    XML_SERIALIZER_SERIALIZE(writer, zwscn_security_scene_last_trigger_t, &param->lt_arm);
    xml_writer_write_end_element(writer);
    xml_writer_write_start_element(writer, "lt_disarm");
    XML_SERIALIZER_SERIALIZE(writer, zwscn_security_scene_last_trigger_t, &param->lt_disarm);
    xml_writer_write_end_element(writer);
    xml_writer_write_start_element(writer, "lt_alarm_on");
    XML_SERIALIZER_SERIALIZE(writer, zwscn_security_scene_last_trigger_t, &param->lt_alarm_on);
    xml_writer_write_end_element(writer);
    xml_writer_write_start_element(writer, "lt_alarm_off");
    XML_SERIALIZER_SERIALIZE(writer, zwscn_security_scene_last_trigger_t, &param->lt_alarm_off);
    xml_writer_write_end_element(writer);
}

void zwscn_security_scene_last_trigger_serializer(xml_writer_t* writer, void* value) {
    zwscn_security_scene_last_trigger_t* param = value;
    if (!param) return;
    xml_writer_write_start_element(writer, "last_trigger");
    xml_writer_write_attribute(writer, "type", xml_uint32_to_string(strbuf, param->type));
    xml_writer_write_attribute(writer, "time", xml_uint64_to_string(strbuf, param->time));
    XML_SERIALIZER_SERIALIZE(writer, zwscn_security_event_t, param->event);
    xml_writer_write_end_element(writer);
}

void zwscn_serializer_functions_register_all_types() {
    XML_SERIALIZER_REGISTER(zwscn_args_basic_set_t, zwscn_args_basic_set_serializer);
    XML_SERIALIZER_REGISTER(zwscn_args_switch_binary_set_t, zwscn_args_switch_binary_set_serializer);
    XML_SERIALIZER_REGISTER(zwscn_args_switch_multilevel_set_t, zwscn_args_switch_multilevel_set_serializer);
    XML_SERIALIZER_REGISTER(zwscn_args_door_lock_operation_set_t, zwscn_args_door_lock_operation_set_serializer);
    XML_SERIALIZER_REGISTER(zwscn_args_thermostat_setpoint_set_t, zwscn_args_thermostat_setpoint_set_serializer);
    XML_SERIALIZER_REGISTER(zwsetp_t, zwsetp_serializer);
    XML_SERIALIZER_REGISTER(zwscn_args_sensor_binary_t, zwscn_args_sensor_binary_serializer);
    XML_SERIALIZER_REGISTER(zwscn_args_sensor_multilevel_t, zwscn_args_sensor_multilevel_serializer);
    XML_SERIALIZER_REGISTER(zwsensor_t, zwsensor_serializer);
    XML_SERIALIZER_REGISTER(zwscn_args_alarm_t, zwscn_args_alarm_serializer);
    XML_SERIALIZER_REGISTER(zwscn_args_basic_event_t, zwscn_args_basic_event_serializer);
    XML_SERIALIZER_REGISTER(zwscn_args_central_scene_t, zwscn_args_central_scene_serializer);

    XML_SERIALIZER_REGISTER(zwscn_scene_cont_t, zwscn_scene_cont_serializer);
    XML_SERIALIZER_REGISTER(zwscn_scene_t, zwscn_scene_serializer);
    XML_SERIALIZER_REGISTER(zwscn_action_t, zwscn_action_serializer);
    XML_SERIALIZER_REGISTER(zwscn_event_t, zwscn_event_serializer);
    XML_SERIALIZER_REGISTER(zwscn_schedule_t, zwscn_schedule_serializer);

    XML_SERIALIZER_REGISTER(zwscn_security_scene_cont_t, zwscn_security_scene_cont_serializer);
    XML_SERIALIZER_REGISTER(zwscn_security_scene_t, zwscn_security_scene_serializer);
    XML_SERIALIZER_REGISTER(zwscn_security_event_t, zwscn_event_serializer);
    XML_SERIALIZER_REGISTER(zwscn_security_scene_last_trigger_t, zwscn_security_scene_last_trigger_serializer);

}

typedef void (*XMLSerializerFunc)(xml_writer_t* reader, void* value);

typedef struct xml_serializer {
    zwscn_hash_t function_map;
} xml_serializer_t;

static xml_serializer_t* xml_serializer_instance = 0;

void xml_serializer_init() {
    if (!xml_serializer_instance) {
        static xml_serializer_t sz;
        sz.function_map = zwscn_hash_new_str_hash();
        xml_serializer_instance = &sz;
    }
}

void xml_serializer_register(char* struct_type, XMLSerializerFunc func) {
    xml_serializer_init();
    zwscn_hash_insert(xml_serializer_instance->function_map, struct_type, func);
}

void xml_serializer_serialize(xml_writer_t* writer, char* struct_type, void* value) {
    XMLSerializerFunc func = zwscn_hash_get_value(xml_serializer_instance->function_map, struct_type);
    if (func) {
        func(writer, value);
    }
}
