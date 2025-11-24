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
@file		zwscn_notification_processor_security_scene.h  
  
                To provide support for processing network and node notifications for Security Scenes
  
@author		Arif Mohammand
 
@version	1.0 2015-04-22  Initial release
*/
#ifndef _ZWSCN_NOTIFICATION_PROCESSOR_SECURITY_SCENE_H_
#define _ZWSCN_NOTIFICATION_PROCESSOR_SECURITY_SCENE_H_

/** @defgroup GroupPrivateNotificationProcessorsSecurityScene Notification processor functions for Security Scenes
 *  Functions for processing various notifications for Security Scenes
 *  @ingroup GroupPrivateGeneral
 */
///@{
/**
 * @brief	Helper function to remove Security Scenes by Node id
 * @param[in] net		Network runtime context
 * @param[in] node_id	Node id
 * @return	  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T 	zwscn_remove_security_scenes_by_node_id(void *net, uint8_t node_id);
/**
 * @brief	Foreach-loop callback for helper function to remove Security Scenes by Node id
 * @param[in] list_item	List item (Security Scene container pointer)
 * @param[in] user_data	User data (information about List item)
 */
void 			zwscn_remove_security_scenes_by_node_id_list_callback(void *list_item, void *user_data);
/**
 * @brief	Helper function to remove Security Events by Node id
 * @param[in] net			Network runtime context
 * @param[in] node_id		Node id
 * @param[in] scene_ptr 	parent Security Scene pointer
 * @param[in] event_list	Event list (arm/disarm/alarm events)
 * @return    ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T 	zwscn_remove_security_events_by_node_id(void *net, uint8_t node_id, zwscn_security_scene_cont_p scene_ptr, zwscn_list_t *event_list);
/**
 * @brief	Foreach-loop callback for helper function to remove Security Events by Node id
 * @param[in] list_item	List item (Security Event pointer)
 * @param[in] user_data	User data (information about List item)
 */
void 			zwscn_remove_security_events_by_node_id_list_callback(void *list_item, void *user_data);
/**
 * @brief	Helper function to validate Security Scenes by Node id
 * @param[in] net		Network runtime context
 * @param[in] node_id	Node id
 * @return	  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T	zwscn_validate_security_scenes_on_notification(void *net, uint8_t node_id);
/**
 * @brief	Foreach-loop callback for helper function to validate Security Scenes by Node id from valid scene list
 * @param[in] list_item	List item (Security Scene pointer)
 * @param[in] user_data	User data (information about List item)
 */
void			zwscn_validate_security_scenes_from_valid_list_callback(void *list_item, void *user_data);
/**
 * @brief	Foreach-loop callback for helper function to validate Security Scenes by Node id from invalid scene list
 * @param[in] list_item	List item (Security Scene pointer)
 * @param[in] user_data	User data (information about List item)
 */
void			zwscn_validate_security_scenes_from_invalid_list_callback(void *list_item, void *user_data);
/**
 * @brief	Helper function to validate Security Events by Node id
 * @param[in] net					Network runtime context
 * @param[in] node_id				Node id
 * @param[in] scene_ptr 			parent Security Scene pointer
 * @param[in] list_valid			Valid security events list (arm/disarm/alarm events)
 * @param[in] list_invalid			Invalid security events list (arm/disarm/alarm events)
 * @param[in] security_event_type	Event type (arm/disarm/alarm)
 * @return	  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T	zwscn_validate_security_events(void *net, uint8_t node_id, zwscn_security_scene_cont_p scene_ptr, zwscn_list_t *list_valid, zwscn_list_t *list_invalid, uint8_t security_event_type);
/**
 * @brief	Foreach-loop callback for helper function to validate Security Events by Node id from valid action list
 * @param[in] list_item	List item (Security Scene pointer)
 * @param[in] user_data	User data (information about List item)
 */
void			zwscn_validate_security_events_from_valid_list_callback(void *list_item, void *user_data);
/**
 * @brief	Foreach-loop callback for helper function to validate Security Events by Node id from invalid action list
 * @param[in] list_item	List item (Security Scene pointer)
 * @param[in] user_data	User data (information about List item)
 */
void			zwscn_validate_security_events_from_invalid_list_callback(void *list_item, void *user_data);
/**
 * @brief				Update Security Scenes when a normal Scene is deleted
 * @param[in] net		Network runtime context
 * @param[in] scene_id	Scene id
 * @return ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_security_scene_update_on_scene_deletion(void *net, uint32_t scene_id);
///@}

#endif // _ZWSCN_NOTIFICATION_PROCESSOR_SECURITY_SCENE_H_
