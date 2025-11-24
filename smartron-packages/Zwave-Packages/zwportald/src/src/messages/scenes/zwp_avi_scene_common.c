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
 * @file    zwp_avi_scene_common.c
 *
 * @brief   Common file for Scenes AVI messages
 * @details Common file for Scenes AVI messages.
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
#include "zwscn_scene_api.h"
#include "zwp_avi_scene.h"
#include "zwave/ZW_classcmd.h"
#include "zip_api.h"
#include "zwscn_device_args.h"
#include "zwp_avi_scene_common.h"
#include "zwp_descriptor.h"
 
 /** Command class uri */
static const char *cmd_cls_uri[] =
{
    "zwif_basic",
    "zwif_switch",
    "zwif_level",
    "zwif_dlck",
    "zwif_thrmo_setp",
    "zwif_bsensor",
    "zwif_sensor",
	"zwif_alrm",
	"zwif_centr_scene"
};

/**
zwp_ifd_2_scn_act_evt_id - Convert interface descriptor id to scene action/event id
@param[in]	ifd	            Interface descriptor id
@return Scene action/event id
*/
uint32_t zwp_ifd_2_scn_act_evt_id(uint32_t ifd)
{
    uint32_t act_evt_id;
	uint8_t	node_id;
	uint8_t ep_id;
	uint16_t cls_id;
	
	node_id = (ifd & 0x000000FF);
	ep_id = ((ifd & 0x0000FF00) >> 8);
	cls_id = ((ifd & 0xFFFF0000) >> 16);

	ep_id =  ep_id - 1; 
//    act_evt_id = ((ifd & 0xFF) << 24) | ((ifd & 0xFF00) << 8) | (ifd >> 16);
	act_evt_id = ((node_id << 24) | (ep_id << 16) | cls_id);

    return act_evt_id;

}


/**
zwp_scn_act_evt_id_2_ifd - Convert scene action/event id to interface descriptor id
@param[in]	act_evt_id	   Scene action/event id
@return Interface descriptor id
*/
uint32_t zwp_scn_act_evt_id_2_ifd(uint32_t act_evt_id)
{
    uint32_t    ifd;
	uint8_t	node_id;
	uint8_t ep_id;
	uint16_t cls_id;
	
	node_id = ((act_evt_id & 0xFF000000) >> 24);
	ep_id = ((act_evt_id & 0x00FF0000) >> 16);
	cls_id = (act_evt_id & 0x0000FFFF);

	ep_id =  ep_id + 1; 

    //ifd = (act_evt_id >> 24) | ((act_evt_id & 0xFF0000) >> 8) | (act_evt_id << 16);
	ifd = ((cls_id << 16) | (ep_id << 8) | node_id);

    return ifd;

}


/**
zwp_avi_scene_generic_arg_handler - AVRO argument parser
@param[in]	arg_value	    Parameters encoded in AVRO
@param[out]	args_p	        Decoded AVRO parameters
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_avi_scene_generic_arg_handler(avro_value_t *arg_value, void **args_p)
{
    int32_t *scene_id;

    if ((scene_id = (int32_t *)ZWP_MALLOC(sizeof(int32_t))) == NULL)
    {
        ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
        return ZWP_STATUS_ERROR;
    }

    if (zw_avro_field_get_int(arg_value, scene_arg_scened, scene_id) != ZWP_STATUS_OK)
    {
        ZWP_FREE(scene_id);
        return ZWP_STATUS_ERROR;
    }

    *args_p = scene_id;

    return ZWP_STATUS_OK;
}


/**
zwp_avi_scene_generic_arg_free - Free the data structure allocated in zwp_avi_scene_generic_arg_handler
@param[in]	args	            Data structure to be freed
@return
*/
void zwp_avi_scene_generic_arg_free(void *args)
{
    ZWP_FREE(args);
}


/**
zwp_avi_scene_generic_result - Encode the response to AVRO format
@param[in]	result	            Result
@param[out]	arg_value	        Output in AVRO object
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_avi_scene_generic_result(void *result, avro_value_t *arg_value)
{
    uint32_t scene_desc;

    if (!result)
    {
        ZWP_LOG(ZWP_LOG_ERR, "result parameter is NULL\n");
        return ZWP_STATUS_ERROR;
    }

    scene_desc = *((uint32_t *)result);
    if (zw_avro_field_set_int(arg_value, scene_arg_desc, (int32_t)scene_desc) != ZWP_STATUS_OK)
    {
        return ZWP_STATUS_ERROR;
    }

    return ZWP_STATUS_OK;
}


/**
zwp_avi_scene_generic_result_free - Free the result_p from zwp_avi_XXX_execute()
@param[in]	result	            Result pointer
@return
*/
void zwp_avi_scene_generic_result_free(void *result)
{
    ZWP_FREE(result);
}


/**
zwp_avi_uri_get - Get URI as specified by command class
@param[in]	cmd_cls             Command class as defined by Z-wave
@return URI if successful; else return NULL
*/
const char *zwp_avi_uri_get(uint16_t cmd_cls)
{

    switch (cmd_cls)
    {
        case COMMAND_CLASS_BASIC:
            return cmd_cls_uri[0];
            break;

        case COMMAND_CLASS_SWITCH_BINARY:
            return cmd_cls_uri[1];
            break;

        case COMMAND_CLASS_SWITCH_MULTILEVEL:
            return cmd_cls_uri[2];
            break;

        case COMMAND_CLASS_DOOR_LOCK:
            return cmd_cls_uri[3];
            break;

        case COMMAND_CLASS_THERMOSTAT_SETPOINT:
            return cmd_cls_uri[4];
            break;

        case COMMAND_CLASS_SENSOR_BINARY:
            return cmd_cls_uri[5];
            break;

        case COMMAND_CLASS_SENSOR_MULTILEVEL:
            return cmd_cls_uri[6];
            break;
			
		case COMMAND_CLASS_ALARM:
            return cmd_cls_uri[7];
            break;
			
		case COMMAND_CLASS_CENTRAL_SCENE:
            return cmd_cls_uri[8];
            break;

        default:
            ZWP_LOG(ZWP_LOG_ERR, "Unsupported command class:%04Xh", (unsigned)cmd_cls);
            break;
    }

    return NULL;
}

/**
zwp_avi_scene_int_get - get integer value from a byte stream
@param[in]	byte_buf    The buffer that holds the bytes
@param[in]	size		The integer size
@param[out]	int_val	    The result of the conversion from bytes to integer
@return     1 on success; else return 0
*/
int  zwp_avi_scene_int_get(uint8_t *byte_buf,  uint8_t size, int32_t *int_val)
{

    if (size == 1)
    {
        int8_t  val8;

        val8 = byte_buf[0];
        *int_val = val8;
        return 1;
    }
    else if (size == 2)
    {
        int16_t val16;

        val16 = byte_buf[0];
        val16 = (val16 << 8) | byte_buf[1];
        *int_val = val16;
        return 1;
    }
    else if (size == 4)
    {
        int32_t val32;

        val32 = (((int32_t)(byte_buf[0])) << 24)
                | (((int32_t)(byte_buf[1])) << 16)
                | (((int32_t)(byte_buf[2])) << 8)
                | byte_buf[3];

        *int_val = val32;
        return 1;
    }
    return 0;

}


/**
zwp_avi_param_to_avro - Convert the param into AVRO format
@param[in]	param               Parameter data
@param[in]	cmd_cls             Command class as defined by Z-wave
@param[in]	cmd                 Command as defined by Z-Ware Web Developer's Guide
@param[out]	action_value	    Output in AVRO object
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_avi_param_to_avro(void *param, uint16_t cmd_cls, uint8_t cmd, avro_value_t *action_value)
{
    char         value_str[80];
	avro_value_t param_list_value;
	avro_value_t param_value;

    //Get the parameter list AVRO object
    if (avro_value_get_by_name(action_value, "param_list", &param_list_value, NULL) != 0)
    {
        ZWP_LOG(ZWP_LOG_ERR, "Failed to get the field 'param_list': %s", avro_strerror());
        return ZWP_STATUS_ERROR;
    }

    //Append parameters as key-value pairs
    switch (cmd_cls)
    {
        case COMMAND_CLASS_BASIC:
            if (cmd == 4) //CMD_BASIC_SET
            {
                zwscn_args_basic_set_t  *arg = (zwscn_args_basic_set_t *)param;

                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if (sprintf(value_str, "%u", (unsigned)arg->value) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if ((zw_avro_field_set_string(&param_value, "key", "value") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }

            }
			else if(cmd == 5) //CMD_BASIC_EVENT
			{
				zwscn_args_basic_event_t *arg = (zwscn_args_basic_event_t *)param;

                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }

				//value_low
                if (sprintf(value_str, "%u", (unsigned)arg->value_low) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if ((zw_avro_field_set_string(&param_value, "key", "value_low") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }
				
				if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }
				//value_high
                if (sprintf(value_str, "%u", (unsigned)arg->value_high) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if ((zw_avro_field_set_string(&param_value, "key", "value_high") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }
			}
            break;

        case COMMAND_CLASS_SWITCH_BINARY:
            if (cmd == 4) //CMD_BINARY_SWITCH_SET
            {
                zwscn_args_switch_binary_set_t  *arg = (zwscn_args_switch_binary_set_t *)param;

                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if (sprintf(value_str, "%u", (unsigned)arg->value) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if ((zw_avro_field_set_string(&param_value, "key", "value") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }
            }
            break;

        case COMMAND_CLASS_SWITCH_MULTILEVEL:
            if (cmd == 4) //CMD_MULTILEVEL_SWITCH_SET
            {
                zwscn_args_switch_multilevel_set_t  *arg = (zwscn_args_switch_multilevel_set_t *)param;

                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if (sprintf(value_str, "%u", (unsigned)arg->value) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if ((zw_avro_field_set_string(&param_value, "key", "value") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }

            }
            break;

        case COMMAND_CLASS_DOOR_LOCK:
            if ((cmd == 4) || (cmd == 2)) //CMD_DLOCK_OP_SET || CMD_DLOCK_OP_GET
            {
                zwscn_args_door_lock_operation_set_t  *arg = (zwscn_args_door_lock_operation_set_t *)param;

                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if (sprintf(value_str, "%u", (unsigned)arg->mode) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if ((zw_avro_field_set_string(&param_value, "key", "mode") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }
            }
            break;

        case COMMAND_CLASS_THERMOSTAT_SETPOINT:
            if (cmd == 4) //CMD_THRMO_SETPT_SET
            {
                int32_t                               setp_val;
                zwscn_args_thermostat_setpoint_set_t  *arg = (zwscn_args_thermostat_setpoint_set_t *)param;

                //Set type
                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if (sprintf(value_str, "%u", (unsigned)arg->setpoint.type) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if ((zw_avro_field_set_string(&param_value, "key", "type") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }

                //Set value
                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if (!zwp_avi_scene_int_get(arg->setpoint.data, arg->setpoint.size, &setp_val))
                {
                    return ZWP_STATUS_ERROR;
                }

                if (sprintf(value_str, "%d", setp_val) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if ((zw_avro_field_set_string(&param_value, "key", "value") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }

                //Set precision
                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if (sprintf(value_str, "%u", (unsigned)arg->setpoint.precision) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if ((zw_avro_field_set_string(&param_value, "key", "precision") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }

                //Set unit
                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if (sprintf(value_str, "%u", (unsigned)arg->setpoint.unit) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if ((zw_avro_field_set_string(&param_value, "key", "unit") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }
            }
            break;

        case COMMAND_CLASS_SENSOR_BINARY:
            if (cmd == 2) //CMD_BINARY_SENSOR_GET (Event trigger setting)
            {
                zwscn_args_sensor_binary_t  *arg = (zwscn_args_sensor_binary_t *)param;

				//Sensor Value
                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }
                if (sprintf(value_str, "%u", (unsigned)arg->sensor_value) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }
                if ((zw_avro_field_set_string(&param_value, "key", "state") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }
				
				//Sensor type (only available in v2)
				if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }
				if (sprintf(value_str, "%u", (unsigned)arg->type) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }
                if ((zw_avro_field_set_string(&param_value, "key", "type") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }
            }
            break;

        case COMMAND_CLASS_SENSOR_MULTILEVEL:
            if (cmd == 2) //CMD_MULTILEVEL_SENSOR_GET (Event trigger setting)
            {
                int32_t                         snsr_val;
                zwscn_args_sensor_multilevel_t  *arg = (zwscn_args_sensor_multilevel_t *)param;

                //Set type
                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if (sprintf(value_str, "%u", (unsigned)arg->value_high.type) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if ((zw_avro_field_set_string(&param_value, "key", "type") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }

                //Set unit
                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if (sprintf(value_str, "%u", (unsigned)arg->value_high.unit) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if ((zw_avro_field_set_string(&param_value, "key", "unit") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }

                //Set value low
                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if (!zwp_avi_scene_int_get(arg->value_low.data, arg->value_low.size, &snsr_val))
                {
                    return ZWP_STATUS_ERROR;
                }

                if (sprintf(value_str, "%d", snsr_val) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if ((zw_avro_field_set_string(&param_value, "key", "value_low") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }

                //Set precision of value low
                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if (sprintf(value_str, "%u", (unsigned)arg->value_low.precision) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if ((zw_avro_field_set_string(&param_value, "key", "precision_vl") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }

                //Set value high
                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if (!zwp_avi_scene_int_get(arg->value_high.data, arg->value_high.size, &snsr_val))
                {
                    return ZWP_STATUS_ERROR;
                }

                if (sprintf(value_str, "%d", snsr_val) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if ((zw_avro_field_set_string(&param_value, "key", "value_high") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }

                //Set precision of value high
                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if (sprintf(value_str, "%u", (unsigned)arg->value_high.precision) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if ((zw_avro_field_set_string(&param_value, "key", "precision_vh") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }

            }
            break;
			
			case COMMAND_CLASS_ALARM:
            if (cmd == 2) //CMD_ALARM_GET (Event trigger setting)
            {
                zwscn_args_alarm_t  *arg = (zwscn_args_alarm_t *)param;
				int i, chars_written, offset;

                //Set vendor alarm type
                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if (sprintf(value_str, "%u", (unsigned)arg->va_type) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if ((zw_avro_field_set_string(&param_value, "key", "va_type") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }

                //Set vendor alarm level
                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if (sprintf(value_str, "%u", (unsigned)arg->va_level) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if ((zw_avro_field_set_string(&param_value, "key", "va_level") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }

                //Set Z-Wave alarm type
                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if (sprintf(value_str, "%u", (unsigned)arg->za_type) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }

                if ((zw_avro_field_set_string(&param_value, "key", "za_type") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }

                //Set Z-Wave alarm event
                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }

				chars_written = 0;
				offset = 0;
				for(i=0; i < arg->za_event_count; ++i) {
					chars_written = sprintf(value_str + offset, "%u", (unsigned)arg->za_event[i]);
					if (chars_written < 0)
					{
						return ZWP_STATUS_ERROR;
					}
					offset += chars_written;
					if((i + 1) < arg->za_event_count) { //not the last number
						chars_written = sprintf(value_str + offset, ",");
						if (chars_written < 0)
						{
							return ZWP_STATUS_ERROR;
						}
						offset += chars_written;
					}
				}

                if ((zw_avro_field_set_string(&param_value, "key", "za_event") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }

            }
            break;
			case COMMAND_CLASS_CENTRAL_SCENE:
            if (cmd == 0x03) //CENTRAL_SCENE_NOTIFICATION (Event trigger setting)
            {
                zwscn_args_central_scene_t  *arg = (zwscn_args_central_scene_t *)param;

                //Set central scene number (aka central scene key number)
                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }
                if (sprintf(value_str, "%u", (unsigned)arg->central_scene_key_number) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }
                if ((zw_avro_field_set_string(&param_value, "key", "central_scene_key_number") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }

                //Set central scene key attribute
                if (avro_value_append(&param_list_value, &param_value, 0) != 0)
                {
                    return ZWP_STATUS_ERROR;
                }
                if (sprintf(value_str, "%u", (unsigned)arg->central_scene_key_attribute) < 0)
                {
                    return ZWP_STATUS_ERROR;
                }
                if ((zw_avro_field_set_string(&param_value, "key", "central_scene_key_attribute") != ZWP_STATUS_OK)
                    || (zw_avro_field_set_string(&param_value, "value", value_str) != ZWP_STATUS_OK))
                {
                    return ZWP_STATUS_ERROR;
                }
            }
            break;
			
        default:
            ZWP_LOG(ZWP_LOG_ERR, "Unsupported command class:%04Xh", (unsigned)cmd_cls);
            return ZWP_STATUS_ERROR;
            break;
    }

    return ZWP_STATUS_OK;

}

/**
zwp_avi_param_list_get - Convert the param list from AVRO to data structure
@param[in]	param_list_value    AVRO encoded param list
@param[in]	param_cnt           Number of elements in param list
@param[in]	cmd_cls             Command class as defined by Z-wave
@param[in]	cmd                 Command as defined by Z-Ware Web Developer's Guide
@param[out]	param	            Output data structure
@param[out]	param_sz	        Output data structure size
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_avi_param_list_get(avro_value_t *param_list_value, int param_cnt, uint16_t cmd_cls, uint8_t cmd, void **param, uint32_t *param_sz)
{
    int          i;
	int32_t      spt_val = 0;       //setpoint value
	int32_t      snsr_val_low = 0;  //multilevel sensor value low
	int32_t      snsr_val_hi = 0;   //multilevel sensor value high
    char         *param_key;
    char         *param_value;
    void         *param_buf = NULL;
	avro_value_t param_avro;
	int param_found_1=0, param_found_2=0, param_found_3=0, param_found_4=0, param_found_5=0, param_found_6=0;

    for (i=0; i<param_cnt; i++)
    {
        if (avro_value_get_by_index(param_list_value, i, &param_avro, NULL) != 0)
        {
            return ZWP_STATUS_ERROR;
        }

        if (zw_avro_field_get_string(&param_avro, "key", &param_key) != ZWP_STATUS_OK)
        {
            return ZWP_STATUS_ERROR;
        }

        if (zw_avro_field_get_string(&param_avro, "value", &param_value) != ZWP_STATUS_OK)
        {
            zw_avro_field_free_string(param_key);
            return ZWP_STATUS_ERROR;
        }

        switch (cmd_cls)
        {
            case COMMAND_CLASS_BASIC:
                if (cmd == 4) //CMD_BASIC_SET
                {
                    zwscn_args_basic_set_t  *basic_set_arg;
                    if (!param_buf)
                    {   //Allocate memory for the command parameter
                        if ((param_buf = ZWP_CALLOC(1, sizeof(zwscn_args_basic_set_t))) == NULL)
                        {
                            goto l_PARAM_GET_ERR;
                        }
                    }
                    basic_set_arg = (zwscn_args_basic_set_t *)param_buf;
                    if (strcmp(param_key, "value") == 0)
                    {
                        basic_set_arg->value = (uint8_t)atoi(param_value);
                        param_found_1 = 1;
                    }
					
					if ((i + 1) >= param_cnt)
                    {   //This is the last param
					
						if(!param_found_1)
						{
							ZWP_LOG(ZWP_LOG_ERR, "Mandatory device param not found for COMMAND_CLASS_BASIC");
							goto l_PARAM_GET_ERR;
						}
						*param = param_buf;
                        *param_sz = sizeof(zwscn_args_basic_set_t);
                        param_buf = NULL;
					}
                }
				else if(cmd == 5) //CMD_BASIC_EVENT
				{
					zwscn_args_basic_event_t  *basic_event_arg;
					
					if (!param_buf)
                    {   //Allocate memory for the command parameter
                        if ((param_buf = ZWP_CALLOC(1, sizeof(zwscn_args_basic_event_t))) == NULL)
                        {
                            goto l_PARAM_GET_ERR;
                        }
                    }
                    basic_event_arg = (zwscn_args_basic_event_t *)param_buf;
					if (strcmp(param_key, "value_low") == 0)
                    {
                        basic_event_arg->value_low = (uint8_t)atoi(param_value);
						param_found_1 = 1;
                    }
					if (strcmp(param_key, "value_high") == 0)
                    {
                        basic_event_arg->value_high = (uint8_t)atoi(param_value);
						param_found_2 = 1;
                    }
					
					if ((i + 1) >= param_cnt)
                    {   //This is the last param
					
						if((!param_found_1) || (!param_found_2))
						{
							ZWP_LOG(ZWP_LOG_ERR, "Mandatory device param not found for COMMAND_CLASS_BASIC event");
							goto l_PARAM_GET_ERR;
						}
						*param = param_buf;
                        *param_sz = sizeof(zwscn_args_basic_event_t);
                        param_buf = NULL;
					}
				}
                break;

            case COMMAND_CLASS_SWITCH_BINARY:
                if (cmd == 4) //CMD_BINARY_SWITCH_SET
                {
                    zwscn_args_switch_binary_set_t  *bin_sw_set_arg;
                    if (!param_buf)
                    {   //Allocate memory for the command parameter
                        if ((param_buf = ZWP_CALLOC(1, sizeof(zwscn_args_switch_binary_set_t))) == NULL)
                        {
                            goto l_PARAM_GET_ERR;
                        }
                    }
                    bin_sw_set_arg = (zwscn_args_switch_binary_set_t *)param_buf;
                    if (strcmp(param_key, "value") == 0)
                    {
                        bin_sw_set_arg->value = (uint8_t)atoi(param_value);
                        param_found_1 = 1;
                    }
					
					if ((i + 1) >= param_cnt)
                    {   //This is the last param
					
						if(!param_found_1)
						{
							ZWP_LOG(ZWP_LOG_ERR, "Mandatory device param not found for COMMAND_CLASS_SWITCH_BINARY");
							goto l_PARAM_GET_ERR;
						}
						*param = param_buf;
                        *param_sz = sizeof(zwscn_args_switch_binary_set_t);
                        param_buf = NULL;
					}
                }
                break;

            case COMMAND_CLASS_SWITCH_MULTILEVEL:
                if (cmd == 4) //CMD_MULTILEVEL_SWITCH_SET
                {
                    zwscn_args_switch_multilevel_set_t  *mul_lvl_set_arg;
                    if (!param_buf)
                    {   //Allocate memory for the command parameter
                        if ((param_buf = ZWP_CALLOC(1, sizeof(zwscn_args_switch_multilevel_set_t))) == NULL)
                        {
                            goto l_PARAM_GET_ERR;
                        }
                    }
                    mul_lvl_set_arg = (zwscn_args_switch_multilevel_set_t *)param_buf;
                    if (strcmp(param_key, "value") == 0)
                    {
                        mul_lvl_set_arg->value = (uint8_t)atoi(param_value);
						param_found_1 = 1;
                    }
					
					if ((i + 1) >= param_cnt)
                    {   //This is the last param
					
						if(!param_found_1)
						{
							ZWP_LOG(ZWP_LOG_ERR, "Mandatory device param not found for COMMAND_CLASS_SWITCH_MULTILEVEL");
							goto l_PARAM_GET_ERR;
						}
						*param = param_buf;
                        *param_sz = sizeof(zwscn_args_switch_multilevel_set_t);
                        param_buf = NULL;
					}
                }
                break;

            case COMMAND_CLASS_DOOR_LOCK:
                if ((cmd == 4) || (cmd == 2)) //CMD_DLOCK_OP_SET || CMD_DLOCK_OP_GET
                {
                    zwscn_args_door_lock_operation_set_t  *dlck_op_set_arg;
					
                    if (!param_buf)
                    {   //Allocate memory for the command parameter
                        if ((param_buf = ZWP_CALLOC(1, sizeof(zwscn_args_door_lock_operation_set_t))) == NULL)
                        {
                            goto l_PARAM_GET_ERR;
                        }
                    }
                    dlck_op_set_arg = (zwscn_args_door_lock_operation_set_t *)param_buf;
                    if (strcmp(param_key, "mode") == 0)
                    {
                        dlck_op_set_arg->mode = (uint8_t)atoi(param_value);
                        param_found_1 = 1;
                    }
					
					if ((i + 1) >= param_cnt)
                    {   //This is the last param
					
						if(!param_found_1)
						{
							ZWP_LOG(ZWP_LOG_ERR, "Mandatory device param not found for COMMAND_CLASS_DOOR_LOCK");
							goto l_PARAM_GET_ERR;
						}
						*param = param_buf;
                        *param_sz = sizeof(zwscn_args_door_lock_operation_set_t);
                        param_buf = NULL;
					}
                }
                break;

            case COMMAND_CLASS_THERMOSTAT_SETPOINT:
                if (cmd == 4) //CMD_THRMO_SETPT_SET
                {
                    zwscn_args_thermostat_setpoint_set_t  *thr_stp_set_arg;
					
                    if (!param_buf)
                    {   //Allocate memory for the command parameter
                        if ((param_buf = ZWP_CALLOC(1, sizeof(zwscn_args_thermostat_setpoint_set_t))) == NULL)
                        {
                            goto l_PARAM_GET_ERR;
                        }
                    }
                    thr_stp_set_arg = (zwscn_args_thermostat_setpoint_set_t *)param_buf;
                    if (strcmp(param_key, "type") == 0)
                    {
                        thr_stp_set_arg->setpoint.type = (uint8_t)atoi(param_value);
						param_found_1 = 1;
                    }
                    else if (strcmp(param_key, "value") == 0)
                    {
                        spt_val = atoi(param_value);
						param_found_2 = 1;
                    }
                    else if (strcmp(param_key, "precision") == 0)
                    {
                        thr_stp_set_arg->setpoint.precision = (uint8_t)atoi(param_value);
						param_found_3 = 1;
                    }
                    else if (strcmp(param_key, "unit") == 0)
                    {
                        thr_stp_set_arg->setpoint.unit = (uint8_t)atoi(param_value);
						param_found_4 = 1;
                    }

                    if ((i + 1) >= param_cnt)
                    {   //This is the last param
                        int size;
                        int j;

						if((!param_found_1) || (!param_found_2) || (!param_found_3) || (!param_found_4))
						{
							ZWP_LOG(ZWP_LOG_ERR, "Mandatory device param not found");
							goto l_PARAM_GET_ERR;
						}
						// Find the minimum size needed for the value
						thr_stp_set_arg->setpoint.size = ((spt_val >= -128) && (spt_val <= 127))
								? 1 : (((spt_val >= -32768) && (spt_val <= 32767))? 2 : 4);

                        // Fill in the data value
                        size = thr_stp_set_arg->setpoint.size;
                        for (j=(size - 1); j>=0; j--)
                        {
                            thr_stp_set_arg->setpoint.data[j] = (uint8_t)((spt_val >> (8 * (size - 1 - j))) & 0xFF);
                        }

                        *param = param_buf;
                        *param_sz = sizeof(zwscn_args_thermostat_setpoint_set_t);
                        param_buf = NULL;
                    }
                }
                break;

            case COMMAND_CLASS_SENSOR_BINARY:
                if (cmd == 2) //CMD_BINARY_SENSOR_GET
                {
                    zwscn_args_sensor_binary_t  *bin_snsr_arg;
					
                    if (!param_buf)
                    {   //Allocate memory for the command parameter
                        if ((param_buf = ZWP_CALLOC(1, sizeof(zwscn_args_sensor_binary_t))) == NULL)
                        {
                            goto l_PARAM_GET_ERR;
                        }
                    }
                    bin_snsr_arg = (zwscn_args_sensor_binary_t *)param_buf;
                    if (strcmp(param_key, "state") == 0)
                    {
                        bin_snsr_arg->sensor_value = (uint8_t)atoi(param_value);
						param_found_1 = 1;
                    } 
					else if (strcmp(param_key, "type") == 0)
                    {
                        bin_snsr_arg->type = (uint8_t)atoi(param_value);
						param_found_2 = 1;
                    }
					
					if ((i + 1) >= param_cnt)
                    {   //This is the last param
					
						if((!param_found_1))
						{
							ZWP_LOG(ZWP_LOG_ERR, "Mandatory device param not found");
							goto l_PARAM_GET_ERR;
						}
						if(!param_found_2) {
							bin_snsr_arg->type = ZWP_ZWAVE_SCENE_EVENT_ARG_NOT_FOUND;
						}

                        *param = param_buf;
                        *param_sz = sizeof(zwscn_args_sensor_binary_t);
                        param_buf = NULL;
                    }
                }
                break;

            case COMMAND_CLASS_SENSOR_MULTILEVEL:
                if (cmd == 2) //CMD_MULTILEVEL_SENSOR_GET
                {
                    zwscn_args_sensor_multilevel_t  *mul_snsr_arg;
					
                    if (!param_buf)
                    {   //Allocate memory for the command parameter
                        if ((param_buf = ZWP_CALLOC(1, sizeof(zwscn_args_sensor_multilevel_t))) == NULL)
                        {
                            goto l_PARAM_GET_ERR;
                        }
                    }
                    mul_snsr_arg = (zwscn_args_sensor_multilevel_t *)param_buf;
                    if (strcmp(param_key, "type") == 0)
                    {
                        mul_snsr_arg->value_low.type = mul_snsr_arg->value_high.type = (uint8_t)atoi(param_value);
						param_found_1 = 1;
                    }
                    else if (strcmp(param_key, "unit") == 0)
                    {
                        mul_snsr_arg->value_low.unit = mul_snsr_arg->value_high.unit = (uint8_t)atoi(param_value);
						param_found_2 = 1;
                    }
                    else if (strcmp(param_key, "value_low") == 0)
                    {
                        snsr_val_low = atoi(param_value);
						param_found_3 = 1;
                    }
                    else if (strcmp(param_key, "value_high") == 0)
                    {
                        snsr_val_hi = atoi(param_value);
						param_found_4 = 1;
                    }
                    else if (strcmp(param_key, "precision_vl") == 0)
                    {
                        mul_snsr_arg->value_low.precision = (uint8_t)atoi(param_value);
						param_found_5 = 1;
                    }
                    else if (strcmp(param_key, "precision_vh") == 0)
                    {
                        mul_snsr_arg->value_high.precision = (uint8_t)atoi(param_value);
						param_found_6 = 1;
                    }

                    if ((i + 1) >= param_cnt)
                    {   //This is the last param
                        int size;
                        int j;

						if((!param_found_1) || (!param_found_2) || (!param_found_3) || (!param_found_4) || (!param_found_5) || (!param_found_6))
						{
							ZWP_LOG(ZWP_LOG_ERR, "Mandatory device param not found");
							goto l_PARAM_GET_ERR;
						}
                        // Find the minimum size needed for the value low
                        mul_snsr_arg->value_low.size = ((snsr_val_low >= -128) && (snsr_val_low <= 127))
                                ? 1 : (((snsr_val_low >= -32768) && (snsr_val_low <= 32767))? 2 : 4);

                        // Fill in the data value low
                        size = mul_snsr_arg->value_low.size;
                        for (j=(size - 1); j>=0; j--)
                        {
                            mul_snsr_arg->value_low.data[j] = (uint8_t)((snsr_val_low >> (8 * (size - 1 - j))) & 0xFF);
                        }

                        // Find the minimum size needed for the value high
                        mul_snsr_arg->value_high.size = ((snsr_val_hi >= -128) && (snsr_val_hi <= 127))
                                ? 1 : (((snsr_val_hi >= -32768) && (snsr_val_hi <= 32767))? 2 : 4);

                        // Fill in the data value high
                        size = mul_snsr_arg->value_high.size;
                        for (j=(size - 1); j>=0; j--)
                        {
                            mul_snsr_arg->value_high.data[j] = (uint8_t)((snsr_val_hi >> (8 * (size - 1 - j))) & 0xFF);
                        }

                        *param = param_buf;
                        *param_sz = sizeof(zwscn_args_sensor_multilevel_t);
                        param_buf = NULL;
                    }
                }
                break;
				
				case COMMAND_CLASS_ALARM:
                if (cmd == 2) //CMD_ALARM_GET
                {
                    zwscn_args_alarm_t  *alarm_arg;
					
                    if (!param_buf)
                    {   //Allocate memory for the command parameter
                        if ((param_buf = ZWP_CALLOC(1, sizeof(zwscn_args_alarm_t))) == NULL)
                        {
                            goto l_PARAM_GET_ERR;
                        }
                    }
                    alarm_arg = (zwscn_args_alarm_t *)param_buf;
//					alarm_arg->za_type = ZWP_ZWAVE_SCENE_EVENT_ARG_NOT_FOUND;
//					alarm_arg->za_event_count = ZWP_ZWAVE_SCENE_EVENT_ARG_NOT_FOUND;
                    if (strcmp(param_key, "va_type") == 0)
                    {
                        alarm_arg->va_type = (uint8_t)atoi(param_value);
						param_found_1 = 1;
                    }
                    else if (strcmp(param_key, "va_level") == 0)
                    {
                        alarm_arg->va_level = (uint8_t)atoi(param_value);
						param_found_2 = 1;
                    }
                    else if (strcmp(param_key, "za_type") == 0)
                    {
                        alarm_arg->za_type = (uint8_t)atoi(param_value);
						param_found_3 = 1;
                    }
                    else if (strcmp(param_key, "za_event") == 0)
                    {
						char temp[16] = {'\0'};
						int temp_length = 0;
						int num_chars = 0;
						size_t field_len = 0;
						char *delims = ",";
						uint32_t za_event_count = 0;
//						alarm_arg->za_event = (uint8_t)atoi(param_value);
						
						const char *string = param_value;
						num_chars = strlen(param_value);
						temp_length = sizeof(temp);
						do {
							field_len = strcspn(string, delims);
//							ZWP_LOG(ZWP_LOG_ERR, "\"%.*s\"\n", (int)field_len, s);
							if(num_chars < field_len) {
								goto l_PARAM_GET_ERR;
							}
							if(field_len < temp_length) {
								memcpy(temp, string, field_len);
								temp[field_len] = '\0';
								ZWP_LOG(ZWP_LOG_INFO, "STRCSPN %s\n", temp);
								alarm_arg->za_event[za_event_count] = (uint8_t)atoi(temp);
								++(za_event_count);
							}
							
							num_chars -= (field_len + 1); // +1 is for the delims that is skipped in while post increment
							string += field_len;
						} while ((*string++) && (alarm_arg->za_event_count < ZWSCN_ARGS_ALARM_MAX_EVENTS));
                        
						if(za_event_count > 0) {
							alarm_arg->za_event_count = za_event_count;
							param_found_4 = 1;
						}
                    }
                    
                    if ((i + 1) >= param_cnt)
                    {   //This is the last param
					
						//if((!param_found_1) || (!param_found_2) || (!param_found_3) || (!param_found_4))
						if((!param_found_1) || (!param_found_2)) //param 3 and 4 are checked based on version 
						{
							ZWP_LOG(ZWP_LOG_ERR, "Mandatory device param not found");
							goto l_PARAM_GET_ERR;
						}
						if(!param_found_3) {
							alarm_arg->za_type = ZWP_ZWAVE_SCENE_EVENT_ARG_NOT_FOUND;
						}
						if(!param_found_4) {
							alarm_arg->za_event_count = ZWSCN_ARGS_ALARM_EVENTS_NOT_FOUND;
						}

                        *param = param_buf;
                        *param_sz = sizeof(zwscn_args_alarm_t);
                        param_buf = NULL;
                    }
                }
                break;
				case COMMAND_CLASS_CENTRAL_SCENE:
                if (cmd == 0x03) //CENTRAL_SCENE_NOTIFICATION (Event trigger setting)
                {
                    zwscn_args_central_scene_t  *central_scene_arg;
					
                    if (!param_buf)
                    {   //Allocate memory for the command parameter
                        if ((param_buf = ZWP_CALLOC(1, sizeof(zwscn_args_central_scene_t))) == NULL)
                        {
                            goto l_PARAM_GET_ERR;
                        }
                    }
                    central_scene_arg = (zwscn_args_central_scene_t *)param_buf;
                    if (strcmp(param_key, "central_scene_key_number") == 0)
                    {
                        central_scene_arg->central_scene_key_number = (uint8_t)atoi(param_value);
						param_found_1 = 1;
                    }
                    else if (strcmp(param_key, "central_scene_key_attribute") == 0)
                    {
                        central_scene_arg->central_scene_key_attribute = (uint8_t)atoi(param_value);
						param_found_2 = 1;
                    }
                    
                    if ((i + 1) >= param_cnt)
                    {   //This is the last param

						if((!param_found_1) || (!param_found_2))
						{
							ZWP_LOG(ZWP_LOG_ERR, "Mandatory device param not found");
							goto l_PARAM_GET_ERR;
						}
                        *param = param_buf;
                        *param_sz = sizeof(zwscn_args_central_scene_t);
                        param_buf = NULL;
                    }
                }
                break;				

            default:
                ZWP_LOG(ZWP_LOG_ERR, "Unsupported command class:%04Xh", (unsigned)cmd_cls);
                goto l_PARAM_GET_ERR;
                break;
        }

        zw_avro_field_free_string(param_key);
        zw_avro_field_free_string(param_value);

        //Reset param value
        avro_value_reset(&param_avro);
    }

    if (param_buf)
        ZWP_FREE(param_buf);

    return ZWP_STATUS_OK;

l_PARAM_GET_ERR:
    if (param_buf)
        ZWP_FREE(param_buf);
    zw_avro_field_free_string(param_key);
    zw_avro_field_free_string(param_value);
    return ZWP_STATUS_ERROR;

}


/***********The following macros are copied from ZWeb's zws_cgi_common_networks.h ***************/
/** Node ID bitmask */
#define ZWS_CGI_NODE_ID_MASK             0x000000FF
/** Node ID shift */
#define ZWS_CGI_NODE_ID_SHIFT            0
/** Endpoint ID bitmask */
#define ZWS_CGI_ENDPOINT_ID_MASK         0x0000FF00
/** Endpoint ID shift */
#define ZWS_CGI_ENDPOINT_ID_SHIFT        8
/** Interface ID bitmask */
#define ZWS_CGI_INTERFACE_ID_MASK        0xFFFF0000
/** Interface ID shift */
#define ZWS_CGI_INTERFACE_ID_SHIFT       16
/** Get node ID from combo ID */
#define ZWS_CGI_GET_NODE_ID(desc)            (((desc) & ZWS_CGI_NODE_ID_MASK) >> ZWS_CGI_NODE_ID_SHIFT)
/** Get endpoint ID from combo ID */
#define ZWS_CGI_GET_ENDPOINT_ID(desc)        ((((desc) & ZWS_CGI_ENDPOINT_ID_MASK) >> ZWS_CGI_ENDPOINT_ID_SHIFT) - 1)
/** Get interface ID from combo ID */
#define ZWS_CGI_GET_INTERFACE_ID(desc)       (((desc) & ZWS_CGI_INTERFACE_ID_MASK) >> ZWS_CGI_INTERFACE_ID_SHIFT)
int zwp_avi_scene_event_validate_device_args(zwp_net_t net, zwscn_event_t event) {
	int result = 1; //The default is to return TRUE (1)
	uint32_t ifd_id = 0;
	uint8_t node_id = 0;
	uint8_t ep_id = 0;
	uint16_t cls_id = 0;
	zwifd_p desc_interface;
	uint8_t	version;
	
	if(net == NULL) {
		goto l_err_args;
	}
	if((event.device_args_size == 0) || (event.device_args == NULL)) {
		goto l_err_device_args;
	}
	ifd_id = zwp_scn_act_evt_id_2_ifd(event.id);
	node_id = ZWS_CGI_GET_NODE_ID(ifd_id);
	ep_id = ZWS_CGI_GET_ENDPOINT_ID(ifd_id);
	cls_id = ZWS_CGI_GET_INTERFACE_ID(ifd_id);
	if (zwp_desc_get_desc_interface(zwp_net_get_desc_net(net), node_id, ep_id, cls_id, &desc_interface) != ZWP_STATUS_OK) {
		goto l_err_get_desc_interface;
	}
	version = desc_interface->ver;
	
	switch(cls_id) {
		case COMMAND_CLASS_SENSOR_BINARY:
		{
			zwscn_args_sensor_binary_t *device_args = event.device_args;
			
			if((version >= 2) && (device_args->type == ZWP_ZWAVE_SCENE_EVENT_ARG_NOT_FOUND)) {
				result = 0; //FALSE
			}
		}
		break;
		case COMMAND_CLASS_ALARM:
		{
			zwscn_args_alarm_t *device_args = event.device_args;
			
			if((version >= 2) && ((device_args->za_type == ZWP_ZWAVE_SCENE_EVENT_ARG_NOT_FOUND) || (device_args->za_event_count == ZWSCN_ARGS_ALARM_EVENTS_NOT_FOUND))) {
				result = 0; //FALSE
			}
		}
		break;
	}
	
	
	return result;
l_err_args:
l_err_device_args:
l_err_get_desc_interface:
	return 1; //The default is to return TRUE (1), even on errors
}

 