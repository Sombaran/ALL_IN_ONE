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
 * @file    zwp_avi_interfaces_thermostat_fan_state.c
 *
 * @brief   AVI message 'interface_thermostat_fan_state_xxx'
 * @details Handles the AVI message 'interface_thermostat_fan_state_xxx'.
 *
 * @author  Sasidhar
 *
 * @version 1.0 - 2013-10-04
 * - Initial version
 */

#include "zwp_avi_interfaces.h"

/** Report command state */
typedef struct {
	zwu_time_t         utime;                              /**< Last updated time of Report command */
	uint8_t            fan_operating_state;                /**< Fan operating state */

	int                report_receive_state;               /**< Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Report command receive condition */
} zwp_avi_interfaces_thermostat_fan_state_report_t;

/** Interface level state */
typedef struct {
	zwp_avi_interfaces_thermostat_fan_state_report_t      report;           /**< Report command state */
} zwp_avi_interfaces_thermostat_fan_state_t;

/**
 * @brief Allocate interface level data
 * @param[out]    state_p           Interface level data
 * @param[in]     pool              Memory pool to be used for allocation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_thermostat_fan_state_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_thermostat_fan_state_alloc(void **state_p, zwp_pool_t pool)
{
	zwp_avi_interfaces_thermostat_fan_state_t *state;

	if ((state = (zwp_avi_interfaces_thermostat_fan_state_t *)ZWP_CALLOC(1, sizeof(*state))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_state_alloc;
	}

	if (zwp_thread_cond_create(&state->report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for report received event");
		goto l_err_net_report_received_cond_create;
	}

	*state_p = state;

	return ZWP_STATUS_OK;

l_err_net_report_received_cond_create:
	ZWP_FREE(state);
l_err_state_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free interface level data
 * @param[in]     arg               Interface level data
 * @return        Nothing
 * @see zwp_avi_interfaces_thermostat_fan_state_alloc
 */
static void zwp_avi_interfaces_thermostat_fan_state_free(void *arg)
{
	zwp_avi_interfaces_thermostat_fan_state_t *state;

	state = (zwp_avi_interfaces_thermostat_fan_state_t *)arg;

	zwp_interface_report_receive_done(&state->report.report_receive_state, state->report.report_receive_condition);

	zwp_thread_cond_destroy(state->report.report_receive_condition);

	ZWP_FREE(state);
}

/**
 * @brief HCAPI callback for Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  value	            fan operating state, ZW_THRMO_FAN_STA_XXX
 * @return        Nothing
 */
static void zwp_avi_interfaces_thermostat_fan_state_report_handler(zwifd_p ifd, uint8_t value, time_t ts)
{
	zwp_avi_interfaces_thermostat_fan_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_thermostat_fan_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if(ts)
	{
		//state->report.utime = zwu_time_now();
		state->report.utime = ts;

		state->report.fan_operating_state = value;
	}

	zwp_interface_report_receive_done(&state->report.report_receive_state, state->report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

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
static zwp_status_t zwp_avi_interfaces_thermostat_fan_state_setup(zwifd_p desc_interface, int *hcapi_code_p)
{
	*hcapi_code_p = zwif_thrmo_fan_sta_rpt_set(desc_interface, zwp_avi_interfaces_thermostat_fan_state_report_handler);

	return ZWP_STATUS_OK;
}

/**
 * @brief Test if the expected Report command is received
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Get command
 * @param[out]    report_receive_condition_p    Report command receive condition
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_thermostat_fan_state_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_thermostat_fan_state_t *state;

	if ((state = (zwp_avi_interfaces_thermostat_fan_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
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
static zwp_status_t zwp_avi_interfaces_thermostat_fan_state_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_thermostat_fan_state_t *state;

	if ((state = (zwp_avi_interfaces_thermostat_fan_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->report.report_receive_state);

	*hcapi_code_p = zwif_thrmo_fan_sta_get(desc_interface, ZWIF_GET_BMSK_LIVE | ZWIF_GET_BMSK_CACHE);

	return ZWP_STATUS_OK;

l_err_get_state:
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
 * The caller should call zwp_avi_interfaces_thermostat_fan_state_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_thermostat_fan_state_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_thermostat_fan_state_t *state;
	zwp_avi_interfaces_thermostat_fan_state_report_t *report;

	if ((state = (zwp_avi_interfaces_thermostat_fan_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((report = (zwp_avi_interfaces_thermostat_fan_state_report_t *)ZWP_CALLOC(1, sizeof(*report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	memcpy(report, &state->report, sizeof(*report));

	*result_p = report;

	return ZWP_STATUS_OK;

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
static zwp_status_t zwp_avi_interfaces_thermostat_fan_state_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_thermostat_fan_state_report_t *report;

	report = (zwp_avi_interfaces_thermostat_fan_state_report_t *)result;

	if ((zw_avro_field_set_long(arg_value, "utime", report->utime) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "fan_operating_state", report->fan_operating_state) != ZWP_STATUS_OK))
	{
		goto l_err_set_report_value;
	}

	return ZWP_STATUS_OK;

l_err_set_report_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_thermostat_fan_state_get_execute_passive
 */
static void zwp_avi_interfaces_thermostat_fan_state_get_result_free(void *result)
{
	ZWP_FREE(result);
}

/** Handlers for Get command */
static zwp_avi_message_interfaces_t interfaces_thermostat_fan_state_get =
{
		"interface_thermostat_fan_state_get",
		COMMAND_CLASS_THERMOSTAT_FAN_STATE, zwp_avi_interfaces_thermostat_fan_state_setup,
		zwp_avi_interfaces_thermostat_fan_state_alloc, zwp_avi_interfaces_thermostat_fan_state_free,
		NULL, NULL,
		zwp_avi_interfaces_thermostat_fan_state_get_report_receive_assertion,
		zwp_avi_interfaces_thermostat_fan_state_get_execute_active, zwp_avi_interfaces_thermostat_fan_state_get_execute_passive,
		zwp_avi_interfaces_thermostat_fan_state_get_result, zwp_avi_interfaces_thermostat_fan_state_get_result_free
};

/** Register command handlers */
zwp_status_t zwp_avi_interfaces_thermostat_fan_state_init(zwu_array_list_t message_list)
{
	if (zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_thermostat_fan_state_get) != ZWP_STATUS_OK)
	{
		goto l_err_interface_list_get;
	}

	return ZWP_STATUS_OK;

l_err_interface_list_get:
	return ZWP_STATUS_ERROR;
}
