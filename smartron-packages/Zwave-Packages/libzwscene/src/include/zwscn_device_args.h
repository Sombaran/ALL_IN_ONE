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
@file		zwscn_device_args.h - Device specific arguments

			public definitions for device specific arguments.

@author 	Arif

@version    1.1 2013-11-26  Initial release
*/ 

#ifndef _ZWSCN_DEVICE_ARGS_H_
#define _ZWSCN_DEVICE_ARGS_H_

/** @defgroup GroupDeviceArgs Device Arguments Structures
 *  Structures for Device Arguments
 *  @ingroup GroupPublicAPI
 */
///@{
/** Device Arguments for BASIC Set operation */
typedef struct {
	uint8_t            value;			/**< Value @note Please see CC documentation for details */
} zwscn_args_basic_set_t;				/**< Device Arguments for BASIC Set operation */

/** Device Arguments for BIINARY SWITCH Set operation */
typedef struct {
	uint8_t	value;						/**< Value @note Please see CC documentation for details */
} zwscn_args_switch_binary_set_t;		/**< Device Arguments for BIINARY SWITCH Set operation */

/** Device Arguments for MULTILEVEL SWITCH Set operation */
typedef struct {
	uint8_t            value;			/**< Value @note Please see CC documentation for details */
} zwscn_args_switch_multilevel_set_t;	/**< Device Arguments for MULTILEVEL SWITCH Set operation */

/** Device Arguments for DOORLOCK Set operation */
typedef struct {
	uint8_t	mode;						/**< Mode @note Please see CC documentation for details */
	 
	/** @name private data @remark used internally by Event trigger logic */
	//@{
	zwscn_time_t			last_match_time;					/**< Private variable to store value of last match timestamp */
	//@}
} zwscn_args_door_lock_operation_set_t;	/**< Device Arguments for DOORLOCK Set operation */

/** Device Arguments for THERMOSTAT SETPOINT Set operation */
typedef struct {
	zwsetp_t           setpoint;		/**< Thermostat Setpoint data structure @note Please see C-API header for details */
} zwscn_args_thermostat_setpoint_set_t;	/**< Device Arguments for THERMOSTAT SETPOINT Set operation */

/** Device Arguments for THERMOSTAT SETPOINT Get operation */
typedef struct {
	uint32_t type;						/**< Thermostat Setpoint Type @note Please see CC documentation for details */
} zwscn_args_thermostat_setpoint_get_t;	/**< Device Arguments for THERMOSTAT SETPOINT Get operation */

//The following device specific arguments are not used for SET operation but are used for event triggers
#define ZWSCN_ARGS_SENSOR_BINARY_MAX_TYPE	0x0F	/**< Maximum number of Z-Wave BINARY SENSOR types @note It is actually 0x00 - 0x0D in spec so we need 0x0E spaces only */
/** Device Arguments for BINARY SENSOR */
typedef struct {
	uint8_t			sensor_value;		/**< Value @note Please see CC documentation for details */
	uint8_t			type;				/**< Sensor type @note introduced in CC version 2 */
	
	/** @name private data @remark used internally by Event trigger logic */
	//@{
	zwscn_time_t			last_match_time;		/**< Private variable to store value of last match timestamp */
	//@}
} zwscn_args_sensor_binary_t;			/**< Device Arguments for BINARY SENSOR */

/** Device Arguments for MULTILEVEL SENSOR
 * @note Multilevel Sensor Scene event accepts a range of low-high values. The 'type' and 'unit' of value_low and value_high MUST be the same.
 */
typedef struct { 
	zwsensor_t		value_low;			/**< Low Value for specified range @note Please see C-API header for data structure details */
	zwsensor_t		value_high;			/**< High Value for specified range @note Please see C-API header for data structure details */
	
	/** @name private data @remark used internally by Event trigger logic */
	//@{
	uint8_t			flag_first_time_check_done;	/**< Private flag to check first time operation */
	double			value_last;					/**< Private variable to store last value */
	//@}
} zwscn_args_sensor_multilevel_t;		/**< Device Arguments for MULTILEVEL SENSOR */

#define ZWSCN_ARGS_ALARM_MAX_EVENTS	32					/**< Maximum number of Z-Wave Alarm events per type */
#define ZWSCN_ARGS_ALARM_EVENTS_NOT_FOUND	(ZWSCN_ARGS_ALARM_MAX_EVENTS + 10)	/**< Z-Wave Alarm events not found marker */
/** Device Arguments for ALARM / NOTIFICATION */
typedef struct { 
	/** @name Vendor Alarm (Alarm CC v1) */
	//@{
	uint8_t		va_type; 								/**< Vendor Alarm type */
	uint8_t		va_level;								/**< Vendor Alarm level */
	//@}
	
	/** @name Z-Wave Alarm (Alarm CC v2+) */
	//@{
	uint8_t		za_type;								/**< Z-Wave Alarm type */
	uint8_t		za_event[ZWSCN_ARGS_ALARM_MAX_EVENTS];	/**< Z-Wave Alarm event */
	uint32_t	za_event_count;							/**< Z-Wave Alarm events count */
	
	uint8_t			va_level_value_last;				/**< Private variable to store last value */
	zwscn_time_t	va_last_matched_time;
	uint32_t		za_event_last_match;				/**< Z-Wave Alarm event matched last time */
	zwscn_time_t	za_event_last_matched_time[ZWSCN_ARGS_ALARM_MAX_EVENTS];	/**< Z-Wave Alarm event */
	
	//@}
} zwscn_args_alarm_t;					/**< Device Arguments for ALARM / NOTIFICATION*/

/** Device Arguments for BASIC EVENT
 * @note Multilevel Sensor Scene event accepts a range of low-high values. The 'type' and 'unit' of value_low and value_high MUST be the same.
 */
typedef struct { 
	uint8_t		value_low;			/**< Low Value for specified range */
	uint8_t		value_high;			/**< High Value for specified range */
	
	/** @name private data @remark used internally by Event trigger logic */
	//@{
	uint8_t			flag_first_time_check_done;	/**< Private flag to check first time operation */
	uint8_t			value_last;					/**< Private variable to store last value */
	//@}
} zwscn_args_basic_event_t;		/**< Device Arguments for BASIC EVENT */

/** Device Arguments for CENTRAL SCENE CC
 */
typedef struct { 
	uint8_t		central_scene_key_number;		/**< Central Scene key number @note it is scene as Scene Number in CC documentation */
	uint8_t		central_scene_key_attribute;	/**< Central Scene key attribute */
} zwscn_args_central_scene_t;					/**< Device Arguments for CENTRAL SCENE CC */
///@}

#endif /*_ZWSCN_DEVICE_ARGS_H_*/
