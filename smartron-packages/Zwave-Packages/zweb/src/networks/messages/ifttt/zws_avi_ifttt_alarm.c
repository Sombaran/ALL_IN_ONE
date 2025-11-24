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
 * @file    zws_avi_ifttt_alarm.c
 *
 * @brief   AVI message 'ifttt_alarm'
 * @details Handles the AVI message 'ifttt_alarm'.
 *
 * @author  Angela
 *
 * @version 1.0 - 2016-12-29
 * - Initial version
 */

#include "zws_status.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zws_ifttt_common.h"
#include "zws_ifttt_response.h"
#include "zws_avi_ifttt_common.h"
#include "zws_avi_message_ifttt.h"

 /** Alarm "category" definition */
#define ZW_ALARM_CATEGORY_MOTION            1       /**< Motion alarm detected/clear */
#define ZW_ALARM_CATEGORY_SMOKE				2		/**< Smoke alarm detected/clear */
#define ZW_ALARM_CATEGORY_CO				3       /**< CO alarm detected/clear */
#define ZW_ALARM_CATEGORY_CO2				4		/**< CO2 alarm detected/clear */
#define ZW_ALARM_CATEGORY_WATER				5		/**< Water alarm detected/clear */
#define ZW_ALARM_CATEGORY_CONTACT           6       /**< Contact alarm detected/clear */


/**
* @brief Set Avro value for RPC request for alarm option command using CGI request arguments
* @param[in]		stifttt_uri       IFTTT URI request
* @param[in]		stifttt_args      IFTTT request parameters
* @param[in, out]   arg_value_p       Avro value for RPC request
* @retval			ZWS_STATUS_OK     Success
* @retval			ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_network_ifttt_alarm_option_argument(ifttt_uri_t stifttt_uri, ifttt_args_t stifttt_args, avro_value_t *arg_value_p)
{
	int32_t iDevice_name;
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_PARAM;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);


	if ((stifttt_uri.eRequest_IF_feature == eTrigger_motionAlarm_Detected )
		|| (stifttt_uri.eRequest_IF_feature == eTrigger_motionAlarm_Clear))
	{
		res = zw_avro_field_set_int(arg_value_p, "category", ZW_ALARM_CATEGORY_MOTION);
	}
	else if ((stifttt_uri.eRequest_IF_feature == eTrigger_smokeAlarm_Detected)
		|| (stifttt_uri.eRequest_IF_feature == eTrigger_smokeAlarm_Clear))
	{
		res = zw_avro_field_set_int(arg_value_p, "category", ZW_ALARM_CATEGORY_SMOKE);
	}
	else if ((stifttt_uri.eRequest_IF_feature == eTrigger_coAlarm_Detected)
		|| (stifttt_uri.eRequest_IF_feature == eTrigger_coAlarm_Clear))
	{
		res = zw_avro_field_set_int(arg_value_p, "category", ZW_ALARM_CATEGORY_CO);
	}
	else if ((stifttt_uri.eRequest_IF_feature == eTrigger_co2Alarm_Detected)
		|| (stifttt_uri.eRequest_IF_feature == eTrigger_co2Alarm_Clear))
	{
		res = zw_avro_field_set_int(arg_value_p, "category", ZW_ALARM_CATEGORY_CO2);
	}
	else if ((stifttt_uri.eRequest_IF_feature == eTrigger_waterAlarm_Detected)
		|| (stifttt_uri.eRequest_IF_feature == eTrigger_waterAlarm_Clear))
	{
		res = zw_avro_field_set_int(arg_value_p, "category", ZW_ALARM_CATEGORY_WATER);
	}
	else if ((stifttt_uri.eRequest_IF_feature == eTrigger_contactAlarm_Open)
		|| (stifttt_uri.eRequest_IF_feature == eTrigger_contactAlarm_Closed))
	{
		res = zw_avro_field_set_int(arg_value_p, "category", ZW_ALARM_CATEGORY_CONTACT);
	}
	else
	{
		ZWS_LOG(ZWS_LOG_ERR, "Uncognized eRequest_IF_feature %d", stifttt_uri.eRequest_IF_feature);
	}

	if (res == ZWS_STATUS_OK)
	{
		if (stifttt_uri.eRequest_IF_field == eDevice_name)
		{
			iDevice_name = (int32_t)eDevice_name;
			res = zw_avro_field_set_int(arg_value_p, "efield", iDevice_name);

			if (res != ZWS_STATUS_OK)
			{
				goto l_err_set_avro_field;
			}
		}
		else
		{
			ZWS_LOG(ZWS_LOG_ERR, "eRequest_IF_field %d not supported for alarm_option", stifttt_uri.eRequest_IF_field);
			res = ZWS_STATUS_IFTTT_ERROR_SERVER;
			goto l_err_invalid_field;
		}

		res = ZWS_STATUS_OK;
	}

l_err_invalid_field:
l_err_set_avro_field:
	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);
	return res;
}


/**
* @brief Set Avro value for RPC request for alarm command using CGI request arguments
* @param[in]		stifttt_uri       IFTTT URI request
* @param[in]		stifttt_args      IFTTT request parameters
* @param[in, out]   arg_value_p       Avro value for RPC request
* @retval			ZWS_STATUS_OK     Success
* @retval			ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_network_ifttt_alarm_get_argument(ifttt_uri_t stifttt_uri, ifttt_args_t stifttt_args, avro_value_t *arg_value_p)
{
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_SERVER;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);

	res = zw_avro_field_set_int(arg_value_p, "itrigger", (int)stifttt_uri.eRequest_IF_feature);

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);
	return res;
}

/** Handlers for motionalarm_detected option */
static zws_avi_message_ifttt_t ifttt_motionalarm_detected_option =
{
	"ifttt_motionalarm_detected", 
	
	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_motionAlarm_Detected,

	{IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_name},

	"ifttt_network_alarm_option",

	zws_avi_network_ifttt_alarm_option_argument,
	zws_avi_network_ifttt_device_name_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for motionalarm_detected */
static zws_avi_message_ifttt_t ifttt_motionalarm_detected =
{
	"ifttt_motionalarm_detected",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_motionAlarm_Detected,

	{IFTTT_REQUEST_SUBTYPE_NONE},

	"ifttt_interface_alarm_get",

	zws_avi_network_ifttt_alarm_get_argument,
	zws_avi_network_ifttt_trigger_get_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for motionalarm_clear option */
static zws_avi_message_ifttt_t ifttt_motionalarm_clear_option =
{
	"ifttt_motionalarm_clear",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_motionAlarm_Clear,

	{IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_name},

	"ifttt_network_alarm_option",

	zws_avi_network_ifttt_alarm_option_argument,
	zws_avi_network_ifttt_device_name_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for motionalarm_clear */
static zws_avi_message_ifttt_t ifttt_motionalarm_clear =
{
	"ifttt_motionalarm_clear",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_motionAlarm_Clear,

	{IFTTT_REQUEST_SUBTYPE_NONE},

	"ifttt_interface_alarm_get",

	zws_avi_network_ifttt_alarm_get_argument,
	zws_avi_network_ifttt_trigger_get_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for smokealarm_detected option */
static zws_avi_message_ifttt_t ifttt_smokealarm_detected_option =
{
	"ifttt_smokealarm_detected",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_smokeAlarm_Detected,

	{ IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_name },

	"ifttt_network_alarm_option",

	zws_avi_network_ifttt_alarm_option_argument,
	zws_avi_network_ifttt_device_name_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for smokealarm_detected */
static zws_avi_message_ifttt_t ifttt_smokealarm_detected =
{
	"ifttt_smokealarm_detected",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_smokeAlarm_Detected,

	{ IFTTT_REQUEST_SUBTYPE_NONE },

	"ifttt_interface_alarm_get",

	zws_avi_network_ifttt_alarm_get_argument,
	zws_avi_network_ifttt_trigger_get_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for smokealarm_clear option */
static zws_avi_message_ifttt_t ifttt_smokealarm_clear_option =
{
	"ifttt_smokealarm_clear",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_smokeAlarm_Clear,

	{ IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_name },

	"ifttt_network_alarm_option",

	zws_avi_network_ifttt_alarm_option_argument,
	zws_avi_network_ifttt_device_name_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for smokealarm_clear */
static zws_avi_message_ifttt_t ifttt_smokealarm_clear =
{
	"ifttt_smokealarm_clear",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_smokeAlarm_Clear,

	{ IFTTT_REQUEST_SUBTYPE_NONE },

	"ifttt_interface_alarm_get",

	zws_avi_network_ifttt_alarm_get_argument,
	zws_avi_network_ifttt_trigger_get_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for coalarm_detected option */
static zws_avi_message_ifttt_t ifttt_coalarm_detected_option =
{
	"ifttt_coalarm_detected",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_coAlarm_Detected,

	{ IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_name },

	"ifttt_network_alarm_option",

	zws_avi_network_ifttt_alarm_option_argument,
	zws_avi_network_ifttt_device_name_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for coalarm_detected */
static zws_avi_message_ifttt_t ifttt_coalarm_detected =
{
	"ifttt_coalarm_detected",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_coAlarm_Detected,

	{ IFTTT_REQUEST_SUBTYPE_NONE },

	"ifttt_interface_alarm_get",

	zws_avi_network_ifttt_alarm_get_argument,
	zws_avi_network_ifttt_trigger_get_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for coalarm_clear option */
static zws_avi_message_ifttt_t ifttt_coalarm_clear_option =
{
	"ifttt_coalarm_clear",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_coAlarm_Clear,

	{ IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_name },

	"ifttt_network_alarm_option",

	zws_avi_network_ifttt_alarm_option_argument,
	zws_avi_network_ifttt_device_name_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for coalarm_clear */
static zws_avi_message_ifttt_t ifttt_coalarm_clear =
{
	"ifttt_coalarm_clear",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_coAlarm_Clear,

	{ IFTTT_REQUEST_SUBTYPE_NONE },

	"ifttt_interface_alarm_get",

	zws_avi_network_ifttt_alarm_get_argument,
	zws_avi_network_ifttt_trigger_get_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for co2alarm_detected option */
static zws_avi_message_ifttt_t ifttt_co2alarm_detected_option =
{
	"ifttt_co2alarm_detected",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_co2Alarm_Detected,

	{ IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_name },

	"ifttt_network_alarm_option",

	zws_avi_network_ifttt_alarm_option_argument,
	zws_avi_network_ifttt_device_name_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for co2alarm_detected */
static zws_avi_message_ifttt_t ifttt_co2alarm_detected =
{
	"ifttt_co2alarm_detected",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_co2Alarm_Detected,

	{ IFTTT_REQUEST_SUBTYPE_NONE },

	"ifttt_interface_alarm_get",

	zws_avi_network_ifttt_alarm_get_argument,
	zws_avi_network_ifttt_trigger_get_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for co2alarm_clear option */
static zws_avi_message_ifttt_t ifttt_co2alarm_clear_option =
{
	"ifttt_co2alarm_clear",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_co2Alarm_Clear,

	{ IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_name },

	"ifttt_network_alarm_option",

	zws_avi_network_ifttt_alarm_option_argument,
	zws_avi_network_ifttt_device_name_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for co2alarm_clear */
static zws_avi_message_ifttt_t ifttt_co2alarm_clear =
{
	"ifttt_co2alarm_clear",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_co2Alarm_Clear,

	{ IFTTT_REQUEST_SUBTYPE_NONE },

	"ifttt_interface_alarm_get",

	zws_avi_network_ifttt_alarm_get_argument,
	zws_avi_network_ifttt_trigger_get_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for wateralarm_detected option */
static zws_avi_message_ifttt_t ifttt_wateralarm_detected_option =
{
	"ifttt_wateralarm_detected",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_waterAlarm_Detected,

	{ IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_name },

	"ifttt_network_alarm_option",

	zws_avi_network_ifttt_alarm_option_argument,
	zws_avi_network_ifttt_device_name_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for wateralarm_detected */
static zws_avi_message_ifttt_t ifttt_wateralarm_detected =
{
	"ifttt_wateralarm_detected",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_waterAlarm_Detected,

	{ IFTTT_REQUEST_SUBTYPE_NONE },

	"ifttt_interface_alarm_get",

	zws_avi_network_ifttt_alarm_get_argument,
	zws_avi_network_ifttt_trigger_get_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for wateralarm_clear option */
static zws_avi_message_ifttt_t ifttt_wateralarm_clear_option =
{
	"ifttt_wateralarm_clear",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_waterAlarm_Clear,

	{ IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_name },

	"ifttt_network_alarm_option",

	zws_avi_network_ifttt_alarm_option_argument,
	zws_avi_network_ifttt_device_name_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for wateralarm_clear */
static zws_avi_message_ifttt_t ifttt_wateralarm_clear =
{
	"ifttt_wateralarm_clear",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_waterAlarm_Clear,

	{ IFTTT_REQUEST_SUBTYPE_NONE },

	"ifttt_interface_alarm_get",

	zws_avi_network_ifttt_alarm_get_argument,
	zws_avi_network_ifttt_trigger_get_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for contactalarm_open option */
static zws_avi_message_ifttt_t ifttt_contactalarm_open_option =
{
	"ifttt_contactalarm_open",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_contactAlarm_Open,

	{ IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_name },

	"ifttt_network_alarm_option",

	zws_avi_network_ifttt_alarm_option_argument,
	zws_avi_network_ifttt_device_name_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for contactalarm_open */
static zws_avi_message_ifttt_t ifttt_contactalarm_open =
{
	"ifttt_contactalarm_open",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_contactAlarm_Open,

	{ IFTTT_REQUEST_SUBTYPE_NONE },

	"ifttt_interface_alarm_get",

	zws_avi_network_ifttt_alarm_get_argument,
	zws_avi_network_ifttt_trigger_get_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for contactalarm_closed option */
static zws_avi_message_ifttt_t ifttt_contactalarm_closed_option =
{
	"ifttt_contactalarm_closed",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_contactAlarm_Closed,

	{ IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_name },

	"ifttt_network_alarm_option",

	zws_avi_network_ifttt_alarm_option_argument,
	zws_avi_network_ifttt_device_name_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for contactalarm_closed */
static zws_avi_message_ifttt_t ifttt_contactalarm_closed =
{
	"ifttt_contactalarm_closed",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_contactAlarm_Closed,

	{ IFTTT_REQUEST_SUBTYPE_NONE },

	"ifttt_interface_alarm_get",

	zws_avi_network_ifttt_alarm_get_argument,
	zws_avi_network_ifttt_trigger_get_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Register command handlers */
zws_status_t zws_avi_ifttt_alarm_init(zwu_array_list_t message_list)
{
	if ((zws_avi_message_list_add_message_ifttt(message_list, &ifttt_motionalarm_detected_option) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_motionalarm_detected) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_motionalarm_clear_option) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_motionalarm_clear) != ZWS_STATUS_OK) ||

		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_smokealarm_detected_option) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_smokealarm_detected) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_smokealarm_clear_option) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_smokealarm_clear) != ZWS_STATUS_OK) ||

		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_coalarm_detected_option) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_coalarm_detected) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_coalarm_clear_option) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_coalarm_clear) != ZWS_STATUS_OK) ||

		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_co2alarm_detected_option) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_co2alarm_detected) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_co2alarm_clear_option) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_co2alarm_clear) != ZWS_STATUS_OK) ||

		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_wateralarm_detected_option) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_wateralarm_detected) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_wateralarm_clear_option) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_wateralarm_clear) != ZWS_STATUS_OK) ||

		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_contactalarm_open_option) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_contactalarm_open) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_contactalarm_closed_option) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_contactalarm_closed) != ZWS_STATUS_OK)
		)
	{
		goto l_err_ifttt_status;
	}

	return ZWS_STATUS_OK;

l_err_ifttt_status:
	return ZWS_STATUS_ERROR;
}



