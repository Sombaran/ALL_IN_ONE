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
 * @file    zwp_avi_ifttt_alarm.c
 *
 * @brief   AVI message 'ifttt_alarm'
 * @details Handles the AVI message 'ifttt_alarm'.
 *
 * @author  Angela Yan
 *
 * @version 1.0 - 2017-01-09
 * - Initial version
 */

#include "zwp_status.h"
#include "zwp_avi_message_ifttt.h"
#include "zwp_ifttt_common.h"
#include "zwp_avi_ifttt_common.h"
#include "zwp_log.h"
#include "zwp_memory.h"
#include "zwp_error.h"
#include "zwp_network.h"
#include "zwp_descriptor.h"
#include "zwp_avi_ifttt_db_common.h"
#include "zwp_avi_interfaces_alarm.h"

 /** alarm option argument */
typedef struct {
	int					category;
	eifttt_fields		efield;
} zwp_avi_ifttt_alarm_option_t;


/**
* @brief Allocate interface level data for IFTTT
* @param[in]	 desc_interface    Interface descriptor
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zwp_avi_interfaces_alarm_state_free_ifttt() to free resources
*/
zwp_status_t zwp_avi_interfaces_alarm_state_alloc_ifttt(zwifd_p desc_interface)
{
	zwp_avi_interfaces_alarm_state_t *state;
	pzwp_db_table	pifttt_generic_trigger_table;
	pzwp_db_table	pifttt_alarm_table;

	if ((state = (zwp_avi_interfaces_alarm_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((pifttt_generic_trigger_table = zwp_avi_ifttt_generic_trigger_table_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get generic table");
		goto l_err_get_generic_table;
	}

	if ((pifttt_alarm_table = zwp_avi_ifttt_cc_table_get(desc_interface, COMMAND_CLASS_ALARM)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get alarm table");
		goto l_err_get_cc_table;
	}

	state->pifttt_generic_trigger_table = pifttt_generic_trigger_table;
	state->pifttt_alarm_table = pifttt_alarm_table;
	state->ifttt_alarm_get_result.putimeList = NULL;

	return ZWP_STATUS_OK;

	zwp_db_close_table(state->pifttt_alarm_table);
	state->pifttt_alarm_table = NULL;
l_err_get_cc_table:
l_err_get_generic_table:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free interface level data
* @param[in]     state               Interface level data
* @return        Nothing
* @see zwp_avi_interfaces_alarm_state_alloc_ifttt
*/
void zwp_avi_interfaces_alarm_state_free_ifttt(zwp_avi_interfaces_alarm_state_t *state)
{
	//zwu_array_list_destroy(state->ifttt_get_result_list);

	if (state->ifttt_alarm_get_result.putimeList)
		ZWP_FREE(state->ifttt_alarm_get_result.putimeList);

	state->pifttt_generic_trigger_table = NULL;

	zwp_db_close_table(state->pifttt_alarm_table);
	state->pifttt_alarm_table = NULL;
}

 /**
 * @brief Get argument for alarm option operation from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Node Add operation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_networks_node_add_argument_free() to free resources
 */
static zwp_status_t zwp_avi_ifttt_alarm_option_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_ifttt_alarm_option_t *argument;
	int32_t ifield;

	if ((argument = (zwp_avi_ifttt_alarm_option_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "category", &argument->category) != ZWP_STATUS_OK)
	{
		goto l_err_get_bonoff;
	}											   
		
	if (zw_avro_field_get_int(arg_value, "efield", &ifield) != ZWP_STATUS_OK)
	{
		goto l_err_get_efield;
	}

	argument->efield = (eifttt_fields)ifield;

	*args_p = argument;							   
												   
	return ZWP_STATUS_OK;	

l_err_get_efield:
l_err_get_bonoff:
	ZWP_FREE(argument);
l_err_args_alloc:								   
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free arguments for alarm option operation
* @param[in]     args              Arguments passed with Remove Failed Node operation
* @return        Nothing
* @see zwp_avi_networks_failed_node_remove_argument
*/
static void zwp_avi_ifttt_alarm_option_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
 * @brief Execute call to alarm option operation status
 * @param[in]     net               Network object
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 *@note
 * @par
 * The caller should call zwp_avi_ifttt_alarm_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_ifttt_alarm_option_execute(zwp_net_t net, void *arg, void **result_p)
{
	//zwp_status_t res = ZWP_STATUS_ERROR;
	zwp_avi_ifttt_alarm_option_t *argument;
	zwp_desc_net_t desc_net;
	zwu_array_list_t if_ep_name_list;
	uint16_t interfaceid = COMMAND_CLASS_ALARM;
	zwp_desc_IF_ep_name_t  *pstIF_ep_name;
	int i;

	argument = (zwp_avi_ifttt_alarm_option_t *)arg;

	desc_net = zwp_net_get_desc_net(net);

	if (!desc_net)
		goto l_err_invalid_net_desc;

	if (argument->efield == eDevice_name)
	{
		if (zwp_desc_get_device_from_interface_id(desc_net, interfaceid, &if_ep_name_list) != ZWP_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed at zwp_desc_get_device_from_interface_id");
			goto l_err_get_if_ep_name_list;
		}

		ZWP_LOG(ZWP_LOG_DEBUG, "Found %d devices with class id: %d ", zwu_array_list_size(if_ep_name_list), COMMAND_CLASS_ALARM);


		//Check whether the thermostat mode device has support Mode or not
		//iSize = zwu_array_list_size(if_ep_name_list);

		for (i = 0; i < zwu_array_list_size(if_ep_name_list); i++)
		{
			pstIF_ep_name = zwu_array_list_get(if_ep_name_list, i);

			if (pstIF_ep_name)
			{
				if (!zwp_ifttt_interface_has_supported_alarm_type_event(pstIF_ep_name->desc_interface, argument->category))
				{
					//not found
					//Remove the device from array. readjust i value
					ZWP_FREE(pstIF_ep_name);
					zwu_array_list_remove_at_index(if_ep_name_list, i);
					i--;
				}
			}
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_ERR, "Unrecognized efield %d", argument->efield);
		goto l_err_invalid_efield;
	}

	*result_p = if_ep_name_list;

	return ZWP_STATUS_OK;

l_err_invalid_efield:
l_err_get_if_ep_name_list:
l_err_invalid_net_desc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to network operation state
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_ifttt_alarm_get_execute
 */
static void zwp_avi_ifttt_alarm_option_result_free(void *result)
{
	zwp_desc_free_if_ep_name_list(result);
}

/**
* @brief Get argument for alarm set operation from Avro value object
* @param[in]     arg_value         Avro value object
* @param[out]    args_p            Arguments passed with Node Add operation
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zwp_avi_networks_node_add_argument_free() to free resources
*/
static zwp_status_t zwp_avi_ifttt_alarm_get_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_ifttt_alarm_get_t *argument;
	int itrigger;

	if ((argument = (zwp_avi_ifttt_alarm_get_t *)ZWP_CALLOC(1, sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zwp_avi_ifttt_get_trigger_data_argument(arg_value, &argument->trigger_id_data) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Fail to get trigger data argument");
		goto l_err_get_trigger_data;
	}

	if (zw_avro_field_get_int(arg_value, "itrigger", &itrigger) != ZWP_STATUS_OK)
	{
		goto l_err_get_category;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "itrigger: %d", itrigger);

	if (zwu_number_is_uint8(itrigger) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid value: %d", itrigger);
		goto l_err_invalid_value;
	}

	argument->eTrigger = (eifttt_IF_feature_type)itrigger;

	switch (argument->eTrigger)
	{
		case eTrigger_motionAlarm_Detected:
		{
			argument->alarm_param[0].zwave_alarm_type = ZW_ALRM_BURGLAR;
			argument->alarm_param[0].zwave_alarm_event = ZW_ALRM_EVT_MOTION_DET_L;
			argument->alarm_param[0].zwave_alarm_event_param_len = -1;  //-1 means ignore param during comparison

			argument->alarm_param[1].zwave_alarm_type = ZW_ALRM_BURGLAR;
			argument->alarm_param[1].zwave_alarm_event = ZW_ALRM_EVT_MOTION_DET;
			argument->alarm_param[1].zwave_alarm_event_param_len = 0;

			argument->inofParam = 2;
		}
		break;

		case eTrigger_motionAlarm_Clear:
		{
			argument->alarm_param[0].zwave_alarm_type = ZW_ALRM_BURGLAR;
			argument->alarm_param[0].zwave_alarm_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
			argument->alarm_param[0].zwave_alarm_event_param_len = 1;
			argument->alarm_param[0].zwave_alarm_event_param[0] = ZW_ALRM_EVT_MOTION_DET_L;

			argument->alarm_param[1].zwave_alarm_type = ZW_ALRM_BURGLAR;
			argument->alarm_param[1].zwave_alarm_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
			argument->alarm_param[1].zwave_alarm_event_param_len = 1;
			argument->alarm_param[1].zwave_alarm_event_param[0] = ZW_ALRM_EVT_MOTION_DET;

			argument->inofParam = 2;
		}
		break;

		case eTrigger_smokeAlarm_Detected:
		{
			argument->alarm_param[0].zwave_alarm_type = ZW_ALRM_SMOKE;
			argument->alarm_param[0].zwave_alarm_event = ZW_ALRM_EVT_SMOKE_L;
			argument->alarm_param[0].zwave_alarm_event_param_len = -1;

			argument->alarm_param[1].zwave_alarm_type = ZW_ALRM_SMOKE;
			argument->alarm_param[1].zwave_alarm_event = ZW_ALRM_EVT_SMOKE;
			argument->alarm_param[1].zwave_alarm_event_param_len = 0;

			argument->inofParam = 2;
		}
		break;

		case eTrigger_smokeAlarm_Clear:
		{
			argument->alarm_param[0].zwave_alarm_type = ZW_ALRM_SMOKE;
			argument->alarm_param[0].zwave_alarm_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
			argument->alarm_param[0].zwave_alarm_event_param_len = 1;
			argument->alarm_param[0].zwave_alarm_event_param[0] = ZW_ALRM_EVT_SMOKE_L;

			argument->alarm_param[1].zwave_alarm_type = ZW_ALRM_SMOKE;
			argument->alarm_param[1].zwave_alarm_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
			argument->alarm_param[1].zwave_alarm_event_param_len = 1;
			argument->alarm_param[1].zwave_alarm_event_param[0] = ZW_ALRM_EVT_SMOKE;

			argument->inofParam = 2;
		}
		break;

		case eTrigger_coAlarm_Detected:
		{
			argument->alarm_param[0].zwave_alarm_type = ZW_ALRM_CO;
			argument->alarm_param[0].zwave_alarm_event = ZW_ALRM_EVT_CO_L;
			argument->alarm_param[0].zwave_alarm_event_param_len = -1;

			argument->alarm_param[1].zwave_alarm_type = ZW_ALRM_CO;
			argument->alarm_param[1].zwave_alarm_event = ZW_ALRM_EVT_CO;
			argument->alarm_param[1].zwave_alarm_event_param_len = 0;

			argument->inofParam = 2;
		}
		break;

		case eTrigger_coAlarm_Clear:
		{
			argument->alarm_param[0].zwave_alarm_type = ZW_ALRM_CO;
			argument->alarm_param[0].zwave_alarm_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
			argument->alarm_param[0].zwave_alarm_event_param_len = 1;
			argument->alarm_param[0].zwave_alarm_event_param[0] = ZW_ALRM_EVT_CO_L;

			argument->alarm_param[1].zwave_alarm_type = ZW_ALRM_CO;
			argument->alarm_param[1].zwave_alarm_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
			argument->alarm_param[1].zwave_alarm_event_param_len = 1;
			argument->alarm_param[1].zwave_alarm_event_param[0] = ZW_ALRM_EVT_CO;

			argument->inofParam = 2;
		}
		break;

		case eTrigger_co2Alarm_Detected:
		{
			argument->alarm_param[0].zwave_alarm_type = ZW_ALRM_CO2;
			argument->alarm_param[0].zwave_alarm_event = ZW_ALRM_EVT_CO2_L;
			argument->alarm_param[0].zwave_alarm_event_param_len = -1;

			argument->alarm_param[1].zwave_alarm_type = ZW_ALRM_CO2;
			argument->alarm_param[1].zwave_alarm_event = ZW_ALRM_EVT_CO2;
			argument->alarm_param[1].zwave_alarm_event_param_len = 0;

			argument->inofParam = 2;
		}
		break;

		case eTrigger_co2Alarm_Clear:
		{
			argument->alarm_param[0].zwave_alarm_type = ZW_ALRM_CO2;
			argument->alarm_param[0].zwave_alarm_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
			argument->alarm_param[0].zwave_alarm_event_param_len = 1;
			argument->alarm_param[0].zwave_alarm_event_param[0] = ZW_ALRM_EVT_CO2_L;

			argument->alarm_param[1].zwave_alarm_type = ZW_ALRM_CO2;
			argument->alarm_param[1].zwave_alarm_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
			argument->alarm_param[1].zwave_alarm_event_param_len = 1;
			argument->alarm_param[1].zwave_alarm_event_param[0] = ZW_ALRM_EVT_CO2;

			argument->inofParam = 2;
		}
		break;

		case eTrigger_waterAlarm_Detected:
		{
			argument->alarm_param[0].zwave_alarm_type = ZW_ALRM_WATER;
			argument->alarm_param[0].zwave_alarm_event = ZW_ALRM_EVT_LEAK_L;
			argument->alarm_param[0].zwave_alarm_event_param_len = -1;

			argument->alarm_param[1].zwave_alarm_type = ZW_ALRM_WATER;
			argument->alarm_param[1].zwave_alarm_event = ZW_ALRM_EVT_LEAK;
			argument->alarm_param[1].zwave_alarm_event_param_len = 0;

			argument->inofParam = 2;
		}
		break;

		case eTrigger_waterAlarm_Clear:
		{
			argument->alarm_param[0].zwave_alarm_type = ZW_ALRM_WATER;
			argument->alarm_param[0].zwave_alarm_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
			argument->alarm_param[0].zwave_alarm_event_param_len = 1;
			argument->alarm_param[0].zwave_alarm_event_param[0] = ZW_ALRM_EVT_LEAK_L;

			argument->alarm_param[1].zwave_alarm_type = ZW_ALRM_WATER;
			argument->alarm_param[1].zwave_alarm_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
			argument->alarm_param[1].zwave_alarm_event_param_len = 1;
			argument->alarm_param[1].zwave_alarm_event_param[0] = ZW_ALRM_EVT_LEAK;

			argument->inofParam = 2;
		}
		break;

		case eTrigger_contactAlarm_Open:
		{
			argument->alarm_param[0].zwave_alarm_type = ZW_ALRM_LOCK;
			argument->alarm_param[0].zwave_alarm_event = ZW_ALRM_EVT_WINDOW_DOOR_OPEN;
			argument->alarm_param[0].zwave_alarm_event_param_len = 0;

			argument->inofParam = 1;
		}
		break;

		case eTrigger_contactAlarm_Closed:
		{
			argument->alarm_param[0].zwave_alarm_type = ZW_ALRM_LOCK;
			argument->alarm_param[0].zwave_alarm_event = ZW_ALRM_EVT_WINDOW_DOOR_CLOSED;
			argument->alarm_param[0].zwave_alarm_event_param_len = 0;

			argument->inofParam = 1;
		}
		break;

		default:
		  ZWP_LOG(ZWP_LOG_ERR, "Trigger: %d not recognized", argument->eTrigger);
		  goto l_err_parse_category;
		  break;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_parse_category:
l_err_invalid_value:
l_err_get_category:
l_err_get_trigger_data:

	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free arguments for alarm get operation
* @param[in]     args              Arguments passed with alarm get operation
* @return        Nothing
* @see zwp_avi_networks_failed_node_remove_argument
*/
static void zwp_avi_ifttt_alarm_get_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
* @brief Execute Get command
* @param[in]     desc_interface    Interface descriptor
* @param[in]     args              Arguments passed with Get command
* @param[out]    hcapi_code_p      HCAPI error code
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_ifttt_alarm_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_ifttt_alarm_get_t *argument;
	zwp_avi_interfaces_alarm_state_t *state;
	int		i, j;
	int iRecordUpdated = 0;
	int iNumRecordFound, iNumColPerRecord;
	pzwp_db_table_value pMatchedTableValue = NULL;
	ifttt_generic_table_device_data device_data;

	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_START, NULL);

	//IFTTT Get trigger
	//check/update trigger arguments inside the generic trigger table
	//If trigger argument not exists in generic trigger table, insert it.
	//Query alarm table with the specifc trigger ID data

	argument = (zwp_avi_ifttt_alarm_get_t *)args;

	if ((state = (zwp_avi_interfaces_alarm_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	for (i = 0; i < argument->inofParam; i++)
	{
		//Update generic trigger table for the trigger ID.
		//If not exists, create the record.

		sprintf(device_data.sState, "ztype=%d&event=%d", 
			argument->alarm_param[i].zwave_alarm_type, 
			argument->alarm_param[i].zwave_alarm_event);
		device_data.iParam_size = argument->alarm_param[i].zwave_alarm_event_param_len;
		device_data.pParam = argument->alarm_param[i].zwave_alarm_event_param;

		if (zwp_avi_ifttt_generic_table_update_trigger_id(state->pifttt_generic_trigger_table,
			argument->trigger_id_data,
			desc_interface,
			device_data,
			&iRecordUpdated) != ZWP_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to update trigger id record to generic table");
			goto l_err_update_generic_table;
		}

		ZWP_LOG(ZWP_LOG_DEBUG, "%d record update.", iRecordUpdated);

		//if (iRecordUpdated != 0) means already data already in generic table
		if (iRecordUpdated == 0)
		{
			//Data not in generic table
			//Insert record
			if (zwp_avi_ifttt_generic_table_insert(state->pifttt_generic_trigger_table,
				argument->trigger_id_data,
				desc_interface,
				device_data,
				IFTTT_DB_TABLE_NAME_ALARM) != ZWP_STATUS_OK)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to insert record to generic table");
				goto l_err_insert_generic_table;
			}
		}
	}


	//Query record from CC table based on trigger ID and sort by timestamp
	if (zwp_avi_ifttt_query_trigger_id_from_cc_table(
		state->pifttt_alarm_table,
		argument->trigger_id_data,
		&iNumRecordFound,
		&iNumColPerRecord,
		&pMatchedTableValue) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to query record from cc table. Error: %s", zwp_db_get_table_errortext(state->pifttt_alarm_table));
		goto l_err_query_cc_table;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "%d record found in cc table.", iNumRecordFound);

	//Save query record to state
	if (state->ifttt_alarm_get_result.putimeList)
	{
		//If contains previous data, delete first
		ZWP_FREE(state->ifttt_alarm_get_result.putimeList);
		state->ifttt_alarm_get_result.putimeList = NULL;
	}

	if (iNumRecordFound > 0)
	{

		if ((state->ifttt_alarm_get_result.putimeList = (zwu_time_t *)ZWP_CALLOC(iNumRecordFound, sizeof(zwu_time_t))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_get_result_alloc;
		}

		for (i = ALARM_TABLE_INDEX_TIMESTAMP, j = 0;
			j < iNumRecordFound;
			i += iNumColPerRecord, j++)
		{
			if (strcmp(pMatchedTableValue[i].pcColumnName, TABLE_COL_NAME_TIMESTAMP))
			{
				//Column name is not 'timestamp'
				ZWP_LOG(ZWP_LOG_ERR, "Wrong column format");
				goto l_err_table_data_format;
			}

			state->ifttt_alarm_get_result.putimeList[j] = pMatchedTableValue[i].data.valueint64;
		}

	}

	state->ifttt_alarm_get_result.iNofRecords = iNumRecordFound;

	zwp_db_query_record_free(iNumColPerRecord, iNumRecordFound, pMatchedTableValue);

	*hcapi_code_p = ZW_ERR_QUEUED;	//Cannot use ZW_ERR_NONE else the frame work will wait for the transmit status

	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_END, ZWP_STATUS_OK);

	return ZWP_STATUS_OK;

l_err_table_data_format:
	if (state->ifttt_alarm_get_result.putimeList)
	{
		ZWP_FREE(state->ifttt_alarm_get_result.putimeList);
		state->ifttt_alarm_get_result.putimeList = NULL;
	}
l_err_get_result_alloc:
	zwp_db_query_record_free(iNumColPerRecord, iNumRecordFound, pMatchedTableValue);
l_err_query_cc_table:
l_err_insert_generic_table:
l_err_update_generic_table:
l_err_get_state:

	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_END, ZWP_STATUS_ERROR);
	return ZWP_STATUS_ERROR;
}

/**
* @brief Execute passive command
* @param[in]     desc_interface    Interface descriptor
* @param[in]     args              Arguments (if any) passed with Get command
* @param[out]    result_p          Result object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zwp_avi_interfaces_switch_binary_get_result_free() to free resources
*/
static zwp_status_t zwp_avi_ifttt_alarm_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_START, NULL);

	zwp_avi_interfaces_alarm_state_t *state;
	zwp_avi_ifttt_alarm_get_result_t *get_result;
	zw_nameloc_t EpNameLoc = { { 0 } };

	if ((state = (zwp_avi_interfaces_alarm_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((get_result = (zwp_avi_ifttt_alarm_get_result_t *)ZWP_CALLOC(1, sizeof(*get_result))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_get_result_alloc;
	}

	if (zwp_desc_get_endpoint_nameloc_from_interface(desc_interface, &EpNameLoc) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Fail to get endpoint name/loc.");
		goto l_err_get_ep_name_loc;
	}

	strcpy(get_result->name, EpNameLoc.name);
	strcpy(get_result->loc, EpNameLoc.loc);

	get_result->iNofRecords = state->ifttt_alarm_get_result.iNofRecords;

	if ((get_result->putimeList = (zwu_time_t *)ZWP_CALLOC(1, get_result->iNofRecords * sizeof(zwu_time_t))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_timestamp_alloc;
	}

	memcpy(get_result->putimeList, state->ifttt_alarm_get_result.putimeList, get_result->iNofRecords * sizeof(zwu_time_t));

	ZWP_LOG(ZWP_LOG_DEBUG, "%d records found.", get_result->iNofRecords);

	*result_p = get_result;

	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_END, ZWP_STATUS_OK);

	return ZWP_STATUS_OK;

	ZWP_FREE(get_result->putimeList);
l_err_timestamp_alloc:
l_err_get_ep_name_loc:
	ZWP_FREE(get_result);
l_err_get_result_alloc:
l_err_get_state:
	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_END, ZWP_STATUS_ERROR);
	return ZWP_STATUS_ERROR;
}


/**
* @brief Set Avro value object with values from Result object corresponding to Report command
* @param[in]     result            Result object
* @param[out]    arg_value         Avro value object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_ifttt_alarm_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_ifttt_alarm_get_result_t *get_result;
	avro_value_t timestamp_list_value;
	avro_value_t timestamp;
	int i;

	get_result = (zwp_avi_ifttt_alarm_get_result_t *)result;

	if (zwp_avi_ifttt_get_result_assign_ep_name_loc(get_result->name, get_result->loc, arg_value) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to set endpoint name/loc to avro");
		goto l_err_set_name_loc_value;
	}

	if (avro_value_get_by_name(arg_value, "utime_array", &timestamp_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'utime_array': %s", avro_strerror());
		goto l_err_get_timestamp_list_value_value;
	}

	for (i = 0; i < get_result->iNofRecords; i++)
	{
		if (avro_value_append(&timestamp_list_value, &timestamp, 0) != 0)
		{
			goto l_err_append_timestamp_value;
		}

		if (avro_value_set_long(&timestamp, get_result->putimeList[i]) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to set value '%llu' to the field utime_array : %s",
				get_result->putimeList[i], avro_strerror);
			goto l_err_set_timestamp_value;
		}
	}

	return ZWP_STATUS_OK;

l_err_set_timestamp_value:
l_err_append_timestamp_value:
l_err_get_timestamp_list_value_value:
l_err_set_name_loc_value:
	return ZWP_STATUS_ERROR;
}


/**
* @brief Free Result object corresponding to Report command
* @param[in]     result            Result object
* @return        Nothing
* @see zwp_avi_interfaces_switch_binary_get_execute_passive
*/
static void zwp_avi_ifttt_alarm_get_result_free(void *result)
{
	zwp_avi_ifttt_alarm_get_result_t *get_result = result;

	if (get_result)
	{
		if(get_result->putimeList)
			ZWP_FREE(get_result->putimeList);
		ZWP_FREE(get_result);
	}
}

/**
* @brief HCAPI report handler for IFTTT trigger
* @param[in]	  desc_interface  interface descriptor
* @param[in]	  alarm_info	  Alarm information
* @param[in]	  ts			  timestamp
*/
void zwp_avi_interfaces_alarm_report_handler_ifttt(zwifd_p desc_interface, zwalrm_p alarm_info, time_t ts)
{
	zwp_avi_interfaces_alarm_state_t *state;
	char sTriggerID[MAX_TRIGGER_ID_LENGTH] = { 0 };
	ifttt_generic_table_device_data stQueryCond;

	//IFTTT Trigger only works for ZWave type & event. If no ZWave type & event, no need to check
	if (!alarm_info->ex_info) return;

	//Check if generic table has the data
	//If yes, get the trigger ID and set to alarm table

	if ((state = (zwp_avi_interfaces_alarm_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	//Create generic table query condition for alarm report
	sprintf(stQueryCond.sState, "ztype=%d&event=%d", alarm_info->ex_type, alarm_info->ex_event);
	stQueryCond.iParam_size = alarm_info->ex_evt_len;
	stQueryCond.pParam = &alarm_info->ex_evt_prm[0];

	if (zwp_avi_ifttt_generic_table_report_exist(state->pifttt_generic_trigger_table,
		desc_interface,
		stQueryCond,
		sTriggerID) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Failed to find matched record in generic table. No need to save to database.");
		goto l_err_report_exist;
	}

	ZWP_LOG(ZWP_LOG_ERR, "Match found. Try to insert into CC table.");

	if (zwp_avi_ifttt_cc_table_insert(state->pifttt_alarm_table,
		desc_interface,
		alarm_info,
		COMMAND_CLASS_ALARM,
		(zwu_time_t)ts,
		sTriggerID) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Failed to insert record in cc table. ");
		goto l_err_cc_table_insert;
	}

l_err_cc_table_insert:
l_err_report_exist:
l_err_get_state:
	return;
}

/** Handlers for getting alarm option */
static zwp_avi_message_networks_t ifttt_alarm_option =
{
	"ifttt_network_alarm_option",
	zwp_avi_ifttt_alarm_option_argument, zwp_avi_ifttt_alarm_option_argument_free,
	zwp_avi_ifttt_alarm_option_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
	zwp_avi_ifttt_device_name_option_result, zwp_avi_ifttt_alarm_option_result_free
};

/** Handlers for alarm Get command (interface leel) */
static zwp_avi_message_interfaces_t ifttt_interfaces_alarm_get =
{
	"ifttt_interface_alarm_get",
	COMMAND_CLASS_ALARM, NULL,
	NULL, NULL,
	zwp_avi_ifttt_alarm_get_argument, zwp_avi_ifttt_alarm_get_argument_free,
	NULL,
	zwp_avi_ifttt_alarm_get_execute_active, zwp_avi_ifttt_alarm_get_execute_passive,
	zwp_avi_ifttt_alarm_get_result, zwp_avi_ifttt_alarm_get_result_free
};

/** Register handlers */
zwp_status_t zwp_avi_ifttt_alarm_init(zwu_array_list_t network_message_list, zwu_array_list_t interface_message_list)
{
	if ((zwp_avi_message_list_add_message_networks(network_message_list, &ifttt_alarm_option) != ZWP_STATUS_OK) ||
		(zwp_avi_message_list_add_message_interfaces(interface_message_list, &ifttt_interfaces_alarm_get)))
	{
		goto l_err_alarm_option;
	}

	return ZWP_STATUS_OK;

l_err_alarm_option:
	return ZWP_STATUS_ERROR;
}

