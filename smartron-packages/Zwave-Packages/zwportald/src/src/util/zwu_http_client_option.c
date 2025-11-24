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
 * @file    zwu_http_client_option.c
 *
 * @brief   HTTP Client Option
 * @details Helper option for zwu_http_client with curl.
 *
 * @author  Zelin
 *
 * @version 1.0 - 2017-01-06
 * - Initial version
 */

#include "curl/curl.h"
#include "zwu_http_client_option.h"
#include "zwp_memory.h"
#include "zwp_log.h"
#include "zwp_error.h"

/**
 * @defgroup zwu_http_client_option HTTP Client Option
 * @{
 * @ingroup util
 */

#define ZWU_CURL_DEFAULT_OPTION_CONNECT_TIMEOUT (300L)    /**< 300 seconds for connection timeout */
#define ZWU_CURL_DEFAULT_OPTION_TIMEOUT (0L)              /**< 0 means never times out during transfer.  */
//#define ZWU_CURL_DEFAULT_OPTION_SSL_CIPHER_LIST "ALL"     //"ECDHE-ECDSA-AES128-GCM-SHA256,ECDHE-RSA-AES128-GCM-SHA256"
														  /**< TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 and TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256
														   Refer to https://www.openssl.org/docs/man1.0.2/apps/ciphers.html */

static struct {
	zwu_http_client_use_ssl_enum_t zwu_value;
	long curl_value;
} g_zwu_http_client_use_ssl_map[] =
	{
		{ZWU_HTTP_CLIENT_USE_SSL_NONE, CURLUSESSL_NONE},
		{ZWU_HTTP_CLIENT_USE_SSL_TRY, CURLUSESSL_TRY},
		{ZWU_HTTP_CLIENT_USE_SSL_CONTROL, CURLUSESSL_CONTROL},
		{ZWU_HTTP_CLIENT_USE_SSL_ALL, CURLUSESSL_ALL}
	};

static struct {
	zwu_http_client_ssl_version_enum_t zwu_value;
	long curl_value;
} g_zwu_http_client_ssl_version_map[] =
	{
		{ZWU_HTTP_CLIENT_SSL_VERSION_DEFAULT, CURL_SSLVERSION_DEFAULT},
		{ZWU_HTTP_CLIENT_SSL_VERSION_TLSV1, CURL_SSLVERSION_TLSv1},
		{ZWU_HTTP_CLIENT_SSL_VERSION_SSLV2, CURL_SSLVERSION_SSLv2},
		{ZWU_HTTP_CLIENT_SSL_VERSION_SSLV3, CURL_SSLVERSION_SSLv3},
		{ZWU_HTTP_CLIENT_SSL_VERSION_TLSV1_0, CURL_SSLVERSION_TLSv1_0},
		{ZWU_HTTP_CLIENT_SSL_VERSION_TLSV1_1, CURL_SSLVERSION_TLSv1_1},
		{ZWU_HTTP_CLIENT_SSL_VERSION_TLSV1_2, CURL_SSLVERSION_TLSv1_2},
		//{ZWU_CURL_SSL_VERSION_TLSV1_3, CURL_SSLVERSION_TLSv1_3},  // Added in 7.52.0
	};

static struct {
	zwu_http_client_ssl_enable_alpn_enum_t zwu_value;
	long curl_value;
} g_zwu_http_client_ssl_enable_alpn_map[] = 
	{
		{ZWU_HTTP_CLIENT_SSL_ENABLE_ALPN_DISABLE, 0L},
		{ZWU_HTTP_CLIENT_SSL_ENABLE_ALPN_ENABLE, 1L},
	};

static struct {
	zwu_http_client_ssl_verify_peer_enum_t zwu_value;
	long curl_value;
} g_zwu_http_client_ssl_verify_peer_map[] = 
	{
		{ZWU_HTTP_CLIENT_SSL_VERIFY_PEER_DISABLE, 0L},
		{ZWU_HTTP_CLIENT_SSL_VERIFY_PEER_ENABLE, 1L},
	};

static struct {
	zwu_http_client_ssl_verify_host_enum_t zwu_value;
	long curl_value;
} g_zwu_http_client_ssl_verify_host_map[] = 
	{
		{ZWU_HTTP_CLIENT_SSL_VERIFY_HOST_DISABLE, 0L},
		{ZWU_HTTP_CLIENT_SSL_VERIFY_HOST_ENABLE, 2L},  // 1L is obsolate
	};

static struct {
	zwu_http_client_http_version_enum_t zwu_value;
	long curl_value;
} g_zwu_http_client_http_version_map[] =
	{
		{ZWU_HTTP_CLIENT_HTTP_VERSION_NONE, CURL_HTTP_VERSION_NONE},
		{ZWU_HTTP_CLIENT_HTTP_VERSION_1_0, CURL_HTTP_VERSION_1_0},
		{ZWU_HTTP_CLIENT_HTTP_VERSION_1_1, CURL_HTTP_VERSION_1_1},
		{ZWU_HTTP_CLIENT_HTTP_VERSION_2_0, CURL_HTTP_VERSION_2_0},
		{ZWU_HTTP_CLIENT_HTTP_VERSION_2_TLS, CURL_HTTP_VERSION_2TLS},
		{ZWU_HTTP_CLIENT_HTTP_VERSION_2_PRIOR_KNOWLEDGE, CURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE},
	};

static struct {
	zwu_http_client_verbose_enum_t zwu_value;
	long curl_value;
} g_zwu_http_client_verbose_map[] =
	{
		{ZWU_HTTP_CLIENT_VERBOSE_OFF, 0L},
		{ZWU_HTTP_CLIENT_VERBOSE_ON, 1L},
	};

static struct {
	zwu_http_client_pipe_wait_enum_t zwu_value;
	long curl_value;
} g_zwu_http_client_pipe_wait_map[] =
	{
		{ZWU_HTTP_CLIENT_PIPE_WAIT_OFF, 0L},
		{ZWU_HTTP_CLIENT_PIPE_WAIT_ON, 1L},
	};

static struct {
	zwu_http_client_fresh_connect_enum_t zwu_value;
	long curl_value;
} g_zwu_http_client_fresh_connect_map[] =
	{
		{ZWU_HTTP_CLIENT_FRESH_CONNECT_OFF, 0L},
		{ZWU_HTTP_CLIENT_FRESH_CONNECT_ON, 1L},
	};

static struct {
	CURLcode curl_code;
	zwu_status_t zwu_status;
} g_zwu_http_client_curl_easy_code_map[] =
	{
		{CURLE_OK, ZWU_STATUS_OK},                        /**< All fine. Proceed as usual. */
		{CURLE_OUT_OF_MEMORY, ZWU_STATUS_ENOMEM},         /**< A memory allocation request failed. This is serious badness and things are severely
															screwed up if this ever occurs.*/

		/* Operation timeout may due to connect timeout or read/write timeout, consider as server unreachable */
		{CURLE_OPERATION_TIMEDOUT, ZWU_STATUS_ETIME},     /**< Operation timeout. The specified time-out period was reached according to the conditions. */

		/* dns resolve error also consider as server unreachable */
		{CURLE_COULDNT_RESOLVE_PROXY, ZWU_STATUS_ETIME},  /**< Couldn't resolve proxy. The given proxy host could not be resolved. */
		{CURLE_COULDNT_RESOLVE_HOST, ZWU_STATUS_ETIME},   /**< Couldn't resolve host. The given remote host was not resolved. */

		/* Cound not connect is most likely due to server unreachable */
		{CURLE_COULDNT_CONNECT, ZWU_STATUS_ETIME},        /**< Failed to connect() to host or proxy. */

		{CURLE_REMOTE_FILE_EXISTS, ZWU_STATUS_ENOENT},    /**< The resource referenced in the URL does not exist. */
		{CURLE_REMOTE_ACCESS_DENIED, ZWU_STATUS_EACCES},  /**< We were denied access to the resource given in the URL. For FTP, this occurs while
															trying to change to the remote directory. */
		{CURLE_BAD_FUNCTION_ARGUMENT, ZWU_STATUS_EINVAL}, /**< Internal error. A function was called with a bad parameter. */
	};

// To get curl option value from http_client_enum_t
/**
 * @brief Convert http client enum to curl option value
 * @param[in]     value                     Http client enum value
 * @param[out]    curl_option_p             Curl option value
 * @retval        ZWU_STATUS_OK             Success
 * @retval        ZWU_STATUS_ERROR          Failure
 */
#define ZWU_HTTP_CLIENT_DEFINE_HTTP_CLIENT_ENUM_2_CURL_OPTION_VALUE(option_name) \
	static zwu_status_t zwu_http_client_##option_name##_enum_to_curl_option(zwu_http_client_##option_name##_enum_t value, long *curl_option_p) \
	{ \
		int i;\
		int table_size = sizeof(g_zwu_http_client_##option_name##_map)/sizeof(g_zwu_http_client_##option_name##_map[0]); \
		for (i=0; i<table_size && g_zwu_http_client_##option_name##_map[i].zwu_value != value; i++) \
			; \
		if (i >= table_size) \
		{ \
			goto l_not_found; \
		} \
		if (curl_option_p) \
		{ \
			*curl_option_p = g_zwu_http_client_##option_name##_map[i].curl_value; \
		} \
		return ZWU_STATUS_OK; \
	l_not_found: \
		return ZWU_STATUS_ERROR; \
	}

// static zwu_status_t zwu_http_client_use_ssl_enum_to_curl_option(zwu_http_client_use_ssl_enum_t value, long *curl_option_p)
ZWU_HTTP_CLIENT_DEFINE_HTTP_CLIENT_ENUM_2_CURL_OPTION_VALUE(use_ssl)

// static zwu_status_t zwu_http_client_ssl_version_enum_to_curl_option(zwu_http_client_ssl_version_enum_t value, long *curl_option_p)
ZWU_HTTP_CLIENT_DEFINE_HTTP_CLIENT_ENUM_2_CURL_OPTION_VALUE(ssl_version)

// static zwu_status_t zwu_http_client_ssl_enable_alpn_enum_to_curl_option(zwu_http_client_ssl_enable_alpn_enum_t value, long *curl_option_p)
ZWU_HTTP_CLIENT_DEFINE_HTTP_CLIENT_ENUM_2_CURL_OPTION_VALUE(ssl_enable_alpn)

// static zwu_status_t zwu_http_client_ssl_verify_peer_enum_to_curl_option(zwu_http_client_ssl_verify_peer_enum_t value, long *curl_option_p)
ZWU_HTTP_CLIENT_DEFINE_HTTP_CLIENT_ENUM_2_CURL_OPTION_VALUE(ssl_verify_peer)

// static zwu_status_t zwu_http_client_ssl_verify_host_enum_to_curl_option(zwu_http_client_ssl_verify_host_enum_t value, long *curl_option_p)
ZWU_HTTP_CLIENT_DEFINE_HTTP_CLIENT_ENUM_2_CURL_OPTION_VALUE(ssl_verify_host)

// static zwu_status_t zwu_http_client_http_version_enum_to_curl_option(zwu_http_client_http_version_enum_t value, long *curl_option_p)
ZWU_HTTP_CLIENT_DEFINE_HTTP_CLIENT_ENUM_2_CURL_OPTION_VALUE(http_version)

// static zwu_status_t zwu_http_client_verbose_enum_to_curl_option(zwu_http_client_verbose_enum_t value, long *curl_option_p)
ZWU_HTTP_CLIENT_DEFINE_HTTP_CLIENT_ENUM_2_CURL_OPTION_VALUE(verbose)

// static zwu_status_t zwu_http_client_pipe_wait_enum_to_curl_option(zwu_http_client_pipe_wait_enum_t value, long *curl_option_p)
ZWU_HTTP_CLIENT_DEFINE_HTTP_CLIENT_ENUM_2_CURL_OPTION_VALUE(pipe_wait)

// static zwu_status_t zwu_http_client_fresh_connect_enum_to_curl_option(zwu_http_client_fresh_connect_enum_t value, long *curl_option_p)
ZWU_HTTP_CLIENT_DEFINE_HTTP_CLIENT_ENUM_2_CURL_OPTION_VALUE(fresh_connect)

zwu_status_t zwu_http_client_easy_code_to_zwu_status(CURLcode result)
{
	int i;
	int table_size;

	table_size = sizeof(g_zwu_http_client_curl_easy_code_map) / sizeof(g_zwu_http_client_curl_easy_code_map[0]);
	for (i=0; i<table_size && g_zwu_http_client_curl_easy_code_map[i].curl_code != result; i++)
		;
	return i<table_size ? g_zwu_http_client_curl_easy_code_map[i].zwu_status : ZWU_STATUS_ERROR;
}

zwu_http_client_http_version_enum_t zwu_http_client_option_curl_value_to_zwu_value_http_version(long curl_http_version)
{
	int i, table_size;
	zwu_http_client_http_version_enum_t http_version;

	table_size = sizeof(g_zwu_http_client_http_version_map)/sizeof(g_zwu_http_client_http_version_map[0]);
	for (i=0; i<table_size && curl_http_version!=g_zwu_http_client_http_version_map[i].curl_value; i++)
		;
	http_version = i<table_size ? g_zwu_http_client_http_version_map[i].zwu_value : ZWU_HTTP_CLIENT_HTTP_VERSION_NONE;

	return http_version;
}

/**
 * @brief Dump log string to ZWP_LOG()
 * @param[in]   description       Text description
 * @param[in]   data              Data, NOT be zero terminated
 * @param[in]   size              Data size
 * @return      Nothing
 */
static void zwu_http_client_option_dump_string(const char *description, unsigned char *data, size_t size)
{
	unsigned char *log_buffer;
	size_t buffer_length;
	int i,j;

	buffer_length = strlen(description) + size;
	// Skip tailing \r\n or \n
	if (data[size - 2] == '\r' && data[size - 1] == '\n')
	{
		buffer_length -= 2;
	}
	else if (data[size - 1] == '\n')
	{
		buffer_length--;
	}

	// Extra byte at the end to store zero terminator
	if ((log_buffer=(unsigned char *)ZWP_CALLOC(1, buffer_length+1)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_log_buffer_alloc;
	}
	log_buffer[buffer_length] = '\0';

	memcpy(&log_buffer[0], description, strlen(description));
	for (i=strlen(description),j=0; i<buffer_length; i++,j++)
	{
		// keep the printable character or new line(in the middle) to log buffer
		if ((data[j] >= 0x20 && data[j] <= 0x80) || (data[j] == '\r') || (data[j] == '\n'))
		{
			log_buffer[i] = data[j];
		}
		else
		{
			log_buffer[i] = '.';
		}
	}
	ZWP_LOG(ZWP_LOG_DEBUG, "%s", log_buffer);

	ZWP_FREE(log_buffer);
	return;

	ZWP_FREE(log_buffer);
l_err_log_buffer_alloc:
	return;
}

int zwu_http_client_option_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userp)
{
	const char *text;
	int dump_string;
	(void)handle; /* prevent compiler warning */
	(void)userp;

	dump_string = 0;
	text = NULL;
	switch (type)
	{
		case CURLINFO_TEXT:
			text = "Info==: ";
			dump_string = 1;
			break;
		case CURLINFO_HEADER_OUT:
			text = "Send Header=>: ";
			dump_string = 1;
			break;
		case CURLINFO_DATA_OUT:
			text = "Send Data=>: ";
			dump_string = 1;
			break;
		case CURLINFO_SSL_DATA_OUT:
			text = "Send SSL data=>: ";
			break;
		case CURLINFO_HEADER_IN:
			text = "Recv Header<=: ";
			dump_string = 1;
			break;
		case CURLINFO_DATA_IN:
			text = "Recv Data<=: ";
			// Incoming data is too much
			//dump_string = 1;
			break;
		case CURLINFO_SSL_DATA_IN:
			text = "Recv SSL data<=: ";
			break;
		default: /* in case a new one is introduced to shock us */
			return 0;
	}

	if (dump_string)
	{
		zwu_http_client_option_dump_string(text, (unsigned char *)data, size);
	}
	return 0;
}

CURLcode zwu_http_client_option_configure_fixed_easy_options(const zwu_http_client_option_t *options, CURL *curl_easy_handle, const void *context)
{
	CURLcode result;

	// Prepare error buffer
	result = curl_easy_setopt(curl_easy_handle, CURLOPT_ERRORBUFFER, options->error_buffer);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_ERRORBUFFER failed(%d): %s", result, curl_easy_strerror(result));
		ZWP_LOG(ZWP_LOG_ERR, "Should not rely on error description from error_buffer later!");
	}

#ifdef USE_NGHTTP2
	result = curl_easy_setopt(curl_easy_handle, CURLOPT_HTTP2_PINGFUNCTION, options->http2_ping_callback);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_HTTP2_PINGFUNCTION failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_http2_ping_function;
	}

	result = curl_easy_setopt(curl_easy_handle, CURLOPT_HTTP2_PINGDATA, context);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_HTTP2_PINGDATA failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_http2_ping_data;
	}
#endif

	result = curl_easy_setopt(curl_easy_handle, CURLOPT_WRITEFUNCTION, options->write_callback);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_WRITEFUNCTION failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_write_function;
	}

	result = curl_easy_setopt(curl_easy_handle, CURLOPT_WRITEDATA, context);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_WRITEDATA failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_write_data;
	}
#if 0
	result = curl_easy_setopt(curl_easy_handle, CURLOPT_READFUNCTION, read_callback);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_READFUNCTION failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_read_function;
	}

	result = curl_easy_setopt(curl_easy_handle, CURLOPT_READDATA, context);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_READDATA failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_read_data;
	}
#endif
	result = curl_easy_setopt(curl_easy_handle, CURLOPT_HEADERFUNCTION, options->header_callback);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_HEADERFUNCTION failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_header_callback;
	}

	result = curl_easy_setopt(curl_easy_handle, CURLOPT_HEADERDATA, context);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_HEADERDATA failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_header_data;
	}

	return result;
l_err_curl_easy_setopt_header_data:
l_err_curl_easy_setopt_header_callback:
l_err_curl_easy_setopt_write_data:
l_err_curl_easy_setopt_write_function:
#ifdef USE_NGHTTP2
l_err_curl_easy_setopt_http2_ping_data:
l_err_curl_easy_setopt_http2_ping_function:
#endif
	return result;
}

CURLcode zwu_http_client_option_configure_easy_options(const zwu_http_client_option_t *options, CURL *curl_easy_handle)
{
	CURLcode result;
	long curl_option_value;

	if (options->use_ssl != ZWU_HTTP_CLIENT_USE_SSL_INVALID
		&& zwu_http_client_use_ssl_enum_to_curl_option(options->use_ssl, &curl_option_value) == ZWU_STATUS_OK)
	{
		result = curl_easy_setopt(curl_easy_handle, CURLOPT_USE_SSL, curl_option_value);
		if (result != CURLE_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_USE_SSL failed: %s",
				strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
			goto l_err_curl_easy_setopt_use_ssl;
		}
	}

	if (options->ssl_version != ZWU_HTTP_CLIENT_SSL_VERSION_INVALID
		&& zwu_http_client_ssl_version_enum_to_curl_option(options->ssl_version, &curl_option_value) == ZWU_STATUS_OK)
	{
		result = curl_easy_setopt(curl_easy_handle, CURLOPT_SSLVERSION, curl_option_value);
		if (result != CURLE_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_SSLVERSION failed: %s",
				strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
			goto l_err_curl_easy_setopt_ssL_version;
		}
	}

	if (options->ssl_enable_alpn != ZWU_HTTP_CLIENT_SSL_ENABLE_ALPN_INVALID
		&& zwu_http_client_ssl_enable_alpn_enum_to_curl_option(options->ssl_enable_alpn, &curl_option_value) == ZWU_STATUS_OK)
	{
		result = curl_easy_setopt(curl_easy_handle, CURLOPT_SSL_ENABLE_ALPN, curl_option_value);
		if (result != CURLE_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_SSL_ENABLE_ALPN failed: %s",
				strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
			goto l_err_curl_easy_setopt_ssl_enable_alpn;
		}
	}

	if (options->ssl_verify_peer != ZWU_HTTP_CLIENT_SSL_VERIFY_PEER_INVALID
		&& zwu_http_client_ssl_verify_peer_enum_to_curl_option(options->ssl_verify_peer, &curl_option_value) == ZWU_STATUS_OK)
	{
		result = curl_easy_setopt(curl_easy_handle, CURLOPT_SSL_VERIFYPEER, curl_option_value);
		if (result != CURLE_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_SSL_VERIFYPEER failed: %s",
				strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
			goto l_err_curl_easy_setopt_ssl_verify_peer;
		}
	}

	if (options->ssl_verify_host != ZWU_HTTP_CLIENT_SSL_VERIFY_HOST_INVALID
		&& zwu_http_client_ssl_verify_host_enum_to_curl_option(options->ssl_verify_host, &curl_option_value) == ZWU_STATUS_OK)
	{
		result = curl_easy_setopt(curl_easy_handle, CURLOPT_SSL_VERIFYHOST, curl_option_value);
		if (result != CURLE_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_SSL_VERIFYHOST failed: %s",
				strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
			goto l_err_curl_easy_setopt_ssl_verify_host;
		}
	}

	if (options->http_version != ZWU_HTTP_CLIENT_HTTP_VERSION_INVALID
		&& zwu_http_client_http_version_enum_to_curl_option(options->http_version, &curl_option_value) == ZWU_STATUS_OK)
	{
		result = curl_easy_setopt(curl_easy_handle, CURLOPT_HTTP_VERSION, curl_option_value);
		if (result != CURLE_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_HTTP_VERSION failed: %s",
				strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
			goto l_err_curl_easy_setopt_http_version;
		}
	}

	if (options->verbose != ZWU_HTTP_CLIENT_VERBOSE_INVALID
		&& zwu_http_client_verbose_enum_to_curl_option(options->verbose, &curl_option_value) == ZWU_STATUS_OK)
	{
		result = curl_easy_setopt(curl_easy_handle, CURLOPT_DEBUGFUNCTION, curl_option_value?options->debug_callback:NULL);
		if (result != CURLE_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_DEBUGFUNCTION failed: %s",
				strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
			goto l_err_curl_easy_setopt_debug_function;
		}

		/* the DEBUGFUNCTION has no effect until we enable VERBOSE */
		result = curl_easy_setopt(curl_easy_handle, CURLOPT_VERBOSE, curl_option_value);
		if (result != CURLE_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_VERBOSE failed: %s",
				strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
			goto l_err_curl_easy_setopt_verbose;
		}
	}

	if (options->pipe_wait != ZWU_HTTP_CLIENT_PIPE_WAIT_INVALID
		&& zwu_http_client_pipe_wait_enum_to_curl_option(options->pipe_wait, &curl_option_value) == ZWU_STATUS_OK)
	{
		result = curl_easy_setopt(curl_easy_handle, CURLOPT_PIPEWAIT, curl_option_value);
		if (result != CURLE_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_PIPEWAIT failed: %s",
				strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
			goto l_err_curl_easy_setopt_pipe_wait;
		}
	}

	if (options->fresh_connect != ZWU_HTTP_CLIENT_FRESH_CONNECT_INVALID
		&& zwu_http_client_fresh_connect_enum_to_curl_option(options->fresh_connect, &curl_option_value) == ZWU_STATUS_OK)
	{
		result = curl_easy_setopt(curl_easy_handle, CURLOPT_FRESH_CONNECT, curl_option_value);
		if (result != CURLE_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_FRESH_CONNECT failed: %s",
				strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
			goto l_err_curl_easy_setopt_fresh_connect;
		}
	}

	result = curl_easy_setopt(curl_easy_handle, CURLOPT_CONNECTTIMEOUT, options->connect_timeout);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_CONNECTTIMEOUT failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_connect_timeout;
	}

	result = curl_easy_setopt(curl_easy_handle, CURLOPT_TIMEOUT, options->timeout);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_TIMEOUT failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_timeout;
	}

#ifdef USE_NGHTTP2
	result = curl_easy_setopt(curl_easy_handle, CURLOPT_HTTP2_PINGOPAQUE_DATA, options->http2_ping_opaque_data);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_HTTP2_PINGOPAQUE_DATA failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_http2_ping_opaque_data;
	}
#endif

	if (options->ca_info.set_flag)
	{
		result = curl_easy_setopt(curl_easy_handle, CURLOPT_CAINFO, options->ca_info.option);
		if (result != CURLE_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_CAINFO failed: %s",
				strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
			goto l_err_curl_easy_setopt_ca_info;
		}
	}

	if (options->ca_path.set_flag)
	{
		result = curl_easy_setopt(curl_easy_handle, CURLOPT_CAPATH, options->ca_path.option);
		if (result != CURLE_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_CAPATH failed: %s",
				strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
			goto l_err_curl_easy_setopt_ca_path;
		}
	}

	if (options->ssl_cipher_list.set_flag)
	{
		result = curl_easy_setopt(curl_easy_handle, CURLOPT_SSL_CIPHER_LIST, options->ssl_cipher_list.option);
		if (result != CURLE_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_SSL_CIPHER_LIST failed: %s",
				strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
			goto l_err_curl_easy_setopt_ssl_cipher_list;
		}
	}

	if (options->ssl_cert_type.set_flag)
	{
		result = curl_easy_setopt(curl_easy_handle, CURLOPT_SSLCERTTYPE, options->ssl_cert_type.option);
		if (result != CURLE_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_SSLCERTTYPE failed: %s",
				strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
			goto l_err_curl_easy_setopt_ssl_cert_type;
		}
	}

	if (options->ssl_cert.set_flag)
	{
		result = curl_easy_setopt(curl_easy_handle, CURLOPT_SSLCERT, options->ssl_cert.option);
		if (result != CURLE_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_SSLCERT failed: %s",
				strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
			goto l_err_curl_easy_setopt_ssl_cert;
		}
	}

	if (options->ssl_key_type.set_flag)
	{
		result = curl_easy_setopt(curl_easy_handle, CURLOPT_SSLKEYTYPE, options->ssl_key_type.option);
		if (result != CURLE_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_SSLKEYTYPE failed: %s",
				strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
			goto l_err_curl_easy_setopt_ssl_key_type;
		}
	}

	if (options->ssl_key.set_flag)
	{
		result = curl_easy_setopt(curl_easy_handle, CURLOPT_SSLKEY, options->ssl_key.option);
		if (result != CURLE_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_SSLKEY failed: %s",
				strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
			goto l_err_curl_easy_setopt_ssl_key;
		}
	}

	if (options->base_hostname.set_flag)
	{
		result = curl_easy_setopt(curl_easy_handle, CURLOPT_BASE_HOSTNAME, options->base_hostname.option);
		if (result != CURLE_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_BASE_HOSTNAME failed: %s",
				strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
			goto l_err_curl_easy_setopt_ssl_key;
		}
	}

	result = curl_easy_setopt(curl_easy_handle, CURLOPT_URL, options->url);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_URL failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_url;
	}

	if (options->http_header)
	{
		result = curl_easy_setopt(curl_easy_handle, CURLOPT_HTTPHEADER, options->http_header);
		if (result != CURLE_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_HTTPHEADER failed: %s",
				strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
			goto l_err_curl_easy_setopt_http_header;
		}
	}

	return result;

l_err_curl_easy_setopt_http_header:
l_err_curl_easy_setopt_url:
l_err_curl_easy_setopt_ssl_key:
l_err_curl_easy_setopt_ssl_key_type:
l_err_curl_easy_setopt_ssl_cert:
l_err_curl_easy_setopt_ssl_cert_type:
l_err_curl_easy_setopt_ssl_cipher_list:
l_err_curl_easy_setopt_ca_path:
l_err_curl_easy_setopt_ca_info:
#ifdef USE_NGHTTP2
l_err_curl_easy_setopt_http2_ping_opaque_data:
#endif
l_err_curl_easy_setopt_timeout:
l_err_curl_easy_setopt_connect_timeout:
l_err_curl_easy_setopt_fresh_connect:
l_err_curl_easy_setopt_pipe_wait:
l_err_curl_easy_setopt_verbose:
l_err_curl_easy_setopt_debug_function:
l_err_curl_easy_setopt_http_version:
l_err_curl_easy_setopt_ssl_verify_host:
l_err_curl_easy_setopt_ssl_verify_peer:
l_err_curl_easy_setopt_ssl_enable_alpn:
l_err_curl_easy_setopt_ssL_version:
l_err_curl_easy_setopt_use_ssl:
	return result;
}

/**
 * @brief Prepare http GET request
 * @param[in]   options           Http client options
 * @param[in]   curl_easy_handle  Curl easy handle
 * @retval      Curl easy code
 */
static CURLcode zwu_http_client_option_prepare_http_get_request(const zwu_http_client_option_t *options, CURL *curl_easy_handle)
{
	CURLcode result;

	result = curl_easy_setopt(curl_easy_handle, CURLOPT_HTTPGET, 1L);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_HTTPGET failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_http_get;
	}

	return result;

l_err_curl_easy_setopt_http_get:
	return result;
}

/**
 * @brief Prepare http POST request
 * @param[in]   options           Http client options
 * @param[in]   curl_easy_handle  Curl easy handle
 * @param[in]   post_buffer       Post buffer
 * @param[in]   post_buffer_size  Post buffer size
 * @retval      Curl easy code
 */
static CURLcode zwu_http_client_option_prepare_http_post_request(const zwu_http_client_option_t *options, CURL *curl_easy_handle, const char *post_buffer, size_t post_buffer_size)
{
	CURLcode result;

	result = curl_easy_setopt(curl_easy_handle, CURLOPT_POST, 1L);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_POST failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_http_post;
	}

	// size of the data to copy from the buffer and send in the request
	// TODO: If post more than 2GB, then use CURLOPT_POSTFIELDSIZE_LARGE.
	// But in that case, most likely should use read_callback, as loading 2GB data in RAM is not reasonable
	result = curl_easy_setopt(curl_easy_handle, CURLOPT_POSTFIELDSIZE, post_buffer_size);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_POSTFIELDSIZE failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_post_field_size;
	}
	// send data from the local stack
	result = curl_easy_setopt(curl_easy_handle, CURLOPT_COPYPOSTFIELDS, post_buffer);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_COPYPOSTFIELDS failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_copy_post_fields;
	}

	return result;

l_err_curl_easy_setopt_copy_post_fields:
l_err_curl_easy_setopt_post_field_size:
l_err_curl_easy_setopt_http_post:
	return result;
}

/**
 * @brief Prepare http POST request with callback
 * @param[in]   options                Http client options
 * @param[in]   curl_easy_handle       Curl easy handle
 * @param[in]   post_read_callback     Post callback
 * @param[in]   context                Post callback context
 * @retval      Curl easy code
 */
static CURLcode zwu_http_client_option_prepare_http_post_request_with_callback(const zwu_http_client_option_t *options, CURL *curl_easy_handle,
	zwu_http_client_post_read_callback post_read_callback, void *context)
{
	CURLcode result;

	if (!post_read_callback)
	{
		result = CURLE_BAD_FUNCTION_ARGUMENT;  // consider as invalid argument
		goto l_err_invalid_buffer;
	}

	result = curl_easy_setopt(curl_easy_handle, CURLOPT_POST, 1L);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_POST failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_http_post;
	}

	result = curl_easy_setopt(curl_easy_handle, CURLOPT_READFUNCTION, post_read_callback);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_READFUNCTION failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_read_function;
	}

	result = curl_easy_setopt(curl_easy_handle, CURLOPT_READDATA, context);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_READDATA failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_read_data;
	}
#if 0
	// size of the data to copy from the buffer and send in the request
	result = curl_easy_setopt(curl_easy_handle, CURLOPT_POSTFIELDSIZE, post_buffer_size);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_POSTFIELDSIZE failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_post_field_size;
	}
#endif
	// Set CURLOPT_COPYPOSTFIELDS to NULL, so that libcurl will get the POST data from the read callback
	result = curl_easy_setopt(curl_easy_handle, CURLOPT_COPYPOSTFIELDS, NULL);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_COPYPOSTFIELDS failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_copy_post_fields;
	}
	return result;

l_err_curl_easy_setopt_copy_post_fields:
l_err_curl_easy_setopt_read_data:
l_err_curl_easy_setopt_read_function:
l_err_curl_easy_setopt_http_post:
l_err_invalid_buffer:
	return result;
}

/**
 * @brief Prepare HTTP2/PING request
 * @param[in]   options                Http client options
 * @param[in]   curl_easy_handle       Curl easy handle
 * @retval      Curl easy code
 */
static CURLcode zwu_http_client_option_prepare_http2_ping_request(const zwu_http_client_option_t *options, CURL *curl_easy_handle)
{
	CURLcode result;

	// TODO: set http version to enforce HTTP2?

	result = curl_easy_setopt(curl_easy_handle, CURLOPT_HTTP2PING, 1L);
	if (result != CURLE_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "curl_easy_setopt for CURLOPT_HTTP2PING failed: %s",
			strlen(options->error_buffer) ? options->error_buffer : curl_easy_strerror(result));
		goto l_err_curl_easy_setopt_http2_ping;
	}

	return result;

l_err_curl_easy_setopt_http2_ping:
	return result;
}

CURLcode zwu_http_client_option_prepare_http_request(const zwu_http_client_option_t *options, CURL *curl_easy_handle, zwu_http_client_request_enum_t request,
	const char *post_buffer, size_t post_buffer_size, zwu_http_client_post_read_callback post_read_callback, void *context)
{
	CURLcode result;

	result = CURLE_OK;
	switch(request)
	{
		case ZWU_HTTP_CLIENT_REQUEST_HTTP_GET:
			result = zwu_http_client_option_prepare_http_get_request(options, curl_easy_handle);
			break;
		case ZWU_HTTP_CLIENT_REQUEST_HTTP_POST:
			result = zwu_http_client_option_prepare_http_post_request(options, curl_easy_handle, post_buffer, post_buffer_size);
			break;
		case ZWU_HTTP_CLIENT_REQUEST_HTTP_POST_WITH_CALLBACK:
			result = zwu_http_client_option_prepare_http_post_request_with_callback(options, curl_easy_handle, post_read_callback, context);
			break;
		case ZWU_HTTP_CLIENT_REQUEST_HTTP2_PING:
			result = zwu_http_client_option_prepare_http2_ping_request(options, curl_easy_handle);
			break;
	}

	return result;
}

void zwu_http_client_option_init_with_default_value(zwu_http_client_option_t *curl_option)
{
	curl_option->use_ssl = ZWU_HTTP_CLIENT_USE_SSL_INVALID;
	curl_option->ssl_version = ZWU_HTTP_CLIENT_SSL_VERSION_INVALID;
	curl_option->ssl_enable_alpn = ZWU_HTTP_CLIENT_SSL_ENABLE_ALPN_INVALID;
	curl_option->ssl_verify_peer = ZWU_HTTP_CLIENT_SSL_VERIFY_PEER_INVALID;
	curl_option->ssl_verify_host = ZWU_HTTP_CLIENT_SSL_VERIFY_HOST_INVALID;
	curl_option->http_version = ZWU_HTTP_CLIENT_HTTP_VERSION_INVALID;
	curl_option->verbose = ZWU_HTTP_CLIENT_VERBOSE_INVALID;
	curl_option->pipe_wait = ZWU_HTTP_CLIENT_PIPE_WAIT_INVALID;
	curl_option->fresh_connect = ZWU_HTTP_CLIENT_FRESH_CONNECT_INVALID;
	curl_option->connect_timeout = ZWU_CURL_DEFAULT_OPTION_CONNECT_TIMEOUT;
	curl_option->timeout = ZWU_CURL_DEFAULT_OPTION_TIMEOUT;
	memset(curl_option->http2_ping_opaque_data, 0, sizeof(curl_option->http2_ping_opaque_data));
	curl_option->ca_info.set_flag = 0;
	curl_option->ca_info.option = NULL;
	curl_option->ca_path.set_flag = 0;
	curl_option->ca_path.option = NULL;
	curl_option->ssl_cipher_list.set_flag = 0;
	curl_option->ssl_cipher_list.option = NULL;
	curl_option->ssl_cert_type.set_flag = 0;
	curl_option->ssl_cert_type.option = NULL;
	curl_option->ssl_cert.set_flag = 0;
	curl_option->ssl_cert.option = NULL;
	curl_option->ssl_key_type.set_flag = 0;
	curl_option->ssl_key_type.option = NULL;
	curl_option->ssl_key.set_flag = 0;
	curl_option->ssl_key.option = NULL;
	curl_option->base_hostname.set_flag = 0;
	curl_option->base_hostname.option = NULL;
	curl_option->url.set_flag = 0;
	curl_option->url.option = NULL;
	curl_option->http_header = NULL;
	memset(curl_option->error_buffer, 0, sizeof(curl_option->error_buffer));
#if 0  // Donot set callback list, let the user application do it.
	curl_option->write_callback = NULL;
	curl_option->header_callback = NULL;
	curl_option->debug_callback = NULL;
	curl_option->http2_ping_callback = NULL;
#endif
}

void zwu_http_client_option_destroy(zwu_http_client_option_t *curl_option)
{
	// Just free all the strings
	curl_slist_free_all(curl_option->http_header);
	ZWP_FREE(curl_option->url.option);
	ZWP_FREE(curl_option->base_hostname.option);
	ZWP_FREE(curl_option->ssl_key.option);
	ZWP_FREE(curl_option->ssl_key_type.option);
	ZWP_FREE(curl_option->ssl_cert.option);
	ZWP_FREE(curl_option->ssl_cert_type.option);
	ZWP_FREE(curl_option->ssl_cipher_list.option);
	ZWP_FREE(curl_option->ca_path.option);
	ZWP_FREE(curl_option->ca_info.option);
}

zwu_status_t zwu_http_client_option_copy(const zwu_http_client_option_t *source,
	zwu_http_client_option_t *destination)
{
	char *ca_info;
	char *ca_path;
	char *ssl_cipher_list;
	char *ssl_cert_type;
	char *ssl_cert;
	char *ssl_key_type;
	char *ssl_key;
	char *base_hostname;
	char *url;
	struct curl_slist *http_header;

	ca_info = NULL;
	ca_path = NULL;
	ssl_cipher_list = NULL;
	ssl_cert_type = NULL;
	ssl_cert = NULL;
	ssl_key_type = NULL;
	ssl_key = NULL;
	base_hostname = NULL;
	url = NULL;
	http_header = NULL;

	// Allocate string to store option value 
#define ZWU_HTTP_CLIENT_STRING_OPTION_COPY(option_name) \
	if (source->option_name.option \
		&& ((option_name = ZWP_STRDUP(source->option_name.option)) == NULL)) \
		goto l_err_strdup_##option_name;
	ZWU_HTTP_CLIENT_STRING_OPTION_COPY(ca_info)
	ZWU_HTTP_CLIENT_STRING_OPTION_COPY(ca_path)
	ZWU_HTTP_CLIENT_STRING_OPTION_COPY(ssl_cipher_list)
	ZWU_HTTP_CLIENT_STRING_OPTION_COPY(ssl_cert_type)
	ZWU_HTTP_CLIENT_STRING_OPTION_COPY(ssl_cert)
	ZWU_HTTP_CLIENT_STRING_OPTION_COPY(ssl_key_type)
	ZWU_HTTP_CLIENT_STRING_OPTION_COPY(ssl_key)
	ZWU_HTTP_CLIENT_STRING_OPTION_COPY(base_hostname)
	ZWU_HTTP_CLIENT_STRING_OPTION_COPY(url)

	// Copy header link list
	if ((http_header = Curl_slist_duplicate(source->http_header)) == NULL)
	{
		goto l_err_curl_slist_duplicate;
	}

	// All the allocation done, let's make the stings/header pointer to proper address
	destination->ca_info.set_flag = source->ca_info.set_flag;
	destination->ca_info.option = ca_info;
	destination->ca_path.set_flag = source->ca_path.set_flag;
	destination->ca_path.option = ca_path;
	destination->ssl_cipher_list.set_flag = source->ssl_cipher_list.set_flag;
	destination->ssl_cipher_list.option = ssl_cipher_list;
	destination->ssl_cert_type.set_flag = source->ssl_cert_type.set_flag;
	destination->ssl_cert_type.option = ssl_cert_type;
	destination->ssl_cert.set_flag = source->ssl_cert.set_flag;
	destination->ssl_cert.option = ssl_cert;
	destination->ssl_key_type.set_flag = source->ssl_key_type.set_flag;
	destination->ssl_key_type.option = ssl_key_type;
	destination->ssl_key.set_flag = source->ssl_key.set_flag;
	destination->ssl_key.option = ssl_key;
	destination->base_hostname.set_flag = source->base_hostname.set_flag;
	destination->base_hostname.option = base_hostname;
	destination->url.set_flag = source->url.set_flag;
	destination->url.option = url;

	destination->http_header = http_header;

	// Copy enum options and long options
	destination->use_ssl = source->use_ssl;
	destination->ssl_version = source->ssl_version;
	destination->ssl_enable_alpn = source->ssl_enable_alpn;
	destination->ssl_verify_peer = source->ssl_verify_peer;
	destination->ssl_verify_host = source->ssl_verify_host;
	destination->http_version = source->http_version;
	destination->verbose = source->verbose;
	destination->pipe_wait = source->pipe_wait;
	destination->fresh_connect = source->fresh_connect;
	destination->connect_timeout = source->connect_timeout;
	destination->timeout = source->timeout;

	// Copy HTTP2/PING opaque data
	memcpy(destination->http2_ping_opaque_data, source->http2_ping_opaque_data, sizeof(destination->http2_ping_opaque_data));

	// DONOT copy callback list, as callback should be fixed.
#if 0
	destination->write_callback = source->write_callback;
	destination->header_callback = source->header_callback;
	destination->debug_callback = source->debug_callback;
	destination->http2_ping_callback = source->http2_ping_callback;
#endif
	// Donot copy error_buffer!

	return ZWU_STATUS_OK;

	curl_slist_free_all(http_header);
l_err_curl_slist_duplicate:
	ZWP_FREE(url);
l_err_strdup_url:
	ZWP_FREE(base_hostname);
l_err_strdup_base_hostname:
	ZWP_FREE(ssl_key);
l_err_strdup_ssl_key:
	ZWP_FREE(ssl_key_type);
l_err_strdup_ssl_key_type:
	ZWP_FREE(ssl_cert);
l_err_strdup_ssl_cert:
	ZWP_FREE(ssl_cert_type);
l_err_strdup_ssl_cert_type:
	ZWP_FREE(ssl_cipher_list);
l_err_strdup_ssl_cipher_list:
	ZWP_FREE(ca_path);
l_err_strdup_ca_path:
	ZWP_FREE(ca_info);
l_err_strdup_ca_info:
	return ZWU_STATUS_ERROR;
}

/**
 * @}
 */
