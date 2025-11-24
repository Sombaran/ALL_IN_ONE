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
 * @file    zwp_avi_interfaces_thermostat_setpoint.c
 *
 * @brief   AVI message 'interface_thermostat_setpoint_xxx'
 * @details Handles the AVI message 'interface_thermostat_setpoint_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-10-10
 * - Initial version
 */

#include "zwp_avi_interfaces.h"

/** Setpoint type not specified */
#define COMMAND_THERMOSTAT_SETPOINT_TYPE_NONE         (-1)

/** Get command state */
typedef struct {
	int                setpoint_type;                      /**< Setpoint type */
} zwp_avi_interfaces_thermostat_setpoint_get_t;

/** Set command state */
typedef struct {
	zwsetp_t           setpoint;                           /**< Setpoint */
} zwp_avi_interfaces_thermostat_setpoint_set_t;

/** Setpoint value description */
typedef struct {
	zwu_time_t         utime;                              /**< Last update time for specific type and scale combination */
	zwsetp_t           value;                              /**< Setpoint value */
} zwp_avi_interfaces_thermostat_setpoint_report_thermostat_setpoint_value_t;

/** Report command state */
typedef struct {
	zwu_time_t         utime;                              /**< Last update time across all types */
	zwu_array_list_t   thermostat_setpoint_value_list;     /**< List of setpoint values of type @a zwp_avi_interfaces_thermostat_setpoint_report_thermostat_setpoint_value_t */

	int                report_receive_state;               /**< Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Report command receive condition */
	int                report_receive_setpoint_type;       /**< Current setpoint type for Report command receive condition */
} zwp_avi_interfaces_thermostat_setpoint_report_t;

/** Supported Report command state */
typedef struct {
	zwu_time_t         utime;                              /**< Last updated time of Supported Report command */
	uint8_t            *supported_types;                   /**< List of supported setpoint types */
	uint8_t            supported_types_count;              /**< Count of supported setpoint types in the list @a supported_types */

	int                report_receive_state;               /**< Supported Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Supported Report command receive condition */
} zwp_avi_interfaces_thermostat_setpoint_supported_report_t;

/** Setpoint Range description */
typedef struct
{
	zwu_time_t         utime;                              /**< Last update time for specific type and scale combination */
	uint8_t			   type;							   /**< Setpoint type */
	zwsetp_t           range_min;                          /**< Minimum value supported */
	zwsetp_t           range_max;                          /**< Maximum value supported */
} zwp_avi_interfaces_thermostat_setpoint_range_report_single_t;

/** Range Report command state */
typedef struct
{
	zwu_time_t         utime;                              /**< Last update time across all types */
	zwu_array_list_t   thermostat_setpoint_range_list;     /**< List of setpoint values of type @a zwp_avi_interfaces_thermostat_setpoint_range_report_single_t */

	int                report_receive_state;               /**< Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Report command receive condition */
	int                report_receive_setpoint_type;       /**< Current setpoint type for Range Report command receive condition */
} zwp_avi_interfaces_thermostat_setpoint_range_report_t;

/** Interface level state */
typedef struct {
	zwp_avi_interfaces_thermostat_setpoint_report_t                report;					/**< Report command state */
	zwp_avi_interfaces_thermostat_setpoint_supported_report_t      supported_report;		/**< Supported Report command state */
	zwp_avi_interfaces_thermostat_setpoint_range_report_t		   range_report;			/**< Range Report command state */
} zwp_avi_interfaces_thermostat_setpoint_state_t;

#ifndef __GNUC__
#pragma region Get Command
#endif

/**
 * @brief Allocate interface level data
 * @param[out]    state_p           Interface level data
 * @param[in]     pool              Memory pool to be used for allocation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_thermostat_setpoint_state_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_state_alloc(void **state_p, zwp_pool_t pool)
{
	zwp_avi_interfaces_thermostat_setpoint_state_t *state;

	if ((state = (zwp_avi_interfaces_thermostat_setpoint_state_t *)ZWP_CALLOC(1, sizeof(*state))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_state_alloc;
	}

	//Get command
	if (zwp_thread_cond_create(&state->report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for report received event");
		goto l_err_net_report_received_cond_create;
	}

	state->report.report_receive_setpoint_type = COMMAND_THERMOSTAT_SETPOINT_TYPE_NONE;

	if (zwu_array_list_create(0, &state->report.thermostat_setpoint_value_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	//Supported Get command
	if (zwp_thread_cond_create(&state->supported_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for supported_report received event");
		goto l_err_net_supported_report_received_cond_create;
	}

	state->supported_report.supported_types = NULL;
	state->supported_report.supported_types_count = 0;

	//Range Get report
	if (zwp_thread_cond_create(&state->range_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for range report received event");
		goto l_err_net_range_report_received_cond_create;
	}

	state->range_report.report_receive_setpoint_type = COMMAND_THERMOSTAT_SETPOINT_TYPE_NONE;

	if (zwu_array_list_create(0, &state->range_report.thermostat_setpoint_range_list) != ZWP_STATUS_OK)
	{
		goto l_err_range_list_create;
	}

	*state_p = state;

	return ZWP_STATUS_OK;

	zwu_array_list_destroy(state->range_report.thermostat_setpoint_range_list);
l_err_range_list_create:
	zwp_thread_cond_destroy(state->range_report.report_receive_condition);
l_err_net_range_report_received_cond_create:
	zwp_thread_cond_destroy(state->supported_report.report_receive_condition);
l_err_net_supported_report_received_cond_create:
	zwu_array_list_destroy(state->report.thermostat_setpoint_value_list);
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
 * @see zwp_avi_interfaces_thermostat_setpoint_state_alloc
 */
static void zwp_avi_interfaces_thermostat_setpoint_state_free(void *arg)
{
	zwp_avi_interfaces_thermostat_setpoint_state_t *state;
	zwp_avi_interfaces_thermostat_setpoint_report_t *report;
	zwp_avi_interfaces_thermostat_setpoint_range_report_t *range_report;
	int thermostat_setpoint_value_count, thermostat_setpoint_range_count;
	int i;

	state = (zwp_avi_interfaces_thermostat_setpoint_state_t *)arg;
	report = &state->report;
	range_report = &state->range_report;

	//Range Get command
	thermostat_setpoint_range_count = zwu_array_list_size(range_report->thermostat_setpoint_range_list);

	for (i = 0; i < thermostat_setpoint_range_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(range_report->thermostat_setpoint_range_list, i));
	}

	zwu_array_list_destroy(range_report->thermostat_setpoint_range_list);

	zwp_interface_report_receive_done(&state->range_report.report_receive_state, state->range_report.report_receive_condition);

	zwp_thread_cond_destroy(state->range_report.report_receive_condition);

	//Supported Get command
	zwp_interface_report_receive_done(&state->supported_report.report_receive_state, state->supported_report.report_receive_condition);

	zwp_thread_cond_destroy(state->supported_report.report_receive_condition);

	ZWP_FREE(state->supported_report.supported_types);

	//Get command
	thermostat_setpoint_value_count = zwu_array_list_size(report->thermostat_setpoint_value_list);

	for (i = 0; i < thermostat_setpoint_value_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(report->thermostat_setpoint_value_list, i));
	}

	zwu_array_list_destroy(report->thermostat_setpoint_value_list);

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
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_report_update(zwp_avi_interfaces_thermostat_setpoint_report_t *report,
		zwsetp_p data, zwu_time_t ts)
{
	int thermostat_setpoint_value_count;
	int i;
	//zwu_time_t  time_now;
	zwp_avi_interfaces_thermostat_setpoint_report_thermostat_setpoint_value_t *thermostat_setpoint_value;

	thermostat_setpoint_value_count = zwu_array_list_size(report->thermostat_setpoint_value_list);

	for (i = 0; i < thermostat_setpoint_value_count; i++)
	{
		thermostat_setpoint_value = zwu_array_list_get(report->thermostat_setpoint_value_list, i);

		/*
		 * TODO: Use type-unit combo after the following fixes are done.
		 * Engg UI: If reports differ only by scale and not type, the UI shows multiple listings as
		 * expected. But, the set controls refreshes continuously because the utime from multiple
		 * listings are not handled properly.
		 * Production UI: If reports differ only by scale and not type, the UI shows only one listing
		 * that corresponds to the first record which may not be the most recent for the given type. The
		 * expectation is to show multiple listings or the one with the most recent utime.
		 */
		// If both type and scale matches, update the entry instead of creating new one.
		// if ((thermostat_setpoint_value->value.type == data->type) && (thermostat_setpoint_value->value.unit == data->unit))

		// If type matches, update the entry instead of creating new one.
		if (thermostat_setpoint_value->value.type == data->type)
		{
			break;
		}
	}

	if (i >= thermostat_setpoint_value_count)
	{
		if ((thermostat_setpoint_value = (zwp_avi_interfaces_thermostat_setpoint_report_thermostat_setpoint_value_t *)ZWP_CALLOC(1, sizeof(*thermostat_setpoint_value))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		if (zwu_array_list_add(report->thermostat_setpoint_value_list, thermostat_setpoint_value) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}
	}

	//time_now = zwu_time_now();

	//report->utime = time_now;
	//thermostat_setpoint_value->utime = time_now;
	report->utime = ts;
	thermostat_setpoint_value->utime = ts;
	memcpy(&thermostat_setpoint_value->value, data, sizeof(thermostat_setpoint_value->value));

	/*
	 * To avoid usage of uninitialized portion in Avro message
	 */
	memset(thermostat_setpoint_value->value.data, 0, sizeof(thermostat_setpoint_value->value.data));
	memcpy(thermostat_setpoint_value->value.data, data->data, data->size);

	return ZWP_STATUS_OK;

l_err_value_list_add:
	ZWP_FREE(thermostat_setpoint_value);
l_err_value_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief HCAPI callback for Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  data	            setpoint data
 * @param[in]	  ts      			time stamp
 * @return        Nothing
 */
static void zwp_avi_interfaces_thermostat_setpoint_report_handler(zwifd_p ifd, zwsetp_p data, time_t ts)
{
	zwp_avi_interfaces_thermostat_setpoint_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_thermostat_setpoint_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (zwp_avi_interfaces_thermostat_setpoint_report_update(&state->report, data, ts) != ZWP_STATUS_OK)
	{
		goto l_err_report_update;
	}

	if(ts)
	{
		state->report.report_receive_setpoint_type = data->type;
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
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_setup(zwifd_p desc_interface, int *hcapi_code_p)
{
	*hcapi_code_p = hcaal_thrmo_setp_rpt_set(desc_interface, zwp_avi_interfaces_thermostat_setpoint_report_handler);

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
 * The caller should call zwp_avi_interfaces_thermostat_setpoint_get_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_get_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_thermostat_setpoint_get_t *argument;
	int integer;

	if ((argument = (zwp_avi_interfaces_thermostat_setpoint_get_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "setpoint_type", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_get_value;
	}
	argument->setpoint_type = integer;

	ZWP_LOG(ZWP_LOG_DEBUG, "Setpoint Type: %d", argument->setpoint_type);

	if ((argument->setpoint_type != COMMAND_THERMOSTAT_SETPOINT_TYPE_NONE)
			&& (zwu_number_is_uint8(argument->setpoint_type) != ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid setpoint type: %d", argument->setpoint_type);
		goto l_err_invalid_setpoint_type;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_invalid_setpoint_type:
l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Get command
 * @param[in]     args              Arguments passed with Get command
 * @return        Nothing
 * @see zwp_avi_interfaces_thermostat_setpoint_get_argument
 */
static void zwp_avi_interfaces_thermostat_setpoint_get_argument_free(void *args)
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
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_thermostat_setpoint_state_t *state;
	zwp_avi_interfaces_thermostat_setpoint_get_t *argument;

	argument = (zwp_avi_interfaces_thermostat_setpoint_get_t *)args;

	if ((state = (zwp_avi_interfaces_thermostat_setpoint_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->report.report_receive_condition;

	if ((zwp_interface_report_receive_assertion(state->report.report_receive_state) != ZWP_STATUS_OK)
			|| ((argument->setpoint_type != COMMAND_THERMOSTAT_SETPOINT_TYPE_NONE) && (state->report.report_receive_setpoint_type != argument->setpoint_type)))
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
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_thermostat_setpoint_state_t *state;
	zwp_avi_interfaces_thermostat_setpoint_get_t *argument;
	uint8_t setpoint_type;

	argument = (zwp_avi_interfaces_thermostat_setpoint_get_t *)args;

	if ((state = (zwp_avi_interfaces_thermostat_setpoint_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->report.report_receive_state);

	if (argument->setpoint_type == COMMAND_THERMOSTAT_SETPOINT_TYPE_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Missing one or more required arguments");
		goto l_err_invalid_argument;
	}

	setpoint_type = (uint8_t)argument->setpoint_type;

	*hcapi_code_p = hcaal_thrmo_setp_get(desc_interface, setpoint_type, ZWIF_GET_BMSK_LIVE | ZWIF_GET_BMSK_CACHE);

	return ZWP_STATUS_OK;

l_err_invalid_argument:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Allocate Result object corresponding to Report command
 * @param[out]    report_dest       Destination Report
 * @param[in]     report_src        Source Report
 * @param[in]     setpoint_type     Setpoint type
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_get_result_alloc(zwp_avi_interfaces_thermostat_setpoint_report_t *report_dest,
		zwp_avi_interfaces_thermostat_setpoint_report_t *report_src, int setpoint_type)
{
	int thermostat_setpoint_value_count;
	int i;
	zwp_avi_interfaces_thermostat_setpoint_report_thermostat_setpoint_value_t *thermostat_setpoint_value_dest;
	zwp_avi_interfaces_thermostat_setpoint_report_thermostat_setpoint_value_t *thermostat_setpoint_value_src;

	thermostat_setpoint_value_count = zwu_array_list_size(report_src->thermostat_setpoint_value_list);

	for (i = 0; i < thermostat_setpoint_value_count; i++)
	{
		thermostat_setpoint_value_src = zwu_array_list_get(report_src->thermostat_setpoint_value_list, i);

		// If setpoint_type doesn't match, skip the entry
		if ((setpoint_type != COMMAND_THERMOSTAT_SETPOINT_TYPE_NONE) && (thermostat_setpoint_value_src->value.type != setpoint_type))
		{
			continue;
		}

		if ((thermostat_setpoint_value_dest = (zwp_avi_interfaces_thermostat_setpoint_report_thermostat_setpoint_value_t *)ZWP_CALLOC(1, sizeof(*thermostat_setpoint_value_dest))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		if (zwu_array_list_add(report_dest->thermostat_setpoint_value_list, thermostat_setpoint_value_dest) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}

		memcpy(thermostat_setpoint_value_dest, thermostat_setpoint_value_src, sizeof(*thermostat_setpoint_value_dest));
	}

	return ZWP_STATUS_OK;

l_err_value_list_add:
	ZWP_FREE(thermostat_setpoint_value_dest);
l_err_value_alloc:
	for (i--; i >= 0; i--)
	{
		ZWP_FREE(zwu_array_list_get(report_dest->thermostat_setpoint_value_list, i));
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
 * The caller should call zwp_avi_interfaces_thermostat_setpoint_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_thermostat_setpoint_state_t *state;
	zwp_avi_interfaces_thermostat_setpoint_report_t *report;
	zwp_avi_interfaces_thermostat_setpoint_get_t *argument;

	argument = (zwp_avi_interfaces_thermostat_setpoint_get_t *)args;

	if ((state = (zwp_avi_interfaces_thermostat_setpoint_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((report = (zwp_avi_interfaces_thermostat_setpoint_report_t *)ZWP_CALLOC(1, sizeof(*report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	if (zwu_array_list_create(0, &report->thermostat_setpoint_value_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	if (zwp_avi_interfaces_thermostat_setpoint_get_result_alloc(report, &state->report, argument->setpoint_type) != ZWP_STATUS_OK)
	{
		goto l_err_result_alloc;
	}

	*result_p = report;

	return ZWP_STATUS_OK;

l_err_result_alloc:
	zwu_array_list_destroy(report->thermostat_setpoint_value_list);
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
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_thermostat_setpoint_report_t *report;
	avro_value_t thermostat_setpoint_value_list_value;
	avro_value_t thermostat_setpoint_value_value;
	zwp_avi_interfaces_thermostat_setpoint_report_thermostat_setpoint_value_t *thermostat_setpoint_value;
	int thermostat_setpoint_value_count;
	int i;

	report = (zwp_avi_interfaces_thermostat_setpoint_report_t *)result;

	if (avro_value_get_by_name(arg_value, "thermostat_setpoint_value_list", &thermostat_setpoint_value_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'thermostat_setpoint_value_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	thermostat_setpoint_value_count = zwu_array_list_size(report->thermostat_setpoint_value_list);

	for (i = 0; i < thermostat_setpoint_value_count; i++)
	{
		if (avro_value_append(&thermostat_setpoint_value_list_value, &thermostat_setpoint_value_value, 0) != 0)
		{
			goto l_err_append_thermostat_setpoint_value_value;
		}

		thermostat_setpoint_value = zwu_array_list_get(report->thermostat_setpoint_value_list, i);

		if ((zw_avro_field_set_long(&thermostat_setpoint_value_value, "utime", thermostat_setpoint_value->utime) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&thermostat_setpoint_value_value, "setpoint_type", thermostat_setpoint_value->value.type) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&thermostat_setpoint_value_value, "precision", thermostat_setpoint_value->value.precision) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&thermostat_setpoint_value_value, "scale", thermostat_setpoint_value->value.unit) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&thermostat_setpoint_value_value, "size", thermostat_setpoint_value->value.size) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_fixed(&thermostat_setpoint_value_value, "value", thermostat_setpoint_value->value.data, 4) != ZWP_STATUS_OK)
				)
		{
			goto l_err_set_thermostat_setpoint_value_value;
		}
	}

	return ZWP_STATUS_OK;

l_err_set_thermostat_setpoint_value_value:
l_err_append_thermostat_setpoint_value_value:
l_err_get_list_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_thermostat_setpoint_get_execute_passive
 */
static void zwp_avi_interfaces_thermostat_setpoint_get_result_free(void *result)
{
	zwp_avi_interfaces_thermostat_setpoint_report_t *report;
	int thermostat_setpoint_value_count;
	int i;

	report = (zwp_avi_interfaces_thermostat_setpoint_report_t *)result;

	thermostat_setpoint_value_count = zwu_array_list_size(report->thermostat_setpoint_value_list);

	for (i = 0; i < thermostat_setpoint_value_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(report->thermostat_setpoint_value_list, i));
	}
	zwu_array_list_destroy(report->thermostat_setpoint_value_list);

	ZWP_FREE(report);
}
#ifndef __GNUC__
#pragma endregion
#endif

#ifndef __GNUC__
#pragma region Set Command
#endif
/**
 * @brief Get argument for Set command from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Set command
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_thermostat_setpoint_set_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_set_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_thermostat_setpoint_set_t *argument;
	int32_t setpoint_type;
	int32_t precision;
	int32_t scale;
	int32_t size;
	void *setpoint_value;
	size_t setpoint_value_size;

	if ((argument = (zwp_avi_interfaces_thermostat_setpoint_set_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if ((zw_avro_field_get_int(arg_value, "setpoint_type", &setpoint_type) != ZWP_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "precision", &precision) != ZWP_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "scale", &scale) != ZWP_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "size", &size) != ZWP_STATUS_OK)
			)
	{
		goto l_err_get_value;
	}

	if (zw_avro_field_get_fixed(arg_value, "value", &setpoint_value, &setpoint_value_size) != ZWP_STATUS_OK)
	{
		goto l_err_get_thermostat_setpoint_value;
	}

	if (setpoint_value_size != 4)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid setpoint value field size");
		goto l_err_thermostat_setpoint_size;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Setpoint Type: %d", setpoint_type);
	ZWP_LOG(ZWP_LOG_DEBUG, "Value: 0x%02X%02X%02X%02X",
			((uint8_t *)setpoint_value)[0], ((uint8_t *)setpoint_value)[1],
			((uint8_t *)setpoint_value)[2], ((uint8_t *)setpoint_value)[3]);
	ZWP_LOG(ZWP_LOG_DEBUG, "Precision: %d", precision);
	ZWP_LOG(ZWP_LOG_DEBUG, "scale: %d", scale);
	ZWP_LOG(ZWP_LOG_DEBUG, "size: %d", size);

	if (zwu_number_is_uint8(setpoint_type) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid setpoint type: %d", setpoint_type);
		goto l_err_invalid_setpoint_type;
	}

	if (zwu_number_is_uint8(precision) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid precision: %d", precision);
		goto l_err_invalid_precision;
	}

	if (zwu_number_is_uint8(scale) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid scale: %d", scale);
		goto l_err_invalid_scale;
	}

	if (zwu_number_is_uint8(size) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid size: %d", size);
		goto l_err_invalid_size;
	}

	argument->setpoint.type = (uint8_t)setpoint_type;
	argument->setpoint.precision = (uint8_t)precision;
	argument->setpoint.unit = (uint8_t)scale;
	argument->setpoint.size = (uint8_t)size;

	memset(argument->setpoint.data, 0, sizeof(argument->setpoint.data));

	memcpy(argument->setpoint.data, setpoint_value, sizeof(argument->setpoint.data));

	zw_avro_field_free_fixed(setpoint_value);

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_invalid_size:
l_err_invalid_scale:
l_err_invalid_precision:
l_err_invalid_setpoint_type:
l_err_thermostat_setpoint_size:
	zw_avro_field_free_fixed(setpoint_value);
l_err_get_thermostat_setpoint_value:
l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Set command
 * @param[in]     args              Arguments passed with Set command
 * @return        Nothing
 * @see zwp_avi_interfaces_thermostat_setpoint_set_argument
 */
static void zwp_avi_interfaces_thermostat_setpoint_set_argument_free(void *args)
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
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_set_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_thermostat_setpoint_set_t *argument;

	argument = (zwp_avi_interfaces_thermostat_setpoint_set_t *)args;

	*hcapi_code_p = zwif_thrmo_setp_set(desc_interface, &argument->setpoint);

	return ZWP_STATUS_OK;
}
#ifndef __GNUC__
#pragma endregion
#endif

#ifndef __GNUC__
#pragma region Supported Get Command
#endif
/**
 * @brief HCAPI callback for Supported Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  type_len          size of type buffer
 * @param[in]	  type              buffer to store supported thermostat setpoint types (ZW_THRMO_SETP_TYP_XXX)
 * @return        Nothing
 */
static void zwp_avi_interfaces_thermostat_setpoint_supported_report_handler(zwifd_p ifd, uint8_t type_len, uint8_t *type, int valid)
{
	zwp_avi_interfaces_thermostat_setpoint_state_t *state;
	zwifd_p desc_interface;
	uint8_t *tmp;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_thermostat_setpoint_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (valid)
	{
		state->supported_report.utime = zwu_time_now();

		if (type_len > state->supported_report.supported_types_count)
		{
			if ((tmp = (uint8_t *)ZWP_REALLOC(state->supported_report.supported_types, sizeof(*tmp) * type_len)) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				goto l_err_type_list_alloc;
			}

			state->supported_report.supported_types = tmp;
		}
		state->supported_report.supported_types_count = type_len;

		memcpy(state->supported_report.supported_types, type, state->supported_report.supported_types_count);
	}
	zwp_interface_report_receive_done(&state->supported_report.report_receive_state, state->supported_report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

l_err_type_list_alloc:
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
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_supported_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_thermostat_setpoint_state_t *state;

	if ((state = (zwp_avi_interfaces_thermostat_setpoint_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
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
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_supported_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_thermostat_setpoint_state_t *state;

	if ((state = (zwp_avi_interfaces_thermostat_setpoint_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->supported_report.report_receive_state);

	*hcapi_code_p = zwif_thrmo_setp_sup_get(desc_interface, zwp_avi_interfaces_thermostat_setpoint_supported_report_handler, 0);

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
 * The caller should call zwp_avi_interfaces_thermostat_setpoint_supported_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_supported_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_thermostat_setpoint_state_t *state;
	zwp_avi_interfaces_thermostat_setpoint_supported_report_t *supported_report;

	if ((state = (zwp_avi_interfaces_thermostat_setpoint_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((supported_report = (zwp_avi_interfaces_thermostat_setpoint_supported_report_t *)ZWP_CALLOC(1, sizeof(*supported_report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	memcpy(supported_report, &state->supported_report, sizeof(*supported_report));

	if ((supported_report->supported_types = (uint8_t *)ZWP_CALLOC(1,
			sizeof(*supported_report->supported_types) * state->supported_report.supported_types_count)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_type_list_alloc;
	}

	memcpy(supported_report->supported_types, state->supported_report.supported_types, state->supported_report.supported_types_count);

	*result_p = supported_report;

	return ZWP_STATUS_OK;

	ZWP_FREE(supported_report->supported_types);
l_err_type_list_alloc:
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
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_supported_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_thermostat_setpoint_supported_report_t *supported_report;
	avro_value_t setpoint_type_list_value;
	avro_value_t setpoint_type_value;
	int i;

	supported_report = (zwp_avi_interfaces_thermostat_setpoint_supported_report_t *)result;

	if (zw_avro_field_set_long(arg_value, "utime", supported_report->utime) != ZWP_STATUS_OK)
	{
		goto l_err_set_report_value;
	}

	if (avro_value_get_by_name(arg_value, "supported_types", &setpoint_type_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'supported_types': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	for (i = 0; i < supported_report->supported_types_count; i++)
	{
		if (avro_value_append(&setpoint_type_list_value, &setpoint_type_value, 0) != 0)
		{
			goto l_err_append_setpoint_type_value;
		}

		if (avro_value_set_int(&setpoint_type_value, supported_report->supported_types[i]) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to set setpoint type '%d': %s", supported_report->supported_types[i], avro_strerror());
			goto l_err_set_setpoint_type;
		}
	}

	return ZWP_STATUS_OK;

l_err_set_setpoint_type:
l_err_append_setpoint_type_value:
l_err_get_list_value:
l_err_set_report_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Supported Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_thermostat_setpoint_supported_get_execute_passive
 */
static void zwp_avi_interfaces_thermostat_setpoint_supported_get_result_free(void *result)
{
	zwp_avi_interfaces_thermostat_setpoint_supported_report_t *supported_report;

	supported_report = (zwp_avi_interfaces_thermostat_setpoint_supported_report_t *)result;

	ZWP_FREE(supported_report->supported_types);

	ZWP_FREE(supported_report);
}
#ifndef __GNUC__
#pragma endregion
#endif

#ifndef __GNUC__
#pragma region Range Get Command
#endif
/**
* @brief Range Get argument for Get command from Avro value object
* @param[in]     arg_value         Avro value object
* @param[out]    args_p            Arguments passed with Get command
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zwp_avi_interfaces_thermostat_setpoint_range_get_argument_free() to free resources
*/
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_range_get_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_thermostat_setpoint_get_t *argument;
	int integer;

	if ((argument = (zwp_avi_interfaces_thermostat_setpoint_get_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "setpoint_type", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_get_value;
	}
	argument->setpoint_type = integer;

	ZWP_LOG(ZWP_LOG_DEBUG, "Range Get Setpoint Type: %d", argument->setpoint_type);

	if ((argument->setpoint_type != COMMAND_THERMOSTAT_SETPOINT_TYPE_NONE)
		&& (zwu_number_is_uint8(argument->setpoint_type) != ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid setpoint type: %d", argument->setpoint_type);
		goto l_err_invalid_setpoint_type;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_invalid_setpoint_type:
l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free arguments for Range Get command
* @param[in]     args              Arguments passed with Get command
* @return        Nothing
* @see zwp_avi_interfaces_thermostat_setpoint_range_get_argument
*/
static void zwp_avi_interfaces_thermostat_setpoint_range_get_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
* @brief HCAPI callback for Rnage Report command
* @param[in]	  ifd	            interface
* @param[in]	  data	            setpoint data
* @param[in]	  ts      			time stamp
* @return        Nothing
*/
static void zwp_avi_interfaces_thermostat_setpoint_range_report_handler(zwifd_p ifd, uint8_t type, zwsetp_p min, zwsetp_p max, int valid)
{
	zwp_avi_interfaces_thermostat_setpoint_state_t *state;
	zwifd_p desc_interface;
	int range_list_count, i;
	zwp_avi_interfaces_thermostat_setpoint_range_report_single_t  *pRange_per_type;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_thermostat_setpoint_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (valid)
	{
		state->range_report.utime = zwu_time_now();
		state->range_report.report_receive_setpoint_type = type;

		range_list_count = zwu_array_list_size(state->range_report.thermostat_setpoint_range_list);

		ZWP_LOG(ZWP_LOG_DEBUG, "range_list_count %d", range_list_count);

		for (i = 0; i < range_list_count; i++)
		{
			pRange_per_type = (zwp_avi_interfaces_thermostat_setpoint_range_report_single_t *)zwu_array_list_get(state->range_report.thermostat_setpoint_range_list, i);

			if (pRange_per_type->type == type)
			{
				//match found, delete the current data
				memset(pRange_per_type, 0, sizeof(zwsetp_t));
				break;
			}
		}

		if (i >= range_list_count)
		{
			//Cannot find a match
			if ((pRange_per_type = (zwp_avi_interfaces_thermostat_setpoint_range_report_single_t *)ZWP_CALLOC(1, sizeof(zwp_avi_interfaces_thermostat_setpoint_range_report_single_t))) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				goto l_err_range_struct_alloc;
			}

			if (zwu_array_list_add(state->range_report.thermostat_setpoint_range_list, pRange_per_type) != ZWP_STATUS_OK)
			{
				goto l_err_range_list_add;
			}
		}

		pRange_per_type->utime = state->range_report.utime;
		pRange_per_type->type = type;
		memcpy(&pRange_per_type->range_min, min, sizeof(zwsetp_t));
		memcpy(&pRange_per_type->range_max, max, sizeof(zwsetp_t));
		
	}
	zwp_interface_report_receive_done(&state->range_report.report_receive_state, state->range_report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

l_err_range_list_add:
	if (i >= range_list_count)
	{
		ZWP_FREE(pRange_per_type);
	}
l_err_range_struct_alloc:
l_err_get_state:
	zwp_net_release_desc_interface(ifd);
l_err_acquire_desc:

	return;
}

/**
* @brief Test if the expected Report command is received
* @param[in]     desc_interface    Interface descriptor
* @param[in]     args              Arguments (if any) passed with Get command
* @param[out]    report_receive_condition_p    Report command receive condition
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_range_get_report_receive_assertion(zwifd_p desc_interface, void *args,
	zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_thermostat_setpoint_state_t *state;
	zwp_avi_interfaces_thermostat_setpoint_get_t *argument;

	argument = (zwp_avi_interfaces_thermostat_setpoint_get_t *)args;

	if ((state = (zwp_avi_interfaces_thermostat_setpoint_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->range_report.report_receive_condition;

	if ((zwp_interface_report_receive_assertion(state->range_report.report_receive_state) != ZWP_STATUS_OK)
		|| ((argument->setpoint_type != COMMAND_THERMOSTAT_SETPOINT_TYPE_NONE) && (state->range_report.report_receive_setpoint_type != argument->setpoint_type)))
	{
		goto l_err_report_not_received;
	}

	return ZWP_STATUS_OK;

l_err_report_not_received:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Execute active Range Get command
* @param[in]     desc_interface    Interface descriptor
* @param[in]     args              Arguments (if any) passed with Get command
* @param[out]    hcapi_code_p      HCAPI error code
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_range_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_thermostat_setpoint_state_t *state;
	zwp_avi_interfaces_thermostat_setpoint_get_t *argument;
	uint8_t setpoint_type;

	argument = (zwp_avi_interfaces_thermostat_setpoint_get_t *)args;

	if ((state = (zwp_avi_interfaces_thermostat_setpoint_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->range_report.report_receive_state);

	if (argument->setpoint_type == COMMAND_THERMOSTAT_SETPOINT_TYPE_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Missing one or more required arguments");
		goto l_err_invalid_argument;
	}

	setpoint_type = (uint8_t)argument->setpoint_type;

	*hcapi_code_p = zwif_thrmo_setp_sup_range_get(desc_interface, setpoint_type, zwp_avi_interfaces_thermostat_setpoint_range_report_handler, 0);

	return ZWP_STATUS_OK;

l_err_invalid_argument:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Allocate Result object corresponding to Range Report command
* @param[out]    report_dest       Destination Report
* @param[in]     report_src        Source Report
* @param[in]     setpoint_type     Setpoint type
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_range_get_result_alloc(
	zwp_avi_interfaces_thermostat_setpoint_range_report_t *report_dest,
	zwp_avi_interfaces_thermostat_setpoint_range_report_t *report_src,
	int setpoint_type)
{
	int thermostat_setpoint_range_count;
	int i;
	zwp_avi_interfaces_thermostat_setpoint_range_report_single_t *range_single_dest;
	zwp_avi_interfaces_thermostat_setpoint_range_report_single_t *range_single_src;

	thermostat_setpoint_range_count = zwu_array_list_size(report_src->thermostat_setpoint_range_list);

	for (i = 0; i < thermostat_setpoint_range_count; i++)
	{
		range_single_src = zwu_array_list_get(report_src->thermostat_setpoint_range_list, i);

		// If setpoint_type doesn't match, skip the entry
		if ((setpoint_type != COMMAND_THERMOSTAT_SETPOINT_TYPE_NONE) && (range_single_src->type != setpoint_type))
		{
			continue;
		}

		if ((range_single_dest = (zwp_avi_interfaces_thermostat_setpoint_range_report_single_t *)ZWP_CALLOC(1, sizeof(*range_single_dest))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_range_alloc;
		}

		if (zwu_array_list_add(report_dest->thermostat_setpoint_range_list, range_single_dest) != ZWP_STATUS_OK)
		{
			goto l_err_range_list_add;
		}

		memcpy(range_single_dest, range_single_src, sizeof(*range_single_src));
	}

	return ZWP_STATUS_OK;

l_err_range_list_add:
	ZWP_FREE(range_single_dest);
l_err_range_alloc:
	for (i--; i >= 0; i--)
	{
		ZWP_FREE(zwu_array_list_get(report_dest->thermostat_setpoint_range_list, i));
	}
	return ZWP_STATUS_ERROR;
}

/**
* @brief Execute passive Range Get command
* @param[in]     desc_interface    Interface descriptor
* @param[in]     args              Arguments (if any) passed with Get command
* @param[out]    result_p          Result object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zwp_avi_interfaces_thermostat_setpoint_range_get_result_free() to free resources
*/
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_range_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_thermostat_setpoint_state_t *state;
	zwp_avi_interfaces_thermostat_setpoint_range_report_t *range_report;
	zwp_avi_interfaces_thermostat_setpoint_get_t *argument;

	argument = (zwp_avi_interfaces_thermostat_setpoint_get_t *)args;

	if ((state = (zwp_avi_interfaces_thermostat_setpoint_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((range_report = (zwp_avi_interfaces_thermostat_setpoint_range_report_t *)ZWP_CALLOC(1, sizeof(*range_report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	if (zwu_array_list_create(0, &range_report->thermostat_setpoint_range_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	if (zwp_avi_interfaces_thermostat_setpoint_range_get_result_alloc(range_report, &state->range_report, argument->setpoint_type) != ZWP_STATUS_OK)
	{
		goto l_err_result_alloc;
	}

	*result_p = range_report;

	return ZWP_STATUS_OK;

l_err_result_alloc:
	zwu_array_list_destroy(range_report->thermostat_setpoint_range_list);
l_err_value_list_create:
	ZWP_FREE(range_report);
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
static zwp_status_t zwp_avi_interfaces_thermostat_setpoint_range_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_thermostat_setpoint_range_report_t *range_report;
	avro_value_t thermostat_setpoint_range_list_value;
	avro_value_t thermostat_setpoint_range_value;
	avro_value_t range_field;
	zwp_avi_interfaces_thermostat_setpoint_range_report_single_t *range_value;
	int thermostat_setpoint_range_count;
	int i;

	range_report = (zwp_avi_interfaces_thermostat_setpoint_range_report_t *)result;

	if (avro_value_get_by_name(arg_value, "thermostat_setpoint_range_list", &thermostat_setpoint_range_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'thermostat_setpoint_range_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	thermostat_setpoint_range_count = zwu_array_list_size(range_report->thermostat_setpoint_range_list);

	for (i = 0; i < thermostat_setpoint_range_count; i++)
	{
		if (avro_value_append(&thermostat_setpoint_range_list_value, &thermostat_setpoint_range_value, 0) != 0)
		{
			goto l_err_append_thermostat_setpoint_range_value;
		}

		range_value = zwu_array_list_get(range_report->thermostat_setpoint_range_list, i);

		if ((zw_avro_field_set_long(&thermostat_setpoint_range_value, "utime", range_value->utime) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(&thermostat_setpoint_range_value, "setpoint_type", range_value->type) != ZWP_STATUS_OK)
			)
		{
			goto l_err_set_thermostat_setpoint_range_value;
		}

		//Set range_min
		if (avro_value_get_by_name(&thermostat_setpoint_range_value, "range_min", &range_field, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'range_min' field from 'ThermostatSetpointRangeReport' record: %s", avro_strerror());
			goto l_err_get_range_min_field;
		}

		if((zw_avro_field_set_int(&range_field, "precision", range_value->range_min.precision) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(&range_field, "scale", range_value->range_min.unit) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(&range_field, "size", range_value->range_min.size) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_fixed(&range_field, "value", range_value->range_min.data, 4) != ZWP_STATUS_OK))
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to set 'range_min' valuesrecord: %s", avro_strerror());
			goto l_err_set_range_min_field;
		}

		//Set range_max
		if (avro_value_get_by_name(&thermostat_setpoint_range_value, "range_max", &range_field, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'range_max' field from 'ThermostatSetpointRangeReport' record: %s", avro_strerror());
			goto l_err_get_range_max_field;
		}

		if ((zw_avro_field_set_int(&range_field, "precision", range_value->range_max.precision) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(&range_field, "scale", range_value->range_max.unit) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(&range_field, "size", range_value->range_max.size) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_fixed(&range_field, "value", range_value->range_max.data, 4) != ZWP_STATUS_OK))
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to set 'range_max' valuesrecord: %s", avro_strerror());
			goto l_err_set_range_max_field;
		}
	}

	return ZWP_STATUS_OK;

l_err_set_range_max_field:
l_err_get_range_max_field:
l_err_set_range_min_field:
l_err_get_range_min_field:
l_err_set_thermostat_setpoint_range_value:
l_err_append_thermostat_setpoint_range_value:
l_err_get_list_value:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free Result object corresponding to Range Report command
* @param[in]     result            Result object
* @return        Nothing
* @see zwp_avi_interfaces_thermostat_setpoint_range_get_execute_passive
*/
static void zwp_avi_interfaces_thermostat_setpoint_range_get_result_free(void *result)
{
	zwp_avi_interfaces_thermostat_setpoint_range_report_t *range_report;
	int thermostat_setpoint_range_count;
	int i;

	range_report = (zwp_avi_interfaces_thermostat_setpoint_range_report_t *)result;

	thermostat_setpoint_range_count = zwu_array_list_size(range_report->thermostat_setpoint_range_list);

	for (i = 0; i < thermostat_setpoint_range_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(range_report->thermostat_setpoint_range_list, i));
	}
	zwu_array_list_destroy(range_report->thermostat_setpoint_range_list);

	ZWP_FREE(range_report);
}

#ifndef __GNUC__
#pragma endregion
#endif

/** Handlers for Get command */
static zwp_avi_message_interfaces_t interfaces_thermostat_setpoint_get =
{
		"interface_thermostat_setpoint_get",
		COMMAND_CLASS_THERMOSTAT_SETPOINT, zwp_avi_interfaces_thermostat_setpoint_setup,
		zwp_avi_interfaces_thermostat_setpoint_state_alloc, zwp_avi_interfaces_thermostat_setpoint_state_free,
		zwp_avi_interfaces_thermostat_setpoint_get_argument, zwp_avi_interfaces_thermostat_setpoint_get_argument_free,
		zwp_avi_interfaces_thermostat_setpoint_get_report_receive_assertion,
		zwp_avi_interfaces_thermostat_setpoint_get_execute_active, zwp_avi_interfaces_thermostat_setpoint_get_execute_passive,
		zwp_avi_interfaces_thermostat_setpoint_get_result, zwp_avi_interfaces_thermostat_setpoint_get_result_free
};

/** Handlers for Set command */
static zwp_avi_message_interfaces_t interfaces_thermostat_setpoint_set =
{
		"interface_thermostat_setpoint_set",
		COMMAND_CLASS_THERMOSTAT_SETPOINT, NULL,
		NULL, NULL,
		zwp_avi_interfaces_thermostat_setpoint_set_argument, zwp_avi_interfaces_thermostat_setpoint_set_argument_free,
		NULL,
		zwp_avi_interfaces_thermostat_setpoint_set_execute_active, NULL,
		NULL, NULL
};

/** Handlers for Supported Get command */
static zwp_avi_message_interfaces_t interfaces_thermostat_setpoint_supported_get =
{
		"interface_thermostat_setpoint_supported_get",
		COMMAND_CLASS_THERMOSTAT_SETPOINT, NULL,
		NULL, NULL,
		NULL, NULL,
		zwp_avi_interfaces_thermostat_setpoint_supported_get_report_receive_assertion,
		zwp_avi_interfaces_thermostat_setpoint_supported_get_execute_active, zwp_avi_interfaces_thermostat_setpoint_supported_get_execute_passive,
		zwp_avi_interfaces_thermostat_setpoint_supported_get_result, zwp_avi_interfaces_thermostat_setpoint_supported_get_result_free
};

/** Handlers for Capabilities Get command */
static zwp_avi_message_interfaces_t interfaces_thermostat_setpoint_range_get =
{
	"interface_thermostat_setpoint_range_get",
	COMMAND_CLASS_THERMOSTAT_SETPOINT, NULL,
	NULL, NULL,
	zwp_avi_interfaces_thermostat_setpoint_range_get_argument, zwp_avi_interfaces_thermostat_setpoint_range_get_argument_free,
	zwp_avi_interfaces_thermostat_setpoint_range_get_report_receive_assertion,
	zwp_avi_interfaces_thermostat_setpoint_range_get_execute_active, zwp_avi_interfaces_thermostat_setpoint_range_get_execute_passive,
	zwp_avi_interfaces_thermostat_setpoint_range_get_result, zwp_avi_interfaces_thermostat_setpoint_range_get_result_free
};

/** Register command handlers */
zwp_status_t zwp_avi_interfaces_thermostat_setpoint_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_thermostat_setpoint_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_thermostat_setpoint_set) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_thermostat_setpoint_supported_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_thermostat_setpoint_range_get) != ZWP_STATUS_OK)
			)
	{
		goto l_err_interface_list_get;
	}

	return ZWP_STATUS_OK;

l_err_interface_list_get:
	return ZWP_STATUS_ERROR;
}


