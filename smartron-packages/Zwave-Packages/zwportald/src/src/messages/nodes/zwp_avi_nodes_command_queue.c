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
 * @file    zwp_avi_nodes_command_queue.c
 *
 * @brief   AVI message 'node_command_queue_xxxx'
 * @details Handles the AVI message 'node_command_queue_xxxx'.
 *
 * @author  Sasidhar
 *
 * @version 1.0 - 2013-11-27
 * - Initial version
 */

#include "zwp_status.h"
#include "zwp_avi_message_nodes.h"
#include "zwp_log.h"
#include "zwp_memory.h"
#include "zwp_error.h"
#include "zwu_number.h"
#include "zwp_hcapi.h"
#include "zwp_descriptor.h"

/** Result Command Queue State */
typedef struct {
	uint8_t            state ;		/**< State (enable/disable) of command queue */
} zwp_avi_nodes_commandqueue_enable_get_result_t;

/** Set Command Queue State */
typedef struct {
	uint8_t            state;		/**< State (enable/disable) of command queue */
} zwp_avi_nodes_commandqueue_enable_set_t;

/** Result Set Command Queue State */
typedef struct {
	uint8_t            state;		/**< State (enable/disable) of command queue */
} zwp_avi_nodes_commandqueue_enable_set_result_t;

/** Report Get Command Queue  */
typedef struct {
	uint16_t		*commandq ;		/**< Command queue list */
	int 			count;			/**< Count of pending commands in the command queue list  */
} zwp_avi_nodes_commandqueue_get_result_t;


/**
 * @brief Get the node command queue enable state.
 * @param[in]     desc_node         Node descriptor
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_nodes_commandqueue_enable_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_nodes_commandqueue_enable_get_execute(zwnoded_p desc_node, void *arg, void **result_p)
{
	zwp_avi_nodes_commandqueue_enable_get_result_t *result;

	if ((result = (zwp_avi_nodes_commandqueue_enable_get_result_t *)ZWP_MALLOC(sizeof(*result))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_result_alloc;
	}

	if (zwnode_cmd_q_ena_get(desc_node, &result->state) != ZW_ERR_NONE)
	{
		goto l_err_get_commandqueue_enable;
	}

	*result_p = result;

	return ZWP_STATUS_OK;

l_err_get_commandqueue_enable:
	ZWP_FREE(result);
l_err_result_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to command queue get enable state
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_nodes_commandqueue_enable_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_nodes_commandqueue_enable_get_result_t *result_p = (zwp_avi_nodes_commandqueue_enable_get_result_t *) result;

	if(zw_avro_field_set_int(arg_value, "state", result_p->state) != ZWP_STATUS_OK)
	{
		goto l_err_set_commandqueue_enable_value;
	}

	return ZWP_STATUS_OK;

l_err_set_commandqueue_enable_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to command queue enable sate
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_nodes_commandqueue_enable_get_execute
 */
static void zwp_avi_nodes_commandqueue_enable_get_result_free(void *result)
{
	ZWP_FREE(result);
}

/**
 * @brief Set argument for command queue enable set command from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Set command
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_nodes_commandqueue_enable_set_get_argument_free() to free resources
 */
zwp_status_t zwp_avi_nodes_commandqueue_enable_set_get_argument (avro_value_t *arg_value, void **args_p)
{
	int integer;
	zwp_avi_nodes_commandqueue_enable_set_t *argument;

	if ((argument = (zwp_avi_nodes_commandqueue_enable_set_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if(zw_avro_field_get_int(arg_value, "state", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_commandq_state;
	}

	if (zwu_number_is_uint8(integer) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid enable state: %d", integer);
		goto l_err_invalid_value;
	}

	argument->state = (uint8_t)integer;

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_invalid_value:
l_err_commandq_state:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;

}

/**
 * @brief Free arguments for command queue enable set command
 * @param[in]     args              Arguments passed with Set command
 * @return        Nothing
 * @see zwp_avi_nodes_commandqueue_enable_set_get_argument
 */
void zwp_avi_nodes_commandqueue_enable_set_get_argument_free(void *args)
{
	ZWP_FREE(args);
	return;
}

/**
 * @brief Set the node command queue enable state.
 * @param[in]     desc_node         Node descriptor
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_nodes_commandqueue_enable_set_result_free() to free resources
 */
static zwp_status_t zwp_avi_nodes_commandqueue_enable_set_execute(zwnoded_p desc_node, void *arg, void **result_p)
{
	zwp_avi_nodes_commandqueue_enable_set_t *argument;
	zwp_avi_nodes_commandqueue_enable_set_result_t *result;

	argument = (zwp_avi_nodes_commandqueue_enable_set_t *)arg;

	if (zwnode_cmd_q_ena_set(desc_node, argument->state) != ZW_ERR_NONE)
	{
		goto l_err_get_commandqueue_enable;
	}

	if ((result = (zwp_avi_nodes_commandqueue_enable_set_result_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_result_alloc;
	}

	result->state = argument->state;

	*result_p = result;

	return ZWP_STATUS_OK;

l_err_get_commandqueue_enable:
l_err_result_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding set command queue enable state
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_nodes_commandqueue_enable_set_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_nodes_commandqueue_enable_set_result_t *result_p = (zwp_avi_nodes_commandqueue_enable_set_result_t *) result;

	if(zw_avro_field_set_int(arg_value, "state", result_p->state) != ZWP_STATUS_OK)
	{
		goto l_err_set_commandqueue_enable_value;
	}

	return ZWP_STATUS_OK;

l_err_set_commandqueue_enable_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to command queue enable set
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_nodes_commandqueue_enable_set_execute
 */
static void zwp_avi_nodes_commandqueue_enable_set_result_free(void *result)
{
	ZWP_FREE(result);
}

/**
 * @brief Get the list of commands in the command queue for the give node
 * @param[in]     desc_node         Node descriptor
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_nodes_commandqueue_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_nodes_commandqueue_get_execute(zwnoded_p desc_node, void *arg, void **result_p)
{
	zwp_avi_nodes_commandqueue_get_result_t *result;

	if ((result = (zwp_avi_nodes_commandqueue_get_result_t *)ZWP_MALLOC(sizeof(*result))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_result_alloc;
	}

	result->count = zwnode_cmd_q_get(desc_node,&result->commandq);

	*result_p = result;

	return ZWP_STATUS_OK;

l_err_result_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Get Avro value object with values from Result object corresponding to nodes command queue get
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_nodes_commandqueue_get_result(void *result, avro_value_t *arg_value)
{
	avro_value_t commandq_list_value;
	avro_value_t commandq_value;
	zwp_avi_nodes_commandqueue_get_result_t *result_p;
	int i;

	result_p = (zwp_avi_nodes_commandqueue_get_result_t *)result;

	if (avro_value_get_by_name(arg_value, "commandqueue_list", &commandq_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'commandq_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	for (i = 0; i < result_p->count; i++)
	{
		if (avro_value_append(&commandq_list_value, &commandq_value, 0) != 0)
		{
			goto l_err_append_commandq_list_value;
		}

		if (avro_value_set_int(&commandq_value, result_p->commandq[i]) != 0)
		{
			goto l_err_set_commandq_value;
		}
	}

	return ZWP_STATUS_OK;

l_err_append_commandq_list_value:
l_err_set_commandq_value:
l_err_get_list_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to command queue get result
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_nodes_commandqueue_get_execute
 */
static void zwp_avi_nodes_commandqueue_get_result_free(void *result)
{
	zwp_avi_nodes_commandqueue_get_result_t *result_p;

	result_p = (zwp_avi_nodes_commandqueue_get_result_t *)result;

	if(result_p->count > 0)
	{
		ZWP_FREE(result_p->commandq);
	}
	ZWP_FREE(result);
}

/**
 * @brief Execute canceling of all the pending commands in the command queue of given node descriptor
 * @param[in]     desc_node         Node descriptor
 * @param[in]     arg               Arguments (if any)
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 */
static zwp_status_t zwp_avi_nodes_commandqueue_cancel_execute(zwnoded_p desc_node, void *arg, void **result_p)
{
	if (zwnode_cmd_q_cancel(desc_node) == ZW_ERR_NONE)
	{
		return ZWP_STATUS_OK;
	}
	else
	{
		return ZWP_STATUS_ERROR;
	}
}

/** Handlers for getting command queue state */
static zwp_avi_message_nodes_t nodes_commandqueue_enable_get =
{
		"node_commandqueue_enable_get",
		NULL, NULL,
		zwp_avi_nodes_commandqueue_enable_get_execute,
		zwp_avi_nodes_commandqueue_enable_get_result, zwp_avi_nodes_commandqueue_enable_get_result_free
};

/** Handlers for setting command queue state */
static zwp_avi_message_nodes_t nodes_commandqueue_enable_set =
{
		"node_commandqueue_enable_set",
		zwp_avi_nodes_commandqueue_enable_set_get_argument, zwp_avi_nodes_commandqueue_enable_set_get_argument_free,
		zwp_avi_nodes_commandqueue_enable_set_execute,
		zwp_avi_nodes_commandqueue_enable_set_result, zwp_avi_nodes_commandqueue_enable_set_result_free
};

/** Handlers for getting command queue list */
static zwp_avi_message_nodes_t nodes_commandqueue_get =
{
		"node_commandqueue_get",
		NULL, NULL,
		zwp_avi_nodes_commandqueue_get_execute,
		zwp_avi_nodes_commandqueue_get_result, zwp_avi_nodes_commandqueue_get_result_free
};

/** Handlers for canceling command queue list */
static zwp_avi_message_nodes_t nodes_commandqueue_cancel =
{
		"node_commandqueue_cancel",
		NULL, NULL,
		zwp_avi_nodes_commandqueue_cancel_execute,
		NULL, NULL
};

/** Register handlers */
zwp_status_t zwp_avi_nodes_commandqueue_enable_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_message_list_add_message_nodes(message_list, &nodes_commandqueue_enable_get) != ZWP_STATUS_OK) ||
		(zwp_avi_message_list_add_message_nodes(message_list, &nodes_commandqueue_enable_set) != ZWP_STATUS_OK) ||
		(zwp_avi_message_list_add_message_nodes(message_list, &nodes_commandqueue_get) != ZWP_STATUS_OK)||
		(zwp_avi_message_list_add_message_nodes(message_list, &nodes_commandqueue_cancel) != ZWP_STATUS_OK)
	   )
	{
		goto l_err_commandqueue_enable_get;
	}
	return ZWP_STATUS_OK;

l_err_commandqueue_enable_get:
	return ZWP_STATUS_ERROR;
}

