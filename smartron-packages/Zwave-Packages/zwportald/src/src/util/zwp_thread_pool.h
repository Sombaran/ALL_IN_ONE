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
 * @file    zwp_thread_pool.h
 *
 * @brief   Thread Pool
 * @details Handling thread pool for concurrency
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-09-20
 * - Initial version
 */

#ifndef ZWP_THREAD_POOL_H_
#define ZWP_THREAD_POOL_H_

#include <apr_thread_pool.h>
#include "zwp_status.h"
#include "zwp_apr.h"
#include "zwp_thread.h"
#include "zwp_time.h"

/** Task priority level : Lowest */
#define ZWP_THREAD_TASK_PRIORITY_LOWEST     APR_THREAD_TASK_PRIORITY_LOWEST
/** Task priority level : Low */
#define ZWP_THREAD_TASK_PRIORITY_LOW        APR_THREAD_TASK_PRIORITY_LOW
/** Task priority level : Normal */
#define ZWP_THREAD_TASK_PRIORITY_NORMAL 	APR_THREAD_TASK_PRIORITY_NORMAL
/** Task priority level : High */
#define ZWP_THREAD_TASK_PRIORITY_HIGH 	    APR_THREAD_TASK_PRIORITY_HIGH
/** Task priority level : Highest */
#define ZWP_THREAD_TASK_PRIORITY_HIGHEST 	APR_THREAD_TASK_PRIORITY_HIGHEST

/** Thread pool class */
typedef apr_thread_pool_t * zwp_thread_pool_t;

/** Get the current number of tasks waiting in the queue */
#define zwp_thread_pool_tasks_count                  apr_thread_pool_tasks_count
/** Get the current number of scheduled tasks waiting in the queue  */
#define zwp_thread_pool_scheduled_tasks_count        apr_thread_pool_scheduled_tasks_count
/**  Get number of tasks that have run */
#define zwp_thread_pool_tasks_run_count              apr_thread_pool_tasks_run_count
/** Get high water mark of the number of tasks */
#define zwp_thread_pool_tasks_high_count             apr_thread_pool_tasks_high_count
/** Access function for the threshold of tasks in queue to trigger a new thread */
#define zwp_thread_pool_threshold_get                apr_thread_pool_threshold_get

/** Get the current number of threads  */
#define zwp_thread_pool_threads_count                apr_thread_pool_threads_count
/** Get the current number of busy threads */
#define zwp_thread_pool_busy_count                   apr_thread_pool_busy_count
/** Get the current number of idle threads */
#define zwp_thread_pool_idle_count                   apr_thread_pool_idle_count
/** Get high water mark of the number of threads */
#define zwp_thread_pool_threads_high_count           apr_thread_pool_threads_high_count
/** Access function for the maximum wait time (in microseconds) of an idling thread that exceeds the maximum number of idling threads  */
#define zwp_thread_pool_idle_wait_get                apr_thread_pool_idle_wait_get
/** Get the number of idle threads that were destroyed after timing out  */
#define zwp_thread_pool_threads_idle_timeout_count   apr_thread_pool_threads_idle_timeout_count
/** Access function for the maximum number of idle threads  */
#define zwp_thread_pool_idle_max_get                 apr_thread_pool_idle_max_get
/** Access function for the maximum number of threads  */
#define zwp_thread_pool_thread_max_get               apr_thread_pool_thread_max_get

/**
 * @brief Log detailed thread pool statistics
 * @param[in]     thread_pool       Thread pool object
 * @return        Nothing
 */
void zwp_thread_pool_statistics_detailed(zwp_thread_pool_t thread_pool);

/**
 * @brief Log thread pool statistics summary
 * @param[in]     thread_pool       Thread pool object
 * @return        Nothing
 */
void zwp_thread_pool_statistics(zwp_thread_pool_t thread_pool);

/**
 * @brief Schedule a task to the bottom of the tasks of same priority.
 * @param[in]     thread_pool       Thread pool object
 * @param[in]     func              The task function
 * @param[in]     param             The parameter for the task function
 * @param[in]     priority          The priority of the task.
 * @param[in]     owner             Owner of this task
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_thread_pool_push(zwp_thread_pool_t thread_pool, zwp_thread_start_t func, void *param, unsigned char priority, void *owner);

/**
 * @brief Cancel tasks submitted by the owner. If there is any task from the owner that is currently running, the function will spin until the task finished
 * @param[in]     thread_pool       Thread pool object
 * @param[in]     owner             Owner of this task
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_thread_pool_tasks_cancel(zwp_thread_pool_t thread_pool, void *owner);

/**
 * @brief Create a thread pool
 * @param[out]    thread_pool_p     The new thread pool object
 * @param[in]     init_threads      The number of threads to be created initially, this number will also be used as the initial value for the maximum number of idle threads
 * @param[in]     max_threads       The maximum number of threads that can be created
 * @param[in]     pool              The memory pool to use
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_thread_pool_create(zwp_thread_pool_t *thread_pool_p, size_t init_threads, size_t max_threads, zwp_pool_t pool);

/**
 * @brief Destroy the thread pool and stop all the threads
 * @param[in]     thread_pool       Thread pool object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_thread_pool_destroy(zwp_thread_pool_t thread_pool);

#endif /* ZWP_THREAD_POOL_H_ */
