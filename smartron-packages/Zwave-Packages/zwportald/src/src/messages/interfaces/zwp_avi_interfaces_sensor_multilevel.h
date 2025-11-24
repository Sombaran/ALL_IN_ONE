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
 * @file    zwp_avi_interfaces_sensor_multilevel.h
 *
 * @brief   Structure definitions for AVI message 'interface_sensor_multilevel_xxx'
 * @details Structure definitions for AVI message 'interface_sensor_multilevel_xxx'.
 *
 * @author  Angela Yan
 *
 * @version 1.0 - 2017-2-23
 * - Initial version
 */

#ifndef ZWP_AVI_INTERFACES_SENSOR_MULTILEVEL_H_
#define ZWP_AVI_INTERFACES_SENSOR_MULTILEVEL_H_

#include "zwp_avi_interfaces.h"
#include "zwp_avi_ifttt_common.h"
#include "zwp_db.h"

 /** Sensor type not specified */
#define COMMAND_SENSOR_TYPE_NONE							(-1)

 /** Sensor scale not specified */
#define COMMAND_SENSOR_SCALE_NONE							(-1)

 /** Sensor type param not used. To be used to set to HCAPI. */
#define COMMAND_HCAPI_SENSOR_TYPE_NOT_USED					0

 /** Sensor scale param not used. To be used to set to HCAPI. */
#define COMMAND_HCAPI_SENSOR_SCALE_NOT_USED					0

 /** IFTTT comparison command. Copied from Zweb */
#define ZW_SENSOR_COMPARISON_COMMAND_LT		1		/**< Comparison command less than */
#define ZW_SENSOR_COMPARISON_COMMAND_GT		2		/**< Comparison command greater than */

 /** Get command state */
typedef struct
{
	int					type;								/**< Multilevel sensor type */
	int					scale;								/**< Multilevel sensor scale */
} zwp_avi_interfaces_sensor_multilevel_get_t;

/** Sensor value description */
typedef struct
{
	zwu_time_t         utime;                              /**< Last update time for specific type and scale combination */
	zwsensor_t         value;                              /**< Value */
} zwp_avi_interfaces_sensor_multilevel_report_sensor_value_t;

/** Report command state */
typedef struct
{
	zwu_time_t         utime;                              /**< Last update time across all types */
	zwu_array_list_t   sensor_value_list;                  /**< List of sensor values of type @a zwp_avi_interfaces_sensor_multilevel_report_sensor_value_t */

	int                report_receive_state;               /**< Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Report command receive condition */
} zwp_avi_interfaces_sensor_multilevel_report_t;

/** Supported Report command state */
typedef struct
{
	zwu_time_t         utime;                              /**< Last updated time of Supported Report command */
	uint8_t            *supported_types;                   /**< List of supported types */
	uint8_t            supported_types_count;              /**< Count of supported types in the list @a supported_types */

	int                report_receive_state;               /**< Supported Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Supported Report command receive condition */
} zwp_avi_interfaces_sensor_multilevel_supported_report_t;

/** Supported scale Get command state */
typedef struct
{
	int					type;								/**< Multilevel sensor type */
} zwp_avi_interfaces_sensor_multilevel_scale_supported_get_t;

/** Supported Scale Report command state */
typedef struct
{
	zwu_time_t         utime;								/**< Last updated time of Supported scale Report command */
	uint8_t            *supported_scales;					/**< List of supported scales */
	uint8_t            supported_scales_count;				/**< Count of supported scales */

	int                report_receive_sensor_type;			/**< Current sensor type for Report command receive condition */
} zwp_avi_interfaces_sensor_multilevel_single_scale_supported_report_t;

typedef struct
{
	zwu_array_list_t	scale_supported_list;				/**< List of supported Scale Report command state per
															sensor_type. @a zwp_avi_interfaces_sensor_multilevel_single_scale_supported_report_t */

	int                report_receive_state;				/**< Supported Report command receive state */
	zwp_thread_cond_t  report_receive_condition;			/**< Supported Report command receive condition */
} zwp_avi_interfaces_sensor_multilevel_type_scale_supported_report_t;

/** IFTTT trigger sensor_multilevel get record single */
typedef struct
{
	int				type;			/**< Sensor Type */
	char			cchSnsr_value[50];			/**< Sensor value in string format */
	int				unit;			/**< Sensor Unit */
	char			cchSnsr_unit[50];			/**< Sensor Unit in string format*/
	zwu_time_t      utime;			/**< Time stample for each records */
} zwp_avi_ifttt_sensor_multilevel_get_record_single_t;

/** IFTTT trigger sensor_multilevel get result */
typedef struct
{
	char		    name[ZW_LOC_STR_MAX + 1];   /**< user configured name string of the endpoint */
	char		    loc[ZW_LOC_STR_MAX + 1];	/**< user configured location string of the endpoint */

	zwu_array_list_t sensor_result_list;		/**< Array of  zwp_avi_ifttt_sensor_multilevel_get_record_single_t */
} zwp_avi_ifttt_sensor_multilevel_get_result_t;

/** Interface level state */
typedef struct
{
	zwp_avi_interfaces_sensor_multilevel_report_t      report;                    /**< Report command state */
	zwp_avi_interfaces_sensor_multilevel_supported_report_t supported_report;     /**< Supported Report command state */
	zwp_avi_interfaces_sensor_multilevel_type_scale_supported_report_t	type_scale_supported_report;  /**< Supported Scale Report command state */

#ifndef LOCAL_CLIENT_ENABLE
	zwp_db_table										*pifttt_generic_trigger_table;		/**< IFTTT generic trigger table pointer */
	zwp_db_table										*pifttt_msensor_table;				/**< IFTTT specific CC table pointer */
	//zwu_array_list_t									 ifttt_msensor_get_result_list;		/**< Array of  zwp_avi_ifttt_sensor_multilevel_get_record_single_t */
	zwp_avi_ifttt_sensor_multilevel_get_result_t		ifttt_msensor_get_result;			/**< IFTTT Get Report. */
																										   //zwu_array_list_t									 ifttt_get_result_list; /**< IFTTT Get Report list. Each element is zwu_time_t* */
#endif
} zwp_avi_interfaces_sensor_multilevel_state_t;


//typedef struct
//{
//	int								zwave_sensor_multilevel_type;       /**< Z-Wave sensor_multilevel type */
//	int								zwave_sensor_multilevel_event;      /**< Z-Wave sensor_multilevel event */
//	int								zwave_sensor_multilevel_event_param_len; /**< Z-Wave sensor_multilevel event param length*/
//	uint8_t							zwave_sensor_multilevel_event_param[50];/**< Z-Wave sensor_multilevel event param- optional parameter */
//} zwp_avi_ifttt_sensor_multilevel_get_param_single_t;
//
/** IFTTT trigger sensor_multilevel get argument */
typedef struct
{
	zwp_avi_ifttt_trigger_get_t				trigger_id_data;	/**< Trigger ID data */
	int										type;				/**< Multilevel sensor type */
	int										comp_command;		/**< Comparison command */
	char									cArrThld_value[50];	/**< Theshold value in string */
	int										thld_unit;			/**< Threshold unit */
	//eifttt_IF_feature_type					eTrigger;			/**< IFTTT eTrigger */
	//int										inofParam;			/**< No. of sensor_multilevel get parameters parsed from category */
	//zwp_avi_ifttt_sensor_multilevel_get_param_single_t	sensor_multilevel_param[2];		/**< Z-Wave sensor_multilevel get parameters */
} zwp_avi_ifttt_sensor_multilevel_get_t;





/**
* @brief HCAPI callback for Supported Report command
* @param[in]	  ifd	            interface
* @param[in]	  type_len          size of sensor type buffer
* @param[in]	  type              buffer to store supported sensor types (ZW_SENSOR_TYPE_XXX)
* @return        Nothing
*/
void zwp_avi_interfaces_sensor_multilevel_supported_report_handler(zwifd_p ifd, uint8_t type_len, uint8_t *type, int valid);

/**
* @brief Allocate interface level data for IFTTT
* @param[in]	 desc_interface    Interface descriptor
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zwp_avi_interfaces_sensor_multilevel_state_free_ifttt() to free resources
*/
zwp_status_t zwp_avi_interfaces_sensor_multilevel_state_alloc_ifttt(zwifd_p desc_interface);

/**
* @brief Free interface level data
* @param[in]     state               Interface level data
* @return        Nothing
* @see zwp_avi_interfaces_sensor_multilevel_state_alloc_ifttt
*/
void zwp_avi_interfaces_msensor_state_free_ifttt(zwp_avi_interfaces_sensor_multilevel_state_t *state);

/**
* @brief Free ifttt trigger get result
* @param[in]     pifttt_msensor_get_result      Pointer to IFTTT get result struct
* @return        Nothing
*/
void zwp_avi_interfaces_sensor_multilevel_ifttt_get_result_free_ifttt(zwp_avi_ifttt_sensor_multilevel_get_result_t *pifttt_msensor_get_result);

/**
* @brief HCAPI callback for Report command
* @param[in]	  ifd	            interface
* @param[in]	  data	            current value and unit of the sensor
* @param[in]	  ts      			time stamp
* @return        Nothing
*/
void zwp_avi_interfaces_msensor_report_handler_ifttt(zwifd_p ifd, zwsensor_p data, time_t ts);

/**
* @brief Convert Z-Ware Lib sensor value to double value
* @param[in]     sensor_value      Z-Ware Lib sensor value
* @param[out]    sensor_value_double_p      Double value
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
zwp_status_t zwh_interface_sensor_value_to_double(zwsensor_t *sensor_value, double *sensor_value_double_p, int *sensor_value_precision_p);

#endif /* ZWP_AVI_INTERFACES_SENSOR_MULTILEVEL_H_ */