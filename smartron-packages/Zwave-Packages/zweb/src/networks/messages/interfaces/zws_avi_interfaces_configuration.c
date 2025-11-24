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
 * @file    zws_avi_interfaces_configuration.c
 *
 * @brief   AVI message 'interface_configuration_xxx'
 * @details Handles the AVI message 'interface_configuration_xxx'.
 *
 * @author  Sasidhar
 *
 * @version 1.0 - 2013-11-14
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message_interfaces.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"

/** Configuration Setup */
#define COMMAND_CONFIGURATION_SETUP              1
/** Configuration Get */
#define COMMAND_CONFIGURATION_GET                2
/** Configuration Report */
#define COMMAND_CONFIGURATION_REPORT             3
/** Configuration Set */
#define COMMAND_CONFIGURATION_SET                4

/**
 * @brief Set Avro value for RPC request for configuration get command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_configuration_get_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i;

	return_status = ZWS_STATUS_ERROR;
	i = 0;


	if ((i < args_count) && !strcmp(args[i][0], "param_num")
			&& (zw_avro_field_set_int(arg_value_p, "parameternumber", atoi(args[i][1])) == ZWS_STATUS_OK)
		)
	{
		i++;
		return_status = ZWS_STATUS_OK;
	}

	return return_status;
}



/**
 * @brief Get CGI response for configuration get command from Avro value for RPC response
 * @param[in]     web_api_version   Web API version
 * @param[in]     combo_id          Combo ID
 * @param[in]     arg_value         Avro value for RPC response
 * @param[out]    result            CGI response string
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_configuration_get_result(int web_api_version, unsigned long combo_id, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	int64_t utime;
	int32_t param, size, i, j, value = 0, integer;
	avro_value_t config_value_array;
	avro_value_t config_value;
	avro_value_t config_param_value_list;
	avro_value_t config_param_value;
	size_t configuration_value_count;


	if (avro_value_get_by_name(arg_value, "configuration_param_value_list", &config_param_value_list, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'configuration_param_value_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (avro_value_get_size(&config_param_value_list, &configuration_value_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get configuration value list size");
		goto l_err_configuration_value_list_size;
	}

	for (i = 0; i < configuration_value_count; i++)
	{
		if (avro_value_get_by_index(&config_param_value_list, i, &config_param_value, NULL) != 0)
		{
			goto l_err_get_config_param_value;
		}

		if ((zw_avro_field_get_long(&config_param_value, "utime", &utime) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&config_param_value, "parameternumber", &param) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&config_param_value, "size", &size) != ZWS_STATUS_OK)
			)
		{
			goto l_err_get_fields;
		}

		// Size validation to prevent underflow/overlfow
		if ((size != 1) && (size != 2)  && (size != 4))
		{
			ZWS_LOG(ZWS_LOG_ERR, "Invalid thermostat_setpoint value field size");
			goto l_err_invalid_configuration_value_size;
		}

		if (avro_value_get_by_name(&config_param_value, "configvalue_array", &config_value_array, NULL) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'configvalue_array': %s", avro_strerror());
			goto l_err_get_config_array;
		}

		for (j = 0; j < size; j++)
		{
			if (avro_value_get_by_index(&config_value_array, j, &config_value, NULL) != 0)
			{
				goto l_err_get_config_value;
			}

			if (avro_value_get_int(&config_value, &integer) != ZWS_STATUS_OK)
			{
				goto l_err_get_cofnig_field_value;
			}

			if(j==0)
			{
				value |=  (((int8_t)integer) << ((size-j-1)*8));
			}
			else
			{
				value |=  (((uint8_t)integer) << ((size-j-1)*8));
			}

		}

		if (zwu_string_buffer_append_format(result,
					"<config %s=\"%llu\" num=\"%u\" val=\"%d\" />",
					((web_api_version == ZWS_WEB_API_VERSION_1) ? "update" : "utime"),
					(uint64_t)utime,
					(uint8_t)param,
					value
					) != ZWS_STATUS_OK)
		{
			goto l_err_append;
		}
	}

	return ZWS_STATUS_OK;

l_err_append:
l_err_get_cofnig_field_value:
l_err_get_config_value:
l_err_get_config_array:
l_err_invalid_configuration_value_size:
l_err_get_fields:
l_err_get_config_param_value:
l_err_configuration_value_list_size:
l_err_get_list_value:

	return ZWS_STATUS_ERROR;
}

/**
 * @brief Set Avro value for RPC request for configuration Set command using CGI request arguments
 * @param[in]     args              CGI request arguments
 * @param[in]     args_count        Count of CGI request arguments
 * @param[in]     arg_value_p       Avro value for RPC request
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 */
static zws_status_t zws_avi_interfaces_configuration_set_argument(char *(*args)[2], int args_count, avro_value_t *arg_value_p)
{
	zws_status_t return_status;
	int i, j, use_def=0, param_val = 0, param_size=0;
	avro_value_t config_value_array, config_element;
	int config_val = 0;

	return_status = ZWS_STATUS_ERROR;
	i = 0;

	if ((i < args_count) && !strcmp(args[i][0], "param_num")
			&& (zw_avro_field_set_int(arg_value_p, "parameternumber", atoi(args[i][1])) == ZWS_STATUS_OK))
	{
		i++;
		if ((i < args_count) && !strcmp(args[i][0], "use_def")
					&& (zw_avro_field_set_int(arg_value_p, "default", atoi(args[i][1])) == ZWS_STATUS_OK))
		{
			use_def = atoi(args[i][1]);
			i++;

			if(use_def == 0)
			{
				if ((i < args_count) && !strcmp(args[i][0], "param_val"))
				{
					param_val = atoi(args[i][1]);
					i++;

					if ((i < args_count) && !strcmp(args[i][0], "param_size"))
					{
						param_size = atoi(args[i][1]);
						i++;

						if(param_size == 0)
						{
							// if not given, find the minimum size needed for the value
							param_size = ((param_val >= (int8_t)0x80) && (param_val <= (int8_t)0x7F))
									? 1 : (((param_val >= (int16_t)0x8000) && (param_val <= (int16_t)0x7FFF))
											? 2 : 4);
						}

						if (((param_size == 1) || (param_size == 2) || (param_size == 4))
							 && (zw_avro_field_set_int(arg_value_p, "size", param_size) == ZWS_STATUS_OK)
							)
						{

							if (avro_value_get_by_name(arg_value_p, "configvalue_array", &config_value_array, NULL) != 0)
							{
								ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'configvalue_array': %s", avro_strerror());
								goto l_err_get_config_array;
							}

							for(j = 0; j < param_size; j++)
							{
								if (avro_value_append(&config_value_array, &config_element, 0) != 0)
								{
									goto l_err_append_config_value_element;
								}

								config_val = (uint8_t)((param_val >> (8 * (param_size - 1 - j))) & 0xFF);

								if (avro_value_set_int(&config_element, config_val) != 0)
								{
									ZWS_LOG(ZWS_LOG_ERR, "Failed to set value '%d' to the field configvalue : %s", config_val, avro_strerror);
									goto l_err_set_value;
								}
							}

							return_status = ZWS_STATUS_OK;
						}

					}
				}

			}
			else
			{
				return_status = ZWS_STATUS_OK;
			}

		}
	}

	return return_status;

l_err_set_value:
l_err_append_config_value_element:
l_err_get_config_array:
	return return_status;
}

/** Handlers for Configuration Get Command */
static zws_avi_message_interfaces_t interfaces_configuration_get =
{
		{"zwif_config", "config"}, COMMAND_CONFIGURATION_GET,
		ZWS_AVI_MESSAGE_PASSIVE_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_configuration_get",

		zws_avi_interfaces_configuration_get_argument,
		zws_avi_interfaces_configuration_get_result,

		"tServConfig"
};

/** Handlers for Configuration Set Command */
static zws_avi_message_interfaces_t interfaces_configuration_set =
{
		{"zwif_config", "config"}, COMMAND_CONFIGURATION_SET,
		ZWS_AVI_MESSAGE_PASSIVE_NOT_SUPPORTED,
		ZWS_AVI_MESSAGE_SETUP_SUPPORTED,

		"interface_configuration_set",

		zws_avi_interfaces_configuration_set_argument,
		NULL,

		"tServConfig"
};

/** Register command handlers */
zws_status_t zws_avi_interfaces_configuration_init(zwu_array_list_t message_list)
{
	if ((zws_avi_message_list_add_message_interfaces(message_list, &interfaces_configuration_get) != ZWS_STATUS_OK)
			|| (zws_avi_message_list_add_message_interfaces(message_list, &interfaces_configuration_set) != ZWS_STATUS_OK))
	{
		goto l_err_networks_list_get;
	}

	return ZWS_STATUS_OK;

l_err_networks_list_get:
	return ZWS_STATUS_ERROR;
}
