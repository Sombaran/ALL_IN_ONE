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
 * @file    zws_avi_interfaces_firmware_update_md.c
 *
 * @brief   AVI message 'interface_firmware_xxx'
 * @details Handles the AVI message 'interface_firmware_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-02-26
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message_interfaces.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zwu_number.h"
#include "zwu_encoding.h"

/** Firmware Info Get */
#define COMMAND_FIRMWARE_INFO_GET                1
/** Firmware Info Report */
#define COMMAND_FIRMWARE_INFO_REPORT             2
/** Firmware Update Reqiest Get */
#define COMMAND_FIRMWARE_UPDATE_REQUEST_GET      3
/** Firmware Update Reqiest Report */
#define COMMAND_FIRMWARE_UPDATE_REQUEST_REPORT   4

/** Manufacturer not specified */
#define COMMAND_MANUFACTURER_ID_NONE             (-1)
/** Firmware target not specified */
#define COMMAND_FIRMWARE_TARGET_NONE             (-1)
/** Firmware ID not specified */
#define COMMAND_FIRMWARE_ID_NONE                 (-1)
/** Firmware file name not specified */
#define COMMAND_FIRMWARE_FILE_NAME_NONE          NULL
/** Hardware version not specified */
#define COMMAND_HARDWARE_VERSION                   (-1)

/**
 * @brief Get CGI response for Info Get command from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_firmware_info_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int64_t utime;
	int32_t manufacturer_id;
	int32_t firmware_0_id;
	int32_t firmware_0_checksum;
	int32_t is_firmware_upgradable;
	int32_t max_fragment_size;
	int32_t is_fixed_frgament_size;
	int32_t hardware_version_valid;
	int32_t hardware_version;
	avro_value_t firmware_id_list_value;
	avro_value_t firmware_id_value;
	size_t firmware_id_count;
	int32_t firmware_id;
	zwu_string_buffer_t firmware_id_list;
	int i;
#ifdef LOCAL_CLIENT_ENABLE
	char *fw_dir_name = NULL;
#endif

	if ((zw_avro_field_get_long(arg_value, "utime", &utime) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "manufacturer_id", &manufacturer_id) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "firmware_0_id", &firmware_0_id) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "firmware_0_checksum", &firmware_0_checksum) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "is_firmware_upgradable", &is_firmware_upgradable) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "max_fragment_size", &max_fragment_size) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "is_fixed_frgament_size", &is_fixed_frgament_size) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "hardware_version_valid", &hardware_version_valid) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "hardware_version", &hardware_version) != ZWS_STATUS_OK)
			)
	{
		goto l_err_get_fields;
	}

#ifdef LOCAL_CLIENT_ENABLE
	if (zw_avro_field_get_string(arg_value, "firmware_directory_location", &fw_dir_name) != ZWS_STATUS_OK)
	{
		goto l_err_get_firmware_directory_location_field;
	}
#endif

	if (avro_value_get_by_name(arg_value, "firmware_n_id_list", &firmware_id_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'firmware_n_id_list': %s", avro_strerror());
		goto l_err_get_type_list_value;
	}

	if (avro_value_get_size(&firmware_id_list_value, &firmware_id_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get firmware value list size");
		goto l_err_firmware_id_list_size;
	}

	if (zwu_string_buffer_create(0, &firmware_id_list) != ZWS_STATUS_OK)
	{
		goto l_err_firmware_id_list_create;
	}

	for (i = 0; i < firmware_id_count; i++)
	{
		if (avro_value_get_by_index(&firmware_id_list_value, i, &firmware_id_value, NULL) != 0)
		{
			goto l_err_get_firmware_id_value;
		}

		if (avro_value_get_int(&firmware_id_value, &firmware_id) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get firmware id: %s", avro_strerror());
			goto l_err_get_firmware_id;
		}

		if (zwu_string_buffer_append_format(firmware_id_list, "<%s target=\"%u\" id=\"%u\" />",
				"fw_id",
				(i + 1),
				firmware_id
				) != ZWS_STATUS_OK)
		{
			goto l_err_firmware_id_append;
		}
	}

	if (zwu_string_buffer_append_format(result,
			"<%s %s=\"%llu\" vid=\"%u\" zw_fw_id=\"%u\" chksum=\"%u\" max_frag_sz=\"%u\" "
			"fixed_frag_sz=\"%u\" upgrade_flg=\"%u\" hw_ver_valid=\"%u\" hw_ver=\"%u\" app_data_dir=\"%s\">%s</%s>",
			"fw_info",
			"utime",
			(uint64_t)utime,
			(uint16_t)manufacturer_id,
			(uint16_t)firmware_0_id,
			(uint16_t)firmware_0_checksum,
			(uint16_t)max_fragment_size,
			(uint8_t)is_fixed_frgament_size,
			(uint8_t)is_firmware_upgradable,
			(uint8_t)hardware_version_valid,
			(uint8_t)hardware_version,
#ifdef LOCAL_CLIENT_ENABLE
			fw_dir_name,
#else
			// Security: Refrain from exposing server directory structure.
			"",
#endif
			zwu_string_buffer_get_string(firmware_id_list),
			"fw_info"
			) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}
#ifdef LOCAL_CLIENT_ENABLE
	ZWS_FREE(fw_dir_name);
#endif
	zwu_string_buffer_destroy(firmware_id_list);

	return ZWS_STATUS_OK;

l_err_append:
l_err_firmware_id_append:
l_err_get_firmware_id:
l_err_get_firmware_id_value:
	zwu_string_buffer_destroy(firmware_id_list);
l_err_firmware_id_list_create:
l_err_firmware_id_list_size:
l_err_get_type_list_value:
#ifdef LOCAL_CLIENT_ENABLE
	ZWS_FREE(fw_dir_name);
l_err_get_firmware_directory_location_field:
#endif
l_err_get_fields:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Set Avro value for RPC request for Update Request Get command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_firmware_update_request_get_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int manufacturer_id;
	int firmware_target;
	int firmware_id;
	char *firmware_file_name;
	size_t firmware_file_name_size;
	int hardware_version;
	char *COMMAND_FIRMWARE_FILE_NAME_EMPTY = "\0";

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	manufacturer_id = COMMAND_MANUFACTURER_ID_NONE;
	firmware_target = COMMAND_FIRMWARE_TARGET_NONE;
	firmware_id = COMMAND_FIRMWARE_ID_NONE;
	firmware_file_name = COMMAND_FIRMWARE_FILE_NAME_NONE;
	hardware_version = COMMAND_HARDWARE_VERSION;

	if ((i < args_count) && !strcmp(args[i][0], "vid"))
	{
		if (zwu_parse_int32_non_negative(args[i][1], &manufacturer_id) != ZWU_STATUS_OK)
		{
			goto l_err_manufacturer_id;
		}
		i++;
	}

	if ((i < args_count) && !strcmp(args[i][0], "fw_target"))
	{
		if (zwu_parse_int32_non_negative(args[i][1], &firmware_target) != ZWU_STATUS_OK)
		{
			goto l_err_firmware_target;
		}
		i++;
	}

	if ((i < args_count) && !strcmp(args[i][0], "fw_id"))
	{
		if (zwu_parse_int32_non_negative(args[i][1], &firmware_id) != ZWU_STATUS_OK)
		{
			goto l_err_firmware_id;
		}
		i++;
	}

	if ((i < args_count) && !strcmp(args[i][0], "file_name"))
	{
		if (!strcmp(args[i][1], COMMAND_FIRMWARE_FILE_NAME_EMPTY))
		{
			ZWS_LOG(ZWS_LOG_DEBUG, "Firmware file name is empty");
			goto l_err_firmware_file_name;
		}

		firmware_file_name_size = strlen(args[i][1]) + 1;

		if ((firmware_file_name = (char *)ZWS_MALLOC(firmware_file_name_size)) == NULL)
		{
			ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
			goto l_err_firmware_file_name_alloc;
		}

		strncpy(firmware_file_name, args[i][1], firmware_file_name_size);

		if (zwu_uri_decode_string(firmware_file_name, firmware_file_name_size, firmware_file_name) != ZWS_STATUS_OK)
		{
			goto l_err_decode_firmware_file_name;
		}

		i++;
	}


	if ((i < args_count) && !strcmp(args[i][0], "hw_ver"))
	{
		if (zwu_parse_int32_non_negative(args[i][1], &hardware_version) != ZWU_STATUS_OK)
		{
			goto l_err_hardware_version;
		}
		i++;
	}

	if (zw_avro_field_set_int(arg_value_p, "manufacturer_id", manufacturer_id) == ZWS_STATUS_OK)
	{
		if (zw_avro_field_set_int(arg_value_p, "firmware_target", firmware_target) == ZWS_STATUS_OK)
		{
			if (zw_avro_field_set_int(arg_value_p, "firmware_id", firmware_id) == ZWS_STATUS_OK)
			{
				if (firmware_file_name == COMMAND_FIRMWARE_FILE_NAME_NONE)
				{
					firmware_file_name = COMMAND_FIRMWARE_FILE_NAME_EMPTY;
				}

				if (zw_avro_field_set_string(arg_value_p, "firmware_file_name", firmware_file_name) == ZWS_STATUS_OK)
				{
					if (zw_avro_field_set_int(arg_value_p, "hardware_version", hardware_version) == ZWS_STATUS_OK)
					{
						return_status = ZWS_STATUS_OK;
					}
				}
			}
		}
	}

	if ((firmware_file_name != COMMAND_FIRMWARE_FILE_NAME_NONE)
			&& (firmware_file_name != COMMAND_FIRMWARE_FILE_NAME_EMPTY))
	{
		ZWS_FREE(firmware_file_name);
	}

	return return_status;

l_err_hardware_version:
l_err_decode_firmware_file_name:
	if ((firmware_file_name != COMMAND_FIRMWARE_FILE_NAME_NONE)
			&& (firmware_file_name != COMMAND_FIRMWARE_FILE_NAME_EMPTY))
	{
		ZWS_FREE(firmware_file_name);
	}
l_err_firmware_file_name_alloc:
l_err_firmware_file_name:
l_err_firmware_id:
l_err_firmware_target:
l_err_manufacturer_id:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Get CGI response for Update Request Get command from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_firmware_update_request_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	avro_value_t update_status_list_value;
	avro_value_t update_status_value;
	size_t update_status_count;
	int i;
	int32_t manufacturer_id;
	int32_t firmware_target;
	int32_t firmware_id;
	int64_t update_request_time;
	int64_t update_request_status_utime;
	int32_t update_request_status;
	int64_t update_completion_status_utime;
	int32_t update_completion_status;
	int32_t wait_time;
	int32_t wait_time_is_valid;
	int64_t target_restart_status_utime;
	int32_t target_restart_status;

	if (avro_value_get_by_name(arg_value, "update_status_list", &update_status_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'update_status_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (avro_value_get_size(&update_status_list_value, &update_status_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get firmware value list size");
		goto l_err_firmware_value_list_size;
	}

	// FIXME: Workaround: Dummy entry to satisfy Engg UI which expects at least one record.
	// Remove after fixing in Engg UI
	if (update_status_count == 0)
	{
		if (zwu_string_buffer_append_format(result,
				"<%s vid=\"0\" fw_tgt=\"0\" fw_id=\"0\" req_time=\"0\" req_status_utime=\"0\" req_status=\"0\" "
				"cmplt_status_utime=\"0\" cmplt_status=\"0\" wait_time=\"0\" wait_time_valid=\"0\" "
				"restart_status_utime=\"0\" restart_status=\"0\" />",
				"fw_update"
				) != ZWS_STATUS_OK)
		{
			goto l_err_append;
		}
	}

	for (i = 0; i < update_status_count; i++)
	{
		if (avro_value_get_by_index(&update_status_list_value, i, &update_status_value, NULL) != 0)
		{
			goto l_err_get_endpoint_value;
		}

		if ((zw_avro_field_get_int(&update_status_value, "manufacturer_id", &manufacturer_id) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&update_status_value, "firmware_target", &firmware_target) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&update_status_value, "firmware_id", &firmware_id) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_long(&update_status_value, "update_request_time", &update_request_time) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_long(&update_status_value, "update_request_status_utime", &update_request_status_utime) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&update_status_value, "update_request_status", &update_request_status) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_long(&update_status_value, "update_completion_status_utime", &update_completion_status_utime) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&update_status_value, "update_completion_status", &update_completion_status) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&update_status_value, "wait_time", &wait_time) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&update_status_value, "wait_time_is_valid", &wait_time_is_valid) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_long(&update_status_value, "target_restart_status_utime", &target_restart_status_utime) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&update_status_value, "target_restart_status", &target_restart_status) != ZWS_STATUS_OK)
				)
		{
			goto l_err_get_fields;
		}

		if (zwu_string_buffer_append_format(result,
				"<%s vid=\"%u\" fw_tgt=\"%u\" fw_id=\"%u\" req_time=\"%llu\" req_status_utime=\"%llu\" req_status=\"%u\" "
				"cmplt_status_utime=\"%llu\" cmplt_status=\"%u\" wait_time=\"%u\" wait_time_valid=\"%d\" "
				"restart_status_utime=\"%llu\" restart_status=\"%u\" />",
				"fw_update",
				(uint16_t)manufacturer_id,
				(uint8_t)firmware_target,
				(uint16_t)firmware_id,
				(uint64_t)update_request_time,
				(uint64_t)update_request_status_utime,
				(uint8_t)update_request_status,
				(uint64_t)update_completion_status_utime,
				(uint8_t)update_completion_status,
				(uint16_t)wait_time,
				wait_time_is_valid,
				(uint64_t)target_restart_status_utime,
				(uint8_t)target_restart_status
				) != ZWS_STATUS_OK)
		{
			goto l_err_append;
		}
	}

	return ZWS_STATUS_OK;

l_err_append:
l_err_get_fields:
l_err_get_endpoint_value:
l_err_firmware_value_list_size:
l_err_get_list_value:
	return ZWS_STATUS_ERROR;
}

/** Handlers for Info Get command */
static zws_avi_message_interfaces_t interfaces_firmware_info_get =
{
		{"zwif_fw", "firmware"}, COMMAND_FIRMWARE_INFO_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_NOT_SUPPORTED,

		"interface_firmware_info_get",

		NULL,
		zws_avi_interfaces_firmware_info_get_result,

		"tServFirmware"
};

/** Handlers for Update Request Get command */
static zws_avi_message_interfaces_t interfaces_firmware_update_request_get =
{
		{"zwif_fw", "firmware"}, COMMAND_FIRMWARE_UPDATE_REQUEST_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_NOT_SUPPORTED,

		"interface_firmware_update_request_get",

		zws_avi_interfaces_firmware_update_request_get_argument,
		zws_avi_interfaces_firmware_update_request_get_result,

		"tServFirmware"
};

/** Register command handlers */
zws_status_t zws_avi_interfaces_firmware_update_md_init(zwu_array_list_t message_list)
{
	if ((zws_avi_message_list_add_message_interfaces(message_list, &interfaces_firmware_info_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_firmware_update_request_get) != ZWS_STATUS_OK))
	{
		goto l_err_networks_list_get;
	}

	return ZWS_STATUS_OK;

l_err_networks_list_get:
	return ZWS_STATUS_ERROR;
}



