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
 * @file    zws_avi_ifttt_common.c
 *
 * @brief   Common header file for IFTTT AVI messages.
 * @details Common header file for IFTTT AVI messages.
 *
 * @author  Angela
 *
 * @version 1.0 - 2016-08-04
 * - Initial version
 */
#include "zws_log.h"
#include "zws_error.h"
#include "zws_memory.h"
#include "zwu_encoding.h"
#include "zwu_time.h"
#include "zws_ifttt_common.h"
#include "zws_avi_ifttt_common.h"
#include "zws_ifttt_types.h"
#include "zws_cgi_common_networks.h"

 /**
 @brief Generate the device label in encoded format for IFTTT based on given info
 @param[in]	if_id	            Interface id
 @param[in]	node_id				Node id
 @param[in]	endpoint_id			Endpoint id
 @param[in]	name				Device name
 @param[in]	location			Device location
 @param[out] pGenLabel	        Pointer to generated label
 @return ZWS_STATUS_XXX
 @note Caller of this function needs to free pGenLabel if the funtion returns success
 */
zws_status_t zws_avi_generate_device_label(int32_t if_id,
											int32_t node_id,
											int32_t endpoint_id,
											char *name,
											char *location,
											char **pGenLabel)
{
	zws_status_t res = ZWS_STATUS_ERROR;
	char *pLabel = NULL;
	//char *pLabel_encoded = NULL;
	int iName_len = strlen(name);
	int iLoc_len = strlen(location);
	//size_t string_size;

	//Logic: "name" OR "name - location" OR
	// "location(Node x Endpoint x)" OR "Node x Endpoint X"

	if (iName_len > 0)
	{
		if (iLoc_len > 0)
		{
			if ((pLabel = (char *)ZWS_CALLOC(1, iName_len + iLoc_len + 10)) == NULL)
			{
				ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
				goto l_err_pLabel_alloc;
			}

			sprintf(pLabel, "%s - %s", name, location);
		}
		else
		{
			if ((pLabel = (char *)ZWS_CALLOC(1, iName_len + 1)) == NULL)
			{
				ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
				goto l_err_pLabel_alloc;
			}

			sprintf(pLabel, "%s", name);
		}
	}
	else
	{
		//No device name present
		if (iLoc_len > 0)
		{
			if ((pLabel = (char *)ZWS_CALLOC(1, iLoc_len + 50)) == NULL)
			{
				ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
				goto l_err_pLabel_alloc;
			}

			sprintf(pLabel, "%s (Node %d Endpoint %d)", location, node_id, endpoint_id);
		}
		else
		{
			if ((pLabel = (char *)ZWS_CALLOC(1, 50)) == NULL)
			{
				ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
				goto l_err_pLabel_alloc;
			}

			sprintf(pLabel, "Node %d Endpoint %d", node_id, endpoint_id);
		}
	}

	if (pLabel != NULL)
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "Device name label: %s", pLabel);

		//IFTTT seems doesn't need encoding

		//string_size = strlen(pLabel) * 3 + 1;
		//if ((pLabel_encoded = (char *)ZWS_CALLOC(1, string_size)) == NULL)
		//{
		//	ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		//	goto l_err_pLabel_encoded_alloc;
		//}

		//if (zwu_uri_encode_string(pLabel_encoded, string_size, pLabel) != ZWS_STATUS_OK)
		//{
		//	goto l_err_encode_pLabel;
		//}

		//ZWS_FREE(pLabel);

		//*pGenLabel = pLabel_encoded;

		*pGenLabel = pLabel;

		res = ZWS_STATUS_OK;
	}

	return res;

//l_err_encode_pLabel:
//	ZWS_FREE(pLabel_encoded);
//l_err_pLabel_encoded_alloc:
	ZWS_FREE(pLabel);
l_err_pLabel_alloc:
	return res;
}

/**
@brief Generate the device/interface value for IFTTT based on given info
@param[in]	if_id	            Interface id
@param[in]	node_id				Node id
@param[in]	endpoint_id			Endpoint id
@param[out]	pGenValue	        Pointer to generated label
@return ZWS_STATUS_XXX
@note Caller of this function needs to free pGenValue if the funtion returns success
*/
zws_status_t zws_avi_generate_device_value(int32_t if_id,
											int32_t node_id,
											int32_t endpoint_id,
											char **pGenValue)
{
	zws_status_t res = ZWS_STATUS_ERROR;
	unsigned long desc_id_temp;
	char *pIDValue = NULL;
	int length;

	desc_id_temp = ZWS_CGI_GET_NODE_DESC_ID((uint8_t)node_id);
	desc_id_temp = ZWS_CGI_GET_ENDPOINT_DESC_ID(desc_id_temp, (uint8_t)endpoint_id);
	desc_id_temp = ZWS_CGI_GET_INTERFACE_DESC_ID(desc_id_temp, (uint16_t)if_id);

	ZWS_LOG(ZWS_LOG_DEBUG, "Interface value: %lu", desc_id_temp);

	// Get the length of formatted string
	if ((length = snprintf(NULL, 0, "%lu", desc_id_temp)) < 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get length of formatted string");
		goto l_err_length;
	}

	if ((pIDValue = (char *)ZWS_MALLOC(length + 1)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_pIDValue_alloc;
	}

	sprintf(pIDValue, "%lu", desc_id_temp);

	*pGenValue = pIDValue;

	res = ZWS_STATUS_OK;

l_err_pIDValue_alloc:
l_err_length:
	return res;
}


/**
* @brief Get CGI response for IFTTT IF device name options API from Avro value for RPC response
* @param[in]     arg_value         Avro value for RPC response
* @param[in]     padditional_info  Additional information to aid to generate the result
* @param[out]    jsData            CGI response json data
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_avi_network_ifttt_device_name_option_result(avro_value_t *arg_value, void *padditional_info, zwu_json_t jsData)
{
	avro_value_t ifttt_device_list_value;
	avro_value_t ifttt_device_value;
	size_t count;
	int i;
	int32_t if_id, node_id, endpoint_id;
	char *name = NULL;
	char *location = NULL;
	char *pchLabel;
	char *pchValue;
	zwu_json_t jsDeviceObj = NULL;
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_SERVER;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);

	if (avro_value_get_by_name(arg_value, "ifttt_device_list", &ifttt_device_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'ifttt_device_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (avro_value_get_size(&ifttt_device_list_value, &count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get ifttt device list size");
		goto l_err_device_list_size;
	}

	if (count == 0)
	{
		//No such interface type exists in the entire network.
		//Return IFTTT with response with blank "data" array or return one element
		//with label "No such device in the network" and value "-1".
		ZWS_LOG(ZWS_LOG_ERR, "No supported device in the network. Return empty 'Data' array.");
	}

	for (i = 0; i < count; i++)
	{
		if (avro_value_get_by_index(&ifttt_device_list_value, i, &ifttt_device_value, NULL) != 0)
		{
			goto l_err_get_device_value;
		}

		if ((zw_avro_field_get_int(&ifttt_device_value, "id", &if_id) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(&ifttt_device_value, "node_id", &node_id) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(&ifttt_device_value, "endpoint_id", &endpoint_id) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_string(&ifttt_device_value, "name", &name) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_string(&ifttt_device_value, "location", &location) != ZWS_STATUS_OK)
			)
		{
			goto l_err_get_fields;
		}

		//Generate device name label
		if (zws_avi_generate_device_label(if_id, node_id, endpoint_id, name, location, &pchLabel) != ZWS_STATUS_OK)
		{
			goto l_err_generate_label;
		}

		//Generate device/interface value (descirptor ID in string)
		if (zws_avi_generate_device_value(if_id, node_id, endpoint_id, &pchValue) != ZWS_STATUS_OK)
		{
			goto l_err_generate_value;
		}

		jsDeviceObj = zwu_json_create_object();

		if (jsDeviceObj)
		{
			zwu_json_add_string_to_object(jsDeviceObj, "label", pchLabel);
			zwu_json_add_string_to_object(jsDeviceObj, "value", pchValue);

			zwu_json_add_item_to_array(jsData, jsDeviceObj);
		}


		ZWS_FREE(pchValue);
		ZWS_FREE(pchLabel);

		zw_avro_field_free_string(location);
		location = NULL;
		zw_avro_field_free_string(name);
		name = NULL;
	}

	res = ZWS_STATUS_OK;

	return res;

	ZWS_FREE(pchValue);
l_err_generate_value:
	ZWS_FREE(pchLabel);
l_err_generate_label:
l_err_get_fields:
	if (location)
		zw_avro_field_free_string(location);
	if (name)
		zw_avro_field_free_string(name);
l_err_get_device_value:
l_err_device_list_size:
l_err_get_list_value:
	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);
	return res;
}

/**
@brief Find the interface slug and field slug based on efeature
@param[in]	iTriggerActionFlag	IFTTT_REQUEST_TYPE_TRIGGER or IFTTT_REQUEST_TYPE_ACTION
@param[in]	efeature			Interface feature enum
@param[out]	pchInterfaceSlug	Pointer to Interface Slug
@param[out]	pGenValue	        Pointer to Field Slug
@return ZWS_STATUS_XXX
@note Caller of this function DO NOT need to free pchInterfaceSlug and pchFieldSlug if the funtion returns success
*/
zws_status_t zws_avi_ifttt_get_interfaceslug(int iTriggerActionFlag,
	eifttt_IF_feature_type efeature,
	char **pchInterfaceSlug,
	char **pchFieldSlug)
{
	//zws_status_t res = ZWS_STATUS_ERROR;
	//char *pInterfaceSlug = NULL, *pFieldSlug = NULL;
	ifttt_uri_detail_t *pCurrURI_detail = NULL;
	int i, count = 0;

	if (iTriggerActionFlag == IFTTT_REQUEST_TYPE_TRIGGER)
	{
		pCurrURI_detail = arIFTTT_Triggers;
		count = sizeof(arIFTTT_Triggers) / sizeof(ifttt_uri_detail_t);
	}
	else if (iTriggerActionFlag == IFTTT_REQUEST_TYPE_ACTION)
	{
		pCurrURI_detail = arIFTTT_Actions;
		count = sizeof(arIFTTT_Actions) / sizeof(ifttt_uri_detail_t);
	}

	for (i = 0; i < count; i++, pCurrURI_detail++)
	{
		if (pCurrURI_detail->eIFTTT_IF_feature == efeature &&
			pCurrURI_detail->eIFTTTField != eNo_fields)
		{
			*pchInterfaceSlug = (char*)pCurrURI_detail->sIFTTTInterfaceSlug;
			*pchFieldSlug = (char*)pCurrURI_detail->sIFTTTFieldSlug;

			ZWS_LOG(ZWS_LOG_DEBUG, "efeature found. Interface slug: %s Field slug: %s", *pchInterfaceSlug, *pchFieldSlug);
			return ZWS_STATUS_OK;
		}
	}

	return ZWS_STATUS_ERROR;
}

/**
* @brief Get CGI response for IFTTT Action set result API from Avro value for RPC response
* @param[in]     arg_value         Avro value for RPC response
* @param[in]     padditional_info  Additional information to aid to generate the result
* @param[out]    jsData            CGI response json data
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_avi_network_ifttt_action_set_result(avro_value_t *arg_value, void *padditional_info, zwu_json_t jsData)
{
	unsigned long *pcombo_id;
	int64_t utime;
	zwu_json_t jsIdObj = NULL;
	char arIDValue[100] = { 0 };
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_SERVER;
	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);

	pcombo_id = (unsigned long *)padditional_info;

	if (!pcombo_id || *pcombo_id == 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Invalid combo id");
		goto l_err_invalid_combo_id;
	}

	if (zw_avro_field_get_long(arg_value, "utime", &utime) != ZWS_STATUS_OK)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Fail to get utime.");
		goto l_err_get_fields;
	}

	ZWS_LOG(ZWS_LOG_DEBUG, "utime %llu", utime);

	jsIdObj = zwu_json_create_object();

	if (jsIdObj)
	{
		sprintf(arIDValue, "%lu-%llu", *pcombo_id, (unsigned long long)utime);

		zwu_json_add_string_to_object(jsIdObj, "id", arIDValue);

		zwu_json_add_item_to_array(jsData, jsIdObj);

		res = ZWS_STATUS_OK;
	}

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);
	return res;

	zwu_json_delete(jsIdObj);
l_err_get_fields:
l_err_invalid_combo_id:

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);
	return res;
}

/**
* @brief Set Avro value for RPC request for generic IFTTT trigger data using CGI request arguments
* @param[in, out]	param_value_p     avro API param value
* @param[in]		stifttt_uri       IFTTT URI request
* @param[in]		stifttt_args      IFTTT request parameters
* @retval			ZWS_STATUS_OK     Success
* @retval			ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_avi_network_ifttt_trigger_get_data_argument(avro_value_t *param_value_p, ifttt_uri_t stifttt_uri, ifttt_args_t stifttt_args)
{
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_SERVER;
	avro_value_t trigger_data_value;

	if ((stifttt_uri.iRequestType == IFTTT_REQUEST_TYPE_TRIGGER) &&
		(stifttt_args.pchTrigger_identity != NULL))
	{
		if (avro_value_get_by_name(param_value_p, "triggerdata", &trigger_data_value, NULL) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get 'triggerdata' field: %s", avro_strerror());
			goto l_err_get_trigger_avro;
		}

		if ((zw_avro_field_set_string(&trigger_data_value, "triggerid", stifttt_args.pchTrigger_identity) != ZWS_STATUS_OK) ||
			(zw_avro_field_set_int(&trigger_data_value, "limit", stifttt_args.iLimit) != ZWS_STATUS_OK))
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to set 'triggerid' or 'limit' field: %s", avro_strerror());
			goto l_err_set_trigger_id;
		}
	}

	res = ZWS_STATUS_OK;

l_err_set_trigger_id:
l_err_get_trigger_avro:
	return res;
}


/**
* @brief Get CGI response for IFTTT Trigger get API from Avro value for RPC response
* @param[in]     arg_value         Avro value for RPC response
* @param[in]     padditional_info  Additional information to aid to generate the result
* @param[out]    jsData            CGI response json data
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_avi_network_ifttt_trigger_get_result(avro_value_t *arg_value, void *padditional_info, zwu_json_t jsData)
{
	avro_value_t timestamp_list_value;
	avro_value_t timestamp_value;
	size_t count;
	int64_t utime;
	int i;
	unsigned long *pcombo_id;
	char arIDValue[100] = { 0 }, arDeviceName[150] = { 0 };
	char *pchValue;
	char *utime_iso8601 = NULL;
	zwu_json_t jsMatchedRecordObj = NULL;
	zwu_json_t jsMetaObj = NULL;
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_SERVER;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);

	pcombo_id = (unsigned long *)padditional_info;

	if (!pcombo_id || *pcombo_id == 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Invalid combo id");
		goto l_err_invalid_combo_id;
	}

	if ((pchValue = (char *)ZWS_MALLOC(sizeof(int64_t))) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_pchValue_alloc;
	}

	sprintf(pchValue, "%lu", *pcombo_id);

	if (zws_avi_interface_ifttt_get_device_name(*pcombo_id, arg_value, arDeviceName) != ZWS_STATUS_OK)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Fail to generate device name");
		goto l_err_generate_device_name;
	}

	if (avro_value_get_by_name(arg_value, "utime_array", &timestamp_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'utime_array': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (avro_value_get_size(&timestamp_list_value, &count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get timestamp list size");
		goto l_err_timestamp_list_size;
	}

	if (count == 0)
	{
		//No data has been saved in the DB.
		ZWS_LOG(ZWS_LOG_ERR, "No matched trigger data in the network. Return empty 'Data' array.");
	}

	for (i = 0; i < count; i++)
	{
		if (avro_value_get_by_index(&timestamp_list_value, i, &timestamp_value, NULL) != 0)
		{
			goto l_err_get_timestamp_value;
		}

		if (avro_value_get_long(&timestamp_value, &utime) != ZWS_STATUS_OK)
		{
			goto l_err_get_field_utime_value;
		}

		if (zwu_time_get_iso8601(utime, &utime_iso8601) != ZWU_STATUS_OK)
		{
			goto l_err_get_iso8601_time;
		}

		jsMetaObj = zwu_json_create_object();

		if (jsMetaObj)
		{
			sprintf(arIDValue, "%lu-%llu", *pcombo_id, (unsigned long long)utime);

			zwu_json_add_string_to_object(jsMetaObj, "id", arIDValue);

			zwu_json_add_number_to_object(jsMetaObj, "timestamp", utime);
		}

		jsMatchedRecordObj = zwu_json_create_object();

		if (jsMatchedRecordObj)
		{
			zwu_json_add_string_to_object(jsMatchedRecordObj, "created_at", utime_iso8601);

			zwu_json_add_string_to_object(jsMatchedRecordObj, "device_id", pchValue);

			zwu_json_add_string_to_object(jsMatchedRecordObj, IFTTT_REQUEST_BODY_IFTTT_DEVICE_NAME, arDeviceName);

			zwu_json_add_item_to_object(jsMatchedRecordObj, "meta", jsMetaObj);

			zwu_json_add_item_to_array(jsData, jsMatchedRecordObj);
		}

		ZWS_FREE(utime_iso8601);
		utime_iso8601 = NULL;
	}

	ZWS_FREE(pchValue);

	res = ZWS_STATUS_OK;

	return res;

	ZWS_FREE(utime_iso8601);
l_err_get_iso8601_time:
l_err_get_field_utime_value:
l_err_get_timestamp_value:
l_err_timestamp_list_size:
l_err_get_list_value:
l_err_generate_device_name:
	ZWS_FREE(pchValue);
l_err_pchValue_alloc:
l_err_invalid_combo_id:
	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);
	return res;
}

/**
* @brief Generate device name for IFTTT trigger get result based on combo_id and device name/loc from Avro response
* @param[in]     combo_id		   Combo id
* @param[in]     arg_value         Avro value for RPC response
* @param[out]    arDeviceName      Char array which points to the generated device name
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
zws_status_t zws_avi_interface_ifttt_get_device_name(unsigned long combo_id, avro_value_t *arg_value, char* arDeviceName)
{
	avro_value_t ifttt_device_name;
	char *name = NULL;
	char *location = NULL;
	char *pchLabel;
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_SERVER;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);

	if (avro_value_get_by_name(arg_value, "ifttt_interface_name", &ifttt_device_name, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'ifttt_interface_name': %s", avro_strerror());
		goto l_err_get_ifttt_interface_name;
	}

	if((zw_avro_field_get_string(&ifttt_device_name, "name", &name) != ZWS_STATUS_OK)
		|| (zw_avro_field_get_string(&ifttt_device_name, "location", &location) != ZWS_STATUS_OK))
	{
		goto l_err_get_fields;
	}

	//Generate device name label
	if (zws_avi_generate_device_label(ZWS_CGI_GET_INTERFACE_ID(combo_id), 
									  ZWS_CGI_GET_NODE_ID(combo_id), 
									  ZWS_CGI_GET_ENDPOINT_ID(combo_id), 
									  name, location, &pchLabel) != ZWS_STATUS_OK)
	{
		goto l_err_generate_label;
	}

	strcpy(arDeviceName, pchLabel);

	ZWS_FREE(pchLabel);

	zw_avro_field_free_string(location);
	zw_avro_field_free_string(name);

	res = ZWS_STATUS_OK;

	return res;

	
	ZWS_FREE(pchLabel);
l_err_generate_label:
l_err_get_fields:
	if (location)
		zw_avro_field_free_string(location);
	if (name)
		zw_avro_field_free_string(name);
l_err_get_ifttt_interface_name:
	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);
	return res;
}
