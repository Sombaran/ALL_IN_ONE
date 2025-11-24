#include "integration.h"
#include "common.h"


extern hl_appl_ctx_t appl_ctx;
extern request_t request;
extern int get_state;
extern int msqid_resp;
extern int msqid_req;

/**
response_DM - This function will send response to the device manager by constructing the response structure using MESSAGE QUEUE.
*/
void response_DM(response_t dm_response)
{
	syslog(LOG_DEBUG, "ZW: In response_DM");
	syslog(LOG_DEBUG, "ZW: sizeof(response_t):%ld",sizeof(response_t));
	dm_response.mtype = 1;
	if (msgsnd(msqid_resp, &dm_response, sizeof(response_t), 0) < 0)
	{
		syslog(LOG_DEBUG, "ZW:  %d\n, %d\n, %d\n, %d\n", msqid_resp, dm_response.mtype, dm_response.home_id, dm_response.node_id);
		exit(0);
	}
	else
	{
		syslog(LOG_DEBUG, "ZW: success response dm");
	}

}





/**
bootup_scenario - Upon bootup and Upon Gateway discovery, this function updates the node details if any to the DeviceManager.
*/
void bootup_scenario(void)
{
	syslog(LOG_DEBUG, "ZW: In bootup_scenario");
	int result;
	response_t response = {0};
	zwnoded_t node;
	zwepd_t ep;
	zwifd_t intf;
	char i = 0, j = 0;;
	
	//Get first node (controller node)
	result = zwnet_get_node(appl_ctx.zwnet, &node);
	if (result != 0)
	{
		return result;
	}

	while (!zwnode_get_next(&node, &node)) //get next node
	{
			syslog(LOG_DEBUG, "ZW:  get next node ");
			response.node_id = node.nodeid;
		//if(!zwnode_update(&node))
		// {

			if (!zwnode_get_ep(&node, &ep)) //get first endpoint of the node
			{
				syslog(LOG_DEBUG, "ZW: Node Updated");
				do
				{
					response.ep[i].ep_id = intf.epid;
					if (!zwep_get_if(&ep, &intf)) //get first interface of the endpoint
					{
						do
						{
							switch (intf.cls)
							{
							case COMMAND_CLASS_SWITCH_BINARY:
							     dm_value(&intf);
							     j++;
							     break;

							case COMMAND_CLASS_SWITCH_MULTILEVEL:
							     dm_value(&intf);
							     break;

							case COMMAND_CLASS_SWITCH_COLOR:
							     dm_value(&intf);
							     break;
						
							case COMMAND_CLASS_DOOR_LOCK:
							     dm_value(&intf);
							     break;

							case COMMAND_CLASS_SENSOR_MULTILEVEL:
							     dm_value(&intf);
							     break;

							case COMMAND_CLASS_BATTERY:
							     dm_value(&intf);
							     break;

					                case COMMAND_CLASS_ALARM:
							     dm_value(&intf);
							     break;

							default:
							     syslog(LOG_DEBUG, "ZW: Wrong CC");
		                           		     break;
							}

						} while (!zwif_get_next(&intf, &intf)); //get next interface
					}
					i++;
				} while (!zwep_get_next(&ep, &ep)); //get next endpoint
			}
		
		//}

  }

}


