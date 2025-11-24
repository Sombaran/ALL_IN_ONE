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
 * @file    zwp_avi_security_scene_get.c
 *
 * @brief   AVI message 'security_scene_get'
 * @details Handles the AVI message 'security_scene_get'.
 *
 * @author  Arif
 *
 * @version 1.0 - 2015-05-26
 * - Initial version
 */
#include "zwp_status.h"
#include "zwp_avi_message_networks.h"
#include "zwp_log.h"
#include "zwp_memory.h"
#include "zwp_error.h"
#include "zwp_hcapi.h"
#include "zwp_descriptor.h"
#include "zwscn_scene_api.h"
#include "zwscn_security_scene_api.h"
#include "zwscn_device_args.h"
#include "zwp_avi_scene.h"
#include "zwp_avi_scene_common.h"
#include "zwp_avi_security_scene_common.h"

/** Security Scene get data structure */
typedef struct {
    zwscn_security_scene_t       scene;         /**< Parameters such as security scene id, name, active */
	int                 event_arm_count;		/**< Number of elements in arm event array*/
	int                 event_disarm_count;		/**< Number of elements in disarm event array*/
    int                 event_alarm_count;		/**< Number of elements in alarm event array*/
	zwscn_security_event_t	*event_arm;			/**< Arm Security Event array*/
	zwscn_security_event_t	*event_disarm;		/**< Disarm Security Event array*/
    zwscn_security_event_t	*event_alarm;		/**< Alarm Security Event array*/
} zwscn_get_t;

/**
zwp_avi_security_scene_get_event_list - Get event list of a specified scene id
@param[in] 	net				Network runtime context
@param[in] 	scene_id		Scene id
@param[in]	event_cnt       Number of elements in event list
@param[out]	event	        Output event
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_security_scene_get_event_list(void *net, uint32_t scene_id, int event_cnt, zwscn_security_event_t *event, ZWSCN_SECURITY_SCENE_EVENT_TYPE_T event_type)
{
	int             i;
    zwscn_security_event_p   scene_event;

	for (i=0; i<event_cnt; i++)
	{
		if(event_type == ZWSCN_SECURITY_SCENE_EVENT_ARM) {
			if (zwscn_get_arm_security_event_by_index(net, scene_id, i, &scene_event) != ZWSCN_ERR_NONE)
			{
				return ZWP_STATUS_ERROR;
			}
		} else if(event_type == ZWSCN_SECURITY_SCENE_EVENT_DISARM) {
			if (zwscn_get_disarm_security_event_by_index(net, scene_id, i, &scene_event) != ZWSCN_ERR_NONE)
			{
				return ZWP_STATUS_ERROR;
			}
		} else if(event_type == ZWSCN_SECURITY_SCENE_EVENT_ALARM) {
			if (zwscn_get_alarm_security_event_by_index(net, scene_id, i, &scene_event) != ZWSCN_ERR_NONE)
			{
				return ZWP_STATUS_ERROR;
			}
		} else {
			return ZWP_STATUS_ERROR;
		}

        //Copy the scene event
        event[i] = *scene_event;
        event[i].device_args = NULL;
        event[i].device_args_size = 0;
        event[i].id = zwp_scn_act_evt_id_2_ifd(scene_event->id);

        if (scene_event->device_args && scene_event->device_args_size)
        {   //create and copy parameters
            if ((event[i].device_args = ZWP_MALLOC(scene_event->device_args_size)) == NULL)
            {
                ZWP_FREE(scene_event->device_args);
                ZWP_FREE(scene_event);
                return ZWP_STATUS_ERROR;
            }

            memcpy(event[i].device_args, scene_event->device_args, scene_event->device_args_size);
            event[i].device_args_size = scene_event->device_args_size;
            ZWP_FREE(scene_event->device_args);
        }

        ZWP_FREE(scene_event);

	}

    return ZWP_STATUS_OK;
}

/**
zwp_avi_security_scene_get_execute - Invoke library function to execute request
@param[in]	net	            Network
@param[in]	arg	            Parameters
@param[out]	result_p	    Result of the execution
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_security_scene_get_execute(zwp_net_t net, void *arg, void **result_p)
{
    int             i;
    int             element_cnt;
    zwscn_get_t     *scn_get_arg;
    zwscn_security_scene_p   scene;
    uint32_t        *scene_id = (uint32_t *)arg;

    //Initialize to NULL in case it is freed later on
    *result_p = NULL;

    ZWP_LOG(ZWP_LOG_DEBUG, "Execute - security scene_get");

    if (arg == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, "arg is NULL");
        return ZWP_STATUS_ERROR;
    }

    if ((scn_get_arg = (zwscn_get_t *)ZWP_CALLOC(1, sizeof(zwscn_get_t))) == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
        return ZWP_STATUS_ERROR;
    }

    //Get scene
    if (zwscn_get_security_scene(net, *scene_id, &scene) != ZWSCN_ERR_NONE)
    {
        goto l_SCN_GET_EXE_ERR1;
    }
//    scn_get_arg->scene = *scene;
//    ZWP_FREE(scene);
	zwscn_security_scene_copy(&(scn_get_arg->scene), scene);
	zwscn_security_scene_free(scene);

	//Get arm event
    element_cnt = zwscn_count_arm_security_events(net, *scene_id);
    if (element_cnt < 0)
    {
        goto l_SCN_GET_EXE_ERR1;
    }

    if (element_cnt > 0)
    {
        //Allocate buffer
        if ((scn_get_arg->event_arm = (zwscn_security_event_t *)ZWP_CALLOC(element_cnt, sizeof(zwscn_security_event_t))) == NULL)
        {
            goto l_SCN_GET_EXE_ERR1;
        }

        scn_get_arg->event_arm_count = element_cnt;

        if (zwp_avi_security_scene_get_event_list(net, *scene_id, element_cnt, scn_get_arg->event_arm, ZWSCN_SECURITY_SCENE_EVENT_ARM) != ZWP_STATUS_OK)
        {
            goto l_SCN_GET_EXE_ERR2;
        }
    }
	
	//Get disarm event
    element_cnt = zwscn_count_disarm_security_events(net, *scene_id);
    if (element_cnt < 0)
    {
        goto l_SCN_GET_EXE_ERR2;
    }

    if (element_cnt > 0)
    {
        //Allocate buffer
        if ((scn_get_arg->event_disarm = (zwscn_security_event_t *)ZWP_CALLOC(element_cnt, sizeof(zwscn_security_event_t))) == NULL)
        {
            goto l_SCN_GET_EXE_ERR2;
        }

        scn_get_arg->event_disarm_count = element_cnt;

        if (zwp_avi_security_scene_get_event_list(net, *scene_id, element_cnt, scn_get_arg->event_disarm, ZWSCN_SECURITY_SCENE_EVENT_DISARM) != ZWP_STATUS_OK)
        {
            goto l_SCN_GET_EXE_ERR3;
        }
    }
	
    //Get alarm event
    element_cnt = zwscn_count_alarm_security_events(net, *scene_id);
    if (element_cnt < 0)
    {
        goto l_SCN_GET_EXE_ERR3;
    }

    if (element_cnt > 0)
    {
        //Allocate buffer
        if ((scn_get_arg->event_alarm = (zwscn_security_event_t *)ZWP_CALLOC(element_cnt, sizeof(zwscn_security_event_t))) == NULL)
        {
            goto l_SCN_GET_EXE_ERR3;
        }

        scn_get_arg->event_alarm_count = element_cnt;

        if (zwp_avi_security_scene_get_event_list(net, *scene_id, element_cnt, scn_get_arg->event_alarm, ZWSCN_SECURITY_SCENE_EVENT_ALARM) != ZWP_STATUS_OK)
        {
            goto l_SCN_GET_EXE_ERR4;
        }
    }

    *result_p = scn_get_arg;

    return ZWP_STATUS_OK;

l_SCN_GET_EXE_ERR4:
    for (i=0; i<scn_get_arg->event_alarm_count; i++)
    {
        ZWP_FREE(scn_get_arg->event_alarm[i].device_args);
    }
    ZWP_FREE(scn_get_arg->event_alarm);

l_SCN_GET_EXE_ERR3:
	for (i=0; i<scn_get_arg->event_disarm_count; i++)
    {
        ZWP_FREE(scn_get_arg->event_disarm[i].device_args);
    }
    ZWP_FREE(scn_get_arg->event_disarm);
    
l_SCN_GET_EXE_ERR2:
	for (i=0; i<scn_get_arg->event_arm_count; i++)
    {
        ZWP_FREE(scn_get_arg->event_arm[i].device_args);
    }
    ZWP_FREE(scn_get_arg->event_arm);    

l_SCN_GET_EXE_ERR1:
	if(scn_get_arg->scene.lt_arm.event && scn_get_arg->scene.lt_arm.event->device_args) {
		ZWP_FREE(scn_get_arg->scene.lt_arm.event->device_args);
	}
	if(scn_get_arg->scene.lt_disarm.event && scn_get_arg->scene.lt_disarm.event->device_args) {
		ZWP_FREE(scn_get_arg->scene.lt_disarm.event->device_args);
	}
	if(scn_get_arg->scene.lt_alarm_on.event && scn_get_arg->scene.lt_alarm_on.event->device_args) {
		ZWP_FREE(scn_get_arg->scene.lt_alarm_on.event->device_args);
	}
	if(scn_get_arg->scene.lt_alarm_off.event && scn_get_arg->scene.lt_alarm_off.event->device_args) {
		ZWP_FREE(scn_get_arg->scene.lt_alarm_off.event->device_args);
	}
	ZWP_FREE(scn_get_arg->scene.lt_arm.event);
	ZWP_FREE(scn_get_arg->scene.lt_disarm.event);
	ZWP_FREE(scn_get_arg->scene.lt_alarm_on.event);
	ZWP_FREE(scn_get_arg->scene.lt_alarm_off.event);
    ZWP_FREE(scn_get_arg);
    return ZWP_STATUS_ERROR;
}


/**
zwp_avi_security_scene_get_result - Encode the "scene get" response to AVRO format
@param[in]	result	       Result
@param[out]	arg_value	   Output in AVRO object
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_security_scene_get_result(void *result, avro_value_t *arg_value)
{
    zwscn_get_t  *scn_get_arg = (zwscn_get_t *)result;
    const char   *uri;
    int          i;
	avro_value_t event_list_arm_value;
	avro_value_t event_list_disarm_value;
    avro_value_t event_list_alarm_value;
    avro_value_t scene_rpt_value;
    avro_value_t event_value = {0};
    uint16_t     cmd_cls;

    if (!result)
    {
        ZWP_LOG(ZWP_LOG_ERR, "result parameter is NULL");
        return ZWP_STATUS_ERROR;
    }

    if (avro_value_get_by_name(arg_value, "report", &scene_rpt_value, NULL) != 0)
    {
        ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'report': %s", avro_strerror());
        return ZWP_STATUS_ERROR;
    }

	if ((zw_avro_field_set_int(&scene_rpt_value, security_scene_arg_scened, scn_get_arg->scene.id) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_string(&scene_rpt_value, security_scene_arg_name, (char *)scn_get_arg->scene.name) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_int(&scene_rpt_value, security_scene_arg_active, scn_get_arg->scene.active) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_int(&scene_rpt_value, security_scene_arg_is_armed, scn_get_arg->scene.is_armed) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_int(&scene_rpt_value, security_scene_arg_is_alarmed, scn_get_arg->scene.is_alarmed) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_int(&scene_rpt_value, security_scene_arg_scene_id_at_arm, scn_get_arg->scene.scene_id_at_arm) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_int(&scene_rpt_value, security_scene_arg_scene_id_at_disarm, scn_get_arg->scene.scene_id_at_disarm) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_int(&scene_rpt_value, security_scene_arg_scene_id_at_alarm, scn_get_arg->scene.scene_id_at_alarm) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_int(&scene_rpt_value, security_scene_arg_is_notification_on, scn_get_arg->scene.is_notification_on) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_int(&scene_rpt_value, security_scene_arg_is_notification_by_sms_on, scn_get_arg->scene.is_notification_by_sms_on) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_int(&scene_rpt_value, security_scene_arg_is_notification_by_email_on, scn_get_arg->scene.is_notification_by_email_on) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_string(&scene_rpt_value, security_scene_arg_notification_sms_number, scn_get_arg->scene.notification_sms_number) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_string(&scene_rpt_value, security_scene_arg_notification_email, scn_get_arg->scene.notification_email) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_long(&scene_rpt_value, security_scene_arg_utime, scn_get_arg->scene.utime) != ZWP_STATUS_OK))
	{
		return ZWP_STATUS_ERROR;
	}
	
	if(zwp_avi_security_scene_get_last_trigger(&scene_rpt_value, "lt_arm", &(scn_get_arg->scene.lt_arm)) != ZWP_STATUS_OK) {
		return ZWP_STATUS_ERROR;
	}
	if(zwp_avi_security_scene_get_last_trigger(&scene_rpt_value, "lt_disarm", &(scn_get_arg->scene.lt_disarm)) != ZWP_STATUS_OK) {
		return ZWP_STATUS_ERROR;
	}
	if(zwp_avi_security_scene_get_last_trigger(&scene_rpt_value, "lt_alarm_on", &(scn_get_arg->scene.lt_alarm_on)) != ZWP_STATUS_OK) {
		return ZWP_STATUS_ERROR;
	}
	if(zwp_avi_security_scene_get_last_trigger(&scene_rpt_value, "lt_alarm_off", &(scn_get_arg->scene.lt_alarm_off)) != ZWP_STATUS_OK) {
		return ZWP_STATUS_ERROR;
	}

    //Get arm, disarm and alarm event lists
    if (avro_value_get_by_name(arg_value, "security_scene_event_list_arm", &event_list_arm_value, NULL) != 0)
    {
        ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'security_scene_event_list_arm': %s", avro_strerror());
        return ZWP_STATUS_ERROR;
    }

    if (avro_value_get_by_name(arg_value, "security_scene_event_list_disarm", &event_list_disarm_value, NULL) != 0)
    {
        ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'security_scene_event_list_disarm': %s", avro_strerror());
        return ZWP_STATUS_ERROR;
    }

    if (avro_value_get_by_name(arg_value, "security_scene_event_list_alarm", &event_list_alarm_value, NULL) != 0)
    {
        ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'security_scene_event_list_alarm': %s", avro_strerror());
        return ZWP_STATUS_ERROR;
    }

   
    // Process arm event
    if ((scn_get_arg->event_arm_count > 0) && scn_get_arg->event_arm)
    {
        for (i=0; i<scn_get_arg->event_arm_count; i++)
        {
            //Get URI
            cmd_cls = (uint16_t)((scn_get_arg->event_arm[i].id >> 16) & 0xFFFF);
            uri = zwp_avi_uri_get(cmd_cls);

            if (!uri)
            {
                return ZWP_STATUS_ERROR;
            }

            if (avro_value_append(&event_list_arm_value, &event_value, 0) != 0)
            {
                return ZWP_STATUS_ERROR;
            }

            if ((zw_avro_field_set_int(&event_value, "event_arm", (int32_t)scn_get_arg->event_arm[i].type) != ZWP_STATUS_OK)
                || (zw_avro_field_set_int(&event_value, security_scene_arg_status, (int32_t)scn_get_arg->event_arm[i].status) != ZWP_STATUS_OK)
                || (zw_avro_field_set_long(&event_value, security_scene_arg_sutime, (int64_t)scn_get_arg->event_arm[i].lrtime) != ZWP_STATUS_OK)
                || (zw_avro_field_set_string(&event_value, security_scene_arg_uri, (char *)uri) != ZWP_STATUS_OK)
                || (zw_avro_field_set_int(&event_value, security_scene_arg_cmd, (int32_t)scn_get_arg->event_arm[i].cmd) != ZWP_STATUS_OK)
                || (zw_avro_field_set_int(&event_value, security_scene_arg_ifd, (int32_t)scn_get_arg->event_arm[i].id) != ZWP_STATUS_OK))
            {
                return ZWP_STATUS_ERROR;
            }

            if (scn_get_arg->event_arm[i].device_args)
            {
                if (zwp_avi_param_to_avro(scn_get_arg->event_arm[i].device_args, cmd_cls, scn_get_arg->event_arm[i].cmd, &event_value) != ZWP_STATUS_OK)
                {
                    return ZWP_STATUS_ERROR;
                }
            }
        }
	}

	// Process disarm event
    if ((scn_get_arg->event_disarm_count > 0) && scn_get_arg->event_disarm)
    {
        for (i=0; i<scn_get_arg->event_disarm_count; i++)
        {
            //Get URI
            cmd_cls = (uint16_t)((scn_get_arg->event_disarm[i].id >> 16) & 0xFFFF);
            uri = zwp_avi_uri_get(cmd_cls);

            if (!uri)
            {
                return ZWP_STATUS_ERROR;
            }

            if (avro_value_append(&event_list_disarm_value, &event_value, 0) != 0)
            {
                return ZWP_STATUS_ERROR;
            }

            if ((zw_avro_field_set_int(&event_value, "event_disarm", (int32_t)scn_get_arg->event_disarm[i].type) != ZWP_STATUS_OK)
                || (zw_avro_field_set_int(&event_value, security_scene_arg_status, (int32_t)scn_get_arg->event_disarm[i].status) != ZWP_STATUS_OK)
                || (zw_avro_field_set_long(&event_value, security_scene_arg_sutime, (int64_t)scn_get_arg->event_disarm[i].lrtime) != ZWP_STATUS_OK)
                || (zw_avro_field_set_string(&event_value, security_scene_arg_uri, (char *)uri) != ZWP_STATUS_OK)
                || (zw_avro_field_set_int(&event_value, security_scene_arg_cmd, (int32_t)scn_get_arg->event_disarm[i].cmd) != ZWP_STATUS_OK)
                || (zw_avro_field_set_int(&event_value, security_scene_arg_ifd, (int32_t)scn_get_arg->event_disarm[i].id) != ZWP_STATUS_OK))
            {
                return ZWP_STATUS_ERROR;
            }

            if (scn_get_arg->event_disarm[i].device_args)
            {
                if (zwp_avi_param_to_avro(scn_get_arg->event_disarm[i].device_args, cmd_cls, scn_get_arg->event_disarm[i].cmd, &event_value) != ZWP_STATUS_OK)
                {
                    return ZWP_STATUS_ERROR;
                }
            }
        }
	}
		
    // Process alarm event
    if ((scn_get_arg->event_alarm_count > 0) && scn_get_arg->event_alarm)
    {
        for (i=0; i<scn_get_arg->event_alarm_count; i++)
        {
            //Get URI
            cmd_cls = (uint16_t)((scn_get_arg->event_alarm[i].id >> 16) & 0xFFFF);
            uri = zwp_avi_uri_get(cmd_cls);

            if (!uri)
            {
                return ZWP_STATUS_ERROR;
            }

            if (avro_value_append(&event_list_alarm_value, &event_value, 0) != 0)
            {
                return ZWP_STATUS_ERROR;
            }

            if ((zw_avro_field_set_int(&event_value, "event_alarm", (int32_t)scn_get_arg->event_alarm[i].type) != ZWP_STATUS_OK)
                || (zw_avro_field_set_int(&event_value, security_scene_arg_status, (int32_t)scn_get_arg->event_alarm[i].status) != ZWP_STATUS_OK)
                || (zw_avro_field_set_long(&event_value, security_scene_arg_sutime, (int64_t)scn_get_arg->event_alarm[i].lrtime) != ZWP_STATUS_OK)
                || (zw_avro_field_set_string(&event_value, security_scene_arg_uri, (char *)uri) != ZWP_STATUS_OK)
                || (zw_avro_field_set_int(&event_value, security_scene_arg_cmd, (int32_t)scn_get_arg->event_alarm[i].cmd) != ZWP_STATUS_OK)
                || (zw_avro_field_set_int(&event_value, security_scene_arg_ifd, (int32_t)scn_get_arg->event_alarm[i].id) != ZWP_STATUS_OK))
            {
                return ZWP_STATUS_ERROR;
            }

            if (scn_get_arg->event_alarm[i].device_args)
            {
                if (zwp_avi_param_to_avro(scn_get_arg->event_alarm[i].device_args, cmd_cls, scn_get_arg->event_alarm[i].cmd, &event_value) != ZWP_STATUS_OK)
                {
                    return ZWP_STATUS_ERROR;
                }
            }
        }
    }

    return ZWP_STATUS_OK;

}


/**
zwp_avi_security_scene_get_result_free - Free the result_p from zwp_avi_XXX_execute()
@param[in]	result	            Result pointer
@return
*/
static void zwp_avi_security_scene_get_result_free(void *result)
{
    zwscn_get_t     *scn_get_arg = (zwscn_get_t *)result;
    int             i;

    //Free the result_p from zwp_avi_security_scene_get_execute()
    if (scn_get_arg)
    {
        for (i=0; i<scn_get_arg->event_alarm_count; i++)
        {
            ZWP_FREE(scn_get_arg->event_alarm[i].device_args);
        }
        ZWP_FREE(scn_get_arg->event_alarm);

        for (i=0; i<scn_get_arg->event_disarm_count; i++)
        {
            ZWP_FREE(scn_get_arg->event_disarm[i].device_args);
        }
        ZWP_FREE(scn_get_arg->event_disarm);

        for (i=0; i<scn_get_arg->event_arm_count; i++)
        {
            ZWP_FREE(scn_get_arg->event_arm[i].device_args);
        }
        ZWP_FREE(scn_get_arg->event_arm);

		if(scn_get_arg->scene.lt_arm.event && scn_get_arg->scene.lt_arm.event->device_args) {
			ZWP_FREE(scn_get_arg->scene.lt_arm.event->device_args);
		}
		if(scn_get_arg->scene.lt_disarm.event && scn_get_arg->scene.lt_disarm.event->device_args) {
			ZWP_FREE(scn_get_arg->scene.lt_disarm.event->device_args);
		}
		if(scn_get_arg->scene.lt_alarm_on.event && scn_get_arg->scene.lt_alarm_on.event->device_args) {
			ZWP_FREE(scn_get_arg->scene.lt_alarm_on.event->device_args);
		}
		if(scn_get_arg->scene.lt_alarm_off.event && scn_get_arg->scene.lt_alarm_off.event->device_args) {
			ZWP_FREE(scn_get_arg->scene.lt_alarm_off.event->device_args);
		}
		ZWP_FREE(scn_get_arg->scene.lt_arm.event);
		ZWP_FREE(scn_get_arg->scene.lt_disarm.event);
		ZWP_FREE(scn_get_arg->scene.lt_alarm_on.event);
		ZWP_FREE(scn_get_arg->scene.lt_alarm_off.event);
        ZWP_FREE(scn_get_arg);
    }
}


/** Scene get details AVRO message handler registration data structure */
static zwp_avi_message_networks_t security_scene_get =
{
    "security_scene_get",                               /**< AVRO message name*/
    zwp_avi_scene_generic_arg_handler,         /**< Command specific AVRO argument parser; NULL if no such arguments */
    zwp_avi_scene_generic_arg_free,            /**< Free argument */
    zwp_avi_security_scene_get_execute,                 /**< Invoke library function to execute request*/
    ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,   /**< Flag to control execution based on validity of network */
    zwp_avi_security_scene_get_result,                  /**< Encode the response to AVRO format*/
    zwp_avi_security_scene_get_result_free              /**< Free the result_p from zwp_avi_XXX_execute()*/
};


/**
zwp_avi_security_scene_get_init - Scene get details initialization
@param[in]	message_list	    Message list
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_avi_security_scene_get_init(zwu_array_list_t message_list)
{
    if (zwp_avi_message_list_add_message_networks(message_list, &security_scene_get) != ZWP_STATUS_OK)
    {
        return ZWP_STATUS_ERROR;
    }

    return ZWP_STATUS_OK;
}
