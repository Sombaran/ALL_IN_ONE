/*
 * File_name: device_manager_gateway.cpp
 * @Brief: It is the main file which initializes all the threads, lED, Databases
 * Created on: 26-Jul-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#include <iostream>
#include <thread>
#include <sys/syslog.h>
#include "dm_platform.hpp"
#include "dm_led_interface.hpp"
#include "fw-env.h"
extern void AudioPlayer();

/*
 * 1 ARM
 * 2 LINUX
 * */

int Led_Socket = 0;

int main() {

	srand(time(NULL));
	Led_Socket = shifu_led_init();									//!< Initializing Led fucnionalities
	DmPlatform::DeviceManagerInitialize();							//!< Initializing DM  environment parameters
	DmPlatform::DeviceManagerDatabase();							//!<
	std::thread DmFotaZb(DmPlatform::DmFotaclient);					//!< Handles request from FOTA module
	std::thread DmMqZb(DmPlatform::DmZigbeeReceive);				//!< Handles response from the Zigbee module
	std::thread DmMqZw(DmPlatform::DmZwaveReceive);					//!< Handles response from the Zwave module
	std::thread DmReqQueue(DmPlatform::DmProcessIOTRequest);		//!< Handles the incoming request to Dm from WS, Cron and MA
	std::thread DmResQueue(DmPlatform::DmProcessIOTResponse);
	std::thread DmSendQueue(DmPlatform::DmProcessTronxResponse);
	std::thread DmZeroMq(DmPlatform::DmReceiveRequest);
	std::thread Aud(AudioPlayer);									//!< Audioplayer Initialization thread
	DmPlatform::DeviceManagerFetchScenesRules();
	DmPlatform::TronxRemoveThingSync();								//!< TronxRemoveThingSync used to sync/remove thing from TronX which are not updated
	DmZeroMq.join();
	DmFotaZb.join();
	DmSendQueue.join();
	DmResQueue.join();
	DmReqQueue.join();
	DmMqZw.join();
	DmMqZb.join();
	Aud.join();
	return 0;
}
