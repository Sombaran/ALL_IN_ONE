/*
 * File_name:ip_camera_framework.hpp
 * @Brief: Inherited class to driver IP camera 
 * Created on: 03-Dec-2018
 * Author: KRISHNA
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef INCLUDE_IP_CAMERA_FRAMEWORK_HPP_
#define INCLUDE_IP_CAMERA_FRAMEWORK_HPP_

#include "dm_framework.hpp"
#include <pthread.h>
#include <json-c/json.h>

extern "C" {
extern json_object* FoscamSearch(int);
extern bool RemoveFoscam(const char *);
}

class IpCameraFramework: public DmFramework {
public:
	IpCameraFramework() {}
	void discovery(RequestResponse);
	void control(RequestResponse);
	void remove(RequestResponse);
	void exclusion(RequestResponse);
	void s2security(RequestResponse);
	void forceremove(RequestResponse);
	void learnmode(RequestResponse);
	void zigbeefota(RequestResponse );
	virtual ~IpCameraFramework() {}
};



#endif /* INCLUDE_IP_CAMERA_FRAMEWORK_HPP_ */
