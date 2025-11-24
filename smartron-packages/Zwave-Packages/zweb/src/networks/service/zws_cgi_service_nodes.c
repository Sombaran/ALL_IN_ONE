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
 * @file    zws_cgi_service_nodes.c
 *
 * @brief   CGI service - Nodes Portion
 * @details Handles nodes portion of CGI service parameters.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-15
 * - Initial version
 */

#ifdef ZWS_WITH_FCGI
#include <fcgi_stdio.h>
#else
#include <stdio.h>
#endif

#include "zws_log.h"
#if defined(ZWS_DEBUG_CGI_SERVICE) || defined(ZWS_DEBUG_CGI_SERVICE_NODES)
#include "zws_log_enable.h"
#else
#include "zws_log_disable.h"
#endif

#include "zws_cgi_service_networks.h"
#include "zws_cgi_service_nodes.h"
#include "zws_cgi_service_endpoints.h"
#include "zws_avi_message_nodes.h"
#include "zws_error.h"
#include "zwu_number.h"
#include "zws_memory.h"
#include "zws_io.h"

/** CGI Node level Service class */
struct _zws_service_nodes
{
	/* Level specific */
	int                      node_id;                     /**< Node ID */
	zws_cgi_command_nodes_t  node_command;                /**< Commands as in Web API v1 & v2. Eg: zwnode_xxx  */
	zws_service_networks_t   service_net;                 /**< Back reference to 'Network' level service */
	zws_service_endpoints_t  service_endpoint;            /**< Reference to 'Endpoint' level service */

	int                      sub_command;                 /**< Sub command passed as 'cmd=' in POST data */

	/* Common to all levels */
	zws_service_common_t     service_common;              /**< Parameters common to all levels */
};

/**
 * @brief Set AVI message information
 * @param[in]     service_node      Node service object
 * @param[in]     cgi_uri           CGI URI
 * @param[in]     sub_command       Sub command
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_cgi_set_avi_message_nodes_info(zws_service_nodes_t service_node, char *cgi_uri, int sub_command)
{
	return zws_avi_get_avi_message_nodes_from_cgi_uri(zws_avi_get_message_list_nodes(service_node->service_common->avi),
			cgi_uri, sub_command,
			&service_node->service_common->message_name,
			&service_node->service_common->error_text, &service_node->service_common->error_text_index,
			(zws_avi_message_nodes_t **)&service_node->service_common->avi_message);
}

zws_status_t zws_cgi_set_avro_value_of_field_nodes(zws_service_nodes_t service_node,
		const char *type_name, avro_value_t *arg_value_p)
{
	avro_value_t field_value;
	int avro_code;
	zws_status_t return_status;

	//ZWS_LOG(ZWS_LOG_DEBUG, "Avro type: '%s' (Node level)", type_name);

	return_status = ZWS_STATUS_OK;
	avro_code = 0;

	if (!(strcmp(type_name, "Node")))
	{
		if ((avro_code = avro_value_get_by_name(arg_value_p, "network", &field_value, NULL)) == 0)
		{
			if ((return_status = zws_cgi_set_avro_value_of_field_networks(service_node->service_net,
					"Network", &field_value)) == ZWS_STATUS_OK)
			{
				return_status = zw_avro_field_set_int(arg_value_p, "id", service_node->node_id);
			}
		}
	}
	else
	{
		ZWS_LOG(ZWS_LOG_ERR, "Avro type '%s' is not supported at node level", type_name);
		return_status = ZWS_STATUS_ERROR;
	}

	if (return_status != ZWS_STATUS_OK)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to set argument of type '%s'", type_name);
	}
	else if (avro_code != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to set argument of type '%s': %s", type_name, avro_strerror());

		return_status = ZWS_STATUS_ERROR;
	}

	return return_status;
}


zws_status_t zws_cgi_set_avro_value_of_request_arg_nodes(zws_service_nodes_t service_node,
		const char *type_name, avro_value_t *arg_value_p)
{
	avro_value_t commad_value;
	avro_value_t node_value;
	avro_value_t parameter_value;
	zws_avi_message_nodes_t *avi_message;

	if (service_node->service_common->command_level == ZWS_CGI_COMMAND_LEVEL_NODES)
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "Avro type: '%s' (Node level)", type_name);

		if ((avro_value_get_by_name(arg_value_p, "command", &commad_value, NULL)) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'command' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_command_value;
		}

		if (avro_value_get_by_name(&commad_value, "node", &node_value, NULL) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'node' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_node_value;
		}

		if (zws_cgi_set_avro_value_of_field_nodes(service_node, "Node", &node_value) != ZWS_STATUS_OK)
		{
			goto l_err_field_nodes;
		}

		avi_message = (zws_avi_message_nodes_t *)service_node->service_common->avi_message;

		// Optional field
		if ((avro_value_get_by_name(arg_value_p, "parameter", &parameter_value, NULL)) == 0)
		{
			if (avi_message->argument_handler)
			{
				if (avi_message->argument_handler(service_node->service_common->args,
						service_node->service_common->args_count, &parameter_value) != ZWS_STATUS_OK)
				{
					ZWS_LOG(ZWS_LOG_ERR, "Failed to set arguments for type '%s'", type_name);
					goto l_err_argument_handler;
				}
			}
		}

		// If not equal, then it is a passive equivalent of this AVI message
		if (service_node->sub_command != avi_message->cgi_sub_command)
		{
			if (zw_avro_field_set_boolean(&commad_value, "passive", 1) != 0)
			{
				goto l_err_field_active;
			}
		}
	}
	else
	{
		if (zws_cgi_set_avro_value_of_request_arg_endpoints(service_node->service_endpoint, type_name, arg_value_p) != ZWS_STATUS_OK)
		{
			goto l_err_service_arg_endpoints;
		}
	}

	return ZWS_STATUS_OK;

l_err_service_arg_endpoints:
l_err_field_active:
l_err_argument_handler:
l_err_field_nodes:
l_err_get_node_value:
l_err_get_command_value:
	return ZWS_STATUS_ERROR;
}

zws_status_t zws_cgi_get_cgi_service_nodes_from_environment(char *relative_uri, int sub_command,
		zws_service_common_t service_common, zws_service_networks_t service_net, zws_service_nodes_t *service_node_p)
{
	zws_service_nodes_t service_node;
	char *key;
	char *value;
	int64_t combo_id;

	if ((service_node = (zws_service_nodes_t)ZWS_MALLOC(sizeof(*service_node))) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_service_node_alloc;
	}

	service_node->node_id = ZWS_CGI_NODE_ID_NONE;
	service_node->node_command = ZWS_CGI_NODES_COMMAND_NONE;
	service_node->service_endpoint = NULL;

	service_node->service_common = service_common;
	service_node->service_net = service_net;

	service_node->sub_command = sub_command;

	service_common->combo_id = ZWS_CGI_NET_COMBO_ID_NONE;

	// Get combo id used in Web API v1 & v2
	if (service_common->args_count)
	{
		key = service_common->args[0][0];
		value = service_common->args[0][1];

		if (!strcmp(key, "ifd") || !strcmp(key, "epd") || !strcmp(key, "noded"))
		{
			if (zwu_parse_int64_non_negative(value, &combo_id) != ZWU_STATUS_OK)
			{
				goto l_err_combo_id;
			}
			service_common->combo_id = (unsigned long)combo_id;

			// Consume this arg
			service_common->args++;
			service_common->args_count--;
		}
	}

	service_node->node_id = ZWS_CGI_GET_NODE_ID(service_common->combo_id);

	ZWS_LOG(ZWS_LOG_DEBUG, "Node id: '%d'", service_node->node_id);

	if (service_node->node_id == ZWS_CGI_NODE_ID_NONE)
	{
		goto l_err_invalid_node_id;
	}

	if (zws_cgi_set_avi_message_nodes_info(service_node,
			service_node->service_common->command_name, sub_command) != ZWS_STATUS_OK)
	{
		if (zws_cgi_get_cgi_service_endpoints_from_environment(relative_uri, service_node->sub_command,
				service_common, service_node, &service_node->service_endpoint) != ZWS_STATUS_OK)
		{
			goto l_err_endpoint_command_no_match;
		}
	}
	else
	{
		service_common->command_level = ZWS_CGI_COMMAND_LEVEL_NODES;

		ZWS_LOG(ZWS_LOG_DEBUG, "CGI command: '%s' (Node level)", relative_uri);
	}

	*service_node_p = service_node;

	return ZWS_STATUS_OK;

l_err_endpoint_command_no_match:
l_err_invalid_node_id:
l_err_combo_id:
	ZWS_FREE(service_node);
l_err_service_node_alloc:
	return ZWS_STATUS_ERROR;
}

void zws_cgi_free_cgi_service_nodes_from_environment(zws_service_nodes_t service_node)
{
	if (service_node->service_endpoint)
	{
		zws_cgi_free_cgi_service_endpoints_from_environment(service_node->service_endpoint);
	}
	ZWS_FREE(service_node);
}

/*
 * Output
 */
zws_status_t zws_cgi_set_service_result_nodes(zws_service_nodes_t service_node, const char *type_name, avro_value_t *result_value_p)
{
	avro_value_t parameter_value;
	zws_avi_message_nodes_t *avi_message;
	zwu_string_buffer_t result;

	if (service_node->service_common->command_level == ZWS_CGI_COMMAND_LEVEL_NODES)
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "Avro type: '%s' (Node level)", type_name);

		avi_message = (zws_avi_message_nodes_t *)service_node->service_common->avi_message;

		// Optional field
		if ((avro_value_get_by_name(result_value_p, "parameter", &parameter_value, NULL)) == 0)
		{
			if (avi_message->result_handler)
			{
				result = service_node->service_common->result;

				// Envelope
				if (zwu_string_buffer_append_format(result, "<node desc=\"%lu\" deprecated=\"1\">",
						service_node->service_common->combo_id) != ZWS_STATUS_OK)
				{
					goto l_err_envelope_start_v1;
				}

				if (avi_message->result_handler(ZWS_WEB_API_VERSION_1, service_node->service_common->combo_id, &parameter_value, result) != ZWS_STATUS_OK)
				{
					ZWS_LOG(ZWS_LOG_ERR, "Failed to set results for type '%s'", type_name);
					goto l_err_result_handler_v1;
				}

				if (zwu_string_buffer_append_string(result, "</node>") != ZWS_STATUS_OK)
				{
					goto l_err_envelope_end_v1;
				}

				// Envelope
				if (zwu_string_buffer_append_format(result, "<zwnode desc=\"%lu\">",
						service_node->service_common->combo_id) != ZWS_STATUS_OK)
				{
					goto l_err_envelope_start_v2;
				}

				if (avi_message->result_handler(ZWS_WEB_API_VERSION_2, service_node->service_common->combo_id, &parameter_value, result) != ZWS_STATUS_OK)
				{
					ZWS_LOG(ZWS_LOG_ERR, "Failed to set results for type '%s'", type_name);
					goto l_err_result_handler_v2;
				}

				if (zwu_string_buffer_append_string(result, "</zwnode>") != ZWS_STATUS_OK)
				{
					goto l_err_envelope_end_v2;
				}
			}
		}
	}
	else
	{
		if (zws_cgi_set_service_result_endpoints(service_node->service_endpoint, type_name, result_value_p) != ZWS_STATUS_OK)
		{
			goto l_err_service_result_endpoints;
		}
	}

	return ZWS_STATUS_OK;

l_err_service_result_endpoints:
l_err_envelope_end_v2:
l_err_result_handler_v2:
l_err_envelope_start_v2:
l_err_envelope_end_v1:
l_err_result_handler_v1:
l_err_envelope_start_v1:
	return ZWS_STATUS_ERROR;
}

zws_status_t zws_cgi_send_cgi_output_nodes(zws_service_nodes_t service_node)
{
	zwu_string_buffer_t result;

	if (service_node->service_common->command_level == ZWS_CGI_COMMAND_LEVEL_NODES)
	{
		result = service_node->service_common->result;

		if (result)
		{
			fwrite(zwu_string_buffer_get_string(result), zwu_string_buffer_length(result), 1, stdout);

			if (ferror(stdout))
			{
				ZWS_LOG(ZWS_LOG_ERR, "Failed to write result to standard output stream");
				goto l_err_write;
			}
			ZWS_LOG(ZWS_LOG_DEBUG, "Result Length: %d", zwu_string_buffer_length(result));
		}
		else
		{
			ZWS_LOG(ZWS_LOG_ERR, "service_common->result is NULL");
		}
	}
	else
	{
		if (zws_cgi_send_cgi_output_endpoints(service_node->service_endpoint) != ZWS_STATUS_OK)
		{
			goto l_err_output_endpoints;
		}
	}

	return ZWS_STATUS_OK;

l_err_output_endpoints:
l_err_write:
	return ZWS_STATUS_ERROR;
}

