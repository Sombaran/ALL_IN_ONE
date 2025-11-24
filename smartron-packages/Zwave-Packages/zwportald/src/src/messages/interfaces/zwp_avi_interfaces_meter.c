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
 * @file    zwp_avi_interfaces_meter.c
 *
 * @brief   AVI message 'interface_meter_xxx'
 * @details Handles the AVI message 'interface_meter_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-10-02
 * - Initial version
 */

#include "zwp_avi_interfaces.h"

/** Meter scale not specified */
#define COMMAND_METER_SCALE_NONE         (-1)

/** Get command state */
typedef struct {
	int                scale;                              /**< Scale */
} zwp_avi_interfaces_meter_get_t;

/** Meter value description */
typedef struct {
	zwu_time_t         utime;                              /**< Last update time for specific type and scale combination */
	zwmeter_dat_t      value;                              /**< Value */
} zwp_avi_interfaces_meter_report_meter_value_t;

/** Report command state */
typedef struct {
	zwu_time_t         utime;                              /**< Last update time across all types */
	zwu_array_list_t   meter_value_list;                   /**< List of meter value of type @a zwp_avi_interfaces_meter_report_meter_value_t */

	int                report_receive_state;               /**< Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Report command receive condition */
	int                report_receive_scale;               /**< Current scale for Report command receive condition */
} zwp_avi_interfaces_meter_report_t;

/** Supported Report command state */
typedef struct {
	zwu_time_t         utime;                              /**< Last updated time of Supported Report command */
	zwmeter_cap_t      meter_capability;                   /**< Meter capability */

	int                report_receive_state;               /**< Supported Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Supported Report command receive condition */
} zwp_avi_interfaces_meter_supported_report_t;

/** Interface level state */
typedef struct {
	zwp_avi_interfaces_meter_report_t                report;               /**< Report command state */
	zwp_avi_interfaces_meter_supported_report_t      supported_report;     /**< Supported Report command state */
} zwp_avi_interfaces_meter_state_t;

/**
 * @brief Allocate interface level data
 * @param[out]    state_p           Interface level data
 * @param[in]     pool              Memory pool to be used for allocation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_meter_state_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_meter_state_alloc(void **state_p, zwp_pool_t pool)
{
	zwp_avi_interfaces_meter_state_t *state;

	if ((state = (zwp_avi_interfaces_meter_state_t *)ZWP_CALLOC(1, sizeof(*state))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_state_alloc;
	}

	if (zwp_thread_cond_create(&state->report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for report received event");
		goto l_err_net_report_received_cond_create;
	}

	state->report.report_receive_scale = COMMAND_METER_SCALE_NONE;

	if (zwu_array_list_create(0, &state->report.meter_value_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	if (zwp_thread_cond_create(&state->supported_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for supported_report received event");
		goto l_err_net_supported_report_received_cond_create;
	}

	*state_p = state;

	return ZWP_STATUS_OK;

	zwp_thread_cond_destroy(state->supported_report.report_receive_condition);
l_err_net_supported_report_received_cond_create:
	zwu_array_list_destroy(state->report.meter_value_list);
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
 * @see zwp_avi_interfaces_meter_state_alloc
 */
static void zwp_avi_interfaces_meter_state_free(void *arg)
{
	zwp_avi_interfaces_meter_state_t *state;
	zwp_avi_interfaces_meter_report_t *report;
	int meter_value_count;
	int i;

	state = (zwp_avi_interfaces_meter_state_t *)arg;
	report = &state->report;

	zwp_interface_report_receive_done(&state->supported_report.report_receive_state, state->supported_report.report_receive_condition);

	zwp_thread_cond_destroy(state->supported_report.report_receive_condition);

	meter_value_count = zwu_array_list_size(report->meter_value_list);

	for (i = 0; i < meter_value_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(report->meter_value_list, i));
	}

	zwu_array_list_destroy(report->meter_value_list);

	zwp_interface_report_receive_done(&state->report.report_receive_state, state->report.report_receive_condition);

	zwp_thread_cond_destroy(state->report.report_receive_condition);

	ZWP_FREE(state);
}

/**
 * @brief Update interface level state with the new Report
 * @param[in]     report            Report state in interface level
 * @param[in]     data              Value from new Report
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_meter_report_update(zwp_avi_interfaces_meter_report_t *report,
		zwmeter_dat_p data, zwu_time_t ts)
{
	int meter_value_count;
	int i;
	//zwu_time_t  time_now;
	zwp_avi_interfaces_meter_report_meter_value_t *meter_value;

	meter_value_count = zwu_array_list_size(report->meter_value_list);

	for (i = 0; i < meter_value_count; i++)
	{
		meter_value = zwu_array_list_get(report->meter_value_list, i);

		// If both type and scale matches, update the entry instead of creating new one.
		if ((meter_value->value.type == data->type) && (meter_value->value.unit == data->unit))
		{
			break;
		}
	}

	if (i >= meter_value_count)
	{
		if ((meter_value = (zwp_avi_interfaces_meter_report_meter_value_t *)ZWP_CALLOC(1, sizeof(*meter_value))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		if (zwu_array_list_add(report->meter_value_list, meter_value) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}
	}

	//time_now = zwu_time_now();

	//report->utime = time_now;
	//meter_value->utime = time_now;
	report->utime = ts;
	meter_value->utime = ts;
	memcpy(&meter_value->value, data, sizeof(meter_value->value));

	/*
	 * To avoid usage of uninitialized portion in Avro message
	 */
	memset(meter_value->value.data, 0, sizeof(meter_value->value.data));
	memcpy(meter_value->value.data, data->data, data->size);

	return ZWP_STATUS_OK;

l_err_value_list_add:
	ZWP_FREE(meter_value);
l_err_value_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief HCAPI callback for Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  data              Value and unit of the meter
 * @param[in]	  ts      			time stamp
 * @return        Nothing
 */
static void zwp_avi_interfaces_meter_report_handler(zwifd_p ifd, zwmeter_dat_p data, time_t ts)
{
	zwp_avi_interfaces_meter_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_meter_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if(ts)
	{
		if (zwp_avi_interfaces_meter_report_update(&state->report, data, ts) != ZWP_STATUS_OK)
		{
			goto l_err_report_update;
		}
		state->report.report_receive_scale = data->unit;
	}

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
 * @brief Setup interface
 * @param[in]     desc_interface    Interface descriptor
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_meter_setup(zwifd_p desc_interface, int *hcapi_code_p)
{
	*hcapi_code_p = zwif_meter_rpt_set(desc_interface, zwp_avi_interfaces_meter_report_handler);

	return ZWP_STATUS_OK;
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
 * The caller should call zwp_avi_interfaces_meter_get_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_meter_get_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_meter_get_t *argument;
	int integer;

	if ((argument = (zwp_avi_interfaces_meter_get_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "scale", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_get_value;
	}
	argument->scale = integer;

	ZWP_LOG(ZWP_LOG_DEBUG, "Scale: %d", argument->scale);

	if ((argument->scale != COMMAND_METER_SCALE_NONE)
			&& (zwu_number_is_uint8(argument->scale) != ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Scale: %d", argument->scale);
		goto l_err_invalid_scale;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_invalid_scale:
l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Get command
 * @param[in]     args              Arguments passed with Get command
 * @return        Nothing
 * @see zwp_avi_interfaces_meter_get_argument
 */
static void zwp_avi_interfaces_meter_get_argument_free(void *args)
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
static zwp_status_t zwp_avi_interfaces_meter_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_meter_state_t *state;
	zwp_avi_interfaces_meter_get_t *argument;

	argument = (zwp_avi_interfaces_meter_get_t *)args;

	if ((state = (zwp_avi_interfaces_meter_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->report.report_receive_condition;

	if ((zwp_interface_report_receive_assertion(state->report.report_receive_state) != ZWP_STATUS_OK)
			|| ((argument->scale != COMMAND_METER_SCALE_NONE) && (state->report.report_receive_scale != argument->scale)))
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
static zwp_status_t zwp_avi_interfaces_meter_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_meter_state_t *state;
	zwp_avi_interfaces_meter_get_t *argument;
	uint8_t scale;

	argument = (zwp_avi_interfaces_meter_get_t *)args;

	if ((state = (zwp_avi_interfaces_meter_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->report.report_receive_state);

	// Optional parameter: When scale is not specified, default to 0
	scale = (argument->scale == COMMAND_METER_SCALE_NONE) ? 0 : (uint8_t)argument->scale;

	*hcapi_code_p = zwif_meter_get(desc_interface, scale, ZWIF_GET_BMSK_LIVE | ZWIF_GET_BMSK_CACHE);

	return ZWP_STATUS_OK;

l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Allocate Result object corresponding to Report command
 * @param[out]    report_dest       Destination Report
 * @param[in]     report_src        Source Report
 * @param[in]     scale             Scale
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_meter_get_result_alloc(zwp_avi_interfaces_meter_report_t *report_dest,
		zwp_avi_interfaces_meter_report_t *report_src, int scale)
{
	int meter_value_count;
	int i;
	zwp_avi_interfaces_meter_report_meter_value_t *meter_value_dest;
	zwp_avi_interfaces_meter_report_meter_value_t *meter_value_src;

	meter_value_count = zwu_array_list_size(report_src->meter_value_list);

	for (i = 0; i < meter_value_count; i++)
	{
		meter_value_src = zwu_array_list_get(report_src->meter_value_list, i);

		// If scale doesn't match, skip the entry
		if ((scale != COMMAND_METER_SCALE_NONE) && (meter_value_src->value.unit != scale))
		{
			continue;
		}

		if ((meter_value_dest = (zwp_avi_interfaces_meter_report_meter_value_t *)ZWP_CALLOC(1, sizeof(*meter_value_dest))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		if (zwu_array_list_add(report_dest->meter_value_list, meter_value_dest) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}

		memcpy(meter_value_dest, meter_value_src, sizeof(*meter_value_dest));
	}

	return ZWP_STATUS_OK;

l_err_value_list_add:
	ZWP_FREE(meter_value_dest);
l_err_value_alloc:
	for (i--; i >= 0; i--)
	{
		ZWP_FREE(zwu_array_list_get(report_dest->meter_value_list, i));
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
 * The caller should call zwp_avi_interfaces_meter_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_meter_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_meter_state_t *state;
	zwp_avi_interfaces_meter_report_t *report;
	zwp_avi_interfaces_meter_get_t *argument;

	argument = (zwp_avi_interfaces_meter_get_t *)args;

	if ((state = (zwp_avi_interfaces_meter_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((report = (zwp_avi_interfaces_meter_report_t *)ZWP_CALLOC(1, sizeof(*report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	if (zwu_array_list_create(0, &report->meter_value_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	if (zwp_avi_interfaces_meter_get_result_alloc(report, &state->report, argument->scale) != ZWP_STATUS_OK)
	{
		goto l_err_result_alloc;
	}

	*result_p = report;

	return ZWP_STATUS_OK;

l_err_result_alloc:
	zwu_array_list_destroy(report->meter_value_list);
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
static zwp_status_t zwp_avi_interfaces_meter_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_meter_report_t *report;
	avro_value_t meter_value_list_value;
	avro_value_t meter_value_value;
	zwp_avi_interfaces_meter_report_meter_value_t *meter_value;
	int meter_value_count;
	int i;

	report = (zwp_avi_interfaces_meter_report_t *)result;

	if (avro_value_get_by_name(arg_value, "meter_value_list", &meter_value_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'meter_value_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	meter_value_count = zwu_array_list_size(report->meter_value_list);

	for (i = 0; i < meter_value_count; i++)
	{
		if (avro_value_append(&meter_value_list_value, &meter_value_value, 0) != 0)
		{
			goto l_err_append_meter_value_value;
		}

		meter_value = zwu_array_list_get(report->meter_value_list, i);

		if ((zw_avro_field_set_long(&meter_value_value, "utime", meter_value->utime) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&meter_value_value, "meter_type", meter_value->value.type) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&meter_value_value, "precision", meter_value->value.precision) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&meter_value_value, "scale", meter_value->value.unit) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&meter_value_value, "size", meter_value->value.size) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_fixed(&meter_value_value, "meter_value", meter_value->value.data, 4) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&meter_value_value, "rate_type", meter_value->value.rate_type) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&meter_value_value, "delta_time", meter_value->value.delta_time) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_fixed(&meter_value_value, "previous_meter_value", meter_value->value.prv_data, 4) != ZWP_STATUS_OK)
				)
		{
			goto l_err_set_meter_value_value;
		}
	}

	return ZWP_STATUS_OK;

l_err_set_meter_value_value:
l_err_append_meter_value_value:
l_err_get_list_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_meter_get_execute_passive
 */
static void zwp_avi_interfaces_meter_get_result_free(void *result)
{
	zwp_avi_interfaces_meter_report_t *report;
	int meter_value_count;
	int i;

	report = (zwp_avi_interfaces_meter_report_t *)result;

	meter_value_count = zwu_array_list_size(report->meter_value_list);

	for (i = 0; i < meter_value_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(report->meter_value_list, i));
	}
	zwu_array_list_destroy(report->meter_value_list);

	ZWP_FREE(report);
}

/**
 * @brief HCAPI callback for Supported Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  meter_cap         meter capabilities
 * @return        Nothing
 */
static void zwp_avi_interfaces_meter_supported_report_handler(zwifd_p ifd, zwmeter_cap_p meter_cap, int valid)
{
	zwp_avi_interfaces_meter_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_meter_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (valid)
	{
		state->supported_report.utime = zwu_time_now();

		memcpy(&state->supported_report.meter_capability, meter_cap, sizeof(state->supported_report.meter_capability));
	}
	zwp_interface_report_receive_done(&state->supported_report.report_receive_state, state->supported_report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

l_err_get_state:
	zwp_net_release_desc_interface(ifd);
l_err_acquire_desc:

	return;
}

/**
 * @brief Test if the expected Supported Report command is received
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Supported Get command
 * @param[out]    report_receive_condition_p    Report command receive condition
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_meter_supported_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_meter_state_t *state;

	if ((state = (zwp_avi_interfaces_meter_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->supported_report.report_receive_condition;

	if (zwp_interface_report_receive_assertion(state->supported_report.report_receive_state) != ZWP_STATUS_OK)
	{
		goto l_err_report_not_received;
	}

	return ZWP_STATUS_OK;

l_err_report_not_received:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute active Supported Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Supported Get command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_meter_supported_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_meter_state_t *state;

	if ((state = (zwp_avi_interfaces_meter_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->supported_report.report_receive_state);

	*hcapi_code_p = zwif_meter_sup_get(desc_interface, zwp_avi_interfaces_meter_supported_report_handler, 0);

	return ZWP_STATUS_OK;

l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute passive Supported Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Supported Get command
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_meter_supported_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_meter_supported_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_meter_state_t *state;
	zwp_avi_interfaces_meter_supported_report_t *supported_report;

	if ((state = (zwp_avi_interfaces_meter_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((supported_report = (zwp_avi_interfaces_meter_supported_report_t *)ZWP_CALLOC(1, sizeof(*supported_report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	memcpy(supported_report, &state->supported_report, sizeof(*supported_report));

	*result_p = supported_report;

	return ZWP_STATUS_OK;

	ZWP_FREE(supported_report);
l_err_report_alloc:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to Supported Report command
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_meter_supported_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_meter_supported_report_t *supported_report;

	supported_report = (zwp_avi_interfaces_meter_supported_report_t *)result;

	if ((zw_avro_field_set_long(arg_value, "utime", supported_report->utime) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "meter_type", supported_report->meter_capability.type) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "scale_supported", supported_report->meter_capability.unit_sup) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "meter_reset", supported_report->meter_capability.reset_cap) != ZWP_STATUS_OK))
	{
		goto l_err_set_report_value;
	}

	return ZWP_STATUS_OK;

l_err_set_report_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Supported Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_meter_supported_get_execute_passive
 */
static void zwp_avi_interfaces_meter_supported_get_result_free(void *result)
{
	zwp_avi_interfaces_meter_supported_report_t *supported_report;

	supported_report = (zwp_avi_interfaces_meter_supported_report_t *)result;

	ZWP_FREE(supported_report);
}

/**
 * @brief Execute Reset command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Reset command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_meter_reset_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	*hcapi_code_p = zwif_meter_reset(desc_interface);

	return ZWP_STATUS_OK;
}

/** Handlers for Get command */
static zwp_avi_message_interfaces_t interfaces_meter_get =
{
		"interface_meter_get",
		COMMAND_CLASS_METER, zwp_avi_interfaces_meter_setup,
		zwp_avi_interfaces_meter_state_alloc, zwp_avi_interfaces_meter_state_free,
		zwp_avi_interfaces_meter_get_argument, zwp_avi_interfaces_meter_get_argument_free,
		zwp_avi_interfaces_meter_get_report_receive_assertion,
		zwp_avi_interfaces_meter_get_execute_active, zwp_avi_interfaces_meter_get_execute_passive,
		zwp_avi_interfaces_meter_get_result, zwp_avi_interfaces_meter_get_result_free
};

/** Handlers for Supported Get command */
static zwp_avi_message_interfaces_t interfaces_meter_supported_get =
{
		"interface_meter_supported_get",
		COMMAND_CLASS_METER, NULL,
		NULL, NULL,
		NULL, NULL,
		zwp_avi_interfaces_meter_supported_get_report_receive_assertion,
		zwp_avi_interfaces_meter_supported_get_execute_active, zwp_avi_interfaces_meter_supported_get_execute_passive,
		zwp_avi_interfaces_meter_supported_get_result, zwp_avi_interfaces_meter_supported_get_result_free
};

/** Handlers for Reset command */
static zwp_avi_message_interfaces_t interfaces_meter_reset =
{
		"interface_meter_reset",
		COMMAND_CLASS_METER, NULL,
		NULL, NULL,
		NULL, NULL,
		NULL,
		zwp_avi_interfaces_meter_reset_execute_active, NULL,
		NULL, NULL
};

/** Register command handlers */
zwp_status_t zwp_avi_interfaces_meter_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_meter_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_meter_supported_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_meter_reset) != ZWP_STATUS_OK))
	{
		goto l_err_interface_list_get;
	}

	return ZWP_STATUS_OK;

l_err_interface_list_get:
	return ZWP_STATUS_ERROR;
}

