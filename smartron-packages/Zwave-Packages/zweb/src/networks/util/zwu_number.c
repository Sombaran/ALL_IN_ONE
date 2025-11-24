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
 * @file    zwu_number.c
 *
 * @brief   Handles numbers
 * @details Handles numbers
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-03-25
 * - Initial version
 */

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>

#include "zwu_number.h"
#include "zwu_log.h"

/*
 * Parse the string as long
 */
zwu_status_t zwu_parse_long(const char *string, long *long_p)
{
	char *endptr;
	long value;

	// If empty
	if (!string[0])
	{
		ZWU_LOG(ZWU_LOG_ERR, "Empty string");
		goto l_err_empty;
	}

	errno = 0;
	value = strtol(string, &endptr, 10);

	// Checking range and invalid value
	if (((errno == ERANGE) && ((value == LONG_MAX) || (value == LONG_MIN)))
			|| ((errno != 0) && (value == 0)))
	{
		ZWU_LOG(ZWU_LOG_ERR, "strtol() failed: %s", strerror(errno));
		goto l_err_invalid_value;
	}

	if (endptr == string)
	{
		ZWU_LOG(ZWU_LOG_ERR, "No valid digits");
		goto l_err_invalid_digit;
	}

	if (*endptr != '\0')
	{
		ZWU_LOG(ZWU_LOG_ERR, "Extra string after parsing");
		goto l_err_extra_string;
	}

	*long_p = value;

	return ZWU_STATUS_OK;

l_err_extra_string:
l_err_invalid_digit:
l_err_invalid_value:
l_err_empty:
	return ZWU_STATUS_ERROR;
}

/*
 * Parse the string as long long
 */
zwu_status_t zwu_parse_long_long(const char *string, long long *long_long_p)
{
	char *endptr;
	long long value;

	// If empty
	if (!string[0])
	{
		ZWU_LOG(ZWU_LOG_ERR, "Empty string");
		goto l_err_empty;
	}

	errno = 0;
	value = strtoll(string, &endptr, 10);

	// Checking range and invalid value
	if (((errno == ERANGE) && ((value == LLONG_MAX) || (value == LLONG_MIN)))
			|| ((errno != 0) && (value == 0)))
	{
		ZWU_LOG(ZWU_LOG_ERR, "strtol() failed: %s", strerror(errno));
		goto l_err_invalid_value;
	}

	if (endptr == string)
	{
		ZWU_LOG(ZWU_LOG_ERR, "No valid digits");
		goto l_err_invalid_digit;
	}

	if (*endptr != '\0')
	{
		ZWU_LOG(ZWU_LOG_ERR, "Extra string after parsing");
		goto l_err_extra_string;
	}

	*long_long_p = value;

	return ZWU_STATUS_OK;

l_err_extra_string:
l_err_invalid_digit:
l_err_invalid_value:
l_err_empty:
	return ZWU_STATUS_ERROR;
}

/*
 * Parse the string as int32_t
 */
zwu_status_t zwu_parse_int32(const char *string, int32_t *value_p)
{
	long value;

	if (zwu_parse_long(string, &value) != ZWU_STATUS_OK)
	{
		goto l_err_parse_long;
	}

	if ((value > INT32_MAX) || (value < INT32_MIN))
	{
		ZWU_LOG(ZWU_LOG_ERR, "Parsed valued doesn't fit into int32_t : %ld", value);
		goto l_err_invalid_integer;
	}

	*value_p = (int32_t)value;

	return ZWU_STATUS_OK;

l_err_invalid_integer:
l_err_parse_long:
		return ZWU_STATUS_ERROR;
}

/*
 * Parse the string as non-negative int32_t
 */
zwu_status_t zwu_parse_int32_non_negative(const char *string, int32_t *value_p)
{
	if (zwu_parse_int32(string, value_p) != ZWU_STATUS_OK)
	{
		goto l_err_parse_int32;
	}

	if ((*value_p < 0))
	{
		ZWU_LOG(ZWU_LOG_ERR, "Parsed valued is a negative int32_t : %d", *value_p);
		goto l_err_invalid_sign;
	}

	return ZWU_STATUS_OK;

l_err_invalid_sign:
l_err_parse_int32:
		return ZWU_STATUS_ERROR;
}

/*
 * Parse the string as int64_t
 */
zwu_status_t zwu_parse_int64(const char *string, int64_t *value_p)
{
	long long value;

	if (zwu_parse_long_long(string, &value) != ZWU_STATUS_OK)
	{
		goto l_err_parse_long_long;
	}

	if ((value > INT64_MAX) || (value < INT64_MIN))
	{
		ZWU_LOG(ZWU_LOG_ERR, "Parsed valued doesn't fit into int64_t : %lld", value);
		goto l_err_invalid_integer;
	}

	*value_p = (int64_t)value;

	return ZWU_STATUS_OK;

l_err_invalid_integer:
l_err_parse_long_long:
		return ZWU_STATUS_ERROR;
}

/*
 * Parse the string as non-negative int64_t
 */
zwu_status_t zwu_parse_int64_non_negative(const char *string, int64_t *value_p)
{
	if (zwu_parse_int64(string, value_p) != ZWU_STATUS_OK)
	{
		goto l_err_parse_int64;
	}

	if ((*value_p < 0))
	{
		ZWU_LOG(ZWU_LOG_ERR, "Parsed valued is a negative int64_t : %lld", *value_p);
		goto l_err_invalid_sign;
	}

	return ZWU_STATUS_OK;

l_err_invalid_sign:
l_err_parse_int64:
		return ZWU_STATUS_ERROR;
}

/*
* Parse the string as double
*/
zwu_status_t zwu_parse_double(const char *string, double *double_p)
{
	char *endptr;
	double value;

	// If empty
	if (!string[0])
	{
		ZWU_LOG(ZWU_LOG_ERR, "Empty string");
		goto l_err_empty;
	}

	errno = 0;
	value = strtod(string, &endptr);

	// Checking range and invalid value
	if (((errno == ERANGE) && ((value == HUGE_VAL) || (value == - HUGE_VAL)))
		|| ((errno != 0) && (value == 0.0)))
	{
		ZWU_LOG(ZWU_LOG_ERR, "strtod() failed: %s. String to parse is: %s", strerror(errno), string);
		goto l_err_invalid_value;
	}

	if (endptr == string)
	{
		ZWU_LOG(ZWU_LOG_ERR, "No valid double");
		goto l_err_invalid_digit;
	}

	if (*endptr != '\0')
	{
		ZWU_LOG(ZWU_LOG_ERR, "Extra string after parsing");
		goto l_err_extra_string;
	}

	*double_p = value;

	return ZWU_STATUS_OK;

l_err_extra_string:
l_err_invalid_digit:
l_err_invalid_value:
l_err_empty:
	return ZWU_STATUS_ERROR;
}

/*
 * Check if a number is uint8_t
 */
zwu_status_t zwu_number_is_uint8(long number)
{
	if ((number > UINT8_MAX) || (number < 0))
	{
		ZWU_LOG(ZWU_LOG_ERR, "Number not in range uint8_t : %ld", number);
		goto l_err_invalid_number;
	}

	return ZWU_STATUS_OK;

l_err_invalid_number:
	return ZWU_STATUS_ERROR;
}

/*
 * Check if a number is uint16_t
 */
zwu_status_t zwu_number_is_uint16(long number)
{
	if ((number > UINT16_MAX) || (number < 0))
	{
		ZWU_LOG(ZWU_LOG_ERR, "Number not in range uint16_t : %ld", number);
		goto l_err_invalid_number;
	}

	return ZWU_STATUS_OK;

l_err_invalid_number:
	return ZWU_STATUS_ERROR;
}

/*
 * Check if a number is uint32_t
 */
zwu_status_t zwu_number_is_uint32(long long number)
{
	if ((number > UINT32_MAX) || (number < 0))
	{
		ZWU_LOG(ZWU_LOG_ERR, "Number not in range uint32_t : %lld", number);
		goto l_err_invalid_number;
	}

	return ZWU_STATUS_OK;

l_err_invalid_number:
	return ZWU_STATUS_ERROR;
}
