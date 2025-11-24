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
@file		zwscn_defines.h  
  
                definations of various constants and macros
  
@author		Arif Mohammand
 
@version	1.0 2014-04-17  Initial release

@ingroup GroupPrivateGeneral
*/

#ifndef _ZWSCN_DEFINES_H_
#define _ZWSCN_DEFINES_H_

/** @addtogroup GroupPrivateDefines
 */
///@{

/** @defgroup GroupPrivateDefinesThreads Threads
 *  Threads Definitions
 */
///@{
#define ZWSCN_THREAD_NAME_EVENTS 						"T_EVENTS"			/**< Name of Events thread */
#define ZWSCN_THREAD_NAME_EXECUTE_REQUEST_HANDLER		"T_EXECUTE"			/**< Name of Execute thread */
#define ZWSCN_THREAD_NAME_STATUS_UPDATE_REQUEST_HANDLER	"T_SU_REQ"			/**< Name of Status Update Request handler thread */
#define ZWSCN_THREAD_NAME_STATUS_UPDATE_REPORT_HANDLER	"T_SU_REP"			/**< Name of Status Update Report handler thread */
#define ZWSCN_THREAD_NAME_NOTIFICATION_PROCESSOR		"T_NOTIFICATION"	/**< Name of Notification processor thread */
#define ZWSCN_THREAD_NAME_SECURITY_EVENTS 				"T_SS_EVENTS"		/**< Name of Security Scenes Events thread */
#define ZWSCN_THREAD_NAME_SECURITY_SECENE_NOTIFICATIONS	"T_SS_NOTIFICATION"	/**< Name of Security Scenes Notification Request handler thread */
///@}

/** @defgroup GroupPrivateDefinesMessage Message Types and Priority
 *  Message Types and Priority Definitions
 *  @remark Randomly chosen number for message type
 */
///@{
#define ZWSCN_MESSAGE_TYPE_EXIT								1	/**< Exit message */
#define ZWSCN_MESSAGE_TYPE_NOTIFICATION_NETWORK				10	/**< Network Notification message */
#define ZWSCN_MESSAGE_TYPE_NOTIFICATION_NODE				20	/**< Node Notification message */
#define ZWSCN_MESSAGE_TYPE_NOTIFICATION_ZWPOLL				25	/**< ZWPoll Notification message */
#define ZWSCN_MESSAGE_TYPE_NOTIFICATION_POSTSETPOLL			26	/**< Post-Set Poll Notification message */
#define ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_SCENE				30	/**< Scene Status Update message */
#define ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_SCENE_ON_CHANGE	31	/**< Scene Status Update on change in Scene message */
#define ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_SCENE_ON_EXECUTE	32	/**< Scene Status Update on Execute message */
#define ZWSCN_MESSAGE_TYPE_STATUS_UPDATE_ALL				33	/**< Status Update All message */
#define ZWSCN_MESSAGE_TYPE_EXECUTE_SCENE					50	/**< Scene Execute message */
#define ZWSCN_MESSAGE_TYPE_EXECUTE_SCENE_ON_SCHEDULE		51	/**< Scene Execute on Schedule trigger message */
#define ZWSCN_MESSAGE_TYPE_EXECUTE_SCENE_ON_EVENT			52	/**< Scene Execute on Event trigger message */
#define ZWSCN_MESSAGE_TYPE_SECURITY_SCENE_NOTIFICATION_ALARM 100	/**< Security Scene Notification Send request */
#define ZWSCN_MESSAGE_TYPE_SECURITY_SCENE_NOTIFICATION_ADD	 101	/**< Security Scene Notification Send request */
#define ZWSCN_MESSAGE_TYPE_SECURITY_SCENE_NOTIFICATION_EDIT	 102	/**< Security Scene Notification Send request */

/** Message priority */
#define ZWSCN_MESSAGE_PRIORITY_STATUS_UPDATE_ON_EXECUTE	50								/**< Priority of Status Update on Execute message */
#define ZWSCN_MESSAGE_PRIORITY_HIGHEST					100								/**< Highest Priority of a message */
#define ZWSCN_MESSAGE_TYPE_EXIT_PRIORITY				ZWSCN_MESSAGE_PRIORITY_HIGHEST	/**< Priority of Exit message */
///@}

/** @defgroup GroupPrivateDefinesNotifications Notifications Types
 *  Notifications Types (message subtypes) Definitions
 */
///@{
#define	ZWSCN_NOTIFICATION_NETWORK_RESET				11	/**< Network Reset Notification */
#define	ZWSCN_NOTIFICATION_NETWORK_UPDATE				12	/**< Network Update Notification */
#define	ZWSCN_NOTIFICATION_NETWORK_REPLACE_FAILED_NODE	13	/**< Network Replace Failed Node Notification */
#define	ZWSCN_NOTIFICATION_NODE_REMOVED					21	/**< Node Removed Notification */
#define	ZWSCN_NOTIFICATION_NODE_UPDATE					22	/**< Node Update Notification */
///@}

/** @defgroup GroupPrivateDefinesMessageQueue Message Queue
 *  Message Queue Definitions
 */
///@{
#define ZWSCN_MQUEUE_LENGTH_MAX_EVENTS							100	/**< Events messages queue capacity */
#define ZWSCN_MQUEUE_LENGTH_MAX_EXECUTE_REQUEST_HANDLER			100	/**< Execute messages queue capacity */
#define ZWSCN_MQUEUE_LENGTH_MAX_STATUS_UPDATE_REQUEST_HANDLER	100	/**< Status Update Request messages queue capacity */
#define ZWSCN_MQUEUE_LENGTH_MAX_STATUS_UPDATE_REPORT_HANDLER	100	/**< Status Update Report messages queue capacity */
#define ZWSCN_MQUEUE_LENGTH_MAX_NOTIFICATIONS					100	/**< Notifications processor messages queue capacity */
#define ZWSCN_MQUEUE_LENGTH_MAX_SECURITY_SCENE_EVENTS			100	/**< Security Scene Events messages queue capacity */
#define ZWSCN_MQUEUE_LENGTH_MAX_SECURITY_SCENE_NOTIFICATIONS	100	/**< Security Scene Notifications sender module messages queue capacity */
///@}

/** @defgroup GroupPrivateDefinesSchedules Schedules
 *  Schedules Definitions
 */
///@{
#define ZWSCN_SCHEDULE_DAY_BITMASK_MIN	0		/**< Schedule Day-Bitmask minimum value */
#define ZWSCN_SCHEDULE_DAY_BITMASK_MAX	0x7F	/**< Schedule Day-Bitmask maximum value	@remark Seven bits 0-6 are set */
#define ZWSCN_SCHEDULE_HOUR_MIN			0		/**< Schedule Hour minimum value */
#define ZWSCN_SCHEDULE_HOUR_MAX			23		/**< Schedule Hour maximum value */
#define ZWSCN_SCHEDULE_MINUTE_MIN		0		/**< Schedule Minute minimum value */
#define ZWSCN_SCHEDULE_MINUTE_MAX		59		/**< Schedule Minute maximum value */
///@}

/** @defgroup GroupPrivateDefinesStatusUpdate Status Update
 *  Status Update Definitions
 */
///@{
#define ZWSCN_STATUS_CC_SWITCH_MULTILEVEL_THRESHHOLD	10	/**< Threshold for multilevel switch value when match against configured value */
#define ZWSCN_STATUS_UPDATE_REQ_TIMEOUT					10	/**< Status Update Request common timeout @remark in seconds */
#define ZWSCN_STATUS_UPDATE_REQ_TIMEOUT_PER_ACTION		2	/**< Status Update Request timeout per action @remark in seconds */
#define ZWSCN_DEVICE_REPORT_FRESHNESS_TIMEOUT			1	/**< Device Report freshness timeout after which cached value is considered stale @ remark in second */
#define ZWSCN_STATUS_UPDATE_HASH_KEY_BASE				10	/**< Hash key base (arbitrary number to avoid key with 0 value) */
///@}

/** @defgroup GroupPrivateDefinesZWPoll ZWPoll
 *  ZWPoll Definitions
 */
///@{
#define ZWSCN_ZWPOLL_INTERVAL_DEFAULT					1	/**< Default interval (poll every 1 second if possible for number of count) */
#define ZWSCN_ZWPOLL_COUNT_DEFAULT						1	/**< Default Poll count */
#define ZWSCN_ZWPOLL_COUNT_CC_SWITCH_MULTILEVEL			12	/**< Poll count for Multilevel Switch CC @remark slow device - shades */
#define ZWSCN_ZWPOLL_COUNT_CC_DOOR_LOCK					10	/**< Poll count for Door-Lock CC @remark slow device - door lock */
///@}

/** @defgroup GroupPrivateDefinesTime Time
 *  Time Definitions
 */
///@{
#define ZWSCN_TIME_NOW()	zwscn_time_now()									/**< Macro to get current timestamp */
#define ZWSCN_TIME_WEEK_DAYS_MAX	7											/**< Maximum number of days in a week */
#define ZWSCN_TIME_SEC_MAX			59											/**< Maximum number of seconds in a minute */
#define ZWSCN_TIME_MICRO2MILLI_SEC(microsec)	((microsec)/1000)				/**< Convert microseconds to milliseconds */
#define ZWSCN_TIME_MICRO2SEC(microsec)			(((microsec)/1000000) + 0.5)	/**< Convert microseconds to seconds */
#define ZWSCN_TIME_SEC2MICRO(sec)				((sec)*1000000)					/**< Convert seconds to microseconds */
///@}

/** @defgroup GroupPrivateDefinesBufferLimits Memory Buffer Limits
 *  Memory Buffer Limits Definitions
 */
///@{
#define ZWSCN_DEVICE_ARGS_MAX_SIZE		(2*1024)	/**< Device Arguments maximum buffer size */
#define ZWSCN_DEVICE_REPORT_MAX_SIZE	(2*1024)	/**< Device Report maximum buffer size */
#define ZWSCN_NOTIFICATION_MAX_SIZE		(1024)		/**< Notification maximum buffer size */
///@}

/** @defgroup GroupPrivateDefinesZWave Z-Wave
 *  Z-Wave related Definitions
 */
///@{
#define ZWSCN_ZWAVE_CC_THERMOSTAT_SETPOINT_MAX_UNIT	0x01	/**< Maximum index of Thermostat Setpoint unit */
#define ZWSCN_ZWAVE_CC_SENSOR_MULTILEVEL_TYPE_RESERVED	0	/**< Reseved type of Multilevel Sensor */
#define ZWSCN_ZWAVE_CC_SENSOR_MULTILEVEL_MAX_TYPE	0x33	/**< Maximum number of Multilevel Sensor types */
#define ZWSCN_ZWAVE_CC_SENSOR_MULTILEVEL_MAX_UNIT	0x03	/**< Maximum index of Multilevel Sensor unit for a given type */
#define ZWSCN_ZWAVE_SAME_REPORT_IGNORE_INTERVAL		(30UL * 60UL) /**< Default interval to ignore the same report if it is received repeatedly */
///@}

/** @defgroup GroupPrivateDefinesGeneral General/Utils
 *  General/Utils Definitions
 */
///@{
#define ZWSCN_COMPARE_DOUBLE_EPSILON	0.00000001							/**< Epsilon value used for checking equality of double values */
#define ZWSCN_SCENE_ID_MIN				1									/**< Minimum allowed id for Scenes */
#define ZWSCN_SECURITY_SCENE_ID_MIN		10000								/**< Minimum allowed id for Security Scenes */
#define ZWSCN_SCENE_ID_MAX				(ZWSCN_SECURITY_SCENE_ID_MIN - 1)	/**< Maximum allowed id for Scenes */
#define ZWSCN_SECURITY_SCENE_ID_MAX		99999999							/**< Maximum allowed id for Security Scenes */
///@}

/** @defgroup GroupPrivateDefinesCodeControl Code Control
 *  Code Control Definitions
 */
///@{
#define ZWSCN_CHECK_DEVICE_INTF_IN_ACTIONS_AND_EVENSTS		1	/**< Control if Device Interface is checked across Actions and Events for uniqueness */
#define ZWSCN_CHECK_DUPLICATE_IN_SAME_SECURITY_EVENSTS		1	/**< Control if Device Interface is checked for uniqueness */
#define ZWSCN_CHECK_DUPLICATE_IN_OTHER_SECURITY_EVENSTS		1	/**< Control if Device Interface is checked across arm, disarm and alarm Security Events for uniqueness */
#define ZWSCN_ZWAVE_BASIC_EVENT_TRIGGER_IN_RANGE			1	/**< Control if BASIC EVENT triggers a Scene even when the last BASIC EVENT was in the valid range */
#define ZWSCN_DEVICE_ARGS_EVAL_ENABLE						0	/**< Control if Device args eval code is enabled */
#define ZWSCN_PERSISTENCE_ENABLE_XML                        0   /**< Enables XML Persistence */
///@}
/** @defgroup GroupPrivateDefinesRegEx Regular Expressions
 *  Regular Expressions Definitions
 */
///@{
#define ZWSCN_REGEX_SMS_NUMBER	"^\\+?\\d{8,20}$"											/**< Regular Expression for validating SMS number */
#define ZWSCN_REGEX_EMAIL_ADDR	"^[A-Za-z0-9._%+-]+@(?:[A-Za-z0-9-]+\\.)+[A-Za-z]{2,4}$"	/**< Regular Expression for validating Email address */
///@}
/** @defgroup GroupPrivateDefinesSecurityScenes Security Scenes
 *  Security Scenes Definitions
 */
///@{
#define ZWSCN_SENDER_EMAIL_ADDR	"z-wavesupport@sigmadesigns.com"	/**< Sender's Email address */
#define ZWSCN_SENDER_EMAIL_ADDR_FOR_SMS	ZWSCN_SENDER_EMAIL_ADDR		/**< Sender's Email address for SMS */
#define ZWSCN_SMS_GATEWAY	"onewaysms.asia"						/**< SMS gateway address */
#define ZWSCN_NODE_STR_MAX	32										/**< Node name string length */
#define ZWSCN_LOC_STR_MAX	32										/**< Location string length */
#define ZWSCN_TEMP_STR_MAX	1024									/**< Temporary string length */
///@}
/** @defgroup GroupPrivateDefinesConfigFile Configuration File
 *  Configuration File Definitions
 */
///@{
#define ZWSCN_CONFIG_FILENAME	"zwscenes.conf"												/**< ZWScene configuration file name */
#define ZWSCN_CONFIG_GROUP_COMMON					"COMMON"								/**< Configuration group name for common settings */
#define ZWSCN_CONFIG_GROUP_SS_NOTIFICATION_EMAIL	"SECURITY SCENE NOTIFICATION EMAIL"		/**< Configuration group name for Security Scenes email notification */
#define ZWSCN_CONFIG_GROUP_SS_NOTIFICATION_SMS		"SECURITY SCENE NOTIFICATION SMS"		/**< Configuration group name for Security Scenes SMS notification */
#define ZWSCN_CONFIG_GROUP_SMTP						"SMTP"									/**< Configuration group name for Security Scenes SMTP */
#define ZWSCN_CONFIG_KEY_ZWAVE_SAME_REPORT_IGNORE_INTERVAL	"zwave_same_report_ignore_interval"	/**< Configuration key */
#define ZWSCN_CONFIG_KEY_ENABLE				"enable"										/**< Configuration key */
#define ZWSCN_CONFIG_KEY_SENDER_EMAIL_ADDR	"sender"										/**< Configuration key */
#define ZWSCN_CONFIG_KEY_SMS_GATEWAY		"gateway"										/**< Configuration key */
#define ZWSCN_CONFIG_KEY_AUTH_ENABLE		"auth_enable"									/**< Configuration key */
#define ZWSCN_CONFIG_KEY_USERNAME			"username"										/**< Configuration key */
#define ZWSCN_CONFIG_KEY_PASSWORD			"password"										/**< Configuration key */
#define ZWSCN_CONFIG_KEY_SERVER_HOSTNAME	"server_hostname"								/**< Configuration key */
#define ZWSCN_CONFIG_KEY_SERVER_PORT		"server_port"									/**< Configuration key */
#define ZWSCN_CONFIG_KEY_SECURE_METHOD		"secure_method"									/**< Configuration key */
///@}

///@}
#endif // _ZWSCN_DEFINES_H_
