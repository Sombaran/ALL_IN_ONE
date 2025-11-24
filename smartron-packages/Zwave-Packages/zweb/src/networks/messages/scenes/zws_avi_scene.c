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
 * @file    zws_avi_scene.c
 *
 * @brief   Common file for Scenes AVI messages
 * @details Common file for Scenes AVI messages.
 *
 * @author  Arif & David
 *
 * @version 1.0 - 2013-10-02
 * - Initial version
 */

#include "zws_status.h"
#include "zws_avi_message_networks.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zws_avi_scene.h"
#include "zws_avi_security_scene_common.h"

const char scene_api_1[]            = "scene";
const char scene_api_2[]            = "zw_scene";
const char scene_tag[]              = "scene";
const char scene_arg_scened[]       = "scened";
const char scene_arg_name[]         = "name";
const char scene_arg_active[]       = "active";
const char scene_arg_status[]       = "status";
const char scene_arg_sutime[]       = "sutime";
const char scene_arg_lkstatus[]     = "lkstatus";
const char scene_arg_lksutime[]     = "lksutime";
const char scene_arg_letrigger[]    = "letrigger";
const char scene_arg_letime[]       = "letime";
const char scene_arg_triggers[]     = "triggers";
const char scene_arg_max_scenes[]   = "max_scenes";
const char scene_arg_max_actions[]  = "max_actions";
const char scene_arg_max_schedules[]= "max_schedules";
const char scene_arg_max_events[]   = "max_events";
const char scene_arg_timezone[]     = "timezone";


/**
zws_avi_param_list_to_xml - Convert the param list from AVRO to XML format
@param[in]	param_list_value    AVRO encoded param list
@param[in]	param_cnt           Number of elements in param list
@param[out]	result	            Output in XML format
@return ZWS_STATUS_XXX
*/
zws_status_t zws_avi_param_list_to_xml(avro_value_t *param_list_value, int param_cnt, zwu_string_buffer_t result)
{
	int          j;
    char         *param_key;
    char         *param_value;
    zws_status_t res;
	avro_value_t param_avro;

    for (j=0; j<param_cnt; j++)
    {
        if (avro_value_get_by_index(param_list_value, j, &param_avro, NULL) != 0)
        {
            return ZWS_STATUS_ERROR;
        }

        if (zw_avro_field_get_string(&param_avro, "key", &param_key) != ZWS_STATUS_OK)
        {
            return ZWS_STATUS_ERROR;
        }

        if (zw_avro_field_get_string(&param_avro, "value", &param_value) != ZWS_STATUS_OK)
        {
            zw_avro_field_free_string(param_key);
            return ZWS_STATUS_ERROR;
        }

        res = zwu_string_buffer_append_format(result,
                                            "%s=\"%s\"%s",
                                            param_key,
                                            param_value,
                                            ((j + 1) >= param_cnt)? " />" : " "
                                            );

        zw_avro_field_free_string(param_key);
        zw_avro_field_free_string(param_value);

        if (res != ZWS_STATUS_OK)
        {
            return ZWS_STATUS_ERROR;
        }

        //Reset param value
        avro_value_reset(&param_avro);
    }

    return ZWS_STATUS_OK;
}


/**
zws_avi_scene_generic_arg_handler - Convert the scene generic request from POST data to AVRO format
@param[in]	args	            Buffer to store key-value pairs
@param[in]	args_count	        Number of key-value pairs in args
@param[out]	arg_value_p	        Output in AVRO object
@return ZWS_STATUS_XXX
*/
static zws_status_t zws_avi_scene_generic_arg_handler(char *(*args)[2], int  args_count, avro_value_t *arg_value_p)
{
    if (args_count > 0)
    {
        if (strcmp(args[0][0], scene_arg_scened) == 0)
        {
            //Check for empty "value"
            if (*(args[0][1]) == '\0')
            {
                return ZWS_STATUS_ERROR;
            }

            if (zw_avro_field_set_int(arg_value_p, scene_arg_scened, atoi(args[0][1])) == ZWS_STATUS_OK)
            {
                return ZWS_STATUS_OK;
            }
        }
    }
    return ZWS_STATUS_ERROR;
}


/**
zws_avi_scene_generic_resp - Convert the scene generic response from AVRO to XML format
@param[in]	web_api_version	    Web API version
@param[in]	arg_value	        AVRO encoded response
@param[out]	result	            Output in XML format
@return ZWS_STATUS_XXX
*/
static zws_status_t zws_avi_scene_generic_resp(int web_api_version, avro_value_t *arg_value, zwu_string_buffer_t result)
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
                                        scene_tag,
                                        scene_desc);
}


/**
zws_avi_scene_get_action_list - Convert the scene get details action list from AVRO to XML format
@param[in]	action_list_value   AVRO encoded action list
@param[in]	action_cnt          Number of elements in action list
@param[out]	result	            Output in XML format
@return ZWS_STATUS_XXX
*/
static zws_status_t zws_avi_scene_get_action_list(avro_value_t *action_list_value, int action_cnt, zwu_string_buffer_t result)
{
    int32_t         scene_status;
    int32_t         scene_lkstatus;
    int32_t         scene_actype;
    int32_t         scene_cmd;
    int32_t         scene_ifd;
//    int32_t         epid;
	size_t          param_cnt;
	int             i;
    char            *scene_uri;
	int64_t         scene_sutime;
	int64_t         scene_lksutime;
    zws_status_t    res;
	avro_value_t    action_value;
	avro_value_t    param_list_value;

	for (i=0; i<action_cnt; i++)
	{
		if (avro_value_get_by_index(action_list_value, i, &action_value, NULL) != 0)
		{
			return ZWS_STATUS_ERROR;
		}

		if ((zw_avro_field_get_int(&action_value, "actype", &scene_actype) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&action_value, "status", &scene_status) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_long(&action_value, "sutime", &scene_sutime) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&action_value, "lkstatus", &scene_lkstatus) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_long(&action_value, "lksutime", &scene_lksutime) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&action_value, "cmd", &scene_cmd) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&action_value, "ifd", &scene_ifd) != ZWS_STATUS_OK))
		{
			return ZWS_STATUS_ERROR;
		}

        //Check whether parameter list exists
        if (avro_value_get_by_name(&action_value, "param_list", &param_list_value, NULL) != 0)
        {
            ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'param_list': %s", avro_strerror());
            return ZWS_STATUS_ERROR;
        }

        if (avro_value_get_size(&param_list_value, &param_cnt) != 0)
        {
            ZWS_LOG(ZWS_LOG_ERR, "Failed to get param list size");
            return ZWS_STATUS_ERROR;
        }

        if (zw_avro_field_get_string(&action_value, "uri", &scene_uri) != ZWS_STATUS_OK)
        {
            return ZWS_STATUS_ERROR;
        }

//Done on zwportal side now
//        //Adjust for endpoint id
//        epid = (scene_ifd >> 8) & 0xFF;
//        scene_ifd = (scene_ifd & 0xFFFF00FF) | (((epid + 1) & 0xFF) << 8);


        res = zwu_string_buffer_append_format(result,
                                            "<action actype=\"%u\" status=\"%u\" sutime=\"%llu\" "
                                            "lkstatus=\"%u\" lksutime=\"%llu\" uri=\"%s\" cmd=\"%u\" "
                                            "ifd=\"%u\"%s",
                                            (unsigned)scene_actype,
                                            (unsigned)scene_status,
                                            (unsigned long long int)scene_sutime,
                                            (unsigned)scene_lkstatus,
                                            (unsigned long long int)scene_lksutime,
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

        //Reset action value
        avro_value_reset(&action_value);
	}

    return ZWS_STATUS_OK;
}


/**
zws_avi_scene_get_schedule_list - Convert the scene get details schedule list from AVRO to XML format
@param[in]	schedule_list_value   AVRO encoded schedule list
@param[in]	schedule_cnt          Number of elements in schedule list
@param[out]	result	              Output in XML format
@return ZWS_STATUS_XXX
*/
static zws_status_t zws_avi_scene_get_schedule_list(avro_value_t *schedule_list_value, int schedule_cnt, zwu_string_buffer_t result)
{
    int32_t         scene_sctype;
    int32_t         scene_day;
    int32_t         scene_hour;
    int32_t         scene_minute;
	int             i;
    zws_status_t    res;
	avro_value_t    schedule_value;

	for (i=0; i<schedule_cnt; i++)
	{
		if (avro_value_get_by_index(schedule_list_value, i, &schedule_value, NULL) != 0)
		{
			return ZWS_STATUS_ERROR;
		}

		if ((zw_avro_field_get_int(&schedule_value, "sctype", &scene_sctype) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&schedule_value, "day", &scene_day) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&schedule_value, "hour", &scene_hour) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&schedule_value, "minute", &scene_minute) != ZWS_STATUS_OK))
		{
			return ZWS_STATUS_ERROR;
		}

        res = zwu_string_buffer_append_format(result,
                                            "<schedule sctype=\"%u\" day=\"%u\" hour=\"%u\" minute=\"%u\" />",
                                            (unsigned)scene_sctype,
                                            (unsigned)scene_day,
                                            (unsigned)scene_hour,
                                            (unsigned)scene_minute
                                            );

        if (res != ZWS_STATUS_OK)
        {
            return ZWS_STATUS_ERROR;
        }

        //Reset schedule value
        avro_value_reset(&schedule_value);
	}

    return ZWS_STATUS_OK;
}


/**
zws_avi_scene_get_event_list - Convert the scene get details event list from AVRO to XML format
@param[in]	event_list_value   AVRO encoded event list
@param[in]	event_cnt          Number of elements in event list
@param[out]	result	           Output in XML format
@return ZWS_STATUS_XXX
*/
static zws_status_t zws_avi_scene_get_event_list(avro_value_t *event_list_value, int event_cnt, zwu_string_buffer_t result)
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

		if ((zw_avro_field_get_int(&event_value, "evtype", &scene_evtype) != ZWS_STATUS_OK)
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
                                            "<event evtype=\"%u\" status=\"%u\" sutime=\"%llu\" "
                                            "uri=\"%s\" cmd=\"%u\" "
                                            "ifd=\"%u\"%s",
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
zws_avi_scene_get_resp - Convert the scene get details response from AVRO to XML format
@param[in]	web_api_version	    Web API version
@param[in]	arg_value	        AVRO encoded response
@param[out]	result	            Output in XML format
@return ZWS_STATUS_XXX
*/
static zws_status_t zws_avi_scene_get_resp(int web_api_version, avro_value_t *arg_value, zwu_string_buffer_t result)
{
    int32_t         scene_desc;
    int32_t         scene_active;
    int32_t         scene_status;
    int32_t         scene_lkstatus;
    int32_t         scene_triggers;
    int32_t         scene_letrigger;
	size_t          action_cnt;
	size_t          schedule_cnt;
	size_t          event_cnt;
    char            *scene_name;
	int64_t         scene_sutime;
	int64_t         scene_lksutime;
	int64_t         scene_letime;
    zws_status_t    res;
	avro_value_t    action_list_value;
	avro_value_t    schedule_list_value;
	avro_value_t    event_list_value;
	avro_value_t    scene_rpt_value;

    //This function is called twice for different API versions. We ignore the second call to avoid duplicate results.
    if (web_api_version == ZWS_WEB_API_VERSION_2)
    {
        return ZWS_STATUS_OK;
    }

	if (avro_value_get_by_name(arg_value, "report", &scene_rpt_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'report': %s", avro_strerror());
		return ZWS_STATUS_ERROR;
	}

    if ((zw_avro_field_get_int(&scene_rpt_value, scene_arg_scened, &scene_desc) != ZWS_STATUS_OK)
        || (zw_avro_field_get_int(&scene_rpt_value, "active", &scene_active) != ZWS_STATUS_OK)
        || (zw_avro_field_get_int(&scene_rpt_value, "status", &scene_status) != ZWS_STATUS_OK)
        || (zw_avro_field_get_long(&scene_rpt_value, "sutime", &scene_sutime) != ZWS_STATUS_OK)
        || (zw_avro_field_get_int(&scene_rpt_value, "lkstatus", &scene_lkstatus) != ZWS_STATUS_OK)
        || (zw_avro_field_get_long(&scene_rpt_value, "lksutime", &scene_lksutime) != ZWS_STATUS_OK)
        || (zw_avro_field_get_int(&scene_rpt_value, "triggers", &scene_triggers) != ZWS_STATUS_OK)
        || (zw_avro_field_get_int(&scene_rpt_value, "letrigger", &scene_letrigger) != ZWS_STATUS_OK)
        || (zw_avro_field_get_long(&scene_rpt_value, "letime", &scene_letime) != ZWS_STATUS_OK))
    {
        return ZWS_STATUS_ERROR;
    }

    if (zw_avro_field_get_string(&scene_rpt_value, "name", &scene_name) != ZWS_STATUS_OK)
    {
        return ZWS_STATUS_ERROR;
    }

    res = zwu_string_buffer_append_format(result,
                                        "<%s desc=\"%u\" name=\"%s\" active=\"%u\" status=\"%u\" sutime=\"%llu\" "
                                        "lkstatus=\"%u\" lksutime=\"%llu\" triggers=\"%u\" "
                                        "letrigger=\"%u\" letime=\"%llu\" >",
                                        scene_tag,
                                        (unsigned)scene_desc,
                                        scene_name,
                                        (unsigned)scene_active,
                                        (unsigned)scene_status,
                                        (unsigned long long int)scene_sutime,
                                        (unsigned)scene_lkstatus,
                                        (unsigned long long int)scene_lksutime,
                                        (unsigned)scene_triggers,
                                        (unsigned)scene_letrigger,
                                        (unsigned long long int)scene_letime
                                        );

    zw_avro_field_free_string(scene_name);

    if (res != ZWS_STATUS_OK)
    {
        return ZWS_STATUS_ERROR;
    }

    //----------------------------------------------------------------------------------------
    // Process action list
    //----------------------------------------------------------------------------------------
	if (avro_value_get_by_name(arg_value, "scene_action_list", &action_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'scene_action_list': %s", avro_strerror());
		return ZWS_STATUS_ERROR;
	}

	if (avro_value_get_size(&action_list_value, &action_cnt) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get action list size");
		return ZWS_STATUS_ERROR;
	}

    if (action_cnt > 0)
    {
        if (zws_avi_scene_get_action_list(&action_list_value, action_cnt, result) != ZWS_STATUS_OK)
        {
            return ZWS_STATUS_ERROR;
        }
    }

    //----------------------------------------------------------------------------------------
    // Process schedule list
    //----------------------------------------------------------------------------------------
	if (avro_value_get_by_name(arg_value, "scene_schedule_list", &schedule_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'scene_schedule_list': %s", avro_strerror());
		return ZWS_STATUS_ERROR;
	}

	if (avro_value_get_size(&schedule_list_value, &schedule_cnt) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get schedule list size");
		return ZWS_STATUS_ERROR;
	}

    if (schedule_cnt > 0)
    {
        if (zws_avi_scene_get_schedule_list(&schedule_list_value, schedule_cnt, result) != ZWS_STATUS_OK)
        {
            return ZWS_STATUS_ERROR;
        }
    }

    //----------------------------------------------------------------------------------------
    // Process event list
    //----------------------------------------------------------------------------------------
    if (avro_value_get_by_name(arg_value, "scene_event_list", &event_list_value, NULL) != 0)
    {
        ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'scene_event_list': %s", avro_strerror());
        return ZWS_STATUS_ERROR;
    }

    if (avro_value_get_size(&event_list_value, &event_cnt) != 0)
    {
        ZWS_LOG(ZWS_LOG_ERR, "Failed to get event list size");
        return ZWS_STATUS_ERROR;
    }

    if (event_cnt > 0)
    {
        if (zws_avi_scene_get_event_list(&event_list_value, event_cnt, result) != ZWS_STATUS_OK)
        {
            return ZWS_STATUS_ERROR;
        }
    }

    res = zwu_string_buffer_append_format(result, "</scene>");

    return res;

}


/** Get scene details command handler structure */
static zws_avi_message_networks_t scene_get =
{
    {scene_api_1, scene_api_2},         /**< URI for scene related commands*/
    CMD_SCENE_GET,                      /**< Commands passed as value of POST parameters 'cmd=' or 'cmd_cgi=' */
    "scene_get",                        /**< AVRO message name as in the protocol schema "zwave.avsc"*/
    zws_avi_scene_generic_arg_handler,  /**< POST request to AVRO parser*/
    zws_avi_scene_get_resp,             /**< AVRO to XML converter*/
    ""                                  /**< Error text to be sent if command fails */
};


/**
zws_avi_scene_get_init - initialize scene get details command handler for POST->AVRO and AVRO->XML
@param[in]	message_list	    Message list
@return ZWS_STATUS_XXX
*/
zws_status_t zws_avi_scene_get_init(zwu_array_list_t message_list)
{
    if (zws_avi_message_list_add_message_networks(message_list, &scene_get) != ZWS_STATUS_OK)
    {
        return ZWS_STATUS_ERROR;
    }

    return ZWS_STATUS_OK;
}


/** Delete scene command handler structure */
static zws_avi_message_networks_t scene_del =
{
    {scene_api_1, scene_api_2},         /**< URI for scene related commands*/
    CMD_SCENE_DELETE,                   /**< Commands passed as value of POST parameters 'cmd=' or 'cmd_cgi=' */
    "scene_del",                        /**< AVRO message name as in the protocol schema "zwave.avsc"*/
    zws_avi_scene_generic_arg_handler,  /**< POST request to AVRO parser*/
    zws_avi_scene_generic_resp,         /**< AVRO to XML converter*/
    ""                                  /**< Error text to be sent if command fails */
};


/**
zws_avi_scene_del_init - initialize scene delete command handler for POST->AVRO and AVRO->XML
@param[in]	message_list	    Message list
@return ZWS_STATUS_XXX
*/
zws_status_t zws_avi_scene_del_init(zwu_array_list_t message_list)
{
    if (zws_avi_message_list_add_message_networks(message_list, &scene_del) != ZWS_STATUS_OK)
    {
        return ZWS_STATUS_ERROR;
    }

    return ZWS_STATUS_OK;
}


/** Execute scene command handler structure */
static zws_avi_message_networks_t scene_exec =
{
    {scene_api_1, scene_api_2},         /**< URI for scene related commands*/
    CMD_SCENE_EXEC,                     /**< Commands passed as value of POST parameters 'cmd=' or 'cmd_cgi=' */
    "scene_exec",                       /**< AVRO message name as in the protocol schema "zwave.avsc"*/
    zws_avi_scene_generic_arg_handler,  /**< POST request to AVRO parser*/
    zws_avi_scene_generic_resp,         /**< AVRO to XML converter*/
    ""                                  /**< Error text to be sent if command fails */
};


/**
zws_avi_scene_exec_init - initialize scene execute command handler for POST->AVRO and AVRO->XML
@param[in]	message_list	    Message list
@return ZWS_STATUS_XXX
*/
zws_status_t zws_avi_scene_exec_init(zwu_array_list_t message_list)
{
    if (zws_avi_message_list_add_message_networks(message_list, &scene_exec) != ZWS_STATUS_OK)
    {
        return ZWS_STATUS_ERROR;
    }

    return ZWS_STATUS_OK;
}


/**
zws_avi_scene_updt_arg_handler - Convert the scene update status request from POST data to AVRO format
@param[in]	args	            Buffer to store key-value pairs
@param[in]	args_count	        Number of key-value pairs in args
@param[out]	arg_value_p	        Output in AVRO object
@return ZWS_STATUS_XXX
*/
static zws_status_t zws_avi_scene_updt_arg_handler(char *(*args)[2], int  args_count, avro_value_t *arg_value_p)
{
    if (args_count >= 2)
    {
        //Check for empty "value"
        if ((*(args[0][1]) == '\0') || (*(args[1][1]) == '\0'))
        {
            return ZWS_STATUS_ERROR;
        }

        if ((strcmp(args[0][0], scene_arg_scened) == 0)
            && (zw_avro_field_set_int(arg_value_p, scene_arg_scened, atoi(args[0][1])) == ZWS_STATUS_OK)
            && (strcmp(args[1][0], "all") == 0)
            && (zw_avro_field_set_int(arg_value_p, "all", atoi(args[1][1])) == ZWS_STATUS_OK))
        {
            return ZWS_STATUS_OK;
        }
    }
    return ZWS_STATUS_ERROR;
}


/** Update scene status command handler structure */
static zws_avi_message_networks_t scene_updt =
{
    {scene_api_1, scene_api_2},         /**< URI for scene related commands*/
    CMD_SCENE_STATUS_UPDATE,            /**< Commands passed as value of POST parameters 'cmd=' or 'cmd_cgi=' */
    "scene_updt",                       /**< AVRO message name as in the protocol schema "zwave.avsc"*/
    zws_avi_scene_updt_arg_handler,     /**< POST request to AVRO parser*/
    zws_avi_scene_generic_resp,         /**< AVRO to XML converter*/
    ""                                  /**< Error text to be sent if command fails */
};


/**
zws_avi_scene_updt_init - initialize scene update status command handler for POST->AVRO and AVRO->XML
@param[in]	message_list	    Message list
@return ZWS_STATUS_XXX
*/
zws_status_t zws_avi_scene_updt_init(zwu_array_list_t message_list)
{
    if (zws_avi_message_list_add_message_networks(message_list, &scene_updt) != ZWS_STATUS_OK)
    {
        return ZWS_STATUS_ERROR;
    }

    return ZWS_STATUS_OK;
}


/**
zws_avi_scene_get_state_log_arg_handler - Convert the get scene state log request from POST data to AVRO format
@param[in]	args	            Buffer to store key-value pairs
@param[in]	args_count	        Number of key-value pairs in args
@param[out]	arg_value_p	        Output in AVRO object
@return ZWS_STATUS_XXX
*/
static zws_status_t zws_avi_scene_get_state_log_arg_handler(char *(*args)[2], int  args_count, avro_value_t *arg_value_p)
{
    if (args_count > 0)
    {
        //Check for empty "value"
        if (*(args[0][1]) == '\0')
        {
            return ZWS_STATUS_ERROR;
        }

        if ((strcmp(args[0][0], "type") == 0)
            && (zw_avro_field_set_int(arg_value_p, "type", atoi(args[0][1])) == ZWS_STATUS_OK))
        {
            return ZWS_STATUS_OK;
        }
    }
    return ZWS_STATUS_ERROR;
}


/**
zws_avi_scene_get_state_log_list - Convert the scene state log list from AVRO to XML format
@param[in]	state_log_list_value   AVRO encoded state log list
@param[in]	state_log_cnt          Number of elements in state_log list
@param[out]	result	               Output in XML format
@return ZWS_STATUS_XXX
*/
static zws_status_t zws_avi_scene_get_state_log_list(avro_value_t *state_log_list_value, int state_log_cnt, zwu_string_buffer_t result)
{
    int32_t         type;
    int32_t         curr_op;
    int32_t         cur_op_scened;
    int32_t         cur_op_trigger;
    int32_t         last_op_scened;
    int32_t         last_op_trigger;
	int             i;
	int64_t         last_op_time;
    zws_status_t    res;
	avro_value_t    state_log_value;

	for (i=0; i<state_log_cnt; i++)
	{
		if (avro_value_get_by_index(state_log_list_value, i, &state_log_value, NULL) != 0)
		{
			return ZWS_STATUS_ERROR;
		}

		if ((zw_avro_field_get_int(&state_log_value, "type", &type) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&state_log_value, "curr_op", &curr_op) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&state_log_value, "cur_op_scened", &cur_op_scened) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&state_log_value, "cur_op_trigger", &cur_op_trigger) != ZWS_STATUS_OK)
                || (zw_avro_field_get_int(&state_log_value, "last_op_scened", &last_op_scened) != ZWS_STATUS_OK)
                || (zw_avro_field_get_int(&state_log_value, "last_op_trigger", &last_op_trigger) != ZWS_STATUS_OK)
                || (zw_avro_field_get_long(&state_log_value, "last_op_time", &last_op_time) != ZWS_STATUS_OK))
		{
			return ZWS_STATUS_ERROR;
		}

        res = zwu_string_buffer_append_format(result,
                                            "<statelog type=\"%u\" curr_op=\"%u\" cur_op_scened=\"%u\" cur_op_trigger=\"%u\" "
                                            "last_op_scened=\"%u\" last_op_time=\"%llu\" last_op_trigger=\"%u\" />",
                                            (unsigned)type,
                                            (unsigned)curr_op,
                                            (unsigned)cur_op_scened,
                                            (unsigned)cur_op_trigger,
                                            (unsigned)last_op_scened,
                                            (unsigned long long int)last_op_time,
                                            (unsigned)last_op_trigger
                                            );

        if (res != ZWS_STATUS_OK)
        {
            return ZWS_STATUS_ERROR;
        }

        //Reset state_log value
        avro_value_reset(&state_log_value);
	}

    return ZWS_STATUS_OK;
}

/**
zws_avi_security_scene_get_state_log_list - Convert the security scene state log list from AVRO to XML format
@param[in]	state_log_list_value   AVRO encoded state log list
@param[in]	state_log_cnt          Number of elements in state_log list
@param[out]	result	               Output in XML format
@return ZWS_STATUS_XXX
*/
static zws_status_t zws_avi_security_scene_get_state_log_list(avro_value_t *state_log_list_value, int state_log_cnt, zwu_string_buffer_t result)
{
    int32_t         type;
    int32_t         counter;
    int32_t         scened;
	int             i;
    zws_status_t    res;
	avro_value_t    state_log_value;

	for (i=0; i<state_log_cnt; i++)
	{
		if (avro_value_get_by_index(state_log_list_value, i, &state_log_value, NULL) != 0)
		{
			return ZWS_STATUS_ERROR;
		}

		if ((zw_avro_field_get_int(&state_log_value, security_scene_arg_type, &type) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&state_log_value, security_scene_arg_counter, &counter) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&state_log_value, security_scene_arg_scened, &scened) != ZWS_STATUS_OK))
		{
			return ZWS_STATUS_ERROR;
		}

        res = zwu_string_buffer_append_format(result,
                                            "<statelog type=\"%u\" counter=\"%u\" scened=\"%u\" />",
                                            (unsigned)type,
                                            (unsigned)counter,
                                            (unsigned)scened);

        if (res != ZWS_STATUS_OK)
        {
            return ZWS_STATUS_ERROR;
        }

        //Reset state_log value
        avro_value_reset(&state_log_value);
	}

    return ZWS_STATUS_OK;
}

/**
zws_avi_scene_get_state_log_resp - Convert the scene get state log response from AVRO to XML format
@param[in]	web_api_version	    Web API version
@param[in]	arg_value	        AVRO encoded response
@param[out]	result	            Output in XML format
@return ZWS_STATUS_XXX
*/
static zws_status_t zws_avi_scene_get_state_log_resp(int web_api_version, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	size_t       state_log_cnt;
	avro_value_t state_log_list_value;
	size_t       security_scene_state_log_cnt;
	avro_value_t security_scene_state_log_list_value;

    //This function is called twice for different API versions. We ignore the second call to avoid duplicate results.
    if (web_api_version == ZWS_WEB_API_VERSION_2)
    {
        return ZWS_STATUS_OK;
    }

    //----------------------------------------------------------------------------------------
    // Process state log list
    //----------------------------------------------------------------------------------------
	if (avro_value_get_by_name(arg_value, "scene_state_log_list", &state_log_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'scene_state_log_list': %s", avro_strerror());
		return ZWS_STATUS_ERROR;
	}

	if (avro_value_get_size(&state_log_list_value, &state_log_cnt) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get state log list size");
		return ZWS_STATUS_ERROR;
	}

    if (state_log_cnt > 0)
    {
        if (zws_avi_scene_get_state_log_list(&state_log_list_value, state_log_cnt, result) != ZWS_STATUS_OK)
        {
            return ZWS_STATUS_ERROR;
        }
    }
	
	//----------------------------------------------------------------------------------------
    // Process security scene state log list
    //----------------------------------------------------------------------------------------
	if (avro_value_get_by_name(arg_value, "security_scene_state_log_list", &security_scene_state_log_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'security_scene_state_log_list': %s", avro_strerror());
		return ZWS_STATUS_ERROR;
	}

	if (avro_value_get_size(&security_scene_state_log_list_value, &security_scene_state_log_cnt) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get security scene state log list size");
		return ZWS_STATUS_ERROR;
	}

    if (security_scene_state_log_cnt > 0)
    {
        if (zws_avi_security_scene_get_state_log_list(&security_scene_state_log_list_value, security_scene_state_log_cnt, result) != ZWS_STATUS_OK)
        {
            return ZWS_STATUS_ERROR;
        }
    }

	return ZWS_STATUS_OK;
}


/** Get scene statelog command handler structure */
static zws_avi_message_networks_t scene_get_state_log =
{
    {scene_api_1, scene_api_2},                 /**< URI for scene related commands*/
    CMD_SCENE_GET_STATELOG,                     /**< Commands passed as value of POST parameters 'cmd=' or 'cmd_cgi=' */
    "scene_get_state_log",                      /**< AVRO message name as in the protocol schema "zwave.avsc"*/
    zws_avi_scene_get_state_log_arg_handler,    /**< POST request to AVRO parser*/
    zws_avi_scene_get_state_log_resp,           /**< AVRO to XML converter*/
    ""                                          /**< Error text to be sent if command fails */
};


/**
zws_avi_scene_get_state_log_init - initialize get scene statelog command handler for POST->AVRO and AVRO->XML
@param[in]	message_list	    Message list
@return ZWS_STATUS_XXX
*/
zws_status_t zws_avi_scene_get_state_log_init(zwu_array_list_t message_list)
{
    if (zws_avi_message_list_add_message_networks(message_list, &scene_get_state_log) != ZWS_STATUS_OK)
    {
        return ZWS_STATUS_ERROR;
    }

    return ZWS_STATUS_OK;
}

