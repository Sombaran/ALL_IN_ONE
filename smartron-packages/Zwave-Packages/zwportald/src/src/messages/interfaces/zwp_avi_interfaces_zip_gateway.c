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
 * @file    zwp_avi_interfaces_zip_gateway.c
 *
 * @brief   AVI message 'interface_zip_gateway_xxx'
 * @details Handles the AVI message 'interface_zip_gateway_xxx'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-10-07
 * - Initial version
 */

#include "zwp_avi_interfaces.h"

/** Portal mode */
#define PORTAL_MODE		0x2

/** Mode Set command state */
typedef struct {
	uint8_t            mode;                               /**< Mode */
	zwgw_portal_prof_t profile;                            /**< Portal profile */
} zwp_avi_interfaces_zip_gateway_mode_set_t;

/** Mode Report command state */
typedef struct {
	zwu_time_t         utime;                              /**< Last updated time of Mode Report command */
	uint8_t            status;                             /**< Status */
	uint8_t            mode;                               /**< Mode */
	uint8_t            profile_present;                    /**< Is portal profile present? */
	zwgw_portal_prof_t profile;                            /**< Portal profile */


	int                report_receive_state;               /**< Mode Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Mode Report command receive condition */
} zwp_avi_interfaces_zip_gateway_mode_report_t;

/** Lock Set command state */
typedef struct {
	uint8_t            lock;                               /**< Lock */
	uint8_t 		   show;                               /**< Show */
} zwp_avi_interfaces_zip_gateway_lock_set_t;

/** Unsolicited Destination Set command state */
typedef struct {
	uint8_t            unsolicited_ipv6_destination[16];   /**< Unsolicited destination IPv6 address */
	uint16_t           unsolicited_destination_port;       /**< Unsolicited destination port */
} zwp_avi_interfaces_zip_gateway_unsolicited_destination_set_t;

/** Unsolicited Destination Report command state */
typedef struct {
	zwu_time_t         utime;                              /**< Last updated time of Unsolicited Destination Report command */
	uint8_t            unsolicited_ipv6_destination[16];   /**< Unsolicited destination IPv6 address */
	uint16_t           unsolicited_destination_port;       /**< Unsolicited destination port */
	uint8_t            local_ipv6_address[16];             /**< Local IPv6 address */
	uint16_t           local_destination_port;             /**< Local destination port */

	int                report_receive_state;               /**< Unsolicited Destination Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Unsolicited Destination Report command receive condition */
} zwp_avi_interfaces_zip_gateway_unsolicited_destination_report_t;

/** Interface level state */
typedef struct {
	zwp_avi_interfaces_zip_gateway_mode_report_t                         mode_report;                           /**< Mode Report command state */
	zwp_avi_interfaces_zip_gateway_unsolicited_destination_report_t      unsolicited_destination_report;        /**< Unsolicted Destination Report command state */
} zwp_avi_interfaces_zip_gateway_state_t;

/**
 * @brief Allocate interface level data
 * @param[out]    state_p           Interface level data
 * @param[in]     pool              Memory pool to be used for allocation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_zip_gateway_state_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_zip_gateway_state_alloc(void **state_p, zwp_pool_t pool)
{
	zwp_avi_interfaces_zip_gateway_state_t *state;

	if ((state = (zwp_avi_interfaces_zip_gateway_state_t *)ZWP_CALLOC(1, sizeof(*state))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_state_alloc;
	}

	if (zwp_thread_cond_create(&state->mode_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for mode report received event");
		goto l_err_net_mode_report_received_cond_create;
	}

	if (zwp_thread_cond_create(&state->unsolicited_destination_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for unsolicited destination report received event");
		goto l_err_net_unsol_dest_report_received_cond_create;
	}

	*state_p = state;

	return ZWP_STATUS_OK;

	zwp_thread_cond_destroy(state->unsolicited_destination_report.report_receive_condition);
l_err_net_unsol_dest_report_received_cond_create:
	zwp_thread_cond_destroy(state->mode_report.report_receive_condition);
l_err_net_mode_report_received_cond_create:
	ZWP_FREE(state);
l_err_state_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free interface level data
 * @param[in]     arg               Interface level data
 * @return        Nothing
 * @see zwp_avi_interfaces_zip_gateway_state_alloc
 */
static void zwp_avi_interfaces_zip_gateway_state_free(void *arg)
{
	zwp_avi_interfaces_zip_gateway_state_t *state;

	state = (zwp_avi_interfaces_zip_gateway_state_t *)arg;

	zwp_interface_report_receive_done(&state->mode_report.report_receive_state, state->mode_report.report_receive_condition);

	zwp_thread_cond_destroy(state->mode_report.report_receive_condition);

	zwp_interface_report_receive_done(&state->unsolicited_destination_report.report_receive_state, state->unsolicited_destination_report.report_receive_condition);

	zwp_thread_cond_destroy(state->unsolicited_destination_report.report_receive_condition);

	ZWP_FREE(state);
}

/**
 * @brief HCAPI callback for Mode Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  status            0=success; 1=failure due to either timeout or the gateway is locked with its parameter hidden. 2=transmit error.
 * @param[in]	  mode	            operating mode; ZW_GW_XXX
 * @param[in]	  portal_profile	portal profile; only valid if mode is ZW_GW_PORTAL. If it is NULL, no profile is stored in the Z/IP gateway.
 * @return        Nothing
 */
static void zwp_avi_interfaces_zip_gateway_mode_report_handler(zwifd_p ifd, uint8_t status, uint8_t mode, zwgw_portal_prof_t *portal_profile)
{
	zwp_avi_interfaces_zip_gateway_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_zip_gateway_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	state->mode_report.utime = zwu_time_now();
	state->mode_report.status = status;
	state->mode_report.mode = mode;

	memset(&state->mode_report.profile, 0, sizeof(zwgw_portal_prof_t));

	state->mode_report.profile_present = 0;

	if(portal_profile)
	{
		memcpy(&state->mode_report.profile, portal_profile, sizeof(zwgw_portal_prof_t));
		state->mode_report.profile_present = 1;

		/*
		 * To avoid usage of uninitialized portion in Avro message
		 */
		memset(state->mode_report.profile.portal_name, 0, sizeof(state->mode_report.profile.portal_name));
		memcpy(state->mode_report.profile.portal_name, portal_profile->portal_name, portal_profile->name_len);
	}

	zwp_interface_report_receive_done(&state->mode_report.report_receive_state, state->mode_report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

l_err_get_state:
	zwp_net_release_desc_interface(ifd);
l_err_acquire_desc:

	return;
}

/**
 * @brief Test if the expected Mode Report command is received
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Mode Get command
 * @param[out]    report_receive_condition_p    Report command receive condition
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_zip_gateway_mode_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_zip_gateway_state_t *state;

	if ((state = (zwp_avi_interfaces_zip_gateway_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->mode_report.report_receive_condition;

	if (zwp_interface_report_receive_assertion(state->mode_report.report_receive_state) != ZWP_STATUS_OK)
	{
		goto l_err_report_not_received;
	}

	return ZWP_STATUS_OK;

l_err_report_not_received:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute active Mode Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Mode Get command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_zip_gateway_mode_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_zip_gateway_state_t *state;

	if ((state = (zwp_avi_interfaces_zip_gateway_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->mode_report.report_receive_state);

	*hcapi_code_p = zwif_gw_mode_get(desc_interface, zwp_avi_interfaces_zip_gateway_mode_report_handler);

	return ZWP_STATUS_OK;

l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute passive Mode Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Mode Get command
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_zip_gateway_mode_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_zip_gateway_mode_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_zip_gateway_state_t *state;
	zwp_avi_interfaces_zip_gateway_mode_report_t *report;

	if ((state = (zwp_avi_interfaces_zip_gateway_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((report = (zwp_avi_interfaces_zip_gateway_mode_report_t *)ZWP_CALLOC(1, sizeof(*report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	memcpy(report, &state->mode_report, sizeof(*report));

	*result_p = report;

	return ZWP_STATUS_OK;

l_err_report_alloc:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to Mode Report command
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_zip_gateway_mode_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_zip_gateway_mode_report_t *report;

	report = (zwp_avi_interfaces_zip_gateway_mode_report_t *)result;

	if((zw_avro_field_set_long(arg_value, "utime", report->utime) != ZWP_STATUS_OK) ||
	   (zw_avro_field_set_int(arg_value, "status", report->status) != ZWP_STATUS_OK) ||
	   (zw_avro_field_set_int(arg_value, "mode", report->mode) != ZWP_STATUS_OK) ||
	   (zw_avro_field_set_int(arg_value, "profile_present", report->profile_present) != ZWP_STATUS_OK) ||
	   (zw_avro_field_set_fixed(arg_value, "ipv6_address", report->profile.addr6, 16) != ZWP_STATUS_OK) ||
	   (zw_avro_field_set_int(arg_value, "port", report->profile.port) != 0) ||
	   (zw_avro_field_set_int(arg_value, "peer_name_length", report->profile.name_len) != ZWP_STATUS_OK)||
	   (zw_avro_field_set_fixed(arg_value, "peer_name", report->profile.portal_name, 63) != ZWP_STATUS_OK)
	  )
	{
		goto l_err_set_report_value;
	}

	return ZWP_STATUS_OK;

l_err_set_report_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Mode Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_zip_gateway_mode_get_execute_passive
 */
static void zwp_avi_interfaces_zip_gateway_mode_get_result_free(void *result)
{
	ZWP_FREE(result);
}

/**
 * @brief Get argument for Mode Set command from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Mode Set command
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_zip_gateway_mode_set_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_zip_gateway_mode_set_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_zip_gateway_mode_set_t *argument;
	int mode, port, name_len;
	void *ipv6_address, *peer_name;
	size_t ipv6_address_size, peer_name_size;

	if ((argument = (zwp_avi_interfaces_zip_gateway_mode_set_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "mode", &mode) != ZWP_STATUS_OK)
	{
		goto l_err_get_value;
	}
	else
	{
		memset(&argument->profile, 0, sizeof(zwgw_portal_prof_t));
		argument->mode = (uint8_t)mode;

		if(mode == PORTAL_MODE)
		{
			if ((zw_avro_field_get_int(arg_value, "port", &port) != ZWP_STATUS_OK) ||
				(zw_avro_field_get_int(arg_value, "peer_name_length", &name_len) != ZWP_STATUS_OK)
			   )
			{
				goto l_err_get_value;
			}

			if (zw_avro_field_get_fixed(arg_value, "ipv6_address", &ipv6_address, &ipv6_address_size) != ZWP_STATUS_OK)
			{
				goto l_err_get_ipv6_address_value;
			}

			if (zw_avro_field_get_fixed(arg_value, "peer_name", &peer_name, &peer_name_size) != ZWP_STATUS_OK)
			{
				goto l_err_get_peer_name_value;
			}

			if ((ipv6_address_size != 16) || (peer_name_size != 63) || (name_len > 63))
			{
				ZWP_LOG(ZWP_LOG_ERR, "Invalid GW profile settings");
				goto l_err_addr_name_size;
			}

			argument->profile.port = (uint16_t)port;
			argument->profile.name_len = (uint8_t)name_len;

			memcpy(&argument->profile.addr6[0], ipv6_address, ipv6_address_size);
			memcpy(&argument->profile.portal_name[0], peer_name, peer_name_size);

			zw_avro_field_free_fixed(ipv6_address);
			zw_avro_field_free_fixed(peer_name);

		}
	}

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_addr_name_size:
	zw_avro_field_free_fixed(peer_name);
l_err_get_peer_name_value:
	zw_avro_field_free_fixed(ipv6_address);
l_err_get_ipv6_address_value:
l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Mode Set command
 * @param[in]     args              Arguments passed with Mode Set command
 * @return        Nothing
 * @see zwp_avi_interfaces_zip_gateway_mode_set_argument
 */
static void zwp_avi_interfaces_zip_gateway_mode_set_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
 * @brief Execute Mode Set command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments passed with Mode Set command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_zip_gateway_mode_set_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_zip_gateway_mode_set_t *argument;

	argument = (zwp_avi_interfaces_zip_gateway_mode_set_t *)args;

	*hcapi_code_p = zwif_gw_mode_set(desc_interface, argument->mode, &argument->profile);

	return ZWP_STATUS_OK;
}

/**
 * @brief Get argument for Lock Set command from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Lock Set command
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_zip_gateway_lock_set_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_zip_gateway_lock_set_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_zip_gateway_lock_set_t *argument;
	int lock;
	int show;

	if ((argument = (zwp_avi_interfaces_zip_gateway_lock_set_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if ((zw_avro_field_get_int(arg_value, "lock", &lock) != ZWP_STATUS_OK) ||
		(zw_avro_field_get_int(arg_value, "show", &show) != ZWP_STATUS_OK)
	   )
	{
		goto l_err_get_value;
	}

	argument->lock = (uint8_t)lock;
	argument->show = (uint8_t)show;

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Lock Set command
 * @param[in]     args              Arguments passed with Lock Set command
 * @return        Nothing
 * @see zwp_avi_interfaces_zip_gateway_lock_set_argument
 */
static void zwp_avi_interfaces_zip_gateway_lock_set_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
 * @brief Execute Lock Set command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments passed with Lock Set command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_zip_gateway_lock_set_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_zip_gateway_lock_set_t *argument;

	argument = (zwp_avi_interfaces_zip_gateway_lock_set_t *)args;

	*hcapi_code_p = zwif_gw_cfg_lock(desc_interface, argument->lock, argument->show);

	return ZWP_STATUS_OK;
}

/**
 * @brief HCAPI callback for Unsolicited Destination Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  dst_ip	        unsolicited destination IPv6 address
 * @param[in]	  dst_port	        unsolicited destination port (recommended 4123)
 * @return        Nothing
 */
static void zwp_avi_interfaces_zip_gateway_unsolicited_destination_report_handler(zwifd_p ifd,  uint8_t *dst_ip, uint16_t dst_port)
{
	zwp_avi_interfaces_zip_gateway_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_zip_gateway_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	state->unsolicited_destination_report.utime = zwu_time_now();
	memcpy(state->unsolicited_destination_report.unsolicited_ipv6_destination, dst_ip, sizeof(state->unsolicited_destination_report.unsolicited_ipv6_destination));
	state->unsolicited_destination_report.unsolicited_destination_port = dst_port;

	zwp_interface_report_receive_done(&state->unsolicited_destination_report.report_receive_state, state->unsolicited_destination_report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

l_err_get_state:
	zwp_net_release_desc_interface(ifd);
l_err_acquire_desc:

	return;
}

/**
 * @brief Test if the expected Unsolicited Destination Report command is received
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Unsolicited Destination Get command
 * @param[out]    report_receive_condition_p    Report command receive condition
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_zip_gateway_unsolicited_destination_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_zip_gateway_state_t *state;

	if ((state = (zwp_avi_interfaces_zip_gateway_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->unsolicited_destination_report.report_receive_condition;

	if (zwp_interface_report_receive_assertion(state->unsolicited_destination_report.report_receive_state) != ZWP_STATUS_OK)
	{
		goto l_err_report_not_received;
	}

	return ZWP_STATUS_OK;

l_err_report_not_received:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute active Unsolicited Destination Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Unsolicited Destination Get command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_zip_gateway_unsolicited_destination_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_zip_gateway_state_t *state;

	if ((state = (zwp_avi_interfaces_zip_gateway_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->unsolicited_destination_report.report_receive_state);

	*hcapi_code_p = zwif_gw_unsolicit_get(desc_interface, zwp_avi_interfaces_zip_gateway_unsolicited_destination_report_handler);

	return ZWP_STATUS_OK;

l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Execute passive Unsolicited Destination Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Unsolicited Destination Get command
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_zip_gateway_unsolicited_destination_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_zip_gateway_unsolicited_destination_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_zip_gateway_state_t *state;
	zwp_avi_interfaces_zip_gateway_unsolicited_destination_report_t *report;
	zwp_net_t net;
	zwnet_p zwnet;
	zwnet_init_p net_init;
	uint8_t local_address[16];

	if ((state = (zwp_avi_interfaces_zip_gateway_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((report = (zwp_avi_interfaces_zip_gateway_unsolicited_destination_report_t *)ZWP_CALLOC(1, sizeof(*report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	memcpy(report, &state->unsolicited_destination_report, sizeof(*report));

	net = (zwp_net_t)zwnet_get_user(desc_interface->net);
	zwnet = zwp_net_get_zwnet(net);
	net_init = zwp_net_get_zwnet_init(net);

	if (net_init->use_ipv4)
	{
		// IPv4 mapped IPv6 address "::ffff:"
		memset(local_address, 0, 10);
		local_address[10] = local_address[11] = 0xFF;

		zwnet_local_addr_get(zwnet, net_init->zip_router, local_address + 12, net_init->use_ipv4);
	}
	else
	{
		zwnet_local_addr_get(zwnet, net_init->zip_router, local_address, net_init->use_ipv4);
	}
	memcpy(report->local_ipv6_address, local_address, sizeof(report->local_ipv6_address));

	report->local_destination_port = zwnet_listen_port_get(zwnet);

	*result_p = report;

	return ZWP_STATUS_OK;

l_err_report_alloc:
l_err_get_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Set Avro value object with values from Result object corresponding to Unsolicited Destination Report command
 * @param[in]     result            Result object
 * @param[out]    arg_value         Avro value object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_zip_gateway_unsolicited_destination_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_zip_gateway_unsolicited_destination_report_t *report;

	report = (zwp_avi_interfaces_zip_gateway_unsolicited_destination_report_t *)result;

	if ((zw_avro_field_set_long(arg_value, "utime", report->utime) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_fixed(arg_value, "unsolicited_ipv6_destination", report->unsolicited_ipv6_destination, 16) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "unsolicited_destination_port", report->unsolicited_destination_port) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_fixed(arg_value, "local_ipv6_address", report->local_ipv6_address, 16) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "local_destination_port", report->local_destination_port) != ZWP_STATUS_OK))
	{
		goto l_err_set_report_value;
	}

	return ZWP_STATUS_OK;

l_err_set_report_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Unsolicited Destination Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_zip_gateway_unsolicited_destination_get_execute_passive
 */
static void zwp_avi_interfaces_zip_gateway_unsolicited_destination_get_result_free(void *result)
{
	ZWP_FREE(result);
}

/**
 * @brief Get argument for Unsolicited Destination Set command from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Unsolicited Destination Set command
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_zip_gateway_unsolicited_destination_set_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_zip_gateway_unsolicited_destination_set_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_zip_gateway_unsolicited_destination_set_t *argument;
	void *unsolicited_ipv6_destination;
	size_t unsolicited_ipv6_destination_size;
	int32_t unsolicited_destination_port;

	if ((argument = (zwp_avi_interfaces_zip_gateway_unsolicited_destination_set_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "unsolicited_destination_port", &unsolicited_destination_port) != ZWP_STATUS_OK)
	{
		goto l_err_get_value;
	}

	if (zw_avro_field_get_fixed(arg_value, "unsolicited_ipv6_destination", &unsolicited_ipv6_destination, &unsolicited_ipv6_destination_size) != ZWP_STATUS_OK)
	{
		goto l_err_get_unsol_dest_value;
	}

	if (unsolicited_ipv6_destination_size != 16)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid unsolicited ipv6 destination value field size");
		goto l_err_unsol_dest_size;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Unsolicited IPv6 destination: "
			"%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
			((uint8_t *)unsolicited_ipv6_destination)[0], ((uint8_t *)unsolicited_ipv6_destination)[1],
			((uint8_t *)unsolicited_ipv6_destination)[2], ((uint8_t *)unsolicited_ipv6_destination)[3],
			((uint8_t *)unsolicited_ipv6_destination)[4], ((uint8_t *)unsolicited_ipv6_destination)[5],
			((uint8_t *)unsolicited_ipv6_destination)[6], ((uint8_t *)unsolicited_ipv6_destination)[7],
			((uint8_t *)unsolicited_ipv6_destination)[8], ((uint8_t *)unsolicited_ipv6_destination)[9],
			((uint8_t *)unsolicited_ipv6_destination)[10], ((uint8_t *)unsolicited_ipv6_destination)[11],
			((uint8_t *)unsolicited_ipv6_destination)[12], ((uint8_t *)unsolicited_ipv6_destination)[13],
			((uint8_t *)unsolicited_ipv6_destination)[14], ((uint8_t *)unsolicited_ipv6_destination)[15]);

	ZWP_LOG(ZWP_LOG_DEBUG, "Unsolicited Destination Port: %d", unsolicited_destination_port);

	if (zwu_number_is_uint16(unsolicited_destination_port) != ZWU_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid unsolicited destination port: %d", unsolicited_destination_port);
		goto l_err_invalid_unsolicited_destination_port;
	}

	memcpy(argument->unsolicited_ipv6_destination, unsolicited_ipv6_destination, sizeof(argument->unsolicited_ipv6_destination));
	argument->unsolicited_destination_port = (uint16_t)unsolicited_destination_port;

	zw_avro_field_free_fixed(unsolicited_ipv6_destination);

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_invalid_unsolicited_destination_port:
l_err_unsol_dest_size:
	zw_avro_field_free_fixed(unsolicited_ipv6_destination);
l_err_get_unsol_dest_value:
l_err_get_value:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Unsolicited Destination Set command
 * @param[in]     args              Arguments passed with Unsolicited Destination Set command
 * @return        Nothing
 * @see zwp_avi_interfaces_zip_gateway_unsolicited_destination_set_argument
 */
static void zwp_avi_interfaces_zip_gateway_unsolicited_destination_set_argument_free(void *args)
{
	ZWP_FREE(args);
}

/**
 * @brief Execute Unsolicited Destination Set command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments passed with Unsolicited Destination Set command
 * @param[out]    hcapi_code_p      HCAPI error code
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_zip_gateway_unsolicited_destination_set_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_zip_gateway_unsolicited_destination_set_t *argument;

	argument = (zwp_avi_interfaces_zip_gateway_unsolicited_destination_set_t *)args;

	*hcapi_code_p = zwif_gw_unsolicit_set(desc_interface, argument->unsolicited_ipv6_destination, argument->unsolicited_destination_port);

	return ZWP_STATUS_OK;
}

/** Handlers for Mode Get command */
static zwp_avi_message_interfaces_t interfaces_zip_gateway_mode_get =
{
		"interface_zip_gateway_mode_get",
		COMMAND_CLASS_ZIP_GATEWAY, NULL,
		zwp_avi_interfaces_zip_gateway_state_alloc, zwp_avi_interfaces_zip_gateway_state_free,
		NULL, NULL,
		zwp_avi_interfaces_zip_gateway_mode_get_report_receive_assertion,
		zwp_avi_interfaces_zip_gateway_mode_get_execute_active, zwp_avi_interfaces_zip_gateway_mode_get_execute_passive,
		zwp_avi_interfaces_zip_gateway_mode_get_result, zwp_avi_interfaces_zip_gateway_mode_get_result_free
};

/** Handlers for Mode Set command */
static zwp_avi_message_interfaces_t interfaces_zip_gateway_mode_set =
{
		"interface_zip_gateway_mode_set",
		COMMAND_CLASS_ZIP_GATEWAY, NULL,
		NULL, NULL,
		zwp_avi_interfaces_zip_gateway_mode_set_argument, zwp_avi_interfaces_zip_gateway_mode_set_argument_free,
		NULL,
		zwp_avi_interfaces_zip_gateway_mode_set_execute_active, NULL,
		NULL, NULL
};

/** Handlers for Lock Set command */
static zwp_avi_message_interfaces_t interfaces_zip_gateway_lock_set =
{
		"interface_zip_gateway_lock_set",
		COMMAND_CLASS_ZIP_GATEWAY, NULL,
		NULL, NULL,
		zwp_avi_interfaces_zip_gateway_lock_set_argument, zwp_avi_interfaces_zip_gateway_lock_set_argument_free,
		NULL,
		zwp_avi_interfaces_zip_gateway_lock_set_execute_active, NULL,
		NULL, NULL
};

/** Handlers for Unsolicited Destination Get command */
static zwp_avi_message_interfaces_t interfaces_zip_gateway_unsolicited_destination_get =
{
		"interface_zip_gateway_unsolicited_destination_get",
		COMMAND_CLASS_ZIP_GATEWAY, NULL,
		NULL, NULL,
		NULL, NULL,
		zwp_avi_interfaces_zip_gateway_unsolicited_destination_get_report_receive_assertion,
		zwp_avi_interfaces_zip_gateway_unsolicited_destination_get_execute_active, zwp_avi_interfaces_zip_gateway_unsolicited_destination_get_execute_passive,
		zwp_avi_interfaces_zip_gateway_unsolicited_destination_get_result, zwp_avi_interfaces_zip_gateway_unsolicited_destination_get_result_free
};

/** Handlers for Unsolicited Destination Set command */
static zwp_avi_message_interfaces_t interfaces_zip_gateway_unsolicited_destination_set =
{
		"interface_zip_gateway_unsolicited_destination_set",
		COMMAND_CLASS_ZIP_GATEWAY, NULL,
		NULL, NULL,
		zwp_avi_interfaces_zip_gateway_unsolicited_destination_set_argument, zwp_avi_interfaces_zip_gateway_unsolicited_destination_set_argument_free,
		NULL,
		zwp_avi_interfaces_zip_gateway_unsolicited_destination_set_execute_active, NULL,
		NULL, NULL
};

/** Register command handlers */
zwp_status_t zwp_avi_interfaces_zip_gateway_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_zip_gateway_mode_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_zip_gateway_mode_set) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_zip_gateway_lock_set) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_zip_gateway_unsolicited_destination_get) != ZWP_STATUS_OK)
			|| (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_zip_gateway_unsolicited_destination_set) != ZWP_STATUS_OK))
	{
		goto l_err_interface_list_get;
	}

	return ZWP_STATUS_OK;

l_err_interface_list_get:
	return ZWP_STATUS_ERROR;
}
