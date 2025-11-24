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
 * @file    zwp_net_discovery.h
 *
 * @brief   Z-Wave network discovery.
 * @details Handles Z-Wave network discovery.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-07-23
 * - Initial version
 */

#ifndef ZWP_NET_DISCOVERY_H_
#define ZWP_NET_DISCOVERY_H_

/**
 * \addtogroup  zwportaldzwave
 * @{
 */

#include "zwp_status.h"
#include "zwp_hcapi.h"

#define ZWP_NET_DISCOVERY_IP_VERSION_6       0	///< Network discovery type IPv6
#define ZWP_NET_DISCOVERY_IP_VERSION_4       1	///< Network discovery type IPv4

//MDNS flags work in conjunction with IP flags
#define ZWP_NET_DISCOVERY_WITH_NO_MDNS		 0	///< Network discovery without MDNS
#define ZWP_NET_DISCOVERY_WITH_MDNS			 1	///< Network discovery with MDNS

//Clear flag works in conjunction with IP/MDNS flags
#define	ZWP_NET_DISCOVERY_CLEAR_LIST		1	///< Clear the discovery lists before starting the discovery
#define	ZWP_NET_DISCOVERY_NO_CLEAR_LIST		0	///< Don't clear the discovery lists before starting the discovery

/** Network Discovery Types */
#define ZWP_NET_DISCOVERY_TYPE_NONE					0	///< No discovery in progress
#define ZWP_NET_DISCOVERY_TYPE_IPV4					1	///< IPv4 discovery in progress
#define ZWP_NET_DISCOVERY_TYPE_IPV4_MDNS			2	///< IPv4 mdns discovery in progress
#define ZWP_NET_DISCOVERY_TYPE_IPV6					3	///< IPv6 discovery in progress
#define ZWP_NET_DISCOVERY_TYPE_IPV6_MDNS			4	///< IPv6 mdns discovery in progress

#define ZWP_ZIPGW_DEFAULT_PORT				41231		///< The new Z/IP Gateway (from 2.5x onwards) does not allow 4123

#define ZWP_ZIP_SERVER_ADDRESS_SIZE			IPV6_ADDR_LEN		///< Maximum Z/IP Gateway address size
//#define ZWP_ZIP_SERVER_DEFAULT_PORT         ZWAVE_HOME_CTL_PORT	///< Maximum Z/IP Gateway listening port number
#define ZWP_ZIP_SERVER_DEFAULT_PORT         ZWP_ZIPGW_DEFAULT_PORT	///< Maximum Z/IP Gateway listening port number


/** Class: Z/IP Network Discovery */
struct _zwp_net_discovery;
/** Typedef for Class: Z/IP Network Discovery */
typedef struct _zwp_net_discovery *zwp_net_discovery_t;

/** Discovery completion notification call back */
typedef void (*zwp_net_discovery_completion_notification_t)(void *completion_notification_argument);

/** Network Discovery Report */
typedef struct _zwp_net_discovery_report
{
	uint8_t	*pgwlist;			/**< Pointer to discovered gateways list */
	size_t  gwlistlen;			/**< Length of discovered gateways list */
	int32_t received_reports;	/**< Number of discovery reports that are received so far */
	int32_t total_reports;		/**< Total number of discovery reports that will be delivered */
	uint8_t type;				/**< Current discovery type (ZWP_NET_DISCOVERY_TYPE_XXXX) */
	char    **gwname;			/**< Gateways mdns name list corresponding to gateway list */

}zwp_net_discovery_report_t;

/**
 * @brief Get the list of discovered gateways.
 * @param[in]		discovery					Discovery application context
 * @param[in]		ip_version					Flag to indicate to get ipv4 or ipv6 discovery list 1=get ipv4 0 = get ipv6
 * @param[out]		zwp_net_discovery_report	Pointer to discovery list report
 * @retval        	ZWP_STATUS_OK     			Success
 * @retval        	ZWP_STATUS_ERROR  			Failure
 *
 * @note
 * @par
 * The caller should free discovery list and mdns list
 * If the list is empty, pGwListLen will be set to zero and returns success
 */

zwp_status_t zwp_net_discovery_get_list(zwp_net_discovery_t discovery, int ip_version,
		zwp_net_discovery_report_t *zwp_net_discovery_report);

/**
 * @brief Set the current discovery type to none
 * @param[in]		discovery			Discovery application context
 * @retval        	ZWP_STATUS_OK     	Success
 * @retval        	ZWP_STATUS_ERROR  	Failure
 */
zwp_status_t zwp_net_discovery_clear_type(zwp_net_discovery_t discovery);

/**
 * @brief Get the current discovery type.
 * @param[in]		discovery			Discovery application context
 * @param[out]		pType				Discovery type
 * @retval        	ZWP_STATUS_OK     	Success
 * @retval        	ZWP_STATUS_ERROR  	Failure
 */
zwp_status_t zwp_net_discovery_get_type(zwp_net_discovery_t discovery, uint8_t *pType);

/**
 * @brief Get the first network/gateway address from discovery list
 * @param[in]		discovery			Discovery application context
 * @param[in]		ip_version			Flag to indicate get first ipv4 or ipv6 address from list 1=use ipv4 0 = use ipv6(ZWP_NET_DISCOVERY_IP_VERSION_XXX)
 * @param[out]		zip_server_address	Pointer to ZIP Gateway address
 * @retval        	ZWP_STATUS_OK     	Success
 * @retval        	ZWP_STATUS_ERROR  	Failure
 */
zwp_status_t zwp_net_discovery_get_first_network(zwp_net_discovery_t discovery, int ip_version,
		uint8_t zip_server_address[ZWP_ZIP_SERVER_ADDRESS_SIZE]);


/**
 * @brief Initialize discovery module
 * @param[in]		discovery							Discovery application context
 * @param[in]		ip_version							Flag to indicate to start ipv4 or ipv6 discovery 1=use ipv4 0 = use ipv6(ZWP_NET_DISCOVERY_IP_VERSION_XXX)
 * @param[in]		mdns								Flag to indicate to start mdns ipv4 or ipv6 discovery(depends on ip_version flag). Use mdns if flag is non-zero.
 * @param[in]		clear_list							Flag to indicate to clear the existing discovery list. Clear the list if flag is non-zero.
 * @param[in]		completion_notification_handler		Notification call back for discovery completion
 * @param[in]		completion_notification_argument	Argument to Notification call back call back function
 * @retval        	ZWP_STATUS_OK     	Success
 * @retval        	ZWP_STATUS_EBUSY  	Discovery cycle is in progress
 * @retval        	ZWP_STATUS_ERROR  	Failure
 *
 * @note
 * @par
 * Discovery states must be cleared by calling zwp_net_discovery_clear_type after successfully completing discovery.
 */
zwp_status_t zwp_net_discovery_start(zwp_net_discovery_t discovery, int ip_version, int mdns, int clear_list,
		zwp_net_discovery_completion_notification_t completion_notification_handler,
		void *completion_notification_argument);

/**
 * @brief Stop the network discovery and clears the application discovery context
 * @param[in]   discovery		Discovery application context
 * @param[in]   clear_list		Flag to clear the existing discovery list. clear the list if flag is non-zero
 * @retval		nothing
 */
void zwp_net_discovery_stop(zwp_net_discovery_t discovery, int clear_list);

/**
 * @brief Initialize discovery module
 * @param[out]    discovery_p		Discovery application context
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_net_discovery_init(zwp_net_discovery_t *discovery_p);

/**
 * @brief Uninitialize discovery module
 * @param[in]     discovery           Discovery application context
 * @return        Nothing
 */
void zwp_net_discovery_exit(zwp_net_discovery_t discovery);
/** }@ */
#endif /* ZWP_NET_DISCOVERY_H_ */
