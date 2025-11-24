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
 * @file    zwp_avi_root_network_list.c
 *
 * @brief   AVI message 'root_network_list_get'
 * @details Handles the AVI message 'root_network_list_get'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-23
 * - Initial version
 */

#include "zwp_status.h"
#include "zwp_avi_message.h"
#include "zwp_log.h"
#include "zwp_memory.h"
#include "zwp_error.h"
#include "zwp_portal.h"
#include "zwp_directory_service.h"

/** Network state */
typedef struct
{
	char                        *net_id;                     /**< Network ID */
	int                         connected;                   /**< Flag to indicate if network connected to the portal */
} zwp_avi_entry_root_network_t;

/**
 * @brief Allocate a network entry object and initialize with network ID
 * @param[in]     net_id            Network ID
 * @param[out]    network_p         The new network entry object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_root_network_list_free_network_entry() to free resources
 */
static zwp_status_t zwp_avi_root_network_list_create_network_entry(char *net_id, zwp_avi_entry_root_network_t **network_p)
{
	zwp_avi_entry_root_network_t *network;

	if ((network = (zwp_avi_entry_root_network_t *)ZWP_MALLOC(sizeof(*network))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_network_entry_alloc;
	}

	if ((network->net_id = (char *)ZWP_STRDUP(net_id)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_net_id;
	}

	*network_p = network;

	return ZWP_STATUS_OK;

	ZWP_FREE(network->net_id);
l_err_net_id:
	ZWP_FREE(network);
l_err_network_entry_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free network entry object
 * @param[in]     network           Network entry object
 * @return        Nothing
 * @see zwp_avi_root_network_list_create_network_entry
 */
static void zwp_avi_root_network_list_free_network_entry(zwp_avi_entry_root_network_t *network)
{
	ZWP_FREE(network->net_id);
	ZWP_FREE(network);
}

/**
 * @brief Get the list of networks
 * @param[in]     portal            Portal object
 * @param[in]     user              User name
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_root_network_list_result_free() to free resources
 */
static zwp_status_t zwp_avi_root_network_list_execute(zwp_portal_t portal, char *user, void *arg, void **result_p)
{
	char **directory_service_net_list;
	int directory_service_net_count;
	zwu_array_list_t network_list;
	int network_count;
	zwp_avi_entry_root_network_t *network;
	int i;

	if (zwp_directory_service_get_network_list_for_user(zwp_portal_get_directory_service(portal), user, &directory_service_net_list, &directory_service_net_count) != ZWP_STATUS_OK)
	{
		goto l_err_networks_list_for_user;
	}

	if (zwu_array_list_create(directory_service_net_count, &network_list) != ZWP_STATUS_OK)
	{
		goto l_err_networks_list_create;
	}

	network_count = 0;
	for (i = 0; i < directory_service_net_count; i++)
	{

		if (zwp_avi_root_network_list_create_network_entry(directory_service_net_list[i], &network) != ZWP_STATUS_OK)
		{
			goto l_err_network_entry_create;
		}

		if (zwu_array_list_add(network_list, network) != ZWP_STATUS_OK)
		{
			goto l_err_network_list_add;
		}

		network_count++;
	}

	zwp_directory_service_free_network_list_for_user(directory_service_net_list, directory_service_net_count);

	*result_p = network_list;

	return ZWP_STATUS_OK;

l_err_network_list_add:
	zwp_avi_root_network_list_free_network_entry(network);
l_err_network_entry_create:
	for (i = 0; i < network_count; i++)
	{
		zwp_avi_root_network_list_free_network_entry(zwu_array_list_get(network_list, i));
	}
	zwu_array_list_destroy(network_list);
l_err_networks_list_create:
	zwp_directory_service_free_network_list_for_user(directory_service_net_list, directory_service_net_count);
l_err_networks_list_for_user:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to list of networks
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_root_network_list_result(void *result, avro_value_t *arg_value)
{
	avro_value_t network_list_value;
	avro_value_t network_value;
	zwu_array_list_t network_list;
	zwp_avi_entry_root_network_t *network;
	int network_count;
	int i;

	if (avro_value_get_by_name(arg_value, "network_list", &network_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'network_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	network_list = (zwu_array_list_t)result;
	network_count = zwu_array_list_size(network_list);

	for (i = 0; i < network_count; i++)
	{
		if (avro_value_append(&network_list_value, &network_value, 0) != 0)
		{
			goto l_err_append_network_value;
		}

		network = zwu_array_list_get(network_list, i);

		if (zw_avro_field_set_string(&network_value, "id", network->net_id) != 0)
		{
			goto l_err_set_network_value;
		}
	}

	return ZWP_STATUS_OK;

l_err_set_network_value:
l_err_append_network_value:
l_err_get_list_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to list of networks
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_root_network_list_execute
 */
static void zwp_avi_root_network_list_result_free(void *result)
{
	zwu_array_list_t network_list;
	int network_count;
	int i;

	network_list = (zwu_array_list_t)result;
	network_count = zwu_array_list_size(network_list);

	for (i = 0; i < network_count; i++)
	{
		zwp_avi_root_network_list_free_network_entry(zwu_array_list_get(network_list, i));
	}

	zwu_array_list_destroy(network_list);
}

/** Handlers for getting list of networks */
static zwp_avi_message_t root_network_list_get =
{
		"root_network_list_get",
		NULL, NULL,
		zwp_avi_root_network_list_execute,
		zwp_avi_root_network_list_result, zwp_avi_root_network_list_result_free
};

/** Register handlers */
zwp_status_t zwp_avi_root_network_list_init(zwu_array_list_t message_list)
{
	if (zwp_avi_message_list_add_message(message_list, &root_network_list_get) != ZWP_STATUS_OK)
	{
		goto l_err_network_list_get;
	}

	return ZWP_STATUS_OK;

l_err_network_list_get:
	return ZWP_STATUS_ERROR;
}
