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
 * @file    zws_log.c
 *
 * @brief   Logging module.
 * @details Handles application logging.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-02
 * - Initial version
 */

#include <stdarg.h>

#ifdef ZWS_WITH_FCGI
#include <fcgi_stdio.h>
#else
#include <stdio.h>
#endif

#include <syslog.h>
#include <ctype.h>
#include <stdint.h>

#include "zws_log.h"

/** Log 'upto' mask */
#define ZWS_LOG_LEVEL_UPTO LOG_UPTO
/** Log bit mask */
#define ZWS_LOG_LEVEL_MASK LOG_MASK

#define LOG_FUNCTION	LOG_DEBUG

/** Log priority: Emergency */
int ZWS_LOG_EMERG = LOG_EMERG;
/** Log priority: Alert */
int ZWS_LOG_ALERT = LOG_ALERT;
/** Log priority: Critical */
int ZWS_LOG_CRIT = LOG_CRIT;
/** Log priority: Error */
int ZWS_LOG_ERR = LOG_ERR;
/** Log priority: Warning */
int ZWS_LOG_WARNING = LOG_WARNING;
/** Log priority: Notice */
int ZWS_LOG_NOTICE = LOG_NOTICE;
/** Log priority: Information */
int ZWS_LOG_INFO = LOG_INFO;
/** Log priority: Debug */
int ZWS_LOG_DEBUG = LOG_DEBUG;
/** Log priority: Function */
int ZWS_LOG_FUNCTION = LOG_FUNCTION;

/** Default log level */
#define ZWS_DEFAULT_UPTO_LEVEL ZWS_LOG_DEBUG;

/** Global mask of enabled log priorities. Mask of ZWS_LOG_XXX */
int ZWS_LOG_MASK;

/* Logging function prototypes */
static void zws_log_func_console(int priority, const char *format, ...);
static void zws_log_dump_func_console(int priority, const char *address, const long length);

/** Logging function */
void (*ZWS_LOG_FUNC)(int priority, const char *format, ...) = zws_log_func_console;
void (*ZWS_LOG_DUMP_FUNC)(int priority, const char *address, const long length) = zws_log_dump_func_console;

/**
 * @brief Log messages to the console
 * @param[in]     priority          Log priority : ZWS_LOG_EMERG to ZWS_LOG_DEBUG
 * @param[in]     format            Format string followed by items to be passed to it
 * @return        Nothing
 */
static void zws_log_func_console(int priority, const char *format, ...)
{
	va_list ap;
	int level;

	//Set mask to default, if logger is not started
	if (!ZWS_LOG_MASK)
	{
		level = ZWS_DEFAULT_UPTO_LEVEL;
		ZWS_LOG_MASK = ZWS_LOG_LEVEL_UPTO(level);
	}

	if(!(ZWS_LOG_LEVEL_MASK(priority) & ZWS_LOG_MASK))
	{
		return;
	}

	va_start(ap, format);
	vfprintf(stderr, format, ap);
	fflush(stderr);
	va_end(ap);
}

/**
 * @brief Logging function for dumping a line of buffer content in console
 * @param[in]     stream            FILE stream
 * @param[in]     address           Buffer
 * @param[in]     length            Buffer length
 * @return        Nothing
 */
static void zws_log_dump_line_console(FILE * stream, const char *address, const long length)
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
 * @param[in]     priority          Log Priority : ZWS_LOG_XXX
 * @param[in]     address           Buffer
 * @param[in]     length            Buffer length
 * @return        Nothing
 */
static void zws_log_dump_func_console(int priority, const char *address, const long length)
{
	int level;
	FILE *stream;
	int i;

	//Set mask to default, if logger is not started
	if (!ZWS_LOG_MASK)
	{
		level = ZWS_DEFAULT_UPTO_LEVEL;
		ZWS_LOG_MASK = ZWS_LOG_LEVEL_UPTO(level);
	}

	if(!(ZWS_LOG_LEVEL_MASK(priority) & ZWS_LOG_MASK))
	{
		return;
	}

	stream = stderr;

	for (i = 0; i < length; i += 16) {
		zws_log_dump_line_console(stream, address + i, (length - i) < 16 ? (length - i) : 16);
		fprintf(stream, "\n");
	}
	fflush(stream);
}

