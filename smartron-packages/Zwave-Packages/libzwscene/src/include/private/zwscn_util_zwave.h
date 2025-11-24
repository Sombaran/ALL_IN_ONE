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
@file		zwscn_util_zwave.h  
  
                To provide support for Z-Wave related functionality 
  
@author		Arif Mohammand
 
@version	1.0 2013-12-05  Initial release
*/
#ifndef _ZWSCN_UTIL_ZWAVE_H_
#define _ZWSCN_UTIL_ZWAVE_H_

/** @defgroup GroupPrivateUtilZWave Z-Wave utils
 *  Utility functions for Z-Wave
 *  @ingroup GroupPrivateUtils
 */
///@{

/**
 * @brief	Prepare arguments for Z-Wave Get operation
 * @param[in] net			Network runtime context
 * @param[in] action_ptr	Action pointer
 * @return	Get operation arguments (as void *)
 */
void *zwscn_zwave_prepare_get_op_args(void *net, zwscn_action_p action_ptr);
/**
 * @brief	arguments for Z-Wave Set operation
 * @param[in] net			Network runtime context
 * @param[in] action_ptr	Action pointer
 * @return	Set operation arguments (as void *)
 */
void *zwscn_zwave_prepare_set_op_args(void *net, zwscn_action_p action_ptr);
/**
 * @brief	Get the count of ZWPoll for a give command class (CC)
 * @param[in] action_ptr Actopm
 * @return count of ZWPoll
 */
uint16_t zwscn_zwave_get_zwpoll_count(zwscn_action_p action_ptr);
/**
 * @brief	Get the GET operation cmd value for a give command class (CC)
 * @param[in] cls_id	Command Class id
 * @param[in] cmd		cmd value for SET operation
 * @return cmd value
 */
uint8_t zwscn_zwave_find_cmd_get(uint16_t cls_id, uint8_t cmd);
/**
 * @brief	Get the unit string from unit type
 * @param[in] cls_id	Command Class id
 * @param[in] type		type of the unit (e.g. Temprature)
 * @param[in] unit		unit
 * @return string representing the unit or NULL if the unit is not valid
 */
const char *zwscn_zwave_get_unit_string(uint16_t cls_id, uint8_t type, uint8_t unit);
/**
 * @brief	Check if Command Class is valid for Scene Action
 * @param[in] cls_id	Command Class id
 * @param[in] cmd		cmd value
 * @return    Boolean value
 */
ZWSCN_BOOL_T zwscn_zwave_is_cc_for_action(uint16_t cls_id, uint8_t cmd);
/**
 * @brief	Check if Command Class is valid for Scene Event
 * @param[in] cls_id	Command Class id
 * @param[in] cmd		cmd value
 * @return	  Boolean value
 */
ZWSCN_BOOL_T zwscn_zwave_is_cc_for_event(uint16_t cls_id, uint8_t cmd);
/**
 * @brief	Check if Command Class is valid for Security Scene Event for Arm/Disarm trigger
 * @param[in] cls_id	Command Class id
 * @param[in] cmd		cmd value
 * @return	  Boolean value
 */
ZWSCN_BOOL_T zwscn_zwave_is_cc_for_arm_security_event(uint16_t cls_id, uint8_t cmd);
/**
 * @brief	Check if Command Class is valid for Security Scene Event for Alarm trigger
 * @param[in] cls_id	Command Class id
 * @param[in] cmd		cmd value
 * @return	  Boolean value
 */
ZWSCN_BOOL_T zwscn_zwave_is_cc_for_alarm_security_event(uint16_t cls_id, uint8_t cmd);
/**
 * @brief	Check if Command Class has Post-Set Poll
 * @param[in] cls_id	Command Class id
 * @param[in] cmd		cmd value
 * @return	  Boolean value
 */
ZWSCN_BOOL_T zwscn_zwave_cc_has_postset_poll(uint16_t cls_id, uint8_t cmd);
/**
 * @brief	Get timestamp from Device Report
 * @param[in] cls_id			Command Class id
 * @param[in] report			Report buffer
 * @param[in] report_size		Report size
 * @param[out] report_timestamp	Timestamp
 * @return     ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_zwave_get_device_report_timestamp(uint16_t cls_id, void *report, uint32_t report_size, zwscn_time_t *report_timestamp);
/**
 * @brief	Compare Device Report value with configured Device Arguments
 * @param[in] net 			Network runtime context
 * @param[in] device_args	Device arguments
 * @param[in] id			Report id
 * @param[in] cmd			Report cmd
 * @param[in] report		Report buffer
 * @param[out] out_result	Result of comparison (Boolean value)
 * @param[out] out_sutime	Status Update Time
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_zwave_compare_cc(void *net, void *device_args, uint32_t id, uint32_t cmd, void *report, ZWSCN_BOOL_T *out_result, zwscn_time_t *out_sutime);
/**
 * @brief	Get string for a Command Class
 * @param[in] cls_id Command class id
 * @return String representation of CC
 */
const char *zwscn_zwave_get_cc_string(uint16_t cls_id);
/**
 * @brief	Get string representation of Command Class specific arguments
 * @param[in] cls_id			Command class id
 * @param[in] device_args		Device specific arguments
 * @param[out] output_buf		Buffer for output string
 * @param[in] output_buf_size	Output buffer size
 * @return String representation of CC specific arguments
 */
ZWSCN_STATUS_T zwscn_zwave_get_cc_args_string(uint16_t cls_id, void *device_args, char *output_buf, size_t output_buf_size);
/**
 * @brief	Get Binary Sensor CC string
 * @param[in] type	Binary Sensor type
 * @return String representation of Binary Sensor CC
 */
const char *zwscn_zwave_get_cc_sensor_binary_string(uint8_t type);
/**
 * @brief	Get Alarm CC string
 * @param[in] alarm_type	Alarm type
 * @param[in] event_type	Event type
 * @return String representation of Alarm CC
 */
const char *zwscn_zwave_get_cc_alarm_string(uint8_t alarm_type, uint8_t event_type);

/**
* @brief	Utility function to copy device_arg field in event for 'last triggered'
* @param[in] src_event	Incoming event whose device_args will be copied from
* @param[in,out] dest_event Event whose device_args will be copied to 
* @return	   ZWSCN_STATUS_T
* @note For those events that contains a group of triggers (eg. Alarm CC), the function will only copy
* the last report matched trigger into the device_arg.
*/
ZWSCN_STATUS_T zwscn_copy_device_arg_2_lt(zwscn_event_p src_event, zwscn_event_p dest_event);

///@}

#endif // _ZWSCN_UTIL_ZWAVE_H_
