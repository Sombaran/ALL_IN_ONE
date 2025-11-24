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
 * @file    zwp_network.h
 *
 * @brief   Z-Wave Network module
 * @details Handles network level of Z-Wave network tree.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-06-11
 * - Initial version
 */

#ifndef ZWP_NETWORK_H_
#define ZWP_NETWORK_H_

/**
 * \addtogroup  zwportaldzwave
 * @{
 */

#include "zwp_network_define.h"
#include "zwp_descriptor_define.h"
#include "zwp_avi_service_define.h"
#include "zwp_status.h"
#include "zwp_hcapi.h"
#include "zwp_portal.h"
#include "zwp_thread.h"
#include "zwp_hcapi.h"
#include "zwp_hcapi_aal.h"
#include "zwp_avi.h"
#include "zwu_time.h"
#include "zwp_thread_cond.h"
#include "zwp_db.h"

/** Network not initialized */
#define ZWP_NET_INITIALIZE_STATE_NONE               0
/** Initializing network */
#define ZWP_NET_INITIALIZE_STATE_INITIALIZING       1
/** Network initialized */
#define ZWP_NET_INITIALIZE_STATE_INITIALIZED        2
/** Loading network information */
//#define ZWP_NET_INITIALIZE_STATE_LOADING_NET_INFO   3
/** Network initialization completed */
//#define ZWP_NET_INITIALIZE_STATE_COMPLETED          4
/** Network shutting down */
#define ZWP_NET_INITIALIZE_STATE_SHUTTING_DOWN      5

/** Network transmit status not specified */
#define ZWP_NET_TRANSMIT_STATUS_NONE                (-1)

/** Network operation status - No network - Network uninitialized? */
#define ZWP_NET_OPERATION_STATUS_NO_NETWORK         (-4)
/** Network operation status - Operation aborted */
#define ZWP_NET_OPERATION_STATUS_ABORTED            (-5)

/** Forward declaration for network configuration class */
struct _zwp_net_config;
/** Network configuration type */
typedef struct _zwp_net_config *zwp_net_config_t;

/** Network operation state */
typedef struct
{
	uint8_t       current_operation;                       /**< Current network operation - ZWNET_OP_XXX */
	int           current_operation_status;                /**< Current network operation status - OP_DONE, OP_FAILED, ZW_ERR_FAILED, ZWP_NET_OPERATION_STATUS_XXX */
	int           current_operation_total_nodes;           /**< Total number of nodes involved in the operation */
	int           current_operation_completed_nodes;       /**< Number of nodes for which the current operation is completed */
	uint8_t       previous_operation;                      /**< Previous network operation - ZWNET_OP_XXX */
} zwp_net_operation_state_t;
#ifdef LOCAL_CLIENT_ENABLE
typedef struct _zwp_zip_server_config
{
	uint8_t                    zip_server_address[16]; 
	int                        ipv4;
	uint16_t                   unsolicited_port_number;
	uint8_t					   dtls_pskey[32];
	uint8_t					   key_len;
}zwp_net_zip_server_config_t;
#endif

/** Z-Wave whitelist device info */
typedef struct
{
	char	*dsk;					/**< Device Specific Key (DSK) of the joining node for user to verify.  Note that
									the first 5 digits of DSK are missing if pin_required=1.
									Example of complete DSK:
									34028-23669-20938-46346-33746-07431-56821-14553
									Example of DSK with first 5 digits missing:
									-23669-20938-46346-33746-07431-56821-14553 */
	int		grant_keys;				/**< Granted keys of the device */
	int		metatype_count;			/**< Number of meta data type such as name/location. */
	char    name[WL_MAX_NAME + 1];  /**< For type=WL_INFO_TYPE_NAME; device name in UTF-8 encoding. It must not contains period character '.',
									underscore character '_' and must not end with the dash character '-' and must be terminated by
									NUL character*/
	char    loc[WL_MAX_LOC + 1];    /**< For type=WL_INFO_TYPE_LOC; device location in UTF-8 encoding with restrictions same as name field. */
} zwp_net_wl_dev_info_t;

/**
 * @name Getters
 * @{
 */
/**
 * @brief Get portal object from network object
 * @param[in]     net               Network object
 * @return        Portal object
 */
zwp_portal_t            zwp_net_get_portal(zwp_net_t net);
/**
 * @brief Get hash key from network object
 * @param[in]     net               Network object
 * @return        Hash key
 */
const void              *zwp_net_get_hash_key(zwp_net_t net);
/**
 * @brief Get hash key size from network object
 * @param[in]     net               Network object
 * @return        Hash key size
 */
ssize_t                 zwp_net_get_hash_key_size(zwp_net_t net);
/**
 * @brief Get portal client profile  from network object
 * @param[in]     net               Network object
 * @return        Portal client profile
 */
const clnt_prof_t       *zwp_net_get_portal_client_profile(zwp_net_t net);
/**
 * @brief Get  from network object
 * @param[in]     net               Network object
 * @return        akafsd
 */
zwp_pool_t              zwp_net_get_network_pool(zwp_net_t net);
/**
 * @brief Get HCAPI network initialization parameter from network object
 * @param[in]     net               Network object
 * @return        HCAPI network initialization parameter
 */
zwnet_init_p            zwp_net_get_zwnet_init(zwp_net_t net);
/**
 * @brief Get HCAPI network handle from network object
 * @param[in]     net               Network object
 * @return        HCAPI network handle
 */
zwnet_p                 zwp_net_get_zwnet(zwp_net_t net);
/**
 * @brief Get network descriptor from network object
 * @param[in]     net               Network object
 * @return        Network descriptor
 */
zwp_desc_net_t          zwp_net_get_desc_net(zwp_net_t net);
/**
 * @brief Get last updated time of network composition
 * @param[in]     net               Network object
 * @return        Last updated time of network composition
 */
zwu_time_t              zwp_net_get_network_utime(zwp_net_t net);
/**
* @brief Get homeid from network object
* @param[in]     net               Network object
* @param[out]    pHomeID           Home id
* @return        1 - value home id. 0 - no home id
*/
int						zwp_net_get_homeid(zwp_net_t net, uint32_t *pHomeID);
/**
 * @brief Get network operation state from network object
 * @param[in]     net               Network object
 * @param[out]    operation_state   Network operation state
 * @return        Nothing
 */
void                    zwp_net_get_operation_state(zwp_net_t net, zwp_net_operation_state_t *operation_state);
/**
* @brief Get node add requested key from network object
* @param[in]     net               Network object
* @param[out]    preq_key		   Requested key from joining node
* @return        1 for success and 0 for failure.
*/
int						zwp_net_get_req_key(zwp_net_t net, sec2_keys_req_cb_prm_t *preq_key);
/**
* @brief Get node add dsk parameters from network object
* @param[in]     net               Network object
* @param[out]    pdsk_prm		   DSK parameters from joining node
* @return        1 for success and 0 for failure.
*/
int						zwp_net_get_dsk(zwp_net_t net, sec2_dsk_cb_prm_t *pdsk_prm);
/**
* @brief Get prekit joining node dsk information from network object
* @param[in]     net               Network object
* @param[out]    pprekit_dsk	   prekit node dsk information from joining node
* @return        1 for success and 0 for failure.
*/
int						zwp_net_get_prekit_node_dsk(zwp_net_t net, zwnet_sts_t *pprekit_dsk);

/**
* @brief Get local node dsk for network initiate from network object
* @param[in]     net               Network object
* @param[out]    ppdsk			   Pointer to DSK string
* @return        1 for success and 0 for failure.
*/
int						zwp_net_get_local_node_dsk(zwp_net_t net, char **ppdsk);
/**
 * @brief Get HCAPI AAL context from network object
 * @param[in]     net               Network object
 * @return        HCAPI AAL context
 */
hcaal_ctx_p             zwp_net_get_hcaal(zwp_net_t net);
/**
 * @brief Get persistence context from network object
 * @param[in]     net               Network object
 * @return        Persistence context
 */
int                     zwp_net_get_ctx_persistent(zwp_net_t net, char *ctx_persistent, int len);
/**
 * @brief Get last replaced node from network object
 * @param[in]     net               Network object
 * @return        Last replaced node
 */
uint8_t                 zwp_net_get_last_rplc_node(zwp_net_t net);
/**
 * @brief Get timezone from network object
 * @param[in]     net               Network object
 * @return        Timezone
 */
char                    *zwp_net_get_timezone(zwp_net_t net);
/**
 * @brief Get report wait timeout (in seconds) from network object
 * @param[in]     net               Network object
 * @return        Report wait timeout (in seconds)
 */
int                     zwp_net_get_report_wait_timeout(zwp_net_t net);

#ifndef LOCAL_CLIENT_ENABLE
/**
* @brief Get IFTTT database pointer from network object
* @param[in]     net               Network object
* @return        IFTTT database pointer
*/
pzwp_db_db              zwp_net_get_ifttt_db(zwp_net_t net);

/**
* @brief Get IFTTT "generic trigger" database table pointer from network object
* @param[in]     net               Network object
* @return        IFTTT "generic trigger" database table pointer
*/
pzwp_db_table           zwp_net_get_ifttt_table_trigger_generic(zwp_net_t net);
#endif
/**
 * @}
 */

/**
 * @name Setters
 * @{
 */
/**
 * @brief Set network operation to network object
 * @param[in]     net               Network object
 * @param[in]     operation         Network operation
 * @return        Nothing
 */
void                    zwp_net_set_operation(zwp_net_t net, uint8_t operation);
/**
 * @brief Set node descriptor of last updated node to network object
 * @param[in]     net               Network object
 * @param[in]     node_desc         Node descriptor of last udpated node
 * @return        Nothing
 */
void                    zwp_net_set_node_updated_desc(zwp_net_t net, zwnoded_t node_desc);
#ifdef LOCAL_CLIENT_ENABLE
void                    zwp_net_set_zip_serverconfig(zwp_net_t net, zwp_net_zip_server_config_t *server_cofig);
void 					zwp_net_get_zip_server_addr(zwp_net_t net, zwp_net_zip_server_config_t *zwp_net_zip_server_config_t);
#endif
#ifndef LOCAL_CLIENT_ENABLE
/**
* @brief Set IFTTT "generic trigger" database table pointer to network object
* @param[in]     net						Network object
* @param[in]     ptable_trigger_generic		IFTTT "generic trigger" database table pointer
*/
void					zwp_net_set_ifttt_table_trigger_generic(zwp_net_t net, pzwp_db_table ptable_trigger_generic);
#endif
/**
 * @}
 */

/**
 * @brief Abort network operation
 * @param[in]     net               Network object
 * @param[in]     status            ZWP_STATUS_OK if HCAPI network abort call was successful. ZWP_STATUS_ERROR, if otherwise
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_net_abort_operation(zwp_net_t net, zwp_status_t status);

/**
 * @brief Increment the reference count of network object
 * @param[in]     net               Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_net_reference_count_increment(zwp_net_t net);
/**
 * @brief Decrement the reference count of network object
 * @param[in]     net               Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_net_reference_count_decrement(zwp_net_t net);

/**
 * @brief Lock the network descriptor tree and check if it is valid (initialized)
 * @param[in]     net               Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_net_desc_container_lock_and_test(zwp_net_t net);
/**
 * @brief Check if the network descriptor tree is valid (initialized)
 * @param[in]     net               Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_net_desc_container_test(zwp_net_t net);
/**
 * @brief Lock the network descriptor tree
 * @param[in]     net               Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_net_desc_container_lock(zwp_net_t net);
/**
 * @brief Unlock the network descriptor tree
 * @param[in]     net               Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_net_desc_container_unlock(zwp_net_t net);


/**
 * @brief Prepare the network object to wait for receiving network transmit status callback from HCAPI
 * @param[in]     net               Network object
 * @return        Nothing
 */
void zwp_net_transmit_wait_init(zwp_net_t net);
/**
 * @brief Wait for receiving network transmit status callback from HCAPI
 * @param[in]     net               Network object
 * @param[in]     transmit_status   Network transmit status
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_net_transmit_wait(zwp_net_t net, uint8_t *transmit_status);

/**
 * @brief Wait for receiving Report command callback from HCAPI
 * @param[in]     net               Network object
 * @param[in]     report_receive_condition   Report command receive condition
 * @param[in]     timeout           Maximum waiting time (in microseconds)
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_net_report_receive_wait(zwp_net_t net, zwp_thread_cond_t report_receive_condition, zwp_interval_time_t timeout);

/**
 * @brief Acquire interface descriptor by searching through network descriptor tree
 * @param[in]     ifd               Interface descriptor object that provides node ID, endpoint ID and interface ID for the search
 * @param[out]    desc_interface_p  The aquired interface descriptor
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * This leaves the descriptor container lock locked. The caller needs to be pair this call with zwp_net_release_desc_interface() to unlock
 */
zwp_status_t zwp_net_acquire_desc_interface(zwifd_p ifd, zwifd_p *desc_interface_p);
/**
 * @brief Release the previously acquired interface descriptor
 * @param[in]     ifd               Previously acquired interface descriptor
 * @return        Nothing
 * @see zwp_net_acquire_desc_interface
 */
void zwp_net_release_desc_interface(zwifd_p ifd);

/**
 * @brief Acquire node descriptor by searching through network descriptor tree
 * @param[in]     noded             Interface descriptor object that provides node ID for the search
 * @param[out]    desc_node_p       The aquired node descriptor
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * This leaves the descriptor container lock locked. The caller needs to be pair this call with zwp_net_release_desc_node() to unlock
 */
zwp_status_t zwp_net_acquire_desc_node(zwnoded_p noded, zwnoded_p *desc_node_p);
/**
 * @brief Release the previously acquired node descriptor
 * @param[in]     noded             Previously acquired node descriptor
 * @return        Nothing
 * @see zwp_net_acquire_desc_node
 */
void zwp_net_release_desc_node(zwnoded_p noded);

/**
 * @brief Execute AVI network message
 * @param[in]     net               Network object
 * @param[in]     service_net       Network service object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_net_execute_avi_message_networks(zwp_net_t net, zwp_service_networks_t service_net);

/**
 * @brief Initialize a network
 * @param[in]     net               Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_net_initialize(zwp_net_t net);
/**
 * @brief Uninitialize a network
 * @param[in]     net               Network object
 * @return        Nothing
 */
void zwp_net_exit(zwp_net_t net);

/**
 * @brief Created a new thread to initialize a network
 * @param[in]     net               Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_net_start(zwp_net_t net);

/**
 * @brief A task hanlder to asynchrnously uninitialize a network
 * @param[in]     thread            Thread object
 * @param[in]     arg               Network object of type @a zwp_net_t
 * @return        NULL
 */
void *ZWP_THREAD_FUNC zwp_net_stop_async_handler(zwp_thread_t thread, void *arg);
/**
 * @brief Wait for the start handler thread to 'join' and uninitialize a network
 * @param[in]     net               Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_net_stop(zwp_net_t net);

/**
 * @brief Allocate a new network object
 * @param[out]    net_p             The new network object
 * @param[in]     file_descriptor   File descriptor to connect to Z/IP gateway/router using TLS
 * @param[in]     SSL_object        SSL object pointer to connect to Z/IP gateway/router using TLS
 * @param[in]     profile           Portal client configuration profile for the Z/IP gateway
 * @param[in]     portal            Portal object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_net_free() to free resources
 */
zwp_status_t zwp_net_alloc(zwp_net_t *net_p, int file_descriptor,
		void *SSL_object, clnt_prof_t *profile, zwp_portal_t portal);

/**
 * @brief Free a network object
 * @param[in]     net               Network object
 * @return        Nothing
 * @see zwp_net_alloc
 */
void zwp_net_free(zwp_net_t net);

/**
 * @brief Get network class configuration
 * @param[out]    net_config_p      Network configuration object
 * @param[in]     config_parser     Configuration parser handle
 * @param[in]     config_dir        Configuration directory path to convert path configurations from relative to absolute
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @post @a net_config_p refers to a singleton \link _zwp_net_config object \endlink
 * allocated/returned by this function (constructor)
 *
 * @note
 * @par
 * The caller should call zwp_net_config_free() to free resources
 */
zwp_status_t zwp_net_config_get_instance(zwp_net_config_t *net_config_p,
		const config_t *config_parser, const char *config_dir);

/**
 * @brief Free network class configuration
 * @param[in]     net_config        Network configuration object
 * @return        Nothing
 * @see zwp_net_config_get_instance
 */
void zwp_net_config_free(zwp_net_config_t net_config);

/**
 * @brief zwp_net_init_scn - Initialize scene module
 * @param[in]	  net				Network object
 * @param[in]	  net_timezone	    Network timezone (optional).  If it is NULL, the timezone will be retrieved from directory service.
 * @return	      ZWP_STATUS_XXX
 */
zwp_status_t zwp_net_init_scn(zwp_net_t net, char *net_timezone);

/**
* @brief Add node operation security 2 callback function
* @param[in]     usr_param        User parameter of the callback function
* @param[in]     cb_param		  Security 2 callback parameters
* @return        Nothing
*/
void ZWP_HCAPI_NOTIFICATION add_node_sec2(void *usr_param, sec2_add_cb_prm_t *cb_param);

/**
* @brief Unsolicited add node operation security 2 callback function
* @param[in]     usr_param        User parameter of the callback function
* @param[in]     cb_param		  Security 2 callback parameters
* @return        Nothing
*/
void ZWP_HCAPI_NOTIFICATION zwp_net_add_node_sec2_unsolicited_notification(void *usr_param, sec2_add_cb_prm_t *cb_param);

/**
* @brief Get security 2 local node DSK callback function
* @param[in]     usr_ctx		  User context of the callback function
* @param[in]     dsk			  The DSK string
* @return        Nothing
*/
void ZWP_HCAPI_NOTIFICATION get_local_node_dsk(void *usr_ctx, char *dsk);

/**
* @brief Whitelist DSK list callback function
* @param[in]	usr_ctx     user context
* @param[in]	dsk_list    DSK list
*/
void ZWP_HCAPI_NOTIFICATION zwp_net_whitelist_list_notification(void *usr_ctx, wl_dsk_lst_t *dsk_list);

/**
* @brief Remove one Whitelist device info record
* @param[in]	pwl_dev_info  Pointer to a whitelist device info record
*/
void zwp_net_whitelist_remove(zwp_net_wl_dev_info_t *pwl_dev_info);

/**
* @brief Remove all the Whitelist device info records
* @param[in]	net		  Network object
* @param[in]	whitelist_list  whitelist list
*/
void zwp_net_whitelist_remove_all(zwp_net_t net, zwu_array_list_t whitelist_list);

/**
* @brief Create a whitelist device info record
* @param[in]	  dsk				dsk info for the record
* @param[out]	  ppwl_dev_info	    Pointer to a whitelist device info record
* @return	      ZWP_STATUS_XXX
*/
zwp_status_t zwp_net_create_whitelist_dev(char* dsk, zwp_net_wl_dev_info_t **ppwl_dev_info);

/**
* @brief Find a whitelist device info record with matching dsk
* @param[in]	net		  Network object
* @param[in]	dsk				dsk info for the record
* @param[out]	ppwl_dev_info	    Pointer to a matching whitelist device info record if found
* @return	    ZWP_STATUS_XXX
*/
zwp_status_t zwp_net_find_a_whitelist_dev(zwp_net_t net, char* dsk, zwp_net_wl_dev_info_t **ppwl_dev_info);

/**
* @brief Whitelist device info callback function
* @param[in]	usr_ctx     user context
* @param[in]	dsk         DSK string
* @param[in]	info		Buffer to store additional information of the device (optional).
* @param[in]	info_cnt    Number of additional information stored in "info".
*/
void ZWP_HCAPI_NOTIFICATION zwp_net_whitelist_dev_info_notification(void *usr_ctx, char *dsk, uint8_t grant_keys, wl_info_t *info, uint8_t info_cnt);

void zwp_net_whitelist_dsk_array_free(zwu_array_list_t whitelist_dsk_array);

int zwp_net_whitelist_list_report(zwp_net_t net, zwu_array_list_t *pwhitelist_dsk_array);

/** }@ */
#endif /* ZWP_NETWORK_H_ */
