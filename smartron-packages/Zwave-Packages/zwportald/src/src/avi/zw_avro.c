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
 * @file    zw_avro.c
 *
 * @brief   Avro
 * @details Handles Avro schema and data
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-14
 * - Initial version
 */

#include "zw_avro.h"
#include "zwp_log.h"
#include "zwp_memory.h"
#include "zwp_error.h"
#include "zwp_io.h"

/** Avro schema file for Z-Wave */
#define ZW_SCHEMA_ZWAVE "zwave.avsc"

/**
 * @brief Avro class
 */
struct _zw_avro
{
	avro_schema_t              protocol_schema;               /**< Protocol schema  */

	avro_schema_t              types_schema;                  /**< Types schema  */
	avro_value_iface_t         *types_iface;                  /**< Types interface */

	avro_value_iface_t         *cookies_iface;                /**< Cookies interface */

	avro_schema_t              messages_schema;               /**< Messages schema  */
};

/**
 * @name Getters
 * @{
 */
avro_schema_t zw_avro_get_protocol_schema(zw_avro_t avro)
{
	return avro->protocol_schema;
}

avro_schema_t zw_avro_get_types_schema(zw_avro_t avro)
{
	return avro->types_schema;
}

avro_value_iface_t *zw_avro_get_types_iface(zw_avro_t avro)
{
	return avro->types_iface;
}

avro_value_iface_t *zw_avro_get_cookies_iface(zw_avro_t avro)
{
	return avro->cookies_iface;
}

avro_schema_t zw_avro_get_messages_schema(zw_avro_t avro)
{
	return avro->messages_schema;
}
/**
 * @}
 */

zwp_status_t zw_avro_field_get_boolean(avro_value_t *record_value, const char *field_name, int *boolean_value_p)
{
	avro_value_t field_value;

	if (avro_value_get_by_name(record_value, field_name, &field_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field '%s': %s", field_name, avro_strerror());
		goto l_err_get_field_value;
	}

	if (avro_value_get_boolean(&field_value, boolean_value_p) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get boolean value of the field '%s': %s", field_name, avro_strerror);
		goto l_err_get_value;
	}

	return ZWP_STATUS_OK;

l_err_get_value:
l_err_get_field_value:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zw_avro_field_get_int(avro_value_t *record_value, const char *field_name, int32_t *integer_value_p)
{
	avro_value_t field_value;

	if (avro_value_get_by_name(record_value, field_name, &field_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field '%s': %s", field_name, avro_strerror());
		goto l_err_get_field_value;
	}

	if (avro_value_get_int(&field_value, integer_value_p) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get integer value of the field '%s': %s", field_name, avro_strerror());
		goto l_err_get_value;
	}

	return ZWP_STATUS_OK;

l_err_get_value:
l_err_get_field_value:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zw_avro_field_get_long(avro_value_t *record_value, const char *field_name, int64_t *long_value_p)
{
	avro_value_t field_value;

	if (avro_value_get_by_name(record_value, field_name, &field_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field '%s': %s", field_name, avro_strerror());
		goto l_err_get_field_value;
	}

	if (avro_value_get_long(&field_value, long_value_p) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get integer value of the field '%s': %s", field_name, avro_strerror());
		goto l_err_get_value;
	}

	return ZWP_STATUS_OK;

l_err_get_value:
l_err_get_field_value:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zw_avro_field_get_string(avro_value_t *record_value, const char *field_name, char **string_value_p)
{
	avro_value_t field_value;
	const char *string;
	size_t size;

	if (avro_value_get_by_name(record_value, field_name, &field_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field '%s': %s", field_name, avro_strerror());
		goto l_err_get_field_value;
	}

	if (avro_value_get_string(&field_value, &string, &size) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get string value of the field '%s': %s", field_name, avro_strerror());
		goto l_err_get_value;
	}

	if ((*string_value_p = ZWP_STRDUP(string)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_dup_string;
	}

	return ZWP_STATUS_OK;

l_err_dup_string:
l_err_get_value:
l_err_get_field_value:
	return ZWP_STATUS_ERROR;
}

void zw_avro_field_free_string(char *string_value)
{
	ZWP_FREE(string_value);
}

zwp_status_t zw_avro_field_get_fixed(avro_value_t *record_value, const char *field_name, void **buffer_p, size_t *size_p)
{
	avro_value_t field_value;
	const void *buffer;
	size_t size;

	if (avro_value_get_by_name(record_value, field_name, &field_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field '%s': %s", field_name, avro_strerror());
		goto l_err_get_field_value;
	}

	if (avro_value_get_fixed(&field_value, &buffer, &size) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get fixed value in the field '%s': %s",
				field_name, avro_strerror());
		goto l_err_get_value;
	}

	if ((*buffer_p = ZWP_MALLOC(size * sizeof(uint8_t))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_alloc_buffer;
	}

	memcpy(*buffer_p, buffer, size);
	*size_p = size;

	return ZWP_STATUS_OK;

l_err_alloc_buffer:
l_err_get_value:
l_err_get_field_value:
	return ZWP_STATUS_ERROR;
}

void zw_avro_field_free_fixed(void *buffer)
{
	ZWP_FREE(buffer);
}

zwp_status_t zw_avro_field_get_bytes(avro_value_t *record_value, const char *field_name, void **buffer_p, size_t *size_p)
{
	avro_value_t field_value;
	const void *buffer;
	size_t size;

	if (avro_value_get_by_name(record_value, field_name, &field_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field '%s': %s", field_name, avro_strerror());
		goto l_err_get_field_value;
	}

	if (avro_value_get_bytes(&field_value, &buffer, &size) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get bytes value in the field '%s': %s",
				field_name, avro_strerror());
		goto l_err_get_value;
	}

	if ((*buffer_p = ZWP_MALLOC(size * sizeof(uint8_t))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_alloc_buffer;
	}

	memcpy(*buffer_p, buffer, size);
	*size_p = size;

	return ZWP_STATUS_OK;

l_err_alloc_buffer:
l_err_get_value:
l_err_get_field_value:
	return ZWP_STATUS_ERROR;
}

void zw_avro_field_free_bytes(void *buffer)
{
	ZWP_FREE(buffer);
}

zwp_status_t zw_avro_field_get_null(avro_value_t *record_value, const char *field_name)
{
	avro_value_t field_value;

	if (avro_value_get_by_name(record_value, field_name, &field_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field '%s': %s", field_name, avro_strerror());
		goto l_err_get_field_value;
	}

	if (avro_value_get_null(&field_value) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get null value of the field '%s': %s", field_name, avro_strerror());
		goto l_err_get_value;
	}

	return ZWP_STATUS_OK;

l_err_get_value:
l_err_get_field_value:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zw_avro_field_set_boolean(avro_value_t *record_value, const char *field_name, int boolean_value)
{
	avro_value_t field_value;

	if (avro_value_get_by_name(record_value, field_name, &field_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field '%s': %s", field_name, avro_strerror());
		goto l_err_get_field_value;
	}

	if (avro_value_set_boolean(&field_value, boolean_value) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to set value '%d' to the field '%s': %s", boolean_value, field_name, avro_strerror());
		goto l_err_set_value;
	}

	return ZWP_STATUS_OK;

l_err_set_value:
l_err_get_field_value:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zw_avro_field_set_int(avro_value_t *record_value, const char *field_name, int32_t integer_value)
{
	avro_value_t field_value;

	if (avro_value_get_by_name(record_value, field_name, &field_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field '%s': %s", field_name, avro_strerror());
		goto l_err_get_field_value;
	}

	if (avro_value_set_int(&field_value, integer_value) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to set value '%d' to the field '%s': %s", integer_value, field_name, avro_strerror());
		goto l_err_set_value;
	}

	return ZWP_STATUS_OK;

l_err_set_value:
l_err_get_field_value:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zw_avro_field_set_long(avro_value_t *record_value, const char *field_name, int64_t long_value)
{
	avro_value_t field_value;

	if (avro_value_get_by_name(record_value, field_name, &field_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field '%s': %s", field_name, avro_strerror());
		goto l_err_get_field_value;
	}

	if (avro_value_set_long(&field_value, long_value) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to set value '%ld' to the field '%s': %s", long_value, field_name, avro_strerror());
		goto l_err_set_value;
	}

	return ZWP_STATUS_OK;

l_err_set_value:
l_err_get_field_value:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zw_avro_field_set_string(avro_value_t *record_value, const char *field_name, char *string_value)
{
	avro_value_t field_value;

	if (avro_value_get_by_name(record_value, field_name, &field_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field '%s': %s", field_name, avro_strerror());
		goto l_err_get_field_value;
	}

	if (avro_value_set_string(&field_value, string_value) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to set value '%s' to the field '%s': %s", string_value, field_name, avro_strerror());
		goto l_err_set_value;
	}

	return ZWP_STATUS_OK;

l_err_set_value:
l_err_get_field_value:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zw_avro_field_set_fixed(avro_value_t *record_value, const char *field_name, void *buffer, size_t size)
{
	avro_value_t field_value;

	if (avro_value_get_by_name(record_value, field_name, &field_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field '%s': %s", field_name, avro_strerror());
		goto l_err_get_field_value;
	}

	if (avro_value_set_fixed(&field_value, buffer, size) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to set fixed value of size '%d' in the field '%s': %s",
				size, field_name, avro_strerror());
		goto l_err_set_value;
	}

	return ZWP_STATUS_OK;

l_err_set_value:
l_err_get_field_value:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zw_avro_field_set_bytes(avro_value_t *record_value, const char *field_name, void *buffer, size_t size)
{
	avro_value_t field_value;

	if (avro_value_get_by_name(record_value, field_name, &field_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field '%s': %s", field_name, avro_strerror());
		goto l_err_get_field_value;
	}

	// Avro doesn't accept (buffer == NULL) even when (buffer_size == 0)
	if ((size == 0) && (buffer == NULL))
	{
		buffer = "";
	}

	if (avro_value_set_bytes(&field_value, buffer, size) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to set bytes value of size '%d' in the field '%s': %s",
				size, field_name, avro_strerror());
		goto l_err_set_value;
	}

	return ZWP_STATUS_OK;

l_err_set_value:
l_err_get_field_value:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zw_avro_field_set_null(avro_value_t *record_value, const char *field_name)
{
	avro_value_t field_value;

	if (avro_value_get_by_name(record_value, field_name, &field_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field '%s': %s", field_name, avro_strerror());
		goto l_err_get_field_value;
	}

	if (avro_value_set_null(&field_value) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to set null value to the field '%s': %s", field_name, avro_strerror());
		goto l_err_set_value;
	}

	return ZWP_STATUS_OK;

l_err_set_value:
l_err_get_field_value:
	return ZWP_STATUS_ERROR;
}

#if 0
zwp_status_t zw_avro_get_type_from_type_name(const char *type_name, zw_avro_type_t *type_p)
{
	int i;
	int count;

	count = sizeof(zw_avro_type_name_mapping) / sizeof(*zw_avro_type_name_mapping);

	for (i = 0; i < count; i++)
	{
		if (!strcmp(zw_avro_type_name_mapping[i].avro_type_name, type_name))
		{
			*type_p = zw_avro_type_name_mapping[i].avro_type;
			return ZWP_STATUS_OK;
		}
	}

	ZWP_LOG(ZWP_LOG_ERR, "Failed to match with supported avro types for the type name '%s'", type_name);
	return ZWP_STATUS_ERROR;
}
#endif

/**
 * @brief Get Avro schema object from Avro schema file
 * @param[in]     path              Avro schema file path
 * @param[out]    schema            Avro schema object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zw_avro_release_schema() to free resources
 */
static zwp_status_t zw_avro_get_schema_from_file(const char *path, avro_schema_t *schema)
{
	avro_schema_error_t schema_error;
	char *json_text;
	int read;

	if (zwp_read_from_file_path_with_dest_alloc(path, &json_text, &read, 0) != ZWP_STATUS_OK)
	{
		goto l_err_read_file;
	}

	if (avro_schema_from_json(json_text, read, schema, &schema_error) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get schema from JSON text of file '%s': %s", path, avro_strerror());
		goto l_err_schema_from_json;
	}

	zwp_read_dest_free(json_text);

	return ZWP_STATUS_OK;

l_err_schema_from_json:
	zwp_read_dest_free(json_text);
l_err_read_file:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Avro schema object
 * @param[in]     schema            Avro schema object
 * @return        Nothing
 * @see zw_avro_get_schema_from_file
 */
static void zw_avro_release_schema(avro_schema_t schema)
{
	avro_schema_decref(schema);
}

zwp_status_t zw_avro_init(zw_avro_t *avro_p, char *config_dir)
{
	static zw_avro_t g_avro;
	zw_avro_t avro;
	char *schema_path;
	avro_schema_t cookies_schema;

	//Enforce singleton
	if (g_avro)
	{
		ZWP_LOG(ZWP_LOG_WARNING, ZWP_MSG_SINGLETON_MULTIPLE_CALLS);
		*avro_p = g_avro;

		return ZWP_STATUS_OK;
	}

	if ((avro = (zw_avro_t)ZWP_MALLOC(sizeof(*avro))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_avro_alloc;
	}

	if ((schema_path = (char *)ZWP_MALLOC(strlen(config_dir) + 1 + sizeof(ZW_SCHEMA_ZWAVE))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_schema_path_alloc;
	}
	sprintf(schema_path, "%s/%s", config_dir, ZW_SCHEMA_ZWAVE);

	if (zw_avro_get_schema_from_file(schema_path, &avro->protocol_schema) != ZWP_STATUS_OK)
	{
		goto l_err_schema_from_file;
	}

	if ((avro->types_schema = avro_schema_get_subschema(avro->protocol_schema, "types")) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the 'types' subschema: %s", avro_strerror());
		goto l_err_types_schema;
	}

	if ((avro->types_iface = avro_generic_class_from_schema(avro->types_schema)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'Types' interface from schema: %s", avro_strerror());
		goto l_err_types_interface;
	}

	if ((cookies_schema = avro_schema_union_branch_by_name(avro->types_schema, NULL, "Cookies")) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'Cookies' schema from 'types' schema: %s", avro_strerror());
		goto l_err_cookies_schema;
	}

	if ((avro->cookies_iface = avro_generic_class_from_schema(cookies_schema)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'Cookie' interface from schema: %s", avro_strerror());
		goto l_err_cookies_interface;
	}

	if ((avro->messages_schema = avro_schema_get_subschema(avro->protocol_schema, "messages")) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the 'messages' subschema: %s", avro_strerror());
		goto l_err_messages_schema;
	}

	ZWP_FREE(schema_path);

	g_avro = *avro_p = avro;

	return ZWP_STATUS_OK;

l_err_messages_schema:
	avro_value_iface_decref(avro->cookies_iface);
l_err_cookies_interface:
l_err_cookies_schema:
	avro_value_iface_decref(avro->types_iface);
l_err_types_interface:
l_err_types_schema:
	zw_avro_release_schema(avro->protocol_schema);
l_err_schema_from_file:
	ZWP_FREE(schema_path);
l_err_schema_path_alloc:
	ZWP_FREE(avro);
l_err_avro_alloc:
	return ZWP_STATUS_ERROR;
}

void zw_avro_exit(zw_avro_t avro)
{
	avro_value_iface_decref(avro->cookies_iface);

	avro_value_iface_decref(avro->types_iface);

	zw_avro_release_schema(avro->protocol_schema);

	ZWP_FREE(avro);
}



