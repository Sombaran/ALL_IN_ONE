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
 * @file    zwp_avi_ifttt_msensor.c
 *
 * @brief   AVI message 'ifttt_msensor'
 * @details Handles the AVI message 'ifttt_msensor'.
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
#include "zwu_scale.h"
#include "zwp_network.h"
#include "zwp_descriptor.h"
#include "zwp_avi_ifttt_db_common.h"
#include "zwp_avi_interfaces_sensor_multilevel.h"
#include "zwp_command_class_type_define.h"

 /** msensor option argument */
typedef struct {
	int					type;
	eifttt_fields		efield;
} zwp_avi_ifttt_msensor_option_t;


/**
* @brief Allocate interface level data for IFTTT
* @param[in]	 desc_interface    Interface descriptor
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zwp_avi_interfaces_msensor_state_free_ifttt() to free resources
*/
zwp_status_t zwp_avi_interfaces_sensor_multilevel_state_alloc_ifttt(zwifd_p desc_interface)
{
	zwp_avi_interfaces_sensor_multilevel_state_t *state;
	pzwp_db_table	pifttt_generic_trigger_table;
	pzwp_db_table	pifttt_msensor_table;

	if ((state = (zwp_avi_interfaces_sensor_multilevel_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((pifttt_generic_trigger_table = zwp_avi_ifttt_generic_trigger_table_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get generic table");
		goto l_err_get_generic_table;
	}

	if ((pifttt_msensor_table = zwp_avi_ifttt_cc_table_get(desc_interface, COMMAND_CLASS_SENSOR_MULTILEVEL)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get msensor table");
		goto l_err_get_cc_table;
	}

	state->pifttt_generic_trigger_table = pifttt_generic_trigger_table;
	state->pifttt_msensor_table = pifttt_msensor_table;
	//state->ifttt_msensor_get_result.putimeList = NULL;

	if (zwu_array_list_create(0, &state->ifttt_msensor_get_result.sensor_result_list) != ZWP_STATUS_OK)
	{
		goto l_err_get_result_list_create;
	}

	return ZWP_STATUS_OK;

	zwu_array_list_destroy(state->ifttt_msensor_get_result.sensor_result_list);
l_err_get_result_list_create:
	zwp_db_close_table(state->pifttt_msensor_table);
	state->pifttt_msensor_table = NULL;
l_err_get_cc_table:
l_err_get_generic_table:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free interface level data
* @param[in]     state               Interface level data
* @return        Nothing
* @see zwp_avi_interfaces_sensor_multilevel_state_alloc_ifttt
*/
void zwp_avi_interfaces_msensor_state_free_ifttt(zwp_avi_interfaces_sensor_multilevel_state_t *state)
{
	zwp_avi_interfaces_sensor_multilevel_ifttt_get_result_free_ifttt(&state->ifttt_msensor_get_result);

	zwu_array_list_destroy(state->ifttt_msensor_get_result.sensor_result_list);

	state->pifttt_generic_trigger_table = NULL;

	zwp_db_close_table(state->pifttt_msensor_table);
	state->pifttt_msensor_table = NULL;
}

/**
* @brief Free ifttt trigger get result
* @param[in]     pifttt_msensor_get_result      Pointer to IFTTT get result struct
* @return        Nothing
*/
void zwp_avi_interfaces_sensor_multilevel_ifttt_get_result_free_ifttt(zwp_avi_ifttt_sensor_multilevel_get_result_t *pifttt_msensor_get_result)
{
	int get_result_count, i;

	zwp_avi_ifttt_sensor_multilevel_get_record_single_t *pGet_result_single;

	get_result_count = zwu_array_list_size(pifttt_msensor_get_result->sensor_result_list);

	for (i = 0; i < get_result_count; i++)
	{
		pGet_result_single = zwu_array_list_get(pifttt_msensor_get_result->sensor_result_list, i);

		ZWP_FREE(pGet_result_single);
	}

	zwu_array_list_remove_all(pifttt_msensor_get_result->sensor_result_list);
}

 /**
 * @brief Get argument for msensor option operation from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Node Add operation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_networks_node_add_argument_free() to free resources
 */
static zwp_status_t zwp_avi_ifttt_msensor_option_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_ifttt_msensor_option_t *argument;
	int32_t ifield;

	if ((argument = (zwp_avi_ifttt_msensor_option_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "type", &argument->type) != ZWP_STATUS_OK)
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
* @brief Free arguments for msensor option operation
* @param[in]     args              Arguments passed with Remove Failed Node operation
* @return        Nothing
* @see zwp_avi_networks_failed_node_remove_argument
*/
static void zwp_avi_ifttt_msensor_option_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
 * @brief Execute call to msensor option operation status
 * @param[in]     net               Network object
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 *@note
 * @par
 * The caller should call zwp_avi_ifttt_msensor_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_ifttt_msensor_option_execute(zwp_net_t net, void *arg, void **result_p)
{
	//zwp_status_t res = ZWP_STATUS_ERROR;
	zwp_avi_ifttt_msensor_option_t *argument;
	zwp_desc_net_t desc_net;
	zwu_array_list_t if_ep_name_list;
	uint16_t interfaceid = COMMAND_CLASS_SENSOR_MULTILEVEL;
	zwp_desc_IF_ep_name_t  *pstIF_ep_name;
	int i;

	argument = (zwp_avi_ifttt_msensor_option_t *)arg;

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

		ZWP_LOG(ZWP_LOG_DEBUG, "Found %d devices with class id: %d ", zwu_array_list_size(if_ep_name_list), COMMAND_CLASS_SENSOR_MULTILEVEL);


		//Check whether the thermostat mode device has support Mode or not
		//iSize = zwu_array_list_size(if_ep_name_list);

		for (i = 0; i < zwu_array_list_size(if_ep_name_list); i++)
		{
			pstIF_ep_name = zwu_array_list_get(if_ep_name_list, i);

			if (pstIF_ep_name)
			{
				if (!zwp_ifttt_interface_has_supported_msensor_type(pstIF_ep_name->desc_interface, argument->type))
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
 * @see zwp_avi_ifttt_msensor_get_execute
 */
static void zwp_avi_ifttt_msensor_option_result_free(void *result)
{
	zwp_desc_free_if_ep_name_list(result);
}

/**
* @brief Get argument for msensor set operation from Avro value object
* @param[in]     arg_value         Avro value object
* @param[out]    args_p            Arguments passed with Node Add operation
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zwp_avi_networks_node_add_argument_free() to free resources
*/
static zwp_status_t zwp_avi_ifttt_msensor_get_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_ifttt_sensor_multilevel_get_t *argument;
	//int type, ;
	char *string = NULL;

	if ((argument = (zwp_avi_ifttt_sensor_multilevel_get_t *)ZWP_CALLOC(1, sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zwp_avi_ifttt_get_trigger_data_argument(arg_value, &argument->trigger_id_data) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Fail to get trigger data argument");
		goto l_err_get_trigger_data;
	}

	if ((zw_avro_field_get_int(arg_value, "type", &argument->type) != ZWP_STATUS_OK)
		|| (zw_avro_field_get_int(arg_value, "comparision_command", &argument->comp_command) != ZWP_STATUS_OK)
		|| (zw_avro_field_get_string(arg_value, "thld_value", &string) != ZWP_STATUS_OK)
		|| (zw_avro_field_get_int(arg_value, "thld_unit", &argument->thld_unit) != ZWP_STATUS_OK)
		)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Fail to get avro value");
		goto l_err_get_avro_value;
	}

	strncpy(argument->cArrThld_value, string, sizeof(argument->cArrThld_value));
	zw_avro_field_free_string(string);

	ZWP_LOG(ZWP_LOG_DEBUG, "type: %d, comparision_command: %d, thld_value %s, thld_unit: %d", 
		argument->type, argument->comp_command, argument->cArrThld_value, argument->thld_unit);

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_get_avro_value:
	if (string)
		zw_avro_field_free_string(string);
l_err_get_trigger_data:

	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free arguments for msensor get operation
* @param[in]     args              Arguments passed with msensor get operation
* @return        Nothing
* @see zwp_avi_networks_failed_node_remove_argument
*/
static void zwp_avi_ifttt_msensor_get_argument_free(void *args)
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
static zwp_status_t zwp_avi_ifttt_msensor_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_ifttt_sensor_multilevel_get_t *argument;
	zwp_avi_interfaces_sensor_multilevel_state_t *state;
	int		i, index;
	int iRecordUpdated = 0;
	int iNumRecordFound, iNumColPerRecord;
	pzwp_db_table_value pMatchedTableValue = NULL;
	ifttt_generic_table_device_data device_data;
	zwp_avi_ifttt_sensor_multilevel_get_record_single_t *pGet_record_single = NULL;

	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_START, NULL);

	//IFTTT Get trigger
	//check/update trigger arguments inside the generic trigger table
	//If trigger argument not exists in generic trigger table, insert it.
	//Query msensor table with the specifc trigger ID data

	argument = (zwp_avi_ifttt_sensor_multilevel_get_t *)args;

	if ((state = (zwp_avi_interfaces_sensor_multilevel_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	{
		//Update generic trigger table for the trigger ID.
		//If not exists, create the record.

		sprintf(device_data.sState, "type=%d&comp_command=%d&thld_value=%s&thld_unit=%d", 
			argument->type, 
			argument->comp_command,
			argument->cArrThld_value,
			argument->thld_unit);
		device_data.iParam_size = 0;
		device_data.pParam = 0;

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
				IFTTT_DB_TABLE_NAME_MSENSOR) != ZWP_STATUS_OK)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to insert record to generic table");
				goto l_err_insert_generic_table;
			}
		}
	}


	//Query record from CC table based on trigger ID and sort by timestamp
	if (zwp_avi_ifttt_query_trigger_id_from_cc_table(
		state->pifttt_msensor_table,
		argument->trigger_id_data,
		&iNumRecordFound,
		&iNumColPerRecord,
		&pMatchedTableValue) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to query record from cc table. Error: %s", zwp_db_get_table_errortext(state->pifttt_msensor_table));
		goto l_err_query_cc_table;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "%d record found in cc table.", iNumRecordFound);

	//Save query record to state
	zwp_avi_interfaces_sensor_multilevel_ifttt_get_result_free_ifttt(&state->ifttt_msensor_get_result);

	for (i = 0; i < iNumRecordFound; i++)
	{
		if ((pGet_record_single = (zwp_avi_ifttt_sensor_multilevel_get_record_single_t *)ZWP_CALLOC(1, sizeof(zwp_avi_ifttt_sensor_multilevel_get_record_single_t))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_get_result_alloc;
		}

		index = iNumColPerRecord * i;

		if (strcmp(pMatchedTableValue[index + MSENSOR_TABLE_INDEX_TYPE].pcColumnName, TABLE_COL_NAME_TYPE))
		{
			//Column name is not 'type'
			ZWP_LOG(ZWP_LOG_ERR, "Wrong column format");
			goto l_err_table_data_format;
		}

		pGet_record_single->type = pMatchedTableValue[index + MSENSOR_TABLE_INDEX_TYPE].data.valueint;

		strncpy(pGet_record_single->cchSnsr_value, pMatchedTableValue[index + MSENSOR_TABLE_INDEX_VALUE].data.valuestring,
			pMatchedTableValue[index + MSENSOR_TABLE_INDEX_VALUE].isize);

		pGet_record_single->unit = pMatchedTableValue[index + MSENSOR_TABLE_INDEX_UNIT].data.valueint;
		pGet_record_single->utime = pMatchedTableValue[index + MSENSOR_TABLE_INDEX_TIMESTAMP].data.valueint64;

		strcpy(pGet_record_single->cchSnsr_unit, arMSensorUnit[pGet_record_single->type][pGet_record_single->unit]);

		zwu_array_list_add(state->ifttt_msensor_get_result.sensor_result_list, pGet_record_single);
	}

	zwp_db_query_record_free(iNumColPerRecord, iNumRecordFound, pMatchedTableValue);

	*hcapi_code_p = ZW_ERR_QUEUED;	//Cannot use ZW_ERR_NONE else the frame work will wait for the transmit status

	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_END, ZWP_STATUS_OK);

	return ZWP_STATUS_OK;

l_err_table_data_format:
	if(pGet_record_single)
		ZWP_FREE(pGet_record_single);
l_err_get_result_alloc:
	zwp_avi_interfaces_sensor_multilevel_ifttt_get_result_free_ifttt(&state->ifttt_msensor_get_result);
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
static zwp_status_t zwp_avi_ifttt_msensor_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_START, NULL);

	zwp_avi_interfaces_sensor_multilevel_state_t *state;
	zwp_avi_ifttt_sensor_multilevel_get_result_t *get_result;
	zwp_avi_ifttt_sensor_multilevel_get_record_single_t *per_record, *per_result_record;
	zw_nameloc_t EpNameLoc = {{0}};
	int	i, count;

	if ((state = (zwp_avi_interfaces_sensor_multilevel_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((get_result = (zwp_avi_ifttt_sensor_multilevel_get_result_t *)ZWP_CALLOC(1, sizeof(*get_result))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_get_result_alloc;
	}

	if (zwu_array_list_create(0, &get_result->sensor_result_list) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_get_result_list_create;
	}

	if (zwp_desc_get_endpoint_nameloc_from_interface(desc_interface, &EpNameLoc) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Fail to get endpoint name/loc.");
		goto l_err_get_ep_name_loc;
	}

	strcpy(get_result->name, EpNameLoc.name);
	strcpy(get_result->loc, EpNameLoc.loc);

	count = zwu_array_list_size(state->ifttt_msensor_get_result.sensor_result_list);

	for (i = 0; i < count; i++)
	{
		per_record = (zwp_avi_ifttt_sensor_multilevel_get_record_single_t*)zwu_array_list_get(state->ifttt_msensor_get_result.sensor_result_list, i);
	
		if (per_record)
		{
			if ((per_result_record = (zwp_avi_ifttt_sensor_multilevel_get_record_single_t *)ZWP_CALLOC(1, sizeof(zwp_avi_ifttt_sensor_multilevel_get_record_single_t))) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				goto l_err_per_result_record_alloc;
			}

			memcpy(per_result_record, per_record, sizeof(zwp_avi_ifttt_sensor_multilevel_get_record_single_t));

			zwu_array_list_add(get_result->sensor_result_list, per_result_record);
		}
	}

	*result_p = get_result;

	ZWP_LOG_EX(ZWP_LOG_FUNCTION, FUNC_LOG_END, ZWP_STATUS_OK);

	return ZWP_STATUS_OK;

	zwp_avi_interfaces_sensor_multilevel_ifttt_get_result_free_ifttt(get_result);
	ZWP_FREE(per_result_record);
l_err_per_result_record_alloc:
l_err_get_ep_name_loc:
	zwu_array_list_destroy(get_result->sensor_result_list);
l_err_get_result_list_create:
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
static zwp_status_t zwp_avi_ifttt_msensor_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_ifttt_sensor_multilevel_get_result_t *get_result;
	zwp_avi_ifttt_sensor_multilevel_get_record_single_t *per_record;
	avro_value_t msensor_result_list_value;
	avro_value_t msensor_result_value;
	int i, count;

	get_result = (zwp_avi_ifttt_sensor_multilevel_get_result_t *)result;

	if (zwp_avi_ifttt_get_result_assign_ep_name_loc(get_result->name, get_result->loc, arg_value) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to set endpoint name/loc to avro");
		goto l_err_set_name_loc_value;
	}

	if (avro_value_get_by_name(arg_value, "ifttt_msensor_get_result_array", &msensor_result_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'ifttt_msensor_get_result_array': %s", avro_strerror());
		goto l_err_get_msensor_result_list_value_value;
	}

	count = zwu_array_list_size(get_result->sensor_result_list);

	for (i = 0; i < count; i++)
	{
		per_record = (zwp_avi_ifttt_sensor_multilevel_get_record_single_t*)zwu_array_list_get(get_result->sensor_result_list, i);

		if (per_record)
		{
			if (avro_value_append(&msensor_result_list_value, &msensor_result_value, 0) != 0)
			{
				goto l_err_append_msensor_result_value;
			}

			if ((zw_avro_field_set_string(&msensor_result_value, "sensor_value", per_record->cchSnsr_value) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_string(&msensor_result_value, "sensor_unit", per_record->cchSnsr_unit) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_long(&msensor_result_value, "utime", per_record->utime) != ZWP_STATUS_OK))
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to get the msensor_result_value: %s", avro_strerror());
				goto l_err_set_msensor_result_value;
			}
		}
	}

	return ZWP_STATUS_OK;

l_err_set_msensor_result_value:
l_err_append_msensor_result_value:
l_err_get_msensor_result_list_value_value:
l_err_set_name_loc_value:
	return ZWP_STATUS_ERROR;
}


/**
* @brief Free Result object corresponding to Report command
* @param[in]     result            Result object
* @return        Nothing
* @see zwp_avi_interfaces_switch_binary_get_execute_passive
*/
static void zwp_avi_ifttt_msensor_get_result_free(void *result)
{
	zwp_avi_ifttt_sensor_multilevel_get_result_t *get_result = result;

	if (get_result)
	{
		zwp_avi_interfaces_sensor_multilevel_ifttt_get_result_free_ifttt(get_result);

		zwu_array_list_destroy(get_result->sensor_result_list);

		ZWP_FREE(get_result);
	}
}

void zwp_avi_interfaces_msensor_find_cached_data(zwp_avi_interfaces_sensor_multilevel_report_t *report,
												 zwsensor_p pcurr_sensor_data, 
												 zwsensor_p pcached_sensor_data,
												 zwu_time_t *pcached_ts)
{
	int sensor_value_count, i;
	zwp_avi_interfaces_sensor_multilevel_report_sensor_value_t *sensor_value;

	sensor_value_count = zwu_array_list_size(report->sensor_value_list);

	for (i = 0; i < sensor_value_count; i++)
	{
		sensor_value = zwu_array_list_get(report->sensor_value_list, i);

		//Find cached sensor report of the same sensor type (Temperature) / same sensor type same unit (Luminance, humility)
		if (sensor_value->value.type == pcurr_sensor_data->type)
		{
			if (pcurr_sensor_data->type == ZW_SENSOR_TYPE_TEMP)
			{
				memcpy(pcached_sensor_data, &sensor_value->value, sizeof(zwsensor_t));
				*pcached_ts = sensor_value->utime;
				break;
			}
			else if (pcurr_sensor_data->unit == sensor_value->value.unit)
			{
				memcpy(pcached_sensor_data, &sensor_value->value, sizeof(zwsensor_t));
				*pcached_ts = sensor_value->utime;
				break;
			}
		}
	}
}


/**
* @brief HCAPI report handler for IFTTT trigger
* @param[in]	  desc_interface  interface descriptor
* @param[in]	  msensor_info	   current value and unit of the sensor
* @param[in]	  ts			  timestamp
*/
void zwp_avi_interfaces_msensor_report_handler_ifttt(zwifd_p desc_interface, zwsensor_p pcurr_sensor_data, time_t ts)
{
	zwp_avi_interfaces_sensor_multilevel_state_t *state;
	zwsensor_t cached_sensor_data = { 0 };
	double dCurr_sensor_value = 0.0, dCached_sensor_value = 0.0, dCurr_value_convert, dCached_value_convert;
	double dgeneric_record_value = 0.0;
	//ifttt_generic_table_device_data stQueryCond;
	int count, i;
	zwu_time_t cached_ts = 0;
	zwu_array_list_t genericTableMSensorList;
	zwp_avi_ifttt_sensor_multilevel_get_t *pSingleResult = NULL;

	if (!pcurr_sensor_data) return;

	//Steps: 
	//1. Find cached sensor values of same type (and same unit) of the current value
	//2. If new report timestamp is same or older than cached timestamp, return
	//3. Find msensor records in generic table according to desc_interface
	//4. Convert msensor records found in generic table to zwp_avi_ifttt_sensor_multilevel_get_t
	//	 and filter records based on sensor type (and sensor unit)
	//5. Convert cached data and current sensor value unit to generic table record unit (only for temperature)
	//6. Compare generic table msensor record with cached and current sensor data
	//   a. If cached data is NULL, new sensor report must fulfill the generic table record condition
	//	 b. If cached data is not NULL, cached data MUST not fulfill the generic table record condition AND
	//	    new report data MUST fulfill the generic record condition
	//7. If comparison result passes, set the current data to specfic cc table (may need to set multiple times with different trigger ID)

	if ((state = (zwp_avi_interfaces_sensor_multilevel_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	//Find cached sensor report of the same sensor type (Temperature) / same sensor type same unit (Luminance, humility)
	zwp_avi_interfaces_msensor_find_cached_data(&state->report, pcurr_sensor_data, &cached_sensor_data, &cached_ts);

	//If new data timestamp is same or older than cached data timestamp, return
	if (ts <= cached_ts)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "New data has same or older timestamp than cached data. Ignore.");
		goto l_success;
	}

	if (zwu_array_list_create(0, &genericTableMSensorList) != ZWP_STATUS_OK)
	{
		goto l_err_msensor_list_create;
	}

	//Find msensor records in generic table according to desc_interface, type (and unit)
	if(zwp_avi_ifttt_generic_table_report_msensor_query(state->pifttt_generic_trigger_table, 
		desc_interface,
		pcurr_sensor_data->type,
		pcurr_sensor_data->unit,
		genericTableMSensorList
		) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Find matched record in generic table fail. ");
		goto l_err_find_matched_report;
	}

	count = zwu_array_list_size(genericTableMSensorList);

	ZWP_LOG(ZWP_LOG_DEBUG, "Found %d matched record in generic table of type %d.", count, pcurr_sensor_data->type);

	if (count == 0)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "No matched record found in generic talbe. Return.");
		goto l_success_free;
	}

	//Convert cached sensor data and current sensor data to double type
	if (cached_ts)
	{
		if (zwh_interface_sensor_value_to_double(&cached_sensor_data, &dCached_sensor_value, NULL) != ZWP_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to convert cached sensor data to double");
			goto l_err_convert_cached_data_double;
		}
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "cached_ts = %ld,  dCached_sensor_value = %f", cached_ts, dCached_sensor_value);

	if (zwh_interface_sensor_value_to_double(pcurr_sensor_data, &dCurr_sensor_value, NULL) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to convert current sensor data to double");
		goto l_err_convert_current_data_double;
	}

	//For each record found
	for (i = 0; i < count; i++)
	{
		pSingleResult = zwu_array_list_get(genericTableMSensorList, i);
		dCurr_value_convert = dCurr_sensor_value;
		dCached_value_convert = dCached_sensor_value;

		if (zwu_parse_double(pSingleResult->cArrThld_value, &dgeneric_record_value) != ZWU_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to parse generic record data to double %s", pSingleResult->cArrThld_value);
			continue;
		}

		//Convert cached data and current sensor value unit to generic table record unit(only for temperature)
		if (pcurr_sensor_data->type == ZW_SENSOR_TYPE_TEMP)
		{
			//Unit conversion
			if (pcurr_sensor_data->unit != pSingleResult->thld_unit)
			{
				if (pSingleResult->thld_unit == ZW_SENSOR_UNIT_TEMP_CEL)
				{
					zwu_scale_fahrenheit_to_celsius(dCurr_value_convert, &dCurr_value_convert);
				}
				else
				{
					zwu_scale_celsius_to_fahrenheit(dCurr_value_convert, &dCurr_value_convert);
				}
			}

			if (cached_ts && (cached_sensor_data.unit != pSingleResult->thld_unit))
			{
				if (pSingleResult->thld_unit == ZW_SENSOR_UNIT_TEMP_CEL)
				{
					zwu_scale_fahrenheit_to_celsius(dCached_value_convert, &dCached_value_convert);
				}
				else
				{
					zwu_scale_celsius_to_fahrenheit(dCached_value_convert, &dCached_value_convert);
				}
			}
		}

		ZWP_LOG(ZWP_LOG_DEBUG, "dCurr_value_convert = %f, dCached_value_convert = %f, dgeneric_record_value = %f, comp_command = %s",
			dCurr_value_convert, dCached_value_convert, dgeneric_record_value,
			(pSingleResult->comp_command == ZW_SENSOR_COMPARISON_COMMAND_LT) ? "Less than" : "Greater than" );

		//Compare generic table msensor record with cached and current sensor data
		if (cached_ts)
		{
			//cached data MUST not fulfill the generic table record condition
			if (pSingleResult->comp_command == ZW_SENSOR_COMPARISON_COMMAND_LT)
			{
				if (zwu_number_double_definitely_greater(dgeneric_record_value, dCached_value_convert))
				{
					ZWP_LOG(ZWP_LOG_DEBUG, "Cached value not fulfill condition. Try next record.");
					continue;
				}
			}
			else if (pSingleResult->comp_command == ZW_SENSOR_COMPARISON_COMMAND_GT)
			{
				if (zwu_number_double_definitely_greater(dCached_value_convert, dgeneric_record_value))
				{
					ZWP_LOG(ZWP_LOG_DEBUG, "Cached value not fulfill condition. Try next record.");
					continue;
				}
			}
		}

		//Compare current value
		if (pSingleResult->comp_command == ZW_SENSOR_COMPARISON_COMMAND_LT)
		{
			if (zwu_number_double_definitely_greater(dCurr_value_convert, dgeneric_record_value) 
				|| zwu_number_double_nearly_equal(dCurr_value_convert, dgeneric_record_value))
			{
				ZWP_LOG(ZWP_LOG_DEBUG, "Current value not fulfill condition. Try next record.");
				continue;
			}
		}
		else if (pSingleResult->comp_command == ZW_SENSOR_COMPARISON_COMMAND_GT)
		{
			if (zwu_number_double_definitely_greater(dgeneric_record_value, dCurr_value_convert)
				|| zwu_number_double_nearly_equal(dCurr_value_convert, dgeneric_record_value))
			{
				ZWP_LOG(ZWP_LOG_DEBUG, "Current value not fulfill condition. Try next record.");
				continue;
			}
		}

		ZWP_LOG(ZWP_LOG_DEBUG, "Match found. Try to insert into CC table.");

		if (zwp_avi_ifttt_cc_table_insert(state->pifttt_msensor_table,
			desc_interface,
			pcurr_sensor_data,
			COMMAND_CLASS_SENSOR_MULTILEVEL,
			(zwu_time_t)ts,
			pSingleResult->trigger_id_data.striggerID) != ZWP_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_DEBUG, "Failed to insert record in cc table. ");
			goto l_err_cc_table_insert;
		}

	}


l_success_free:
	zwp_avi_interfaces_msensor_free_msensor_get_list(genericTableMSensorList);

l_success:
	return;

l_err_cc_table_insert:
	l_err_convert_current_data_double:
	l_err_convert_cached_data_double:
l_err_find_matched_report:
	zwp_avi_interfaces_msensor_free_msensor_get_list(genericTableMSensorList);
l_err_msensor_list_create:
l_err_get_state:
	return;
}

/**
* @brief Convert Z-Ware Lib sensor value to double value
* @param[in]     sensor_value      Z-Ware Lib sensor value
* @param[out]    sensor_value_double_p      Double value
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
zwp_status_t zwh_interface_sensor_value_to_double(zwsensor_t *sensor_value, double *sensor_value_double_p, int *sensor_value_precision_p)
{
	uint8_t *sensor_value_data;
	size_t sensor_value_size;
	int precision;
	int size;
	int32_t sensor_value_significand;
	double sensor_value_double;
	int j;

	sensor_value_data = sensor_value->data;
	sensor_value_size = sizeof(sensor_value->data);
	precision = sensor_value->precision;
	size = sensor_value->size;

	// Size validation to prevent underflow/overlfow
	if ((sensor_value_size != 4) || (size < 0) || (size > sensor_value_size) || (size == 3))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Sensor value field size");
		goto l_err_invalid_sensor_value_size;
	}

	sensor_value_significand = 0;
	for (j = size - 1; j >= 0; j--)
	{
		if (j > 0)
		{
			sensor_value_significand |= *((uint8_t *)sensor_value_data + j) << (8 * (size - 1 - j));
		}
		else
		{
			sensor_value_significand |= *((int8_t *)sensor_value_data + j) << (8 * (size - 1 - j));
		}
	}

	sensor_value_double = (double)sensor_value_significand;

	for (j = 0; j < precision; j++)
	{
		sensor_value_double /= 10.0;
	}

	*sensor_value_double_p = sensor_value_double;

	if (sensor_value_precision_p)
	{
		*sensor_value_precision_p = precision;
	}

	return ZWP_STATUS_OK;

l_err_invalid_sensor_value_size:
	return ZWP_STATUS_ERROR;
}

/** Handlers for getting msensor option */
static zwp_avi_message_networks_t ifttt_msensor_option =
{
	"ifttt_network_msensor_option",
	zwp_avi_ifttt_msensor_option_argument, zwp_avi_ifttt_msensor_option_argument_free,
	zwp_avi_ifttt_msensor_option_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
	zwp_avi_ifttt_device_name_option_result, zwp_avi_ifttt_msensor_option_result_free
};

/** Handlers for msensor Get command (interface leel) */
static zwp_avi_message_interfaces_t ifttt_interfaces_msensor_get =
{
	"ifttt_interface_msensor_get",
	COMMAND_CLASS_SENSOR_MULTILEVEL, NULL,
	NULL, NULL,
	zwp_avi_ifttt_msensor_get_argument, zwp_avi_ifttt_msensor_get_argument_free,
	NULL,
	zwp_avi_ifttt_msensor_get_execute_active, zwp_avi_ifttt_msensor_get_execute_passive,
	zwp_avi_ifttt_msensor_get_result, zwp_avi_ifttt_msensor_get_result_free
};

/** Register handlers */
zwp_status_t zwp_avi_ifttt_msensor_init(zwu_array_list_t network_message_list, zwu_array_list_t interface_message_list)
{
	if ((zwp_avi_message_list_add_message_networks(network_message_list, &ifttt_msensor_option) != ZWP_STATUS_OK)
		||(zwp_avi_message_list_add_message_interfaces(interface_message_list, &ifttt_interfaces_msensor_get))
		)
	{
		goto l_err_msensor_option;
	}

	return ZWP_STATUS_OK;

l_err_msensor_option:
	return ZWP_STATUS_ERROR;
}

