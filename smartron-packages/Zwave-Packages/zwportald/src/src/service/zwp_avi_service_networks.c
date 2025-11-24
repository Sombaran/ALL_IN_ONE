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
 * @file    zwp_avi_service_networks.c
 *
 * @brief   Avro Interface service - Networks Portion
 * @details Handles networks portion of Avro Interface service parameters.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-14
 * - Initial version
 */

#include <stdio.h>

#include "zwp_log.h"
#if defined(ZWP_DEBUG_AVI_SERVICE) || defined(ZWP_DEBUG_AVI_SERVICE_NETWORKS)
#include "zwp_log_enable.h"
#else
#include "zwp_log_disable.h"
#endif

#include "zwp_avi_service.h"
#include "zwp_avi_service_nodes.h"
#include "zwp_descriptor_define.h"
#include "zwp_network.h"
#include "zwp_error.h"
#include "zwp_memory.h"
#include "zwp_io.h"
#include "zwu_time.h"

/** AVI Network level Service class */
struct _zwp_service_networks
{
	/* Level specific */
	char                       *net_id;                       /**< Network ID: Remote Access Code  */
	zwp_service_nodes_t        service_node;                  /**< Reference to 'Node' level service */

	zwu_time_t                 utime;                         /**< Last updated time of network */

	/* Common to all levels */
	zwp_service_common_t       service_common;                /**< Parameters common to all levels */
};

/**
 * @name Getters
 * @{
 */
char *zwp_avi_service_networks_get_net_id(zwp_service_networks_t service_net)
{
	return service_net->net_id;
}

char *zwp_avi_service_networks_get_message_name(zwp_service_networks_t service_net)
{
	return service_net->service_common->message_name;
}

zwp_avi_message_level_t zwp_avi_service_networks_get_message_level(zwp_service_networks_t service_net)
{
	return service_net->service_common->message_level;
}

zwp_avi_message_networks_execute_t zwp_avi_service_networks_get_avi_message_networks_execute_handler(zwp_service_networks_t service_net)
{
	return ((zwp_avi_message_networks_t *)service_net->service_common->avi_message)->execute_handler;
}

int zwp_avi_service_networks_get_avi_message_networks_valid_network_check(zwp_service_networks_t service_net)
{
	return ((zwp_avi_message_networks_t *)service_net->service_common->avi_message)->valid_network_check;
}

zwp_avi_message_result_free_t zwp_avi_service_networks_get_avi_message_result_free_handler(zwp_service_networks_t service_net)
{
	return ((zwp_avi_message_networks_t *)service_net->service_common->avi_message)->result_free_handler;
}

zwp_service_nodes_t zwp_avi_service_networks_get_service_node(zwp_service_networks_t service_net)
{
	return service_net->service_node;
}
/**
 * @}
 */

/**
 * @name Setters
 * @{
 */
zwp_status_t zwp_avi_service_networks_set_net_id(zwp_service_networks_t service_net, const char *net_id)
{
	ZWP_FREE(service_net->net_id);

	if ((service_net->net_id = ZWP_STRDUP(net_id)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_dup_string;
	}

	return ZWP_STATUS_OK;

l_err_dup_string:
	return ZWP_STATUS_ERROR;
}
/**
 * @}
 */

/**
 * @brief Set argument for a network service object
 * @param[in]     service_network   Network service object
 * @param[in]     argument_data     Argument data
 * @param[in]     free_handler      Argument free handler
 * @return        Nothing
 */
static void zwp_avi_service_networks_set_argument(zwp_service_networks_t service_network, void *argument_data, zwp_avi_message_argument_free_t free_handler)
{
	service_network->service_common->argument.data = argument_data;
	service_network->service_common->argument.free_handler = free_handler;
}

/**
 * @brief Set result for a network service object
 * @param[in]     service_network   Network service object
 * @param[in]     result_data       Result data
 * @param[in]     free_handler      Result free handler
 * @return        Nothing
 */
static void zwp_avi_service_networks_set_result(zwp_service_networks_t service_network, void *result_data, zwp_avi_message_result_free_t free_handler)
{
	service_network->service_common->result.data = result_data;
	service_network->service_common->result.free_handler = free_handler;
}

/*
 * @note 'net' can be NULL
 */
zwp_status_t zwp_avi_service_networks_execute_avi_messages(zwp_service_networks_t service_net, zwp_net_t net)
{
	zwp_avi_message_level_t message_level;
	zwp_avi_message_networks_execute_t execute_handler;
	zwp_avi_message_result_free_t result_free_handler;
	int valid_network_check;
	int network_exit;

	network_exit = 0;

	message_level = zwp_avi_service_networks_get_message_level(service_net);

	if (message_level == ZWP_AVI_MESSAGE_LEVEL_NETWORKS)
	{
		execute_handler = zwp_avi_service_networks_get_avi_message_networks_execute_handler(service_net);
		result_free_handler = zwp_avi_service_networks_get_avi_message_result_free_handler(service_net);
		valid_network_check = zwp_avi_service_networks_get_avi_message_networks_valid_network_check(service_net);

		if (!strcmp(zwp_avi_service_networks_get_message_name(service_net), "network_exit"))
		{
			network_exit = 1;
		}

		if (net && !network_exit)
		{
			if (valid_network_check == ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES)
			{
				if (zwp_net_desc_container_lock_and_test(net) != ZWP_STATUS_OK)
				{
					goto l_err_desc_container_net_lock;
				}
			}
			else
			{
				if (zwp_net_desc_container_lock(net) != ZWP_STATUS_OK)
				{
					goto l_err_desc_container_net_lock;
				}
			}
		}

		if (execute_handler(net,
				service_net->service_common->argument.data,
				&service_net->service_common->result.data) != ZWP_STATUS_OK)
		{
			goto l_err_execute;
		}

		// If "network_exit" call, 'net' is no longer valid
		if (network_exit)
		{
			net = NULL;
		}

		service_net->utime = zwp_net_get_network_utime(net);

		if (net && !network_exit)
		{
			zwp_net_desc_container_unlock(net);
		}

		zwp_avi_service_networks_set_result(service_net,
				service_net->service_common->result.data, result_free_handler);
	}
	else
	{
		if (!net
				|| zwp_avi_service_nodes_execute_avi_messages(zwp_avi_service_networks_get_service_node(service_net), net) != ZWP_STATUS_OK)
		{
			goto l_err_execute_avi_message_nodes;
		}
	}

	return ZWP_STATUS_OK;

l_err_execute_avi_message_nodes:
l_err_execute:
	if (message_level == ZWP_AVI_MESSAGE_LEVEL_NETWORKS)
	{
		if (net && !network_exit)
		{
			zwp_net_desc_container_unlock(net);
		}
	}
l_err_desc_container_net_lock:
	return ZWP_STATUS_ERROR;
}

/*
 * Input
 */
zwp_status_t zwp_avi_get_avro_value_of_field_networks(zwp_service_networks_t service_net, const char *type_name, avro_value_t *arg_value_p)
{
	zwp_status_t return_status;

	//ZWP_LOG(ZWP_LOG_DEBUG, "Avro type: '%s' (Network level)", type_name);

	return_status = ZWP_STATUS_OK;

	if (!strcmp(type_name, "Network"))
	{
		return_status = zw_avro_field_get_string(arg_value_p, "id", &service_net->net_id);
	}
	else
	{
		ZWP_LOG(ZWP_LOG_ERR, "Avro type '%s' is not supported at network level", type_name);
		return_status = ZWP_STATUS_ERROR;
	}

	if (return_status != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get argument of type index '%s'", type_name);
	}

	return return_status;
}

zwp_status_t zwp_avi_get_service_arg_networks(zwp_service_networks_t service_net, const char *type_name, avro_value_t *arg_value_p)
{
	avro_value_t command_value;
	avro_value_t network_value;
	avro_value_t parameter_value;
	zwp_avi_message_networks_t *avi_message;
	void *args;

	if (service_net->service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_NETWORKS)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Avro type: '%s' (Network level)", type_name);

		if ((avro_value_get_by_name(arg_value_p, "command", &command_value, NULL)) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'command' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_command_value;
		}

		if (avro_value_get_by_name(&command_value, "network", &network_value, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'network' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_network_value;
		}

		if (zwp_avi_get_avro_value_of_field_networks(service_net, "Network", &network_value) != ZWP_STATUS_OK)
		{
			goto l_err_field_networks;
		}

		avi_message = (zwp_avi_message_networks_t *)service_net->service_common->avi_message;

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

				zwp_avi_service_networks_set_argument(service_net, args, avi_message->argument_free_handler);
			}
		}
	}
	else
	{
		if (zwp_avi_get_service_arg_nodes(service_net->service_node, type_name, arg_value_p) != ZWP_STATUS_OK)
		{
			goto l_err_service_arg_nodes;
		}
	}

	return ZWP_STATUS_OK;

l_err_service_arg_nodes:
l_err_argument_handler:
l_err_field_networks:
l_err_get_network_value:
l_err_get_command_value:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_avi_alloc_avi_service_networks(zwp_service_common_t service_common, zwp_service_networks_t *service_net_p)
{
	zwp_service_networks_t service_net;

	if ((service_net = (zwp_service_networks_t)ZWP_MALLOC(sizeof(*service_net))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_service_net_alloc;
	}

	service_net->net_id = NULL;
	service_net->service_node = NULL;

	service_net->service_common = service_common;

	if ((service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_NODES)
			|| (service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_ENDPOINTS)
			|| (service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_INTERFACES))
	{
		if (zwp_avi_alloc_avi_service_nodes(service_common, service_net, &service_net->service_node) != ZWP_STATUS_OK)
		{
			goto l_err_alloc_avi_service_nodes;
		}
	}

	*service_net_p = service_net;

	return ZWP_STATUS_OK;

l_err_alloc_avi_service_nodes:
	ZWP_FREE(service_net);
l_err_service_net_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_avi_free_avi_service_networks(zwp_service_networks_t service_net)
{
	if ((service_net->service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_NODES)
			|| (service_net->service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_ENDPOINTS)
			|| (service_net->service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_INTERFACES))
	{
		zwp_avi_free_avi_service_nodes(service_net->service_node);
	}

	zw_avro_field_free_string(service_net->net_id);

	ZWP_FREE(service_net);
}

/*
 * Output
 */
zwp_status_t zwp_avi_set_avro_value_of_field_networks(zwp_service_networks_t service_net,
		const char *type_name, avro_value_t *arg_value_p)
{
	int avro_code;
	zwp_status_t return_status;

	//ZWP_LOG(ZWP_LOG_DEBUG, "Avro type: '%s' (Network level)", type_name);

	return_status = ZWP_STATUS_OK;
	avro_code = 0;

	if (!strcmp(type_name, "Network"))
	{
		return_status = zw_avro_field_set_string(arg_value_p, "id", service_net->net_id);
	}
	else
	{
		ZWP_LOG(ZWP_LOG_ERR, "Avro type '%s' is not supported at network level", type_name);
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

zwp_status_t zwp_avi_set_avro_value_of_response_with_result_networks(zwp_service_networks_t service_net,
		const char *type_name, avro_value_t *arg_value_p)
{
	avro_value_t command_value;
	avro_value_t network_value;
	avro_value_t parameter_value;
	zwp_avi_message_networks_t *avi_message;

	if (service_net->service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_NETWORKS)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Avro type: '%s' (Network level)", type_name);

		if ((avro_value_get_by_name(arg_value_p, "command", &command_value, NULL)) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'command' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_command_value;
		}

		if (avro_value_get_by_name(&command_value, "network", &network_value, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'network' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_network_value;
		}

		if (zwp_avi_set_avro_value_of_field_networks(service_net, "Network", &network_value) != ZWP_STATUS_OK)
		{
			goto l_err_field_networks;
		}

		if (zw_avro_field_set_long(&command_value, "utime", service_net->utime) != ZWP_STATUS_OK)
		{
			goto l_err_field_utime;
		}

		avi_message = (zwp_avi_message_networks_t *)service_net->service_common->avi_message;

		// Optional field
		if ((avro_value_get_by_name(arg_value_p, "parameter", &parameter_value, NULL)) == 0)
		{
			if (avi_message->result_handler)
			{
				if (avi_message->result_handler(service_net->service_common->result.data, &parameter_value) != ZWP_STATUS_OK)
				{
					ZWP_LOG(ZWP_LOG_ERR, "Failed to set results for type '%s'", type_name);
					goto l_err_result_handler;
				}
			}
		}
	}
	else
	{
		if (zwp_avi_set_avro_value_of_response_with_result_nodes(service_net->service_node, type_name, arg_value_p) != ZWP_STATUS_OK)
		{
			goto l_err_service_arg_nodes;
		}
	}

	return ZWP_STATUS_OK;

l_err_service_arg_nodes:
l_err_result_handler:
l_err_field_utime:
l_err_field_networks:
l_err_get_network_value:
l_err_get_command_value:
	return ZWP_STATUS_ERROR;
}

