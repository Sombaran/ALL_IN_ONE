/**
 @file   zwave_app.c - Sample application to discover a Z/IP gateway.

 @author Smartron

 @version   0.1  06-2-18  Initial Push
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdbool.h>
#include <stdlib.h>
#include "common.h"
#include "integration.h"

response_t discovery_struct;
size_t buflen_resp;
uint8_t check_discovery_flag=-1;
int msqid_resp;
int msqid_req;
int find_zipgateway = 0;
uint32_t homeid;
uint8_t add_node_flag = 0;
uint8_t remove_node_flag = 0;
hl_appl_ctx_t appl_ctx = { 0 };
request_t request = { 0 };
request_t requestdummy = { 0 };
uint8_t discovery_node_id = 0;
int endpoint_count=0;
enum actutor_sensor {
	sensors, actuators, wrong_cc,
};
char addr_str[80];

/**
 hex2bin - Convert hex character to binary
 @param[in] c        hex character
 @return  Value of hex character on success, negative value on failure
 */
static int hex2bin(char c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
	} else if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	} else if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	} else {
		return -1;
	}
}

/**
 hexstring_to_bin - Convert ASCII hexstring to binary string
 @param[in] psk_str   ASCII hexstring
 @param[in] psk_len   ASCII hexstring length (must be even number)
 @param[out] psk_bin  Binary string
 @return  Zero on success, non-zero on failure
 */
static int hexstring_to_bin(char *psk_str, int psk_len, uint8_t *psk_bin) {
	int i = 0;
	int val;

	while (psk_len > 0) {
		val = hex2bin(*psk_str++);
		if (val < 0)
			return -1;
		psk_bin[i] = (val & 0x0F) << 4;

		val = hex2bin(*psk_str++);
		if (val < 0)
			return -1;
		psk_bin[i] |= (val & 0x0F);

		i++;
		psk_len -= 2;
	}

	return 0;
}
/**
 gw_intf_get - Search for the Z/IP gateway interface
 @param[in]  net     network handle
 @param[out] gw_if   Z/IP gateway interface
 @return  0 on success; otherwise negative number
 */
static int gw_intf_get(zwnet_p net, zwifd_t *gw_if) {
	int result;
	zwnoded_t node;
	zwepd_t ep;
	zwifd_t intf;

//Get first node (controller node)
	result = zwnet_get_node(net, &node);
	if (result != 0) {
		return result;
	}

	if (!zwnode_get_ep(&node, &ep)) //get first endpoint of the node
			{
		if (!zwep_get_if(&ep, &intf)) //get first interface of the endpoint
				{
			do {
				if (intf.cls == COMMAND_CLASS_ZIP_GATEWAY) { //Found
					*gw_if = intf;
					return 0;
				}

			} while (!zwif_get_next(&intf, &intf)); //get next interface
		}
	}

	return ZW_ERR_INTF_NOT_FOUND;
}
/**
 hl_unsolicited_addr_setup - Setup unsolicited address to receive unsolicited report
 @param[in]	hl_appl	    The high-level api context
 @return  0 on success, negative error number on failure
 */
static int hl_unsolicited_addr_setup(hl_appl_ctx_t *hl_appl) {
	int result;
	uint8_t local_ip[16];
	uint16_t local_port;
	zwifd_t gw_ifd;

//Get local Z/IP client listening address and port
	result = zwnet_local_addr_get(hl_appl->zwnet, hl_appl->zip_gw_ip, local_ip,
			hl_appl->use_ipv4);

	if (result != 0) {
		syslog(LOG_ERR,
				"ZW: couldn't get local Z/IP client listening address: %d",
				result);
		return result;
	}

	local_port = zwnet_listen_port_get(hl_appl->zwnet);

	if (hl_appl->use_ipv4) { //Convert to IPv4-mapped IPv6 address
		uint8_t unsolicit_ipv4[4];

		/*Save the IPv4 address*/
		memcpy(unsolicit_ipv4, local_ip, 4);

		/*Convert the IPv4 address to IPv4-mapped IPv6 address*/
		memset(local_ip, 0, 10);
		local_ip[10] = 0xFF;
		local_ip[11] = 0xFF;
		memcpy(&local_ip[12], unsolicit_ipv4, 4);
	}

	result = gw_intf_get(hl_appl->zwnet, &gw_ifd);
	if (result != 0) {
		syslog(LOG_ERR, "ZW: couldn't find Z/IP gateway interface: %d", result);
		return result;
	}

	result = zwif_gw_unsolicit_set(&gw_ifd, local_ip, local_port);

	if (result != 0) {
		syslog(LOG_ERR, "ZW: Error: couldn't set unsolicited address: %d",
				result);
	}

	return result;
}

/**
 hl_nw_tx_cb - Callback function to notify application transmit data status
 @param[in]	user	    The high-level api context
 param[in]	tx_sts	    Transmit status ZWNET_TX_xx
 @return
 */
static void hl_nw_tx_cb(void *user, uint8_t tx_sts) {
	response_t response = { 0 };
	static const char *tx_cmplt_sts[] = { "ok", "timeout: no ACK received",
			"system error", "destination host needs long response time",
			"frame failed to reach destination host" };

	if (tx_sts == TRANSMIT_COMPLETE_OK) {
		syslog(LOG_DEBUG,
				"ZW: Higher level appl send data completed successfully");
		if (requestdummy.operation == CONSIGN
				&& requestdummy.cmd_cls == COMMAND_CLASS_SWITCH_COLOR) {
			response.msgid = requestdummy.msgid;
			response.operation = requestdummy.operation;
			response.home_id = homeid;
			response.node_id = requestdummy.node_id;
			response.ep[0].no_of_intf = 1;
			response.err_code = OP_SUCCESS;
			response.no_of_ep = 1;
			response.ep[0].ep_id = requestdummy.ep_id;
			response.ep[0].generic_dc = requestdummy.generic_dc;
			response.ep[0].specific_dc = requestdummy.specific_dc;
			response.ep[0].intf[0].cmd_cls = requestdummy.cmd_cls;
			response.ep[0].intf[0].property_type = requestdummy.property_type;

			for (int count_value = 0; count_value < NO_OF_COLOR_COMP;
					count_value++)
				response.ep[0].intf[0].property_value.arr[count_value] =
						requestdummy.property_value.arr[count_value];

			response_DM(response);
			memset(&requestdummy,0,sizeof(requestdummy));
			memset(&response, 0, sizeof(response));
			syslog(LOG_DEBUG, "ZW: Color switch response sent successfully");
		}
		else if (requestdummy.operation == CONSIGN
				&& (requestdummy.cmd_cls == COMMAND_CLASS_SWITCH_MULTILEVEL)) {
			syslog(LOG_DEBUG, "ZW: Bulb response");
			response.msgid = requestdummy.msgid;
			response.operation = requestdummy.operation;
			response.home_id = homeid;
			response.node_id = requestdummy.node_id;
			response.ep[0].no_of_intf = 1;
			response.err_code = OP_SUCCESS;
			response.no_of_ep = 1;
			response.ep[0].ep_id = requestdummy.ep_id;
			response.ep[0].generic_dc = requestdummy.generic_dc;
			response.ep[0].specific_dc = requestdummy.specific_dc;
			response.ep[0].intf[0].cmd_cls = requestdummy.cmd_cls;
			response.ep[0].intf[0].property_type = requestdummy.property_type;
			response.ep[0].intf[0].property_value.ch=requestdummy.property_value.ch;
			response_DM(response);
			memset(&requestdummy,0,sizeof(requestdummy));
			memset(&response, 0, sizeof(response));
			syslog(LOG_DEBUG, "ZW: Bulb response sent successfully");
		}
		else if (requestdummy.operation == CONSIGN
				&& (requestdummy.cmd_cls == COMMAND_CLASS_SWITCH_BINARY)) {
			syslog(LOG_DEBUG, "ZW: plug and switchboard response");
			response.msgid = requestdummy.msgid;
			response.operation = requestdummy.operation;
			response.home_id = homeid;
			response.node_id = requestdummy.node_id;
			response.ep[0].no_of_intf = 1;
			response.err_code = OP_SUCCESS;
			response.no_of_ep = 1;
			response.ep[0].ep_id = requestdummy.ep_id;
			response.ep[0].generic_dc = requestdummy.generic_dc;
			response.ep[0].specific_dc = requestdummy.specific_dc;
			response.ep[0].intf[0].cmd_cls = requestdummy.cmd_cls;
			response.ep[0].intf[0].property_type = requestdummy.property_type;
			response.ep[0].intf[0].property_value.ch=requestdummy.property_value.ch;
			response_DM(response);
			memset(&requestdummy,0,sizeof(requestdummy));
			memset(&response, 0, sizeof(response));
			syslog(LOG_DEBUG, "ZW: plug response sent successfully");
		}
		else if (requestdummy.operation == CONSIGN
				&& (requestdummy.cmd_cls == COMMAND_CLASS_DOOR_LOCK)) {
			syslog(LOG_DEBUG, "ZW: Door lock response");
			response.msgid = requestdummy.msgid;
			response.operation = requestdummy.operation;
			response.home_id = homeid;
			response.node_id = requestdummy.node_id;
			response.ep[0].no_of_intf = 1;
			response.err_code = OP_SUCCESS;
			response.no_of_ep = 1;
			response.ep[0].ep_id = requestdummy.ep_id;
			response.ep[0].generic_dc = requestdummy.generic_dc;
			response.ep[0].specific_dc = requestdummy.specific_dc;
			response.ep[0].intf[0].cmd_cls = requestdummy.cmd_cls;
			response.ep[0].intf[0].property_type = requestdummy.property_type;
			response.ep[0].intf[0].property_value.ch=requestdummy.property_value.ch;
			response_DM(response);
			memset(&requestdummy,0,sizeof(requestdummy));
			memset(&response, 0, sizeof(response));
			syslog(LOG_DEBUG, "ZW: Door response sent successfully");
		}
	
	} else {
		syslog(LOG_ERR,
				"ZW: Higher level appl send data completed with error: %s",
				(tx_sts < sizeof(tx_cmplt_sts) / sizeof(char *)) ?
						tx_cmplt_sts[tx_sts] : "unknown");
		if(requestdummy.operation==CONSIGN || requestdummy.operation==OP_SUCCESS || requestdummy.operation==NODE_STATUS )
		{
		syslog(LOG_DEBUG, "ZW: Higher level error");
		response.msgid = requestdummy.msgid;
		response.operation = requestdummy.operation;
		response.home_id = homeid;
		response.node_id = requestdummy.node_id;
		response.err_code = ERR_HIGH_LEVEL_SYS;
		response.no_of_ep = 1;
		response.ep[0].no_of_intf = 1;
		response.ep[0].ep_id = requestdummy.ep_id;
		response.ep[0].generic_dc = requestdummy.generic_dc;
		response.ep[0].specific_dc = requestdummy.specific_dc;
		response.ep[0].intf[0].cmd_cls = requestdummy.cmd_cls;
		response.ep[0].intf[0].property_type = requestdummy.property_type;
		response_DM(response);
		memset(&requestdummy,0,sizeof(requestdummy));
		memset(&response, 0, sizeof(response));
		}
	}
}


/**
 hl_nw_node_cb - Callback function to notify node is added, deleted, or updated
 @param[in]	user	    The high-level api context
 @param[in]	noded	Node
 @param[in]	mode	    The node status
 @return
 */
static void hl_nw_node_cb(void *user, zwnoded_p noded, int mode) {
	zwepd_t ep;
	zwifd_t intf;
	int i = 0, j = 0;
	response_t response = { 0 };
	switch (mode) {
	case ZWNET_NODE_ADDED: {
		add_node_flag = noded->nodeid;
		syslog(LOG_DEBUG, "ZW: Node Added: %d", add_node_flag);

	}
		break;

	case ZWNET_NODE_REMOVED: {
		syslog(LOG_DEBUG, "ZW: Node Removed:%u", (unsigned) noded->nodeid);
		remove_node_flag = noded->nodeid;
	}
		break;

		case ZWNET_NODE_STATUS_ALIVE: {
		 syslog(LOG_DEBUG, "ZW: Node is Alive: %u", (unsigned) noded->nodeid);
		 response.operation = NODE_STATUS;
		 response.home_id = homeid;
		 response.node_id = noded->nodeid;
		 response.err_code = ZWNET_NODE_STATUS_ALIVE;
		 response.vid = noded->vid;
		 response.pid = noded->pid;
		 response.vtype = noded->type;
		 response.category = noded->category;
		 response_DM(response);
		 memset(&response,0, sizeof(response));
		 }
		 break;

	
	case ZWNET_NODE_STATUS_DOWN: {
		syslog(LOG_DEBUG, "ZW: Node Down: %u", (unsigned) noded->nodeid);
		response.operation = NODE_STATUS;
		response.home_id = homeid;
		response.node_id = noded->nodeid;
		response.err_code = ZWNET_NODE_STATUS_DOWN;
		response.vid = noded->vid;
		response.pid = noded->pid;
		response.vtype = noded->type;
		response.category = noded->category;
		response_DM(response);
		memset(&response, 0, sizeof(response));
	}
		break;

	case ZWNET_NODE_STATUS_SLEEP: {
		syslog(LOG_DEBUG, "ZW: Node Sleeping: %u", (unsigned) noded->nodeid);
		response.operation = NODE_STATUS;
		response.home_id = homeid;
		response.node_id = noded->nodeid;
		response.err_code = ZWNET_NODE_STATUS_SLEEP;
		response.vid = noded->vid;
		response.pid = noded->pid;
		response.vtype = noded->type;
		response.category = noded->category;
		response_DM(response);
		memset(&response, 0, sizeof(response));
	}
		break;
	}
}

/*Function will find the Command class is actuator OR sensor and return 1/0 respectively*/
int find_actuator_or_sensor(uint16_t cmd_cls) {
	switch (cmd_cls) {

	case COMMAND_CLASS_SWITCH_BINARY: /*0x25:*/
	case COMMAND_CLASS_SWITCH_MULTILEVEL: /*0x26:*/
	case COMMAND_CLASS_SWITCH_COLOR: /*0x33:*/
	case COMMAND_CLASS_DOOR_LOCK: /*0x62:*/
	case COMMAND_CLASS_BATTERY: /*0x80:*/
		return actuators; /*Treating as Actuator */

	case COMMAND_CLASS_ALARM: /*0x71:*/
	case COMMAND_CLASS_SENSOR_MULTILEVEL: /*0x31:*/
		return sensors; /*Treating as Sensor*/

	default:
		syslog(LOG_DEBUG, "ZW: Wrong CC");
		return wrong_cc;
	}

}

/*This function will skip the unwanted cms_cls*/
void filter_cc_for_event(void) {

	syslog(LOG_DEBUG, "ZW: filter_cc_for_event");
	char filter_CC[] = "25,26,33,31,62,80"; /*Command classes which does not have events*/
	char income_cc[5] = "";
	uint32_t ep_count = 0, intf_count = 0, event_count = 0;

	syslog(LOG_DEBUG, "ZW: End_points in  filter_cc_for_event : %d",
							discovery_struct.no_of_ep);	
	for (ep_count = 0; ep_count < discovery_struct.no_of_ep; ep_count++) {

		for (intf_count = 0;
				intf_count < discovery_struct.ep[ep_count].no_of_intf;
				intf_count++) {
			sprintf(income_cc, "%x",
					discovery_struct.ep[ep_count].intf[intf_count].cmd_cls);
			syslog(LOG_DEBUG, "ZW: event-income_cc: %s", income_cc);
			if (strstr(filter_CC, income_cc)) {
				for (event_count = 0; event_count < NO_EVENTS; event_count++) {
					discovery_struct.ep[ep_count].intf[intf_count].EVENT[event_count].no_of_events =
							0;
					syslog(LOG_DEBUG, "ZW: Events cleared for cmd_cls: %d",
							discovery_struct.ep[ep_count].intf[intf_count].cmd_cls);
				}
			} else {
				syslog(LOG_DEBUG, "ZW: event-cmd_cls %s is not filtered",
						income_cc);
			}
			memset(income_cc, 0, sizeof(income_cc));
		}
	}
	print_discovery_struct();

}

/*This function will clear types of particular cms_cls*/
void filter_cc_for_type(void) {
	char filter_CC[] = "25,26,33,62,80"; //Command classes which does not have types
	char income_cc[5] = "";
	uint32_t ep_count = 0, intf_count = 0, type_count = 0;

	for (ep_count = 0; ep_count < discovery_struct.no_of_ep; ep_count++) {

		for (intf_count = 0;
				intf_count < discovery_struct.ep[ep_count].no_of_intf;
				intf_count++) {
			sprintf(income_cc, "%x",
					discovery_struct.ep[ep_count].intf[intf_count].cmd_cls);
			syslog(LOG_DEBUG, "ZW: type-income_cc: %s", income_cc);
			if (strstr(filter_CC, income_cc)) {
				for (type_count = 0; type_count < NO_TYPES; type_count++) {
					discovery_struct.ep[ep_count].intf[intf_count].no_of_types =
							0;
					discovery_struct.ep[ep_count].intf[intf_count].EVENT[type_count].type =
							0;
					syslog(LOG_DEBUG, "ZW: types cleared for cmd_cls: %d",
							discovery_struct.ep[ep_count].intf[intf_count].cmd_cls);
				}
			} else
				syslog(LOG_DEBUG, "ZW: type-cmd_cls %s is not filtered",
						income_cc);

			memset(income_cc, 0, sizeof(income_cc));
		}
	}
	print_discovery_struct();
}

/*This function will clear types of particular cms_cls*/
void filter_cc_to_skip(void) {

	uint32_t ep_count = 0, intf_count = 0;
	char income_cc[5] = "";

	for (ep_count = 0; ep_count <discovery_struct.no_of_ep; ep_count++) {

		for (intf_count = 0;
				intf_count < discovery_struct.ep[ep_count].no_of_intf;
				intf_count++) {
			if ((discovery_struct.ep[ep_count].intf[intf_count].cmd_cls
					== COMMAND_CLASS_DOOR_LOCK)
					|| (discovery_struct.ep[ep_count].intf[intf_count].cmd_cls
							== COMMAND_CLASS_SWITCH_MULTILEVEL)) {
				syslog(LOG_DEBUG,
						"ZW: discovery_struct.ep[0].intf[j].cmd_cls: cmd_cls 0x62 and 0x38 found");
				for (intf_count = 0;
						intf_count < discovery_struct.ep[ep_count].no_of_intf;
						intf_count++) {
					if (discovery_struct.ep[ep_count].intf[intf_count].cmd_cls
							== COMMAND_CLASS_ALARM) {
						discovery_struct.ep[ep_count].intf[intf_count].no_of_types =
								0;

						discovery_struct.ep[ep_count].intf[intf_count].EVENT[0].no_of_events =
								0;
						syslog(LOG_DEBUG, "ZW: filter_cc_to_skip: %x",
								discovery_struct.ep[ep_count].intf[intf_count].cmd_cls);
					}
				}
			}
		}
	}
	print_discovery_struct();

}

/*This function Monitor the discovery structure filled OR Not*/
void discovery_monitor(void) {

	uint32_t intf_count = 0, ep_count = 0, type_count = 0, event_count = 0,
			find_return = 0;
	syslog(LOG_DEBUG, "ZW: DiscoveryMonitor:START");
	if (discovery_struct.node_id != 0xff && discovery_struct.no_of_ep != 0xff) {


		syslog(LOG_DEBUG, "ZW: DiscoveryMonitor check_discovery_flag %d",check_discovery_flag);
		if(check_discovery_flag==0)
		{

		syslog(LOG_DEBUG, "ZW: DiscoveryMonitor:START for END_POINT is %d",endpoint_count);
		for (ep_count = 0; ep_count < discovery_struct.no_of_ep; ep_count++) {
			if (discovery_struct.ep[ep_count].ep_id == 0xff) {
				syslog(LOG_DEBUG,
						"ZW: Null Endpoint, structure still not formed");
				return;
			}

			syslog(LOG_DEBUG, "ZW: no_of_intf:%d",
					discovery_struct.ep[ep_count].no_of_intf);

			for (intf_count = 0;
					intf_count < discovery_struct.ep[ep_count].no_of_intf;
					intf_count++) {
				if (discovery_struct.ep[ep_count].intf[intf_count].cmd_cls
						== 0xff) {
					syslog(LOG_DEBUG, "ZW: command class not updated for ep %d",
							discovery_struct.ep[ep_count].ep_id);
					return;
				}

				syslog(LOG_DEBUG, "ZW: no_of_types:%d @ep: %d @cmd_cls: %d",
						discovery_struct.ep[ep_count].intf[intf_count].no_of_types,
						discovery_struct.ep[ep_count].ep_id,
						discovery_struct.ep[ep_count].intf[intf_count].cmd_cls);

				find_return = find_actuator_or_sensor(
						discovery_struct.ep[ep_count].intf[intf_count].cmd_cls);
				if (find_return == 0) {

					syslog(LOG_DEBUG,
							"ZW: discovery_struct.ep[i].intf[intf_count].no_of_types %d",
							discovery_struct.ep[ep_count].intf[intf_count].no_of_types);

					for (type_count = 0;
							type_count
									< discovery_struct.ep[ep_count].intf[intf_count].no_of_types;
							type_count++) {

						syslog(LOG_DEBUG, "ZW: checking types for cmd_cls: %d",
								discovery_struct.ep[ep_count].intf[intf_count].cmd_cls);

						if (discovery_struct.ep[ep_count].intf[intf_count].EVENT[type_count].type
								!= 0xff) {
							syslog(LOG_DEBUG,
									"ZW: no_of_events:%d,ep: %d, interface: %d",
									discovery_struct.ep[ep_count].intf[intf_count].EVENT[type_count].no_of_events,
									ep_count, intf_count);

							for (event_count = 0;
									event_count
											< discovery_struct.ep[ep_count].intf[intf_count].EVENT[type_count].no_of_events;
									event_count++) {
								if (discovery_struct.ep[ep_count].intf[intf_count].EVENT[type_count].evt[event_count]
										== 0xff) {
									syslog(LOG_DEBUG,
											"ZW: Event is not update for type %d,@cmd_cls: %d i=%d,intf_count=%d,k=%d,event=%d",
											discovery_struct.ep[ep_count].intf[intf_count].EVENT[type_count].type,
											discovery_struct.ep[ep_count].intf[intf_count].cmd_cls,
											ep_count, intf_count, type_count,
											event_count);
									return;
								}
							}
						} else {
							syslog(LOG_DEBUG,
									"ZW: type is not updated for cmd_cls: %d",
									discovery_struct.ep[ep_count].intf[intf_count].cmd_cls);
							return;
						}

					}
				} else if (find_return == 1) {
					switch (discovery_struct.ep[ep_count].intf[intf_count].property_type) {

					case 2:
						for (char count_value = 0;
								count_value < NO_OF_COLOR_COMP; count_value++) {
							syslog(LOG_DEBUG, "ZW: case 2: arr[%d]: %d",
									count_value,
									discovery_struct.ep[ep_count].intf[intf_count].property_value.arr[count_value]);
							if (discovery_struct.ep[ep_count].intf[intf_count].property_value.arr[count_value]
									== 255) {
								syslog(LOG_DEBUG,
										"ZW: arr Null Interface property value, structure still not formed");
								return;
							}
						}
						break;

					case 3:
						syslog(LOG_DEBUG, "ZW: case 3: %d",
								discovery_struct.ep[ep_count].intf[intf_count].property_value.ch);

						if (discovery_struct.ep[ep_count].intf[intf_count].property_value.ch
								== 200) {
							syslog(LOG_DEBUG,
									"ZW: ch Null Interface property value, structure still not formed");
							return;
						}

						break;

					}
				} else {
					syslog(LOG_DEBUG, "ZW: wrong command class");
					return;
				}
			}
		}

		syslog(LOG_DEBUG, "ZW: Now we can update to DM:");
		response_DM(discovery_struct);

		syslog(LOG_DEBUG,
				"ZW: Device Discovery Completed and Sent it to Device Manager");

		/**
		 Clearing the request structure, discovery structure and discovery_node_id
		 Initially in the device discovery process, the discovery_node_id is filled
		 with some node id = 'X',and after completion of discovery process the variable should
		 be nullified for the next device discovery in order to handle the following case:
		 while discovering 2nd device if we get an observe response from 1st device.
		 */
		memset(&request, 0, sizeof(request));
		memset(&requestdummy, 0, sizeof(request));
		memset(&discovery_struct, 0, sizeof(discovery_struct));
		discovery_node_id = 0;
	}

	else
	{
			syslog(LOG_DEBUG, "ZW: DiscoveryMonitor:START for END_POINT is %d",endpoint_count);
		for (ep_count = 0; ep_count < endpoint_count; ep_count++) {
			if (discovery_struct.ep[ep_count].ep_id == 0xff) {
				syslog(LOG_DEBUG,
						"ZW: Null Endpoint, structure still not formed");
				return;
			}

			syslog(LOG_DEBUG, "ZW: no_of_intf:%d",
					discovery_struct.ep[ep_count].no_of_intf);

			for (intf_count = 0;
					intf_count < discovery_struct.ep[ep_count].no_of_intf;
					intf_count++) {
				if (discovery_struct.ep[ep_count].intf[intf_count].cmd_cls
						== 0xff) {
					syslog(LOG_DEBUG, "ZW: command class not updated for ep %d",
							discovery_struct.ep[ep_count].ep_id);
					return;
				}

				syslog(LOG_DEBUG, "ZW: no_of_types:%d @ep: %d @cmd_cls: %d",
						discovery_struct.ep[ep_count].intf[intf_count].no_of_types,
						discovery_struct.ep[ep_count].ep_id,
						discovery_struct.ep[ep_count].intf[intf_count].cmd_cls);

				find_return = find_actuator_or_sensor(
						discovery_struct.ep[ep_count].intf[intf_count].cmd_cls);
				if (find_return == 0) {

					syslog(LOG_DEBUG,
							"ZW: discovery_struct.ep[i].intf[intf_count].no_of_types %d",
							discovery_struct.ep[ep_count].intf[intf_count].no_of_types);

					for (type_count = 0;
							type_count
									< discovery_struct.ep[ep_count].intf[intf_count].no_of_types;
							type_count++) {

						syslog(LOG_DEBUG, "ZW: checking types for cmd_cls: %d",
								discovery_struct.ep[ep_count].intf[intf_count].cmd_cls);

						if (discovery_struct.ep[ep_count].intf[intf_count].EVENT[type_count].type
								!= 0xff) {
							syslog(LOG_DEBUG,
									"ZW: no_of_events:%d,ep: %d, interface: %d",
									discovery_struct.ep[ep_count].intf[intf_count].EVENT[type_count].no_of_events,
									ep_count, intf_count);

							for (event_count = 0;
									event_count
											< discovery_struct.ep[ep_count].intf[intf_count].EVENT[type_count].no_of_events;
									event_count++) {
								if (discovery_struct.ep[ep_count].intf[intf_count].EVENT[type_count].evt[event_count]
										== 0xff) {
									syslog(LOG_DEBUG,
											"ZW: Event is not update for type %d,@cmd_cls: %d i=%d,intf_count=%d,k=%d,event=%d",
											discovery_struct.ep[ep_count].intf[intf_count].EVENT[type_count].type,
											discovery_struct.ep[ep_count].intf[intf_count].cmd_cls,
											ep_count, intf_count, type_count,
											event_count);
									return;
								}
							}
						} else {
							syslog(LOG_DEBUG,
									"ZW: type is not updated for cmd_cls: %d",
									discovery_struct.ep[ep_count].intf[intf_count].cmd_cls);
							return;
						}

					}
				} else if (find_return == 1) {
					switch (discovery_struct.ep[ep_count].intf[intf_count].property_type) {

					case 2:
						for (char count_value = 0;
								count_value < NO_OF_COLOR_COMP; count_value++) {
							syslog(LOG_DEBUG, "ZW: case 2: arr[%d]: %d",
									count_value,
									discovery_struct.ep[ep_count].intf[intf_count].property_value.arr[count_value]);
							if (discovery_struct.ep[ep_count].intf[intf_count].property_value.arr[count_value]
									== 255) {
								syslog(LOG_DEBUG,
										"ZW: arr Null Interface property value, structure still not formed");
								return;
							}
						}
						break;

					case 3:
						syslog(LOG_DEBUG, "ZW: case 3: %d",
								discovery_struct.ep[ep_count].intf[intf_count].property_value.ch);

						if (discovery_struct.ep[ep_count].intf[intf_count].property_value.ch
								== 200) {
							syslog(LOG_DEBUG,
									"ZW: ch Null Interface property value, structure still not formed");
							return;
						}

						break;

					}
				} else {
					syslog(LOG_DEBUG, "ZW: wrong command class");
					return;
				}
			}
		}

		syslog(LOG_DEBUG, "ZW: Now we can update to DM:");
		response_DM(discovery_struct);

		syslog(LOG_DEBUG,
				"ZW: Device Discovery Completed and Sent it to Device Manager");

		/**
		 Clearing the request structure, discovery structure and discovery_node_id
		 Initially in the device discovery process, the discovery_node_id is filled
		 with some node id = 'X',and after completion of discovery process the variable should
		 be nullified for the next device discovery in order to handle the following case:
		 while discovering 2nd device if we get an observe response from 1st device.
		 */
		//memset(&request, 0, sizeof(request));
		//memset(&requestdummy, 0, sizeof(request));
		memset(&discovery_struct, 0, sizeof(discovery_struct));
		discovery_node_id = 0;
		endpoint_count=0;
	}
 } else
		syslog(LOG_DEBUG,
				"ZW: DiscoveryMonitor:node_id or no_of_ep not updated");

	syslog(LOG_DEBUG, "ZW: DiscoveryMonitor:END");

}

/*This function prints the global discovery structure during discovery process*/

void print_discovery_struct(void) {

	int ep_count, cc_count, type_count, event_count, count_value = 0;



	if(	discovery_struct.no_of_ep==0)
		return;

	syslog(LOG_DEBUG, "ZW:=============== START =====================");

	syslog(LOG_DEBUG, "ZW: msgid: %ld", discovery_struct.msgid);
	syslog(LOG_DEBUG, "ZW: Operation %d", discovery_struct.operation);
	syslog(LOG_DEBUG, "ZW: Homeid: %lu", discovery_struct.home_id);
	syslog(LOG_DEBUG, "ZW: Nodeid: %u", discovery_struct.node_id);
	syslog(LOG_DEBUG, "ZW: Vid: %u", discovery_struct.vid);
	syslog(LOG_DEBUG, "ZW: Pid : %u", discovery_struct.pid);
	syslog(LOG_DEBUG, "ZW: Ptype : %u", discovery_struct.vtype);
	syslog(LOG_DEBUG, "ZW: Device category : %u", discovery_struct.category);
	syslog(LOG_DEBUG, "ZW: Error code: %d", discovery_struct.err_code);
	syslog(LOG_DEBUG, "ZW: Number of end points: %d",
			discovery_struct.no_of_ep);

	
	
	for (ep_count = 0;
			ep_count < discovery_struct.no_of_ep && ep_count < NO_OF_ENDPOINTS;
			ep_count++) {
		syslog(LOG_DEBUG, "ZW: Epid : %d", discovery_struct.ep[ep_count].ep_id);
		syslog(LOG_DEBUG, "ZW: Generic device class : %d",
				discovery_struct.ep[ep_count].generic_dc);
		syslog(LOG_DEBUG, "ZW: Specific device class : %d",
				discovery_struct.ep[ep_count].specific_dc);
		syslog(LOG_DEBUG, "ZW: Number of interfaces : %d",
				discovery_struct.ep[ep_count].no_of_intf);

			if(	discovery_struct.ep[ep_count].no_of_intf==0 && discovery_struct.no_of_ep<2 )
				return;		

		/* Checking the details for each and Every Endpoint wheter it is having specific and Generic device class or not*/
		if(discovery_struct.ep[ep_count].specific_dc==0 && discovery_struct.ep[ep_count].generic_dc==0)
		{
			syslog(LOG_DEBUG, "ZW: SINCE THERE ARE NO DETAILS FOR PARTICULAR ENDPOINT");
			return;
		}

		
		for (cc_count = 0; cc_count < discovery_struct.ep[ep_count].no_of_intf;
				cc_count++) {
			syslog(LOG_DEBUG, "ZW: cls : %d",
					discovery_struct.ep[ep_count].intf[cc_count].cmd_cls);
			syslog(LOG_DEBUG, "ZW: no_of_types*---: %d",
					discovery_struct.ep[ep_count].intf[cc_count].no_of_types);
			for (type_count = 0; type_count < NO_TYPES; type_count++) {
				syslog(LOG_DEBUG, "ZW: TYPE is :%d",
						discovery_struct.ep[ep_count].intf[cc_count].EVENT[type_count].type);
				syslog(LOG_DEBUG, "ZW: no_of_events*---: %d",
						discovery_struct.ep[ep_count].intf[cc_count].EVENT[type_count].no_of_events);
				for (event_count = 0; event_count < NO_EVENTS; event_count++)
					syslog(LOG_DEBUG, "ZW: \tEVENT[%d].evt[%d]:%d", type_count,
							event_count,
							discovery_struct.ep[ep_count].intf[cc_count].EVENT[type_count].evt[event_count]);
			}

			syslog(LOG_DEBUG, "ZW: property type : %d",
					discovery_struct.ep[ep_count].intf[cc_count].property_type);

			if (discovery_struct.ep[ep_count].intf[cc_count].property_type
					== ARR_TYPE) {
				for (count_value = 0; count_value < NO_OF_COLOR_COMP;
						count_value++) {
					syslog(LOG_DEBUG, "ZW: property value arr %d:%d",
							count_value,
							discovery_struct.ep[ep_count].intf[cc_count].property_value.arr[count_value]);
				}
			} else if (discovery_struct.ep[ep_count].intf[cc_count].property_type
					== CHAR_TYPE) {
				syslog(LOG_DEBUG, "ZW: value ch %d",
						discovery_struct.ep[ep_count].intf[cc_count].property_value.ch);
			}

			else {
				syslog(LOG_DEBUG, "ZW: else:Invalid property type");
			}
		}
	}

	syslog(LOG_DEBUG, "ZW: ================== END ====================");
	discovery_monitor();

}

/**
 hl_nw_notify_cb - Callback function to notify the status of current operation
 @param[in]	user	    user context
 @param[in]	op		    network operation ZWNET_OP_XXX
 @param[in]	sts		    status of current operation
 @param[in]	info	    additional information for the specified op and sts; NULL if there is no additional info
 @return
 */
static void hl_nw_notify_cb(void *user, uint8_t op, uint16_t sts,
		zwnet_sts_t *info) {
	hl_appl_ctx_t *hl_appl = (hl_appl_ctx_t *) user;
	zwnetd_p net_desc;
	zwnoded_t noded;
	zwepd_t ep;
	int result = 0;
	int ep_count = 0, intf_count = 0;
	zwifd_t intf;
	response_t response = { 0 };
	//endpoint_count=0;
	zwnoded_p nod;
	zwnet_p net;
	/*Check whether the status is progress status of discovering each detailed node information*/
	if (sts & OP_GET_NI_TOTAL_NODE_MASK) {
		uint16_t total_nodes;
		uint16_t cmplt_nodes;
		total_nodes = (sts & OP_GET_NI_TOTAL_NODE_MASK) >> 8;
		cmplt_nodes = sts & OP_GET_NI_NODE_CMPLT_MASK;
		syslog(LOG_DEBUG, "ZW: Get node info %u/%u completed ", cmplt_nodes,
				total_nodes);
		return;
	}

	switch (op) {
	case ZWNET_OP_INITIALIZE:
		syslog(LOG_DEBUG, "ZW: Initialization status:%u ", (unsigned) sts);
		if (sts == OP_DONE) {
			hl_appl->init_status = 1;
			net_desc = zwnet_get_desc(hl_appl->zwnet);
			syslog(LOG_DEBUG, "ZW: Z-Wave role: %d ", net_desc->ctl_zw_role);
			/*homeid = net_desc->id;*/
			if(net_desc->ctl_zw_role==2)
			{
			check_discovery_flag=1;
			syslog(LOG_DEBUG, "ZW:ZWNET_OP_INITIALIZE check_discovery_flag1: %u ",check_discovery_flag);
			}

			homeid = net_desc->id;
			response.home_id = net_desc->id;
			syslog(LOG_DEBUG, "ZW: Home id : %x ", response.home_id);
			syslog(LOG_DEBUG, "ZW: ZWNET OP INITIALIZE");
			syslog(LOG_DEBUG,
					"ZW: Network initialized!  Setting up unsolicited address, please wait ...");

			if (hl_unsolicited_addr_setup(hl_appl) == 0) {
				hl_appl->init_status = 2;

			}
		} else {
			syslog(LOG_DEBUG, "ZW: Initialization fail .... ");

			response.err_code = sts;
			response.operation = NW_INIT;
			hl_appl->init_status = sts;
			response_DM(response);
			//bzero(&response, sizeof(response));
			memset(&response, 0, sizeof(response));

			/* zwnet_exit(hl_appl->zwnet);*/
		}
		break;

	case ZWNET_OP_ADD_NODE:
		syslog(LOG_DEBUG, "ZW: Add Node status: %u ", (unsigned) sts);
		if (sts == OP_DONE) { //Clear add node DSK callback control & status
			hl_appl->sec2_cb_enter = 0;
			hl_appl->sec2_cb_exit = 1;
			hl_appl->add_status = ADD_NODE_STS_DONE;
			result = zwnet_get_node_by_id(hl_appl->zwnet, add_node_flag,
					&noded);
			syslog(LOG_DEBUG, "ZW: Get node by id function return value: %d:",
					result);
			if (result == 0) {
				/** 
				 We are assigning the discovery_struct with OXFF because
				 in our zwave-application the value is zero in some cases
				 example bulb(brightness 0-99) and plug(0-1) so the discovery_struct
				 member values should not match with commnad class,generic and specific class etc.
				 Assigninig the property value as 200 because the property value might be 255
				 */
				memset(&response, 0xff, sizeof(response));
				for (char i = 0; i < NO_OF_ENDPOINTS; i++) {
					for (char j = 0; j < NO_OF_INTERFACES; j++) {
						response.ep[i].intf[j].property_value.ch = 200;
					}
				}
				discovery_node_id = noded.nodeid;
				syslog(LOG_DEBUG, "ZW: Node Added: %u",
						(unsigned) noded.nodeid);
				syslog(LOG_DEBUG, "ZW: Node Secure: %u",
						(unsigned) noded.propty);
				syslog(LOG_DEBUG, "ZW: The security operation is: %d",
						requestdummy.operation);

				if (request.operation == S2_SECURITY
						&& noded.propty == NODE_SECURE) {
					response.msgid = requestdummy.msgid;
					response.operation = S2_SECURITY;
					response.home_id = 0;
					response.node_id = 0;
					response.vid = 0;
					response.pid = 0;
					response.vtype = 0;
					response.category = 0;
					syslog(LOG_DEBUG,
							"ZW: The S2_SECURITY response operation is; %d",
							response.operation);
					response_DM(response);
				} else if ((request.operation == S2_SECURITY
						|| requestdummy.operation == COMMISSIONING)
						&& noded.propty == NODE_INSECURE) {
					response.msgid = requestdummy.msgid;
					response.operation = S2_SECURITY;
					response.home_id = 0;
					response.node_id = 0;
					response.vid = 0;
					response.pid = 0;
					response.vtype = 0;
					response.category = 0;
					response.err_code = 2;
					syslog(LOG_DEBUG, "ZW: The NODE_INSECURE operation is: %d",
							response.operation);
					response_DM(response);
					sleep(1); // To maintain time gap b/w two responses moving forwared will remove this.
				}

				syslog(LOG_DEBUG, "ZW: Vendor Id is : %u",
						(unsigned) noded.vid);
				syslog(LOG_DEBUG, "ZW: Product Id : %u", (unsigned) noded.pid);
				syslog(LOG_DEBUG, "ZW: Product Type : %u",
						(unsigned) noded.type);
				syslog(LOG_DEBUG, "ZW: Device Category is: %u",
						(unsigned) noded.category);

				response.operation = COMMISSIONING;
				response.home_id = homeid;
				response.msgid = requestdummy.msgid;
				response.node_id = noded.nodeid;
				response.vid = noded.vid;
				response.pid = noded.pid;
				response.vtype = noded.type;
				response.category = noded.category;
				response.err_code = ZWNET_NODE_ADDED;
				if (!zwnode_get_ep(&noded, &ep)) {
					do {
						response.ep[ep_count].ep_id = ep.epid;
						response.ep[ep_count].generic_dc = ep.generic;
						response.ep[ep_count].specific_dc = ep.specific;
						intf_count = 0;
						if (!zwep_get_if(&ep, &intf)) {
							do {
								if ((intf.cls == COMMAND_CLASS_SENSOR_MULTILEVEL)
										|| (intf.cls
												== COMMAND_CLASS_SWITCH_COLOR)
										|| (intf.cls == COMMAND_CLASS_ALARM)
										|| (intf.cls == COMMAND_CLASS_BATTERY)
										|| (intf.cls
												== COMMAND_CLASS_SWITCH_BINARY)
										|| (intf.cls
												== COMMAND_CLASS_SWITCH_MULTILEVEL)
										|| (intf.cls == COMMAND_CLASS_DOOR_LOCK)) {
									response.ep[ep_count].intf[intf_count].cmd_cls =
											intf.cls;
									switch (intf.cls) {

									case COMMAND_CLASS_SWITCH_COLOR:
										response.ep[ep_count].intf[intf_count].property_type =
												ARR_TYPE;
										break;

									case COMMAND_CLASS_ALARM:
									case COMMAND_CLASS_BATTERY:
									case COMMAND_CLASS_DOOR_LOCK:
									case COMMAND_CLASS_SWITCH_MULTILEVEL:
									case COMMAND_CLASS_SWITCH_BINARY:
									case COMMAND_CLASS_SENSOR_MULTILEVEL:
										response.ep[ep_count].intf[intf_count].property_type =
												CHAR_TYPE;
										break;
									default:
										syslog(LOG_DEBUG,
												"ZW: Inavalid Commnad classes");
										break;
									}
									sleep(1);
									get_value(&intf);
									intf_count++;
								}
							} while (!zwif_get_next(&intf, &intf));
						}
						response.ep[ep_count].no_of_intf = intf_count;
						ep_count++;
					} while (!zwep_get_next(&ep, &ep));
				}
					response.no_of_ep = ep_count;
					syslog(LOG_DEBUG, "ZW:  Node added");
					memcpy(&discovery_struct, &response, sizeof(response));

					filter_cc_for_event();
					filter_cc_for_type();
					filter_cc_to_skip();
					print_discovery_struct();
					sleep(1);
			} else {
				syslog(LOG_ERR, "ZW: zwnet_get_node_by_id failed: %d", result);
				response.err_code = result;
				response.msgid = request.msgid;
				response_DM(response);
			}
		} else if (sts == OP_FAILED) { //Clear add node DSK callback control & status
			hl_appl->sec2_cb_enter = 0;
			hl_appl->sec2_cb_exit = 1;
			hl_appl->add_status = ADD_NODE_STS_UNKNOWN;

			response.operation = COMMISSIONING;
			response.home_id = homeid;
			response.msgid = requestdummy.msgid;
			response.err_code = sts;

			response_DM(response);
			memset(&response, 0, sizeof(response));
		}

		break;

	case ZWNET_OP_RM_NODE:
		syslog(LOG_DEBUG, "ZW: Remove Node Status: %u", (unsigned) sts);
		if (sts == OP_DONE) {
			hl_appl->rm_status = RM_NODE_STS_DONE;

			response.err_code = OP_SUCCESS;
			response.operation = request.operation;
			response.home_id = homeid;
			response.node_id = remove_node_flag;
			response.msgid = requestdummy.msgid;

			response_DM(response);
			memset(&response, 0, sizeof(response));
			memset(&request, 0, sizeof(request));
		} else if (sts == OP_FAILED) {
			hl_appl->rm_status = RM_NODE_STS_UNKNOWN;
			response.err_code = sts;
			response.operation = request.operation;
			response.home_id = homeid;
			response.node_id = requestdummy.node_id;
			response.msgid = requestdummy.msgid;

			response_DM(response);
			memset(&response, 0, sizeof(response));
			memset(&request, 0, sizeof(request));
		}
		if (hl_appl->rm_status != RM_NODE_STS_PROGRESS) {
		}
		break;

	case ZWNET_OP_RESET:
		syslog(LOG_DEBUG, "ZW: Reset Status: %u", (unsigned) sts);
		if (sts == OP_DONE) {
			hl_appl->rst_status = RESET_NW_STS_DONE;
			response.err_code = RESET_NW_STS_DONE;
			response.operation = RESET;
			response.home_id = homeid;
			response.msgid = requestdummy.msgid;

			response_DM(response);
			memset(&response, 0, sizeof(response));
		} else if (sts == OP_FAILED) {
			hl_appl->rst_status = RESET_NW_STS_UNKNOWN;
			response.err_code = sts;
			response.operation = RESET;
			response.home_id = homeid;
			response.msgid = requestdummy.msgid;

			response_DM(response);
			memset(&response, 0, sizeof(response));
		}

		if (hl_appl->rst_status != RESET_NW_STS_PROGRESS) {
			syslog(LOG_DEBUG, "ZW: Press 'x' to exit ...");
		}
		break;

	case ZWNET_OP_INITIATE:
		syslog(LOG_DEBUG, "ZW: ZWNET_OP_INITIATE: %u", (unsigned) sts);

		if(sts==0)
		{
			requestdummy.operation=1;
		
			update_network();
		}

	case ZWNET_OP_UPDATE:
			syslog(LOG_DEBUG, "ZW: ZWNET_OP_UPDATE: %u", (unsigned) sts);

	/*		if(sts==0)
		{
			requestdummy.operation=15;
		
			update_network();
		}*/


	
	
		break;

	

	default:
		syslog(LOG_DEBUG, "ZW: hl_nw_notify_cb op: %u, status: %u",
				(unsigned) op, (unsigned) sts);
		net_desc = zwnet_get_desc(hl_appl->zwnet);
			syslog(LOG_DEBUG, "ZW: Z-Wave role: %d ", net_desc->ctl_zw_role);
		if(net_desc->ctl_zw_role==2)
		{
			check_discovery_flag=1;
			syslog(LOG_DEBUG, "ZW:ZWNET_OP_INITIALIZE check_discovery_flag2: %u ",check_discovery_flag);

		}
	}
}


/**
 lib_init - Initialize library
 @param[in]	hl_appl		        The high-level api context
 @param[in]	host_port		    Host listening port
 @param[in]	zip_router_ip		Z/IP router IP address in numerical form
 @param[in]	use_ipv4		    Flag to indicate zip_router_ip is in IPv4 or IPv6 format. 1= IPv4; 0= IPv6
 @param[in]	dev_cfg_file_name	Device specific configuration database file name
 @param[in]	dtls_psk		    DTLS pre-shared key
 @param[in]	dtls_psk_len		DTLS pre-shared key length (in bytes)
 @param[in]	pref_dir		    Full path of directory for storing network/user preference files
 @return  0 on success, negative error number on failure
 */
int lib_init(hl_appl_ctx_t *hl_appl, uint16_t host_port, uint8_t *zip_router_ip,
		int use_ipv4, char *dev_cfg_file_name, uint8_t *dtls_psk,
		uint8_t dtls_psk_len, char *pref_dir) {
	int result;
	zwnet_init_t zw_init = { 0 };

	zw_init.user = hl_appl; //high-level application context
	zw_init.node = hl_nw_node_cb;
	zw_init.notify = hl_nw_notify_cb;
	zw_init.appl_tx = hl_nw_tx_cb;
	zw_init.pref_dir = pref_dir;
	zw_init.print_txt_fn = NULL;
	//zw_init.net_info_dir = NULL;
	zw_init.net_info_dir = "/root/zwave/";
	zw_init.host_port = host_port;
	zw_init.use_ipv4 = use_ipv4;
	memcpy(zw_init.zip_router, zip_router_ip,
			(use_ipv4) ? IPV4_ADDR_LEN : IPV6_ADDR_LEN);
	zw_init.dev_cfg_file = dev_cfg_file_name;
	zw_init.dev_cfg_usr = NULL;
	zw_init.dtls_psk_len = dtls_psk_len;
	if (dtls_psk_len) {
		memcpy(zw_init.dtls_psk, dtls_psk, dtls_psk_len);
	}
	/*Unhandled command handler*/
	zw_init.unhandled_cmd = NULL;

	/*Init ZW network*/
	result = zwnet_init(&zw_init, &hl_appl->zwnet);

	if (result != 0) {
		syslog(LOG_ERR, "ZW: zwnet_init with error: %d", result);

		/*Display device configuration file error*/
		if (zw_init.err_loc.dev_ent) {
			syslog(LOG_ERR, "ZW: Parsing device configuration file error loc:");
			syslog(LOG_DEBUG, "ZW: Device entry number: %u",
					zw_init.err_loc.dev_ent);
			if (zw_init.err_loc.ep_ent) {
				syslog(LOG_DEBUG, "ZW: Endpoint entry number: %u",
						zw_init.err_loc.ep_ent);
			}
			if (zw_init.err_loc.if_ent) {
				syslog(LOG_DEBUG, "ZW: Interface entry number: %u",
						zw_init.err_loc.if_ent);
			}
		}
		return result;
	}
	return 0;
}

/**
 nw_init - initialization network
 @param[in] hl_appl   user application context
 @return  0 on success; otherwise negative number
 */
int nw_init(hl_appl_ctx_t *hl_appl) {

	int ret;
	response_t response = { 0 };
	char zip_gw_addr_str[LEN_GW_ADDR_STR];
	strcpy(zip_gw_addr_str, addr_str);
	uint16_t host_port = HOST_PORT; ///< Host listening port
	char psk_str[LEN_PSK_STR] = "123456789012345678901234567890AA";
	int psk_len;
	uint8_t dtls_psk[MAX_DTLS_PSK / 2]; ///< DTLS pre-shared key
	uint8_t zip_gw_ip[ZIP_GW_IP];      ///< Z/IP gateway address in IPv4 or IPv6

	/*  get_fw_env("zip_gw_addr", zip_gw_addr_str);
	 syslog(LOG_DEBUG, "ZW:Zipgateway address string is: %s \n", zip_gw_addr_str);*/

	/*Check DTLS pre-shared key validity*/
	psk_len = strlen(psk_str);

	if (psk_len > 0) {
		if (psk_len > MAX_DTLS_PSK) {
			syslog(LOG_ERR, "ZW: PSK string length is too long");
			response.err_code = ZW_ERR_VALUE;
			response.operation = NW_INIT;
			response_DM(response);
			memset(&response, 0, sizeof(response));
			return ZW_ERR_VALUE;
		}
		if (psk_len % 2) {
			syslog(LOG_ERR, "ZW: PSK string length should be even");
			response.err_code = ZW_ERR_VALUE;
			response.operation = NW_INIT;
			response_DM(response);
			memset(&response, 0, sizeof(response));
			return ZW_ERR_VALUE;
		}
		/*Convert ASCII hexstring to binary string*/
		ret = hexstring_to_bin(psk_str, psk_len, dtls_psk);
		if (ret != 0) {
			syslog(LOG_ERR, "ZW: PSK string is not hex string");
			response.err_code = ZW_ERR_VALUE;
			response.operation = NW_INIT;
			response_DM(response);
			memset(&response, 0, sizeof(response));
			return ZW_ERR_VALUE;
		}
	}

	/*Convert IPv4 / IPv6 address string to numeric equivalent*/
	ret = zwnet_ip_aton(zip_gw_addr_str, hl_appl->zip_gw_ip,
			&hl_appl->use_ipv4);

	if (ret != 0) {
		syslog(LOG_ERR, "ZW: Invalid Z/IP router IP address: %s",
				zip_gw_addr_str);
		response.err_code = ret;
		response.operation = NW_INIT;
		response_DM(response);
		memset(&response, 0, sizeof(response));
		return ZW_ERR_IP_ADDR;
	}

	/*Initialize library*/
	ret = lib_init(hl_appl, host_port, hl_appl->zip_gw_ip, hl_appl->use_ipv4,
			"zwave_device_rec.txt" /*device info database file*/, dtls_psk,
			psk_len / 2, "/root/zwave");

	if (ret < 0) {
		syslog(LOG_ERR, "ZW: lib_init with error: %d", ret);
		response.err_code = ret;
		response.operation = NW_INIT;
		response_DM(response);
		memset(&response, 0, sizeof(response));
	}
	return ret;
}

/**
 gw_discvr_cb - gateway discovery callback
 @param[in]	gw_addr	    Gateway addresses
 @param[in]	gw_addr_cnt Number of gateway addresses returned in gw_addr
 @param[in]	ipv4        Flag to indicate the addr parameter is IPv4 or IPv6. 1=IPv4; 0=IPv6
 @param[in]	usr_param   User defined parameter used in callback function
 @param[in]	rpt_num     Report number that this callback is delivering the gateway addresses report; start from 1
 @param[in]	total_rpt   Total reports that will be delivered by callbacks. Each callback delivers one report.
 @param[in]	gw_name	    Gateway names corresponding to the gw_ip.  If NULL, it means gateway name information is unavailable.
 */
void gw_discvr_cb(uint8_t *gw_addr, uint8_t gw_addr_cnt, int ipv4,
		void *usr_param, int rpt_num, int total_rpt, char **gw_name) {
	hl_appl_ctx_t *hl_appl = (hl_appl_ctx_t *) usr_param;
	uint8_t *tmp_buf;
	char **tmp_gw_name;
	int gw_addr_buf_sz;
	int i;
	int result;
	int one_ip_addr_len; //number of bytes required for storing one IP address. IPv4=4, IPv6=16

	syslog(LOG_DEBUG, "ZW: Received report:%d/%d with gw count:%u", rpt_num,
			total_rpt, gw_addr_cnt);

	if (total_rpt == 0) {
		syslog(LOG_ERR, "ZW: The system has no valid IP, please configure it");
		return;
	}

	one_ip_addr_len = (ipv4) ? 4 : 16;

	if (gw_addr_cnt > 0) {
		/*Calculate IP address buffer size for storing new found gateways from this report*/
		gw_addr_buf_sz = (gw_addr_cnt * one_ip_addr_len);

		if (hl_appl->gw_addr_buf) { /*Expand buffer to store new found gateways from this report*/
			tmp_buf = realloc(hl_appl->gw_addr_buf,
					(hl_appl->gw_addr_cnt * one_ip_addr_len) + gw_addr_buf_sz);
		} else { /*Allocate buffer to store new found gateways*/
			tmp_buf = malloc(gw_addr_buf_sz);
		}

		if (gw_name) { /*Gateway names are available*/
			if (hl_appl->gw_name) { /*Expand buffer to store new found gateways from this report*/
				tmp_gw_name = (char **) realloc(hl_appl->gw_name,
						(hl_appl->gw_addr_cnt + gw_addr_cnt) * sizeof(char *));
			} else { /*Allocate buffer to store new found gateways*/
				tmp_gw_name = (char **) malloc(gw_addr_cnt * sizeof(char *));
			}
			if (tmp_gw_name) { /*Save gateway names*/
				hl_appl->gw_name = tmp_gw_name;
				for (i = 0; i < gw_addr_cnt; i++) {
					tmp_gw_name[hl_appl->gw_addr_cnt + i] = strdup(gw_name[i]);
				}
			}
		}

		if (tmp_buf) { /*Save gateway IP addresses*/
			hl_appl->gw_addr_buf = tmp_buf;
			memcpy(
					hl_appl->gw_addr_buf
							+ (hl_appl->gw_addr_cnt * one_ip_addr_len), gw_addr,
					gw_addr_buf_sz);
			hl_appl->gw_addr_cnt += gw_addr_cnt;
		}
	}

	if (hl_appl->gw_addr_cnt > 0) {
		find_zipgateway = 1;
		/*Display gateway ip addresses*/
		syslog(LOG_DEBUG, "ZW: ---Gateways found---");
		for (i = 0; i < hl_appl->gw_addr_cnt; i++) {
			result = zwnet_ip_ntoa(hl_appl->gw_addr_buf + (i * one_ip_addr_len),
					addr_str, 80, ipv4);
			if (result == 0) {
				syslog(LOG_DEBUG,
						"ZW: Discoverd gateway (%d), address (%s), name is (%s)",
						i, addr_str,
						(hl_appl->gw_name) ? hl_appl->gw_name[i] : "unknown");
			}
		}
	}
}

static void hl_get_dsk_cb(void *usr_ctx, char *dsk) {
	syslog(LOG_DEBUG, "ZW: In initate_network");
	response_t response = { 0 };
	response.operation = request.operation;
	int dsk_key = 0;

	syslog(LOG_DEBUG, "ZW: The DSK key is  %s", dsk);
	strncpy(response.Key,dsk,5);
	response.Key[5]='\0';

	syslog(LOG_DEBUG, "ZW: The DSK key is  %s", response.Key);
	response.operation = request.operation;
	response.msgid = request.msgid;
	response_DM(response);
	memset(&request, 0, sizeof(request));
}

void initate_network() {
	void *usr_ctx;
	int result;
	response_t response = { 0 };
	syslog(LOG_DEBUG, "ZW: In initate_network");

	result = zwnet_initiate(appl_ctx.zwnet, hl_get_dsk_cb, &usr_ctx);
	if (result != 0) {
		syslog(LOG_ERR, "ZW: zwnet_initiate return error: %d", result);
		response.operation = request.operation;
		response.msgid = request.msgid;
		response.err_code = result;
		response_DM(response);
		memset(&request, 0, sizeof(request));
	}
}

void update_network(){
	syslog(LOG_DEBUG, "ZW: In UPADTE_NETWORK");
	int result;
	response_t response = { 0 };
	zwnoded_t noded;
	zwepd_t ep;
	zwifd_t intf;
	//int ep_count = 0, intf_count = 0;
	result=zwnet_update(appl_ctx.zwnet);
	if (result != 0) {
		syslog(LOG_ERR, "ZW: zwnet_update return error: %d", result);
		response.operation = request.operation;
		response.msgid = request.msgid;
		response.err_code = result;
		response_DM(response);
		memset(&request, 0, sizeof(request));
	}
		result = zwnet_get_node(appl_ctx.zwnet, &noded);
			if (result == 0)
			{
			response.node_id = noded.nodeid;
			result = zwnet_get_node_by_id(appl_ctx.zwnet, noded.nodeid,
					&noded);
			}

			while (zwnode_get_next(&noded, &noded)== 0)
			{

				int ep_count = 0, intf_count = 0;
				endpoint_count=0;

				syslog(LOG_DEBUG, "ZW:ep_count123  %d intf_count %d",ep_count,intf_count);

						if (result == 0) {
				/** 
				 We are assigning the discovery_struct with OXFF because
				 in our zwave-application the value is zero in some cases
				 example bulb(brightness 0-99) and plug(0-1) so the discovery_struct
				 member values should not match with commnad class,generic and specific class etc.
				 Assigninig the property value as 200 because the property value might be 255
				 */
				memset(&response, 0xff, sizeof(response));
				for (char i = 0; i < NO_OF_ENDPOINTS; i++) {
					for (char j = 0; j < NO_OF_INTERFACES; j++) {
						response.ep[i].intf[j].property_value.ch = 200;	
					}
				}
				discovery_node_id = noded.nodeid;
				syslog(LOG_DEBUG, "ZW: Node Added: %u",
						(unsigned) noded.nodeid);
				syslog(LOG_DEBUG, "ZW: Node Secure: %u",
						(unsigned) noded.propty);
				syslog(LOG_DEBUG, "ZW: The security operation is: %d",
						requestdummy.operation);

				syslog(LOG_DEBUG, "ZW: Vendor Id is : %u",
						(unsigned) noded.vid);
				syslog(LOG_DEBUG, "ZW: Product Id : %u", (unsigned) noded.pid);
				syslog(LOG_DEBUG, "ZW: Product Type : %u",
						(unsigned) noded.type);
				syslog(LOG_DEBUG, "ZW: Device Category is: %u",
						(unsigned) noded.category);
					
						//response.no_of_ep = ep_count;
						response.operation = COMMISSIONING;
						response.home_id = homeid;
						response.msgid = 123;
						response.node_id = noded.nodeid;
						response.vid = noded.vid;
						response.pid = noded.pid;
						response.vtype = noded.type;
						response.category = noded.category;
						response.err_code = ZWNET_NODE_ADDED;



					if (!zwnode_get_ep(&noded, &ep)) {
					do {
						
						//response.ep[ep_count].no_of_intf = intf_count;
						endpoint_count++;
						//syslog(LOG_DEBUG, "ZW: END_POINT IN Do-while %d",endpoint_count);
							if (!zwep_get_if(&ep, &intf)) {

							do {
							
							} while (!zwif_get_next(&intf, &intf));
						}

					syslog(LOG_DEBUG, "ZW: END_POINT IN Do-while %d",endpoint_count);
				
						
				} while (!zwep_get_next(&ep, &ep));
					
				}

				if (!zwnode_get_ep(&noded, &ep)) {
					do {
						syslog(LOG_DEBUG, "ZW: Device ENDPOINT ID is: %u",
						(unsigned) ep.epid);

						syslog(LOG_DEBUG, "ZW: END ID is : %u",
						(unsigned) ep.epid);
						syslog(LOG_DEBUG, "ZW: GENERIC ID is : %u",
						(unsigned) ep.generic);
						syslog(LOG_DEBUG, "ZW: SPECIFIC is : %u",
						(unsigned) ep.specific);
						response.ep[ep_count].ep_id = ep.epid;
						response.ep[ep_count].generic_dc = ep.generic;
						response.ep[ep_count].specific_dc = ep.specific;
						ep_count++;
						response.no_of_ep = ep_count;
						intf_count = 0;
						
						if (!zwep_get_if(&ep, &intf)) {

							do {
							
								if ((intf.cls == COMMAND_CLASS_SENSOR_MULTILEVEL)
										|| (intf.cls
												== COMMAND_CLASS_SWITCH_COLOR)
										|| (intf.cls == COMMAND_CLASS_ALARM)
										|| (intf.cls == COMMAND_CLASS_BATTERY)
										|| (intf.cls
												== COMMAND_CLASS_SWITCH_BINARY)
										|| (intf.cls
												== COMMAND_CLASS_SWITCH_MULTILEVEL)
										|| (intf.cls == COMMAND_CLASS_DOOR_LOCK)) {

											
									syslog(LOG_DEBUG, "ZW:ep_count  %d intf_count %d",ep_count,intf_count
												);
									response.ep[ep_count-1].intf[intf_count].cmd_cls =
											intf.cls;
									switch (intf.cls) {


									case COMMAND_CLASS_SWITCH_COLOR:
										response.ep[ep_count-1].intf[intf_count].property_type =
												ARR_TYPE;
										break;

									case COMMAND_CLASS_ALARM:
									case COMMAND_CLASS_BATTERY:
									case COMMAND_CLASS_DOOR_LOCK:
									case COMMAND_CLASS_SWITCH_MULTILEVEL:
									case COMMAND_CLASS_SWITCH_BINARY:
									case COMMAND_CLASS_SENSOR_MULTILEVEL:
										response.ep[ep_count-1].intf[intf_count].property_type =
												CHAR_TYPE;
										break;
									default:
										syslog(LOG_DEBUG,
												"ZW: Inavalid Commnad classes");
										break;
											
									}
									sleep(1);
									intf_count++;
									response.ep[ep_count-1].no_of_intf = intf_count;
									memcpy(&discovery_struct, &response, sizeof(response));
									
									
								}
								
							} while (!zwif_get_next(&intf, &intf));

								
						}

						syslog(LOG_DEBUG, "ZW: Interface_count %d",intf_count);


						if(ep_count==1 && intf_count==0)
						{
							response.no_of_ep = ep_count;
							response.ep[ep_count-1].no_of_intf = intf_count;
							memcpy(&discovery_struct, &response, sizeof(response));
						}

						else if(endpoint_count==5 && intf_count==0)
						{
							response.no_of_ep = ep_count;
							response.ep[ep_count-1].no_of_intf = intf_count;
							memcpy(&discovery_struct, &response, sizeof(response));
						}

					
					filter_cc_for_event();
					syslog(LOG_DEBUG, "ZW: AFTER FILTER filter_cc_for_event");
					filter_cc_for_type();
					syslog(LOG_DEBUG, "ZW: AFTER FILTER filter_cc_for_type");
					filter_cc_to_skip();
					syslog(LOG_DEBUG, "ZW:AFTER FILTER filter_cc_to_skip ");
					print_discovery_struct();

				} while (!zwep_get_next(&ep, &ep));
					
			}
					if (!zwnode_get_ep(&noded, &ep))
					{
						do
						{
							if(!zwep_get_if(&ep, &intf))
							{
								do
								{
									if ((intf.cls == COMMAND_CLASS_SENSOR_MULTILEVEL)
										|| (intf.cls
												== COMMAND_CLASS_SWITCH_COLOR)
										|| (intf.cls == COMMAND_CLASS_ALARM)
										|| (intf.cls == COMMAND_CLASS_BATTERY)
										|| (intf.cls
												== COMMAND_CLASS_SWITCH_BINARY)
										|| (intf.cls
												== COMMAND_CLASS_SWITCH_MULTILEVEL)
										|| (intf.cls == COMMAND_CLASS_DOOR_LOCK))
										{
											syslog(LOG_DEBUG,
												"ZW:  Commnad classes");
											get_value(&intf);
											sleep(1);
										}

								}while (!zwif_get_next(&intf, &intf));
							}

						}while (!zwep_get_next(&ep, &ep));
					}

				
					syslog(LOG_DEBUG, "ZW: NUMBER OF END points %d",response.no_of_ep);
					
					
					
					sleep(1);
			
			}
			 else {
				syslog(LOG_ERR, "ZW: zwnet_get_node_by_id failed: %d", result);
				response.err_code = result;
				response.msgid = request.msgid;
				response_DM(response);
			}

			}	
}
			

/**
 show_usage - show program usage
 */
void show_usage(void) {
	syslog(LOG_DEBUG, "ZW: usage: gw_discovery <ip option>");
	syslog(LOG_DEBUG,
			"ZW: ip option: -4 for using IPv4; -6 for using IPv6; default to IPv6 without option given");
}

int main(int argc, char **argv) {

	char zip_gw_addr_str[LEN_GW_ADDR_STR];
	void *gw_discvr_ctx;
	int result;
	int msgq_ret;
	int choice;
	int count_value;
	response_t response = { 0 };
	check_discovery_flag=0;
	int i;
	int one_ip_addr_len; /*number of bytes required for storing one IP address. IPv4=4, IPv6=16*/

	find_zipgateway = 0;

	/*Initialization of message queue to communication b/w zwave and device manager*/
	msgq_ret = dm_zw_comm_create();
	if (msgq_ret == 0) {
		syslog(LOG_DEBUG,
				"ZW: communication b/w dm-zw created succesfully via message queue");
	} else if (msgq_ret == -1) {
		/* TODO : Inform to Service Manager */
		syslog(LOG_CRIT, "ZW: communication b/w dm-zw created Fail");
		exit(0);
	}

	/*Parsing program arguments*/
	if (argc == 1) { /*Default to IPv4*/
		appl_ctx.use_ipv4 = 1;
	} else if (argc == 2) {
		if ((strcmp(argv[1], "-4") == 0) || (strcmp(argv[1], "-6") == 0)) {

			appl_ctx.use_ipv4 = (strcmp(argv[1], "-6") == 0) ? 0 : 1;
		} else {
			show_usage();
			return -1;
		}
	} else {
		show_usage();
		return -1;
	}

	syslog(LOG_DEBUG, "ZW: Scanning for Z/IP gateway ...");
	int count;
	do {
		/*Start gateway discovery*/
		count = 5;
		gw_discvr_ctx = zwnet_gw_discvr_start(gw_discvr_cb, &appl_ctx,
				appl_ctx.use_ipv4, 1 /*use mDNS for discovery*/);

		if (!gw_discvr_ctx) {
			return -1;
		}
		do {
			sleep(1);
			count--;
		} while (count > 0 && find_zipgateway == 0);

		/*Stop gateway discovery*/
		zwnet_gw_discvr_stop(gw_discvr_ctx);

		syslog(LOG_DEBUG, "ZW: Number of gateways found: %d",
				appl_ctx.gw_addr_cnt);
		if (appl_ctx.gw_addr_cnt > 0) {
			syslog(LOG_DEBUG, "ZW: Gateways_found");
			for (i = 0; i < appl_ctx.gw_addr_cnt; i++) {
				result = zwnet_ip_ntoa(
						appl_ctx.gw_addr_buf + (i * one_ip_addr_len),
						zip_gw_addr_str, 80, appl_ctx.use_ipv4);
				if (result == 0) {
					syslog(LOG_DEBUG,
							"ZW: Discoverd gateway (%d), address (%s), name (%s)",
							i, zip_gw_addr_str,
							(appl_ctx.gw_name) ?
									appl_ctx.gw_name[i] : "unknown");
					/*Initialize network*/
					if (nw_init(&appl_ctx) != 0)
						syslog(LOG_DEBUG,
								"ZW: Initialize network in progress, please wait for status ...");
					do {
						sleep(1);
					} while (appl_ctx.init_status == 0);
				} else {
					/* TODO : Inform to Service Manager */
					syslog(LOG_CRIT, "ZW: Gateway_failed to Initialize");
				}

				if (appl_ctx.init_status == 1)
					break;
			}
		}

	} while (appl_ctx.gw_addr_cnt <= 0);
	/*Free allocated memory*/
	if (appl_ctx.gw_addr_cnt > 0) {
		/*Free gateway names buffer*/
		if (appl_ctx.gw_name) {
			int i;
			for (i = 0; i < appl_ctx.gw_addr_cnt; i++) {
				free(appl_ctx.gw_name[i]);
			}
			free(appl_ctx.gw_name);
			appl_ctx.gw_name = NULL;
		}
		/*Free gateway IP address buffer*/
		free(appl_ctx.gw_addr_buf);
		appl_ctx.gw_addr_buf = NULL;
		appl_ctx.gw_addr_cnt = 0;
	}

	bootup_scenario();

	while (1) {
		syslog(LOG_DEBUG, "ZW: *******MAIN******* ");


		syslog(LOG_DEBUG, "ZW: check_discovery_flag111dsfsf1 %u %d",check_discovery_flag,&check_discovery_flag);



		/************* Data Receiving from dm**************** */
		if (msgrcv(msqid_req, &request, sizeof(request_t), 1, 0) < 0) {
			/* TODO : Inform to Service Manager */
			syslog(LOG_CRIT, "ZW: msgrcv error : Failed to receive Message ");
			exit(0);
		} else {


			syslog(LOG_DEBUG, "ZW: check_discovery_flag111dsfsf2 %u %d",check_discovery_flag,&check_discovery_flag);


			syslog(LOG_DEBUG, "ZW: SIZE OF REQUEST *********** IS : %d", sizeof(request));
			syslog(LOG_DEBUG, "ZW: operation: %d", request.operation);
			syslog(LOG_DEBUG, "ZW: Homeid: %lu", request.home_id);
			syslog(LOG_DEBUG, "ZW: Nodeid: %u", request.node_id);
			syslog(LOG_DEBUG, "ZW: Endpointid: %u", request.ep_id);
			syslog(LOG_DEBUG, "ZW: cls: %u", request.cmd_cls);
			syslog(LOG_DEBUG, "ZW: Generic device class : %u",
					request.generic_dc);
			syslog(LOG_DEBUG, "ZW: Specific device class: %u",
					request.specific_dc);
			syslog(LOG_DEBUG, "ZW: Property_type: %d", request.property_type);

			memset(&request.dsk,0,sizeof(request.dsk));

			syslog(LOG_DEBUG, "ZW: Property_type kalyan: %s", request.dsk);

			if (request.operation != 9) {

				requestdummy = request;
			}

			if (requestdummy.property_type == ARR_TYPE) {
				for (count_value = 0; count_value < NO_OF_COLOR_COMP;
						count_value++) {
					syslog(LOG_DEBUG, "ZW: property value %d:%d", count_value,
							requestdummy.property_value.arr[count_value]);
				}
			} else if (requestdummy.property_type == CHAR_TYPE) {
				syslog(LOG_DEBUG, "ZW: property value %d",
						requestdummy.property_value.ch);
			} else {
				syslog(LOG_DEBUG, "ZW: Wrong property type ");
				if (request.operation == CONSIGN) {
					memset(&request, 0, sizeof(request));
				}
			}
		
		}

	syslog(LOG_DEBUG, "ZW: check_discovery_flag111dsfsf3 %u %d",check_discovery_flag,&check_discovery_flag);
		if(check_discovery_flag==0)
		{
		switch (request.operation) {
		case COMMISSIONING:
			add_node();
			break;

		case CONSIGN:
			control_node();
			break;

			/* case REFRESH:
			 refresh_node();
			 break;
			 */

		case REMOVE:
		case EXCLUSION:
			remove_node();
			break;

		case RESET:
			reset_network();
			break;

		case CLEAR_REQUEST:
			syslog(LOG_DEBUG, "ZW: Cleared request structure");
			memset(&request, 0, sizeof(request));
			break;

		case FORCE_REMOVE:
			syslog(LOG_DEBUG, "ZW: In FORCE_REMOVE");
			response.err_code = OP_SUCCESS;
			response.operation = request.operation;
			response.home_id = homeid;
			response.node_id = request.node_id;
			response.msgid = request.msgid;
			response_DM(response);
			memset(&response, 0, sizeof(response));
			result = zwnet_fail(appl_ctx.zwnet, request.node_id, 0, 0, 0);
			syslog(LOG_DEBUG, "ZW: Force remove result is:%d", result);
			memset(&request, 0, sizeof(request));
			break;

		case INTIATE:
			syslog(LOG_DEBUG, "ZW: Intaite Request");
			initate_network();
			break;
	
		case UPDATE:
			syslog(LOG_DEBUG, "ZW: UPDATE Request");
			update_network();
			break;	

		default:
			syslog(LOG_ERR, "ZW: Main Invalid Operation choice");
			response.home_id = homeid;
			response.err_code = ERR_INVALID_REQ_OP;
			response.operation = request.operation;
			response.msgid = request.msgid;
			response_DM(response);
			memset(&request, 0, sizeof(request));
			//print_discovery_struct();
			break;
		}
	}
	
	else
	{
		switch (request.operation)
		{
				case COMMISSIONING:
				syslog(LOG_DEBUG, "ZW: UPDATE Request");
				update_network();
				break;

				case EXCLUSION:	
				case REMOVE:
				syslog(LOG_ERR, "ZW: Main Invalid Operation choice");
				response.home_id = homeid;
				response.err_code = 255;
				response.operation = request.operation;
				response.msgid = request.msgid;
				response_DM(response);
				memset(&request, 0, sizeof(request));	
				break;

				/*case EXCLUSION:
				case REMOVE:
				update_network();
				break;*/

				case CONSIGN:
				control_node();
				break;

				case RESET:
				reset_network();
				break;

			case INTIATE:
			syslog(LOG_DEBUG, "ZW: Intaite Request");
			initate_network();
			break;

			case UPDATE:
			syslog(LOG_DEBUG, "ZW: UPDATE Request");
			update_network();

		

			break;	
									
		}
	}
}
	return 0;
}

