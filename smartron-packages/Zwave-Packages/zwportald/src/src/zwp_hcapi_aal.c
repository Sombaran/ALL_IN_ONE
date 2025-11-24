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
 * @file    zwp_hcapi_aal.c
 *
 * @brief   HCAPI Application Abstraction Layer
 * @details Provide HCAPI abstration for notification and report callbacks.
 *
 * @author  David Chow
 *
 * @version 1.0 - 2013-11-21
 * - Initial version
 */

#include <string.h>
#include <sys/stat.h>

#include "zwp_log.h"
#if defined(ZWP_DEBUG_NETWORK)
#include "zwp_log_enable.h"
#else
#include "zwp_log_disable.h"
#endif

#include "zwp_network.h"
#include "zwp_descriptor.h"
#include "zwp_error.h"
#include "zwp_memory.h"
#include "zwp_queue.h"
#include "zwp_thread_mutex.h"
#include "zwp_thread_cond.h"
#include "zwp_avi_service.h"
#include "zwp_avi_service_networks.h"
#include "zwp_avi_service_nodes.h"
#include "zwp_avi_service_endpoints.h"
#include "zwp_avi_service_interfaces.h"
#include "zwp_avi_message_networks.h"
#include "zwp_avi_scene.h"
#include "zwp_hcapi_aal.h"
#include "zwave/ZW_classcmd.h"
#include "zip_api.h"
#include "zwscn_device_args.h"
#include "zwscn_device_reports.h"

//#define  SHOW_HCAPI_MSG
static void hcaal_basic_set_rpt_cb(zwp_net_t net, uint8_t nodeid, uint8_t epid, uint16_t cls, uint8_t level);

/**
 * @brief		Function to perform the Setup (registering and enabling report callbacks)
 * @param[in] 	net		Network runtime context
 * @param[in] 	node_id	Node id
 * @param[in] 	ep_id	EP id
 * @param[in] 	cls_id	Command Class
 * @param[in] 	cmd		Command (SETUP)
 * @return		0 if success; else negative value on error
 */
int hcaal_rpt_setup(void *net, uint8_t node_id, uint8_t ep_id, uint16_t cls_id, uint8_t cmd)
{
    return 0;
}

/**
hcaal_poll_cb - callback on Post-Set polling completion
@param[in]	net	        Z-wave network handle
@param[in]	node_id	    Node id
@param[in]	ep_id		Endpoint id
@param[in]	cls_id		Command Class id
@param[in]	usr_param	user parameter
@param[in]	reason		Reason for the callback
*/
static void hcaal_postset_poll_cb(zwnet_p net, uint8_t node_id, uint8_t ep_id, uint16_t cls_id, void *usr_param, int reason)
{
    zwscn_fn_postset_poll_cb_t postset_poll_cb = (zwscn_fn_postset_poll_cb_t)usr_param;

    ZWP_LOG(ZWP_LOG_DEBUG, "Post-Set Poll cb, node:%u, ep:%u cls:%u reason:%u user_param:%p", node_id, ep_id, cls_id, reason, usr_param);

    if (postset_poll_cb)
    {
        postset_poll_cb(zwnet_get_user(net), node_id, ep_id, cls_id, reason);
    }
}

/**
 * @brief		Function to perform the Set operation on a device
 * @param[in] 	net		Network runtime context
 * @param[in] 	node_id	Node id
 * @param[in] 	ep_id	EP id
 * @param[in] 	cls_id	Command Class
 * @param[in] 	cmd		Command (SET)
 * @param[in] 	args	device specific arguments - see zwscn_device_args.h
 * @param[in]	postset_poll_cb Post-Set poll callback
 * @return		0 if success; else negative value on error
 */
int hcaal_set(void *net, uint8_t node_id, uint8_t ep_id, uint16_t cls_id, uint8_t cmd, void *args, zwscn_fn_postset_poll_cb_t postset_poll_cb)
{
	zwifd_t         if_desc;
    int             result;
	zwnet_p         zwnet;

	zwnet = zwp_net_get_zwnet(net);

    if (zwnet_get_if_by_id(zwnet, node_id, ep_id, cls_id, &if_desc) != 0)
    {
        return ZW_ERR_INTF_NOT_FOUND;
    }

    //ZWP_LOG(ZWP_LOG_DEBUG, "command class:%04Xh, cmd:%u", (unsigned)cls_id, (unsigned)cmd);

    //Initialize return code
    result = ZW_ERR_UNSUPPORTED;

    switch (cls_id)
    {
        case COMMAND_CLASS_BASIC:
            if (cmd == CMD_BASIC_SET)
            {
                zwscn_args_basic_set_t  *basic_set_arg = (zwscn_args_basic_set_t *)args;

                result = zwif_basic_set(&if_desc, basic_set_arg->value);
            }
            break;

        case COMMAND_CLASS_SWITCH_BINARY:
            if (cmd == CMD_BINARY_SWITCH_SET)
            {
                zwscn_args_switch_binary_set_t  *bin_sw_set_arg = (zwscn_args_switch_binary_set_t *)args;

                result = zwif_switch_set(&if_desc, bin_sw_set_arg->value);
            }
            break;

        case COMMAND_CLASS_SWITCH_MULTILEVEL:
            if (cmd == CMD_MULTILEVEL_SWITCH_SET)
            {
                zwscn_args_switch_multilevel_set_t  *mul_lvl_set_arg = (zwscn_args_switch_multilevel_set_t *)args;

				if(postset_poll_cb != NULL) {
					result = zwif_level_set(&if_desc, mul_lvl_set_arg->value, 0, hcaal_postset_poll_cb, postset_poll_cb);
				} else {
					result = zwif_level_set(&if_desc, mul_lvl_set_arg->value, 0, NULL, NULL);
				}
				
            }
            break;

        case COMMAND_CLASS_DOOR_LOCK:
            if (cmd == CMD_DLOCK_OP_SET)
            {
                zwscn_args_door_lock_operation_set_t  *dlck_op_set_arg = (zwscn_args_door_lock_operation_set_t *)args;

				if(postset_poll_cb != NULL) {
					result = zwif_dlck_op_set(&if_desc, dlck_op_set_arg->mode, hcaal_postset_poll_cb, postset_poll_cb);
				} else {
					result = zwif_dlck_op_set(&if_desc, dlck_op_set_arg->mode, NULL, NULL);
				}
				
            }
            break;

        case COMMAND_CLASS_THERMOSTAT_SETPOINT:
            if (cmd == CMD_THRMO_SETPT_SET)
            {
                zwscn_args_thermostat_setpoint_set_t  *thr_stp_set_arg = (zwscn_args_thermostat_setpoint_set_t *)args;

                result = zwif_thrmo_setp_set(&if_desc, &thr_stp_set_arg->setpoint);
            }
            break;

        default:
            ZWP_LOG(ZWP_LOG_ERR, "Unsupported command class:%04Xh", (unsigned)cls_id);
            result = ZW_ERR_CLASS_NOT_FOUND;
            break;
    }

	ZWP_FREE(args);

    return result;
}


/**
hcaal_poll_cb - callback on polling completion
@param[in]	net	        Z-wave network handle
@param[in]	handle	    handle of the polling request
@param[in]	usr_token	user defined token to facilitate deletion of multiple polling requests
@param[in]	usr_param	user parameter
*/
static void hcaal_poll_cb(zwnet_p net, uint16_t handle, uint32_t usr_token, void *usr_param)
{
    zwscn_device_zwpoll_cmplt_fn    poll_cb = (zwscn_device_zwpoll_cmplt_fn)usr_param;

    //ZWP_LOG(ZWP_LOG_DEBUG, "Poll cb, handle:%u, user param:%p", handle, poll_cb);

    if (poll_cb)
    {
        poll_cb(zwnet_get_user(net), handle, usr_token);
    }
}

/**
 * @brief		Function to perform the Get operation on a device
 * @param[in] 	net			Network runtime context
 * @param[in] 	node_id		Node id
 * @param[in] 	ep_id		EP id
 * @param[in] 	cls_id		Command Class
 * @param[in] 	set_cmd	    Original set command
 * @param[in] 	poll_req	Poll request structure if a zwpoll is to be started. Otherwise it is NULL.
 * @return
 */
int hcaal_get(void *net, uint8_t node_id, uint8_t ep_id, uint16_t cls_id, uint8_t set_cmd, void *args, zwscn_device_zwpoll_req_t *poll_req)
{
	zwifd_t         if_desc;
    zwpoll_req_t    zw_poll_req = {0};
    int             result;
	zwnet_p         zwnet;

	zwnet = zwp_net_get_zwnet(net);

    if (zwnet_get_if_by_id(zwnet, node_id, ep_id, cls_id, &if_desc) != 0)
    {
        return ZW_ERR_INTF_NOT_FOUND;
    }

    if (poll_req)
    {
        zw_poll_req.interval = poll_req->interval;
        zw_poll_req.poll_cnt = poll_req->poll_cnt;
        zw_poll_req.usr_token = poll_req->usr_token;
        if (poll_req->cmplt_cb)
        {   //Require callback
            zw_poll_req.cmplt_cb = hcaal_poll_cb;
            zw_poll_req.usr_param = (void *)poll_req->cmplt_cb;
        }
    }


    //ZWP_LOG(ZWP_LOG_DEBUG, "command class:%04Xh, cmd:%u", (unsigned)cls_id, (unsigned)cmd);
    //cmd = 2; //testing


    //Initialize return code
    result = ZW_ERR_UNSUPPORTED;

    switch (cls_id)
    {
        case COMMAND_CLASS_BASIC:
            if (set_cmd == CMD_BASIC_SET)
            {
                result = (poll_req)? zwif_basic_get_poll(&if_desc, &zw_poll_req) : zwif_basic_get(&if_desc, ZWIF_GET_BMSK_LIVE | ZWIF_GET_BMSK_CACHE);
            }
            break;

        case COMMAND_CLASS_SWITCH_BINARY:
            if (set_cmd == CMD_BINARY_SWITCH_SET)
            {
                result = (poll_req)? zwif_switch_get_poll(&if_desc, &zw_poll_req) : zwif_switch_get(&if_desc, ZWIF_GET_BMSK_LIVE | ZWIF_GET_BMSK_CACHE);
            }
            break;

        case COMMAND_CLASS_SWITCH_MULTILEVEL:
            if (set_cmd == CMD_MULTILEVEL_SWITCH_SET)
            {
                result = (poll_req)? zwif_level_get_poll(&if_desc, &zw_poll_req) : zwif_level_get(&if_desc, ZWIF_GET_BMSK_LIVE | ZWIF_GET_BMSK_CACHE);
            }
            break;

        case COMMAND_CLASS_DOOR_LOCK:
            if (set_cmd == CMD_DLOCK_OP_SET)
            {
                result = (poll_req)? zwif_dlck_op_get_poll(&if_desc, &zw_poll_req) : zwif_dlck_op_get(&if_desc, ZWIF_GET_BMSK_LIVE | ZWIF_GET_BMSK_CACHE);
            }
            break;

        case COMMAND_CLASS_THERMOSTAT_SETPOINT:
            if (set_cmd == CMD_THRMO_SETPT_SET)
            {
                zwscn_args_thermostat_setpoint_get_t *setpoint_get_data = (zwscn_args_thermostat_setpoint_get_t *)args;
				if(setpoint_get_data) {
					result = (poll_req)? zwif_thrmo_setp_get_poll(&if_desc, setpoint_get_data->type, &zw_poll_req) : zwif_thrmo_setp_get(&if_desc, setpoint_get_data->type, ZWIF_GET_BMSK_LIVE | ZWIF_GET_BMSK_CACHE);
				} else {
					result = ZW_ERR_VALUE;
				}
            }
            break;

        default:
            ZWP_LOG(ZWP_LOG_ERR, "Unsupported command class:%04Xh", (unsigned)cls_id);
            result = ZW_ERR_CLASS_NOT_FOUND;
            break;
    }

    //Save the poll handle
    if (poll_req)
    {
        poll_req->handle = zw_poll_req.handle;
    }

	ZWP_FREE(args);

    return result;
}

/**
 * @brief		Function to get Name and Location of a device
 * @param[in] 	net			Network runtime context
 * @param[in] 	node_id		Node id
 * @param[in] 	ep_id		EP id
 * @param[in] 	cls_id		Command Class
 * @param[out] 	name_buf	Name buffer
 * @param[in] 	name_buf_size	Name buffer size
 * @param[out] 	loc_buf			Location buffer
 * @param[in] 	loc_buf_size	Location buffer size
 * @return
 */
int hcaal_get_nameloc(void *net, uint8_t node_id, uint8_t ep_id, uint16_t cls_id, char *name_buf, size_t name_buf_size, char *loc_buf, size_t loc_buf_size)
{
	zwnet_p zwnet;
//	zwnoded_t noded;
	zwepd_t epd;
	int result = 0;
	
	zwnet = zwp_net_get_zwnet(net);
	
//	result = zwnet_get_node_by_id(zwnet, node_id, &noded);
//	if(result != ZW_ERR_NONE) {
//		ZWP_LOG(ZWP_LOG_DEBUG, "NODE for node=%u is not found", node_id);
//		return ZW_ERR_NODE_NOT_FOUND;
//	}
	memset(&epd, 0, sizeof(epd));
	result = zwnet_get_ep_by_id(zwnet, node_id, ep_id, &epd);
	if(result != ZW_ERR_NONE) {
		ZWP_LOG(ZWP_LOG_DEBUG, "EP for node_id=%u and ep_id=%u is not found", node_id, ep_id);
		return ZW_ERR_EP_NOT_FOUND;
	}
	
	if((name_buf != NULL) && (name_buf_size != 0)) {
		strncpy(name_buf, epd.name, name_buf_size - 1);
		name_buf[name_buf_size - 1] = '\0';
	}
	
	if((loc_buf != NULL) && (loc_buf_size != 0)) {
		strncpy(loc_buf, epd.loc, loc_buf_size - 1);
		loc_buf[loc_buf_size - 1] = '\0';
	}
	
	return ZW_ERR_NONE;
}


/**
* @brief Function to remove a polling request
* @param[in]	net	        Network runtime context
* @param[in]	handle	    handle of the polling request to remove
* @return		0 if success; else negative value on error
*/
int hcaal_poll_rm(void *net, uint16_t handle)
{
    zwp_net_t netp = (zwp_net_t)net;

    return zwnet_poll_rm(zwp_net_get_zwnet(netp), handle);
}


/**
* @brief Function to remove multiple polling requests
* @param[in]	net	        Network runtime context
* @param[in]	usr_token	usr_token of the polling requests to remove
* @return		0 if success; else negative value on error
*/
int hcaal_poll_rm_mul(void *net, uint32_t usr_token)
{
    zwp_net_t netp = (zwp_net_t)net;

    return zwnet_poll_rm_mul(zwp_net_get_zwnet(netp), usr_token);
}


/**
* @brief Function to check validity of node, ep and interface (cls)
* @param[in]	net	        Network runtime context
* @param[in]	flag_type	flag to specifiy if check is to be done for node, node and ep or node, ep and cls combination
* @param[in] 	node_id		Node id
* @param[in] 	ep_id		EP id
* @param[in] 	cls_id		Command Class
* @return		ZWSCN_TRUE if valid; else ZWSCN_FALSE on error
*/
ZWSCN_BOOL_T hcaal_is_obj_valid(void *net, ZWSCN_TYPE_VALIDITY_CHECK_T flag_type, uint8_t node_id, uint8_t ep_id, uint16_t cls_id)
{
	zwnet_p         zwnet;
    ZWSCN_BOOL_T    result = ZWSCN_FALSE;

	zwnet = zwp_net_get_zwnet(net);

    switch(flag_type)
    {
        case ZWSCN_CHECK_NODE:
            if (zwnet_get_node_by_id(zwnet, node_id, NULL) == 0)
            {
                result = ZWSCN_TRUE;
            }
            break;

        case ZWSCN_CHECK_NODE_EP:
            if (zwnet_get_ep_by_id(zwnet, node_id, ep_id, NULL) == 0)
            {
                result = ZWSCN_TRUE;
            }
            break;

        case ZWSCN_CHECK_NODE_EP_CLS:
            if (zwnet_get_if_by_id(zwnet, node_id, ep_id, cls_id, NULL) == 0)
            {
                result = ZWSCN_TRUE;
            }
            break;
    }

    return result;

}


/**
hcaal_nw_notify_cb - Callback function to notify the status of current operation
@param[in]	user	The high-level api context
@param[in]	op		Network operation ZWNET_OP_XXX
@param[in]	sts		The status of current operation
@param[in]	info	    additional information for the specified op and sts; NULL if there is no additional info
@return
*/
static void hcaal_nw_notify_cb(void *user, uint8_t op, uint16_t sts, zwnet_sts_t *info)
{
    zwp_net_t   net = (zwp_net_t)user;
    char        context_persistent[ZWSCN_LEN_CONTEXT_PERSISTENT];
    uint8_t     node_id = 0;

    if (net)
    {
        hcaal_ctx_p ctx;

        //Trying to get node_id for the replaced node
        if ((op == ZWNET_OP_RP_NODE) && (sts == OP_DONE))
        {
            node_id = zwp_net_get_last_rplc_node(net);
        }
        ctx = zwp_net_get_hcaal(net);

        if (ctx->notify_cb)
        {
            ctx->notify_cb(user, op, sts, info);
        }

        //For scene callback
        if (ctx->cb_fn.zwscn_function_network_notify_handler)
        {
            if ((op == ZWNET_OP_RP_NODE) && (sts == OP_DONE))
            {
                ctx->cb_fn.zwscn_function_network_notify_handler(net, op, sts, &node_id, sizeof(node_id));
                //Until we figure out how ti get the node id we just pass node_id 0 which will check all the nodes for changes
                //ctx->cb_fn.zwscn_function_network_notify_handler(net, op, sts, NULL, 0);
            }
            else if (zwp_net_get_ctx_persistent(net, context_persistent, sizeof(context_persistent)))
            {
                ctx->cb_fn.zwscn_function_network_notify_handler(net, op, sts, context_persistent, 25);
            }
            else
            {
                ZWP_LOG(ZWP_LOG_ERR, "zwp_net_get_ctx_persistent() failed");
                ctx->cb_fn.zwscn_function_network_notify_handler(net, op, sts, NULL, 0);
            }
        }
    }
}


/**
hcaal_nw_node_cb - Callback function to notify node is added, deleted, or updated
@param[in]	user	    The high-level api context
@param[in]	noded	Node
@param[in]	mode	    The node status
@return
*/
static void hcaal_nw_node_cb(void *user, zwnoded_p noded, int mode)
{
    zwp_net_t net = (zwp_net_t)user;

	ZWP_LOG(ZWP_LOG_DEBUG, "Beginning of nw_node_cb");

    if (net)
    {
        hcaal_ctx_p ctx;

        ctx = zwp_net_get_hcaal(net);

		zwp_net_reference_count_increment(net);

        if (ctx->node_cb)
        {
            ctx->node_cb(user, noded, mode);
        }

        //For scene callback
        if (ctx->cb_fn.zwscn_function_node_notify_handler)
        {
            ZWSCN_NODE_NOTIFY_MODE_T node_status;

            switch (mode)
            {
                case ZWNET_NODE_ADDED:
                    node_status = ZWSCN_NODE_ADDED;
                    break;

                case ZWNET_NODE_REMOVED:
                    node_status = ZWSCN_NODE_REMOVED;
                    break;

                case ZWNET_NODE_UPDATED:
                    node_status = ZWSCN_NODE_UPDATED;
                    break;

                case ZWNET_NODE_STATUS_ALIVE:
                    ZWP_LOG(ZWP_LOG_DEBUG, "Node:%u is alive", (unsigned)noded->nodeid);
                    node_status = ZWSCN_NODE_STATUS_ALIVE;
                    break;

                case ZWNET_NODE_STATUS_DOWN:
                    ZWP_LOG(ZWP_LOG_DEBUG, "Node:%u is down", (unsigned)noded->nodeid);
                    node_status = ZWSCN_NODE_STATUS_DOWN;
                    break;

                default:
					ZWP_LOG(ZWP_LOG_DEBUG, "End of nw_node_cb 1");
					zwp_net_reference_count_decrement(net);
                    return;
            }

            ctx->cb_fn.zwscn_function_node_notify_handler(net, noded->nodeid, node_status);
        }
    }

	zwp_net_reference_count_decrement(net);
	ZWP_LOG(ZWP_LOG_DEBUG, "End of nw_node_cb 2");
}

static void hcaal_unhandled_cmd_cb(void *user, uint8_t src_node, uint8_t src_ep, uint8_t *cmd, uint16_t cmd_len) {
	zwp_net_t net = (zwp_net_t)user;
	int i=0;
	uint16_t cls_id=0;
	uint8_t cls_cmd=0;
	uint8_t value=0;
	
	if((user == NULL) || (cmd == NULL) || (cmd_len == 0)) {
		goto l_err_args;
	}
	ZWP_LOG(ZWP_LOG_DEBUG, "Received Unhandled command from node:%d ep:%d", src_node, src_ep);
	ZWP_LOG(ZWP_LOG_DEBUG, "Unhandled command buffer_length: %d", cmd_len);
	ZWP_LOG(ZWP_LOG_DEBUG, "Unhandled command buffer:");
	for(i=0; i < cmd_len; ++i) {
		ZWP_LOG(ZWP_LOG_DEBUG, "0x%02X\t", cmd[i]);
	}
	
	cls_id = *(cmd);
	cls_cmd = *(cmd + 1);
	
	if((cls_id == COMMAND_CLASS_BASIC) && (cls_cmd == BASIC_SET)) {
		value = *(cmd + 2);
		ZWP_LOG(ZWP_LOG_DEBUG, "Unhandled COMMAND_CLASS_BASIC - BASIC_SET command parsed with value: %d", value);
		hcaal_basic_set_rpt_cb(net, src_node, src_ep, cls_id, value);
	}
	
l_err_args:
	return;
}

/**
hcaal_scene_cb_set - Set scene notification and report callback functions
@param[in]	hcaal_ctx	    HCAPI AAL context
@param[in]	scene_cb	    Scene notification and report callback functions
@return
*/
void hcaal_scene_cb_set(hcaal_ctx_p hcaal_ctx, zwscn_init_response_p scene_cb)
{
    hcaal_ctx->cb_fn = *scene_cb; //C structure copy
}


#ifdef  SHOW_HCAPI_MSG
/**
hcaal_msg_show - show message to the user
@param[in] msg   The output message to printf.
@return
*/
static void hcaal_msg_show(void *msg, void *display_ctx)
{
    fputs((const char *)msg, stderr);
}
#endif


/**
hcaal_init - Initialize Z-wave network
@param[in]	hcaal_ctx	    HCAPI AAL context
@param[in]	net	            Network
@param[in]	init	        Client initialization parameters
@param[out]	zwnet	        Z-wave network handle
@return		ZW_ERR_xxx
*/
int hcaal_init(hcaal_ctx_p hcaal_ctx, zwp_net_t net,
               zwnet_init_p init, zwnet_p *zwnet)
{
    int ret;

    //Clear everything
    memset(hcaal_ctx, 0, sizeof(hcaal_ctx_t));

    //Save callback functions
    hcaal_ctx->notify_cb = init->notify;
    hcaal_ctx->node_cb = init->node;

    //Overwrite the callback functions
    init->notify = hcaal_nw_notify_cb;
    init->node = hcaal_nw_node_cb;
	init->unhandled_cmd = hcaal_unhandled_cmd_cb;
#ifdef  SHOW_HCAPI_MSG
    init->print_txt_fn = hcaal_msg_show;
#endif

	ZWP_LOG(ZWP_LOG_DEBUG, "Begin HCAPI net_init()");
	if ((ret = zwnet_init(init, zwnet)) != ZW_ERR_NONE)
	{
        return ret;
	}
	ZWP_LOG(ZWP_LOG_DEBUG, "End of HCAPI net_init(), zwnet = %p", *zwnet);

    //Restore callback functions
    init->notify = hcaal_ctx->notify_cb;
    init->node = hcaal_ctx->node_cb;

    //Save everything that is useful
    hcaal_ctx->net = net;
    hcaal_ctx->zwnet = *zwnet;

    return ret;

}


/**
hcaal_exit - Clean up
@param[in]	hcaal_ctx	    HCAPI AAL context
@param[in]	net		        Network

@return
*/
void hcaal_exit(hcaal_ctx_p hcaal_ctx, zwnet_p net)
{
    zwnet_exit(net);
}


/**
hcaal_get_ctx - Get HCAPI AAL context
@param[in]	ifd         Interface descriptor
return      HCAPI AAL context on success; else return NULL
*/
static hcaal_ctx_p hcaal_get_ctx(zwifd_p ifd)
{
    zwp_net_t net = (zwp_net_t)zwnet_get_user(ifd->net);

    if (net)
    {
        return zwp_net_get_hcaal(net);
    }
    return NULL;
}


/**
hcaal_basic_rpt_cb - Generic report callback for switch, binary sensor, battery, lock, etc.
@param[in]	ifd	    interface
@param[in]	level	level
*/
static void hcaal_basic_rpt_cb(zwifd_p ifd, uint8_t level, time_t ts)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {
        if (hcaal_ctx->basic_rpt)
        {
            hcaal_ctx->basic_rpt(ifd, level, ts);
        }

        //For scene
        if (hcaal_ctx->cb_fn.zwscn_function_report_notify_handler)
        {
            zwscn_report_basic_t rpt;
            rpt.utime = zwu_time_now();
            rpt.value = level;

            hcaal_ctx->cb_fn.zwscn_function_report_notify_handler(hcaal_ctx->net, ifd->nodeid,
                                                                  ifd->epid, ifd->cls, BASIC_REPORT, &rpt, sizeof(rpt));
        }
    }
}

static void hcaal_basic_set_rpt_cb(zwp_net_t net, uint8_t nodeid, uint8_t epid, uint16_t cls, uint8_t level)
{
    hcaal_ctx_p hcaal_ctx = zwp_net_get_hcaal(net);

    if (hcaal_ctx)
    {
        //For scene
        if (hcaal_ctx->cb_fn.zwscn_function_report_notify_handler)
        {
            zwscn_report_basic_t rpt;
            rpt.utime = zwu_time_now();
            rpt.value = level;

            hcaal_ctx->cb_fn.zwscn_function_report_notify_handler(hcaal_ctx->net, nodeid,
                                                                  epid, cls, BASIC_SET, &rpt, sizeof(rpt));
        }
    }
}


/**
hcaal_basic_rpt_set - Setup a basic report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
return      ZW_ERR_XXX
*/
int hcaal_basic_rpt_set(zwifd_p ifd, zwrep_ts_fn rpt_cb)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {   //Save the report callback
        hcaal_ctx->basic_rpt = rpt_cb;
        return zwif_basic_rpt_set(ifd, hcaal_basic_rpt_cb);
    }

    return ZW_ERR_FAILED;
}


/**
hcaal_switch_rpt_cb - Report callback for switch
@param[in]	ifd	    interface
@param[in]	on		0=off, else on
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/
static void hcaal_switch_rpt_cb(zwifd_p ifd, uint8_t on, time_t ts)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {
        if (hcaal_ctx->switch_rpt)
        {
            hcaal_ctx->switch_rpt(ifd, on, ts);
        }

        //For scene
        if (hcaal_ctx->cb_fn.zwscn_function_report_notify_handler)
        {
            zwscn_report_switch_binary_t rpt;
            rpt.utime = zwu_time_now();
            rpt.value = on;

            hcaal_ctx->cb_fn.zwscn_function_report_notify_handler(hcaal_ctx->net, ifd->nodeid,
                                                                  ifd->epid, ifd->cls, SWITCH_BINARY_REPORT, &rpt, sizeof(rpt));
        }
    }
}


/**
hcaal_switch_rpt_set - Setup a switch report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
return      ZW_ERR_XXX
*/
int hcaal_switch_rpt_set(zwifd_p ifd, zwrep_switch_fn rpt_cb)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {   //Save the report callback
        hcaal_ctx->switch_rpt = rpt_cb;
        return zwif_switch_rpt_set(ifd, hcaal_switch_rpt_cb);
    }

    return ZW_ERR_FAILED;

}


/**
hcaal_level_rpt_cb - Multi-level switch report callback
@param[in]	ifd	    interface
@param[in]	level	level
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/
static void hcaal_level_rpt_cb(zwifd_p ifd, uint8_t level, time_t ts)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {
        if (hcaal_ctx->level_rpt)
        {
            hcaal_ctx->level_rpt(ifd, level, ts);
        }

        //For scene
        if (hcaal_ctx->cb_fn.zwscn_function_report_notify_handler)
        {
            zwscn_report_switch_multilevel_t rpt;
            rpt.utime = zwu_time_now();
            rpt.value = level;

            hcaal_ctx->cb_fn.zwscn_function_report_notify_handler(hcaal_ctx->net, ifd->nodeid,
                                                                  ifd->epid, ifd->cls, SWITCH_MULTILEVEL_REPORT, &rpt, sizeof(rpt));
        }
    }
}


/**
hcaal_level_rpt_set - Setup a multi-level switch report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
return      ZW_ERR_XXX
*/
int hcaal_level_rpt_set(zwifd_p ifd, zwrep_ts_fn rpt_cb)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {   //Save the report callback
        hcaal_ctx->level_rpt = rpt_cb;
        return zwif_level_rpt_set(ifd, hcaal_level_rpt_cb);
    }

    return ZW_ERR_FAILED;
}

/**
hcaal_color_rpt_cb - Color switch report callback
@param[in]	ifd	    interface
@param[in]	data	Color switch report data
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/
static void hcaal_color_rpt_cb(zwifd_p ifd, zwcolor_t *data, time_t ts)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {
        if (hcaal_ctx->color_rpt)
        {
            hcaal_ctx->color_rpt(ifd, data, ts);
        }
        //todo: for scene
    }
}

/**
hcaal_color_rpt_set - Setup a color switch report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
return      ZW_ERR_XXX
*/
int hcaal_color_rpt_set(zwifd_p ifd, zwrep_color_sw_get_fn rpt_cb)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {   //Save the report callback
        hcaal_ctx->color_rpt = rpt_cb;
        return zwif_color_sw_rpt_set(ifd, hcaal_color_rpt_cb);
    }

    return ZW_ERR_FAILED;
}

/**
hcaal_dlck_op_rpt_cb - Door lock operation report callback
@param[in]	ifd	    interface
@param[in]	op_sts  operation status
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/
static void hcaal_dlck_op_rpt_cb(zwifd_p ifd, zwdlck_op_p  op_sts, time_t ts)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {
        if (hcaal_ctx->dlck_op_rpt)
        {
            hcaal_ctx->dlck_op_rpt(ifd, op_sts, ts);
        }

        //For scene
        if (hcaal_ctx->cb_fn.zwscn_function_report_notify_handler)
        {
            zwscn_report_door_lock_operation_t rpt;
            rpt.utime = zwu_time_now();
            rpt.operation = *op_sts;

            hcaal_ctx->cb_fn.zwscn_function_report_notify_handler(hcaal_ctx->net, ifd->nodeid,
                                                                  ifd->epid, ifd->cls, DOOR_LOCK_OPERATION_REPORT, &rpt, sizeof(rpt));
        }
    }
}


/**
hcaal_dlck_op_rpt_set - Setup a door lock operation report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
return      ZW_ERR_XXX
*/
int hcaal_dlck_op_rpt_set(zwifd_p ifd, zwrep_dlck_op_fn rpt_cb)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {   //Save the report callback
        hcaal_ctx->dlck_op_rpt = rpt_cb;
        return zwif_dlck_op_rpt_set(ifd, hcaal_dlck_op_rpt_cb);
    }

    return ZW_ERR_FAILED;
}


/**
hcaal_thrmo_setp_rpt_cb - Thermostat setpoint report callback
@param[in]	ifd	    interface
@param[in]	data    setpoint data
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/
static void hcaal_thrmo_setp_rpt_cb(zwifd_p ifd, zwsetp_p data, time_t ts)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {
        if (hcaal_ctx->thrmo_setp_rpt)
        {
            hcaal_ctx->thrmo_setp_rpt(ifd, data, ts);
        }

        //For scene
        if (hcaal_ctx->cb_fn.zwscn_function_report_notify_handler)
        {
            zwscn_report_thermostat_setpoint_t rpt;
            rpt.utime = zwu_time_now();
            rpt.value = *data;

            hcaal_ctx->cb_fn.zwscn_function_report_notify_handler(hcaal_ctx->net, ifd->nodeid,
                                                                  ifd->epid, ifd->cls, THERMOSTAT_SETPOINT_REPORT, &rpt, sizeof(rpt));
        }
    }
}


/**
hcaal_thrmo_setp_rpt_set - Setup a thermostat setpoint report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
return      ZW_ERR_XXX
*/
int hcaal_thrmo_setp_rpt_set(zwifd_p ifd, zwrep_thrmo_setp_fn rpt_cb)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {   //Save the report callback
        hcaal_ctx->thrmo_setp_rpt = rpt_cb;
        return zwif_thrmo_setp_rpt_set(ifd, hcaal_thrmo_setp_rpt_cb);
    }

    return ZW_ERR_FAILED;
}


/**
hcaal_bsensor_rpt_cb - Binary sensor report callback
@param[in]	ifd	    interface
@param[in]	state	0=idle, else event detected
@@param[in]	 ts    time stamp
*/
static void hcaal_bsensor_rpt_cb(zwifd_p ifd, uint8_t state, uint8_t type, time_t ts)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {
        if (hcaal_ctx->bsensor_rpt)
        {
            hcaal_ctx->bsensor_rpt(ifd, state, type, ts);
        }

        //For scene
        if (hcaal_ctx->cb_fn.zwscn_function_report_notify_handler)
        {
            zwscn_report_sensor_binary_t rpt;

            if (state == 0)
            {   //idle
                rpt.event_utime = 0;
                rpt.idle_utime = zwu_time_now();
            }
            else
            {   //event detected
                rpt.idle_utime = 0;
                rpt.event_utime = zwu_time_now();
            }
            rpt.sensor_value = state;
			rpt.type = type;

            hcaal_ctx->cb_fn.zwscn_function_report_notify_handler(hcaal_ctx->net, ifd->nodeid,
                                                                  ifd->epid, ifd->cls, SENSOR_BINARY_REPORT, &rpt, sizeof(rpt));
        }
    }
}


/**
hcaal_bsensor_rpt_set - Setup a binary sensor report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
return      ZW_ERR_XXX
*/
int hcaal_bsensor_rpt_set(zwifd_p ifd, zwrep_bsensor_fn rpt_cb)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {   //Save the report callback
        hcaal_ctx->bsensor_rpt = rpt_cb;
        return zwif_bsensor_rpt_set(ifd, hcaal_bsensor_rpt_cb);
    }

    return ZW_ERR_FAILED;
}


/**
hcaal_sensor_rpt_cb - Sensor report callback
@param[in]	ifd	    interface
@param[in]	data	current value and unit of the sensor
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/
static void hcaal_sensor_rpt_cb(zwifd_p ifd, zwsensor_p data, time_t ts)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {
        if (hcaal_ctx->sensor_rpt)
        {
            hcaal_ctx->sensor_rpt(ifd, data, ts);
        }

        //For scene
        if (hcaal_ctx->cb_fn.zwscn_function_report_notify_handler)
        {
            zwscn_report_sensor_multilevel_t rpt;
            rpt.utime = zwu_time_now();
            rpt.value = *data;

            hcaal_ctx->cb_fn.zwscn_function_report_notify_handler(hcaal_ctx->net, ifd->nodeid,
                                                                  ifd->epid, ifd->cls, SENSOR_MULTILEVEL_REPORT, &rpt, sizeof(rpt));
        }
    }
}


/**
hcaal_sensor_rpt_set - Setup a sensor report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
return      ZW_ERR_XXX
*/
int hcaal_sensor_rpt_set(zwifd_p ifd, zwrep_sensor_fn rpt_cb)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {   //Save the report callback
        hcaal_ctx->sensor_rpt = rpt_cb;
        return zwif_sensor_rpt_set(ifd, hcaal_sensor_rpt_cb);
    }

    return ZW_ERR_FAILED;
}

/**
hcaal_alrm_rpt_cb - Alarm report callback
@param[in]	ifd	    interface
@param[in]	data	current value and unit of the alarm
*/
static void hcaal_alrm_rpt_cb(zwifd_p ifd, zwalrm_p data, time_t ts)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {
        if (hcaal_ctx->alrm_rpt)
        {
            hcaal_ctx->alrm_rpt(ifd, data, ts);
        }

        //For scene
        if (hcaal_ctx->cb_fn.zwscn_function_report_notify_handler)
        {
            zwscn_report_alarm_t rpt;
            rpt.utime = zwu_time_now();
            rpt.value = *data;

            hcaal_ctx->cb_fn.zwscn_function_report_notify_handler(hcaal_ctx->net, ifd->nodeid,
                                                                  ifd->epid, ifd->cls, ALARM_REPORT, &rpt, sizeof(rpt));
        }
    }
}


/**
hcaal_alrm_rpt_set - Setup a alarm report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
return      ZW_ERR_XXX
*/
int hcaal_alrm_rpt_set(zwifd_p ifd, zwrep_alrm_fn rpt_cb)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {   //Save the report callback
        hcaal_ctx->alrm_rpt = rpt_cb;
        return zwif_alrm_rpt_set(ifd, hcaal_alrm_rpt_cb);
    }

    return ZW_ERR_FAILED;
}

/**
hcaal_central_scene_rpt_cb - Central Scene report callback
@param[in]	ifd	    interface
@param[in]	data	current value and unit of the central_scene
*/
static void hcaal_central_scene_rpt_cb(zwifd_p ifd, zwcsc_notif_p data, time_t ts)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {
        if (hcaal_ctx->central_scene_rpt)
        {
            hcaal_ctx->central_scene_rpt(ifd, data, ts);
        }

        //For scene
        if (hcaal_ctx->cb_fn.zwscn_function_report_notify_handler)
        {
            zwscn_report_central_scene_t rpt;
            rpt.utime = zwu_time_now();
            rpt.central_scene_key_number = data->sceneNo;
			rpt.central_scene_key_attribute = data->keyAttr;

            hcaal_ctx->cb_fn.zwscn_function_report_notify_handler(hcaal_ctx->net, ifd->nodeid,
                                                                  ifd->epid, ifd->cls, CENTRAL_SCENE_NOTIFICATION, &rpt, sizeof(rpt));
        }
    }
}


/**
hcaal_central_scene_rpt_set - Setup a central scene report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
return      ZW_ERR_XXX
*/
int hcaal_central_scene_rpt_set(zwifd_p ifd, zwrep_csc_fn rpt_cb)
{
    hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

    if (hcaal_ctx)
    {   //Save the report callback
        hcaal_ctx->central_scene_rpt = rpt_cb;
        return zwif_csc_rpt_set(ifd, hcaal_central_scene_rpt_cb);
    }

    return ZW_ERR_FAILED;
}

/**
hcaal_barrier_op_rpt_cb - Report callback for barrier_op
@param[in]	ifd	    interface
@param[in]	state	barrier operator state, ZW_BAR_STA_XXX. It may also be in a range from 1 to 99 (%) open.
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
values should be ignored.
*/
static void hcaal_barrier_op_rpt_cb(zwifd_p ifd, uint8_t state, time_t ts)
{
	hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

	if (hcaal_ctx)
	{
		if (hcaal_ctx->barrier_op_rpt)
		{
			hcaal_ctx->barrier_op_rpt(ifd, state, ts);
		}

		//For scene
		//if (hcaal_ctx->cb_fn.zwscn_function_report_notify_handler)
		//{
		//	zwscn_report_barrier_op_binary_t rpt;
		//	rpt.utime = zwu_time_now();
		//	rpt.value = on;

		//	hcaal_ctx->cb_fn.zwscn_function_report_notify_handler(hcaal_ctx->net, ifd->nodeid,
		//		ifd->epid, ifd->cls, SWITCH_BINARY_REPORT, &rpt, sizeof(rpt));
		//}
	}
}


/**
hcaal_barrier_op_rpt_set - Setup a barrier_op report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
return      ZW_ERR_XXX
*/
int hcaal_barrier_op_rpt_set(zwifd_p ifd, zwrep_barrier_fn rpt_cb)
{
	hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

	if (hcaal_ctx)
	{   //Save the report callback
		hcaal_ctx->barrier_op_rpt = rpt_cb;
		return zwif_barrier_rpt_set(ifd, hcaal_barrier_op_rpt_cb);
	}

	return ZW_ERR_FAILED;

}

/**
hcaal_barrier_op_rpt_cb - Report callback for barrier_op
@param[in]	ifd	    interface
@param[in]	subsys_type	barrier operator subsystem type, ZW_BAR_NOTIF_TYP_XXX.
@param[in]	subsys_sta	barrier operator subsystem state: 0=off; 0xFF=on.
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
values should be ignored.
*/
static void hcaal_barrier_op_subsys_rpt_cb(zwifd_p ifd, uint8_t subsys_type, uint8_t subsys_sta, time_t ts)
{
	hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

	if (hcaal_ctx)
	{
		if (hcaal_ctx->barrier_op_subsys_rpt)
		{
			hcaal_ctx->barrier_op_subsys_rpt(ifd, subsys_type, subsys_sta, ts);
		}

		//For scene
		//if (hcaal_ctx->cb_fn.zwscn_function_report_notify_handler)
		//{
		//	zwscn_report_barrier_op_binary_t rpt;
		//	rpt.utime = zwu_time_now();
		//	rpt.value = on;

		//	hcaal_ctx->cb_fn.zwscn_function_report_notify_handler(hcaal_ctx->net, ifd->nodeid,
		//		ifd->epid, ifd->cls, SWITCH_BINARY_REPORT, &rpt, sizeof(rpt));
		//}
	}
}

/**
hcaal_barrier_op_rpt_set - Setup a barrier_op subsystem report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
return      ZW_ERR_XXX
*/
int hcaal_barrier_op_subsys_rpt_set(zwifd_p ifd, zwrep_barrier_subsys_fn rpt_cb)
{
	hcaal_ctx_p hcaal_ctx = hcaal_get_ctx(ifd);

	if (hcaal_ctx)
	{   //Save the report callback
		hcaal_ctx->barrier_op_subsys_rpt = rpt_cb;
		return zwif_barrier_notif_rpt_set(ifd, hcaal_barrier_op_subsys_rpt_cb);
	}

	return ZW_ERR_FAILED;

}