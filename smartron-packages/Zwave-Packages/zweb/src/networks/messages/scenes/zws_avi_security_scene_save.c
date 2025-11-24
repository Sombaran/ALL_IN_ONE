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
 * @file    zws_avi_security_scene_save.c
 *
 * @brief   AVI message 'security_scene_save'
 * @details Handles the AVI message 'security_scene_save'.
 *
 * @author  Arif
 *
 * @version 1.0 - 2015-05-20
 * - Initial version
 */
#include "zws_status.h"
#include "zws_avi_message_networks.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zws_avi_scene.h"
#include "zws_avi_security_scene_common.h"

/** Scene Save POST data current section*/
#define SCENE_SAVE_ARG_SECTION_NONE			0   /**< Not in any section*/
#define SCENE_SAVE_ARG_SECTION_EVENT_ARM	1   /**< In "Arm Event" section*/
#define SCENE_SAVE_ARG_SECTION_EVENT_DISARM	2   /**< In "Disarm Event" section*/
#define SCENE_SAVE_ARG_SECTION_EVENT_ALARM	3   /**< In "Alarm Event" section*/

/** POST data current sub-section*/
#define ARG_SUB_SECTION_FIELD           0   /**< Mandatory field that belongs to the section*/
#define ARG_SUB_SECTION_PARAM           1   /**< Optional parameter that belongs to the section*/


/**
zws_avi_security_scene_save_event - Convert the security scene save request "event" section from POST data to AVRO format
@param[in]	    key	                Key of the field
@param[in]	    value	            Value corresponding to the key
@param[in, out]	arg_sub_section	    Current state of the sub-section (ARG_SUB_SECTION_XXX)
@param[out]	    event_value	    Output in AVRO object
@return ZWS_STATUS_XXX
*/
static zws_status_t zws_avi_security_scene_save_event(char *key, char *value, int *arg_sub_section, avro_value_t *event_value)
{
	avro_value_t param_list_value;
	avro_value_t param_value;

    if (*arg_sub_section == ARG_SUB_SECTION_FIELD)
    {
        if (strcmp(key, security_scene_arg_uri) == 0)
        {
            return zw_avro_field_set_string(event_value, "uri", value);
        }
        else if (strcmp(key, security_scene_arg_cmd) == 0)
        {
            return zw_avro_field_set_int(event_value, "cmd", atoi(value));
        }
        else if (strcmp(key, security_scene_arg_ifd) == 0)
        {
//            int32_t epid;
            int32_t scene_ifd = atoi(value);

//Done on zwportal side now
//            //Adjust for endpoint id
//            epid = (scene_ifd >> 8) & 0xFF;
//            scene_ifd = (scene_ifd & 0xFFFF00FF) | (((epid - 1) & 0xFF) << 8);

            return zw_avro_field_set_int(event_value, security_scene_arg_ifd, scene_ifd);
        }
        else
        {
            //Optional parameters
            *arg_sub_section = ARG_SUB_SECTION_PARAM;
        }
    }

    if (avro_value_get_by_name(event_value, "param_list", &param_list_value, NULL) != 0)
    {
        ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'param_list': %s", avro_strerror());
        return ZWS_STATUS_ERROR;
    }

    if (avro_value_append(&param_list_value, &param_value, 0) != 0)
    {
        return ZWS_STATUS_ERROR;
    }

    if (zw_avro_field_set_string(&param_value, "key", key) == ZWS_STATUS_OK)
    {
        if (zw_avro_field_set_string(&param_value, "value", value) == ZWS_STATUS_OK)
        {
            return ZWS_STATUS_OK;
        }
    }
    return ZWS_STATUS_ERROR;

}
//
//
///**
//zws_avi_security_scene_save_schedule - Convert the security scene save request "schedule" section from POST data to AVRO format
//@param[in]	key	                Key of the field
//@param[in]	value	            Value corresponding to the key
//@param[out]	schedule_value	    Output in AVRO object
//@return ZWS_STATUS_XXX
//*/
//static zws_status_t zws_avi_security_scene_save_schedule(char *key, char *value, avro_value_t *schedule_value)
//{
//    if (strcmp(key, "day") == 0)
//    {
//        return zw_avro_field_set_int(schedule_value, "day", atoi(value));
//    }
//    else if (strcmp(key, "hour") == 0)
//    {
//        return zw_avro_field_set_int(schedule_value, "hour", atoi(value));
//    }
//    else if (strcmp(key, "minute") == 0)
//    {
//        return zw_avro_field_set_int(schedule_value, "minute", atoi(value));
//    }
//    return ZWS_STATUS_ERROR;
//}


/**
zws_avi_security_scene_save_argument_handler - Convert the security scene save request from POST data to AVRO format
@param[in]	args	            Buffer to store key-value pairs
@param[in]	args_count	        Number of key-value pairs in args
@param[out]	arg_value_p	        Output in AVRO object
@return ZWS_STATUS_XXX
*/
static zws_status_t zws_avi_security_scene_save_argument_handler(char *(*args)[2], int  args_count, avro_value_t *arg_value_p)
{
    int          i;
    int          arg_section = SCENE_SAVE_ARG_SECTION_NONE;
    int          arg_sub_section;
//    int          have_action;
//	avro_value_t action_list_value;
//	avro_value_t schedule_list_value;
	avro_value_t event_list_arm_value;
	avro_value_t event_list_disarm_value;
	avro_value_t event_list_alarm_value;
//	avro_value_t action_value = {0};
//	avro_value_t schedule_value = {0};
	avro_value_t event_arm_value = {0};
	avro_value_t event_disarm_value = {0};
	avro_value_t event_alarm_value = {0};

	ZWS_LOG(ZWS_LOG_ERR, "args_count: %d", args_count);
    //Check for empty "value"
    for (i=0; i<args_count; i++)
    {
//		ZWS_LOG(ZWS_LOG_ERR, "key: %s, value:%s", args[i][0], args[i][1]);
        if (*(args[i][1]) == '\0')
        {
            return ZWS_STATUS_ERROR;
        }
    }


    i = 0;

    if ((i < args_count) && !strcmp(args[i][0], security_scene_arg_scened)
        && (zw_avro_field_set_int(arg_value_p, security_scene_arg_scened, atoi(args[i][1])) == ZWS_STATUS_OK))
    {
        i++;
    }
    else
    {
        return ZWS_STATUS_ERROR; //No Scene without a id
    }

    if ((i < args_count) && !strcmp(args[i][0], security_scene_arg_name)
        && (zw_avro_field_set_string(arg_value_p, security_scene_arg_name, args[i][1]) == ZWS_STATUS_OK))
    {
        i++;
    }
    else
    {
        return ZWS_STATUS_ERROR; //No Scene without a name
    }

    if ((i < args_count) && !strcmp(args[i][0], security_scene_arg_active)
        && (zw_avro_field_set_int(arg_value_p, security_scene_arg_active, atoi(args[i][1])) == ZWS_STATUS_OK))
    {
        i++;
    }
    else
    {
        return ZWS_STATUS_ERROR; //No Scene without an active triggers
    }
	
	if ((i < args_count) && !strcmp(args[i][0], security_scene_arg_is_armed)
        && (zw_avro_field_set_int(arg_value_p, security_scene_arg_is_armed, atoi(args[i][1])) == ZWS_STATUS_OK))
    {
        i++;
    }
	if ((i < args_count) && !strcmp(args[i][0], security_scene_arg_scene_id_at_arm)
        && (zw_avro_field_set_int(arg_value_p, security_scene_arg_scene_id_at_arm, atoi(args[i][1])) == ZWS_STATUS_OK))
    {
        i++;
    }
	if ((i < args_count) && !strcmp(args[i][0], security_scene_arg_scene_id_at_disarm)
        && (zw_avro_field_set_int(arg_value_p, security_scene_arg_scene_id_at_disarm, atoi(args[i][1])) == ZWS_STATUS_OK))
    {
        i++;
    }
	if ((i < args_count) && !strcmp(args[i][0], security_scene_arg_scene_id_at_alarm)
        && (zw_avro_field_set_int(arg_value_p, security_scene_arg_scene_id_at_alarm, atoi(args[i][1])) == ZWS_STATUS_OK))
    {
        i++;
    }
	if ((i < args_count) && !strcmp(args[i][0], security_scene_arg_is_notification_on)
        && (zw_avro_field_set_int(arg_value_p, security_scene_arg_is_notification_on, atoi(args[i][1])) == ZWS_STATUS_OK))
    {
        i++;
    }
	if ((i < args_count) && !strcmp(args[i][0], security_scene_arg_is_notification_by_sms_on)
        && (zw_avro_field_set_int(arg_value_p, security_scene_arg_is_notification_by_sms_on, atoi(args[i][1])) == ZWS_STATUS_OK))
    {
        i++;
    }
	if ((i < args_count) && !strcmp(args[i][0], security_scene_arg_is_notification_by_email_on)
        && (zw_avro_field_set_int(arg_value_p, security_scene_arg_is_notification_by_email_on, atoi(args[i][1])) == ZWS_STATUS_OK))
    {
        i++;
    }
	if ((i < args_count) && !strcmp(args[i][0], security_scene_arg_notification_sms_number)
        && (zw_avro_field_set_string(arg_value_p, security_scene_arg_notification_sms_number, args[i][1]) == ZWS_STATUS_OK))
    {
        i++;
    }
	if ((i < args_count) && !strcmp(args[i][0], security_scene_arg_notification_email)
        && (zw_avro_field_set_string(arg_value_p, security_scene_arg_notification_email, args[i][1]) == ZWS_STATUS_OK))
    {
        i++;
    }

//    //Get action, schedule and event lists
//	if (avro_value_get_by_name(arg_value_p, "scene_action_list", &action_list_value, NULL) != 0)
//	{
//		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'scene_action_list': %s", avro_strerror());
//        return ZWS_STATUS_ERROR;
//	}
//
//	if (avro_value_get_by_name(arg_value_p, "scene_schedule_list", &schedule_list_value, NULL) != 0)
//	{
//		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'scene_schedule_list': %s", avro_strerror());
//        return ZWS_STATUS_ERROR;
//	}
//
	if (avro_value_get_by_name(arg_value_p, "security_scene_event_list_arm", &event_list_arm_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'security_scene_event_list_arm': %s", avro_strerror());
        return ZWS_STATUS_ERROR;
	}
	if (avro_value_get_by_name(arg_value_p, "security_scene_event_list_disarm", &event_list_disarm_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'security_scene_event_list_disarm': %s", avro_strerror());
        return ZWS_STATUS_ERROR;
	}
	if (avro_value_get_by_name(arg_value_p, "security_scene_event_list_alarm", &event_list_alarm_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'security_scene_event_list_alarm': %s", avro_strerror());
        return ZWS_STATUS_ERROR;
	}

    // Process action, schedule and event

//    have_action = 0;
//
    while (i < args_count)
    {
//        if(strcmp(args[i][0], "action") == 0)
//        {
//            arg_section = SCENE_SAVE_ARG_SECTION_ACTION;
//            arg_sub_section = ARG_SUB_SECTION_FIELD;
//            have_action = 1;
//
//            if (avro_value_append(&action_list_value, &action_value, 0) != 0)
//            {
//                return ZWS_STATUS_ERROR;
//            }
//
//            if (zw_avro_field_set_int(&action_value, "action", atoi(args[i][1])) != ZWS_STATUS_OK)
//            {
//                return ZWS_STATUS_ERROR;
//            }
//            i++;
//            continue;
//        }
//        else if(strcmp(args[i][0], "schedule") == 0)
//        {
//            arg_section = SCENE_SAVE_ARG_SECTION_SCHED;
//            arg_sub_section = ARG_SUB_SECTION_FIELD;
//
//            if (avro_value_append(&schedule_list_value, &schedule_value, 0) != 0)
//            {
//                return ZWS_STATUS_ERROR;
//            }
//
//            if (zw_avro_field_set_int(&schedule_value, "schedule", atoi(args[i][1])) != ZWS_STATUS_OK)
//            {
//                return ZWS_STATUS_ERROR;
//            }
//            i++;
//            continue;
//        }
//        else 

		if(strcmp(args[i][0], "event_arm") == 0)
        {
            arg_section = SCENE_SAVE_ARG_SECTION_EVENT_ARM;
            arg_sub_section = ARG_SUB_SECTION_FIELD;

            if (avro_value_append(&event_list_arm_value, &event_arm_value, 0) != 0)
            {
                return ZWS_STATUS_ERROR;
            }

            if (zw_avro_field_set_int(&event_arm_value, "event_arm", atoi(args[i][1])) != ZWS_STATUS_OK)
            {
                return ZWS_STATUS_ERROR;
            }
            i++;
            continue;
        }
		else if(strcmp(args[i][0], "event_disarm") == 0)
        {
            arg_section = SCENE_SAVE_ARG_SECTION_EVENT_DISARM;
            arg_sub_section = ARG_SUB_SECTION_FIELD;

            if (avro_value_append(&event_list_disarm_value, &event_disarm_value, 0) != 0)
            {
                return ZWS_STATUS_ERROR;
            }

            if (zw_avro_field_set_int(&event_disarm_value, "event_disarm", atoi(args[i][1])) != ZWS_STATUS_OK)
            {
                return ZWS_STATUS_ERROR;
            }
            i++;
            continue;
        }
		else if(strcmp(args[i][0], "event_alarm") == 0)
        {
            arg_section = SCENE_SAVE_ARG_SECTION_EVENT_ALARM;
            arg_sub_section = ARG_SUB_SECTION_FIELD;

            if (avro_value_append(&event_list_alarm_value, &event_alarm_value, 0) != 0)
            {
                return ZWS_STATUS_ERROR;
            }

            if (zw_avro_field_set_int(&event_alarm_value, "event_alarm", atoi(args[i][1])) != ZWS_STATUS_OK)
            {
                return ZWS_STATUS_ERROR;
            }
            i++;
            continue;
        }


        switch (arg_section)
        {
            case SCENE_SAVE_ARG_SECTION_NONE:
                ZWS_LOG(ZWS_LOG_ERR, "Unexpected field '%s' in security scene save POST data", args[i][0]);
                return ZWS_STATUS_ERROR;
                break;

//            case SCENE_SAVE_ARG_SECTION_ACTION:
//                if (zws_avi_security_scene_save_event(args[i][0], args[i][1], &arg_sub_section, &action_value) != ZWS_STATUS_OK)
//                {
//                    return ZWS_STATUS_ERROR;
//                }
//                break;
//
//            case SCENE_SAVE_ARG_SECTION_SCHED:
//                if (zws_avi_security_scene_save_schedule(args[i][0], args[i][1], &schedule_value) != ZWS_STATUS_OK)
//                {
//                    return ZWS_STATUS_ERROR;
//                }
//                break;

			case SCENE_SAVE_ARG_SECTION_EVENT_ARM:
                if (zws_avi_security_scene_save_event(args[i][0], args[i][1], &arg_sub_section, &event_arm_value) != ZWS_STATUS_OK)
                {
                    return ZWS_STATUS_ERROR;
                }
                break;
			case SCENE_SAVE_ARG_SECTION_EVENT_DISARM:
                if (zws_avi_security_scene_save_event(args[i][0], args[i][1], &arg_sub_section, &event_disarm_value) != ZWS_STATUS_OK)
                {
                    return ZWS_STATUS_ERROR;
                }
                break;
            case SCENE_SAVE_ARG_SECTION_EVENT_ALARM:
                if (zws_avi_security_scene_save_event(args[i][0], args[i][1], &arg_sub_section, &event_alarm_value) != ZWS_STATUS_OK)
                {
                    return ZWS_STATUS_ERROR;
                }
                break;
        }

        i++;
    }
//
//    return (have_action)? ZWS_STATUS_OK : ZWS_STATUS_ERROR;
	return ZWS_STATUS_OK;
}

/** Scene save command handler structure */
static zws_avi_message_networks_t security_scene_save =
{
    {scene_api_1, scene_api_2},         			/**< URI for security scene related commands*/
    CMD_SECURITY_SCENE_SAVE,                     	/**< Commands passed as value of POST parameters 'cmd=' or 'cmd_cgi=' */
    "security_scene_save",                       	/**< AVRO message name as in the protocol schema "zwave.avsc"*/
    zws_avi_security_scene_save_argument_handler,	/**< POST request to AVRO parser*/
    zws_avi_security_scene_generic_resp,          	/**< AVRO to XML converter*/
    ""                                  			/**< Error text to be sent if command fails */
};


/**
zws_avi_security_scene_save_init - initialize security scene save command handler for POST->AVRO and AVRO->XML
@param[in]	message_list	    Message list
@return ZWS_STATUS_XXX
*/
zws_status_t zws_avi_security_scene_save_init(zwu_array_list_t message_list)
{
    if (zws_avi_message_list_add_message_networks(message_list, &security_scene_save) != ZWS_STATUS_OK)
    {
        return ZWS_STATUS_ERROR;
    }

    return ZWS_STATUS_OK;
}
