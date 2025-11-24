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
@file   	zwscn_security_scene_api.h - Security Scenes API

			Security Scenes API public definitions.

@author 	Arif

@version    1.0.0 2015-03-04

@ingroup GroupPublicAPI
*/
#ifndef _ZWSCN_SECURITY_SCENE_API_H_
#define _ZWSCN_SECURITY_SCENE_API_H_

#include "stdint.h"

/** @addtogroup GroupPublicAPI
 */
///@{
/** Security Scene Event types */
typedef enum
{
	ZWSCN_SECURITY_SCENE_EVENT_ARM,			/**< ARM events */
	ZWSCN_SECURITY_SCENE_EVENT_DISARM,		/**< DISARM events */
	ZWSCN_SECURITY_SCENE_EVENT_ALARM		/**< ALARM events */
} ZWSCN_SECURITY_SCENE_EVENT_TYPE_T;				

struct zwscn_event;							/**< Forward declaration of zwscene_event structure */
typedef struct zwscn_event zwscn_security_event_t, *zwscn_security_event_p;	/**< Scene Arm/Disarm Event */

/** Security Scene last trigger */
typedef struct {
	uint8_t 	type; 						/**< last alarm trigger reason */
	uint64_t 	time; 						/**< last alarm time */
	zwscn_security_event_p event;			/**< copy of event if the last trigger type was an event otherwise NULL */
} zwscn_security_scene_last_trigger_t, *zwscn_security_scene_last_trigger_p; /**< Security Scene last trigger */

/** Security Scene */
typedef struct zwscn_security_scene {
	/** @name Editable properties */
	//@{
	char	 	name[ZWSCN_LEN_SCENE_NAME];								/**< scene name */
	uint8_t 	active;													/**< scene active flag @note its is scene level enable/disable flag */
	uint8_t		is_armed;												/**< scene Arm/Disarm status */
	uint8_t		is_alarmed;												/**< scene Alarm status */
	uint32_t 	scene_id_at_arm;										/**< ID of normal scene to execute on Arm state */
	uint32_t 	scene_id_at_disarm;										/**< ID of normal scene to execute on Disarm state */
	uint32_t 	scene_id_at_alarm;										/**< ID of normal scene to execute on Alarm state */
	uint8_t		is_notification_on;										/**< flag to check if any notification enabled */
	uint8_t		is_notification_by_sms_on;								/**< flag to check if SMS/Text notification enabled */
	uint8_t		is_notification_by_email_on;							/**< flag to check if email notification enabled */
	char	 	notification_sms_number[ZWSCN_LEN_SMS_NUMBER];			/**< phone number for sending SMS/Text notification */
	char	 	notification_email[ZWSCN_LEN_EMAIL];					/**< address for sending email notification */
	//@}

	/** @name Identifier(s) */
	//@{
	uint32_t 	id;														/**< unique identifier for this scene */
	//@}

	/** @name Read only parameters */
	//@{
	zwscn_security_scene_last_trigger_t lt_arm;							/**< last trigger that caused Arm state */
	zwscn_security_scene_last_trigger_t lt_disarm;						/**< last trigger that caused Disarm state */
	zwscn_security_scene_last_trigger_t	lt_alarm_on;					/**< last trigger that caused Alarm On state */
	zwscn_security_scene_last_trigger_t lt_alarm_off;					/**< last trigger that caused Alarm Off state */
	uint64_t 	utime; 													/**< last update time */
	//@}
} zwscn_security_scene_t, *zwscn_security_scene_p;						/**< Security  Scene */

/** @defgroup GroupSSAdd Security Scenes Add
 *  Functions to add a security scene, arm, disarm or alarm event
 */
///@{
/**
 * @brief Create a new Security Scene without any Events
 * @param[in]	net				Network runtime context
 * @param[in]	scene			New Scene with editable parameters set
 * @param[out]	out_id			Id assigned to the new Security Scene. The memory for this parameter should be pre-allocated by the caller.
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any input arguments after it is copied by this function
 */
ZWSCN_STATUS_T zwscn_add_security_scene(void *net, zwscn_security_scene_p scene, uint32_t *out_id);
/**
 * @brief		Add a new Arm Event to an existing Security Scene
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in] 	event			New Event with editable parameters, id and cmd set
 * @param[out]	out_id			The same id is returned that was part of input parameter event. The memory for this parameter should be pre-allocated by the caller.
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any input arguments after it is copied by this function
 */
ZWSCN_STATUS_T zwscn_add_arm_security_event(void *net, uint32_t scene_id, zwscn_security_event_p event, uint32_t *out_id);
/**
 * @brief		Add a new Disarm Event to an existing Security Scene
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in] 	event			New Event with editable parameters, id and cmd set
 * @param[out]	out_id			The same id is returned that was part of input parameter event. The memory for this parameter should be pre-allocated by the caller.
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any input arguments after it is copied by this function
 */
ZWSCN_STATUS_T zwscn_add_disarm_security_event(void *net, uint32_t scene_id, zwscn_security_event_p event, uint32_t *out_id);
/**
 * @brief		Add a new Alarm Event to an existing Security Scene
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in] 	event			New Event with editable parameters, id and cmd set
 * @param[out]	out_id			The same id is returned that was part of input parameter event. The memory for this parameter should be pre-allocated by the caller.
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any input arguments after it is copied by this function
 */
ZWSCN_STATUS_T zwscn_add_alarm_security_event(void *net, uint32_t scene_id, zwscn_security_event_p event, uint32_t *out_id);
/**
 * @brief	create a new security scene or edit an exiting one
 * @param[in] net					Network runtime context
 * @param[in] scene					New Scene with editable parameters set
 * @param[in] array_arm_events		Array of Arm events
 * @param[in] num_arm_events		Number of Arm events
 * @param[in] array_disarm_events	Array of Disarm events
 * @param[in] num_disarm_events		Number of Disarm events
 * @param[in] array_alarm_events	Array of Alarm events
 * @param[in] num_alarm_events		Number of Alarm events
 * @param[out] out_id				Id assigned to the new Security Scene or id of the exiting Security Scene. The memory for this parameter should be pre-allocated by the caller.
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_save_security_scene(void *net, zwscn_security_scene_p scene,
								zwscn_security_event_t *array_arm_events, int num_arm_events,
								zwscn_security_event_t *array_disarm_events, int num_disarm_events,
								zwscn_security_event_t *array_alarm_events, int num_alarm_events, uint32_t *out_id);
///@}

/** @defgroup GroupSSDelete Security Scenes Delete
 * Functions to delete a security scene, arm, disarm or alarm event
 */
///@{
/**
 * @brief		delete a security scene including all children (its arm, disarm and alarm events etc.)
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_security_scene(void *net, uint32_t scene_id);
/**
 * @brief		delete an Arm event
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in]	event_id		Event id
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_arm_security_event(void *net, uint32_t scene_id, uint32_t event_id);
/**
 * @brief		delete an Disarm event
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in]	event_id		Event id
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_disarm_security_event(void *net, uint32_t scene_id, uint32_t event_id);
/**
 * @brief		delete an Alarm event
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in]	event_id		Event id
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_alarm_security_event(void *net, uint32_t scene_id, uint32_t event_id);
/**
 * @brief		delete all security scenes for a given network
 * @param[in] 	net				Network runtime context
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_all_security_scenes(void *net);
/**
 * @brief		delete all arm, disarm and alarm events for a given scene
 * @param[in] 	net				Network runtime context
 * @param[in]	scene_id		Scene id
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_all_security_events(void *net, uint32_t scene_id);
/**
 * @brief		delete all arm events for a given scene
 * @param[in] 	net				Network runtime context
 * @param[in]	scene_id		Scene id
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_all_arm_security_events(void *net, uint32_t scene_id);
/**
 * @brief		delete all disarm events for a given scene
 * @param[in] 	net				Network runtime context
 * @param[in]	scene_id		Scene id
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_all_disarm_security_events(void *net, uint32_t scene_id);
/**
 * @brief		delete all alarm events for a given scene
 * @param[in] 	net				Network runtime context
 * @param[in]	scene_id		Scene id
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_all_alarm_security_events(void *net, uint32_t scene_id);
///@}

/** @defgroup GroupSSEdit Security Scenes Edit
 *  Functions to edit a security scene, arm, disarm or alarm event
 */
///@{
/**
 * @brief		Edit (editable) security scene information
 * @param[in] 	net				Network runtime context
 * @param[in]	scene_id		Scene id
 * @param[in]	scene			new security scene information (only applies to editable information)
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any input arguments after it is copied by this function
 * @post		Arm/Disarm and Alarm states retain their value. Please use zwscn_security_scene_set_arm_state() or zwscn_security_scene_set_alarm_state() to change state.
 */
ZWSCN_STATUS_T zwscn_edit_security_scene(void *net, uint32_t scene_id, zwscn_security_scene_p scene);
/**
 * @brief		Edit (editable) security scene Arm event information
 * @param[in] 	net				Network runtime context
 * @param[in]	scene_id		Scene id
 * @param[in]	event_id		Event id
 * @param[in]	event		new event information (only applies to editable information)
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any input arguments after it is copied by this function
 */
ZWSCN_STATUS_T zwscn_edit_arm_security_event(void *net, uint32_t scene_id, uint32_t event_id, zwscn_security_event_p event);
/**
 * @brief		Edit (editable) security scene Disarm event information
 * @param[in] 	net				Network runtime context
 * @param[in]	scene_id		Scene id
 * @param[in]	event_id		Event id
 * @param[in]	event		new event information (only applies to editable information)
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any input arguments after it is copied by this function
 */
ZWSCN_STATUS_T zwscn_edit_disarm_security_event(void *net, uint32_t scene_id, uint32_t event_id, zwscn_security_event_p event);
/**
 * @brief		Edit (editable) security scene Alarm event information
 * @param[in] 	net				Network runtime context
 * @param[in]	scene_id		Scene id
 * @param[in]	event_id		Event id
 * @param[in]	event		new event information (only applies to editable information)
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any input arguments after it is copied by this function
 */
ZWSCN_STATUS_T zwscn_edit_alarm_security_event(void *net, uint32_t scene_id, uint32_t event_id, zwscn_security_event_p event);
///@}

/** @defgroup GroupSSGet Security Scenes Get Information
 * Functions to get information about a security scene, arm, disarm or alarm event
 */
///@{
/**
 * @brief		get security scene information
 * @param[in] 	net				Network runtime context
 * @param[in]	scene_id		Scene id
 * @param[out]	out_scene		output structure containing scene information
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any output parameter
 */
ZWSCN_STATUS_T zwscn_get_security_scene(void *net, uint32_t scene_id, zwscn_security_scene_p *out_scene);
/**
 * @brief		get security scene Arm event information
 * @param[in] 	net					Network runtime context
 * @param[in]	scene_id			Scene id
 * @param[in]	event_id			Event id
 * @param[out] 	out_scene_event	output structure containing scene event information
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any output parameter
 */
ZWSCN_STATUS_T zwscn_get_arm_security_event(void *net, uint32_t scene_id, uint32_t event_id, zwscn_security_event_p *out_scene_event);
/**
 * @brief		get security scene Disarm event information
 * @param[in] 	net					Network runtime context
 * @param[in]	scene_id			Scene id
 * @param[in]	event_id			Event id
 * @param[out] 	out_scene_event	output structure containing scene event information
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any output parameter
 */
ZWSCN_STATUS_T zwscn_get_disarm_security_event(void *net, uint32_t scene_id, uint32_t event_id, zwscn_security_event_p *out_scene_event);
/**
 * @brief		get security scene Alarm event information
 * @param[in] 	net					Network runtime context
 * @param[in]	scene_id			Scene id
 * @param[in]	event_id			Event id
 * @param[out] 	out_scene_event	output structure containing scene event information
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any output parameter
 */
ZWSCN_STATUS_T zwscn_get_alarm_security_event(void *net, uint32_t scene_id, uint32_t event_id, zwscn_security_event_p *out_scene_event);
/**
 * @brief		get security scene information by its index (useful for iterating in a loop)
 * @param[in] 	net				Network runtime context
 * @param[in]	index			Index of the scene in the list
 * @param[out] 	out_scene		output structure containing scene information
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any output parameter
 */
ZWSCN_STATUS_T zwscn_get_security_scene_by_index(void *net, uint32_t index, zwscn_security_scene_p *out_scene);
/**
 * @brief		get security scene Arm event information by its index (useful for iterating in a loop)
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in]	index			Index of the event in a scene's event list
 * @param[out] 	out_scene_event	output structure containing scene event information
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any output parameter
 */
ZWSCN_STATUS_T zwscn_get_arm_security_event_by_index(void *net, uint32_t scene_id, uint32_t index, zwscn_security_event_p *out_scene_event);
/**
 * @brief		get security scene Disarm event information by its index (useful for iterating in a loop)
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in]	index			Index of the event in a scene's event list
 * @param[out] 	out_scene_event	output structure containing scene event information
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any output parameter
 */
ZWSCN_STATUS_T zwscn_get_disarm_security_event_by_index(void *net, uint32_t scene_id, uint32_t index, zwscn_security_event_p *out_scene_event);
/**
 * @brief		get security scene Alarm event information by its index (useful for iterating in a loop)
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in]	index			Index of the event in a scene's event list
 * @param[out] 	out_scene_event	output structure containing scene event information
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any output parameter
 */
ZWSCN_STATUS_T zwscn_get_alarm_security_event_by_index(void *net, uint32_t scene_id, uint32_t index, zwscn_security_event_p *out_scene_event);
/**
 * @brief	get default maximum number of security scenes in a network
 * @return		default maximum number of scenes or a negative error code
 */
int zwscn_get_default_max_num_security_scenes();
/**
 * @brief	get default maximum number of Arm events in a security scene
 * @return		default maximum number of Arm events or a negative error code
 */
int zwscn_get_default_max_num_arm_security_events();
/**
 * @brief	get default maximum number of Disarm events in a security scene
 * @return		default maximum number of Disarm events or a negative error code
 */
int zwscn_get_default_max_num_disarm_security_events();
/**
 * @brief	get default maximum number of Alarm events in a security scene
 * @return		default maximum number of Alarm events or a negative error code
 */
int zwscn_get_default_max_num_alarm_security_events();
/**
 * @brief	get maximum number of security scenes for a given network
 * @param[in] 	net				Network runtime context
 * @return		maximum number of scenes or a negative error code
 */
int zwscn_get_max_num_security_scenes(void *net);
/**
 * @brief	get maximum number of Arm events in a security scene
 * @param[in] 	net				Network runtime context
 * @return		maximum number of Arm events or a negative error code
 */
int zwscn_get_max_num_arm_security_events(void *net);
/**
 * @brief	get maximum number of Disarm events in a security scene
 * @param[in] 	net				Network runtime context
 * @return		maximum number of Disarm events or a negative error code
 */
int zwscn_get_max_num_disarm_security_events(void *net);
/**
 * @brief	get maximum number of Alarm events in a security scene
 * @param[in] 	net				Network runtime context
 * @return		maximum number of Alarm events or a negative error code
 */
int zwscn_get_max_num_alarm_security_events(void *net);
/**
 * @brief	get number of security scenes for a given network
 * @param[in] 	net				Network runtime context
 * @return		number of security scenes or a negative error code
 */
int zwscn_count_security_scenes(void *net);
/**
 * @brief	get number of Arm events for a given security scene
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @return		number of Arm events or a negative error code
 */
int zwscn_count_arm_security_events(void *net, uint32_t scene_id);
/**
 * @brief	get number of Disarm events for a given security scene
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @return		number of Disarm events or a negative error code
 */
int zwscn_count_disarm_security_events(void *net, uint32_t scene_id);
/**
 * @brief	get number of Alarm events for a given security scene
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @return		number of Alarm events or a negative error code
 */
int zwscn_count_alarm_security_events(void *net, uint32_t scene_id);
///@}

/** @defgroup GroupSSSetState Security Scenes Set State
 *  Function to set Arm/Disarm/Alarm states
 */
///@{
/**
 * @brief	set Arm/Disarm state of a security scene
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param flag_arm_disarm_state	flag to indicate Arm and Disarm state (1=Arm, 0=Disarm)
 * @return
 */
ZWSCN_STATUS_T zwscn_security_scene_set_arm_state(void *net, uint32_t scene_id, ZWSCN_BOOL_T flag_arm_disarm_state);
/**
 * @brief	set Alarm state of a security scene
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param flag_alarm_state		flag to indicate Alarm On and Off state (1=On, 0=Off)
 * @return
 */
ZWSCN_STATUS_T zwscn_security_scene_set_alarm_state(void *net, uint32_t scene_id, ZWSCN_BOOL_T flag_alarm_state);
///@}

/** @defgroup GroupSSStatelog Security Scenes Statelog
 *  Function to get the current state or the last modified state log.
 */
///@{
/**
 * @brief		get current state and the last state log for scenes
 * @param[in] 	net				Network runtime context
 * @param[in]	category		Main statelog category
 * @param[out]	counter			Statelog change counter value
 * @param[out]	scened			Id of last affected security scene
 * @return		ZWSCN_STATUS_T
 * @post The memory for out parameters should be pre-allocated by the caller.
 */
ZWSCN_STATUS_T zwscn_security_scene_get_statelog(void *net, uint32_t category, uint32_t* counter, uint32_t *scened);
///@}

/** @defgroup GroupSSMisc Security Scenes Miscellaneous
 *  Security Scenes miscellaneous Function
 */
///@{
/**
 * @brief	get the most recent Alarmed security scene
 * @param[in] 	net		Network runtime context
 * @return	Security Scene id
 */
uint32_t zwscn_get_last_alarmed_security_scene(void *net);
/**
 * @brief	copy security scene with its children (deep copy)
 * @param[in] dest_scene_ptr	destination security scene
 * @param[in] source_scene_ptr	source security scene
 * @return ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_security_scene_copy(zwscn_security_scene_p dest_scene_ptr, zwscn_security_scene_p source_scene_ptr);
/**
 * @brief	free security scene and its children
 * @param[in] scene_ptr	scene pointer
 * @return ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_security_scene_free(zwscn_security_scene_p scene_ptr);
///@}
///@}
#endif /* _ZWSCN_SECURITY_SCENE_API_H_ */
