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
 * @file    zwu_string_buffer.c
 *
 * @brief   String buffer
 * @details String buffer utility
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-09-10
 * - Initial version
 */

#ifdef ZWS_WITH_FCGI
#include <fcgi_stdio.h>
#else
#include <stdio.h>
#endif

#include <stdarg.h>

#include "zwu_string_buffer.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zws_log.h"

/** Default allocation capacity for string buffer */
#define ZWS_STRING_BUFFER_DEFAULT_ALLOC_COUNT       16

/** String buffer class */
struct _zwu_string_buffer
{
	char                        *data;                       /**< Stored string */
	int                         capacity;                    /**< Maximum string length that can be stored */
	int                         length;                      /**< Length of the string stored */
};

int zwu_string_buffer_length(zwu_string_buffer_t string_buffer)
{
	return string_buffer->length;
}

char *zwu_string_buffer_get_string(zwu_string_buffer_t string_buffer)
{
	return string_buffer->data;
}

/**
 * @brief Make sure the string buffer is capable of storing at least the given length of string
 * @param[in]     string_buffer     String buffer object
 * @param[in]     min_capacity      Minimum length string that the string buffer should be capable of storing
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zwu_string_buffer_set_min_capacity(zwu_string_buffer_t string_buffer, int min_capacity)
{
	void *tmp;
	int capacity;

	if (min_capacity > string_buffer->capacity)
	{
		// Set to max(capacity + alloc_count, min_capacity)
		capacity = string_buffer->capacity + ZWS_STRING_BUFFER_DEFAULT_ALLOC_COUNT;
		if (min_capacity > capacity)
		{
			capacity = min_capacity;
		}

		if ((tmp = (void *)ZWS_REALLOC(string_buffer->data, capacity)) == NULL)
		{
			ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
			goto l_err_string_buffer_alloc;
		}

		string_buffer->data = tmp;
		string_buffer->capacity = capacity;
	}

	return ZWS_STATUS_OK;

l_err_string_buffer_alloc:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Append a C string to string buffer
 * @param[in]     string_buffer     String buffer object
 * @param[in]     string            C string to be appended
 * @param[in]     length            Length of the C string @a string
 * @return        Nothing
 */
static void zwu_string_buffer_append(zwu_string_buffer_t string_buffer, const char *string, int length)
{
	strncat(string_buffer->data, string, length);
	string_buffer->length += length;
}

zws_status_t zwu_string_buffer_append_string(zwu_string_buffer_t string_buffer, const char *string)
{
	int length;

	length = strlen(string);

	if (zwu_string_buffer_set_min_capacity(string_buffer, string_buffer->length + length + 1) != ZWS_STATUS_OK)
	{
		goto l_err_set_min_capacity;
	}

	zwu_string_buffer_append(string_buffer, string, length);

	return ZWS_STATUS_OK;

l_err_set_min_capacity:
	return ZWS_STATUS_ERROR;
}

zws_status_t zwu_string_buffer_append_format(zwu_string_buffer_t string_buffer, const char *format, ...)
{
	va_list ap;
	int length;

	va_start(ap, format);

	// Get the length of formatted string
	if ((length = vsnprintf(NULL, 0, format, ap)) < 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get length of formatted string");
		goto l_err_length;
	}

	if (zwu_string_buffer_set_min_capacity(string_buffer, string_buffer->length + length + 1) != ZWS_STATUS_OK)
	{
		goto l_err_set_min_capacity;
	}

	va_end(ap);
	va_start(ap, format);

	if ((length = vsnprintf(string_buffer->data + string_buffer->length, length + 1, format, ap)) < 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to append formatted string");
		goto l_err_append;
	}
	string_buffer->length += length;

	va_end(ap);

	return ZWS_STATUS_OK;

l_err_append:
l_err_set_min_capacity:
l_err_length:
	va_end(ap);

	return ZWS_STATUS_ERROR;
}

zws_status_t zwu_string_buffer_create(int capacity_hint, zwu_string_buffer_t *string_buffer_p)
{
	zwu_string_buffer_t string_buffer;

	if ((string_buffer = (zwu_string_buffer_t)ZWS_MALLOC(sizeof(*string_buffer))) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_string_buffer_alloc;
	}

	string_buffer->data = NULL;
	string_buffer->capacity = 0;
	string_buffer->length = 0;

	if (capacity_hint <= 0)
	{
		capacity_hint = ZWS_STRING_BUFFER_DEFAULT_ALLOC_COUNT;
	}

	if ((string_buffer->data = (char *)ZWS_MALLOC(sizeof(char) * capacity_hint)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_string_buffer_data_alloc;
	}

	string_buffer->data[0] = '\0';

	string_buffer->capacity = capacity_hint;

	*string_buffer_p  = string_buffer;

	return ZWS_STATUS_OK;

l_err_string_buffer_data_alloc:
	ZWS_FREE(string_buffer);
l_err_string_buffer_alloc:
	return ZWS_STATUS_ERROR;
}

void zwu_string_buffer_destroy(zwu_string_buffer_t string_buffer)
{
	ZWS_FREE(string_buffer->data);
	ZWS_FREE(string_buffer);
}
