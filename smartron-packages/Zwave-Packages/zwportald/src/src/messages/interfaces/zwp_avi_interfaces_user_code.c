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
 * @file    zwp_avi_interfaces_user_code.c
 *
 * @brief   AVI message 'interface_user_code_xxx'
 * @details Handles the AVI message 'interface_user_code_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-10-03
 * - Initial version
 */

#include "zwp_avi_interfaces.h"

/** Get command state */
typedef struct {
	uint8_t            user_identifier;                    /**< User identifier */
} zwp_avi_interfaces_user_code_get_t;

/** Set command state */
typedef struct {
	zwusrcod_t         user_code;                          /**< User code value */
} zwp_avi_interfaces_user_code_set_t;

/** User code value description */
typedef struct {
	zwu_time_t         utime;                              /**< Last update time for specific type and scale combination */
	zwusrcod_t         value;                              /**< User code value */
} zwp_avi_interfaces_user_code_report_user_code_value_t;

/** Report command state */
typedef struct {
	zwu_time_t         utime;                              /**< Last update time across all types */
	zwu_array_list_t   user_code_value_list;               /**< List of user code values of type @a zwp_avi_interfaces_user_code_report_user_code_value_t */

	int                report_receive_state;               /**< Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Report command receive condition */
	uint8_t            report_receive_user_identifier;     /**< Current User identifier for Report command receive condition */
} zwp_avi_interfaces_user_code_report_t;

/** Users Number Report command state */
typedef struct {
	zwu_time_t         utime;                              /**< Last updated time of Users Number Report command */
	uint8_t            supported_users;                    /**< Number of supported users */

	int                report_receive_state;               /**< Users Number Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Users Number Report command receive condition */
} zwp_avi_interfaces_user_code_users_number_report_t;

/** Interface level state */
typedef struct {
	zwp_avi_interfaces_user_code_report_t                   report;                     /**< Report command state */
	zwp_avi_interfaces_user_code_users_number_report_t      users_number_report;        /**< Users Number Report command state */
} zwp_avi_interfaces_user_code_state_t;

/**
 * @brief Allocate interface level data
 * @param[out]    state_p           Interface level data
 * @param[in]     pool              Memory pool to be used for allocation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_user_code_state_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_user_code_state_alloc(void **state_p, zwp_pool_t pool)
{
	zwp_avi_interfaces_user_code_state_t *state;

	if ((state = (zwp_avi_interfaces_user_code_state_t *)ZWP_CALLOC(1, sizeof(*state))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_state_alloc;
	}

	if (zwp_thread_cond_create(&state->report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for report received event");
		goto l_err_net_report_received_cond_create;
	}

	state->report.report_receive_user_identifier = 0;

	if (zwu_array_list_create(0, &state->report.user_code_value_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	if (zwp_thread_cond_create(&state->users_number_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for users_number_report received event");
		goto l_err_net_users_number_report_received_cond_create;
	}

	*state_p = state;

	return ZWP_STATUS_OK;

	zwp_thread_cond_destroy(state->users_number_report.report_receive_condition);
l_err_net_users_number_report_received_cond_create:
	zwu_array_list_destroy(state->report.user_code_value_list);
l_err_value_list_create:
	zwp_thread_cond_destroy(state->report.report_receive_condition);
l_err_net_report_received_cond_create:
	ZWP_FREE(state);
l_err_state_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free interface level data
 * @param[in]     arg               Interface level data
 * @return        Nothing
 * @see zwp_avi_interfaces_user_code_state_alloc
 */
static void zwp_avi_interfaces_user_code_state_free(void *arg)
{
	zwp_avi_interfaces_user_code_state_t *state;
	zwp_avi_interfaces_user_code_report_t *report;
	int user_code_value_count;
	int i;

	state = (zwp_avi_interfaces_user_code_state_t *)arg;
	report = &state->report;

	zwp_interface_report_receive_done(&state->users_number_report.report_receive_state, state->users_number_report.report_receive_condition);

	zwp_thread_cond_destroy(state->users_number_report.report_receive_condition);

	user_code_value_count = zwu_array_list_size(report->user_code_value_list);

	for (i = 0; i < user_code_value_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(report->user_code_value_list, i));
	}

	zwu_array_list_destroy(report->user_code_value_list);

	zwp_interface_report_receive_done(&state->report.report_receive_state, state->report.report_receive_condition);

	zwp_thread_cond_destroy(state->report.report_receive_condition);

	ZWP_FREE(state);
}

/**
 * @brief Update interface level state with the new Report
 * @param[in]     report            Report state in interface level
 * @param[in]     usr_cod           Value from new Report
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_user_code_report_update(zwp_avi_interfaces_user_code_report_t *report,
		zwusrcod_p usr_cod)
{
	int user_code_value_count;
	int i;
	zwu_time_t  time_now;
	zwp_avi_interfaces_user_code_report_user_code_value_t *user_code_value;

	user_code_value_count = zwu_array_list_size(report->user_code_value_list);

	for (i = 0; i < user_code_value_count; i++)
	{
		user_code_value = zwu_array_list_get(report->user_code_value_list, i);

		// If user_identifier matches, update the entry instead of creating new one.
		if (user_code_value->value.id == usr_cod->id)
		{
			break;
		}
	}

	if (i >= user_code_value_count)
	{
		if ((user_code_value = (zwp_avi_interfaces_user_code_report_user_code_value_t *)ZWP_CALLOC(1, sizeof(*user_code_value))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		if (zwu_array_list_add(report->user_code_value_list, user_code_value) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}
	}

	time_now = zwu_time_now();

	report->utime = time_now;
	user_code_value->utime = time_now;
	memcpy(&user_code_value->value, usr_cod, sizeof(user_code_value->value));

	/*
	 * To avoid usage of uninitialized portion in Avro message
	 */
	memset(user_code_value->value.code, 0, sizeof(user_code_value->value.code));
	memcpy(user_code_value->value.code, usr_cod->code, usr_cod->code_len);

	return ZWP_STATUS_OK;

l_err_value_list_add:
	ZWP_FREE(user_code_value);
l_err_value_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief HCAPI callback for Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  usr_cod           user code and its status
 * @return        Nothing
 */
static void zwp_avi_interfaces_user_code_report_handler(zwifd_p ifd, zwusrcod_p usr_cod)
{
	zwp_avi_interfaces_user_code_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_user_code_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (zwp_avi_interfaces_user_code_report_update(&state->report, usr_cod) != ZWP_STATUS_OK)
	{
		goto l_err_report_update;
	}

	state->report.report_receive_user_identifier = usr_cod->id;
	zwp_interface_report_receive_done(&state->report.report_receive_state, state->report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

l_err_report_update:
l_err_get_state:
	zwp_net_release_desc_interface(ifd);
l_err_acquire_desc:

	return;
}

/**
 * @brief Get argument for Get command from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Get command
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_user_code_get_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_user_code_get_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_user_code_get_t *argument;
	int integer;

	if ((argument = (zwp_avi_interfaces_user_code_get_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "user_identifier", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_get_value;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "User Identifier: %d", integer);

	if (zwu_number_is_uint8(integer) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid user identifier: %d", integer);
		goto l_err_invalid_user_identifier;
	}

	argument->user_identifier = (int8_t)integer;

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_invalid_user_identifier:
l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Get command
 * @param[in]     args              Arguments passed with Get command
 * @return        Nothing
 * @see zwp_avi_interfaces_user_code_get_argument
 */
static void zwp_avi_interfaces_user_code_get_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
 * @brief Test if the expected Report command is received
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Get command
 * @param[out]    report_receive_condition_p    Report command receive condition
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_user_code_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_user_code_state_t *state;
	zwp_avi_interfaces_user_code_get_t *argument;

	argument = (zwp_avi_interfaces_user_code_get_t *)args;

	if ((state = (zwp_avi_interfaces_user_code_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->report.report_receive_condition;

	if ((zwp_interface_report_receive_assertion(state->report.report_receive_state) != ZWP_STATUS_OK)
			|| (state->report.report_receive_user_identifier != argument->user_identifier))
	{
		goto l_err_report_not_received;
	}

	return ZWP_STATUS_OK;

l_err_report_not_received:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute active Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Get command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_user_code_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_user_code_state_t *state;
	zwp_avi_interfaces_user_code_get_t *argument;

	argument = (zwp_avi_interfaces_user_code_get_t *)args;

	if ((state = (zwp_avi_interfaces_user_code_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->report.report_receive_state);

	*hcapi_code_p = zwif_usrcod_get(desc_interface, argument->user_identifier, zwp_avi_interfaces_user_code_report_handler);

	return ZWP_STATUS_OK;

l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Allocate Result object corresponding to Report command
 * @param[out]    report_dest       Destination Report
 * @param[in]     report_src        Source Report
 * @param[in]     user_identifier   User identifier
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_user_code_get_result_alloc(zwp_avi_interfaces_user_code_report_t *report_dest,
		zwp_avi_interfaces_user_code_report_t *report_src, uint8_t user_identifier)
{
	int user_code_value_count;
	int i;
	zwp_avi_interfaces_user_code_report_user_code_value_t *user_code_value_dest;
	zwp_avi_interfaces_user_code_report_user_code_value_t *user_code_value_src;

	user_code_value_count = zwu_array_list_size(report_src->user_code_value_list);

	for (i = 0; i < user_code_value_count; i++)
	{
		user_code_value_src = zwu_array_list_get(report_src->user_code_value_list, i);

		// If user_identifier doesn't match, skip the entry. If 0, list all
		if ((user_identifier != 0) && (user_code_value_src->value.id != user_identifier))
		{
			continue;
		}

		if ((user_code_value_dest = (zwp_avi_interfaces_user_code_report_user_code_value_t *)ZWP_CALLOC(1, sizeof(*user_code_value_dest))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		if (zwu_array_list_add(report_dest->user_code_value_list, user_code_value_dest) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}

		memcpy(user_code_value_dest, user_code_value_src, sizeof(*user_code_value_dest));
	}

	return ZWP_STATUS_OK;

l_err_value_list_add:
	ZWP_FREE(user_code_value_dest);
l_err_value_alloc:
	for (i--; i >= 0; i--)
	{
		ZWP_FREE(zwu_array_list_get(report_dest->user_code_value_list, i));
	}
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute passive Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Get command
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_user_code_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_user_code_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_user_code_state_t *state;
	zwp_avi_interfaces_user_code_report_t *report;
	zwp_avi_interfaces_user_code_get_t *argument;

	argument = (zwp_avi_interfaces_user_code_get_t *)args;

	if ((state = (zwp_avi_interfaces_user_code_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((report = (zwp_avi_interfaces_user_code_report_t *)ZWP_CALLOC(1, sizeof(*report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	if (zwu_array_list_create(0, &report->user_code_value_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	if (zwp_avi_interfaces_user_code_get_result_alloc(report, &state->report, argument->user_identifier) != ZWP_STATUS_OK)
	{
		goto l_err_result_alloc;
	}

	*result_p = report;

	return ZWP_STATUS_OK;

l_err_result_alloc:
	zwu_array_list_destroy(report->user_code_value_list);
l_err_value_list_create:
	ZWP_FREE(report);
l_err_report_alloc:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to Report command
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_user_code_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_user_code_report_t *report;
	avro_value_t user_code_value_list_value;
	avro_value_t user_code_value_value;
	zwp_avi_interfaces_user_code_report_user_code_value_t *user_code_value;
	int user_code_value_count;
	int i;

	report = (zwp_avi_interfaces_user_code_report_t *)result;

	if (avro_value_get_by_name(arg_value, "user_code_list", &user_code_value_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'user_code_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	user_code_value_count = zwu_array_list_size(report->user_code_value_list);

	for (i = 0; i < user_code_value_count; i++)
	{
		if (avro_value_append(&user_code_value_list_value, &user_code_value_value, 0) != 0)
		{
			goto l_err_append_user_code_value_value;
		}

		user_code_value = zwu_array_list_get(report->user_code_value_list, i);

		if ((zw_avro_field_set_long(&user_code_value_value, "utime", user_code_value->utime) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&user_code_value_value, "user_identifier", user_code_value->value.id) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&user_code_value_value, "user_id_status", user_code_value->value.id_sts) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&user_code_value_value, "user_code_length", user_code_value->value.code_len) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_fixed(&user_code_value_value, "user_code", user_code_value->value.code, 10) != ZWP_STATUS_OK)
				)
		{
			goto l_err_set_user_code_value_value;
		}
	}

	return ZWP_STATUS_OK;

l_err_set_user_code_value_value:
l_err_append_user_code_value_value:
l_err_get_list_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_user_code_get_execute_passive
 */
static void zwp_avi_interfaces_user_code_get_result_free(void *result)
{
	zwp_avi_interfaces_user_code_report_t *report;
	int user_code_value_count;
	int i;

	report = (zwp_avi_interfaces_user_code_report_t *)result;

	user_code_value_count = zwu_array_list_size(report->user_code_value_list);

	for (i = 0; i < user_code_value_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(report->user_code_value_list, i));
	}
	zwu_array_list_destroy(report->user_code_value_list);

	ZWP_FREE(report);
}

/**
 * @brief Get argument for Set command from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Set command
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_user_code_set_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_user_code_set_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_user_code_set_t *argument;
	int32_t user_identifier;
	int32_t user_id_status;
	int32_t user_code_length;
	void *user_code;
	size_t user_code_size;

	if ((argument = (zwp_avi_interfaces_user_code_set_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if ((zw_avro_field_get_int(arg_value, "user_identifier", &user_identifier) != ZWP_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "user_id_status", &user_id_status) != ZWP_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "user_code_length", &user_code_length) != ZWP_STATUS_OK))
	{
		goto l_err_get_value;
	}

	if (zw_avro_field_get_fixed(arg_value, "user_code", &user_code, &user_code_size) != ZWP_STATUS_OK)
	{
		goto l_err_get_user_code_value;
	}

	if (user_code_size != 10)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid user code value field size");
		goto l_err_user_code_size;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "User Identifier: %d", user_identifier);
	ZWP_LOG(ZWP_LOG_DEBUG, "User ID Status: %d", user_id_status);
	ZWP_LOG(ZWP_LOG_DEBUG, "User Code Length: %d", user_code_length);
	ZWP_LOG(ZWP_LOG_DEBUG, "User Code: 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
			((uint8_t *)user_code)[0], ((uint8_t *)user_code)[1],
			((uint8_t *)user_code)[2], ((uint8_t *)user_code)[3],
			((uint8_t *)user_code)[4], ((uint8_t *)user_code)[5],
			((uint8_t *)user_code)[6], ((uint8_t *)user_code)[7],
			((uint8_t *)user_code)[8], ((uint8_t *)user_code)[9]);

	if (zwu_number_is_uint8(user_identifier) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid user identifier: %d", user_identifier);
		goto l_err_invalid_user_identifier;
	}

	if (zwu_number_is_uint8(user_id_status) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid user ID status: %d", user_id_status);
		goto l_err_invalid_user_id_status;
	}

	if (zwu_number_is_uint8(user_code_length) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid user code length: %d", user_code_length);
		goto l_err_invalid_user_code_length;
	}

	argument->user_code.id = (uint8_t)user_identifier;
	argument->user_code.id_sts = (uint8_t)user_id_status;
	argument->user_code.code_len = (uint8_t)user_code_length;

	memset(argument->user_code.code, 0, sizeof(argument->user_code.code));

	strncpy((char *)argument->user_code.code, user_code, sizeof(argument->user_code.code));

	zw_avro_field_free_fixed(user_code);

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_invalid_user_code_length:
l_err_invalid_user_id_status:
l_err_invalid_user_identifier:
l_err_user_code_size:
	zw_avro_field_free_fixed(user_code);
l_err_get_user_code_value:
l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Set command
 * @param[in]     args              Arguments passed with Set command
 * @return        Nothing
 * @see zwp_avi_interfaces_user_code_set_argument
 */
static void zwp_avi_interfaces_user_code_set_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
 * @brief Execute Set command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments passed with Set command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_user_code_set_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_user_code_set_t *argument;

	argument = (zwp_avi_interfaces_user_code_set_t *)args;

	*hcapi_code_p = zwif_usrcod_set(desc_interface, &argument->user_code);

	return ZWP_STATUS_OK;
}

/**
 * @brief HCAPI callback for Users Number Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  usr_num           number of supported user codes
 * @return        Nothing
 */
static void zwp_avi_interfaces_user_code_users_number_report_handler(zwifd_p ifd, uint8_t usr_num, int valid)
{
	zwp_avi_interfaces_user_code_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_user_code_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (valid)
	{
		state->users_number_report.utime = zwu_time_now();
		state->users_number_report.supported_users = usr_num;
	}
	zwp_interface_report_receive_done(&state->users_number_report.report_receive_state, state->users_number_report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

l_err_get_state:
	zwp_net_release_desc_interface(ifd);
l_err_acquire_desc:

	return;
}

/**
 * @brief Test if the expected Users Number Report command is received
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Get command
 * @param[out]    report_receive_condition_p    Report command receive condition
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_user_code_users_number_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_user_code_state_t *state;

	if ((state = (zwp_avi_interfaces_user_code_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->users_number_report.report_receive_condition;

	if (zwp_interface_report_receive_assertion(state->users_number_report.report_receive_state) != ZWP_STATUS_OK)
	{
		goto l_err_report_not_received;
	}

	return ZWP_STATUS_OK;

l_err_report_not_received:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute active Users Number Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Users Number Get command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_user_code_users_number_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_user_code_state_t *state;

	if ((state = (zwp_avi_interfaces_user_code_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->users_number_report.report_receive_state);

	*hcapi_code_p = zwif_usrcod_sup_get(desc_interface, zwp_avi_interfaces_user_code_users_number_report_handler, 0);

	return ZWP_STATUS_OK;

l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute passive Users Number Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Users Number Get command
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_user_code_users_number_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_user_code_users_number_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_user_code_state_t *state;
	zwp_avi_interfaces_user_code_users_number_report_t *users_number_report;

	if ((state = (zwp_avi_interfaces_user_code_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((users_number_report = (zwp_avi_interfaces_user_code_users_number_report_t *)ZWP_CALLOC(1, sizeof(*users_number_report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	memcpy(users_number_report, &state->users_number_report, sizeof(*users_number_report));

	*result_p = users_number_report;

	return ZWP_STATUS_OK;

l_err_report_alloc:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to Users Number Report command
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_user_code_users_number_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_user_code_users_number_report_t *users_number_report;

	users_number_report = (zwp_avi_interfaces_user_code_users_number_report_t *)result;

	if ((zw_avro_field_set_long(arg_value, "utime", users_number_report->utime) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "supported_users", users_number_report->supported_users) != ZWP_STATUS_OK))
	{
		goto l_err_set_report_value;
	}

	return ZWP_STATUS_OK;

l_err_set_report_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Users Number Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_user_code_users_number_get_execute_passive
 */
static void zwp_avi_interfaces_user_code_users_number_get_result_free(void *result)
{
	ZWP_FREE(result);
}

/** Handlers for Get command */
static zwp_avi_message_interfaces_t interfaces_user_code_get =
{
		"interface_user_code_get",
		COMMAND_CLASS_USER_CODE, NULL,
		zwp_avi_interfaces_user_code_state_alloc, zwp_avi_interfaces_user_code_state_free,
		zwp_avi_interfaces_user_code_get_argument, zwp_avi_interfaces_user_code_get_argument_free,
		zwp_avi_interfaces_user_code_get_report_receive_assertion,
		zwp_avi_interfaces_user_code_get_execute_active, zwp_avi_interfaces_user_code_get_execute_passive,
		zwp_avi_interfaces_user_code_get_result, zwp_avi_interfaces_user_code_get_result_free
};

/** Handlers for Set command */
static zwp_avi_message_interfaces_t interfaces_user_code_set =
{
		"interface_user_code_set",
		COMMAND_CLASS_USER_CODE, NULL,
		NULL, NULL,
		zwp_avi_interfaces_user_code_set_argument, zwp_avi_interfaces_user_code_set_argument_free,
		NULL,
		zwp_avi_interfaces_user_code_set_execute_active, NULL,
		NULL, NULL
};

/** Handlers for Users Number Get command */
static zwp_avi_message_interfaces_t interfaces_user_code_users_number_get =
{
		"interface_user_code_users_number_get",
		COMMAND_CLASS_USER_CODE, NULL,
		NULL, NULL,
		NULL, NULL,
		zwp_avi_interfaces_user_code_users_number_get_report_receive_assertion,
		zwp_avi_interfaces_user_code_users_number_get_execute_active, zwp_avi_interfaces_user_code_users_number_get_execute_passive,
		zwp_avi_interfaces_user_code_users_number_get_result, zwp_avi_interfaces_user_code_users_number_get_result_free
};

/** Register command handlers */
zwp_status_t zwp_avi_interfaces_user_code_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_user_code_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_user_code_set) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_user_code_users_number_get) != ZWP_STATUS_OK))
	{
		goto l_err_interface_list_get;
	}

	return ZWP_STATUS_OK;

l_err_interface_list_get:
	return ZWP_STATUS_ERROR;
}


