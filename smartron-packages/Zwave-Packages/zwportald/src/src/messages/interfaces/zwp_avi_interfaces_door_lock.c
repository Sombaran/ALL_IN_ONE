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
 * @file    zwp_avi_interfaces_door_lock.c
 *
 * @brief   AVI message 'interface_door_lock_xxx'
 * @details Handles the AVI message 'interface_door_lock_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-09-17
 * - Initial version
 */

#include "zwp_avi_interfaces_door_lock.h"

/**
 * @brief Allocate interface level data
 * @param[out]    state_p           Interface level data
 * @param[in]     pool              Memory pool to be used for allocation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_door_lock_state_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_door_lock_state_alloc(void **state_p, zwp_pool_t pool)
{
	zwp_avi_interfaces_door_lock_state_t *state;

	if ((state = (zwp_avi_interfaces_door_lock_state_t *)ZWP_CALLOC(1, sizeof(*state))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_state_alloc;
	}

	if (zwp_thread_cond_create(&state->operation_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for report received event");
		goto l_err_net_operation_report_received_cond_create;
	}

	if (zwp_thread_cond_create(&state->configuration_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for report received event");
		goto l_err_net_configuration_report_received_cond_create;
	}

	*state_p = state;

	return ZWP_STATUS_OK;

	zwp_thread_cond_destroy(state->configuration_report.report_receive_condition);
l_err_net_configuration_report_received_cond_create:
	zwp_thread_cond_destroy(state->operation_report.report_receive_condition);
l_err_net_operation_report_received_cond_create:
	ZWP_FREE(state);
l_err_state_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free interface level data
 * @param[in]     arg               Interface level data
 * @return        Nothing
 * @see zwp_avi_interfaces_door_lock_state_alloc
 */
static void zwp_avi_interfaces_door_lock_state_free(void *arg)
{
	zwp_avi_interfaces_door_lock_state_t *state;

	state = (zwp_avi_interfaces_door_lock_state_t *)arg;

	zwp_interface_report_receive_done(&state->configuration_report.report_receive_state, state->configuration_report.report_receive_condition);

	zwp_thread_cond_destroy(state->configuration_report.report_receive_condition);

	zwp_interface_report_receive_done(&state->operation_report.report_receive_state, state->operation_report.report_receive_condition);

	zwp_thread_cond_destroy(state->operation_report.report_receive_condition);

	ZWP_FREE(state);
}

/**
 * @brief HCAPI callback for Operation Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  operation	        door lock operation
 * @param[in]	  ts      			time stamp
 * @return        Nothing
 */
static void zwp_avi_interfaces_door_lock_operation_report_handler(zwifd_p ifd, zwdlck_op_p operation, time_t ts)
{
	zwp_avi_interfaces_door_lock_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_door_lock_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if(ts)
	{
		//state->operation_report.utime = zwu_time_now();
		state->operation_report.utime = ts;
		memcpy(&state->operation_report.operation, operation, sizeof(state->operation_report.operation));
	}

	zwp_interface_report_receive_done(&state->operation_report.report_receive_state, state->operation_report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

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
static zwp_status_t zwp_avi_interfaces_door_lock_setup(zwifd_p desc_interface, int *hcapi_code_p)
{
	*hcapi_code_p = hcaal_dlck_op_rpt_set(desc_interface, zwp_avi_interfaces_door_lock_operation_report_handler);

	return ZWP_STATUS_OK;
}

/**
 * @brief Test if the expected Operation Report command is received
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Operation Get command
 * @param[out]    report_receive_condition_p    Report command receive condition
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_door_lock_operation_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_door_lock_state_t *state;

	if ((state = (zwp_avi_interfaces_door_lock_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->operation_report.report_receive_condition;

	if (zwp_interface_report_receive_assertion(state->operation_report.report_receive_state) != ZWP_STATUS_OK)
	{
		goto l_err_report_not_received;
	}

	return ZWP_STATUS_OK;

l_err_report_not_received:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute active Operation Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Operation Get command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_door_lock_operation_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_door_lock_state_t *state;

	if ((state = (zwp_avi_interfaces_door_lock_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->operation_report.report_receive_state);

	*hcapi_code_p = hcaal_dlck_op_get(desc_interface, ZWIF_GET_BMSK_LIVE | ZWIF_GET_BMSK_CACHE);

	return ZWP_STATUS_OK;

l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute passive Operaiton Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Operation Get command
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_door_lock_operation_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_door_lock_operation_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_door_lock_state_t *state;
	zwp_avi_interfaces_door_lock_operation_report_t *report;

	if ((state = (zwp_avi_interfaces_door_lock_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((report = (zwp_avi_interfaces_door_lock_operation_report_t *)ZWP_CALLOC(1, sizeof(*report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	memcpy(report, &state->operation_report, sizeof(*report));

	*result_p = report;

	return ZWP_STATUS_OK;

l_err_report_alloc:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to Operation Report command
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_door_lock_operation_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_door_lock_operation_report_t *report;

	report = (zwp_avi_interfaces_door_lock_operation_report_t *)result;

	if ((zw_avro_field_set_long(arg_value, "utime", report->utime) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "mode", report->operation.mode) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "outside_door_handles_mode", report->operation.out_mode) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "inside_door_handles_mode", report->operation.in_mode) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "door_condition", report->operation.cond) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "lock_timeout_minutes", report->operation.tmout_min) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "lock_timeout_seconds", report->operation.tmout_sec) != ZWP_STATUS_OK))
	{
		goto l_err_set_report_value;
	}

	return ZWP_STATUS_OK;

l_err_set_report_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Operation Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_door_lock_operation_get_execute_passive
 */
static void zwp_avi_interfaces_door_lock_operation_get_result_free(void *result)
{
	ZWP_FREE(result);
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
 * The caller should call zwp_avi_interfaces_door_lock_operation_set_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_door_lock_operation_set_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_door_lock_operation_set_t *argument;
	int integer;

	if ((argument = (zwp_avi_interfaces_door_lock_operation_set_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "mode", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_get_value;
	}

	if (zwu_number_is_uint8(integer) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Mode: %d", integer);
		goto l_err_invalid_mode;
	}

	argument->mode = (uint8_t)integer;

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_invalid_mode:
l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Operation Set command
 * @param[in]     args              Arguments passed with Operation Set command
 * @return        Nothing
 * @see zwp_avi_interfaces_door_lock_operation_set_argument
 */
static void zwp_avi_interfaces_door_lock_operation_set_argument_free(void *args)
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
static zwp_status_t zwp_avi_interfaces_door_lock_operation_set_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_door_lock_operation_set_t *argument;

	argument = (zwp_avi_interfaces_door_lock_operation_set_t *)args;

	*hcapi_code_p = zwif_dlck_op_set(desc_interface, argument->mode, NULL, NULL);

	return ZWP_STATUS_OK;
}

/**
 * @brief HCAPI callback for Configuration Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  configuration	    door lock configuration
 * @return        Nothing
 */
static void zwp_avi_interfaces_door_lock_configuration_report_handler(zwifd_p ifd, zwdlck_cfg_p configuration, time_t ts)
{
	zwp_avi_interfaces_door_lock_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_door_lock_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if(ts)
	{
		//state->configuration_report.utime = zwu_time_now();
		state->configuration_report.utime = ts;
		memcpy(&state->configuration_report.configuration, configuration, sizeof(state->configuration_report.configuration));
	}

	zwp_interface_report_receive_done(&state->configuration_report.report_receive_state, state->configuration_report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

l_err_get_state:
	zwp_net_release_desc_interface(ifd);
l_err_acquire_desc:

	return;
}

/**
 * @brief Test if the expected Configuration Report command is received
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Configuration Get command
 * @param[out]    report_receive_condition_p    Report command receive condition
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_door_lock_configuration_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_door_lock_state_t *state;

	if ((state = (zwp_avi_interfaces_door_lock_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->configuration_report.report_receive_condition;

	if (zwp_interface_report_receive_assertion(state->configuration_report.report_receive_state) != ZWP_STATUS_OK)
	{
		goto l_err_report_not_received;
	}

	return ZWP_STATUS_OK;

l_err_report_not_received:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute active Configuration Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Configuration Get command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_door_lock_configuration_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_door_lock_state_t *state;

	if ((state = (zwp_avi_interfaces_door_lock_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->configuration_report.report_receive_state);

	*hcapi_code_p = zwif_dlck_cfg_get(desc_interface, zwp_avi_interfaces_door_lock_configuration_report_handler, ZWIF_GET_BMSK_LIVE | ZWIF_GET_BMSK_CACHE);

	return ZWP_STATUS_OK;

l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute passive Configuration Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Configuration Get command
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_door_lock_configuration_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_door_lock_configuration_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_door_lock_state_t *state;
	zwp_avi_interfaces_door_lock_configuration_report_t *configuration_report;

	if ((state = (zwp_avi_interfaces_door_lock_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((configuration_report = (zwp_avi_interfaces_door_lock_configuration_report_t *)ZWP_CALLOC(1, sizeof(*configuration_report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	memcpy(configuration_report, &state->configuration_report, sizeof(*configuration_report));

	*result_p = configuration_report;

	return ZWP_STATUS_OK;

l_err_report_alloc:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to Configuration Report command
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_door_lock_configuration_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_door_lock_configuration_report_t *report;

	report = (zwp_avi_interfaces_door_lock_configuration_report_t *)result;

	if ((zw_avro_field_set_long(arg_value, "utime", report->utime) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "operation_type", report->configuration.type) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "outside_door_handles_state", report->configuration.out_sta) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "inside_door_handles_state", report->configuration.in_sta) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "lock_timeout_minutes", report->configuration.tmout_min) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "lock_timeout_seconds", report->configuration.tmout_sec) != ZWP_STATUS_OK))
	{
		goto l_err_set_report_value;
	}

	return ZWP_STATUS_OK;

l_err_set_report_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Configuration Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_door_lock_configuration_get_execute_passive
 */
static void zwp_avi_interfaces_door_lock_configuration_get_result_free(void *result)
{
	ZWP_FREE(result);
}

/**
 * @brief Get argument for Configuration Set command from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Configuration Set command
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_door_lock_configuration_set_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_door_lock_configuration_set_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_door_lock_configuration_set_t *argument;

	if ((argument = (zwp_avi_interfaces_door_lock_configuration_set_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if ((zw_avro_field_get_int(arg_value, "operation_type", &argument->operation_type) != ZWP_STATUS_OK)
		|| (zw_avro_field_get_int(arg_value, "outside_door_handles_state", &argument->outside_door_handles_state) != ZWP_STATUS_OK)
		|| (zw_avro_field_get_int(arg_value, "inside_door_handles_state", &argument->inside_door_handles_state) != ZWP_STATUS_OK)
		|| (zw_avro_field_get_int(arg_value, "lock_timeout_minutes", &argument->lock_timeout_minutes) != ZWP_STATUS_OK)
		|| (zw_avro_field_get_int(arg_value, "lock_timeout_seconds", &argument->lock_timeout_seconds) != ZWP_STATUS_OK))
	{
		goto l_err_get_value;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Operation Type: %d", argument->operation_type);
	ZWP_LOG(ZWP_LOG_DEBUG, "Outside Door Handles State: %d", argument->outside_door_handles_state);
	ZWP_LOG(ZWP_LOG_DEBUG, "Inside Door Handles State: %d", argument->inside_door_handles_state);
	ZWP_LOG(ZWP_LOG_DEBUG, "Lock Timeout Minutes: %d", argument->lock_timeout_minutes);
	ZWP_LOG(ZWP_LOG_DEBUG, "Lock Timeout Seconds: %d", argument->lock_timeout_seconds);

	if (zwu_number_is_uint8(argument->operation_type) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Operation Type: %d", argument->operation_type);
		goto l_err_invalid_operation_type;
	}

	if (zwu_number_is_uint8(argument->outside_door_handles_state) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Outside Door Handles State: %d", argument->outside_door_handles_state);
		goto l_err_invalid_outside_door_handles_state;
	}

	if (zwu_number_is_uint8(argument->inside_door_handles_state) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Inside Door Handles State: %d", argument->inside_door_handles_state);
		goto l_err_invalid_inside_door_handles_state;
	}

	if (zwu_number_is_uint8(argument->lock_timeout_minutes) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Lock Timeout Minutes: %d", argument->lock_timeout_minutes);
		goto l_err_invalid_lock_timeout_minutes;
	}

	if (zwu_number_is_uint8(argument->lock_timeout_seconds) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Lock Timeout Seconds: %d", argument->lock_timeout_seconds);
		goto l_err_invalid_lock_timeout_seconds;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_invalid_lock_timeout_seconds:
l_err_invalid_lock_timeout_minutes:
l_err_invalid_inside_door_handles_state:
l_err_invalid_outside_door_handles_state:
l_err_invalid_operation_type:
l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Configuration Set command
 * @param[in]     args              Arguments passed with Configuration Set command
 * @return        Nothing
 * @see zwp_avi_interfaces_door_lock_configuration_set_argument
 */
static void zwp_avi_interfaces_door_lock_configuration_set_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
 * @brief Execute Configuration Set command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments passed with Configuration Set command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_door_lock_configuration_set_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_door_lock_configuration_set_t *argument;
	zwdlck_cfg_t configuration;

	argument = (zwp_avi_interfaces_door_lock_configuration_set_t *)args;

	configuration.type = (uint8_t)argument->operation_type;
	configuration.out_sta = (uint8_t)argument->outside_door_handles_state;
	configuration.in_sta = (uint8_t)argument->inside_door_handles_state;
	configuration.tmout_min = (uint8_t)argument->lock_timeout_minutes;
	configuration.tmout_sec = (uint8_t)argument->lock_timeout_seconds;

	*hcapi_code_p = zwif_dlck_cfg_set(desc_interface, &configuration);

	return ZWP_STATUS_OK;
}

/** Handlers for Operation Get command */
static zwp_avi_message_interfaces_t interfaces_door_lock_operation_get =
{
		"interface_door_lock_operation_get",
		COMMAND_CLASS_DOOR_LOCK, zwp_avi_interfaces_door_lock_setup,
		zwp_avi_interfaces_door_lock_state_alloc, zwp_avi_interfaces_door_lock_state_free,
		NULL, NULL,
		zwp_avi_interfaces_door_lock_operation_get_report_receive_assertion,
		zwp_avi_interfaces_door_lock_operation_get_execute_active, zwp_avi_interfaces_door_lock_operation_get_execute_passive,
		zwp_avi_interfaces_door_lock_operation_get_result, zwp_avi_interfaces_door_lock_operation_get_result_free
};

/** Handlers for Operation Set command */
static zwp_avi_message_interfaces_t interfaces_door_lock_operation_set =
{
		"interface_door_lock_operation_set",
		COMMAND_CLASS_DOOR_LOCK, NULL,
		NULL, NULL,
		zwp_avi_interfaces_door_lock_operation_set_argument, zwp_avi_interfaces_door_lock_operation_set_argument_free,
		NULL,
		zwp_avi_interfaces_door_lock_operation_set_execute_active, NULL,
		NULL, NULL
};

/** Handlers for Configuration Get command */
static zwp_avi_message_interfaces_t interfaces_door_lock_configuration_get =
{
		"interface_door_lock_configuration_get",
		COMMAND_CLASS_DOOR_LOCK, NULL,
		NULL, NULL,
		NULL, NULL,
		zwp_avi_interfaces_door_lock_configuration_get_report_receive_assertion,
		zwp_avi_interfaces_door_lock_configuration_get_execute_active, zwp_avi_interfaces_door_lock_configuration_get_execute_passive,
		zwp_avi_interfaces_door_lock_configuration_get_result, zwp_avi_interfaces_door_lock_configuration_get_result_free
};

/** Handlers for Configuration Set command */
static zwp_avi_message_interfaces_t interfaces_door_lock_configuration_set =
{
		"interface_door_lock_configuration_set",
		COMMAND_CLASS_DOOR_LOCK, NULL,
		NULL, NULL,
		zwp_avi_interfaces_door_lock_configuration_set_argument, zwp_avi_interfaces_door_lock_configuration_set_argument_free,
		NULL,
		zwp_avi_interfaces_door_lock_configuration_set_execute_active, NULL,
		NULL, NULL
};

/** Register command handlers */
zwp_status_t zwp_avi_interfaces_door_lock_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_door_lock_operation_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_door_lock_operation_set) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_door_lock_configuration_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_door_lock_configuration_set) != ZWP_STATUS_OK))
	{
		goto l_err_interface_list_get;
	}

	return ZWP_STATUS_OK;

l_err_interface_list_get:
	return ZWP_STATUS_ERROR;
}
