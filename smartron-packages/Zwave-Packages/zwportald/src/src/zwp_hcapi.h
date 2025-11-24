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
 * @file    zwp_hcapi.h
 *
 * @brief   HCAPI helpers
 * @details Helper functions for HCAPI calls.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-05-03
 * - Initial version
 */

#ifndef ZWP_HCAPI_H_
#define ZWP_HCAPI_H_

/**
 * \addtogroup  zwportaldzwarec
 * @{
 */

#include "zip_api.h"

/** Generic HCAPI thread name */
#define ZWP_HCAPI_THREAD_NAME                                  "HCAPI"
/** Name for HCAPI threads created as part of zwportal_init() */
#define ZWP_HCAPI_PORTAL_INIT_THREAD_NAME                      "HCAPIPortal"
/** Name for HCAPI threads created as part of zwnet_init() */
#define ZWP_HCAPI_NET_INIT_THREAD_NAME                         "HCAPINet"
/** Name for HCAPI threads created as part of zwnet_load() */
#define ZWP_HCAPI_NET_LOAD_THREAD_NAME                         "HCAPILoad"

/** Marker for HCAPI notifications (callbacks) */
#define ZWP_HCAPI_NOTIFICATION

/** Z-Wave Controller Role - Proxy */
#define ZWP_HCAPI_CONTROLLER_ROLE_STRING_PROXY                 "Proxy"
/** Z-Wave Controller Role - Inclusion */
#define ZWP_HCAPI_CONTROLLER_ROLE_STRING_INCLUSION             "Inclusion"
/** Z-Wave Controller Role - Primary */
#define ZWP_HCAPI_CONTROLLER_ROLE_STRING_PRIMARY               "Primary"
/** Maximum size of string needed to show all Z-Wave Controller Roles */
#define ZWP_HCAPI_CONTROLLER_ROLE_STRING_BUFFER_MAX_SIZE \
		(sizeof ZWP_HCAPI_CONTROLLER_ROLE_STRING_PROXY \
		+ sizeof ZWP_HCAPI_CONTROLLER_ROLE_STRING_INCLUSION \
		+ sizeof ZWP_HCAPI_CONTROLLER_ROLE_STRING_PRIMARY)


/** Progress status of adding node and migrating primary controller */
//#define     OP_ADD_NODE_PROTOCOL_DONE   1   /**< Protocol part of adding node done*/ defined by HCAPI
//#define     OP_ADD_NODE_GET_NODE_INFO   2   /**< Getting node detailed information*/ defined by HCAPI
//#define     OP_ADD_NODE_PROTOCOL_START  3   /**< Prekit add node Z-wave protocol started*/
#define     OP_ADD_NODE_SEC_REQ_KEY_READY    11		/**< Additional status from HCAPI to notify client request key is ready*/
#define     OP_ADD_NODE_SEC_DSK_READY		 12		/**< Additional status from HCAPI to notify client DSK is ready*/
#define     OP_ADD_NODE_ON_BEHALF_SEC_REQ_KEY_READY		21		/**< Additional status from HCAPI to notify on-behalf unsolicited add node client request key is ready*/
#define     OP_ADD_NODE_ON_BEHALF_SEC_DSK_READY			22		/**< Additional status from HCAPI to notify on-behalf unsolicited add node client DSK is ready*/


/** Progress status of initiate */
//#define     OP_INI_PROTOCOL_DONE        1   /**< Protocol part of initiating done*/
//#define     OP_INI_GET_NODE_INFO        2   /**< Getting node detailed information*/
#define     OP_INI_SEC_OWN_DSK_READY		11		/**< Additional status from HCAPI to notify local node DSK is ready*/


/** Test for HCAPI return code status */

#define HCAPI_SUCCEEDED(hcapi_code) ((hcapi_code == ZW_ERR_NONE) || (hcapi_code == ZW_ERR_QUEUED) || (hcapi_code == ZW_ERR_CACHE_AVAIL))


/**
 * @brief Get error description from HCAPI error code
 * @param[in]     hcapi_code        HCAPI error code
 * @return        Error description
 */
char *zwp_hcapi_error_code_to_string(int hcapi_code);

/**
 * @brief Get type description from HCAPI types
 * @param[in]     type              HCAPI type
 * @return        Type description
 */
char *zwp_hcapi_type_to_string(int type);

/**
 * @brief Get Z-Wave network operation description from network operation code
 * @param[in]     operation         Network operation code
 * @return        Network operation description
 */
char *zwp_hcapi_net_operation_to_string(uint8_t operation);

/**
 * @brief Get Z-Wave network operation status description from network operation status code
 * @param[in]     status            Network operation status code
 * @return        Network operation status description
 */
char *zwp_hcapi_net_operation_status_to_string(uint16_t status);

/**
 * @brief Get Z-Wave network operation intermediate status description from network operation intermediate status code
 * @param[in]     operation         Network operation code
 * @param[in]     status            Network operation intermediate status code
 * @return        Network operation intermediate status description
 */
char *zwp_hcapi_net_operation_intermediate_status_to_string(uint8_t operation, uint8_t status);

/**
 * @brief Get Z-Wave network node status description from network node status code
 * @param[in]     status            Network node status code
 * @return        Network node status description
 */
char *zwp_hcapi_net_node_status(int status);

/**
 * @brief Get Z-Wave network transmit status description from network transmit status code
 * @param[in]     status            Network transmit status code
 * @return        Network transmit status description
 */
char *zwp_hcapi_net_transmit_status_to_string(int8_t status);

/**
 * @brief Get Z-Wave network transmit status description from network transmit status code
 * @param[in]     role_bitmap       Controller role bitmap
 * @param[out]    buffer            Buffer to hold controller role description string
 * @param[in]     buffer_size       Size of passed buffer @a buffer
 * @return        Nothing
 */
void zwp_hcapi_controller_role_to_string(int8_t role_bitmap, char *buffer, size_t buffer_size);

/**
 * @brief Get Firmware update restart status description from Firmware update restart status code
 * @param[in]     status            Firmware update restart status code
 * @return        Firmware update restart status description
 */
char *zwp_hcapi_firmware_update_target_restart_status_to_string(int8_t status);

/**
 * @brief Get command class name from command class ID and command class version
 * @param[in]     class_id          Command class ID
 * @param[in]     class_version     Command class version
 * @return        Command class name
 */
char *zwp_hcapi_zwave_command_class_to_string(uint16_t class_id, uint8_t class_version);

/**
 * @brief Get command name from command class ID and command ID
 * @param[in]     class_id          Command class ID
 * @param[in]     command_id        Command ID
 * @return        Command name
 */
char *zwp_hcapi_zwave_command_to_string(uint16_t class_id, uint8_t command_id);
/** }@ */
#endif /* ZWP_HCAPI_H_ */
