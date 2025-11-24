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
 * @file    zws_avi_ifttt_common.h
 *
 * @brief   Common header file for IFTTT AVI messages.
 * @details Common header file for IFTTT AVI messages.
 *
 * @author  Angela
 *
 * @version 1.0 - 2016-08-04
 * - Initial version
 */
#ifndef ZWS_AVI_IFTTT_COMMON_H_
#define ZWS_AVI_IFTTT_COMMON_H_

#include <stdint.h>

#include "zws_status.h"
#include "zw_avro.h"
#include "zws_ifttt_response.h"

 /**
 * \addtogroup  zwebmessages
 * @{
 */

/**
@brief Generate the device label in encoded format for IFTTT based on given info
@param[in]	if_id	            Interface id
@param[in]	node_id				Node id
@param[in]	endpoint_id			Endpoint id
@param[in]	name				Device name
@param[in]	location			Device location
@param[out]	pGenLabel	        Pointer to generated label
@return ZWS_STATUS_XXX
@note Caller of this function needs to free pGenLabel if the funtion returns success
*/
zws_status_t zws_avi_generate_device_label(int32_t if_id, 
										   int32_t node_id, 
										   int32_t endpoint_id, 
										   char *name, 
										   char *location,
										   char **pGenLabel);

/**
@brief Generate the device/interface value for IFTTT based on given info
@param[in]	if_id	            Interface id
@param[in]	node_id				Node id
@param[in]	endpoint_id			Endpoint id
@param[out]	pGenValue	        Pointer to generated label
@return ZWS_STATUS_XXX
@note Caller of this function needs to free pGenValue if the funtion returns success
*/
zws_status_t zws_avi_generate_device_value(int32_t if_id,
											int32_t node_id,
											int32_t endpoint_id,
											char **pGenValue);

/**
* @brief Get CGI response for IFTTT IF device name options API from Avro value for RPC response
* @param[in]     arg_value         Avro value for RPC response
* @param[in]     padditional_info  Additional information to aid to generate the result
* @param[out]    jsData            CGI response json data
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_avi_network_ifttt_device_name_option_result(avro_value_t *arg_value, void *padditional_info, zwu_json_t jsData);


/**
@brief Find the interface slug and field slug based on efeature
@param[in]	iTriggerActionFlag	IFTTT_REQUEST_TYPE_TRIGGER or IFTTT_REQUEST_TYPE_ACTION
@param[in]	efeature			Interface feature enum
@param[out]	pchInterfaceSlug	Pointer to Interface Slug
@param[out]	pGenValue	        Pointer to Field Slug
@return ZWS_STATUS_XXX
@note Caller of this function needs to free pchInterfaceSlug and pchFieldSlug if the funtion returns success
*/
zws_status_t zws_avi_ifttt_get_interfaceslug(int iTriggerActionFlag,
											eifttt_IF_feature_type efeature,
											char **pchInterfaceSlug,
											char **pchFieldSlug);

/**
* @brief Get CGI response for IFTTT Action set result API from Avro value for RPC response
* @param[in]     arg_value         Avro value for RPC response
* @param[in]     padditional_info  Additional information to aid to generate the result
* @param[out]    jsData            CGI response json data
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_avi_network_ifttt_action_set_result(avro_value_t *arg_value, void *padditional_info, zwu_json_t jsData);

/**
* @brief Set Avro value for RPC request for generic IFTTT trigger data using CGI request arguments
* @param[in, out]	param_value_p     avro API param value
* @param[in]		stifttt_uri       IFTTT URI request
* @param[in]		stifttt_args      IFTTT request parameters
* @retval			ZWS_STATUS_OK     Success
* @retval			ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_avi_network_ifttt_trigger_get_data_argument(avro_value_t *param_value_p, ifttt_uri_t stifttt_uri, ifttt_args_t stifttt_args);

/**
* @brief Get CGI response for IFTTT Trigger get API from Avro value for RPC response
* @param[in]     arg_value         Avro value for RPC response
* @param[in]     padditional_info  Additional information to aid to generate the result
* @param[out]    jsData            CGI response json data
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_avi_network_ifttt_trigger_get_result(avro_value_t *arg_value, void *padditional_info, zwu_json_t jsData);

/**
* @brief Generate device name for IFTTT trigger get result based on combo_id and device name/loc from Avro response
* @param[in]     combo_id		   Combo id
* @param[in]     arg_value         Avro value for RPC response
* @param[out]    arDeviceName      Char array which points to the generated device name
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_avi_interface_ifttt_get_device_name(unsigned long combo_id, avro_value_t *arg_value, char* arDeviceName);


/** }@ */
#endif /* ZWS_AVI_IFTTT_COMMON_H_ */