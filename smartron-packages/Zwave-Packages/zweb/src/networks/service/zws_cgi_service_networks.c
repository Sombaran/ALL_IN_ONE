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
 * @file    zws_cgi_service_networks.c
 *
 * @brief   CGI service - Networks Portion
 * @details Handles networks portion of CGI service parameters.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-13
 * - Initial version
 */

#ifdef ZWS_WITH_FCGI
#include <fcgi_stdio.h>
#else
#include <stdio.h>
#endif

#include "zws_log.h"
#if defined(ZWS_DEBUG_CGI_SERVICE) || defined(ZWS_DEBUG_CGI_SERVICE_NETWORKS)
#include "zws_log_enable.h"
#else
#include "zws_log_disable.h"
#endif

#include "zws_cgi_service_networks.h"
#include "zws_cgi_service_nodes.h"
#include "zws_avi_message_networks.h"
#include "zws_error.h"
#include "zwu_number.h"
#include "zws_memory.h"
#include "zws_io.h"
#include "zwu_string_buffer.h"
#include "zws_ifttt_common.h"

/** CGI Network level Service class */
struct _zws_service_networks
{
	/* Level specific */
	char                          *net_id;                     /**< Network ID: Remote Access Code  */
	zws_cgi_command_networks_t    net_command;                 /**< Commands as in Web API v1 & v2. Eg: zwnet_xxx  */
	zws_service_nodes_t           service_node;                /**< Reference to 'Node' level service */

	int                           sub_command;                 /**< Sub command passed as 'cmd=' in POST data */

	/* Common to all levels */
	zws_service_common_t          service_common;              /**< Parameters common to all levels */
};

/**
 * @brief Set AVI message information
 * @param[in]     service_net       Network service object
 * @param[in]     cgi_uri           CGI URI
 * @param[in]     sub_command       Sub command
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_cgi_set_avi_message_networks_info(zws_service_networks_t service_net, char *cgi_uri, int sub_command)
{
	return zws_avi_get_avi_message_networks_from_cgi_uri(zws_avi_get_message_list_networks(service_net->service_common->avi),
			cgi_uri, sub_command,
			&service_net->service_common->message_name,
			&service_net->service_common->error_text, &service_net->service_common->error_text_index,
			(zws_avi_message_networks_t **)&service_net->service_common->avi_message);
}

zws_status_t zws_cgi_set_avro_value_of_field_networks(zws_service_networks_t service_net,
		const char *type_name, avro_value_t *arg_value_p)
{
	int avro_code;
	zws_status_t return_status;

	//ZWS_LOG(ZWS_LOG_DEBUG, "Avro type: '%s' (Network level)", type_name);

	return_status = ZWS_STATUS_OK;
	avro_code = 0;

	if (!strcmp(type_name, "Network"))
	{
		return_status = zw_avro_field_set_string(arg_value_p, "id", service_net->net_id);
	}
	else
	{
		ZWS_LOG(ZWS_LOG_ERR, "Avro type '%s' is not supported at network level", type_name);
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

zws_status_t zws_cgi_set_avro_value_of_request_arg_networks(zws_service_networks_t service_net,
		const char *type_name, avro_value_t *arg_value_p)
{
	avro_value_t commad_value;
	avro_value_t network_value;
	avro_value_t parameter_value;
	zws_avi_message_networks_t *avi_message;

	if (service_net->service_common->command_level == ZWS_CGI_COMMAND_LEVEL_NETWORKS)
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "Avro type: '%s' (Network level)", type_name);

		if ((avro_value_get_by_name(arg_value_p, "command", &commad_value, NULL)) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'command' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_command_value;
		}

		if (avro_value_get_by_name(&commad_value, "network", &network_value, NULL) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'network' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_network_value;
		}

		if (zws_cgi_set_avro_value_of_field_networks(service_net, "Network", &network_value) != ZWS_STATUS_OK)
		{
			goto l_err_field_networks;
		}

		avi_message = (zws_avi_message_networks_t *)service_net->service_common->avi_message;

		// Optional field
		if ((avro_value_get_by_name(arg_value_p, "parameter", &parameter_value, NULL)) == 0)
		{
			if (avi_message->argument_handler)
			{
				if (avi_message->argument_handler(service_net->service_common->args,
						service_net->service_common->args_count, &parameter_value) != ZWS_STATUS_OK)
				{
					ZWS_LOG(ZWS_LOG_ERR, "Failed to set arguments for type '%s'", type_name);
					goto l_err_argument_handler;
				}
			}
		}

		// If not equal, then it is a passive equivalent of this AVI message
		if (service_net->sub_command != avi_message->cgi_sub_command)
		{
			if (zw_avro_field_set_boolean(&commad_value, "passive", 1) != 0)
			{
				goto l_err_field_active;
			}
		}
	}
	else
	{
		if (zws_cgi_set_avro_value_of_request_arg_nodes(service_net->service_node, type_name, arg_value_p) != ZWS_STATUS_OK)
		{
			goto l_err_service_arg_nodes;
		}
	}

	return ZWS_STATUS_OK;

l_err_service_arg_nodes:
l_err_field_active:
l_err_argument_handler:
l_err_field_networks:
l_err_get_network_value:
l_err_get_command_value:
	return ZWS_STATUS_ERROR;
}

zws_status_t zws_cgi_get_cgi_service_networks_from_environment(char *relative_uri, char *net_id, zws_service_common_t service_common,
		zws_service_networks_t *service_net_p)
{
	zws_service_networks_t service_net;
	char *uri_next;
	char *key;
	char *value;

	if ((service_net = (zws_service_networks_t)ZWS_MALLOC(sizeof(*service_net))) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_service_net_alloc;
	}

	ZWS_LOG(ZWS_LOG_DEBUG, "net_id:  %s", net_id);

	if ((service_net->net_id = ZWS_STRDUP(net_id)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_dup_string;
	}

	service_net->net_command = ZWS_CGI_NETWORKS_COMMAND_NONE;
	service_net->service_node = NULL;

	service_net->service_common = service_common;

	service_net->sub_command = ZWS_CGI_NET_SUB_COMMAND_NONE;

	if (service_common->args_count)
	{
		key = service_common->args[0][0];
		value = service_common->args[0][1];

		if (!strcmp(key, "cmd") || !strcmp(key, "cmd_cgi") || !strcmp(key, "cmd_type"))
		{
			if (zwu_parse_int32_non_negative(value, &service_net->sub_command) != ZWU_STATUS_OK)
			{
				goto l_err_invalid_sub_command;
			}

			// Consume this arg
			service_common->args++;
			service_common->args_count--;
		}
	}

	// Get commands as in Web API v1 & v2
	if (relative_uri && relative_uri[0])
	{
		if (service_common->cgi_client == ZWS_CGI_CLIENT_ZWARE)
		{
			if ((uri_next = strstr(relative_uri, "/")))
			{
				*uri_next = '\0';
				uri_next++;
			}
			snprintf(service_net->service_common->command_name, ZWS_MAX_COMMAND_NAME_LENGTH, "%s", relative_uri);
			//service_net->service_common->command_name = relative_uri;
		}

		if (zws_cgi_set_avi_message_networks_info(service_net,
				service_net->service_common->command_name, service_net->sub_command) != ZWS_STATUS_OK)
		{
			if (zws_cgi_get_cgi_service_nodes_from_environment(relative_uri, service_net->sub_command, service_common,
					service_net, &service_net->service_node) != ZWS_STATUS_OK)
			{
				goto l_err_no_match;
			}
		}
		else
		{
			service_common->command_level = ZWS_CGI_COMMAND_LEVEL_NETWORKS;

			ZWS_LOG(ZWS_LOG_DEBUG, "CGI command: '%s' (Network level)", service_net->service_common->command_name);
		}
	}
	else
	{
		ZWS_LOG(ZWS_LOG_ERR, "Command missing in the URI");
		goto l_err_invalid_command;
	}

	*service_net_p = service_net;

	return ZWS_STATUS_OK;

l_err_invalid_command:
l_err_no_match:
l_err_invalid_sub_command:

	ZWS_FREE(service_net->net_id);
l_err_dup_string:
	ZWS_FREE(service_net);
l_err_service_net_alloc:
	return ZWS_STATUS_ERROR;
}

void zws_cgi_free_cgi_service_networks_from_environment(zws_service_networks_t service_net)
{
	if (service_net)
	{
		if (service_net->service_node)
		{
			zws_cgi_free_cgi_service_nodes_from_environment(service_net->service_node);
		}
		ZWS_FREE(service_net->net_id);
		ZWS_FREE(service_net);
	}
}

/*
 * Output
 */
/**
 * @brief Get fields of Avro value for a network service object
 * @param[in]     service_net       Network service object
 * @param[in]     type_name         Avro type name
 * @param[out]    arg_value_p       Avro value object for fields of the service
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_cgi_get_avro_value_of_field_networks(zws_service_networks_t service_net,
		const char *type_name, avro_value_t *arg_value_p)
{
	zws_status_t return_status;

	ZWS_LOG(ZWS_LOG_DEBUG, "Avro type: '%s' (Network level)", type_name);

	return_status = ZWS_STATUS_OK;

	if (!strcmp(type_name, "Network"))
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "net_id %s", service_net->net_id);

		ZWS_FREE(service_net->net_id);
		return_status = zw_avro_field_get_string(arg_value_p, "id", &service_net->net_id);
	}
	else
	{
		ZWS_LOG(ZWS_LOG_ERR, "Avro type '%s' is not supported at network level", type_name);
		return_status = ZWS_STATUS_ERROR;
	}

	if (return_status != ZWS_STATUS_OK)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get argument of type index '%s'", type_name);
	}

	return return_status;
}

zws_status_t zws_cgi_set_service_result_networks(zws_service_networks_t service_net, const char *type_name, avro_value_t *result_value_p)
{
	avro_value_t command_value;
	avro_value_t network_value;
	avro_value_t parameter_value;
	zws_avi_message_networks_t *avi_message;
	zwu_string_buffer_t result;
	int is_envelope, is_v2APIonly;
	int64_t utime = 0;
	zws_status_t res = ZWS_STATUS_OK;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);

	if (service_net->service_common->command_level == ZWS_CGI_COMMAND_LEVEL_NETWORKS)
	{
		if(type_name)
			ZWS_LOG(ZWS_LOG_DEBUG, "Avro type: '%s' (Network level)", type_name);

		avi_message = (zws_avi_message_networks_t *)service_net->service_common->avi_message;

		if (service_net->service_common->message_name == NULL)
		{
			//No avro message but may still have result handler
			if (avi_message->result_handler)
			{
				result = service_net->service_common->result;
				ZWS_LOG(ZWS_LOG_DEBUG, "command name: %s", service_net->service_common->command_name);
				
				res = avi_message->result_handler(ZWS_WEB_API_VERSION_2, NULL, result);

			}
		}
		else
		{

			if ((avro_value_get_by_name(result_value_p, "command", &command_value, NULL)) != 0)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'command' field within argument of type '%s': %s", type_name, avro_strerror());
				goto l_err_get_command_value;
			}

			if (avro_value_get_by_name(&command_value, "network", &network_value, NULL) != 0)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'network' field within argument of type '%s': %s", type_name, avro_strerror());
				goto l_err_get_network_value;
			}

			if (zws_cgi_get_avro_value_of_field_networks(service_net, "Network", &network_value) != ZWS_STATUS_OK)
			{
				goto l_err_field_networks;
			}

			if (zw_avro_field_get_long(&command_value, "utime", &utime) != ZWS_STATUS_OK)
			{
				goto l_err_field_utime;
			}

			ZWS_LOG(ZWS_LOG_DEBUG, "utime = %llu", utime);

			ZWS_LOG(ZWS_LOG_DEBUG, "[6] message_name: %s", service_net->service_common->message_name);
			// Optional field
			if ((avro_value_get_by_name(result_value_p, "parameter", &parameter_value, NULL)) == 0)
			{
				if (avi_message->result_handler)
				{
					result = service_net->service_common->result;

					is_envelope = 1;
					is_v2APIonly = 0;	//support both v1 and v2 API
					if (!strcmp(service_net->service_common->message_name, "network_info_get")
#ifndef LOCAL_CLIENT_ENABLE
						|| !strcmp(service_net->service_common->message_name, "network_gateway_status_get")
#endif
						|| !strcmp(service_net->service_common->message_name, "network_summary_get")
						|| !strcmp(service_net->service_common->message_name, "network_version_get")
						|| !strncmp(service_net->service_common->message_name, "scene", sizeof("scene") - 1)
						|| !strncmp(service_net->service_common->message_name, "security_scene", sizeof("security_scene") - 1)
						|| service_net->service_common->cgi_client == ZWS_CGI_CLIENT_IFTTT)
					{
						// Commands for which envelope is not required.
						// Typically an anomaly that needs fix in API definition. Until then...
						is_envelope = 0;
					}

					//Basically those returned with avro string APIs can only be called once.
					if (!strcmp(service_net->service_common->message_name, "network_whitelist_list_report")
						|| !strcmp(service_net->service_common->message_name, "network_whitelist_device_info")
						|| !strcmp(service_net->service_common->message_name, "network_node_prekit_dsk_get"))
					{
						is_v2APIonly = 1;
					}

					if (!is_v2APIonly)
					{
						if (avi_message->result_handler(ZWS_WEB_API_VERSION_1, &parameter_value, result) != ZWS_STATUS_OK)
						{
							ZWS_LOG(ZWS_LOG_ERR, "Failed to set results for type '%s'", type_name);
							goto l_err_result_handler_v1;
						}
					}

					if (is_envelope)
					{
						// Envelope
						//TODO: set utime
						if (zwu_string_buffer_append_format(result,
							"<zwnet utime=\"%llu\" id=\"%s\">",
							utime,
							service_net->net_id) != ZWS_STATUS_OK)
						{
							goto l_err_envelope_start;
						}
					}

					if (avi_message->result_handler(ZWS_WEB_API_VERSION_2, &parameter_value, result) != ZWS_STATUS_OK)
					{
						ZWS_LOG(ZWS_LOG_ERR, "Failed to set results for type '%s'", type_name);
						goto l_err_result_handler_v2;
					}

					if (is_envelope)
					{
						if (zwu_string_buffer_append_string(result, "</zwnet>") != ZWS_STATUS_OK)
						{
							goto l_err_envelope_end;
						}
					}
				}
			}
		}
	}
	else
	{
		if (zws_cgi_set_service_result_nodes(service_net->service_node, type_name, result_value_p) != ZWS_STATUS_OK)
		{
			goto l_err_service_result_nodes;
		}
	}

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);

	return res;

l_err_service_result_nodes:
l_err_envelope_end:
l_err_result_handler_v2:
l_err_envelope_start:
l_err_result_handler_v1:
l_err_field_utime:
l_err_field_networks:
l_err_get_network_value:
l_err_get_command_value:

	res = ZWS_STATUS_ERROR;
	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);
	return res;
}

zws_status_t zws_cgi_send_cgi_output_networks(zws_service_networks_t service_net)
{
	zwu_string_buffer_t result;

	ZWS_LOG(ZWS_LOG_DEBUG, "[3] message_name: %s", service_net->service_common->message_name);
	if (service_net->service_common->command_level == ZWS_CGI_COMMAND_LEVEL_NETWORKS)
	{
		result = service_net->service_common->result;

		ZWS_LOG(ZWS_LOG_DEBUG, "[4] message_name: %s", service_net->service_common->message_name);
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
		ZWS_LOG(ZWS_LOG_DEBUG, "[5] message_name: %s", service_net->service_common->message_name);
		if (zws_cgi_send_cgi_output_nodes(service_net->service_node) != ZWS_STATUS_OK)
		{
			goto l_err_output_nodes;
		}
	}

	return ZWS_STATUS_OK;

l_err_output_nodes:
l_err_write:
	return ZWS_STATUS_ERROR;
}
