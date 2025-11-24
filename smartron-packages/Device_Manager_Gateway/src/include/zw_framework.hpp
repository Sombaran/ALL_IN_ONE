/*
 * File_name:zw_framework.hpp
 * @Brief: ZwFramework is inherited from the DmFramework, and declaring all the functions which are performed in between Devicemanager to Zwave
 * Created on: 26-Jul-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef INCLUDE_ZW_FRAMEWORK_HPP_
#define INCLUDE_ZW_FRAMEWORK_HPP_
#include <iostream>
#include "dm_framework.hpp"
#include <json-c/json.h>
#include <string>
#include <pthread.h>

#define DM_ZW_ACT_EVENT 	0			//! Value is set to zero for Events like DOOR_OPEN,GAS_NOT DECTED
#define ZW_CLS_113 			113			//! Command class for Door_LOCK
#define ZW_CLS_128 			128			//! Command class for Battery

#define ZW_VALUE_99			99			//! Value to send it Zwave_frame work to make the bulb ON
#define ZW_VALUE_0			0			//! Value to send it Zwave_frame work to make the bulb OFF
#define ZW_Sensor_timeout	90			//! Timeout maintained to send undetected response

//22015: Structure to maintain the thread-details for occupancy sensors
typedef struct {

	pthread_t tid;
	pthread_mutex_t mut;
	pthread_cond_t cond;
	int response_done;
	std::string thing_id;

}Sensor_response;

class ZwFramework: public DmFramework {
public:
	/*
	 * @Brief: Default constructor
	 * */
	ZwFramework() {}

	/*
	 * @Brief: Message Queue initialization in order to communicate with Zwave framework
	 * @return True if it is success or False it the initialization is failed
	 * */
	static bool DMMessageQueueInit();

	/*
	 * @Brief: Function for receiving the zwave response
	 * */
	static void ZwReceive();

	/*
	 * @Brief:Fuction to stop the discovery
	 * */
	static void stopdiscovery(void);


	/*
	 * @Brief: Function to send the discovery request to Zwave framework
	 * @param: RequestResponse structure created in  dm_utility.hpp
	 * */
	void discovery(RequestResponse);

	/*
	 * @Brief: Function to send the Control request to Zwave framework
	 * @param: RequestResponse structure created in  dm_utility.hpp
	 * */
	void control(RequestResponse);

	/*
	 *	@Brief: Function to send the remove request to Zwave framework
	 *	@param: RequestResponse structure created in  dm_utility.hpp
	 * */
	void remove(RequestResponse);

	/*
	 * @Brief: Function to send the exclusion request to Zwave framework
	 * @param: RequestResponse structure created in  dm_utility.hpp
	 * */
	void exclusion(RequestResponse);

	/*
	 * @Brief: Function to send the S2_SEcurity  request to Zwave framework
	 * @param: RequestResponse structure created in  dm_utility.hpp
	 * */
	void s2security(RequestResponse);

	/*
	 * @Brief: Function to send the Forceremove request to Zwave framework
	 * @param: RequestResponse structure created in  dm_utility.hpp
	 * */
	void forceremove(RequestResponse);

	/*
	 * @Brief: Function to send the learnmode request to Zwave framework
	 * @param: RequestResponse structure created in  dm_utility.hpp
	 * */
	void learnmode(RequestResponse);

	/*
	 * @Brief: Since the zigbeefota is pure virtuial function so that we have to declare in ZwaveFramework
	 * @param: RequestResponse structure created in  dm_utility.hpp
	 * */
	void zigbeefota(RequestResponse );

	/*
	* @Brief: Function to destroy the Zwframework and DmFramework
	* */
	virtual ~ZwFramework() {}
};

#endif /* INCLUDE_ZW_FRAMEWORK_HPP_ */
