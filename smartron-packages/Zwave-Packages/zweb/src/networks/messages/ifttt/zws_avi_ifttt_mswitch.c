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
 * @file    zws_avi_ifttt_mswitch.c
 *
 * @brief   AVI message 'ifttt_mswitch'
 * @details Handles the AVI message 'ifttt_mswitch'.
 *
 * @author  Angela
 *
 * @version 1.0 - 2016-12-15
 * - Initial version
 */

#include "zws_status.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zwu_number.h"
#include "zws_ifttt_common.h"
#include "zws_ifttt_response.h"
#include "zws_avi_ifttt_common.h"
#include "zws_avi_message_ifttt.h"



#define IFTTT_MSWITCH_TYPE_DIMMER	1

/**
* @brief Set Avro value for RPC request for mswitch option command using CGI request arguments
* @param[in]		stifttt_uri       IFTTT URI request
* @param[in]		stifttt_args      IFTTT request parameters
* @param[in, out]   arg_value_p       Avro value for RPC request
* @retval			ZWS_STATUS_OK     Success
* @retval			ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_network_ifttt_mswitch_device_name_option_argument(ifttt_uri_t stifttt_uri, ifttt_args_t stifttt_args, avro_value_t *arg_value_p)
{
	int32_t iDevice_name;
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_PARAM;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);

	if (stifttt_uri.eRequest_IF_feature == eAction_mSwitch_Dimmer)
	{
		res = zw_avro_field_set_int(arg_value_p, "type", IFTTT_MSWITCH_TYPE_DIMMER); 
	}

	if (res == ZWS_STATUS_OK)
	{
		iDevice_name = (int32_t)eDevice_name;
		res = zw_avro_field_set_int(arg_value_p, "efield", iDevice_name);

		if (res != ZWS_STATUS_OK)
		{
			goto l_err_set_avro_field;
		}
	}

l_err_set_avro_field:
	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);
	return res;
}

/**
* @brief Get CGI response for IFTTT IF device state options API from Avro value for RPC response
* @param[in]     arg_value         Avro value for RPC response
* @param[in]     padditional_info  Additional information to aid to generate the result
* @param[out]    jsData            CGI response json data
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_avi_network_ifttt_mswitch_device_state_option_result(avro_value_t *arg_value, void *padditional_info, zwu_json_t jsData)
{
	int i;
	char cArState[10] = { 0 }, cArLabel[10] = { 0 };
	zwu_json_t jsStateObj = NULL;
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_SERVER;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);

	//On state
	jsStateObj = zwu_json_create_object();

	if (jsStateObj)
	{
		sprintf(cArState, "%d", 255);
		zwu_json_add_string_to_object(jsStateObj, "label", "On/Enable");
		zwu_json_add_string_to_object(jsStateObj, "value", cArState);

		zwu_json_add_item_to_array(jsData, jsStateObj);
	}

	//Off state
	jsStateObj = zwu_json_create_object();

	if (jsStateObj)
	{
		sprintf(cArState, "%d", 0);
		zwu_json_add_string_to_object(jsStateObj, "label", "Off/Disable");
		zwu_json_add_string_to_object(jsStateObj, "value", cArState);

		zwu_json_add_item_to_array(jsData, jsStateObj);
	}

	//Other states
	for (i = 1; i < 100; i++)
	{
		jsStateObj = zwu_json_create_object();

		if (jsStateObj)
		{
			sprintf(cArLabel, "Level %d", i);
			sprintf(cArState, "%d", i);
			zwu_json_add_string_to_object(jsStateObj, "label", cArLabel);
			zwu_json_add_string_to_object(jsStateObj, "value", cArState);

			zwu_json_add_item_to_array(jsData, jsStateObj);
		}
	}

	res = ZWS_STATUS_OK;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);
	return res;
}


/**
* @brief Set Avro value for RPC request for mswitch command using CGI request arguments
* @param[in]		stifttt_uri       IFTTT URI request
* @param[in]		stifttt_args      IFTTT request parameters
* @param[in, out]   arg_value_p       Avro value for RPC request
* @retval			ZWS_STATUS_OK     Success
* @retval			ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_network_ifttt_mswitch_set_argument(ifttt_uri_t stifttt_uri, ifttt_args_t stifttt_args, avro_value_t *arg_value_p)
{
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_PARAM;
	zwu_json_t jsDeviceState = NULL;
	int32_t iDeviceState = 0;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);

	if (stifttt_args.jsFields_obj)
	{
		jsDeviceState = zwu_json_get_object_item(stifttt_args.jsFields_obj, IFTTT_REQUEST_BODY_IFTTT_DEVICE_STATE);
		if (jsDeviceState && jsDeviceState->type == ZWU_JSON_TYPE_STRING)
		{
			if (zwu_parse_int32_non_negative(jsDeviceState->valuestring, &iDeviceState) != ZWU_STATUS_OK)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Fail to get parse %s to state value.", IFTTT_REQUEST_BODY_IFTTT_DEVICE_STATE);
				goto l_err_parse_device_state_str;
			}

			ZWS_LOG(ZWS_LOG_DEBUG, "state %u found.", iDeviceState);
		}
		else
		{
			//service_common->error_text = IFTTT_ERROR_STR_PARAM_NO_DEVICE_NAME_IN_TRIGGER_ACTION_FIELD;
			ZWS_LOG(ZWS_LOG_ERR, "Fail to get %s in trigger/action field.", IFTTT_REQUEST_BODY_IFTTT_DEVICE_STATE);
			goto l_err_get_device_state;
		}

		res = zw_avro_field_set_int(arg_value_p, "value", iDeviceState);
	}

l_err_get_device_state:
l_err_parse_device_state_str:
	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);
	return res;
}

/** Handlers for mswitch devicename option */
static zws_avi_message_ifttt_t ifttt_mswitch_dimmer_device_name_option =
{
	"ifttt_mswitch_dimmer", 
	
	IFTTT_REQUEST_TYPE_ACTION,
	eAction_mSwitch_Dimmer,

	{IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_name},

	"ifttt_network_mswitch_device_name_option",

	zws_avi_network_ifttt_mswitch_device_name_option_argument,
	zws_avi_network_ifttt_device_name_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for mswitch devicestate option */
static zws_avi_message_ifttt_t ifttt_mswitch_dimmer_device_state_option =
{
	"ifttt_mswitch_dimmer",

	IFTTT_REQUEST_TYPE_ACTION,
	eAction_mSwitch_Dimmer,

	{ IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_state },

	NULL,

	NULL,
	zws_avi_network_ifttt_mswitch_device_state_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for mswitch set */
static zws_avi_message_ifttt_t ifttt_mswitch_dimmer_set =
{
	"ifttt_mswitch_dimmer",

	IFTTT_REQUEST_TYPE_ACTION,
	eAction_mSwitch_Dimmer,

	{IFTTT_REQUEST_SUBTYPE_NONE},

	"ifttt_interface_mswitch_set",

	zws_avi_network_ifttt_mswitch_set_argument,
	zws_avi_network_ifttt_action_set_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};



/** Register command handlers */
zws_status_t zws_avi_ifttt_mswitch_init(zwu_array_list_t message_list)
{
	if ((zws_avi_message_list_add_message_ifttt(message_list, &ifttt_mswitch_dimmer_device_name_option) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_mswitch_dimmer_device_state_option) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_ifttt(message_list, &ifttt_mswitch_dimmer_set) != ZWS_STATUS_OK)
		)
	{
		goto l_err_ifttt_status;
	}

	return ZWS_STATUS_OK;

l_err_ifttt_status:
	return ZWS_STATUS_ERROR;
}



