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
 * @file    zws_session.c
 *
 * @brief   Session.
 * @details Handles sessions.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-04-09
 * - Initial version
 */

#ifdef ZWS_WITH_FCGI
#include <fcgi_stdio.h>
#include <fcgios.h>
#else
#include <stdio.h>
#endif

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "zws_log.h"

#include "zws_session.h"
#include "zws_error.h"
#include "zws_memory.h"
#include "zws_io.h"
#include "zws_file.h"
#include "zws_exec_php_script.h"


/** PHP script file name that gets user name from session ID */
#define USER_NAME_GET_SCRIPT_FILE             "zweb/auth-cgi-bin/user_name_get.php"

/** PHP session ID cookie variable */
#define COOKIE_VAR_PHP_SESSION_ID             "PHPSESSID"
/** HTTP cookie variable name */
#define ENV_VAR_KEY_HTTP_COOKIE               "HTTP_COOKIE"

/** Default block size for read */
#define FILE_READ_DEFAULT_BLOCK_SIZE           128

/**
 * @brief Session class
 */
struct _zws_session
{
	zws_memcache_t                memcache;           /**< Memcache handle  */
};

/**
 * @brief Get session ID from environment
 * @param[out]    session_id_p      Session ID
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zws_session_id_free() to free resources
 */
static zws_status_t zws_session_id_get(char **session_id_p)
{
	char *http_cookie_external;
	char *http_cookie;
	char *session_id;
	char *cookie;
	char *next_cookie;
	char *key;
	char *value;

	session_id = NULL;

	if ((http_cookie_external = getenv(ENV_VAR_KEY_HTTP_COOKIE)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get environment variable: %s", ENV_VAR_KEY_HTTP_COOKIE);

		return ZWS_STATUS_EACCES;
	}

	if ((http_cookie = ZWS_STRDUP(http_cookie_external)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_http_cookie_alloc;
	}

	for (cookie = http_cookie; cookie; cookie = next_cookie)
	{
		// Consume spaces
		while ((*cookie == ' ') || (*cookie == '\t'))
		{
			cookie++;
		}

		// Break if end of string
		if (*cookie == '\0')
		{
			break;
		}

		if ((next_cookie = strstr(cookie, ";")))
		{
			*next_cookie = '\0';
			next_cookie++;
		}

		// Split key and value from cookie
		key = cookie;

		if (!(value = strstr(key, "=")))
		{
			ZWS_LOG(ZWS_LOG_ERR, "Warning: Failed to find key-value delimiter in cookie entry: %s", cookie);
			continue;
		}

		*value='\0';
		value++;

		if ((*value != '\0') && !strcmp(key, COOKIE_VAR_PHP_SESSION_ID))
		{
			session_id = value;
			break;
		}
	}

	if (session_id == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to find PHP session id");
		goto l_err_no_session_id;
	}

	if ((*session_id_p = ZWS_STRDUP(session_id)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_session_id_dup;
	}

	ZWS_FREE(http_cookie);

	return ZWS_STATUS_OK;

	ZWS_FREE(*session_id_p);
l_err_session_id_dup:
l_err_no_session_id:
	ZWS_FREE(http_cookie);
l_err_http_cookie_alloc:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Free session ID
 * @param[in]     session_id        Session ID
 * @return        Nothing
 * @see zws_session_id_get
 */
static void zws_session_id_free(char *session_id)
{
	ZWS_FREE(session_id);
}

/**
 * @brief Get session ID to user name mapping from external script
 * @param[in]     session_id        Session ID
 * @param[out]    user_name_p       User name
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call ZWS_FREE(*user_name_p) to free resources
 */
zws_status_t zws_session_user_name_get_external(char *session_id, char **user_name_p)
{
	char *user_name;
	int iRespLength;
	int iExitCode;
	zws_status_t return_status;

	return_status = ZWS_STATUS_ERROR;

	user_name = NULL;

	if (zws_exec_php_request(USER_NAME_GET_SCRIPT_FILE, 1, session_id, NULL, NULL, &user_name,
							 &iRespLength, &iExitCode) == ZWS_STATUS_OK)
	{
		if (iExitCode == 0 && user_name != NULL)
		{
			*user_name_p = user_name;
			return_status = ZWS_STATUS_OK;
		}
		else if (iExitCode == 253 || iExitCode == 254)
		{
			return_status = ZWS_STATUS_EACCES;		
		}
	}

	if (return_status != ZWS_STATUS_OK)
	{
		if (user_name != NULL)
		{
			ZWS_FREE(user_name);
		}
	}

	return return_status;
}

/*
 * @note   On success, call ZWS_FREE(*user_name_p)
 */
zws_status_t zws_session_user_name_get(zws_session_t session, char **user_name_p)
{
	char *session_id;
	int cache_hit;
	zws_status_t return_status;
	zws_status_t get_user_return_status;

	return_status = ZWS_STATUS_ERROR;

	cache_hit = 0;

	if ((get_user_return_status = zws_session_id_get(&session_id)) != ZWS_STATUS_OK)
	{
		return_status = get_user_return_status;
		goto l_err_session_id;
	}

	if (zws_memcache_get_user_name(session->memcache, session_id, user_name_p) == ZWS_STATUS_OK)
	{
		cache_hit = 1;
	}

	if (!cache_hit)
	{
		if ((get_user_return_status = zws_session_user_name_get_external(session_id, user_name_p)) != ZWS_STATUS_OK)
		{
			return_status = get_user_return_status;
			goto l_err_user_name_get_external;
		}

		// Replenish cache
		zws_memcache_set_user_name(session->memcache, session_id, *user_name_p);
	}

	zws_session_id_free(session_id);

	return ZWS_STATUS_OK;

l_err_user_name_get_external:
	zws_session_id_free(session_id);
l_err_session_id:
	return return_status;
}

zws_status_t zws_session_init(zws_session_t *session_p, zws_memcache_t memcache)
{
	static zws_session_t g_session;
	zws_session_t session;

	//Enforce singleton
	if (g_session)
	{
		ZWS_LOG(ZWS_LOG_WARNING, ZWS_MSG_SINGLETON_MULTIPLE_CALLS);
		*session_p = g_session;

		return ZWS_STATUS_OK;
	}

	if ((session = (zws_session_t)ZWS_MALLOC(sizeof(*session))) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_session_alloc;
	}

	session->memcache = memcache;

	g_session = *session_p = session;

	return ZWS_STATUS_OK;

	ZWS_FREE(session);
l_err_session_alloc:
	return ZWS_STATUS_ERROR;
}

void zws_session_exit(zws_session_t session)
{
	ZWS_FREE(session);
}
