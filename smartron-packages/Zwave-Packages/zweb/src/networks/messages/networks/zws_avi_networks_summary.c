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
 * @file    zws_avi_networks_summary.c
 *
 * @brief   AVI message 'network_summary_get'
 * @details Handles the AVI message 'network_summary_get'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-02-10
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message_networks.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zws_version.h"
#include "zws_file.h"
#include "zwu_encoding.h"
#include "zwu_inet.h"

/** HCAPI version */
#define ZWS_VERSION_ATTRIBUTE_NAME_HC_API              "zw_ctl"
/** HC App version */
#define ZWS_VERSION_ATTRIBUTE_NAME_HC_APP              "zw_appl"
/** Web API version */
#define ZWS_VERSION_ATTRIBUTE_NAME_WEB_API             "zw_webapi"
/** Engineering UI version */
#define ZWS_VERSION_ATTRIBUTE_NAME_UI_ENGINEERING      "zw_ui_engineering"
/** TV/STB UI version */
#define ZWS_VERSION_ATTRIBUTE_NAME_UI_TV               "zw_ui_stb"
/** PC/Tablet UI version */
#define ZWS_VERSION_ATTRIBUTE_NAME_UI_PC               "zw_ui_pc"
/** Phone UI version */
#define ZWS_VERSION_ATTRIBUTE_NAME_UI_PHONE            "zw_ui_phone"

/**
 * @brief Get CGI response for summary information from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_networks_summary_get_result(int web_api_version, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	char *vendor_name;
	char *product_name;
	char *server_platform;
	int32_t is_initialized;
	avro_value_t hc_api_version_field;
	avro_value_t summary_initialized_field;
	avro_value_t summary_initialized_value;
	int64_t home_id;
	int32_t controller_node_id;
	char *network_id;
	int disc;
	char *web_user_name;
	char *web_server_address;
	char *web_client_address;
	char *root_dir_path;
	int32_t hc_api_version_major;   /**< Host Controller API version - major */
	int32_t hc_api_version_minor;   /**< Host Controller API version - minor */
	char *hc_app_version_major;     /**< Host Controller Application version - major */
	char *hc_app_version_minor;     /**< Host Controller Application version - minor */
	char *ui_version_engineering;
	char *ui_version_tv;
	char *ui_version_pc;
	char *ui_version_phone;
	size_t string_size;
	char *vendor_name_encoded;
	char *product_name_encoded;
	char *web_user_name_encoded;
#ifdef LOCAL_CLIENT_ENABLE
	void *zip_gw_ipv6_addr = NULL;
	char zip_gw_ipv6_addr_string[INET6_ADDRSTRLEN] = {0};
	int address_type = 0;
	size_t zip_gw_ipv6_addr_size;
#endif


	// No equivalent in Web API v1. So return
	if (web_api_version == ZWS_WEB_API_VERSION_1)
	{
		return ZWS_STATUS_OK;
	}

	// Items that are filled in the web server itself and doesn't require trip to portal daemon.
	web_user_name = getenv("ZWARE_USER_NAME");
	web_server_address = getenv("SERVER_ADDR");
	web_client_address = getenv("REMOTE_ADDR");

	if ((zw_avro_field_get_int(arg_value, "is_initialized", &is_initialized) != ZWS_STATUS_OK))
	{
		goto l_err_get_fields;
	}

	if (avro_value_get_by_name(arg_value, "host_controller_api_version", &hc_api_version_field, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'host_controller_api_version' field from 'Summary' record: %s", avro_strerror());
		goto l_err_get_hc_api_version_field;
	}

	if ((zw_avro_field_get_int(&hc_api_version_field, "major", &hc_api_version_major) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(&hc_api_version_field, "minor", &hc_api_version_minor) != ZWS_STATUS_OK))
	{
		goto l_err_get_hc_api_version_value;
	}

	if (avro_value_get_by_name(arg_value, "summary_initialized", &summary_initialized_field, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'summary_initialized' field from 'Summary' record: %s", avro_strerror());
		goto l_err_get_summary_initialized_field;
	}

	if (avro_value_get_discriminant(&summary_initialized_field, &disc) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get branch number of 'summary_initialized' field: %s", avro_strerror());
		goto l_err_get_summary_initialized_disc;
	}

	if (disc == 0)
	{
		if (avro_value_get_current_branch(&summary_initialized_field, &summary_initialized_value) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'summary_initialized' field in branch '0': %s", avro_strerror());
			goto l_err_get_summary_initialized_value;
		}

		if ((zw_avro_field_get_long(&summary_initialized_value, "home_id", &home_id) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&summary_initialized_value, "controller_node_id", &controller_node_id) != ZWS_STATUS_OK))
		{
			goto l_err_get_summary_initialized_fields;
		}

		if (zw_avro_field_get_string(&summary_initialized_value, "network_id", &network_id) != ZWS_STATUS_OK)
		{
			goto l_err_get_fields_network_id;
		}
#ifdef LOCAL_CLIENT_ENABLE
		if (zw_avro_field_get_int(&summary_initialized_value, "address_type", &address_type) != ZWS_STATUS_OK)
		{
			goto l_err_get_address_type;
		}

		if (zw_avro_field_get_fixed(&summary_initialized_value, "gateway_address", &zip_gw_ipv6_addr, &zip_gw_ipv6_addr_size) != ZWS_STATUS_OK)
		{
			goto l_err_get_zip_gw_ipv6_addr_value;
		}

		if((zip_gw_ipv6_addr_size != 16) ||
		  ((zwu_inet_ntop_iptype(zip_gw_ipv6_addr, zip_gw_ipv6_addr_string, address_type, sizeof(zip_gw_ipv6_addr_string)) != ZWS_STATUS_OK)))
		{
			goto l_err_get_zip_gw_ipv6_addr_size;
		}
#endif
	}
	else
	{
		home_id = 0;
		controller_node_id = 0;
		network_id = NULL;
	}

	if (zw_avro_field_get_string(arg_value, "vendor_name", &vendor_name) != ZWS_STATUS_OK)
	{
		goto l_err_get_fields_vendor_name;
	}

	if (zw_avro_field_get_string(arg_value, "product_name", &product_name) != ZWS_STATUS_OK)
	{
		goto l_err_get_fields_product_name;
	}

	if (zw_avro_field_get_string(arg_value, "server_platform", &server_platform) != ZWS_STATUS_OK)
	{
		goto l_err_get_fields_server_platform;
	}

	if (zws_file_get_root_dir_path(&root_dir_path) != ZWS_STATUS_OK)
	{
		goto l_err_root_dir_path;
	}

	hc_app_version_major = hc_app_version_minor = NULL;
	zws_file_get_host_controller_application_version(root_dir_path, &hc_app_version_major, &hc_app_version_minor);

	ui_version_engineering = NULL;
	zws_file_get_ui_version(root_dir_path, ZWS_UI_ENGINEERING, &ui_version_engineering);

	ui_version_tv = NULL;
	zws_file_get_ui_version(root_dir_path, ZWS_UI_TV, &ui_version_tv);

	ui_version_pc = NULL;
	zws_file_get_ui_version(root_dir_path, ZWS_UI_PC, &ui_version_pc);

	ui_version_phone = NULL;
	zws_file_get_ui_version(root_dir_path, ZWS_UI_PHONE, &ui_version_phone);

	string_size = (strlen(vendor_name) * 3) + 1;
	if ((vendor_name_encoded = (char *)ZWS_MALLOC(string_size)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_vendor_name_encoded_alloc;
	}

	if (zwu_uri_encode_string(vendor_name_encoded, string_size, vendor_name) != ZWS_STATUS_OK)
	{
		goto l_err_encode_vendor_name;
	}

	string_size = (strlen(product_name) * 3) + 1;
	if ((product_name_encoded = (char *)ZWS_MALLOC(string_size)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_product_name_encoded_alloc;
	}

	if (zwu_uri_encode_string(product_name_encoded, string_size, product_name) != ZWS_STATUS_OK)
	{
		goto l_err_encode_product_name;
	}

	web_user_name_encoded = NULL;
	if (web_user_name)
	{
		string_size = (strlen(web_user_name) * 3) + 1;
		if ((web_user_name_encoded = (char *)ZWS_MALLOC(string_size)) == NULL)
		{
			ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
			goto l_err_web_user_name_encoded_alloc;
		}

		if (zwu_uri_encode_string(web_user_name_encoded, string_size, web_user_name) != ZWS_STATUS_OK)
		{
			goto l_err_encode_web_user_name;
		}
	}

	/*
	 * FIXME: change 'vendor_name' to 'vendor_name_encoded' and 'product_name' to 'product_name_encoded'
	 * after they are handled as encoded strings in UIs
	 */
#ifdef LOCAL_CLIENT_ENABLE
	if (zwu_string_buffer_append_format(result,
				"<zw_info vname=\"%s\" pname=\"%s\" spltform=\"%s\" uname=\"%s\" init=\"%d\" "
				"home_id=\"%08X\" ctlr_id=\"%u\" portalcid=\"%s\" zipgw=\"%s\" server_ip=\"%s\" client_ip=\"%s\" >",
				vendor_name,
				product_name,
				server_platform,
				((web_user_name_encoded) ? web_user_name_encoded : ""),
				is_initialized,
				(uint32_t)home_id,
				(uint8_t)controller_node_id,
				((network_id) ? network_id : ""),
				((zip_gw_ipv6_addr) ? zip_gw_ipv6_addr_string : ""),
				((web_server_address) ? web_server_address : ""),
				((web_client_address) ? web_client_address : "")
				) != ZWS_STATUS_OK)
	{
		goto l_err_append_info;
	}
#else
	if (zwu_string_buffer_append_format(result,
					"<zw_info vname=\"%s\" pname=\"%s\" spltform=\"%s\" uname=\"%s\" init=\"%d\" "
					"home_id=\"%08X\" ctlr_id=\"%u\" portalcid=\"%s\" server_ip=\"%s\" client_ip=\"%s\" >",
					vendor_name,
					product_name,
					server_platform,
					((web_user_name_encoded) ? web_user_name_encoded : ""),
					is_initialized,
					(uint32_t)home_id,
					(uint8_t)controller_node_id,
					((network_id) ? network_id : ""),
					((web_server_address) ? web_server_address : ""),
					((web_client_address) ? web_client_address : "")
					) != ZWS_STATUS_OK)
		{
			goto l_err_append_info;
		}
#endif

	/* HC API version */
	if (zwu_string_buffer_append_format(result, "<version name=\"%s\" value=\"%u.%02u\" />",
			ZWS_VERSION_ATTRIBUTE_NAME_HC_API,
			(uint8_t)hc_api_version_major,
			(uint8_t)hc_api_version_minor
			) != ZWS_STATUS_OK)
	{
		goto l_err_append_version;
	}

	/* HC App version - this application */
	if (zwu_string_buffer_append_format(result, "<version name=\"%s\" value=\"%s.%s\" />",
			ZWS_VERSION_ATTRIBUTE_NAME_HC_APP,
			((hc_app_version_major) ? hc_app_version_major : ""),
			((hc_app_version_minor) ? hc_app_version_minor : "")
			) != ZWS_STATUS_OK)
	{
		goto l_err_append_version;
	}

	/* Web API version */
	if (zwu_string_buffer_append_format(result, "<version name=\"%s\" value=\"%u.%02u\" />",
			ZWS_VERSION_ATTRIBUTE_NAME_WEB_API,
			ZWS_WEB_API_VERSION_MAJOR,
			ZWS_WEB_API_VERSION_MINOR
			) != ZWS_STATUS_OK)
	{
		goto l_err_append_version;
	}

	/* Engineering UI version */
	if (zwu_string_buffer_append_format(result, "<version name=\"%s\" value=\"%s\" />",
			ZWS_VERSION_ATTRIBUTE_NAME_UI_ENGINEERING,
			((ui_version_engineering) ? ui_version_engineering : "")
			) != ZWS_STATUS_OK)
	{
		goto l_err_append_version;
	}

	/* TV UI version */
	if (zwu_string_buffer_append_format(result, "<version name=\"%s\" value=\"%s\" />",
			ZWS_VERSION_ATTRIBUTE_NAME_UI_TV,
			((ui_version_tv) ? ui_version_tv : "")
			) != ZWS_STATUS_OK)
	{
		goto l_err_append_version;
	}

	/* PC UI version */
	if (zwu_string_buffer_append_format(result, "<version name=\"%s\" value=\"%s\" />",
			ZWS_VERSION_ATTRIBUTE_NAME_UI_PC,
			((ui_version_pc) ? ui_version_pc : "")
			) != ZWS_STATUS_OK)
	{
		goto l_err_append_version;
	}

	/* Phone UI version */
	if (zwu_string_buffer_append_format(result, "<version name=\"%s\" value=\"%s\" />",
			ZWS_VERSION_ATTRIBUTE_NAME_UI_PHONE,
			((ui_version_phone) ? ui_version_phone : "")
			) != ZWS_STATUS_OK)
	{
		goto l_err_append_version;
	}

	if (zwu_string_buffer_append_format(result,
				"</zw_info>"
				) != ZWS_STATUS_OK)
	{
		goto l_err_append_info_end;
	}

	ZWS_FREE(web_user_name_encoded);
	ZWS_FREE(product_name_encoded);
	ZWS_FREE(vendor_name_encoded);

	zws_file_free_ui_version(ui_version_phone);
	zws_file_free_ui_version(ui_version_pc);
	zws_file_free_ui_version(ui_version_tv);
	zws_file_free_ui_version(ui_version_engineering);

	zws_file_free_host_controller_application_version(hc_app_version_major, hc_app_version_minor);

	zws_file_free_root_dir_path(root_dir_path);

	zw_avro_field_free_string(server_platform);
	zw_avro_field_free_string(product_name);
	zw_avro_field_free_string(vendor_name);

	if (disc == 0)
	{
		zw_avro_field_free_string(network_id);
#ifdef LOCAL_CLIENT_ENABLE
		zw_avro_field_free_fixed(zip_gw_ipv6_addr);
#endif
	}

	return ZWS_STATUS_OK;

l_err_append_info_end:
l_err_append_version:
l_err_append_info:
l_err_encode_web_user_name:
	ZWS_FREE(web_user_name_encoded);
l_err_web_user_name_encoded_alloc:
l_err_encode_product_name:
	ZWS_FREE(product_name_encoded);
l_err_product_name_encoded_alloc:
l_err_encode_vendor_name:
	ZWS_FREE(vendor_name_encoded);
l_err_vendor_name_encoded_alloc:
	zws_file_free_ui_version(ui_version_phone);
	zws_file_free_ui_version(ui_version_pc);
	zws_file_free_ui_version(ui_version_tv);
	zws_file_free_ui_version(ui_version_engineering);
	zws_file_free_host_controller_application_version(hc_app_version_major, hc_app_version_minor);
	zws_file_free_root_dir_path(root_dir_path);
l_err_root_dir_path:
	zw_avro_field_free_string(server_platform);
l_err_get_fields_server_platform:
	zw_avro_field_free_string(product_name);
l_err_get_fields_product_name:
	zw_avro_field_free_string(vendor_name);
l_err_get_fields_vendor_name:
#ifdef LOCAL_CLIENT_ENABLE
l_err_get_zip_gw_ipv6_addr_size:
	zw_avro_field_free_fixed(zip_gw_ipv6_addr);
l_err_get_zip_gw_ipv6_addr_value:
l_err_get_address_type:
#endif
	zw_avro_field_free_string(network_id);
l_err_get_fields_network_id:
l_err_get_summary_initialized_fields:
l_err_get_summary_initialized_value:
l_err_get_summary_initialized_disc:
l_err_get_summary_initialized_field:

l_err_get_hc_api_version_value:
l_err_get_hc_api_version_field:
l_err_get_fields:
	return ZWS_STATUS_ERROR;
}

/** Handlers for getting summary information */
static zws_avi_message_networks_t networks_summary_get =
{
		{"zw_info", NULL}, ZWS_CGI_NET_SUB_COMMAND_NONE,

		"network_summary_get",

		NULL,
		zws_avi_networks_summary_get_result,

		""
};

/** Register command handlers */
zws_status_t zws_avi_networks_summary_init(zwu_array_list_t message_list)
{
	if (zws_avi_message_list_add_message_networks(message_list, &networks_summary_get) != ZWS_STATUS_OK)
	{
		goto l_err_networks_list_get;
	}

	return ZWS_STATUS_OK;

l_err_networks_list_get:
	return ZWS_STATUS_ERROR;
}





