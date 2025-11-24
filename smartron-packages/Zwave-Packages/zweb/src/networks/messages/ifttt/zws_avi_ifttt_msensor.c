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
 * @file    zws_avi_ifttt_msensor.c
 *
 * @brief   AVI message 'ifttt_msensor'
 * @details Handles the AVI message 'ifttt_msensor'.
 *
 * @author  Angela
 *
 * @version 1.0 - 2017-2-22
 * - Initial version
 */

#include "zws_status.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zwu_number.h"
#include "zwu_time.h"
#include "zws_ifttt_common.h"
#include "zws_ifttt_response.h"
#include "zws_avi_ifttt_common.h"
#include "zws_avi_message_ifttt.h"

 /** Multilevel sensor type definition, quoted from HCAPI */
#define ZW_SENSOR_TYPE_TEMP					0x01    /**< Temperature sensor*/
#define ZW_SENSOR_TYPE_LUM					0x03    /**< Luminance sensor*/
#define ZW_SENSOR_TYPE_HUMD					0x05    /**< Relative humidity sensor*/


#define ZW_SENSOR_COMPARISON_COMMAND_LT		1		/**< Comparison command less than */
#define ZW_SENSOR_COMPARISON_COMMAND_GT		2		/**< Comparison command greater than */

/**
* @brief Set Avro value for RPC request for msensor option command using CGI request arguments
* @param[in]		stifttt_uri       IFTTT URI request
* @param[in]		stifttt_args      IFTTT request parameters
* @param[in, out]   arg_value_p       Avro value for RPC request
* @retval			ZWS_STATUS_OK     Success
* @retval			ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_network_ifttt_msensor_option_argument(ifttt_uri_t stifttt_uri, ifttt_args_t stifttt_args, avro_value_t *arg_value_p)
{
	int32_t iDevice_name;
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_PARAM;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);


	if ((stifttt_uri.eRequest_IF_feature == eTrigger_mSensor_Temp_LT )
		|| (stifttt_uri.eRequest_IF_feature == eTrigger_mSensor_Temp_GT))
	{
		res = zw_avro_field_set_int(arg_value_p, "type", ZW_SENSOR_TYPE_TEMP);
	}
	else if ((stifttt_uri.eRequest_IF_feature == eTrigger_mSensor_Luminance_LT)
		|| (stifttt_uri.eRequest_IF_feature == eTrigger_mSensor_Luminance_GT))
	{
		res = zw_avro_field_set_int(arg_value_p, "type", ZW_SENSOR_TYPE_LUM);
	}
	else if ((stifttt_uri.eRequest_IF_feature == eTrigger_mSensor_Humidity_LT)
		|| (stifttt_uri.eRequest_IF_feature == eTrigger_mSensor_Humidity_GT))
	{
		res = zw_avro_field_set_int(arg_value_p, "type", ZW_SENSOR_TYPE_HUMD);
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
			ZWS_LOG(ZWS_LOG_ERR, "eRequest_IF_field %d not supported for msensor_option", stifttt_uri.eRequest_IF_field);
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
* @brief Set Avro value for RPC request for msensor command using CGI request arguments
* @param[in]		stifttt_uri       IFTTT URI request
* @param[in]		stifttt_args      IFTTT request parameters
* @param[in, out]   arg_value_p       Avro value for RPC request
* @retval			ZWS_STATUS_OK     Success
* @retval			ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_network_ifttt_msensor_get_argument(ifttt_uri_t stifttt_uri, ifttt_args_t stifttt_args, avro_value_t *arg_value_p)
{
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_SERVER;
	zwu_json_t jsThld_value = NULL, jsThld_unit = NULL;
	int32_t ithld_unit = 0, itype, icomp_command;
	double dthld_value = 0.0;


	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);

	//res = zw_avro_field_set_int(arg_value_p, "itrigger", (int)stifttt_uri.eRequest_IF_feature);

	if (stifttt_uri.eRequest_IF_feature == eTrigger_mSensor_Temp_LT)
	{
		itype = ZW_SENSOR_TYPE_TEMP;
		icomp_command = ZW_SENSOR_COMPARISON_COMMAND_LT;
	}
	else if (stifttt_uri.eRequest_IF_feature == eTrigger_mSensor_Temp_GT)
	{
		itype = ZW_SENSOR_TYPE_TEMP;
		icomp_command = ZW_SENSOR_COMPARISON_COMMAND_GT;
	}
	else if (stifttt_uri.eRequest_IF_feature == eTrigger_mSensor_Luminance_LT)
	{
		itype = ZW_SENSOR_TYPE_LUM;
		icomp_command = ZW_SENSOR_COMPARISON_COMMAND_LT;
	}
	else if (stifttt_uri.eRequest_IF_feature == eTrigger_mSensor_Luminance_GT)
	{
		itype = ZW_SENSOR_TYPE_LUM;
		icomp_command = ZW_SENSOR_COMPARISON_COMMAND_GT;
	}
	else if (stifttt_uri.eRequest_IF_feature == eTrigger_mSensor_Humidity_LT)
	{
		itype = ZW_SENSOR_TYPE_HUMD;
		icomp_command = ZW_SENSOR_COMPARISON_COMMAND_LT;
	}
	else if (stifttt_uri.eRequest_IF_feature == eTrigger_mSensor_Humidity_GT)
	{
		itype = ZW_SENSOR_TYPE_HUMD;
		icomp_command = ZW_SENSOR_COMPARISON_COMMAND_GT;
	}
	else
	{
		ZWS_LOG(ZWS_LOG_ERR, "Uncognized eRequest_IF_feature %d", stifttt_uri.eRequest_IF_feature);
		goto l_err_invalid_type;
	}

	if (stifttt_args.jsFields_obj)
	{
		//Get "thld_value"
		jsThld_value = zwu_json_get_object_item(stifttt_args.jsFields_obj, IFTTT_REQUEST_BODY_IFTTT_THLD_VALUE);
		if (jsThld_value && jsThld_value->type == ZWU_JSON_TYPE_STRING)
		{
			if (zwu_parse_double(jsThld_value->valuestring, &dthld_value) != ZWU_STATUS_OK)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Fail to get parse %s to threshold value. String is %s.", 
					IFTTT_REQUEST_BODY_IFTTT_THLD_VALUE, jsThld_value->valuestring);
				res = ZWS_STATUS_IFTTT_ERROR_PARAM;
				goto l_err_parse_thld_value_str;
			}
		}
		else
		{
			//service_common->error_text = IFTTT_ERROR_STR_PARAM_NO_DEVICE_NAME_IN_TRIGGER_ACTION_FIELD;
			ZWS_LOG(ZWS_LOG_ERR, "Fail to get %s in trigger/action field.", IFTTT_REQUEST_BODY_IFTTT_THLD_VALUE);
			goto l_err_get_thld_value;
		}

		//Get "thld_unit"
		jsThld_unit = zwu_json_get_object_item(stifttt_args.jsFields_obj, IFTTT_REQUEST_BODY_IFTTT_THLD_UNIT);
		if (jsThld_unit && jsThld_unit->type == ZWU_JSON_TYPE_STRING)
		{
			if (zwu_parse_int32_non_negative(jsThld_unit->valuestring, &ithld_unit) != ZWU_STATUS_OK)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Fail to get parse %s to threshold unit. String is %s.",
					IFTTT_REQUEST_BODY_IFTTT_THLD_UNIT, jsThld_unit->valuestring);
				res = ZWS_STATUS_IFTTT_ERROR_PARAM;
				goto l_err_parse_thld_unit_str;
			}
		}
		else
		{
			//service_common->error_text = IFTTT_ERROR_STR_PARAM_NO_DEVICE_NAME_IN_TRIGGER_ACTION_FIELD;
			ZWS_LOG(ZWS_LOG_ERR, "Fail to get %s in trigger/action field.", IFTTT_REQUEST_BODY_IFTTT_THLD_UNIT);
			goto l_err_get_thld_unit;
		}

		ZWS_LOG(ZWS_LOG_DEBUG, "type: %d comparision_command: %d thld_value: %lf thld_unit %d found.", 
			itype, icomp_command, dthld_value, ithld_unit);

		if ((zw_avro_field_set_int(arg_value_p, "type", itype) != ZWS_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value_p, "comparision_command", icomp_command) != ZWS_STATUS_OK)
			|| (zw_avro_field_set_string(arg_value_p, "thld_value", jsThld_value->valuestring) != ZWS_STATUS_OK)
			//|| (zw_avro_field_set_double(arg_value_p, "thld_value", dthld_value) != ZWS_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value_p, "thld_unit", ithld_unit) != ZWS_STATUS_OK)
			)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Fail to set avro values");
			goto l_err_avro_set_avro;
		}

		res = ZWS_STATUS_OK;
	}

l_err_avro_set_avro:
l_err_get_thld_unit:
l_err_parse_thld_unit_str:
l_err_get_thld_value:
l_err_parse_thld_value_str:
l_err_invalid_type:
	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);
	return res;
}

/**
* @brief Get CGI response for IFTTT Trigger get API from Avro value for RPC response
* @param[in]     arg_value         Avro value for RPC response
* @param[in]     padditional_info  Additional information to aid to generate the result
* @param[out]    jsData            CGI response json data
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_network_ifttt_msensor_get_result(avro_value_t *arg_value, void *padditional_info, zwu_json_t jsData)
{
	avro_value_t get_result_list_value;
	avro_value_t get_result_value;
	size_t count;
	int64_t utime;
	int i;
	unsigned long *pcombo_id;
	char arIDValue[100] = { 0 }, arDeviceName[150] = { 0 };
	char *pchValue;
	char *utime_iso8601 = NULL;
	char *pchSensor_value, *pchSensor_Unit;
	zwu_json_t jsMatchedRecordObj = NULL;
	zwu_json_t jsMetaObj = NULL;
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_SERVER;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);

	pcombo_id = (unsigned long *)padditional_info;

	if (!pcombo_id || *pcombo_id == 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Invalid combo id");
		goto l_err_invalid_combo_id;
	}

	if ((pchValue = (char *)ZWS_MALLOC(sizeof(int64_t))) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_pchValue_alloc;
	}

	sprintf(pchValue, "%lu", *pcombo_id);

	if (zws_avi_interface_ifttt_get_device_name(*pcombo_id, arg_value, arDeviceName) != ZWS_STATUS_OK)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Fail to generate device name");
		goto l_err_generate_device_name;
	}

	if (avro_value_get_by_name(arg_value, "ifttt_msensor_get_result_array", &get_result_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'ifttt_msensor_get_result_array': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (avro_value_get_size(&get_result_list_value, &count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get get_result list size");
		goto l_err_get_result_list_size;
	}

	if (count == 0)
	{
		//No data has been saved in the DB.
		ZWS_LOG(ZWS_LOG_ERR, "No matched trigger data in the network. Return empty 'Data' array.");
	}

	for (i = 0; i < count; i++)
	{
		if (avro_value_get_by_index(&get_result_list_value, i, &get_result_value, NULL) != 0)
		{
			goto l_err_get_result_value;
		}

		pchSensor_value = NULL;
		pchSensor_Unit = NULL;
		if ((zw_avro_field_get_string(&get_result_value, "sensor_value", &pchSensor_value) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_string(&get_result_value, "sensor_unit", &pchSensor_Unit) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_long(&get_result_value, "utime", &utime) != ZWS_STATUS_OK))
		{
			goto l_err_get_fields;
		}

		if (zwu_time_get_iso8601(utime, &utime_iso8601) != ZWU_STATUS_OK)
		{
			goto l_err_get_iso8601_time;
		}

		jsMetaObj = zwu_json_create_object();

		if (jsMetaObj)
		{
			sprintf(arIDValue, "%lu-%llu", *pcombo_id, (unsigned long long)utime);

			zwu_json_add_string_to_object(jsMetaObj, "id", arIDValue);

			zwu_json_add_number_to_object(jsMetaObj, "timestamp", utime);
		}

		jsMatchedRecordObj = zwu_json_create_object();

		if (jsMatchedRecordObj)
		{
			zwu_json_add_string_to_object(jsMatchedRecordObj, "created_at", utime_iso8601);

			zwu_json_add_string_to_object(jsMatchedRecordObj, "device_id", pchValue);

			zwu_json_add_string_to_object(jsMatchedRecordObj, IFTTT_REQUEST_BODY_IFTTT_DEVICE_NAME, arDeviceName);

			zwu_json_add_string_to_object(jsMatchedRecordObj, "sensor_value", pchSensor_value);
			zwu_json_add_string_to_object(jsMatchedRecordObj, "sensor_unit", pchSensor_Unit);

			zwu_json_add_item_to_object(jsMatchedRecordObj, "meta", jsMetaObj);

			zwu_json_add_item_to_array(jsData, jsMatchedRecordObj);
		}

		ZWS_FREE(utime_iso8601);
		utime_iso8601 = NULL;

		zw_avro_field_free_string(pchSensor_value);
		zw_avro_field_free_string(pchSensor_Unit);
	}

	ZWS_FREE(pchValue);

	res = ZWS_STATUS_OK;

	return res;

	ZWS_FREE(utime_iso8601);
l_err_get_iso8601_time:
l_err_get_fields:
	if (pchSensor_value)
		zw_avro_field_free_string(pchSensor_value);
	if (pchSensor_Unit)
		zw_avro_field_free_string(pchSensor_Unit);
l_err_get_result_value:
l_err_get_result_list_size:
l_err_get_list_value:
l_err_generate_device_name:
	ZWS_FREE(pchValue);
l_err_pchValue_alloc:
l_err_invalid_combo_id:
	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);
	return res;
}

/** Handlers for msensor_temp_lt option */
static zws_avi_message_ifttt_t ifttt_msensor_temp_lt_option =
{
	"ifttt_msensor_temp_lt", 
	
	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_mSensor_Temp_LT,

	{IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_name},

	"ifttt_network_msensor_option",

	zws_avi_network_ifttt_msensor_option_argument,
	zws_avi_network_ifttt_device_name_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for msensor_temp_lt */
static zws_avi_message_ifttt_t ifttt_msensor_temp_lt =
{
	"ifttt_msensor_temp_lt",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_mSensor_Temp_LT,

	{IFTTT_REQUEST_SUBTYPE_NONE},

	"ifttt_interface_msensor_get",

	zws_avi_network_ifttt_msensor_get_argument,
	zws_avi_network_ifttt_msensor_get_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for msensor_temp_gt option */
static zws_avi_message_ifttt_t ifttt_msensor_temp_gt_option =
{
	"ifttt_msensor_temp_gt",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_mSensor_Temp_GT,

	{ IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_name },

	"ifttt_network_msensor_option",

	zws_avi_network_ifttt_msensor_option_argument,
	zws_avi_network_ifttt_device_name_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for msensor_temp_gt */
static zws_avi_message_ifttt_t ifttt_msensor_temp_gt =
{
	"ifttt_msensor_temp_gt",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_mSensor_Temp_GT,

	{ IFTTT_REQUEST_SUBTYPE_NONE },

	"ifttt_interface_msensor_get",

	zws_avi_network_ifttt_msensor_get_argument,
	zws_avi_network_ifttt_msensor_get_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for msensor_lum_lt option */
static zws_avi_message_ifttt_t ifttt_msensor_lum_lt_option =
{
	"ifttt_msensor_lum_lt",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_mSensor_Luminance_LT,

	{ IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_name },

	"ifttt_network_msensor_option",

	zws_avi_network_ifttt_msensor_option_argument,
	zws_avi_network_ifttt_device_name_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for msensor_lum_lt */
static zws_avi_message_ifttt_t ifttt_msensor_lum_lt =
{
	"ifttt_msensor_lum_lt",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_mSensor_Luminance_LT,

	{ IFTTT_REQUEST_SUBTYPE_NONE },

	"ifttt_interface_msensor_get",

	zws_avi_network_ifttt_msensor_get_argument,
	zws_avi_network_ifttt_msensor_get_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for msensor_lum_gt option */
static zws_avi_message_ifttt_t ifttt_msensor_lum_gt_option =
{
	"ifttt_msensor_lum_gt",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_mSensor_Luminance_GT,

	{ IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_name },

	"ifttt_network_msensor_option",

	zws_avi_network_ifttt_msensor_option_argument,
	zws_avi_network_ifttt_device_name_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for msensor_lum_gt */
static zws_avi_message_ifttt_t ifttt_msensor_lum_gt =
{
	"ifttt_msensor_lum_gt",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_mSensor_Luminance_GT,

	{ IFTTT_REQUEST_SUBTYPE_NONE },

	"ifttt_interface_msensor_get",

	zws_avi_network_ifttt_msensor_get_argument,
	zws_avi_network_ifttt_msensor_get_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for msensor_hum_lt option */
static zws_avi_message_ifttt_t ifttt_msensor_hum_lt_option =
{
	"ifttt_msensor_hum_lt",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_mSensor_Humidity_LT,

	{ IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_name },

	"ifttt_network_msensor_option",

	zws_avi_network_ifttt_msensor_option_argument,
	zws_avi_network_ifttt_device_name_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for msensor_hum_lt */
static zws_avi_message_ifttt_t ifttt_msensor_hum_lt =
{
	"ifttt_msensor_hum_lt",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_mSensor_Humidity_LT,

	{ IFTTT_REQUEST_SUBTYPE_NONE },

	"ifttt_interface_msensor_get",

	zws_avi_network_ifttt_msensor_get_argument,
	zws_avi_network_ifttt_msensor_get_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for msensor_hum_gt option */
static zws_avi_message_ifttt_t ifttt_msensor_hum_gt_option =
{
	"ifttt_msensor_hum_gt",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_mSensor_Humidity_GT,

	{ IFTTT_REQUEST_SUBTYPE_OPTION, eDevice_name },

	"ifttt_network_msensor_option",

	zws_avi_network_ifttt_msensor_option_argument,
	zws_avi_network_ifttt_device_name_option_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};

/** Handlers for msensor_hum_gt */
static zws_avi_message_ifttt_t ifttt_msensor_hum_gt =
{
	"ifttt_msensor_hum_gt",

	IFTTT_REQUEST_TYPE_TRIGGER,
	eTrigger_mSensor_Humidity_GT,

	{ IFTTT_REQUEST_SUBTYPE_NONE },

	"ifttt_interface_msensor_get",

	zws_avi_network_ifttt_msensor_get_argument,
	zws_avi_network_ifttt_msensor_get_result,

	IFTTT_RESPONSE_TYPE_DATA_ARRAY,

	""
};


/** Register command handlers */
zws_status_t zws_avi_ifttt_msensor_init(zwu_array_list_t message_list)
{
	if ((zws_avi_message_list_add_message_ifttt(message_list, &ifttt_msensor_temp_lt_option) != ZWS_STATUS_OK)
		|| (zws_avi_message_list_add_message_ifttt(message_list, &ifttt_msensor_temp_lt) != ZWS_STATUS_OK)
		|| (zws_avi_message_list_add_message_ifttt(message_list, &ifttt_msensor_temp_gt_option) != ZWS_STATUS_OK)
		|| (zws_avi_message_list_add_message_ifttt(message_list, &ifttt_msensor_temp_gt) != ZWS_STATUS_OK)

		|| (zws_avi_message_list_add_message_ifttt(message_list, &ifttt_msensor_lum_lt_option) != ZWS_STATUS_OK)
		|| (zws_avi_message_list_add_message_ifttt(message_list, &ifttt_msensor_lum_lt) != ZWS_STATUS_OK)
		|| (zws_avi_message_list_add_message_ifttt(message_list, &ifttt_msensor_lum_gt_option) != ZWS_STATUS_OK)
		|| (zws_avi_message_list_add_message_ifttt(message_list, &ifttt_msensor_lum_gt) != ZWS_STATUS_OK)

		|| (zws_avi_message_list_add_message_ifttt(message_list, &ifttt_msensor_hum_lt_option) != ZWS_STATUS_OK)
		|| (zws_avi_message_list_add_message_ifttt(message_list, &ifttt_msensor_hum_lt) != ZWS_STATUS_OK)
		|| (zws_avi_message_list_add_message_ifttt(message_list, &ifttt_msensor_hum_gt_option) != ZWS_STATUS_OK)
		|| (zws_avi_message_list_add_message_ifttt(message_list, &ifttt_msensor_hum_gt) != ZWS_STATUS_OK)

		)
	{
		goto l_err_ifttt_status;
	}

	return ZWS_STATUS_OK;

l_err_ifttt_status:
	return ZWS_STATUS_ERROR;
}



