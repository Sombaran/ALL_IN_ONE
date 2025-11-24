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
@file		zwscn_security_scene.h  
  
                Header for genral functions for Scene core functionality
  
@author		Arif Mohammand
 
@version	1.0 2015-04-16  Initial release
*/
#ifndef _ZWSCN_SECURITY_SCENE_H_
#define _ZWSCN_SECURITY_SCENE_H_

/** @defgroup GroupPrivateSecurityScene Security Scenes Housekeeping and helper functions
 *  Data structures and helper functions for internal use
 *  @ingroup GroupPrivateGeneral
 */
///@{

/** Security Scene container */
typedef struct zwscn_security_scene_container {
	void *net; 										/**< Network runtime context @remark back reference */
	zwscn_security_scene_p scene;					/**< Security Scene data */
	zwscn_list_t list_arm_events;					/**< List of Security Scene Arm events */
	zwscn_list_t list_disarm_events;				/**< List of Security Scene Disarm events */
	zwscn_list_t list_alarm_events;					/**< List of Security Scene Alarm events  */
	
	zwscn_list_t list_arm_events_invalid;			/**< List of Security Scene invalid Arm events */
	zwscn_list_t list_disarm_events_invalid;		/**< List of Security Scene invalid Disarm events */
	zwscn_list_t list_alarm_events_invalid;			/**< List of Security Scene invalid Alarm events  */
} zwscn_security_scene_cont_t, *zwscn_security_scene_cont_p;		/**< Security Scene container */

/**
 * @brief	Get Security Scene Events message queue
 * @param[in] net			Network runtime context
 * @param[out] mqueue_ptr	Message queue
 * @return     ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_mqueue_thread_security_events(void *net, zwscn_mqueue_t *mqueue_ptr);
/**
 * @brief	Get Security Scene notification request handler message queue
 * @param[in] net			Network runtime context
 * @param[out] mqueue_ptr	Message queue
 * @return     ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_mqueue_thread_security_scene_notification_request_handler(void *net, zwscn_mqueue_t *mqueue_ptr);
/**
 * @brief	Get Security Scene list for a network
 * @param[in] net				Network runtime context
 * @param[out] scene_list_ptr	Security Scene list
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_security_scene_list(void *net, zwscn_list_t **scene_list_ptr);
/**
 * @brief	Get Arm Event list for a Scene
 * @param[in] net				Network runtime context
 * @param[in] scene_id			Scene id
 * @param[out] list_ptr			Arm Event list
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_arm_security_event_list(void *net, uint32_t scene_id, zwscn_list_t **list_ptr);
/**
 * @brief	Get Disarm Event list for a Scene
 * @param[in] net				Network runtime context
 * @param[in] scene_id			Scene id
 * @param[out] list_ptr			Disarm Event list
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_disarm_security_event_list(void *net, uint32_t scene_id, zwscn_list_t **list_ptr);
/**
 * @brief	Get Alarm Event list for a Scene
 * @param[in] net				Network runtime context
 * @param[in] scene_id			Scene id
 * @param[out] list_ptr			Alarm Event list
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_alarm_security_event_list(void *net, uint32_t scene_id, zwscn_list_t **list_ptr);
/**
 * @brief	Get Security Scene by id
 * @param[in] net			Network runtime context
 * @param[in] scene_id		Scene id
 * @param[out] scene_ptr 	Security Scene pointer
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_security_scene(void *net, uint32_t scene_id, zwscn_security_scene_cont_p *scene_ptr);
/**
 * @brief	Get Arm Event
 * @param[in] net			Network runtime context
 * @param[in] scene_id		Scene id
 * @param[in] id			Event id
 * @param[out] out_event	Event pointer
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_arm_security_event(void *net, uint32_t scene_id, uint32_t id, zwscn_event_p *out_event);
/**
 * @brief	Get Disarm Event
 * @param[in] net			Network runtime context
 * @param[in] scene_id		Scene id
 * @param[in] id			Event id
 * @param[out] out_event	Event pointer
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_disarm_security_event(void *net, uint32_t scene_id, uint32_t id, zwscn_event_p *out_event);
/**
 * @brief	Get Alarm Event
 * @param[in] net			Network runtime context
 * @param[in] scene_id		Scene id
 * @param[in] id			Event id
 * @param[out] out_event	Event pointer
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_alarm_security_event(void *net, uint32_t scene_id, uint32_t id, zwscn_event_p *out_event);
/**
 * @brief	Check if Security Scene list is full
 * @param[in] net			Network runtime context
 * @param[in] scene_list	Scene list
 * @return	  ZWSCN_BOOL_T
 */
ZWSCN_BOOL_T zwscn_is_list_full_security_scene(void *net, zwscn_list_t scene_list);
/**
 * @brief	Check if Arm Event list is full
 * @param[in] net			Network runtime context
 * @param[in] event_list	Event list
 * @return	  ZWSCN_BOOL_T
 */
ZWSCN_BOOL_T zwscn_is_list_full_arm_security_event(void *net, zwscn_list_t event_list);
/**
 * @brief	Check if Disarm Event list is full
 * @param[in] net			Network runtime context
 * @param[in] event_list	Event list
 * @return	  ZWSCN_BOOL_T
 */
ZWSCN_BOOL_T zwscn_is_list_full_disarm_security_event(void *net, zwscn_list_t event_list);
/**
 * @brief	Check if Alarm Event list is full
 * @param[in] net			Network runtime context
 * @param[in] event_list	Event list
 * @return	  ZWSCN_BOOL_T
 */
ZWSCN_BOOL_T zwscn_is_list_full_alarm_security_event(void *net, zwscn_list_t event_list);
/**
 * @brief	Generate Security Scene id
 * @param[in] net	Network runtime context
 * @return    Security Scene id
 */
uint32_t		zwscn_generate_security_scene_id(void *net);
/**
 * @brief	Get Security Scene id pool index
 * @param[in] net	Network runtime context
 * @return    Security Scene id pool index
 */
uint32_t		zwscn_get_security_scene_id_pool_index(void *net);
/**
 * @brief	Set Security Scene id pool index
 * @param[in] net	Network runtime context
 * @param[in] index	Security Scene id pool index
 */
void			zwscn_set_security_scene_id_pool_index(void *net, uint32_t index);
/**
 * @brief	Set Arm/Disarm state
 * @param[in] net					Network runtime context
 * @param[in] scene_ptr 			Security Scene pointer
 * @param[in] flag_arm_disarm_state	Arm/Disarm state to set (1=Arm, 0=Disarm)
 * @param[in] trigger_type			Trigger type for this change
 * @param[in] event					Event value if trigger types was Event otherwise NULL
 */
void			zwscn_security_scene_arm_state_set(void *net, zwscn_security_scene_cont_p scene_ptr, ZWSCN_BOOL_T flag_arm_disarm_state, uint8_t trigger_type, zwscn_security_event_p event);
/**
 * @brief	Get Arm/Disarm state
 * @param[in] scene_ptr	Security Scene pointer
 * @return	Current Arm/Disarm state
 */
ZWSCN_BOOL_T	zwscn_security_scene_arm_state_get(zwscn_security_scene_cont_p scene_ptr);
/**
 * @brief	Set Alarm On/Off state
 * @param[in] net					Network runtime context
 * @param[in] scene_ptr 			Security Scene pointer
 * @param[in] flag_alarm_state		Alarm state to set (1=On, 0=Off)
 * @param[in] trigger_type			Trigger type for this change
 * @param[in] event					Event value if trigger types was Event otherwise NULL
 */
void			zwscn_security_scene_alarm_state_set(void *net, zwscn_security_scene_cont_p scene_ptr, ZWSCN_BOOL_T flag_alarm_state, uint8_t trigger_type, zwscn_security_event_p event);
/**
 * @brief	Get Alarm On/Off state
 * @param[in] scene_ptr	Security Scene pointer
 * @return	Current Alarm On/Off state
 */
ZWSCN_BOOL_T	zwscn_security_scene_alarm_state_get(zwscn_security_scene_cont_p scene_ptr);
/**
 * @brief	Get statelog data structure
 * @param[in] net		Network runtime context
 * @param[in] category	Statelog category
 * @return statelog data structure
 */
zwscn_statelog_security_scene_p zwscn_security_scene_get_statelog_data(void *net, uint32_t category);
/**
 * @brief	Utility function to copy Security Scenes Event
 * @param[in] event	Arm/Disarm/Alarm event
 * @return Copy of input event
 */
zwscn_security_event_p zwscn_security_scene_copy_event(zwscn_security_event_p event);
/**
 * @brief	Update timestamp of Security Scenes after any change
 * @param[in] scene_ptr 			Security Scene pointer
 */
void zwscn_security_scene_update_timestamp(zwscn_security_scene_cont_p scene_ptr);
/**
 * @brief	Get maximum number of Arm Events
 * @param[in] net	Network runtime context
 * @return Maximum number of Arm Events
 */
int zwscn_get_max_arm_events(void *net);
/**
 * @brief	Get maximum number of Disarm Events
 * @param[in] net	Network runtime context
 * @return Maximum number of Disarm Events
 */
int zwscn_get_max_disarm_events(void *net);
/**
 * @brief	Get maximum number of Alarm Events
 * @param[in] net	Network runtime context
 * @return Maximum number of Alarm Events
 */
int zwscn_get_max_alarm_events(void *net);
///@}

#endif // _ZWSCN_SECURITY_SCENE_H_
