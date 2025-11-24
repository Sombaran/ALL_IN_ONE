#include "dm_thingmap.hpp"
#include "dm_things_backup_db.hpp"

/*DmThingMap::DmThingMap() {
}
DmThingMap::~DmThingMap() {
}*/

DmThingMap *DmThingMap::Instance = 0;

DmThingMap * DmThingMap::GetInstance() {
	if (Instance == 0) {
		Instance = new DmThingMap;
	}
	return Instance;
}

void DmThingMap::AddToThingMap(std::string ThingID, DmThing* ThingObj) {

	syslog(LOG_DEBUG,
			"[DM]: Thing map add to thing map thing added into thing map thing_id: [%s]",
			ThingID.c_str());

	DmThingDataMap[ThingID] = ThingObj;
}

void DmThingMap::RemoveToThingMap(std::string ThingID) {

	/*syslog(LOG_DEBUG,
			"[DM]: Thing map remove thing from main thing map for thing_id: [%s]",
			ThingID.c_str());*/

	DmThingDataMap.erase(ThingID);

	syslog(LOG_DEBUG,
			"[DM]: Thing map remove thing from main thing database for thing_id: [%s]",
			ThingID.c_str());

	DmThingsBackupDb* DmThingsBackupDbOBJ = DmThingsBackupDb::GetInstance();

	DmThingsBackupDbOBJ->DeleteDataBaseThing(ThingID);
}

bool DmThingMap::CheckToThingMap(std::string ThingID) {

	syslog(LOG_DEBUG,
			"[DM]: Thing map check to thing map thing checking into thing map thing_id: [%s]",
			ThingID.c_str());

	DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

	std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
			DmThingMapObj->DmThingDataMap.find(ThingID);
	if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {

		return true;
	} else {
		return false;
	}
	return false;
}

bool DmThingMap::SetDmThingMapCtrlObj(std::string Message_Id,
		DmThing* DmThingObj) {

	DmThingControlObj = DmThingObj;

	DmThingDataCtrlObjMap[Message_Id] = DmThingControlObj;

	/*syslog(LOG_INFO,
			"[DM]: Thing map thing control map setting control thing object");*/

	return true;
}

bool DmThingMap::RemoveDmThingMapCtrlObj(std::string Message_Id) {

	DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

	std::map<std::string, DmThing*>::iterator DmThingDataCtrlObjMapItr =
			DmThingMapObj->DmThingDataCtrlObjMap.find(Message_Id);
	if (DmThingDataCtrlObjMapItr != DmThingMapObj->DmThingDataMap.end()) {

		syslog(LOG_DEBUG,
				"[DM]: Thing to be removed found for message_id:[%s]",
				Message_Id.c_str());

		DmThingMapObj->DmThingDataCtrlObjMap.erase(Message_Id);
		return true;

	} else {
		syslog(LOG_DEBUG,
				"[DM]:Thing to be removed not found for message_id:[%s]",
				Message_Id.c_str());

		return false;
	}
	return false;
}

DmThing* DmThingMap::GetDmThingMapCtrlObj(std::string Message_Id) {

	DmThing* DmThingOBJ = new DmThing;

	/*syslog(LOG_INFO,
			"[DM]: Thing map thing control map getting control thing object");*/

	DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

	std::map<std::string, DmThing*>::iterator DmThingDataCtrlObjMapItr =
			DmThingMapObj->DmThingDataCtrlObjMap.find(Message_Id);
	if (DmThingDataCtrlObjMapItr != DmThingMapObj->DmThingDataMap.end()) {

		syslog(LOG_DEBUG,
				"[DM]: Thing map get thing map control object found for message_id:[%s]",
				Message_Id.c_str());

		return DmThingDataCtrlObjMapItr->second;

	} else {
		syslog(LOG_DEBUG,
				"[DM]: Thing map get thing map control object not found for message_id:[%s]",
				Message_Id.c_str());

		return DmThingOBJ;
	}
	return DmThingOBJ;
}

void DmThingMap::PrintThingMap() {

	for (std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
			DmThingDataMap.begin(); DmThingMapObjItr != DmThingDataMap.end();
			++DmThingMapObjItr) {
		syslog(LOG_DEBUG,
				"[DM]: Thing map check to thing map printing thing map thing_id:[%s]",
				DmThingMapObjItr->first.c_str());
		DmThingMapObjItr->second->Display();
	}
}
