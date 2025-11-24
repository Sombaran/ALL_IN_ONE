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
 * @file    zwp_avi.c
 *
 * @brief   AVI message initialization
 * @details Initializes AVI messages
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-23
 * - Initial version
 */

#include "zwp_avi.h"
#include "zwp_avi_message_interfaces.h"
#include "zwp_memory.h"
#include "zwp_error.h"
#include "zwp_log.h"

/** AVI class */
struct _zwp_avi
{
	zwu_array_list_t            message_list_root;         /**< List of root level message handlers */
	zwu_array_list_t            message_list_ifttt;		   /**< List of ifttt level message handlers */
	zwu_array_list_t            message_list_networks;     /**< List of network level message handlers */
	zwu_array_list_t            message_list_nodes;        /**< List of node level message handlers */
	zwu_array_list_t            message_list_endpoints;    /**< List of endpoint level message handlers */
	zwu_array_list_t            message_list_interfaces;   /**< List of interface level message handlers */

	/*
	 * TODO: Scenes
	 */
	//zwp_array_list_t            message_list_scenes;

	zw_avro_t                   avro;                        /**< Avro object */
};

zwp_status_t zwp_avi_interfaces_alarm_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_association_grp_info_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_association_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_basic_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_battery_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_central_scene_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_configuration_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_door_lock_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_firmware_update_md_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_meter_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_sensor_binary_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_sensor_multilevel_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_switch_binary_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_switch_multilevel_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_switch_color_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_thermostat_fan_mode_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_thermostat_fan_state_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_thermostat_mode_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_thermostat_operating_state_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_thermostat_setpoint_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_user_code_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_barrier_operator_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_wake_up_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_zip_gateway_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_interfaces_zip_portal_init(zwu_array_list_t message_list);

zwp_status_t zwp_avi_endpoints_interface_list_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_endpoints_node_naming_init(zwu_array_list_t message_list);

zwp_status_t zwp_avi_nodes_endpoint_list_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_nodes_commandqueue_enable_init(zwu_array_list_t message_list);

zwp_status_t zwp_avi_scene_save_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_scene_get_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_scene_del_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_scene_exec_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_scene_updt_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_scene_get_state_log_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_scene_list_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_scene_get_sup_init(zwu_array_list_t message_list);

zwp_status_t zwp_avi_security_scene_list_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_security_scene_save_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_security_scene_get_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_security_scene_del_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_security_scene_set_state_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_security_scene_get_sup_init(zwu_array_list_t message_list);

zwp_status_t zwp_avi_networks_exit_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_networks_info_get_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_networks_node_list_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_networks_node_endpoint_list_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_networks_operations_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_networks_operation_state_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_networks_whitelist_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_networks_gateway_status_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_networks_summary_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_networks_version_init(zwu_array_list_t message_list);

#ifdef LOCAL_CLIENT_ENABLE
zwp_status_t zwp_avi_root_gateway_init(zwu_array_list_t message_list);
#else
zwp_status_t zwp_avi_root_network_list_init(zwu_array_list_t message_list);
zwp_status_t zwp_avi_root_network_refresh_init(zwu_array_list_t message_list);

zwp_status_t zwp_avi_ifttt_test_init(zwu_array_list_t network_message_list, zwu_array_list_t interface_message_list);
zwp_status_t zwp_avi_ifttt_bsensor_init(zwu_array_list_t network_message_list, zwu_array_list_t interface_message_list);
zwp_status_t zwp_avi_ifttt_bswitch_init(zwu_array_list_t network_message_list, zwu_array_list_t interface_message_list);
zwp_status_t zwp_avi_ifttt_dlock_init(zwu_array_list_t network_message_list, zwu_array_list_t interface_message_list);
zwp_status_t zwp_avi_ifttt_mswitch_init(zwu_array_list_t network_message_list, zwu_array_list_t interface_message_list);
zwp_status_t zwp_avi_ifttt_thrmomode_init(zwu_array_list_t network_message_list, zwu_array_list_t interface_message_list);

zwp_status_t zwp_avi_ifttt_alarm_init(zwu_array_list_t network_message_list, zwu_array_list_t interface_message_list);
zwp_status_t zwp_avi_ifttt_msensor_init(zwu_array_list_t network_message_list, zwu_array_list_t interface_message_list);

#endif

/**
 * @name Getters
 * @{
 */
zw_avro_t zwp_avi_get_avro(zwp_avi_t avi)
{
	return avi->avro;
}

zwu_array_list_t zwp_avi_get_message_list_root(zwp_avi_t avi)
{
	return avi->message_list_root;
}

zwu_array_list_t zwp_avi_get_message_list_ifttt(zwp_avi_t avi)
{
	return avi->message_list_ifttt;
}

zwu_array_list_t zwp_avi_get_message_list_networks(zwp_avi_t avi)
{
	return avi->message_list_networks;
}

zwu_array_list_t zwp_avi_get_message_list_nodes(zwp_avi_t avi)
{
	return avi->message_list_nodes;
}

zwu_array_list_t zwp_avi_get_message_list_endpoints(zwp_avi_t avi)
{
	return avi->message_list_endpoints;
}

zwu_array_list_t zwp_avi_get_message_list_interfaces(zwp_avi_t avi)
{
	return avi->message_list_interfaces;
}
/**
 * @}
 */

zwp_status_t zwp_avi_interfaces_setup(zwp_avi_t avi, zwifd_p desc_interface, zwp_pool_t pool)
{
	return zwp_avi_message_interfaces_setup(avi->message_list_interfaces, desc_interface, pool);
}

/**
 * @brief Initialize list of interface level message handlers
 * @param[in]     message_list      List of interface level message handlers
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_interfaces_alarm_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_association_grp_info_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_association_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_basic_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_battery_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_central_scene_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_configuration_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_door_lock_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_firmware_update_md_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_meter_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_sensor_binary_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_sensor_multilevel_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_switch_binary_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_switch_multilevel_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_switch_color_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_thermostat_fan_mode_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_thermostat_fan_state_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_thermostat_mode_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_thermostat_operating_state_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_thermostat_setpoint_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_user_code_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_barrier_operator_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_wake_up_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_zip_gateway_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_zip_portal_init(message_list) != ZWP_STATUS_OK)

			)
	{
		goto l_err_interfaces_init;
	}

	return ZWP_STATUS_OK;

l_err_interfaces_init:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Initialize list of endpoint level message handlers
 * @param[in]     message_list      List of endpoint level message handlers
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_endpoints_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_endpoints_interface_list_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_endpoints_node_naming_init(message_list) != ZWP_STATUS_OK))
	{
		goto l_err_endpoints_init;
	}

	return ZWP_STATUS_OK;

l_err_endpoints_init:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Initialize list of node level message handlers
 * @param[in]     message_list      List of node level message handlers
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_nodes_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_nodes_endpoint_list_init(message_list) != ZWP_STATUS_OK) ||
		(zwp_avi_nodes_commandqueue_enable_init(message_list) != ZWP_STATUS_OK)
	   )
	{
		goto l_err_nodes_init;
	}

	return ZWP_STATUS_OK;

l_err_nodes_init:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Initialize list of scene message handlers
 * @param[in]     message_list      List of scene message handlers
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_scenes_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_scene_save_init(message_list) != ZWP_STATUS_OK)
        || (zwp_avi_scene_get_init(message_list) != ZWP_STATUS_OK)
        || (zwp_avi_scene_del_init(message_list) != ZWP_STATUS_OK)
        || (zwp_avi_scene_exec_init(message_list) != ZWP_STATUS_OK)
        || (zwp_avi_scene_updt_init(message_list) != ZWP_STATUS_OK)
        || (zwp_avi_scene_get_state_log_init(message_list) != ZWP_STATUS_OK)
		|| (zwp_avi_scene_list_init(message_list) != ZWP_STATUS_OK)
		|| (zwp_avi_scene_get_sup_init(message_list) != ZWP_STATUS_OK)
		|| (zwp_avi_security_scene_list_init(message_list) != ZWP_STATUS_OK)
		|| (zwp_avi_security_scene_save_init(message_list) != ZWP_STATUS_OK)
		|| (zwp_avi_security_scene_get_init(message_list) != ZWP_STATUS_OK)
		|| (zwp_avi_security_scene_del_init(message_list) != ZWP_STATUS_OK)
		|| (zwp_avi_security_scene_set_state_init(message_list) != ZWP_STATUS_OK)
		|| (zwp_avi_security_scene_get_sup_init(message_list) != ZWP_STATUS_OK))
	{
		 goto l_err_scenes_init;
	}

	return ZWP_STATUS_OK;

l_err_scenes_init:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Initialize list of network level message handlers
 * @param[in]     message_list      List of network level message handlers
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_networks_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_networks_exit_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_networks_info_get_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_networks_node_list_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_networks_node_endpoint_list_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_networks_operations_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_networks_whitelist_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_networks_operation_state_init(message_list) != ZWP_STATUS_OK)
#ifndef LOCAL_CLIENT_ENABLE
			|| (zwp_avi_networks_gateway_status_init(message_list) != ZWP_STATUS_OK)
#endif
			|| (zwp_avi_networks_summary_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_networks_version_init(message_list) != ZWP_STATUS_OK)
			)
	{
		 goto l_err_networks_init;
	}

	return ZWP_STATUS_OK;

l_err_networks_init:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Initialize list of ifttt level message handlers
* @param[in]     message_list      List of ifttt level message handlers
* @retval        ZWP_STATUS_OK     Success
* @retval        ZWP_STATUS_ERROR  Failure
*/
static zwp_status_t zwp_avi_ifttt_init(zwu_array_list_t network_message_list, zwu_array_list_t interface_message_list)
{
#ifndef LOCAL_CLIENT_ENABLE
	if ((zwp_avi_ifttt_bsensor_init(network_message_list, interface_message_list) != ZWP_STATUS_OK) ||
		(zwp_avi_ifttt_bswitch_init(network_message_list, interface_message_list) != ZWP_STATUS_OK) ||
		(zwp_avi_ifttt_test_init(network_message_list, interface_message_list) != ZWP_STATUS_OK) ||
		(zwp_avi_ifttt_dlock_init(network_message_list, interface_message_list) != ZWP_STATUS_OK) ||
		(zwp_avi_ifttt_mswitch_init(network_message_list, interface_message_list) != ZWP_STATUS_OK) ||
		(zwp_avi_ifttt_thrmomode_init(network_message_list, interface_message_list) != ZWP_STATUS_OK) ||
		(zwp_avi_ifttt_alarm_init(network_message_list, interface_message_list) != ZWP_STATUS_OK) ||
		(zwp_avi_ifttt_msensor_init(network_message_list, interface_message_list) != ZWP_STATUS_OK)
		)
	{
		goto l_err_ifttt_init;
	}
#endif

	return ZWP_STATUS_OK;

#ifndef LOCAL_CLIENT_ENABLE
l_err_ifttt_init:
#endif

	return ZWP_STATUS_ERROR;
}

/**
 * @brief Initialize list of root level message handlers
 * @param[in]     message_list      List of root level message handlers
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_root_init(zwu_array_list_t message_list)
{
	if (
#ifdef LOCAL_CLIENT_ENABLE
			(zwp_avi_root_gateway_init(message_list) != ZWP_STATUS_OK)
#else
			(zwp_avi_root_network_list_init(message_list) != ZWP_STATUS_OK)
			|| (zwp_avi_root_network_refresh_init(message_list) != ZWP_STATUS_OK)
#endif
			)
	{
		 goto l_err_root_init;
	}

	return ZWP_STATUS_OK;

l_err_root_init:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_avi_init(zwp_avi_t *avi_p, char *config_dir)
{
	static zwp_avi_t g_avi;
	zwp_avi_t avi;

	//Enforce singleton
	if (g_avi)
	{
		ZWP_LOG(ZWP_LOG_WARNING, ZWP_MSG_SINGLETON_MULTIPLE_CALLS);
		*avi_p = g_avi;

		return ZWP_STATUS_OK;
	}

	if ((avi = (zwp_avi_t)ZWP_MALLOC(sizeof(*avi))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_avi_alloc;
	}

	if (zw_avro_init(&avi->avro, config_dir) != ZWP_STATUS_OK)
	{
		goto l_err_avro_init;
	}

	avi->message_list_root = NULL;
	avi->message_list_networks = NULL;
	avi->message_list_ifttt = NULL;
	avi->message_list_nodes = NULL;
	avi->message_list_endpoints = NULL;
	avi->message_list_interfaces = NULL;

	if ((zwp_avi_message_list_create(&avi->message_list_root) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_create(&avi->message_list_networks) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_create(&avi->message_list_ifttt) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_create(&avi->message_list_nodes) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_create(&avi->message_list_endpoints) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_create(&avi->message_list_interfaces) != ZWP_STATUS_OK))
	{
		goto l_err_message_list_create;
	}

	/*
	 * TODO: Scenes
	 */
	//zwp_avi_message_list_create(&avi->message_list_scenes)
	//IFTTT also init to network list

	if ((zwp_avi_root_init(avi->message_list_root) != ZWP_STATUS_OK)
			|| (zwp_avi_networks_init(avi->message_list_networks) != ZWP_STATUS_OK)
			|| (zwp_avi_ifttt_init(avi->message_list_networks, avi->message_list_interfaces) != ZWP_STATUS_OK)
			|| (zwp_avi_nodes_init(avi->message_list_nodes) != ZWP_STATUS_OK)
			|| (zwp_avi_endpoints_init(avi->message_list_endpoints) != ZWP_STATUS_OK)
			|| (zwp_avi_interfaces_init(avi->message_list_interfaces) != ZWP_STATUS_OK)
			|| (zwp_avi_scenes_init(avi->message_list_networks) != ZWP_STATUS_OK)) //NOTE: Scenes use networks list as Scenes commands are network level commands
	{
		goto l_err_message_list_init;
	}

	/*
	 * TODO: Scenes
	 * zwp_avi_scenes_init()
	 */

	*avi_p = avi;

	return ZWP_STATUS_OK;

l_err_message_list_init:
	/*
	 * TODO: Scenes
	 */
	//zwp_avi_message_list_destroy(avi->message_list_scenes);

	zwp_avi_message_list_destroy(avi->message_list_interfaces);
	zwp_avi_message_list_destroy(avi->message_list_endpoints);
	zwp_avi_message_list_destroy(avi->message_list_nodes);
	zwp_avi_message_list_destroy(avi->message_list_ifttt);
	zwp_avi_message_list_destroy(avi->message_list_networks);
	zwp_avi_message_list_destroy(avi->message_list_root);
l_err_message_list_create:
	zw_avro_exit(avi->avro);
l_err_avro_init:
	ZWP_FREE(avi);
l_err_avi_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_avi_exit(zwp_avi_t avi)
{
	/*
	 * TODO: Scenes
	 */
	//zwp_avi_message_list_destroy(avi->message_list_scenes);

	zwp_avi_message_list_destroy(avi->message_list_interfaces);
	zwp_avi_message_list_destroy(avi->message_list_endpoints);
	zwp_avi_message_list_destroy(avi->message_list_nodes);
	zwp_avi_message_list_destroy(avi->message_list_ifttt);
	zwp_avi_message_list_destroy(avi->message_list_networks);
	zwp_avi_message_list_destroy(avi->message_list_root);

	zw_avro_exit(avi->avro);
	ZWP_FREE(avi);
}
