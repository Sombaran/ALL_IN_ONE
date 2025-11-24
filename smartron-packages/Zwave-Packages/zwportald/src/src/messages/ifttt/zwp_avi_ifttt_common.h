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
 * @file    zwp_avi_ifttt_common.h
 *
 * @brief   Common header file for IFTTT AVI messages
 * @details Common header file for IFTTT AVI messages.
 *
 * @author  Angela
 *
 * @version 1.0 - 2016-08-02
 * - Initial version
 */
#ifndef ZWP_AVI_IFTTT_COMMON_H_
#define ZWP_AVI_IFTTT_COMMON_H_

#include <stdint.h>

#include "zwp_status.h"
#include "zw_avro.h"
#include "zwp_ifttt_common.h"
#include "zwp_network_define.h"
#include "zwu_array_list.h"
#include "zwp_network.h"

/** Maximum trigger ID length */
#define MAX_TRIGGER_ID_LENGTH				300

 /** alarm option argument */
typedef struct
{
	char			striggerID[MAX_TRIGGER_ID_LENGTH];
	int				iLimit;
} zwp_avi_ifttt_trigger_get_t;

#define IFTTT_DEFAULT_TRIGGER_RECORD_LIMIT	50

/**IFTTT CC specific types/categories */

/**Multilevel switch type */
#define IFTTT_MSWITCH_TYPE_DIMMER	1

/** Alarm "category" definition */
#define ZW_ALARM_CATEGORY_MOTION            1       /**< Motion alarm detected/clear */
#define ZW_ALARM_CATEGORY_SMOKE				2		/**< Smoke alarm detected/clear */
#define ZW_ALARM_CATEGORY_CO				3       /**< CO alarm detected/clear */
#define ZW_ALARM_CATEGORY_CO2				4		/**< CO2 alarm detected/clear */
#define ZW_ALARM_CATEGORY_WATER				5		/**< Water alarm detected/clear */
#define ZW_ALARM_CATEGORY_CONTACT           6       /**< Contact alarm detected/clear */


/**
@brief Return the corresponding ZWave command class ID based on IFTTT interface feature.
@param[in]	aefeature				IFTTT interface feature enum.
@param[out]	pcls_id					Corresponding ZWave command class ID
@param[out]	piNofParam				Corresponding number of additional params
@param[out]	piParam					Corresponding additional params
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_ifttt_find_cls_from_efeature(eifttt_IF_feature_type aefeature, uint16_t *pcls_id, int *piNofParam, int* piParam);

/**
* @brief Set Avro value object with values from Result object corresponding to network operation state
* @param[in]     result            Result object
* @param[out]    arg_value         Avro value object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
zwp_status_t zwp_avi_ifttt_device_name_option_result(void *result, avro_value_t *arg_value);

/**
* @brief Set Avro value object with endpoint name and location
* @param[in]     name			   Endpoint name
* @param[in]     loc			   Endpoint location
* @param[out]    arg_value         Avro value object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
zwp_status_t zwp_avi_ifttt_get_result_assign_ep_name_loc(char *name, char *loc, avro_value_t *arg_value);

/**
* @brief Check whether the thermostat mode interface has a specific mode
* @param[in]     desc_interface    Interface descriptor
* @param[in]     mode			   The thermostat mode to look for
* @retval        0 - interface does not support this mode; 1 - interface supports this mode
*/
int zwp_ifttt_interface_has_supported_thermostat_mode(zwifd_p desc_interface, int mode);

/**
* @brief Check whether device endpoint is a correct multilevel switch type
* @param[in]     desc_net          Network descriptor object
* @param[in]     desc_interface    Interface descriptor
* @param[in]     type			   The different device type for multilevel switch
* @retval        0 - interface does not support this mode; 1 - interface supports this mode
*/
int zwp_ifttt_interface_has_supported_multilevel_switch_device(zwp_desc_net_t desc_net, zwifd_p desc_interface, int type);

/**
@brief Check the interface device list and modify it according to the additional param requirement.
@param[in]	aefeature				IFTTT interface feature enum.
@param[in, out]	if_ep_name_list		Device interface list that supports the command class
@param[in]  desc_net				Network descriptor object
@param[in]	iNofParam				Number of additional params
@param[in]	iParam					Additional params
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_ifttt_check_device_list_with_additional_param(eifttt_IF_feature_type aefeature, 
																zwu_array_list_t if_ep_name_list, 
																zwp_desc_net_t desc_net,
																int iNofParam, 
																int iParam);

/**
* @brief Check whether the alarm interface has supported type/event
* @param[in]     desc_interface    Interface descriptor
* @param[in]     category		   The notification category to look for
* @retval        0 - interface does not support this category; 1 - interface supports this category
*/
int zwp_ifttt_interface_has_supported_alarm_type_event(zwifd_p desc_interface, int category);

/**
* @brief Check whether the multilevel interface has supported type
* @param[in]     desc_interface    Interface descriptor
* @param[in]     type			   The multilevel type to look for
* @retval        0 - interface does not support this type; 1 - interface supports this type
*/
int zwp_ifttt_interface_has_supported_msensor_type(zwifd_p desc_interface, int type);

/**
* @brief Get trigger id argument for trigger operation from Avro value object
* @param[in]     arg_value         Avro value object
* @param[out]    ptrigger_id_data  Trigger ID data
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
zwp_status_t zwp_avi_ifttt_get_trigger_data_argument(avro_value_t *arg_value, zwp_avi_ifttt_trigger_get_t *ptrigger_id_data);
#endif /* ZWP_AVI_IFTTT_COMMON_H_ */