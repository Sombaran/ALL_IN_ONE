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
 * @file    zwp_avi_interfaces_firmware_update_md.c
 *
 * @brief   AVI message 'interface_firmware_xxx'
 * @details Handles the AVI message 'interface_firmware_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-02-26
 * - Initial version
 */

#include "zwp_avi_interfaces.h"
#include "zwp_io.h"
#include "zwu_checksum.h"
#include "zwp_node_state.h"

/** Manufacturer ID not specified */
#define COMMAND_MANUFACTURER_ID_NONE             (-1)
/** Firmware target not specified */
#define COMMAND_FIRMWARE_TARGET_NONE             (-1)
/** Firmware ID not specified */
#define COMMAND_FIRMWARE_ID_NONE                 (-1)
/** Firmare file name not specified */
#define COMMAND_FIRMWARE_FILE_NAME_NONE          NULL
/** Firmware file name empty */
#define COMMAND_FIRMWARE_FILE_NAME_EMPTY         "\0"
/** Hardware version not specified */
#define COMMAND_HARDWARE_VERSION_NONE              (-1)

/** CRC-CCITT checksum for firmware */
#define ZWP_FIRMWARE_CHECKSUM_METHOD_CRC_CCITT   1

/** Directory for firmwares under application data area */
#define ZWP_FIRMWARES_DIR             "firmwares"


/** Get command state */
typedef struct {
	int manufacturer_id;                                   /**< Manufacturer ID */
	int firmware_target;                                   /**< Firmware target */
	int firmware_id;                                       /**< Firmware ID */
	char *firmware_file_name;                              /**< Firmware file name */
	int hardware_version;                                  /**< Hardware version */
} zwp_avi_interfaces_firmware_get_t;

/** Info Report command state */
typedef struct {
	zwu_time_t         utime;                              /**< Last updated time of Info Report command */
	zwfw_info_t        firmware_info;                      /**< Firmware information */
#ifdef LOCAL_CLIENT_ENABLE
	char 			   *firmware_dir_location;			   /**< Directory location to place firmware file to be updated */
#endif

	int                report_receive_state;               /**< Info Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Info Report command receive condition */
} zwp_avi_interfaces_firmware_info_report_t;

/** Update status description */
typedef struct {
	int                manufacturer_id;                    /**< Manufacturer ID */
	int                firmware_target;                    /**< Firmware target */
	int                firmware_id;                        /**< Firmware ID */

	zwu_time_t         update_request_time;                /**< Firmware update request time */

	zwu_time_t         update_request_status_utime;        /**< Last updated time of firmware update request status */
	int                update_request_status;              /**< Firmware update request status */

	zwu_time_t         update_completion_status_utime;     /**< Last updated time of firmware update completion status */
	int                update_completion_status;           /**< Firmware update completion status */

	int                wait_time;                          /**< Wait time */
	int                wait_time_is_valid;                 /**< Flag to indicate if @a wait_time is valid */

	zwp_firmware_update_target_restart_status_t  target_restart_status;           /**< Firmware target restart status */
} zwp_avi_interfaces_firmware_update_status_t;

/** Report command state */
typedef struct {
	zwu_time_t         utime;                          /**< Last update time across all records */
	zwu_array_list_t   update_status_list;             /**< List of update statuses of type @a zwp_avi_interfaces_firmware_update_status_t */

	int                last_request_manufacturer_id;   /**< Manufacturer ID of last firmware update request */
	int                last_request_firmware_target;   /**< Firmware Target of last firmware update request */
	int                last_request_firmware_id;       /**< Firmware ID of last firmware update request */

	int                report_receive_state;           /**< Report command receive state */
	zwp_thread_cond_t  report_receive_condition;       /**< Report command receive condition */
} zwp_avi_interfaces_firmware_update_request_report_t;

/** Interface level state */
typedef struct {
	zwp_avi_interfaces_firmware_info_report_t      info_report;                      /**< Info Report command state */
	zwp_avi_interfaces_firmware_update_request_report_t     update_request_report;   /**< Update Request Report command state */
} zwp_avi_interfaces_firmware_state_t;

/**
 * @brief Allocate interface level data
 * @param[out]    state_p           Interface level data
 * @param[in]     pool              Memory pool to be used for allocation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_firmware_state_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_firmware_state_alloc(void **state_p, zwp_pool_t pool)
{
	zwp_avi_interfaces_firmware_state_t *state;

	if ((state = (zwp_avi_interfaces_firmware_state_t *)ZWP_CALLOC(1, sizeof(*state))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_state_alloc;
	}

	if (zwp_thread_cond_create(&state->info_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for info_report received event");
		goto l_err_net_info_report_received_cond_create;
	}

	state->info_report.firmware_info.other_fw_id = NULL;
	state->info_report.firmware_info.other_fw_cnt = 0;

	if (zwp_thread_cond_create(&state->update_request_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for report received event");
		goto l_err_net_report_received_cond_create;
	}

	state->update_request_report.last_request_manufacturer_id = COMMAND_MANUFACTURER_ID_NONE;
	state->update_request_report.last_request_firmware_target = COMMAND_FIRMWARE_TARGET_NONE;
	state->update_request_report.last_request_firmware_id = COMMAND_FIRMWARE_ID_NONE;

	if (zwu_array_list_create(0, &state->update_request_report.update_status_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	*state_p = state;

	return ZWP_STATUS_OK;

	zwu_array_list_destroy(state->update_request_report.update_status_list);
l_err_value_list_create:
	zwp_thread_cond_destroy(state->update_request_report.report_receive_condition);
l_err_net_report_received_cond_create:
	zwp_thread_cond_destroy(state->info_report.report_receive_condition);
l_err_net_info_report_received_cond_create:
	ZWP_FREE(state);
l_err_state_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free interface level data
 * @param[in]     arg               Interface level data
 * @return        Nothing
 * @see zwp_avi_interfaces_firmware_state_alloc
 */
static void zwp_avi_interfaces_firmware_state_free(void *arg)
{
	zwp_avi_interfaces_firmware_state_t *state;
	zwp_avi_interfaces_firmware_update_request_report_t *report;
	int firmware_value_count;
	int i;
	zwp_avi_interfaces_firmware_update_status_t *update_status;

	state = (zwp_avi_interfaces_firmware_state_t *)arg;
	report = &state->update_request_report;

	firmware_value_count = zwu_array_list_size(report->update_status_list);

	for (i = 0; i < firmware_value_count; i++)
	{
		update_status = zwu_array_list_get(report->update_status_list, i);

		zwp_firmware_update_target_restart_status_free(update_status->target_restart_status);

		ZWP_FREE(update_status);
	}

	zwu_array_list_destroy(report->update_status_list);

	zwp_interface_report_receive_done(&state->update_request_report.report_receive_state, state->update_request_report.report_receive_condition);

	zwp_thread_cond_destroy(state->update_request_report.report_receive_condition);

	zwp_interface_report_receive_done(&state->info_report.report_receive_state, state->info_report.report_receive_condition);

	zwp_thread_cond_destroy(state->info_report.report_receive_condition);

	ZWP_FREE(state->info_report.firmware_info.other_fw_id);

	ZWP_FREE(state);
}

/**
 * @brief HCAPI callback for Info Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  fw_info	        firmware information
 * @return        Nothing
 */
static void zwp_avi_interfaces_firmware_info_report_handler(zwifd_p ifd, zwfw_info_t *fw_info)
{
	zwp_avi_interfaces_firmware_state_t *state;
	zwifd_p desc_interface;
	uint16_t *tmp;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_firmware_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	state->info_report.utime = zwu_time_now();

	if (fw_info->other_fw_cnt > state->info_report.firmware_info.other_fw_cnt)
	{
		if ((tmp = (uint16_t *)ZWP_REALLOC(state->info_report.firmware_info.other_fw_id,
				sizeof(*tmp) * fw_info->other_fw_cnt)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_type_list_alloc;
		}

		state->info_report.firmware_info.other_fw_id = tmp;
	}
	state->info_report.firmware_info.other_fw_cnt = fw_info->other_fw_cnt;

	// Save firmware id list pointer
	tmp = state->info_report.firmware_info.other_fw_id;

	// Copy zwfw_info_t
	memcpy(&state->info_report.firmware_info, fw_info, sizeof(state->info_report.firmware_info));

	// Restore firmware id list pointer
	state->info_report.firmware_info.other_fw_id = tmp;

	// Copy the firmware id list
	memcpy(state->info_report.firmware_info.other_fw_id, fw_info->other_fw_id,
			sizeof(*state->info_report.firmware_info.other_fw_id) * state->info_report.firmware_info.other_fw_cnt);

	zwp_interface_report_receive_done(&state->info_report.report_receive_state, state->info_report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

l_err_type_list_alloc:
l_err_get_state:
	zwp_net_release_desc_interface(ifd);
l_err_acquire_desc:

	return;
}

/**
 * @brief Test if the expected Info Report command is received
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Info Get command
 * @param[out]    report_receive_condition_p    Report command receive condition
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_firmware_info_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_firmware_state_t *state;

	if ((state = (zwp_avi_interfaces_firmware_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->info_report.report_receive_condition;

	if (zwp_interface_report_receive_assertion(state->info_report.report_receive_state) != ZWP_STATUS_OK)
	{
		goto l_err_report_not_received;
	}

	return ZWP_STATUS_OK;

l_err_report_not_received:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute active Info Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Info Get command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_firmware_info_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_firmware_state_t *state;

	if ((state = (zwp_avi_interfaces_firmware_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->info_report.report_receive_state);

	*hcapi_code_p = zwif_fw_info_get(desc_interface, zwp_avi_interfaces_firmware_info_report_handler);

	return ZWP_STATUS_OK;

l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute passive Info Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Info Get command
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_firmware_info_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_firmware_info_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_firmware_state_t *state;
	zwp_avi_interfaces_firmware_info_report_t *info_report;
#ifdef LOCAL_CLIENT_ENABLE
	zwp_net_t net;
	zwp_portal_t portal;
	char *local_state_dir;
	char *firmware_file_path;
#endif

	if ((state = (zwp_avi_interfaces_firmware_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((info_report = (zwp_avi_interfaces_firmware_info_report_t *)ZWP_CALLOC(1, sizeof(*info_report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	memcpy(info_report, &state->info_report, sizeof(*info_report));

	if ((info_report->firmware_info.other_fw_id = (uint16_t *)ZWP_CALLOC(1,
			sizeof(*info_report->firmware_info.other_fw_id) * state->info_report.firmware_info.other_fw_cnt)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_type_list_alloc;
	}

#ifdef LOCAL_CLIENT_ENABLE
	// Get local-state-dir : 'var' directory
	net = zwnet_get_user(desc_interface->net);
	portal = zwp_net_get_portal(net);
	local_state_dir = zwp_portal_get_local_state_dir(portal);

	// Set firmware dir path ..extra one for forward slash at the end
	if ((firmware_file_path = (char *)ZWP_MALLOC(strlen(local_state_dir) + 1 + 1)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_firmware_path_alloc;
	}

	if (realpath(local_state_dir, firmware_file_path) == NULL)
	{
		sprintf(firmware_file_path, "%s/", local_state_dir);
	}

	info_report->firmware_dir_location = firmware_file_path;

	ZWP_LOG(ZWP_LOG_DEBUG, "Firmware Directory Path: %s", firmware_file_path);
#endif

	memcpy(info_report->firmware_info.other_fw_id, state->info_report.firmware_info.other_fw_id,
				sizeof(*info_report->firmware_info.other_fw_id) * state->info_report.firmware_info.other_fw_cnt);

	*result_p = info_report;

	return ZWP_STATUS_OK;
#ifdef LOCAL_CLIENT_ENABLE
	ZWP_FREE(info_report->firmware_dir_location);
l_err_firmware_path_alloc:
#endif
	ZWP_FREE(info_report->firmware_info.other_fw_id);
l_err_type_list_alloc:
	ZWP_FREE(info_report);
l_err_report_alloc:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to Info Report command
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_firmware_info_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_firmware_info_report_t *info_report;
	avro_value_t firmware_id_list_value;
	avro_value_t firmware_id_value;
	int i;

	info_report = (zwp_avi_interfaces_firmware_info_report_t *)result;

	if ((zw_avro_field_set_long(arg_value, "utime", info_report->utime) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "manufacturer_id", info_report->firmware_info.vid) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "firmware_0_id", info_report->firmware_info.zw_fw_id) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "firmware_0_checksum", info_report->firmware_info.chksum) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "is_firmware_upgradable", info_report->firmware_info.upgrade_flg) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "max_fragment_size", info_report->firmware_info.max_frag_sz) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "is_fixed_frgament_size", info_report->firmware_info.fixed_frag_sz) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "hardware_version_valid", info_report->firmware_info.hw_ver_valid) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "hardware_version", info_report->firmware_info.hw_ver) != ZWP_STATUS_OK)
#ifdef LOCAL_CLIENT_ENABLE
			|| (zw_avro_field_set_string(arg_value, "firmware_directory_location", info_report->firmware_dir_location) != ZWP_STATUS_OK)
#endif
			)
	{
		goto l_err_set_report_value;
	}

	if (avro_value_get_by_name(arg_value, "firmware_n_id_list", &firmware_id_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'firmware_n_id_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	for (i = 0; i < info_report->firmware_info.other_fw_cnt; i++)
	{
		if (avro_value_append(&firmware_id_list_value, &firmware_id_value, 0) != 0)
		{
			goto l_err_append_firmware_id_value;
		}

		if (avro_value_set_int(&firmware_id_value, info_report->firmware_info.other_fw_id[i]) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to set firmware id '%d': %s", info_report->firmware_info.other_fw_id[i], avro_strerror());
			goto l_err_set_firmware_id;
		}
	}

	return ZWP_STATUS_OK;

l_err_set_firmware_id:
l_err_append_firmware_id_value:
l_err_get_list_value:
l_err_set_report_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Info Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_firmware_info_get_execute_passive
 */
static void zwp_avi_interfaces_firmware_info_get_result_free(void *result)
{
	zwp_avi_interfaces_firmware_info_report_t *info_report;

	info_report = (zwp_avi_interfaces_firmware_info_report_t *)result;

	ZWP_FREE(info_report->firmware_info.other_fw_id);
#ifdef LOCAL_CLIENT_ENABLE
	ZWP_FREE(info_report->firmware_dir_location);
#endif
	ZWP_FREE(info_report);
}

/**
 * @brief Find and update interface level state record
 * @param[in]     report            Report state in interface level
 * @param[out]	  update_status_p   Update status
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_firmware_update_status_find_record(zwp_avi_interfaces_firmware_update_request_report_t *report,
		zwp_avi_interfaces_firmware_update_status_t **update_status_p)
{
	int update_status_count;
	int i;
	zwp_avi_interfaces_firmware_update_status_t *update_status;

	update_status_count = zwu_array_list_size(report->update_status_list);

	for (i = 0; i < update_status_count; i++)
	{
		update_status = zwu_array_list_get(report->update_status_list, i);

		// If manufacturer_id, firmware_target and firmware_id matches, update the entry instead of creating new one.
		if ((update_status->manufacturer_id == report->last_request_manufacturer_id)
				&& (update_status->firmware_target == report->last_request_firmware_target)
				&& (update_status->firmware_id == report->last_request_firmware_id))
		{
			/*
			 * Remove and reinsert this record to make it the recently updated record.
			 * Assumption: This search function is always used by record update functions
			 */
			zwu_array_list_remove_at_index(report->update_status_list, i);

			if (zwu_array_list_add_at_index(report->update_status_list, update_status, 0) != ZWP_STATUS_OK)
			{
				goto l_err_value_list_add;
			}

			break;
		}
	}

	if (i >= update_status_count)
	{
		if ((update_status = (zwp_avi_interfaces_firmware_update_status_t *)ZWP_CALLOC(1, sizeof(*update_status))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		update_status->manufacturer_id = report->last_request_manufacturer_id;
		update_status->firmware_target = report->last_request_firmware_target;
		update_status->firmware_id = report->last_request_firmware_id;

		if (zwp_firmware_update_target_restart_status_alloc(&update_status->target_restart_status) != ZWP_STATUS_OK)
		{
			goto l_err_restart_status_alloc;
		}

		// Newly added record becomes the recently updated record
		if (zwu_array_list_add_at_index(report->update_status_list, update_status, 0) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}
	}

	*update_status_p = update_status;

	return ZWP_STATUS_OK;

l_err_value_list_add:
	zwp_firmware_update_target_restart_status_free(update_status->target_restart_status);
l_err_restart_status_alloc:
	ZWP_FREE(update_status);
l_err_value_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Update interface level state with the new Update Status Report
 * @param[in]     report            Report state in interface level
 * @param[in]	  status            Firmware update status, ZW_FW_UPDT_ERR_XXX
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_firmware_update_status_report_update(zwp_avi_interfaces_firmware_update_request_report_t *report, uint8_t status)
{
	zwu_time_t  time_now;
	zwp_avi_interfaces_firmware_update_status_t *update_status;

	if (zwp_avi_interfaces_firmware_update_status_find_record(report, &update_status) != ZWP_STATUS_OK)
	{
		goto l_err_find_record;
	}

	time_now = zwu_time_now();

	report->utime = time_now;
	update_status->update_request_status_utime = time_now;
	update_status->update_request_status = status;

	return ZWP_STATUS_OK;

l_err_find_record:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Update Status Report callback from HCAPI for Firmware Update Request Get
 * @param[in]	  ifd	            interface
 * @param[in]	  status            firmware update status, ZW_FW_UPDT_ERR_XXX
 * @return        Nothing
 */
static void zwp_avi_interfaces_firmware_update_status_report_handler(zwifd_p ifd, uint8_t status)
{
	zwp_avi_interfaces_firmware_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_firmware_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (zwp_avi_interfaces_firmware_update_status_report_update(&state->update_request_report, status) != ZWP_STATUS_OK)
	{
		goto l_err_report_update;
	}

	zwp_interface_report_receive_done(&state->update_request_report.report_receive_state, state->update_request_report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

l_err_report_update:
l_err_get_state:
	zwp_net_release_desc_interface(ifd);
l_err_acquire_desc:

	return;
}

/**
 * @brief Update interface level state with the new Update Completion Status Report
 * @param[in]     report            Report state in interface level
 * @param[in]	  status            firmware update completion status, ZW_FW_UPDT_CMPLT_XXX
 * @param[in]	  wait_tm           time (in seconds) that is needed before the receiving node becomes
 * available again for communication after the transfer of an image. This parameter is
 * valid only if wait_tm_valid=1
 * @param[in]	  wait_tm_valid     flag to indicate the wait_tm parameter is valid.
 * @param[in]     node_state        Node state
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_firmware_update_completion_status_report_update(zwp_avi_interfaces_firmware_update_request_report_t *report,
		uint8_t status, uint16_t wait_tm, int wait_tm_valid, zwp_node_state_t node_state)
{
	zwu_time_t  time_now;
	zwp_avi_interfaces_firmware_update_status_t *update_status;

	if (zwp_avi_interfaces_firmware_update_status_find_record(report, &update_status) != ZWP_STATUS_OK)
	{
		goto l_err_find_record;
	}

	time_now = zwu_time_now();

	report->utime = time_now;
	update_status->update_completion_status_utime = time_now;
	update_status->update_completion_status = status;
	update_status->wait_time = wait_tm;
	update_status->wait_time_is_valid = wait_tm_valid;

	if (wait_tm_valid == 1)
	{
		if (zwp_node_state_add_firmware_update_restart_status_listener(node_state, update_status->target_restart_status,
				time_now, wait_tm) != ZWP_STATUS_OK)
		{
			goto l_err_add_target_restart_status_listener;
		}
	}

	return ZWP_STATUS_OK;

l_err_add_target_restart_status_listener:
l_err_find_record:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Update Completion Status Report callback from HCAPI for Firmware Update Request Get
 * @param[in]	  ifd	            interface
 * @param[in]	  status            firmware update completion status, ZW_FW_UPDT_CMPLT_XXX
 * @param[in]	  wait_tm           time (in seconds) that is needed before the receiving node becomes
 * available again for communication after the transfer of an image. This parameter is
 * valid only if wait_tm_valid=1
 * @param[in]	  wait_tm_valid     flag to indicate the wait_tm parameter is valid.
 * @return        Nothing
 */
static void zwp_avi_interfaces_firmware_update_completion_status_report_handler(zwifd_p ifd, uint8_t status, uint16_t wait_tm, int wait_tm_valid)
{
	zwp_avi_interfaces_firmware_state_t *state;
	zwifd_p desc_interface;
	zwp_net_t net;
	zwp_desc_net_t desc_net;
	zwnoded_p desc_node;
	zwp_node_state_t node_state;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	net = zwnet_get_user(desc_interface->net);

	desc_net = zwp_net_get_desc_net(net);

	if (zwp_desc_get_desc_node(desc_net, desc_interface->nodeid, &desc_node) != ZW_ERR_NONE)
	{
		goto l_err_get_desc_node;
	}

	if ((state = (zwp_avi_interfaces_firmware_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((node_state = zwp_node_state_get(desc_node)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get node state");
		goto l_err_get_node_state;
	}

	if (zwp_avi_interfaces_firmware_update_completion_status_report_update(&state->update_request_report,
			status, wait_tm, wait_tm_valid, node_state) != ZWP_STATUS_OK)
	{
		goto l_err_report_update;
	}

	zwp_interface_report_receive_done(&state->update_request_report.report_receive_state, state->update_request_report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

l_err_report_update:
l_err_get_node_state:
l_err_get_state:
l_err_get_desc_node:
	zwp_net_release_desc_interface(ifd);
l_err_acquire_desc:

	return;
}

/**
 * @brief Target Restart Status Report callback from HCAPI for Firmware Update Request Get
 * @param[in]	  noded	            node
 * @param[in]	  status	        firmware update target restart status, ZW_FW_UPDT_RESTART_XXX
 * @return        Nothing
 */
static void zwp_avi_interfaces_firmware_target_restart_status_report_handler(zwnoded_p noded, uint8_t status)
{
	zwp_node_state_t node_state;
	zwnoded_p desc_node;

	if (zwp_net_acquire_desc_node(noded, &desc_node) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((node_state = zwp_node_state_get(desc_node)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_node_state_event_firmware_update_target_restart_status(node_state, zwu_time_now(), status);

	zwp_net_release_desc_node(noded);

	return;

l_err_get_state:
	zwp_net_release_desc_node(noded);
l_err_acquire_desc:

	return;
}

/**
 * @brief Get argument for Update Request Get command from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Update Request Get command
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_firmware_update_request_get_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_firmware_update_request_get_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_firmware_get_t *argument;
	int integer;
	char *firmware_file_name;

	if ((argument = (zwp_avi_interfaces_firmware_get_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "manufacturer_id", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_get_value_manufacturer_id;
	}
	argument->manufacturer_id = integer;

	if (zw_avro_field_get_int(arg_value, "firmware_target", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_get_value_firmware_target;
	}
	argument->firmware_target = integer;

	if (zw_avro_field_get_int(arg_value, "firmware_id", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_get_value_firmware_id;
	}
	argument->firmware_id = integer;

	if (zw_avro_field_get_int(arg_value, "hardware_version", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_get_value_hardware_version;
	}
	argument->hardware_version = integer;

	if (zw_avro_field_get_string(arg_value, "firmware_file_name", &firmware_file_name) != ZWP_STATUS_OK)
	{
		goto l_err_get_value_firmware_file_name;
	}

	if ((argument->firmware_file_name = ZWP_STRDUP(firmware_file_name)) == NULL)
	{
		goto l_err_alloc_firmware_file_name;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Firmware Update Request:");
	ZWP_LOG(ZWP_LOG_DEBUG, "Manufacturer ID   : %d", argument->manufacturer_id);
	ZWP_LOG(ZWP_LOG_DEBUG, "Firmware Target   : %d", argument->firmware_target);
	ZWP_LOG(ZWP_LOG_DEBUG, "Firmware ID       : %d", argument->firmware_id);
	ZWP_LOG(ZWP_LOG_DEBUG, "Firmware File Name: %s", argument->firmware_file_name);
	ZWP_LOG(ZWP_LOG_DEBUG, "Hardware Version     : %d", argument->hardware_version);

	if ((argument->manufacturer_id != COMMAND_MANUFACTURER_ID_NONE)
			&& (zwu_number_is_uint16(argument->manufacturer_id) != ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid manufacturer id: %d", argument->manufacturer_id);
		goto l_err_invalid_manufacturer_id;
	}

	if ((argument->firmware_target != COMMAND_FIRMWARE_TARGET_NONE)
			&& (zwu_number_is_uint8(argument->firmware_target) != ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid firmware target: %d", argument->firmware_target);
		goto l_err_invalid_firmware_target;
	}

	if ((argument->firmware_id != COMMAND_FIRMWARE_ID_NONE)
			&& (zwu_number_is_uint16(argument->firmware_id) != ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid firmware id: %d", argument->firmware_id);
		goto l_err_invalid_firmware_id;
	}

	if ((argument->hardware_version != COMMAND_HARDWARE_VERSION_NONE)
			&& (zwu_number_is_uint8(argument->hardware_version) != ZWU_STATUS_OK))
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid hardware version: %d", argument->hardware_version);
		goto l_err_invalid_hardware_version;
	}

	zw_avro_field_free_string(firmware_file_name);

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_invalid_hardware_version:
l_err_invalid_firmware_id:
l_err_invalid_firmware_target:
l_err_invalid_manufacturer_id:
	ZWP_FREE(argument->firmware_file_name);
l_err_alloc_firmware_file_name:
	zw_avro_field_free_string(firmware_file_name);
l_err_get_value_firmware_file_name:
l_err_get_value_hardware_version:
l_err_get_value_firmware_id:
l_err_get_value_firmware_target:
l_err_get_value_manufacturer_id:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Update Request Get command
 * @param[in]     args              Arguments passed with Update Request Get command
 * @return        Nothing
 * @see zwp_avi_interfaces_firmware_update_request_get_argument
 */
static void zwp_avi_interfaces_firmware_update_request_get_argument_free(void *args)
{
	zwp_avi_interfaces_firmware_get_t *argument;

	argument = (zwp_avi_interfaces_firmware_get_t *)args;

	ZWP_FREE(argument->firmware_file_name);
	ZWP_FREE(argument);
}

/**
 * @brief Test if the expected Update Request Report command is received
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Update Request Get command
 * @param[out]    report_receive_condition_p    Report command receive condition
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_firmware_update_request_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_firmware_state_t *state;

	if ((state = (zwp_avi_interfaces_firmware_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->update_request_report.report_receive_condition;

	if (zwp_interface_report_receive_assertion(state->update_request_report.report_receive_state) != ZWP_STATUS_OK)
	{
		goto l_err_report_not_received;
	}

	return ZWP_STATUS_OK;

l_err_report_not_received:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute active Update Request Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Update Request Get command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_firmware_update_request_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_firmware_state_t *state;
	zwp_avi_interfaces_firmware_get_t *argument;
	zwfw_updt_req_t update_request;
	zwp_net_t net;
	zwp_portal_t portal;
	char *local_state_dir;
	char *firmware_file_path;
	zwp_avi_interfaces_firmware_update_status_t *update_status;
	zwu_time_t time_now;

	argument = (zwp_avi_interfaces_firmware_get_t *)args;

	// Get local-state-dir : 'var' directory
	net = zwnet_get_user(desc_interface->net);
	portal = zwp_net_get_portal(net);
	local_state_dir = zwp_portal_get_local_state_dir(portal);

	if ((state = (zwp_avi_interfaces_firmware_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->update_request_report.report_receive_state);

	if ((argument->manufacturer_id == COMMAND_MANUFACTURER_ID_NONE)
			|| (argument->firmware_target == COMMAND_FIRMWARE_TARGET_NONE)
			|| (argument->firmware_id == COMMAND_FIRMWARE_ID_NONE)
			|| (argument->firmware_file_name == COMMAND_FIRMWARE_FILE_NAME_NONE)
			|| (!strcmp(argument->firmware_file_name, COMMAND_FIRMWARE_FILE_NAME_EMPTY))
			|| (argument->hardware_version == COMMAND_HARDWARE_VERSION_NONE)
			)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Missing one or more required arguments");
		goto l_err_invalid_argument;
	}

	// Security check
	if (zwp_file_name_is_safe(argument->firmware_file_name) != ZWP_STATUS_OK)
	{
		goto l_err_unsafe_file_name;
	}

	// Set firmware update parameters
	update_request.vid = (uint16_t)argument->manufacturer_id;
	update_request.fw_tgt = (uint8_t)argument->firmware_target;
	update_request.fw_id = (uint16_t)argument->firmware_id;
	update_request.hw_ver = (uint8_t)argument->hardware_version;
	update_request.sts_cb = zwp_avi_interfaces_firmware_update_status_report_handler;
	update_request.cmplt_cb = zwp_avi_interfaces_firmware_update_completion_status_report_handler;
	update_request.restart_cb = zwp_avi_interfaces_firmware_target_restart_status_report_handler;

	// Set full path of firmware file name
	if ((firmware_file_path = (char *)ZWP_MALLOC(strlen(local_state_dir) + 1 + sizeof(ZWP_FIRMWARES_DIR) +
			strlen(argument->firmware_file_name) + 1)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_firmware_path_alloc;
	}
	sprintf(firmware_file_path, "%s/%s/%s", local_state_dir, ZWP_FIRMWARES_DIR, argument->firmware_file_name);
	update_request.fw_file = firmware_file_path;
	ZWP_LOG(ZWP_LOG_DEBUG, "Firmware File Path: %s", update_request.fw_file);

	*hcapi_code_p = zwif_fw_updt_req(desc_interface, &update_request);

	if ((*hcapi_code_p == ZW_ERR_NONE) || (*hcapi_code_p == ZW_ERR_QUEUED))
	{
		state->update_request_report.last_request_manufacturer_id = argument->manufacturer_id;
		state->update_request_report.last_request_firmware_target = argument->firmware_target;
		state->update_request_report.last_request_firmware_id = argument->firmware_id;
	}

	if (zwp_avi_interfaces_firmware_update_status_find_record(&state->update_request_report, &update_status) != ZWP_STATUS_OK)
	{
		goto l_err_find_record;
	}

	time_now = zwu_time_now();

	state->update_request_report.utime = time_now;
	update_status->update_request_time = time_now;

	ZWP_FREE(firmware_file_path);

	return ZWP_STATUS_OK;

l_err_find_record:
	ZWP_FREE(firmware_file_path);
l_err_firmware_path_alloc:
l_err_unsafe_file_name:
l_err_invalid_argument:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Allocate Result object corresponding to Update Request Report command
 * @param[out]    report_dest       Destination Report
 * @param[in]     report_src        Source Report
 * @param[in]     manufacturer_id   Manufacturer ID
 * @param[in]     firmware_target   Firmware target
 * @param[in]     firmware_id       Firmware ID
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_firmware_update_request_get_result_alloc(zwp_avi_interfaces_firmware_update_request_report_t *report_dest,
		zwp_avi_interfaces_firmware_update_request_report_t *report_src,
		int manufacturer_id, int firmware_target, int firmware_id)
{
	int firmware_value_count;
	int i;
	zwp_avi_interfaces_firmware_update_status_t *firmware_value_dest;
	zwp_avi_interfaces_firmware_update_status_t *firmware_value_src;

	firmware_value_count = zwu_array_list_size(report_src->update_status_list);

	for (i = 0; i < firmware_value_count; i++)
	{
		firmware_value_src = zwu_array_list_get(report_src->update_status_list, i);

#if 0
		ZWP_LOG(ZWP_LOG_DEBUG, "Manufacturer Target: %d\n", firmware_value_src->manufacturer_id);
		ZWP_LOG(ZWP_LOG_DEBUG, "Firmware Target    : %d\n", firmware_value_src->firmware_target);
		ZWP_LOG(ZWP_LOG_DEBUG, "Firmware ID        : %d\n", firmware_value_src->firmware_id);
#endif

		// If manufacturer_id or firmware_target or firmware_id doesn't match, skip the entry
		if (((manufacturer_id != COMMAND_MANUFACTURER_ID_NONE) && (firmware_value_src->manufacturer_id != manufacturer_id))
				|| ((firmware_target != COMMAND_FIRMWARE_TARGET_NONE) && (firmware_value_src->firmware_target != firmware_target))
				|| ((firmware_id != COMMAND_FIRMWARE_ID_NONE) && (firmware_value_src->firmware_id != firmware_id)))
		{
			continue;
		}

		if ((firmware_value_dest = (zwp_avi_interfaces_firmware_update_status_t *)ZWP_CALLOC(1, sizeof(*firmware_value_dest))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_value_alloc;
		}

		if (zwu_array_list_add(report_dest->update_status_list, firmware_value_dest) != ZWP_STATUS_OK)
		{
			goto l_err_value_list_add;
		}

		memcpy(firmware_value_dest, firmware_value_src, sizeof(*firmware_value_dest));
	}

	return ZWP_STATUS_OK;

l_err_value_list_add:
	ZWP_FREE(firmware_value_dest);
l_err_value_alloc:
	for (i--; i >= 0; i--)
	{
		ZWP_FREE(zwu_array_list_get(report_dest->update_status_list, i));
	}
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute passive Update Request Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Update Request Get command
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_firmware_update_request_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_firmware_update_request_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_firmware_state_t *state;
	zwp_avi_interfaces_firmware_update_request_report_t *report;
	zwp_avi_interfaces_firmware_get_t *argument;

	argument = (zwp_avi_interfaces_firmware_get_t *)args;

	if ((state = (zwp_avi_interfaces_firmware_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((report = (zwp_avi_interfaces_firmware_update_request_report_t *)ZWP_CALLOC(1, sizeof(*report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	if (zwu_array_list_create(0, &report->update_status_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	if (zwp_avi_interfaces_firmware_update_request_get_result_alloc(report, &state->update_request_report,
			argument->manufacturer_id, argument->firmware_target, argument->firmware_id) != ZWP_STATUS_OK)
	{
		goto l_err_result_alloc;
	}

	*result_p = report;

	return ZWP_STATUS_OK;

l_err_result_alloc:
	zwu_array_list_destroy(report->update_status_list);
l_err_value_list_create:
	ZWP_FREE(report);
l_err_report_alloc:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to Update Request Report command
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_firmware_update_request_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_firmware_update_request_report_t *report;
	avro_value_t update_status_list_value;
	avro_value_t update_status_value;
	zwp_avi_interfaces_firmware_update_status_t *update_status;
	int firmware_value_count;
	int i;
	zwu_time_t target_restart_status_utime;
	uint8_t target_restart_status;

	report = (zwp_avi_interfaces_firmware_update_request_report_t *)result;

	if (avro_value_get_by_name(arg_value, "update_status_list", &update_status_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'udpate_status_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	firmware_value_count = zwu_array_list_size(report->update_status_list);

	for (i = 0; i < firmware_value_count; i++)
	{
		if (avro_value_append(&update_status_list_value, &update_status_value, 0) != 0)
		{
			goto l_err_append_firmware_value_value;
		}

		update_status = zwu_array_list_get(report->update_status_list, i);

		zwp_firmware_update_target_restart_status_get(update_status->target_restart_status, &target_restart_status_utime, &target_restart_status);

		if ((zw_avro_field_set_int(&update_status_value, "manufacturer_id", update_status->manufacturer_id) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&update_status_value, "firmware_target", update_status->firmware_target) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&update_status_value, "firmware_id", update_status->firmware_id) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_long(&update_status_value, "update_request_time", update_status->update_request_time) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_long(&update_status_value, "update_request_status_utime", update_status->update_request_status_utime) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&update_status_value, "update_request_status", update_status->update_request_status) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_long(&update_status_value, "update_completion_status_utime", update_status->update_completion_status_utime) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&update_status_value, "update_completion_status", update_status->update_completion_status) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&update_status_value, "wait_time", update_status->wait_time) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&update_status_value, "wait_time_is_valid", update_status->wait_time_is_valid) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_long(&update_status_value, "target_restart_status_utime", target_restart_status_utime) != ZWP_STATUS_OK)
				|| (zw_avro_field_set_int(&update_status_value, "target_restart_status", target_restart_status) != ZWP_STATUS_OK)
				)
		{
			goto l_err_set_firmware_value_value;
		}
	}

	return ZWP_STATUS_OK;

l_err_set_firmware_value_value:
l_err_append_firmware_value_value:
l_err_get_list_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Update Request Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_firmware_update_request_get_execute_passive
 */
static void zwp_avi_interfaces_firmware_update_request_get_result_free(void *result)
{
	zwp_avi_interfaces_firmware_update_request_report_t *report;
	int firmware_value_count;
	int i;

	report = (zwp_avi_interfaces_firmware_update_request_report_t *)result;

	firmware_value_count = zwu_array_list_size(report->update_status_list);

	for (i = 0; i < firmware_value_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(report->update_status_list, i));
	}
	zwu_array_list_destroy(report->update_status_list);

	ZWP_FREE(report);
}

/** Handlers for Info Get command */
static zwp_avi_message_interfaces_t interfaces_firmware_info_get =
{
		"interface_firmware_info_get",
		COMMAND_CLASS_FIRMWARE_UPDATE_MD, NULL,
		zwp_avi_interfaces_firmware_state_alloc, zwp_avi_interfaces_firmware_state_free,
		NULL, NULL,
		zwp_avi_interfaces_firmware_info_get_report_receive_assertion,
		zwp_avi_interfaces_firmware_info_get_execute_active, zwp_avi_interfaces_firmware_info_get_execute_passive,
		zwp_avi_interfaces_firmware_info_get_result, zwp_avi_interfaces_firmware_info_get_result_free
};

/** Handlers for Update Request Get command */
static zwp_avi_message_interfaces_t interfaces_firmware_update_request_get =
{
		"interface_firmware_update_request_get",
		COMMAND_CLASS_FIRMWARE_UPDATE_MD, NULL,
		NULL, NULL,
		zwp_avi_interfaces_firmware_update_request_get_argument, zwp_avi_interfaces_firmware_update_request_get_argument_free,
		zwp_avi_interfaces_firmware_update_request_get_report_receive_assertion,
		zwp_avi_interfaces_firmware_update_request_get_execute_active, zwp_avi_interfaces_firmware_update_request_get_execute_passive,
		zwp_avi_interfaces_firmware_update_request_get_result, zwp_avi_interfaces_firmware_update_request_get_result_free
};

/** Register command handlers */
zwp_status_t zwp_avi_interfaces_firmware_update_md_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_firmware_info_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_firmware_update_request_get) != ZWP_STATUS_OK))
	{
		goto l_err_interface_list_get;
	}

	return ZWP_STATUS_OK;

l_err_interface_list_get:
	return ZWP_STATUS_ERROR;
}



