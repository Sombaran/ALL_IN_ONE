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
@file		zwscn_scene_status_update.h  
  
                To provide support for Scene status update
  
@author		Arif Mohammand
 
@version	1.0 2013-12-05  Initial release
*/
#ifndef _ZWSCN_SCENE_STATUS_UPDATE_H_
#define _ZWSCN_SCENE_STATUS_UPDATE_H_

/** @defgroup GroupPrivateStatusUpdate Scene Status Update
 *  Functions for handling Scene Status Update
 *  @ingroup GroupPrivateGeneral
 */
///@{

/**
 * @brief	Send Scene status update request
 * @param[in] net			Network runtime context
 * @param[in] scene_id		Scene id
 * @param[in] action_id 	Action id
 * @param[in] message_type	Message type (e.g. ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_SCENE_ON_EXECUTE)
 * @return	  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_status_update_request_message_send(void *net, uint32_t scene_id, uint32_t action_id, uint8_t message_type);
/**
 * @brief	Start Status Update All request
 * @param[in] net					Network runtime context
 * @param[out] list_get_requests_ptr	List of GET requests to be sent
 * @return    ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_status_update_all_req_start(void *net, zwscn_list_t *list_get_requests_ptr);
/**
 * @brief	Start Scene Status Update request
 * @param[in] net					Network runtime context
 * @param[in] scene_ptr				Scene pointer
 * @param[in] action_id				Action id
 * @param[in] flag_is_poll_req		Flag to indicate if it is a ZWPoll request
 * @param[in] flag_use_cache		Flag to indicate if Device Report Cache can be used
 * @param[out] list_get_requests_ptr	List of GET requests to be sent
 * @return	  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_status_update_scene_req_start(void *net, zwscn_scene_cont_p scene_ptr, uint32_t action_id, ZWSCN_BOOL_T flag_is_poll_req, ZWSCN_BOOL_T flag_use_cache, zwscn_list_t *list_get_requests_ptr);
/**
 * @brief	Start Action Status Update request
 * @param[in] net					Network runtime context
 * @param[in] scene_ptr				Scene pointer
 * @param[in] action_ptr			Action pointer
 * @param[in] flag_is_poll_req		Flag to indicate if it is a ZWPoll request
 * @param[out] list_get_requests_ptr List of GET requests to be sent
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_status_update_action_req_start(void *net, zwscn_scene_cont_p scene_ptr, zwscn_action_p action_ptr, ZWSCN_BOOL_T flag_is_poll_req, zwscn_list_t *list_get_requests_ptr);
/**
 * @brief	Thread for receiving Scene Status Update requests
 * @param[in] data Thread data (message queue on which this thread would block)
 */
void zwscn_status_update_request_handler_thread(void *data);	//NOTE: this function uses zwscn_network_lock and zwscn_network_unlock
/**
 * @brief	Thread for receiving Device Reports
 * @param[in] data Thread data (message queue on which this thread would block)
 */
void zwscn_status_update_report_handler_thread(void *data); 	//NOTE: this function uses zwscn_network_lock and zwscn_network_unlock
/**
 * @brief	Callback function for Status Update request timeout
 * @param[in] user_data User data containing information about Network and Scene etc.
 * @return  0 to dismiss the timer
 */
int zwscn_status_update_on_timeout_callback(void *user_data);	//NOTE: this function uses zwscn_network_lock and zwscn_network_unlock
/**
 * @brief	Internal function to process Device Report for Status Update
 * @param[in] net		Network runtime context
 * @param[in] id		Report id
 * @param[in] cmd		command
 * @param[in] report	Report buffer
 * @return	  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_status_update_process_report(void *net, uint32_t id, uint32_t cmd, void *report);
/**
 * @brief	Scene Status Update on Device Report
 * @param[in] net		Network runtime context
 * @param[in] scene_ptr	Scene pointer
 * @param[in] id		Report id
 * @param[in] cmd		command
 * @param[in] report	Report buffer
 * @return	  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_status_update_scene_on_report(void *net, zwscn_scene_cont_p scene_ptr, uint32_t id, uint32_t cmd, void *report);
/**
 * @brief	Scene Status Update on timeout
 * @param[in] net						Network runtime context
 * @param[in] scene_ptr					Scene pointer
 * @param[in] action_id					Action id
 * @param[in] flag_update_other_scenes	Flag to indicate if other Scenes need to be updated
 * @return	  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_status_update_scene_on_timeout(void *net, zwscn_scene_cont_p scene_ptr, uint32_t action_id, ZWSCN_BOOL_T flag_update_other_scenes);
/**
 * @brief	Common function for Scene Status Update
 * @param[in] net							Network runtime context
 * @param[in] scene_ptr						Scene pointer
 * @param[in] id							Report id
 * @param[in] cmd							command
 * @param[in] report						Report buffer
 * @param[in] flag_status_update_on_report	Flag to indicate of Status Update is on Device Report trigger
 * @param[in] flag_update_other_scenes		Flag to indicate if other Scenes need to be updated
 * @return	  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_status_update_scene_common(void *net, zwscn_scene_cont_p scene_ptr, uint32_t id, uint32_t cmd, void *report, ZWSCN_BOOL_T flag_status_update_on_report, ZWSCN_BOOL_T flag_update_other_scenes);
/**
 * @brief	Action Status Update on Device Report
 * @param[in] net			Network runtime context
 * @param[in] scene_ptr		Scene pointer
 * @param[in] action_ptr	Action pointer
 * @param[in] id		Report id
 * @param[in] cmd		command
 * @param[in] report	Report buffer
 * @return	  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_status_update_action_on_report(void *net, zwscn_scene_cont_p scene_ptr, zwscn_action_p action_ptr, uint32_t id, uint32_t cmd, void *report);
/**
 * @brief	Action Status Update on timeout
 * @param[in] net						Network runtime context
 * @param[in] scene_ptr					Scene pointer
 * @param[in] action_ptr				Action pointer
 * @param[in] hash_key_action_id		Action id for hash lookup
 * @param[in] flag_update_other_scenes 	Flag to indicate if other Scenes need to be updated
 * @return	  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_status_update_action_on_timeout(void *net,zwscn_scene_cont_p scene_ptr, zwscn_action_p action_ptr, uint32_t hash_key_action_id, ZWSCN_BOOL_T flag_update_other_scenes);
/**
 * @brief	Action Status Update
 * @param[in] action_ptr			Action pointer
 * @param[in] new_status			New status to be set
 * @param[in] sutime				New status update time
 * @return	  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_status_update_action(zwscn_action_p action_ptr, ZWSCN_BOOL_T new_status, zwscn_time_t sutime);
/**
 * @brief	Status update other Scenes having same action
 * @param[in] net			Network runtime context
 * @param[in] scene_ptr		Scene pointer
 * @param[in] action_ptr	Action pointer
 * @return	  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_status_update_other_scenes_for_same_action(void *net, zwscn_scene_cont_p scene_ptr, zwscn_action_p action_ptr);
/**
 * @brief	Scene Status Update for status of current Action
 * @param[in] net			Network runtime context
 * @param[in] scene_ptr		Scene pointer
 * @return	  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_status_update_scene_for_current_action_status(void *net, zwscn_scene_cont_p scene_ptr);
/**
 * @brief	Start Status update timer
 * @param[in] net			Network runtime context
 * @param[in] scene_ptr		Scene pointer
 * @param[in] action_id		Action id
 * @param[in] count_reqs	Number of Action status update requests sent
 * @return	  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_status_update_start_timer(void *net, zwscn_scene_cont_p scene_ptr, uint32_t action_id, uint32_t count_reqs);
/**
 * @brief	Cancel Status 					update timer
 * @param[in] scene_ptr						Scene pointer
 * @param[in] action_id						Action id
 * @param[in] status_update_timer_info_ptr	Status update timer information
 * @return
 */
ZWSCN_STATUS_T zwscn_status_update_cancel_timer(zwscn_scene_cont_p scene_ptr, uint32_t action_id, zwscn_status_update_timer_info_p status_update_timer_info_ptr);

/**
 * @brief	Set Status Update Abort flag
 * @param[in] net	Network runtime context
 * @return	  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T	zwscn_status_update_abort_flag_set(void *net);
/**
 * @brief	Set Status Update Abort flag
 * @param[in] init_data	Init data
 * @return	  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_status_update_abort_flag_set_2(zwscn_init_p init_data);
/**
 * @brief	Clear Status Update Abort flag
 * @param[in] net	Network runtime context
 * @return	  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T	zwscn_status_update_abort_flag_clear(void *net);
/**
 * @brief	Get Status Update Abort flag
 * @param[in] net	Network runtime context
 * @return	  ZWSCN_BOOL_T
 */
ZWSCN_BOOL_T	zwscn_status_update_abort_flag_get(void *net);

//void zwscn_status_update_remove_pending_reqs_on_scene_delete(void *net, zwscn_scene_cont_p scene_ptr);
/**
 * @brief	Remove pending Status Update requests when All Actions are deleted
 * @param[in] net		Network runtime context
 * @param[in] scene_ptr	Scene pointer
 */
void zwscn_status_update_remove_pending_reqs_on_action_delete_all(void *net, zwscn_scene_cont_p scene_ptr);
/**
 * @brief	Remove pending Status Update requests when an Action is deleted
 * @param[in] net			Network runtime context
 * @param[in] scene_ptr		Scene pointer
 * @param[in] action_ptr	Action pointer
 */
void zwscn_status_update_remove_pending_reqs_on_action_delete(void *net, zwscn_scene_cont_p scene_ptr, zwscn_action_p action_ptr);
/**
 * @brief	Reset Status Update Pending flag
 * @param[in] net	Network runtime context
 */
void zwscn_status_update_reset_pending_flag(void *net);
///@}

#endif // _ZWSCN_SCENE_STATUS_UPDATE_H_
