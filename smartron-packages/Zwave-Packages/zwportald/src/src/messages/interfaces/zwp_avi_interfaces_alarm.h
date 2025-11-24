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
 * @file    zwp_avi_interfaces_alarm.h
 *
 * @brief   Structure definitions for AVI message 'interface_alarm_xxx'
 * @details Structure definitions for AVI message 'interface_alarm_xxx'.
 *
 * @author  Angela Yan
 *
 * @version 1.0 - 2016-12-30
 * - Initial version
 */

#ifndef ZWP_AVI_INTERFACES_ALARM_H_
#define ZWP_AVI_INTERFACES_ALARM_H_

#include "zwp_avi_interfaces.h"
#include "zwp_avi_ifttt_common.h"
#include "zwp_db.h"

 //#define ZWP_INTERFACE_ALARM_TEST

 /** Alarm type not specified */
#define COMMAND_ALARM_TYPE_NONE               (-1)
 /** Latest alarm type */
#define COMMAND_ALARM_TYPE_LATEST             (-2)

 /** Z-Wave alarm type not specified */
#define COMMAND_ZWAVE_ALARM_TYPE_NONE         (-1)

 /** Z-Wave alarm event not specified */
#define COMMAND_ZWAVE_ALARM_EVENT_NONE        (-1)

 /** Event param not used. To be used to set to HCAPI. Upon receiving this, HCAPI will not
 *  send any event parameter in the Get call.
 *  See @a zwif_alrm_get from HCAPI.*/
#define COMMAND_ZWAVE_ALARM_EVENT_NOT_USED     0

 /** Get command state */
typedef struct
{
	int                alarm_type;             /**< Alarm type */
	int                zwave_alarm_type;       /**< Z-Wave alarm type */
	int                zwave_alarm_event;      /**< Z-Wave alarm event - optional parameter */
} zwp_avi_interfaces_alarm_get_t;

/** Set command state */
typedef struct
{
	int                zwave_alarm_type;       /**< Z-Wave alarm type */
	int                zwave_alarm_status;     /**< Z-Wave alarm status */
} zwp_avi_interfaces_alarm_set_t;

/** Alarm value state */
typedef struct
{
	zwu_time_t         utime;                  /**< Last update time for specific alarm_type and zwave_alarm_type combination */
	zwalrm_t           value;                  /**< Alarm value */
	void               *event_parameter;       /**< Stores copy of 'value.ex_evt_prm' because it spills over zwalrm_t */
} zwp_avi_interfaces_alarm_report_alarm_value_t;

/** Report command state */
typedef struct
{
	zwu_time_t         utime;                  /**< Last update time across all types */
	zwu_array_list_t   alarm_value_list;       /**< List of alarm values of type @a zwp_avi_interfaces_alarm_report_alarm_value_t */
	int                alarm_value_last_updated; /**< Index of last updated alarm value in 'alarm_value_list' */

	int                first_alarm_type;       /**< Alarm type of first Report received after sending Get with ZW_ALRM_FIRST */
	int                first_zwave_alarm_type; /**< Z-Wave Alarm type of first Report received after sending Get with ZW_ALRM_FIRST */
	int                wait_for_first_alarm;   /**< Flag to indicate if waiting for first alarm. 1, if waiting; 0, otherwise */

	int                report_receive_state;              /**< Report command receive state */
	zwp_thread_cond_t  report_receive_condition;          /**< Report command receive condition */
	int                report_receive_alarm_type;         /**< Current alarm type for Report command receive condition */
	int                report_receive_zwave_alarm_type;   /**< Current Z-Wave alarm type for Report command receive condition */
	int                report_receive_zwave_alarm_event;   /**< Current Z-Wave alarm event for Report command receive condition */
} zwp_avi_interfaces_alarm_report_t;

/** Supported Type Report command state */
typedef struct
{
	zwu_time_t         utime;                  /**< Last updated time of Supported Report command */
	uint8_t            have_vendor_type;       /**< Flag indicating if vendor type is supported */
	uint8_t            *supported_types;       /**< List of supported types */
	uint8_t            supported_types_count;  /**< Count of supported types */

	int                report_receive_state;         /**< Supported Report command receive state */
	zwp_thread_cond_t  report_receive_condition;     /**< Supported Report command receive condition */
} zwp_avi_interfaces_alarm_type_supported_report_t;

/** Supported Event Get command state */
typedef struct
{
	int                zwave_alarm_type;       /**< Z-Wave alarm type */
} zwp_avi_interfaces_alarm_event_supported_get_t;

/** Supported Event Report command state */
typedef struct
{
	zwu_time_t         utime;                  /**< Last updated time of Supported event Report command */
	uint8_t            *supported_events;       /**< List of supported events */
	uint8_t            supported_events_count;  /**< Count of supported events */

	int                report_receive_zwave_alarm_type;   /**< Current Z-Wave alarm type for Report command receive condition */
} zwp_avi_interfaces_alarm_single_event_supported_report_t;

typedef struct
{
	zwu_array_list_t	event_supported_list;	/**< List of supported Event Report command state per
												zwave_alarm_type. @a zwp_avi_interfaces_alarm_single_event_supported_report_t */

	int                report_receive_state;         /**< Supported Report command receive state */
	zwp_thread_cond_t  report_receive_condition;     /**< Supported Report command receive condition */
} zwp_avi_interfaces_alarm_type_event_supported_report_t;

/** IFTTT trigger alarm get result */
typedef struct
{
	char		    name[ZW_LOC_STR_MAX + 1];   /**< user configured name string of the endpoint */
	char		    loc[ZW_LOC_STR_MAX + 1];	/**< user configured location string of the endpoint */

	int				iNofRecords;			/**< Number of records found */
	zwu_time_t      *putimeList;			/**< Time stample for each records */
} zwp_avi_ifttt_alarm_get_result_t;

/** Interface level state */
typedef struct
{
	zwp_avi_interfaces_alarm_report_t						report;                 /**< Report command state */
	zwp_avi_interfaces_alarm_type_supported_report_t		type_supported_report;  /**< Supported Type Report command state */
																					//zwu_array_list_t 										event_supported_list;   
	zwp_avi_interfaces_alarm_type_event_supported_report_t	type_event_supported_report;  /**< Supported Event Report command state */

																						  //state for IFTTT
#ifndef LOCAL_CLIENT_ENABLE
	zwp_db_table										*pifttt_generic_trigger_table;		/**< IFTTT generic trigger table pointer */
	zwp_db_table										*pifttt_alarm_table;		/**< IFTTT Alarm CC table pointer */
	zwp_avi_ifttt_alarm_get_result_t					 ifttt_alarm_get_result;   /**< IFTTT Get Report. */
  //zwu_array_list_t									 ifttt_get_result_list; /**< IFTTT Get Report list. Each element is zwu_time_t* */
#endif
} zwp_avi_interfaces_alarm_state_t;


typedef struct
{
	int								zwave_alarm_type;       /**< Z-Wave alarm type */
	int								zwave_alarm_event;      /**< Z-Wave alarm event */
	int								zwave_alarm_event_param_len; /**< Z-Wave alarm event param length*/
	uint8_t							zwave_alarm_event_param[50];/**< Z-Wave alarm event param- optional parameter */
} zwp_avi_ifttt_alarm_get_param_single_t;

/** IFTTT trigger alarm get argument */
typedef struct
{
	zwp_avi_ifttt_trigger_get_t				trigger_id_data;	/**< Trigger ID data */
	eifttt_IF_feature_type					eTrigger;			/**< IFTTT eTrigger */
	int										inofParam;			/**< No. of alarm get parameters parsed from category */
	zwp_avi_ifttt_alarm_get_param_single_t	alarm_param[2];		/**< Z-Wave alarm get parameters */
} zwp_avi_ifttt_alarm_get_t;





/**
* @brief HCAPI callback for Supported Report command
* @param[in]	  ifd	            interface
* @param[in]	  have_vtype        flag to indicate whether vendor specific alarm type supported. 1=supported; else 0=unsupported
* @param[in]	  ztype_len         size of ztype buffer
* @param[in]	  ztype             buffer to store supported Z-wave alarm types (ZW_ALRM_XXX)
*/
void zwp_avi_interfaces_alarm_type_supported_report_handler(zwifd_p ifd, uint8_t have_vtype, uint8_t ztype_len, uint8_t *ztype, int valid);

/**
* @brief HCAPI callback for Event Supported Report command
* @param[in]	  ifd	          interface
* @param[in]	  ztype			  Z-wave alarm type (ZW_ALRM_XXX)
* @param[in]	  evt_len         size of evt buffer
* @param[in]	  evt             buffer to store supported event of the alarm type specified in ztype
*/
void zwp_avi_interfaces_alarm_event_supported_report_handler(zwifd_p ifd, uint8_t ztype, uint8_t evt_len, uint8_t *evt, int valid);

/**
* @brief Allocate interface level data for IFTTT
* @param[in]	 desc_interface    Interface descriptor
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zwp_avi_interfaces_alarm_state_free_ifttt() to free resources
*/
zwp_status_t zwp_avi_interfaces_alarm_state_alloc_ifttt(zwifd_p desc_interface);

/**
* @brief Free interface level data
* @param[in]     state               Interface level data
* @return        Nothing
* @see zwp_avi_interfaces_alarm_state_alloc_ifttt
*/
void zwp_avi_interfaces_alarm_state_free_ifttt(zwp_avi_interfaces_alarm_state_t *state);

/**
* @brief HCAPI report handler for IFTTT trigger
* @param[in]	  desc_interface  interface descriptor
* @param[in]	  alarm_info	  Alarm information
* @param[in]	  ts			  timestamp
*/
void zwp_avi_interfaces_alarm_report_handler_ifttt(zwifd_p desc_interface, zwalrm_p alarm_info, time_t ts);

#endif /* ZWP_AVI_INTERFACES_ALARM_H_ */