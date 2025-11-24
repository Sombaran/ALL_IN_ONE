/*
 * File_name:dm_led_interface.hpp
 * @Brief: Driver class to enable LED interfacing functionalities
 * Created on: 02-Aug-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef INCLUDE_DM_LED_INTERFACE_HPP_
#define INCLUDE_DM_LED_INTERFACE_HPP_

#include <iostream>

#define LED_STATE_0 0
#define LED_STATE_1 1
#define LED_VALUE 5

#define LED_FAIL	10
#define LED_SUCCESS 12

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "shifu-led.h"
#include <sys/syslog.h>

#ifdef __cplusplus
}
#endif

class DmLedInterface {

public:
	DmLedInterface() {}
	static bool DiscoveryStart();
	static bool DiscoveryStop();
	static bool DeviceConnected();
	static bool DeviceDisConnected();
	 ~DmLedInterface() {}
};

int shifu_led_init();

#endif /* INCLUDE_DM_LED_INTERFACE_HPP_ */
