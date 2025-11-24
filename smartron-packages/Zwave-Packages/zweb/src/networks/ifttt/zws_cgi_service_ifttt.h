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
 * @file    zws_cgi_service_ifttt.h
 *
 * @brief   CGI Input from IFTTT
 * @details Handles CGI Input parameters from IFTTT.
 *
 * @author  Angela
 *
 * @version 1.0 - 2016-06-22
 * - Initial version
 */

#ifndef zws_cgi_service_ifttt_H_
#define zws_cgi_service_ifttt_H_

/**
 * \addtogroup  zwebcgiiftttservice
 * @{
 */

#include "zws_cgi_common.h"
#include "zws_cgi_service.h"
#include "zws_ifttt_common.h"

/** IFTTT service class */
struct _zws_service_ifttt;

typedef struct _zws_service_ifttt *pzws_service_ifttt_t;

/**
* @brief Get CGI ZWare command and args from environment and also process post request arguments
* @param[in]     service					 Service object
* @retval        ZWS_STATUS_OK				 Success
* @retval        ZWS_STATUS_IFTTT_ERROR_xxx  Failure
*/
zws_status_t zws_cgi_get_ifttt_command(zws_service_t service);

/**
* @brief Get and process IFTTT post request arguments
* @param[in]     service_common    Object representing items common across all levels
* @param[in]     pservice_ifttt    IFTTT service object
* @retval        ZWS_STATUS_OK				 Success
* @retval        ZWS_STATUS_IFTTT_ERROR_xxx  Failure
*/
zws_status_t zws_cgi_get_ifttt_args(zws_service_common_t service_common, pzws_service_ifttt_t pservice_ifttt);

/**
* @brief Free IFTTT post request arguments
* @param[in]     pservice_ifttt    IFTTT service object
* @retval        ZWS_STATUS_OK				 Success
* @retval        ZWS_STATUS_IFTTT_ERROR_xxx  Failure
*/
void zws_cgi_free_ifttt_args(pzws_service_ifttt_t pservice_ifttt);

/**
* @brief Format and send IFTTT CGI error output to web server
* @param[in]     user_name		   User id
* @param[out]    pbExist           Pointer to result of existence
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_cgi_ifttt_check_user_existence(char *user_name, int *pbExist);

/**
* @brief Format and send IFTTT CGI error output to web server
* @param[in]     sAccessToken		Access Token without "Bearer" keyword
* @param[out]    ppifttt_user       Pointer to the matched IFTTT user ID
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_cgi_ifttt_oauth2_user_name_get(char *sAccessToken, char **ppifttt_user);

/**
* @brief Validate the ifttt incoming uris for triggers and actions and return an uri struct if validated
* @param[in, out]     pstifttt_uri		   Pointer to uri structure
* @param[in]		  sintf_slug           Interface slug command
* @param[in]		  sfield_command       Field slug command
* @param[in]		  ssub_command         Subcommand
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_cgi_validate_ifttt_trigger_action_command(ifttt_uri_t	*pstifttt_uri,
															char *sintf_slug,
															char *sfield_command,
															char *ssub_command);

/**
* @brief Send IFTTT CGI output to web server
* @param[in]     service           Service object
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_cgi_ifttt_send_cgi_output(zws_service_t service);

/**
* @brief Format and send IFTTT CGI error output to web server
* @param[in]     error_status      Error status code from the request processing
* @param[in]     service           Service object
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_cgi_ifttt_send_cgi_output_error(zws_status_t error_status, zws_service_t service);


/**
* @brief Allocate IFTTT level service object
* @param[in]     stifttt_uri	   Object representing ifttt uri commands
* @param[in]     service_common    Object representing items common across all levels
* @param[out]    ppservice_ifttt   IFTTT service object
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zws_cgi_free_cgi_service_networks_from_environment() to free resources
*/
zws_status_t zws_cgi_get_cgi_service_ifttt_from_environment(ifttt_uri_t	stifttt_uri, 
															zws_service_common_t service_common,
															pzws_service_ifttt_t *ppservice_ifttt);

/**
* @brief Free ifttt level service object
* @param[in]     pservice_ifttt      IFTTT service object
* @return        Nothing
* @see zws_cgi_get_cgi_service_ifttt_from_environment
*/
void zws_cgi_free_cgi_service_ifttt_from_environment(pzws_service_ifttt_t pservice_ifttt);

/**
* @brief Set arguments of Avro value object for ifttt service
* @param[in]     pservice_ifttt    IFTTT service object
* @param[in]     type_name         Avro type name
* @param[in]     arg_value_p       Avro value object for arguments of the service
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_cgi_set_avro_value_of_request_arg_ifttt(pzws_service_ifttt_t pservice_ifttt,
	const char *type_name, avro_value_t *arg_value_p);


/**
* @brief Get result from Avro value object for a ifttt service
* @param[in]     pservice_ifttt    IFTTT service object
* @param[in]     type_name         Avro type name
* @param[in]     result_value_p    Avro value object for results of the service
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_cgi_set_service_result_ifttt(pzws_service_ifttt_t pservice_ifttt, 
											  const char *type_name, 
											  avro_value_t *result_value_p);

/** }@ */
#endif /* zws_cgi_service_ifttt_H_ */
