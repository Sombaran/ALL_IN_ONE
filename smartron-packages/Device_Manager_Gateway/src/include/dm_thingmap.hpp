/*
 * File_name:dm_thingmap.hpp
 * @Brief: Inherited class from Thing_class to map the things details into map 
 * Created on: 26-Jul-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef INCLUDE_DM_THINGMAP_HPP_
#define INCLUDE_DM_THINGMAP_HPP_

#include <iostream>
#include <map>
#include <iterator>
#include "dm_thing.hpp"
#include "zw_framework.hpp"

class DmThingMap: public DmThing {

private:

	static DmThingMap *Instance;

	DmThing* DmThingControlObj;

public:
	DmThingMap() {}
	std::map<std::string, DmThing*> DmThingDataMap;
	std::map<std::string, std::string> DmMsgNotifyMap;
	std::map<std::string, DmThing*> DmThingDataCtrlObjMap;
	std::map<std::string,std::pair<RequestResponse,Sensor_response*>> UndetectMap;		//22015: Map to store the Req-Res Object and thread pointer
	static DmThingMap * GetInstance();
	DmThing* GetDmThingMapCtrlObj(std::string);
	bool SetDmThingMapCtrlObj(std::string, DmThing*);
	bool RemoveDmThingMapCtrlObj(std::string);
	bool CheckToThingMap(std::string);
	void AddToThingMap(std::string, DmThing*);
	void RemoveToThingMap(std::string);
	void PrintThingMap();
	 ~DmThingMap() {}
};

#endif /* INCLUDE_DM_THINGMAP_HPP_ */
