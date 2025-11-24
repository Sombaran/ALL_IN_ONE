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
 @file       zwscn_deserializer_functions.c - deserializer functions for classes

 To provide deserialization functions for known classes

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
#include "zwscn_deserializer_functions.h"
#include "zwscn_xml.h"
#include "zwscn_util_zwave.h"
#include "zwscn_security_scene_events.h"
#include <glib.h>
#include <string.h>

//static char strbuf[sizeof(uint32_t) * 10 + 1];

ZWSCN_STATUS_T zwscn_args_basic_set_deserializer(xml_reader_t* reader, void* value) {
    zwscn_args_basic_set_t* param = value;
    xml_exception_t error = XML_EXCEPTION_NONE;
    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "basic_set")) return ZWSCN_ERR_GENERIC;

    while (!(error = xml_reader_next(reader)) &&xml_reader_node_type(reader) == XML_NODE_TYPE_ATTRIBUTE) {
        if (strcmp("value", xml_reader_node_name(reader)) == 0) {
            param->value = xml_string_to_uint8(xml_reader_node_content(reader));
        }
    }
    return ZWSCN_ERR_NONE;
}

ZWSCN_STATUS_T zwscn_args_switch_binary_set_deserializer(xml_reader_t* reader, void* value) {
    zwscn_args_switch_binary_set_t* param = value;
    xml_exception_t error = XML_EXCEPTION_NONE;
    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "switch_binary_set")) return ZWSCN_ERR_GENERIC;

    while (!(error = xml_reader_next(reader)) &&xml_reader_node_type(reader) == XML_NODE_TYPE_ATTRIBUTE) {
        if (strcmp("value", xml_reader_node_name(reader)) == 0) {
            param->value = xml_string_to_uint8(xml_reader_node_content(reader));
        }
    }
    return ZWSCN_ERR_NONE;
}

ZWSCN_STATUS_T zwscn_args_switch_multilevel_set_deserializer(xml_reader_t* reader, void* value) {
    zwscn_args_switch_multilevel_set_t* param = value;
    xml_exception_t error = XML_EXCEPTION_NONE;
    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "switch_multilevel_set")) return ZWSCN_ERR_GENERIC;

    while (!(error = xml_reader_next(reader)) &&xml_reader_node_type(reader) == XML_NODE_TYPE_ATTRIBUTE) {
        if (strcmp("value", xml_reader_node_name(reader)) == 0) {
            param->value = xml_string_to_uint8(xml_reader_node_content(reader));
        }
    }
    return ZWSCN_ERR_NONE;
}

ZWSCN_STATUS_T zwscn_args_door_lock_operation_set_deserializer(xml_reader_t* reader, void* value) {
    zwscn_args_door_lock_operation_set_t* param = value;
    xml_exception_t error = XML_EXCEPTION_NONE;
    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "door_lock_operation_set")) return ZWSCN_ERR_GENERIC;

    while (!(error = xml_reader_next(reader)) &&xml_reader_node_type(reader) == XML_NODE_TYPE_ATTRIBUTE) {
        if (strcmp("mode", xml_reader_node_name(reader)) == 0) {
            param->mode = xml_string_to_uint8(xml_reader_node_content(reader));
        }
    }
    return ZWSCN_ERR_NONE;
}

ZWSCN_STATUS_T zwscn_args_thermostat_setpoint_set_deserializer(xml_reader_t* reader, void* value) {
    ZWSCN_STATUS_T result;
    zwscn_args_thermostat_setpoint_set_t* param = value;
    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "thermostat_setpoint_set")) return ZWSCN_ERR_GENERIC;
    result = XML_DESERIALIZER_DESERIALIZE(reader, zwsetp_t, &param->setpoint);
    if (result != ZWSCN_ERR_NONE) {
        return result;
    }
    return ZWSCN_ERR_NONE;
}

ZWSCN_STATUS_T zwsetp_deserializer(xml_reader_t* reader, void* value) {
    zwsetp_t* param = value;
    xml_exception_t error = XML_EXCEPTION_NONE;
    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "setp")) return ZWSCN_ERR_GENERIC;

    while (!(error = xml_reader_next(reader)) &&xml_reader_node_type(reader) == XML_NODE_TYPE_ATTRIBUTE) {
        if (strcmp("unit", xml_reader_node_name(reader)) == 0) {
            param->unit = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("type", xml_reader_node_name(reader)) == 0) {
            param->type = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("precision", xml_reader_node_name(reader)) == 0) {
            param->precision = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("size", xml_reader_node_name(reader)) == 0) {
            param->size = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("data", xml_reader_node_name(reader)) == 0) {
            int32_t val32 = xml_string_to_int32(xml_reader_node_content(reader));
            if(param->size == 1){
                param->data[0] = (uint8_t)val32;
            }else if(param->size == 2){
                param->data[1] = (uint8_t)val32;
                param->data[0] = (uint8_t)(val32 >> 8);

            }else if(param->size == 4){
                param->data[3] = (uint8_t)val32;
                param->data[2] = (uint8_t)(val32 >> 8);
                param->data[1] = (uint8_t)(val32 >> 16);
                param->data[0] = (uint8_t)(val32 >> 24);
            }
        }
    }
    return ZWSCN_ERR_NONE;
}

ZWSCN_STATUS_T zwscn_args_sensor_binary_deserializer(xml_reader_t* reader, void* value) {
    zwscn_args_sensor_binary_t* param = value;
    xml_exception_t error = XML_EXCEPTION_NONE;
    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "sensor_binary")) return ZWSCN_ERR_GENERIC;

    while (!(error = xml_reader_next(reader)) &&xml_reader_node_type(reader) == XML_NODE_TYPE_ATTRIBUTE) {
        if (strcmp("type", xml_reader_node_name(reader)) == 0) {
            param->type = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("value", xml_reader_node_name(reader)) == 0) {
            param->sensor_value = xml_string_to_uint8(xml_reader_node_content(reader));
        }
    }
    return ZWSCN_ERR_NONE;
}

ZWSCN_STATUS_T zwscn_args_sensor_multilevel_deserializer(xml_reader_t* reader, void* value) {
    zwscn_args_sensor_multilevel_t* param = value;
    xml_exception_t error = XML_EXCEPTION_NONE;
    uint32_t name_hash = zwscn_util_str_hash("sensor_multilevel");
    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "sensor_multilevel")) return ZWSCN_ERR_GENERIC;

    while (!(error = xml_reader_next(reader)) &&!(xml_reader_node_type(reader) == XML_NODE_TYPE_END_ELEMENT
            && zwscn_util_str_hash(xml_reader_node_name(reader)) == name_hash)) {

        if (xml_reader_node_type(reader) == XML_NODE_TYPE_START_ELEMENT) {
            if (strcmp("value_low", xml_reader_node_name(reader)) == 0) {
                XML_DESERIALIZER_DESERIALIZE(reader, zwsensor_t, &param->value_low);
            }
            else if (strcmp("value_high", xml_reader_node_name(reader)) == 0) {
                XML_DESERIALIZER_DESERIALIZE(reader, zwsensor_t, &param->value_high);
            }
        }
    }
    return ZWSCN_ERR_NONE;
}

ZWSCN_STATUS_T zwsensor_deserializer(xml_reader_t* reader, void* value) {
    zwsensor_t* param = value;
    xml_exception_t error = XML_EXCEPTION_NONE;
    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "sensor")) return ZWSCN_ERR_GENERIC;

    while (!(error = xml_reader_next(reader)) &&xml_reader_node_type(reader) == XML_NODE_TYPE_ATTRIBUTE) {
        if (strcmp("unit", xml_reader_node_name(reader)) == 0) {
            param->unit = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("type", xml_reader_node_name(reader)) == 0) {
            param->type = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("precision", xml_reader_node_name(reader)) == 0) {
            param->precision = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("size", xml_reader_node_name(reader)) == 0) {
            param->size = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("data", xml_reader_node_name(reader)) == 0) {
            int32_t val32 = xml_string_to_int32(xml_reader_node_content(reader));
            if(param->size == 1){
                param->data[0] = (uint8_t)val32;
            }else if(param->size == 2){
                param->data[1] = (uint8_t)val32;
                param->data[0] = (uint8_t)(val32 >> 8);

            }else if(param->size == 4){
                param->data[3] = (uint8_t)val32;
                param->data[2] = (uint8_t)(val32 >> 8);
                param->data[1] = (uint8_t)(val32 >> 16);
                param->data[0] = (uint8_t)(val32 >> 24);
            }
        }
    }
    return ZWSCN_ERR_NONE;
}

ZWSCN_STATUS_T zwscn_args_alarm_deserializer(xml_reader_t* reader, void* value) {
    zwscn_args_alarm_t* param = value;
    xml_exception_t error = XML_EXCEPTION_NONE;
    size_t i = 0;
    uint32_t name_hash1 = zwscn_util_str_hash("za_events");
    uint32_t name_hash2 = zwscn_util_str_hash("za_event");
    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "alarm")) return ZWSCN_ERR_GENERIC;

    while (!(error = xml_reader_next(reader)) &&xml_reader_node_type(reader) == XML_NODE_TYPE_ATTRIBUTE) {
        if (strcmp("va_level", xml_reader_node_name(reader)) == 0) {
            param->va_level = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("va_type", xml_reader_node_name(reader)) == 0) {
            param->va_type = xml_string_to_uint8(xml_reader_node_content(reader));
        }
    }

    i = 0;
    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "za_events")) return ZWSCN_ERR_GENERIC;
    while (!(error = xml_reader_next(reader)) &&!(xml_reader_node_type(reader) == XML_NODE_TYPE_END_ELEMENT
            && zwscn_util_str_hash(xml_reader_node_name(reader)) == name_hash1)) {
        if (xml_reader_node_type(reader) == XML_NODE_TYPE_ATTRIBUTE) {
            if (strcmp("za_type", xml_reader_node_name(reader)) == 0) {
                param->za_type = xml_string_to_uint8(xml_reader_node_content(reader));
            }
        }
        else if (xml_reader_node_type(reader) == XML_NODE_TYPE_START_ELEMENT && zwscn_util_str_hash(xml_reader_node_name(reader)) == name_hash2) {
            while (!(error = xml_reader_next(reader)) &&xml_reader_node_type(reader) == XML_NODE_TYPE_ATTRIBUTE) {
                if (strcmp("value", xml_reader_node_name(reader)) == 0) {
                    param->za_event[i++] = xml_string_to_uint8(xml_reader_node_content(reader));
                }
            }
        }
    }

    return ZWSCN_ERR_NONE;
}

ZWSCN_STATUS_T zwscn_args_basic_event_deserializer(xml_reader_t* reader, void* value) {
    zwscn_args_basic_event_t* param = value;
    xml_exception_t error = XML_EXCEPTION_NONE;

    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "basic_event")) return ZWSCN_ERR_GENERIC;

    while (!(error = xml_reader_next(reader)) &&xml_reader_node_type(reader) == XML_NODE_TYPE_ATTRIBUTE) {
        if (strcmp("value_low", xml_reader_node_name(reader)) == 0) {
            param->value_low = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("value_high", xml_reader_node_name(reader)) == 0) {
            param->value_high = xml_string_to_uint8(xml_reader_node_content(reader));
        }
    }
    return ZWSCN_ERR_NONE;
}

ZWSCN_STATUS_T zwscn_args_central_scene_deserializer(xml_reader_t* reader, void* value) {
    zwscn_args_central_scene_t* param = value;
    xml_exception_t error = XML_EXCEPTION_NONE;

    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "central_scene")) return ZWSCN_ERR_GENERIC;
    while (!(error = xml_reader_next(reader)) &&xml_reader_node_type(reader) == XML_NODE_TYPE_ATTRIBUTE) {
        if (strcmp("central_scene_key_attribute", xml_reader_node_name(reader)) == 0) {
            param->central_scene_key_attribute = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("central_scene_key_number", xml_reader_node_name(reader)) == 0) {
            param->central_scene_key_number = xml_string_to_uint8(xml_reader_node_content(reader));
        }
    }
    return ZWSCN_ERR_NONE;
}

ZWSCN_STATUS_T zwscn_deserializer_device_args(xml_reader_t* reader, uint32_t id, uint32_t cmd, void **device_args, uint32_t *device_args_size) {
    if (device_args) {
        uint16_t cls_id = zwscn_util_combo_id_to_cls_id(id);
        switch (cls_id) {
        case COMMAND_CLASS_BASIC:
            if (cmd == BASIC_SET) {
                *device_args_size = sizeof(zwscn_args_basic_event_t);
                (*device_args) = calloc(1, sizeof(zwscn_args_basic_event_t));
                return XML_DESERIALIZER_DESERIALIZE(reader, zwscn_args_basic_event_t, *device_args);
            }
            else {
                *device_args_size = sizeof(zwscn_args_basic_set_t);
                (*device_args) = calloc(1, sizeof(zwscn_args_basic_set_t));
                return XML_DESERIALIZER_DESERIALIZE(reader, zwscn_args_basic_set_t, *device_args);
            }
            break;
        case COMMAND_CLASS_SWITCH_BINARY:
            *device_args_size = sizeof(zwscn_args_switch_binary_set_t);
            (*device_args) = calloc(1, sizeof(zwscn_args_switch_binary_set_t));
            return XML_DESERIALIZER_DESERIALIZE(reader, zwscn_args_switch_binary_set_t, *device_args);
            break;
        case COMMAND_CLASS_SWITCH_MULTILEVEL:
            *device_args_size = sizeof(zwscn_args_switch_multilevel_set_t);
            (*device_args) = calloc(1, sizeof(zwscn_args_switch_multilevel_set_t));
            return XML_DESERIALIZER_DESERIALIZE(reader, zwscn_args_switch_multilevel_set_t, *device_args);
            break;
        case COMMAND_CLASS_DOOR_LOCK:
            *device_args_size = sizeof(zwscn_args_door_lock_operation_set_t);
            (*device_args) = calloc(1, sizeof(zwscn_args_door_lock_operation_set_t));
            return XML_DESERIALIZER_DESERIALIZE(reader, zwscn_args_door_lock_operation_set_t, *device_args);
            break;
        case COMMAND_CLASS_THERMOSTAT_SETPOINT:
            *device_args_size = sizeof(zwscn_args_thermostat_setpoint_set_t);
            (*device_args) = calloc(1, sizeof(zwscn_args_thermostat_setpoint_set_t));
            return XML_DESERIALIZER_DESERIALIZE(reader, zwscn_args_thermostat_setpoint_set_t, *device_args);
            break;
        case COMMAND_CLASS_SENSOR_BINARY:
            *device_args_size = sizeof(zwscn_args_sensor_binary_t);
            (*device_args) = calloc(1, sizeof(zwscn_args_sensor_binary_t));
            return XML_DESERIALIZER_DESERIALIZE(reader, zwscn_args_sensor_binary_t, *device_args);
            break;
        case COMMAND_CLASS_SENSOR_MULTILEVEL:
            *device_args_size = sizeof(zwscn_args_sensor_multilevel_t);
            (*device_args) = calloc(1, sizeof(zwscn_args_sensor_multilevel_t));
            return XML_DESERIALIZER_DESERIALIZE(reader, zwscn_args_sensor_multilevel_t, *device_args);
            break;
        case COMMAND_CLASS_ALARM:
            *device_args_size = sizeof(zwscn_args_alarm_t);
            (*device_args) = calloc(1, sizeof(zwscn_args_alarm_t));
            return XML_DESERIALIZER_DESERIALIZE(reader, zwscn_args_alarm_t, *device_args);
            break;
        case COMMAND_CLASS_CENTRAL_SCENE:
            *device_args_size = sizeof(zwscn_args_central_scene_t);
            (*device_args) = calloc(1, sizeof(zwscn_args_central_scene_t));
            return XML_DESERIALIZER_DESERIALIZE(reader, zwscn_args_central_scene_t, *device_args);
            break;
        }
    }
    return ZWSCN_ERR_INVALID_ARGS;
}

ZWSCN_STATUS_T zwscn_scene_cont_deserializer(xml_reader_t* reader, void* value) {

    typedef ZWSCN_STATUS_T (*func)(xml_reader_t* reader, zwscn_scene_cont_t* param);

    ZWSCN_STATUS_T scene(xml_reader_t* reader, zwscn_scene_cont_t* param) {
        ZWSCN_STATUS_T result;
        uint32_t scene_global_id;
        zwscn_list_t *scene_list;

        param->scene = calloc(1, sizeof(zwscn_scene_t));
        if (param->scene == NULL) {
            goto l_err_memory;
        }
        result = XML_DESERIALIZER_DESERIALIZE(reader, zwscn_scene_t, param->scene);
        if (result != ZWSCN_ERR_NONE) {
            goto l_err;
        }

        scene_global_id = zwscn_get_scene_id_pool_index(param->net);
        if (param->scene->id > scene_global_id) {
            zwscn_set_scene_id_pool_index(param->net, param->scene->id);
        }
        zwscn_set_statelog_start_op_add(param->net, param->scene->id, ZWSCN_STATELOG_TRIGGER_SYSTEM);
        if (zwscn_find_scene_list(param->net, &scene_list) != ZWSCN_ERR_NONE) {
            goto l_err;
        }
        if (zwscn_is_list_full_scene(param->net, (*scene_list))) {
            goto l_err_list_full;
        }
        if (((*scene_list) = zwscn_list_add((*scene_list), param)) == NULL) {
            goto l_err;
        }
        zwscn_set_statelog_end_op_add(param->net, param->scene->id, ZWSCN_STATELOG_TRIGGER_SYSTEM);

        return ZWSCN_ERR_NONE;

        l_err_memory: return ZWSCN_ERR_MEMORY;
        l_err_list_full: return ZWSCN_ERR_LIST_FULL;
        l_err: free(param->scene);
        param->scene = NULL;
        return ZWSCN_ERR_GENERIC;
    }

    ZWSCN_STATUS_T actions(xml_reader_t* reader, zwscn_scene_cont_t* param) {
        ZWSCN_STATUS_T result;
        xml_exception_t error;
        zwscn_action_t* action, *action_temp;
        zwscn_event_p event_temp;
        uint8_t node_id, ep_id;
        uint16_t cls_id;
        zwscn_init_p init_data;
        uint32_t name1_hash = zwscn_util_str_hash("actions");
        uint32_t name2_hash = zwscn_util_str_hash("action");
        if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "actions")) {
            goto l_err;
        }

        while (!(error = xml_reader_next(reader)) && !(xml_reader_node_type(reader) == XML_NODE_TYPE_END_ELEMENT
                && zwscn_util_str_hash(xml_reader_node_name(reader)) == name1_hash)) {
            if(!(xml_reader_node_type(reader) == XML_NODE_TYPE_START_ELEMENT
                    && zwscn_util_str_hash(xml_reader_node_name(reader)) == name2_hash)){
                continue;
            }
            action = calloc(1, sizeof(zwscn_action_t));
            if (action == NULL) {
                goto l_err_memory;
            }
            result = XML_DESERIALIZER_DESERIALIZE(reader, zwscn_action_t, action);
            if (result != ZWSCN_ERR_NONE) {
                goto l_err;
            }

            if (zwscn_find_action(param->net, param->scene->id, action->id, &action_temp) == ZWSCN_ERR_NONE) { //Action exists with the same id
                goto l_err_duplicate_id;
            }
#if ZWSCN_CHECK_DEVICE_INTF_IN_ACTIONS_AND_EVENSTS
            if (zwscn_find_event(param->net, param->scene->id, action->id, &event_temp) == ZWSCN_ERR_NONE) { //Event exists with the same id
                goto l_err_duplicate_id;
            }
#endif
            node_id = zwscn_util_combo_id_to_node_id(action->id);
            ep_id = zwscn_util_combo_id_to_ep_id(action->id);
            cls_id = zwscn_util_combo_id_to_cls_id(action->id);
            init_data = zwscn_get_init_data(param->net);
            if (init_data && init_data->init_input && init_data->init_input->zwscn_fn_is_valid_handler) {
                result = init_data->init_input->zwscn_fn_is_valid_handler(param->net, ZWSCN_CHECK_NODE_EP_CLS, node_id, ep_id, cls_id);
                zwscn_message_log_debug("[1] zwscn_fn_is_valid_handler returned %d for node=%d ep=%d cls=%d\n", result, node_id, ep_id, cls_id);
                if (!result) {
                    if ((param->list_actions_invalid = zwscn_list_add(param->list_actions_invalid, action)) == NULL) {
                        goto l_err;
                    }
                }
                else {
                    if (zwscn_is_list_full_action(param->net, param->list_actions)) {
                        goto l_err_list_full;
                    }
                    if ((param->list_actions = zwscn_list_add(param->list_actions, action)) == NULL) {
                        goto l_err;
                    }
                }
            }
            else {
                zwscn_message_log_warning("Function zwscn_fn_is_valid_handler is NULL for net=%p\n", param->net);
                goto l_err_callback_null;
            }

            if (zwscn_setup_op_request(param->net, ZWSCN_TYPE_ACTION, action) != ZWSCN_ERR_NONE) {
                goto l_err;
            }
        }

        if(zwscn_list_size(param->list_actions) == 0 && zwscn_list_size(param->list_actions_invalid) == 0){
            goto l_err;
        }

        return ZWSCN_ERR_NONE;

        l_err_memory: return ZWSCN_ERR_MEMORY;
        l_err_list_full: return ZWSCN_ERR_LIST_FULL;
        l_err_duplicate_id: free(action->device_args);
        free(action);
        return ZWSCN_ERR_ITEM_IGNORED;
        l_err: free(action);
        return ZWSCN_ERR_GENERIC;
        l_err_callback_null: free(action->device_args);
        free(action);
        return ZWSCN_ERR_CALLBACK_NULL;
    }

    ZWSCN_STATUS_T events(xml_reader_t* reader, zwscn_scene_cont_t* param) {
        ZWSCN_STATUS_T result;
        xml_exception_t error;
        zwscn_event_t* event;
        zwscn_action_p action_temp;
        zwscn_event_p event_temp;
        uint8_t node_id, ep_id;
        uint16_t cls_id;
        zwscn_init_p init_data;
        uint32_t name1_hash = zwscn_util_str_hash("events");
        uint32_t name2_hash = zwscn_util_str_hash("event");
        if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "events")) {
            goto l_err;
        }
        while (!(error = xml_reader_next(reader)) && !(xml_reader_node_type(reader) == XML_NODE_TYPE_END_ELEMENT
                && zwscn_util_str_hash(xml_reader_node_name(reader)) == name1_hash)) {
            if(!(xml_reader_node_type(reader) == XML_NODE_TYPE_START_ELEMENT
                    && zwscn_util_str_hash(xml_reader_node_name(reader)) == name2_hash)){
                continue;
            }
            event = calloc(1, sizeof(zwscn_event_t));
            if (event == NULL) {
                goto l_err_memory;
            }
            result = XML_DESERIALIZER_DESERIALIZE(reader, zwscn_event_t, event);
            if (result != ZWSCN_ERR_NONE) {
                goto l_err;
            }
            if (zwscn_find_event(param->net, param->scene->id, event->id, &event_temp) == ZWSCN_ERR_NONE) { //Event exists with the same id
                goto l_err_duplicate_id;
            }
#if ZWSCN_CHECK_DEVICE_INTF_IN_ACTIONS_AND_EVENSTS
            if (zwscn_find_action(param->net, param->scene->id, event->id, &action_temp) == ZWSCN_ERR_NONE) { //Action exists with the same id
                goto l_err_duplicate_id;
            }
#endif
            node_id = zwscn_util_combo_id_to_node_id(event->id);
            ep_id = zwscn_util_combo_id_to_ep_id(event->id);
            cls_id = zwscn_util_combo_id_to_cls_id(event->id);
            init_data = zwscn_get_init_data(param->net);
            if (init_data && init_data->init_input && init_data->init_input->zwscn_fn_is_valid_handler) {
                result = init_data->init_input->zwscn_fn_is_valid_handler(param->net, ZWSCN_CHECK_NODE_EP_CLS, node_id, ep_id, cls_id);
                if (!result) {
                    if ((param->list_events_invalid = zwscn_list_add(param->list_events_invalid, event)) == NULL) {
                        goto l_err;
                    }
                }
                else {
                    if (zwscn_is_list_full_event(param->net, param->list_events)) {
                        goto l_err_list_full;
                    }
                    if ((param->list_events = zwscn_list_add(param->list_events, event)) == NULL) {
                        goto l_err;
                    }
                }
            }
            else {
                zwscn_message_log_warning("Function zwscn_fn_is_valid_handler is NULL for net=%p\n", param->net);
                goto l_err_callback_null;
            }
            zwscn_scene_triggers_info_update(param->net, param);
            if (zwscn_setup_op_request(param->net, ZWSCN_TYPE_EVENT, param) != ZWSCN_ERR_NONE) {
                goto l_err;
            }
        }

        return ZWSCN_ERR_NONE;
        l_err_memory: return ZWSCN_ERR_MEMORY;
        l_err_duplicate_id: free(event->device_args);
        free(event);
        return ZWSCN_ERR_ITEM_IGNORED;
        l_err_list_full: free(event->device_args);
        free(event);
        return ZWSCN_ERR_LIST_FULL;
        l_err: free(event->device_args);
        free(event);
        return ZWSCN_ERR_GENERIC;
        l_err_callback_null: free(event->device_args);
        free(event);
        return ZWSCN_ERR_CALLBACK_NULL;
    }

    ZWSCN_STATUS_T schedules(xml_reader_t* reader, zwscn_scene_cont_t* param) {
        ZWSCN_STATUS_T result;
        xml_exception_t error;
        zwscn_schedule_t* schedule;
        uint32_t name1_hash = zwscn_util_str_hash("schedules");
        uint32_t name2_hash = zwscn_util_str_hash("schedule");
        if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "schedules")) {
            goto l_err;
        }
        while (!(error = xml_reader_next(reader)) && !(xml_reader_node_type(reader) == XML_NODE_TYPE_END_ELEMENT
                && zwscn_util_str_hash(xml_reader_node_name(reader)) == name1_hash)) {
            if(!(xml_reader_node_type(reader) == XML_NODE_TYPE_START_ELEMENT
                    && zwscn_util_str_hash(xml_reader_node_name(reader)) == name2_hash)){
                continue;
            }
            schedule = calloc(1, sizeof(zwscn_schedule_t));
            if (schedule == NULL) {
                goto l_err_memory;
            }
            result = XML_DESERIALIZER_DESERIALIZE(reader, zwscn_schedule_t, schedule);
            if (result != ZWSCN_ERR_NONE) {
                goto l_err;
            }
            if (zwscn_schedule_is_data_valid(schedule) == ZWSCN_FALSE) {
                zwscn_message_log_warning("Schedule value for day bitmask(0x%02x) or hour(%d) or minute(%d) is invalid\n", schedule->day, schedule->hour,
                        schedule->minute);
                goto l_err;
            }
            if ((schedule->id = zwscn_generate_scene_schedule_id(param->net, param->scene->id)) == 0) {
                goto l_err;
            }
            if (zwscn_is_list_full_schedule(param->net, param->list_schedules)) {
                goto l_err_list_full;
            }
            if ((param->list_schedules = zwscn_list_add(param->list_schedules, schedule)) == NULL) {
                goto l_err;
            }
            zwscn_scene_triggers_info_update(param->net, param);
            if (zwscn_schedule_handle_update(param->net, param, schedule->id, ZWSCN_TRUE) != ZWSCN_ERR_NONE) { //ZWSCN_TRUE is for checking the schedule from today
                goto l_err_schedule;
            }
        }

        return ZWSCN_ERR_NONE;
        l_err_memory: return ZWSCN_ERR_MEMORY;
        l_err_list_full: free(schedule);
        return ZWSCN_ERR_LIST_FULL;
        l_err: free(schedule);
        return ZWSCN_ERR_GENERIC;
        l_err_schedule: return ZWSCN_ERR_GENERIC;
    }

    zwscn_scene_cont_t* param = value;
    xml_exception_t error = XML_EXCEPTION_NONE;
    ZWSCN_STATUS_T result;
    zwscn_hash_t map = zwscn_hash_new_str_hash();
    zwscn_hash_insert(map, "scene", scene);
    zwscn_hash_insert(map, "actions", actions);
    zwscn_hash_insert(map, "events", events);
    zwscn_hash_insert(map, "schedules", schedules);

    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "zwave")) return ZWSCN_ERR_GENERIC;

    while (!error) {
        if (xml_reader_node_type(reader) != XML_NODE_TYPE_START_ELEMENT) {
            error = xml_reader_next(reader);
            continue;
        }
        func f = zwscn_hash_get_value(map, xml_reader_node_name(reader));
        if (f) {
            result = f(reader, param);
            if (result != ZWSCN_ERR_NONE) {
                zwscn_free_scene_lists(param);
                break;
            }
        }
        else {
            error = xml_reader_next(reader);
        }
    }
    zwscn_hash_destroy(map);
    return result;

}

ZWSCN_STATUS_T zwscn_scene_deserializer(xml_reader_t* reader, void* value) {
    xml_exception_t error = XML_EXCEPTION_NONE;
    char* str = NULL;
    zwscn_scene_t* param = value;

    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "scene")) return ZWSCN_ERR_GENERIC;

    while (!(error = xml_reader_next(reader)) &&xml_reader_node_type(reader) == XML_NODE_TYPE_ATTRIBUTE) {
        if (strcmp("id", xml_reader_node_name(reader)) == 0) {
            param->id = xml_string_to_uint32(xml_reader_node_content(reader));
        }
        else if (strcmp("name", xml_reader_node_name(reader)) == 0) {
            str = zwscn_util_escape_uri(xml_reader_node_content(reader));
            strcpy(param->name, str);
            free(str);
        }
        else if (strcmp("active", xml_reader_node_name(reader)) == 0) {
            param->active = xml_string_to_uint8(xml_reader_node_content(reader));
        }
    }

    return ZWSCN_ERR_NONE;
}

ZWSCN_STATUS_T zwscn_action_deserializer(xml_reader_t* reader, void* value) {
    zwscn_action_t* param = value;
    xml_exception_t error = XML_EXCEPTION_NONE;
    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "action")) return ZWSCN_ERR_GENERIC;

    while (!(error = xml_reader_next(reader)) &&xml_reader_node_type(reader) == XML_NODE_TYPE_ATTRIBUTE) {
        if (strcmp("id", xml_reader_node_name(reader)) == 0) {
            param->id = xml_string_to_uint32(xml_reader_node_content(reader));
        }
        else if (strcmp("type", xml_reader_node_name(reader)) == 0) {
            param->type = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("cmd", xml_reader_node_name(reader)) == 0) {
            param->cmd = xml_string_to_uint8(xml_reader_node_content(reader));
        }
    }
    if (zwscn_deserializer_device_args(reader, param->id, param->cmd, &param->device_args, &param->device_args_size)) return ZWSCN_ERR_GENERIC;
    return ZWSCN_ERR_NONE;
}

ZWSCN_STATUS_T zwscn_event_deserializer(xml_reader_t* reader, void* value) {
    zwscn_event_t* param = value;
    xml_exception_t error = XML_EXCEPTION_NONE;

    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "event")) return ZWSCN_ERR_GENERIC;

    while (!(error = xml_reader_next(reader)) &&xml_reader_node_type(reader) == XML_NODE_TYPE_ATTRIBUTE) {
        if (strcmp("id", xml_reader_node_name(reader)) == 0) {
            param->id = xml_string_to_uint32(xml_reader_node_content(reader));
        }
        else if (strcmp("type", xml_reader_node_name(reader)) == 0) {
            param->type = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("cmd", xml_reader_node_name(reader)) == 0) {
            param->cmd = xml_string_to_uint8(xml_reader_node_content(reader));
        }
    }
    if (zwscn_deserializer_device_args(reader, param->id, param->cmd, &param->device_args, &param->device_args_size)) return ZWSCN_ERR_GENERIC;
    return ZWSCN_ERR_NONE;

}

ZWSCN_STATUS_T zwscn_schedule_deserializer(xml_reader_t* reader, void* value) {
    zwscn_schedule_t* param = value;
    xml_exception_t error = XML_EXCEPTION_NONE;

    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "schedule")) return ZWSCN_ERR_GENERIC;

    while (!(error = xml_reader_next(reader)) &&xml_reader_node_type(reader) == XML_NODE_TYPE_ATTRIBUTE) {
        if (strcmp("id", xml_reader_node_name(reader)) == 0) {
            param->id = xml_string_to_uint32(xml_reader_node_content(reader));
        }
        else if (strcmp("type", xml_reader_node_name(reader)) == 0) {
            param->type = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("day", xml_reader_node_name(reader)) == 0) {
            param->day = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("hour", xml_reader_node_name(reader)) == 0) {
            param->hour = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("minute", xml_reader_node_name(reader)) == 0) {
            param->minute = xml_string_to_uint8(xml_reader_node_content(reader));
        }
    }
    return ZWSCN_ERR_NONE;
}

ZWSCN_STATUS_T zwscn_security_scene_cont_deserializer(xml_reader_t* reader, void* value) {

    typedef ZWSCN_STATUS_T (*func)(xml_reader_t* reader, zwscn_security_scene_cont_t* param);
    ZWSCN_STATUS_T lt_arm(xml_reader_t* reader, zwscn_security_scene_cont_t* param) {
        ZWSCN_STATUS_T result;
        result = XML_DESERIALIZER_DESERIALIZE(reader, zwscn_security_scene_last_trigger_t, &param->scene->lt_arm);
        if (result != ZWSCN_ERR_NONE) {
            goto l_err;
        }
        return ZWSCN_ERR_NONE;
        l_err: zwscn_free_security_scene_struct(param->scene);
        param->scene = NULL;
        return ZWSCN_ERR_GENERIC;
    }
    ZWSCN_STATUS_T lt_disarm(xml_reader_t* reader, zwscn_security_scene_cont_t* param) {
        ZWSCN_STATUS_T result;
        result = XML_DESERIALIZER_DESERIALIZE(reader, zwscn_security_scene_last_trigger_t, &param->scene->lt_disarm);
        if (result != ZWSCN_ERR_NONE) {
            goto l_err;
        }
        return ZWSCN_ERR_NONE;
        l_err: zwscn_free_security_scene_struct(param->scene);
        param->scene = NULL;
        return ZWSCN_ERR_GENERIC;
    }
    ZWSCN_STATUS_T lt_alarm_on(xml_reader_t* reader, zwscn_security_scene_cont_t* param) {
        ZWSCN_STATUS_T result;
        result = XML_DESERIALIZER_DESERIALIZE(reader, zwscn_security_scene_last_trigger_t, &param->scene->lt_alarm_on);
        if (result != ZWSCN_ERR_NONE) {
            goto l_err;
        }
        return ZWSCN_ERR_NONE;
        l_err: zwscn_free_security_scene_struct(param->scene);
        param->scene = NULL;
        return ZWSCN_ERR_GENERIC;
    }
    ZWSCN_STATUS_T lt_alarm_off(xml_reader_t* reader, zwscn_security_scene_cont_t* param) {
        ZWSCN_STATUS_T result;
        result = XML_DESERIALIZER_DESERIALIZE(reader, zwscn_security_scene_last_trigger_t, &param->scene->lt_alarm_off);
        if (result != ZWSCN_ERR_NONE) {
            goto l_err;
        }
        return ZWSCN_ERR_NONE;
        l_err: zwscn_free_security_scene_struct(param->scene);
        param->scene = NULL;
        return ZWSCN_ERR_GENERIC;
    }

    ZWSCN_STATUS_T security_scene(xml_reader_t* reader, zwscn_security_scene_cont_t* param) {
        ZWSCN_STATUS_T result;
        uint32_t scene_global_id;
        zwscn_list_t *scene_list;

        param->scene = calloc(1, sizeof(zwscn_security_scene_t));
        if (param->scene == NULL) {
            goto l_err_memory;
        }
        result = XML_DESERIALIZER_DESERIALIZE(reader, zwscn_security_scene_t, param->scene);
        if (result != ZWSCN_ERR_NONE) {
            goto l_err;
        }

        if ((param->scene->is_notification_by_sms_on) && (zwscn_util_sms_number_is_valid(param->scene->notification_sms_number, ZWSCN_TRUE) != ZWSCN_TRUE)) {
            ZWSCN_MESSAGE_LOG_DEBUG("SMS NUMBER: %s is invalid\n", param->scene->notification_sms_number);
            goto l_err_invalid_data;
        }

        if ((param->scene->is_notification_by_email_on) && (zwscn_util_email_is_valid(param->scene->notification_email, ZWSCN_TRUE) != ZWSCN_TRUE)) {
            ZWSCN_MESSAGE_LOG_DEBUG("Email: %s is invalid\n", param->scene->notification_email);
            goto l_err_invalid_data;
        }

        scene_global_id = zwscn_get_security_scene_id_pool_index(param->net);
        if (param->scene->id > scene_global_id) {
            zwscn_set_security_scene_id_pool_index(param->net, param->scene->id);
        }

        if (zwscn_find_security_scene_list(param->net, &scene_list) != ZWSCN_ERR_NONE) {
            goto l_err;
        }
        if (zwscn_is_list_full_security_scene(param->net, (*scene_list))) {
            goto l_err_list_full;
        }
        if (((*scene_list) = zwscn_list_add((*scene_list), param)) == NULL) {
            goto l_err;
        }
        zwscn_security_scene_update_timestamp(param);
        zwscn_statelog_security_scene_set(param->net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_ADD, param->scene->id);

        return ZWSCN_ERR_NONE;

        l_err_memory: return ZWSCN_ERR_MEMORY;
        l_err_list_full: return ZWSCN_ERR_LIST_FULL;
        l_err: zwscn_free_security_scene_struct(param->scene);
        param->scene = NULL;
        return ZWSCN_ERR_GENERIC;
        l_err_invalid_data: zwscn_free_security_scene_struct(param->scene);
        param->scene = NULL;
        return ZWSCN_ERR_INVALID_DATA;
    }

    ZWSCN_STATUS_T list_arm_events(xml_reader_t* reader, zwscn_security_scene_cont_t* param) {
        ZWSCN_STATUS_T result;
        xml_exception_t error;
        uint32_t name1_hash = zwscn_util_str_hash("list_arm_events");
        uint32_t name2_hash = zwscn_util_str_hash("event");
        zwscn_list_t event_list;
        zwscn_security_event_p event_ptr;
        zwscn_security_event_p event_ptr_temp;
        uint8_t node_id, ep_id;
        uint16_t cls_id;
        zwscn_init_p init_data;
        uint32_t scene_id;

        if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "list_arm_events")) {
            goto l_err;
        }
        while (!(error = xml_reader_next(reader)) && !(xml_reader_node_type(reader) == XML_NODE_TYPE_END_ELEMENT
                && zwscn_util_str_hash(xml_reader_node_name(reader)) == name1_hash)) {
            if(!(xml_reader_node_type(reader) == XML_NODE_TYPE_START_ELEMENT
                    && zwscn_util_str_hash(xml_reader_node_name(reader)) == name2_hash)){
                continue;
            }
            event_ptr = (zwscn_security_event_p) calloc(1, sizeof(zwscn_security_event_t));
            if (event_ptr == NULL) {
                goto l_err_memory;
            }
            result = XML_DESERIALIZER_DESERIALIZE(reader, zwscn_security_event_t, event_ptr);
            if (result != ZWSCN_ERR_NONE) {
                goto l_err;
            }

            cls_id = zwscn_util_combo_id_to_cls_id(event_ptr->id);
            if (zwscn_zwave_is_cc_for_arm_security_event(cls_id, event_ptr->cmd) != ZWSCN_TRUE) {
                zwscn_message_log_warning("Z-Wave Command Class 0x%x is not valid for Arm events\n", cls_id);
                goto l_err_device_invalid;
            }
            scene_id = param->scene->id;
#if ZWSCN_CHECK_DUPLICATE_IN_SAME_SECURITY_EVENSTS
            if (zwscn_find_arm_security_event(param->net, scene_id, event_ptr->id, &event_ptr_temp) == ZWSCN_ERR_NONE) { //Event exists with the same id
                goto l_err_duplicate_id;
            }
#endif
#if ZWSCN_CHECK_DUPLICATE_IN_OTHER_SECURITY_EVENSTS
            if (zwscn_security_scene_has_same_disarm_event(param, event_ptr) == ZWSCN_TRUE) {
                goto l_err_duplicate_id;
            }
            if (zwscn_security_scene_has_same_alarm_event(param, event_ptr) == ZWSCN_TRUE) {
                goto l_err_duplicate_id;
            }
#endif
            node_id = zwscn_util_combo_id_to_node_id(event_ptr->id);
            ep_id = zwscn_util_combo_id_to_ep_id(event_ptr->id);
            cls_id = zwscn_util_combo_id_to_cls_id(event_ptr->id);
            init_data = zwscn_get_init_data(param->net);
            if (init_data && init_data->init_input && init_data->init_input->zwscn_fn_is_valid_handler) {
                result = init_data->init_input->zwscn_fn_is_valid_handler(param->net, ZWSCN_CHECK_NODE_EP_CLS, node_id, ep_id, cls_id);
                if (!result) {
                    event_list = param->list_arm_events_invalid;
                    if ((param->list_arm_events_invalid = zwscn_list_add(event_list, event_ptr)) == NULL) {
                        goto l_err;
                    }
                }
                else {
                    event_list = param->list_arm_events;
                    if (zwscn_is_list_full_arm_security_event(param->net, event_list)) {
                        goto l_err_list_full;
                    }
                    if ((param->list_arm_events = zwscn_list_add(event_list, event_ptr)) == NULL) {
                        goto l_err;
                    }
                }
            }
            else {
                zwscn_message_log_warning("Function zwscn_fn_is_valid_handler is NULL for net=%p\n", param->net);
                goto l_err_callback_null;
            }
            if (zwscn_setup_op_request(param->net, ZWSCN_TYPE_EVENT, event_ptr) != ZWSCN_ERR_NONE) {
                goto l_err;
            }
            zwscn_security_scene_update_timestamp(param);
            zwscn_statelog_security_scene_set(param->net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, param->scene->id);
        }

        return ZWSCN_ERR_NONE;
        l_err_memory: return ZWSCN_ERR_MEMORY;
        l_err_duplicate_id: free(event_ptr->device_args);
        free(event_ptr);
        return ZWSCN_ERR_ITEM_IGNORED;
        l_err_list_full: free(event_ptr->device_args);
        free(event_ptr);
        return ZWSCN_ERR_LIST_FULL;
        l_err: free(event_ptr->device_args);
        free(event_ptr);
        return ZWSCN_ERR_GENERIC;
        l_err_callback_null: free(event_ptr->device_args);
        free(event_ptr);
        return ZWSCN_ERR_CALLBACK_NULL;
        l_err_device_invalid: free(event_ptr->device_args);
        free(event_ptr);
        return ZWSCN_ERR_INVALID_DEVICE;
    }

    ZWSCN_STATUS_T list_disarm_events(xml_reader_t* reader, zwscn_security_scene_cont_t* param) {
        xml_exception_t error;
        zwscn_list_t event_list;
        zwscn_security_event_p event_ptr;
        zwscn_security_event_p event_ptr_temp;
        int32_t result;
        uint8_t node_id, ep_id;
        uint16_t cls_id;
        zwscn_init_p init_data;
        uint32_t scene_id;
        uint32_t name1_hash = zwscn_util_str_hash("list_disarm_events");
        uint32_t name2_hash = zwscn_util_str_hash("event");

        if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "list_disarm_events")) {
            goto l_err;
        }
        while (!(error = xml_reader_next(reader)) &&!(xml_reader_node_type(reader) == XML_NODE_TYPE_END_ELEMENT
                && zwscn_util_str_hash(xml_reader_node_name(reader)) == name1_hash)) {

            if(!(xml_reader_node_type(reader) == XML_NODE_TYPE_START_ELEMENT
                    && zwscn_util_str_hash(xml_reader_node_name(reader)) == name2_hash)){
                continue;
            }

            event_ptr = (zwscn_security_event_p) calloc(1, sizeof(zwscn_security_event_t));
            if (event_ptr == NULL) {
                goto l_err_memory;
            }
            result = XML_DESERIALIZER_DESERIALIZE(reader, zwscn_security_event_t, event_ptr);
            if (result != ZWSCN_ERR_NONE) {
                goto l_err;
            }
            cls_id = zwscn_util_combo_id_to_cls_id(event_ptr->id);
            if (zwscn_zwave_is_cc_for_arm_security_event(cls_id, event_ptr->cmd) != ZWSCN_TRUE) { //Same function works for Arm and Disarm events
                zwscn_message_log_warning("Z-Wave Command Class 0x%x is not valid for Disarm events\n", cls_id);
                goto l_err_device_invalid;
            }
            scene_id = param->scene->id;
#if ZWSCN_CHECK_DUPLICATE_IN_SAME_SECURITY_EVENSTS
            if (zwscn_find_disarm_security_event(param->net, scene_id, event_ptr->id, &event_ptr_temp) == ZWSCN_ERR_NONE) { //Event exists with the same id
                goto l_err_duplicate_id;
            }
#endif
#if ZWSCN_CHECK_DUPLICATE_IN_OTHER_SECURITY_EVENSTS
            if (zwscn_security_scene_has_same_arm_event(param, event_ptr) == ZWSCN_TRUE) {
                goto l_err_duplicate_id;
            }
            if (zwscn_security_scene_has_same_alarm_event(param, event_ptr) == ZWSCN_TRUE) {
                goto l_err_duplicate_id;
            }
#endif
            node_id = zwscn_util_combo_id_to_node_id(event_ptr->id);
            ep_id = zwscn_util_combo_id_to_ep_id(event_ptr->id);
            cls_id = zwscn_util_combo_id_to_cls_id(event_ptr->id);
            init_data = zwscn_get_init_data(param->net);
            if (init_data && init_data->init_input && init_data->init_input->zwscn_fn_is_valid_handler) {
                result = init_data->init_input->zwscn_fn_is_valid_handler(param->net, ZWSCN_CHECK_NODE_EP_CLS, node_id, ep_id, cls_id);
                if (!result) {
                    //goto l_err_device_invalid;
                    event_list = param->list_disarm_events_invalid;
                    if ((param->list_disarm_events_invalid = zwscn_list_add(event_list, event_ptr)) == NULL) {
                        goto l_err;
                    }
                }
                else {
                    event_list = param->list_disarm_events;
                    if (zwscn_is_list_full_disarm_security_event(param->net, event_list)) {
                        goto l_err_list_full;
                    }
                    if ((param->list_disarm_events = zwscn_list_add(event_list, event_ptr)) == NULL) {
                        goto l_err;
                    }
                }
            }
            else {
                zwscn_message_log_warning("Function zwscn_fn_is_valid_handler is NULL for net=%p\n", param->net);
                goto l_err_callback_null;
            }
            if (zwscn_setup_op_request(param->net, ZWSCN_TYPE_EVENT, event_ptr) != ZWSCN_ERR_NONE) {
                goto l_err;
            }
            zwscn_security_scene_update_timestamp(param);
            zwscn_statelog_security_scene_set(param->net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, param->scene->id);
        }
        return ZWSCN_ERR_NONE;
        l_err_memory: return ZWSCN_ERR_MEMORY;
        l_err_duplicate_id: free(event_ptr->device_args);
        free(event_ptr);
        return ZWSCN_ERR_ITEM_IGNORED;
        l_err_list_full: free(event_ptr->device_args);
        free(event_ptr);
        return ZWSCN_ERR_LIST_FULL;
        l_err: free(event_ptr->device_args);
        free(event_ptr);
        return ZWSCN_ERR_GENERIC;
        l_err_device_invalid: free(event_ptr->device_args);
        free(event_ptr);
        return ZWSCN_ERR_INVALID_DEVICE;
        l_err_callback_null: free(event_ptr->device_args);
        free(event_ptr);
        return ZWSCN_ERR_CALLBACK_NULL;
    }

    ZWSCN_STATUS_T list_alarm_events(xml_reader_t* reader, zwscn_security_scene_cont_t* param) {
        xml_exception_t error;
        zwscn_list_t event_list;
        zwscn_security_event_p event_ptr;
        int32_t result;
        uint8_t node_id, ep_id;
        uint16_t cls_id;
        zwscn_init_p init_data;
        uint32_t name2_hash = zwscn_util_str_hash("event");
        uint32_t name1_hash = zwscn_util_str_hash("list_alarm_events");

        if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "list_alarm_events")) {
            goto l_err;
        }
        while (!(error = xml_reader_next(reader)) &&!(xml_reader_node_type(reader) == XML_NODE_TYPE_END_ELEMENT
                && zwscn_util_str_hash(xml_reader_node_name(reader)) == name1_hash)) {

            if(!(xml_reader_node_type(reader) == XML_NODE_TYPE_START_ELEMENT
                    && zwscn_util_str_hash(xml_reader_node_name(reader)) == name2_hash)){
                continue;
            }

            event_ptr = (zwscn_security_event_p) calloc(1, sizeof(zwscn_security_event_t));
            if (event_ptr == NULL) {
                goto l_err_memory;
            }
            result = XML_DESERIALIZER_DESERIALIZE(reader, zwscn_security_event_t, event_ptr);
            if (result != ZWSCN_ERR_NONE) {
                goto l_err;
            }
            cls_id = zwscn_util_combo_id_to_cls_id(event_ptr->id);
            if (zwscn_zwave_is_cc_for_alarm_security_event(cls_id, event_ptr->cmd) != ZWSCN_TRUE) {
                zwscn_message_log_warning("Z-Wave Command Class 0x%x is not valid for Alarm events\n", cls_id);
                goto l_err_device_invalid;
            }
#if ZWSCN_CHECK_DUPLICATE_IN_SAME_SECURITY_EVENSTS
            if (zwscn_security_scene_has_same_alarm_event(param, event_ptr) == ZWSCN_TRUE) {
                goto l_err_duplicate_id;
            }
#endif
#if ZWSCN_CHECK_DUPLICATE_IN_OTHER_SECURITY_EVENSTS
            if (zwscn_security_scene_has_same_arm_event(param, event_ptr) == ZWSCN_TRUE) {
                goto l_err_duplicate_id;
            }
            if (zwscn_security_scene_has_same_disarm_event(param, event_ptr) == ZWSCN_TRUE) {
                goto l_err_duplicate_id;
            }
#endif
            node_id = zwscn_util_combo_id_to_node_id(event_ptr->id);
            ep_id = zwscn_util_combo_id_to_ep_id(event_ptr->id);
            cls_id = zwscn_util_combo_id_to_cls_id(event_ptr->id);
            init_data = zwscn_get_init_data(param->net);
            if (init_data && init_data->init_input && init_data->init_input->zwscn_fn_is_valid_handler) {
                result = init_data->init_input->zwscn_fn_is_valid_handler(param->net, ZWSCN_CHECK_NODE_EP_CLS, node_id, ep_id, cls_id);
                if (!result) {
                    event_list = param->list_alarm_events_invalid;
                    if ((param->list_alarm_events_invalid = zwscn_list_add(event_list, event_ptr)) == NULL) {
                        goto l_err;
                    }
                }
                else {
                    event_list = param->list_alarm_events;
                    if (zwscn_is_list_full_alarm_security_event(param->net, event_list)) {
                        goto l_err_list_full;
                    }
                    if ((param->list_alarm_events = zwscn_list_add(event_list, event_ptr)) == NULL) {
                        goto l_err;
                    }
                }
            }
            else {
                zwscn_message_log_warning("Function zwscn_fn_is_valid_handler is NULL for net=%p\n", param->net);
                goto l_err_callback_null;
            }
            if (zwscn_setup_op_request(param->net, ZWSCN_TYPE_EVENT, event_ptr) != ZWSCN_ERR_NONE) {
                goto l_err;
            }
            zwscn_security_scene_update_timestamp(param);
            zwscn_statelog_security_scene_set(param->net, ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT, param->scene->id);
        }
	if(zwscn_list_size(param->list_alarm_events) == 0 && zwscn_list_size(param->list_alarm_events_invalid) == 0){
	    goto l_err;
	}
        return ZWSCN_ERR_NONE;
        l_err_memory: return ZWSCN_ERR_MEMORY;
        l_err_duplicate_id: free(event_ptr->device_args);
        free(event_ptr);
        return ZWSCN_ERR_ITEM_IGNORED;
        l_err_list_full: free(event_ptr->device_args);
        free(event_ptr);
        return ZWSCN_ERR_LIST_FULL;
        l_err: free(event_ptr->device_args);
        free(event_ptr);
        return ZWSCN_ERR_GENERIC;
        l_err_device_invalid: free(event_ptr->device_args);
        free(event_ptr);
        return ZWSCN_ERR_INVALID_DEVICE;
        l_err_callback_null: free(event_ptr->device_args);
        free(event_ptr);
        return ZWSCN_ERR_CALLBACK_NULL;
    }

    zwscn_security_scene_cont_t* param = value;
    xml_exception_t error = XML_EXCEPTION_NONE;
    ZWSCN_STATUS_T result;
    zwscn_hash_t map = zwscn_hash_new_str_hash();
    zwscn_hash_insert(map, "security_scene", security_scene);
    zwscn_hash_insert(map, "lt_arm", lt_arm);
    zwscn_hash_insert(map, "lt_disarm", lt_disarm);
    zwscn_hash_insert(map, "lt_alarm_on", lt_alarm_on);
    zwscn_hash_insert(map, "lt_alarm_off", lt_alarm_off);

    zwscn_hash_insert(map, "list_arm_events", list_arm_events);
    zwscn_hash_insert(map, "list_disarm_events", list_disarm_events);
    zwscn_hash_insert(map, "list_alarm_events", list_alarm_events);

    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "zwave")) return ZWSCN_ERR_GENERIC;

    while (!error) {
        if (xml_reader_node_type(reader) != XML_NODE_TYPE_START_ELEMENT) {
            error = xml_reader_next(reader);
            continue;
        }
        func f = zwscn_hash_get_value(map, xml_reader_node_name(reader));
        if (f) {
            result = f(reader, param);
            if (result != ZWSCN_ERR_NONE) {
                zwscn_free_scene_lists(param);
                break;
            }
        }
        else {
            error = xml_reader_next(reader);
        }
    }
    zwscn_hash_destroy(map);
    return result;
}

ZWSCN_STATUS_T zwscn_security_scene_deserializer(xml_reader_t* reader, void* value) {
    ZWSCN_STATUS_T result = ZWSCN_ERR_NONE;
    zwscn_security_scene_t* param = value;
    xml_exception_t error = XML_EXCEPTION_NONE;
    char* str = NULL;
    //uint32_t name_hash = zwscn_util_str_hash("security_scene");

    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "security_scene")) return ZWSCN_ERR_GENERIC;

    while (!(error = xml_reader_next(reader)) &&xml_reader_node_type(reader) == XML_NODE_TYPE_ATTRIBUTE) {
        if (strcmp("id", xml_reader_node_name(reader)) == 0) {
            param->id = xml_string_to_uint32(xml_reader_node_content(reader));
        }
        else if (strcmp("name", xml_reader_node_name(reader)) == 0) {
            str = zwscn_util_escape_uri(xml_reader_node_content(reader));
            strcpy(param->name, str);
            free(str);
        }
        else if (strcmp("active", xml_reader_node_name(reader)) == 0) {
            param->active = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("is_armed", xml_reader_node_name(reader)) == 0) {
            param->is_armed = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("is_alarmed", xml_reader_node_name(reader)) == 0) {
            param->is_alarmed = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("scene_id_at_arm", xml_reader_node_name(reader)) == 0) {
            param->scene_id_at_arm = xml_string_to_uint32(xml_reader_node_content(reader));
        }
        else if (strcmp("scene_id_at_disarm", xml_reader_node_name(reader)) == 0) {
            param->scene_id_at_disarm = xml_string_to_uint32(xml_reader_node_content(reader));
        }
        else if (strcmp("scene_id_at_alarm", xml_reader_node_name(reader)) == 0) {
            param->scene_id_at_alarm = xml_string_to_uint32(xml_reader_node_content(reader));
        }
        else if (strcmp("is_notification_on", xml_reader_node_name(reader)) == 0) {
            param->is_notification_on = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("is_notification_by_sms_on", xml_reader_node_name(reader)) == 0) {
            param->is_notification_by_sms_on = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("is_notification_by_email_on", xml_reader_node_name(reader)) == 0) {
            param->is_notification_by_email_on = xml_string_to_uint8(xml_reader_node_content(reader));
        }
        else if (strcmp("notification_sms_number", xml_reader_node_name(reader)) == 0) {
            str = zwscn_util_escape_uri(xml_reader_node_content(reader));
            strcpy(param->notification_sms_number, str);
            free(str);
        }
        else if (strcmp("notification_email", xml_reader_node_name(reader)) == 0) {
            str = zwscn_util_escape_uri(xml_reader_node_content(reader));
            strcpy(param->notification_email, str);
            free(str);
        }
        else if (strcmp("utime", xml_reader_node_name(reader)) == 0) {
            param->utime = xml_string_to_uint64(xml_reader_node_content(reader));
        }
    }
    return result;
}

ZWSCN_STATUS_T zwscn_security_scene_last_trigger_deserializer(xml_reader_t* reader, void* value) {
    ZWSCN_STATUS_T result;
    zwscn_security_scene_last_trigger_t* param = value;
    xml_exception_t error = XML_EXCEPTION_NONE;
    uint32_t name1_hash = zwscn_util_str_hash("last_trigger");
    uint32_t name2_hash = zwscn_util_str_hash("event");

    if (!xml_reader_skip_till(reader, XML_NODE_TYPE_START_ELEMENT, "last_trigger")) return ZWSCN_ERR_GENERIC;

    while(!(error = xml_reader_next(reader)) && !(xml_reader_node_type(reader) == XML_NODE_TYPE_END_ELEMENT && zwscn_util_str_hash(xml_reader_node_name(reader)) == name1_hash)){
        if(xml_reader_node_type(reader) == XML_NODE_TYPE_ATTRIBUTE){
            if (strcmp("type", xml_reader_node_name(reader)) == 0) {
                param->type = xml_string_to_uint32(xml_reader_node_content(reader));
            }
            else if (strcmp("time", xml_reader_node_name(reader)) == 0) {
                param->time = xml_string_to_uint8(xml_reader_node_content(reader));
            }
        }
        else if(xml_reader_node_type(reader) == XML_NODE_TYPE_START_ELEMENT && zwscn_util_str_hash(xml_reader_node_name(reader)) == name2_hash){
            param->event = calloc(1, sizeof(zwscn_event_t));
            if (param->event == NULL) {
                return ZWSCN_ERR_MEMORY;
            }
            result = XML_DESERIALIZER_DESERIALIZE(reader, zwscn_security_event_t, param->event);
            if (result != ZWSCN_ERR_NONE) {
                return result;
            }
        }
    }

    return ZWSCN_ERR_NONE;
}

void zwscn_deserializer_functions_register_all_types() {
    XML_DESERIALIZER_REGISTER(zwscn_args_basic_set_t, zwscn_args_basic_set_deserializer);
    XML_DESERIALIZER_REGISTER(zwscn_args_switch_binary_set_t, zwscn_args_switch_binary_set_deserializer);
    XML_DESERIALIZER_REGISTER(zwscn_args_switch_multilevel_set_t, zwscn_args_switch_multilevel_set_deserializer);
    XML_DESERIALIZER_REGISTER(zwscn_args_door_lock_operation_set_t, zwscn_args_door_lock_operation_set_deserializer);
    XML_DESERIALIZER_REGISTER(zwscn_args_thermostat_setpoint_set_t, zwscn_args_thermostat_setpoint_set_deserializer);
    XML_DESERIALIZER_REGISTER(zwsetp_t, zwsetp_deserializer);
    XML_DESERIALIZER_REGISTER(zwscn_args_sensor_binary_t, zwscn_args_sensor_binary_deserializer);
    XML_DESERIALIZER_REGISTER(zwscn_args_sensor_multilevel_t, zwscn_args_sensor_multilevel_deserializer);
    XML_DESERIALIZER_REGISTER(zwsensor_t, zwsensor_deserializer);
    XML_DESERIALIZER_REGISTER(zwscn_args_alarm_t, zwscn_args_alarm_deserializer);
    XML_DESERIALIZER_REGISTER(zwscn_args_basic_event_t, zwscn_args_basic_event_deserializer);
    XML_DESERIALIZER_REGISTER(zwscn_args_central_scene_t, zwscn_args_central_scene_deserializer);

    XML_DESERIALIZER_REGISTER(zwscn_scene_cont_t, zwscn_scene_cont_deserializer);
    XML_DESERIALIZER_REGISTER(zwscn_scene_t, zwscn_scene_deserializer);
    XML_DESERIALIZER_REGISTER(zwscn_action_t, zwscn_action_deserializer);
    XML_DESERIALIZER_REGISTER(zwscn_event_t, zwscn_event_deserializer);
    XML_DESERIALIZER_REGISTER(zwscn_schedule_t, zwscn_schedule_deserializer);

    XML_DESERIALIZER_REGISTER(zwscn_security_scene_cont_t, zwscn_security_scene_cont_deserializer);
    XML_DESERIALIZER_REGISTER(zwscn_security_scene_t, zwscn_security_scene_deserializer);
    XML_DESERIALIZER_REGISTER(zwscn_security_event_t, zwscn_event_deserializer);
    XML_DESERIALIZER_REGISTER(zwscn_security_scene_last_trigger_t, zwscn_security_scene_last_trigger_deserializer);

}

typedef ZWSCN_STATUS_T (*XMLDeserializerFunc)(xml_reader_t* reader, void* value);

typedef struct xml_deserializer {
    zwscn_hash_t function_map;
} xml_deserializer_t;

static xml_deserializer_t* xml_deserializer_instance = 0;

void xml_deserializer_init() {
    if (!xml_deserializer_instance) {
        static xml_deserializer_t sz;
        sz.function_map = zwscn_hash_new_str_hash();
        xml_deserializer_instance = &sz;
    }
}

void xml_deserializer_register(char* struct_type, ZWSCN_STATUS_T (*func)(xml_reader_t* reader, void* value)) {
    xml_deserializer_init();
    zwscn_hash_insert(xml_deserializer_instance->function_map, struct_type, func);
}

ZWSCN_STATUS_T xml_deserializer_deserialize(xml_reader_t* reader, char* struct_type, void* value) {
    XMLDeserializerFunc func = zwscn_hash_get_value(xml_deserializer_instance->function_map, struct_type);
    if (func) {
        return func(reader, value);
    }
    return ZWSCN_ERR_INVALID_ARGS;
}
