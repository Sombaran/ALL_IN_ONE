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
 * @file    zwu_http_client_manager.c
 *
 * @brief   HTTP Client Manager
 * @details Handle HTTP(S) client with curl.
 *
 * @author  Zelin
 *
 * @version 1.0 - 2017-01-06
 * - Initial version
 */
#include <fcntl.h>              /* Obtain O_* constant definitions */
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "zwu_http_client_manager.h"
#include "curl/curl.h"
#include "zwu_http_client.h"
#include "zwu_http_client_option.h"
#include "zwp_memory.h"
#include "zwu_log.h"
#include "zwp_error.h"
#include "zwp_thread_mutex.h"
#include "zwp_thread.h"
#include "zwp_thread_cond.h"
#include "zwu_array_list.h"

#define ZWU_HTTP_CLIENT_MANAGER_DEFAULT_ARRAY_SIZE 20

typedef enum {
	MANAGE_ENTITY_STATUS_INIT,          /**< Entity is initiated */
	MANAGE_ENTITY_STATUS_TO_ADD,        /**< Entity request is to be added */
	MANAGE_ENTITY_STATUS_ADDED,         /**< Entity request is added */
	MANAGE_ENTITY_STATUS_TO_REMOVE,     /**< Entity request is to be removed */
	MANAGE_ENTITY_STATUS_REMOVED,       /**< Entity request is removed */
} zwu_http_client_manager_entity_status_enum_t;

typedef enum {
	MANAGE_NOTIFICATION_TERMINATE = -1,      /**< Terminate process thread */
	MANAGE_NOTIFICATION_DISCONNECT = -2,    /**< Destroy all the socket connections */
} zwu_http_client_manager_notification_enum_t;

typedef struct _zwu_http_client_manager_entity *zwu_http_client_manager_entity_t;
struct _zwu_http_client_manager_entity
{
	zwu_http_client_manager_entity_status_enum_t status;                         /**< Current entity status */
	zwu_http_client_t http_client;                                               /**< http_client ID that current entity is serving */

	// Data to handle request related.
	zwu_http_client_request_enum_t request_method;                               /**< HTTP client request method */
	zwu_http_client_option_t options;                                            /**< HTTP client options */
	zwu_http_client_response_callback response_callback;                         /**< HTTP response callback */
	zwu_http_client_http2_ping_response_callback http2_ping_response_callback;   /**< HTTP2 PING response callback */
	void *response_context;                                                      /**< Response context pointer */

	// Data to handle curl related
	CURL *curl_easy_handle;                           /**< CURL easy handle */

	int fixed_options_set;                            /**< Flag to indicate if already set the fixed options for current curl handle */

	char *write_buffer;                               /**< Buffer for write_callback */
	size_t write_buffer_size;                         /**< Buffer size for write_callback */

	int ping_ack;                                     /**< Acknowledge for HTTP2/PING request*/

	zwu_http_response_header_t http_response_header;  /**< Buffer for header_callback */

	char *post_buffer;                                /**< Post buffer */
	size_t post_buffer_size;                          /**< Post buffer size */
};

typedef struct _zwu_http_client_manager *zwu_http_client_manager_t;
struct _zwu_http_client_manager
{
	int pipefd[2];                                        /**< Pipe to notify process thread */

	zwp_pool_t memory_pool;                               /**< Memory pool */
	zwp_thread_mutex_t lock;                              /**< Lock to protect array */
	zwu_array_list_t array;                               /**< Array that stores all the entities. As elements in array can be reset and be reused,
															   elements won't be release and the array size will be always increasing */
	int terminated;                                       /**< Flag to indicate termination of process thread */
	zwp_thread_cond_t disconnect_wait;                    /**< Condition variable to wait until all connections closed */
	int disconnected;                                     /**< Flag to indicate all connections are closed */

	zwp_thread_t thread_id;                               /**< Thread id that running the process thread */

	CURLM *curl_multi_handle;                             /**< CURL multi-handle */
};

/**
 * @brief curl write callback
 * @param[in]     ptr                     Http client enum value
 * @param[in]     size                    Member size
 * @param[in]     nmemb                   Member count
 * @param[in]     context                 User context
 * @retval        Number of bytes process by user
 */
// static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *context)
ZWU_HTTP_CLIENT_DEFINE_WRITE_CALLBACK(entity_write_callback, zwu_http_client_manager_entity_t)

/**
 * @brief curl header callback
 * @param[in]     ptr                     Http client enum value
 * @param[in]     size                    Member size
 * @param[in]     nmemb                   Member count
 * @param[in]     context                 User context
 * @retval        Number of bytes process by user
 */
// static size_t header_callback(void *ptr, size_t size, size_t nmemb, void *context)
ZWU_HTTP_CLIENT_DEFINE_HEADER_CALLBACK(entity_header_callback, zwu_http_client_manager_entity_t)

/**
 * @brief curl debug function callback
 * @param[in]     handle                  Curl handle
 * @param[in]     type                    Information type
 * @param[in]     data                    Data, NOT be zero terminated
 * @param[in]     size                    Data size
 * @param[in]     userp                   User context
 * @retval        0
 */
static int entity_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userp)
{
	return zwu_http_client_option_debug_callback(handle, type, data, size, userp);
}

/**
 * @brief curl HTTP2 PING callback
 * @param[in]     flag                    HTTP2 PING frame flag
 * @param[in]     opaque_data             HTTP2 PING frame opaque data
 * @param[in]     context                 User context
 * @retval        Nothing
 */
// static void http2_ping_callback(uint8_t flag, const uint8_t opaque_data[8], void *context)
ZWU_HTTP_CLIENT_DEFINE_HTTP2_PING_CALLBACK(entity_http2_ping_callback, zwu_http_client_manager_entity_t)

/**
 * @brief Init simple member variables for manager entity
 * @param[out]    http_client_manager_entity       The http client manager entity object
 * @return        Nothing
 */
static void zwu_http_client_manager_entity_member_initialization(zwu_http_client_manager_entity_t http_client_manager_entity)
{
	http_client_manager_entity->status = MANAGE_ENTITY_STATUS_INIT;
	http_client_manager_entity->http_client = NULL;
	http_client_manager_entity->request_method = ZWU_HTTP_CLIENT_REQUEST_HTTP_GET;  /* will be set to proper value when new request coming */

	// Set options with default value
	zwu_http_client_option_init_with_default_value(&http_client_manager_entity->options);
	http_client_manager_entity->options.write_callback = entity_write_callback;
	http_client_manager_entity->options.header_callback = entity_header_callback;
	http_client_manager_entity->options.debug_callback = entity_debug_callback;
	http_client_manager_entity->options.http2_ping_callback = entity_http2_ping_callback;

	http_client_manager_entity->response_callback = NULL;
	http_client_manager_entity->http2_ping_response_callback = NULL;
	http_client_manager_entity->response_context = NULL;

	// Lazy initialization, handle will be initiated when start HTTP request.
	http_client_manager_entity->curl_easy_handle = NULL;
	http_client_manager_entity->fixed_options_set = 0;

	http_client_manager_entity->write_buffer = NULL;
	http_client_manager_entity->write_buffer_size = 0;
	http_client_manager_entity->ping_ack = 0;

	http_client_manager_entity->post_buffer = NULL;
	http_client_manager_entity->post_buffer_size = 0;
}

/**
 * @brief Destroy simple member variables for manager entity
 * @param[out]    http_client_manager_entity       The http client manager entity object
 * @return        Nothing
 */
static void zwu_http_client_manager_entity_member_destroy(zwu_http_client_manager_entity_t http_client_manager_entity)
{
	if (http_client_manager_entity->curl_easy_handle)
	{
		curl_easy_cleanup(http_client_manager_entity->curl_easy_handle);
	}
	ZWP_FREE(http_client_manager_entity->post_buffer);
	ZWP_FREE(http_client_manager_entity->write_buffer);
	zwu_http_client_option_destroy(&http_client_manager_entity->options);
}

/**
 * @brief Create a http client manager entity object
 * @param[out]    http_client_manager_entity_p      The new http client manager entity object
 * @retval        ZWU_STATUS_OK                     Success
 * @retval        ZWU_STATUS_ERROR                  Failure
 */
static zwu_status_t zwu_http_client_manager_entity_create(zwu_http_client_manager_entity_t *http_client_manager_entity_p)
{
	zwu_http_client_manager_entity_t http_client_manager_entity;

	if ((http_client_manager_entity = (zwu_http_client_manager_entity_t)ZWP_MALLOC(sizeof(*http_client_manager_entity))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_http_client_manager_entity_alloc;
	}

	if (zwu_http_response_header_create(&http_client_manager_entity->http_response_header) != ZWU_STATUS_OK)
	{
		goto l_err_curl_http_response_header_create;
	}

	// Init the other simple member variable
	zwu_http_client_manager_entity_member_initialization(http_client_manager_entity);

	*http_client_manager_entity_p = http_client_manager_entity;

	return ZWU_STATUS_OK;

	zwu_http_response_header_destroy(http_client_manager_entity->http_response_header);
l_err_curl_http_response_header_create:
	ZWP_FREE(http_client_manager_entity);
l_err_http_client_manager_entity_alloc:
	return ZWU_STATUS_ERROR;
}

/**
 * @brief Destroy http client manager entity object
 * @param[in]     http_client_manager_entity       The http client manager entity object
 * @return        Nothing
 */
static void zwu_http_client_manager_entity_destroy(zwu_http_client_manager_entity_t http_client_manager_entity)
{
	zwu_http_client_manager_entity_member_destroy(http_client_manager_entity);
	zwu_http_response_header_destroy(http_client_manager_entity->http_response_header);
	ZWP_FREE(http_client_manager_entity);
}

/**
 * @brief Reset http client manager entity object
 * @param[in]     http_client_manager_entity       The http client manager entity object
 * @return        Nothing
 */
static void zwu_http_client_manager_entity_reset(zwu_http_client_manager_entity_t http_client_manager_entity)
{
	zwu_http_client_manager_entity_member_destroy(http_client_manager_entity);
	// Reset http_response_header if previous request success
	zwu_http_response_header_release_all_fields(http_client_manager_entity->http_response_header);
	// Re-init other member variables
	zwu_http_client_manager_entity_member_initialization(http_client_manager_entity);
}

/**
 * @brief Process status MANAGE_ENTITY_STATUS_TO_ADD for manager entity
 * @param[in]     http_client_manager_entity       The http client manager entity object
 * @param[in]     curl_multi_handle                CURL multi-handle
 * @retval        ZWU_STATUS_OK                    Success
 * @retval        ZWU_STATUS_ERROR                 Failure
 */
static zwu_status_t zwu_http_client_manager_entity_process_status_to_add(zwu_http_client_manager_entity_t http_client_manager_entity, CURLM *curl_multi_handle)
{
	CURLcode result;
	CURLMcode multi_result;

	if (http_client_manager_entity->curl_easy_handle == NULL)
	{
		if ((http_client_manager_entity->curl_easy_handle = curl_easy_init()) == NULL)
		{
			result = CURLE_FAILED_INIT;
			goto l_err_curl_easy_init;
		}
	}

	// Such fixed options can just be called once, right after curl handle initialization.
	if (!http_client_manager_entity->fixed_options_set)
	{
		if ((result = zwu_http_client_option_configure_fixed_easy_options(&http_client_manager_entity->options,
			http_client_manager_entity->curl_easy_handle, (void*)http_client_manager_entity)) != CURLE_OK)
		{
			goto l_err_curl_set_fixed_easy_options;
		}
		http_client_manager_entity->fixed_options_set = 1;
	}

	// Prepare options, should be called before every request since the option value may change by Setter.
	if ((result = zwu_http_client_option_configure_easy_options(&http_client_manager_entity->options, http_client_manager_entity->curl_easy_handle)) != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "http_client_manager_entity set easy options failed");
		goto l_err_curl_set_easy_options;
	}

	if ((result = zwu_http_client_option_prepare_http_request(&http_client_manager_entity->options, http_client_manager_entity->curl_easy_handle,
		http_client_manager_entity->request_method, http_client_manager_entity->post_buffer, http_client_manager_entity->post_buffer_size, NULL, NULL)) != CURLE_OK)
	{
		goto l_err_http_request_prepare;
	}

	// Reset write_buffer if previous request success
	http_client_manager_entity->write_buffer_size = 0;
	ZWP_FREE(http_client_manager_entity->write_buffer);
	http_client_manager_entity->write_buffer = NULL;

	// Reset http_response_header if previous request success
	zwu_http_response_header_release_all_fields(http_client_manager_entity->http_response_header);

	// Reset HTTP2/PING acknowledge
	http_client_manager_entity->ping_ack = 0;

	// Set private context attached with curl_easy_handle;
	if ((result = curl_easy_setopt(http_client_manager_entity->curl_easy_handle, CURLOPT_PRIVATE, (void*)http_client_manager_entity)) != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_PRIVATE failed: %s",
			strlen(http_client_manager_entity->options.error_buffer) ? http_client_manager_entity->options.error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_set_private;
	}

	// Add to multi stack
	if ((multi_result = curl_multi_add_handle(curl_multi_handle, http_client_manager_entity->curl_easy_handle)) != CURLM_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_multi_add_handle failed: %s", curl_multi_strerror(multi_result));
		// Map CURLMcode to CURLcode. Here just simply map the OUT_OF_MEMORY code, other CURLMcodes are mapped to CURLE_FAILED_INIT.
		result = multi_result == CURLM_OUT_OF_MEMORY ? CURLE_OUT_OF_MEMORY : CURLE_FAILED_INIT;
		goto l_err_curl_multi_add_handle;
	}

	// Added
	http_client_manager_entity->status = MANAGE_ENTITY_STATUS_ADDED;

	return ZWU_STATUS_OK;

	curl_multi_remove_handle(curl_multi_handle, http_client_manager_entity->curl_easy_handle);
l_err_curl_multi_add_handle:
l_err_curl_easy_set_private:
l_err_http_request_prepare:
l_err_curl_set_easy_options:
l_err_curl_set_fixed_easy_options:
	// Don't clean up easy handle here. Set option fail or easy_perform fail should not affect easy handle.
	//curl_easy_cleanup(http_client_manager_entity->curl_easy_handle);
l_err_curl_easy_init:	
	return zwu_http_client_easy_code_to_zwu_status(result);
}

/**
 * @brief Process status MANAGE_ENTITY_STATUS_TO_REMOVE for manager entity
 * @param[in]     http_client_manager_entity       The http client manager entity object
 * @param[in]     curl_multi_handle                CURL multi-handle
 * @retval        ZWU_STATUS_OK                    Success
 * @retval        ZWU_STATUS_ERROR                 Failure
 */
static zwu_status_t zwu_http_client_manager_entity_process_status_to_remove(zwu_http_client_manager_entity_t http_client_manager_entity, CURLM *curl_multi_handle)
{
	CURLMcode multi_result;

	// Removing an easy handle while being used is perfectly legal and will effectively halt the transfer in progress involving that easy handle.
	// All other easy handles and transfers will remain unaffected.
	if ((multi_result = curl_multi_remove_handle(curl_multi_handle, http_client_manager_entity->curl_easy_handle)) != CURLM_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_multi_remove_handle failed: %s", curl_multi_strerror(multi_result));
		goto l_err_curl_multi_remove_handle;
	}
	http_client_manager_entity->status = MANAGE_ENTITY_STATUS_REMOVED;

	return ZWU_STATUS_OK;

l_err_curl_multi_remove_handle:
	return ZWU_STATUS_ERROR;
}

/**
 * @brief Process manager entity
 * @param[in]     http_client_manager_entity       The http client manager entity object
 * @param[in]     curl_multi_handle                CURL multi-handle
 * @retval        ZWU_STATUS_OK                    Success
 * @retval        ZWU_STATUS_ERROR                 Failure
 */
static zwu_status_t zwu_http_client_manager_entity_process(zwu_http_client_manager_entity_t http_client_manager_entity, CURLM *curl_multi_handle)
{
	zwu_status_t status;

	status = ZWU_STATUS_OK;
	switch(http_client_manager_entity->status)
	{
		case MANAGE_ENTITY_STATUS_TO_ADD:
			status = zwu_http_client_manager_entity_process_status_to_add(http_client_manager_entity, curl_multi_handle);
			break;
		case MANAGE_ENTITY_STATUS_TO_REMOVE:
			status = zwu_http_client_manager_entity_process_status_to_remove(http_client_manager_entity, curl_multi_handle);
			break;
		default:
			status = ZWU_STATUS_ERROR;
			break;
	}

	return status;
}

/**
 * @brief Post process manager entity
 * @param[in]     http_client_manager_entity       The http client manager entity object
 * @param[in]     curl_multi_handle                CURL multi-handle
 * @retval        ZWU_STATUS_OK                    Success
 * @retval        ZWU_STATUS_ERROR                 Failure
 */
static zwu_status_t zwu_http_client_manager_entity_post_process(zwu_http_client_manager_entity_t http_client_manager_entity, CURLcode curl_result)
{
	int should_notify_user_app;
	zwu_http_client_response_callback response_callback_backup;
	zwu_http_client_http2_ping_response_callback http2_ping_response_callback_backup;
	void *response_context_backup;
	zwu_http_client_request_enum_t request_method_backup;
	zwu_status_t zwu_result;
	char *realloc_write_buffer;
	long curl_http_version;

	should_notify_user_app = 0;
	// During callback, the member variable, like request_method, *_response_callback, context, options, within current entity maybe modified.
	// So we backup the vairable that maybe modified here.
	response_callback_backup = http_client_manager_entity->response_callback;
	http2_ping_response_callback_backup = http_client_manager_entity->http2_ping_response_callback;
	response_context_backup = http_client_manager_entity->response_context;;
	request_method_backup = http_client_manager_entity->request_method;
	zwu_result = zwu_http_client_easy_code_to_zwu_status(curl_result);

	// we just focus on entity that with status ADDED to multi-handle.
	if (http_client_manager_entity->status != MANAGE_ENTITY_STATUS_ADDED
		&& http_client_manager_entity->status != MANAGE_ENTITY_STATUS_TO_REMOVE)  // It's possible that we haven't remove easy handle from multi-hanle at this stage
		goto l_err_invalid_status;

	// At this moment we can release the http_client_option.
	zwu_http_client_option_destroy(&http_client_manager_entity->options);
	zwu_http_client_option_init_with_default_value(&http_client_manager_entity->options);

	// If current status is TO_REMOVE, http_client may already be released by user application. At this moment we should not access http_client any more.
	// So here we just handle the case for status ADDED.
	if (http_client_manager_entity->status == MANAGE_ENTITY_STATUS_ADDED)
	{
		if (curl_easy_getinfo(http_client_manager_entity->curl_easy_handle, CURLINFO_HTTP_VERSION, &curl_http_version) == CURLE_OK)
		{
			zwu_http_client_set_http_version_in_use(http_client_manager_entity->http_client,
				zwu_http_client_option_curl_value_to_zwu_value_http_version(curl_http_version));
		}
		should_notify_user_app = 1;
	}

	http_client_manager_entity->status = MANAGE_ENTITY_STATUS_REMOVED;

	// After settling down everything, let's active the callback.
	// User may do asynchronous HTTP request again, since we are using RECURSIVE mutex,
	// or even destory http_client, as not we don't have dependency on http_client.
	// But user is not allowed to call disconnect(), as it will cause dead lock.
	if (should_notify_user_app)
	{
		if (request_method_backup == ZWU_HTTP_CLIENT_REQUEST_HTTP2_PING)
		{
			if (http2_ping_response_callback_backup)
			{
				http2_ping_response_callback_backup(http_client_manager_entity->ping_ack ? zwu_result : ZWU_STATUS_ERROR, response_context_backup);
			}
		}
		else
		{
			if (response_callback_backup)
			{
				// If reallocation successfull, then append zero-terminator.
				if ((realloc_write_buffer = ZWP_REALLOC(http_client_manager_entity->write_buffer, http_client_manager_entity->write_buffer_size + 1)) != NULL)
				{
					http_client_manager_entity->write_buffer = realloc_write_buffer;
					http_client_manager_entity->write_buffer[http_client_manager_entity->write_buffer_size] = '\0';
				}
				response_callback_backup(zwu_result, http_client_manager_entity->http_response_header, http_client_manager_entity->write_buffer,
					http_client_manager_entity->write_buffer_size, response_context_backup);
			}
		}
	}

	return ZWU_STATUS_OK;
l_err_invalid_status:
	return ZWU_STATUS_ERROR;
}

/* ======================================================================== */
/* Starting of zwu_http_client_manager */
/* ======================================================================== */
/**
 * @brief Prepare CURL multi-handle
 * @param[in]     http_client_manager         The http client manager object
 * @retval        ZWU_STATUS_OK               Success
 * @retval        ZWU_STATUS_ERROR            Failure
 */
static zwu_status_t zwu_http_client_manager_prepare_curl_multi_handle(zwu_http_client_manager_t http_client_manager)
{
	CURLMcode multi_result;

	if ((http_client_manager->curl_multi_handle = curl_multi_init()) == NULL)
	{
		goto l_err_curl_multi_init;
	}

	if ((multi_result = curl_multi_setopt(http_client_manager->curl_multi_handle, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX)) != CURLM_OK)
	{
		goto l_err_curl_multi_setopt_pipeling;
	}

	return ZWU_STATUS_OK;
l_err_curl_multi_setopt_pipeling:
	curl_multi_cleanup(http_client_manager->curl_multi_handle);
	http_client_manager->curl_multi_handle = NULL;
l_err_curl_multi_init:
	return ZWU_STATUS_ERROR;
}

/**
 * @brief Process thread to handle http request from user application, to handle curl IO.
 * @param[in]     thread                           The thread object
 * @param[in]     arg                              User context
 * @retval        NULL
 */
static void *ZWP_THREAD_FUNC zwu_http_client_manager_process(zwp_thread_t thread, void *arg)
{
	zwu_http_client_manager_t http_client_manager;
	CURLM *curl_multi_handle;
	CURL *curl_easy_handle;
	CURLMcode multi_result;
	struct curl_waitfd extra_fds;
	long timeout;
	int number_of_fds;
	zwu_status_t thread_result;
	int manage_id;
	zwu_http_client_manager_entity_t entity;
	int array_size;
	struct CURLMsg *curl_message;
	int i;
	int terminate;
	int disconnect;
	int still_running;
	int message_left;

	http_client_manager = (zwu_http_client_manager_t)arg;
	curl_multi_handle = http_client_manager->curl_multi_handle;
	curl_easy_handle = NULL;
	multi_result = CURLM_OK;

	memset(&extra_fds, 0, sizeof(extra_fds));
	extra_fds.fd = http_client_manager->pipefd[0];
	extra_fds.events = CURL_WAIT_POLLIN;

	timeout = -1;
	number_of_fds = 0;
	thread_result = ZWU_STATUS_OK;
	manage_id = -1;
	entity = NULL;
	array_size = 0;
	curl_message = NULL;
	terminate = 0;
	disconnect = 0;
	still_running = 0;  // No use yet. Just to feed libcurl
	message_left = 0;  // No use yet. Just to feed libcurl

	while(1)
	{
		if (curl_multi_timeout(curl_multi_handle, &timeout) != CURLM_OK)
		{
			timeout = -1;  // infinite timeout
		}

		number_of_fds = 0;
		if ((multi_result = curl_multi_wait(curl_multi_handle, &extra_fds, 1, timeout, &number_of_fds)) != CURLM_OK)
		{
			thread_result = ZWU_STATUS_ERROR;
			ZWP_LOG(ZWP_LOG_ERR, "curl_multi_wait failed: %s", curl_multi_strerror(multi_result));
			break;
		}

		if (extra_fds.revents & CURL_WAIT_POLLIN)
		{
			extra_fds.revents = 0;
			// Process all the data in pipifd[0]
			while (read(http_client_manager->pipefd[0], &manage_id, sizeof(manage_id)) == sizeof(manage_id))
			{
				if (manage_id == MANAGE_NOTIFICATION_TERMINATE)
				{
					// Terminate thread
					terminate = 1;
					break;
				}
				else if (manage_id == MANAGE_NOTIFICATION_DISCONNECT)
				{
					disconnect = 1;
					break;
				}
				if (zwp_thread_mutex_lock(http_client_manager->lock) != ZWP_STATUS_OK)
				{
					//thread_result = ZWU_STATUS_ERROR;
					ZWP_LOG(ZWP_LOG_ERR, "failed to lock");
					continue;  // TODO: should break?
				}
				array_size = zwu_array_list_size(http_client_manager->array);
				if (manage_id < 0 || manage_id >= array_size)
				{
					ZWP_LOG(ZWP_LOG_ERR, "Invalid manage_id: %d, array_size: %d", manage_id, array_size);
				}
				else
				{
					entity = (zwu_http_client_manager_entity_t)zwu_array_list_get(http_client_manager->array, manage_id);
					if (zwu_http_client_manager_entity_process(entity, curl_multi_handle) != ZWU_STATUS_OK)
					{
						ZWP_LOG(ZWP_LOG_ERR, "http_client_manager_entity_process failed. manage_id: %d", manage_id);
					}
				}
				zwp_thread_mutex_unlock(http_client_manager->lock);
			}
		}

		if ((!terminate && !disconnect)  // no terminate or disconnect request from user application
			&& (timeout == 0  // libcurl: timeout == 0 means have to call multi_perform immediately
				|| ((extra_fds.revents & CURL_WAIT_POLLIN) && number_of_fds > 1)  // Both extra_fd and curl handle are active
				|| (!(extra_fds.revents & CURL_WAIT_POLLIN) && number_of_fds)))  // Only curl handle are active
		{
			if ((multi_result = curl_multi_perform(curl_multi_handle, &still_running)) != CURLM_OK)
			{
				thread_result = ZWU_STATUS_ERROR;
				ZWP_LOG(ZWP_LOG_ERR, "curl_multi_perform failed: %s", curl_multi_strerror(multi_result));
				break;
			}

			while ((curl_message = curl_multi_info_read(curl_multi_handle, &message_left)))
			{
				if (terminate || disconnect)
				{
					break;
				}
				if (curl_message->msg == CURLMSG_DONE)
				{
					if (zwp_thread_mutex_lock(http_client_manager->lock) != ZWP_STATUS_OK)
					{
						//thread_result = ZWU_STATUS_ERROR;
						ZWP_LOG(ZWP_LOG_ERR, "failed to lock");
						break;
					}
					curl_easy_handle = curl_message->easy_handle;
					if ((multi_result = curl_multi_remove_handle(curl_multi_handle, curl_easy_handle)) != CURLM_OK)
					{
						ZWP_LOG(ZWP_LOG_ERR, "curl_multi_remove_handle failed: %s", curl_multi_strerror(multi_result));
					}
					else
					{
						if ((curl_easy_getinfo(curl_easy_handle, CURLINFO_PRIVATE, &entity) == CURLE_OK)
							&& entity)
						{
							// entity_post_process() will call user callback to pass back the result and buffer.
							// TODO: if user callback do a lot of asynchronous HTTP request, and cause pipe buffer full? Then most likely
							// the callback will be block infinitely.
							if (zwu_http_client_manager_entity_post_process(entity, curl_message->data.result) != ZWU_STATUS_OK)
							{
								ZWP_LOG(ZWP_LOG_ERR, "http_client_manager_entity_post_process failed");
							}
							// Since user may do asynchronous HTTP request in callback, we should check the read pipe and process the new coming request.
							while (read(http_client_manager->pipefd[0], &manage_id, sizeof(manage_id)) == sizeof(manage_id))
							{
								// Both terminate and disconnect are not allowed, but anyway, we just double check here.
								if (manage_id == MANAGE_NOTIFICATION_TERMINATE)
								{
									terminate = 1;
									break;
								}
								else if (manage_id == MANAGE_NOTIFICATION_DISCONNECT)
								{
									disconnect = 1;
									break;
								}
								array_size = zwu_array_list_size(http_client_manager->array);
								if (manage_id < 0 || manage_id >= array_size)
								{
									ZWP_LOG(ZWP_LOG_ERR, "Invalid manage_id: %d, array_size: %d", manage_id, array_size);
								}
								else
								{
									entity = (zwu_http_client_manager_entity_t)zwu_array_list_get(http_client_manager->array, manage_id);
									if (zwu_http_client_manager_entity_process(entity, curl_multi_handle) != ZWU_STATUS_OK)
									{
										ZWP_LOG(ZWP_LOG_ERR, "http_client_manager_entity_process failed. manage_id: %d", manage_id);
									}
								}
							}
						}
					}
					zwp_thread_mutex_unlock(http_client_manager->lock);
				}
			}
		}

		if (terminate)
		{
			break;
		}
		else if (disconnect)
		{
			disconnect = 0;
			if (zwp_thread_mutex_lock(http_client_manager->lock) != ZWP_STATUS_OK)
				break;
			array_size = zwu_array_list_size(http_client_manager->array);
			for (i=0; i<array_size; i++)
			{
				entity=(zwu_http_client_manager_entity_t)zwu_array_list_get(http_client_manager->array, i);
				// Current easy handle is already added to multi-handle
				if (entity->curl_easy_handle
					&& (entity->status == MANAGE_ENTITY_STATUS_TO_REMOVE
						|| entity->status == MANAGE_ENTITY_STATUS_ADDED))
				{
					if ((multi_result = curl_multi_remove_handle(curl_multi_handle, entity->curl_easy_handle)) != CURLM_OK)
					{
						ZWP_LOG(ZWP_LOG_ERR, "curl_multi_remove_handle failed: %s", curl_multi_strerror(multi_result));
					}
				}
				// reset each entity
				zwu_http_client_manager_entity_reset(entity);
			}

			// By curl_multi_cleanup, all the connection are closed
			curl_multi_cleanup(http_client_manager->curl_multi_handle);
			if (zwu_http_client_manager_prepare_curl_multi_handle(http_client_manager) != ZWU_STATUS_OK)
			{
				// Something wrong when re-init multi-handle, no way to continue, exit process thread.
				http_client_manager->terminated = 1;
				terminate = 1;
			}
			curl_multi_handle = http_client_manager->curl_multi_handle;  // May become NULL, but doesn't matter, it will just exit loop
			http_client_manager->disconnected = 1;
			zwp_thread_cond_signal(http_client_manager->disconnect_wait);
			zwp_thread_mutex_unlock(http_client_manager->lock);

			if (terminate)
			{
				break;
			}
			else
			{
				continue;
			}
		}
	}

	zwp_thread_mutex_lock(http_client_manager->lock);
	http_client_manager->terminated = 1;
	// Release all the entities.
	for (i=0; i<zwu_array_list_size(http_client_manager->array); i++)
	{
		entity=(zwu_http_client_manager_entity_t)zwu_array_list_get(http_client_manager->array, i);
		if (entity->curl_easy_handle
			&& (entity->status == MANAGE_ENTITY_STATUS_TO_REMOVE
				||entity->status == MANAGE_ENTITY_STATUS_ADDED))
		{
			if ((multi_result = curl_multi_remove_handle(curl_multi_handle, entity->curl_easy_handle)) != CURLM_OK)
			{
				ZWP_LOG(ZWP_LOG_ERR, "curl_multi_remove_handle failed: %s", curl_multi_strerror(multi_result));
			}
		}
		zwu_http_client_manager_entity_destroy(entity);
	}
	zwp_thread_cond_broadcast(http_client_manager->disconnect_wait);  // in case any condition variable is waiting
	zwp_thread_mutex_unlock(http_client_manager->lock);

	zwp_thread_exit(thread, thread_result);
	return NULL;
}

/**
 * @brief Process request from user application that want to add HTTP request
 * @param[in]     http_client_manager       The http client manager object
 * @param[in]     request                   User request
 * @param[out]    manage_id_p               Manager ID
 * @retval        ZWU_STATUS_OK             Success
 * @retval        ZWU_STATUS_ERROR          Failure
 */
static zwu_status_t zwu_http_client_manager_process_request_action_add(zwu_http_client_manager_t http_client_manager, const zwu_http_client_request_t *request, int *manage_id_p)
{
	int reuse_entity;
	zwu_http_client_manager_entity_t entity;
	zwu_http_client_manager_entity_t allocate_entity;
	int i;
	int array_size;
	int manage_id;
	char *post_buffer;

	// URL Must not be NULL
	if (!request->options->url.set_flag || !request->options->url.option)
	{
		ZWP_LOG(ZWP_LOG_ERR, "URL is NULL");
		goto l_err_invalid_url;
	}

	reuse_entity = 0;
	entity = NULL;
	allocate_entity = NULL;
	array_size = zwu_array_list_size(http_client_manager->array);
	manage_id = -1;

	ZWP_LOG(ZWP_LOG_DEBUG, "Finding reusable entity!");

	// Find reusable entity via manage_id.
	// If retrieved entity holding the same http_client as the one within request, and current status is TO_ADD(request is pending) or ADDED(easy handle is added),
	// then we should return error, as user try to add new request before the current request finish(or cancel current request).
	// If retrieved entity have status TO_REMOVE, means current request is pending, but current easy handle is still within multi-handle. We should not disturb this
	// entity, let the process thread to deal with it. So it's not reusable entity. Note that in this case, if the current http_client is the same as the one within
	// request, we should not report error, allow user to add new request once the previous is cancelled, while process thread is not yet remove the easy handle.
	// If retrieved entity have status INIT or REMOVED, it's reusable entity.
	if (request->manage_id >= 0
		&& request->manage_id < array_size  // valid manage_id
		&& !array_size  // valid array
		&& (entity=(zwu_http_client_manager_entity_t)zwu_array_list_get(http_client_manager->array, request->manage_id)))  // retrieve entity
	{
		if (entity->http_client == request->http_client
			&& (entity->status == MANAGE_ENTITY_STATUS_TO_ADD || entity->status == MANAGE_ENTITY_STATUS_ADDED))
		{
			ZWP_LOG(ZWP_LOG_ERR, "Request already exists!");
			goto l_err_request_already_exist;
		}
		if (entity->status == MANAGE_ENTITY_STATUS_INIT
			|| entity->status == MANAGE_ENTITY_STATUS_REMOVED
			/*|| entity->status == MANAGE_ENTITY_STATUS_TO_REMOVE*/
			)
		{
			reuse_entity = 1;
			manage_id = request->manage_id;
		}
	}

	// Cannot reuse, or no previous entity, then have to go for one loop to search other allocated entity which can be reused.
	if (!reuse_entity && array_size)
	{
		for (i=0; i<array_size; i++)
		{
			if ((entity=(zwu_http_client_manager_entity_t)zwu_array_list_get(http_client_manager->array, i))
				&& (entity->status == MANAGE_ENTITY_STATUS_INIT
					|| entity->status == MANAGE_ENTITY_STATUS_REMOVED)
				)
			{
				break;
			}
		}
		// Cannot find proper entity
		if (i == array_size)
		{
			entity = NULL;
		}
		else
		{
			manage_id = i;
		}
	}

	// Allocate new entity
	if (entity == NULL)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Creating a new entity!");

		if (zwu_http_client_manager_entity_create(&allocate_entity) != ZWU_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to create entity!");
			goto l_err_http_client_manager_entity_create;
		}

		if (zwu_array_list_add(http_client_manager->array, (void *)allocate_entity) != ZWU_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to add entity to the manager!");
			goto l_err_array_list_add;
		}

		// Now the newly allocated entity is appended in the array.
		manage_id = zwu_array_list_size(http_client_manager->array) - 1;
		entity = allocate_entity;
	}

	//ZWP_LOG(ZWP_LOG_DEBUG, "\n>>>>> manage_id=%d <<<<<", manage_id);

	// Make it to null, so that new allocated entity(if have) won't be released during error handling, and can be re-used later.
	allocate_entity = NULL;

	ZWP_LOG(ZWP_LOG_DEBUG, "manage_id: %d", manage_id);
	post_buffer = NULL;
	if (request->request_method == ZWU_HTTP_CLIENT_REQUEST_HTTP_POST)
	{
		if (request->post_buffer_size)
		{
			if ((post_buffer = ZWP_REALLOC(entity->post_buffer, request->post_buffer_size)) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				goto l_err_realloc_post_buffer;
			}
			entity->post_buffer = post_buffer;
			memcpy(entity->post_buffer, request->post_buffer, request->post_buffer_size);
			entity->post_buffer_size = request->post_buffer_size;
		}
		else
		{
			ZWP_FREE(entity->post_buffer);
			entity->post_buffer = NULL;
			entity->post_buffer_size = 0;
		}
	}

	if(zwu_http_client_option_copy(request->options, &entity->options) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to copy handle options!");
		goto l_err_http_client_option_copy;
	}

	// Configure internal status
	entity->status = MANAGE_ENTITY_STATUS_TO_ADD;
	entity->http_client = request->http_client;
	entity->request_method = request->request_method;
	entity->response_callback = request->response_callback;
	entity->http2_ping_response_callback = request->http2_ping_response_callback;
	entity->response_context = request->response_context;

	*manage_id_p = manage_id;

	return ZWU_STATUS_OK;

	zwu_http_client_option_destroy(&entity->options);
	zwu_http_client_option_init_with_default_value(&entity->options);  // back to initial value
l_err_http_client_option_copy:
	// ZWP_FREE(post_buffer);  // Don't need to free buffer here, as we use realloc() to manage post buffer.
l_err_realloc_post_buffer:
l_err_array_list_add:
	if (allocate_entity)
	{
		zwu_http_client_manager_entity_destroy(allocate_entity);
	}
l_err_http_client_manager_entity_create:
l_err_request_already_exist:
l_err_invalid_url:
	return ZWU_STATUS_ERROR;
}

/**
 * @brief Process request from user application that want to remove HTTP request
 * @param[in]     http_client_manager       The http client manager object
 * @param[in]     request                   User request
 * @param[out]    manage_id_p               Manager ID
 * @retval        ZWU_STATUS_OK             Success
 * @retval        ZWU_STATUS_ERROR          Failure
 */
static zwu_status_t zwu_http_client_manager_process_request_action_remove(zwu_http_client_manager_t http_client_manager, const zwu_http_client_request_t *request, int *manage_id_p)
{
	int array_size;
	zwu_http_client_manager_entity_t entity;
	int manage_id;

	array_size = zwu_array_list_size(http_client_manager->array);
	entity = NULL;
	manage_id = -1;

	// Validate manage_id
	if (request->manage_id < 0 || request->manage_id >= array_size)
	{
		goto l_err_invalid_manage_id;
	}

	// Validate http_client
	// http_client is valid only when current status is: TO_ADD, ADDED
	entity = (zwu_http_client_manager_entity_t)zwu_array_list_get(http_client_manager->array, request->manage_id);
	// Invalid status: most likely it's due to current request is finished(REMOVED), or duplicate action_move is called(TO_REMOVE)
	if (entity->status != MANAGE_ENTITY_STATUS_TO_ADD && entity->status != MANAGE_ENTITY_STATUS_ADDED)
	{
		goto l_err_invalid_status;
	}
	if (entity->http_client != request->http_client)  // http_client corrupted?
	{
		goto l_err_http_client_mismatched;
	}

	// For status TO_ADD or ADDED, proper status, go ahead.
	if (entity->status == MANAGE_ENTITY_STATUS_TO_ADD)
	{
		// entity is not yet processed by the loop-process thread.
		entity->status = MANAGE_ENTITY_STATUS_REMOVED;
		zwu_http_client_option_destroy(&entity->options);
		zwu_http_client_option_init_with_default_value(&entity->options);  // back to initial value
	}
	else if (entity->status == MANAGE_ENTITY_STATUS_ADDED)
	{
		// Easy handle is already added to multi-handle, so we should let the loop-process thread to remove it
		entity->status = MANAGE_ENTITY_STATUS_TO_REMOVE;
		manage_id = request->manage_id;
	}

	*manage_id_p = manage_id;
	return ZWU_STATUS_OK;

l_err_http_client_mismatched:
l_err_invalid_status:
l_err_invalid_manage_id:
	return ZWU_STATUS_ERROR;
}

/**
 * @brief Process request from user application
 * @param[in]     http_client_manager       The http client manager object
 * @param[in]     request                   User request
 * @param[out]    manage_id_p               Manager ID
 * @retval        ZWU_STATUS_OK             Success
 * @retval        ZWU_STATUS_ERROR          Failure
 */
zwu_status_t zwu_http_client_manager_process_request(zwu_http_client_manager_t http_client_manager, const zwu_http_client_request_t *request, int *manage_id_p)
{
	int manage_id;

	manage_id = -1;

	if (zwp_thread_mutex_lock(http_client_manager->lock) != ZWP_STATUS_OK)
	{
		goto l_err_thread_mutex_lock;
	}

	if (http_client_manager->terminated)
	{
		ZWP_LOG(ZWP_LOG_ERR, "http_client_manager terminated!");
		goto l_err_process_thread_terminated;
	}

	switch(request->action)
	{
		case MANAGE_ACTION_ADD:
			if (zwu_http_client_manager_process_request_action_add(http_client_manager, request, &manage_id) != ZWU_STATUS_OK)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Add request failed!");
				goto l_err_http_client_manager_process_request_action_add;
			}
			break;
		case MANAGE_ACTION_REMOVE:
			if (zwu_http_client_manager_process_request_action_remove(http_client_manager, request, &manage_id) != ZWU_STATUS_OK)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Remove request failed!");
				goto l_err_http_client_manager_process_request_action_remove;
			}
			break;
		default:
			goto l_err_invalid_action;
			break;
	}

	if (request->action == MANAGE_ACTION_ADD && manage_id_p)
	{
		*manage_id_p = manage_id;
	}

	zwp_thread_mutex_unlock(http_client_manager->lock);

	// Inform process thread
	// Note that pipe writes are atomic if buffer size is less than PIPE_BUF(4096 in linux) bytes.
	if (manage_id != -1)
	{
		if (write(http_client_manager->pipefd[1], &manage_id, sizeof(manage_id)) < sizeof(manage_id))
		{
			ZWP_LOG(ZWP_LOG_ERR, "pipe write failed(%d): %s", errno, strerror(errno));
		}
	}

	return ZWU_STATUS_OK;

l_err_invalid_action:
l_err_http_client_manager_process_request_action_remove:
l_err_http_client_manager_process_request_action_add:
l_err_process_thread_terminated:
	zwp_thread_mutex_unlock(http_client_manager->lock);
l_err_thread_mutex_lock:
	return ZWU_STATUS_ERROR;
}

zwu_status_t zwu_http_client_manager_start(zwu_http_client_manager_t http_client_manager)
{
	if (zwp_thread_create(&http_client_manager->thread_id, zwu_http_client_manager_process,
		(void*)http_client_manager, http_client_manager->memory_pool, ZWP_THREAD_CREATE_JOINABLE) != ZWP_STATUS_OK)
	{
		goto l_err_thread_create;
	}

	return ZWU_STATUS_OK;
l_err_thread_create:
	return ZWU_STATUS_ERROR;
}

zwu_status_t zwu_http_client_manager_stop(zwu_http_client_manager_t http_client_manager)
{
	int ternimate;
	int already_terminated;
	zwu_status_t thread_result;

	ternimate = MANAGE_NOTIFICATION_TERMINATE;
	already_terminated = 0;

	if (zwp_thread_mutex_lock(http_client_manager->lock) == ZWP_STATUS_OK)
	{
		already_terminated = http_client_manager->terminated;
		zwp_thread_mutex_unlock(http_client_manager->lock);
	}

	if (!already_terminated)
	{
		if (write(http_client_manager->pipefd[1], &ternimate, sizeof(ternimate)) != sizeof(ternimate))
		{
			goto l_err_pipe_write;
		}
	}

	thread_result = ZWU_STATUS_OK;
	if (http_client_manager->thread_id)
	{
		zwp_thread_join(http_client_manager->thread_id, &thread_result);
	}
	return thread_result;

l_err_pipe_write:
	return ZWU_STATUS_ERROR;
}

zwu_status_t zwu_http_client_manager_disconnect(zwu_http_client_manager_t http_client_manager)
{
	int disconnect;

	disconnect = MANAGE_NOTIFICATION_DISCONNECT;


	if (write(http_client_manager->pipefd[1], &disconnect, sizeof(disconnect)) != sizeof(disconnect))
	{
		goto l_err_pipe_write;
	}

	if (zwp_thread_mutex_lock(http_client_manager->lock) != ZWP_STATUS_OK)
	{
		goto l_err_thread_mutex_lock;
	}
	// Wait if disconnected and terminated are not set yet.
	if (!http_client_manager->disconnected && !http_client_manager->terminated)
	{
		zwp_thread_cond_wait(http_client_manager->disconnect_wait, http_client_manager->lock);
	}
	http_client_manager->disconnected = 0;
	zwp_thread_mutex_unlock(http_client_manager->lock);

	return ZWU_STATUS_OK;

	zwp_thread_mutex_unlock(http_client_manager->lock);
l_err_thread_mutex_lock:
l_err_pipe_write:
	return ZWU_STATUS_ERROR;
}

zwu_status_t zwu_http_client_manager_create(zwu_http_client_manager_t *http_client_manager_p)
{
	zwu_http_client_manager_t http_client_manager;
	int flags;

	if ((http_client_manager = (zwu_http_client_manager_t)ZWP_MALLOC(sizeof(*http_client_manager))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_http_client_manager_alloc;
	}

	if (zwp_pool_create(&http_client_manager->memory_pool, NULL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create memory pool for directory service");
		goto l_err_memory_pool_create;
	}

	if (pipe(http_client_manager->pipefd))
	{
		goto l_err_pipe;
	}

	// Make read pipe as non-block, so that we can monitor it later.
	// Let write pipe be block, so that we can atomically write to it.
	flags = fcntl(http_client_manager->pipefd[0], F_GETFL);
	if (flags == -1)
	{
		goto l_err_fcntl_getfl;
	}
	if (fcntl(http_client_manager->pipefd[0], F_SETFL, flags | O_NONBLOCK) == -1)
	{
		goto l_err_fcntl_setfl;
	}

	// Recursive lock, so that user can call process_request even in callback
	if (zwp_thread_mutex_create(&http_client_manager->lock, http_client_manager->memory_pool, ZWP_THREAD_MUTEX_CREATE_RECURSIVE) != ZWP_STATUS_OK)
	{
		goto l_err_thread_mutex_create;
	}

	if (zwu_array_list_create(ZWU_HTTP_CLIENT_MANAGER_DEFAULT_ARRAY_SIZE, &http_client_manager->array) != ZWU_STATUS_OK)
	{
		goto l_err_array_list_create;
	}

	http_client_manager->terminated = 0;

	if (zwp_thread_cond_create(&http_client_manager->disconnect_wait, http_client_manager->memory_pool) != ZWP_STATUS_OK)
	{
		goto l_err_thread_cond_create_disconnect_wait;
	}
	http_client_manager->disconnected = 0;

	http_client_manager->thread_id = NULL;

	if (zwu_http_client_manager_prepare_curl_multi_handle(http_client_manager) != ZWU_STATUS_OK)
	{
		goto l_err_http_client_manager_prepare_curl_multi_handle;
	}

	*http_client_manager_p = http_client_manager;
	return ZWU_STATUS_OK;

	if (http_client_manager->curl_multi_handle)
	{
		curl_multi_cleanup(http_client_manager->curl_multi_handle);
	}
l_err_http_client_manager_prepare_curl_multi_handle:
	zwp_thread_cond_destroy(http_client_manager->disconnect_wait);
l_err_thread_cond_create_disconnect_wait:
	zwu_array_list_destroy(http_client_manager->array);
l_err_array_list_create:
	zwp_thread_mutex_destroy(http_client_manager->lock);
l_err_thread_mutex_create:
l_err_fcntl_setfl:
l_err_fcntl_getfl:
	close(http_client_manager->pipefd[0]);
	close(http_client_manager->pipefd[1]);
l_err_pipe:
	zwp_pool_destroy(http_client_manager->memory_pool);
l_err_memory_pool_create:
	ZWP_FREE(http_client_manager);
l_err_http_client_manager_alloc:
	return ZWU_STATUS_ERROR;
}

void zwu_http_client_manager_destroy(zwu_http_client_manager_t http_client_manager)
{
	if (http_client_manager->curl_multi_handle)
	{
		// curl_multi_handle may become NULL during disconnect and re-connect stage.
		curl_multi_cleanup(http_client_manager->curl_multi_handle);
	}
	zwp_thread_cond_destroy(http_client_manager->disconnect_wait);
	zwu_array_list_destroy(http_client_manager->array);
	zwp_thread_mutex_destroy(http_client_manager->lock);
	close(http_client_manager->pipefd[0]);
	close(http_client_manager->pipefd[1]);
	zwp_pool_destroy(http_client_manager->memory_pool);
	ZWP_FREE(http_client_manager);
}
