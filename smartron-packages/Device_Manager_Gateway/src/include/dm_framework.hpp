/*
 * File_name:dm_framework.hpp
 * @Brief: Driver class to operate all basic functionalities used by ZWAVE and ZIGBEE layers
 * Created on: 26-Jul-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef DM_FRAMEWORK_HPP_
#define DM_FRAMEWORK_HPP_

#include "dm_platform.hpp"
#include "dm_utility.hpp"
#include <iostream>
#include <json-c/json.h>
#include <string>
#include <string.h>
#include <assert.h>
#include <chrono>
#include <mutex>
#include <future>
#include <map>
#include <thread>
#include <time.h>
#include <sys/syslog.h>



class DmFramework {

	std::promise<void> exitSignal;
	std::future<void> futureObj;
public:

	DmFramework() :
			futureObj(exitSignal.get_future()) {

	}
	DmFramework(DmFramework && obj) :
			exitSignal(std::move(obj.exitSignal)), futureObj(
					std::move(obj.futureObj)) {
	}

	DmFramework & operator=(DmFramework && obj) {
		exitSignal = std::move(obj.exitSignal);
		futureObj = std::move(obj.futureObj);
		return *this;
	}

	// Thread function to be executed by thread
	void operator()() {
//		discovery(NULL);
//		control(NULL);
//		remove(NULL);
//		exclusion(NULL);
	}

	//Checks if thread is requested to stop
	bool stopRequested() {
		// checks if value in future object is available
		if (futureObj.wait_for(std::chrono::milliseconds(0))
				== std::future_status::timeout) {
			return false;
		} else {
			return true;
		}
	}
	// Request the thread to stop by setting value in promise object
	void stop(std::string ReqHandleMapID) {
		syslog(LOG_DEBUG, "[DM] : REQUESTHANDLER HANDLER STOPPED FOR ID: %s",
				ReqHandleMapID.c_str());
		exitSignal.set_value();
	}

	std::string Property_Name;
	virtual void discovery(RequestResponse)=0;
	virtual void control(RequestResponse)=0;
	virtual void remove(RequestResponse)=0;
	virtual void exclusion(RequestResponse)=0;
	virtual void s2security(RequestResponse)=0;
	virtual void forceremove(RequestResponse)=0;
	virtual void learnmode(RequestResponse)=0;
	virtual void zigbeefota(RequestResponse)=0;
	virtual ~DmFramework() {}
};

class DmFrameworkHandle: public DmFramework {

public:

	DmFrameworkHandle() {}
	static std::string PropertyNameStopReqHandleThread(std::string);
	static bool StopDiscReqHandleThread(std::string, std::string);
	static bool StopReqHandleThread(std::string);
	static bool RemoveReqHandleMapId(std::string);
	static void DiscoveryStart(RequestResponse);
	static bool ControlStart(RequestResponse);
	static bool RemoveStart(RequestResponse);
	static void ExclusionStart(RequestResponse);
	static void S2securityStart(RequestResponse);
	static void ForceRemoveStart(RequestResponse);
	static void LearnModeStart(RequestResponse);
	static void ZigbeeFotaStart(RequestResponse);
	virtual ~DmFrameworkHandle() {}

};

#endif /* DM_FRAMEWORK_HPP_ */
