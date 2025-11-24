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
 * @file    zws_avi_interfaces_user_code.c
 *
 * @brief   AVI message 'interface_user_code_xxx'
 * @details Handles the AVI message 'interface_user_code_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-10-03
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message_interfaces.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zwu_number.h"

/** User Code Get */
#define COMMAND_USER_CODE_GET                   1
/** User Code Report */
#define COMMAND_USER_CODE_REPORT                2
/** User Code Set */
#define COMMAND_USER_CODE_SET                   3
/** User Code Users Number Get */
#define COMMAND_USER_CODE_USERS_NUMBER_GET      4
/** User Code Users Number Report */
#define COMMAND_USER_CODE_USERS_NUMBER_REPORT   5

/**
 * @brief Set Avro value for RPC request for Get command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_user_code_get_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int user_identifier;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if ((i < args_count) && !strcmp(args[i][0], "id")
			&& (zwu_parse_int32_non_negative(args[i][1], &user_identifier) == ZWU_STATUS_OK)
			&& (zw_avro_field_set_int(arg_value_p, "user_identifier", user_identifier) == ZWS_STATUS_OK))
	{
		i++;
		return_status = ZWS_STATUS_OK;
	}

	return return_status;
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
static zws_status_t zws_avi_interfaces_user_code_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	avro_value_t user_code_list_value;
	avro_value_t user_code_value;
	size_t user_code_count;
	int i;
	int64_t utime;
	int32_t user_identifier;
	int32_t user_id_status;
	int32_t user_code_length;
	void *user_code;
	size_t user_code_size;
	char user_code_string[11];

	if (avro_value_get_by_name(arg_value, "user_code_list", &user_code_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'user_code_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (avro_value_get_size(&user_code_list_value, &user_code_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get user_code value list size");
		goto l_err_user_code_list_size;
	}

	for (i = 0; i < user_code_count; i++)
	{
		if (avro_value_get_by_index(&user_code_list_value, i, &user_code_value, NULL) != 0)
		{
			goto l_err_get_endpoint_value;
		}

		if ((zw_avro_field_get_long(&user_code_value, "utime", &utime) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&user_code_value, "user_identifier", &user_identifier) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&user_code_value, "user_id_status", &user_id_status) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&user_code_value, "user_code_length", &user_code_length) != ZWS_STATUS_OK)
				)
		{
			goto l_err_get_fields;
		}

		ZWS_LOG(ZWS_LOG_DEBUG, "User Code Length: %d", user_code_length);

		if (zw_avro_field_get_fixed(&user_code_value, "user_code", &user_code, &user_code_size) != ZWS_STATUS_OK)
		{
			goto l_err_get_field_user_code_value;
		}

		// Size validation to prevent underflow/overlfow
		if ((user_code_size != 10) || (user_code_length < 0) || (user_code_length > user_code_size))
		{
			ZWS_LOG(ZWS_LOG_ERR, "Invalid user_code value field size");
			goto l_err_invalid_user_code_size;
		}

		memcpy(user_code_string, user_code, user_code_length);
		user_code_string[user_code_length] = '\0';

		if (zwu_string_buffer_append_format(result,
				"<%s %s=\"%llu\" id=\"%u\" status=\"%u\" code=\"%s\" />",
				((web_api_version == ZWS_WEB_API_VERSION_1) ? "user_code" : "usrcod"),
				((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
				(uint64_t)utime,
				(uint8_t)user_identifier,
				(uint8_t)user_id_status,
				(char *)user_code_string
				) != ZWS_STATUS_OK)
		{
			goto l_err_append;
		}

		zw_avro_field_free_fixed(user_code);
	}

	return ZWS_STATUS_OK;

l_err_append:
l_err_invalid_user_code_size:
	zw_avro_field_free_fixed(user_code);
l_err_get_field_user_code_value:
l_err_get_fields:
l_err_get_endpoint_value:
l_err_user_code_list_size:
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
static zws_status_t zws_avi_interfaces_user_code_set_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	char user_code[11];
	int user_identifier;
	int user_id_status;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if ((i < args_count) && !strcmp(args[i][0], "id")
			&& (zwu_parse_int32_non_negative(args[i][1], &user_identifier) == ZWU_STATUS_OK)
			&& (zw_avro_field_set_int(arg_value_p, "user_identifier", user_identifier) == ZWS_STATUS_OK))
	{
		i++;
		if ((i < args_count) && !strcmp(args[i][0], "status")
				&& (zwu_parse_int32_non_negative(args[i][1], &user_id_status) == ZWU_STATUS_OK)
				&& (zw_avro_field_set_int(arg_value_p, "user_id_status", user_id_status) == ZWS_STATUS_OK))
		{
			i++;
			if ((i < args_count) && !strcmp(args[i][0], "code")
					&& (strlen(args[i][1]) <= 10))
			{
				memset(user_code, 0, sizeof(user_code));

				strncpy(user_code, args[i][1], sizeof(user_code) - 1);
				user_code[sizeof(user_code) - 1] = '\0';

				if (zw_avro_field_set_fixed(arg_value_p, "user_code", user_code, 10) == ZWS_STATUS_OK)
				{
					i++;
					if (zw_avro_field_set_int(arg_value_p, "user_code_length", strlen(user_code)) == ZWS_STATUS_OK)
					{
						return_status = ZWS_STATUS_OK;
					}
				}
			}
		}
	}

	return return_status;
}

/**
 * @brief Get CGI response for Users Number Get command from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_user_code_users_number_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int64_t utime;
	int32_t supported_users;

	if ((zw_avro_field_get_long(arg_value, "utime", &utime) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "supported_users", &supported_users) != ZWS_STATUS_OK)
			)
	{
		goto l_err_get_fields;
	}

	if (zwu_string_buffer_append_format(result,
			"<%s %s=\"%llu\" user_cnt=\"%u\" />",
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "users_user_code" : "usrcod_sup"),
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
			(uint64_t)utime,
			(uint8_t)supported_users
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
static zws_avi_message_interfaces_t interfaces_user_code_get =
{
		{"zwif_usrcod", "user_code"}, COMMAND_USER_CODE_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_NOT_SUPPORTED,

		"interface_user_code_get",

		zws_avi_interfaces_user_code_get_argument,
		zws_avi_interfaces_user_code_get_result,

		"tServUserCode"
};

/** Handlers for Set command */
static zws_avi_message_interfaces_t interfaces_user_code_set =
{
		{"zwif_usrcod", "user_code"}, COMMAND_USER_CODE_SET,
		ZWS_AVI_MESSAGE_PASSIVE_NOT_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_NOT_SUPPORTED,

		"interface_user_code_set",

		zws_avi_interfaces_user_code_set_argument,
		NULL,

		"tServUserCode"
};

/** Handlers for Users Number Get command */
static zws_avi_message_interfaces_t interfaces_user_code_users_number_get =
{
		{"zwif_usrcod", "user_code"}, COMMAND_USER_CODE_USERS_NUMBER_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_NOT_SUPPORTED,

		"interface_user_code_users_number_get",

		NULL,
		zws_avi_interfaces_user_code_users_number_get_result,

		"tServUserCode"
};

/** Register command handlers */
zws_status_t zws_avi_interfaces_user_code_init(zwu_array_list_t message_list)
{
	if ((zws_avi_message_list_add_message_interfaces(message_list, &interfaces_user_code_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_user_code_set) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_user_code_users_number_get) != ZWS_STATUS_OK))
	{
		goto l_err_networks_list_get;
	}

	return ZWS_STATUS_OK;

l_err_networks_list_get:
	return ZWS_STATUS_ERROR;
}


