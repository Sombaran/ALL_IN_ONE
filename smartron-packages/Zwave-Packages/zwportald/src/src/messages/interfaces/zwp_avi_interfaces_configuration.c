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
 * @file    zwp_avi_interfaces_configuration.c
 *
 * @brief   AVI message 'interface_configuration_xxx'
 * @details Handles the AVI message 'interface_configuration_xxx'.
 *
 * @author  Sasidhar
 *
 * @version 1.0 - 2013-11-14
 * - Initial version
 */

#include "zwp_avi_interfaces.h"

/** Configuration parameter not specified */
#define COMMAND_CONFIGURATION_PARAMETER_NONE (-1)

/** Set command state */
typedef struct {
	zwconfig_t		config;                                /**< Configuration */
} zwp_avi_interfaces_configuration_set_t;

/** Get command state */
typedef struct {
	uint8_t		param_num;                                 /**< Parameter number */
} zwp_avi_interfaces_configuration_get_t;

/** Configuration parameter value state */
typedef struct {
	zwu_time_t         utime;                              /**< Last update time for specific config parameter */
	zwconfig_t		   config;                             /**< Configuration */
} zwp_avi_interfaces_configuration_report_param_config_value_t;

/** Report command state */
typedef struct {
	zwu_time_t         utime;                              /**< Last update time for across all config parameters */
	zwu_array_list_t   config_param_value_list;            /**< List of configurations of type @a zwp_avi_interfaces_configuration_report_param_config_value_t */

	int                report_receive_state;               /**< Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Report command receive condition */
	int                report_receive_param_num;           /**< Current parameter number for Report command receive condition */
} zwp_avi_interfaces_configuration_report_t;

/** Interface level state */
typedef struct {
	zwp_avi_interfaces_configuration_report_t      report; /**< Report command state */
} zwp_avi_interfaces_configuration_state_t;

/**
 * @brief Allocate interface level data
 * @param[out]    state_p           Interface level data
 * @param[in]     pool              Memory pool to be used for allocation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_configuration_state_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_configuration_state_alloc(void **state_p, zwp_pool_t pool)
{
	zwp_avi_interfaces_configuration_state_t *state;

	if ((state = (zwp_avi_interfaces_configuration_state_t *)ZWP_CALLOC(1, sizeof(*state))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_state_alloc;
	}

	state->report.report_receive_param_num = COMMAND_CONFIGURATION_PARAMETER_NONE;

	if (zwu_array_list_create(0, &state->report.config_param_value_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	if (zwp_thread_cond_create(&state->report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for report received event");
		goto l_err_net_report_received_cond_create;
	}

	*state_p = state;

	return ZWP_STATUS_OK;

l_err_net_report_received_cond_create:
l_err_value_list_create:
	ZWP_FREE(state);
l_err_state_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free interface level data
 * @param[in]     arg               Interface level data
 * @return        Nothing
 * @see zwp_avi_interfaces_configuration_state_alloc
 */
static void zwp_avi_interfaces_configuration_state_free(void *arg)
{
	zwp_avi_interfaces_configuration_state_t *state;
	zwp_avi_interfaces_configuration_report_t *report;
	int configuration_value_count;
	int i;

	state = (zwp_avi_interfaces_configuration_state_t *)arg;
	report = &state->report;

	configuration_value_count = zwu_array_list_size(report->config_param_value_list);

	for (i = 0; i < configuration_value_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(report->config_param_value_list, i));
	}

	zwu_array_list_destroy(report->config_param_value_list);

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
static zwp_status_t zwp_avi_interfaces_configuration_report_update(zwp_avi_interfaces_configuration_report_t *report,
		zwconfig_p data)
{
	int configuration_value_count;
	int i;
	zwu_time_t  time_now;
	zwp_avi_interfaces_configuration_report_param_config_value_t *configuration_value;

	configuration_value_count = zwu_array_list_size(report->config_param_value_list);

	for (i = 0; i < configuration_value_count; i++)
	{
		configuration_value = zwu_array_list_get(report->config_param_value_list, i);
		// If parameter number matches, update the entry instead of creating new one.
		if (configuration_value->config.param_num == data->param_num)
		{
			break;
		}
	}

	if (i >= configuration_value_count)
	{
		if ((configuration_value = (zwp_avi_interfaces_configuration_report_param_config_value_t *)ZWP_CALLOC(1, sizeof(*configuration_value))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		if (zwu_array_list_add(report->config_param_value_list, configuration_value) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}
	}

	time_now = zwu_time_now();

	report->utime = time_now;
	configuration_value->utime = time_now;

	if(data)
	{
		memcpy(&configuration_value->config, data, sizeof(configuration_value->config));
	}
	else
	{
		memset(&configuration_value->config, 0, sizeof(configuration_value->config));
	}


	return ZWP_STATUS_OK;

l_err_value_list_add:
	ZWP_FREE(configuration_value);
l_err_value_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief HCAPI callback for Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  param	            parameter value
 * @return        Nothing
 */
static void zwp_avi_interfaces_configuration_report_handler(zwifd_p ifd, zwconfig_p param)
{
	zwp_avi_interfaces_configuration_state_t *state;
	zwifd_p desc_interface;


	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_configuration_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (zwp_avi_interfaces_configuration_report_update(&state->report,param) != ZWP_STATUS_OK)
	{
			goto l_err_report_update;
	}

	state->report.report_receive_param_num = param->param_num;

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
static zwp_status_t zwp_avi_interfaces_configuration_setup(zwifd_p desc_interface, int *hcapi_code_p)
{
	*hcapi_code_p = zwif_config_rpt_set(desc_interface, zwp_avi_interfaces_configuration_report_handler);

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
 * The caller should call zwp_avi_interfaces_configuration_get_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_configuration_get_argument(avro_value_t *arg_value, void **args_p)
 {
 	zwp_avi_interfaces_configuration_get_t *argument;
 	int integer;

 	if ((argument = (zwp_avi_interfaces_configuration_get_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
 	{
 		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
 		goto l_err_args_alloc;
 	}

 	if (zw_avro_field_get_int(arg_value, "parameternumber", &integer) != ZWP_STATUS_OK)
 	{
 		goto l_err_get_value;
 	}
 	argument->param_num = (uint8_t)integer;

 	*args_p = argument;

 	return ZWP_STATUS_OK;

 l_err_get_value:
 	ZWP_FREE(argument);
 l_err_args_alloc:
 	return ZWP_STATUS_ERROR;
 }

/**
 * @brief Free arguments for Get command
 * @param[in]     args              Arguments passed with Get command
 * @return        Nothing
 * @see zwp_avi_interfaces_configuration_get_argument
 */
static void zwp_avi_interfaces_configuration_get_argument_free(void *args)
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
static zwp_status_t zwp_avi_interfaces_configuration_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_configuration_state_t *state;
	zwp_avi_interfaces_configuration_get_t *argument;

	argument = (zwp_avi_interfaces_configuration_get_t *)args;

	if ((state = (zwp_avi_interfaces_configuration_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->report.report_receive_condition;

	if ((zwp_interface_report_receive_assertion(state->report.report_receive_state) != ZWP_STATUS_OK)
		|| ((argument->param_num != COMMAND_CONFIGURATION_PARAMETER_NONE) && (state->report.report_receive_param_num != argument->param_num)))
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
static zwp_status_t zwp_avi_interfaces_configuration_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_configuration_state_t *state;
	zwp_avi_interfaces_configuration_get_t *argument;
	uint8_t param_num;

	argument = (zwp_avi_interfaces_configuration_get_t *)args;

	if ((state = (zwp_avi_interfaces_configuration_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->report.report_receive_state);

	// When param_num not specified, default to 0..
	// 0 is not a valid value. So this will lead to error message.
	param_num = (argument->param_num == COMMAND_CONFIGURATION_PARAMETER_NONE) ? 0 : (uint8_t)argument->param_num;

	*hcapi_code_p = zwif_config_get(desc_interface, param_num);

	return ZWP_STATUS_OK;

l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Allocate Result object corresponding to Report command
 * @param[out]    report_dest       Destination Report
 * @param[in]     report_src        Source Report
 * @param[in]     config_param      Configuration parameter
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_configuration_get_result_alloc(zwp_avi_interfaces_configuration_report_t *report_dest,
		zwp_avi_interfaces_configuration_report_t *report_src, int config_param)
{
	int configuration_value_count;
	int i;
	zwp_avi_interfaces_configuration_report_param_config_value_t *configuration_value_dest;
	zwp_avi_interfaces_configuration_report_param_config_value_t *configuration_value_src;

	configuration_value_count = zwu_array_list_size(report_src->config_param_value_list);

	for (i = 0; i < configuration_value_count; i++)
	{
		configuration_value_src = zwu_array_list_get(report_src->config_param_value_list, i);

		// If parameter number doesn't match, skip the entry
		if ((config_param != COMMAND_CONFIGURATION_PARAMETER_NONE) && (configuration_value_src->config.param_num != config_param))
		{
			continue;
		}

		if ((configuration_value_dest = (zwp_avi_interfaces_configuration_report_param_config_value_t *)ZWP_CALLOC(1, sizeof(*configuration_value_dest))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		if (zwu_array_list_add(report_dest->config_param_value_list, configuration_value_dest) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}

		memcpy(configuration_value_dest, configuration_value_src, sizeof(*configuration_value_dest));
	}

	return ZWP_STATUS_OK;

l_err_value_list_add:
	ZWP_FREE(configuration_value_dest);
l_err_value_alloc:
	for (i--; i >= 0; i--)
	{
		ZWP_FREE(zwu_array_list_get(report_dest->config_param_value_list, i));
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
 * The caller should call zwp_avi_interfaces_configuration_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_configuration_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_configuration_state_t *state;
	zwp_avi_interfaces_configuration_report_t *report;
	zwp_avi_interfaces_configuration_get_t *argument;


	argument = (zwp_avi_interfaces_configuration_get_t *)args;

	if ((state = (zwp_avi_interfaces_configuration_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((report = (zwp_avi_interfaces_configuration_report_t *)ZWP_CALLOC(1, sizeof(*report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	if (zwu_array_list_create(0, &report->config_param_value_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	if (zwp_avi_interfaces_configuration_get_result_alloc(report, &state->report, argument->param_num) != ZWP_STATUS_OK)
	{
		goto l_err_result_alloc;
	}


	*result_p = report;

	return ZWP_STATUS_OK;

l_err_result_alloc:
	zwu_array_list_destroy(report->config_param_value_list);
l_err_value_list_create:
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
static zwp_status_t zwp_avi_interfaces_configuration_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_configuration_report_t *report;
	int i=0, j=0;
	avro_value_t configvalue_array;
	avro_value_t configvalue_element;
	avro_value_t config_param_value_list;
	avro_value_t config_param_value;
	zwp_avi_interfaces_configuration_report_param_config_value_t * config_param_report;

	int config_param_value_count;

	report = (zwp_avi_interfaces_configuration_report_t *)result;

	if (avro_value_get_by_name(arg_value, "configuration_param_value_list", &config_param_value_list, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'configuration_param_value_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	config_param_value_count = zwu_array_list_size(report->config_param_value_list);

	for (i = 0; i < config_param_value_count; i++)
	{
		if (avro_value_append(&config_param_value_list, &config_param_value, 0) != 0)
		{
			goto l_err_append_configuration_value;
		}

		config_param_report = zwu_array_list_get(report->config_param_value_list, i);

		if ((zw_avro_field_set_long(&config_param_value, "utime", config_param_report->utime) != ZWP_STATUS_OK)
				 || (zw_avro_field_set_int(&config_param_value, "parameternumber", config_param_report->config.param_num) != ZWP_STATUS_OK)
				 || (zw_avro_field_set_int(&config_param_value, "size", config_param_report->config.size) != ZWP_STATUS_OK)
			)
		{
			goto l_err_set_report_value;
		}


		if (avro_value_get_by_name(&config_param_value, "configvalue_array", &configvalue_array, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'configvalue_array': %s", avro_strerror());
			goto l_err_get_array_value;
		}


		for(j=0; j<config_param_report->config.size; j++)
		{
			if (avro_value_append(&configvalue_array, &configvalue_element, 0) != 0)
			{
				goto l_err_append_config_value_element;
			}

			if (avro_value_set_int(&configvalue_element, config_param_report->config.data[j]) != 0)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to set value '%d' to the field configvalue : %s", config_param_report->config.data[j], avro_strerror);
				goto l_err_set_value;
			}
		}
	}

	return ZWP_STATUS_OK;

l_err_set_value:
l_err_append_config_value_element:
l_err_get_array_value:
l_err_set_report_value:
l_err_append_configuration_value:
l_err_get_list_value:

	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_configuration_get_execute_passive
 */
static void zwp_avi_interfaces_configuration_get_result_free(void *result)
{
	zwp_avi_interfaces_configuration_report_t *report;
	int configuration_value_count;
	int i;

	report = (zwp_avi_interfaces_configuration_report_t *)result;

	configuration_value_count = zwu_array_list_size(report->config_param_value_list);

	for (i = 0; i < configuration_value_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(report->config_param_value_list, i));
	}
	zwu_array_list_destroy(report->config_param_value_list);

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
 * The caller should call zwp_avi_interfaces_configuration_set_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_configuration_set_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_configuration_set_t *argument;
	avro_value_t config_value_array;
	avro_value_t config_value;
	int use_default, param_num, size,i, value;

	if ((argument = (zwp_avi_interfaces_configuration_set_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if ((zw_avro_field_get_int(arg_value, "parameternumber", &param_num) != ZWP_STATUS_OK)
	    ||(zw_avro_field_get_int(arg_value, "default", &use_default) != ZWP_STATUS_OK)

	   )
	{
		goto l_err_get_value;
	}

	argument->config.param_num = (uint8_t)param_num;
	argument->config.use_default = (uint8_t)use_default;

	if(argument->config.use_default == 0)
	{
		 if (zw_avro_field_get_int(arg_value, "size", &size) != ZWP_STATUS_OK)
		 {
			 goto l_err_get_size_value;
		 }
		 argument->config.size = (uint8_t)size;

		 if (avro_value_get_by_name(arg_value, "configvalue_array", &config_value_array, NULL) != 0)
		 {
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'configvalue_array': %s", avro_strerror());
			goto l_err_get_array_value;
		 }

		 for (i = 0; i < argument->config.size; i++)
		 {
			if (avro_value_get_by_index(&config_value_array, i, &config_value, NULL) != 0)
			{
				goto l_err_get_config_value;
			}

			if (avro_value_get_int(&config_value, &value) != ZWP_STATUS_OK)
			{
				goto l_err_get_cofnig_field_value;
			}

			if(i < sizeof(argument->config.data))
			{
				argument->config.data[i] = (uint8_t)value;
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		//TDOD: HC API returning ERROR if size is zero
		argument->config.size = 4;
		memset(argument->config.data, 0, sizeof(argument->config.data));
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_get_cofnig_field_value:
l_err_get_config_value:
l_err_get_array_value:
l_err_get_size_value:
l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Set command
 * @param[in]     args              Arguments passed with Set command
 * @return        Nothing
 * @see zwp_avi_interfaces_configuration_set_argument
 */
static void zwp_avi_interfaces_configuration_set_argument_free(void *args)
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
static zwp_status_t zwp_avi_interfaces_configuration_set_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_configuration_set_t *argument;

	argument = (zwp_avi_interfaces_configuration_set_t *)args;

	*hcapi_code_p = zwif_config_set(desc_interface, &argument->config);

	return ZWP_STATUS_OK;
}

/** Handlers for Get command */
static zwp_avi_message_interfaces_t interfaces_configuration_get =
{
		"interface_configuration_get",
		COMMAND_CLASS_CONFIGURATION, zwp_avi_interfaces_configuration_setup,
		zwp_avi_interfaces_configuration_state_alloc, zwp_avi_interfaces_configuration_state_free,
		zwp_avi_interfaces_configuration_get_argument, zwp_avi_interfaces_configuration_get_argument_free,
		zwp_avi_interfaces_configuration_get_report_receive_assertion,
		zwp_avi_interfaces_configuration_get_execute_active, zwp_avi_interfaces_configuration_get_execute_passive,
		zwp_avi_interfaces_configuration_get_result, zwp_avi_interfaces_configuration_get_result_free
};

/** Handlers for Set command */
static zwp_avi_message_interfaces_t interfaces_configuration_set =
{
		"interface_configuration_set",
		COMMAND_CLASS_CONFIGURATION, NULL,
		NULL, NULL,
		zwp_avi_interfaces_configuration_set_argument, zwp_avi_interfaces_configuration_set_argument_free,
		NULL,
		zwp_avi_interfaces_configuration_set_execute_active, NULL,
		NULL, NULL
};

/** Register command handlers */
zwp_status_t zwp_avi_interfaces_configuration_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_configuration_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_configuration_set) != ZWP_STATUS_OK))
	{
		goto l_err_interface_list_get;
	}

	return ZWP_STATUS_OK;

l_err_interface_list_get:
	return ZWP_STATUS_ERROR;
}
