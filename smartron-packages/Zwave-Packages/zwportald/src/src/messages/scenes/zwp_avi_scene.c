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
 * @file    zwp_avi_scene.c
 *
 * @brief   Common file for Scenes AVI message
 * @details Common file for Scenes AVI message.
 *
 * @author  Arif & David
 *
 * @version 1.0 - 2013-10-02
 * - Initial version
 */

#include "zwp_status.h"
#include "zwp_avi_message_networks.h"
#include "zwp_log.h"
#include "zwp_memory.h"
#include "zwp_error.h"
#include "zwscn_scene_api.h"
#include "zwp_avi_scene.h"
#include "zwave/ZW_classcmd.h"
#include "zip_api.h"
#include "zwscn_device_args.h"
#include "zwp_avi_scene_common.h"
#include "zwscn_security_scene_api.h"
#include "zwp_avi_security_scene_common.h"

const char scene_api_1[] 			= "scene";
const char scene_api_2[] 			= "zw_scene";
const char scene_tag[] 				= "scene";
const char scene_arg_scened[] 		= "scened";
const char scene_arg_name[] 		= "name";
const char scene_arg_active[] 		= "active";
const char scene_arg_status[] 		= "status";
const char scene_arg_sutime[] 		= "sutime";
const char scene_arg_lkstatus[] 	= "lkstatus";
const char scene_arg_lksutime[] 	= "lksutime";
const char scene_arg_letrigger[] 	= "letrigger";
const char scene_arg_letime[] 		= "letime";
const char scene_arg_triggers[] 	= "triggers";
const char scene_arg_desc[] 		= "desc";
const char scene_arg_uri[] 		    = "uri";
const char scene_arg_cmd[] 		    = "cmd";
const char scene_arg_ifd[] 		    = "ifd";
const char scene_arg_max_scenes[]     = "max_scenes";
const char scene_arg_max_actions[]    = "max_actions";
const char scene_arg_max_schedules[]  = "max_schedules";
const char scene_arg_max_events[]     = "max_events";
const char scene_arg_timezone[]     = "timezone";

#define MAX_STATE_LOG_SCENES   8   			/**< Maximum number of scenes state logs that can be queried with one CMD_SCENE_GET_STATELOG command*/
#define MAX_STATE_LOG_SECURITY_SCENES   8   /**< Maximum number of security scenes state logs that can be queried with one CMD_SCENE_GET_STATELOG command*/
#define MAX_STATE_LOG   (MAX_STATE_LOG_SCENES + MAX_STATE_LOG_SECURITY_SCENES)   /**< Maximum number of state logs that can be queried with one CMD_SCENE_GET_STATELOG command*/

/** Scene state log*/
typedef struct
{
	uint32_t    type;
	uint32_t    curr_op;
	uint32_t    curr_op_scened;
	uint32_t    curr_op_trigger;
	uint32_t    last_op_scened;
	uint32_t    last_op_trigger;
	uint64_t    last_op_time;
} zwscn_sta_log_t;

/** Security Scene state log*/
typedef struct
{
	uint32_t    type;
	uint32_t    counter;
	uint32_t    scened;
} zwscn_security_scene_sta_log_t;

/** Scene get state log data structure */
typedef struct
{
    int                 sta_log_count;          								/**< Number of elements in state log array*/
    zwscn_sta_log_t     sta_log[MAX_STATE_LOG_SCENES]; 							/**< State log array*/
	int                 security_scene_sta_log_count;          					/**< Number of elements in security scenes state log array*/
    zwscn_security_scene_sta_log_t     security_scene_sta_log[MAX_STATE_LOG_SECURITY_SCENES]; 	/**< Security scenes State log array*/
} zwscn_get_sta_log_t;

/** Scene get data structure */
typedef struct
{
    zwscn_scene_t       param;           /**< Parameters such as scene id, name, active */
    int                 action_cnt;      /**< Number of elements in action array*/
    int                 sched_cnt;       /**< Number of elements in schedule array*/
    int                 event_cnt;       /**< Number of elements in event array*/
    zwscn_action_t      *action;         /**< Action array*/
    zwscn_schedule_t    *sched;          /**< Schedule array*/
    zwscn_event_t       *event;          /**< Event array*/
} zwscn_get_t;

/**
zwp_avi_param_get - Get parameter as specified by command class and command and create a copy of it
@param[in]	param_data          Parameter data
@param[in]	cmd_cls             Command class as defined by Z-wave
@param[in]	cmd                 Command as defined by Z-Ware Web Developer's Guide
@param[out]	param	            A copy of param_data
@return ZWP_STATUS_XXX
*/
#if 0
static zwp_status_t zwp_avi_param_get(void *param_data, uint16_t cmd_cls, uint8_t cmd, void **param)
{

    switch (cmd_cls)
    {
        case COMMAND_CLASS_BASIC:
            if (cmd == 4) //CMD_BASIC_SET
            {
                zwscn_args_basic_set_t  *arg;

                if ((arg = (zwscn_args_basic_set_t *)ZWP_CALLOC(1, sizeof(zwscn_args_basic_set_t))) == NULL)
                {
                    return ZWP_STATUS_ERROR;
                }

                *arg = *((zwscn_args_basic_set_t *)param_data);
                *param = arg;
                return ZWP_STATUS_OK;
            }
            break;

        case COMMAND_CLASS_SWITCH_BINARY:
            if (cmd == 4) //CMD_BINARY_SWITCH_SET
            {
                zwscn_args_switch_binary_set_t  *arg;

                if ((arg = (zwscn_args_switch_binary_set_t *)ZWP_CALLOC(1, sizeof(zwscn_args_switch_binary_set_t))) == NULL)
                {
                    return ZWP_STATUS_ERROR;
                }

                *arg = *((zwscn_args_switch_binary_set_t *)param_data);
                *param = arg;
                return ZWP_STATUS_OK;
            }
            break;

        case COMMAND_CLASS_SWITCH_MULTILEVEL:
            if (cmd == 4) //CMD_MULTILEVEL_SWITCH_SET
            {
                zwscn_args_switch_multilevel_set_t  *arg;

                if ((arg = (zwscn_args_switch_multilevel_set_t *)ZWP_CALLOC(1, sizeof(zwscn_args_switch_multilevel_set_t))) == NULL)
                {
                    return ZWP_STATUS_ERROR;
                }

                *arg = *((zwscn_args_switch_multilevel_set_t *)param_data);
                *param = arg;
                return ZWP_STATUS_OK;
            }
            break;

        case COMMAND_CLASS_DOOR_LOCK:
            if ((cmd == 4) || (cmd == 2)) //CMD_DLOCK_OP_SET || CMD_DLOCK_OP_GET
            {
                zwscn_args_door_lock_operation_set_t  *arg;

                if ((arg = (zwscn_args_door_lock_operation_set_t *)ZWP_CALLOC(1, sizeof(zwscn_args_door_lock_operation_set_t))) == NULL)
                {
                    return ZWP_STATUS_ERROR;
                }

                *arg = *((zwscn_args_door_lock_operation_set_t *)param_data);
                *param = arg;
                return ZWP_STATUS_OK;
            }
            break;

        case COMMAND_CLASS_THERMOSTAT_SETPOINT:
            if (cmd == 4) //CMD_THRMO_SETPT_SET
            {
                zwscn_args_thermostat_setpoint_set_t  *arg;

                if ((arg = (zwscn_args_thermostat_setpoint_set_t *)ZWP_CALLOC(1, sizeof(zwscn_args_thermostat_setpoint_set_t))) == NULL)
                {
                    return ZWP_STATUS_ERROR;
                }

                *arg = *((zwscn_args_thermostat_setpoint_set_t *)param_data);
                *param = arg;
                return ZWP_STATUS_OK;
            }
            break;

        case COMMAND_CLASS_SENSOR_BINARY:
            if (cmd == 2) //CMD_BINARY_SENSOR_GET
            {
                zwscn_args_sensor_binary_t  *arg;

                if ((arg = (zwscn_args_sensor_binary_t *)ZWP_CALLOC(1, sizeof(zwscn_args_sensor_binary_t))) == NULL)
                {
                    return ZWP_STATUS_ERROR;
                }

                *arg = *((zwscn_args_sensor_binary_t *)param_data);
                *param = arg;
                return ZWP_STATUS_OK;
            }
            break;

        case COMMAND_CLASS_SENSOR_MULTILEVEL:
            if (cmd == 2) //CMD_MULTILEVEL_SENSOR_GET
            {
                zwscn_args_sensor_multilevel_t  *arg;

                if ((arg = (zwscn_args_sensor_multilevel_t *)ZWP_CALLOC(1, sizeof(zwscn_args_sensor_multilevel_t))) == NULL)
                {
                    return ZWP_STATUS_ERROR;
                }

                *arg = *((zwscn_args_sensor_multilevel_t *)param_data);
                *param = arg;
                return ZWP_STATUS_OK;
            }
            break;

        default:
            ZWP_LOG(ZWP_LOG_ERR, "Unsupported command class:%04Xh", (unsigned)cmd_cls);
            break;
    }

    return ZWP_STATUS_ERROR;
}
#endif


/**
zwp_avi_scene_get_action_list - Get action list of a specified scene id
@param[in] 	net				Network runtime context
@param[in] 	scene_id		Scene id
@param[in]	action_cnt      Number of elements in action list
@param[out]	action	        Output action
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_scene_get_action_list(void *net, uint32_t scene_id, int action_cnt, zwscn_action_t *action)
{
	int             i;
    zwscn_action_p  scene_action;

	for (i=0; i<action_cnt; i++)
	{
        if (zwscn_get_action_by_index(net, scene_id, i, &scene_action) != ZWSCN_ERR_NONE)
        {
			return ZWP_STATUS_ERROR;
        }

        //Copy the scene action
        action[i] = *scene_action;
        action[i].device_args = NULL;
        action[i].device_args_size = 0;
        action[i].id = zwp_scn_act_evt_id_2_ifd(scene_action->id);


        if (scene_action->device_args && scene_action->device_args_size)
        {   //create and copy parameters
            if ((action[i].device_args = ZWP_MALLOC(scene_action->device_args_size)) == NULL)
            {
                ZWP_FREE(scene_action->device_args);
                ZWP_FREE(scene_action);
                return ZWP_STATUS_ERROR;
            }

            memcpy(action[i].device_args, scene_action->device_args, scene_action->device_args_size);
            action[i].device_args_size = scene_action->device_args_size;
            ZWP_FREE(scene_action->device_args);

        }

        ZWP_FREE(scene_action);
	}

    return ZWP_STATUS_OK;
}


/**
zwp_avi_scene_get_schedule_list - Get schedule list of a specified scene id
@param[in] 	net				Network runtime context
@param[in] 	scene_id		Scene id
@param[in]	schedule_cnt    Number of elements in schedule list
@param[out]	schedule	    Output schedule
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_scene_get_schedule_list(void *net, uint32_t scene_id, int schedule_cnt, zwscn_schedule_t *schedule)
{
    zwscn_schedule_p    scene_sched;
	int                 i;

	for (i=0; i<schedule_cnt; i++)
	{
        if (zwscn_get_schedule_by_index(net, scene_id, i, &scene_sched) != ZWSCN_ERR_NONE)
        {
			return ZWP_STATUS_ERROR;
        }

        //Copy the scene schedule
        schedule[i] = *scene_sched;
        ZWP_FREE(scene_sched);

	}

    return ZWP_STATUS_OK;
}


/**
zwp_avi_scene_get_event_list - Get event list of a specified scene id
@param[in] 	net				Network runtime context
@param[in] 	scene_id		Scene id
@param[in]	event_cnt       Number of elements in event list
@param[out]	event	        Output event
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_scene_get_event_list(void *net, uint32_t scene_id, int event_cnt, zwscn_event_t *event)
{
	int             i;
    zwscn_event_p   scene_event;

	for (i=0; i<event_cnt; i++)
	{
        if (zwscn_get_event_by_index(net, scene_id, i, &scene_event) != ZWSCN_ERR_NONE)
        {
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
zwp_avi_scene_get_execute - Invoke library function to execute request
@param[in]	net	            Network
@param[in]	arg	            Parameters
@param[out]	result_p	    Result of the execution
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_scene_get_execute(zwp_net_t net, void *arg, void **result_p)
{
    int             i;
    int             element_cnt;
    zwscn_get_t     *scn_get_arg;
    zwscn_scene_p   scene;
    uint32_t        *scene_id = (uint32_t *)arg;

    //Initialize to NULL in case it is freed later on
    *result_p = NULL;

    ZWP_LOG(ZWP_LOG_DEBUG, "Execute - scene_get");

    if (arg == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, "arg is NULL\n");
        return ZWP_STATUS_ERROR;
    }

    if ((scn_get_arg = (zwscn_get_t *)ZWP_CALLOC(1, sizeof(zwscn_get_t))) == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
        return ZWP_STATUS_ERROR;
    }

    //Get scene
    if (zwscn_get_scene(net, *scene_id, &scene) != ZWSCN_ERR_NONE)
    {
        goto l_SCN_GET_EXE_ERR1;
    }
    scn_get_arg->param = *scene;
    ZWP_FREE(scene);

    //Get action
    element_cnt = zwscn_count_actions(net, *scene_id);
    if (element_cnt <= 0)
    {
        goto l_SCN_GET_EXE_ERR1;
    }

    //Allocate buffer
    if ((scn_get_arg->action = (zwscn_action_t *)ZWP_CALLOC(element_cnt, sizeof(zwscn_action_t))) == NULL)
    {
        goto l_SCN_GET_EXE_ERR1;
    }

    scn_get_arg->action_cnt = element_cnt;

    if (zwp_avi_scene_get_action_list(net, *scene_id, element_cnt, scn_get_arg->action) != ZWP_STATUS_OK)
    {
        goto l_SCN_GET_EXE_ERR2;
    }

    //Get schedule
    element_cnt = zwscn_count_schedules(net, *scene_id);
    if (element_cnt < 0)
    {
        goto l_SCN_GET_EXE_ERR2;
    }

    if (element_cnt > 0)
    {
        //Allocate buffer
        if ((scn_get_arg->sched = (zwscn_schedule_t *)ZWP_CALLOC(element_cnt, sizeof(zwscn_schedule_t))) == NULL)
        {
            goto l_SCN_GET_EXE_ERR2;
        }

        scn_get_arg->sched_cnt = element_cnt;

        if (zwp_avi_scene_get_schedule_list(net, *scene_id, element_cnt, scn_get_arg->sched) != ZWP_STATUS_OK)
        {
            goto l_SCN_GET_EXE_ERR3;
        }
    }

    //Get event
    element_cnt = zwscn_count_events(net, *scene_id);
    if (element_cnt < 0)
    {
        goto l_SCN_GET_EXE_ERR3;
    }

    if (element_cnt > 0)
    {
        //Allocate buffer
        if ((scn_get_arg->event = (zwscn_event_t *)ZWP_CALLOC(element_cnt, sizeof(zwscn_event_t))) == NULL)
        {
            goto l_SCN_GET_EXE_ERR3;
        }

        scn_get_arg->event_cnt = element_cnt;

        if (zwp_avi_scene_get_event_list(net, *scene_id, element_cnt, scn_get_arg->event) != ZWP_STATUS_OK)
        {
            goto l_SCN_GET_EXE_ERR4;
        }
    }

    *result_p = scn_get_arg;

    return ZWP_STATUS_OK;

l_SCN_GET_EXE_ERR4:
    for (i=0; i<scn_get_arg->event_cnt; i++)
    {
        ZWP_FREE(scn_get_arg->event[i].device_args);
    }
    ZWP_FREE(scn_get_arg->event);

l_SCN_GET_EXE_ERR3:
    ZWP_FREE(scn_get_arg->sched);

l_SCN_GET_EXE_ERR2:
    for (i=0; i<scn_get_arg->action_cnt; i++)
    {
        ZWP_FREE(scn_get_arg->action[i].device_args);
    }
    ZWP_FREE(scn_get_arg->action);

l_SCN_GET_EXE_ERR1:
    ZWP_FREE(scn_get_arg);
    return ZWP_STATUS_ERROR;
}


/**
zwp_avi_scene_get_result - Encode the "scene get" response to AVRO format
@param[in]	result	       Result
@param[out]	arg_value	   Output in AVRO object
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_scene_get_result(void *result, avro_value_t *arg_value)
{
    zwscn_get_t  *scn_get_arg = (zwscn_get_t *)result;
    const char   *uri;
    int          i;
    avro_value_t action_list_value;
    avro_value_t sched_list_value;
    avro_value_t event_list_value;
    avro_value_t scene_rpt_value;
    avro_value_t action_value = {0};
    avro_value_t sched_value = {0};
    avro_value_t event_value = {0};
    uint16_t     cmd_cls;

    if (!result)
    {
        ZWP_LOG(ZWP_LOG_ERR, "result parameter is NULL\n");
        return ZWP_STATUS_ERROR;
    }

    if (avro_value_get_by_name(arg_value, "report", &scene_rpt_value, NULL) != 0)
    {
        ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'report': %s", avro_strerror());
        return ZWP_STATUS_ERROR;
    }

    if ((zw_avro_field_set_int(&scene_rpt_value, scene_arg_scened, (int32_t)scn_get_arg->param.id) != ZWP_STATUS_OK)
        || (zw_avro_field_set_string(&scene_rpt_value, scene_arg_name, (char *)scn_get_arg->param.name) != ZWP_STATUS_OK)
        || (zw_avro_field_set_int(&scene_rpt_value, scene_arg_active, (int32_t)scn_get_arg->param.active) != ZWP_STATUS_OK)
        || (zw_avro_field_set_int(&scene_rpt_value, scene_arg_status, (int32_t)scn_get_arg->param.status) != ZWP_STATUS_OK)
        || (zw_avro_field_set_long(&scene_rpt_value, scene_arg_sutime, (int64_t)scn_get_arg->param.sutime) != ZWP_STATUS_OK)
        || (zw_avro_field_set_int(&scene_rpt_value, scene_arg_lkstatus, (int32_t)scn_get_arg->param.lkstatus) != ZWP_STATUS_OK)
        || (zw_avro_field_set_long(&scene_rpt_value, scene_arg_lksutime, (int64_t)scn_get_arg->param.lksutime) != ZWP_STATUS_OK)
        || (zw_avro_field_set_int(&scene_rpt_value, scene_arg_triggers, (int32_t)scn_get_arg->param.triggers) != ZWP_STATUS_OK)
        || (zw_avro_field_set_int(&scene_rpt_value, scene_arg_letrigger, (int32_t)scn_get_arg->param.letrigger) != ZWP_STATUS_OK)
        || (zw_avro_field_set_long(&scene_rpt_value, scene_arg_letime, (int64_t)scn_get_arg->param.letime) != ZWP_STATUS_OK))
    {
        return ZWP_STATUS_ERROR;
    }

    //Get action, schedule and event lists
    if (avro_value_get_by_name(arg_value, "scene_action_list", &action_list_value, NULL) != 0)
    {
        ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'scene_action_list': %s", avro_strerror());
        return ZWP_STATUS_ERROR;
    }

    if (avro_value_get_by_name(arg_value, "scene_schedule_list", &sched_list_value, NULL) != 0)
    {
        ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'scene_schedule_list': %s", avro_strerror());
        return ZWP_STATUS_ERROR;
    }

    if (avro_value_get_by_name(arg_value, "scene_event_list", &event_list_value, NULL) != 0)
    {
        ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'scene_event_list': %s", avro_strerror());
        return ZWP_STATUS_ERROR;
    }

    // Process action
    if ((scn_get_arg->action_cnt > 0) && scn_get_arg->action)
    {
        for (i=0; i<scn_get_arg->action_cnt; i++)
        {
            //Get URI
            cmd_cls = (uint16_t)((scn_get_arg->action[i].id >> 16) & 0xFFFF);
            uri = zwp_avi_uri_get(cmd_cls);

            if (!uri)
            {
                return ZWP_STATUS_ERROR;
            }

            if (avro_value_append(&action_list_value, &action_value, 0) != 0)
            {
                return ZWP_STATUS_ERROR;
            }

            if ((zw_avro_field_set_int(&action_value, "actype", (int32_t)scn_get_arg->action[i].type) != ZWP_STATUS_OK)
                || (zw_avro_field_set_int(&action_value, scene_arg_status, (int32_t)scn_get_arg->action[i].status) != ZWP_STATUS_OK)
                || (zw_avro_field_set_long(&action_value, scene_arg_sutime, (int64_t)scn_get_arg->action[i].sutime) != ZWP_STATUS_OK)
                || (zw_avro_field_set_int(&action_value, scene_arg_lkstatus, (int32_t)scn_get_arg->action[i].lkstatus) != ZWP_STATUS_OK)
                || (zw_avro_field_set_long(&action_value, scene_arg_lksutime, (int64_t)scn_get_arg->action[i].lksutime) != ZWP_STATUS_OK)
                || (zw_avro_field_set_string(&action_value, scene_arg_uri, (char *)uri) != ZWP_STATUS_OK)
                || (zw_avro_field_set_int(&action_value, scene_arg_cmd, (int32_t)scn_get_arg->action[i].cmd) != ZWP_STATUS_OK)
                || (zw_avro_field_set_int(&action_value, scene_arg_ifd, (int32_t)scn_get_arg->action[i].id) != ZWP_STATUS_OK))
            {
                return ZWP_STATUS_ERROR;
            }

            if (scn_get_arg->action[i].device_args)
            {
                if (zwp_avi_param_to_avro(scn_get_arg->action[i].device_args, cmd_cls, scn_get_arg->action[i].cmd, &action_value) != ZWP_STATUS_OK)
                {
                    return ZWP_STATUS_ERROR;
                }
            }
        }
    }

    // Process schedule
    if ((scn_get_arg->sched_cnt > 0) && scn_get_arg->sched)
    {
        for (i=0; i<scn_get_arg->sched_cnt; i++)
        {
            if (avro_value_append(&sched_list_value, &sched_value, 0) != 0)
            {
                return ZWP_STATUS_ERROR;
            }

            if ((zw_avro_field_set_int(&sched_value, "sctype", (int32_t)scn_get_arg->sched[i].type) != ZWP_STATUS_OK)
                || (zw_avro_field_set_int(&sched_value, "day", (int32_t)scn_get_arg->sched[i].day) != ZWP_STATUS_OK)
                || (zw_avro_field_set_int(&sched_value, "hour", (int32_t)scn_get_arg->sched[i].hour) != ZWP_STATUS_OK)
                || (zw_avro_field_set_int(&sched_value, "minute", (int32_t)scn_get_arg->sched[i].minute) != ZWP_STATUS_OK))
            {
                return ZWP_STATUS_ERROR;
            }
        }
    }

    // Process event
    if ((scn_get_arg->event_cnt > 0) && scn_get_arg->event)
    {
        for (i=0; i<scn_get_arg->event_cnt; i++)
        {
            //Get URI
            cmd_cls = (uint16_t)((scn_get_arg->event[i].id >> 16) & 0xFFFF);
            uri = zwp_avi_uri_get(cmd_cls);

            if (!uri)
            {
                return ZWP_STATUS_ERROR;
            }

            if (avro_value_append(&event_list_value, &event_value, 0) != 0)
            {
                return ZWP_STATUS_ERROR;
            }

            if ((zw_avro_field_set_int(&event_value, "evtype", (int32_t)scn_get_arg->event[i].type) != ZWP_STATUS_OK)
                || (zw_avro_field_set_int(&event_value, scene_arg_status, (int32_t)scn_get_arg->event[i].status) != ZWP_STATUS_OK)
                || (zw_avro_field_set_long(&event_value, scene_arg_sutime, (int64_t)scn_get_arg->event[i].lrtime) != ZWP_STATUS_OK)
                || (zw_avro_field_set_string(&event_value, scene_arg_uri, (char *)uri) != ZWP_STATUS_OK)
                || (zw_avro_field_set_int(&event_value, scene_arg_cmd, (int32_t)scn_get_arg->event[i].cmd) != ZWP_STATUS_OK)
                || (zw_avro_field_set_int(&event_value, scene_arg_ifd, (int32_t)scn_get_arg->event[i].id) != ZWP_STATUS_OK))
            {
                return ZWP_STATUS_ERROR;
            }

            if (scn_get_arg->event[i].device_args)
            {
                if (zwp_avi_param_to_avro(scn_get_arg->event[i].device_args, cmd_cls, scn_get_arg->event[i].cmd, &event_value) != ZWP_STATUS_OK)
                {
                    return ZWP_STATUS_ERROR;
                }
            }
        }
    }

    return ZWP_STATUS_OK;

}


/**
zwp_avi_scene_get_result_free - Free the result_p from zwp_avi_XXX_execute()
@param[in]	result	            Result pointer
@return
*/
static void zwp_avi_scene_get_result_free(void *result)
{
    zwscn_get_t     *scn_get_arg = (zwscn_get_t *)result;
    int             i;

    //Free the result_p from zwp_avi_scene_get_execute()
    if (scn_get_arg)
    {
        for (i=0; i<scn_get_arg->event_cnt; i++)
        {
            ZWP_FREE(scn_get_arg->event[i].device_args);
        }
        ZWP_FREE(scn_get_arg->event);

        ZWP_FREE(scn_get_arg->sched);

        for (i=0; i<scn_get_arg->action_cnt; i++)
        {
            ZWP_FREE(scn_get_arg->action[i].device_args);
        }
        ZWP_FREE(scn_get_arg->action);

        ZWP_FREE(scn_get_arg);
    }
}


/** Scene get details AVRO message handler registration data structure */
static zwp_avi_message_networks_t scene_get =
{
    "scene_get",                               /**< AVRO message name*/
    zwp_avi_scene_generic_arg_handler,         /**< Command specific AVRO argument parser; NULL if no such arguments */
    zwp_avi_scene_generic_arg_free,            /**< Free argument */
    zwp_avi_scene_get_execute,                 /**< Invoke library function to execute request*/
    ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,   /**< Flag to control execution based on validity of network */
    zwp_avi_scene_get_result,                  /**< Encode the response to AVRO format*/
    zwp_avi_scene_get_result_free              /**< Free the result_p from zwp_avi_XXX_execute()*/
};


/**
zwp_avi_scene_get_init - Scene get details initialization
@param[in]	message_list	    Message list
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_avi_scene_get_init(zwu_array_list_t message_list)
{
    if (zwp_avi_message_list_add_message_networks(message_list, &scene_get) != ZWP_STATUS_OK)
    {
        return ZWP_STATUS_ERROR;
    }

    return ZWP_STATUS_OK;
}

//---------------------------------------------------------------------------------------------------------

/**
zwp_avi_scene_del_execute - Invoke library function to execute request
@param[in]	net	            Network
@param[in]	arg	            Parameters
@param[out]	result_p	    Result of the execution
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_scene_del_execute(zwp_net_t net, void *arg, void **result_p)
{
    uint32_t        *scene_id = (uint32_t *)arg;
    uint32_t        *scene_id_result;

    //Initialize to NULL in case it is freed later on
    *result_p = NULL;

    ZWP_LOG(ZWP_LOG_DEBUG, "Execute - scene_del");

    if (arg == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, "arg is NULL\n");
        return ZWP_STATUS_ERROR;
    }

    if ((scene_id_result = (uint32_t *)ZWP_MALLOC(sizeof(uint32_t))) == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
        return ZWP_STATUS_ERROR;
    }

    //Delete scene
    if (zwscn_del_scene(net, *scene_id) != ZWSCN_ERR_NONE)
    {
        ZWP_FREE(scene_id_result);
        return ZWP_STATUS_ERROR;
    }

    *scene_id_result = *scene_id;
    *result_p = scene_id_result;

    return ZWP_STATUS_OK;
}


/** Scene get details AVRO message handler registration data structure */
static zwp_avi_message_networks_t scene_del =
{
    "scene_del",                               /**< AVRO message name*/
    zwp_avi_scene_generic_arg_handler,         /**< Command specific AVRO argument parser; NULL if no such arguments */
    zwp_avi_scene_generic_arg_free,            /**< Free argument */
    zwp_avi_scene_del_execute,                 /**< Invoke library function to execute request*/
    ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,   /**< Flag to control execution based on validity of network */
    zwp_avi_scene_generic_result,              /**< Encode the response to AVRO format*/
    zwp_avi_scene_generic_result_free          /**< Free the result_p from zwp_avi_XXX_execute()*/
};


/**
zwp_avi_scene_del_init - Scene delete initialization
@param[in]	message_list	    Message list
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_avi_scene_del_init(zwu_array_list_t message_list)
{
    if (zwp_avi_message_list_add_message_networks(message_list, &scene_del) != ZWP_STATUS_OK)
    {
        return ZWP_STATUS_ERROR;
    }

    return ZWP_STATUS_OK;
}

//---------------------------------------------------------------------------------------------------------

/**
zwp_avi_scene_exec_execute - Invoke library function to execute request
@param[in]	net	            Network
@param[in]	arg	            Parameters
@param[out]	result_p	    Result of the execution
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_scene_exec_execute(zwp_net_t net, void *arg, void **result_p)
{
    uint32_t        *scene_id = (uint32_t *)arg;
    uint32_t        *scene_id_result;

    //Initialize to NULL in case it is freed later on
    *result_p = NULL;

    ZWP_LOG(ZWP_LOG_DEBUG, "Execute - scene_exec");

    if (arg == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, "arg is NULL\n");
        return ZWP_STATUS_ERROR;
    }

    //Execute scene
    if (zwscn_execute_scene(net, *scene_id) != ZWSCN_ERR_NONE)
    {
        return ZWP_STATUS_ERROR;
    }

    if ((scene_id_result = (uint32_t *)ZWP_MALLOC(sizeof(uint32_t))) == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
        return ZWP_STATUS_ERROR;
    }

    *scene_id_result = *scene_id;
    *result_p = scene_id_result;

    return ZWP_STATUS_OK;
}


/** Scene execute AVRO message handler registration data structure */
static zwp_avi_message_networks_t scene_exec =
{
    "scene_exec",                               /**< AVRO message name*/
    zwp_avi_scene_generic_arg_handler,          /**< Command specific AVRO argument parser; NULL if no such arguments */
    zwp_avi_scene_generic_arg_free,             /**< Free argument */
    zwp_avi_scene_exec_execute,                 /**< Invoke library function to execute request*/
    ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,    /**< Flag to control execution based on validity of network */
    zwp_avi_scene_generic_result,               /**< Encode the response to AVRO format*/
    zwp_avi_scene_generic_result_free           /**< Free the result_p from zwp_avi_XXX_execute()*/
};


/**
zwp_avi_scene_exec_init - Scene execute initialization
@param[in]	message_list	    Message list
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_avi_scene_exec_init(zwu_array_list_t message_list)
{
    if (zwp_avi_message_list_add_message_networks(message_list, &scene_exec) != ZWP_STATUS_OK)
    {
        return ZWP_STATUS_ERROR;
    }

    return ZWP_STATUS_OK;
}

//---------------------------------------------------------------------------------------------------------

/**
zwp_avi_scene_updt_arg_handler - AVRO argument parser
@param[in]	arg_value	    Parameters encoded in AVRO
@param[out]	args_p	        Decoded AVRO parameters
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_scene_updt_arg_handler(avro_value_t *arg_value, void **args_p)
{
    int32_t *scene_id;
    int32_t updt_all;

    if (zw_avro_field_get_int(arg_value, "all", &updt_all) != ZWP_STATUS_OK)
    {
        return ZWP_STATUS_ERROR;
    }

    if ((scene_id = (int32_t *)ZWP_MALLOC(sizeof(int32_t))) == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
        return ZWP_STATUS_ERROR;
    }

    if (updt_all == 1)
    {   //Update all, set scene id to zero
        *scene_id = 0;
    }
    else
    {
        if (zw_avro_field_get_int(arg_value, scene_arg_scened, scene_id) != ZWP_STATUS_OK)
        {
            ZWP_FREE(scene_id);
            return ZWP_STATUS_ERROR;
        }
    }

    *args_p = scene_id;

    return ZWP_STATUS_OK;
}


/**
zwp_avi_scene_updt_execute - Invoke library function to execute request
@param[in]	net	            Network
@param[in]	arg	            Parameters
@param[out]	result_p	    Result of the execution
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_scene_updt_execute(zwp_net_t net, void *arg, void **result_p)
{
    uint32_t        *scene_id = (uint32_t *)arg;
    uint32_t        *scene_id_result;

    //Initialize to NULL in case it is freed later on
    *result_p = NULL;

    ZWP_LOG(ZWP_LOG_DEBUG, "Execute - scene_updt");

    if (arg == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, "arg is NULL\n");
        return ZWP_STATUS_ERROR;
    }

    if ((scene_id_result = (uint32_t *)ZWP_MALLOC(sizeof(uint32_t))) == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
        return ZWP_STATUS_ERROR;
    }

    //Update scene
    if (*scene_id == 0)
    {   //Update all
        if (zwscn_status_update_all_scenes(net) != ZWSCN_ERR_NONE)
        {
            ZWP_FREE(scene_id_result);
            return ZWP_STATUS_ERROR;
        }
    }
    else
    {   //Update single scene
        if (zwscn_status_update_scene(net, *scene_id) != ZWSCN_ERR_NONE)
        {
            ZWP_FREE(scene_id_result);
            return ZWP_STATUS_ERROR;
        }
    }

    *scene_id_result = *scene_id;
    *result_p = scene_id_result;

    return ZWP_STATUS_OK;
}


/** Scene update AVRO message handler registration data structure */
static zwp_avi_message_networks_t scene_updt =
{
    "scene_updt",                              /**< AVRO message name*/
    zwp_avi_scene_updt_arg_handler,            /**< Command specific AVRO argument parser; NULL if no such arguments */
    zwp_avi_scene_generic_arg_free,            /**< Free argument */
    zwp_avi_scene_updt_execute,                /**< Invoke library function to execute request*/
    ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,   /**< Flag to control execution based on validity of network */
    zwp_avi_scene_generic_result,              /**< Encode the response to AVRO format*/
    zwp_avi_scene_generic_result_free          /**< Free the result_p from zwp_avi_XXX_execute()*/
};


/**
zwp_avi_scene_updt_init - Scene update initialization
@param[in]	message_list	    Message list
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_avi_scene_updt_init(zwu_array_list_t message_list)
{
    if (zwp_avi_message_list_add_message_networks(message_list, &scene_updt) != ZWP_STATUS_OK)
    {
        return ZWP_STATUS_ERROR;
    }

    return ZWP_STATUS_OK;
}

//---------------------------------------------------------------------------------------------------------

/**
zwp_avi_scene_get_state_log_arg_handler - AVRO argument parser
@param[in]	arg_value	    Parameters encoded in AVRO
@param[out]	args_p	        Decoded AVRO parameters
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_scene_get_state_log_arg_handler(avro_value_t *arg_value, void **args_p)
{
    int32_t *type;

    if ((type = (int32_t *)ZWP_MALLOC(sizeof(int32_t))) == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
        return ZWP_STATUS_ERROR;
    }

    if (zw_avro_field_get_int(arg_value, "type", type) != ZWP_STATUS_OK)
    {
        ZWP_FREE(type);
        return ZWP_STATUS_ERROR;
    }

    *args_p = type;

    return ZWP_STATUS_OK;
}


/**
zwp_avi_scene_get_state_log_execute - Invoke library function to execute request
@param[in]	net	            Network
@param[in]	arg	            Parameters
@param[out]	result_p	    Result of the execution
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_scene_get_state_log_execute(zwp_net_t net, void *arg, void **result_p)
{
    int                 i;
    int                 j;
	int                 k;
    uint32_t            category;
    int32_t             *type = (int32_t *)arg;
    zwscn_get_sta_log_t *sta_log;
    uint64_t            value;
	uint32_t			counter;
	uint32_t			scened;

    //Initialize to NULL in case it is freed later on
    *result_p = NULL;

    ZWP_LOG(ZWP_LOG_DEBUG, "Execute - scene_get_state_log");

    if (arg == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, "arg is NULL");
        return ZWP_STATUS_ERROR;
    }

    if ((sta_log = (zwscn_get_sta_log_t *)ZWP_CALLOC(1, sizeof(zwscn_get_sta_log_t))) == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
        return ZWP_STATUS_ERROR;
    }

    j = 0;
	k = 0;

    for (i=0; i<MAX_STATE_LOG; i++)
    {
        category = (0x01 << i);

        if (*type & category)
        {
			if(category < ZWSCN_STATELOG_CATEGORY_SECURITY_SCENE_MIN) {
				sta_log->sta_log[j].type = category;

				if (zwscn_get_statelog(net, category, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP, &value) == ZWSCN_ERR_NONE)
				{
					sta_log->sta_log[j].curr_op = value;

					if (value > 0)
					{
						if (zwscn_get_statelog(net, category, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_SCENED, &value) == ZWSCN_ERR_NONE)
						{
							sta_log->sta_log[j].curr_op_scened = value;
						}

						if (zwscn_get_statelog(net, category, ZWSCN_STATELOG_SUBCATEGORY_CURR_OP_TRIGGER, &value) == ZWSCN_ERR_NONE)
						{
							sta_log->sta_log[j].curr_op_trigger = value;
						}
					}
				}

				if (zwscn_get_statelog(net, category, ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_SCENED, &value) == ZWSCN_ERR_NONE)
				{
					sta_log->sta_log[j].last_op_scened = value;
				}

				if (zwscn_get_statelog(net, category, ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_TIME, &value) == ZWSCN_ERR_NONE)
				{
					sta_log->sta_log[j].last_op_time = value;
				}

				if (zwscn_get_statelog(net, category, ZWSCN_STATELOG_SUBCATEGORY_LAST_OP_TRIGGER, &value) == ZWSCN_ERR_NONE)
				{
					sta_log->sta_log[j].last_op_trigger = value;
				}

				j++;
			} else {
				sta_log->security_scene_sta_log[k].type = category;
				if (zwscn_security_scene_get_statelog(net, category, &counter, &scened) == ZWSCN_ERR_NONE) {
					sta_log->security_scene_sta_log[k].counter = counter;
					sta_log->security_scene_sta_log[k].scened = scened;
				}
				k++;
			}
        }
    }

    sta_log->sta_log_count = j;
	sta_log->security_scene_sta_log_count = k;
    *result_p = sta_log;

    return ZWP_STATUS_OK;
}


/**
zwp_avi_scene_get_state_log_result - Encode the scene get state log response to AVRO format
@param[in]	result	        Result
@param[out]	arg_value	    Output in AVRO object
@return ZWP_STATUS_XXX
*/
static zwp_status_t zwp_avi_scene_get_state_log_result(void *result, avro_value_t *arg_value)
{
    zwscn_get_sta_log_t  *sta_log = (zwscn_get_sta_log_t *)result;
    int                  i;
    avro_value_t         sta_log_list_value;
    avro_value_t         sta_log_value = {0};
	avro_value_t         security_scene_sta_log_list_value;
    avro_value_t         security_scene_sta_log_value = {0};

    if (!result)
    {
        ZWP_LOG(ZWP_LOG_ERR, "result parameter is NULL");
        return ZWP_STATUS_ERROR;
    }

    //Get state logs
    if (avro_value_get_by_name(arg_value, "scene_state_log_list", &sta_log_list_value, NULL) != 0)
    {
        ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'scene_state_log_list': %s", avro_strerror());
        return ZWP_STATUS_ERROR;
    }

    for (i=0; i<sta_log->sta_log_count; i++)
    {
        if (avro_value_append(&sta_log_list_value, &sta_log_value, 0) != 0)
        {
            return ZWP_STATUS_ERROR;
        }

        if ((zw_avro_field_set_int(&sta_log_value, "type", (int32_t)sta_log->sta_log[i].type) != ZWP_STATUS_OK)
            || (zw_avro_field_set_int(&sta_log_value, "curr_op", (int32_t)sta_log->sta_log[i].curr_op) != ZWP_STATUS_OK)
            || (zw_avro_field_set_int(&sta_log_value, "cur_op_scened", (int32_t)sta_log->sta_log[i].curr_op_scened) != ZWP_STATUS_OK)
            || (zw_avro_field_set_int(&sta_log_value, "cur_op_trigger", (int32_t)sta_log->sta_log[i].curr_op_trigger) != ZWP_STATUS_OK)
            || (zw_avro_field_set_int(&sta_log_value, "last_op_scened", (int32_t)sta_log->sta_log[i].last_op_scened) != ZWP_STATUS_OK)
            || (zw_avro_field_set_long(&sta_log_value, "last_op_time", (int64_t)sta_log->sta_log[i].last_op_time) != ZWP_STATUS_OK)
            || (zw_avro_field_set_int(&sta_log_value, "last_op_trigger", (int32_t)sta_log->sta_log[i].last_op_trigger) != ZWP_STATUS_OK))
        {
            return ZWP_STATUS_ERROR;
        }
    }
	
	//Get state logs
    if (avro_value_get_by_name(arg_value, "security_scene_state_log_list", &security_scene_sta_log_list_value, NULL) != 0)
    {
        ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'security_scene_state_log_list': %s", avro_strerror());
        return ZWP_STATUS_ERROR;
    }

    for (i=0; i<sta_log->security_scene_sta_log_count; i++)
    {
        if (avro_value_append(&security_scene_sta_log_list_value, &security_scene_sta_log_value, 0) != 0)
        {
            return ZWP_STATUS_ERROR;
        }

        if ((zw_avro_field_set_int(&security_scene_sta_log_value, security_scene_arg_type, (int32_t)sta_log->security_scene_sta_log[i].type) != ZWP_STATUS_OK)
            || (zw_avro_field_set_int(&security_scene_sta_log_value, security_scene_arg_counter, (int32_t)sta_log->security_scene_sta_log[i].counter) != ZWP_STATUS_OK)
            || (zw_avro_field_set_int(&security_scene_sta_log_value, security_scene_arg_scened, (int32_t)sta_log->security_scene_sta_log[i].scened) != ZWP_STATUS_OK))
        {
            return ZWP_STATUS_ERROR;
        }

    }

    return ZWP_STATUS_OK;

}


/** Scene update AVRO message handler registration data structure */
static zwp_avi_message_networks_t scene_get_state_log =
{
    "scene_get_state_log",                           /**< AVRO message name*/
    zwp_avi_scene_get_state_log_arg_handler,         /**< Command specific AVRO argument parser; NULL if no such arguments */
    zwp_avi_scene_generic_arg_free,                  /**< Free argument */
    zwp_avi_scene_get_state_log_execute,             /**< Invoke library function to execute request*/
    ZWP_AVI_MESSAGE_VALID_NETWORK_CHECK_YES,         /**< Flag to control execution based on validity of network */
    zwp_avi_scene_get_state_log_result,              /**< Encode the response to AVRO format*/
    zwp_avi_scene_generic_result_free                /**< Free the result_p from zwp_avi_XXX_execute()*/
};


/**
zwp_avi_scene_get_state_log_init - Scene update initialization
@param[in]	message_list	    Message list
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_avi_scene_get_state_log_init(zwu_array_list_t message_list)
{
    if (zwp_avi_message_list_add_message_networks(message_list, &scene_get_state_log) != ZWP_STATUS_OK)
    {
        return ZWP_STATUS_ERROR;
    }

    return ZWP_STATUS_OK;
}

