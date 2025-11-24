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
 * @file    zws_avi_endpoints_interface_list.c
 *
 * @brief   AVI message 'endpoint_interface_list_get'
 * @details Handles the AVI message 'endpoint_interface_list_get'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-28
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message_endpoints.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"

/**
 * @brief Get CGI response for interface list from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_endpoints_interface_list_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	avro_value_t interface_list_value;
	avro_value_t interface_value;
	size_t interface_count;
	int i;
	int32_t interface_id;
	char *name;
	int32_t version;
	int32_t real_version;
	int32_t secure_access;
	int32_t unsecure_access;

	if (avro_value_get_by_name(arg_value, "interface_list", &interface_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'interface_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (avro_value_get_size(&interface_list_value, &interface_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get interface list size");
		goto l_err_interface_list_size;
	}

	for (i = 0; i < interface_count; i++)
	{
		if (avro_value_get_by_index(&interface_list_value, i, &interface_value, NULL) != 0)
		{
			goto l_err_get_interface_value;
		}

		if ((zw_avro_field_get_int(&interface_value, "id", &interface_id) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&interface_value, "version", &version) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&interface_value, "real_version", &real_version) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&interface_value, "secure_access", &secure_access) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&interface_value, "unsecure_access", &unsecure_access) != ZWS_STATUS_OK)
				)
		{
			goto l_err_get_fields;
		}

		if (zw_avro_field_get_string(&interface_value, "name", &name) != ZWS_STATUS_OK)
		{
			goto l_err_get_field_name;
		}

		if (zwu_string_buffer_append_format(result,
				"<%s desc=\"%lu\" id=\"%u\" name=\"%s\" ver=\"%u\" real_ver=\"%u\" sec=\"%u\" unsec=\"%u\" />",
				((web_api_version == ZWS_WEB_API_VERSION_1) ? "intf" : "zwif"),
				ZWS_CGI_GET_INTERFACE_DESC_ID(combo_id, (uint16_t)interface_id),
				(uint8_t)interface_id,
				name,
				(uint8_t)version,
				(uint8_t)real_version,
				(uint8_t)secure_access,
				(uint8_t)unsecure_access
				) != ZWS_STATUS_OK)
		{
			goto l_err_append;
		}

		zw_avro_field_free_string(name);
	}

	return ZWS_STATUS_OK;

l_err_append:
	zw_avro_field_free_string(name);
l_err_get_field_name:
l_err_get_fields:
l_err_get_interface_value:
l_err_interface_list_size:
l_err_get_list_value:
	return ZWS_STATUS_ERROR;
}

/** Handlers for getting interface list */
static zws_avi_message_endpoints_t endpoints_interface_list_get =
{
		{"zwep_get_if_list", "get_intf_list"}, ZWS_CGI_NET_SUB_COMMAND_NONE,

		"endpoint_interface_list_get",

		NULL,
		zws_avi_endpoints_interface_list_result,

		"tServIntfListFailed"
};

/** Register command handlers */
zws_status_t zws_avi_endpoints_interface_list_init(zwu_array_list_t message_list)
{
	if (zws_avi_message_list_add_message_endpoints(message_list, &endpoints_interface_list_get) != ZWS_STATUS_OK)
	{
		goto l_err_networks_list_get;
	}

	return ZWS_STATUS_OK;

l_err_networks_list_get:
	return ZWS_STATUS_ERROR;
}


