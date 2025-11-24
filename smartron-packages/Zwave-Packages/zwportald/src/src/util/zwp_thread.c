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
 * @file    zwp_thread.c
 *
 * @brief   Threads
 * @details Handling threads for concurrency
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-05-03
 * - Initial version
 */

#include <sys/prctl.h>

#include "zwp_log.h"
#include "zwp_thread.h"

/*
 * @note Size of 'current_name' should be at least ZWP_THREAD_NAME_SIZE
 * @note   If getting current name fails, 'current_name' will be set to empty string "" and the call will fail.
 */
zwp_status_t zwp_thread_name_get(char *current_name)
{
	if (prctl(PR_GET_NAME, current_name) == -1)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get name for current thread: %s", strerror(errno));
		current_name[0] = '\0';
		goto l_err_get_name;
	}

	return ZWP_STATUS_OK;

l_err_get_name:
	return ZWP_STATUS_ERROR;
}

/**
 * @param[in]  new_name       New name for the thread
 * @param[out] current_name   Current name of the thread. If NULL, current_name is not fetched.
 *
 * @note   Size of 'current_name' should be at least ZWP_THREAD_NAME_SIZE
 * @note   If getting current name fails, 'current_name' will be set to empty string "" and the call will fail.
 * @note   If 'new_name' is empty string "", the new name will not be set and the call will fail.
 */
zwp_status_t zwp_thread_name_set(const char *new_name, char *current_name)
{
	if (current_name && (zwp_thread_name_get(current_name) != ZWP_STATUS_OK))
	{
		goto l_err_get_name;
	}

	if (!new_name || (new_name[0] == '\0') || (prctl(PR_SET_NAME, new_name) == -1))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to set name for current thread: %s", strerror(errno));
		goto l_err_set_name;
	}

	return ZWP_STATUS_OK;

l_err_set_name:
l_err_get_name:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_thread_join(zwp_thread_t thread, zwp_status_t *return_status_p)
{
	apr_status_t return_status;
	int apr_code;
	char strerror_buffer[ZWP_ERROR_STRING_BUFFER_MAX_SIZE];

	if ((apr_code = apr_thread_join(&return_status, thread)) != APR_SUCCESS)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to join with thread: %s", apr_strerror(apr_code, strerror_buffer, sizeof(strerror_buffer)));
		goto l_err_thread_join;
	}

	if (return_status_p)
	{
		*return_status_p = return_status;
	}

	return ZWP_STATUS_OK;

l_err_thread_join:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_thread_detach(zwp_thread_t thread, zwp_status_t *return_status_p)
{
	apr_status_t return_status;

	if ((return_status = apr_thread_detach(thread)) != APR_SUCCESS)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to detach with thread. Status code: %d", return_status);
		goto l_err_thread_detach;
	}

	if (return_status_p)
	{
		*return_status_p = return_status;
	}

	return ZWP_STATUS_OK;

l_err_thread_detach:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_thread_create(zwp_thread_t *thread_p, zwp_thread_start_t function, void *argument, zwp_pool_t pool, int detached)
{
	apr_threadattr_t *attr;
	apr_thread_t *thread;
	apr_pool_t *apr_pool;
	int apr_code;
	char strerror_buffer[ZWP_ERROR_STRING_BUFFER_MAX_SIZE];

	apr_pool = pool;

	if ((apr_code = apr_threadattr_create(&attr, apr_pool)) != APR_SUCCESS)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create thread attribute: %s", apr_strerror(apr_code, strerror_buffer, sizeof(strerror_buffer)));
		goto l_err_threadattr_create;
	}

	if ((apr_code = apr_threadattr_detach_set(attr,
			((detached == ZWP_THREAD_CREATE_JOINABLE) ? 0 : 1))) != APR_SUCCESS)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to set thread attribute for joinable/detached: %s",
				apr_strerror(apr_code, strerror_buffer, sizeof(strerror_buffer)));
		goto l_err_threadattr_detach_set;
	}

	if ((apr_code = apr_thread_create(&thread, attr, function, argument, apr_pool)) != APR_SUCCESS)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create thread: %s", apr_strerror(apr_code, strerror_buffer, sizeof(strerror_buffer)));
		goto l_err_thread_create;
	}

	if (thread_p)
	{
		*thread_p = thread;
	}

	return ZWP_STATUS_OK;

l_err_thread_create:
l_err_threadattr_detach_set:
l_err_threadattr_create:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_thread_exit(zwp_thread_t thread, zwp_status_t status)
{
	int apr_code;
	char strerror_buffer[ZWP_ERROR_STRING_BUFFER_MAX_SIZE];

	if ((apr_code = apr_thread_exit(thread, status)) != APR_SUCCESS)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to stop thread: %s", apr_strerror(apr_code, strerror_buffer, sizeof(strerror_buffer)));
		goto l_err_thread_stop;
	}

	return ZWP_STATUS_OK;

l_err_thread_stop:
	return ZWP_STATUS_ERROR;
}

