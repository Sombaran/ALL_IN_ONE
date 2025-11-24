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
 * @file    zwp_ifttt.c
 *
 * @brief   IFTTT
 * @details IFTTT setup
 *
 * @author  Kumara
 *
 * @version 1.0 - 2017-02-03
 * - Initial version
 */

#include <unistd.h>

#include "zwp_ifttt.h"
#include "zwp_db.h"
#include "zwp_db_internal.h"
#include "zwp_memory.h"
#include "zwp_error.h"
#include "zwp_log.h"
#include "zwu_json.h"
#include "zwp_thread.h"
#include "zwp_thread_mutex.h"
#include "zwu_http_client.h"

#define ZWP_OPTION_DEFAULT_REALTIME_API_POST_INTERVAL                    1000
#define ZWP_OPTION_DEFAULT_REALTIME_API_POST_RETRY                       2

#define ZWP_IFTTT_REALTIME_NOTIFICATION_URL                              "https://realtime.ifttt.com/v1/notifications"

#define ZWP_IFTTT_REALTIME_NOTIFICATION_MAXIMUM_ITEMS                    1000

#define ZWP_IFTTT_DB_TABLE_REALTIME_NOTIFICATION_NAME                    "realtime_notification"

#define ZWP_IFTTT_DB_COLUMN_TRIGGER_IDENTITY_NAME                        "trigger_identity"
#define ZWP_IFTTT_DB_COLUMN_TRIGGER_IDENTITY_MAX_LENGTH                  300

#define ZWP_ITTTT_DB_CLOUMN_NOTIFICATION_STATE_NAME                      "notification_state"
#define ZWP_IFTTT_DB_COLUMN_NOTIFICATION_STATE_PENDING                   0
#define ZWP_IFTTT_DB_COLUMN_NOTIFICATION_STATE_PROCESSING                1

#define ZWP_IFTTT_REALTIME_NOTIFICATION_THREAD_NAME                      "AIFTTTNotif"

/** Realtime Notification handler is down */
#define ZWP_IFTTT_REALTIME_NOTIFICATION_HANDLER_STATE_DOWN               0
/** Realtime Notification handler is up  */
#define ZWP_IFTTT_REALTIME_NOTIFICATION_HANDLER_STATE_UP                 1
/** Realtime Notification handler is shutting down  */
#define ZWP_IFTTT_REALTIME_NOTIFICATION_HANDLER_STATE_SHUTTING_DOWN      2

/** Printing/serializing JSON */
#define ZWU_JSON_PRINT                zwu_json_print_unformatted

struct _zwp_ifttt_config
{
	char *                       local_state_dir;                           /**< Directory for storing local state */
	char *                       channel_key;                               /**< Channel key */
	useconds_t                   realtime_api_post_interval;                /**< Realtime API POST Interval (in microseconds) */
	int                          realtime_api_post_retry;                   /**< Realtime API POST Retry */
};

/**
 * @brief Directory Service class
 */
struct _zwp_ifttt
{
	zwp_db_db *					 db;					                    /**< Common database for DB */
	zwp_db_table *				 realtime_notification_table;               /**< Realtime notification table */

	zwp_pool_t                   memory_pool;                               /**< Memory pool for the IFTTT module */
	zwp_thread_t                 realtime_notification_handler;             /**< Realtime Notification handler */

	int                          realtime_notification_pending;             /**< State to indicate realtime notification is pending */
	int                          realtime_notification_handler_state;       /**< Realtime Notification handler state - ZWP_IFTTT_REALTIME_NOTIFICATION_HANDLER_STATE_XXX */
	zwp_thread_mutex_t           realtime_notification_handler_state_lock;  /**< Mutex lock for network initialization state */

	zwu_http_client_t            http_client;                               /**< HTTP client handle */

	zwp_ifttt_config_t           ifttt_config;                              /**< Directory service configuration */
};

static zwp_db_table_head zwp_ifttt_realtime_notification_table_head[] =
{
	{ ZWP_IFTTT_DB_COLUMN_TRIGGER_IDENTITY_NAME,        ZWP_DB_DATATYPE_TEXT,   ZWP_IFTTT_DB_COLUMN_TRIGGER_IDENTITY_MAX_LENGTH,   ZWP_DB_PROPERTY_PRIMARY_KEY },
	{ ZWP_ITTTT_DB_CLOUMN_NOTIFICATION_STATE_NAME,      ZWP_DB_DATATYPE_INT,    sizeof(int),                                       ZWP_DB_PROPERTY_PRIMARY_KEY }
};

typedef enum
{
	ZWP_REALTIME_NOTIFICATION_TABLE_INDEX_TRIGGER_IDENTITY = 0,
	ZWP_REALTIME_NOTIFICATION_TABLE_INDEX_NOTIFICATION_STATE,
} zwp_ifttt_realtime_notification_table_index;

zwp_db_table *zwp_ifttt_get_realtime_notification_table(zwp_ifttt_t ifttt)
{
	return ifttt->realtime_notification_table;
}

zwp_status_t zwp_ifttt_realtime_notification_http_client_init(zwp_ifttt_t ifttt)
{
	char *channel_key_header;
	int length;

	if (zwu_http_client_create(&ifttt->http_client) != ZWU_STATUS_OK)
	{
		goto l_err_http_client_create;
	}

#ifdef ZWP_DEBUG
	zwu_http_client_set_verbose(ifttt->http_client, ZWU_HTTP_CLIENT_VERBOSE_ON);
#endif

	// Get the length of formatted string
	if ((length = snprintf(NULL, 0, "IFTTT-Channel-Key: %s", ifttt->ifttt_config->channel_key)) < 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get length of formatted string");
		goto l_err_channel_key_header_length;
	}

	if ((channel_key_header = (char *)ZWP_MALLOC(length + 1)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_channel_key_header_alloc;
	}

	sprintf(channel_key_header, "IFTTT-Channel-Key: %s", ifttt->ifttt_config->channel_key);

	if ((zwu_http_client_set_http_header(ifttt->http_client, channel_key_header) != ZWU_STATUS_OK)
			|| (zwu_http_client_set_http_header(ifttt->http_client, "Accept: application/json") != ZWU_STATUS_OK)
			|| (zwu_http_client_set_http_header(ifttt->http_client, "Accept-Charset: utf-8") != ZWU_STATUS_OK)
			|| (zwu_http_client_set_http_header(ifttt->http_client, "Content-Type: application/json") != ZWU_STATUS_OK))
	{
		goto l_err_set_http_header;
	}

	if (zwu_http_client_set_url(ifttt->http_client, ZWP_IFTTT_REALTIME_NOTIFICATION_URL) != ZWU_STATUS_OK)
	{
		goto l_err_set_url;
	}

	ZWP_FREE(channel_key_header);

	return ZWP_STATUS_OK;

l_err_set_url:
l_err_set_http_header:
	ZWP_FREE(channel_key_header);
l_err_channel_key_header_alloc:
l_err_channel_key_header_length:
	zwu_http_client_destroy(ifttt->http_client);
l_err_http_client_create:
	return ZWP_STATUS_ERROR;
}

void zwp_ifttt_realtime_notification_http_client_exit(zwp_ifttt_t ifttt)
{
	zwu_http_client_destroy(ifttt->http_client);
}

zwp_status_t zwp_ifttt_field_trigger_identity_set_query_value(zwp_db_table_value *field)
{
	field->pcColumnName = ZWP_IFTTT_DB_COLUMN_TRIGGER_IDENTITY_NAME;
	field->eType = ZWP_DB_DATATYPE_QUERY;

	// Get the length of formatted string
	if ((field->isize = snprintf(NULL, 0, "(SELECT %s FROM %s LIMIT %d)",
			ZWP_IFTTT_DB_COLUMN_TRIGGER_IDENTITY_NAME, ZWP_IFTTT_DB_TABLE_REALTIME_NOTIFICATION_NAME,
			ZWP_IFTTT_REALTIME_NOTIFICATION_MAXIMUM_ITEMS)) < 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get length of formatted string");
		goto l_err_length;
	}

	if ((field->data.valuestring = (char *)ZWP_MALLOC(field->isize + 1)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_value_alloc;
	}

	sprintf(field->data.valuestring, "(SELECT %s FROM %s LIMIT %d)",
			ZWP_IFTTT_DB_COLUMN_TRIGGER_IDENTITY_NAME, ZWP_IFTTT_DB_TABLE_REALTIME_NOTIFICATION_NAME,
			ZWP_IFTTT_REALTIME_NOTIFICATION_MAXIMUM_ITEMS);

	return ZWP_STATUS_OK;

	ZWP_FREE(field->data.valuestring);
l_err_value_alloc:
l_err_length:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_ifttt_field_trigger_identity_set_value(zwp_db_table_value *field, char* trigger_identity)
{
	field->pcColumnName = ZWP_IFTTT_DB_COLUMN_TRIGGER_IDENTITY_NAME;
	field->eType = ZWP_DB_DATATYPE_TEXT;
	field->isize = strlen(trigger_identity);
	if ((field->data.valuestring = ZWP_STRDUP(trigger_identity)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_value_alloc;
	}

	return ZWP_STATUS_OK;

	ZWP_FREE(field->data.valuestring);
l_err_value_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_ifttt_field_notification_state_set_value(zwp_db_table_value *field, int notification_state)
{
	field->pcColumnName = ZWP_ITTTT_DB_CLOUMN_NOTIFICATION_STATE_NAME;
	field->eType = ZWP_DB_DATATYPE_INT;
	field->isize = sizeof(int);
	field->data.valueint = notification_state;
}

void zwp_ifttt_field_unset_value(zwp_db_table_value *field)
{
	if ((field->eType == ZWP_DB_DATATYPE_BLOB))
	{
		if (field->isize != 0)
		{
			ZWP_FREE(field->data.valuebyte);
		}
	}
	else if ((field->eType == ZWP_DB_DATATYPE_TEXT)
			|| (field->eType == ZWP_DB_DATATYPE_QUERY))
	{
		ZWP_FREE(field->data.valuestring);
	}
}

static zwp_status_t zwp_ifttt_table_realtime_notification_record_alloc(char *trigger_identity, int notification_state, int *column_count_p, zwp_db_table_value **record_p)
{
	int column_count;
	zwp_db_table_value *record;

	column_count = sizeof(zwp_ifttt_realtime_notification_table_head) / sizeof(zwp_db_table_head);

	if ((record = (zwp_db_table_value *)ZWP_CALLOC(column_count, sizeof(zwp_db_table_value))) == NULL)
	{
		goto l_err_record_alloc;
	}

	if (zwp_ifttt_field_trigger_identity_set_value(&record[ZWP_REALTIME_NOTIFICATION_TABLE_INDEX_TRIGGER_IDENTITY], trigger_identity) != ZWP_STATUS_OK)
	{
		goto l_err_field_trigger_identity_set_value;
	}

	zwp_ifttt_field_notification_state_set_value(&record[ZWP_REALTIME_NOTIFICATION_TABLE_INDEX_NOTIFICATION_STATE], notification_state);

	*column_count_p = column_count;
	*record_p = record;

	return ZWP_STATUS_OK;


	zwp_ifttt_field_unset_value(&record[ZWP_REALTIME_NOTIFICATION_TABLE_INDEX_NOTIFICATION_STATE]);
	zwp_ifttt_field_unset_value(&record[ZWP_REALTIME_NOTIFICATION_TABLE_INDEX_TRIGGER_IDENTITY]);
l_err_field_trigger_identity_set_value:
	ZWP_FREE(record);
l_err_record_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_ifttt_table_realtime_notification_record_free(int column_count, zwp_db_table_value *record)
{
	int i;

	if (record)
	{
		for (i = 0; i < column_count; i++)
		{
			zwp_ifttt_field_unset_value(&record[i]);
		}

		ZWP_FREE(record);
	}
}

zwp_status_t zwp_ifttt_table_realtime_notification_record_insert(zwp_ifttt_t ifttt, char *trigger_identity, int notification_state)
{
	zwp_status_t return_status;
	int column_count;
	zwp_db_table_value *record;

	if (zwp_ifttt_table_realtime_notification_record_alloc(trigger_identity, notification_state, &column_count, &record) != ZWP_STATUS_OK)

	{
		ZWP_LOG(ZWP_LOG_ERR, "Fail to generate record for realtime notifications table");
		goto l_err_generate_record;
	}

	if ((return_status = zwp_db_insert_record(ifttt->realtime_notification_table, column_count, record)) != ZWP_STATUS_OK)
	{
		// Ignore failure due to primary key check (i.e, attempt to insert duplicates)
		if (return_status != ZWP_DB_ERROR_DB_CONSTRAINT)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Fail to insert record to realtime notifications table: %s", zwp_db_get_table_errortext(ifttt->realtime_notification_table));
			goto l_err_insert_record;
		}
	}

	zwp_ifttt_table_realtime_notification_record_free(column_count, record);

	return ZWP_STATUS_OK;

l_err_insert_record:
	zwp_ifttt_table_realtime_notification_record_free(column_count, record);
l_err_generate_record:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_ifttt_realtime_notification_trigger_add(zwp_ifttt_t ifttt, char *trigger_identity)
{
	if (zwp_thread_mutex_lock(ifttt->realtime_notification_handler_state_lock) != ZWP_STATUS_OK)
	{
		goto l_err_realtime_notification_handler_state_lock;
	}

	if (ifttt->realtime_notification_handler_state != ZWP_IFTTT_REALTIME_NOTIFICATION_HANDLER_STATE_UP)
	{
		goto l_err_realtime_notification_handler_not_up;
	}

	if (zwp_ifttt_table_realtime_notification_record_insert(ifttt, trigger_identity, ZWP_IFTTT_DB_COLUMN_NOTIFICATION_STATE_PENDING) != ZWP_STATUS_OK)
	{
		goto l_err_realtime_notification_record_insert;
	}

	ifttt->realtime_notification_pending = 1;

	zwp_thread_mutex_unlock(ifttt->realtime_notification_handler_state_lock);

	return ZWP_STATUS_OK;

l_err_realtime_notification_record_insert:
l_err_realtime_notification_handler_not_up:
	zwp_thread_mutex_unlock(ifttt->realtime_notification_handler_state_lock);
l_err_realtime_notification_handler_state_lock:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_ifttt_realtime_notification_start_notification(zwp_ifttt_t ifttt, int *records_count_p)
{
	zwp_status_t return_status;
	zwp_db_table_value query_condition;
	int query_condition_column_count;
	zwp_db_table_value update_set;
	int update_set_column_count;
	int updated_records_count;

	if (zwp_ifttt_field_trigger_identity_set_query_value(&query_condition) != ZWP_STATUS_OK)
	{
		goto l_err_field_trigger_identity_set_value;
	}
	query_condition_column_count = 1;

	zwp_ifttt_field_notification_state_set_value(&update_set, ZWP_IFTTT_DB_COLUMN_NOTIFICATION_STATE_PROCESSING);
	update_set_column_count = 1;

	if ((return_status = zwp_db_update_record(ifttt->realtime_notification_table, query_condition_column_count, &query_condition,
			update_set_column_count, &update_set, &updated_records_count)) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Fail to update record in realtime notifications table: %s", zwp_db_get_table_errortext(ifttt->realtime_notification_table));
		goto l_err_update_record;
	}

	zwp_ifttt_field_unset_value(&update_set);
	zwp_ifttt_field_unset_value(&query_condition);

	*records_count_p = updated_records_count;

	return ZWP_STATUS_OK;

l_err_update_record:
	zwp_ifttt_field_unset_value(&update_set);
	zwp_ifttt_field_unset_value(&query_condition);
l_err_field_trigger_identity_set_value:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_ifttt_realtime_notification_complete_notification(zwp_ifttt_t ifttt)
{
	zwp_status_t return_status;
	zwp_db_table_value query_condition;
	int query_condition_column_count;

	zwp_ifttt_field_notification_state_set_value(&query_condition, ZWP_IFTTT_DB_COLUMN_NOTIFICATION_STATE_PROCESSING);
	query_condition_column_count = 1;

	if ((return_status = zwp_db_delete_record(ifttt->realtime_notification_table, query_condition_column_count, &query_condition)) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Fail to delete record(s) in realtime notifications table: %s", zwp_db_get_table_errortext(ifttt->realtime_notification_table));
		goto l_err_delete_record;
	}

	zwp_ifttt_field_unset_value(&query_condition);

	return ZWP_STATUS_OK;

l_err_delete_record:
	zwp_ifttt_field_unset_value(&query_condition);
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_ifttt_realtime_notification_get_processing_notification(zwp_ifttt_t ifttt, zwu_json_t *realtime_notification_list_json_p, int *record_count_p)
{
	zwp_status_t return_status;
	zwp_db_table_value query_condition;
	int query_condition_column_count;
	zwp_db_table_value *result_set;
	zwp_db_table_value *record;
	int result_set_column_count;
	int result_set_row_count;
	int i;
	zwu_json_t realtime_notification_list_json;
	zwu_json_t realtime_notification_json;
	zwu_json_t trigger_json;

	return_status = ZWP_STATUS_ERROR;
	realtime_notification_list_json = NULL;

	zwp_ifttt_field_notification_state_set_value(&query_condition, ZWP_IFTTT_DB_COLUMN_NOTIFICATION_STATE_PROCESSING);
	query_condition_column_count = 1;

	if (zwp_db_query_record(ifttt->realtime_notification_table, query_condition_column_count, &query_condition,
			ZWP_IFTTT_REALTIME_NOTIFICATION_MAXIMUM_ITEMS, 0, NULL,
			&result_set_column_count, &result_set_row_count, &result_set) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Fail to select/get record(s) from realtime notifications table: %s", zwp_db_get_table_errortext(ifttt->realtime_notification_table));
		goto l_err_query_record;
	}

	/*
	 * Note: If record_count == 0, realtime_notification_list_json returned is not an empty array but NULL. Returns ZWP_STATUS_OK
	 */
	if (result_set_row_count == 0)
	{
		*realtime_notification_list_json_p = realtime_notification_list_json;
		*record_count_p = result_set_row_count;
		return_status = ZWP_STATUS_OK;
		goto l_no_record;
	}

	//ZWP_LOG(ZWP_LOG_DEBUG, "Result Set Column Count: %d Result Set Row Count: %d", result_set_column_count, result_set_row_count);

	if ((realtime_notification_list_json = zwu_json_create_array()) == NULL)
	{
		goto l_err_realtime_notification_list_json_create;
	}

	for (i = 0; i < result_set_row_count; i++)
	{
		record = &result_set[i * result_set_column_count];

		if ((realtime_notification_json = zwu_json_create_object()) == NULL)
		{
			goto l_err_realtime_notification_json_create;
		}
		zwu_json_add_item_to_array(realtime_notification_list_json, realtime_notification_json);

		if ((trigger_json = zwu_json_create_string(record[ZWP_REALTIME_NOTIFICATION_TABLE_INDEX_TRIGGER_IDENTITY].data.valuestring)) == NULL)
		{
			goto l_err_trigger_json_create;
		}
		zwu_json_add_item_to_object(realtime_notification_json, "trigger_identity", trigger_json);
	}

	zwp_db_query_record_free(result_set_column_count, result_set_row_count, result_set);
	zwp_ifttt_field_unset_value(&query_condition);

	*realtime_notification_list_json_p = realtime_notification_list_json;
	*record_count_p = result_set_row_count;

	return ZWP_STATUS_OK;

l_err_trigger_json_create:
l_err_realtime_notification_json_create:
l_err_realtime_notification_list_json_create:
	zwu_json_delete(realtime_notification_list_json);
l_no_record:
	zwp_db_query_record_free(result_set_column_count, result_set_row_count, result_set);
l_err_query_record:
	zwp_ifttt_field_unset_value(&query_condition);
	return return_status;
}

zwp_status_t zwp_ifttt_realtime_notification_post(zwp_ifttt_t ifttt, zwu_json_t realtime_notification_list_json)
{
	zwu_json_t realtime_notification_request_json;
	char *realtime_notification_request;
	zwu_http_response_header_t realtime_notification_response_header;
	char *http_status_line;
	int return_code;
	int http_status_code;

	realtime_notification_request_json = NULL;
	realtime_notification_request = NULL;

	if ((realtime_notification_request_json = zwu_json_create_object()) == NULL)
	{
		zwu_json_delete(realtime_notification_list_json);
		goto l_err_realtime_notification_json_create;
	}
	zwu_json_add_item_to_object(realtime_notification_request_json, "data", realtime_notification_list_json);

	if ((realtime_notification_request = ZWU_JSON_PRINT(realtime_notification_request_json)) == NULL)
	{
		goto l_err_realtime_notification_request_json_print;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Realtime API Request Body: %s", realtime_notification_request);

	if (zwu_http_client_http_post(ifttt->http_client, realtime_notification_request, strlen(realtime_notification_request),
			&realtime_notification_response_header, NULL, NULL) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Realtime API HTTP POST failed");
		goto l_err_realtime_notification_post;
	}

	if (zwu_http_response_header_get_status_line(realtime_notification_response_header, &http_status_line) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get HTTP Status line");
		goto l_err_http_response_header_get_status_line;
	}

	if (((return_code = sscanf(http_status_line, "%*s %d %*s", &http_status_code)) == EOF) || (return_code != 1))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid HTTP Status line: %s", http_status_line);
		goto l_err_get_http_status_code;
	}

	if (http_status_code != 200)
	{
		if ((http_status_code >= 400) && (http_status_code < 500))
		{
			ZWP_LOG(ZWP_LOG_ERR, "4xx Client Error. Treated as successful attempt to prevent wasteful retry. HTTP Status line: %s", http_status_line);
		}
		else
		{
			ZWP_LOG(ZWP_LOG_ERR, "HTTP Status Code is neither 200 nor 4xx. HTTP Status line: %s", http_status_line);
			goto l_err_failed_http_status_code;
		}
	}

	zwu_http_response_header_destroy(realtime_notification_response_header);
	ZWP_FREE(realtime_notification_request);
	zwu_json_delete(realtime_notification_request_json);

	return ZWP_STATUS_OK;

l_err_failed_http_status_code:
l_err_get_http_status_code:
l_err_http_response_header_get_status_line:
	zwu_http_response_header_destroy(realtime_notification_response_header);
l_err_realtime_notification_post:
	ZWP_FREE(realtime_notification_request);
l_err_realtime_notification_request_json_print:
	zwu_json_delete(realtime_notification_request_json);
l_err_realtime_notification_json_create:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_ifttt_realtime_notification_process(zwp_ifttt_t ifttt, int *retry_count_p)
{
	zwu_json_t realtime_notification_list_json;
	int record_count;

	if (zwp_ifttt_realtime_notification_get_processing_notification(ifttt, &realtime_notification_list_json, &record_count) != ZWP_STATUS_OK)
	{
		goto l_err_ifttt_notification_get_processing_notification;
	}

	if (record_count > 0)
	{
		/*
		 * Note: realtime_notification_list_json is freed in zwp_ifttt_realtime_notification_post()
		 */
		if (zwp_ifttt_realtime_notification_post(ifttt, realtime_notification_list_json) != ZWP_STATUS_OK)
		{
			(*retry_count_p)--;
			if (*retry_count_p >= 0)
			{
				goto l_err_ifttt_realtime_notification_post;
			}
			//else if retry_count has expired, treat it as success and proceed to complete_notification (delete_record)
		}

		if (zwp_ifttt_realtime_notification_complete_notification(ifttt) != ZWP_STATUS_OK)
		{
			goto l_err_ifttt_notification_complete_notification;
		}
	}

	return ZWP_STATUS_OK;

l_err_ifttt_notification_complete_notification:
l_err_ifttt_realtime_notification_post:
l_err_ifttt_notification_get_processing_notification:
	return ZWP_STATUS_ERROR;
}

static void *ZWP_THREAD_FUNC zwp_ifttt_realtime_notification_handler(zwp_thread_t thread, void *arg)
{
	zwp_ifttt_t ifttt;
	zwp_status_t return_status;
	int record_count;
	int retry_count;

	ifttt = (zwp_ifttt_t)arg;

	zwp_thread_name_set(ZWP_IFTTT_REALTIME_NOTIFICATION_THREAD_NAME, NULL);

	if (zwp_ifttt_realtime_notification_http_client_init(ifttt) != ZWP_STATUS_OK)
	{
		goto l_err_realtime_notification_http_client_init;
	}

	retry_count = ifttt->ifttt_config->realtime_api_post_retry;
	record_count = 1;
	while (1)
	{
		return_status = (record_count > 0) ? zwp_ifttt_realtime_notification_process(ifttt, &retry_count) : ZWP_STATUS_OK;

		if (zwp_thread_mutex_lock(ifttt->realtime_notification_handler_state_lock) == ZWP_STATUS_OK)
		{
			if (ifttt->realtime_notification_handler_state != ZWP_IFTTT_REALTIME_NOTIFICATION_HANDLER_STATE_UP)
			{
				zwp_thread_mutex_unlock(ifttt->realtime_notification_handler_state_lock);
				break;
			}
			zwp_thread_mutex_unlock(ifttt->realtime_notification_handler_state_lock);
		}
		else
		{
			goto l_err_realtime_notification_handler_state_lock;
		}

		usleep(ifttt->ifttt_config->realtime_api_post_interval);

		/*
		 * Start next set of notification only when last notification process was successful.
		 */
		if (return_status == ZWP_STATUS_OK)
		{
			retry_count = ifttt->ifttt_config->realtime_api_post_retry;
			record_count = 0;

			if (zwp_thread_mutex_lock(ifttt->realtime_notification_handler_state_lock) == ZWP_STATUS_OK)
			{
				if (ifttt->realtime_notification_handler_state != ZWP_IFTTT_REALTIME_NOTIFICATION_HANDLER_STATE_UP)
				{
					zwp_thread_mutex_unlock(ifttt->realtime_notification_handler_state_lock);
					break;
				}

				if (ifttt->realtime_notification_pending)
				{
					if (zwp_ifttt_realtime_notification_start_notification(ifttt, &record_count) == ZWP_STATUS_OK)
					{
						if (record_count > 0)
						{
							ifttt->realtime_notification_pending = 0;
						}
					}
				}

				zwp_thread_mutex_unlock(ifttt->realtime_notification_handler_state_lock);
			}
			else
			{
				goto l_err_realtime_notification_handler_state_lock_2;
			}
		}
	}

	zwp_ifttt_realtime_notification_http_client_exit(ifttt);

	zwp_thread_exit(thread, ZWP_STATUS_OK);
	return NULL;

l_err_realtime_notification_handler_state_lock_2:
l_err_realtime_notification_handler_state_lock:
	zwp_ifttt_realtime_notification_http_client_exit(ifttt);
l_err_realtime_notification_http_client_init:
	ZWP_LOG(ZWP_LOG_EMERG, "IFTTT Realtime Notification handler: Failed abruptly");
	zwp_thread_exit(thread, ZWP_STATUS_ERROR);
	return NULL;
}

static zwp_status_t zwp_ifttt_table_realtime_notification_open(zwp_ifttt_t ifttt)
{
	zwp_status_t return_status;
	int column_count;

	if ((return_status = zwp_db_open_table(ifttt->db, ZWP_IFTTT_DB_TABLE_REALTIME_NOTIFICATION_NAME, &ifttt->realtime_notification_table)) != ZWP_STATUS_OK)
	{
		if (return_status == ZWP_DB_ERROR_DB_SEARCH_NOT_FOUND)
		{
			column_count = sizeof(zwp_ifttt_realtime_notification_table_head) / sizeof(zwp_db_table_head);

			if (zwp_db_create_table(ifttt->db, ZWP_IFTTT_DB_TABLE_REALTIME_NOTIFICATION_NAME,
					column_count, zwp_ifttt_realtime_notification_table_head,
					&ifttt->realtime_notification_table) != ZWP_STATUS_OK)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Fail to create realtime notification table");
				goto l_err_create_realtime_notification_table;
			}
		}
		else
		{
			ZWP_LOG(ZWP_LOG_ERR, "Fail to open realtime notification table");
			goto l_err_open_realtime_notification_table;
		}
	}

	return ZWP_STATUS_OK;

l_err_open_realtime_notification_table:
l_err_create_realtime_notification_table:
	return ZWP_STATUS_ERROR;
}

static void zwp_ifttt_table_realtime_notification_close(zwp_ifttt_t ifttt)
{
	zwp_db_close_table(ifttt->realtime_notification_table);
}

static zwp_status_t zwp_ifttt_db_open(zwp_ifttt_t ifttt)
{
	if (zwp_db_open_db(ifttt->ifttt_config->local_state_dir, 0, &ifttt->db) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to open/create common IFTTT database");
		goto l_err_ifttt_db_init;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Common IFTTT DB created/opened %p", ifttt->db);

	return ZWP_STATUS_OK;

	zwp_db_close_db(ifttt->db);
l_err_ifttt_db_init:
	return ZWP_STATUS_ERROR;
}

static void zwp_ifttt_db_close(zwp_ifttt_t ifttt)
{
	zwp_db_close_db(ifttt->db);
}

zwp_status_t zwp_ifttt_init(zwp_ifttt_t *ifttt_p, const zwp_ifttt_config_t ifttt_config)
{
	static zwp_ifttt_t g_ifttt;
	zwp_ifttt_t ifttt;
	zwp_status_t return_status;

	//Enforce singleton
	if (g_ifttt)
	{
		ZWP_LOG(ZWP_LOG_WARNING, ZWP_MSG_SINGLETON_MULTIPLE_CALLS);
		*ifttt_p = g_ifttt;

		return ZWP_STATUS_OK;
	}

	if ((ifttt = (zwp_ifttt_t)ZWP_MALLOC(sizeof(*ifttt))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_ifttt_alloc;
	}

	ifttt->realtime_notification_pending = 0;
	ifttt->realtime_notification_handler_state = ZWP_IFTTT_REALTIME_NOTIFICATION_HANDLER_STATE_DOWN;

	//Link with configuration
	ifttt->ifttt_config = ifttt_config;

	if (zwp_ifttt_db_open(ifttt) != ZWP_STATUS_OK)
	{
		goto l_err_ifttt_db_open;
	}

	if (zwp_ifttt_table_realtime_notification_open(ifttt) != ZWP_STATUS_OK)
	{
		goto l_err_ifttt_table_realtime_notification_open;
	}

	// Debugging
#if 0
	if ((zwp_ifttt_realtime_notification_trigger_add(ifttt, "abc1") != ZWP_STATUS_OK)
			|| (zwp_ifttt_realtime_notification_trigger_add(ifttt, "abc2") != ZWP_STATUS_OK)
			|| (zwp_ifttt_realtime_notification_trigger_add(ifttt, "abc3") != ZWP_STATUS_OK)
			|| (zwp_ifttt_realtime_notification_trigger_add(ifttt, "abc4") != ZWP_STATUS_OK)
			|| (zwp_ifttt_realtime_notification_trigger_add(ifttt, "abc5") != ZWP_STATUS_OK)
			|| (zwp_ifttt_realtime_notification_trigger_add(ifttt, "abc6") != ZWP_STATUS_OK)
			|| (zwp_ifttt_realtime_notification_trigger_add(ifttt, "abc7") != ZWP_STATUS_OK)
			|| (zwp_ifttt_realtime_notification_trigger_add(ifttt, "abc8") != ZWP_STATUS_OK)
			|| (zwp_ifttt_realtime_notification_trigger_add(ifttt, "abc9") != ZWP_STATUS_OK)
			|| (zwp_ifttt_realtime_notification_trigger_add(ifttt, "abc10") != ZWP_STATUS_OK))
	{
		goto l_err_ifttt_notification_trigger_add;
	}
#endif

	// Debugging
#if 0
	if (zwp_ifttt_realtime_notification_trigger_add(ifttt, "adf0ee2635281c9527daed37719a72a0ab6a67f5") != ZWP_STATUS_OK)
	{
		goto l_err_ifttt_notification_trigger_add;
	}
#endif

	//Create memory pool for IFTTT module
	if (zwp_pool_create(&ifttt->memory_pool, NULL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create memory pool for IFTTT module");
		goto l_err_memory_pool_create;
	}

	if (zwp_thread_mutex_create(&ifttt->realtime_notification_handler_state_lock, ifttt->memory_pool, ZWP_THREAD_MUTEX_CREATE_NORMAL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create lock for network initialize state");
		goto l_err_notification_handler_state_lock_create;
	}

	ifttt->realtime_notification_handler_state = ZWP_IFTTT_REALTIME_NOTIFICATION_HANDLER_STATE_UP;

	if (zwp_thread_create(&ifttt->realtime_notification_handler, zwp_ifttt_realtime_notification_handler, ifttt,
				ifttt->memory_pool, ZWP_THREAD_CREATE_JOINABLE) != ZWP_STATUS_OK)
	{
		goto l_err_notification_handler_create;
	}

	g_ifttt = *ifttt_p = ifttt;

	return ZWP_STATUS_OK;

	if (zwp_thread_mutex_lock(ifttt->realtime_notification_handler_state_lock) == ZWP_STATUS_OK)
	{
		ifttt->realtime_notification_handler_state = ZWP_IFTTT_REALTIME_NOTIFICATION_HANDLER_STATE_SHUTTING_DOWN;
		zwp_thread_mutex_unlock(ifttt->realtime_notification_handler_state_lock);
	}
	zwp_thread_join(ifttt->realtime_notification_handler, &return_status);
l_err_notification_handler_create:
	zwp_thread_mutex_destroy(ifttt->realtime_notification_handler_state_lock);
l_err_notification_handler_state_lock_create:
	zwp_pool_destroy(ifttt->memory_pool);
l_err_memory_pool_create:
// Debugging
#if 0
l_err_ifttt_notification_trigger_add:
#endif
	zwp_ifttt_table_realtime_notification_close(ifttt);
l_err_ifttt_table_realtime_notification_open:
	zwp_ifttt_db_close(ifttt);
l_err_ifttt_db_open:
	ZWP_FREE(ifttt);
l_err_ifttt_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_ifttt_exit(zwp_ifttt_t ifttt)
{
	zwp_status_t return_status;

	if (zwp_thread_mutex_lock(ifttt->realtime_notification_handler_state_lock) == ZWP_STATUS_OK)
	{
		ifttt->realtime_notification_handler_state = ZWP_IFTTT_REALTIME_NOTIFICATION_HANDLER_STATE_SHUTTING_DOWN;
		zwp_thread_mutex_unlock(ifttt->realtime_notification_handler_state_lock);
	}
	else
	{
		// Shutdown anyway
		ifttt->realtime_notification_handler_state = ZWP_IFTTT_REALTIME_NOTIFICATION_HANDLER_STATE_SHUTTING_DOWN;
	}

	if (zwp_thread_join(ifttt->realtime_notification_handler, &return_status) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to join the handler for network operation status");
		goto l_err_thread_join;
	}
	zwp_thread_mutex_destroy(ifttt->realtime_notification_handler_state_lock);
	zwp_pool_destroy(ifttt->memory_pool);
	zwp_ifttt_table_realtime_notification_close(ifttt);
	zwp_ifttt_db_close(ifttt);
	ZWP_FREE(ifttt);

	return;

l_err_thread_join:
	return;
}

zwp_status_t zwp_ifttt_config_get_instance(zwp_ifttt_config_t *ifttt_config_p,
		const config_t *config_parser, const char *config_dir)
{
	static zwp_ifttt_config_t g_ifttt_config;
	zwp_ifttt_config_t ifttt_config;
	const char *channel_key;
	int realtime_api_post_interval;
	int realtime_api_post_retry;

	//Enforce singleton
	if (g_ifttt_config)
	{
		ZWP_LOG(ZWP_LOG_WARNING, ZWP_MSG_SINGLETON_MULTIPLE_CALLS);
		*ifttt_config_p = g_ifttt_config;

		return ZWP_STATUS_OK;
	}

	if ((ifttt_config = (zwp_ifttt_config_t)ZWP_MALLOC(sizeof(*ifttt_config))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_ifttt_config_alloc;
	}

	//Local state dir
	//Get 'localstatedir' relative to 'sysconfigdir' (autotools)
	if ((ifttt_config->local_state_dir = (char *)ZWP_MALLOC(strlen(config_dir) + 1 + sizeof("../var/ifttt"))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_local_state_dir_alloc;
	}
	sprintf(ifttt_config->local_state_dir, "%s/../var/ifttt", config_dir);
	ZWP_LOG(ZWP_LOG_DEBUG, "IFTTT local state dir: %s", ifttt_config->local_state_dir);

	//Channel Key
	if (config_lookup_string(config_parser, "ifttt.channel_key", &channel_key) != CONFIG_TRUE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to lookup Channel Key");
		goto l_err_lookup_channel_key;
	}

	if ((ifttt_config->channel_key = ZWP_STRDUP(channel_key)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_channel_key_alloc;
	}

	//Realtime API POST Interval (in milliseconds)
	realtime_api_post_interval = ZWP_OPTION_DEFAULT_REALTIME_API_POST_INTERVAL;
	if (config_lookup_int(config_parser, "ifttt.realtime_api_post_interval", &realtime_api_post_interval) != CONFIG_TRUE)
	{
		ZWP_LOG(ZWP_LOG_WARNING, "Failed to lookup IFTTT Realtime API POST interval. Proceeding with default interval %d milliseconds",
				ZWP_OPTION_DEFAULT_REALTIME_API_POST_INTERVAL);
	}
	if (realtime_api_post_interval < 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Configuration 'ifttt.realtime_api_post_interval' is out of valid range");
		goto l_err_invalid_realtime_api_post_interval;
	}
	ifttt_config->realtime_api_post_interval = realtime_api_post_interval * 1000; // to microseconds
	ZWP_LOG(ZWP_LOG_DEBUG, "IFTTT Realtime API POST interval: %d milliseconds", realtime_api_post_interval);

	//Realtime API POST Retry
	realtime_api_post_retry = ZWP_OPTION_DEFAULT_REALTIME_API_POST_RETRY;
	if (config_lookup_int(config_parser, "ifttt.realtime_api_post_retry", &realtime_api_post_retry) != CONFIG_TRUE)
	{
		ZWP_LOG(ZWP_LOG_WARNING, "Failed to lookup IFTTT Realtime API POST retry. Proceeding with default retry count %d",
				ZWP_OPTION_DEFAULT_REALTIME_API_POST_RETRY);
	}
	if (realtime_api_post_retry < 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Configuration 'ifttt.realtime_api_post_retry' is out of valid range");
		goto l_err_invalid_realtime_api_post_interval;
	}
	ifttt_config->realtime_api_post_retry = realtime_api_post_retry;
	ZWP_LOG(ZWP_LOG_DEBUG, "IFTTT Realtime API POST interval: %d", realtime_api_post_retry);

	g_ifttt_config = *ifttt_config_p = ifttt_config;

	return ZWP_STATUS_OK;

l_err_invalid_realtime_api_post_interval:
	ZWP_FREE(ifttt_config->channel_key);
l_err_channel_key_alloc:
l_err_lookup_channel_key:
	ZWP_FREE(ifttt_config->local_state_dir);
l_err_local_state_dir_alloc:
	ZWP_FREE(ifttt_config);
l_err_ifttt_config_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_ifttt_config_free(zwp_ifttt_config_t ifttt_config)
{
	ZWP_FREE(ifttt_config->channel_key);
	ZWP_FREE(ifttt_config->local_state_dir);
	ZWP_FREE(ifttt_config);
}
