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
@file       zwscn_xml.h - xml reader writer API

                To provide xml reading and writing services

@author     Mafahir Fairoze

@version    1.0 2016-01-18  Initial release
 */
#pragma once

/** @defgroup GroupPrivateUtilXML XML utils
 *  Utility functions for reading writing XML files
 *  @ingroup GroupPrivateUtils
 */
///@{

/**
 * @brief   Enumeration to specify encoding format
 */
typedef enum {
	XML_ENCODING_ASCII, XML_ENCODING_UTF_8,
} xml_encoding;

/**
 * @brief   XML Writer settings to be passed on creation
 */
typedef struct xml_writer_settings{
	xml_encoding encoding;                  /**< Document Encoding */
	uint8_t indent;                         /**< Indent XML output */
	uint8_t new_line_on_attributes;         /**< Wrap new attributes with new line */
	uint8_t omit_xml_decleration;           /**< Remove xml declaration */
} xml_writer_settings_t;

struct xml_writer;
typedef struct xml_writer xml_writer_t;     /**< Opaque handle for a xml writer */

struct xml_reader;
typedef struct xml_reader xml_reader_t;     /**< Opaque handle for a xml reader */

/**
 * @brief   Create a new xml writer
 * @param filename  Output filename
 * @param settings  Document settings
 * @return    xml_writer_t handle
 */
xml_writer_t* xml_create_writer(const char* filename, xml_writer_settings_t settings);

/**
 * @brief   Destroys any allocated memory for xml writer
 * @param writer  xml writer handle
 */
void xml_writer_close(xml_writer_t* writer);

/**
 * @brief   Appends a xml start element to the document
 * @param writer  xml writer handle
 * @param name  element name
 */
void xml_writer_write_start_element(xml_writer_t* writer, char* name);

/**
 * @brief   Appends a xml attribute to the document
 * @param writer  xml writer handle
 * @param name  atribute name
 * @param value  attribute value
 */
void xml_writer_write_attribute(xml_writer_t* writer, char* name, char* value);

/**
 * @brief   Appends a xml comment to the document
 * @param writer  xml writer handle
 * @param comment  string content
 */
void xml_writer_write_comment(xml_writer_t* writer, char* comment);

/**
 * @brief   Appends a xml cdata section to the document
 * @param writer  xml writer handle
 * @param cdata  cdata content
 * @param length  cdata content length
 */
void xml_writer_write_cdata(xml_writer_t* writer, char* cdata, size_t length);

/**
 * @brief   Appends a xml string to the document
 * @param writer  xml writer handle
 * @param str  string content
 */
void xml_writer_write_string(xml_writer_t* writer, char* str);

/**
 * @brief   Appends all end elements that are open so far
 * @param writer  xml writer handle
 */
void xml_writer_write_end_all_element(xml_writer_t* writer);

/**
 * @brief   Appends the last end element opened
 * @param writer  xml writer handle
 */
void xml_writer_write_end_element(xml_writer_t* writer);

/**
 * @brief   Converts uint64 to string
 * @param buf  buffer that can hold the string length of the uint64 type
 * @param value to convert from
 * @return char* uint64 as string
 */
char* xml_uint64_to_string(char* buf, uint64_t value);

/**
 * @brief   Converts uint32 to string
 * @param buf  buffer that can hold the string length of the uint32 type
 * @param value to convert from
 * @return char* uint32 as string
 */
char* xml_uint32_to_string(char* buf, uint32_t value);

/**
 * @brief   Converts uint8 to string
 * @param buf  buffer that can hold the string length of the uint8 type
 * @param value to convert from
 * @return char* uint8 as string
 */
char* xml_uint8_to_string(char* buf, uint8_t value);

/**
 * @brief   Converts uint8 to hex string
 * @param buf  buffer that can hold the string length of the uint8 type
 * @param value to convert from
 * @return char* uint8 as hex string
 */
char* xml_uint8_to_hexstring(char* buf, uint8_t value);

/**
 * @brief   Converts int32 to string
 * @param buf  buffer that can hold the string length of the int32 type
 * @param value to convert from
 * @return char* int32 as string
 */
char* xml_int32_to_string(char* buf, int32_t value);

/**
 * @brief   Converts string to uint64
 * @param str  string to convert from
 * @return uint64_t value
 */
uint64_t xml_string_to_uint64(char* str);

/**
 * @brief   Converts string to uint32
 * @param str  string to convert from
 * @return uint32_t value
 */
uint32_t xml_string_to_uint32(char* str);

/**
 * @brief   Converts string to uint8
 * @param str  string to convert from
 * @return uint8_t value
 */
uint8_t xml_string_to_uint8(char* str);

/**
 * @brief   Converts string to int32
 * @param str  string to convert from
 * @return int32_t value
 */
int32_t xml_string_to_int32(char* str);

/**
 * @brief   XML reader parsing exceptions
 */
typedef enum xml_exception {
	XML_EXCEPTION_END_OF_FILE = -1,
	XML_EXCEPTION_NONE = 0,
	XML_EXCEPTION_ARGS,
	XML_EXCEPTION_PI,
	XML_EXCEPTION_CDATA,
	XML_EXCEPTION_COMMENT,
	XML_EXCEPTION_END_ELEMENT,
	XML_EXCEPTION_START_ELEMENT,
	XML_EXCEPTION_ATTRIBUTE,
	XML_EXCEPTION_TEXT,
	XML_EXCEPTION_MISMATCHED_TAG,
} xml_exception_t;

/**
 * @brief   xml reader node types
 */
typedef enum xml_node_type {
	XML_NODE_TYPE_NONE,
	XML_NODE_TYPE_PI,
	XML_NODE_TYPE_ATTRIBUTE,
	XML_NODE_TYPE_START_ELEMENT,
	XML_NODE_TYPE_END_ELEMENT,
	XML_NODE_TYPE_TEXT,
	XML_NODE_TYPE_COMMENT,
	XML_NODE_TYPE_CDATA,
} xml_node_type_t;

/**
 * @brief   Creates a new xml reader for the given file
 * @param filename  file to read
 * @return xml_reader_t handle
 */
xml_reader_t* xml_reader_new(const char* filename);

/**
 * @brief   Releases all memory allocated for the xml reader
 * @param reader  handle
 */
void xml_reader_close(xml_reader_t* reader);

/**
 * @brief   Iterates through the xml document to the next node
 * @param reader  handle
 * @return xml_exception_t  parser exception if any
 */
xml_exception_t xml_reader_next(xml_reader_t* reader);

/**
 * @brief   Retrieves the current node type
 * @param reader  handle
 * @return xml_node_type_t  node type
 */
xml_node_type_t xml_reader_node_type(xml_reader_t* reader);

/**
 * @brief   Retrieves the current node name
 * @param reader  handle
 * @return char*  node name
 */
char* xml_reader_node_name(xml_reader_t* reader);

/**
 * @brief   Retrieves the current node content
 * @param reader  handle
 * @return char*  node content
 */
char* xml_reader_node_content(xml_reader_t* reader);

/**
 * @brief   Converts xml parser error to string
 * @param error  error to convert from
 * @return char*  xml parser error in string
 */
const char* xml_error_to_string(xml_exception_t error);

/**
 * @brief   Skips xml content until node name and type or end of document
 * @param reader  handle
 * @param type  node type
 * @param node_name  node name
 * @return uint8_t  1 if node found else 0
 */
uint8_t xml_reader_skip_till(xml_reader_t* reader, xml_node_type_t type, char* node_name);

/**
 * @brief   Skips xml content until type or end of document
 * @param reader  handle
 * @param type  node type
 * @param till  node type to break searching
 * @return uint8_t  1 if node type found else 0
 */
uint8_t xml_reader_skip_till_type(xml_reader_t* reader, xml_node_type_t type, xml_node_type_t till);

///@}
