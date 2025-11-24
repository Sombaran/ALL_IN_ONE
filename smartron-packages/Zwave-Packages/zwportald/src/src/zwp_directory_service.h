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
 * @file    zwp_directory_service.h
 *
 * @brief   Directory Service connection
 * @details Handles Directory Service connections.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-06-05
 * - Initial version
 */

#ifndef ZWP_DIRECTORY_SERVICE_H_
#define ZWP_DIRECTORY_SERVICE_H_

/**
 * \addtogroup  zwportaldportalclient
 * @{
 */


#include <libconfig.h>

#include "zwp_status.h"
#include "zwp_hcapi.h"

#ifndef TCP_PORTAL
//This has been defined to make local client API compatible
/** Portal client profile class */
typedef struct
{
    uint8_t         clnt_id[8];             ///< Client's ID (currently using MAC address formatted as an IEEE EUI-64 identifier)
    uint8_t         clnt_pin[8];            ///< Client's PIN (password)
    uint8_t         clnt_ipv6_addr[16];     ///< Client's IPv6 address. May be all-zeroes IPv6 address
    uint8_t         clnt_dflt_gw[16];       ///< Client's default IPv6 gateway
    uint8_t         clnt_pan_prefix[16];    ///< Client's PAN interface prefix with /64 prefix length. May be all-zeroes IPv6 address
    uint8_t         clnt_unsolicited_dst[16];///< Client's forwarding destination address for unsolicited message
    uint8_t         svr_ipv6_addr[16];      ///< Server's IPv6 address
    uint16_t        clnt_unsolicited_port;  ///< Client's forwarding destination port for unsolicited message. Should be 4123
    uint8_t         clnt_ipv6_prefix;       ///< Client's IPv6 address prefix length
    uint8_t         svr_ipv6_prefix;        ///< Server's IPv6 address prefix length
} clnt_prof_t;
#endif


/** Forward declaration for Directory Service configuration class */
struct _zwp_directory_service_config;
/** Directory Service configuration type */
typedef struct _zwp_directory_service_config *zwp_directory_service_config_t;

/** Forward declaration for Directory Service class */
struct _zwp_directory_service;
/** Directory Service type */
typedef struct _zwp_directory_service *zwp_directory_service_t;

/**
 * @brief Get list of portal client profiles
 * @param[in]     directory_service Directory service object
 * @param[out]    profile_list_p    Array of portal client profiles
 * @param[out]    profile_count_p   Number of portal client profiles in array
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_directory_service_zipgw_profile_list_free() to free resources
 */
zwp_status_t zwp_directory_service_zipgw_profile_list_get(zwp_directory_service_t directory_service, clnt_prof_t **profile_list_p, int *profile_count_p);

/**
 * @brief Free list of portal client profiles
 * @param[in]     profile_list      Array of portal client profiles
 * @return        Nothing
 * @see zwp_directory_service_zipgw_profile_list_get
 */
void zwp_directory_service_zipgw_profile_list_free(clnt_prof_t *profile_list);

/**
 * @brief Get the portal client profile for a given network ID
 * @param[in]     directory_service Directory service object
 * @param[in]     net_id            Network ID
 * @param[out]    profile_p         Portal client profile
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_directory_service_zipgw_profile_free() to free resources
 *
 * @par
 * When the network ID is not found (i.e, the LDAP entry count is zero), <em> *profile_p </em> is set to NULL and the call returns with <em> ZWP_STATUS_OK </em>
 */
zwp_status_t zwp_directory_service_zipgw_profile_get(zwp_directory_service_t directory_service, char *net_id, clnt_prof_t **profile_p);

/**
 * @brief Free list of portal client profiles
 * @param[in]     profile           Portal client profile
 * @return        Nothing
 * @see zwp_directory_service_zipgw_profile_get
 */
void zwp_directory_service_zipgw_profile_free(clnt_prof_t *profile);

/**
 * @brief Get timezone string based on the given network id
 * @param[in]   directory_service Directory service object
 * @param[in]   net_id	        Network id (e.g. 00-1E-32-FF-FF-12-49-A3)
 * @param[out]  timezone	    Time zone
 * @return ZWP_STATUS_XXX
 * @post   Caller has to free the returned timezone if return status is ZWP_STATUS_OK
 */
zwp_status_t zwp_directory_service_get_timezone(zwp_directory_service_t directory_service, char *net_id, char **timezone);

/**
 * @brief Get list of network IDs authorized for a given user
 * @param[in]     directory_service Directory service object
 * @param[in]     user              User name
 * @param[out]    network_list_p    Array of network IDs
 * @param[out]    network_count_p   Number of network IDs in array
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_directory_service_free_network_list_for_user() to free resources
 */
zwp_status_t zwp_directory_service_get_network_list_for_user(zwp_directory_service_t directory_service, char *user, char ***network_list_p, int *network_count_p);

/**
 * @brief Free list of network IDs
 * @param[in]     network_list      Array of network IDs
 * @param[in]     network_count     Number of network IDs in array
 * @return        Nothing
 * @see zwp_directory_service_get_network_list_for_user
 */
void zwp_directory_service_free_network_list_for_user(char **network_list, int network_count);

/**
 * @brief To check if a user has access to a given network
 * @param[in]  directory_service       Directory Service Object
 * @param[in]  user                    User name for whom the access is checked
 * @param[in]  net_id                  Network ID of the network for which the access is checked
 * @retval     ZWP_STATUS_OK           User has valid access to the network
 * @retval     ZWP_STATUS_ERROR        User has no access to the network or the operation failed
 */
zwp_status_t zwp_directory_service_check_network_authorization_for_user(zwp_directory_service_t directory_service, char *user, char *net_id);

/**
 * @brief Setup Directory Service connection
 * @param[out]    directory_service_p       Directory Service object
 * @param[in]     directory_service_config  Directory service configuration object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @post @a directory_service_p refers to a singleton \link _zwp_directory_service object \endlink
 * allocated/returned by this function (constructor)
 *
 * @note
 * @par
 * The caller should call zwp_directory_service_exit() to release Directory Service connection and free resources
 */
zwp_status_t zwp_directory_service_init(zwp_directory_service_t *directory_service_p, const zwp_directory_service_config_t directory_service_config);

/**
 * @brief Free Directory Service connection
 * @param[in]     directory_service              Directory Service object
 * @return        Nothing
 * @see zwp_directory_service_init
 */
void zwp_directory_service_exit(zwp_directory_service_t directory_service);

/**
 * @brief Get Directory Service configuration
 * @param[out]    directory_service_config_p     Directory Service configuration object
 * @param[in]     config_parser     Configuration parser handle
 * @param[in]     config_dir        Configuration directory path to convert path configurations from relative to absolute
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @post @a directory_service_config_p refers to a singleton \link _zwp_directory_service_config object \endlink
 * allocated/returned by this function (constructor)
 *
 * @note
 * @par
 * The caller should call zwp_directory_service_config_free() to free resources
 */
zwp_status_t zwp_directory_service_config_get_instance(zwp_directory_service_config_t *directory_service_config_p,
		const config_t *config_parser, const char *config_dir);

/**
 * @brief Free Directory Service configuration
 * @param[in]     directory_service_config       Directory service configuration object
 * @return        Nothing
 * @see zwp_directory_service_config_get_instance
 */
void zwp_directory_service_config_free(zwp_directory_service_config_t directory_service_config);

/** }@ */
#endif /* ZWP_DIRECTORY_SERVICE_H_ */
