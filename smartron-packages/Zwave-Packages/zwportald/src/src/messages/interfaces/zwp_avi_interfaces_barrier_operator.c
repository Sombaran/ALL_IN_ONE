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
 * @file    zwp_avi_interfaces_barrier_operator.c
 *
 * @brief   AVI message 'interface_barrier_operator_xxx'
 * @details Handles the AVI message 'interface_barrier_operator_xxx'.
 *
 * @author  Angela Yan
 *
 * @version 1.0 - 2016-11-21
 * - Initial version
 */

#include "zwp_avi_interfaces_barrier_operator.h"

 /** Setpoint type not specified */
#define COMMAND_BARRIER_OPERATOR_SUBSYSTEM_TYPE_NONE         (-1)


//#pragma region Get Commands
/**
 * @brief Allocate interface level data
 * @param[out]    state_p           Interface level data
 * @param[in]     pool              Memory pool to be used for allocation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_barrier_operator_state_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_barrier_operator_state_alloc(void **state_p, zwp_pool_t pool)
{
	zwp_avi_interfaces_barrier_operator_state_t *state;

	if ((state = (zwp_avi_interfaces_barrier_operator_state_t *)ZWP_CALLOC(1, sizeof(*state))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_state_alloc;
	}

	if (zwp_thread_cond_create(&state->report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for report received event");
		goto l_err_net_report_received_cond_create;
	}

	if (zwp_thread_cond_create(&state->subsystem_supported_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for supported report received event");
		goto l_err_net_supported_report_received_cond_create;
	}

	if (zwp_thread_cond_create(&state->subsystem_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for subsystem report received event");
		goto l_err_net_subsys_report_received_cond_create;
	}

	state->subsystem_report.report_receive_subsystem_type = COMMAND_BARRIER_OPERATOR_SUBSYSTEM_TYPE_NONE;

	if (zwu_array_list_create(0, &state->subsystem_report.barrier_operator_subsystem_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	*state_p = state;

	return ZWP_STATUS_OK;

l_err_value_list_create:
	zwp_thread_cond_destroy(state->subsystem_report.report_receive_condition);
l_err_net_subsys_report_received_cond_create:
	zwp_thread_cond_destroy(state->subsystem_supported_report.report_receive_condition);
l_err_net_supported_report_received_cond_create:
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
 * @see zwp_avi_interfaces_barrier_operator_state_alloc
 */
static void zwp_avi_interfaces_barrier_operator_state_free(void *arg)
{
	zwp_avi_interfaces_barrier_operator_state_t *state;
	zwp_avi_interfaces_barrier_operator_subsystem_report_t	subsystem_report;
	int count;
	int i;

	state = (zwp_avi_interfaces_barrier_operator_state_t *)arg;
	subsystem_report = state->subsystem_report;

	//subsystem report
	zwp_interface_report_receive_done(&subsystem_report.report_receive_state, subsystem_report.report_receive_condition);

	zwp_thread_cond_destroy(subsystem_report.report_receive_condition);

	count = zwu_array_list_size(subsystem_report.barrier_operator_subsystem_list);

	for (i = 0; i < count; i++)
	{
		ZWP_FREE(zwu_array_list_get(subsystem_report.barrier_operator_subsystem_list, i));
	}

	zwu_array_list_destroy(subsystem_report.barrier_operator_subsystem_list);

	//supported report
	zwp_interface_report_receive_done(&state->subsystem_supported_report.report_receive_state, state->subsystem_supported_report.report_receive_condition);

	zwp_thread_cond_destroy(state->subsystem_supported_report.report_receive_condition);

	ZWP_FREE(state->subsystem_supported_report.supported_types);

	//report
	zwp_interface_report_receive_done(&state->report.report_receive_state, state->report.report_receive_condition);

	zwp_thread_cond_destroy(state->report.report_receive_condition);

	ZWP_FREE(state);
}

/**
 * @brief HCAPI callback for Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  state	            barrier operator state, ZW_BAR_STA_XXX. It may also be in a range from 1 to 99 (%) open.
 * @param[in]	  ts      			time stamp.  If this is zero, the callback has no data and hence other parameter
                    				values should be ignored.
 * @return        Nothing
 */
static void zwp_avi_interfaces_barrier_operator_report_handler(zwifd_p ifd, uint8_t state, time_t ts)
{
	zwp_avi_interfaces_barrier_operator_state_t *ba_state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((ba_state = (zwp_avi_interfaces_barrier_operator_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if(ts)
	{
		//state->report.utime = zwu_time_now();
		ba_state->report.utime = ts;
		ba_state->report.state = state;
	}

	zwp_interface_report_receive_done(&ba_state->report.report_receive_state, ba_state->report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

l_err_get_state:
	zwp_net_release_desc_interface(ifd);
l_err_acquire_desc:

	return;
}

/**
* @brief Update interface level state with the new Report
* @param[in]     report            Report state in interface level
* @param[in]	 subsys_type		barrier operator subsystem type, ZW_BAR_NOTIF_TYP_XXX.
* @param[in]	 subsys_sta	barrier operator subsystem state: 0=off; 0xFF=on.
* @retval        ZWP_STATUS_OK		Success
* @retval        ZWP_STATUS_ERROR	Failure
*/
static zwp_status_t zwp_avi_interfaces_barrier_operator_subsystem_report_update(zwp_avi_interfaces_barrier_operator_subsystem_report_t *subsystem_report,
	uint8_t subsys_type, uint8_t subsys_sta, zwu_time_t ts)
{
	int barrier_operator_subsystem_value_count;
	int i;
	//zwu_time_t  time_now;
	zwp_avi_interfaces_barrier_operator_subsystem_config_value_t *barrier_operator_subsystem_value;

	barrier_operator_subsystem_value_count = zwu_array_list_size(subsystem_report->barrier_operator_subsystem_list);

	for (i = 0; i < barrier_operator_subsystem_value_count; i++)
	{
		barrier_operator_subsystem_value = zwu_array_list_get(subsystem_report->barrier_operator_subsystem_list, i);

		// If type matches, update the entry instead of creating new one.
		if (barrier_operator_subsystem_value->subsystem_type == subsys_type)
		{
			break;
		}
	}

	if (i >= barrier_operator_subsystem_value_count)
	{
		//Not found
		if ((barrier_operator_subsystem_value = (zwp_avi_interfaces_barrier_operator_subsystem_config_value_t *)ZWP_CALLOC(1, sizeof(*barrier_operator_subsystem_value))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		if (zwu_array_list_add(subsystem_report->barrier_operator_subsystem_list, barrier_operator_subsystem_value) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}
	}

	//time_now = zwu_time_now();

	//report->utime = time_now;
	//barrier_operator_subsystem_value->utime = time_now;
	subsystem_report->utime = ts;
	barrier_operator_subsystem_value->utime = ts;
	barrier_operator_subsystem_value->subsystem_type = subsys_type;
	barrier_operator_subsystem_value->subsystem_state = subsys_sta;

	return ZWP_STATUS_OK;

l_err_value_list_add:
	ZWP_FREE(barrier_operator_subsystem_value);
l_err_value_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief HCAPI callback for Report command
* @param[in]	ifd	        interface
* @param[in]	subsys_type	barrier operator subsystem type, ZW_BAR_NOTIF_TYP_XXX.
* @param[in]	subsys_sta	barrier operator subsystem state: 0=off; 0xFF=on.
* @param[in]	ts          time stamp.  If this is zero, the callback has no data and hence other parameter
values should be ignored.
* @return        Nothing
*/
static void zwp_avi_interfaces_barrier_operator_subsystem_report_handler(zwifd_p ifd, uint8_t subsys_type, uint8_t subsys_sta, time_t ts)
{
	zwp_avi_interfaces_barrier_operator_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_barrier_operator_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (zwp_avi_interfaces_barrier_operator_subsystem_report_update(&state->subsystem_report, subsys_type, subsys_sta, ts) != ZWP_STATUS_OK)
	{
		goto l_err_report_update;
	}

	if (ts)
	{
		state->subsystem_report.report_receive_subsystem_type = subsys_type;
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
static zwp_status_t zwp_avi_interfaces_barrier_operator_setup(zwifd_p desc_interface, int *hcapi_code_p)
{
	*hcapi_code_p = hcaal_barrier_op_rpt_set(desc_interface, zwp_avi_interfaces_barrier_operator_report_handler);
	*hcapi_code_p = hcaal_barrier_op_subsys_rpt_set(desc_interface, zwp_avi_interfaces_barrier_operator_subsystem_report_handler);

	return ZWP_STATUS_OK;
}

/**
 * @brief Test if the expected Report command is received
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Get command
 * @param[out]    report_receive_condition_p    Report command receive condition
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_barrier_operator_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_barrier_operator_state_t *state;

	if ((state = (zwp_avi_interfaces_barrier_operator_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->report.report_receive_condition;

	if (zwp_interface_report_receive_assertion(state->report.report_receive_state) != ZWP_STATUS_OK)
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
static zwp_status_t zwp_avi_interfaces_barrier_operator_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_barrier_operator_state_t *state;

	if ((state = (zwp_avi_interfaces_barrier_operator_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->report.report_receive_state);

	*hcapi_code_p = zwif_barrier_get(desc_interface, ZWIF_GET_BMSK_LIVE | ZWIF_GET_BMSK_CACHE);

	return ZWP_STATUS_OK;

l_err_get_state:
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
 * The caller should call zwp_avi_interfaces_barrier_operator_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_barrier_operator_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_barrier_operator_state_t *state;
	zwp_avi_interfaces_barrier_operator_report_t *report;

	if ((state = (zwp_avi_interfaces_barrier_operator_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((report = (zwp_avi_interfaces_barrier_operator_report_t *)ZWP_CALLOC(1, sizeof(*report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	memcpy(report, &state->report, sizeof(*report));

	*result_p = report;

	return ZWP_STATUS_OK;

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
static zwp_status_t zwp_avi_interfaces_barrier_operator_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_barrier_operator_report_t *report;

	report = (zwp_avi_interfaces_barrier_operator_report_t *)result;

	if ((zw_avro_field_set_long(arg_value, "utime", report->utime) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "state", report->state) != ZWP_STATUS_OK))
	{
		goto l_err_set_report_value;
	}

	return ZWP_STATUS_OK;

l_err_set_report_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_barrier_operator_get_execute_passive
 */
static void zwp_avi_interfaces_barrier_operator_get_result_free(void *result)
{
	ZWP_FREE(result);
}
//#pragma endregion

//#pragma region Set command
/**
 * @brief Get argument for Set command from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Set command
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_barrier_operator_set_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_barrier_operator_set_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_barrier_operator_set_t *argument;
	int integer;

	if ((argument = (zwp_avi_interfaces_barrier_operator_set_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "target_state", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_get_value;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "target_state: %d", integer);

	if (zwu_number_is_uint8(integer) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid value: %d", integer);
		goto l_err_invalid_value;
	}

	argument->target_state = (uint8_t)integer;

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_invalid_value:
l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Set command
 * @param[in]     args              Arguments passed with Set command
 * @return        Nothing
 * @see zwp_avi_interfaces_barrier_operator_set_argument
 */
static void zwp_avi_interfaces_barrier_operator_set_argument_free(void *args)
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
static zwp_status_t zwp_avi_interfaces_barrier_operator_set_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_barrier_operator_set_t *argument;

	argument = (zwp_avi_interfaces_barrier_operator_set_t *)args;

	*hcapi_code_p = zwif_barrier_set(desc_interface, argument->target_state, NULL, NULL);

	return ZWP_STATUS_OK;
}

//#pragma endregion


//#pragma region Subsystem Config Get Command




/**
* @brief Get argument for Get command from Avro value object
* @param[in]     arg_value         Avro value object
* @param[out]    args_p            Arguments passed with Get command
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zwp_avi_interfaces_barrier_operator_subsystem_get_argument_free() to free resources
*/
static zwp_status_t zwp_avi_interfaces_barrier_operator_subsystem_get_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_barrier_operator_subsystem_config_get_t *argument;
	int integer;

	if ((argument = (zwp_avi_interfaces_barrier_operator_subsystem_config_get_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "type", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_get_value;
	}
	argument->subsystem_type = integer;

	ZWP_LOG(ZWP_LOG_DEBUG, "Subsystem Type: %d", argument->subsystem_type);

	if ((argument->subsystem_type != COMMAND_BARRIER_OPERATOR_SUBSYSTEM_TYPE_NONE)
		&& (zwu_number_is_uint8(argument->subsystem_type) != ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid subsystem type: %d", argument->subsystem_type);
		goto l_err_invalid_subsystem_type;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_invalid_subsystem_type:
l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free arguments for Get command
* @param[in]     args              Arguments passed with Get command
* @return        Nothing
* @see zwp_avi_interfaces_barrier_operator_subsystem_get_argument
*/
static void zwp_avi_interfaces_barrier_operator_subsystem_get_argument_free(void *args)
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
static zwp_status_t zwp_avi_interfaces_barrier_operator_subsystem_get_report_receive_assertion(zwifd_p desc_interface, void *args,
	zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_barrier_operator_state_t *state;
	zwp_avi_interfaces_barrier_operator_subsystem_config_get_t *argument;

	argument = (zwp_avi_interfaces_barrier_operator_subsystem_config_get_t *)args;

	if ((state = (zwp_avi_interfaces_barrier_operator_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->subsystem_report.report_receive_condition;

	if ((zwp_interface_report_receive_assertion(state->subsystem_report.report_receive_state) != ZWP_STATUS_OK)
		|| ((argument->subsystem_type != COMMAND_BARRIER_OPERATOR_SUBSYSTEM_TYPE_NONE) && (state->subsystem_report.report_receive_subsystem_type != argument->subsystem_type)))
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
static zwp_status_t zwp_avi_interfaces_barrier_operator_subsystem_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_barrier_operator_state_t *state;
	zwp_avi_interfaces_barrier_operator_subsystem_config_get_t *argument;
	uint8_t subsystem_type;

	argument = (zwp_avi_interfaces_barrier_operator_subsystem_config_get_t *)args;

	if ((state = (zwp_avi_interfaces_barrier_operator_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->subsystem_report.report_receive_state);

	if (argument->subsystem_type == COMMAND_BARRIER_OPERATOR_SUBSYSTEM_TYPE_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Missing one or more required arguments");
		goto l_err_invalid_argument;
	}

	subsystem_type = (uint8_t)argument->subsystem_type;

	*hcapi_code_p = zwif_barrier_notif_cfg_get(desc_interface, subsystem_type, ZWIF_GET_BMSK_LIVE | ZWIF_GET_BMSK_CACHE);

	return ZWP_STATUS_OK;

l_err_invalid_argument:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Allocate Result object corresponding to Report command
* @param[out]    report_dest       Destination Report
* @param[in]     report_src        Source Report
* @param[in]     subsystem_type     Setpoint type
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_interfaces_barrier_operator_subsystem_get_result_alloc(zwp_avi_interfaces_barrier_operator_subsystem_report_t *report_dest,
	zwp_avi_interfaces_barrier_operator_subsystem_report_t *report_src, int subsystem_type)
{
	int barrier_operator_subsystem_value_count;
	int i;
	zwp_avi_interfaces_barrier_operator_subsystem_config_value_t *barrier_operator_subsystem_value_dest;
	zwp_avi_interfaces_barrier_operator_subsystem_config_value_t *barrier_operator_subsystem_value_src;

	barrier_operator_subsystem_value_count = zwu_array_list_size(report_src->barrier_operator_subsystem_list);

	for (i = 0; i < barrier_operator_subsystem_value_count; i++)
	{
		barrier_operator_subsystem_value_src = zwu_array_list_get(report_src->barrier_operator_subsystem_list, i);

		// If subsystem_type doesn't match, skip the entry
		if ((subsystem_type != COMMAND_BARRIER_OPERATOR_SUBSYSTEM_TYPE_NONE) && (barrier_operator_subsystem_value_src->subsystem_type != subsystem_type))
		{
			continue;
		}

		if ((barrier_operator_subsystem_value_dest = (zwp_avi_interfaces_barrier_operator_subsystem_config_value_t *)ZWP_CALLOC(1, sizeof(*barrier_operator_subsystem_value_dest))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		if (zwu_array_list_add(report_dest->barrier_operator_subsystem_list, barrier_operator_subsystem_value_dest) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}

		memcpy(barrier_operator_subsystem_value_dest, barrier_operator_subsystem_value_src, sizeof(*barrier_operator_subsystem_value_dest));
	}

	return ZWP_STATUS_OK;

l_err_value_list_add:
	ZWP_FREE(barrier_operator_subsystem_value_dest);
l_err_value_alloc:
	for (i--; i >= 0; i--)
	{
		ZWP_FREE(zwu_array_list_get(report_dest->barrier_operator_subsystem_list, i));
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
* The caller should call zwp_avi_interfaces_barrier_operator_subsystem_get_result_free() to free resources
*/
static zwp_status_t zwp_avi_interfaces_barrier_operator_subsystem_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_barrier_operator_state_t *state;
	zwp_avi_interfaces_barrier_operator_subsystem_report_t *subsystem_report;
	zwp_avi_interfaces_barrier_operator_subsystem_config_get_t *argument;

	argument = (zwp_avi_interfaces_barrier_operator_subsystem_config_get_t *)args;

	if ((state = (zwp_avi_interfaces_barrier_operator_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((subsystem_report = (zwp_avi_interfaces_barrier_operator_subsystem_report_t *)ZWP_CALLOC(1, sizeof(*subsystem_report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	if (zwu_array_list_create(0, &subsystem_report->barrier_operator_subsystem_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	if (zwp_avi_interfaces_barrier_operator_subsystem_get_result_alloc(subsystem_report, &state->subsystem_report, argument->subsystem_type) != ZWP_STATUS_OK)
	{
		goto l_err_result_alloc;
	}

	*result_p = subsystem_report;

	return ZWP_STATUS_OK;

l_err_result_alloc:
	zwu_array_list_destroy(subsystem_report->barrier_operator_subsystem_list);
l_err_value_list_create:
	ZWP_FREE(subsystem_report);
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
static zwp_status_t zwp_avi_interfaces_barrier_operator_subsystem_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_barrier_operator_subsystem_report_t *subsystem_report;
	avro_value_t barrier_operator_subsystem_value_list_value;
	avro_value_t barrier_operator_subsystem_value_value;
	zwp_avi_interfaces_barrier_operator_subsystem_config_value_t *barrier_operator_subsystem_value;
	int barrier_operator_subsystem_value_count;
	int i;

	subsystem_report = (zwp_avi_interfaces_barrier_operator_subsystem_report_t *)result;

	if (avro_value_get_by_name(arg_value, "barrier_operator_subsystem_config_value_list", &barrier_operator_subsystem_value_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'barrier_operator_subsystem_config_value_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	barrier_operator_subsystem_value_count = zwu_array_list_size(subsystem_report->barrier_operator_subsystem_list);

	for (i = 0; i < barrier_operator_subsystem_value_count; i++)
	{
		if (avro_value_append(&barrier_operator_subsystem_value_list_value, &barrier_operator_subsystem_value_value, 0) != 0)
		{
			goto l_err_append_barrier_operator_subsystem_value_value;
		}

		barrier_operator_subsystem_value = zwu_array_list_get(subsystem_report->barrier_operator_subsystem_list, i);

		if ((zw_avro_field_set_long(&barrier_operator_subsystem_value_value, "utime", barrier_operator_subsystem_value->utime) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(&barrier_operator_subsystem_value_value, "type", barrier_operator_subsystem_value->subsystem_type) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(&barrier_operator_subsystem_value_value, "state", barrier_operator_subsystem_value->subsystem_state) != ZWP_STATUS_OK)
			)
		{
			goto l_err_set_barrier_operator_subsystem_value_value;
		}
	}

	return ZWP_STATUS_OK;

l_err_set_barrier_operator_subsystem_value_value:
l_err_append_barrier_operator_subsystem_value_value:
l_err_get_list_value:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free Result object corresponding to Report command
* @param[in]     result            Result object
* @return        Nothing
* @see zwp_avi_interfaces_barrier_operator_subsystem_get_execute_passive
*/
static void zwp_avi_interfaces_barrier_operator_subsystem_get_result_free(void *result)
{
	zwp_avi_interfaces_barrier_operator_subsystem_report_t *subsystem_report;
	int barrier_operator_subsystem_value_count;
	int i;

	subsystem_report = (zwp_avi_interfaces_barrier_operator_subsystem_report_t *)result;

	barrier_operator_subsystem_value_count = zwu_array_list_size(subsystem_report->barrier_operator_subsystem_list);

	for (i = 0; i < barrier_operator_subsystem_value_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(subsystem_report->barrier_operator_subsystem_list, i));
	}
	zwu_array_list_destroy(subsystem_report->barrier_operator_subsystem_list);

	ZWP_FREE(subsystem_report);
}
//#pragma endregion

#ifndef __GNUC__
#pragma region Subsystem Config Set Command
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
* The caller should call zwp_avi_interfaces_barrier_operator_subsystem_set_argument_free() to free resources
*/
static zwp_status_t zwp_avi_interfaces_barrier_operator_subsystem_set_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_barrier_operator_subsystem_config_set_t *argument;
	int32_t subsystem_type;
	int32_t state;

	if ((argument = (zwp_avi_interfaces_barrier_operator_subsystem_config_set_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if ((zw_avro_field_get_int(arg_value, "type", &subsystem_type) != ZWP_STATUS_OK)
		|| (zw_avro_field_get_int(arg_value, "state", &state) != ZWP_STATUS_OK)
		)
	{
		goto l_err_get_value;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "subsystem Type: %d", subsystem_type);
	ZWP_LOG(ZWP_LOG_DEBUG, "state: %d", state);

	if (zwu_number_is_uint8(subsystem_type) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid subsystem type: %d", subsystem_type);
		goto l_err_invalid_subsystem_type;
	}

	if (zwu_number_is_uint8(state) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid state: %d", state);
		goto l_err_invalid_state;
	}

	argument->subsystem_type = (uint8_t)subsystem_type;
	argument->subsystem_state = (uint8_t)state;

	*args_p = argument;

	return ZWP_STATUS_OK;


l_err_invalid_state:
l_err_invalid_subsystem_type:
l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free arguments for Set command
* @param[in]     args              Arguments passed with Set command
* @return        Nothing
* @see zwp_avi_interfaces_barrier_operator_subsystem_set_argument
*/
static void zwp_avi_interfaces_barrier_operator_subsystem_set_argument_free(void *args)
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
static zwp_status_t zwp_avi_interfaces_barrier_operator_subsystem_set_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_barrier_operator_subsystem_config_set_t *argument;

	argument = (zwp_avi_interfaces_barrier_operator_subsystem_config_set_t *)args;

	*hcapi_code_p = zwif_barrier_notif_cfg_set(desc_interface, argument->subsystem_type, argument->subsystem_state);

	return ZWP_STATUS_OK;
}
#ifndef __GNUC__
#pragma endregion
#endif

//#pragma region Subsystem Supported Get Command
/**
* @brief HCAPI callback for Supported Report command
* @param[in]	  ifd	            interface
* @param[in]	  type_len          size of type buffer
* @param[in]	  type              buffer to store supported thermostat setpoint types (ZW_THRMO_SETP_TYP_XXX)
* @return        Nothing
*/
static void zwp_avi_interfaces_barrier_operator_subsystem_supported_report_handler(zwifd_p ifd, uint8_t type_len, uint8_t *type, int valid)
{
	zwp_avi_interfaces_barrier_operator_state_t *state;
	zwifd_p desc_interface;
	uint8_t *tmp;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_barrier_operator_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (valid)
	{
		state->subsystem_supported_report.utime = zwu_time_now();

		if (type_len > state->subsystem_supported_report.supported_types_count)
		{
			if ((tmp = (uint8_t *)ZWP_REALLOC(state->subsystem_supported_report.supported_types, sizeof(*tmp) * type_len)) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				goto l_err_type_list_alloc;
			}

			state->subsystem_supported_report.supported_types = tmp;
		}
		state->subsystem_supported_report.supported_types_count = type_len;

		memcpy(state->subsystem_supported_report.supported_types, type, state->subsystem_supported_report.supported_types_count);
	}
	zwp_interface_report_receive_done(&state->subsystem_supported_report.report_receive_state, state->subsystem_supported_report.report_receive_condition);

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
static zwp_status_t zwp_avi_interfaces_barrier_operator_subsystem_supported_get_report_receive_assertion(zwifd_p desc_interface, void *args,
	zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_barrier_operator_state_t *state;

	if ((state = (zwp_avi_interfaces_barrier_operator_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->subsystem_supported_report.report_receive_condition;

	if (zwp_interface_report_receive_assertion(state->subsystem_supported_report.report_receive_state) != ZWP_STATUS_OK)
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
static zwp_status_t zwp_avi_interfaces_barrier_operator_subsystem_supported_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_barrier_operator_state_t *state;

	if ((state = (zwp_avi_interfaces_barrier_operator_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->subsystem_supported_report.report_receive_state);

	*hcapi_code_p = zwif_barrier_notif_sup_get(desc_interface, zwp_avi_interfaces_barrier_operator_subsystem_supported_report_handler, 0);

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
* The caller should call zwp_avi_interfaces_barrier_operator_subsystem_supported_get_result_free() to free resources
*/
static zwp_status_t zwp_avi_interfaces_barrier_operator_subsystem_supported_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_barrier_operator_state_t *state;
	zwp_avi_interfaces_barrier_operator_subsystem_supported_report_t *supported_report;

	if ((state = (zwp_avi_interfaces_barrier_operator_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((supported_report = (zwp_avi_interfaces_barrier_operator_subsystem_supported_report_t *)ZWP_CALLOC(1, sizeof(*supported_report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	memcpy(supported_report, &state->subsystem_supported_report, sizeof(*supported_report));

	if ((supported_report->supported_types = (uint8_t *)ZWP_CALLOC(1,
		sizeof(*supported_report->supported_types) * state->subsystem_supported_report.supported_types_count)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_type_list_alloc;
	}

	memcpy(supported_report->supported_types, state->subsystem_supported_report.supported_types, state->subsystem_supported_report.supported_types_count);

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
static zwp_status_t zwp_avi_interfaces_barrier_operator_subsystem_supported_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_barrier_operator_subsystem_supported_report_t *supported_report;
	avro_value_t subsystem_type_list_value;
	avro_value_t subsystem_type_value;
	int i;

	supported_report = (zwp_avi_interfaces_barrier_operator_subsystem_supported_report_t *)result;

	if (zw_avro_field_set_long(arg_value, "utime", supported_report->utime) != ZWP_STATUS_OK)
	{
		goto l_err_set_report_value;
	}

	if (avro_value_get_by_name(arg_value, "supported_types", &subsystem_type_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'supported_types': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	for (i = 0; i < supported_report->supported_types_count; i++)
	{
		if (avro_value_append(&subsystem_type_list_value, &subsystem_type_value, 0) != 0)
		{
			goto l_err_append_subsystem_type_value;
		}

		if (avro_value_set_int(&subsystem_type_value, supported_report->supported_types[i]) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to set setpoint type '%d': %s", supported_report->supported_types[i], avro_strerror());
			goto l_err_set_subsystem_type;
		}
	}

	return ZWP_STATUS_OK;

l_err_set_subsystem_type:
l_err_append_subsystem_type_value:
l_err_get_list_value:
l_err_set_report_value:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free Result object corresponding to Supported Report command
* @param[in]     result            Result object
* @return        Nothing
* @see zwp_avi_interfaces_barrier_operator_subsystem_supported_get_execute_passive
*/
static void zwp_avi_interfaces_barrier_operator_subsystem_supported_get_result_free(void *result)
{
	zwp_avi_interfaces_barrier_operator_subsystem_supported_report_t *supported_report;

	supported_report = (zwp_avi_interfaces_barrier_operator_subsystem_supported_report_t *)result;

	ZWP_FREE(supported_report->supported_types);

	ZWP_FREE(supported_report);
}

//#pragma endregion

/** Handlers for Get command */
static zwp_avi_message_interfaces_t interfaces_barrier_operator_get =
{
		"interface_barrier_operator_get",
		COMMAND_CLASS_BARRIER_OPERATOR, zwp_avi_interfaces_barrier_operator_setup,
		zwp_avi_interfaces_barrier_operator_state_alloc, zwp_avi_interfaces_barrier_operator_state_free,
		NULL, NULL,
		zwp_avi_interfaces_barrier_operator_get_report_receive_assertion,
		zwp_avi_interfaces_barrier_operator_get_execute_active, zwp_avi_interfaces_barrier_operator_get_execute_passive,
		zwp_avi_interfaces_barrier_operator_get_result, zwp_avi_interfaces_barrier_operator_get_result_free
};

/** Handlers for Set command */
static zwp_avi_message_interfaces_t interfaces_barrier_operator_set =
{
		"interface_barrier_operator_set",
		COMMAND_CLASS_BARRIER_OPERATOR, NULL,
		NULL, NULL,
		zwp_avi_interfaces_barrier_operator_set_argument, zwp_avi_interfaces_barrier_operator_set_argument_free,
		NULL,
		zwp_avi_interfaces_barrier_operator_set_execute_active, NULL,
		NULL, NULL
};

/** Handlers for subsystem config Get command */
static zwp_avi_message_interfaces_t interfaces_barrier_operator_subsystem_config_get =
{
	"interface_barrier_operator_subsystem_config_get",
	COMMAND_CLASS_BARRIER_OPERATOR, NULL,
	NULL, NULL,
	zwp_avi_interfaces_barrier_operator_subsystem_get_argument, zwp_avi_interfaces_barrier_operator_subsystem_get_argument_free,
	zwp_avi_interfaces_barrier_operator_subsystem_get_report_receive_assertion,
	zwp_avi_interfaces_barrier_operator_subsystem_get_execute_active, zwp_avi_interfaces_barrier_operator_subsystem_get_execute_passive,
	zwp_avi_interfaces_barrier_operator_subsystem_get_result, zwp_avi_interfaces_barrier_operator_subsystem_get_result_free
};

/** Handlers for subsystem config Set command */
static zwp_avi_message_interfaces_t interfaces_barrier_operator_subsystem_config_set =
{
	"interface_barrier_operator_subsystem_config_set",
	COMMAND_CLASS_BARRIER_OPERATOR, NULL,
	NULL, NULL,
	zwp_avi_interfaces_barrier_operator_subsystem_set_argument, zwp_avi_interfaces_barrier_operator_subsystem_set_argument_free,
	NULL,
	zwp_avi_interfaces_barrier_operator_subsystem_set_execute_active, NULL,
	NULL, NULL
};

/** Handlers for subsystem Supported Get command */
static zwp_avi_message_interfaces_t interfaces_barrier_operator_subsystem_supported_get =
{
	"interface_barrier_operator_subsystem_supported_get",
	COMMAND_CLASS_BARRIER_OPERATOR, NULL,
	NULL, NULL,
	NULL, NULL,
	zwp_avi_interfaces_barrier_operator_subsystem_supported_get_report_receive_assertion,
	zwp_avi_interfaces_barrier_operator_subsystem_supported_get_execute_active, zwp_avi_interfaces_barrier_operator_subsystem_supported_get_execute_passive,
	zwp_avi_interfaces_barrier_operator_subsystem_supported_get_result, zwp_avi_interfaces_barrier_operator_subsystem_supported_get_result_free
};

/** Register command handlers */
zwp_status_t zwp_avi_interfaces_barrier_operator_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_barrier_operator_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_barrier_operator_set) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_barrier_operator_subsystem_config_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_barrier_operator_subsystem_config_set) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_barrier_operator_subsystem_supported_get) != ZWP_STATUS_OK))
	{
		goto l_err_interface_list_get;
	}

	return ZWP_STATUS_OK;

l_err_interface_list_get:
	return ZWP_STATUS_ERROR;
}
