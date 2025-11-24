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
 * @file    zwp_avi_interfaces_switch_color.c
 *
 * @brief   AVI message 'interface_switch_color_xxx'
 * @details Handles the AVI message 'interface_switch_color_xxx'.
 *
 * @author  Jason Zheng
 *
 * @version 1.0 - 2016-09-21
 * - Initial version
 */

#include "zwp_avi_interfaces.h"

/** Switch Color Start Level not specified */
#define COMMAND_SWITCH_COLOR_START_LEVEL_NONE              (-1)
/** Switch Color Dimming Duration not specified */
#define COMMAND_SWITCH_COLOR_DURATION_NONE                 (-1)

#define MAX_NUM_OF_COLORS 		9

/** Set command state */
typedef struct {
	uint8_t            cnt;                                /**< Component Count */
	uint8_t            id[MAX_NUM_OF_COLORS];              /**< Component ID */
	uint8_t            val[MAX_NUM_OF_COLORS];             /**< Value */
	uint8_t            dur;                                /**< Duration */
} zwp_avi_interfaces_switch_color_set_t;

/** Get command state */
typedef struct {
	int                mask;                               /**< Color Component Mask */
} zwp_avi_interfaces_switch_color_get_t;

/** Level Change Set command state */
typedef struct {
	int                start_stop;                         /**< Start/Stop */
	int                up_down;                            /**< Up/Down */
	int                ignore_start_level;                 /**< Ignore start level? */
	int                id;                                 /**< Color Component ID */
	int                start_level;                        /**< Start level */
	int                duration;                           /**< Duration */
} zwp_avi_interfaces_switch_color_level_change_set_t;

/** Level Change Get command state */
typedef struct {
	int                mask;                                 /**< Color Component Mask */
} zwp_avi_interfaces_switch_color_level_change_get_t;

/** Color Switch value description */
typedef struct {
	zwu_time_t         utime;           					/**< Last updated time of Report command */
	zwcolor_t          value;                               /**< Color Switch value */
} zwp_avi_interfaces_switch_color_report_switch_color_value_t;

//todo : upper layer sends one request, then here zwportald reads back all colors. need to modify hcapi.
/** Report command state */
typedef struct {
	uint8_t            ver;                                /**< Color Switch CC version */	
	int                mask;                               /**< Color Component Mask */
	zwu_time_t         utime;                              /**< Last updated time of Report command */
	zwu_array_list_t   switch_color_value_list;     	   /**< List of color switch values of type @a zwp_avi_interfaces_switch_color_report_switch_color_value_t */	
	int                report_receive_state;               /**< Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Report command receive condition */
} zwp_avi_interfaces_switch_color_report_t;

/** Color Switch level change data */
typedef struct
{
    uint8_t     id;											/**< Component ID */
    uint8_t     start_stop;  								/**< 1=Start, 0=Stop */
}zwp_avi_interfaces_switch_color_level_change_t, *zwp_avi_interfaces_switch_color_level_change_p;

/** Color Switch level change description */
typedef struct {
	zwu_time_t         utime;           					/**< Last updated time of Level Change Report command */
	zwp_avi_interfaces_switch_color_level_change_t value; 	/**< Color Switch Level Change value */
} zwp_avi_interfaces_switch_color_report_switch_color_level_change_value_t;

/** Level Change Report command state */
typedef struct {
	zwu_time_t         utime;                              /**< Last updated time of Level Change Report command */
	zwu_array_list_t   switch_color_level_change_value_list;     /**< List of color switch values of type @a zwp_avi_interfaces_switch_color_report_switch_color_level_change_value_t */	
} zwp_avi_interfaces_switch_color_level_change_report_t;

/** Supported Report command state */
typedef struct {
	zwu_time_t         utime;                              /**< Last updated time of Supported Report command */
	int                mask;                               /**< Color Component Mask */
	int                report_receive_state;               /**< Supported Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Supported Report command receive condition */
} zwp_avi_interfaces_switch_color_supported_report_t;

/** Interface level state */
typedef struct {
	zwp_avi_interfaces_switch_color_report_t                   report;              /**< Report command state */
	zwp_avi_interfaces_switch_color_level_change_report_t      level_change_report; /**< Level Change Report command state */
	zwp_avi_interfaces_switch_color_supported_report_t         supported_report;    /**< Supported Report command state */
} zwp_avi_interfaces_switch_color_state_t;

/**
 * @brief Update interface level state with the new Report
 * @param[in]     report            Report state in interface level
 * @param[in]     data              Value from new Report
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */

static zwp_status_t zwp_avi_interfaces_switch_color_report_update(zwp_avi_interfaces_switch_color_report_t *report,
		zwcolor_p data, zwu_time_t ts)
{
	int switch_color_value_count;
	int i;

	zwp_avi_interfaces_switch_color_report_switch_color_value_t *switch_color_value;

	switch_color_value_count = zwu_array_list_size(report->switch_color_value_list);

	for (i = 0; i < switch_color_value_count; i++)
	{
		switch_color_value = zwu_array_list_get(report->switch_color_value_list, i);

		// If id matches, update the entry instead of creating new one.
		if (switch_color_value->value.id == data->id)
		{
			break;
		}
	}

	if (i >= switch_color_value_count)
	{
		if ((switch_color_value = (zwp_avi_interfaces_switch_color_report_switch_color_value_t *)ZWP_CALLOC(1, sizeof(*switch_color_value))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		if (zwu_array_list_add(report->switch_color_value_list, switch_color_value) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}
	}

	report->utime = ts;
	switch_color_value->utime = ts;
	memcpy(&switch_color_value->value, data, sizeof(switch_color_value->value));

	return ZWP_STATUS_OK;

l_err_value_list_add:
	ZWP_FREE(switch_color_value);
l_err_value_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief HCAPI callback for Report command
 * @param[in]     ifd               interface
 * @param[in]     data              color component data
 * @param[in]     ts                time stamp
 * @return        Nothing
 */
static void zwp_avi_interfaces_switch_color_report_handler(zwifd_p ifd, zwcolor_p data, time_t ts)
{
	zwp_avi_interfaces_switch_color_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_switch_color_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if(ts)
	{
		state->report.ver = desc_interface->ver;
		
		if (zwp_avi_interfaces_switch_color_report_update(&state->report, data, ts) != ZWP_STATUS_OK)
		{
			goto l_err_report_update;
		}		
	}

	zwp_interface_report_receive_done(&state->report.report_receive_state, state->report.report_receive_condition);
	zwp_net_release_desc_interface(ifd);
	return;
	
l_err_report_update:
l_err_get_state:
	zwp_net_release_desc_interface(ifd);
l_err_acquire_desc:
	return;
}

/**
 * @brief Setup interface
 * @param[in]     desc_interface    Interface descriptor
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_switch_color_setup(zwifd_p desc_interface, int *hcapi_code_p)
{
	*hcapi_code_p = hcaal_color_rpt_set(desc_interface, zwp_avi_interfaces_switch_color_report_handler);
	return ZWP_STATUS_OK;
}

/**
 * @brief Allocate interface level data
 * @param[out]    state_p           Interface level data
 * @param[in]     pool              Memory pool to be used for allocation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_switch_color_state_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_switch_color_state_alloc(void **state_p, zwp_pool_t pool)
{
	zwp_avi_interfaces_switch_color_state_t *state;

	if ((state = (zwp_avi_interfaces_switch_color_state_t *)ZWP_CALLOC(1, sizeof(*state))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_state_alloc;
	}

	if (zwp_thread_cond_create(&state->report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for report received event");
		goto l_err_net_report_received_cond_create;
	}

	if (zwu_array_list_create(0, &state->report.switch_color_value_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}	

	if (zwu_array_list_create(0, &state->level_change_report.switch_color_level_change_value_list) != ZWP_STATUS_OK)
	{
		goto l_err_level_change_list_create;
	}	

	if (zwp_thread_cond_create(&state->supported_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for supported report received event");
		goto l_err_net_supported_report_received_cond_create;
	}

	*state_p = state;

	return ZWP_STATUS_OK;

	zwp_thread_cond_destroy(state->supported_report.report_receive_condition);
l_err_net_supported_report_received_cond_create:
	zwu_array_list_destroy(state->level_change_report.switch_color_level_change_value_list);
l_err_level_change_list_create:
	zwu_array_list_destroy(state->report.switch_color_value_list);
l_err_value_list_create:
	zwp_thread_cond_destroy(state->report.report_receive_condition);
l_err_net_report_received_cond_create:
	ZWP_FREE(state);
l_err_state_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free interface level data
 * @param[in]     arg               Interface level data
 * @return        Nothing
 * @see zwp_avi_interfaces_switch_color_state_alloc
 */
static void zwp_avi_interfaces_switch_color_state_free(void *arg)
{
	zwp_avi_interfaces_switch_color_state_t *state;
	zwp_avi_interfaces_switch_color_report_t *report;
	int switch_color_value_count;
	zwp_avi_interfaces_switch_color_level_change_report_t *level_change_report;
	int switch_color_level_change_count;
	int i;

	state = (zwp_avi_interfaces_switch_color_state_t *)arg;
	report = &state->report;
	level_change_report = &state->level_change_report;

	zwp_interface_report_receive_done(&state->supported_report.report_receive_state, state->supported_report.report_receive_condition);

	zwp_thread_cond_destroy(state->supported_report.report_receive_condition);

	switch_color_level_change_count = zwu_array_list_size(level_change_report->switch_color_level_change_value_list);

	for (i = 0; i < switch_color_level_change_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(level_change_report->switch_color_level_change_value_list, i));
	}

	zwu_array_list_destroy(level_change_report->switch_color_level_change_value_list);
	
	switch_color_value_count = zwu_array_list_size(report->switch_color_value_list);

	for (i = 0; i < switch_color_value_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(report->switch_color_value_list, i));
	}

	zwu_array_list_destroy(report->switch_color_value_list);		
	
	zwp_interface_report_receive_done(&state->report.report_receive_state, state->report.report_receive_condition);

	zwp_thread_cond_destroy(state->report.report_receive_condition);

	ZWP_FREE(state);
}

/**
 * @brief Get argument for Get command from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Get command
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_switch_color_get_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_switch_color_get_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_switch_color_get_t *argument;
	int32_t mask;

	if ((argument = (zwp_avi_interfaces_switch_color_get_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "mask", &mask) != ZWP_STATUS_OK)
	{
		goto l_err_get_value;
	}
	
	ZWP_LOG(ZWP_LOG_DEBUG, "mask: %d", mask);

	if (mask == 0)
	{
		goto l_err_invalid_value;
	}
	
	argument->mask = mask;
	*args_p = argument;
	return ZWP_STATUS_OK;
	
l_err_get_value:
l_err_invalid_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Get command
 * @param[in]     args              Arguments passed with Get command
 * @return        Nothing
 * @see zwp_avi_interfaces_switch_color_get_argument
 */
static void zwp_avi_interfaces_switch_color_get_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
 * @brief Test if the expected Report command is received
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Get command
 * @param[out]    report_receive_condition_p    Report command receive condition
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_switch_color_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_switch_color_state_t *state;

	if ((state = (zwp_avi_interfaces_switch_color_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->report.report_receive_condition;

	if (zwp_interface_report_receive_assertion(state->report.report_receive_state) != ZWP_STATUS_OK)
	{
		goto l_err_report_not_received;
	}

	return ZWP_STATUS_OK;

l_err_report_not_received:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute active Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Get command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_switch_color_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_switch_color_state_t *state;
	zwp_avi_interfaces_switch_color_get_t *argument;
	argument = (zwp_avi_interfaces_switch_color_get_t *)args;
	
	if ((state = (zwp_avi_interfaces_switch_color_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		return ZWP_STATUS_ERROR;
	}	

	if (argument->mask == 0)
	{
		return ZWP_STATUS_ERROR;
	}
	
	zwp_interface_report_receive_init(&state->report.report_receive_state);	
	state->report.mask = argument->mask;

	int i;
	for (i = 0; i < sizeof(state->report.mask)*8; i++)
	{
		if (state->report.mask & (1<<i))
		{
			//todo: maybe hcapi will support multiple color get in the future.
			*hcapi_code_p = hcaal_color_get(desc_interface, i, ZWIF_GET_BMSK_LIVE | ZWIF_GET_BMSK_CACHE);
			break;
		}
	}
	return ZWP_STATUS_OK;
}

/**
 * @brief Allocate Result object corresponding to Report command
 * @param[out]    report_dest       Destination Report
 * @param[in]     report_src        Source Report
 * @param[in]     mask     			Color Mask
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_switch_color_get_result_alloc(zwp_avi_interfaces_switch_color_report_t *report_dest,
		zwp_avi_interfaces_switch_color_report_t *report_src, int mask)
{
	int switch_color_value_count;
	int i;
	zwp_avi_interfaces_switch_color_report_switch_color_value_t *switch_color_value_dest;
	zwp_avi_interfaces_switch_color_report_switch_color_value_t *switch_color_value_src;
	report_dest->ver = report_src->ver;
	
	switch_color_value_count = zwu_array_list_size(report_src->switch_color_value_list);

	for (i = 0; i < switch_color_value_count; i++)
	{
		switch_color_value_src = zwu_array_list_get(report_src->switch_color_value_list, i);

		// If id doesn't match, skip the entry
		if ((1<<(switch_color_value_src->value.id) & mask) == 0)
		{
			continue;
		}

		if ((switch_color_value_dest = (zwp_avi_interfaces_switch_color_report_switch_color_value_t *)ZWP_CALLOC(1, sizeof(*switch_color_value_dest))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		if (zwu_array_list_add(report_dest->switch_color_value_list, switch_color_value_dest) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}

		memcpy(switch_color_value_dest, switch_color_value_src, sizeof(*switch_color_value_dest));
	}

	return ZWP_STATUS_OK;

l_err_value_list_add:
	ZWP_FREE(switch_color_value_dest);
l_err_value_alloc:
	for (i--; i >= 0; i--)
	{
		ZWP_FREE(zwu_array_list_get(report_dest->switch_color_value_list, i));
	}
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute passive Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Get command
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_switch_color_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_switch_color_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_switch_color_state_t *state;
	zwp_avi_interfaces_switch_color_report_t *report;
	zwp_avi_interfaces_switch_color_get_t *argument;
	argument = (zwp_avi_interfaces_switch_color_get_t *)args;
	
	if ((state = (zwp_avi_interfaces_switch_color_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (argument->mask == 0)
	{
		goto l_err_invalid_arg;
	}
	
	if ((report = (zwp_avi_interfaces_switch_color_report_t *)ZWP_CALLOC(1, sizeof(*report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}
	
	state->report.mask = argument->mask;
	
	if (zwu_array_list_create(0, &report->switch_color_value_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	if (zwp_avi_interfaces_switch_color_get_result_alloc(report, &state->report, argument->mask) != ZWP_STATUS_OK)
	{
		goto l_err_result_alloc;
	}	
	
	*result_p = report;
	return ZWP_STATUS_OK;

l_err_result_alloc:
	zwu_array_list_destroy(report->switch_color_value_list);
l_err_value_list_create:
	ZWP_FREE(report);
l_err_report_alloc:
l_err_invalid_arg:
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
static zwp_status_t zwp_avi_interfaces_switch_color_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_switch_color_report_t *report;
	avro_value_t avro_list;
	avro_value_t avro_value;
	zwp_avi_interfaces_switch_color_report_switch_color_value_t *switch_color_value;
	int avro_value_count;

	report = (zwp_avi_interfaces_switch_color_report_t *)result;
	
	if (avro_value_get_by_name(arg_value, "switch_color_value_list", &avro_list, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'switch_color_value_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}
	
	avro_value_count = zwu_array_list_size(report->switch_color_value_list);
	
	int i;
	for (i = 0; i < avro_value_count; i++) 
	{

		if (avro_value_append(&avro_list, &avro_value, 0) != 0)
		{
			goto l_err_append_switch_color_value_value;
		}

		switch_color_value = zwu_array_list_get(report->switch_color_value_list, i);

		int tar_val;
		int dur;
		if (report->ver < 3)
		{
			tar_val = (-1);
			dur = (-1);
		}
		else
		{
			tar_val = switch_color_value->value.target_val;
			dur = switch_color_value->value.dur;
		}		
		if ((zw_avro_field_set_long(&avro_value, "utime", switch_color_value->utime) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&avro_value, "id", switch_color_value->value.id) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&avro_value, "cvalue", switch_color_value->value.value) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&avro_value, "tvalue", tar_val) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&avro_value, "dur", dur) != ZWP_STATUS_OK)
				)
		{
			goto l_err_set_report_value;
		}

	}	
	return ZWP_STATUS_OK;

l_err_get_list_value:
l_err_append_switch_color_value_value:
l_err_set_report_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_switch_color_get_execute_passive
 */
static void zwp_avi_interfaces_switch_color_get_result_free(void *result)
{
	zwp_avi_interfaces_switch_color_report_t *report;
	int value_count;
	int i;

	report = (zwp_avi_interfaces_switch_color_report_t *)result;

	value_count = zwu_array_list_size(report->switch_color_value_list);

	for (i = 0; i < value_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(report->switch_color_value_list, i));
	}
	zwu_array_list_destroy(report->switch_color_value_list);

	ZWP_FREE(report);	
}

/**
 * @brief Get argument for Set command from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Set command
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_switch_color_set_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_switch_color_set_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_switch_color_set_t *argument;

	if ((argument = (zwp_avi_interfaces_switch_color_set_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}	

	avro_value_t avro_list;
	avro_value_t avro_value;
	size_t switch_color_value_count;
	int i;
	int32_t id[MAX_NUM_OF_COLORS]; 
	int32_t value[MAX_NUM_OF_COLORS]; 
	int32_t dur;

	if (avro_value_get_by_name(arg_value, "switch_color_set_list", &avro_list, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'switch_color_set_list': %s", avro_strerror());
		goto l_free_argument;
	}

	if (avro_value_get_size(&avro_list, &switch_color_value_count) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get switch_color value list size");
		goto l_free_argument;
	}

	if (switch_color_value_count > MAX_NUM_OF_COLORS)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid switch_color value list size");
		goto l_free_argument;
	}	
	
	for (i = 0; i < switch_color_value_count; i++)
	{
		if (avro_value_get_by_index(&avro_list, i, &avro_value, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "avro_value_get_by_index() fail");
			goto l_free_argument;
		}
		
		if ((zw_avro_field_get_int(&avro_value, "id", &id[i]) != ZWP_STATUS_OK)
				|| (zw_avro_field_get_int(&avro_value, "value", &value[i]) != ZWP_STATUS_OK)
				)
		{
			ZWP_LOG(ZWP_LOG_ERR, "zw_avro_field_get_int() fail");
			goto l_free_argument;
		}
		
		if (zwu_number_is_uint8(id[i]) != ZWU_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Invalid Color Component ID: %d", id[i]);
			goto l_free_argument;
		}
		
		if (zwu_number_is_uint8(value[i]) != ZWU_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Invalid Color Component Value: %d", value[i]);
			goto l_free_argument;
		}
		
		argument->id[i] = (uint8_t)id[i];
		argument->val[i] = (uint8_t)value[i];
	}

	if (zw_avro_field_get_int(arg_value, "dur", &dur) != ZWP_STATUS_OK)
	{
		goto l_free_argument;
	}
	
	if ((dur != COMMAND_SWITCH_COLOR_DURATION_NONE)
			&& (zwu_number_is_uint8(dur) != ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Color Switch Set Duration: %d", dur);
		goto l_free_argument;
	}

	argument->cnt = (uint8_t)switch_color_value_count;
	argument->dur = (uint8_t)dur;

	*args_p = argument;
	return ZWP_STATUS_OK;

l_free_argument:
	ZWP_FREE(argument);

l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Set command
 * @param[in]     args              Arguments passed with Set command
 * @return        Nothing
 * @see zwp_avi_interfaces_switch_color_set_argument
 */
static void zwp_avi_interfaces_switch_color_set_argument_free(void *args)
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
static zwp_status_t zwp_avi_interfaces_switch_color_set_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_switch_color_state_t *state;
		
	if ((state = (zwp_avi_interfaces_switch_color_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		return ZWP_STATUS_ERROR;
	}
	
	zwp_avi_interfaces_switch_color_set_t *argument;
	argument = (zwp_avi_interfaces_switch_color_set_t *)args;

	if (argument->dur == COMMAND_SWITCH_COLOR_DURATION_NONE)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Color Switch Duration not provided. Defaulting to 0xFF (Factory Default)");

		// If not given, use factory default 0xFF
		argument->dur = 0xFF;
	}
	
	*hcapi_code_p = zwif_color_sw_set(desc_interface, argument->cnt, &argument->id[0], &argument->val[0], argument->dur);
	
	return ZWP_STATUS_OK;
}


/**
 * @brief Update interface level change state with the new Report
 * @param[in]     report            Report state in interface level
 * @param[in]     data              Value from new Report
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_switch_color_level_change_report_update(zwp_avi_interfaces_switch_color_level_change_report_t *report,
		zwp_avi_interfaces_switch_color_level_change_p data, zwu_time_t ts)
{
	int switch_color_level_change_value_count;
	int i;

	zwp_avi_interfaces_switch_color_report_switch_color_level_change_value_t *switch_color_level_change_value;

	switch_color_level_change_value_count = zwu_array_list_size(report->switch_color_level_change_value_list);

	for (i = 0; i < switch_color_level_change_value_count; i++)
	{
		switch_color_level_change_value = zwu_array_list_get(report->switch_color_level_change_value_list, i);

		// If id matches, update the entry instead of creating new one.
		if (switch_color_level_change_value->value.id == data->id)
		{
			break;
		}
	}

	if (i >= switch_color_level_change_value_count)
	{
		if ((switch_color_level_change_value = (zwp_avi_interfaces_switch_color_report_switch_color_level_change_value_t *)ZWP_CALLOC(1, sizeof(*switch_color_level_change_value))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		if (zwu_array_list_add(report->switch_color_level_change_value_list, switch_color_level_change_value) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}
	}

	report->utime = ts;
	switch_color_level_change_value->utime = ts;
	memcpy(&switch_color_level_change_value->value, data, sizeof(switch_color_level_change_value->value));
	return ZWP_STATUS_OK;

l_err_value_list_add:
	ZWP_FREE(switch_color_level_change_value);
l_err_value_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Allocate Result object corresponding to Level Change Report command
 * @param[out]    report_dest       Destination Report
 * @param[in]     report_src        Source Report
 * @param[in]     mask     			Color Component Mask
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_switch_color_level_change_get_result_alloc(zwp_avi_interfaces_switch_color_level_change_report_t *report_dest,
		zwp_avi_interfaces_switch_color_level_change_report_t *report_src, int mask)
{
	int switch_color_level_change_value_count;
	int i;
	zwp_avi_interfaces_switch_color_report_switch_color_level_change_value_t *switch_color_level_change_value_dest;
	zwp_avi_interfaces_switch_color_report_switch_color_level_change_value_t *switch_color_level_change_value_src;

	switch_color_level_change_value_count = zwu_array_list_size(report_src->switch_color_level_change_value_list);
	
	for (i = 0; i < switch_color_level_change_value_count; i++)
	{
		switch_color_level_change_value_src = zwu_array_list_get(report_src->switch_color_level_change_value_list, i);

		// If id doesn't match, skip the entry
		if (!(mask & (1<<switch_color_level_change_value_src->value.id)))
		{
			continue;
		}

		if ((switch_color_level_change_value_dest = (zwp_avi_interfaces_switch_color_report_switch_color_level_change_value_t *)ZWP_CALLOC(1, sizeof(*switch_color_level_change_value_dest))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		if (zwu_array_list_add(report_dest->switch_color_level_change_value_list, switch_color_level_change_value_dest) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}

		memcpy(switch_color_level_change_value_dest, switch_color_level_change_value_src, sizeof(*switch_color_level_change_value_dest));
	}

	return ZWP_STATUS_OK;

l_err_value_list_add:
	ZWP_FREE(switch_color_level_change_value_dest);
l_err_value_alloc:
	for (i--; i >= 0; i--)
	{
		ZWP_FREE(zwu_array_list_get(report_dest->switch_color_level_change_value_list, i));
	}
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Get argument for Level Change Get command from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Get command
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_switch_color_level_change_get_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_switch_color_level_change_get_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_switch_color_level_change_get_t *argument;

	if ((argument = (zwp_avi_interfaces_switch_color_level_change_get_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		return ZWP_STATUS_ERROR;
	}

	if (zw_avro_field_get_int(arg_value, "mask", &argument->mask) != ZWP_STATUS_OK)		
	{
		ZWP_FREE(argument);
		return ZWP_STATUS_ERROR;
	}
	
	ZWP_LOG(ZWP_LOG_DEBUG, "Color Component MASK: %d", argument->mask);	
	*args_p = argument;
	return ZWP_STATUS_OK;
}

/**
 * @brief Free arguments for Level Change Set command
 * @param[in]     args              Arguments passed with Set command
 * @return        Nothing
 * @see zwp_avi_interfaces_switch_color_level_change_get_argument
 */
static void zwp_avi_interfaces_switch_color_level_change_get_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
 * @brief Execute passive Level Change Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Level Change Get command
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_switch_color_level_change_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_switch_color_level_change_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_switch_color_state_t *state;
	zwp_avi_interfaces_switch_color_level_change_report_t *level_change_report;

	if ((state = (zwp_avi_interfaces_switch_color_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}
	
	zwp_avi_interfaces_switch_color_level_change_get_t *argument;
	argument = (zwp_avi_interfaces_switch_color_level_change_get_t *)args;	

	if ((level_change_report = (zwp_avi_interfaces_switch_color_level_change_report_t *)ZWP_CALLOC(1, sizeof(zwp_avi_interfaces_switch_color_level_change_report_t))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	if (zwu_array_list_create(0, &level_change_report->switch_color_level_change_value_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	if (zwp_avi_interfaces_switch_color_level_change_get_result_alloc(level_change_report, &state->level_change_report, argument->mask) != ZWP_STATUS_OK)
	{
		goto l_err_result_alloc;
	}	
	
	*result_p = level_change_report;
	return ZWP_STATUS_OK;

l_err_result_alloc:
	zwu_array_list_destroy(level_change_report->switch_color_level_change_value_list);
l_err_value_list_create:
	ZWP_FREE(level_change_report);
l_err_report_alloc:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to Level Change Report command
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_switch_color_level_change_get_result(void *result, avro_value_t *arg_value)
{	
	zwp_avi_interfaces_switch_color_level_change_report_t *report;
	zwp_avi_interfaces_switch_color_report_switch_color_level_change_value_t *value;
	int avro_value_count;	
	avro_value_t avro_list;
	avro_value_t avro_value;	
	
	report = (zwp_avi_interfaces_switch_color_level_change_report_t *)result;
	
	if (avro_value_get_by_name(arg_value, "switch_color_level_change_value_list", &avro_list, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'switch_color_level_change_value_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}
	
	avro_value_count = zwu_array_list_size(report->switch_color_level_change_value_list);
	
	int i;
	for (i = 0; i < avro_value_count; i++) 
	{

		if (avro_value_append(&avro_list, &avro_value, 0) != 0)
		{
			goto l_err_append_switch_color_value_value;
		}

		value = zwu_array_list_get(report->switch_color_level_change_value_list, i);
	
		if ((zw_avro_field_set_long(&avro_value, "utime", value->utime) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&avro_value, "id", value->value.id) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&avro_value, "start_stop", value->value.start_stop) != ZWP_STATUS_OK)
				)
		{
			goto l_err_set_report_value;
		}

	}	
	return ZWP_STATUS_OK;

l_err_get_list_value:
l_err_append_switch_color_value_value:
l_err_set_report_value:
	return ZWP_STATUS_ERROR;	
}

/**
 * @brief Free Result object corresponding to Level Change Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_switch_color_level_change_get_execute_passive
 */
static void zwp_avi_interfaces_switch_color_level_change_get_result_free(void *result)
{
	zwp_avi_interfaces_switch_color_level_change_report_t *report;
	int value_count;
	int i;

	report = (zwp_avi_interfaces_switch_color_level_change_report_t *)result;

	value_count = zwu_array_list_size(report->switch_color_level_change_value_list);

	for (i = 0; i < value_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(report->switch_color_level_change_value_list, i));
	}
	zwu_array_list_destroy(report->switch_color_level_change_value_list);

	ZWP_FREE(report);	
}

/**
 * @brief Get argument for Level Change Set command from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Set command
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_switch_color_level_change_set_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_switch_color_level_change_set_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_switch_color_level_change_set_t *argument;

	if ((argument = (zwp_avi_interfaces_switch_color_level_change_set_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if ((zw_avro_field_get_int(arg_value, "start_stop", &argument->start_stop) != ZWP_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "up_down", &argument->up_down) != ZWP_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "ignore_start_level", &argument->ignore_start_level) != ZWP_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "id", &argument->id) != ZWP_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "start_level", &argument->start_level) != ZWP_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "duration", &argument->duration) != ZWP_STATUS_OK))
	{
		goto l_err_get_value;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Start/Stop: %d", argument->start_stop);
	ZWP_LOG(ZWP_LOG_DEBUG, "Up/Down: %d", argument->up_down);
	ZWP_LOG(ZWP_LOG_DEBUG, "Ignore Start Level: %d", argument->ignore_start_level);
	ZWP_LOG(ZWP_LOG_DEBUG, "Component ID: %d", argument->id);
	ZWP_LOG(ZWP_LOG_DEBUG, "Start Level: %d", argument->start_level);
	ZWP_LOG(ZWP_LOG_DEBUG, "Duration: %d", argument->duration);

	if (zwu_number_is_uint8(argument->start_stop) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Start/Stop value: %d", argument->start_stop);
		goto l_err_invalid_start_stop;
	}

	if (zwu_number_is_uint8(argument->up_down) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Up/Down value: %d", argument->up_down);
		goto l_err_invalid_up_down;
	}

	if (zwu_number_is_uint8(argument->ignore_start_level) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Ignore Start Level value: %d", argument->ignore_start_level);
		goto l_err_invalid_ignore_start_level;
	}
	
	if (zwu_number_is_uint8(argument->id) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Component ID value: %d", argument->id);
		goto l_err_invalid_component_id;
	}
	
	if ((argument->start_level != COMMAND_SWITCH_COLOR_START_LEVEL_NONE)
			&& (zwu_number_is_uint8(argument->start_level) != ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Start Level value: %d", argument->start_level);
		goto l_err_invalid_start_level;
	}

	if ((argument->duration != COMMAND_SWITCH_COLOR_DURATION_NONE)
			&& (zwu_number_is_uint8(argument->duration) != ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid Duration value: %d", argument->duration);
		goto l_err_invalid_duration;
	}
	
	*args_p = argument;
	return ZWP_STATUS_OK;

l_err_invalid_duration:
l_err_invalid_start_level:
l_err_invalid_component_id:
l_err_invalid_ignore_start_level:
l_err_invalid_up_down:
l_err_invalid_start_stop:
l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Level Change Set command
 * @param[in]     args              Arguments passed with Set command
 * @return        Nothing
 * @see zwp_avi_interfaces_switch_color_level_change_set_argument
 */
static void zwp_avi_interfaces_switch_color_level_change_set_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
 * @brief Execute Level Change Set command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments passed with Level Change Set command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_switch_color_level_change_set_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_switch_color_state_t *state;
	zwp_avi_interfaces_switch_color_level_change_set_t *argument;
	zwcol_ctl_t level_change;

	argument = (zwp_avi_interfaces_switch_color_level_change_set_t *)args;
	
	if ((state = (zwp_avi_interfaces_switch_color_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		return ZWP_STATUS_ERROR;
	}

	if (argument->start_stop != 0)
	{
		if ((argument->ignore_start_level == 0)
				&& (argument->start_level == COMMAND_SWITCH_COLOR_START_LEVEL_NONE))
		{
			ZWP_LOG(ZWP_LOG_ERR, "Missing required argument Start Level");
			return ZWP_STATUS_ERROR;
		}

		if (argument->duration == COMMAND_SWITCH_COLOR_DURATION_NONE)
		{
			ZWP_LOG(ZWP_LOG_DEBUG, "Duration not provided. Defaulting to 0xFF (Factory Default)");

			// If not given, use factory default 0xFF
			argument->duration = 0xFF;
		}
	
		level_change.dir = (uint8_t)argument->up_down;
		if ((uint8_t)argument->ignore_start_level)
		{
			level_change.use_start_lvl = 0;
		}
		else
		{
			level_change.use_start_lvl = 1;
		}
		
		if ((argument->ignore_start_level == 1)
				&& (argument->start_level == COMMAND_SWITCH_COLOR_START_LEVEL_NONE))
			level_change.start_level = 0;
		else
			level_change.start_level = (uint8_t)argument->start_level;		
		
		level_change.dur = (uint8_t)argument->duration;
		level_change.id = (uint8_t)argument->id;
		
		*hcapi_code_p = zwif_color_sw_start(desc_interface, &level_change);
	}
	else
	{		
		*hcapi_code_p = zwif_color_sw_stop(desc_interface, (uint8_t)argument->id);
	}
	
	if (*hcapi_code_p == ZW_ERR_NONE)
	{
		zwp_avi_interfaces_switch_color_level_change_t data;
		data.id = (uint8_t)argument->id;
		data.start_stop = (uint8_t)argument->start_stop;
		
		if (zwp_avi_interfaces_switch_color_level_change_report_update(&state->level_change_report, &data, zwu_time_now()) != ZWP_STATUS_OK)
		{
			return ZWP_STATUS_ERROR;
		}	
	}
	
	return ZWP_STATUS_OK;
}

/**
 * @brief HCAPI callback for Supported Report command
 * @param[in]     ifd               interface
 * @param[in]     comp_cnt    		number of color components in comp_id array
 * @param[in]     comp_id     		color component id (see COL_SW_COMP_ID_XXX )array
 * @return        Nothing
 */
static void zwp_avi_interfaces_switch_color_supported_report_handler(zwifd_p ifd, uint8_t comp_cnt, uint8_t *comp_id, int valid)
{
	zwp_avi_interfaces_switch_color_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_switch_color_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (valid)
	{
		state->supported_report.utime = zwu_time_now();
		int mask;
		mask = 0;
		int i;
		if (comp_id && comp_cnt)
		{
			for (i = 0; i < comp_cnt; i++)
			{
				mask |= (1<<comp_id[i]);
			}
		}
		
		state->supported_report.mask = mask;
	}
	
	zwp_interface_report_receive_done(&state->supported_report.report_receive_state, state->supported_report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

l_err_get_state:
	zwp_net_release_desc_interface(ifd);
l_err_acquire_desc:

	return;
}

/**
 * @brief Test if the expected Supported Report command is received
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Supprted Get command
 * @param[out]    report_receive_condition_p    Report command receive condition
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_switch_color_supported_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_switch_color_state_t *state;

	if ((state = (zwp_avi_interfaces_switch_color_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->supported_report.report_receive_condition;

	if (zwp_interface_report_receive_assertion(state->supported_report.report_receive_state) != ZWP_STATUS_OK)
	{
		goto l_err_report_not_received;
	}
	
	return ZWP_STATUS_OK;

l_err_report_not_received:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute active Supported Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Supported Get command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_switch_color_supported_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_switch_color_state_t *state;

	if ((state = (zwp_avi_interfaces_switch_color_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->supported_report.report_receive_state);

	*hcapi_code_p = zwif_color_sw_sup_get(desc_interface, zwp_avi_interfaces_switch_color_supported_report_handler, 0);

	return ZWP_STATUS_OK;

l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute passive Supported Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Supported Get command
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_switch_color_supported_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_switch_color_supported_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_switch_color_state_t *state;
	zwp_avi_interfaces_switch_color_supported_report_t *supported_report;

	if ((state = (zwp_avi_interfaces_switch_color_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((supported_report = (zwp_avi_interfaces_switch_color_supported_report_t *)ZWP_CALLOC(1, sizeof(*supported_report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	memcpy(supported_report, &state->supported_report, sizeof(*supported_report));	
	
	*result_p = supported_report;
	return ZWP_STATUS_OK;

l_err_report_alloc:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to Supported Report command
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_switch_color_supported_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_switch_color_supported_report_t *supported_report;
	supported_report = (zwp_avi_interfaces_switch_color_supported_report_t *)result;
	
	if ((zw_avro_field_set_long(arg_value, "utime", supported_report->utime) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "mask", supported_report->mask) != ZWP_STATUS_OK)
			)
	{
		goto l_err_set_report_value;
	}

	return ZWP_STATUS_OK;

l_err_set_report_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Supported Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_switch_color_supported_get_execute_passive
 */
static void zwp_avi_interfaces_switch_color_supported_get_result_free(void *result)
{
	ZWP_FREE(result);
}

/** Handlers for Get command */
static zwp_avi_message_interfaces_t interfaces_switch_color_get =
{
		"interface_switch_color_get",
		COMMAND_CLASS_SWITCH_COLOR, zwp_avi_interfaces_switch_color_setup,
		zwp_avi_interfaces_switch_color_state_alloc, zwp_avi_interfaces_switch_color_state_free,
		zwp_avi_interfaces_switch_color_get_argument, zwp_avi_interfaces_switch_color_get_argument_free,
		zwp_avi_interfaces_switch_color_get_report_receive_assertion,
		zwp_avi_interfaces_switch_color_get_execute_active, zwp_avi_interfaces_switch_color_get_execute_passive,
		zwp_avi_interfaces_switch_color_get_result, zwp_avi_interfaces_switch_color_get_result_free
};

/** Handlers for Set command */
static zwp_avi_message_interfaces_t interfaces_switch_color_set =
{
		"interface_switch_color_set",
		COMMAND_CLASS_SWITCH_COLOR, NULL,
		NULL, NULL,
		zwp_avi_interfaces_switch_color_set_argument, zwp_avi_interfaces_switch_color_set_argument_free,
		NULL,
		zwp_avi_interfaces_switch_color_set_execute_active, NULL,
		NULL, NULL
};

/** Handlers for Level Change Get command */
static zwp_avi_message_interfaces_t interfaces_switch_color_level_change_get =
{
		"interface_switch_color_level_change_get",
		COMMAND_CLASS_SWITCH_COLOR, NULL,
		NULL, NULL,
		zwp_avi_interfaces_switch_color_level_change_get_argument, zwp_avi_interfaces_switch_color_level_change_get_argument_free,
		NULL,
		NULL, zwp_avi_interfaces_switch_color_level_change_get_execute_passive,
		zwp_avi_interfaces_switch_color_level_change_get_result, zwp_avi_interfaces_switch_color_level_change_get_result_free
};

/** Handlers for Level Change Set command */
static zwp_avi_message_interfaces_t interfaces_switch_color_level_change_set =
{
		"interface_switch_color_level_change_set",
		COMMAND_CLASS_SWITCH_COLOR, NULL,
		NULL, NULL,
		zwp_avi_interfaces_switch_color_level_change_set_argument, zwp_avi_interfaces_switch_color_level_change_set_argument_free, 
		NULL,
		zwp_avi_interfaces_switch_color_level_change_set_execute_active, NULL,
		NULL, NULL
};

/** Handlers for Supported Get command */
static zwp_avi_message_interfaces_t interfaces_switch_color_supported_get =
{
		"interface_switch_color_supported_get",
		COMMAND_CLASS_SWITCH_COLOR, NULL,
		NULL, NULL,
		NULL, NULL,
		zwp_avi_interfaces_switch_color_supported_get_report_receive_assertion,
		zwp_avi_interfaces_switch_color_supported_get_execute_active, zwp_avi_interfaces_switch_color_supported_get_execute_passive,
		zwp_avi_interfaces_switch_color_supported_get_result, zwp_avi_interfaces_switch_color_supported_get_result_free
};

/** Register command handlers */
zwp_status_t zwp_avi_interfaces_switch_color_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_switch_color_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_switch_color_set) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_switch_color_level_change_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_switch_color_level_change_set) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_switch_color_supported_get) != ZWP_STATUS_OK))
	{
		goto l_err_interface_list_get;
	}

	return ZWP_STATUS_OK;

l_err_interface_list_get:
	return ZWP_STATUS_ERROR;
}