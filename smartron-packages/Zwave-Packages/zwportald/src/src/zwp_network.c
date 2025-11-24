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
 * @file    zwp_network.c
 *
 * @brief   Z-Wave Network module
 * @details Handles network level of Z-Wave network tree.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-06-11
 * - Initial version
 */

#include <string.h>
#include <sys/stat.h>

#include "zwp_log.h"
#if defined(ZWP_DEBUG_NETWORK)
#include "zwp_log_enable.h"
#else
#include "zwp_log_disable.h"
#endif

#include "zwp_portal.h"
#include "zwp_network.h"
#include "zwp_descriptor.h"
#include "zwp_db.h"
#include "zwp_error.h"
#include "zwp_memory.h"
#include "zwp_queue.h"
#include "zwp_thread_mutex.h"
#include "zwp_thread_cond.h"
#include "zwp_avi_service.h"
#include "zwp_avi_service_networks.h"
#include "zwp_avi_service_nodes.h"
#include "zwp_avi_service_endpoints.h"
#include "zwp_avi_service_interfaces.h"
#include "zwp_avi_message_networks.h"
#include "zwp_avi_message_nodes.h"
#include "zwp_avi_message_endpoints.h"
#include "zwp_avi_message_interfaces.h"
#ifdef LOCAL_CLIENT_ENABLE
#include "zwp_io.h"
#include "zwp_inet.h"
#endif

/** Directory name for network information persistence */
#define ZWP_NET_INFO_DIR                                    "networks"
/** File extension for network information persistence files */
#define ZWP_NET_INFO_EXTENSION                              "net"

/** Network start handler thread name */
#define ZWP_NET_START_HANDLER_THREAD_NAME                   "ANetStartHdlr"
/** Network stop hanlder thread name */
#define ZWP_NET_STOP_HANDLER_THREAD_NAME                    "ANetStopHdlr"
/** Network operation status handler thread name */
#define ZWP_NET_OPERATION_STATUS_HANDLER_THREAD_NAME        "ANetOpStatusHdlr"
#ifdef LOCAL_CLIENT_ENABLE
#define LOCAL_CLIENT_TIMEZONE_PATH	"/etc/timezone"
#endif
/**
 * @brief Z-Wave network configuration class
 */
struct _zwp_net_config
{
#if 0
	struct
	{
		int                 file_descriptor;          /**< File descriptor to connect to Z/IP gateway/router using TLS */
		void                *SSL_object;              /**< SSL object pointer to connect to Z/IP gateway/router using TLS */
		clnt_prof_t         profile;                  /**< Portal configuration profile for the Z/IP gateway */
	} portal_client;

	zwnet_p                 zwnet;                    /**< Z-Wave network */
#endif
};



/**
* @brief Z-Wave whitelist list class
*/
typedef struct
{
	int						wl_dev_count;	/**< Number of devices in the wl_dev_list */
	zwp_net_wl_dev_info_t*  wl_dev_list;    /**< For type=WL_INFO_TYPE_LOC; device location in UTF-8 encoding with restrictions same as name field. */
} zwp_net_wl_list_t;

/**
 * @brief Z-Wave network class
 */
struct _zwp_net
{
	zwp_pool_t                   net_pool;                          /**< Memory pool at Network level */

	struct
	{
		int                      file_descriptor;                   /**< File descriptor to connect to Z/IP gateway/router using TLS */
		void                     *SSL_object;                       /**< SSL object pointer to connect to Z/IP gateway/router using TLS */
		clnt_prof_t              profile;                           /**< Portal configuration profile for the Z/IP gateway */
	} portal_client;                                                /**< Portal client conffiguration */

	volatile int                 net_initialize_state;              /**< Network initialization state - ZWP_NET_INITIALIZE_STATE_XXX */
	zwp_thread_mutex_t           net_initialize_state_lock;         /**< Mutex lock for network initialization state */
	zwp_thread_cond_t            net_initialize_state_changed;      /**< Conditional variable indicating state change */

	zwp_thread_t                 net_start_handler;                 /**< Network initialization handler thread */

	zwp_queue_t                  net_operation_status_queue;        /**< Network operation status queue */
	zwp_thread_t                 net_operation_status_handler;      /**< Network operation status handler thread*/

	zwp_net_operation_state_t    net_operation_state;               /**< Network operation state */

	zwp_desc_net_t               net_desc;                          /**< Descriptor tree at the Network level */
	zwp_thread_mutex_t           net_desc_container_lock;           /**< Mutex lock for network descriptor container */
	volatile int                 net_transmit_status;               /**< Transmit status notified by HCAPI */
	zwp_thread_cond_t            net_transmit_status_changed;       /**< Conditional variable indicating transmit state change */

	zwnoded_t                    node_added_desc;                   /**< Descriptor of last added node */
	zwnoded_t                    node_replaced_desc;                /**< Descriptor of last replaced node */
	zwnoded_t                    node_updated_desc;                 /**< Descriptor of last updated node */

	sec2_add_cb_prm_t			 node_added_s2_param;				/**< Security 2 parameter for the joining node */
	char *						 ctrl_local_dsk;					/**< Security 2 local node DSK (as per last checked) */

	zwu_array_list_t			 net_whitelist_list;				/**< Network whitelist table. Each element is a zwp_net_wl_dev_info_t  */
	zwp_thread_mutex_t           net_whitelist_lock;				/**< Mutex lock for network whitelist */

	zwnet_sts_t					 node_prekit_info;					/**< Prekit information for the joining node */

	zwnet_init_t                 net_init;                          /**< Network initialization parameters */
	zwnet_p                      zwnet;                             /**< Z-Wave network */

	char                         *net_info_dir;                    /**< Directory path for network information persistence */

	volatile int                 reference_count;                   /**< Reference count */
	zwp_thread_mutex_t           reference_count_lock;              /**< Mutex lock for network */
	zwp_thread_cond_t            reference_count_zeroed;            /**< Conditional variable indicating ref count becoming zero */

	zwp_portal_t                 portal;                            /**< Back pointer to portal */
    hcaal_ctx_t                  hcaal_ctx;                         /**< HCAPI AAL context */
#ifndef LOCAL_CLIENT_ENABLE
	pzwp_db_db					 net_ifttt_db_p;					/**< IFTTT database pointer (trigger database) */
	pzwp_db_table				 net_ifttt_table_trigger_generic_p; /**< IFTTT database table pointer for generic trigger table */
#endif
    char                         timezone[ZWSCN_LEN_CLIENT_TIMEZONE];/**< Time zone of the network */
};

/** Network operation status */
typedef struct
{
	uint8_t operation;                  /**< Network operation - ZWNET_OP_XXX */
	uint16_t status;                    /**< Status of current operation */
	zwnet_sts_t *info;					/**< additional information for the specified op and sts; NULL if there is no additional info */
} *zwp_net_operation_status_t;

static zwp_status_t zwp_net_desc_init(zwp_net_t net);
static zwp_status_t zwp_net_desc_exit(zwp_net_t net);

/**
 * @name Getters
 * @{
 */
zwp_portal_t zwp_net_get_portal(zwp_net_t net)
{
	return net->portal;
}

hcaal_ctx_p zwp_net_get_hcaal(zwp_net_t net)
{
    return &net->hcaal_ctx;
}

char *zwp_net_get_timezone(zwp_net_t net)
{
    return net->timezone;
}

/**
* @brief Get homeid from network object
* @param[in]     net               Network object
* @param[out]    pHomeID           Home id
* @return        1 - value home id. 0 - no home id
*/
int	zwp_net_get_homeid(zwp_net_t net, uint32_t *pHomeID)
{
	zwnetd_p  zwnet_desc;

	if ((zwnet_desc = zwnet_get_desc(net->zwnet)) == NULL)
	{
		return 0;
	}

	*pHomeID = zwnet_desc->id;

	return 1;

}

#ifdef LOCAL_CLIENT_ENABLE
zwp_status_t zwp_net_get_system_timezone(char **timezone)
{

	char *timezone_filepath = LOCAL_CLIENT_TIMEZONE_PATH;
	char *file_content;
	int read = 0;
	char *time_zone_str;
	char *next_line;

	if (zwp_read_from_file_path_with_dest_alloc(timezone_filepath, &file_content, &read, 0) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Reading %s Failed. Scenes won't work.", LOCAL_CLIENT_TIMEZONE_PATH);
		goto l_err_read_file;
	}

	if(read  > 0)
	{
		if ((time_zone_str = ZWP_CALLOC(1,read+1)) == NULL)
		{
			goto l_err_timezone_malloc;
		}

		if ((next_line = strstr(file_content, "\r\n"))
			|| (next_line = strstr(file_content, "\n")))
		{
				*next_line = '\0';
		}

		strcpy(time_zone_str, file_content);
		*timezone = time_zone_str;

		ZWP_LOG(ZWP_LOG_ERR, "Using time zone = %s ", *timezone);
	}
	else
		goto l_err_invalid_timezone;

	ZWP_FREE(file_content);

	return ZWP_STATUS_OK;

l_err_timezone_malloc:
l_err_invalid_timezone:
	ZWP_FREE(file_content);
l_err_read_file:
	return ZWP_STATUS_ERROR;
}

/**
zwp_net_get_controller_node_id - Get controller node id;
@param[in]	net				Network
@param[out]	nodeid_p		node id of z/ip controller
@return	Non-zero if o.k.; else return zero
*/
int zwp_net_get_controller_node_id(zwp_net_t net, uint8_t* nodeid_p)
{
    zwnetd_p  zwnet_desc;

	if ((zwnet_desc = zwnet_get_desc(net->zwnet)) == NULL)
	{
		return 0;
	}

	*nodeid_p = zwnet_desc->ctl_id;

	return 1;
}

int zwp_net_unsolicited_set_to_local(zwp_net_t net) {
	uint16_t dst_port;
	uint8_t local_addr[16];
	uint8_t local_addr_ipv6[16];
	char	local_addr_str[INET6_ADDRSTRLEN];
	uint8_t nodeid = 0;
	zwifd_p desc_interface_p;
	int result;

	memset(local_addr, 0, sizeof(local_addr));

	if (!zwp_net_get_controller_node_id(net, &nodeid))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Getting controller node id failed.");
		return 0;
	}

	if (zwp_desc_get_desc_interface(net->net_desc, nodeid, 0, COMMAND_CLASS_ZIP_GATEWAY, &desc_interface_p) == ZWP_STATUS_OK)
	{
		if (zwnet_local_addr_get(net->zwnet, net->net_init.zip_router, local_addr, net->net_init.use_ipv4) == 0)
		{
			dst_port = zwnet_listen_port_get(net->zwnet);
			//ZWP_LOG(ZWP_LOG_DEBUG, "address=%d %d %d %d port=%d\n", local_addr[12], local_addr[13], local_addr[14], local_addr[15], dst_port);
			zwnet_ip_ntoa(local_addr, local_addr_str, sizeof(local_addr_str), net->net_init.use_ipv4);
			//ZWP_LOG(ZWP_LOG_DEBUG,"SET2LOCAL: address=%s port=%d\n", local_addr_str, dst_port);
			if (zwp_inet6_pton(local_addr_str, local_addr_ipv6) == ZWP_STATUS_OK)
			{
				//ZWP_LOG(ZWP_LOG_DEBUG, "SET2LOCAL: calling zwif_gw_unsolicit_set\n");
				result = zwif_gw_unsolicit_set(desc_interface_p, local_addr_ipv6, dst_port);
				if (result != 0)
				{
					ZWP_LOG(ZWP_LOG_ERR, "Setting unsolicited IP failed.");
					return 0;
				}
			}
			else
			{
				ZWP_LOG(ZWP_LOG_ERR, "IPv6 pton conversion failed");
			}
		}
	}

	return 1;
}


#endif

/**
zwp_net_get_ctx_persistent - Get context persistent string for scene module
@param[in]	net				Network
@param[in]	len				Length of context persistent string buffer
@param[out]	ctx_persistent	Context persistent string buffer
@return	Non-zero if o.k.; else return zero
*/
int zwp_net_get_ctx_persistent(zwp_net_t net, char *ctx_persistent, int len)
{
    zwnetd_p  zwnet_desc;

	if (!ctx_persistent || (len < 25))
	{
		return 0;
	}

	if ((zwnet_desc = zwnet_get_desc(net->zwnet)) == NULL)
	{
		return 0;
	}

	snprintf(ctx_persistent, len,
			 "%02X%02X%02X%02X%02X%02X%02X%02X%08X",
			net->portal_client.profile.clnt_id[0], net->portal_client.profile.clnt_id[1],
			net->portal_client.profile.clnt_id[2], net->portal_client.profile.clnt_id[3],
			net->portal_client.profile.clnt_id[4], net->portal_client.profile.clnt_id[5],
			net->portal_client.profile.clnt_id[6], net->portal_client.profile.clnt_id[7],
			zwnet_desc->id);

	return 1;
}

/**
zwp_net_get_last_rplc_node - Get last replaced node id
@param[in]	net				Network
@return	Replaced node id
*/
#ifdef LOCAL_CLIENT_ENABLE
const uint8_t local_clnt_id [8] = {1,2,3,4,5,6,7,8};
#endif
uint8_t zwp_net_get_last_rplc_node(zwp_net_t net)
{
    return net->node_replaced_desc.nodeid;
}


const void *zwp_net_get_hash_key(zwp_net_t net)
{

#ifdef LOCAL_CLIENT_ENABLE
	return local_clnt_id;
#else
	return net->portal_client.profile.clnt_id;
#endif
}

ssize_t zwp_net_get_hash_key_size(zwp_net_t net)
{
#ifdef LOCAL_CLIENT_ENABLE
	return sizeof local_clnt_id;
#else
	return sizeof net->portal_client.profile.clnt_id;
#endif

}

const clnt_prof_t *zwp_net_get_portal_client_profile(zwp_net_t net)
{
	return &net->portal_client.profile;
}

zwp_pool_t zwp_net_get_network_pool(zwp_net_t net)
{
	return net->net_pool;
}

zwnet_init_p zwp_net_get_zwnet_init(zwp_net_t net)
{
	return &net->net_init;
}

zwnet_p zwp_net_get_zwnet(zwp_net_t net)
{
	return net->zwnet;
}

zwp_desc_net_t zwp_net_get_desc_net(zwp_net_t net)
{
	return net->net_desc;
}

zwu_time_t zwp_net_get_network_utime(zwp_net_t net)
{
	if (net && net->net_desc)
	{
		return zwp_desc_get_network_utime(net->net_desc);
	}
	else
	{
		return 0;
	}
}

void zwp_net_get_operation_state(zwp_net_t net, zwp_net_operation_state_t *operation_state)
{
	memcpy(operation_state, &net->net_operation_state, sizeof(*operation_state));
}

int zwp_net_get_req_key(zwp_net_t net, sec2_keys_req_cb_prm_t *preq_key)
{
	if (net->net_operation_state.current_operation == ZWNET_OP_ADD_NODE ||
		net->net_operation_state.current_operation == ZWNET_OP_RP_NODE)
	{
		if (net->net_operation_state.current_operation_status == OP_ADD_NODE_SEC_REQ_KEY_READY ||
			net->net_operation_state.current_operation_status == OP_ADD_NODE_ON_BEHALF_SEC_REQ_KEY_READY)
		{
			if(net->node_added_s2_param.cb_type == S2_CB_TYPE_REQ_KEY)
			{
				*preq_key = net->node_added_s2_param.cb_prm.req_key;
				return 1;
			}
		}
	}

	return 0;
}

int zwp_net_get_dsk(zwp_net_t net, sec2_dsk_cb_prm_t *pdsk_prm)
{
	if (net->net_operation_state.current_operation == ZWNET_OP_ADD_NODE ||
		net->net_operation_state.current_operation == ZWNET_OP_RP_NODE)
	{
		if (net->net_operation_state.current_operation_status == OP_ADD_NODE_SEC_DSK_READY ||
			net->net_operation_state.current_operation_status == OP_ADD_NODE_ON_BEHALF_SEC_DSK_READY)
		{
			if (net->node_added_s2_param.cb_type == S2_CB_TYPE_DSK)
			{

				if ((pdsk_prm->dsk = ZWP_STRDUP(net->node_added_s2_param.cb_prm.dsk.dsk)) == NULL)
					return 0;
				pdsk_prm->pin_required = net->node_added_s2_param.cb_prm.dsk.pin_required;
				return 1;
			}
		}
	}

	return 0;
}

int zwp_net_get_prekit_node_dsk(zwp_net_t net, zwnet_sts_t *pprekit_dsk)
{
	if (net->net_operation_state.current_operation == ZWNET_OP_ADD_NODE)
	{
		if (net->net_operation_state.current_operation_status == OP_ADD_NODE_PROTOCOL_START)
		{
			if (net->node_prekit_info.type == ZWNET_STS_INFO_PREKIT_START)
			{
				memcpy(pprekit_dsk, &net->node_prekit_info, sizeof(zwnet_sts_t));
				return 1;
			}
		}
	}

	return 0;
}

int zwp_net_get_local_node_dsk(zwp_net_t net, char **ppdsk)
{
	//Currently only allow getting the DSK during the network initiate process
	if (ppdsk &&
		(net->net_operation_state.current_operation == ZWNET_OP_INITIATE) &&
		(net->net_operation_state.current_operation_status == OP_INI_SEC_OWN_DSK_READY))
	{
		if (((*ppdsk) = ZWP_STRDUP(net->ctrl_local_dsk)) == NULL)
			return 0;

		return 1;
	}

	return 0;
}

int zwp_net_get_report_wait_timeout(zwp_net_t net)
{
	return zwp_portal_get_report_wait_timeout(net->portal);
}
#ifndef LOCAL_CLIENT_ENABLE
pzwp_db_db zwp_net_get_ifttt_db(zwp_net_t net)
{
	return net->net_ifttt_db_p;
}

pzwp_db_table zwp_net_get_ifttt_table_trigger_generic(zwp_net_t net)
{
	return net->net_ifttt_table_trigger_generic_p;
}
#endif

#ifdef LOCAL_CLIENT_ENABLE
void zwp_net_get_zip_server_addr(zwp_net_t net, zwp_net_zip_server_config_t *server_config)
{
    memcpy(server_config->zip_server_address, net->net_init.zip_router, 16);
    server_config->ipv4 = net->net_init.use_ipv4;
    server_config->unsolicited_port_number =  net->net_init.host_port;
}
#endif
/**
 * @}
 */

/**
 * @name Setters
 * @{
 */
void zwp_net_set_operation(zwp_net_t net, uint8_t operation)
{
	net->net_operation_state.previous_operation = net->net_operation_state.current_operation;
	net->net_operation_state.current_operation = operation;
	net->net_operation_state.current_operation_status = 0;
	net->net_operation_state.current_operation_total_nodes = 0;
	net->net_operation_state.current_operation_completed_nodes = 0;
}

void zwp_net_set_node_updated_desc(zwp_net_t net, zwnoded_t node_desc)
{
	memcpy(&net->node_updated_desc, &node_desc, sizeof(net->node_updated_desc));
}

#ifdef LOCAL_CLIENT_ENABLE
void zwp_net_set_zip_serverconfig(zwp_net_t net, zwp_net_zip_server_config_t *server_cofig)
{
	net->net_init.host_port = server_cofig->unsolicited_port_number;
	net->net_init.use_ipv4 = server_cofig->ipv4;
    memcpy(net->net_init.zip_router, server_cofig->zip_server_address,  sizeof(net->net_init.zip_router) );
	net->net_init.dtls_psk_len = server_cofig->key_len;
	memcpy(net->net_init.dtls_psk, server_cofig->dtls_pskey, sizeof(net->net_init.dtls_psk));
}
#endif

#ifndef LOCAL_CLIENT_ENABLE
/**
* @brief Set IFTTT "generic trigger" database table pointer to network object
* @param[in]     net						Network object
* @param[in]     ptable_trigger_generic		IFTTT "generic trigger" database table pointer
*/
void zwp_net_set_ifttt_table_trigger_generic(zwp_net_t net, pzwp_db_table ptable_trigger_generic)
{
	net->net_ifttt_table_trigger_generic_p = ptable_trigger_generic;
}
#endif

/**
 * @}
 */

zwp_status_t zwp_net_abort_operation(zwp_net_t net, zwp_status_t status)
{
	if (status == ZWP_STATUS_OK)
	{
		net->net_operation_state.previous_operation = net->net_operation_state.current_operation;
		net->net_operation_state.current_operation = ZWNET_OP_NONE;
		net->net_operation_state.current_operation_status = ZWP_NET_OPERATION_STATUS_ABORTED;
	}
	else
	{
		net->net_operation_state.current_operation = ZWNET_OP_NONE;
	}

	net->net_operation_state.current_operation_total_nodes = 0;
	net->net_operation_state.current_operation_completed_nodes = 0;

	zwp_desc_net_exit(net->net_desc);
	net->net_desc = NULL;

	if (zwp_desc_net_init(&net->net_desc, net) != ZWP_STATUS_OK)
	{
		goto l_err_desc_net_init;
	}

	return ZWP_STATUS_OK;

l_err_desc_net_init:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_net_reference_count_increment(zwp_net_t net)
{
	if (zwp_thread_mutex_lock(net->reference_count_lock) != ZWP_STATUS_OK)
	{
		goto l_err_reference_count_lock;
	}

	net->reference_count++;

	zwp_thread_mutex_unlock(net->reference_count_lock);

	return ZWP_STATUS_OK;

l_err_reference_count_lock:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_net_reference_count_decrement(zwp_net_t net)
{
	if (zwp_thread_mutex_lock(net->reference_count_lock) != ZWP_STATUS_OK)
	{
		goto l_err_reference_count_lock;
	}

	if (net->reference_count > 0)
	{
		net->reference_count--;
	}

	if (net->reference_count <= 0)
	{
		zwp_thread_cond_broadcast(net->reference_count_zeroed);
	}

	zwp_thread_mutex_unlock(net->reference_count_lock);

	return ZWP_STATUS_OK;

l_err_reference_count_lock:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Wait for network reference count to become zero
 * @param[in]     net               Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_net_reference_count_wait_for_zero(zwp_net_t net)
{
	if (zwp_thread_mutex_lock(net->reference_count_lock) != ZWP_STATUS_OK)
	{
		goto l_err_reference_count_lock;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Network reference count: %d", net->reference_count);

	while (!(net->reference_count == 0))
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Waiting for network reference count to become zero");

		if (zwp_thread_cond_wait(net->reference_count_zeroed, net->reference_count_lock) != ZWP_STATUS_OK)
		{
			goto l_err_reference_count_wait_for_zero;
		}
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "All references to network released");

	zwp_thread_mutex_unlock(net->reference_count_lock);

	return ZWP_STATUS_OK;

l_err_reference_count_wait_for_zero:
l_err_reference_count_lock:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_net_desc_container_lock_and_test(zwp_net_t net)
{
	if (zwp_thread_mutex_lock(net->net_desc_container_lock) != ZWP_STATUS_OK)
	{
		goto l_err_net_desc_lock;
	}

	if (net->net_desc == NULL)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Descriptor container tree is NULL. Network is not initialized.");
		goto l_err_empty_desc;
	}

	return ZWP_STATUS_OK;

l_err_empty_desc:
	zwp_thread_mutex_unlock(net->net_desc_container_lock);
l_err_net_desc_lock:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_net_desc_container_test(zwp_net_t net)
{
	if (net->net_desc == NULL)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Descriptor container tree is NULL. Network is not initialized.");
		goto l_err_empty_desc;
	}

	return ZWP_STATUS_OK;

l_err_empty_desc:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_net_desc_container_lock(zwp_net_t net)
{
	if (zwp_thread_mutex_lock(net->net_desc_container_lock) != ZWP_STATUS_OK)
	{
		goto l_err_net_desc_lock;
	}

	return ZWP_STATUS_OK;

l_err_net_desc_lock:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_net_desc_container_unlock(zwp_net_t net)
{
	return zwp_thread_mutex_unlock(net->net_desc_container_lock);
}

void zwp_net_transmit_wait_init(zwp_net_t net)
{
	net->net_transmit_status = ZWP_NET_TRANSMIT_STATUS_NONE;
}

zwp_status_t zwp_net_transmit_wait(zwp_net_t net, uint8_t *transmit_status)
{
	// Check condition to handle spurious signal
	while (!((net->net_desc == NULL) || (net->net_transmit_status != ZWP_NET_TRANSMIT_STATUS_NONE)))
	{
		if (zwp_thread_cond_wait(net->net_transmit_status_changed, net->net_desc_container_lock) != ZWP_STATUS_OK)
		{
			goto l_err_net_trasnsmit_status_wait_for_change;
		}
	}

	if (net->net_desc == NULL)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Descriptor container tree is NULL. Network is not initialized.");
		goto l_err_empty_desc;
	}

	*transmit_status = net->net_transmit_status;

	return ZWP_STATUS_OK;

l_err_empty_desc:
l_err_net_trasnsmit_status_wait_for_change:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_net_report_receive_wait(zwp_net_t net, zwp_thread_cond_t report_receive_condition, zwp_interval_time_t timeout)
{
	return zwp_thread_cond_timedwait(report_receive_condition, net->net_desc_container_lock, timeout);
}

zwp_status_t zwp_net_acquire_desc_interface(zwifd_p ifd, zwifd_p *desc_interface_p)
{
	zwp_net_t net;

	net = zwnet_get_desc(ifd->net)->user;

	if (zwp_net_desc_container_lock_and_test(net) != ZWP_STATUS_OK)
	{
		goto l_err_net_desc_lock;
	}

	if (zwp_desc_get_desc_interface(net->net_desc, ifd->nodeid, ifd->epid, ifd->cls, desc_interface_p) != ZWP_STATUS_OK)
	{
		goto l_err_get_desc_interface;
	}

	return ZWP_STATUS_OK;

l_err_get_desc_interface:
	zwp_net_desc_container_unlock(net);
l_err_net_desc_lock:
	return ZWP_STATUS_ERROR;
}

void zwp_net_release_desc_interface(zwifd_p ifd)
{
	zwp_net_t net;

	net = zwnet_get_desc(ifd->net)->user;

	zwp_net_desc_container_unlock(net);
}

/*
 * @note   This leaves the descriptor container lock locked. Needs to be paired with zwp_net_release_desc_node() to unlock
 */
zwp_status_t zwp_net_acquire_desc_node(zwnoded_p noded, zwnoded_p *desc_node_p)
{
	zwp_net_t net;

	net = zwnet_get_desc(noded->net)->user;

	if (zwp_net_desc_container_lock_and_test(net) != ZWP_STATUS_OK)
	{
		goto l_err_net_desc_lock;
	}

	if (zwp_desc_get_desc_node(net->net_desc, noded->nodeid, desc_node_p) != ZWP_STATUS_OK)
	{
		goto l_err_get_desc_node;
	}

	return ZWP_STATUS_OK;

l_err_get_desc_node:
	zwp_net_desc_container_unlock(net);
l_err_net_desc_lock:
	return ZWP_STATUS_ERROR;
}

void zwp_net_release_desc_node(zwnoded_p noded)
{
	zwp_net_t net;

	net = zwnet_get_desc(noded->net)->user;

	zwp_net_desc_container_unlock(net);
}

/*
 * @note 'net' can be NULL
 */
zwp_status_t zwp_net_execute_avi_message_networks(zwp_net_t net, zwp_service_networks_t service_net)
{
	if (zwp_avi_service_networks_execute_avi_messages(service_net, net) != ZWP_STATUS_OK)
	{
		goto l_err_execute;
	}

	return ZWP_STATUS_OK;

l_err_execute:
	return ZWP_STATUS_ERROR;
}

#if 0
static int ZWP_HCAPI_NOTIFICATION zwp_device_config_find(void *device_configuration_context,
		struct _dev_rec_srch_key *search_key, struct _dev_record *device_record)
{
	ZWP_LOG(ZWP_LOG_DEBUG, "Notification for finding device configuration: vendor id '%u'; product type '%u'; product id '%u'",
			search_key->vid, search_key->ptype, search_key->pid);

	return 0;
}
#endif

/**
 * @brief Notification handler for network transmit status
 * @param[in]     user               Network object of type @a zwp_net_t
 * @param[in]     transmit_status    Network transmit status
 * @return        Nothing
 */
static void ZWP_HCAPI_NOTIFICATION zwp_net_transmit_notification(void *user, uint8_t transmit_status)
{
	zwp_net_t net;

	net = (zwp_net_t)user;

	ZWP_LOG(ZWP_LOG_DEBUG, "Notification for network data transmit status '%s'", zwp_hcapi_net_transmit_status_to_string(transmit_status));

	if (zwp_net_desc_container_lock(net) != ZWP_STATUS_OK)
	{
		goto l_err_net_desc_lock;
	}

	net->net_transmit_status = transmit_status;

	zwp_thread_cond_broadcast(net->net_transmit_status_changed);

	zwp_net_desc_container_unlock(net);

l_err_net_desc_lock:
	return;
}
#ifndef LOCAL_CLIENT_ENABLE
/**
 * @brief Notification handler for network error
 * @param[in]     user               Network object of type @a zwp_net_t
 * @return        Nothing
 */
static void ZWP_HCAPI_NOTIFICATION zwp_net_error_notification(void *user)
{
	zwp_net_t net;

	net = (zwp_net_t)user;

	ZWP_LOG(ZWP_LOG_ERR, "Notification for network error: %p", user);

	zwp_portal_client_kill_net_async(net->portal, net, 1);

	ZWP_LOG(ZWP_LOG_ERR, "Notification for network error returned: %p", user);
}
#endif

/**
 * @brief Notification handler for node addition and removal
 * @param[in]     user               Network object of type @a zwp_net_t
 * @param[in]     noded              Node descriptor of node that is added or removed
 * @param[in]     mode               0 means node added, 1 means node removed, 2 means node updated
 * @return        Nothing
 */
static void ZWP_HCAPI_NOTIFICATION zwp_net_node_list_notification(void *user, zwnoded_p noded, int mode)
{
	zwp_net_t net;

	net = (zwp_net_t)user;

	ZWP_LOG(ZWP_LOG_DEBUG, "Notification for node '%s'", zwp_hcapi_net_node_status(mode));

	ZWP_LOG(ZWP_LOG_DEBUG, "Current operation '%s'",
			zwp_hcapi_net_operation_to_string(net->net_operation_state.current_operation));

	if (zwp_thread_mutex_lock(net->net_initialize_state_lock) != ZWP_STATUS_OK)
	{
		goto l_err_net_initialize_state_lock;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Acquired net->net_initialize_state_lock");

	if (net->net_initialize_state != ZWP_NET_INITIALIZE_STATE_INITIALIZED)
	{
		goto l_err_net_initialize_not_completed;
	}

	if (zwp_net_desc_container_lock_and_test(net) != ZWP_STATUS_OK)
	{
		goto l_err_net_desc_lock;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Passed zwp_net_desc_container_lock_and_test");

	if (mode == ZWNET_NODE_ADDED)
	{
		memcpy(&net->node_added_desc, noded, sizeof(net->node_added_desc));
		// Defer desc node addition till network operation status notification
	}
	else if (mode == ZWNET_NODE_REMOVED)
	{
		// Effect desc node removal immediately
		if (zwp_desc_remove_node(net->net_desc, noded->nodeid) != ZWP_STATUS_OK)
		{
			goto l_err_remove_node;
		}
	}
	else if (mode == ZWNET_NODE_UPDATED)
	{
		memcpy(&net->node_replaced_desc, noded, sizeof(net->node_replaced_desc));
		// Defer desc node replacement till network operation status notification
	}
	else if ((mode == ZWNET_NODE_STATUS_ALIVE) || (mode == ZWNET_NODE_STATUS_DOWN) || (mode == ZWNET_NODE_STATUS_SLEEP))
	{
		if (zwp_desc_refresh_node(net->net_desc, noded->nodeid) != ZWP_STATUS_OK)
		{
			goto l_err_refresh_node;
		}
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Before zwp_net_desc_container_unlock");

	zwp_net_desc_container_unlock(net);

	ZWP_LOG(ZWP_LOG_DEBUG, "Before unlock net->net_initialize_state_lock");

	zwp_thread_mutex_unlock(net->net_initialize_state_lock);

	ZWP_LOG(ZWP_LOG_DEBUG, "Released net->net_initialize_state_lock");

	return;

l_err_refresh_node:
l_err_remove_node:
	zwp_net_desc_container_unlock(net);
l_err_net_desc_lock:
l_err_net_initialize_not_completed:
	zwp_thread_mutex_unlock(net->net_initialize_state_lock);
l_err_net_initialize_state_lock:
	return;
}

void ZWP_HCAPI_NOTIFICATION add_node_sec2(void *usr_param, sec2_add_cb_prm_t *cb_param)
{
	zwp_net_operation_status_t   net_operation_status;
	zwp_net_t net = (zwp_net_t)usr_param;
	int status = 0;
	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_START, NULL);

	ZWP_LOG(ZWP_LOG_INFO, "HCAPI security 2 inclusion callback - cb_type = %d", cb_param->cb_type);

	if (net)
	{
		if (net->node_added_s2_param.cb_type == S2_CB_TYPE_DSK)
		{
			if (net->node_added_s2_param.cb_prm.dsk.dsk)
				ZWP_FREE(net->node_added_s2_param.cb_prm.dsk.dsk);
		}

		if (cb_param->cb_type == S2_CB_TYPE_REQ_KEY)
		{
			net->node_added_s2_param.cb_type = cb_param->cb_type;
			net->node_added_s2_param.cb_prm.req_key = cb_param->cb_prm.req_key;

			status = OP_ADD_NODE_SEC_REQ_KEY_READY;		//defined a 'fake' intermediate status for the client to get the key
		}
		else if (cb_param->cb_type == S2_CB_TYPE_DSK)
		{
			net->node_added_s2_param.cb_type = cb_param->cb_type;

			if ((net->node_added_s2_param.cb_prm.dsk.dsk = ZWP_STRDUP(cb_param->cb_prm.dsk.dsk)) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				goto l_err_dup_string;
			}
			net->node_added_s2_param.cb_prm.dsk.pin_required = cb_param->cb_prm.dsk.pin_required;

			status = OP_ADD_NODE_SEC_DSK_READY;			//defined a 'fake' intermediate status for the client to get the key
		}

		if ((net_operation_status = (zwp_net_operation_status_t)ZWP_CALLOC(1, sizeof(*net_operation_status))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_net_operation_status_alloc;
		}

		net_operation_status->operation = ZWNET_OP_ADD_NODE;
		net_operation_status->status = status;
		net_operation_status->info = NULL;

		ZWP_LOG(ZWP_LOG_DEBUG, "Notification queue: Waiting to Push: %p", net_operation_status);
		if ((zwp_queue_push(net->net_operation_status_queue, net_operation_status)) != ZWP_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to push network operation status notification into the queue");
			goto l_err_queue_push;
		}
		ZWP_LOG(ZWP_LOG_DEBUG, "Notification queue: Pushed");

		ZWP_LOG(ZWP_LOG_INFO, "Network operation '%s' in progress with intermediate status '%s'",
			zwp_hcapi_net_operation_to_string(net->net_operation_state.current_operation),
			zwp_hcapi_net_operation_intermediate_status_to_string(net->net_operation_state.current_operation, status));
	}

	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_END, NULL);
	return;

l_err_queue_push:
	ZWP_FREE(net_operation_status);
l_err_net_operation_status_alloc:
l_err_dup_string:
	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_END, NULL);
	return;
}

void ZWP_HCAPI_NOTIFICATION zwp_net_add_node_sec2_unsolicited_notification(void *usr_param, sec2_add_cb_prm_t *cb_param)
{
	zwp_net_operation_status_t   net_operation_status;
	zwp_net_t net = (zwp_net_t)usr_param;
	int status = 0;
	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_START, NULL);

	ZWP_LOG(ZWP_LOG_INFO, "HCAPI security 2 unsolicited inclusion callback - cb_type = %d", cb_param->cb_type);

	if (net)
	{
		if (net->node_added_s2_param.cb_type == S2_CB_TYPE_DSK)
		{
			if (net->node_added_s2_param.cb_prm.dsk.dsk)
				ZWP_FREE(net->node_added_s2_param.cb_prm.dsk.dsk);
		}

		if (cb_param->cb_type == S2_CB_TYPE_REQ_KEY)
		{
			net->node_added_s2_param.cb_type = cb_param->cb_type;
			net->node_added_s2_param.cb_prm.req_key = cb_param->cb_prm.req_key;

			status = OP_ADD_NODE_ON_BEHALF_SEC_REQ_KEY_READY;		//defined a 'fake' intermediate status for the client to get the key
		}
		else if (cb_param->cb_type == S2_CB_TYPE_DSK)
		{
			net->node_added_s2_param.cb_type = cb_param->cb_type;

			if ((net->node_added_s2_param.cb_prm.dsk.dsk = ZWP_STRDUP(cb_param->cb_prm.dsk.dsk)) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				goto l_err_dup_string;
			}
			net->node_added_s2_param.cb_prm.dsk.pin_required = cb_param->cb_prm.dsk.pin_required;

			status = OP_ADD_NODE_ON_BEHALF_SEC_DSK_READY;			//defined a 'fake' intermediate status for the client to get the key
		}

		//Set add node operation here as previously there is no 'add node' request.
		//This is the unsolicited "add node"
		zwp_net_set_operation(net, ZWNET_OP_ADD_NODE);

		if ((net_operation_status = (zwp_net_operation_status_t)ZWP_CALLOC(1, sizeof(*net_operation_status))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_net_operation_status_alloc;
		}

		net_operation_status->operation = ZWNET_OP_ADD_NODE;
		net_operation_status->status = status;
		net_operation_status->info = NULL;

		ZWP_LOG(ZWP_LOG_DEBUG, "Notification queue: Waiting to Push: %p", net_operation_status);
		if ((zwp_queue_push(net->net_operation_status_queue, net_operation_status)) != ZWP_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to push network operation status notification into the queue");
			goto l_err_queue_push;
		}
		ZWP_LOG(ZWP_LOG_DEBUG, "Notification queue: Pushed");

		ZWP_LOG(ZWP_LOG_INFO, "Network operation '%s' in progress with intermediate status '%s'",
			zwp_hcapi_net_operation_to_string(net->net_operation_state.current_operation),
			zwp_hcapi_net_operation_intermediate_status_to_string(net->net_operation_state.current_operation, status));
	}

	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_END, NULL);
	return;

l_err_queue_push:
	ZWP_FREE(net_operation_status);
l_err_net_operation_status_alloc:
l_err_dup_string:
	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_END, NULL);
	return;
}


void ZWP_HCAPI_NOTIFICATION get_local_node_dsk(void *usr_ctx, char *dsk)
{
	zwp_net_operation_status_t   net_operation_status;
	zwp_net_t net = (zwp_net_t)usr_ctx;
	int status;
	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_START, NULL);

	if (net)
	{
		if (net->ctrl_local_dsk != NULL)
		{
			ZWP_FREE(net->ctrl_local_dsk);
		}

		if (dsk)
		{
			if ((net->ctrl_local_dsk = ZWP_STRDUP(dsk)) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				goto l_err_dup_dsk;
			}
			status = OP_INI_SEC_OWN_DSK_READY;			//defined a 'fake' intermediate status for the client to get the DSK
			
			if ((net_operation_status = (zwp_net_operation_status_t)ZWP_CALLOC(1, sizeof(*net_operation_status))) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				goto l_err_net_operation_status_alloc;
			}

			net_operation_status->operation = ZWNET_OP_INITIATE;
			net_operation_status->status = status;
			net_operation_status->info = NULL;

			ZWP_LOG(ZWP_LOG_DEBUG, "Notification queue: Waiting to Push: %p", net_operation_status);
			if ((zwp_queue_push(net->net_operation_status_queue, net_operation_status)) != ZWP_STATUS_OK)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to push network operation status notification into the queue");
				goto l_err_queue_push;
			}
			ZWP_LOG(ZWP_LOG_DEBUG, "Notification queue: Pushed");

			ZWP_LOG(ZWP_LOG_INFO, "Network operation '%s' in progress with intermediate status '%s'",
				zwp_hcapi_net_operation_to_string(net->net_operation_state.current_operation),
				zwp_hcapi_net_operation_intermediate_status_to_string(net->net_operation_state.current_operation, status));

		}
	}

	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_END, NULL);
	return;

l_err_queue_push:
	ZWP_FREE(net_operation_status);
l_err_net_operation_status_alloc:
l_err_dup_dsk:
	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_END, NULL);
	return;
}

void zwp_net_whitelist_remove(zwp_net_wl_dev_info_t *pwl_dev_info)
{
	if (pwl_dev_info)
	{
		if (pwl_dev_info->dsk)
			ZWP_FREE(pwl_dev_info->dsk);

		ZWP_FREE(pwl_dev_info);
	}
}

void zwp_net_whitelist_remove_all(zwp_net_t net, zwu_array_list_t whitelist_list)
{
	int wl_count;
	int i;
	zwp_net_wl_dev_info_t *pwl_dev_info = NULL;

	if (zwp_thread_mutex_lock(net->net_whitelist_lock) != ZWP_STATUS_OK)
	{
		goto l_err_net_whitelist_lock;
	}

	wl_count = zwu_array_list_size(whitelist_list);

	for (i = 0; i < wl_count; i++)
	{
		pwl_dev_info = zwu_array_list_get(whitelist_list, i);

		zwp_net_whitelist_remove(pwl_dev_info);
	}

	zwu_array_list_remove_all(whitelist_list);

	zwp_thread_mutex_unlock(net->net_whitelist_lock);

l_err_net_whitelist_lock:
	return;
}

zwp_status_t zwp_net_create_whitelist_dev(char* dsk, zwp_net_wl_dev_info_t **ppwl_dev_info)
{
	zwp_net_wl_dev_info_t *pwl_dev_info = NULL;
	zwp_status_t res = ZWP_STATUS_ERROR;

	if (dsk[0] != '\0')
	{
		if ((pwl_dev_info = (zwp_net_wl_dev_info_t *)ZWP_CALLOC(1, sizeof(zwp_net_wl_dev_info_t))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, "zwp_net_wl_dev_info_t memory creation fail.");
			res = ZWP_STATUS_ENOMEM;
			goto l_err_memory;
		}

		pwl_dev_info->dsk = ZWP_STRDUP(dsk);

		*ppwl_dev_info = pwl_dev_info;

		res = ZWP_STATUS_OK;
	}


l_err_memory:
	return res;
}

zwp_status_t zwp_net_find_a_whitelist_dev(zwp_net_t net, char* dsk, zwp_net_wl_dev_info_t **ppwl_dev_info)
{
	int wl_count;
	int i;
	zwp_net_wl_dev_info_t *pwl_dev_info = NULL;
	zwu_array_list_t whitelist_list;
	zwp_status_t res = ZWP_STATUS_ERROR;

	whitelist_list = net->net_whitelist_list;

	if (zwp_thread_mutex_lock(net->net_whitelist_lock) != ZWP_STATUS_OK)
	{
		goto l_err_net_whitelist_lock;
	}

	wl_count = zwu_array_list_size(whitelist_list);

	for (i = 0; i < wl_count; i++)
	{
		pwl_dev_info = zwu_array_list_get(whitelist_list, i);

		if (pwl_dev_info)
		{
			if (!strcmp(pwl_dev_info->dsk, dsk))
			{
				//Found matching dsk;
				*ppwl_dev_info = pwl_dev_info;
				res = ZWP_STATUS_OK;
				break;
			}

		}
	}

	zwp_thread_mutex_unlock(net->net_whitelist_lock);

l_err_net_whitelist_lock:

	return res;
}

void ZWP_HCAPI_NOTIFICATION zwp_net_whitelist_list_notification(void *usr_ctx, wl_dsk_lst_t *dsk_list)
{
	zwp_net_t net = (zwp_net_t)usr_ctx;
	zwp_net_wl_dev_info_t *pwl_dev_info = NULL;
	int i;

	//Clear the old list first
	zwp_net_whitelist_remove_all(net, net->net_whitelist_list);

	if (dsk_list && dsk_list->dsk_cnt > 0)
	{
		for (i = 0; i < dsk_list->dsk_cnt; i++)
		{
			if(zwp_net_create_whitelist_dev(dsk_list->dsk_lst[i], &pwl_dev_info) != ZWP_STATUS_OK)
			{
				ZWP_LOG(ZWP_LOG_ERR, "zwp_net_wl_dev_info_t structure creation fail. DSK: %s", dsk_list->dsk_lst[i]);
				goto l_err_dev_creation;
			}

			if (zwp_thread_mutex_lock(net->net_whitelist_lock) != ZWP_STATUS_OK)
			{
				goto l_err_net_whitelist_lock;
			}

			if (zwu_array_list_add(net->net_whitelist_list, pwl_dev_info) != ZWP_STATUS_OK)
			{
				goto l_err_whitelist_list_add;
			}

			zwp_thread_mutex_unlock(net->net_whitelist_lock);
		}
	}

	return;

l_err_whitelist_list_add:
	zwp_thread_mutex_unlock(net->net_whitelist_lock);
l_err_net_whitelist_lock:
	zwp_net_whitelist_remove(pwl_dev_info);
l_err_dev_creation:
	zwp_net_whitelist_remove_all(net, net->net_whitelist_list);
	return;
}

void ZWP_HCAPI_NOTIFICATION zwp_net_whitelist_dev_info_notification(void *usr_ctx, char *dsk, uint8_t grant_keys, wl_info_t *info, uint8_t info_cnt)
{
	zwp_net_t net = (zwp_net_t)usr_ctx;
	zwp_net_wl_dev_info_t *pwl_dev_info = NULL;
	int i = 0;

	ZWP_LOG(ZWP_LOG_DEBUG, "DSK: %s, grant_keys: %d, info_cnt: %d", dsk, grant_keys, info_cnt);

	if (zwp_net_find_a_whitelist_dev(net, dsk, &pwl_dev_info) == ZWP_STATUS_OK)
	{
		//Clear grant key (if available), name and loc
		memset(pwl_dev_info->name, 0, sizeof(pwl_dev_info->name));
		memset(pwl_dev_info->loc, 0, sizeof(pwl_dev_info->loc));
	}
	else
	{
		//Cannot find matching DSK record in the whitelist list, add it 
		if (zwp_net_create_whitelist_dev(dsk, &pwl_dev_info) != ZWP_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "zwp_net_wl_dev_info_t structure creation fail. DSK: %s", dsk);
			goto l_err_dev_creation;
		}

		if (zwp_thread_mutex_lock(net->net_whitelist_lock) != ZWP_STATUS_OK)
		{
			goto l_err_net_whitelist_lock;
		}

		if (zwu_array_list_add(net->net_whitelist_list, pwl_dev_info) != ZWP_STATUS_OK)
		{
			goto l_err_whitelist_list_add;
		}

		zwp_thread_mutex_unlock(net->net_whitelist_lock);
	}

	//Update the whitelist dev info structure
	if (pwl_dev_info)
	{
		for (i = 0; i < info_cnt; i++)
		{
			if (info[i].type == WL_INFO_TYPE_NAME)
			{
				strcpy(pwl_dev_info->name, info[i].info.name);
			}
			else if(info[i].type == WL_INFO_TYPE_LOC)
			{
				strcpy(pwl_dev_info->loc, info[i].info.loc);
			}
		}
		pwl_dev_info->metatype_count = i;
		pwl_dev_info->grant_keys = grant_keys;
	}

	return;

l_err_whitelist_list_add:
	zwp_thread_mutex_unlock(net->net_whitelist_lock);
l_err_net_whitelist_lock:
	zwp_net_whitelist_remove(pwl_dev_info);
l_err_dev_creation:
	return;
}

void zwp_net_whitelist_dsk_array_free(zwu_array_list_t whitelist_dsk_array)
{
	char *pdsk = NULL;
	int wl_count;
	int i;

	if (whitelist_dsk_array)
	{
		wl_count = zwu_array_list_size(whitelist_dsk_array);

		for (i = 0; i < wl_count; i++)
		{
			pdsk = zwu_array_list_get(whitelist_dsk_array, i);

			ZWP_FREE(pdsk);
		}

		zwu_array_list_destroy(whitelist_dsk_array);
	}
}

int zwp_net_whitelist_list_report(zwp_net_t net, zwu_array_list_t *pwhitelist_dsk_array)
{
	int wl_count;
	int i;
	zwp_net_wl_dev_info_t *pwl_dev_info = NULL;
	char *pdsk = NULL;

	if (zwu_array_list_create(0, pwhitelist_dsk_array) != ZWP_STATUS_OK)
	{
		goto l_err_whitelist_array_create;
	}

	if (zwp_thread_mutex_lock(net->net_whitelist_lock) != ZWP_STATUS_OK)
	{
		goto l_err_net_whitelist_lock;
	}

	wl_count = zwu_array_list_size(net->net_whitelist_list);

	for (i = 0; i < wl_count; i++)
	{
		pwl_dev_info = zwu_array_list_get(net->net_whitelist_list, i);

		if (pwl_dev_info)
		{
			pdsk = ZWP_STRDUP(pwl_dev_info->dsk);

			if (zwu_array_list_add(*pwhitelist_dsk_array, pdsk) != ZWP_STATUS_OK)
			{
				goto l_err_dsk_add;
			}
		}
	}

	zwp_thread_mutex_unlock(net->net_whitelist_lock);

	return 1;

l_err_dsk_add:
	ZWP_FREE(pdsk);
	zwp_net_whitelist_dsk_array_free(*pwhitelist_dsk_array);
	return 0;

l_err_net_whitelist_lock:
	zwu_array_list_destroy(*pwhitelist_dsk_array);
l_err_whitelist_array_create:
	return 0;
}


/**
 * @brief Task that handles network operation status
 * @param[in]     thread             Thread object
 * @param[in]     arg                Network object of type @a zwp_net_t
 * @return        Nothing
 */
static void *ZWP_THREAD_FUNC zwp_net_operation_status_handler(zwp_thread_t thread, void *arg)
{
	zwp_net_t                    net;
	zwp_net_operation_status_t   net_operation_status;
	uint8_t                      operation;
	uint16_t                     status;
	zwnet_sts_t					 stinfo = { 0 };
	int							 binfo = 0;
	uint16_t                     completed_nodes;
	uint16_t                     total_nodes;

	#if ZWP_LOG_DEBUG_IS_ENABLED
	zwnetd_p                     net_descriptor;
	char                         role_string[ZWP_HCAPI_CONTROLLER_ROLE_STRING_BUFFER_MAX_SIZE];
	#endif

	net = (zwp_net_t)arg;

	zwp_thread_name_set(ZWP_NET_OPERATION_STATUS_HANDLER_THREAD_NAME, NULL);

	ZWP_LOG(ZWP_LOG_DEBUG, "Notification thread started");
	while (1)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Notification queue: Waiting to pop");

		if ((zwp_queue_pop(net->net_operation_status_queue, (void **)&net_operation_status)) != ZWP_STATUS_OK)
		{
			goto l_err_queue_pop;
		}

		//NULL entry indicates handler shutdown
		if (!net_operation_status)
		{
			ZWP_LOG(ZWP_LOG_DEBUG, "Notification queue: Popped : NULL");

			if (zwp_thread_mutex_lock(net->net_initialize_state_lock) != ZWP_STATUS_OK)
			{
				break;
			}

			/*
			 * Check if still in STATE_SHUTTING_DOWN;
			 * Network shall be in other states if zwnet_xxx() is called before processing this notification
			 */
			if (net->net_initialize_state == ZWP_NET_INITIALIZE_STATE_SHUTTING_DOWN)
			{
				net->net_initialize_state = ZWP_NET_INITIALIZE_STATE_NONE;

				zwp_thread_cond_broadcast(net->net_initialize_state_changed);
			}
			zwp_thread_mutex_unlock(net->net_initialize_state_lock);

			//End the thread
			break;
		}

		ZWP_LOG(ZWP_LOG_DEBUG, "Notification queue: Popped : %p", net_operation_status);

		operation = net_operation_status->operation;
		status = net_operation_status->status;
		if (net_operation_status->info)
		{
			binfo = 1;
			memcpy(&stinfo, net_operation_status->info, sizeof(zwnet_sts_t));
		}
		else
			binfo = 0;

		ZWP_LOG(ZWP_LOG_DEBUG, "Notification queue: Popped : operation: %u status: %x info: %p", operation, status, net_operation_status->info);

		if (net_operation_status->info)
			ZWP_FREE(net_operation_status->info);

		ZWP_FREE(net_operation_status);

		if (status & OP_GET_NI_TOTAL_NODE_MASK)
		{
			completed_nodes = status & OP_GET_NI_NODE_CMPLT_MASK;
			total_nodes = (status & OP_GET_NI_TOTAL_NODE_MASK) >> 8;

			ZWP_LOG(ZWP_LOG_DEBUG, "Notification for network operation '%s' with progress '%u/%u'",
					zwp_hcapi_net_operation_to_string(operation), completed_nodes, total_nodes);

			if (zwp_thread_mutex_lock(net->net_initialize_state_lock) != ZWP_STATUS_OK)
			{
				continue;
			}

			net->net_operation_state.current_operation_total_nodes = total_nodes;
			net->net_operation_state.current_operation_completed_nodes = completed_nodes;

			zwp_thread_mutex_unlock(net->net_initialize_state_lock);

			continue;
		}

		/*
		 * Set the operation state
		 */
		if (zwp_thread_mutex_lock(net->net_initialize_state_lock) != ZWP_STATUS_OK)
		{
			continue;
		}

		if (status == OP_FAILED)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Network operation '%s' failed", zwp_hcapi_net_operation_to_string(operation));

			zwp_net_set_operation(net, ZWNET_OP_NONE);
			net->net_operation_state.current_operation_status = ZW_ERR_FAILED;
		}
		else if (status == OP_DONE)
		{
			ZWP_LOG(ZWP_LOG_INFO, "Network operation '%s' completed", zwp_hcapi_net_operation_to_string(operation));
//#ifdef LOCAL_CLIENT_ENABLE
//			if(!((operation == ZWNET_OP_LOAD_NW_INFO)) )
//#endif
			if (operation != ZWNET_OP_NW_CHANGED)
				zwp_net_set_operation(net, ZWNET_OP_NONE);

			// 'zwnet' won't be ready until ZWNET_OP_LOAD_NW_INFO is complete
			//if (operation != ZWNET_OP_INITIALIZE)
			//{
				//zwp_net_save(net);
			//}
		}
		else
		{		
			if (binfo)
			{
				if ((operation == ZWNET_OP_ADD_NODE) && (status == OP_ADD_NODE_PROTOCOL_START))
				{
					//Set add node operation here as previously there is no 'add node' request.
					//This is the unsolicited "add node" (prekit "add node")
					//zwp_net_set_operation(net, ZWNET_OP_ADD_NODE);

					if(stinfo.type == ZWNET_STS_INFO_PREKIT_START)
						memcpy(&net->node_prekit_info, &stinfo, sizeof(zwnet_sts_t));
				}
			}
			//If current network operation != new operation. Set to new operation
			if (net->net_operation_state.current_operation != operation)
			{
				zwp_net_set_operation(net, operation);
			}

			net->net_operation_state.current_operation_status = status;

			ZWP_LOG(ZWP_LOG_INFO, "Network operation '%s' in progress with intermediate status '%s'",
				zwp_hcapi_net_operation_to_string(net->net_operation_state.current_operation),
				zwp_hcapi_net_operation_intermediate_status_to_string(net->net_operation_state.current_operation,
					net->net_operation_state.current_operation_status));


			// Processing over for intermediate states. So unlock and continue with the next in queue
			zwp_thread_mutex_unlock(net->net_initialize_state_lock);
			continue;
		}

		zwp_thread_mutex_unlock(net->net_initialize_state_lock);


		switch (operation)
		{
		case ZWNET_OP_INITIALIZE:
		{
			if (zwp_thread_mutex_lock(net->net_initialize_state_lock) != ZWP_STATUS_OK)
			{
				break;
			}
			/*
			 * Check if still in STATE_INITIALIZING;
			 * Network shall be in other states if zwnet_exit() is called before processing this notification
			 */
			if (net->net_initialize_state == ZWP_NET_INITIALIZE_STATE_INITIALIZING)
			{
				if (status == OP_DONE)
				{
#if ZWP_LOG_DEBUG_IS_ENABLED
					net_descriptor = zwnet_get_desc(net->zwnet);

					zwp_hcapi_controller_role_to_string(net_descriptor->ctl_role, role_string, sizeof role_string);

					ZWP_LOG(ZWP_LOG_DEBUG, "Network home id: '%08X'; Local controller node id: '%u'; Local controller role: '%s'",
						net_descriptor->id, net_descriptor->ctl_id, role_string);
#endif

					net->net_initialize_state = ZWP_NET_INITIALIZE_STATE_INITIALIZED;
				}
				else
				{
					ZWP_LOG(ZWP_LOG_ERR, "Failed to initialize network: %s", zwp_hcapi_net_operation_status_to_string(status));

					net->net_initialize_state = ZWP_NET_INITIALIZE_STATE_NONE;
				}

				zwp_thread_cond_broadcast(net->net_initialize_state_changed);
			}
			zwp_thread_mutex_unlock(net->net_initialize_state_lock);
			break;

			//case ZWNET_OP_LOAD_NW_INFO:
			//	if (zwp_thread_mutex_lock(net->net_initialize_state_lock) != ZWP_STATUS_OK)
			//	{
			//		break;
			//	}
			//	/*
			//	 * Check if still in STATE_LOADING_NET_INFO;
			//	 * Network shall be in other states if zwnet_exit() is called before processing this notification
			//	 */
			//	if (net->net_initialize_state == ZWP_NET_INITIALIZE_STATE_LOADING_NET_INFO)
			//	{
			//		if (status == OP_DONE)
			//		{
			//			net->net_initialize_state = ZWP_NET_INITIALIZE_STATE_COMPLETED;
			//		}
			//		else
			//		{
			//			ZWP_LOG(ZWP_LOG_ERR, "Failed to load network information: %s", zwp_hcapi_net_operation_status_to_string(status));

			//			net->net_initialize_state = ZWP_NET_INITIALIZE_STATE_NONE;
			//		}

			//		zwp_thread_cond_broadcast(net->net_initialize_state_changed);
			//	}
			//	zwp_thread_mutex_unlock(net->net_initialize_state_lock);
			//	break;
		}

		case ZWNET_OP_ADD_NODE:
		{
			if (zwp_thread_mutex_lock(net->net_initialize_state_lock) != ZWP_STATUS_OK)
			{
				break;
			}

			if ((net->net_initialize_state == ZWP_NET_INITIALIZE_STATE_INITIALIZED) && (status == OP_DONE))
			{
				//Note: return status is checked for success here
				if (zwp_net_desc_container_lock_and_test(net) == ZWP_STATUS_OK)
				{
					if (zwp_desc_add_or_update_node(net->net_desc, &net->node_added_desc) != ZWP_STATUS_OK)
					{
						ZWP_LOG(ZWP_LOG_ERR, "Failed to add node to descriptor container tree: %s",
							zwp_hcapi_net_operation_status_to_string(status));
					}

					zwp_net_desc_container_unlock(net);
				}
			}

			zwp_thread_mutex_unlock(net->net_initialize_state_lock);
			break;
		}

		case ZWNET_OP_RP_NODE:
		{
			if (zwp_thread_mutex_lock(net->net_initialize_state_lock) != ZWP_STATUS_OK)
			{
				break;
			}

			if ((net->net_initialize_state == ZWP_NET_INITIALIZE_STATE_INITIALIZED) && (status == OP_DONE))
			{
				//Note: return status is checked for success here
				if (zwp_net_desc_container_lock_and_test(net) == ZWP_STATUS_OK)
				{
					if (zwp_desc_add_or_update_node(net->net_desc, &net->node_replaced_desc) != ZWP_STATUS_OK)
					{
						ZWP_LOG(ZWP_LOG_ERR, "Failed to add node to descriptor container tree: %s",
							zwp_hcapi_net_operation_status_to_string(status));
					}

					zwp_net_desc_container_unlock(net);
				}
			}

			zwp_thread_mutex_unlock(net->net_initialize_state_lock);
			break;
		}

		case ZWNET_OP_NODE_UPDATE:
		{
			if (zwp_thread_mutex_lock(net->net_initialize_state_lock) != ZWP_STATUS_OK)
			{
				break;
			}

			// Update node desc container for both (status == OP_DONE) and (status != OP_DONE)
			if (net->net_initialize_state == ZWP_NET_INITIALIZE_STATE_INITIALIZED)
			{
				//Note: return status is checked for success here
				if (zwp_net_desc_container_lock_and_test(net) == ZWP_STATUS_OK)
				{
					if (zwp_desc_add_or_update_node(net->net_desc, &net->node_updated_desc) != ZWP_STATUS_OK)
					{
						ZWP_LOG(ZWP_LOG_ERR, "Failed to add node to descriptor container tree: %s",
							zwp_hcapi_net_operation_status_to_string(status));
					}

					zwp_net_desc_container_unlock(net);
				}
			}

			zwp_thread_mutex_unlock(net->net_initialize_state_lock);
			break;
		}

		case ZWNET_OP_UPDATE:
		case ZWNET_OP_NW_CHANGED:
		case ZWNET_OP_INITIATE:
		case ZWNET_OP_MIGRATE:
		case ZWNET_OP_RESET:
			if (zwp_thread_mutex_lock(net->net_initialize_state_lock) != ZWP_STATUS_OK)
			{
				break;
			}

			// Rebuild desc container trees for both (status == OP_DONE) and (status != OP_DONE)

			//zwp_net_desc_exit() and zwp_net_desc_init() includes the desc_container_lock/unlock
			zwp_net_desc_exit(net);

			if (zwp_net_desc_init(net) != ZWP_STATUS_OK)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to rebuild descriptor container tree: %s",
						zwp_hcapi_net_operation_status_to_string(status));
			}

			zwp_thread_mutex_unlock(net->net_initialize_state_lock);
			break;
		default:
			break;
		}


	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Notification thread exiting");
	zwp_thread_exit(thread, ZWP_STATUS_OK);
	return NULL;

l_err_queue_pop:
	ZWP_LOG(ZWP_LOG_CRIT, "Network operation status handler: Failed abruptly");
	zwp_thread_exit(thread, ZWP_STATUS_ERROR);
	return NULL;
}

/**
 * @brief Notification handler for network operation status
 * @param[in]     user               Network object of type @a zwp_net_t
 * @param[in]     operation          Network operation
 * @param[in]     status             Network operation status
 * @param[in]	info	    additional information for the specified op and sts; NULL if there is no additional info
 * @return        Nothing
 */
static void ZWP_HCAPI_NOTIFICATION zwp_net_operation_status_notification(void *user, uint8_t operation, uint16_t status, zwnet_sts_t *info)
{
	zwp_net_t                    net;
	zwp_net_operation_status_t   net_operation_status;

	net = (zwp_net_t)user;

	if ((net_operation_status = (zwp_net_operation_status_t)ZWP_CALLOC(1, sizeof(*net_operation_status))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_net_operation_status_alloc;
	}

	net_operation_status->operation = operation;
	net_operation_status->status = status;

	if (info)
	{
		if ((net_operation_status->info = (zwnet_sts_t *)ZWP_CALLOC(1, sizeof(zwnet_sts_t))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_net_operation_info_alloc;
		}

		memcpy(net_operation_status->info, info, sizeof(zwnet_sts_t));
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Notification queue: Waiting to Push: %p", net_operation_status);
	if ((zwp_queue_push(net->net_operation_status_queue, net_operation_status)) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to push network operation status notification into the queue");
		goto l_err_queue_push;
	}
	ZWP_LOG(ZWP_LOG_DEBUG, "Notification queue: Pushed");

	return;

l_err_queue_push:
	if(net_operation_status->info)
		ZWP_FREE(net_operation_status->info);
l_err_net_operation_info_alloc:
	ZWP_FREE(net_operation_status);
l_err_net_operation_status_alloc:
	return;
}

/**
 * @brief Initialize network descriptor tree of a network and save the network information
 * @param[in]     net               Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_net_desc_exit() to free resources
 */
static zwp_status_t zwp_net_desc_init(zwp_net_t net)
{
	if (zwp_thread_mutex_lock(net->net_desc_container_lock) != ZWP_STATUS_OK)
	{
		goto l_err_net_desc_container_lock;
	}

	if (zwp_desc_net_init(&net->net_desc, net) != ZWP_STATUS_OK)
	{
		goto l_err_desc_net_init;
	}

	zwp_thread_mutex_unlock(net->net_desc_container_lock);

	return ZWP_STATUS_OK;

l_err_desc_net_init:
	zwp_thread_mutex_unlock(net->net_desc_container_lock);
l_err_net_desc_container_lock:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Uninitialize network descriptor tree of a network
 * @param[in]     net               Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 * @see zwp_net_desc_init
 */
static zwp_status_t zwp_net_desc_exit(zwp_net_t net)
{
	if (zwp_thread_mutex_lock(net->net_desc_container_lock) != ZWP_STATUS_OK)
	{
		goto l_err_net_desc_container_lock;
	}

	zwp_desc_net_exit(net->net_desc);

	net->net_desc = NULL;

	/* To help end threads waiting on HCAPI transmit status callback 'appl_tx'.
	 * zwnet_exit() does not ensure this callback being called by HCAPI for pending tx.
	 */
	zwp_thread_cond_broadcast(net->net_transmit_status_changed);

	zwp_thread_mutex_unlock(net->net_desc_container_lock);

	return ZWP_STATUS_OK;

	zwp_thread_mutex_unlock(net->net_desc_container_lock);
l_err_net_desc_container_lock:
	return ZWP_STATUS_ERROR;
}


/**
zwp_net_init_scn - Initialize scene module
@param[in]	net				Network
@param[in]	net_timezone	Network timezone (optional).  If it is NULL, the timezone will be retrieved from directory service.
@return	ZWP_STATUS_XXX
*/
zwp_status_t zwp_net_init_scn(zwp_net_t net, char *net_timezone)
{
    zwscn_init_input_t      scn_init_prm = {0};
    zwscn_init_response_p   scn_init_resp = NULL;
    ZWSCN_STATUS_T          scn_result;
    char                    *timezone = NULL;
    //int                     j;
    //int                     last_slash;
    char                    net_id[28];

    scn_init_prm.net = net;

    if (!zwp_net_get_ctx_persistent(net, (char *)scn_init_prm.context_persistent, sizeof(scn_init_prm.context_persistent)))
    {
        ZWP_LOG(ZWP_LOG_ERR, "zwp_net_get_ctx_persistent() failed");
        return ZWP_STATUS_ERROR;
    }

    //Get timezone
    if (net_timezone)
    {
        timezone = net_timezone;
    }
    else
    {
        snprintf(net_id, sizeof(net_id), "%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X",
                 net->portal_client.profile.clnt_id[0], net->portal_client.profile.clnt_id[1],
                 net->portal_client.profile.clnt_id[2], net->portal_client.profile.clnt_id[3],
                 net->portal_client.profile.clnt_id[4], net->portal_client.profile.clnt_id[5],
                 net->portal_client.profile.clnt_id[6], net->portal_client.profile.clnt_id[7]);
#ifndef LOCAL_CLIENT_ENABLE
        if (zwp_directory_service_get_timezone(zwp_portal_get_directory_service(net->portal), net_id, &timezone) != ZWP_STATUS_OK)
#else
        if (zwp_net_get_system_timezone(&timezone) != ZWP_STATUS_OK)
#endif
        {
            ZWP_LOG(ZWP_LOG_ERR, "zwp_directory_service_get_timezone() failed");
            return ZWP_STATUS_ERROR;
        }
    }

    strncpy((char *)scn_init_prm.client_timezone, timezone, ZWSCN_LEN_CLIENT_TIMEZONE);
    scn_init_prm.client_timezone[ZWSCN_LEN_CLIENT_TIMEZONE - 1] = '\0';

    //Save time zone for network
    strcpy(net->timezone, (char *)scn_init_prm.client_timezone);
    if (!net_timezone)
    {
        ZWP_FREE(timezone);
    }
    ZWP_LOG(ZWP_LOG_DEBUG, "Timezone=%s", scn_init_prm.client_timezone);

	memcpy(scn_init_prm.persistent_data_path, net->net_info_dir, strlen(net->net_info_dir));
    ZWP_LOG(ZWP_LOG_DEBUG, "Persistent directory=%s", scn_init_prm.persistent_data_path);

    //Use default max values
    scn_init_prm.num_max_scenes = -1;
    scn_init_prm.num_max_actions_per_scene = -1;
    scn_init_prm.num_max_events_per_scene = -1;
    scn_init_prm.num_max_schedules_per_scene = -1;
	scn_init_prm.num_max_security_scenes = -1;
	scn_init_prm.num_max_arm_security_events_per_scene = -1;
	scn_init_prm.num_max_disarm_security_events_per_scene = -1;
	scn_init_prm.num_max_alarm_security_events_per_scene = -1;

    //Assign HCAPI AAL functions
    scn_init_prm.zwscn_function_setup_op_handler = hcaal_rpt_setup;
    scn_init_prm.zwscn_function_set_op_handler = hcaal_set;
    scn_init_prm.zwscn_function_get_op_handler = hcaal_get;
	scn_init_prm.zwscn_function_get_nameloc_handler = hcaal_get_nameloc;
    scn_init_prm.zwscn_function_zwpoll_rm_handler = hcaal_poll_rm;
    scn_init_prm.zwscn_function_zwpoll_rm_mul_handler = hcaal_poll_rm_mul;
    scn_init_prm.zwscn_fn_is_valid_handler = hcaal_is_obj_valid;

    scn_result = zwscn_init_network(&scn_init_prm, &scn_init_resp);
    if (scn_result)
    {
        ZWP_LOG(ZWP_LOG_ERR, "zwscn_init_network with error:%d", scn_result);
        return ZWP_STATUS_ERROR;
    }

    //Save the HCAPI notification and report callback functions
    hcaal_scene_cb_set(&net->hcaal_ctx, scn_init_resp);
    ZWP_FREE(scn_init_resp); //free the original structure as the above function has copied it

    scn_result = zwscn_status_update_all_scenes(net); //We should update the scenes which are loaded from the persistent storage
    if (scn_result)
    {
        ZWP_LOG(ZWP_LOG_WARNING, "zwscn_status_update_all_scenes with error:%d", scn_result);
        //If an error occurs Just print a warning and move on as this operation is not critical
    }

    return ZWP_STATUS_OK;
}


zwp_status_t zwp_net_initialize(zwp_net_t net)
{
#if 0
	dev_cfg_usr_t      device_config;
#endif
	int                hcapi_code;
	zwp_status_t       return_status;
	//struct stat        stat_info;
	char               current_thread_name[ZWP_THREAD_NAME_SIZE];
#ifndef LOCAL_CLIENT_ENABLE
	uint32_t		   dwHomeID;
#endif

	if (zwp_thread_create(&net->net_operation_status_handler, zwp_net_operation_status_handler, net,
			net->net_pool, ZWP_THREAD_CREATE_JOINABLE) != ZWP_STATUS_OK)
	{
#ifndef LOCAL_CLIENT_ENABLE
		// Close the descriptors and destroy SSL object within HCAPI
		zwportal_clnt_conn_close(net->portal_client.file_descriptor, net->portal_client.SSL_object);
#endif

		goto l_err_net_operation_status_thread_create;
	}

	net->net_init.instance = 0;

	//TODO: Logging
	net->net_init.print_txt_fn = NULL;
#ifndef LOCAL_CLIENT_ENABLE
	net->net_init.display_ctx = NULL;
#endif

	//Device specific configurations
	net->net_init.dev_cfg_usr = NULL;
	net->net_init.dev_cfg_file = zwp_portal_get_device_specific_configuration_file(net->portal);

	net->net_init.net_info_dir = net->net_info_dir;

#if 0
	device_config.dev_rec_find_fn = zwp_device_config_find;
	device_config.dev_cfg_ctx = net;
	net->net_init.dev_cfg_usr = &device_config;
#endif

	//Z-Wave network notifications
	net->net_init.user = net;
	net->net_init.node = zwp_net_node_list_notification;
	net->net_init.notify = zwp_net_operation_status_notification;
	net->net_init.appl_tx = zwp_net_transmit_notification;
	net->net_init.s2_unsolicited_cb = zwp_net_add_node_sec2_unsolicited_notification;

#ifndef LOCAL_CLIENT_ENABLE
	//Portal configurations
	net->net_init.portal_fd = net->portal_client.file_descriptor;
	net->net_init.portal_ssl = net->portal_client.SSL_object;
	memcpy(&net->net_init.portal_prof, &net->portal_client.profile, sizeof(net->net_init.portal_prof));
	net->net_init.net_err = zwp_net_error_notification;
	net->net_init.host_port = ZWAVE_HOME_CTL_PORT;
	net->net_init.use_ipv4 = 0;
	memcpy(net->net_init.zip_router, &net->portal_client.profile.clnt_ipv6_addr, IPV6_ADDR_LEN);
#endif

	if (zwp_thread_mutex_lock(net->net_initialize_state_lock) != ZWP_STATUS_OK)
	{
#ifndef LOCAL_CLIENT_ENABLE
		// Close the descriptors and destroy SSL object within HCAPI
		zwportal_clnt_conn_close(net->portal_client.file_descriptor, net->portal_client.SSL_object);
#endif

		goto l_err_net_initialize_state_lock;
	}

	ZWP_LOG(ZWP_LOG_INFO, "zwnet_init() Intialization paramters");

	//Change the set operation to before calling HCAPI to do the actual work, rather than after...
	//so that when HCAPI fails the calls, either directly or through callback, the status can be
	//updated correctly.
#ifndef LOCAL_CLIENT_ENABLE
	//In case of local client we are already doing it in zwp_portal_client_spawn_net
	zwp_net_set_operation(net, ZWNET_OP_INITIALIZE);
#endif

	net->net_initialize_state = ZWP_NET_INITIALIZE_STATE_INITIALIZING;

	zwp_thread_name_set(ZWP_HCAPI_NET_INIT_THREAD_NAME, current_thread_name);

	if ((hcapi_code = hcaal_init(&net->hcaal_ctx, net, &net->net_init, &net->zwnet)) != ZW_ERR_NONE)
	{
		zwp_thread_name_set(current_thread_name, NULL);

		ZWP_LOG(ZWP_LOG_ERR, "zwnet_init() failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));

		//For each failure in the init, set the operation status so that UI can reflect correctly.
		zwp_net_set_operation(net, ZWNET_OP_NONE);
		net->net_operation_state.current_operation_status = ZW_ERR_FAILED;

		if (net->net_init.err_loc.dev_ent)
		{
			if (net->net_init.err_loc.ep_ent)
			{
				if (net->net_init.err_loc.if_ent)
				{
					ZWP_LOG(ZWP_LOG_ERR, "Failed to parse device specific configuration file: Entry numbers (Device, Endpoint, Interface): (%u, %u, %u)",
							net->net_init.err_loc.dev_ent,
							net->net_init.err_loc.ep_ent,
							net->net_init.err_loc.if_ent);
				}
				else
				{
					ZWP_LOG(ZWP_LOG_ERR, "Failed to parse device specific configuration file: Entry numbers (Device, Endpoint): (%u, %u)",
							net->net_init.err_loc.dev_ent,
							net->net_init.err_loc.ep_ent);
				}
			}
			else
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to parse device specific configuration file: Entry numbers (Device): (%u)",
						net->net_init.err_loc.dev_ent);
			}
		}

#ifndef LOCAL_CLIENT_ENABLE
		// Close the descriptors and destroy SSL object within HCAPI
		zwportal_clnt_conn_close(net->portal_client.file_descriptor, net->portal_client.SSL_object);
#endif

		goto l_err_zwnet_init;
	}

	zwp_thread_name_set(current_thread_name, NULL);

	/*
	 * After zwnet_init() is successful, there is no need to call zwportal_clnt_conn_close()
	 * for all error cases. Because zwnet_exit() in error handling sequence of this function
	 * will call zwportal_clnt_conn_close() internally.
	 */

	while (!(net->net_initialize_state != ZWP_NET_INITIALIZE_STATE_INITIALIZING))
	{
		if (zwp_thread_cond_wait(net->net_initialize_state_changed, net->net_initialize_state_lock) != ZWP_STATUS_OK)
		{
			goto l_err_net_initialize_state_changed_wait_for_init;
		}
	}

	if (net->net_initialize_state != ZWP_NET_INITIALIZE_STATE_INITIALIZED)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to initialize network");
		goto l_err_net_initialize_state_failed;
	}

	//
	// Initialize IFTTT database for this network
	// This must be done prior to constructing network tree
#ifndef LOCAL_CLIENT_ENABLE
	if (zwp_net_get_homeid(net, &dwHomeID))
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "dwHomeID = %x", dwHomeID);

		if (zwp_db_open_db(net->net_info_dir, dwHomeID, &net->net_ifttt_db_p) != ZWP_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to open/create IFTTT database with homeid %8x", dwHomeID);

			zwp_net_set_operation(net, ZWNET_OP_NONE);
			net->net_operation_state.current_operation_status = ZW_ERR_FAILED;
			goto l_err_ifttt_db_init;
		}
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "DB created/open %p with HomeID %X", net->net_ifttt_db_p, dwHomeID);

#endif

	ZWP_LOG(ZWP_LOG_DEBUG, "Constructing Network Tree");
	if (zwp_net_desc_init(net) != ZWP_STATUS_OK)
	{
		zwp_net_set_operation(net, ZWNET_OP_NONE);
		net->net_operation_state.current_operation_status = ZW_ERR_FAILED;
		goto l_err_net_desc_init;
	}

#ifdef LOCAL_CLIENT_ENABLE
	//set to local here
	if (zwp_portal_get_gw_auto_set_status(net->portal))
	{
		zwp_net_unsolicited_set_to_local(net);
		ZWP_LOG(ZWP_LOG_DEBUG, "Setting Gateway Unsolicited IP to Auto Local Done");
	}
	else
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Setting Gateway Unsolicited IP to Auto Local is DISABLED");
	}
#endif

	//
	// Initialize scene for this network
	//
	if (zwp_net_init_scn(net, NULL) != ZWP_STATUS_OK)
	{
		zwp_net_set_operation(net, ZWNET_OP_NONE);
		net->net_operation_state.current_operation_status = ZW_ERR_FAILED;
		goto l_err_scn_init;
	}


	zwp_net_set_operation(net, ZWNET_OP_NONE);

//	if (net->net_initialize_state == ZWP_NET_INITIALIZE_STATE_INITIALIZED)
//	{
//        //zwnetd_p                zwnet_desc;
//
//		net->net_initialize_state = ZWP_NET_INITIALIZE_STATE_LOADING_NET_INFO;
//
//		zwp_thread_name_set(ZWP_HCAPI_NET_LOAD_THREAD_NAME, current_thread_name);
//
//		ZWP_LOG(ZWP_LOG_DEBUG, "Loading network information");
//
//		zwp_net_set_operation(net, ZWNET_OP_LOAD_NW_INFO);
//
//		if ((hcapi_code = zwnet_load(net->zwnet, net->net_info_path)) != ZW_ERR_NONE)
//		{
//			if (stat(net->net_info_path, &stat_info) != 0)
//			{
//				// Report as Info
//				ZWP_LOG(ZWP_LOG_INFO, "zwnet_load() with 'network info file' failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
//				ZWP_LOG(ZWP_LOG_INFO, "Network info file is not present. Possibly connecting to this portal for the first time");
//			}
//			else
//			{
//				// Report as Warning
//				ZWP_LOG(ZWP_LOG_WARNING, "zwnet_load() with 'network info file' failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
//				ZWP_LOG(ZWP_LOG_WARNING, "Network info file exists. But failed to load network using it");
//			}
//
//			if ((hcapi_code = zwnet_load(net->zwnet, NULL)) != ZW_ERR_NONE)
//			{
//				zwp_net_set_operation(net, ZWNET_OP_NONE);
//				net->net_operation_state.current_operation_status = ZW_ERR_FAILED;
//
//				ZWP_LOG(ZWP_LOG_ERR, "zwnet_load() with the network fall-back failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
//				zwp_thread_name_set(current_thread_name, NULL);
//				goto l_err_zwnet_load;
//			}
//		}
//
//		zwp_thread_name_set(current_thread_name, NULL);
//
//		while (!(net->net_initialize_state != ZWP_NET_INITIALIZE_STATE_LOADING_NET_INFO))
//		{
//			if (zwp_thread_cond_wait(net->net_initialize_state_changed, net->net_initialize_state_lock) != ZWP_STATUS_OK)
//			{
//				goto l_err_net_initialize_state_changed_wait_for_load;
//			}
//		}
//
//		if (net->net_initialize_state == ZWP_NET_INITIALIZE_STATE_COMPLETED)
//		{
//			ZWP_LOG(ZWP_LOG_DEBUG, "Constructing Network Tree");
//			if (zwp_net_desc_init(net) != ZWP_STATUS_OK)
//			{
//				zwp_net_set_operation(net, ZWNET_OP_NONE);
//				net->net_operation_state.current_operation_status = ZW_ERR_FAILED;
//				goto l_err_net_desc_init;
//			}
//		}
//
//#ifdef LOCAL_CLIENT_ENABLE
//		//set to local here
//		if (zwp_portal_get_gw_auto_set_status(net->portal))
//		{
//			zwp_net_unsolicited_set_to_local(net);
//			ZWP_LOG(ZWP_LOG_DEBUG, "Setting Gateway Unsolicited IP to Auto Local Done");
//		}
//		else
//		{
//			ZWP_LOG(ZWP_LOG_DEBUG, "Setting Gateway Unsolicited IP to Auto Local is DISABLED");
//		}
//#endif
//
//        //
//        // Initialize scene for this network
//        //
//        if (zwp_net_init_scn(net, NULL) != ZWP_STATUS_OK)
//        {
//        	zwp_net_set_operation(net, ZWNET_OP_NONE);
//        	net->net_operation_state.current_operation_status = ZW_ERR_FAILED;
//            goto l_err_scn_init;
//        }
//
//		zwp_net_set_operation(net, ZWNET_OP_NONE);
//	}
	zwp_thread_mutex_unlock(net->net_initialize_state_lock);

	return ZWP_STATUS_OK;


l_err_scn_init:
    zwp_net_desc_exit(net);
l_err_net_desc_init:
#ifndef LOCAL_CLIENT_ENABLE
	zwp_db_close_db(net->net_ifttt_db_p);
	net->net_ifttt_db_p = NULL;
l_err_ifttt_db_init:
#endif
//l_err_net_initialize_state_changed_wait_for_load:
//l_err_zwnet_load:
l_err_net_initialize_state_failed:
l_err_net_initialize_state_changed_wait_for_init:

	hcaal_exit(&net->hcaal_ctx, net->zwnet);
	net->zwnet = NULL;

l_err_zwnet_init:
l_err_net_initialize_state_lock:
	net->net_initialize_state = ZWP_NET_INITIALIZE_STATE_SHUTTING_DOWN;
	zwp_queue_push(net->net_operation_status_queue, NULL);

	while (net->net_initialize_state != ZWP_NET_INITIALIZE_STATE_NONE)
	{
		zwp_thread_cond_wait(net->net_initialize_state_changed, net->net_initialize_state_lock);
	}

	//Comment out this. If init fails, operation status should remain as ZW_ERR_FAILED so that it will
	//be consistent with the failure comes from HCAPI callback or directly failed. In addition, UI will
	//be able to use the ZW_ERR_FAILED (-1) to reflect the correct state.
	//zwp_net_set_operation(net, ZWNET_OP_NONE);
	//net->net_operation_state.current_operation_status = ZWP_NET_OPERATION_STATUS_NO_NETWORK;

	zwp_thread_mutex_unlock(net->net_initialize_state_lock);

	zwp_thread_join(net->net_operation_status_handler, &return_status);

l_err_net_operation_status_thread_create:
	return ZWP_STATUS_ERROR;
}

void zwp_net_exit(zwp_net_t net)
{
	zwp_status_t return_status;

	if (zwp_thread_mutex_lock(net->net_initialize_state_lock) != ZWP_STATUS_OK)
	{
		goto l_err_net_initialize_state_lock;
	}

	net->net_initialize_state = ZWP_NET_INITIALIZE_STATE_SHUTTING_DOWN;

	zwp_net_desc_exit(net);

    zwscn_exit_network(net);
	ZWP_LOG(ZWP_LOG_DEBUG, "Begin HCAPI net_exit(), zwnet = %p", net->zwnet);
	hcaal_exit(&net->hcaal_ctx, net->zwnet);
	ZWP_LOG(ZWP_LOG_DEBUG, "End of HCAPI net_exit(), zwnet = %p", net->zwnet);
	net->zwnet = NULL;

#ifndef LOCAL_CLIENT_ENABLE
	if (net->net_ifttt_table_trigger_generic_p)
	{
		zwp_db_close_table(net->net_ifttt_table_trigger_generic_p);
		net->net_ifttt_table_trigger_generic_p = NULL;
	}
	zwp_db_close_db(net->net_ifttt_db_p);
	net->net_ifttt_db_p = NULL;
#endif

	zwp_net_set_operation(net, ZWNET_OP_NONE);
	net->net_operation_state.current_operation_status = ZWP_NET_OPERATION_STATUS_NO_NETWORK;

	//Push NULL entry into the queue to indicate net operation status handler shutdown
	if ((zwp_queue_push(net->net_operation_status_queue, NULL)) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to push stop handler notification into network operation status queue");
		zwp_thread_mutex_unlock(net->net_initialize_state_lock);
		goto l_err_queue_push;
	}

	while (net->net_initialize_state != ZWP_NET_INITIALIZE_STATE_NONE)
	{
		if (zwp_thread_cond_wait(net->net_initialize_state_changed, net->net_initialize_state_lock) != ZWP_STATUS_OK)
		{
			zwp_thread_mutex_unlock(net->net_initialize_state_lock);
			goto l_err_net_initialize_state_changed_wait_for_exit;
		}
	}

	zwp_thread_mutex_unlock(net->net_initialize_state_lock);

	if (zwp_thread_join(net->net_operation_status_handler, &return_status) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to join the handler for network operation status");
		goto l_err_thread_join;
	}

	if (return_status != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Network operation status handler returned with failure");
	}

	return;

l_err_thread_join:
l_err_net_initialize_state_changed_wait_for_exit:
l_err_queue_push:
l_err_net_initialize_state_lock:
	return;
}

/**
 * @brief Thread that handles network initialization
 * @param[in]     thread            Thread object
 * @param[in]     arg               Network object of type @a zwp_net_t
 * @return        NULL
 */
static void *ZWP_THREAD_FUNC zwp_net_start_handler(zwp_thread_t thread, void *arg)
{
	zwp_net_t      net;

	net = (zwp_net_t)arg;

	ZWP_LOG(ZWP_LOG_DEBUG, "zwp_net_start_handler thread started");

	zwp_thread_name_set(ZWP_NET_START_HANDLER_THREAD_NAME, NULL);

	if (zwp_net_initialize(net) != ZWP_STATUS_OK)
	{
		goto l_err_net_initialize;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "zwp_net_start_handler thread exiting");
	zwp_thread_exit(thread, ZWP_STATUS_OK);
	return NULL;

l_err_net_initialize:

	ZWP_LOG(ZWP_LOG_DEBUG, "zwp_net_start_handler thread exiting with error");
	zwp_thread_exit(thread, ZWP_STATUS_ERROR);
	return NULL;
}

zwp_status_t zwp_net_start(zwp_net_t net)
{
	if (zwp_thread_create(&net->net_start_handler, zwp_net_start_handler, net,
			net->net_pool, ZWP_THREAD_CREATE_JOINABLE) != ZWP_STATUS_OK)
	{
		goto l_err_thread_create;
	}

	return ZWP_STATUS_OK;

l_err_thread_create:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_net_stop(zwp_net_t net)
{
	zwp_status_t return_status;

	if (zwp_thread_join(net->net_start_handler, &return_status) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to join network start handler");
		goto l_err_thread_join;
	}

	if (return_status == ZWP_STATUS_OK)
	{
		zwp_net_exit(net);
	}
	else
	{
		// This is a debug log as error must have been reported already
		ZWP_LOG(ZWP_LOG_DEBUG, "Previous invocation of zwp_net_start_handler() failed");
	}

	return ZWP_STATUS_OK;

l_err_thread_join:
	return ZWP_STATUS_ERROR;
}

void *ZWP_THREAD_FUNC zwp_net_stop_async_handler(zwp_thread_t thread, void *arg)
{
	zwp_net_t      net;
	zwp_portal_t	portal;
	zwp_status_t    status;

	//ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_START);
	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_START, NULL);

	net = (zwp_net_t)arg;
	portal = zwp_net_get_portal(net);

	zwp_thread_name_set(ZWP_NET_STOP_HANDLER_THREAD_NAME, NULL);

	//Add itself to portal's net stop array thread list
	zwp_portal_thread_handle_list_add(portal, thread);

	if (zwp_net_stop(net) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_WARNING, "zwp_net_stop() failed. 'net' not freed. Possible memory leak");
		goto l_err_net_stop;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Network about to be freed %p", net);

	zwp_net_free(net);

	ZWP_LOG(ZWP_LOG_DEBUG, "Network freed %p", net);

	//Remove itself from portal's net stop array thread list. If it is found and removed,
	//make itself a detached thread and exit
	//Otherwise if it is not found, it mean portal probably started exiting and the main thread
	//is currently 'joining' this thread. Then just exit thread normally.
	if (zwp_portal_thread_handle_list_remove(portal, thread) == ZWP_STATUS_OK)
	{
		zwp_thread_detach(thread, &status);
	}

	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_END, ZWP_STATUS_OK);

	zwp_thread_exit(thread, ZWP_STATUS_OK);
	return NULL;

l_err_net_stop:

	if (zwp_portal_thread_handle_list_remove(portal, thread) == ZWP_STATUS_OK)
	{
		zwp_thread_detach(thread, &status);
	}
	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_END, ZWP_STATUS_ERROR);
	zwp_thread_exit(thread, ZWP_STATUS_ERROR);
	return NULL;
}

/**
 * @brief Allocate routine data strctures within a network object
 * @param[in]     net               Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_net_data_free() to free resources
 */
static zwp_status_t zwp_net_data_alloc(zwp_net_t net)
{
	if (zwp_pool_create(&net->net_pool, zwp_portal_get_portal_pool(net->portal)) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create pool for network level");
		goto l_err_net_pool_create;
	}

	if (zwp_thread_mutex_create(&net->reference_count_lock, net->net_pool, ZWP_THREAD_MUTEX_CREATE_NORMAL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create lock for reference count");
		goto l_err_ref_count_lock_create;
	}

	if (zwp_thread_cond_create(&net->reference_count_zeroed, net->net_pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for reference count zeroed state");
		goto l_err_ref_count_zeroed_cond_create;
	}

	//TODO: Make queue size configurable
	if (zwp_queue_create(&net->net_operation_status_queue, 20, net->net_pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create queue for network operation status notification");
		goto l_err_net_operation_queue_create;
	}

	if (zwp_thread_mutex_create(&net->net_initialize_state_lock, net->net_pool, ZWP_THREAD_MUTEX_CREATE_NORMAL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create lock for network initialize state");
		goto l_err_net_initialize_state_lock_create;
	}

	if (zwp_thread_cond_create(&net->net_initialize_state_changed, net->net_pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for network initialized state");
		goto l_err_net_initialized_state_cond_create;
	}

    //Use recursive lock as scene module needs to use it on event & timer.
	//if (zwp_thread_mutex_create(&net->net_desc_container_lock, net->net_pool, ZWP_THREAD_MUTEX_CREATE_NORMAL) != ZWP_STATUS_OK)
    if (zwp_thread_mutex_create(&net->net_desc_container_lock, net->net_pool, ZWP_THREAD_MUTEX_CREATE_RECURSIVE) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create lock for network descriptors");
		goto l_err_net_desc_lock_create;
	}

	if (zwp_thread_cond_create(&net->net_transmit_status_changed, net->net_pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for network transmit status change");
		goto l_err_net_transmit_status_cond_create;
	}

	if (zwu_array_list_create(0, &net->net_whitelist_list) != ZWP_STATUS_OK)
	{
		goto l_err_whitelist_list_create;
	}

	if (zwp_thread_mutex_create(&net->net_whitelist_lock, net->net_pool, ZWP_THREAD_MUTEX_CREATE_NORMAL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create lock for whitelist");
		goto l_err_whitelist_lock_create;
	}

	net->node_added_s2_param.cb_type = 255;
	net->node_added_s2_param.cb_prm.dsk.dsk = NULL;

	net->node_prekit_info.type = -1;

	return ZWP_STATUS_OK;

	zwp_thread_mutex_destroy(net->net_whitelist_lock);
l_err_whitelist_lock_create:
	zwu_array_list_destroy(net->net_whitelist_list);
l_err_whitelist_list_create:
l_err_net_transmit_status_cond_create:
	zwp_thread_mutex_destroy(net->net_desc_container_lock);
l_err_net_desc_lock_create:
	zwp_thread_cond_destroy(net->net_initialize_state_changed);
l_err_net_initialized_state_cond_create:
	zwp_thread_mutex_destroy(net->net_initialize_state_lock);
l_err_net_initialize_state_lock_create:
	zwp_queue_destroy(net->net_operation_status_queue);
l_err_net_operation_queue_create:
	zwp_thread_cond_destroy(net->reference_count_zeroed);
l_err_ref_count_zeroed_cond_create:
	zwp_thread_mutex_destroy(net->reference_count_lock);
l_err_ref_count_lock_create:
	zwp_pool_destroy(net->net_pool);
l_err_net_pool_create:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free routine data strctures within a network object
 * @param[in]     net               Network object
 * @return        Nothing
 * @see zwp_net_data_alloc
 */
static void zwp_net_data_free(zwp_net_t net)
{
	zwp_net_whitelist_remove_all(net, net->net_whitelist_list);

	zwp_thread_cond_destroy(net->net_transmit_status_changed);
	zwp_thread_mutex_destroy(net->net_desc_container_lock);
	zwp_thread_cond_destroy(net->net_initialize_state_changed);
	zwp_thread_mutex_destroy(net->net_initialize_state_lock);
	zwp_queue_destroy(net->net_operation_status_queue);
	zwp_thread_cond_destroy(net->reference_count_zeroed);
	zwp_thread_mutex_destroy(net->reference_count_lock);
	zwu_array_list_destroy(net->net_whitelist_list);
	zwp_thread_mutex_destroy(net->net_whitelist_lock);
	zwp_pool_destroy(net->net_pool);

	if (net->node_added_s2_param.cb_type == S2_CB_TYPE_DSK)
	{
		if (net->node_added_s2_param.cb_prm.dsk.dsk)
			ZWP_FREE(net->node_added_s2_param.cb_prm.dsk.dsk);
	}
}

zwp_status_t zwp_net_alloc(zwp_net_t *net_p, int file_descriptor,
		void *SSL_object, clnt_prof_t *profile, zwp_portal_t portal)
{
	zwp_net_t net;
	char rac[24];
	char *local_state_dir;

	if ((net = (zwp_net_t)ZWP_CALLOC(1, sizeof(*net))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_net_alloc;
	}

#ifndef LOCAL_CLIENT_ENABLE
	net->portal_client.file_descriptor = file_descriptor;
	net->portal_client.SSL_object = SSL_object;
	memcpy(&net->portal_client.profile, profile, sizeof(*profile));
#endif

	local_state_dir = zwp_portal_get_local_state_dir(portal);

#ifndef LOCAL_CLIENT_ENABLE
	snprintf(rac, sizeof(rac), "%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X",
			profile->clnt_id[0], profile->clnt_id[1], profile->clnt_id[2], profile->clnt_id[3],
			profile->clnt_id[4], profile->clnt_id[5], profile->clnt_id[6], profile->clnt_id[7]);
#else
	snprintf(rac, sizeof(rac), "%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X",
			local_clnt_id[0], local_clnt_id[1], local_clnt_id[2], local_clnt_id[3],
			local_clnt_id[4], local_clnt_id[5], local_clnt_id[6], local_clnt_id[7]);
#endif

	if ((net->net_info_dir = (char *)ZWP_MALLOC(strlen(local_state_dir) + 2 + sizeof(ZWP_NET_INFO_DIR))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_net_info_path_alloc;
	}
	sprintf(net->net_info_dir, "%s/%s/", local_state_dir, ZWP_NET_INFO_DIR);

	//Back reference to portal
	net->portal = portal;

	if (zwp_net_data_alloc(net) != ZWP_STATUS_OK)
	{
		goto l_err_data_create;
	}

	memset(&net->net_operation_state, 0, sizeof(net->net_operation_state));
#ifndef LOCAL_CLIENT_ENABLE
	net->net_operation_state.current_operation_status = ZWP_NET_OPERATION_STATUS_NO_NETWORK;
#else
	net->net_operation_state.current_operation_status = 0;
#endif

	net->reference_count = 0;
	net->net_initialize_state = ZWP_NET_INITIALIZE_STATE_NONE;

	net->net_transmit_status = ZWP_NET_TRANSMIT_STATUS_NONE;

	net->zwnet = NULL;
	net->net_desc = NULL;
	net->ctrl_local_dsk = NULL;

	*net_p = net;

	return ZWP_STATUS_OK;

l_err_data_create:
	ZWP_FREE(net->net_info_dir);
l_err_net_info_path_alloc:
	ZWP_FREE(net);
l_err_net_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_net_free(zwp_net_t net)
{
	//Wait for all ongoing AVI calls to complete and release their references to this network
	zwp_net_reference_count_wait_for_zero(net);

	zwp_net_data_free(net);

	if (net->ctrl_local_dsk)
	{
		ZWP_FREE(net->ctrl_local_dsk);
		net->ctrl_local_dsk = NULL;
	}

	ZWP_FREE(net->net_info_dir);

	ZWP_FREE(net);

	return;
}

zwp_status_t zwp_net_config_get_instance(zwp_net_config_t *net_config_p,
		const config_t *config_parser, const char *config_dir)
{
	zwp_net_config_t net_config;

	if ((net_config = (zwp_net_config_t)ZWP_MALLOC(sizeof(*net_config))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_net_config_alloc;
	}

	*net_config_p = net_config;

	return ZWP_STATUS_OK;

l_err_net_config_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_net_config_free(zwp_net_config_t net_config)
{
	ZWP_FREE(net_config);
}

