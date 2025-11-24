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
 * @file    zwportald.c
 *
 * @brief   Z-Ware portal daemon application.
 * @details Main file for Z-Ware portal daemon application.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-04-29
 * - Initial version
 */

#include <stdlib.h>
#include <libgen.h>
#include <signal.h>

#include "zwp_apr.h"
#include "zwp_signal.h"
#include "zwp_thread.h"
#include "zwp_hcapi.h"
#include "zwp_memory.h"
#include "zwp_error.h"
#include "zwp_status.h"
#include "zwp_config.h"
#include "zwp_directory_service.h"
#include "zwp_ifttt.h"
#include "zwp_portal.h"
#include "zwp_avi.h"
#include "zwp_server.h"
#include "zwp_log.h"

/** Main thread name */
#define ZWP_PORTALD_THREAD_NAME     "AMainThread"

/**
 * @brief Z-Wave application context class
 */
typedef struct
{
	zwp_config_t            app_config;              /**< Application configuration handle */
	zwp_logger_t            log;                     /**< Log handle */
	zwp_directory_service_t directory_service;       /**< Directory service handle */
	zwp_ifttt_t             ifttt;                   /**< IFTTT handle */
	zwp_portal_t            portal;                  /**< Z-Wave network provisioning handle */
	zwp_avi_t               avi;                     /**< Avro interface handle */
	zwp_server_t            server;                  /**< Avro service handle */

	zwp_thread_run_state_t  run_state;               /**< Thread run state ZWP_THREAD_RUN_STATE_XXX */
} *zwportald_t;

/** Z-Wave application context */
zwportald_t g_portald;

/**
 * @brief Start Z-Ware application daemon
 * @param[out]    portald_p         Z-Wave application context
 * @param[in]     argc              Number of command line arguments
 * @param[in]     argv              List of command line arguments
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @post @a portald_p refers to a singleton \link zwportald_t object \endlink
 * allocated/returned by this function (constructor)
 *
 * @note
 * @par
 * The caller should call zwportald_stop() to free resources
 *
 * @par
 * This function is not thread safe
 */
static zwp_status_t zwportald_start(zwportald_t *portald_p, int argc, const char * const argv[])
{
	zwportald_t portald;

	//Enforce singleton
	if (g_portald)
	{
		ZWP_LOG(ZWP_LOG_WARNING, ZWP_MSG_SINGLETON_MULTIPLE_CALLS);
		*portald_p = g_portald;

		return ZWP_STATUS_OK;
	}

	if ((portald = (zwportald_t)ZWP_MALLOC(sizeof(*portald))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_portald_alloc;
	}

	//Initialize the application configuration
	if (zwp_config_get_instance(&portald->app_config, argc, argv) != ZWP_STATUS_OK)
	{
		goto l_err_config;
	}

	//Start logging
	if (zwp_logger_start(&portald->log, zwp_config_get_log_config(portald->app_config)) != ZWP_STATUS_OK)
	{
		goto l_err_log;
	}
#ifndef LOCAL_CLIENT_ENABLE
	//Setup Directory Service connection
	if (zwp_directory_service_init(&portald->directory_service,
			zwp_config_get_directory_service_config(portald->app_config)) != ZWP_STATUS_OK)
	{
		goto l_err_directory_service;
	}

	if (zwp_ifttt_init(&portald->ifttt, zwp_config_get_ifttt_config(portald->app_config)) != ZWP_STATUS_OK)
	{
		goto l_err_ifttt;
	}
#endif

	//Setup AVI
	if (zwp_avi_init(&portald->avi, zwp_config_get_config_dir(portald->app_config)) != ZWP_STATUS_OK)
	{
		goto l_err_avi;
	}

	//Start portal
	if (zwp_portal_start(&portald->portal, zwp_config_get_portal_config(portald->app_config),
			portald->avi, portald->directory_service, portald->ifttt) != ZWP_STATUS_OK)
	{
		goto l_err_portal;
	}

	//Start server
	if (zwp_server_start(&portald->server, zwp_config_get_server_config(portald->app_config), portald->portal) != ZWP_STATUS_OK)
	{
		goto l_err_server;
	}

	g_portald = *portald_p = portald;

	//TODO Start Scenes module

	return ZWP_STATUS_OK;

l_err_server:
	zwp_portal_stop(portald->portal);
l_err_portal:
	zwp_avi_exit(portald->avi);
l_err_avi:
#ifndef LOCAL_CLIENT_ENABLE
	zwp_ifttt_exit(portald->ifttt);
l_err_ifttt:
	zwp_directory_service_exit(portald->directory_service);
l_err_directory_service:
#endif
	zwp_logger_stop(portald->log);
l_err_log:
	zwp_config_free(portald->app_config);
l_err_config:
	ZWP_FREE(portald);
l_err_portald_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Stop Z-Ware application daemon
 * @param[in]     portald           Z-Wave application context
 * @return        Nothing
 * @see zwportald_start
 */
static void zwportald_stop(zwportald_t portald)
{
	if (portald)
	{
		zwp_server_stop(portald->server);
		zwp_portal_stop(portald->portal);
		zwp_avi_exit(portald->avi);
#ifndef LOCAL_CLIENT_ENABLE
		zwp_ifttt_exit(portald->ifttt);
		zwp_directory_service_exit(portald->directory_service);
#endif
		zwp_logger_stop(portald->log);
		zwp_config_free(portald->app_config);
		ZWP_FREE(portald);
	}
}

/**
 * @brief Uninitialize Z-Ware application daemon
 * @return        Nothing
 */
static void zwportald_exit()
{
	ZWP_LOG(ZWP_LOG_DEBUG, "Stopping the portal");

	zwportald_stop(g_portald);

	zwp_utility_library_exit();
}

/**
 * @brief Signal handler for all signals that needs to terminate application
 * @return        Nothing
 */
static void zwportald_terminate(int signal_number)
{
	ZWP_LOG(ZWP_LOG_DEBUG, "Caught the signal '%s[%d]'", ZWP_SIGNAL_DESCRIPTION_GET(signal_number), signal_number);

	g_portald->run_state = ZWP_THREAD_RUN_STATE_STOPPABLE;
}

/**
 * @brief Initialize Z-Ware application daemon
 * @param[in]     argc              Number of command line arguments
 * @param[in]     argv List of      command line arguments
 * @retval        EXIT_SUCCESS      Success
 * @retval        EXIT_FAILURE      Failure
 */
int main(int argc, const char * const argv[])
{
	zwportald_t portald;

	//zwp_thread_name_set(ZWP_PORTALD_THREAD_NAME, NULL);

	if (zwp_utility_library_init(&argc, &argv, NULL) != ZWP_STATUS_OK)
	{
		goto l_err_util_lib_init;
	}

	ZWP_SIGNAL_BLOCK(SIGTERM);
	ZWP_SIGNAL_BLOCK(SIGINT);
	ZWP_SIGNAL_BLOCK(SIGHUP);

#ifdef LOCAL_CLIENT_ENABLE
	ZWP_SIGNAL_BLOCK(SIGPIPE);
#endif
	//atexit(zwportald_exit);

	if (zwportald_start(&portald, argc, argv) != ZWP_STATUS_OK)
	{
		goto l_err_portald_start;
	}

	portald->run_state = ZWP_THREAD_RUN_STATE_RUNNING;

	ZWP_SIGNAL(SIGTERM, zwportald_terminate);
	ZWP_SIGNAL(SIGINT, zwportald_terminate);
	ZWP_SIGNAL(SIGHUP, zwportald_terminate);

	ZWP_SIGNAL_UNBLOCK(SIGTERM);
	ZWP_SIGNAL_UNBLOCK(SIGINT);
	ZWP_SIGNAL_UNBLOCK(SIGHUP);

	//Wait until run_state is changed to 'stoppable' by application termination signal handlers
	while ((portald->run_state == ZWP_THREAD_RUN_STATE_RUNNING)
			&& (zwp_server_get_state(portald->server) != ZWP_SERVER_STATE_SHUT_DOWNABLE))
	{
		sleep(1);
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Portal shutting down");
	zwportald_exit();
	ZWP_LOG(ZWP_LOG_DEBUG, "Portal has shut down. Exit main thread.");

	pthread_exit(NULL);
	//return EXIT_SUCCESS;

l_err_portald_start:
	zwp_utility_library_exit();
l_err_util_lib_init:
	return EXIT_FAILURE;
}
