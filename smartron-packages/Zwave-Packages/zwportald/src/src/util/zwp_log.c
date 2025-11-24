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
 * @file    zwp_log.c
 *
 * @brief   Logging module
 * @details Handles application logging.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-05-03
 * - Initial version
 */

#include <stdarg.h>
#include <stdio.h>
#include <syslog.h>
#include <ctype.h>
#include <stdint.h>

#include "zwp_log.h"
#include "zwp_memory.h"
#include "zwp_util_config.h"
#include "zwp_error.h"

/** List of log targets */
typedef enum
{
	ZWP_LOG_TARGET_CONSOLE = 0,        /**< Console */
	ZWP_LOG_TARGET_SYSLOG = 1          /**< Syslog */
} zwp_log_target_t;

/**
 * @brief Log configuration class
 */
struct _zwp_log_config
{
	char                *identity;               /**< Program name used in the log */
	int                 mask;                    /**< Mask of enabled log priorities. Mask of ZWP_LOG_XXX */
	zwp_log_target_t    target;                  /**< Log target */
	union
	{
		struct {
			int       option;                    /**< Option flags set for openlog() system call */
			int       facility;                  /**< Type of program that is logging. See openlog() system call */
		} syslog;                                /**< syslog specific configurations */
	} target_config;                             /**< Target specific configurations */
};

/**
 * @brief Logger class
 */
struct _zwp_logger
{
	zwp_log_target_t    target;                  /**< Log target. Eg: "syslogd" */
};

/** Default log level */
#define ZWP_OPTION_DEFAULT_UPTO_LEVEL "debug"

/** Default log target */
#define ZWP_OPTION_DEFAULT_TARGET "console"

/** Log 'upto' mask */
#define ZWP_LOG_LEVEL_UPTO LOG_UPTO
/** Log bit mask */
#define ZWP_LOG_LEVEL_MASK LOG_MASK

#define LOG_FUNCTION	LOG_DEBUG

/** Log priority: Emergency */
int ZWP_LOG_EMERG = LOG_EMERG;
/** Log priority: Alert */
int ZWP_LOG_ALERT = LOG_ALERT;
/** Log priority: Critical */
int ZWP_LOG_CRIT = LOG_CRIT;
/** Log priority: Error */
int ZWP_LOG_ERR = LOG_ERR;
/** Log priority: Warning */
int ZWP_LOG_WARNING = LOG_WARNING;
/** Log priority: Notice */
int ZWP_LOG_NOTICE = LOG_NOTICE;
/** Log priority: Information */
int ZWP_LOG_INFO = LOG_INFO;
/** Log priority: Function */
int ZWP_LOG_FUNCTION = LOG_FUNCTION;
/** Log priority: Debug */
int ZWP_LOG_DEBUG = LOG_DEBUG;

/** Global mask of enabled log priorities. Mask of ZWP_LOG_XXX */
int ZWP_LOG_MASK;

/* Logging function prototypes */
static void zwp_log_func_console(int priority, const char *format, ...);
static void zwp_log_func_syslog(int priority, const char *format, ...);
static void zwp_log_dump_func_console(int priority, const char *address, const long length);
static void zwp_log_dump_func_syslog(int priority, const char *address, const long length);

/** Logging function */
void (*ZWP_LOG_FUNC)(int priority, const char *format, ...) = zwp_log_func_console;
void (*ZWP_LOG_DUMP_FUNC)(int priority, const char *address, const long length) = zwp_log_dump_func_console;

/** List of log level options */
static struct
{
	const char *option;
	int *level;
} zwp_log_level_options[] =
	{
		{"emergency", &ZWP_LOG_EMERG},
		{"alert", &ZWP_LOG_ALERT},
		{"critical", &ZWP_LOG_CRIT},
		{"error", &ZWP_LOG_ERR},
		{"warning", &ZWP_LOG_WARNING},
		{"notice", &ZWP_LOG_NOTICE},
		{"information", &ZWP_LOG_INFO},
		{"function", &ZWP_LOG_FUNCTION},
		{"debug", &ZWP_LOG_DEBUG}
	};

/** List of log target options */
static struct
{
	const char *option;
	zwp_log_target_t target;
} zwp_log_target_options[] =
	{
		{"console", ZWP_LOG_TARGET_CONSOLE},
		{"syslog", ZWP_LOG_TARGET_SYSLOG}
	};

static void zwp_log_option_set_mask(const char *option_upto_level, int *mask);
static void zwp_log_option_set_target(const char *option_target, zwp_log_target_t *target);

/**
 * @brief Log messages to the console
 * @param[in]     priority          Log priority : ZWP_LOG_EMERG to ZWP_LOG_DEBUG
 * @param[in]     format            Format string followed by items to be passed to it
 * @return        Nothing
 *
 * @note "console" is the default log target.
 * This target is used even before zwp_logger_start() is used
 */
static void zwp_log_func_console(int priority, const char *format, ...)
{
	va_list ap;

	//Set mask to default, if logger is not started
	if (!ZWP_LOG_MASK)
	{
		zwp_log_option_set_mask(ZWP_OPTION_DEFAULT_UPTO_LEVEL, &ZWP_LOG_MASK);
	}

	if(!(ZWP_LOG_LEVEL_MASK(priority) & ZWP_LOG_MASK))
	{
		return;
	}

	va_start(ap, format);
	if (priority <= ZWP_LOG_ERR)
	{
		vfprintf(stderr, format, ap);
		fflush(stderr);
	}
	else
	{
		vfprintf(stdout, format, ap);
		fflush(stdout);
	}
	va_end(ap);
}

/**
 * @brief Log messages to the system logger
 * @param[in]     priority          Log priority : ZWP_LOG_EMERG to ZWP_LOG_DEBUG
 * @param[in]     format            Format string followed by items to be passed to it
 * @return        Nothing
 */
static void zwp_log_func_syslog(int priority, const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vsyslog(priority, format, ap);
	va_end(ap);
}

/**
 * @brief Logging function for dumping a line of buffer content in console
 * @param[in]     stream            FILE stream
 * @param[in]     address           Buffer
 * @param[in]     length            Buffer length
 * @return        Nothing
 */
static void zwp_log_dump_line_console(FILE * stream, const char *address, const long length)
{
	int i;
	char c;

	fprintf(stream, "|");

	for (i = 0; i < 16; i++)
	{
		if (i < length)
		{
			fprintf(stream, " %02X", ((uint8_t *) address)[i]);
		}
		else
		{
			fprintf(stream, " ..");
		}

		if (!((i + 1) % 8))
		{
			fprintf(stream, " |");
		}
	}

	fprintf(stream, " ");

	for (i = 0; i < 16; i++)
	{
		c = 0x7f & ((uint8_t *) address)[i];

		if (i < length && isprint(c))
		{
			fprintf(stream, "%c", c);
		}
		else
		{
			fprintf(stream, ".");
		}
	}
}

/**
 * @brief Logging function for dumping a buffer content in console
 * @param[in]     priority          Log Priority : ZWP_LOG_XXX
 * @param[in]     address           Buffer
 * @param[in]     length            Buffer length
 * @return        Nothing
 */
static void zwp_log_dump_func_console(int priority, const char *address, const long length)
{
	FILE *stream;
	int i;

	//Set mask to default, if logger is not started
	if (!ZWP_LOG_MASK)
	{
		zwp_log_option_set_mask(ZWP_OPTION_DEFAULT_UPTO_LEVEL, &ZWP_LOG_MASK);
	}

	if(!(ZWP_LOG_LEVEL_MASK(priority) & ZWP_LOG_MASK))
	{
		return;
	}

	stream = (priority <= ZWP_LOG_ERR) ? stderr : stdout;

	for (i = 0; i < length; i += 16) {
		zwp_log_dump_line_console(stream, address + i, (length - i) < 16 ? (length - i) : 16);
		fprintf(stream, "\n");
	}
	fflush(stream);
}

/**
 * @brief Logging function for dumping a line of buffer content in syslog
 * @param[in]     priority          Log Priority : ZWP_LOG_XXX
 * @param[in]     address           Buffer
 * @param[in]     length            Buffer length
 * @return        Nothing
 */
static void zwp_log_dump_line_syslog(int priority, const char *address, const long length)
{
	char buffer[100];
	int offset;
	int i;
	char c;

	buffer[0] = '\0';
	offset = 0;

	offset += sprintf(buffer + offset, "|");

	for (i = 0; i < 16; i++)
	{
		if (i < length)
		{
			offset += sprintf(buffer + offset, " %02X", ((uint8_t *) address)[i]);
		}
		else
		{
			offset += sprintf(buffer + offset, " ..");
		}

		if (!((i + 1) % 8))
		{
			offset += sprintf(buffer + offset, " |");
		}
	}

	offset += sprintf(buffer + offset, " ");

	for (i = 0; i < 16; i++)
	{
		c = 0x7f & ((uint8_t *) address)[i];

		if (i < length && isprint(c))
		{
			offset += sprintf(buffer + offset, "%c", c);
		}
		else
		{
			offset += sprintf(buffer + offset, ".");
		}
	}

	syslog(priority, "%s", buffer);
}

/**
 * @brief Logging function for dumping a buffer content in syslog
 * @param[in]     priority          Log Priority : ZWP_LOG_XXX
 * @param[in]     address           Buffer
 * @param[in]     length            Buffer length
 * @return        Nothing
 */
static void zwp_log_dump_func_syslog(int priority, const char *address, const long length)
{
	int i;

	for (i = 0; i < length; i += 16) {
		zwp_log_dump_line_syslog(priority, address + i, (length - i) < 16 ? (length - i) : 16);
	}
}

zwp_status_t zwp_logger_start(zwp_logger_t *logger_p, const zwp_log_config_t log_config)
{
	static zwp_logger_t g_logger;
	zwp_logger_t logger;

	//Enforce singleton
	if (g_logger)
	{
		ZWP_LOG(ZWP_LOG_WARNING, ZWP_MSG_SINGLETON_MULTIPLE_CALLS);
		*logger_p = g_logger;

		return ZWP_STATUS_OK;
	}

	if ((logger = (zwp_logger_t)ZWP_MALLOC(sizeof(*logger))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_log_alloc;
	}

	if (log_config->target == ZWP_LOG_TARGET_CONSOLE)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Log target: changing to 'console'");
		ZWP_LOG_FUNC = zwp_log_func_console;
		ZWP_LOG_DUMP_FUNC = zwp_log_dump_func_console;

		//Equivalent to setlogmask()
		ZWP_LOG_MASK = log_config->mask;
	}
	else if (log_config->target == ZWP_LOG_TARGET_SYSLOG)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Log target: changing to 'syslog'");
		ZWP_LOG_FUNC = zwp_log_func_syslog;
		ZWP_LOG_DUMP_FUNC = zwp_log_dump_func_syslog;

		openlog(log_config->identity, LOG_CONS, LOG_USER);
		setlogmask(log_config->mask);
	}

	logger->target = log_config->target;

	g_logger = *logger_p = logger;

	return ZWP_STATUS_OK;

l_err_log_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_logger_stop(zwp_logger_t logger)
{
	if (logger->target == ZWP_LOG_TARGET_SYSLOG)
	{
		closelog();
	}
	ZWP_FREE(logger);
}

/**
 * @brief Helper function that sets log mask based on the the configuration
 * @param[in]     option_upto_level Configuration value for 'log.upto_level'
 * @param[out]    mask              Log mask
 * @return        Nothing
 *
 * @note If no match, @a mask remains unchanged
 */
static void zwp_log_option_set_mask(const char *option_upto_level, int *mask)
{
	int i, count;

	count = sizeof(zwp_log_level_options) / sizeof(*zwp_log_level_options);
	for (i = 0; i < count; i++)
	{
		if (strcasecmp(option_upto_level, zwp_log_level_options[i].option) == 0)
		{
			*mask = ZWP_LOG_LEVEL_UPTO(*zwp_log_level_options[i].level);
			break;
		}
	}
}

/**
 * @brief Helper function that sets log target based on the the configuration
 * @param[in]     option_target     Configuration value for 'log.target'
 * @param[out]    target            Enumerated target value
 * @return        Nothing
 *
 * @note If no match, @a target remains unchanged
 */
static void zwp_log_option_set_target(const char *option_target, zwp_log_target_t *target)
{
	int i, count;

	// Default to console
	*target = ZWP_LOG_TARGET_CONSOLE;

	count = sizeof(zwp_log_target_options) / sizeof(*zwp_log_target_options);
	for (i = 0; i < count; i++)
	{
		if (strcasecmp(option_target, zwp_log_target_options[i].option) == 0)
		{
			*target = zwp_log_target_options[i].target;
			break;
		}
	}
}

zwp_status_t zwp_log_config_get_instance(zwp_log_config_t *log_config_p,
		const config_t *config_parser, const char *config_dir, const char *identity)
{
	static zwp_log_config_t g_log_config;
	zwp_log_config_t log_config;
	const char *upto_level;
	const char *target;

	//Enforce singleton
	if (g_log_config)
	{
		ZWP_LOG(ZWP_LOG_WARNING, ZWP_MSG_SINGLETON_MULTIPLE_CALLS);
		*log_config_p = g_log_config;

		return ZWP_STATUS_OK;
	}

	if ((log_config = (zwp_log_config_t)ZWP_MALLOC(sizeof(*log_config))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_log_config_alloc;
	}

	//Identity
	if ((log_config->identity = ZWP_STRDUP(identity)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_identity_alloc;
	}

	//Log level (optional)
	upto_level = ZWP_OPTION_DEFAULT_UPTO_LEVEL;
	config_lookup_string(config_parser, "log.upto_level", &upto_level);
	zwp_log_option_set_mask(upto_level, &log_config->mask);

	//Log target (optional)
	target = ZWP_OPTION_DEFAULT_TARGET;
	config_lookup_string(config_parser, "log.target", &target);
	zwp_log_option_set_target(target, &log_config->target);

	g_log_config = *log_config_p = log_config;

	return ZWP_STATUS_OK;

l_err_identity_alloc:
	ZWP_FREE(log_config);
l_err_log_config_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_log_config_free(zwp_log_config_t log_config)
{
	ZWP_FREE(log_config->identity);
	ZWP_FREE(log_config);
}


