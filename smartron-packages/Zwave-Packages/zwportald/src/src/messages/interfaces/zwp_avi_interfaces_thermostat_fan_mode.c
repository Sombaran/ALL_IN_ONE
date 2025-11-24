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
 * @file    zwp_avi_interfaces_thermostat_fan_mode.c
 *
 * @brief   AVI message 'interface_thermostat_fan_mode_xxx'
 * @details Handles the AVI message 'interface_thermostat_fan_mode_xxx'.
 *
 * @author  Sasidhar
 *
 * @version 1.0 - 2013-10-04
 * - Initial version
 */

#include "zwp_avi_interfaces.h"

/** Set command state */
typedef struct {
	uint8_t            mode;                               /**< Fan Mode */
	uint8_t            off;                                /**< Is fan off?  */
} zwp_avi_interfaces_thermostat_fan_mode_set_t;

/** Report command state */
typedef struct {
	zwu_time_t         utime;                              /**< Last updated time of Report command */
	uint8_t            mode;                               /**< Fan Mode */
	uint8_t            off;                                /**< Is fan off? */

	int                report_receive_state;               /**< Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Report command receive condition */
} zwp_avi_interfaces_thermostat_fan_mode_report_t;

/** Supported Report command state */
typedef struct {
	zwu_time_t         utime;                              /**< Last updated time of Supported Report command */
	uint8_t            *sup_mode_list;                     /**< List of supported fan modes */
	uint8_t				sup_mode_list_cnt;                 /**< Count of fan modes in the list @a sup_mode_list */

	int                report_receive_state;               /**< Supported Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Supported Report command receive condition */
} zwp_avi_interfaces_thermostat_fan_mode_supported_report_t;

/** Interface level state */
typedef struct {
	zwp_avi_interfaces_thermostat_fan_mode_report_t                report;         /**< Report command state */
	zwp_avi_interfaces_thermostat_fan_mode_supported_report_t      sup_report;     /**< Supported Report command state */
} zwp_avi_interfaces_thermostat_fan_mode_state_t;


/**
 * @brief Allocate interface level data
 * @param[out]    state_p           Interface level data
 * @param[in]     pool              Memory pool to be used for allocation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_thermostat_fan_mode_state_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_thermostat_fan_mode_state_alloc(void **state_p, zwp_pool_t pool)
{
	zwp_avi_interfaces_thermostat_fan_mode_state_t *state;

	if ((state = (zwp_avi_interfaces_thermostat_fan_mode_state_t *)ZWP_CALLOC(1, sizeof(*state))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_state_alloc;
	}

	if (zwp_thread_cond_create(&state->report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for report received event");
		goto l_err_net_report_received_cond_create;
	}

	if (zwp_thread_cond_create(&state->sup_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for report received event");
		goto l_err_net_sup_report_received_cond_create;
	}

	state->sup_report.sup_mode_list = NULL;
	state->sup_report.sup_mode_list_cnt = 0;

	*state_p = state;

	return ZWP_STATUS_OK;

	zwp_thread_cond_destroy(state->sup_report.report_receive_condition);

l_err_net_sup_report_received_cond_create:
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
 * @see zwp_avi_interfaces_thermostat_fan_mode_state_alloc
 */
static void zwp_avi_interfaces_thermostat_fan_mode_state_free(void *arg)
{
	zwp_avi_interfaces_thermostat_fan_mode_state_t *state;

	state = (zwp_avi_interfaces_thermostat_fan_mode_state_t *)arg;

	ZWP_FREE(state->sup_report.sup_mode_list);

	zwp_interface_report_receive_done(&state->report.report_receive_state, state->report.report_receive_condition);

	zwp_thread_cond_destroy(state->report.report_receive_condition);

	zwp_interface_report_receive_done(&state->sup_report.report_receive_state, state->sup_report.report_receive_condition);

	zwp_thread_cond_destroy(state->sup_report.report_receive_condition);

	ZWP_FREE(state);
}

/**
 * @brief HCAPI callback for Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  off               fan off mode flag. Non-zero indicates that the fan is fully OFF, 0 indicates that it is possible to change between Fan Modes.
 * @param[in]	  mode              fan operating mode, ZW_THRMO_FAN_MD_XXX
 * @return        Nothing
 */
static void zwp_avi_interfaces_thermostat_fan_mode_report_handler(zwifd_p ifd, uint8_t off, uint8_t mode, time_t ts)
{
	zwp_avi_interfaces_thermostat_fan_mode_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_thermostat_fan_mode_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if(ts)
	{
		//state->report.utime = zwu_time_now();
		state->report.utime = ts;

		state->report.mode = mode;

		state->report.off = off;
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
static zwp_status_t zwp_avi_interfaces_thermostat_fan_mode_setup(zwifd_p desc_interface, int *hcapi_code_p)
{
	*hcapi_code_p = zwif_thrmo_fan_md_rpt_set(desc_interface, zwp_avi_interfaces_thermostat_fan_mode_report_handler);

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
static zwp_status_t zwp_avi_interfaces_thermostat_fan_mode_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_thermostat_fan_mode_state_t *state;

	if ((state = (zwp_avi_interfaces_thermostat_fan_mode_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
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
static zwp_status_t zwp_avi_interfaces_thermostat_fan_mode_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_thermostat_fan_mode_state_t *state;

	if ((state = (zwp_avi_interfaces_thermostat_fan_mode_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->report.report_receive_state);

	*hcapi_code_p = zwif_thrmo_fan_md_get(desc_interface, ZWIF_GET_BMSK_LIVE | ZWIF_GET_BMSK_CACHE);

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
 * The caller should call zwp_avi_interfaces_thermostat_fan_mode_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_thermostat_fan_mode_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_thermostat_fan_mode_state_t *state;
	zwp_avi_interfaces_thermostat_fan_mode_report_t *report;

	if ((state = (zwp_avi_interfaces_thermostat_fan_mode_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((report = (zwp_avi_interfaces_thermostat_fan_mode_report_t *)ZWP_CALLOC(1, sizeof(*report))) == NULL)
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
static zwp_status_t zwp_avi_interfaces_thermostat_fan_mode_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_thermostat_fan_mode_report_t *report;

	report = (zwp_avi_interfaces_thermostat_fan_mode_report_t *)result;

	if ((zw_avro_field_set_long(arg_value, "utime", report->utime) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "mode", report->mode) != ZWP_STATUS_OK)
			|| (zw_avro_field_set_int(arg_value, "off", report->off) != ZWP_STATUS_OK))
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
 * @see zwp_avi_interfaces_thermostat_fan_mode_get_execute_passive
 */
static void zwp_avi_interfaces_thermostat_fan_mode_get_result_free(void *result)
{
	ZWP_FREE(result);
}

/**
 * @brief HCAPI callback for Supported Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  off               flag to indicate whether off mode is supported.
 * @param[in]	  mode_len          size of mode buffer
 * @param[in]	  mode              buffer to store supported thermostat fan operating modes (ZW_THRMO_FAN_MD_XXX)
 * @return        Nothing
 */
static void zwp_avi_interfaces_thermostat_fan_mode_supported_report_handler(zwifd_p ifd, uint8_t off, uint8_t mode_len, uint8_t *mode, int valid)
{
	zwp_avi_interfaces_thermostat_fan_mode_state_t *state;
	zwifd_p desc_interface;
	uint8_t *tmp;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_thermostat_fan_mode_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if (valid)
	{
		state->sup_report.utime = zwu_time_now();


		if (mode_len > state->sup_report.sup_mode_list_cnt)
		{
			if ((tmp = (uint8_t *)ZWP_REALLOC(state->sup_report.sup_mode_list, sizeof(*tmp) * mode_len)) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				goto l_err_sup_mode_list_alloc;
			}

			state->sup_report.sup_mode_list = tmp;
		}
		state->sup_report.sup_mode_list_cnt = mode_len;

		memcpy(state->sup_report.sup_mode_list, mode, state->sup_report.sup_mode_list_cnt);
	}
	zwp_interface_report_receive_done(&state->sup_report.report_receive_state, state->sup_report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

l_err_sup_mode_list_alloc:
l_err_get_state:
	zwp_net_release_desc_interface(ifd);
l_err_acquire_desc:

	return;
}

/**
 * @brief Test if the expected Supported Report command is received
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Supported Get command
 * @param[out]    report_receive_condition_p    Report command receive condition
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_interfaces_thermostat_fan_mode_supported_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_thermostat_fan_mode_state_t *state;

	if ((state = (zwp_avi_interfaces_thermostat_fan_mode_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->sup_report.report_receive_condition;

	if (zwp_interface_report_receive_assertion(state->sup_report.report_receive_state) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "report_receive_condition");
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
static zwp_status_t zwp_avi_interfaces_thermostat_fan_mode_supported_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_thermostat_fan_mode_state_t *state;

	if ((state = (zwp_avi_interfaces_thermostat_fan_mode_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get mode");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->sup_report.report_receive_state);

	*hcapi_code_p = zwif_thrmo_fan_md_sup_get(desc_interface, zwp_avi_interfaces_thermostat_fan_mode_supported_report_handler, 0);

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
 * The caller should call zwp_avi_interfaces_thermostat_fan_mode_supported_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_thermostat_fan_mode_supported_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_thermostat_fan_mode_state_t *state;
	zwp_avi_interfaces_thermostat_fan_mode_supported_report_t *sup_report;

	if ((state = (zwp_avi_interfaces_thermostat_fan_mode_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get mode");
		goto l_err_get_state;
	}

	if ((sup_report = (zwp_avi_interfaces_thermostat_fan_mode_supported_report_t *)ZWP_CALLOC(1, sizeof(*sup_report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	memcpy(sup_report, &state->sup_report, sizeof(*sup_report));

	if ((sup_report->sup_mode_list = (uint8_t *)ZWP_CALLOC(1,
			sizeof(*sup_report->sup_mode_list) * state->sup_report.sup_mode_list_cnt)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_type_list_alloc;
	}

	memcpy(sup_report->sup_mode_list, state->sup_report.sup_mode_list, state->sup_report.sup_mode_list_cnt);

	*result_p = sup_report;

	return ZWP_STATUS_OK;

l_err_type_list_alloc:
	ZWP_FREE(sup_report);
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
static zwp_status_t zwp_avi_interfaces_thermostat_fan_mode_supported_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_thermostat_fan_mode_supported_report_t *report;
	int32_t i=0;
	avro_value_t sup_modes_list_value;
	avro_value_t mode_element;

	report = (zwp_avi_interfaces_thermostat_fan_mode_supported_report_t *)result;

	if (avro_value_get_by_name(arg_value, "sup_mode_list", &sup_modes_list_value, NULL) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'sup_mode_list': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (zw_avro_field_set_long(arg_value, "utime", report->utime) != ZWP_STATUS_OK)
	{
		goto l_err_set_report_value;
	}

	for (i = 0; i < report->sup_mode_list_cnt; i++)
	{
		if (avro_value_append(&sup_modes_list_value, &mode_element, 0) != 0)
		{
			goto l_err_append_sup_mode_value;
		}

		if (avro_value_set_int(&mode_element, report->sup_mode_list[i]) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to set value '%d' to the field sup_modes : %s", report->sup_mode_list[i], avro_strerror);
			goto l_err_set_value;
		}
	}

	return ZWP_STATUS_OK;

l_err_set_value:
l_err_append_sup_mode_value:
l_err_set_report_value:
l_err_get_list_value:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Supported Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_thermostat_fan_mode_supported_get_execute_passive
 */
static void zwp_avi_interfaces_thermostat_fan_mode_supported_get_result_free(void *result)
{
	zwp_avi_interfaces_thermostat_fan_mode_supported_report_t *sup_report;

	sup_report = (zwp_avi_interfaces_thermostat_fan_mode_supported_report_t *)result;

	ZWP_FREE(sup_report->sup_mode_list);

	ZWP_FREE(sup_report);
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
 * The caller should call zwp_avi_interfaces_thermostat_fan_mode_set_argument_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_thermostat_fan_mode_set_argument(avro_value_t *arg_value, void **args_p)
{
	zwp_avi_interfaces_thermostat_fan_mode_set_t *argument;
	int integer;

	if ((argument = (zwp_avi_interfaces_thermostat_fan_mode_set_t *)ZWP_MALLOC(sizeof(*argument))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_args_alloc;
	}

	if (zw_avro_field_get_int(arg_value, "mode", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_get_value_mode;
	}
	argument->mode = (uint8_t)integer;

	if (zw_avro_field_get_int(arg_value, "off", &integer) != ZWP_STATUS_OK)
	{
		goto l_err_get_value_off;
	}
	argument->off = (uint8_t)integer;

	*args_p = argument;

	return ZWP_STATUS_OK;

l_err_get_value_off:
l_err_get_value_mode:
	ZWP_FREE(argument);
l_err_args_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Set command
 * @param[in]     args              Arguments passed with Set command
 * @return        Nothing
 * @see zwp_avi_interfaces_thermostat_fan_mode_set_argument
 */
static void zwp_avi_interfaces_thermostat_fan_mode_set_argument_free(void *args)
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
static zwp_status_t zwp_avi_interfaces_thermostat_fan_mode_set_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_thermostat_fan_mode_set_t *argument;

	argument = (zwp_avi_interfaces_thermostat_fan_mode_set_t *)args;

	*hcapi_code_p = zwif_thrmo_fan_md_set(desc_interface, argument->off, argument->mode);

	return ZWP_STATUS_OK;
}

/** Handlers for Get command */
static zwp_avi_message_interfaces_t interfaces_thermostat_fan_mode_get =
{
		"interface_thermostat_fan_mode_get",
		COMMAND_CLASS_THERMOSTAT_FAN_MODE, zwp_avi_interfaces_thermostat_fan_mode_setup,
		zwp_avi_interfaces_thermostat_fan_mode_state_alloc, zwp_avi_interfaces_thermostat_fan_mode_state_free,
		NULL, NULL,
		zwp_avi_interfaces_thermostat_fan_mode_get_report_receive_assertion,
		zwp_avi_interfaces_thermostat_fan_mode_get_execute_active, zwp_avi_interfaces_thermostat_fan_mode_get_execute_passive,
		zwp_avi_interfaces_thermostat_fan_mode_get_result, zwp_avi_interfaces_thermostat_fan_mode_get_result_free
};


/** Handlers for Set command */
static zwp_avi_message_interfaces_t interfaces_thermostat_fan_mode_set =
{
		"interface_thermostat_fan_mode_set",
		COMMAND_CLASS_THERMOSTAT_FAN_MODE, NULL,
		NULL, NULL,
		zwp_avi_interfaces_thermostat_fan_mode_set_argument, zwp_avi_interfaces_thermostat_fan_mode_set_argument_free,
		NULL,
		zwp_avi_interfaces_thermostat_fan_mode_set_execute_active, NULL,
		NULL, NULL
};

/** Handlers for Supported Get command */
static zwp_avi_message_interfaces_t interfaces_thermostat_fan_mode_supported_get =
{
		"interface_thermostat_fan_mode_supported_get",
		COMMAND_CLASS_THERMOSTAT_FAN_MODE, NULL,
		NULL, NULL,
		NULL, NULL,
		zwp_avi_interfaces_thermostat_fan_mode_supported_get_report_receive_assertion,
		zwp_avi_interfaces_thermostat_fan_mode_supported_get_execute_active, zwp_avi_interfaces_thermostat_fan_mode_supported_get_execute_passive,
		zwp_avi_interfaces_thermostat_fan_mode_supported_get_result, zwp_avi_interfaces_thermostat_fan_mode_supported_get_result_free
};

/** Register command handlers */
zwp_status_t zwp_avi_interfaces_thermostat_fan_mode_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_thermostat_fan_mode_get) != ZWP_STATUS_OK)
		||(zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_thermostat_fan_mode_supported_get) != ZWP_STATUS_OK)
		||(zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_thermostat_fan_mode_set) != ZWP_STATUS_OK))
	{
		goto l_err_interface_list_get;
	}

	return ZWP_STATUS_OK;

l_err_interface_list_get:
	return ZWP_STATUS_ERROR;
}
