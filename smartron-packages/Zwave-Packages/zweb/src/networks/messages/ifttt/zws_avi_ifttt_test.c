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
 * @file    zws_avi_ifttt_test.c
 *
 * @brief   AVI message 'ifttt_test'
 * @details Handles the AVI message 'ifttt_test'.
 *
 * @author  Angela
 *
 * @version 1.0 - 2016-10-17
 * - Initial version
 */

#include "zws_status.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zws_ifttt_common.h"
#include "zws_ifttt_response.h"
#include "zws_avi_ifttt_common.h"
#include "zws_avi_message_ifttt.h"
#include "zws_exec_php_script.h"


/**
* @brief Set Avro value for RPC request for test setup command using CGI request arguments
* @param[in]		stifttt_uri       IFTTT URI request
* @param[in]		stifttt_args      IFTTT request parameters
* @param[in, out]   arg_value_p       Avro value for RPC request
* @retval			ZWS_STATUS_OK     Success
* @retval			ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_network_ifttt_test_setup_argument(ifttt_uri_t stifttt_uri, ifttt_args_t stifttt_args, avro_value_t *arg_value_p)
{
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_SERVER;
	avro_value_t eifttt_feature_list_value;
	avro_value_t eifttt_feature_value;
	int32_t ifeature_type;
	eifttt_IF_feature_type efeature_type;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);

	if (avro_value_get_by_name(arg_value_p, "eifttt_feature_list", &eifttt_feature_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'eifttt_feature_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	//Send triggers eifttt_feature
	for (efeature_type = eTrigger_motionAlarm_Detected; efeature_type < eTrigger_IF_feature_all; efeature_type++)
	{
		if (avro_value_append(&eifttt_feature_list_value, &eifttt_feature_value, 0) != 0)
		{
			goto l_err_append_eifttt_trigger_feature_value;
		}

		ifeature_type = (int32_t)efeature_type;
		if (avro_value_set_int(&eifttt_feature_value, ifeature_type) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to set value '%d' to the field eifttt_feature_list : %s",
				ifeature_type, avro_strerror);
			goto l_err_set_eifttt_trigger_feature_value;
		}
	}

	//Send actions eifttt_feature
	for (efeature_type = eAction_bSwitch_On; efeature_type < eAction_IF_feature_all; efeature_type++)
	{
		if (avro_value_append(&eifttt_feature_list_value, &eifttt_feature_value, 0) != 0)
		{
			goto l_err_append_eifttt_action_feature_value;
		}

		ifeature_type = (int32_t)efeature_type;
		if (avro_value_set_int(&eifttt_feature_value, ifeature_type) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to set value '%d' to the field eifttt_feature_list : %s",
				ifeature_type, avro_strerror);
			goto l_err_set_eifttt_action_feature_value;
		}
	}

	res = ZWS_STATUS_OK;

l_err_set_eifttt_action_feature_value:
l_err_append_eifttt_action_feature_value:
l_err_set_eifttt_trigger_feature_value:
l_err_append_eifttt_trigger_feature_value:
l_err_get_list_value:

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);
	return res;
}

static zws_status_t zws_avi_networks_ifttt_test_setup_get_user_accesstoken(char *pchUserID, char **ppchAccessToken)
{
	zws_status_t res = ZWS_STATUS_ERROR;
	int iRespLength;
	int iExitCode;

	if (pchUserID && ppchAccessToken)
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "Ger user accessToken user id: '%s'", pchUserID);

		if (zws_exec_php_request(GET_USER_INFO_SCRIPT_FILE, 3, PHP_SCRIPT_INPUT_USER_ID, pchUserID, PHP_SCRIPT_INPUT_ACCESS_TOKEN,
			ppchAccessToken, &iRespLength, &iExitCode) == ZWS_STATUS_OK)
		{
			if (iExitCode == PHP_SCRIPT_SUCCESS && (*ppchAccessToken) != NULL)
			{
				ZWS_LOG(ZWS_LOG_DEBUG, "Ger user AccessToken response: %s", *ppchAccessToken);

				res = ZWS_STATUS_OK;

			}
			else if (iExitCode != PHP_SCRIPT_SUCCESS)
			{
				ZWS_LOG(ZWS_LOG_ERR, "Ger user AccessToken fail. Exit code: %d", iExitCode);
			}
		}
	}

	return res;
}

/**
* @brief Get CGI response for gateway list get from Avro value for RPC response
* @param[in]     arg_value         Avro value for RPC response
* @param[in]     padditional_info  Additional information to aid to generate the result
* @param[out]    jsData            CGI response json data
* @retval        ZWS_STATUS_OK     Success
* @retval        ZWS_STATUS_ERROR  Failure
*/
static zws_status_t zws_avi_network_ifttt_test_setup_result(avro_value_t *arg_value, void *padditional_info, zwu_json_t jsData)
{
	avro_value_t ifttt_device_list_value, ifttt_device_list_info_list_value;
	avro_value_t ifttt_device_feature_value, ifttt_device_list_info_value;
	char *user = padditional_info;
	char *strAccessToekn = NULL;
	zwu_json_t jsSampleObj = NULL, jsTriggerObj = NULL, jsActionObj = NULL, jsCurrObj = NULL;
	zwu_json_t jsInterfaceObj = NULL;
	char *pchInterfaceSlug, *pchFieldSlug;
	int iTriggerActionFlag;
	size_t feature_device_list_count, if_ep_name_list_count;;
	int i, j;
	int32_t ifeature;
	eifttt_IF_feature_type efeature;
	int32_t if_id, node_id, endpoint_id;
	char *name = NULL;
	char *location = NULL;
	char *pchValue;
	char cArState[10] = { 0 }, cArLabel[50] = { 0 };
	zws_status_t res = ZWS_STATUS_IFTTT_ERROR_SERVER;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_START, NULL);

	//Find current user access token
	if(!user)
		goto l_err_no_user_id;

	if ((zws_avi_networks_ifttt_test_setup_get_user_accesstoken(user, &strAccessToekn) == ZWS_STATUS_OK) && 
		(strAccessToekn))
	{
		zwu_json_add_string_to_object(jsData, "accessToken", strAccessToekn);
	}
	else
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get access token for user id %s", user);
		goto l_err_get_access_token;
	}

	ZWS_FREE(strAccessToekn);

	if (avro_value_get_by_name(arg_value, "ifttt_device_list_array", &ifttt_device_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'ifttt_device_list_array': %s", avro_strerror());
		goto l_err_get_device_list_array_value;
	}

	if (avro_value_get_size(&ifttt_device_list_value, &feature_device_list_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get ifttt_device_list_array size");
		goto l_err_device_list_array_size;
	}

	if (feature_device_list_count == 0)
	{
		//No such interface type exists in the entire network.
		//Return IFTTT with response with blank "data" array or return one element
		//with label "No such device in the network" and value "-1".
		ZWS_LOG(ZWS_LOG_ERR, "No supported device with the efeatures in the network. Return empty 'Data' array.");
	}

	if ((jsSampleObj = zwu_json_create_object()))
	{
		zwu_json_add_item_to_object(jsData, "samples", jsSampleObj);
	}
	else
	{
		ZWS_LOG(ZWS_LOG_ERR, "Fail to create Samples json object");
		goto l_err_sample_json_create;
	}

	for (i = 0; i < feature_device_list_count; i++)
	{
		if (avro_value_get_by_index(&ifttt_device_list_value, i, &ifttt_device_feature_value, NULL) != 0)
		{
			goto l_err_get_device_feature_info_value;
		}
		if (zw_avro_field_get_int(&ifttt_device_feature_value, "eifttt_feature", &ifeature) != ZWS_STATUS_OK)
		{
			goto l_err_get_efeature_fields;
		}

		efeature = (eifttt_IF_feature_type)ifeature;
		
		ZWS_LOG(ZWS_LOG_ERR, "efeature = %d", efeature);

		//test it is trigger feature or action feature and create the json object accordingly
		if (efeature < eTrigger_IF_feature_all)
		{
			//trigger
			if (!jsTriggerObj)
			{
				if ((jsTriggerObj = zwu_json_create_object()))
				{
					zwu_json_add_item_to_object(jsSampleObj, "triggers", jsTriggerObj);
				}
				else
				{
					ZWS_LOG(ZWS_LOG_ERR, "Fail to create Triggers json object");
					goto l_err_trigger_json_create;
				}
			}
			iTriggerActionFlag = IFTTT_REQUEST_TYPE_TRIGGER;
			jsCurrObj = jsTriggerObj;
		}
		else
		{
			//action
			if (!jsActionObj)
			{
				if ((jsActionObj = zwu_json_create_object()))
				{
					zwu_json_add_item_to_object(jsSampleObj, "actions", jsActionObj);
				}
				else
				{
					ZWS_LOG(ZWS_LOG_ERR, "Fail to create Actions json object");
					goto l_err_action_json_create;
				}
			}

			iTriggerActionFlag = IFTTT_REQUEST_TYPE_ACTION;
			jsCurrObj = jsActionObj;
		}

		//Get the corresponding interfaceSlug string and fieldSlug string based on efeature
		if (zws_avi_ifttt_get_interfaceslug(iTriggerActionFlag, efeature, &pchInterfaceSlug, &pchFieldSlug) != ZWS_STATUS_OK)
		{
			goto l_err_get_interface_slug;
		}

		if (avro_value_get_by_name(&ifttt_device_feature_value, "ifttt_device_info", &ifttt_device_list_info_list_value, NULL) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'ifttt_device_info': %s", avro_strerror());
			goto l_err_get_device_info_array_value;
		}

		if (avro_value_get_size(&ifttt_device_list_info_list_value, &if_ep_name_list_count) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get ifttt_device_info_array size");
			goto l_err_device_info_array_size;
		}

		ZWS_LOG(ZWS_LOG_ERR, "if_ep_name_list_count: %d", if_ep_name_list_count);

		for (j = 0; j < if_ep_name_list_count; j++)
		{
			if (avro_value_get_by_index(&ifttt_device_list_info_list_value, j, &ifttt_device_list_info_value, NULL) != 0)
			{
				goto l_err_get_device_feature_info_value;
			}

			if ((zw_avro_field_get_int(&ifttt_device_list_info_value, "id", &if_id) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&ifttt_device_list_info_value, "node_id", &node_id) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&ifttt_device_list_info_value, "endpoint_id", &endpoint_id) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_string(&ifttt_device_list_info_value, "name", &name) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_string(&ifttt_device_list_info_value, "location", &location) != ZWS_STATUS_OK)
				)
			{
				goto l_err_get_device_feature_fields;
			}

			//Generate device name label
			//if (zws_avi_generate_device_label(if_id, node_id, endpoint_id, name, location, &pchLabel) != ZWS_STATUS_OK)
			//{
			//	goto l_err_generate_label;
			//}

			//Generate device/interface value (descirptor ID in string)
			if (zws_avi_generate_device_value(if_id, node_id, endpoint_id, &pchValue) != ZWS_STATUS_OK)
			{
				goto l_err_generate_value;
			}

			jsInterfaceObj = zwu_json_create_object();

			if (jsInterfaceObj)
			{
				zwu_json_add_item_to_object(jsCurrObj, pchInterfaceSlug, jsInterfaceObj);
				zwu_json_add_string_to_object(jsInterfaceObj, pchFieldSlug, pchValue);
			}

			ZWS_LOG(ZWS_LOG_ERR, "device_name: %s", pchValue);

			ZWS_FREE(pchValue);

			zw_avro_field_free_string(location);
			location = NULL;
			zw_avro_field_free_string(name);
			name = NULL;

			//Test/setup function seems ony allow 1 selection in the drop down
			break;
		}

		//Custom options
		//For Multilevel switch
		if (efeature == eAction_mSwitch_Dimmer)
		{
			sprintf(cArLabel, "%s", IFTTT_REQUEST_BODY_IFTTT_DEVICE_STATE);
			sprintf(cArState, "%d", 255);
			
			zwu_json_add_string_to_object(jsInterfaceObj, cArLabel, cArState);
		}
		else if ((efeature >= eTrigger_mSensor_Temp_LT) && (efeature <= eTrigger_mSensor_Humidity_GT))
		{
			//All multilevel sensor type uses threshold value 50 and unit 0
			sprintf(cArLabel, "%s", IFTTT_REQUEST_BODY_IFTTT_THLD_VALUE);
			sprintf(cArState, "%d", 50);
			zwu_json_add_string_to_object(jsInterfaceObj, cArLabel, cArState);

			sprintf(cArLabel, "%s", IFTTT_REQUEST_BODY_IFTTT_THLD_UNIT);
			sprintf(cArState, "%d", 0);
			zwu_json_add_string_to_object(jsInterfaceObj, cArLabel, cArState);
		}
	}

	res = ZWS_STATUS_OK;

	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);
	return res;

ZWS_FREE(pchValue);
l_err_generate_value:
l_err_get_device_feature_fields:
if(location)
	zw_avro_field_free_string(location);
if(name)
	zw_avro_field_free_string(name);
l_err_device_info_array_size:
l_err_get_device_info_array_value:
l_err_get_interface_slug:
l_err_action_json_create:
l_err_trigger_json_create:
l_err_get_efeature_fields:
l_err_get_device_feature_info_value:
l_err_sample_json_create:
l_err_device_list_array_size:
l_err_get_device_list_array_value:
l_err_get_access_token:
l_err_no_user_id:
	ZWS_LOG_EX(ZWS_LOG_FUNCTION, FUNC_LOG_END, res);
	return res;
}

/** Handlers for test_setup option */
static zws_avi_message_ifttt_t ifttt_test_setup =
{
	"ifttt_test_setup", 
	
	IFTTT_REQUEST_TYPE_GENERAL,
	eifttt_IF_feature_type_none,

	{IFTTT_REQUEST_SUBTYPE_NONE},

	"ifttt_network_test_setup",

	zws_avi_network_ifttt_test_setup_argument,
	zws_avi_network_ifttt_test_setup_result,

	IFTTT_RESPONSE_TYPE_DATA_OBJECT,

	""
};

/** Register command handlers */
zws_status_t zws_avi_ifttt_test_init(zwu_array_list_t message_list)
{
	if (zws_avi_message_list_add_message_ifttt(message_list, &ifttt_test_setup) != ZWS_STATUS_OK)
	{
		goto l_err_add_message;
	}

	return ZWS_STATUS_OK;

l_err_add_message:
	return ZWS_STATUS_ERROR;
}



