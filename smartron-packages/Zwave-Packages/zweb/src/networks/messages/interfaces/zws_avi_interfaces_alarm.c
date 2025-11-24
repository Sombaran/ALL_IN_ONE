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
 * @file    zws_avi_interfaces_alarm.c
 *
 * @brief   AVI message 'interface_alarm_xxx'
 * @details Handles the AVI message 'interface_alarm_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-02-21
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message_interfaces.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zwu_number.h"
#include "zwu_encoding.h"

/** Alarm Setup */
#define COMMAND_ALARM_SETUP                   1
/** Alarm Get */
#define COMMAND_ALARM_GET                     2
/** Alarm Report */
#define COMMAND_ALARM_REPORT                  3
/** Alarm Set */
#define COMMAND_ALARM_SET                     4
/** Alarm Supported Get */
#define COMMAND_ALARM_TYPE_SUPPORTED_GET      5
/** Alarm Supported Report */
#define COMMAND_ALARM_TYPE_SUPPORTED_REPORT   6
/** Alarm Event Supported Get */
#define COMMAND_ALARM_EVENT_SUPPORTED_GET     7
/** Alarm Event Supported Report */
#define COMMAND_ALARM_EVENT_SUPPORTED_REPORT  8

/** Alarm type not specified */
#define COMMAND_ALARM_TYPE_NONE               (-1)
/** Latest alarm type */
#define COMMAND_ALARM_TYPE_LATEST             (-2)

/** Z-Wave Alarm type not specified */
#define COMMAND_ZWAVE_ALARM_TYPE_NONE         (-1)

/** Z-Wave alarm event not specified */
#define COMMAND_ZWAVE_ALARM_EVENT_NONE        (-1)

/** Z-wave alarm parameter type */
#define ZW_ALRM_PARAM_LOC               1    /**< node location UTF-8 string (NULL terminated)*/
#define ZW_ALRM_PARAM_USRID             2    /**< user id*/
#define ZW_ALRM_PARAM_OEM_ERR_CODE      3    /**< OEM proprietary system failure code */
#define ZW_ALRM_PARAM_PROPRIETARY		4	 /**< Proprietary event parameters */
#define ZW_ALRM_PARAM_UNKNOWN			0xFF /**< Unknown alarm event parameters. It could be from a higher version of CC,
												  or the device violates the spec and send parameters when Spec does not define*/


/** First Alarm */
#define ZW_ALRM_FIRST  0xFF

/**
 * @brief Set Avro value for RPC request for Get command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_alarm_get_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int alarm_type;
	int zwave_alarm_type;
	int zwave_alarm_event;
	int j;
	char *key;
	char *value;
	int sub_command;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	alarm_type = COMMAND_ALARM_TYPE_NONE;
	zwave_alarm_type = COMMAND_ZWAVE_ALARM_TYPE_NONE;
	zwave_alarm_event = COMMAND_ZWAVE_ALARM_EVENT_NONE;

	if ((i < args_count) && !strcmp(args[i][0], "vtype"))
	{
		if (zwu_parse_int32_non_negative(args[i][1], &alarm_type) != ZWU_STATUS_OK)
		{
			goto l_err_alarm_type;
		}
		i++;
	}

	if ((i < args_count) && !strcmp(args[i][0], "ztype"))
	{
		if (zwu_parse_int32_non_negative(args[i][1], &zwave_alarm_type) != ZWU_STATUS_OK)
		{
			goto l_err_zwave_alarm_type;
		}
		i++;
	}

	ZWS_LOG(ZWS_LOG_ERR, "b4 setting event ha");

	if ((i < args_count) && !strcmp(args[i][0], "event"))
	{
		if (zwu_parse_int32_non_negative(args[i][1], &zwave_alarm_event) != ZWU_STATUS_OK)
		{
			goto l_err_zwave_alarm_event;
		}
		i++;
	}
	ZWS_LOG(ZWS_LOG_ERR, "zwave_alarm_type: %d", zwave_alarm_type);
	ZWS_LOG(ZWS_LOG_ERR, "zwave_alarm_event: %d", zwave_alarm_event);

	/*
	 * TODO: Latest Alarm Report: To use some other special value for 'latest report' instead of
	 * overriding passive call for ZW_ALRM_FIRST. To be taken up after Engg UI supports it.
	 * Deliberate array index underflow: A hack to find if the request is active or passive.
	 * If passive and {alarm_type = 0; zwave_alarm_type = 255}, the request fetches latest
	 * Alarm report.
	 */
	if ((alarm_type == 0) && (zwave_alarm_type == ZW_ALRM_FIRST))
	{
		j = -1;
		while (j >= -2)
		{
			key = args[j][0];
			value = args[j][1];

			if ((!strcmp(key, "cmd") || !strcmp(key, "cmd_cgi") || !strcmp(key, "cmd_type"))
					&& (zwu_parse_int32_non_negative(value, &sub_command) == ZWU_STATUS_OK))
			{
				if (sub_command == COMMAND_ALARM_REPORT)
				{
					alarm_type = COMMAND_ALARM_TYPE_LATEST;

					ZWS_LOG(ZWS_LOG_DEBUG, "Latest Notification Report");
				}

				break;
			}

			j--;
		}
	}

	ZWS_LOG(ZWS_LOG_ERR, "b4 set avro field");
	if (zw_avro_field_set_int(arg_value_p, "alarm_type", alarm_type) == ZWS_STATUS_OK)
	{
		if ((zw_avro_field_set_int(arg_value_p, "zwave_alarm_type", zwave_alarm_type) == ZWS_STATUS_OK) &&
			(zw_avro_field_set_int(arg_value_p, "zwave_alarm_event", zwave_alarm_event) == ZWS_STATUS_OK))
		{
			ZWS_LOG(ZWS_LOG_ERR, "set avro field successful");
			ZWS_LOG(ZWS_LOG_ERR, "zwave_alarm_event %d", zwave_alarm_event);
			return_status = ZWS_STATUS_OK;
		}
	}

	return return_status;

l_err_zwave_alarm_type:
l_err_alarm_type:
l_err_zwave_alarm_event:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Get string equivalent of alarm event parameter
 * @param[in]     event_parameter_type       Event parameter type
 * @param[in]     event_parameter            Event parameter
 * @param[in]     event_parameter_length     Event parameter length
 * @param[out]    event_parameter_string_p   String equivalent of alarm event parameter
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zws_avi_interfaces_alarm_event_parameter_string_free() to free resources
 */
static zws_status_t zws_avi_interfaces_alarm_event_parameter_to_string(int event_parameter_type, char *event_parameter,
		size_t event_parameter_length, char **event_parameter_string_p)
{
	char *string;
	size_t string_size;
	int i;
	char byte_string[3];

	// Calculate string_size
	switch (event_parameter_type)
	{
	case ZW_ALRM_PARAM_LOC:
		// Needs maximum of 3 times the input buffer length for URI encoding
		string_size = (event_parameter_length * 3) + 1;
		break;

	case ZW_ALRM_PARAM_USRID:
		// String representation of one byte user id shall required a maximum of 4 bytes. Eg: 255\0
		string_size = 4;
		break;

	case ZW_ALRM_PARAM_OEM_ERR_CODE:
	case ZW_ALRM_PARAM_PROPRIETARY:
	case ZW_ALRM_PARAM_UNKNOWN:
	default:
		// Printing binary stream. Eg: The 4 byte value 0xAABBCCDD requires 11 bytes for equivalent string [i.e ((4 * 2) + 3) bytes]
		// Extra 3 bytes is to store the characters '0', 'x' and '\0'
		string_size = (event_parameter_length * 2) + 3;
		break;
	}


	if ((string = (char *)ZWS_MALLOC(string_size)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_string_alloc;
	}

	string[0] = '\0';

	switch (event_parameter_type)
	{
	case ZW_ALRM_PARAM_LOC:
		if (zwu_uri_encode_string(string, string_size, event_parameter) != ZWS_STATUS_OK)
		{
			goto l_err_uri_encode;
		}
		break;

	case ZW_ALRM_PARAM_USRID:
		snprintf(string, string_size, "%u", (uint8_t)event_parameter[0]);
		break;

	case ZW_ALRM_PARAM_OEM_ERR_CODE:
	case ZW_ALRM_PARAM_PROPRIETARY:
	case ZW_ALRM_PARAM_UNKNOWN:
	default:
		if (event_parameter_length > 0)
		{
			strcat(string, "0x");
		}

		for (i = 0; i < event_parameter_length; i++)
		{
			snprintf(byte_string, sizeof(byte_string), "%02X", (uint8_t)event_parameter[i]);
			strcat(string, byte_string);
		}
		break;
	}

	*event_parameter_string_p = string;

	return ZWS_STATUS_OK;

l_err_uri_encode:
	ZWS_FREE(string);
l_err_string_alloc:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Free string equivalent of alarm event parameter
 * @param[in]     event_parameter_string     String equivalent of alarm event parameter
 * @return        Nothing
 * @see zws_avi_interfaces_alarm_event_parameter_to_string
 */
static void zws_avi_interfaces_alarm_event_parameter_string_free(char *event_parameter_string)
{
	ZWS_FREE(event_parameter_string);
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
static zws_status_t zws_avi_interfaces_alarm_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	avro_value_t alarm_value_list_value;
	avro_value_t alarm_value_value;
	size_t alarm_value_count;
	int i;
	int64_t utime;
	int32_t alarm_type;
	int32_t alarm_level;
	avro_value_t extended_alarm_info_field;
	avro_value_t extended_alarm_info_value;
	int disc;
	int32_t zensor_net_source_node_id;
	int32_t zwave_alarm_status;
	int32_t zwave_alarm_type;
	int32_t zwave_alarm_event;
	int32_t event_parameter_type;
	void *event_parameter;
	size_t size;
	char *event_parameter_string;

	if (avro_value_get_by_name(arg_value, "alarm_value_list", &alarm_value_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'alarm_value_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (avro_value_get_size(&alarm_value_list_value, &alarm_value_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get alarm value list size");
		goto l_err_alarm_value_list_size;
	}

	for (i = 0; i < alarm_value_count; i++)
	{
		if (avro_value_get_by_index(&alarm_value_list_value, i, &alarm_value_value, NULL) != 0)
		{
			goto l_err_get_alarm_value;
		}

		if ((zw_avro_field_get_long(&alarm_value_value, "utime", &utime) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&alarm_value_value, "alarm_type", &alarm_type) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&alarm_value_value, "alarm_level", &alarm_level) != ZWS_STATUS_OK)
				)
		{
			goto l_err_get_fields;
		}

		if (avro_value_get_by_name(&alarm_value_value, "extended_alarm_info", &extended_alarm_info_field, NULL) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'extended_alarm_info' field from 'AlarmReport' record: %s", avro_strerror());
			goto l_err_get_extended_alarm_info_field;
		}

		if (avro_value_get_discriminant(&extended_alarm_info_field, &disc) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get branch number of 'extended_alarm_info' field: %s", avro_strerror());
			goto l_err_get_extended_alarm_info_disc;
		}

		if (disc == 0)
		{
			if (avro_value_get_current_branch(&extended_alarm_info_field, &extended_alarm_info_value) != 0)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'extended_alarm_info' field in branch '0': %s", avro_strerror());
				goto l_err_get_extended_alarm_info_value;
			}

			if ((zw_avro_field_get_int(&extended_alarm_info_value, "zensor_net_source_node_id", &zensor_net_source_node_id) != ZWS_STATUS_OK)
					|| (zw_avro_field_get_int(&extended_alarm_info_value, "zwave_alarm_status", &zwave_alarm_status) != ZWS_STATUS_OK)
					|| (zw_avro_field_get_int(&extended_alarm_info_value, "zwave_alarm_type", &zwave_alarm_type) != ZWS_STATUS_OK)
					|| (zw_avro_field_get_int(&extended_alarm_info_value, "zwave_alarm_event", &zwave_alarm_event) != ZWS_STATUS_OK)
					|| (zw_avro_field_get_int(&extended_alarm_info_value, "event_parameter_type", &event_parameter_type) != ZWS_STATUS_OK)
					)
			{
				goto l_err_get_extended_alarm_info_fields;
			}

			if (zw_avro_field_get_bytes(&extended_alarm_info_value, "event_parameter", &event_parameter, &size) != ZWS_STATUS_OK)
			{
				goto l_err_get_fields_event_parameter;
			}

			event_parameter_string = NULL;
			if (zws_avi_interfaces_alarm_event_parameter_to_string(event_parameter_type,
					event_parameter, size, &event_parameter_string) != ZWS_STATUS_OK)
			{
				goto l_err_event_parameter_to_string;
			}
		}
		else
		{
			zensor_net_source_node_id = 0;
			zwave_alarm_status = 0;
			zwave_alarm_type = 0;
			zwave_alarm_event = 0;
			size = 0;
			event_parameter_type = 0;
			event_parameter = NULL;
			event_parameter_string = NULL;
		}

		if (zwu_string_buffer_append_format(result,
				"<%s %s=\"%llu\" vtype=\"%u\" level=\"%u\" ext=\"%u\" zensor_nodeid=\"%u\" status=\"%u\" "
				"ztype=\"%u\" event=\"%u\" eparam_len=\"%u\" eparam_type=\"%u\" eparam=\"%s\" />",
				((web_api_version == ZWS_WEB_API_VERSION_1) ? "alarm" : "alrm"),
				((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
				(uint64_t)utime,
				(uint8_t)alarm_type,
				(uint8_t)alarm_level,
				(uint8_t)((disc == 0) ? 1 : 0),
				(uint8_t)zensor_net_source_node_id,
				(uint8_t)zwave_alarm_status,
				(uint8_t)zwave_alarm_type,
				(uint8_t)zwave_alarm_event,
				(uint8_t)size,
				(uint8_t)event_parameter_type,
				((event_parameter_string) ? event_parameter_string : "")
				) != ZWS_STATUS_OK)
		{
			goto l_err_append;
		}

		zws_avi_interfaces_alarm_event_parameter_string_free(event_parameter_string);
		zw_avro_field_free_bytes(event_parameter);
	}

	return ZWS_STATUS_OK;

l_err_append:
	zws_avi_interfaces_alarm_event_parameter_string_free(event_parameter_string);
l_err_event_parameter_to_string:
	zw_avro_field_free_bytes(event_parameter);
l_err_get_fields_event_parameter:
l_err_get_extended_alarm_info_fields:
l_err_get_extended_alarm_info_value:
l_err_get_extended_alarm_info_disc:
l_err_get_extended_alarm_info_field:
l_err_get_fields:
l_err_get_alarm_value:
l_err_alarm_value_list_size:
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
static zws_status_t zws_avi_interfaces_alarm_set_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int ztype;
	int status;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if ((i < args_count) && !strcmp(args[i][0], "ztype")
			&& (zwu_parse_int32_non_negative(args[i][1], &ztype) == ZWU_STATUS_OK)
			&& (zw_avro_field_set_int(arg_value_p, "zwave_alarm_type", ztype) == ZWS_STATUS_OK))
	{
		i++;
		if ((i < args_count) && !strcmp(args[i][0], "status")
				&& (zwu_parse_int32_non_negative(args[i][1], &status) == ZWU_STATUS_OK)
				&& (zw_avro_field_set_int(arg_value_p, "zwave_alarm_status", status) == ZWS_STATUS_OK))
		{
			i++;
			return_status = ZWS_STATUS_OK;
		}
	}

	return return_status;
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
static zws_status_t zws_avi_interfaces_alarm_type_supported_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int64_t utime;
	int32_t have_vendor_type;
	avro_value_t zwave_alarm_type_list_value;
	avro_value_t zwave_alarm_type_value;
	size_t zwave_alarm_type_count;
	int32_t zwave_alarm_type;
	zwu_string_buffer_t zwave_alarm_type_list;
	int i;

	if ((zw_avro_field_get_long(arg_value, "utime", &utime) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "have_vendor_type", &have_vendor_type) != ZWS_STATUS_OK))
	{
		goto l_err_get_fields;
	}

	if (avro_value_get_by_name(arg_value, "supported_types", &zwave_alarm_type_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'supported_types': %s", avro_strerror());
		goto l_err_get_type_list_value;
	}

	if (avro_value_get_size(&zwave_alarm_type_list_value, &zwave_alarm_type_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get alarm value list size");
		goto l_err_alarm_type_list_size;
	}

	if (zwu_string_buffer_create(0, &zwave_alarm_type_list) != ZWS_STATUS_OK)
	{
		goto l_err_alarm_type_list_create;
	}

	for (i = 0; i < zwave_alarm_type_count; i++)
	{
		if (avro_value_get_by_index(&zwave_alarm_type_list_value, i, &zwave_alarm_type_value, NULL) != 0)
		{
			goto l_err_get_alarm_type_value;
		}

		if (avro_value_get_int(&zwave_alarm_type_value, &zwave_alarm_type) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get alarm type: %s", avro_strerror());
			goto l_err_get_alarm_type;
		}

		if (zwu_string_buffer_append_format(zwave_alarm_type_list, "%u,", zwave_alarm_type) != ZWS_STATUS_OK)
		{
			goto l_err_alarm_type_append;
		}
	}

	if (zwu_string_buffer_append_format(result,
			"<%s %s=\"%llu\" %s=\"%u\" list=\"%s\" />",
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "type_sup_alarm" : "alrm_sup"),
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
			(uint64_t)utime,
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "have_vtype" : "has_vtype"),
			(uint8_t)have_vendor_type,
			zwu_string_buffer_get_string(zwave_alarm_type_list)
			) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	zwu_string_buffer_destroy(zwave_alarm_type_list);

	return ZWS_STATUS_OK;

l_err_append:
l_err_alarm_type_append:
l_err_get_alarm_type:
l_err_get_alarm_type_value:
	zwu_string_buffer_destroy(zwave_alarm_type_list);
l_err_alarm_type_list_create:
l_err_alarm_type_list_size:
l_err_get_type_list_value:
l_err_get_fields:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Set Avro value for RPC request for event supported Get command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_alarm_event_supported_get_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int zwave_alarm_type;
	//int j;
	//char *key;
	//char *value;
	//int sub_command;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	zwave_alarm_type = COMMAND_ZWAVE_ALARM_TYPE_NONE;

	if ((i < args_count) && !strcmp(args[i][0], "ztype"))
	{
		if (zwu_parse_int32_non_negative(args[i][1], &zwave_alarm_type) != ZWU_STATUS_OK)
		{
			goto l_err_zwave_alarm_type;
		}
		i++;
	}

	if (zw_avro_field_set_int(arg_value_p, "zwave_alarm_type", zwave_alarm_type) == ZWS_STATUS_OK)
	{
		return_status = ZWS_STATUS_OK;
	}

	return return_status;

l_err_zwave_alarm_type:
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
static zws_status_t zws_avi_interfaces_alarm_event_supported_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int64_t utime;
	avro_value_t zwave_alarm_event_list_value;
	avro_value_t zwave_alarm_event_value;
	size_t zwave_alarm_event_count;
	int32_t zwave_alarm_type;
	int32_t zwave_alarm_event;
	zwu_string_buffer_t zwave_alarm_event_list;
	int i;

	if ((zw_avro_field_get_long(arg_value, "utime", &utime) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "zwave_alarm_type", &zwave_alarm_type) != ZWS_STATUS_OK))
	{
		goto l_err_get_fields;
	}

	if (avro_value_get_by_name(arg_value, "supported_events", &zwave_alarm_event_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'supported_events': %s", avro_strerror());
		goto l_err_get_event_list_value;
	}

	if (avro_value_get_size(&zwave_alarm_event_list_value, &zwave_alarm_event_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get alarm event value list size");
		goto l_err_get_event_list_value;
	}

	if (zwu_string_buffer_create(0, &zwave_alarm_event_list) != ZWS_STATUS_OK)
	{
		goto l_err_alarm_event_list_create;
	}

	for (i = 0; i < zwave_alarm_event_count; i++)
	{
		if (avro_value_get_by_index(&zwave_alarm_event_list_value, i, &zwave_alarm_event_value, NULL) != 0)
		{
			goto l_err_get_alarm_event_value;
		}

		if (avro_value_get_int(&zwave_alarm_event_value, &zwave_alarm_event) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get alarm event: %s", avro_strerror());
			goto l_err_get_alarm_event;
		}

		if (zwu_string_buffer_append_format(zwave_alarm_event_list, "%u,", zwave_alarm_event) != ZWS_STATUS_OK)
		{
			goto l_err_alarm_event_append;
		}
	}

	if (zwu_string_buffer_append_format(result,
			"<%s %s=\"%llu\" ztype=\"%u\" list=\"%s\" />",
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "event_sup_alarm" : "alrm_event_sup"),
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
			(uint64_t)utime,
			(uint8_t)zwave_alarm_type,
			zwu_string_buffer_get_string(zwave_alarm_event_list)
			) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	zwu_string_buffer_destroy(zwave_alarm_event_list);

	return ZWS_STATUS_OK;

l_err_append:
l_err_alarm_event_append:
l_err_get_alarm_event:
l_err_get_alarm_event_value:
	zwu_string_buffer_destroy(zwave_alarm_event_list);
l_err_alarm_event_list_create:
l_err_get_event_list_value:
l_err_get_fields:
	return ZWS_STATUS_ERROR;
}

/** Handlers for Get command */
static zws_avi_message_interfaces_t interfaces_alarm_get =
{
		{"zwif_alrm", "alarm"}, COMMAND_ALARM_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_alarm_get",

		zws_avi_interfaces_alarm_get_argument,
		zws_avi_interfaces_alarm_get_result,

		"tServAlarm"
};

/** Handlers for Set command */
static zws_avi_message_interfaces_t interfaces_alarm_set =
{
		{"zwif_alrm", "alarm"}, COMMAND_ALARM_SET,
		ZWS_AVI_MESSAGE_PASSIVE_NOT_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_alarm_set",

		zws_avi_interfaces_alarm_set_argument,
		NULL,

		"tServAlarm"
};

/** Handlers for Supported Get command */
static zws_avi_message_interfaces_t interfaces_alarm_type_supported_get =
{
		{"zwif_alrm", "alarm"}, COMMAND_ALARM_TYPE_SUPPORTED_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_alarm_type_supported_get",

		NULL,
		zws_avi_interfaces_alarm_type_supported_get_result,

		"tServAlarm"
};

/** Handlers for Event Supported Get command */
static zws_avi_message_interfaces_t interfaces_alarm_event_supported_get =
{
		{"zwif_alrm", "alarm"}, COMMAND_ALARM_EVENT_SUPPORTED_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_alarm_event_supported_get",

		zws_avi_interfaces_alarm_event_supported_get_argument,
		zws_avi_interfaces_alarm_event_supported_get_result,

		"tServAlarm"
};

/** Register command handlers */
zws_status_t zws_avi_interfaces_alarm_init(zwu_array_list_t message_list)
{
	if ((zws_avi_message_list_add_message_interfaces(message_list, &interfaces_alarm_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_alarm_set) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_alarm_type_supported_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_alarm_event_supported_get) != ZWS_STATUS_OK))
	{
		goto l_err_networks_list_get;
	}

	return ZWS_STATUS_OK;

l_err_networks_list_get:
	return ZWS_STATUS_ERROR;
}



