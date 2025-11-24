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
 * @file    zws_avi_interfaces_switch_multilevel.c
 *
 * @brief   AVI message 'interface_switch_multilevel_xxx'
 * @details Handles the AVI message 'interface_switch_multilevel_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-09-17
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message_interfaces.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zwu_number.h"

/** Switch Multilevel Setup */
#define COMMAND_SWITCH_MULTILEVEL_SETUP              1
/** Switch Multilevel Get */
#define COMMAND_SWITCH_MULTILEVEL_GET                2
/** Switch Multilevel Report */
#define COMMAND_SWITCH_MULTILEVEL_REPORT             3
/** Switch Multilevel Set */
#define COMMAND_SWITCH_MULTILEVEL_SET                4

/** Switch Multilevel Level Change Get */
#define COMMAND_SWITCH_MULTILEVEL_LEVEL_CHANGE_GET   5
/** Switch Multilevel Level Change Set */
#define COMMAND_SWITCH_MULTILEVEL_LEVEL_CHANGE_SET   6

/** Switch Multilevel Supported Get */
#define COMMAND_SWITCH_MULTILEVEL_SUPPORTED_GET      7
/** Switch Multilevel Supported Report */
#define COMMAND_SWITCH_MULTILEVEL_SUPPORTED_REPORT   8

/** Switch Multilevel Start Level not specified */
#define COMMAND_SWITCH_MULTILEVEL_START_LEVEL_NONE              (-1)
/** Switch Multilevel Dimming Duration not specified */
#define COMMAND_SWITCH_MULTILEVEL_DIMMING_DURATION_NONE         (-1)
/** Switch Multilevel Inc/Dec not specified */
#define COMMAND_SWITCH_MULTILEVEL_INC_DEC_NONE                  (-1)
/** Switch Multilevel Step Size not specified */
#define COMMAND_SWITCH_MULTILEVEL_STEP_SIZE_NONE                (-1)

/**
 * @brief Get CGI response for Get command from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_switch_multilevel_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int64_t utime;
	int32_t value;

	if ((zw_avro_field_get_long(arg_value, "utime", &utime) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "value", &value) != ZWS_STATUS_OK)
			)
	{
		goto l_err_get_fields;
	}

	if (zwu_string_buffer_append_format(result,
			"<%s %s=\"%llu\" state=\"%u\" />",
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "multi_lvl_sw" : "level"),
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
			(uint64_t)utime,
			(uint8_t)value
			) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	return ZWS_STATUS_OK;

l_err_append:
l_err_get_fields:
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
static zws_status_t zws_avi_interfaces_switch_multilevel_set_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int value;
	int dimming_duration;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if ((i < args_count) && !strcmp(args[i][0], "value")
			&& (zwu_parse_int32_non_negative(args[i][1], &value) == ZWU_STATUS_OK)
			&& (zw_avro_field_set_int(arg_value_p, "value", value) == ZWS_STATUS_OK))
	{
		i++;

		dimming_duration = COMMAND_SWITCH_MULTILEVEL_DIMMING_DURATION_NONE;
		if ((i < args_count) && !strcmp(args[i][0], "dur"))
		{
			if (zwu_parse_int32_non_negative(args[i][1], &dimming_duration) != ZWU_STATUS_OK)
			{
				goto l_err_dimming_duration;
			}
			i++;
		}

		if (zw_avro_field_set_int(arg_value_p, "dimming_duration", dimming_duration) == ZWS_STATUS_OK)
		{
			return_status = ZWS_STATUS_OK;
		}
	}

l_err_dimming_duration:
	return return_status;
}

/**
 * @brief Get CGI response for Level Change Get command from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_switch_multilevel_level_change_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int64_t utime;
	int32_t start_stop;

	if ((zw_avro_field_get_long(arg_value, "utime", &utime) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "start_stop", &start_stop) != ZWS_STATUS_OK)
			)
	{
		goto l_err_get_fields;
	}

	if (zwu_string_buffer_append_format(result,
			"<%s status=\"%u\" />",
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "lvl_chg_multi_lvl_sw" : "level_change"),
			(uint8_t)start_stop
			) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	return ZWS_STATUS_OK;

l_err_append:
l_err_get_fields:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Set Avro value for RPC request for Level Change Set command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_switch_multilevel_level_change_set_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int start_stop;
	int up_down;
	int ignore_start_level;
	int start_level;
	int dimming_duration;
	int inc_dec;
	int step_size;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if ((i < args_count) && !strcmp(args[i][0], "start_stop")
			&& (zwu_parse_int32_non_negative(args[i][1], &start_stop) == ZWU_STATUS_OK)
			&& (zw_avro_field_set_int(arg_value_p, "start_stop", start_stop) == ZWS_STATUS_OK))
	{
		i++;

		if (start_stop == 1)
		{
			if ((i < args_count) && !strcmp(args[i][0], "dir")
					&& (zwu_parse_int32_non_negative(args[i][1], &up_down) == ZWU_STATUS_OK)
					&& (zw_avro_field_set_int(arg_value_p, "up_down", up_down) == ZWS_STATUS_OK))
			{
				i++;

				if ((i < args_count) && !strcmp(args[i][0], "ignore_start_lvl")
						&& (zwu_parse_int32_non_negative(args[i][1], &ignore_start_level) == ZWU_STATUS_OK)
						&& (zw_avro_field_set_int(arg_value_p, "ignore_start_level", ignore_start_level) == ZWS_STATUS_OK))
				{
					i++;

					start_level = COMMAND_SWITCH_MULTILEVEL_START_LEVEL_NONE;
					if ((i < args_count) && !strcmp(args[i][0], "start_lvl")
							&& (zwu_parse_int32_non_negative(args[i][1], &start_level) == ZWU_STATUS_OK))
					{
						i++;

						if (ignore_start_level == 0)
						{
							return_status = ZWS_STATUS_OK;
						}
					}

					if (ignore_start_level != 0)
					{
						return_status = ZWS_STATUS_OK;
					}

					dimming_duration = COMMAND_SWITCH_MULTILEVEL_DIMMING_DURATION_NONE;
					if ((i < args_count) && !strcmp(args[i][0], "dur"))
					{
						if (zwu_parse_int32_non_negative(args[i][1], &dimming_duration) != ZWU_STATUS_OK)
						{
							return_status = ZWS_STATUS_ERROR;
							goto l_err_dimming_duration;
						}
						i++;
					}

					inc_dec = COMMAND_SWITCH_MULTILEVEL_INC_DEC_NONE;
					if ((i < args_count) && !strcmp(args[i][0], "sec"))
					{
						if (zwu_parse_int32_non_negative(args[i][1], &inc_dec) != ZWU_STATUS_OK)
						{
							return_status = ZWS_STATUS_ERROR;
							goto l_err_inc_dec;
						}
						i++;
					}

					step_size = COMMAND_SWITCH_MULTILEVEL_STEP_SIZE_NONE;
					if ((i < args_count) && !strcmp(args[i][0], "step"))
					{
						if (zwu_parse_int32_non_negative(args[i][1], &step_size) != ZWU_STATUS_OK)
						{
							return_status = ZWS_STATUS_ERROR;
							goto l_err_step_size;
						}
						i++;
					}

					if ((zw_avro_field_set_int(arg_value_p, "start_level", start_level) != ZWS_STATUS_OK)
							|| (zw_avro_field_set_int(arg_value_p, "dimming_duration", dimming_duration) != ZWS_STATUS_OK)
							|| (zw_avro_field_set_int(arg_value_p, "inc_dec", inc_dec) != ZWS_STATUS_OK)
							|| (zw_avro_field_set_int(arg_value_p, "step_size", step_size) != ZWS_STATUS_OK))
					{
						return_status = ZWS_STATUS_ERROR;
						goto l_err_set_value;
					}
				}
			}
		}
		else
		{
			return_status = ZWS_STATUS_OK;
		}
	}

l_err_set_value:
l_err_step_size:
l_err_inc_dec:
l_err_dimming_duration:
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
static zws_status_t zws_avi_interfaces_switch_multilevel_supported_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int64_t utime;
	int32_t primary_switch_type;
	int32_t secondary_switch_type;

	if ((zw_avro_field_get_long(arg_value, "utime", &utime) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "primary_switch_type", &primary_switch_type) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "secondary_switch_type", &secondary_switch_type) != ZWS_STATUS_OK)
			)
	{
		goto l_err_get_fields;
	}

	if (zwu_string_buffer_append_format(result,
			"<%s %s=\"%llu\" pri_type=\"%u\" sec_type=\"%u\" />",
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "sup_multi_lvl_sw" : "level_sup"),
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
			(uint64_t)utime,
			(uint8_t)primary_switch_type,
			(uint8_t)secondary_switch_type
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
static zws_avi_message_interfaces_t interfaces_switch_multilevel_get =
{
		{"zwif_level", "multi_lvl_sw"}, COMMAND_SWITCH_MULTILEVEL_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_switch_multilevel_get",

		NULL,
		zws_avi_interfaces_switch_multilevel_get_result,

		"tServMultiLvlSw"
};

/** Handlers for Set command */
static zws_avi_message_interfaces_t interfaces_switch_multilevel_set =
{
		{"zwif_level", "multi_lvl_sw"}, COMMAND_SWITCH_MULTILEVEL_SET,
		ZWS_AVI_MESSAGE_PASSIVE_NOT_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_switch_multilevel_set",

		zws_avi_interfaces_switch_multilevel_set_argument,
		NULL,

		"tServMultiLvlSw"
};

/** Handlers for Level Change Get command */
static zws_avi_message_interfaces_t interfaces_switch_multilevel_level_change_get =
{
		{"zwif_level", "multi_lvl_sw"}, COMMAND_SWITCH_MULTILEVEL_LEVEL_CHANGE_GET,
		ZWS_AVI_MESSAGE_PASSIVE_NOT_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_switch_multilevel_level_change_get",

		NULL,
		zws_avi_interfaces_switch_multilevel_level_change_get_result,

		"tServMultiLvlSw"
};

/** Handlers for Level Change Set command */
static zws_avi_message_interfaces_t interfaces_switch_multilevel_level_change_set =
{
		{"zwif_level", "multi_lvl_sw"}, COMMAND_SWITCH_MULTILEVEL_LEVEL_CHANGE_SET,
		ZWS_AVI_MESSAGE_PASSIVE_NOT_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_switch_multilevel_level_change_set",

		zws_avi_interfaces_switch_multilevel_level_change_set_argument,
		NULL,

		"tServMultiLvlSw"
};

/** Handlers for Supported Get command */
static zws_avi_message_interfaces_t interfaces_switch_multilevel_supported_get =
{
		{"zwif_level", "multi_lvl_sw"}, COMMAND_SWITCH_MULTILEVEL_SUPPORTED_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_switch_multilevel_supported_get",

		NULL,
		zws_avi_interfaces_switch_multilevel_supported_get_result,

		"tServMultiLvlSw"
};

/** Register command handlers */
zws_status_t zws_avi_interfaces_switch_multilevel_init(zwu_array_list_t message_list)
{
	if ((zws_avi_message_list_add_message_interfaces(message_list, &interfaces_switch_multilevel_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_switch_multilevel_set) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_switch_multilevel_level_change_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_switch_multilevel_level_change_set) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_switch_multilevel_supported_get) != ZWS_STATUS_OK))
	{
		goto l_err_networks_list_get;
	}

	return ZWS_STATUS_OK;

l_err_networks_list_get:
	return ZWS_STATUS_ERROR;
}



