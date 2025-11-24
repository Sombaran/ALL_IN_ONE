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
 * @file    zwp_config.c
 *
 * @brief   Application configuration.
 * @details Handles overall application configuration.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-04-30
 * - Initial version
 */

#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <libconfig.h>

#include "zwp_memory.h"
#include "zwp_config.h"
#include "zwp_util_config.h"
#include "zwp_error.h"

/** Z-Ware portal daemon configuration file */
#define ZWP_CONFIG_FILE_NAME      "zwportald.conf"

/** Error message when configuration file is not specified */
#define ZWP_MSG_ERR_CONFIG_FILE_NOT_SPECIFIED "Configuration file not specified\n"

/**
 * @brief Z-Ware portal application configuration class
 */
struct _zwp_config
{
	char                  *program_name;           /**< Program name */
	char                  *config_file_path;       /**< Configuration file path */
	char                  *config_dir;             /**< Configuration directory */

	zwp_log_config_t      log_config;              /**< Configuration for logging module */
	zwp_directory_service_config_t
	                      directory_service_config;/**< Configuration for directory service module */
	zwp_ifttt_config_t    ifttt_config;            /**< Configuration for IFTTT module */
	zwp_portal_config_t   portal_config;           /**< Configuration for portal module */
	zwp_server_config_t   server_config;           /**< Configuration for Server module */
};

/** External variable used in parsing command line options */
extern char *optarg;

char *zwp_config_get_config_dir(zwp_config_t app_config)
{
	return app_config->config_dir;
}

zwp_log_config_t zwp_config_get_log_config(zwp_config_t app_config)
{
	return app_config->log_config;
}

zwp_directory_service_config_t zwp_config_get_directory_service_config(zwp_config_t app_config)
{
	return app_config->directory_service_config;
}

zwp_ifttt_config_t zwp_config_get_ifttt_config(zwp_config_t app_config)
{
	return app_config->ifttt_config;
}

zwp_portal_config_t zwp_config_get_portal_config(zwp_config_t app_config)
{
	return app_config->portal_config;
}

zwp_server_config_t zwp_config_get_server_config(zwp_config_t app_config)
{
	return app_config->server_config;
}

/**
 * @brief Get configuration file settings
 * @param[out] app_config        Application configuration object
 * @param[in]  config_file_path  Configuration file path
 * @retval     ZWP_STATUS_OK     Success
 * @retval     ZWP_STATUS_ERROR  Failure
 *
 * @post
 * Values set:
 * - \link zwp_config_t#log_config app_config->log_config \endlink
 * - \link zwp_config_t#portal_config app_config->portal_config \endlink
 *
 * @note The caller should call zwp_config_file_free() to free resources
 */
static zwp_status_t zwp_config_file_get(zwp_config_t app_config, const char *config_file_path)
{
	config_t config_parser;

	config_init(&config_parser);

	//Read and parse configuration settings from file
	if (config_read_file(&config_parser, config_file_path) != CONFIG_TRUE)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_ERR_CONFIG_READ_FILE(config_parser, config_file_path));
		goto l_err_config_read_file;
	}

	//Fill up log configuration
	if (zwp_log_config_get_instance(&app_config->log_config,
			&config_parser, app_config->config_dir, basename(app_config->program_name)) != ZWP_STATUS_OK)
	{
		goto l_err_config_log;
	}
#ifndef LOCAL_CLIENT_ENABLE
	//Fill up directory service configuration
	if (zwp_directory_service_config_get_instance(&app_config->directory_service_config,
			&config_parser, app_config->config_dir) != ZWP_STATUS_OK)
	{
		goto l_err_config_directory_service;
	}

	//Fill up ifttt configuration
	if (zwp_ifttt_config_get_instance(&app_config->ifttt_config,
			&config_parser, app_config->config_dir) != ZWP_STATUS_OK)
	{
		goto l_err_config_ifttt;
	}
#endif

	//Fill up portal configuration
	if (zwp_portal_config_get_instance(&app_config->portal_config,
			&config_parser, app_config->config_dir) != ZWP_STATUS_OK)
	{
		goto l_err_config_portal;
	}

	//Fill up Server configuration
	if (zwp_server_config_get_instance(&app_config->server_config,
			&config_parser, app_config->config_dir) != ZWP_STATUS_OK)
	{
		goto l_err_config_server;
	}

	config_destroy(&config_parser);

	return ZWP_STATUS_OK;

	zwp_server_config_free(app_config->server_config);
l_err_config_server:
	zwp_portal_config_free(app_config->portal_config);
l_err_config_portal:
#ifndef LOCAL_CLIENT_ENABLE
	zwp_ifttt_config_free(app_config->ifttt_config);
l_err_config_ifttt:
	zwp_directory_service_config_free(app_config->directory_service_config);
l_err_config_directory_service:
#endif
	zwp_log_config_free(app_config->log_config);
l_err_config_log:
l_err_config_read_file:
	config_destroy(&config_parser);
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free configuration file settings
 * @param[in]  app_config        Application configuration object
 * @return     Nothing
 * @see zwp_config_file_get
 */
static void zwp_config_file_free(zwp_config_t app_config)
{
	zwp_server_config_free(app_config->server_config);
	zwp_portal_config_free(app_config->portal_config);
#ifndef LOCAL_CLIENT_ENABLE
	zwp_ifttt_config_free(app_config->ifttt_config);
	zwp_directory_service_config_free(app_config->directory_service_config);
#endif
	zwp_log_config_free(app_config->log_config);
}

/**
 * @brief Get command line options
 * @par
 * The command line option "-f <file_path>" is used to specify configuration file path.
 * If unspecified, etc/zwportald.conf is used where 'etc' is the system configuration directory
 * defined by GNU Autoconf scripts.
 *
 * @param[out] app_config        Application configuration object
 * @param[in]  argc              Number of command line arguments
 * @param[in]  argv              List of command line arguments
 * @retval     ZWP_STATUS_OK     Success
 * @retval     ZWP_STATUS_ERROR  Failure

 * @post
 * Values set:
 * - \link zwp_config_t#program_name app_config->program_name \endlink
 * - \link zwp_config_t#config_file_path app_config->config_file_path \endlink
 * - \link zwp_config_t#config_dir app_config->config_dir \endlink
 *
 * @note The caller should call zwp_config_cmdline_free() to free resources
 */
static zwp_status_t zwp_config_cmdline_get(zwp_config_t app_config, int argc, const char * const argv[])
{
	int option;
	char *path;

	if ((app_config->program_name = ZWP_STRDUP(argv[0])) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_program_name_alloc;
	}

	app_config->config_file_path = NULL;

	while((option = getopt(argc, (char * const*)argv, "f:")) != -1)
	{
		switch(option)
		{
		case 'f':
			if ((app_config->config_file_path = ZWP_STRDUP(optarg)) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			}
			break;

		default: /* '?' */
			break;
		}
	}

	if (app_config->config_file_path == NULL)
	{
#ifdef ZWP_SYSCONFDIR
		if ((app_config->config_file_path =	ZWP_STRDUP(ZWP_SYSCONFDIR "/" ZWP_CONFIG_FILE_NAME)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_config_file_path_alloc;
		}
#endif
	}

	if (app_config->config_file_path == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_ERR_CONFIG_FILE_NOT_SPECIFIED);
		goto l_err_config_file_path_alloc;
	}

	//Derive configuration directory
	if ((path = ZWP_STRDUP(app_config->config_file_path)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_config_file_path_copy;
	}
	if ((app_config->config_dir = ZWP_STRDUP(dirname(path))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_config_dir_alloc;
	}
	ZWP_FREE(path);

	return ZWP_STATUS_OK;

l_err_config_dir_alloc:
	ZWP_FREE(path);
l_err_config_file_path_copy:
	ZWP_FREE(app_config->config_file_path);
l_err_config_file_path_alloc:
	ZWP_FREE(app_config->program_name);
l_err_program_name_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free command line options
 * @param[in]  app_config        Application configuration object
 * @return     Nothing
 * @see zwp_config_cmdline_get
 */
static void zwp_config_cmdline_free(zwp_config_t app_config)
{
	ZWP_FREE(app_config->config_dir);
	ZWP_FREE(app_config->config_file_path);
	ZWP_FREE(app_config->program_name);
}

zwp_status_t zwp_config_get_instance(zwp_config_t *app_config_p, int argc, const char * const argv[])
{
	static zwp_config_t g_app_config;
	zwp_config_t app_config;

	//Enforce singleton
	if (g_app_config)
	{
		ZWP_LOG(ZWP_LOG_WARNING, ZWP_MSG_SINGLETON_MULTIPLE_CALLS);
		*app_config_p = g_app_config;

		return ZWP_STATUS_OK;
	}

	if ((app_config = (zwp_config_t)ZWP_MALLOC(sizeof(*app_config))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_config_alloc;
	}

	//Get configurations from command line arguments
	if (zwp_config_cmdline_get(app_config, argc, argv) != ZWP_STATUS_OK)
	{
		goto l_err_config_getopt_alloc;
	}

	//Get configurations from file
	if (zwp_config_file_get(app_config, app_config->config_file_path) != ZWP_STATUS_OK)
	{
		goto l_err_config_parse;
	}

	g_app_config = *app_config_p = app_config;

	return ZWP_STATUS_OK;

l_err_config_parse:
	zwp_config_cmdline_free(app_config);
l_err_config_getopt_alloc:
	ZWP_FREE(app_config);
l_err_config_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_config_free(zwp_config_t app_config)
{
	zwp_config_file_free(app_config);
	zwp_config_cmdline_free(app_config);
	ZWP_FREE(app_config);
}
