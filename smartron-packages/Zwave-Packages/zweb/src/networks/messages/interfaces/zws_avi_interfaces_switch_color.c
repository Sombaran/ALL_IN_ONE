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
 * @file    zws_avi_interfaces_switch_multilevel.c
 *
 * @brief   AVI message 'interface_switch_multilevel_xxx'
 * @details Handles the AVI message 'interface_switch_multilevel_xxx'.
 *
 * @author  Jason Zheng
 *
 * @version 1.0 - 2016-09-21
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message_interfaces.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zwu_number.h"

/** Switch Color Setup */
#define COMMAND_SWITCH_COLOR_SETUP              1
/** Switch Color Get */
#define COMMAND_SWITCH_COLOR_GET                2
/** Switch Color Report */
#define COMMAND_SWITCH_COLOR_REPORT             3
/** Switch Color Set */
#define COMMAND_SWITCH_COLOR_SET                4
/** Switch Color Level Change Get */
#define COMMAND_SWITCH_COLOR_LEVEL_CHANGE_GET   5
/** Switch Color Level Change Set */
#define COMMAND_SWITCH_COLOR_LEVEL_CHANGE_SET   6
/** Switch Color Supported Get */
#define COMMAND_SWITCH_COLOR_SUPPORTED_GET      7
/** Switch Color Supported Report */
#define COMMAND_SWITCH_COLOR_SUPPORTED_REPORT   8

/** Switch Color Start Level not specified */
#define COMMAND_SWITCH_COLOR_START_LEVEL_NONE              (-1)
/** Switch Color Dimming Duration not specified */
#define COMMAND_SWITCH_COLOR_DURATION_NONE                 (-1)

#define MAX_NUM_OF_COLORS 		9

/**
 * @brief Set Avro value for RPC request for Get command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_switch_color_get_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int mask;
	return_status = ZWS_STATUS_ERROR;
	mask=0;

	if ((args_count>0) && !strcmp(args[0][0], "color_id_list"))
	{
		char *entry;
		char *next_entry;
		int entry_count;
		int id;
		
		entry = args[0][1];
		entry_count = 0;
		
		while (entry && entry[0] && entry_count<MAX_NUM_OF_COLORS)
		{			
			if ((next_entry = strstr(entry, ",")))
			{
				*next_entry = '\0';
				next_entry++;
			}
			
			entry_count++;
			
			if (zwu_parse_int32_non_negative(entry, &id) != ZWU_STATUS_OK)
			{
				goto l_err_get_arg_err_value;
			}
			
			mask|=(1<<id);

			// Iterate to next entry
			entry = next_entry;
		}
	}
	else if ((args_count>0) && !strcmp(args[0][0], "color_id"))
	{
		int id;
		if (zwu_parse_int32_non_negative(args[0][1], &id) != ZWU_STATUS_OK)
		{
			goto l_err_get_arg_err_value;
		}		
		mask|=(1<<id);
	}
	else
	{
		mask = 0x1FF;
	}
	
	if ((zw_avro_field_set_int(arg_value_p, "mask", mask) == ZWS_STATUS_OK))
	{
		return_status = ZWS_STATUS_OK;
	}
	
l_err_get_arg_err_value:
	return return_status;
}

/**
 * @brief Get CGI response for Get command from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_switch_color_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	avro_value_t avro_list;
	avro_value_t avro_value;
	size_t avro_value_count;
	int i;
	int64_t utime;
	int32_t id;
	int32_t cvalue;
	int32_t tvalue;
	int32_t dur;

	if (avro_value_get_by_name(arg_value, "switch_color_value_list", &avro_list, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'switch_color_value_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (avro_value_get_size(&avro_list, &avro_value_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get switch_color value list size");
		goto l_err_value_list_size;
	}

	for (i = 0; i < avro_value_count; i++)
	{
		if (avro_value_get_by_index(&avro_list, i, &avro_value, NULL) != 0)
		{
			goto l_err_get_endpoint_value;
		}
		
		if ((zw_avro_field_get_long(&avro_value, "utime", &utime) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&avro_value, "id", &id) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&avro_value, "cvalue", &cvalue) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&avro_value, "tvalue", &tvalue) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&avro_value, "dur", &dur) != ZWS_STATUS_OK)
				)
		{
			goto l_err_get_fields;
		}
		
		if (tvalue == (-1) || dur == (-1))
		{
			if (zwu_string_buffer_append_format(result,
					"<color %s=\"%llu\" color_id=\"%u\" color_cvalue=\"%u\" />",
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
					(uint64_t)utime,
					(uint8_t)id,
					(uint8_t)cvalue
					) != ZWS_STATUS_OK)
			{
				goto l_err_append;
			}
		}
		else
		{
			if (zwu_string_buffer_append_format(result,
					"<color %s=\"%llu\" color_id=\"%u\" color_cvalue=\"%u\" color_tvalue=\"%u\" color_dur=\"%u\" />",
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
					(uint64_t)utime,
					(uint8_t)id,
					(uint8_t)cvalue,
					(uint8_t)tvalue,
					(uint8_t)dur
					) != ZWS_STATUS_OK)
			{
				goto l_err_append;
			}
		}		
	}

	return ZWS_STATUS_OK;

l_err_append:
l_err_get_fields:
l_err_get_endpoint_value:
l_err_value_list_size:
l_err_get_list_value:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Set Avro value for RPC request for Set command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_switch_color_set_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	int id[MAX_NUM_OF_COLORS];
	int value[MAX_NUM_OF_COLORS];
	int duration;
	int id_entry_count;
	int val_entry_count;

	id_entry_count = -1;
	val_entry_count = -1;

	if ((args_count>0) && !strcmp(args[0][0], "color_id_list"))
	{
		char *entry;
		char *next_entry;
		int entry_count;
		
		entry = args[0][1];
		entry_count = 0;
		
		while (entry && entry[0] && entry_count<MAX_NUM_OF_COLORS)
		{
			if ((next_entry = strstr(entry, ",")))
			{
				*next_entry = '\0';
				next_entry++;
			}	
		
			entry_count++;
			
			if (zwu_parse_int32_non_negative(entry, &id[entry_count-1]) != ZWU_STATUS_OK) 
			{
				return ZWS_STATUS_ERROR;
			}

			// Iterate to next entry
			entry = next_entry;
		}
		id_entry_count = entry_count;
	}
	else
	{
		return ZWS_STATUS_ERROR;
	}
		
	if ((args_count>1) && !strcmp(args[1][0], "color_value_list"))
	{	
		char *entry;
		char *next_entry;
		int entry_count;
		
		entry = args[1][1];
		entry_count = 0;
		
		while (entry && entry[0] && entry_count<MAX_NUM_OF_COLORS)
		{			
			if ((next_entry = strstr(entry, ",")))
			{
				*next_entry = '\0';
				next_entry++;
			}	
		
			entry_count++;
			
			if (zwu_parse_int32_non_negative(entry, &value[entry_count-1]) != ZWU_STATUS_OK) 
			{
				return ZWS_STATUS_ERROR;
			}
			
			// Iterate to next entry
			entry = next_entry;
		}
		val_entry_count = entry_count;
	}	
	else
	{
		return ZWS_STATUS_ERROR;
	}

	if (id_entry_count == -1 || val_entry_count == -1 || id_entry_count != val_entry_count)
	{
		return ZWS_STATUS_ERROR;
	}
	
	duration = COMMAND_SWITCH_COLOR_DURATION_NONE;
	if ((args_count>2) && !strcmp(args[2][0], "color_dur"))
	{
		if (zwu_parse_int32_non_negative(args[2][1], &duration) != ZWU_STATUS_OK) 
		{
			return ZWS_STATUS_ERROR;
		}
	}

	if ((zw_avro_field_set_int(arg_value_p, "dur", duration) != ZWS_STATUS_OK))
	{
		return ZWS_STATUS_ERROR;
	}
	
	avro_value_t avro_list;
	avro_value_t avro_value;	
	
	if (avro_value_get_by_name(arg_value_p, "switch_color_set_list", &avro_list, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'switch_color_set_list': %s", avro_strerror());
		return ZWS_STATUS_ERROR;
	}	
	
	int i;
	for (i = 0; i < id_entry_count; i++) 
	{
		if (avro_value_append(&avro_list, &avro_value, 0) != 0)
		{
			return ZWS_STATUS_ERROR;
		}

		if ((zw_avro_field_set_int(&avro_value, "id", id[i]) != ZWS_STATUS_OK)
				|| (zw_avro_field_set_int(&avro_value, "value", value[i]) != ZWS_STATUS_OK)
				)
		{
			return ZWS_STATUS_ERROR;
		}
	}	

	return ZWS_STATUS_OK;
}

/**
 * @brief Set Avro value for RPC request for Level Change Get command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */ 
static zws_status_t zws_avi_interfaces_switch_color_level_change_get_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int mask;
	return_status = ZWS_STATUS_ERROR;
	mask=0;

	if ((args_count>0) && !strcmp(args[0][0], "color_id_list"))
	{
		char *entry;
		char *next_entry;
		int entry_count;
		int id;
		
		entry = args[0][1];
		entry_count = 0;
		
		while (entry && entry[0] && entry_count<MAX_NUM_OF_COLORS)
		{			
			if ((next_entry = strstr(entry, ",")))
			{
				*next_entry = '\0';
				next_entry++;
			}
			
			entry_count++;
			
			if (zwu_parse_int32_non_negative(entry, &id) != ZWU_STATUS_OK)
			{
				goto l_err_get_arg_err_value;
			}
			
			mask|=(1<<id);
			
			// Iterate to next entry
			entry = next_entry;
		}
	}
	else
	{
		mask = 0x1FF;
	}
	
	if ((zw_avro_field_set_int(arg_value_p, "mask", mask) == ZWS_STATUS_OK))
	{
		return_status = ZWS_STATUS_OK;
	}
	
l_err_get_arg_err_value:
	return return_status;	
}

/**
 * @brief Get CGI response for Level Change Get command from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_switch_color_level_change_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{	
	avro_value_t avro_list;
	avro_value_t avro_value;
	size_t avro_value_count;
	int i;
	int64_t utime;
	int32_t id;
	int32_t start_stop;

	if (avro_value_get_by_name(arg_value, "switch_color_level_change_value_list", &avro_list, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'switch_color_level_change_value_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (avro_value_get_size(&avro_list, &avro_value_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get switch_color_level_change_value_list size");
		goto l_err_value_list_size;
	}

	for (i = 0; i < avro_value_count; i++)
	{
		if (avro_value_get_by_index(&avro_list, i, &avro_value, NULL) != 0)
		{
			goto l_err_get_endpoint_value;
		}

		if ((zw_avro_field_get_long(&avro_value, "utime", &utime) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&avro_value, "id", &id) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&avro_value, "start_stop", &start_stop) != ZWS_STATUS_OK)
				)
		{
			goto l_err_get_fields;
		}		

		if (zwu_string_buffer_append_format(result,
				"<color_change color_id=\"%u\" status=\"%u\" />",
				(uint8_t)id,
				(uint8_t)start_stop
				) != ZWS_STATUS_OK)
		{
			goto l_err_append;
		}				
	}

	return ZWS_STATUS_OK;

l_err_append:
l_err_get_fields:
l_err_get_endpoint_value:
l_err_value_list_size:
l_err_get_list_value:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Set Avro value for RPC request for Level Change Set command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */ 
static zws_status_t zws_avi_interfaces_switch_color_level_change_set_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;
	int start_stop;
	int up_down;
	int ignore_start_level;
	int id;
	int start_level;
	int duration;

	return_status = ZWS_STATUS_ERROR;
	i = 0;
	
	if ((i < args_count) && !strcmp(args[i][0], "color_id")
			&& (zwu_parse_int32_non_negative(args[i][1], &id) == ZWU_STATUS_OK)
			&& (zw_avro_field_set_int(arg_value_p, "id", id) == ZWS_STATUS_OK))
	{
		i++;

		if ((i < args_count) && !strcmp(args[i][0], "color_start_stop")
				&& (zwu_parse_int32_non_negative(args[i][1], &start_stop) == ZWU_STATUS_OK)
				&& (start_stop <= 1)
				&& (zw_avro_field_set_int(arg_value_p, "start_stop", start_stop) == ZWS_STATUS_OK))
		{
			i++;

			if (start_stop == 1)
			{
				if ((i < args_count) && !strcmp(args[i][0], "color_dir")
						&& (zwu_parse_int32_non_negative(args[i][1], &up_down) == ZWU_STATUS_OK)
						&& (up_down <= 1)
						&& (zw_avro_field_set_int(arg_value_p, "up_down", up_down) == ZWS_STATUS_OK))
				{
					i++;

					if ((i < args_count) && !strcmp(args[i][0], "color_ignore_start")
							&& (zwu_parse_int32_non_negative(args[i][1], &ignore_start_level) == ZWU_STATUS_OK)
							&& (ignore_start_level <= 1)
							&& (zw_avro_field_set_int(arg_value_p, "ignore_start_level", ignore_start_level) == ZWS_STATUS_OK))
					{
						i++;

						start_level = COMMAND_SWITCH_COLOR_START_LEVEL_NONE;
						if ((i < args_count) && !strcmp(args[i][0], "color_start_lvl")
								&& (zwu_parse_int32_non_negative(args[i][1], &start_level) == ZWU_STATUS_OK)
								)
						{
							i++;

							if (ignore_start_level == 0)
							{
								return_status = ZWS_STATUS_OK;
							}
						}

						if (ignore_start_level != 0)
						{
							return_status = ZWS_STATUS_OK;
						}

						duration = COMMAND_SWITCH_COLOR_DURATION_NONE;
						if ((i < args_count) && !strcmp(args[i][0], "color_dur"))
						{
							if (zwu_parse_int32_non_negative(args[i][1], &duration) != ZWU_STATUS_OK)
							{
								return_status = ZWS_STATUS_ERROR;
								goto l_err_duration;
							}
							i++;
						}

						if ((zw_avro_field_set_int(arg_value_p, "start_level", start_level) != ZWS_STATUS_OK)
								|| (zw_avro_field_set_int(arg_value_p, "duration", duration) != ZWS_STATUS_OK))
						{
							return_status = ZWS_STATUS_ERROR;
							goto l_err_set_value;
						}
					}
				}
			}		
			else if (start_stop == 0)
			{
				return_status = ZWS_STATUS_OK;
			}
		}		
	}

l_err_set_value:
l_err_duration:
	return return_status;
}

/**
 * @brief Get CGI response for Supported Get command from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_switch_color_supported_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int64_t utime;
	int32_t mask;

	if ((zw_avro_field_get_long(arg_value, "utime", &utime) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(arg_value, "mask", &mask) != ZWS_STATUS_OK))
	{
		return ZWS_STATUS_ERROR;
	}

	zwu_string_buffer_t sup_list;
	if (zwu_string_buffer_create(0, &sup_list) != ZWS_STATUS_OK)
	{
		return ZWS_STATUS_ERROR;
	}
	
	int temp_mask;
	temp_mask = 0;
	int i;
	for (i = 0; i < sizeof(mask)*8; i++)
	{
		temp_mask += (1<<i);
		if (mask & (1<<i))
		{
			if (mask & (~temp_mask))
			{
				if (zwu_string_buffer_append_format(sup_list, "%u,", i) != ZWS_STATUS_OK)
				{
					zwu_string_buffer_destroy(sup_list);
					return ZWS_STATUS_ERROR;
				}
			}
			else
			{
				if (zwu_string_buffer_append_format(sup_list, "%u", i) != ZWS_STATUS_OK)
				{
					zwu_string_buffer_destroy(sup_list);
					return ZWS_STATUS_ERROR;
				}
			}
		}
	}

	if (zwu_string_buffer_append_format(result,
			"<color_sup %s=\"%llu\" color_sup_list=\"%s\" />",
			((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),	
			(uint64_t)utime,
			zwu_string_buffer_get_string(sup_list)
			) != ZWS_STATUS_OK)
	{
		zwu_string_buffer_destroy(sup_list);
		return ZWS_STATUS_ERROR;
	}

	zwu_string_buffer_destroy(sup_list);
	return ZWS_STATUS_OK;
}

/** Handlers for Get command */
static zws_avi_message_interfaces_t interfaces_switch_color_get =
{
		{"zwif_color", "color_sw"}, COMMAND_SWITCH_COLOR_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,
		"interface_switch_color_get",
		zws_avi_interfaces_switch_color_get_argument,
		zws_avi_interfaces_switch_color_get_result,
		"tServColorSw"
};

/** Handlers for Set command */
static zws_avi_message_interfaces_t interfaces_switch_color_set =
{
		{"zwif_color", "color_sw"}, COMMAND_SWITCH_COLOR_SET,
		ZWS_AVI_MESSAGE_PASSIVE_NOT_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,
		"interface_switch_color_set",
		zws_avi_interfaces_switch_color_set_argument,
		NULL,
		"tServColorSw"
};

/** Handlers for Level Change Get command */
static zws_avi_message_interfaces_t interfaces_switch_color_level_change_get =
{
		{"zwif_color", "color_sw"}, COMMAND_SWITCH_COLOR_LEVEL_CHANGE_GET,
		ZWS_AVI_MESSAGE_PASSIVE_NOT_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,
		"interface_switch_color_level_change_get",
		zws_avi_interfaces_switch_color_level_change_get_argument,
		zws_avi_interfaces_switch_color_level_change_get_result,
		"tServColorSw"
};

/** Handlers for Level Change Set command */
static zws_avi_message_interfaces_t interfaces_switch_color_level_change_set =
{
		{"zwif_color", "color_sw"}, COMMAND_SWITCH_COLOR_LEVEL_CHANGE_SET,
		ZWS_AVI_MESSAGE_PASSIVE_NOT_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,
		"interface_switch_color_level_change_set",
		zws_avi_interfaces_switch_color_level_change_set_argument,
		NULL,
		"tServColorSw"
};

/** Handlers for Supported Get command */
static zws_avi_message_interfaces_t interfaces_switch_color_supported_get =
{
		{"zwif_color", "color_sw"}, COMMAND_SWITCH_COLOR_SUPPORTED_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,
		"interface_switch_color_supported_get",
		NULL,
		zws_avi_interfaces_switch_color_supported_get_result,
		"tServColorSw"
};

/** Register command handlers */
zws_status_t zws_avi_interfaces_switch_color_init(zwu_array_list_t message_list)
{	
	if ((zws_avi_message_list_add_message_interfaces(message_list, &interfaces_switch_color_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_switch_color_set) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_switch_color_level_change_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_switch_color_level_change_set) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_switch_color_supported_get) != ZWS_STATUS_OK))
	{
		goto l_err_networks_list_get;
	}

	return ZWS_STATUS_OK;

l_err_networks_list_get:
	return ZWS_STATUS_ERROR;
}
