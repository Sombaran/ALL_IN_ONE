/*
 * File_name:dm_converter_handler.hpp
 * @Brief: This file initialize the value for each propertydetails/entitydetails
 * structutes for request/response, Zigbee/Zwave, final_response brfore sending to TronX
 * URI structure, RGB color changes 
 * Created on: 29-Aug-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef INCLUDE_DM_UTILITY_HPP_
#define INCLUDE_DM_UTILITY_HPP_

#include <iostream>
#include <string>
#include <cmath>
#include <sys/time.h>
#include <sys/syslog.h>
#include <json-c/json.h>

#define DM_SUCCESS				200
#define DM_NULL					0
#define DM_FAIL					500
#define SCENE_ALREADY_PROCESSING		102
#define PRORITY_VALUE				50
#define THING_NOT_PRESENT_IN_DM  		204
#define CONTROL_TIME_OUT			504
#define BAD_REQUEST				400
#define DISCOVERY_ALREADY_IN_PROCESS 		102

#define CRON_HANDLER	                11
#define MOBILE_AGENT		        13
#define SOCKET_SERVICE		        12
#define MQTT_SERVICE		        14
#define DM_SERVICE		        15
#define FOTA_SERVICE		        16
#define AREA_CONTROL		        77
#define SCENE_CONTROL		        99
#define RULE_COLTROL		        88

#define SWITCH 				1
#define RANGE		  		2
#define COLOR 				3
#define LIST				4
#define INFO				5

#define DM_SWITCH 			706
#define DM_RANGE		  	605
#define DM_COLOR 			623
#define DM_LIST				556
#define DM_INFO				5

#define MODE_COMMISSIONING               1
#define MODE_CONTROL                     2
#define MODE_OBSERVE                     6
#define MODE_REMOVE                      4
#define MODE_RESET 			 5
#define MODE_REFRESH		         3
#define MODE_S2_SECURITY 	         9
#define MODE_TIMEOUT                     10
#define MODE_STOP_DISC		         11
#define MODE_EXCLUSION		         13
#define MODE_FORCE_REMOVE                12
#define MODE_LEARN_MODE				14

//#define 	SWITCH			1
//#define 	RANGE			2
//#define 	COLOR			3
//#define 	LIST			4
//#define 	INFO			5

//#define SWITCH 			466
//#define RANGE		  		365
//#define COLOR 			383
//#define LIST				316
//#define INFO				300

#define DISCOVERY 			696
#define CONTROL		  		545
#define REMOVE 				462
#define OBSERVE 			534
#define THING_LEFT			1060
#define EXCLUSION			698
#define FACTORY_RESET		        1018
#define IOT_RESET			718
#define THINGSLIST			777
#define GETRULE 			536
#define GETSCENE			590
#define RULE			        312
#define SCENE			        366
#define VOICE_SCENE            		835	/** This Macro gives ascii value of string "VOICE_SCENE" which is created to differentiate the scene request from Voice or Mobile */
#define S2_SECURITY			860
#define FORCE_REMOVE		        924		/** This Macro gives ascii value of string "FORCE_REMOVE"*/
#define LEARN_MODE 		        758
#define SECURITY		        632     /** THis Macro gives ascii value of "security"*/
#define FOTA_REQ			625

#define DM_VA_SERVICE_DM	        "11"
#define DM_SS_SERVICE_DM	        "12"
#define DM_MA_SERVICE_DM	        "13"
#define DM_MQTT_SERVICE_DM      	"14"
#define DM_SERVICE_DM		        "15"

#define DM_AREA				"77"
#define DM_RULE				"88"
#define DM_SCENE			"99"

#define DM_SWITCH_DM		        "SWITCH"
#define DM_RANGE_DM			"RANGE"
#define DM_COLOR_DM			"COLOR"
#define DM_LIST_DM			"LIST"
#define DM_INFO_DM			"INFO"

#define DM_AREA_CONTROL		        "AREA"
#define DM_SCENE_CONTROL	        "SCENE"
#define DM_VOICE_SCENE_CONTROL	        "VOICE_SCENE"	/** This Macro used to differentiate the scene request from Voice or Mobile */
#define DM_RULE_COLTROL		        "RULE"

#define DM_DISCOVERY 	        	"DISCOVERY"
#define DM_CONTROL		  	"CONTROL"
#define DM_OBSERVE 			"OBSERVE"
#define DM_REMOVE 			"REMOVE"
#define DM_THING_LEFT		        "THING_LEFT"
#define DM_EXCLUSION		        "EXCLUSION"
#define DM_FACTORY_RESET	        "FACTORY_RESET"
#define DM_IOT_RESET		        "IOT_RESET"
#define DM_THINGSLIST		        "THINGSLIST"
#define DM_S2_SECURITY		        "S2_SECURITY"
#define DM_FORCE_REMOVE  	        "FORCE_REMOVE"
#define DM_LEARN_MODE				"LEARN_MODE"
#define DM_SECURITY		        "SECURITY"

#define SMART_PLUG			"Smart Plug"
#define SWITCH_BOARD			"SwitchBoard"
#define CONTACT_DETECT 		        "Contact Detect"
#define COVERING 			"Covering"
#define DOOR_STATE			"Door State"
#define DIMMER				"Dimmer"
#define BRIGHTNESS			"Brightness"
#define ONOFF				"OnOff"
#define DOOR_LOCK			"Door Lock"

#define ALARM 				"Alarm"
#define TAMPER 				"Tamper"
#define BATTERY				"Battery"
#define BATTERY_DEFECT		        "Battery Defect"
#define OCCUPANCY			"Occupancy"
#define LUMINANCE			"Luminance"
#define SMOKE_DETECT		        "Smoke Detect"
#define CONTACT_DETECT		        "Contact Detect"
#define STATE			 	"State"
#define ON_OFF			 	"OnOff"
#define DOOR_STATE			"Door State"
#define BRIGHTNESS			"Brightness"
#define COLORS				"Color"
#define DOOR_STATE 			"Door State"
#define HUMIDITY			"Humidity"
#define TEMPERATURE			"Temperature"
#define	FANSPEED 			"Fan Speed"
#define	COLOR_TEMPERATURE	        "Color Temperature"
#define	COVERING 			"Covering"
#define	COVERING_STATUS		        "Covering Status"
#define GAS_DETECT			"Gas Detect"
#define DM_SENSOR                       "Sensor"

#define DM_VALUE_1			"1"
#define DM_VALUE_0			"0"
#define DM_VALUE_100		        "100"

#define DM_MQ_1 			 1

#define DM_OBSERVE_MSG_ID 					15111

#define DM_SENSOR_BASED_RULE 			        	1
#define DM_TIME_BASED_RULE 					2
#define DM_SENSOR_TIME_BASED_RULE_START	 	                3
#define DM_SENSOR_TIME_BASED_RULE_END 	                	4

#define PRIORITY_S2SECURITY			1
#define PRIORITY_DISCOVERY			2
#define PRIORITY_CONTROL			3
#define PRIORITY_REMOVE				4
#define PRIORITY_FORCE_REMOVE		        4
#define PRIORITY_EXCLUSION			4
#define PRIORITY_SCENE				5
#define PRIORITY_RULE				6
#define PRIORITY_FOTA				9

#define Requested_Thing_Data_Good 0
#define Entity_id_wrong  1
#define property_type_wrong 2
#define property_name_wrong 3
#define property_value_wrong 4

#define REQUEST_SUCCESS   			 0
#define DISCOVERY_ALREADY_PROCESS   		 2
#define EXECLUSION_ALREADY_PROCESS        	 4
#define REMOVE_ALREADY_PROCESS        	 	 32
#define CONTROL_ALREADY_PROCESS        	 	 64
#define SCENE_ALREADY_PROCESS			 8
#define RULE_ALREADY_PROCESS 			 16
#define REQUEST_QUEUE_SIZE_OVERFLOW		128
#define FOTA_IN_PROGRESS			256

typedef struct {
	json_object* iot_action;
	json_object* non_iot_action;
	std::string RuleSceneID;
	int Sceneflag;
	bool RuleSceneExecuteFlag;
}SecneRuleExecution ;

typedef struct  {
	json_object* allResponse;
	std::string DMFinalResponseData;
	int total_count;
	int response_success_count;
	int response_fail_count;
}DMFinalResMessage;

typedef struct {
	std::string Command;
	std::string Message_Id;
	std::string Message;
	std::string Notif_Id;
	std::string Thing_Id;
	std::string Entity_Id;
	std::string Property_Type;
	std::string Property_Name;
	std::string Property_Range;
	std::string Property_Value;
	std::string RuleSceneData;
	std::string Thing_Response;
	std::string Req_URI;
	std::string Req_Type;
	std::string queue_id;
	std::string Thing_Type;
	std::string Firmware_path;
	std::string Firmware_version;
	int priority;
	int Status_Code;
	int Client;
	int RequestCount;
	bool SendFlag;
} RequestResponse;

typedef struct {
	int RequestCount;
	int ResponseCount;
}RSARequestStruct ;

typedef struct {
	std::string ResponseURI;
	std::string ResponseMethod;
	std::string ReponseData;
	std::string Thing_ID;
	std::string queue_id;
	std::string Command;
	int priority;
}ReqMessageData;

//firmware table
struct FirmwareDB {
	std::string THING_ID;
	std::string FIRMWARE_VERSION;
	int HARDWARE_VERSION;
	std::string MANUFACTURER_NAME;
	int MODEL_ID;
};

typedef struct  {
	std::string THING_ID;
	std::string FIRMWARE_VERSION;
	std::string FRAMEWORK;
	std::string HARDWARE_VERSION;
	std::string MANUFACTURER_NAME;
	std::string MANUFACTURER_URL;
	std::string THING_TYPE;
	std::string THING_NAME;
	std::string THING_STATUS;
	std::string BATTERY_STATUS;
	std::string ZONE_TYPE;
	std::string HOME_NODE_ID;
}DMThingDB;

typedef struct  {
	std::string ENTITY_ID;
	std::string ENTITY_NAME;
	std::string ENTITY_TYPE;
	int SECURITY_LEVEL;
	std::string THING_ID;
}DMEntityDB;

typedef struct  {
	std::string PROPERTY_VALUE;
	std::string PROPERTY_TYPE;
	std::string PROPERTY_NAME;
	std::string PROPERTY_RANGE;
	std::string ENTITY_ID;
	std::string THING_ID;
}DMPropertyDB;

struct MessageIndex {
	std::string Thing_Id;
	std::string queue_id;
	int count;
	bool repeatAllowedStatus;
};

typedef struct {
	int Red = 0;					//!< red
	int Green = 0;					//!< green
	int Blue = 0;					//!< blue
} RGBVALUE;

typedef struct {
	uint16_t X;					//!< CIE color X
	uint16_t Y;					//!< CIE color Y
} XYVALUE;

//--------------------------RESPOMSE STRUCTURE START----------------------------------

#define IEEE_ADDRESS_LENGTH 8
#define NUMBER_OF_ENDPOINTS 	10
#define NUMBER_OF_CLUSTERS 		6
#define NUMBER_OF_ATTRIBUTES 	6
#define NUMBER_OF_COMMANDS 		5
#define MAX_COUNT 					10
typedef struct {
	uint16_t attributeId;
	int attributeVal;
	uint8_t attributeType;
} Attributedata;

typedef struct {
	uint16_t clusterId;
	uint8_t cmdId[NUMBER_OF_COMMANDS];
	uint8_t cmdCount;
	uint8_t attributeCount;
	Attributedata attributes[NUMBER_OF_ATTRIBUTES];
} Cluster;

typedef struct {
	uint8_t endpointId;
	uint16_t deviceId;
	uint8_t clusterCount;
	Cluster cluster[NUMBER_OF_CLUSTERS];
} DefinedEndpoint;

typedef struct {
	long mtype;
	uint8_t Mode;
	uint32_t MsgId;
	uint16_t ErrorCode;
	uint16_t node_id;
	uint64_t IEEE;
	uint8_t endpointCount;
	uint8_t ManFacturererName[32];
	uint8_t DoorLockPIN[8];
	DefinedEndpoint Endpoint[NUMBER_OF_ENDPOINTS];
} DMRespStructZB;							//!< Zigbee response structure

//--------------------------RESPOMSE STRUCTURE END----------------------------------

//--------------------------REQUEST STRUCTURE START----------------------------------

typedef struct {
	uint16_t attributeId;
	uint16_t argtype; //1=u 2=v
	union {
		int intargValue;
		char str[MAX_COUNT];
	} argValue;
} argValueStruct;

typedef struct {

	long mtype;
	uint32_t MsgId;
	uint8_t Mode;
	uint16_t timeout;
	uint16_t nodeId; // short id of specific device.
	uint64_t IEEE;
	uint8_t endpoint; // destination endpoint.
	uint16_t deviceId;
	uint16_t clusterId; // cluster id.
	uint16_t commandId; // command id .
	uint16_t argcount; // no of count of datatypes.
	argValueStruct argValue_info[3]; // value of each argument with some delimanator.
	uint16_t ErrorCode;
} DMReqStructZB;					//!< Device Manager Request structure

//--------------------------REQUEST STRUCTURE END---------------------------------

/**
 * Static function to convert hex to decimak
 * @param hex : hexadeciumal input
 * @return decimal number
 */
//static int HexadecimalToDecimal(std::string hex);

/**
 * Function to convert hex value to RGBVALUE structure
 * @param hex : hexadecimal number
 * @return RGBVALUE structure
 */
RGBVALUE convertHEXAtoRGB(std::string );

/**
 * Function to convert decimal to hexadecimal number
 * @param dec : decimal number
 * @return string return
 */
std::string DecimalToHexadecimal(int );

/**
 * Function to convert hexadecimal string array
 * @param R : red
 * @param G : green
 * @param B : blue
 * @return string to hexadecimal array
 */
std::string convertRGBtoHEXA(int , int , int );

/**
 * Function to convertRGB value to CIE XYY value
 * @param R : red
 * @param G : green
 * @param B : blue
 * @return XY value structure
 */
XYVALUE convertRGBtoXY(uint16_t , uint16_t , uint16_t );

/**
 * Function to convert CIE XY to RGB
 * @param x : CIE X color
 * @param y : CIE Y color
 * @return RGB value structure
 */
RGBVALUE convertXYtoRGB(float , float );

long long CurrentZWReqTime();

int ConvertStringToInt(std::string);

#endif /* INCLUDE_DM_UTILITY_HPP_ */
