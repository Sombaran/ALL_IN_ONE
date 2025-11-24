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
 * @file    zwu_http_response_header.h
 *
 * @brief   Curl http response header wrapper.
 * @details Handles HTTP(S) response header.
 *
 * @author  Zelin
 *
 * @version 1.0 - 2016-12-19
 * - Initial version
 */

#ifndef ZWU_HTTP_RESPONSE_HEADER_H_
#define ZWU_HTTP_RESPONSE_HEADER_H_
#include <stddef.h>
#include "zwu_status.h"

/**
 * @defgroup zwu_http_response_header http response header
 * @{
 * @ingroup util
 */

/** Http response header class */
typedef struct _zwu_http_response_header *zwu_http_response_header_t;

/**
 * @brief Append field to http response header
 * @param[in]     http_response_header          Http response header object
 * @param[in]     field                         Field
 * @param[in]     field_length                  Field length
 * @retval        ZWU_STATUS_OK                 Success
 * @retval        ZWU_STATUS_ERROR              Failure
 */
zwu_status_t zwu_http_response_header_append_field(zwu_http_response_header_t http_response_header, char *field, size_t field_length);

/**
 * @brief Search field in the http response header
 * @param[in]     http_response_header          Http response header object
 * @param[in]     field_name                    Field name
 * @param[out]    field_value_content_p         Pointer to field value and content within http_response_header, no more '\r\n' or '\n', with zero-terminating.
 * @param[out]    field_value_content_length_p  Field value and content length, excluding tailing zero-terminator.
 * @retval        ZWU_STATUS_OK                 Success
 * @retval        ZWU_STATUS_ERROR              Failure
 */
zwu_status_t zwu_http_response_header_search_field(zwu_http_response_header_t http_response_header, const char *field_name,
	char **field_value_content_p, size_t *field_value_content_length_p);

/**
 * @brief Get status line from http response header
 * @param[in]     http_response_header          Http response header object
 * @param[out]    status_line_p                 Pointer to status line within http_response_header, zero-terminating.
 * @retval        ZWU_STATUS_OK                 Success
 * @retval        ZWU_STATUS_ERROR              Failure
 */
zwu_status_t zwu_http_response_header_get_status_line(zwu_http_response_header_t http_response_header, char **status_line_p);

/**
 * @brief Release all the fields in current http response header
 * @param[in]     http_response_header          Http response header object
 * @return        Nothing
 */
void zwu_http_response_header_release_all_fields(zwu_http_response_header_t http_response_header);

/**
 * @brief Create a http response header object
 * @param[in]     http_response_header_p    The new created Http response header object
 * @retval        ZWU_STATUS_OK             Success
 * @retval        ZWU_STATUS_ERROR          Failure
 */
zwu_status_t zwu_http_response_header_create(zwu_http_response_header_t *http_response_header_p);

/**
 * @brief Copy a http response header object
 * @param[in]     http_response_header      The source Http response header object
 * @param[out]    http_response_header_p    The destination http response header object
 * @retval        ZWU_STATUS_OK             Success
 * @retval        ZWU_STATUS_ERROR          Failure
 */
zwu_status_t zwu_http_response_header_copy(zwu_http_response_header_t http_response_header, zwu_http_response_header_t *http_response_header_p);

/**
 * @brief Destroy http response header
 * @param[in]     http_response_header      Http response header object
 * @return        Nothing
 * @see zwu_http_response_header_create
 */
void zwu_http_response_header_destroy(zwu_http_response_header_t http_response_header);

/**
 * @}
 */
#endif /* ZWU_HTTP_RESPONSE_HEADER_H_ */