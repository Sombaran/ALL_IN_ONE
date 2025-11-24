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
 * @file    zwu_http_client.h
 *
 * @brief   HTTP Client
 * @details Handle HTTP(S) request with curl.
 *
 * @author  Zelin
 *
 * @version 1.0 - 2016-12-15
 * - Initial version
 */

#ifndef ZWU_HTTP_CLIENT_H_
#define ZWU_HTTP_CLIENT_H_

#include <stdint.h>

#include "zwu_status.h"
#include "zwu_http_response_header.h"
#include "zwu_http_client_helper.h"
#include "zwu_http_client_manager.h"

/**
 * @defgroup zwu_http_client HTTP Client
 * @{
 * @ingroup util
 */

/** Http client class */
typedef struct _zwu_http_client *zwu_http_client_t;

/**
 * @name Setters
 * @{
 */
/**
 * @brief Set use ssl
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @return        Nothing
 */
void zwu_http_client_set_use_ssl(zwu_http_client_t http_client, zwu_http_client_use_ssl_enum_t option);

/**
 * @brief Set use ssl
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @return        Nothing
 */
void zwu_http_client_set_ssl_version(zwu_http_client_t http_client, zwu_http_client_ssl_version_enum_t option);

/**
 * @brief Enable/Disable ssl ALPN
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @return        Nothing
 */
void zwu_http_client_set_ssl_enable_alpn(zwu_http_client_t http_client, zwu_http_client_ssl_enable_alpn_enum_t option);

/**
 * @brief Enable/Disable ssl peer verify
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @return        Nothing
 */
void zwu_http_client_set_ssl_verify_peer(zwu_http_client_t http_client, zwu_http_client_ssl_verify_peer_enum_t option);

/**
 * @brief Enable/Disable ssl host verify
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @return        Nothing
 */
void zwu_http_client_set_ssl_verify_host(zwu_http_client_t http_client, zwu_http_client_ssl_verify_host_enum_t option);

/**
 * @brief Set http version
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @return        Nothing
 */
void zwu_http_client_set_http_version(zwu_http_client_t http_client, zwu_http_client_http_version_enum_t option);

/**
 * @brief Set verbose
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @return        Nothing
 */
void zwu_http_client_set_verbose(zwu_http_client_t http_client, zwu_http_client_verbose_enum_t option);

/**
 * @brief Enable/Disable pipe wait
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @return        Nothing
 */
void zwu_http_client_set_pipe_wait(zwu_http_client_t http_client, zwu_http_client_pipe_wait_enum_t option);

/**
 * @brief To force new connection for current request
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @return        Nothing
 */
void zwu_http_client_set_fresh_connect(zwu_http_client_t http_client, zwu_http_client_fresh_connect_enum_t option);

/**
 * @brief Set connection timeout
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @return        Nothing
 */
void zwu_http_client_set_connect_timeout(zwu_http_client_t http_client, long timeout);

/**
 * @brief Set total curl request timeout
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @return        Nothing
 */
void zwu_http_client_set_timeout(zwu_http_client_t http_client, long timeout);

/**
 * @brief Set HTTP2/PING opaque data
 * @param[in]     http_client        Http client object
 * @param[in]     opaque_data        HTTP2/PING opaque data
 * @return        Nothing
 */
void zwu_http_client_set_http2_ping_opaque_data(zwu_http_client_t http_client, uint8_t opaque_data[8]);

/**
 * @brief Set CA info
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @retval        ZWU Status
 */
zwu_status_t zwu_http_client_set_ca_info(zwu_http_client_t http_client, const char *option);

/**
 * @brief Set CA path
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @retval        ZWU Status
 */
zwu_status_t zwu_http_client_set_ca_path(zwu_http_client_t http_client, const char *option);

/**
 * @brief Set ssl cipher list
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @retval        ZWU Status
 */
zwu_status_t zwu_http_client_set_ssl_cipher_list(zwu_http_client_t http_client, const char *option);

/**
 * @brief Set ssl cert type
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @retval        ZWU Status
 */
zwu_status_t zwu_http_client_set_ssl_cert_type(zwu_http_client_t http_client, const char *option);

/**
 * @brief Set ssl cert
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @retval        ZWU Status
 */
zwu_status_t zwu_http_client_set_ssl_cert(zwu_http_client_t http_client, const char *option);

/**
 * @brief Set ssl key type
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @retval        ZWU Status
 */
zwu_status_t zwu_http_client_set_ssl_key_type(zwu_http_client_t http_client, const char *option);

/**
 * @brief Set ssl key
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @retval        ZWU Status
 */
zwu_status_t zwu_http_client_set_ssl_key(zwu_http_client_t http_client, const char *option);

/**
 * @brief Set base host name
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @return        ZWU Status
 */
zwu_status_t zwu_http_client_set_base_hostname(zwu_http_client_t http_client, const char *option);

/**
 * @brief Set URL
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @retval        ZWU Status
 */
zwu_status_t zwu_http_client_set_url(zwu_http_client_t http_client, const char *option);

/**
 * @brief Set HTTP header
 * @param[in]     http_client        Http client object
 * @param[in]     option             Option value
 * @return        Nothing
 */
zwu_status_t zwu_http_client_set_http_header(zwu_http_client_t http_client, char *http_header_content);

/**
 * @brief Set HTTP version used in the last http connection
 * @param[in]     http_client        Http client object
 * @param[in]     http_version       Http version
 * @return        Nothing
  *
 * @note
 * @par
 * This API is main for zwu_http_client_manager, to set http version after request has successfully finished.
 * User SHOULD NOT call this API, instead, should call zwu_http_client_set_http_version()
 */
void zwu_http_client_set_http_version_in_use(zwu_http_client_t http_client, zwu_http_client_http_version_enum_t http_version);

/**
 * @}
 */

/**
 * @name Getters
 * @{
 */
/**
 * @brief Get HTTP version used in the connection
 * @param[in]     http_client        Http client object
 * @return        HTTP version
 *
 * @note
 * @par
 * The return value may not be the same as the one set by Setter zwu_http_client_set_http_version().
 * Caller should call this API after finishing http request, so that the return value is the version used in the last http connection.
 */
zwu_http_client_http_version_enum_t zwu_http_client_get_http_version(zwu_http_client_t http_client);

/**
 * @}
 */

/** 
 * @brief Do HTTP request GET
 * @param[in]    http_client                   Http client object
 * @param[out]   http_response_header_p        Http response message header
 * @param[out]   body_p                        Http response message body, with zero-terminator
 * @param[out]   body_size_p                   Http response message body size, exclude tailing zeor-terminator
 * @retval       ZWU Status
 *
 * @note
 * @par
 * The caller should call zwu_http_response_header_destroy() to destroy http_response_header_p
 */
zwu_status_t zwu_http_client_http_get(zwu_http_client_t http_client,
	zwu_http_response_header_t *http_response_header_p, char **body_p, size_t *body_size_p);

/** 
 * @brief Do asynchronous HTTP request GET
 * @param[in]    http_client                   Http client object
 * @param[in]    response_callback             Http response callback
 * @param[in]    context                       Http response callback context
 * @retval       ZWU_STATUS_OK                 Success
 * @retval       ZWU_STATUS_ERROR              Failure
 */
zwu_status_t zwu_http_client_asynchronous_http_get(zwu_http_client_t http_client,
	zwu_http_client_response_callback response_callback, void *context);

/** 
 * @brief Do HTTP request POST with callback
 * @param[in]    http_client                   Http client object
 * @param[in]    post_read_callback            Post read callback
 * @param[in]    context                       Post read callback context
 * @param[out]   http_response_header_p        Http response message header
 * @param[out]   body_p                        Http response message body, with zero-terminator
 * @param[out]   body_size_p                   Http response message body size, exclude tailing zeor-terminator
 * @retval       ZWU Status
 *
 * @note
 * @par
 * The caller should call zwu_http_response_header_destroy() to destroy http_response_header_p
 */
zwu_status_t zwu_http_client_http_post_with_callback(zwu_http_client_t http_client, zwu_http_client_post_read_callback post_read_callback, void *context,
	zwu_http_response_header_t *http_response_header_p, char **body_p, size_t *body_size_p);

// zwu_status_t zwu_http_client_asynchronous_http_post_with_callback(zwu_http_client_t http_client, zwu_http_client_post_read_callback post_read_callback, void *context,
//	zwu_http_response_header_t *http_response_header_p, char **body_p, size_t *body_size_p);

/** 
 * @brief Do HTTP request POST
 * @param[in]    http_client                   Http client object
 * @param[in]    post_buffer                   Post buffer
 * @param[in]    post_buffer_size              Post buffer size
 * @param[out]   http_response_header_p        Http response message header
 * @param[out]   body_p                        Http response message body, with zero-terminator
 * @param[out]   body_size_p                   Http response message body size, exclude tailing zeor-terminator
 * @retval       ZWU Status
 *
 * @note
 * @par
 * The caller should call zwu_http_response_header_destroy() to destroy http_response_header_p
 */
zwu_status_t zwu_http_client_http_post(zwu_http_client_t http_client, const char *post_buffer, size_t post_buffer_size,
	zwu_http_response_header_t *http_response_header_p, char **body_p, size_t *body_size_p);

/** 
 * @brief Do asynchronous HTTP request POST
 * @param[in]    http_client                   Http client object
 * @param[in]    post_buffer                   Post buffer
 * @param[in]    post_buffer_size              Post buffer size
 * @param[in]    response_callback             Http response callback
 * @param[in]    context                       Http response callback context
 * @retval       ZWU_STATUS_OK                 Success
 * @retval       ZWU_STATUS_ERROR              Failure
 */
zwu_status_t zwu_http_client_asynchronous_http_post(zwu_http_client_t http_client, const char *post_buffer, size_t post_buffer_size,
	zwu_http_client_response_callback response_callback, void *context);

/** 
 * @brief Do HTTP2/PING
 * @param[in]    http_client                   Http client object
 * @retval       ZWU Status
 *
 * @note
 * @par
 * This API should be called on existing HTTP2 connection(means reusing zwu_http_client), or make sure zwu_http_client
 * is set to use ZWU_HTTP_CLIENT_HTTP_VERSION_2_TLS/ZWU_HTTP_CLIENT_HTTP_VERSION_2_PRIOR_KNOWLEDGE, so that HTTP2
 * connection can be built without upgrading from any HTTP/1.1 request. Any fallback to HTTP/1.1 should cause to fail
 * for this API.
 */
zwu_status_t zwu_http_client_http2_ping(zwu_http_client_t http_client);

/** 
 * @brief Do asynchronous HTTP2/PING
 * @param[in]    http_client                   Http client object
 * @param[in]    http2_ping_response_callback  Http2 ping response callback
 * @param[in]    context                       Http response callback context
 * @retval       ZWU_STATUS_OK                 Success
 * @retval       ZWU_STATUS_ERROR              Failure
 */
zwu_status_t zwu_http_client_asynchronous_http2_ping(zwu_http_client_t http_client,
	zwu_http_client_http2_ping_response_callback http2_ping_response_callback, void *context);

/**
 * @brief Create a http client object
 * @param[out]    http_client_p             The new http_client object
 * @retval        ZWU_STATUS_OK             Success
 * @retval        ZWU_STATUS_ERROR          Failure
 */
zwu_status_t zwu_http_client_create(zwu_http_client_t *http_client_p);

/**
 * @brief Create a http client object for asynchronous http request with custom http_client_manager object
 * @param[in]     http_client_manager       Http client manager object
 * @param[out]    http_client_p             The new http_client object
 * @retval        ZWU_STATUS_OK             Success
 * @retval        ZWU_STATUS_ERROR          Failure
 */
zwu_status_t zwu_asynchronous_http_client_create_with_manager(zwu_http_client_manager_t http_client_manager, zwu_http_client_t *http_client_p);

/**
 * @brief Create a http client object for asynchronous http request, which use global http_client_manager object
 * @param[out]    http_client_p             The new http_client object
 * @retval        ZWU_STATUS_OK             Success
 * @retval        ZWU_STATUS_ERROR          Failure
 *
 * @note
 * @par
 * For simple usage, user can just call this API to create http_client and do asynchronous HTTP request.
 * zwu_asynchronous_http_client_create_with_manager() is for more advance usage, which requires user to maintain http_client_manager object
 */
zwu_status_t zwu_asynchronous_http_client_create(zwu_http_client_t *http_client_p);

/**
 * @brief Copy a http client object from http_client to http_client_p
 * @param[in]     http_client               Source http_client object
 * @param[out]    http_client_p             The copied http_client object
 * @retval        ZWU_STATUS_OK             Success
 * @retval        ZWU_STATUS_ERROR          Failure
 *
 * @note
 * @par
 * This API will create a new zwu_http_client object, and copy the http related option value from source object to the new object.
 * So user don't need to call Setters again to configure http options unless want to update new option value. But Getter value may
 * be different and better to do HTTP request first
 */
zwu_status_t zwu_http_client_copy(zwu_http_client_t http_client, zwu_http_client_t *http_client_p);

/**
 * @brief Destroy http client object
 * @param[in]     http_client        Http client object
 * @return        Nothing
 * @see zwu_http_client_create
 *
 * @note
 * @par
 * Once destroyed, for asynchronous request, no more callback will be called.
 */
void zwu_http_client_destroy(zwu_http_client_t http_client);

/**
 * @brief Http client global init
 * @retval        ZWU_STATUS_OK             Success
 * @retval        ZWU_STATUS_ERROR          Failure
 */
zwu_status_t zwu_http_client_global_init(void);

/**
 * @brief Http client global cleanup
 * @return        Nothing
 */
void zwu_http_client_global_cleanup(void);

/**
 * @brief Close all the connections for asynchronous request that created by zwu_asynchronous_http_client_create()
 * @retval        ZWU_STATUS_OK             Success
 * @retval        ZWU_STATUS_ERROR          Failure
 *
 * @note
 * @par
 * Once disconnected, for asynchronous request, no more callback will be called until request again.
 */
zwu_status_t zwu_http_client_asynchronous_disconnect_all(void);

#if ZWU_HTTP_CLIENT_TEST
zwu_status_t zwu_http_client_test();
#endif

/**
 * @}
 */
#endif /* ZWU_HTTP_CLIENT_H_ */
