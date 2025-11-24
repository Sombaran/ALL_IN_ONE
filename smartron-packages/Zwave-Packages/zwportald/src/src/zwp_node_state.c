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
 * @file    zwp_node_state.c
 *
 * @brief   Node level data
 * @details Handles the node level data.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-01-07
 * - Initial version
 *
 * @version 1.1 - 2014-01-21
 * - is_secure flag  --Sasidhar
 *
 * @version 1.2 - 2014-01-24
 * - Version command class version 2  --Sasidhar
 */

#include "zwp_log.h"
#if defined(ZWP_DEBUG_NODE_STATE)
#include "zwp_log_enable.h"
#else
#include "zwp_log_disable.h"
#endif

#include "zwp_node_state.h"
#include "zwp_memory.h"
#include "zwp_error.h"
#include "zwp_descriptor.h"

/** Target restart status after firmware update class */
struct _firmware_update_target_restart_status
{
	zwu_time_t         restart_begin_time;   /**< Time at which the restart began */
	int                restart_wait_time;    /**< Waiting time for target to complete restart */

	zwu_time_t         status_utime;         /**< Last updated time of target restart status */
	int                status;               /**< Firmware update target availability status. Refer HCAPI ZW_FW_UPDT_RESTART_XXX */
};

/** Node level state class */
struct _zwp_node_state
{
	uint8_t            alive_state;          /**< Node status: alive or down or sleeping */
	uint8_t            secure;               /**< Node type: secure or non-secure */
	ext_ver_t          *ext_ver;			 /**< Node version info */

	zwu_array_list_t   firmware_update_target_restart_status_listeners; /**< List of listeners for Firmware update target restart status */
};

/** Type of node level state change */
typedef enum
{
	node_state_create = 0,
	node_state_refresh
} zwp_node_state_update_type;

/**
 * @name Getters
 * @{
 */
uint8_t zwp_node_state_is_alive(zwp_node_state_t node_state)
{
	return node_state->alive_state;
}

uint8_t zwp_node_state_is_secure(zwp_node_state_t node_state)
{
	return node_state->secure;
}

ext_ver_t *zwp_node_state_get_extended_version(zwp_node_state_t node_state)
{
	return node_state->ext_ver;
}
/**
 * @}
 */

void zwp_firmware_update_target_restart_status_get(zwp_firmware_update_target_restart_status_t restart_status, zwu_time_t *status_utime_p, uint8_t *status_p)
{
	*status_utime_p = restart_status->status_utime;
	*status_p = restart_status->status;
}

/**
 * @brief Set target restart status after firmware update
 * @param[in]     restart_status    Target restart status object
 * @param[in]     status_utime      Last updated time of status
 * @param[in]     status            Target restart status
 * @return        Nothing
 */
static void zwp_firmware_update_target_restart_status_set(zwp_firmware_update_target_restart_status_t restart_status, zwu_time_t status_utime, uint8_t status)
{
	restart_status->status_utime = status_utime;
	restart_status->status = status;
}

zwp_status_t zwp_firmware_update_target_restart_status_alloc(zwp_firmware_update_target_restart_status_t *restart_status_p)
{
	zwp_firmware_update_target_restart_status_t restart_status;

	if ((restart_status = (zwp_firmware_update_target_restart_status_t)ZWP_CALLOC(1, sizeof(*restart_status))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_firmware_update_target_restart_status_alloc;
	}

	*restart_status_p = restart_status;

	return ZWP_STATUS_OK;

l_err_firmware_update_target_restart_status_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_firmware_update_target_restart_status_free(zwp_firmware_update_target_restart_status_t restart_status)
{
	ZWP_FREE(restart_status);
}

zwp_status_t zwp_node_state_add_firmware_update_restart_status_listener(zwp_node_state_t node_state, zwp_firmware_update_target_restart_status_t restart_status,
		zwu_time_t restart_begin_time, int restart_wait_time)
{
	// Remove any entry that already exists
	zwu_array_list_remove(node_state->firmware_update_target_restart_status_listeners, restart_status);

	if (zwu_array_list_add(node_state->firmware_update_target_restart_status_listeners, restart_status) != ZWP_STATUS_OK)
	{
		goto l_err_target_restart_status_listeners_add;
	}

	restart_status->restart_begin_time = restart_begin_time;
	restart_status->restart_wait_time = restart_wait_time;

	return ZWP_STATUS_OK;

l_err_target_restart_status_listeners_add:
	return ZWP_STATUS_ERROR;
}

void zwp_node_state_event_firmware_update_target_restart_status(zwp_node_state_t node_state, zwu_time_t status_utime, uint8_t status)
{
	int listeners_count;
	int i;
	zwp_firmware_update_target_restart_status_t restart_status;
	zwu_time_t time_elapsed;

	listeners_count = zwu_array_list_size(node_state->firmware_update_target_restart_status_listeners);

	i = 0;

	ZWP_LOG(ZWP_LOG_DEBUG, "Firmware update target restart availability event: %s [time: %llu]",
			zwp_hcapi_firmware_update_target_restart_status_to_string(status), status_utime);

	while (i < listeners_count)
	{
		restart_status = zwu_array_list_get(node_state->firmware_update_target_restart_status_listeners, i);

		if (status == ZW_FW_UPDT_RESTART_OK)
		{
			zwp_firmware_update_target_restart_status_set(restart_status, status_utime, status);

			ZWP_LOG(ZWP_LOG_DEBUG, "Removing listener: Restart Begin Time: %llu. Restart Wait Time: %u seconds",
					restart_status->restart_begin_time, restart_status->restart_wait_time);

			zwu_array_list_remove_at_index(node_state->firmware_update_target_restart_status_listeners, i);
			listeners_count--;
		}
		else
		{
			// Time elapsed between beginning of node restart and the reception of node availability status
			time_elapsed = (status_utime > restart_status->restart_begin_time) ? (status_utime - restart_status->restart_begin_time) : 0;

			ZWP_LOG(ZWP_LOG_DEBUG, "Time elapsed between beginning of restart and node availability event: %llu", time_elapsed);

			if (time_elapsed > restart_status->restart_wait_time)
			{
				zwp_firmware_update_target_restart_status_set(restart_status, status_utime, status);

				ZWP_LOG(ZWP_LOG_DEBUG, "Removing listener: Restart Begin Time: %llu. Restart Wait Time: %u seconds",
						restart_status->restart_begin_time, restart_status->restart_wait_time);

				zwu_array_list_remove_at_index(node_state->firmware_update_target_restart_status_listeners, i);
				listeners_count--;
			}
			else
			{
				ZWP_LOG(ZWP_LOG_DEBUG, "Not removing listener: Restart Begin Time: %llu. Restart Wait Time: %u seconds",
						restart_status->restart_begin_time, restart_status->restart_wait_time);
				i++;
			}
		}
	}
}

/**
 * @brief Allocate and initialize a new node level state object
 * @param[out]    node_state_p      Node level state object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_node_state_free() to free resources
 */
static zwp_status_t zwp_node_state_alloc(zwp_node_state_t *node_state_p)
{
	zwp_node_state_t node_state;

	if ((node_state = (zwp_node_state_t)ZWP_MALLOC(sizeof(*node_state))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_node_state_alloc;
	}

	// Defaults
	node_state->alive_state = ZWNET_NODE_STATUS_ALIVE;
	node_state->secure = 0;
	node_state->ext_ver = NULL;

	*node_state_p = node_state;

	return ZWP_STATUS_OK;

	ZWP_FREE(node_state);
l_err_node_state_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_node_state_free(zwp_node_state_t node_state)
{
	// Use 'free()' instead of ZWP_FREE() as this memory is allocated by HCAPI.
	free(node_state->ext_ver);

	ZWP_FREE(node_state);
}

/**
 * @brief Duplicate a node level state object
 * @param[in]     original          The original node level state object
 * @param[out]    duplicate_p       The duplicated node level state object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_node_state_free() on *duplicate_p to free resources
 */
static zwp_status_t zwp_node_state_duplicate(zwp_node_state_t original, zwp_node_state_t *duplicate_p)
{
	zwp_node_state_t duplicate;
	size_t size;

	if (zwp_node_state_alloc(&duplicate) != ZWP_STATUS_OK)
	{
		goto l_err_node_state_alloc;
	}

	memcpy(duplicate, original, sizeof(*duplicate));

	if (original->ext_ver)
	{
		// Size of ext_ver_t and the attached variable length memory
		size = sizeof(*(original->ext_ver)) +
				(sizeof(original->ext_ver->fw_ver) * (original->ext_ver->fw_cnt - 1));

		// Deep copy. Not using ZWP_MALLOC() since HCAPI memory is using malloc()
		if ((duplicate->ext_ver = (ext_ver_t *)malloc(size)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_ext_ver_alloc;
		}

		// This copy extends beyond ext_ver_t into the attached variable length memory
		memcpy(duplicate->ext_ver, original->ext_ver, size);
	}

	*duplicate_p = duplicate;

	return ZWP_STATUS_OK;

	if (original->ext_ver)
	{
		free(duplicate->ext_ver);
	}
l_err_ext_ver_alloc:
	zwp_node_state_free(duplicate);
l_err_node_state_alloc:
	return ZWP_STATUS_ERROR;
}

zwp_node_state_t zwp_node_state_get(zwnoded_p desc_node)
{
	#ifdef ZWP_DESC_STATE_USE_HCAPI
	return zwnode_get_user(desc_node);
	#else
	return zwp_desc_node_get_user(desc_node);
	#endif
}

/**
 * @brief Set node level state object to HCAPI node descriptor
 * @param[in]     desc_node         HCAPI node descriptor
 * @param[in]     state             Node level state object
 * @return        Nothing
 */
static void zwp_node_state_set(zwnoded_p desc_node, zwp_node_state_t state)
{
	#ifdef ZWP_DESC_STATE_USE_HCAPI
	zwnode_set_user(desc_node, state);
	#else
	zwp_desc_node_set_user(desc_node, state);
	#endif
}

/**
 * @brief Update node level state of a given node descriptor
 * @param[in]     desc_node         HCAPI node descriptor
 * @param[in]     update_type       The type of update - node_state_xxx
 * @param[out]    state_copy_p      If not NULL, live state is returned even if saved state is absent.
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_node_state_free() on *state_copy_p to free resources
 */
static zwp_status_t zwp_node_state_update(zwnoded_p desc_node,
		zwp_node_state_update_type update_type, zwp_node_state_t *state_copy_p)
{
	zwp_node_state_t node_state;
	uint8_t alive_state, hcapi_state;

	if ((node_state = zwp_node_state_get(desc_node)) == NULL)
	{
		if (zwp_node_state_alloc(&node_state) != ZWP_STATUS_OK)
		{
			goto l_err_node_state_alloc;
		}

		// If update_type is not to create, just return with success.
		// If state_copy is requested, pass the allocated state with default states. Else free the allocated state.
		if (update_type != node_state_create)
		{
			if (state_copy_p)
			{
				*state_copy_p = node_state;
			}
			else
			{
				zwp_node_state_free(node_state);
			}

			return ZWP_STATUS_OK;
		}

		// If update_type is node_state_create
		if (zwu_array_list_create(0, &node_state->firmware_update_target_restart_status_listeners) != ZWP_STATUS_OK)
		{
			zwp_node_state_free(node_state);

			goto l_err_target_restart_status_listeners_create;
		}

		zwp_node_state_set(desc_node, node_state);
	}

	alive_state = node_state->alive_state;
	/*
	 * Live states: Portion of node_state that is updated both during 'create' and 'refresh'
	 */
	// State indicating if the node is alive or else down/sleeping.
	// Note: 0/1 from HCAPI is reversed here
	hcapi_state = zwnet_node_sts_get(desc_node->net, desc_node->nodeid);

	if (hcapi_state == ZWNET_NODE_STS_UP)
	{
		node_state->alive_state = ZWNET_NODE_STATUS_ALIVE;
	}
	else if (hcapi_state == ZWNET_NODE_STS_SLEEP)
	{
		node_state->alive_state = ZWNET_NODE_STATUS_SLEEP;
	}
	else
	{
		node_state->alive_state = ZWNET_NODE_STATUS_DOWN;
	}
	//node_state->alive_state = (zwnet_node_sts_get(desc_node->net, desc_node->nodeid) == ZWNET_NODE_STS_DOWN) ? 0 : 1;

	if (node_state->alive_state != alive_state)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Node state: alive state: %s -> %s", zwp_hcapi_net_node_status(alive_state), zwp_hcapi_net_node_status(node_state->alive_state));
	}

	/*
	 * Cached states: Portion of node_state that is updated only during 'create'
	 */
	if (update_type == node_state_create)
	{
		// State containing extended version info.
		// As of now, zwnode_get_ext_ver() from HCAPI gives only cached info discovered during node creation stage.
		// Going forward, every zwnode_get_ext_ver() may trigger Version Get command to make this state 'live'. Until
		// then, the update of this state shall remain under this 'create only' portion of the code.
		if (node_state->ext_ver)
		{
			free(node_state->ext_ver);
		}
		node_state->ext_ver = zwnode_get_ext_ver(desc_node);

		// State indicating whether the node contains secure interfaces.
		node_state->secure = zwp_desc_node_get_secure_state(desc_node);
	}

	// Copy
	if (state_copy_p)
	{
		if (zwp_node_state_duplicate(node_state, state_copy_p) != ZWP_STATUS_OK)
		{
			goto l_err_node_state_duplicate;
		}
	}

	return ZWP_STATUS_OK;

	if (state_copy_p)
	{
		zwp_node_state_free(*state_copy_p);
	}
l_err_node_state_duplicate:
	/*
	 * Once zwp_node_state_set(desc_node, node_state) is called, there is no need to free 'node_state'
	 * in case of subsequent failure. The caller is expected to free 'node_state' using zwp_desc_node_destroy_state().
	 * Moreover, this memory may have been allocated during previous invocation of this function call.
	 */
l_err_target_restart_status_listeners_create:
l_err_node_state_alloc:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_node_state_refresh(zwnoded_p desc_node, zwp_node_state_t *state_copy_p)
{
	return zwp_node_state_update(desc_node, node_state_refresh, state_copy_p);
}

zwp_status_t zwp_node_state_create(zwnoded_p desc_node)
{
	return zwp_node_state_update(desc_node, node_state_create, NULL);
}

void zwp_node_state_destroy(zwnoded_p desc_node)
{
	zwp_node_state_t node_state;

	if ((node_state = zwp_node_state_get(desc_node)) != NULL)
	{
		zwu_array_list_destroy(node_state->firmware_update_target_restart_status_listeners);

		zwp_node_state_free(node_state);

		zwp_node_state_set(desc_node, NULL);
	}
}



