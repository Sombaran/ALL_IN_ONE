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
 * @file    zwp_ifttt_common.h
 *
 * @brief   IFTTT Common
 * @details Common header pertaining to IFTTT types.
 *
 * @author  Angela Yan Yan
 *
 * @version 1.0 - 2016-07-29
 * - Initial version
 */

#ifndef ZWP_IFTTT_COMMON_H_
#define ZWP_IFTTT_COMMON_H_

#include "zwave/ZW_classcmd.h"
/** @defgroup zwportal_ifttt
 *
 * Common defines for IFTTT.
 *
 */

/**
 * \addtogroup  zwportal_ifttt
 * @{
 */

/** IFTTT triggers and action enum */
//Note: Any new types added here must update arIFTTT_feature_2_cls[]
typedef enum _eifttt_IF_feature_type
{
	//----------Type none--------------
	eifttt_IF_feature_type_none = 0,

	//----------Triggers--------------
	eTrigger_bSensor_On = 1,
	eTrigger_bSensor_Off,
	eTrigger_motionAlarm_Detected,
	eTrigger_motionAlarm_Clear,
	eTrigger_smokeAlarm_Detected,
	eTrigger_smokeAlarm_Clear,
	eTrigger_coAlarm_Detected,
	eTrigger_coAlarm_Clear,
	eTrigger_co2Alarm_Detected,
	eTrigger_co2Alarm_Clear,
	eTrigger_waterAlarm_Detected,
	eTrigger_waterAlarm_Clear,
	eTrigger_contactAlarm_Open,
	eTrigger_contactAlarm_Closed,
	eTrigger_mSensor_Temp_LT,
	eTrigger_mSensor_Temp_GT,
	eTrigger_mSensor_Luminance_LT,
	eTrigger_mSensor_Luminance_GT,
	eTrigger_mSensor_Humidity_LT,
	eTrigger_mSensor_Humidity_GT,
	eTrigger_IF_feature_all,

	//----------Actions--------------
	eAction_bSwitch_On = 101,
	eAction_bSwitch_Off,
	eAction_dLock_Open,
	eAction_dLock_Close,
	eAction_mSwitch_Dimmer,
	eAction_thrmoMode_Off,
	eAction_thrmoMode_Heat,
	eAction_thrmoMode_Cool,
	eAction_thrmoMode_Auto,
	eAction_IF_feature_all
}eifttt_IF_feature_type;


/** IFTTT trigger or action fields */
typedef enum _eifttt_fields
{
	eNo_fields,
	eDevice_name

}eifttt_fields;


/** IFTTT URI */
typedef struct _ifttt_uris
{
	int							iRequestType;					/**< IFTTT_REQUEST_TYPE_xxx */

	eifttt_IF_feature_type		eRequest_IF_feature;			/**< Request interface with feature in enum type*/
	char*						pchRequestIF;					/**< Request interface. Trigger slug / action slug */

	int							iRequestSubType;				/**< IFTTT_REQUEST_SUBTYPE_xxx */

	eifttt_fields				eRequest_IF_field;				/**< Request interface field in enum type*/
	char*						pchRequestIFField;				/**< Request interface field. Trigger field slug / action field slug*/
}ifttt_uri_t, *pifttt_uri_t;

typedef struct _ifttt_args
{
	ifttt_uri_t stifttt_uri;

}ifttt_args_t, *pifttt_args_t;



/** IFTTT trigger Details */
typedef struct _ifttt_uri_detail
{
	eifttt_IF_feature_type			eIFTTT_IF_feature;        /**< IFTTT interface and feature */
	const char*						sIFTTTInterfaceSlug;      /**< The IFTTT slug defined on IFTTT channel platform */
	eifttt_fields					eIFTTTField;              /**< IFTTT interface field */
	const char*						sIFTTTFieldSlug;          /**< IFTTT interface field slug */
} ifttt_uri_detail_t, *pifttt_uri_detail;


/** Private to zwportald */
/** IFTTT feature to command class */
typedef struct _ifttt_feature_cls
{
	eifttt_IF_feature_type			eIFTTT_IF_feature;        /**< IFTTT interface and feature */
	uint16_t						wZwave_cls_id;			  /**< ZWave command class id */
	int								iNofParam;				  /**< Number of additional param for this efeature*/
	int								iParam;					  /**< Additional param (currently only support 1) */
} ifttt_feature_cls_t;


/** }@ */
#endif /* ZWP_IFTTT_COMMON_H_ */
