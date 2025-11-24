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
 * @file    zwp_avi_security_scene_get_sup.c
 *
 * @brief   AVI message 'security_scene_get_sup'
 * @details Handles the AVI message 'security_scene_get_sup'.
 *
 * @author  Arif
 *
 * @version 1.0 - 2015-06-25
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
#include "zwp_avi_scene.h"
#include "zwscn_security_scene_api.h"
#include "zwp_avi_security_scene_common.h"


/** Scene get_sup data structure*/
typedef struct {
    char	*timezone;          	/**< timezone string */
	int		max_security_scenes;
	int		max_security_arm_events;
	int		max_security_disarm_events;
	int		max_security_alarm_events;
} zwscn_security_scene_get_sup_info_t;


/**
zwp_avi_scene_get_sup_execute - Invoke library function to execute request
@param[in]	net	            Network
@param[in]	arg	            Parameters
@param[out]	result_p	    Result of the execution
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_security_scene_get_sup_execute(zwp_net_t net, void *arg, void **result_p)
{
    zwscn_security_scene_get_sup_info_t    *scene_get_sup_info;

    //Initialize to NULL in case it is freed later on
    *result_p = NULL;

    ZWP_LOG(ZWP_LOG_DEBUG, "Execute - security_scene_get_sup");

    if ((scene_get_sup_info = (zwscn_security_scene_get_sup_info_t *)ZWP_CALLOC(1, sizeof(zwscn_security_scene_get_sup_info_t))) == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
        return ZWP_STATUS_ERROR;
    }

	if(zwscn_get_network_timezone(net, &(scene_get_sup_info->timezone)) != ZWSCN_ERR_NONE) {
		scene_get_sup_info->timezone = NULL;
	}
	
	scene_get_sup_info->max_security_scenes = zwscn_get_max_num_security_scenes(net);
	scene_get_sup_info->max_security_arm_events = zwscn_get_max_num_arm_security_events(net);
	scene_get_sup_info->max_security_disarm_events = zwscn_get_max_num_disarm_security_events(net);
	scene_get_sup_info->max_security_alarm_events = zwscn_get_max_num_alarm_security_events(net);
	
    //Save the result
    *result_p = scene_get_sup_info;

    return ZWP_STATUS_OK;
}


/**
zwp_avi_security_scene_get_sup_result - Encode the scene get sup response to AVRO format
@param[in]	result	            Result
@param[out]	arg_value	        Output in AVRO object
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_security_scene_get_sup_result(void *result, avro_value_t *arg_value)
{
    zwscn_security_scene_get_sup_info_t    *scene_get_sup_info = (zwscn_security_scene_get_sup_info_t *)result;
//    avro_value_t    scene_get_sup;

    if (!result)
    {
        ZWP_LOG(ZWP_LOG_ERR, "result parameter is NULL");
        return ZWP_STATUS_ERROR;
    }

//    if (avro_value_get_by_name(arg_value, "SceneGetSup", &scene_get_sup, NULL) != 0)
//    {
//        ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'SceneGetSup': %s", avro_strerror());
//        return ZWP_STATUS_ERROR;
//    }
//   
	if ((zw_avro_field_set_string(arg_value, scene_arg_timezone, (char *) scene_get_sup_info->timezone) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_int(arg_value, security_scene_arg_max_scenes, scene_get_sup_info->max_security_scenes) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_int(arg_value, security_scene_arg_max_arm_events, scene_get_sup_info->max_security_arm_events) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_int(arg_value, security_scene_arg_max_disarm_events, scene_get_sup_info->max_security_disarm_events) != ZWP_STATUS_OK)
		|| (zw_avro_field_set_int(arg_value, security_scene_arg_max_alarm_events, scene_get_sup_info->max_security_alarm_events) != ZWP_STATUS_OK))
	{
		return ZWP_STATUS_ERROR;
	}

    return ZWP_STATUS_OK;
}


/**
zwp_avi_security_scene_get_sup_result_free - Free the result_p from zwp_avi_XXX_execute()
@param[in]	result	            Result pointer
@return
*/
static void zwp_avi_security_scene_get_sup_result_free(void *result)
{
    zwscn_security_scene_get_sup_info_t   *scene_get_sup_info = (zwscn_security_scene_get_sup_info_t *)result;

    if (scene_get_sup_info)
    {
        ZWP_FREE(scene_get_sup_info->timezone);
        ZWP_FREE(scene_get_sup_info);
    }
}


/** Scene get sup AVRO message handler registration data structure */
static zwp_avi_message_networks_t security_scene_get_sup =
{
    "security_scene_get_sup",								/**< AVRO message name*/
    NULL, NULL,
    zwp_avi_security_scene_get_sup_execute,					/**< Invoke library function to execute request*/
    ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,		/**< Flag to control execution based on validity of network */
    zwp_avi_security_scene_get_sup_result,					/**< Encode the response to AVRO format*/
    zwp_avi_security_scene_get_sup_result_free				/**< Free the result_p from zwp_avi_XXX_execute()*/
};


/**
zwp_avi_security_scene_get_sup_init - Scene get sup initialization
@param[in]	message_list	    Message list
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_avi_security_scene_get_sup_init(zwu_array_list_t message_list)
{
    if (zwp_avi_message_list_add_message_networks(message_list, &security_scene_get_sup) != ZWP_STATUS_OK)
    {
        return ZWP_STATUS_ERROR;
    }

    return ZWP_STATUS_OK;
}
