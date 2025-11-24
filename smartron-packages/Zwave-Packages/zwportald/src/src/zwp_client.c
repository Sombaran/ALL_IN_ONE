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
 * @file    zwp_client.c
 *
 * @brief   Portal client
 * @details Module that handles each clients of portal service
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-14
 * - Initial version
 */

#include <stdio.h>
#include <stdint.h>

#include "zwp_log.h"
#if defined(ZWP_DEBUG_AVI_CONNECTION)
#include "zwp_log_enable.h"
#else
#include "zwp_log_disable.h"
#endif

#include "zwp_client.h"
#include "zwp_memory.h"
#include "zwp_error.h"
#include "zw_avro.h"
#include "zwp_ring.h"

/** Default receive block size */
#define ZWP_SERVICE_RECV_DEFAULT_BLOCK_SIZE  1024

/**
 * @brief Portal client class
 */
struct _zwp_client
{
	ZWP_RING_ENTRY(_zwp_client) link;

	int                 socket_fd;               /**< File descriptor of Client */

	int                 max_fd;                  /**< Maximum file descriptor number for use in select() syscall */
	fd_set              read_fds;                /**< Set of file descriptors for read */
	fd_set              write_fds;               /**< Set of file descriptors for write */

	zwp_portal_t        portal;                  /**< Reference to portal object */
};

/** Ring container type */
typedef struct _zwp_client_ring_t *zwp_client_ring_t;

/** Portal client list type */
ZWP_RING_HEAD(_zwp_client_ring_t, _zwp_client);

zwp_pool_t                g_client_pool;         /**< Memory pool for client */

zwp_client_ring_t         g_client_ring;         /**< Head of portal client list*/
zwp_thread_mutex_t        g_client_ring_lock;    /**< Mutex lock for ring of clients */

int zwp_client_get_socket_fd(zwp_client_t client)
{
	return client->socket_fd;
}

zwp_avi_t zwp_client_get_avi(zwp_client_t client)
{
	return zwp_portal_get_avi(client->portal);
}

zwp_portal_t zwp_client_get_portal(zwp_client_t client)
{
	return client->portal;
}

zwp_status_t zwp_client_send(zwp_client_t client, const char *buffer, int length)
{
	struct timeval timeout;
	fd_set write_fds;
	int count_fds;
	const char *send_buffer;
	int sent_bytes;
	int remaining_bytes;

	send_buffer = buffer;
	remaining_bytes = length;

	while (remaining_bytes)
	{
		//Send timeout: 1 sec
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		while (1)
		{
			memcpy(&write_fds, &client->write_fds, sizeof(write_fds));
			count_fds = select(client->max_fd + 1, NULL, &write_fds, NULL, &timeout);

			if ((count_fds == -1) && (errno == EINTR))
			{
				ZWP_LOG(ZWP_LOG_DEBUG, "select(): %s. Retrying...", strerror(errno));
				continue;
			}
			break;
		}

		if (count_fds == -1)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to select connection for write: %s", strerror(errno));
			goto l_err_select;
		}

		if (FD_ISSET(client->socket_fd, &write_fds))
		{
			while (1)
			{
				sent_bytes = send(client->socket_fd, send_buffer, remaining_bytes, 0);

				if ((sent_bytes == -1) && (errno == EINTR))
				{
					ZWP_LOG(ZWP_LOG_DEBUG, "send(): %s. Retrying...", strerror(errno));
					continue;
				}
				break;
			}

			if (sent_bytes == -1)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to send request: %s", strerror(errno));
				goto l_err_send;
			}

			ZWP_LOG(ZWP_LOG_DEBUG, "Sent bytes: %d", sent_bytes);

			send_buffer += sent_bytes;
			remaining_bytes -= sent_bytes;
		}
	}

	return ZWP_STATUS_OK;

l_err_send:
l_err_select:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_client_recv(zwp_client_t client, char **buffer_p, int *length_p)
{
	char *buffer;
	int size;
	char *tmp;
	int bytes;
	struct timeval timeout;
	fd_set read_fds;
	int count_fds;
	int received_bytes;
	uint32_t avro_message_length;

	avro_message_length = -1;

	buffer = NULL;
	size = 0;
	received_bytes = 0;

	while (1)
	{
		size += ZWP_SERVICE_RECV_DEFAULT_BLOCK_SIZE;

		if ((tmp = (char *)ZWP_REALLOC(buffer, size)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_realloc;
		}
		buffer = tmp;

		//TODO: Receive timeout - Make it configurable
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		while (1)
		{
			memcpy(&read_fds, &client->read_fds, sizeof(read_fds));
			count_fds = select(client->max_fd + 1, &read_fds, NULL, NULL, &timeout);

			if ((count_fds == -1) && (errno == EINTR))
			{
				ZWP_LOG(ZWP_LOG_DEBUG, "select(): %s. Retrying...", strerror(errno));
				continue;
			}
			break;
		}

		if (count_fds == -1)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to select connection: %s", strerror(errno));
			goto l_err_select;
		}

		if (FD_ISSET(client->socket_fd, &read_fds))
		{
			while (1)
			{
				bytes = recv(client->socket_fd, buffer + received_bytes, (size - received_bytes), 0);

				if ((bytes == -1) && (errno == EINTR))
				{
					ZWP_LOG(ZWP_LOG_DEBUG, "recv(): %s. Retrying...", strerror(errno));
					continue;
				}
				break;
			}

			if (bytes <= 0)
			{
				if (bytes != 0)
				{
					ZWP_LOG(ZWP_LOG_ERR, "Failed to receive response: %s", strerror(errno));
					goto l_err_recv;
				}

				//(bytes == 0)
				break;
			}

			//ZWP_LOG(ZWP_LOG_DEBUG, "Received: [%d]", bytes);

			// Get the Avro message length to detect reception of complete Avro frame
			if (received_bytes <= 0)
			{
				if (bytes < ZW_AVRO_FRAME_MESSAGE_LENGTH_SIZE)
				{
					ZWP_LOG(ZWP_LOG_ERR, "Invalid Avro message. Message length is tool short: %d", bytes);
					goto l_err_message_too_short;
				}

				memcpy(&avro_message_length, buffer + ZW_AVRO_FRAME_MESSAGE_LENGTH_OFFSET, ZW_AVRO_FRAME_MESSAGE_LENGTH_SIZE);
				// Network byte order to Host byte order
				avro_message_length = ntohl((uint32_t)avro_message_length);

				ZWP_LOG(ZWP_LOG_DEBUG, "Avro message length: %u", avro_message_length);
			}

			received_bytes += bytes;

			if (received_bytes >= avro_message_length + ZW_AVRO_FRAME_MESSAGE_LENGTH_SIZE)
			{
				break;
			}
		}
		else
		{
			//Timeout
			ZWP_LOG(ZWP_LOG_ERR, "Receive  timeout");
			goto l_err_timeout;
		}
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Received bytes: %d", received_bytes);

	*buffer_p = buffer;
	*length_p = received_bytes;

	return ZWP_STATUS_OK;

l_err_timeout:
l_err_message_too_short:
l_err_recv:
l_err_select:
l_err_realloc:
	ZWP_FREE(buffer);
	return ZWP_STATUS_ERROR;
}

void zwp_client_recv_free(char *buffer)
{
	ZWP_FREE(buffer);
}

zwp_status_t zwp_client_alloc(int client_fd, zwp_portal_t portal, zwp_client_t *client_p)
{
	zwp_client_t client;

	if ((client = (zwp_client_t)ZWP_MALLOC(sizeof(*client))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_client_alloc;
	}

	client->socket_fd = client_fd;
	client->max_fd = 0;
	FD_ZERO(&client->read_fds);
	FD_ZERO(&client->write_fds);
	FD_SET(client->socket_fd, &client->read_fds);
	FD_SET(client->socket_fd, &client->write_fds);

	if (client->socket_fd > client->max_fd)
	{
		client->max_fd = client->socket_fd;
	}

	client->portal = portal;

	if (zwp_thread_mutex_lock(g_client_ring_lock) != ZWP_STATUS_OK)
	{
		goto l_err_client_ring_lock;
	}

	ZWP_RING_INSERT_TAIL(g_client_ring, client, _zwp_client, link);

	zwp_thread_mutex_unlock(g_client_ring_lock);

	*client_p = client;

	return ZWP_STATUS_OK;

l_err_client_ring_lock:
	ZWP_FREE(client);
l_err_client_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_client_free(zwp_client_t client)
{
	if (zwp_thread_mutex_lock(g_client_ring_lock) == ZWP_STATUS_OK)
	{
		ZWP_RING_REMOVE(client, link);

		zwp_thread_mutex_unlock(g_client_ring_lock);

		ZWP_FREE(client);
	}
}

zwp_status_t zwp_client_init()
{
	zwp_client_ring_t client_ring;

	//Enforce singleton
	if (g_client_ring)
	{
		ZWP_LOG(ZWP_LOG_WARNING, ZWP_MSG_SINGLETON_MULTIPLE_CALLS);
		return ZWP_STATUS_OK;
	}

	if ((g_client_ring = (zwp_client_ring_t)ZWP_MALLOC(sizeof(*client_ring))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_client_ring_alloc;
	}
	ZWP_RING_INIT(g_client_ring, _zwp_client, link);

	//Create memory pool for clients
	if (zwp_pool_create(&g_client_pool, NULL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create pool for clients");
		goto l_err_client_pool_create;
	}

	//Create mutex lock for client ring
	if (zwp_thread_mutex_create(&g_client_ring_lock, g_client_pool, ZWP_THREAD_MUTEX_CREATE_NORMAL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create lock for client ring");
		goto l_err_client_ring_lock_create;
	}

	return ZWP_STATUS_OK;

	zwp_thread_mutex_destroy(g_client_ring_lock);
l_err_client_ring_lock_create:
	zwp_pool_destroy(g_client_pool);
l_err_client_pool_create:
	ZWP_FREE(g_client_ring); g_client_ring = NULL;
l_err_client_ring_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_client_exit()
{
	zwp_client_t client;
	zwp_client_t previous_client;
	int count = 0;

	if (zwp_thread_mutex_lock(g_client_ring_lock) == ZWP_STATUS_OK)
	{
		client = ZWP_RING_FIRST(g_client_ring);

		while (client != ZWP_RING_SENTINEL(g_client_ring, _zwp_client, link))
		{
			ZWP_RING_UNSPLICE(client, client, link);

			previous_client = client;

			client = ZWP_RING_NEXT(client, link);

			ZWP_FREE(previous_client);
			count++;
		}

		zwp_thread_mutex_unlock(g_client_ring_lock);
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Number of clients free'd on exit : %d", count);

	zwp_thread_mutex_destroy(g_client_ring_lock);
	zwp_pool_destroy(g_client_pool);
	ZWP_FREE(g_client_ring); g_client_ring = NULL;
}


