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
 * @file    zwp_avi_interfaces_sensor_multilevel.c
 *
 * @brief   AVI message 'interface_sensor_multilevel_xxx'
 * @details Handles the AVI message 'interface_sensor_multilevel_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-09-18
 * - Initial version
 */

#include "zwp_avi_interfaces_sensor_multilevel.h"

/**
 * @brief Allocate interface level data
 * @param[out]    state_p           Interface level data
 * @param[in]     pool              Memory pool to be used for allocation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_sensor_multilevel_state_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_sensor_multilevel_state_alloc(void **state_p, zwp_pool_t pool)
{
	zwp_avi_interfaces_sensor_multilevel_state_t *state;

	if ((state = (zwp_avi_interfaces_sensor_multilevel_state_t *)ZWP_CALLOC(1, sizeof(*state))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_state_alloc;
	}

	if (zwp_thread_cond_create(&state->report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for report received event");
		goto l_err_net_report_received_cond_create;
	}

	if (zwu_array_list_create(0, &state->report.sensor_value_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	if (zwp_thread_cond_create(&state->supported_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for supported_report received event");
		goto l_err_net_supported_report_received_cond_create;
	}

	state->supported_report.supported_types = NULL;
	state->supported_report.supported_types_count = 0;

	//Scale supported
	if (zwp_thread_cond_create(&state->type_scale_supported_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for type_scale_supported_report received scale");
		goto l_err_net_scale_supported_report_received_cond_create;
	}

	if (zwu_array_list_create(0, &state->type_scale_supported_report.scale_supported_list) != ZWP_STATUS_OK)
	{
		goto l_err_scale_list_create;
	}

	*state_p = state;

	return ZWP_STATUS_OK;
	zwu_array_list_destroy(state->type_scale_supported_report.scale_supported_list);
l_err_scale_list_create:
	zwp_thread_cond_destroy(state->type_scale_supported_report.report_receive_condition);
l_err_net_scale_supported_report_received_cond_create:
	zwp_thread_cond_destroy(state->supported_report.report_receive_condition);
l_err_net_supported_report_received_cond_create:
	zwu_array_list_destroy(state->report.sensor_value_list);
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
 * @see zwp_avi_interfaces_sensor_multilevel_state_alloc
 */
static void zwp_avi_interfaces_sensor_multilevel_state_free(void *arg)
{
	zwp_avi_interfaces_sensor_multilevel_state_t *state;
	zwp_avi_interfaces_sensor_multilevel_report_t *report;
	zwp_avi_interfaces_sensor_multilevel_single_scale_supported_report_t *pscales_per_type;
	int sensor_value_count;
	int i;

	state = (zwp_avi_interfaces_sensor_multilevel_state_t *)arg;
	report = &state->report;

	//supported_report
	zwp_interface_report_receive_done(&state->supported_report.report_receive_state, state->supported_report.report_receive_condition);

	zwp_thread_cond_destroy(state->supported_report.report_receive_condition);

	ZWP_FREE(state->supported_report.supported_types);

	//report
	sensor_value_count = zwu_array_list_size(report->sensor_value_list);

	for (i = 0; i < sensor_value_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(report->sensor_value_list, i));
	}

	zwu_array_list_destroy(report->sensor_value_list);

	zwp_interface_report_receive_done(&state->report.report_receive_state, state->report.report_receive_condition);

	zwp_thread_cond_destroy(state->report.report_receive_condition);

	//type_scale_supported_report
	sensor_value_count = zwu_array_list_size(state->type_scale_supported_report.scale_supported_list);

	for (i = 0; i < sensor_value_count; i++)
	{
		pscales_per_type = (zwp_avi_interfaces_sensor_multilevel_single_scale_supported_report_t *)zwu_array_list_get(state->type_scale_supported_report.scale_supported_list, i);

		if (pscales_per_type)
		{
			ZWP_FREE(pscales_per_type->supported_scales);
			ZWP_FREE(pscales_per_type);
		}
	}

	zwu_array_list_destroy(state->type_scale_supported_report.scale_supported_list);

	zwp_interface_report_receive_done(&state->type_scale_supported_report.report_receive_state, state->type_scale_supported_report.report_receive_condition);

	zwp_thread_cond_destroy(state->type_scale_supported_report.report_receive_condition);

#ifndef LOCAL_CLIENT_ENABLE
	zwp_avi_interfaces_msensor_state_free_ifttt(state);
#endif

	ZWP_FREE(state);
}

/**
 * @brief Update interface level state with the new Report
 * @param[in]     report            Report state in interface level
 * @param[in]     data              Value from new Report
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_sensor_multilevel_report_update(zwp_avi_interfaces_sensor_multilevel_report_t *report,
		zwsensor_p data, zwu_time_t ts)
{
	int sensor_value_count;
	int i;
	//zwu_time_t  time_now;
	zwp_avi_interfaces_sensor_multilevel_report_sensor_value_t *sensor_value;

	sensor_value_count = zwu_array_list_size(report->sensor_value_list);

	for (i = 0; i < sensor_value_count; i++)
	{
		sensor_value = zwu_array_list_get(report->sensor_value_list, i);

		/*
		 * Use type-unit combo after the following fix is done.
		 * Production UI: If reports differ only by scale and not type, the UI shows multiple listings
		 * as expected. But with Z-Ware 1, it shows only the recent listing.
		 */
		// If both type and scale matches, update the entry instead of creating new one.
		// if ((sensor_value->value.type == data->type) && (sensor_value->value.unit == data->unit))

		// If type matches, update the entry instead of creating new one.
		//if (sensor_value->value.type == data->type)
		if ((sensor_value->value.type == data->type) && (sensor_value->value.unit == data->unit))
		{
			break;
		}
	}

	if (i >= sensor_value_count)
	{
		if ((sensor_value = (zwp_avi_interfaces_sensor_multilevel_report_sensor_value_t *)ZWP_CALLOC(1, sizeof(*sensor_value))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		if (zwu_array_list_add(report->sensor_value_list, sensor_value) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}
	}

	//time_now = zwu_time_now();

	//report->utime = time_now;
	report->utime = ts;
	//sensor_value->utime = time_now;
	sensor_value->utime = ts;
	memcpy(&sensor_value->value, data, sizeof(sensor_value->value));

	/*
	 * To avoid usage of uninitialized portion in Avro message
	 */
	memset(sensor_value->value.data, 0, sizeof(sensor_value->value.data));
	memcpy(sensor_value->value.data, data->data, data->size);

	return ZWP_STATUS_OK;

l_err_value_list_add:
	ZWP_FREE(sensor_value);
l_err_value_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief HCAPI callback for Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  data	            current value and unit of the sensor
 * @param[in]	  ts      			time stamp
 * @return        Nothing
 */
void zwp_avi_interfaces_sensor_multilevel_report_handler(zwifd_p ifd, zwsensor_p data, time_t ts)
{
	zwp_avi_interfaces_sensor_multilevel_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_sensor_multilevel_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if(ts)
	{
		//Must call this before updating the cached entries
		ZWP_LOG(ZWP_LOG_DEBUG, "Type = %d, precision = %d, unit = %d, size = %d, data = %d %d %d %d",
			data->type, data->precision, data->unit, data->size, data->data[0], data->data[1], data->data[2], data->data[3]);
#ifndef LOCAL_CLIENT_ENABLE
		zwp_avi_interfaces_msensor_report_handler_ifttt(desc_interface, data, ts);
#endif

		if (zwp_avi_interfaces_sensor_multilevel_report_update(&state->report, data, ts) != ZWP_STATUS_OK)
		{
			goto l_err_report_update;
		}
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
static zwp_status_t zwp_avi_interfaces_sensor_multilevel_setup(zwifd_p desc_interface, int *hcapi_code_p)
{
	*hcapi_code_p = hcaal_sensor_rpt_set(desc_interface, zwp_avi_interfaces_sensor_multilevel_report_handler);

#ifndef LOCAL_CLIENT_ENABLE
	if (zwp_avi_interfaces_sensor_multilevel_state_alloc_ifttt(desc_interface) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to alloc IFTTT interface level data.");
		goto l_err_ifttt_alloc;
	}
#endif

	return ZWP_STATUS_OK;

#ifndef LOCAL_CLIENT_ENABLE
	l_err_ifttt_alloc :
#endif
	return ZWP_STATUS_ERROR;
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
* The caller should call zwp_avi_interfaces_sensor_multilevel_get_argument_free() to free resources
*/
static zwp_status_t zwp_avi_interfaces_sensor_multilevel_get_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_sensor_multilevel_get_t *argument;
	int integer;

	if ((argument = (zwp_avi_interfaces_sensor_multilevel_get_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "sensor_type", &integer) != ZWP_STATUS_OK)
	{
		//optional parameter
		argument->type = COMMAND_SENSOR_TYPE_NONE;
	}
	else
	{
		argument->type = integer;
	}

	if (zw_avro_field_get_int(arg_value, "scale", &integer) != ZWP_STATUS_OK)
	{
		//optional parameter
		argument->scale = COMMAND_SENSOR_SCALE_NONE;
	}
	else
	{
		argument->scale = integer;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Sensor Type: %d", argument->type);
	ZWP_LOG(ZWP_LOG_DEBUG, "Sensor scale: %d", argument->scale);

	if ((argument->type != COMMAND_SENSOR_TYPE_NONE)
		&& (zwu_number_is_uint8(argument->type) != ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid sensor type: %d", argument->type);
		goto l_err_invalid_sensor_type;
	}

	if ((argument->scale != COMMAND_SENSOR_SCALE_NONE)
		&& (zwu_number_is_uint8(argument->scale) != ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid sensor scale: %d", argument->scale);
		goto l_err_invalid_sensor_scale;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;


l_err_invalid_sensor_type :
l_err_invalid_sensor_scale :
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free arguments for Get command
* @param[in]     args              Arguments passed with Get command
* @return        Nothing
* @see zwp_avi_interfaces_sensor_multilevel_get_argument
*/
static void zwp_avi_interfaces_sensor_multilevel_get_argument_free(void *args)
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
static zwp_status_t zwp_avi_interfaces_sensor_multilevel_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_sensor_multilevel_state_t *state;

	if ((state = (zwp_avi_interfaces_sensor_multilevel_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
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
static zwp_status_t zwp_avi_interfaces_sensor_multilevel_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_sensor_multilevel_state_t *state;
	zwp_avi_interfaces_sensor_multilevel_get_t *argument;
	uint8_t type, unit;

	argument = (zwp_avi_interfaces_sensor_multilevel_get_t *)args;

	if ((state = (zwp_avi_interfaces_sensor_multilevel_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->report.report_receive_state);

	if (argument->type == COMMAND_SENSOR_TYPE_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "No sensor type provided");
		type = COMMAND_HCAPI_SENSOR_TYPE_NOT_USED;
	}
	else
	{
		type = (uint8_t)argument->type;
	}

	if (argument->scale == COMMAND_SENSOR_SCALE_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "No sensor scale provided");
		unit = COMMAND_HCAPI_SENSOR_SCALE_NOT_USED;
	}
	else
	{
		unit = (uint8_t)argument->scale;
	}

	ZWP_LOG(ZWP_LOG_ERR, "Issuing Get command with type %d and scale %d to HCAPI", type, unit);

	*hcapi_code_p = hcaal_sensor_get(desc_interface, type, unit, ZWIF_GET_BMSK_LIVE | ZWIF_GET_BMSK_CACHE);

	return ZWP_STATUS_OK;

l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Allocate Result object corresponding to Report command
 * @param[out]    report_dest       Destination Report
 * @param[in]     report_src        Source Report
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_sensor_multilevel_get_result_alloc(zwp_avi_interfaces_sensor_multilevel_report_t *report_dest,
		zwp_avi_interfaces_sensor_multilevel_report_t *report_src, int sensor_type, int scale)
{
	int sensor_value_count;
	int i;
	zwp_avi_interfaces_sensor_multilevel_report_sensor_value_t *sensor_value_src;
	zwp_avi_interfaces_sensor_multilevel_report_sensor_value_t *sensor_value_dest;

	//If UI sends sensor_type 0, treat it as COMMAND_SENSOR_TYPE_NONE (returning all cached reports)
	if (sensor_type == 0) sensor_type = COMMAND_SENSOR_TYPE_NONE;

	sensor_value_count = zwu_array_list_size(report_src->sensor_value_list);

	for (i = 0; i < sensor_value_count; i++)
	{
		//Do both type and unit matching
		//If type is COMMAND_SENSOR_TYPE_NONE, return all the cached records for backward compatiblity
		//as we have v1 multilevel device can return 3 reports of different types when responding to multilevel sensor get
		sensor_value_src = zwu_array_list_get(report_src->sensor_value_list, i);

		if (sensor_type != COMMAND_SENSOR_TYPE_NONE && 
			(sensor_value_src->value.type != sensor_type || sensor_value_src->value.unit != scale))
			continue;

		if ((sensor_value_dest = (zwp_avi_interfaces_sensor_multilevel_report_sensor_value_t *)ZWP_CALLOC(1, sizeof(*sensor_value_dest))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		if (zwu_array_list_add(report_dest->sensor_value_list, sensor_value_dest) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}

		memcpy(sensor_value_dest, sensor_value_src, sizeof(*sensor_value_dest));
	}

	return ZWP_STATUS_OK;

l_err_value_list_add:
	ZWP_FREE(sensor_value_dest);
l_err_value_alloc:
	for (i--; i >= 0; i--)
	{
		ZWP_FREE(zwu_array_list_get(report_dest->sensor_value_list, i));
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
 * The caller should call zwp_avi_interfaces_sensor_multilevel_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_sensor_multilevel_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_sensor_multilevel_state_t *state;
	zwp_avi_interfaces_sensor_multilevel_report_t *report;
	zwp_avi_interfaces_sensor_multilevel_get_t *argument;

	argument = (zwp_avi_interfaces_sensor_multilevel_get_t *)args;

	if ((state = (zwp_avi_interfaces_sensor_multilevel_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((report = (zwp_avi_interfaces_sensor_multilevel_report_t *)ZWP_CALLOC(1, sizeof(*report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	if (zwu_array_list_create(0, &report->sensor_value_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	if (zwp_avi_interfaces_sensor_multilevel_get_result_alloc(report, &state->report, argument->type, argument->scale) != ZWP_STATUS_OK)
	{
		goto l_err_result_alloc;
	}

	*result_p = report;

	return ZWP_STATUS_OK;

l_err_result_alloc:
	zwu_array_list_destroy(report->sensor_value_list);
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
static zwp_status_t zwp_avi_interfaces_sensor_multilevel_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_sensor_multilevel_report_t *report;
	avro_value_t sensor_value_list_value;
	avro_value_t sensor_value_value;
	zwp_avi_interfaces_sensor_multilevel_report_sensor_value_t *sensor_value;
	int sensor_value_count;
	int i;

	report = (zwp_avi_interfaces_sensor_multilevel_report_t *)result;

	if (avro_value_get_by_name(arg_value, "sensor_value_list", &sensor_value_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'sensor_value_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	sensor_value_count = zwu_array_list_size(report->sensor_value_list);

	for (i = 0; i < sensor_value_count; i++)
	{
		if (avro_value_append(&sensor_value_list_value, &sensor_value_value, 0) != 0)
		{
			goto l_err_append_sensor_value_value;
		}

		sensor_value = zwu_array_list_get(report->sensor_value_list, i);

		if ((zw_avro_field_set_long(&sensor_value_value, "utime", sensor_value->utime) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&sensor_value_value, "sensor_type", sensor_value->value.type) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&sensor_value_value, "precision", sensor_value->value.precision) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&sensor_value_value, "scale", sensor_value->value.unit) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&sensor_value_value, "size", sensor_value->value.size) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_fixed(&sensor_value_value, "sensor_value", sensor_value->value.data, 4) != ZWP_STATUS_OK))
		{
			goto l_err_set_sensor_value_value;
		}
	}

	return ZWP_STATUS_OK;

l_err_set_sensor_value_value:
l_err_append_sensor_value_value:
l_err_get_list_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_sensor_multilevel_get_execute_passive
 */
static void zwp_avi_interfaces_sensor_multilevel_get_result_free(void *result)
{
	zwp_avi_interfaces_sensor_multilevel_report_t *report;
	int sensor_value_count;
	int i;

	report = (zwp_avi_interfaces_sensor_multilevel_report_t *)result;

	sensor_value_count = zwu_array_list_size(report->sensor_value_list);

	for (i = 0; i < sensor_value_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(report->sensor_value_list, i));
	}
	zwu_array_list_destroy(report->sensor_value_list);

	ZWP_FREE(report);
}

/**
 * @brief Test if the expected Supported Report command is received
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Supported Get command
 * @param[out]    report_receive_condition_p    Report command receive condition
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_sensor_multilevel_supported_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_sensor_multilevel_state_t *state;

	if ((state = (zwp_avi_interfaces_sensor_multilevel_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
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
 * @brief HCAPI callback for Supported Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  type_len          size of sensor type buffer
 * @param[in]	  type              buffer to store supported sensor types (ZW_SENSOR_TYPE_XXX)
 * @return        Nothing
 */
void zwp_avi_interfaces_sensor_multilevel_supported_report_handler(zwifd_p ifd, uint8_t type_len, uint8_t *type, int valid)
{
	zwp_avi_interfaces_sensor_multilevel_state_t *state;
	zwifd_p desc_interface;
	uint8_t *tmp;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_sensor_multilevel_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
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
 * @brief Execute active Supported Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Supported Get command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_sensor_multilevel_supported_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_sensor_multilevel_state_t *state;

	if ((state = (zwp_avi_interfaces_sensor_multilevel_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->supported_report.report_receive_state);

	*hcapi_code_p = zwif_sensor_sup_get(desc_interface, zwp_avi_interfaces_sensor_multilevel_supported_report_handler, 0);

	if(*hcapi_code_p  == ZW_ERR_CMD_VERSION)
	{
		//This is special case to handle supported get failure if database is not updated
		state->supported_report.utime = zwu_time_now();
		state->supported_report.supported_types_count = 0;

		//state->supported_report.report_receive_state = ZWP_INTERFACE_REPORT_RECEIVE_DONE;
		zwp_interface_report_receive_done(&state->supported_report.report_receive_state, state->supported_report.report_receive_condition);

		*hcapi_code_p = ZW_ERR_QUEUED;
		ZWP_LOG(ZWP_LOG_ERR, "Changed retun code to hcapi_code_p = %d", *hcapi_code_p);
	}

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
 * The caller should call zwp_avi_interfaces_sensor_multilevel_supported_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_sensor_multilevel_supported_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_sensor_multilevel_state_t *state;
	zwp_avi_interfaces_sensor_multilevel_supported_report_t *supported_report;

	if ((state = (zwp_avi_interfaces_sensor_multilevel_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((supported_report = (zwp_avi_interfaces_sensor_multilevel_supported_report_t *)ZWP_CALLOC(1, sizeof(*supported_report))) == NULL)
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
static zwp_status_t zwp_avi_interfaces_sensor_multilevel_supported_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_sensor_multilevel_supported_report_t *supported_report;
	avro_value_t sensor_type_list_value;
	avro_value_t sensor_type_value;
	int i;

	supported_report = (zwp_avi_interfaces_sensor_multilevel_supported_report_t *)result;

	if (zw_avro_field_set_long(arg_value, "utime", supported_report->utime) != ZWP_STATUS_OK)
	{
		goto l_err_set_report_value;
	}

	if (avro_value_get_by_name(arg_value, "supported_types", &sensor_type_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'supported_types': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	for (i = 0; i < supported_report->supported_types_count; i++)
	{
		if (avro_value_append(&sensor_type_list_value, &sensor_type_value, 0) != 0)
		{
			goto l_err_append_sensor_type_value;
		}

		if (avro_value_set_int(&sensor_type_value, supported_report->supported_types[i]) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to set sensor type '%d': %s", supported_report->supported_types[i], avro_strerror());
			goto l_err_set_sensor_type;
		}
	}

	return ZWP_STATUS_OK;

l_err_set_sensor_type:
l_err_append_sensor_type_value:
l_err_get_list_value:
l_err_set_report_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Supported Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_sensor_multilevel_supported_get_execute_passive
 */
static void zwp_avi_interfaces_sensor_multilevel_supported_get_result_free(void *result)
{
	zwp_avi_interfaces_sensor_multilevel_supported_report_t *supported_report;

	supported_report = (zwp_avi_interfaces_sensor_multilevel_supported_report_t *)result;

	ZWP_FREE(supported_report->supported_types);

	ZWP_FREE(supported_report);
}

/**
* @brief HCAPI callback for Scale Supported Report command
* @param[in]	ifd	            interface
* @param[in]	sensor_type     sensor type, ZW_SENSOR_TYPE_XXX
* @param[in]	sensor_unit_msk bitmask of units supported for the sensor_type, ZW_SENSOR_UNIT_XXX
* @param[in]	valid           validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/
static void zwp_avi_interfaces_sensor_multilevel_scale_supported_report_handler(zwifd_p ifd, uint8_t sensor_type, uint8_t sensor_unit_msk, int valid)
{
	zwp_avi_interfaces_sensor_multilevel_state_t *state;
	zwifd_p desc_interface;
	int scale_supported_list_count, i, j, k=0;
	zwp_avi_interfaces_sensor_multilevel_single_scale_supported_report_t *pscales_per_type;
	uint8_t scale_list[8] = { 0 };

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_sensor_multilevel_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (valid)
	{
		scale_supported_list_count = zwu_array_list_size(state->type_scale_supported_report.scale_supported_list);

		for (i = 0; i < scale_supported_list_count; i++)
		{
			pscales_per_type = (zwp_avi_interfaces_sensor_multilevel_single_scale_supported_report_t *)zwu_array_list_get(state->type_scale_supported_report.scale_supported_list, i);

			if (pscales_per_type->report_receive_sensor_type == sensor_type)
			{
				//match found, delete the current data
				pscales_per_type->supported_scales_count = 0;
				if (pscales_per_type->supported_scales)
				{
					ZWP_FREE(pscales_per_type->supported_scales);
					pscales_per_type->supported_scales = NULL;
					break;
				}
			}
		}

		if (i >= scale_supported_list_count)
		{
			//Cannot find a match
			if ((pscales_per_type = (zwp_avi_interfaces_sensor_multilevel_single_scale_supported_report_t *)ZWP_CALLOC(1, sizeof(zwp_avi_interfaces_sensor_multilevel_single_scale_supported_report_t))) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				goto l_err_scale_struct_alloc;
			}

			if (zwu_array_list_add(state->type_scale_supported_report.scale_supported_list, pscales_per_type) != ZWP_STATUS_OK)
			{
				goto l_err_scale_list_add;
			}
		}

		for (j = 0, k = 0; j < 8; j++)
		{
			if (sensor_unit_msk & (0x01 << j))
			{
				scale_list[k++] = j;
			}
		}

		pscales_per_type->utime = zwu_time_now();

		pscales_per_type->supported_scales_count = k;
		pscales_per_type->report_receive_sensor_type = sensor_type;

		if (k > 0)
		{
			if ((pscales_per_type->supported_scales = (uint8_t *)ZWP_CALLOC(1, sizeof(uint8_t) * k)) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				goto l_err_scale_list_alloc;
			}

			for (j = 0; j < k; j++)
			{
				pscales_per_type->supported_scales[j] = scale_list[j];
			}
		}
	}
	zwp_interface_report_receive_done(&state->type_scale_supported_report.report_receive_state, state->type_scale_supported_report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

	ZWP_FREE(pscales_per_type->supported_scales);
l_err_scale_list_alloc:
	// Remove from list only if a new alarm_value
	if (i >= scale_supported_list_count)
	{
		zwu_array_list_remove(state->type_scale_supported_report.scale_supported_list, pscales_per_type);
	}
l_err_scale_list_add:
	// Free only if a new sensor type scales
	if (i >= scale_supported_list_count)
	{
		ZWP_FREE(pscales_per_type);
	}
l_err_scale_struct_alloc:
l_err_get_state :
	zwp_net_release_desc_interface(ifd);
l_err_acquire_desc:

	return;
}

/**
* @brief Get argument for Scale Supported Get command from Avro value object
* @param[in]     arg_value         Avro value object
* @param[out]    args_p            Arguments passed with Scale supported Get command
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zwp_avi_interfaces_sensor_multilevel_scale_supported_get_argument_free() to free resources
*/
static zwp_status_t zwp_avi_interfaces_sensor_multilevel_scale_supported_get_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_sensor_multilevel_scale_supported_get_t *argument;
	int integer;

	if ((argument = (zwp_avi_interfaces_sensor_multilevel_scale_supported_get_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "sensor_type", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_get_value_sensor_type;
	}
	argument->type = integer;

	ZWP_LOG(ZWP_LOG_DEBUG, "Sensor Type: %d", argument->type);

	if ((argument->type == COMMAND_SENSOR_TYPE_NONE)
		|| (zwu_number_is_uint8(argument->type) != ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid sensor type: %d", argument->type);
		goto l_err_invalid_sensor_type;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_get_value_sensor_type:
l_err_invalid_sensor_type :
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free arguments for Scale Supported Get command
* @param[in]     args              Arguments passed with Scale Supported Get command
* @return        Nothing
* @see zwp_avi_interfaces_sensor_multilevel_scale_supported_get_argument
*/
static void zwp_avi_interfaces_sensor_multilevel_scale_supported_get_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
* @brief Test if the expected Scale Supported Report command is received
* @param[in]     desc_interface    Interface descriptor
* @param[in]     args              Arguments (if any) passed with Get command
* @param[out]    report_receive_condition_p    Report command receive condition
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_interfaces_sensor_multilevel_scale_supported_get_report_receive_assertion(zwifd_p desc_interface, void *args,
	zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_sensor_multilevel_state_t *state;

	if ((state = (zwp_avi_interfaces_sensor_multilevel_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->type_scale_supported_report.report_receive_condition;

	if (zwp_interface_report_receive_assertion(state->type_scale_supported_report.report_receive_state) != ZWP_STATUS_OK)
	{
		goto l_err_report_not_received;
	}

	return ZWP_STATUS_OK;

l_err_report_not_received:
l_err_get_state :
	return ZWP_STATUS_ERROR;
}

/**
* @brief Execute active Scale Supported Get command
* @param[in]     desc_interface    Interface descriptor
* @param[in]     args              Arguments (if any) passed with Scale Supported Get command
* @param[out]    hcapi_code_p      HCAPI error code
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_interfaces_sensor_multilevel_scale_supported_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_sensor_multilevel_state_t *state;
	zwp_avi_interfaces_sensor_multilevel_scale_supported_get_t *argument;
	uint8_t sensor_type;

	argument = (zwp_avi_interfaces_sensor_multilevel_scale_supported_get_t *)args;

	if ((state = (zwp_avi_interfaces_sensor_multilevel_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	sensor_type = (uint8_t)argument->type;

	zwp_interface_report_receive_init(&state->type_scale_supported_report.report_receive_state);

	*hcapi_code_p = zwif_sensor_unit_get(desc_interface, sensor_type, zwp_avi_interfaces_sensor_multilevel_scale_supported_report_handler, 0);

	return ZWP_STATUS_OK;

//l_err_invalid_argument:
l_err_get_state :
	return ZWP_STATUS_ERROR;
}

/**
* @brief Execute passive Scale Supported Get command
* @param[in]     desc_interface    Interface descriptor
* @param[in]     args              Arguments (if any) passed with Scale Supported Get command
* @param[out]    result_p          Result object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zwp_avi_interfaces_alarm_get_result_free() to free resources
*/
static zwp_status_t zwp_avi_interfaces_sensor_multilevel_scale_supported_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_sensor_multilevel_state_t *state;
	zwp_avi_interfaces_sensor_multilevel_single_scale_supported_report_t *report;
	zwp_avi_interfaces_sensor_multilevel_single_scale_supported_report_t *pscales_per_type = NULL;
	zwp_avi_interfaces_sensor_multilevel_scale_supported_get_t *argument;
	int i, scale_supported_list_count;

	argument = (zwp_avi_interfaces_sensor_multilevel_scale_supported_get_t *)args;

	if ((state = (zwp_avi_interfaces_sensor_multilevel_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((report = (zwp_avi_interfaces_sensor_multilevel_single_scale_supported_report_t *)ZWP_CALLOC(1, sizeof(*report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	scale_supported_list_count = zwu_array_list_size(state->type_scale_supported_report.scale_supported_list);

	for (i = 0; i < scale_supported_list_count; i++)
	{
		pscales_per_type = (zwp_avi_interfaces_sensor_multilevel_single_scale_supported_report_t *)zwu_array_list_get(state->type_scale_supported_report.scale_supported_list, i);

		if (pscales_per_type->report_receive_sensor_type == argument->type)
		{
			//match found
			memcpy(report, pscales_per_type, sizeof(zwp_avi_interfaces_sensor_multilevel_single_scale_supported_report_t));
			if (pscales_per_type->supported_scales)
			{
				if ((report->supported_scales = (uint8_t *)ZWP_CALLOC(1, sizeof(uint8_t) * pscales_per_type->supported_scales_count)) == NULL)
				{
					ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
					goto l_err_report_supported_event_alloc;
				}

				memcpy(report->supported_scales, pscales_per_type->supported_scales, pscales_per_type->supported_scales_count);
				break;
			}
		}
	}

	*result_p = report;

	return ZWP_STATUS_OK;

	ZWP_FREE(report->supported_scales);
l_err_report_supported_event_alloc:
	ZWP_FREE(report);
l_err_report_alloc:
l_err_get_state :
	return ZWP_STATUS_ERROR;
}

/**
* @brief Set Avro value object with values from Result object corresponding to Scale Supported Report command
* @param[in]     result            Result object
* @param[out]    arg_value         Avro value object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_interfaces_sensor_multilevel_scale_supported_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_sensor_multilevel_single_scale_supported_report_t *scale_supported_report;
	avro_value_t zwave_sensor_multilevel_scale_list_value;
	avro_value_t zwave_sensor_multilevel_scale_value;
	int i;

	scale_supported_report = (zwp_avi_interfaces_sensor_multilevel_single_scale_supported_report_t *)result;

	if ((zw_avro_field_set_long(arg_value, "utime", scale_supported_report->utime) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_int(arg_value, "sensor_type", scale_supported_report->report_receive_sensor_type) != ZWP_STATUS_OK))
	{
		goto l_err_set_report_value;
	}

	if (avro_value_get_by_name(arg_value, "supported_scales", &zwave_sensor_multilevel_scale_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'supported_scales': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	for (i = 0; i < scale_supported_report->supported_scales_count; i++)
	{
		if (avro_value_append(&zwave_sensor_multilevel_scale_list_value, &zwave_sensor_multilevel_scale_value, 0) != 0)
		{
			goto l_err_append_zwave_sensor_multilevel_scale_value;
		}

		if (avro_value_set_int(&zwave_sensor_multilevel_scale_value, scale_supported_report->supported_scales[i]) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to set scale '%d': %s", scale_supported_report->supported_scales[i], avro_strerror());
			goto l_err_set_zwave_sensor_multilevel_scale;
		}
	}

	return ZWP_STATUS_OK;

l_err_set_zwave_sensor_multilevel_scale:
l_err_append_zwave_sensor_multilevel_scale_value :
l_err_get_list_value :
l_err_set_report_value :
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free Result object corresponding to Scale Supported Report command
* @param[in]     result            Result object
* @return        Nothing
* @see zwp_avi_interfaces_sensor_multilevel_scale_supported_get_execute_passive
*/
static void zwp_avi_interfaces_sensor_multilevel_scale_supported_get_result_free(void *result)
{
	zwp_avi_interfaces_sensor_multilevel_single_scale_supported_report_t *scale_supported_report;

	scale_supported_report = (zwp_avi_interfaces_sensor_multilevel_single_scale_supported_report_t *)result;

	ZWP_FREE(scale_supported_report->supported_scales);

	ZWP_FREE(scale_supported_report);
}


/** Handlers for Get command */
static zwp_avi_message_interfaces_t interfaces_sensor_multilevel_get =
{
		"interface_sensor_multilevel_get",
		COMMAND_CLASS_SENSOR_MULTILEVEL, zwp_avi_interfaces_sensor_multilevel_setup,
		zwp_avi_interfaces_sensor_multilevel_state_alloc, zwp_avi_interfaces_sensor_multilevel_state_free,
		zwp_avi_interfaces_sensor_multilevel_get_argument, zwp_avi_interfaces_sensor_multilevel_get_argument_free,
		zwp_avi_interfaces_sensor_multilevel_get_report_receive_assertion,
		zwp_avi_interfaces_sensor_multilevel_get_execute_active, zwp_avi_interfaces_sensor_multilevel_get_execute_passive,
		zwp_avi_interfaces_sensor_multilevel_get_result, zwp_avi_interfaces_sensor_multilevel_get_result_free
};

/** Handlers for Supported Get command */
static zwp_avi_message_interfaces_t interfaces_sensor_multilevel_supported_get =
{
		"interface_sensor_multilevel_supported_get",
		COMMAND_CLASS_SENSOR_MULTILEVEL, NULL,
		NULL, NULL,
		NULL, NULL,
		zwp_avi_interfaces_sensor_multilevel_supported_get_report_receive_assertion,
		zwp_avi_interfaces_sensor_multilevel_supported_get_execute_active, zwp_avi_interfaces_sensor_multilevel_supported_get_execute_passive,
		zwp_avi_interfaces_sensor_multilevel_supported_get_result, zwp_avi_interfaces_sensor_multilevel_supported_get_result_free
};

/** Handlers for Scale Supported Get command */
static zwp_avi_message_interfaces_t interfaces_sensor_multilevel_scale_supported_get =
{
		"interface_sensor_multilevel_scale_supported_get",
		COMMAND_CLASS_SENSOR_MULTILEVEL, NULL,
		NULL, NULL,
		zwp_avi_interfaces_sensor_multilevel_scale_supported_get_argument, zwp_avi_interfaces_sensor_multilevel_scale_supported_get_argument_free,
		//NULL,
		zwp_avi_interfaces_sensor_multilevel_scale_supported_get_report_receive_assertion,
		zwp_avi_interfaces_sensor_multilevel_scale_supported_get_execute_active, zwp_avi_interfaces_sensor_multilevel_scale_supported_get_execute_passive,
		zwp_avi_interfaces_sensor_multilevel_scale_supported_get_result, zwp_avi_interfaces_sensor_multilevel_scale_supported_get_result_free
};

/** Register command handlers */
zwp_status_t zwp_avi_interfaces_sensor_multilevel_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_sensor_multilevel_get) != ZWP_STATUS_OK)||
		(zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_sensor_multilevel_supported_get) != ZWP_STATUS_OK)||
		(zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_sensor_multilevel_scale_supported_get) != ZWP_STATUS_OK)
		)
	{
		goto l_err_interface_list_get;
	}

	return ZWP_STATUS_OK;

l_err_interface_list_get:
	return ZWP_STATUS_ERROR;
}
