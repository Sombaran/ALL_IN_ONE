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
 * @file    zwp_avi_networks_whitelist.c
 *
 * @brief   AVI message 'network_whitelist'
 * @details Handles the AVI message 'network_whitelist'.
 *
 * @author  Angela Yan
 *
 * @version 1.0 - 2016-12-06
 * - Initial version
 */

#include "zwp_status.h"
#include "zwp_avi_message_networks.h"
#include "zwp_log.h"
#include "zwp_memory.h"
#include "zwp_error.h"
#include "zwp_hcapi.h"
#include "zwp_network.h"


/** Whitelist device info get operation state */
typedef struct
{
	char*		dsk;										/**< DSK of the device to query. */
} zwp_avi_networks_whitelist_device_info_t;

/** Whitelist Add argument */
typedef struct
{
	char*		dsk;										/**< DSK of the joining device
															Format: “34028-23669-20938-46346-33746-07431-56821-14553” */
	int			grant_keys;									/**< Restrict S2 grant keys to the device as specified. (Optional) */
	int			metatype_count;								/**< Number of meta types such name/loc */
	char		name[WL_MAX_NAME + 1];						/**< device name in UTF-8 encoding. It must not contains period character '.', \
																 underscore character '_' and must not end with the dash character '-' and \
																 must be terminated by NULL character */
	char		loc[WL_MAX_LOC + 1];						/**< device location in UTF-8 encoding. Same restriction as name field */

} zwp_avi_networks_whitelist_add_t;

/** Whitelist Remove operation state */
typedef struct
{
	char*		dsk;										/**< DSK of the device to be removed. Optional. Null means remove all. */
} zwp_avi_networks_whitelist_remove_t;

/**
 * @brief Execute call to send a get call for whitelist information
 * @param[in]     net               Network object
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 */
static zwp_status_t zwp_avi_networks_whitelist_list_get_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwnet_p zwnet;
	int hcapi_code;

	zwnet = zwp_net_get_zwnet(net);

	if ((hcapi_code = zwnet_wl_list_get(zwnet, zwp_net_whitelist_list_notification, net)) != ZW_ERR_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwnet_wl_list_get() failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
		goto l_err_wl_list_get;
	}

	return ZWP_STATUS_OK;

l_err_wl_list_get:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Execute call to retrieve whitelist list info and return to client
* @param[in]     net               Network object
* @param[in]     arg               Arguments (if any)
* @param[out]    result_p          Result object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
*@note
* @par
* The caller should call zwp_avi_networks_whitelist_list_report_result_free() to free resources
*/
static zwp_status_t zwp_avi_networks_whitelist_list_report_execute(zwp_net_t net, void *arg, void **result_p)
{
	//zwnet_p zwnet;
	zwu_array_list_t whitelist_dsk_array;

	//zwnet = zwp_net_get_zwnet(net);

	if (!(zwp_net_whitelist_list_report(net, &whitelist_dsk_array)))
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwp_net_whitelist_list_report() failed");
		goto l_err_wl_list_report;
	}

	*result_p = whitelist_dsk_array;

	return ZWP_STATUS_OK;

l_err_wl_list_report:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to network information
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_networks_whitelist_list_report_result(void *result, avro_value_t *arg_value)
{
	zwu_array_list_t whitelist_dsk_array;
	int i, wl_count;
	char *pdsk;
	avro_value_t whitelist_list_value;
	avro_value_t whitelist_value;

	whitelist_dsk_array = (zwu_array_list_t)result;

	wl_count = zwu_array_list_size(whitelist_dsk_array);

	if (avro_value_get_by_name(arg_value, "whitelist_list", &whitelist_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'whitelist_list': %s", avro_strerror());
		goto l_err_get_whitelist_list_value;
	}

	for (i = 0; i < wl_count; i++)
	{
		pdsk = zwu_array_list_get(whitelist_dsk_array, i);

		if (avro_value_append(&whitelist_list_value, &whitelist_value, 0) != 0)
		{
			goto l_err_append_whitelist_value;
		}

		if (avro_value_set_string(&whitelist_value, pdsk) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to set dsk string: %s, %s", pdsk, avro_strerror());
			goto l_err_set_whitelist_value;
		}
	}

	return ZWP_STATUS_OK;

l_err_set_whitelist_value:
l_err_append_whitelist_value:
l_err_get_whitelist_list_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to network information
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_networks_gateway_status_get_execute
 */
static void zwp_avi_networks_whitelist_list_report_result_free(void *result)
{
	zwu_array_list_t whitelist_dsk_array = result;

	zwp_net_whitelist_dsk_array_free(whitelist_dsk_array);
}


/**
* @brief Get argument for Whitelist device info operation from Avro value object
* @param[in]     arg_value         Avro value object
* @param[out]    args_p            Arguments passed with Whitelist device info operation
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zwp_avi_networks_whitelist_device_info_argument_free() to free resources
*/
static zwp_status_t zwp_avi_networks_whitelist_device_info_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_networks_whitelist_device_info_t *argument;

	if ((argument = (zwp_avi_networks_whitelist_device_info_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_string(arg_value, "dsk", &argument->dsk) != ZWP_STATUS_OK || strlen(argument->dsk) == 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Missing dsk argument or invalid dsk length");
		goto l_err_arg_invalid;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_arg_invalid:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free arguments for Whitelist device info operation
* @param[in]     args              Arguments passed with Whitelist device info operation
* @return        Nothing
* @see zwp_avi_networks_whitelist_device_info_argument
*/
static void zwp_avi_networks_whitelist_device_info_argument_free(void *args)
{
	if (args)
	{
		zwp_avi_networks_whitelist_device_info_t *argument = args;

		if (argument->dsk)
			zw_avro_field_free_string(argument->dsk);

		ZWP_FREE(argument);
	}
}

/**
* @brief Execute call to retrieve whitelist device info and return to client
* @param[in]     net               Network object
* @param[in]     arg               Arguments (if any)
* @param[out]    result_p          Result object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
*@note
* @par
* The caller should call zwp_avi_networks_whitelist_device_info_result_free() to free resources
*/
static zwp_status_t zwp_avi_networks_whitelist_device_info_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwp_avi_networks_whitelist_device_info_t *argument = arg;
	zwnet_p zwnet;
	int hcapi_code;
	zwp_net_wl_dev_info_t *pwl_dev_info_temp, *pwl_dev_info_result = NULL;

	zwnet = zwp_net_get_zwnet(net);

	ZWP_LOG(ZWP_LOG_DEBUG, "argument->dsk : %s", argument->dsk);

	if ((hcapi_code = zwnet_wl_get(zwnet, argument->dsk, zwp_net_whitelist_dev_info_notification, net)) != ZW_ERR_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwnet_wl_get() failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
		goto l_err_wl_device_info_get;
	}

	if (zwp_net_find_a_whitelist_dev(net, argument->dsk, &pwl_dev_info_temp) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Fail to find whitelist device info with dsk: %s", argument->dsk);
		goto l_err_wl_find_dev_info;
	}

	if (pwl_dev_info_temp)
	{
		if ((pwl_dev_info_result = (zwp_net_wl_dev_info_t *)ZWP_MALLOC(sizeof(*pwl_dev_info_result))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_dev_info_alloc;
		}
	}

	memcpy(pwl_dev_info_result, pwl_dev_info_temp, sizeof(zwp_net_wl_dev_info_t));

	*result_p = pwl_dev_info_result;

	return ZWP_STATUS_OK;

	ZWP_FREE(pwl_dev_info_result);
l_err_dev_info_alloc:
l_err_wl_find_dev_info:
l_err_wl_device_info_get:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Set Avro value object with values from Result object corresponding to network information
* @param[in]     result            Result object
* @param[out]    arg_value         Avro value object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_networks_whitelist_device_info_result(void *result, avro_value_t *arg_value)
{
	zwp_net_wl_dev_info_t *pwl_dev_info = result;

	if ((zw_avro_field_set_string(arg_value, "dsk", pwl_dev_info->dsk) != ZWP_STATUS_OK) ||
		(zw_avro_field_set_int(arg_value, "granted_keys", pwl_dev_info->grant_keys) != ZWP_STATUS_OK) ||
		(zw_avro_field_set_string(arg_value, "name", pwl_dev_info->name) != ZWP_STATUS_OK) ||
		(zw_avro_field_set_string(arg_value, "loc", pwl_dev_info->loc) != ZWP_STATUS_OK)
		)
	{
		goto l_err_set_value;
	}

	return ZWP_STATUS_OK;

l_err_set_value:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free Result object corresponding to network information
* @param[in]     result            Result object
* @return        Nothing
* @see zwp_avi_networks_gateway_status_get_execute
*/
static void zwp_avi_networks_whitelist_device_info_result_free(void *result)
{
	zwp_net_wl_dev_info_t *pwl_dev_info = result;

	ZWP_FREE(pwl_dev_info);
}

/**
* @brief Get argument for Whitelist Add operation from Avro value object
* @param[in]     arg_value         Avro value object
* @param[out]    args_p            Arguments passed with Node Add operation
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zwp_avi_networks_whitelist_add_argument_free() to free resources
*/
static zwp_status_t zwp_avi_networks_whitelist_add_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_networks_whitelist_add_t *argument;
	char *string;

	if ((argument = (zwp_avi_networks_whitelist_add_t *)ZWP_CALLOC(1, sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_string(arg_value, "dsk", &argument->dsk) != ZWP_STATUS_OK || strlen(argument->dsk) == 0)
	{
		goto l_err_args_dsk;
	}

	//Optional param
	if (zw_avro_field_get_int(arg_value, "granted_keys", &argument->grant_keys) != ZWP_STATUS_OK)
	{
		argument->grant_keys = 0;
	}



	//Optional param
	if (zw_avro_field_get_string(arg_value, "name", &string) == ZWP_STATUS_OK)
	{
		strncpy(argument->name, string, sizeof(argument->name));
		argument->name[sizeof(argument->name) - 1] = '\0';
		zw_avro_field_free_string(string);
		argument->metatype_count++;
	}

	//Optional param
	if (zw_avro_field_get_string(arg_value, "location", &string) == ZWP_STATUS_OK)
	{
		strncpy(argument->loc, string, sizeof(argument->loc));
		argument->loc[sizeof(argument->loc) - 1] = '\0';
		zw_avro_field_free_string(string);
		argument->metatype_count++;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Name = %s, loc = %s, metatype_count = %d", argument->name, argument->loc, argument->metatype_count);

	*args_p = argument;

	return ZWP_STATUS_OK;


	if (argument->dsk)
		zw_avro_field_free_string(argument->dsk);
l_err_args_dsk:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free arguments for Whitelist Add operation
* @param[in]     args              Arguments passed with Whitelist Add operation
* @return        Nothing
* @see zwp_avi_networks_whitelist_add_argument
*/
static void zwp_avi_networks_whitelist_add_argument_free(void *args)
{
	if (args)
	{
		zwp_avi_networks_whitelist_add_t *argument = args;

		if (argument->dsk)
			zw_avro_field_free_string(argument->dsk);

		ZWP_FREE(argument);
	}
}

/**
* @brief Execute call to Whitelist Add to network
* @param[in]     net               Network object
* @param[in]     arg               Arguments (if any)
* @param[out]    result_p          Result object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_networks_whitelist_add_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwp_avi_networks_whitelist_add_t *argument = arg;
	zwnet_p zwnet;
	int hcapi_code;
	wl_info_t *info = NULL;
	int i = 0;

	zwnet = zwp_net_get_zwnet(net);

	if(argument->metatype_count > 0)
	{
		if ((info = (wl_info_t *)ZWP_CALLOC(argument->metatype_count, sizeof(*info))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_meta_info_alloc;
		}

		if (argument->name[0] != '\0')
		{
			info[i].type = WL_INFO_TYPE_NAME;
			strncpy(info[i].info.name, argument->name, sizeof(argument->name));
			i++;
		}

		if (argument->loc[0] != '\0')
		{
			info[i].type = WL_INFO_TYPE_LOC;
			strncpy(info[i].info.loc, argument->loc, sizeof(argument->loc));
			i++;
		}
	}

	if ((hcapi_code = zwnet_wl_add(zwnet, argument->dsk, argument->grant_keys, 
									info, i)) != ZW_ERR_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwnet_wl_add() failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
		goto l_err_wl_add;
	}

	//zwp_net_set_operation(net, ZWNET_OP_ADD_NODE);

	return ZWP_STATUS_OK;

l_err_wl_add:
l_err_meta_info_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Get argument for Whitelist Remove operation from Avro value object
* @param[in]     arg_value         Avro value object
* @param[out]    args_p            Arguments passed with Whitelist Remove operation
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zwp_avi_networks_whitelist_remove_argument_free() to free resources
*/
static zwp_status_t zwp_avi_networks_whitelist_remove_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_networks_whitelist_remove_t *argument;

	if ((argument = (zwp_avi_networks_whitelist_remove_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_string(arg_value, "dsk", &argument->dsk) != ZWP_STATUS_OK || strlen(argument->dsk) == 0)
	{
		//Optional param.
		argument->dsk = NULL;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;


	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free arguments for Whitelist Remove operation
* @param[in]     args              Arguments passed with Whitelist Remove operation
* @return        Nothing
* @see zwp_avi_networks_whitelist_remove_argument
*/
static void zwp_avi_networks_whitelist_remove_argument_free(void *args)
{
	if (args)
	{
		zwp_avi_networks_whitelist_remove_t *argument = args;

		if (argument->dsk)
			zw_avro_field_free_string(argument->dsk);

		ZWP_FREE(argument);
	}
}

/**
* @brief Execute call to Whitelist Remove from network
* @param[in]     net               Network object
* @param[in]     arg               Arguments (if any)
* @param[out]    result_p          Result object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_networks_whitelist_remove_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwp_avi_networks_whitelist_remove_t *argument;
	zwnet_p zwnet;
	int hcapi_code;

	argument = (zwp_avi_networks_whitelist_remove_t *)arg;

	zwnet = zwp_net_get_zwnet(net);

	if (argument->dsk)
	{
		//Removce 1 whitelist device
		if ((hcapi_code = zwnet_wl_clr(zwnet, argument->dsk)) != ZW_ERR_NONE)
		{
			ZWP_LOG(ZWP_LOG_ERR, "zwnet_wl_clr() failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
			goto l_err_whitelist_clear_fail;
		}
	}
	else
	{
		//Remove all whitelist devices
		if ((hcapi_code = zwnet_wl_list_clr(zwnet)) != ZW_ERR_NONE)
		{
			ZWP_LOG(ZWP_LOG_ERR, "zwnet_wl_list_clr() failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
			goto l_err_whitelist_clear_all_fail;
		}
	}

	//zwp_net_set_operation(net, ZWNET_OP_RM_FAILED_ID);

	return ZWP_STATUS_OK;

l_err_whitelist_clear_all_fail:
l_err_whitelist_clear_fail:
	return ZWP_STATUS_ERROR;
}


/** Handlers for whitelist list Get */
static zwp_avi_message_networks_t networks_whitelist_list_get =
{
	"network_whitelist_list_get",
	NULL, NULL,
	zwp_avi_networks_whitelist_list_get_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
	NULL, NULL
};

/** Handlers for whitelist list Report */
static zwp_avi_message_networks_t networks_whitelist_list_report =
{
	"network_whitelist_list_report",
	NULL, NULL,
	zwp_avi_networks_whitelist_list_report_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
	zwp_avi_networks_whitelist_list_report_result, zwp_avi_networks_whitelist_list_report_result_free
};

/** Handlers for whitelist device info Report */
static zwp_avi_message_networks_t networks_whitelist_device_info =
{
	"network_whitelist_device_info",
	zwp_avi_networks_whitelist_device_info_argument, zwp_avi_networks_whitelist_device_info_argument_free,
	zwp_avi_networks_whitelist_device_info_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
	zwp_avi_networks_whitelist_device_info_result, zwp_avi_networks_whitelist_device_info_result_free
};

/** Handlers for whitelist Add */
static zwp_avi_message_networks_t networks_whitelist_add =
{
	"network_whitelist_add",
	zwp_avi_networks_whitelist_add_argument, zwp_avi_networks_whitelist_add_argument_free,
	zwp_avi_networks_whitelist_add_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
	NULL, NULL
};

/** Handlers for whitelist remove operation */
static zwp_avi_message_networks_t networks_whitelist_remove =
{
	"network_whitelist_remove",
	zwp_avi_networks_whitelist_remove_argument, zwp_avi_networks_whitelist_remove_argument_free,
	zwp_avi_networks_whitelist_remove_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
	NULL, NULL
};

/** Register handlers */
zwp_status_t zwp_avi_networks_whitelist_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_message_list_add_message_networks(message_list, &networks_whitelist_list_get) != ZWP_STATUS_OK) ||
		(zwp_avi_message_list_add_message_networks(message_list, &networks_whitelist_list_report) != ZWP_STATUS_OK) ||
		(zwp_avi_message_list_add_message_networks(message_list, &networks_whitelist_device_info) != ZWP_STATUS_OK) ||
		(zwp_avi_message_list_add_message_networks(message_list, &networks_whitelist_add) != ZWP_STATUS_OK) ||
		(zwp_avi_message_list_add_message_networks(message_list, &networks_whitelist_remove) != ZWP_STATUS_OK)
		)
	{
		goto l_err_info_get;
	}

	return ZWP_STATUS_OK;

l_err_info_get:
	return ZWP_STATUS_ERROR;
}

