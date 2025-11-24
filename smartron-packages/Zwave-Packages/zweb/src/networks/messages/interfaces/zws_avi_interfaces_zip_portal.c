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
 * @file    zws_avi_interfaces_zip_portal.c
 *
 * @brief   AVI message 'interface_zip_portal_xxx'
 * @details Handles the AVI message 'interface_zip_portal_xxx'.
 *
 * @author  Sasidhar
 *
 * @version 1.0 - 2013-11-07
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message_interfaces.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zwu_inet.h"
#include "zwu_encoding.h"

/** Z/IP Portal Gateway Configuration Get */
#define COMMAND_ZIP_PORTAL_GATEWAY_CONFIG_GET				1
/** Z/IP Portal Gateway Configuration Report */
#define COMMAND_ZIP_PORTAL_GATEWAY_CONFIG_REPORT			2
/** Z/IP Portal Gateway Configuration Set */
#define COMMAND_ZIP_PORTAL_GATEWAY_CONFIG_SET				3
/** Z/IP Portal Gateway Configuration Status Report */
#define COMMAND_ZIP_PORTAL_GATEWAY_CONFIG_STATUS_REPORT		4

/**
 * @brief Get CGI response for Z/IP portal configuration Get command from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_zip_portal_configuration_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int64_t utime;
	int lan_ipv6_prefix_len, portal_ipv6_prefix_len;
	void *lan_ipv6_address, *portal_ipv6_prefix, *default_gw_ipv6_addr, *pan_ipv6_prefix;
	size_t lan_ipv6_address_size, portal_ipv6_prefix_size, default_gw_ipv6_addr_size, pan_ipv6_prefix_size;

	char lan_ipv6_address_string[INET6_ADDRSTRLEN] = {0};
	char portal_ipv6_prefix_string[INET6_ADDRSTRLEN] = {0};
	char default_gw_ipv6_addr_string[INET6_ADDRSTRLEN] = {0};
	char pan_ipv6_prefix_string[INET6_ADDRSTRLEN] = {0};

	if (zw_avro_field_get_long(arg_value, "utime", &utime) != ZWS_STATUS_OK)
	{
		goto l_err_get_fields;
	}

	if (zw_avro_field_get_fixed(arg_value, "lan_ipv6_addr", &lan_ipv6_address, &lan_ipv6_address_size) != ZWS_STATUS_OK)
	{
		goto l_err_get_lan_ipv6_address_value;
	}

	if ((lan_ipv6_address_size != 16) || (zwu_inet_ntop(lan_ipv6_address, lan_ipv6_address_string, sizeof(lan_ipv6_address_string)) != ZWS_STATUS_OK))
	{
		goto l_err_lan_ipv6_addr_size;
	}

	if (zw_avro_field_get_int(arg_value, "lan_ipv6_prefix_len", &lan_ipv6_prefix_len) != ZWS_STATUS_OK)
	{
		goto l_err_get_lan_ipv6_prefix_len;
	}

	if (zw_avro_field_get_fixed(arg_value, "portal_ipv6_prefix", &portal_ipv6_prefix, &portal_ipv6_prefix_size) != ZWS_STATUS_OK)
	{
		goto l_err_get_portal_ipv6_address_value;
	}

	if ((portal_ipv6_prefix_size != 16) ||
		(zwu_inet_ntop(portal_ipv6_prefix, portal_ipv6_prefix_string, sizeof(portal_ipv6_prefix_string)) != ZWS_STATUS_OK))
	{
		goto l_err_portal_ipv6_addr_size;
	}

	if (zw_avro_field_get_int(arg_value, "portal_ipv6_prefix_len", &portal_ipv6_prefix_len) != ZWS_STATUS_OK)
	{
		goto l_err_get_portal_ipv6_prefix_len;
	}

	if (zw_avro_field_get_fixed(arg_value, "default_gw_ipv6_addr", &default_gw_ipv6_addr, &default_gw_ipv6_addr_size) != ZWS_STATUS_OK)
	{
		goto l_err_get_default_gw_ipv6_addr_value;
	}

	if ((default_gw_ipv6_addr_size != 16) ||
		(zwu_inet_ntop(default_gw_ipv6_addr, default_gw_ipv6_addr_string, sizeof(default_gw_ipv6_addr_string)) != ZWS_STATUS_OK)
	   )
	{
		goto l_err_default_gw_ipv6_addr_size;
	}

	if (zw_avro_field_get_fixed(arg_value, "pan_ipv6_prefix", &pan_ipv6_prefix, &pan_ipv6_prefix_size) != ZWS_STATUS_OK)
	{
		goto l_err_get_pan_ipv6_prefix_value;
	}

	if ((pan_ipv6_prefix_size != 16) ||
		(zwu_inet_ntop(pan_ipv6_prefix, pan_ipv6_prefix_string, sizeof(pan_ipv6_prefix_string)) != ZWS_STATUS_OK)
	   )
	{
		goto l_err_pan_ipv6_prefix_size;
	}
	if (zwu_string_buffer_append_format(result,
			"<gw_cfg utime=\"%llu\" lan_addr=\"%s\" lan_prefix_len=\"%u\" portal_prefix=\"%s\" portal_prefix_len=\"%u\" pan_prefix=\"%s\" default_gw_addr=\"%s\" />",
			(uint64_t)utime,
			lan_ipv6_address_string,
			(uint8_t)lan_ipv6_prefix_len,
			portal_ipv6_prefix_string,
			(uint8_t)portal_ipv6_prefix_len,
			pan_ipv6_prefix_string,
			default_gw_ipv6_addr_string
		    ) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	zw_avro_field_free_fixed(pan_ipv6_prefix);
	zw_avro_field_free_fixed(default_gw_ipv6_addr);
	zw_avro_field_free_fixed(portal_ipv6_prefix);
	zw_avro_field_free_fixed(lan_ipv6_address);

	return ZWS_STATUS_OK;

l_err_append:
l_err_pan_ipv6_prefix_size:
	zw_avro_field_free_fixed(pan_ipv6_prefix);
l_err_get_pan_ipv6_prefix_value:
l_err_default_gw_ipv6_addr_size:
	zw_avro_field_free_fixed(default_gw_ipv6_addr);
l_err_get_default_gw_ipv6_addr_value:
l_err_get_portal_ipv6_prefix_len:
l_err_portal_ipv6_addr_size:
	zw_avro_field_free_fixed(portal_ipv6_prefix);
l_err_get_portal_ipv6_address_value:
l_err_get_lan_ipv6_prefix_len:
l_err_lan_ipv6_addr_size:
	zw_avro_field_free_fixed(lan_ipv6_address);
l_err_get_lan_ipv6_address_value:
l_err_get_fields:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Set Avro value for RPC request for Z/IP portal configuration Set command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_zip_portal_configuration_set_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	char ipv6_addr_string[INET6_ADDRSTRLEN] = {0};
	uint8_t ipv6_addr[16] = {0};

	return_status = ZWS_STATUS_ERROR;
	i = 0;
	if ((i < args_count) && !strcmp(args[i][0], "lan_addr")
		 && (zwu_uri_decode_string(ipv6_addr_string, sizeof(ipv6_addr_string), args[i][1]) == ZWS_STATUS_OK))
	{
		i++;
		if ((zwu_inet_pton(ipv6_addr_string, ipv6_addr) == ZWS_STATUS_OK) &&
			(zw_avro_field_set_fixed(arg_value_p, "lan_ipv6_addr", ipv6_addr, 16) == ZWS_STATUS_OK)
		   )
		{
			if ((i < args_count) && !strcmp(args[i][0], "lan_prefix_len")
					 && (zw_avro_field_set_int(arg_value_p, "lan_ipv6_prefix_len", atoi(args[i][1])) == ZWS_STATUS_OK)
				)
			{
				i++;

				if ((i < args_count) && !strcmp(args[i][0], "portal_prefix")
						 && (zwu_uri_decode_string(ipv6_addr_string, sizeof(ipv6_addr_string), args[i][1]) == ZWS_STATUS_OK))
				{
					i++;
					if ((zwu_inet_pton(ipv6_addr_string, ipv6_addr) == ZWS_STATUS_OK) &&
						(zw_avro_field_set_fixed(arg_value_p, "portal_ipv6_prefix", ipv6_addr, 16) == ZWS_STATUS_OK)
					   )
					{
						if ((i < args_count) && !strcmp(args[i][0], "portal_prefix_len")
								 && (zw_avro_field_set_int(arg_value_p, "portal_ipv6_prefix_len", atoi(args[i][1])) == ZWS_STATUS_OK)
							)
						{
							i++;

							if ((i < args_count) && !strcmp(args[i][0], "pan_prefix")
								&& (zwu_uri_decode_string(ipv6_addr_string, sizeof(ipv6_addr_string), args[i][1]) == ZWS_STATUS_OK))
							{
								i++;
								if ((zwu_inet_pton(ipv6_addr_string, ipv6_addr) == ZWS_STATUS_OK) &&
									(zw_avro_field_set_fixed(arg_value_p, "pan_ipv6_prefix", ipv6_addr, 16) == ZWS_STATUS_OK)
								   )
								{
									if ((i < args_count) && !strcmp(args[i][0], "default_gw_addr")
										&& (zwu_uri_decode_string(ipv6_addr_string, sizeof(ipv6_addr_string), args[i][1]) == ZWS_STATUS_OK))
									{
										i++;
										if ((zwu_inet_pton(ipv6_addr_string, ipv6_addr) == ZWS_STATUS_OK) &&
											(zw_avro_field_set_fixed(arg_value_p, "default_gw_ipv6_addr", ipv6_addr, 16) == ZWS_STATUS_OK)
										   )
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
	}

	return return_status;
}


/**
 * @brief Get CGI response for Z/IP portal configuration status command from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_zip_portal_configuration_status_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int64_t utime, set_time;
	int status;

	if ((zw_avro_field_get_long(arg_value, "utime", &utime) != ZWS_STATUS_OK) ||
		(zw_avro_field_get_long(arg_value, "set_time", &set_time) != ZWS_STATUS_OK) ||
		(zw_avro_field_get_int(arg_value, "status", &status) != ZWS_STATUS_OK)
	   )
	{
		goto l_err_get_fields;
	}


	if (zwu_string_buffer_append_format(result,
			"<gw_cfg_sts set_time=\"%llu\" status_utime=\"%llu\" status=\"%u\" />",
			(uint64_t)set_time,
			(uint64_t)utime,
			(uint8_t)status
		    ) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	return ZWS_STATUS_OK;

l_err_append:
l_err_get_fields:
	return ZWS_STATUS_ERROR;
}

/** Handlers for Z/IP Portal Configuration Get Command */
static zws_avi_message_interfaces_t interfaces_zip_portal_configuration_get =
{
		{"zwif_gw_cfg", NULL}, COMMAND_ZIP_PORTAL_GATEWAY_CONFIG_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_NOT_SUPPORTED,

		"interface_zip_portal_configuration_get",

		NULL,
		zws_avi_interfaces_zip_portal_configuration_get_result,

		"tServPortal"
};

/** Handlers for Z/IP Portal Configuration Set Command */
static zws_avi_message_interfaces_t interfaces_zip_portal_configuration_set =
{
		{"zwif_gw_cfg", NULL}, COMMAND_ZIP_PORTAL_GATEWAY_CONFIG_SET,
		ZWS_AVI_MESSAGE_PASSIVE_NOT_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_NOT_SUPPORTED,

		"interface_zip_portal_configuration_set",

		zws_avi_interfaces_zip_portal_configuration_set_argument,
		NULL,

		"tServPortal"
};

/** Handlers for Z/IP Portal Status Command */
static zws_avi_message_interfaces_t interfaces_zip_portal_configuration_status =
{
		{"zwif_gw_cfg", NULL}, COMMAND_ZIP_PORTAL_GATEWAY_CONFIG_STATUS_REPORT,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_NOT_SUPPORTED,

		"interface_zip_portal_configuration_status",

		NULL,
		zws_avi_interfaces_zip_portal_configuration_status_result,

		"tServPortal"
};

/** Register command handlers */
zws_status_t zws_avi_interfaces_zip_portal_init(zwu_array_list_t message_list)
{
	if ((zws_avi_message_list_add_message_interfaces(message_list, &interfaces_zip_portal_configuration_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_zip_portal_configuration_set) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_zip_portal_configuration_status) != ZWS_STATUS_OK)
		)
	{
		goto l_err_networks_list_get;
	}

	return ZWS_STATUS_OK;

l_err_networks_list_get:
	return ZWS_STATUS_ERROR;
}


