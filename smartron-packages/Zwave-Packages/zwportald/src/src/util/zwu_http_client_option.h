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
 * @file    zwu_http_client_option.h
 *
 * @brief   HTTP Client Option
 * @details Helper option for zwu_http_client with curl, it also include functions that should not be exposed to user application.
 *
 * @author  Zelin
 *
 * @version 1.0 - 2017-01-06
 * - Initial version
 */

#ifndef ZWU_HTTP_CLIENT_OPTION_H_
#define ZWU_HTTP_CLIENT_OPTION_H_
#include "curl/curl.h"
#include "zwu_status.h"
#include "zwu_http_client_helper.h"

/**
 * @defgroup zwu_http_client_option HTTP Client Option
 * @{
 * @ingroup util
 */

/** zwu_http_client internal helper. Http client request enum */
typedef enum zwu_http_client_request_enum
{
	ZWU_HTTP_CLIENT_REQUEST_HTTP_GET,
	ZWU_HTTP_CLIENT_REQUEST_HTTP_POST,
	ZWU_HTTP_CLIENT_REQUEST_HTTP_POST_WITH_CALLBACK,
	ZWU_HTTP_CLIENT_REQUEST_HTTP2_PING,
}zwu_http_client_request_enum_t;

/** zwu_http_client internal helper. Http client manager action enum */
typedef enum {
	MANAGE_ACTION_ADD,
	MANAGE_ACTION_REMOVE,
} zwu_http_client_manager_action_enum_t;

// Expose the internal structure here, so that zwu_http_client/zwu_http_client_manager can just access the member without Getter/Setter.

/** String option for http client */
typedef struct _zwu_http_client_string_option zwu_http_client_string_option_t;
struct _zwu_http_client_string_option
{
	char *option;                        /**< pointer to sting option */
	int set_flag;                        /**< flag to indicate user set */
};

/** Http client option class */
typedef struct _zwu_http_client_option zwu_http_client_option_t;
struct _zwu_http_client_option
{
	zwu_http_client_use_ssl_enum_t use_ssl;                   /**< CURLOPT_USE_SSL - request using SSL / TLS for the transfer */
	zwu_http_client_ssl_version_enum_t ssl_version;           /**< CURLOPT_SSLVERSION - set preferred TLS/SSL version */
	zwu_http_client_ssl_enable_alpn_enum_t ssl_enable_alpn;   /**< CURLOPT_SSL_ENABLE_ALPN - enable ALPN */
	zwu_http_client_ssl_verify_peer_enum_t ssl_verify_peer;   /**< CURLOPT_SSL_VERIFYPEER - verify the peer's SSL certificate */
	zwu_http_client_ssl_verify_host_enum_t ssl_verify_host;   /**< CURLOPT_SSL_VERIFYHOST - verify the certificate's name against host */

	zwu_http_client_http_version_enum_t http_version;         /**< CURLOPT_HTTP_VERSION - specify HTTP protocol version to use */
	zwu_http_client_verbose_enum_t verbose;                   /**< CURLOPT_VERBOSE - set verbose mode on/off */
	zwu_http_client_pipe_wait_enum_t pipe_wait;               /**< CURLOPT_PIPEWAIT - wait for pipelining/multiplexing */
	zwu_http_client_fresh_connect_enum_t fresh_connect;       /**< CURLOPT_FRESH_CONNECT - force a new connection to be used */
	long connect_timeout;                                     /**< CURLOPT_CONNECTTIMEOUT - timeout(seconds) for the connect phase */
	long timeout;                                             /**< CURLOPT_TIMEOUT - set maximum time the request is allowed to take */
	uint8_t http2_ping_opaque_data[8];                        /**< CURLOPT_HTTP2_PINGOPAQUE_DATA - set http2/ping opaque data */

	zwu_http_client_string_option_t ca_path;                  /**< CURLOPT_CAPATH - specify directory holding CA certificates. Default: Built-in system specific */
	zwu_http_client_string_option_t ca_info;                  /**< CURLOPT_CAINFO - path to Certificate Authority (CA) bundle */
	zwu_http_client_string_option_t ssl_cipher_list;          /**< CURLOPT_SSL_CIPHER_LIST - specify ciphers to use for TLS */
	// Client side authentication related setting
	zwu_http_client_string_option_t ssl_cert_type;            /**< CURLOPT_SSLCERTTYPE - specify type of the client SSL certificate */
	zwu_http_client_string_option_t ssl_cert;                 /**< CURLOPT_SSLCERT - set SSL client certificate */
	zwu_http_client_string_option_t ssl_key_type;             /**< CURLOPT_SSLKEYTYPE - set type of the private key file */
	zwu_http_client_string_option_t ssl_key;                  /**< CURLOPT_SSLKEY - specify private keyfile for TLS and SSL client cert */
	zwu_http_client_string_option_t base_hostname;            /**< CURLOPT_BASE_HOSTNAME - set base hostname for host name verification during ssl handshake */

	zwu_http_client_string_option_t url;                      /**< CURLOPT_URL - provide the URL to use in the request */
	struct curl_slist *http_header;                           /**< CURLOPT_HTTPHEADER - set custom HTTP headers */

	char error_buffer[CURL_ERROR_SIZE];                       /**< CURLOPT_ERRORBUFFER - set error buffer for error messages */

	size_t (*write_callback)(void *ptr, size_t size, size_t nmemb, void *context);   /**< CURLOPT_WRITEFUNCTION - set callback for writing received data */
	size_t (*header_callback)(void *ptr, size_t size, size_t nmemb, void *context);  /**< CURLOPT_HEADERFUNCTION - callback that receives header data */
	int (*debug_callback)(CURL *handle, curl_infotype type, char *data, size_t size, void *userp);  /**< CURLOPT_DEBUGFUNCTION - debug callback */
	void (*http2_ping_callback)(uint8_t flag, const uint8_t opaque_data[8], void *context);         /**< CURLOPT_HTTP2_PINGFUNCTION - HTTP2/PING callback */
};

/** opaque zwu_http_client */
typedef struct _zwu_http_client *zwu_http_client_t;

/** zwu_http_client internal helper. Http client request struct */
typedef struct _zwu_http_client_request zwu_http_client_request_t;
struct _zwu_http_client_request
{
	zwu_http_client_manager_action_enum_t action;
	zwu_http_client_request_enum_t request_method;
	zwu_http_client_option_t *options;  // Change to pointer? looks like this struct is used as temporary variable
	zwu_http_client_response_callback response_callback;
	zwu_http_client_http2_ping_response_callback http2_ping_response_callback;
	void *response_context;
	zwu_http_client_t http_client;
	const char *post_buffer;
	size_t post_buffer_size;
	int manage_id;
};

/**
 * @brief zwu_http_client internal helper. Convert curl easy code to zwu status code.
 * @param[in]   result       Curl easy code
 * @retval      ZWU status code
 */
zwu_status_t zwu_http_client_easy_code_to_zwu_status(CURLcode result);

/**
 * @brief  Convert http version from curl value to zwu enum value.
 * @param[in]   curl_http_version       Curl value for http version
 * @retval      ZWU value for http version
 */
zwu_http_client_http_version_enum_t zwu_http_client_option_curl_value_to_zwu_value_http_version(long curl_http_version);

/**
 * @brief zwu_http_client internal helper. curl debug function callback
 * @param[in]     handle                  Curl handle
 * @param[in]     type                    Information type
 * @param[in]     data                    Data, NOT be zero terminated
 * @param[in]     size                    Data size
 * @param[in]     userp                   User context
 * @retval        0
 */
int zwu_http_client_option_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userp);

/**
 * @brief Set fixed curl easy options, which should remain the same for curl handle.
 * @param[in]   options                HTTP client options
 * @param[in]   curl_easy_handle       Curl handle
 * @param[in]   context                User context
 * @retval      Curl easy code
 */
CURLcode zwu_http_client_option_configure_fixed_easy_options(const zwu_http_client_option_t *options, CURL *curl_easy_handle, const void *context);

/**
 * @brief Set curl easy options, which may vary due to Setter.
 * @param[in]   options                HTTP client options
 * @param[in]   curl_easy_handle       Curl handle
 * @retval      Curl easy code
 */
CURLcode zwu_http_client_option_configure_easy_options(const zwu_http_client_option_t *options, CURL *curl_easy_handle);

/**
 * @brief Prepare http request
 * @param[in]   options                HTTP client options
 * @param[in]   curl_easy_handle       Curl handle
 * @param[in]   request                Http request enum
 * @param[in]   post_buffer            Http post buffer
 * @param[in]   post_buffer_size       Http post buffer size
 * @param[in]   post_read_callback     Post callback
 * @param[in]   context                Post callback context
 * @retval      Curl easy code
 */
CURLcode zwu_http_client_option_prepare_http_request(const zwu_http_client_option_t *options, CURL *curl_easy_handle, zwu_http_client_request_enum_t request,
	const char *post_buffer, size_t post_buffer_size, zwu_http_client_post_read_callback post_read_callback, void *context);

/**
 * @brief Init client options with default value
 * @param[in]    curl_option          curl option
 * @return       Nothing
 *
 * @note
 * @par
 * This function doesn't set the callback list to NULL. User should init the callback list.
 */
void zwu_http_client_option_init_with_default_value(zwu_http_client_option_t *curl_option);

/**
 * @brief Destroy client options with default value
 * @param[in]    curl_option          curl option
 * @return       Nothing
 */
void zwu_http_client_option_destroy(zwu_http_client_option_t *curl_option);

/**
 * @brief Copy client options from source to destination
 * @param[in]    source              Source curl option
 * @param[out]   destination         Destination curl option
 * @retval       ZWU_STATUS_OK       Success
 * @retval       ZWU_STATUS_ERROR    Failure
 */
zwu_status_t zwu_http_client_option_copy(const zwu_http_client_option_t *source,
	zwu_http_client_option_t *destination);

/** Template for write callback, context_type should have member variable char *write_buffer and size_t write_buffer_size */
#define ZWU_HTTP_CLIENT_DEFINE_WRITE_CALLBACK(callback_name, context_type) \
	static size_t callback_name(void *ptr, size_t size, size_t nmemb, void *context) \
	{ \
		context_type http_client; \
		size_t real_write_size; \
		char* realloc_buffer; \
		http_client = (context_type) context; \
		real_write_size = size * nmemb; \
		if ((realloc_buffer = ZWP_REALLOC(http_client->write_buffer, http_client->write_buffer_size + real_write_size)) == NULL) \
		{ \
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED); \
			goto l_err_write_buffer_realloc; \
		} \
		http_client->write_buffer = realloc_buffer; \
		memcpy(&(http_client->write_buffer[http_client->write_buffer_size]), ptr, real_write_size); \
		http_client->write_buffer_size += real_write_size; \
		return real_write_size; \
	l_err_write_buffer_realloc: \
		return 0; \
	}

/** Template for header callback, context_type should have member variable http_response_header */
#define ZWU_HTTP_CLIENT_DEFINE_HEADER_CALLBACK(callback_name, context_type) \
	static size_t callback_name(void *ptr, size_t size, size_t nmemb, void *context) \
	{ \
		context_type http_client; \
		size_t real_write_size; \
		http_client = (context_type) context; \
		real_write_size = size * nmemb; \
		if (zwu_http_response_header_append_field(http_client->http_response_header, (char*)ptr, real_write_size) != ZWU_STATUS_OK) \
		{ \
			goto l_err_curl_http_response_header_append_field; \
		} \
		return real_write_size; \
	l_err_curl_http_response_header_append_field: \
		return 0; \
	}

/** Template for HTTP2 PING callback, context_type should have member variable options and int ping_ack */
#define ZWU_HTTP_CLIENT_DEFINE_HTTP2_PING_CALLBACK(callback_name, context_type) \
	static void callback_name(uint8_t flag, const uint8_t opaque_data[8], void *context) \
	{ \
		context_type http_client; \
		http_client = (context_type) context; \
		ZWP_LOG(ZWP_LOG_DEBUG, "flag: 0x%x, and opaque_data:", flag); \
		ZWP_LOG_DUMP(ZWP_LOG_DEBUG, (char*) opaque_data, 8); \
		if (!memcmp(http_client->options.http2_ping_opaque_data, opaque_data, sizeof(http_client->options.http2_ping_opaque_data))) \
		{ \
			http_client->ping_ack = 1; \
		} \
	}

/**
 * @}
 */
#endif /* ZWU_HTTP_CLIENT_OPTION_H_ */
