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
 * @file    zws_avi_root_gateway_initialize.c
 *
 * @brief   AVI message 'root_gateway_set/get/list'
 * @details Handles the AVI message 'root_gateway_set/get/list'.
 *
 * @author  Sasidhar
 *
 * @version 1.0 - 2014-11-11
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zwu_inet.h"
#include "zwu_number.h"
#include "zwu_encoding.h"

/** Max MDNS string length */
#define GATEWAY_NAME_ONLY_MAX	 64

/** Max IPV6 address length */
#define GATEWAY_IP_ADDRESS_ONLY_MAX 48

/** Max gateway name length (MDNS string  + IPv6 string) */
#define GATEWAY_NAME_LEN_MAX	 128

/**
 * @brief Set Avro value for RPC request for gateway set command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_root_gateway_set_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	char ipv6_addr_string[GATEWAY_NAME_LEN_MAX * 3 + 1]={0};
	uint8_t ipv6_addr[16] = {0};
	uint8_t iptype = 1;
	int host_port=0;
	int key_len = 0;
	avro_value_t preshared_key;
	avro_value_t preshared_key_value;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if ((i < args_count) && !strcmp(args[i][0], "zip_gw_name")
			 && (zwu_uri_decode_string(ipv6_addr_string, sizeof(ipv6_addr_string), args[i][1]) == ZWS_STATUS_OK))
	{
		i++;
		if ((zwu_inet_pton_iptype(ipv6_addr_string, ipv6_addr, &iptype) == ZWS_STATUS_OK) &&
			(zw_avro_field_set_fixed(arg_value_p, "gateway_address", ipv6_addr, 16) == ZWS_STATUS_OK))
		{
			//ZWS_LOG(ZWS_LOG_ERR, "ip_addr_string = %s", ipv6_addr_string);
			if (zw_avro_field_set_int(arg_value_p, "address_type", iptype) == ZWS_STATUS_OK)
			{
				if ((i < args_count) && !strcmp(args[i][0], "unsol_rpt_port")
					&& (zwu_parse_int32_non_negative(args[i][1], &host_port) == ZWU_STATUS_OK))
				{
					if (zw_avro_field_set_int(arg_value_p, "unsolicited_port_number", host_port) == ZWS_STATUS_OK)
					{
						i++;

						if (avro_value_get_by_name(arg_value_p, "preshared_key", &preshared_key, NULL) != 0)
						{
							ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'preshared_key' field from 'Gateway Set' record: %s", avro_strerror());
							goto l_err_get_preshared_key_field;
						}

						if ((i < args_count) && !strcmp(args[i][0], "key"))
						{
							key_len =  strlen(args[i][1]);
							//ZWS_LOG(ZWS_LOG_ERR, "received shared key = %s key_len = %u", args[i][1], key_len);

							if(!((key_len == 0) || (key_len%2)|| (key_len > 64)))
							{
								if (avro_value_set_branch(&preshared_key, 0, &preshared_key_value) != 0)
								{
									ZWS_LOG(ZWS_LOG_ERR, "Failed to set 'preshared_key' field to branch '0': %s", avro_strerror());
									goto l_err_set_preshared_key_value_field;
								}

								if (avro_value_set_string(&preshared_key_value, args[i][1]) != 0)
								{
									goto l_err_set_preshared_key_value;
								}
							}
							i++;
						}
						else
						{
							if (avro_value_set_branch(&preshared_key, 1, NULL) != 0)
							{
								ZWS_LOG(ZWS_LOG_ERR, "Failed to set 'preshared_key' field to branch '1': %s", avro_strerror());
								goto l_err_set_preshared_keyfield_info;
							}

						}

						return_status = ZWS_STATUS_OK;

					}
				}
			}
		}
	}

l_err_set_preshared_keyfield_info:
l_err_set_preshared_key_value:
l_err_set_preshared_key_value_field:
l_err_get_preshared_key_field:
	return return_status;
}

/**
 * @brief Set Avro value for RPC request for gateway list get command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_root_gateway_list_set_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i = 0;
	int discvr_flag = 0 ;

	return_status = ZWS_STATUS_OK;

	if((i < args_count) && !strcmp(args[i][0], "discvr"))
	{
		return_status = ZWS_STATUS_ERROR;
		 if(zwu_parse_int32_non_negative(args[i][1], &discvr_flag) == ZWS_STATUS_OK)
		 {
			 return_status = ZWS_STATUS_OK;
		 }
	}

	if (zw_avro_field_set_int(arg_value_p, "discovery_flag", discvr_flag) != ZWS_STATUS_OK)
	{
		return_status = ZWS_STATUS_ERROR;
	}
	return return_status;
}

/**
 * @brief Get CGI response for gateway list get from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_root_gateway_list_result(int web_api_version, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int unsol_rpt_port = 0;
	avro_value_t gw_list_value;
	avro_value_t gw_value;
	size_t gw_count=0;
	int gateway_flag = 0, addr_type=0;
	int i;
	void *gw_addr;
	size_t gw_addr_size;
	const char *gw_name ;
	char ip_addr_string[1+INET6_ADDRSTRLEN+1] = {0};
	int disc = 1;
	avro_value_t gw_name_field;
	avro_value_t gw_name_field_value;
	char gw_name_encode_str[GATEWAY_NAME_LEN_MAX * 3 + 1]={0};
	int discovery_state, received_reports, total_reports;
#if 0
	int encoded_str_len = 0;
#endif

	if ((zw_avro_field_get_int(arg_value,"discovery_state",&discovery_state) != ZWS_STATUS_OK) ||
		(zw_avro_field_get_int(arg_value,"received_reports",&received_reports) != ZWS_STATUS_OK) ||
		(zw_avro_field_get_int(arg_value,"total_reports",&total_reports) != ZWS_STATUS_OK))
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to set discovery_state info: %s", avro_strerror());
		goto l_err_get_discovery_state_info;
	}

	if (zwu_string_buffer_append_format(result,
			"<gw_discvr_status state=\"%u\" report_recvd=\"%u\" total_reports=\"%u\" />",
			(uint8_t)discovery_state,
			(uint32_t)received_reports,
			(uint32_t)total_reports
			) != ZWS_STATUS_OK)
	{
			goto l_err_append_1;
	}

	if (avro_value_get_by_name(arg_value, "gateway_list", &gw_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'gw_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (avro_value_get_size(&gw_list_value, &gw_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get gw list size");
		goto l_err_gw_list_size;
	}

	ZWS_LOG(ZWS_LOG_ERR, "gw_count: %u", gw_count);

	for(i=0;i<gw_count;i++)
	{
		if (avro_value_get_by_index(&gw_list_value, i, &gw_value, NULL) != 0)
		{
			goto l_err_get_gw_value;
		}

		if (zw_avro_field_get_fixed(&gw_value,"gateway_address",&gw_addr, &gw_addr_size) != ZWS_STATUS_OK)
		{
			goto l_err_get_gw_address;
		}

		if(gw_addr_size != 16)
		{
			goto l_err_get_gw_address_size;
		}

		if (avro_value_get_by_name(&gw_value, "gateway_name", &gw_name_field, NULL) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'gateway_name' field from 'Gateway List Report' record: %s", avro_strerror());
			goto l_err_get_gw_name_field;
		}

		if (avro_value_get_discriminant(&gw_name_field, &disc) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get branch number of 'gw_name_field' field: %s", avro_strerror());
			goto l_err_get_gw_name_field_info_disc;
		}

		ZWS_LOG(ZWS_LOG_ERR, "disc: %u", disc);

		if (disc == 0)
		{
			if (avro_value_get_current_branch(&gw_name_field, &gw_name_field_value) != 0)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'gateway_name' field in branch '0': %s", avro_strerror());
				goto l_err_get_gateway_name_field;
			}

			ZWS_LOG(ZWS_LOG_ERR, "got the branch: %u", disc);

			if (avro_value_get_string(&gw_name_field_value, &gw_name, &gw_addr_size) != 0)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'gateway_name' %s", avro_strerror());
				goto l_err_get_gateway_name;
			}
		}


		if(	(zw_avro_field_get_int(&gw_value,"address_type", &addr_type) != ZWS_STATUS_OK) ||
			(zw_avro_field_get_int(&gw_value,"unsolicited_port_number", &unsol_rpt_port) != ZWS_STATUS_OK) ||
			(zw_avro_field_get_int(&gw_value,"gateway_flag",&gateway_flag) != ZWS_STATUS_OK)
		   )
		{
			goto l_err_get_gw_values;
		}

		if((addr_type == TYPE_IPV6)||(addr_type == TYPE_IPV4))
		{
			if(zwu_inet_ntop_iptype(gw_addr, &ip_addr_string[1], addr_type, INET6_ADDRSTRLEN ) != ZWS_STATUS_OK)
			{
				goto l_err_get_ip_addr_string;
			}
			ZWS_LOG(ZWS_LOG_ERR,"ip_addr_string = %s", &ip_addr_string[1]);
		}
		else
		{
			goto l_err_get_addr_type;
		}

		if(disc == 0)
		{
#if 0
			ZWS_LOG(ZWS_LOG_ERR, "Gateway Name is not empty string");
			ip_addr_string[0] = '[';
			ip_addr_string[strlen(ip_addr_string)] = ']';

			if (zwu_uri_encode_string(gw_name_encode_str, sizeof(gw_name_encode_str), ip_addr_string) != ZWS_STATUS_OK)
			{
				goto l_err_uri_encode_gw_name;
			}

			encoded_str_len = strlen(gw_name_encode_str);

			ZWS_LOG(ZWS_LOG_ERR, "IP addr encoded_str = %s encoded len = %u", gw_name_encode_str, encoded_str_len);

			if (zwu_uri_encode_string(&gw_name_encode_str[encoded_str_len], (sizeof(gw_name_encode_str)-encoded_str_len), (char*)gw_name) != ZWS_STATUS_OK)
			{
				goto l_err_uri_encode_gw_name;
			}
#else

			if (zwu_uri_encode_string(gw_name_encode_str, sizeof(gw_name_encode_str), (char*)gw_name) != ZWS_STATUS_OK)
			{
				goto l_err_uri_encode_gw_name;
			}
#endif

			ZWS_LOG(ZWS_LOG_ERR, "gw_name_encode_str = %s", gw_name_encode_str);
		}

		if (zwu_string_buffer_append_format(result,
				"<gw name=\"%s\" addr=\"%s\"  unsol_rpt_port=\"%u\" flag=\"%u\" />",
				(disc == 0) ? gw_name_encode_str: "",
				&ip_addr_string[1],
				(uint16_t)unsol_rpt_port,
				(uint8_t)gateway_flag
				) != ZWS_STATUS_OK)
		{
				goto l_err_append;
		}
		zw_avro_field_free_fixed(gw_addr);
	}

	return ZWS_STATUS_OK;

l_err_append:
l_err_uri_encode_gw_name:
l_err_get_addr_type:
l_err_get_ip_addr_string:
l_err_get_gw_values:
l_err_get_gateway_name:
l_err_get_gateway_name_field:
l_err_get_gw_name_field_info_disc:
l_err_get_gw_name_field:
l_err_get_gw_address_size:
	zw_avro_field_free_fixed(gw_addr);
l_err_get_gw_address:
l_err_get_gw_value:
l_err_gw_list_size:
l_err_get_list_value:
l_err_append_1:
l_err_get_discovery_state_info:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Get CGI response for gateway list get from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_root_gateway_get_result(int web_api_version, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	const char *gateway_name = NULL;
	void *gateway_address;
	int unsol_rpt_port = 0;
	int addr_type = 0, gateway_status = 0;
	size_t gw_address_size;
	char ip_addr_string[INET6_ADDRSTRLEN] = {0};
	char gw_name_encode_str[GATEWAY_NAME_ONLY_MAX * 3 + 1]={0};
	char gw_ip_encode_str[GATEWAY_IP_ADDRESS_ONLY_MAX * 3 + 1]={0};
	int disc=1;
	avro_value_t gw_name_field;
	avro_value_t gw_name_field_value;

	if (avro_value_get_by_name(arg_value, "gateway_name", &gw_name_field, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'gateway_name' field from GatewayReport record: %s", avro_strerror());
		goto l_err_get_gateway_name_field;
	}

	if (avro_value_get_discriminant(&gw_name_field, &disc) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get branch number of 'gw_name_field' field: %s", avro_strerror());
		goto l_err_get_gateway_name_discriminant_disc;
	}

	if (disc == 0)
	{
		if (avro_value_get_current_branch(&gw_name_field, &gw_name_field_value) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'gateway_name' field in branch '0': %s", avro_strerror());
			goto l_err_get_gateway_name_branch_field;
		}

		if (avro_value_get_string(&gw_name_field_value, &gateway_name, &gw_address_size) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'gateway_name' %s", avro_strerror());
			goto l_err_get_gateway_name;
		}
	}

	if(zw_avro_field_get_fixed(arg_value,"gateway_address",&gateway_address, &gw_address_size) != ZWS_STATUS_OK)
	{
		ZWS_LOG(ZWS_LOG_ERR,"gateway_address Failed");
		goto l_err_get_gateway_address;
	}

	if(gw_address_size != 16)
	{
		ZWS_LOG(ZWS_LOG_ERR,"gateway_address Invalid size");
		goto l_err_get_gateway_size;
	}

	if(	(zw_avro_field_get_int(arg_value,"address_type", &addr_type) != ZWS_STATUS_OK) ||
		(zw_avro_field_get_int(arg_value,"unsolicited_port_number", &unsol_rpt_port) != ZWS_STATUS_OK) ||
		(zw_avro_field_get_int(arg_value,"gateway_status",&gateway_status) != ZWS_STATUS_OK)
	  )
	{
		ZWS_LOG(ZWS_LOG_ERR,"gateway values Failed");
		goto l_err_get_gw_values;
	}

	if((addr_type == TYPE_IPV6)||(addr_type == TYPE_IPV4))
	{
		if(zwu_inet_ntop_iptype(gateway_address, ip_addr_string, addr_type, sizeof(ip_addr_string) ) != ZWS_STATUS_OK)
		{
			goto l_err_get_ip_addr_string;
		}
		ZWS_LOG(ZWS_LOG_ERR,"ip_addr_string = %s", ip_addr_string);
	}
	else
	{
		ZWS_LOG(ZWS_LOG_ERR,"gateway IP address conversion failed");
		goto l_err_get_addr_type;
	}

	if(gateway_name)
	{
		if (zwu_uri_encode_string(gw_name_encode_str, sizeof(gw_name_encode_str), (char *)gateway_name) != ZWS_STATUS_OK)
		{
			goto l_err_uri_encode_gw_name;
		}
	}

	if (zwu_uri_encode_string(gw_ip_encode_str, sizeof(gw_ip_encode_str), ip_addr_string) != ZWS_STATUS_OK)
	{
		goto l_err_uri_encode_gw_name;
	}


	//Name field shouldn't be empty
	if (zwu_string_buffer_append_format(result,
			"<gw_status status=\"%u\" name=\"%s\" addr=\"%s\" unsol_rpt_port=\"%u\" />",
			(uint8_t)gateway_status,
			(disc == 0) ? gw_name_encode_str : "",
			gw_ip_encode_str,
			(uint16_t)unsol_rpt_port
			) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	zw_avro_field_free_fixed(gateway_address);

	return ZWS_STATUS_OK;


l_err_append:
l_err_uri_encode_gw_name:
l_err_get_addr_type:
l_err_get_ip_addr_string:
l_err_get_gw_values:
l_err_get_gateway_size:
	zw_avro_field_free_fixed(gateway_address);
l_err_get_gateway_address:
l_err_get_gateway_name:
l_err_get_gateway_name_branch_field:
l_err_get_gateway_name_discriminant_disc:
l_err_get_gateway_name_field:

	return ZWS_STATUS_ERROR;
}

/** Handlers for setting the gateway */
static zws_avi_message_t root_gateway_set =
{
		"zw_gw_set",

		"root_gateway_set",

		zws_avi_root_gateway_set_argument,

		NULL,

		""
};

/** Handlers for getting currently connected gateway status */
static zws_avi_message_t root_gateway_get =
{
		"zw_gw_get",

		"root_gateway_get",

		NULL,

		zws_avi_root_gateway_get_result,

		""
};

/** Handlers for getting list of discovered gateways */
static zws_avi_message_t root_gateway_list =
{
		"zw_gw_list",

		"root_gateway_list",

		zws_avi_root_gateway_list_set_argument,

		zws_avi_root_gateway_list_result,

		""
};

/** Register command handlers */
zws_status_t zws_avi_root_gateway_init(zwu_array_list_t message_list)
{
	if ((zws_avi_message_list_add_message(message_list, &root_gateway_set) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message(message_list, &root_gateway_get) != ZWS_STATUS_OK) ||
		(zws_avi_message_list_add_message(message_list, &root_gateway_list) != ZWS_STATUS_OK)
	   )
	{
		goto l_err_gateway_init;
	}
	return ZWS_STATUS_OK;

l_err_gateway_init:
	return ZWS_STATUS_ERROR;
}

