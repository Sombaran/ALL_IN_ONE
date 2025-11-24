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
 * @file    zws_avi_interfaces_sensor_multilevel.c
 *
 * @brief   AVI message 'interface_sensor_multilevel_xxx'
 * @details Handles the AVI message 'interface_sensor_multilevel_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-09-18
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message_interfaces.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zwu_number.h"

/** Sensor Multilevel Setup */
#define COMMAND_SENSOR_MULTILEVEL_SETUP              1
/** Sensor Multilevel Get */
#define COMMAND_SENSOR_MULTILEVEL_GET                2
/** Sensor Multilevel Report */
#define COMMAND_SENSOR_MULTILEVEL_REPORT             3
/** Sensor Multilevel Supported Get */
#define COMMAND_SENSOR_MULTILEVEL_SUPPORTED_GET      4
/** Sensor Multilevel Supported Report */
#define COMMAND_SENSOR_MULTILEVEL_SUPPORTED_REPORT   5
/** Sensor Multilevel Supported Get */
#define COMMAND_SENSOR_MULTILEVEL_SCALE_SUPPORTED_GET      6
/** Sensor Multilevel Supported Report */
#define COMMAND_SENSOR_MULTILEVEL_SCALE_SUPPORTED_REPORT   7

/** Sensor type not specified */
#define COMMAND_SENSOR_TYPE_NONE					(-1)

/** Sensor scale not specified */
#define COMMAND_SENSOR_SCALE_NONE					(-1)


/**
* @brief Set Avro value for RPC request for Get command using CGI request arguments
* @param[in]     args              CGI request arguments
* @param[in]     args_count        Count of CGI request arguments
* @param[in]     arg_value_p       Avro value for RPC request
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_interfaces_sensor_multilevel_get_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int type, scale;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	type = COMMAND_SENSOR_TYPE_NONE;
	if ((i < args_count) && !strcmp(args[i][0], "type"))
	{
		if (zwu_parse_int32_non_negative(args[i][1], &type) != ZWU_STATUS_OK)
		{
			goto l_err_type;
		}
		i++;
	}

	scale = COMMAND_SENSOR_SCALE_NONE;
	if ((i < args_count) && !strcmp(args[i][0], "unit"))
	{
		if (zwu_parse_int32_non_negative(args[i][1], &scale) != ZWU_STATUS_OK)
		{
			goto l_err_scale;
		}
		i++;
	}

	if ((zw_avro_field_set_int(arg_value_p, "sensor_type", type) == ZWS_STATUS_OK) &&
		(zw_avro_field_set_int(arg_value_p, "scale", scale) == ZWS_STATUS_OK))
	{
		//ZWS_LOG(ZWS_LOG_ERR, "set avro field successful");
		//ZWS_LOG(ZWS_LOG_ERR, "type %d, scale %d", type, scale);
		return_status = ZWS_STATUS_OK;
	}

	return return_status;

l_err_scale:
l_err_type:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Get CGI response for Get command from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_sensor_multilevel_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	avro_value_t sensor_value_list_value;
	avro_value_t sensor_value_value;
	size_t sensor_value_count;
	int i;
	int64_t utime;
	int32_t sensor_type;
	int32_t precision;
	int32_t scale;
	int32_t size;
	void *sensor_value;
	size_t sensor_value_size;
	int32_t sensor_value_significand;
	double sensor_value_double;
	int j;

	if (avro_value_get_by_name(arg_value, "sensor_value_list", &sensor_value_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'sensor_value_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (avro_value_get_size(&sensor_value_list_value, &sensor_value_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get sensor value list size");
		goto l_err_sensor_value_list_size;
	}

	for (i = 0; i < sensor_value_count; i++)
	{
		if (avro_value_get_by_index(&sensor_value_list_value, i, &sensor_value_value, NULL) != 0)
		{
			goto l_err_get_endpoint_value;
		}

		if ((zw_avro_field_get_long(&sensor_value_value, "utime", &utime) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&sensor_value_value, "sensor_type", &sensor_type) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&sensor_value_value, "precision", &precision) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&sensor_value_value, "scale", &scale) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&sensor_value_value, "size", &size) != ZWS_STATUS_OK)
				)
		{
			goto l_err_get_fields;
		}

		if (zw_avro_field_get_fixed(&sensor_value_value, "sensor_value", &sensor_value, &sensor_value_size) != ZWS_STATUS_OK)
		{
			goto l_err_get_field_sensor_value;
		}

		// Size validation to prevent underflow/overlfow
		if ((sensor_value_size != 4) || (size < 0) || (size > sensor_value_size) || (size == 3))
		{
			ZWS_LOG(ZWS_LOG_ERR, "Invalid Sensor value field size");
			goto l_err_invalid_sensor_value_size;
		}

		sensor_value_significand = 0;
		for (j = size - 1; j >= 0 ; j--)
		{
			if (j > 0)
			{
				sensor_value_significand |= *((uint8_t *)sensor_value + j) << (8 * (size - 1 - j));
			}
			else
			{
				sensor_value_significand |= *((int8_t *)sensor_value + j) << (8 * (size - 1 - j));
			}
		}


		if (precision == 0)
		{
			if (zwu_string_buffer_append_format(result,
					"<%s %s=\"%llu\" %s=\"%u\" %s=\"%d\" %s=\"%u\" %s=\"%u\" />",
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "multi_lvl_snsr" : "sensor"),
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
					(uint64_t)utime,
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "s_type" : "type"),
					(uint8_t)sensor_type,
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "s_value" : "value"),
					sensor_value_significand,
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "s_precision" : "precision"),
					(uint8_t)precision,
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "s_unit" : "unit"),
					(uint8_t)scale
					) != ZWS_STATUS_OK)
			{
				goto l_err_append;
			}
		}
		else
		{
			sensor_value_double = (double)sensor_value_significand;

			for (j = 0; j < precision; j++)
			{
				sensor_value_double /= 10.0;
			}

			if (zwu_string_buffer_append_format(result,
					"<%s %s=\"%llu\" %s=\"%u\" %s=\"%.7lf\" %s=\"%u\" %s=\"%u\" />",
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "multi_lvl_snsr" : "sensor"),
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
					(uint64_t)utime,
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "s_type" : "type"),
					(uint8_t)sensor_type,
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "s_value" : "value"),
					sensor_value_double,
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "s_precision" : "precision"),
					(uint8_t)precision,
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "s_unit" : "unit"),
					(uint8_t)scale
					) != ZWS_STATUS_OK)
			{
				goto l_err_append;
			}
		}

		zw_avro_field_free_fixed(sensor_value);
	}

	return ZWS_STATUS_OK;

l_err_append:
l_err_invalid_sensor_value_size:
	zw_avro_field_free_fixed(sensor_value);
l_err_get_field_sensor_value:
l_err_get_fields:
l_err_get_endpoint_value:
l_err_sensor_value_list_size:
l_err_get_list_value:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Get CGI response for Supported Get command from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_sensor_multilevel_supported_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int64_t utime;
	avro_value_t sensor_type_list_value;
	avro_value_t sensor_type_value;
	size_t sensor_type_count;
	int32_t sensor_type;
	zwu_string_buffer_t sensor_type_list;
	int i;

	if (zw_avro_field_get_long(arg_value, "utime", &utime) != ZWS_STATUS_OK)
	{
		goto l_err_get_fields;
	}

	if (avro_value_get_by_name(arg_value, "supported_types", &sensor_type_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'supported_types': %s", avro_strerror());
		goto l_err_get_type_list_value;
	}

	if (avro_value_get_size(&sensor_type_list_value, &sensor_type_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get sensor type count value list size");
		goto l_err_sensor_type_list_size;
	}

	if (zwu_string_buffer_create(0, &sensor_type_list) != ZWS_STATUS_OK)
	{
		goto l_err_sensor_type_list_create;
	}

	for (i = 0; i < sensor_type_count; i++)
	{
		if (avro_value_get_by_index(&sensor_type_list_value, i, &sensor_type_value, NULL) != 0)
		{
			goto l_err_get_sensor_type_value;
		}

		if (avro_value_get_int(&sensor_type_value, &sensor_type) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get sensor type: %s", avro_strerror());
			goto l_err_get_sensor_type;
		}

		if (zwu_string_buffer_append_format(sensor_type_list, "%u,", sensor_type) != ZWS_STATUS_OK)
		{
			goto l_err_sensor_type_append;
		}
	}

	if (zwu_string_buffer_append_format(result,
			"<%s %s=\"%llu\" list=\"%s\" />",
			"sensor",
		    "utime",
			(uint64_t)utime,
			zwu_string_buffer_get_string(sensor_type_list)
			) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	zwu_string_buffer_destroy(sensor_type_list);

	return ZWS_STATUS_OK;

	l_err_append:
	l_err_sensor_type_append:
	l_err_get_sensor_type:
	l_err_get_sensor_type_value:
		zwu_string_buffer_destroy(sensor_type_list);
	l_err_sensor_type_list_create:
	l_err_sensor_type_list_size:
	l_err_get_type_list_value:
	l_err_get_fields:
		return ZWS_STATUS_ERROR;
}

/**
* @brief Set Avro value for RPC request for scale supported Get command using CGI request arguments
* @param[in]     args              CGI request arguments
* @param[in]     args_count        Count of CGI request arguments
* @param[in]     arg_value_p       Avro value for RPC request
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_interfaces_sensor_multilevel_scale_supported_get_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int sensor_type;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	sensor_type = COMMAND_SENSOR_TYPE_NONE;

	if ((i < args_count) && !strcmp(args[i][0], "type"))
	{
		if (zwu_parse_int32_non_negative(args[i][1], &sensor_type) != ZWU_STATUS_OK)
		{
			goto l_err_sensor_multilevel_type;
		}
		i++;
	}

	if (zw_avro_field_set_int(arg_value_p, "sensor_type", sensor_type) == ZWS_STATUS_OK)
	{
		return_status = ZWS_STATUS_OK;
	}

	return return_status;

l_err_sensor_multilevel_type:
	return ZWS_STATUS_ERROR;
}

/**
* @brief Get CGI response for Event Supported Get command from Avro value for RPC response
* @param[in]     web_api_version   Web API version
* @param[in]     combo_id          Combo ID
* @param[in]     arg_value         Avro value for RPC response
* @param[out]    result            CGI response string
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_interfaces_sensor_multilevel_scale_supported_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int64_t utime;
	avro_value_t scale_list_value;
	avro_value_t scale_value;
	size_t scale_count;
	int32_t sensor_type;
	int32_t scale;
	zwu_string_buffer_t scale_list;
	int i;

	if ((zw_avro_field_get_long(arg_value, "utime", &utime) != ZWS_STATUS_OK)
		|| (zw_avro_field_get_int(arg_value, "sensor_type", &sensor_type) != ZWS_STATUS_OK))
	{
		goto l_err_get_fields;
	}

	if (avro_value_get_by_name(arg_value, "supported_scales", &scale_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'supported_scales': %s", avro_strerror());
		goto l_err_get_scale_list_value;
	}

	if (avro_value_get_size(&scale_list_value, &scale_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get sensor_multilevel scale value list size");
		goto l_err_get_scale_list_value;
	}

	if (zwu_string_buffer_create(0, &scale_list) != ZWS_STATUS_OK)
	{
		goto l_err_sensor_multilevel_scale_list_create;
	}

	for (i = 0; i < scale_count; i++)
	{
		if (avro_value_get_by_index(&scale_list_value, i, &scale_value, NULL) != 0)
		{
			goto l_err_get_sensor_multilevel_scale_value;
		}

		if (avro_value_get_int(&scale_value, &scale) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get sensor_multilevel scale: %s", avro_strerror());
			goto l_err_get_sensor_multilevel_scale;
		}

		if (zwu_string_buffer_append_format(scale_list, "%u,", scale) != ZWS_STATUS_OK)
		{
			goto l_err_sensor_multilevel_scale_append;
		}
	}

	if (zwu_string_buffer_append_format(result,
		"<%s %s=\"%llu\" type=\"%u\" list=\"%s\" />",
		"sensor",
		"utime",
		(uint64_t)utime,
		(uint8_t)sensor_type,
		zwu_string_buffer_get_string(scale_list)
		) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	zwu_string_buffer_destroy(scale_list);

	return ZWS_STATUS_OK;

l_err_append:
l_err_sensor_multilevel_scale_append :
l_err_get_sensor_multilevel_scale :
l_err_get_sensor_multilevel_scale_value :
	zwu_string_buffer_destroy(scale_list);
l_err_sensor_multilevel_scale_list_create:
l_err_get_scale_list_value :
l_err_get_fields :
	return ZWS_STATUS_ERROR;
}

/** Handlers for Get command */
static zws_avi_message_interfaces_t interfaces_sensor_multilevel_get =
{
		{"zwif_sensor", "multi_lvl_snsr"}, COMMAND_SENSOR_MULTILEVEL_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_sensor_multilevel_get",

		zws_avi_interfaces_sensor_multilevel_get_argument,
		zws_avi_interfaces_sensor_multilevel_get_result,

		"tServMultiLvlSnsr"
};

/** Handlers for Supported Get command */
static zws_avi_message_interfaces_t interfaces_sensor_multilevel_supported_get =
{
		{"zwif_sensor", "multi_lvl_snsr"}, COMMAND_SENSOR_MULTILEVEL_SUPPORTED_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_sensor_multilevel_supported_get",

		NULL,
		zws_avi_interfaces_sensor_multilevel_supported_get_result,

		"tServMultiLvlSnsr"
};

/** Handlers for Scale Supported Get command */
static zws_avi_message_interfaces_t interfaces_sensor_multilevel_scale_supported_get =
{
		{ "zwif_sensor", "multi_lvl_snsr" }, COMMAND_SENSOR_MULTILEVEL_SCALE_SUPPORTED_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_sensor_multilevel_scale_supported_get",

		zws_avi_interfaces_sensor_multilevel_scale_supported_get_argument,
		zws_avi_interfaces_sensor_multilevel_scale_supported_get_result,

		"tServMultiLvlSnsr"
};

/** Register command handlers */
zws_status_t zws_avi_interfaces_sensor_multilevel_init(zwu_array_list_t message_list)
{
	if ((zws_avi_message_list_add_message_interfaces(message_list, &interfaces_sensor_multilevel_get) != ZWS_STATUS_OK)||
		 (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_sensor_multilevel_supported_get) != ZWS_STATUS_OK) ||
		 (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_sensor_multilevel_scale_supported_get) != ZWS_STATUS_OK)
		)
	{
		goto l_err_networks_list_get;
	}

	return ZWS_STATUS_OK;

l_err_networks_list_get:
	return ZWS_STATUS_ERROR;
}
