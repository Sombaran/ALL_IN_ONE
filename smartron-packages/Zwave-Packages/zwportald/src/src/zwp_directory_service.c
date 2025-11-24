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
 * @file    zwp_directory_service.c
 *
 * @brief   LDAP connection
 * @details Handles LDAP connections.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-06-05
 * - Initial version
 */

#include <ldap.h>
#include <stdint.h>
#include <stddef.h>

#include "zwp_log.h"
#if defined(ZWP_DEBUG_LDAP)
#include "zwp_log_enable.h"
#else
#include "zwp_log_disable.h"
#endif

#include "zwp_memory.h"
#include "zwp_error.h"
#include "zwp_directory_service.h"
#include "zwp_thread_mutex.h"
#include "zwu_time.h"

/** Default LDAP bind type */
#define ZWP_OPTION_DEFAULT_BIND_TYPE  "anonymous"

/** LDAP OU for ZIP Gateways */
#define ZWP_LDAP_ZIPGW_OU             "ou=ZIPGateways"

/** LDAP search filter for ZIP Gateway remote connect settings*/
#define ZWP_LDAP_ZIPGW_FILTER         "(objectClass=zwaveIPGwRemoteConnect)"

/** LDAP attribute for Remote Access Code for ZIP Gateway */
#define ZWP_LDAP_ZIPGW_ATTR_RA_CODE   "zwaveRemoteAccessCode"

/** LDAP attribute for Remote Access PIN for ZIP Gateway */
#define ZWP_LDAP_ZIPGW_ATTR_RA_PIN    "zwaveRemoteAccessPin"

/** LDAP attribute for ZIP Gateway Timezone information */
#define ZWP_LDAP_ZIPGW_ATTR_TIME_ZN   "zwaveTimeZone"

/** LDAP OU for users */
#define ZWP_LDAP_USER_OU              "ou=Users"

/** LDAP DN for users */
#define ZWP_LDAP_USER_DN_PREFIX       "uid="

/** LDAP filter for users */
#define ZWP_LDAP_USER_FILTER          "(objectClass=organizationalPerson)"

/** LDAP attribute for Z-Wave resource */
#define ZWP_LDAP_USER_ATTR_ZWAVE_RESOURCE  "zwaveResource"

/** Stringfy X */
#define STR(X)	#X
/** Default LDAP URI and Port */
#define ZWP_OPTION_DEFAULT_URI_PORT(ldaps_port,ldap_port) "ldaps://127.0.0.1:" STR(ldaps_port) ",ldap://127.0.0.1:" STR(ldap_port)
/** Default LDAP URI */
#define ZWP_OPTION_DEFAULT_URI ZWP_OPTION_DEFAULT_URI_PORT(LDAPS_PORT, LDAP_PORT)

/** List of LDAP bind types */
typedef enum
{
	ZWP_LDAP_BIND_TYPE_ANONYMOUS = 0,      /**< Anonymous bind */
	ZWP_LDAP_BIND_TYPE_SIMPLE = 1          /**< Simple bind */
} zwp_directory_service_bind_type_t;

/**
 * @brief LDAP configuration class
 */
typedef struct
{
	char                     *uri;                    /**< LDAP URI string */

	zwp_directory_service_bind_type_t     bind_type;  /**< LDAP bind type */
	union
	{
		struct
		{
			char             *dn;                     /**< Bind DN */
			char             *password;               /**< Bind password */
		} simple;                                     /**< Simple bind specific configuration */
	} bind_config;                                    /**< Bind type specific configuration */

	char                     *base_dn;                /**< Base DN */
} zwp_ldap_service_config_t;

/**
 * @brief Directory Service configuration class
 */
struct _zwp_directory_service_config
{
	zwp_ldap_service_config_t       ldap_service_config;          /**< LDAP Service configuration */
};

/**
 * @brief LDAP Service class
 */
typedef struct
{
	LDAP                             *ldap_connection;            /**< LDAP connection handle created by LDAP library */

	zwp_ldap_service_config_t        *ldap_service_config;        /**< LDAP configuration */

	zwp_directory_service_t          directory_service;           /**< Back reference to directory service */
} zwp_ldap_service_t;

/**
 * @brief Directory Service class
 */
struct _zwp_directory_service
{
	zwp_ldap_service_t               ldap_service;                /**< LDAP service handle */

	zwp_pool_t                       memory_pool;                /**< Memory pool for directory service */

	int                              service_state;               /**< service state. 0, if not ready. 1, if ready */
	zwu_time_t                       service_state_utime;         /**< Last updated time of 'service_state' */
	zwp_thread_mutex_t               service_state_lock;          /**< Mutex lock to protect 'service_state' */

	zwp_directory_service_config_t   directory_service_config;    /**< Directory service configuration */
};

/** List of LDAP bind type options */
static struct
{
	const char            *option;
	zwp_directory_service_bind_type_t  bind_type;
} zwp_directory_service_bind_type_options[] =
	{
		{"anonymous", ZWP_LDAP_BIND_TYPE_ANONYMOUS},
		{"simple", ZWP_LDAP_BIND_TYPE_SIMPLE}
	};

static void zwp_ldap_service_free(zwp_ldap_service_t *ldap_service);
static zwp_status_t zwp_directory_service_reopen(zwp_directory_service_t directory_service, zwp_ldap_service_t *ldap_service_dup);
static zwp_status_t zwp_directory_service_get_ldap_service(zwp_directory_service_t directory_service, zwp_ldap_service_t **ldap_service_p);

/**
 * @brief Get the single string corresponding to a single-value LDAP attribute
 * @param[in]     ldap_service      LDAP service object
 * @param[in]     entry             LDAP entry
 * @param[in]     attr              LDAP attribute
 * @param[in]     is_optional_attr  Flag to indicate if LDAP attribute <em> attr </em>is optional. 1, if optional; 0, if otherwise
 * @param[out]    string_p          LDAP attribute value
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_ldap_service_single_value_string_attr_free() to free resources
 *
 * @par
 * If the attribute value is successfully derived, <em> *string_p </em> is set to the attribute value and the call returns with <em> ZWP_STATUS_OK </em>.
 * If failed to derive the attribute value and <em> is_optional_attr </em> is 1, <em> *string_p </em> is set to NULL and the call returns with <em> ZWP_STATUS_OK </em>.
 * If failed to derive the attribute value and <em> is_optional_attr </em> is 0, <em> *string_p </em> is not set and the call returns with <em> ZWP_STATUS_ERROR </em>.
 */
static zwp_status_t zwp_ldap_service_single_value_string_attr_get(zwp_ldap_service_t *ldap_service, LDAPMessage *entry,
		const char *attr, int is_optional_attr,
		char **string_p)
{
	int ldap_code;
	struct berval **bv_string;
	char *string;

	if ((bv_string = ldap_get_values_len(ldap_service->ldap_connection, entry, attr)) == NULL)
	{
		ldap_code = LDAP_OTHER; ldap_get_option(ldap_service->ldap_connection, LDAP_OPT_RESULT_CODE, &ldap_code);
		ZWP_LOG(ZWP_LOG_WARNING, "Failed to get single string value for '%s': %s", attr, ldap_err2string(ldap_code));

		if (is_optional_attr)
		{
			*string_p = NULL;

			//Reset ld_errno to 'success' to suppress this error
			ldap_code = LDAP_SUCCESS;
			ldap_set_option(ldap_service->ldap_connection, LDAP_OPT_RESULT_CODE, &ldap_code);

			return ZWP_STATUS_OK;
		}
		else
		{
			goto l_err_value_get;
		}
	}

	if (bv_string[0] == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Empty value for the attribute '%s'", attr);
		goto l_err_invalid_value;
	}

	if ((string = (char *)ZWP_MALLOC(bv_string[0]->bv_len + 1)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_value_alloc;
	}
	memcpy(string, bv_string[0]->bv_val, bv_string[0]->bv_len);
	string[bv_string[0]->bv_len] = '\0';

	ldap_value_free_len(bv_string);

	*string_p = string;

	return ZWP_STATUS_OK;

l_err_value_alloc:
l_err_invalid_value:
	ldap_value_free_len(bv_string);
l_err_value_get:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free the single string corresponding to a single-value LDAP attribute
 * @param[in]     string            Attribute value
 * @return        Nothing
 * @see zwp_ldap_service_single_value_string_attr_get
 */
static void zwp_ldap_service_single_value_string_attr_free(char *string)
{
	ZWP_FREE(string);
}

/**
 * @brief Get the list of strings corresponding to a multi-value LDAP attribute
 * @param[in]     ldap_service      LDAP service object
 * @param[in]     entry             LDAP entry
 * @param[in]     attr              LDAP attribute
 * @param[in]     is_optional_attr  Flag to indicate if LDAP attribute <em> attr </em>is optional. 1, if optional; 0, if otherwise
 * @param[out]    string_list_p     LDAP attribute value list
 * @param[out]    string_count_p    Count of attribute values in <em> string_list_p </em>
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_ldap_service_multi_value_string_attr_free() to free resources
 *
 * @par
 * If the attribute values are successfully derived, <em> *string_list_p </em> is set to the list of attribute value strings; <em> *string_count_p </em> is set to the count of values in the list and the call returns with <em> ZWP_STATUS_OK </em>.
 * If failed to derive the attribute values and <em> is_optional_attr </em> is 1, <em> *string_list_p </em> is set to NULL; <em> *string_count_p </em> is set to 0 and the call returns with <em> ZWP_STATUS_OK </em>.
 * If failed to derive the attribute values and <em> is_optional_attr </em> is 0, <em> *string_list_p </em> and <em> *string_count_p </em> are not set and the call returns with <em> ZWP_STATUS_ERROR </em>.
 */
static zwp_status_t zwp_ldap_service_multi_value_string_attr_get(zwp_ldap_service_t *ldap_service, LDAPMessage *entry,
		const char *attr, int is_optional_attr,
		char ***string_list_p, int *string_count_p)
{
	int ldap_code;
	struct berval **bv_string;
	char **string_list;
	int string_count;
	int i;

	if ((bv_string = ldap_get_values_len(ldap_service->ldap_connection, entry, attr)) == NULL)
	{
		ldap_code = LDAP_OTHER; ldap_get_option(ldap_service->ldap_connection, LDAP_OPT_RESULT_CODE, &ldap_code);
		ZWP_LOG(ZWP_LOG_WARNING, "Failed to get multi string value for '%s': %s", attr, ldap_err2string(ldap_code));

		if (is_optional_attr)
		{
			*string_list_p = NULL;
			*string_count_p = 0;

			//Reset ld_errno to 'success' to suppress this error
			ldap_code = LDAP_SUCCESS;
			ldap_set_option(ldap_service->ldap_connection, LDAP_OPT_RESULT_CODE, &ldap_code);

			return ZWP_STATUS_OK;
		}
		else
		{
			goto l_err_value_get;
		}
	}

	string_count = ldap_count_values_len(bv_string);

	if ((string_list = (char **)ZWP_MALLOC(sizeof(*string_list) * string_count)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_string_alloc;
	}

	for (i = 0; (i < string_count) && (bv_string[i] != NULL); i++)
	{
		if ((string_list[i] = (char *)ZWP_MALLOC(bv_string[i]->bv_len + 1)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}
		memcpy(string_list[i], bv_string[i]->bv_val, bv_string[i]->bv_len);
		string_list[i][bv_string[i]->bv_len] = '\0';
	}

	ldap_value_free_len(bv_string);

	*string_list_p = string_list;
	*string_count_p = string_count;

	return ZWP_STATUS_OK;

l_err_value_alloc:
	ZWP_FREE(string_list);
l_err_string_alloc:
	ldap_value_free_len(bv_string);
l_err_value_get:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free the list of strings corresponding to a multi-value LDAP attribute
 * @param[in]     string_list       List of attribute values
 * @param[in]     string_count      Count of attribute values in <em> string_list </em>
 * @return        Nothing
 * @see zwp_ldap_service_multi_value_string_attr_get
 */
static void zwp_ldap_service_multi_value_string_attr_free(char **string_list, int string_count)
{
	int i;

	for (i = 0; i < string_count; i++)
	{
		ZWP_FREE(string_list[i]);
	}

	ZWP_FREE(string_list);
}

/**
 * @brief Set portal client profile using attributes from LDAP entry (e.g: Remote-Access-Code, Remote-Access-PIN)
 * @param[in]     ldap_service      LDAP service object
 * @param[in]     entry             LDAP entry
 * @param[out]    profile_p         Portal client profile
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_ldap_service_zipgw_profile_entry_get(zwp_ldap_service_t *ldap_service, LDAPMessage *entry, clnt_prof_t *profile_p)
{
	clnt_prof_t profile;
	char *ra_code;
	char *ra_pin;
	unsigned int ra_code_buf[8];
	int i;
	int is_ipv4;

	if (zwp_ldap_service_single_value_string_attr_get(ldap_service, entry, ZWP_LDAP_ZIPGW_ATTR_RA_CODE, 0, &ra_code) != ZWP_STATUS_OK)
	{
		goto l_err_ra_code;
	}

	if (zwp_ldap_service_single_value_string_attr_get(ldap_service, entry, ZWP_LDAP_ZIPGW_ATTR_RA_PIN, 0, &ra_pin) != ZWP_STATUS_OK)
	{
		goto l_err_ra_pin;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Remote Connect: %s|%s", ra_code, ra_pin);

	if (sscanf(ra_code, "%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x",
			&ra_code_buf[0], &ra_code_buf[1], &ra_code_buf[2], &ra_code_buf[3],
			&ra_code_buf[4], &ra_code_buf[5], &ra_code_buf[6], &ra_code_buf[7]) == 8)
	{
		for (i = 0; i < 8; i++)
		{
			if (ra_code_buf[i] <= 0xFF)
			{
				profile.clnt_id[i] = (uint8_t) ra_code_buf[i];
			}
			else
			{
				ZWP_LOG(ZWP_LOG_ERR, "Invalid value '%s' for '%s'", ra_code, ZWP_LDAP_ZIPGW_ATTR_RA_CODE);
				goto l_err_ra_code_parse;
			}
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid value '%s' for '%s'", ra_code, ZWP_LDAP_ZIPGW_ATTR_RA_CODE);
		goto l_err_ra_code_parse;
	}

	memcpy(&profile.clnt_pin, ra_pin, sizeof(profile.clnt_pin));

	//TODO: Rest of the configurations. Make these configurable through LDAP. Also to use libconfig to set default values
	//profile.clnt_ipv6_addr = "::"
	memset(profile.clnt_ipv6_addr, 0, sizeof(profile.clnt_ipv6_addr));
	profile.clnt_ipv6_prefix = 64;

	zwnet_ip_aton("3000::1", profile.svr_ipv6_addr, &is_ipv4);
	profile.svr_ipv6_prefix = 64;

	//profile.clnt_pan_prefix = "::";
	memset(profile.clnt_pan_prefix, 0, sizeof(profile.clnt_pan_prefix));
	//zwnet_ip_aton("3000::1", profile.clnt_dflt_gw, &is_ipv4);
	zwnet_ip_aton("::", profile.clnt_dflt_gw, &is_ipv4);

	zwnet_ip_aton("3000::1", profile.clnt_unsolicited_dst, &is_ipv4);
	profile.clnt_unsolicited_port = 4123;

	zwp_ldap_service_single_value_string_attr_free(ra_pin);
	zwp_ldap_service_single_value_string_attr_free(ra_code);

	memcpy(profile_p, &profile, sizeof(*profile_p));

	return ZWP_STATUS_OK;

l_err_ra_code_parse:
	ZWP_FREE(ra_pin);
l_err_ra_pin:
	ZWP_FREE(ra_code);
l_err_ra_code:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Perform an LDAP search for list of all ZIP gateways
 * @param[in]     ldap_service      LDAP service object
 * @param[out]    ldap_message_p    LDAP search result set for all ZIP Gateways
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call ldap_msgfree() to free resources
 *
 * @par
 * Equivalent shell command:
 * ldapsearch -LLL -x -b "ou=ZIPGateways,<base-dn>" '(objectClass=zwaveIPGwRemoteConnect)"
 */
static zwp_status_t zwp_ldap_service_zipgw_list_get(zwp_ldap_service_t *ldap_service, LDAPMessage **ldap_message_p)
{
	LDAPMessage *ldap_message;
	int ldap_code;
	char *search_base;

	ldap_message = NULL;

	if ((search_base = (char *)ZWP_MALLOC(sizeof(ZWP_LDAP_ZIPGW_OU) + strlen(ldap_service->ldap_service_config->base_dn) + 1)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_ldap_search_base_alloc;
	}
	sprintf(search_base, "%s,%s", ZWP_LDAP_ZIPGW_OU, ldap_service->ldap_service_config->base_dn);

	while ((ldap_code = ldap_search_ext_s(ldap_service->ldap_connection,
			search_base, LDAP_SCOPE_SUBTREE, ZWP_LDAP_ZIPGW_FILTER,
			NULL, 0,
			NULL, NULL,	NULL, LDAP_NO_LIMIT,
			&ldap_message)) != LDAP_SUCCESS)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to search '%s' under '%s': %s",
				ZWP_LDAP_ZIPGW_FILTER, ldap_service->ldap_service_config->base_dn, ldap_err2string(ldap_code));

		//Must be freed even in failure
		ldap_msgfree(ldap_message);

		if ((ldap_code == LDAP_SERVER_DOWN)
				&& (zwp_directory_service_reopen(ldap_service->directory_service, ldap_service) == ZWP_STATUS_OK))
		{
			continue;
		}

		goto l_err_ldap_search;
	}

	ZWP_FREE(search_base);

	*ldap_message_p = ldap_message;

	return ZWP_STATUS_OK;

l_err_ldap_search:
	ZWP_FREE(search_base);
l_err_ldap_search_base_alloc:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_directory_service_zipgw_profile_list_get(zwp_directory_service_t directory_service, clnt_prof_t **profile_list_p, int *profile_count_p)
{
	zwp_ldap_service_t *ldap_service;
	LDAPMessage *ldap_message;
	clnt_prof_t *profile_list;
	int profile_count;
	int entry_count;
	int ldap_code;
	LDAPMessage *entry;
	char *dn;

	if (zwp_directory_service_get_ldap_service(directory_service, &ldap_service) != ZWP_STATUS_OK)
	{
		goto l_err_ldap_service_get;
	}

	if (zwp_ldap_service_zipgw_list_get(ldap_service, &ldap_message) != ZWP_STATUS_OK)
	{
		goto l_err_zipgw_get;
	}

	if ((entry_count = ldap_count_entries(ldap_service->ldap_connection, ldap_message)) == -1)
	{
		ldap_code = LDAP_OTHER; ldap_get_option(ldap_service->ldap_connection, LDAP_OPT_RESULT_CODE, &ldap_code);
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get entry count for search '%s' under '%s': %s",
				ZWP_LDAP_ZIPGW_FILTER, ldap_service->ldap_service_config->base_dn, ldap_err2string(ldap_code));
		goto l_err_count_entries;
	}

	if ((profile_list = (clnt_prof_t *)ZWP_CALLOC(entry_count, sizeof(*profile_list))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_profile_list_alloc;
	}

	profile_count = 0;

	for (entry = ldap_first_entry(ldap_service->ldap_connection, ldap_message);
			entry != NULL;
			entry = ldap_next_entry(ldap_service->ldap_connection, entry))
	{
		if (zwp_ldap_service_zipgw_profile_entry_get(ldap_service, entry, &profile_list[profile_count]) != ZWP_STATUS_OK)
		{
			dn = ldap_get_dn(ldap_service->ldap_connection, entry);
			ZWP_LOG(ZWP_LOG_WARNING, "Failed to get profile from entry '%s'. Skipping the entry", dn);
			ZWP_FREE(dn);

			//Reset ld_errno to 'success' to separate this failure from ldap_next_entry()
			ldap_code = LDAP_SUCCESS;
			ldap_set_option(ldap_service->ldap_connection, LDAP_OPT_RESULT_CODE, &ldap_code);

			continue;
		}

		profile_count++;
	}

	ldap_code = LDAP_OTHER;	ldap_get_option(ldap_service->ldap_connection, LDAP_OPT_RESULT_CODE, &ldap_code);
	if (ldap_code != LDAP_SUCCESS)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get entries: %s", ldap_err2string(ldap_code));
		goto l_err_entry;
	}

	ldap_msgfree(ldap_message);

	zwp_ldap_service_free(ldap_service);

	*profile_list_p = profile_list;
	*profile_count_p = profile_count;

	return ZWP_STATUS_OK;

l_err_entry:
	ZWP_FREE(profile_list);
l_err_profile_list_alloc:
l_err_count_entries:
	ldap_msgfree(ldap_message);
l_err_zipgw_get:
	zwp_ldap_service_free(ldap_service);
l_err_ldap_service_get:
	return ZWP_STATUS_ERROR;
}

void zwp_directory_service_zipgw_profile_list_free(clnt_prof_t *profile_list)
{
	ZWP_FREE(profile_list);
}

/**
 * @brief Perform an LDAP search for a given ZIP Gateway using its Network ID (e.g. Remote-Access-Code)
 * @param[in]     ldap_service      LDAP service object
 * @param[in]     net_id            Network ID
 * @param[out]    ldap_message_p    LDAP search result set for the given ZIP Gateway
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call ldap_msgfree() to free resources
 *
 * @par
 * Equivalent shell command:
 * ldapsearch -LLL -x -b "ou=ZIPGateways,<base-dn>" 'zwaveRemoteAccessCode=<zipgw-rac>"
 */
static zwp_status_t zwp_ldap_service_zipgw_get(zwp_ldap_service_t *ldap_service, char *net_id, LDAPMessage **ldap_message_p)
{
	LDAPMessage *ldap_message;
	int ldap_code;
	char *search_base;
	char *search_filter;

	ldap_message = NULL;

	if ((search_base = (char *)ZWP_MALLOC(sizeof(ZWP_LDAP_ZIPGW_OU) + strlen(ldap_service->ldap_service_config->base_dn) + 1)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_ldap_search_base_alloc;
	}
	sprintf(search_base, "%s,%s", ZWP_LDAP_ZIPGW_OU, ldap_service->ldap_service_config->base_dn);

	if ((search_filter = (char *)ZWP_MALLOC(sizeof(ZWP_LDAP_ZIPGW_ATTR_RA_CODE) +
			strlen(net_id) + 1)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_ldap_search_filter_alloc;
	}
	sprintf(search_filter, "%s=%s", ZWP_LDAP_ZIPGW_ATTR_RA_CODE, net_id);

	ZWP_LOG(ZWP_LOG_DEBUG, "Search filter: %s", search_filter);

	while ((ldap_code = ldap_search_ext_s(ldap_service->ldap_connection,
			search_base, LDAP_SCOPE_SUBTREE, search_filter,
			NULL, 0,
			NULL, NULL,	NULL, LDAP_NO_LIMIT,
			&ldap_message)) != LDAP_SUCCESS)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to search '%s' under '%s': %s",
				search_filter, search_base, ldap_err2string(ldap_code));

		//Must be freed even in failure
		ldap_msgfree(ldap_message);

		if ((ldap_code == LDAP_SERVER_DOWN)
				&& (zwp_directory_service_reopen(ldap_service->directory_service, ldap_service) == ZWP_STATUS_OK))
		{
			continue;
		}

		goto l_err_ldap_search;
	}

	ZWP_FREE(search_filter);
	ZWP_FREE(search_base);

	*ldap_message_p = ldap_message;

	return ZWP_STATUS_OK;

l_err_ldap_search:
	ZWP_FREE(search_filter);
l_err_ldap_search_filter_alloc:
	ZWP_FREE(search_base);
l_err_ldap_search_base_alloc:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_directory_service_zipgw_profile_get(zwp_directory_service_t directory_service, char *net_id, clnt_prof_t **profile_p)
{
	zwp_ldap_service_t *ldap_service;
	LDAPMessage *ldap_message;
	clnt_prof_t *profile;
	int entry_count;
	int ldap_code;
	LDAPMessage *entry;
	char *dn;

	if (zwp_directory_service_get_ldap_service(directory_service, &ldap_service) != ZWP_STATUS_OK)
	{
		goto l_err_ldap_service_get;
	}

	if (zwp_ldap_service_zipgw_get(ldap_service, net_id, &ldap_message) != ZWP_STATUS_OK)
	{
		goto l_err_zipgw_get;
	}

	if ((entry_count = ldap_count_entries(ldap_service->ldap_connection, ldap_message)) == -1)
	{
		ldap_code = LDAP_OTHER; ldap_get_option(ldap_service->ldap_connection, LDAP_OPT_RESULT_CODE, &ldap_code);
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get entry count for search '%s' under '%s': %s",
				ZWP_LDAP_ZIPGW_FILTER, ldap_service->ldap_service_config->base_dn, ldap_err2string(ldap_code));
		goto l_err_count_entries;
	}

	if (entry_count == 0)
	{
		ldap_msgfree(ldap_message);

		zwp_ldap_service_free(ldap_service);

		*profile_p = NULL;

		return ZWP_STATUS_OK;
	}

	if ((profile = (clnt_prof_t *)ZWP_CALLOC(1, sizeof(*profile))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_profile_alloc;
	}

	// Expected to have at most 1 entry
	if ((entry = ldap_first_entry(ldap_service->ldap_connection, ldap_message)) != NULL)
	{
		if (zwp_ldap_service_zipgw_profile_entry_get(ldap_service, entry, profile) != ZWP_STATUS_OK)
		{
			dn = ldap_get_dn(ldap_service->ldap_connection, entry);
			ZWP_LOG(ZWP_LOG_WARNING, "Failed to get profile from entry '%s'. Skipping the entry", dn);
			ZWP_FREE(dn);

			//Reset ld_errno to 'success' to separate this failure from ldap_next_entry()
			ldap_code = LDAP_SUCCESS;
			ldap_set_option(ldap_service->ldap_connection, LDAP_OPT_RESULT_CODE, &ldap_code);

			goto l_err_profile_entry;
		}
	}

	ldap_code = LDAP_OTHER;	ldap_get_option(ldap_service->ldap_connection, LDAP_OPT_RESULT_CODE, &ldap_code);
	if (ldap_code != LDAP_SUCCESS)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get entries: %s", ldap_err2string(ldap_code));
		goto l_err_entry;
	}

	ldap_msgfree(ldap_message);

	zwp_ldap_service_free(ldap_service);

	*profile_p = profile;

	return ZWP_STATUS_OK;

l_err_entry:
l_err_profile_entry:
	ZWP_FREE(profile);
l_err_profile_alloc:
l_err_count_entries:
	ldap_msgfree(ldap_message);
l_err_zipgw_get:
	zwp_ldap_service_free(ldap_service);
l_err_ldap_service_get:
	return ZWP_STATUS_ERROR;
}

void zwp_directory_service_zipgw_profile_free(clnt_prof_t *profile)
{
	ZWP_FREE(profile);
}

/**
 * @brief Perform an LDAP search to get timezone for a given ZIP Gateway using its Network ID (e.g. Remote-Access-Code)
 * @param[in]     ldap_service      LDAP service object
 * @param[in]     net_id            Network ID
 * @param[out]    ldap_message_p    LDAP search result set containing timezone for the given ZIP Gateway
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call ldap_msgfree() to free resources
 *
 * @par
 * Equivalent shell command:
 * ldapsearch -LLL -x -b "zwaveRemoteAccessCode=<zipgw-rac>,ou=ZIPGateways,<base-dn>" 'objectClass=zwaveIPGwRemoteConnect' zwaveTimeZone
 */
static zwp_status_t zwp_ldap_service_search_timezone(zwp_ldap_service_t *ldap_service, char *net_id, LDAPMessage **ldap_message_p)
{
	LDAPMessage *ldap_message;
	int         ldap_code;
	char        *search_base;
	char        *attrs[] = {ZWP_LDAP_ZIPGW_ATTR_TIME_ZN, NULL};

	ldap_message = NULL;

	if ((search_base = (char *)ZWP_MALLOC(sizeof(ZWP_LDAP_ZIPGW_ATTR_RA_CODE) + strlen(net_id)
					   + sizeof(ZWP_LDAP_ZIPGW_OU) + strlen(ldap_service->ldap_service_config->base_dn) + 8)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		return ZWP_STATUS_ERROR;
	}

	sprintf(search_base, "%s=%s,%s,%s", ZWP_LDAP_ZIPGW_ATTR_RA_CODE, net_id, ZWP_LDAP_ZIPGW_OU, ldap_service->ldap_service_config->base_dn);
	//ZWP_LOG(ZWP_LOG_DEBUG, "Search base:%s", search_base);

	while ((ldap_code = ldap_search_ext_s(ldap_service->ldap_connection,
			search_base, LDAP_SCOPE_SUBTREE, ZWP_LDAP_ZIPGW_FILTER,
			attrs, 0,
			NULL, NULL,	NULL, LDAP_NO_LIMIT,
			&ldap_message)) != LDAP_SUCCESS)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to search '%s' under '%s': %s",
				ZWP_LDAP_ZIPGW_FILTER, search_base, ldap_err2string(ldap_code));

		//Must be freed even in failure
		ldap_msgfree(ldap_message);

		if ((ldap_code == LDAP_SERVER_DOWN)
				&& (zwp_directory_service_reopen(ldap_service->directory_service, ldap_service) == ZWP_STATUS_OK))
		{
			continue;
		}

		ZWP_FREE(search_base);
		return ZWP_STATUS_ERROR;

	}

	ZWP_FREE(search_base);

	*ldap_message_p = ldap_message;

	return ZWP_STATUS_OK;
}

/**
 * @brief Get timezone string based on the given network id
 * @param[in]   directory_service Directory service object
 * @param[in]   net_id	        Network id (e.g. 00-1E-32-FF-FF-12-49-A3)
 * @param[out]  timezone	    Time zone
 * @return ZWP_STATUS_XXX
 * @post   Caller has to free the returned timezone if return status is ZWP_STATUS_OK
 */
zwp_status_t zwp_directory_service_get_timezone(zwp_directory_service_t directory_service, char *net_id, char **timezone)
{
	zwp_ldap_service_t *ldap_service;
	LDAPMessage *ldap_message;
	LDAPMessage *entry;
	int         entry_count;
	int         ldap_code;
	zwp_status_t status;

	if (!net_id)
	{
		return ZWP_STATUS_ERROR;
	}

	if (zwp_directory_service_get_ldap_service(directory_service, &ldap_service) != ZWP_STATUS_OK)
	{
		return ZWP_STATUS_ERROR;
	}

	if (zwp_ldap_service_search_timezone(ldap_service, net_id, &ldap_message) != ZWP_STATUS_OK)
	{
		zwp_ldap_service_free(ldap_service);
		return ZWP_STATUS_ERROR;
	}

	if ((entry_count = ldap_count_entries(ldap_service->ldap_connection, ldap_message)) == -1)
	{
		ldap_code = LDAP_OTHER;
		ldap_get_option(ldap_service->ldap_connection, LDAP_OPT_RESULT_CODE, &ldap_code);
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get entry count for zwp_directory_service_search_timezone: %s", ldap_err2string(ldap_code));
		ldap_msgfree(ldap_message);
		zwp_ldap_service_free(ldap_service);
		return ZWP_STATUS_ERROR;
	}

	// Expected to have at most 1 entry because search is on a specific DN
	if ((entry = ldap_first_entry(ldap_service->ldap_connection, ldap_message)) != NULL)
	{
		status = zwp_ldap_service_single_value_string_attr_get(ldap_service, entry, ZWP_LDAP_ZIPGW_ATTR_TIME_ZN, 0,
				timezone);

		ldap_msgfree(ldap_message);

		zwp_ldap_service_free(ldap_service);

		return status;
	}

	ldap_code = LDAP_OTHER;
	ldap_get_option(ldap_service->ldap_connection, LDAP_OPT_RESULT_CODE, &ldap_code);
	if (ldap_code != LDAP_SUCCESS)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get entries: %s", ldap_err2string(ldap_code));
	}

	ldap_msgfree(ldap_message);
	zwp_ldap_service_free(ldap_service);
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Perform an LDAP search to get Z-Wave resource DN for a given user
 * @param[in]     ldap_service      LDAP service object
 * @param[in]     user              User name
 * @param[out]    ldap_message_p    LDAP search result set containing Z-Wave resource DN for a given user
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call ldap_msgfree() to free resources
 *
 * @par
 * Equivalent shell command:
 * ldapsearch -LLL -x -b "uid=<user>,ou=Users,<base-dn>" '(objectClass=organizationalPerson)' zwaveResource
 */
static zwp_status_t zwp_ldap_service_search_zwave_resource_dn_for_user(zwp_ldap_service_t *ldap_service, char *user, LDAPMessage **ldap_message_p)
{
	LDAPMessage *ldap_message;
	int ldap_code;
	char *search_base;
	char *attrs[] = {ZWP_LDAP_USER_ATTR_ZWAVE_RESOURCE, NULL};

	ldap_message = NULL;

	if ((search_base = (char *)ZWP_MALLOC(sizeof(ZWP_LDAP_USER_DN_PREFIX) + strlen(user) + 1 +
			sizeof(ZWP_LDAP_USER_OU) + strlen(ldap_service->ldap_service_config->base_dn) + 1)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_ldap_search_base_alloc;
	}
	sprintf(search_base, "%s%s,%s,%s", ZWP_LDAP_USER_DN_PREFIX, user, ZWP_LDAP_USER_OU, ldap_service->ldap_service_config->base_dn);

	while ((ldap_code = ldap_search_ext_s(ldap_service->ldap_connection,
			search_base, LDAP_SCOPE_SUBTREE, ZWP_LDAP_USER_FILTER,
			attrs, 0,
			NULL, NULL,	NULL, LDAP_NO_LIMIT,
			&ldap_message)) != LDAP_SUCCESS)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to search '%s' under '%s': %s",
				ZWP_LDAP_USER_FILTER, search_base, ldap_err2string(ldap_code));

		//Must be freed even in failure
		ldap_msgfree(ldap_message);

		if ((ldap_code == LDAP_SERVER_DOWN)
				&& (zwp_directory_service_reopen(ldap_service->directory_service, ldap_service) == ZWP_STATUS_OK))
		{
			continue;
		}

		goto l_err_ldap_search;
	}

	ZWP_FREE(search_base);

	*ldap_message_p = ldap_message;

	return ZWP_STATUS_OK;

l_err_ldap_search:
	ZWP_FREE(search_base);
l_err_ldap_search_base_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Get list of Z-Wave resource DNs from an LDAP result set entry
 * @param[in]     ldap_service      LDAP service object
 * @param[in]     entry             LDAP entry
 * @param[out]    zwave_resource_dn_p    List of Z-Wave resource DNs
 * @param[out]    zwave_resource_count_p Count of Z-Wave resource DNs in the list <em> zwave_resource_dn_p </em>
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_ldap_service_free_zwave_resource_dn_entry() to free resources
 */
static zwp_status_t zwp_ldap_service_get_zwave_resource_dn_entry(zwp_ldap_service_t *ldap_service, LDAPMessage *entry,
		char ***zwave_resource_dn_p, int *zwave_resource_count_p)
{
	if (zwp_ldap_service_multi_value_string_attr_get(ldap_service, entry, ZWP_LDAP_USER_ATTR_ZWAVE_RESOURCE, 1,
			zwave_resource_dn_p, zwave_resource_count_p) != ZWP_STATUS_OK)
	{
		goto l_err_zwave_resource_dn;
	}

	return ZWP_STATUS_OK;

l_err_zwave_resource_dn:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free the list of Z-Wave resource DNs got from an LDAP result set entry
 * @param[out]    zwave_resource_dn    List of Z-Wave resource DNs
 * @param[out]    zwave_resource_count Count of Z-Wave resource DNs in the list <em> zwave_resource_dn_p </em>
 * @return        Nothing
 * @see zwp_ldap_service_get_zwave_resource_dn_entry
 */
static void zwp_ldap_service_free_zwave_resource_dn_entry(char **zwave_resource_dn, int zwave_resource_count)
{
	zwp_ldap_service_multi_value_string_attr_free(zwave_resource_dn, zwave_resource_count);
}

/**
 * @brief Get the list of Z-Wave resource DNs for a given user
 * @param[in]     ldap_service      LDAP service object
 * @param[in]     user              User name
 * @param[out]    zwave_resource_dn_p    List of Z-Wave resource DNs
 * @param[out]    zwave_resource_count_p Count of Z-Wave resource DNs in the list <em> zwave_resource_dn_p </em>
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_ldap_service_free_zwave_resource_dn_for_user() to free resources
 */
static zwp_status_t zwp_ldap_service_get_zwave_resource_dn_for_user(zwp_ldap_service_t *ldap_service, char *user,
		char ***zwave_resource_dn_p, int *zwave_resource_count_p)
{
	LDAPMessage *ldap_message;
	int entry_count;
	int ldap_code;
	LDAPMessage *entry;
	char **zwave_resource_dn;
	int zwave_resource_count;
	char *dn;

	zwave_resource_dn = NULL;
	zwave_resource_count = 0;

	if (zwp_ldap_service_search_zwave_resource_dn_for_user(ldap_service, user, &ldap_message) != ZWP_STATUS_OK)
	{
		goto l_err_zwave_resource_dn_get;
	}

	if ((entry_count = ldap_count_entries(ldap_service->ldap_connection, ldap_message)) == -1)
	{
		ldap_code = LDAP_OTHER; ldap_get_option(ldap_service->ldap_connection, LDAP_OPT_RESULT_CODE, &ldap_code);
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get entry count for search '%s' under '%s%s,%s,%s': %s",
				ZWP_LDAP_USER_FILTER,
				ZWP_LDAP_USER_DN_PREFIX, user, ZWP_LDAP_USER_OU, ldap_service->ldap_service_config->base_dn,
				ldap_err2string(ldap_code));
		goto l_err_count_entries;
	}

	// Expected to have at most 1 entry because search is on a user
	if ((entry = ldap_first_entry(ldap_service->ldap_connection, ldap_message)) != NULL)
	{
		if (zwp_ldap_service_get_zwave_resource_dn_entry(ldap_service, entry, &zwave_resource_dn, &zwave_resource_count) != ZWP_STATUS_OK)
		{
			dn = ldap_get_dn(ldap_service->ldap_connection, entry);
			ZWP_LOG(ZWP_LOG_WARNING, "Failed to get zwave resource DNs from entry '%s'", dn);
			ZWP_FREE(dn);

			//Reset ld_errno to 'success' to separate this failure from ldap_next_entry()
			ldap_code = LDAP_SUCCESS;
			ldap_set_option(ldap_service->ldap_connection, LDAP_OPT_RESULT_CODE, &ldap_code);

			goto l_err_zwave_resource_dn_entry;
		}
	}

	ldap_code = LDAP_OTHER;	ldap_get_option(ldap_service->ldap_connection, LDAP_OPT_RESULT_CODE, &ldap_code);
	if (ldap_code != LDAP_SUCCESS)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get entries: %s", ldap_err2string(ldap_code));
		goto l_err_entry;
	}

	ldap_msgfree(ldap_message);

	*zwave_resource_dn_p = zwave_resource_dn;
	*zwave_resource_count_p = zwave_resource_count;

	return ZWP_STATUS_OK;

l_err_entry:
l_err_zwave_resource_dn_entry:
l_err_count_entries:
	ldap_msgfree(ldap_message);
l_err_zwave_resource_dn_get:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free the list of Z-Wave resource DNs for a given user
 * @param[out]    zwave_resource_dn    List of Z-Wave resource DNs
 * @param[out]    zwave_resource_count Count of Z-Wave resource DNs in the list <em> zwave_resource_dn_p </em>
 * @return        Nothing
 * @see zwp_ldap_service_get_zwave_resource_dn_for_user
 */
static void zwp_ldap_service_free_zwave_resource_dn_for_user(char **zwave_resource_dn, int zwave_resource_count)
{
	zwp_ldap_service_free_zwave_resource_dn_entry(zwave_resource_dn, zwave_resource_count);
}

/**
 * @brief Perform an LDAP search to get Remote-Access-Code from a Z-Wave resource DN
 * @param[in]     ldap_service      LDAP service object
 * @param[in]     zwave_resource_dn Z-Wave resource DN
 * @param[out]    ldap_message_p    LDAP search result set containing Remote-Access-Code from a Z-Wave resource DN
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call ldap_msgfree() to free resources
 *
 * @par
 * Equivalent shell command:
 * ./ldapsearch -LLL -x -b "<zwave-resource-dn>" '(objectClass=zwaveIPGwRemoteConnect)' zwaveRemoteAccessCode
 */
static zwp_status_t zwp_ldap_service_search_remote_access_code_for_zwave_resource_dn(zwp_ldap_service_t *ldap_service, char *zwave_resource_dn,
		LDAPMessage **ldap_message_p)
{
	LDAPMessage *ldap_message;
	int ldap_code;
	char *search_base;
	char *attrs[] = {ZWP_LDAP_ZIPGW_ATTR_RA_CODE, NULL};

	ldap_message = NULL;

	search_base = zwave_resource_dn;

	while ((ldap_code = ldap_search_ext_s(ldap_service->ldap_connection,
			search_base, LDAP_SCOPE_SUBTREE, ZWP_LDAP_ZIPGW_FILTER,
			attrs, 0,
			NULL, NULL,	NULL, LDAP_NO_LIMIT,
			&ldap_message)) != LDAP_SUCCESS)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to search '%s' under '%s': %s",
				ZWP_LDAP_ZIPGW_FILTER, search_base, ldap_err2string(ldap_code));

		//Must be freed even in failure
		ldap_msgfree(ldap_message);

		if ((ldap_code == LDAP_SERVER_DOWN)
				&& (zwp_directory_service_reopen(ldap_service->directory_service, ldap_service) == ZWP_STATUS_OK))
		{
			continue;
		}

		goto l_err_ldap_search;
	}

	*ldap_message_p = ldap_message;

	return ZWP_STATUS_OK;

l_err_ldap_search:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Get Network ID (Z-Wave resource) from an LDAP result set entry
 * @param[in]     ldap_service      LDAP service object
 * @param[in]     entry             LDAP entry
 * @param[out]    zwave_resource_p  Network ID (Z-Wave Resource)
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_ldap_service_free_zwave_resource_entry() to free resources
 */
static zwp_status_t zwp_ldap_service_get_zwave_resource_entry(zwp_ldap_service_t *ldap_service, LDAPMessage *entry,
		char **zwave_resource_p)
{
	if (zwp_ldap_service_single_value_string_attr_get(ldap_service, entry, ZWP_LDAP_ZIPGW_ATTR_RA_CODE, 0,
			zwave_resource_p) != ZWP_STATUS_OK)
	{
		goto l_err_zwave_resource_dn;
	}

	return ZWP_STATUS_OK;

l_err_zwave_resource_dn:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free the network ID (Z-Wave resource) got from an LDAP result set entry
 * @param[out]    zwave_resource    Network ID (Z-Wave Resource)
 * @return        Nothing
 * @see zwp_ldap_service_get_zwave_resource_entry
 */
static void zwp_ldap_service_free_zwave_resource_entry(char *zwave_resource)
{
	zwp_ldap_service_single_value_string_attr_free(zwave_resource);
}

zwp_status_t zwp_directory_service_get_network_list_for_user(zwp_directory_service_t directory_service, char *user, char ***network_list_p, int *network_count_p)
{
	zwp_ldap_service_t *ldap_service;
	LDAPMessage *ldap_message;
	int entry_count;
	int ldap_code;
	LDAPMessage *entry;
	char **network_list;
	int network_count;
	char **zwave_resource_dn;
	int zwave_resource_count;
	char *permission;
	char *dn;
	int i;

	network_list = NULL;
	network_count = 0;

	if (!user)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Failed to get network list. User is NULL");
		goto l_err_null_user;
	}

	if (zwp_directory_service_get_ldap_service(directory_service, &ldap_service) != ZWP_STATUS_OK)
	{
		goto l_err_ldap_service_get;
	}

	if (zwp_ldap_service_get_zwave_resource_dn_for_user(ldap_service, user, &zwave_resource_dn, &zwave_resource_count) != ZWP_STATUS_OK)
	{
		goto l_err_zwave_resource_dn_get;
	}

	if ((network_list = (char **)ZWP_MALLOC(sizeof(*network_list) * zwave_resource_count)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_network_list_alloc;
	}

	network_count = zwave_resource_count;

	for (i = 0; i < zwave_resource_count; i++)
	{
		if ((permission = strstr(zwave_resource_dn[i], "/")))
		{
			*permission = '\0';
			permission++;
		}

		//ZWP_LOG(ZWP_LOG_DEBUG, "Z-Wave Resource DN: %s", zwave_resource_dn[i]);

		if (zwp_ldap_service_search_remote_access_code_for_zwave_resource_dn(ldap_service, zwave_resource_dn[i], &ldap_message) != ZWP_STATUS_OK)
		{
			goto l_err_remote_access_code_get;
		}

		if ((entry_count = ldap_count_entries(ldap_service->ldap_connection, ldap_message)) == -1)
		{
			ldap_code = LDAP_OTHER; ldap_get_option(ldap_service->ldap_connection, LDAP_OPT_RESULT_CODE, &ldap_code);
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get entry count for search '%s' under '%s%s,%s,%s': %s",
					ZWP_LDAP_ZIPGW_FILTER,
					zwave_resource_dn[i],
					ldap_err2string(ldap_code));
			goto l_err_count_entries;
		}

		// Expected to have at most 1 entry because search is on a specific DN
		if ((entry = ldap_first_entry(ldap_service->ldap_connection, ldap_message)) != NULL)
		{
			if (zwp_ldap_service_get_zwave_resource_entry(ldap_service, entry, &network_list[i]) != ZWP_STATUS_OK)
			{
				dn = ldap_get_dn(ldap_service->ldap_connection, entry);
				ZWP_LOG(ZWP_LOG_WARNING, "Failed to get remote access code from zwave resource DN '%s'", dn);
				ZWP_FREE(dn);

				//Reset ld_errno to 'success' to separate this failure from ldap_next_entry()
				ldap_code = LDAP_SUCCESS;
				ldap_set_option(ldap_service->ldap_connection, LDAP_OPT_RESULT_CODE, &ldap_code);

				goto l_err_zwave_resource_entry;
			}
		}

		ldap_code = LDAP_OTHER;	ldap_get_option(ldap_service->ldap_connection, LDAP_OPT_RESULT_CODE, &ldap_code);
		if (ldap_code != LDAP_SUCCESS)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get entries: %s", ldap_err2string(ldap_code));
			goto l_err_entry;
		}

		ldap_msgfree(ldap_message);
	}

	zwp_ldap_service_free_zwave_resource_dn_for_user(zwave_resource_dn, zwave_resource_count);

	zwp_ldap_service_free(ldap_service);

	*network_list_p = network_list;
	*network_count_p = network_count;

	return ZWP_STATUS_OK;

l_err_entry:
l_err_zwave_resource_entry:
l_err_count_entries:
	ldap_msgfree(ldap_message);
l_err_remote_access_code_get:
	ZWP_FREE(network_list);
l_err_network_list_alloc:
	zwp_ldap_service_free_zwave_resource_dn_for_user(zwave_resource_dn, zwave_resource_count);
l_err_zwave_resource_dn_get:
	zwp_ldap_service_free(ldap_service);
l_err_ldap_service_get:
l_err_null_user:
	return ZWP_STATUS_ERROR;
}

void zwp_directory_service_free_network_list_for_user(char **network_list, int network_count)
{
	int i;

	for (i = 0; i < network_count; i++)
	{
		zwp_ldap_service_free_zwave_resource_entry(network_list[i]);
	}

	ZWP_FREE(network_list);
}

/**
 * @brief Perform an LDAP search to get an entry for a given user and a given network ID
 * @param[in]     ldap_service      LDAP service object
 * @param[in]     user              User name
 * @param[in]     net_id            Network ID
 * @param[out]    ldap_message_p    LDAP search result set for a given user and a given network ID
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call ldap_msgfree() to free resources
 *
 * @par
 * Equivalent shell command:
 * ldapsearch -LLL -x -b "uid=<user>,ou=Users,<base-dn>" 'zwaveResource=zwaveRemoteAccessCode=<zipgw-rac>,ou=ZIPGateways,<base-dn>*'
 */
static zwp_status_t zwp_ldap_service_search_for_network_in_user(zwp_ldap_service_t *ldap_service, char *user, char *net_id, LDAPMessage **ldap_message_p)
{
	LDAPMessage *ldap_message;
	int ldap_code;
	char *search_base;
	char *search_filter;

	ldap_message = NULL;

	if ((search_base = (char *)ZWP_MALLOC(sizeof(ZWP_LDAP_USER_DN_PREFIX) + strlen(user) + 1 +
			sizeof(ZWP_LDAP_USER_OU) + strlen(ldap_service->ldap_service_config->base_dn) + 1)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_ldap_search_base_alloc;
	}
	sprintf(search_base, "%s%s,%s,%s", ZWP_LDAP_USER_DN_PREFIX, user, ZWP_LDAP_USER_OU, ldap_service->ldap_service_config->base_dn);

	ZWP_LOG(ZWP_LOG_DEBUG, "Search base: %s", search_base);

	if ((search_filter = (char *)ZWP_MALLOC(sizeof(ZWP_LDAP_USER_ATTR_ZWAVE_RESOURCE) + sizeof(ZWP_LDAP_ZIPGW_ATTR_RA_CODE) +
			strlen(net_id) + 1 +
			sizeof(ZWP_LDAP_ZIPGW_OU) + strlen(ldap_service->ldap_service_config->base_dn) + 1 + 1)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_ldap_search_filter_alloc;
	}
	sprintf(search_filter, "%s=%s=%s,%s,%s*", ZWP_LDAP_USER_ATTR_ZWAVE_RESOURCE, ZWP_LDAP_ZIPGW_ATTR_RA_CODE,
			net_id,
			ZWP_LDAP_ZIPGW_OU, ldap_service->ldap_service_config->base_dn);

	ZWP_LOG(ZWP_LOG_DEBUG, "Search filter: %s", search_filter);

	while ((ldap_code = ldap_search_ext_s(ldap_service->ldap_connection,
			search_base, LDAP_SCOPE_SUBTREE, search_filter,
			NULL, 0,
			NULL, NULL,	NULL, LDAP_NO_LIMIT,
			&ldap_message)) != LDAP_SUCCESS)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to search '%s' under '%s': %s",
				search_filter, search_base, ldap_err2string(ldap_code));

		//Must be freed even in failure
		ldap_msgfree(ldap_message);

		if ((ldap_code == LDAP_SERVER_DOWN)
				&& (zwp_directory_service_reopen(ldap_service->directory_service, ldap_service) == ZWP_STATUS_OK))
		{
			continue;
		}

		goto l_err_ldap_search;
	}

	ZWP_FREE(search_filter);
	ZWP_FREE(search_base);

	*ldap_message_p = ldap_message;

	return ZWP_STATUS_OK;

l_err_ldap_search:
	ZWP_FREE(search_filter);
l_err_ldap_search_filter_alloc:
	ZWP_FREE(search_base);
l_err_ldap_search_base_alloc:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_directory_service_check_network_authorization_for_user(zwp_directory_service_t directory_service, char *user, char *net_id)
{
	zwp_ldap_service_t *ldap_service;
	LDAPMessage *ldap_message;
	int entry_count;
	int ldap_code;

	if (!user)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Failed authorization. User is NULL");
		goto l_err_null_user;
	}

	if (zwp_directory_service_get_ldap_service(directory_service, &ldap_service) != ZWP_STATUS_OK)
	{
		goto l_err_ldap_service_get;
	}

	if (zwp_ldap_service_search_for_network_in_user(ldap_service, user, net_id, &ldap_message) != ZWP_STATUS_OK)
	{
		goto l_err_search_for_network;
	}

	if ((entry_count = ldap_count_entries(ldap_service->ldap_connection, ldap_message)) == -1)
	{
		ldap_code = LDAP_OTHER; ldap_get_option(ldap_service->ldap_connection, LDAP_OPT_RESULT_CODE, &ldap_code);
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get entry count for search '%s=%s=%s,%s,%s*' under '%s%s,%s,%s': %s",
				ZWP_LDAP_USER_ATTR_ZWAVE_RESOURCE, ZWP_LDAP_ZIPGW_ATTR_RA_CODE,	net_id,	ZWP_LDAP_ZIPGW_OU, ldap_service->ldap_service_config->base_dn,
				ZWP_LDAP_USER_DN_PREFIX, user, ZWP_LDAP_USER_OU, ldap_service->ldap_service_config->base_dn,
				ldap_err2string(ldap_code));
		goto l_err_count_entries;
	}

	if (entry_count != 1)
	{
		ZWP_LOG(ZWP_LOG_ERR, "User '%s' is not authorized to access network '%s'", user, net_id);
		goto l_err_not_authorized;
	}

	ldap_msgfree(ldap_message);

	zwp_ldap_service_free(ldap_service);

	return ZWP_STATUS_OK;

l_err_not_authorized:
l_err_count_entries:
	ldap_msgfree(ldap_message);
l_err_search_for_network:
	zwp_ldap_service_free(ldap_service);
l_err_ldap_service_get:
l_err_null_user:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Duplicate LDAP service object within a directory service object
 * @param[in]     directory_service    Directory service object containing LDAP service object to be duplicated
 * @param[out]    ldap_service_dup_p   The duplicated LDAP service object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_ldap_service_free() to free resources
 *
 * @par
 * If <em> *ldap_service_dup_p </em> is NULL, a new LDAP service object is created before duplication.
 * If <em> *ldap_service_dup_p </em> is not NULL, the passed LDAP service object is used for duplication.
 */
static zwp_status_t zwp_directory_service_duplicate(zwp_directory_service_t directory_service, zwp_ldap_service_t **ldap_service_dup_p)
{
	zwp_ldap_service_t *ldap_service;
	zwp_ldap_service_t *ldap_service_dup;
	int ldap_code;
	int is_allocate;

	ldap_service = &directory_service->ldap_service;

	is_allocate = (*ldap_service_dup_p) ? 0 : 1;

	if (is_allocate)
	{
		if ((ldap_service_dup = (zwp_ldap_service_t *)ZWP_MALLOC(sizeof(*ldap_service_dup))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_directory_service_alloc;
		}

		memcpy(ldap_service_dup, ldap_service, sizeof(*ldap_service_dup));
	}
	else
	{
		ldap_service_dup = *ldap_service_dup_p;
	}

	if ((ldap_service_dup->ldap_connection = ldap_dup(ldap_service->ldap_connection)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to duplicate LDAP connection for thread: %s", strerror(errno));
		goto l_err_ldap_dup;
	}

	if (is_allocate)
	{
		*ldap_service_dup_p = ldap_service_dup;
	}

	return ZWP_STATUS_OK;

	if (ldap_service_dup->ldap_connection)
	{
		if ((ldap_code = ldap_destroy(ldap_service_dup->ldap_connection)) != LDAP_SUCCESS)
		{
			ZWP_LOG(ZWP_LOG_WARNING, "Failed to destroy LDAP duplicate handler: %s. Continuing...", ldap_err2string(ldap_code));
		}

		ldap_service_dup->ldap_connection = NULL;
	}
l_err_ldap_dup:
	if (is_allocate)
	{
		ZWP_FREE(ldap_service_dup);
	}
l_err_directory_service_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free LDAP service object
 * @param[in]     ldap_service      LDAP service object
 * @return        Nothing
 * @see zwp_directory_service_duplicate
 */
static void zwp_ldap_service_free(zwp_ldap_service_t *ldap_service)
{
	int ldap_code;

	if (ldap_service->ldap_connection)
	{
		if ((ldap_code = ldap_destroy(ldap_service->ldap_connection)) != LDAP_SUCCESS)
		{
			ZWP_LOG(ZWP_LOG_WARNING, "Failed to destroy LDAP duplicate handler: %s. Continuing...", ldap_err2string(ldap_code));
		}

		ldap_service->ldap_connection = NULL;
	}

	ZWP_FREE(ldap_service);
}

/**
 * @brief Open a new directory service (connection)
 * @param[in]     directory_service    Directory service object
 * @param[out]    ldap_service_dup_p   Duplicate LDAP service object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_directory_service_close() to free resources
 *
 * @par
 * If <em> ldap_service_dup_p </em> is not NULL, a duplicate LDAP service object is created and returned.
 * If <em> ldap_service_dup_p </em> is NULL, no duplicate LDAP service object is returned.
 */
static zwp_status_t zwp_directory_service_open(zwp_directory_service_t directory_service, zwp_ldap_service_t **ldap_service_dup_p)
{
	zwp_ldap_service_t *ldap_service;
	zwp_ldap_service_config_t *ldap_service_config;
	int ldap_code;
	int integer_option;

	ldap_service = &directory_service->ldap_service;

	if (!directory_service->service_state)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "LDAP service is opening...");

		ldap_service_config = ldap_service->ldap_service_config;

		if ((ldap_code = ldap_initialize(&ldap_service->ldap_connection, ldap_service_config->uri)) != LDAP_SUCCESS)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to initialize LDAP connection with URI '%s': %s", ldap_service_config->uri, ldap_err2string(ldap_code));
			goto l_err_ldap_init;
		}

		integer_option = LDAP_VERSION3;
		if ((ldap_code = ldap_set_option(ldap_service->ldap_connection, LDAP_OPT_PROTOCOL_VERSION, &integer_option)) != LDAP_OPT_SUCCESS)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to set LDAP option '%s': %s", "PROTOCOL_VERSION", ldap_err2string(ldap_code));
			goto l_err_ldap_opt_protocol_version;
		}

		if (ldap_service_config->bind_type == ZWP_LDAP_BIND_TYPE_SIMPLE)
		{
			if ((ldap_code = ldap_simple_bind_s(ldap_service->ldap_connection,
					ldap_service_config->bind_config.simple.dn, ldap_service_config->bind_config.simple.password)) != LDAP_SUCCESS)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to bind with DN '%s': %s", ldap_service_config->bind_config.simple.dn, ldap_err2string(ldap_code));
				goto l_err_ldap_bind;
			}
		}

		directory_service->service_state = 1;
		directory_service->service_state_utime = zwu_time_now();

		ZWP_LOG(ZWP_LOG_DEBUG, "LDAP service is opened");
	}

	if (ldap_service_dup_p)
	{
		if (zwp_directory_service_duplicate(directory_service, ldap_service_dup_p) != ZWP_STATUS_OK)
		{
			goto l_err_ldap_service_duplicate;
		}
	}

	return ZWP_STATUS_OK;

	if (ldap_service_dup_p)
	{
		zwp_ldap_service_free(*ldap_service_dup_p);
	}
l_err_ldap_service_duplicate:
l_err_ldap_bind:
l_err_ldap_opt_protocol_version:
	if (ldap_service->ldap_connection)
	{
		if ((ldap_code = ldap_destroy(ldap_service->ldap_connection)) != LDAP_SUCCESS)
		{
			ZWP_LOG(ZWP_LOG_WARNING, "Failed to destroy LDAP connection: %s", ldap_err2string(ldap_code));
		}

		ldap_service->ldap_connection = NULL;

		ZWP_LOG(ZWP_LOG_DEBUG, "LDAP service open failed");
	}

	directory_service->service_state = 0;
	directory_service->service_state_utime = zwu_time_now();
l_err_ldap_init:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Close a directory service (connection)
 * @param[in]     directory_service Directory service object
 * @return        Nothing
 * @see zwp_directory_service_open
 */
static void zwp_directory_service_close(zwp_directory_service_t directory_service)
{
	zwp_ldap_service_t *ldap_service;
	int ldap_code;

	ldap_service = &directory_service->ldap_service;

	if (ldap_service->ldap_connection)
	{
		//Equivalent to ldap_unbind() in case there are no other sibling sessions
		if ((ldap_code = ldap_destroy(ldap_service->ldap_connection)) != LDAP_SUCCESS)
		{
			ZWP_LOG(ZWP_LOG_WARNING, "Failed to destroy LDAP connection: %s. Continuing...", ldap_err2string(ldap_code));
		}

		ldap_service->ldap_connection = NULL;

		ZWP_LOG(ZWP_LOG_DEBUG, "LDAP service is closed");
	}

	directory_service->service_state = 0;
	directory_service->service_state_utime = zwu_time_now();
}

/**
 * @brief Reopen a directory service (connection)
 * @param[in]     directory_service Directory service object
 * @param[in]     ldap_service_dup  Duplicate LDAP service object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_directory_service_close() to free resources
 *
 * @par
 * If <em> ldap_service_dup </em> is not NULL, a duplicate LDAP service object is created.
 * If <em> ldap_service_dup </em> is NULL, no duplicate LDAP service object is created.
 */
static zwp_status_t zwp_directory_service_reopen(zwp_directory_service_t directory_service, zwp_ldap_service_t *ldap_service_dup)
{
	int ldap_code;
	zwu_time_t time_now;

	time_now = zwu_time_now();

	if (zwp_thread_mutex_lock(directory_service->service_state_lock) != ZWP_STATUS_OK)
	{
		goto l_err_service_state_lock;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "LDAP service is reopening... current state: %d", directory_service->service_state);

	if (ldap_service_dup->ldap_connection)
	{
		if ((ldap_code = ldap_destroy(ldap_service_dup->ldap_connection)) != LDAP_SUCCESS)
		{
			ZWP_LOG(ZWP_LOG_WARNING, "Failed to destroy LDAP duplicate handler: %s. Continuing...", ldap_err2string(ldap_code));
		}

		ldap_service_dup->ldap_connection = NULL;
	}

	/*
	 * Close the service only when no other thread has attempted to
	 * open/close after this function is called by 'this' thread.
	 */
	if (time_now > directory_service->service_state_utime)
	{
		zwp_directory_service_close(directory_service);
	}

	if (zwp_directory_service_open(directory_service, &ldap_service_dup) != ZWP_STATUS_OK)
	{
		goto l_err_directory_service_open;
	}

	zwp_thread_mutex_unlock(directory_service->service_state_lock);

	return ZWP_STATUS_OK;

	zwp_directory_service_close(directory_service);
l_err_directory_service_open:
	zwp_thread_mutex_unlock(directory_service->service_state_lock);
l_err_service_state_lock:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Initialize a directory service object
 * @param[in]     directory_service Directory service object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_directory_service_ldap_exit() to free resources
 */
static zwp_status_t zwp_directory_service_ldap_init(zwp_directory_service_t directory_service)
{
	zwp_ldap_service_t *ldap_service;

	ldap_service = &directory_service->ldap_service;

	//Link with directory service
	ldap_service->directory_service = directory_service;

	//Link with configuration
	ldap_service->ldap_service_config = &directory_service->directory_service_config->ldap_service_config;

	if (zwp_thread_mutex_lock(directory_service->service_state_lock) != ZWP_STATUS_OK)
	{
		goto l_err_service_state_lock;
	}

	if ((zwp_directory_service_open(directory_service, NULL)) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_WARNING, "Unable to connect to LDAP server. Continuing...");
		goto l_err_directory_service_open;
	}

	zwp_thread_mutex_unlock(directory_service->service_state_lock);

	return ZWP_STATUS_OK;

	zwp_directory_service_close(directory_service);
l_err_directory_service_open:
	zwp_thread_mutex_unlock(directory_service->service_state_lock);
l_err_service_state_lock:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Uninitialize a directory service object
 * @param[in]     directory_service Directory service object
 * @return        Nothing
 * @see zwp_directory_service_ldap_init
 */
static void zwp_directory_service_ldap_exit(zwp_directory_service_t directory_service)
{
	if (zwp_thread_mutex_lock(directory_service->service_state_lock) != ZWP_STATUS_OK)
	{
		goto l_err_service_state_lock;
	}

	zwp_directory_service_close(directory_service);

	zwp_thread_mutex_unlock(directory_service->service_state_lock);

	return;

	zwp_thread_mutex_unlock(directory_service->service_state_lock);
l_err_service_state_lock:
	return;
}

/**
 * @brief Get LDAP service object from a directory service object
 * @param[in]     directory_service Directory service object
 * @param[out]    ldap_service_p    LDAP service object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_directory_service_close() to free resources
 */
static zwp_status_t zwp_directory_service_get_ldap_service(zwp_directory_service_t directory_service, zwp_ldap_service_t **ldap_service_p)
{
	*ldap_service_p = NULL;

	if (zwp_thread_mutex_lock(directory_service->service_state_lock) != ZWP_STATUS_OK)
	{
		goto l_err_service_state_lock;
	}

	if ((zwp_directory_service_open(directory_service, ldap_service_p)) != ZWP_STATUS_OK)
	{
		goto l_err_directory_service_open;
	}

	zwp_thread_mutex_unlock(directory_service->service_state_lock);

	return ZWP_STATUS_OK;

	zwp_directory_service_close(directory_service);
l_err_directory_service_open:
	zwp_thread_mutex_unlock(directory_service->service_state_lock);
l_err_service_state_lock:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_directory_service_init(zwp_directory_service_t *directory_service_p, const zwp_directory_service_config_t directory_service_config)
{
	static zwp_directory_service_t g_directory_service;
	zwp_directory_service_t directory_service;

	//Enforce singleton
	if (g_directory_service)
	{
		ZWP_LOG(ZWP_LOG_WARNING, ZWP_MSG_SINGLETON_MULTIPLE_CALLS);
		*directory_service_p = g_directory_service;

		return ZWP_STATUS_OK;
	}

	if ((directory_service = (zwp_directory_service_t)ZWP_MALLOC(sizeof(*directory_service))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_directory_service_alloc;
	}

	//Link with configuration
	directory_service->directory_service_config = directory_service_config;

	if (zwp_pool_create(&directory_service->memory_pool, NULL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create memory pool for directory service");
		goto l_err_memory_pool_create;
	}

    if (zwp_thread_mutex_create(&directory_service->service_state_lock, directory_service->memory_pool, ZWP_THREAD_MUTEX_CREATE_NORMAL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create lock for network descriptors");
		goto l_err_service_state_lock_create;
	}

    directory_service->service_state = 0;
    directory_service->service_state_utime = zwu_time_now();

	if (zwp_directory_service_ldap_init(directory_service) != ZWP_STATUS_OK)
	{
		goto l_err_directory_service_ldap_init;
	}

	g_directory_service = *directory_service_p = directory_service;

	return ZWP_STATUS_OK;

	zwp_directory_service_ldap_exit(directory_service);
l_err_directory_service_ldap_init:
	zwp_thread_mutex_destroy(directory_service->service_state_lock);
l_err_service_state_lock_create:
	zwp_pool_destroy(directory_service->memory_pool);
l_err_memory_pool_create:
	ZWP_FREE(directory_service);
l_err_directory_service_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_directory_service_exit(zwp_directory_service_t directory_service)
{
	zwp_directory_service_ldap_exit(directory_service);

	zwp_thread_mutex_destroy(directory_service->service_state_lock);
	zwp_pool_destroy(directory_service->memory_pool);

	ZWP_FREE(directory_service);
}

/**
 * @brief Helper function that sets LDAP bind type based on the the configuration
 * @param[in]     option_bind_type  Configuration value for 'ldap.bind.type'
 * @param[out]    bind_type         Enumerated bind type value
 * @return        Nothing
 *
 * @note If no match, @a bind_type remains unchanged
 */
static void zwp_directory_service_option_set_bind_type(const char *option_bind_type, zwp_directory_service_bind_type_t *bind_type)
{
	int i, count;

	count = sizeof(zwp_directory_service_bind_type_options) / sizeof(*zwp_directory_service_bind_type_options);
	for (i = 0; i < count; i++)
	{
		if (strcasecmp(option_bind_type, zwp_directory_service_bind_type_options[i].option) == 0)
		{
			*bind_type = zwp_directory_service_bind_type_options[i].bind_type;
			break;
		}
	}
}

zwp_status_t zwp_directory_service_config_get_instance(zwp_directory_service_config_t *directory_service_config_p,
		const config_t *config_parser, const char *config_dir)
{
	static zwp_directory_service_config_t g_directory_service_config;
	zwp_directory_service_config_t directory_service_config;
	zwp_ldap_service_config_t *ldap_service_config;
	const char *uri;
	const char *bind_type;
	const char *bind_dn;
	const char *bind_password;
	const char *base_dn;

	//Enforce singleton
	if (g_directory_service_config)
	{
		ZWP_LOG(ZWP_LOG_WARNING, ZWP_MSG_SINGLETON_MULTIPLE_CALLS);
		*directory_service_config_p = g_directory_service_config;

		return ZWP_STATUS_OK;
	}

	if ((directory_service_config = (zwp_directory_service_config_t)ZWP_MALLOC(sizeof(*directory_service_config))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_directory_service_config_alloc;
	}

	ldap_service_config = &directory_service_config->ldap_service_config;

	//LDAP URI (optional)
	if (config_lookup_string(config_parser, "ldap.uri", &uri) == CONFIG_TRUE)
	{
		if ((ldap_service_config->uri = ZWP_STRDUP(uri)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_uri_alloc;
		}
	}
	else
	{
		//Default URI
		if ((ldap_service_config->uri = ZWP_STRDUP(ZWP_OPTION_DEFAULT_URI)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_uri_alloc;
		}
	}
	ZWP_LOG(ZWP_LOG_DEBUG, "LDAP URI: %s", ldap_service_config->uri);

	//LDAP bind type (optional)
	bind_type = ZWP_OPTION_DEFAULT_BIND_TYPE;
	config_lookup_string(config_parser, "ldap.bind.type", &bind_type);
	zwp_directory_service_option_set_bind_type(bind_type, &ldap_service_config->bind_type);

	//If simple bind
	if (ldap_service_config->bind_type == ZWP_LDAP_BIND_TYPE_SIMPLE)
	{
		//Bind DN
		if (config_lookup_string(config_parser, "ldap.bind.simple.dn", &bind_dn) != CONFIG_TRUE)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to lookup bind DN for simple bind");
			goto l_err_lookup_bind_dn;
		}

		if ((ldap_service_config->bind_config.simple.dn = ZWP_STRDUP(bind_dn)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_bind_dn_alloc;
		}

		//Bind password
		if (config_lookup_string(config_parser, "ldap.bind.simple.password", &bind_password) != CONFIG_TRUE)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to lookup bind password for simple bind");
			goto l_err_lookup_bind_password;
		}

		if ((ldap_service_config->bind_config.simple.password = ZWP_STRDUP(bind_password)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_bind_password_alloc;
		}
	}

	//Base DN
	if (config_lookup_string(config_parser, "ldap.base_dn", &base_dn) != CONFIG_TRUE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to lookup base DN");
		goto l_err_lookup_base_dn;
	}

	if ((ldap_service_config->base_dn = ZWP_STRDUP(base_dn)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_base_dn_alloc;
	}

	g_directory_service_config = *directory_service_config_p = directory_service_config;

	return ZWP_STATUS_OK;

l_err_base_dn_alloc:
l_err_lookup_base_dn:
	if (ldap_service_config->bind_type == ZWP_LDAP_BIND_TYPE_SIMPLE)
	{
		ZWP_FREE(ldap_service_config->bind_config.simple.password);
	}
l_err_bind_password_alloc:
l_err_lookup_bind_password:
	if (ldap_service_config->bind_type == ZWP_LDAP_BIND_TYPE_SIMPLE)
	{
		ZWP_FREE(ldap_service_config->bind_config.simple.dn);
	}
l_err_bind_dn_alloc:
l_err_lookup_bind_dn:
	ZWP_FREE(ldap_service_config->uri);
l_err_uri_alloc:
	ZWP_FREE(directory_service_config);
l_err_directory_service_config_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_directory_service_config_free(zwp_directory_service_config_t directory_service_config)
{
	zwp_ldap_service_config_t *ldap_service_config;

	ldap_service_config = &directory_service_config->ldap_service_config;

	ZWP_FREE(ldap_service_config->base_dn);
	if (ldap_service_config->bind_type == ZWP_LDAP_BIND_TYPE_SIMPLE)
	{
		ZWP_FREE(ldap_service_config->bind_config.simple.password);
		ZWP_FREE(ldap_service_config->bind_config.simple.dn);
	}
	ZWP_FREE(ldap_service_config->uri);
	ZWP_FREE(ldap_service_config);
}
