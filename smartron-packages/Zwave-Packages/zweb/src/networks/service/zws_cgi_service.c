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
 * @file    zws_cgi_service.c
 *
 * @brief   CGI Input
 * @details Handles CGI Input parameters.
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

#include <stdlib.h>
#include <arpa/inet.h>

#include "zws_log.h"
#if defined(ZWS_DEBUG_CGI_SERVICE) || defined(ZWS_DEBUG_CGI_SERVICE_ROOT)
#include "zws_log_enable.h"
#else
#include "zws_log_disable.h"
#endif

#include "zws_cgi_service.h"
#include "zws_cgi_service_common.h"
#include "zws_cgi_service_networks.h"
#include "zws_io.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zwu_string_buffer.h"
#include "zws_string.h"

//#ifndef LOCAL_CLIENT_ENABLE
#include "zws_ifttt_common.h"
#include "zws_cgi_service_ifttt.h"
//#endif

/** Default block size of Avro writer */
#define ZW_AVRO_WRITER_MEMORY_DEFAULT_BLOCK_SIZE       256

/** Default allocation count for CGI input arguments */
#define ZWS_INPUT_ARGS_DEFAULT_ALLOC_COUNT              4

#if 0
//man environ(7)
extern char **environ;
#endif

/** CGI service class */
struct _zws_service
{
	/* Level specific */
	zws_cgi_command_type_t        command_type;                /**< Level 1 commands: 'networks', 'scenes', etc  */
	void                          *service_next;               /**< Next level service based on 'command' */

	/* Common to all levels */
	zws_service_common_t          service_common;              /**< Parameters common to all levels */
};

/**
 * @brief Set AVI message information
 * @param[in]     service           Service object
 * @param[in]     cgi_uri           CGI URI
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
zws_status_t zws_cgi_set_avi_message_info(zws_service_t service, char *cgi_uri)
{
	return zws_avi_get_avi_message_from_cgi_uri(zws_avi_get_message_list_root(service->service_common->avi),
			cgi_uri,
			&service->service_common->message_name, &service->service_common->error_text,
			(zws_avi_message_t **)&service->service_common->avi_message);
}

/**
 * @brief Create Avro value object for cookies
 * @param[in]     service           Service object
 * @param[out]    cookies_value_p   Avro value object for cookies
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zws_cgi_destroy_avro_value_of_cookies() to free resources
 */
static zws_status_t zws_cgi_create_avro_value_of_cookies(zws_service_t service, avro_value_t *cookies_value_p)
{
	zw_avro_t avro;
	avro_value_t user_field;
	avro_value_t user_string;
	char *user;

	avro = zws_avi_get_avro(service->service_common->avi);

	if (avro_generic_value_new(zw_avro_get_cookies_iface(avro), cookies_value_p) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'Cookies' record from interface: %s", avro_strerror());
		goto l_err_cookie_record;
	}

	if (avro_value_get_by_name(cookies_value_p, "user", &user_field, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'user' field from 'Cookies' record: %s", avro_strerror());
		goto l_err_get_user_field;
	}

	user = service->service_common->user;

	if (user)
	{
		if (avro_value_set_branch(&user_field, 0, &user_string) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to set 'user' field to branch '0': %s", avro_strerror());
			goto l_err_set_user_field;
		}

		if (avro_value_set_string(&user_string, user) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to set 'user' string: %s", avro_strerror());
			goto l_err_set_user_string;
		}
	}
	else
	{
		if (avro_value_set_branch(&user_field, 1, NULL) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to set 'user' field to branch '1': %s", avro_strerror());
			goto l_err_set_user_field;
		}
	}

	return ZWS_STATUS_OK;

l_err_set_user_string:
l_err_set_user_field:
l_err_get_user_field:
	avro_value_decref(cookies_value_p);
l_err_cookie_record:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Free avro value object for cookies
 * @param[in]     cookies_value_p   Avro value object for cookies
 * @return        Nothing
 * @see zws_cgi_create_avro_value_of_cookies
 */
static void zws_cgi_destroy_avro_value_of_cookies(avro_value_t *cookies_value_p)
{
	avro_value_decref(cookies_value_p);
}

/**
 * @brief Create Avro value object for message name
 * @param[in]     service           Service object
 * @param[in]     message_name      Message name
 * @param[out]    message_value_p   Avro value object for message name
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zws_cgi_destroy_avro_value_of_message_name() to free resources
 */
static zws_status_t zws_cgi_create_avro_value_of_message_name(zws_service_t service, const char *message_name, avro_value_t *message_value_p)
{
	if (avro_generic_string_new(message_value_p, message_name) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'message' value for '%s': %s", message_name, avro_strerror());
		goto l_err_message_value;
	}

	return ZWS_STATUS_OK;

l_err_message_value:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Free avro value object for cookies
 * @param[in]     message_value_p   Avro value object for message name
 * @return        Nothing
 * @see zws_cgi_create_avro_value_of_message_name
 */
static void zws_cgi_destroy_avro_value_of_message_name(avro_value_t *message_value_p)
{
	avro_value_decref(message_value_p);
}

/**
 * @brief Set Avro value for RPC request for given Avro type name
 * @param[in]     service           Service object
 * @param[in]     type_name         Avro type name
 * @param[out]    arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_cgi_set_avro_value_of_request_arg_root(zws_service_t service, const char *type_name, avro_value_t *arg_value_p)
{
	avro_value_t commad_value;
	avro_value_t parameter_value;
	zws_avi_message_t *avi_message;

	if (!strcmp(type_name, "Void"))
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "Request is 'Void'");
	}
	else if (service->service_common->cgi_client == ZWS_CGI_CLIENT_IFTTT)
	{
#ifndef LOCAL_CLIENT_ENABLE
		if (zws_cgi_set_avro_value_of_request_arg_ifttt((pzws_service_ifttt_t)service->service_next, type_name, arg_value_p) != ZWS_STATUS_OK)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to set arguments for IFTTT type '%s'", type_name);
			goto l_err_ifttt_argument_handler;
		}
#endif
	}
	else if (service->service_common->command_level == ZWS_CGI_COMMAND_LEVEL_ROOT)
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "Avro type: '%s' (Root level)", type_name);

		if ((avro_value_get_by_name(arg_value_p, "command", &commad_value, NULL)) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'command' field within argument of type '%s': %s", type_name, avro_strerror());
			goto l_err_get_command_value;
		}

		avi_message = (zws_avi_message_t *)service->service_common->avi_message;

		// Optional field
		if ((avro_value_get_by_name(arg_value_p, "parameter", &parameter_value, NULL)) == 0)
		{
			if (avi_message->argument_handler)
			{
				if (avi_message->argument_handler(service->service_common->args,
					service->service_common->args_count, &parameter_value) != ZWS_STATUS_OK)
				{
					ZWS_LOG(ZWS_LOG_ERR, "Failed to set arguments for type '%s'", type_name);
					goto l_err_argument_handler;
				}
			}
		}
	}
	else
	{
		if (zws_cgi_set_avro_value_of_request_arg_networks((zws_service_networks_t)service->service_next, type_name, arg_value_p) != ZWS_STATUS_OK)
		{
			goto l_err_service_arg_networks;
		}
	}

	return ZWS_STATUS_OK;

l_err_service_arg_networks:
#ifndef LOCAL_CLIENT_ENABLE
	l_err_ifttt_argument_handler:
#endif
l_err_argument_handler:
l_err_get_command_value:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Set Avro value for RPC request for given message name
 * @param[in]     service           Service object
 * @param[in]     avi_message       AVI message
 * @param[in]     message_name      Message name
 * @param[out]    request_value_p   Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zws_cgi_destroy_avro_value_of_request() to free resources
 */
static zws_status_t zws_cgi_create_avro_value_of_request(zws_service_t service,
		void *avi_message, const char *message_name, avro_value_t *request_value_p)
{
	zw_avro_t avro;
	avro_schema_t message_schema;
	avro_schema_t request_schema;
	const char *request_schema_name;
	avro_schema_t param_schema;
	avro_value_t arg_value;
	int type_index;

	avro = zws_avi_get_avro(service->service_common->avi);

	if ((message_schema = avro_schema_get_subschema(zw_avro_get_messages_schema(avro), message_name)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the schema of '%s' message: %s", message_name, avro_strerror());
		goto l_err_message_schema;
	}

	if ((request_schema = avro_schema_get_subschema(message_schema, "request")) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the request schema of '%s' message: %s", message_name, avro_strerror());
		goto l_err_request_schema;
	}

	if ((request_schema_name = avro_schema_type_name(request_schema)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get type name for request schema of '%s' message: %s", message_name, avro_strerror());
		goto l_err_type_name;
	}

	if ((param_schema = avro_schema_union_branch_by_name(zw_avro_get_types_schema(avro), &type_index, request_schema_name)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get '%s' schema from 'types' schema: %s", request_schema_name, avro_strerror());
		goto l_err_arg_type_index;
	}

	if (avro_generic_value_new(zw_avro_get_types_iface(avro), request_value_p) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get arg value from type interface '%s': %s", request_schema_name, avro_strerror());
		goto l_err_type_value;
	}

	if (avro_value_set_branch(request_value_p, type_index, &arg_value) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to set 'user' field to branch '0': %s", avro_strerror());
		goto l_err_set_branch_type;
	}

	ZWS_LOG(ZWS_LOG_DEBUG, "RPC: %s(%s)", message_name, request_schema_name);

	if ((avro_typeof(param_schema) == AVRO_RECORD)
			&& (zws_cgi_set_avro_value_of_request_arg_root(service, request_schema_name, &arg_value) != ZWS_STATUS_OK))
	{
		goto l_err_request_args;
	}

	return ZWS_STATUS_OK;

l_err_request_args:
l_err_set_branch_type:
	avro_value_decref(request_value_p);
l_err_type_value:
l_err_arg_type_index:
l_err_type_name:
l_err_request_schema:
l_err_message_schema:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Free Avro value for RPC request
 * @param[in]     request_value_p   Avro value for RPC request
 * @return        Nothing
 * @see zws_cgi_create_avro_value_of_request
 */
static void zws_cgi_destroy_avro_value_of_request(avro_value_t *request_value_p)
{
	avro_value_decref(request_value_p);
}

zws_status_t zws_cgi_get_rpc_request_from_cgi_service(zws_service_t service,
		char **rpc_request_p, int *rpc_request_length)
{
	avro_writer_t writer;
	avro_value_t cookies_value;
	avro_value_t message_name_value;
	avro_value_t request_value;
	char *rpc_request, *tmp;
	long written;
	int size;
	int avro_code;
	uint32_t avro_message_length;

	ZWS_LOG(ZWS_LOG_DEBUG, "Check avi_msg: %p", service->service_common->avi_message);

	// Commands that doesn't translate to Avro message. Eg: zwif_xxx_rpt_set ifttt_user_info etc
	if ((service->service_common->avi_message == NULL) ||
		(service->service_common->message_name == NULL) )
	{
		*rpc_request_p = NULL;
		*rpc_request_length = 0;
		return ZWS_STATUS_OK;
	}

	// Get cookies
	if(zws_cgi_create_avro_value_of_cookies(service, &cookies_value) != ZWS_STATUS_OK)
	{
		goto l_err_get_cookies;
	}

	// Get message
	if(zws_cgi_create_avro_value_of_message_name(service, service->service_common->message_name, &message_name_value) != ZWS_STATUS_OK)
	{
		goto l_err_get_message;
	}

	// Get request parameters
	if(zws_cgi_create_avro_value_of_request(service, service->service_common->avi_message,
			service->service_common->message_name, &request_value) != ZWS_STATUS_OK)
	{
		goto l_err_get_request;
	}

	ZWS_LOG(ZWS_LOG_DEBUG, "zws_cgi_create_avro_value_of_request PASS");

	// Write with on-demand memory allocation
	rpc_request = NULL;
	written = 0;
	size = 0;

	while (1)
	{
		size += ZW_AVRO_WRITER_MEMORY_DEFAULT_BLOCK_SIZE;

		if ((tmp = (char *)ZWS_REALLOC(rpc_request, size)) == NULL)
		{
			ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
			goto l_err_realloc;
		}

		rpc_request = tmp;

		if ((writer = avro_writer_memory(rpc_request + ZW_AVRO_FRAME_MESSAGE_DATA_OFFSET,
				size - ZW_AVRO_FRAME_MESSAGE_DATA_OFFSET)) == NULL)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to create avro writer: %s", avro_strerror());
			goto l_err_avro_writer;
		}

		// Write cookies, message and its request parameter
		if (((avro_code = avro_value_write(writer, &cookies_value)) == 0)
				&& ((avro_code = avro_value_write(writer, &message_name_value)) == 0)
				&& ((avro_code = avro_value_write(writer, &request_value)) == 0))
		{
			//Success
			break;
		}
		else if (avro_code != ENOSPC)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to write 'Cookie' and 'Request' value: %s", avro_strerror());
			goto l_err_value_write;
		}

		// ENOSPC, so free the writer and retry with bigger buffer
		avro_writer_free(writer);
	}

	if ((written = avro_writer_tell(writer)) < 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get buffer size from writer: %s", avro_strerror());
		goto l_err_writer_tell;
	}

	zws_cgi_destroy_avro_value_of_request(&request_value);
	zws_cgi_destroy_avro_value_of_message_name(&message_name_value);
	zws_cgi_destroy_avro_value_of_cookies(&cookies_value);

	avro_writer_free(writer);

	if (written > ZW_AVRO_FRAME_MESSAGE_LENGTH_MASK)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to build Avro frame: Message length is too long");
		goto l_err_length_too_long;
	}

	avro_message_length = htonl((uint32_t)written);
	// Host byte order to Network byte order
	memcpy(rpc_request + ZW_AVRO_FRAME_MESSAGE_LENGTH_OFFSET, &avro_message_length, ZW_AVRO_FRAME_MESSAGE_LENGTH_SIZE);

	*rpc_request_p = rpc_request;
	*rpc_request_length = written + ZW_AVRO_FRAME_MESSAGE_LENGTH_SIZE;

	return ZWS_STATUS_OK;

l_err_length_too_long:
l_err_writer_tell:
l_err_value_write:
	avro_writer_free(writer);
l_err_avro_writer:
l_err_realloc:
	ZWS_FREE(rpc_request);
	zws_cgi_destroy_avro_value_of_request(&request_value);
l_err_get_request:
	zws_cgi_destroy_avro_value_of_message_name(&message_name_value);
l_err_get_message:
	zws_cgi_destroy_avro_value_of_cookies(&cookies_value);
l_err_get_cookies:
	return ZWS_STATUS_ERROR;
}

void zws_cgi_free_rpc_request_from_cgi_service(char *rpc_request)
{
	ZWS_FREE(rpc_request);
}

#ifndef LOCAL_CLIENT_ENABLE
/**
* @brief Get user name from session object
* @param[in]     session           Session object
* @param[out]    user_p            User name
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call ZWS_FREE(*user_p) to free resources
*/
static zws_status_t zws_cgi_ifttt_get_user(zws_service_common_t service_common, char* relative_uri, char **user_p)
{
	zws_status_t return_status = ZWS_STATUS_IFTTT_ERROR_OA2_AT_INVALID;
	char *ifttt_channel_key = NULL;
	char *sAccessToken = NULL;
	char *ifttt_user;
	char *user = NULL;
	int bExist = 0;

	//If channel key is found, no need to check access token
	if ((ifttt_channel_key = getenv(IFTTT_CHANNEL_KEY_HEADER)) != NULL)
	{
		if ((!strcmp(ifttt_channel_key, IFTTT_CHANNEL_KEY_VALUE)) || (!strcmp(ifttt_channel_key, IFTTT_CHANNEL_KEY_VALUE2)))
		{
			ifttt_user = IFTTT_TEST_USER_ID;
			ZWS_LOG(ZWS_LOG_DEBUG, "IFTTT Channel Key in header matched record. Use default IFTTT user ID: %s", ifttt_user);

			//Check for existence
			if ((zws_cgi_ifttt_check_user_existence(ifttt_user, &bExist) != ZWS_STATUS_OK) || (bExist == 0))
			{
				ZWS_LOG(ZWS_LOG_ERR, "ifttt user: %s do not exist.", ifttt_user);
				service_common->error_text = IFTTT_ERROR_STR_AUTH_IFTTT_USER_NOT_FOUND;
				ifttt_user = NULL;

				goto l_err_ifttt_default_user_not_exist;
			}
		}
		else
		{
			ZWS_LOG(ZWS_LOG_ERR, "IFTTT Channel Key in header not match record. Invalid request.");
			service_common->error_text = IFTTT_ERROR_STR_AUTH_CHANNEL_KEY_NOT_MATCH;

			goto l_err_channel_key_not_match;
		}
	}
	else if ((sAccessToken = getenv(IFTTT_AUTHORIZATION_HEADER)) != NULL)
	{
		bExist = zws_string_remove_substring(&sAccessToken, IFTTT_AUTHORIZATION_HEADER_BEARER);

		if (bExist && sAccessToken)
		{
			if(zws_cgi_ifttt_oauth2_user_name_get(sAccessToken, &ifttt_user) == ZWS_STATUS_OK)
			{
				//Get from OAuth2 server
				ZWS_LOG(ZWS_LOG_DEBUG, "User Name from OAuth2: '%s'", ifttt_user);

				user = ZWS_STRDUP(ifttt_user);

				ZWS_FREE(ifttt_user);
				ifttt_user = NULL;

				return_status = ZWS_STATUS_OK;
			}
			else
			{
				service_common->error_text = IFTTT_ERROR_STR_AUTH_NO_MATCH_USER;
				ZWS_LOG(ZWS_LOG_ERR, "Cannot find match user with Access token.");
				goto l_err_user_not_found;
			}
		}
		else
		{
			service_common->error_text = IFTTT_ERROR_STR_AUTH_ACCESS_TOKEN_WRONG_FORMAT;
			ZWS_LOG(ZWS_LOG_ERR, "Access Token format not correct.");
			goto l_err_access_token_format;
		}
	}
	else
	{
		ZWS_LOG(ZWS_LOG_ERR, "Neither IFTTT Channel Key nor Access Token is found in header. Invalid request.");
		service_common->error_text = IFTTT_ERROR_STR_AUTH_NO_CHANNEL_KEY_NOR_TOKEN;
		goto l_err_no_key_and_token;
	}

	if (!user && ifttt_user && (user = ZWS_STRDUP(ifttt_user)) != NULL)
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "User Name from IFTTT server: '%s'", ifttt_user);

		return_status = ZWS_STATUS_OK;
	}

	*user_p = user;

l_err_no_key_and_token:
l_err_access_token_format:
l_err_user_not_found:
l_err_channel_key_not_match:
l_err_ifttt_default_user_not_exist:
	return return_status;
}

#endif

/**
 * @brief Get user name from session object
 * @param[in]     session           Session object
 * @param[out]    user_p            User name
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call ZWS_FREE(*user_p) to free resources
 */
static zws_status_t zws_cgi_get_user(zws_service_common_t service_common, zws_cgi_client_type_t cgi_client, char* relative_uri, zws_session_t session, char **user_p)
{
	char *user;
	char *remote_user;
	char *session_user;
	zws_status_t return_status;
	zws_status_t get_user_return_status;

	return_status = ZWS_STATUS_ERROR;
	get_user_return_status = ZWS_STATUS_ERROR;

	user = NULL;

	session_user = NULL;

	if (cgi_client == ZWS_CGI_CLIENT_ZWARE)
	{
		/*
		 * ZWare Session user
		 */
		if ((get_user_return_status = zws_session_user_name_get(session, &session_user)) == ZWS_STATUS_OK)
		{
			ZWS_LOG(ZWS_LOG_DEBUG, "User Name from Session: '%s'", session_user);

			user = session_user;
		}
	}
	else if (cgi_client == ZWS_CGI_CLIENT_IFTTT)
	{
#ifndef LOCAL_CLIENT_ENABLE
		get_user_return_status = zws_cgi_ifttt_get_user(service_common, relative_uri, &user);
#endif
	}

	/*
	 * FIXME: If depending on environment variable is a security hole, got to plug it.
	 */
	if (user == NULL)
	{
		if ((remote_user = getenv("REMOTE_USER"))
			&& ((user = ZWS_STRDUP(remote_user)) != NULL))
		{
			ZWS_LOG(ZWS_LOG_DEBUG, "User Name from REMOTE_USER: '%s'", remote_user);
		}
		else
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to find user name. Error code: %d", get_user_return_status);

			return_status = get_user_return_status;

			goto l_err_user_name;
		}
	}

	ZWS_LOG(ZWS_LOG_DEBUG, "User Name (effective): '%s'", (user ? user : "null"));

	/*
	 * Set ZWARE_USER_NAME environment variable for use in zw_info API.
	 */
	if (user)
	{
		setenv("ZWARE_USER_NAME", user, 1);
	}
	else
	{
		unsetenv("ZWARE_USER_NAME");
	}

	*user_p = user;

	return ZWS_STATUS_OK;

l_err_user_name:
	return return_status;
}

/**
 * @brief Add a key-value pair to argument list
 * @param[out]    args_p            Argument list
 * @param[out]    args_count        Number of argument
 * @param[in]     key               Key
 * @param[in]     value             Value
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_cgi_add_args(char *(**args_p)[2], int *args_count, char *key, char *value)
{
	int entry_count;
	int size;
	char *(*args)[2];
	char *(*tmp)[2];

	args = *args_p;
	entry_count = *args_count;

	size = entry_count + 1;

	if ((tmp = (char *(*)[2])ZWS_REALLOC(args, size * sizeof(*tmp))) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_realloc;
	}

	args = tmp;
	ZWS_LOG(ZWS_LOG_DEBUG, "Reallocated args list size to %d arg pairs", size);

	args[entry_count][0] = key;
	args[entry_count][1] = value;

	entry_count++;

	*args_p = args;
	*args_count = entry_count;

	return ZWS_STATUS_OK;

l_err_realloc:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Get argument list from HTTP body
 * @param[in]     http_body         HTTP body
 * @param[out]    args_p            Argument list
 * @param[out]    args_count        Number of argument
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zws_cgi_free_args() to free resources
 */
static zws_status_t zws_cgi_get_args(char *http_body, char *(**args_p)[2], int *args_count)
{
	char *entry;
	char *next_entry;
	int entry_count;
	char *key;
	char *value;
	char *(*args)[2];
	char *(*tmp)[2];
	int size;  /* Available memory for 'size' number of argument pairs */

	entry = http_body;
	entry_count = 0;

	args = NULL;
	size = 0;

	if (strchr(http_body, '`'))
	{
		ZWS_LOG(ZWS_LOG_ALERT, "Use of unsafe character backtick: Possible attempt to execute shell sub command");
		goto l_err_unsafe_backtick;
	}

	while (entry && entry[0])
	{
		/*Entry delimiters:
		 * '\r\n' for Web API v1;
		 * '&' for Web API v2
		 */
		if ((next_entry = strstr(entry, "&")))
		{
			*next_entry = '\0';
			next_entry++;
		}
		else if ((next_entry = strstr(entry, "\r\n")))
		{
			*next_entry = '\0';
			next_entry += 2;
		}

		//On demand memory allocation for args list
		if (entry_count >= size)
		{
			size += ZWS_INPUT_ARGS_DEFAULT_ALLOC_COUNT;

			if ((tmp = (char *(*)[2])ZWS_REALLOC(args, size * sizeof(*tmp))) == NULL)
			{
				ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
				goto l_err_realloc;
			}
			args = tmp;
			ZWS_LOG(ZWS_LOG_DEBUG, "Reallocated args list size to %d arg pairs", size);
		}

		ZWS_LOG(ZWS_LOG_DEBUG, "Entry: %s", entry);

		//Split key and value from entry
		key = entry;

		if (!(value = strstr(key, "=")))
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to find key-value delimiter in argument entry: %s", entry);
			goto l_err_key_value;
		}

		*value='\0';
		value++;

		if (!key[0])
		{
			ZWS_LOG(ZWS_LOG_ERR, "Empty key with value: '%s'", value);
			goto l_err_empty_key;
		}

		args[entry_count][0] = key;
		args[entry_count][1] = value;

		entry_count++;
		// Iterate to next entry
		entry = next_entry;
	}

	ZWS_LOG(ZWS_LOG_DEBUG, "Entry count: %d", entry_count);

	*args_p = args;
	*args_count = entry_count;

	return ZWS_STATUS_OK;

l_err_empty_key:
l_err_key_value:
l_err_realloc:
l_err_unsafe_backtick:
	ZWS_FREE(args);
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Free argument list
 * @param[in]     args              Argument list
 * @return        Nothing
 * @see zws_cgi_get_args
 */
static void zws_cgi_free_args(char *(*args)[2])
{
	ZWS_FREE(args);
}

/**
* @brief Get HTTP requester/CGI client of the call
* @param[in]     relative_uri      Relative URI
* @param[out]    pcgi_client	   CGI client
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*
*/
static zws_status_t zws_cgi_get_client(char* relative_uri, zws_cgi_client_type_t *pcgi_client)
{
	char *uri_next = NULL;
	char *root_command = NULL;
	char *relative_uri_temp = NULL;
	char *relative_uri_temp1 = NULL;
	//char *ifttt_channel_key = NULL;

	*pcgi_client = ZWS_CGI_CLIENT_UNKNOWN;

	if ((relative_uri_temp = ZWS_STRDUP(relative_uri)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_uri_temp_alloc;
	}

	relative_uri_temp1 = relative_uri_temp;

	if (relative_uri_temp1 && relative_uri_temp1[0])
	{
		//'zcgi'
		if ((uri_next = strstr(relative_uri_temp1, "/")))
		{
			*uri_next = '\0';
			uri_next++;
		}

		relative_uri_temp1 = uri_next;

		// Level 1 commands: 'networks' or 'ifttt'
		if (relative_uri_temp1 && relative_uri_temp1[0])
		{
			if ((uri_next = strstr(relative_uri_temp1, "/")))
			{
				*uri_next = '\0';
				uri_next++;
			}

			root_command = relative_uri_temp1;
			relative_uri_temp1 = uri_next;

			if (!strcmp(root_command, "networks"))
			{
				*pcgi_client = ZWS_CGI_CLIENT_ZWARE;
			}
			else if (!strcmp(root_command, "ifttt"))
			{
				ZWS_LOG(ZWS_LOG_DEBUG, "Request contains client keyword 'ifttt'.");
				//It could be ifttt request.
				//Validate the request comes from IFTTT

				//Validate IFTTT Channel key with Access Token. It seems like their design
				//is EITHER one is valid in every request.
				*pcgi_client = ZWS_CGI_CLIENT_IFTTT;

			}
			else
			{
				*pcgi_client = ZWS_CGI_CLIENT_UNKNOWN;
			}
		}
	}

	ZWS_LOG(ZWS_LOG_DEBUG, "cgi_client = %s", (*pcgi_client == ZWS_CGI_CLIENT_UNKNOWN) ? "ZWS_CGI_CLIENT_UNKNOWN" : 
											  ((*pcgi_client == ZWS_CGI_CLIENT_ZWARE) ? "ZWS_CGI_CLIENT_ZWARE" : "ZWS_CGI_CLIENT_IFTTT"));

	ZWS_FREE(relative_uri_temp);
	
	return ZWS_STATUS_OK;

l_err_uri_temp_alloc:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Get relative URI
 * @param[out]    relative_uri_p    Relative URI
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zws_cgi_free_relative_uri() to free resources
 */
static zws_status_t zws_cgi_get_relative_uri(char **relative_uri_p)
{
	char *request_uri;
	char *context_prefix;
	char *uri;
	char *relative_uri;
	#if 0
	char **envp;


	envp = environ;

	while (*envp)
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "%s", *envp);
		envp++;
	}
	#endif

	if ((request_uri = getenv("REQUEST_URI")) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to find CGI variable REQUEST_URI");
		goto l_err_request_uri;
	}

	if ((context_prefix = getenv("CONTEXT_PREFIX")) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to find CGI variable CONTEXT_PREFIX");
		goto l_err_context_prefix;
	}

	ZWS_LOG(ZWS_LOG_DEBUG, "REQUEST_URI: '%s' CONTEXT_PREFIX: '%s'", request_uri, context_prefix);

	// Strip context prefix
	if (memcmp(request_uri, context_prefix, strlen(context_prefix)))
	{
		ZWS_LOG(ZWS_LOG_ERR, "CONTEXT_PREFIX prefix is not a prefix for REQUEST_URI. Unusual.");
		goto l_err_context_prefix_check;
	}

	uri = request_uri + strlen(context_prefix);

	if ((relative_uri =	ZWS_STRDUP(uri)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_relative_uri_alloc;
	}

	ZWS_LOG(ZWS_LOG_DEBUG, "Relative URI: %s", relative_uri);

	*relative_uri_p = relative_uri;

	return ZWS_STATUS_OK;

l_err_relative_uri_alloc:
l_err_context_prefix_check:
l_err_context_prefix:
l_err_request_uri:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Free relative URI
 * @param[in]     relative_uri      Relative URI
 * @return        Nothing
 * @see zws_cgi_get_relative_uri
 */
static void zws_cgi_free_relative_uri(char *relative_uri)
{
	ZWS_FREE(relative_uri);
}

zws_status_t zws_cgi_get_zware_command(zws_service_t service, zws_status_t get_user_return_status)
{
	zws_service_common_t service_common;
	char *relative_uri = NULL;
	//char *uri_next = NULL;
	char *root_command = NULL;
	char *net_id = NULL;
	char *web_client_address = NULL;
	zws_status_t res = ZWS_STATUS_ERROR;

	if (!service) return res;

	service_common = service->service_common;

	relative_uri = service_common->relative_uri;

	ZWS_LOG(ZWS_LOG_DEBUG, "relative_uri 1: '%s'", relative_uri);
	
	//strip off zcgi
	if((root_command = zws_url_string_strip_1layer(&relative_uri)))
	{
		// Level 1 commands: 'networks', 'ifttt'
		if ((root_command = zws_url_string_strip_1layer(&relative_uri)))
		{
			// Remote Access Code
			if ((net_id = zws_url_string_strip_1layer(&relative_uri)))
			{
				ZWS_LOG(ZWS_LOG_DEBUG, "Network id: '%s'", net_id);
			}
			else
			{
				net_id = relative_uri;
				relative_uri = NULL;	//switch net_id and relative_uri value
			}

			ZWS_LOG(ZWS_LOG_DEBUG, "net_id: %s", net_id);
			
			if (!relative_uri || !relative_uri[0])
			{
				ZWS_LOG(ZWS_LOG_DEBUG, "CGI command: '%s' (Root level)", root_command);

				if (!strcmp(root_command, "networks"))
				{
#ifdef LOCAL_CLIENT_ENABLE
					goto l_err_invalid_root_command;
#endif
					if (net_id && net_id[0])
					{
						strcpy(service_common->command_name, "zw_refresh_network");

						// Pass the network ID as argument
						if (zws_cgi_add_args(&service_common->args_all, &service_common->args_count,
							"network_id", net_id) != ZWS_STATUS_OK)
						{
							goto l_err_add_args;
						}
						service_common->args = service_common->args_all;

						/*
						* There is no check for valid user name here. This allows
						* internal apps to use this API easily.
						* FIXME: This may be a security hole. Plug it after the
						* internal apps call this API with auth information. For now,
						* the check is there for web client address address to be 127.0.0.1
						*/
						if ((web_client_address = getenv("REMOTE_ADDR")) == NULL)
						{
							ZWS_LOG(ZWS_LOG_ERR, "Failed to get environment variable REMOTE_ADDR");
							goto l_err_web_client_address_get;
						}

						ZWS_LOG(ZWS_LOG_DEBUG, "Web client address: %s", web_client_address);

						if (strcmp(web_client_address, "127.0.0.1"))
						{
							ZWS_LOG(ZWS_LOG_ERR, "Environment variable REMOTE_ADDR is not 127.0.0.1. Access denied.");
							goto l_err_web_client_address_match;
						}
					}
					else
					{
						strcpy(service_common->command_name, "zw_get_network_list");
					}
				}
				else
				{
					ZWS_LOG(ZWS_LOG_DEBUG, "Invalid command at root level: %s", root_command);
					goto l_err_invalid_root_command;
				}

				if (zws_cgi_set_avi_message_info(service, service->service_common->command_name) != ZWS_STATUS_OK)
				{
					goto l_err_set_avi_message_info;
				}
			}
			else
			{
				if (get_user_return_status == ZWS_STATUS_EACCES)
				{
					res = get_user_return_status;

					goto l_err_invalid_user_for_command_type_networks;
				}

#ifdef LOCAL_CLIENT_ENABLE
				if (!strcmp(relative_uri, "zw_gw_set"))
				{
					service->command_type = ZWS_CGI_COMMAND_TYPE_ROOT;
					strcpy(service->service_common->command_name, "zw_gw_set");
					//service->service_common->command_name = "zw_gw_set";
					if (zws_cgi_set_avi_message_info(service, service->service_common->command_name) != ZWS_STATUS_OK)
					{
						goto l_err_set_gw_set_avi_message_info;
					}
				}
				else if (!strcmp(relative_uri, "zw_gw_get"))
				{
					service->command_type = ZWS_CGI_COMMAND_TYPE_ROOT;
					strcpy(service->service_common->command_name, "zw_gw_get");
					//service->service_common->command_name = "zw_gw_get";
					if (zws_cgi_set_avi_message_info(service, service->service_common->command_name) != ZWS_STATUS_OK)
					{
						goto l_err_set_gw_get_avi_message_info;
					}
				}
				else if (!strcmp(relative_uri, "zw_gw_list"))
				{
					service->command_type = ZWS_CGI_COMMAND_TYPE_ROOT;
					strcpy(service->service_common->command_name, "zw_gw_list");
					//service->service_common->command_name = "zw_gw_list";
					if (zws_cgi_set_avi_message_info(service, service->service_common->command_name) != ZWS_STATUS_OK)
					{
						goto l_err_set_gw_list_avi_message_info;
					}
				}
				else
#endif
				{
					service->command_type = ZWS_CGI_COMMAND_TYPE_NETWORKS;

					if (zws_cgi_get_cgi_service_networks_from_environment(relative_uri, net_id, service_common,
						(zws_service_networks_t *)&service->service_next) != ZWS_STATUS_OK)
					{
						ZWS_LOG(ZWS_LOG_ERR, "No match with supported commands");
						goto l_err_get_service_param;
					}
				}
			}
		}
	}

	return ZWS_STATUS_OK;

l_err_get_service_param:
#ifdef LOCAL_CLIENT_ENABLE
l_err_set_gw_list_avi_message_info :
l_err_set_gw_get_avi_message_info:
l_err_set_gw_set_avi_message_info:
#endif
l_err_invalid_user_for_command_type_networks:
l_err_set_avi_message_info:
l_err_web_client_address_match:
l_err_web_client_address_get:
l_err_add_args:
l_err_invalid_root_command:
	return res;
}

zws_status_t zws_cgi_get_cgi_service_from_environment(zws_service_t *service_p, zws_avi_t avi, zws_session_t session)
{
	zws_status_t return_status;
	zws_service_t service;
	zws_service_common_t service_common;
	zws_status_t get_user_return_status;

	return_status = ZWS_STATUS_ERROR;

	if ((service = (zws_service_t)ZWS_MALLOC(sizeof(*service))) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_service_alloc;
	}

	if ((service_common = (zws_service_common_t)ZWS_MALLOC(sizeof(*service_common))) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		if (service) ZWS_FREE(service);
		*service_p = NULL;
		goto l_err_service_common_alloc;
	}
	service->service_common = service_common;

	service->command_type = ZWS_CGI_COMMAND_TYPE_ROOT;
	service->service_next = NULL;

	service_common->cgi_client = ZWS_CGI_CLIENT_UNKNOWN;
	service_common->user = NULL;
	service_common->args_all = NULL;
	service_common->args = NULL;
	service_common->args_count = 0;
	//memset(&service_common->stifttt_args, 0, sizeof(ifttt_args_t));

	service_common->result = NULL;
	service_common->command_level = ZWS_CGI_COMMAND_LEVEL_ROOT;
	service_common->avi = avi;
	memset(service_common->command_name, 0, ZWS_MAX_COMMAND_NAME_LENGTH);

	service_common->error_code = ZWS_CGI_ERROR_CODE_SUCCESS;
	service_common->error_text = NULL;
	service_common->error_text_index = ZWS_CGI_ERROR_TEXT_INDEX_NONE;

	service_common->relative_uri = NULL;
	service_common->http_body = NULL;
	service_common->http_body_length = 0;

	*service_p = service;

	//Angela 20 June 2016
	//Get relative URI to determine if it is request is from ZWare or 3rd party client (eg. IFTTT)
	//Get relative URI
	if (zws_cgi_get_relative_uri(&service_common->relative_uri) != ZWS_STATUS_OK)
	{
		goto l_err_get_relative_uri;
	}

	//Get cgi_client (ZWARE/IFTTT)
	zws_cgi_get_client(service_common->relative_uri, &service_common->cgi_client);
	
	if(service_common->cgi_client == ZWS_CGI_CLIENT_UNKNOWN)
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "ZWS_CGI_CLIENT_UNKNOWN detected. Return ERROR.");
		return_status = ZWS_STATUS_IFTTT_ERROR_OA2_AT_INVALID;
		goto l_err_get_cgi_client;
	}

	//Get user
	if ((get_user_return_status = zws_cgi_get_user(service_common, service_common->cgi_client, service_common->relative_uri, session, &service_common->user)) != ZWS_STATUS_OK)
	{
		service_common->user = NULL;

		//When registering a new portal user, PHP script will send a fcgi request with no HTTP_COOKIE,
		//which make it impossible to find the 'user' of the request and ZWS_STATUS_EACCES will be returned.
		//This is a valid case and should be handled and should still proceed.
			if (get_user_return_status != ZWS_STATUS_EACCES)
		{
			// ZWS_STATUS_ERROR
			return_status = get_user_return_status;
			goto l_err_get_user;
		}
	}

	// Get HTTP body
	if (zws_read_from_FILE_with_dest_alloc(stdin, &service_common->http_body, &service_common->http_body_length, 0) != ZWS_STATUS_OK)
	{
		goto l_err_read_body;
	}

	ZWS_LOG(ZWS_LOG_DEBUG, "Body length: %d Body: '%s'", service->service_common->http_body_length, service->service_common->http_body);

	if (service_common->cgi_client != ZWS_CGI_CLIENT_IFTTT)
	{
		if (zws_cgi_get_args(service_common->http_body, &service_common->args_all, &service_common->args_count) != ZWS_STATUS_OK)
		{
			goto l_err_get_args;
		}
		service_common->args = service_common->args_all;
	}

	if (service_common->cgi_client == ZWS_CGI_CLIENT_ZWARE)
	{
		if ((return_status = zws_cgi_get_zware_command(service, get_user_return_status)) != ZWS_STATUS_OK)
		{
			goto l_err_get_command;
		}
	}
	else if (service_common->cgi_client == ZWS_CGI_CLIENT_IFTTT)
	{
#ifndef LOCAL_CLIENT_ENABLE
		if((return_status = zws_cgi_get_ifttt_command(service)) != ZWS_STATUS_OK)
		{
			//goto l_err_get_command;
			//For IFTTT request, even if creating service info fails, we still need to return corresponding error code
			//with corresponding error msg.
			*service_p = service;
			return return_status;
		}
#endif
	}

	return ZWS_STATUS_OK;

l_err_get_command:
l_err_get_args:
l_err_read_body:
l_err_get_user:
l_err_get_cgi_client:
l_err_get_relative_uri:
l_err_service_common_alloc:
l_err_service_alloc:
	return return_status;
}

void zws_cgi_free_cgi_service_from_environment(zws_service_t service)
{
	zws_service_common_t service_common;

	if (service)
	{
		service_common = service->service_common;
		if (service_common)
		{
			if (service->command_type == ZWS_CGI_COMMAND_TYPE_NETWORKS)
			{
				zws_cgi_free_cgi_service_networks_from_environment((zws_service_networks_t)service->service_next);
			}
			else if(service->command_type == ZWS_CGI_COMMAND_TYPE_IFTTT)
			{
#ifndef LOCAL_CLIENT_ENABLE
				zws_cgi_free_cgi_service_ifttt_from_environment((pzws_service_ifttt_t)service->service_next);
#endif
			}
			zws_cgi_free_args(service_common->args_all);
			zws_read_dest_free(service_common->http_body);
			zws_cgi_free_relative_uri(service_common->relative_uri);
			ZWS_FREE(service_common->user);
			if (service_common->result)
			{
				zwu_string_buffer_destroy(service_common->result);
			}
			ZWS_FREE(service->service_common);
		}

		ZWS_FREE(service);
	}

	ZWS_LOG(ZWS_LOG_DEBUG, "zws_cgi_free_cgi_service_from_environment DONE");

}

/*
 * Output
 */
/**
 * @brief Allocate Avro value  for RPC response
 * @param[in]     avro              Avro object
 * @param[out]    response_value_p  Avro value for RPC response
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zws_cgi_free_avro_value_of_response() to free resources
 */
static zws_status_t zws_cgi_alloc_avro_value_of_response(zw_avro_t avro, avro_value_t *response_value_p)
{
	if (avro_generic_value_new(zw_avro_get_types_iface(avro), response_value_p) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get arg value from type interface: %s", avro_strerror());
		goto l_err_type_value;
	}

	return ZWS_STATUS_OK;

l_err_type_value:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Free Avro value for RPC response
 * @param[in]     response_value_p  Avro value for RPC response
 * @return        Nothing
 * @see zws_cgi_alloc_avro_value_of_response
 */
static void zws_cgi_free_avro_value_of_response(avro_value_t *response_value_p)
{
	avro_value_decref(response_value_p);
}

/**
 * @brief Set CGI service result using Avro value given by its Avro type name
 * @param[in]     service           Service object
 * @param[in]     type_name         Avro type name
 * @param[out]    arg_value_p       Avro value for RPC response
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_cgi_set_service_result_root(zws_service_t service, const char *type_name, avro_value_t *arg_value_p)
{
	avro_value_t command_value;
	avro_value_t parameter_value;
	zws_avi_message_t *avi_message;
#ifndef LOCAL_CLIENT_ENABLE
	zwu_string_buffer_t result;
#endif
	zws_status_t res = ZWS_STATUS_ERROR;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);

	// Prepare the result string buffer for both all responses to cater for ifttt resonse
	if (zwu_string_buffer_create(0, &service->service_common->result) != ZWS_STATUS_OK)
	{
		goto l_err_result_create;
	}

	if ((type_name != NULL) && (arg_value_p != NULL))
	{
		if (!strcmp(type_name, "Void"))
		{
			ZWS_LOG(ZWS_LOG_DEBUG, "Response is 'Void'");

			ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, ZWS_STATUS_OK);

			return ZWS_STATUS_OK;
		}

		if (service->service_common->command_level == ZWS_CGI_COMMAND_LEVEL_ROOT)
		{
			ZWS_LOG(ZWS_LOG_DEBUG, "Avro type: '%s' (Root level)", type_name);

			if ((avro_value_get_by_name(arg_value_p, "command", &command_value, NULL)) != 0)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'command' field within argument of type '%s': %s", type_name, avro_strerror());
				goto l_err_get_command_value;
			}

			// Optional field
			if ((avro_value_get_by_name(arg_value_p, "parameter", &parameter_value, NULL)) != 0)
			{
				ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, ZWS_STATUS_OK);

				return ZWS_STATUS_OK;
			}
		}
	}

	if (service->command_type == ZWS_CGI_COMMAND_TYPE_IFTTT)
	{
#ifndef LOCAL_CLIENT_ENABLE
		if((res = zws_cgi_set_service_result_ifttt((pzws_service_ifttt_t)service->service_next, type_name, arg_value_p)) != ZWS_STATUS_OK)
		{
			goto l_err_service_result_ifttt;
		}
#endif
	}
	else if (service->service_common->command_level == ZWS_CGI_COMMAND_LEVEL_ROOT)
	{
		avi_message = (zws_avi_message_t *)service->service_common->avi_message;

		if (avi_message->result_handler)
		{
			// Envelope
#ifndef LOCAL_CLIENT_ENABLE
			result = service->service_common->result;
			if (service->service_common->cgi_client == ZWS_CGI_CLIENT_ZWARE)
			{
				if (zwu_string_buffer_append_format(result, "<zwuser id=\"%s\">",
					service->service_common->user) != ZWS_STATUS_OK)
				{
					goto l_err_envelope_start;
				}
			}
#endif
			if (avi_message->result_handler(ZWS_WEB_API_VERSION_2, &parameter_value, service->service_common->result) != ZWS_STATUS_OK)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Failed to set results for type '%s'", type_name);
				goto l_err_result_handler;
			}
#ifndef LOCAL_CLIENT_ENABLE
			if (service->service_common->cgi_client == ZWS_CGI_CLIENT_ZWARE)
			{
				if (zwu_string_buffer_append_string(result, "</zwuser>") != ZWS_STATUS_OK)
				{
					goto l_err_envelope_end;
				}
			}
#endif
		}
	}
	else
	{
		if (zws_cgi_set_service_result_networks((zws_service_networks_t)service->service_next, type_name, arg_value_p) != ZWS_STATUS_OK)
		{
			goto l_err_service_result_networks;
		}
	}

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, ZWS_STATUS_OK);

	return ZWS_STATUS_OK;

l_err_service_result_networks:
#ifndef LOCAL_CLIENT_ENABLE
l_err_envelope_end:
#endif
l_err_result_handler:
#ifndef LOCAL_CLIENT_ENABLE
l_err_envelope_start:
l_err_service_result_ifttt:
#endif
l_err_get_command_value:
	zwu_string_buffer_destroy(service->service_common->result);
	service->service_common->result = NULL;
l_err_result_create:
	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);
	return res;
}

/**
 * @brief Set CGI service result using Avro value for RPC response
 * @param[in]     service           Service object
 * @param[in]     response_value_p  Avro value for RPC response
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_cgi_set_cgi_service_result(zws_service_t service, avro_value_t *response_value_p)
{
	avro_value_t result_value;
	avro_schema_t param_schema;
	const char *response_schema_name;

	if (response_value_p == NULL)
		return zws_cgi_set_service_result_root(service, NULL, NULL);

	// Get specific branch (return value) from union value
	if (avro_value_get_current_branch(response_value_p, &result_value) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get current branch of response union: %s", avro_strerror());
		goto l_err_current_branch;
	}

	// Get schema from return value
	if ((param_schema = avro_value_get_schema(&result_value)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get schema of return value: %s", avro_strerror());
		goto l_err_get_value_schema;
	}

	if ((response_schema_name = avro_schema_type_name(param_schema)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get type name for return schema: %s", avro_strerror());
		goto l_err_type_name;
	}

	ZWS_LOG(ZWS_LOG_DEBUG, "RPC: %s %s(<argument>)", response_schema_name, service->service_common->message_name);

	if ((avro_typeof(param_schema) == AVRO_RECORD)
			&& (zws_cgi_set_service_result_root(service, response_schema_name, &result_value) != ZWS_STATUS_OK))
	{
		goto l_err_response_results;
	}

	return ZWS_STATUS_OK;

l_err_response_results:
l_err_type_name:
l_err_get_value_schema:
l_err_current_branch:
	return ZWS_STATUS_ERROR;
}

#if 0
static void zws_cgi_free_cgi_service(zws_service_t service)
{
}
#endif

zws_status_t zws_cgi_set_cgi_service_result_from_rpc_response(zws_service_t service, char *rpc_response, int rpc_response_length)
{
	zw_avro_t avro;
	uint32_t avro_message_length;
	avro_reader_t reader;
	avro_value_t response_value;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);

	// Commands that doesn't translate to Avro message. Eg: zwif_xxx_rpt_set 
	if (service->service_common->avi_message == NULL)
	{
		ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, ZWS_STATUS_OK);

		return ZWS_STATUS_OK;
	}

	//Even if message_name is NULL (doesn't translate to Avro message), still need to set result
	if (service->service_common->message_name == NULL)
	{
		//// Set service result from response
		//if (zws_cgi_set_cgi_service_result(service, &response_value) != ZWS_STATUS_OK)
		//{
		//	goto l_err_cgi_service_result;
		//}

		ZWS_LOG(ZWS_LOG_DEBUG, "Function end with zws_cgi_set_cgi_service_result().");

		return zws_cgi_set_cgi_service_result(service, NULL);
	}

	// When the message fails in portal, the received Avro message is of length 0
	if (rpc_response_length < ZW_AVRO_FRAME_MESSAGE_LENGTH_SIZE)
	{
		goto l_err_invalid_response_length;
	}

	avro = zws_avi_get_avro(service->service_common->avi);

	memcpy(&avro_message_length, rpc_response + ZW_AVRO_FRAME_MESSAGE_LENGTH_OFFSET, ZW_AVRO_FRAME_MESSAGE_LENGTH_SIZE);
	// Network byte order to Host byte order
	avro_message_length = ntohl((uint32_t)avro_message_length);

	ZWS_LOG(ZWS_LOG_DEBUG, "Avro message length: %u", avro_message_length);

	if ((reader = avro_reader_memory(rpc_response + ZW_AVRO_FRAME_MESSAGE_DATA_OFFSET, avro_message_length)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to create avro reader: %s", avro_strerror());
		goto l_err_avro_reader;
	}

	// Allocate response
	if(zws_cgi_alloc_avro_value_of_response(avro, &response_value) != ZWS_STATUS_OK)
	{
		goto l_err_alloc_response;
	}

	if (avro_value_read(reader, &response_value) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to read response value: %s", avro_strerror());
		goto l_err_read_response;
	}

	// Set service result from response
	if (zws_cgi_set_cgi_service_result(service, &response_value) != ZWS_STATUS_OK)
	{
		goto l_err_service_arg;
	}

	zws_cgi_free_avro_value_of_response(&response_value);

	avro_reader_free(reader);

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, ZWS_STATUS_OK);

	return ZWS_STATUS_OK;

l_err_service_arg:
l_err_read_response:
	zws_cgi_free_avro_value_of_response(&response_value);
l_err_alloc_response:
	avro_reader_free(reader);
l_err_avro_reader:
l_err_invalid_response_length:
	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, ZWS_STATUS_ERROR);
	return ZWS_STATUS_ERROR;
}

#if 0
void zws_cgi_free_cgi_service_from_rpc_response(zws_service_t service)
{
	zws_cgi_free_cgi_service(service);
}
#endif

zws_status_t zws_cgi_send_cgi_output(zws_service_t service)
{
	zwu_string_buffer_t result;
	zws_service_common_t service_common;

	if (service)
	{
		//If it is IFTTT client, send IFTTT response instead of zware response
		service_common = service->service_common;

		if (service_common->cgi_client == ZWS_CGI_CLIENT_IFTTT)
			return zws_cgi_ifttt_send_cgi_output(service);
	}
	else
	{
		ZWS_LOG(ZWS_LOG_ERR, "service pointer is NULL");
		goto l_err_service_null;
	}

	fprintf(stdout, "Content-Type: text/plain\n\n");

	/*
	 * Commands that doesn't translate to Avro message. Eg: zwif_xxx_rpt_set
	 * or Commands that doesn't have result text
	 */
	if ((service->service_common->avi_message == NULL)
			|| (service->service_common->result == NULL))
	{
		return ZWS_STATUS_OK;
	}

	ZWS_LOG(ZWS_LOG_DEBUG, "[1] message_name: %s", service->service_common->message_name);
	// In case of Scenes some commands can have completely empty response e.g. scene_list
	if ((zwu_string_buffer_length(service->service_common->result) > 0) ||
		(!strncmp(service->service_common->message_name, "scene", sizeof("scene") - 1)) ||
		(!strncmp(service->service_common->message_name, "security_scene", sizeof("security_scene") - 1)))
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "[2] message_name: %s", service->service_common->message_name);
		fprintf(stdout, "<?xml version=\"1.0\"?><zwave>");

		if (service->service_common->command_level == ZWS_CGI_COMMAND_LEVEL_ROOT)
		{
			result = service->service_common->result;

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
			if (zws_cgi_send_cgi_output_networks((zws_service_networks_t)service->service_next) != ZWS_STATUS_OK)
			{
				goto l_err_output_networks;
			}
		}

		fprintf(stdout, "</zwave>");
	}

	return ZWS_STATUS_OK;

l_err_output_networks:
l_err_write:
l_err_service_null:
	return ZWS_STATUS_ERROR;
}

zws_status_t zws_cgi_send_cgi_output_error(zws_service_t service)
{
	zws_service_common_t service_common;

	ZWS_LOG(ZWS_LOG_DEBUG, "service = %p", service);

	if (service)
	{
		//If it is IFTTT client, send IFTTT error instead of zware error
		service_common = service->service_common;

		if (service_common->cgi_client == ZWS_CGI_CLIENT_IFTTT)
		{
			zws_cgi_ifttt_send_cgi_output_error(ZWS_STATUS_IFTTT_ERROR_SERVER, service);
			return ZWS_STATUS_OK;
		}

	}

	ZWS_LOG(ZWS_LOG_DEBUG, "zws_cgi_send_cgi_output_error zware");

	fprintf(stdout, "Content-Type: text/plain\n\n");

	fprintf(stdout, "<?xml version=\"1.0\"?><zwave><error>");

	if (service)
	{
		service_common = service->service_common;

		if (service_common->error_text != NULL)
		{
			if (service_common->error_text_index != ZWS_CGI_ERROR_TEXT_INDEX_NONE)
			{
				fprintf(stdout, "%s[%d]", service_common->error_text, service_common->error_text_index);
			}
			else
			{
				fprintf(stdout, "%s", service_common->error_text);
			}
		}
	}

	fprintf(stdout, "</error></zwave>");
	return ZWS_STATUS_OK;
}

zws_status_t zws_cgi_send_cgi_output_access_denied(void)
{
	fprintf(stdout, "Status: 403 Forbidden\n\n");

	return ZWS_STATUS_OK;
}

void zws_cgi_set_error_text(zws_service_t service, char *error_text)
{
	zws_service_common_t service_common;

	if (service)
	{
		service_common = service->service_common;

		service_common->error_text = error_text;
	}
}

char *zws_cgi_get_error_text(zws_service_t service)
{
	char *error_text = "";

	zws_service_common_t service_common;

	if (service)
	{
		service_common = service->service_common;

		if (service_common)
		{
			error_text = service_common->error_text;
		}
	}

	return error_text;
}

zws_service_common_t zws_cgi_get_service_common(zws_service_t service)
{
	if (service) return service->service_common;

	return NULL;
}

void  zws_cgi_set_service_command_type(zws_service_t service, zws_cgi_command_type_t command_type)
{
	if (service)
	{
		service->command_type = command_type;
	}
}

void zws_cgi_set_service_next(zws_service_t service, void  *service_next)
{
	if (service)
	{
		service->service_next = service_next;
	}
}
