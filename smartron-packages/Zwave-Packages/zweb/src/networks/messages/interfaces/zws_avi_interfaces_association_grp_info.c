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
 * @file    zws_avi_interfaces_association_grp_info.c
 *
 * @brief   AVI message 'interface_association_grp_info_xxx'
 * @details Handles the AVI message 'interface_association_grp_info_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-02-19
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message_interfaces.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zwu_encoding.h"

/** Association Group Info Get */
#define COMMAND_ASSOCIATION_GRP_INFO_GET                1

/**
 * @brief Get CGI response for Get command from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_association_grp_info_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int32_t dynamic_info;
	int32_t group_count;
	avro_value_t association_grp_info_list_value;
	avro_value_t association_grp_info_value;
	size_t association_grp_info_count;
	int i;
	int32_t grouping_identifier;
	int32_t profile;
	int32_t event_code;
	char *group_name;
	char *group_name_encoded;
	size_t string_size;
	avro_value_t command_list_value;
	avro_value_t command_value;
	size_t commands_count;
	int j;
	char *command_class_name;
	char *command_name;

	if ((zw_avro_field_get_int(arg_value, "dynamic_info", &dynamic_info) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "group_count", &group_count) != ZWS_STATUS_OK)
			)
	{
		goto l_err_get_fields;
	}

	if (avro_value_get_by_name(arg_value, "association_grp_info_list", &association_grp_info_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'association_grp_info_list': %s", avro_strerror());
		goto l_err_get_association_grp_info_list_value;
	}

	if (avro_value_get_size(&association_grp_info_list_value, &association_grp_info_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get association grp info list size");
		goto l_err_association_grp_info_list_size;
	}

	if (zwu_string_buffer_append_format(result,
			"<%s dynamic=\"%u\" group_cnt=\"%u\" valid_grp_cnt=\"%u\" >",
			"group_info_list",
			(uint8_t)dynamic_info,
			(uint16_t)group_count,
			(uint16_t)association_grp_info_count
			) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	for (i = 0; i < association_grp_info_count; i++)
	{
		if (avro_value_get_by_index(&association_grp_info_list_value, i, &association_grp_info_value, NULL) != 0)
		{
			goto l_err_get_association_grp_info_value;
		}

		if ((zw_avro_field_get_int(&association_grp_info_value, "grouping_identifier", &grouping_identifier) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&association_grp_info_value, "profile", &profile) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&association_grp_info_value, "event_code", &event_code) != ZWS_STATUS_OK)
				)
		{
			goto l_err_get_association_grp_info_fields;
		}

		if (zw_avro_field_get_string(&association_grp_info_value, "name", &group_name) != ZWS_STATUS_OK)
		{
			goto l_err_get_group_name_field;
		}

		string_size = (strlen(group_name) * 3) + 1;
		if ((group_name_encoded = (char *)ZWS_MALLOC(string_size)) == NULL)
		{
			ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
			zw_avro_field_free_string(group_name);
			goto l_err_group_name_encoded_alloc;
		}

		if (zwu_uri_encode_string(group_name_encoded, string_size, group_name) != ZWS_STATUS_OK)
		{
			ZWS_FREE(group_name_encoded);
			zw_avro_field_free_string(group_name);
			goto l_err_encode_group_name;
		}

		if (zwu_string_buffer_append_format(result,
				"<%s group=\"%u\" profile=\"%u\" evt_code=\"%u\" name=\"%s\" >",
				"group_info",
				(uint8_t)grouping_identifier,
				(uint16_t)profile,
				(uint16_t)event_code,
				group_name_encoded
				) != ZWS_STATUS_OK)
		{
			ZWS_FREE(group_name_encoded);
			zw_avro_field_free_string(group_name);

			goto l_err_append_association_grp_info;
		}

		ZWS_FREE(group_name_encoded);
		zw_avro_field_free_string(group_name);

		if (avro_value_get_by_name(&association_grp_info_value, "command_list", &command_list_value, NULL) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'command_list': %s", avro_strerror());
			goto l_err_get_command_list_value;
		}

		if (avro_value_get_size(&command_list_value, &commands_count) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get commands list size");
			goto l_err_command_list_size;
		}

		for (j = 0; j < commands_count; j++)
		{
			if (avro_value_get_by_index(&command_list_value, j, &command_value, NULL) != 0)
			{
				goto l_err_get_command_value;
			}

			if (zw_avro_field_get_string(&command_value, "command_class_name", &command_class_name) != ZWS_STATUS_OK)
			{
				goto l_err_get_command_class_name_field;
			}

			if (zw_avro_field_get_string(&command_value, "command_name", &command_name) != ZWS_STATUS_OK)
			{
				zw_avro_field_free_string(command_class_name);
				goto l_err_get_command_name_field;
			}

			if (zwu_string_buffer_append_format(result,
					"<%s intf_name=\"%s\" intf_cmd_name=\"%s\" />",
					"group_cmd",
					command_class_name,
					command_name
					) != ZWS_STATUS_OK)
			{
				zw_avro_field_free_string(command_name);
				zw_avro_field_free_string(command_class_name);

				goto l_err_append_command;
			}

			zw_avro_field_free_string(command_name);
			zw_avro_field_free_string(command_class_name);
		}

		if (zwu_string_buffer_append_format(result,
				"</%s>",
				"group_info"
				) != ZWS_STATUS_OK)
		{
			goto l_err_append_association_grp_info_end;
		}
	}

	if (zwu_string_buffer_append_format(result,
			"</%s>",
			"group_info_list"
			) != ZWS_STATUS_OK)
	{
		goto l_err_append_end;
	}

	return ZWS_STATUS_OK;

l_err_append_end:
l_err_append_association_grp_info_end:
l_err_append_command:
l_err_get_command_name_field:
l_err_get_command_class_name_field:
l_err_get_command_value:
l_err_command_list_size:
l_err_get_command_list_value:
l_err_append_association_grp_info:
l_err_encode_group_name:
l_err_group_name_encoded_alloc:
l_err_get_group_name_field:
l_err_get_association_grp_info_fields:
l_err_get_association_grp_info_value:
l_err_append:
l_err_association_grp_info_list_size:
l_err_get_association_grp_info_list_value:
l_err_get_fields:
	return ZWS_STATUS_ERROR;
}

/** Handlers for Get command */
static zws_avi_message_interfaces_t interfaces_association_grp_info_get =
{
		{"zwif_group_info", NULL}, COMMAND_ASSOCIATION_GRP_INFO_GET,
		ZWS_AVI_MESSAGE_PASSIVE_NOT_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_NOT_SUPPORTED,

		"interface_association_grp_info_get",

		NULL,
		zws_avi_interfaces_association_grp_info_get_result,

		"tServAssocGrpInfo"
};

/** Register command handlers */
zws_status_t zws_avi_interfaces_association_grp_info_init(zwu_array_list_t message_list)
{
	if (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_association_grp_info_get) != ZWS_STATUS_OK)
	{
		goto l_err_networks_list_get;
	}

	return ZWS_STATUS_OK;

l_err_networks_list_get:
	return ZWS_STATUS_ERROR;
}


