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
 * @file    zws_exec_php_script.h
 *
 * @brief   Manipulate php scripts in ZWare
 * @details Communicate with php scripts in ZWare to send request and get resposne
 *
 * @author  Angela
 *
 * @version 1.0 - 2016-07-13
 * - Initial version
 */

#ifndef ZWS_EXEC_PHP_SCRIPT_H_
#define ZWS_EXEC_PHP_SCRIPT_H_

#include <stdio.h>
#include <stdint.h>

#include "zws_status.h"


/**
 * @defgroup zws_php_script Manage_php_script
 * @{
 * @ingroup util
 */

 /** PHP-CLI file path */
#define PHP_CLI_FILE								"../bin/php-cli"

 /** PHP script file name that checks user existence */
#define CHECK_USER_EXISTENCE_SCRIPT_FILE			"install/zweb/auth-cgi-bin/ldap_check_user_existence.php"

 /** PHP script file name that gets user info from user ID */
#define GET_USER_INFO_SCRIPT_FILE					"install/zweb/auth-cgi-bin/ldap_get_user_info.php"


/** PHP script file return codes */
#define PHP_SCRIPT_SUCCESS							0
#define PHP_SCRIPT_ERROR_FOUND_0_RECORD				254	//-2
#define PHP_SCRIPT_ERROR_FOUND_MULTIPLE_RECORD		253 //-3
#define PHP_SCRIPT_ERROR_INTERNAL_FAILURE			252 //-4
#define PHP_SCRIPT_ERROR_INVALID_ARGS_PARAM			251 //-5

/** PHP script file input param */
#define PHP_SCRIPT_INPUT_USER_ID					"USR_ID"
#define PHP_SCRIPT_INPUT_USER_NAME					"USR_NAME" //Can be first name, or last name or full name
#define PHP_SCRIPT_INPUT_EMAIL						"EMAIL"
#define PHP_SCRIPT_INPUT_RAC						"RAC"
#define PHP_SCRIPT_INPUT_FULLNAME					"FULLNAME"  //For GET_USER_INFO_SCRIPT_FILE script
#define PHP_SCRIPT_INPUT_RAPIN						"RAPIN"  //For GET_USER_INFO_SCRIPT_FILE script
#define PHP_SCRIPT_INPUT_TIMEZONE					"TIMEZONE"  //For GET_USER_INFO_SCRIPT_FILE script
#define PHP_SCRIPT_INPUT_ACCESS_TOKEN				"ACCESS_TOKEN"  //For GET_USER_INFO_SCRIPT_FILE script

/**
 * @brief Transmit request to a php script
 * @param[in]     pipefd            Pipe
 * @param[in]     chScriptPath      The php script path without the Root directory (ZWare) path
 * @param[in]     iArgCnt           Number of arguments supplied. (Max 3)
 * @param[in]     chArg1            Argument 1 for the PHP script
 * @param[in]     chArg2            Argument 2 for the PHP script
 * @param[in]     chArg3            Argument 3 for the PHP script
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
zws_status_t zws_php_request_transmitter(int pipefd[2], 
										 char* chScriptPath,
										 int iArgCnt,
										 char *chArg1,
										 char *chArg2,
										 char *chArg3);


/**
* @brief Receive response from a php script
* @param[in]     pipefd            Pipe
* @param[out]    ppchResponse      Response of the PHP script execution
* @param[out]    piResLen          Length of the response
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
* @note
* @par
* The caller should call ZWS_FREE(*ppchResponse) to free resources
*/
zws_status_t zws_php_request_receiver(int pipefd[2],
									  char **ppchResponse,
									  int *piResLen);


/**
* @brief Execute a php script
* @param[in]     chScriptPath      The php script path without the Root directory (ZWare) path
* @param[in]     iArgCnt           Number of arguments supplied. (Max 3)
* @param[in]     chArg1            Argument 1 for the PHP script
* @param[in]     chArg2            Argument 2 for the PHP script
* @param[in]     chArg3            Argument 3 for the PHP script
* @param[out]    ppchResponse      Response of the PHP script execution
* @param[out]    piResLen          Length of the response
* @param[out]    piExitCode        Exit code of the PHP script
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_exec_php_request(char* chScriptPath,
								  int iArgCnt,
								  char *chArg1,
								  char *chArg2,
								  char *chArg3,
								  char **ppchResponse,
								  int *piResLen,
								  int *piExitCode);

/**
 * @}
 */

#endif /* ZWS_EXEC_PHP_SCRIPT_H_ */
