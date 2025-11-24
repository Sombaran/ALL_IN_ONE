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
 * @file    zws_avi_interfaces_zip_gateway.c
 *
 * @brief   AVI message 'interface_zip_gateway_xxx'
 * @details Handles the AVI message 'interface_zip_gateway_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-10-04
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message_interfaces.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zwu_number.h"
#include "zwu_inet.h"
#include "zwu_encoding.h"

/** ZIP Gateway Mode Get */
#define COMMAND_ZIP_GATEWAY_MODE_GET                         1
/** ZIP Gateway Mode Report */
#define COMMAND_ZIP_GATEWAY_MODE_REPORT                      2
/** ZIP Gateway Mode Set */
#define COMMAND_ZIP_GATEWAY_MODE_SET                         3
/** ZIP Gateway Lock Set */
#define COMMAND_ZIP_GATEWAY_LOCK_SET                         4
/** ZIP Gateway Unsolicited Destination Get */
#define COMMAND_ZIP_GATEWAY_UNSOLICITED_DESTINATION_GET      5
/** ZIP Gateway Unsolicited Destination Report */
#define COMMAND_ZIP_GATEWAY_UNSOLICITED_DESTINATION_REPORT   6
/** ZIP Gateway Unsolicited Destination Set */
#define COMMAND_ZIP_GATEWAY_UNSOLICITED_DESTINATION_SET      7

/** ZIP Gateway peer name maximum length  */
#define GATEWAY_PEER_NAME_LEN_MAX 64
/** ZIP Gateway mode - standalone  */
#define STANDALONE_MODE 0x1
/** ZIP Gateway mode - portal  */
#define PORTAL_MODE		0x2

/**
 * @brief Get CGI response for Mode Get command from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_zip_gateway_mode_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int64_t utime;
	int32_t status, mode, port, name_len, profile_present;
	void *ipv6_address, *peer_name;
	size_t ipv6_address_size, peer_name_size;
	char peer_addr_ipv6_string[INET6_ADDRSTRLEN]={0};
	char peer_name_str[GATEWAY_PEER_NAME_LEN_MAX] = {0};
	char peer_name_encode_str[GATEWAY_PEER_NAME_LEN_MAX * 3 + 1]={0};


	if ((zw_avro_field_get_long(arg_value, "utime", &utime) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "status", &status) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "mode", &mode) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "profile_present", &profile_present) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "port", &port) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "peer_name_length", &name_len) != ZWS_STATUS_OK)
		)
	{
		goto l_err_get_fields;
	}

	if (zw_avro_field_get_fixed(arg_value, "ipv6_address", &ipv6_address, &ipv6_address_size) != ZWS_STATUS_OK)
	{
		goto l_err_get_ipv6_address_value;
	}


	if (zwu_inet_ntop(ipv6_address, peer_addr_ipv6_string, sizeof(peer_addr_ipv6_string)) != ZWS_STATUS_OK)
	{
		goto l_err_local_ipv6_address_string;
	}

	if (zw_avro_field_get_fixed(arg_value, "peer_name", &peer_name, &peer_name_size) != ZWS_STATUS_OK)
	{
		goto l_err_get_peer_name_value;
	}

	if((peer_name_size != 63) || name_len > 63 )
	{
		goto l_err_get_peer_name_len;
	}

	memcpy(peer_name_str, peer_name, name_len);

	zwu_uri_encode_string(peer_name_encode_str, sizeof(peer_name_encode_str), peer_name_str);

	if (zwu_string_buffer_append_format(result,
			"<gw_mode utime=\"%llu\" status=\"%u\" mode=\"%u\" profile_present=\"%u\" peer_addr=\"%s\" peer_name=\"%s\" peer_port=\"%u\" />",
			(uint64_t)utime,
			(uint8_t)status,
			(uint8_t)mode,
			profile_present,
			peer_addr_ipv6_string,
			peer_name_str,
			(uint16_t)port
			) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	zw_avro_field_free_fixed(ipv6_address);
	zw_avro_field_free_fixed(peer_name);

	return ZWS_STATUS_OK;

l_err_append:
l_err_get_peer_name_len:
	zw_avro_field_free_fixed(peer_name);
l_err_get_peer_name_value:
l_err_local_ipv6_address_string:
	zw_avro_field_free_fixed(ipv6_address);
l_err_get_ipv6_address_value:
l_err_get_fields:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Set Avro value for RPC request for Mode Set command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_zip_gateway_mode_set_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i, mode = 0;
	char peer_addr_ipv6_string[INET6_ADDRSTRLEN];
	uint8_t peer_addr_ipv6[16];

	char peer_name_string[GATEWAY_PEER_NAME_LEN_MAX]={0};
	uint8_t name_len = 0;


	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if ((i < args_count) && !strcmp(args[i][0], "mode")
			&& (zw_avro_field_set_int(arg_value_p, "mode", atoi(args[i][1])) == ZWS_STATUS_OK))
	{
		mode = atoi(args[i][1]);
		i++;

		if(mode == STANDALONE_MODE)
		{
			return_status = ZWS_STATUS_OK;
		}
		else if(mode == PORTAL_MODE)
		{

			if ((i < args_count) && !strcmp(args[i][0], "peer_addr")
						&& (zwu_uri_decode_string(peer_addr_ipv6_string, sizeof(peer_addr_ipv6_string), args[i][1]) == ZWS_STATUS_OK))
			{
				i++;
				if (zwu_inet_pton(peer_addr_ipv6_string, peer_addr_ipv6) == ZWS_STATUS_OK)
				{
					if (zw_avro_field_set_fixed(arg_value_p, "ipv6_address", peer_addr_ipv6, 16) == ZWS_STATUS_OK)
					{
						if ((i < args_count) && !strcmp(args[i][0], "peer_name")
							 && (zwu_uri_decode_string(peer_name_string, (sizeof(peer_name_string)-1), args[i][1]) == ZWS_STATUS_OK))
						{
							i++;
							if((name_len = strlen(peer_name_string)) < GATEWAY_PEER_NAME_LEN_MAX)
							{
								if ((zw_avro_field_set_fixed(arg_value_p, "peer_name", peer_name_string, 63) == ZWS_STATUS_OK)  &&
									(zw_avro_field_set_int(arg_value_p, "peer_name_length", name_len) == ZWS_STATUS_OK)
								   )
								{
									if ((i < args_count) && !strcmp(args[i][0], "peer_port")
												&& (zw_avro_field_set_int(arg_value_p, "port", atoi(args[i][1])) == ZWS_STATUS_OK))
									{
											i++;
											return_status = ZWS_STATUS_OK;
									}
								}
							}
						}

					}
				}
			}
		}

	}

	return return_status;
}

/**
 * @brief Set Avro value for RPC request for Lock Set command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_zip_gateway_lock_set_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;


	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if ((i < args_count) && !strcmp(args[i][0], "lock")
			&& (zw_avro_field_set_int(arg_value_p, "lock", atoi(args[i][1])) == ZWS_STATUS_OK))
	{
		i++;
		if ((i < args_count) && !strcmp(args[i][0], "show")
					&& (zw_avro_field_set_int(arg_value_p, "show", atoi(args[i][1])) == ZWS_STATUS_OK))
		{
			i++;
			return_status = ZWS_STATUS_OK;
		}
	}

	return return_status;
}

/**
 * @brief Get CGI response for Unsolicited Destination Get command from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_zip_gateway_unsolicited_destination_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int64_t utime;
	void *unsolicited_ipv6_destination;
	size_t unsolicited_ipv6_destination_size;
	int32_t unsolicited_destination_port;
	void *local_ipv6_address;
	size_t local_ipv6_address_size;
	int32_t local_destination_port;
	char unsolicited_ipv6_destination_string[INET6_ADDRSTRLEN];
	char local_ipv6_address_string[INET6_ADDRSTRLEN];

	if ((zw_avro_field_get_long(arg_value, "utime", &utime) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "unsolicited_destination_port", &unsolicited_destination_port) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "local_destination_port", &local_destination_port) != ZWS_STATUS_OK)
			)
	{
		goto l_err_get_fields;
	}

	if (zw_avro_field_get_fixed(arg_value, "unsolicited_ipv6_destination", &unsolicited_ipv6_destination, &unsolicited_ipv6_destination_size) != ZWS_STATUS_OK)
	{
		goto l_err_get_field_unsolicited_ipv6_address_value;
	}

	// Size validation to prevent underflow/overlfow
	if (unsolicited_ipv6_destination_size != 16)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Invalid unsolicited ipv6 destination value field size");
		goto l_err_invalid_unsol_dest_size;
	}

	if (zw_avro_field_get_fixed(arg_value, "local_ipv6_address", &local_ipv6_address, &local_ipv6_address_size) != ZWS_STATUS_OK)
	{
		goto l_err_get_field_local_ipv6_address_value;
	}

	// Size validation to prevent underflow/overlfow
	if (local_ipv6_address_size != 16)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Invalid local address value field size");
		goto l_err_invalid_local_address_size;
	}

	if (zwu_inet_ntop(unsolicited_ipv6_destination, unsolicited_ipv6_destination_string, sizeof(unsolicited_ipv6_destination_string)) != ZWS_STATUS_OK)
	{
		goto l_err_unsolicited_ipv6_address_string;
	}

	if (zwu_inet_ntop(local_ipv6_address, local_ipv6_address_string, sizeof(local_ipv6_address_string)) != ZWS_STATUS_OK)
	{
		goto l_err_local_ipv6_address_string;
	}

	if (zwu_string_buffer_append_format(result,
			"<gw_unsol_dest utime=\"%llu\" unsol_dest_addr=\"%s\" unsol_dest_port=\"%u\" local_addr=\"%s\" local_port=\"%u\" />",
			(uint64_t)utime,
			unsolicited_ipv6_destination_string,
			(uint16_t)unsolicited_destination_port,
			local_ipv6_address_string,
			(uint16_t)local_destination_port
			) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	zw_avro_field_free_fixed(local_ipv6_address);
	zw_avro_field_free_fixed(unsolicited_ipv6_destination);

	return ZWS_STATUS_OK;

l_err_append:
l_err_local_ipv6_address_string:
l_err_unsolicited_ipv6_address_string:
l_err_invalid_local_address_size:
	zw_avro_field_free_fixed(local_ipv6_address);
l_err_get_field_local_ipv6_address_value:
l_err_invalid_unsol_dest_size:
	zw_avro_field_free_fixed(unsolicited_ipv6_destination);
l_err_get_field_unsolicited_ipv6_address_value:
l_err_get_fields:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Set Avro value for RPC request for Unsolicited Destination Set command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_zip_gateway_unsolicited_destination_set_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	char unsolicited_ipv6_destination_string[INET6_ADDRSTRLEN];
	uint8_t unsolicited_ipv6_destination[16];
	int unsolicited_destination_port;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if ((i < args_count) && !strcmp(args[i][0], "unsol_dest_addr")
			&& (zwu_uri_decode_string(unsolicited_ipv6_destination_string, sizeof(unsolicited_ipv6_destination_string), args[i][1]) == ZWS_STATUS_OK))
	{
		i++;
		if (zwu_inet_pton(unsolicited_ipv6_destination_string, unsolicited_ipv6_destination) == ZWS_STATUS_OK)
		{
			if (zw_avro_field_set_fixed(arg_value_p, "unsolicited_ipv6_destination", unsolicited_ipv6_destination, 16) == ZWS_STATUS_OK)
			{
				if ((i < args_count) && !strcmp(args[i][0], "unsol_dest_port")
						&& (zwu_parse_int32_non_negative(args[i][1], &unsolicited_destination_port) == ZWU_STATUS_OK)
						&& (zw_avro_field_set_int(arg_value_p, "unsolicited_destination_port", unsolicited_destination_port) == ZWS_STATUS_OK))
				{
					i++;
					return_status = ZWS_STATUS_OK;
				}
			}
		}
	}

	return return_status;
}

/** Handlers for Get command */
static zws_avi_message_interfaces_t interfaces_zip_gateway_mode_get =
{
		{"zwif_gw", NULL}, COMMAND_ZIP_GATEWAY_MODE_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_NOT_SUPPORTED,

		"interface_zip_gateway_mode_get",

		NULL,
		zws_avi_interfaces_zip_gateway_mode_get_result,

		"tServGateway"
};

/** Handlers for Set command */
static zws_avi_message_interfaces_t interfaces_zip_gateway_mode_set =
{
		{"zwif_gw", NULL}, COMMAND_ZIP_GATEWAY_MODE_SET,
		ZWS_AVI_MESSAGE_PASSIVE_NOT_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_NOT_SUPPORTED,

		"interface_zip_gateway_mode_set",

		zws_avi_interfaces_zip_gateway_mode_set_argument,
		NULL,

		"tServGateway"
};

/** Handlers for Lock Set command */
static zws_avi_message_interfaces_t interfaces_zip_gateway_lock_set =
{
		{"zwif_gw", NULL}, COMMAND_ZIP_GATEWAY_LOCK_SET,
		ZWS_AVI_MESSAGE_PASSIVE_NOT_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_NOT_SUPPORTED,

		"interface_zip_gateway_lock_set",

		zws_avi_interfaces_zip_gateway_lock_set_argument,
		NULL,

		"tServGateway"
};

/** Handlers for Unsolicited Destination Get command */
static zws_avi_message_interfaces_t interfaces_zip_gateway_unsolicited_destination_get =
{
		{"zwif_gw", NULL}, COMMAND_ZIP_GATEWAY_UNSOLICITED_DESTINATION_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_NOT_SUPPORTED,

		"interface_zip_gateway_unsolicited_destination_get",

		NULL,
		zws_avi_interfaces_zip_gateway_unsolicited_destination_get_result,

		"tServGateway"
};

/** Handlers for Unsolicited Destination Set command */
static zws_avi_message_interfaces_t interfaces_zip_gateway_unsolicited_destination_set =
{
		{"zwif_gw", NULL}, COMMAND_ZIP_GATEWAY_UNSOLICITED_DESTINATION_SET,
		ZWS_AVI_MESSAGE_PASSIVE_NOT_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_NOT_SUPPORTED,

		"interface_zip_gateway_unsolicited_destination_set",

		zws_avi_interfaces_zip_gateway_unsolicited_destination_set_argument,
		NULL,

		"tServGateway"
};

/** Register command handlers */
zws_status_t zws_avi_interfaces_zip_gateway_init(zwu_array_list_t message_list)
{
	if ((zws_avi_message_list_add_message_interfaces(message_list, &interfaces_zip_gateway_mode_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_zip_gateway_mode_set) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_zip_gateway_lock_set) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_zip_gateway_unsolicited_destination_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_zip_gateway_unsolicited_destination_set) != ZWS_STATUS_OK))
	{
		goto l_err_networks_list_get;
	}

	return ZWS_STATUS_OK;

l_err_networks_list_get:
	return ZWS_STATUS_ERROR;
}


