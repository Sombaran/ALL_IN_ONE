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
 * @file    zws_cgi_service.h
 *
 * @brief   CGI Input
 * @details Handles CGI Input parameters.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-13
 * - Initial version
 */

#ifndef ZWS_CGI_SERVICE_H_
#define ZWS_CGI_SERVICE_H_

/**
 * \addtogroup  zwebcgiservice
 * @{
 */

#include "zws_cgi_common.h"
#include "zws_avi_message.h"
#include "zws_status.h"
#include "zws_avi.h"
#include "zws_session.h"
#include "zws_cgi_service_common.h"

/** Forward declaration for CGI service class */
struct _zws_service;

/** Service class */
typedef struct _zws_service *zws_service_t;

/**
* @brief Set AVI message information
* @param[in]     service           Service object
* @param[in]     cgi_uri           CGI URI
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_cgi_set_avi_message_info(zws_service_t service, char *cgi_uri);

/**
 * @brief Get RPC request from CGI service
 * @param[in]     service           Service object
 * @param[out]    rpc_request_p     RPC request
 * @param[out]    rpc_request_length     Length of RPC request @a   rpc_request_p
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zws_cgi_free_rpc_request_from_cgi_service() to free resources
 */
zws_status_t zws_cgi_get_rpc_request_from_cgi_service(zws_service_t service, char **rpc_request_p, int *rpc_request_length);

/**
 * @brief Free RPC request
 * @param[in]     rpc_request       RPC request
 * @return        Nothing
 * @see zws_cgi_get_rpc_request_from_cgi_service
 */
void zws_cgi_free_rpc_request_from_cgi_service(char *rpc_request);

/**
 * @brief Get CGI service from environment
 * @param[out]    service_p         Service object
 * @param[in]     avi               AVI object
 * @param[in]     session           Session object
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zws_cgi_free_cgi_service_from_environment() to free resources
 */
zws_status_t zws_cgi_get_cgi_service_from_environment(zws_service_t *service_p, zws_avi_t avi, zws_session_t session);


/**
* @brief Get CGI ZWare command and args from environment
* @param[in]     service		   Service object
* @param[in]     get_user_return_status		   Status from get username
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_cgi_get_zware_command(zws_service_t service, zws_status_t get_user_return_status);

/**
 * @brief Free CGI service object
 * @param[in]     service           Service object
 * @return        Nothing
 * @see zws_cgi_get_cgi_service_from_environment
 */
void zws_cgi_free_cgi_service_from_environment(zws_service_t service);

/**
 * @brief Set result using RPC response
 * @param[in]     service           Service object
 * @param[in]     rpc_response      RPC response
 * @param[in]     rpc_response_length     Length of RPC response @a   rpc_response
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
zws_status_t zws_cgi_set_cgi_service_result_from_rpc_response(zws_service_t service, char *rpc_response, int rpc_response_length);

/**
 * @brief Send CGI output to web server
 * @param[in]     service           Service object
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
zws_status_t zws_cgi_send_cgi_output(zws_service_t service);

/**
 * @brief Send CGI error output to web server
 * @param[in]     service           Service object
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
zws_status_t zws_cgi_send_cgi_output_error(zws_service_t service);

/**
 * @brief Send CGI 'access denied' output to web server
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
zws_status_t zws_cgi_send_cgi_output_access_denied(void);

/**
 * @brief Setter for error text in service object
 */
void zws_cgi_set_error_text(zws_service_t service, char *error_text);

/**
* @brief Getter for error text in service object
*/
char *zws_cgi_get_error_text(zws_service_t service);

/**
* @brief Getter for service_common in service object
*/
zws_service_common_t zws_cgi_get_service_common(zws_service_t service);

/**
* @brief Setter for command_type in service object
*/
void  zws_cgi_set_service_command_type(zws_service_t service, zws_cgi_command_type_t command_type);

/**
* @brief Setter for service_next in service object
*/
void  zws_cgi_set_service_next(zws_service_t service, void  *service_next);

/** }@ */
#endif /* ZWS_CGI_SERVICE_H_ */
