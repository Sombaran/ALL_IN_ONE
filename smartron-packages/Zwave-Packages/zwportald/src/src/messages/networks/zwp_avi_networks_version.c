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
 * @file    zwp_avi_networks_version.c
 *
 * @brief   AVI message 'network_version_get'
 * @details Handles the AVI message 'network_version_get'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-02-12
 * - Initial version
 */

#include "zwp_status.h"
#include "zwp_avi_message_networks.h"
#include "zwp_log.h"
#include "zwp_memory.h"
#include "zwp_error.h"
#include "zwp_hcapi.h"
#include "zwp_const.h"

/** Version information state */
typedef struct
{
	char              *server_platform;       /**< Server platform */

	uint8_t           hc_api_version_major;   /**< Host Controller API version - major */
	uint8_t           hc_api_version_minor;   /**< Host Controller API version - minor */
} zwp_version_t;

/**
 * @brief Execute call to get version information
 * @param[in]     net               Network object
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 *@note
 * @par
 * The caller should call zwp_avi_networks_version_get_result_free() to free resources
 *
 * @par
 * @a net can be NULL
 */
static zwp_status_t zwp_avi_networks_version_get_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwp_version_t *version;

	if ((version = (zwp_version_t *)ZWP_MALLOC(sizeof(*version))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_version_alloc;
	}

	if ((version->server_platform = ZWP_STRDUP(ZWP_SERVER_PLATFORM)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_dup_server_platform;
	}

	version->hc_api_version_major = 0;
	version->hc_api_version_minor = 0;
	zwnet_version(&version->hc_api_version_major, &version->hc_api_version_minor);

	*result_p = version;

	return ZWP_STATUS_OK;

	ZWP_FREE(version->server_platform);
l_err_dup_server_platform:
	ZWP_FREE(version);
l_err_version_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to version information
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_networks_version_get_result(void *result, avro_value_t *arg_value)
{
	zwp_version_t *version;
	avro_value_t hc_api_version_field;

	version = (zwp_version_t *)result;

	if (zw_avro_field_set_string(arg_value, "server_platform", version->server_platform) != ZWP_STATUS_OK)
	{
		goto l_err_set_server_platform_value;
	}

	if (avro_value_get_by_name(arg_value, "host_controller_api_version", &hc_api_version_field, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'host_controller_api_version' field from 'Summary' record: %s", avro_strerror());
		goto l_err_get_hc_api_version_field;
	}

	if ((zw_avro_field_set_int(&hc_api_version_field, "major", version->hc_api_version_major) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(&hc_api_version_field, "minor", version->hc_api_version_minor) != ZWP_STATUS_OK))
	{
		goto l_err_set_hc_api_version_value;
	}

	return ZWP_STATUS_OK;

l_err_set_hc_api_version_value:
l_err_get_hc_api_version_field:
l_err_set_server_platform_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to version information
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_networks_version_get_execute
 */
static void zwp_avi_networks_version_get_result_free(void *result)
{
	zwp_version_t *version;

	version = (zwp_version_t *)result;

	ZWP_FREE(version->server_platform);

	ZWP_FREE(version);
}

/** Handlers for getting version information */
static zwp_avi_message_networks_t networks_version_get =
{
		"network_version_get",
		NULL, NULL,
		zwp_avi_networks_version_get_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_NO,
		zwp_avi_networks_version_get_result, zwp_avi_networks_version_get_result_free
};

/** Register handlers */
zwp_status_t zwp_avi_networks_version_init(zwu_array_list_t message_list)
{
	if (zwp_avi_message_list_add_message_networks(message_list, &networks_version_get) != ZWP_STATUS_OK)
	{
		goto l_err_version_get;
	}

	return ZWP_STATUS_OK;

l_err_version_get:
	return ZWP_STATUS_ERROR;
}







