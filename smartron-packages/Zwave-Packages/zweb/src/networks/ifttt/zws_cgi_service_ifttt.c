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
 * @file    zws_cgi_service_ifttt.c
 *
 * @brief   CGI Input from IFTTT
 * @details Handles CGI Input parameters from IFTTT.
 *
 * @author  Angela
 *
 * @version 1.0 - 2016-06-22
 * - Initial version
 */

#ifdef ZWS_WITH_FCGI
#include <fcgi_stdio.h>
#else
#include <stdio.h>
#endif

#include <stdlib.h>

#include "zws_log.h"
#if defined(ZWS_DEBUG_CGI_SERVICE) || defined(ZWS_DEBUG_CGI_SERVICE_ROOT)
#include "zws_log_enable.h"
#else
#include "zws_log_disable.h"
#endif

#include "zws_string.h"
#include "zws_memory.h"
#include "zwu_number.h"
#include "zws_error.h"
#include "zws_cgi_service_ifttt.h"


#ifndef LOCAL_CLIENT_ENABLE
	#include "zws_cgi_service_common.h"
	#include "zws_avi_ifttt_common.h"
	#include "zws_ifttt_types.h"
	#include "zws_exec_php_script.h"
	#include "zws_ifttt_response.h"
	#include "zws_avi_message_ifttt.h"
#endif


 /** CGI Network level Service class */
struct _zws_service_ifttt
{
	/* Level specific */
	ifttt_uri_t					stifttt_uri;					/**< IFTTT commands/uris  */
	ifttt_args_t				stifttt_args;					/**< IFTTT post data arguments  */

	/* Common to all levels */
	zws_service_common_t          service_common;              /**< Parameters common to all levels */
};

#ifndef LOCAL_CLIENT_ENABLE

/**
* @brief Set AVI message information
* @param[in]     service_net       Network service object
* @param[in]     cgi_uri           CGI URI
* @param[in]     sub_command       Sub command
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_cgi_set_avi_message_ifttt_info(pzws_service_ifttt_t pservice_ifttt, char *command_name)
{
	//zws_status_t res;
	
	return zws_avi_get_avi_message_ifttt_from_cgi_uri(zws_avi_get_message_list_ifttt(pservice_ifttt->service_common->avi),
		command_name, pservice_ifttt->stifttt_uri.iRequestType,
		pservice_ifttt->stifttt_uri.eRequest_IF_feature, 
		pservice_ifttt->stifttt_uri.iRequestSubType,
		pservice_ifttt->stifttt_uri.eRequest_IF_field,
		&pservice_ifttt->service_common->message_name,
		&pservice_ifttt->service_common->error_text, 
		(zws_avi_message_ifttt_t **)&pservice_ifttt->service_common->avi_message);
}

//This function must return either ZWS_STATUS_OK or ZWS_STATUS_IFTTT_ERROR_xxx
zws_status_t zws_cgi_get_ifttt_command(zws_service_t service)
{
	zws_service_common_t service_common;
	ifttt_uri_t		stifttt_uri = { 0 };
	char *relative_uri = NULL;
	char *root_command = NULL, *intf_slug_command = NULL, *sub_command = NULL, *field_slug_command = NULL;
	char *uri_next = NULL;
	int  iNofLayers = 0;
	int  bValidIFTTTCommand = 0;
	pzws_service_ifttt_t pservice_ifttt;
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_NOT_FOUND;

	if (!service) return res;

	service_common = zws_cgi_get_service_common(service);

	relative_uri = service_common->relative_uri;

	ZWS_LOG(ZWS_LOG_DEBUG, "relative_uri 1: '%s'", relative_uri);

	//Strip off zcgi
	if ((root_command = zws_url_string_strip_1layer(&relative_uri)))
	{
		//First 2 layers (zcgi/ifttt) need to verify, already verified in zws_cgi_get_client
		if ((root_command = zws_url_string_strip_1layer(&relative_uri)))
		{
			//get API version
			if ((root_command = zws_url_string_strip_1layer(&relative_uri)))
			{
				ZWS_LOG(ZWS_LOG_DEBUG, "relative_uri 4: '%s'", relative_uri);
				if (strcmp(root_command, IFTTT_API_VERSION1))
				{
					ZWS_LOG(ZWS_LOG_ERR, "IFTTT API version %s not supported.", root_command);

					zws_cgi_set_error_text(service, IFTTT_ERROR_MSG_API_VER_NOT_SUPPORTED);

					res = ZWS_STATUS_IFTTT_ERROR_PARAM;
					goto l_err_api_version_not_supported;
				}

				//Main root command (triggers, actions, test, status, user etc)
				iNofLayers = zws_count_nof_char_occurance(relative_uri, '/');

				//All IFTTT request is IFTTT type
				if (iNofLayers == 0)
				{
					if (!strcmp(relative_uri, "status"))
					{
						stifttt_uri.iRequestType = IFTTT_REQUEST_TYPE_GENERAL;
						bValidIFTTTCommand = 1;	

						service_common->command_level = ZWS_CGI_COMMAND_LEVEL_ROOT;
					}
				}
				else if (iNofLayers == 1)
				{
					if (!strcmp(relative_uri, "user/info"))
					{
						stifttt_uri.iRequestType = IFTTT_REQUEST_TYPE_GENERAL;
						bValidIFTTTCommand = 1;

						service_common->command_level = ZWS_CGI_COMMAND_LEVEL_NETWORKS;
					}
					else if (!strcmp(relative_uri, "test/setup"))
					{
						stifttt_uri.iRequestType = IFTTT_REQUEST_TYPE_GENERAL;
						bValidIFTTTCommand = 1;

						service_common->command_level = ZWS_CGI_COMMAND_LEVEL_NETWORKS;
					}
				}

				if ((bValidIFTTTCommand == 0) &&
					(root_command = zws_url_string_strip_1layer(&relative_uri)))
				{
					if (!strcmp(root_command, IFTTT_REQUEST_TRIGGER_WORD))
					{
						stifttt_uri.iRequestType = IFTTT_REQUEST_TYPE_TRIGGER;
					}
					else if (!strcmp(root_command, IFTTT_REQUEST_ACTION_WORD))
					{
						stifttt_uri.iRequestType = IFTTT_REQUEST_TYPE_ACTION;
					}
					else
					{
						ZWS_LOG(ZWS_LOG_DEBUG, "Unrecognized root command: '%s'", root_command);
						goto l_err_unknown_root_command;
					}

					if ((intf_slug_command = zws_url_string_strip_1layer(&relative_uri)))
					{
						ZWS_LOG(ZWS_LOG_DEBUG, "intf_slug_command: '%s'", intf_slug_command);

						if ((sub_command = zws_url_string_strip_1layer(&relative_uri)) &&
							!strcmp(sub_command, IFTTT_REQUEST_FIELDS_WORD))
						{
							ZWS_LOG(ZWS_LOG_DEBUG, "intermediate_command: '%s'", sub_command);

							if ((field_slug_command = zws_url_string_strip_1layer(&relative_uri)))
							{
								ZWS_LOG(ZWS_LOG_DEBUG, "field_slug_command: '%s'", field_slug_command);

								if ((sub_command = zws_url_string_strip_1layer(&relative_uri)) == NULL)
								{
									ZWS_LOG(ZWS_LOG_DEBUG, "relative_uri: '%s'", relative_uri);

									sub_command = relative_uri;

									if (zws_cgi_validate_ifttt_trigger_action_command(&stifttt_uri,
																					intf_slug_command,
																					field_slug_command,
																					sub_command) == ZWS_STATUS_OK)
									{
										bValidIFTTTCommand = 1;

										relative_uri = intf_slug_command;

										service_common->command_level = ZWS_CGI_COMMAND_LEVEL_NETWORKS;
									}
								}
							}
						}
					}
					else
					{
						ZWS_LOG(ZWS_LOG_DEBUG, "intf_slug_command: '%s'", relative_uri);

						if (zws_cgi_validate_ifttt_trigger_action_command(&stifttt_uri,
																		relative_uri,
																		"",
																		"") == ZWS_STATUS_OK)
						{
							bValidIFTTTCommand = 1;

							//Actual trigger/action will be at interface level
							service_common->command_level = ZWS_CGI_COMMAND_LEVEL_INTERFACES;
						}
					}
				}

			

				if (bValidIFTTTCommand == 1)
				{
					ZWS_LOG(ZWS_LOG_DEBUG, "Valid IFTTT CGI command: '%s'", relative_uri);

					if ((uri_next = strstr(relative_uri, "/")))
					{
						*uri_next = '_'; //relative_uri become user_info
					}

					CONSTRUCT_IFTTT_COMMAND_NAME(service_common->command_name, relative_uri)

					ZWS_LOG(ZWS_LOG_DEBUG, "command_name %s", service_common->command_name);

					zws_cgi_set_service_command_type(service, ZWS_CGI_COMMAND_TYPE_IFTTT);

					if ((res = zws_cgi_get_cgi_service_ifttt_from_environment(stifttt_uri, service_common,
						(pzws_service_ifttt_t *)&pservice_ifttt)) != ZWS_STATUS_OK)
					{
						ZWS_LOG(ZWS_LOG_ERR, "No match with supported commands");
						goto l_err_get_service_param;
					}

					zws_cgi_set_service_next(service, pservice_ifttt);

					res = ZWS_STATUS_OK;
				}
				else
				{
					ZWS_LOG(ZWS_LOG_DEBUG, "Unrecognized IFTTT CGI command: '%s'", relative_uri);
				}


			}
		}
	}
	return res;

//l_err_unrecognized_keyword:
l_err_get_service_param:
l_err_unknown_root_command:
l_err_api_version_not_supported:
	return res;
}

zws_status_t zws_cgi_get_cgi_service_ifttt_from_environment(ifttt_uri_t	stifttt_uri, zws_service_common_t service_common,
	pzws_service_ifttt_t *ppservice_ifttt)
{
	pzws_service_ifttt_t pservice_ifttt;
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_PARAM;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);

	if ((pservice_ifttt = (pzws_service_ifttt_t)ZWS_CALLOC(1, sizeof(*pservice_ifttt))) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_pservice_ifttt_alloc;
	}

	pservice_ifttt->stifttt_uri = stifttt_uri;

	if (service_common->http_body_length > 0)
	{
		//Process IFTTT request arguments
		if ((res = zws_cgi_get_ifttt_args(service_common, pservice_ifttt)) != ZWS_STATUS_OK)
		{
			goto l_err_get_args;
		}
	}

	pservice_ifttt->service_common = service_common;

	if ((res = zws_cgi_set_avi_message_ifttt_info(pservice_ifttt, service_common->command_name)) != ZWS_STATUS_OK)
	{
		ZWS_LOG(ZWS_LOG_ERR, "command %s is not found in avi messages.", service_common->command_name);
		goto l_err_invalid_command;
	}
	
	*ppservice_ifttt = pservice_ifttt;

	res = ZWS_STATUS_OK;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);

	return res;

l_err_invalid_command:
//l_err_no_match:
l_err_get_args:
	ZWS_FREE(pservice_ifttt);
l_err_pservice_ifttt_alloc:

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);
	return res;
}

void zws_cgi_free_cgi_service_ifttt_from_environment(pzws_service_ifttt_t pservice_ifttt)
{
	//Free pservice_ifttt arguments
	if (pservice_ifttt)
	{
		zws_cgi_free_ifttt_args(pservice_ifttt);

		//if (pservice_ifttt->service_node)
		//{
		//	zws_cgi_free_cgi_service_nodes_from_environment(service_net->service_node);
		//}
		//ZWS_FREE(service_net->net_id);
		ZWS_FREE(pservice_ifttt);
	}
}

zws_status_t zws_cgi_get_ifttt_args(zws_service_common_t service_common, pzws_service_ifttt_t pservice_ifttt)
{
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_PARAM;
	zwu_json_t jsFull_args = NULL;
	zwu_json_t jsTemp = NULL, jsDeviceName = NULL;

	jsFull_args = zwu_json_parse(service_common->http_body);

	if (!jsFull_args)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Fail to parse the IFTTT post arguments.");
		service_common->error_text = IFTTT_ERROR_STR_PARAM_JSON_INVALID;
		goto l_err_parse_args;
	}

	pservice_ifttt->stifttt_args.jsFull_args_obj = jsFull_args;

	if (jsFull_args->child == NULL)
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "Empty json request.");
		res = ZWS_STATUS_OK;
		goto l_ok_empty_json;
	}

	//Get "trigger_identity"
	jsTemp = zwu_json_get_object_item(jsFull_args, IFTTT_REQUEST_BODY_TRIGGER_ID);

	if (jsTemp && jsTemp->type == ZWU_JSON_TYPE_STRING)
	{
		pservice_ifttt->stifttt_args.pchTrigger_identity = jsTemp->valuestring;
		ZWS_LOG(ZWS_LOG_DEBUG, "Trigger_identity: %s", pservice_ifttt->stifttt_args.pchTrigger_identity);
	}

	//Get "triggerFields" or "actionFields"
	jsTemp = zwu_json_get_object_item(jsFull_args, IFTTT_REQUEST_BODY_TRIGGER_FIELDS);

	if (jsTemp && jsTemp->type == ZWU_JSON_TYPE_OBJECT)
	{
		pservice_ifttt->stifttt_args.jsFields_obj = jsTemp;
		ZWS_LOG(ZWS_LOG_DEBUG, "%s found.", IFTTT_REQUEST_BODY_TRIGGER_FIELDS);
	}
	else if ((jsTemp = zwu_json_get_object_item(jsFull_args, IFTTT_REQUEST_BODY_ACTION_FIELDS)) &&
			 (jsTemp->type == ZWU_JSON_TYPE_OBJECT))
	{
		pservice_ifttt->stifttt_args.jsFields_obj = jsTemp;
		ZWS_LOG(ZWS_LOG_DEBUG, "%s found.", IFTTT_REQUEST_BODY_ACTION_FIELDS);
	}
	else
	{
		ZWS_LOG(ZWS_LOG_ERR, "Fail to get both TriggerFields and ActionFields.");
		service_common->error_text = IFTTT_ERROR_STR_PARAM_NO_TRIGGER_ACTION_FIELD;
		goto l_err_get_args_field_obj;
	}

	//Get "device_name" (combo_id) for triggerFields or actionFields
	jsDeviceName = zwu_json_get_object_item(jsTemp, IFTTT_REQUEST_BODY_IFTTT_DEVICE_NAME);
	if (jsDeviceName && jsDeviceName->type == ZWU_JSON_TYPE_STRING)
	{
		int64_t combo_id;

		if (zwu_parse_int64_non_negative(jsDeviceName->valuestring, &combo_id) != ZWU_STATUS_OK)
		{
			service_common->error_text = IFTTT_ERROR_STR_PARAM_DEVICE_NAME_INVALID;
			ZWS_LOG(ZWS_LOG_ERR, "Fail to get parse %s to combo_id.", IFTTT_REQUEST_BODY_IFTTT_DEVICE_NAME);
			goto l_err_parse_device_name_str;
		}

		service_common->combo_id = (unsigned long)combo_id;
		ZWS_LOG(ZWS_LOG_DEBUG, "combo id %lu found.", service_common->combo_id);
	}
	else
	{
		service_common->error_text = IFTTT_ERROR_STR_PARAM_NO_DEVICE_NAME_IN_TRIGGER_ACTION_FIELD;
		ZWS_LOG(ZWS_LOG_ERR, "Fail to get device_name in trigger/action field.");
		goto l_err_get_device_name;
	}

	//Get "limit"
	pservice_ifttt->stifttt_args.iLimit = -1;
	jsTemp = zwu_json_get_object_item(jsFull_args, IFTTT_REQUEST_BODY_LIMIT);

	if (jsTemp && jsTemp->type == ZWU_JSON_TYPE_NUMBER)
	{
		pservice_ifttt->stifttt_args.iLimit = jsTemp->valueint;
		ZWS_LOG(ZWS_LOG_DEBUG, "%s found.", IFTTT_REQUEST_BODY_LIMIT);
	}

	//Get "user"
	jsTemp = zwu_json_get_object_item(jsFull_args, IFTTT_REQUEST_BODY_USER);

	if (jsTemp && jsTemp->type == ZWU_JSON_TYPE_OBJECT)
	{
		pservice_ifttt->stifttt_args.jsUser_obj = jsTemp;
		ZWS_LOG(ZWS_LOG_DEBUG, "%s found.", IFTTT_REQUEST_BODY_USER);
	}

	//Get "ifttt_source"
	jsTemp = zwu_json_get_object_item(jsFull_args, IFTTT_REQUEST_BODY_IFTTT_SRC);

	if (jsTemp && jsTemp->type == ZWU_JSON_TYPE_OBJECT)
	{
		pservice_ifttt->stifttt_args.jsIFTTT_src_obj = jsTemp;
		ZWS_LOG(ZWS_LOG_DEBUG, "%s found.", IFTTT_REQUEST_BODY_IFTTT_SRC);
	}

	res = ZWS_STATUS_OK;
	return res;

l_err_get_device_name:
l_err_parse_device_name_str:
l_err_get_args_field_obj:
	zws_cgi_free_ifttt_args(pservice_ifttt);
l_err_parse_args:
l_ok_empty_json:
	return res;
}

void zws_cgi_free_ifttt_args(pzws_service_ifttt_t pservice_ifttt)
{
	ifttt_args_t	*pstifttt_args = &(pservice_ifttt->stifttt_args);

	if (pstifttt_args->jsFull_args_obj != NULL)
	{
		zwu_json_delete(pstifttt_args->jsFull_args_obj);

		pstifttt_args->pchTrigger_identity = NULL;
		pstifttt_args->jsFields_obj = NULL;
		pstifttt_args->iLimit = -1;

		pstifttt_args->jsUser_obj = NULL;
		pstifttt_args->jsIFTTT_src_obj = NULL;
		pstifttt_args->jsFull_args_obj = NULL;
	}
}

zws_status_t zws_cgi_set_avro_value_of_request_arg_ifttt(pzws_service_ifttt_t pservice_ifttt,
	const char *type_name, avro_value_t *arg_value_p)
{
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_PARAM;
	avro_value_t commad_value;
	avro_value_t network_value;
	avro_value_t interface_value;
	avro_value_t endpoint_field_value;
	avro_value_t node_field_value;
	avro_value_t parameter_value;
	zws_avi_message_ifttt_t *avi_message;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);

	avi_message = pservice_ifttt->service_common->avi_message;

	if ((avro_value_get_by_name(arg_value_p, "command", &commad_value, NULL)) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'command' field within argument of type '%s': %s", type_name, avro_strerror());
		goto l_err_get_command_value;
	}

	if (pservice_ifttt->service_common->command_level == ZWS_CGI_COMMAND_LEVEL_NETWORKS)
	{
		if (avro_value_get_by_name(&commad_value, "network", &network_value, NULL) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'network' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_network_value;
		}

		//Set network id as NULL as other ZWare network commands
		if (zw_avro_field_set_string(&network_value, "id", "") != ZWS_STATUS_OK)
		{
			goto l_err_set_network_id;
		}
	}
	else if (pservice_ifttt->service_common->command_level == ZWS_CGI_COMMAND_LEVEL_INTERFACES)
	{
		int inode_id = ZWS_CGI_NODE_ID_NONE;
		int iendppoint_id = ZWS_CGI_ENDPOINT_ID_NONE;
		int iinterface_id = ZWS_CGI_INTERFACE_ID_NONE;

		inode_id = ZWS_CGI_GET_NODE_ID(pservice_ifttt->service_common->combo_id);
		iendppoint_id = ZWS_CGI_GET_ENDPOINT_ID(pservice_ifttt->service_common->combo_id);
		iinterface_id = ZWS_CGI_GET_INTERFACE_ID(pservice_ifttt->service_common->combo_id);

		if ((avro_value_get_by_name(&commad_value, "interface", &interface_value, NULL) != 0) ||
			(avro_value_get_by_name(&interface_value, "endpoint", &endpoint_field_value, NULL) != 0) ||
			(avro_value_get_by_name(&endpoint_field_value, "node", &node_field_value, NULL) != 0) ||
			(avro_value_get_by_name(&node_field_value, "network", &network_value, NULL) != 0))
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'interface/endpoint/node/network' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_interface_value;
		}

		//Set network id as NULL as other ZWare network commands
		if ((zw_avro_field_set_string(&network_value, "id", "") != ZWS_STATUS_OK) ||
			(zw_avro_field_set_int(&node_field_value, "id", inode_id) != ZWS_STATUS_OK) ||
			(zw_avro_field_set_int(&endpoint_field_value, "id", iendppoint_id) != ZWS_STATUS_OK) ||
			(zw_avro_field_set_int(&interface_value, "id", iinterface_id) != ZWS_STATUS_OK))

		{
			goto l_err_set_interface_id;
		}

		//if (avro_value_get_by_name(&commad_value, "network", &network_value, NULL) != 0)
		//{
		//	ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'network' field within argument of type '%s': %s", type_name, avro_strerror());
		//	goto l_err_get_network_value;
		//}

		//Get the node id/endpoint id/interface id from the param
		//if ((zw_avro_field_set_int(&commad_value, "nodeid", inode_id) != ZWS_STATUS_OK) ||
		//	(zw_avro_field_set_int(&commad_value, "endpointid", iendppoint_id) != ZWS_STATUS_OK) ||
		//	(zw_avro_field_set_int(&commad_value, "interfaceid", iinterface_id) != ZWS_STATUS_OK))
		//{
		//	ZWS_LOG(ZWS_LOG_ERR, "Failed to set commands for combo_id '%ul'", pservice_ifttt->service_common->combo_id);

		//	goto l_err_set_interface_id;
		//}

		////For trigger API, there are some extra param to set
		//if ((pservice_ifttt->stifttt_uri.iRequestType == IFTTT_REQUEST_TYPE_TRIGGER) &&
		//	(pservice_ifttt->stifttt_args.pchTrigger_identity != NULL))
		//{
		//	if ((zw_avro_field_set_string(&commad_value, "triggerid", pservice_ifttt->stifttt_args.pchTrigger_identity) != ZWS_STATUS_OK) ||
		//		(zw_avro_field_set_int(&commad_value, "limit", pservice_ifttt->stifttt_args.iLimit) != ZWS_STATUS_OK))
		//	{
		//		ZWS_LOG(ZWS_LOG_ERR, "Failed to set 'triggerid' or 'limit' field within argument of type '%s': %s", type_name, avro_strerror());
		//		goto l_err_set_trigger_id;
		//	}

		//}
	}

	// Optional field
	if ((avro_value_get_by_name(arg_value_p, "parameter", &parameter_value, NULL)) == 0)
	{

		if (avi_message->argument_handler)
		{
			if ((res = zws_avi_network_ifttt_trigger_get_data_argument(&parameter_value,
				pservice_ifttt->stifttt_uri,
				pservice_ifttt->stifttt_args)) != ZWS_STATUS_OK)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Failed to set ifttt trigger data for type '%s'", type_name);
				goto l_err_trigger_data;
			}

			if ((res = avi_message->argument_handler(pservice_ifttt->stifttt_uri,
				pservice_ifttt->stifttt_args, &parameter_value)) != ZWS_STATUS_OK)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Failed to set arguments for type '%s'", type_name);
				goto l_err_argument_handler;
			}
		}
	}
	else
		res = ZWS_STATUS_OK;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);

l_err_argument_handler:
l_err_trigger_data:
//l_err_set_trigger_id:
l_err_set_interface_id:
l_err_get_interface_value:
l_err_set_network_id:
l_err_get_network_value:
l_err_get_command_value:
	return res;
}


zws_status_t zws_cgi_set_service_result_ifttt(pzws_service_ifttt_t pservice_ifttt, const char *type_name, avro_value_t *result_value_p)
{
	avro_value_t command_value;
	//avro_value_t network_value;
	avro_value_t parameter_value;
	zws_avi_message_ifttt_t *avi_message;
	zwu_string_buffer_t result;
	//int64_t utime;
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_SERVER;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);

	if (type_name)
		ZWS_LOG(ZWS_LOG_DEBUG, "Avro type: '%s' (IFTTT)", type_name);

	if (result_value_p != NULL)
	//if ((pservice_ifttt->service_common->command_level == ZWS_CGI_COMMAND_LEVEL_NETWORKS) && 
	//	(result_value_p != NULL))
	{
		if ((avro_value_get_by_name(result_value_p, "command", &command_value, NULL)) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'command' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_command_value;
		}

		//if (avro_value_get_by_name(&command_value, "network", &network_value, NULL) != 0)
		//{
		//	ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'network' field within argument of type '%s': %s", type_name, avro_strerror());
		//	goto l_err_get_network_value;
		//}

		//TODO: reaccess whether this should be enabled and port over
		//if (zws_cgi_get_avro_value_of_field_networks(pservice_ifttt, "Network", &network_value) != ZWS_STATUS_OK)
		//{
		//	goto l_err_field_networks;
		//}

		//if (zw_avro_field_get_long(&command_value, "utime", &utime) != ZWS_STATUS_OK)
		//{
		//	goto l_err_field_utime;
		//}

		ZWS_LOG(ZWS_LOG_DEBUG, "[6] message_name: %s", pservice_ifttt->service_common->message_name);

		// Optional field
		avro_value_get_by_name(result_value_p, "parameter", &parameter_value, NULL);
	}
	
	avi_message = (zws_avi_message_ifttt_t *)pservice_ifttt->service_common->avi_message;

	if (avi_message->result_handler)
	{
		zwu_json_t jsRoot = NULL;
		zwu_json_t jsData = NULL;

		result = pservice_ifttt->service_common->result;

		ZWS_LOG(ZWS_LOG_DEBUG, "command name: %s", pservice_ifttt->service_common->command_name);

		if (avi_message->cgi_response_type == IFTTT_RESPONSE_TYPE_NONE)
		{
			jsRoot = NULL;
			jsData = NULL;

			res = ZWS_STATUS_OK;
		}
		else if (avi_message->cgi_response_type == IFTTT_RESPONSE_TYPE_DATA_OBJECT)
		{
			res = zws_ifttt_response_create_data_object(&jsRoot, &jsData);
		}
		else if (avi_message->cgi_response_type == IFTTT_RESPONSE_TYPE_DATA_ARRAY)
		{
			res = zws_ifttt_response_create_data_array(&jsRoot, &jsData);
		}

		if (res == ZWS_STATUS_OK)
		{
			//Pass to result handler
			if ((!strcmp(pservice_ifttt->service_common->command_name, "ifttt_user_info")) ||
				(!strcmp(pservice_ifttt->service_common->command_name, "ifttt_test_setup")))
			{
				res = avi_message->result_handler(&parameter_value, (void *)pservice_ifttt->service_common->user, jsData);
			}
			else if (pservice_ifttt->service_common->command_level == ZWS_CGI_COMMAND_LEVEL_INTERFACES)
			{
				res = avi_message->result_handler(&parameter_value, (void *)&pservice_ifttt->service_common->combo_id, jsData);
			}
			else
			{
				res = avi_message->result_handler(&parameter_value, NULL, jsData);
			}

			//Convert json to result string
			//As long as the json is create, we should send response back, even if it is empty "Data" array/object
			//If there is a need to distinguish between send response and not send response, with empty/non-empty
			//"Data" array/object, we will then modify result handler declartion to add more parameter
			if (jsRoot != NULL)
			{
				zws_ifttt_response_to_stringbuffer(jsRoot, result);

				zws_ifttt_response_destroy(jsRoot);
			}
		}
		else
		{
			ZWS_LOG(ZWS_LOG_DEBUG, "Create json response fail.");
			goto l_err_create_json_response;
		}
	}	

//l_err_field_utime:
//l_err_field_networks:
l_err_create_json_response:
l_err_get_command_value:

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);
	return res;
}

zws_status_t zws_cgi_ifttt_check_user_existence(char *user_name, int *pbExist)
{
	zws_status_t return_status = ZWS_STATUS_ERROR;
	char *pchExist = NULL;
	int iRespLength;
	int iExitCode;

	if (user_name && pbExist)
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "Check for existence of user id: '%s'", user_name);

		if (zws_exec_php_request(CHECK_USER_EXISTENCE_SCRIPT_FILE, 2, PHP_SCRIPT_INPUT_USER_ID, user_name, NULL, &pchExist,
			&iRespLength, &iExitCode) == ZWS_STATUS_OK)
		{
			if (iExitCode == PHP_SCRIPT_SUCCESS && pchExist != NULL)
			{
				ZWS_LOG(ZWS_LOG_DEBUG, "Check for existence response: %s", pchExist);
				//if result is found
				if (!strcmp(pchExist, "0"))
				{
					*pbExist = 1;
					return_status = ZWS_STATUS_OK;
				}		
			}
			else if (iExitCode != PHP_SCRIPT_SUCCESS)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Check for existence of user id fail. Exit code: %d", iExitCode);
				return_status = ZWS_STATUS_EACCES;
			}
		}

		if (pchExist != NULL)
		{
			ZWS_FREE(pchExist);
		}
	}

	return return_status;
}

zws_status_t zws_cgi_ifttt_oauth2_user_name_get(char *sAccessToken, char **ppifttt_user)
{
	zws_status_t return_status = ZWS_STATUS_ERROR;
	int iRespLength;
	int iExitCode;

	if (sAccessToken && sAccessToken[0])
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "Find user ID with Access Toekn: %s", sAccessToken);

		if (zws_exec_php_request(GET_USER_INFO_SCRIPT_FILE, 3, PHP_SCRIPT_INPUT_ACCESS_TOKEN, sAccessToken, PHP_SCRIPT_INPUT_USER_ID, ppifttt_user,
			&iRespLength, &iExitCode) == ZWS_STATUS_OK)
		{
			if (iExitCode == PHP_SCRIPT_SUCCESS && (*ppifttt_user) != NULL)
			{
				ZWS_LOG(ZWS_LOG_DEBUG, "Find user ID response: %s", *ppifttt_user);
				//if result is found
				return_status = ZWS_STATUS_OK;
			}
			else if (iExitCode != PHP_SCRIPT_SUCCESS)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Find user ID with Access token fail. Exit code: %d", iExitCode);
				return_status = ZWS_STATUS_EACCES;
			}
		}
	}

	return return_status;
}


zws_status_t zws_cgi_validate_ifttt_trigger_action_command(ifttt_uri_t	*pstifttt_uri, 
														   char *sintf_slug, 
														   char *sfield_command,
														   char *ssub_command)
{
	zws_status_t res = ZWS_STATUS_ERROR;
	int i = 0;

	if (!strcmp(ssub_command, ""))
	{
		pstifttt_uri->iRequestSubType = IFTTT_REQUEST_SUBTYPE_NONE;
	}
	else if (!strcmp(ssub_command, IFTTT_REQUEST_OPTIONS_WORD))
	{
		pstifttt_uri->iRequestSubType = IFTTT_REQUEST_SUBTYPE_OPTION;
	}
	else if (!strcmp(ssub_command, IFTTT_REQUEST_VALIDATE_WORD) && pstifttt_uri->iRequestType == IFTTT_REQUEST_TYPE_TRIGGER)
	{
		//Only Trigger has /validate command
		pstifttt_uri->iRequestSubType = IFTTT_REQUEST_SUBTYPE_VALIDATE;
	}
	else
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "Invalid subcommand: '%s', request type: %s", 
						ssub_command, 
						(pstifttt_uri->iRequestType == IFTTT_REQUEST_TYPE_TRIGGER) ? IFTTT_REQUEST_TRIGGER_WORD : IFTTT_REQUEST_ACTION_WORD);
		goto l_err_invalid_subcommand;
	}

	if (pstifttt_uri->iRequestType == IFTTT_REQUEST_TYPE_TRIGGER)
	{
		for (i = 0; i < sizeof(arIFTTT_Triggers) / sizeof(ifttt_uri_detail_t); i++)
		{
			if (!strcmp(arIFTTT_Triggers[i].sIFTTTInterfaceSlug, sintf_slug) &&
				!strcmp(arIFTTT_Triggers[i].sIFTTTFieldSlug, sfield_command))
			{
				ZWS_LOG(ZWS_LOG_DEBUG, "Matching Trigger URI found!");

				pstifttt_uri->eRequest_IF_feature = arIFTTT_Triggers[i].eIFTTT_IF_feature;
				pstifttt_uri->pchRequestIF = sintf_slug;
				pstifttt_uri->eRequest_IF_field = arIFTTT_Triggers[i].eIFTTTField;
				pstifttt_uri->pchRequestIFField = sfield_command;

				res = ZWS_STATUS_OK;

				break;
			}
		}
	}
	else if (pstifttt_uri->iRequestType == IFTTT_REQUEST_TYPE_ACTION)
	{
		for (i = 0; i < sizeof(arIFTTT_Actions) / sizeof(ifttt_uri_detail_t); i++)
		{
			if (!strcmp(arIFTTT_Actions[i].sIFTTTInterfaceSlug, sintf_slug) &&
				!strcmp(arIFTTT_Actions[i].sIFTTTFieldSlug, sfield_command))
			{
				ZWS_LOG(ZWS_LOG_DEBUG, "Matching Action URI found!");

				pstifttt_uri->eRequest_IF_feature = arIFTTT_Actions[i].eIFTTT_IF_feature;
				pstifttt_uri->pchRequestIF = sintf_slug;
				pstifttt_uri->eRequest_IF_field = arIFTTT_Actions[i].eIFTTTField;
				pstifttt_uri->pchRequestIFField = sfield_command;

				res = ZWS_STATUS_OK;

				break;
			}
		}
	}
	else
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "Invalid request type: %d", pstifttt_uri->iRequestType);
	}

l_err_invalid_subcommand:
	return res;
}

zws_status_t zws_cgi_ifttt_send_cgi_output(zws_service_t service)
{
	zws_service_common_t   service_common;
	zwu_string_buffer_t result;

	fprintf(stdout, "Content-Type: application/json; charset=utf-8\r\n\r\n");

	if (service)
	{
		service_common = zws_cgi_get_service_common(service);

		if (service_common && service_common->result &&
			(zwu_string_buffer_length(service_common->result) > 0))
		{
			result = service_common->result;

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
	}

	return ZWS_STATUS_OK;
l_err_write:
	return ZWS_STATUS_ERROR;
}

zws_status_t zws_cgi_ifttt_send_cgi_output_error(zws_status_t error_status, zws_service_t service)
{
	//zws_service_common_t service_common;
	char *error_text;

	if (error_status == ZWS_STATUS_IFTTT_ERROR_PARAM)
	{
		fprintf(stdout, "Status: %d Bad Request\r\n", ZWS_STATUS_IFTTT_ERROR_PARAM);
	}
	else if (error_status == ZWS_STATUS_IFTTT_ERROR_OA2_AT_INVALID)
	{
		fprintf(stdout, "Status: %d Unauthorized\r\n", ZWS_STATUS_IFTTT_ERROR_OA2_AT_INVALID);
	}
	else if (error_status == ZWS_STATUS_IFTTT_ERROR_NOT_FOUND)
	{
		fprintf(stdout, "Status: %d Not Found\r\n", ZWS_STATUS_IFTTT_ERROR_NOT_FOUND);
	}
	else if (error_status == ZWS_STATUS_IFTTT_ERROR_SERVER)
	{
		fprintf(stdout, "Status: %d Internal Server Error\r\n", ZWS_STATUS_IFTTT_ERROR_SERVER);
	}
	else if (error_status == ZWS_STATUS_IFTTT_ERROR_SERVICE_UNAVAILABLE)
	{
		fprintf(stdout, "Status: %d Service Unavailable\r\n", ZWS_STATUS_IFTTT_ERROR_SERVICE_UNAVAILABLE);
	}

	fprintf(stdout, "Content-Type: application/json; charset=utf-8\r\n\r\n");

	if (service)
	{
		error_text = zws_cgi_get_error_text(service);

		if (error_text && error_text[0] != '\0')
		{
			zwu_json_t jsRoot = NULL;
			zwu_json_t jsEmptyObj = NULL;
			char *strResult;

			if (zws_ifttt_response_create_error_message_obj(&jsRoot, &jsEmptyObj) == ZWS_STATUS_OK)
			{
				zwu_json_add_string_to_object(jsEmptyObj, "message", error_text);

				if ((strResult = ZWU_JSON_PRINT(jsRoot)))
				{
					fprintf(stdout, "%s", strResult);

					ZWS_FREE(strResult);
				}

				zws_ifttt_response_destroy(jsRoot);
			}
		}
	}

	return ZWS_STATUS_OK;
}

#else

zws_status_t zws_cgi_get_ifttt_command(zws_service_t service) 
{
	return ZWS_STATUS_IFTTT_ERROR_SERVICE_UNAVAILABLE;
}

zws_status_t zws_cgi_get_ifttt_args(zws_service_common_t service_common, pzws_service_ifttt_t pservice_ifttt)
{
	return ZWS_STATUS_IFTTT_ERROR_SERVICE_UNAVAILABLE;
}

void zws_cgi_free_ifttt_args(pzws_service_ifttt_t pservice_ifttt)
{
	return;
}

zws_status_t zws_cgi_ifttt_check_user_existence(char *user_name, int *pbExist)
{
	return ZWS_STATUS_IFTTT_ERROR_SERVICE_UNAVAILABLE;
}

zws_status_t zws_cgi_ifttt_oauth2_user_name_get(char *sAccessToken, char **ppifttt_user)
{
	return ZWS_STATUS_IFTTT_ERROR_SERVICE_UNAVAILABLE;
}

zws_status_t zws_cgi_ifttt_send_cgi_output(zws_service_t service)
{
	return ZWS_STATUS_IFTTT_ERROR_SERVICE_UNAVAILABLE;
}

zws_status_t zws_cgi_ifttt_send_cgi_output_error(zws_status_t error_status, zws_service_t service)
{
	return ZWS_STATUS_IFTTT_ERROR_SERVICE_UNAVAILABLE;
}

zws_status_t zws_cgi_get_cgi_service_ifttt_from_environment(ifttt_uri_t	stifttt_uri,
	zws_service_common_t service_common,
	pzws_service_ifttt_t *ppservice_ifttt)
{
	return ZWS_STATUS_IFTTT_ERROR_SERVICE_UNAVAILABLE;
}

void zws_cgi_free_cgi_service_ifttt_from_environment(pzws_service_ifttt_t pservice_ifttt)
{
	return;
}

zws_status_t zws_cgi_set_avro_value_of_request_arg_ifttt(pzws_service_ifttt_t pservice_ifttt,
	const char *type_name, avro_value_t *arg_value_p)
{
	return ZWS_STATUS_IFTTT_ERROR_SERVICE_UNAVAILABLE;
}

zws_status_t zws_cgi_set_service_result_ifttt(pzws_service_ifttt_t pservice_ifttt,
	const char *type_name,
	avro_value_t *result_value_p)
{
	return ZWS_STATUS_IFTTT_ERROR_SERVICE_UNAVAILABLE;
}
#endif
