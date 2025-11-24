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
 * @file    zwp_portal.h
 *
 * @brief   Portal module
 * @details Module for setting up portal service
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-04-30
 * - Initial version
 */

#ifndef ZWP_PORTAL_H_
#define ZWP_PORTAL_H_
/**
 * \addtogroup  zwportaldportalclient
 * @{
 */

#include <libconfig.h>

#include "zwp_portal_define.h"
#include "zwp_status.h"
#include "zwp_directory_service.h"
#include "zwp_ifttt.h"
#include "zwp_hash.h"
#include "zwp_avi.h"
#include "zwp_avi_service.h"
#include "zwp_thread.h"

/** Forward declaration for portal configuration class */
struct _zwp_portal_config;
/** Portal configuration type */
typedef struct _zwp_portal_config *zwp_portal_config_t;

#ifdef LOCAL_CLIENT_ENABLE
//This has been defined to make local client API compatible
typedef struct
{
	int                          file_descriptor;          /**< File descriptor to connect to Z/IP gateway/router using TLS */
	void                         *SSL_object;              /**< SSL object pointer to connect to Z/IP gateway/router using TLS */
	clnt_prof_t                  *profile;                 /**< Portal configuration profile for the Z/IP gateway */
} zwp_portal_new_client_t;

/**
 * @brief Z/IP Gateway IP and Key configuration
 */
typedef struct
{
	uint8_t                    zip_server_address[16];	 	/**< Z/IP Gateway IP address */
	int                        ipv4;						/**< Z/IP Gateway IP address type */
	uint16_t                   unsolicited_port_number;		/**< Unsolicited or host port number */
	uint8_t					   dtls_pskey[32];				/**< Pre-shared key for DTLS communication */
	uint8_t					   key_len;						/**< Pre-shared key length */
}zwp_zwave_client_ip_settings_t;
#endif

//typedef void (*zwp_net_cleanup_t)(void *user);

/**
 * @name Getters
 * @{
 */
/**
 * @brief Get memory pool from portal object
 * @param[in]     portal            Portal object
 * @return        Memory pool
 */
zwp_pool_t                zwp_portal_get_portal_pool(zwp_portal_t portal);
/**
 * @brief Get directory service from portal object
 * @param[in]     portal            Portal object
 * @return        Directory service
 */
zwp_directory_service_t   zwp_portal_get_directory_service(zwp_portal_t portal);

zwp_ifttt_t   zwp_portal_get_ifttt(zwp_portal_t portal);

/**
 * @brief Get AVI object from portal object
 * @param[in]     portal            Portal object
 * @return        AVI object
 */
zwp_avi_t                 zwp_portal_get_avi(zwp_portal_t portal);
/**
 * @brief Get HCAPI portal handle from portal object
 * @param[in]     portal            Portal object
 * @return        HCAPI portal handle
 */
void *                    zwp_portal_get_portal_handle(zwp_portal_t portal);
/**
 * @brief Get local state directory from portal object
 * @param[in]     portal            Portal object
 * @return        Local state directory
 */
char *                    zwp_portal_get_local_state_dir(zwp_portal_t portal);
/**
 * @brief Get device specific configuration file path from portal object
 * @param[in]     portal            Portal object
 * @return        Device specific configuration file path
 */
char *                    zwp_portal_get_device_specific_configuration_file(zwp_portal_t portal);
/**
 * @brief Get timeout (in seconds) for waiting for Z-Wave Report command
 * @param[in]     portal            Portal object
 * @return        Timeout (in seconds) for waiting for Z-Wave Report command
 */
int                       zwp_portal_get_report_wait_timeout(zwp_portal_t portal);

#ifdef LOCAL_CLIENT_ENABLE
/**
 * @brief Z/IP client handle from portal object
 * @param[in]     portal            Portal object
 * @return        Z/IP client handle
 */
void *					zwp_portal_get_zip_client_handle(zwp_portal_t portal);

/**
 * @brief Get Z/IP client initialization lock from portal object
 * @param[in]     portal            Portal object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t 			zwp_portal_get_client_intialize_lock(zwp_portal_t portal);

/**
 * @brief Get Z/IP client initialization lock from portal object
 * @param[in]     portal            Portal object
 * @retval        Flag that indicates whether automatically set unsolicited IP enabled or not
 */
uint8_t zwp_portal_get_gw_auto_set_status(zwp_portal_t portal);

/**
 * @brief Get Z/IP GW IP and key settings from portal object
 * @param[in]     portal            				Portal object
 * @param[out]    zip_client_ip_settings            client ip and key settings
 * @retval        ZWP_STATUS_OK     				Success
 * @retval        ZWP_STATUS_ERROR  				Failure
 */
zwp_status_t zwp_portal_get_zip_client_ip_settings(zwp_portal_t portal, zwp_zwave_client_ip_settings_t *zip_client_ip_settings);

#endif
/**
 * @}
 */

#ifdef LOCAL_CLIENT_ENABLE
/**
 * @name Setters
 * @{
 */
/**
 * @brief Put Z/IP client initialization lock from portal object
 * @param[in]     portal            Portal object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_portal_put_client_intialize_lock(zwp_portal_t portal);

/**
 * @brief Set Z/IP GW IP and key settings in the  portal object
 * @param[in]     portal            				Portal object
 * @param[in]     zip_client_ip_settings            Client ip and key settings
 * @retval        ZWP_STATUS_OK     				Success
 * @retval        ZWP_STATUS_ERROR  				Failure
 */
zwp_status_t zwp_portal_set_zip_client_ip_settings(zwp_portal_t portal, zwp_zwave_client_ip_settings_t *zip_client_ip_settings);

/**
 * @}
 */
#endif

/**
 * @brief Execute an AVI message
 * @param[in]     portal            Portal object
 * @param[in]     service           Service object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_portal_execute_avi_message(zwp_portal_t portal, zwp_service_t service);

/**
 * @brief Kill asynchronously the connection with a network
 * @param[in]     portal            Portal object
 * @param[in]     net               Network object
 * @param[in]     check_net_exists  If 0, net exists check is skipped. Else, the check is done before removing.
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_portal_client_kill_net_async(zwp_portal_t portal, zwp_net_t net, int check_net_exists);
/**
 * @brief Kill the connection with a network
 * @param[in]     portal            Portal object
 * @param[in]     net               Network object
 * @param[in]     check_net_exists  If 0, net exists check is skipped. Else, the check is done before removing.
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_portal_client_kill_net(zwp_portal_t portal, zwp_net_t net, int check_net_exists);
/**
 * @brief Kill the connection with a network identified by its remote access code
 * @param[in]     portal            Portal object
 * @param[in]     ra_code           Remote access code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_portal_client_kill_net_by_ra_code(zwp_portal_t portal, char *ra_code);

#ifdef LOCAL_CLIENT_ENABLE
/**
 * @brief Allocate a new network object and start initializing it
 * @param[in]     portal            Portal object
 * @param[in]     new_client_parameter  Parameters for new portal client
 * @param[out]    net_p             Network object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 * @see zwp_portal_client_kill_net_async, zwp_portal_client_kill_net and zwp_portal_client_kill_net_by_ra_code
 */
zwp_status_t zwp_portal_client_spawn_net(zwp_portal_t portal, zwp_portal_new_client_t *new_client_parameter, zwp_net_t *net_p);

#endif

/**
 * @brief Start portal
 * @param[out]    portal_p          Portal object
 * @param[in]     portal_config     Portal configuration
 * @param[in]     avi               AVI object
 * @param[in]     directory_service Directory service
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @post @a portal_p refers to a singleton object @a zwp_portal_t
 * allocated/returned by this function (constructor)
 *
 * @note
 * @par
 * The caller should call zwp_portal_stop() to stop the logger and free resources
 */
zwp_status_t zwp_portal_start(zwp_portal_t *portal_p, zwp_portal_config_t portal_config,
		zwp_avi_t avi, zwp_directory_service_t directory_service, zwp_ifttt_t ifttt);

/**
 * @brief Stop and free portal
 * @param[in]     portal            Application configuration object
 * @return        Nothing
 * @see zwp_portal_start
 */
void zwp_portal_stop(zwp_portal_t portal);

/**
 * @brief Get portal configuration
 * @param[out]    portal_config_p   Portal configuration object
 * @param[in]     config_parser     Configuration parser handle
 * @param[in]     config_dir        Configuration directory path to convert path configurations from relative to absolute
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @post @a portal_config_p refers to a singleton \link _zwp_portal_config object \endlink
 * allocated/returned by this function (constructor)
 *
 * @note
 * @par
 * The caller should call zwp_portal_config_free() to free resources
 *
 * @par
 * This function is not thread safe
 */
zwp_status_t zwp_portal_config_get_instance(zwp_portal_config_t *portal_config_p, const config_t *config_parser, const char *config_dir);

/**
 * @brief Free portal configuration
 * @param[in]     portal_config     Application configuration object
 * @return        Nothing
 * @see zwp_portal_config_get_instance
 */
void zwp_portal_config_free(zwp_portal_config_t portal_config);

/**
 * @brief Get network handle based on the given network id
 * @param[in]	portal	        Portal context
 * @param[in]	net_id	        Network id (e.g. 00-1E-32-FF-FF-12-49-A3)
 * @param[out]	net_p	        Network handle
 * @return ZWP_STATUS_XXX
 * @post   Caller must free the returned network handle using zwp_portal_free_net() if return status is ZWP_STATUS_OK
 */
zwp_status_t zwp_portal_get_net(zwp_portal_t portal, char *net_id, zwp_net_t *net_p);

/**
 * @brief Free the network handle returned by call to zwp_portal_get_net()
 * @param[in]	net	      Network handle
 * @return
 */
void zwp_portal_free_net(zwp_net_t net);

/**
* @brief Add a thread handle to portal threah handle array list
* @param[in]	portal	        Portal context
* @param[in]	thread	        thread handle
* @return ZWP_STATUS_XXX 
*/
zwp_status_t zwp_portal_thread_handle_list_add(zwp_portal_t portal, zwp_thread_t thread);

/**
zwp_portal_thread_handle_list_remove - Find the thread handle in portal thread handle array list and remove it
* @param[in]	portal	        Portal context
* @param[in]	thread	        thread handle
* @return ZWP_STATUS_OK			Found the thread handle and successfully remove it \n
*	      ZWP_STATUS_ENOENT		The thread handle is not found in the array list \n
*		  ZWP_STATUS_ERROR		Internal error
*/
zwp_status_t zwp_portal_thread_handle_list_remove(zwp_portal_t portal, zwp_thread_t thread);

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
zwp_status_t zwp_portal_thread_handle_list_pop(zwp_portal_t portal, zwp_thread_t *pthread);
/** }@ */
#endif /* ZWP_PORTAL_H_ */
