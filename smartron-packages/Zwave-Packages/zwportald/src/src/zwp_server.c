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
 * @file    zwp_server.c
 *
 * @brief   Portal server
 * @details Module that exports the server
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-06-31
 * - Initial version
 */

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "zwp_log.h"
#if defined(ZWP_DEBUG_AVI)
#include "zwp_log_enable.h"
#else
#include "zwp_log_disable.h"
#endif

#include "zwp_server.h"
#include "zwp_client.h"
#include "zwp_error.h"
#include "zwp_thread.h"
#include "zwp_memory.h"
#include "zwp_avi.h"
#include "zwp_thread_pool.h"

/** Unix domain socket path */
#define ZWP_SERVER_SOCKET_PATH            "#zwportald_socket"

/** Thread name for server */
#define ZWP_SERVER_THREAD_NAME            "AServer"

/** Thread name for server workers */
#define ZWP_SERVER_WORKER_THREAD_NAME     "AServerWorker"

/** Default initial worker thread count */
#define ZWP_OPTION_DEFAULT_THREAD_INIT_COUNT               10
/** Default maximum worker thread count */
#define ZWP_OPTION_DEFAULT_THREAD_MAX_COUNT                50

/**
 * @brief Server configuration class
 */
struct _zwp_server_config
{
	struct
	{
		int             thread_init_count;       /**< Initial thread count */
		int             thread_max_count;        /**< Maximum thread count */
	} thread_pool;                               /**< Thread Pool */
};

/**
 * @brief Service class
 */
struct _zwp_server
{
	zwp_pool_t          server_pool;             /**< Memory pool for the Service layer */

	volatile int        server_state;            /**< Service state - ZWP_SERVER_STATE_XXX */

	int                 server_fd;               /**< Descriptor of Server */
	int                 max_fd;                  /**< Maximum file descriptor number for use in select() syscall */
	fd_set              read_fds;                /**< Set of file descriptors for read */

	zwp_thread_t        service_handler;         /**< New connection handler thread*/

	zwp_thread_pool_t   connection_handler_pool; /**< Connection handler thread pool */

	zwp_portal_t        portal;                  /**< Reference to portal object */

	zwp_server_config_t server_config;           /**< Server configuration */
};

/**
 * @name Getters
 * @{
 */
volatile int zwp_server_get_state(zwp_server_t server)
{
	return server->server_state;
}
/**
 * @}
 */

/**
 * @brief Task for worker threads
 * @param[in]     thread            Task thread for workers
 * @param[in]     arg               Thread argument of type @a zwp_client_t
 * @return        NULL
 */
static void *ZWP_THREAD_FUNC zwp_server_handle_connection(zwp_thread_t thread, void *arg)
{
	zwp_client_t client;
	char *rpc_request;
	int rpc_request_length;
	char *rpc_response;
	int rpc_response_length;
	zwp_service_t service;

	client = (zwp_client_t)arg;

	if ((zwp_client_recv(client, &rpc_request, &rpc_request_length) != ZWP_STATUS_OK ) || (rpc_request_length == 0))
	{
		goto l_err_client_recv;
	}

	ZWP_LOG_DUMP(ZWP_LOG_DEBUG, rpc_request, rpc_request_length);

	if (zwp_avi_get_avi_service_from_rpc_request(zwp_client_get_avi(client), rpc_request, rpc_request_length, &service) != ZWP_STATUS_OK)
	{
		goto l_err_avi_get_avi_service;
	}

	if (zwp_portal_execute_avi_message(zwp_client_get_portal(client), service) != ZWP_STATUS_OK)
	{
		goto l_err_portal_execute_message;
	}

	if (zwp_avi_get_rpc_response_from_avi_service(service, &rpc_response, &rpc_response_length) != ZWP_STATUS_OK)
	{
		goto l_err_get_rpc_response;
	}

	ZWP_LOG_DUMP(ZWP_LOG_DEBUG, rpc_response, rpc_response_length);

	if(zwp_client_send(client, rpc_response, rpc_response_length) != ZWP_STATUS_OK)
	{
		goto l_err_client_send;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "SUCCESS");

	zwp_avi_free_rpc_response_from_avi_service(rpc_response);

	zwp_avi_free_avi_service_from_rpc_request(service);

	zwp_client_recv_free(rpc_request);

	close(zwp_client_get_socket_fd(client));

	zwp_client_free(client);

	// Do not call zwp_thread_exit() as this is run within thread pool
	return NULL;

l_err_client_send:
	zwp_avi_free_rpc_response_from_avi_service(rpc_response);
l_err_get_rpc_response:
l_err_portal_execute_message:
	zwp_avi_free_avi_service_from_rpc_request(service);
l_err_avi_get_avi_service:
	zwp_client_recv_free(rpc_request);
l_err_client_recv:
	close(zwp_client_get_socket_fd(client));
	zwp_client_free(client);

	// Do not call zwp_thread_exit() as this is run within thread pool
	return NULL;
}

/**
 * @brief Runs server
 * @param[in]     thread            Task thread for main server
 * @param[in]     arg               Thread argument of type @a zwp_server_t
 * @return        NULL
 */
static void *ZWP_THREAD_FUNC zwp_server_run(zwp_thread_t thread, void *arg)
{
	zwp_server_t server;
	struct timeval timeout;
	fd_set read_fds;
	int count_fds;
	int client_fd;
	struct sockaddr_un client_address;
	socklen_t client_address_length;
	zwp_client_t client;
	char current_thread_name[ZWP_THREAD_NAME_SIZE];
	zwp_status_t return_status;

	server = (zwp_server_t)arg;

	return_status = ZWP_STATUS_OK;

	zwp_thread_name_set(ZWP_SERVER_THREAD_NAME, NULL);

	server->server_state = ZWP_SERVER_STATE_UP;

	client_fd = 0;

	if (zwp_client_init() != ZWP_STATUS_OK)
	{
		goto l_err_client_init;
	}

	while ((server->server_state == ZWP_SERVER_STATE_UP)
			&& (return_status == ZWP_STATUS_OK))
	{
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		while (1)
		{
			memcpy(&read_fds, &server->read_fds, sizeof(read_fds));
			count_fds = select(server->max_fd + 1, &read_fds, NULL, NULL, &timeout);

			if ((count_fds == -1) && (errno == EINTR))
			{
				ZWP_LOG(ZWP_LOG_DEBUG, "select(): %s. Retrying...", strerror(errno));
				continue;
			}
			break;
		}

		if (count_fds == -1)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to select connections: %s", strerror(errno));
			return_status = ZWP_STATUS_ERROR;
		}
		else if (FD_ISSET(server->server_fd, &read_fds))
		{
			client_address_length = sizeof(client_address);
			while (1)
			{
				client_fd = accept(server->server_fd, (struct sockaddr *)&client_address, &client_address_length);

				if ((client_fd == -1) && (errno == EINTR))
				{
					ZWP_LOG(ZWP_LOG_DEBUG, "accept(): %s. Retrying...", strerror(errno));
					continue;
				}
				break;
			}

			if (client_fd == -1)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to accept connections: %s", strerror(errno));
				continue;
			}

			ZWP_LOG(ZWP_LOG_DEBUG, "Accepted new connection: fd: %d", client_fd);

			if (zwp_client_alloc(client_fd, server->portal, &client) != ZWP_STATUS_OK)
			{
				close(client_fd);
				continue;
			}

			zwp_thread_name_set(ZWP_SERVER_WORKER_THREAD_NAME, current_thread_name);

			if (zwp_thread_pool_push(server->connection_handler_pool, zwp_server_handle_connection, client,
					ZWP_THREAD_TASK_PRIORITY_NORMAL, server) != ZWP_STATUS_OK)
			{
				zwp_thread_name_set(current_thread_name, NULL);
				zwp_client_free(client);
				close(client_fd);
				continue;
			}

			zwp_thread_name_set(current_thread_name, NULL);

			zwp_thread_pool_statistics(server->connection_handler_pool);
		}
	}

	// Wait till all tasks finishes
	ZWP_LOG(ZWP_LOG_DEBUG, "Waiting for busy tasks in server thread pool to finish");
	if (zwp_thread_pool_tasks_cancel(server->connection_handler_pool, server) != ZWP_STATUS_OK)
	{
		goto l_err_thread_pool_tasks_cancel;
	}
	ZWP_LOG(ZWP_LOG_DEBUG, "Busy tasks in server thread pool finished");

	/*
	 * At any instant, server thread pool contains tasks that are 'queued' and those that are 'busy'.
	 * zwp_thread_pool_tasks_cancel() waits for 'busy' tasks to finish this releasing their 'client' object.
	 * But, the cancel call releases 'queued' tasks without giving an opportunity to free their 'client' object.
	 * Usually all 'client' objects are registered in zwp_client.c on creation and unregistered on destruction.
	 * The objects associated with 'queued' tasks remain registered at this point of execution and
	 * are free'd by zwp_client_exit()
	 */
	zwp_client_exit();

	server->server_state = ZWP_SERVER_STATE_SHUT_DOWNABLE;
	zwp_thread_exit(thread, return_status);
	return NULL;

l_err_thread_pool_tasks_cancel:
l_err_client_init:
	zwp_client_exit();
	server->server_state = ZWP_SERVER_STATE_SHUT_DOWNABLE;
	ZWP_LOG(ZWP_LOG_EMERG, "Avro server: Failed abruptly");
	zwp_thread_exit(thread, ZWP_STATUS_ERROR);
	return NULL;
}

/**
 * @brief Setup the server
 * @param[in]     server            Server object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_server_setup(zwp_server_t server)
{
	struct sockaddr_un address;
	int server_fd;

	server->server_state = ZWP_SERVER_STATE_STARTING;

	if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create server socket: %s", strerror(errno));
		goto l_err_socket;
	}

	memset(&address, 0, sizeof(address));
	address.sun_family = AF_UNIX;
	snprintf(address.sun_path, sizeof(address.sun_path), ZWP_SERVER_SOCKET_PATH);
	address.sun_path[0] = '\0';

	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == -1)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to bind server socket: %s", strerror(errno));
		goto l_err_bind;
	}

	if (listen(server_fd, SOMAXCONN) == -1)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to listen on server socket: %s", strerror(errno));
		goto l_err_listen;
	}

	server->server_fd = server_fd;
	server->max_fd = 0;
	FD_ZERO(&server->read_fds);
	FD_SET(server->server_fd, &server->read_fds);

	if (server->server_fd > server->max_fd)
	{
		server->max_fd = server->server_fd;
	}

	return ZWP_STATUS_OK;

l_err_listen:
l_err_bind:
	close(server_fd);
	server->server_state = ZWP_SERVER_STATE_DOWN;
l_err_socket:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Close the server
 * @param[in]     server            Server object
 * @return        Nothing
 */
static void zwp_server_close(zwp_server_t server)
{
	FD_ZERO(&server->read_fds);

	server->max_fd = 0;

	close(server->server_fd);

	server->server_state = ZWP_SERVER_STATE_DOWN;
}

zwp_status_t zwp_server_start(zwp_server_t *server_p, zwp_server_config_t server_config, zwp_portal_t portal)
{
	static zwp_server_t g_server;
	zwp_server_t server;
	char current_thread_name[ZWP_THREAD_NAME_SIZE];

	//Enforce singleton
	if (g_server)
	{
		ZWP_LOG(ZWP_LOG_WARNING, ZWP_MSG_SINGLETON_MULTIPLE_CALLS);
		*server_p = g_server;

		return ZWP_STATUS_OK;
	}

	if ((server = (zwp_server_t)ZWP_MALLOC(sizeof(*server))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_server_alloc;
	}

	server->server_state = ZWP_SERVER_STATE_DOWN;

	//Link with configuration
	server->server_config = server_config;

	server->portal = portal;

	//Create memory pool for server layer
	if (zwp_pool_create(&server->server_pool, NULL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create pool for network level");
		goto l_err_server_pool_create;
	}

	if (zwp_server_setup(server) != ZWP_STATUS_OK)
	{
		goto l_err_server_setup;
	}

	zwp_thread_name_set(ZWP_SERVER_WORKER_THREAD_NAME, current_thread_name);

	if (zwp_thread_pool_create(&server->connection_handler_pool,
			server_config->thread_pool.thread_init_count, server_config->thread_pool.thread_max_count,
			server->server_pool) != ZWP_STATUS_OK)
	{
		zwp_thread_name_set(current_thread_name, NULL);
		goto l_err_server_client_thread_pool_create;
	}

	zwp_thread_name_set(current_thread_name, NULL);

	zwp_thread_pool_statistics_detailed(server->connection_handler_pool);

	if (zwp_thread_create(&server->service_handler, zwp_server_run, server,
				server->server_pool, ZWP_THREAD_CREATE_JOINABLE) != ZWP_STATUS_OK)
	{
		goto l_err_server_connection_thread_create;
	}

	g_server = *server_p = server;

	return ZWP_STATUS_OK;

l_err_server_connection_thread_create:
	zwp_thread_pool_destroy(server->connection_handler_pool);
l_err_server_client_thread_pool_create:
	zwp_server_close(server);
l_err_server_setup:
	zwp_pool_destroy(server->server_pool);
l_err_server_pool_create:
	ZWP_FREE(server);
l_err_server_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_server_stop(zwp_server_t server)
{
	zwp_status_t return_status;

	server->server_state = ZWP_SERVER_STATE_SHUTTING_DOWN;

	if (zwp_thread_join(server->service_handler, &return_status) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to join the handler for network operation status");
		goto l_err_thread_join;
	}

	zwp_thread_pool_statistics_detailed(server->connection_handler_pool);

	ZWP_LOG(ZWP_LOG_DEBUG, "Destroying connection handler pool");

	zwp_thread_pool_destroy(server->connection_handler_pool);

	ZWP_LOG(ZWP_LOG_DEBUG, "Connection handler pool destroyed");

	zwp_server_close(server);

	zwp_pool_destroy(server->server_pool);

	ZWP_FREE(server);

	return;

l_err_thread_join:
	return;
}

zwp_status_t zwp_server_config_get_instance(zwp_server_config_t *server_config_p, const config_t *config_parser, const char *config_dir)
{
	static zwp_server_config_t g_server_config;
	zwp_server_config_t server_config;

	//Enforce singleton
	if (g_server_config)
	{
		ZWP_LOG(ZWP_LOG_WARNING, ZWP_MSG_SINGLETON_MULTIPLE_CALLS);
		*server_config_p = g_server_config;

		return ZWP_STATUS_OK;
	}

	if ((server_config = (zwp_server_config_t)ZWP_MALLOC(sizeof(*server_config))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_server_config_alloc;
	}

	//Initial thread count
	server_config->thread_pool.thread_init_count = ZWP_OPTION_DEFAULT_THREAD_INIT_COUNT;
	if (config_lookup_int(config_parser, "server.thread_pool.thread_init_count", &server_config->thread_pool.thread_init_count) != CONFIG_TRUE)
	{
		ZWP_LOG(ZWP_LOG_WARNING, "Failed to lookup server initial thread count. Using default %d", ZWP_OPTION_DEFAULT_THREAD_INIT_COUNT);
	}
	ZWP_LOG(ZWP_LOG_DEBUG, "Server initial thread count: %d", server_config->thread_pool.thread_init_count);

	//Maximum thread count
	server_config->thread_pool.thread_max_count = ZWP_OPTION_DEFAULT_THREAD_MAX_COUNT;
	if (config_lookup_int(config_parser, "server.thread_pool.thread_max_count", &server_config->thread_pool.thread_max_count) != CONFIG_TRUE)
	{
		ZWP_LOG(ZWP_LOG_WARNING, "Failed to lookup server maximum thread count. Using default %d", ZWP_OPTION_DEFAULT_THREAD_MAX_COUNT);
	}
	ZWP_LOG(ZWP_LOG_DEBUG, "Server maximum thread count: %d", server_config->thread_pool.thread_max_count);

	g_server_config = *server_config_p = server_config;

	return ZWP_STATUS_OK;

	ZWP_FREE(server_config);
l_err_server_config_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_server_config_free(zwp_server_config_t server_config)
{
	ZWP_FREE(server_config);
}


