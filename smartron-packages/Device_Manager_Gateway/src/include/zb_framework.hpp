/*
 * File_name:zb_framework.hpp
 * @Brief: Inherited class to drive Zigbee funcitonalies
 * Created on: 26-Jul-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef INCLUDE_ZB_FRAMEWORK_HPP_
#define INCLUDE_ZB_FRAMEWORK_HPP_

#include <iostream>
#include "dm_framework.hpp"
#include <json-c/json.h>
#include <string>
#include <pthread.h>

#define BASIC_INFO			        	0
#define POWER_CONFG			        	1
#define IAS_ZONE			        	1280
        
#define ZERO_ZONE_ID 			                0

#define 	MODE_ID_HANDSHAKING			1
#define 	MODE_ID_NWKCREAT			2
#define 	MODE_ID_NWKLEAVE			3
#define 	MODE_ID_COMMISINING			4
#define 	MODE_ID_CONTROL				5
#define 	MODE_ID_OBSERVE				6
#define 	MODE_ID_REMOVE				7
#define 	MODE_ID_LEFT				8
#define 	MODE_ID_NODEID_CHANGE		        9
#define   	MODE_ID_STOP_COMMISINING	        10
#define		MODE_ID_ZB_DEVICE_INFO	                11
#define 	MODE_ID_TIMEOUT				12
#define 	MODE_ID_ZBFOTA				13

#define 	ONOFF_CLUSTER 				6
#define 	RANGE_CLUSTER 				8
#define 	COLOR_CLUSTER				768

#define 	RANGE_COMMAND 				4
#define 	COLOR_COMMAND				7

#define ZBALARM						0
#define ZBTAMPER					2
#define ZBBATTERY					3
#define ZBBATTERYDEFECT					9

#define TRANSMISSION_VALUE				0
#define DIMMABLE_BULB_DEVICE_ID	        		258

class ZbFramework: public DmFramework {
public:
	ZbFramework() {}
	static bool DMMessageQueueInitZB();
	static bool DMMessageQueueDiscInitZB();
	static void ZbReceive();
	static void stopdiscovery(void);
	void discovery(RequestResponse);
	void control(RequestResponse);
	void remove(RequestResponse);
	void exclusion(RequestResponse);
	void s2security(RequestResponse);
	void forceremove(RequestResponse);
	void learnmode(RequestResponse);
	void zigbeefota(RequestResponse );
	virtual ~ZbFramework() {}
};

#endif /* INCLUDE_ZB_FRAMEWORK_HPP_ */
