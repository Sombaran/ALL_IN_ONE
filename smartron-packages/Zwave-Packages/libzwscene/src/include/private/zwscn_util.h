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
@file		zwscn_util.h  
  
                General utility functions 
  
@author		Arif Mohammand
 
@version	1.0 2013-12-05  Initial release
*/
#ifndef _ZWSCN_UTIL_H_
#define _ZWSCN_UTIL_H_

/** @defgroup GroupPrivateUtilsCommon Common utils
 *  Utility functions
 *  @ingroup GroupPrivateUtils
 */
///@{

/**
 * @brief	Get Node id from combo id
 * @param[in] combo_id	Combo id
 * @return Node id
 */
uint8_t zwscn_util_combo_id_to_node_id(uint32_t combo_id);
/**
 * @brief	Get EP id from combo id
 * @param[in] combo_id	Combo id
 * @return EP id
 */
uint8_t zwscn_util_combo_id_to_ep_id(uint32_t combo_id);
/**
 * @brief	Get Command Class id from combo id
 * @param[in] combo_id	Combo id
 * @return Command Class id
 */
uint16_t zwscn_util_combo_id_to_cls_id(uint32_t combo_id);
/**
 * @brief	Make combo id from Node, EP and Command Class ids
 * @param[in] node_id	Node id
 * @param[in] ep_id		EP id
 * @param[in] cls_id	Command Class id
 * @return	Combo id
 */
uint32_t zwscn_util_make_combo_id(uint8_t node_id, uint8_t ep_id, uint16_t cls_id);
/**
 * @brief	Safe string copy
 * @param[in] dest	Destination string buffer
 * @param[in] src	Source string buffer
 * @param[in] dest_size	Destination string buffer size
 * @return Length of src
 */
unsigned long zwscn_util_strcpy(char *dest, const char *src, unsigned long dest_size);
/**
 * @brief	Safe string concatenation
 * @param[in] dest	Destination string buffer
 * @param[in] src	Source string buffer
 * @param[in] dest_size	Destination string buffer size
 * @return Length of src
 */

unsigned long zwscn_util_strcat(char *dest, const char *src, unsigned long dest_size);
/**
 * @brief	Duplicate a string
 * @param[in] src	Source string
 * @return Copy of input string
 */
char *zwscn_util_strdup(const char *src);
/**
 * @brief	Free memory of string
 * @param[in] string	String pointer
 */
void zwscn_util_str_free(char *string);
/**
 * @brief   Convert string to hash
 * @param[in] string    String pointer
 * @return Computed hash value
 */
uint32_t zwscn_util_str_hash(char *string);
//void zwscn_util_atomic_set(int *atomic_var, int value);
/**
 * @brief	Convert Z-Wave multi-byte data buffer to Integer value
 * @param[in] size	Size of the data
 * @param[in] data	Data buffer
 * @return	Integer value
 */
int32_t zwscn_util_data2value(uint8_t size, uint8_t *data);
/**
 * @brief	Convert from Integer value to double based input precision value
 * @param[in] value_int	Integer value
 * @param[in] precision	Precision value
 * @return Double value
 */
double zwscn_util_int2double(int32_t value_int, uint8_t precision);
/**
 * @brief	round a double value based input precision value
 * @param[in] value		Input value
 * @param[in] precision	Precision value
 * @return Double value
 */
double zwscn_util_round(double value, uint8_t precision);
/**
 * @brief	Compare double values for equality
 * @param a	First value
 * @param b	Second value
 * @return Boolean value (Returns TRUE is a and b are equal)
 * @note In-built comparison operator "=" may fail for Double value
 */
int zwscn_is_value_equal(double a, double b);
/**
 * @brief	Check if a number falls between two given numbers
 * @param[in] value_low_double	Low range value
 * @param[in] value_high_double	High range value
 * @param[in] value_double		Number to check
 * @return	Boolean value (TRUE if value_double is in range)
 */
ZWSCN_BOOL_T zwscn_is_value_in_range(double value_low_double, double value_high_double, double value_double);
/**
 * @brief	Convert unsigned integer to pointer in a portable manner
 * @param[in] value	Input unsigned integer value
 * @return	void* 	Pointer representation of input value
 * @note	This is only used for storing integer values in memory allocated for a pointer value.
 * 			The returned pointer should not be used as actual pointer value.
 */
void *zwscn_uint_to_pointer(uint32_t value);
/**
 * @brief	Convert pointer to unsigned integer in a portable manner
 * @param[in] ptr	Input pointer value
 * @return	uint32_t Unsigned integer representation of the pointer
 * @note	This is reverse function of @ref zwscn_uint_to_pointer. 
 */
uint32_t zwscn_pointer_to_uint(void *ptr);
/**
 * @brief	Unescape (decode) an URLencoded URI
 * @param[in] input_string	URI string
 * @return Unescaped URI string
 */
char *zwscn_util_unescape_uri(const char *input_string);

/**
 * @brief   Escape (encode) to URLencoded URI
 * @param[in] input_string  string
 * @return Escaped URI string
 */
char *zwscn_util_escape_uri(const char *input_string);
/**
 * @brief	Check if number for sending SMS is valid
 * @param[in] sms_number			Number for sending SMS
 * @param[in] flag_is_url_encoded	Flag to indicate if the input string is URLencoded
 * @return ZWSCN_BOOL_T
 */
ZWSCN_BOOL_T zwscn_util_sms_number_is_valid(const char *sms_number, ZWSCN_BOOL_T flag_is_url_encoded);
/**
 * @brief	Check if email address is valid
 * @param[in] email_addr		Email address
 * @param flag_is_url_encoded	Flag to indicate if the input string is URLencoded
 * @return ZWSCN_BOOL_T
 */
ZWSCN_BOOL_T zwscn_util_email_is_valid(const char *email_addr, ZWSCN_BOOL_T flag_is_url_encoded);
///@}

#endif // _ZWSCN_UTIL_H_
