#include "dm_framework.hpp"
#include "dm_thingmap.hpp"
#include "zb_framework.hpp"
#include "zw_framework.hpp"
#include "ip_camera_framework.hpp"
#include "dm_req_res_mapping.hpp"

std::map<std::string, DmFramework*> ReqHandleMap;
pthread_mutex_t LockMutexDiscStop = PTHREAD_MUTEX_INITIALIZER;

std::mutex LockControlFunction;

/*DmFrameworkHandle::DmFrameworkHandle() {
}
DmFrameworkHandle::~DmFrameworkHandle() {
}*/

bool DmFrameworkHandle::StopDiscReqHandleThread(std::string ReqHandleMapID,
		std::string StackName) {

	pthread_mutex_lock(&LockMutexDiscStop);

	/*syslog(LOG_DEBUG,
			"[DM]: Framework request handler map discovery stop request handle thread map id size:[%ld] for id [%s]",
			ReqHandleMap.size(), ReqHandleMapID.c_str());*/

	bool ReqHandleFlag = false;

	std::map<std::string, DmFramework*>::iterator ReqHandleMapItrZB =
			ReqHandleMap.find(std::string("ZB") + ReqHandleMapID);

	if (ReqHandleMapItrZB != ReqHandleMap.end()) {

		ReqHandleMapItrZB->second->stop(ReqHandleMapItrZB->first);

		ReqHandleMap.erase(ReqHandleMapItrZB->first);

		syslog(LOG_DEBUG,
				"[DM]: Dm_framework zigbee discovery thread removal success for id:[%s]",
				ReqHandleMapItrZB->first.c_str());
		if (StackName != "ZB") {
			ZbFramework::stopdiscovery();
		}
		ReqHandleFlag = true;

	} else {

		syslog(LOG_DEBUG,
				"[DM]: Dm_framework zigbee discovery thread removal failed for id:[%s]",
				(std::string("ZB") + ReqHandleMapID).c_str());
	}

	std::map<std::string, DmFramework*>::iterator ReqHandleMapItrZW =
			ReqHandleMap.find(std::string("ZW") + ReqHandleMapID);
	if (ReqHandleMapItrZW != ReqHandleMap.end()) {

		ReqHandleMapItrZW->second->stop(ReqHandleMapItrZW->first);

		ReqHandleMap.erase(ReqHandleMapItrZW->first);

		syslog(LOG_DEBUG,
				"[DM]: Dm_framework zwave discovery thread removal success for id:[%s]",
				ReqHandleMapItrZW->first.c_str());
		if (StackName != "ZW") {
			ZwFramework::stopdiscovery();
		}
		ReqHandleFlag = true;

	} else {
		syslog(LOG_DEBUG,
				"[DM]: Dm_framework zwave discovery thread removal failed for id:[%s]",
				(std::string("ZW") + ReqHandleMapID).c_str());
	}

	pthread_mutex_unlock(&LockMutexDiscStop);

	if (ReqHandleFlag == true) {

		/*syslog(LOG_DEBUG,
				"[DM]: Framework request handler map discovery stop request handle thread map id size success:[%ld] for id [%s]",
				ReqHandleMap.size(), ReqHandleMapID.c_str());*/

		return true;
	} else {
		/*syslog(LOG_DEBUG,
				"[DM]: Framework request handler map discovery stop request handle thread map id size fail:[%ld] for id [%s]",
				ReqHandleMap.size(), ReqHandleMapID.c_str());*/
		return false;
	}
	return false;
}

std::string DmFrameworkHandle::PropertyNameStopReqHandleThread(
		std::string ReqHandleMapID) {

	LockControlFunction.lock();

	for (std::map<std::string, DmFramework*>::iterator ReqHandleMapItr1 =
			ReqHandleMap.begin(); ReqHandleMapItr1 != ReqHandleMap.end();
			++ReqHandleMapItr1) {

		/*syslog(LOG_DEBUG,
				"[DM]: Framework request handler control map size : [%ld] for request message id :[%s] MAP message id :[%s] propertyName :[%s]",
				ReqHandleMap.size(), ReqHandleMapID.c_str(),
				ReqHandleMapItr1->first.c_str(),
				ReqHandleMapItr1->second->Property_Name.c_str());*/
	}

	/*syslog(LOG_DEBUG,
			"[DM]: Framework request handler map size : [%ld] for request message id :[%s]",
			ReqHandleMap.size(), ReqHandleMapID.c_str());*/

	std::map<std::string, DmFramework*>::iterator ReqHandleMapItr =
			ReqHandleMap.find(ReqHandleMapID);
	if (ReqHandleMapItr != ReqHandleMap.end()) {

		std::string Property_Name = ReqHandleMapItr->second->Property_Name;

		/*syslog(LOG_DEBUG,
				"[DM]: Framework request handler map size success :[%ld] before for request message id :[%s] return property name : [%s]",
				ReqHandleMap.size(), ReqHandleMapID.c_str(),
				Property_Name.c_str());*/

		ReqHandleMapItr->second->stop(ReqHandleMapID);

		ReqHandleMap.erase(ReqHandleMapID);

		/*syslog(LOG_DEBUG,
				"[DM]: Framework request handler map size success :[%ld] after for request message id :[%s] return property name : [%s]",
				ReqHandleMap.size(), ReqHandleMapID.c_str(),
				Property_Name.c_str());*/

		LockControlFunction.unlock();
		return Property_Name;

	} else {
		syslog(LOG_DEBUG,
				"[DM]: Dm_framework failed to get property name:[%zu] for id [%s]",
				ReqHandleMap.size(), ReqHandleMapID.c_str());
		LockControlFunction.unlock();
		return "DM_DEFAULT";
	}

	LockControlFunction.unlock();
	return "DM_DEFAULT";
}

bool DmFrameworkHandle::StopReqHandleThread(std::string ReqHandleMapID) {

	for (std::map<std::string, DmFramework*>::iterator ReqHandleMapItr1 =
			ReqHandleMap.begin(); ReqHandleMapItr1 != ReqHandleMap.end();
			++ReqHandleMapItr1) {

		/*syslog(LOG_DEBUG,
				"[DM]: Framework request handler stop map size : [%ld] for request message id :[%s] MAP message id :[%s] propertyName :[%s]",
				ReqHandleMap.size(), ReqHandleMapID.c_str(),
				ReqHandleMapItr1->first.c_str(),
				ReqHandleMapItr1->second->Property_Name.c_str());*/
	}

	/*syslog(LOG_DEBUG,
			"[DM]: Framework request handler map stop request handle thread map id size:[%ld] for id [%s]",
			ReqHandleMap.size(), ReqHandleMapID.c_str());*/

	std::map<std::string, DmFramework*>::iterator ReqHandleMapItr =
			ReqHandleMap.find(ReqHandleMapID);
	if (ReqHandleMapItr != ReqHandleMap.end()) {

		/*syslog(LOG_DEBUG,
				"[DM]: Framework request handler map stop request handle thread map id size success:[%ld] for id [%s]",
				ReqHandleMap.size(), ReqHandleMapID.c_str());*/

		ReqHandleMapItr->second->stop(ReqHandleMapID);

		ReqHandleMap.erase(ReqHandleMapID);

		return true;
	} else {
		syslog(LOG_DEBUG,
				"[DM]: Dm_framework to stop request handler thread failed :[%zu] for id [%s]",
				ReqHandleMap.size(), ReqHandleMapID.c_str());
		return false;
	}

	return false;
}

bool DmFrameworkHandle::RemoveReqHandleMapId(std::string ReqHandleMapID) {

	/*syslog(LOG_DEBUG,
			"[DM]: Framework request handler map remove request handle map id size:[%ld] for id [%s]",
			ReqHandleMap.size(), ReqHandleMapID.c_str());*/

	std::map<std::string, DmFramework*>::iterator ReqHandleMapItr =
			ReqHandleMap.find(ReqHandleMapID);
	if (ReqHandleMapItr != ReqHandleMap.end()) {

		/*TODO delete the object from MAP*/
		//delete (ReqHandleMapItr->second);
		ReqHandleMap.erase(ReqHandleMapID);

		/*syslog(LOG_DEBUG,
				"[DM]: Framework request handler map remove request handle map id size success:[%ld] for id [%s]",
				ReqHandleMap.size(), ReqHandleMapID.c_str());*/

		return true;
	} else {
		/*syslog(LOG_DEBUG,
				"[DM]: Framework request handler map remove request handle map id size fail:[%ld] for id [%s]",
				ReqHandleMap.size(), ReqHandleMapID.c_str());*/
		return false;
	}

	return false;
}

void DmFrameworkHandle::DiscoveryStart(RequestResponse RequestResponseOBJ) {

	syslog(LOG_INFO, "[DM]: Dm_framework /' DISCOVERY zigbee , zwave & ipcamera /' start");
	DmFramework* DmFrameworkZb = new ZbFramework;
	DmFramework* DmFrameworkZw = new ZwFramework;
	DmFramework* DmFrameworkIpCamera = new IpCameraFramework;
//	std::unique_ptr <DmFramework> *DmFrameworkZb = std::make_unique <DmFramework>();
//	 std::shared_ptr <DmFramework>  DmFrameworkZb = std::make_shared <DmFramework>();
	ReqHandleMap[std::string("ZB") + RequestResponseOBJ.Message_Id] =
			DmFrameworkZb;
	std::thread ZBdiscovery(&DmFramework::discovery, DmFrameworkZb,
			RequestResponseOBJ);
	ReqHandleMap[std::string("ZW") + RequestResponseOBJ.Message_Id] =
			DmFrameworkZw;
	std::thread ZWdiscovery(&DmFramework::discovery, DmFrameworkZw,
			RequestResponseOBJ);
	std::thread IpCameradiscovery(&DmFramework::discovery, DmFrameworkIpCamera,
			RequestResponseOBJ);
	DmReqResMapping* DmReqResMappingOBJ = DmReqResMapping::GetInstance();
	DmReqResMappingOBJ->AddToDmReqResMapping(RequestResponseOBJ.Notif_Id,
			RequestResponseOBJ.Message_Id, RequestResponseOBJ.RequestCount);
	ZBdiscovery.detach();
	ZWdiscovery.detach();
	IpCameradiscovery.detach();
}

bool DmFrameworkHandle::ControlStart(RequestResponse RequestResponseOBJ) {
	/**locking resource to limit access by any another thread*/
	LockControlFunction.lock();

	syslog(LOG_INFO, "[DM]: Dm_framework /' CONTROL zigbee & zwave /' start");

	syslog(LOG_DEBUG,
			"[DM]: Framework request print = client: [%d] command: [%s] entity_id: [%s] message_id: [%s] message: [%s] notif_id: [%s] property_name: [%s] property_type: [%s] property_value: [%s] proeporty_range: [%s] rulescenedata: [%s] status_code: [%d] thing_id: [%s]",
			RequestResponseOBJ.Client, RequestResponseOBJ.Command.c_str(),
			RequestResponseOBJ.Entity_Id.c_str(),
			RequestResponseOBJ.Message_Id.c_str(),
			RequestResponseOBJ.Message.c_str(),
			RequestResponseOBJ.Notif_Id.c_str(),
			RequestResponseOBJ.Property_Name.c_str(),
			RequestResponseOBJ.Property_Type.c_str(),
			RequestResponseOBJ.Property_Value.c_str(),
			RequestResponseOBJ.Property_Range.c_str(),
			RequestResponseOBJ.RuleSceneData.c_str(),
			RequestResponseOBJ.Status_Code,
			RequestResponseOBJ.Thing_Id.c_str());

	DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

	DmFramework* DmFrameworkControl;

	//syslog(LOG_INFO, "[DM]: Framework control start");

	/**checking if thing exist in local map or not*/
	std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
			DmThingMapObj->DmThingDataMap.find(RequestResponseOBJ.Thing_Id);

	if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {

		/*syslog(LOG_INFO,
				"[DM]: Framework thing control entity searching for: [%s]",
				RequestResponseOBJ.Entity_Id.c_str());

		syslog(LOG_INFO, "[DM]: Framework control found for: [%s]",
				RequestResponseOBJ.Thing_Id.c_str());*/

		DmReqResMapping* DmReqResMappingOBJ = DmReqResMapping::GetInstance();

		DmReqResMappingOBJ->AddToDmReqResMapping(RequestResponseOBJ.Notif_Id,
				RequestResponseOBJ.Message_Id, RequestResponseOBJ.RequestCount);

		if (DmThingMapObjItr->second->getframeworkName() == "zigbee") {

			DmFrameworkControl = new ZbFramework;

			/*syslog(LOG_INFO,
					"[DM]: Framework control ZB Request handle map size : [%d] before add for Message id : [%s] Thing id: [%s] property name : [%s]",
					ReqHandleMap.size(),
					(std::string("ZB") + RequestResponseOBJ.Message_Id).c_str(),
					RequestResponseOBJ.Thing_Id.c_str(),
					RequestResponseOBJ.Property_Name.c_str());*/

			/**Adding new request to ReqHandleMap*/
			ReqHandleMap[std::string("ZB") + RequestResponseOBJ.Message_Id] =
					DmFrameworkControl;

			DmFrameworkControl->Property_Name =
					RequestResponseOBJ.Property_Name;

			/*syslog(LOG_INFO,
					"[DM]: Framework control ZB Request handle map size : [%d] after add for Message id : [%s] Thing id: [%s] property name : [%s]",
					ReqHandleMap.size(),
					(std::string("ZB") + RequestResponseOBJ.Message_Id).c_str(),
					RequestResponseOBJ.Thing_Id.c_str(),
					RequestResponseOBJ.Property_Name.c_str());*/

			/**calling ZigBee control method*/
			std::thread ZBZWControl(&DmFrameworkControl->control,
					DmFrameworkControl, RequestResponseOBJ);

			ZBZWControl.detach();

			LockControlFunction.unlock();
			return true;

		} else if (DmThingMapObjItr->second->getframeworkName() == "zwave") {

			DmFrameworkControl = new ZwFramework;

			/*syslog(LOG_INFO,
					"[DM]: Framework control ZW Request handle map size : [%d] before add for Thing id: [%s] property name : [%s]",
					ReqHandleMap.size(), RequestResponseOBJ.Thing_Id.c_str(),
					RequestResponseOBJ.Property_Name.c_str());

			syslog(LOG_INFO,
					"[DM]: Framework control ZW Request handle map size : [%d] after add for Thing id: [%s] property name : [%s]",
					ReqHandleMap.size(), RequestResponseOBJ.Thing_Id.c_str(),
					RequestResponseOBJ.Property_Name.c_str());*/

			if (DmThingMapObjItr->second->getthing_status() == "online") {
				for (int EntityNo = 0;
						EntityNo < DmThingMapObjItr->second->getEntityListSize();
						EntityNo++) {

					if (DmThingMapObjItr->second->getEntityInfo(EntityNo)->getentity_id()
							== std::string(RequestResponseOBJ.Entity_Id)) {
						for (int PropertyNo = 0;
								PropertyNo
										< DmThingMapObjItr->second->getEntityInfo(
												EntityNo)->getPropertySize();
								PropertyNo++) {

							if (DmThingMapObjItr->second->getEntityInfo(
									EntityNo)->getPropertyInfo(PropertyNo)->getproperty_name()
									== std::string(
											RequestResponseOBJ.Property_Name)) {

								bool brightnesscheck = false;

								if (RequestResponseOBJ.Property_Name
										== BRIGHTNESS) { /** When we are getting previous brightness value, checking the OnOff state and processing the request */

									brightnesscheck =
											(DmThingMapObjItr->second->getEntityInfo(
													EntityNo)->getPropertyInfo(
													PropertyNo - 1)->getproperty_value()
													== "0");	/** This is to check the OnOff state */
								}

								if ((DmThingMapObjItr->second->getEntityInfo(
										EntityNo)->getPropertyInfo(PropertyNo)->getproperty_value()
										== std::string(
												RequestResponseOBJ.Property_Value)) && (!brightnesscheck)) {

									RequestResponseOBJ.Message =
											"Control completed successfully";
									RequestResponseOBJ.Status_Code = DM_SUCCESS;

									DmPlatform::DmSendingResponse(
											RequestResponseOBJ);

									LockControlFunction.unlock();
									return false;

								} else {
									ReqHandleMap[std::string("ZW")
											+ RequestResponseOBJ.Message_Id] =
											DmFrameworkControl;
									DmFrameworkControl->Property_Name =
											RequestResponseOBJ.Property_Name;
									std::thread ZBZWControl(
											&DmFrameworkControl->control,
											DmFrameworkControl,
											RequestResponseOBJ);
									ZBZWControl.detach();

									LockControlFunction.unlock();
									return true;
								}
								break;
							}
						}

					}

				}
			} else {
				/**zwave device is offline*/
				ReqHandleMap[std::string("ZW") + RequestResponseOBJ.Message_Id] =
						DmFrameworkControl;
				DmFrameworkControl->Property_Name =
						RequestResponseOBJ.Property_Name;
				std::thread ZBZWControl(&DmFrameworkControl->control,
						DmFrameworkControl, RequestResponseOBJ);
				ZBZWControl.detach();

				LockControlFunction.unlock();
				return true;
			}
		}

	} else {

		syslog(LOG_INFO, "[DM]: Framework control not found for: [%s]",
				RequestResponseOBJ.Thing_Id.c_str());

		LockControlFunction.unlock();
		return false;
	}

	LockControlFunction.unlock();
	return false;
}

bool DmFrameworkHandle::RemoveStart(RequestResponse RequestResponseOBJ) {

	DmThingMap* DmThingMapObj = DmThingMap::GetInstance();
	DmFramework* DmFrameworkControl;

	//syslog(LOG_INFO, "[DM]: Framework remove start");
	syslog(LOG_INFO, "[DM]: Framework remove start");
	if (RequestResponseOBJ.Thing_Id.compare(0, 2, "FC") == 0) {
		syslog(LOG_INFO, "[DM]: Foscam remove start");
		if (RemoveFoscam(RequestResponseOBJ.Thing_Id.substr(2).c_str())) {

			RequestResponseOBJ.Command = DM_REMOVE;
			RequestResponseOBJ.Message = "Remove completed successfully";
			RequestResponseOBJ.Status_Code = DM_SUCCESS;

			DmPlatform::DmSendingResponse(RequestResponseOBJ);

			syslog(LOG_INFO, "[DM]: Foscam remove success");
			return 0;
		}
	}
	std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
			DmThingMapObj->DmThingDataMap.find(RequestResponseOBJ.Thing_Id);
	if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {

		/*syslog(LOG_INFO, "[DM]: Framework remove found for: [%s]",
				RequestResponseOBJ.Thing_Id.c_str());*/

		if (DmThingMapObjItr->second->getframeworkName() == "zigbee") {
			DmFrameworkControl = new ZbFramework;

			/*syslog(LOG_INFO,
					"[DM]: Framework remove ZB Request handle map size : [%d] before add for Thing id: [%s] property name : [%s]",
					ReqHandleMap.size(), RequestResponseOBJ.Thing_Id.c_str(),
					RequestResponseOBJ.Property_Name.c_str());*/

			ReqHandleMap[std::string("ZB") + RequestResponseOBJ.Message_Id] =
					DmFrameworkControl;
			DmFrameworkControl->Property_Name =
					RequestResponseOBJ.Property_Name;

			/*syslog(LOG_INFO,
					"[DM]: Framework remove ZB Request handle map size : [%d] after add for Thing id: [%s] property name : [%s]",
					ReqHandleMap.size(), RequestResponseOBJ.Thing_Id.c_str(),
					RequestResponseOBJ.Property_Name.c_str());*/

		} else if (DmThingMapObjItr->second->getframeworkName() == "zwave") {
			DmFrameworkControl = new ZwFramework;

			/*syslog(LOG_INFO,
					"[DM]: Framework remove ZW Request handle map size : [%d] before add for Thing id: [%s] property name : [%s]",
					ReqHandleMap.size(), RequestResponseOBJ.Thing_Id.c_str(),
					RequestResponseOBJ.Property_Name.c_str());*/

			ReqHandleMap[std::string("ZW") + RequestResponseOBJ.Message_Id] =
					DmFrameworkControl;
			DmFrameworkControl->Property_Name =
					RequestResponseOBJ.Property_Name;
			/*syslog(LOG_INFO,
					"[DM]: Framework remove ZW Request handle map size : [%d] after add for Thing id: [%s] property name : [%s]",
					ReqHandleMap.size(), RequestResponseOBJ.Thing_Id.c_str(),
					RequestResponseOBJ.Property_Name.c_str());*/
		}

		std::thread ZWRemove(&DmFrameworkControl->remove, DmFrameworkControl,
				RequestResponseOBJ);

		DmReqResMapping* DmReqResMappingOBJ = DmReqResMapping::GetInstance();

		DmReqResMappingOBJ->AddToDmReqResMapping(RequestResponseOBJ.Notif_Id,
				RequestResponseOBJ.Message_Id, RequestResponseOBJ.RequestCount);

		ZWRemove.detach();
		return true;
	} else {

		syslog(LOG_INFO, "[DM]: For Remove, Thing ID not found: [%s]",
				RequestResponseOBJ.Thing_Id.c_str());
	}
	return false;
}

void DmFrameworkHandle::ExclusionStart(RequestResponse RequestResponseOBJ) {

	DmFramework* DmFrameworkZw = new ZwFramework;

	std::thread ZWExclusion(&DmFrameworkZw->exclusion, DmFrameworkZw,
			RequestResponseOBJ);

	/*syslog(LOG_INFO,
			"[DM]: Framework exclusion ZW Request handle map size : [%d] before add for Thing id: [%s] property name : [%s]",
			ReqHandleMap.size(), RequestResponseOBJ.Thing_Id.c_str(),
			RequestResponseOBJ.Property_Name.c_str());*/

	DmFrameworkZw->Property_Name = RequestResponseOBJ.Property_Name;

	ReqHandleMap[std::string("ZW") + RequestResponseOBJ.Message_Id] =
			DmFrameworkZw;

	/*syslog(LOG_INFO,
			"[DM]: Framework exclusion ZW Request handle map size : [%d] after add for Thing id: [%s] property name : [%s]",
			ReqHandleMap.size(), RequestResponseOBJ.Thing_Id.c_str(),
			RequestResponseOBJ.Property_Name.c_str());*/

	DmReqResMapping* DmReqResMappingOBJ = DmReqResMapping::GetInstance();

	DmReqResMappingOBJ->AddToDmReqResMapping(RequestResponseOBJ.Notif_Id,
			RequestResponseOBJ.Message_Id, RequestResponseOBJ.RequestCount);

	ZWExclusion.detach();

}

void DmFrameworkHandle::LearnModeStart(RequestResponse RequestResponseOBJ) {

	DmFramework* DmFrameworkZw = new ZwFramework;

	std::thread ZWExclusion(&DmFrameworkZw->learnmode, DmFrameworkZw,
			RequestResponseOBJ);

	/*syslog(LOG_INFO,
			"[DM]: Framework exclusion ZW Request handle map size : [%d] before add for Thing id: [%s] property name : [%s]",
			ReqHandleMap.size(), RequestResponseOBJ.Thing_Id.c_str(),
			RequestResponseOBJ.Property_Name.c_str());*/

	DmFrameworkZw->Property_Name = RequestResponseOBJ.Property_Name;

	ReqHandleMap[std::string("ZW") + RequestResponseOBJ.Message_Id] =
			DmFrameworkZw;

	/*syslog(LOG_INFO,
			"[DM]: Framework exclusion ZW Request handle map size : [%d] after add for Thing id: [%s] property name : [%s]",
			ReqHandleMap.size(), RequestResponseOBJ.Thing_Id.c_str(),
			RequestResponseOBJ.Property_Name.c_str());*/

	DmReqResMapping* DmReqResMappingOBJ = DmReqResMapping::GetInstance();

	DmReqResMappingOBJ->AddToDmReqResMapping(RequestResponseOBJ.Notif_Id,
			RequestResponseOBJ.Message_Id, RequestResponseOBJ.RequestCount);

	ZWExclusion.detach();

}


void DmFrameworkHandle::S2securityStart(RequestResponse RequestResponseOBJ) {

	DmFramework* DmFrameworkZw = new ZwFramework;

	std::thread ZWS2Security(&DmFrameworkZw->s2security, DmFrameworkZw,
			RequestResponseOBJ);

	syslog(LOG_INFO,"Framework starting S2securityStart");

	/*syslog(LOG_INFO,
			"[DM]: Framework S2securityStart ZW Request handle map size : [%d] before add for Thing id: [%s] property name : [%s]",
			ReqHandleMap.size(), RequestResponseOBJ.Thing_Id.c_str(),
			RequestResponseOBJ.Property_Name.c_str());*/

	DmFrameworkZw->Property_Name = RequestResponseOBJ.Property_Name;

	/*syslog(LOG_INFO,
			"[DM]: Framework exclusion ZW Request handle map size : [%d] after add for Thing id: [%s] property name : [%s]",
			ReqHandleMap.size(), RequestResponseOBJ.Thing_Id.c_str(),
			RequestResponseOBJ.Property_Name.c_str());*/

	ZWS2Security.detach();

}

void DmFrameworkHandle::ForceRemoveStart(RequestResponse RequestResponseOBJ) {
	DmFramework* DmFrameworkZw = new ZwFramework;

	std::thread ZWForceRemove(&DmFrameworkZw->forceremove, DmFrameworkZw,
			RequestResponseOBJ);

	syslog(LOG_INFO,
			"[DM]: Framework force remove ZW Request handle map size : [%zu] "
			"before add for Thing id: [%s] property name : [%s]",
			ReqHandleMap.size(), RequestResponseOBJ.Thing_Id.c_str(),
			RequestResponseOBJ.Property_Name.c_str());

	DmFrameworkZw->Property_Name = RequestResponseOBJ.Property_Name;

	ReqHandleMap[std::string("ZW") + RequestResponseOBJ.Message_Id] =
			DmFrameworkZw;

	syslog(LOG_INFO,
			"[DM]: Framework exclusion ZW Request handle map size : [%zu] after add for Thing id: [%s] property name : [%s]",
			ReqHandleMap.size(), RequestResponseOBJ.Thing_Id.c_str(),
			RequestResponseOBJ.Property_Name.c_str());

	DmReqResMapping* DmReqResMappingOBJ = DmReqResMapping::GetInstance();

	DmReqResMappingOBJ->AddToDmReqResMapping(RequestResponseOBJ.Notif_Id,
			RequestResponseOBJ.Message_Id, RequestResponseOBJ.RequestCount);

	ZWForceRemove.detach();
}
/* Function Name : ZigbeeFotaStart
 * Description   : This constructs the request forwarded to Zigbee module via MQ. Adds the request into Request-handle-map using message_Id.
 */
void DmFrameworkHandle::ZigbeeFotaStart(RequestResponse RequestResponseOBJ){

	DmFramework* ZBfota = new ZbFramework;

	std::thread ZigbeeUpgrade(&ZBfota->zigbeefota, ZBfota,RequestResponseOBJ);

	syslog(LOG_INFO,"ZB: FOTA -> Framework starting ZigbeeFotaStart");

	ZBfota->Property_Name = RequestResponseOBJ.Property_Name;

	ReqHandleMap[std::string("ZB") + RequestResponseOBJ.Message_Id] =
			ZBfota;			/* Adding the request_id into Request_handle map to keep track of processes. */

	DmReqResMapping* DmReqResMappingOBJ = DmReqResMapping::GetInstance();

	DmReqResMappingOBJ->AddToDmReqResMapping(RequestResponseOBJ.Notif_Id,
			RequestResponseOBJ.Message_Id, RequestResponseOBJ.RequestCount);

	ZigbeeUpgrade.detach();

}

