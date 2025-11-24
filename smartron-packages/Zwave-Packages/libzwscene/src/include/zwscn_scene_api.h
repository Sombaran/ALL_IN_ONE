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
@file   	zwscn_scene_api.h - Scenes API

			Scenes API public definitions.

@author 	Arif

@version    3.2.0 2014-08-20

@ingroup GroupPublicAPI
*/
#ifndef _ZWSCN_API_H_
#define _ZWSCN_API_H_

#include "stdint.h"

/** @addtogroup GroupPublicAPI
 */
///@{

#define ZWSCN_LEN_CONTEXT_PERSISTENT	25										/**< Maximum length of persistent context string */
#define ZWSCN_LEN_CLIENT_TIMEZONE		256										/**< Maximum length of client timezone string */
#define ZWSCN_LEN_SCENE_NAME_CHARS		50										/**< Maximum length of Scene name */
#define ZWSCN_LEN_SCENE_NAME_UNICODE_MAX	(ZWSCN_LEN_SCENE_NAME_CHARS * 6 + 2) /**< Max length for unicode name
																				 * @note  Worst case: Four bytes of UTF-8 may translate to two bytes of UTF-16. So, multiply by 2
																				 * Also, each UTF-8 byte is URI encoded to equivalent three characters of the form %NN. So, multiply again by 3.
																				 */
#define ZWSCN_LEN_SCENE_NAME				ZWSCN_LEN_SCENE_NAME_UNICODE_MAX	/**< Maximum length of scene name string */
#define ZWSCN_LEN_PERSISTENT_DATA_PATH		512									/**< Maximum length of persistent data storage path string */
#define ZWSCN_LEN_SMS_NUMBER_CHARS			32									/**< Maximum length of SMS number */
#define ZWSCN_LEN_SMS_NUMBER_UNICODE_MAX	(ZWSCN_LEN_SMS_NUMBER_CHARS *6 + 2)	/**< Max length for unicode SMS number
																				 * @note  Worst case: Four bytes of UTF-8 may translate to two bytes of UTF-16. So, multiply by 2
																				 * Also, each UTF-8 byte is URI encoded to equivalent three characters of the form %NN. So, multiply again by 3.
																				 */
#define ZWSCN_LEN_SMS_NUMBER				ZWSCN_LEN_SMS_NUMBER_UNICODE_MAX	/**< Maximum length of SMS number string */
#define ZWSCN_LEN_EMAIL_CHARS				128									/**< Maximum length of EMAIL */
#define ZWSCN_LEN_EMAIL_UNICODE_MAX			(ZWSCN_LEN_EMAIL_CHARS * 6 + 2)		/**< Max length for unicode EMAIL string
																				 * @note  Worst case: Four bytes of UTF-8 may translate to two bytes of UTF-16. So, multiply by 2
																				 * Also, each UTF-8 byte is URI encoded to equivalent three characters of the form %NN. So, multiply again by 3.
																				 */
#define ZWSCN_LEN_EMAIL						ZWSCN_LEN_EMAIL_UNICODE_MAX			/**< Maximum length of EMAIL string */

/** @defgroup GroupDefaultLimits Default Limits
 * 	Default Limits for Scenes, Actions, Schedules and Events
 * @note The following default limits for maximum number of Scenes, Actions, Schedules and Events
 * are chosen by the Management for limiting the QA effort.
 * Technically these numbers are only limited by the system memory. Different non-default limits could be specified at init time.
 */
///@{
#define ZWSCN_MAX_SCENES				20 	/**< Maximum number of Scenes */
#define ZWSCN_MAX_ACTIONS				10	/**< Maximum number of Actions per Scene */
#define ZWSCN_MAX_SCHEDULES				1	/**< Maximum number of Schedules per Scene */
#define ZWSCN_MAX_EVENTS				2	/**< Maximum number of Events per Scene */

#define ZWSCN_MAX_SECURITY_SCENES			2 	/**< Maximum number of Security Scenes */
#define ZWSCN_MAX_ARM_SECURITY_EVENTS		10	/**< Maximum number of Arm Events per Security Scene */
#define ZWSCN_MAX_DISARM_SECURITY_EVENTS	10	/**< Maximum number of Disarm Events per Security Scene */
#define ZWSCN_MAX_ALARM_SECURITY_EVENTS		16	/**< Maximum number of Disarm Events per Security Scene */
///@}

#define ZWSCN_SCENE_TRIGGER_MANUAL		0	/**< Manual (user initiated) trigger */
#define ZWSCN_SCENE_TRIGGER_SCHEDULE	1	/**< Schedule trigger */
#define ZWSCN_SCENE_TRIGGER_EVENT		2	/**< Event trigger */

#define ZWSCN_STATUS_INACTIVE			0 	/**< Inactive Scene Status value @remark same as ZWSCN_FALSE */
#define ZWSCN_STATUS_ACTIVE				1	/**< Active Scene Status value @remark same as ZWSCN_TRUE */
#define ZWSCN_STATUS_UNKNOWN			2	/**< Scene Unknown Status value */

#define ZWSCN_SCHEDULE_NORMAL		1		/**< Schedule type for Normal Schedule */
#define ZWSCN_SCHEDULE_DISABLED		2		/**< Schedule type for Disabled Schedule */
#define ZWSCN_EVENT_ZWAVE			1		/**< Schedule type for Normal Z-Wave Event */
#define ZWSCN_EVENT_DISABLED		2		/**< Schedule type for Disabled Z-Wave Event */


/** @defgroup GroupStatelogDefs Statelog definitions
 *  Definitions for statelog API
 */
///@{
#define ZWSCN_STATELOG_CATEGORY_STATUS				1		/**< Statelog category for Status Update */
#define ZWSCN_STATELOG_CATEGORY_EXEC				2		/**< Statelog category for Execute operation */
#define ZWSCN_STATELOG_CATEGORY_ADD					4		/**< Statelog category for Add operation */
#define ZWSCN_STATELOG_CATEGORY_DELETE				8		/**< Statelog category for Delete operation */

#define ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_ADD		(1 << 8)		/**< Statelog category for Security Scene Add operation */
#define ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_DELETE	(2 << 8)		/**< Statelog category for Security Scene Delete operation */
#define ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_EDIT		(4 << 8)		/**< Statelog category for Security Scene Edit operation */
#define ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_ARM		(8 << 8)		/**< Statelog category for Security Scene Arm/Disarm state operation */
#define ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_ALARM	(16 << 8)		/**< Statelog category for Security Scene Alarm state operation */
#define ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_MIN		ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_ADD		/**< Statelog category for Security Scene Minimum value */

#define ZWSCN_STATELOG_SUBCATEGORY_CURR_OP			1		/**< Statelog subcategory for Current Operation */
#define ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_SCENED	2		/**< Statelog subcategory for Current Operation Scene Id*/
#define ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_TRIGGER	3		/**< Statelog subcategory for Current Operation Trigger */
#define ZWSCN_STATELOG_SUBCATEGORY_LAST_OP			4		/**< Statelog subcategory for Last Operation */
#define ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_SCENED	5		/**< Statelog subcategory for Last Operation Scene Id*/
#define ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_TRIGGER	6		/**< Statelog subcategory for Last Operation Trigger */
#define ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_TIME		7		/**< Statelog subcategory for Last Operation Time */

/** Values for SUBCATEGORY_CURR_OP and SUBCATEGORY_LAST_OP **/
#define ZWSCN_STATELOG_OP_NONE							0	/**< Statelog operation type None */
#define ZWSCN_STATELOG_OP_SCENE_STATUS_UPDATE_ALL		1	/**< Statelog operation type Status Update All */
#define ZWSCN_STATELOG_OP_SCENE_STATUS_UPDATE_SINGLE	2	/**< Statelog operation type Status Update a Single Scene */
#define ZWSCN_STATELOG_OP_SCENE_ADD						3	/**< Statelog operation type Add */
#define ZWSCN_STATELOG_OP_SCENE_DELETE					4	/**< Statelog operation type Delete */
#define ZWSCN_STATELOG_OP_SCENE_EXEC					5	/**< Statelog operation type Execute */
#define ZWSCN_STATELOG_OP_SCENE_ADD_MODIFY				6	/**< Statelog operation type Add Modify */
#define ZWSCN_STATELOG_OP_SCENE_DELETE_MODIFY			7	/**< Statelog operation type Delete Modify */
#define ZWSCN_STATELOG_OP_SCENE_STATUS_UPDATE_PENDING	8	/**< Statelog operation type Status Update Pending */

/** Values for SUBCATEGORY_CURR_OP_TRIGGER and SUBCATEGORY_LAST_OP_TRIGGER **/
#define ZWSCN_STATELOG_TRIGGER_NONE			0				/**< Statelog trigger type None */
#define ZWSCN_STATELOG_TRIGGER_MANUAL		1				/**< Statelog trigger type Manual */
#define ZWSCN_STATELOG_TRIGGER_SCHEDULE		2				/**< Statelog trigger type Schedule */
#define ZWSCN_STATELOG_TRIGGER_EVENT		3				/**< Statelog trigger type Event */
#define ZWSCN_STATELOG_TRIGGER_SYSTEM		4				/**< Statelog trigger type System */
///@}

typedef uint64_t zwscn_time_t;	/**< Type definition for time values */

/** General boolean definition */
typedef enum
{
	ZWSCN_FALSE = 0,
	ZWSCN_TRUE = 1,
} ZWSCN_BOOL_T;															/**< General boolean defintion */

/** Type flag passed to is_valid() callback */
typedef enum
{
	ZWSCN_CHECK_NODE,
	ZWSCN_CHECK_NODE_EP,
	ZWSCN_CHECK_NODE_EP_CLS
} ZWSCN_TYPE_VALIDITY_CHECK_T;											/**< Type flag passed to is_valid() callback */

/** Status returned by Node status notify callback */
typedef enum
{
	ZWSCN_NODE_ADDED,
	ZWSCN_NODE_REMOVED,
	ZWSCN_NODE_UPDATED,
    ZWSCN_NODE_STATUS_ALIVE,                                            /**< Node status has changed to "alive" */
    ZWSCN_NODE_STATUS_DOWN                                              /**< Node status has changed to "down" */
} ZWSCN_NODE_NOTIFY_MODE_T;												/**< Status returned by Node status notify callback */

/** Return value codes @remark more to be added */
typedef enum
{
	ZWSCN_ERR_NONE					= 0,
	ZWSCN_ERR_GENERIC				=-1,
	ZWSCN_ERR_INVALID_ARGS			=-2,
	ZWSCN_ERR_INVALID_NETWORK		=-3,
	ZWSCN_ERR_INVALID_DEVICE		=-4,
	ZWSCN_ERR_INVALID_DATA			=-5,
	ZWSCN_ERR_MEMORY				=-6,
	ZWSCN_ERR_MEMORY_INIT_RESPONSE	=-7,
	ZWSCN_ERR_INIT					=-8,
	ZWSCN_ERR_TIMEZONE				=-9,
	ZWSCN_ERR_LIST_FULL				=-10,
	ZWSCN_ERR_QUEUE_FULL			=-11,
	ZWSCN_ERR_ITEM_IGNORED			=-12,
	ZWSCN_ERR_CALLBACK_NULL			=-13,
	ZWSCN_ERR_CALLBACK_RESULT		=-14,
	ZWSCN_ERR_FILE					=-15,
	ZWSCN_ERR_FILE_OPEN				=-16,
	ZWSCN_ERR_PERSISTENT_DATA		=-17,
	ZWSCN_ERR_ABORT					=-18,
	ZWSCN_ERR_ACTION_EXISTS			=-19,
	ZWSCN_ERR_EVENT_EXISTS			=-20,
	ZWSCN_ERR_NOT_FOUND				=-21
} ZWSCN_STATUS_T;													/**< Return value codes @remark more to be added */


/**
@brief callback on polling completion
@param[in]	net	        Network runtime context
@param[in]	handle	    Handle of the polling request
@param[in]	token		user defined token to facilitate deletion of multiple polling requests
*/
typedef void (*zwscn_device_zwpoll_cmplt_fn)(void *net, uint16_t handle, uint32_t token);

/**
* @brief callback on Post-Set polling completion
* @param[in]	net	        Network runtime context
* @param[in] 	node_id		Node id
* @param[in] 	ep_id		EP id
* @param[in] 	cls_id		Command Class
* @param[in] 	reason		Reason of the callback (Target reached, timeout or error etc.)
*/
typedef void (*zwscn_fn_postset_poll_cb_t)(void *net, uint8_t node_id, uint8_t ep_id, uint16_t cls_id, int reason);

/** Polling request */
typedef struct
{   /** @name Input to zwpoll */
	//@{
    uint32_t                usr_token;      	    /**< User defined token to facilitate deletion of multiple polling requests */
    uint16_t                interval;       	    /**< Polling interval in seconds; zero = the smallest possible interval*/
    uint16_t                poll_cnt;       		/**< Number of times to poll; zero = unlimited times (i.e. repetitive polling)*/
    zwscn_device_zwpoll_cmplt_fn    cmplt_cb;       /**< Polling completion callback. NULL if callback is not required*/
	void                    *usr_param;     		/**< User parameter of polling completion callback */
    //@}

    /** @name Output of zwpoll */
    //@{
    uint16_t                handle;         							/**< Handle if the request is accepted into the polling queue. The handle
                                                 	 	 	 	 	 	 can be used to facilitate deletion of the polling request */
    //@}
} zwscn_device_zwpoll_req_t;

/** @defgroup GroupInputFunctionsPointers Input Function Pointers
 *  Function pointers those are passed to Scenes library at the initialization time
 */
///@{
/**
 * @brief		Function to perform the Setup (registering and enabling report callbacks)
 * @param[in] 	net		Network runtime context
 * @param[in] 	node_id	Node id
 * @param[in] 	ep_id	EP id
 * @param[in] 	cls_id	Command Class
 * @param[in] 	cmd		Command (The original value that was part of action/event at its creation time)
 * @return		0 if success; else negative value on error
 */
typedef int (*zwscn_fn_setup_op_t)(void *net, uint8_t node_id, uint8_t ep_id, uint16_t cls_id, uint8_t cmd);
/**
 * @brief		Function to perform the Set operation on a device
 * @param[in] 	net		Network runtime context
 * @param[in] 	node_id	Node id
 * @param[in] 	ep_id	EP id
 * @param[in] 	cls_id	Command Class
 * @param[in] 	cmd		Command (The original value that was part of action/event at its creation time)
 * @param[in] 	args	device specific arguments - see zwscn_device_args.h 
 * @return		0 if success; else negative value on error
 * @post		This function is responsible for freeing 'args'.
 */
typedef int (*zwscn_fn_set_op_t)(void *net, uint8_t node_id, uint8_t ep_id, uint16_t cls_id, uint8_t cmd, void *args, zwscn_fn_postset_poll_cb_t postset_poll_cb);
/**
 * @brief		Function to perform the Get operation on a device
 * @param[in] 	net			Network runtime context
 * @param[in] 	node_id		Node id
 * @param[in] 	ep_id		EP id
 * @param[in] 	cls_id		Command Class
 * @param[in] 	cmd			Command (The original value that was part of action/event at its creation time)
 * @param[in] 	args		device specific arguments - see zwscn_device_args.h
 * @param[in] 	poll_req	Poll request structure if a zwpoll is to be started. Otherwise it is NULL.
 * @return		0 if success; else negative value on error
 * @post		This function is responsible for freeing 'args'. This function must NOT free 'poll_req' as it is used for returning handle value.
 */
typedef int (*zwscn_fn_get_op_t)(void *net, uint8_t node_id, uint8_t ep_id, uint16_t cls_id, uint8_t cmd, void *args, zwscn_device_zwpoll_req_t *poll_req);
/**
 * @brief		Function to get Name and Location of a device
 * @param[in] 	net			Network runtime context
 * @param[in] 	node_id		Node id
 * @param[in] 	ep_id		EP id
 * @param[in] 	cls_id		Command Class
 * @param[out] 	name_buf	Name buffer
 * @param[in] 	name_buf_size	Name buffer size
 * @param[out] 	loc_buf			Location buffer
 * @param[in] 	loc_buf_size	Location buffer size
 * @return		0 if success; else negative value on error
 */
typedef int (*zwscn_fn_get_nameloc_t)(void *net, uint8_t node_id, uint8_t ep_id, uint16_t cls_id, char *name_buf, size_t name_buf_size, char *loc_buf, size_t loc_buf_size);
/**
* @brief Function to remove a polling request
* @param[in]	net	        Network runtime context
* @param[in]	handle	    handle of the polling request to remove
* @return		0 if success; else negative value on error
*/
typedef int (*zwscn_fn_zwpoll_rm_t)(void *net, uint16_t handle);
/**
* @brief Function to remove multiple polling requests
* @param[in]	net	        Network runtime context
* @param[in]	usr_token	usr_token of the polling requests to remove
* @return		0 if success; else negative value on error
*/
typedef int (*zwscn_fn_zwpoll_rm_mul_t)(void *net, uint32_t usr_token);
/**
* @brief Function to check validity of node, ep and interface (cls)
* @param[in]	net	        Network runtime context
* @param[in]	flag_type	flag to specifiy if check is to be done for node, node and ep or node, ep and cls combination
* @param[in] 	node_id		Node id
* @param[in] 	ep_id		EP id
* @param[in] 	cls_id		Command Class
* @return		0 if success; else negative value on error
*/
typedef ZWSCN_BOOL_T (*zwscn_fn_is_valid_t)(void *net, ZWSCN_TYPE_VALIDITY_CHECK_T flag_type, uint8_t node_id, uint8_t ep_id, uint16_t cls_id);
///@}

/** @defgroup GroupOutputFunctionsPointers Output Function Pointers
 *  Function pointers returned by Scenes library at the initialization time
 */
///@{
/**
 * @brief Function to handle status update notification for network
 * @param[in]	net	    	Network runtime context
 * @param[in]	op			network operation ZWNET_OP_XXX (As defined in HCAPI)
 * @param[in]	status		status of current operation
 * @param[in]	buf			op specific input data
 * @param[in]	buf_size	op specific input data size
 * @post		Caller is responsible for freeing any input arguments (after it is copied by this function)
 */
typedef void (*zwscn_fn_network_notify_t)(void *net, uint8_t op, uint16_t status, void *buf, uint32_t buf_size);
/**
 * @brief Function to handle status update notification for nodes
 * @param[in]	net	    Network runtime context
 * @param[in]	node_id	Node id
 * @param[in]	mode	ZWSCN_NODE_NOTIFY_MODE
 */
typedef void (*zwscn_fn_node_notify_t)(void *net, uint8_t node_id, ZWSCN_NODE_NOTIFY_MODE_T mode);
/**
 * @brief		Function to handle device reports
 * @param[in] 	net			Network runtime context
 * @param[in] 	node_id		Node id
 * @param[in] 	ep_id		EP id
 * @param[in] 	cls_id		Command Class
 * @param[in] 	cmd			Command (The original value that was part of action/event at its creation time)
 * @param[in]	report		device specific reports - see zwscn_device_reports.h
 * @param[in]	report_size	size of device specific report
 * @return
 * @post		Caller is responsible for freeing any input arguments (after it is copied by this function)
 */
typedef int (*zwscn_fn_report_notify_t)(void *net, uint8_t node_id, uint8_t ep_id, uint16_t cls_id, uint8_t cmd, void *report, uint32_t report_size);
///@}

/** Scene initialization input */
typedef struct zwscn_init_input {
	void *	net;														/**< runtime context e.g. zwnet pointer used by HCAPI */
	char 	context_persistent[ZWSCN_LEN_CONTEXT_PERSISTENT]; 			/**< persistent context e.g. RAC (16) + home id (8) as in Hex string format */
	char 	client_timezone[ZWSCN_LEN_CLIENT_TIMEZONE]; 				/**< as used for setting TZ env variable */
	char 	persistent_data_path[ZWSCN_LEN_PERSISTENT_DATA_PATH];		/**< PATH of persistent data storage location */
	int num_max_scenes;													/**< maximum number of scenes allowed. Set to -1 to use system default */
	int num_max_actions_per_scene;										/**< maximum number of actions allowed in a scene. Set to -1 to use system default */
	int num_max_schedules_per_scene;									/**< maximum number of schedules allowed in a scene. Set to -1 to use system default */
	int num_max_events_per_scene;										/**< maximum number of events allowed in a scene. Set to -1 to use system default */
	int num_max_security_scenes;										/**< maximum number of security scenes allowed. Set to -1 to use system default */
	int num_max_arm_security_events_per_scene;							/**< maximum number of Arm events allowed in a security scene. Set to -1 to use system default */
	int num_max_disarm_security_events_per_scene;						/**< maximum number of Disarm events allowed in a security scene. Set to -1 to use system default */
	int num_max_alarm_security_events_per_scene;						/**< maximum number of Alarm events allowed in a security scene. Set to -1 to use system default */
	
	/** @name Input Function Pointers */
	//@{
	zwscn_fn_setup_op_t			zwscn_function_setup_op_handler; 		/**< Function pointer to handle SETUP operations - This is optional if the application performs auto-setup. In such a case this input can be NULL */
	zwscn_fn_set_op_t			zwscn_function_set_op_handler; 			/**< Function pointer to handle SET operations */
	zwscn_fn_get_op_t			zwscn_function_get_op_handler;			/**< Function pointer to handle GET operations */
	zwscn_fn_get_nameloc_t		zwscn_function_get_nameloc_handler;		/**< Function pointer to Name and Location of a device */
	zwscn_fn_zwpoll_rm_t 		zwscn_function_zwpoll_rm_handler;		/**< Function pointer to remove zwpoll */
	zwscn_fn_zwpoll_rm_mul_t 	zwscn_function_zwpoll_rm_mul_handler;	/**< Function pointer to remove multiple zwpolls */
	zwscn_fn_is_valid_t			zwscn_fn_is_valid_handler;				/**< Function pointer to check validity of node, ep and cls */
	
	//@}
} zwscn_init_input_t, *zwscn_init_input_p;								/**< Scene initialization input */

/** Scene initialization output */
typedef struct zwscn_init_response {
	/** @name Output Function Pointers */
	//@{
	zwscn_fn_network_notify_t	zwscn_function_network_notify_handler;	/**< Function pointer to handle status update notification for network */
	zwscn_fn_node_notify_t		zwscn_function_node_notify_handler;		/**< Function pointer to handle status update notification for nodes */
	zwscn_fn_report_notify_t	zwscn_function_report_notify_handler;	/**< Function pointer to handle device reports */
	//@}
}zwscn_init_response_t, *zwscn_init_response_p;							/**< Scene initialization output */

/** Scene */
typedef struct zwscn_scene {
	/** @name Editable properties */
	//@{
	char		name[ZWSCN_LEN_SCENE_NAME];								/**< scene name */
	uint8_t 	active;													/**< scene active flag */
	//@}

	/** @name Identifier(s) */
	//@{
	uint32_t 	id;														/**< unique identifier for this scene */
	//@}

	/** @name Read only parameters */
	//@{
	uint8_t 	triggers;												/**< triggers in this scene. (manual=0 | schedule=1 | event=2) */
	uint8_t 	status;													/**< current status */
	uint64_t 	sutime;													/**< current status update time */
	uint8_t 	lkstatus; 												/**< last known status */
	uint64_t 	lksutime; 												/**< last known status update time */
	uint8_t 	letrigger; 												/**< last execution trigger */
	uint64_t 	letime; 												/**< last execution time */
	uint8_t		flag_updating;											/**< boolean value to show if a scene is updating or changing */
	//@}
} zwscn_scene_t, *zwscn_scene_p;										/**< Scene */

/** Scene Action */
typedef struct zwscn_action {
	/** @name Editable properties */
	//@{
	uint8_t		type;													/**< type of Action */
	void * 		device_args;											/**< device specific parameters - see zwscn_device_args.h */
	uint32_t	device_args_size;										/**< device specific parameters size */
	//@}

	/** @name Identifier(s) */
	//@{
	uint32_t	id;														/**< Combo id based on Node ID (uint8_t) | Endpoint ID (uint8_t) | Command class (uint16_t) */
	uint8_t 	cmd;													/**< Command e.g. COMMAND_DOOR_LOCK_OPERATION_SET - used for identification in addition to node, ep and cls */
	//@}

	/** @name Read only parameters */
	//@{
	uint8_t 	status;													/**< current status */
	uint64_t 	sutime;													/**< current status update time */
	uint8_t 	lkstatus; 												/**< last known status */
	uint64_t 	lksutime; 												/**< last known status update time */
	uint32_t	pending_report_reqs;									/**< number of normal pending reports */
	uint32_t	pending_zwpoll_reqs;									/**< number of ZWPoll pending reports */
	//@}
} zwscn_action_t, *zwscn_action_p;										/**< Scene Action */

/** Scene Schedule */
typedef struct zwscn_schedule {
	/** @name Editable properties */
	//@{
	uint8_t		type;													/**< type of Schedule - Also used to enable/disable a Schedule */
	uint8_t		day;													/**< day of a week (sunday=0 and saturday=6) bit-mask. "Everyday" means bit 0 to 6 all set. */
	uint8_t		hour;													/**< hour 0-23 */
	uint8_t		minute;													/**< minute 0-59 */
	//@}

	/** @name Identifier(s) */
	//@{
	uint32_t 	id;														/**< unique identifier for this Schedule */
	//@}

	/** @name Read only parameters */
	//@{
	uint64_t	lctime;													/**< time when last check was performed for this Schedule */
	uint32_t	timer_id;												/**< Private data - used by the library only */
	void *		timer_user_data;										/**< Private data - used by the library only */
	//@}
} zwscn_schedule_t, *zwscn_schedule_p;									/**< Scene Schedule */

/** Scene Event */
typedef struct zwscn_event {
	/** @name Editable properties */
	//@{
	uint8_t		type;													/**< type of Event - Also used to enable/disable an Event*/
	void * 		device_args;											/**< device specific parameters - see zwscn_device_args.h */
	uint32_t	device_args_size;										/**< device specific parameters size */
	//@}

	/** @name Identifier(s) */
	//@{
	uint32_t	id;														/**< Combo id based on Node ID (uint8_t) | Endpoint ID (uint8_t) | Command class (uint16_t) */
	uint8_t 	cmd;													/**< Command - used for identification in addition to node, ep and cls */
	//@}

	/** @name Read only parameters */
	//@{
	uint8_t 	status;													/**< current status */
	uint64_t	lrtime;													/**< last report time  */
	//@}
} zwscn_event_t, *zwscn_event_p;										/**< Scene Event */

/** @defgroup GroupInit Initialization and Shutdown
 *  Functions to initialize or shutdown Scenes library
 */
///@{
/**
 * @brief		Scenes global initialization
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_init();
/**
 * @brief		Scenes initialization for a given network
 * @param[in] 	scene_init_args			Input arguments for Scenes initialization
 * @param[out] 	out_scene_init_response	Output response for Scenes initialization
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any input arguments (after it is copied by this function) and output arguments
 */
ZWSCN_STATUS_T zwscn_init_network(zwscn_init_input_p scene_init_args, zwscn_init_response_p *out_scene_init_response);
/**
 * @brief		Scenes exit for a given network
 * @param[in]	net	Network runtime context
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_exit_network(void *net);
/**
 * @brief		Scenes exit for all networks
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_shutdown();
///@}

/** @defgroup GroupAdd Scenes Add
 *  Functions to add a scene, action, schedule or event
 */
///@{
/**
 * @brief		Create a new Scene without Actions, Schedules or Events
 * @param[in]	net				Network runtime context
 * @param[in]	scene			New Scene with editable parameters set
 * @param[out]	out_id			Id assigned to the new Scene. The memory for this parameter should be pre-allocated by the caller.
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any input arguments after it is copied by this function
 */
ZWSCN_STATUS_T zwscn_add_scene(void *net, zwscn_scene_p scene, uint32_t *out_id);
/**
 * @brief		Add a new Action to an existing scene
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in] 	action			New Action with editable parameters, id and cmd set
 * @param[out]	out_id			The same id is returned that was part of input parameter action. The memory for this parameter should be pre-allocated by the caller.
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any input arguments after it is copied by this function
 */
ZWSCN_STATUS_T zwscn_add_action(void *net, uint32_t scene_id, zwscn_action_p action, uint32_t *out_id);
/**
 * @brief		Add a new Schedule to an existing scene
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in] 	schedule		New Schedule with editable parameters set
 * @param[out]	out_id			Id assigned to the new Schedule. The memory for this parameter should be pre-allocated by the caller.
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any input arguments after it is copied by this function
 */
ZWSCN_STATUS_T zwscn_add_schedule(void *net, uint32_t scene_id, zwscn_schedule_p schedule, uint32_t *out_id);
/**
 * @brief		Add a new Event to an existing scene
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in] 	event			New Event with editable parameters, id and cmd set
 * @param[out]	out_id			The same id is returned that was part of input parameter event. The memory for this parameter should be pre-allocated by the caller.
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any input arguments after it is copied by this function
 */
ZWSCN_STATUS_T zwscn_add_event(void *net, uint32_t scene_id, zwscn_event_p event, uint32_t *out_id);
/**
 * @brief create a new scene or edit an exiting one
 * @param[in] net				Network runtime context
 * @param[in] scene				New Scene with editable parameters set
 * @param[in] array_actions		Array of Actions
 * @param[in] num_actions		Number of Actions
 * @param[in] array_schedules	Array of Schedules
 * @param[in] num_schedules		Number of Schedules
 * @param[in] array_events		Array of Events
 * @param[in] num_events		Number of Events
 * @param[out] out_id			Id assigned to the new Scene or id of the exiting Scene. The memory for this parameter should be pre-allocated by the caller.
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_save_scene(void *net, zwscn_scene_p scene,
								zwscn_action_t *array_actions, int num_actions,
								zwscn_schedule_t *array_schedules, int num_schedules,
								zwscn_event_t *array_events, int num_events, uint32_t *out_id);
///@}

/** @defgroup GroupSDelete Scenes Delete
 * Functions to delete a scene, action, schedule or event
 */
///@{
/**
 * @brief		delete a scene including all children (its actions, schedules and events etc.)
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_scene(void *net, uint32_t scene_id);
/**
 * @brief		delete a scene action
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in]	action_id		Action id
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_action(void *net, uint32_t scene_id, uint32_t action_id);
/**
 * @brief		delete a scene schedule
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in]	schedule_id		Schedule id
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_schedule(void *net, uint32_t scene_id, uint32_t schedule_id);
/**
 * @brief		delete a scene event
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in]	event_id		Event id
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_event(void *net, uint32_t scene_id, uint32_t event_id);

/**
 * @brief		delete a scene action by its index (useful for iterating in a loop)
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in]	index			Index of the action in a scene's action list
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_action_by_index(void *net, uint32_t scene_id, uint32_t index);
/**
 * @brief		delete a scene schedule by its index (useful for iterating in a loop)
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in]	index			Index of the schedule in a scene's schedule list
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_schedule_by_index(void *net, uint32_t scene_id, uint32_t index);
/**
 * @brief		delete a scene event by its index (useful for iterating in a loop)
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in]	index			Index of the event in a scene's event list
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_event_by_index(void *net, uint32_t scene_id, uint32_t index);

/**
 * @brief		delete all scenes for a given network
 * @param[in] 	net				Network runtime context
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_all_scenes(void *net);
/**
 * @brief		delete all actions for a given scene
 * @param[in] 	net				Network runtime context
 * @param[in]	scene_id		Scene id
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_all_actions(void *net, uint32_t scene_id);
/**
 * @brief		delete all schedules for a given scene
 * @param[in] 	net				Network runtime context
 * @param[in]	scene_id		Scene id
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_all_schedules(void *net, uint32_t scene_id);
/**
 * @brief		delete all events for a given scene
 * @param[in] 	net				Network runtime context
 * @param[in]	scene_id		Scene id
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_del_all_events(void *net, uint32_t scene_id);
///@}

/** @defgroup GroupEdit Scenes Edit
 *  Functions to edit a scene, action, schedule or event
 */
///@{
/**
 * @brief		Edit (editable) scene information
 * @param[in] 	net				Network runtime context
 * @param[in]	scene_id		Scene id
 * @param[in]	scene			new scene information (only applies to editable information)
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any input arguments after it is copied by this function
 */
ZWSCN_STATUS_T zwscn_edit_scene(void *net, uint32_t scene_id, zwscn_scene_p scene);
/**
 * @brief		Edit (editable) scene action information
 * @param[in] 	net				Network runtime context
 * @param[in]	scene_id		Scene id
 * @param[in]	action_id		Action id
 * @param[in]	scene_action	new action information (only applies to editable information)
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any input arguments after it is copied by this function
 */
ZWSCN_STATUS_T zwscn_edit_action(void *net, uint32_t scene_id, uint32_t action_id, zwscn_action_p scene_action);
/**
 * @brief		Edit (editable) scene schedule information
 * @param[in] 	net				Network runtime context
 * @param[in]	scene_id		Scene id
 * @param[in]	schedule_id		Schedule id
 * @param[in]	scene_schedule	new schedule information (only applies to editable information)
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any input arguments after it is copied by this function
 */
ZWSCN_STATUS_T zwscn_edit_schedule(void *net, uint32_t scene_id, uint32_t schedule_id, zwscn_schedule_p scene_schedule);
/**
 * @brief		Edit (editable) scene event information
 * @param[in] 	net				Network runtime context
 * @param[in]	scene_id		Scene id
 * @param[in]	event_id		Event id
 * @param[in]	scene_event	new event information (only applies to editable information)
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any input arguments after it is copied by this function
 */
ZWSCN_STATUS_T zwscn_edit_event(void *net, uint32_t scene_id, uint32_t event_id, zwscn_event_p scene_event);
///@}

/** @defgroup GroupSGet Scenes Get Information
 *  Functions to get information about a scene, action, schedule or event
 */
///@{
/**
 * @brief		get scene information
 * @param[in] 	net				Network runtime context
 * @param[in]	scene_id		Scene id
 * @param[out]	out_scene		output structure containing scene information
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any output parameter
 */
ZWSCN_STATUS_T zwscn_get_scene(void *net, uint32_t scene_id, zwscn_scene_p *out_scene);
/**
 * @brief		get scene action information
 * @param[in] 	net					Network runtime context
 * @param[in]	scene_id			Scene id
 * @param[in]	action_id			Action id
 * @param[out] 	out_scene_action	output structure containing scene action information
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any output parameter
 */
ZWSCN_STATUS_T zwscn_get_action(void *net, uint32_t scene_id, uint32_t action_id, zwscn_action_p *out_scene_action);
/**
 * @brief		get scene schedule information
 * @param[in] 	net					Network runtime context
 * @param[in]	scene_id			Scene id
 * @param[in]	schedule_id			Schedule id
 * @param[out] 	out_scene_schedule	output structure containing scene schedule information
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any output parameter
 */
ZWSCN_STATUS_T zwscn_get_schedule(void *net, uint32_t scene_id, uint32_t schedule_id, zwscn_schedule_p *out_scene_schedule);
/**
 * @brief		get scene event information
 * @param[in] 	net					Network runtime context
 * @param[in]	scene_id			Scene id
 * @param[in]	event_id			Event id
 * @param[out] 	out_scene_event	output structure containing scene event information
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any output parameter
 */
ZWSCN_STATUS_T zwscn_get_event(void *net, uint32_t scene_id, uint32_t event_id, zwscn_event_p *out_scene_event);

/**
 * @brief		get scene information by its index (useful for iterating in a loop)
 * @param[in] 	net				Network runtime context
 * @param[in]	index			Index of the scene in the list
 * @param[out] 	out_scene		output structure containing scene information
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any output parameter
 */
ZWSCN_STATUS_T zwscn_get_scene_by_index(void *net, uint32_t index, zwscn_scene_p *out_scene);
/**
 * @brief		get scene action information by its index (useful for iterating in a loop)
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in]	index			Index of the action in a scene's action list
 * @param[out] 	out_scene_action	output structure containing scene action information
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any output parameter
 */
ZWSCN_STATUS_T zwscn_get_action_by_index(void *net, uint32_t scene_id, uint32_t index, zwscn_action_p *out_scene_action);
/**
 * @brief		get scene schedule information by its index (useful for iterating in a loop)
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in]	index			Index of the schedule in a scene's schedule list
 * @param[out] 	out_scene_schedule	output structure containing scene schedule information
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any output parameter
 */
ZWSCN_STATUS_T zwscn_get_schedule_by_index(void *net, uint32_t scene_id, uint32_t index, zwscn_schedule_p *out_scene_schedule);
/**
 * @brief		get scene event information by its index (useful for iterating in a loop)
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @param[in]	index			Index of the event in a scene's event list
 * @param[out] 	out_scene_event	output structure containing scene event information
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any output parameter
 */
ZWSCN_STATUS_T zwscn_get_event_by_index(void *net, uint32_t scene_id, uint32_t index, zwscn_event_p *out_scene_event);

/**
 * @brief	get network's persistent context
 * @param[in] 	net				Network runtime context
 * @param[out] 	out_context	output string containing network's persistent context
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any output parameter
 */
ZWSCN_STATUS_T zwscn_get_network_persistent_context(void *net, char **out_context);
/**
 * @brief	get network's timezone
 * @param[in] 	net				Network runtime context
 * @param[out] 	out_timezone	output string containing network's timezone
 * @return		ZWSCN_STATUS_T
 * @post		Caller is responsible for freeing any output parameter
 */
ZWSCN_STATUS_T zwscn_get_network_timezone(void *net, char **out_timezone);

/**
 * @brief	get number of scenes for a given network
 * @param[in] 	net				Network runtime context
 * @return		number of scenes or a negative error code
 */
int zwscn_count_scenes(void *net);
/**
 * @brief	get number of actions for a given scene
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @return		number of actions or a negative error code
 */
int zwscn_count_actions(void *net, uint32_t scene_id);
/**
 * @brief	get number of schedules for a given scene
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @return		number of schedules or a negative error code
 */
int zwscn_count_schedules(void *net, uint32_t scene_id);
/**
 * @brief	get number of events for a given scene
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @return		number of events or a negative error code
 */
int zwscn_count_events(void *net, uint32_t scene_id);

/**
 * @brief	get maximum number of scenes for a given network
 * @param[in] 	net				Network runtime context
 * @return		maximum number of scenes or a negative error code
 */
int zwscn_get_max_num_scenes(void *net);
/**
 * @brief	get maximum number of actions in a scene
 * @param[in] 	net				Network runtime context
 * @return		maximum number of actions or a negative error code
 */
int zwscn_get_max_num_actions(void *net);
/**
 * @brief	get maximum number of schedules in a scene
 * @param[in] 	net				Network runtime context
 * @return		maximum number of schedules or a negative error code
 */
int zwscn_get_max_num_schedules(void *net);
/**
 * @brief	get maximum number of events in a scene
 * @param[in] 	net				Network runtime context
 * @return		maximum number of events or a negative error code
 */
int zwscn_get_max_num_events(void *net);

/**
 * @brief	get default maximum number of scenes in a netwrork
 * @return		default maximum number of scenes or a negative error code
 */
int zwscn_get_default_max_num_scenes();
/**
 * @brief	get default maximum number of actions in a scene
  * @return		default maximum number of actions or a negative error code
 */
int zwscn_get_default_max_num_actions();
/**
 * @brief	get default maximum number of schedules in a scene
 * @return		default maximum number of schedules or a negative error code
 */
int zwscn_get_default_max_num_schedules();
/**
 * @brief	get default maximum number of events in a scene
 * @return		default maximum number of events or a negative error code
 */
int zwscn_get_default_max_num_events();
///@}

/** @defgroup GroupExecute Scenes Execute
 *  Function execute a scene
 */
///@{
/**
 * @brief		execute a scene
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_execute_scene(void *net, uint32_t scene_id);
/**
 * @brief		return the scene id of the last executed scene
 * @return		scene id of the last executed scene
 */
uint32_t zwscn_get_last_executed_scene(void *net);
///@}

/** @defgroup GroupStatus Scenes Status Update
 *  Functions to request status update operation
 */
///@{
/**
 * @brief		request status update for a scene
 * @param[in] 	net				Network runtime context
 * @param[in] 	scene_id		Scene id
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_status_update_scene(void *net, uint32_t scene_id);
/**
 * @brief		request status update for all the scenes in a network
 * @param[in] 	net				Network runtime context
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_status_update_all_scenes(void *net);
///@}

/** @defgroup GroupStatelog Scenes Statelog
 *  Function to get the current state or the last modified state log.
 */
///@{
/**
 * @brief		get current state and the last state log for scenes
 * @param[in] 	net				Network runtime context
 * @param[in]	category		main statelog category
 * @param[in]	subcategory		subcategory of statelog
 * @param[out]	value			statelog value. The memory for this parameter should be pre-allocated by the caller. uint64_t is for accomodating time values.
 * @return		ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_get_statelog(void *net, uint32_t category, uint32_t subcategory, uint64_t *value);
///@}
///@}
#endif /*_ZWSCN_API_H_*/

