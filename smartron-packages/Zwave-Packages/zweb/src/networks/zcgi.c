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
 * @file    zcgi.c
 *
 * @brief   Z-Wave web services.
 * @details Main file for Z-Wave web services application.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-01
 * - Initial version
 */

#ifdef ZWS_WITH_FCGI
#include <fcgi_stdio.h>
#include <fcgios.h>
#else
#include <stdio.h>
#endif

#include <stdlib.h>
#include <libgen.h>

#include "zws_log.h"
#if defined(ZWS_DEBUG_CGI)
#include "zws_log_enable.h"
#else
#include "zws_log_disable.h"
#endif

#include "zws_portal.h"
#include "zws_memcache.h"
#include "zws_session.h"
#include "zws_cgi.h"
#include "zws_avi.h"
#include "zws_error.h"
#include "zws_memory.h"

#ifndef LOCAL_CLIENT_ENABLE
#include "zwu_json.h"
#include "zws_ifttt_common.h"
#include "zws_cgi_service_ifttt.h"
#endif

/** Z-Wave Web CGI class */
typedef struct
{
	zws_portal_t           portal;                          /**< Portal object */
	int                    portal_receive_timeout;          /**< RPC response receive timeout */

	zws_avi_t              avi;                             /**< AVI object */
	zws_memcache_t         memcache;                        /**< Memcache object */
	zws_session_t          session;                         /**< Session object */
} *zcgi_t;

/** Z-Wave Web CGI singleton instance */
zcgi_t g_zcgi;

/**
 * @brief starts Z-Wave web service as CGI
 * @param[out]    zcgi_p            Z-Wave Web CGI object
 * @param[in]     argc              Number of command line arguments
 * @param[in]     argv              List of command line arguments
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zws_networks_exit() to free resources
 */
static zws_status_t zws_networks_init(zcgi_t *zcgi_p, int argc, const char * const argv[])
{
	zcgi_t zcgi;

	//Enforce singleton
	if (g_zcgi)
	{
		ZWS_LOG(ZWS_LOG_WARNING, ZWS_MSG_SINGLETON_MULTIPLE_CALLS);
		*zcgi_p = g_zcgi;

		return ZWS_STATUS_OK;
	}

	if ((zcgi = (zcgi_t)ZWS_MALLOC(sizeof(*zcgi))) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_networks_alloc;
	}

	if (zws_avi_init(&zcgi->avi) != ZWS_STATUS_OK)
	{
		goto l_err_avro_init;
	}

	if (zws_memcache_init(&zcgi->memcache) != ZWS_STATUS_OK)
	{
		goto l_err_memcache_init;
	}

	if (zws_session_init(&zcgi->session, zcgi->memcache) != ZWS_STATUS_OK)
	{
		goto l_err_session_init;
	}

	zws_portal_get_receive_timeout(&zcgi->portal_receive_timeout);

	g_zcgi = *zcgi_p = zcgi;

	return ZWS_STATUS_OK;

	zws_session_exit(zcgi->session);
l_err_session_init:
	zws_memcache_exit(zcgi->memcache);
l_err_memcache_init:
	zws_avi_exit(zcgi->avi);
l_err_avro_init:
	ZWS_FREE(zcgi);
l_err_networks_alloc:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Free Z-Wave Web CGI object
 * @param[in]     zcgi              Z-Wave Web CGI object
 * @return        Nothing
 * @see zws_networks_init
 */
static void zws_networks_exit(zcgi_t zcgi)
{
	zws_session_exit(zcgi->session);
	zws_memcache_exit(zcgi->memcache);
	zws_avi_exit(zcgi->avi);
	ZWS_FREE(zcgi);
	g_zcgi = NULL;
}

/**
 * @brief Send RPC request and receive RPC response
 * @param[in]     zcgi              Z-Wave Web CGI object
 * @param[in]     rpc_request       RPC request
 * @param[in]     rpc_request_length     Length of RPC request @a rpc_request
 * @param[out]    rpc_response_p    RPC response
 * @param[out]    rpc_response_length_p  Length of RPC response @a rpc_response_p
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zws_rpc_send_recv_free() to free resources
 */
static zws_status_t zws_rpc_send_recv(zcgi_t zcgi, char *rpc_request, int rpc_request_length,
		char **rpc_response_p, int *rpc_response_length_p)
{
	ZWS_LOG_DUMP(ZWS_LOG_DEBUG, rpc_request, rpc_request_length);

	//Start connection to portal
	if (zws_portal_init(&zcgi->portal, zcgi->portal_receive_timeout) != ZWS_STATUS_OK)
	{
		goto l_err_portal;
	}

	if (zws_portal_send(zcgi->portal, rpc_request, rpc_request_length) != ZWS_STATUS_OK)
	{
		goto l_err_portal_send;
	}

	if (zws_portal_recv(zcgi->portal, rpc_response_p, rpc_response_length_p) != ZWS_STATUS_OK)
	{
		goto l_err_portal_recv;
	}

	zws_portal_exit(zcgi->portal);

	return ZWS_STATUS_OK;

l_err_portal_recv:
l_err_portal_send:
	zws_portal_exit(zcgi->portal);
l_err_portal:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Free RPC response
 * @param[in]     rpc_response      RPC Response
 * @return        Nothing
 * @see zws_rpc_send_recv
 */
static void zws_rpc_send_recv_free(char *rpc_response)
{
	zws_portal_recv_free(rpc_response);
}

/**
 * @brief Handle a CGI request
 * @param[in]     zcgi              Z-Wave Web CGI object
 * @param[in]     service           Service object
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_cgi_handle(zcgi_t zcgi, zws_service_t service)
{
	char *rpc_request;
	int rpc_request_length;
	char *rpc_response;
	int rpc_response_length;

	if (zws_cgi_get_rpc_request_from_cgi_service(service, &rpc_request, &rpc_request_length) != ZWS_STATUS_OK)
	{
		goto l_err_avro_get_rpc_request;
	}

	rpc_response = NULL;
	rpc_response_length = 0;

	if (rpc_request_length > 0)
	{
		if (zws_rpc_send_recv(zcgi, rpc_request, rpc_request_length, &rpc_response, &rpc_response_length) != ZWS_STATUS_OK)
		{
			goto l_err_rpc_handle;
		}

		ZWS_LOG(ZWS_LOG_DEBUG, "zws_rpc_send_recv done.");
	}

	if (zws_cgi_set_cgi_service_result_from_rpc_response(service, rpc_response, rpc_response_length) != ZWS_STATUS_OK)
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "zws_cgi_set_cgi_service_result_from_rpc_response error.");

		goto l_err_cgi_set_cgi_service_result;
	}

	ZWS_LOG(ZWS_LOG_DEBUG, "zws_cgi_set_cgi_service_result_from_rpc_response done.");

	if (zws_cgi_send_cgi_output(service) != ZWS_STATUS_OK)
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "zws_cgi_send_cgi_output error.");

		goto l_err_cgi_send_output;
	}

	ZWS_LOG(ZWS_LOG_DEBUG, "zws_cgi_send_cgi_output done.");


	zws_rpc_send_recv_free(rpc_response);

	zws_cgi_free_rpc_request_from_cgi_service(rpc_request);

	return ZWS_STATUS_OK;

l_err_cgi_send_output:
l_err_cgi_set_cgi_service_result:
	zws_rpc_send_recv_free(rpc_response);
l_err_rpc_handle:
	zws_cgi_free_rpc_request_from_cgi_service(rpc_request);
l_err_avro_get_rpc_request:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Initialize Z-Ware Web (CGI)
 * @param[in]     argc              Number of command line arguments
 * @param[in]     argv List of      command line arguments
 * @retval        EXIT_SUCCESS      Success
 * @retval        EXIT_FAILURE      Failure
 */
int main(int argc, const char * const argv[])
{
	int return_status;
	int count;
	zcgi_t zcgi;
	zws_service_t service = NULL;
	int is_accept_called;

	#ifdef ZWS_WITH_FCGI
	FCGI_Init();
	#endif

	is_accept_called = 0;

#ifndef LOCAL_CLIENT_ENABLE
	zwu_json_init();
#endif

	if (zws_networks_init(&zcgi, argc, argv) != ZWS_STATUS_OK)
	{
		goto l_err_networks_init;
	}

	count = 0;

	#ifdef ZWS_WITH_FCGI
	while (FCGI_Accept() >= 0)
	#else
	if (1)
	#endif
	{
		is_accept_called = 1;

		service = NULL; //Reinit again as it might contain values from previous loop 

		ZWS_LOG(ZWS_LOG_DEBUG, "Request %d of current process", ++count);

		if ((return_status = zws_cgi_get_cgi_service_from_environment(&service, zcgi->avi, zcgi->session)) == ZWS_STATUS_OK)
		{

			if (zws_cgi_handle(zcgi, service) != ZWS_STATUS_OK)
			{
				zws_cgi_send_cgi_output_error(service);
			}			
		}
		else
		{
			ZWS_LOG(ZWS_LOG_ERR, "Get environment var error: %d", return_status);

			if (return_status == ZWS_STATUS_EACCES)
			{
				zws_cgi_send_cgi_output_access_denied();
			}
#ifndef LOCAL_CLIENT_ENABLE
			else if ((return_status == ZWS_STATUS_IFTTT_ERROR_PARAM) ||
					(return_status == ZWS_STATUS_IFTTT_ERROR_OA2_AT_INVALID) ||
					(return_status == ZWS_STATUS_IFTTT_ERROR_NOT_FOUND) ||
					(return_status == ZWS_STATUS_IFTTT_ERROR_SERVER) ||
					(return_status == ZWS_STATUS_IFTTT_ERROR_SERVICE_UNAVAILABLE)
					)
			{
				ZWS_LOG(ZWS_LOG_ERR, "IFTTT error");

				zws_cgi_ifttt_send_cgi_output_error(return_status, service);
			}
#endif
			else
			{
				zws_cgi_send_cgi_output_error(NULL);
			}
		}

		zws_cgi_free_cgi_service_from_environment(service);

		#ifdef ZWS_WITH_FCGI
		/* Sending EXIT_SUCCESS even in case of failure because this
		 * helps to send error message also with HTTP 200 OK response
		 */
		FCGI_SetExitStatus(EXIT_SUCCESS);
		#endif
	}

	if (!is_accept_called)
	{
		zws_cgi_send_cgi_output_error(NULL);
	}

	#ifdef ZWS_WITH_FCGI
	OS_LibShutdown();
	#endif

	zws_networks_exit(zcgi);

#ifndef LOCAL_CLIENT_ENABLE
	zwu_json_exit();
#endif

	ZWS_LOG(ZWS_LOG_DEBUG, "Exiting current process with %d requests", count);

	return EXIT_SUCCESS;

	zws_networks_exit(zcgi);
l_err_networks_init:

	#ifdef ZWS_WITH_FCGI
	if (FCGI_Accept() >= 0)
	#else
	if (1)
	#endif
	{
		zws_cgi_send_cgi_output_error(NULL);

		/* Sending EXIT_SUCCESS even in case of failure because this
		 * helps to send error message also with HTTP 200 OK response
		 */
		#ifdef ZWS_WITH_FCGI
		FCGI_SetExitStatus(EXIT_SUCCESS);
		#endif
	}

	#ifdef ZWS_WITH_FCGI
	OS_LibShutdown();
	#endif

	#ifndef LOCAL_CLIENT_ENABLE
	zwu_json_exit();
	#endif
	/* Sending EXIT_SUCCESS even in case of failure because this
	 * helps to send error message also with HTTP 200 OK response
	 */
	return EXIT_SUCCESS;
}

