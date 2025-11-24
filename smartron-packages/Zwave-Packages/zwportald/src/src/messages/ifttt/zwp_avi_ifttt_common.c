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
 * @file    zwp_avi_ifttt_common.c
 *
 * @brief   Common header file for IFTTT AVI messages
 * @details Common header file for IFTTT AVI messages.
 *
 * @author  Angela
 *
 * @version 1.0 - 2016-08-02
 * - Initial version
 */

#include "zwp_avi_ifttt_common.h"
#include "zwp_descriptor.h"
#include "zwp_log.h"
#include "zwp_avi_interfaces_thermostat_mode.h"
#include "zwp_avi_interfaces_alarm.h"
#include "zwp_avi_interfaces_sensor_multilevel.h"
 
/**IFTTT Action param define/details */
static ifttt_feature_cls_t	arIFTTT_feature_2_cls[] =
{
	//eIFTTT_IF_feature					//wZwave_cls_id					//iNofParam		//iParam
	{ eTrigger_bSensor_On,				COMMAND_CLASS_SENSOR_BINARY,	0,				0},
	{ eTrigger_bSensor_Off,				COMMAND_CLASS_SENSOR_BINARY,	0,				0},
	{ eTrigger_motionAlarm_Detected,	COMMAND_CLASS_ALARM,			1,				ZW_ALARM_CATEGORY_MOTION },
	{ eTrigger_motionAlarm_Clear,		COMMAND_CLASS_ALARM,			1,				ZW_ALARM_CATEGORY_MOTION },
	{ eTrigger_smokeAlarm_Detected,		COMMAND_CLASS_ALARM,			1,				ZW_ALARM_CATEGORY_SMOKE },
	{ eTrigger_smokeAlarm_Clear,		COMMAND_CLASS_ALARM,			1,				ZW_ALARM_CATEGORY_SMOKE },
	{ eTrigger_coAlarm_Detected,		COMMAND_CLASS_ALARM,			1,				ZW_ALARM_CATEGORY_CO },
	{ eTrigger_coAlarm_Clear,			COMMAND_CLASS_ALARM,			1,				ZW_ALARM_CATEGORY_CO },
	{ eTrigger_co2Alarm_Detected,		COMMAND_CLASS_ALARM,			1,				ZW_ALARM_CATEGORY_CO2 },
	{ eTrigger_co2Alarm_Clear,			COMMAND_CLASS_ALARM,			1,				ZW_ALARM_CATEGORY_CO2 },
	{ eTrigger_waterAlarm_Detected,		COMMAND_CLASS_ALARM,			1,				ZW_ALARM_CATEGORY_WATER },
	{ eTrigger_waterAlarm_Clear,		COMMAND_CLASS_ALARM,			1,				ZW_ALARM_CATEGORY_WATER },
	{ eTrigger_contactAlarm_Open,		COMMAND_CLASS_ALARM,			1,				ZW_ALARM_CATEGORY_CONTACT },
	{ eTrigger_contactAlarm_Closed,		COMMAND_CLASS_ALARM,			1,				ZW_ALARM_CATEGORY_CONTACT },
	{ eTrigger_mSensor_Temp_LT,			COMMAND_CLASS_SENSOR_MULTILEVEL,1,				ZW_SENSOR_TYPE_TEMP },
	{ eTrigger_mSensor_Temp_GT,			COMMAND_CLASS_SENSOR_MULTILEVEL,1,				ZW_SENSOR_TYPE_TEMP },
	{ eTrigger_mSensor_Luminance_LT,	COMMAND_CLASS_SENSOR_MULTILEVEL,1,				ZW_SENSOR_TYPE_LUM },
	{ eTrigger_mSensor_Luminance_GT,	COMMAND_CLASS_SENSOR_MULTILEVEL,1,				ZW_SENSOR_TYPE_LUM },
	{ eTrigger_mSensor_Humidity_LT,		COMMAND_CLASS_SENSOR_MULTILEVEL,1,				ZW_SENSOR_TYPE_HUMD },
	{ eTrigger_mSensor_Humidity_GT,		COMMAND_CLASS_SENSOR_MULTILEVEL,1,				ZW_SENSOR_TYPE_HUMD },

	{ eAction_bSwitch_On,				COMMAND_CLASS_SWITCH_BINARY,	0,				0},
	{ eAction_bSwitch_Off,				COMMAND_CLASS_SWITCH_BINARY,	0,				0},
	{ eAction_dLock_Open,				COMMAND_CLASS_DOOR_LOCK,		0,				0},
	{ eAction_dLock_Close,				COMMAND_CLASS_DOOR_LOCK,		0,				0},
	{ eAction_mSwitch_Dimmer,			COMMAND_CLASS_SWITCH_MULTILEVEL,1,				IFTTT_MSWITCH_TYPE_DIMMER},
	{ eAction_thrmoMode_Off,			COMMAND_CLASS_THERMOSTAT_MODE,	1,				ZW_THRMO_MD_OFF},
	{ eAction_thrmoMode_Heat,			COMMAND_CLASS_THERMOSTAT_MODE,	1,				ZW_THRMO_MD_HEAT},
	{ eAction_thrmoMode_Cool,			COMMAND_CLASS_THERMOSTAT_MODE,	1,				ZW_THRMO_MD_COOL},
	{ eAction_thrmoMode_Auto,			COMMAND_CLASS_THERMOSTAT_MODE,	1,				ZW_THRMO_MD_AUTO}
	

};

/**
@brief Return the corresponding ZWave command class ID based on IFTTT interface feature.
@param[in]	aefeature				IFTTT interface feature enum.
@param[out]	pcls_id					Corresponding ZWave command class ID
@param[out]	piNofParam				Corresponding number of additional params
@param[out]	piParam					Corresponding additional params
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_ifttt_find_cls_from_efeature(eifttt_IF_feature_type aefeature, uint16_t *pcls_id, int *piNofParam, int* piParam)
{
	int i, feature_count;

	feature_count = sizeof(arIFTTT_feature_2_cls) / sizeof(ifttt_feature_cls_t);

	for (i = 0; i < feature_count; i++)
	{
		if (arIFTTT_feature_2_cls[i].eIFTTT_IF_feature == aefeature)
		{
			*pcls_id = arIFTTT_feature_2_cls[i].wZwave_cls_id;
			*piNofParam = arIFTTT_feature_2_cls[i].iNofParam;
			*piParam = arIFTTT_feature_2_cls[i].iParam;

			return ZWP_STATUS_OK;
		}
	}

	ZWP_LOG(ZWP_LOG_ERR, "Unrecognized IFTTT efeature %d", aefeature);

	return ZWP_STATUS_ERROR;
}

/**
* @brief Set Avro value object with values from Result object corresponding to network operation state
* @param[in]     result            Result object
* @param[out]    arg_value         Avro value object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
zwp_status_t zwp_avi_ifttt_device_name_option_result(void *result, avro_value_t *arg_value)
{

	zwu_array_list_t if_ep_name_list;
	zwp_desc_IF_ep_name_t  *pstIF_ep_name;
	avro_value_t ifttt_device_list_value;
	avro_value_t ifttt_device_value;
	int i, count;

	if_ep_name_list = (zwu_array_list_t)result;
	count = zwu_array_list_size(if_ep_name_list);

	if (avro_value_get_by_name(arg_value, "ifttt_device_list", &ifttt_device_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'ifttt_device_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	for (i = 0; i < count; i++)
	{
		if (avro_value_append(&ifttt_device_list_value, &ifttt_device_value, 0) != 0)
		{
			goto l_err_append_ifttt_device_value;
		}

		pstIF_ep_name = zwu_array_list_get(if_ep_name_list, i);

		if ((zw_avro_field_set_int(&ifttt_device_value, "id", pstIF_ep_name->desc_interface->cls) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(&ifttt_device_value, "node_id", pstIF_ep_name->desc_interface->nodeid) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(&ifttt_device_value, "endpoint_id", pstIF_ep_name->desc_interface->epid) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_string(&ifttt_device_value, "name", pstIF_ep_name->name) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_string(&ifttt_device_value, "location", pstIF_ep_name->loc) != ZWP_STATUS_OK)
			)
		{
			goto l_err_set_ifttt_device_value;
		}
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Done. %d devices found.", count);

	return ZWP_STATUS_OK;

l_err_set_ifttt_device_value:
l_err_append_ifttt_device_value:
l_err_get_list_value:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Set Avro value object with endpoint name and location
* @param[in]     name			   Endpoint name
* @param[in]     loc			   Endpoint location
* @param[out]    arg_value         Avro value object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
zwp_status_t zwp_avi_ifttt_get_result_assign_ep_name_loc(char *name, char *loc, avro_value_t *arg_value)
{
	avro_value_t ifttt_interface_name;

	if (avro_value_get_by_name(arg_value, "ifttt_interface_name", &ifttt_interface_name, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'ifttt_interface_name': %s", avro_strerror());
		goto l_err_get_interface_name;
	}

	if ((zw_avro_field_set_string(&ifttt_interface_name, "name", name) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_string(&ifttt_interface_name, "location", loc) != ZWP_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'name'/'location': %s", avro_strerror());
		goto l_err_set_interface_name;
	}

	return ZWP_STATUS_OK;

l_err_get_interface_name:
l_err_set_interface_name:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Check whether the thermostat mode interface has a specific mode
* @param[in]     desc_interface    Interface descriptor
* @param[in]     mode			   The thermostat mode to look for
* @retval        0 - interface does not support this mode; 1 - interface supports this mode
*/
int zwp_ifttt_interface_has_supported_thermostat_mode(zwifd_p desc_interface, int mode)
{
	int iFound = 0, i;
	zwp_avi_interfaces_thermostat_mode_state_t *state;
	uint8_t arMode_buf[255] = { 0 }, uMode_cnt = 0;
	int hcapi_code;

	//Get cached supported mode from ZWare thermostat mode interface state
	//If ZWare cache is empty, get from HCAPI cached value and update the ZWare thermostat mode interface state

	if ((state = (zwp_avi_interfaces_thermostat_mode_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (state->sup_report.sup_mode_list_cnt == 0)
	{
		//ZWare thermostat mode cache is empty, call HCAPI direct
		hcapi_code = zwif_thrmo_md_sup_cache_get(desc_interface, arMode_buf, &uMode_cnt);

		if (HCAPI_SUCCEEDED(hcapi_code) && uMode_cnt != 0)
		{
			//Update ZWare thermostat mode supported list
			zwp_avi_interfaces_thermostat_mode_supported_report_handler(desc_interface,
																		uMode_cnt,
																		arMode_buf,
																		1);
		}
		else
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get supported therstat modes from HCAPI cache. Error: %d", hcapi_code);
			goto l_err_hcapi_get_sup_mode;
		}
	}
	
	for (i = 0; i < state->sup_report.sup_mode_list_cnt; i++)
	{
		if (state->sup_report.sup_mode_list[i] == mode)
		{
			//This thermostast device has the support Mode
			ZWP_LOG(ZWP_LOG_DEBUG, "Found supported mode %d in this dev (node %d, ep %d) ",
				mode, desc_interface->nodeid, desc_interface->epid);
			iFound = 1;
			break;
		}
	}

	return iFound;

l_err_hcapi_get_sup_mode:
l_err_get_state:
	ZWP_LOG(ZWP_LOG_DEBUG, "Cannot find thermostat mode %d in node %d, ep %d", 
		mode, desc_interface->nodeid, desc_interface->epid);
	return iFound;
}

/**
* @brief Check whether device endpoint is a correct multilevel switch type
* @param[in]     desc_net          Network descriptor object
* @param[in]     desc_interface    Interface descriptor
* @param[in]     type			   The different device type for multilevel switch
* @retval        0 - interface does not support this mode; 1 - interface supports this mode
*/
int zwp_ifttt_interface_has_supported_multilevel_switch_device(zwp_desc_net_t desc_net, zwifd_p desc_interface, int type)
{
	int iFound = 0;
	zwepd_p desc_endpoint = NULL;

	if((zwp_desc_get_desc_endpoint(desc_net, desc_interface->nodeid, desc_interface->epid, &desc_endpoint)) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to corresponding endpoint descriptor.");
		goto l_err_get_ep_desc;
	}

	if (type == IFTTT_MSWITCH_TYPE_DIMMER)
	{
		if (desc_endpoint->generic == GENERIC_TYPE_SWITCH_MULTILEVEL)
		{
			if ((desc_endpoint->specific == SPECIFIC_TYPE_NOT_USED)
				|| (desc_endpoint->specific == SPECIFIC_TYPE_POWER_SWITCH_MULTILEVEL)
				|| (desc_endpoint->specific == SPECIFIC_TYPE_SCENE_SWITCH_MULTILEVEL)
				)
			{
				iFound = 1;
				ZWP_LOG(ZWP_LOG_DEBUG, "Found supported Dimmer device (node %d, ep %d) ", desc_interface->nodeid, desc_interface->epid);
			}
		}
	}

l_err_get_ep_desc:
	return iFound;
}

/**
* @brief Check whether the alarm interface has supported type/event
* @param[in]     desc_interface    Interface descriptor
* @param[in]     category		   The notification category to look for
* @retval        0 - interface does not support this category; 1 - interface supports this category
*/
int zwp_ifttt_interface_has_supported_alarm_type_event(zwifd_p desc_interface, int category)
{
	int iFound = 0, i, j;
	zwp_avi_interfaces_alarm_state_t *state;
	uint8_t have_vtype, ztype_cnt, ztype_buf[255] = { 0 }, evt_cnt, evt_buf[255] = { 0 };
	int event_supported_list_count;
	zwp_avi_interfaces_alarm_single_event_supported_report_t *palarm_events_per_type;
	int hcapi_code;

	//Get cached supported type and event from ZWare alarm interface state
	//If ZWare cache is empty, get from HCAPI cached value and update the ZWare thermostat mode interface state

	if ((state = (zwp_avi_interfaces_alarm_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (state->type_supported_report.supported_types_count == 0)
	{
		//ZWare alarm type cache is empty, call HCAPI direct
		hcapi_code = zwif_alrm_sup_cache_get(desc_interface, &have_vtype, &ztype_cnt, ztype_buf);

		if (HCAPI_SUCCEEDED(hcapi_code) && ztype_cnt != 0)
		{
			//Update ZWare thermostat mode supported list
			zwp_avi_interfaces_alarm_type_supported_report_handler(desc_interface,
				have_vtype,
				ztype_cnt,
				ztype_buf,
				1);
		}
		else
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get supported alarm types from HCAPI cache. Error: %d, ztype_cnt = %d", hcapi_code, ztype_cnt);
			goto l_err_hcapi_get_sup_type;
		}
	}

	if (zwu_array_list_size(state->type_event_supported_report.event_supported_list) == 0)
	{
		for (i = 0; i < state->type_supported_report.supported_types_count; i++)
		{
			//ZWare alarm event cache is empty, call HCAPI direct
			hcapi_code = zwif_alrm_sup_evt_cache_get(desc_interface, 
													state->type_supported_report.supported_types[i], 
													&evt_cnt,
													evt_buf);

			if (HCAPI_SUCCEEDED(hcapi_code) && evt_cnt != 0)
			{
				//Update ZWare thermostat mode supported list
				zwp_avi_interfaces_alarm_event_supported_report_handler(desc_interface,
																		state->type_supported_report.supported_types[i],
																		evt_cnt,
																		evt_buf,
																		1);
			}
			else
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to get supported alarm event from HCAPI cache. Error: %d, evt_buf: %d", hcapi_code, evt_buf);
				continue;
				//goto l_err_hcapi_get_sup_event;
			}
		}
	}

	event_supported_list_count = zwu_array_list_size(state->type_event_supported_report.event_supported_list);

	for (i = 0; i < event_supported_list_count; i++)
	{
		palarm_events_per_type = (zwp_avi_interfaces_alarm_single_event_supported_report_t *)zwu_array_list_get(state->type_event_supported_report.event_supported_list, i);

		if (category == ZW_ALARM_CATEGORY_MOTION)
		{
			if (palarm_events_per_type->report_receive_zwave_alarm_type == ZW_ALRM_BURGLAR)
			{
				for (j = 0; j < palarm_events_per_type->supported_events_count; j++)
				{
					if ((palarm_events_per_type->supported_events[j] == ZW_ALRM_EVT_MOTION_DET_L)
						|| (palarm_events_per_type->supported_events[j] == ZW_ALRM_EVT_MOTION_DET))
					{
						//This alarm device has the support type & event						
						iFound = 1;
						break;
					}
				}
			}
		}
		else if (category == ZW_ALARM_CATEGORY_SMOKE)
		{
			if (palarm_events_per_type->report_receive_zwave_alarm_type == ZW_ALRM_SMOKE)
			{
				for (j = 0; j < palarm_events_per_type->supported_events_count; j++)
				{
					if ((palarm_events_per_type->supported_events[j] == ZW_ALRM_EVT_SMOKE_L)
						|| (palarm_events_per_type->supported_events[j] == ZW_ALRM_EVT_SMOKE))
					{
						//This alarm device has the support type & event						
						iFound = 1;
						break;
					}
				}
			}
		}
		else if (category == ZW_ALARM_CATEGORY_CO)
		{
			if (palarm_events_per_type->report_receive_zwave_alarm_type == ZW_ALRM_CO)
			{
				for (j = 0; j < palarm_events_per_type->supported_events_count; j++)
				{
					if ((palarm_events_per_type->supported_events[j] == ZW_ALRM_EVT_CO_L)
						|| (palarm_events_per_type->supported_events[j] == ZW_ALRM_EVT_CO))
					{
						//This alarm device has the support type & event						
						iFound = 1;
						break;
					}
				}
			}
		}
		else if (category == ZW_ALARM_CATEGORY_CO2)
		{
			if (palarm_events_per_type->report_receive_zwave_alarm_type == ZW_ALRM_CO2)
			{
				for (j = 0; j < palarm_events_per_type->supported_events_count; j++)
				{
					if ((palarm_events_per_type->supported_events[j] == ZW_ALRM_EVT_CO2_L)
						|| (palarm_events_per_type->supported_events[j] == ZW_ALRM_EVT_CO2))
					{
						//This alarm device has the support type & event						
						iFound = 1;
						break;
					}
				}
			}
		}
		else if (category == ZW_ALARM_CATEGORY_WATER)
		{
			if (palarm_events_per_type->report_receive_zwave_alarm_type == ZW_ALRM_WATER)
			{
				for (j = 0; j < palarm_events_per_type->supported_events_count; j++)
				{
					if ((palarm_events_per_type->supported_events[j] == ZW_ALRM_EVT_LEAK_L)
						|| (palarm_events_per_type->supported_events[j] == ZW_ALRM_EVT_LEAK))
					{
						//This alarm device has the support type & event						
						iFound = 1;
						break;
					}
				}
			}
		}
		else if (category == ZW_ALARM_CATEGORY_CONTACT)
		{
			if (palarm_events_per_type->report_receive_zwave_alarm_type == ZW_ALRM_LOCK)
			{
				for (j = 0; j < palarm_events_per_type->supported_events_count; j++)
				{
					if ((palarm_events_per_type->supported_events[j] == ZW_ALRM_EVT_WINDOW_DOOR_OPEN)
						|| (palarm_events_per_type->supported_events[j] == ZW_ALRM_EVT_WINDOW_DOOR_CLOSED))
					{
						//This alarm device has the support type & event						
						iFound = 1;
						break;
					}
				}
			}
		}

		if (iFound == 1)
		{
			ZWP_LOG(ZWP_LOG_DEBUG, "Found category %d supported type %d event %d in this dev (node %d, ep %d) ",
				category,
				palarm_events_per_type->report_receive_zwave_alarm_type,
				palarm_events_per_type->supported_events[j],
				desc_interface->nodeid, desc_interface->epid);
			break;
		}
	}

	return iFound;

//l_err_hcapi_get_sup_event:
l_err_hcapi_get_sup_type:
l_err_get_state:
	ZWP_LOG(ZWP_LOG_DEBUG, "Cannot find alarm state category %d in node %d, ep %d",
		category, desc_interface->nodeid, desc_interface->epid);
	return iFound;
}

/**
* @brief Check whether the multilevel interface has supported type
* @param[in]     desc_interface    Interface descriptor
* @param[in]     type			   The multilevel type to look for
* @retval        0 - interface does not support this type; 1 - interface supports this type
*/
int zwp_ifttt_interface_has_supported_msensor_type(zwifd_p desc_interface, int type)
{
	int iFound = 0, i;
	zwp_avi_interfaces_sensor_multilevel_state_t *state;
	uint8_t snsr_cnt, snr_type_buf[255] = { 0 };
	int hcapi_code;

	//Get cached supported type from ZWare multilevel sensor interface state
	//If ZWare cache is empty, get from HCAPI cached value and update the ZWare multilevel sensor interface state

	if ((state = (zwp_avi_interfaces_sensor_multilevel_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Looking for type = %d", type);

	if (state->supported_report.supported_types_count == 0)
	{
		//ZWare alarm type cache is empty, call HCAPI direct
		hcapi_code = zwif_sensor_sup_cache_get(desc_interface, &snsr_cnt, snr_type_buf);

		if (HCAPI_SUCCEEDED(hcapi_code) && snsr_cnt != 0)
		{
			ZWP_LOG(ZWP_LOG_DEBUG, "snsr_cnt = %d", snsr_cnt);

			for(i= 0; i < snsr_cnt; i++)
				ZWP_LOG(ZWP_LOG_DEBUG, "snr_type_buf[i] = %d", snr_type_buf[i]);

			//Update ZWare multilevel sensor type supported list
			zwp_avi_interfaces_sensor_multilevel_supported_report_handler(desc_interface,
				snsr_cnt,
				snr_type_buf,
				1);
		}
		else
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get supported multilevel sensor types from HCAPI cache. Error: %d, snsr_cnt = %d", hcapi_code, snsr_cnt);
			goto l_err_hcapi_get_sup_type;
		}
	}

	for (i = 0; i < state->supported_report.supported_types_count; i++)
	{
		if (state->supported_report.supported_types[i] == type)
		{
			//This thermostast device has the support Mode
			ZWP_LOG(ZWP_LOG_DEBUG, "Found supported type %d in this dev (node %d, ep %d) ",
				type, desc_interface->nodeid, desc_interface->epid);
			iFound = 1;
			break;
		}
	}

	return iFound;

l_err_hcapi_get_sup_type:
l_err_get_state:
	ZWP_LOG(ZWP_LOG_DEBUG, "Cannot find multilevel sensor types %d in node %d, ep %d",
		type, desc_interface->nodeid, desc_interface->epid);
	return iFound;
}


/**
@brief Check the interface device list and modify it according to the additional param requirement.
@param[in]	aefeature				IFTTT interface feature enum.
@param[in, out]	if_ep_name_list		Device interface list that supports the command class
@param[in]  desc_net				Network descriptor object
@param[in]	iNofParam				Number of additional params
@param[in]	iParam					Additional params
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_ifttt_check_device_list_with_additional_param(eifttt_IF_feature_type aefeature,
	zwu_array_list_t if_ep_name_list,
	zwp_desc_net_t desc_net,
	int iNofParam,
	int iParam)
{
	int i, iMatch = 1;
	zwp_desc_IF_ep_name_t  *pstIF_ep_name;

	for (i = 0; i < zwu_array_list_size(if_ep_name_list); i++)
	{
		pstIF_ep_name = zwu_array_list_get(if_ep_name_list, i);
		iMatch = 1;

		if ((aefeature == eAction_thrmoMode_Off) ||
			(aefeature == eAction_thrmoMode_Heat) ||
			(aefeature == eAction_thrmoMode_Cool) ||
			(aefeature == eAction_thrmoMode_Auto))
		{
			if (!zwp_ifttt_interface_has_supported_thermostat_mode(pstIF_ep_name->desc_interface, iParam))
			{
				iMatch = 0;	
			}
		}
		else if (aefeature == eAction_mSwitch_Dimmer)
		{
			if (!zwp_ifttt_interface_has_supported_multilevel_switch_device(desc_net,
																			pstIF_ep_name->desc_interface,
																			iParam))
			{
				iMatch = 0;
			}
		}
		else if ((aefeature >= eTrigger_motionAlarm_Detected) &&
			(aefeature <= eTrigger_contactAlarm_Closed) 
			)
		{
			if (!zwp_ifttt_interface_has_supported_alarm_type_event(pstIF_ep_name->desc_interface,
																	iParam))
			{
				iMatch = 0;
			}
		}
		else if ((aefeature == eTrigger_mSensor_Temp_LT) ||
			(aefeature == eTrigger_mSensor_Temp_GT) ||
			(aefeature == eTrigger_mSensor_Luminance_LT) ||
			(aefeature == eTrigger_mSensor_Luminance_GT) ||
			(aefeature == eTrigger_mSensor_Humidity_LT) ||
			(aefeature == eTrigger_mSensor_Humidity_GT)
			)
		{
			if (!zwp_ifttt_interface_has_supported_msensor_type(pstIF_ep_name->desc_interface,
																iParam))
			{
				iMatch = 0;
			}
		}

		if (iMatch == 0)
		{
			//not found
			//Remove the device from array. readjust i value
			ZWP_FREE(pstIF_ep_name);
			zwu_array_list_remove_at_index(if_ep_name_list, i);
			i--;
		}
	}

	return ZWP_STATUS_OK;
}

/**
* @brief Get trigger id argument for trigger operation from Avro value object
* @param[in]     arg_value         Avro value object
* @param[out]    ptrigger_id_data  Trigger ID data
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
zwp_status_t zwp_avi_ifttt_get_trigger_data_argument(avro_value_t *arg_value, zwp_avi_ifttt_trigger_get_t *ptrigger_id_data)
{
	char *string;
	avro_value_t  triggerdata_value;
	zwp_status_t res = ZWP_STATUS_ERROR;

	if (avro_value_get_by_name(arg_value, "triggerdata", &triggerdata_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'triggerdata' field %s", avro_strerror());
		goto l_err_get_triggerdata_field;
	}

	if (zw_avro_field_get_string(&triggerdata_value, "triggerid", &string) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'triggerid' data %s", avro_strerror());
		goto l_err_get_triggerid_value;
	}
	ZWP_LOG(ZWP_LOG_DEBUG, "triggerid: %s", string);

	strncpy(ptrigger_id_data->striggerID, string, strlen(string));
	zw_avro_field_free_string(string);

	ZWP_LOG(ZWP_LOG_DEBUG, "triggerid: %s", ptrigger_id_data->striggerID);

	if (zw_avro_field_get_int(&triggerdata_value, "limit", &ptrigger_id_data->iLimit) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get 'limit' data %s", avro_strerror());
		goto l_err_get_limit_value;
	}

	if (ptrigger_id_data->iLimit == -1)
		ptrigger_id_data->iLimit = IFTTT_DEFAULT_TRIGGER_RECORD_LIMIT;

	ZWP_LOG(ZWP_LOG_DEBUG, "limit: %d", ptrigger_id_data->iLimit);

	res = ZWP_STATUS_OK;

l_err_get_limit_value:
l_err_get_triggerid_value:
l_err_get_triggerdata_field:
	return res;
}
