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
 * @file    zws_avi_interfaces_meter.c
 *
 * @brief   AVI message 'interface_meter_xxx'
 * @details Handles the AVI message 'interface_meter_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-10-02
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message_interfaces.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zwu_number.h"

/** Meter Setup */
#define COMMAND_METER_SETUP              1
/** Meter Get */
#define COMMAND_METER_GET                2
/** Meter Report */
#define COMMAND_METER_REPORT             3
/** Meter Supported Get */
#define COMMAND_METER_SUPPORTED_GET      4
/** Meter Supported Report */
#define COMMAND_METER_SUPPORTED_REPORT   5
/** Meter Reset */
#define COMMAND_METER_RESET              6

/** Meter scale not specified */
#define COMMAND_METER_SCALE_NONE         (-1)

/**
 * @brief Set Avro value for RPC request for Get command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_meter_get_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int scale;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	scale = COMMAND_METER_SCALE_NONE;
	if ((i < args_count) && !strcmp(args[i][0], "unit"))
	{
		if (zwu_parse_int32_non_negative(args[i][1], &scale) != ZWU_STATUS_OK)
		{
			goto l_err_scale;
		}
		i++;
	}

	if (zw_avro_field_set_int(arg_value_p, "scale", scale) == ZWS_STATUS_OK)
	{
		return_status = ZWS_STATUS_OK;
	}

	return return_status;

l_err_scale:
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
static zws_status_t zws_avi_interfaces_meter_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	avro_value_t meter_value_list_value;
	avro_value_t meter_value_value;
	size_t meter_value_count;
	int i;
	int64_t utime;
	int32_t meter_type;
	int32_t precision;
	int32_t scale;
	int32_t size;
	void *meter_value;
	size_t meter_value_size;
	int32_t meter_value_significand;
	double meter_value_double;
	int32_t rate_type;
	int32_t delta_time;
	void *previous_meter_value;
	size_t previous_meter_value_size;
	int32_t previous_meter_value_significand;
	double previous_meter_value_double;
	int j;

	if (avro_value_get_by_name(arg_value, "meter_value_list", &meter_value_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'meter_value_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (avro_value_get_size(&meter_value_list_value, &meter_value_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get meter value list size");
		goto l_err_meter_value_list_size;
	}

	for (i = 0; i < meter_value_count; i++)
	{
		if (avro_value_get_by_index(&meter_value_list_value, i, &meter_value_value, NULL) != 0)
		{
			goto l_err_get_endpoint_value;
		}

		if ((zw_avro_field_get_long(&meter_value_value, "utime", &utime) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&meter_value_value, "meter_type", &meter_type) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&meter_value_value, "precision", &precision) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&meter_value_value, "scale", &scale) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&meter_value_value, "size", &size) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&meter_value_value, "rate_type", &rate_type) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&meter_value_value, "delta_time", &delta_time) != ZWS_STATUS_OK)
				)
		{
			goto l_err_get_fields;
		}

		if (zw_avro_field_get_fixed(&meter_value_value, "meter_value", &meter_value, &meter_value_size) != ZWS_STATUS_OK)
		{
			goto l_err_get_field_meter_value;
		}

		if (zw_avro_field_get_fixed(&meter_value_value, "previous_meter_value", &previous_meter_value, &previous_meter_value_size) != ZWS_STATUS_OK)
		{
			goto l_err_get_field_previous_meter_value;
		}

		// Size validation to prevent underflow/overlfow
		if ((meter_value_size != 4) || ((uint8_t)size > meter_value_size) || ((uint8_t)size == 3))
		{
			ZWS_LOG(ZWS_LOG_ERR, "Invalid meter value field size");
			goto l_err_invalid_meter_value_size;
		}

		if ((previous_meter_value_size != 4) || ((uint8_t)size > previous_meter_value_size))
		{
			ZWS_LOG(ZWS_LOG_ERR, "Invalid previous meter value field size");
			goto l_err_invalid_previous_meter_value_size;
		}

		meter_value_significand = 0;
		previous_meter_value_significand = 0;
		for (j = size - 1; j >= 0 ; j--)
		{
			if (j > 0)
			{
				meter_value_significand |= *((uint8_t *)meter_value + j) << (8 * (size - 1 - j));
				previous_meter_value_significand |= *((uint8_t *)previous_meter_value + j) << (8 * (size - 1 - j));
			}
			else
			{
				meter_value_significand |= *((int8_t *)meter_value + j) << (8 * (size - 1 - j));
				previous_meter_value_significand |= *((int8_t *)previous_meter_value + j) << (8 * (size - 1 - j));
			}
		}

		if (precision == 0)
		{
			if (zwu_string_buffer_append_format(result,
					"<%s %s=\"%llu\" type=\"%u\" value=\"%d\" precision=\"%u\" unit=\"%u\" rtype=\"%u\" delta=\"%u\" prev=\"%d\" />",
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "meter" : "meter"),
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
					(uint64_t)utime,
					(uint8_t)meter_type,
					meter_value_significand,
					(uint8_t)precision,
					(uint8_t)scale,
					(uint8_t)rate_type,
					(uint16_t)delta_time,
					previous_meter_value_significand
					) != ZWS_STATUS_OK)
			{
				goto l_err_append;
			}
		}
		else
		{
			meter_value_double = (double)meter_value_significand;
			previous_meter_value_double = (double)previous_meter_value_significand;

			for (j = 0; j < precision; j++)
			{
				meter_value_double /= 10.0;
				previous_meter_value_double /= 10.0;
			}

			if (zwu_string_buffer_append_format(result,
					"<%s %s=\"%llu\" type=\"%u\" value=\"%.7lf\" precision=\"%u\" unit=\"%u\" rtype=\"%u\" delta=\"%u\" prev=\"%.7lf\" />",
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "meter" : "meter"),
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
					(uint64_t)utime,
					(uint8_t)meter_type,
					meter_value_double,
					(uint8_t)precision,
					(uint8_t)scale,
					(uint8_t)rate_type,
					(uint16_t)delta_time,
					previous_meter_value_double
					) != ZWS_STATUS_OK)
			{
				goto l_err_append;
			}
		}

		zw_avro_field_free_fixed(previous_meter_value);
		zw_avro_field_free_fixed(meter_value);
	}

	return ZWS_STATUS_OK;

l_err_append:
l_err_invalid_previous_meter_value_size:
l_err_invalid_meter_value_size:
	zw_avro_field_free_fixed(previous_meter_value);
l_err_get_field_previous_meter_value:
	zw_avro_field_free_fixed(meter_value);
l_err_get_field_meter_value:
l_err_get_fields:
l_err_get_endpoint_value:
l_err_meter_value_list_size:
l_err_get_list_value:
	return ZWS_STATUS_ERROR;
}

/*
 * Supported Get
 */
/**
 * @brief Get CGI response for Supported Get command from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_meter_supported_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int64_t utime;
	int32_t meter_type;
	int32_t scale_supported;
	int32_t meter_reset;

	if ((zw_avro_field_get_long(arg_value, "utime", &utime) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "meter_type", &meter_type) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "scale_supported", &scale_supported) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "meter_reset", &meter_reset) != ZWS_STATUS_OK))
	{
		goto l_err_get_fields;
	}

	if (zwu_string_buffer_append_format(result,
			"<%s %s=\"%llu\" type=\"%u\" units=\"%u\" reset=\"%u\" />",
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "sup_meter" : "meter_sup"),
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
			(uint64_t)utime,
			meter_type,
			scale_supported,
			meter_reset
			) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	return ZWS_STATUS_OK;

l_err_append:
l_err_get_fields:
	return ZWS_STATUS_ERROR;
}

/** Handlers for Get command */
static zws_avi_message_interfaces_t interfaces_meter_get =
{
		{"zwif_meter", "meter"}, COMMAND_METER_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_meter_get",

		zws_avi_interfaces_meter_get_argument,
		zws_avi_interfaces_meter_get_result,

		"tServMeter"
};

/** Handlers for Supported Get command */
static zws_avi_message_interfaces_t interfaces_meter_supported_get =
{
		{"zwif_meter", "meter"}, COMMAND_METER_SUPPORTED_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_meter_supported_get",

		NULL,
		zws_avi_interfaces_meter_supported_get_result,

		"tServMeter"
};

/** Handlers for Reset command */
static zws_avi_message_interfaces_t interfaces_meter_reset =
{
		{"zwif_meter", "meter"}, COMMAND_METER_RESET,
		ZWS_AVI_MESSAGE_PASSIVE_NOT_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_meter_reset",

		NULL,
		NULL,

		"tServMeter"
};

/** Register command handlers */
zws_status_t zws_avi_interfaces_meter_init(zwu_array_list_t message_list)
{
	if ((zws_avi_message_list_add_message_interfaces(message_list, &interfaces_meter_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_meter_supported_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_meter_reset) != ZWS_STATUS_OK))
	{
		goto l_err_networks_list_get;
	}

	return ZWS_STATUS_OK;

l_err_networks_list_get:
	return ZWS_STATUS_ERROR;
}


