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
 * @file    zwp_avi_service_nodes.c
 *
 * @brief   Avro Interface Input - Nodes Portion
 * @details Handles nodes portion of Avro Interface Input parameters.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-17
 * - Initial version
 */

#include <stdio.h>

#include "zwp_log.h"
#if defined(ZWP_DEBUG_AVI_SERVICE) || defined(ZWP_DEBUG_AVI_SERVICE_NODES)
#include "zwp_log_enable.h"
#else
#include "zwp_log_disable.h"
#endif

#include "zwp_avi_service_nodes.h"
#include "zwp_avi_service_networks.h"
#include "zwp_avi_service_endpoints.h"
#include "zwp_avi_common_networks.h"
#include "zwp_descriptor_define.h"
#include "zwp_network.h"
#include "zwp_descriptor.h"
#include "zwp_error.h"
#include "zwp_memory.h"
#include "zwp_io.h"

/** AVI Node level Service class */
struct _zwp_service_nodes
{
	/* Level specific */
	int                        node_id;                       /**< Node ID */
	zwp_service_networks_t     service_net;                   /**< Back reference to 'Network' level service */
	zwp_service_endpoints_t    service_endpoint;              /**< Reference to 'Endpoint' level service */

	/* Common to all levels */
	zwp_service_common_t       service_common;                /**< Parameters common to all levels */
};

/**
 * @name Getters
 * @{
 */
int zwp_avi_service_nodes_get_node_id(zwp_service_nodes_t service_node)
{
	return service_node->node_id;
}

zwp_avi_message_level_t zwp_avi_service_nodes_get_message_level(zwp_service_nodes_t service_node)
{
	return service_node->service_common->message_level;
}

zwp_avi_message_nodes_execute_t zwp_avi_service_nodes_get_avi_message_nodes_execute_handler(zwp_service_nodes_t service_node)
{
	return ((zwp_avi_message_nodes_t *)service_node->service_common->avi_message)->execute_handler;
}

zwp_avi_message_result_free_t zwp_avi_service_nodes_get_avi_message_result_free_handler(zwp_service_nodes_t service_node)
{
	return ((zwp_avi_message_nodes_t *)service_node->service_common->avi_message)->result_free_handler;
}

zwp_service_endpoints_t zwp_avi_service_nodes_get_service_endpoint(zwp_service_nodes_t service_node)
{
	return service_node->service_endpoint;
}
/**
 * @}
 */

/**
 * @brief Set argument for a node service object
 * @param[in]     service_node      Node service object
 * @param[in]     argument_data     Argument data
 * @param[in]     free_handler      Argument free handler
 * @return        Nothing
 */
static void zwp_avi_service_nodes_set_argument(zwp_service_nodes_t service_node, void *argument_data, zwp_avi_message_argument_free_t free_handler)
{
	service_node->service_common->argument.data = argument_data;
	service_node->service_common->argument.free_handler = free_handler;
}

/**
 * @brief Set result for a node service object
 * @param[in]     service_node      Node service object
 * @param[in]     result_data       Result data
 * @param[in]     free_handler      Result free handler
 * @return        Nothing
 */
static void zwp_avi_service_nodes_set_result(zwp_service_nodes_t service_node, void *result_data, zwp_avi_message_result_free_t free_handler)
{
	service_node->service_common->result.data = result_data;
	service_node->service_common->result.free_handler = free_handler;
}

zwp_status_t zwp_avi_service_nodes_execute_avi_messages(zwp_service_nodes_t service_node, zwp_net_t net)
{
	zwp_desc_net_t desc_net;
	zwnoded_p desc_node;
	zwp_avi_message_level_t message_level;
	zwp_avi_message_nodes_execute_t execute_handler;
	zwp_avi_message_result_free_t result_free_handler;

	message_level = zwp_avi_service_nodes_get_message_level(service_node);

	if (message_level == ZWP_AVI_MESSAGE_LEVEL_NODES)
	{
		execute_handler = zwp_avi_service_nodes_get_avi_message_nodes_execute_handler(service_node);
		result_free_handler = zwp_avi_service_nodes_get_avi_message_result_free_handler(service_node);

		if (zwp_net_desc_container_lock_and_test(net) != ZWP_STATUS_OK)
		{
			goto l_err_desc_container_net_lock;
		}

		desc_net = zwp_net_get_desc_net(net);

		if (zwp_desc_get_desc_node(desc_net, zwp_avi_service_nodes_get_node_id(service_node), &desc_node) != ZWP_STATUS_OK)
		{
			goto l_err_get_desc_node;
		}

		if (execute_handler(desc_node,
				service_node->service_common->argument.data,
				&service_node->service_common->result.data) != ZWP_STATUS_OK)
		{
			goto l_err_execute;
		}

		zwp_net_desc_container_unlock(net);

		zwp_avi_service_nodes_set_result(service_node,
				service_node->service_common->result.data, result_free_handler);
	}
	else
	{
		if (zwp_avi_service_endpoints_execute_avi_messages(zwp_avi_service_nodes_get_service_endpoint(service_node), net) != ZWP_STATUS_OK)
		{
			goto l_err_execute_avi_message_endpoints;
		}
	}

	return ZWP_STATUS_OK;

l_err_execute_avi_message_endpoints:
l_err_execute:
l_err_get_desc_node:
	if (message_level == ZWP_AVI_MESSAGE_LEVEL_NODES)
	{
		zwp_net_desc_container_unlock(net);
	}
l_err_desc_container_net_lock:
	return ZWP_STATUS_ERROR;
}

/*
 * Input
 */
zwp_status_t zwp_avi_get_avro_value_of_field_nodes(zwp_service_nodes_t service_node, const char *type_name, avro_value_t *arg_value_p)
{
	int avro_code;
	zwp_status_t return_status;
	avro_value_t field_value;

	//ZWP_LOG(ZWP_LOG_DEBUG, "Avro type: '%s' (Node level)", type_name);

	return_status = ZWP_STATUS_OK;
	avro_code = 0;

	if (!strcmp(type_name, "Node"))
	{
		if ((avro_code = avro_value_get_by_name(arg_value_p, "network", &field_value, NULL)) == 0)
		{
			if ((return_status = zwp_avi_get_avro_value_of_field_networks(service_node->service_net,
					"Network", &field_value)) == ZWP_STATUS_OK)
			{
				return_status = zw_avro_field_get_int(arg_value_p, "id", &service_node->node_id);
			}
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_ERR, "Avro type '%s' is not supported at node level", type_name);
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

zwp_status_t zwp_avi_get_service_arg_nodes(zwp_service_nodes_t service_node, const char *type_name, avro_value_t *arg_value_p)
{
	avro_value_t command_value;
	avro_value_t node_value;
	avro_value_t parameter_value;
	zwp_avi_message_nodes_t *avi_message;
	void *args;

	if (service_node->service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_NODES)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Avro type: '%s' (Node level)", type_name);

		if ((avro_value_get_by_name(arg_value_p, "command", &command_value, NULL)) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'command' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_command_value;
		}

		if (avro_value_get_by_name(&command_value, "node", &node_value, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'node' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_node_value;
		}

		if (zwp_avi_get_avro_value_of_field_nodes(service_node, "Node", &node_value) != ZWP_STATUS_OK)
		{
			goto l_err_field_nodes;
		}

		avi_message = (zwp_avi_message_nodes_t *)service_node->service_common->avi_message;

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

				zwp_avi_service_nodes_set_argument(service_node, args, avi_message->argument_free_handler);
			}
		}
	}
	else
	{
		if (zwp_avi_get_service_arg_endpoints(service_node->service_endpoint, type_name, arg_value_p) != ZWP_STATUS_OK)
		{
			goto l_err_service_arg_endpoints;
		}
	}

	return ZWP_STATUS_OK;

l_err_service_arg_endpoints:
l_err_argument_handler:
l_err_field_nodes:
l_err_get_node_value:
l_err_get_command_value:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_avi_alloc_avi_service_nodes(zwp_service_common_t service_common, zwp_service_networks_t service_net,
		zwp_service_nodes_t *service_node_p)
{
	zwp_service_nodes_t service_node;

	if ((service_node = (zwp_service_nodes_t)ZWP_MALLOC(sizeof(*service_node))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_service_node_alloc;
	}

	service_node->node_id = ZWP_AVI_NODE_ID_NONE;
	service_node->service_endpoint = NULL;

	service_node->service_common = service_common;
	service_node->service_net = service_net;

	if ((service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_ENDPOINTS)
			|| (service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_INTERFACES))
	{
		if (zwp_avi_alloc_avi_service_endpoints(service_common, service_node, &service_node->service_endpoint) != ZWP_STATUS_OK)
		{
			goto l_err_alloc_avi_service_endpoints;
		}
	}

	*service_node_p = service_node;

	return ZWP_STATUS_OK;

l_err_alloc_avi_service_endpoints:
	ZWP_FREE(service_node);
l_err_service_node_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_avi_free_avi_service_nodes(zwp_service_nodes_t service_node)
{
	if ((service_node->service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_ENDPOINTS)
			|| (service_node->service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_INTERFACES))
	{
		zwp_avi_free_avi_service_endpoints(service_node->service_endpoint);
	}

	ZWP_FREE(service_node);
}

/*
 * Output
 */
zwp_status_t zwp_avi_set_avro_value_of_field_nodes(zwp_service_nodes_t service_node,
		const char *type_name, avro_value_t *arg_value_p)
{
	avro_value_t field_value;
	int avro_code;
	zwp_status_t return_status;

	//ZWP_LOG(ZWP_LOG_DEBUG, "Avro type: '%s' (Node level)", type_name);

	return_status = ZWP_STATUS_OK;
	avro_code = 0;

	if (!strcmp(type_name, "Node"))
	{
		if ((avro_code = avro_value_get_by_name(arg_value_p, "network", &field_value, NULL)) == 0)
		{
			if ((return_status = zwp_avi_set_avro_value_of_field_networks(service_node->service_net,
					"Network", &field_value)) == ZWP_STATUS_OK)
			{
				return_status = zw_avro_field_set_int(arg_value_p, "id", service_node->node_id);
			}
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_ERR, "Avro type '%s' is not supported at node level", type_name);
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

zwp_status_t zwp_avi_set_avro_value_of_response_with_result_nodes(zwp_service_nodes_t service_node,
		const char *type_name, avro_value_t *arg_value_p)
{
	avro_value_t command_value;
	avro_value_t node_value;
	avro_value_t parameter_value;
	zwp_avi_message_nodes_t *avi_message;

	if (service_node->service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_NODES)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Avro type: '%s' (Node level)", type_name);

		if ((avro_value_get_by_name(arg_value_p, "command", &command_value, NULL)) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'command' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_command_value;
		}

		if (avro_value_get_by_name(&command_value, "node", &node_value, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'node' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_node_value;
		}

		if (zwp_avi_set_avro_value_of_field_nodes(service_node, "Node", &node_value) != ZWP_STATUS_OK)
		{
			goto l_err_field_nodes;
		}

		avi_message = (zwp_avi_message_nodes_t *)service_node->service_common->avi_message;

		// Optional field
		if ((avro_value_get_by_name(arg_value_p, "parameter", &parameter_value, NULL)) == 0)
		{
			if (avi_message->result_handler)
			{
				if (avi_message->result_handler(service_node->service_common->result.data, &parameter_value) != ZWP_STATUS_OK)
				{
					ZWP_LOG(ZWP_LOG_ERR, "Failed to set results for type '%s'", type_name);
					goto l_err_result_handler;
				}
			}
		}
	}
	else
	{
		if (zwp_avi_set_avro_value_of_response_with_result_endpoints(service_node->service_endpoint, type_name, arg_value_p) != ZWP_STATUS_OK)
		{
			goto l_err_service_arg_endpoints;
		}
	}

	return ZWP_STATUS_OK;

l_err_service_arg_endpoints:
l_err_result_handler:
l_err_field_nodes:
l_err_get_node_value:
l_err_get_command_value:
	return ZWP_STATUS_ERROR;
}


