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
@file		zwscn_util_zwave.c  
  
                To provide support for Z-Wave related functionality
  
@author		Arif Mohammand
 
@version	1.0 2013-12-05  Initial release
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "zip_api.h"

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
#include "zwscn_util_config.h"
#include "zwscn_util_rwlock.h"
#include "zwscn_util_time.h"
#include "zwscn_util_timer.h"
#include "zwscn_util_unit.h"
#include "zwscn_statelog.h"
#include "zwscn_scene.h"
#include "zwscn_device_args.h"
#include "zwscn_device_reports.h"
#include "zwscn_util_zwave.h"
#include "zwscn_zwpoll.h"
#include "zwscn_scene_status_update.h"

uint16_t zwscn_zwave_cc_for_action[] = {
	COMMAND_CLASS_BASIC,
	COMMAND_CLASS_SWITCH_BINARY,
	COMMAND_CLASS_SWITCH_MULTILEVEL,
	COMMAND_CLASS_DOOR_LOCK,
	COMMAND_CLASS_THERMOSTAT_SETPOINT
};

uint16_t zwscn_zwave_cc_for_event[] = {
	COMMAND_CLASS_BASIC, //incoming BASIC SET is used as EVENT
	COMMAND_CLASS_SENSOR_BINARY,
	COMMAND_CLASS_SENSOR_MULTILEVEL,
	COMMAND_CLASS_DOOR_LOCK,
	COMMAND_CLASS_ALARM,
	COMMAND_CLASS_CENTRAL_SCENE
};

uint16_t zwscn_zwave_cc_for_arm_security_event[] = { //The same is used for arm and disarm events
	COMMAND_CLASS_DOOR_LOCK,
	COMMAND_CLASS_CENTRAL_SCENE
};

uint16_t zwscn_zwave_cc_for_alarm_security_event[] = {
	COMMAND_CLASS_BASIC, //incoming BASIC SET is used as EVENT
	COMMAND_CLASS_SENSOR_BINARY,
	COMMAND_CLASS_ALARM
};

uint16_t zwscn_zwave_cc_for_postset_poll[] = {
	COMMAND_CLASS_SWITCH_MULTILEVEL,
	COMMAND_CLASS_DOOR_LOCK
};

const char *zwscn_zwave_cc_for_alarm_security_event_string[] = {
	"Basic",
	"Binary Sensor",
	"Notification"
};

const char *zwscn_zwave_cc_thermostat_setpoint_units[ZWSCN_ZWAVE_CC_THERMOSTAT_SETPOINT_MAX_UNIT + 1] = {
	"celsius", "fahrenheit"
};
	
const char *zwscn_zwave_cc_sensor_multilevel_units[][ZWSCN_ZWAVE_CC_SENSOR_MULTILEVEL_MAX_UNIT + 1] = {
	{NULL, NULL, NULL, NULL},											/**< Reserved */
	{"celsius", "fahrenheit", NULL, NULL},								/**< Air Temperature (v1) */
	{"percent", "dimensionless", NULL, NULL}, 							/**< General Purpose (v1) */
	{"percent", "lux", NULL, NULL},										/**< Luminance (v1) */
	{"watt", "Btu/h", NULL, NULL},										/**< Power (v2) */
	{"percent", "g/m3", NULL, NULL},									/**< Humidity (v2 + v5) */ //TODO UDUnits does not convert
	{"m/s", "mi/h", NULL, NULL},										/**< Velocity (v2) */
	{"degree", NULL, NULL, NULL}, 										/**< Direction (v2) */
	{"kPa", "inHg", NULL, NULL},										/**< Atmospheric Pressure (v2) */
	{"kPa", "inHg", NULL, NULL},										/**< Barometric Pressure (v2) */
	{"W/m2", NULL, NULL, NULL},											/**< Solar Radiation (v2) */
	{"celsius", "fahrenheit", NULL, NULL},								/**< Dew point (v2) */
	{"mm/h", "in/h", NULL, NULL},										/**< Rain rate (v2) */
	{"m", "ft", NULL, NULL},											/**< Tide level (v2) */
	{"kg", "lb", NULL, NULL},											/**< Weight (v3) */
	{"V", "mV", NULL, NULL},											/**< Voltage (v3) */
	{"A", "mA", NULL, NULL},											/**< Current (v3) */
	{"ppm", NULL, NULL, NULL},											/**< Carbon Dioxide CO2-level (v3) */
	{"m3/h", "ft3/min", NULL, NULL},									/**< Air flow (v3) */
	{"l", "m3", "gallon"},												/**< Tank capacity (v3) */
	{"m", "cm", "ft"},													/**< Distance (v3) */
	{"degree", NULL, NULL, NULL},										/**< Angle Position (v4) */ //TODO
	{"rpm", "Hz", NULL, NULL},											/**< Rotation (v5) */		//TODO - Wrong value from UDUnits
	{"celsius", "fahrenheit", NULL, NULL},								/**< Water Temperature (v5) */
	{"celsius", "fahrenheit", NULL, NULL},								/**< Soil Temperature (v5) */
	{NULL, NULL, NULL, NULL},											/**< Seismic Intensity (v5) */ //TODO
	{NULL, NULL, NULL, NULL},											/**< Seismic Magnitude (v5) */ //TODO
	{NULL, NULL, NULL, NULL},											/**< Ultraviolet (v5) */ //TODO
	{"ohm.m", NULL, NULL, NULL},										/**< Electrical Resistivity (v5) */
	{"S/m", NULL, NULL, NULL},											/**< Electrical Conductivity (v5) */
	{NULL, NULL, NULL, NULL},											/**< Loudness (v5) */ //TODO
	{NULL, NULL, NULL, NULL},											/**< Moisture (v5) */ //TODO
	{"Hz", "kHz", NULL, NULL},											/**< Frequency (v6) */
	{"s", NULL, NULL, NULL},											/**< Time (v6) */
	{"celsius", "fahrenheit", NULL, NULL},								/**< Target Temperature (v6) */
	{"mol/m3", "ug/m3", NULL, NULL},									/**< Particulate Matter 2.5 (v7) */
	{"mol/m3", NULL, NULL, NULL},										/**< Formaldehyde CH2O-level (v7) */
	{"Bq/m3", "pCi/l", NULL, NULL},										/**< Radon Concentration (v7) */
	{"mol/m3", NULL, NULL, NULL},										/**< Methane Density CH4 (v7) */
	{"mol/m3", NULL, NULL, NULL},										/**< Volatile Organic Compound (v7) */
	{"mol/m3", NULL, NULL, NULL},										/**< Carbon Monoxide CO-level (v7) */
	{"percent", NULL, NULL, NULL},										/**< Soil Humidity (v7) */
	{NULL, NULL, NULL, NULL},											/**< Soil Humidity (v7) */ //TODO
	{"mol/m3", NULL, NULL, NULL},										/**< Soil Salinity (v7) */
	{NULL, NULL, NULL, NULL},											/**< Heart Rate (v7) */ //TODO
	{NULL, NULL, NULL, NULL},											/**< Blood Pressure (v7) */ //TODO
	{"kg", NULL, NULL, NULL},											/**< Muscle Mass (v7) */
	{"kg", NULL, NULL, NULL},											/**< Fat Mass (v7) */
	{"kg", NULL, NULL, NULL},											/**< Bone Mass (v7) */
	{"kg", NULL, NULL, NULL},											/**< Total Body Water, TBW (v7) */
	{"J", NULL, NULL, NULL},											/**< Basic Metabolic Rate, BMR (v7) */
	{NULL, NULL, NULL, NULL}											/**< Body Mass Index, BMI (v7) */ //TODO
};

const char *zwscn_zwave_cc_alarm_type[] = {
	"Unknown Alarm",
	"Smoke Alarm",
	"CO Alarm",
	"CO2 Alarm",
	"Heat Alarm",
	"Water Alarm",
	"Access Control Alarm",
	"Burgler Alarm",
	"Power Management Alarm",
	"System Alarm",
	"Emergency Alarm",
	"Alarm Clock",
	"Appliance",
	"Home Health"
};

ZWSCN_BOOL_T zwscn_zwave_compare_cc_basic(zwscn_args_basic_set_t *device_args, zwscn_report_basic_t *report);
ZWSCN_BOOL_T zwscn_zwave_compare_cc_switch_binary(zwscn_args_switch_binary_set_t *device_args, zwscn_report_switch_binary_t *report);
ZWSCN_BOOL_T zwscn_zwave_compare_cc_switch_multilevel(zwscn_args_switch_multilevel_set_t *device_args, zwscn_report_switch_multilevel_t *report);
ZWSCN_BOOL_T zwscn_zwave_compare_cc_door_lock_op(void *net, zwscn_args_door_lock_operation_set_t *device_args, zwscn_report_door_lock_operation_t *report, uint32_t cmd);
ZWSCN_BOOL_T zwscn_zwave_compare_cc_thermostat_setpoint(zwscn_args_thermostat_setpoint_set_t *device_args, zwscn_report_thermostat_setpoint_t *report);
ZWSCN_BOOL_T zwscn_zwave_compare_cc_sensor_binary(void *net, zwscn_args_sensor_binary_t *device_args, zwscn_report_sensor_binary_t *report);
ZWSCN_BOOL_T zwscn_zwave_compare_cc_sensor_multilevel(zwscn_args_sensor_multilevel_t *device_args, zwscn_report_sensor_multilevel_t *report);
ZWSCN_BOOL_T zwscn_zwave_compare_cc_alarm(void *net, zwscn_args_alarm_t *device_args, zwscn_report_alarm_t *report);
ZWSCN_BOOL_T zwscn_zwave_compare_cc_basic_event(zwscn_args_basic_event_t *device_args, zwscn_report_basic_event_t *report);
ZWSCN_BOOL_T zwscn_zwave_compare_cc_central_scene(zwscn_args_central_scene_t *device_args, zwscn_report_central_scene_t *report);

void *zwscn_zwave_prepare_get_op_args(void *net, zwscn_action_p action_ptr) {
	uint16_t cls_id;
	void *result=NULL;
	
	cls_id = zwscn_util_combo_id_to_cls_id(action_ptr->id);
	switch(cls_id)
	{
		case COMMAND_CLASS_THERMOSTAT_SETPOINT:
		//case COMMAND_CLASS_THERMOSTAT_SETPOINT_V2:
		//case COMMAND_CLASS_THERMOSTAT_SETPOINT_V3:
			{
				zwscn_args_thermostat_setpoint_set_t *args_set_ptr;
				zwscn_args_thermostat_setpoint_get_t *args_get_ptr;
				
				if(action_ptr->device_args_size && action_ptr->device_args) {
					args_set_ptr = (zwscn_args_thermostat_setpoint_set_t *) action_ptr->device_args;
					args_get_ptr = malloc(sizeof(zwscn_args_thermostat_setpoint_get_t));
					if(args_get_ptr != NULL) {
						args_get_ptr->type = args_set_ptr->setpoint.type;
						result = args_get_ptr;
					}
				}
			}
			break;
	}
	
	return result;
}

void *zwscn_zwave_prepare_set_op_args(void *net, zwscn_action_p action_ptr) {
//	uint16_t cls_id;
	void *result=NULL;
	void *args_set_ptr;
	
	if(action_ptr->device_args_size && action_ptr->device_args) {
		args_set_ptr = malloc(action_ptr->device_args_size);
		if(args_set_ptr != NULL) {
			memcpy(args_set_ptr, action_ptr->device_args, action_ptr->device_args_size);
			result = args_set_ptr;
		}
	}
	
	return result;
}

uint16_t zwscn_zwave_get_zwpoll_count(zwscn_action_p action_ptr) {
	uint16_t cls_id;
	uint16_t result = ZWSCN_ZWPOLL_COUNT_DEFAULT;
	
	cls_id = zwscn_util_combo_id_to_cls_id(action_ptr->id);
	switch( cls_id )
	{
		case COMMAND_CLASS_SWITCH_MULTILEVEL:
			result = ZWSCN_ZWPOLL_COUNT_CC_SWITCH_MULTILEVEL;
		break;
		case COMMAND_CLASS_DOOR_LOCK:
			result = ZWSCN_ZWPOLL_COUNT_CC_DOOR_LOCK;
		break; 
	}
	
	return result;
}

uint8_t zwscn_zwave_find_cmd_get(uint16_t cls_id, uint8_t cmd) {
	uint8_t result=0;
	
	switch(cls_id)
	{
	case COMMAND_CLASS_BASIC:
		result = BASIC_GET;
		break;
	case COMMAND_CLASS_SWITCH_BINARY:
		result = SWITCH_BINARY_GET;
		break;
	case COMMAND_CLASS_SWITCH_MULTILEVEL:
		result = SWITCH_MULTILEVEL_GET;
		break;
	case COMMAND_CLASS_DOOR_LOCK:
		if(cmd == DOOR_LOCK_CONFIGURATION_SET) {
			result = DOOR_LOCK_CONFIGURATION_GET;
		} else if(cmd == DOOR_LOCK_OPERATION_SET) {
			result = DOOR_LOCK_OPERATION_GET;
		}
		break;
	case COMMAND_CLASS_THERMOSTAT_SETPOINT:
		result = THERMOSTAT_SETPOINT_GET;
		break;
	default:
		zwscn_message_log_warning("Command Class (%u) is not supported\n", cls_id);
	}
	
	return result;
}

const char *zwscn_zwave_get_unit_string(uint16_t cls_id, uint8_t type, uint8_t unit) {
	const char *result = NULL;
	
	
	switch(cls_id)
	{
		case COMMAND_CLASS_SENSOR_MULTILEVEL:
			if((type == ZWSCN_ZWAVE_CC_SENSOR_MULTILEVEL_TYPE_RESERVED) || 
				(type > ZWSCN_ZWAVE_CC_SENSOR_MULTILEVEL_MAX_TYPE) || 
				(unit > ZWSCN_ZWAVE_CC_SENSOR_MULTILEVEL_MAX_UNIT)) {
				goto l_err_args;
			}
			result = zwscn_zwave_cc_sensor_multilevel_units[type][unit];
			break;
		case COMMAND_CLASS_THERMOSTAT_SETPOINT:
			if(unit > ZWSCN_ZWAVE_CC_THERMOSTAT_SETPOINT_MAX_UNIT) {
				goto l_err_args;
			}
			result = zwscn_zwave_cc_thermostat_setpoint_units[unit];
			break;
	}
	
	return result;
l_err_args:
	return NULL;
}

ZWSCN_BOOL_T zwscn_zwave_is_cc_for_action(uint16_t cls_id, uint8_t cmd) {
	ZWSCN_BOOL_T result =  ZWSCN_FALSE;
	int count = 0;
	int i = 0;
	
	count = sizeof(zwscn_zwave_cc_for_action)/sizeof(uint16_t);
	ZWSCN_MESSAGE_LOG_DEBUG("Total Action CC count=%d\n", count);
	for(i=0; i < count; ++i) {
		ZWSCN_MESSAGE_LOG_DEBUG("checking cc=%d against cls_id=%d\n", zwscn_zwave_cc_for_action[i], cls_id);
		if(zwscn_zwave_cc_for_action[i] == cls_id) {
			result = ZWSCN_TRUE;
			break;
		}
	}
	if((cls_id == COMMAND_CLASS_BASIC) && (cmd == BASIC_SET)) { //special case where incoming BASIC SET is used as EVENT
		ZWSCN_MESSAGE_LOG_DEBUG("BASIC cc=0x%x SET cmd=0x%x is not a valid value for actions\n", cls_id, cmd);
		result = ZWSCN_FALSE;
	}
	
	return result;
}

ZWSCN_BOOL_T zwscn_zwave_is_cc_for_event(uint16_t cls_id, uint8_t cmd) {
	ZWSCN_BOOL_T result =  ZWSCN_FALSE;
	int count = 0;
	int i = 0;
	
	count = sizeof(zwscn_zwave_cc_for_event)/sizeof(uint16_t);
	ZWSCN_MESSAGE_LOG_DEBUG("Total Event CC count=%d\n", count);
	for(i=0; i < count; ++i) {
		ZWSCN_MESSAGE_LOG_DEBUG("checking cc=%d against cls_id=%d\n", zwscn_zwave_cc_for_event[i], cls_id);
		if(zwscn_zwave_cc_for_event[i] == cls_id) {
			result = ZWSCN_TRUE;
			break;
		}
	}
	if((cls_id == COMMAND_CLASS_BASIC) && (cmd != BASIC_SET)) { //special case where incoming BASIC SET is used as EVENT
		ZWSCN_MESSAGE_LOG_DEBUG("BASIC cc=0x%x SET cmd=0x%x is not a valid value for events\n", cls_id, cmd);
		result = ZWSCN_FALSE;
	}
	
	return result;
}

ZWSCN_BOOL_T zwscn_zwave_is_cc_for_arm_security_event(uint16_t cls_id, uint8_t cmd) {
	ZWSCN_BOOL_T result =  ZWSCN_FALSE;
	int count = 0;
	int i = 0;
	
	count = sizeof(zwscn_zwave_cc_for_arm_security_event)/sizeof(uint16_t);
	ZWSCN_MESSAGE_LOG_DEBUG("Total Arm Security Event CC count=%d\n", count);
	for(i=0; i < count; ++i) {
		ZWSCN_MESSAGE_LOG_DEBUG("checking cc=%d against cls_id=%d\n", zwscn_zwave_cc_for_arm_security_event[i], cls_id);
		if(zwscn_zwave_cc_for_arm_security_event[i] == cls_id) {
			result = ZWSCN_TRUE;
			break;
		}
	}
	
	return result;
}

ZWSCN_BOOL_T zwscn_zwave_is_cc_for_alarm_security_event(uint16_t cls_id, uint8_t cmd) {
	ZWSCN_BOOL_T result =  ZWSCN_FALSE;
	int count = 0;
	int i = 0;
	
	count = sizeof(zwscn_zwave_cc_for_alarm_security_event)/sizeof(uint16_t);
	ZWSCN_MESSAGE_LOG_DEBUG("Total Alarm Security Event CC count=%d\n", count);
	for(i=0; i < count; ++i) {
		ZWSCN_MESSAGE_LOG_DEBUG("checking cc=%d against cls_id=%d\n", zwscn_zwave_cc_for_alarm_security_event[i], cls_id);
		if(zwscn_zwave_cc_for_alarm_security_event[i] == cls_id) {
			result = ZWSCN_TRUE;
			break;
		}
	}
	if((cls_id == COMMAND_CLASS_BASIC) && ((cmd != BASIC_SET) && (cmd != 0x5))) { //special case where incoming BASIC SET is used as EVENT - note: value 0x5 is set by UI 
		ZWSCN_MESSAGE_LOG_DEBUG("BASIC cc=0x%x SET cmd=0x%x is not a valid value for events\n", cls_id, cmd);
		result = ZWSCN_FALSE;
	}
	
	return result;
}

ZWSCN_BOOL_T zwscn_zwave_cc_has_postset_poll(uint16_t cls_id, uint8_t cmd) {
	ZWSCN_BOOL_T result =  ZWSCN_FALSE;
	int count = 0;
	int i = 0;
	
	count = sizeof(zwscn_zwave_cc_for_postset_poll)/sizeof(uint16_t);
//	ZWSCN_MESSAGE_LOG_DEBUG("Total Post-Set Poll CC count=%d\n", count);
	for(i=0; i < count; ++i) {
//		ZWSCN_MESSAGE_LOG_DEBUG("checking cc=%d against cls_id=%d\n", zwscn_zwave_cc_for_postset_poll[i], cls_id);
		if(zwscn_zwave_cc_for_postset_poll[i] == cls_id) {
			result = ZWSCN_TRUE;
			break;
		}
	}
	
	ZWSCN_MESSAGE_LOG_DEBUG("input cls:%u cmd:%u, output result:%u\n", cls_id, cmd, result);
	return result;
}

ZWSCN_STATUS_T zwscn_zwave_get_device_report_timestamp(uint16_t cls_id, void *report, uint32_t report_size, zwscn_time_t *report_timestamp) {
	zwscn_time_t sutime = 0;
	
	if((cls_id == 0) || (report == NULL) || (report_size == 0)) {
		goto l_err_args;
	} 
	
	switch(cls_id) {
		case COMMAND_CLASS_BASIC:
		{
			zwscn_report_basic_t *param_rep;

			zwscn_message_log_debug("COMMAND_CLASS_BASIC\n");
			param_rep = (zwscn_report_basic_t *)report;
			sutime = param_rep->utime;
		}
		break;
		case COMMAND_CLASS_SWITCH_BINARY:
		{
			zwscn_report_switch_binary_t *param_rep;

			zwscn_message_log_debug("COMMAND_CLASS_SWITCH_BINARY\n");
			param_rep = (zwscn_report_switch_binary_t *)report;
			sutime = param_rep->utime;
		}
		break;
		case COMMAND_CLASS_SWITCH_MULTILEVEL:
		{
			zwscn_report_switch_multilevel_t *param_rep;

			zwscn_message_log_debug("COMMAND_CLASS_SWITCH_MULTILEVEL\n");
			param_rep = (zwscn_report_switch_multilevel_t *)report;
			sutime = param_rep->utime;
		}
		break;
		case COMMAND_CLASS_DOOR_LOCK:
		{
			zwscn_report_door_lock_operation_t *param_rep;

			zwscn_message_log_debug("COMMAND_CLASS_DOOR_LOCK\n");
			param_rep = (zwscn_report_door_lock_operation_t *)report;
			sutime = param_rep->utime;
		}
		break;
		case COMMAND_CLASS_THERMOSTAT_SETPOINT:
		{
			zwscn_report_thermostat_setpoint_t *param_rep;

			zwscn_message_log_debug("COMMAND_CLASS_THERMOSTAT_SETPOINT\n");
			param_rep = (zwscn_report_thermostat_setpoint_t *)report;
			sutime = param_rep->utime;
		}
		break;
		case COMMAND_CLASS_SENSOR_BINARY:
		{
			zwscn_report_sensor_binary_t *param_rep;

			zwscn_message_log_debug("COMMAND_CLASS_SENSOR_BINARY\n");
			param_rep = (zwscn_report_sensor_binary_t *)report;
			sutime = (param_rep->sensor_value == 0) ? param_rep->idle_utime : param_rep->event_utime;
		}
		break;
		case COMMAND_CLASS_SENSOR_MULTILEVEL:
		{
			zwscn_report_sensor_multilevel_t *param_rep;

			zwscn_message_log_debug("COMMAND_CLASS_SENSOR_MULTILEVEL\n");
			param_rep = (zwscn_report_sensor_multilevel_t *)report;
			sutime = param_rep->utime;
		}
		break;
		case COMMAND_CLASS_ALARM:
		{
			zwscn_report_alarm_t *param_rep;

			zwscn_message_log_debug("COMMAND_CLASS_ALARM\n");
			param_rep = (zwscn_report_alarm_t *)report;
			sutime = param_rep->utime;
		}
		break;
	}
	
	(*report_timestamp) = sutime;
	zwscn_message_log_debug("Report sutime=%lu\n", sutime);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
//l_err:
//	return ZWSCN_ERR_GENERIC;	
}

ZWSCN_STATUS_T zwscn_zwave_compare_cc(void *net, void *device_args, uint32_t id, uint32_t cmd, void *report, ZWSCN_BOOL_T *out_result, zwscn_time_t *out_sutime) {
	ZWSCN_BOOL_T result = ZWSCN_FALSE;
	zwscn_time_t sutime = 0;
	uint16_t cls_id;
	
	if((device_args == NULL) || (id == 0) || (report == NULL)) {
		goto l_err_args;
	} 
	
	cls_id = zwscn_util_combo_id_to_cls_id(id);
	switch(cls_id) {
		case COMMAND_CLASS_BASIC:
		{
			if(cmd == BASIC_SET) {
				zwscn_args_basic_event_t *param;
				zwscn_report_basic_event_t *param_rep;

				zwscn_message_log_debug("STATUS COMMAND_CLASS_BASIC EVENT\n");
				param = (zwscn_args_basic_event_t *)device_args;
				param_rep = (zwscn_report_basic_event_t *)report;
				
				result = zwscn_zwave_compare_cc_basic_event(param, param_rep);
				sutime = param_rep->utime;
			} else {
				zwscn_args_basic_set_t *param;
				zwscn_report_basic_t *param_rep;

				zwscn_message_log_debug("STATUS COMMAND_CLASS_BASIC\n");
				param = (zwscn_args_basic_set_t *)device_args;
				param_rep = (zwscn_report_basic_t *)report;
				
				result = zwscn_zwave_compare_cc_basic(param, param_rep);
				sutime = param_rep->utime;
			}
		}
		break;
		case COMMAND_CLASS_SWITCH_BINARY:
		{
			zwscn_args_switch_binary_set_t *param;
			zwscn_report_switch_binary_t *param_rep;

			zwscn_message_log_debug("STATUS COMMAND_CLASS_SWITCH_BINARY\n");
			param = (zwscn_args_switch_binary_set_t *)device_args;
			param_rep = (zwscn_report_switch_binary_t *)report;
			
			result = zwscn_zwave_compare_cc_switch_binary(param, param_rep);
			sutime = param_rep->utime;
		}
		break;
		case COMMAND_CLASS_SWITCH_MULTILEVEL:
		{
			zwscn_args_switch_multilevel_set_t *param;
			zwscn_report_switch_multilevel_t *param_rep;

			zwscn_message_log_debug("STATUS COMMAND_CLASS_SWITCH_MULTILEVEL\n");
			param = (zwscn_args_switch_multilevel_set_t *)device_args;
			param_rep = (zwscn_report_switch_multilevel_t *)report;
			
			result = zwscn_zwave_compare_cc_switch_multilevel(param, param_rep);
			sutime = param_rep->utime;
		}
		break;
		case COMMAND_CLASS_DOOR_LOCK:
		{
			zwscn_args_door_lock_operation_set_t *param;
			zwscn_report_door_lock_operation_t *param_rep;

			zwscn_message_log_debug("STATUS COMMAND_CLASS_DOOR_LOCK\n");
			param = (zwscn_args_door_lock_operation_set_t *)device_args;
			param_rep = (zwscn_report_door_lock_operation_t *)report;
			
			result = zwscn_zwave_compare_cc_door_lock_op(net, param, param_rep, cmd); //cmd is passed to differenciate between a Doorlock Action or Event as Doolock can be used for both
			sutime = param_rep->utime;
			
		}
		break;
		case COMMAND_CLASS_THERMOSTAT_SETPOINT:
		{
			zwscn_args_thermostat_setpoint_set_t *param;
			zwscn_report_thermostat_setpoint_t *param_rep;

			zwscn_message_log_debug("STATUS COMMAND_CLASS_THERMOSTAT_SETPOINT\n");
			param = (zwscn_args_thermostat_setpoint_set_t *)device_args;
			param_rep = (zwscn_report_thermostat_setpoint_t *)report;
			
			if(param->setpoint.type != param_rep->value.type) {
				zwscn_message_log_debug("THERMOSTAT SETPOINT target type=%d, current type=%d. Ignoring...\n", param->setpoint.type, param_rep->value.type);
				goto l_err_not_found;
			}
			result = zwscn_zwave_compare_cc_thermostat_setpoint(param, param_rep);
			sutime = param_rep->utime;
		}
		break;
		case COMMAND_CLASS_SENSOR_BINARY:
		{
			zwscn_args_sensor_binary_t *param;
			zwscn_report_sensor_binary_t *param_rep;

			zwscn_message_log_debug("EVENT COMMAND_CLASS_SENSOR_BINARY\n");
			param = (zwscn_args_sensor_binary_t *)device_args;
			param_rep = (zwscn_report_sensor_binary_t *)report;
			
			result = zwscn_zwave_compare_cc_sensor_binary(net, param, param_rep);
			sutime = (param_rep->sensor_value == 0) ? param_rep->idle_utime : param_rep->event_utime;
		}
		break;
		case COMMAND_CLASS_SENSOR_MULTILEVEL:
		{
			zwscn_args_sensor_multilevel_t *param;
			zwscn_report_sensor_multilevel_t *param_rep;

			zwscn_message_log_debug("EVENT COMMAND_CLASS_SENSOR_MULTILEVEL\n");
			param = (zwscn_args_sensor_multilevel_t *)device_args;
			param_rep = (zwscn_report_sensor_multilevel_t *)report;
			
			result = zwscn_zwave_compare_cc_sensor_multilevel(param, param_rep);
			sutime = param_rep->utime;
		}
		break;
		case COMMAND_CLASS_ALARM:
		{
			zwscn_args_alarm_t *param;
			zwscn_report_alarm_t *param_rep;

			zwscn_message_log_debug("EVENT COMMAND_CLASS_ALARM\n");
			param = (zwscn_args_alarm_t *)device_args;
			param_rep = (zwscn_report_alarm_t *)report;
			
			result = zwscn_zwave_compare_cc_alarm(net, param, param_rep);
			sutime = param_rep->utime;
		}
		break;
		case COMMAND_CLASS_CENTRAL_SCENE:
		{
			zwscn_args_central_scene_t *param;
			zwscn_report_central_scene_t *param_rep;

			zwscn_message_log_debug("EVENT COMMAND_CLASS_CENTRAL_SCENE\n");
			param = (zwscn_args_central_scene_t *)device_args;
			param_rep = (zwscn_report_central_scene_t *)report;
			
			result = zwscn_zwave_compare_cc_central_scene(param, param_rep);
			sutime = param_rep->utime;
		}
		break;
	}
	
	(*out_result) = result;
	(*out_sutime) = sutime;
	zwscn_message_log_debug("NEW status=%d sutime=%lu\n", result, sutime);
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
//l_err:
//	return ZWSCN_ERR_GENERIC;
l_err_not_found:
	return ZWSCN_ERR_NOT_FOUND;
}

ZWSCN_BOOL_T zwscn_zwave_compare_cc_basic(zwscn_args_basic_set_t *device_args, zwscn_report_basic_t *report) {
	ZWSCN_BOOL_T result = ZWSCN_FALSE;
	
	zwscn_message_log_debug("BASIC target value=%d current value=%d\n", device_args->value, report->value);
	if(device_args->value == report->value) {
		result = ZWSCN_TRUE;
	} else {
		result = ZWSCN_FALSE;
	}
	
	return result;
}

ZWSCN_BOOL_T zwscn_zwave_compare_cc_switch_binary(zwscn_args_switch_binary_set_t *device_args, zwscn_report_switch_binary_t *report) {
	ZWSCN_BOOL_T result = ZWSCN_FALSE;
	
	zwscn_message_log_debug("SWITCH BINARY target value=%d current value=%d\n", device_args->value, report->value);
	if(device_args->value == report->value) {
		result = ZWSCN_TRUE;
	} else {
		result = ZWSCN_FALSE;
	}
	
	return result;
}

ZWSCN_BOOL_T zwscn_zwave_compare_cc_switch_multilevel(zwscn_args_switch_multilevel_set_t *device_args, zwscn_report_switch_multilevel_t *report) {
	ZWSCN_BOOL_T result = ZWSCN_FALSE;
	uint8_t range_low, range_high;
	
	zwscn_message_log_debug("SWITCH MULTILEVEL target value=%d current value=%d\n", device_args->value, report->value);
	
	// Handle underflow
	range_low = (device_args->value > ZWSCN_STATUS_CC_SWITCH_MULTILEVEL_THRESHHOLD) ? (device_args->value - ZWSCN_STATUS_CC_SWITCH_MULTILEVEL_THRESHHOLD) : 1;
	// Handle (unlikely) overflow
	range_high = ((device_args->value + ZWSCN_STATUS_CC_SWITCH_MULTILEVEL_THRESHHOLD) > device_args->value) ? (device_args->value + ZWSCN_STATUS_CC_SWITCH_MULTILEVEL_THRESHHOLD) : device_args->value;
	// If target is 0, expected value is exactly 0
	if (((device_args->value == 0) && (report->value == 0))
			// If target is non-zero but unknown, expected value is any non-zero value
			|| ((device_args->value == 0xFF) && (report->value != 0))
			// If target is non-zero known value, expected value is any value in the range centered around the target
			|| ((device_args->value != 0) && (device_args->value != 0xFF) && (report->value >= range_low) && (report->value <= range_high))) {
		result = ZWSCN_TRUE;
	} else {
		result = ZWSCN_FALSE;
	}
	
	return result;
}

ZWSCN_BOOL_T zwscn_zwave_compare_cc_door_lock_op(void *net, zwscn_args_door_lock_operation_set_t *device_args, zwscn_report_door_lock_operation_t *report, uint32_t cmd) {
	ZWSCN_BOOL_T result = ZWSCN_FALSE;
	zwscn_time_t event_match_time;
	zwscn_time_t time_diff;
	uint16_t zwscn_zwave_same_report_ignore_interval;
	
	if(cmd == DOOR_LOCK_OPERATION_SET) { //it is an Action
		zwscn_message_log_debug("DOOR LOCK Action target mode=%d current mode=%d\n", device_args->mode, report->operation.mode);
		if(device_args->mode == report->operation.mode) {
			result = ZWSCN_TRUE;
		}
	} else {
		zwscn_message_log_debug("DOOR LOCK Event target mode=%d current mode=%d last_match_time=%llu\n", device_args->mode, report->operation.mode, device_args->last_match_time);
		if(device_args->mode == report->operation.mode) {
			event_match_time = device_args->last_match_time;
			time_diff = ZWSCN_TIME_NOW() - event_match_time;
			zwscn_zwave_same_report_ignore_interval = zwscn_config_common_get_zwave_same_report_ignore_interval (net);
			zwscn_message_log_debug("DOOR LOCK CC event_match_time=%llu time_diff=%llu expected_diff=%u\n", event_match_time, time_diff, zwscn_zwave_same_report_ignore_interval); 
			if((event_match_time == 0) || (time_diff >= zwscn_zwave_same_report_ignore_interval)) {
					result = ZWSCN_TRUE;
					device_args->last_match_time = ZWSCN_TIME_NOW();
			}
		} else {
			device_args->last_match_time = 0;
		}
	}
	
	return result;
}

ZWSCN_BOOL_T zwscn_zwave_compare_cc_thermostat_setpoint(zwscn_args_thermostat_setpoint_set_t *device_args, zwscn_report_thermostat_setpoint_t *report) {
	ZWSCN_BOOL_T result = ZWSCN_FALSE;
	int32_t value_int_target, value_int_current;
	double value_double_target, value_double_current;
	uint8_t type;
	uint8_t unit;
	const char *unit_str_config = NULL;
	const char *unit_str_report = NULL;
	double value_double_current_converted;
	ZWSCN_STATUS_T status;
	
	type = device_args->setpoint.type;
	unit = device_args->setpoint.unit;
	zwscn_message_log_debug("THERMOSTAT_SETPOINT target type=%d current type=%d\n", device_args->setpoint.type, report->value.type);
	if(type == report->value.type) {
		//Get the correct value
		value_int_target = zwscn_util_data2value(device_args->setpoint.size, device_args->setpoint.data);
		value_int_current = zwscn_util_data2value(report->value.size, report->value.data);

		value_double_target = zwscn_util_int2double(value_int_target, device_args->setpoint.precision);
		value_double_current = zwscn_util_int2double(value_int_current, report->value.precision);
		zwscn_message_log_debug("THERMOSTAT_SETPOINT target value_double=%f current value_double=%f\n", value_double_target, value_double_current);
		
		//If Units are not the same then convert it	
		if(unit != report->value.unit) {
			unit_str_config = zwscn_zwave_get_unit_string(COMMAND_CLASS_THERMOSTAT_SETPOINT, type, unit);
			unit_str_report = zwscn_zwave_get_unit_string(COMMAND_CLASS_THERMOSTAT_SETPOINT, type, report->value.unit);
			if(unit_str_config != NULL) {
				zwscn_message_log_debug("UNITS THERMOSTAT_SETPOINT configuration type=0x%x unit=0x%x string=%s\n", type, unit, unit_str_config);
			} else {
				zwscn_message_log_debug("UNITS THERMOSTAT_SETPOINT configuration type=0x%x unit=0x%x string NOT found\n", type, unit, unit_str_config);
			}
			if(unit_str_report != NULL) {
				zwscn_message_log_debug("UNITS THERMOSTAT_SETPOINT report type=0x%x unit=0x%x string=%s\n", type, report->value.unit, unit_str_report);
			} else {
				zwscn_message_log_debug("UNITS THERMOSTAT_SETPOINT report type=0x%x unit=0x%x string NOT found\n", type, report->value.unit, unit_str_report);
			}
			status = zwscn_units_convert(unit_str_report, value_double_current, unit_str_config, &value_double_current_converted);
			if(status != ZWSCN_ERR_NONE) {
				zwscn_message_log_debug("UNITS zwscn_units_convert() returned error %d\n", status);
				goto l_err;
			}
			
			value_double_current = value_double_current_converted;
		}
		value_double_current = zwscn_util_round(value_double_current, device_args->setpoint.precision); //use configured precision rather than reported one
		zwscn_message_log_debug("THERMOSTAT_SETPOINT 2 target value_double=%f current value_double=%f\n", value_double_target, value_double_current);
		
		if(zwscn_is_value_equal(value_double_target, value_double_current)) {
			result = ZWSCN_TRUE;
		} else {
			result = ZWSCN_FALSE;
		}
	}
	
	return result;
l_err:
	return ZWSCN_FALSE;
}

ZWSCN_BOOL_T zwscn_zwave_compare_cc_sensor_binary(void *net, zwscn_args_sensor_binary_t *device_args, zwscn_report_sensor_binary_t *report) {
	ZWSCN_BOOL_T result = ZWSCN_FALSE;
	zwscn_time_t event_match_time;
	zwscn_time_t time_diff;
	uint16_t zwscn_zwave_same_report_ignore_interval;
	
	zwscn_message_log_debug("SENSOR_BINARY target value=%d current value=%d last_match_time=%llu\n", device_args->sensor_value, report->sensor_value, device_args->last_match_time);
	zwscn_message_log_debug("SENSOR_BINARY target type=%d current type=%d\n", device_args->type, report->type);
	if((device_args->type == 0) || 				//For SENSOR_BINARY CC V1
		(device_args->type == report->type)) {	//For SENSOR_BINARY CC V2
		if(device_args->sensor_value == report->sensor_value) { 
			event_match_time = device_args->last_match_time;
			time_diff = ZWSCN_TIME_NOW() - event_match_time;
			zwscn_zwave_same_report_ignore_interval = zwscn_config_common_get_zwave_same_report_ignore_interval(net);
			zwscn_message_log_debug("SENSOR_BINARY CC event_match_time=%llu time_diff=%llu expected_diff=%u\n", event_match_time, time_diff, zwscn_zwave_same_report_ignore_interval); 
			if((event_match_time == 0) || (time_diff >= zwscn_zwave_same_report_ignore_interval)) {
					result = ZWSCN_TRUE;
					device_args->last_match_time = ZWSCN_TIME_NOW();
			}
		} else {
			device_args->last_match_time = 0;
		}
	}
	
	return result;
}

ZWSCN_BOOL_T zwscn_zwave_compare_cc_sensor_multilevel(zwscn_args_sensor_multilevel_t *device_args, zwscn_report_sensor_multilevel_t *report) {
	int32_t value_int_target_value_low, value_int_target_value_high, value_int_current;
	double value_double_target_value_low, value_double_target_value_high, value_double_current, value_temp_double;
	uint8_t result_new_value = ZWSCN_FALSE;
	uint8_t result_last_value = ZWSCN_FALSE;
	ZWSCN_BOOL_T result = ZWSCN_FALSE;
	uint8_t type;
	uint8_t unit;
	const char *unit_str_config = NULL;
	const char *unit_str_report = NULL;
	double value_double_current_converted;
	ZWSCN_STATUS_T status;
	

	type = device_args->value_low.type; //This should be equal to device_args->value_high.type
	unit = device_args->value_low.unit; //This should be equal to device_args->value_high.unit
	zwscn_message_log_debug("SENSOR_MULTILEVEL type target=%d\n", type);
						
	if(type == report->value.type) {
		value_int_target_value_low = zwscn_util_data2value(device_args->value_low.size, device_args->value_low.data);
		value_int_target_value_high = zwscn_util_data2value(device_args->value_high.size, device_args->value_high.data);
		value_int_current = zwscn_util_data2value(report->value.size, report->value.data);
		zwscn_message_log_debug("\tINT low=%d high=%d current=%d\n", value_int_target_value_low, value_int_target_value_high, value_int_current);
		
		value_double_target_value_low = zwscn_util_int2double(value_int_target_value_low, device_args->value_low.precision);
		value_double_target_value_high = zwscn_util_int2double(value_int_target_value_high, device_args->value_high.precision);
		value_double_current = zwscn_util_int2double(value_int_current, report->value.precision);
		zwscn_message_log_debug("\tDOUBLE low=%f high=%f current=%f last=%f\n", value_double_target_value_low, value_double_target_value_high, value_double_current, device_args->value_last);
		
		if(value_double_target_value_low > value_double_target_value_high) {
			zwscn_message_log_debug("SENSOR_MULTILEVEL value_high is lower than value_low. Reversing...\n");
			value_temp_double = value_double_target_value_low;
			value_double_target_value_low = value_double_target_value_high;
			value_double_target_value_high = value_temp_double;
		}
		zwscn_message_log_debug("\tunit target=%d current=%d\n", unit, report->value.unit);
		if(unit != report->value.unit){
			unit_str_config = zwscn_zwave_get_unit_string(COMMAND_CLASS_SENSOR_MULTILEVEL, type, unit);
			unit_str_report = zwscn_zwave_get_unit_string(COMMAND_CLASS_SENSOR_MULTILEVEL, type, report->value.unit);
			if(unit_str_config != NULL) {
				zwscn_message_log_debug("UNITS SENSOR_MULTILEVEL event configuration type=0x%x unit=0x%x string=%s\n", type, unit, unit_str_config);
			} else {
				zwscn_message_log_debug("UNITS SENSOR_MULTILEVEL event configuration type=0x%x unit=0x%x string NOT found\n", type, unit, unit_str_config);
			}
			if(unit_str_report != NULL) {
				zwscn_message_log_debug("UNITS SENSOR_MULTILEVEL event report type=0x%x unit=0x%x string=%s\n", type, report->value.unit, unit_str_report);
			} else {
				zwscn_message_log_debug("UNITS SENSOR_MULTILEVEL event report type=0x%x unit=0x%x string NOT found\n", type, report->value.unit, unit_str_report);
			}
			status = zwscn_units_convert(unit_str_report, value_double_current, unit_str_config, &value_double_current_converted);
			if(status != ZWSCN_ERR_NONE) {
				zwscn_message_log_debug("UNITS zwscn_units_convert() returned error %d\n", status);
				goto l_err;
			}
			
			value_double_current = value_double_current_converted;
		}
		//Set all precision to the higher value between device_args->value_low.precision and device_args->value_high.precision before comparison
		if(device_args->value_low.precision > device_args->value_high.precision) {
			//value_double_target_value_high = zwscn_util_round(value_double_target_value_high, device_args->value_low.precision); //use configured precision rather than reported one
			value_double_current = zwscn_util_round(value_double_current, device_args->value_low.precision); //use configured precision rather than reported one
		} else {
			//value_double_target_value_low = zwscn_util_round(value_double_target_value_low, device_args->value_high.precision); //use configured precision rather than reported one
			value_double_current = zwscn_util_round(value_double_current, device_args->value_high.precision); //use configured precision rather than reported one
		}
		zwscn_message_log_debug("\tDOUBLE 2 low=%f high=%f current=%f last=%f\n", value_double_target_value_low, value_double_target_value_high, value_double_current, device_args->value_last);
		
		result_new_value = zwscn_is_value_in_range(value_double_target_value_low, value_double_target_value_high, value_double_current);
		zwscn_message_log_debug("SENSOR_MULTILEVEL is_value_in_range result=%d NEW\n", result_new_value);
		if(device_args->flag_first_time_check_done == ZWSCN_FALSE) { //It is the first time
			zwscn_message_log_debug("SENSOR_MULTILEVEL first time received a report for this event\n");
			result = result_new_value;
			device_args->flag_first_time_check_done = ZWSCN_TRUE;
		} else { //not the first time
			result_last_value = zwscn_is_value_in_range(value_double_target_value_low, value_double_target_value_high, device_args->value_last);
			zwscn_message_log_debug("SENSOR_MULTILEVEL is_value_in_range result=%d LAST\n", result_last_value);
			if((result_last_value == ZWSCN_FALSE) && (result_new_value == ZWSCN_TRUE)) { //Just entering the range after going out
				zwscn_message_log_debug("SENSOR_MULTILEVEL entered valid range again\n");
				result = ZWSCN_TRUE;
			}
		}
		device_args->value_last = value_double_current;
	}
	
	return result;
l_err:
	return ZWSCN_FALSE;
}

ZWSCN_BOOL_T zwscn_zwave_compare_cc_alarm(void *net, zwscn_args_alarm_t *device_args, zwscn_report_alarm_t *report) {
	ZWSCN_BOOL_T result_1 = ZWSCN_FALSE;
	ZWSCN_BOOL_T result_2 = ZWSCN_FALSE;
	ZWSCN_BOOL_T result = ZWSCN_FALSE;
	int i = 0;
	zwscn_time_t event_match_time;
	zwscn_time_t time_diff;
	uint16_t zwscn_zwave_same_report_ignore_interval;
	
	zwscn_message_log_debug("ALARM CC target va_type=%d current va_type=%d\n", device_args->va_type, report->value.type);
	if(report->value.ex_info) {
		zwscn_message_log_debug("ALARM CC target za_type=%d current za_type=%d\n", device_args->za_type, report->value.ex_type);
	} else {
		zwscn_message_log_debug("ALARM CC target za_type=%d current za_type=\"Not Valid\"\n", device_args->za_type);
	}
	
	if(device_args->va_type != 0) {
		if(device_args->va_type == report->value.type) {
			event_match_time = device_args->va_last_matched_time;
			time_diff = ZWSCN_TIME_NOW() - event_match_time;
			zwscn_zwave_same_report_ignore_interval = zwscn_config_common_get_zwave_same_report_ignore_interval(net);
			zwscn_message_log_debug("ALARM CC va_level=%d value_last=%d report_value=%d event_match_time=%llu time_diff=%llu expected_diff=%u\n", device_args->va_level, device_args->va_level_value_last, report->value.level, event_match_time, time_diff, zwscn_zwave_same_report_ignore_interval); 
			if((device_args->va_level == report->value.level) && 
				((device_args->va_level_value_last != report->value.level) || (event_match_time == 0) || (time_diff >= zwscn_zwave_same_report_ignore_interval))) {
				result_1 = ZWSCN_TRUE;
				device_args->va_last_matched_time = ZWSCN_TIME_NOW();
			} 
			device_args->va_level_value_last = report->value.level;
		}
	}
	
	if(device_args->za_type != 0) {
		if(report->value.ex_info) {
			if(device_args->za_type == report->value.ex_type) {
				if(report->value.ex_event == 0) { //idle event
					zwscn_message_log_debug("ALARM CC type=%d, event=idle. Resetting za_event_last_matched_time array\n", report->value.ex_type);
					memset(device_args->za_event_last_matched_time, 0, sizeof(device_args->za_event_last_matched_time)); //clear event match times for all events of this type
				} else {
					for(i=0; i < device_args->za_event_count; ++i) {
						 if(device_args->za_event[i] == report->value.ex_event) {
							 event_match_time = device_args->za_event_last_matched_time[i];
							 time_diff = ZWSCN_TIME_NOW() - event_match_time;
							 zwscn_zwave_same_report_ignore_interval = zwscn_config_common_get_zwave_same_report_ignore_interval(net);
							 zwscn_message_log_debug("ALARM CC za_event[%d]=%d report_value=%d event_match_time=%llu time_diff=%llu expected_diff=%u\n", i, device_args->za_event[i], report->value.ex_event, event_match_time, time_diff, zwscn_zwave_same_report_ignore_interval); 
							 if((event_match_time == 0) || (time_diff >= zwscn_zwave_same_report_ignore_interval)) { 
								 device_args->za_event_last_match = i;
								 device_args->za_event_last_matched_time[i] = ZWSCN_TIME_NOW();
								 result_2 = ZWSCN_TRUE;
								 break;
							 }
						 }
					}
				}
			}
		}
	}
	
	if((device_args->va_type != 0) && (device_args->za_type == 0)) { 			//only Vendor alarm is valid 
		result = result_1;
	} else if((device_args->va_type == 0) && (device_args->za_type != 0)) {	//only Z-Wave alarm is valid 
		result = result_2;
	} else if((device_args->va_type != 0) && (device_args->za_type != 0)) {	//both Vendor and Z-Wave alarm are valid
		result = (result_1 || result_2); //Change the logical operator (&& or ||) here as per requirements
	}
	zwscn_message_log_debug("ALARM CC result_1=%d, result_2=%d, result=%d\n", result_1, result_2, result);
	
	return result;
}

ZWSCN_BOOL_T zwscn_zwave_compare_cc_basic_event(zwscn_args_basic_event_t *device_args, zwscn_report_basic_event_t *report) {
	int32_t value_int_target_value_low, value_int_target_value_high, value_int_current;
	uint8_t result_new_value = ZWSCN_FALSE;
//	uint8_t result_last_value = ZWSCN_FALSE;
	ZWSCN_BOOL_T result = ZWSCN_FALSE;

	value_int_target_value_low = device_args->value_low;
	value_int_target_value_high = device_args->value_high;
	value_int_current = report->value;
	zwscn_message_log_debug("BASIC EVENT low=%d high=%d current=%d\n", value_int_target_value_low, value_int_target_value_high, value_int_current);
	
	result_new_value = zwscn_is_value_in_range(value_int_target_value_low, value_int_target_value_high, value_int_current);
	zwscn_message_log_debug("BASIC EVENT is_value_in_range result=%d NEW\n", result_new_value);
	
#if ZWSCN_ZWAVE_BASIC_EVENT_TRIGGER_IN_RANGE
	result = result_new_value;
#else
	if(device_args->flag_first_time_check_done == ZWSCN_FALSE) { //It is the first time
		zwscn_message_log_debug("BASIC EVENT first time received a report for this event\n");
		result = result_new_value;
		device_args->flag_first_time_check_done = ZWSCN_TRUE;
	} else { //not the first time
		result_last_value = zwscn_is_value_in_range(value_int_target_value_low, value_int_target_value_high, device_args->value_last);
		zwscn_message_log_debug("BASIC EVENT is_value_in_range result=%d LAST\n", result_last_value);
		if((result_last_value == ZWSCN_FALSE) && (result_new_value == ZWSCN_TRUE)) { //Just entering the range after going out
			zwscn_message_log_debug("BASIC EVENT entered valid range again\n");
			result = ZWSCN_TRUE;
		}
	}
#endif

	device_args->value_last = value_int_current;
	
	return result;
//l_err:
//	return ZWSCN_FALSE;
}

ZWSCN_BOOL_T zwscn_zwave_compare_cc_central_scene(zwscn_args_central_scene_t *device_args, zwscn_report_central_scene_t *report) {
	ZWSCN_BOOL_T result = ZWSCN_FALSE;
	
	zwscn_message_log_debug("CENTRAL_SCENE target key_number=%d current key_number=%d\n", device_args->central_scene_key_number, report->central_scene_key_number);
	zwscn_message_log_debug("CENTRAL_SCENE target key_attribute=%d current key_attribute=%d\n", device_args->central_scene_key_attribute, report->central_scene_key_attribute);
	
	if((device_args->central_scene_key_number == report->central_scene_key_number) && 
	(device_args->central_scene_key_attribute == report->central_scene_key_attribute)) {
		result = ZWSCN_TRUE;
	} else {
		result = ZWSCN_FALSE;
	}
	
	return result;
}

const char *zwscn_zwave_get_cc_string(uint16_t cls_id) {
	int i = 0;
	int count = 0;
	int found = 0;
	const char *result = NULL;
	
	count = sizeof(zwscn_zwave_cc_for_alarm_security_event);
	for(i=0; i < count; ++i) {
		if(zwscn_zwave_cc_for_alarm_security_event[i] == cls_id) {
			found = 1;
			break;
		}
	}
	
	if(found == 1) {
		result = zwscn_zwave_cc_for_alarm_security_event_string[i];
	}
	
	return result;
}

ZWSCN_STATUS_T zwscn_zwave_get_cc_args_string(uint16_t cls_id, void *device_args, char *output_buf, size_t output_buf_size) {
	if((output_buf == NULL) || (output_buf_size == 0)) {
		goto l_err_args;
	}
	
	switch(cls_id) {
		case COMMAND_CLASS_BASIC: //incoming BASIC SET is used as EVENT
		{
			zwscn_util_strcpy(output_buf, "[Event received]", output_buf_size);
		}
		break;
		case COMMAND_CLASS_SENSOR_BINARY:
		{
			zwscn_args_sensor_binary_t *dev_args = (zwscn_args_sensor_binary_t *) device_args;
			
			if(dev_args) {
				if(dev_args->type == 0) {
					snprintf(output_buf, output_buf_size, "[Event detected]");
				} else {
					snprintf(output_buf, output_buf_size, "[Event detected][%s]", zwscn_zwave_get_cc_sensor_binary_string(dev_args->type));
				}
			}
		}
		break;
		case COMMAND_CLASS_ALARM:
		{
			zwscn_args_alarm_t *dev_args = (zwscn_args_alarm_t *) device_args;
			
			if(dev_args) {
				if(dev_args->za_type < sizeof(zwscn_zwave_cc_alarm_type)) {
					if(dev_args->za_event == 0) {
						snprintf(output_buf, output_buf_size, "[%s]", zwscn_zwave_cc_alarm_type[dev_args->za_type]);
					} else {
						snprintf(output_buf, output_buf_size, "[%s][%s]", zwscn_zwave_cc_alarm_type[dev_args->za_type], zwscn_zwave_get_cc_alarm_string(dev_args->za_type, dev_args->za_event[dev_args->za_event_last_match])); //TODO print exactly matched event
					}
				}
			}
		}
		break;
	}
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
}

ZWSCN_STATUS_T zwscn_copy_device_arg_2_lt(zwscn_event_p src_event, zwscn_event_p dest_event)
{
	ZWSCN_STATUS_T ret = ZWSCN_ERR_NONE;
	uint16_t cls_id;

	if ((src_event == NULL) || (dest_event == NULL)) {
		ret = ZWSCN_ERR_INVALID_ARGS;
		goto l_err_args;
	}

	if (!src_event->device_args)	//Nothing to copy
		goto l_err_none;

	//Copy the entire device_args over first
	dest_event->device_args = calloc(1, dest_event->device_args_size);
	if (!dest_event->device_args) {
		ret = ZWSCN_ERR_MEMORY;
		goto l_err_memory;
	}
	memcpy(dest_event->device_args, src_event->device_args, src_event->device_args_size);
	dest_event->device_args_size = src_event->device_args_size;

	cls_id = zwscn_util_combo_id_to_cls_id(src_event->id);
	switch (cls_id) 
	{
		case COMMAND_CLASS_ALARM:
		{
			zwscn_args_alarm_t *dev_args = (zwscn_args_alarm_t *)src_event->device_args;
			zwscn_args_alarm_t *dest_dev_args = (zwscn_args_alarm_t *)dest_event->device_args;
			uint8_t za_event_last_match_value;
			zwscn_time_t za_event_last_matched_time_single;

			if (dev_args->za_event_count > 1)
			{
				za_event_last_match_value = dev_args->za_event[dev_args->za_event_last_match];
				za_event_last_matched_time_single = dev_args->za_event_last_matched_time[dev_args->za_event_last_match];

				memset(dest_dev_args->za_event, 0, sizeof(dest_dev_args->za_event));
				memset(dest_dev_args->za_event_last_matched_time, 0, sizeof(dest_dev_args->za_event_last_matched_time));

				dest_dev_args->za_event_count = 1;
				dest_dev_args->za_event[0] = za_event_last_match_value;

				dest_dev_args->za_event_last_match = 0;

				dest_dev_args->za_event_last_matched_time[0] = za_event_last_matched_time_single;
			}						
		}
		break;
	}

l_err_none:
	return ret;

l_err_memory:
l_err_args:
	return ret;
}


const char *zwscn_zwave_get_cc_sensor_binary_string(uint8_t type) {
	switch(type) {
		case 1: return "General Purpose";
		break;
		case 2: return "Smoke";
		break;
		case 3: return "CO";
		break;
		case 4: return "CO2";
		break;
		case 5: return "Heat";
		break;
		case 6: return "Water";
		break;
		case 7: return "Freeze";
		break;
		case 8: return "Tamper";
		break;
		case 9: return "Aux";
		break;
		case 10: return "Door/Window";
		break;
		case 11: return "Tilt";
		break;
		case 12: return "Motion";
		break;
		case 13: return "Glass Break";
		break;
	}
	
	return "Unknown";
}

const char *zwscn_zwave_get_cc_alarm_string(uint8_t alarm_type, uint8_t event_type) {
	switch(alarm_type) {
		case 1:	/*Smoke Alarm*/
			switch(event_type) {
				case 0x00: return "Event inactive (push mode)/Previous Events cleared (pull mode)"; 	
				break; 
				case 0x01: return "Smoke detected";
				break;
				case 0x02: return "Smoke detected, Unknown Location"; 
				break; 
				case 0x03: return "Smoke Alarm Test"; 
				break; 
				case 0x04: return "Replacement Required"; 
				break;
				case 0xFE: return "Unknown Event";
				break;
			 }
		break;
		case 2:	/*CO Alarm*/
			switch(event_type) {
				case 0x00: return "Event inactive (push mode)/Previous Events cleared (pull mode)"; 
				break; 	 
				case 0x01: return "Carbon monoxide detected"; 
				break; 
				case 0x02: return "Carbon monoxide detected, Unknown Location"; 
				break; 
				case 0x03: return "Carbon monoxide Test"; 
				break; 
				case 0x04: return "Replacement Required"; 
				break;
				case 0xFE: return "Unknown Event";
				break;
			 }
		break;
		case 3:	/*CO2 Alarm*/ 
			switch(event_type) {
				case 0x00: return "Event inactive (push mode)/Previous Events cleared (pull mode)"; 
				break; 	
				case 0x01: return "Carbon dioxide detected"; 
				break; 
				case 0x02: return "Carbon dioxide detected, Unknown Location"; 
				break; 
				case 0x03: return "Carbon dioxide Test"; 
				break; 
				case 0x04: return "Replacement Required"; 
				break;
				case 0xFE: return "Unknown Event";
				break;
			}
		break;
		case 4:	/*Heat Alarm*/
			switch(event_type) {
				case 0x00: return "Event inactive (push mode)/Previous Events cleared (pull mode)"; 
				break; 	 
				case 0x01: return "Overheat detected"; 
				break; 
				case 0x02: return "Overheat detected, Unknown Location"; 
				break; 
				case 0x03: return "Rapid Temperature Rise"; 
				break; 
				case 0x04: return "Rapid Temperature Rise, Unknown Location"; 
				break; 
				case 0x05: return "Underheat detected"; 
				break; 
				case 0x06: return "Underheat detected, Unknown Location"; 
				break;
				case 0xFE: return "Unknown Event";
				break;
			}
		break;
		case 5:	/*Water Alarm*/
			switch(event_type) {
				case 0x00: return "Event inactive (push mode)/Previous Events cleared (pull mode)"; 
				break; 	
				case 0x01: return "Water Leak detected"; 
				break; 
				case 0x02: return "Water Leak detected, Unknown Location"; 
				break; 
				case 0x03: return "Water Level dropped"; 
				break; 
				case 0x04: return "Water Level dropped, Unknown Location"; 
				break; 
				case 0x05: return "Replace Water Filter"; 
				break;
				case 0xFE: return "Unknown Event";
				break;
			}
		break;
		case 6:	/*Access Control Alarm*/
			switch(event_type) {
				case 0x00: return "Event inactive (push mode)/Previous Events cleared (pull mode)"; 
				break; 	 
				case 0x01: return "Manual Lock Operation"; 
				break; 
				case 0x02: return "Manual Unlock Operation"; 
				break; 
				case 0x03: return "RF Lock Operation"; 
				break; 
				case 0x04: return "RF Unlock Operation"; 
				break; 
				case 0x05: return "Keypad Lock Operation"; 
				break; 
				case 0x06: return "Keypad Unlock Operation"; 
				break; 
				case 0x07: return "Manual Not Fully Locked Operation"; 
				break; 
				case 0x08: return "RF Not Fully Locked Operation"; 
				break; 
				case 0x09: return "Auto Lock Locked Operation"; 
				break; 
				case 0x0A: return "Auto Lock Not Fully Operation"; 
				break; 
				case 0x0B: return "Lock Jammed"; 
				break; 
				case 0x0C: return "All user codes deleted"; 
				break; 
				case 0x0D: return "Single user code deleted"; 
				break; 
				case 0x0E: return "New user code added"; 
				break;
				case 0x0F: return "New user code not added due to duplicate code"; 
				break; 
				case 0x10: return "Keypad temporary disabled"; 
				break; 
				case 0x11: return "NewKeypad busy"; 
				break; 
				case 0x12: return "New Program code Entered - Unique code for lock configuration"; 
				break; 
				case 0x13: return "Manually Enter user Access code exceeds code limit"; 
				break; 
				case 0x14: return "Unlock By RF with invalid user code"; 
				break; 
				case 0x15: return "Locked by RF with invalid user codes"; 
				break; 
				case 0x16: return "Window/Door is open"; 
				break; 
				case 0x17: return "Window/Door is closed"; 
				break;
				case 0x40: return "Barrier performing Initialization process"; 
				break;
				case 0x41: return "Barrier operation (Open / Close) force has been exceeded."; 
				break;
				case 0x42: return "Barrier motor has exceeded manufacturer’s operational time limit"; 
				break;
				case 0x43: return "Barrier operation has exceeded physical mechanical limits. (For example: barrier has opened past the open limit)"; 
				break;
				case 0x44: return "Barrier unable to perform requested operation due to UL requirements"; 
				break;
				case 0x45: return "Barrier Unattended operation has been disabled per UL requirements"; 
				break;
				case 0x46: return "Barrier failed to perform Requested operation, device malfunction"; 
				break;
				case 0x47: return "Barrier Vacation Mode"; 
				break;
				case 0x48: return "Barrier Safety Beam Obstacle"; 
				break;
				case 0x49: return "Barrier Sensor Not Detected / Supervisory Error"; 
				break;
				case 0x4A: return "Barrier Sensor Low Battery Warning"; 
				break;
				case 0x4B: return "Barrier detected short in Wall Station wires"; 
				break;
				case 0x4C: return "Barrier associated with non-Z-wave remote control"; 
				break;
				case 0xFE: return "Unknown Event";
				break;
			}
		break;
		case 7:	/*Burgler Alarm*/
			switch(event_type) {
				case 0x00: return "Event inactive (push mode)/Previous Events cleared (pull mode)"; 
				break; 	 
				case 0x01: return "Intrusion detected"; 
				break; 
				case 0x02: return "Intrusion detected, Unknown Location"; 
				break; 
				case 0x03: return "Tampering, product covering removed"; 
				break; 
				case 0x04: return "Tampering, Invalid Code"; 
				break; 
				case 0x05: return "Glass Breakage"; 
				break; 
				case 0x06: return "Glass Breakage, Unknown Location"; 
				break;
				case 0x07: return "Motion Detection"; 
				break;
				case 0x08: return "Motion Detection, Unknown Location"; 
				break;
				case 0xFE: return "Unknown Event";
				break;
			}
		break;
		case 8:	/*Power Management Alarm*/
			switch(event_type) {
				case 0x00: return "Event inactive (push mode)/Previous Events cleared (pull mode)"; 
				break; 	 
				case 0x01: return "Power has been applied"; 
				break; 
				case 0x02: return "AC mains disconnected"; 
				break; 
				case 0x03: return "AC mains re-connected"; 
				break; 
				case 0x04: return "Surge Detection"; 
				break; 
				case 0x05: return "Voltage Drop/Drift"; 
				break;
				case 0x06: return "Over-current detected"; 
				break;
				case 0x07: return "Over-voltage detected"; 
				break;
				case 0x08: return "Over-load detected"; 
				break;
				case 0x09: return "Load error"; 
				break;
				case 0x0A: return "Replace battery soon"; 
				break;
				case 0x0B: return "Replace battery now"; 
				break;
				case 0x0C: return "Battery is charging"; 
				break;
				case 0x0D: return "Battery is fully charged"; 
				break;
				case 0x0E: return "Charge battery soon"; 
				break;
				case 0x0F: return "Charge battery now!"; 
				break;
				case 0xFE: return "Unknown Event";
				break;
			}
		break;
		case 9:	/*System Alarm*/
			switch(event_type) {
				case 0x00: return "Event inactive (push mode)/Previous Events cleared (pull mode)"; 
				break; 	 
				case 0x01: return "System hardware failure"; 
				break; 
				case 0x02: return "System software failure"; 
				break;
				case 0x03: return "System hardware failure with manufacturer proprietary failure code"; 
				break;
				case 0x04: return "System software failure with manufacturer proprietary failure code"; 
				break;
				case 0xFE: return "Unknown Event";
				break;
			}
			 
		break;
		case 10:	/*Emergency Alarm*/
			switch(event_type) {
				case 0x00: return "Event inactive (push mode)/Previous Events cleared (pull mode)"; 
				break; 	 
				case 0x01: return "Contact Police"; 
				break; 
				case 0x02: return "Contact Fire Service"; 
				break; 
				case 0x03: return "Contact Medical Service"; 
				break;
				case 0xFE: return "Unknown Event";
				break;
			}
		break;
		case 11:	/*Alarm Clock*/
			switch(event_type) {
				case 0x00: return "Event inactive (push mode)/Previous Events cleared (pull mode)"; 
				break; 	 
				case 0x01: return "Wake Up Alert"; 
				break;
				case 0x02: return "Timer Ended"; 
				break;
				case 0x03: return "Time remaining"; 
				break;
				case 0xFE: return "Unknown Event";
				break;
			}
		break;
		case 12:	/*Appliance*/
			switch(event_type) {
				case 0x00: return "Event inactive (push mode)/Previous Events cleared (pull mode)"; 
				break; 	 
				case 0x01: return "Program started"; 
				break;
				case 0x02: return "Program in progress"; 
				break;
				case 0x03: return "Program completed"; 
				break;
				case 0x04: return "Replace main filter"; 
				break;
				case 0x05: return "Failure to set target temperature"; 
				break;
				case 0x06: return "Supplying water"; 
				break;
				case 0x07: return "Water supply failure"; 
				break;
				case 0x08: return "Boiling"; 
				break;
				case 0x09: return "Boiling failure"; 
				break;
				case 0x0A: return "Washing"; 
				break;
				case 0x0B: return "Washing failure"; 
				break;
				case 0x0C: return "Rinsing"; 
				break;
				case 0x0D: return "Rinsing failure"; 
				break;
				case 0x0E: return "Draining"; 
				break;
				case 0x0F: return "Draining failure"; 
				break;
				case 0x10: return "Spinning"; 
				break;
				case 0x11: return "Spinning failure"; 
				break;
				case 0x12: return "Drying"; 
				break;
				case 0x13: return "Drying failure"; 
				break;
				case 0x14: return "Fan failure"; 
				break;
				case 0x15: return "Compressor failure"; 
				break;
				case 0xFE: return "Unknown Event";
				break;
			}
		break;
		case 13:	/*Home Health Alarm*/
			switch(event_type) {
				case 0x00: return "Event inactive (push mode)/Previous Events cleared (pull mode)"; 
				break; 	 
				case 0x01: return "Leaving Bed"; 
				break; 
				case 0x02: return "Sitting on bed"; 
				break; 
				case 0x03: return "Lying on bed"; 
				break; 
				case 0x04: return "Posture changed"; 
				break; 
				case 0x05: return "Sitting on edge of bed"; 
				break; 
				case 0x06: return "Volatile Organic Compound level"; 
				break;
				case 0xFE: return "Unknown Event";
				break;
			}
		break;
	}
	
	return "Unknown";
}
