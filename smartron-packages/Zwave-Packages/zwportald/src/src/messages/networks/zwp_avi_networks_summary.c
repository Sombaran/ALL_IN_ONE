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
 * @file    zwp_avi_networks_summary.c
 *
 * @brief   AVI message 'network_summary_get'
 * @details Handles the AVI message 'network_summary_get'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-02-10
 * - Initial version
 */

#include <sys/stat.h>

#include "zwp_status.h"
#include "zwp_avi_message_networks.h"
#include "zwp_log.h"
#include "zwp_memory.h"
#include "zwp_io.h"
#include "zwp_error.h"
#include "zwp_network.h"
#include "zwp_const.h"

/** Maximum length of portal client ID string */
#define ZWP_PORTAL_CLIENT_ID_STRING_MAX_LEN                 24

/** Summary information state */
typedef struct
{
	char              *vendor_name;           /**< Vendor name */
	char              *product_name;          /**< Product name */
	char              *server_platform;       /**< Server platform */

	uint8_t           hc_api_version_major;   /**< Host Controller API version - major */
	uint8_t           hc_api_version_minor;   /**< Host Controller API version - minor */

	int               is_initialized;         /**< Is network initialized? */
	uint32_t          home_id;                /**< Home ID */
	uint8_t           controller_node_id;     /**< Node ID of the controller */
	char              *network_id;            /**< Network ID - Remote Access Code */
#ifdef LOCAL_CLIENT_ENABLE
	uint8_t			  zipr_gateway_addr[16];
	uint8_t			  gateway_addr_type;
#endif

} zwp_net_summary_t;

/**
 * @brief Execute call to get summary information
 * @param[in]     net               Network object
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 *@note
 * @par
 * The caller should call zwp_avi_networks_summary_get_result_free() to free resources
 *
 * @par
 * @a net can be NULL
 */
static zwp_status_t zwp_avi_networks_summary_get_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwp_net_summary_t *summary;
	zwnet_p zwnet;
	zwnetd_p info;
	const clnt_prof_t *profile;
#ifdef LOCAL_CLIENT_ENABLE
	zwp_net_zip_server_config_t zip_gw_config;
#endif

	if ((summary = (zwp_net_summary_t *)ZWP_MALLOC(sizeof(*summary))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_network_summary_alloc;
	}

	if ((summary->vendor_name = ZWP_STRDUP(ZWP_VENDOR_NAME)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_dup_vendor_name;
	}

	if ((summary->product_name = ZWP_STRDUP(ZWP_PRODUCT_NAME)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_dup_product_name;
	}

	if ((summary->server_platform = ZWP_STRDUP(ZWP_SERVER_PLATFORM)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_dup_server_platform;
	}

	summary->hc_api_version_major = 0;
	summary->hc_api_version_minor = 0;
	zwnet_version(&summary->hc_api_version_major, &summary->hc_api_version_minor);

	// If (net == NULL), the gateway is disconnected from portal
	// If (zwp_net_get_desc_net(net) == NULL), the gateway is not yet fully initialized.
	summary->is_initialized = (net) ? ((zwp_net_get_desc_net(net)) ? 1 : 0) : 0;

	if (summary->is_initialized)
	{
		zwnet = zwp_net_get_zwnet(net);

		if ((info = zwnet_get_desc(zwnet)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, "zwnet_get_desc() failed");
			goto l_err_net_get_desc;
		}

		summary->home_id = info->id;
		summary->controller_node_id = info->ctl_id;

		profile = zwp_net_get_portal_client_profile(net);

		if ((summary->network_id = (char *)ZWP_MALLOC(ZWP_PORTAL_CLIENT_ID_STRING_MAX_LEN)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_net_id_alloc;
		}

		snprintf(summary->network_id, ZWP_PORTAL_CLIENT_ID_STRING_MAX_LEN, "%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X",
				profile->clnt_id[0], profile->clnt_id[1], profile->clnt_id[2], profile->clnt_id[3],
				profile->clnt_id[4], profile->clnt_id[5], profile->clnt_id[6], profile->clnt_id[7]);
#ifdef LOCAL_CLIENT_ENABLE
		zwp_net_get_zip_server_addr(net, &zip_gw_config);
		memcpy(summary->zipr_gateway_addr, zip_gw_config.zip_server_address, sizeof(summary->zipr_gateway_addr));
		summary->gateway_addr_type = zip_gw_config.ipv4;
#endif
	}
	else
	{
		summary->home_id = 0;
		summary->controller_node_id = 0;
		summary->network_id = NULL;
#ifdef LOCAL_CLIENT_ENABLE
		memset(summary->zipr_gateway_addr, 0, sizeof(summary->zipr_gateway_addr));
		summary->gateway_addr_type = 0;
#endif
	}

	*result_p = summary;

	return ZWP_STATUS_OK;

	ZWP_FREE(summary->network_id);
l_err_net_id_alloc:
l_err_net_get_desc:
	ZWP_FREE(summary->server_platform);
l_err_dup_server_platform:
	ZWP_FREE(summary->product_name);
l_err_dup_product_name:
	ZWP_FREE(summary->vendor_name);
l_err_dup_vendor_name:
	ZWP_FREE(summary);
l_err_network_summary_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to summary information
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_networks_summary_get_result(void *result, avro_value_t *arg_value)
{
	zwp_net_summary_t *summary;
	avro_value_t hc_api_version_field;
	avro_value_t summary_initialized_field;
	avro_value_t summary_initialized_value;

	summary = (zwp_net_summary_t *)result;

	if ((zw_avro_field_set_string(arg_value, "vendor_name", summary->vendor_name) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_string(arg_value, "product_name", summary->product_name) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_string(arg_value, "server_platform", summary->server_platform) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "is_initialized", summary->is_initialized) != ZWP_STATUS_OK))
	{
		goto l_err_set_summary_value;
	}

	/*
	 * Version related items
	 */
	if (avro_value_get_by_name(arg_value, "host_controller_api_version", &hc_api_version_field, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'host_controller_api_version' field from 'Summary' record: %s", avro_strerror());
		goto l_err_get_hc_api_version_field;
	}

	if ((zw_avro_field_set_int(&hc_api_version_field, "major", summary->hc_api_version_major) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(&hc_api_version_field, "minor", summary->hc_api_version_minor) != ZWP_STATUS_OK))
	{
		goto l_err_set_hc_api_version_value;
	}

	/*
	 * Items to be filled when network is initialized
	 */
	if (avro_value_get_by_name(arg_value, "summary_initialized", &summary_initialized_field, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'summary_initialized' field from 'Summary' record: %s", avro_strerror());
		goto l_err_get_summary_initialized_field;
	}

	if (summary->is_initialized)
	{
		if (avro_value_set_branch(&summary_initialized_field, 0, &summary_initialized_value) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to set 'summary_initialized' field to branch '0': %s", avro_strerror());
			goto l_err_set_summary_initialized_field;
		}

		if ((zw_avro_field_set_long(&summary_initialized_value, "home_id", summary->home_id) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&summary_initialized_value, "controller_node_id", summary->controller_node_id) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_string(&summary_initialized_value, "network_id", summary->network_id) != ZWP_STATUS_OK)
#ifdef LOCAL_CLIENT_ENABLE
				|| (zw_avro_field_set_fixed(&summary_initialized_value, "gateway_address", summary->zipr_gateway_addr, 16) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&summary_initialized_value, "address_type", summary->gateway_addr_type) != ZWP_STATUS_OK)
#endif
				)
		{
			goto l_err_set_summary_initialized_value;
		}
	}
	else
	{
		if (avro_value_set_branch(&summary_initialized_field, 1, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to set 'summary_initialized' field to branch '1': %s", avro_strerror());
			goto l_err_set_summary_initialized_field;
		}
	}

	return ZWP_STATUS_OK;

l_err_set_summary_initialized_value:
l_err_set_summary_initialized_field:
l_err_get_summary_initialized_field:
l_err_set_hc_api_version_value:
l_err_get_hc_api_version_field:
l_err_set_summary_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to summary information
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_networks_summary_get_execute
 */
static void zwp_avi_networks_summary_get_result_free(void *result)
{
	zwp_net_summary_t *summary;

	summary = (zwp_net_summary_t *)result;

	ZWP_FREE(summary->network_id);

	ZWP_FREE(summary->server_platform);
	ZWP_FREE(summary->product_name);
	ZWP_FREE(summary->vendor_name);

	ZWP_FREE(summary);
}

/** Handlers for getting summary information */
static zwp_avi_message_networks_t networks_summary_get =
{
		"network_summary_get",
		NULL, NULL,
		zwp_avi_networks_summary_get_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_NO,
		zwp_avi_networks_summary_get_result, zwp_avi_networks_summary_get_result_free
};

/** Register handlers */
zwp_status_t zwp_avi_networks_summary_init(zwu_array_list_t message_list)
{
	if (zwp_avi_message_list_add_message_networks(message_list, &networks_summary_get) != ZWP_STATUS_OK)
	{
		goto l_err_summary_get;
	}

	return ZWP_STATUS_OK;

l_err_summary_get:
	return ZWP_STATUS_ERROR;
}
