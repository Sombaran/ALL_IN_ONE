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
 * @file    zws_avi_security_scene_common.c
 *
 * @brief   Common file for Security Scenes AVI messages
 * @details Common file for Security Scenes AVI messages.
 *
 * @author  Arif
 *
 * @version 1.0 - 2015-05-15
 * - Initial version
 */
#include "zws_status.h"
#include "zws_avi_message_networks.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zws_avi_scene.h"
#include "zws_avi_security_scene_common.h"
 
const char security_scene_tag[] 							= "security_scene";
const char security_scene_tag_close[] 						= "</security_scene>";
const char security_scene_tag_lt_alarm_on[] 				= "lt_alarm_on";
const char security_scene_arg_scened[] 						= "scened";
const char security_scene_arg_name[] 						= "name";
const char security_scene_arg_active[] 						= "active";
const char security_scene_arg_is_armed[] 					= "is_armed";
const char security_scene_arg_is_alarmed[] 					= "is_alarmed";
const char security_scene_arg_scene_id_at_arm[] 			= "scene_id_at_arm";
const char security_scene_arg_scene_id_at_disarm[] 			= "scene_id_at_disarm";
const char security_scene_arg_scene_id_at_alarm[] 			= "scene_id_at_alarm";
const char security_scene_arg_is_notification_on[] 			= "is_notification_on";
const char security_scene_arg_is_notification_by_sms_on[] 	= "is_notification_by_sms_on";
const char security_scene_arg_is_notification_by_email_on[] = "is_notification_by_email_on";
const char security_scene_arg_notification_sms_number[]		= "notification_sms_number";
const char security_scene_arg_notification_email[]			= "notification_email";
const char security_scene_arg_desc[]						= "desc";
const char security_scene_arg_ifd[]							= "ifd";
const char security_scene_arg_cmd[]							= "cmd";
const char security_scene_arg_uri[]							= "uri";
const char security_scene_arg_status[]						= "status";
const char security_scene_arg_sutime[]						= "sutime";
const char security_scene_arg_arm[]							= "arm";
const char security_scene_arg_disarm[]						= "disarm";
const char security_scene_arg_alarm[]						= "alarm";
const char security_scene_arg_type[]						= "type";
const char security_scene_arg_time[]						= "time";
const char security_scene_arg_counter[]						= "counter";
const char security_scene_arg_max_scenes[]   				= "max_security_scenes";
const char security_scene_arg_max_arm_events[]  			= "max_security_arm_events";
const char security_scene_arg_max_disarm_events[]			= "max_security_disarm_events";
const char security_scene_arg_max_alarm_events[]   			= "max_security_alarm_events"; 
const char security_scene_arg_event[]   					= "event";
const char security_scene_arg_utime[]						= "utime";
const char security_scene_arg_notification[] 				= "notification";
const char security_scene_arg_sms[] 						= "sms";
const char security_scene_arg_email[] 						= "email";
const char security_scene_arg_number[] 						= "number";
const char security_scene_arg_address[] 					= "address";
const char security_scene_arg_on[] 							= "on";

/**
zws_avi_security_scene_generic_arg_handler - Convert the security scene generic request from POST data to AVRO format
@param[in]	args	            Buffer to store key-value pairs
@param[in]	args_count	        Number of key-value pairs in args
@param[out]	arg_value_p	        Output in AVRO object
@return ZWS_STATUS_XXX
*/
zws_status_t zws_avi_security_scene_generic_arg_handler(char *(*args)[2], int  args_count, avro_value_t *arg_value_p)
{
    if (args_count > 0)
    {
        if (strcmp(args[0][0], security_scene_arg_scened) == 0)
        {
            //Check for empty "value"
            if (*(args[0][1]) == '\0')
            {
                return ZWS_STATUS_ERROR;
            }

            if (zw_avro_field_set_int(arg_value_p, security_scene_arg_scened, atoi(args[0][1])) == ZWS_STATUS_OK)
            {
                return ZWS_STATUS_OK;
            }
        }
    }
    return ZWS_STATUS_ERROR;
}

/**
zws_avi_security_scene_generic_resp - Convert the scene generic response from AVRO to XML format
@param[in]	web_api_version	    Web API version
@param[in]	arg_value	        AVRO encoded response
@param[out]	result	            Output in XML format
@return ZWS_STATUS_XXX
*/
zws_status_t zws_avi_security_scene_generic_resp(int web_api_version, avro_value_t *arg_value, zwu_string_buffer_t result)
{
    int32_t scene_desc;

    //This function is called twice for different API versions. We ignore the second call to avoid duplicate results.
    if (web_api_version == ZWS_WEB_API_VERSION_2)
    {
        return ZWS_STATUS_OK;
    }

    if (zw_avro_field_get_int(arg_value, "desc", &scene_desc) != ZWS_STATUS_OK)
    {
        return ZWS_STATUS_ERROR;
    }

    return zwu_string_buffer_append_format(result,
                                        "<%s desc=\"%u\" />",
                                        security_scene_tag,
                                        scene_desc);
}

zws_status_t zws_avi_security_scene_get_event(avro_value_t *event_value, zwu_string_buffer_t result, const char *event_type)
{
	int32_t         scene_evtype;
    int32_t         scene_status;
    int32_t         scene_cmd;
    int32_t         scene_ifd;
//    int32_t         epid;
	size_t          param_cnt;
//	int             i;
    char            *scene_uri;
	int64_t         scene_sutime;
    zws_status_t    res;
	avro_value_t    param_list_value;
	
	if ((zw_avro_field_get_int(event_value, event_type, &scene_evtype) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(event_value, "status", &scene_status) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_long(event_value, "sutime", &scene_sutime) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(event_value, "cmd", &scene_cmd) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(event_value, "ifd", &scene_ifd) != ZWS_STATUS_OK))
		{
			return ZWS_STATUS_ERROR;
		}

        //Check whether parameter list exists
        if (avro_value_get_by_name(event_value, "param_list", &param_list_value, NULL) != 0)
        {
            ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'param_list': %s", avro_strerror());
            return ZWS_STATUS_ERROR;
        }

        if (avro_value_get_size(&param_list_value, &param_cnt) != 0)
        {
            ZWS_LOG(ZWS_LOG_ERR, "Failed to get param list size");
            return ZWS_STATUS_ERROR;
        }

        if (zw_avro_field_get_string(event_value, "uri", &scene_uri) != ZWS_STATUS_OK)
        {
            return ZWS_STATUS_ERROR;
        }

//Done on zwportal side now
//        //Adjust for endpoint id
//        epid = (scene_ifd >> 8) & 0xFF;
//        scene_ifd = (scene_ifd & 0xFFFF00FF) | (((epid + 1) & 0xFF) << 8);

        res = zwu_string_buffer_append_format(result,
                                            "<%s evtype=\"%u\" status=\"%u\" sutime=\"%llu\" "
                                            "uri=\"%s\" cmd=\"%u\" "
                                            "ifd=\"%u\"%s",
											security_scene_arg_event,
                                            (unsigned)scene_evtype,
                                            (unsigned)scene_status,
                                            (unsigned long long int)scene_sutime,
                                            scene_uri,
                                            (unsigned)scene_cmd,
                                            (unsigned)scene_ifd,
                                            (param_cnt > 0)? " " : " />"
                                            );

        zw_avro_field_free_string(scene_uri);

        if (res != ZWS_STATUS_OK)
        {
            return ZWS_STATUS_ERROR;
        }

        //Process parameter list
        if (param_cnt > 0)
        {
            if (zws_avi_param_list_to_xml(&param_list_value, param_cnt, result) != ZWS_STATUS_OK)
            {
                return ZWS_STATUS_ERROR;
            }
        }
		
		return ZWS_STATUS_OK;
}

zws_status_t zws_avi_security_scene_get_last_trigger(avro_value_t *scene_value, const char *avro_name, const char *tag_name, zwu_string_buffer_t result) {
	avro_value_t    lt_value;
	avro_value_t    event_value;
	int32_t temp_type = 0;
	int64_t temp_time = 0;
	
	if (avro_value_get_by_name(scene_value, avro_name, &lt_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field '%s': %s", avro_name, avro_strerror());
		return ZWS_STATUS_ERROR;
	}
	
	if ((zw_avro_field_get_int(&lt_value, security_scene_arg_type, &temp_type) != ZWS_STATUS_OK)
		|| (zw_avro_field_get_long(&lt_value, security_scene_arg_time, &temp_time) != ZWS_STATUS_OK))
	{
		return ZWS_STATUS_ERROR;
	}
	
	if (zwu_string_buffer_append_format(result, "<%s type=\"%u\" time=\"%llu\" >",
			tag_name,
			(unsigned)temp_type,
			(unsigned long long int)temp_time
			) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}
	
	if((temp_type == ZWSCN_STATELOG_TRIGGER_EVENT) && (temp_time !=0)) {
		if (avro_value_get_by_name(&lt_value, security_scene_arg_event, &event_value, NULL) != 0)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field '%s': %s", security_scene_arg_event, avro_strerror());
			return ZWS_STATUS_ERROR;
		}
//			if(event_value != NULL) {
			zws_avi_security_scene_get_event(&event_value, result, "event_alarm");
//			}
	}
	
	if (zwu_string_buffer_append_format(result, "</%s>", tag_name) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}
	
	return ZWS_STATUS_OK;
l_err_append:
	return ZWS_STATUS_ERROR;
}
