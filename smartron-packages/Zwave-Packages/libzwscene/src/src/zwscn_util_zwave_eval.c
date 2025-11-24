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
@file       zwscn_util_zwave_eval.c

                Implementation of Device Argument Evaluator

@author     Mafahir

@version    1.0 2015-12-08  Initial release
*/
#include <stdlib.h>
#include <stdint.h>
#include "zwscn_defines.h"
#include "zwscn_scene_api.h"
#include "zwscn_util.h"
#include "zwscn_util_message_log.h"
#include "zip_api.h"
#include "zwscn_scene_api.h"
#include "zwscn_device_args.h"
#include "zwscn_util_zwave_eval.h"

#define eval(val, a) ((val) == (a))
#define eval_range(val, a, b) (((val) >= (a)) && ((val) <= (b)))
#define eval_if(condition, val, a) if(condition) eval(val, a)
#define eval_if_range(condition, val, a, b) if(condition) evan_range(val, a, b)

ZWSCN_BOOL_T zwscn_eval_basic_set(zwscn_args_basic_set_t val);
ZWSCN_BOOL_T zwscn_eval_switch_binary_set(zwscn_args_switch_binary_set_t val);
ZWSCN_BOOL_T zwscn_eval_switch_multilevel_set(zwscn_args_switch_multilevel_set_t val);
ZWSCN_BOOL_T zwscn_eval_door_lock_operation_set(zwscn_args_door_lock_operation_set_t val);
ZWSCN_BOOL_T zwscn_eval_thermostat_setpoint_set(zwscn_args_thermostat_setpoint_set_t *val);
ZWSCN_BOOL_T zwscn_eval_sensor_binary(zwscn_args_sensor_binary_t val);
ZWSCN_BOOL_T zwscn_eval_sensor_multilevel(zwscn_args_sensor_multilevel_t *val);
ZWSCN_BOOL_T zwscn_eval_alarm(zwscn_args_alarm_t *val);
ZWSCN_BOOL_T zwscn_eval_basic_event(zwscn_args_basic_event_t val);
ZWSCN_BOOL_T zwscn_eval_central_scene(zwscn_args_central_scene_t val);

ZWSCN_BOOL_T zwscn_eval_basic_set(zwscn_args_basic_set_t val) {
    return (eval_range(val.value, 0x00, 0x63) || eval(val.value, 0xFF));
}

ZWSCN_BOOL_T zwscn_eval_switch_binary_set(zwscn_args_switch_binary_set_t val) {
    return (eval_range(val.value, 0x00, 0x63) || eval(val.value, 0xFF));
}

ZWSCN_BOOL_T zwscn_eval_switch_multilevel_set(zwscn_args_switch_multilevel_set_t val) {
    return (eval_range(val.value, 0x00, 0x63) || eval(val.value, 0xFF));
}

ZWSCN_BOOL_T zwscn_eval_door_lock_operation_set(zwscn_args_door_lock_operation_set_t val) {
    return (eval_range(val.mode, 0x00, 0x01) || eval_range(val.mode, 0x10, 0x11) || eval_range(val.mode, 0x20, 0x21) || eval(val.mode, 0xFF));
}

ZWSCN_BOOL_T zwscn_eval_thermostat_setpoint_set(zwscn_args_thermostat_setpoint_set_t *val) {
    zwsetp_t *setp = &val->setpoint;
    return ((eval_range(setp->type, 0x01, 0x02) || eval_range(setp->type, 0x07, 0x0D)) && eval_range(setp->precision, 0x00, 0x07)
            && eval_range(setp->unit, 0x00, 0x01) && (eval_range(setp->size, 1, 2) || eval(setp->size, 4)));
}

ZWSCN_BOOL_T zwscn_eval_sensor_binary(zwscn_args_sensor_binary_t val) {
    return (eval_range(val.type, 0x01, 0x0D) || eval(val.type, 0xFF)) && (eval(val.sensor_value, 0x00) || eval(val.sensor_value, 0xFF));
}

ZWSCN_BOOL_T zwscn_eval_sensor_multilevel_value(zwsensor_t *val) {
    int rval = (eval_range(val->type, 0x01, 0x37) && eval_range(val->precision, 0x00, 0x07) && (eval_range(val->size, 1, 2) || eval(val->size, 4)));
    if (rval) {
        switch (val->type) {
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x08:
        case 0x09:
        case 0x0B:
        case 0x0C:
        case 0x0D:
        case 0x0E:
        case 0x0F:
        case 0x10:
        case 0x12:
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x1E:
        case 0x20:
        case 0x22:
        case 0x23:
        case 0x25:
        case 0x2D:
            rval = rval && eval_range(val->unit, 0x00, 0x01);
            break;
        case 0x13:
        case 0x14:
        case 0x15:
            rval = rval && eval_range(val->unit, 0x00, 0x02);
            break;
        case 0x19:
        case 0x1A:
        case 0x1F:
            rval = rval && eval_range(val->unit, 0x00, 0x03);
            break;
        default:
            rval = rval && eval(val->unit, 0x00);
            break;
        }
    }
    return rval;
}

ZWSCN_BOOL_T zwscn_eval_sensor_multilevel(zwscn_args_sensor_multilevel_t *val) {
    return (zwscn_eval_sensor_multilevel_value(&val->value_high) && zwscn_eval_sensor_multilevel_value(&val->value_low));
}

ZWSCN_BOOL_T zwscn_eval_alarm(zwscn_args_alarm_t *val) {
    int i = 0;
    int rval = (/*(eval_range(val->va_type, 0x01, 0x0B) || eval(val->va_type, 0xFF)) && */(eval_range(val->za_type, 0x00, 0x0B) || eval(val->za_type, 0xFF))
            && val->za_event_count <= ZWSCN_ARGS_ALARM_MAX_EVENTS);
    if (rval) {
        for (i = 0; i < val->za_event_count; ++i) {
            uint8_t za_event = val->za_event[i];
            switch (val->za_type) {
            case 0x00:
                break;
            case 0x01:
                rval = rval && (eval_range(za_event, 0x01, 0x02) || eval(za_event, 0xFE));
                break;
            case 0x02:
                rval = rval && (eval_range(za_event, 0x01, 0x02) || eval(za_event, 0xFE));
                break;
            case 0x03:
                rval = rval && (eval_range(za_event, 0x01, 0x02) || eval(za_event, 0xFE));
                break;
            case 0x04:
                rval = rval && (eval_range(za_event, 0x01, 0x06) || eval(za_event, 0xFE));
                break;
            case 0x05:
                rval = rval && (eval_range(za_event, 0x01, 0x04) || eval(za_event, 0xFE));
                break;
            case 0x06:
                rval = rval && (eval_range(za_event, 0x01, 0x06) || eval(za_event, 0xFE));
                break;
            case 0x07:
                rval = rval && (eval_range(za_event, 0x01, 0x06) || eval(za_event, 0xFE));
                break;
            case 0x08:
                rval = rval && (eval_range(za_event, 0x01, 0x05) || eval(za_event, 0xFE));
                break;
            case 0x09:
                rval = rval && (eval_range(za_event, 0x01, 0x02) || eval(za_event, 0xFE));
                break;
            case 0x0A:
                rval = rval && (eval_range(za_event, 0x01, 0x03) || eval(za_event, 0xFE));
                break;
            case 0x0B:
                rval = rval && eval(za_event, 0x01);
                break;
            }
        }
    }
    return rval;

}

ZWSCN_BOOL_T zwscn_eval_basic_event(zwscn_args_basic_event_t val) {
    return ((eval_range(val.value_high, 0x00, 0x63) || eval(val.value_high, 0xFF)) && (eval_range(val.value_low, 0x00, 0x63) || eval(val.value_low, 0xFF)));

}

ZWSCN_BOOL_T zwscn_eval_central_scene(zwscn_args_central_scene_t val) {
    return eval_range(val.central_scene_key_attribute, 0x00, 0x06);
}

ZWSCN_STATUS_T zwscn_eval_device_args(void *device_args, uint32_t id, uint32_t cmd, ZWSCN_BOOL_T *out_result) {
	
#if defined(ZWSCN_DEVICE_ARGS_EVAL_ENABLE) && (ZWSCN_DEVICE_ARGS_EVAL_ENABLE == 1)

    ZWSCN_BOOL_T result = ZWSCN_FALSE;
    uint16_t cls_id;
    if ((device_args == NULL) || (id == 0)) {
        goto l_err_args;
    }

    cls_id = zwscn_util_combo_id_to_cls_id(id);
    switch (cls_id) {
    case COMMAND_CLASS_BASIC: {
        if (cmd == BASIC_SET) {
            zwscn_args_basic_event_t *param;

            zwscn_message_log_debug("EVALUATE STATUS COMMAND_CLASS_BASIC EVENT\n");
            param = (zwscn_args_basic_event_t *) device_args;

            result = zwscn_eval_basic_event(*param);
        } else {
            zwscn_args_basic_set_t *param;

            zwscn_message_log_debug("EVALUATE STATUS COMMAND_CLASS_BASIC\n");
            param = (zwscn_args_basic_set_t *) device_args;

            result = zwscn_eval_basic_set(*param);
        }
    }
        break;
    case COMMAND_CLASS_SWITCH_BINARY: {
        zwscn_args_switch_binary_set_t *param;

        zwscn_message_log_debug("EVALUATE STATUS COMMAND_CLASS_SWITCH_BINARY\n");
        param = (zwscn_args_switch_binary_set_t *) device_args;

        result = zwscn_eval_switch_binary_set(*param);

    }
        break;
    case COMMAND_CLASS_SWITCH_MULTILEVEL: {
        zwscn_args_switch_multilevel_set_t *param;

        zwscn_message_log_debug("EVALUATE STATUS COMMAND_CLASS_SWITCH_MULTILEVEL\n");
        param = (zwscn_args_switch_multilevel_set_t *) device_args;

        result = zwscn_eval_switch_multilevel_set(*param);

    }
        break;
    case COMMAND_CLASS_DOOR_LOCK: {
        zwscn_args_door_lock_operation_set_t *param;

        zwscn_message_log_debug("EVALUATE STATUS COMMAND_CLASS_DOOR_LOCK\n");
        param = (zwscn_args_door_lock_operation_set_t *) device_args;

        result = zwscn_eval_door_lock_operation_set(*param);

    }
        break;
    case COMMAND_CLASS_THERMOSTAT_SETPOINT: {
        zwscn_args_thermostat_setpoint_set_t *param;

        zwscn_message_log_debug("EVALUATE STATUS COMMAND_CLASS_THERMOSTAT_SETPOINT\n");
        param = (zwscn_args_thermostat_setpoint_set_t *) device_args;

        result = zwscn_eval_thermostat_setpoint_set(param);

    }
        break;
    case COMMAND_CLASS_SENSOR_BINARY: {
        zwscn_args_sensor_binary_t *param;

        zwscn_message_log_debug("EVALUATE EVENT COMMAND_CLASS_SENSOR_BINARY\n");
        param = (zwscn_args_sensor_binary_t *) device_args;

        result = zwscn_eval_sensor_binary(*param);

    }
        break;
    case COMMAND_CLASS_SENSOR_MULTILEVEL: {
        zwscn_args_sensor_multilevel_t *param;

        zwscn_message_log_debug("EVALUATE EVENT COMMAND_CLASS_SENSOR_MULTILEVEL\n");
        param = (zwscn_args_sensor_multilevel_t *) device_args;

        result = zwscn_eval_sensor_multilevel(param);

    }
        break;
    case COMMAND_CLASS_ALARM: {
        zwscn_args_alarm_t *param;

        zwscn_message_log_debug("EVALUATE EVENT COMMAND_CLASS_ALARM\n");
        param = (zwscn_args_alarm_t *) device_args;

        result = zwscn_eval_alarm(param);

    }
        break;
    case COMMAND_CLASS_CENTRAL_SCENE: {
        zwscn_args_central_scene_t *param;

        zwscn_message_log_debug("EVALUATE EVENT COMMAND_CLASS_CENTRAL_SCENE\n");
        param = (zwscn_args_central_scene_t *) device_args;

        result = zwscn_eval_central_scene(*param);

    }
        break;
    }
    if(out_result){
        zwscn_message_log_debug("EVALUATE EVENT SUCCESS\n");
    } else {
        zwscn_message_log_debug("EVALUATE EVENT FAIL\n");
    }
    (*out_result) = result;
	
	return ZWSCN_ERR_NONE;
l_err_args: 
	return ZWSCN_ERR_INVALID_ARGS;
	
#else

    (*out_result) = ZWSCN_TRUE;
	return ZWSCN_ERR_NONE;
	
#endif //if 0
}

