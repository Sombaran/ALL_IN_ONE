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
@file		zwscn_scene.h  
  
                Header for genral functions for Scene core functionality
  
@author		Arif Mohammand
 
@version	1.0 2013-12-05  Initial release
*/
#ifndef _ZWSCN_SCENE_H_
#define _ZWSCN_SCENE_H_

/** @defgroup GroupPrivateScene Scenes Housekeeping and helper functions
 *  Data structures and helper functions for internal use
 *  @ingroup GroupPrivateGeneral
 */
///@{

/** Enumeration for Scene member types */
typedef enum {
	ZWSCN_TYPE_ACTION,					/**< Scene Action */
	ZWSCN_TYPE_SCHEDULE,				/**< Scene Schedule */
	ZWSCN_TYPE_EVENT,					/**< Scene Event */
	ZWSCN_TYPE_ARM_SECURITY_EVENT,		/**< Security Scene Arm Event */
	ZWSCN_TYPE_DISARM_SECURITY_EVENT,	/**< Security Scene Disarm Event */
	ZWSCN_TYPE_ALARM_SECURITY_EVENT		/**< Security Scene Alarm Event */
} ZWSCN_MEMBERS_TYPE_T;					/**< Scene member types */

/** Structure for holding global data */
typedef struct zwscn_global_data {
	int				initialized;						/**< Flag to indicate initialization status */
	int				exiting;							/**< Flag to indicate shutdown is in progress */
	zwscn_hash_t 	network_hash;						/**< Hash table of networks */
	zwscn_mutex_t	network_hash_mutex;					/**< Mutex for protecting network Hash */
	zwscn_hash_t 	unavailable_network_hash;			/**< Hash table of networks that are exiting/unavailable */
	zwscn_mutex_t	unavailable_network_hash_mutex;		/**< Mutex for protecting unavailable network Hash */
	void *			timer_subsystem_data;				/**< Global data for timer subsystem */
} zwscn_global_data_t, *zwscn_global_data_p;			/**< Global data */

/** Structure for storing ZWScnes configuration */
typedef struct zwscn_config {
	//General
	uint16_t		zwave_same_report_ignore_interval;				/**< Same Z-Wave report ignore interval */
	//Security Scene email
	ZWSCN_BOOL_T	security_scene_notification_email_enable;		/**< Security Scenes notification email enable */
	char *			security_scene_notification_email_sender_addr;	/**< Security Scenes notification email sender address */
	//Security Scene SMS
	ZWSCN_BOOL_T	security_scene_notification_sms_enable;			/**< Security Scenes notification SMS/Text enable */
	char *			security_scene_notification_sms_sender_addr;	/**< Security Scenes notification SMS/Text sender address */
	char *			security_scene_notification_sms_gateway;		/**< Security Scenes notification SMS gateway */
	//SMTP
	ZWSCN_BOOL_T	smtp_enable;									/**< Security Scenes enable SMTP */
	ZWSCN_BOOL_T	smtp_auth_enable;								/**< Security Scenes enable SMTP authentication */
	char *			smtp_username;									/**< Security Scenes SMTP username */
	char *			smtp_password;									/**< Security Scenes SMTP password */
	char *			smtp_server_hostname;							/**< Security Scenes SMTP server hotname */
	char *			smtp_server_port;								/**< Security Scenes SMTP server port */
	char *			smtp_secure_method;								/**< Security Scenes SMTP security method */
} zwscn_config_t, *zwscn_config_p;									/**< ZWScnes configuration */

/** Structure for storing initialization data */
typedef struct zwscn_init {
	zwscn_init_input_p 	init_input;										/**< Init input data */
 
	zwscn_hash_t		hash_device_report_cache;						/**< Device Report Cache hash */
	zwscn_hash_t		hash_pending_postset_poll_cb;					/**< Post-Set Poll pending callback hash */
//	zwscn_mutex_t		net_mutex;
	zwscn_rwlock_t		net_rwlock;										/**< Network read write lock @remark Using it instead of net_mutex for better performance */
	zwscn_mutex_t		mqueue_mutex;									/**< Mutex for Message Queues access */
	zwscn_mutex_t		statelog_mutex;									/**< Mutex for Statelog */
	zwscn_mutex_t		mutex_abort_status_update_op;					/**< Mutex for Status Update Abort flag */
	zwscn_mutex_t		mutex_device_report_cache;						/**< Mutex for Device Report Cache */
	zwscn_mutex_t		mutex_pending_report_status;					/**< Mutex for pending Status Update report flag */
	
	zwscn_thread_t 		thread_events;												/**< Thread for Scene Events */
	zwscn_thread_t 		thread_execute_request_handler;								/**< Thread for Scene Execute requests */
	zwscn_thread_t 		thread_status_update_request_handler;						/**< Thread for Status Update requests */
	zwscn_thread_t 		thread_status_update_report_handler;						/**< Thread for Status Update Device Reports */
	zwscn_thread_t 		thread_notification_processor;								/**< Thread for notification processors */
	zwscn_thread_t 		thread_security_events;										/**< Thread for Security Scene Events */
	zwscn_thread_t 		thread_security_scene_notification_request_handler;			/**< Thread for Security Scene Notification requests */
	
	zwscn_mqueue_t 		mqueue_thread_events;										/**< Message Queue for Scene Events */
	zwscn_mqueue_t 		mqueue_thread_execute_request_handler;						/**< Message Queue for Scene Execute requests */
	zwscn_mqueue_t 		mqueue_thread_status_update_request_handler;				/**< Message Queue for Status Update requests */
	zwscn_mqueue_t 		mqueue_thread_status_update_report_handler;					/**< Message Queue for Status Update Device Reports */
	zwscn_mqueue_t 		mqueue_thread_notification_processor;						/**< Message Queue for notification processors */
	zwscn_mqueue_t 		mqueue_thread_security_events;								/**< Message Queue for Security Scene Events (arm/disarm/alarm) */
	zwscn_mqueue_t 		mqueue_thread_security_scene_notification_request_handler;	/**< Message Queue for Security Scene Notification requests */
	
	zwscn_list_t 		list_scenes;									/**< List of Scenes */
	zwscn_list_t 		list_scenes_invalid;							/**< List of invalid Scenes */
	uint32_t 			pool_scene_id; 									/**< Scene id pool. It used for generating new Scene id */
	uint32_t			last_executed_scene_id;							/**< Scene id of the last executed Scene */
	
	zwscn_list_t 		list_security_scenes;							/**< List of Security Scenes */
	zwscn_list_t 		list_security_scenes_invalid;					/**< List of invalid Security Scenes */
	uint32_t 			pool_security_scene_id; 						/**< Security Scene id pool. It used for generating new Scene id */
	uint32_t			last_alarmed_security_scene_id;					/**< Scene id of the last Alarmed Security Scene */
	
	zwscn_statelog_t 	statelog_scene_status_data;						/**< Statelog for Status Update */
	zwscn_statelog_t 	statelog_scene_add_data;						/**< Statelog for Scene Add */
	zwscn_statelog_t 	statelog_scene_delete_data;						/**< Statelog for Scene Delete */
	zwscn_statelog_t 	statelog_scene_exec_data;						/**< Statelog for Scene Execute */
	
	zwscn_statelog_security_scene_t 	statelog_security_scene_add_data;		/**< Statelog for Security Scene Add */
	zwscn_statelog_security_scene_t 	statelog_security_scene_delete_data;	/**< Statelog for Security Scene Delete */
	zwscn_statelog_security_scene_t 	statelog_security_scene_edit_data;		/**< Statelog for Security Scene Edit */
	zwscn_statelog_security_scene_t 	statelog_security_scene_arm_data;		/**< Statelog for Security Scene Arm/Disarm state */
	zwscn_statelog_security_scene_t 	statelog_security_scene_alarm_data;		/**< Statelog for Security Scene Alarm state */
	
	zwscn_config_t	config;												/**< ZWScene config */
	
	ZWSCN_BOOL_T	flag_abort_status_update_op;						/**< Flag for aborting Status Update operation */
	
	uint32_t		pending_report_reqs;								/**< Number of Pending requests */
} zwscn_init_t, *zwscn_init_p;											/**< Initialization data */

/** Scene container */
typedef struct zwscn_scene_container {
	void *net; 										/**< Network runtime context @remark back reference */
	zwscn_scene_p scene;							/**< Scene data */
	zwscn_list_t list_actions;						/**< List of Scene Actions */
	zwscn_list_t list_schedules;					/**< List of Scene Schedules */
	zwscn_list_t list_events;						/**< List of Scene Events  */
	
	zwscn_list_t list_actions_invalid;				/**< List of invalid Actions */
	zwscn_list_t list_events_invalid;				/**< List of invalid Events */
	
	uint32_t		pool_schedule_id; 				/**< Scene Schedule id poll @remark It used for generating new Schedule id */
	zwscn_mutex_t	mutex_status_update_timer_info;	/**< Status update timer info */
	zwscn_hash_t	hash_status_update_timer_info;	/**< Hash for Status Update */
	
	uint32_t		pending_report_reqs;			/**< Number of Pending Device Requests */
} zwscn_scene_cont_t, *zwscn_scene_cont_p;			/**< Scene container */

/** GET operation request */
typedef struct {
	uint32_t					scene_id;			/**< Scene id */
	uint32_t					action_id;			/**< Action id */
	uint8_t						cmd;				/**< Command */
	void *						args;				/**< Device Specific arguments */
	zwscn_device_zwpoll_req_t *	zwscn_zwpoll_ptr;	/**< ZWPoll request buffer */
	zwscn_fn_get_op_t 			func_get_op;		/**< GET operation function */
} zwscn_get_op_request_t, *zwscn_get_op_request_p;	/**< GET operation request */

/** SET operation request */
typedef struct {
	uint32_t					scene_id;			/**< Scene id */
	uint32_t					action_id;			/**< Action id */
	uint8_t						cmd;				/**< Command */
	void *						args;				/**< Device Specific arguments */
	zwscn_fn_set_op_t 			func_set_op;		/**< SET operation function */
	int							result;				/**< SET operation result */
} zwscn_set_op_request_t, *zwscn_set_op_request_p;	/**< SET operation request */

/** Status update timer information */
typedef struct {
	zwscn_time_t 	status_update_req_start_time;	/**< Start time for Status Update request */
	uint32_t		count_reqs;						/**< Number of requests */
	zwscn_timer_t	timer_id;						/**< Timer id */
	void *			timer_user_data;				/**< Timer user data */
} zwscn_status_update_timer_info_t, *zwscn_status_update_timer_info_p;	/**< Status update timer information */

/**
 * @brief Lock global hash Mutex
 */
void zwscn_global_hash_lock();
/**
 * @brief Unlock global hash Mutex
 */
void zwscn_global_hash_unlock();
//void zwscn_network_lock(void *net);
//void zwscn_network_unlock(void *net);
/**
 * @brief Lock unavailable network hash mutex
 */
void zwscn_unavailable_network_hash_lock();
/**
 * @brief Unlock unavailable network hash mutex
 */
void zwscn_unavailable_network_hash_unlock();
/**
 * @brief	Lock network for reading @note Many reading operation can go in parallel
 * @param[in] net Network runtime context
 */
void zwscn_network_lock_reader(void *net);
/**
 * @brief	Unlock network after reading operation is over
 * @param[in] net Network runtime context
 */
void zwscn_network_unlock_reader(void *net);
/**
 * @brief	Lock network for writing @note Only one writing operation can go in parallel
 * @param[in] net Network runtime context
 */
void zwscn_network_lock_writer(void *net);
/**
 * @brief	Unlock network after wrting operation is over
 * @param[in] net Network runtime context
 */
void zwscn_network_unlock_writer(void *net);
/**
 * @brief Lock message queue mutex
 * @param[in] net Network runtime context
 */
void zwscn_mqueue_lock(void *net);
/**
 * @brief Unlock message queue mutex
 * @param[in] net Network runtime context
 */
void zwscn_mqueue_unlock(void *net);
/**
 * @brief	Lock Statelog Mutex
 * @param[in] net Network runtime context
 */
void zwscn_statelog_lock(void *net);
/**
 * @brief	Unlock Statelog Mutex
 * @param[in] net Network runtime context
 */
void zwscn_statelog_unlock(void *net);

/**
 * @brief	Get global data
 * @return zwscn_global_data_p
 */
zwscn_global_data_p	zwscn_get_global_db();
/**
 * @brief	Get network hash
 * @return zwscn_hash_t
 */
zwscn_hash_t 		zwscn_get_network_hash();
/**
 * @brief	Get unavailable network hash
 * @return zwscn_hash_t
 */
zwscn_hash_t		zwscn_get_unavailable_network_hash();
/**
 * @brief	Get init data for a network
 * @param[in] net Network runtime context
 * @return    zwscn_init_p
 */
zwscn_init_p 		zwscn_get_init_data(void *net);
/**
 * @brief	Get Statelog data
 * @param[in] net		Network runtime context
 * @param[in] category	Statelog category (e.g. ZWSCN_STATELOG_CATEGORY_STATUS)
 * @return	  zwscn_statelog_p
 */
zwscn_statelog_p 	zwscn_get_statelog_data(void *net, uint32_t category);
/**
 * @brief	Check if a network is initialized
 * @param[in] net	Network runtime context
 * @return    0 for False and non-zero for True
 */
int 				zwscn_does_net_exist(void *net);
/**
 * @brief	Check if a network is initialized
 * @param[in] net	Network runtime context
 * @return    0 for False and non-zero for True
 * @remark	This function is called by shutdown code only
 */
int					zwscn_does_net_exist_2(void *net);
/**
 * @brief	Generate Scene id
 * @param[in] net	Network runtime context
 * @return    Scene id
 */
uint32_t 			zwscn_generate_scene_id(void *net);
/**
 * @brief	Get Scene id pool index
 * @param[in] net	Network runtime context
 * @return    Scene id pool index
 */
uint32_t			zwscn_get_scene_id_pool_index(void *net);
/**
 * @brief	Set Scene id pool index
 * @param[in] net	Network runtime context
 * @param[in] index	Scene id pool index
 */
void				zwscn_set_scene_id_pool_index(void *net, uint32_t index);
/**
 * @brief	Generate Schedule id
 * @param[in] net		Network runtime context
 * @param[in] scene_id	Scene id
 * @return	  Schedule id
 */
uint32_t 			zwscn_generate_scene_schedule_id(void *net, uint32_t scene_id);
/**
 * @brief	Get a new id for schedule
 * @param[in] scene_container_ptr	Scene container pointer
 * @return Schedule id
 */
uint32_t zwscn_get_new_schedule_id(zwscn_scene_cont_p scene_container_ptr);
/**
 * @brief	Get Scene Events thread
 * @param[in] net			Network runtime context
 * @param[out] thread_ptr 	Thread pointer
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_thread_events(void *net, zwscn_thread_t *thread_ptr);
/**
 * @brief	Get Scene Execute request handler thread
 * @param[in] net			Network runtime context
 * @param[out] thread_ptr 	Thread pointer
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_thread_execute_request_handler(void *net, zwscn_thread_t *thread_ptr);
/**
 * @brief	Get Scene Status Update request handler thread
 * @param[in] net			Network runtime context
 * @param[out] thread_ptr 	Thread pointer
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_thread_status_update_request_handler(void *net, zwscn_thread_t *thread_ptr);
/**
 * @brief	Get Scene Status Update report handler thread
 * @param[in] net			Network runtime context
 * @param[out] thread_ptr 	Thread pointer
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_thread_status_update_report_handler(void *net, zwscn_thread_t *thread_ptr);
/**
 * @brief	Get Notification processor thread
 * @param[in] net			Network runtime context
 * @param[out] thread_ptr 	Thread pointer
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_thread_notification_processor(void *net, zwscn_thread_t *thread_ptr);

/**
 * @brief	Get Scene Events message queue
 * @param[in] net			Network runtime context
 * @param[out] mqueue_ptr	Message queue
 * @return     ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_mqueue_thread_events(void *net, zwscn_mqueue_t *mqueue_ptr);
/**
 * @brief	Get Scene Execute request handler message queue
 * @param[in] net			Network runtime context
 * @param[out] mqueue_ptr	Message queue
 * @return     ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_mqueue_thread_execute_request_handler(void *net, zwscn_mqueue_t *mqueue_ptr);
/**
 * @brief	Get Scene Status Update request handler message queue
 * @param[in] net			Network runtime context
 * @param[out] mqueue_ptr	Message queue
 * @return     ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_mqueue_thread_status_update_request_handler(void *net, zwscn_mqueue_t *mqueue_ptr);
/**
 * @brief	Get Scene Status Update report handler message queue
 * @param[in] net			Network runtime context
 * @param[out] mqueue_ptr	Message queue
 * @return     ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_mqueue_thread_status_update_report_handler(void *net, zwscn_mqueue_t *mqueue_ptr);
/**
 * @brief	Get Notification processor message queue
 * @param[in] net			Network runtime context
 * @param[out] mqueue_ptr	Message queue
 * @return     ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_mqueue_thread_notification_processor(void *net, zwscn_mqueue_t *mqueue_ptr);

/**
 * @brief	Get Scene list for a network
 * @param[in] net				Network runtime context
 * @param[out] scene_list_ptr	Scene list
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_scene_list(void *net, zwscn_list_t **scene_list_ptr);
/**
 * @brief	Get Action list for a Scene
 * @param[in] net				Network runtime context
 * @param[in] scene_id			Scene id
 * @param[out] action_list_ptr	Action list
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_action_list(void *net, uint32_t scene_id, zwscn_list_t **action_list_ptr);
/**
 * @brief	Get Schedule list for a Scene
 * @param[in] net					Network runtime context
 * @param[in] scene_id				Scene id
 * @param[out] schedule_list_ptr	Schedule list
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_schedule_list(void *net, uint32_t scene_id, zwscn_list_t **schedule_list_ptr);
/**
 * @brief	Get Event list for a Scene
 * @param[in] net				Network runtime context
 * @param[in] scene_id			Scene id
 * @param[out] event_list_ptr	Event list
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_event_list(void *net, uint32_t scene_id, zwscn_list_t **event_list_ptr);

/**
 * @brief	Get Scene by id
 * @param[in] net			Network runtime context
 * @param[in] scene_id		Scene id
 * @param[out] scene_ptr 	Scene pointer
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_scene(void *net, uint32_t scene_id, zwscn_scene_cont_p *scene_ptr);
/**
 * @brief	Get Action
 * @param[in] net			Network runtime context
 * @param[in] scene_id		Scene id
 * @param[in] id			Action id
 * @param[out] out_action	Action pointer
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_action(void *net, uint32_t scene_id, uint32_t id, zwscn_action_p *out_action);
/**
 * @brief	Get Schedule
 * @param[in] net			Network runtime context
 * @param[in] scene_id		Scene id
 * @param[in] id			Schedule id
 * @param[out] out_schedule	Schedule pointer
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_schedule(void *net, uint32_t scene_id, uint32_t id, zwscn_schedule_p *out_schedule);
/**
 * @brief	Get Event
 * @param[in] net			Network runtime context
 * @param[in] scene_id		Scene id
 * @param[in] id			Event id
 * @param[out] out_event	Event pointer
 * @return	   ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_find_event(void *net, uint32_t scene_id, uint32_t id, zwscn_event_p *out_event);

/**
 * @brief	Get Scene id for last executed Scene
 * @param[in] net	Network runtime context
 * @return	  Scene id
 */
uint32_t	 zwscn_find_last_executed_scene(void *net);
/**
 * @brief	Check if it is last executed Scene
 * @param[in] net		Network runtime context
 * @param[in] scene_ptr	Scene pointer
 * @return	  ZWSCN_BOOL_T
 */
ZWSCN_BOOL_T zwscn_is_last_executed_scene(void *net, zwscn_scene_cont_p scene_ptr);
/**
 * @brief	Get maximum number of Scenes
 * @param[in] net	Network runtime context
 * @return Maximum number of Scenes
 */
int zwscn_get_max_scenes(void *net);
/**
 * @brief	Get maximum number of Actions
 * @param[in] net	Network runtime context
 * @return Maximum number of Actions
 */
int zwscn_get_max_actions(void *net);
/**
 * @brief	Get maximum number of Schedules
 * @param[in] net	Network runtime context
 * @return Maximum number of Schedules
 */
int zwscn_get_max_schedules(void *net);
/**
 * @brief	Get maximum number of Events
 * @param[in] net	Network runtime context
 * @return Maximum number of Events
 */
int zwscn_get_max_events(void *net);
/**
 * @brief	Check if Scene list is full
 * @param[in] net			Network runtime context
 * @param[in] scene_list	Scene list
 * @return	  ZWSCN_BOOL_T
 */
ZWSCN_BOOL_T zwscn_is_list_full_scene(void *net, zwscn_list_t scene_list);
/**
 * @brief	Check if Action list is full
 * @param[in] net			Network runtime context
 * @param[in] action_list	Action list
 * @return	  ZWSCN_BOOL_T
 */
ZWSCN_BOOL_T zwscn_is_list_full_action(void *net, zwscn_list_t action_list);
/**
 * @brief	Check if Schedule list is full
 * @param[in] net			Network runtime context
 * @param[in] schedule_list	Schedule list
 * @return	  ZWSCN_BOOL_T
 */
ZWSCN_BOOL_T zwscn_is_list_full_schedule(void *net, zwscn_list_t schedule_list);
/**
 * @brief	Check if Event list is full
 * @param[in] net			Network runtime context
 * @param[in] event_list	Event list
 * @return	  ZWSCN_BOOL_T
 */
ZWSCN_BOOL_T zwscn_is_list_full_event(void *net, zwscn_list_t event_list);

/**
 * @brief	Update triggers information for a Scene
 * @param[in] net			Network runtime context
 * @param[in,out] scene_ptr	Scene to be updated
 * @return		  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_scene_triggers_info_update(void *net, zwscn_scene_cont_p scene_ptr);
///@}

#endif // _ZWSCN_SCENE_H_
