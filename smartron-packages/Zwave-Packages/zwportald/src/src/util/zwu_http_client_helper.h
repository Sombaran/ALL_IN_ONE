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
 * @file    zwu_http_client_helper.h
 *
 * @brief   HTTP Client Helper
 * @details Helper header for zwu_http_client with curl, which should also be exposed to user application.
 *
 * @author  Zelin
 *
 * @version 1.0 - 2017-01-06
 * - Initial version
 */

#ifndef ZWU_HTTP_CLIENT_HELPER_H_
#define ZWU_HTTP_CLIENT_HELPER_H_
#include <stddef.h>
#include "zwu_status.h"

/**
 * @defgroup zwu_http_client_helper HTTP Client Helper
 * @{
 * @ingroup util
 */

typedef enum zwu_http_client_use_ssl_enum {
	ZWU_HTTP_CLIENT_USE_SSL_INVALID,   /**< Invalid */
	ZWU_HTTP_CLIENT_USE_SSL_NONE,      /**< Don't attempt to use SSL. */
	ZWU_HTTP_CLIENT_USE_SSL_TRY,       /**< Try using SSL, proceed as normal otherwise. */
	ZWU_HTTP_CLIENT_USE_SSL_CONTROL,   /**< Require SSL for the control connection or fail with CURLE_USE_SSL_FAILED. */
	ZWU_HTTP_CLIENT_USE_SSL_ALL        /**< Require SSL for all communication or fail with CURLE_USE_SSL_FAILED. */
} zwu_http_client_use_ssl_enum_t;

typedef enum zwu_http_client_ssl_version_enum {
	ZWU_HTTP_CLIENT_SSL_VERSION_INVALID,    /**< Invalid */
	ZWU_HTTP_CLIENT_SSL_VERSION_DEFAULT,    /**< The default action. This will attempt to figure out the remote SSL protocol version. */
	ZWU_HTTP_CLIENT_SSL_VERSION_TLSV1,      /**< TLSv1.x */
	ZWU_HTTP_CLIENT_SSL_VERSION_SSLV2,      /**< SSLv2 */
	ZWU_HTTP_CLIENT_SSL_VERSION_SSLV3,      /**< SSLv3 */
	ZWU_HTTP_CLIENT_SSL_VERSION_TLSV1_0,    /**< TLSv1.0 */
	ZWU_HTTP_CLIENT_SSL_VERSION_TLSV1_1,    /**< TLSv1.1 */
	ZWU_HTTP_CLIENT_SSL_VERSION_TLSV1_2,    /**< TLSv1.2 */
	ZWU_HTTP_CLIENT_SSL_VERSION_TLSV1_3     /**< TLSv1.3 */
} zwu_http_client_ssl_version_enum_t;

typedef enum zwu_http_client_ssl_enable_alpn_enum {
	ZWU_HTTP_CLIENT_SSL_ENABLE_ALPN_INVALID,    /**< Invalid */
	ZWU_HTTP_CLIENT_SSL_ENABLE_ALPN_DISABLE,    /**< disable alpn */
	ZWU_HTTP_CLIENT_SSL_ENABLE_ALPN_ENABLE      /**< enable alpn */
} zwu_http_client_ssl_enable_alpn_enum_t;

typedef enum zwu_http_client_ssl_verify_peer_enum {
	ZWU_HTTP_CLIENT_SSL_VERIFY_PEER_INVALID,    /**< Invalid */
	ZWU_HTTP_CLIENT_SSL_VERIFY_PEER_DISABLE,    /**< disable peer verify */
	ZWU_HTTP_CLIENT_SSL_VERIFY_PEER_ENABLE      /**< enable peer verify */
} zwu_http_client_ssl_verify_peer_enum_t;

typedef enum zwu_http_client_ssl_verify_host_enum {
	ZWU_HTTP_CLIENT_SSL_VERIFY_HOST_INVALID,    /**< Invalid */
	ZWU_HTTP_CLIENT_SSL_VERIFY_HOST_DISABLE,    /**< disable host verify */
	ZWU_HTTP_CLIENT_SSL_VERIFY_HOST_ENABLE      /**< enable host verify */
} zwu_http_client_ssl_verify_host_enum_t;

typedef enum zwu_http_client_http_version_enum {
	ZWU_HTTP_CLIENT_HTTP_VERSION_INVALID, /**< Invalid */
	ZWU_HTTP_CLIENT_HTTP_VERSION_NONE,    /**< We don't care about what version the library uses. libcurl will use whatever it thinks fit. */
	ZWU_HTTP_CLIENT_HTTP_VERSION_1_0,     /**< Enforce HTTP 1.0 requests. */
	ZWU_HTTP_CLIENT_HTTP_VERSION_1_1,     /**< Enforce HTTP 1.1 requests. */
	ZWU_HTTP_CLIENT_HTTP_VERSION_2_0,     /**< Attempt HTTP 2 requests. libcurl will fall back to HTTP 1.1 if HTTP 2 can't be negotiated with the server. */
	ZWU_HTTP_CLIENT_HTTP_VERSION_2_TLS,   /**< Attempt HTTP 2 over TLS (HTTPS) only. libcurl will fall back to HTTP 1.1 if HTTP 2 can't be negotiated with
									the HTTPS server. For clear text HTTP servers, libcurl will use 1.1. */
	ZWU_HTTP_CLIENT_HTTP_VERSION_2_PRIOR_KNOWLEDGE /**< Issue non-TLS HTTP requests using HTTP/2 without HTTP/1.1 Upgrade. It requires prior knowledge that
											the server supports HTTP/2 straight away. HTTPS requests will still do HTTP/2 the standard way with
											negotiated protocol version in the TLS handshake. */
} zwu_http_client_http_version_enum_t;

typedef enum zwu_http_client_verbose_enum {
	ZWU_HTTP_CLIENT_VERBOSE_INVALID,      /**< Invalid */
	ZWU_HTTP_CLIENT_VERBOSE_OFF,          /**< verbose mode off */
	ZWU_HTTP_CLIENT_VERBOSE_ON            /**< verbose mode on */
} zwu_http_client_verbose_enum_t;

typedef enum zwu_http_client_pipe_wait_enum {
	ZWU_HTTP_CLIENT_PIPE_WAIT_INVALID,      /**< Invalid */
	ZWU_HTTP_CLIENT_PIPE_WAIT_OFF,          /**< pipe wait off */
	ZWU_HTTP_CLIENT_PIPE_WAIT_ON            /**< pipe wait on */	
} zwu_http_client_pipe_wait_enum_t;

typedef enum zwu_http_client_fresh_connect_enum {
	ZWU_HTTP_CLIENT_FRESH_CONNECT_INVALID,  /**< Invalid */
	ZWU_HTTP_CLIENT_FRESH_CONNECT_OFF,      /**< Use existing connection */
	ZWU_HTTP_CLIENT_FRESH_CONNECT_ON,       /**< Create new connection */
} zwu_http_client_fresh_connect_enum_t;

/**
 * @brief Callback to populate buffer for HTTP POST
 * @param[out]    buffer                     Buffer to be filled by callback
 * @param[in]     size                       Element size
 * @param[in]     nmemb                      Element count
 * @param[in]     context                    User context
 * @retval        Actual number of bytes that it stored in buffer.
 */
typedef size_t (*zwu_http_client_post_read_callback)(void *buffer, size_t size, size_t nmemb, void *context);

/** opaque declaration for http_response_header */
typedef struct _zwu_http_response_header *zwu_http_response_header_t;

/**
 * @brief Callback when response for asynchronous HTTP GET/POST is received
 * @param[in]    result                      ZWU status
 * @param[in]    http_response_header        Http response message header
 * @param[in]    body                        Http response message body, with zero-terminator
 * @param[in]    body_size                   Http response message body size, exclude tailing zeor-terminator
 * @param[in]    context                     User context
 * @return       Nothing
 *
 * @note
 * @par
 * User is allowed to do asynchronous HTTP request again, or even destory http_client if no more reuqest is needed, in callback.
 * But if doing multiple asynchronous HTTP request, like more than 1000 times, in single callback, may make pipe full of data and cause deadlock.
 * Following APIs are not allowed in callback:
 *     zwu_http_client_manager_disconnect() or zwu_http_client_asynchronous_disconnect_all() is NOT allowed as it will cause deadlock.
 *     zwu_http_client_manager_destroy() is also NOT allowed.
 */
typedef void (*zwu_http_client_response_callback)(zwu_status_t result, zwu_http_response_header_t http_response_header,
	char *body, size_t body_size, void *context);

/**
 * @brief Callback when response for asynchronous HTTP2 PING is received
 * @param[in]    result                      ZWU status
 * @param[in]    context                     User context
 * @return       Nothing
 *
 * @note
 * @par
 * The rules is same as zwu_http_client_response_callback.
 */
typedef void (*zwu_http_client_http2_ping_response_callback)(zwu_status_t result, void *context);

/**
 * @}
 */
#endif /* ZWU_HTTP_CLIENT_HELPER_H_ */