/*
 * File_name:zb_message_queue.hpp
 * @Brief:Driver class to intantiate IPC mechanism between Zigbee framework in DM and Zigbee customized framework
 * Created on: 26-Jul-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef INCLUDE_ZB_MESSAGE_QUEUE_HPP_
#define INCLUDE_ZB_MESSAGE_QUEUE_HPP_

#include <iostream>
#include <fstream>
#include <sys/msg.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>
#include <sys/time.h>

#include <json-c/json.h>
#include "fw-env.h"
#include <sys/syslog.h>

#define MSQ_TX_KEY 			1111	//	receiving
#define MSQ_RX_KEY 			2222  	//	sending Non Discovery
#define MSQ_RX_DISC_KEY 	        3333 	//	sending Discovery

#define ZBALARM						0
#define ZBTAMPER					2
#define ZBBATTERY					3
#define ZBBATTERYDEFECT				        9

#define OPERATION_SIZE 				        15
#define MAX_ARG_COUNT 				        6
#define ATTRIBUTE_ARR_VALUE 		                32

#define ZONE_STATUS_ATTRIBUTE 	                        2
#define ZONE_TYPE_ATTRIBUTE 	                        1

class ZbMessageQueue {
public:

	ZbMessageQueue() {}
	static bool ZbMsgCleanTest();
	static bool ZbMsgReadTest();
	static bool ZbMsgSendTest();
	static bool ZbMsgReadDiscTest();
	static bool ZigbeeMsgQueueReq(DMReqStructZB);
	static DMReqStructZB ZigbeeNonDiscoveryRcvResp(DMReqStructZB);
	static DMRespStructZB ZigbeeDiscoveryRcvResp(DMRespStructZB);
	 ~ZbMessageQueue() {}
};

#endif /* INCLUDE_ZB_MESSAGE_QUEUE_HPP_ */
