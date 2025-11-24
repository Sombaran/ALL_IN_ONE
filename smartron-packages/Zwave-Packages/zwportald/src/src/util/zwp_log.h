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
 * @file    zwp_log.h
 *
 * @brief   Logging module
 * @details Handles application logging.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-05-03
 * - Initial version
 */

#ifndef ZWP_LOG_H_
#define ZWP_LOG_H_

#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <libconfig.h>
#include <libgen.h>

#include "zwp_status.h"

/** Log priority: Emergency */
extern int ZWP_LOG_EMERG;
/** Log priority: Alert */
extern int ZWP_LOG_ALERT;
/** Log priority: Critical */
extern int ZWP_LOG_CRIT;
/** Log priority: Error */
extern int ZWP_LOG_ERR;
/** Log priority: Warning */
extern int ZWP_LOG_WARNING;
/** Log priority: Notice */
extern int ZWP_LOG_NOTICE;
/** Log priority: Information */
extern int ZWP_LOG_INFO;
/** Log priority: Function */
extern int ZWP_LOG_FUNCTION;
/** Log priority: Debug */
extern int ZWP_LOG_DEBUG;

/**
 * @brief Logging function for formatted text
 * @param[in]     priority          Log Priority : ZWP_LOG_XXX
 * @param[in]     format            Format
 * @return        Nothing
 */
extern void (*ZWP_LOG_FUNC)(int priority, const char *format, ...);
/**
 * @brief Logging function for dumping a buffer content
 * @param[in]     priority          Log Priority : ZWP_LOG_XXX
 * @param[in]     address           Buffer
 * @param[in]     length            Buffer length
 * @return        Nothing
 */
extern void (*ZWP_LOG_DUMP_FUNC)(int priority, const char *address, const long length);

/** Log priority name: emergency */
#define ZWP_LOG_PRIORITY_NAME_ZWP_LOG_EMERG   "emergency"
/** Log priority name: alert */
#define ZWP_LOG_PRIORITY_NAME_ZWP_LOG_ALERT   "alert"
/** Log priority name: critical */
#define ZWP_LOG_PRIORITY_NAME_ZWP_LOG_CRIT    "critical"
/** Log priority name: error */
#define ZWP_LOG_PRIORITY_NAME_ZWP_LOG_ERR     "error"
/** Log priority name: warning */
#define ZWP_LOG_PRIORITY_NAME_ZWP_LOG_WARNING "warning"
/** Log priority name: notice */
#define ZWP_LOG_PRIORITY_NAME_ZWP_LOG_NOTICE  "notice"
/** Log priority name: information */
#define ZWP_LOG_PRIORITY_NAME_ZWP_LOG_INFO    "information"
/** Log priority name: function */
#define ZWP_LOG_PRIORITY_NAME_ZWP_LOG_FUNCTION    "function"
/** Log priority name: debug */
#define ZWP_LOG_PRIORITY_NAME_ZWP_LOG_DEBUG   "debug"

/** Log flag denoting start of a function */
#define FUNC_LOG_START							1
/** Log flag denoting end of a function */
#define FUNC_LOG_END							2

/** Log priority name builder  */
#define ZWP_LOG_PRIORITY_NAME(priority) ZWP_LOG_PRIORITY_NAME_##priority

/* The list controls the levels enabled and disabled.
 * Optimization: Disabled levels do not generate code for compiler
 */
/** Controls enable/disable of log level 'emergency' */
#define ZWP_LOG_EMERG_IS_ENABLED           (1)
/** Map emergency log function to generic log function */
#define ZWP_LOG_ZWP_LOG_EMERG(...)         ZWP_LOG_FUNC(__VA_ARGS__)
/** Map emergency dump log function to generic dump log function */
#define ZWP_LOG_DUMP_ZWP_LOG_EMERG(...)    ZWP_LOG_DUMP_FUNC(__VA_ARGS__)

/** Controls enable/disable of log level 'alert' */
#define ZWP_LOG_ALERT_IS_ENABLED           (1)
/** Map alert log function to generic log function */
#define ZWP_LOG_ZWP_LOG_ALERT(...)         ZWP_LOG_FUNC(__VA_ARGS__)
/** Map alert dump log function to generic dump log function */
#define ZWP_LOG_DUMP_ZWP_LOG_ALERT(...)    ZWP_LOG_DUMP_FUNC(__VA_ARGS__)

/** Controls enable/disable of log level 'critical' */
#define ZWP_LOG_CRIT_IS_ENABLED            (1)
/** Map critical log function to generic log function */
#define ZWP_LOG_ZWP_LOG_CRIT(...)          ZWP_LOG_FUNC(__VA_ARGS__)
/** Map critical dump log function to generic dump log function */
#define ZWP_LOG_DUMP_ZWP_LOG_CRIT(...)     ZWP_LOG_DUMP_FUNC(__VA_ARGS__)

/** Controls enable/disable of log level 'error' */
#define ZWP_LOG_ERR_IS_ENABLED             (1)
/** Map error log function to generic log function */
#define ZWP_LOG_ZWP_LOG_ERR(...)           ZWP_LOG_FUNC(__VA_ARGS__)
/** Map error dump log function to generic dump log function */
#define ZWP_LOG_DUMP_ZWP_LOG_ERR(...)      ZWP_LOG_DUMP_FUNC(__VA_ARGS__)

/** Controls enable/disable of log level 'warning' */
#define ZWP_LOG_WARNING_IS_ENABLED         (1)
/** Map warning log function to generic log function */
#define ZWP_LOG_ZWP_LOG_WARNING(...)       ZWP_LOG_FUNC(__VA_ARGS__)
/** Map warning dump log function to generic dump log function */
#define ZWP_LOG_DUMP_ZWP_LOG_WARNING(...)  ZWP_LOG_DUMP_FUNC(__VA_ARGS__)

/** Controls enable/disable of log level 'notice' */
#define ZWP_LOG_NOTICE_IS_ENABLED          (1)
/** Map notice log function to generic log function */
#define ZWP_LOG_ZWP_LOG_NOTICE(...)        ZWP_LOG_FUNC(__VA_ARGS__)
/** Map notice dump log function to generic dump log function */
#define ZWP_LOG_DUMP_ZWP_LOG_NOTICE(...)   ZWP_LOG_DUMP_FUNC(__VA_ARGS__)

/** Controls enable/disable of log level 'information' */
#define ZWP_LOG_INFO_IS_ENABLED            (1)
/** Map information log function to generic log function */
#define ZWP_LOG_ZWP_LOG_INFO(...)          ZWP_LOG_FUNC(__VA_ARGS__)
/** Map information dump log function to generic dump log function */
#define ZWP_LOG_DUMP_ZWP_LOG_INFO(...)     ZWP_LOG_DUMP_FUNC(__VA_ARGS__)

/** Controls enable/disable of log level 'function' */
#define ZWP_LOG_FUNCTION_IS_ENABLED            (1)
/** Map function log function to generic log function */
#define ZWP_LOG_ZWP_LOG_FUNCTION(priority, flag, ...)  
//
//{\
	if (flag == FUNC_LOG_START ) {\
		 ZWP_LOG_FUNC(priority, "[%s] [%u:%u] %s:%d %s(): Function started\n", \
			ZWP_LOG_PRIORITY_NAME(priority), getpid(), syscall(SYS_gettid), basename(__FILE__), __LINE__, __func__);\
	} else {\
	ZWP_LOG_FUNC(priority, "[%s] [%u:%u] %s:%d %s(): Function ended with %d\n", \
			ZWP_LOG_PRIORITY_NAME(priority), getpid(), syscall(SYS_gettid), basename(__FILE__), __LINE__, __func__, __VA_ARGS__);\
	}\
}

/** Map function dump log function to generic dump log function */
#define ZWP_LOG_DUMP_ZWP_LOG_FUNCTION(...)   
//(flag == FUNC_LOG_START) ? ZWP_LOG_DUMP_FUNC(priority, "[%s] Function started\n", ZWP_LOG_PRIORITY_NAME(priority)) : ZWP_LOG_DUMP_FUNC(priority, "[%s] Function ended with %s\n", ZWP_LOG_PRIORITY_NAME(priority), __VA_ARGS__)

#ifdef ZWP_DEBUG
/** Controls enable/disable of log level 'debug' */
#define ZWP_LOG_DEBUG_IS_ENABLED           (1)
/** Map debug log function to generic log function */
#define ZWP_LOG_ZWP_LOG_DEBUG(...)         ZWP_LOG_FUNC(__VA_ARGS__)
/** Map debug dump log function to generic dump log function */
#define ZWP_LOG_DUMP_ZWP_LOG_DEBUG(...)    ZWP_LOG_DUMP_FUNC(__VA_ARGS__)

/** Logging helper */

#define ZWP_LOG_HELPER(priority, format, ...) ZWP_LOG_##priority(priority, "[%s] %s:%d %s(): " format "\n%s", \
		ZWP_LOG_PRIORITY_NAME(priority),  basename(__FILE__), __LINE__, __func__, __VA_ARGS__)

#else
/** Controls enable/disable of log level 'debug' */
#define ZWP_LOG_DEBUG_IS_ENABLED           (0)
/** Unmap debug log function */
#define ZWP_LOG_ZWP_LOG_DEBUG(...)
/** Unmap debug dump log function */
#define ZWP_LOG_DUMP_ZWP_LOG_DEBUG(...)

/** Logging helper */
#define ZWP_LOG_HELPER(priority, format, ...) \
	ZWP_LOG_##priority(priority, "[%s] " format "\n%s", ZWP_LOG_PRIORITY_NAME(priority), __VA_ARGS__)

#endif

/** Dump Logging helper */
#define ZWP_LOG_DUMP_HELPER(priority, address, length) \
		{ \
			ZWP_LOG_HELPER(priority, "", ""); \
			ZWP_LOG_DUMP_##priority(priority, address, length); \
		}

/**
 * @brief Logging a formatted text
 *
 * @par
 * First argument is the priority ZWP_LOG_EMERG to ZWP_LOG_DEBUG
 *
 * @par
 * Second and subsequent arguments are the format string followed by items to be passed to it
 *
 * @return        Nothing
 */
#define ZWP_LOG(...) ZWP_LOG_HELPER(__VA_ARGS__, "")

#define ZWP_LOG_EX(priority, flag, ...) ZWP_LOG_##priority(priority, flag, __VA_ARGS__)

/**
 * @brief Logging a buffer content in hexdump format
 */
#define ZWP_LOG_DUMP(priority, address, length) ZWP_LOG_DUMP_HELPER(priority, address, length)

/** Forward declaration for log configuration class */
struct _zwp_log_config;
/** Log configuration type */
typedef struct _zwp_log_config *zwp_log_config_t;

/** Forward declaration for logger class */
struct _zwp_logger;
/** Logger type */
typedef struct _zwp_logger *zwp_logger_t;

/**
 * @brief Start logging
 * @param[out]    logger_p          Logger object
 * @param[in]     log_config        Log configuration
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @post @a logger_p refers to a singleton \link _zwp_logger object \endlink
 * allocated/returned by this function (constructor)
 *
 * @note
 * @par
 * The caller should call zwp_logger_stop() to stop the logger and free resources
 *
 * @par
 * This function is not thread safe
 */
zwp_status_t zwp_logger_start(zwp_logger_t *logger_p, const zwp_log_config_t log_config);

/**
 * @brief Stop and free logger
 * @param[in]     logger            Application configuration object
 * @return        Nothing
 * @see zwp_logger_start
 */
void zwp_logger_stop(zwp_logger_t logger);

/**
 * @brief Get log configuration
 * @param[out]    log_config_p      Log configuration object
 * @param[in]     config_parser     Configuration parser handle
 * @param[in]     config_dir        Configuration directory path to convert path configurations from relative to absolute
 * @param[in]     identity          Identity string to be used as log entry prefix
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @post @a log_config_p refers to a singleton \link _zwp_log_config object \endlink
 * allocated/returned by this function (constructor)
 *
 * @note
 * @par
 * The caller should call zwp_log_config_free() to free resources
 *
 * @par
 * This function is not thread safe
 */
zwp_status_t zwp_log_config_get_instance(zwp_log_config_t *log_config_p,
		const config_t *config_parser, const char *config_dir, const char *identity);

/**
 * @brief Free log configuration
 * @param[in]     log_config        Application configuration object
 * @return        Nothing
 * @see zwp_log_config_get_instance
 */
void zwp_log_config_free(zwp_log_config_t log_config);

#endif /* ZWP_LOG_H_ */
