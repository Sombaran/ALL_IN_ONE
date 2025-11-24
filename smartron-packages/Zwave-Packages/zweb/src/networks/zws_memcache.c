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
 * @file    zws_memcache.c
 *
 * @brief   Memcache client
 * @details Handles connections to memcached server.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-04-15
 * - Initial version
 */

#ifdef ZWS_WITH_FCGI
#include <fcgi_stdio.h>
#else
#include <stdio.h>
#endif

#include "zws_log.h"

#include "zws_memcache.h"
#include "zws_error.h"
#include "zws_memory.h"
#include "zws_file.h"

#ifdef ZWS_WITH_LIBMEMCACHED

#include <libmemcached/memcached.h>

#define ZWS_SESSION_PREFIX                  "zware.sess"
#define ZWS_SESSION_ITEM_USERNAME           "username"
#define ZWS_SESSION_EXPIRY                  (600)

#define MEMCACHED_CONFIG_STRING_SOCKET       "--SOCKET="
#define MEMCACHED_SOCKET_FILE                "install/memcached/var/run/memcached_socket"

/**
 * @brief Memcache class
 */
struct _zws_memcache
{
	memcached_st *memcached;
};

static zws_memcache_t g_memcache;

/*
 * @note On success, call ZWS_FREE(*value_p)
 */
static zws_status_t zws_memcache_get(zws_memcache_t memcache, const char *key, char **value_p, size_t *value_length_p)
{
	memcached_return_t error;
	char *value_external;
	char *value;
	size_t value_length;
	uint32_t flags;

	if ((value_external = memcached_get(memcache->memcached, key, strlen(key), &value_length, &flags, &error)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, "memcached_get() failed: %s", memcached_strerror(memcache->memcached, error));
		goto l_err_memcached_get;
	}

	/*
	 * Copying library allocated value to bring this under the fold of ZWS_*ALLOC() and ZWS_FREE()
	 */
	if ((value = (char *)ZWS_MALLOC(value_length + 1)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_value_alloc;
	}

	memcpy(value, value_external, value_length + 1);

	free(value_external);

	*value_p = value;
	*value_length_p = value_length;

	return ZWS_STATUS_OK;

l_err_value_alloc:
	free(value_external);
l_err_memcached_get:
	return ZWS_STATUS_ERROR;
}

static zws_status_t zws_memcache_set(zws_memcache_t memcache, const char *key, char *value, size_t value_length, uint64_t expiration)
{
	memcached_return_t error;

	if ((error = memcached_set(memcache->memcached, key, strlen(key),
			value, value_length, expiration, 0)) != MEMCACHED_SUCCESS)
	{
		ZWS_LOG(ZWS_LOG_ERR, "memcached_set() failed: %s", memcached_strerror(memcache->memcached, error));
		goto l_err_memcached_set;
	}

	return ZWS_STATUS_OK;

l_err_memcached_set:
	return ZWS_STATUS_ERROR;
}

/*
 * @note On success, call ZWS_FREE(*user_name_p)
 */
zws_status_t zws_memcache_get_user_name(zws_memcache_t memcache, const char *session_id, char **user_name_p)
{
	char *key;
	size_t user_name_length;

	if ((key = (char *)ZWS_MALLOC(sizeof(ZWS_SESSION_PREFIX)
			+ strlen(session_id) + 1
			+ sizeof(ZWS_SESSION_ITEM_USERNAME))) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_key_alloc;
	}
	sprintf(key, "%s.%s.%s", ZWS_SESSION_PREFIX, session_id, ZWS_SESSION_ITEM_USERNAME);

	if (zws_memcache_get(memcache, key, user_name_p, &user_name_length) != ZWS_STATUS_OK)
	{
		goto l_err_memcache_get;
	}

	ZWS_FREE(key);

	return ZWS_STATUS_OK;

l_err_memcache_get:
	ZWS_FREE(key);
l_err_key_alloc:
	return ZWS_STATUS_ERROR;
}

zws_status_t zws_memcache_set_user_name(zws_memcache_t memcache, const char *session_id, char *user_name)
{
	char *key;

	if ((key = (char *)ZWS_MALLOC(sizeof(ZWS_SESSION_PREFIX)
			+ strlen(session_id) + 1
			+ sizeof(ZWS_SESSION_ITEM_USERNAME))) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_key_alloc;
	}
	sprintf(key, "%s.%s.%s", ZWS_SESSION_PREFIX, session_id, ZWS_SESSION_ITEM_USERNAME);

	if (zws_memcache_set(memcache, key, user_name, strlen(user_name), ZWS_SESSION_EXPIRY) != ZWS_STATUS_OK)
	{
		goto l_err_memcache_set;
	}

	ZWS_FREE(key);

	return ZWS_STATUS_OK;

l_err_memcache_set:
	ZWS_FREE(key);
l_err_key_alloc:
	return ZWS_STATUS_ERROR;
}

zws_status_t zws_memcache_init(zws_memcache_t *memcache_p)
{
	zws_memcache_t memcache;
	char *root_dir_path;
	char *memcached_config_string;

	//Enforce singleton
	if (g_memcache)
	{
		ZWS_LOG(ZWS_LOG_WARNING, ZWS_MSG_SINGLETON_MULTIPLE_CALLS);
		*memcache_p = g_memcache;

		return ZWS_STATUS_OK;
	}

	if ((memcache = (zws_memcache_t)ZWS_MALLOC(sizeof(*memcache))) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_memcache_alloc;
	}

	if (zws_file_get_root_dir_path(&root_dir_path) != ZWS_STATUS_OK)
	{
		goto l_err_root_dir_path;
	}

	if ((memcached_config_string = ZWS_MALLOC(sizeof(MEMCACHED_CONFIG_STRING_SOCKET)
			+ 2  /* Quotes */
			+ strlen(root_dir_path) + 1
			+ sizeof(MEMCACHED_SOCKET_FILE))) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_memcached_config_string_alloc;
	}
	sprintf(memcached_config_string, "%s\"%s/%s\"", MEMCACHED_CONFIG_STRING_SOCKET, root_dir_path, MEMCACHED_SOCKET_FILE);

	if ((memcache->memcached = memcached(memcached_config_string, strlen(memcached_config_string))) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, "memcached() failed. Config string: %s", memcached_config_string);
		goto l_err_memcached_alloc;
	}

	ZWS_FREE(memcached_config_string);
	zws_file_free_root_dir_path(root_dir_path);

	g_memcache = *memcache_p = memcache;

	return ZWS_STATUS_OK;

	memcached_free(memcache->memcached);
l_err_memcached_alloc:
	ZWS_FREE(memcached_config_string);
l_err_memcached_config_string_alloc:
	zws_file_free_root_dir_path(root_dir_path);
l_err_root_dir_path:
	ZWS_FREE(memcache);
l_err_memcache_alloc:
	return ZWS_STATUS_ERROR;
}

void zws_memcache_exit(zws_memcache_t memcache)
{
	memcached_free(memcache->memcached);
	ZWS_FREE(memcache);
	g_memcache = NULL;
}

#else /* ZWS_WITH_LIBMEMCACHED */

zws_status_t zws_memcache_get_user_name(zws_memcache_t memcache, const char *session_id, char **user_name_p)
{
	return ZWS_STATUS_ERROR;
}

zws_status_t zws_memcache_set_user_name(zws_memcache_t memcache, const char *session_id, char *user_name)
{
	return ZWS_STATUS_ERROR;
}

zws_status_t zws_memcache_init(zws_memcache_t *memcache_p)
{
	*memcache_p = NULL;

	return ZWS_STATUS_OK;
}

void zws_memcache_exit(zws_memcache_t memcache) {}

#endif /* ZWS_WITH_LIBMEMCACHED */
