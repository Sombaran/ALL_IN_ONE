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
 * @file    zws_ifttt_types.h
 *
 * @brief   Type defines for IFTTT 
 * @details Common header pertaining to IFTTT type define details
 *
 * @author  Angela Yan Yan
 *
 * @version 1.0 - 2016-07-21
 * - Initial version
 */

#ifndef ZWS_IFTTT_TYPES_H_
#define ZWS_IFTTT_TYPES_H_

#include "zws_ifttt_common.h"
/**
 * \addtogroup  zweb_ifttt
 * @{
 */




/**IFTTT Trigger param define/details */
static ifttt_uri_detail_t	arIFTTT_Triggers [] =
{
	//eifttt_triggers_actions			//sTriggerInterfaceSlug		//eTriggerField			//sTriggerFieldSlug
	//{ eTrigger_bSensor_On,				"bsensor_on",				eDevice_name,			"device_name" },
	//{ eTrigger_bSensor_Off,				"bsensor_off",				eDevice_name,			"device_name" },
	{ eTrigger_motionAlarm_Detected,	"motionalarm_detected",		eDevice_name,			"device_name" },
	{ eTrigger_motionAlarm_Detected,	"motionalarm_detected",		eNo_fields,				"" },
	{ eTrigger_motionAlarm_Clear,		"motionalarm_clear",		eDevice_name,			"device_name" },
	{ eTrigger_motionAlarm_Clear,		"motionalarm_clear",		eNo_fields,				"" },
	{ eTrigger_smokeAlarm_Detected,		"smokealarm_detected",		eDevice_name,			"device_name" },
	{ eTrigger_smokeAlarm_Detected,		"smokealarm_detected",		eNo_fields,				"" },
	{ eTrigger_smokeAlarm_Clear,		"smokealarm_clear",			eDevice_name,			"device_name" },
	{ eTrigger_smokeAlarm_Clear,		"smokealarm_clear",			eNo_fields,				"" },
	{ eTrigger_coAlarm_Detected,		"coalarm_detected",			eDevice_name,			"device_name" },
	{ eTrigger_coAlarm_Detected,		"coalarm_detected",			eNo_fields,				"" },
	{ eTrigger_coAlarm_Clear,			"coalarm_clear",			eDevice_name,			"device_name" },
	{ eTrigger_coAlarm_Clear,			"coalarm_clear",			eNo_fields,				"" },
	{ eTrigger_co2Alarm_Detected,		"co2alarm_detected",		eDevice_name,			"device_name" },
	{ eTrigger_co2Alarm_Detected,		"co2alarm_detected",		eNo_fields,				"" },
	{ eTrigger_co2Alarm_Clear,			"co2alarm_clear",			eDevice_name,			"device_name" },
	{ eTrigger_co2Alarm_Clear,			"co2alarm_clear",			eNo_fields,				"" },
	{ eTrigger_waterAlarm_Detected,		"wateralarm_detected",		eDevice_name,			"device_name" },
	{ eTrigger_waterAlarm_Detected,		"wateralarm_detected",		eNo_fields,				"" },
	{ eTrigger_waterAlarm_Clear,		"wateralarm_clear",			eDevice_name,			"device_name" },
	{ eTrigger_waterAlarm_Clear,		"wateralarm_clear",			eNo_fields,				"" },
	{ eTrigger_contactAlarm_Open,		"contactalarm_open",		eDevice_name,			"device_name" },
	{ eTrigger_contactAlarm_Open,		"contactalarm_open",		eNo_fields,				"" },
	{ eTrigger_contactAlarm_Closed,		"contactalarm_closed",		eDevice_name,			"device_name" },
	{ eTrigger_contactAlarm_Closed,		"contactalarm_closed",		eNo_fields,				"" },
	{ eTrigger_contactAlarm_Closed,		"contactalarm_closed",		eDevice_name,			"device_name" },
	{ eTrigger_contactAlarm_Closed,		"contactalarm_closed",		eNo_fields,				"" },
	{ eTrigger_mSensor_Temp_LT,			"msensor_temp_lt",			eDevice_name,			"device_name" },
	//{ eTrigger_mSensor_Temp_LT,			"msensor_temp_lt",			eThreshold_value,		"thld_value" },
	//{ eTrigger_mSensor_Temp_LT,			"msensor_temp_lt",			eThreshold_unit,		"thld_unit" },
	{ eTrigger_mSensor_Temp_LT,			"msensor_temp_lt",			eNo_fields,				"" },
	{ eTrigger_mSensor_Temp_GT,			"msensor_temp_gt",			eDevice_name,			"device_name" },
	//{ eTrigger_mSensor_Temp_GT,			"msensor_temp_gt",			eThreshold_value,		"thld_value" },
	//{ eTrigger_mSensor_Temp_GT,			"msensor_temp_gt",			eThreshold_unit,		"thld_unit" },
	{ eTrigger_mSensor_Temp_GT,			"msensor_temp_gt",			eNo_fields,				"" },
	{ eTrigger_mSensor_Luminance_LT,	"msensor_lum_lt",			eDevice_name,			"device_name" },
	//{ eTrigger_mSensor_Luminance_LT,	"msensor_lum_lt",			eThreshold_value,		"thld_value" },
	//{ eTrigger_mSensor_Luminance_LT,	"msensor_lum_lt",			eThreshold_unit,		"thld_unit" },
	{ eTrigger_mSensor_Luminance_LT,	"msensor_lum_lt",			eNo_fields,				"" },
	{ eTrigger_mSensor_Luminance_GT,	"msensor_lum_gt",			eDevice_name,			"device_name" },
	//{ eTrigger_mSensor_Luminance_GT,	"msensor_lum_gt",			eThreshold_value,		"thld_value" },
	//{ eTrigger_mSensor_Luminance_GT,	"msensor_lum_gt",			eThreshold_unit,		"thld_unit" },
	{ eTrigger_mSensor_Luminance_GT,	"msensor_lum_gt",			eNo_fields,				"" },
	{ eTrigger_mSensor_Humidity_LT,		"msensor_hum_lt",			eDevice_name,			"device_name" },
	//{ eTrigger_mSensor_Humidity_LT,	"msensor_hum_lt",			eThreshold_value,		"thld_value" },
	//{ eTrigger_mSensor_Humidity_LT,	"msensor_hum_lt",			eThreshold_unit,		"thld_unit" },
	{ eTrigger_mSensor_Humidity_LT,		"msensor_hum_lt",			eNo_fields,				"" },
	{ eTrigger_mSensor_Humidity_GT,		"msensor_hum_gt",			eDevice_name,			"device_name" },
	//{ eTrigger_mSensor_Humidity_GT,	"msensor_hum_gt",			eThreshold_value,		"thld_value" },
	//{ eTrigger_mSensor_Humidity_GT,	"msensor_hum_gt",			eThreshold_unit,		"thld_unit" },
	{ eTrigger_mSensor_Humidity_GT,		"msensor_hum_gt",			eNo_fields,				"" }

};


/**IFTTT Action param define/details */
static ifttt_uri_detail_t	arIFTTT_Actions [] =
{
	//eIFTTT_IF_feature					//sIFTTTInterfaceSlug		//eIFTTTField			//sIFTTTFieldSlug
	//eifttt_triggers_actions			//sActionInterfaceSlug		//eActionField			//sActionFieldSlug
	{ eAction_bSwitch_On,				"bswitch_on",				eDevice_name,			"device_name" },
	{ eAction_bSwitch_On,				"bswitch_on",				eNo_fields,				""			  },
	{ eAction_bSwitch_Off,				"bswitch_off",				eDevice_name,			"device_name" },
	{ eAction_bSwitch_Off,				"bswitch_off",				eNo_fields,				""			  },
	{ eAction_dLock_Open,				"dlock_open",				eDevice_name,			"device_name" },
	{ eAction_dLock_Open,				"dlock_open",				eNo_fields,				""			  },
	{ eAction_dLock_Close,				"dlock_close",				eDevice_name,			"device_name" },
	{ eAction_dLock_Close,				"dlock_close",				eNo_fields,				""			  },
	{ eAction_mSwitch_Dimmer,			"mswitch_dimmer",			eDevice_name,			"device_name" },
	{ eAction_mSwitch_Dimmer,			"mswitch_dimmer",			eDevice_state,			"device_state"},
	{ eAction_mSwitch_Dimmer,			"mswitch_dimmer",			eNo_fields,				""			  },
	{ eAction_thrmoMode_Off,			"thrmomode_off",			eDevice_name,			"device_name" },
	{ eAction_thrmoMode_Off,			"thrmomode_off",			eNo_fields,				""			  },
	{ eAction_thrmoMode_Heat,			"thrmomode_heat",			eDevice_name,			"device_name" },
	{ eAction_thrmoMode_Heat,			"thrmomode_heat",			eNo_fields,				""			  },
	{ eAction_thrmoMode_Cool,			"thrmomode_cool",			eDevice_name,			"device_name" },
	{ eAction_thrmoMode_Cool,			"thrmomode_cool",			eNo_fields,				""			  },
	{ eAction_thrmoMode_Auto,			"thrmomode_auto",			eDevice_name,			"device_name" },
	{ eAction_thrmoMode_Auto,			"thrmomode_auto",			eNo_fields,				""			  }
};

/** }@ */
#endif /* ZWS_IFTTT_TYPES_H_ */
