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
 * @file    zwu_http_client.c
 *
 * @brief   HTTP Client
 * @details Handle HTTP(S) request with curl.
 *
 * @author  Zelin
 *
 * @version 1.0 - 2016-12-15
 * - Initial version
 */

#include "curl/curl.h"
#include "zwu_http_client.h"
#include "zwu_http_client_option.h"
#include "zwp_memory.h"
#include "zwu_log.h"
#include "zwp_error.h"
#include "zwu_http_client_manager.h"
#include "zwp_thread_mutex.h"

/**
 * @defgroup zwu_http_client HTTP Client
 * @{
 * @ingroup util
 */

/** Global http_client_manager object. The defualt manager object for asynchronous http_client,
    which is created by zwu_asynchronous_http_client_create() */
static zwu_http_client_manager_t global_http_client_manager;

struct _zwu_http_client
{
	// Common data for blocking and nonblocking operation
	int nonblock_flag;                   /**< Flag to indicate non-blocking HTTP request */
	zwu_http_client_option_t options;    /**< CURL options */

	// Data for blocking operation
	CURL *curl_easy_handle;              /**< CURL easy handle for blocking operation */
	int fixed_options_set;               /**< Flag to indicate if already set the fixed options for current curl handle */

	char *write_buffer;                  /**< Buffer for write_callback */
	size_t write_buffer_size;            /**< Buffer size for write_callback */

	int ping_ack;                        /**< Acknowledge for HTTP2/PING request*/

	zwu_http_response_header_t http_response_header;  /**< Buffer for header_callback */

	// Data for nonblocking operation
	int manage_id;                                             /**< Manage ID to communicate with zwu_http_client_manager */
	zwp_pool_t memory_pool;                                    /**< Memory pool */
	zwp_thread_mutex_t lock;                                   /**< Lock to protect share data between current context and zwu_http_client_manager */
	zwu_http_client_http_version_enum_t http_version_in_use;   /**< HTTP version used in the last http connection */
	zwu_http_client_manager_t http_client_manager;             /**< HTTP client manager, weak pointer. http_client must be destroyed before http_client_manager */
};

// To define Setter for string option
#define ZWU_HTTP_CLIENT_DEFINE_SETTER_STRING_OPTION(option_name) \
	zwu_status_t zwu_http_client_set_##option_name(zwu_http_client_t http_client, const char *option) \
	{ \
		ZWP_FREE(http_client->options.option_name.option); \
		http_client->options.option_name.option = NULL; \
		if (option && ((http_client->options.option_name.option = ZWP_STRDUP(option)) == NULL)) return ZWU_STATUS_ERROR; \
		http_client->options.option_name.set_flag = 1; \
		return ZWU_STATUS_OK; \
	}

// To define Setter for enum option
#define ZWU_HTTP_CLIENT_DEFINE_SETTER_LONG_OPTION(option_name) \
	void zwu_http_client_set_##option_name(zwu_http_client_t http_client, zwu_http_client_##option_name##_enum_t option) \
	{ \
		http_client->options.option_name = option;\
	}

/** Setters start here */
ZWU_HTTP_CLIENT_DEFINE_SETTER_LONG_OPTION(use_ssl)

ZWU_HTTP_CLIENT_DEFINE_SETTER_LONG_OPTION(ssl_version)

ZWU_HTTP_CLIENT_DEFINE_SETTER_LONG_OPTION(ssl_enable_alpn)

ZWU_HTTP_CLIENT_DEFINE_SETTER_LONG_OPTION(ssl_verify_peer)

ZWU_HTTP_CLIENT_DEFINE_SETTER_LONG_OPTION(ssl_verify_host)

ZWU_HTTP_CLIENT_DEFINE_SETTER_LONG_OPTION(http_version)

ZWU_HTTP_CLIENT_DEFINE_SETTER_LONG_OPTION(verbose)

ZWU_HTTP_CLIENT_DEFINE_SETTER_LONG_OPTION(pipe_wait)

ZWU_HTTP_CLIENT_DEFINE_SETTER_LONG_OPTION(fresh_connect)

void zwu_http_client_set_http2_ping_opaque_data(zwu_http_client_t http_client, uint8_t opaque_data[8])
{
	memcpy(http_client->options.http2_ping_opaque_data, opaque_data, sizeof(http_client->options.http2_ping_opaque_data));
}

void zwu_http_client_set_connect_timeout(zwu_http_client_t http_client, long timeout)
{
	// TODO: verify input?
	http_client->options.connect_timeout = timeout;
}

void zwu_http_client_set_timeout(zwu_http_client_t http_client, long timeout)
{
	// TODO: verify input?
	http_client->options.timeout = timeout;
}

ZWU_HTTP_CLIENT_DEFINE_SETTER_STRING_OPTION(ca_info)

ZWU_HTTP_CLIENT_DEFINE_SETTER_STRING_OPTION(ca_path)

ZWU_HTTP_CLIENT_DEFINE_SETTER_STRING_OPTION(ssl_cipher_list)

ZWU_HTTP_CLIENT_DEFINE_SETTER_STRING_OPTION(ssl_cert_type)

ZWU_HTTP_CLIENT_DEFINE_SETTER_STRING_OPTION(ssl_cert)

ZWU_HTTP_CLIENT_DEFINE_SETTER_STRING_OPTION(ssl_key_type)

ZWU_HTTP_CLIENT_DEFINE_SETTER_STRING_OPTION(ssl_key)

ZWU_HTTP_CLIENT_DEFINE_SETTER_STRING_OPTION(base_hostname)

ZWU_HTTP_CLIENT_DEFINE_SETTER_STRING_OPTION(url)

void zwu_http_client_set_http_version_in_use(zwu_http_client_t http_client, zwu_http_client_http_version_enum_t http_version)
{
	if (zwp_thread_mutex_lock(http_client->lock) != ZWP_STATUS_OK)
	{
		goto l_thread_mutex_lock;
	}

	http_client->http_version_in_use = http_version;
	zwp_thread_mutex_unlock(http_client->lock);
	return;

	zwp_thread_mutex_unlock(http_client->lock);
l_thread_mutex_lock:
	return;
}

/** Advance Setter */
zwu_status_t zwu_http_client_set_http_header(zwu_http_client_t http_client, char *http_header_content)
{
	struct curl_slist *http_header;

	http_header = curl_slist_append(http_client->options.http_header, http_header_content);
	if (http_header == NULL)
	{
		goto l_err_slist_append;
	}
	http_client->options.http_header = http_header;
	return ZWU_STATUS_OK;

l_err_slist_append:
	return ZWU_STATUS_ERROR;
}

/** Setters end here */

/** Getters start here */
zwu_http_client_http_version_enum_t zwu_http_client_get_http_version(zwu_http_client_t http_client)
{
	long curl_http_version;
	zwu_http_client_http_version_enum_t zwu_http_version;

	if (!http_client->nonblock_flag)
	{
		if (curl_easy_getinfo(http_client->curl_easy_handle, CURLINFO_HTTP_VERSION, &curl_http_version) != CURLE_OK)
		{
			goto l_err_curl_easy_getinfo_http_version;
		}

		zwu_http_version = zwu_http_client_option_curl_value_to_zwu_value_http_version(curl_http_version);
	}
	else
	{
		if (zwp_thread_mutex_lock(http_client->lock) != ZWP_STATUS_OK)
		{
			goto l_thread_mutex_lock;
		}
		zwu_http_version = http_client->http_version_in_use;
		zwp_thread_mutex_unlock(http_client->lock);
	}

	return zwu_http_version;

	if (http_client->nonblock_flag)
	{
		zwp_thread_mutex_unlock(http_client->lock);
	}
l_thread_mutex_lock:
l_err_curl_easy_getinfo_http_version:
	return ZWU_HTTP_CLIENT_HTTP_VERSION_INVALID;
}
/** Getters end here */

/**
 * @brief curl write callback
 * @param[in]     ptr                     Http client enum value
 * @param[in]     size                    Member size
 * @param[in]     nmemb                   Member count
 * @param[in]     context                 User context
 * @retval        Number of bytes process by user
 */
// static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *context)
ZWU_HTTP_CLIENT_DEFINE_WRITE_CALLBACK(write_callback, zwu_http_client_t)

/**
 * @brief curl header callback
 * @param[in]     ptr                     Http client enum value
 * @param[in]     size                    Member size
 * @param[in]     nmemb                   Member count
 * @param[in]     context                 User context
 * @retval        Number of bytes process by user
 */
// static size_t header_callback(void *ptr, size_t size, size_t nmemb, void *context)
ZWU_HTTP_CLIENT_DEFINE_HEADER_CALLBACK(header_callback, zwu_http_client_t)

/**
 * @brief curl debug function callback
 * @param[in]     handle                  Curl handle
 * @param[in]     type                    Information type
 * @param[in]     data                    Data, NOT be zero terminated
 * @param[in]     size                    Data size
 * @param[in]     userp                   User context
 * @retval        0
 */
static int debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userp)
{
	return zwu_http_client_option_debug_callback(handle, type, data, size, userp);
}

// static void http2_ping_callback(uint8_t flag, const uint8_t opaque_data[8], void *context)
ZWU_HTTP_CLIENT_DEFINE_HTTP2_PING_CALLBACK(http2_ping_callback, zwu_http_client_t)

/**
 * @brief Perfrom http request
 * @param[in]   http_client            Http client object
 * @param[in]   request                Http request enum
 * @param[in]   post_buffer            Http post buffer
 * @param[in]   post_buffer_size       Http post buffer size
 * @param[in]   post_read_callback     Post callback
 * @param[in]   context                Post callback context
 * @param[out]  http_response_header_p Http response header
 * @param[out]  body_p                 Http body buffer
 * @param[out]  body_size_p            Http body size
 * @retval      ZWU Status
 */
static zwu_status_t zwu_http_client_http_request(zwu_http_client_t http_client, zwu_http_client_request_enum_t request, const char *post_buffer, size_t post_buffer_size,
	zwu_http_client_post_read_callback post_read_callback, void *context,
	zwu_http_response_header_t *http_response_header_p, char **body_p, size_t *body_size_p)
{
	CURLcode result;
	char *body_buffer;
	size_t body_buffer_size;
	zwu_http_response_header_t http_response_header;

	if (http_client->nonblock_flag)
	{
		result = CURLE_BAD_FUNCTION_ARGUMENT;  // consider as invalid argument
		goto l_err_not_support_unblock;
	}

	// URL Must not be NULL
	if (!http_client->options.url.set_flag || !http_client->options.url.option)
	{
		result = CURLE_BAD_FUNCTION_ARGUMENT;  // consider as invalid argument
		ZWP_LOG(ZWP_LOG_ERR, "URL is NULL");
		goto l_err_invalid_url;
	}

	// TODO: following code can be made as template.
	// Init libcurl easy handle when necessary
	if (!http_client->curl_easy_handle)
	{
		if ((http_client->curl_easy_handle = curl_easy_init()) == NULL)
		{
			result = CURLE_FAILED_INIT;
			goto l_err_curl_easy_init;
		}
	}

	// Such fixed options can just be called once, right after curl handle initialization.
	if (!http_client->fixed_options_set)
	{
		if ((result = zwu_http_client_option_configure_fixed_easy_options(&http_client->options, http_client->curl_easy_handle, (void*)http_client)) != CURLE_OK)
		{
			goto l_err_curl_set_fixed_easy_options;
		}
		http_client->fixed_options_set = 1;
	}

	// Prepare options, should be called before every request since the option value may change by Setter.
	if ((result = zwu_http_client_option_configure_easy_options(&http_client->options, http_client->curl_easy_handle)) != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "http_client set easy options failed");
		goto l_err_curl_set_easy_options;
	}

	if ((result = zwu_http_client_option_prepare_http_request(&http_client->options, http_client->curl_easy_handle,
		request, post_buffer, post_buffer_size, post_read_callback, context)) != CURLE_OK)
	{
		goto l_err_http_request_prepare;
	}

	// Reset write_buffer if previous request success
	http_client->write_buffer_size = 0;
	ZWP_FREE(http_client->write_buffer);
	http_client->write_buffer = NULL;

	// Reset http_response_header if previous request success
	zwu_http_response_header_release_all_fields(http_client->http_response_header);

	// Reset HTTP2/PING acknowledge
	http_client->ping_ack = 0;
	// End of template

	// Perform the request
	result = curl_easy_perform(http_client->curl_easy_handle);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_perform failed, errno(%d): %s",
			result, strlen(http_client->options.error_buffer) ? http_client->options.error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_perform;
	}

	if (request == ZWU_HTTP_CLIENT_REQUEST_HTTP2_PING)
	{
		// When curl_easy_perform return with CURLE_OK, libcurl should always make sure that ping_ack=true on the ping_callback.
		// Here is to double check.
		if (!http_client->ping_ack)
			goto l_err_curl_easy_perform;
	}
	else
	{
		body_buffer = NULL;
		body_buffer_size = 0;
		if (http_client->write_buffer_size && http_client->write_buffer)
		{
			// One more byte at the end to store zero terminator.
			if ((body_buffer = ZWP_MALLOC(http_client->write_buffer_size+1)) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				goto l_err_body_buffer_alloc;
			}
			memcpy(body_buffer, http_client->write_buffer, http_client->write_buffer_size);
			body_buffer[http_client->write_buffer_size] = '\0';
			body_buffer_size = http_client->write_buffer_size;
		}

		http_response_header = NULL;
		if (zwu_http_response_header_copy(http_client->http_response_header, &http_response_header) != ZWU_STATUS_OK)
		{
			goto l_err_curl_http_response_header_copy;
		}

		if (http_response_header_p)
		{
			*http_response_header_p = http_response_header;
		}
		else
		{
			zwu_http_response_header_destroy(http_response_header);
			http_response_header = NULL;
		}

		if (body_p)
		{
			*body_p = body_buffer;
		}
		else
		{
			ZWP_FREE(body_buffer);
			body_buffer = NULL;
		}

		if (body_size_p)
		{
			*body_size_p = body_buffer_size;
		}
	}

	return ZWU_STATUS_OK;

	zwu_http_response_header_destroy(http_response_header);
l_err_curl_http_response_header_copy:
	ZWP_FREE(body_buffer);
l_err_body_buffer_alloc:
l_err_curl_easy_perform:
l_err_http_request_prepare:
l_err_curl_set_easy_options:
l_err_curl_set_fixed_easy_options:
	// Don't clean up easy handle here. Set option fail or easy_perform fail should not affect easy handle.
	//curl_easy_cleanup(http_client->curl_easy_handle);
l_err_curl_easy_init:
l_err_invalid_url:
l_err_not_support_unblock:
	return zwu_http_client_easy_code_to_zwu_status(result);
}

zwu_status_t zwu_http_client_http_get(zwu_http_client_t http_client,
	zwu_http_response_header_t *http_response_header_p, char **body_p, size_t *body_size_p)
{
	return zwu_http_client_http_request(http_client, ZWU_HTTP_CLIENT_REQUEST_HTTP_GET, NULL, 0, NULL, NULL, http_response_header_p, body_p, body_size_p);
}

zwu_status_t zwu_http_client_http_post_with_callback(zwu_http_client_t http_client, zwu_http_client_post_read_callback post_read_callback, void *context,
	zwu_http_response_header_t *http_response_header_p, char **body_p, size_t *body_size_p)
{
	return zwu_http_client_http_request(http_client, ZWU_HTTP_CLIENT_REQUEST_HTTP_POST_WITH_CALLBACK, NULL, 0, post_read_callback, context,
		http_response_header_p, body_p, body_size_p);
}

zwu_status_t zwu_http_client_http_post(zwu_http_client_t http_client, const char *post_buffer, size_t post_buffer_size,
	zwu_http_response_header_t *http_response_header_p, char **body_p, size_t *body_size_p)
{
	return zwu_http_client_http_request(http_client, ZWU_HTTP_CLIENT_REQUEST_HTTP_POST, post_buffer, post_buffer_size, NULL, NULL,
		http_response_header_p, body_p, body_size_p);
}

zwu_status_t zwu_http_client_http2_ping(zwu_http_client_t http_client) {
	return zwu_http_client_http_request(http_client, ZWU_HTTP_CLIENT_REQUEST_HTTP2_PING, NULL, 0, NULL, NULL,
		NULL, NULL, NULL);
}

/** 
 * @brief Cancel the previous asynchronous operation
 * @param[in]    http_client                   Http client object
 * @retval       ZWU_STATUS_OK                 Success
 * @retval       ZWU_STATUS_ERROR              Failure
 */
static zwu_status_t zwu_http_client_asynchronous_cancel(zwu_http_client_t http_client)
{
	zwu_http_client_request_t request;
	int manage_id;

	if (!http_client->nonblock_flag)
	{
		goto l_err_not_support;
	}

	manage_id = -1;
	memset(&request, 0, sizeof(request));
	request.action = MANAGE_ACTION_REMOVE;
	request.http_client = http_client;
	request.manage_id = http_client->manage_id;

	if (zwu_http_client_manager_process_request(http_client->http_client_manager, &request, &manage_id) != ZWU_STATUS_OK)
	{
		goto l_err_http_client_manager_process_request;
	}

	//http_client->manage_id = manage_id;
	return ZWU_STATUS_OK;

l_err_http_client_manager_process_request:
l_err_not_support:
	return ZWU_STATUS_ERROR;
}

zwu_status_t zwu_http_client_asynchronous_http_get(zwu_http_client_t http_client, zwu_http_client_response_callback response_callback, void *context)
{
	zwu_http_client_request_t request;
	int manage_id;

	if (!http_client->nonblock_flag)
	{
		goto l_err_not_support;
	}

	manage_id = -1;
	memset(&request, 0, sizeof(request));
	request.action = MANAGE_ACTION_ADD;
	request.request_method = ZWU_HTTP_CLIENT_REQUEST_HTTP_GET;
	request.options = &http_client->options;
	request.response_callback = response_callback;
	//request.http2_ping_response_callback = NULL;
	request.response_context = context;
	request.http_client = http_client;
	//request.post_buffer = NULL;
	//request.post_buffer_size = 0;
	request.manage_id = http_client->manage_id;

	if (zwu_http_client_manager_process_request(http_client->http_client_manager, &request, &manage_id) != ZWU_STATUS_OK)
	{
		goto l_err_http_client_manager_process_request;
	}

	http_client->manage_id = manage_id;
	return ZWU_STATUS_OK;

l_err_http_client_manager_process_request:
l_err_not_support:
	return ZWU_STATUS_ERROR;
}

zwu_status_t zwu_http_client_asynchronous_http_post(zwu_http_client_t http_client, const char *post_buffer, size_t post_buffer_size,
	zwu_http_client_response_callback response_callback, void *context)
{
	zwu_http_client_request_t request;
	int manage_id;

	if (!http_client->nonblock_flag)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Asynchronous request on a synchronous handle");
		goto l_err_not_support;
	}

	manage_id = -1;
	memset(&request, 0, sizeof(request));
	request.action = MANAGE_ACTION_ADD;
	request.request_method = ZWU_HTTP_CLIENT_REQUEST_HTTP_POST;
	request.options = &http_client->options;
	request.response_callback = response_callback;
	//request.http2_ping_response_callback = NULL;
	request.response_context = context;
	request.http_client = http_client;
	request.post_buffer = post_buffer;
	request.post_buffer_size = post_buffer_size;
	request.manage_id = http_client->manage_id;

	if (zwu_http_client_manager_process_request(http_client->http_client_manager, &request, &manage_id) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwu_http_client_manager_process_request() failed!");
		goto l_err_http_client_manager_process_request;
	}

	http_client->manage_id = manage_id;
	return ZWU_STATUS_OK;

l_err_http_client_manager_process_request:
l_err_not_support:
	return ZWU_STATUS_ERROR;
}

zwu_status_t zwu_http_client_asynchronous_http2_ping(zwu_http_client_t http_client,
	zwu_http_client_http2_ping_response_callback http2_ping_response_callback, void *context)
{
	zwu_http_client_request_t request;
	int manage_id;

	if (!http_client->nonblock_flag)
	{
		goto l_err_not_support;
	}

	manage_id = -1;
	memset(&request, 0, sizeof(request));
	request.action = MANAGE_ACTION_ADD;
	request.request_method = ZWU_HTTP_CLIENT_REQUEST_HTTP2_PING;
	request.options = &http_client->options;
	//request.response_callback = response_callback;
	request.http2_ping_response_callback = http2_ping_response_callback;
	request.response_context = context;
	request.http_client = http_client;
	//request.post_buffer = post_buffer;
	//request.post_buffer_size = post_buffer_size;
	request.manage_id = http_client->manage_id;

	if (zwu_http_client_manager_process_request(http_client->http_client_manager, &request, &manage_id) != ZWU_STATUS_OK)
	{
		goto l_err_http_client_manager_process_request;
	}

	http_client->manage_id = manage_id;
	return ZWU_STATUS_OK;

l_err_http_client_manager_process_request:
l_err_not_support:
	return ZWU_STATUS_ERROR;
}

zwu_status_t zwu_http_client_create(zwu_http_client_t *http_client_p)
{
	zwu_http_client_t http_client;

	if ((http_client = (zwu_http_client_t)ZWP_MALLOC(sizeof(*http_client))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_curl_alloc;
	}
	http_client->nonblock_flag = 0;

	// Init http_client options with default value
	zwu_http_client_option_init_with_default_value(&http_client->options);
	http_client->options.write_callback = write_callback;
	http_client->options.header_callback = header_callback;
	http_client->options.debug_callback = debug_callback;
	http_client->options.http2_ping_callback = http2_ping_callback;

	// Initialization data for blocking operation.
	// Lazy initialization, handle will be initiated when start HTTP request.
	http_client->curl_easy_handle = NULL;
	http_client->fixed_options_set = 0;

	http_client->write_buffer = NULL;
	http_client->write_buffer_size = 0;
	http_client->ping_ack = 0;

	if (zwp_pool_create(&http_client->memory_pool, NULL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create memory pool for directory service");
		goto l_err_memory_pool_create;
	}

	if (zwu_http_response_header_create(&http_client->http_response_header) != ZWU_STATUS_OK)
	{
		goto l_err_curl_http_response_header_create;
	}

	http_client->manage_id = -1;
	http_client->lock = NULL;
	http_client->http_version_in_use = ZWU_HTTP_CLIENT_HTTP_VERSION_INVALID;
	http_client->http_client_manager = NULL;

	*http_client_p = http_client;

	return ZWU_STATUS_OK;

	zwu_http_response_header_destroy(http_client->http_response_header);
l_err_curl_http_response_header_create:
	zwp_pool_destroy(http_client->memory_pool);
l_err_memory_pool_create:
	ZWP_FREE(http_client);
l_err_curl_alloc:
	return ZWU_STATUS_ERROR;
}

zwu_status_t zwu_asynchronous_http_client_create_with_manager(zwu_http_client_manager_t http_client_manager, zwu_http_client_t *http_client_p)
{
	zwu_http_client_t http_client;

	if (!http_client_manager)
	{
		goto l_err_invalid_http_client_manager;
	}

	if (zwu_http_client_create(&http_client) != ZWU_STATUS_OK)
	{
		goto l_err_http_client_create;
	}

	if (zwp_thread_mutex_create(&http_client->lock, http_client->memory_pool, ZWP_THREAD_MUTEX_CREATE_NORMAL) != ZWP_STATUS_OK)
	{
		goto l_err_thread_mutex_create;
	}

	http_client->nonblock_flag = 1;
	http_client->http_client_manager = http_client_manager;

	*http_client_p = http_client;
	return ZWU_STATUS_OK;

	zwp_thread_mutex_destroy(http_client->lock);
l_err_thread_mutex_create:
	zwu_http_client_destroy(http_client);
l_err_http_client_create:
l_err_invalid_http_client_manager:
	return ZWU_STATUS_ERROR;
}

zwu_status_t zwu_asynchronous_http_client_create(zwu_http_client_t *http_client_p)
{
	return zwu_asynchronous_http_client_create_with_manager(global_http_client_manager, http_client_p);
}

zwu_status_t zwu_http_client_copy(zwu_http_client_t http_client, zwu_http_client_t *http_client_p)
{
	zwu_http_client_t new_http_client;

	// Create new http client with default value and settings.
	if (zwu_http_client_create(&new_http_client) != ZWU_STATUS_OK)
	{
		goto l_err_http_client_create;
	}

	// Note that we will just copy the data that is common for both block/non-block operation.
	// We just care about the http related options. Other member variables should be set after HTTP request
	if (zwu_http_client_option_copy(&http_client->options, &new_http_client->options) != ZWU_STATUS_OK)
	{
		goto l_err_http_client_option_copy;
	}

	// Donot copy manage_id, let http_client_manager assign new one.

	// Cannot copy mutex, let's create new one for nonblocking request.
	if (http_client->nonblock_flag)
	{
		if (zwp_thread_mutex_create(&new_http_client->lock, http_client->memory_pool, ZWP_THREAD_MUTEX_CREATE_NORMAL) != ZWP_STATUS_OK)
		{
			goto l_err_thread_mutex_create;
		}
		new_http_client->http_client_manager = http_client->http_client_manager;
	}

	// Finally copy unblock flag
	new_http_client->nonblock_flag = http_client->nonblock_flag;

	*http_client_p = new_http_client;

	return ZWU_STATUS_OK;

	if (http_client->nonblock_flag)
	{
		zwp_thread_mutex_destroy(new_http_client->lock);
	}
l_err_thread_mutex_create:
l_err_http_client_option_copy:
 	// zwu_http_client_destroy should also destroy all the options inside it.
	zwu_http_client_destroy(new_http_client);
l_err_http_client_create:
	return ZWU_STATUS_ERROR;
}

void zwu_http_client_destroy(zwu_http_client_t http_client)
{
	zwu_http_response_header_destroy(http_client->http_response_header);
	ZWP_FREE(http_client->write_buffer);
	zwu_http_client_option_destroy(&http_client->options);
	if (http_client->curl_easy_handle)
	{
		curl_easy_cleanup(http_client->curl_easy_handle);
	}
	if (http_client->nonblock_flag)
	{
		// Note that we don't keep track of the request status, and this api may return error if
		// we cancel the request twice. But it won't affect anything.
		zwu_http_client_asynchronous_cancel(http_client);
		// Once canceled, zwu_http_client_manager should not acccess http_client anymore.
		// Then we can safely destroy the lock.
		zwp_thread_mutex_destroy(http_client->lock);
	}
	zwp_pool_destroy(http_client->memory_pool);
	ZWP_FREE(http_client);
}

zwu_status_t zwu_http_client_global_init(void)
{
	if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK)
	{
		goto l_err_curl_global_init;
	}

	if (zwu_http_client_manager_create(&global_http_client_manager) != ZWU_STATUS_OK)
	{
		goto l_err_http_client_manager_create;
	}

	if (zwu_http_client_manager_start(global_http_client_manager) != ZWU_STATUS_OK)
	{
		goto l_http_client_manager_start;
	}

	return ZWU_STATUS_OK;

	zwu_http_client_manager_stop(global_http_client_manager);
l_http_client_manager_start:
	zwu_http_client_manager_destroy(global_http_client_manager);
l_err_http_client_manager_create:
	curl_global_cleanup();
l_err_curl_global_init:
	return ZWU_STATUS_ERROR;
}

void zwu_http_client_global_cleanup(void)
{
	zwu_http_client_manager_stop(global_http_client_manager);
	zwu_http_client_manager_destroy(global_http_client_manager);
	curl_global_cleanup();
}

zwu_status_t zwu_http_client_asynchronous_disconnect_all(void)
{
	return zwu_http_client_manager_disconnect(global_http_client_manager);
}

#if ZWU_HTTP_CLIENT_TEST
#include <stdlib.h>
#include <time.h>
zwu_status_t zwu_http_client_test()
{
	zwu_http_client_t curl_test;
	zwu_http_client_t dup_curl_test;
	char *http_body;
	size_t http_body_size;
	zwu_status_t status;
	int retry_count;
	int slot_count;
	const int retry_count_max = 10;
	zwu_http_response_header_t http_response_header;
	int redirect;
	int http_status_code;
	char *ptr;
	size_t ptr_length;
	char *http_status_line;
	int sleep_time;
	uint8_t opaque_data[8] = {0};
	int i;

	if (zwu_http_client_global_init() != ZWU_STATUS_OK)
	{
		goto l_err_http_client_global_init;
	}

	if (zwu_http_client_create(&curl_test) != ZWU_STATUS_OK)
	{
		goto l_err_curl_create;
	}

	slot_count = 1;
	http_body = NULL;
	http_body_size = 0;
	retry_count = 0;
	http_response_header = NULL;
	redirect = 0;
	http_status_code = 0;
	ptr = NULL;
	ptr_length = 0;
	http_status_line = NULL;
	sleep_time = 1;
	srand(time(NULL));
	zwu_http_client_set_verbose(curl_test, ZWU_HTTP_CLIENT_VERBOSE_ON);
	zwu_http_client_set_pipe_wait(curl_test, ZWU_HTTP_CLIENT_PIPE_WAIT_ON);
	zwu_http_client_set_ssl_enable_alpn(curl_test, ZWU_HTTP_CLIENT_SSL_ENABLE_ALPN_ENABLE);
	zwu_http_client_set_http_version(curl_test, ZWU_HTTP_CLIENT_HTTP_VERSION_2_TLS);
	//zwu_http_client_set_use_ssl(curl_test, ZWU_HTTP_CLIENT_USE_SSL_ALL);
	//zwu_http_client_set_ssl_version(curl_test, ZWU_HTTP_CLIENT_SSL_VERSION_TLSV1_2);
	//zwu_http_client_set_ssl_verify_peer(curl_test, ZWU_HTTP_CLIENT_SSL_VERIFY_PEER_ENABLE);
	//zwu_http_client_set_ssl_verify_host(curl_test, ZWU_HTTP_CLIENT_SSL_VERIFY_HOST_ENABLE);

#if 0
	// Refer to https://www.openssl.org/docs/man1.0.2/apps/ciphers.html */
	zwu_http_client_set_ssl_cipher_list(curl_test, "ECDHE-ECDSA-AES128-GCM-SHA256,ECDHE-RSA-AES128-GCM-SHA256");
#else
	zwu_http_client_set_ssl_cipher_list(curl_test, "ALL");
#endif
	zwu_http_client_set_ca_info(curl_test, "/etc/ssl/certs/ca-certificates.crt");
	//zwu_http_client_set_base_hostname(curl_test, "images.apple.com");
	zwu_http_client_set_connect_timeout(curl_test, 10);
	zwu_http_client_set_timeout(curl_test, 15);
	zwu_http_client_set_url(curl_test, "https://www.google.com");
	zwu_http_client_set_http_header(curl_test, "Content-Type: application/json");
	zwu_http_client_set_http_header(curl_test, "X-protocol-version: 1");
	zwu_http_client_set_http_header(curl_test, "Accept: application/json");
	// To clear header
	// zwu_http_client_set_http_header(curl_test, "Accept:");

	do
	{
		ZWP_FREE(http_body);
		zwu_http_response_header_destroy(http_response_header);
		http_body = NULL;
		http_response_header = NULL;
#if 1  // GET test
		status = zwu_http_client_http_get(curl_test, &http_response_header, &http_body, &http_body_size);
#else  // POST test
		status = zwu_http_client_http_post(curl_test, "{test}", strlen("{test}"), &http_response_header, &http_body, &http_body_size);
#endif
		if (status == ZWU_STATUS_ETIME)
		{
			slot_count *= 2;
			sleep_time = random() % slot_count;
			usleep(sleep_time*1000000);
			++retry_count;
			ZWP_LOG(ZWP_LOG_DEBUG, "server is unreachable! slot_count: %d, sleep_time: %d seconds, retry_count: %d, retry_count_max: %d",
				slot_count, sleep_time, retry_count, retry_count_max);
		}
		else if (status == ZWU_STATUS_OK)
		{
			if (zwu_http_response_header_get_status_line(http_response_header, &ptr) == ZWU_STATUS_OK)
			{
				if (sscanf(ptr, "%*s %d %*s", &http_status_code)
					&& (http_status_code == 302)
					&& (zwu_http_response_header_search_field(http_response_header, "Location", &ptr, &ptr_length) == ZWU_STATUS_OK
						|| zwu_http_response_header_search_field(http_response_header, "location", &ptr, &ptr_length) == ZWU_STATUS_OK))
				{
					// Can redirect using curl option: CURLOPT_FOLLOWLOCATION
					redirect = 1;
					ZWP_LOG(ZWP_LOG_DEBUG, "Will redirect to: %s", ptr);
					zwu_http_client_set_url(curl_test, ptr);
					slot_count = 1;
					retry_count = 0;
				}
			}
			if (redirect)
			{
				redirect = 0;
				continue;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	} while (retry_count < retry_count_max);

	if (status != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "HTTP Request failed");
		goto l_err_curl_http_get;
	}

	if (zwu_http_response_header_get_status_line(http_response_header, &http_status_line) == ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "HTTP Status line: %s", http_status_line);
	}

	if (zwu_http_response_header_search_field(http_response_header, "Date", &ptr, &ptr_length) == ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "HTTP response header -- Date: %s", ptr);
	}

	if ((zwu_http_response_header_search_field(http_response_header, "Cache-Control", &ptr, &ptr_length) == ZWU_STATUS_OK)
		|| (zwu_http_response_header_search_field(http_response_header, "Cache-control", &ptr, &ptr_length) == ZWU_STATUS_OK)
		|| (zwu_http_response_header_search_field(http_response_header, "cache-control", &ptr, &ptr_length) == ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "HTTP response header -- Cache-Control: %s", ptr);
	}

	if (zwu_http_client_get_http_version(curl_test) == ZWU_HTTP_CLIENT_HTTP_VERSION_2_0)
	{
		// Set HTTP2/PING opaque data with random value.
		for (i=0; i<sizeof(opaque_data)/sizeof(opaque_data[0]); i++)
		{
			opaque_data[i] = random() % 256;
		}
		ZWP_LOG(ZWP_LOG_DEBUG, "HTTP2/PING opaque data:");
		ZWP_LOG_HEX(ZWP_LOG_DEBUG, (unsigned char*) opaque_data, 8);
		zwu_http_client_set_http2_ping_opaque_data(curl_test, opaque_data);
		// It will reuse corrent connection
		if (zwu_http_client_http2_ping(curl_test) != ZWU_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "HTTP2/PING fail for curl_test!");
		}
	}
	// For debug
	//getchar();

	if (zwu_http_client_copy(curl_test, &dup_curl_test) != ZWU_STATUS_OK)
	{
		goto l_err_http_client_copy;
	}

	// Re-Set HTTP2/PING opaque data with random value.
	for (i=0; i<sizeof(opaque_data)/sizeof(opaque_data[0]); i++)
	{
		opaque_data[i] = random() % 256;
	}
	ZWP_LOG(ZWP_LOG_DEBUG, "HTTP2/PING opaque data:");
	ZWP_LOG_HEX(ZWP_LOG_DEBUG, (unsigned char*) opaque_data, 8);
	// Set opaque data again, else it will use old value.
	zwu_http_client_set_http2_ping_opaque_data(dup_curl_test, opaque_data);
	// Make sure should use HTTP2 over TLS, so that negotiation for HTTP2 should be confirmed during SSL handshake.
	zwu_http_client_set_http_version(dup_curl_test, ZWU_HTTP_CLIENT_HTTP_VERSION_2_TLS);
	// It will create new connection
	if (zwu_http_client_http2_ping(dup_curl_test) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "HTTP2/PING fail for dup_curl_test!");
	}
	// For debug
	//getchar();

	ZWP_LOG(ZWP_LOG_DEBUG, "HTTP request body: %s", http_body);

	ZWP_FREE(http_body);
	zwu_http_response_header_destroy(http_response_header);
	zwu_http_client_destroy(dup_curl_test);
	zwu_http_client_destroy(curl_test);
	zwu_http_client_global_cleanup();
	return ZWU_STATUS_OK;

	zwu_http_client_destroy(dup_curl_test);
l_err_http_client_copy:
	ZWP_FREE(http_body);
	zwu_http_response_header_destroy(http_response_header);
l_err_curl_http_get:
	zwu_http_client_destroy(curl_test);
l_err_curl_create:
	zwu_http_client_global_cleanup();
l_err_http_client_global_init:
	return ZWU_STATUS_ERROR;
}

#endif

/**
 * @}
 */
