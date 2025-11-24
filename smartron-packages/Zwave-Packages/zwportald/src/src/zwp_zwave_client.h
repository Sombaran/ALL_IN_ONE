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
 * @file    zwp_zwave_client.h
 *
 * @brief   Z-Wave client.
 * @details Handles Z-Wave client.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-07-23
 * - Initial version
 */

#ifndef ZWP_ZWAVE_CLIENT_H_
#define ZWP_ZWAVE_CLIENT_H_

/**
 * \addtogroup  zwportaldzwave
 * @{
 */

#include "zwp_status.h"
#include "zwp_network_define.h"
#include "zwp_net_discovery.h"
#include "zwp_portal.h"


#define ZIP_GW_FLAG_BUSY	0x01		///< Gateway is currently initialized or busy
#define ZIP_GW_FLAG_DISCVR	0x02		///< Gateway is in the discovery list
#define ZIP_GW_FLAG_CACHE	0x04		///< Gateway is in the cached list
#define ZIP_GW_FLAG_V2		0x08		///< Gateway is version 2 gateway

#define MAX_CACHED_ZIP_GW_ADDR					5		///< Max Number of cached entries in the cached list
#define ZIP_GW_URI_LEN_MAX						111		///< Max gateway name length
#define ZIP_GW_UNSOL_RPT_PORT_SUFFIX_LEN 		6		///< Max unsolicited port number(including NULL character) length
#define DTLS_PSK_KEY							65		///< Max pre-shared DTLS key(ASCII characters including NULL character) length

/**
 * @brief Z/IP Gateway name, unsolicited port and key configuration
 */
typedef struct
{
	char name[IPV6_ADDR_LEN + 1];			/**<  Name of the Z/IP Gateway */
	uint8_t	address_type;					/**<  Address type(1:ipv4 0:ipv6) of the Z/IP Gateway */
	uint16_t unsol_rpt_port;				/**<  Host unsolicited port number */
	uint8_t	flag;							/**<  Flag to indicate Gateway entry type (ZIP_GW_FLAG_XXXX) */
	uint8_t dtls_pskey[32];					/**<  Pre-shared key for DTLS communication */
	uint8_t key_len;						/**<  Length of pre-shared key for DTLS communication */
} zip_gw_name_t;


/**
 * @brief Z-wave client discovery and network configuration
 */
typedef struct _zwp_zwave_client
{
	zwp_net_discovery_t        net_discovery;                  	/**< Network discovery object */

	volatile int               net_discovery_state;            	/**< Network discovery state - progress/completed */
	zwp_thread_mutex_t         net_discovery_state_lock;       	/**< Mutex lock for network discovery state */
	zwp_thread_cond_t          net_discovery_state_changed;    	/**< Conditional variable indicating network discovery state change */

	int                        zip_server_selected;            	/**< Flag to indicate if zip_server is selected */

	zwp_pool_t				   zwclient_pool;					/**< memory pool for discovery */
	zwp_net_t                  net;                            	/**< Current network */

	uint8_t                    zip_server_address[ZWP_ZIP_SERVER_ADDRESS_SIZE];  /**< ZIP server address */
	int                        ipv4;                            /**< Flag to indicate zip_server_address type. 1 = IPv4, otherwise IPv6  */
	uint16_t                   unsolicited_port_number;         /**< Host Port Number for sending and receiving */
	uint8_t 				   dtls_pskey[32];					/**< Pre-shared key for DTLS communication */
	uint8_t					   key_len;							/**< Length of pre-shared key */

	zip_gw_name_t			   gateway_cached_list[MAX_CACHED_ZIP_GW_ADDR]; /**< Previously connected gateways list */

}*zwp_zwave_client_t;


/* Getters section starts */
/**
 * @brief Get current network object from Z-wave client object
 * @param[in]     client            Z-wave client object
 * @retval        network object     Success
 * @note
 * @par
 * It may return NULL if network was not set before.
 */
zwp_net_t zwp_zwave_client_get_net(zwp_zwave_client_t client);
/* Getters section ends */

/**
 * @brief Initializes Z-wave client with required pools, discovery locks and gateway configuration.
 * @param[in]     portal            				Portal object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 */
zwp_status_t zwp_zwave_client_init(zwp_portal_t portal);

/**
 * @brief Un-initialize or free the resources created by initialization routine
 * @param[in]     client            				Z-wave client object
 * @retval        nothing
 *
 */
void zwp_zwave_client_exit(zwp_zwave_client_t client);

/**
 * @brief Update the Z-wave client cached gateway list and file with given IP and key info.
 * @param[in]     client            				Z-wave client object
 * @param[in]     app_var_networks_path        		Path to application network directory(where gateway file exists)
 * @param[in]     client_ip_settings        		IP and Key settings of the gateway
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 */
zwp_status_t zwp_zwave_client_set_zip_server_to_file(zwp_zwave_client_t client, char *app_var_networks_path, const zwp_zwave_client_ip_settings_t *client_ip_settings);

/**
 * @brief Update the Z-wave client cached gateway list from gateway list file
 * @param[in]     client            				Z-wave client object
 * @param[in]     app_var_networks_path        		Path to application network directory
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 */
zwp_status_t zwp_zwave_client_get_zip_server_from_file(zwp_zwave_client_t client, char *app_var_networks_path);

/**
 * @brief Get the copy of cached gateway list
 * @param[in]     client            Z-wave client object
 * @param[out]    cached_list       Pointer to hold cached list
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 */
zwp_status_t zwp_zwave_client_get_copy_cached_list(zwp_zwave_client_t client, zip_gw_name_t *cached_list);

/**
 * @brief Update the Z-wave client object with current/new network object
 * @param[in]     client            Z-wave client object
 * @param[in]     net        		network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 */
zwp_status_t zwp_zwave_client_set_net(zwp_zwave_client_t client, zwp_net_t net);

/**
 * @brief Start the Z/IP Gateway network discovery
 * @param[in]     client            Z-wave client object
 * @param[in]     ip_version        IP4/6 domain to start discovery(ZWP_NET_DISCOVERY_IP_VERSION_XXX)
 * @param[in]     mdns      		mdns flag(ZWP_NET_DISCOVERY_WITH_XXX) to indicate start mdns discovery in given IP domain
 * @param[in]     clean             Clean the previously discovered gateways from cached list.
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * Discovery state must be ZWP_NET_DISCOVERY_TYPE_NONE before starting discovery.
 * Clean flag clears both IP and MDNS entries from the cached list.
 * Call zwp_net_discovery_clear_type() after completing the discovery to set the discovery state to none.
 */
zwp_status_t zwp_zwave_client_net_discover(zwp_zwave_client_t client, int ip_version, int mdns, int clean);

/**
zwp_hexstring_to_bin - Convert ASCII hexstring to binary string
@param[in] psk_str   ASCII hexstring
@param[in] psk_len   ASCII hexstring length (must be even number)
@param[out] psk_bin  Binary string
@return  Zero on success, non-zero on failure
*/
int zwp_hexstring_to_bin(char *psk_str, int psk_len, uint8_t *psk_bin);

/**
zwp_discovery_start - Launch network discovery thread
@param[in] 		client   		  Z-wave client object
@retval        ZWP_STATUS_OK     Success
@retval        ZWP_STATUS_ERROR  Failure
*/
zwp_status_t zwp_discovery_start(zwp_zwave_client_t client);
/** }@ */
#endif /* ZWP_ZWAVE_CLIENT_H_ */
