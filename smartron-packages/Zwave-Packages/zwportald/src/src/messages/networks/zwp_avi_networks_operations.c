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
 * @file    zwp_avi_networks_operations.c
 *
 * @brief   AVI messages for all Z-Wave network operations
 * @details Handles the AVI messages for all Z-Wave network operations.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-09-13
 * - Initial version
 */

#include "zwp_status.h"
#include "zwp_avi_message_networks.h"
#include "zwp_log.h"
#include "zwp_memory.h"
#include "zwp_error.h"
#include "zwp_hcapi.h"
#include "zwp_network.h"
#include "zwp_descriptor.h"
#include "zwp_node_state.h"

/** Add Node operation state */
typedef struct {
	char*     dsk;											/**< Optional DSK of the joining device 
															Format: “34028-23669-20938-46346-33746-07431-56821-14553” */
} zwp_avi_networks_node_add_t;

/** Add Node grant key operation state */
typedef struct {
	int		granted_keys;									/**< Grant key for security 2 inclusion */
	int		grant_csa;										/**< Grant key for security 2 inclusion */
} zwp_avi_networks_node_add_grant_key_t;

/** Add Node accept operation state */
typedef struct {
	int		accept;											/**< Accept option 1=accept; 0=reject */
	char*   dsk;											/**< DSK from the client
															Format: “34028-23669-20938-46346-33746-07431-56821-14553” */
} zwp_avi_networks_node_add_accept_t;

/** Remove Failed Node operation state */
typedef struct {
	int     node_id;										/**< Failed node ID to be removed */
} zwp_avi_networks_failed_node_remove_t;

/** Replace Failed Node operation state */
typedef struct {
	int     node_id;										/**< Failed node ID to be replaced */
	char*   dsk;											/**< Optional DSK of the joining device
															Format: “34028-23669-20938-46346-33746-07431-56821-14553” */
} zwp_avi_networks_failed_node_replace_t;

/** Node Update operation state */
typedef struct {
	int     node_id;                                       /**< Node ID of node to be updated */
} zwp_avi_networks_node_update_t;

/** Send NIF operation state */
typedef struct {
	int     node_id;                                       /**< Destination Node ID */
	int     is_broadcast;                                  /**< Flag to indicate of the NIF is to be sent as broadcast */
} zwp_avi_networks_send_NIF_t;



/**
* @brief Execute call to get prekit dsk of joining node
* @param[in]     net               Network object
* @param[in]     arg               Arguments (if any)
* @param[out]    result_p          Result object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
*@note
* @par
* The caller should call zwp_avi_networks_node_prekit_dsk_get_result_free() to free resources
*/
static zwp_status_t zwp_avi_networks_node_prekit_dsk_get_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwnet_sts_t *pdsk;

	ZWP_LOG(ZWP_LOG_DEBUG, "zwp_avi_networks_node_prekit_dsk_get_execute");
	if ((pdsk = (zwnet_sts_t *)ZWP_MALLOC(sizeof(zwnet_sts_t))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_dsk_alloc;
	}

	if (!zwp_net_get_prekit_node_dsk(net, pdsk))
		goto l_err_dsk_get;

	*result_p = pdsk;

	return ZWP_STATUS_OK;

l_err_dsk_get:
	if (pdsk)
	{
		ZWP_FREE(pdsk);
	}
l_err_dsk_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Set Avro value object with values from Result object corresponding to prekit dsk during prekit node joining
* @param[in]     result            Result object
* @param[out]    arg_value         Avro value object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_networks_node_prekit_dsk_get_result(void *result, avro_value_t *arg_value)
{
	zwnet_sts_t *pdsk = (zwnet_sts_t *)result;

	ZWP_LOG(ZWP_LOG_DEBUG, "zwp_avi_networks_node_prekit_dsk_get_result");
	if (zw_avro_field_set_string(arg_value, "dsk", pdsk->info.s2_dsk) != ZWP_STATUS_OK)
	{
		goto l_err_set_dsk_value;
	}

	return ZWP_STATUS_OK;

l_err_set_dsk_value:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free Result object corresponding to prekit dsk during prekit node joining
* @param[in]     result            Result object
* @return        Nothing
* @see zwp_avi_networks_operation_state_get_execute
*/
static void zwp_avi_networks_node_prekit_dsk_get_result_free(void *result)
{
	zwnet_sts_t *pdsk = (zwnet_sts_t *)result;

	if(pdsk)
		ZWP_FREE(pdsk);

}


/**
 * @brief Execute call to Add Node to network
 * @param[in]     net               Network object
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_networks_node_add_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwp_avi_networks_node_add_t *argument = arg;
	zwnet_p zwnet;
	int hcapi_code;
	sec2_add_prm_t sec2_param;

	zwnet = zwp_net_get_zwnet(net);

	sec2_param.usr_param = net;
	sec2_param.cb = add_node_sec2;
	sec2_param.dsk = NULL;
	if (argument && argument->dsk)
		sec2_param.dsk = argument->dsk;

	if ((hcapi_code = zwnet_add(zwnet, 1, &sec2_param, 0)) != ZW_ERR_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwnet_add() with 'add' failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
		goto l_err_node_add;
	}

	zwp_net_set_operation(net, ZWNET_OP_ADD_NODE);

	return ZWP_STATUS_OK;

l_err_node_add:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Get argument for Add Node operation from Avro value object
* @param[in]     arg_value         Avro value object
* @param[out]    args_p            Arguments passed with Node Add operation
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zwp_avi_networks_node_add_argument_free() to free resources
*/
static zwp_status_t zwp_avi_networks_node_add_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_networks_node_add_t *argument;

	if ((argument = (zwp_avi_networks_node_add_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_string(arg_value, "dsk", &argument->dsk) != ZWP_STATUS_OK || strlen(argument->dsk) == 0)
	{
		//dsk is optional
		if (argument->dsk)
			zw_avro_field_free_string(argument->dsk);
		argument->dsk = NULL;
		ZWP_FREE(argument);
		argument = NULL;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free arguments for Add Node operation
* @param[in]     args              Arguments passed with Add Node operation
* @return        Nothing
* @see zwp_avi_networks_node_add_argument
*/
static void zwp_avi_networks_node_add_argument_free(void *args)
{
	if (args)
	{
		zwp_avi_networks_node_add_t *argument = args;

		if (argument->dsk)
			zw_avro_field_free_string(argument->dsk);

		ZWP_FREE(argument);
	}
}

/**
* @brief Execute call to Add Node On-behalf to network
* @param[in]     net               Network object
* @param[in]     arg               Arguments (if any)
* @param[out]    result_p          Result object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_networks_node_add_on_behalf_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwp_avi_networks_node_add_t *argument = arg;
	zwnet_p zwnet;
	int hcapi_code;
	sec2_add_prm_t sec2_param;

	zwnet = zwp_net_get_zwnet(net);

	sec2_param.usr_param = net;
	sec2_param.cb = add_node_sec2;
	sec2_param.dsk = NULL;
	if (argument && argument->dsk)
		sec2_param.dsk = argument->dsk;

	if ((hcapi_code = zwnet_add(zwnet, 1, &sec2_param, 1)) != ZW_ERR_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwnet_add() with 'add on-behalf' failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
		goto l_err_node_add;
	}

	zwp_net_set_operation(net, ZWNET_OP_ADD_NODE);

	return ZWP_STATUS_OK;

l_err_node_add:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Execute call to get requested key during Security 2 Node Add
* @param[in]     net               Network object
* @param[in]     arg               Arguments (if any)
* @param[out]    result_p          Result object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
*@note
* @par
* The caller should call zwp_avi_networks_node_add_req_keys_get_result_free() to free resources
*/
static zwp_status_t zwp_avi_networks_node_add_req_key_get_execute(zwp_net_t net, void *arg, void **result_p)
{
	sec2_keys_req_cb_prm_t *pvalue;

	if ((pvalue = (sec2_keys_req_cb_prm_t *)ZWP_MALLOC(sizeof(sec2_keys_req_cb_prm_t))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_req_key_alloc;
	}

	if (!zwp_net_get_req_key(net, pvalue))
		goto l_err_req_key_get;

	*result_p = pvalue;

	return ZWP_STATUS_OK;

l_err_req_key_get:
	ZWP_FREE(pvalue);
l_err_req_key_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Set Avro value object with values from Result object corresponding to requested key during Security 2 Node Add
* @param[in]     result            Result object
* @param[out]    arg_value         Avro value object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_networks_node_add_req_key_get_result(void *result, avro_value_t *arg_value)
{
	sec2_keys_req_cb_prm_t *pvalue = (sec2_keys_req_cb_prm_t *)result;

	if ((zw_avro_field_set_int(arg_value, "req_keys", pvalue->req_keys) != ZWP_STATUS_OK) ||
		(zw_avro_field_set_int(arg_value, "req_csa", pvalue->req_csa) != ZWP_STATUS_OK) ||
		(zw_avro_field_set_string(arg_value, "csa_pin", pvalue->csa_pin) != ZWP_STATUS_OK)
		)
	{
		goto l_err_set_req_key_value;
	}

	return ZWP_STATUS_OK;

l_err_set_req_key_value:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free Result object corresponding to requested key during Security 2 Node Add
* @param[in]     result            Result object
* @return        Nothing
* @see zwp_avi_networks_operation_state_get_execute
*/
static void zwp_avi_networks_node_add_req_key_get_result_free(void *result)
{
	ZWP_FREE(result);
}

/**
* @brief Get argument for add node set grant key operation from Avro value object
* @param[in]     arg_value         Avro value object
* @param[out]    args_p            Arguments passed with Remove Failed Node operation
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zwp_avi_networks_failed_node_remove_argument_free() to free resources
*/
static zwp_status_t zwp_avi_networks_node_add_grant_key_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_networks_node_add_grant_key_t *argument;

	if ((argument = (zwp_avi_networks_node_add_grant_key_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "granted_keys", &argument->granted_keys) != ZWP_STATUS_OK)
	{
		goto l_err_get_value;
	}

	//Optional param
	if (zw_avro_field_get_int(arg_value, "grant_csa", &argument->grant_csa) != ZWP_STATUS_OK)
	{
		argument->grant_csa = 0;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free arguments for add node set grant key operation
* @param[in]     args              Arguments passed with Remove Failed Node operation
* @return        Nothing
* @see zwp_avi_networks_failed_node_remove_argument
*/
static void zwp_avi_networks_node_add_grant_key_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
* @brief Execute call to add node set grant key from network
* @param[in]     net               Network object
* @param[in]     arg               Arguments (if any)
* @param[out]    result_p          Result object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_networks_node_add_grant_key_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwp_avi_networks_node_add_grant_key_t *argument;
	zwnet_p zwnet;
	int hcapi_code;

	ZWP_LOG(ZWP_LOG_DEBUG, "zwnet_add_sec2_grant_key");
	argument = (zwp_avi_networks_node_add_grant_key_t *)arg;

	zwnet = zwp_net_get_zwnet(net);

	if ((hcapi_code = zwnet_add_sec2_grant_key(zwnet, (uint8_t)argument->granted_keys, (uint8_t)argument->grant_csa)) != ZW_ERR_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwnet_add_sec2_grant_key()  with grant key %u grant_csa %u failed with '%s'", 
			argument->granted_keys, argument->grant_csa, zwp_hcapi_error_code_to_string(hcapi_code));
		goto l_err_grant_key;
	}

	return ZWP_STATUS_OK;

l_err_grant_key:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Execute call to get dsk during Security 2 Node Add
* @param[in]     net               Network object
* @param[in]     arg               Arguments (if any)
* @param[out]    result_p          Result object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
*@note
* @par
* The caller should call zwp_avi_networks_node_add_dsk_get_result_free() to free resources
*/
static zwp_status_t zwp_avi_networks_node_add_dsk_get_execute(zwp_net_t net, void *arg, void **result_p)
{
	sec2_dsk_cb_prm_t *pdsk;

	ZWP_LOG(ZWP_LOG_DEBUG, "zwp_avi_networks_node_add_dsk_get_execute");
	if ((pdsk = (sec2_dsk_cb_prm_t *)ZWP_MALLOC(sizeof(sec2_dsk_cb_prm_t))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_dsk_alloc;
	}

	pdsk->dsk =NULL;

	if (!zwp_net_get_dsk(net, pdsk))
		goto l_err_dsk_get;

	*result_p = pdsk;

	return ZWP_STATUS_OK;

l_err_dsk_get:
	if (pdsk)
	{
		if (pdsk->dsk)
			ZWP_FREE(pdsk->dsk);
		ZWP_FREE(pdsk);
	}
l_err_dsk_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Set Avro value object with values from Result object corresponding to dsk during Security 2 Node Add
* @param[in]     result            Result object
* @param[out]    arg_value         Avro value object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_networks_node_add_dsk_get_result(void *result, avro_value_t *arg_value)
{
	sec2_dsk_cb_prm_t *pdsk = (sec2_dsk_cb_prm_t *)result;

	ZWP_LOG(ZWP_LOG_DEBUG, "zwp_avi_networks_node_add_dsk_get_result");
	if ((zw_avro_field_set_int(arg_value, "pin_required", pdsk->pin_required) != ZWP_STATUS_OK) ||
		(zw_avro_field_set_string(arg_value, "dsk", pdsk->dsk) != ZWP_STATUS_OK))
	{
		goto l_err_set_dsk_value;
	}

	return ZWP_STATUS_OK;

l_err_set_dsk_value:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free Result object corresponding to dsk during Security 2 Node Add
* @param[in]     result            Result object
* @return        Nothing
* @see zwp_avi_networks_operation_state_get_execute
*/
static void zwp_avi_networks_node_add_dsk_get_result_free(void *result)
{
	sec2_dsk_cb_prm_t *pdsk = (sec2_dsk_cb_prm_t *)result;

	if (pdsk)
	{
		if (pdsk->dsk)
			ZWP_FREE(pdsk->dsk);
		ZWP_FREE(pdsk);
	}
}

/**
* @brief Get argument for add node accept operation from Avro value object
* @param[in]     arg_value         Avro value object
* @param[out]    args_p            Arguments passed with Remove Failed Node operation
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
* @note
* @par
* The caller should call zwp_avi_networks_failed_node_remove_argument_free() to free resources
*/
static zwp_status_t zwp_avi_networks_node_add_accept_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_networks_node_add_accept_t *argument;

	if ((argument = (zwp_avi_networks_node_add_accept_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if ((zw_avro_field_get_int(arg_value, "accept", &argument->accept) != ZWP_STATUS_OK) ||
		(zw_avro_field_get_string(arg_value, "dsk", &argument->dsk) != ZWP_STATUS_OK))
	{
		goto l_err_get_value;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free arguments for add node accept operation
* @param[in]     args              Arguments passed with Remove Failed Node operation
* @return        Nothing
* @see zwp_avi_networks_failed_node_remove_argument
*/
static void zwp_avi_networks_node_add_accept_argument_free(void *args)
{
	zwp_avi_networks_node_add_accept_t *argument = (zwp_avi_networks_node_add_accept_t *)args;

	if (argument)
	{
		if (argument->dsk)
			ZWP_FREE(argument->dsk);

		ZWP_FREE(args);
	}
}

/**
* @brief Execute call to add node accept from network
* @param[in]     net               Network object
* @param[in]     arg               Arguments (if any)
* @param[out]    result_p          Result object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_networks_node_add_accept_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwp_avi_networks_node_add_accept_t *argument;
	zwnet_p zwnet;
	int hcapi_code;

	argument = (zwp_avi_networks_node_add_accept_t *)arg;

	ZWP_LOG(ZWP_LOG_DEBUG, "zwp_avi_networks_node_add_accept_execute accept=%d dsk=%s", argument->accept, argument->dsk);
	zwnet = zwp_net_get_zwnet(net);

	if ((hcapi_code = zwnet_add_sec2_accept(zwnet, argument->accept, argument->dsk)) != ZW_ERR_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwnet_add_sec2_accept()  with accept: %u and dsk: %s failed with '%s'", 
			argument->accept, argument->dsk, zwp_hcapi_error_code_to_string(hcapi_code));
		goto l_err_accept;
	}

	return ZWP_STATUS_OK;

l_err_accept:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute call to Remove Node from network
 * @param[in]     net               Network object
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_networks_node_remove_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwnet_p zwnet;
	int hcapi_code;

	zwnet = zwp_net_get_zwnet(net);

	if ((hcapi_code = zwnet_add(zwnet, 0, NULL, 0)) != ZW_ERR_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwnet_add() with 'remove' failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
		goto l_err_node_add;
	}

	zwp_net_set_operation(net, ZWNET_OP_RM_NODE);

	return ZWP_STATUS_OK;

l_err_node_add:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Get argument for Remove Failed Node operation from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Remove Failed Node operation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_networks_failed_node_remove_argument_free() to free resources
 */
static zwp_status_t zwp_avi_networks_failed_node_remove_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_networks_failed_node_remove_t *argument;

	if ((argument = (zwp_avi_networks_failed_node_remove_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "node_id", &argument->node_id) != ZWP_STATUS_OK)
	{
		goto l_err_get_value_node_id;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_get_value_node_id:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Remove Failed Node operation
 * @param[in]     args              Arguments passed with Remove Failed Node operation
 * @return        Nothing
 * @see zwp_avi_networks_failed_node_remove_argument
 */
static void zwp_avi_networks_failed_node_remove_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
 * @brief Execute call to Remove Failed Node from network
 * @param[in]     net               Network object
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_networks_failed_node_remove_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwp_avi_networks_failed_node_remove_t *argument;
	//zwp_desc_net_t desc_net;
	//zwnoded_p desc_node;
	zwnet_p zwnet;
	int hcapi_code;

	argument = (zwp_avi_networks_failed_node_remove_t *)arg;

	zwnet = zwp_net_get_zwnet(net);

	//desc_net = zwp_net_get_desc_net(net);

	//if (zwp_desc_get_desc_node(desc_net, argument->node_id, &desc_node) != ZWP_STATUS_OK)
	//{
	//	goto l_err_get_desc_node;
	//}

	if ((hcapi_code = zwnet_fail(zwnet, argument->node_id, 0, NULL, 0)) != ZW_ERR_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwnet_fail() with 'remove' failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
		goto l_err_node_fail;
	}

	zwp_net_set_operation(net, ZWNET_OP_RM_FAILED_ID);

	return ZWP_STATUS_OK;

l_err_node_fail:
//l_err_get_desc_node:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Get argument for Replace Failed Node operation from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Replace Failed Node operation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_networks_failed_node_replace_argument_free() to free resources
 */
static zwp_status_t zwp_avi_networks_failed_node_replace_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_networks_failed_node_replace_t *argument;

	if ((argument = (zwp_avi_networks_failed_node_replace_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "node_id", &argument->node_id) != ZWP_STATUS_OK)
	{
		goto l_err_get_value_node_id;
	}

	if (zw_avro_field_get_string(arg_value, "dsk", &argument->dsk) != ZWP_STATUS_OK || strlen(argument->dsk) == 0)
	{
		//dsk is optional
		if (argument->dsk)
			zw_avro_field_free_string(argument->dsk);
		argument->dsk = NULL;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_get_value_node_id:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Remove Failed Node operation
 * @param[in]     args              Arguments passed with Remove Failed Node operation
 * @return        Nothing
 * @see zwp_avi_networks_failed_node_replace_argument
 */
static void zwp_avi_networks_failed_node_replace_argument_free(void *args)
{
	if (args)
	{
		zwp_avi_networks_failed_node_replace_t *argument = args;

		if (argument->dsk)
			zw_avro_field_free_string(argument->dsk);

		ZWP_FREE(argument);
	}
}

/**
 * @brief Execute call to Replace Failed Node in a network
 * @param[in]     net               Network object
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_networks_failed_node_replace_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwp_avi_networks_failed_node_replace_t *argument;
	//zwp_desc_net_t desc_net;
	//zwnoded_p desc_node;
	zwnet_p zwnet;
	int hcapi_code;
	sec2_add_prm_t sec2_param;

	argument = (zwp_avi_networks_failed_node_replace_t *)arg;

	zwnet = zwp_net_get_zwnet(net);

	//desc_net = zwp_net_get_desc_net(net);

	sec2_param.usr_param = net;
	sec2_param.cb = add_node_sec2;
	sec2_param.dsk = NULL;
	if (argument && argument->dsk)
		sec2_param.dsk = argument->dsk;

	//if (zwp_desc_get_desc_node(desc_net, argument->node_id, &desc_node) != ZWP_STATUS_OK)
	//{
	//	goto l_err_get_desc_node;
	//}

	#ifdef ZWP_DESC_STATE_USE_HCAPI
	// Clear states before HCAPI resets the user data
	zwp_desc_interface_state_under_node_destroy(desc_node);
	zwp_node_state_destroy(desc_node);
	#endif

	if ((hcapi_code = zwnet_fail(zwnet, argument->node_id, 1, &sec2_param, 0)) != ZW_ERR_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwnet_fail() with 'replace' failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
		goto l_err_node_fail;
	}

	zwp_net_set_operation(net, ZWNET_OP_RP_NODE);

	return ZWP_STATUS_OK;

l_err_node_fail:
//l_err_get_desc_node:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Execute call to Replace Failed Node on-behalf in a network
* @param[in]     net               Network object
* @param[in]     arg               Arguments (if any)
* @param[out]    result_p          Result object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_networks_failed_node_replace_on_behalf_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwp_avi_networks_failed_node_replace_t *argument;
	//zwp_desc_net_t desc_net;
	//zwnoded_p desc_node;
	zwnet_p zwnet;
	int hcapi_code;
	sec2_add_prm_t sec2_param;

	argument = (zwp_avi_networks_failed_node_replace_t *)arg;

	zwnet = zwp_net_get_zwnet(net);
	//desc_net = zwp_net_get_desc_net(net);

	sec2_param.usr_param = net;
	sec2_param.cb = add_node_sec2;
	sec2_param.dsk = NULL;
	if (argument && argument->dsk)
		sec2_param.dsk = argument->dsk;

	//if (zwp_desc_get_desc_node(desc_net, argument->node_id, &desc_node) != ZWP_STATUS_OK)
	//{
	//	goto l_err_get_desc_node;
	//}

#ifdef ZWP_DESC_STATE_USE_HCAPI
	// Clear states before HCAPI resets the user data
	zwp_desc_interface_state_under_node_destroy(desc_node);
	zwp_node_state_destroy(desc_node);
#endif

	if ((hcapi_code = zwnet_fail(zwnet, argument->node_id, 1, &sec2_param, 1)) != ZW_ERR_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwnet_fail() with 'replace' failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
		goto l_err_node_fail;
	}

	zwp_net_set_operation(net, ZWNET_OP_RP_NODE);

	return ZWP_STATUS_OK;

l_err_node_fail:
//l_err_get_desc_node:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Get argument for Node Update operation from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Node Update operation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_networks_node_update_argument_free() to free resources
 */
static zwp_status_t zwp_avi_networks_node_update_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_networks_node_update_t *argument;

	if ((argument = (zwp_avi_networks_node_update_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "node_id", &argument->node_id) != ZWP_STATUS_OK)
	{
		goto l_err_get_value_node_id;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_get_value_node_id:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Node Update operation
 * @param[in]     args              Arguments passed with Node Update operation
 * @return        Nothing
 * @see zwp_avi_networks_node_update_argument
 */
static void zwp_avi_networks_node_update_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
 * @brief Execute call to Update Node in a network
 * @param[in]     net               Network object
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_networks_node_update_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwp_avi_networks_node_update_t *argument;
	zwp_desc_net_t desc_net;
	zwnoded_p desc_node;
	int hcapi_code;

	argument = (zwp_avi_networks_node_update_t *)arg;

	desc_net = zwp_net_get_desc_net(net);

	if (zwp_desc_get_desc_node(desc_net, argument->node_id, &desc_node) != ZWP_STATUS_OK)
	{
		goto l_err_get_desc_node;
	}

	zwp_net_set_node_updated_desc(net, *desc_node);

	#ifdef ZWP_DESC_STATE_USE_HCAPI
	// HCAPI seems to retain user data. So not destroying states.
	#endif

	if ((hcapi_code = zwnode_update(desc_node)) != ZW_ERR_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwnode_update() failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
		goto l_err_node_update;
	}

	zwp_net_set_operation(net, ZWNET_OP_NODE_UPDATE);

	return ZWP_STATUS_OK;

l_err_node_update:
l_err_get_desc_node:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Get argument for Send NIF operation from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Send NIF operation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_networks_send_NIF_argument_free() to free resources
 */
static zwp_status_t zwp_avi_networks_send_NIF_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_networks_send_NIF_t *argument;

	if ((argument = (zwp_avi_networks_send_NIF_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if ((zw_avro_field_get_int(arg_value, "node_id", &argument->node_id) != ZWP_STATUS_OK)
		|| (zw_avro_field_get_int(arg_value, "is_broadcast", &argument->is_broadcast) != ZWP_STATUS_OK))
	{
		goto l_err_get_value_node_id;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_get_value_node_id:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Send NIF operation
 * @param[in]     args              Arguments passed with Send NIF operation
 * @return        Nothing
 * @see zwp_avi_networks_send_NIF_argument
 */
static void zwp_avi_networks_send_NIF_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
 * @brief Execute call to send NIF
 * @param[in]     net               Network object
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_networks_send_NIF_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwp_avi_networks_send_NIF_t *argument;
	zwnet_p zwnet;
	zwp_desc_net_t desc_net;
	zwnoded_p desc_node;
	int hcapi_code;

	argument = (zwp_avi_networks_send_NIF_t *)arg;

	zwnet = zwp_net_get_zwnet(net);

	desc_net = zwp_net_get_desc_net(net);

	if (!argument->is_broadcast
			&& (zwp_desc_get_desc_node(desc_net, argument->node_id, &desc_node) != ZWP_STATUS_OK))
	{
		goto l_err_get_desc_node;
	}

	if ((hcapi_code = zwnet_send_nif(zwnet, desc_node, argument->is_broadcast)) != ZW_ERR_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwnet_send_nif() failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
		goto l_err_send_nif;
	}

	zwp_net_set_operation(net, ZWNET_OP_SEND_NIF);

	return ZWP_STATUS_OK;

l_err_send_nif:
l_err_get_desc_node:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute call to Update Network
 * @param[in]     net               Network object
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_networks_update_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwnet_p zwnet;
	int hcapi_code;
	#ifdef ZWP_DESC_STATE_USE_HCAPI
	zwp_desc_net_t desc_net;
	#endif

	zwnet = zwp_net_get_zwnet(net);

	#ifdef ZWP_DESC_STATE_USE_HCAPI
	desc_net = zwp_net_get_desc_net(net);

	// Clear states before HCAPI resets the user data
	zwp_desc_interface_state_under_net_destroy(desc_net);
	#endif

	if ((hcapi_code = zwnet_update(zwnet)) != ZW_ERR_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwnet_update() failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
		goto l_err_update;
	}

	zwp_net_set_operation(net, ZWNET_OP_UPDATE);

	return ZWP_STATUS_OK;

l_err_update:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute call to Initiate Network
 * @param[in]     net               Network object
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_networks_initiate_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwnet_p zwnet;
	int hcapi_code;
	#ifdef ZWP_DESC_STATE_USE_HCAPI
	zwp_desc_net_t desc_net;
	#endif

	zwnet = zwp_net_get_zwnet(net);

	#ifdef ZWP_DESC_STATE_USE_HCAPI
	desc_net = zwp_net_get_desc_net(net);

	// Clear states before HCAPI resets the user data
	zwp_desc_interface_state_under_net_destroy(desc_net);
	zwp_desc_node_state_under_net_destroy(desc_net);
	#endif

	if ((hcapi_code = zwnet_initiate(zwnet, get_local_node_dsk, net)) != ZW_ERR_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwnet_initiate() failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
		goto l_err_initiate;
	}

	zwp_net_set_operation(net, ZWNET_OP_INITIATE);

	return ZWP_STATUS_OK;

l_err_initiate:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Execute call to get local node dsk during Security 2 network initiate process
* @param[in]     net               Network object
* @param[in]     arg               Arguments (if any)
* @param[out]    result_p          Result object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*
*@note
* @par
* The caller should call zwp_avi_networks_initiate_local_dsk_get_result_free() to free resources
*/
static zwp_status_t zwp_avi_networks_initiate_local_dsk_get_execute(zwp_net_t net, void *arg, void **result_p)
{
	char *pdsk;

	ZWP_LOG(ZWP_LOG_DEBUG, "zwp_avi_networks_initiate_local_dsk_get_execute");

	if (!zwp_net_get_local_node_dsk(net, &pdsk))
		goto l_err_local_dsk_get;

	*result_p = pdsk;

	return ZWP_STATUS_OK;

l_err_local_dsk_get:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Set Avro value object with values from Result object corresponding to dsk during network initiate
* @param[in]     result            Result object
* @param[out]    arg_value         Avro value object
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_networks_initiate_local_dsk_get_result(void *result, avro_value_t *arg_value)
{
	char *pdsk = (char *)result;

	ZWP_LOG(ZWP_LOG_DEBUG, "zwp_avi_networks_initiate_local_dsk_get_result");
	if (zw_avro_field_set_string(arg_value, "local_dsk", pdsk) != ZWP_STATUS_OK)
	{
		goto l_err_set_dsk_value;
	}

	return ZWP_STATUS_OK;

l_err_set_dsk_value:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free Result object corresponding to dsk during network initiate
* @param[in]     result            Result object
* @return        Nothing
* @see zwp_avi_networks_operation_state_get_execute
*/
static void zwp_avi_networks_initiate_local_dsk_get_result_free(void *result)
{
	char *pdsk = (char *)result;

	if (pdsk)
	{
		ZWP_FREE(pdsk);
	}
}

/**
 * @brief Execute call to Migrate Primary
 * @param[in]     net               Network object
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_networks_migrate_primary_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwnet_p zwnet;
	int hcapi_code;
	#ifdef ZWP_DESC_STATE_USE_HCAPI
	zwp_desc_net_t desc_net;
	#endif

	zwnet = zwp_net_get_zwnet(net);

	#ifdef ZWP_DESC_STATE_USE_HCAPI
	desc_net = zwp_net_get_desc_net(net);

	// Clear states before HCAPI resets the user data
	zwp_desc_interface_state_under_net_destroy(desc_net);
	zwp_desc_node_state_under_net_destroy(desc_net);
	#endif

	if ((hcapi_code = zwnet_migrate(zwnet)) != ZW_ERR_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwnet_migrate() failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
		goto l_err_migrate_primary;
	}

	zwp_net_set_operation(net, ZWNET_OP_MIGRATE);

	return ZWP_STATUS_OK;

l_err_migrate_primary:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute call to Reset Network
 * @param[in]     net               Network object
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_networks_reset_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwnet_p zwnet;
	int hcapi_code;
	#ifdef ZWP_DESC_STATE_USE_HCAPI
	zwp_desc_net_t desc_net;
	#endif

	zwnet = zwp_net_get_zwnet(net);

	#ifdef ZWP_DESC_STATE_USE_HCAPI
	desc_net = zwp_net_get_desc_net(net);

	// Clear states before HCAPI resets the user data
	zwp_desc_interface_state_under_net_destroy(desc_net);
	zwp_desc_node_state_under_net_destroy(desc_net);
	#endif

	if ((hcapi_code = zwnet_reset(zwnet)) != ZW_ERR_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwnet_reset() failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
		goto l_err_reset;
	}

	zwp_net_set_operation(net, ZWNET_OP_RESET);

	return ZWP_STATUS_OK;

l_err_reset:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute call to abort network operation
 * @param[in]     net               Network object
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_networks_operation_abort_execute(zwp_net_t net, void *arg, void **result_p)
{
	zwnet_p zwnet;
	int hcapi_code;

	zwnet = zwp_net_get_zwnet(net);

	if ((hcapi_code = zwnet_abort(zwnet)) != ZW_ERR_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwnet_abort() failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
		goto l_err_abort;
	}

	if (zwp_net_abort_operation(net, ZWP_STATUS_OK) != ZWP_STATUS_OK)
	{
		goto l_err_abort_operation;
	}

	return ZWP_STATUS_OK;

l_err_abort:
	zwp_net_abort_operation(net, ZWP_STATUS_ERROR);

l_err_abort_operation:
	return ZWP_STATUS_ERROR;
}

/** Handlers for prekit node dsk info operation */
static zwp_avi_message_networks_t networks_node_prekit_dsk_get =
{
		"network_node_prekit_dsk_get",
		NULL, NULL,
		zwp_avi_networks_node_prekit_dsk_get_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
		zwp_avi_networks_node_prekit_dsk_get_result, zwp_avi_networks_node_prekit_dsk_get_result_free
};

/** Handlers for Add Node operation */
static zwp_avi_message_networks_t networks_node_add =
{
		"network_node_add",
		zwp_avi_networks_node_add_argument, zwp_avi_networks_node_add_argument_free,
		zwp_avi_networks_node_add_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
		NULL, NULL
};

/** Handlers for Add Node on-behalf operation */
static zwp_avi_message_networks_t networks_node_add_on_behalf =
{
		"network_node_add_on_behalf",
		//Use add node argument and free function as they are identical
		zwp_avi_networks_node_add_argument, zwp_avi_networks_node_add_argument_free,
		zwp_avi_networks_node_add_on_behalf_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
		NULL, NULL
};

/** Handlers for Add Node security 2 get requested key operation */
static zwp_avi_message_networks_t networks_node_add_req_key_get =
{
		"network_node_add_req_key_get",
		NULL, NULL,
		zwp_avi_networks_node_add_req_key_get_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
		zwp_avi_networks_node_add_req_key_get_result, zwp_avi_networks_node_add_req_key_get_result_free
};

/** Handlers for Add Node security 2 set grant key operation */
static zwp_avi_message_networks_t networks_node_add_grant_key_set =
{
		"network_node_add_grant_key_set",
		zwp_avi_networks_node_add_grant_key_argument, zwp_avi_networks_node_add_grant_key_argument_free,
		zwp_avi_networks_node_add_grant_key_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
		NULL, NULL
};

/** Handlers for Add Node security 2 get dsk operation */
static zwp_avi_message_networks_t networks_node_add_dsk_get =
{
		"network_node_add_dsk_get",
		NULL, NULL,
		zwp_avi_networks_node_add_dsk_get_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
		zwp_avi_networks_node_add_dsk_get_result, zwp_avi_networks_node_add_dsk_get_result_free
};

/** Handlers for Add Node security 2 accept operation */
static zwp_avi_message_networks_t networks_node_add_accept =
{
		"network_node_add_accept",
		zwp_avi_networks_node_add_accept_argument, zwp_avi_networks_node_add_accept_argument_free,
		zwp_avi_networks_node_add_accept_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
		NULL, NULL
};

/** Handlers for Remove Node operation */
static zwp_avi_message_networks_t networks_node_remove =
{
		"network_node_remove",
		NULL, NULL,
		zwp_avi_networks_node_remove_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
		NULL, NULL
};

/** Handlers for Remove Node operation */
static zwp_avi_message_networks_t networks_failed_node_remove =
{
		"network_failed_node_remove",
		zwp_avi_networks_failed_node_remove_argument, zwp_avi_networks_failed_node_remove_argument_free,
		zwp_avi_networks_failed_node_remove_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
		NULL, NULL
};

/** Handlers for replace failed node operation */
static zwp_avi_message_networks_t networks_failed_node_replace =
{
		"network_failed_node_replace",
		zwp_avi_networks_failed_node_replace_argument, zwp_avi_networks_failed_node_replace_argument_free,
		zwp_avi_networks_failed_node_replace_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
		NULL, NULL
};

/** Handlers for replace failed node on-behalf operation */
static zwp_avi_message_networks_t networks_failed_node_replace_on_behalf =
{
		"network_failed_node_replace_on_behalf",
		zwp_avi_networks_failed_node_replace_argument, zwp_avi_networks_failed_node_replace_argument_free,
		zwp_avi_networks_failed_node_replace_on_behalf_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
		NULL, NULL
};

/** Handlers for Node Update operation */
static zwp_avi_message_networks_t networks_node_update =
{
		"network_node_update",
		zwp_avi_networks_node_update_argument, zwp_avi_networks_node_update_argument_free,
		zwp_avi_networks_node_update_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
		NULL, NULL
};

/** Handlers for Send NIF operation */
static zwp_avi_message_networks_t networks_send_NIF =
{
		"network_send_NIF",
		zwp_avi_networks_send_NIF_argument, zwp_avi_networks_send_NIF_argument_free,
		zwp_avi_networks_send_NIF_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
		NULL, NULL
};

/** Handlers for Network Update operation */
static zwp_avi_message_networks_t networks_update =
{
		"network_update",
		NULL, NULL,
		zwp_avi_networks_update_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
		NULL, NULL
};

/** Handlers for Network Initiate operation */
static zwp_avi_message_networks_t networks_initiate =
{
		"network_initiate",
		NULL, NULL,
		zwp_avi_networks_initiate_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
		NULL, NULL
};

/** Handlers for Network initiate security 2 get local node dsk operation */
static zwp_avi_message_networks_t networks_initiate_local_dsk_get =
{
		"network_initiate_local_dsk_get",
		NULL, NULL,
		zwp_avi_networks_initiate_local_dsk_get_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
		zwp_avi_networks_initiate_local_dsk_get_result, zwp_avi_networks_initiate_local_dsk_get_result_free
};

/** Handlers for Migrate Primary operation */
static zwp_avi_message_networks_t networks_migrate_primary =
{
		"network_migrate_primary",
		NULL, NULL,
		zwp_avi_networks_migrate_primary_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
		NULL, NULL
};

/** Handlers for Network Reset operation */
static zwp_avi_message_networks_t networks_reset =
{
		"network_reset",
		NULL, NULL,
		zwp_avi_networks_reset_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
		NULL, NULL
};

/** Handlers for abort operation */
static zwp_avi_message_networks_t networks_operation_abort =
{
		"network_operation_abort",
		NULL, NULL,
		zwp_avi_networks_operation_abort_execute, ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,
		NULL, NULL
};

/** Register handlers */
zwp_status_t zwp_avi_networks_operations_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_message_list_add_message_networks(message_list, &networks_node_prekit_dsk_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_networks(message_list, &networks_node_add) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_networks(message_list, &networks_node_add_on_behalf) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_networks(message_list, &networks_node_add_req_key_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_networks(message_list, &networks_node_add_grant_key_set) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_networks(message_list, &networks_node_add_dsk_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_networks(message_list, &networks_node_add_accept) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_networks(message_list, &networks_node_remove) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_networks(message_list, &networks_failed_node_remove) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_networks(message_list, &networks_failed_node_replace) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_networks(message_list, &networks_failed_node_replace_on_behalf) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_networks(message_list, &networks_node_update) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_networks(message_list, &networks_update) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_networks(message_list, &networks_send_NIF) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_networks(message_list, &networks_initiate) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_networks(message_list, &networks_initiate_local_dsk_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_networks(message_list, &networks_migrate_primary) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_networks(message_list, &networks_reset) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_networks(message_list, &networks_operation_abort) != ZWP_STATUS_OK))
	{
		goto l_err_info_get;
	}

	return ZWP_STATUS_OK;

l_err_info_get:
	return ZWP_STATUS_ERROR;
}

