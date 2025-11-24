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
 * @file    zws_ifttt_common.h
 *
 * @brief   IFTTT Common
 * @details Common header pertaining to IFTTT channel details.
 *
 * @author  Angela Yan Yan
 *
 * @version 1.0 - 2016-06-20
 * - Initial version
 */

#ifndef ZWS_IFTTT_COMMON_H_
#define ZWS_IFTTT_COMMON_H_

#ifndef LOCAL_CLIENT_ENABLE
#include "zwu_json.h"
#endif
//#include "zws_ifttt_types.h"

/** @defgroup zweb_ifttt
 *
 * Common defines for IFTTT.
 *
 */

/**
 * \addtogroup  zweb_ifttt
 * @{
 */

/** IFTTT Channel key value for AWS 1*/
#define IFTTT_CHANNEL_KEY_VALUE				"6VNVn0J086webgSsVDTtPoZmGpOQGeCg8ghBTKbDtAZ1mpmeRoWJeWZ2ESW1NWDS" 

/** IFTTT Channel key value for AWS 2 - testing channel*/
#define IFTTT_CHANNEL_KEY_VALUE2			"-xXuz4lnLR4PlJ2EPcO9XS8oibHSr-kyLOh251Sf0FyG4lGcFtNemBKG3zUULTRg" 


/** IFTTT Channel key key string in request header */
#define IFTTT_CHANNEL_KEY_HEADER			"HTTP_IFTTT_CHANNEL_KEY"

/** IFTTT Authorization string in request header (for bearer token) */
#define IFTTT_AUTHORIZATION_HEADER			"HTTP_AUTHORIZATION"

/** IFTTT Access token string Bearer keywork in request header (for bearer token) */
#define IFTTT_AUTHORIZATION_HEADER_BEARER	"Bearer "

/** Supported IFTTT API version string in URL */
#define IFTTT_API_VERSION1					"v1"

/** IFTTT status message (exception handling as this request does not require user name) */
#define IFTTT_REQUEST_STATUS				"/status"

/** IFTTT test/setup message (exception handling as this request does not require user name) */
#define IFTTT_REQUEST_TEST_SETUP			"/test/setup"

/** IFTTT user/info message */
#define IFTTT_REQUEST_USER_INFO				"/user/info"

/** IFTTT request triggers word without '/' */
#define IFTTT_REQUEST_TRIGGER_WORD			"triggers"

/** IFTTT request actions word without '/' */
#define IFTTT_REQUEST_ACTION_WORD			"actions"

/** IFTTT request fields word without '/' */
#define IFTTT_REQUEST_FIELDS_WORD			"fields"

/** IFTTT request options word without '/' */
#define IFTTT_REQUEST_OPTIONS_WORD			"options"

/** IFTTT request validate word without '/' */
#define IFTTT_REQUEST_VALIDATE_WORD			"validate"

/** IFTTT command prefix for all the cgi commands */
#define IFTTT_COMMAND_PREFIX				"ifttt_"

/** The following are the "known" IFTTT post body arguments */
/** IFTTT request body argument trigger_identity */
#define IFTTT_REQUEST_BODY_TRIGGER_ID		"trigger_identity"

/** IFTTT request body argument triggerFields */
#define IFTTT_REQUEST_BODY_TRIGGER_FIELDS	"triggerFields"

/** IFTTT request body argument actionFields */
#define IFTTT_REQUEST_BODY_ACTION_FIELDS	"actionFields"

/** IFTTT request body argument limit */
#define IFTTT_REQUEST_BODY_LIMIT			"limit"

/** IFTTT request body argument user */
#define IFTTT_REQUEST_BODY_USER				"user"

/** IFTTT request body argument ifttt_source */
#define IFTTT_REQUEST_BODY_IFTTT_SRC		"ifttt_source"

/** IFTTT request body argument device_name */
#define IFTTT_REQUEST_BODY_IFTTT_DEVICE_NAME		"device_name"

/** IFTTT request body argument device_state */
#define IFTTT_REQUEST_BODY_IFTTT_DEVICE_STATE		"device_state"

/** IFTTT request body argument thld_value */
#define IFTTT_REQUEST_BODY_IFTTT_THLD_VALUE			"thld_value"

/** IFTTT request body argument thld_unit */
#define IFTTT_REQUEST_BODY_IFTTT_THLD_UNIT			"thld_unit"

/** IFTTT demo user/test user ID */
#define IFTTT_TEST_USER_ID					"iftttdemouser"


/** Macro to construct ifttt command names for avi request. Format: "ifttt_xxx" */
#define CONSTRUCT_IFTTT_COMMAND_NAME(command_name, uri)						\
{																			\
	snprintf(command_name, ZWS_MAX_COMMAND_NAME_LENGTH, "%s%s", IFTTT_COMMAND_PREFIX, uri);		\
}

/** Generic IFTTT request type, non-trigger, non-action. Eg. status, /user/info etc */
#define IFTTT_REQUEST_TYPE_GENERAL			0	
#define IFTTT_REQUEST_TYPE_TRIGGER			1
#define IFTTT_REQUEST_TYPE_ACTION			2

#define IFTTT_REQUEST_SUBTYPE_NONE			0
#define IFTTT_REQUEST_SUBTYPE_OPTION		11
#define IFTTT_REQUEST_SUBTYPE_VALIDATE		12


/** IFTTT response type. No response needed for both success and failure case. */
#define IFTTT_RESPONSE_TYPE_NONE			0	
/** IFTTT response type. Json response includes "Data" object */
#define IFTTT_RESPONSE_TYPE_DATA_OBJECT		1
/** IFTTT response type. Json response includes "Data" array */
#define IFTTT_RESPONSE_TYPE_DATA_ARRAY		2


/** IFTTT triggers and action enum */
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
	eDevice_name,
	eDevice_state,
	eThreshold_value,
	eThreshold_unit

}eifttt_fields;





/** IFTTT URI */
typedef struct _ifttt_uris
{
#ifndef LOCAL_CLIENT_ENABLE
	int							iRequestType;					/**< IFTTT_REQUEST_TYPE_xxx */

	eifttt_IF_feature_type		eRequest_IF_feature;			/**< Request interface with feature in enum type*/
	char*						pchRequestIF;					/**< Request interface. Trigger slug / action slug */

	int							iRequestSubType;				/**< IFTTT_REQUEST_SUBTYPE_xxx */

	eifttt_fields				eRequest_IF_field;				/**< Request interface field in enum type*/
	char*						pchRequestIFField;				/**< Request interface field. Trigger field slug / action field slug*/
#endif
}ifttt_uri_t, *pifttt_uri_t;

/** IFTTT request post data/arguments */
typedef struct _ifttt_args
{
#ifndef LOCAL_CLIENT_ENABLE
	char*						pchTrigger_identity;			/**< String format Trigger identity (Only valid in Trigger request)*/
	zwu_json_t					jsFields_obj;					/**< JSon format Trigger/Action Fields (Only valid in Trigger/Action request)*/
	int							iLimit;							/**< Integer format limit (Only valid and optional, in Trigger request)*/
	
	zwu_json_t					jsUser_obj;						/**< JSon format User Object (Only valid in Trigger/Action request)*/
	zwu_json_t					jsIFTTT_src_obj;				/**< JSon format IFTTT source Object (Only valid in Trigger/Action request)*/
	zwu_json_t					jsFull_args_obj;				/**< JSon format entire post data (Valid for all non-empty request)*/
#endif
}ifttt_args_t, *pifttt_args_t;



/** IFTTT trigger Details */
typedef struct _ifttt_uri_detail
{
	eifttt_IF_feature_type			eIFTTT_IF_feature;        /**< IFTTT interface and feature */
	const char*						sIFTTTInterfaceSlug;      /**< The IFTTT slug defined on IFTTT channel platform */
	eifttt_fields					eIFTTTField;              /**< IFTTT interface field */
	const char*						sIFTTTFieldSlug;          /**< IFTTT interface field slug */
	//uint16_t						wInterfaceid;			  /**< Command Class ID for the IFTTT interface */
} ifttt_uri_detail_t, *pifttt_uri_detail;


///** IFTTT action Details */
//typedef struct _ifttt_action_detail
//{
//	eifttt_actions          eActionInterface;          /**< IFTTT Action interface and feature */
//	const char*				sActionInterfaceSlug;      /**< The IFTTT Action slug defined on IFTTT channel platform */
//	eifttt_fields			eActionField;              /**< IFTTT Action interface field */
//	const char*				sActionFieldSlug;          /**< IFTTT Action interface field slug */
//} ifttt_action_detail_t, *pifttt_action_detail;

//#define PPCAT_NX(A, B) A ## B
//#define PPCAT(A, B) PPCAT_NX(A, B)
//
//#define STRINGIZE_NX(A) #A
//#define STRINGIZE(A) STRINGIZE_NX(A)

/** @defgroup zweb_ifttt_status_code
 *
 * HTTP error return code for IFTTT request
 *
 */

/**
* \addtogroup  zweb_ifttt_status_code
* @{
*/
/** There was something wrong with incoming data from IFTTT. 
    Provide an error response body to clarify what went wrong.*/ 
#define ZWS_STATUS_IFTTT_ERROR_PARAM				400

/** OAuth2 access token invalid */
#define ZWS_STATUS_IFTTT_ERROR_OA2_AT_INVALID		401

/** URL doesn't exist */
#define ZWS_STATUS_IFTTT_ERROR_NOT_FOUND			404

/** Error in Server logic */
#define ZWS_STATUS_IFTTT_ERROR_SERVER				500

/** Service unavailable at the momoent. IFTTT should try again later. */
#define ZWS_STATUS_IFTTT_ERROR_SERVICE_UNAVAILABLE	503

/** }@ */

/** @defgroup zweb_ifttt_response_msg
*
* IFTTT Response message defines
*
*/
/**
* \addtogroup  zweb_ifttt_response_msg
* @{
*/
/** Top-level response message json object name*/
#define IFTTT_RESPONSE_DATA_STR			"data"

/** Top-level response message json object name*/
#define IFTTT_RESPONSE_ERROR_STR		"errors"


/** IFTTT error messages - channel key not match */
#define IFTTT_ERROR_STR_AUTH_CHANNEL_KEY_NOT_MATCH		"IFTTT Channel Key in header does not match with record."

/** IFTTT error messages - IFTTT user not match */
#define IFTTT_ERROR_STR_AUTH_IFTTT_USER_NOT_FOUND		"IFTTT user does not exist in DB."

/** IFTTT error messages - access token in wrong format */
#define IFTTT_ERROR_STR_AUTH_ACCESS_TOKEN_WRONG_FORMAT	"Access Token in header has wrong format."

/** IFTTT error messages - cannot find matched user with access token */
#define IFTTT_ERROR_STR_AUTH_NO_MATCH_USER				"Access Token in header does not have matched user or expired."

/** IFTTT error messages - no channel key nor access token in header */
#define IFTTT_ERROR_STR_AUTH_NO_CHANNEL_KEY_NOR_TOKEN	"Neither IFTTT Channel Key nor Access Token is found in header."

/** IFTTT error messages - json format is invalid in request */
#define IFTTT_ERROR_STR_PARAM_JSON_INVALID				"Json format invalid in request."

/** IFTTT error messages - No trigger field or action field is found in request */
#define IFTTT_ERROR_STR_PARAM_NO_TRIGGER_ACTION_FIELD	"No trigger or action field is found in request."

/** IFTTT error messages - no device_name option found in trigger field or action field */
#define IFTTT_ERROR_STR_PARAM_NO_DEVICE_NAME_IN_TRIGGER_ACTION_FIELD	"No 'device_name' option found in request."

/** IFTTT error messages - 'device_name' is invalid in reqest */
#define IFTTT_ERROR_STR_PARAM_DEVICE_NAME_INVALID	"'device_name' string is invalid."


/** }@ */

/** @defgroup zweb_ifttt_error_msg
*
* HTTP body error messages for IFTTT request
*
*/
/**
* \addtogroup  zweb_ifttt_error_msg
* @{
*/
/** The API version used by the incoming request is not supported*/
#define IFTTT_ERROR_MSG_API_VER_NOT_SUPPORTED		"API version not supported"

/** }@ */
#endif /* ZWS_IFTTT_COMMON_H_ */
