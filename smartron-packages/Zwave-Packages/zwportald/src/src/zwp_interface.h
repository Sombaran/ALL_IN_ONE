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
 * @file    zwp_interface.h
 *
 * @brief   Z-Wave Interface module
 * @details Handles interface level of Z-Wave network tree.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-09-19
 * - Initial version
 */

#ifndef ZWP_INTERFACE_H_
#define ZWP_INTERFACE_H_

/**
 * \addtogroup  zwportaldzwave
 * @{
 */

#include "zwp_status.h"
#include "zwp_hcapi.h"
#include "zwp_thread_cond.h"

/** Waiting to receive Report command */
#define ZWP_INTERFACE_REPORT_RECEIVE_WAITING          (-1)
/** Done waiting to receive Report command */
#define ZWP_INTERFACE_REPORT_RECEIVE_DONE              0

/**
 * @brief Initialize waiting for network transmit status
 * @param[in]     desc_interface    HCAPI interface descriptor
 * @return        Nothing
 */
void zwp_interface_transmit_wait_init(zwifd_p desc_interface);

/**
 * @brief Wait for network transmit status
 * @param[in]     desc_interface    HCAPI interface descriptor
 * @param[out]    transmit_status   Transmit status
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_interface_transmit_wait(zwifd_p desc_interface, uint8_t *transmit_status);

/**
 * @brief Initialize waiting to receive Report command
 * @param[out]    condition_status_p   Report receive status
 * @return        Nothing
 */
void zwp_interface_report_receive_init(int *condition_status_p);

/**
 * @brief Mark as done waiting to receive Report command
 * @param[out]    condition_status_p   Report receive status
 * @param[in]     condition_variable   Report receive condition
 * @return        Nothing
 */
void zwp_interface_report_receive_done(int *condition_status_p, zwp_thread_cond_t condition_variable);

/**
 * @brief Test the Report receive status
 * @param[out]    condition_status  Report receive status
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_interface_report_receive_assertion(int condition_status);

/**
 * @brief Wait for receiving Report command
 * @param[in]     desc_interface    HCAPI interface descriptor
 * @param[in]     report_receive_condition   Report receive condition
 * @param[in]     timeout           Timeout for the wait (in seconds)
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_interface_report_receive_wait(zwifd_p desc_interface, zwp_thread_cond_t report_receive_condition, zwp_interval_time_t timeout);
/** }@ */
#endif /* ZWP_INTERFACE_H_ */
