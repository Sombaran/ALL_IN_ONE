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
 * @file    zwp_avi_service_interfaces.c
 *
 * @brief   Avro Interface service - Interfaces Portion
 * @details Handles nexts portion of Avro Interface service parameters.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-29
 * - Initial version
 */

#include <stdio.h>

#include "zwp_log.h"
#if defined(ZWP_DEBUG_AVI_SERVICE) || defined(ZWP_DEBUG_AVI_SERVICE_INTERFACES)
#include "zwp_log_enable.h"
#else
#include "zwp_log_disable.h"
#endif

#include "zwp_avi_service_interfaces.h"
#include "zwp_avi_service_endpoints.h"
#include "zwp_avi_service_nodes.h"
#include "zwp_avi_common_networks.h"
#include "zwp_descriptor_define.h"
#include "zwp_network.h"
#include "zwp_descriptor.h"
#include "zwp_error.h"
#include "zwp_memory.h"
#include "zwp_io.h"

/** AVI Interface level Service class */
struct _zwp_service_interfaces
{
	/* Level specific */
	int                           interface_id;           /**< Interface ID */
	zwp_service_nodes_t           service_node;           /**< Back reference to 'Node' level service */
	zwp_service_endpoints_t       service_endpoint;       /**< Back reference to 'Endpoint' level service */

	/* Common to all levels */
	zwp_service_common_t          service_common;         /**< Parameters common to all levels */

	int                           passive;                /**< Flag to indicate if the command is passive */
};

/**
 * @name Getters
 * @{
 */
int zwp_avi_service_interfaces_get_interface_id(zwp_service_interfaces_t service_interface)
{
	return service_interface->interface_id;
}

int zwp_avi_service_interfaces_get_endpoint_id(zwp_service_interfaces_t service_interface)
{
	return zwp_avi_service_endpoints_get_endpoint_id(service_interface->service_endpoint);
}

int zwp_avi_service_interfaces_get_node_id(zwp_service_interfaces_t service_interface)
{
	return zwp_avi_service_nodes_get_node_id(service_interface->service_node);
}

void *zwp_avi_service_interfaces_get_argument(zwp_service_interfaces_t service_interface)
{
	return service_interface->service_common->argument.data;
}

int zwp_avi_service_interfaces_is_passive(zwp_service_interfaces_t service_interface)
{
	return service_interface->passive;
}

zwp_avi_message_level_t zwp_avi_service_interfaces_get_message_level(zwp_service_interfaces_t service_interface)
{
	return service_interface->service_common->message_level;
}

zwp_avi_message_interfaces_report_receive_t zwp_avi_service_interfaces_get_avi_message_interfaces_report_receive_handler(zwp_service_interfaces_t service_interface)
{
	return ((zwp_avi_message_interfaces_t *)service_interface->service_common->avi_message)->report_receive_handler;
}

zwp_avi_message_interfaces_execute_active_t zwp_avi_service_interfaces_get_avi_message_interfaces_execute_active_handler(zwp_service_interfaces_t service_interface)
{
	return ((zwp_avi_message_interfaces_t *)service_interface->service_common->avi_message)->execute_active_handler;
}

zwp_avi_message_interfaces_execute_passive_t zwp_avi_service_interfaces_get_avi_message_interfaces_execute_passive_handler(zwp_service_interfaces_t service_interface)
{
	return ((zwp_avi_message_interfaces_t *)service_interface->service_common->avi_message)->execute_passive_handler;
}

zwp_avi_message_result_free_t zwp_avi_service_interfaces_get_avi_messsage_result_free_handler(zwp_service_interfaces_t service_interface)
{
	return ((zwp_avi_message_interfaces_t *)service_interface->service_common->avi_message)->result_free_handler;
}
/**
 * @}
 */

/**
 * @brief Set argument for a interface service object
 * @param[in]     service_interface Interface service object
 * @param[in]     argument_data     Argument data
 * @param[in]     free_handler      Argument free handler
 * @return        Nothing
 */
static void zwp_avi_service_interfaces_set_argument(zwp_service_interfaces_t service_interface, void *argument_data, zwp_avi_message_argument_free_t free_handler)
{
	service_interface->service_common->argument.data = argument_data;
	service_interface->service_common->argument.free_handler = free_handler;
}

/**
 * @brief Set result for a interface service object
 * @param[in]     service_interface Interface service object
 * @param[in]     result_data       Result data
 * @param[in]     free_handler      Result free handler
 * @return        Nothing
 */
static void zwp_avi_service_interfaces_set_result(zwp_service_interfaces_t service_interface, void *result_data, zwp_avi_message_result_free_t free_handler)
{
	service_interface->service_common->result.data = result_data;
	service_interface->service_common->result.free_handler = free_handler;
}

/**
 * @brief Wait for Report command
 * @param[in]     service_interface Service object
 * @param[in]     net               Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_service_interfaces_report_wait(zwp_service_interfaces_t service_interface, zwp_net_t net)
{
	zwp_avi_message_interfaces_report_receive_t report_receive_handler;
	zwp_thread_cond_t report_receive_condition;
	zwifd_p desc_interface;
	zwp_status_t return_status;
	zwp_interval_time_t report_wait_timeout;
	zwu_time_t iteration_report_wait_time;

	report_receive_handler = zwp_avi_service_interfaces_get_avi_message_interfaces_report_receive_handler(service_interface);

	if (report_receive_handler)
	{
		//report_wait_timeout in microseconds
		report_wait_timeout = zwp_net_get_report_wait_timeout(net) * 1000000L;

		iteration_report_wait_time = zwu_time_now();

		while (1)
		{
			// cond_wait releases the lock. So check the validity of net_desc
			if (zwp_net_desc_container_test(net) != ZWP_STATUS_OK)
			{
				goto l_err_desc_container_test;
			}

			if (zwp_desc_get_desc_interface(zwp_net_get_desc_net(net),
					zwp_avi_service_interfaces_get_node_id(service_interface),
					zwp_avi_service_interfaces_get_endpoint_id(service_interface),
					zwp_avi_service_interfaces_get_interface_id(service_interface),
					&desc_interface) != ZWP_STATUS_OK)
			{
				goto l_err_get_desc_interface;
			}

			report_receive_condition = NULL;

			if (report_receive_handler(desc_interface, service_interface->service_common->argument.data,
					&report_receive_condition) == ZWP_STATUS_OK)
			{
				break;
			}

			if (report_receive_condition ==  NULL)
			{
				goto l_err_report_receive_condition_null;
			}

			return_status = zwp_interface_report_receive_wait(desc_interface, report_receive_condition, report_wait_timeout);

			if (return_status != ZWP_STATUS_OK)
			{
				if (return_status == ZWP_STATUS_ETIME)
				{
					ZWP_LOG(ZWP_LOG_INFO, "Timed out on waiting for report after %" ZWP_TIME_T_FMT " microseconds", report_wait_timeout);
					break;
				}
				else
				{
					goto l_err_net_report_wait;
				}
			}
			//else iterate again to check the condition. This handles spurious signal associated with cond_var

			iteration_report_wait_time = (zwu_time_now() - iteration_report_wait_time) * 1000000L;

			report_wait_timeout = (report_wait_timeout > iteration_report_wait_time) ? (report_wait_timeout - iteration_report_wait_time) : 0;

			ZWP_LOG(ZWP_LOG_DEBUG, "Time elapsed in this report wait iteration: %llu", iteration_report_wait_time);
			ZWP_LOG(ZWP_LOG_DEBUG, "Remaining wait time for receiving report: %lld", report_wait_timeout);

			iteration_report_wait_time = zwu_time_now();
		}
	}

	return ZWP_STATUS_OK;

l_err_net_report_wait:
l_err_report_receive_condition_null:
l_err_get_desc_interface:
l_err_desc_container_test:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute active commands
 * @param[in]     service_interface Service object
 * @param[in]     net               Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_service_interfaces_execute_active_avi_messages(zwp_service_interfaces_t service_interface, zwp_net_t net)
{
	zwifd_p desc_interface;
	zwp_avi_message_interfaces_execute_active_t execute_active_handler;
	uint8_t transmit_status;
	int hcapi_code;

	execute_active_handler = zwp_avi_service_interfaces_get_avi_message_interfaces_execute_active_handler(service_interface);

	/*
	 * If request is passive or if active handler is not defined, return with success.
	 * Mostly applicable for XXX_REPORT commands.
	 */
	if (service_interface->passive || !execute_active_handler)
	{
		return ZWP_STATUS_OK;
	}

	if (zwp_net_desc_container_lock_and_test(net) != ZWP_STATUS_OK)
	{
		goto l_err_desc_container_net_lock;
	}

	if (zwp_desc_get_desc_interface(zwp_net_get_desc_net(net),
			zwp_avi_service_interfaces_get_node_id(service_interface),
			zwp_avi_service_interfaces_get_endpoint_id(service_interface),
			zwp_avi_service_interfaces_get_interface_id(service_interface),
			&desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_get_desc_interface_active;
	}

	zwp_interface_transmit_wait_init(desc_interface);

	hcapi_code = ZW_ERR_NONE;

	if (execute_active_handler(desc_interface,
			service_interface->service_common->argument.data, &hcapi_code) != ZWP_STATUS_OK)
	{
		goto l_err_execute_active;
	}

	if ((hcapi_code != ZW_ERR_NONE) && (hcapi_code != ZW_ERR_QUEUED) && (hcapi_code != ZW_ERR_CACHE_AVAIL))
	{
		ZWP_LOG(ZWP_LOG_ERR, "HCAPI call for '%s' failed with '%s'",
				service_interface->service_common->message_name, zwp_hcapi_error_code_to_string(hcapi_code));
		goto l_err_hcapi;
	}

	// Wait for transmit status only if not queued
	if (hcapi_code != ZW_ERR_QUEUED)
	{
		//Don't wait for transmit if return code is CACHE_AVAIL
		if (hcapi_code != ZW_ERR_CACHE_AVAIL)
		{
			if ((zwp_interface_transmit_wait(desc_interface, &transmit_status) != ZWP_STATUS_OK)
					|| (transmit_status != ZWNET_TX_OK))
			{
				goto l_err_trasnsmit_wait;
			}
		}

		if (zwp_avi_service_interfaces_report_wait(service_interface, net) != ZWP_STATUS_OK)
		{
			goto l_err_report_wait;
		}
	}

	zwp_net_desc_container_unlock(net);

	return ZWP_STATUS_OK;

l_err_report_wait:
l_err_trasnsmit_wait:
l_err_hcapi:
l_err_execute_active:
l_err_get_desc_interface_active:
		zwp_net_desc_container_unlock(net);
l_err_desc_container_net_lock:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute passive commands
 * @param[in]     service_interface Service object
 * @param[in]     net               Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_service_interfaces_execute_passive_avi_messages(zwp_service_interfaces_t service_interface, zwp_net_t net)
{
	zwifd_p desc_interface;
	zwp_avi_message_interfaces_execute_passive_t execute_passive_handler;

	execute_passive_handler = zwp_avi_service_interfaces_get_avi_message_interfaces_execute_passive_handler(service_interface);

	/*
	 * If passive handler is not defined, return with success.
	 * Mostly applicable for XXX_SET commands.
	 */
	if (!execute_passive_handler)
	{
		return ZWP_STATUS_OK;
	}

	if (zwp_net_desc_container_lock_and_test(net) != ZWP_STATUS_OK)
	{
		goto l_err_desc_container_net_lock;
	}

	if (zwp_desc_get_desc_interface(zwp_net_get_desc_net(net),
			zwp_avi_service_interfaces_get_node_id(service_interface),
			zwp_avi_service_interfaces_get_endpoint_id(service_interface),
			zwp_avi_service_interfaces_get_interface_id(service_interface),
			&desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_get_desc_interface_passive;
	}

	if (execute_passive_handler(desc_interface,
			service_interface->service_common->argument.data,
			&service_interface->service_common->result.data) != ZWP_STATUS_OK)
	{
		goto l_err_execute_passive;
	}

	zwp_net_desc_container_unlock(net);

	return ZWP_STATUS_OK;

l_err_execute_passive:
l_err_get_desc_interface_passive:
	zwp_net_desc_container_unlock(net);
l_err_desc_container_net_lock:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_avi_service_interfaces_execute_avi_messages(zwp_service_interfaces_t service_interface, zwp_net_t net)
{
	zwp_avi_message_level_t message_level;
	zwp_avi_message_result_free_t result_free_handler;

	message_level = zwp_avi_service_interfaces_get_message_level(service_interface);

	if (message_level == ZWP_AVI_MESSAGE_LEVEL_INTERFACES)
	{
		result_free_handler = zwp_avi_service_interfaces_get_avi_messsage_result_free_handler(service_interface);

		if (zwp_avi_service_interfaces_execute_active_avi_messages(service_interface, net) != ZWP_STATUS_OK)
		{
			goto l_err_execute_active_avi_messages;
		}

		/*
		 * Releasing and re-acquiring the lock here to allow an opportunity for any immediate callbacks
		 * from HCAPI to update interface level state. More relevant for XXX_SUPPORTED_GET commands.
		 */

		if (zwp_avi_service_interfaces_execute_passive_avi_messages(service_interface, net) != ZWP_STATUS_OK)
		{
			goto l_err_execute_passive_avi_messages;
		}

		zwp_avi_service_interfaces_set_result(service_interface,
				service_interface->service_common->result.data, result_free_handler);
	}
	else
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to match with all levels");
		goto l_err_execute_no_match;
	}

	return ZWP_STATUS_OK;

l_err_execute_no_match:
l_err_execute_passive_avi_messages:
l_err_execute_active_avi_messages:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_avi_get_avro_value_of_field_interfaces(zwp_service_interfaces_t service_interface, const char *type_name, avro_value_t *arg_value_p)
{
	int avro_code;
	zwp_status_t return_status;
	avro_value_t field_value;

	//ZWP_LOG(ZWP_LOG_DEBUG, "Avro type: '%s' (Interface level)", type_name);

	return_status = ZWP_STATUS_OK;
	avro_code = 0;

	if (!strcmp(type_name, "Interface"))
	{
		if ((avro_code = avro_value_get_by_name(arg_value_p, "endpoint", &field_value, NULL)) == 0)
		{
			if ((return_status = zwp_avi_get_avro_value_of_field_endpoints(service_interface->service_endpoint,
					"Endpoint", &field_value)) == ZWP_STATUS_OK)
			{
				return_status = zw_avro_field_get_int(arg_value_p, "id", &service_interface->interface_id);
			}
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_ERR, "Avro type '%s' is not supported at interface level", type_name);
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

zwp_status_t zwp_avi_get_service_arg_interfaces(zwp_service_interfaces_t service_interface, const char *type_name, avro_value_t *arg_value_p)
{
	avro_value_t command_value;
	avro_value_t interface_value;
	avro_value_t parameter_value;
	zwp_avi_message_interfaces_t *avi_message;
	void *args;

	if (service_interface->service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_INTERFACES)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Avro type: '%s' (Interface level)", type_name);

		if ((avro_value_get_by_name(arg_value_p, "command", &command_value, NULL)) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'command' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_command_value;
		}

		if (avro_value_get_by_name(&command_value, "interface", &interface_value, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'interface' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_interface_value;
		}

		if (zwp_avi_get_avro_value_of_field_interfaces(service_interface, "Interface", &interface_value) != ZWP_STATUS_OK)
		{
			goto l_err_field_interfaces;
		}

		// Optional command attribute
		zw_avro_field_get_boolean(&command_value, "passive", &service_interface->passive);

		avi_message = (zwp_avi_message_interfaces_t *)service_interface->service_common->avi_message;

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

				zwp_avi_service_interfaces_set_argument(service_interface, args, avi_message->argument_free_handler);
			}
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to match with supported command levels");
		goto l_err_invalid_command_level;
	}

	return ZWP_STATUS_OK;

l_err_invalid_command_level:
l_err_argument_handler:
l_err_field_interfaces:
l_err_get_interface_value:
l_err_get_command_value:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_avi_alloc_avi_service_interfaces(zwp_service_common_t service_common,
		zwp_service_nodes_t service_node, zwp_service_endpoints_t service_endpoint,
		zwp_service_interfaces_t *service_interface_p)
{
	zwp_service_interfaces_t service_interface;

	if ((service_interface = (zwp_service_interfaces_t)ZWP_MALLOC(sizeof(*service_interface))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_service_interface_alloc;
	}

	service_interface->interface_id = ZWP_AVI_INTERFACE_ID_NONE;

	service_interface->service_common = service_common;
	service_interface->service_node = service_node;
	service_interface->service_endpoint = service_endpoint;

	service_interface->passive = 0;

	*service_interface_p = service_interface;

	return ZWP_STATUS_OK;

	ZWP_FREE(service_interface);
l_err_service_interface_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_avi_free_avi_service_interfaces(zwp_service_interfaces_t service_interface)
{
	ZWP_FREE(service_interface);
}

/*
 * Output
 */
zwp_status_t zwp_avi_set_avro_value_of_field_interfaces(zwp_service_interfaces_t service_interface,
		const char *type_name, avro_value_t *arg_value_p)
{
	avro_value_t field_value;
	int avro_code;
	zwp_status_t return_status;

	//ZWP_LOG(ZWP_LOG_DEBUG, "Avro type: '%s' (Interface level)", type_name);

	return_status = ZWP_STATUS_OK;
	avro_code = 0;

	if (!strcmp(type_name, "Interface"))
	{
		if ((avro_code = avro_value_get_by_name(arg_value_p, "endpoint", &field_value, NULL)) == 0)
		{
			if ((return_status = zwp_avi_set_avro_value_of_field_endpoints(service_interface->service_endpoint,
					"Endpoint", &field_value)) == ZWP_STATUS_OK)
			{
				return_status = zw_avro_field_set_int(arg_value_p, "id", service_interface->interface_id);
			}
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_ERR, "Avro type '%s' is not supported at interface level", type_name);
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

zwp_status_t zwp_avi_set_avro_value_of_response_with_result_interfaces(zwp_service_interfaces_t service_interface,
		const char *type_name, avro_value_t *arg_value_p)
{
	avro_value_t command_value;
	avro_value_t interface_value;
	avro_value_t parameter_value;
	zwp_avi_message_interfaces_t *avi_message;

	if (service_interface->service_common->message_level == ZWP_AVI_MESSAGE_LEVEL_INTERFACES)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Avro type: '%s' (Interface level)", type_name);

		if ((avro_value_get_by_name(arg_value_p, "command", &command_value, NULL)) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'command' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_command_value;
		}

		if (avro_value_get_by_name(&command_value, "interface", &interface_value, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'interface' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_interface_value;
		}

		if (zwp_avi_set_avro_value_of_field_interfaces(service_interface, "Interface", &interface_value) != ZWP_STATUS_OK)
		{
			goto l_err_field_interfaces;
		}

		avi_message = (zwp_avi_message_interfaces_t *)service_interface->service_common->avi_message;

		// Optional field
		if ((avro_value_get_by_name(arg_value_p, "parameter", &parameter_value, NULL)) == 0)
		{
			if (avi_message->result_handler)
			{
				if (avi_message->result_handler(service_interface->service_common->result.data, &parameter_value) != ZWP_STATUS_OK)
				{
					ZWP_LOG(ZWP_LOG_ERR, "Failed to set results for type '%s'", type_name);
					goto l_err_result_handler;
				}
			}
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to match with supported command levels");
		goto l_err_invalid_command_level;
	}

	return ZWP_STATUS_OK;

l_err_invalid_command_level:
l_err_result_handler:
l_err_field_interfaces:
l_err_get_interface_value:
l_err_get_command_value:
	return ZWP_STATUS_ERROR;
}
