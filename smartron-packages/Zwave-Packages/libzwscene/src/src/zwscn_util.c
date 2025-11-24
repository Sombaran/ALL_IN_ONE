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
@file		zwscn_util.c  
  
                General utility functions
  
@author		Arif Mohammand
 
@version	1.0 2013-12-05  Initial release
*/

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "glib.h"
#include "zwscn_defines.h"
#include "zwscn_scene_api.h"
#include "zwscn_security_scene_api.h"
#include "zwscn_util.h"
#include "zwscn_util_message_log.h"

uint8_t zwscn_util_combo_id_to_node_id(uint32_t combo_id) {
	uint8_t node_id=0;
	
	node_id = 0xFF & (combo_id >> 24);
	
	return node_id;	
}

uint8_t zwscn_util_combo_id_to_ep_id(uint32_t combo_id) {
	uint8_t ep_id=0;
	
	ep_id = 0xFF & (combo_id >> 16);
	
	return ep_id;	
}

uint16_t zwscn_util_combo_id_to_cls_id(uint32_t combo_id) {
	uint16_t cls_id=0;
	
	cls_id = 0xFFFF & combo_id;
	
	return cls_id;	
}

uint32_t zwscn_util_make_combo_id(uint8_t node_id, uint8_t ep_id, uint16_t cls_id) {
	uint32_t result = 0;
	
	result = ((0xFF & node_id) << 24) | ((0xFF & ep_id) << 16) | (0xFFFF & cls_id);
	
	return result;
}

unsigned long zwscn_util_strcpy(char *dest, const char *src, unsigned long dest_size) { //safe strcpy
	return  g_strlcpy(dest, src, dest_size);
}

unsigned long zwscn_util_strcat(char *dest, const char *src, unsigned long dest_size) { //safe strcat
	return  g_strlcat (dest, src, dest_size);
}

char *zwscn_util_strdup(const char *src) { //safe strdup
	return  g_strdup (src);
}

void zwscn_util_str_free(char *string) {
	g_free(string);
}

uint32_t zwscn_util_str_hash(char *string){
    return g_str_hash(string);
}
//void zwscn_util_atomic_set(int *atomic_var, int value) {
//	 g_atomic_int_set(atomic_var, value);
//}

int32_t zwscn_util_data2value(uint8_t size, uint8_t *data) {
	int32_t value_int;
	int32_t val32;
	int16_t val16;
	int8_t val8;
	
	if(data == NULL) {
		goto l_err;
	}
				
	if (size == 1)
	{
		val8 = data[0];
		value_int = val8;
	}
	else if (size == 2)
	{
		val16 = data[0];
		val16 = (val16 << 8) | data[1];
		value_int = val16;
	}
	else
	{
		val32 = ((int32_t)(data[0])) << 24 | ((int32_t)(data[1])) << 16 
				| ((int32_t)(data[2])) << 8 | data[3];
		value_int = val32;
	}
	
	return value_int;
	
l_err:
	return 0;
}

double zwscn_util_int2double(int32_t value_int, uint8_t precision) {
	double value_dbl = 0.0;
	int i;
	
	value_dbl = (double)value_int;
	for(i=0 ; i < precision; i++)
	{
            value_dbl /= 10.0;
	}
	
	return value_dbl;
}

double zwscn_util_round(double value, uint8_t precision) {
	double value_dbl = 0.0;
	int i;
	
	value_dbl = (double)value;
	for(i=0 ; i < precision; i++)
	{
            value_dbl *= 10.0;
	}
	value_dbl = round(value_dbl);
	for(i=0 ; i < precision; i++)
	{
            value_dbl /= 10.0;
	}
	
	return value_dbl;
}


int zwscn_is_value_equal(double a, double b)
{
    return (fabs(a - b) < ZWSCN_COMPARE_DOUBLE_EPSILON);
}

ZWSCN_BOOL_T zwscn_is_value_in_range(double value_low_double, double value_high_double, double value_double) {
	ZWSCN_BOOL_T result = ZWSCN_FALSE;
	double temp;
	
//	//if(value_double >= 0){ //Make sure positive value
//		if(value_low_double <= value_double){
//			if(value_high_double != 0){
//				if(value_high_double >= value_double) {
//					result = ZWSCN_TRUE;
//				}
//			} else {
//				result = ZWSCN_TRUE; //Value High is not set, so comparison with Value low is sufficient
//			}
//		}
//	//}

	if(value_low_double > value_high_double) { //if low value is more than high value then swap them
		temp = value_low_double;
		value_low_double = value_high_double;
		value_high_double = temp;
	}
	
	if(zwscn_is_value_equal(value_low_double, value_high_double)) { //if values are equal - this is equality check using a special function
		if(zwscn_is_value_equal(value_low_double, value_double)) { //if the current value is also equal to any of them
			result = ZWSCN_TRUE;
		}
	} else {
		if((value_low_double <= value_double) && (value_high_double >= value_double)) { //if current value is within the range
			result = ZWSCN_TRUE;
		}
	}
	
	return result;
}

uint32_t zwscn_pointer_to_uint(void *ptr) {
	return (GPOINTER_TO_UINT(ptr));
}

void *zwscn_uint_to_pointer(uint32_t value) {
	return (GUINT_TO_POINTER(value));
}

char *zwscn_util_unescape_uri(const char *input_string) {
	return g_uri_unescape_string(input_string, NULL);
}

char *zwscn_util_escape_uri(const char *input_string) {
    return g_uri_escape_string(input_string, NULL, FALSE);
}

ZWSCN_BOOL_T zwscn_util_sms_number_is_valid(const char *sms_number, ZWSCN_BOOL_T flag_is_url_encoded) {
	char temp[ZWSCN_LEN_SMS_NUMBER];
	const char *number = NULL;
	ZWSCN_BOOL_T result = ZWSCN_FALSE;
	char *pattern = ZWSCN_REGEX_SMS_NUMBER;
	
	if(sms_number == NULL) {
		goto l_err_args;
	}
	zwscn_util_strcpy(temp, sms_number, ZWSCN_LEN_SMS_NUMBER);
	if(flag_is_url_encoded == ZWSCN_TRUE) {
		number = zwscn_util_unescape_uri(temp);
	} else {
		number = temp;
	}
	
	ZWSCN_MESSAGE_LOG_DEBUG("Checking Number %s\n", number);
	result = g_regex_match_simple (pattern, number, 0, 0);
	if(flag_is_url_encoded == ZWSCN_TRUE) {
		free((void *)number);
	}
	
	return result;
l_err_args:
	return ZWSCN_FALSE;
}

ZWSCN_BOOL_T zwscn_util_email_is_valid(const char *email_addr, ZWSCN_BOOL_T flag_is_url_encoded) {
	char temp[ZWSCN_LEN_EMAIL];
	const char *email = NULL;
	ZWSCN_BOOL_T result = ZWSCN_FALSE;
	char *pattern = ZWSCN_REGEX_EMAIL_ADDR;
	
	if(email_addr == NULL) {
		goto l_err_args;
	}
	zwscn_util_strcpy(temp, email_addr, ZWSCN_LEN_EMAIL);
	if(flag_is_url_encoded == ZWSCN_TRUE) {
		email = zwscn_util_unescape_uri(temp);
	} else {
		email = temp;
	}
	
	ZWSCN_MESSAGE_LOG_DEBUG("Checking Email %s\n", email);
	result = g_regex_match_simple (pattern, email, 0, 0);
	if(flag_is_url_encoded == ZWSCN_TRUE) {
		free((void *)email);
	}
	
	return result;
l_err_args:
	return ZWSCN_FALSE;
}
