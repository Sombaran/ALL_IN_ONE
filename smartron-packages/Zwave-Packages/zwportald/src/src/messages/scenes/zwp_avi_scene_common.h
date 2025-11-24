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
 * @file    zwp_avi_scene_common.h
 *
 * @brief   Common header file for Scenes AVI messages
 * @details Common header file for Scenes AVI messages.
 *
 * @author  Arif
 *
 * @version 1.0 - 2015-05-26
 * - Initial version
 */
#ifndef ZWP_AVI_SCENE_COMMON_H_
#define ZWP_AVI_SCENE_COMMON_H_

/**
 * \addtogroup  zwportaldmessages
 * @{
 */

#define ZWP_ZWAVE_SCENE_EVENT_ARG_NOT_FOUND	(-2)
 
/**
@brief Convert interface descriptor id to scene action/event id
@param[in]	ifd	            Interface descriptor id
@return Scene action/event id
*/
uint32_t zwp_ifd_2_scn_act_evt_id(uint32_t ifd);
/**
@brief Convert scene action/event id to interface descriptor id
@param[in]	act_evt_id	   Scene action/event id
@return Interface descriptor id
*/
uint32_t zwp_scn_act_evt_id_2_ifd(uint32_t act_evt_id);
/**
@brief AVRO argument parser
@param[in]	arg_value	    Parameters encoded in AVRO
@param[out]	args_p	        Decoded AVRO parameters
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_avi_scene_generic_arg_handler(avro_value_t *arg_value, void **args_p); 
/**
@brief Free the data structure allocated in zwp_avi_scene_generic_arg_handler
@param[in]	args	            Data structure to be freed
@return
*/
void zwp_avi_scene_generic_arg_free(void *args);
/**
@brief Encode the response to AVRO format
@param[in]	result	            Result
@param[out]	arg_value	        Output in AVRO object
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_avi_scene_generic_result(void *result, avro_value_t *arg_value);
/**
@brief Free the result_p from zwp_avi_XXX_execute()
@param[in]	result	            Result pointer
@return
*/
void zwp_avi_scene_generic_result_free(void *result);
/**
@brief Get URI as specified by command class
@param[in]	cmd_cls             Command class as defined by Z-wave
@return URI if successful; else return NULL
*/
const char *zwp_avi_uri_get(uint16_t cmd_cls);
/**
@brief Convert the param into AVRO format
@param[in]	param               Parameter data
@param[in]	cmd_cls             Command class as defined by Z-wave
@param[in]	cmd                 Command as defined by Z-Ware Web Developer's Guide
@param[out]	action_value	    Output in AVRO object
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_avi_param_to_avro(void *param, uint16_t cmd_cls, uint8_t cmd, avro_value_t *action_value);
/**
@brief get integer value from a byte stream
@param[in]	byte_buf    The buffer that holds the bytes
@param[in]	size		The integer size
@param[out]	int_val	    The result of the conversion from bytes to integer
@return     1 on success; else return 0
*/
int  zwp_avi_scene_int_get(uint8_t *byte_buf,  uint8_t size, int32_t *int_val);
/**
@brief Convert the param list from AVRO to data structure
@param[in]	param_list_value    AVRO encoded param list
@param[in]	param_cnt           Number of elements in param list
@param[in]	cmd_cls             Command class as defined by Z-wave
@param[in]	cmd                 Command as defined by Z-Ware Web Developer's Guide
@param[out]	param	            Output data structure
@param[out]	param_sz	        Output data structure size
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_avi_param_list_get(avro_value_t *param_list_value, int param_cnt, uint16_t cmd_cls, uint8_t cmd, void **param, uint32_t *param_sz);
/**
@brief		Function to validate device args (considering CC version)
@param[in] 	net		Network runtime context
@param[in]	event	Scene event
@return 1 (true) or 0 (false)
*/
int zwp_avi_scene_event_validate_device_args(zwp_net_t net, zwscn_event_t event);
 /** }@ */
#endif /* ZWP_AVI_SCENE_COMMON_H_ */