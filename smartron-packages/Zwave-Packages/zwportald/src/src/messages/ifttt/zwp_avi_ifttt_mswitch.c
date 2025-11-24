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
 * @file    zwp_avi_ifttt_mswitch.c
 *
 * @brief   AVI message 'ifttt_mswitch'
 * @details Handles the AVI message 'ifttt_mswitch'.
 *
 * @author  Angela Yan
 *
 * @version 1.0 - 2016-12-14
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
#include "zwp_avi_interfaces_switch_multilevel.h"

 /** mswitch option argument */
typedef struct {
	int					type;		//Dimmer or otherwise
	eifttt_fields		efield;
} zwp_avi_ifttt_mswitch_device_name_option_t;

 /**
 * @brief Get argument for mswitch option operation from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Node Add operation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_networks_node_add_argument_free() to free resources
 */
static zwp_status_t zwp_avi_ifttt_mswitch_device_name_option_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_ifttt_mswitch_device_name_option_t *argument;
	int32_t ifield;

	if ((argument = (zwp_avi_ifttt_mswitch_device_name_option_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	ifield = (int32_t)argument->efield;										   
		
	if ((zw_avro_field_get_int(arg_value, "type", &argument->type) != ZWP_STATUS_OK)
		|| (zw_avro_field_get_int(arg_value, "efield", &ifield) != ZWP_STATUS_OK))
	{
		goto l_err_get_efield;
	}

	argument->efield = (eifttt_fields)ifield;

	*args_p = argument;							   
												   
	return ZWP_STATUS_OK;	

l_err_get_efield:
	ZWP_FREE(argument);
l_err_args_alloc:								   
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free arguments for mswitch option operation
* @param[in]     args              Arguments passed with Remove Failed Node operation
* @return        Nothing
* @see zwp_avi_networks_failed_node_remove_argument
*/
static void zwp_avi_ifttt_mswitch_device_name_option_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
 * @brief Execute call to mswitch option operation status
 * @param[in]     net               Network object
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 *@note
 * @par
 * The caller should call zwp_avi_ifttt_mswitch_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_ifttt_mswitch_device_name_option_execute(zwp_net_t net, void *arg, void **result_p)
{
	//zwp_status_t res = ZWP_STATUS_ERROR;
	int i;
	zwp_avi_ifttt_mswitch_device_name_option_t *argument;
	zwp_desc_net_t desc_net;
	zwu_array_list_t if_ep_name_list;
	zwp_desc_IF_ep_name_t  *pstIF_ep_name;
	uint16_t interfaceid = COMMAND_CLASS_SWITCH_MULTILEVEL;

	argument = (zwp_avi_ifttt_mswitch_device_name_option_t *)arg;

	desc_net = zwp_net_get_desc_net(net);

	if (!desc_net)
		goto l_err_invalid_net_desc;

	if (argument->efield == eDevice_name)
	{
		if (zwp_desc_get_device_from_interface_id(desc_net, interfaceid, &if_ep_name_list) != ZWP_STATUS_OK)
		{
			goto l_err_get_if_ep_name_list;
		}

		//Check whether the multilevel switch device is dimmer or not
		for (i = 0; i < zwu_array_list_size(if_ep_name_list); i++)
		{
			pstIF_ep_name = zwu_array_list_get(if_ep_name_list, i);

			if (pstIF_ep_name)
			{
				if (!zwp_ifttt_interface_has_supported_multilevel_switch_device(desc_net,
																				pstIF_ep_name->desc_interface, 
																				argument->type))
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
 * @see zwp_avi_ifttt_mswitch_get_execute
 */
static void zwp_avi_ifttt_mswitch_device_name_option_result_free(void *result)
{
	zwp_desc_free_if_ep_name_list(result);
}

/**
* @brief Get argument for mswitch set operation from Avro value object
* @param[in]     arg_value         Avro value object
* @param[out]    args_p            Arguments passed with Node Add operation
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zwp_avi_networks_node_add_argument_free() to free resources
*/
static zwp_status_t zwp_avi_ifttt_mswitch_set_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_switch_multilevel_set_t *argument;
	int integer;

	if ((argument = (zwp_avi_interfaces_switch_multilevel_set_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "value", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_get_bonoff;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "value: %d", integer);

	if (zwu_number_is_uint8(integer) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid value: %d", integer);
		goto l_err_invalid_value;
	}

	argument->value = (uint8_t)integer;
	argument->dimming_duration = COMMAND_SWITCH_MULTILEVEL_DIMMING_DURATION_NONE; //set to none

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_invalid_value:
l_err_get_bonoff:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free arguments for mswitch set operation
* @param[in]     args              Arguments passed with Remove Failed Node operation
* @return        Nothing
* @see zwp_avi_networks_failed_node_remove_argument
*/
static void zwp_avi_ifttt_mswitch_set_argument_free(void *args)
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
static zwp_status_t zwp_avi_ifttt_mswitch_set_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_switch_multilevel_set_t *argument;
	zwp_avi_interfaces_switch_multilevel_state_t *state;

	argument = (zwp_avi_interfaces_switch_multilevel_set_t *)args;

	if (argument->dimming_duration == COMMAND_SWITCH_MULTILEVEL_DIMMING_DURATION_NONE)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Dimming Duration not provided. Defaulting to 0xFF (Factory Default)");

		// If not given, use factory default 0xFF
		argument->dimming_duration = 0xFF;
	}

	*hcapi_code_p = zwif_level_set(desc_interface, argument->value, argument->dimming_duration, NULL, NULL);

	if ((state = (zwp_avi_interfaces_switch_multilevel_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "HCAPI zwif_level_set result: %d", *hcapi_code_p);

	if(HCAPI_SUCCEEDED(*hcapi_code_p))
	//if ((*hcapi_code_p != ZW_ERR_NONE) && (*hcapi_code_p != ZW_ERR_QUEUED) && (*hcapi_code_p != ZW_ERR_CACHE_AVAIL))
	{
		state->ifttt_set_report.value = argument->value;
		state->ifttt_set_report.utime = zwu_time_now();

		ZWP_LOG(ZWP_LOG_DEBUG, "utime: %lu", state->ifttt_set_report.utime);
	}

	return ZWP_STATUS_OK;

l_err_get_state:
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
* The caller should call zwp_avi_ifttt_mswitch_set_result_free() to free resources
*/
static zwp_status_t zwp_avi_ifttt_mswitch_set_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_switch_multilevel_state_t *state;
	zwp_avi_ifttt_mswitch_set_result_t *set_result;

	if ((state = (zwp_avi_interfaces_switch_multilevel_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((set_result = (zwp_avi_ifttt_mswitch_set_result_t *)ZWP_CALLOC(1, sizeof(*set_result))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	memcpy(set_result, &state->ifttt_set_report, sizeof(*set_result));

	ZWP_LOG(ZWP_LOG_DEBUG, "utime: %lu", set_result->utime);

	*result_p = set_result;

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
static zwp_status_t zwp_avi_ifttt_mswitch_set_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_ifttt_mswitch_set_result_t *set_result;

	set_result = (zwp_avi_ifttt_mswitch_set_result_t *)result;

	ZWP_LOG(ZWP_LOG_DEBUG, "utime: %lu", set_result->utime);

	if (zw_avro_field_set_long(arg_value, "utime", set_result->utime) != ZWP_STATUS_OK)
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
* @see zwp_avi_ifttt_mswitch_set_execute_passive
*/
static void zwp_avi_ifttt_mswitch_set_result_free(void *result)
{
	ZWP_FREE(result);
}

/** Handlers for getting mswitch device_name option */
static zwp_avi_message_networks_t ifttt_mswitch_device_name_option =
{
	"ifttt_network_mswitch_device_name_option",
	zwp_avi_ifttt_mswitch_device_name_option_argument, zwp_avi_ifttt_mswitch_device_name_option_argument_free,
	zwp_avi_ifttt_mswitch_device_name_option_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
	zwp_avi_ifttt_device_name_option_result, zwp_avi_ifttt_mswitch_device_name_option_result_free
};

/** Handlers for mswitch Set command (interface leel) */
static zwp_avi_message_interfaces_t ifttt_interfaces_mswitch_set =
{
	"ifttt_interface_mswitch_set",
	COMMAND_CLASS_DOOR_LOCK, NULL,
	NULL, NULL,
	zwp_avi_ifttt_mswitch_set_argument, zwp_avi_ifttt_mswitch_set_argument_free,
	NULL,
	zwp_avi_ifttt_mswitch_set_execute_active, zwp_avi_ifttt_mswitch_set_execute_passive,
	zwp_avi_ifttt_mswitch_set_result, zwp_avi_ifttt_mswitch_set_result_free
};

/** Register handlers */
zwp_status_t zwp_avi_ifttt_mswitch_init(zwu_array_list_t network_message_list, zwu_array_list_t interface_message_list)
{
	if ((zwp_avi_message_list_add_message_networks(network_message_list, &ifttt_mswitch_device_name_option) != ZWP_STATUS_OK) ||
		(zwp_avi_message_list_add_message_interfaces(interface_message_list, &ifttt_interfaces_mswitch_set)))
	{
		goto l_err_mswitch_option;
	}

	return ZWP_STATUS_OK;

l_err_mswitch_option:
	return ZWP_STATUS_ERROR;
}

