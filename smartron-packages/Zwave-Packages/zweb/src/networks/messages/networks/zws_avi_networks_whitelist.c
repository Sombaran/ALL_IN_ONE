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
 * @file    zws_avi_networks_whitelist.c
 *
 * @brief   AVI message 'network_whitelist'
 * @details Handles the AVI message 'network_whitelist'.
 *
 * @author  Angela Yan
 *
 * @version 1.0 - 2016-12-08
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message_networks.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zwu_number.h"
#include "zwu_encoding.h"

/**
 * @brief Get CGI response for network information from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_networks_whitelist_list_report_result(int web_api_version, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	avro_value_t whitelist_list_value;
	avro_value_t whitelist_value;
	size_t whitelist_count;
	size_t size;
	const char *strdsk = NULL;
	zwu_string_buffer_t whitelist_list;
	int i;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);

	if (avro_value_get_by_name(arg_value, "whitelist_list", &whitelist_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'whitelist_list': %s", avro_strerror());
		goto l_err_get_whitelist_list_value;
	}

	if (avro_value_get_size(&whitelist_list_value, &whitelist_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get whitelist list size");
		goto l_err_whitelist_list_size;
	}

	if (zwu_string_buffer_create(0, &whitelist_list) != ZWS_STATUS_OK)
	{
		goto l_err_whitelist_list_create;
	}
	ZWS_LOG(ZWS_LOG_DEBUG, "whitelist_count = %d", whitelist_count);

	for (i = 0; i < whitelist_count; i++)
	{
		if (avro_value_get_by_index(&whitelist_list_value, i, &whitelist_value, NULL) != 0)
		{
			goto l_err_get_whitelist_value;
		}

		if (avro_value_get_string(&whitelist_value, &strdsk, &size) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get dsk: %s", avro_strerror());
			goto l_err_get_dsk;
		}

		ZWS_LOG(ZWS_LOG_DEBUG, "strdsk = %s, size = %d", strdsk, size);

		if (zwu_string_buffer_append_format(whitelist_list, "%s,", strdsk) != ZWS_STATUS_OK)
		{
			goto l_err_dsk_append;
		}

		//zw_avro_field_free_string((char*)strdsk);
	}

	if (zwu_string_buffer_append_format(result,
		"<%s  dsk_list=\"%s\" />",
		"whitelist_list",
		zwu_string_buffer_get_string(whitelist_list)
	) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	zwu_string_buffer_destroy(whitelist_list);

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, ZWS_STATUS_OK);

	return ZWS_STATUS_OK;

l_err_append:
l_err_dsk_append:
l_err_get_dsk:
l_err_get_whitelist_value:
	zwu_string_buffer_destroy(whitelist_list);
l_err_whitelist_list_create:
l_err_whitelist_list_size:
l_err_get_whitelist_list_value:

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, ZWS_STATUS_ERROR);

	return ZWS_STATUS_ERROR;
}

/**
* @brief Set Avro value for RPC request for whitelist device info command using CGI request arguments
* @param[in]     args              CGI request arguments
* @param[in]     args_count        Count of CGI request arguments
* @param[in]     arg_value_p       Avro value for RPC request
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_networks_whitelist_device_info_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	char *dsk_string;
	size_t dsk_string_size;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if ((i < args_count) && !strcmp(args[i][0], "dsk"))
	{
		dsk_string_size = strlen(args[i][1]) + 1;

		if ((dsk_string = (char *)ZWS_MALLOC(dsk_string_size)) == NULL)
		{
			ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
			goto l_err_dsk_alloc;
		}

		strncpy(dsk_string, args[i][1], dsk_string_size);

		i++;

		ZWS_LOG(ZWS_LOG_DEBUG, "dsk_string = %s", dsk_string);

		if (zw_avro_field_set_string(arg_value_p, "dsk", dsk_string) == ZWS_STATUS_OK)
		{
			return_status = ZWS_STATUS_OK;
		}
		ZWS_FREE(dsk_string);
	}
	
l_err_dsk_alloc:
	return return_status;
}

/**
* @brief Get CGI response for whitelist device info from Avro value for RPC response
* @param[in]     web_api_version   Web API version
* @param[in]     arg_value         Avro value for RPC response
* @param[out]    result            CGI response string
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_networks_whitelist_device_info_result(int web_api_version, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int32_t granted_keys;
	char *dsk = NULL;;
	char *name = NULL;
	char *location = NULL;
	char *name_encoded;
	char *location_encoded;
	size_t string_size;

	if ((zw_avro_field_get_string(arg_value, "dsk", &dsk) != ZWS_STATUS_OK) ||
		(zw_avro_field_get_int(arg_value, "granted_keys", &granted_keys) != ZWS_STATUS_OK) ||
		(zw_avro_field_get_string(arg_value, "name", &name) != ZWS_STATUS_OK) ||
		(zw_avro_field_get_string(arg_value, "loc", &location) != ZWS_STATUS_OK))
	{
		goto l_err_get_fields;
	}

	string_size = (strlen(name) * 3) + 1;
	if ((name_encoded = (char *)ZWS_MALLOC(string_size)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_name_encoded_alloc;
	}

	if (zwu_uri_encode_string(name_encoded, string_size, name) != ZWS_STATUS_OK)
	{
		goto l_err_encode_name;
	}

	string_size = (strlen(location) * 3) + 1;
	if ((location_encoded = (char *)ZWS_MALLOC(string_size)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_location_encoded_alloc;
	}

	if (zwu_uri_encode_string(location_encoded, string_size, location) != ZWS_STATUS_OK)
	{
		goto l_err_encode_location;
	}

	if (zwu_string_buffer_append_format(result,
		"<%s dsk=\"%s\" granted_keys=\"%u\" name=\"%s\" loc=\"%s\" />",
		"whitelist_info", dsk,
		granted_keys,
		name_encoded, location_encoded
	) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	ZWS_FREE(location_encoded);
	ZWS_FREE(name_encoded);

	zw_avro_field_free_string(dsk);
	zw_avro_field_free_string(location);
	zw_avro_field_free_string(name);

	return ZWS_STATUS_OK;

l_err_append:
l_err_encode_location:
	ZWS_FREE(location_encoded);
l_err_location_encoded_alloc:
l_err_encode_name:
	ZWS_FREE(name_encoded);
l_err_name_encoded_alloc:
l_err_get_fields:
	if(dsk)
		zw_avro_field_free_string(dsk);
	if (name)
		zw_avro_field_free_string(name);
	if (location)
		zw_avro_field_free_string(location);
	return ZWS_STATUS_ERROR;
}

/**
* @brief Set Avro value for RPC request for whitelist add command using CGI request arguments
* @param[in]     args              CGI request arguments
* @param[in]     args_count        Count of CGI request arguments
* @param[in]     arg_value_p       Avro value for RPC request
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_networks_whitelist_add_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int granted_keys;
	char *dsk_string = NULL;
	size_t dsk_string_size;
	char *name_string = NULL;
	size_t name_string_size;
	char *location_string = NULL;
	size_t location_string_size;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	dsk_string = NULL;

	if ((i < args_count) && !strcmp(args[i][0], "dsk"))
	{
		dsk_string_size = strlen(args[i][1]) + 1;

		if ((dsk_string = (char *)ZWS_MALLOC(dsk_string_size)) == NULL)
		{
			ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
			return_status = ZWS_STATUS_ENOMEM;
			goto l_err_dsk_alloc;
		}

		strncpy(dsk_string, args[i][1], dsk_string_size);

		i++;
		if (zw_avro_field_set_string(arg_value_p, "dsk", dsk_string) != ZWS_STATUS_OK)
		{
			goto l_err_set_dsk_avro;
		}

		ZWS_FREE(dsk_string);
		dsk_string = NULL;
	}

	//Optional
	if ((i < args_count) && !strcmp(args[i][0], "granted_keys")
		&& (zwu_parse_int32_non_negative(args[i][1], &granted_keys) == ZWU_STATUS_OK)
		&& (zw_avro_field_set_int(arg_value_p, "granted_keys", granted_keys) == ZWS_STATUS_OK))
	{
		i++;
	}

	if ((i < args_count) && !strcmp(args[i][0], "name"))
	{
		name_string_size = strlen(args[i][1]) + 1;

		if ((name_string = (char *)ZWS_MALLOC(name_string_size)) == NULL)
		{
			ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
			goto l_err_name_alloc;
		}

		strncpy(name_string, args[i][1], name_string_size);

		if (zwu_uri_decode_string(name_string, name_string_size, name_string) != ZWS_STATUS_OK)
		{
			goto l_err_decode_name;
		}

		i++;

		if (zw_avro_field_set_string(arg_value_p, "name", name_string) != ZWS_STATUS_OK)
		{
			return_status = ZWS_STATUS_ERROR;
			goto l_err_set_name_avro;
		}
		ZWS_LOG(ZWS_LOG_DEBUG, "name_string = %s", name_string);

		ZWS_FREE(name_string);
		name_string = NULL;
	}

	if ((i < args_count) && !strcmp(args[i][0], "loc"))
	{
		location_string_size = strlen(args[i][1]) + 1;

		if ((location_string = (char *)ZWS_MALLOC(location_string_size)) == NULL)
		{
			ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
			goto l_err_location_alloc;
		}

		strncpy(location_string, args[i][1], location_string_size);

		if (zwu_uri_decode_string(location_string, location_string_size, location_string) != ZWS_STATUS_OK)
		{
			goto l_err_decode_location;
		}

		i++;

		if (zw_avro_field_set_string(arg_value_p, "location", location_string) != ZWS_STATUS_OK)
		{
			return_status = ZWS_STATUS_ERROR;
			goto l_err_set_location_avro;
		}

		ZWS_LOG(ZWS_LOG_DEBUG, "location_string = %s", location_string);

		ZWS_FREE(location_string);
		location_string = NULL;
	}

	return ZWS_STATUS_OK;

l_err_set_location_avro:
l_err_decode_location:
	l_err_set_name_avro:
l_err_location_alloc:
l_err_decode_name:
l_err_name_alloc:
l_err_set_dsk_avro:
	if(name_string)  ZWS_FREE(name_string);
	if (location_string)  ZWS_FREE(location_string);
	if(dsk_string)  ZWS_FREE(dsk_string);
l_err_dsk_alloc:
	return return_status;
}

/**
* @brief Set Avro value for RPC request for whitelist remove command using CGI request arguments
* @param[in]     args              CGI request arguments
* @param[in]     args_count        Count of CGI request arguments
* @param[in]     arg_value_p       Avro value for RPC request
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_networks_whitelist_remove_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	char *dsk_string;
	size_t dsk_string_size;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if ((i < args_count) && !strcmp(args[i][0], "dsk"))
	{
		dsk_string_size = strlen(args[i][1]) + 1;

		if ((dsk_string = (char *)ZWS_MALLOC(dsk_string_size)) == NULL)
		{
			ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
			goto l_err_dsk_alloc;
		}

		strncpy(dsk_string, args[i][1], dsk_string_size);

		i++;

		if (zw_avro_field_set_string(arg_value_p, "dsk", dsk_string) == ZWS_STATUS_OK)
		{
			return_status = ZWS_STATUS_OK;
		}
		ZWS_FREE(dsk_string);
	}

l_err_dsk_alloc:
	return return_status;
}


/** Handlers for whitelist list get command */
static zws_avi_message_networks_t networks_whitelist_list_get =
{
	{"zwnet_whitelist_list_get", NULL}, ZWS_CGI_NET_SUB_COMMAND_NONE,

	"network_whitelist_list_get",

	NULL,
	NULL,

	"tServWhitelistListGetFailed"
};

/** Handlers for whitelist list report command */
static zws_avi_message_networks_t networks_whitelist_list_report =
{
	{ "zwnet_whitelist_list_report", NULL }, ZWS_CGI_NET_SUB_COMMAND_NONE,

	"network_whitelist_list_report",

	NULL,
	zws_avi_networks_whitelist_list_report_result,

	"tServWhitelistListReportFailed"
};

/** Handlers for whitelist device info command */
static zws_avi_message_networks_t networks_whitelist_device_info =
{
	{ "zwnet_whitelist_device_info", NULL }, ZWS_CGI_NET_SUB_COMMAND_NONE,

	"network_whitelist_device_info",

	zws_avi_networks_whitelist_device_info_argument,
	zws_avi_networks_whitelist_device_info_result,

	"tServWhitelistDeviceInfoFailed"
};

/** Handlers for whitelist add command */
static zws_avi_message_networks_t networks_whitelist_add =
{
	{ "zwnet_whitelist_add", NULL }, ZWS_CGI_NET_SUB_COMMAND_NONE,

	"network_whitelist_add",

	zws_avi_networks_whitelist_add_argument,
	NULL,

	"tServWhitelistAddFailed"
};

/** Handlers for whitelist remove command */
static zws_avi_message_networks_t networks_whitelist_remove =
{
	{ "zwnet_whitelist_remove", NULL }, ZWS_CGI_NET_SUB_COMMAND_NONE,

	"network_whitelist_remove",

	zws_avi_networks_whitelist_remove_argument,
	NULL,

	"tServWhitelistRemoveFailed"
};

/** Handlers for whitelist remove all command */
static zws_avi_message_networks_t networks_whitelist_remove_all =
{
	{ "zwnet_whitelist_remove_all", NULL }, ZWS_CGI_NET_SUB_COMMAND_NONE,

	"network_whitelist_remove",

	NULL,
	NULL,

	"tServWhitelistRemoveAllFailed"
};

/** Register command handlers */
zws_status_t zws_avi_networks_whitelist_init(zwu_array_list_t message_list)
{
	if ((zws_avi_message_list_add_message_networks(message_list, &networks_whitelist_list_get) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_networks(message_list, &networks_whitelist_list_report) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_networks(message_list, &networks_whitelist_device_info) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_networks(message_list, &networks_whitelist_add) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_networks(message_list, &networks_whitelist_remove) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message_networks(message_list, &networks_whitelist_remove_all) != ZWS_STATUS_OK) )
	{
		goto l_err_networks_whitelist;
	}

	return ZWS_STATUS_OK;

l_err_networks_whitelist:
	return ZWS_STATUS_ERROR;
}
