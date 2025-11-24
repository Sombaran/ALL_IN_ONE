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
 * @file    zws_avi_interfaces_association.c
 *
 * @brief   AVI message 'interface_association_xxx'
 * @details Handles the AVI message 'interface_association_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-02-14
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message_interfaces.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zwu_number.h"

/** Association Get */
#define COMMAND_ASSOCIATION_GET                     1
/** Association Report */
#define COMMAND_ASSOCIATION_REPORT                  2
/** Association Set */
#define COMMAND_ASSOCIATION_SET                     3   /* ASSOCIATION_REMOVE is also handled by this option */
/** Association Groupings Get */
#define COMMAND_ASSOCIATION_GROUPINGS_GET           4
/** Association Groupings Report */
#define COMMAND_ASSOCIATION_GROUPINGS_REPORT        5
/** Association Specific Group Get */
#define COMMAND_ASSOCIATION_SPECIFIC_GROUP_GET      6
/** Association Specific Group Report */
#define COMMAND_ASSOCIATION_SPECIFIC_GROUP_REPORT   7

/** Association Groupings Identifier not specified */
#define COMMAND_ASSOCIATION_GROUPINGS_IDENTIFIER_NONE         (-1)

/**
 * @brief Set Avro value for RPC request for Get command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_association_get_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int grouping_identifier;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	grouping_identifier = COMMAND_ASSOCIATION_GROUPINGS_IDENTIFIER_NONE;
	if ((i < args_count) && !strcmp(args[i][0], "group_id"))
	{
		if (zwu_parse_int32_non_negative(args[i][1], &grouping_identifier) != ZWU_STATUS_OK)
		{
			goto l_err_grouping_identifier;
		}
		i++;
	}

	if (zw_avro_field_set_int(arg_value_p, "grouping_identifier", grouping_identifier) == ZWS_STATUS_OK)
	{
		return_status = ZWS_STATUS_OK;
	}

	return return_status;

l_err_grouping_identifier:
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
static zws_status_t zws_avi_interfaces_association_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	avro_value_t association_value_list_value;
	avro_value_t association_value_value;
	size_t association_value_count;
	int i;
	int64_t utime;
	int32_t grouping_identifier;
	int32_t max_nodes_supported;
	avro_value_t endpoints_list_value;
	size_t endpoints_count;
	zwu_string_buffer_t endpoints_list;
	avro_value_t endpoint_value;
	avro_value_t node_value;
	int j;
	int32_t node_id;
	int32_t endpoint_id;

	if (avro_value_get_by_name(arg_value, "association_value_list", &association_value_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'association_value_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (avro_value_get_size(&association_value_list_value, &association_value_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get association value list size");
		goto l_err_association_value_list_size;
	}

	for (i = 0; i < association_value_count; i++)
	{
		if (avro_value_get_by_index(&association_value_list_value, i, &association_value_value, NULL) != 0)
		{
			goto l_err_get_association_value;
		}

		if ((zw_avro_field_get_long(&association_value_value, "utime", &utime) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&association_value_value, "grouping_identifier", &grouping_identifier) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&association_value_value, "max_nodes_supported", &max_nodes_supported) != ZWS_STATUS_OK)
				)
		{
			goto l_err_get_fields;
		}

		if (avro_value_get_by_name(&association_value_value, "endpoints_list", &endpoints_list_value, NULL) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'endpoints_list': %s", avro_strerror());
			goto l_err_get_endpoints_list_value;
		}

		if (avro_value_get_size(&endpoints_list_value, &endpoints_count) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get endpoints list size");
			goto l_err_endpoints_list_size;
		}

		if (zwu_string_buffer_create(0, &endpoints_list) != ZWS_STATUS_OK)
		{
			goto l_err_endpoints_list_create;
		}

		for (j = 0; j < endpoints_count; j++)
		{
			if (avro_value_get_by_index(&endpoints_list_value, j, &endpoint_value, NULL) != 0)
			{
				goto l_err_get_endpoint_value;
			}

			if (avro_value_get_by_name(&endpoint_value, "node", &node_value, NULL) != 0)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'node': %s", avro_strerror());
				goto l_err_get_node_value;
			}

			if ((zw_avro_field_get_int(&endpoint_value, "id", &endpoint_id) != ZWS_STATUS_OK)
					|| (zw_avro_field_get_int(&node_value, "id", &node_id) != ZWS_STATUS_OK)
					)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Failed to get endpoint: %s", avro_strerror());
				goto l_err_get_endpoint;
			}

			if (zwu_string_buffer_append_format(endpoints_list, "%u|%u,", node_id, endpoint_id) != ZWS_STATUS_OK)
			{
				goto l_err_endpoint_append;
			}
		}

		if (zwu_string_buffer_append_format(result,
				"<%s %s=\"%llu\" group=\"%u\" max_cnt=\"%u\" ep_cnt=\"%u\" ep_list=\"%s\" />",
				((web_api_version == ZWS_WEB_API_VERSION_1) ? "assoc" : "group"),
				((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
				(uint64_t)utime,
				(uint8_t)grouping_identifier,
				(uint8_t)max_nodes_supported,
				(uint8_t)endpoints_count,
				zwu_string_buffer_get_string(endpoints_list)
				) != ZWS_STATUS_OK)
		{
			goto l_err_append;
		}

		zwu_string_buffer_destroy(endpoints_list);
	}

	return ZWS_STATUS_OK;

l_err_append:

l_err_endpoint_append:
l_err_get_endpoint:
l_err_get_node_value:
l_err_get_endpoint_value:
	zwu_string_buffer_destroy(endpoints_list);
l_err_endpoints_list_create:
l_err_endpoints_list_size:
l_err_get_endpoints_list_value:

l_err_get_fields:
l_err_get_association_value:
l_err_association_value_list_size:
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
static zws_status_t zws_avi_interfaces_association_set_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	avro_value_t endpoints_list_value;
	avro_value_t endpoint_value;
	avro_value_t node_value;
	int member_count;
	int j;
	int add_del;
	int grouping_identifier;
	int32_t node_id, ep_id;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if (avro_value_get_by_name(arg_value_p, "endpoints_list", &endpoints_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'endpoints_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if ((i < args_count) && !strcmp(args[i][0], "add_del")
			&& (zwu_parse_int32_non_negative(args[i][1], &add_del) == ZWU_STATUS_OK)
			&& (zw_avro_field_set_int(arg_value_p, "action", add_del) == ZWS_STATUS_OK))
	{
		i++;
		if ((i < args_count) && !strcmp(args[i][0], "group_id")
				&& (zwu_parse_int32_non_negative(args[i][1], &grouping_identifier) == ZWU_STATUS_OK)
				&& (zw_avro_field_set_int(arg_value_p, "grouping_identifier", grouping_identifier) == ZWS_STATUS_OK))
		{
			i++;
			if ((i < args_count) && !strcmp(args[i][0], "member_cnt")
					&& (zwu_parse_int32_non_negative(args[i][1], &member_count) == ZWU_STATUS_OK))
			{
				i++;

				j = 0;

				while((j < member_count) && (i < args_count))
				{
					if(!(!strcmp(args[i][0], "node_id")
							&& (zwu_parse_int32_non_negative(args[i][1], &node_id) == ZWU_STATUS_OK)
							&& (zwu_number_is_uint8(node_id) == ZWU_STATUS_OK)))
					{
						goto l_err_get_node_id;
					}
					i++;

					if(!(!strcmp(args[i][0], "ep_id")
							&& (zwu_parse_int32_non_negative(args[i][1], &ep_id) == ZWU_STATUS_OK)
							&& (zwu_number_is_uint8(ep_id) == ZWU_STATUS_OK)))
					{
						goto l_err_get_ep_id;
					}

					if (avro_value_append(&endpoints_list_value, &endpoint_value, 0) != 0)
					{
						ZWS_LOG(ZWS_LOG_ERR, "Failed to append to 'endpoints_list': %s", avro_strerror());
						goto l_err_append_endpoint_value;
					}

					if (avro_value_get_by_name(&endpoint_value, "node", &node_value, NULL) != 0)
					{
						ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'node': %s", avro_strerror());
						goto l_err_get_node_value;
					}

					if((zw_avro_field_set_int(&endpoint_value, "id", ep_id) != ZWS_STATUS_OK)
							|| (zw_avro_field_set_int(&node_value, "id", node_id) != ZWS_STATUS_OK))
					{
						goto l_err_set_endpoint_value;
					}

					i++;
					j++;
				}

				return_status = ZWS_STATUS_OK;
			}
		}
	}

l_err_set_endpoint_value:
l_err_get_node_value:
l_err_append_endpoint_value:
l_err_get_ep_id:
l_err_get_node_id:
l_err_get_list_value:
	return return_status;
}

/**
 * @brief Get CGI response for Groupings Get command from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_association_groupings_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int64_t utime;
	int32_t supported_groupings;

	if ((zw_avro_field_get_long(arg_value, "utime", &utime) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "supported_groupings", &supported_groupings) != ZWS_STATUS_OK))
	{
		goto l_err_get_fields;
	}

	if (zwu_string_buffer_append_format(result,
			"<%s %s=\"%llu\" grp_cnt=\"%u\" />",
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "grp_sup_assoc" : "group_sup"),
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
			(uint64_t)utime,
			(uint8_t)supported_groupings
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
 * @brief Get CGI response for Specific Group Get command from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_association_specific_group_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int64_t utime;
	int32_t group;

	if ((zw_avro_field_get_long(arg_value, "utime", &utime) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "group", &group) != ZWS_STATUS_OK))
	{
		goto l_err_get_fields;
	}

	if (zwu_string_buffer_append_format(result,
			"<%s %s=\"%llu\" grp_actv=\"%u\" />",
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "grp_spec_assoc" : "group_actv"),
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
			(uint64_t)utime,
			(uint8_t)group
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
static zws_avi_message_interfaces_t interfaces_association_get =
{
		{"zwif_group", "assoc"}, COMMAND_ASSOCIATION_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_NOT_SUPPORTED,

		"interface_association_get",

		zws_avi_interfaces_association_get_argument,
		zws_avi_interfaces_association_get_result,

		"tServAssoc"
};

/** Handlers for Set command */
static zws_avi_message_interfaces_t interfaces_association_set =
{
		{"zwif_group", "assoc"}, COMMAND_ASSOCIATION_SET,
		ZWS_AVI_MESSAGE_PASSIVE_NOT_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_NOT_SUPPORTED,

		"interface_association_set",

		zws_avi_interfaces_association_set_argument,
		NULL,

		"tServAssoc"
};

/** Handlers for Groupings Get command */
static zws_avi_message_interfaces_t interfaces_association_groupings_get =
{
		{"zwif_group", "assoc"}, COMMAND_ASSOCIATION_GROUPINGS_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_NOT_SUPPORTED,

		"interface_association_groupings_get",

		NULL,
		zws_avi_interfaces_association_groupings_get_result,

		"tServAssoc"
};

/** Handlers for Specific Group Get command */
static zws_avi_message_interfaces_t interfaces_association_specific_group_get =
{
		{"zwif_group", "assoc"}, COMMAND_ASSOCIATION_SPECIFIC_GROUP_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_NOT_SUPPORTED,

		"interface_association_specific_group_get",

		NULL,
		zws_avi_interfaces_association_specific_group_get_result,

		"tServAssoc"
};

/** Register command handlers */
zws_status_t zws_avi_interfaces_association_init(zwu_array_list_t message_list)
{
	if ((zws_avi_message_list_add_message_interfaces(message_list, &interfaces_association_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_association_set) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_association_groupings_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_association_specific_group_get) != ZWS_STATUS_OK))
	{
		goto l_err_networks_list_get;
	}

	return ZWS_STATUS_OK;

l_err_networks_list_get:
	return ZWS_STATUS_ERROR;
}



