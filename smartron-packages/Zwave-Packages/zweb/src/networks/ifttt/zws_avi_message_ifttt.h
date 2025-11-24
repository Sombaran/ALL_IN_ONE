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
 * @file    zws_avi_message_ifttt.h
 *
 * @brief   AVI messages - IFTTT Portion
 * @details Handles IFTTT portion of AVI messages.
 *
 * @author  Angela
 *
 * @version 1.0 - 2016-07-19
 * - Initial version
 */

#ifndef ZWS_AVI_MESSAGE_IFTTT_H_
#define ZWS_AVI_MESSAGE_IFTTT_H_

/**
 * \addtogroup  zwebavro
 * @{
 */

#include "zws_status.h"
#include "zwu_json.h"
#include "zws_avi_message.h"
#include "zw_avro.h"
#include "zwu_array_list.h"
#include "zws_ifttt_common.h"

/** Forward declaration for 'AVI ifttt message' class
 * @note    'zws_avi_message_ifttt_t' is not a pointer
 */
typedef struct _zws_avi_message_ifttt zws_avi_message_ifttt_t;

/**
* AVI ifttt message argument handlers
*/
typedef zws_status_t(*zws_avi_ifttt_message_argument_t)(ifttt_uri_t stifttt_uri, ifttt_args_t stifttt_args, avro_value_t *arg_value_p);

/**
* AVI ifttt message result handlers
*/
typedef zws_status_t(*zws_avi_ifttt_message_result_t)(avro_value_t *arg_value, void *padditional_info, zwu_json_t jsData);


/** AVI ifttt message class */
struct _zws_avi_message_ifttt
{
	const char							*cgi_uri;							/**< CGI URI */
	
	int									cgi_request_type;					/**< CGI request type */
	eifttt_IF_feature_type				cgi_e_IF_feature;					/**< CGI request interface and feature */
	//const char							*cgi_request_interface_feature;		/**< CGI request interface and feature */

	int									cgi_request_subtype[ZWS_IFTTT_SUBTYPE_VALUE];	/**< CGI request subtype and its type value */
	//const char							*cgi_request_interface_field; //This should pass in as a function parameter


	const char							*avro_message_name;       /**< Avro message name */

	zws_avi_ifttt_message_argument_t	argument_handler;         /**< Command specific argument parser; NULL if no such arguments */
	zws_avi_ifttt_message_result_t		result_handler;           /**< Result handler */

	int									cgi_response_type;		  /**< CGI request type. IFTTT_RESPONSE_TYPE_xxx */
	char								*error_text;              /**< Error text to be sent if command fails */
};

/**
 * @brief Get AVI ifttt message object from CGI URI
 * @param[in]     message_list      List of ifttt message handlers
 * @param[in]     cgi_uri           CGI URI
 * @param[in]     cgi_request_type   CGI request type
 * @param[in]     cgi_request_interface_feature       CGI request interface and feature
 * @param[in]     cgi_request_subtype   CGI request subtype
 * @param[in]     cgi_request_IF_field   CGI subtype field value
 * @param[out]    message_name      Message name
 * @param[out]    error_text        Error text
 * @param[out]    message_p         AVI message object
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
zws_status_t zws_avi_get_avi_message_ifttt_from_cgi_uri(zwu_array_list_t message_list,
														const char *cgi_uri,
														int cgi_request_type,
														eifttt_IF_feature_type cgi_e_IF_feature,
														int cgi_request_subtype,
														eifttt_fields cgi_request_IF_field,
														const char **message_name,
														char **error_text,
														zws_avi_message_ifttt_t **message_p);

/**
 * @brief Add AVI ifttt message object to the list of ifttt message handlers
 * @param[in]     message_list      List of ifttt message handlers
 * @param[in]     message           AVI ifttt message object
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
zws_status_t zws_avi_message_list_add_message_ifttt(zwu_array_list_t message_list, zws_avi_message_ifttt_t *message);
/** }@ */
#endif /* ZWS_AVI_MESSAGE_IFTTT_H_ */
