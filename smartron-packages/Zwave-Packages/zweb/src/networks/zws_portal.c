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
 * @file    zws_portal.c
 *
 * @brief   Connection to Portal daemon.
 * @details Handles connection to Z-Ware portal daemon.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-02
 * - Initial version
 */

#ifdef ZWS_WITH_FCGI
#include <fcgi_stdio.h>
#else
#include <stdio.h>
#endif

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stddef.h>

#include "zws_log.h"
#if defined(ZWS_DEBUG_AVI_CONNECTION)
#include "zws_log_enable.h"
#else
#include "zws_log_disable.h"
#endif

#include "zws_portal.h"
#include "zws_error.h"
#include "zws_memory.h"
#include "zwu_number.h"

/** Unix domain socket path */
#define ZWS_SERVICE_SOCKET_PATH "#zwportald_socket"

/** Default receive timeout for messages from portal server */
#define ZWS_PORTAL_RECV_DEFAULT_BLOCK_SIZE  1024

/** Default portal receive timeout in seconds */
#define ZWS_PORTAL_RECEIVE_TIMEOUT_DEFAULT  15

/**
 * @brief Portal class
 */
struct _zws_portal
{
	int                 socket_fd;               /**< Socket descriptor of this client connecting to portal  */
	int                 max_fd;                  /**< Maximum file descriptor number for use in select() syscall */
	fd_set              read_fds;                /**< Set of file descriptors for read */
	fd_set              write_fds;               /**< Set of file descriptors for write */

	int                 receive_timeout;         /**< Receive timeout in connection to portal */
};

/** Portal singleton object */
static zws_portal_t g_portal;

void zws_portal_get_receive_timeout(int *receive_timeout_p)
{
	char *receive_timeout_string;
	int32_t value;

	// Get Portal receive timeout from ZWARE_PORTAL_RECEIVE_TIMEOUT env variable
	if (((receive_timeout_string = getenv("ZWARE_PORTAL_RECEIVE_TIMEOUT")) != NULL)
			&& (zwu_parse_int32_non_negative(receive_timeout_string, &value) == ZWU_STATUS_OK))
	{
		*receive_timeout_p = value;

		ZWS_LOG(ZWS_LOG_DEBUG, "Portal receive timeout: %d seconds",
				*receive_timeout_p);
	}
	else
	{
		*receive_timeout_p = ZWS_PORTAL_RECEIVE_TIMEOUT_DEFAULT;

		ZWS_LOG(ZWS_LOG_WARNING, "Failed to get portal receive timeout. Using default: %d seconds",
				ZWS_PORTAL_RECEIVE_TIMEOUT_DEFAULT);
	}
}

/**
 * @brief Connect to portal server
 * @param[in]     portal            Portal object
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_portal_connect(zws_portal_t portal)
{
	struct sockaddr_un address;
	int socket_fd;
	int status;

	if ((socket_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to create socket: %s", strerror(errno));
		goto l_err_socket;
	}

	memset(&address, 0, sizeof(address));
	address.sun_family = AF_UNIX;
	snprintf(address.sun_path, sizeof(address.sun_path), ZWS_SERVICE_SOCKET_PATH);
	address.sun_path[0] = '\0';

	while (1)
	{
		status = connect(socket_fd, (struct sockaddr*)&address, sizeof(address));

		if ((status == -1) && (errno == EINTR))
		{
			ZWS_LOG(ZWS_LOG_DEBUG, "connect(): %s. Retrying...", strerror(errno));
			continue;
		}
		break;
	}

	if (status == -1)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to connect to portal: %s", strerror(errno));
		goto l_err_bind;
	}

	portal->socket_fd = socket_fd;
	portal->max_fd = 0;
	FD_ZERO(&portal->read_fds);
	FD_ZERO(&portal->write_fds);
	FD_SET(portal->socket_fd, &portal->read_fds);
	FD_SET(portal->socket_fd, &portal->write_fds);

	if (portal->socket_fd > portal->max_fd)
	{
		portal->max_fd = portal->socket_fd;
	}

	return ZWS_STATUS_OK;

l_err_bind:
	close(socket_fd);
l_err_socket:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Disconnect from portal server
 * @param[in]     portal            Portal object
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static void zws_portal_close(zws_portal_t portal)
{
	FD_ZERO(&portal->read_fds);
	FD_ZERO(&portal->write_fds);

	portal->max_fd = 0;

	close(portal->socket_fd);
}

zws_status_t zws_portal_send(zws_portal_t portal, const char *buffer, int length)
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
			memcpy(&write_fds, &portal->write_fds, sizeof(write_fds));
			count_fds = select(portal->max_fd + 1, NULL, &write_fds, NULL, &timeout);

			if ((count_fds == -1) && (errno == EINTR))
			{
				ZWS_LOG(ZWS_LOG_DEBUG, "select(): %s. Retrying...", strerror(errno));
				continue;
			}
			break;
		}

		if (count_fds == -1)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to select connection for write: %s", strerror(errno));
			goto l_err_select;
		}

		if (FD_ISSET(portal->socket_fd, &write_fds))
		{
			while (1)
			{
				sent_bytes = send(portal->socket_fd, send_buffer, remaining_bytes, 0);

				if ((sent_bytes == -1) && (errno == EINTR))
				{
					ZWS_LOG(ZWS_LOG_DEBUG, "send(): %s. Retrying...", strerror(errno));
					continue;
				}
				break;
			}

			if (sent_bytes == -1)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Failed to send request: %s", strerror(errno));
				goto l_err_send;
			}

			ZWS_LOG(ZWS_LOG_DEBUG, "Sent bytes: %d", sent_bytes);

			send_buffer += sent_bytes;
			remaining_bytes -= sent_bytes;
		}
	}

	return ZWS_STATUS_OK;

l_err_send:
l_err_select:
	return ZWS_STATUS_ERROR;
}

zws_status_t zws_portal_recv(zws_portal_t portal, char **buffer_p, int *length_p)
{
	char *buffer;
	int size;
	char *tmp;
	int bytes;
	struct timeval timeout;
	fd_set read_fds;
	int count_fds;
	int received_bytes;

	buffer = NULL;
	size = 0;
	received_bytes = 0;

	while (1)
	{
		size += ZWS_PORTAL_RECV_DEFAULT_BLOCK_SIZE;

		if ((tmp = (char *)ZWS_REALLOC(buffer, size)) == NULL)
		{
			ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
			goto l_err_realloc;
		}
		buffer = tmp;

		timeout.tv_sec = portal->receive_timeout;
		timeout.tv_usec = 0;

		while (1)
		{
			memcpy(&read_fds, &portal->read_fds, sizeof(read_fds));
			count_fds = select(portal->max_fd + 1, &read_fds, NULL, NULL, &timeout);

			if ((count_fds == -1) && (errno == EINTR))
			{
				ZWS_LOG(ZWS_LOG_DEBUG, "select(): %s. Retrying...", strerror(errno));
				continue;
			}
			break;
		}

		if (count_fds == -1)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to select connection: %s", strerror(errno));
			goto l_err_select;
		}

		if (FD_ISSET(portal->socket_fd, &read_fds))
		{
			while (1)
			{
				bytes = recv(portal->socket_fd, buffer + received_bytes, (size - received_bytes), 0);

				if ((bytes == -1) && (errno == EINTR))
				{
					ZWS_LOG(ZWS_LOG_DEBUG, "recv(): %s. Retrying...", strerror(errno));
					continue;
				}
				break;
			}

			if (bytes <= 0)
			{
				if (bytes != 0)
				{
					ZWS_LOG(ZWS_LOG_ERR, "Failed to receive response: %s", strerror(errno));
					goto l_err_recv;
				}

				//(bytes == 0)
				break;
			}

			received_bytes += bytes;
		}
		else
		{
			//Timeout
			ZWS_LOG(ZWS_LOG_ERR, "Receive  timeout after %d seconds", portal->receive_timeout);
			goto l_err_timeout;
		}
	}

	ZWS_LOG(ZWS_LOG_DEBUG, "Received bytes: %d", received_bytes);

	ZWS_LOG_DUMP(ZWS_LOG_DEBUG, buffer, received_bytes);

	*buffer_p = buffer;
	*length_p = received_bytes;

	return ZWS_STATUS_OK;

l_err_timeout:
l_err_recv:
l_err_select:
l_err_realloc:
	ZWS_FREE(buffer);
	return ZWS_STATUS_ERROR;
}

void zws_portal_recv_free(char *buffer)
{
	ZWS_FREE(buffer);
}

zws_status_t zws_portal_init(zws_portal_t *portal_p, int receive_timeout)
{
	zws_portal_t portal;

	//Enforce singleton
	if (g_portal)
	{
		ZWS_LOG(ZWS_LOG_WARNING, ZWS_MSG_SINGLETON_MULTIPLE_CALLS);
		*portal_p = g_portal;

		return ZWS_STATUS_OK;
	}

	if ((portal = (zws_portal_t)ZWS_MALLOC(sizeof(*portal))) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_portal_alloc;
	}

	portal->receive_timeout = receive_timeout;

	if (zws_portal_connect(portal) != ZWS_STATUS_OK)
	{
		goto l_err_portal_connect;
	}

	g_portal = *portal_p = portal;

	return ZWS_STATUS_OK;

l_err_portal_connect:
	ZWS_FREE(portal);
l_err_portal_alloc:
	return ZWS_STATUS_ERROR;
}

void zws_portal_exit(zws_portal_t portal)
{
	zws_portal_close(portal);
	ZWS_FREE(portal);
	g_portal = NULL;
}
