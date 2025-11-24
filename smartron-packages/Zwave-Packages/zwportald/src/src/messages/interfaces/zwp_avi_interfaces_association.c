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
 * @file    zwp_avi_interfaces_association.c
 *
 * @brief   AVI message 'interface_association_xxx'
 * @details Handles the AVI message 'interface_association_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-02-14
 * - Initial version
 */

#include "zwp_avi_interfaces.h"

/** Group identifier not specified */
#define COMMAND_ASSOCIATION_GROUPINGS_IDENTIFIER_NONE         (-1)

/** Association Set action : Add */
#define ZWP_ASSOCIATION_SET_ACTION_ADD         0
/** Association Set action : Remove */
#define ZWP_ASSOCIATION_SET_ACTION_REMOVE      1

/** Get command state */
typedef struct {
	int                grouping_identifier;                    /**< Grouping identidier */
} zwp_avi_interfaces_association_get_t;


/** Set command state */
typedef struct {
	int                action;                                 /**< Action: add or remove */
	int                grouping_identifier;                    /**< Grouping identifier */
	grp_member_t   	   *group_members;    					   /**< List of group members */
	size_t             group_members_count;                    /**< Count of group members in the list */
} zwp_avi_interfaces_association_set_t;

/** Association description */
typedef struct {
	zwu_time_t         utime;                                  /**< Last update time for specific grouping_identifier */
	int                grouping_identifier;                    /**< Grouping identifier */
	int                max_nodes_supported;                    /**< Maximum number of nodes supported */
	grp_member_t   	   *group_members;    					   /**< List of group members */
	size_t             group_members_count;                    /**< Count of group members in the list */
} zwp_avi_interfaces_association_report_association_value_t;

/** Report command state */
typedef struct {
	zwu_time_t         utime;                                  /**< Last update time across all grouping_identifiers */
	zwu_array_list_t   association_value_list;                 /**< List of associations of type @a zwp_avi_interfaces_association_report_association_value_t */

	int                report_receive_state;                   /**< Report command receive state */
	zwp_thread_cond_t  report_receive_condition;               /**< Report command receive condition */
	int                report_receive_grouping_identifier;     /**< Current grouping identifier for Report command receive condition */
} zwp_avi_interfaces_association_report_t;

/** Supported Report command state */
typedef struct {
	zwu_time_t         utime;                                  /**< Last updated time of Supported Report command */
	uint8_t            supported_groupings;                    /**< Maximum number of supported groups */

	int                report_receive_state;                   /**< Supported Report command receive state */
	zwp_thread_cond_t  report_receive_condition;               /**< Supported Report command receive condition */
} zwp_avi_interfaces_association_groupings_report_t;

/** Specific Group Report command state */
typedef struct {
	zwu_time_t         utime;                                  /**< Last updated time of Specific Group Report command */
	uint8_t            group;                                  /**< Specific group identifier */

	int                report_receive_state;                   /**< Specific Group Report command receive state */
	zwp_thread_cond_t  report_receive_condition;               /**< Specific Group Report command receive condition */
} zwp_avi_interfaces_association_specific_group_report_t;

/** Interface level state */
typedef struct {
	zwp_avi_interfaces_association_report_t                report;                  /**< Report command state */
	zwp_avi_interfaces_association_groupings_report_t      groupings_report;        /**< Supported Report command state */
	zwp_avi_interfaces_association_specific_group_report_t specific_group_report;   /**< Specific Group Report command state */
} zwp_avi_interfaces_association_state_t;

/**
 * @brief Allocate interface level data
 * @param[out]    state_p           Interface level data
 * @param[in]     pool              Memory pool to be used for allocation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_association_state_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_association_state_alloc(void **state_p, zwp_pool_t pool)
{
	zwp_avi_interfaces_association_state_t *state;

	if ((state = (zwp_avi_interfaces_association_state_t *)ZWP_CALLOC(1, sizeof(*state))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_state_alloc;
	}

	if (zwp_thread_cond_create(&state->report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for report received event");
		goto l_err_net_report_received_cond_create;
	}

	state->report.report_receive_grouping_identifier = COMMAND_ASSOCIATION_GROUPINGS_IDENTIFIER_NONE;

	if (zwu_array_list_create(0, &state->report.association_value_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	if (zwp_thread_cond_create(&state->groupings_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for groupings_report received event");
		goto l_err_net_groupings_report_received_cond_create;
	}

	state->groupings_report.supported_groupings = 0;

	if (zwp_thread_cond_create(&state->specific_group_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for groupings_report received event");
		goto l_err_net_specific_group_report_received_cond_create;
	}

	state->specific_group_report.group = 0;

	*state_p = state;

	return ZWP_STATUS_OK;

	zwp_thread_cond_destroy(state->specific_group_report.report_receive_condition);
l_err_net_specific_group_report_received_cond_create:
	zwp_thread_cond_destroy(state->groupings_report.report_receive_condition);
l_err_net_groupings_report_received_cond_create:
	zwu_array_list_destroy(state->report.association_value_list);
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
 * @see zwp_avi_interfaces_association_state_alloc
 */
static void zwp_avi_interfaces_association_state_free(void *arg)
{
	zwp_avi_interfaces_association_state_t *state;
	zwp_avi_interfaces_association_report_t *report;
	int association_value_count;
	int i;
	zwp_avi_interfaces_association_report_association_value_t *association_value;

	state = (zwp_avi_interfaces_association_state_t *)arg;
	report = &state->report;

	zwp_interface_report_receive_done(&state->specific_group_report.report_receive_state, state->specific_group_report.report_receive_condition);

	zwp_thread_cond_destroy(state->specific_group_report.report_receive_condition);

	zwp_interface_report_receive_done(&state->groupings_report.report_receive_state, state->groupings_report.report_receive_condition);

	zwp_thread_cond_destroy(state->groupings_report.report_receive_condition);

	association_value_count = zwu_array_list_size(report->association_value_list);

	for (i = 0; i < association_value_count; i++)
	{
		association_value = zwu_array_list_get(report->association_value_list, i);

		ZWP_FREE(association_value->group_members);

		ZWP_FREE(association_value);
	}

	zwu_array_list_destroy(report->association_value_list);

	zwp_interface_report_receive_done(&state->report.report_receive_state, state->report.report_receive_condition);

	zwp_thread_cond_destroy(state->report.report_receive_condition);

	ZWP_FREE(state);
}

/**
 * @brief Update interface level state with the new Report
 * @param[in]     report            Report state in interface level
 * @param[in]	  group	            Grouping identifier
 * @param[in]	  max_cnt	        Maximum number of end points the grouping identifier above supports
 * @param[in]	  cnt	            The number of members in the grouping in this report
 * @param[in]	  grp_member	    An array of members in the grouping. Note that the group members may contain
                        			non-existence node/endpoint
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_association_report_update(zwp_avi_interfaces_association_report_t *report,
		uint8_t group, uint8_t max_cnt, uint8_t cnt, grp_member_t *grp_member)
{
	int association_value_count;
	int i;
	zwu_time_t  time_now;
	zwp_avi_interfaces_association_report_association_value_t *association_value;
	grp_member_t *tmp;

	association_value_count = zwu_array_list_size(report->association_value_list);

	for (i = 0; i < association_value_count; i++)
	{
		association_value = zwu_array_list_get(report->association_value_list, i);

		// If grouping_identifier matches, update the entry instead of creating new one.
		if (association_value->grouping_identifier == group)
		{
			break;
		}
	}

	if (i >= association_value_count)
	{
		// Create new association_value
		if ((association_value = (zwp_avi_interfaces_association_report_association_value_t *)ZWP_CALLOC(1, sizeof(*association_value))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		association_value->group_members = NULL;
		association_value->group_members_count = 0;

		if (zwu_array_list_add(report->association_value_list, association_value) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}
	}

	time_now = zwu_time_now();

	report->utime = time_now;
	association_value->utime = time_now;

	association_value->grouping_identifier = group;
	association_value->max_nodes_supported = max_cnt;

	if (cnt > association_value->group_members_count)
	{
		if ((tmp = (grp_member_t *)ZWP_REALLOC(association_value->group_members, sizeof(*tmp) * cnt)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_endpoints_list_alloc;
		}

		association_value->group_members = tmp;
	}
	association_value->group_members_count = cnt;

	memcpy(association_value->group_members, grp_member,
			sizeof(*association_value->group_members) * association_value->group_members_count);

	return ZWP_STATUS_OK;

l_err_endpoints_list_alloc:
	// Remove from list only if a new association_value
	if (i >= association_value_count)
	{
		zwu_array_list_remove(report->association_value_list, association_value);
	}
l_err_value_list_add:
	// Free only if a new association_value
	if (i >= association_value_count)
	{
		ZWP_FREE(association_value);
	}
l_err_value_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief HCAPI callback for Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  group	            Grouping identifier
 * @param[in]	  max_cnt	        Maximum number of end points the grouping identifier above supports
 * @param[in]	  cnt	            The number of end points in the grouping in this report
 * @param[in]	  grp_member	    An array of members in the grouping. Note that the group members may contain
                        			non-existence node/endpoint
 * @return        Nothing
 */
static void zwp_avi_interfaces_association_report_handler(zwifd_p ifd, uint8_t group, uint8_t max_cnt, uint8_t cnt, grp_member_t *grp_member)
{
	zwp_avi_interfaces_association_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_association_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (zwp_avi_interfaces_association_report_update(&state->report, group, max_cnt, cnt, grp_member) != ZWP_STATUS_OK)
	{
		goto l_err_report_update;
	}

	state->report.report_receive_grouping_identifier = group;
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
 * @brief Get argument for Get command from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Get command
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_association_get_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_association_get_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_association_get_t *argument;
	int integer;

	if ((argument = (zwp_avi_interfaces_association_get_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "grouping_identifier", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_get_value;
	}
	argument->grouping_identifier = integer;

	ZWP_LOG(ZWP_LOG_DEBUG, "Grouping Identifier: %d", argument->grouping_identifier);

	if ((argument->grouping_identifier != COMMAND_ASSOCIATION_GROUPINGS_IDENTIFIER_NONE)
			&& (zwu_number_is_uint8(argument->grouping_identifier) != ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid grouping identifier: %d", argument->grouping_identifier);
		goto l_err_invalid_grouping_identifier;
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_invalid_grouping_identifier:
l_err_get_value:
 	ZWP_FREE(argument);
l_err_args_alloc:
 	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Get command
 * @param[in]     args              Arguments passed with Get command
 * @return        Nothing
 * @see zwp_avi_interfaces_association_get_argument
 */
static void zwp_avi_interfaces_association_get_argument_free(void *args)
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
static zwp_status_t zwp_avi_interfaces_association_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_association_state_t *state;
	zwp_avi_interfaces_association_get_t *argument;

	argument = (zwp_avi_interfaces_association_get_t *)args;

	if ((state = (zwp_avi_interfaces_association_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->report.report_receive_condition;

	if ((zwp_interface_report_receive_assertion(state->report.report_receive_state) != ZWP_STATUS_OK)
			|| ((argument->grouping_identifier != COMMAND_ASSOCIATION_GROUPINGS_IDENTIFIER_NONE) && (state->report.report_receive_grouping_identifier != argument->grouping_identifier)))
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
static zwp_status_t zwp_avi_interfaces_association_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_association_state_t *state;
	zwp_avi_interfaces_association_get_t *argument;
	uint8_t grouping_identifier;

	argument = (zwp_avi_interfaces_association_get_t *)args;

	if ((state = (zwp_avi_interfaces_association_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->report.report_receive_state);

	if (argument->grouping_identifier == COMMAND_ASSOCIATION_GROUPINGS_IDENTIFIER_NONE)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Missing one or more required arguments");
		goto l_err_invalid_argument;
	}

	grouping_identifier = (uint8_t)argument->grouping_identifier;

	*hcapi_code_p = zwif_group_get(desc_interface, grouping_identifier, zwp_avi_interfaces_association_report_handler);

	return ZWP_STATUS_OK;

l_err_invalid_argument:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Allocate Result object corresponding to Report command
 * @param[out]    report_dest       Destination Report
 * @param[in]     report_src        Source Report
 * @param[in]     grouping_identifier  Grouping identifier
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_association_get_result_alloc(zwp_avi_interfaces_association_report_t *report_dest,
		zwp_avi_interfaces_association_report_t *report_src, int grouping_identifier)
{
	int association_value_count;
	int i;
	zwp_avi_interfaces_association_report_association_value_t *association_value_dest;
	zwp_avi_interfaces_association_report_association_value_t *association_value_src;

	association_value_count = zwu_array_list_size(report_src->association_value_list);

	for (i = 0; i < association_value_count; i++)
	{
		association_value_src = zwu_array_list_get(report_src->association_value_list, i);

		// If grouping_identifier doesn't match, skip the entry
		if ((grouping_identifier != COMMAND_ASSOCIATION_GROUPINGS_IDENTIFIER_NONE) && (association_value_src->grouping_identifier != grouping_identifier))
		{
			continue;
		}

		if ((association_value_dest = (zwp_avi_interfaces_association_report_association_value_t *)ZWP_CALLOC(1, sizeof(*association_value_dest))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		if (zwu_array_list_add(report_dest->association_value_list, association_value_dest) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}

		memcpy(association_value_dest, association_value_src, sizeof(*association_value_dest));

		if ((association_value_dest->group_members = (grp_member_t *)ZWP_CALLOC(1,
				sizeof(*association_value_dest->group_members) * association_value_dest->group_members_count)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_endpoints_list_alloc;
		}

		memcpy(association_value_dest->group_members, association_value_src->group_members,
				sizeof(*association_value_dest->group_members) * association_value_dest->group_members_count);
	}

	return ZWP_STATUS_OK;

	ZWP_FREE(association_value_dest->group_members);
l_err_endpoints_list_alloc:
	zwu_array_list_remove(report_dest->association_value_list, association_value_dest);
l_err_value_list_add:
	ZWP_FREE(association_value_dest);
l_err_value_alloc:
	for (i--; i >= 0; i--)
	{
		association_value_dest = zwu_array_list_get(report_dest->association_value_list, i);
		ZWP_FREE(association_value_dest->group_members);
		ZWP_FREE(association_value_dest);
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
 * The caller should call zwp_avi_interfaces_association_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_association_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_association_state_t *state;
	zwp_avi_interfaces_association_report_t *report;
	zwp_avi_interfaces_association_get_t *argument;

	argument = (zwp_avi_interfaces_association_get_t *)args;

	if ((state = (zwp_avi_interfaces_association_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((report = (zwp_avi_interfaces_association_report_t *)ZWP_CALLOC(1, sizeof(*report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	if (zwu_array_list_create(0, &report->association_value_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	if (zwp_avi_interfaces_association_get_result_alloc(report, &state->report, argument->grouping_identifier) != ZWP_STATUS_OK)
	{
		goto l_err_result_alloc;
	}

	*result_p = report;

	return ZWP_STATUS_OK;

l_err_result_alloc:
	zwu_array_list_destroy(report->association_value_list);
l_err_value_list_create:
	ZWP_FREE(report);
l_err_report_alloc:
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
static zwp_status_t zwp_avi_interfaces_association_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_association_report_t *report;
	avro_value_t association_value_list_value;
	avro_value_t association_value_value;
	zwp_avi_interfaces_association_report_association_value_t *association_value;
	int association_value_count;
	int i;
	avro_value_t endpoints_list_value;
	avro_value_t endpoint_value;
	avro_value_t node_value;
	int j;

	report = (zwp_avi_interfaces_association_report_t *)result;

	if (avro_value_get_by_name(arg_value, "association_value_list", &association_value_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'association_value_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	association_value_count = zwu_array_list_size(report->association_value_list);

	for (i = 0; i < association_value_count; i++)
	{
		if (avro_value_append(&association_value_list_value, &association_value_value, 0) != 0)
		{
			goto l_err_append_association_value_value;
		}

		association_value = zwu_array_list_get(report->association_value_list, i);

		if ((zw_avro_field_set_long(&association_value_value, "utime", association_value->utime) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&association_value_value, "grouping_identifier", association_value->grouping_identifier) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&association_value_value, "max_nodes_supported", association_value->max_nodes_supported) != ZWP_STATUS_OK)
				)
		{
			goto l_err_set_association_value_value;
		}

		if (avro_value_get_by_name(&association_value_value, "endpoints_list", &endpoints_list_value, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'endpoints_list': %s", avro_strerror());
			goto l_err_get_list_value;
		}

		for (j = 0; j < association_value->group_members_count; j++)
		{
			if (avro_value_append(&endpoints_list_value, &endpoint_value, 0) != 0)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to append to 'endpoints_list': %s", avro_strerror());
				goto l_err_append_endpoint_value;
			}

			if (avro_value_get_by_name(&endpoint_value, "node", &node_value, NULL) != 0)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'node': %s", avro_strerror());
				goto l_err_get_node_value;
			}

			if ((zw_avro_field_set_int(&endpoint_value, "id", association_value->group_members[j].ep_id) != ZWP_STATUS_OK)
					|| (zw_avro_field_set_int(&node_value, "id",  association_value->group_members[j].node_id) != ZWP_STATUS_OK)
					)
			{
				goto l_err_set_endpoint_value;
			}
		}
	}

	return ZWP_STATUS_OK;

l_err_set_endpoint_value:
l_err_get_node_value:
l_err_append_endpoint_value:
l_err_set_association_value_value:
l_err_append_association_value_value:
l_err_get_list_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_association_get_execute_passive
 */
static void zwp_avi_interfaces_association_get_result_free(void *result)
{
	zwp_avi_interfaces_association_report_t *report;
	int association_value_count;
	int i;
	zwp_avi_interfaces_association_report_association_value_t *association_value;

	report = (zwp_avi_interfaces_association_report_t *)result;

	association_value_count = zwu_array_list_size(report->association_value_list);

	for (i = 0; i < association_value_count; i++)
	{
		association_value = zwu_array_list_get(report->association_value_list, i);
		ZWP_FREE(association_value->group_members);
		ZWP_FREE(association_value);
	}
	zwu_array_list_destroy(report->association_value_list);

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
 * The caller should call zwp_avi_interfaces_association_set_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_association_set_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_association_set_t *argument;
	int32_t action;
	int32_t grouping_identifier;
	avro_value_t endpoints_list_value;
	avro_value_t endpoint_value;
	avro_value_t node_value;
	int i;
	int32_t node_id;
	int32_t endpoint_id;

	if ((argument = (zwp_avi_interfaces_association_set_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if ((zw_avro_field_get_int(arg_value, "action", &action) != ZWP_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "grouping_identifier", &grouping_identifier) != ZWP_STATUS_OK)
			)
	{
		goto l_err_get_value;
	}

	if ((action != ZWP_ASSOCIATION_SET_ACTION_ADD) && (action != ZWP_ASSOCIATION_SET_ACTION_REMOVE))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid association set action: Add or Remove");
		goto l_err_invalid_set_action;
	}

	argument->action = action;
	argument->grouping_identifier = grouping_identifier;

	ZWP_LOG(ZWP_LOG_DEBUG, "Grouping Identifier: %d", argument->grouping_identifier);

	if (zwu_number_is_uint8(argument->grouping_identifier) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid grouping identifier: %d", argument->grouping_identifier);
		goto l_err_invalid_grouping_identifier;
	}

	if (avro_value_get_by_name(arg_value, "endpoints_list", &endpoints_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'endpoints_list': %s", avro_strerror());
		goto l_err_get_endpoints_list_value;
	}

	if (avro_value_get_size(&endpoints_list_value, &argument->group_members_count) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get endpoints list size");
		goto l_err_endpoints_list_size;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Endpoint count: %d", argument->group_members_count);

	if ((argument->group_members = (grp_member_t *)ZWP_CALLOC(1,
			sizeof(*argument->group_members) * argument->group_members_count)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_endpoints_list_alloc;
	}

	for (i = 0; i < argument->group_members_count; i++)
	{
		if (avro_value_get_by_index(&endpoints_list_value, i, &endpoint_value, NULL) != 0)
		{
			goto l_err_get_endpoint_value;
		}

		if (avro_value_get_by_name(&endpoint_value, "node", &node_value, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'node': %s", avro_strerror());
			goto l_err_get_node_value;
		}

		if ((zw_avro_field_get_int(&node_value, "id", &node_id) != ZWP_STATUS_OK)
				|| (zw_avro_field_get_int(&endpoint_value, "id", &endpoint_id) != ZWP_STATUS_OK)
				)
		{
			goto l_err_get_node_endpoint_value;
		}

		argument->group_members[i].node_id = node_id;
		argument->group_members[i].ep_id = endpoint_id;

		ZWP_LOG(ZWP_LOG_DEBUG, "Node ID: %d Endpoint ID: %d",
				argument->group_members[i].node_id, argument->group_members[i].ep_id);

		if (zwu_number_is_uint8(argument->group_members[i].node_id) != ZWU_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Invalid node id: %d", argument->group_members[i].node_id);
			goto l_err_invalid_node_id;
		}

		if (zwu_number_is_uint8(argument->group_members[i].ep_id) != ZWU_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Invalid endpoint id: %d", argument->group_members[i].ep_id);
			goto l_err_invalid_endpoint_id;
		}
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_invalid_endpoint_id:
l_err_invalid_node_id:
l_err_get_node_endpoint_value:
l_err_get_node_value:
l_err_get_endpoint_value:
	ZWP_FREE(argument->group_members);
l_err_endpoints_list_alloc:
l_err_endpoints_list_size:
l_err_get_endpoints_list_value:
l_err_invalid_grouping_identifier:
l_err_invalid_set_action:
l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Set command
 * @param[in]     args              Arguments passed with Set command
 * @return        Nothing
 * @see zwp_avi_interfaces_association_set_argument
 */
static void zwp_avi_interfaces_association_set_argument_free(void *args)
{
	zwp_avi_interfaces_association_set_t *argument;

	argument = (zwp_avi_interfaces_association_set_t *)args;

	ZWP_FREE(argument->group_members);
	ZWP_FREE(argument);
}

/**
 * @brief Execute Set command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments passed with Set command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_association_set_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_association_set_t *argument;
	zwp_net_t net;
	zwp_desc_net_t desc_net;
	zwepd_p endpoints;
	zwepd_p desc_endpoint;
	int i;

	argument = (zwp_avi_interfaces_association_set_t *)args;

	net = zwnet_get_user(desc_interface->net);

	desc_net = zwp_net_get_desc_net(net);


	if (argument->action == ZWP_ASSOCIATION_SET_ACTION_ADD)
	{
		if ((endpoints = (zwepd_p)ZWP_CALLOC(1, sizeof(*endpoints) * argument->group_members_count)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_endpoints_list_alloc;
		}

		for (i = 0; i < argument->group_members_count; i++)
		{
			if (zwp_desc_get_desc_endpoint(desc_net, argument->group_members[i].node_id, argument->group_members[i].ep_id,
					&desc_endpoint) != ZWP_STATUS_OK)
			{
				goto l_err_get_desc_endpoint;
			}

			memcpy(&endpoints[i], desc_endpoint, sizeof(*endpoints));
		}

		if (argument->group_members_count > 0)
		{
			*hcapi_code_p = zwif_group_add(desc_interface, argument->grouping_identifier, endpoints, argument->group_members_count);
		}
		else
		{
			//FIXME: Remove after check added in HCAPI
			*hcapi_code_p = ZW_ERR_VALUE;
		}

		ZWP_FREE(endpoints);
	}
	else
	{
		*hcapi_code_p = zwif_group_del(desc_interface, argument->grouping_identifier, argument->group_members, argument->group_members_count);
	}



	return ZWP_STATUS_OK;

l_err_get_desc_endpoint:
	ZWP_FREE(endpoints);
l_err_endpoints_list_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief HCAPI callback for Grouping Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  max_grp           maximum number of groupings
 * @return        Nothing
 */
static void zwp_avi_interfaces_association_groupings_report_handler(zwifd_p ifd, uint8_t max_grp, int valid)
{
	zwp_avi_interfaces_association_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_association_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (valid)
	{
		state->groupings_report.utime = zwu_time_now();

		state->groupings_report.supported_groupings = max_grp;
	}
	zwp_interface_report_receive_done(&state->groupings_report.report_receive_state, state->groupings_report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

l_err_get_state:
	zwp_net_release_desc_interface(ifd);
l_err_acquire_desc:

	return;
}

/**
 * @brief Test if the expected Groupings Report command is received
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Groupings Get command
 * @param[out]    report_receive_condition_p    Report command receive condition
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_association_groupings_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_association_state_t *state;

	if ((state = (zwp_avi_interfaces_association_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->groupings_report.report_receive_condition;

	if (zwp_interface_report_receive_assertion(state->groupings_report.report_receive_state) != ZWP_STATUS_OK)
	{
		goto l_err_report_not_received;
	}

	return ZWP_STATUS_OK;

l_err_report_not_received:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute active Groupings Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Groupings Get command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_association_groupings_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_association_state_t *state;

	if ((state = (zwp_avi_interfaces_association_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->groupings_report.report_receive_state);

	*hcapi_code_p = zwif_group_sup_get(desc_interface, zwp_avi_interfaces_association_groupings_report_handler, 0);

	return ZWP_STATUS_OK;

l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute passive Groupings Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Groupings Get command
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_association_groupings_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_association_groupings_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_association_state_t *state;
	zwp_avi_interfaces_association_groupings_report_t *groupings_report;

	if ((state = (zwp_avi_interfaces_association_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((groupings_report = (zwp_avi_interfaces_association_groupings_report_t *)ZWP_CALLOC(1, sizeof(*groupings_report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	memcpy(groupings_report, &state->groupings_report, sizeof(*groupings_report));

	*result_p = groupings_report;

	return ZWP_STATUS_OK;

	ZWP_FREE(groupings_report);
l_err_report_alloc:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to Groupings Report command
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_association_groupings_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_association_groupings_report_t *groupings_report;

	groupings_report = (zwp_avi_interfaces_association_groupings_report_t *)result;

	if ((zw_avro_field_set_long(arg_value, "utime", groupings_report->utime) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "supported_groupings", groupings_report->supported_groupings) != ZWP_STATUS_OK))
	{
		goto l_err_set_report_value;
	}

	return ZWP_STATUS_OK;

l_err_set_report_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Groupings Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_association_groupings_get_execute_passive
 */
static void zwp_avi_interfaces_association_groupings_get_result_free(void *result)
{
	zwp_avi_interfaces_association_groupings_report_t *groupings_report;

	groupings_report = (zwp_avi_interfaces_association_groupings_report_t *)result;

	ZWP_FREE(groupings_report);
}

/**
 * @brief HCAPI callback for Specific Group Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  group             current active group
 * @return        Nothing
 */
static void zwp_avi_interfaces_association_specific_group_report_handler(zwifd_p ifd, uint8_t group)
{
	zwp_avi_interfaces_association_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_association_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	state->specific_group_report.utime = zwu_time_now();

	state->specific_group_report.group = group;

	zwp_interface_report_receive_done(&state->specific_group_report.report_receive_state, state->specific_group_report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

l_err_get_state:
	zwp_net_release_desc_interface(ifd);
l_err_acquire_desc:

	return;
}

/**
 * @brief Test if the expected Specific Group Report command is received
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Specific Group Get command
 * @param[out]    report_receive_condition_p    Report command receive condition
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_association_specific_group_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_association_state_t *state;

	if ((state = (zwp_avi_interfaces_association_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->specific_group_report.report_receive_condition;

	if (zwp_interface_report_receive_assertion(state->specific_group_report.report_receive_state) != ZWP_STATUS_OK)
	{
		goto l_err_report_not_received;
	}

	return ZWP_STATUS_OK;

l_err_report_not_received:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute active Specific Group Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Specific Group Get command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_association_specific_group_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_association_state_t *state;

	if ((state = (zwp_avi_interfaces_association_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->specific_group_report.report_receive_state);

	*hcapi_code_p = zwif_group_actv_get(desc_interface, zwp_avi_interfaces_association_specific_group_report_handler);

	return ZWP_STATUS_OK;

l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute passive Specific Group Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Specific Group Get command
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_association_specific_group_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_association_specific_group_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_association_state_t *state;
	zwp_avi_interfaces_association_specific_group_report_t *specific_group_report;

	if ((state = (zwp_avi_interfaces_association_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((specific_group_report = (zwp_avi_interfaces_association_specific_group_report_t *)ZWP_CALLOC(1, sizeof(*specific_group_report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	memcpy(specific_group_report, &state->specific_group_report, sizeof(*specific_group_report));

	*result_p = specific_group_report;

	return ZWP_STATUS_OK;

	ZWP_FREE(specific_group_report);
l_err_report_alloc:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to Specific Group Report command
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_association_specific_group_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_association_specific_group_report_t *specific_group_report;

	specific_group_report = (zwp_avi_interfaces_association_specific_group_report_t *)result;

	if ((zw_avro_field_set_long(arg_value, "utime", specific_group_report->utime) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "group", specific_group_report->group) != ZWP_STATUS_OK))
	{
		goto l_err_set_report_value;
	}

	return ZWP_STATUS_OK;

l_err_set_report_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Specific Group Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_association_specific_group_get_execute_passive
 */
static void zwp_avi_interfaces_association_specific_group_get_result_free(void *result)
{
	zwp_avi_interfaces_association_specific_group_report_t *specific_group_report;

	specific_group_report = (zwp_avi_interfaces_association_specific_group_report_t *)result;

	ZWP_FREE(specific_group_report);
}

/** Handlers for Get command */
static zwp_avi_message_interfaces_t interfaces_association_get =
{
		"interface_association_get",
		COMMAND_CLASS_ASSOCIATION, NULL,
		zwp_avi_interfaces_association_state_alloc, zwp_avi_interfaces_association_state_free,
		zwp_avi_interfaces_association_get_argument, zwp_avi_interfaces_association_get_argument_free,
		zwp_avi_interfaces_association_get_report_receive_assertion,
		zwp_avi_interfaces_association_get_execute_active, zwp_avi_interfaces_association_get_execute_passive,
		zwp_avi_interfaces_association_get_result, zwp_avi_interfaces_association_get_result_free
};

/** Handlers for Set command */
static zwp_avi_message_interfaces_t interfaces_association_set =
{
		"interface_association_set",
		COMMAND_CLASS_ASSOCIATION, NULL,
		NULL, NULL,
		zwp_avi_interfaces_association_set_argument, zwp_avi_interfaces_association_set_argument_free,
		NULL,
		zwp_avi_interfaces_association_set_execute_active, NULL,
		NULL, NULL
};

/** Handlers for Groupings Get command */
static zwp_avi_message_interfaces_t interfaces_association_groupings_get =
{
		"interface_association_groupings_get",
		COMMAND_CLASS_ASSOCIATION, NULL,
		NULL, NULL,
		NULL, NULL,
		zwp_avi_interfaces_association_groupings_get_report_receive_assertion,
		zwp_avi_interfaces_association_groupings_get_execute_active, zwp_avi_interfaces_association_groupings_get_execute_passive,
		zwp_avi_interfaces_association_groupings_get_result, zwp_avi_interfaces_association_groupings_get_result_free
};

/** Handlers for Specific Group Get command */
static zwp_avi_message_interfaces_t interfaces_association_specific_group_get =
{
		"interface_association_specific_group_get",
		COMMAND_CLASS_ASSOCIATION, NULL,
		NULL, NULL,
		NULL, NULL,
		zwp_avi_interfaces_association_specific_group_get_report_receive_assertion,
		zwp_avi_interfaces_association_specific_group_get_execute_active, zwp_avi_interfaces_association_specific_group_get_execute_passive,
		zwp_avi_interfaces_association_specific_group_get_result, zwp_avi_interfaces_association_specific_group_get_result_free
};

/** Handlers for Multi channel Association Get command */
static zwp_avi_message_interfaces_t interfaces_multi_channel_association_get =
{
		"interface_association_get",
		COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2, NULL,
		zwp_avi_interfaces_association_state_alloc, zwp_avi_interfaces_association_state_free,
		zwp_avi_interfaces_association_get_argument, zwp_avi_interfaces_association_get_argument_free,
		zwp_avi_interfaces_association_get_report_receive_assertion,
		zwp_avi_interfaces_association_get_execute_active, zwp_avi_interfaces_association_get_execute_passive,
		zwp_avi_interfaces_association_get_result, zwp_avi_interfaces_association_get_result_free
};

/** Handlers for Multi channel Association Set command */
static zwp_avi_message_interfaces_t interfaces_multi_channel_association_set =
{
		"interface_association_set",
		COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2, NULL,
		NULL, NULL,
		zwp_avi_interfaces_association_set_argument, zwp_avi_interfaces_association_set_argument_free,
		NULL,
		zwp_avi_interfaces_association_set_execute_active, NULL,
		NULL, NULL
};

/** Handlers for Multi channel Association Groupings Get command */
static zwp_avi_message_interfaces_t interfaces_multi_channel_association_groupings_get =
{
		"interface_association_groupings_get",
		COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2, NULL,
		NULL, NULL,
		NULL, NULL,
		zwp_avi_interfaces_association_groupings_get_report_receive_assertion,
		zwp_avi_interfaces_association_groupings_get_execute_active, zwp_avi_interfaces_association_groupings_get_execute_passive,
		zwp_avi_interfaces_association_groupings_get_result, zwp_avi_interfaces_association_groupings_get_result_free
};

/** Handlers for Multi channel Association Speicific Group Get command */
static zwp_avi_message_interfaces_t interfaces_multi_channel_association_specific_group_get =
{
		"interface_association_specific_group_get",
		COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2, NULL,
		NULL, NULL,
		NULL, NULL,
		zwp_avi_interfaces_association_specific_group_get_report_receive_assertion,
		zwp_avi_interfaces_association_specific_group_get_execute_active, zwp_avi_interfaces_association_specific_group_get_execute_passive,
		zwp_avi_interfaces_association_specific_group_get_result, zwp_avi_interfaces_association_specific_group_get_result_free
};

/** Register command handlers */
zwp_status_t zwp_avi_interfaces_association_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_association_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_association_set) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_association_groupings_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_association_specific_group_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_multi_channel_association_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_multi_channel_association_set) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_multi_channel_association_groupings_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_multi_channel_association_specific_group_get) != ZWP_STATUS_OK)
			)
	{
		goto l_err_interface_list_get;
	}

	return ZWP_STATUS_OK;

l_err_interface_list_get:
	return ZWP_STATUS_ERROR;
}




