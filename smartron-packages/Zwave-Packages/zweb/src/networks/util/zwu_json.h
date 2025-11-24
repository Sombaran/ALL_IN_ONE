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
 * @file    zwu_json.h
 *
 * @brief   JSON
 * @details Handles JSON
 *
 * @author  Angela
 *
 * @version 1.0 - 2016-07-12
 * - Initial version
 */

#ifndef ZWU_JSON_H_
#define ZWU_JSON_H_

#include <stdio.h>
#include <stdint.h>

#include "zwu_status.h"
#include "cJSON.h"

/**
 * @defgroup zwu_json JSON
 * @{
 * @ingroup util
 */

/** JSON type */
typedef cJSON * zwu_json_t;

/** Format specifier for JSON integer */
#define ZWH_JSON_INT_T_FMT                        PRId64

/** JSON Type: False */
#define ZWU_JSON_TYPE_FALSE                       cJSON_False
/** JSON Type: True */
#define ZWU_JSON_TYPE_TRUE                        cJSON_True
/** JSON Type: NULL */
#define ZWU_JSON_TYPE_NULL                        cJSON_NULL
/** JSON Type: Number */
#define ZWU_JSON_TYPE_NUMBER                      cJSON_Number
/** JSON Type: String */
#define ZWU_JSON_TYPE_STRING                      cJSON_String
/** JSON Type: Array */
#define ZWU_JSON_TYPE_ARRAY                       cJSON_Array
/** JSON Type: Object */
#define ZWU_JSON_TYPE_OBJECT                      cJSON_Object

/** Create JSON item NULL */
#define zwu_json_create_null                      cJSON_CreateNull
/** Create JSON item True */
#define zwu_json_create_true                      cJSON_CreateTrue
/** Create JSON item False */
#define zwu_json_create_false                     cJSON_CreateFalse
/** Create JSON item Boolean */
#define zwu_json_create_bool                      cJSON_CreateBool
/** Create JSON item Number */
#define cJSON_CreateNumber                        cJSON_CreateNumber
/** Create JSON item String */
#define zwu_json_create_string                    cJSON_CreateString
/** Create JSON item Array */
#define zwu_json_create_array                     cJSON_CreateArray
/** Create JSON item Object */
#define zwu_json_create_object                    cJSON_CreateObject

/** Parse JSON string into JSON item */
#define zwu_json_parse                            cJSON_Parse
/** Delete JSON item */
#define zwu_json_delete                           cJSON_Delete

/** Add JSON item to JSON object */
#define zwu_json_add_item_to_object               cJSON_AddItemToObject
/** Add JSON item to JSON array */
#define zwu_json_add_item_to_array                cJSON_AddItemToArray

/** Add null to JSON object */
#define zwu_json_add_null_to_object					cJSON_AddNullToObject
/** Add true to JSON object */
#define	zwu_json_add_true_to_object					cJSON_AddTrueToObject
/** Add false to JSON object */
#define	zwu_json_add_false_to_object				cJSON_AddFalseToObject	
/** Add bool to JSON object */
#define	zwu_json_add_bool_to_object					cJSON_AddBoolToObject
/** Add number to JSON object */
#define	zwu_json_add_number_to_object				cJSON_AddNumberToObject
/** Add string to JSON object */
#define	zwu_json_add_string_to_object				cJSON_AddStringToObject	


/** Add reference of JSON item to JSON object */
#define zwu_json_add_item_reference_to_object     cJSON_AddItemReferenceToObject
/** Add reference of JSON item to JSON array */
#define zwu_json_add_item_reference_to_array      cJSON_AddItemReferenceToArray

/** Get JSON array size */
#define zwu_json_get_array_size                   cJSON_GetArraySize
/** Get JSON item from JSON array */
#define zwu_json_get_array_item                   cJSON_GetArrayItem
/** Get JSON item from JSON object */
#define zwu_json_get_object_item                  cJSON_GetObjectItem
/** Get number of JSON items in JSON object */
#define zwu_json_get_object_item_count            cJSON_GetArraySize
/** Get JSON item from JSON object by index */
#define zwu_json_get_object_item_by_index         cJSON_GetArrayItem

/** Detach JSON item from JSON array */
#define zwu_json_detach_item_from_array           cJSON_DetachItemFromArray
/** Detach JSON item from JSON object */
#define zwu_json_detach_item_from_object          cJSON_DetachItemFromObject
/** Detach JSON item from JSON object by index */
#define zwu_json_detach_item_from_object_by_index cJSON_DetachItemFromArray

/** Print JSON item as string (formatted) */
#define zwu_json_print                            cJSON_Print
/** Print JSON item as string (unformatted) */
#define zwu_json_print_unformatted                cJSON_PrintUnformatted

/** Get integer value from JSON item */
#define zwu_json_value_int(number)                (number->valueint)
/** Get float value from JSON item */
#define zwu_json_value_float(number)              (number->valuefloat)
/** Get string value from JSON item */
#define zwu_json_value_string(text)               (text->valuestring)

//void zwu_json_value_string_set_max_length(zwu_json_t string_json, int max_length);

/**
 * @brief Initialize JSON library
 * @return        Nothing
 * @see zwu_json_exit
 */
void zwu_json_init();

/**
 * @brief Exit JSON library
 * @return        Nothing
 * @see zwu_json_init
 */
void zwu_json_exit();

/**
 * @}
 */

#endif /* ZWU_JSON_H_ */
