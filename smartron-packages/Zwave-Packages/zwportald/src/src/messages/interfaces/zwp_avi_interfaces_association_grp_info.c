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
 * @file    zwp_avi_interfaces_association_grp_info.c
 *
 * @brief   AVI message 'interface_association_grp_info_xxx'
 * @details Handles the AVI message 'interface_association_grp_info_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-02-20
 * - Initial version
 */

#include "zwp_avi_interfaces.h"

/** Report command state */
typedef struct {
	if_grp_info_dat_t *grp_info_list;                                        /**< List of group information */
} zwp_avi_interfaces_association_grp_info_report_t;

/** Interface level state */
typedef struct {
	zwp_avi_interfaces_association_grp_info_report_t                report;  /**< Report command state */
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
 * The caller should call zwp_avi_interfaces_association_grp_info_state_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_association_grp_info_state_alloc(void **state_p, zwp_pool_t pool)
{
	zwp_avi_interfaces_association_state_t *state;

	if ((state = (zwp_avi_interfaces_association_state_t *)ZWP_CALLOC(1, sizeof(*state))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_state_alloc;
	}

	state->report.grp_info_list = NULL;

	*state_p = state;

	return ZWP_STATUS_OK;

	ZWP_FREE(state);
l_err_state_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free interface level data
 * @param[in]     arg               Interface level data
 * @return        Nothing
 * @see zwp_avi_interfaces_association_grp_info_state_alloc
 */
static void zwp_avi_interfaces_association_grp_info_state_free(void *arg)
{
	zwp_avi_interfaces_association_state_t *state;

	state = (zwp_avi_interfaces_association_state_t *)arg;

	zwif_group_info_free(state->report.grp_info_list);

	ZWP_FREE(state);
}

/**
 * @brief Update interface level state with the new Report
 * @param[in]     report            Report state in interface level
 * @param[in]     grp_info_list     Group information list
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_association_grp_info_report_update(zwp_avi_interfaces_association_grp_info_report_t *report,
		if_grp_info_dat_t *grp_info_list)
{

	zwif_group_info_free(report->grp_info_list);

	report->grp_info_list = grp_info_list;

	return ZWP_STATUS_OK;
}

/**
 * @brief Group information report handler
 * @param[in]	  desc_interface    Interface descriptor
 * @param[in]	  grp_info_list	    Group information list
 */
static zwp_status_t zwp_avi_interfaces_association_grp_info_report_handler(zwifd_p desc_interface, if_grp_info_dat_t *grp_info_list)
{
	zwp_avi_interfaces_association_state_t *state;

	if ((state = (zwp_avi_interfaces_association_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (zwp_avi_interfaces_association_grp_info_report_update(&state->report, grp_info_list) != ZWP_STATUS_OK)
	{
		goto l_err_report_update;
	}

	return ZWP_STATUS_OK;

l_err_report_update:
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
static zwp_status_t zwp_avi_interfaces_association_grp_info_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	if_grp_info_dat_t *grp_info_list;

	*hcapi_code_p = zwif_group_info_get(desc_interface, &grp_info_list);

	if (zwp_avi_interfaces_association_grp_info_report_handler(desc_interface, grp_info_list) != ZWP_STATUS_OK)
	{
		goto l_err_report_handler;
	}

	// Change to 'Queued' state so that the caller doesn't wait for Z-Wave ACK.
	if (*hcapi_code_p == ZW_ERR_NONE)
	{
		*hcapi_code_p = ZW_ERR_QUEUED;
	}

	return ZWP_STATUS_OK;

l_err_report_handler:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Allocate Result object corresponding to Report command
 * @param[out]    report_dest       Destination Report
 * @param[in]     report_src        Source Report
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_association_grp_info_get_result_alloc(zwp_avi_interfaces_association_grp_info_report_t *report_dest,
		zwp_avi_interfaces_association_grp_info_report_t *report_src)
{
	int i;
	if_grp_info_dat_t *grp_info_list_dest;
	if_grp_info_dat_t *grp_info_list_src;
	zw_grp_info_p grp_info;

	grp_info_list_src = report_src->grp_info_list;

	if ((grp_info_list_dest = (if_grp_info_dat_t *)ZWP_CALLOC(1, sizeof(*grp_info_list_dest)
			+ (sizeof(*grp_info_list_dest->grp_info) * grp_info_list_src->valid_grp_cnt))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_grp_info_list_alloc;
	}

	memcpy(grp_info_list_dest, grp_info_list_src, sizeof(*grp_info_list_dest));

	for (i = 0; i < grp_info_list_dest->valid_grp_cnt; i++)
	{
		if ((grp_info = (zw_grp_info_p)ZWP_CALLOC(1, sizeof(*grp_info)
				+ (sizeof(*grp_info->cmd_lst) * grp_info_list_src->grp_info[i]->cmd_ent_cnt))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_group_info_alloc;
		}

		memcpy(grp_info, grp_info_list_src->grp_info[i], sizeof(*grp_info));

		memcpy(grp_info->cmd_lst, grp_info_list_src->grp_info[i]->cmd_lst,
				sizeof(*grp_info->cmd_lst) * grp_info->cmd_ent_cnt);

		grp_info_list_dest->grp_info[i] = grp_info;
	}

	report_dest->grp_info_list = grp_info_list_dest;

	return ZWP_STATUS_OK;

	ZWP_FREE(grp_info);
l_err_group_info_alloc:
	for (i--; i >= 0; i--)
	{
		grp_info = grp_info_list_dest->grp_info[i];
		ZWP_FREE(grp_info);
	}
	ZWP_FREE(grp_info_list_dest);
l_err_grp_info_list_alloc:
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
 * The caller should call zwp_avi_interfaces_association_grp_info_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_association_grp_info_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_association_state_t *state;
	zwp_avi_interfaces_association_grp_info_report_t *report;

	if ((state = (zwp_avi_interfaces_association_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((report = (zwp_avi_interfaces_association_grp_info_report_t *)ZWP_CALLOC(1, sizeof(*report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	if (zwp_avi_interfaces_association_grp_info_get_result_alloc(report, &state->report) != ZWP_STATUS_OK)
	{
		goto l_err_result_alloc;
	}

	*result_p = report;

	return ZWP_STATUS_OK;

l_err_result_alloc:
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
static zwp_status_t zwp_avi_interfaces_association_grp_info_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_association_grp_info_report_t *report;
	if_grp_info_dat_t *grp_info_list;
	avro_value_t association_grp_info_list_value;
	avro_value_t association_grp_info_value;
	zw_grp_info_p grp_info;
	int i;
	avro_value_t command_list_value;
	avro_value_t command_value;
	int j;
	grp_cmd_ent_t *command;

	report = (zwp_avi_interfaces_association_grp_info_report_t *)result;

	grp_info_list = report->grp_info_list;

	if ((zw_avro_field_set_int(arg_value, "dynamic_info", grp_info_list->dynamic) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "group_count", grp_info_list->group_cnt) != ZWP_STATUS_OK)
			)
	{
		goto l_err_get_fields;
	}

	if (avro_value_get_by_name(arg_value, "association_grp_info_list", &association_grp_info_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'association_grp_info_list': %s", avro_strerror());
		goto l_err_get_association_grp_info_list_value;
	}

	for (i = 0; i < grp_info_list->valid_grp_cnt; i++)
	{
		grp_info = grp_info_list->grp_info[i];

		if (avro_value_append(&association_grp_info_list_value, &association_grp_info_value, 0) != 0)
		{
			goto l_err_append_association_grp_info_value;
		}

		if ((zw_avro_field_set_int(&association_grp_info_value, "grouping_identifier", grp_info->grp_num) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&association_grp_info_value, "profile", grp_info->profile) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&association_grp_info_value, "event_code", grp_info->evt_code) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_string(&association_grp_info_value, "name", grp_info->name) != ZWP_STATUS_OK)
				)
		{
			goto l_err_set_association_grp_info_value;
		}

		if (avro_value_get_by_name(&association_grp_info_value, "command_list", &command_list_value, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'command_list': %s", avro_strerror());
			goto l_err_get_command_list_value;
		}

		for (j = 0; j < grp_info->cmd_ent_cnt; j++)
		{
			command = &grp_info->cmd_lst[j];

			if (avro_value_append(&command_list_value, &command_value, 0) != 0)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to append to 'command_list': %s", avro_strerror());
				goto l_err_append_command_value;
			}

			if ((zw_avro_field_set_string(&command_value, "command_class_name",
					zwp_hcapi_zwave_command_class_to_string(command->cls, 1)) != ZWP_STATUS_OK)
					|| (zw_avro_field_set_string(&command_value, "command_name",
							zwp_hcapi_zwave_command_to_string(command->cls, command->cmd)) != ZWP_STATUS_OK)
					)
			{
				goto l_err_set_command_value;
			}
		}
	}

	return ZWP_STATUS_OK;

l_err_set_command_value:
l_err_append_command_value:
l_err_get_command_list_value:
l_err_set_association_grp_info_value:
l_err_append_association_grp_info_value:
l_err_get_association_grp_info_list_value:
l_err_get_fields:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_association_grp_info_get_execute_passive
 */
static void zwp_avi_interfaces_association_grp_info_get_result_free(void *result)
{
	zwp_avi_interfaces_association_grp_info_report_t *report;
	if_grp_info_dat_t *grp_info_list;
	int i;

	report = (zwp_avi_interfaces_association_grp_info_report_t *)result;

	grp_info_list = report->grp_info_list;

	for (i = 0; i < grp_info_list->valid_grp_cnt; i++)
	{
		ZWP_FREE(grp_info_list->grp_info[i]);
	}

	ZWP_FREE(grp_info_list);

	ZWP_FREE(report);
}

/** Handlers for Get command */
static zwp_avi_message_interfaces_t interfaces_association_grp_info_get =
{
		"interface_association_grp_info_get",
		COMMAND_CLASS_ASSOCIATION_GRP_INFO, NULL,
		zwp_avi_interfaces_association_grp_info_state_alloc, zwp_avi_interfaces_association_grp_info_state_free,
		NULL, NULL,
		NULL,
		zwp_avi_interfaces_association_grp_info_get_execute_active, zwp_avi_interfaces_association_grp_info_get_execute_passive,
		zwp_avi_interfaces_association_grp_info_get_result, zwp_avi_interfaces_association_grp_info_get_result_free
};

/** Register command handlers */
zwp_status_t zwp_avi_interfaces_association_grp_info_init(zwu_array_list_t message_list)
{
	if (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_association_grp_info_get) != ZWP_STATUS_OK)
	{
		goto l_err_interface_list_get;
	}

	return ZWP_STATUS_OK;

l_err_interface_list_get:
	return ZWP_STATUS_ERROR;
}






