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
 * @file    zwp_avi_service.c
 *
 * @brief   Avro Interface Input
 * @details Handles Avro Interface Input parameters.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-14
 * - Initial version
 */

#include <stdio.h>
#include <stdlib.h>

#include "zwp_log.h"
#if defined(ZWP_DEBUG_AVI_SERVICE) || defined(ZWP_DEBUG_AVI_SERVICE_ROOT)
#include "zwp_log_enable.h"
#else
#include "zwp_log_disable.h"
#endif

#include "zwp_avi_service_common.h"
#include "zwp_avi_service.h"
#include "zwp_avi_service_networks.h"
#include "zwp_memory.h"
#include "zwp_error.h"

/** Default memory block size of Avro writer  */
#define ZW_AVRO_WRITER_MEMORY_DEFAULT_BLOCK_SIZE       256

/** AVI Service class */
struct _zwp_service
{
	/* Level specific */
	zwp_avi_message_type_t      message_type;                /**< Level 1 commands: 'networks', 'scenes', etc  */
	void                        *service_next;               /**< Next level service based on 'command' */

	/* Common to all levels */
	zwp_service_common_t        service_common;              /**< Parameters common to all levels */
};

/**
 * @name Getters
 * @{
 */
char *zwp_avi_service_get_message_name(zwp_service_t service)
{
	return service->service_common->message_name;
}

zwp_avi_message_type_t zwp_avi_service_get_message_type(zwp_service_t service)
{
	return service->message_type;
}

zwp_avi_message_level_t zwp_avi_service_get_message_level(zwp_service_t service)
{
	return service->service_common->message_level;
}

char *zwp_avi_service_get_user(zwp_service_t service)
{
	return service->service_common->user;
}

zwp_avi_message_execute_t zwp_avi_service_get_avi_message_execute_handler(zwp_service_t service)
{
	return ((zwp_avi_message_t *)service->service_common->avi_message)->execute_handler;
}

zwp_avi_message_result_free_t zwp_avi_service_get_avi_message_result_free_handler(zwp_service_t service)
{
	return ((zwp_avi_message_t *)service->service_common->avi_message)->result_free_handler;
}

void *zwp_avi_service_get_service_next(zwp_service_t service)
{
	return service->service_next;
}
/**
 * @}
 */

/**
 * @name Setters
 * @{
 */
void zwp_avi_service_set_argument(zwp_service_t service, void *argument_data, zwp_avi_message_argument_free_t free_handler)
{
	service->service_common->argument.data = argument_data;
	service->service_common->argument.free_handler = free_handler;
}

void zwp_avi_service_set_result(zwp_service_t service, void *result_data, zwp_avi_message_result_free_t free_handler)
{
	service->service_common->result.data = result_data;
	service->service_common->result.free_handler = free_handler;
}
/**
 * @}
 */

zwp_status_t zwp_portal_execute_avi_message_root(zwp_portal_t portal, zwp_service_t service)
{
	zwp_avi_message_level_t message_level;
	zwp_avi_message_execute_t execute_handler;
	zwp_avi_message_result_free_t result_free_handler;
	char *user;

	message_level = zwp_avi_service_get_message_level(service);
	user = zwp_avi_service_get_user(service);

	ZWP_LOG(ZWP_LOG_DEBUG, "Message level: %d", message_level);

	if (message_level == ZWP_AVI_MESSAGE_LEVEL_ROOT)
	{
		execute_handler = zwp_avi_service_get_avi_message_execute_handler(service);
		result_free_handler = zwp_avi_service_get_avi_message_result_free_handler(service);

		if (execute_handler(portal, user,
				service->service_common->argument.data,
				&service->service_common->result.data) != ZWP_STATUS_OK)
		{
			goto l_err_execute;
		}

		zwp_avi_service_set_result(service,
				service->service_common->result.data, result_free_handler);
	}
	else
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "No match with messages of root type");
		goto l_no_match;
	}

	return ZWP_STATUS_OK;

l_no_match:
l_err_execute:
	return ZWP_STATUS_ERROR;
}

/*
 * Input
 */

/**
 * @brief Allocate Avro value object for cookies
 * @param[in]     avro              Avro object
 * @param[out]    cookies_value_p   Avro value object for cookies
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_free_avro_value_of_cookies() to free resources
 */
static zwp_status_t zwp_avi_alloc_avro_value_of_cookies(zw_avro_t avro, avro_value_t *cookies_value_p)
{
	if (avro_generic_value_new(zw_avro_get_cookies_iface(avro), cookies_value_p) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to allocate 'Cookies' record from interface: %s", avro_strerror());
		goto l_err_cookie_record;
	}

	return ZWP_STATUS_OK;

l_err_cookie_record:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Avro value object for cookies
 * @param[in]     cookies_value_p   Avro value object for cookies
 * @return        Nothing
 * @see zwp_avi_alloc_avro_value_of_cookies
 */
static void zwp_avi_free_avro_value_of_cookies(avro_value_t *cookies_value_p)
{
	avro_value_decref(cookies_value_p);
}

/**
 * @brief Get user name from Avro value object for cookies
 * @param[in]     cookies_value_p   Avro value object for cookies
 * @param[out]    user_p            User name
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_free_user() to free resources
 */
static zwp_status_t zwp_avi_get_user(avro_value_t *cookies_value_p, char **user_p)
{
	avro_value_t user_field;
	avro_value_t user_string;
	size_t size;
	const char *user;
	int disc;

	if (avro_value_get_by_name(cookies_value_p, "user", &user_field, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'user' field from 'Cookies' record: %s", avro_strerror());
		goto l_err_get_user_field;
	}

	if (avro_value_get_discriminant(&user_field, &disc) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get branch number of 'user' field: %s", avro_strerror());
		goto l_err_get_user_disc;
	}

	if (disc == 0)
	{
		if (avro_value_get_current_branch(&user_field, &user_string) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'user' field in branch '0': %s", avro_strerror());
			goto l_err_get_user_field;
		}

		if (avro_value_get_string(&user_string, &user, &size) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'user' string: %s", avro_strerror());
			goto l_err_get_user_string;
		}

		if ((*user_p =	ZWP_STRDUP(user)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_user_string_alloc;
		}
	}
	else
	{
		*user_p = NULL;
	}

	return ZWP_STATUS_OK;

l_err_user_string_alloc:
l_err_get_user_string:
l_err_get_user_disc:
l_err_get_user_field:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free user name from Avro value object for cookies
 * @param[in]     user              User name
 * @return        Nothing
 * @see zwp_avi_get_user
 */
static void zwp_avi_free_user(char *user)
{
	ZWP_FREE(user);
}

/**
 * @brief Allocate Avro value object for message name
 * @param[in]     avro              Avro object
 * @param[out]    message_value_p   Avro value object for message name
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_free_avro_value_of_message_name() to free resources
 */
static zwp_status_t zwp_avi_alloc_avro_value_of_message_name(zw_avro_t avro, avro_value_t *message_value_p)
{
	if (avro_generic_string_new(message_value_p, "") != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to allocate 'message' value: %s", avro_strerror());
		goto l_err_message_value;
	}

	return ZWP_STATUS_OK;

l_err_message_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Avro value object for message name
 * @param[in]     message_value_p   Avro value object for message name
 * @return        Nothing
 * @see zwp_avi_alloc_avro_value_of_message_name
 */
static void zwp_avi_free_avro_value_of_message_name(avro_value_t *message_value_p)
{
	avro_value_decref(message_value_p);
}

/**
 * @brief Get message name from Avro value object for message name
 * @param[in]     message_name_value_p   Avro value object for message name
 * @param[out]    message_name_p    Message name
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_free_message_name() to free resources
 */
static zwp_status_t zwp_avi_get_message_name(avro_value_t *message_name_value_p, char **message_name_p)
{
	const char *message_name;
	size_t size;

	if (avro_value_get_string(message_name_value_p, &message_name, &size) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'message name' string: %s", avro_strerror());
		goto l_err_get_message_name_string;
	}

	if ((*message_name_p =	ZWP_STRDUP(message_name)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_message_name_string_alloc;
	}

	return ZWP_STATUS_OK;

l_err_message_name_string_alloc:
l_err_get_message_name_string:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free message name from Avro value object for message name
 * @param[in]     message_name      Message name
 * @return        Nothing
 * @see zwp_avi_get_message_name
 */
static void zwp_avi_free_message_name(char *message_name)
{
	ZWP_FREE(message_name);
}

/**
 * @brief Set AVI message info of a service based on message name
 * @param[in]     service           Service object
 * @param[in]     message_name      Message name
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_set_avi_message_info(zwp_service_t service, const char *message_name)
{
	if (!strncmp(message_name, "root_", sizeof("root_") - 1))
	{
		service->message_type = ZWP_AVI_MESSAGE_TYPE_ROOT;
		service->service_common->message_level = ZWP_AVI_MESSAGE_LEVEL_ROOT;
		return zwp_avi_get_avi_message_from_message_name(zwp_avi_get_message_list_root(service->service_common->avi),
				message_name, (zwp_avi_message_t **)&service->service_common->avi_message);
	}
	else if ((!strncmp(message_name, "network_", sizeof("network_") - 1))	|| 
			(!strncmp(message_name, "scene_", sizeof("scene_") - 1))		|| 
			(!strncmp(message_name, "security_scene_", sizeof("security_scene_") - 1)))
	{
		service->message_type = ZWP_AVI_MESSAGE_TYPE_NETWORKS;
		service->service_common->message_level = ZWP_AVI_MESSAGE_LEVEL_NETWORKS;
		return zwp_avi_get_avi_message_networks_from_message_name(zwp_avi_get_message_list_networks(service->service_common->avi),
				message_name, (zwp_avi_message_networks_t **)&service->service_common->avi_message);
	}
	else if (!strncmp(message_name, "ifttt_", sizeof("ifttt_") - 1))
	{
		service->message_type = ZWP_AVI_MESSAGE_TYPE_IFTTT;
		if (!strncmp(message_name, "ifttt_network_", sizeof("ifttt_network_") - 1))
		{
			service->service_common->message_level = ZWP_AVI_MESSAGE_LEVEL_NETWORKS;

			return zwp_avi_get_avi_message_networks_from_message_name(zwp_avi_get_message_list_networks(service->service_common->avi),
				message_name, (zwp_avi_message_networks_t **)&service->service_common->avi_message);

		}
		else if (!strncmp(message_name, "ifttt_interface_", sizeof("ifttt_interface_") - 1))
		{
			service->service_common->message_level = ZWP_AVI_MESSAGE_LEVEL_INTERFACES;

			return zwp_avi_get_avi_message_interfaces_from_message_name(zwp_avi_get_message_list_interfaces(service->service_common->avi),
				message_name, (zwp_avi_message_interfaces_t **)&service->service_common->avi_message);

		}
		else
		{
			service->service_common->message_level = ZWP_AVI_MESSAGE_LEVEL_ROOT;
			return zwp_avi_get_avi_message_from_message_name(zwp_avi_get_message_list_root(service->service_common->avi),
				message_name, (zwp_avi_message_t **)&service->service_common->avi_message);

		}
		//return zwp_avi_get_avi_message_ifttt_from_message_name(zwp_avi_get_message_list_ifttt(service->service_common->avi),
		//	message_name, (zwp_avi_message_ifttt_t **)&service->service_common->avi_message);
	}
	else if (!strncmp(message_name, "node_", sizeof("node_") - 1))
	{
		service->message_type = ZWP_AVI_MESSAGE_TYPE_NETWORKS;
		service->service_common->message_level = ZWP_AVI_MESSAGE_LEVEL_NODES;
		return zwp_avi_get_avi_message_nodes_from_message_name(zwp_avi_get_message_list_nodes(service->service_common->avi),
				message_name, (zwp_avi_message_nodes_t **)&service->service_common->avi_message);
	}
	else if (!strncmp(message_name, "endpoint_", sizeof("endpoint_") - 1))
	{
		service->message_type = ZWP_AVI_MESSAGE_TYPE_NETWORKS;
		service->service_common->message_level = ZWP_AVI_MESSAGE_LEVEL_ENDPOINTS;
		return zwp_avi_get_avi_message_endpoints_from_message_name(zwp_avi_get_message_list_endpoints(service->service_common->avi),
				message_name, (zwp_avi_message_endpoints_t **)&service->service_common->avi_message);
	}
	else if (!strncmp(message_name, "interface_", sizeof("interface_") - 1))
	{
		service->message_type = ZWP_AVI_MESSAGE_TYPE_NETWORKS;
		service->service_common->message_level = ZWP_AVI_MESSAGE_LEVEL_INTERFACES;
		return zwp_avi_get_avi_message_interfaces_from_message_name(zwp_avi_get_message_list_interfaces(service->service_common->avi),
				message_name, (zwp_avi_message_interfaces_t **)&service->service_common->avi_message);
	}

	ZWP_LOG(ZWP_LOG_ERR, "Failed to detect the message level for '%s'", message_name);

	return ZWP_STATUS_ERROR;
}

/**
 * @brief Allocate Avro value object for request
 * @param[in]     avro              Avro object
 * @param[out]    request_value_p   Avro value object for request
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_free_avro_value_of_request() to free resources
 */
static zwp_status_t zwp_avi_alloc_avro_value_of_request(zw_avro_t avro, avro_value_t *request_value_p)
{
	if (avro_generic_value_new(zw_avro_get_types_iface(avro), request_value_p) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get arg value from type interface: %s", avro_strerror());
		goto l_err_type_value;
	}

	return ZWP_STATUS_OK;

l_err_type_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Avro value object for request
 * @param[in]     request_value_p   Avro value object for request
 * @return        Nothing
 * @see zwp_avi_alloc_avro_value_of_request
 */
static void zwp_avi_free_avro_value_of_request(avro_value_t *request_value_p)
{
	avro_value_decref(request_value_p);
}

/**
 * @brief Allocate AVI service object
 * @param[in]     avi               AVI object
 * @param[in]     message_name      Message name
 * @param[in]     user              User name
 * @param[out]    service_p         The new service object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_free_avi_service_root() to free resources
 */
static zwp_status_t zwp_avi_alloc_avi_service_root(zwp_avi_t avi, char *message_name, char *user,
		zwp_service_t *service_p)
{
	zwp_service_t service;
	zwp_service_common_t service_common;

	if ((service = (zwp_service_t)ZWP_MALLOC(sizeof(*service))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_service_alloc;
	}

	if ((service_common = (zwp_service_common_t)ZWP_MALLOC(sizeof(*service_common))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_service_common_alloc;
	}

	service->service_common = service_common;
	service->service_next = NULL;

	service_common->user = user;
	service_common->message_name = message_name;

	zwp_avi_service_set_argument(service, NULL, NULL);
	zwp_avi_service_set_result(service, NULL, NULL);

	service_common->avi = avi;

	// Set message info
	if (zwp_avi_set_avi_message_info(service, message_name) != ZWP_STATUS_OK)
	{
		goto l_err_avi_set_avi_message_info;
	}

	if (service->message_type == ZWP_AVI_MESSAGE_TYPE_NETWORKS)
	{
		if (zwp_avi_alloc_avi_service_networks(service_common, (zwp_service_networks_t *)&service->service_next) != ZWP_STATUS_OK)
		{
			goto l_err_alloc_avi_service_networks;
		}
	}
	else if (service->message_type == ZWP_AVI_MESSAGE_TYPE_ROOT)
	{
		// Nothing to allocate for root level messages only
	}
	else if (service->message_type == ZWP_AVI_MESSAGE_TYPE_IFTTT)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "message_type: ZWP_AVI_MESSAGE_TYPE_IFTTT");

		if ((service->service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_NETWORKS) || 
			(service->service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_INTERFACES))
		{
			ZWP_LOG(ZWP_LOG_DEBUG, "message_level: %s", 
				(service->service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_NETWORKS) ? 
					"ZWP_AVI_MESSAGE_LEVEL_NETWORKS" : "ZWP_AVI_MESSAGE_LEVEL_INTERFACES");
			if (zwp_avi_alloc_avi_service_networks(service_common, (zwp_service_networks_t *)&service->service_next) != ZWP_STATUS_OK)
			{
				goto l_err_alloc_avi_service_ifttt_networks;
			}
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to match with supported messages: %s", message_name);
		goto l_err_message_no_match;
	}

	*service_p = service;

	return ZWP_STATUS_OK;

l_err_message_no_match:
l_err_alloc_avi_service_ifttt_networks:
l_err_alloc_avi_service_networks:
l_err_avi_set_avi_message_info:
	ZWP_FREE(service->service_common);
l_err_service_common_alloc:
	ZWP_FREE(service);
l_err_service_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free AVI service object
 * @param[in]     service           Service object
 * @return        Nothing
 * @see zwp_avi_alloc_avi_service_root
 */
static void zwp_avi_free_avi_service_root(zwp_service_t service)
{
	zwp_service_common_t service_common;

	service_common = service->service_common;

	if ((service->message_type == ZWP_AVI_MESSAGE_TYPE_NETWORKS) || 
		(service->message_type == ZWP_AVI_MESSAGE_TYPE_IFTTT))
	{
		zwp_avi_free_avi_service_networks((zwp_service_networks_t)service->service_next);
	}

	if (service_common->argument.free_handler)
	{
		service_common->argument.free_handler(service_common->argument.data);
	}

	if (service_common->result.free_handler)
	{
		service_common->result.free_handler(service_common->result.data);
	}

	ZWP_FREE(service->service_common);

	ZWP_FREE(service);
}

/**
 * @brief Get argument for the service
 * @param[in]     service           Service object
 * @param[in]     type_name         Avro type name
 * @param[out]    arg_value_p       Avro value object for argument
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_get_service_arg_root(zwp_service_t service, const char *type_name, avro_value_t *arg_value_p)
{
	avro_value_t command_value;
	avro_value_t parameter_value;
	zwp_avi_message_t *avi_message;
	void *args;

	if (!strcmp(type_name, "Void"))
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Request is 'Void'");
	}
	else if (service->service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_ROOT)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Avro type: '%s' (Root level)", type_name);

		if ((avro_value_get_by_name(arg_value_p, "command", &command_value, NULL)) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'command' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_command_value;
		}

		avi_message = (zwp_avi_message_t *)service->service_common->avi_message;

		// Optional field
		if ((avro_value_get_by_name(arg_value_p, "parameter", &parameter_value, NULL)) == 0)
		{
			if (avi_message->argument_handler)
			{
				if (avi_message->argument_handler(&parameter_value, &args) != ZWP_STATUS_OK)
				{
					ZWP_LOG(ZWP_LOG_ERR, "Failed to get arguments for type '%s'", type_name);
					goto l_err_argument_handler;
				}

				zwp_avi_service_set_argument(service, args, avi_message->argument_free_handler);
			}
		}
	}
	else
	{
		if (zwp_avi_get_service_arg_networks((zwp_service_networks_t)service->service_next, type_name, arg_value_p) != ZWP_STATUS_OK)
		{
			goto l_err_service_arg_networks;
		}
	}

	return ZWP_STATUS_OK;

l_err_service_arg_networks:
l_err_argument_handler:
l_err_get_command_value:
	return ZWP_STATUS_ERROR;
}

#if 0
static zwp_status_t zwp_avi_get_avro_value_of_record(zw_avro_t avro, zwp_service_t service,
		zw_avro_type_t type, const char * type_name,
		avro_schema_t arg_schema, avro_value_t *arg_value_p)
{
	int field_count;
	const char *field_name;
	const char *field_type_name;
	zw_avro_type_t field_type;
	avro_schema_t field_schema;
	avro_value_t field_value;
	int i;

	ZWP_LOG(ZWP_LOG_DEBUG, "Record: '%s' {", type_name);

	field_count = avro_schema_record_size(arg_schema);

	for (i = 0; i < field_count; i++)
	{
		if ((field_name = avro_schema_record_field_name(arg_schema, i)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get field name for record at index [%d]: %s", i, avro_strerror());
			goto l_err_record_field_name;
		}

		if ((field_schema = avro_schema_record_field_get(arg_schema, field_name)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get field schema '%s' for record at index [%d]: %s", field_name, i, avro_strerror());
			goto l_err_record_field_schema;
		}

		if (avro_value_get_by_name(arg_value_p, field_name, &field_value, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get field value '%s' for record at index [%d]: %s", field_name, i, avro_strerror());
			goto l_err_record_field_value;
		}

		ZWP_LOG(ZWP_LOG_DEBUG, "\tField: %s", field_name);

		if ((avro_typeof(field_schema) == AVRO_LINK)
				&& (avro_typeof(avro_schema_link_target(field_schema)) == AVRO_RECORD))
		{
			if ((field_type_name = avro_schema_type_name(field_schema)) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to get field type name for '%s' field for record at index [%d]: %s", field_name, i, avro_strerror());
				goto l_err_field_type_name;
			}

			if ((field_schema = avro_schema_get_subschema(zw_avro_get_types_schema(avro), field_type_name)) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to get the schema of '%s' type: %s", field_type_name, avro_strerror());
				goto l_err_link_target_schema;
			}

			if (zw_avro_get_type_from_type_name(field_type_name, &field_type) != ZWP_STATUS_OK)
			{
				goto l_err_type_from_type_name;
			}

			// Recursion
			if (zwp_avi_get_avro_value_of_record(avro, service, field_type, field_type_name, field_schema, &field_value) != ZWP_STATUS_OK)
			{
				goto l_err_request_args;
			}
		}

		if (zwp_avi_get_avro_value_field(service, type, type_name, field_name, &field_value) != ZWP_STATUS_OK)
		{
			goto l_err_set_field;
		}
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "} Record: '%s'", type_name);

	return ZWP_STATUS_OK;

l_err_set_field:
l_err_request_args:
l_err_type_from_type_name:
l_err_link_target_schema:
l_err_field_type_name:
l_err_record_field_value:
l_err_record_field_schema:
l_err_record_field_name:
	return ZWP_STATUS_ERROR;
}
#endif

/**
 * @brief Get AVI service object
 * @param[in]     avi               AVI object
 * @param[in]     message_name      Message name
 * @param[in]     user              User name
 * @param[out]    request_value_p   Avro value object for request
 * @param[out]    service_p         Service object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_free_avro_value_of_request() and zwp_avi_free_avi_service_root() to free resources
 */
static zwp_status_t zwp_avi_get_avi_service(zwp_avi_t avi, char *message_name, char *user,
		avro_value_t *request_value_p, zwp_service_t *service_p)
{
	zwp_service_t service;
	avro_value_t arg_value;
	avro_schema_t param_schema;
	const char *request_schema_name;

	// Get specific branch (return value) from union value
	if (avro_value_get_current_branch(request_value_p, &arg_value) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get current branch of request union: %s", avro_strerror());
		goto l_err_current_branch;
	}

	// Get schema from return value
	if ((param_schema = avro_value_get_schema(&arg_value)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get schema of return value: %s", avro_strerror());
		goto l_err_get_value_schema;
	}

	if ((request_schema_name = avro_schema_type_name(param_schema)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get type name for return schema: %s", avro_strerror());
		goto l_err_type_name;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "RPC: %s(%s)", message_name, request_schema_name);

	if (zwp_avi_alloc_avi_service_root(avi, message_name, user, &service) != ZWP_STATUS_OK)
	{
		goto l_err_get_service_root;
	}

	if ((avro_typeof(param_schema) == AVRO_RECORD)
			&& (zwp_avi_get_service_arg_root(service, request_schema_name, &arg_value) != ZWP_STATUS_OK))
	{
		goto l_err_request_args;
	}

	*service_p = service;

	return ZWP_STATUS_OK;

l_err_request_args:
	zwp_avi_free_avi_service_root(service);
l_err_get_service_root:
l_err_type_name:
l_err_get_value_schema:
l_err_current_branch:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free AVI service object
 * @param[in]     service           Service object
 * @return        Nothing
 * @see zwp_avi_free_avro_value_of_request
 */
static void zwp_avi_free_avi_service(zwp_service_t service)
{
	zwp_avi_free_avi_service_root(service);
}

zwp_status_t zwp_avi_get_avi_service_from_rpc_request(zwp_avi_t avi, char *rpc_request, int rpc_request_length,
		zwp_service_t *service_p)
{
	zw_avro_t avro;
	uint32_t avro_message_length;
	avro_reader_t reader;
	avro_value_t cookies_value;
	avro_value_t message_name_value;
	avro_value_t request_value;
	char *user;
	char *message_name;

	avro = zwp_avi_get_avro(avi);

	memcpy(&avro_message_length, rpc_request + ZW_AVRO_FRAME_MESSAGE_LENGTH_OFFSET, ZW_AVRO_FRAME_MESSAGE_LENGTH_SIZE);
	// Network byte order to Host byte order
	avro_message_length = ntohl((uint32_t)avro_message_length);

	//ZWP_LOG(ZWP_LOG_DEBUG, "Avro message length: %u", avro_message_length);

	if ((reader = avro_reader_memory(rpc_request + ZW_AVRO_FRAME_MESSAGE_DATA_OFFSET, avro_message_length)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create avro reader: %s", avro_strerror());
		goto l_err_avro_reader;
	}

	// Allocate cookies
	if(zwp_avi_alloc_avro_value_of_cookies(avro, &cookies_value) != ZWP_STATUS_OK)
	{
		goto l_err_alloc_cookies;
	}

	if (avro_value_read(reader, &cookies_value) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to read cookies value: %s", avro_strerror());
		goto l_err_read_cookies;
	}

	// Get cookies
	if (zwp_avi_get_user(&cookies_value, &user) != ZWP_STATUS_OK)
	{
		goto l_err_get_cookies;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "User: %s", user);

	// Allocate message name
	if(zwp_avi_alloc_avro_value_of_message_name(avro, &message_name_value) != ZWP_STATUS_OK)
	{
		goto l_err_alloc_message_name;
	}

	if (avro_value_read(reader, &message_name_value) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to read message name value: %s", avro_strerror());
		goto l_err_read_message_name;
	}

	// Get message name
	if (zwp_avi_get_message_name(&message_name_value, &message_name) != ZWP_STATUS_OK)
	{
		goto l_err_get_message_name;
	}

	//ZWP_LOG(ZWP_LOG_DEBUG, "Message name: %s", message_name);

	// Allocate request
	if(zwp_avi_alloc_avro_value_of_request(avro, &request_value) != ZWP_STATUS_OK)
	{
		goto l_err_alloc_request;
	}

	if (avro_value_read(reader, &request_value) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to read request value: %s", avro_strerror());
		goto l_err_read_request;
	}

	// Get service from request
	if (zwp_avi_get_avi_service(avi, message_name, user, &request_value, service_p) != ZWP_STATUS_OK)
	{
		goto l_err_service_arg;
	}

	zwp_avi_free_avro_value_of_request(&request_value);
	zwp_avi_free_avro_value_of_message_name(&message_name_value);
	zwp_avi_free_avro_value_of_cookies(&cookies_value);

	avro_reader_free(reader);

	return ZWP_STATUS_OK;

l_err_service_arg:
l_err_read_request:
	zwp_avi_free_avro_value_of_request(&request_value);
l_err_alloc_request:

	zwp_avi_free_message_name(message_name);
l_err_get_message_name:
l_err_read_message_name:
	zwp_avi_free_avro_value_of_message_name(&message_name_value);
l_err_alloc_message_name:

	zwp_avi_free_user(user);
l_err_get_cookies:
l_err_read_cookies:
	zwp_avi_free_avro_value_of_cookies(&cookies_value);
l_err_alloc_cookies:
	avro_reader_free(reader);
l_err_avro_reader:

	return ZWP_STATUS_ERROR;
}

void zwp_avi_free_avi_service_from_rpc_request(zwp_service_t service)
{
	zwp_avi_free_message_name(service->service_common->message_name);

	zwp_avi_free_user(service->service_common->user);

	zwp_avi_free_avi_service(service);
}

/*
 * Output
 */
/**
 * @brief Get result of the service
 * @param[in]     service           Service object
 * @param[in]     type_name         Avro type name
 * @param[out]    arg_value_p       Avro value object for result
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_set_avro_value_of_response_with_result_root(zwp_service_t service, const char *type_name, avro_value_t *arg_value_p)
{
	avro_value_t command_value;
	avro_value_t parameter_value;
	zwp_avi_message_t *avi_message;

	if (!strcmp(type_name, "Void"))
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Response is 'Void'");
	}
	else if (service->service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_ROOT)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Avro type: '%s' (Root level)", type_name);

		if ((avro_value_get_by_name(arg_value_p, "command", &command_value, NULL)) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'command' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_command_value;
		}

		avi_message = (zwp_avi_message_t *)service->service_common->avi_message;

		// Optional field
		if ((avro_value_get_by_name(arg_value_p, "parameter", &parameter_value, NULL)) == 0)
		{
			if (avi_message->result_handler)
			{
				if (avi_message->result_handler(service->service_common->result.data, &parameter_value) != ZWP_STATUS_OK)
				{
					ZWP_LOG(ZWP_LOG_ERR, "Failed to set results for type '%s'", type_name);
					goto l_err_result_handler;
				}
			}
		}
	}
	else
	{
		if (zwp_avi_set_avro_value_of_response_with_result_networks((zwp_service_networks_t)service->service_next, type_name, arg_value_p) != ZWP_STATUS_OK)
		{
			goto l_err_service_arg_networks;
		}
	}

	return ZWP_STATUS_OK;

l_err_service_arg_networks:
l_err_result_handler:
l_err_get_command_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Get Avro value object of service response
 * @param[in]     service           Service object
 * @param[out]    response_value_p  Avro value object for response
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_destroy_avro_value_of_response() to free resources
 */
static zwp_status_t zwp_avi_create_avro_value_of_response(zwp_service_t service, avro_value_t *response_value_p)
{
	zw_avro_t avro;
	const char *message_name;
	avro_schema_t message_schema;
	avro_schema_t response_schema;
	const char *response_schema_name;
	avro_schema_t param_schema;
	avro_value_t arg_value;
	int type_index;

	avro = zwp_avi_get_avro(service->service_common->avi);
	message_name = service->service_common->message_name;

	if ((message_schema = avro_schema_get_subschema(zw_avro_get_messages_schema(avro), message_name)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the schema of '%s' message: %s", message_name, avro_strerror());
		goto l_err_message_schema;
	}

	if ((response_schema = avro_schema_get_subschema(message_schema, "response")) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the response schema of '%s' message: %s", message_name, avro_strerror());
		goto l_err_response_schema;
	}

	if ((response_schema_name = avro_schema_type_name(response_schema)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get type name for response schema of '%s' message: %s", message_name, avro_strerror());
		goto l_err_type_name;
	}

	if ((param_schema = avro_schema_union_branch_by_name(zw_avro_get_types_schema(avro), &type_index, response_schema_name)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get '%s' schema from 'types' schema: %s", response_schema_name, avro_strerror());
		goto l_err_arg_type_index;
	}

	if (avro_generic_value_new(zw_avro_get_types_iface(avro), response_value_p) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get arg value from type interface '%s': %s", response_schema_name, avro_strerror());
		goto l_err_type_value;
	}

	if (avro_value_set_branch(response_value_p, type_index, &arg_value) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to set 'user' field to branch '0': %s", avro_strerror());
		goto l_err_set_branch_type;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "RPC: %s %s(<argument>)", response_schema_name, message_name);

	if ((avro_typeof(param_schema) == AVRO_RECORD)
			&& (zwp_avi_set_avro_value_of_response_with_result_root(service, response_schema_name, &arg_value) != ZWP_STATUS_OK))
	{
		goto l_err_response_args;
	}

	return ZWP_STATUS_OK;

l_err_response_args:
l_err_set_branch_type:
	avro_value_decref(response_value_p);
l_err_type_value:
l_err_arg_type_index:
l_err_type_name:
l_err_response_schema:
l_err_message_schema:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Avro value object of service response
 * @param[in]     response_value_p  Avro value object for response
 * @return        Nothing
 * @see zwp_avi_create_avro_value_of_response
 */
static void zwp_avi_destroy_avro_value_of_response(avro_value_t *response_value_p)
{
	avro_value_decref(response_value_p);
}

zwp_status_t zwp_avi_get_rpc_response_from_avi_service(zwp_service_t service,
		char **rpc_response_p, int *rpc_response_length)
{
	avro_writer_t writer;
	avro_value_t response_value;
	char *rpc_response, *tmp;
	long written;
	int size;
	int avro_code;
	uint32_t avro_message_length;

	// Get response parameters
	if(zwp_avi_create_avro_value_of_response(service, &response_value) != ZWP_STATUS_OK)
	{
		goto l_err_get_response;
	}

	// Write with on-demand memory allocation
	rpc_response = NULL;
	written = 0;
	size = 0;

	while (1)
	{
		size += ZW_AVRO_WRITER_MEMORY_DEFAULT_BLOCK_SIZE;

		if ((tmp = (char *)ZWP_REALLOC(rpc_response, size)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_realloc;
		}

		rpc_response = tmp;

		if ((writer = avro_writer_memory(rpc_response + ZW_AVRO_FRAME_MESSAGE_DATA_OFFSET,
				size - ZW_AVRO_FRAME_MESSAGE_DATA_OFFSET)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to create avro writer: %s", avro_strerror());
			goto l_err_avro_writer;
		}

		// Write cookies, message and its response parameter
		if ((avro_code = avro_value_write(writer, &response_value)) == 0)
		{
			//Success
			break;
		}
		else if (avro_code != ENOSPC)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to write 'Cookie' and 'Request' value: %s", avro_strerror());
			goto l_err_value_write;
		}

		// ENOSPC, so free the writer and retry with bigger buffer
		avro_writer_free(writer);
	}

	if ((written = avro_writer_tell(writer)) < 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get buffer size from writer: %s", avro_strerror());
		goto l_err_writer_tell;
	}

	zwp_avi_destroy_avro_value_of_response(&response_value);

	avro_writer_free(writer);

	if (written > ZW_AVRO_FRAME_MESSAGE_LENGTH_MASK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to build Avro frame: Message length is too long");
		goto l_err_length_too_long;
	}

	avro_message_length = htonl((uint32_t)written);
	// Host byte order to Network byte order
	memcpy(rpc_response + ZW_AVRO_FRAME_MESSAGE_LENGTH_OFFSET, &avro_message_length, ZW_AVRO_FRAME_MESSAGE_LENGTH_SIZE);

	*rpc_response_p = rpc_response;
	*rpc_response_length = written + ZW_AVRO_FRAME_MESSAGE_LENGTH_SIZE;

	return ZWP_STATUS_OK;

l_err_length_too_long:
l_err_writer_tell:
l_err_value_write:
	avro_writer_free(writer);
l_err_avro_writer:
l_err_realloc:
	ZWP_FREE(rpc_response);
	zwp_avi_destroy_avro_value_of_response(&response_value);
l_err_get_response:
	return ZWP_STATUS_ERROR;
}

void zwp_avi_free_rpc_response_from_avi_service(char *rpc_response)
{
	ZWP_FREE(rpc_response);
}

