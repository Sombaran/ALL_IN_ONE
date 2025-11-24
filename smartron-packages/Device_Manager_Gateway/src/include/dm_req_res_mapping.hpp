/*
 * File_name:dm_req_res_mapping.hpp
 * @Brief:Driver class to map all data during request and response 
 * Created on: 01-Aug-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef INCLUDE_DM_REQ_RES_MAPPING_HPP_
#define INCLUDE_DM_REQ_RES_MAPPING_HPP_

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <sys/syslog.h>
#include <unistd.h>
#include "dm_platform.hpp"

class DmReqResMapping {

private:

	static DmReqResMapping *Instance;

public:

	static DmReqResMapping *GetInstance();

	DmReqResMapping() {}
//	std::map<std::string, RequestResponse> CheckMessageMap;
	std::map<std::string, std::map<std::string, RequestResponse>> CheckNotifMap;
	std::map<std::string, RSARequestStruct> RSARequestMapping;
	std::map<std::string, DMFinalResMessage> DMFinalResponse;
	void AddToDmReqResMapping(std::string, std::string, int);
	RequestResponse CheckToDmReqResMapping(RequestResponse);
	void PrintDmReqResMapping(std::string );
	 ~DmReqResMapping() {}
};

#endif /* INCLUDE_DM_REQ_RES_MAPPING_HPP_ */
