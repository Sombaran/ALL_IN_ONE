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
 * @file    zws_avi_nodes_endpoint_list.c
 *
 * @brief   AVI message 'node_endpoint_list_get'
 * @details Handles the AVI message 'node_endpoint_list_get'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-26
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message_nodes.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zwu_encoding.h"

/**
 * @brief Get CGI response for endpoint list from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_nodes_endpoint_list_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	avro_value_t endpoint_list_value;
	avro_value_t endpoint_value;
	size_t endpoint_count;
	int i, j;
	int32_t endpoint_id;
	int32_t generic_device_class;
	int32_t specific_device_class;
	char *name;
	char *location;
	char *name_encoded;
	char *location_encoded;
	size_t string_size;
	int32_t zwave_plus_version;
	int32_t zwave_plus_role_type;
	int32_t zwave_plus_node_type;
	int32_t zwave_plus_installer_icon_type;
	int32_t zwave_plus_user_icon_type;
	avro_value_t aggregated_ep_list_value;
	avro_value_t aggregated_ep_value;
	int32_t aggregated_ep_cnt;
	size_t aggregated_ep_cnt_from_list;
	zwu_string_buffer_t aggregated_ep_list;
	int32_t aggregated_ep;

	if (avro_value_get_by_name(arg_value, "endpoint_list", &endpoint_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'endpoint_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (avro_value_get_size(&endpoint_list_value, &endpoint_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get endpoint list size");
		goto l_err_endpoint_list_size;
	}

	for (i = 0; i < endpoint_count; i++)
	{
		if (avro_value_get_by_index(&endpoint_list_value, i, &endpoint_value, NULL) != 0)
		{
			goto l_err_get_endpoint_value;
		}

		if ((zw_avro_field_get_int(&endpoint_value, "id", &endpoint_id) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&endpoint_value, "generic_device_class", &generic_device_class) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&endpoint_value, "specific_device_class", &specific_device_class) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&endpoint_value, "zwave_plus_version", &zwave_plus_version) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&endpoint_value, "zwave_plus_role_type", &zwave_plus_role_type) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&endpoint_value, "zwave_plus_node_type", &zwave_plus_node_type) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&endpoint_value, "zwave_plus_installer_icon_type", &zwave_plus_installer_icon_type) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&endpoint_value, "zwave_plus_user_icon_type", &zwave_plus_user_icon_type) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&endpoint_value, "aggregated_ep_cnt", &aggregated_ep_cnt) != ZWS_STATUS_OK)
				)
		{
			goto l_err_get_fields;
		}

		if (avro_value_get_by_name(&endpoint_value, "aggregated_ep_list", &aggregated_ep_list_value, NULL) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'aggregated_ep_list_value': %s", avro_strerror());
			goto l_err_get_aggregated_ep_list_value;
		}

		if (avro_value_get_size(&aggregated_ep_list_value, &aggregated_ep_cnt_from_list) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get aggregated_ep_cnt value list size");
			goto l_err_aggregated_ep_list_size;
		}

		if (zwu_string_buffer_create(0, &aggregated_ep_list) != ZWS_STATUS_OK)
		{
			goto l_err_aggregated_ep_list_list_create;
		}

		for (j = 0; j < aggregated_ep_cnt_from_list; j++)
		{
			if (avro_value_get_by_index(&aggregated_ep_list_value, j, &aggregated_ep_value, NULL) != 0)
			{
				goto l_err_get_aggregated_ep_value_value;
			}

			if (avro_value_get_int(&aggregated_ep_value, &aggregated_ep) != ZWS_STATUS_OK)
			{
				goto l_err_get_field_aggregated_ep_value;
			}

			if (zwu_string_buffer_append_format(aggregated_ep_list, "%u,", aggregated_ep) != ZWS_STATUS_OK)
			{
				goto l_err_aggregated_ep_list_get_append;
			}
		}

		if (zw_avro_field_get_string(&endpoint_value, "name", &name) != ZWS_STATUS_OK)
		{
			goto l_err_get_field_name;
		}

		if (zw_avro_field_get_string(&endpoint_value, "location", &location) != ZWS_STATUS_OK)
		{
			goto l_err_get_field_location;
		}

		string_size = (strlen(name) * 3) + 1;
		if ((name_encoded = (char *)ZWS_MALLOC(string_size)) == NULL)
		{
			ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
			goto l_err_name_encoded_alloc;
		}

		if (zwu_uri_encode_string(name_encoded, string_size, name) != ZWS_STATUS_OK)
		{
			goto l_err_encode_name;
		}

		string_size = (strlen(location) * 3) + 1;
		if ((location_encoded = (char *)ZWS_MALLOC(string_size)) == NULL)
		{
			ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
			goto l_err_location_encoded_alloc;
		}

		if (zwu_uri_encode_string(location_encoded, string_size, location) != ZWS_STATUS_OK)
		{
			goto l_err_encode_location;
		}

		if (zwu_string_buffer_append_format(result,
				"<%s desc=\"%lu\" id=\"%u\" generic=\"%u\" specific=\"%u\" name=\"%s\" loc=\"%s\" "
				"zwplus_ver=\"%u\" role_type=\"%u\" node_type=\"%u\" instr_icon=\"%u\" usr_icon=\"%u\" aggregated_ep_list=\"%s\" />",
				((web_api_version == ZWS_WEB_API_VERSION_1) ? "ep" : "zwep"),
				ZWS_CGI_GET_ENDPOINT_DESC_ID(combo_id, (uint8_t)endpoint_id),
				(uint8_t)endpoint_id,
				(uint8_t)generic_device_class,
				(uint8_t)specific_device_class,
				name_encoded,
				location_encoded,
				(uint8_t)zwave_plus_version,
				(uint8_t)zwave_plus_role_type,
				(uint8_t)zwave_plus_node_type,
				(uint16_t)zwave_plus_installer_icon_type,
				(uint16_t)zwave_plus_user_icon_type,
				zwu_string_buffer_get_string(aggregated_ep_list)
				) != ZWS_STATUS_OK)
		{
			goto l_err_append;
		}

		zwu_string_buffer_destroy(aggregated_ep_list);

		ZWS_FREE(location_encoded);
		ZWS_FREE(name_encoded);

		zw_avro_field_free_string(location);
		zw_avro_field_free_string(name);
	}

	return ZWS_STATUS_OK;

l_err_append:
l_err_encode_location:
	ZWS_FREE(location_encoded);
l_err_location_encoded_alloc:
l_err_encode_name:
	ZWS_FREE(name_encoded);
l_err_name_encoded_alloc:
	zw_avro_field_free_string(location);
l_err_get_field_location:
	zw_avro_field_free_string(name);
l_err_get_field_name:
l_err_aggregated_ep_list_get_append:
l_err_get_field_aggregated_ep_value:
l_err_get_aggregated_ep_value_value:
	zwu_string_buffer_destroy(aggregated_ep_list);
l_err_aggregated_ep_list_list_create:
l_err_aggregated_ep_list_size:
l_err_get_aggregated_ep_list_value:
l_err_get_fields:
l_err_get_endpoint_value:
l_err_endpoint_list_size:
l_err_get_list_value:
	return ZWS_STATUS_ERROR;
}

/** Handlers for getting endpoint list */
static zws_avi_message_nodes_t nodes_endpoint_list_get =
{
		{"zwnode_get_ep_list", "get_ep_list"}, ZWS_CGI_NET_SUB_COMMAND_NONE,

		"node_endpoint_list_get",

		NULL,
		zws_avi_nodes_endpoint_list_result,

		"tServEpListFailed"
};

/** Register command handlers */
zws_status_t zws_avi_nodes_endpoint_list_init(zwu_array_list_t message_list)
{
	if (zws_avi_message_list_add_message_nodes(message_list, &nodes_endpoint_list_get) != ZWS_STATUS_OK)
	{
		goto l_err_networks_list_get;
	}

	return ZWS_STATUS_OK;

l_err_networks_list_get:
	return ZWS_STATUS_ERROR;
}

