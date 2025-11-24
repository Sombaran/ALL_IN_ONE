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
@file		zwscn_util_list.h  
  
                To provide support for list data structure
  
@author		Arif Mohammand
 
@version	1.0 2013-12-05  Initial release
*/
#ifndef _ZWSCN_UTIL_LIST_H_
#define _ZWSCN_UTIL_LIST_H_

/** @defgroup GroupPrivateUtilList List utils
 *  Utility functions for List data structure
 *  @ingroup GroupPrivateUtils
 */
///@{

typedef void *zwscn_list_t; 												/**< Opaque handle for a List */
typedef void (*zwscn_list_foreach_fn_t) (void *list_item, void *user_data);	/**< Prototype of callback function to be used in foreach loop */


zwscn_list_t zwscn_list_next(zwscn_list_t list);

/**
 * @brief	Add an item to List
 * @param[in] list	List
 * @param[in] data	Data to add
 * @return	  List handle
 */
zwscn_list_t zwscn_list_add(zwscn_list_t list, void *data);
/**
 * @brief	Insert an item to List at a given position
 * @param[in] list		List
 * @param[in] data		Data to add
 * @param[in] position 	Position
 * @return	  List handle
 */
zwscn_list_t zwscn_list_insert(zwscn_list_t list, void *data, int position);
/**
 * @brief	Insert a string into the sorted List
 * @param[in] list	List
 * @param[in] data	String data to add
 * @return	  List handle
 */
zwscn_list_t zwscn_list_insert_sorted_strings(zwscn_list_t list, void *data);
/**
 * @brief	Remove an item from List
 * @param[in] list	List
 * @param[in] data	Data to remove
 * @return	  List handle
 */
zwscn_list_t zwscn_list_remove(zwscn_list_t list, void *data);
/**
 * @brief	Get List size
 * @param[in] list	List
 * @return	  Number of items in the List
 */
int zwscn_list_size(zwscn_list_t list);
/**
 * @brief	Get index of the data
 * @param[in] list	List
 * @param[in] data	Data to remove
 * @return Index
 */
int zwscn_list_get_index(zwscn_list_t list, void *data);
/**
 * @brief	Iterate over List items using foreach-loop
 * @param[in] list						List
 * @param[in] list_foreach_handler_func	Callback function called on for each List item
 * @param[in] user_data					User data
 */
void zwscn_list_foreach(zwscn_list_t list, zwscn_list_foreach_fn_t list_foreach_handler_func, void *user_data);
/**
 * @brief	Get List item by index
 * @param[in] list	List
 * @param[in] index	Index of the item
 * @return	  Value
 */
void *zwscn_list_get_by_index(zwscn_list_t list, unsigned int index);
/**
 * @brief	Get Scene from List matching input Scene id
 * @param[in] list	List
 * @param[in] id	Scene id
 * @return	  Scene pointer
 */
void *zwscn_list_get_scene_by_id(zwscn_list_t list, uint32_t id);
/**
 * @brief	Get Action from List matching input Action id
 * @param[in] list	List
 * @param[in] id	Action id
 * @return	  Action pointer
 */
void *zwscn_list_get_action_by_id(zwscn_list_t list, uint32_t id);
/**
 * @brief	Get Schedule from List matching input Schedule id
 * @param[in] list	List
 * @param[in] id	Schedule id
 * @return	  Schedule pointer
 */
void *zwscn_list_get_schedule_by_id(zwscn_list_t list, uint32_t id);
/**
 * @brief	Get Event from List matching input Event id
 * @param[in] list	List
 * @param[in] id	Event id
 * @return	  Event pointer
 */
void *zwscn_list_get_event_by_id(zwscn_list_t list, uint32_t id);

/**
 * @brief	Free a List
 * @param[in] list_ptr	List
 */
void zwscn_list_free(zwscn_list_t *list_ptr); 				/**< @remark generic function - uses free() */
/**
 * @brief	Free Scene List
 * @param[in] list_ptr	Scene List
 */
void zwscn_list_free_scene_list(zwscn_list_t *list_ptr);
/**
 * @brief	Free Action List
 * @param[in] list_ptr	Action List
 */
void zwscn_list_free_action_list(zwscn_list_t *list_ptr);
/**
 * @brief	Free Schedule List
 * @param[in] list_ptr	Schedule List
 */
void zwscn_list_free_schedule_list(zwscn_list_t *list_ptr);
/**
 * @brief	Free Event List
 * @param[in] list_ptr	Event List
 */
void zwscn_list_free_event_list(zwscn_list_t *list_ptr);
/**
 * @brief	Get Security Scene from List matching input Scene id
 * @param[in] list	List
 * @param[in] id	Scene id
 * @return	  Security Scene pointer
 */
void *zwscn_list_get_security_scene_by_id(zwscn_list_t list, uint32_t id);
/**
 * @brief	Get Security Event from List matching input Event id
 * @param[in] list	List
 * @param[in] id	Event id
 * @return	  Security Event pointer
 */
void *zwscn_list_get_security_event_by_id(zwscn_list_t list, uint32_t id);
/**
 * @brief	Free Security Scene List
 * @param[in] list_ptr	Security Scene List
 */
void zwscn_list_free_security_scene_list(zwscn_list_t *list_ptr);
/**
 * @brief	Free Arm Event List
 * @param[in] list_ptr	Event List
 */
void zwscn_list_free_arm_security_event_list(zwscn_list_t *list_ptr);
/**
 * @brief	Free Disarm Event List
 * @param[in] list_ptr	Event List
 */
void zwscn_list_free_disarm_security_event_list(zwscn_list_t *list_ptr);
/**
 * @brief	Free Alarm Event List
 * @param[in] list_ptr	Event List
 */
void zwscn_list_free_alarm_security_event_list(zwscn_list_t *list_ptr);
///@}

#endif // _ZWSCN_UTIL_LIST_H_
