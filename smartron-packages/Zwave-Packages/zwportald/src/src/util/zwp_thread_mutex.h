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
 * @file    zwp_thread_mutex.c
 *
 * @brief   Mutex locks for threads
 * @details Mutual exclusion for threads using Mutex locks.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-06-28
 * - Initial version
 */

#ifndef ZWP_THREAD_MUTEX_H_
#define ZWP_THREAD_MUTEX_H_

#include <apr_thread_mutex.h>
#include "zwp_apr.h"
#include "zwp_status.h"

/** Normal thread mutex */
#define ZWP_THREAD_MUTEX_CREATE_NORMAL         0
/** Recursive thread mutex */
#define ZWP_THREAD_MUTEX_CREATE_RECURSIVE      1

/** Thread mutex class */
typedef apr_thread_mutex_t * zwp_thread_mutex_t;

/**
 * Acquire the lock for the given mutex. If the mutex is already locked,
 * the current thread will be put to sleep until the lock becomes available.
 * @param         mutex             The mutex on which to acquire the lock
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_thread_mutex_lock_inner(zwp_thread_mutex_t mutex);

/**
 * Attempt to acquire the lock for the given mutex. If the mutex has already
 * been acquired, the call returns immediately with ZWP_STATUS_EBUSY.
 * @param         mutex             The mutex on which to attempt the lock acquiring
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 * @retval        ZWP_STATUS_EBUSY  Busy
 */
zwp_status_t zwp_thread_mutex_trylock_inner(zwp_thread_mutex_t mutex);

/**
 * Release the lock for the given mutex.
 * @param         mutex             The mutex from which to release the lock
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_thread_mutex_unlock_inner(zwp_thread_mutex_t mutex);

#if defined(ZWP_DEBUG_THREAD_MUTEX) && ZWP_LOG_DEBUG_IS_ENABLED
#define zwp_thread_mutex_lock(mutex) \
	({ \
		zwp_status_t mutex_status_debug; \
		ZWP_LOG(ZWP_LOG_DEBUG, "Mutex: Trying to acquire lock"); \
		mutex_status_debug = zwp_thread_mutex_lock_inner(mutex); \
		ZWP_LOG(ZWP_LOG_DEBUG, "Mutex: Lock acquired"); \
		mutex_status_debug; \
	})

#define zwp_thread_mutex_trylock(mutex) \
	({ \
		zwp_status_t mutex_status_debug; \
		ZWP_LOG(ZWP_LOG_DEBUG, "Mutex: Trying to acquire trylock"); \
		mutex_status_debug = zwp_thread_mutex_trylock_inner(mutex); \
		ZWP_LOG(ZWP_LOG_DEBUG, "Mutex: Lock acquired"); \
		mutex_status_debug; \
	})

#define zwp_thread_mutex_unlock(mutex) \
	({ \
		zwp_status_t mutex_status_debug; \
		mutex_status_debug = zwp_thread_mutex_unlock_inner(mutex); \
		ZWP_LOG(ZWP_LOG_DEBUG, "Mutex: Lock released"); \
		mutex_status_debug; \
	})
#else
/**
 * Acquire the lock for the given mutex. If the mutex is already locked,
 * the current thread will be put to sleep until the lock becomes available.
 * @param         mutex             The mutex on which to acquire the lock
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
#define zwp_thread_mutex_lock zwp_thread_mutex_lock_inner

/**
 * Attempt to acquire the lock for the given mutex. If the mutex has already
 * been acquired, the call returns immediately with ZWP_STATUS_EBUSY.
 * @param         mutex             The mutex on which to attempt the lock acquiring
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 * @retval        ZWP_STATUS_EBUSY  Busy
 */
#define zwp_thread_mutex_trylock zwp_thread_mutex_trylock_inner

/**
 * Release the lock for the given mutex.
 * @param         mutex             The mutex from which to release the lock
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
#define zwp_thread_mutex_unlock zwp_thread_mutex_unlock_inner
#endif

/**
 * Create and initialize a mutex that can be used to synchronize threads.
 * @param         mutex_p           The memory address where the newly created mutex will be stored.
 * @param         pool              The pool from which to allocate the mutex.
 * @param         recursive         @a ZWP_THREAD_MUTEX_CREATE_NORMAL or @a ZWP_THREAD_MUTEX_CREATE_RECURSIVE
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_thread_mutex_create(zwp_thread_mutex_t *mutex_p, zwp_pool_t pool, int recursive);

/**
 * Destroy the mutex and free the memory associated with the lock.
 * @param         mutex             The mutex to destroy
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_thread_mutex_destroy(zwp_thread_mutex_t mutex);

#endif /* ZWP_THREAD_MUTEX_H_ */
