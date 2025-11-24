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
@file		zwscn_util_unit.c  
  
                To provide support for unit conversion
  
@author		Arif Mohammand
 
@version	1.0 2015-01-23  Initial release
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>

#include <udunits2.h>
#include "zwscn_defines.h"
#include "zwscn_scene_api.h"
#include "zwscn_security_scene_api.h"
#include "zwscn_util_message_log.h"
#include "zwscn_util_unit.h"

static const char*	xmlPath = NULL; /**< use default path */
static double zwscn_units_convert_Hz_to_rpm(double input_value);
static double zwscn_units_convert_rpm_to_Hz(double input_value);

ZWSCN_STATUS_T zwscn_units_convert(const char *input_unit_str, double input_value, const char *output_unit_str, double *output_value) {
	ut_system*	unit_system;
	ut_unit* input_unit;
	ut_unit* output_unit;
	cv_converter* converter;
//	ut_status status;
	double value = 0;
	int result = 0;
	
	if((input_unit_str == NULL) || (output_unit_str == NULL) || (output_value == NULL)) {
		goto l_err_args;
	}
	
	if((!(strcmp(input_unit_str, "Hz"))) && (!(strcmp(output_unit_str, "rpm")))) {
		value = zwscn_units_convert_Hz_to_rpm(input_value);
	} else if((!(strcmp(input_unit_str, "rpm"))) && (!(strcmp(output_unit_str, "Hz")))) {
		value = zwscn_units_convert_rpm_to_Hz(input_value);
	} else {
		unit_system = ut_read_xml(xmlPath);
		if (unit_system == NULL) {
//			const char *path = ut_get_path_xml(xmlPath, &status);
//			const char *error = strerror(errno);
//			zwscn_message_log_warning("UNITS Couldn't initialize unit-system from database \"%s\": %s\n", path, error);
			zwscn_message_log_warning("UNITS Couldn't initialize unit-system\n");
			goto l_err_unit_system;
		}
		input_unit = ut_parse(unit_system, input_unit_str, UT_ASCII);
		if (input_unit == NULL) {
			ZWSCN_MESSAGE_LOG_DEBUG("UNITS Don't recognize \"%s\"\n", input_unit_str);
			goto l_err_unit_not_found;
		}
		output_unit = ut_parse(unit_system, output_unit_str, UT_ASCII);
		if (output_unit == NULL) {
			ZWSCN_MESSAGE_LOG_DEBUG("UNITS Don't recognize \"%s\"\n", output_unit_str);
			goto l_err_unit_not_found;
		}
		
		result = ut_are_convertible(input_unit, output_unit);
		if(result) {
			ZWSCN_MESSAGE_LOG_DEBUG("UNITS Conversion is possible between %s and %s\n",  input_unit_str, output_unit_str);
		} else {
			ZWSCN_MESSAGE_LOG_DEBUG("UNITS Conversion is NOT possible between %s and %s\n",  input_unit_str, output_unit_str);
			goto l_err_unit_not_convertible;
		}
		
		converter = ut_get_converter(input_unit, output_unit);
		value = cv_convert_double(converter, input_value);
		
		ut_free(input_unit);
		ut_free(output_unit);
		ut_free_system(unit_system);
	}
	ZWSCN_MESSAGE_LOG_DEBUG("UNITS input\t%0.3f %s\n", input_value, input_unit_str);
	ZWSCN_MESSAGE_LOG_DEBUG("UNITS output\t%0.3f %s\n", value, output_unit_str);
	(*output_value) = value;
	
	return ZWSCN_ERR_NONE;
l_err_args:
	return ZWSCN_ERR_INVALID_ARGS;
l_err_unit_system:
	return ZWSCN_ERR_GENERIC;
l_err_unit_not_found:
	return ZWSCN_ERR_INVALID_ARGS;
l_err_unit_not_convertible:
	return ZWSCN_ERR_INVALID_DATA;
}

static double zwscn_units_convert_Hz_to_rpm(double input_value) {
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	return (input_value * 60);
}

static double zwscn_units_convert_rpm_to_Hz(double input_value) {
	ZWSCN_MESSAGE_LOG_DEBUG_ENTER();
	return (input_value / 60);
}
