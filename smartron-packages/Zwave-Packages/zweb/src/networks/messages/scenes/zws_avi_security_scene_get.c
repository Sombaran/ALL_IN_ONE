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
 * @file    zws_avi_security_scene_get.c
 *
 * @brief   AVI message 'security_scene_get'
 * @details Handles the AVI message 'security_scene_get'.
 *
 * @author  Arif
 *
 * @version 1.0 - 2015-05-21
 * - Initial version
 */
#include "zws_status.h"
#include "zws_avi_message_networks.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zws_avi_scene.h"
#include "zws_avi_security_scene_common.h"


/**
zws_avi_security_scene_get_event_list - Convert the security scene get details event list from AVRO to XML format
@param[in]	event_list_value   AVRO encoded event list
@param[in]	event_cnt          Number of elements in event list
@param[out]	result	           Output in XML format
@return ZWS_STATUS_XXX
*/
static zws_status_t zws_avi_security_scene_get_event_list(avro_value_t *event_list_value, int event_cnt, zwu_string_buffer_t result, char *event_type)
{
    int32_t         scene_evtype;
    int32_t         scene_status;
    int32_t         scene_cmd;
    int32_t         scene_ifd;
//    int32_t         epid;
	size_t          param_cnt;
	int             i;
    char            *scene_uri;
	int64_t         scene_sutime;
    zws_status_t    res;
	avro_value_t    event_value;
	avro_value_t    param_list_value;

	for (i=0; i<event_cnt; i++)
	{
		if (avro_value_get_by_index(event_list_value, i, &event_value, NULL) != 0)
		{
			return ZWS_STATUS_ERROR;
		}

		if ((zw_avro_field_get_int(&event_value, event_type, &scene_evtype) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&event_value, "status", &scene_status) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_long(&event_value, "sutime", &scene_sutime) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&event_value, "cmd", &scene_cmd) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&event_value, "ifd", &scene_ifd) != ZWS_STATUS_OK))
		{
			return ZWS_STATUS_ERROR;
		}

        //Check whether parameter list exists
        if (avro_value_get_by_name(&event_value, "param_list", &param_list_value, NULL) != 0)
        {
            ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'param_list': %s", avro_strerror());
            return ZWS_STATUS_ERROR;
        }

        if (avro_value_get_size(&param_list_value, &param_cnt) != 0)
        {
            ZWS_LOG(ZWS_LOG_ERR, "Failed to get param list size");
            return ZWS_STATUS_ERROR;
        }

        if (zw_avro_field_get_string(&event_value, "uri", &scene_uri) != ZWS_STATUS_OK)
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

        //Reset event value
        avro_value_reset(&event_value);
	}

    return ZWS_STATUS_OK;
}

/**
zws_avi_security_scene_get_result - Convert the security scene list response from AVRO to XML format
@param[in]	web_api_version	    Web API version
@param[in]	arg_value	        AVRO encoded response
@param[out]	result	            Output in XML format
@return ZWS_STATUS_XXX
*/
static zws_status_t zws_avi_security_scene_get_result(int web_api_version, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	avro_value_t    scene_get_value;
	int             scened;
	char            *name = NULL;
	int             active;
	int             is_armed;
	int             is_alarmed;
	int				scene_id_at_arm;
	int				scene_id_at_disarm;
	int				scene_id_at_alarm;
	int				is_notification_on;
	int				is_notification_by_sms_on;
	int				is_notification_by_email_on;
	char            *notification_sms_number = NULL;
	char            *notification_email = NULL;
//	int             latrigger_type;
//	int             latrigger_event_id;
//	int64_t         latrigger_time;
	zws_status_t    res;
	size_t          event_arm_count;
	size_t          event_disarm_count;
	size_t          event_alarm_count;
	avro_value_t    event_list_arm_value;
	avro_value_t    event_list_disarm_value;
	avro_value_t    event_list_alarm_value;
	int64_t         utime;
	

	//ZWS_LOG(ZWS_LOG_DEBUG, "[1] zws_avi_security_scene_get_result");
	if(web_api_version == ZWS_WEB_API_VERSION_2) { //this function is called twice for different API versions. We ignore the second call to avoid duplicate results.
			return ZWS_STATUS_OK;
	}

	//ZWS_LOG(ZWS_LOG_DEBUG, "[2] zws_avi_security_scene_get_result");
	if (avro_value_get_by_name(arg_value, "report", &scene_get_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'report': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	//ZWS_LOG(ZWS_LOG_DEBUG, "[3] zws_avi_security_scene_get_result");
	if ((zw_avro_field_get_int(&scene_get_value, security_scene_arg_scened, &scened) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_string(&scene_get_value, security_scene_arg_name, &name) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(&scene_get_value, security_scene_arg_active, &active) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(&scene_get_value, security_scene_arg_is_armed, &is_armed) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(&scene_get_value, security_scene_arg_is_alarmed, &is_alarmed) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(&scene_get_value, security_scene_arg_scene_id_at_arm, &scene_id_at_arm) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(&scene_get_value, security_scene_arg_scene_id_at_disarm, &scene_id_at_disarm) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(&scene_get_value, security_scene_arg_scene_id_at_alarm, &scene_id_at_alarm) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(&scene_get_value, security_scene_arg_is_notification_on, &is_notification_on) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(&scene_get_value, security_scene_arg_is_notification_by_sms_on, &is_notification_by_sms_on) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_int(&scene_get_value, security_scene_arg_is_notification_by_email_on, &is_notification_by_email_on) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_string(&scene_get_value, security_scene_arg_notification_sms_number, &notification_sms_number) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_string(&scene_get_value, security_scene_arg_notification_email, &notification_email) != ZWS_STATUS_OK)
			|| (zw_avro_field_get_long(&scene_get_value, security_scene_arg_utime, &utime) != ZWS_STATUS_OK))
	{
		goto l_err_get_fields;
	}

	//ZWS_LOG(ZWS_LOG_DEBUG, "[4] zws_avi_security_scene_get_result");
	if (zwu_string_buffer_append_format(result,
			"<%s desc=\"%u\" name=\"%s\" active=\"%u\" armed=\"%u\" alarmed=\"%u\" utime=\"%llu\" >",
			security_scene_tag,
			(unsigned)scened,
			name,
			(unsigned)active,
			(unsigned)is_armed,
			(unsigned)is_alarmed,
			(unsigned long long int)utime
			) != ZWS_STATUS_OK)
	{
		goto l_err_append;
	}

	//ZWS_LOG(ZWS_LOG_DEBUG, "[5] zws_avi_security_scene_get_result");
	zw_avro_field_free_string(name);
	name = NULL;
	
	//ZWS_LOG(ZWS_LOG_DEBUG, "[6] zws_avi_security_scene_get_result");
	//----------------------------------------------------------------------------------------
    // Process arm event list
    //----------------------------------------------------------------------------------------
	if (zwu_string_buffer_append_format(result, "<%s exec_scened=\"%u\" >", security_scene_arg_arm, scene_id_at_arm) != ZWS_STATUS_OK) {
		goto l_err_append;
	}
    if (avro_value_get_by_name(arg_value, "security_scene_event_list_arm", &event_list_arm_value, NULL) != 0)
    {
        ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'security_scene_event_list_arm': %s", avro_strerror());
        goto l_err_append;
    }

	//ZWS_LOG(ZWS_LOG_DEBUG, "[7] zws_avi_security_scene_get_result");
    if (avro_value_get_size(&event_list_arm_value, &event_arm_count) != 0)
    {
        ZWS_LOG(ZWS_LOG_ERR, "Failed to get arm event list size");
        goto l_err_append;
    }

	//ZWS_LOG(ZWS_LOG_DEBUG, "[8] zws_avi_security_scene_get_result event_arm_count: %d", event_arm_count);
    if (event_arm_count > 0)
    {
        if (zws_avi_security_scene_get_event_list(&event_list_arm_value, event_arm_count, result, "event_arm") != ZWS_STATUS_OK)
        {
            goto l_err_append;
        }
    }
	
	if(zws_avi_security_scene_get_last_trigger(&scene_get_value, "lt_arm", "lt", result) != ZWS_STATUS_OK) {
		goto l_err_append;
	}
	if (zwu_string_buffer_append_format(result, "</%s>", security_scene_arg_arm) != ZWS_STATUS_OK) {
		goto l_err_append;
	}
	//----------------------------------------------------------------------------------------
	
	//ZWS_LOG(ZWS_LOG_DEBUG, "[9] zws_avi_security_scene_get_result");
	//----------------------------------------------------------------------------------------
    // Process disarm event list
    //----------------------------------------------------------------------------------------
	if (zwu_string_buffer_append_format(result, "<%s exec_scened=\"%u\" >", security_scene_arg_disarm, scene_id_at_disarm) != ZWS_STATUS_OK) {
		goto l_err_append;
	}
    if (avro_value_get_by_name(arg_value, "security_scene_event_list_disarm", &event_list_disarm_value, NULL) != 0)
    {
        ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'security_scene_event_list_disarm': %s", avro_strerror());
        goto l_err_append;
    }

	//ZWS_LOG(ZWS_LOG_DEBUG, "[10] zws_avi_security_scene_get_result");
    if (avro_value_get_size(&event_list_disarm_value, &event_disarm_count) != 0)
    {
        ZWS_LOG(ZWS_LOG_ERR, "Failed to get disarm event list size");
        goto l_err_append;
    }

	//ZWS_LOG(ZWS_LOG_DEBUG, "[11] zws_avi_security_scene_get_result event_disarm_count: %d", event_disarm_count);
    if (event_disarm_count > 0)
    {
        if (zws_avi_security_scene_get_event_list(&event_list_disarm_value, event_disarm_count, result, "event_disarm") != ZWS_STATUS_OK)
        {
            goto l_err_append;
        }
    }
	if(zws_avi_security_scene_get_last_trigger(&scene_get_value, "lt_disarm", "lt", result) != ZWS_STATUS_OK) {
		goto l_err_append;
	}
	if (zwu_string_buffer_append_format(result, "</%s>", security_scene_arg_disarm) != ZWS_STATUS_OK) {
		goto l_err_append;
	}
	//----------------------------------------------------------------------------------------
	
	//ZWS_LOG(ZWS_LOG_DEBUG, "[12] zws_avi_security_scene_get_result");
	//----------------------------------------------------------------------------------------
    // Process alarm event list
    //----------------------------------------------------------------------------------------
	if (zwu_string_buffer_append_format(result, "<%s exec_scened=\"%u\" >", security_scene_arg_alarm, scene_id_at_alarm) != ZWS_STATUS_OK) {
		goto l_err_append;
	}
    if (avro_value_get_by_name(arg_value, "security_scene_event_list_alarm", &event_list_alarm_value, NULL) != 0)
    {
        ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'security_scene_event_list_alarm': %s", avro_strerror());
        goto l_err_append;
    }

	//ZWS_LOG(ZWS_LOG_DEBUG, "[13] zws_avi_security_scene_get_result");
    if (avro_value_get_size(&event_list_alarm_value, &event_alarm_count) != 0)
    {
        ZWS_LOG(ZWS_LOG_ERR, "Failed to get alarm event list size");
        goto l_err_append;
    }

	//ZWS_LOG(ZWS_LOG_DEBUG, "[14] zws_avi_security_scene_get_result event_alarm_count: %d", event_alarm_count);
    if (event_alarm_count > 0)
    {
        if (zws_avi_security_scene_get_event_list(&event_list_alarm_value, event_alarm_count, result, "event_alarm") != ZWS_STATUS_OK)
        {
            goto l_err_append;
        }
    }
	if(zws_avi_security_scene_get_last_trigger(&scene_get_value, "lt_alarm_on", "lt_on", result) != ZWS_STATUS_OK) {
		goto l_err_append;
	}
	if(zws_avi_security_scene_get_last_trigger(&scene_get_value, "lt_alarm_off", "lt_off", result) != ZWS_STATUS_OK) {
		goto l_err_append;
	}
	if (zwu_string_buffer_append_format(result, "</%s>", security_scene_arg_alarm) != ZWS_STATUS_OK) {
		goto l_err_append;
	}
	//----------------------------------------------------------------------------------------
    // Process Notification
    //----------------------------------------------------------------------------------------
	if (zwu_string_buffer_append_format(result, "<%s on=\"%u\" >", security_scene_arg_notification, (unsigned)is_notification_on) != ZWS_STATUS_OK) {
		goto l_err_append;
	}
	if (zwu_string_buffer_append_format(result, "<%s on=\"%u\" number=\"%s\" />", security_scene_arg_sms, (unsigned)is_notification_by_sms_on, notification_sms_number) != ZWS_STATUS_OK) {
		goto l_err_append;
	}
	if (zwu_string_buffer_append_format(result, "<%s on=\"%u\" address=\"%s\" />", security_scene_arg_email, (unsigned)is_notification_by_email_on, notification_email) != ZWS_STATUS_OK) {
		goto l_err_append;
	}
	if (zwu_string_buffer_append_format(result, "</%s>", security_scene_arg_notification) != ZWS_STATUS_OK) {
		goto l_err_append;
	}
	//----------------------------------------------------------------------------------------
	res = zwu_string_buffer_append_format(result, security_scene_tag_close);
	//ZWS_LOG(ZWS_LOG_DEBUG, "[15] zws_avi_security_scene_get_result");

	if(notification_sms_number)
		zw_avro_field_free_string(notification_sms_number);
	if(notification_email)
		zw_avro_field_free_string(notification_email);

	return res;

l_err_append:
l_err_get_fields:
	if(name)
		zw_avro_field_free_string(name);
	if(notification_sms_number)
		zw_avro_field_free_string(notification_sms_number);
	if(notification_email)
		zw_avro_field_free_string(notification_email);
l_err_get_list_value:
	return ZWS_STATUS_ERROR;
}


/** Security Scene list command handler structure */
static zws_avi_message_networks_t security_scene_get =
{
    {scene_api_1, scene_api_2},        			 	/**< URI for security scene related commands*/
    CMD_SECURITY_SCENE_GET,                     	/**< Commands passed as value of POST parameters 'cmd=' or 'cmd_cgi=' */
    "security_scene_get",                       	/**< AVRO message name as in the protocol schema "zwave.avsc"*/
    zws_avi_security_scene_generic_arg_handler,     /**< POST request to AVRO parser*/
    zws_avi_security_scene_get_result,          	/**< AVRO to XML converter*/
    ""                                  			/**< Error text to be sent if command fails */
};


/**
zws_avi_security_scene_get_init - initialize security scene list command handler for POST->AVRO and AVRO->XML
@param[in]	message_list	    Message list
@return ZWS_STATUS_XXX
*/
zws_status_t zws_avi_security_scene_get_init(zwu_array_list_t message_list)
{
	if (zws_avi_message_list_add_message_networks(message_list, &security_scene_get) != ZWS_STATUS_OK)
	{
        return ZWS_STATUS_ERROR;
	}

	return ZWS_STATUS_OK;
}

