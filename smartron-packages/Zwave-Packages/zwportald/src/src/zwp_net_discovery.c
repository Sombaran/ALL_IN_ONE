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
 * @file    zwp_net_discovery.c
 *
 * @brief   Z-Wave network discovery.
 * @details Handles Z-Wave network discovery.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-07-23
 * - Initial version
 * @version 1.1 - 2014-11-03
 * - Changes made to adapt to zwportal local client changes.
 */

#include <stdio.h>

#include "zwp_net_discovery.h"
#include "zwp_error.h"
#include "zwp_memory.h"
#include "zwp_log.h"
#include "zwp_thread_mutex.h"
#include "zwp_inet.h"

/**
 * @brief Network Discovery class
 */
struct _zwp_net_discovery
{
	zwp_thread_mutex_t     net_discovery_lock;                	/**< Mutex for network discovery */

	zwp_net_discovery_completion_notification_t
	                       completion_notification_handler;   	/**< Notification for discovery completion */
	void *                 completion_notification_argument;  	/**< Argument for discovery completion notification */

	void *                 net_discovery_context;            	/**< HCAPI context for network discovery */

	uint8_t *              network_ipv4;                      	/**< List of IPv4 networks */
	size_t                 network_ipv4_count;                	/**< Count of IPv4 networks */
	char **				   gw_name_ipv4_gwv2;					/**< List of MDNS names corresponding to IPV4 networks */


	uint8_t *              network_ipv6;                      	/**< List of IPv6 networks */
	size_t                 network_ipv6_count;                	/**< Count of IPv6 networks */
	char **				   gw_name_ipv6_gwv2;					/**< List of MDNS names corresponding to IPV6 networks */

	zwp_pool_t			   discovery_pool;					  	/**< Memory pool for discovery */

	uint8_t				   discovery_type;					  	/**< Current discovery type (ZWP_NET_DISCOVERY_TYPE_XXXX) */
	uint16_t			   received_reports;					/**< Number of discovery reports that are received so far */
	uint16_t 			   total_reports;						/**< Total number of discovery reports that will be delivered */

};

/** Pointer to current discovery configuration */
static zwp_net_discovery_t g_discovery;

zwp_status_t zwp_net_discovery_get_first_network(zwp_net_discovery_t discovery, int ip_version,
		uint8_t zip_server_address[ZWP_ZIP_SERVER_ADDRESS_SIZE])
{
	uint8_t *network_list;
	size_t network_count;
	size_t network_address_size;

	if (zwp_thread_mutex_lock(discovery->net_discovery_lock) != ZWP_STATUS_OK)
	{
		goto l_err_discovery_lock;
	}

	if (ip_version == ZWP_NET_DISCOVERY_IP_VERSION_4)
	{
		network_list = discovery->network_ipv4;
		network_count = discovery->network_ipv4_count;
		network_address_size = IPV4_ADDR_LEN;
	}
	else
	{
		network_list = discovery->network_ipv6;
		network_count = discovery->network_ipv6_count;
		network_address_size = IPV6_ADDR_LEN;
	}

	if (network_count == 0)
	{
		goto l_err_empty_list;
	}

	memcpy(zip_server_address, network_list, network_address_size);

	zwp_thread_mutex_unlock(discovery->net_discovery_lock);

	return ZWP_STATUS_OK;

l_err_empty_list:
	zwp_thread_mutex_unlock(discovery->net_discovery_lock);
l_err_discovery_lock:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_net_discovery_get_list(zwp_net_discovery_t discovery, int ip_version,
		zwp_net_discovery_report_t *zwp_net_discovery_report)
{
	uint8_t *gw_list=NULL;
	size_t network_address_size;
	int i = 0;


	if (zwp_thread_mutex_lock(discovery->net_discovery_lock) != ZWP_STATUS_OK)
	{
		goto l_err_discovery_lock;
	}

	if ((ip_version == ZWP_NET_DISCOVERY_IP_VERSION_4) && (discovery->network_ipv4_count > 0))
	{
		network_address_size = discovery->network_ipv4_count * IPV4_ADDR_LEN;

		if((gw_list = (uint8_t *)ZWP_CALLOC(1, network_address_size)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_gw_list_alloc;
		}
		memcpy(gw_list, discovery->network_ipv4,  network_address_size);
		zwp_net_discovery_report->gwlistlen= discovery->network_ipv4_count;
		zwp_net_discovery_report->pgwlist = gw_list;

		if ((zwp_net_discovery_report->gwname = (char **)ZWP_CALLOC(1, (discovery->network_ipv4_count * sizeof(char *)))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_calloc_name;
		}

		for(i=0;i<discovery->network_ipv4_count ;i++)
		{
			if(discovery->gw_name_ipv4_gwv2[i])
			{
				zwp_net_discovery_report->gwname[i] = strdup(discovery->gw_name_ipv4_gwv2[i]);
			}
			else
				zwp_net_discovery_report->gwname[i] = NULL;
		}
	}
	else if ((ip_version == ZWP_NET_DISCOVERY_IP_VERSION_6) && (discovery->network_ipv6_count > 0))
	{
		network_address_size = discovery->network_ipv6_count * IPV6_ADDR_LEN;
		if((gw_list = (uint8_t *)ZWP_CALLOC(1, network_address_size)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_gw_list_alloc;
		}
		memcpy(gw_list, discovery->network_ipv6,  network_address_size);
		zwp_net_discovery_report->gwlistlen= discovery->network_ipv6_count;
		zwp_net_discovery_report->pgwlist = gw_list;

		if ((zwp_net_discovery_report->gwname = (char **)ZWP_CALLOC(1, (discovery->network_ipv6_count * sizeof(char *)))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_calloc_name;
		}

		for(i=0;i<discovery->network_ipv6_count ;i++)
		{
			if(discovery->gw_name_ipv6_gwv2[i])
				zwp_net_discovery_report->gwname[i] = strdup(discovery->gw_name_ipv6_gwv2[i]);
			else
				zwp_net_discovery_report->gwname[i] = NULL;
		}
	}
	else
	{
		zwp_net_discovery_report->gwlistlen = 0;
		ZWP_LOG(ZWP_LOG_ERR, "discovery get list is ZERO");
	}

	zwp_net_discovery_report->received_reports = discovery->received_reports;
	zwp_net_discovery_report->total_reports = discovery->total_reports;
	zwp_net_discovery_report->type = discovery->discovery_type;

	zwp_thread_mutex_unlock(discovery->net_discovery_lock);

	return ZWP_STATUS_OK;

l_err_calloc_name:
	ZWP_FREE(zwp_net_discovery_report->pgwlist);
l_err_gw_list_alloc:
	zwp_thread_mutex_unlock(discovery->net_discovery_lock);
l_err_discovery_lock:
	return ZWP_STATUS_ERROR;
}


zwp_status_t zwp_net_discovery_clear_type(zwp_net_discovery_t discovery)
{
	if (zwp_thread_mutex_lock(discovery->net_discovery_lock) != ZWP_STATUS_OK)
	{
		goto l_err_discovery_lock;
	}

	ZWP_LOG(ZWP_LOG_ERR, "discovery state ..cleared");
	discovery->discovery_type = ZWP_NET_DISCOVERY_TYPE_NONE;
	zwp_thread_mutex_unlock(discovery->net_discovery_lock);

	return ZWP_STATUS_OK;

l_err_discovery_lock:
	return ZWP_STATUS_ERROR;
}


zwp_status_t zwp_net_discovery_get_type(zwp_net_discovery_t discovery, uint8_t *pType)
{
	if(!pType)
	{
		goto l_err_invalid_input;
	}

	if (zwp_thread_mutex_lock(discovery->net_discovery_lock) != ZWP_STATUS_OK)
	{
		goto l_err_discovery_lock;
	}
	*pType = discovery->discovery_type;
	zwp_thread_mutex_unlock(discovery->net_discovery_lock);

	return ZWP_STATUS_OK;

l_err_discovery_lock:
l_err_invalid_input:
	return ZWP_STATUS_ERROR;
}

/** Top level network discovery completion call back */
static void zwp_net_discovery_notify_completion(zwp_net_discovery_t discovery)
{
	if (discovery->completion_notification_handler)
	{
		discovery->completion_notification_handler(discovery->completion_notification_argument);
		discovery->completion_notification_handler = NULL;

		discovery->completion_notification_argument = NULL;
	}
}

/** HC API discovery notification call back */
static void ZWP_HCAPI_NOTIFICATION zwp_net_discovery_notification(uint8_t *gw_addr, uint8_t gw_cnt, int ipv4, void *usr_param,
                                     int rpt_num, int total_rpt, char **gw_name)
{
	zwp_net_discovery_t discovery;
	uint8_t *network_list;
	uint8_t *tmp = NULL;
	size_t size;
	size_t additional_size;
	char ip_address_string[INET6_ADDRSTRLEN + 1];
	int i;
	char **gw_name_handle;

	discovery = (zwp_net_discovery_t)usr_param;


	ZWP_LOG(ZWP_LOG_ERR, "Network  MDNS = %u: ipv4 = %u gw count:%u Report: %d/%d ", ((gw_name)? 1: 0), ipv4, gw_cnt, rpt_num, total_rpt);

	if(!gw_cnt)
	{
		if (rpt_num >= total_rpt)
		{
			zwp_net_discovery_notify_completion(discovery);
		}

		return;
	}

	if (zwp_thread_mutex_lock(discovery->net_discovery_lock) != ZWP_STATUS_OK)
	{
		goto l_err_discovery_lock;
	}


	if (ipv4 == ZWP_NET_DISCOVERY_IP_VERSION_4)
	{
		ZWP_LOG(ZWP_LOG_ERR, "discovery->network_ipv4_count: %u", discovery->network_ipv4_count);

		network_list = discovery->network_ipv4;
		size = discovery->network_ipv4_count * IPV4_ADDR_LEN;

		additional_size = gw_cnt * IPV4_ADDR_LEN;

		if ((tmp = (uint8_t *)ZWP_REALLOC(network_list, (size + additional_size))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_realloc_ip;
		}
		memcpy(tmp + size, gw_addr, additional_size);
		discovery->network_ipv4 = tmp;
		if ((gw_name_handle = (char **)ZWP_REALLOC(discovery->gw_name_ipv4_gwv2, ((discovery->network_ipv4_count+gw_cnt) * sizeof(char *)))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_realloc_name;
		}

		discovery->gw_name_ipv4_gwv2 = gw_name_handle;

		for(i=0;i<gw_cnt;i++)
		{
			if(!gw_name)
			{
				discovery->gw_name_ipv4_gwv2[discovery->network_ipv4_count + i] = NULL;
			}
			else
			{
				discovery->gw_name_ipv4_gwv2[discovery->network_ipv4_count + i] = strdup(gw_name[i]);
				ZWP_LOG(ZWP_LOG_ERR, "Discovered IPv4 MDNS name: %s", discovery->gw_name_ipv4_gwv2[discovery->network_ipv4_count + i]);
			}
		}

		discovery->network_ipv4_count += gw_cnt;
		discovery->discovery_type =  ZWP_NET_DISCOVERY_TYPE_IPV4;
	}
	else if (ipv4 == ZWP_NET_DISCOVERY_IP_VERSION_6)
	{
		ZWP_LOG(ZWP_LOG_ERR, "discovery->network_ipv6_count: %u", discovery->network_ipv6_count);

		network_list = discovery->network_ipv6;
		size = discovery->network_ipv6_count * IPV6_ADDR_LEN;

		additional_size = gw_cnt * IPV6_ADDR_LEN;

		if ((tmp = (uint8_t *)ZWP_REALLOC(network_list, (size + additional_size))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_realloc_ip;
		}

		memcpy(tmp + size, gw_addr, additional_size);
		discovery->network_ipv6 = tmp;

		if ((gw_name_handle = (char **)ZWP_REALLOC(discovery->gw_name_ipv6_gwv2, ((discovery->network_ipv6_count+gw_cnt) * sizeof(char *)))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_realloc_name;
		}

		discovery->gw_name_ipv6_gwv2 = gw_name_handle;

		for(i=0;i<gw_cnt;i++)
		{
			if(!gw_name)
			{
				discovery->gw_name_ipv6_gwv2[discovery->network_ipv6_count + i] = NULL;
			}
			else
			{
				discovery->gw_name_ipv6_gwv2[discovery->network_ipv6_count + i] = strdup(gw_name[i]);
				ZWP_LOG(ZWP_LOG_INFO, "Discovered IPv6 MDNS name: %s", discovery->gw_name_ipv6_gwv2[discovery->network_ipv6_count + i]);
			}
		}
		discovery->network_ipv6_count += gw_cnt;
		discovery->discovery_type =  ZWP_NET_DISCOVERY_TYPE_IPV6;
	}

	discovery->received_reports = rpt_num;
	discovery->total_reports = total_rpt;


	zwp_thread_mutex_unlock(discovery->net_discovery_lock);

	if (rpt_num >= total_rpt)
	{
		zwp_net_discovery_notify_completion(discovery);
	}


	//===============Debug start ======================================
	if (ipv4 == ZWP_NET_DISCOVERY_IP_VERSION_4)
	{
		for (i = 0; i < gw_cnt; i++)
		{
			if (zwp_inet_ntop(gw_addr + (i * IPV4_ADDR_LEN), ip_address_string, sizeof(ip_address_string)) == ZWP_STATUS_OK)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Discovered: %s", ip_address_string);
			}
		}
	}
	else
	{
		for (i = 0; i < gw_cnt; i++)
		{
			if (zwp_inet6_ntop(gw_addr + (i * IPV6_ADDR_LEN), ip_address_string, sizeof(ip_address_string)) == ZWP_STATUS_OK)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Discovered: %s", ip_address_string);
			}
		}
	}
   //==============Debug End ================


	return;

l_err_realloc_name:
l_err_realloc_ip:
	zwp_thread_mutex_unlock(discovery->net_discovery_lock);
l_err_discovery_lock:
	if (rpt_num >= total_rpt)
	{
		zwp_net_discovery_notify_completion(discovery);
	}
	return;
}

/**
 * Free the previously discovered discovery list
 * @note Caller must hold lock
 */
static void zwp_net_discovery_list_free(zwp_net_discovery_t discovery, int ip_version, int clean)
{
	int i;
	if((ip_version == ZWP_NET_DISCOVERY_IP_VERSION_4) && clean)
	{
		if(discovery->gw_name_ipv4_gwv2)
		{
			for(i=0;i<discovery->network_ipv4_count; i++)
			{
				//Freeing NULL pointer is OK
				ZWP_FREE(discovery->gw_name_ipv4_gwv2[i]);
			}
			ZWP_FREE(discovery->gw_name_ipv4_gwv2);
		}

		ZWP_FREE(discovery->network_ipv4);
		discovery->network_ipv4 = NULL;
		discovery->network_ipv4_count = 0;
		discovery->gw_name_ipv4_gwv2 =  NULL;
	}
	else if((ip_version == ZWP_NET_DISCOVERY_IP_VERSION_6) && clean)
	{
		if(discovery->gw_name_ipv6_gwv2)
		{
			for(i=0;i<discovery->network_ipv6_count; i++)
			{
				//Freeing NULL pointer is OK
				ZWP_FREE(discovery->gw_name_ipv6_gwv2[i]);
			}
			ZWP_FREE(discovery->gw_name_ipv6_gwv2);
		}
		ZWP_FREE(discovery->network_ipv6);
		discovery->network_ipv6 = NULL;
		discovery->network_ipv6_count = 0;
		discovery->gw_name_ipv6_gwv2 =  NULL;
	}
}


zwp_status_t zwp_net_discovery_start(zwp_net_discovery_t discovery, int ip_version, int mdns, int clear_list,
		zwp_net_discovery_completion_notification_t completion_notification_handler,
		void *completion_notification_argument)
{

	ZWP_LOG(ZWP_LOG_DEBUG, "Discovery (re)start. Waiting. mdns = %d ip_version = %d", mdns, ip_version);

	if (zwp_thread_mutex_lock(discovery->net_discovery_lock) != ZWP_STATUS_OK)
	{
		goto l_err_discovery_lock;
	}

	if (discovery->net_discovery_context)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Discovery stop");

		zwnet_gw_discvr_stop(discovery->net_discovery_context);
	}
	discovery->net_discovery_context = NULL;

	zwp_net_discovery_list_free(discovery, ip_version, clear_list);

	discovery->completion_notification_handler = completion_notification_handler;
	discovery->completion_notification_argument = completion_notification_argument;

	ZWP_LOG(ZWP_LOG_DEBUG, "Discovery start");

	discovery->total_reports = 0;
	discovery->received_reports = 0;

	if(ip_version)
	{
		discovery->discovery_type = ((mdns) ?  ZWP_NET_DISCOVERY_TYPE_IPV4_MDNS : ZWP_NET_DISCOVERY_TYPE_IPV4);
	}
	else
	{
		discovery->discovery_type = ((mdns) ?  ZWP_NET_DISCOVERY_TYPE_IPV6_MDNS : ZWP_NET_DISCOVERY_TYPE_IPV6);
	}

	if ((discovery->net_discovery_context = zwnet_gw_discvr_start(zwp_net_discovery_notification,
			discovery, ip_version, mdns)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Discovery start failed");
		goto l_err_discovery_start;
	}

	zwp_thread_mutex_unlock(discovery->net_discovery_lock);

	return ZWP_STATUS_OK;

l_err_discovery_start:
	discovery->discovery_type =  ZWP_NET_DISCOVERY_TYPE_NONE;
	discovery->total_reports = 0;
	discovery->received_reports = 0;
	zwp_thread_mutex_unlock(discovery->net_discovery_lock);
l_err_discovery_lock:
	return ZWP_STATUS_ERROR;
}

void zwp_net_discovery_stop(zwp_net_discovery_t discovery, int clear_list)
{
	ZWP_LOG(ZWP_LOG_DEBUG, "Discovery stop. Waiting");

	if (zwp_thread_mutex_lock(discovery->net_discovery_lock) != ZWP_STATUS_OK)
	{
		goto l_err_discovery_lock;
	}

	if (discovery->net_discovery_context)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Discovery stop");

		zwnet_gw_discvr_stop(discovery->net_discovery_context);
	}
	discovery->net_discovery_context = NULL;

	zwp_net_discovery_list_free(discovery, ZWP_NET_DISCOVERY_IP_VERSION_4, clear_list);
	zwp_net_discovery_list_free(discovery, ZWP_NET_DISCOVERY_IP_VERSION_6, clear_list);
	zwp_thread_mutex_unlock(discovery->net_discovery_lock);

	return;

	zwp_thread_mutex_unlock(discovery->net_discovery_lock);
l_err_discovery_lock:
	return;
}

zwp_status_t zwp_net_discovery_init(zwp_net_discovery_t *discovery_p)
{
	zwp_net_discovery_t discovery;

	//Enforce singleton
	if (g_discovery)
	{
		ZWP_LOG(ZWP_LOG_WARNING, ZWP_MSG_SINGLETON_MULTIPLE_CALLS);
		*discovery_p = g_discovery;

		return ZWP_STATUS_OK;
	}

	if ((discovery = (zwp_net_discovery_t)ZWP_MALLOC(sizeof(*discovery))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_discovery_alloc;
	}

	//Create memory pool for clients
	if (zwp_pool_create(&discovery->discovery_pool, NULL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create pool for clients");
		goto l_err_client_pool_create;
	}

	discovery->net_discovery_context = NULL;

	discovery->completion_notification_handler = NULL;
	discovery->completion_notification_argument = NULL;

	discovery->network_ipv4 = NULL;
	discovery->network_ipv4_count = 0;
	discovery->network_ipv6 = NULL;
	discovery->network_ipv6_count = 0;


	discovery->gw_name_ipv4_gwv2 = NULL;
	discovery->gw_name_ipv6_gwv2 = NULL;

	discovery->received_reports = 0;
	discovery->total_reports = 0;
	discovery->discovery_type =  ZWP_NET_DISCOVERY_TYPE_NONE;

	ZWP_LOG(ZWP_LOG_DEBUG, "Discovery init");

	if (zwp_thread_mutex_create(&discovery->net_discovery_lock, discovery->discovery_pool, ZWP_THREAD_MUTEX_CREATE_NORMAL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create lock for network discovery");
		goto l_err_discovery_lock_create;
	}

	*discovery_p = discovery;

	return ZWP_STATUS_OK;

	zwp_thread_mutex_destroy(discovery->net_discovery_lock);
l_err_discovery_lock_create:
	zwp_pool_destroy(discovery->discovery_pool);
l_err_client_pool_create:
	ZWP_FREE(discovery);
	g_discovery = NULL;
l_err_discovery_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_net_discovery_exit(zwp_net_discovery_t discovery)
{
	ZWP_LOG(ZWP_LOG_DEBUG, "Discovery exit");

	zwp_net_discovery_stop(discovery, ZWP_NET_DISCOVERY_CLEAR_LIST);

	zwp_thread_mutex_destroy(discovery->net_discovery_lock);
	zwp_pool_destroy(discovery->discovery_pool);
	ZWP_FREE(discovery);
	g_discovery = NULL;
}

