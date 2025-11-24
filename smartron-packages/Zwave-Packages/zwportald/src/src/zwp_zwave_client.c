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
 * @file    zwp_zwave_client.c
 *
 * @brief   Z-Wave client.
 * @details Handles Z-Wave client.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-07-23
 * - Initial version
 */

#include "zwp_zwave_client.h"
#include "zwp_error.h"
#include "zwp_memory.h"
#include "zwp_log.h"
#include "zwp_inet.h"
#include "zwp_io.h"
#include "zwp_thread_mutex.h"
#include "zwp_thread_cond.h"
#include "zwp_inet.h"
#include "zwu_number.h"
//#include "zwu_env.h"
#include "zwp_net_discovery.h"
#include "zwp_network.h"

#include "zwp_portal_define.h"

#define ZWP_ZIP_SERVER_CONFIGURATION_FILE         "zip_server.txt" 	///< ZIP Server configuration file name
#define ZWP_NET_INFO_DIR                          "networks"		///< ZIP Server configuration file directory

#define ZWP_NET_DISCOVERY_COMPLETED               0   	///<Discovery status completed
#define ZWP_NET_DISCOVERY_IN_PROGRESS             1		///<Discovery status is in progress

#define ZWP_NET_DISCOVERY_START_HANDLER_THREAD_NAME                   "ANetDiscoveryStartHdlr"

extern zwp_status_t zwp_portal_set_zip_client(zwp_portal_t portal, zwp_zwave_client_t zip_client);
static void* zwp_zwave_client_net_discover_all_handler(zwp_thread_t thread, void *arg);

// URI|port_str|key\n
#define ZIP_GW_CONF_LINE_LEN	(ZIP_GW_URI_LEN_MAX + ZIP_GW_UNSOL_RPT_PORT_SUFFIX_LEN + DTLS_PSK_KEY + 1) ///< ZIP Server configuration entry max length

/** Pointer to z-wave client current configuration */
static zwp_zwave_client_t g_client;

/* Setters section starts */
zwp_status_t zwp_zwave_client_set_net(zwp_zwave_client_t client, zwp_net_t net)
{
	if(client)
	{
		client->net = net;
		return ZWP_STATUS_OK;
	}
	return ZWP_STATUS_ERROR;
}
/* Setters section ends here */

/* Getters section starts */
zwp_net_t zwp_zwave_client_get_net(zwp_zwave_client_t client)
{
	return client->net;
}
/* Getters section ends */

/** Un-initialize the network and free the network resources */
static zwp_status_t zwp_zwave_client_kill_net(zwp_net_t net)
{
	ZWP_LOG(ZWP_LOG_DEBUG, "Network kill: Network about to be killed %p", net);

	ZWP_LOG(ZWP_LOG_DEBUG, "Network kill: Stopping network %p", net);

	if (zwp_net_stop(net) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_WARNING, "Network kill: zwp_net_stop() failed. 'net' not freed. Possible memory leak");
		goto l_err_net_stop;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Network kill: Network about to be freed %p", net);

	zwp_net_free(net);

	ZWP_LOG(ZWP_LOG_DEBUG, "Network kill: Network freed %p", net);

	return ZWP_STATUS_OK;

l_err_net_stop:
	return ZWP_STATUS_ERROR;
}

/** Low level Call back notification Z/IP Gateway network discovery completion  */
static void zwp_zwave_client_net_discovery_completion_notification(void *completion_notification_argument)
{
	zwp_zwave_client_t client;

	client = (zwp_zwave_client_t)completion_notification_argument;

	if (zwp_thread_mutex_lock(client->net_discovery_state_lock) != ZWP_STATUS_OK)
	{
		goto l_err_net_discovery_state_lock;
	}

	client->net_discovery_state = ZWP_NET_DISCOVERY_COMPLETED;

	zwp_thread_cond_broadcast(client->net_discovery_state_changed);

	zwp_thread_mutex_unlock(client->net_discovery_state_lock);

	return;

l_err_net_discovery_state_lock:
	return;
}

zwp_status_t zwp_zwave_client_net_discover(zwp_zwave_client_t client, int ip_version, int mdns, int clean)
{
	uint8_t state = ZWP_NET_DISCOVERY_TYPE_NONE;

	//Note: This state is different from client->net_discovery_state
	if (zwp_net_discovery_get_type(client->net_discovery, &state) != ZWP_STATUS_OK)
	{
		goto l_err_net_discovery_get_state;
	}

	if(state == ZWP_NET_DISCOVERY_TYPE_NONE)
	{
		if (zwp_thread_mutex_lock(client->net_discovery_state_lock) != ZWP_STATUS_OK)
		{
			goto l_err_net_discovery_state_lock;
		}

		client->net_discovery_state = ZWP_NET_DISCOVERY_IN_PROGRESS;
		if (zwp_net_discovery_start(client->net_discovery, ip_version, mdns, clean,
				zwp_zwave_client_net_discovery_completion_notification, client) != ZWP_STATUS_OK)
		{
			goto l_err_net_discovery_start;
		}

		while (!(client->net_discovery_state != ZWP_NET_DISCOVERY_IN_PROGRESS))
		{
			if (zwp_thread_cond_wait(client->net_discovery_state_changed, client->net_discovery_state_lock) != ZWP_STATUS_OK)
			{
				goto l_err_net_discovery_state_changed_wait_for_init;
			}
		}

		zwp_thread_mutex_unlock(client->net_discovery_state_lock);
	}

	return ZWP_STATUS_OK;

l_err_net_discovery_state_changed_wait_for_init:
	zwp_net_discovery_stop(client->net_discovery, ZWP_NET_DISCOVERY_NO_CLEAR_LIST);
l_err_net_discovery_start:
	zwp_thread_mutex_unlock(client->net_discovery_state_lock);
l_err_net_discovery_state_lock:
l_err_net_discovery_get_state:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_discovery_start(zwp_zwave_client_t client)
{
	zwp_thread_t thread;

	if (zwp_thread_create(&thread, zwp_zwave_client_net_discover_all_handler, client,
			client->zwclient_pool, ZWP_THREAD_CREATE_DETACHED) != ZWP_STATUS_OK)
	{
		goto l_err_thread_create;
	}

	return ZWP_STATUS_OK;

l_err_thread_create:
	return ZWP_STATUS_ERROR;
}

static void* zwp_zwave_client_net_discover_all_handler(zwp_thread_t thread, void *arg)
{
	zwp_zwave_client_t client;
	uint8_t state = ZWP_NET_DISCOVERY_TYPE_NONE;

	zwp_thread_name_set(ZWP_NET_DISCOVERY_START_HANDLER_THREAD_NAME, NULL);

	ZWP_LOG(ZWP_LOG_DEBUG, "Discovery handler start.");
	client = (zwp_zwave_client_t) arg;

	//Note: This state is different from client->net_discovery_state
	if (zwp_net_discovery_get_type(client->net_discovery, &state) != ZWP_STATUS_OK)
	{
		goto l_err_net_discovery_get_state;
	}

	if(state == ZWP_NET_DISCOVERY_TYPE_NONE)
	{
		if (zwp_thread_mutex_lock(client->net_discovery_state_lock) != ZWP_STATUS_OK)
		{
			goto l_err_net_discovery_state_lock;
		}

		client->net_discovery_state = ZWP_NET_DISCOVERY_IN_PROGRESS;

		//Start IPv4 without mdns with clear list
		if (zwp_net_discovery_start(client->net_discovery, ZWP_NET_DISCOVERY_IP_VERSION_4, ZWP_NET_DISCOVERY_WITH_NO_MDNS, ZWP_NET_DISCOVERY_CLEAR_LIST,
				zwp_zwave_client_net_discovery_completion_notification, client) !=  ZWP_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "IPv4 without MDNS discovery failed.");
			goto l_err_net_discovery_start;
		}

		while (!(client->net_discovery_state != ZWP_NET_DISCOVERY_IN_PROGRESS))
		{
			if (zwp_thread_cond_wait(client->net_discovery_state_changed, client->net_discovery_state_lock) != ZWP_STATUS_OK)
			{
				goto l_err_net_discovery_state_changed_wait_for_init;
			}
		}

		client->net_discovery_state = ZWP_NET_DISCOVERY_IN_PROGRESS;

		//Start IPv4 with mdns with clear list
		if (zwp_net_discovery_start(client->net_discovery, ZWP_NET_DISCOVERY_IP_VERSION_4, ZWP_NET_DISCOVERY_WITH_MDNS, ZWP_NET_DISCOVERY_NO_CLEAR_LIST,
				zwp_zwave_client_net_discovery_completion_notification, client) !=  ZWP_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "IPv4 with MDNS discovery failed.");
			goto l_err_net_discovery_start;
		}


		while (!(client->net_discovery_state != ZWP_NET_DISCOVERY_IN_PROGRESS))
		{
			if (zwp_thread_cond_wait(client->net_discovery_state_changed, client->net_discovery_state_lock) != ZWP_STATUS_OK)
			{
				goto l_err_net_discovery_state_changed_wait_for_init;
			}
		}

		client->net_discovery_state = ZWP_NET_DISCOVERY_IN_PROGRESS;

		//Start IPv6 without mdns with clear list
		if (zwp_net_discovery_start(client->net_discovery, ZWP_NET_DISCOVERY_IP_VERSION_6, ZWP_NET_DISCOVERY_WITH_NO_MDNS, ZWP_NET_DISCOVERY_CLEAR_LIST,
				zwp_zwave_client_net_discovery_completion_notification, client) !=  ZWP_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "IPv6 without MDNS discovery failed.");
			goto l_err_net_discovery_start;
		}


		while (!(client->net_discovery_state != ZWP_NET_DISCOVERY_IN_PROGRESS))
		{
			if (zwp_thread_cond_wait(client->net_discovery_state_changed, client->net_discovery_state_lock) != ZWP_STATUS_OK)
			{
				goto l_err_net_discovery_state_changed_wait_for_init;
			}
		}

		client->net_discovery_state = ZWP_NET_DISCOVERY_IN_PROGRESS;

		//Start IPv6 with mdns with clear list
		if (zwp_net_discovery_start(client->net_discovery, ZWP_NET_DISCOVERY_IP_VERSION_6, ZWP_NET_DISCOVERY_WITH_MDNS, ZWP_NET_DISCOVERY_NO_CLEAR_LIST,
				zwp_zwave_client_net_discovery_completion_notification, client) !=  ZWP_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "IPv6 with MDNS discovery failed.");
			goto l_err_net_discovery_start;
		}

		while (!(client->net_discovery_state != ZWP_NET_DISCOVERY_IN_PROGRESS))
		{
			if (zwp_thread_cond_wait(client->net_discovery_state_changed, client->net_discovery_state_lock) != ZWP_STATUS_OK)
			{
				goto l_err_net_discovery_state_changed_wait_for_init;
			}
		}


		zwp_thread_mutex_unlock(client->net_discovery_state_lock);
		zwp_net_discovery_clear_type(client->net_discovery);
	}

	zwp_thread_exit(thread, ZWP_STATUS_OK);

	ZWP_LOG(ZWP_LOG_DEBUG, "Discovery handler end.");

	return NULL;

l_err_net_discovery_state_changed_wait_for_init:
	zwp_net_discovery_stop(client->net_discovery, ZWP_NET_DISCOVERY_NO_CLEAR_LIST);
l_err_net_discovery_start:
	zwp_thread_mutex_unlock(client->net_discovery_state_lock);
	zwp_net_discovery_clear_type(client->net_discovery);
l_err_net_discovery_state_lock:
l_err_net_discovery_get_state:
	zwp_thread_exit(thread, ZWP_STATUS_ERROR);
	return NULL;
}

/**
 * @brief Start Z/IP discovery and get first discovered Z/IP gateway from discovery list
 * @param[in]     client            		Z-wave client object
 * @param[out]    zip_server_address 		Z/IP Gateway address
 * @param[out]    ipv4_p					Z/IP Gateway address type(IPv4 or IPv6)
 * @param[in]     zip_server_port_p         ZIP Gateway port number
 * @retval        ZWP_STATUS_OK     		Success
 * @retval        ZWP_STATUS_ERROR  		Failure
 *
 * @note
 * @par
 * This API will start discovery with IPv4 domain and returns first entry in the list if list is not empty. If IPv4 list is empty, it will
 * start IPv6 discovery and returns first entry in the list if list is not empty. Otherwise, it will return error.
 */
static zwp_status_t zwp_zwave_client_get_zip_server_by_discovery(zwp_zwave_client_t client,
		uint8_t zip_server_address[ZWP_ZIP_SERVER_ADDRESS_SIZE],
		int *ipv4_p, uint16_t *zip_server_port_p)
{
	int ipv4;
	int mdns = 0;

	ZWP_LOG(ZWP_LOG_ERR, "zwave_client: Starting discovery");

	if (zwp_zwave_client_net_discover(client, ZWP_NET_DISCOVERY_IP_VERSION_4, mdns, ZWP_NET_DISCOVERY_CLEAR_LIST) != ZWP_STATUS_OK)
	{
		goto l_err_net_discover_ipv4;
	}

	ipv4 = ZWP_NET_DISCOVERY_IP_VERSION_4;
	if (zwp_net_discovery_get_first_network(client->net_discovery, ZWP_NET_DISCOVERY_IP_VERSION_4,
			zip_server_address) != ZWP_STATUS_OK)
	{
		//Clear the previous discovery state
		zwp_net_discovery_clear_type(client->net_discovery);

		if (zwp_zwave_client_net_discover(client, ZWP_NET_DISCOVERY_IP_VERSION_6, mdns, ZWP_NET_DISCOVERY_CLEAR_LIST) != ZWP_STATUS_OK)
		{
			goto l_err_net_discover_ipv6;
		}

		zwp_net_discovery_clear_type(client->net_discovery);

		ipv4 = ZWP_NET_DISCOVERY_IP_VERSION_6;
		if (zwp_net_discovery_get_first_network(client->net_discovery, ZWP_NET_DISCOVERY_IP_VERSION_6,
				zip_server_address) != ZWP_STATUS_OK)
		{
			goto l_err_empty_network_list;
		}
	}

	*ipv4_p = ipv4;
	*zip_server_port_p = ZWP_ZIP_SERVER_DEFAULT_PORT;

	return ZWP_STATUS_OK;

l_err_empty_network_list:
l_err_net_discover_ipv6:
l_err_net_discover_ipv4:
	zwp_net_discovery_clear_type(client->net_discovery);
	return ZWP_STATUS_ERROR;
}

/**
* @brief Perform network discovery and search the given zip_server from discovery list
* @param[in]     client            		Z-wave client object
* @param[out]    gateway_cached 		A cached gateway to be checked against
* @retval        ZWP_STATUS_OK     		Successfully found the gateway through discovery
* @retval        ZWP_STATUS_ERROR  		Failure to find the gateway through discovery
*
* @note
* @par
* This API will start discovery with IPv4 domain and search for the cached gateway. If the cached gateway is not found, it will
* start IPv6 discovery and search again. If not found for both cases, it will return error.
*/
static zwp_status_t zwp_zwave_client_search_zip_server_from_discovery(zwp_zwave_client_t client, zip_gw_name_t gateway_cached)
{
	zwp_status_t ret = ZWP_STATUS_ERROR;
	int mdns = 0;
	size_t i = 0;

	zwp_net_discovery_report_t  zwp_net_discovery_report = { 0 };

	ZWP_LOG(ZWP_LOG_ERR, "zwave_client: Starting discovery ip v4");

	if (zwp_zwave_client_net_discover(client, ZWP_NET_DISCOVERY_IP_VERSION_4, mdns, ZWP_NET_DISCOVERY_CLEAR_LIST) != ZWP_STATUS_OK)
	{
		goto l_err_net_discover_ipv4;
	}

	//ipv4 = ZWP_NET_DISCOVERY_IP_VERSION_4;
	if (zwp_net_discovery_get_list(client->net_discovery, ZWP_NET_DISCOVERY_IP_VERSION_4,
		&zwp_net_discovery_report) != ZWP_STATUS_OK)
	{
		goto l_err_net_discover_ipv4_list;
	}
	//Clear the previous discovery state
	zwp_net_discovery_clear_type(client->net_discovery);

	//Compare ipv4 list with cache entry
	for (i = 0; i< zwp_net_discovery_report.gwlistlen; i++)
	{
		if (!memcmp(gateway_cached.name, &zwp_net_discovery_report.pgwlist[i*IPV4_ADDR_LEN], IPV4_ADDR_LEN))
		{
			ZWP_LOG(ZWP_LOG_ERR, "Found cached gateway entry through discovery ip v4.");
			ret = ZWP_STATUS_OK;
			goto l_err_none;
		}
	}

	//Clear zwp_net_discovery_report struct
	if (zwp_net_discovery_report.gwname)
	{
		for (i = 0; i < zwp_net_discovery_report.gwlistlen; i++)
		{
			ZWP_FREE(zwp_net_discovery_report.gwname[i]);
		}
		ZWP_FREE(zwp_net_discovery_report.gwname);
	}

	if (zwp_net_discovery_report.pgwlist)  ZWP_FREE(zwp_net_discovery_report.pgwlist);

	memset(&zwp_net_discovery_report, 0, sizeof(zwp_net_discovery_report));

	ZWP_LOG(ZWP_LOG_ERR, "zwave_client: Starting discovery ip v6");
	if (zwp_zwave_client_net_discover(client, ZWP_NET_DISCOVERY_IP_VERSION_6, mdns, ZWP_NET_DISCOVERY_CLEAR_LIST) != ZWP_STATUS_OK)
	{
		goto l_err_net_discover_ipv6;
	}

	zwp_net_discovery_clear_type(client->net_discovery);

	//ipv4 = ZWP_NET_DISCOVERY_IP_VERSION_6;

	if (zwp_net_discovery_get_list(client->net_discovery, ZWP_NET_DISCOVERY_IP_VERSION_6,
		&zwp_net_discovery_report) != ZWP_STATUS_OK)
	{
		goto l_err_empty_network_list;
	}

	//Compare ipv6 list with cache entry
	for (i = 0; i< zwp_net_discovery_report.gwlistlen; i++)
	{
		if (!memcmp(gateway_cached.name, &zwp_net_discovery_report.pgwlist[i*IPV6_ADDR_LEN], IPV6_ADDR_LEN))
		{
			ZWP_LOG(ZWP_LOG_ERR, "Found cached gateway entry through discovery ip v6.");
			ret = ZWP_STATUS_OK;
			goto l_err_none;
		}
	}

l_err_none:
	if (zwp_net_discovery_report.gwname)
	{
		for (i = 0; i < zwp_net_discovery_report.gwlistlen; i++)
		{
			ZWP_FREE(zwp_net_discovery_report.gwname[i]);
		}
		ZWP_FREE(zwp_net_discovery_report.gwname);
	}

	if (zwp_net_discovery_report.pgwlist)  ZWP_FREE(zwp_net_discovery_report.pgwlist);

	return ret;

l_err_empty_network_list:
l_err_net_discover_ipv6 :
l_err_net_discover_ipv4_list:
l_err_net_discover_ipv4 :
	zwp_net_discovery_clear_type(client->net_discovery);
	return ret;
}

/*
 * Get Z/IP server address and Port number from var/networks/zip_server.txt
 */
zwp_status_t zwp_zwave_client_get_zip_server_from_file(zwp_zwave_client_t client, char *app_var_networks_path)
{
	int ipv4;
	int32_t zip_server_port;
	char *file_path;
	char *file_content;
	char *zip_server_port_string;
	int read;
	char *line, *next_line, *cr_ptr;
	int i = 0;
	char *key_string;
	zip_gw_name_t *cached_list = client->gateway_cached_list;
	int key_len;
	uint8_t iskey = 0;


	if ((file_path = (char *)ZWP_MALLOC(strlen(app_var_networks_path) + 1
			+ sizeof(ZWP_NET_INFO_DIR) + sizeof(ZWP_ZIP_SERVER_CONFIGURATION_FILE))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_version_path_alloc;
	}
	sprintf(file_path, "%s/%s/%s", app_var_networks_path, ZWP_NET_INFO_DIR, ZWP_ZIP_SERVER_CONFIGURATION_FILE);

	if (zwp_read_from_file_path_with_dest_alloc(file_path, &file_content, &read, ((ZIP_GW_CONF_LINE_LEN * MAX_CACHED_ZIP_GW_ADDR) + 1)) != ZWP_STATUS_OK)
	{
		goto l_err_read_file;
	}

	line = next_line = file_content;
	zip_server_port = ZWP_ZIP_SERVER_DEFAULT_PORT;

	while(((line - file_content) < read) && (i < MAX_CACHED_ZIP_GW_ADDR))
	{
		if ((next_line = strstr(line, "\r\n"))
				|| (next_line = strstr(line, "\n")))
		{
			*next_line = '\0';
		}
		else
		{
			goto l_err_parsing_file;
		}
		next_line++;

		// Handle Windows CRLF
		if ((cr_ptr = strchr(line, '\r')) != NULL)
		{
			*cr_ptr = '\0';
		}

		ZWP_LOG(ZWP_LOG_DEBUG, "ZIP Server String from File: [%s]", line);

		cached_list[i].unsol_rpt_port = 0;
		cached_list[i].flag = ZIP_GW_FLAG_CACHE;
		cached_list[i].key_len = 0;
		iskey = 0;

		if ((zip_server_port_string = strchr(line, '|')))
		{
			*zip_server_port_string = '\0';
			zip_server_port_string++;

			if ((key_string = strchr(zip_server_port_string, '|')))
			{
				ZWP_LOG(ZWP_LOG_DEBUG, "key_string found: [%s]", key_string);
				*key_string = '\0';
				key_string++;
				iskey = 1;
			}

			ZWP_LOG(ZWP_LOG_DEBUG, "Zip_server_port_string: [%s]", zip_server_port_string);

			if ((zwu_parse_int32_non_negative(zip_server_port_string, &zip_server_port) != ZWP_STATUS_OK)
					|| (zwu_number_is_uint16(zip_server_port) != ZWP_STATUS_OK))
			{
				ZWP_LOG(ZWP_LOG_WARNING, "Invalid ZIP server port number. Defaulting to %u", ZWP_ZIP_SERVER_DEFAULT_PORT);
				cached_list[i].unsol_rpt_port = ZWP_ZIP_SERVER_DEFAULT_PORT;
			}
			else
			{
				cached_list[i].unsol_rpt_port = zip_server_port;
			}

			if(iskey)
			{
				key_len = strlen(key_string);

				ZWP_LOG(ZWP_LOG_DEBUG, "key_string_string: [%s] key_len = %d", key_string, key_len);

				if(!((!key_len) || (key_len%2) || (key_len > 64)))
				{
					if (!zwp_hexstring_to_bin(key_string,key_len,&cached_list[i].dtls_pskey[0] ))
					{
						ZWP_LOG(ZWP_LOG_ERR, "key_string_string: hex2bin conversion is done");
						cached_list[i].key_len = key_len/2;
					}
				}
			}

		}

		ipv4 = 1;
		if (zwp_inet_pton(line,cached_list[i].name) != ZWP_STATUS_OK)
		{
			ipv4 = 0;
			if (zwp_inet6_pton(line, cached_list[i].name) != ZWP_STATUS_OK)
			{
				goto l_err_invalid_address;
			}
		}
		cached_list[i].address_type = ipv4;

		i++;
		line = next_line;
	}

	zwp_read_dest_free(file_content);
	ZWP_FREE(file_path);

	return ZWP_STATUS_OK;

l_err_invalid_address:
l_err_parsing_file:
	zwp_read_dest_free(file_content);
l_err_read_file:
	ZWP_FREE(file_path);
l_err_version_path_alloc:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_zwave_client_get_copy_cached_list(zwp_zwave_client_t client, zip_gw_name_t *cached_list)
{
	if(!client || !cached_list)
	{
		goto l_err_invalid_input;
	}

	memcpy(cached_list, &client->gateway_cached_list[0], sizeof(client->gateway_cached_list));

	return ZWP_STATUS_OK;

l_err_invalid_input:
	return ZWP_STATUS_ERROR;
}

/*
 * Write Z/IP server address and Port number to var/networks/zip_server.txt
 */
 zwp_status_t zwp_zwave_client_set_zip_server_to_file(zwp_zwave_client_t client, char *app_var_networks_path, const zwp_zwave_client_ip_settings_t *client_ip_settings)
{
	char *file_path;
	char ip_address_string[INET6_ADDRSTRLEN + 1] = {0};
	char port[ZIP_GW_UNSOL_RPT_PORT_SUFFIX_LEN + 1] = {0};
	char content[ZIP_GW_CONF_LINE_LEN * MAX_CACHED_ZIP_GW_ADDR + 1] = {0};
	int i = 0;
	zip_gw_name_t *cached_list;
	int cached_count = 0;
	int addr_len;
	char shared_key[64+1+1] = {0};
	char *key;

	if ((file_path = (char *)ZWP_MALLOC(strlen(app_var_networks_path) + 1
			+ sizeof(ZWP_NET_INFO_DIR)	+ sizeof(ZWP_ZIP_SERVER_CONFIGURATION_FILE))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_path_alloc;
	}
	sprintf(file_path, "%s/%s/%s", app_var_networks_path, ZWP_NET_INFO_DIR, ZWP_ZIP_SERVER_CONFIGURATION_FILE);

	if (client_ip_settings->ipv4 == 1)
	{
		if (zwp_inet_ntop(client_ip_settings->zip_server_address, ip_address_string, sizeof(ip_address_string)) != ZWP_STATUS_OK)
		{
			goto l_err_ntop;
		}
		addr_len = IPV6_ADDR_LEN;
	}
	else
	{
		if (zwp_inet6_ntop(client_ip_settings->zip_server_address, ip_address_string, sizeof(ip_address_string)) != ZWP_STATUS_OK)
		{
			goto l_err_ntop;
		}
		addr_len = IPV4_ADDR_LEN;
	}

	strncat(content, ip_address_string, strlen(ip_address_string));

	sprintf(port, "|%u", client_ip_settings->unsolicited_port_number);
	strncat(content, port, strlen(port));

    ZWP_LOG(ZWP_LOG_DEBUG, "client_ip_settings->key_len  = %u", client_ip_settings->key_len );
	key = shared_key;
	if(client_ip_settings->key_len > 0)
	{
		for(i=0; i <client_ip_settings->key_len; i++)
		{
			if(i == 0)
			{
				key += sprintf(key, "|%02x", client_ip_settings->dtls_pskey[i]);
			}
			else
			{
				key += sprintf(key, "%02x", client_ip_settings->dtls_pskey[i]);
			}
		}
		ZWP_LOG(ZWP_LOG_ERR, "printed key_len  = %u",  strlen(shared_key));
		strncat(content, shared_key, strlen(shared_key));
	}

	strncat(content, "\n", 1);

	ZWP_LOG(ZWP_LOG_DEBUG, "content = %s", content);

	cached_count  = 1;

	cached_list = client->gateway_cached_list;

	for(i=0;i<MAX_CACHED_ZIP_GW_ADDR; i++)
	{
		if(cached_list[i].flag)
		{
			if(cached_count >= MAX_CACHED_ZIP_GW_ADDR)
				break;

			if(!memcmp(cached_list[i].name, client_ip_settings->zip_server_address, addr_len))
			{
				continue;
			}

			if (cached_list[i].address_type)
			{
				if (zwp_inet_ntop(cached_list[i].name, ip_address_string, sizeof(ip_address_string)) != ZWP_STATUS_OK)
				{
					continue;
				}
			}
			else
			{
				if (zwp_inet6_ntop(cached_list[i].name, ip_address_string, sizeof(ip_address_string)) != ZWP_STATUS_OK)
				{
					continue;
				}
			}

			strncat(content, ip_address_string, strlen(ip_address_string));
			if(cached_list[i].unsol_rpt_port != 0)
			{
				sprintf(port, "|%u", cached_list[i].unsol_rpt_port);
				strncat(content, port, strlen(port));
			}
			strncat(content, "\n", 1);

			cached_count++;
		}
	}

	ZWP_LOG(ZWP_LOG_INFO, "ZIP server address being written to file: %s", content);

	if (zwu_write_to_file_path(file_path, content, strlen(content)) != ZWP_STATUS_OK)
	{
		goto l_err_write_file;
	}

	ZWP_FREE(file_path);

	return ZWP_STATUS_OK;

l_err_write_file:
l_err_ntop:
	ZWP_FREE(file_path);
l_err_path_alloc:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_zwave_client_init(zwp_portal_t portal)
{
	zwp_zwave_client_t client;
	char *local_state_dir;
	zwp_zwave_client_ip_settings_t client_ip_settings;

	//Enforce singleton
	if (g_client)
	{
		ZWP_LOG(ZWP_LOG_WARNING, ZWP_MSG_SINGLETON_MULTIPLE_CALLS);
		zwp_portal_set_zip_client(portal, g_client);
		return ZWP_STATUS_OK;
	}

	if ((client = (zwp_zwave_client_t)ZWP_CALLOC(1, sizeof(*client))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_client_alloc;
	}

	client->net_discovery_state = ZWP_NET_DISCOVERY_COMPLETED;
	client->net = NULL;

	zwp_portal_set_zip_client(portal, client);

	//Create memory pool for clients
	if (zwp_pool_create(&client->zwclient_pool, NULL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create pool for clients");
		goto l_err_client_pool_create;
	}

	if(zwp_thread_mutex_create(&client->net_discovery_state_lock,  client->zwclient_pool, ZWP_THREAD_MUTEX_CREATE_NORMAL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create lock for network discovery state");
		goto l_err_net_discovery_state_lock_create;
	}

	if (zwp_thread_cond_create(&client->net_discovery_state_changed, client->zwclient_pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for network initialized state");
		goto l_err_net_discovery_state_cond_create;
	}

	if (zwp_net_discovery_init(&client->net_discovery) != ZWP_STATUS_OK)
	{
		goto l_err_net_discovery_init;
	}

	client->zip_server_selected = 1;
	local_state_dir = zwp_portal_get_local_state_dir(portal);
	memset(&client_ip_settings, 0, sizeof(client_ip_settings));

	//Reads up to 5 (max) IPs from cached files and store in client->gateway_cached_list
	if (zwp_zwave_client_get_zip_server_from_file(client, local_state_dir) != ZWP_STATUS_OK)
	{
		if (zwp_zwave_client_get_zip_server_by_discovery(client, &client_ip_settings.zip_server_address[0], &client_ip_settings.ipv4,  &client_ip_settings.unsolicited_port_number) != ZWP_STATUS_OK)
		{
			client->zip_server_selected = 0;
		}
		else
		{
			if (zwp_zwave_client_set_zip_server_to_file(client, local_state_dir, &client_ip_settings) != ZWP_STATUS_OK)
			{
				ZWP_LOG(ZWP_LOG_WARNING, "Failed to write ZIP server address to file");
			}
			else
			{
				zwp_zwave_client_get_zip_server_from_file(client, local_state_dir);
			}
			client->ipv4 = client_ip_settings.ipv4;
			memcpy(client->zip_server_address, client_ip_settings.zip_server_address, sizeof(client->zip_server_address));
			client->unsolicited_port_number = client_ip_settings.unsolicited_port_number;
		}

		zwp_net_discovery_clear_type(client->net_discovery);
	}
	else
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Z/IP Gateway cache entry found. Checking the first Z/IP Gateway entry is accessible through discovery.");
		ZWP_LOG(ZWP_LOG_DEBUG, "Checking Z/IP Gateway:");
		zwp_print_ip(client->gateway_cached_list[0].name, client->gateway_cached_list[0].address_type);
		if (zwp_zwave_client_search_zip_server_from_discovery(client, client->gateway_cached_list[0]) != ZWP_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_DEBUG, "Cannot find Z/IP Gateway cache entry from discovery. Try again in 2s.");

			sleep(2);

			if (zwp_zwave_client_search_zip_server_from_discovery(client, client->gateway_cached_list[0]) != ZWP_STATUS_OK)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Cannot find Z/IP Gateway cache entry from discovery. Z/IP Gateway not initialize.");
				client->zip_server_selected = 0;
			}
		}
		else
		{
			ZWP_LOG(ZWP_LOG_DEBUG, "Z/IP Gateway to be initialized with caching entries");

			memcpy(client->zip_server_address, client->gateway_cached_list[0].name, sizeof(client->zip_server_address));
			client->unsolicited_port_number = client->gateway_cached_list[0].unsol_rpt_port;
			client->ipv4 = client->gateway_cached_list[0].address_type;
			client->key_len = client->gateway_cached_list[0].key_len;
			memcpy(client->dtls_pskey, client->gateway_cached_list[0].dtls_pskey, sizeof(client->dtls_pskey));
		}
	}

	if (client->zip_server_selected)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Z/IP Gateway to be initialized: ");
		zwp_print_ip(client->zip_server_address, client->ipv4);
		ZWP_LOG(ZWP_LOG_DEBUG, "Port: %u", client->unsolicited_port_number);

		if (zwp_portal_get_client_intialize_lock(portal) != ZWP_STATUS_OK)
		{
			goto l_err_client_intialize_lock;
		}
		//Initialize the network if GW is found in the list
		if (zwp_portal_client_spawn_net(portal, NULL, &client->net) != ZWP_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "portal_client_spawn failed");
			goto l_err_spawn_net;
		}

		zwp_portal_put_client_intialize_lock(portal);
	}
	else
	{
		ZWP_LOG(ZWP_LOG_WARNING, "No ZIP server selected for initialization");
	}

	g_client = client;
	zwp_portal_set_zip_client(portal, client);

	return ZWP_STATUS_OK;

	if (client->zip_server_selected)
	{
		zwp_zwave_client_kill_net(client->net);
	}
l_err_spawn_net:
	zwp_portal_put_client_intialize_lock(portal);
l_err_client_intialize_lock:
	zwp_net_discovery_exit(client->net_discovery);
l_err_net_discovery_init:
	zwp_thread_cond_destroy(client->net_discovery_state_changed);
l_err_net_discovery_state_cond_create:
	zwp_thread_mutex_destroy(client->net_discovery_state_lock);
l_err_net_discovery_state_lock_create:
	zwp_pool_destroy(client->zwclient_pool);
l_err_client_pool_create:
	ZWP_FREE(client);
	g_client = NULL;
l_err_client_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_zwave_client_exit(zwp_zwave_client_t client)
{
	zwp_net_discovery_exit(client->net_discovery);
	zwp_thread_cond_destroy(client->net_discovery_state_changed);
	zwp_thread_mutex_destroy(client->net_discovery_state_lock);

	ZWP_FREE(client);
	g_client = NULL;
}



