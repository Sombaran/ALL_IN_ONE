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
 * @file    zwp_avi_interfaces_alarm.c
 *
 * @brief   AVI message 'interface_alarm_xxx'
 * @details Handles the AVI message 'interface_alarm_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-02-21
 * - Initial version
 */

#include "zwp_avi_interfaces_alarm.h"

/**
 * @brief Allocate interface level data
 * @param[out]    state_p           Interface level data
 * @param[in]     pool              Memory pool to be used for allocation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_alarm_state_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_alarm_state_alloc(void **state_p, zwp_pool_t pool)
{
	zwp_avi_interfaces_alarm_state_t *state;

	if ((state = (zwp_avi_interfaces_alarm_state_t *)ZWP_CALLOC(1, sizeof(*state))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_state_alloc;
	}

	if (zwp_thread_cond_create(&state->report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for report received event");
		goto l_err_net_report_received_cond_create;
	}

	state->report.first_alarm_type = COMMAND_ALARM_TYPE_NONE;
	state->report.first_zwave_alarm_type = COMMAND_ZWAVE_ALARM_TYPE_NONE;
	state->report.wait_for_first_alarm = 0;

	state->report.report_receive_alarm_type = COMMAND_ALARM_TYPE_NONE;
	state->report.report_receive_zwave_alarm_type = COMMAND_ZWAVE_ALARM_TYPE_NONE;
	state->report.report_receive_zwave_alarm_event = COMMAND_ZWAVE_ALARM_EVENT_NONE;

	if (zwu_array_list_create(0, &state->report.alarm_value_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	state->report.alarm_value_last_updated = -1;

	//Type supported
	if (zwp_thread_cond_create(&state->type_supported_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for type_supported_report received event");
		goto l_err_net_type_supported_report_received_cond_create;
	}

	state->type_supported_report.supported_types = NULL;
	state->type_supported_report.supported_types_count = 0;

	//Event supported
	if (zwp_thread_cond_create(&state->type_event_supported_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for type_event_supported_report received event");
		goto l_err_net_event_supported_report_received_cond_create;
	}

	if (zwu_array_list_create(0, &state->type_event_supported_report.event_supported_list) != ZWP_STATUS_OK)
	{
			goto l_err_event_list_create;
	}

	*state_p = state;

	return ZWP_STATUS_OK;

	zwu_array_list_destroy(state->type_event_supported_report.event_supported_list);
l_err_event_list_create:
	zwp_thread_cond_destroy(state->type_event_supported_report.report_receive_condition);
l_err_net_event_supported_report_received_cond_create:
	zwp_thread_cond_destroy(state->type_supported_report.report_receive_condition);
l_err_net_type_supported_report_received_cond_create:
	zwu_array_list_destroy(state->report.alarm_value_list);
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
 * @see zwp_avi_interfaces_alarm_state_alloc
 */
static void zwp_avi_interfaces_alarm_state_free(void *arg)
{
	zwp_avi_interfaces_alarm_state_t *state;
	zwp_avi_interfaces_alarm_report_t *report;
	int alarm_value_count, alarm_event_count;
	int i;
	zwp_avi_interfaces_alarm_report_alarm_value_t *alarm_value;
	zwp_avi_interfaces_alarm_single_event_supported_report_t *alarm_event;

	state = (zwp_avi_interfaces_alarm_state_t *)arg;
	report = &state->report;

	zwp_interface_report_receive_done(&state->type_supported_report.report_receive_state, state->type_supported_report.report_receive_condition);

	zwp_thread_cond_destroy(state->type_supported_report.report_receive_condition);

	ZWP_FREE(state->type_supported_report.supported_types);

	zwp_interface_report_receive_done(&state->type_event_supported_report.report_receive_state, state->type_event_supported_report.report_receive_condition);

	zwp_thread_cond_destroy(state->type_event_supported_report.report_receive_condition);

	if(state->type_event_supported_report.event_supported_list)
	{
		alarm_event_count = zwu_array_list_size(state->type_event_supported_report.event_supported_list);

		for (i = 0; i < alarm_event_count; i++)
		{
			alarm_event = zwu_array_list_get(state->type_event_supported_report.event_supported_list, i);

			//zwp_interface_report_receive_done(&alarm_event->report_receive_state, alarm_event->report_receive_condition);

			//zwp_thread_cond_destroy(alarm_event->report_receive_condition);

			ZWP_FREE(alarm_event->supported_events);
			ZWP_FREE(alarm_event);
		}
		zwu_array_list_destroy(state->type_event_supported_report.event_supported_list);
	}

	alarm_value_count = zwu_array_list_size(report->alarm_value_list);

	for (i = 0; i < alarm_value_count; i++)
	{
		alarm_value = zwu_array_list_get(report->alarm_value_list, i);

		ZWP_FREE(alarm_value->event_parameter);

		ZWP_FREE(alarm_value);
	}

	zwu_array_list_destroy(report->alarm_value_list);

	zwp_interface_report_receive_done(&state->report.report_receive_state, state->report.report_receive_condition);

	zwp_thread_cond_destroy(state->report.report_receive_condition);

#ifndef LOCAL_CLIENT_ENABLE
	zwp_avi_interfaces_alarm_state_free_ifttt(state);
#endif

	ZWP_FREE(state);
}

static void zwp_avi_interfaces_alarm_report_free_single(zwu_array_list_t alarm_value_list, int index)
{
	int alarm_value_count = 0;
	zwp_avi_interfaces_alarm_report_alarm_value_t *alarm_value = NULL;

	alarm_value_count = zwu_array_list_size(alarm_value_list);

	if(index < alarm_value_count)
	{
		alarm_value = zwu_array_list_get(alarm_value_list, index);

		if(alarm_value)
		{
			zwu_array_list_remove_at_index(alarm_value_list, index);

			ZWP_FREE(alarm_value->event_parameter);

			ZWP_FREE(alarm_value);
		}
	}
}

/**
 * @brief Update interface level state with the new Report
 * @param[in]     report            Report state in interface level
 * @param[in]     alarm_info        Value from new Report
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_alarm_report_update(zwp_avi_interfaces_alarm_report_t *report,
		zwalrm_p alarm_info, zwu_time_t ts)
{
	int alarm_value_count;
	int i;
	//zwu_time_t  time_now;
	zwp_avi_interfaces_alarm_report_alarm_value_t *alarm_value = NULL;
	//void *tmp;

	/*** Angela 20 May 2015
	* When controller issues alarm get with specific zwave alarm type and event, the device response could be:
	* a. status 0xFE, no zwave alarm type and event (HCAPI will fill in 0)
	* b. same zwave alarm type but event 0, or same zwave alarm type but event 0xFE
	* c. same zwave alarm type and same event
	*
	* Therefore, the matching mechanism is implemented as following:
	* 1. sort the report alarm value list by the time recevied.
	* 2. When a match (receive_assition function and get_reuslt function) need to be done, match shall start from the last/latest
	*    entry in the list. 
	* 3. Match will check for all 3 conditions. Condition a will be treated as a wild card which will match any 
	*    Alarm get arguments.
	* 4. Once a match is found, the entry will be returned, regardless whether there is other (closer but older value) match in the list.
	*
	* For Alarm Get with 0xFF, always return the last/latest value in the list.
	****/
	alarm_value_count = zwu_array_list_size(report->alarm_value_list);

	for (i = 0; i < alarm_value_count; i++)
	{
		alarm_value = zwu_array_list_get(report->alarm_value_list, i);

		// If all of alarm_type, zwave_alarm_type, zwave_alarm_event matches, delete the entry and create a new one and add to
		//the end of list.
		if(alarm_value->value.ex_info == 0)
		{
			if((alarm_info->ex_info == 0) &&
			   (alarm_value->value.type == alarm_info->type))
			{
				break;
			}
		}
		else
		{
			if((alarm_info->ex_info == 1) &&
			   (alarm_value->value.type == alarm_info->type) &&
			   (alarm_value->value.ex_type == alarm_info->ex_type) &&
			   (alarm_value->value.ex_event == alarm_info->ex_event))
			{
				break;
			}

		}
	}

	if(i < alarm_value_count)
	{
		//match found. Delete the entry and create a new one.
		zwp_avi_interfaces_alarm_report_free_single(report->alarm_value_list, i);
	}

	if ((alarm_value = (zwp_avi_interfaces_alarm_report_alarm_value_t *)ZWP_CALLOC(1, sizeof(*alarm_value))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_value_alloc;
	}

	alarm_value->event_parameter = NULL;

	//time_now = zwu_time_now();

	//report->utime = time_now;
	report->utime = ts;
	alarm_value->utime = ts;
	//alarm_value->utime = time_now;

	memcpy(&alarm_value->value, alarm_info, sizeof(alarm_value->value));

	if (alarm_value->value.ex_evt_len > 0)
	{
		if ((alarm_value->event_parameter = ZWP_CALLOC(1, alarm_value->value.ex_evt_len)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_event_parameter_alloc;
		}
	}

	memcpy(alarm_value->event_parameter, alarm_info->ex_evt_prm, alarm_value->value.ex_evt_len);

	if (zwu_array_list_add(report->alarm_value_list, alarm_value) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_add;
	}
	
	report->alarm_value_last_updated = zwu_array_list_size(report->alarm_value_list) - 1;

	return ZWP_STATUS_OK;

l_err_value_list_add:
l_err_event_parameter_alloc:
l_err_value_alloc:
	if(alarm_value)
	{
		ZWP_FREE(alarm_value->event_parameter);
		ZWP_FREE(alarm_value);
	}

	report->alarm_value_last_updated = zwu_array_list_size(report->alarm_value_list) - 1;

	return ZWP_STATUS_ERROR;
}

/**
 * @brief HCAPI callback for Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  alarm_info	    Alarm information
 * @return        Nothing
 */
static void zwp_avi_interfaces_alarm_report_handler(zwifd_p ifd, zwalrm_p alarm_info, time_t ts)
{
	zwp_avi_interfaces_alarm_state_t *state;
	zwifd_p desc_interface;

	if (ts)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "type=%u level=%u", alarm_info->type, alarm_info->level);
		if (alarm_info->ex_info)
		{
			ZWP_LOG(ZWP_LOG_DEBUG, "ztype=%d event=%d ex_evt_len=%d ts=%lu", 
				alarm_info->ex_type, alarm_info->ex_event, alarm_info->ex_evt_len, ts);
			if (alarm_info->ex_evt_len > 0)
				ZWP_LOG(ZWP_LOG_DEBUG, "ex_evt_prm = %d", *(alarm_info->ex_evt_prm));
		}

		//Causes invalid memory read for some reason
		//ZWP_LOG_DUMP(ZWP_LOG_DEBUG, (char *)alarm_info, sizeof (*alarm_info));

#if 0
		{
			static zwalrm_t g_alarm_info;

			if (!memcmp(&g_alarm_info, alarm_info, sizeof(g_alarm_info)))
			{
				ZWP_LOG(ZWP_LOG_DEBUG, "Skipping this report");
				return;
			}
			memcpy(&g_alarm_info, alarm_info, sizeof(g_alarm_info));
		}
#endif
	}

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_alarm_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}


	if(ts)
	{
#ifdef ZWP_INTERFACE_ALARM_TEST

		zwalrm_p temp_alarm_info;

		if ((temp_alarm_info = (zwalrm_p)ZWP_CALLOC(1, sizeof(zwalrm_t) + alarm_info->ex_evt_len - 1)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			return;
		}

		memcpy(temp_alarm_info, alarm_info, sizeof(zwalrm_t) + alarm_info->ex_evt_len - 1);

		alarm_info = temp_alarm_info;

		{
			static int times;

			switch (times)
			{
			case 0:
				alarm_info->ex_type = ZW_ALRM_CO;
				alarm_info->ex_event = ZW_ALRM_EVT_CO_L;
				alarm_info->ex_evt_type = ZW_ALRM_PARAM_LOC;
				memcpy(alarm_info->ex_evt_prm, "Kitchen", sizeof("Kitchen"));
				alarm_info->ex_evt_len = sizeof("Kitchen");
				break;

			case 1:
				alarm_info->ex_type = ZW_ALRM_LOCK;
				alarm_info->ex_event = ZW_ALRM_EVT_KEYPAD_LCK;
				alarm_info->ex_evt_type = ZW_ALRM_PARAM_USRID;
				alarm_info->ex_evt_prm[0] = 6;
				alarm_info->ex_evt_len = 1;
				break;

			case 2:
				alarm_info->ex_type = ZW_ALRM_SYSTEM;
				alarm_info->ex_event = ZW_ALRM_EVT_SW_OEM_CODE;
				alarm_info->ex_evt_type = ZW_ALRM_PARAM_OEM_ERR_CODE;
				alarm_info->ex_evt_prm[0] = 0xDE;
				alarm_info->ex_evt_prm[1] = 0xAD;
				alarm_info->ex_evt_len = 2;
				break;

			case 3:
				alarm_info->level = 255;
				break;

			default:
				break;
			}

			times++;

		}
#endif

		if (zwp_avi_interfaces_alarm_report_update(&state->report, alarm_info, ts) != ZWP_STATUS_OK)
		{
			goto l_err_report_update;
		}

		/*
		 * The alarm_type and zwave_alarm_type fields in the first Report received after a Get with ZW_ALRM_FIRST
		 * are noted down to fetch the corresponding entry during subsequent passive calls with ZW_ALRM_FIRST
		 */
		if (state->report.wait_for_first_alarm && (alarm_info->ex_info == 1))
		{
			state->report.first_alarm_type = alarm_info->type;
			state->report.first_zwave_alarm_type = alarm_info->ex_type;

			state->report.wait_for_first_alarm = 0;
		}

		state->report.report_receive_alarm_type = alarm_info->type;
		state->report.report_receive_zwave_alarm_type = (alarm_info->ex_info == 1) ? alarm_info->ex_type : 0;
		state->report.report_receive_zwave_alarm_event = (alarm_info->ex_info == 1) ? alarm_info->ex_event : 0;

#ifndef LOCAL_CLIENT_ENABLE
		zwp_avi_interfaces_alarm_report_handler_ifttt(desc_interface, alarm_info, ts);	
#endif

	}

	zwp_interface_report_receive_done(&state->report.report_receive_state, state->report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

#ifdef ZWP_INTERFACE_ALARM_TEST
	ZWP_FREE(temp_alarm_info);
#endif

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
static zwp_status_t zwp_avi_interfaces_alarm_setup(zwifd_p desc_interface, int *hcapi_code_p)
{
	*hcapi_code_p = hcaal_alrm_rpt_set(desc_interface, zwp_avi_interfaces_alarm_report_handler);

#ifndef LOCAL_CLIENT_ENABLE
	if (zwp_avi_interfaces_alarm_state_alloc_ifttt(desc_interface) != ZWP_STATUS_OK)
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
 * The caller should call zwp_avi_interfaces_alarm_get_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_alarm_get_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_alarm_get_t *argument;
	int integer;

	if ((argument = (zwp_avi_interfaces_alarm_get_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "alarm_type", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_get_value_alarm_type;
	}
	argument->alarm_type = integer;

	if (zw_avro_field_get_int(arg_value, "zwave_alarm_type", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_get_value_zwave_alarm_type;
	}
	argument->zwave_alarm_type = integer;

	if (zw_avro_field_get_int(arg_value, "zwave_alarm_event", &integer) != ZWP_STATUS_OK)
	{
		//optional parameter
		argument->zwave_alarm_event = COMMAND_ZWAVE_ALARM_EVENT_NONE;
	}
	else
	{
		argument->zwave_alarm_event = integer;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Vendor Alarm Type: %d", argument->alarm_type);
	ZWP_LOG(ZWP_LOG_DEBUG, "Z-Wave Alarm Type: %d", argument->zwave_alarm_type);
	ZWP_LOG(ZWP_LOG_DEBUG, "Z-Wave Alarm Event: %d", argument->zwave_alarm_event);

	if ((argument->alarm_type != COMMAND_ALARM_TYPE_LATEST)
			&& (argument->alarm_type != COMMAND_ALARM_TYPE_NONE)
			&& (zwu_number_is_uint8(argument->alarm_type) != ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid alarm type: %d", argument->alarm_type);
		goto l_err_invalid_alarm_type;
	}

	if ((argument->zwave_alarm_type != COMMAND_ZWAVE_ALARM_TYPE_NONE)
			&& (zwu_number_is_uint8(argument->zwave_alarm_type) != ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Z-Wave alarm type: %d", argument->zwave_alarm_type);
		goto l_err_invalid_zwave_alarm_type;
	}

	if ((argument->zwave_alarm_event != COMMAND_ZWAVE_ALARM_EVENT_NONE)
			&& (zwu_number_is_uint8(argument->zwave_alarm_event) != ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Z-Wave alarm event: %d", argument->zwave_alarm_event);
		goto l_err_invalid_zwave_alarm_event;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_invalid_zwave_alarm_type:
l_err_invalid_alarm_type:
l_err_get_value_zwave_alarm_type:
l_err_get_value_alarm_type:
l_err_invalid_zwave_alarm_event:
 	ZWP_FREE(argument);
l_err_args_alloc:
 	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Get command
 * @param[in]     args              Arguments passed with Get command
 * @return        Nothing
 * @see zwp_avi_interfaces_alarm_get_argument
 */
static void zwp_avi_interfaces_alarm_get_argument_free(void *args)
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
static zwp_status_t zwp_avi_interfaces_alarm_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_alarm_state_t *state;
	zwp_avi_interfaces_alarm_get_t *argument;

	argument = (zwp_avi_interfaces_alarm_get_t *)args;

	if ((state = (zwp_avi_interfaces_alarm_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->report.report_receive_condition;

	/*
	 * A Report is NOT considered the correct pair for a given Get, if
	 * The Report itself is not received (spurious wake up when using condition variable) or
	 * If alarm_type doesn't match or
	 * If zwave_alarm_type doesn't match
	 *
	 * Do not aim for match with vendor alarm type,
	 *       when alarm_type is not provided as input parameter.
	 * Do not aim for match with zwave alarm type,
	 *       when zwave_alarm_type is not provided as input parameter or
	 *       when zwave_alarm_type is ZW_ALRM_FIRST when report of any type is expected
	 * Do not aim for match with zwave alarm event,
	 *       when zwave_alarm_event is not provided as input parameter (v2 interface)
	 *
	 * There are some devices that when sent with specific zwave type and event, the response will be
	 * status 0xFE with no zwave type and event. HCAPI will fill in 0 for that matter.
	 * Some device will return the same zwave type and event 0.
	 * All these cares will result in a mismatch for zwave type and event. Therefore we disable the
	 * matching test for zwave type and event.
	 */
	/*** Angela 20 May 2015
	* When controller issues alarm get with specific zwave alarm type and event, the device response could be:
	* a. status 0xFE, no zwave alarm type and event (HCAPI will fill in 0)
	* b. same zwave alarm type but event 0, or same zwave alarm type but event 0xFE
	* c. same zwave alarm type and same event
	*
	* Therefore, the matching mechanism is implemented as following:
	* 1. sort the report alarm value list by the time recevied.
	* 2. When a match (receive_assition function and get_reuslt function) need to be done, match shall start from the last/latest
	*    entry in the list. 
	* 3. Match will check for all 3 conditions. Condition a will be treated as a wild card which will match any 
	*    Alarm get arguments.
	* 4. Once a match is found, the entry will be returned, regardless whether there is other (closer but older value) match in the list.
	*
	* For Alarm Get with 0xFF, always return the last/latest value in the list.
	****/
	if (zwp_interface_report_receive_assertion(state->report.report_receive_state) == ZWP_STATUS_OK)
	{
		if(argument->alarm_type != COMMAND_ALARM_TYPE_NONE)
		{
			if((state->report.report_receive_alarm_type != argument->alarm_type))
			{
				goto l_err_report_not_received;
			}
		}

		if((argument->zwave_alarm_type != COMMAND_ZWAVE_ALARM_TYPE_NONE) && (argument->zwave_alarm_type != ZW_ALRM_FIRST))
		{
			if((state->report.report_receive_zwave_alarm_type != argument->zwave_alarm_type) &&
				(state->report.report_receive_zwave_alarm_type != 0))
			{
				goto l_err_report_not_received;
			}
		}

		if((argument->zwave_alarm_event != COMMAND_ZWAVE_ALARM_TYPE_NONE))
		{
			if((state->report.report_receive_zwave_alarm_event != argument->zwave_alarm_event) &&
				(state->report.report_receive_zwave_alarm_event != 0) &&
				(state->report.report_receive_zwave_alarm_event != 0xFE) )
			{
				goto l_err_report_not_received;
			}
		}

		return ZWP_STATUS_OK;
	}

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
static zwp_status_t zwp_avi_interfaces_alarm_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_alarm_state_t *state;
	zwp_avi_interfaces_alarm_get_t *argument;
	uint8_t alarm_type;
	uint8_t zwave_alarm_type;
	uint8_t zwave_alarm_event;

	argument = (zwp_avi_interfaces_alarm_get_t *)args;

	if ((state = (zwp_avi_interfaces_alarm_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->report.report_receive_state);

	if (argument->alarm_type == COMMAND_ALARM_TYPE_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Missing one or more required arguments");
		goto l_err_invalid_argument;
	}

	if (argument->alarm_type == COMMAND_ALARM_TYPE_LATEST)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid alarm type");
		goto l_err_invalid_alarm_type;
	}

	alarm_type = (uint8_t)argument->alarm_type;

	// Optional parameter: When zwave_alarm_type is not specified, default to 255 (ZW_ALRM_FIRST)
	zwave_alarm_type = (argument->zwave_alarm_type == COMMAND_ZWAVE_ALARM_TYPE_NONE)
			? ZW_ALRM_FIRST : (uint8_t)argument->zwave_alarm_type;

	// Optional parameter: When zwave_alarm_event is not specified, default to 0 (not used)
	zwave_alarm_event = (argument->zwave_alarm_event == COMMAND_ZWAVE_ALARM_EVENT_NONE) ?
			COMMAND_ZWAVE_ALARM_EVENT_NOT_USED : (uint8_t)argument->zwave_alarm_event;

	*hcapi_code_p = zwif_alrm_get(desc_interface, alarm_type, zwave_alarm_type, zwave_alarm_event, ZWIF_GET_BMSK_LIVE);

	// Note down that a request is made for the 'first' alarm/notification
	if ((argument->zwave_alarm_type == ZW_ALRM_FIRST)
			&& ((*hcapi_code_p == ZW_ERR_NONE) || (*hcapi_code_p == ZW_ERR_QUEUED)))
	{
		state->report.wait_for_first_alarm = 1;
	}

	return ZWP_STATUS_OK;

l_err_invalid_alarm_type:
l_err_invalid_argument:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Allocate Result object corresponding to Report command
 * @param[out]    report_dest       Destination Report
 * @param[in]     report_src        Source Report
 * @param[in]     alarm_type        Alarm type
 * @param[in]     zwave_alarm_type  Z-Wave Alarm type
 * @param[in]     zwave_alarm_event Z-Wave Alarm event
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_alarm_get_result_alloc(zwp_avi_interfaces_alarm_report_t *report_dest,
		zwp_avi_interfaces_alarm_report_t *report_src, int alarm_type, int zwave_alarm_type, int zwave_alarm_event)
{
	int alarm_value_count;
	int i, bRetSingle = 0;
	zwp_avi_interfaces_alarm_report_alarm_value_t *alarm_value_dest;
	zwp_avi_interfaces_alarm_report_alarm_value_t *alarm_value_src;
	/*** Angela 20 May 2015
	* When controller issues alarm get with specific zwave alarm type and event, the device response could be:
	* a. status 0xFE, no zwave alarm type and event (HCAPI will fill in 0)
	* b. same zwave alarm type but event 0, or same zwave alarm type but event 0xFE
	* c. same zwave alarm type and same event
	*
	* Therefore, the matching mechanism is implemented as following:
	* 1. sort the report alarm value list by the time recevied.
	* 2. When a match (receive_assition function and get_reuslt function) need to be done, match shall start from the last/latest
	*    entry in the list. 
	* 3. Match will check for all 3 conditions. Condition a will be treated as a wild card which will match any 
	*    Alarm get arguments.
	* 4. Once a match is found, the entry will be returned, regardless whether there is other (closer but older value) match in the list.
	*    (it could change to return all the match reports in the list)
	*
	* For Alarm Get with 0xFF, always return the last/latest value in the list.
	****/

	alarm_value_count = zwu_array_list_size(report_src->alarm_value_list);

	for (i = alarm_value_count - 1; i >= 0; i--) //start from the end of list
	{
		alarm_value_src = (zwp_avi_interfaces_alarm_report_alarm_value_t *)zwu_array_list_get(report_src->alarm_value_list, i);

		if (alarm_type == COMMAND_ALARM_TYPE_LATEST || zwave_alarm_type == ZW_ALRM_FIRST)
		{
			//Found
			bRetSingle = 1; //Only return the last report
			goto l_label_found;
		}

		if(alarm_type != COMMAND_ALARM_TYPE_NONE)
		{
			if(alarm_value_src->value.type != alarm_type)
			{
				continue;
			}
		}

		if(zwave_alarm_type != COMMAND_ZWAVE_ALARM_TYPE_NONE)
		{
			if(alarm_value_src->value.ex_info != 1)
				continue;
			
			if((alarm_value_src->value.ex_type != zwave_alarm_type) && (alarm_value_src->value.ex_type != 0))
				continue;
		}

		if(zwave_alarm_event != COMMAND_ZWAVE_ALARM_EVENT_NONE)
		{
			if(alarm_value_src->value.ex_info != 1)
				continue;
			
			if((alarm_value_src->value.ex_event != zwave_alarm_event) && 
				(alarm_value_src->value.ex_event != 0) && 
				(alarm_value_src->value.ex_event != 0xFE))
				continue;

		}
		
l_label_found:
		if ((alarm_value_dest = (zwp_avi_interfaces_alarm_report_alarm_value_t *)ZWP_CALLOC(1, sizeof(*alarm_value_dest))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		if (zwu_array_list_add(report_dest->alarm_value_list, alarm_value_dest) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}

		memcpy(alarm_value_dest, alarm_value_src, sizeof(*alarm_value_dest));

		alarm_value_dest->event_parameter = NULL;

		if (alarm_value_dest->value.ex_evt_len > 0)
		{
			if ((alarm_value_dest->event_parameter = ZWP_CALLOC(1, alarm_value_dest->value.ex_evt_len)) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				goto l_err_event_parameter_alloc;
			}

			memcpy(alarm_value_dest->event_parameter, alarm_value_src->event_parameter, alarm_value_dest->value.ex_evt_len);
		}

		if(bRetSingle)
			break;
	}

	return ZWP_STATUS_OK;

	ZWP_FREE(alarm_value_dest->event_parameter);
l_err_event_parameter_alloc:
	zwu_array_list_remove(report_dest->alarm_value_list, alarm_value_dest);
l_err_value_list_add:
	ZWP_FREE(alarm_value_dest);
l_err_value_alloc:
	for (i--; i >= 0; i--)
	{
		ZWP_FREE(zwu_array_list_get(report_dest->alarm_value_list, i));
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
 * The caller should call zwp_avi_interfaces_alarm_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_alarm_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_alarm_state_t *state;
	zwp_avi_interfaces_alarm_report_t *report;
	zwp_avi_interfaces_alarm_get_t *argument;

	argument = (zwp_avi_interfaces_alarm_get_t *)args;

	if ((state = (zwp_avi_interfaces_alarm_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((report = (zwp_avi_interfaces_alarm_report_t *)ZWP_CALLOC(1, sizeof(*report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	if (zwu_array_list_create(0, &report->alarm_value_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	if (zwp_avi_interfaces_alarm_get_result_alloc(report, &state->report, argument->alarm_type, argument->zwave_alarm_type, argument->zwave_alarm_event) != ZWP_STATUS_OK)
	{
		goto l_err_result_alloc;
	}

	*result_p = report;

	return ZWP_STATUS_OK;

l_err_result_alloc:
	zwu_array_list_destroy(report->alarm_value_list);
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
static zwp_status_t zwp_avi_interfaces_alarm_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_alarm_report_t *report;
	avro_value_t alarm_value_list_value;
	avro_value_t alarm_value_value;
	zwp_avi_interfaces_alarm_report_alarm_value_t *alarm_value;
	int alarm_value_count;
	int i;
	avro_value_t extended_alarm_info_field;
	avro_value_t extended_alarm_info_value;

	report = (zwp_avi_interfaces_alarm_report_t *)result;

	if (avro_value_get_by_name(arg_value, "alarm_value_list", &alarm_value_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'alarm_value_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	alarm_value_count = zwu_array_list_size(report->alarm_value_list);

	for (i = 0; i < alarm_value_count; i++)
	{
		if (avro_value_append(&alarm_value_list_value, &alarm_value_value, 0) != 0)
		{
			goto l_err_append_alarm_value_value;
		}

		alarm_value = zwu_array_list_get(report->alarm_value_list, i);

		if ((zw_avro_field_set_long(&alarm_value_value, "utime", alarm_value->utime) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&alarm_value_value, "alarm_type", alarm_value->value.type) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&alarm_value_value, "alarm_level", alarm_value->value.level) != ZWP_STATUS_OK)
				)
		{
			goto l_err_set_alarm_value_value;
		}

		/*
		 * Items to be filled when extended info is valid
		 */
		if (avro_value_get_by_name(&alarm_value_value, "extended_alarm_info", &extended_alarm_info_field, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'extended_alarm_info' field from 'AlarmReport' record: %s", avro_strerror());
			goto l_err_get_extended_alarm_info_field;
		}

		if (alarm_value->value.ex_info)
		{
			if (avro_value_set_branch(&extended_alarm_info_field, 0, &extended_alarm_info_value) != 0)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to set 'extended_alarm_info' field to branch '0': %s", avro_strerror());
				goto l_err_set_extended_alarm_info_field;
			}

			if ((zw_avro_field_set_int(&extended_alarm_info_value, "zensor_net_source_node_id", alarm_value->value.ex_zensr_nodeid) != ZWP_STATUS_OK)
					|| (zw_avro_field_set_int(&extended_alarm_info_value, "zwave_alarm_status", alarm_value->value.ex_status) != ZWP_STATUS_OK)
					|| (zw_avro_field_set_int(&extended_alarm_info_value, "zwave_alarm_type", alarm_value->value.ex_type) != ZWP_STATUS_OK)
					|| (zw_avro_field_set_int(&extended_alarm_info_value, "zwave_alarm_event", alarm_value->value.ex_event) != ZWP_STATUS_OK)
					|| (zw_avro_field_set_int(&extended_alarm_info_value, "event_parameter_type", alarm_value->value.ex_evt_type) != ZWP_STATUS_OK)
					|| (zw_avro_field_set_bytes(&extended_alarm_info_value, "event_parameter", alarm_value->event_parameter,
							alarm_value->value.ex_evt_len) != ZWP_STATUS_OK)
					)
			{
				goto l_err_set_extended_alarm_info_value;
			}
		}
		else
		{
			if (avro_value_set_branch(&extended_alarm_info_field, 1, NULL) != 0)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to set 'extended_alarm_info' field to branch '1': %s", avro_strerror());
				goto l_err_set_extended_alarm_info_field;
			}
		}
	}

	return ZWP_STATUS_OK;

l_err_set_extended_alarm_info_value:
l_err_set_extended_alarm_info_field:
l_err_get_extended_alarm_info_field:
l_err_set_alarm_value_value:
l_err_append_alarm_value_value:
l_err_get_list_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_alarm_get_execute_passive
 */
static void zwp_avi_interfaces_alarm_get_result_free(void *result)
{
	zwp_avi_interfaces_alarm_report_t *report;
	int alarm_value_count;
	int i;
	zwp_avi_interfaces_alarm_report_alarm_value_t *alarm_value;

	report = (zwp_avi_interfaces_alarm_report_t *)result;

	alarm_value_count = zwu_array_list_size(report->alarm_value_list);

	for (i = 0; i < alarm_value_count; i++)
	{
		alarm_value = zwu_array_list_get(report->alarm_value_list, i);
		ZWP_FREE(alarm_value->event_parameter);
		ZWP_FREE(alarm_value);
	}
	zwu_array_list_destroy(report->alarm_value_list);

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
 * The caller should call zwp_avi_interfaces_alarm_set_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_alarm_set_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_alarm_set_t *argument;
	int32_t zwave_alarm_type;
	int32_t zwave_alarm_status;

	if ((argument = (zwp_avi_interfaces_alarm_set_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if ((zw_avro_field_get_int(arg_value, "zwave_alarm_type", &zwave_alarm_type) != ZWP_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "zwave_alarm_status", &zwave_alarm_status) != ZWP_STATUS_OK)
			)
	{
		goto l_err_get_value;
	}

	argument->zwave_alarm_type = zwave_alarm_type;
	argument->zwave_alarm_status = zwave_alarm_status;

	ZWP_LOG(ZWP_LOG_DEBUG, "Z-Wave Alarm Type: %d", argument->zwave_alarm_type);
	ZWP_LOG(ZWP_LOG_DEBUG, "Z-Wave Alarm Status: %d", argument->zwave_alarm_status);

	if (zwu_number_is_uint8(argument->zwave_alarm_type) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Z-Wave alarm type: %d", argument->zwave_alarm_type);
		goto l_err_invalid_zwave_alarm_type;
	}

	if (zwu_number_is_uint8(argument->zwave_alarm_status) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Z-Wave alarm status: %d", argument->zwave_alarm_status);
		goto l_err_invalid_zwave_alarm_status;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_invalid_zwave_alarm_status:
l_err_invalid_zwave_alarm_type:
l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Set command
 * @param[in]     args              Arguments passed with Set command
 * @return        Nothing
 * @see zwp_avi_interfaces_alarm_set_argument
 */
static void zwp_avi_interfaces_alarm_set_argument_free(void *args)
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
static zwp_status_t zwp_avi_interfaces_alarm_set_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_alarm_set_t *argument;

	argument = (zwp_avi_interfaces_alarm_set_t *)args;

	*hcapi_code_p = zwif_alrm_set(desc_interface,
			(uint8_t)argument->zwave_alarm_type, (uint8_t)argument->zwave_alarm_status);

	return ZWP_STATUS_OK;
}

/**
 * @brief HCAPI callback for Supported Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  have_vtype        flag to indicate whether vendor specific alarm type supported. 1=supported; else 0=unsupported
 * @param[in]	  ztype_len         size of ztype buffer
 * @param[in]	  ztype             buffer to store supported Z-wave alarm types (ZW_ALRM_XXX)
 */
void zwp_avi_interfaces_alarm_type_supported_report_handler(zwifd_p ifd, uint8_t have_vtype, uint8_t ztype_len, uint8_t *ztype, int valid)
{
	zwp_avi_interfaces_alarm_state_t *state;
	zwifd_p desc_interface;
	uint8_t *tmp;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_alarm_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (valid)
	{
		state->type_supported_report.utime = zwu_time_now();
		state->type_supported_report.have_vendor_type = have_vtype;

		if (ztype_len > state->type_supported_report.supported_types_count)
		{
			if ((tmp = (uint8_t *)ZWP_REALLOC(state->type_supported_report.supported_types, sizeof(*tmp) * ztype_len)) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				goto l_err_type_list_alloc;
			}

			state->type_supported_report.supported_types = tmp;
		}
		state->type_supported_report.supported_types_count = ztype_len;

		memcpy(state->type_supported_report.supported_types, ztype, state->type_supported_report.supported_types_count);
	}
	zwp_interface_report_receive_done(&state->type_supported_report.report_receive_state, state->type_supported_report.report_receive_condition);

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
static zwp_status_t zwp_avi_interfaces_alarm_type_supported_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_alarm_state_t *state;

	if ((state = (zwp_avi_interfaces_alarm_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->type_supported_report.report_receive_condition;

	if (zwp_interface_report_receive_assertion(state->type_supported_report.report_receive_state) != ZWP_STATUS_OK)
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
static zwp_status_t zwp_avi_interfaces_alarm_type_supported_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_alarm_state_t *state;

	if ((state = (zwp_avi_interfaces_alarm_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->type_supported_report.report_receive_state);

	*hcapi_code_p = zwif_alrm_sup_get(desc_interface, zwp_avi_interfaces_alarm_type_supported_report_handler, 0);

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
 * The caller should call zwp_avi_interfaces_alarm_type_supported_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_alarm_type_supported_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_alarm_state_t *state;
	zwp_avi_interfaces_alarm_type_supported_report_t *type_supported_report;

	if ((state = (zwp_avi_interfaces_alarm_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((type_supported_report = (zwp_avi_interfaces_alarm_type_supported_report_t *)ZWP_CALLOC(1, sizeof(*type_supported_report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	memcpy(type_supported_report, &state->type_supported_report, sizeof(*type_supported_report));

	if ((type_supported_report->supported_types = (uint8_t *)ZWP_CALLOC(1,
			sizeof(*type_supported_report->supported_types) * state->type_supported_report.supported_types_count)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_type_list_alloc;
	}

	memcpy(type_supported_report->supported_types, state->type_supported_report.supported_types, state->type_supported_report.supported_types_count);

	*result_p = type_supported_report;

	return ZWP_STATUS_OK;

	ZWP_FREE(type_supported_report->supported_types);
l_err_type_list_alloc:
	ZWP_FREE(type_supported_report);
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
static zwp_status_t zwp_avi_interfaces_alarm_type_supported_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_alarm_type_supported_report_t *type_supported_report;
	avro_value_t zwave_alarm_type_list_value;
	avro_value_t zwave_alarm_type_value;
	int i;

	type_supported_report = (zwp_avi_interfaces_alarm_type_supported_report_t *)result;

	if ((zw_avro_field_set_long(arg_value, "utime", type_supported_report->utime) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "have_vendor_type", type_supported_report->have_vendor_type) != ZWP_STATUS_OK))
	{
		goto l_err_set_report_value;
	}

	if (avro_value_get_by_name(arg_value, "supported_types", &zwave_alarm_type_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'supported_types': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	for (i = 0; i < type_supported_report->supported_types_count; i++)
	{
		if (avro_value_append(&zwave_alarm_type_list_value, &zwave_alarm_type_value, 0) != 0)
		{
			goto l_err_append_zwave_alarm_type_value;
		}

		if (avro_value_set_int(&zwave_alarm_type_value, type_supported_report->supported_types[i]) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to set zwave alarm type '%d': %s", type_supported_report->supported_types[i], avro_strerror());
			goto l_err_set_zwave_alarm_type;
		}
	}

	return ZWP_STATUS_OK;

l_err_set_zwave_alarm_type:
l_err_append_zwave_alarm_type_value:
l_err_get_list_value:
l_err_set_report_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Supported Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_alarm_type_supported_get_execute_passive
 */
static void zwp_avi_interfaces_alarm_type_supported_get_result_free(void *result)
{
	zwp_avi_interfaces_alarm_type_supported_report_t *type_supported_report;

	type_supported_report = (zwp_avi_interfaces_alarm_type_supported_report_t *)result;

	ZWP_FREE(type_supported_report->supported_types);

	ZWP_FREE(type_supported_report);
}

/**
 * @brief HCAPI callback for Event Supported Report command
 * @param[in]	  ifd	          interface
 * @param[in]	  ztype			  Z-wave alarm type (ZW_ALRM_XXX)
 * @param[in]	  evt_len         size of evt buffer
 * @param[in]	  evt             buffer to store supported event of the alarm type specified in ztype
 */
void zwp_avi_interfaces_alarm_event_supported_report_handler(zwifd_p ifd, uint8_t ztype, uint8_t evt_len, uint8_t *evt, int valid)
{
	zwp_avi_interfaces_alarm_state_t *state;
	zwifd_p desc_interface;
	int event_supported_list_count, i;
	zwp_avi_interfaces_alarm_single_event_supported_report_t *palarm_events_per_type;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_alarm_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}
	ZWP_LOG(ZWP_LOG_DEBUG, "ztype %d evt_len %d  evt %d", ztype, evt_len, *evt);

	if (valid)
	{
		event_supported_list_count = zwu_array_list_size(state->type_event_supported_report.event_supported_list);

		ZWP_LOG(ZWP_LOG_DEBUG, "event_supported_list_count %d", event_supported_list_count);

		for (i = 0; i < event_supported_list_count; i++)
		{
			palarm_events_per_type = (zwp_avi_interfaces_alarm_single_event_supported_report_t *)zwu_array_list_get(state->type_event_supported_report.event_supported_list, i);

			if (palarm_events_per_type->report_receive_zwave_alarm_type == ztype)
			{
				//match found, delete the current data
				palarm_events_per_type->supported_events_count = 0;
				if (palarm_events_per_type->supported_events)
				{
					ZWP_FREE(palarm_events_per_type->supported_events);
					palarm_events_per_type->supported_events = NULL;
					break;
				}
			}
		}

		if (i >= event_supported_list_count)
		{
			//Cannot find a match
			if ((palarm_events_per_type = (zwp_avi_interfaces_alarm_single_event_supported_report_t *)ZWP_CALLOC(1, sizeof(zwp_avi_interfaces_alarm_single_event_supported_report_t))) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				goto l_err_event_struct_alloc;
			}

			if (zwu_array_list_add(state->type_event_supported_report.event_supported_list, palarm_events_per_type) != ZWP_STATUS_OK)
			{
				goto l_err_event_list_add;
			}
		}

		palarm_events_per_type->utime = zwu_time_now();

		palarm_events_per_type->supported_events_count = evt_len;
		palarm_events_per_type->report_receive_zwave_alarm_type = ztype;

		if (evt_len > 0)
		{
			if ((palarm_events_per_type->supported_events = (uint8_t *)ZWP_CALLOC(1, sizeof(uint8_t) * evt_len)) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				goto l_err_event_list_alloc;
			}

			memcpy(palarm_events_per_type->supported_events, evt, evt_len);
		}
	}
	zwp_interface_report_receive_done(&state->type_event_supported_report.report_receive_state, state->type_event_supported_report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

	ZWP_FREE(palarm_events_per_type->supported_events);
l_err_event_list_alloc:
	// Remove from list only if a new alarm_value
	if (i >= event_supported_list_count)
	{
		zwu_array_list_remove(state->type_event_supported_report.event_supported_list, palarm_events_per_type);
	}
l_err_event_list_add:
	// Free only if a new alarm_value
	if (i >= event_supported_list_count)
	{
		ZWP_FREE(palarm_events_per_type);
	}
l_err_event_struct_alloc:
l_err_get_state:
	zwp_net_release_desc_interface(ifd);
l_err_acquire_desc:

	return;
}

/**
 * @brief Get argument for Event Supported Get command from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Event supported Get command
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_alarm_event_supported_get_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_alarm_event_supported_get_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_alarm_event_supported_get_t *argument;
	int integer;

	if ((argument = (zwp_avi_interfaces_alarm_event_supported_get_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "zwave_alarm_type", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_get_value_zwave_alarm_type;
	}
	argument->zwave_alarm_type = integer;

	ZWP_LOG(ZWP_LOG_DEBUG, "Z-Wave Alarm Type: %d", argument->zwave_alarm_type);

	if ((argument->zwave_alarm_type != COMMAND_ZWAVE_ALARM_TYPE_NONE)
			&& (zwu_number_is_uint8(argument->zwave_alarm_type) != ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Z-Wave alarm type: %d", argument->zwave_alarm_type);
		goto l_err_invalid_zwave_alarm_type;
	}
	
	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_invalid_zwave_alarm_type:
l_err_get_value_zwave_alarm_type:
 	ZWP_FREE(argument);
l_err_args_alloc:
 	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Event Supported Get command
 * @param[in]     args              Arguments passed with Event Supported Get command
 * @return        Nothing
 * @see zwp_avi_interfaces_alarm_event_supported_get_argument
 */
static void zwp_avi_interfaces_alarm_event_supported_get_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
 * @brief Test if the expected Event Supported Report command is received
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Get command
 * @param[out]    report_receive_condition_p    Report command receive condition
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_alarm_event_supported_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_alarm_state_t *state;

	if ((state = (zwp_avi_interfaces_alarm_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->type_event_supported_report.report_receive_condition;

	if (zwp_interface_report_receive_assertion(state->type_event_supported_report.report_receive_state) != ZWP_STATUS_OK)
	{
		goto l_err_report_not_received;
	}

	return ZWP_STATUS_OK;

l_err_report_not_received:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute active Event Supported Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Event Supported Get command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_alarm_event_supported_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_alarm_state_t *state;
	zwp_avi_interfaces_alarm_event_supported_get_t *argument;
	uint8_t zwave_alarm_type;

	argument = (zwp_avi_interfaces_alarm_event_supported_get_t *)args;

	if ((state = (zwp_avi_interfaces_alarm_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (argument->zwave_alarm_type == COMMAND_ALARM_TYPE_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Missing required arguments");
		goto l_err_invalid_argument;
	}
	zwave_alarm_type = (uint8_t)argument->zwave_alarm_type;

	zwp_interface_report_receive_init(&state->type_event_supported_report.report_receive_state);

	ZWP_LOG(ZWP_LOG_DEBUG, "calling zwif_alrm_sup_evt_get");

	*hcapi_code_p = zwif_alrm_sup_evt_get(desc_interface, zwave_alarm_type, zwp_avi_interfaces_alarm_event_supported_report_handler, 0);

	ZWP_LOG(ZWP_LOG_DEBUG, "hcapi_code_p is %d", *hcapi_code_p);

	return ZWP_STATUS_OK;

l_err_invalid_argument:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute passive Event Supported Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Event Supported Get command
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_alarm_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_alarm_event_supported_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_alarm_state_t *state;
	zwp_avi_interfaces_alarm_single_event_supported_report_t *report;
	zwp_avi_interfaces_alarm_single_event_supported_report_t *palarm_events_per_type = NULL;
	zwp_avi_interfaces_alarm_event_supported_get_t *argument;
	int i, event_supported_list_count;

	argument = (zwp_avi_interfaces_alarm_event_supported_get_t *)args;

	if ((state = (zwp_avi_interfaces_alarm_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((report = (zwp_avi_interfaces_alarm_single_event_supported_report_t *)ZWP_CALLOC(1, sizeof(*report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	event_supported_list_count = zwu_array_list_size(state->type_event_supported_report.event_supported_list);

	for (i = 0; i < event_supported_list_count; i++)
	{
		palarm_events_per_type = (zwp_avi_interfaces_alarm_single_event_supported_report_t *)zwu_array_list_get(state->type_event_supported_report.event_supported_list, i);

		if(palarm_events_per_type->report_receive_zwave_alarm_type == argument->zwave_alarm_type)
		{
			//match found
			memcpy(report, palarm_events_per_type, sizeof(zwp_avi_interfaces_alarm_single_event_supported_report_t));
			if(palarm_events_per_type->supported_events)
			{
				if ((report->supported_events = (uint8_t *)ZWP_CALLOC(1, sizeof(uint8_t) * palarm_events_per_type->supported_events_count)) == NULL)
				{
					ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
					goto l_err_report_supported_event_alloc;
				}

				memcpy(report->supported_events, palarm_events_per_type->supported_events, palarm_events_per_type->supported_events_count);
				break;
			}
		}
	}

	*result_p = report;

	return ZWP_STATUS_OK;

	ZWP_FREE(report->supported_events);
l_err_report_supported_event_alloc:
	ZWP_FREE(report);
l_err_report_alloc:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to Event Supported Report command
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_alarm_event_supported_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_alarm_single_event_supported_report_t *event_supported_report;
	avro_value_t zwave_alarm_event_list_value;
	avro_value_t zwave_alarm_event_value;
	int i;

	event_supported_report = (zwp_avi_interfaces_alarm_single_event_supported_report_t *)result;

	if ((zw_avro_field_set_long(arg_value, "utime", event_supported_report->utime) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "zwave_alarm_type", event_supported_report->report_receive_zwave_alarm_type) != ZWP_STATUS_OK))
	{
		goto l_err_set_report_value;
	}

	if (avro_value_get_by_name(arg_value, "supported_events", &zwave_alarm_event_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'supported_events': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	for (i = 0; i < event_supported_report->supported_events_count; i++)
	{
		if (avro_value_append(&zwave_alarm_event_list_value, &zwave_alarm_event_value, 0) != 0)
		{
			goto l_err_append_zwave_alarm_event_value;
		}

		if (avro_value_set_int(&zwave_alarm_event_value, event_supported_report->supported_events[i]) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to set zwave alarm event '%d': %s", event_supported_report->supported_events[i], avro_strerror());
			goto l_err_set_zwave_alarm_event;
		}
	}

	return ZWP_STATUS_OK;

l_err_set_zwave_alarm_event:
l_err_append_zwave_alarm_event_value:
l_err_get_list_value:
l_err_set_report_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Event Supported Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_alarm_event_supported_get_execute_passive
 */
static void zwp_avi_interfaces_alarm_event_supported_get_result_free(void *result)
{
	zwp_avi_interfaces_alarm_single_event_supported_report_t *event_supported_report;

	event_supported_report = (zwp_avi_interfaces_alarm_single_event_supported_report_t *)result;

	ZWP_FREE(event_supported_report->supported_events);

	ZWP_FREE(event_supported_report);
}

/** Handlers for Get command */
static zwp_avi_message_interfaces_t interfaces_alarm_get =
{
		"interface_alarm_get",
		COMMAND_CLASS_ALARM, zwp_avi_interfaces_alarm_setup,
		zwp_avi_interfaces_alarm_state_alloc, zwp_avi_interfaces_alarm_state_free,
		zwp_avi_interfaces_alarm_get_argument, zwp_avi_interfaces_alarm_get_argument_free,
		zwp_avi_interfaces_alarm_get_report_receive_assertion,
		zwp_avi_interfaces_alarm_get_execute_active, zwp_avi_interfaces_alarm_get_execute_passive,
		zwp_avi_interfaces_alarm_get_result, zwp_avi_interfaces_alarm_get_result_free
};

/** Handlers for Set command */
static zwp_avi_message_interfaces_t interfaces_alarm_set =
{
		"interface_alarm_set",
		COMMAND_CLASS_ALARM, NULL,
		NULL, NULL,
		zwp_avi_interfaces_alarm_set_argument, zwp_avi_interfaces_alarm_set_argument_free,
		NULL,
		zwp_avi_interfaces_alarm_set_execute_active, NULL,
		NULL, NULL
};

/** Handlers for Type Supported Get command */
static zwp_avi_message_interfaces_t interfaces_alarm_type_supported_get =
{
		"interface_alarm_type_supported_get",
		COMMAND_CLASS_ALARM, NULL,
		NULL, NULL,
		NULL, NULL,
		zwp_avi_interfaces_alarm_type_supported_get_report_receive_assertion,
		zwp_avi_interfaces_alarm_type_supported_get_execute_active, zwp_avi_interfaces_alarm_type_supported_get_execute_passive,
		zwp_avi_interfaces_alarm_type_supported_get_result, zwp_avi_interfaces_alarm_type_supported_get_result_free
};

/** Handlers for Event Supported Get command */
static zwp_avi_message_interfaces_t interfaces_alarm_event_supported_get =
{
		"interface_alarm_event_supported_get",
		COMMAND_CLASS_ALARM, NULL,
		NULL, NULL,
		zwp_avi_interfaces_alarm_event_supported_get_argument, zwp_avi_interfaces_alarm_event_supported_get_argument_free,
		//NULL,
		zwp_avi_interfaces_alarm_event_supported_get_report_receive_assertion,
		zwp_avi_interfaces_alarm_event_supported_get_execute_active, zwp_avi_interfaces_alarm_event_supported_get_execute_passive,
		zwp_avi_interfaces_alarm_event_supported_get_result, zwp_avi_interfaces_alarm_event_supported_get_result_free
};

/** Register command handlers */
zwp_status_t zwp_avi_interfaces_alarm_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_alarm_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_alarm_set) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_alarm_type_supported_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_alarm_event_supported_get) != ZWP_STATUS_OK))
	{
		goto l_err_interface_list_get;
	}

	return ZWP_STATUS_OK;

l_err_interface_list_get:
	return ZWP_STATUS_ERROR;
}




