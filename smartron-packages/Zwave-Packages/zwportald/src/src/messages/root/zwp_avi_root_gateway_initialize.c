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
 * @file    zwp_avi_root_gateway_initialize.c
 *
 * @brief   AVI message 'root_gateway_set/get/list'
 * @details Handles the AVI message 'root_gateway_set/get/list.
 *
 * @author  Sasidhar
 *
 * @version 1.0 - 2014-11-11
 * - Initial version
 */

#include "zwp_status.h"
#include "zwp_avi_message.h"
#include "zwp_log.h"
#include "zwp_memory.h"
#include "zwp_error.h"
#include "zwp_portal.h"
#include "zwp_directory_service.h"
#include "zwp_net_discovery.h"
#include "zwp_zwave_client.h"
#include "zwp_network.h"

#define IPV6_ADDR_LEN               16      ///< IPv6 address length in bytes
#define IPV4_ADDR_LEN               4       ///< IPv4 address length in bytes

/** Gateway Set command state */
typedef struct
{
	int 		address_type;				/**< Gateway Address type(Ipv4 or Ipv6) */
	uint8_t    	gateway_address[16];		/**< Gateway Address */
	uint16_t    unsolicited_port_number;	/**< Host Port Number */
	uint8_t     dtls_pskey[32];				/**< Pre-shared key for DTLS communication */
	uint8_t		key_len;					/**< Length of Pre-shared key */
} zwp_avi_entry_root_gateway_set;

/** Gateway Report command state */
typedef struct
{
	int 		address_type;				/**< Gateway Address type(Ipv4 or Ipv6) */
	uint8_t    	gateway_address[16];		/**< Gateway Address */
	uint16_t    unsolicited_port_number;	/**< Host Port Number */
	char 		gateway_name[64];			/**< Gateway Name */
	uint8_t     gateway_status;				/**< Gateway Status. 1 = Initialized, 0 = Uninitialized */
	uint8_t     dtls_pskey[32];				/**< Pre-shared key for DTLS communication */
} zwp_avi_entry_root_gateway_report;

/** Gateway Discovery list and state report */
typedef struct
{
	uint8_t *ipv4_list;										/**< List of IPv4  Z/IP Gateways */
	size_t ipv4_list_len;									/**< Length of IPv4 Z/IP Gateways list */
	char   **gw_name_ipv4_gwv2;								/**< MDNS names corresponding to IPv4 Gateway list */

	uint8_t *ipv6_list;										/**< List of IPv6  Z/IP Gateways */
	size_t ipv6_list_len;									/**< Length of IPv6 Z/IP Gateways list */
	char   **gw_name_ipv6_gwv2;								/**< MDNS names corresponding to IPv6 Gateway list */

	int32_t received_reports;								/**< Number of discovery reports that are received so far */
	int32_t total_reports;									/**< Total number of discovery reports that will be delivered */
	uint8_t type;											/**< Current discovery type (ZWP_NET_DISCOVERY_TYPE_XXXX) */

	zip_gw_name_t *cached_list;								/**< List of Cached Z/IP Gateways */
	zwp_avi_entry_root_gateway_report	current_gw_status;	/**< Currently connected Z/IP Gateway status */

}zwp_gw_list_report;


/**
zwp_hex2bin - Convert hex character to binary
@param[in] c        hex character
@return  Value of hex character on success, negative value on failure
*/
static int zwp_hex2bin(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c-'0';
    }
    else if (c >= 'a' && c <= 'f')
    {
        return c-'a' + 10;
    }
    else if (c >= 'A' && c <= 'F')
    {
        return c-'A' + 10;
    }
    else
    {
        return -1;
    }
}


/**
zwp_hexstring_to_bin - Convert ASCII hexstring to binary string
@param[in] psk_str   ASCII hexstring
@param[in] psk_len   ASCII hexstring length (must be even number)
@param[out] psk_bin  Binary string
@return  Zero on success, non-zero on failure
*/
int zwp_hexstring_to_bin(char *psk_str, int psk_len, uint8_t *psk_bin)
{
    int i = 0;
    int val;

    while(psk_len > 0)
    {
      val = zwp_hex2bin(*psk_str++);
      if(val < 0)
          return -1;
      psk_bin[i]  = (val & 0x0F) << 4;

      val = zwp_hex2bin(*psk_str++);
      if(val < 0)
          return -1;
      psk_bin[i] |= (val & 0x0F);

      i++;
      psk_len -= 2;
    }

    return 0;
}

/**
 * @brief Set argument for Gateway Set from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Gateway Set command
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_root_gateway_set_argument_free() to free resources
 */

static zwp_status_t zwp_avi_root_gateway_set_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_entry_root_gateway_set *argument;
	int addr_type = 0, port_number = 0;
	size_t gw_addr_size;
	void *gw_addr;
	avro_value_t preshared_key;
	avro_value_t preshared_key_value;
	int disc;
	const char *preshared_key_string;
	size_t key_size;


	if ((argument = (zwp_avi_entry_root_gateway_set *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if ((zw_avro_field_get_int(arg_value, "address_type", &addr_type) != ZWP_STATUS_OK)||
		(zw_avro_field_get_int(arg_value, "unsolicited_port_number", &port_number) != ZWP_STATUS_OK)
	   )
	{
		goto l_err_get_value_addr_type_port_number;
	}

	if (zw_avro_field_get_fixed(arg_value, "gateway_address", &gw_addr, &gw_addr_size) != ZWP_STATUS_OK)
	{
		goto l_err_get_value_gateway_address;
	}

	if (gw_addr_size != 16)
	{
		goto l_err_gw_addr_size;
	}

	if (avro_value_get_by_name(arg_value, "preshared_key", &preshared_key, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'preshared_key' field from 'Gateway Set' record: %s", avro_strerror());
		goto l_err_get_preshared_key_field;
	}

	if (avro_value_get_discriminant(&preshared_key, &disc) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get branch number of 'preshared_key' field: %s", avro_strerror());
		goto l_err_get_preshared_key_field_info_disc;
	}

	if (disc == 0)
	{
		if (avro_value_get_current_branch(&preshared_key, &preshared_key_value) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'preshared_key' field in branch '0': %s", avro_strerror());
			goto l_err_get_preshared_key_field_value;
		}

		if (avro_value_get_string(&preshared_key_value, &preshared_key_string, &key_size) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'preshared_key' %s", avro_strerror());
			goto l_err_get_preshared_key_value;
		}

		ZWP_LOG(ZWP_LOG_ERR, "preshared_key strlen = %u", strlen(preshared_key_string));
		if(key_size > 0)
			key_size -= 1; //remove null character from count
		else
			key_size = 0;

		if((!key_size)||(key_size%2)) //Odd length keys are not accepted
		{
			goto l_err_get_preshared_key_odd_size;
		}

		if(zwp_hexstring_to_bin((char *)preshared_key_string, key_size, argument->dtls_pskey))
		{
			goto l_err_get_preshared_key_conversion_failed;
		}

		argument->key_len = key_size/2;
	}
	else
	{
		argument->key_len = 0;
		memset(argument->dtls_pskey, 0, sizeof(argument->dtls_pskey));
	}


	argument->address_type = addr_type;
	argument->unsolicited_port_number = port_number;
	memcpy(argument->gateway_address, gw_addr, gw_addr_size);

	zw_avro_field_free_fixed(gw_addr);

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_get_preshared_key_conversion_failed:
l_err_get_preshared_key_odd_size:
l_err_get_preshared_key_value:
l_err_get_preshared_key_field_value:
l_err_get_preshared_key_field_info_disc:
l_err_get_preshared_key_field:
l_err_gw_addr_size:
	zw_avro_field_free_fixed(gw_addr);
l_err_get_value_gateway_address:
l_err_get_value_addr_type_port_number:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Set command
 * @param[in]     args              Arguments passed with Set command
 * @return        Nothing
 * @see zwp_status_t zwp_avi_root_gateway_set_argument
 */
static void zwp_avi_root_gateway_set_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
 * @brief Execute Gateway set command
 * @param[in]     portal            Portal object
 * @param[in]     user              User name
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 */ 
static zwp_status_t zwp_avi_root_gateway_set_execute(zwp_portal_t portal, char *user, void *arg, void **result_p)
{
	zwp_net_t   net;
	zwp_avi_entry_root_gateway_set *zip_config;
	zwp_zwave_client_ip_settings_t zip_client_ip_settings;
	char dummy_ra_code[1] = {0};
	char *local_state_dir;
	zwp_zwave_client_t client;

	zip_config = (zwp_avi_entry_root_gateway_set *)arg;

	ZWP_LOG(ZWP_LOG_DEBUG, "gateway_set_execute called");

	if (zwp_portal_get_client_intialize_lock(portal) != ZWP_STATUS_OK)
	{
		goto l_err_client_intialize_lock;
	}

	//TODO:clean z-wave client and network discovery related pools and memory.
	zwp_portal_client_kill_net_by_ra_code(portal, dummy_ra_code);

	//Todo avoid double copying...
	zip_client_ip_settings.ipv4 = zip_config->address_type;
	memcpy(zip_client_ip_settings.zip_server_address, zip_config->gateway_address, sizeof(zip_client_ip_settings.zip_server_address));
	zip_client_ip_settings.unsolicited_port_number = zip_config->unsolicited_port_number;
	zip_client_ip_settings.key_len = zip_config->key_len;
	memcpy(zip_client_ip_settings.dtls_pskey, zip_config->dtls_pskey, sizeof(zip_client_ip_settings.dtls_pskey));

    zwp_portal_set_zip_client_ip_settings(portal, &zip_client_ip_settings);
	local_state_dir = zwp_portal_get_local_state_dir(portal);

	client = zwp_portal_get_zip_client_handle(portal);

    zwp_zwave_client_set_zip_server_to_file(client, local_state_dir, &zip_client_ip_settings);
    //update the cached list
    zwp_zwave_client_get_zip_server_from_file(client, local_state_dir);

	if (zwp_portal_client_spawn_net(portal, NULL, &net) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "portal_client_spawn failed");
		goto l_err_spawn_net;
	}
	zwp_portal_put_client_intialize_lock(portal);
	return ZWP_STATUS_OK;
l_err_spawn_net:
	zwp_portal_put_client_intialize_lock(portal);
l_err_client_intialize_lock:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set argument for Gateway List from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Gateway List Set command
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_root_gateway_list_set_argument_free() to free resources
 */
static zwp_status_t zwp_avi_root_gateway_list_set_argument(avro_value_t *arg_value, void **args_p)
{
	int *argument;

	if ((argument = (int *)ZWP_CALLOC(1, sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "discovery_flag", argument) != ZWP_STATUS_OK)
	{
		goto  l_err_discovery_flag;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;
l_err_discovery_flag:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;

}

/**
 * @brief Free arguments for Set argument
 * @param[in]     args              Arguments passed with Set command
 * @return        Nothing
 * @see zwp_status_t zwp_avi_root_gateway_list_set_argument
 */
static void zwp_avi_root_gateway_list_set_argument_free(void *args)
{
	ZWP_FREE(args);
}


/**
 * @brief Execute Gateway Discover command
 * @param[in]     portal            Portal object
 * @param[in]     user              User name
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 * @note
 * @par
 * The caller should call zwp_avi_root_gateway_list_result_free() to free resources if the return value is success
 */ 
static zwp_status_t zwp_avi_root_gateway_list_execute(zwp_portal_t portal, char *user, void *arg, void **result_p)
{
	zwp_gw_list_report *argument;
	zwp_zwave_client_t pzip_client;
	int discovery_flag = 0;
	zwp_net_discovery_report_t  zwp_net_discovery_report;
	zwp_zwave_client_ip_settings_t zip_client_ip_settings;

	pzip_client = zwp_portal_get_zip_client_handle(portal);
	discovery_flag = *(int *)arg;

	//Use only calloc ..we are freeing list without checking len
	if ((argument = (zwp_gw_list_report *)ZWP_CALLOC(1,sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (discovery_flag == 1)
	{
		if(zwp_discovery_start(pzip_client) != ZWP_STATUS_OK)
		{
			goto l_err_net_discover_all;
		}
	}

	//Get the IPV4 list first
	memset(&zwp_net_discovery_report, 0, sizeof(zwp_net_discovery_report));
	if (zwp_net_discovery_get_list(pzip_client->net_discovery,  ZWP_NET_DISCOVERY_IP_VERSION_4, &zwp_net_discovery_report) != ZWP_STATUS_OK)
	{
		goto l_err_net_discover_ipv4_list;
	}

	if(zwp_net_discovery_report.gwlistlen > 0)
	{
		//We should free it later
		argument->ipv4_list = zwp_net_discovery_report.pgwlist;
		argument->ipv4_list_len = zwp_net_discovery_report.gwlistlen;
		argument->gw_name_ipv4_gwv2 = zwp_net_discovery_report.gwname;

		argument->received_reports = zwp_net_discovery_report.received_reports;
		argument->total_reports = zwp_net_discovery_report.total_reports;
	}

	argument->type = zwp_net_discovery_report.type;

	memset(&zwp_net_discovery_report, 0, sizeof(zwp_net_discovery_report));
	if (zwp_net_discovery_get_list(pzip_client->net_discovery,  ZWP_NET_DISCOVERY_IP_VERSION_6, &zwp_net_discovery_report) != ZWP_STATUS_OK)
	{
		goto l_err_net_discover_ipv6_list;
	}

	if(zwp_net_discovery_report.gwlistlen > 0)
	{
		//We should free it later
		argument->ipv6_list = zwp_net_discovery_report.pgwlist;
		argument->ipv6_list_len = zwp_net_discovery_report.gwlistlen;
		argument->gw_name_ipv6_gwv2 = zwp_net_discovery_report.gwname;

		argument->received_reports = zwp_net_discovery_report.received_reports;
		argument->total_reports = zwp_net_discovery_report.total_reports;
	}

	argument->type = zwp_net_discovery_report.type;


	if ((argument->cached_list = (zip_gw_name_t *)ZWP_CALLOC(MAX_CACHED_ZIP_GW_ADDR,sizeof(zip_gw_name_t))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_cached_list_alloc;
	}

	//Get the list of cached gateways
	if (zwp_zwave_client_get_copy_cached_list(pzip_client, argument->cached_list) != ZWP_STATUS_OK)
	{
		goto l_err_net_discover_cached_list;
	}

	if(pzip_client)
	{
		argument->current_gw_status.gateway_status = (zwp_zwave_client_get_net(pzip_client)) ? ((zwp_net_get_desc_net(zwp_zwave_client_get_net(pzip_client))) ? 1 : 0) : 0;
		zwp_portal_get_zip_client_ip_settings(portal, &zip_client_ip_settings);
		argument->current_gw_status.address_type = zip_client_ip_settings.ipv4;
		argument->current_gw_status.unsolicited_port_number = zip_client_ip_settings.unsolicited_port_number;
		memcpy(argument->current_gw_status.gateway_address, zip_client_ip_settings.zip_server_address, sizeof(argument->current_gw_status.gateway_address));
	}
	else
	{
		argument->current_gw_status.gateway_status = 0;
		argument->current_gw_status.unsolicited_port_number = 0;
		argument->current_gw_status.address_type = 0;
		memset(&argument->current_gw_status, 0, sizeof(argument->current_gw_status));
	}

	argument->current_gw_status.gateway_name[0]= 0;

	*result_p = argument;

	return ZWP_STATUS_OK;

l_err_net_discover_cached_list:
	ZWP_FREE(argument->cached_list);
l_err_cached_list_alloc:
	ZWP_FREE(argument->ipv6_list);
l_err_net_discover_ipv6_list:
	ZWP_FREE(argument->ipv4_list);
l_err_net_discover_ipv4_list:
l_err_net_discover_all:
	ZWP_FREE(argument);
l_err_args_alloc:
		return ZWP_STATUS_ERROR;
}

/**
 * @brief Clear the cached list flag if the give entry matches with cached list entry.
 * @param[in]    cached_list     Cached list object
 * @param[in]    size        	 size of the cached list
 * @param[in]    name        	 gateway name that should be checked in cached list
 * @param[in]    addr_type       gateway address(name) type
 * @retval       nothing
 */
static void zwp_avi_root_gateway_update_cached_list(zip_gw_name_t *cached_list, int size, char*name, uint8_t addr_type)
{
	int i = 0;
	int addr_len;

	for(i=0; i<size; i++)
	{
		addr_len = ((addr_type == ZWP_NET_DISCOVERY_IP_VERSION_6) ? IPV6_ADDR_LEN : IPV4_ADDR_LEN);
		if ((cached_list[i].address_type == addr_type) && !memcmp(cached_list[i].name, name, addr_len) )
		{
			cached_list[i].flag &= ~ZIP_GW_FLAG_CACHE;
			break;
		}
	}
}

/**
 * @brief Set Avro value object with values from Result object corresponding to list of discovered gateways
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 * @note
 * @par
 * The caller should call zwp_avi_root_gateway_list_result_free() to free resources
 */
static zwp_status_t zwp_avi_root_gateway_list_result(void *result, avro_value_t *arg_value)
{
	zwp_gw_list_report *pgw_list_report;
	avro_value_t gw_list_value;
	avro_value_t gw_value;
	size_t i = 0;
	unsigned char gw_addr[IPV6_ADDR_LEN] = {0};
	unsigned char addr_type = 0, gateway_flag = 0;
	uint16_t unsolicited_port_number = 0;
	char *gateway_name = NULL;
	avro_value_t gw_name_field;
	avro_value_t gw_name_string;
	uint16_t addr_len = 0;

	pgw_list_report = (zwp_gw_list_report *)result;

	if ((zw_avro_field_set_int(arg_value,"discovery_state",pgw_list_report->type) != ZWP_STATUS_OK) ||
		(zw_avro_field_set_int(arg_value,"received_reports",pgw_list_report->received_reports) != ZWP_STATUS_OK) ||
		(zw_avro_field_set_int(arg_value,"total_reports",pgw_list_report->total_reports) != ZWP_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to set discovery_state info: %s", avro_strerror());
		goto l_err_set_discovery_state_info;
	}

	if (avro_value_get_by_name(arg_value, "gateway_list", &gw_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'gateway_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	for(i=0; i< pgw_list_report->ipv4_list_len; i++)
	{
		if (avro_value_append(&gw_list_value, &gw_value, 0) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to Append gw_list_value: %s", avro_strerror());
			goto l_err_append_ipv4_gw_list_value;
		}

		memcpy(gw_addr, &pgw_list_report->ipv4_list[i*IPV4_ADDR_LEN], IPV4_ADDR_LEN);
		addr_type = ZWP_NET_DISCOVERY_IP_VERSION_4;
		unsolicited_port_number = 0;
		gateway_flag = ZIP_GW_FLAG_DISCVR;
		gateway_name =  pgw_list_report->gw_name_ipv4_gwv2[i];

		if((pgw_list_report->current_gw_status.gateway_status == 1) && (pgw_list_report->current_gw_status.address_type == ZWP_NET_DISCOVERY_IP_VERSION_4))
		{
			if(!memcmp(gw_addr,pgw_list_report->current_gw_status.gateway_address, IPV4_ADDR_LEN))
			{
				gateway_flag |= ZIP_GW_FLAG_BUSY;
				unsolicited_port_number = pgw_list_report->current_gw_status.unsolicited_port_number;
			}
		}

		if (avro_value_get_by_name(&gw_value, "gateway_name", &gw_name_field, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'gateway_name' field from 'Gateway List Report' record: %s", avro_strerror());
			goto l_err_get_gw_name_field;
		}

		if(gateway_name)
		{
			gateway_flag |= ZIP_GW_FLAG_V2;
			if (avro_value_set_branch(&gw_name_field, 0, &gw_name_string) != 0)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to set 'gw_name_string' field to branch '0': %s", avro_strerror());
				goto l_err_set_gw_name_field_info_1;
			}

			if (avro_value_set_string(&gw_name_string, gateway_name) != 0)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to set 'gw_name_string' : %s", avro_strerror());
				goto l_err_set_gw_name_value;
			}
		}
		else
		{
			if (avro_value_set_branch(&gw_name_field, 1, NULL) != 0)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to set 'gateway_name' field to branch '1': %s", avro_strerror());
				goto l_err_set_gw_name_field_info_2;
			}
		}

		if( (zw_avro_field_set_fixed(&gw_value,"gateway_address",gw_addr, 16) != ZWP_STATUS_OK) ||
			(zw_avro_field_set_int(&gw_value,"address_type",addr_type) != ZWP_STATUS_OK) ||
			(zw_avro_field_set_int(&gw_value,"unsolicited_port_number",unsolicited_port_number) != ZWP_STATUS_OK) ||
			(zw_avro_field_set_int(&gw_value,"gateway_flag",gateway_flag) != ZWP_STATUS_OK)
		   )
		{
			ZWP_LOG(ZWP_LOG_ERR,"Setting Gateway list result values failed");
			goto l_err_set_gw_value;
		}

		zwp_avi_root_gateway_update_cached_list(pgw_list_report->cached_list, MAX_CACHED_ZIP_GW_ADDR, (char *)gw_addr, addr_type);
	}

	for(i=0; i< pgw_list_report->ipv6_list_len; i++)
	{
		if (avro_value_append(&gw_list_value, &gw_value, 0) != 0)
		{
			goto l_err_append_ipv6_gw_list_value;
		}

		memcpy(gw_addr, &pgw_list_report->ipv6_list[i*IPV6_ADDR_LEN], IPV6_ADDR_LEN);
		addr_type = ZWP_NET_DISCOVERY_IP_VERSION_6;
		unsolicited_port_number = 0;
		gateway_flag = ZIP_GW_FLAG_DISCVR;
		gateway_name = pgw_list_report->gw_name_ipv6_gwv2[i];

		if((pgw_list_report->current_gw_status.gateway_status == 1) && (pgw_list_report->current_gw_status.address_type == ZWP_NET_DISCOVERY_IP_VERSION_6))
		{
			if(!memcmp(gw_addr,pgw_list_report->current_gw_status.gateway_address, IPV6_ADDR_LEN))
			{
				gateway_flag |= ZIP_GW_FLAG_BUSY;
				unsolicited_port_number = pgw_list_report->current_gw_status.unsolicited_port_number;
			}
		}

		if (avro_value_get_by_name(&gw_value, "gateway_name", &gw_name_field, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'gateway_name' field from 'Gateway List Report' record: %s", avro_strerror());
			goto l_err_get_ipv6_gw_name_field;
		}

		if(gateway_name)
		{
			gateway_flag |= ZIP_GW_FLAG_V2;

			if (avro_value_set_branch(&gw_name_field, 0, &gw_name_string) != 0)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to set 'gw_name_string' field to branch '0': %s", avro_strerror());
				goto l_err_set_ipv6_gw_name_field_info_1;
			}

			if (avro_value_set_string(&gw_name_string, gateway_name) != 0)
			{
				goto l_err_set_ipv6_gw_name_value;
			}

		}
		else
		{
			if (avro_value_set_branch(&gw_name_field, 1, NULL) != 0)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to set 'gateway_name' field to branch '1': %s", avro_strerror());
				goto l_err_set_ipv6_gw_name_field_info_2;
			}
		}

		if( (zw_avro_field_set_fixed(&gw_value,"gateway_address",gw_addr, 16) != ZWP_STATUS_OK) ||
			(zw_avro_field_set_int(&gw_value,"address_type",addr_type) != ZWP_STATUS_OK) ||
			(zw_avro_field_set_int(&gw_value,"unsolicited_port_number",unsolicited_port_number) != ZWP_STATUS_OK) ||
			(zw_avro_field_set_int(&gw_value,"gateway_flag",gateway_flag) != ZWP_STATUS_OK)
		   )
		{
			ZWP_LOG(ZWP_LOG_ERR,"Setting Gateway list result values failed");
			goto l_err_set_ipv6_gw_value;
		}

		zwp_avi_root_gateway_update_cached_list(pgw_list_report->cached_list, MAX_CACHED_ZIP_GW_ADDR, (char *)gw_addr, addr_type);
	}

	//Send the cached list
	for(i=0; i < MAX_CACHED_ZIP_GW_ADDR; i++)
	{
		if (pgw_list_report->cached_list[i].flag)
		{
			if (avro_value_append(&gw_list_value, &gw_value, 0) != 0)
			{
				goto l_err_append_cached_gw_list_value;
			}

			if (avro_value_get_by_name(&gw_value, "gateway_name", &gw_name_field, NULL) != 0)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'gateway_name' field from 'Gateway List Report' record: %s", avro_strerror());
				goto l_err_get_cached_gw_name_field;
			}

			if (avro_value_set_branch(&gw_name_field, 1, NULL) != 0)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to set 'gateway_name' field to branch '1': %s", avro_strerror());
				goto l_err_set_cached_gw_name_field_info_2;
			}

			if(pgw_list_report->current_gw_status.gateway_status == 1)
			{
				addr_len = ((pgw_list_report->current_gw_status.address_type == ZWP_NET_DISCOVERY_IP_VERSION_6) ? IPV6_ADDR_LEN : IPV4_ADDR_LEN);
				if(!memcmp(pgw_list_report->cached_list[i].name,pgw_list_report->current_gw_status.gateway_address, addr_len))
				{
					pgw_list_report->cached_list[i].flag |= ZIP_GW_FLAG_BUSY;
				}
			}

			if( (zw_avro_field_set_fixed(&gw_value,"gateway_address",pgw_list_report->cached_list[i].name, 16) != ZWP_STATUS_OK) ||
				(zw_avro_field_set_int(&gw_value,"address_type",pgw_list_report->cached_list[i].address_type) != ZWP_STATUS_OK) ||
				(zw_avro_field_set_int(&gw_value,"unsolicited_port_number",pgw_list_report->cached_list[i].unsol_rpt_port) != ZWP_STATUS_OK) ||
				(zw_avro_field_set_int(&gw_value,"gateway_flag",pgw_list_report->cached_list[i].flag) != ZWP_STATUS_OK)
			  )
			{
				ZWP_LOG(ZWP_LOG_ERR,"Setting Gateway list result values failed");
				goto l_err_set_cached_gw_value;
			}
		}
	}

	return ZWP_STATUS_OK;

l_err_set_cached_gw_value:
l_err_set_cached_gw_name_field_info_2:
l_err_get_cached_gw_name_field:
l_err_append_cached_gw_list_value:
l_err_set_ipv6_gw_value:
l_err_set_ipv6_gw_name_field_info_2:
l_err_set_ipv6_gw_name_value:
l_err_set_ipv6_gw_name_field_info_1:
l_err_get_ipv6_gw_name_field:
l_err_append_ipv6_gw_list_value:
l_err_set_gw_value:
l_err_set_gw_name_field_info_2:
l_err_set_gw_name_value:
l_err_set_gw_name_field_info_1:
l_err_get_gw_name_field:
l_err_append_ipv4_gw_list_value:
l_err_get_list_value:
l_err_set_discovery_state_info:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to list of discovered gateways
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_root_gateway_list_result
 */
static void zwp_avi_root_gateway_list_result_free(void *result)
{
	zwp_gw_list_report *pgw_list_report = (zwp_gw_list_report *)result;
	int i = 0;

	if(pgw_list_report->gw_name_ipv4_gwv2)
	{
		for(i=0;i<pgw_list_report->ipv4_list_len; i++)
		{
			ZWP_FREE(pgw_list_report->gw_name_ipv4_gwv2[i]);
		}
		ZWP_FREE(pgw_list_report->gw_name_ipv4_gwv2);
	}

	if(pgw_list_report->gw_name_ipv6_gwv2)
	{
		for(i=0;i<pgw_list_report->ipv6_list_len; i++)
		{
			ZWP_FREE(pgw_list_report->gw_name_ipv6_gwv2[i]);
		}
		ZWP_FREE(pgw_list_report->gw_name_ipv6_gwv2);
	}

	ZWP_FREE(pgw_list_report->ipv4_list);
	ZWP_FREE(pgw_list_report->ipv6_list);
	ZWP_FREE(pgw_list_report->cached_list);
	ZWP_FREE(pgw_list_report);
}

/**
 * @brief Execute Gateway get command
 * @param[in]     portal            Portal object
 * @param[in]     user              User name
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 */
static zwp_status_t zwp_avi_root_gateway_get_execute(zwp_portal_t portal, char *user, void *arg, void **result_p)
{
	zwp_avi_entry_root_gateway_report *pgateway_result;
	zwp_zwave_client_ip_settings_t zip_client_ip_settings;
	zwp_zwave_client_t client_handle;

	if ((pgateway_result = (zwp_avi_entry_root_gateway_report *)ZWP_CALLOC(1, sizeof(*pgateway_result))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_gateway_report_alloc;
	}

	client_handle = zwp_portal_get_zip_client_handle(portal);

	if(client_handle)
	{
		pgateway_result->gateway_status = (zwp_zwave_client_get_net(client_handle)) ? ((zwp_net_get_desc_net(zwp_zwave_client_get_net(client_handle))) ? 1 : 0) : 0;
		zwp_portal_get_zip_client_ip_settings(portal, &zip_client_ip_settings);
		pgateway_result->address_type = zip_client_ip_settings.ipv4;
		pgateway_result->unsolicited_port_number = zip_client_ip_settings.unsolicited_port_number;
		memcpy(pgateway_result->gateway_address, zip_client_ip_settings.zip_server_address, sizeof(pgateway_result->gateway_address));
	}
	else
	{
		pgateway_result->gateway_status = 0;
		pgateway_result->unsolicited_port_number = 0;
		pgateway_result->address_type = 0;
		memset(pgateway_result->gateway_address, 0, sizeof(pgateway_result->gateway_address));
	}

	pgateway_result->gateway_name[0]= 0;

	*result_p = pgateway_result;

	return ZWP_STATUS_OK;

l_err_gateway_report_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to gateway report
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 * @note
 * @par
 * The caller should call zwp_avi_root_gateway_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_root_gateway_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_entry_root_gateway_report *pgateway_result;
	avro_value_t gw_name_field;
	avro_value_t gw_name_string;

	pgateway_result = (	zwp_avi_entry_root_gateway_report *)result;

	if (avro_value_get_by_name(arg_value, "gateway_name", &gw_name_field, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'gateway_name' field from 'Gateway List Report' record: %s", avro_strerror());
		goto l_err_get_gw_name_field;
	}

	if(strlen(pgateway_result->gateway_name))
	{
		if (avro_value_set_branch(&gw_name_field, 0, &gw_name_string) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to set 'gw_name_string' field to branch '0': %s", avro_strerror());
			goto l_err_set_gw_name_field_info_1;
		}

		if (avro_value_set_string(&gw_name_string, pgateway_result->gateway_name) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to set 'gateway_name' : %s", avro_strerror());
			goto l_err_set_gw_name_value;
		}

	}
	else
	{
		if (avro_value_set_branch(&gw_name_field, 1, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to set 'gateway_name' field to branch '1': %s", avro_strerror());
			goto l_err_set_gw_name_field_info_2;
		}
	}

	if(	(zw_avro_field_set_fixed(arg_value,"gateway_address",pgateway_result->gateway_address, 16) != ZWP_STATUS_OK) ||
		(zw_avro_field_set_int(arg_value,"address_type",pgateway_result->address_type) != ZWP_STATUS_OK) ||
		(zw_avro_field_set_int(arg_value,"unsolicited_port_number",pgateway_result->unsolicited_port_number) != ZWP_STATUS_OK) ||
		(zw_avro_field_set_int(arg_value,"gateway_status",pgateway_result->gateway_status) != ZWP_STATUS_OK)
	   )
	{
		ZWP_LOG(ZWP_LOG_ERR,"Setting Gateway get result value failed");
		goto l_err_set_gw_value;
	}

	return ZWP_STATUS_OK;

l_err_get_gw_name_field:
l_err_set_gw_name_field_info_1:
l_err_set_gw_name_value:
l_err_set_gw_name_field_info_2:
l_err_set_gw_value:
		return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to list of discovered gateways
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_root_gateway_get_result
 */
static void zwp_avi_root_gateway_get_result_free(void *result)
{
	ZWP_FREE(result);
}

/** Handlers for setting the gateway */
static zwp_avi_message_t root_gateway_set =
{
		"root_gateway_set",
		zwp_avi_root_gateway_set_argument, zwp_avi_root_gateway_set_argument_free,
		zwp_avi_root_gateway_set_execute,
		NULL, NULL
};

/** Handlers for getting currently connected gateway status */
static zwp_avi_message_t root_gateway_get =
{
		"root_gateway_get",
		NULL, NULL,
		zwp_avi_root_gateway_get_execute,
		zwp_avi_root_gateway_get_result, zwp_avi_root_gateway_get_result_free
};

/** Handlers for getting list of discovered gateways */
static zwp_avi_message_t root_gateway_list =
{
		"root_gateway_list",
		zwp_avi_root_gateway_list_set_argument, zwp_avi_root_gateway_list_set_argument_free,
		zwp_avi_root_gateway_list_execute,
		zwp_avi_root_gateway_list_result, zwp_avi_root_gateway_list_result_free
};
/** Register handlers */
zwp_status_t zwp_avi_root_gateway_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_message_list_add_message(message_list, &root_gateway_set) != ZWP_STATUS_OK) ||
		(zwp_avi_message_list_add_message(message_list, &root_gateway_get) != ZWP_STATUS_OK) ||
		(zwp_avi_message_list_add_message(message_list, &root_gateway_list) != ZWP_STATUS_OK)
	   )
	{
		goto l_err_gateway_set;
	}

	return ZWP_STATUS_OK;

l_err_gateway_set:
	return ZWP_STATUS_ERROR;
}


