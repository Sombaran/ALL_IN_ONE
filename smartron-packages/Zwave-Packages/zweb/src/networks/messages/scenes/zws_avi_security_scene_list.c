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
 * @file    zws_avi_security_scene_list.c
 *
 * @brief   AVI message 'security_scene_list'
 * @details Handles the AVI message 'security_scene_list'.
 *
 * @author  Arif
 *
 * @version 1.0 - 2015-05-11
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
zws_avi_security_scene_list_result - Convert the security scene list response from AVRO to XML format
@param[in]	web_api_version	    Web API version
@param[in]	arg_value	        AVRO encoded response
@param[out]	result	            Output in XML format
@return ZWS_STATUS_XXX
*/
static zws_status_t zws_avi_security_scene_list_result(int web_api_version, avro_value_t *arg_value, zwu_string_buffer_t result)
{
	avro_value_t    scene_list_value;
	avro_value_t    scene_value;
	size_t          scene_count;
	int             i;
	int             scened;
	char            *name = NULL;
	int             active;
	int             is_armed;
	int             is_alarmed;
	int				scene_id_at_arm;
	int				scene_id_at_disarm;
	int				scene_id_at_alarm;
//	int             latrigger_type;
//	int             latrigger_event_id;
//	int64_t         latrigger_time;
//	avro_value_t    lt_arm_value;
//	avro_value_t    lt_alarm_on_value;
//	avro_value_t    event_value;
//	int32_t temp_type = 0;
//	int64_t temp_time = 0;
//	int32_t temp_type2 = 0;
//	int64_t temp_time2 = 0;
	int64_t utime = 0;
	

	if(web_api_version == ZWS_WEB_API_VERSION_2) { //this function is called twice for different API versions. We ignore the second call to avoid duplicate results.
			return ZWS_STATUS_OK;
	}

	if (avro_value_get_by_name(arg_value, "SecuritySceneList", &scene_list_value, NULL) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'SecuritySceneList': %s", avro_strerror());
		goto l_err_get_list_value;
	}

	if (avro_value_get_size(&scene_list_value, &scene_count) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get security scene list size");
		goto l_err_node_list_size;
	}

	for (i = 0; i < scene_count; i++)
	{
		if (avro_value_get_by_index(&scene_list_value, i, &scene_value, NULL) != 0)
		{
			goto l_err_get_node_value;
		}

		if ((zw_avro_field_get_int(&scene_value, security_scene_arg_scened, &scened) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_string(&scene_value, security_scene_arg_name, &name) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&scene_value, security_scene_arg_active, &active) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&scene_value, security_scene_arg_is_armed, &is_armed) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&scene_value, security_scene_arg_is_alarmed, &is_alarmed) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&scene_value, security_scene_arg_scene_id_at_arm, &scene_id_at_arm) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&scene_value, security_scene_arg_scene_id_at_disarm, &scene_id_at_disarm) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_int(&scene_value, security_scene_arg_scene_id_at_alarm, &scene_id_at_alarm) != ZWS_STATUS_OK)
				|| (zw_avro_field_get_long(&scene_value, security_scene_arg_utime, &utime) != ZWS_STATUS_OK))
		{
			goto l_err_get_fields;
		}
		
//		if (avro_value_get_by_name(&scene_value, "lt_arm", &lt_arm_value, NULL) != 0)
//		{
//			ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'lt_arm': %s", avro_strerror());
//			return ZWS_STATUS_ERROR;
//		}
//		
//		if ((zw_avro_field_get_int(&lt_arm_value, security_scene_arg_type, &temp_type) != ZWS_STATUS_OK)
//            || (zw_avro_field_get_long(&lt_arm_value, security_scene_arg_time, &temp_time) != ZWS_STATUS_OK))
//        {
//            return ZWS_STATUS_ERROR;
//        }
//		ZWS_LOG(ZWS_LOG_ERR, "temp_type=%u temp_time=%llu", temp_type, temp_time);
		
//		if (avro_value_get_by_name(&scene_value, "lt_alarm_on", &lt_alarm_on_value, NULL) != 0)
//		{
//			ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field 'lt_alarm_on': %s", avro_strerror());
//			return ZWS_STATUS_ERROR;
//		}
//		
//		if ((zw_avro_field_get_int(&lt_alarm_on_value, security_scene_arg_type, &temp_type) != ZWS_STATUS_OK)
//            || (zw_avro_field_get_long(&lt_alarm_on_value, security_scene_arg_time, &temp_time) != ZWS_STATUS_OK))
//        {
//            return ZWS_STATUS_ERROR;
//        }
//		ZWS_LOG(ZWS_LOG_ERR, "temp_type=%u", temp_type);

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
		
//		if (zwu_string_buffer_append_format(result, "<%s type=\"%u\" time=\"%llu\" >",
//				security_scene_tag_lt_alarm_on,
//				(unsigned)temp_type,
//				(unsigned long long int)temp_time
//				) != ZWS_STATUS_OK)
//		{
//			goto l_err_append;
//		}
//		
//		if((temp_type == ZWSCN_STATELOG_TRIGGER_EVENT) && (temp_time !=0)) {
//			if (avro_value_get_by_name(&lt_alarm_on_value, security_scene_arg_event, &event_value, NULL) != 0)
//			{
//				ZWS_LOG(ZWS_LOG_ERR, "Failed to get the field '%s': %s", security_scene_arg_event, avro_strerror());
//				return ZWS_STATUS_ERROR;
//			}
////			if(event_value != NULL) {
//				zws_avi_security_scene_get_event(&event_value, result, "event_alarm");
////			}
//		}
//		
//		if (zwu_string_buffer_append_format(result, "</%s>", security_scene_tag_lt_alarm_on) != ZWS_STATUS_OK)
//		{
//			goto l_err_append;
//		}

		if(zws_avi_security_scene_get_last_trigger(&scene_value, "lt_alarm_on", "lt_alarm_on",  result) != ZWS_STATUS_OK) {
			goto l_err_append;
		}
		
		if (zwu_string_buffer_append_format(result, "</%s>", security_scene_tag) != ZWS_STATUS_OK)
		{
			goto l_err_append;
		}

		free(name);
        name = NULL;
	}

	return ZWS_STATUS_OK;

l_err_append:
l_err_get_fields:
	if(name) free(name);

l_err_get_node_value:
l_err_node_list_size:
l_err_get_list_value:
	return ZWS_STATUS_ERROR;
}


/** Security Scene list command handler structure */
static zws_avi_message_networks_t security_scene_list =
{
    {scene_api_1, scene_api_2},        			 	/**< URI for security scene related commands*/
    CMD_SECURITY_SCENE_LIST,                     	/**< Commands passed as value of POST parameters 'cmd=' or 'cmd_cgi=' */
    "security_scene_list",                       	/**< AVRO message name as in the protocol schema "zwave.avsc"*/
    NULL,                               			/**< POST request to AVRO parser*/
    zws_avi_security_scene_list_result,          	/**< AVRO to XML converter*/
    ""                                  			/**< Error text to be sent if command fails */
};


/**
zws_avi_security_scene_list_init - initialize security scene list command handler for POST->AVRO and AVRO->XML
@param[in]	message_list	    Message list
@return ZWS_STATUS_XXX
*/
zws_status_t zws_avi_security_scene_list_init(zwu_array_list_t message_list)
{
	if (zws_avi_message_list_add_message_networks(message_list, &security_scene_list) != ZWS_STATUS_OK)
	{
        return ZWS_STATUS_ERROR;
	}

	return ZWS_STATUS_OK;
}

