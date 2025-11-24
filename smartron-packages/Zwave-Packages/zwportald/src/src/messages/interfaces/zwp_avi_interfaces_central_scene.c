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
 * @file    zwp_avi_interfaces_central_scene.c
 *
 * @brief   AVI message 'interface_central_scene_xxx'
 * @details Handles the AVI message 'interface_central_scene_xxx'.
 *
 * @author  Angela
 *
 * @version 1.0 - 2015-07-13
 * - Initial version
 */

#include "zwp_avi_interfaces.h"


/** Report command state */
typedef struct {
	zwu_time_t         utime;                              /**< Last updated time of Report command */
	zwcsc_notif_t      csc_notfication;                    /**< Central Scene Notification */

	int                report_receive_state;               /**< Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Report command receive condition */
} zwp_avi_interfaces_central_scene_report_t;

/** key attributes supported by 1 scene */
typedef struct {
	uint8_t			   sceneNo;								/**< Scene identifier, valid only when different scenes supports different key attributes. */
	uint8_t			   sup_ka_cnt;							/**< Count of supported key attributes for this scene */
	uint8_t            *ka_list;							/**< List of supported key attributes for this scene */
} zwp_avi_interfaces_key_attribute_t;

/** Supported Report command state */
typedef struct {
	zwu_time_t         utime;								/**< Last updated time of Supported Report command */
	uint8_t            scene_cnt;							/**< Count of supported scenes */
	uint8_t            sameKA;								/**< If all scenes are supporting the same Key Attributes. 1 - same, 0 - different */
	zwu_array_list_t   scene_ka_list;						/**< List of key attributes of type @a zwp_avi_interfaces_key_attribute_t */

	int                report_receive_state;               /**< Supported Report command receive state */
	zwp_thread_cond_t  report_receive_condition;           /**< Supported Report command receive condition */
} zwp_avi_interfaces_central_scene_supported_report_t;

/** Interface level state */
typedef struct {
	zwp_avi_interfaces_central_scene_report_t                report;             /**< Report command state */
	zwp_avi_interfaces_central_scene_supported_report_t      sup_report;         /**< Supported Report command state */
} zwp_avi_interfaces_central_scene_state_t;


/**
 * @brief Allocate interface level data
 * @param[out]    state_p           Interface level data
 * @param[in]     pool              Memory pool to be used for allocation
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_central_scene_state_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_central_scene_state_alloc(void **state_p, zwp_pool_t pool)
{
	zwp_avi_interfaces_central_scene_state_t *state;

	if ((state = (zwp_avi_interfaces_central_scene_state_t *)ZWP_CALLOC(1, sizeof(*state))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_state_alloc;
	}

	if (zwp_thread_cond_create(&state->report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for report received event");
		goto l_err_net_report_received_cond_create;
	}

	if (zwu_array_list_create(0, &state->sup_report.scene_ka_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	if (zwp_thread_cond_create(&state->sup_report.report_receive_condition, pool) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to create condvar for report received event");
		goto l_err_net_sup_report_received_cond_create;
	}

	*state_p = state;

	return ZWP_STATUS_OK;

	zwp_thread_cond_destroy(state->sup_report.report_receive_condition);

l_err_net_sup_report_received_cond_create:
	zwu_array_list_destroy(state->sup_report.scene_ka_list);
l_err_value_list_create:
	zwp_thread_cond_destroy(state->report.report_receive_condition);
l_err_net_report_received_cond_create:
	ZWP_FREE(state);
l_err_state_alloc:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Free scene_ka_list
* @param[in]     scene_ka_list       an zwu_array_list of type @a zwp_avi_interfaces_key_attribute_t
* @return        Nothing
*/
static void zwp_avi_interfaces_scene_ka_list_free(zwu_array_list_t scene_ka_list)
{
	int i, scene_ka_count = 0;
	zwp_avi_interfaces_key_attribute_t *key_attri_per_scene;

	scene_ka_count = zwu_array_list_size(scene_ka_list);

	for (i = 0; i < scene_ka_count; i++)
	{
		key_attri_per_scene = zwu_array_list_get(scene_ka_list, i);

		if(key_attri_per_scene->ka_list)
			ZWP_FREE(key_attri_per_scene->ka_list);
		ZWP_FREE(key_attri_per_scene);
	}

	zwu_array_list_remove_all(scene_ka_list);
}

/**
 * @brief Free interface level data
 * @param[in]     arg               Interface level data
 * @return        Nothing
 * @see zwp_avi_interfaces_central_scene_state_alloc
 */
static void zwp_avi_interfaces_central_scene_state_free(void *arg)
{
	zwp_avi_interfaces_central_scene_state_t *state;

	state = (zwp_avi_interfaces_central_scene_state_t *)arg;

	zwp_interface_report_receive_done(&state->report.report_receive_state, state->report.report_receive_condition);

	zwp_thread_cond_destroy(state->report.report_receive_condition);

	zwp_interface_report_receive_done(&state->sup_report.report_receive_state, state->sup_report.report_receive_condition);

	zwp_thread_cond_destroy(state->sup_report.report_receive_condition);

	if (state->sup_report.scene_ka_list)
	{
		zwp_avi_interfaces_scene_ka_list_free(state->sup_report.scene_ka_list);
		zwu_array_list_destroy(state->sup_report.scene_ka_list);
	}

	ZWP_FREE(state);
}

/**
 * @brief HCAPI callback for Report command
 * @param[in]	ifd	    interface
 * @param[in]	data	Central scene notification data.
 * @param[in]	ts      time stamp
 * @return      Nothing
 */
static void zwp_avi_interfaces_central_scene_report_handler(zwifd_p ifd, zwcsc_notif_p data, time_t ts)
{
	zwp_avi_interfaces_central_scene_state_t *state;
	zwifd_p desc_interface;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_central_scene_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if(ts)
	{
		//state->report.utime = zwu_time_now();
		state->report.utime = ts;
		state->report.csc_notfication.seqNo = data->seqNo;
		state->report.csc_notfication.sceneNo = data->sceneNo;
		state->report.csc_notfication.keyAttr = data->keyAttr;
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
static zwp_status_t zwp_avi_interfaces_central_scene_setup(zwifd_p desc_interface, int *hcapi_code_p)
{
	*hcapi_code_p = hcaal_central_scene_rpt_set(desc_interface, zwp_avi_interfaces_central_scene_report_handler);

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
static zwp_status_t zwp_avi_interfaces_central_scene_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_central_scene_state_t *state;

	if ((state = (zwp_avi_interfaces_central_scene_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
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
 * @brief Execute passive Get command
 * @param[in]     desc_interface    Interface descriptor
 * @param[in]     args              Arguments (if any) passed with Get command
 * @param[out]    result_p          Result object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_interfaces_central_scene_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_central_scene_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_central_scene_state_t *state;
	zwp_avi_interfaces_central_scene_report_t *report;

	if ((state = (zwp_avi_interfaces_central_scene_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((report = (zwp_avi_interfaces_central_scene_report_t *)ZWP_CALLOC(1, sizeof(*report))) == NULL)
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
static zwp_status_t zwp_avi_interfaces_central_scene_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_central_scene_report_t *report;

	report = (zwp_avi_interfaces_central_scene_report_t *)result;

	if ((zw_avro_field_set_long(arg_value, "utime", report->utime) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_int(arg_value, "sequence_number", report->csc_notfication.seqNo) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_int(arg_value, "key_attribute", report->csc_notfication.keyAttr) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_int(arg_value, "scene_number", report->csc_notfication.sceneNo) != ZWP_STATUS_OK)
	   )
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
 * @see zwp_avi_interfaces_central_scene_get_execute_passive
 */
static void zwp_avi_interfaces_central_scene_get_result_free(void *result)
{
	ZWP_FREE(result);
}

/**
 * @brief HCAPI callback for Supported Report command
 * @param[in]	  ifd	            interface
 * @param[in]	  scene_cnt         maximum number of supported scenes.
 * @param[in]	  sameKA            if all scenes are supporting the same Key Attributes. 1 - same, 0 - different
 * @param[in]	  KA_array_len      length/width of key attribute array. For v1 COMMAND_CLASS_CENTRAL_SCENE, KA_array_len will be 0.
 * @param[in]	  KA_array			key attribute array. It contains all the supported key attributes for each scene. \n
 * 									When KA_array_len is 0, KA_array should be ignored.\n
 * 									If sameKA is 1, KA_array is a one-dimensional array with length indicated by 'KA_array_len'.\n
 * 									All the scenes support the same set of Key Attribute.\n
 * 									For each row of the array, the first element is the number of valid key attributes given in this row.
 * 									If a particular scene does not support any key attribute, the first element for that row will be 0.\n
 * 									If sameKA is 0, KA_array is a two-dimensional array with size KA_array_len x scene_cnt.\n
 * 									For each row of the array, the first element is the number of valid key attributes given in this row.
 * 									If a particular scene does not support any key attribute, the first element for that row will be 0.\n
 * @param[in]	slow_rfsh  status for slow refresh of Key Held Down notification. Non-zero=enable; 0=disable
 * @param[in]	valid      validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
 * @remarks  KA Array returned in the following format:
 *  v1 interface -- sameKA 1, scene_cnt 5, KA_array_len 0, KA Array is NULL	\n
 *  V2 interface or above --													\n
 * eg 1.																		\n
 * sameKA 1, scene_cnt 5, KA_array_len 4										\n
 *  KA_array: [3] [0] [1] [2] 												\n
 * 																			\n
 * eg 2.																		\n
 * sameKA 0, scene_cnt 5, KA_array_len 4										\n
 * KA_array: [3] [0] [1] [2]													\n
 * 			 [2] [0] [5] [0]		//Only have 2 valid KA in this row			\n
 * 			 [0] [0] [0] [0]		//No valid KA in this row					\n
 * 			 [3] [0] [3] [5]													\n
 * 			 [1] [1] [0] [0]		//Only have 1 valid KA in this row			\n
 * @return        Nothing
 */
static void zwp_avi_interfaces_central_scene_supported_report_handler(zwifd_p ifd, uint8_t scene_cnt, uint8_t sameKA, uint8_t KA_array_len, uint8_t *KA_array, uint8_t slow_rfsh, int valid)
{
	zwp_avi_interfaces_central_scene_state_t *state;
	zwifd_p desc_interface;
	int i, scene_ka_count;
	zwp_avi_interfaces_key_attribute_t *key_attri_per_scene;

	if (zwp_net_acquire_desc_interface(ifd, &desc_interface) != ZWP_STATUS_OK)
	{
		goto l_err_acquire_desc;
	}

	if ((state = (zwp_avi_interfaces_central_scene_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}
	ZWP_LOG(ZWP_LOG_DEBUG, "valid = %d, scene_cnt = %d, sameKA = %d, KA_array_len = %d, KA_array = %p",
		valid, scene_cnt, sameKA, KA_array_len, KA_array);

	if (valid)
	{
		//If there is already data stored in the ka_list, clear them.
		zwp_avi_interfaces_scene_ka_list_free(state->sup_report.scene_ka_list);

		//Store the incoming data
		state->sup_report.utime = zwu_time_now();

		state->sup_report.scene_cnt = scene_cnt;
		state->sup_report.sameKA = sameKA;

		if (KA_array_len > 0 && KA_array != NULL)
		{
			if (sameKA)
			{
				scene_ka_count = 1;
			}
			else
			{
				scene_ka_count = scene_cnt;
			}

			for (i = 0; i < scene_ka_count; i++)
			{
				if ((key_attri_per_scene = (zwp_avi_interfaces_key_attribute_t *)ZWP_CALLOC(1, sizeof(zwp_avi_interfaces_key_attribute_t))) == NULL)
				{
					ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
					goto l_err_key_attri_scene_alloc;
				}

				if (zwu_array_list_add(state->sup_report.scene_ka_list, key_attri_per_scene) != ZWP_STATUS_OK)
				{
					goto l_err_ka_list_add;
				}

				key_attri_per_scene->sceneNo = i + 1;	//Scene number start from 1
				key_attri_per_scene->sup_ka_cnt = KA_array[i * KA_array_len];

				if (key_attri_per_scene->sup_ka_cnt > 0)
				{
					if ((key_attri_per_scene->ka_list = (uint8_t *)ZWP_CALLOC(1, sizeof(uint8_t) * key_attri_per_scene->sup_ka_cnt)) == NULL)
					{
						ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
						goto l_err_key_attri_alloc;
					}
					memcpy(key_attri_per_scene->ka_list, &KA_array[i * KA_array_len + 1], key_attri_per_scene->sup_ka_cnt);
				}

				key_attri_per_scene = NULL;
			}
		}
	}
	zwp_interface_report_receive_done(&state->sup_report.report_receive_state, state->sup_report.report_receive_condition);

	zwp_net_release_desc_interface(ifd);

	return;

l_err_key_attri_alloc:
l_err_ka_list_add:
	if (key_attri_per_scene)
	{
		ZWP_FREE(key_attri_per_scene);
		key_attri_per_scene = NULL;
	}
	zwp_avi_interfaces_scene_ka_list_free(state->sup_report.scene_ka_list);
l_err_key_attri_scene_alloc:
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
static zwp_status_t zwp_avi_interfaces_central_scene_supported_get_report_receive_assertion(zwifd_p desc_interface, void *args,
		zwp_thread_cond_t *report_receive_condition_p)
{
	zwp_avi_interfaces_central_scene_state_t *state;

	if ((state = (zwp_avi_interfaces_central_scene_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	*report_receive_condition_p = state->sup_report.report_receive_condition;

	if (zwp_interface_report_receive_assertion(state->sup_report.report_receive_state) != ZWP_STATUS_OK)
	{
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
static zwp_status_t zwp_avi_interfaces_central_scene_supported_get_execute_active(zwifd_p desc_interface, void *args, int *hcapi_code_p)
{
	zwp_avi_interfaces_central_scene_state_t *state;

	if ((state = (zwp_avi_interfaces_central_scene_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	zwp_interface_report_receive_init(&state->sup_report.report_receive_state);

	*hcapi_code_p = zwif_csc_sup_get(desc_interface, zwp_avi_interfaces_central_scene_supported_report_handler, 0);

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
 * The caller should call zwp_avi_interfaces_central_scene_supported_get_result_free() to free resources
 */
static zwp_status_t zwp_avi_interfaces_central_scene_supported_get_execute_passive(zwifd_p desc_interface, void *args, void **result_p)
{
	zwp_avi_interfaces_central_scene_state_t *state;
	zwp_avi_interfaces_central_scene_supported_report_t *sup_report;
	zwp_avi_interfaces_key_attribute_t *key_attri_per_scene, *key_attri_per_scene_src;
	int i, scene_ka_count;

	if ((state = (zwp_avi_interfaces_central_scene_state_t *)zwp_interface_state_get(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get state");
		goto l_err_get_state;
	}

	if ((sup_report = (zwp_avi_interfaces_central_scene_supported_report_t *)ZWP_CALLOC(1, sizeof(*sup_report))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_report_alloc;
	}

	//Basically do a deep copy of the supported report in "state" to another supported report struct
	memcpy(sup_report, &state->sup_report, sizeof(*sup_report));

	if (zwu_array_list_create(0, &sup_report->scene_ka_list) != ZWP_STATUS_OK)
	{
		goto l_err_value_list_create;
	}

	scene_ka_count = zwu_array_list_size(state->sup_report.scene_ka_list);

	for (i = 0; i < scene_ka_count; i++)
	{
		key_attri_per_scene_src = zwu_array_list_get(state->sup_report.scene_ka_list, i);

		if ((key_attri_per_scene = (zwp_avi_interfaces_key_attribute_t *)ZWP_CALLOC(1, sizeof(zwp_avi_interfaces_key_attribute_t))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_key_attri_scene_alloc;
		}

		memcpy(key_attri_per_scene, key_attri_per_scene_src, sizeof(zwp_avi_interfaces_key_attribute_t));

		if (key_attri_per_scene->sup_ka_cnt > 0)
		{
			if ((key_attri_per_scene->ka_list = (uint8_t *)ZWP_CALLOC(1, sizeof(uint8_t) * key_attri_per_scene->sup_ka_cnt)) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				/* Free 'key_attri_per_scene' as zwu_array_list_add() will not be called */
				ZWP_FREE(key_attri_per_scene);
				goto l_err_key_attri_alloc;
			}
			memcpy(key_attri_per_scene->ka_list, key_attri_per_scene_src->ka_list, key_attri_per_scene->sup_ka_cnt);
		}

		if (zwu_array_list_add(sup_report->scene_ka_list, key_attri_per_scene) != ZWP_STATUS_OK)
		{
			goto l_err_ka_list_add;
		}
	}

	*result_p = sup_report;

	return ZWP_STATUS_OK;
l_err_ka_list_add:
l_err_key_attri_alloc:
l_err_key_attri_scene_alloc:
l_err_value_list_create:
	zwp_avi_interfaces_scene_ka_list_free(sup_report->scene_ka_list);
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
static zwp_status_t zwp_avi_interfaces_central_scene_supported_get_result(void *result, avro_value_t *arg_value)
{
	zwp_avi_interfaces_central_scene_supported_report_t *report;
	zwp_avi_interfaces_key_attribute_t *key_attri_per_scene;
	int i, j, scene_ka_count;
	avro_value_t sup_scene_ka_list_value;
	avro_value_t sup_scene_ka_value;

	avro_value_t sup_ka_list_value;
	avro_value_t sup_ka_value;

	report = (zwp_avi_interfaces_central_scene_supported_report_t *)result;

	if ((zw_avro_field_set_long(arg_value, "utime", report->utime) != ZWP_STATUS_OK) ||
		(zw_avro_field_set_int(arg_value, "scene_count", report->scene_cnt) != ZWP_STATUS_OK) ||
		(zw_avro_field_set_int(arg_value, "same_key_attribute", report->sameKA) != ZWP_STATUS_OK))
	{
		goto l_err_set_report_value;
	}

	scene_ka_count = zwu_array_list_size(report->scene_ka_list);

	if (scene_ka_count > 0)
	{
		if (avro_value_get_by_name(arg_value, "scene_ka_list", &sup_scene_ka_list_value, NULL) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'scene_ka_list': %s", avro_strerror());
			goto l_err_get_list_value;
		}

		for (i = 0; i < scene_ka_count; i++)
		{
			key_attri_per_scene = zwu_array_list_get(report->scene_ka_list, i);

			if (avro_value_append(&sup_scene_ka_list_value, &sup_scene_ka_value, 0) != 0)
			{
				goto l_err_append_sup_scene_ka_value;
			}

			if (zw_avro_field_set_int(&sup_scene_ka_value, "scene_number", key_attri_per_scene->sceneNo) != ZWP_STATUS_OK)
			{
				goto l_err_set_scene_ka_report_value;
			}

			if (avro_value_get_by_name(&sup_scene_ka_value, "supported_key_attributes", &sup_ka_list_value, NULL) != 0)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'supported_key_attributes': %s", avro_strerror());
				goto l_err_get_list_value;
			}

			for (j = 0; j < key_attri_per_scene->sup_ka_cnt; j++)
			{
				if (avro_value_append(&sup_ka_list_value, &sup_ka_value, 0) != 0)
				{
					goto l_err_append_sup_ka_list_value;
				}

				if (avro_value_set_int(&sup_ka_value, key_attri_per_scene->ka_list[j]) != 0)
				{
					ZWP_LOG(ZWP_LOG_ERR, "Failed to set value '%d' to the field sup_modes : %s", key_attri_per_scene->ka_list[j], avro_strerror);
					goto l_err_set_value;
				}
			}
		}
	}

	return ZWP_STATUS_OK;

l_err_set_value:
l_err_append_sup_ka_list_value :
l_err_set_scene_ka_report_value :
l_err_append_sup_scene_ka_value:
l_err_get_list_value :
l_err_set_report_value :

	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free Result object corresponding to Supported Report command
 * @param[in]     result            Result object
 * @return        Nothing
 * @see zwp_avi_interfaces_central_scene_supported_get_execute_passive
 */
static void zwp_avi_interfaces_central_scene_supported_get_result_free(void *result)
{
	zwp_avi_interfaces_central_scene_supported_report_t *sup_report;

	sup_report = (zwp_avi_interfaces_central_scene_supported_report_t *)result;

	zwp_avi_interfaces_scene_ka_list_free(sup_report->scene_ka_list);

	ZWP_FREE(sup_report);
}

/** Handlers for Get command */
//No active get for this interface as there is no Get command for this command class
static zwp_avi_message_interfaces_t interfaces_central_scene_get =
{
		"interface_central_scene_get",
		COMMAND_CLASS_CENTRAL_SCENE, zwp_avi_interfaces_central_scene_setup,
		zwp_avi_interfaces_central_scene_state_alloc, zwp_avi_interfaces_central_scene_state_free,
		NULL, NULL,
		zwp_avi_interfaces_central_scene_get_report_receive_assertion,
		NULL, zwp_avi_interfaces_central_scene_get_execute_passive,
		zwp_avi_interfaces_central_scene_get_result, zwp_avi_interfaces_central_scene_get_result_free
};

/** Handlers for Supported Get command */
static zwp_avi_message_interfaces_t interfaces_central_scene_supported_get =
{
		"interface_central_scene_supported_get",
		COMMAND_CLASS_CENTRAL_SCENE, NULL,
		NULL, NULL,
		NULL, NULL,
		zwp_avi_interfaces_central_scene_supported_get_report_receive_assertion,
		zwp_avi_interfaces_central_scene_supported_get_execute_active, zwp_avi_interfaces_central_scene_supported_get_execute_passive,
		zwp_avi_interfaces_central_scene_supported_get_result, zwp_avi_interfaces_central_scene_supported_get_result_free
};

/** Register command handlers */
zwp_status_t zwp_avi_interfaces_central_scene_init(zwu_array_list_t message_list)
{
	if ((zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_central_scene_get) != ZWP_STATUS_OK)
		||(zwp_avi_message_list_add_message_interfaces(message_list, &interfaces_central_scene_supported_get) != ZWP_STATUS_OK))
	{
		goto l_err_interface_list_get;
	}

	return ZWP_STATUS_OK;

l_err_interface_list_get:
	return ZWP_STATUS_ERROR;
}
