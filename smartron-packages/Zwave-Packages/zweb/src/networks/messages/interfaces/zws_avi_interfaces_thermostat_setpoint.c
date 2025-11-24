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
 * @file    zws_avi_interfaces_thermostat_setpoint.c
 *
 * @brief   AVI message 'interface_thermostat_setpoint_xxx'
 * @details Handles the AVI message 'interface_thermostat_setpoint_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-10-10
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message_interfaces.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zwu_number.h"

/** Thermostat Setpoint Setup */
#define COMMAND_THERMOSTAT_SETPOINT_SETUP				1
/** Thermostat Setpoint Get */
#define COMMAND_THERMOSTAT_SETPOINT_GET					2
/** Thermostat Setpoint Report */
#define COMMAND_THERMOSTAT_SETPOINT_REPORT				3
/** Thermostat Setpoint Set */
#define COMMAND_THERMOSTAT_SETPOINT_SET					4
/** Thermostat Setpoint Supported Get */
#define COMMAND_THERMOSTAT_SETPOINT_SUPPORTED_GET		5
/** Thermostat Setpoint Supported Report */
#define COMMAND_THERMOSTAT_SETPOINT_SUPPORTED_REPORT	6
/** Thermostat Setpoint Range Get */
#define COMMAND_THERMOSTAT_SETPOINT_RANGE_GET			7
/** Thermostat Setpoint Range Report */
#define COMMAND_THERMOSTAT_SETPOINT_RANGE_REPORT		8

/** Thermostat Setpoint Type not specified */
#define COMMAND_THERMOSTAT_SETPOINT_TYPE_NONE         (-1)
/** Thermostat Setpoint Size not specified */
#define COMMAND_THERMOSTAT_SETPOINT_SIZE_NONE         (-1)

/**
 * @brief Set Avro value for RPC request for Get command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_thermostat_setpoint_get_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int type;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	type = COMMAND_THERMOSTAT_SETPOINT_TYPE_NONE;
	if ((i < args_count) && !strcmp(args[i][0], "type"))
	{
		if (zwu_parse_int32_non_negative(args[i][1], &type) != ZWU_STATUS_OK)
		{
			goto l_err_type;
		}
		i++;
	}

	if (zw_avro_field_set_int(arg_value_p, "setpoint_type", type) == ZWS_STATUS_OK)
	{
		return_status = ZWS_STATUS_OK;
	}

	return return_status;

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
static zws_status_t zws_avi_interfaces_thermostat_setpoint_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	avro_value_t thermostat_setpoint_value_list_value;
	avro_value_t thermostat_setpoint_value_value;
	size_t thermostat_setpoint_value_count;
	int i;
	int64_t utime;
	int32_t setpoint_type;
	int32_t precision;
	int32_t scale;
	int32_t size;
	void *setpoint_value;
	size_t setpoint_value_size;
	int32_t setpoint_value_significand;
	double setpoint_value_double;
	int j;

	if (avro_value_get_by_name(arg_value, "thermostat_setpoint_value_list", &thermostat_setpoint_value_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'thermostat_setpoint_value_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (avro_value_get_size(&thermostat_setpoint_value_list_value, &thermostat_setpoint_value_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get thermostat_setpoint value list size");
		goto l_err_thermostat_setpoint_value_list_size;
	}

	for (i = 0; i < thermostat_setpoint_value_count; i++)
	{
		if (avro_value_get_by_index(&thermostat_setpoint_value_list_value, i, &thermostat_setpoint_value_value, NULL) != 0)
		{
			goto l_err_get_endpoint_value;
		}

		if ((zw_avro_field_get_long(&thermostat_setpoint_value_value, "utime", &utime) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&thermostat_setpoint_value_value, "setpoint_type", &setpoint_type) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&thermostat_setpoint_value_value, "precision", &precision) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&thermostat_setpoint_value_value, "scale", &scale) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&thermostat_setpoint_value_value, "size", &size) != ZWS_STATUS_OK)
				)
		{
			goto l_err_get_fields;
		}

		if (zw_avro_field_get_fixed(&thermostat_setpoint_value_value, "value", &setpoint_value, &setpoint_value_size) != ZWS_STATUS_OK)
		{
			goto l_err_get_field_thermostat_setpoint_value;
		}

		// Size validation to prevent underflow/overlfow
		if ((setpoint_value_size != 4) || (size < 0) || (size > setpoint_value_size) || (size == 3))
		{
			ZWS_LOG(ZWS_LOG_ERR, "Invalid thermostat_setpoint value field size");
			goto l_err_invalid_thermostat_setpoint_value_size;
		}

		setpoint_value_significand = 0;
		for (j = size - 1; j >= 0; j--)
		{
			if (j > 0)
			{
				setpoint_value_significand |= *((uint8_t *)setpoint_value + j) << (8 * (size - 1 - j));
			}
			else
			{
				setpoint_value_significand |= *((int8_t *)setpoint_value + j) << (8 * (size - 1 - j));
			}
		}

		if (precision == 0)
		{
			if (zwu_string_buffer_append_format(result,
					"<%s %s=\"%llu\" type=\"%u\" value=\"%d\" precision=\"%u\" unit=\"%u\" size=\"%u\" />",
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "thrmo_setpt" : "thrmo_setp"),
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
					(uint64_t)utime,
					(uint8_t)setpoint_type,
					setpoint_value_significand,
					(uint8_t)precision,
					(uint8_t)scale,
					(uint8_t)size
					) != ZWS_STATUS_OK)
			{
				goto l_err_append;
			}
		}
		else
		{
			setpoint_value_double = (double)setpoint_value_significand;

			for (j = 0; j < precision; j++)
			{
				setpoint_value_double /= 10.0;
			}

			if (zwu_string_buffer_append_format(result,
					"<%s %s=\"%llu\" type=\"%u\" value=\"%.7lf\" precision=\"%u\" unit=\"%u\" size=\"%u\" />",
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "thrmo_setpt" : "thrmo_setp"),
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
					(uint64_t)utime,
					(uint8_t)setpoint_type,
					setpoint_value_double,
					(uint8_t)precision,
					(uint8_t)scale,
					(uint8_t)size
					) != ZWS_STATUS_OK)
			{
				goto l_err_append;
			}
		}

		zw_avro_field_free_fixed(setpoint_value);
	}

	return ZWS_STATUS_OK;

l_err_append:
l_err_invalid_thermostat_setpoint_value_size:
	zw_avro_field_free_fixed(setpoint_value);
l_err_get_field_thermostat_setpoint_value:
l_err_get_fields:
l_err_get_endpoint_value:
l_err_thermostat_setpoint_value_list_size:
l_err_get_list_value:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Set Avro value for RPC request for Set command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_thermostat_setpoint_set_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int32_t setpoint_value_significand;
	uint8_t setpoint_data[4];
	int size;
	int minimum_size;
	int j;
	int type;
	int precision;
	int scale;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if ((i < args_count) && !strcmp(args[i][0], "type")
			&& (zwu_parse_int32_non_negative(args[i][1], &type) == ZWU_STATUS_OK)
			&& (zw_avro_field_set_int(arg_value_p, "setpoint_type", type) == ZWS_STATUS_OK))
	{
		i++;
		if ((i < args_count) && !strcmp(args[i][0], "value"))
		{
			// Allow negative also for 'value'
			if (zwu_parse_int32(args[i][1], &setpoint_value_significand) != ZWU_STATUS_OK)
			{
				goto l_err_setpoint_value_significand;
			}

			i++;
			if ((i < args_count) && !strcmp(args[i][0], "precision")
					&& (zwu_parse_int32_non_negative(args[i][1], &precision) == ZWU_STATUS_OK)
					&& (zw_avro_field_set_int(arg_value_p, "precision", precision) == ZWS_STATUS_OK))
			{
				i++;
				if ((i < args_count) && !strcmp(args[i][0], "unit")
						&& (zwu_parse_int32_non_negative(args[i][1], &scale) == ZWU_STATUS_OK)
						&& (zw_avro_field_set_int(arg_value_p, "scale", scale) == ZWS_STATUS_OK))
				{
					i++;

					// 'size' is optional
					size = COMMAND_THERMOSTAT_SETPOINT_SIZE_NONE;
					if ((i < args_count) && !strcmp(args[i][0], "size"))
					{
						if (zwu_parse_int32_non_negative(args[i][1], &size) != ZWU_STATUS_OK)
						{
							goto l_err_size;
						}
						i++;

						// Treat '0' as unspecified size
						if (size == 0)
						{
							size = COMMAND_THERMOSTAT_SETPOINT_SIZE_NONE;
						}
					}

					// if not given, find the minimum size needed for the value
					minimum_size = ((setpoint_value_significand >= (int8_t)0x80) && (setpoint_value_significand <= (int8_t)0x7F))
							? 1 : (((setpoint_value_significand >= (int16_t)0x8000) && (setpoint_value_significand <= (int16_t)0x7FFF))
									? 2 : 4);

					if (size == COMMAND_THERMOSTAT_SETPOINT_SIZE_NONE)
					{
						size = minimum_size;
					}

					if (size < minimum_size)
					{
						ZWS_LOG(ZWS_LOG_DEBUG, "Size too small. Minimum size required: %d", minimum_size);
						goto l_err_size_too_small;
					}

					memset(setpoint_data, 0, sizeof(setpoint_data));

					if (((size == 1) || (size == 2) || (size == 4))
							&& (zw_avro_field_set_int(arg_value_p, "size", size) == ZWS_STATUS_OK))
					{
						for (j = size - 1; j >= 0; j--)
						{
							setpoint_data[j] = (uint8_t)(setpoint_value_significand >> 8 * (size - 1 - j) & 0xFF);
						}

						if (zw_avro_field_set_fixed(arg_value_p, "value", setpoint_data, 4) == ZWS_STATUS_OK)
						{
							return_status = ZWS_STATUS_OK;
						}
					}
				}
			}
		}
	}

	return return_status;

l_err_size_too_small:
l_err_size:
l_err_setpoint_value_significand:
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
static zws_status_t zws_avi_interfaces_thermostat_setpoint_supported_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int64_t utime;
	avro_value_t setpoint_type_list_value;
	avro_value_t setpoint_type_value;
	size_t setpoint_type_count;
	int32_t setpoint_type;
	zwu_string_buffer_t setpoint_type_list;
	int i;

	if (zw_avro_field_get_long(arg_value, "utime", &utime) != ZWS_STATUS_OK)
	{
		goto l_err_get_fields;
	}

	if (avro_value_get_by_name(arg_value, "supported_types", &setpoint_type_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'supported_types': %s", avro_strerror());
		goto l_err_get_type_list_value;
	}

	if (avro_value_get_size(&setpoint_type_list_value, &setpoint_type_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get thermostat_setpoint value list size");
		goto l_err_setpoint_type_list_size;
	}

	if (zwu_string_buffer_create(0, &setpoint_type_list) != ZWS_STATUS_OK)
	{
		goto l_err_setpoint_type_list_create;
	}

	for (i = 0; i < setpoint_type_count; i++)
	{
		if (avro_value_get_by_index(&setpoint_type_list_value, i, &setpoint_type_value, NULL) != 0)
		{
			goto l_err_get_setpoint_type_value;
		}

		if (avro_value_get_int(&setpoint_type_value, &setpoint_type) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get setpoint type: %s", avro_strerror());
			goto l_err_get_setpoint_type;
		}

		if (zwu_string_buffer_append_format(setpoint_type_list, "%u,", setpoint_type) != ZWS_STATUS_OK)
		{
			goto l_err_setpoint_type_append;
		}
	}

	if (zwu_string_buffer_append_format(result,
			"<%s %s=\"%llu\" list=\"%s\" />",
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "sup_thrmo_setpt" : "thrmo_setp_sup"),
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
			(uint64_t)utime,
			zwu_string_buffer_get_string(setpoint_type_list)
			) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	zwu_string_buffer_destroy(setpoint_type_list);

	return ZWS_STATUS_OK;

l_err_append:
l_err_setpoint_type_append:
l_err_get_setpoint_type:
l_err_get_setpoint_type_value:
	zwu_string_buffer_destroy(setpoint_type_list);
l_err_setpoint_type_list_create:
l_err_setpoint_type_list_size:
l_err_get_type_list_value:
l_err_get_fields:
	return ZWS_STATUS_ERROR;
}

/**
* @brief Extract min/max values from Range Get command Avro value for RPC response
* @param[in]     range_min_max_field   Avro value for range min/max field
* @param[in]     combo_id          Combo ID
* @param[in]     arg_value         Avro value for RPC response
* @param[out]    result            CGI response string
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_interfaces_thermostat_setpoint_range_get_result_extract_data(
	avro_value_t range_min_max_field, 
	int32_t *pprecision,
	int32_t *pscale,
	int32_t *psize,
	int32_t *psetpoint_value_significand,
	double *psetpoint_value_double)
{
	int32_t precision;
	int32_t scale;
	int32_t size;
	void *setpoint_value = NULL;
	size_t setpoint_value_size;
	int32_t setpoint_value_significand;
	double setpoint_value_double = 0;
	int j;

	if ((zw_avro_field_get_int(&range_min_max_field, "precision", &precision) != ZWS_STATUS_OK)
		|| (zw_avro_field_get_int(&range_min_max_field, "scale", &scale) != ZWS_STATUS_OK)
		|| (zw_avro_field_get_int(&range_min_max_field, "size", &size) != ZWS_STATUS_OK)
		|| (zw_avro_field_get_fixed(&range_min_max_field, "value", &setpoint_value, &setpoint_value_size) != ZWS_STATUS_OK)
		)
		{
			goto l_err_get_field_thermostat_setpoint_range_min;
		}

	// Size validation to prevent underflow/overlfow
	if ((setpoint_value_size != 4) || (size < 0) || (size > setpoint_value_size) || (size == 3))
	{
		ZWS_LOG(ZWS_LOG_ERR, "Invalid thermostat_setpoint value field size");
		goto l_err_invalid_thermostat_setpoint_range_size_min;
	}

	setpoint_value_significand = 0;
	for (j = size - 1; j >= 0; j--)
	{
		if (j > 0)
		{
			setpoint_value_significand |= *((uint8_t *)setpoint_value + j) << (8 * (size - 1 - j));
		}
		else
		{
			setpoint_value_significand |= *((int8_t *)setpoint_value + j) << (8 * (size - 1 - j));
		}
	}

	if (precision != 0)
	{
		setpoint_value_double = (double)setpoint_value_significand;

		for (j = 0; j < precision; j++)
		{
			setpoint_value_double /= 10.0;
		}
	}

	zw_avro_field_free_fixed(setpoint_value);

	*pprecision = precision;
	*pscale = scale;
	*psize = size;
	*psetpoint_value_significand = setpoint_value_significand;
	*psetpoint_value_double = setpoint_value_double;

	return ZWS_STATUS_OK;

l_err_invalid_thermostat_setpoint_range_size_min:
	if(setpoint_value)
		zw_avro_field_free_fixed(setpoint_value);
l_err_get_field_thermostat_setpoint_range_min:
	return ZWS_STATUS_ERROR;
}

/**
* @brief Get CGI response for Range Get command from Avro value for RPC response
* @param[in]     web_api_version   Web API version
* @param[in]     combo_id          Combo ID
* @param[in]     arg_value         Avro value for RPC response
* @param[out]    result            CGI response string
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_interfaces_thermostat_setpoint_range_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	avro_value_t thermostat_setpoint_range_list_value;
	avro_value_t thermostat_setpoint_range_value;
	avro_value_t thermostat_setpoint_range_min_max_field;
	size_t thermostat_setpoint_range_count;
	int i;
	int64_t utime;
	int32_t setpoint_type;
	int32_t precision;
	int32_t scale;
	int32_t size;
	int32_t setpoint_value_significand;
	double setpoint_value_double;

	if (avro_value_get_by_name(arg_value, "thermostat_setpoint_range_list", &thermostat_setpoint_range_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'thermostat_setpoint_range_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (avro_value_get_size(&thermostat_setpoint_range_list_value, &thermostat_setpoint_range_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get thermostat_setpoint range list size");
		goto l_err_thermostat_setpoint_range_list_size;
	}

	for (i = 0; i < thermostat_setpoint_range_count; i++)
	{
		if (avro_value_get_by_index(&thermostat_setpoint_range_list_value, i, &thermostat_setpoint_range_value, NULL) != 0)
		{
			goto l_err_get_range_value;
		}

		if ((zw_avro_field_get_long(&thermostat_setpoint_range_value, "utime", &utime) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(&thermostat_setpoint_range_value, "setpoint_type", &setpoint_type) != ZWS_STATUS_OK)
			)
			
		{
			goto l_err_get_fields;
		}

		if (zwu_string_buffer_append_format(result,
			"<%s %s=\"%llu\" type=\"%u\" ",
			"thrmo_setp_range",
			"utime",
			(uint64_t)utime,
			(uint8_t)setpoint_type
		) != ZWS_STATUS_OK)
		{
			goto l_err_append_1;
		}

		//range_min
		{
			if (avro_value_get_by_name(&thermostat_setpoint_range_value, "range_min", &thermostat_setpoint_range_min_max_field, NULL) != 0)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'range_min': %s", avro_strerror());
				goto l_err_get_min_field;
			}

			if (zws_avi_interfaces_thermostat_setpoint_range_get_result_extract_data(
				thermostat_setpoint_range_min_max_field,
				&precision,
				&scale,
				&size,
				&setpoint_value_significand,
				&setpoint_value_double) != ZWS_STATUS_OK)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field values in 'range_min': %s", avro_strerror());
				goto l_err_get_min_value;
			}

			if (precision == 0)
			{
				if (zwu_string_buffer_append_format(result, "min_value=\"%d\" ", setpoint_value_significand) != ZWS_STATUS_OK)
				{
					goto l_err_append_2;
				}
			}
			else
			{
				if (zwu_string_buffer_append_format(result, "min_value=\"%.7lf\" ", setpoint_value_double) != ZWS_STATUS_OK)
				{
					goto l_err_append_3;
				}
			}

			if (zwu_string_buffer_append_format(result,
				"min_precision=\"%u\" min_unit=\"%u\" min_size=\"%u\" ",
				(uint8_t)precision,
				(uint8_t)scale,
				(uint8_t)size
			) != ZWS_STATUS_OK)
			{
				goto l_err_append_4;
			}
		}

		//range_max
		{
			if (avro_value_get_by_name(&thermostat_setpoint_range_value, "range_max", &thermostat_setpoint_range_min_max_field, NULL) != 0)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'range_max': %s", avro_strerror());
				goto l_err_get_max_field;
			}

			if (zws_avi_interfaces_thermostat_setpoint_range_get_result_extract_data(
				thermostat_setpoint_range_min_max_field,
				&precision,
				&scale,
				&size,
				&setpoint_value_significand,
				&setpoint_value_double) != ZWS_STATUS_OK)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field values in 'range_max': %s", avro_strerror());
				goto l_err_get_max_value;
			}

			if (precision == 0)
			{
				if (zwu_string_buffer_append_format(result, "max_value=\"%d\" ", setpoint_value_significand) != ZWS_STATUS_OK)
				{
					goto l_err_append_5;
				}
			}
			else
			{
				if (zwu_string_buffer_append_format(result, "max_value=\"%.7lf\" ", setpoint_value_double) != ZWS_STATUS_OK)
				{
					goto l_err_append_6;
				}
			}

			if (zwu_string_buffer_append_format(result,
				"max_precision=\"%u\" max_unit=\"%u\" max_size=\"%u\" />",
				(uint8_t)precision,
				(uint8_t)scale,
				(uint8_t)size
			) != ZWS_STATUS_OK)
			{
				goto l_err_append_7;
			}
		}
	}

	return ZWS_STATUS_OK;

l_err_append_7:
l_err_append_6:
l_err_append_5:
l_err_get_max_value:
l_err_get_max_field:
l_err_append_4:
l_err_append_3:
l_err_append_2:
l_err_get_min_value:
l_err_get_min_field:
l_err_append_1:
l_err_get_fields:
l_err_get_range_value:
l_err_thermostat_setpoint_range_list_size:
l_err_get_list_value:
	return ZWS_STATUS_ERROR;
}

/** Handlers for Get command */
static zws_avi_message_interfaces_t interfaces_thermostat_setpoint_get =
{
		{"zwif_thrmo_setp", "thrmo_setpt"}, COMMAND_THERMOSTAT_SETPOINT_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_thermostat_setpoint_get",

		zws_avi_interfaces_thermostat_setpoint_get_argument,
		zws_avi_interfaces_thermostat_setpoint_get_result,

		"tServThrmoSetpt"
};

/** Handlers for Set command */
static zws_avi_message_interfaces_t interfaces_thermostat_setpoint_set =
{
		{"zwif_thrmo_setp", "thrmo_setpt"}, COMMAND_THERMOSTAT_SETPOINT_SET,
		ZWS_AVI_MESSAGE_PASSIVE_NOT_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_thermostat_setpoint_set",

		zws_avi_interfaces_thermostat_setpoint_set_argument,
		NULL,

		"tServThrmoSetpt"
};

/** Handlers for Supported Get command */
static zws_avi_message_interfaces_t interfaces_thermostat_setpoint_supported_get =
{
		{"zwif_thrmo_setp", "thrmo_setpt"}, COMMAND_THERMOSTAT_SETPOINT_SUPPORTED_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_thermostat_setpoint_supported_get",

		NULL,
		zws_avi_interfaces_thermostat_setpoint_supported_get_result,

		"tServThrmoSetpt"
};

/** Handlers for Range Get command */
static zws_avi_message_interfaces_t interfaces_thermostat_setpoint_range_get =
{
	{ "zwif_thrmo_setp", "thrmo_setpt" }, COMMAND_THERMOSTAT_SETPOINT_RANGE_GET,
	ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
	ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

	"interface_thermostat_setpoint_range_get",

	zws_avi_interfaces_thermostat_setpoint_get_argument,
	zws_avi_interfaces_thermostat_setpoint_range_get_result,

	"tServThrmoSetpt"
};

/** Register command handlers */
zws_status_t zws_avi_interfaces_thermostat_setpoint_init(zwu_array_list_t message_list)
{
	if ((zws_avi_message_list_add_message_interfaces(message_list, &interfaces_thermostat_setpoint_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_thermostat_setpoint_set) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_thermostat_setpoint_supported_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_thermostat_setpoint_range_get) != ZWS_STATUS_OK)
		)
	{
		goto l_err_networks_list_get;
	}

	return ZWS_STATUS_OK;

l_err_networks_list_get:
	return ZWS_STATUS_ERROR;
}

