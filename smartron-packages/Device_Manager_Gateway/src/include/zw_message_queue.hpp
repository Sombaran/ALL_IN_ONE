/*
 * File_name:zw_message_queue.hpp
 * @Brief: This file defines the stucture for Zwave layers
 * Created on: 26-Jul-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef INCLUDE_ZW_MESSAGE_QUEUE_HPP_
#define INCLUDE_ZW_MESSAGE_QUEUE_HPP_

#include <fstream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <json-c/json.h>
#include <sys/syslog.h>
#include "fw-env.h"

#define MSQ_TX_KEY_ZW 			5555  //rec
#define MSQ_RX_KEY_ZW			4444  //send

//+++++++++++++++++++++++++++++ ZW RESPONSE STRUCT START ++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++ZW RESPONSE STRUCT START++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define NO_OF_SENSOR_TYPE 		3
#define NO_OF_EVENT_TYPE 		3
#define NO_OF_ENDPOINTS 		5
#define NO_OF_COLOR_COMP 		5
#define NO_OF_INTERFACES		5

typedef struct {
	uint8_t EventType;
	uint8_t NoSensorType;
	uint8_t SensorType[NO_OF_SENSOR_TYPE];
} event;

typedef struct {
	uint16_t CommandClass;                       ///< Command Class
	event Events[NO_OF_EVENT_TYPE];
	uint8_t NoEventType;
	uint8_t PropertyDataType;                  ///< Data type of property
	union PropertyState {
		uint8_t ArrayValue[NO_OF_COLOR_COMP]; ///< Integer array property value
		uint8_t CharValue;                    ///< Char property value
	} PropertyValue;                  ///< Union for the property values
} interface; /* Interface Structure*/

typedef struct {
	uint8_t EndPointId;                 ///< End point id
	uint8_t GenericDeviceClass;            ///< Generic Device Class
	uint8_t SpecificDevice;           ///< Specific Device Class
	uint8_t NoInterFace;
	interface InterFace[NO_OF_INTERFACES]; ///< Interface structure array
} endpoint; /* Endpoint structure*/

typedef struct {
	long Mtype; ///< Message queue type
	uint32_t MsgId;
	uint8_t Operation; ///< Operation (Add/Get/Set)
	uint32_t HomeId;  ///< Home ID
	uint8_t NodeId;   ///< Node ID
	uint16_t VendorId;      ///< Vendor ID
	uint16_t ProductId;    ///< Product ID
	uint16_t VendorType;    ///< Vendor Product Type
	uint16_t Category;  ///< Device Category
	char Key[6];  ///< Home ID
	int ErrorCode;      ///< 0= success, error code = failure
	uint8_t NoEndPoint;
	endpoint EndPoint[NO_OF_ENDPOINTS]; ///< array of endpoint structures
} DMRespStructZW;                     ///< Response Structure

//++++++++++++++++++++++++++++++++++++++++++++++ZW RESPONSE STRUCT END+++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++ZW REQUEST STRUCT START+++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct {
	long Mtype;          ///< Message Queue type
	uint32_t MsgId;      ///< Message Queue ID
	uint8_t Operation;   ///< Operation (Add/Get/Set)
	uint32_t HomeId;    ///< Home ID
	uint8_t NodeId;     ///< Node ID
	uint8_t EndPoint;       ///< End Point ID
	uint8_t GenericDeviceClass;  ///< Generic Device Class
	uint8_t SpecificDeviceClass; ///< Specific Device Class
	uint16_t CommandClass;    ///< Command Class
	uint8_t SensorType;        ///< Sensor type
	char PropertyDataType;  ///< Data type of Property
	union PropertyState {
		uint8_t ArrayValue[NO_OF_COLOR_COMP]; ///< Integer array property value
		uint8_t CharValue;                    ///< Character value
	} PropertyValue;                  ///< Union for storing property value
		char dsk[6];					/* Device specific key */
} DMReqStructZW;                           ///< Request structure

//++++++++++++++++++++++++++++++++++++++++++++++ZW REQUEST STRUCT END+++++++++++++++++++++++++++++++++++++++++++++++++++++++

class ZwMessageQueue {
public:
	ZwMessageQueue() {}
	static bool ZwaveMsgReadTest();
	static bool ZwaveMsgSendTest();
	static bool ZwMsgCleanTest();
	static bool ZwaveMsgQueueReq(DMReqStructZW);
	static DMRespStructZW ZwaveMsgQueueResp(DMRespStructZW);
	 ~ZwMessageQueue() {}
};

#endif /* INCLUDE_ZW_MESSAGE_QUEUE_HPP_ */

