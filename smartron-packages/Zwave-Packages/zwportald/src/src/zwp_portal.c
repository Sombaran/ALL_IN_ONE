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
 * @file    zwp_portal.c
 *
 * @brief   Portal module
 * @details Module for setting up portal service
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-04-30
 * - Initial version
 */

#include <stddef.h>
#include <stdint.h>

#include "zwp_log.h"
#if defined(ZWP_DEBUG_PORTAL)
#include "zwp_log_enable.h"
#else
#include "zwp_log_disable.h"
#endif

#include "zwp_memory.h"
#include "zwp_portal.h"
#include "zwp_util_config.h"
#include "zwp_error.h"
#include "zwp_hash.h"
#include "zwp_thread_mutex.h"
#include "zwu_time.h"
#include "zwp_ring.h"
#include "zwp_avi_message.h"
#include "zwp_network.h"
#include "zwp_avi_service_networks.h"
#include "zwscn_scene_api.h"

#ifdef LOCAL_CLIENT_ENABLE
#include "zwp_zwave_client.h"
#endif

/** Default listen port for portal connections */
#define ZWP_OPTION_DEFAULT_LISTEN_PORT                                  44123

/** Default timeout (in seconds) to wait for receving a Z-Wave Report command */
#define ZWP_OPTION_DEFAULT_REPORT_WAIT_TIMEOUT                          11

/** Name of thread handling scenes */
#define ZWP_SCENE_THREAD_NAME                                           "Scene"

#ifdef LOCAL_CLIENT_ENABLE
extern const uint8_t local_clnt_id[8];
#endif
/**
 * @brief Portal configuration class
 */
struct _zwp_portal_config
{
	uint16_t            listen_port;                   /**< Listening port */

	char                *local_state_dir;              /**< Directory for storing local state */

	struct
	{
		char            *CA_certificate_file;          /**< CA certificate file */
		char            *SSL_certificate_file;         /**< SSL certificate file */
		char            *private_key_file;             /**< Private key file */
	} security;                                        /**< Security */

	char                *device_specific_configuration_file;   /**< Device specific configuration file */

	struct
	{
		struct
		{
			int         report_wait_timeout;           /**< Z-Wave Report wait timeout (in seconds) */
		} interface;                                   /**< Interface level configuration */
	} network;                                         /**< Network level configuration */
#ifdef LOCAL_CLIENT_ENABLE
	uint8_t            gateway_auto_set_local;         /**< Flag to indicate automatically set the gateway unsolicited IP to local IP or not*/
#endif
};
#ifndef LOCAL_CLIENT_ENABLE

/** Portal new client parameters */
typedef struct
{
	int                          file_descriptor;          /**< File descriptor to connect to Z/IP gateway/router using TLS */
	void                         *SSL_object;              /**< SSL object pointer to connect to Z/IP gateway/router using TLS */
	clnt_prof_t                  *profile;                 /**< Portal configuration profile for the Z/IP gateway */
} zwp_portal_new_client_t;
#endif

/**
 * @brief Portal class
 */
struct _zwp_portal
{
	void                         *portal_handle;                       /**< Handle for portal */

	zwp_pool_t                   portal_pool;                          /**< Memory pool at Portal level */

	zwp_hash_t                   net_table;                            /**< Hash table of networks */
	zwp_thread_mutex_t           net_table_lock;                       /**< Mutex lock for hash table of networks */

	zwp_avi_t                    avi;                                  /**< Reference to Avro interface */
	zwp_directory_service_t      directory_service;                    /**< Directory service handle */
	zwp_ifttt_t                  ifttt;                                /**< IFTTT handle */

	zwu_array_list_t			 net_stop_thread_array;				   /**< An array of thread handles that might be still running when portal exits*/
	zwp_thread_mutex_t           net_stop_thread_array_lock;           /**< Mutex lock for stop thread handles array */

	zwp_portal_config_t          portal_config;                        /**< Portal configuration */
#ifdef LOCAL_CLIENT_ENABLE
	zwp_zwave_client_t 			 zip_client;		   					/**< Local client configuration */
	zwp_thread_mutex_t           client_initialization_lock;           /**< Mutex lock for client initialization */
#endif
};

#ifdef LOCAL_CLIENT_ENABLE
zwp_status_t zwp_portal_client_spawn_net(zwp_portal_t portal, zwp_portal_new_client_t *new_client_parameter, zwp_net_t *net_p);
#else
static zwp_status_t zwp_portal_client_spawn_net(zwp_portal_t portal, zwp_portal_new_client_t *new_client_parameter, zwp_net_t *net_p);
#endif

static zwp_status_t zwp_portal_net_table_add(zwp_portal_t portal, zwp_net_t net);
static zwp_status_t zwp_portal_net_table_remove(zwp_portal_t portal, zwp_net_t net, int check_net_exists);
static zwp_status_t zwp_portal_net_table_remove_by_net_id(zwp_portal_t portal, char *net_id, zwp_net_t *net_p);
static zwp_status_t zwp_portal_net_table_get(zwp_portal_t portal, const uint8_t *clnt_id, size_t clnt_id_size, zwp_net_t *net_p);
static zwp_status_t zwp_portal_net_table_get_from_ra_code(zwp_portal_t portal, char *ra_code, zwp_net_t *net_p);

/* Getters section starts */
zwp_pool_t zwp_portal_get_portal_pool(zwp_portal_t portal)
{
	return portal->portal_pool;
}

zwp_directory_service_t zwp_portal_get_directory_service(zwp_portal_t portal)
{
	return portal->directory_service;
}

zwp_ifttt_t zwp_portal_get_ifttt(zwp_portal_t portal)
{
	return portal->ifttt;
}

zwp_avi_t zwp_portal_get_avi(zwp_portal_t portal)
{
	return portal->avi;
}

void *zwp_portal_get_portal_handle(zwp_portal_t portal)
{
	return portal->portal_handle;
}

char *zwp_portal_get_local_state_dir(zwp_portal_t portal)
{
	return portal->portal_config->local_state_dir;
}

char *zwp_portal_get_device_specific_configuration_file(zwp_portal_t portal)
{
	return portal->portal_config->device_specific_configuration_file;
}

int zwp_portal_get_report_wait_timeout(zwp_portal_t portal)
{
	return portal->portal_config->network.interface.report_wait_timeout;
}
#ifdef LOCAL_CLIENT_ENABLE
void *zwp_portal_get_zip_client_handle(zwp_portal_t portal)
{
	return portal->zip_client;
}
zwp_status_t zwp_portal_get_client_intialize_lock(zwp_portal_t portal)
{
	return zwp_thread_mutex_lock(portal->client_initialization_lock);
}
zwp_status_t zwp_portal_put_client_intialize_lock(zwp_portal_t portal)
{
	return zwp_thread_mutex_unlock(portal->client_initialization_lock);
}

zwp_status_t zwp_portal_get_zip_client_ip_settings(zwp_portal_t portal, zwp_zwave_client_ip_settings_t *zip_client_ip_settings)
{
	if(portal->zip_client && zip_client_ip_settings)
	{
		zip_client_ip_settings->ipv4 = portal->zip_client->ipv4;
		memcpy(zip_client_ip_settings->zip_server_address,portal->zip_client->zip_server_address, sizeof(zip_client_ip_settings->zip_server_address));
		zip_client_ip_settings->unsolicited_port_number = portal->zip_client->unsolicited_port_number ;
		zip_client_ip_settings->key_len = portal->zip_client->key_len;
		memcpy(zip_client_ip_settings->dtls_pskey, portal->zip_client->dtls_pskey, sizeof(zip_client_ip_settings->dtls_pskey));
		return ZWP_STATUS_OK;
	}
	else
	{
		return ZWP_STATUS_ERROR;
	}
}

uint8_t zwp_portal_get_gw_auto_set_status(zwp_portal_t portal)
{
	return portal->portal_config->gateway_auto_set_local;
}

/* Getters section ends */

/* Setters sections starts */
/**
 * @brief Update Z-wave client object in the  portal object
 * @param[in]     portal            				Portal object
 * @param[in]     zip_client            			Z-wave client object
 * @retval        ZWP_STATUS_OK     				Success
 * @retval        ZWP_STATUS_ERROR  				Failure
 */

zwp_status_t zwp_portal_set_zip_client(zwp_portal_t portal, zwp_zwave_client_t zip_client)
{
	if(zip_client)
	{
		portal->zip_client = zip_client;
		return ZWP_STATUS_OK;
	}
	else
	{
		return ZWP_STATUS_ERROR;
	}
}

zwp_status_t zwp_portal_set_zip_client_ip_settings(zwp_portal_t portal, zwp_zwave_client_ip_settings_t *zip_client_ip_settings)
{
	if(portal->zip_client && zip_client_ip_settings)
	{
		portal->zip_client->ipv4 = zip_client_ip_settings->ipv4;
		memcpy(portal->zip_client->zip_server_address, zip_client_ip_settings->zip_server_address, sizeof(portal->zip_client->zip_server_address));
		portal->zip_client->unsolicited_port_number = zip_client_ip_settings->unsolicited_port_number;
		portal->zip_client->key_len = zip_client_ip_settings->key_len ;
		memcpy(portal->zip_client->dtls_pskey, zip_client_ip_settings->dtls_pskey, sizeof(portal->zip_client->dtls_pskey));
		return ZWP_STATUS_OK;
	}
	else
	{
		return ZWP_STATUS_ERROR;
	}
}
/* Setters sections end */
#else
/**
 * @brief Get client ID from network ID
 * @param[in]     net_id            Network ID
 * @param[out]    client_id         Client ID
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_client_id_get_from_net_id(char *net_id, uint8_t client_id[8])
{
	unsigned int clnt_id_buf[8];
	int i;
	uint8_t clnt_id[8];
	zwp_status_t return_status;

	return_status = ZWP_STATUS_ERROR;

	if (!net_id)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Net ID is NULL");
		goto l_err_null_net_id;
	}

	if (sscanf(net_id, "%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x",
			&clnt_id_buf[0], &clnt_id_buf[1], &clnt_id_buf[2], &clnt_id_buf[3],
			&clnt_id_buf[4], &clnt_id_buf[5], &clnt_id_buf[6], &clnt_id_buf[7]) == 8)
	{
		for (i = 0; i < 8; i++)
		{
			if (clnt_id_buf[i] <= 0xFF)
			{
				clnt_id[i] = (uint8_t) clnt_id_buf[i];
			}
			else
			{
				ZWP_LOG(ZWP_LOG_ERR, "Invalid value '%s' for Network ID", net_id);
				goto l_err_net_id_parse;
			}
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid value '%s' for Network ID", net_id);
		goto l_err_net_id_parse;
	}

	memcpy(client_id, clnt_id, sizeof(clnt_id));

	return ZWP_STATUS_OK;

l_err_net_id_parse:
l_err_null_net_id:
	return return_status;
}

/**
 * @brief Auto select the first network ID for a given user
 * @param[in]     directory_service Directrory service
 * @param[in]     user              User name
 * @param[out]    net_id_p          Network ID
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call ZWP_FREE(*net_id_p) to free resources
 */
static zwp_status_t zwp_portal_auto_select_network_id(zwp_directory_service_t directory_service, char *user, char **net_id_p)
{
	char *net_id;
	char **directory_service_net_list;
	int directory_service_net_count;

	if (zwp_directory_service_get_network_list_for_user(directory_service, user, &directory_service_net_list, &directory_service_net_count) != ZWP_STATUS_OK)
	{
		goto l_err_networks_list_for_user;
	}

	if (directory_service_net_count <= 0)
	{
		goto l_err_network_list_empty;
	}

	if ((net_id = (char *)ZWP_STRDUP(directory_service_net_list[0])) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_net_id;
	}

	zwp_directory_service_free_network_list_for_user(directory_service_net_list, directory_service_net_count);

	*net_id_p = net_id;

	return ZWP_STATUS_OK;

l_err_net_id:
l_err_network_list_empty:
	zwp_directory_service_free_network_list_for_user(directory_service_net_list, directory_service_net_count);
l_err_networks_list_for_user:
	return ZWP_STATUS_ERROR;
}
#endif

/**
 * @brief Execute network level AVI message
 * @param[in]     portal            Portal object
 * @param[in]     service           Service object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_portal_execute_avi_message_networks(zwp_portal_t portal, zwp_service_t service)
{
	zwp_net_t net;
	zwp_service_networks_t service_net;

#ifdef LOCAL_CLIENT_ENABLE
	char rac[24] = {0};
#else
	char *user;
#endif
	char *net_id = NULL;
	char *net_id_auto = NULL;
	zwp_status_t return_status;
	int network_exit;

	network_exit = 0;

#ifdef LOCAL_CLIENT_ENABLE

	service_net = zwp_avi_service_get_service_next(service);

	snprintf(rac, sizeof(rac), "%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X",
			local_clnt_id[0], local_clnt_id[1], local_clnt_id[2], local_clnt_id[3],local_clnt_id[4], local_clnt_id[5], local_clnt_id[6], local_clnt_id[7]);
	if (zwp_avi_service_networks_set_net_id(service_net, rac) != ZWP_STATUS_OK)
	{
		goto l_err_set_net_id;
	}

#else

	user = zwp_avi_service_get_user(service);

	service_net = zwp_avi_service_get_service_next(service);
	net_id = zwp_avi_service_networks_get_net_id(service_net);
	net_id_auto = NULL;

	if (!net_id || !net_id[0])
	{
		if (zwp_portal_auto_select_network_id(portal->directory_service, user, &net_id_auto) != ZWP_STATUS_OK)
		{
			goto l_err_auto_select_network_id;
		}

		net_id = net_id_auto;

		ZWP_LOG(ZWP_LOG_DEBUG, "Network id: '%s' [Auto selected]", net_id);

		if (zwp_avi_service_networks_set_net_id(service_net, net_id) != ZWP_STATUS_OK)
		{
			goto l_err_set_net_id;
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Network id: '%s'", net_id);
	}

	if (zwp_directory_service_check_network_authorization_for_user(portal->directory_service, user, net_id) != ZWP_STATUS_OK)
	{
		goto l_err_net_authz;
	}
#endif

	// The call increments the reference count of 'net'
	if ((return_status = zwp_portal_net_table_get_from_ra_code(portal, net_id, &net)) != ZWP_STATUS_OK)
	{
		// Exception cases:
		// Allow continuation of these messages even when not present in the list of connected gateways.
		if ((return_status == ZWP_STATUS_ENOENT)
				&& (
#ifndef LOCAL_CLIENT_ENABLE
						!strcmp(zwp_avi_service_get_message_name(service), "network_gateway_status_get")
						||
#endif
						!strcmp(zwp_avi_service_get_message_name(service), "network_summary_get")
						|| !strcmp(zwp_avi_service_get_message_name(service), "network_version_get")
					))
		{
			net = NULL;
		}
		else
		{
			goto l_err_get_net;
		}
	}

	if (!strcmp(zwp_avi_service_get_message_name(service), "network_exit"))
	{
		network_exit = 1;
	}

	if (zwp_net_execute_avi_message_networks(net, zwp_avi_service_get_service_next(service)) != ZWP_STATUS_OK)
	{
		goto l_err_net_execute_networks;
	}

	// If "network_exit" call, 'net' is no longer valid
	if (network_exit)
	{
		net = NULL;
	}

	ZWP_FREE(net_id_auto);

	if (net && !network_exit)
	{
		zwp_net_reference_count_decrement(net);
	}

	return ZWP_STATUS_OK;

l_err_net_execute_networks:
	if (net && !network_exit)
	{
		zwp_net_reference_count_decrement(net);
	}
l_err_get_net:
#ifndef LOCAL_CLIENT_ENABLE
l_err_net_authz:
#endif
l_err_set_net_id:
#ifndef LOCAL_CLIENT_ENABLE
	ZWP_FREE(net_id_auto);
l_err_auto_select_network_id:
#endif
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_portal_execute_avi_message(zwp_portal_t portal, zwp_service_t service)
{
	zwp_avi_message_type_t message_type;
	zwp_avi_message_level_t message_level;

	message_type = zwp_avi_service_get_message_type(service);
	message_level = zwp_avi_service_get_message_level(service);

	if (message_type == ZWP_AVI_MESSAGE_TYPE_NETWORKS)
	{
		if (zwp_portal_execute_avi_message_networks(portal, service) != ZWP_STATUS_OK)
		{
			goto l_err_execute_avi_message_networks;
		}
	}
	else if (message_type == ZWP_AVI_MESSAGE_TYPE_ROOT)
	{
		if (zwp_portal_execute_avi_message_root(portal, service) != ZWP_STATUS_OK)
		{
			goto l_err_execute_avi_message_root;
		}
	}
	else if (message_type == ZWP_AVI_MESSAGE_TYPE_IFTTT)
	{
		if ((message_level == ZWP_AVI_MESSAGE_LEVEL_NETWORKS) ||
			(message_level == ZWP_AVI_MESSAGE_LEVEL_INTERFACES))
		{
			if (zwp_portal_execute_avi_message_networks(portal, service) != ZWP_STATUS_OK)
			{
				goto l_err_execute_avi_message_ifttt_networks;
			}
		}
	}
	else
	{
		// Unlikely
		goto l_err_no_match;
	}

	return ZWP_STATUS_OK;

l_err_no_match:
l_err_execute_avi_message_ifttt_networks:
l_err_execute_avi_message_root:
l_err_execute_avi_message_networks:
	return ZWP_STATUS_ERROR;
}

#ifndef LOCAL_CLIENT_ENABLE
/**
 * @brief Notification function for a new connection from portal client
 * @param[in]     clnt_fd           Portal client socket file descriptor
 * @param[in]     clnt_ssl	        Portal client SSL object pointer
 * @param[in]     clnt_prof         Portal client profile used
 * @param[in]     user_param        User defined parameter
 * @retval        !0                If the new portal client connection is accepted
 * @retval        0                 If the new portal client connection is rejected
 */
static int ZWP_HCAPI_NOTIFICATION zwp_portal_client_new(int clnt_fd, void *clnt_ssl, clnt_prof_t *clnt_prof, void *user_param)
{
	zwp_portal_t portal;
	zwp_portal_new_client_t new_client_parameter;
	char ra_code[24];
	zwp_net_t net;

	portal = (zwp_portal_t)user_param;

	snprintf(ra_code, sizeof(ra_code), "%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X",
			clnt_prof->clnt_id[0], clnt_prof->clnt_id[1], clnt_prof->clnt_id[2], clnt_prof->clnt_id[3],
			clnt_prof->clnt_id[4], clnt_prof->clnt_id[5], clnt_prof->clnt_id[6], clnt_prof->clnt_id[7]);

	ZWP_LOG(ZWP_LOG_DEBUG, "New Client with Remote Access Code '%s'", ra_code);

	ZWP_LOG(ZWP_LOG_DEBUG, "Tearing down any existing connection to portal from the same client");

	zwp_portal_client_kill_net_by_ra_code(portal, ra_code);

	new_client_parameter.file_descriptor = clnt_fd;
	new_client_parameter.SSL_object = clnt_ssl;
	new_client_parameter.profile = clnt_prof;

	if (zwp_portal_client_spawn_net(portal, &new_client_parameter, &net) != ZWP_STATUS_OK)
	{
		goto l_err_spawn_net;
	}

	return 1;

l_err_spawn_net:
	return 0;
}
#endif
#ifndef LOCAL_CLIENT_ENABLE
/**
 * @brief Allocate a new network object and start initializing it
 * @param[in]     portal            Portal object
 * @param[in]     new_client_parameter  Parameters for new portal client
 * @param[out]    net_p             Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 * @see zwp_portal_client_kill_net_async, zwp_portal_client_kill_net and zwp_portal_client_kill_net_by_ra_code
 */
static zwp_status_t zwp_portal_client_spawn_net(zwp_portal_t portal, zwp_portal_new_client_t *new_client_parameter, zwp_net_t *net_p)
#else
zwp_status_t zwp_portal_client_spawn_net(zwp_portal_t portal, zwp_portal_new_client_t *new_client_parameter, zwp_net_t *net_p)
#endif
{
	zwp_net_t net;
#ifdef LOCAL_CLIENT_ENABLE
	zwp_net_zip_server_config_t server_config;
#endif

	ZWP_LOG(ZWP_LOG_DEBUG, "Network spawn: Allocating new network");

#ifdef LOCAL_CLIENT_ENABLE
	if (zwp_net_alloc(&net, 0, NULL, NULL, portal) != ZWP_STATUS_OK)
#else
	if (zwp_net_alloc(&net, new_client_parameter->file_descriptor, new_client_parameter->SSL_object,
			new_client_parameter->profile, portal) != ZWP_STATUS_OK)
#endif
	{
		goto l_err_net_alloc;
	}
	
#ifdef LOCAL_CLIENT_ENABLE
	server_config.ipv4 = portal->zip_client->ipv4;
	server_config.unsolicited_port_number = portal->zip_client->unsolicited_port_number;
	memcpy(server_config.zip_server_address, portal->zip_client->zip_server_address, sizeof(server_config.zip_server_address));
	server_config.key_len = portal->zip_client->key_len;
	memcpy(server_config.dtls_pskey, portal->zip_client->dtls_pskey, sizeof(server_config.dtls_pskey));
	zwp_net_set_zip_serverconfig(net, &server_config);
	zwp_net_set_operation(net, ZWNET_OP_INITIALIZE);
	zwp_zwave_client_set_net(portal->zip_client, net);
#endif

	ZWP_LOG(ZWP_LOG_DEBUG, "Network spawn: new network allocated: %p", net);

	if (zwp_portal_net_table_add(portal, net) != ZWP_STATUS_OK)
	{
		goto l_err_net_table_add;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Network spawn: network added to list of connected networks");

	if (zwp_net_start(net) != ZWP_STATUS_OK)
	{
		goto l_err_net_start;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Network spawn: network started");

	*net_p = net;

	return ZWP_STATUS_OK;

l_err_net_start:
	zwp_portal_net_table_remove(portal, net, 1);
l_err_net_table_add:
	ZWP_LOG(ZWP_LOG_DEBUG, "Network spawn: network about to be freed %p", net);
	zwp_net_free(net);
	ZWP_LOG(ZWP_LOG_DEBUG, "Network spawn: network freed %p", net);
l_err_net_alloc:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_portal_client_kill_net_async(zwp_portal_t portal, zwp_net_t net, int check_net_exists)
{
	zwp_thread_t thread;

	ZWP_LOG(ZWP_LOG_DEBUG, "Network kill: Network about to be killed async %p", net);

	if (zwp_portal_net_table_remove(portal, net, check_net_exists) != ZWP_STATUS_OK)
	{
		goto l_err_remove_net;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Network kill: Stopping network async %p", net);

	//Create as joinable thread here so that if portal shut down before this thread finishes execution, there is a way
	//for portal to wait for the completion of this thread before shutting down.
	if (zwp_thread_create(&thread, zwp_net_stop_async_handler, net, portal->portal_pool, ZWP_THREAD_CREATE_JOINABLE) != ZWP_STATUS_OK)
	//if (zwp_thread_create(&thread, zwp_net_stop_async_handler, net, portal->portal_pool, ZWP_THREAD_CREATE_DETACHED) != ZWP_STATUS_OK)
	{
		goto l_err_net_stop;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Network kill: Stop network async submitted %p", net);

	return ZWP_STATUS_OK;

l_err_net_stop:
l_err_remove_net:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_portal_client_kill_net(zwp_portal_t portal, zwp_net_t net, int check_net_exists)
{
	ZWP_LOG(ZWP_LOG_DEBUG, "Network kill: Network about to be killed %p", net);

	if (zwp_portal_net_table_remove(portal, net, check_net_exists) != ZWP_STATUS_OK)
	{
		goto l_err_remove_net;
	}

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
l_err_remove_net:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_portal_client_kill_net_by_ra_code(zwp_portal_t portal, char *ra_code)
{
	zwp_net_t net;

	ZWP_LOG(ZWP_LOG_DEBUG, "Network kill: Network about to be killed '%s'", ra_code);

	if (zwp_portal_net_table_remove_by_net_id(portal, ra_code, &net) != ZWP_STATUS_OK)
	{
		goto l_err_remove_net;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Network kill: Stopping network %p", net);

	if (zwp_net_stop(net) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_WARNING, "zwp_net_stop() failed. 'net' not freed. Possible memory leak");
		goto l_err_net_stop;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Network about to be freed %p", net);

	zwp_net_free(net);
	
#ifdef LOCAL_CLIENT_ENABLE
	zwp_zwave_client_set_net(portal->zip_client, NULL);
#endif

	ZWP_LOG(ZWP_LOG_DEBUG, "Network freed %p", net);

	return ZWP_STATUS_OK;

l_err_net_stop:
l_err_remove_net:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Add a network object to portal object
 * @param[in]     portal            Portal object
 * @param[in]     net               Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_portal_net_table_add(zwp_portal_t portal, zwp_net_t net)
{
	if (zwp_thread_mutex_lock(portal->net_table_lock) != ZWP_STATUS_OK)
	{
		goto l_err_net_table_lock;
	}

	zwp_hash_set(portal->net_table, zwp_net_get_hash_key(net), zwp_net_get_hash_key_size(net), net);

	zwp_thread_mutex_unlock(portal->net_table_lock);

	return ZWP_STATUS_OK;

l_err_net_table_lock:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Remove a network object from portal object
 * @param[in]     portal            Portal object
 * @param[in]     net               Network object
 * @param[in]     check_net_exists  If 0, net exists check is skipped. Else, the check is done before removing.
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_portal_net_table_remove(zwp_portal_t portal, zwp_net_t net, int check_net_exists)
{
	zwp_hash_index_t hash_index;
	zwp_net_t net_element;
	zwp_status_t return_status;

	if (zwp_thread_mutex_lock(portal->net_table_lock) != ZWP_STATUS_OK)
	{
		goto l_err_net_table_lock;
	}

	hash_index = NULL;

	if (check_net_exists)
	{
		//Iterate through the hash table of networks
		for (hash_index = zwp_hash_first(portal->portal_pool, portal->net_table); hash_index; hash_index = zwp_hash_next(hash_index))
		{
			zwp_hash_this(hash_index, NULL, NULL, (void*) &net_element);

			if (net == net_element)
			{
				break;
			}
		}
	}

	if (!check_net_exists || hash_index)
	{
		zwp_hash_set(portal->net_table, zwp_net_get_hash_key(net), zwp_net_get_hash_key_size(net), NULL);

		return_status = ZWP_STATUS_OK;
	}
	else
	{
		ZWP_LOG(ZWP_LOG_INFO, "Unable to find network [%p] in the list of connected networks", net);

		return_status = ZWP_STATUS_ENOENT;
	}

	zwp_thread_mutex_unlock(portal->net_table_lock);

	return return_status;

	zwp_thread_mutex_unlock(portal->net_table_lock);
l_err_net_table_lock:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Remove a network object from portal object by its network ID
 * @param[in]     portal            Portal object
 * @param[in]     net_id            Network ID
 * @param[out]    net_p             The removed network
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_portal_net_table_remove_by_net_id(zwp_portal_t portal, char *net_id, zwp_net_t *net_p)
{
	uint8_t client_id[8];
	zwp_net_t net;
	zwp_status_t return_status;

	//Local client doen't have any network id
#ifdef LOCAL_CLIENT_ENABLE
	memcpy(client_id, local_clnt_id, sizeof(client_id));
#else
	if (zwp_client_id_get_from_net_id(net_id, client_id) != ZWP_STATUS_OK)
	{
		goto l_err_client_id_get;
	}
#endif

	if (zwp_thread_mutex_lock(portal->net_table_lock) != ZWP_STATUS_OK)
	{
		goto l_err_net_table_lock;
	}

	if ((net = zwp_hash_get(portal->net_table, client_id, sizeof(client_id))) != NULL)
	{
		zwp_hash_set(portal->net_table, client_id, sizeof(client_id), NULL);

		*net_p = net;

		return_status = ZWP_STATUS_OK;
	}
	else
	{
		ZWP_LOG(ZWP_LOG_INFO, "Unable to find '%s' in the list of connected networks", net_id);

		return_status = ZWP_STATUS_ENOENT;
	}

	zwp_thread_mutex_unlock(portal->net_table_lock);

	return return_status;

	zwp_thread_mutex_unlock(portal->net_table_lock);
l_err_net_table_lock:
#ifndef LOCAL_CLIENT_ENABLE
l_err_client_id_get:
#endif
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Get the network object corresponding to a client ID
 * @param[in]     portal            Portal object
 * @param[in]     clnt_id           Client ID
 * @param[in]     clnt_id_size      Client ID size
 * @param[out]    net_p             Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * This function increments the reference count on @a net_p.
 * So call zwp_net_reference_count_decrement() when the object is no longer required.
 */
static zwp_status_t zwp_portal_net_table_get(zwp_portal_t portal, const uint8_t *clnt_id, size_t clnt_id_size, zwp_net_t *net_p)
{
	zwp_net_t net;

	if (zwp_thread_mutex_lock(portal->net_table_lock) != ZWP_STATUS_OK)
	{
		goto l_err_net_table_lock;
	}

	if ((net = zwp_hash_get(portal->net_table, clnt_id, clnt_id_size)) == NULL)
	{
		goto l_err_hash_get;
	}

	zwp_net_reference_count_increment(net);

	zwp_thread_mutex_unlock(portal->net_table_lock);

	*net_p = net;

	return ZWP_STATUS_OK;

l_err_hash_get:
	zwp_thread_mutex_unlock(portal->net_table_lock);
l_err_net_table_lock:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Get the network object corresponding to a client ID
 * @param[in]     portal            Portal object
 * @param[in]     ra_code           Remote access code
 * @param[out]    net_p             Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * This function increments the reference count on @a net_p.
 * So call zwp_net_reference_count_decrement() when the object is no longer required.
 */
static zwp_status_t zwp_portal_net_table_get_from_ra_code(zwp_portal_t portal, char *ra_code, zwp_net_t *net_p)
{
#ifndef LOCAL_CLIENT_ENABLE
	unsigned int ra_code_buf[8];
	int i;
#endif
	uint8_t clnt_id[8];
	zwp_status_t return_status;

	return_status = ZWP_STATUS_ERROR;
#ifndef LOCAL_CLIENT_ENABLE
	if (!ra_code)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Net ID is NULL");
		goto l_err_null_net_id;
	}

	if (sscanf(ra_code, "%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x",
			&ra_code_buf[0], &ra_code_buf[1], &ra_code_buf[2], &ra_code_buf[3],
			&ra_code_buf[4], &ra_code_buf[5], &ra_code_buf[6], &ra_code_buf[7]) == 8)
	{
		for (i = 0; i < 8; i++)
		{
			if (ra_code_buf[i] <= 0xFF)
			{
				clnt_id[i] = (uint8_t) ra_code_buf[i];
			}
			else
			{
				ZWP_LOG(ZWP_LOG_ERR, "Invalid value '%s' for Network ID", ra_code);
				goto l_err_ra_code_parse;
			}
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid value '%s' for Network ID", ra_code);
		goto l_err_ra_code_parse;
	}
#else
	memcpy(clnt_id, local_clnt_id, sizeof(clnt_id));
#endif
	
	if (zwp_portal_net_table_get(portal, clnt_id, sizeof(clnt_id), net_p) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_INFO, "Unable to find '%s' in the list of connected networks", ra_code);
		return_status = ZWP_STATUS_ENOENT;
		goto l_err_get_net;
	}

	return ZWP_STATUS_OK;

l_err_get_net:
#ifndef LOCAL_CLIENT_ENABLE
l_err_ra_code_parse:
l_err_null_net_id:
#endif
	return return_status;
}

zwp_status_t zwp_portal_start(zwp_portal_t *portal_p, zwp_portal_config_t portal_config,
		zwp_avi_t avi, zwp_directory_service_t directory_service, zwp_ifttt_t ifttt)
{
	static zwp_portal_t g_portal;
	zwp_portal_t portal;
#ifndef LOCAL_CLIENT_ENABLE
	zwportal_init_t init_config;
	clnt_prof_t *profile_list;
	int profile_count;
#endif
	char current_thread_name[ZWP_THREAD_NAME_SIZE];

	//Enforce singleton
	if (g_portal)
	{
		ZWP_LOG(ZWP_LOG_WARNING, ZWP_MSG_SINGLETON_MULTIPLE_CALLS);
		*portal_p = g_portal;

		return ZWP_STATUS_OK;
	}

	if ((portal = (zwp_portal_t)ZWP_MALLOC(sizeof(*portal))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_portal_alloc;
	}

	//Link with configuration
	portal->portal_config = portal_config;

	//Link with AVI handle
	portal->avi = avi;

	//Link with directory service handle
	portal->directory_service = directory_service;
#ifndef LOCAL_CLIENT_ENABLE
	//Get the list of ZIP Gateway profiles
	if (zwp_directory_service_zipgw_profile_list_get(directory_service, &profile_list, &profile_count) != ZWP_STATUS_OK)
	{
		goto l_err_directory_service_zipgw_profile_get;
	}
#endif

	//Link with IFTTT handle
	portal->ifttt = ifttt;

	//Create memory pool for networks
	if (zwp_pool_create(&portal->portal_pool, NULL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create pool for network level");
		goto l_err_portal_pool_create;
	}

	//Create network table
	if (zwp_hash_create(&portal->net_table, portal->portal_pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create hash table for networks");
		goto l_err_net_table_create;
	}

	//Create mutex lock for network table
	if (zwp_thread_mutex_create(&portal->net_table_lock, portal->portal_pool, ZWP_THREAD_MUTEX_CREATE_NORMAL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create lock for networks hash table");
		goto l_err_net_table_lock_create;
	}
#ifdef LOCAL_CLIENT_ENABLE
	if (zwp_thread_mutex_create(&portal->client_initialization_lock,   portal->portal_pool, ZWP_THREAD_MUTEX_CREATE_NORMAL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create lock for zip client initialization");
		goto l_err_client_initialization_lock_create;
	}
#endif

	//Create stop thread array
	if (zwu_array_list_create(0, &portal->net_stop_thread_array) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	//Create mutex lock for stop thread array
	if (zwp_thread_mutex_create(&portal->net_stop_thread_array_lock, portal->portal_pool, ZWP_THREAD_MUTEX_CREATE_NORMAL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create lock for stop network thread array");
		goto l_err_value_list_lock_create;
	}

	zwp_thread_name_set(ZWP_SCENE_THREAD_NAME, current_thread_name);
    //Initialize scene module
    if (zwscn_init() != ZWSCN_ERR_NONE)
    {
		ZWP_LOG(ZWP_LOG_ERR, "Failed to init scene module");
		 zwp_thread_name_set(current_thread_name, NULL);
		goto l_err_scene_init;
    }
    zwp_thread_name_set(current_thread_name, NULL);

#ifndef LOCAL_CLIENT_ENABLE
	memset(&init_config, 0, sizeof(init_config));

	init_config.clnt_prof = profile_list;
	init_config.prof_cnt = profile_count;

	init_config.cb = zwp_portal_client_new;
	init_config.usr_param = portal;

	init_config.ca_file = portal_config->security.CA_certificate_file;
	init_config.ssl_file = portal_config->security.SSL_certificate_file;
	init_config.pvt_key_file = portal_config->security.private_key_file;

	init_config.svr_port = portal_config->listen_port;

	zwp_thread_name_set(ZWP_HCAPI_PORTAL_INIT_THREAD_NAME, current_thread_name);
	if ((portal->portal_handle = zwportal_init(&init_config)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to initialize portal");
		zwp_thread_name_set(current_thread_name, NULL);
		goto l_err_zwportal_init;
	}
	zwp_thread_name_set(current_thread_name, NULL);
#else
	portal->portal_handle = NULL;
	if (zwp_zwave_client_init(portal) != ZWP_STATUS_OK)
	{
		goto l_err_zwave_client_init;
	}
#endif

#ifndef LOCAL_CLIENT_ENABLE
	//Release the list of ZIP Gateway profiles
	zwp_directory_service_zipgw_profile_list_free(profile_list);
#endif

	g_portal = *portal_p = portal;

	return ZWP_STATUS_OK;
#ifdef LOCAL_CLIENT_ENABLE
l_err_zwave_client_init:
#else
l_err_zwportal_init:
#endif
    zwscn_shutdown();
l_err_scene_init:
#ifdef LOCAL_CLIENT_ENABLE
	zwp_thread_mutex_destroy(portal->client_initialization_lock);
l_err_client_initialization_lock_create:
#endif
	zwp_thread_mutex_destroy(portal->net_stop_thread_array_lock);
l_err_value_list_lock_create:
	zwu_array_list_destroy(portal->net_stop_thread_array);
l_err_value_list_create:
	zwp_thread_mutex_destroy(portal->net_table_lock);
l_err_net_table_lock_create:
	zwp_hash_destroy(portal->net_table);
l_err_net_table_create:
	zwp_pool_destroy(portal->portal_pool);
l_err_portal_pool_create:
#ifndef LOCAL_CLIENT_ENABLE
	zwp_directory_service_zipgw_profile_list_free(profile_list);
l_err_directory_service_zipgw_profile_get:
#endif
	ZWP_FREE(portal);
l_err_portal_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_portal_stop(zwp_portal_t portal)
{
	zwp_net_t net;
	zwp_hash_index_t hash_index;
	zwp_thread_t   net_stop_thread;

#ifndef LOCAL_CLIENT_ENABLE
	zwportal_shutdown(portal->portal_handle);
#endif

    //Shutdown scene module
    zwscn_shutdown();

	//Iterate through the hash table of networks
	for (hash_index = zwp_hash_first(portal->portal_pool, portal->net_table); hash_index; hash_index = zwp_hash_next(hash_index)) {
		zwp_hash_this(hash_index, NULL, NULL, (void*) &net);

		zwp_portal_net_table_remove(portal, net, 0);

		if (zwp_net_stop(net) == ZWP_STATUS_OK)
		{
			zwp_net_free(net);
#ifdef LOCAL_CLIENT_ENABLE
			zwp_zwave_client_set_net(portal->zip_client, NULL);
#endif
		}
	}
#ifndef LOCAL_CLIENT_ENABLE
	zwportal_exit(portal->portal_handle);
#else
	zwp_zwave_client_exit(portal->zip_client);
	zwp_thread_mutex_destroy(portal->client_initialization_lock);
#endif

	while (zwp_portal_thread_handle_list_pop(portal, &net_stop_thread) == ZWP_STATUS_OK)
	{
		zwp_status_t status;

		ZWP_LOG(ZWP_LOG_DEBUG, "Joining thread %p before main thread exit", net_stop_thread);

		if (zwp_thread_join(net_stop_thread, &status) != ZWP_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to join thread handler %p", net_stop_thread);
		}
	}

	zwu_array_list_destroy(portal->net_stop_thread_array);

	zwp_thread_mutex_destroy(portal->net_stop_thread_array_lock);

	zwp_thread_mutex_destroy(portal->net_table_lock);
	zwp_hash_destroy(portal->net_table);
	zwp_pool_destroy(portal->portal_pool);
	ZWP_FREE(portal);

	return;
}

zwp_status_t zwp_portal_config_get_instance(zwp_portal_config_t *portal_config_p, const config_t *config_parser, const char *config_dir)
{
	static zwp_portal_config_t g_portal_config;
	zwp_portal_config_t portal_config;
	int listen_port;
#ifdef LOCAL_CLIENT_ENABLE
	int auto_set_local;
#endif

	//Enforce singleton
	if (g_portal_config)
	{
		ZWP_LOG(ZWP_LOG_WARNING, ZWP_MSG_SINGLETON_MULTIPLE_CALLS);
		*portal_config_p = g_portal_config;

		return ZWP_STATUS_OK;
	}

	if ((portal_config = (zwp_portal_config_t)ZWP_MALLOC(sizeof(*portal_config))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_portal_config_alloc;
	}

#ifdef LOCAL_CLIENT_ENABLE
	//auto set to local IP
	auto_set_local = 1;
	if (config_lookup_int(config_parser, "local.gw_auto_set_to_local", &auto_set_local) != CONFIG_TRUE)
	{
		ZWP_LOG(ZWP_LOG_WARNING, "Failed to lookup auto set to local IP. Proceeding with default port %d", auto_set_local);
	}

	if ((auto_set_local < 0) || (auto_set_local > 1))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Configuration 'local.listen_port' is out of valid range.Setting it to Enabled");
		auto_set_local = 1;
	}
	portal_config->gateway_auto_set_local = (uint8_t)auto_set_local;
	ZWP_LOG(ZWP_LOG_DEBUG, "gateway_auto_set_local = %u", portal_config->gateway_auto_set_local);
#endif

	//Listening port
	listen_port = ZWP_OPTION_DEFAULT_LISTEN_PORT;
	if (config_lookup_int(config_parser, "portal.listen_port", &listen_port) != CONFIG_TRUE)
	{
		ZWP_LOG(ZWP_LOG_WARNING, "Failed to lookup portal listening port. Proceeding with default port %d", ZWP_OPTION_DEFAULT_LISTEN_PORT);
	}
	if ((listen_port < 0) || (listen_port > 0xFFFF))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Configuration 'portal.listen_port' is out of valid range");
		goto l_err_invalid_listen_port;
	}
	portal_config->listen_port = (uint16_t)listen_port;
	ZWP_LOG(ZWP_LOG_DEBUG, "Portal listening port: %u", portal_config->listen_port);

	//Local state dir
	//Get 'localstatedir' relative to 'sysconfigdir' (autotools)
	if ((portal_config->local_state_dir = (char *)ZWP_MALLOC(strlen(config_dir) + 1 + sizeof("../var"))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_local_state_dir_alloc;
	}
	sprintf(portal_config->local_state_dir, "%s/../var", config_dir);
	ZWP_LOG(ZWP_LOG_DEBUG, "Local state dir: %s", portal_config->local_state_dir);

	//CA certificate file path
	if (zwp_config_lookup_path(config_parser, "portal.security.CA_certificate_file", config_dir,
			&portal_config->security.CA_certificate_file) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to lookup CA certificate path");
		goto l_err_lookup_ca_certificate;
	}

	//SSL certificate file path
	if (zwp_config_lookup_path(config_parser, "portal.security.SSL_certificate_file", config_dir,
			&portal_config->security.SSL_certificate_file) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to lookup SSL certificate path");
		goto l_err_lookup_ssl_certificate;
	}

	//Private key file
	if (zwp_config_lookup_path(config_parser, "portal.security.private_key_file", config_dir,
			&portal_config->security.private_key_file) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to lookup private key file path");
		goto l_err_lookup_private_key;
	}

	//Device specific configuration file
	if (zwp_config_lookup_path(config_parser, "portal.device_specific_configuration_file", config_dir,
			&portal_config->device_specific_configuration_file) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to lookup device specific configuration file path");
		goto l_err_lookup_device_specific_configuration;
	}

	//Z-Ware Report wait timeout
	portal_config->network.interface.report_wait_timeout = ZWP_OPTION_DEFAULT_REPORT_WAIT_TIMEOUT;
	if (config_lookup_int(config_parser, "portal.network.interface.report_wait_timeout",
			&portal_config->network.interface.report_wait_timeout) != CONFIG_TRUE)
	{
		ZWP_LOG(ZWP_LOG_WARNING, "Failed to lookup Z-Wave Report wait timeout. Using default %d seconds", ZWP_OPTION_DEFAULT_REPORT_WAIT_TIMEOUT);
	}
	ZWP_LOG(ZWP_LOG_DEBUG, "Z-Wave Report wait timeout: %d seconds", portal_config->network.interface.report_wait_timeout);

	g_portal_config = *portal_config_p = portal_config;

	return ZWP_STATUS_OK;

	zwp_config_lookup_path_free(portal_config->device_specific_configuration_file);
l_err_lookup_device_specific_configuration:
	zwp_config_lookup_path_free(portal_config->security.private_key_file);
l_err_lookup_private_key:
	zwp_config_lookup_path_free(portal_config->security.SSL_certificate_file);
l_err_lookup_ssl_certificate:
	zwp_config_lookup_path_free(portal_config->security.CA_certificate_file);
l_err_lookup_ca_certificate:
	ZWP_FREE(portal_config->local_state_dir);
l_err_local_state_dir_alloc:
l_err_invalid_listen_port:
	ZWP_FREE(portal_config);
l_err_portal_config_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_portal_config_free(zwp_portal_config_t portal_config)
{
	zwp_config_lookup_path_free(portal_config->device_specific_configuration_file);
	zwp_config_lookup_path_free(portal_config->security.private_key_file);
	zwp_config_lookup_path_free(portal_config->security.SSL_certificate_file);
	zwp_config_lookup_path_free(portal_config->security.CA_certificate_file);
	ZWP_FREE(portal_config->local_state_dir);
	ZWP_FREE(portal_config);
}


/**
zwp_portal_get_net - Get network handle based on the given network id
@param[in]	portal	        Portal context
@param[in]	net_id	        Network id (e.g. 00-1E-32-FF-FF-12-49-A3)
@param[out]	net_p	        Network handle
@return ZWP_STATUS_XXX
@post   Caller has to free the returned network handle using zwp_portal_free_net() if return status is ZWP_STATUS_OK
*/
zwp_status_t zwp_portal_get_net(zwp_portal_t portal, char *net_id, zwp_net_t *net_p)
{
	int             i;
	unsigned int    rac[8];
	uint8_t         clnt_id[8];

	if (!net_id)
	{
        return ZWP_STATUS_ERROR;
	}

	if (sscanf(net_id, "%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x",
                        &rac[0], &rac[1], &rac[2], &rac[3], &rac[4], &rac[5], &rac[6], &rac[7]) == 8)
	{
		for (i=0; i< 8; i++)
		{
			if (rac[i] <= 0xFF)
			{
				clnt_id[i] = (uint8_t)rac[i];
			}
			else
			{
				ZWP_LOG(ZWP_LOG_ERR, "Invalid value '%s' for Network ID", net_id);
                return ZWP_STATUS_ERROR;
			}
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid value '%s' for Network ID", net_id);
        return ZWP_STATUS_ERROR;
	}

	if (zwp_portal_net_table_get(portal, clnt_id, sizeof(clnt_id), net_p) != ZWP_STATUS_OK)
	{
		return ZWP_STATUS_ENOENT;
	}

	return ZWP_STATUS_OK;
}


/**
zwp_portal_free_net - Free the network handle returned by call to zwp_portal_get_net()
@param[in]	net	      Network handle
@return
*/
void zwp_portal_free_net(zwp_net_t net)
{
	if (net)
	{
		zwp_net_reference_count_decrement(net);
	}
}

/**
zwp_portal_thread_handle_list_add - Add a thread handle to portal thread handle array list
* @param[in]	portal	        Portal context
* @param[in]	thread	        thread handle
* @return ZWP_STATUS_XXX
*/
zwp_status_t zwp_portal_thread_handle_list_add(zwp_portal_t portal, zwp_thread_t thread)
{
	if (zwp_thread_mutex_lock(portal->net_stop_thread_array_lock) != ZWP_STATUS_OK)
	{
		goto l_err_thread_handle_list_lock;
	}

	zwu_array_list_add(portal->net_stop_thread_array, thread);

	zwp_thread_mutex_unlock(portal->net_stop_thread_array_lock);

	return ZWP_STATUS_OK;

l_err_thread_handle_list_lock:
	return ZWP_STATUS_ERROR;
}

/**
zwp_portal_thread_handle_list_remove - Find the thread handle in portal thread handle array list and remove it
* @param[in]	portal	        Portal context
* @param[in]	thread	        thread handle
* @return ZWP_STATUS_OK			Found the thread handle and successfully remove it \n
*	      ZWP_STATUS_ENOENT		The thread handle is not found in the array list \n
*		  ZWP_STATUS_ERROR		Internal error
*/
zwp_status_t zwp_portal_thread_handle_list_remove(zwp_portal_t portal, zwp_thread_t thread)
{
	zwp_status_t return_status = ZWP_STATUS_ENOENT;

	if (zwp_thread_mutex_lock(portal->net_stop_thread_array_lock) != ZWP_STATUS_OK)
	{
		goto l_err_thread_handle_list_lock;
	}

	if (zwu_array_list_contains(portal->net_stop_thread_array, thread))
	{
		zwu_array_list_remove(portal->net_stop_thread_array, thread);

		return_status = ZWP_STATUS_OK;
	}

	zwp_thread_mutex_unlock(portal->net_stop_thread_array_lock);

	return return_status;

	zwp_thread_mutex_unlock(portal->net_stop_thread_array_lock);
l_err_thread_handle_list_lock:
	return ZWP_STATUS_ERROR;
}

/**
zwp_portal_thread_handle_list_pop - Pop a thread handle from portal thread handle array list
* @param[in]	portal	        Portal context
* @param[out]	pthread	        thread handle that popped
* @return ZWP_STATUS_OK			Found a thread handle in the list and pop it\n
*	      ZWP_STATUS_ENOENT		The list is empty and nothing to pop \n
*		  ZWP_STATUS_ERROR		Internal error
* @note If a thread handle is popped by this function, the thread handle will be removed after
*		this function. It is considered as an atomic action.
*/
zwp_status_t zwp_portal_thread_handle_list_pop(zwp_portal_t portal, zwp_thread_t *pthread)
{
	zwp_status_t return_status = ZWP_STATUS_ENOENT;

	if (zwp_thread_mutex_lock(portal->net_stop_thread_array_lock) != ZWP_STATUS_OK)
	{
		goto l_err_thread_handle_list_lock;
	}

	if (zwu_array_list_size(portal->net_stop_thread_array) > 0)
	{
		*pthread = zwu_array_list_get(portal->net_stop_thread_array, 0);
		zwu_array_list_remove_at_index(portal->net_stop_thread_array, 0);

		return_status = ZWP_STATUS_OK;
	}

	zwp_thread_mutex_unlock(portal->net_stop_thread_array_lock);

	return return_status;

	zwp_thread_mutex_unlock(portal->net_stop_thread_array_lock);
l_err_thread_handle_list_lock:
	return ZWP_STATUS_ERROR;
}
