/*
 * File_name: dm_platform.cpp
 * @Brief: This file is used to drive all the threads and functions in MAIN GATEWAY file
 * Created on: 26-Jul-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef INCLUDE_DM_PLATFORM_HPP_
#define INCLUDE_DM_PLATFORM_HPP_

#include <iostream>
#include <json-c/json.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <json-c/json.h>
#include <sys/syslog.h>
#include <sys/time.h>
#include "dm_utility.hpp"


const int SENDFLAG = 0;
class DmPlatform {

public:
	/*
	 * @Brief: Constructor
	 * */
	DmPlatform() {}
	//static void ServiceManagerInit();
	/*
	 * @Brief: Function to initialize all environment paramaters
	 * */
	static bool DeviceManagerInitialize();
	/*
	 * @Brief: Function to initialize the Database "DeviceManagerbackup.db"
	 * except for rules and scenes
	 * */
	static bool DeviceManagerDatabase();
	/*
	 * @Brief: Function to self trigger/Load rules and scenes
	 * */
	static void DeviceManagerFetchScenesRules();
	/*
	 * @Brief: Function to start Dm_zeromq receive thread
	 * std::thread DmZeroMq(DmPlatform::DmReceiveRequest)
	 * */
	static void DmReceiveRequest();
	/*
	 * @Brief: Function to start listen to zigbee response via IPC running as thread
	 * std::thread DmMqZb(DmPlatform::DmZigbeeReceive)
	 * */
	static void DmZigbeeReceive();
	/*
	 * @Brief: Function to start listen to Zwave response via IPC running as thread
	 * std::thread DmMqZw(DmPlatform::DmZwaveReceive)
	 * */
	static void DmZwaveReceive();
	/*
	 * @Brief: Function to start listen to process Fota requests running as thread
	 * std::thread DmFotaZb(DmPlatform::DmFotaclient)
	 * */
	static void DmFotaclient();
	/*
	 * @Brief:Function to put all incoming requests in request queue running as thread
	 * std::thread DmReqQueue(DmPlatform::DmProcessIOTRequest)
	 * */
	static void DmProcessIOTRequest();
	/*
	 * @Brief: Function to process the request from zibgee and zwave stack
	 * */
	static void DmProcessIOTResponse();
	/*
	 * @Brief: Function to process the request to the Tronx
	 * */
	static void DmProcessTronxResponse();
	/*
	 * @Brief: Function to update TronX by calling curl api to send,
	 * updated responses collected from zibgee and zwave stack via IPC
	 */
	static void DmSendingResponse(RequestResponse);
	/*
	 * @Brief: Function to send acknowledgement to the SS/MA/VA client on port 7001
	 * @param: RequestResponse structure created in  dm_utility.hpp
	 * */
	static void DmSendingRequestAck(RequestResponse);
	/*
	 * @Brief: Function to update TronX by calling curl api to send,
	 * updated responses collected from zibgee and zwave stack via IPC
	 * @param: RequestResponse structure created in  dm_utility.hpp
	 * */
	static void DmSendingRequestResp(RequestResponse);
	/*
	 * @Brief: Function to send response to the requested client on ports 8800,7006,7005
	 * @param: RequestResponse structure created in  dm_utility.hpp
	 * */
	static bool DMNodeIdUpdateInDB(std::string, std::string);
	/*
	 * @Brief: Funciton update the Node ID
	 * @param: Thing_ID, Node_id
	 * @return: true/ false on success / failure
	 * */
	static bool DMDeviceDetailsUpdate(std::string);
	/*
	 * @Brief: Function to update DeviceManagerBackUP.db and thingmap once
	 * response is received from zibgee and zwave stack via IPC
	 * @param: Thing_ID
	 * @return: true/ false on success / failure
	 * */
	static bool DMNetworkStatusSM();
	/*
	 * @Brief:Function to sync/remove thing/s from TronX which are not updated
	 * @return: true/ false on success / failure
	 * */
	static void TronxRemoveThingSync();
	/*
	 * @Brief: Destructor
	 * */
	 ~DmPlatform() {}
};

long long CurrentZWReqTime(); //! calculate time in milliseconds

#endif /* INCLUDE_DM_PLATFORM_HPP_ */

