#include "dm_utility.hpp"
#include "zb_message_queue.hpp"

static int msqid_read;
static int msqid_send;
static int msqid_Discread;

/*ZbMessageQueue::ZbMessageQueue() {
}
ZbMessageQueue::~ZbMessageQueue() {
}*/

bool ZbMessageQueue::ZbMsgSendTest() {
	//syslog(LOG_INFO, "[DM]: Zigbee message queue IPC request");

	syslog(LOG_DEBUG, "[DM]: Zigbee message queue IPC request key: [%d]",
	MSQ_TX_KEY);

	if ((msqid_send = msgget(MSQ_TX_KEY, IPC_CREAT | 0666)) < 0) {
		syslog(LOG_INFO,
				"[DM]: Zigbee message queue IPC request setting failed");
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Zigbee message queue IPC request setting successfully");
		return true;
	}
}

bool ZbMessageQueue::ZbMsgReadTest() {
	//syslog(LOG_INFO, "[DM]: Zigbee message queue IPC non discovery response");

	syslog(LOG_DEBUG,
			"[DM]: Zigbee message queue IPC non discovery response key: [%d]",
			MSQ_RX_KEY);

	if ((msqid_read = msgget(MSQ_RX_KEY, IPC_CREAT | 0666)) < 0) {
		syslog(LOG_INFO,
				"[DM]: Zigbee message queue IPC non discovery response setting failed");

		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Zigbee message queue IPC non discovery response setting successfully");
		return true;
	}
}

bool ZbMessageQueue::ZbMsgReadDiscTest() {
	//syslog(LOG_INFO, "[DM]: Zigbee message queue IPC discovery response");

	syslog(LOG_DEBUG,
			"[DM]: Zigbee message queue IPC discovery response key: [%d]",
			MSQ_RX_DISC_KEY);

	if ((msqid_Discread = msgget(MSQ_RX_DISC_KEY, IPC_CREAT | 0666)) < 0) {
		syslog(LOG_INFO,
				"[DM]: Zigbee message queue IPC discovery response setting failed");
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Zigbee message queue IPC discovery response setting successfully");
		return true;
	}
}

void IAS_ZONE(uint16_t status) {

	for (char i = 0; i < 10; i++) {

		switch (i) {

		case ZBALARM:
			syslog(LOG_DEBUG,
					"[DM]: Zigbee message queue IPC IAS zone status: ZBALARM: %d",
					status >> i & 1);
			break;

		case ZBTAMPER:
			syslog(LOG_DEBUG,
					"[DM]: Zigbee message queue IPC IAS zone status: ZBTAMPER: %d",
					status >> i & 1);
			break;

		case ZBBATTERY:
			syslog(LOG_DEBUG,
					"[DM]: Zigbee message queue IPC IAS zone status: ZBBATTERY: %d",
					status >> i & 1);
			break;

		case ZBBATTERYDEFECT:
			syslog(LOG_DEBUG,
					"[DM]: Zigbee message queue IPC IAS zone status: ZBBATTERYDEFECT: %d",
					status >> i & 1);
			break;
		default:

			break;
		}
	}
}

/* Function name : ZigbeeMsgQueueReq
 * Description : This is the send function to communicate the request to Zigbee
 * @param : Device details structure object
 */
bool ZbMessageQueue::ZigbeeMsgQueueReq(DMReqStructZB DMReqStructZBObj) {

	syslog(LOG_INFO, "[DM]: Zigbee message queue IPC request send");

	if (msgsnd(msqid_send, &DMReqStructZBObj, sizeof(DMReqStructZB), 0) == 0) {

		syslog(LOG_DEBUG, "[DM]: Zigbee message queue IPC request mode: [%d]  message_id: [%d]  node_id: [%d]  timeout: [%d]  ieee: [%llx]  device_id: [%d]  endpoint: [%d]  cluster_id: [%d]  command_id: [%d]  error_code: [%d]",
				DMReqStructZBObj.Mode,DMReqStructZBObj.MsgId,
				DMReqStructZBObj.nodeId,DMReqStructZBObj.timeout,
				DMReqStructZBObj.IEEE,DMReqStructZBObj.deviceId,
				DMReqStructZBObj.endpoint,DMReqStructZBObj.clusterId,
				DMReqStructZBObj.commandId,DMReqStructZBObj.ErrorCode);

		/*syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC request message_id: [%d]",
				DMReqStructZBObj.MsgId);

		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC request node_id: [%d]",
				DMReqStructZBObj.nodeId);

		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC request timeout: [%d]",
				DMReqStructZBObj.timeout);
		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC request ieee: [%016lx]",
				DMReqStructZBObj.IEEE);
		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC request device_id: [%d]",
				DMReqStructZBObj.deviceId);

		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC request endpoint: [%d]",
				DMReqStructZBObj.endpoint);
		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC request cluster_id: [%d]",
				DMReqStructZBObj.clusterId);
		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC request command_id: [%d]",
				DMReqStructZBObj.commandId);
		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC request error_code: [%d]",
				DMReqStructZBObj.ErrorCode);*/
		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC request argument_count: [%d]",
				DMReqStructZBObj.argcount);
		for (int i = 0; i < DMReqStructZBObj.argcount; i++) {
			syslog(LOG_DEBUG,
					"[DM]: Zigbee message queue IPC request argument_type: [%d]",
					DMReqStructZBObj.argValue_info[i].argtype);
			syslog(LOG_DEBUG,
					"[DM]: Zigbee message queue IPC request argument_value: [%d]",
					DMReqStructZBObj.argValue_info[i].argValue.intargValue);
		}

		/*syslog(LOG_INFO,
				"[DM]: Zigbee message queue IPC message queue request sending success");*/
		return true;

	} else {
		syslog(LOG_INFO,
				"[DM]: Zigbee message queue IPC message queue request sending failed");
		return false;
	}
	return false;
}

DMReqStructZB ZbMessageQueue::ZigbeeNonDiscoveryRcvResp(
		DMReqStructZB DMReqStructZBObj) {

	/*syslog(LOG_INFO,
			"[DM]: Zigbee message queue IPC non discovery response receive");*/

	if (msgrcv(msqid_read, &DMReqStructZBObj, sizeof(DMReqStructZBObj), 1, 0)
			> 0) {

		syslog(LOG_DEBUG, "[DM]: Zigbee message queue IPC non discovery response mode: [%d]  message_id: [%d]  timeout: [%d] node_id: [%d] ieee: [%llx] endpoint: [%d]  cluster_id: [%d] device_id: [%d]  command_id: [%d]  error_code: [%d]",
				DMReqStructZBObj.Mode,DMReqStructZBObj.MsgId,
				DMReqStructZBObj.timeout,DMReqStructZBObj.nodeId,
				DMReqStructZBObj.IEEE,DMReqStructZBObj.endpoint,
				DMReqStructZBObj.clusterId,DMReqStructZBObj.deviceId,
				DMReqStructZBObj.commandId,DMReqStructZBObj.ErrorCode);
		/*syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC response message_id: [%d]",
				DMReqStructZBObj.MsgId);
		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC response timeout: [%d]",
				DMReqStructZBObj.timeout);
		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC response node_id: [%d]",
				DMReqStructZBObj.nodeId);
		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC response ieee: [%016lx]",
				DMReqStructZBObj.IEEE);
		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC response endpoint: [%d]",
				DMReqStructZBObj.endpoint);
		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC response cluster_id: [%d]",
				DMReqStructZBObj.clusterId);
		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC response device_id: [%d]",
				DMReqStructZBObj.deviceId);
		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC response command_id: [%d]",
				DMReqStructZBObj.commandId);
		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC response error_code: [%d]",
				DMReqStructZBObj.ErrorCode);*/
		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC response argument_count: [%d]",
				DMReqStructZBObj.argcount);

		for (int i = 0; i < DMReqStructZBObj.argcount; i++) {

			syslog(LOG_DEBUG,
					"[DM]: Zigbee message queue IPC response argument_type: [%d]",
					DMReqStructZBObj.argValue_info[i].argtype);

			syslog(LOG_DEBUG,
					"[DM]: Zigbee message queue IPC response attribute_id: [%d]",
					DMReqStructZBObj.argValue_info[i].attributeId);

			syslog(LOG_DEBUG,
					"[DM]: Zigbee message queue IPC response argument_value: [%d]",
					DMReqStructZBObj.argValue_info[i].argValue.intargValue);

			if (DMReqStructZBObj.clusterId
					== 0x0500&& DMReqStructZBObj.argValue_info[i].attributeId == ZONE_STATUS_ATTRIBUTE) {
				IAS_ZONE(
						DMReqStructZBObj.argValue_info[i].argValue.intargValue);
			}
		}

		/*syslog(LOG_INFO,
				"[DM]: Zigbee message queue IPC non discovery response receiving success");*/
		return DMReqStructZBObj;
	} else {
		syslog(LOG_INFO,
				"[DM]: Zigbee message queue IPC non discovery response receiving failed");
		return DMReqStructZBObj;
	}
	return DMReqStructZBObj;
}

DMRespStructZB ZbMessageQueue::ZigbeeDiscoveryRcvResp(
		DMRespStructZB ZB_MsgQue_ResObj) {

	/*syslog(LOG_INFO,
			"[DM]: Zigbee message queue IPC discovery response receive");*/
	ZB_MsgQue_ResObj.mtype = 1;

	if (msgrcv(msqid_Discread, &ZB_MsgQue_ResObj, sizeof(ZB_MsgQue_ResObj), 1,
			0) > 0) {

		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC discovery response nodeid:[%d] msgid: [%d] mode: [%d] ieee: [%lld] error_code: [%d]",
				ZB_MsgQue_ResObj.node_id,ZB_MsgQue_ResObj.MsgId,ZB_MsgQue_ResObj.Mode,ZB_MsgQue_ResObj.IEEE,ZB_MsgQue_ResObj.ErrorCode);
		/*syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC discovery response msgid: [%d]",
				ZB_MsgQue_ResObj.MsgId);
		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC discovery response mode: [%d]",
				ZB_MsgQue_ResObj.Mode);
		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC discovery response ieee: [%lld]",
				ZB_MsgQue_ResObj.IEEE);
		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC discovery response error_code: [%d]",
				ZB_MsgQue_ResObj.ErrorCode);*/
		syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue IPC discovery response ep_count:[%d]",
				ZB_MsgQue_ResObj.endpointCount);
		for (int EndPointNo = 0; EndPointNo < ZB_MsgQue_ResObj.endpointCount;
				EndPointNo++) {
			syslog(LOG_DEBUG,
					"[DM]: Zigbee message queue IPC discovery response endpoint_id[%d]: [%d] device_id:[%d]",
					EndPointNo + 1,
					ZB_MsgQue_ResObj.Endpoint[EndPointNo].endpointId,
					ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId);

			syslog(LOG_DEBUG,
					"[DM]: Zigbee message queue IPC discovery response printing cluster count: [%d]",
					ZB_MsgQue_ResObj.Endpoint[EndPointNo].clusterCount);

			for (unsigned char ClusterNo = 0;
					ClusterNo
							< ZB_MsgQue_ResObj.Endpoint[EndPointNo].clusterCount;
					ClusterNo++) {

				syslog(LOG_DEBUG,
						"[DM]: Zigbee message queue IPC discovery response cluster_no: [%d] cluster_id: [%d]",
						ClusterNo,
						ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId);

				syslog(LOG_DEBUG,
						"[DM]: Zigbee message queue IPC discovery response printing command count: [%d]",
						ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdCount);

				for (unsigned int CommandNo = 0;
						CommandNo
								< ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdCount;
						CommandNo++) {

					syslog(LOG_DEBUG,
							"[DM]: Zigbee message queue IPC discovery response command_no: [%d] command_id: [%d]",
							CommandNo,
							ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandNo]);

				}

				syslog(LOG_DEBUG,
						"[DM]: Zigbee message queue IPC discovery response printing attribute count: [%d]",
						ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributeCount);

				for (unsigned int AttributeNo = 0;
						AttributeNo
								< ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributeCount;
						AttributeNo++) {

					syslog(LOG_DEBUG,
							"[DM]: Zigbee message queue IPC discovery response attribute_type = [%d]",
							ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeType);
					syslog(LOG_DEBUG,
							"[DM]: Zigbee message queue IPC discovery response attribute_id = [%d]",
							ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeId);
					syslog(LOG_DEBUG,
							"[DM]: Zigbee message queue IPC discovery response attribute_val = [%d]",
							ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal);

					if (ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId
							== 0x0500&& ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeId
							== ZONE_STATUS_ATTRIBUTE) {
						IAS_ZONE(
								ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal);
					}
				}
			}
		}
		/*syslog(LOG_INFO,
				"[DM]: Zigbee message queue IPC discovery response receiving success");*/
		return ZB_MsgQue_ResObj;
	} else {
		syslog(LOG_INFO,
				"[DM]: Zigbee message queue IPC discovery response receiving failed");
		return ZB_MsgQue_ResObj;
	}

	return ZB_MsgQue_ResObj;
}

