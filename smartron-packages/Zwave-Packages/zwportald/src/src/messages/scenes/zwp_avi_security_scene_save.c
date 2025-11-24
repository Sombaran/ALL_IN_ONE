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
 * @file    zwp_avi_security_scene_save.c
 *
 * @brief   AVI message 'security_scene_save'
 * @details Handles the AVI message 'security_scene_save'.
 *
 * @author  Arif
 *
 * @version 1.0 - 2015-05-20
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

/** Scene save data structure*/
typedef struct {
    zwscn_security_scene_t	param;					/**< Parameters such as security scene id, name, active */
	int                 	event_arm_count;		/**< Number of elements in arm event array*/
	int                 	event_disarm_count;		/**< Number of elements in disarm event array*/
    int                 	event_alarm_count;		/**< Number of elements in alarm event array*/
	zwscn_security_event_t	*event_arm;				/**< Arm Security Event array*/
	zwscn_security_event_t	*event_disarm;			/**< Disarm Security Event array*/
    zwscn_security_event_t	*event_alarm;			/**< Alarm Security Event array*/
} zwscn_save_t;

/**
zwp_avi_security_scene_get_event_list - Convert the security scene get details event list from AVRO to data structure
@param[in]	event_list_value   AVRO encoded event list
@param[in]	event_cnt          Number of elements in event list
@param[out]	event	           Output event
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_security_scene_get_event_list(avro_value_t *event_list_value, int event_cnt, zwscn_security_event_t *event, char *event_type)
{
    int32_t     scene_evtype;
    int32_t     scene_cmd;
    int32_t     scene_ifd;
	size_t      param_cnt;
	int         i;
    char        *scene_uri;
	avro_value_t event_value;
	avro_value_t param_list_value;

    for (i=0; i<event_cnt; i++)
	{
		if (avro_value_get_by_index(event_list_value, i, &event_value, NULL) != 0)
		{
			return ZWP_STATUS_ERROR;
		}

		if ((zw_avro_field_get_int(&event_value, event_type, &scene_evtype) != ZWP_STATUS_OK)
				|| (zw_avro_field_get_int(&event_value, security_scene_arg_cmd, &scene_cmd) != ZWP_STATUS_OK)
				|| (zw_avro_field_get_int(&event_value, security_scene_arg_ifd, &scene_ifd) != ZWP_STATUS_OK))
		{
			return ZWP_STATUS_ERROR;
		}

        //Check whether parameter list exists
        if (avro_value_get_by_name(&event_value, "param_list", &param_list_value, NULL) != 0)
        {
            ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'param_list': %s", avro_strerror());
            return ZWP_STATUS_ERROR;
        }

        if (avro_value_get_size(&param_list_value, &param_cnt) != 0)
        {
            ZWP_LOG(ZWP_LOG_ERR, "Failed to get param list size");
            return ZWP_STATUS_ERROR;
        }

        if (zw_avro_field_get_string(&event_value, security_scene_arg_uri, &scene_uri) != ZWP_STATUS_OK)
        {
            return ZWP_STATUS_ERROR;
        }

        //Save to data structure
        event[i].type = (uint8_t)scene_evtype;
        event[i].cmd = (uint8_t)scene_cmd;
        event[i].id = zwp_ifd_2_scn_act_evt_id((uint32_t)scene_ifd);

        ZWP_LOG(ZWP_LOG_DEBUG, "Event URI:%s, ifd:%x", scene_uri, scene_ifd);

        zw_avro_field_free_string(scene_uri);


        //Process parameter list
        if (param_cnt > 0)
        {
            uint16_t cmd_cls;

            cmd_cls = (uint16_t)((scene_ifd >> 16) & 0xFFFF);

            if (zwp_avi_param_list_get(&param_list_value, param_cnt, cmd_cls, scene_cmd, &event[i].device_args, &event[i].device_args_size) != ZWP_STATUS_OK)
            {
                ZWP_LOG(ZWP_LOG_ERR, "Failed to get parameter list");
                return ZWP_STATUS_ERROR;
            }
        } else {
			ZWP_LOG(ZWP_LOG_ERR, "No parameter found");
			return ZWP_STATUS_ERROR;
		}

        //Reset event value
        avro_value_reset(&event_value);
	}

    return ZWP_STATUS_OK;
}


/**
zwp_avi_security_scene_save_argument_handler - AVRO argument parser
@param[in]	net	            Network
@param[in]	arg_value	    Parameters encoded in AVRO
@param[out]	args_p	        Decoded AVRO parameters
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_security_scene_save_argument_handler(avro_value_t *arg_value, void **args_p)
{
    zwscn_save_t    *scn_save_arg;
    char            *scene_name = NULL;
    int32_t         scene_active;
    int             i;
	size_t          event_arm_count;
	size_t          event_disarm_count;
	size_t          event_alarm_count;
	avro_value_t    event_list_arm_value;
	avro_value_t    event_list_disarm_value;
	avro_value_t    event_list_alarm_value;
	int32_t         temp_var = 0;
	char            *scene_notification_sms_number = NULL;
	char            *scene_notification_email = NULL;

    ZWP_LOG(ZWP_LOG_DEBUG, "INPUT ARGUMENT HANDLER - security_scene_save");

    if ((scn_save_arg = (zwscn_save_t *)ZWP_CALLOC(1, sizeof(zwscn_save_t))) == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
        return ZWP_STATUS_ERROR;
    }

    //----------------------------------------------------------------------------------------
    //  Get parameters
    //----------------------------------------------------------------------------------------
	scn_save_arg->param.is_notification_on = 1; //default value - if not provided by the API just set to 1
    if (zw_avro_field_get_int(arg_value, security_scene_arg_scened, (int32_t *)(&(scn_save_arg->param.id))) != ZWP_STATUS_OK)
    {
        goto l_SCN_SAVE_ERR1;
    }
    if (zw_avro_field_get_string(arg_value, security_scene_arg_name, &(scene_name)) != ZWP_STATUS_OK)
    {
        goto l_SCN_SAVE_ERR1;
    }
    if (zw_avro_field_get_int(arg_value, security_scene_arg_active, &scene_active) == ZWP_STATUS_OK)
    {
        //optional param
        scn_save_arg->param.active = (uint8_t)scene_active;
    }
	temp_var = 0;
	if (zw_avro_field_get_int(arg_value, security_scene_arg_is_armed, &temp_var) == ZWP_STATUS_OK)
    {
        //optional param
        scn_save_arg->param.is_armed = (uint8_t)temp_var;
    }
	temp_var = 0;
	if (zw_avro_field_get_int(arg_value, security_scene_arg_scene_id_at_arm, &temp_var) == ZWP_STATUS_OK)
    {
        //optional param
        scn_save_arg->param.scene_id_at_arm = temp_var;
    }
	temp_var = 0;
	if (zw_avro_field_get_int(arg_value, security_scene_arg_scene_id_at_disarm, &temp_var) == ZWP_STATUS_OK)
    {
        //optional param
        scn_save_arg->param.scene_id_at_disarm = temp_var;
    }
	temp_var = 0;
	if (zw_avro_field_get_int(arg_value, security_scene_arg_scene_id_at_alarm, &temp_var) == ZWP_STATUS_OK)
    {
        //optional param
        scn_save_arg->param.scene_id_at_alarm = temp_var;
    }
	temp_var = 0;
	if (zw_avro_field_get_int(arg_value, security_scene_arg_is_notification_on, &temp_var) == ZWP_STATUS_OK)
    {
        //optional param
        scn_save_arg->param.is_notification_on = (uint8_t)temp_var;
    }
	temp_var = 0;
	if (zw_avro_field_get_int(arg_value, security_scene_arg_is_notification_by_sms_on, &temp_var) == ZWP_STATUS_OK)
    {
        //optional param
        scn_save_arg->param.is_notification_by_sms_on = (uint8_t)temp_var;
    }
	temp_var = 0;
	if (zw_avro_field_get_int(arg_value, security_scene_arg_is_notification_by_email_on, &temp_var) == ZWP_STATUS_OK)
    {
        //optional param
        scn_save_arg->param.is_notification_by_email_on = (uint8_t)temp_var;
    }
	if (zw_avro_field_get_string(arg_value, security_scene_arg_notification_sms_number, &(scene_notification_sms_number)) == ZWP_STATUS_OK)
    {
		//optional param
        if(scene_notification_sms_number) {
			strncpy((char *)scn_save_arg->param.notification_sms_number, scene_notification_sms_number, sizeof(scn_save_arg->param.notification_sms_number));
			scn_save_arg->param.notification_sms_number[sizeof(scn_save_arg->param.notification_sms_number) - 1] = '\0';
			zw_avro_field_free_string(scene_notification_sms_number);
		}
    }
	if (zw_avro_field_get_string(arg_value, security_scene_arg_notification_email, &(scene_notification_email)) == ZWP_STATUS_OK)
    {
		//optional param
        if(scene_notification_email) {
			strncpy((char *)scn_save_arg->param.notification_email, scene_notification_email, sizeof(scn_save_arg->param.notification_email));
			scn_save_arg->param.notification_email[sizeof(scn_save_arg->param.notification_email) - 1] = '\0';
			zw_avro_field_free_string(scene_notification_email);
		}
    }

    if (scene_name)
    {
        strncpy((char *)scn_save_arg->param.name, scene_name, sizeof(scn_save_arg->param.name));
        scn_save_arg->param.name[sizeof(scn_save_arg->param.name) - 1] = '\0';
        zw_avro_field_free_string(scene_name);
    }

	//----------------------------------------------------------------------------------------
    // Process arm event list
    //----------------------------------------------------------------------------------------
    if (avro_value_get_by_name(arg_value, "security_scene_event_list_arm", &event_list_arm_value, NULL) != 0)
    {
        ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'security_scene_event_list_arm': %s", avro_strerror());
        goto l_SCN_SAVE_ERR1;
    }

    if (avro_value_get_size(&event_list_arm_value, &event_arm_count) != 0)
    {
        ZWP_LOG(ZWP_LOG_ERR, "Failed to get arm event list size");
        goto l_SCN_SAVE_ERR1;
    }

    if (event_arm_count > 0)
    {
        //Allocate buffer
        if ((scn_save_arg->event_arm = (zwscn_security_event_t *)ZWP_CALLOC(event_arm_count, sizeof(zwscn_security_event_t))) == NULL)
        {
            goto l_SCN_SAVE_ERR1;
        }

        scn_save_arg->event_arm_count = event_arm_count;

        if (zwp_avi_security_scene_get_event_list(&event_list_arm_value, event_arm_count, scn_save_arg->event_arm, "event_arm") != ZWP_STATUS_OK)
        {
            goto l_SCN_SAVE_ERR2;
        }
    }
	
    //----------------------------------------------------------------------------------------
    // Process disarm event list
    //----------------------------------------------------------------------------------------
    if (avro_value_get_by_name(arg_value, "security_scene_event_list_disarm", &event_list_disarm_value, NULL) != 0)
    {
        ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'security_scene_event_list_disarm': %s", avro_strerror());
        goto l_SCN_SAVE_ERR2;
    }

    if (avro_value_get_size(&event_list_disarm_value, &event_disarm_count) != 0)
    {
        ZWP_LOG(ZWP_LOG_ERR, "Failed to get disarm event list size");
        goto l_SCN_SAVE_ERR2;
    }

    if (event_disarm_count > 0)
    {
        //Allocate buffer
        if ((scn_save_arg->event_disarm = (zwscn_security_event_t *)ZWP_CALLOC(event_disarm_count, sizeof(zwscn_security_event_t))) == NULL)
        {
            goto l_SCN_SAVE_ERR2;
        }

        scn_save_arg->event_disarm_count = event_disarm_count;

        if (zwp_avi_security_scene_get_event_list(&event_list_disarm_value, event_disarm_count, scn_save_arg->event_disarm, "event_disarm") != ZWP_STATUS_OK)
        {
            goto l_SCN_SAVE_ERR3;
        }
    }
	
    //----------------------------------------------------------------------------------------
    // Process alarm event list
    //----------------------------------------------------------------------------------------
    if (avro_value_get_by_name(arg_value, "security_scene_event_list_alarm", &event_list_alarm_value, NULL) != 0)
    {
        ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'security_scene_event_list_alarm': %s", avro_strerror());
        goto l_SCN_SAVE_ERR3;
    }

    if (avro_value_get_size(&event_list_alarm_value, &event_alarm_count) != 0)
    {
        ZWP_LOG(ZWP_LOG_ERR, "Failed to get alarm event list size");
        goto l_SCN_SAVE_ERR3;
    }

    if (event_alarm_count > 0)
    {
        //Allocate buffer
        if ((scn_save_arg->event_alarm = (zwscn_security_event_t *)ZWP_CALLOC(event_alarm_count, sizeof(zwscn_security_event_t))) == NULL)
        {
            goto l_SCN_SAVE_ERR3;
        }

        scn_save_arg->event_alarm_count = event_alarm_count;

        if (zwp_avi_security_scene_get_event_list(&event_list_alarm_value, event_alarm_count, scn_save_arg->event_alarm, "event_alarm") != ZWP_STATUS_OK)
        {
            goto l_SCN_SAVE_ERR4;
        }
    }

    //ZWP_LOG(ZWP_LOG_DEBUG, "Assigning parsed arguments %p", scn_save_arg);
    *args_p = scn_save_arg;

    return ZWP_STATUS_OK;


l_SCN_SAVE_ERR4:
    for (i=0; i<scn_save_arg->event_alarm_count; i++)
    {
        ZWP_FREE(scn_save_arg->event_alarm[i].device_args);
    }
    ZWP_FREE(scn_save_arg->event_alarm);

l_SCN_SAVE_ERR3:
	for (i=0; i<scn_save_arg->event_disarm_count; i++)
    {
        ZWP_FREE(scn_save_arg->event_disarm[i].device_args);
    }
    ZWP_FREE(scn_save_arg->event_disarm);

l_SCN_SAVE_ERR2:
	for (i=0; i<scn_save_arg->event_arm_count; i++)
    {
        ZWP_FREE(scn_save_arg->event_arm[i].device_args);
    }
    ZWP_FREE(scn_save_arg->event_arm);

l_SCN_SAVE_ERR1:
//	if(scn_save_arg->param.lt_arm.event && scn_save_arg->param.lt_arm.event->device_args) {
//		ZWP_FREE(scn_save_arg->param.lt_arm.event->device_args);
//	}
//	if(scn_save_arg->param.lt_disarm.event && scn_save_arg->param.lt_disarm.event->device_args) {
//		ZWP_FREE(scn_save_arg->param.lt_disarm.event->device_args);
//	}
//	if(scn_save_arg->param.lt_alarm_on.event && scn_save_arg->param.lt_alarm_on.event->device_args) {
//		ZWP_FREE(scn_save_arg->param.lt_alarm_on.event->device_args);
//	}
//	if(scn_save_arg->param.lt_alarm_off.event && scn_save_arg->param.lt_alarm_off.event->device_args) {
//		ZWP_FREE(scn_save_arg->param.lt_alarm_off.event->device_args);
//	}
//	ZWP_FREE(scn_save_arg->param.lt_arm.event);
//	ZWP_FREE(scn_save_arg->param.lt_disarm.event);
//	ZWP_FREE(scn_save_arg->param.lt_alarm_on.event);
//	ZWP_FREE(scn_save_arg->param.lt_alarm_off.event);
    ZWP_FREE(scn_save_arg);
    return ZWP_STATUS_ERROR;
}


/**
zwp_avi_security_scene_save_argument_free - Free the data structure allocated in zwp_avi_XXX_argument_handler
@param[in]	args	            Data structure to be freed
@return
*/
static void zwp_avi_security_scene_save_argument_free(void *args)
{
    zwscn_save_t    *scn_save_arg = (zwscn_save_t *)args;
    int             i;

    //ZWP_LOG(ZWP_LOG_DEBUG, "Free arguments %p", args);

    if (scn_save_arg)
    {
        for (i=0; i<scn_save_arg->event_alarm_count; i++)
        {
            ZWP_FREE(scn_save_arg->event_alarm[i].device_args);
        }
        ZWP_FREE(scn_save_arg->event_alarm);
		for (i=0; i<scn_save_arg->event_disarm_count; i++)
        {
            ZWP_FREE(scn_save_arg->event_disarm[i].device_args);
        }
        ZWP_FREE(scn_save_arg->event_disarm);
		for (i=0; i<scn_save_arg->event_arm_count; i++)
        {
            ZWP_FREE(scn_save_arg->event_arm[i].device_args);
        }
        ZWP_FREE(scn_save_arg->event_arm);
        
		ZWP_FREE(scn_save_arg);
    }
}


/**
zwp_avi_security_scene_save_execute - Invoke library function to execute request
@param[in]	net	            Network
@param[in]	arg	            Parameters
@param[out]	result_p	    Result of the execution
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_security_scene_save_execute(zwp_net_t net, void *arg, void **result_p)
{
    zwscn_save_t    *scn_save_arg = (zwscn_save_t *)arg;
	int i;
	int result = 1;
    uint32_t        *scene_id;
//    uint32_t        tmp_id;

    //Initialize to NULL in case it is freed later on
    *result_p = NULL;

    ZWP_LOG(ZWP_LOG_DEBUG, "Execute - security_scene_save");

    if (scn_save_arg == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, "arg is NULL");
        return ZWP_STATUS_ERROR;
    }
	
/******************************/
	for(i=0; i < scn_save_arg->event_arm_count; ++i) {
		result = zwp_avi_scene_event_validate_device_args(net, scn_save_arg->event_arm[i]);
		if(!result) {
			ZWP_LOG(ZWP_LOG_ERR, "INVALID args\n");
			return ZWP_STATUS_ERROR;
		}
	}
	for(i=0; i < scn_save_arg->event_disarm_count; ++i) {
		result = zwp_avi_scene_event_validate_device_args(net, scn_save_arg->event_disarm[i]);
		if(!result) {
			ZWP_LOG(ZWP_LOG_ERR, "INVALID args\n");
			return ZWP_STATUS_ERROR;
		}
	}
	for(i=0; i < scn_save_arg->event_alarm_count; ++i) {
		result = zwp_avi_scene_event_validate_device_args(net, scn_save_arg->event_alarm[i]);
		if(!result) {
			ZWP_LOG(ZWP_LOG_ERR, "INVALID args\n");
			return ZWP_STATUS_ERROR;
		}
	}
/******************************/

    if ((scene_id = (uint32_t *)ZWP_CALLOC(1, sizeof(uint32_t))) == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
        return ZWP_STATUS_ERROR;
    }

//
//    //Check whether to create or edit a security scene
//    if (scn_save_arg->param.id == 0)
//    {   //Create a new security scene
//        if (zwscn_add_security_scene(net, &scn_save_arg->param, scene_id) != ZWSCN_ERR_NONE)
//        {
//            goto l_SCN_CREATE_EXE_ERR1;
//        }
//    }
//    else
//    {   //Edit security scene
//        *scene_id = scn_save_arg->param.id;
//        if (zwscn_edit_security_scene(net, scn_save_arg->param.id, &scn_save_arg->param) != ZWSCN_ERR_NONE)
//        {
//            goto l_SCN_CREATE_EXE_ERR1;
//        }
//
//		//Remove all events
//        if ((zwscn_del_all_arm_security_events(net, *scene_id) != ZWSCN_ERR_NONE)
//            || (zwscn_del_all_disarm_security_events(net, *scene_id) != ZWSCN_ERR_NONE)
//            || (zwscn_del_all_alarm_security_events(net, *scene_id) != ZWSCN_ERR_NONE))
//        {
//            goto l_SCN_CREATE_EXE_ERR1;
//        }
//    }
//
//	//Add arm event
//    for (i=0; i<scn_save_arg->event_arm_count; i++)
//    {
//        if (zwscn_add_arm_security_event(net, *scene_id, &scn_save_arg->event_arm[i], &tmp_id) != ZWSCN_ERR_NONE)
//        {
//            goto l_SCN_CREATE_EXE_ERR2;
//        }
//    }
//	//Add disarm event
//    for (i=0; i<scn_save_arg->event_disarm_count; i++)
//    {
//        if (zwscn_add_disarm_security_event(net, *scene_id, &scn_save_arg->event_disarm[i], &tmp_id) != ZWSCN_ERR_NONE)
//        {
//            goto l_SCN_CREATE_EXE_ERR2;
//        }
//    }
//    //Add alarm event
//    for (i=0; i<scn_save_arg->event_alarm_count; i++)
//    {
//        if (zwscn_add_alarm_security_event(net, *scene_id, &scn_save_arg->event_alarm[i], &tmp_id) != ZWSCN_ERR_NONE)
//        {
//            goto l_SCN_CREATE_EXE_ERR2;
//        }
//    }
	if(zwscn_save_security_scene(net, &scn_save_arg->param, 
									scn_save_arg->event_arm, scn_save_arg->event_arm_count,
									scn_save_arg->event_disarm, scn_save_arg->event_disarm_count,
									scn_save_arg->event_alarm, scn_save_arg->event_alarm_count,
									scene_id) != ZWSCN_ERR_NONE) {
		goto l_SCN_CREATE_EXE_ERR1;
	}

    *result_p = scene_id;

    //ZWP_LOG(ZWP_LOG_DEBUG, "Result pointer:%p", scene_id);

    return ZWP_STATUS_OK;


//l_SCN_CREATE_EXE_ERR2:
//    if (scn_save_arg->param.id == 0)
//        zwscn_del_security_scene(net, *scene_id);
l_SCN_CREATE_EXE_ERR1:
    ZWP_FREE(scene_id);
    return ZWP_STATUS_ERROR;
}


/**
zwp_avi_security_scene_save_result - Encode the security scene save response to AVRO format
@param[in]	result	            Result
@param[out]	arg_value	        Output in AVRO object
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_security_scene_save_result(void *result, avro_value_t *arg_value)
{
    uint32_t scene_desc;

    if (!result)
    {
        ZWP_LOG(ZWP_LOG_ERR, "result parameter is NULL");
        return ZWP_STATUS_ERROR;
    }

    scene_desc = *((uint32_t *)result);
    if (zw_avro_field_set_int(arg_value, security_scene_arg_desc, (int32_t)scene_desc) != ZWP_STATUS_OK)
    {
        return ZWP_STATUS_ERROR;
    }

    //ZWP_LOG(ZWP_LOG_DEBUG, "scene_save desc : %u", (unsigned)scene_desc);

    return ZWP_STATUS_OK;

}


/**
zwp_avi_security_scene_save_result_free - Free the result_p from zwp_avi_XXX_execute()
@param[in]	result	            Result pointer
@return
*/
static void zwp_avi_security_scene_save_result_free(void *result)
{
    //ZWP_LOG(ZWP_LOG_DEBUG, "Result pointer:%p", result);
    ZWP_FREE(result);
}


/** Scene save AVRO message handler registration data structure */
static zwp_avi_message_networks_t security_scene_save =
{
    "security_scene_save",								/**< AVRO message name*/
    zwp_avi_security_scene_save_argument_handler,		/**< Command specific AVRO argument parser; NULL if no such arguments */
    zwp_avi_security_scene_save_argument_free,			/**< Free argument */
    zwp_avi_security_scene_save_execute,				/**< Invoke library function to execute request*/
    ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,			/**< Flag to control execution based on validity of network */
    zwp_avi_security_scene_save_result,					/**< Encode the response to AVRO format*/
    zwp_avi_security_scene_save_result_free				/**< Free the result_p from zwp_avi_XXX_execute()*/
};


/**
zwp_avi_security_scene_save_init - Scene save initialization
@param[in]	message_list	    Message list
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_avi_security_scene_save_init(zwu_array_list_t message_list)
{
    if (zwp_avi_message_list_add_message_networks(message_list, &security_scene_save) != ZWP_STATUS_OK)
    {
        return ZWP_STATUS_ERROR;
    }

    return ZWP_STATUS_OK;
}

