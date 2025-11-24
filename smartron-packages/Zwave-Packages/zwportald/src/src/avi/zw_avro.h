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
 * @file    zw_avro.h
 *
 * @brief   Avro
 * @details Handles Avro schema and data
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-14
 * - Initial version
 */

#ifndef ZW_AVRO_H_
#define ZW_AVRO_H_

/**
 * \addtogroup zwportaldavro 
 * @{
 */

#include <avro.h>

#include "zwp_status.h"

/** Forward declaration for Avro class */
struct _zw_avro;
/** Avro type */
typedef struct _zw_avro *zw_avro_t;

/** Offset of Avro message length field */
#define ZW_AVRO_FRAME_MESSAGE_LENGTH_OFFSET     0
/** Size of Avro message length field */
#define ZW_AVRO_FRAME_MESSAGE_LENGTH_SIZE       4
/** Bitmask of Avro message length field */
#define ZW_AVRO_FRAME_MESSAGE_LENGTH_MASK       0xFFFFFFFF

/** Offset of Avro message data field */
#define ZW_AVRO_FRAME_MESSAGE_DATA_OFFSET       4

/**
 * @name Getters
 * @{
 */
/**
 * @brief Get Avro protocol schema
 * @param[in]     avro              Avro object
 * @return        Avro protocol schema
 */
avro_schema_t zw_avro_get_protocol_schema(zw_avro_t avro);
/**
 * @brief Get Avro types schema
 * @param[in]     avro              Avro object
 * @return        Avro types schema
 */
avro_schema_t zw_avro_get_types_schema(zw_avro_t avro);
/**
 * @brief Get Avro types interface
 * @param[in]     avro              Avro object
 * @return        Avro types interface
 */
avro_value_iface_t *zw_avro_get_types_iface(zw_avro_t avro);
/**
 * @brief Get Avro cookies interface
 * @param[in]     avro              Avro object
 * @return        Avro cookies interface
 */
avro_value_iface_t *zw_avro_get_cookies_iface(zw_avro_t avro);
/**
 * @brief Get Avro messages schema
 * @param[in]     avro              Avro object
 * @return        Avro messages schema
 */
avro_schema_t zw_avro_get_messages_schema(zw_avro_t avro);
/**
 * @}
 */

/**
 * @brief Get boolean value from Avro record value
 * @param[in]     record_value      Avro record value
 * @param[in]     field_name        Field name
 * @param[out]    boolean_value_p   Boolean value
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zw_avro_field_get_boolean(avro_value_t *record_value, const char *field_name, int *boolean_value_p);

/**
 * @brief Get int32_t integer value from Avro record value
 * @param[in]     record_value      Avro record value
 * @param[in]     field_name        Field name
 * @param[out]    integer_value_p   int32_t integer value
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zw_avro_field_get_int(avro_value_t *record_value, const char *field_name, int32_t *integer_value_p);

/**
 * @brief Get int64_t integer value from Avro record value
 * @param[in]     record_value      Avro record value
 * @param[in]     field_name        Field name
 * @param[out]    long_value_p      int64_t integer value
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zw_avro_field_get_long(avro_value_t *record_value, const char *field_name, int64_t *long_value_p);

/**
 * @brief Get string value from Avro record value
 * @param[in]     record_value      Avro record value
 * @param[in]     field_name        Field name
 * @param[out]    string_value_p    String value
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zw_avro_field_get_string(avro_value_t *record_value, const char *field_name, char **string_value_p);
/**
 * @brief Free string value from Avro record value
 * @param[in]     string_value      String value
 * @return        Nothing
 */
void zw_avro_field_free_string(char *string_value);

/**
 * @brief Get fixed length value from Avro record value
 * @param[in]     record_value      Avro record value
 * @param[in]     field_name        Field name
 * @param[out]    buffer_p          Fixed length value
 * @param[out]    size_p            Length of the fixed length value in @a buffer_p
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zw_avro_field_get_fixed(avro_value_t *record_value, const char *field_name, void **buffer_p, size_t *size_p);
/**
 * @brief Free fixed length value from Avro record value
 * @param[in]     buffer            String value
 * @return        Nothing
 */
void zw_avro_field_free_fixed(void *buffer);

/**
 * @brief Get byte stream value from Avro record value
 * @param[in]     record_value      Avro record value
 * @param[in]     field_name        Field name
 * @param[out]    buffer_p          Byte stream value
 * @param[out]    size_p            Length of the byte stream value in @a buffer_p
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zw_avro_field_get_bytes(avro_value_t *record_value, const char *field_name, void **buffer_p, size_t *size_p);
/**
 * @brief Free byte stream value from Avro record value
 * @param[in]     buffer            String value
 * @return        Nothing
 */
void zw_avro_field_free_bytes(void *buffer);

/**
 * @brief Get null value from Avro record value
 * @param[in]     record_value      Avro record value
 * @param[in]     field_name        Field name
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zw_avro_field_get_null(avro_value_t *record_value, const char *field_name);


/**
 * @brief Set boolean value to Avro record value
 * @param[in]     record_value      Avro record value
 * @param[in]     field_name        Field name
 * @param[in]     boolean_value     Boolean value
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zw_avro_field_set_boolean(avro_value_t *record_value, const char *field_name, int boolean_value);

/**
 * @brief Set int32_t integer value to Avro record value
 * @param[in]     record_value      Avro record value
 * @param[in]     field_name        Field name
 * @param[in]     integer_value     int32_t integer value
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zw_avro_field_set_int(avro_value_t *record_value, const char *field_name, int32_t integer_value);

/**
 * @brief Set int64_t integer value to Avro record value
 * @param[in]     record_value      Avro record value
 * @param[in]     field_name        Field name
 * @param[in]     long_value        int64_t integer value
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zw_avro_field_set_long(avro_value_t *record_value, const char *field_name, int64_t long_value);

/**
 * @brief Set string value to Avro record value
 * @param[in]     record_value      Avro record value
 * @param[in]     field_name        Field name
 * @param[in]     string_value      String value
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zw_avro_field_set_string(avro_value_t *record_value, const char *field_name, char *string_value);

/**
 * @brief Set fixed length value to Avro record value
 * @param[in]     record_value      Avro record value
 * @param[in]     field_name        Field name
 * @param[in]     buffer            Fixed length value
 * @param[in]     size              Length of the fixed length value in @a buffer
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zw_avro_field_set_fixed(avro_value_t *record_value, const char *field_name, void *buffer, size_t size);

/**
 * @brief Set byte stream value to Avro record value
 * @param[in]     record_value      Avro record value
 * @param[in]     field_name        Field name
 * @param[in]     buffer            Byte stream value
 * @param[in]     size              Length of the byte stream value in @a buffer
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zw_avro_field_set_bytes(avro_value_t *record_value, const char *field_name, void *buffer, size_t size);

/**
 * @brief Set null value to Avro record value
 * @param[in]     record_value      Avro record value
 * @param[in]     field_name        Field name
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zw_avro_field_set_null(avro_value_t *record_value, const char *field_name);

#if 0
zwp_status_t zw_avro_get_type_from_type_name(const char *type_name, zw_avro_type_t *type_p);
#endif

/**
 * @brief Allocate and initialize a new Avro object
 * @param[out]    avro_p            The new Avro object
 * @param[in]     config_dir        Application configuration directory
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zw_avro_exit() to free resources
 */
zwp_status_t zw_avro_init(zw_avro_t *avro_p, char *config_dir);

/**
 * @brief Uninitialize and free Avro object
 * @param[in]     avro              Avro object
 * @return        Nothing
 * @see zw_avro_init
 */
void zw_avro_exit(zw_avro_t avro);
/** }@ */
#endif /* ZW_AVRO_H_ */
