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
 * @file    zws_avi_networks_operations.c
 *
 * @brief   AVI messages for all Z-Wave network operations
 * @details Handles the AVI messages for all Z-Wave network operations.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-09-13
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message_networks.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zwu_number.h"

#define COMMAND_INCLUSION_NODE_ADD                   2    /**< ZWNET_OP_ADD_NODE */
#define COMMAND_INCLUSION_NODE_REMOVE                3    /**< ZWNET_OP_RM_NODE */
#define COMMAND_FAILED_NODE_REPLACE                  4    /**< ZWNET_OP_RP_NODE */
#define COMMAND_FAILED_NODE_REMOVE                   5    /**< ZWNET_OP_RM_FAILED_ID */
#define COMMAND_NODE_UPDATE                         13    /**< ZWNET_OP_NODE_UPDATE */
#define COMMAND_SEND_NIF                            14    /**< ZWNET_OP_SEND_NIF */
#define COMMAND_INCLUSION_NODE_ADD_ON_BEHALF        15    /**< ZWNET_OP_ADD_NODE_ON_BEHALF */
#define COMMAND_FAILED_NODE_REPLACE_ON_BEHALF       16    /**< ZWNET_OP_RP_NODE_ON_BEHALF */

 /**
 * @brief Get CGI response for network prekit dsk from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_networks_node_prekit_dsk_get_result(int web_api_version, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	char *dsk;

	if (zw_avro_field_get_string(arg_value, "dsk", &dsk) != ZWS_STATUS_OK)
	{
		goto l_err_get_fields;
	}

	if (zwu_string_buffer_append_format(result,
		"<%s dsk=\"%s\" />",
		"prekit_dsk",
		dsk
	) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	zw_avro_field_free_string(dsk);

	return ZWS_STATUS_OK;

l_err_append:
	zw_avro_field_free_string(dsk);
l_err_get_fields:
	return ZWS_STATUS_ERROR;
}


/**
* @brief Set Avro value for RPC request for Add node command using CGI request arguments
* @param[in]     args              CGI request arguments
* @param[in]     args_count        Count of CGI request arguments
* @param[in]     arg_value_p       Avro value for RPC request
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_networks_node_add_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	char *dsk_string;
	size_t dsk_string_size;

	return_status = ZWS_STATUS_OK;
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

		//if (zwu_uri_decode_string(name_string, name_string_size, name_string) != ZWS_STATUS_OK)
		//{
		//	goto l_err_decode_name;
		//}

		i++;

		return_status = zw_avro_field_set_string(arg_value_p, "dsk", dsk_string);

		ZWS_FREE(dsk_string);
	}

l_err_dsk_alloc:
	return return_status;
}

/**
* @brief Get CGI response for network security 2 request key from Avro value for RPC response
* @param[in]     web_api_version   Web API version
* @param[in]     arg_value         Avro value for RPC response
* @param[out]    result            CGI response string
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_networks_node_add_req_key_get_result(int web_api_version, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int32_t req_keys;
	int32_t req_csa;
	char	*csa_pin;

	if ((zw_avro_field_get_int(arg_value, "req_keys", &req_keys) != ZWS_STATUS_OK) ||
		(zw_avro_field_get_int(arg_value, "req_csa", &req_csa) != ZWS_STATUS_OK) ||
		(zw_avro_field_get_string(arg_value, "csa_pin", &csa_pin) != ZWS_STATUS_OK))
	{
		goto l_err_get_fields;
	}

	if (req_csa == 0)
	{
		if (zwu_string_buffer_append_format(result,
			"<%s req_key=\"%u\" />",
			"security",
			(uint8_t)req_keys
		) != ZWS_STATUS_OK)
		{
			goto l_err_append;
		}
	}
	else
	{
		if (zwu_string_buffer_append_format(result,
			"<%s req_key=\"%u\" csa_pin=\"%s\" />",
			"security",
			(uint8_t)req_keys,
			csa_pin
		) != ZWS_STATUS_OK)
		{
			goto l_err_append;
		}
	}
	zw_avro_field_free_string(csa_pin);

	return ZWS_STATUS_OK;

l_err_append:
	zw_avro_field_free_string(csa_pin);
l_err_get_fields :
	return ZWS_STATUS_ERROR;
}

/**
* @brief Set Avro value for RPC request for Add node set grant key command using CGI request arguments
* @param[in]     args              CGI request arguments
* @param[in]     args_count        Count of CGI request arguments
* @param[in]     arg_value_p       Avro value for RPC request
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_networks_node_add_grant_key_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int granted_keys;
	int grant_csa;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if ((i < args_count) && !strcmp(args[i][0], "granted_keys")
		&& (zwu_parse_int32_non_negative(args[i][1], &granted_keys) == ZWU_STATUS_OK)
		&& (zw_avro_field_set_int(arg_value_p, "granted_keys", granted_keys) == ZWS_STATUS_OK))
	{
		i++;
		return_status = ZWS_STATUS_OK;
	}

	//Optional
	if ((i < args_count) && !strcmp(args[i][0], "grant_csa")
		&& (zwu_parse_int32_non_negative(args[i][1], &grant_csa) == ZWU_STATUS_OK)
		&& (zw_avro_field_set_int(arg_value_p, "grant_csa", grant_csa) == ZWS_STATUS_OK))
	{
		i++;
		return_status = ZWS_STATUS_OK;
	}

	return return_status;
}

/**
* @brief Get CGI response for network security 2 dsk from Avro value for RPC response
* @param[in]     web_api_version   Web API version
* @param[in]     arg_value         Avro value for RPC response
* @param[out]    result            CGI response string
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_networks_node_add_dsk_get_result(int web_api_version, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int32_t pin_required;
	char *dsk;

	if ((zw_avro_field_get_int(arg_value, "pin_required", &pin_required) != ZWS_STATUS_OK) ||
		(zw_avro_field_get_string(arg_value, "dsk", &dsk) != ZWS_STATUS_OK))
	{
		goto l_err_get_fields;
	}

	if (zwu_string_buffer_append_format(result,
		"<%s pin_required=\"%u\" dsk=\"%s\" />",
		"security",
		pin_required,
		dsk
		) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	zw_avro_field_free_string(dsk);

	return ZWS_STATUS_OK;

l_err_append:
	zw_avro_field_free_string(dsk);
l_err_get_fields :
	return ZWS_STATUS_ERROR;
}

/**
* @brief Set Avro value for RPC request for Add node accept command using CGI request arguments
* @param[in]     args              CGI request arguments
* @param[in]     args_count        Count of CGI request arguments
* @param[in]     arg_value_p       Avro value for RPC request
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_networks_node_add_accept_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int32_t accept;
	char *dsk_string;
	size_t dsk_string_size;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if ((i < args_count) && !strcmp(args[i][0], "accept")
		&& (zwu_parse_int32_non_negative(args[i][1], &accept) == ZWU_STATUS_OK)
		&& (zw_avro_field_set_int(arg_value_p, "accept", accept) == ZWS_STATUS_OK))
	{
		i++;
		if ((i < args_count) && !strcmp(args[i][0], "value"))
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
	}

l_err_dsk_alloc:
	return return_status;
}

/**
 * @brief Set Avro value for RPC request for Failed Node Remove command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_networks_failed_node_remove_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int64_t combo_id;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if ((i < args_count) && !strcmp(args[i][0], "noded")
		&& (zwu_parse_int64_non_negative(args[i][1], &combo_id) == ZWU_STATUS_OK)
		&& (zw_avro_field_set_int(arg_value_p, "node_id", ZWS_CGI_GET_NODE_ID(combo_id)) == ZWS_STATUS_OK))
	{
		i++;
		return_status = ZWS_STATUS_OK;
	}

	return return_status;
}

/**
 * @brief Set Avro value for RPC request for Failed Node Replace command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_networks_failed_node_replace_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int64_t combo_id;
	char *dsk_string;
	size_t dsk_string_size;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if ((i < args_count) && !strcmp(args[i][0], "noded")
		&& (zwu_parse_int64_non_negative(args[i][1], &combo_id) == ZWU_STATUS_OK)
		&& (zw_avro_field_set_int(arg_value_p, "node_id", ZWS_CGI_GET_NODE_ID(combo_id)) == ZWS_STATUS_OK))
	{
		i++;
		return_status = ZWS_STATUS_OK;
	}

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

		//if (zwu_uri_decode_string(name_string, name_string_size, name_string) != ZWS_STATUS_OK)
		//{
		//	goto l_err_decode_name;
		//}

		i++;

		return_status = zw_avro_field_set_string(arg_value_p, "dsk", dsk_string);

		ZWS_FREE(dsk_string);
	}

l_err_dsk_alloc:
	return return_status;
}

/**
* @brief Set Avro value for RPC request for Failed Node Replace on-behalf command using CGI request arguments
* @param[in]     args              CGI request arguments
* @param[in]     args_count        Count of CGI request arguments
* @param[in]     arg_value_p       Avro value for RPC request
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_networks_failed_node_replace_on_behalf_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int64_t combo_id;
	char *dsk_string;
	size_t dsk_string_size;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	combo_id = 0;
	//Set node_id to be 0
	if (zw_avro_field_set_int(arg_value_p, "node_id", combo_id) == ZWS_STATUS_OK)
	{
		return_status = ZWS_STATUS_OK;
	}

	//Optional
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

		return_status = zw_avro_field_set_string(arg_value_p, "dsk", dsk_string);

		ZWS_FREE(dsk_string);
	}

l_err_dsk_alloc:
	return return_status;
}


/**
 * @brief Set Avro value for RPC request for Node Update command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_networks_node_update_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int64_t combo_id;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if ((i < args_count) && !strcmp(args[i][0], "noded")
			&& (zwu_parse_int64_non_negative(args[i][1], &combo_id) == ZWU_STATUS_OK)
			&& (zw_avro_field_set_int(arg_value_p, "node_id", ZWS_CGI_GET_NODE_ID(combo_id)) == ZWS_STATUS_OK))
	{
		i++;
		return_status = ZWS_STATUS_OK;
	}

	return return_status;
}

/**
 * @brief Set Avro value for RPC request for Send NIF (Node Information Frame) command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_networks_send_NIF_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int64_t combo_id;
	int node_id;
	int is_broadcast;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if ((i < args_count) && !strcmp(args[i][0], "noded")
			&& (zwu_parse_int64_non_negative(args[i][1], &combo_id) == ZWU_STATUS_OK))
	{
		node_id = ZWS_CGI_GET_NODE_ID(combo_id);

		is_broadcast = (node_id == 0) ? 1 : 0;

		if ((zw_avro_field_set_int(arg_value_p, "node_id", node_id) == ZWS_STATUS_OK)
				&& (zw_avro_field_set_int(arg_value_p, "is_broadcast", is_broadcast) == ZWS_STATUS_OK))
		{
			i++;
			return_status = ZWS_STATUS_OK;
		}
	}

	return return_status;
}

/**
* @brief Get CGI response for network initiate security 2 local node dsk from Avro value for RPC response
* @param[in]     web_api_version   Web API version
* @param[in]     arg_value         Avro value for RPC response
* @param[out]    result            CGI response string
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_networks_initiate_local_dsk_get_result(int web_api_version, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	//int32_t pin_required;
	char *dsk;

	if (zw_avro_field_get_string(arg_value, "local_dsk", &dsk) != ZWS_STATUS_OK)
	{
		goto l_err_get_fields;
	}

	if (zwu_string_buffer_append_format(result,
		"<%s local_node_dsk=\"%s\" />",
		"security",
		dsk
	) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	zw_avro_field_free_string(dsk);

	return ZWS_STATUS_OK;

l_err_append:
	zw_avro_field_free_string(dsk);
l_err_get_fields:
	return ZWS_STATUS_ERROR;
}

/** Handlers for prekit get dsk command */
static zws_avi_message_networks_t networks_node_prekit_dsk_get =
{
	{ "zwnet_add_prekit_get_dsk", NULL }, ZWS_CGI_NET_SUB_COMMAND_NONE,

	"network_node_prekit_dsk_get",

	NULL,
	zws_avi_networks_node_prekit_dsk_get_result,

	"tServNodePrekitDskGetFailed"
};

/** Handlers for Add Node command */
static zws_avi_message_networks_t networks_node_add =
{
		{"zwnet_add", "set_node_incl"}, COMMAND_INCLUSION_NODE_ADD,

		"network_node_add",

		zws_avi_networks_node_add_argument,
		NULL,

		"tServInclFailed"
};

/** Handlers for Add Node on-behalf command */
static zws_avi_message_networks_t networks_node_add_on_behalf =
{
		{ "zwnet_add", "set_node_incl" }, COMMAND_INCLUSION_NODE_ADD_ON_BEHALF,

		"network_node_add_on_behalf",

		//Use the same node add argument function
		zws_avi_networks_node_add_argument,
		NULL,

		"tServInclFailed"
};

/** Handlers for Add Node Security 2 get requested key command */
static zws_avi_message_networks_t networks_node_add_req_key_get =
{
		{"zwnet_add_s2_get_req_keys", NULL }, ZWS_CGI_NET_SUB_COMMAND_NONE,

		"network_node_add_req_key_get",
	
		NULL,
		zws_avi_networks_node_add_req_key_get_result,
	
		"tServNodeAddReqKeyGetFailed"
};

/** Handlers for Add Node Security 2 set grant key command */
static zws_avi_message_networks_t networks_node_add_grant_key_set =
{
		{"zwnet_add_s2_set_grant_keys", "NULL" }, ZWS_CGI_NET_SUB_COMMAND_NONE,

		"network_node_add_grant_key_set",

		zws_avi_networks_node_add_grant_key_argument,
		NULL,

		"tServNodeAddGrantKeySetFailed"
};

/** Handlers for Add Node Security 2 get dsk command */
static zws_avi_message_networks_t networks_node_add_dsk_get =
{
		{"zwnet_add_s2_get_dsk", NULL}, ZWS_CGI_NET_SUB_COMMAND_NONE,

		"network_node_add_dsk_get",

		NULL,
		zws_avi_networks_node_add_dsk_get_result,

		"tServNodeAddDskGetFailed"
};

/** Handlers for Add Node Security 2 set accept command */
static zws_avi_message_networks_t networks_node_add_accept_set =
{
		{"zwnet_add_s2_accept", "NULL" }, ZWS_CGI_NET_SUB_COMMAND_NONE,

		"network_node_add_accept",

		zws_avi_networks_node_add_accept_argument,
		NULL,

		"tServNodeAddAcceptSetFailed"
};

/** Handlers for Remove Node command */
static zws_avi_message_networks_t networks_node_remove =
{
		{"zwnet_add", "set_node_incl"}, COMMAND_INCLUSION_NODE_REMOVE,

		"network_node_remove",

		NULL,
		NULL,

		"tServInclFailed"
};

/** Handlers for Remove Failed Node command */
static zws_avi_message_networks_t networks_failed_node_remove =
{
		{"zwnet_fail", "set_node_fail"}, COMMAND_FAILED_NODE_REMOVE,

		"network_failed_node_remove",

		zws_avi_networks_failed_node_remove_argument,
		NULL,

		"tServRemRepFailed"
};

/** Handlers for Replace Failed Node command */
static zws_avi_message_networks_t networks_failed_node_replace =
{
		{"zwnet_fail", "set_node_fail"}, COMMAND_FAILED_NODE_REPLACE,

		"network_failed_node_replace",

		zws_avi_networks_failed_node_replace_argument,
		NULL,

		"tServRemRepFailed"
};

/** Handlers for Replace Failed Node on-behalf command */
static zws_avi_message_networks_t networks_failed_node_replace_on_behalf =
{
		{ "zwnet_fail", "set_node_fail" }, COMMAND_FAILED_NODE_REPLACE_ON_BEHALF,

		"network_failed_node_replace_on_behalf",

		zws_avi_networks_failed_node_replace_on_behalf_argument,
		NULL,

		"tServRemRepFailed"
};

/** Handlers for Node Update command */
static zws_avi_message_networks_t networks_node_update =
{
		{"zwnode_update", "node_update"}, ZWS_CGI_NET_SUB_COMMAND_NONE,

		"network_node_update",

		zws_avi_networks_node_update_argument,
		NULL,

		"tServNodeUpdateFailed"
};

/** Handlers for Node Update as sub command (deprecated) */
static zws_avi_message_networks_t networks_node_update_sub_command =
{
		{"zwnode_update", "node_update"}, COMMAND_NODE_UPDATE,

		"network_node_update",

		zws_avi_networks_node_update_argument,
		NULL,

		"tServNodeUpdateFailed"
};

/** Handlers for Send NIF command */
static zws_avi_message_networks_t networks_send_NIF =
{
		{"zwnet_send_nif", "send_nif"}, ZWS_CGI_NET_SUB_COMMAND_NONE,

		"network_send_NIF",

		zws_avi_networks_send_NIF_argument,
		NULL,

		"tServSendNifFailed"
};

/** Handlers for Send NIF as sub command (deprecated) */
static zws_avi_message_networks_t networks_send_NIF_sub_command =
{
		{"zwnet_send_nif", "send_nif"}, COMMAND_SEND_NIF,

		"network_send_NIF",

		zws_avi_networks_send_NIF_argument,
		NULL,

		"tServSendNifFailed"
};

/** Handlers for Network Update command */
static zws_avi_message_networks_t networks_update =
{
		{"zwnet_update", "nw_update"}, ZWS_CGI_NET_SUB_COMMAND_NONE,

		"network_update",

		NULL,
		NULL,

		"tServNwUpdateFailed"
};

/** Handlers for Network initiate command */
static zws_avi_message_networks_t networks_initiate =
{
		{"zwnet_initiate", "nw_initiate"}, ZWS_CGI_NET_SUB_COMMAND_NONE,

		"network_initiate",

		NULL,
		NULL,

		"tServInitFailed"
};

/** Handlers for Network initiate get local node dsk command */
static zws_avi_message_networks_t networks_initiate_local_dsk_get =
{
	{"zwnet_initiate_local_dsk_get", NULL}, ZWS_CGI_NET_SUB_COMMAND_NONE,

	"network_initiate_local_dsk_get",

	NULL,
	zws_avi_networks_initiate_local_dsk_get_result,

	"tServNwInitiateLocalDskGetFailed"
};

/** Handlers for Migrate Primary command */
static zws_avi_message_networks_t networks_migrate_primary =
{
		{"zwnet_migrate", "nw_migrate"}, ZWS_CGI_NET_SUB_COMMAND_NONE,

		"network_migrate_primary",

		NULL,
		NULL,

		"tServMigrateFailed"
};

/** Handlers for Network Reset command */
static zws_avi_message_networks_t networks_reset =
{
		{"zwnet_reset", "nw_reset"}, ZWS_CGI_NET_SUB_COMMAND_NONE,

		"network_reset",

		NULL,
		NULL,

		"tServNwReset"
};

/** Handlers for Network Operation Abort command */
static zws_avi_message_networks_t networks_operation_abort =
{
		{"zwnet_abort", "set_abort_op"}, ZWS_CGI_NET_SUB_COMMAND_NONE,

		"network_operation_abort",

		NULL,
		NULL,

		"tServAbortFailed"
};

/** Register command handlers */
zws_status_t zws_avi_networks_operations_init(zwu_array_list_t message_list)
{
	if ((zws_avi_message_list_add_message_networks(message_list, &networks_node_prekit_dsk_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_node_add) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_node_add_on_behalf) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_node_add_req_key_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_node_add_grant_key_set) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_node_add_dsk_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_node_add_accept_set) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_node_remove) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_failed_node_remove) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_failed_node_replace) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_failed_node_replace_on_behalf) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_node_update) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_node_update_sub_command) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_send_NIF) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_send_NIF_sub_command) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_update) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_initiate) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_initiate_local_dsk_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_migrate_primary) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_reset) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_networks(message_list, &networks_operation_abort) != ZWS_STATUS_OK))
	{
		goto l_err_networks_list_get;
	}

	return ZWS_STATUS_OK;

l_err_networks_list_get:
	return ZWS_STATUS_ERROR;
}



