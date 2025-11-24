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
 * @file    zwp_avi_ifttt_test.c
 *
 * @brief   AVI message 'ifttt_test'
 * @details Handles the AVI message 'ifttt_test'.
 *
 * @author  Angela Yan
 *
 * @version 1.0 - 2016-10-18
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

 /** test setup argument */
typedef struct
{
	//zwu_array_list_t			ar_efeature_list; //each element is a eifttt_IF_feature_type
	int							icount;
	eifttt_IF_feature_type		*pefeature_list;
} zwp_avi_ifttt_test_setup_argument_t;

/** efeature and respective device list */
typedef struct
{
	eifttt_IF_feature_type		efeature;
	zwu_array_list_t			if_ep_name_list;
} zwp_avi_ifttt_feature_device_list_t;


 /**
 * @brief Get argument for test setup operation from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Node Add operation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_networks_node_add_argument_free() to free resources
 */
static zwp_status_t zwp_avi_ifttt_test_setup_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_ifttt_test_setup_argument_t *argument;
	avro_value_t efeature_list_value;
	avro_value_t efeature_value;
	size_t feature_count;
	int32_t ifeature;
	int i = 0;
	eifttt_IF_feature_type *paefeature;

	if (avro_value_get_by_name(arg_value, "eifttt_feature_list", &efeature_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'eifttt_feature_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (avro_value_get_size(&efeature_list_value, &feature_count) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get feature_count value list size");
		goto l_err_get_list_size;
	}

	if (feature_count > 0)
	{
		if ((argument = (zwp_avi_ifttt_test_setup_argument_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_args_alloc;
		}

		if ((argument->pefeature_list = (eifttt_IF_feature_type *)ZWP_MALLOC(feature_count * sizeof(eifttt_IF_feature_type))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_args_list_alloc;
		}

		for (i = 0, paefeature = argument->pefeature_list; 
			 i < feature_count; 
			 i++, paefeature++)
		{
			if (avro_value_get_by_index(&efeature_list_value, i, &efeature_value, NULL) != 0)
			{
				goto l_err_get_efeature_value;
			}

			if (avro_value_get_int(&efeature_value, &ifeature) != ZWP_STATUS_OK)
			{
				goto l_err_get_int_efeature_value;
			}

			*paefeature = (eifttt_IF_feature_type)ifeature;
		}

		argument->icount = feature_count;

		*args_p = argument;
	}

	return ZWP_STATUS_OK;	

l_err_get_int_efeature_value:
l_err_get_efeature_value:
	ZWP_FREE(argument->pefeature_list);
l_err_args_list_alloc:
	ZWP_FREE(argument);
l_err_args_alloc:	
l_err_get_list_size:
l_err_get_list_value:

	return ZWP_STATUS_ERROR;
}

/**
* @brief Free arguments for test setup operation
* @param[in]     args              Arguments passed with Remove Failed Node operation
* @return        Nothing
* @see zwp_avi_networks_failed_node_remove_argument
*/
static void zwp_avi_ifttt_test_setup_argument_free(void *args)
{
	zwp_avi_ifttt_test_setup_argument_t *argument;

	argument = (zwp_avi_ifttt_test_setup_argument_t *)args;

	if (argument)
	{
		if (argument->pefeature_list)
		{
			ZWP_FREE(argument->pefeature_list);
		}
		ZWP_FREE(args);
	}
}

/**
* @brief Free Result object corresponding to network operation state
* @param[in]     result            Result object
* @return        Nothing
* @see zwp_avi_ifttt_test_get_execute
*/
static void zwp_avi_ifttt_test_setup_result_free(void *result)
{
	int count;
	int i;
	zwu_array_list_t	feature_device_list = (zwu_array_list_t)result;
	zwp_avi_ifttt_feature_device_list_t  *afeature_device;

	count = zwu_array_list_size(feature_device_list);

	for (i = 0; i < count; i++)
	{
		if ((afeature_device = zwu_array_list_get(feature_device_list, i)) != NULL)
		{
			zwp_desc_free_if_ep_name_list(afeature_device->if_ep_name_list);
			ZWP_FREE(afeature_device);
		}
	}

	zwu_array_list_destroy(feature_device_list);
}

/**
 * @brief Execute call to test setup operation status
 * @param[in]     net               Network object
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 *@note
 * @par
 * The caller should call zwp_avi_ifttt_test_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_ifttt_test_setup_execute(zwp_net_t net, void *arg, void **result_p)
{
	//zwp_status_t res = ZWP_STATUS_ERROR;
	zwp_avi_ifttt_test_setup_argument_t *argument;
	zwp_desc_net_t desc_net;
	zwu_array_list_t if_ep_name_list;
	eifttt_IF_feature_type		*paefeature;
	int feature_count;
	int i, iNofParam, iParam;
	uint16_t interfaceid;
	zwu_array_list_t	feature_device_list;
	zwp_avi_ifttt_feature_device_list_t  *afeature_device;

	argument = (zwp_avi_ifttt_test_setup_argument_t *)arg;

	desc_net = zwp_net_get_desc_net(net);

	if (!desc_net)
		goto l_err_invalid_net_desc;

	if (zwu_array_list_create(0, &feature_device_list) != ZWP_STATUS_OK)
	{
		goto l_err_result_list_create;
	}

	feature_count = argument->icount;

	for (i = 0, paefeature = argument->pefeature_list; i < feature_count; i++, paefeature++)
	{
		if (zwp_ifttt_find_cls_from_efeature(*paefeature, &interfaceid, &iNofParam, &iParam) != ZWP_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Unrecognized IFTTT efeature %d, continue to find other efeature", *paefeature);
		}
		else
		{
			if (zwp_desc_get_device_from_interface_id(desc_net, interfaceid, &if_ep_name_list) != ZWP_STATUS_OK)
			{
				goto l_err_get_if_ep_name_list;
			}

			if (iNofParam > 0)
			{
				//Additional param, additional checking
				if (zwp_ifttt_check_device_list_with_additional_param(*paefeature, if_ep_name_list, desc_net, iNofParam, iParam) != ZWP_STATUS_OK)
				{
					goto l_err_check_dev_list;
				}
			}

			if (zwu_array_list_size(if_ep_name_list) == 0)
				continue;

			if ((afeature_device = (zwp_avi_ifttt_feature_device_list_t *)ZWP_MALLOC(sizeof(*afeature_device))) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				goto l_err_afeature_device_alloc;
			}

			afeature_device->efeature = *paefeature;
			afeature_device->if_ep_name_list = if_ep_name_list;

			if (zwu_array_list_add(feature_device_list, afeature_device) != ZWP_STATUS_OK)
			{
				goto l_err_feature_list_add;
			}

		}
	}

	*result_p = feature_device_list;

	return ZWP_STATUS_OK;

l_err_feature_list_add:
	ZWP_FREE(afeature_device);
l_err_afeature_device_alloc:
l_err_check_dev_list:
	zwp_desc_free_if_ep_name_list(if_ep_name_list);
l_err_get_if_ep_name_list:
	zwp_avi_ifttt_test_setup_result_free(feature_device_list);
l_err_result_list_create:
l_err_invalid_net_desc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to network operation state
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_ifttt_test_setup_result(void *result, avro_value_t *arg_value)
{
	
	zwu_array_list_t	feature_device_list;
	zwp_desc_IF_ep_name_t  *pstIF_ep_name;
	avro_value_t ifttt_device_list_value, ifttt_device_list_info_list_value;
	avro_value_t ifttt_device_feature_value, ifttt_device_list_info_value;
	int i, j, feature_device_list_count, if_ep_name_list_count;
	int32_t ifeature;
	zwp_avi_ifttt_feature_device_list_t  *afeature_device;

	feature_device_list = (zwu_array_list_t)result;
	feature_device_list_count = zwu_array_list_size(feature_device_list);

	if (avro_value_get_by_name(arg_value, "ifttt_device_list_array", &ifttt_device_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'ifttt_device_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	for (i = 0; i < feature_device_list_count; i++)
	{
		if (avro_value_append(&ifttt_device_list_value, &ifttt_device_feature_value, 0) != 0)
		{
			goto l_err_append_ifttt_device_value;
		}

		afeature_device = zwu_array_list_get(feature_device_list, i);
		ifeature = (int32_t)afeature_device->efeature;

		if (zw_avro_field_set_int(&ifttt_device_feature_value, "eifttt_feature", ifeature) != ZWP_STATUS_OK)
		{
			goto l_err_set_efeature_value;
		}

		if_ep_name_list_count = zwu_array_list_size(afeature_device->if_ep_name_list);

		if (avro_value_get_by_name(&ifttt_device_feature_value, "ifttt_device_info", &ifttt_device_list_info_list_value, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'ifttt_device_info': %s", avro_strerror());
			goto l_err_get_ifttt_device_info_list_value;
		}

		for (j = 0; j < if_ep_name_list_count; j++)
		{
			if (avro_value_append(&ifttt_device_list_info_list_value, &ifttt_device_list_info_value, 0) != 0)
			{
				goto l_err_append_ifttt_device_info_value;
			}

			pstIF_ep_name = zwu_array_list_get(afeature_device->if_ep_name_list, j);

			if ((zw_avro_field_set_int(&ifttt_device_list_info_value, "id", pstIF_ep_name->desc_interface->cls) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&ifttt_device_list_info_value, "node_id", pstIF_ep_name->desc_interface->nodeid) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&ifttt_device_list_info_value, "endpoint_id", pstIF_ep_name->desc_interface->epid) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_string(&ifttt_device_list_info_value, "name", pstIF_ep_name->name) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_string(&ifttt_device_list_info_value, "location", pstIF_ep_name->loc) != ZWP_STATUS_OK)
				)
			{
				goto l_err_set_ifttt_device_value;
			}
		}

		ZWP_LOG(ZWP_LOG_DEBUG, "Done. %d devices found for feature %d.", if_ep_name_list_count, ifeature);

	}

	return ZWP_STATUS_OK;

l_err_set_ifttt_device_value:
l_err_append_ifttt_device_info_value:
l_err_get_ifttt_device_info_list_value:
l_err_set_efeature_value:
l_err_append_ifttt_device_value:
l_err_get_list_value:
	return ZWP_STATUS_ERROR;
}

/** Handlers for getting test option */
static zwp_avi_message_networks_t ifttt_test_setup =
{
	"ifttt_network_test_setup",
	zwp_avi_ifttt_test_setup_argument, zwp_avi_ifttt_test_setup_argument_free,
	zwp_avi_ifttt_test_setup_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
	zwp_avi_ifttt_test_setup_result, zwp_avi_ifttt_test_setup_result_free
};


/** Register handlers */
zwp_status_t zwp_avi_ifttt_test_init(zwu_array_list_t network_message_list, zwu_array_list_t interface_message_list)
{
	if (zwp_avi_message_list_add_message_networks(network_message_list, &ifttt_test_setup) != ZWP_STATUS_OK)
	{
		goto l_err_test_setup;
	}

	return ZWP_STATUS_OK;

l_err_test_setup:
	return ZWP_STATUS_ERROR;
}

