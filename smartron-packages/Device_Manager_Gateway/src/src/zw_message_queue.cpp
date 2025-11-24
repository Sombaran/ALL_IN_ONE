#include "dm_utility.hpp"
#include "zw_message_queue.hpp"
#include "dm_platform.hpp"

int Zwmsqid_read;
int Zwmsqid_send;
/*
ZwMessageQueue::ZwMessageQueue() {
}
ZwMessageQueue::~ZwMessageQueue() {
}*/

bool ZwMessageQueue::ZwaveMsgReadTest() {
	syslog(LOG_INFO, "[DM]: Zwave message queue response in zwave message read test");

	syslog(LOG_DEBUG, "[DM]: Zwave message queue response ZWKEY_READ: [%d]",
	MSQ_RX_KEY_ZW);

	if ((Zwmsqid_read = msgget(MSQ_RX_KEY_ZW, IPC_CREAT | 0666)) < 0) {
		syslog(LOG_INFO,
				"[DM]: [DM]: Zwave message queue response recv not set succssfully");
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: [DM]: Zwave message queue response recv set succssfully");
		return true;
	}
}

bool ZwMessageQueue::ZwaveMsgSendTest() {
	syslog(LOG_INFO, "[DM]: Zwave message queue response in zwave message send test");

	syslog(LOG_DEBUG, "[DM]: Zwave message queue response ZWKEY_SEND: [%d]",
	MSQ_TX_KEY_ZW);

	if ((Zwmsqid_send = msgget(MSQ_TX_KEY_ZW, IPC_CREAT | 0666)) < 0) {
		syslog(LOG_INFO, "[DM]: Zwave message queue response send not set successfully");
		return false;
	} else {
		syslog(LOG_INFO, "[DM]: Zwave message queue response send set successfully");
		return true;
	}
}

bool ZwMessageQueue::ZwMsgCleanTest() {
	syslog(LOG_INFO, "[DM]: Zwave message queue response in zwave message clean test");

	if (msgctl(Zwmsqid_read, IPC_RMID, 0) == -1) {
		syslog(LOG_INFO,
				"[DM]: Zwave message queue response RECV CLEAN successfully");
		return false;
	}
	if (msgctl(Zwmsqid_send, IPC_RMID, 0) == -1) {
		syslog(LOG_INFO,
				"[DM]: Zwave message queue response SEND CLEAN successfully");
		return false;
	}
	return true;
}

bool ZwMessageQueue::ZwaveMsgQueueReq(DMReqStructZW DMReqStructZWObj) {

	if (msgsnd(Zwmsqid_send, &DMReqStructZWObj, sizeof(DMReqStructZWObj), 0)
			== 0) {
		syslog(LOG_DEBUG, "[DM]: Zwave message queue response request message_id: [%d]",
				DMReqStructZWObj.MsgId);
		syslog(LOG_DEBUG, "[DM]: Zwave message queue response request operation: [%u]",
				DMReqStructZWObj.Operation);
		syslog(LOG_DEBUG, "[DM]: Zwave message queue response request home_id: [%u]",
				DMReqStructZWObj.HomeId);
		syslog(LOG_DEBUG, "[DM]: Zwave message queue response request node_id: [%d]",
				DMReqStructZWObj.NodeId);

		syslog(LOG_DEBUG, "[DM]: Zwave message queue response request end point id: [%u]",
				DMReqStructZWObj.EndPoint);
		syslog(LOG_DEBUG,
				"[DM]: Zwave message queue response request generic_device_class: [%u]",
				DMReqStructZWObj.GenericDeviceClass);
		syslog(LOG_DEBUG,
				"[DM]: Zwave message queue response Request specific_device_class: [%u]",
				DMReqStructZWObj.SpecificDeviceClass);
		syslog(LOG_DEBUG,
				"[DM]: Zwave message queue response request command_class_id: [%u]",
				DMReqStructZWObj.CommandClass);
		syslog(LOG_DEBUG, "[DM]: Zwave message queue response request sensor_type: [%u]",
				DMReqStructZWObj.SensorType);
		syslog(LOG_DEBUG,
				"[DM]: Zwave message queue response request property_type: [%u]",
				DMReqStructZWObj.PropertyDataType);

		if ((int) DMReqStructZWObj.PropertyDataType == 2) {
			for (int count_value = 0; count_value < 5; count_value++) {
				syslog(LOG_DEBUG,
						"[DM]: Zwave message queue response request property_value: [%d] color value: [%d] ",
						count_value,
						DMReqStructZWObj.PropertyValue.ArrayValue[count_value]);
			}

		} else if ((int) DMReqStructZWObj.PropertyDataType == 3) {
			syslog(LOG_DEBUG,
					"[DM]: Zwave message queue response request property_value: [%d]",
					DMReqStructZWObj.PropertyValue.CharValue);

		}
		syslog(LOG_INFO, "[DM]: Zwave message queue request sent");
		return true;
	} else {
		syslog(LOG_INFO, "[DM]: Zwave message queue request not sent");
		return false;
	}
}

DMRespStructZW ZwMessageQueue::ZwaveMsgQueueResp(
		DMRespStructZW DMRespStructZWObj) {

	syslog(LOG_INFO, "[DM]: Zwave message queue start receive");

	if (msgrcv(Zwmsqid_read, &DMRespStructZWObj, (sizeof(DMRespStructZWObj)), 1,
			0) > 0) {

		syslog(LOG_INFO, "[DM]: Zwave message queue start receive");

		syslog(LOG_DEBUG, "[DM]: Zwave response message_id: [%u] operation: [%u]  home_id: [%u]  node_id: [%u] version_id: [%u]  version_type : [%u]  product_id: [%u]  category_version: [%u]  error_code: [%d]",
				DMRespStructZWObj.MsgId,DMRespStructZWObj.Operation,
				DMRespStructZWObj.HomeId,DMRespStructZWObj.NodeId,
				DMRespStructZWObj.VendorId,DMRespStructZWObj.ProductId,
				DMRespStructZWObj.VendorType,DMRespStructZWObj.Category,
				DMRespStructZWObj.ErrorCode);
	/*	syslog(LOG_DEBUG, "[DM]: Zwave message queue response response operation: [%u]",
				DMRespStructZWObj.Operation);
		syslog(LOG_DEBUG, "[DM]: Zwave message queue response response home_id: [%u]",
				DMRespStructZWObj.HomeId);
		syslog(LOG_DEBUG, "[DM]: Zwave message queue response response node_id: [%u]",
				DMRespStructZWObj.NodeId);
		syslog(LOG_DEBUG, "[DM]: Zwave message queue response response version_id: [%u]",
				DMRespStructZWObj.VendorId);
		syslog(LOG_DEBUG, "[DM]: Zwave message queue response response product_id: [%u]",
				DMRespStructZWObj.ProductId);
		syslog(LOG_DEBUG,
				"[DM]: Zwave message queue response response version_type : [%u]",
				DMRespStructZWObj.VendorType);
		syslog(LOG_DEBUG,
				"[DM]: Zwave message queue response response category_version: [%u]",
				DMRespStructZWObj.Category);
		syslog(LOG_DEBUG, "[DM]: Zwave message queue response response error_code: [%d]",
				DMRespStructZWObj.ErrorCode);*/

		syslog(LOG_DEBUG,
				"[DM]: Zwave message queue response response end point count: [%u]",
				DMRespStructZWObj.NoEndPoint);

		for (int EndPointNo = 0;
				EndPointNo < DMRespStructZWObj.NoEndPoint
						&& DMRespStructZWObj.NoEndPoint != 255; EndPointNo++) {

			syslog(LOG_DEBUG,
					"[DM]: Zwave message queue response response end point_id: [%u]",
					DMRespStructZWObj.EndPoint[EndPointNo].EndPointId);
			syslog(LOG_DEBUG, "[DM]: Zwave response generic class id: [%u]",
					DMRespStructZWObj.EndPoint[EndPointNo].GenericDeviceClass);
			syslog(LOG_DEBUG,
					"[DM]: Zwave response generic specific_id: [%u]",
					DMRespStructZWObj.EndPoint[EndPointNo].SpecificDevice);
			syslog(LOG_DEBUG, "[DM]: Zwave response interface count: [%u]",
					DMRespStructZWObj.EndPoint[EndPointNo].NoInterFace);

			for (int InterfaceNo = 0;
					InterfaceNo
							< DMRespStructZWObj.EndPoint[EndPointNo].NoInterFace
							&& DMRespStructZWObj.EndPoint[EndPointNo].NoInterFace
									!= 255; InterfaceNo++) {

				syslog(LOG_DEBUG,
						"[DM]: Zwave response command_class_id: [%u]",
						DMRespStructZWObj.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass);

				/*syslog(LOG_DEBUG,
						"[DM]: Zwave response no sensor types: [%u]",
						DMRespStructZWObj.EndPoint[EndPointNo].InterFace[InterfaceNo].NoEventType);*/

				for (int EventType = 0;
						EventType
								< DMRespStructZWObj.EndPoint[EndPointNo].InterFace[InterfaceNo].NoEventType
								&& DMRespStructZWObj.EndPoint[EndPointNo].InterFace[InterfaceNo].NoEventType
										!= 255; EventType++) {

					syslog(LOG_DEBUG,
							"[DM]: Zwave response sensor_type: [%d]",
							DMRespStructZWObj.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType);

					/*syslog(LOG_DEBUG,
							"[DM]: Zwave response no event types: [%u]",
							DMRespStructZWObj.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].NoSensorType);*/

					for (int EventCount = 0;
							EventCount
									< DMRespStructZWObj.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].NoSensorType
									&& DMRespStructZWObj.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].NoSensorType
											!= 255; EventCount++) {

						syslog(LOG_DEBUG,
								"[DM]: Zwave Response event_type: [%d]",
								DMRespStructZWObj.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].SensorType[EventCount]);
					}

				}

				syslog(LOG_DEBUG,
						"[DM]: Zwave Response attribute_type: [%d]",
						DMRespStructZWObj.EndPoint[EndPointNo].InterFace[InterfaceNo].PropertyDataType);

				if (DMRespStructZWObj.EndPoint[EndPointNo].InterFace[InterfaceNo].PropertyDataType
						== 2) {

					for (int ArrayCount = 0; ArrayCount < NO_OF_COLOR_COMP;
							ArrayCount++) {
						syslog(LOG_DEBUG,
								"[DM]: Zwave Response attribute_value count: [%d] attribute_value array: [%d]",
								ArrayCount,
								DMRespStructZWObj.EndPoint[EndPointNo].InterFace[InterfaceNo].PropertyValue.ArrayValue[ArrayCount]);
					}

				} else if (DMRespStructZWObj.EndPoint[EndPointNo].InterFace[InterfaceNo].PropertyDataType
						== 3) {
					syslog(LOG_DEBUG,
							"[DM]: Zwave Response attribute_value char: [%d]",
							DMRespStructZWObj.EndPoint[EndPointNo].InterFace[InterfaceNo].PropertyValue.CharValue);
				} else {

				}
			}
		}

		//syslog(LOG_INFO, "[DM]: Zwave message queue end receive");
		return DMRespStructZWObj;
	} else {
		syslog(LOG_INFO, "[DM]: Zwave request response not received");
		DMRespStructZWObj.Operation = 10;
		return DMRespStructZWObj;

	}

}

