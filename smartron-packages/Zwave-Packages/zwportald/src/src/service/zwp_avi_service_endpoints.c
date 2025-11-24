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
 * @file    zwp_avi_service_endpoints.c
 *
 * @brief   Avro Interface service - Endpoints Portion
 * @details Handles interfaces portion of Avro Interface service parameters.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-20
 * - Initial version
 */

#include <stdio.h>

#include "zwp_log.h"
#if defined(ZWP_DEBUG_AVI_SERVICE) || defined(ZWP_DEBUG_AVI_SERVICE_ENDPOINTS)
#include "zwp_log_enable.h"
#else
#include "zwp_log_disable.h"
#endif

#include "zwp_avi_service_endpoints.h"
#include "zwp_avi_service_nodes.h"
#include "zwp_avi_service_interfaces.h"
#include "zwp_avi_common_networks.h"
#include "zwp_descriptor_define.h"
#include "zwp_network.h"
#include "zwp_descriptor.h"
#include "zwp_error.h"
#include "zwp_memory.h"
#include "zwp_io.h"

/** AVI Endpoint level Service class */
struct _zwp_service_endpoints
{
	/* Level specific */
	int                        endpoint_id;                 /**< Endpoint ID */
	zwp_service_nodes_t        service_node;                /**< Back reference to 'Node' level service */
	zwp_service_interfaces_t   service_interface;           /**< Reference to 'Interface' level service */

	/* Common to all levels */
	zwp_service_common_t       service_common;              /**< Parameters common to all levels */
};

/**
 * @name Getters
 * @{
 */
int zwp_avi_service_endpoints_get_endpoint_id(zwp_service_endpoints_t service_endpoint)
{
	return service_endpoint->endpoint_id;
}

int zwp_avi_service_endpoints_get_node_id(zwp_service_endpoints_t service_endpoint)
{
	return zwp_avi_service_nodes_get_node_id(service_endpoint->service_node);
}

zwp_avi_message_level_t zwp_avi_service_endpoints_get_message_level(zwp_service_endpoints_t service_endpoint)
{
	return service_endpoint->service_common->message_level;
}

zwp_avi_message_endpoints_execute_t zwp_avi_service_endpoints_get_avi_message_endpoints_execute_handler(zwp_service_endpoints_t service_endpoint)
{
	return ((zwp_avi_message_endpoints_t *)service_endpoint->service_common->avi_message)->execute_handler;
}

zwp_avi_message_result_free_t zwp_avi_service_endpoints_get_avi_message_result_free_handler(zwp_service_endpoints_t service_endpoint)
{
	return ((zwp_avi_message_endpoints_t *)service_endpoint->service_common->avi_message)->result_free_handler;
}

zwp_service_interfaces_t zwp_avi_service_endpoints_get_service_interface(zwp_service_endpoints_t service_endpoint)
{
	return service_endpoint->service_interface;
}
/**
 * @}
 */

/**
 * @brief Set argument for a endpoint service object
 * @param[in]     service_endpoint  Endpoint service object
 * @param[in]     argument_data     Argument data
 * @param[in]     free_handler      Argument free handler
 * @return        Nothing
 */
static void zwp_avi_service_endpoints_set_argument(zwp_service_endpoints_t service_endpoint, void *argument_data, zwp_avi_message_argument_free_t free_handler)
{
	service_endpoint->service_common->argument.data = argument_data;
	service_endpoint->service_common->argument.free_handler = free_handler;
}

/**
 * @brief Set result for a endpoint service object
 * @param[in]     service_endpoint  Endpoint service object
 * @param[in]     result_data       Result data
 * @param[in]     free_handler      Result free handler
 * @return        Nothing
 */
static void zwp_avi_service_endpoints_set_result(zwp_service_endpoints_t service_endpoint, void *result_data, zwp_avi_message_result_free_t free_handler)
{
	service_endpoint->service_common->result.data = result_data;
	service_endpoint->service_common->result.free_handler = free_handler;
}

zwp_status_t zwp_avi_service_endpoints_execute_avi_messages(zwp_service_endpoints_t service_endpoint, zwp_net_t net)
{
	zwp_desc_net_t desc_net;
	zwepd_p desc_endpoint;
	zwp_avi_message_level_t message_level;
	zwp_avi_message_endpoints_execute_t execute_handler;
	zwp_avi_message_result_free_t result_free_handler;

	message_level = zwp_avi_service_endpoints_get_message_level(service_endpoint);

	if (message_level == ZWP_AVI_MESSAGE_LEVEL_ENDPOINTS)
	{
		execute_handler = zwp_avi_service_endpoints_get_avi_message_endpoints_execute_handler(service_endpoint);
		result_free_handler = zwp_avi_service_endpoints_get_avi_message_result_free_handler(service_endpoint);

		if (zwp_net_desc_container_lock_and_test(net) != ZWP_STATUS_OK)
		{
			goto l_err_desc_container_net_lock;
		}

		desc_net = zwp_net_get_desc_net(net);

		if (zwp_desc_get_desc_endpoint(desc_net,
				zwp_avi_service_endpoints_get_node_id(service_endpoint),
				zwp_avi_service_endpoints_get_endpoint_id(service_endpoint),
				&desc_endpoint) != ZWP_STATUS_OK)
		{
			goto l_err_get_desc_endpoint;
		}

		if (execute_handler(desc_endpoint,
				service_endpoint->service_common->argument.data,
				&service_endpoint->service_common->result.data) != ZWP_STATUS_OK)
		{
			goto l_err_execute;
		}

		zwp_net_desc_container_unlock(net);

		zwp_avi_service_endpoints_set_result(service_endpoint,
				service_endpoint->service_common->result.data, result_free_handler);
	}
	else
	{
		if (zwp_avi_service_interfaces_execute_avi_messages(zwp_avi_service_endpoints_get_service_interface(service_endpoint), net) != ZWP_STATUS_OK)
		{
			goto l_err_execute_avi_message_interfaces;
		}
	}

	return ZWP_STATUS_OK;

l_err_execute_avi_message_interfaces:
l_err_execute:
l_err_get_desc_endpoint:
	if (message_level == ZWP_AVI_MESSAGE_LEVEL_ENDPOINTS)
	{
		zwp_net_desc_container_unlock(net);
	}
l_err_desc_container_net_lock:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_avi_get_avro_value_of_field_endpoints(zwp_service_endpoints_t service_endpoint, const char *type_name, avro_value_t *arg_value_p)
{
	int avro_code;
	zwp_status_t return_status;
	avro_value_t field_value;

	//ZWP_LOG(ZWP_LOG_DEBUG, "Avro type: '%s' (Endpoint level)", type_name);

	return_status = ZWP_STATUS_OK;
	avro_code = 0;

	if (!strcmp(type_name, "Endpoint"))
	{
		if ((avro_code = avro_value_get_by_name(arg_value_p, "node", &field_value, NULL)) == 0)
		{
			if ((return_status = zwp_avi_get_avro_value_of_field_nodes(service_endpoint->service_node,
					"Node", &field_value)) == ZWP_STATUS_OK)
			{
				return_status = zw_avro_field_get_int(arg_value_p, "id", &service_endpoint->endpoint_id);
			}
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_ERR, "Avro type '%s' is not supported at endpoint level", type_name);
		return_status = ZWP_STATUS_ERROR;
	}

	if (return_status != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get argument of type '%s'", type_name);
	}
	else if (avro_code != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get argument of type '%s': %s", type_name, avro_strerror());

		return_status = ZWP_STATUS_ERROR;
	}

	return return_status;
}

zwp_status_t zwp_avi_get_service_arg_endpoints(zwp_service_endpoints_t service_endpoint, const char *type_name, avro_value_t *arg_value_p)
{
	avro_value_t command_value;
	avro_value_t endpoint_value;
	avro_value_t parameter_value;
	zwp_avi_message_endpoints_t *avi_message;
	void *args;

	if (service_endpoint->service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_ENDPOINTS)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Avro type: '%s' (Endpoint level)", type_name);

		if ((avro_value_get_by_name(arg_value_p, "command", &command_value, NULL)) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'command' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_command_value;
		}

		if (avro_value_get_by_name(&command_value, "endpoint", &endpoint_value, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'endpoint' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_endpoint_value;
		}

		if (zwp_avi_get_avro_value_of_field_endpoints(service_endpoint, "Endpoint", &endpoint_value) != ZWP_STATUS_OK)
		{
			goto l_err_field_endpoints;
		}

		avi_message = (zwp_avi_message_endpoints_t *)service_endpoint->service_common->avi_message;

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

				ZWP_LOG(ZWP_LOG_DEBUG, "NAME-LOC: %p", args);
				zwp_avi_service_endpoints_set_argument(service_endpoint, args, avi_message->argument_free_handler);
			}
		}
	}
	else
	{
		if (zwp_avi_get_service_arg_interfaces(service_endpoint->service_interface, type_name, arg_value_p) != ZWP_STATUS_OK)
		{
			goto l_err_service_arg_interfaces;
		}
	}

	return ZWP_STATUS_OK;

l_err_service_arg_interfaces:
l_err_argument_handler:
l_err_field_endpoints:
l_err_get_endpoint_value:
l_err_get_command_value:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_avi_alloc_avi_service_endpoints(zwp_service_common_t service_common, zwp_service_nodes_t service_node,
		zwp_service_endpoints_t *service_endpoint_p)
{
	zwp_service_endpoints_t service_endpoint;

	if ((service_endpoint = (zwp_service_endpoints_t)ZWP_MALLOC(sizeof(*service_endpoint))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_service_endpoint_alloc;
	}

	service_endpoint->endpoint_id = ZWP_AVI_ENDPOINT_ID_NONE;
	service_endpoint->service_interface = NULL;

	service_endpoint->service_common = service_common;
	service_endpoint->service_node = service_node;

	if (service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_INTERFACES)
	{
		if (zwp_avi_alloc_avi_service_interfaces(service_common,
				service_node, service_endpoint, &service_endpoint->service_interface) != ZWP_STATUS_OK)
		{
			goto l_err_alloc_avi_service_interfaces;
		}
	}

	*service_endpoint_p = service_endpoint;

	return ZWP_STATUS_OK;

l_err_alloc_avi_service_interfaces:
	ZWP_FREE(service_endpoint);
l_err_service_endpoint_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_avi_free_avi_service_endpoints(zwp_service_endpoints_t service_endpoint)
{
	if (service_endpoint->service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_INTERFACES)
	{
		zwp_avi_free_avi_service_interfaces(service_endpoint->service_interface);
	}
	ZWP_FREE(service_endpoint);
}

/*
 * Output
 */
zwp_status_t zwp_avi_set_avro_value_of_field_endpoints(zwp_service_endpoints_t service_endpoint,
		const char *type_name, avro_value_t *arg_value_p)
{
	avro_value_t field_value;
	int avro_code;
	zwp_status_t return_status;

	//ZWP_LOG(ZWP_LOG_DEBUG, "Avro type: '%s' (Endpoint level)", type_name);

	return_status = ZWP_STATUS_OK;
	avro_code = 0;

	if (!strcmp(type_name, "Endpoint"))
	{
		if ((avro_code = avro_value_get_by_name(arg_value_p, "node", &field_value, NULL)) == 0)
		{
			if ((return_status = zwp_avi_set_avro_value_of_field_nodes(service_endpoint->service_node,
					"Node", &field_value)) == ZWP_STATUS_OK)
			{
				return_status = zw_avro_field_set_int(arg_value_p, "id", service_endpoint->endpoint_id);
			}
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_ERR, "Avro type '%s' is not supported at endpoint level", type_name);
		return_status = ZWP_STATUS_ERROR;
	}

	if (return_status != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to set argument of type '%s'", type_name);
	}
	else if (avro_code != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to set argument of type '%s': %s", type_name, avro_strerror());

		return_status = ZWP_STATUS_ERROR;
	}

	return return_status;
}

zwp_status_t zwp_avi_set_avro_value_of_response_with_result_endpoints(zwp_service_endpoints_t service_endpoint,
		const char *type_name, avro_value_t *arg_value_p)
{
	avro_value_t command_value;
	avro_value_t endpoint_value;
	avro_value_t parameter_value;
	zwp_avi_message_endpoints_t *avi_message;

	if (service_endpoint->service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_ENDPOINTS)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Avro type: '%s' (Endpoint level)", type_name);

		if ((avro_value_get_by_name(arg_value_p, "command", &command_value, NULL)) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'command' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_command_value;
		}

		if (avro_value_get_by_name(&command_value, "endpoint", &endpoint_value, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'endpoint' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_endpoint_value;
		}

		if (zwp_avi_set_avro_value_of_field_endpoints(service_endpoint, "Endpoint", &endpoint_value) != ZWP_STATUS_OK)
		{
			goto l_err_field_endpoints;
		}

		avi_message = (zwp_avi_message_endpoints_t *)service_endpoint->service_common->avi_message;

		// Optional field
		if ((avro_value_get_by_name(arg_value_p, "parameter", &parameter_value, NULL)) == 0)
		{
		if (avi_message->result_handler)
			{
				if (avi_message->result_handler(service_endpoint->service_common->result.data, &parameter_value) != ZWP_STATUS_OK)
				{
					ZWP_LOG(ZWP_LOG_ERR, "Failed to set results for type '%s'", type_name);
					goto l_err_result_handler;
				}
			}
		}
	}
	else
	{
		if (zwp_avi_set_avro_value_of_response_with_result_interfaces(service_endpoint->service_interface, type_name, arg_value_p) != ZWP_STATUS_OK)
		{
			goto l_err_service_arg_interfaces;
		}
	}

	return ZWP_STATUS_OK;

l_err_service_arg_interfaces:
l_err_result_handler:
l_err_field_endpoints:
l_err_get_endpoint_value:
l_err_get_command_value:
	return ZWP_STATUS_ERROR;
}
