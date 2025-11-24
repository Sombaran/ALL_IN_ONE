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
 * @file    zwp_avi_security_scene_list.c
 *
 * @brief   AVI message 'security_scene_list'
 * @details Handles the AVI message 'security_scene_list'.
 *
 * @author  Arif
 *
 * @version 1.0 - 2015-05-15
 * - Initial version
 */

#include "zwp_status.h"
#include "zwp_avi_message_networks.h"
#include "zwp_log.h"
#include "zwp_memory.h"
#include "zwp_error.h"
#include "zwp_hcapi.h"
#include "zwp_descriptor.h"
#include "zwscn_security_scene_api.h"
#include "zwp_avi_scene.h"
#include "zwp_avi_scene_common.h"
#include "zwp_avi_security_scene_common.h"


/** Scene list data structure*/
typedef struct {
    zwscn_security_scene_t	*scene;          /**< Pointer to array of security scenes */
    int						scene_cnt;       /**< Number of elements in security scene array*/
} zwscn_lst_t;


/**
zwp_avi_security_scene_list_execute - Invoke library function to execute request
@param[in]	net	            Network
@param[in]	arg	            Parameters
@param[out]	result_p	    Result of the execution
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_security_scene_list_execute(zwp_net_t net, void *arg, void **result_p)
{
    zwscn_lst_t    *scn_lst;
    int             scene_count;
    int             i;
    zwscn_security_scene_p   scene_ptr;

    //Initialize to NULL in case it is freed later on
    *result_p = NULL;

    ZWP_LOG(ZWP_LOG_DEBUG, "Execute - security_scene_list");

    if ((scn_lst = (zwscn_lst_t *)ZWP_CALLOC(1, sizeof(zwscn_lst_t))) == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
        return ZWP_STATUS_ERROR;
    }

    //Allocate memory for scenes
    scn_lst->scene_cnt = scene_count = zwscn_count_security_scenes(net);

    if (scene_count > 0)
    {
        if ((scn_lst->scene = (zwscn_security_scene_t *)ZWP_CALLOC(scene_count, sizeof(zwscn_security_scene_t))) == NULL)
        {
            ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
            goto l_SCN_LST_EXE_ERR1;
        }

        //Get scene list
        for (i=0; i<scene_count; i++)
        {
            if (zwscn_get_security_scene_by_index(net, i, &scene_ptr) == ZWSCN_ERR_NONE)
            {
//                scn_lst->scene[i] = *scene_ptr;
//                ZWP_FREE(scene_ptr);
				zwscn_security_scene_copy(&(scn_lst->scene[i]), scene_ptr);
				zwscn_security_scene_free(scene_ptr);
            }
            else
            {
                goto l_SCN_LST_EXE_ERR2;
            }
        }
    }

    //Save the result
    *result_p = scn_lst;

    return ZWP_STATUS_OK;

l_SCN_LST_EXE_ERR2:
//    ZWP_FREE(scn_lst->scene);
	zwscn_security_scene_free(scene_ptr);

l_SCN_LST_EXE_ERR1:
    ZWP_FREE(scn_lst);
    return ZWP_STATUS_ERROR;
}


/**
zwp_avi_security_scene_list_result - Encode the scene list response to AVRO format
@param[in]	result	            Result
@param[out]	arg_value	        Output in AVRO object
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_security_scene_list_result(void *result, avro_value_t *arg_value)
{
    zwscn_lst_t    *scn_lst = (zwscn_lst_t *)result;
    int             i;
    avro_value_t    scene_list_value;
    avro_value_t    scene_value;

    if (!result)
    {
        ZWP_LOG(ZWP_LOG_ERR, "result parameter is NULL\n");
        return ZWP_STATUS_ERROR;
    }

    if (avro_value_get_by_name(arg_value, "SecuritySceneList", &scene_list_value, NULL) != 0)
    {
        ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'SecuritySceneList': %s", avro_strerror());
        return ZWP_STATUS_ERROR;
    }

    for (i=0; i<scn_lst->scene_cnt; i++)
    {
        if (avro_value_append(&scene_list_value, &scene_value, 0) != 0)
        {
            return ZWP_STATUS_ERROR;
        }
		
        if ((zw_avro_field_set_int(&scene_value, security_scene_arg_scened, scn_lst->scene[i].id) != ZWP_STATUS_OK)
            || (zw_avro_field_set_string(&scene_value, security_scene_arg_name, (char *)scn_lst->scene[i].name) != ZWP_STATUS_OK)
            || (zw_avro_field_set_int(&scene_value, security_scene_arg_active, scn_lst->scene[i].active) != ZWP_STATUS_OK)
            || (zw_avro_field_set_int(&scene_value, security_scene_arg_is_armed, scn_lst->scene[i].is_armed) != ZWP_STATUS_OK)
            || (zw_avro_field_set_int(&scene_value, security_scene_arg_is_alarmed, scn_lst->scene[i].is_alarmed) != ZWP_STATUS_OK)
            || (zw_avro_field_set_int(&scene_value, security_scene_arg_scene_id_at_arm, scn_lst->scene[i].scene_id_at_arm) != ZWP_STATUS_OK)
            || (zw_avro_field_set_int(&scene_value, security_scene_arg_scene_id_at_disarm, scn_lst->scene[i].scene_id_at_disarm) != ZWP_STATUS_OK)
            || (zw_avro_field_set_int(&scene_value, security_scene_arg_scene_id_at_alarm, scn_lst->scene[i].scene_id_at_alarm) != ZWP_STATUS_OK)
            || (zw_avro_field_set_long(&scene_value, security_scene_arg_utime, scn_lst->scene[i].utime) != ZWP_STATUS_OK))
        {
            return ZWP_STATUS_ERROR;
        }
		
//		if (avro_value_get_by_name(&scene_value, "lt_arm", &lt_arm_value, NULL) != 0)
//		{
//			ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'lt_arm': %s", avro_strerror());
//			return ZWP_STATUS_ERROR;
//		}
//		
//		if ((zw_avro_field_set_int(&lt_arm_value, security_scene_arg_type, temp_type) != ZWP_STATUS_OK)
//            || (zw_avro_field_set_long(&lt_arm_value, security_scene_arg_time, temp_time) != ZWP_STATUS_OK))
//        {
//            return ZWP_STATUS_ERROR;
//        }
		
//		if (avro_value_get_by_name(&scene_value, "lt_alarm_on", &lt_alarm_on_value, NULL) != 0)
//		{
//			ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'lt_alarm_on': %s", avro_strerror());
//			return ZWP_STATUS_ERROR;
//		}
//		
//		temp_type = scn_lst->scene[i].lt_alarm_on.type;
//		temp_time = scn_lst->scene[i].lt_alarm_on.time;
//		ZWP_LOG(ZWP_LOG_DEBUG, "[%d] lt_alarm_on.type=%d lt_alarm_on.time=%llu", i, temp_type, temp_time);
//		if ((zw_avro_field_set_int(&lt_alarm_on_value, security_scene_arg_type, temp_type) != ZWP_STATUS_OK)
//            || (zw_avro_field_set_long(&lt_alarm_on_value, security_scene_arg_time, temp_time) != ZWP_STATUS_OK))
//        {
//            return ZWP_STATUS_ERROR;
//        }
//		
//		if((temp_type == ZWSCN_STATELOG_TRIGGER_EVENT) && (temp_time != 0) && (scn_lst->scene[i].lt_alarm_on.event != NULL)) {
//			//Get URI
//            cmd_cls = (uint16_t)(((scn_lst->scene[i].lt_alarm_on.event->id)) & 0xFFFF);
//            uri = zwp_avi_uri_get(cmd_cls);
//			
//			if (avro_value_get_by_name(&lt_alarm_on_value, security_scene_arg_event, &event_value, NULL) != 0)
//			{
//				ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field '%s': %s", security_scene_arg_event, avro_strerror());
//				return ZWP_STATUS_ERROR;
//			}
//
//			converted_event_id = zwp_scn_act_evt_id_2_ifd(scn_lst->scene[i].lt_alarm_on.event->id);
//			if ((zw_avro_field_set_int(&event_value, "event_alarm", (int32_t)scn_lst->scene[i].lt_alarm_on.event->type) != ZWP_STATUS_OK)
//				|| (zw_avro_field_set_int(&event_value, security_scene_arg_status, (int32_t)scn_lst->scene[i].lt_alarm_on.event->status) != ZWP_STATUS_OK)
//				|| (zw_avro_field_set_long(&event_value, security_scene_arg_sutime, (int64_t)scn_lst->scene[i].lt_alarm_on.event->lrtime) != ZWP_STATUS_OK)
//				|| (zw_avro_field_set_string(&event_value, security_scene_arg_uri, (char *)uri) != ZWP_STATUS_OK)
//				|| (zw_avro_field_set_int(&event_value, security_scene_arg_cmd, (int32_t)scn_lst->scene[i].lt_alarm_on.event->cmd) != ZWP_STATUS_OK)
//				|| (zw_avro_field_set_int(&event_value, security_scene_arg_ifd, (int32_t)converted_event_id) != ZWP_STATUS_OK))
//			{
//				return ZWP_STATUS_ERROR;
//			}
//
//			if (scn_lst->scene[i].lt_alarm_on.event->device_args)
//			{
//				if (zwp_avi_param_to_avro(scn_lst->scene[i].lt_alarm_on.event->device_args, cmd_cls, scn_lst->scene[i].lt_alarm_on.event->cmd, &event_value) != ZWP_STATUS_OK)
//				{
//					return ZWP_STATUS_ERROR;
//				}
//			}

			if(zwp_avi_security_scene_get_last_trigger(&scene_value, "lt_alarm_on", &(scn_lst->scene[i].lt_alarm_on)) != ZWP_STATUS_OK) {
				return ZWP_STATUS_ERROR;
			}
    }

    return ZWP_STATUS_OK;
}


/**
zwp_avi_security_scene_list_result_free - Free the result_p from zwp_avi_XXX_execute()
@param[in]	result	            Result pointer
@return
*/
static void zwp_avi_security_scene_list_result_free(void *result)
{
    zwscn_lst_t    *scn_lst = (zwscn_lst_t *)result;

    if (scn_lst)
    {
		if(scn_lst->scene) {
			if(scn_lst->scene->lt_arm.event && scn_lst->scene->lt_arm.event->device_args) {
				ZWP_FREE(scn_lst->scene->lt_arm.event->device_args);
			}
			if(scn_lst->scene->lt_disarm.event && scn_lst->scene->lt_disarm.event->device_args) {
				ZWP_FREE(scn_lst->scene->lt_disarm.event->device_args);
			}
			if(scn_lst->scene->lt_alarm_on.event && scn_lst->scene->lt_alarm_on.event->device_args) {
				ZWP_FREE(scn_lst->scene->lt_alarm_on.event->device_args);
			}
			if(scn_lst->scene->lt_alarm_off.event && scn_lst->scene->lt_alarm_off.event->device_args) {
				ZWP_FREE(scn_lst->scene->lt_alarm_off.event->device_args);
			}
			ZWP_FREE(scn_lst->scene->lt_arm.event);
			ZWP_FREE(scn_lst->scene->lt_disarm.event);
			ZWP_FREE(scn_lst->scene->lt_alarm_on.event);
			ZWP_FREE(scn_lst->scene->lt_alarm_off.event);
			ZWP_FREE(scn_lst->scene);
		}
        ZWP_FREE(scn_lst);
    }
}


/** Scene list AVRO message handler registration data structure */
static zwp_avi_message_networks_t security_scene_list =
{
    "security_scene_list",							/**< AVRO message name*/
    NULL, NULL,
    zwp_avi_security_scene_list_execute,			/**< Invoke library function to execute request*/
    ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,		/**< Flag to control execution based on validity of network */
    zwp_avi_security_scene_list_result,				/**< Encode the response to AVRO format*/
    zwp_avi_security_scene_list_result_free			/**< Free the result_p from zwp_avi_XXX_execute()*/
};


/**
zwp_avi_security_scene_list_init - Scene list initialization
@param[in]	message_list	    Message list
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_avi_security_scene_list_init(zwu_array_list_t message_list)
{
    if (zwp_avi_message_list_add_message_networks(message_list, &security_scene_list) != ZWP_STATUS_OK)
    {
        return ZWP_STATUS_ERROR;
    }

    return ZWP_STATUS_OK;
}
