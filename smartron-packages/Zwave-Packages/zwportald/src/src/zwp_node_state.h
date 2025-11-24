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
 * @file    zwp_node_state.h
 *
 * @brief   Node level data
 * @details Handles the node level data.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-01-07
 * - Initial version
 */

#ifndef ZWP_NODE_STATE_H_
#define ZWP_NODE_STATE_H_

/**
 * \addtogroup  zwportaldzwave
 * @{
 */

#include "zwp_status.h"
#include "zwp_hcapi.h"
#include "zwu_time.h"

/** Forward declaration for Firmware update target restart status class */
struct _firmware_update_target_restart_status;
/** Firmware update target restart status type */
typedef struct _firmware_update_target_restart_status *zwp_firmware_update_target_restart_status_t;

/** Forward declaration for node state class */
struct _zwp_node_state;
/** Node state type */
typedef struct _zwp_node_state *zwp_node_state_t;

/**
 * @name Getters
 * @{
 */
/**
 * @brief Test if the node is reachable (alive)
 * @param[in]     node_state        Node state object
 * @retval        0                 Not reachable
 * @retval        1                 Reachable
 */
uint8_t zwp_node_state_is_alive(zwp_node_state_t node_state);
/**
 * @brief Test if the node is secure
 * @param[in]     node_state        Node state object
 * @retval        0                 Not secure
 * @retval        1                 Secure
 */
uint8_t zwp_node_state_is_secure(zwp_node_state_t node_state);
/**
 * @brief Get extended version information from node state object
 * @param[in]     node_state        Node state object
 * @return        Extended version information. NULL if not available
 */
ext_ver_t *zwp_node_state_get_extended_version(zwp_node_state_t node_state);
/**
 * @}
 */

/**
 * @brief Get target restart status after firmware update
 * @param[in]     status            Target restart status object
 * @param[out]    status_utime_p    Last updated time of status
 * @param[out]    status_p          Target restart status
 * @return        Nothing
 */
void zwp_firmware_update_target_restart_status_get(zwp_firmware_update_target_restart_status_t status, zwu_time_t *status_utime_p, uint8_t *status_p);

/**
 * @brief Allocate and initialize a new target restart status object
 * @param[out]    status_p          Target restart status object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_firmware_update_target_restart_status_free() to free resources
 */
zwp_status_t zwp_firmware_update_target_restart_status_alloc(zwp_firmware_update_target_restart_status_t *status_p);
/**
 * @brief Free a target restart status object
 * @param[in]     status            Target restart status object
 * @return        Nothing
 * @see zwp_firmware_update_target_restart_status_alloc
 */
void zwp_firmware_update_target_restart_status_free(zwp_firmware_update_target_restart_status_t status);

/**
 * @brief Add a target restart status object as a listener of status updates
 * @param[in]     node_state        Node state object
 * @param[in]     restart_status    Target restart status object
 * @param[in]     restart_begin_time  Time at which the restart began
 * @param[in]     restart_wait_time Waiting time for target to complete restart
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_node_state_add_firmware_update_restart_status_listener(zwp_node_state_t node_state, zwp_firmware_update_target_restart_status_t restart_status,
		zwu_time_t restart_begin_time, int restart_wait_time);
/**
 * @brief An event to indicate target restart status
 * @param[in]     node_state        Node state object
 * @param[in]     status_utime      Last updated time of status
 * @param[in]     status            Status - ZW_FW_UPDT_RESTART_XXX
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
void zwp_node_state_event_firmware_update_target_restart_status(zwp_node_state_t node_state, zwu_time_t status_utime, uint8_t status);

/**
 * @brief Free a node level state object
 * @param[in]     node_state        Node level state object
 * @return        Nothing
 */
void zwp_node_state_free(zwp_node_state_t node_state);

/**
 * @brief Get node level state object from HCAPI node descriptor
 * @param[in]     desc_node         HCAPI node descriptor
 * @return        Node level state object
 */
zwp_node_state_t zwp_node_state_get(zwnoded_p desc_node);

/**
 * @brief Refresh node level state of a given node descriptor
 * @param[in]     desc_node         HCAPI node descriptor
 * @param[out]    state_copy_p      If not NULL, a copy of the node level state object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_node_state_free() to free resources if state_copy_p is not NULL
 */
zwp_status_t zwp_node_state_refresh(zwnoded_p desc_node, zwp_node_state_t *state_copy_p);

/**
 * @brief Create or update node level state information of a node descriptor
 * @param[in]     desc_node         HCAPI node descriptor
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_node_state_destroy() to free resources
 */
zwp_status_t zwp_node_state_create(zwnoded_p desc_node);
/**
 * @brief Free node level state information of a node descriptor
 * @param[in]     desc_node         HCAPI node descriptor
 * @return        Nothing
 * @see zwp_node_state_create
 */
void zwp_node_state_destroy(zwnoded_p desc_node);
/** }@ */
#endif /* ZWP_NODE_STATE_H_ */
