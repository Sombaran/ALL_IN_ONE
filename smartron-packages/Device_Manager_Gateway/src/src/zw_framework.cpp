#include "zw_framework.hpp"
#include "dm_platform.hpp"
#include "zw_message_queue.hpp"
#include "zw_mapping_database.hpp"
#include "dm_thingmap.hpp"
#include "dm_led_interface.hpp"
#include "dm_utility.hpp"
#include "dm_things_backup_db.hpp"


extern bool FlagDiscSend;
extern bool s2securityflag;

extern char DM_DISCOVERY_TIMEOUT[8];
extern char DM_REMOVE_TIMEOUT[8];
extern char DM_CONTROL_TIMEOUT[8];
extern char DM_EXCLUSION_TIMEOUT[8];
bool smoke_flag;

extern pthread_mutex_t LockMutexDisc;
extern pthread_cond_t DM_Req_Control;

/*ZwFramework::ZwFramework() {
}
ZwFramework::~ZwFramework() {
}*/

bool ZwFramework::DMMessageQueueInit() {

	if ((ZwMessageQueue::ZwaveMsgReadTest() == true)
			&& (ZwMessageQueue::ZwaveMsgSendTest() == true)
			&& (ZwMappingDatabase::ZwaveDatabaseOpen() == true)) {
		syslog(LOG_INFO, "[DM]: Zwave Framework message queue success");
		return true;
	} else {
		syslog(LOG_INFO, "[DM]: Zwave Framework message queue fail");
		return false;
	}
}

void ZwFramework::zigbeefota(RequestResponse RequestResponseOBJ) {
	syslog(LOG_INFO, "[DM]: Zwave framework fota start");
}

void ZwFramework::discovery(RequestResponse RequestResponseOBJ) {

	//syslog(LOG_INFO, "[DM]: Zwave Framework discovery start");

	//----------------------------------------------------------------------------------------------------------
	DMReqStructZW DMReqStructZWObj;
	memset(&DMReqStructZWObj, 0, sizeof(DMReqStructZWObj));
	DMReqStructZWObj.Mtype = 1;
	//----------------------------------------------------------------------------------------------------------

	//syslog(LOG_INFO, "[DM]: Zwave Framework request commissioning start");

	DMReqStructZWObj.MsgId = std::stol(RequestResponseOBJ.Message_Id);
	DMReqStructZWObj.Operation = MODE_COMMISSIONING;

	bool DiscSend = ZwMessageQueue::ZwaveMsgQueueReq(DMReqStructZWObj);

	syslog(LOG_INFO,
			"[DM]: Zwave Framework request commissioning send status: [%d]",
			DiscSend);

		syslog(LOG_INFO,
			"[DM]: Zwave Framework request ################: [%d]",
			sizeof(DMReqStructZWObj));
//--------------------------------------------START DISCOVERY TIMER THERAD------------------------------------------------//
	if (DiscSend == true) {

		time_t seconds = time(NULL);

		while (stopRequested() == false
				&& (seconds + std::stol(DM_DISCOVERY_TIMEOUT)) != time(NULL)) {

		/*	syslog(LOG_DEBUG,
					"[DM]: Zwave discovery thread started waiting for .. [%ld - %s]",
					time(NULL) - seconds,
					(std::string("ZW") + RequestResponseOBJ.Message_Id).c_str());*/

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

		if (stopRequested() == false
				&& (seconds + std::stol(DM_DISCOVERY_TIMEOUT)) <= time(NULL)) {

			pthread_mutex_lock(&LockMutexDisc);

			if (FlagDiscSend == true) {

				FlagDiscSend = false;

				bool RemoveStatusDiscReqZW =
						DmFrameworkHandle::RemoveReqHandleMapId(
								std::string("ZW")
										+ RequestResponseOBJ.Message_Id);

				if (RemoveStatusDiscReqZW == true) {
					syslog(LOG_DEBUG,
							"[DM]: Zwave discovery thread removed for id:[%s]",
							(std::string("ZW") + RequestResponseOBJ.Message_Id).c_str());
				} else {
					syslog(LOG_DEBUG,
							"[DM]: Zwave discovery thread not removed for id:[%s]",
							(std::string("ZW") + RequestResponseOBJ.Message_Id).c_str());
				}

				bool RemoveStatusDiscReqZB =
						DmFrameworkHandle::RemoveReqHandleMapId(
								std::string("ZB")
										+ RequestResponseOBJ.Message_Id);

				if (RemoveStatusDiscReqZB == true) {
					syslog(LOG_DEBUG,
							"[DM]: Zwave discovery thread removed for id:[%s]",
							(std::string("ZB") + RequestResponseOBJ.Message_Id).c_str());
				} else {
					syslog(LOG_DEBUG,
							"[DM]: Zwave discovery thread not removed for id:[%s]",
							(std::string("ZB") + RequestResponseOBJ.Message_Id).c_str());
				}

				syslog(LOG_INFO,
						"[DM]: Zwave Framework request discovery zwave response timeout ");

				//SENDING TIMEOUT RESPONSE PLATFORM
				RequestResponseOBJ.Client = std::stol(
						RequestResponseOBJ.Message_Id.substr(0, 2));
				RequestResponseOBJ.Command = DM_DISCOVERY;
				RequestResponseOBJ.Message = "Discovery timeout done";
				RequestResponseOBJ.RuleSceneData = "NULL";
				RequestResponseOBJ.Status_Code = DM_FAIL;

				DmLedInterface::DiscoveryStop();
				DmPlatform::DmSendingResponse(RequestResponseOBJ);
			}

			pthread_mutex_unlock(&LockMutexDisc);
		}
	} else {

		DmFrameworkHandle::RemoveReqHandleMapId(
				std::string("ZW") + RequestResponseOBJ.Message_Id);
		syslog(LOG_INFO,
				"[DM]: Zwave Framework request commissioning not send from zw");
	}
	/*END DISCOVERY TIMER THERAD*/
}

void ZwFramework::control(RequestResponse RequestResponseOBJ) {

	//syslog(LOG_INFO, "[DM]: Zwave Framework CONTROL START");
	DMReqStructZW DMReqStructZWObj;
	DMReqStructZWObj.Mtype = DM_MQ_1;

	DMReqStructZWObj.MsgId = (uint32_t) std::stol(
			RequestResponseOBJ.Message_Id.c_str());
	DMReqStructZWObj.Operation = MODE_CONTROL;

	DMReqStructZWObj.NodeId =
			(uint8_t) stol(
					((std::string(RequestResponseOBJ.Thing_Id)).substr(14,
							((std::string(RequestResponseOBJ.Thing_Id)).length()
									- 14))));

	DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

	DmThing* DmThingOBJ = new DmThing;

	std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
			DmThingMapObj->DmThingDataMap.find(RequestResponseOBJ.Thing_Id);
	if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {

		DmThingOBJ = DmThingMapObjItr->second;

		std::stringstream HOMEID(DmThingOBJ->gethome_node_id());
		HOMEID >> DMReqStructZWObj.HomeId;

		for (int EntityNo = 0; EntityNo < DmThingOBJ->getEntityListSize();
				EntityNo++) {

			if (DmThingOBJ->getEntityInfo(EntityNo)->getentity_id()
					== std::string(RequestResponseOBJ.Entity_Id)) {

				DMReqStructZWObj.EndPoint = (uint8_t) stol(
						std::string(RequestResponseOBJ.Entity_Id).substr(0));
				DMReqStructZWObj.GenericDeviceClass = (uint8_t) std::stol(
						std::string(RequestResponseOBJ.Entity_Id).substr(2, 3));
				DMReqStructZWObj.SpecificDeviceClass =
						(uint8_t) std::stol(
								std::string(RequestResponseOBJ.Entity_Id).substr(
										5, 6).c_str());

				for (int PropertyNo = 0;
						PropertyNo
								< DmThingOBJ->getEntityInfo(EntityNo)->getPropertySize();
						PropertyNo++) {

					if (DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
							PropertyNo)->getproperty_type()
							== std::string(RequestResponseOBJ.Property_Type)) {

						DMReqStructZWObj.CommandClass =
								ZwMappingDatabase::DMCommandClassIDZW(
										DMReqStructZWObj.GenericDeviceClass,
										DMReqStructZWObj.SpecificDeviceClass,
										RequestResponseOBJ.Property_Name);

						DMReqStructZWObj.PropertyDataType =
								ZwMappingDatabase::DMAttributeTypeZW(
										RequestResponseOBJ.Property_Name);
						syslog(LOG_INFO,
										"[DM]: Zwave Framework control Property Type:%s ",RequestResponseOBJ.Property_Type.c_str());

						switch (ConvertStringToInt(
								std::string("DM_")
										+ RequestResponseOBJ.Property_Type)) {

						case DM_SWITCH: {
							syslog(LOG_INFO,
											"[DM]: Zwave Framework control Property Type:%s ",RequestResponseOBJ.Property_Value.c_str());
							switch ((int) std::stol(
									RequestResponseOBJ.Property_Value)) {
							/*syslog(LOG_INFO,
									"[DM]: Zwave Framework control request switch case ");*/

						case 1: {

							for (int PropertyNo1 = 0;
									PropertyNo1
											< DmThingOBJ->getEntityInfo(
													EntityNo)->getPropertySize();
									PropertyNo1++) {

								if ((DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
										PropertyNo1)->getproperty_name()
										== BRIGHTNESS)
										|| (DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
												PropertyNo1)->getproperty_name()
												== DIMMER)) {

									/*syslog(LOG_INFO,
											"[DM]: Zwave Framework control request switch case 1");*/

									if (std::stoi(
											DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
													PropertyNo1)->getproperty_value())
											== 0) {

										/*syslog(LOG_INFO,
												"[DM]: Zwave Framework control request switch case 2");*/

										DMReqStructZWObj.PropertyValue.CharValue =
										ZW_VALUE_99;

										RequestResponseOBJ.Property_Value = "1";

										DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
												PropertyNo1)->setproperty_value(
												"100.0");
										break;

									} else if (std::stoi(
											DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
													PropertyNo1)->getproperty_value())
											== 100) {

										/*syslog(LOG_INFO,
												"[DM]: Zwave Framework control request switch case 3");*/

										DMReqStructZWObj.PropertyValue.CharValue =
										ZW_VALUE_99;

										RequestResponseOBJ.Property_Value = "1";

										DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
												PropertyNo1)->setproperty_value(
												"100");
										break;

									} else {

										/*syslog(LOG_INFO,
												"[DM]: Zwave Framework control request switch case 4");*/

										DMReqStructZWObj.PropertyValue.CharValue =
												(uint8_t) std::stol(
														DmThingOBJ->getEntityInfo(
																EntityNo)->getPropertyInfo(
																PropertyNo1)->getproperty_value());

										RequestResponseOBJ.Property_Value = "1";

										DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
												PropertyNo1)->setproperty_value(
												DmThingOBJ->getEntityInfo(
														EntityNo)->getPropertyInfo(
														PropertyNo1)->getproperty_value());
										break;

									}
									break;
								} else if (DmThingOBJ->getEntityInfo(EntityNo)->getentity_name() == DOOR_LOCK) {

									if (DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
											PropertyNo1)->getproperty_name() == DOOR_STATE) {
										/*syslog(LOG_INFO,
												"[DM]: Zwave Framework control request Door lock else case 1");*/
										DMReqStructZWObj.PropertyValue.CharValue =
												255;
										//RequestResponseOBJ.Property_Value = "1";

									}

								} else if ((DmThingOBJ->getEntityInfo(
											EntityNo)->getentity_name()
											== SMART_PLUG)
											|| (DmThingOBJ->getEntityInfo(
													EntityNo)->getentity_name()
													== SWITCH_BOARD)) {
									if (DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
											PropertyNo1)->getproperty_name() == ONOFF) {
										DMReqStructZWObj.PropertyValue.CharValue =
												1;
										RequestResponseOBJ.Property_Value = "1";
									}

								} else {

									/*syslog(LOG_INFO,
											"[DM]: Zwave Framework control request switch case 5");*/

									DMReqStructZWObj.PropertyValue.CharValue =
									ZW_VALUE_99;
									RequestResponseOBJ.Property_Value = "1";
								}

							}

							/*syslog(LOG_INFO,
									"[DM]: Zwave Framework control request switch case 6");*/
							DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
									PropertyNo)->setproperty_value(
									RequestResponseOBJ.Property_Value);

						}
							break;
						case 0: {

							if (RequestResponseOBJ.Property_Name == DOOR_STATE) {

								DMReqStructZWObj.PropertyValue.CharValue = 1;
								//RequestResponseOBJ.Property_Value = "0";
								DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
										PropertyNo)->setproperty_value(
										RequestResponseOBJ.Property_Value);
								/*syslog(LOG_INFO,
										"[DM]: Zwave Framework control request Door lock else case 0");*/

							} else {
								DMReqStructZWObj.PropertyValue.CharValue =
								ZW_VALUE_0;

								DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
										PropertyNo)->setproperty_value(
										RequestResponseOBJ.Property_Value);
							}
						}
							break;

						default: {

							syslog(LOG_INFO,
									"[DM]: Zwave Framework control request set switch default value");
						}
							break;
							}
						}
							syslog(LOG_INFO,
									"[DM]: Zwave Framework control request switch set_value: [%s]",
									DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
											PropertyNo)->getproperty_value().c_str());
							break;

						case DM_RANGE: {

							/*syslog(LOG_INFO,
									"[DM]: Zwave Framework control request set range value 100");*/

							if (std::stoi(RequestResponseOBJ.Property_Value)
									== 100) {

								DMReqStructZWObj.PropertyValue.CharValue =
								ZW_VALUE_99;
								DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
										PropertyNo)->setproperty_value(
										RequestResponseOBJ.Property_Value);

							} else if (std::stoi(
									RequestResponseOBJ.Property_Value) == 0) {

								DMReqStructZWObj.PropertyValue.CharValue =
										(uint8_t) std::stol(
												RequestResponseOBJ.Property_Value);

								DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
										PropertyNo)->setproperty_value(
								DM_VALUE_0);

							} else {

								DMReqStructZWObj.PropertyValue.CharValue =
										(uint8_t) std::stol(
												RequestResponseOBJ.Property_Value);

								DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
										PropertyNo)->setproperty_value(
										RequestResponseOBJ.Property_Value);

							}
						}
							syslog(LOG_INFO,
									"[DM]: Zwave Framework control request range set_value: [%s]",
									DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
											PropertyNo)->getproperty_value().c_str());
							break;

						case DM_COLOR: {

							RGBVALUE RGBVALUEObj;
							DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
									PropertyNo)->setproperty_value(
									RequestResponseOBJ.Property_Value);

							DMReqStructZWObj.PropertyValue.ArrayValue[0] = 0;
							DMReqStructZWObj.PropertyValue.ArrayValue[1] = 0;

							RGBVALUEObj = convertHEXAtoRGB(
									RequestResponseOBJ.Property_Value);

							DMReqStructZWObj.PropertyValue.ArrayValue[2] =
									RGBVALUEObj.Red;
							DMReqStructZWObj.PropertyValue.ArrayValue[3] =
									RGBVALUEObj.Green;
							DMReqStructZWObj.PropertyValue.ArrayValue[4] =
									RGBVALUEObj.Blue;

						}
							syslog(LOG_INFO,
									"[DM]: Zwave Framework control request color set_value: [%s]",
									DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
											PropertyNo)->getproperty_value().c_str());
							break;
						case DM_LIST: {
							/*syslog(LOG_INFO,
									"[DM]: Zwave Framework control request for LIST type property");*/
							if (RequestResponseOBJ.Property_Value == "OPEN") {

								DMReqStructZWObj.PropertyValue.CharValue = 1;
								DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
										PropertyNo)->setproperty_value(
										RequestResponseOBJ.Property_Value);

							} else if (RequestResponseOBJ.Property_Value
									== "CLOSE") {
								DMReqStructZWObj.PropertyValue.CharValue = 0;

								DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
										PropertyNo)->setproperty_value(
										RequestResponseOBJ.Property_Value);

							} else {
								DMReqStructZWObj.PropertyValue.CharValue = 99;

								DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
										PropertyNo)->setproperty_value(
										RequestResponseOBJ.Property_Value);
							}
						}
							break;
						case DM_INFO: {

						}
							break;
						default: {

						}
							break;
						}
					}
				}
			}
		}

	}

	bool ControlSend = ZwMessageQueue::ZwaveMsgQueueReq(DMReqStructZWObj);
	/*syslog(LOG_INFO, "[DM]: Zwave Framework request control send status: [%d]",
			ControlSend);*/
	/*START CONTROL TIMER THERAD*/

	if (ControlSend == true) {

		syslog(LOG_INFO,
				"[DM]: Zwave Framework control request message_id: [%s]",
				RequestResponseOBJ.Message_Id.c_str());

		DmThingMapObj->SetDmThingMapCtrlObj(RequestResponseOBJ.Message_Id,
				DmThingOBJ);

		time_t seconds = time(NULL);

		while (stopRequested() == false
				&& (seconds + std::stol(DM_CONTROL_TIMEOUT)) != time(NULL)) {

			syslog(LOG_DEBUG,
					"[DM]: Zwave control thread started waiting for .. [%ld - %s]",
					time(NULL) - seconds,
					(std::string("ZW") + RequestResponseOBJ.Message_Id).c_str());

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

		if (stopRequested() == false
				&& (seconds + std::stol(DM_CONTROL_TIMEOUT)) <= time(NULL)) {

			syslog(LOG_INFO, "[DM]: Zwave control thread removed for id:[%s]",
					(std::string("ZW") + RequestResponseOBJ.Message_Id).c_str());

			DmFrameworkHandle::RemoveReqHandleMapId(
					std::string("ZW") + RequestResponseOBJ.Message_Id);

			/*syslog(LOG_INFO,
					"[DM]: Zwave Framework request control zwave response timeout start");*/

			//SENDING TIMEOUT RESPONSE PLATFORM

			RequestResponseOBJ.Client = std::stol(
					RequestResponseOBJ.Message_Id.substr(0, 2));
			RequestResponseOBJ.Message = "Device Unavailable";
			RequestResponseOBJ.Command = "CONTROL";
			RequestResponseOBJ.Status_Code = CONTROL_TIME_OUT;

			DmPlatform::DmSendingResponse(RequestResponseOBJ);

			DmThingMapObj->RemoveDmThingMapCtrlObj(
					RequestResponseOBJ.Message_Id);

			DMReqStructZW DMReqStructZWObjTimeOut;
			memset(&DMReqStructZWObjTimeOut, 0,
					sizeof(DMReqStructZWObjTimeOut));
			DMReqStructZWObjTimeOut.Mtype = DM_MQ_1;

			DMReqStructZWObjTimeOut.MsgId = (uint32_t) std::stol(
					RequestResponseOBJ.Message_Id.c_str());
			DMReqStructZWObjTimeOut.Operation = MODE_TIMEOUT;
			syslog(LOG_INFO,
					"[DM]: Zwave Framework request control zwave response timeout for message id %u",
					DMReqStructZWObj.MsgId);

			ZwMessageQueue::ZwaveMsgQueueReq(DMReqStructZWObjTimeOut);
		}
	} else {

		DmFrameworkHandle::RemoveReqHandleMapId(
				std::string("ZW") + RequestResponseOBJ.Message_Id);
		syslog(LOG_INFO,
				"[DM]: Zwave Framework request control not send from ZB");
	}
	/*END CONTROL TIMER THERAD*/
}

void ZwFramework::remove(RequestResponse RequestResponseOBJ) {

	//syslog(LOG_INFO, "[DM]: Zwave Framework remove start");

	DMReqStructZW DMReqStructZWObj;
	DMReqStructZWObj.Mtype = DM_MQ_1;

	DMReqStructZWObj.MsgId = (uint32_t) std::stol(
			RequestResponseOBJ.Message_Id.c_str());
	DMReqStructZWObj.Operation = MODE_REMOVE;

	DMReqStructZWObj.NodeId =
			(uint8_t) stol(
					((std::string(RequestResponseOBJ.Thing_Id)).substr(14,
							((std::string(RequestResponseOBJ.Thing_Id)).length()
									- 14))));

	DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

	std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
			DmThingMapObj->DmThingDataMap.find(RequestResponseOBJ.Thing_Id);
	if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {

		std::stringstream HOMEID(DmThingMapObjItr->second->gethome_node_id());
		HOMEID >> DMReqStructZWObj.HomeId;

	} else {
		syslog(LOG_INFO,
				"[DM]: Zwave Framework remove thing_id not found in main map");
	}

	//syslog(LOG_INFO, "[DM]: Zwave Framework request remove end");

	bool RemoveSend = ZwMessageQueue::ZwaveMsgQueueReq(DMReqStructZWObj);

	/*syslog(LOG_INFO, "[DM]: Zwave Framework request remove send status: [%d]",
			RemoveSend);*/
	//--------------------------------------------START REMOVE TIMER THERAD------------------------------------------------//
	if (RemoveSend == true) {

		time_t seconds = time(NULL);

		while (stopRequested() == false
				&& (seconds + std::stol(DM_REMOVE_TIMEOUT)) != time(NULL)) {

			syslog(LOG_DEBUG,
					"[DM]: ZWAVE REMOVE THREAD STARTED WAITING FOR .. [%ld - %s]",
					time(NULL) - seconds,
					(std::string("ZW") + RequestResponseOBJ.Message_Id).c_str());

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

		if (stopRequested() == false
				&& (seconds + std::stol(DM_REMOVE_TIMEOUT)) <= time(NULL)) {

			syslog(LOG_INFO, "[DM]: Zwave remove thread removed for id:[%s]",
					(std::string("ZW") + RequestResponseOBJ.Message_Id).c_str());

			DmFrameworkHandle::RemoveReqHandleMapId(
					std::string("ZW") + RequestResponseOBJ.Message_Id);

			syslog(LOG_INFO,
					"[DM]: Zwave Framework request remove zwave response timeout ");

			//SENDING TIMEOUT RESPONSE PLATFORM

			RequestResponseOBJ.Message = "Remove operation failed";
			RequestResponseOBJ.Command = "REMOVE";
			RequestResponseOBJ.Client = std::stol(
					RequestResponseOBJ.Message_Id.substr(0, 2));
			RequestResponseOBJ.Status_Code = DM_FAIL;

			DmPlatform::DmSendingResponse(RequestResponseOBJ);

		}

	} else {

		DmFrameworkHandle::RemoveReqHandleMapId(
				std::string("ZW") + RequestResponseOBJ.Message_Id);
		syslog(LOG_INFO,
				"[DM]: Zwave Framework request remove not send from ZW");
	}

	/*END REMOVE TIMER THERAD*/
}

void ZwFramework::exclusion(RequestResponse RequestResponseOBJ) {

	//syslog(LOG_INFO, "[DM]: Zwave Framework exclusion start");

	DMReqStructZW DMReqStructZWObj;
	DMReqStructZWObj.Mtype = 1;

	DMReqStructZWObj.MsgId = std::stol(RequestResponseOBJ.Message_Id);
	DMReqStructZWObj.Operation = MODE_EXCLUSION;

	bool ExclusionSend = ZwMessageQueue::ZwaveMsgQueueReq(DMReqStructZWObj);
	/*syslog(LOG_INFO,
			"[DM]: Zwave Framework request exclusion send status: [%d]",
			ExclusionSend);*/
	/*START EXCLUSION TIMER THERAD*/
	if (ExclusionSend == true) {

		time_t seconds = time(NULL);

		while (stopRequested() == false
				&& (seconds + std::stol(DM_EXCLUSION_TIMEOUT)) != time(NULL)) {

			syslog(LOG_DEBUG,
					"[DM]: Zwave exclusion thread started waiting for .. [%ld - %s]",
					time(NULL) - seconds,
					(std::string("ZW") + RequestResponseOBJ.Message_Id).c_str());

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

		if (stopRequested() == false
				&& (seconds + std::stol(DM_EXCLUSION_TIMEOUT)) <= time(NULL)) {

			syslog(LOG_INFO, "[DM]: Zwave exclusion thread removed for id:[%s]",
					(std::string("ZW") + RequestResponseOBJ.Message_Id).c_str());

			DmFrameworkHandle::RemoveReqHandleMapId(
					std::string("ZW") + RequestResponseOBJ.Message_Id);

			syslog(LOG_INFO,
					"[DM]: Zwave Framework request exclusion zwave response timeout ");

			//SENDING TIMEOUT RESPONSE PLATFORM
			RequestResponseOBJ.Client = std::stol(
					RequestResponseOBJ.Message_Id.substr(0, 2));
			RequestResponseOBJ.Command = "EXCLUSION";
			RequestResponseOBJ.Message = "Exclusion operation failed";

			RequestResponseOBJ.Status_Code = DM_FAIL;

			DmPlatform::DmSendingResponse(RequestResponseOBJ);
		}
	} else {

		DmFrameworkHandle::RemoveReqHandleMapId(
				std::string("ZW") + RequestResponseOBJ.Message_Id);
		syslog(LOG_INFO,
				"[DM]: Zwave Framework request exclusion not send from zb");
	}
	//--------------------------------------------END EXCLUSION TIMER THERAD------------------------------------------------//
}

void ZwFramework::learnmode(RequestResponse RequestResponseOBJ) {

	syslog(LOG_INFO, "[DM]: Zwave Framework LearnMode start");

	DMReqStructZW DMReqStructZWObj;
	DMReqStructZWObj.Mtype = 1;

	DMReqStructZWObj.MsgId = std::stol(RequestResponseOBJ.Message_Id);
	DMReqStructZWObj.Operation = MODE_LEARN_MODE;

	bool LearnModeSend = ZwMessageQueue::ZwaveMsgQueueReq(DMReqStructZWObj);
	/*syslog(LOG_INFO,
	 "[DM]: Zwave Framework request exclusion send status: [%d]",
	 ExclusionSend);*/
	/*START EXCLUSION TIMER THERAD*/
	if (LearnModeSend == true) {

		time_t seconds = time(NULL);

		while (stopRequested() == false && (seconds + std::stol(DM_CONTROL_TIMEOUT)) != time(NULL)) {

			syslog(LOG_DEBUG,
					"[DM]: Zwave learn mode thread started waiting for learn.. [%ld - %s]",
					time(NULL) - seconds,
					(std::string("ZW") + RequestResponseOBJ.Message_Id).c_str());

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

		if (stopRequested() == false && (seconds + std::stol(DM_CONTROL_TIMEOUT)) <= time(NULL)) {

			syslog(LOG_INFO, "[DM]: Zwave learn mode thread removed for id:[%s]",
					(std::string("ZW") + RequestResponseOBJ.Message_Id).c_str());

			DmFrameworkHandle::RemoveReqHandleMapId(
					std::string("ZW") + RequestResponseOBJ.Message_Id);

			syslog(LOG_INFO,
					"[DM]: Zwave Framework request learn zwave response timeout ");

			//SENDING TIMEOUT RESPONSE PLATFORM
			RequestResponseOBJ.Client = std::stol(
					RequestResponseOBJ.Message_Id.substr(0, 2));
			RequestResponseOBJ.Command = "LEARN_MODE";
			RequestResponseOBJ.Message = "Learn mode failed";

			RequestResponseOBJ.Status_Code = DM_FAIL;

			DmPlatform::DmSendingResponse(RequestResponseOBJ);
		}
	} else {

		DmFrameworkHandle::RemoveReqHandleMapId(
				std::string("ZW") + RequestResponseOBJ.Message_Id);
		syslog(LOG_INFO,
				"[DM]: Zwave Framework request exclusion not send from zwave");
	}
	//--------------------------------------------END EXCLUSION TIMER THERAD------------------------------------------------//
}
void ZwFramework::forceremove(RequestResponse RequestResponseOBJ) {

	syslog(LOG_INFO, "[DM]: Zwave Framework force remove start");

	DMReqStructZW DMReqStructZWObj;
	DMReqStructZWObj.Mtype = DM_MQ_1;

	DMReqStructZWObj.MsgId = (uint32_t) std::stol(
			RequestResponseOBJ.Message_Id.c_str());
	DMReqStructZWObj.Operation = MODE_FORCE_REMOVE;

	DMReqStructZWObj.NodeId =
			(uint8_t) stol(
					((std::string(RequestResponseOBJ.Thing_Id)).substr(14,
							((std::string(RequestResponseOBJ.Thing_Id)).length()
									- 14))));

	DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

	std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
			DmThingMapObj->DmThingDataMap.find(RequestResponseOBJ.Thing_Id);
	if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {

		std::stringstream HOMEID(DmThingMapObjItr->second->gethome_node_id());
		HOMEID >> DMReqStructZWObj.HomeId;

	} else {
		syslog(LOG_INFO,
				"[DM]: Zwave Framework force remove thing_id not found in mainmap");
	}

	syslog(LOG_INFO, "[DM]: Zwave Framework request remove end");

	bool RemoveSend = ZwMessageQueue::ZwaveMsgQueueReq(DMReqStructZWObj);

	syslog(LOG_INFO, "[DM]: Zwave Framework request remove send status: [%d]",
			RemoveSend);
	//--------------------------------------------START FORCE REMOVE TIMER THERAD------------------------------------------------//
	if (RemoveSend == true) {

		time_t seconds = time(NULL);

		while (stopRequested() == false
				&& (seconds + std::stol(DM_REMOVE_TIMEOUT)) != time(NULL)) {

			syslog(LOG_DEBUG,
					"[DM]: ZWAVE FORCE REMOVE THREAD STARTED WAITING FOR .. [%ld - %s]",
					time(NULL) - seconds,
					(std::string("ZW") + RequestResponseOBJ.Message_Id).c_str());

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

		if (stopRequested() == false
				&& (seconds + std::stol(DM_REMOVE_TIMEOUT)) <= time(NULL)) {

			syslog(LOG_INFO, "[DM]: Zwave force remove thread removed for id:[%s]",
					(std::string("ZW") + RequestResponseOBJ.Message_Id).c_str());

			DmFrameworkHandle::RemoveReqHandleMapId(
					std::string("ZW") + RequestResponseOBJ.Message_Id);

			syslog(LOG_INFO,
					"[DM]: Zwave Framework request remove zwave response timeout ");

			//SENDING TIMEOUT RESPONSE PLATFORM

			RequestResponseOBJ.Message = "DM REMOVE TIMEOUT";
			RequestResponseOBJ.Command = "Force remove operation failed";
			RequestResponseOBJ.Client = std::stol(
					RequestResponseOBJ.Message_Id.substr(0, 2));
			RequestResponseOBJ.Status_Code = DM_FAIL;

			DmPlatform::DmSendingResponse(RequestResponseOBJ);

		}

	} else {

		DmFrameworkHandle::RemoveReqHandleMapId(
				std::string("ZW") + RequestResponseOBJ.Message_Id);
		syslog(LOG_INFO,
				"[DM]: Zwave Framework request remove not send from ZW");
	}

	/*END REMOVE TIMER THERAD*/
}

void ZwFramework::s2security(RequestResponse RequestResponseOBJ) {

	//syslog(LOG_INFO, "[DM]: Zwave Framework s2security start");

	s2securityflag = true;

	DMReqStructZW DMReqStructZWObj;
	DMReqStructZWObj.Mtype = 1;

	syslog(LOG_INFO,
			"[DM]: Zwave Framework request exclusion send status: [%s]",
			RequestResponseOBJ.Thing_Response.c_str());
	DMReqStructZWObj.MsgId = std::stol(RequestResponseOBJ.Message_Id);
	DMReqStructZWObj.Operation = MODE_S2_SECURITY;
	if (RequestResponseOBJ.Thing_Response.size() <= 5) {
		for (unsigned int i = 0; i < RequestResponseOBJ.Thing_Response.size(); i++) {
			DMReqStructZWObj.dsk[i] = RequestResponseOBJ.Thing_Response.at(i);
		}
		DMReqStructZWObj.dsk[5] =0; 	/* Null terminating*/

		bool ExclusionSend = ZwMessageQueue::ZwaveMsgQueueReq(DMReqStructZWObj);
		/*syslog(LOG_INFO,
				"[DM]: Zwave Framework request exclusion send status: [%d]",
				ExclusionSend);*/
		/*START EXCLUSION TIMER THERAD*/
		if (ExclusionSend == true) {

			time_t seconds = time(NULL);

			while (stopRequested() == false && (seconds + 25) != time(NULL)) {

				syslog(LOG_DEBUG,
						"[DM]: Zwave S2SECURITY thread started waiting for .. [%ld - %s]",
						time(NULL) - seconds,
						(std::string("ZW") + RequestResponseOBJ.Message_Id).c_str());

				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}

			if (stopRequested() == false && (seconds + 25) <= time(NULL)) {

				syslog(LOG_INFO,
						"[DM]: Zwave exclusion thread removed for id:[%s]",
						(std::string("ZW") + RequestResponseOBJ.Message_Id).c_str());

				DmFrameworkHandle::RemoveReqHandleMapId(
						std::string("ZW") + RequestResponseOBJ.Message_Id);

				syslog(LOG_INFO,
						"[DM]: Zwave Framework request exclusion zwave response timeout ");

				//SENDING TIMEOUT RESPONSE PLATFORM
				RequestResponseOBJ.Client = std::stol(
						RequestResponseOBJ.Message_Id.substr(0, 2));
				RequestResponseOBJ.Command = "S2_SECURITY";
				RequestResponseOBJ.Message = "S2 security operation failed";

				RequestResponseOBJ.Status_Code = DM_FAIL;

			}
		} else {

			DmFrameworkHandle::RemoveReqHandleMapId(
					std::string("ZW") + RequestResponseOBJ.Message_Id);
			syslog(LOG_INFO,
					"[DM]: Zwave Framework request exclusion not send from zb");
		}
		DmPlatform::DmSendingResponse(RequestResponseOBJ);
	}

	else {
		DMReqStructZWObj.MsgId = std::stol(RequestResponseOBJ.Message_Id);
		DMReqStructZWObj.Operation = MODE_S2_SECURITY;
		RequestResponseOBJ.Status_Code = DM_FAIL;
		RequestResponseOBJ.Message = "Entered wrong key";

		DmPlatform::DmSendingResponse(RequestResponseOBJ);
	}
	//--------------------------------------------END EXCLUSION TIMER THERAD------------------------------------------------//
}

/* 22015:
 * @Brief: Function that runs the thread for 90 seconds and sends the Undetected response
 */
void* MotionSensorRes(void *fp) {

	Sensor_response* pointer = (Sensor_response*) fp;
	DmThingMap* DmUndetectObj =DmThingMap::GetInstance();
	std::map<std::string,std::pair<RequestResponse,Sensor_response*>>::iterator itr= DmUndetectObj->UndetectMap.find(pointer->thing_id);

	pthread_detach(pthread_self());
	struct timeval tp;
	struct timespec Sensor_timeout;
	gettimeofday(&tp, NULL);

	Sensor_timeout.tv_sec = tp.tv_sec;
	Sensor_timeout.tv_sec += ZW_Sensor_timeout;


	pthread_mutex_lock(&(pointer->mut));
	pthread_cond_timedwait(&(pointer->cond), &(pointer->mut), &Sensor_timeout);


	if(pointer->response_done == 0)
	{
		DmPlatform::DmSendingResponse(itr->second.first);
		syslog(LOG_DEBUG,"[DM]:Erasing from undetected map after 90 seconds");

		if(itr != DmUndetectObj->UndetectMap.end()){
			DmUndetectObj->UndetectMap.erase(itr->first);
		}
	}
	else
	{
		syslog(LOG_DEBUG,"[DM]:Sensor Observe is: 0, therefore erasing from undetected map");

		if(itr != DmUndetectObj->UndetectMap.end()){
			DmUndetectObj->UndetectMap.erase(itr->first);
		}
	}

	delete(pointer);
	pthread_mutex_unlock(&(pointer->mut));
	return NULL;
}

/* 22015:
 * @Brief: Function that initializes the pointer and creates the thread for occupancy undetected response
 */
void Sensor_thread_init(RequestResponse Res_Obj){

	Sensor_response *ptr;
	ptr=new Sensor_response;
	Res_Obj.Property_Value=DM_VALUE_0;


	pthread_cond_init(&(ptr->cond), NULL);
	pthread_mutex_init(&(ptr->mut), NULL);
	ptr->response_done=0;
	ptr->tid=-1;
	ptr->thing_id=Res_Obj.Thing_Id;

	DmThingMap* DmUndetectObj =DmThingMap::GetInstance();
	DmUndetectObj->UndetectMap[Res_Obj.Thing_Id].first= Res_Obj;
	DmUndetectObj->UndetectMap[Res_Obj.Thing_Id].second= ptr;
	/*std::map<RequestResponse,Sensor_response*>::iterator itr= DmUndetectObj->UndetectMap.find(Res_Obj.Thing_Id);
	if(itr != DmUndetectObj->UndetectMap.end()){
		itr->second=ptr;
	}*/
	syslog(LOG_DEBUG,"[DM]:Creating thread and calling function MotionSensorRes");
	pthread_create(&(ptr->tid), NULL, MotionSensorRes, ptr);

}




void ZwFramework::ZwReceive() {

	//syslog(LOG_INFO, "[DM]: Zwave Framework start mq receive main thread ");

	bool RemoveStatusDiscRes;

	bool ZwMsgQueueStatus = ZwFramework::DMMessageQueueInit();

	while (ZwMsgQueueStatus) {

		DMRespStructZW DMRespStructZWOBJ;

		DMRespStructZWOBJ = ZwMessageQueue::ZwaveMsgQueueResp(
				DMRespStructZWOBJ);

		//syslog(LOG_INFO, "[DM]: Zwave Framework start receive");

		if (DMRespStructZWOBJ.ErrorCode == DM_NULL) {
			syslog(LOG_INFO, "[DM]: Zwave Framework start mq receive main thread Operation: %d",DMRespStructZWOBJ.Operation);
			syslog(LOG_INFO, "[DM]:MESSAGE_ID: %d",DMRespStructZWOBJ.MsgId);

			switch (DMRespStructZWOBJ.Operation) {

		
			case MODE_COMMISSIONING: {

				syslog(LOG_INFO,
						"[DM]: Zwave Framework commissioning response start");


				 RemoveStatusDiscRes =
						DmFrameworkHandle::StopDiscReqHandleThread(
								std::to_string(DMRespStructZWOBJ.MsgId), "ZW");

				if(DMRespStructZWOBJ.MsgId==123)
				{
					syslog(LOG_INFO,
						"[DM]: Zwave Framework if messsage id id 123 ");
						 RemoveStatusDiscRes=true;
					
				}

				if (RemoveStatusDiscRes == true) {

					DmThingMap *DmThingMapOBJ = DmThingMap::GetInstance();

					RequestResponse RequestResponseOBJ;
					RequestResponseOBJ.Thing_Id = "ZW_"
							+ std::to_string(DMRespStructZWOBJ.HomeId) + "_"
							+ std::to_string(DMRespStructZWOBJ.NodeId);

					bool ThingPresent = DmThingMapOBJ->CheckToThingMap(
							RequestResponseOBJ.Thing_Id);

					//LED DEVICE CONNECTED
					DmLedInterface::DeviceConnected();
					FlagDiscSend = false;

					if (ThingPresent == true) {

						syslog(LOG_DEBUG,
								"[DM]: Zwave Framework discovery response already created for thing_id:[%s] with message_id:[%s]",
								RequestResponseOBJ.Thing_Id.c_str(),
								std::to_string(DMRespStructZWOBJ.MsgId).c_str());
					} else {

						syslog(LOG_DEBUG,
								"[DM]: Zwave Framework discovery response received for thing_id:[%s] with message_id:[%s]",
								RequestResponseOBJ.Thing_Id.c_str(),
								std::to_string(DMRespStructZWOBJ.MsgId).c_str());

						DmThing *DmThingOBJ = new DmThing;

						DmFramework* ZWObj = new ZwFramework;

						DmThingOBJ->setFramework(ZWObj);

						DmThingOBJ->setzone_type(DM_VALUE_0);

						DmThingOBJ->setthing_status("online");

						DmThingOBJ->setthing_id(RequestResponseOBJ.Thing_Id);

						DmThingOBJ->sethome_node_id(
								std::to_string(DMRespStructZWOBJ.HomeId));

						syslog(LOG_DEBUG,
								"[DM]: Zwave Framework discovery home_node_id: [%s]",
								DmThingOBJ->gethome_node_id().c_str());

						DmThingOBJ->setmanufacturerUrl(
								"www.smartron.com?things");

						DmThingOBJ->setmanufacturerName(
								ZwMappingDatabase::DMManufactureNameZW(
										DMRespStructZWOBJ.VendorId,
										DMRespStructZWOBJ.ProductId,
										DMRespStructZWOBJ.Category,
										DMRespStructZWOBJ.VendorType));

						DmThingOBJ->sethardwareVersion(
								std::to_string(DMRespStructZWOBJ.VendorType));

						DmThingOBJ->setframeworkName("zwave");

						DmThingOBJ->setfirmwareVersion(
								std::to_string(DMRespStructZWOBJ.ProductId));

						syslog(LOG_DEBUG,
								"[DM]: Zwave Framework discovery response endpointcount: [%d]",
								DMRespStructZWOBJ.NoEndPoint);

						for (int EndPointNo = 0;
								EndPointNo < DMRespStructZWOBJ.NoEndPoint;
								EndPointNo++) {

							DmThingOBJ->setthing_type(
									ZwMappingDatabase::DMThingTypeZW(
											DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass,
											DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice));

							if (ZwMappingDatabase::DMThingTypeZW(
									DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass,
									DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice)
									== "Sensor") {

//----------------------------------------- Zwave Framework COMMISSIONING SENSOR START ---------------------------------------------------

								syslog(LOG_INFO,
										"[DM]: Zwave Framework discovery response sensor received create sensor");

								for (int InterfaceNo = 0;
										InterfaceNo
												< (int) DMRespStructZWOBJ.EndPoint[EndPointNo].NoInterFace;
										InterfaceNo++) {

									if (!DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass
											== 0) {

										if (DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass
												== ZW_CLS_128) {

											DmThingOBJ->setbattery_status(
													std::to_string(
															DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].PropertyValue.CharValue));
											syslog(LOG_DEBUG,
													"[DM]: Zwave Framework discovery response battery status of device: [%s]",
													std::to_string(
															DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].PropertyValue.CharValue).c_str());
										} else {
											syslog(LOG_INFO,
													"[DM]: Zwave Framework discovery response non-battery device");
										}

										syslog(LOG_DEBUG,
												"[DM]: Zwave Framework discovery response sensor received command_class_id: [%d]",
												DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass);

										if (DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass
												== ZwMappingDatabase::DMCommandIDCheck(
														DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass,
														DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
														DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass)) {

											syslog(LOG_INFO,
													"[DM]: *Zwave Framework discovery response sensor received create sensor");

											if (DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass
													!= 0) {

												if (DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass
														== ZW_CLS_113) {

													syslog(LOG_DEBUG,
															"[DM]: Zwave Framework response no sensor types 113: [%u]",
															DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].NoEventType);

													for (int EventType = 0;
															EventType
																	< DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].NoEventType;
															EventType++) {

														syslog(LOG_DEBUG,
																"[DM]: Zwave Framework response event_type: [%d]",
																DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType);

														syslog(LOG_DEBUG,
																"[DM]: Zwave Framework response no event types: [%u]",
																DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].NoSensorType);

														for (int EventCount = 0;
																EventCount
																		< DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].NoSensorType;
																EventCount++) {

															syslog(
															LOG_DEBUG,
																	"[DM]: Zwave Framework response event_count: [%d]",
																	DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].SensorType[EventCount]);

															DmEntity *ZWEntityObj =
																	new DmEntity;

															RequestResponseOBJ.Entity_Id =
																	std::to_string(
																			DMRespStructZWOBJ.EndPoint[EndPointNo].EndPointId)
																			+ "_"
																			+ std::to_string(
																					DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass)
																			+ "_"
																			+ std::to_string(
																					DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice)
																			+ "_"
																			+ std::to_string(
																					DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass)
																			+ "_"
																			+ std::to_string(
																					DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType);

															ZWEntityObj->setentity_id(
																	RequestResponseOBJ.Entity_Id);

															if (DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].SensorType[EventCount]
																	== ZwMappingDatabase::DMSensorEventTypeZW(
																			DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass,
																			DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																			DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																			DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType,
																			DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].SensorType[EventCount])) {

																syslog(
																LOG_DEBUG,
																		"[DM]: *Zwave Framework discovery response sensor received event type database: [%d]",
																		DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType);

																DmProperty *ZWPropertyObj =
																		new DmProperty;

																
																std::string thing_name =
																		ZwMappingDatabase::DMThingNameZW(
																				DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass,
																				DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																				DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																				DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType);





																if (thing_name
																		!= "DM_DEFAULT") {
																	DmThingOBJ->setthing_name(
																			thing_name);


																	bool check =
																			!strcmp(
																					DmThingOBJ->getthing_name().c_str(),
																					"Smoke Detector");

																	if (check
																			== 1)
																		 smoke_flag =
																				true;

																	syslog(
																	LOG_DEBUG,
																			"[DM]: Check value for smoke is %d",
																			check);
																	ZWEntityObj->setentity_id(
																			RequestResponseOBJ.Entity_Id);

																	std::string entity_name =
																			ZwMappingDatabase::DMEntityNameZW(
																					DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass,
																					DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																					DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																					DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType,
																					DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].SensorType[EventCount]);
																	syslog(
																	LOG_DEBUG,
																			"[DM]: ENTITY_NAME IS [%s]",
																			entity_name.c_str());

																	if (entity_name
																			!= "DM_DEFAULT") {
																		ZWEntityObj->setentity_name(
																				entity_name);

																		ZWEntityObj->setentity_type(
																				ZwMappingDatabase::DMEntityTypeZW(
																						DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass,
																						DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																						DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																						DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType,
																						DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].SensorType[EventCount]));

																		ZWEntityObj->setsecurity_level(
																				ZwMappingDatabase::DMEntitySecurityLevelZW(
																						DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass,
																						DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																						DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																						DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType,
																						DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].SensorType[EventCount]));

																	}

																}
																

																switch (ZwMappingDatabase::DMPropertyTypeZW(
																		DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																		DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																		DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType)) {

																case INFO: {
																	ZWPropertyObj->setproperty_type(
																	DM_INFO_DM);

																	
																	RequestResponseOBJ.Property_Name =
																			ZwMappingDatabase::DMPropertyNameZW(
																					DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																					DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																					ZwMappingDatabase::DMPropertyTypeZW(
																							DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																							DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																							DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType),
																					DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType,
																					DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].SensorType[EventCount]);
																	syslog(
																	LOG_INFO,
																			"[DM] [%d] ",
																			DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].SensorType[EventCount]);


																	RequestResponseOBJ.Property_Range =
																			ZwMappingDatabase::DMPropertyRangeZW(
																					DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																					DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																					ZwMappingDatabase::DMPropertyTypeZW(
																							DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																							DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																							DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType),
																							DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType);

																	ZWPropertyObj->setproperty_name(
																			RequestResponseOBJ.Property_Name);

																	ZWPropertyObj->setproperty_range(
																			RequestResponseOBJ.Property_Range);

																	ZWPropertyObj->setproperty_value(
																			std::to_string(
																					(int) DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].PropertyValue.CharValue));

																}
																	syslog(
																	LOG_INFO,
																			"[DM]: Zwave Framework discovery response sensor property info end ");
																	break;
																}



																if (RequestResponseOBJ.Property_Name
																		== "tamper") {

																	if (DmThingOBJ->getEntityListSize()
																			> 0) {
																		syslog(
																				LOG_INFO,
																				"[DM]: Zwave Framework discovery size greater than zero ");
																		DmThingOBJ->getEntityInfo(
																				DmThingOBJ->getEntityListSize()
																						- 1)->setpropertyInfo(
																				ZWPropertyObj);
																	}
																} else
																	ZWEntityObj->setpropertyInfo(
																			ZWPropertyObj);

															}

															if ((ZWEntityObj->getPropertySize()
																	!= 0)
																	&& (!ZWEntityObj->getentity_id().empty())) {
																DmThingOBJ->setentityInfo(
																		ZWEntityObj);
															} //CHECKING PROPERTY SHOULD BE NON ZERO
														}




													}

													if (smoke_flag == 1)
														DmThingOBJ->setthing_name(
																"Smoke Detector");

															smoke_flag =false;

												} else {

													syslog(
													LOG_DEBUG,
															"[DM]: Zwave Framework response no sensor types other: [%u]",
															DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].NoEventType);

													for (int EventType = 0;
															EventType
																	< DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].NoEventType;
															EventType++) {

														if (DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType
																!= 0) {

															syslog(
															LOG_DEBUG,
																	"[DM]: Zwave Framework discovery response sensor received sensor type response: [%d] sensorcount:[%d]",
																	DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType,
																	EventType);

															syslog(
															LOG_DEBUG,
																	"[DM]: Zwave Framework discovery response sensor received sensor type database: [%d]",
																	ZwMappingDatabase::DMSensorTypeZW(
																			DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass,
																			DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																			DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																			DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType));

															if (DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType
																	== (uint8_t) ZwMappingDatabase::DMSensorTypeZW(
																			DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass,
																			DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																			DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																			DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType)) {

																DmEntity *ZWEntityObj =
																		new DmEntity;

																DmProperty *ZWPropertyObj =
																		new DmProperty;

																RequestResponseOBJ.Entity_Id =
																		std::to_string(
																				DMRespStructZWOBJ.EndPoint[EndPointNo].EndPointId)
																				+ "_"
																				+ std::to_string(
																						DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass)
																				+ "_"
																				+ std::to_string(
																						DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice)
																				+ "_"
																				+ std::to_string(
																						DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass)
																				+ "_"
																				+ std::to_string(
																						DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType);

																syslog(
																LOG_DEBUG,
																		"[DM]: *Zwave Framework discovery response sensor received for already new entity_id: [%s]",
																		RequestResponseOBJ.Entity_Id.c_str());

																
																std::string thing_name =
																		ZwMappingDatabase::DMThingNameZW(
																				DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass,
																				DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																				DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																				DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType);

																if (thing_name
																		!= "DM_DEFAULT") {
																	DmThingOBJ->setthing_name(
																			thing_name);

																	ZWEntityObj->setentity_id(
																			RequestResponseOBJ.Entity_Id);

																	ZWEntityObj->setentity_name(
																			ZwMappingDatabase::DMEntityNameZW(
																					DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass,
																					DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																					DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																					DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType,
																					0));

																	ZWEntityObj->setentity_type(
																			ZwMappingDatabase::DMEntityTypeZW(
																					DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass,
																					DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																					DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																					DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType,
																					0));

																	ZWEntityObj->setsecurity_level(
																			ZwMappingDatabase::DMEntitySecurityLevelZW(
																					DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass,
																					DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																					DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																					DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType,
																					0));
																}

																switch (ZwMappingDatabase::DMPropertyTypeZW(
																		DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																		DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																		DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType)) {

																case INFO: {

																	syslog(
																	LOG_INFO,
																			"[DM]: *Zwave Framework discovery response sensor property info start ");

																	syslog(
																	LOG_DEBUG,
																			"[DM]: *Zwave Framework discovery response sensor received sensor type: [%d]",
																			DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType);

																	ZWPropertyObj->setproperty_type(
																	DM_INFO_DM);
																	RequestResponseOBJ.Property_Name =
																			ZwMappingDatabase::DMPropertyNameZW(
																					DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																					DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																					ZwMappingDatabase::DMPropertyTypeZW(
																							DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																							DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																							DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType),
																					DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType,
																					0);

																	RequestResponseOBJ.Property_Range =
																			ZwMappingDatabase::DMPropertyRangeZW(
																					DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																					DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																					ZwMappingDatabase::DMPropertyTypeZW(
																							DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																							DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																							DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType),
																							DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[EventType].EventType);

																	ZWPropertyObj->setproperty_name(
																			RequestResponseOBJ.Property_Name);
																	ZWPropertyObj->setproperty_range(
																			RequestResponseOBJ.Property_Range);
																	ZWPropertyObj->setproperty_value(
																			std::to_string(
																					(int) DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].PropertyValue.CharValue));
																}
																	syslog(
																	LOG_INFO,
																			"[DM]: *Zwave Framework discovery response sensor property info end ");

																	break;

																}

																ZWEntityObj->setpropertyInfo(
																		ZWPropertyObj);

																if (ZWEntityObj->getPropertySize()
																		!= 0) {
																	DmThingOBJ->setentityInfo(
																			ZWEntityObj);
																}
															}
														}
													}
												}
											}
										}

									}
								}
								/*Zwave Framework COMMISSIONING SENSOR END*/
							} else {
								/*Zwave Framework COMMISSIONING ACTUATOR START*/

								syslog(LOG_INFO,
										"[DM]: Zwave Framework DISCOVERY RESPONSE RECEIVED CREATE ACTUATOR");

								DmEntity *ZWEntityObj = new DmEntity;

								RequestResponseOBJ.Entity_Id =
										std::to_string(
												DMRespStructZWOBJ.EndPoint[EndPointNo].EndPointId)
												+ "_"
												+ std::to_string(
														DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass)
												+ "_"
												+ std::to_string(
														DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice);

								syslog(LOG_DEBUG,
										"[DM]: Zwave Framework DISCOVERY RESPONSE RECEIVED CREATE ACTUATOR ENTITY_ID: [%s]",
										RequestResponseOBJ.Entity_Id.c_str());

								ZWEntityObj->setentity_id(
										RequestResponseOBJ.Entity_Id);

								for (int InterfaceNo = 0;
										InterfaceNo
												< DMRespStructZWOBJ.EndPoint[EndPointNo].NoInterFace;
										InterfaceNo++) {

									syslog(LOG_DEBUG,
											"[DM]: Zwave Framework discovery response received create actuator command_class_id: [%d]",
											DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass);

//REMOVE LOCK_LOOP FOR THE DOOR LOCK DISCOVERY

									if (!(DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass
											== ZW_CLS_113
											|| DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass
													== ZW_CLS_128)) {

										DmThingOBJ->setthing_name(
												ZwMappingDatabase::DMEntityNameZW(
														DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass,
														DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
														DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
														DM_ZW_ACT_EVENT, 0));

										ZWEntityObj->setentity_name(
												ZwMappingDatabase::DMEntityNameZW(
														DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass,
														DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
														DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
														DM_ZW_ACT_EVENT, 0));

										ZWEntityObj->setentity_type(
												ZwMappingDatabase::DMEntityTypeZW(
														DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass,
														DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
														DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
														DM_ZW_ACT_EVENT, 0));

										ZWEntityObj->setsecurity_level(
												ZwMappingDatabase::DMEntitySecurityLevelZW(
														DMRespStructZWOBJ.EndPoint[EndPointNo].GenericDeviceClass,
														DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
														DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
														DM_ZW_ACT_EVENT, 0));

										json_object* Proeprty_Type =
												json_tokener_parse(
														(ZwMappingDatabase::DMDOPropertyTypeZW(
																DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																0)).c_str());

										json_type jsontypeofpropertytype =
												json_object_get_type(
														Proeprty_Type);

										/** Checking whether the json structure of propertytype is array or not */

										if (jsontypeofpropertytype
												== json_type_array) {

											for (size_t NoProperType = 0;
													NoProperType
															< json_object_array_length(
																	Proeprty_Type);
													NoProperType++) {

												DmProperty *ZWPropertyObj =
														new DmProperty;

												std::string pro_name =
														ZwMappingDatabase::DMPropertyNameZW(
																DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																json_object_get_int(
																		json_object_array_get_idx(
																				Proeprty_Type,
																				NoProperType)),
																DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].Events[0].EventType,
																0);

												if (pro_name != "DM_DEFAULT") {
													ZWPropertyObj->setproperty_name(
															pro_name);

												}

												switch (json_object_get_int(
														json_object_array_get_idx(
																Proeprty_Type,
																NoProperType))) {

												case SWITCH: {
													syslog(
													LOG_INFO,
															"[DM]: Zwave Framework discovery response property switch start ");

													ZWPropertyObj->setproperty_type(
													DM_SWITCH_DM);

													ZWPropertyObj->setproperty_range(
															ZwMappingDatabase::DMPropertyRangeZW(
																	DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																	DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																	SWITCH,0));

													if (DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].PropertyValue.CharValue
															> 0) {
														ZWPropertyObj->setproperty_value(
														DM_VALUE_1);
													} else {
														ZWPropertyObj->setproperty_value(
														DM_VALUE_0);
													}

												}
													syslog(
													LOG_INFO,
															"[DM]: Zwave Framework discovery response property switch end ");
													break;

												case RANGE: {
													syslog(
													LOG_INFO,
															"[DM]: Zwave Framework discovery response property range start ");

													ZWPropertyObj->setproperty_type(
													DM_RANGE_DM);

													ZWPropertyObj->setproperty_range(
															ZwMappingDatabase::DMPropertyRangeZW(
																	DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																	DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																	RANGE,0));

													if (DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].PropertyValue.CharValue
															== ZW_VALUE_0) {
														ZWPropertyObj->setproperty_value(
														DM_VALUE_0);
													} else if (DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].PropertyValue.CharValue
															== ZW_VALUE_99) {
														ZWPropertyObj->setproperty_value(
														DM_VALUE_100);
													} else {
														ZWPropertyObj->setproperty_value(
																std::to_string(
																		(int) DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].PropertyValue.CharValue));

													}

												}
													syslog(
													LOG_INFO,
															"[DM]: Zwave Framework discovery response property range end ");

													break;

												case COLOR: {
													syslog(
													LOG_INFO,
															"[DM]: Zwave Framework discovery response property color start ");

													RGBVALUE RGBVALUEObjZW;

													ZWPropertyObj->setproperty_type(
													DM_COLOR_DM);

													ZWPropertyObj->setproperty_range(
															ZwMappingDatabase::DMPropertyRangeZW(
																	DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																	DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																	COLOR,0));

													RGBVALUEObjZW.Red =
															DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].PropertyValue.ArrayValue[2];
													RGBVALUEObjZW.Green =
															DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].PropertyValue.ArrayValue[3];
													RGBVALUEObjZW.Blue =
															DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].PropertyValue.ArrayValue[4];

													ZWPropertyObj->setproperty_value(
															(convertRGBtoHEXA(
																	RGBVALUEObjZW.Red,
																	RGBVALUEObjZW.Green,
																	RGBVALUEObjZW.Blue)));
												}
													syslog(
													LOG_INFO,
															"[DM]: Zwave Framework discovery response property color end ");

													break;
												case LIST: {
													syslog(
													LOG_INFO,
															"[DM]: Zwave Framework discovery response property list start ");
													ZWPropertyObj->setproperty_type(
													DM_LIST_DM);

													ZWPropertyObj->setproperty_range(
															ZwMappingDatabase::DMPropertyRangeZW(
																	DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																	DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																	LIST,0));
													ZWPropertyObj->setproperty_value(
															"STOP");
												}
													syslog(
													LOG_INFO,
															"[DM]: Zwave Framework discovery response property list end ");
													break;
												case INFO: {
													syslog(
													LOG_INFO,
															"[DM]: Zwave Framework discovery response property info start ");
													ZWPropertyObj->setproperty_type(
													DM_INFO_DM);

													ZWPropertyObj->setproperty_range(
															ZwMappingDatabase::DMPropertyRangeZW(
																	DMRespStructZWOBJ.EndPoint[EndPointNo].SpecificDevice,
																	DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].CommandClass,
																	INFO,0));
													if (DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].PropertyValue.CharValue
															>= ZW_VALUE_99) {
														ZWPropertyObj->setproperty_value(
														DM_VALUE_100);
													} else {
														ZWPropertyObj->setproperty_value(
																std::to_string(
																		(int) DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[InterfaceNo].PropertyValue.CharValue));

													}
												}
													syslog(
													LOG_INFO,
															"[DM]: Zwave Framework discovery response property info end ");
													break;
												default:
													syslog(
													LOG_INFO,
															"[DM]: Zwave Framework discovery response property default ");
													break;

												}

												ZWEntityObj->setpropertyInfo(
														ZWPropertyObj);
											}
										} else {
											syslog(LOG_DEBUG,
													"[DM]: Zwave Framework discovery response the json type of propertytype is not array ");
										}
									}
								}

								if (ZWEntityObj->getPropertySize() != 0) {
									DmThingOBJ->setentityInfo(ZWEntityObj);
								}
							}

						}

						if ((DmThingOBJ->getEntityListSize() > 1)
								&& DmThingOBJ->getthing_type() == "Sensor") {
							DmThingOBJ->setthing_name("Multilevel Sensor");
							DmThingOBJ->setthing_type("Multilevel Sensor");
						}

						if ((DmThingOBJ->getEntityListSize() > 1)
								&& DmThingOBJ->getthing_type()
										== "Smart Plug") {
							DmThingOBJ->setthing_name("SwitchBoard");
							DmThingOBJ->setthing_type("SwitchBoard");// Changing thing type,name and entity type,name from SmartPlug to Switchboard
							for (int k = 0;
									k < DmThingOBJ->getEntityListSize(); k++) {
								DmThingOBJ->getEntityInfo(k)->setentity_type(
										"SwitchBoard");
								DmThingOBJ->getEntityInfo(k)->setentity_name(
										"SwitchBoard");
							}
						}

						syslog(LOG_DEBUG,
								"[DM]: Zwave Framework discovery response creating thing in map: [%s]",
								(RequestResponseOBJ.Thing_Id).c_str());

						DmThingMap *ThingMapInstance =
								DmThingMap::GetInstance();

						ThingMapInstance->AddToThingMap(
								DmThingOBJ->getthing_id(), DmThingOBJ);

						RequestResponseOBJ.Client = std::stol(
								std::to_string(DMRespStructZWOBJ.MsgId).substr(
										0, 2));
						RequestResponseOBJ.Command =
						DM_DISCOVERY;
						RequestResponseOBJ.Status_Code =
						DM_SUCCESS;
						RequestResponseOBJ.Message = "Discovery completed successfully";
						RequestResponseOBJ.RuleSceneData =
								DmThingOBJ->getthing_name();
						RequestResponseOBJ.Message_Id = std::to_string(
								DMRespStructZWOBJ.MsgId);
						RequestResponseOBJ.Thing_Id = DmThingOBJ->getthing_id();

						DmPlatform::DmSendingResponse(RequestResponseOBJ);
					}

				} else {

					syslog(LOG_DEBUG,
							"[DM]: Zwave Framework discovery response message_id not found: [%s]",
							std::to_string(DMRespStructZWOBJ.MsgId).c_str());
				}

			}
				syslog(LOG_INFO,
						"[DM]: Zwave Framework commissioning response end");
				break;

			case MODE_CONTROL: {

				/*syslog(LOG_INFO,
						"[DM]: Zwave Framework control response start");*/

				RequestResponse RequestResponseOBJ;

				RequestResponseOBJ.Property_Name =
						DmFrameworkHandle::PropertyNameStopReqHandleThread(
								std::string("ZW")
										+ std::to_string(
												DMRespStructZWOBJ.MsgId));

				syslog(LOG_DEBUG,
						"[DM]: Zwave Framework control response for proeprty_name: [%s]",
						RequestResponseOBJ.Property_Name.c_str());

				if (RequestResponseOBJ.Property_Name
						!= std::string("DM_DEFAULT")) {

					DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

					RequestResponseOBJ.Status_Code = DM_SUCCESS;
					RequestResponseOBJ.Client = std::stol(
							std::to_string(DMRespStructZWOBJ.MsgId).substr(0,
									2));
					RequestResponseOBJ.Command = DM_CONTROL;
					RequestResponseOBJ.Message = "Control completed successfully";
					RequestResponseOBJ.Message_Id = std::to_string(
							DMRespStructZWOBJ.MsgId);
					RequestResponseOBJ.Thing_Id = "ZW_"
							+ std::to_string(DMRespStructZWOBJ.HomeId) + "_"
							+ std::to_string(DMRespStructZWOBJ.NodeId);
					//----------------------------------------------------------------------------------
					DMRespStructZWOBJ.NoEndPoint = DMRespStructZWOBJ.NoEndPoint
							- 1;
					DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace =
							DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace
									- 1;

					RequestResponseOBJ.Entity_Id =
							std::to_string(
									DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].EndPointId)
									+ "_"
									+ std::to_string(
											DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].GenericDeviceClass)
									+ "_"
									+ std::to_string(
											DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].SpecificDevice);

					std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
							DmThingMapObj->DmThingDataCtrlObjMap.find(
									std::to_string(DMRespStructZWOBJ.MsgId));
					if (DmThingMapObjItr
							!= DmThingMapObj->DmThingDataCtrlObjMap.end()) {

						/*syslog(LOG_DEBUG,
								"[DM]: Zwave Framework control response thingid found in temp map: [%s]",
								(RequestResponseOBJ.Thing_Id).c_str());*/

						for (int EndPointNo = 0;
								EndPointNo
										< DmThingMapObjItr->second->getEntityListSize();
								EndPointNo++) {

							DmThingMapObjItr->second->setstatus_code(
									std::to_string(DM_SUCCESS));

							/*syslog(LOG_DEBUG,
									"[DM]: Zwave Framework control response entity_id found in in main map: [%s] response: [%s]",
									DmThingMapObjItr->second->getEntityInfo(
											EndPointNo)->getentity_id().c_str(),
									RequestResponseOBJ.Entity_Id.c_str());*/

							if (DmThingMapObjItr->second->getEntityInfo(
									EndPointNo)->getentity_id()
									== RequestResponseOBJ.Entity_Id) {

								for (int ProeprtyNo = 0;
										ProeprtyNo
												< DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertySize();
										ProeprtyNo++) {

									if (DmThingMapObjItr->second->getEntityInfo(
											EndPointNo)->getPropertyInfo(
											ProeprtyNo)->getproperty_name()
											== RequestResponseOBJ.Property_Name) {

										if (RequestResponseOBJ.Property_Name
												== COVERING)  {

											RequestResponseOBJ.Property_Value =
													"STOP";

											/*syslog(LOG_DEBUG,
													"[DM]: Zwave Framework control response printing curtain STOP");*/

											DmThingMapObjItr->second->getEntityInfo(
													EndPointNo)->getPropertyInfo(
													ProeprtyNo + 1)->setproperty_value(
													std::to_string(
															DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[0].PropertyValue.CharValue));
										}

										RequestResponseOBJ.Property_Type =
												DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertyInfo(
														ProeprtyNo)->getproperty_type();

										RequestResponseOBJ.Property_Range =
												DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertyInfo(
														ProeprtyNo)->getproperty_range();

										RequestResponseOBJ.Property_Value =
												DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertyInfo(
														ProeprtyNo)->getproperty_value();

										/*syslog(LOG_DEBUG,
												"[DM]: Zwave Framework control response printing main thing map size: [%ld]",
												DmThingMapObj->DmThingDataMap.size());

										syslog(LOG_DEBUG,
												"[DM]: Zwave Framework control response printing main control map size: [%ld]",
												DmThingMapObj->DmThingDataCtrlObjMap.size());

										syslog(LOG_DEBUG,
												"[DM]: Zwave Framework control response checking for message_id: [%s]",
												RequestResponseOBJ.Message_Id.c_str());*/

										DmThingMapObj->DmThingDataMap.erase(
												DmThingMapObjItr->first);

										/*syslog(LOG_DEBUG,
												"[DM]: Zwave Framework control response removing main thing map size: [%ld]",
												DmThingMapObj->DmThingDataMap.size());*/

										DmThingMapObj->DmThingDataMap[RequestResponseOBJ.Thing_Id] =
												DmThingMapObj->GetDmThingMapCtrlObj(
														RequestResponseOBJ.Message_Id);

										/*syslog(LOG_DEBUG,
												"[DM]: Zwave Framework control response updated main map size: [%ld]",
												DmThingMapObj->DmThingDataMap.size());*/

										DmThingMapObj->RemoveDmThingMapCtrlObj(
												RequestResponseOBJ.Message_Id);

//										DmThingMapObj->DmThingDataCtrlObjMap.erase(
//												RequestResponseOBJ.Message_Id);

										/*syslog(LOG_DEBUG,
												"[DM]: Zwave Framework control response removing main control map size: [%ld]",
												DmThingMapObj->DmThingDataCtrlObjMap.size());

										syslog(LOG_DEBUG,
												"[DM]: Zwave Framework control response updated main map: [%s]",
												RequestResponseOBJ.Thing_Id.c_str());*/
										break;
									}
								}
								break;
							} else {
								syslog(LOG_DEBUG,
										"[DM]: Zwave Framework control response removing entity_id: [%s] not found in main map",
										RequestResponseOBJ.Entity_Id.c_str());
							}
						}

						if (!RequestResponseOBJ.Thing_Id.empty()
								&& !RequestResponseOBJ.Entity_Id.empty()
								&& !RequestResponseOBJ.Property_Name.empty()
								&& !RequestResponseOBJ.Property_Type.empty()
								&& !RequestResponseOBJ.Property_Range.empty()
								&& !RequestResponseOBJ.Property_Value.empty()) {
							DmPlatform::DmSendingResponse(RequestResponseOBJ);
						} else {
							syslog(LOG_ERR,
									"[DM]: Zwave Framework CONTROL RESPONSE stopped processing empty data to TronX");
						}

					} else {
						syslog(LOG_INFO,
								"[DM]: Zwave Framework CONTROL RESPONSE THING_ID NOT FOUND IN TEMP MAP");
					}

				} else {

					syslog(LOG_INFO,
							"[DM]: Zwave Framework CONTROL RESPONSE MESSAGE_ID NOT FOUND IN REQUEST MAP");
				}
			}

				syslog(LOG_INFO, "[DM]: Zwave Framework CONTROL RESPONSE END");

				pthread_cond_signal(&DM_Req_Control);

				break;

				/*Zwave Framework OBSERVE START*/
				case MODE_OBSERVE: {

				syslog(LOG_INFO,
						"[DM]: Zwave Framework observe response start");

				RequestResponse RequestResponseOBJ;

				DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

				DMRespStructZWOBJ.MsgId = DM_OBSERVE_MSG_ID;

				RequestResponseOBJ.Command = DM_OBSERVE;
				RequestResponseOBJ.Message = "Received observe response";
				RequestResponseOBJ.Status_Code = DM_SUCCESS;
				RequestResponseOBJ.Message_Id = std::to_string(
						DMRespStructZWOBJ.MsgId);
				RequestResponseOBJ.Client =
						std::stol(
								std::string(
										std::to_string(DMRespStructZWOBJ.MsgId)).substr(
										0, 2));
				//-----------------------------------------------------------------------------------------------------------

				DMRespStructZWOBJ.NoEndPoint = DMRespStructZWOBJ.NoEndPoint - 1;

				DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace =
						DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace
								- 1;

				DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].NoEventType =
						DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].NoEventType
								- 1;

				RequestResponseOBJ.Thing_Id = "ZW_"
						+ std::to_string(DMRespStructZWOBJ.HomeId) + "_"
						+ std::to_string(DMRespStructZWOBJ.NodeId);

				std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
						DmThingMapObj->DmThingDataMap.find(
								RequestResponseOBJ.Thing_Id);
				if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {

					syslog(LOG_INFO,
							"[DM]: Zwave Framework observe response for thing_id: [%s]",
							RequestResponseOBJ.Thing_Id.c_str());

					if (ZwMappingDatabase::DMThingTypeZW(
							DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].GenericDeviceClass,
							DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].SpecificDevice) == DM_SENSOR) {

						syslog(LOG_INFO,
								"[DM]: Zwave Framework observe sensor response start");

						for (int EndPointNo = 0;
								EndPointNo
										< DmThingMapObjItr->second->getEntityListSize();
								EndPointNo++) {
							syslog(LOG_DEBUG,
									"[DM]: Endpoint count: %d",
									EndPointNo);
							RequestResponseOBJ.Thing_Type =
									DmThingMapObjItr->second->getthing_type();

							syslog(LOG_INFO,
									"[DM]: Zwave Framework observe sensor response start :%s",
									RequestResponseOBJ.Thing_Type.c_str());

							RequestResponseOBJ.Property_Name =
									ZwMappingDatabase::DMPropertyNameEvent(
											DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].SpecificDevice,
											DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].CommandClass,
											ZwMappingDatabase::DMPropertyTypeZW(
													DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].SpecificDevice,
													DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].CommandClass,
													DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].Events[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].NoEventType].EventType),
											DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].Events[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].NoEventType].EventType,
											3);

							syslog(LOG_INFO,
									"[DM]: Zwave Property_Name check sensor response start :%s",
									RequestResponseOBJ.Property_Name.c_str());

if (RequestResponseOBJ.Property_Name == "tamper"
									&& (DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue
											== 3)) {

								RequestResponseOBJ.Property_Type = "INFO";
								syslog(LOG_INFO,
										"[DM]: Zwave Framework observe response info tamper start");
								RequestResponseOBJ.Entity_Id =
										std::to_string(
												DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].EndPointId)
												+ "_"
												+ std::to_string(
														DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].GenericDeviceClass)
												+ "_"
												+ std::to_string(
														DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].SpecificDevice)
												+ "_"
												+ std::to_string(
														DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].CommandClass)
												+ "_";
								syslog(LOG_DEBUG, "[DM]: CHAR: %d",
										DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue);

								if (strstr(
										DmThingMapObjItr->second->getEntityInfo(
												EndPointNo)->getentity_id().c_str(),
										RequestResponseOBJ.Entity_Id.c_str())) {

									syslog(LOG_INFO,
											"[DM]: Zwave Framework observe response info check tamper ");
									for (int prop_count = 0;
											prop_count
													< DmThingMapObjItr->second->getEntityInfo(
															EndPointNo)->getPropertySize();
											prop_count++) {
										if (DmThingMapObjItr->second->getEntityInfo(
												EndPointNo)->getPropertyInfo(
												prop_count)->getproperty_name()
												== "tamper") {

											syslog(LOG_INFO,
													"[DM]: Zwave Framework observe response property info");
											RequestResponseOBJ.Entity_Id =
													DmThingMapObjItr->second->getEntityInfo(
															EndPointNo)->getentity_id().c_str();

											if (DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue
													== 3) {

												RequestResponseOBJ.Property_Value =
												DM_VALUE_1;
												DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertyInfo(
														prop_count)->setproperty_value(
												DM_VALUE_1);
											} else {
												RequestResponseOBJ.Property_Value =
												DM_VALUE_0;
												DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertyInfo(
														prop_count)->setproperty_value(
												DM_VALUE_0);
											}
										}
									}

								}

							}

							else {
										if (DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].Events[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].NoEventType].EventType
										== 18)
									DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].Events[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].NoEventType].EventType
											= 2;

								RequestResponseOBJ.Entity_Id =
										std::to_string(
												DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].EndPointId)
												+ "_"
												+ std::to_string(
														DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].GenericDeviceClass)
												+ "_"
												+ std::to_string(
														DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].SpecificDevice)
												+ "_"
												+ std::to_string(
														DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].CommandClass)
												+ "_"
												+ std::to_string(
														DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].Events[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].NoEventType].EventType);

								if (DmThingMapObjItr->second->getEntityInfo(
										EndPointNo)->getentity_id()
										== RequestResponseOBJ.Entity_Id) {

									syslog(LOG_INFO,
											"[DM]: Zwave Framework observe response for entity_id: [%s]",
											RequestResponseOBJ.Entity_Id.c_str());

									RequestResponseOBJ.Property_Name =
											ZwMappingDatabase::DMPropertyNameSensorZW(
													DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].SpecificDevice,
													DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].CommandClass,
													ZwMappingDatabase::DMPropertyTypeZW(
															DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].SpecificDevice,
															DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].CommandClass,
															DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].Events[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].NoEventType].EventType),
													DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].Events[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].NoEventType].EventType);


									syslog(LOG_INFO,
											"[DM]: Zwave Framework observe response for property except tamper: [%s]",
											RequestResponseOBJ.Property_Name.c_str());

									if (DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].CommandClass
											== ZW_CLS_128) {

										DmThingMapObjItr->second->setbattery_status(
												std::to_string(
														DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue));
										syslog(LOG_DEBUG,
												"[DM]: Zwave Framework observe response battery status of device: [%s]",
												std::to_string(
														DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue).c_str());
									} else {
										syslog(LOG_INFO,
												"[DM]: Zwave Framework observe response non-battery device");
									}

									for (int ProeprtyNo = 0;
											ProeprtyNo
													< DmThingMapObjItr->second->getEntityInfo(
															EndPointNo)->getPropertySize();
											ProeprtyNo++) {

										if (DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].Events[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].NoEventType].EventType
												== 18) {
											DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].Events[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].NoEventType].EventType =
													2;
										}

										syslog(LOG_DEBUG,
												"[DM]: Zwave Framework observe response info proeprty_name0: [%s]",
												DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertyInfo(
														ProeprtyNo)->getproperty_name().c_str());

										syslog(LOG_DEBUG,
												"[DM]: Zwave Framework observe response info proeprty_name1: [%s]",
												RequestResponseOBJ.Property_Name.c_str());

										if (DmThingMapObjItr->second->getEntityInfo(
												EndPointNo)->getPropertyInfo(
												ProeprtyNo)->getproperty_name()
												== RequestResponseOBJ.Property_Name) {

											switch (ZwMappingDatabase::DMPropertyTypeZW(
													DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].SpecificDevice,
													DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].CommandClass,
													DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].Events[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].NoEventType].EventType)) {

											case INFO: {

												syslog(LOG_INFO,
														"[DM]: Zwave Framework observe response info sensor start");

												RequestResponseOBJ.Property_Type =
												DM_INFO_DM;

												if (TEMPERATURE
														== RequestResponseOBJ.Property_Name) {

													RequestResponseOBJ.Property_Value =
															std::to_string(
																	(int) DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue);
													DmThingMapObjItr->second->getEntityInfo(
															EndPointNo)->getPropertyInfo(
															ProeprtyNo)->setproperty_value(
															std::to_string(
																	(int) DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue));

												} else if (HUMIDITY
														== RequestResponseOBJ.Property_Name) {

													syslog(LOG_INFO,
															"[DM]: Zwave Framework observe response info humidity start");

													RequestResponseOBJ.Property_Value =
															std::to_string(
																	(int) DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue);

													DmThingMapObjItr->second->getEntityInfo(
															EndPointNo)->getPropertyInfo(
															ProeprtyNo)->setproperty_value(
															std::to_string(
																	(int) DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue));

													syslog(LOG_INFO,
															"[DM]: Zwave Framework observe response info humidity end");

												} else if (LUMINANCE
														== RequestResponseOBJ.Property_Name) {

													syslog(LOG_INFO,
															"[DM]: Zwave Framework observe response info luminance start");

													RequestResponseOBJ.Property_Value =
															std::to_string(
																	(int) DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue);
													DmThingMapObjItr->second->getEntityInfo(
															EndPointNo)->getPropertyInfo(
															ProeprtyNo)->setproperty_value(
															std::to_string(
																	(int) DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue));

													syslog(LOG_INFO,
															"[DM]: Zwave Framework observe response info luminance end");

												} else if (RequestResponseOBJ.Property_Name
														== OCCUPANCY) {

													DmThingMap* DmUndetectObj =DmThingMap::GetInstance();

													syslog(LOG_INFO,
															"[DM]: Zwave Framework observe response info occupancy start");

													if (DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue
															== 8) {

														RequestResponseOBJ.Property_Value =
														DM_VALUE_1;
														DmThingMapObjItr->second->getEntityInfo(
																EndPointNo)->getPropertyInfo(
																ProeprtyNo)->setproperty_value(
														DM_VALUE_1);

														syslog(LOG_DEBUG,"[DM]: Property value 1 from Sensor ");

														//22015: Creating Map Iterator
														std::map<std::string,std::pair<RequestResponse,Sensor_response*>>::iterator itr =
																DmUndetectObj->UndetectMap.find(RequestResponseOBJ.Thing_Id);

														//22015: Creating a new entry if empty or unavailable
														if (DmUndetectObj->UndetectMap.empty() || (itr==DmUndetectObj->UndetectMap.end() && itr->first!=RequestResponseOBJ.Thing_Id)) {
															syslog(LOG_DEBUG,
																	"[DM]: Dm_platform Undetect map is empty, creating entry ");

															Sensor_thread_init(RequestResponseOBJ);
														}

													}
													else {

														syslog(LOG_DEBUG,"[DM]: Property value 0 from sensor ");
														std::map<std::string,std::pair<RequestResponse,Sensor_response*>>::iterator itr =
																DmUndetectObj->UndetectMap.find(RequestResponseOBJ.Thing_Id);

														//22015: Terminating the thread incase a sensor returns undetected response within 90 seconds
														if(itr != DmUndetectObj->UndetectMap.end()){

															Sensor_response *dummy = itr->second.second;
															pthread_mutex_lock(&(dummy->mut));
															dummy->response_done=1;
															pthread_cond_signal(&(dummy->cond));
															pthread_mutex_unlock(&(dummy->mut));
														}

														RequestResponseOBJ.Property_Value =
														DM_VALUE_0;
														DmThingMapObjItr->second->getEntityInfo(
																EndPointNo)->getPropertyInfo(
																ProeprtyNo)->setproperty_value(
																DM_VALUE_0);
													}

														syslog(LOG_INFO,
																"[DM]: Zwave Framework observe response info occupancy end");

													} else if (CONTACT_DETECT
														== RequestResponseOBJ.Property_Name) {

													syslog(LOG_INFO,
															"[DM]: Zwave Framework observe response info contact_detect start");

													if (DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue
															== 22) {
														RequestResponseOBJ.Property_Value =
														DM_VALUE_0;
														DmThingMapObjItr->second->getEntityInfo(
																EndPointNo)->getPropertyInfo(
																ProeprtyNo)->setproperty_value(
														DM_VALUE_0);
													} else {
														RequestResponseOBJ.Property_Value =
														DM_VALUE_1;
														DmThingMapObjItr->second->getEntityInfo(
																EndPointNo)->getPropertyInfo(
																ProeprtyNo)->setproperty_value(
														DM_VALUE_1);
													}

													syslog(LOG_INFO,
															"[DM]: Zwave Framework observe response info contact_detect end");

												} else if (SMOKE_DETECT
														== RequestResponseOBJ.Property_Name) {

													syslog(LOG_INFO,
															"[DM]: Zwave Framework observe response info smoke_detect start");

													if (DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue
															== ZW_VALUE_0) {
														RequestResponseOBJ.Property_Value =
														DM_VALUE_0;
														DmThingMapObjItr->second->getEntityInfo(
																EndPointNo)->getPropertyInfo(
																ProeprtyNo)->setproperty_value(
														DM_VALUE_0);
													} else {
														RequestResponseOBJ.Property_Value =
														DM_VALUE_1;
														DmThingMapObjItr->second->getEntityInfo(
																EndPointNo)->getPropertyInfo(
																ProeprtyNo)->setproperty_value(
														DM_VALUE_1);
													}
													syslog(LOG_INFO,
															"[DM]: Zwave Framework observe response info smoke_detect end");
													break;

												} else if (GAS_DETECT
														== RequestResponseOBJ.Property_Name) {

													syslog(LOG_INFO,
															"[DM]: Zwave Framework observe response info gas_detect start");

													if (DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue
															== ZW_VALUE_0) {
														RequestResponseOBJ.Property_Value =
														DM_VALUE_0;
														DmThingMapObjItr->second->getEntityInfo(
																EndPointNo)->getPropertyInfo(
																ProeprtyNo)->setproperty_value(
														DM_VALUE_0);
													} else {
														RequestResponseOBJ.Property_Value =
														DM_VALUE_1;
														DmThingMapObjItr->second->getEntityInfo(
																EndPointNo)->getPropertyInfo(
																ProeprtyNo)->setproperty_value(
														DM_VALUE_1);
													}
													syslog(LOG_INFO,
															"[DM]: Zwave Framework observe response info gas_detect end");
													break;
												}
											}
												break;
											}
											break;
										}
									}
								} else {
									syslog(LOG_INFO,
											"[DM]: Zwave Framework observe response for entity_id not found: [%s]",
											RequestResponseOBJ.Entity_Id.c_str());
								}
							}
							if (!RequestResponseOBJ.Thing_Id.empty()
									&& !RequestResponseOBJ.Entity_Id.empty()
									&& !RequestResponseOBJ.Property_Name.empty()
									&& !RequestResponseOBJ.Property_Type.empty()
									&& !RequestResponseOBJ.Property_Value.empty()) {
								break;
							}

						}
						syslog(LOG_INFO,
								"[DM]: Zwave Framework observe sensor response end");
					} else {

						syslog(LOG_INFO,
								"[DM]: Zwave Framework observe actuator response start");

						RequestResponseOBJ.Entity_Id =
								std::to_string(
										DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].EndPointId)
										+ "_"
										+ std::to_string(
												DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].GenericDeviceClass)
										+ "_"
										+ std::to_string(
												DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].SpecificDevice);

						for (int EndPointNo = 0;
								EndPointNo
										< DmThingMapObjItr->second->getEntityListSize();
								EndPointNo++) {

							if (RequestResponseOBJ.Entity_Id
									== DmThingMapObjItr->second->getEntityInfo(
											EndPointNo)->getentity_id()) {

							RequestResponseOBJ.Thing_Type =
									DmThingMapObjItr->second->getthing_type();
							/* Iterating the loop lopp only for first time*/
							for (int ProeprtyNo = 0;
									ProeprtyNo
											< 1;
									ProeprtyNo++) {

								if (DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].Events[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].NoEventType].EventType
										== 18) {
									DMRespStructZWOBJ.EndPoint[EndPointNo].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].Events[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].NoEventType].EventType =
											2;
								}

								json_object* Proeprty_Type =
										json_tokener_parse(
												(ZwMappingDatabase::DMDOPropertyTypeZW(
														DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].SpecificDevice,
														DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].CommandClass,
														0)).c_str());

								json_type jsontypeofpropertytype =
										json_object_get_type(Proeprty_Type);

								/** Checking whether the json structure of propertytype is array or not */

								if (jsontypeofpropertytype == json_type_array) {
									for (size_t NoProperType = 0;
											NoProperType
													< json_object_array_length(
															Proeprty_Type);
											NoProperType++) {

										RequestResponseOBJ.Property_Name =
												ZwMappingDatabase::DMPropertyNameZW(
														DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].SpecificDevice,
														DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].CommandClass,
														json_object_get_int(
																json_object_array_get_idx(
																		Proeprty_Type,
																		NoProperType)),
														0,
														0);
										/* If the Property NAME is BRIGHTNESS or DIMMER and the Value is Observe Value is Zero we are just breaking the loop*/
											if (RequestResponseOBJ.Property_Name == DIMMER || RequestResponseOBJ.Property_Name == BRIGHTNESS)
											{
												syslog(LOG_INFO,
													   "[DM]: Zwave RANGE");
												if (DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue == 0)
												{
													syslog(LOG_INFO,
														   "[DM]: ZW_FRAMEWORK Zwave RANGE123");
													RequestResponseOBJ.Property_Name =
														ON_OFF;

													break;
												}
											}

										switch (json_object_get_int(
												json_object_array_get_idx(
														Proeprty_Type,
														NoProperType))) {

										case SWITCH: {

											syslog(LOG_INFO,
													"[DM]: Zwave Framework observe response switch start");

											RequestResponseOBJ.Property_Type =
											DM_SWITCH_DM;

											if (ON_OFF
													== RequestResponseOBJ.Property_Name) {
												syslog(LOG_INFO,
														"[DM]: Zwave Framework observe response switch OnOff start");
												if (DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue
														> 0) {
													RequestResponseOBJ.Property_Value =
													DM_VALUE_1;
													DmThingMapObjItr->second->getEntityInfo(
															EndPointNo)->getPropertyInfo(
															ProeprtyNo)->setproperty_value(
													DM_VALUE_1);
												} else {
													RequestResponseOBJ.Property_Value =
													DM_VALUE_0;
													DmThingMapObjItr->second->getEntityInfo(
															EndPointNo)->getPropertyInfo(
															ProeprtyNo)->setproperty_value(
													DM_VALUE_0);

												}
												syslog(LOG_INFO,
														"[DM]: Zwave Framework observe response switch OnOff end");
											} else if (DOOR_STATE
													== RequestResponseOBJ.Property_Name) {
												syslog(LOG_INFO,
														"[DM]: Zwave Framework observe response switch Door State start");
												if (DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue
														== 33) {
													RequestResponseOBJ.Property_Value =
													DM_VALUE_1;
													DmThingMapObjItr->second->getEntityInfo(
															EndPointNo)->getPropertyInfo(
															ProeprtyNo)->setproperty_value(
													DM_VALUE_1);
												} else {
													RequestResponseOBJ.Property_Value =
													DM_VALUE_0;
													DmThingMapObjItr->second->getEntityInfo(
															EndPointNo)->getPropertyInfo(
															ProeprtyNo)->setproperty_value(
													DM_VALUE_0);

												}
												syslog(LOG_INFO,
														"[DM]: Zwave Framework observe response switch Door State end");
											}

										}
											break;

										case RANGE: {

											syslog(LOG_INFO,
													"[DM]: Zwave Framework observe response range start");

											RequestResponseOBJ.Property_Type =
											DM_RANGE_DM;

											if ((BRIGHTNESS
													== RequestResponseOBJ.Property_Name)
													|| (DIMMER
															== RequestResponseOBJ.Property_Name)) {

												if (DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue
														== ZW_VALUE_99) {
													RequestResponseOBJ.Property_Value =
															std::to_string(
															ZW_VALUE_99);
													DmThingMapObjItr->second->getEntityInfo(
															EndPointNo)->getPropertyInfo(
															ProeprtyNo)->setproperty_value(
													DM_VALUE_100);
												} else {
													RequestResponseOBJ.Property_Value =
															std::to_string(
																	(int) DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue);
													DmThingMapObjItr->second->getEntityInfo(
															EndPointNo)->getPropertyInfo(
															ProeprtyNo)->setproperty_value(
															std::to_string(
																	(int) DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue));
												}

											}
										}
											break;

										case INFO: {

											syslog(LOG_INFO,
													"[DM]: Zwave Framework observe response info start");

											RequestResponseOBJ.Property_Type =
											DM_INFO_DM;

											if (COVERING_STATUS
													== RequestResponseOBJ.Property_Name) {
												syslog(LOG_INFO,
														"[DM]: Zwave Framework observe response switch Covering Status start");
												if ((DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue
														== 99)
														|| DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue
																== 255
														|| DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue
																== 254) {
													RequestResponseOBJ.Property_Value =
													DM_VALUE_100;
													DmThingMapObjItr->second->getEntityInfo(
															EndPointNo)->getPropertyInfo(
															ProeprtyNo)->setproperty_value(
													DM_VALUE_100);
												} else {
													RequestResponseOBJ.Property_Value =
															std::to_string(
																	DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].InterFace[DMRespStructZWOBJ.EndPoint[DMRespStructZWOBJ.NoEndPoint].NoInterFace].PropertyValue.CharValue);
													DmThingMapObjItr->second->getEntityInfo(
															EndPointNo)->getPropertyInfo(
															ProeprtyNo)->setproperty_value(
															RequestResponseOBJ.Property_Value);
												}
												syslog(LOG_INFO,
														"[DM]: Zwave Framework observe response switch Covering Status end");
											}

										}
											break;
										}
									}
								} else {
									syslog(LOG_DEBUG,
											"[DM]: Zwave Framework observe actuator response the json type of propertytype is not array ");
								}
							}
						}
					}


					syslog(LOG_INFO,
							"[DM]: Zwave Framework observe actuator response end");
					}

				} else {
					syslog(LOG_INFO,
							"[DM]: Zwave Framework observe response for thing_id not found: [%s]",
							RequestResponseOBJ.Thing_Id.c_str());
				}

				if (!RequestResponseOBJ.Thing_Id.empty()
						&& !RequestResponseOBJ.Entity_Id.empty()
						&& !RequestResponseOBJ.Property_Name.empty()
						&& !RequestResponseOBJ.Property_Type.empty()
						&& !RequestResponseOBJ.Property_Value.empty()) {
					DmPlatform::DmSendingResponse(RequestResponseOBJ);
				} else {
					syslog(LOG_ERR,
							"[DM]: Zwave Framework observe response stopped processing empty data to TronX");
				}

			}
				syslog(LOG_INFO, "[DM]: Zwave Framework observe response end");

				break;

			case MODE_REMOVE: {

				//syslog(LOG_INFO, "[DM]: Zwave Framework remove response start");

				RequestResponse RequestResponseOBJ;

				DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

				RequestResponseOBJ.Status_Code = DM_SUCCESS;
				RequestResponseOBJ.Message_Id = std::to_string(
						DMRespStructZWOBJ.MsgId);
				RequestResponseOBJ.Client =
						std::stol(
								std::string(
										std::to_string(DMRespStructZWOBJ.MsgId)).substr(
										0, 2));

				RequestResponseOBJ.Thing_Id = "ZW_"
						+ std::to_string(DMRespStructZWOBJ.HomeId) + "_"
						+ std::to_string(DMRespStructZWOBJ.NodeId);

				/*syslog(LOG_DEBUG,
						"[DM]: Zwave Framework remove response for thing_id: [%s]",
						RequestResponseOBJ.Thing_Id.c_str());*/

				bool ThingRemoveStatus = DmFrameworkHandle::StopReqHandleThread(
						std::string("ZW")
								+ std::to_string(DMRespStructZWOBJ.MsgId));

				if (ThingRemoveStatus == true) {

					std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
							DmThingMapObj->DmThingDataMap.find(
									RequestResponseOBJ.Thing_Id);
					if (DmThingMapObjItr
							!= DmThingMapObj->DmThingDataMap.end()) {

						RequestResponseOBJ.Command = DM_REMOVE;
						RequestResponseOBJ.Message = "Remove completed successfully";

						if (!RequestResponseOBJ.Thing_Id.empty()) {
							DmThingsBackupDb * DmThingsBackupDbOBJ = DmThingsBackupDb::GetInstance();
							DmThingsBackupDbOBJ->InsertTronxSyncTable(RequestResponseOBJ.Thing_Id);
							DmPlatform::DmSendingResponse(RequestResponseOBJ);
						} else {
							syslog(LOG_ERR,
									"[DM]: Zwave Framework remove response stopped processing empty data to TronX");
						}

					} 
				} else {
					syslog(LOG_INFO,
							"[DM]: Zwave Framework remove response message_id not found request map");
				}

			}
				break;

			case MODE_S2_SECURITY: {

				s2securityflag = true;
				/*syslog(LOG_INFO,
						"[DM]: Zwave Framework security response start");*/

				RequestResponse RequestResponseOBJ;

//				DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

				RequestResponseOBJ.Status_Code = DM_SUCCESS;

				std::string rdm_request_id = "1100"
						+ std::to_string(rand() % 900);

				RequestResponseOBJ.Message_Id = rdm_request_id;
				RequestResponseOBJ.Command = "S2_SECURITY";

				RequestResponseOBJ.Client =
						std::stol(
								std::string(
										std::to_string(DMRespStructZWOBJ.MsgId)).substr(
										0, 2));

				std::string time_stamp = std::to_string(time(NULL));

				json_object *fcm_s2security_request = json_object_new_object();

				json_object *body = json_object_new_object();

				json_object_object_add(fcm_s2security_request, "verb",
						json_object_new_string("S2Security"));
				json_object_object_add(fcm_s2security_request, "domain",
						json_object_new_string("appliance"));
				json_object_object_add(fcm_s2security_request, "subdomain",
						json_object_new_string("things"));

				json_object_object_add(body, "timestamp",
						json_object_new_string(time_stamp.c_str()));
				json_object_object_add(body, "timeout",
						json_object_new_string("25"));
				json_object_object_add(body, "request_id",
						json_object_new_string(rdm_request_id.c_str()));
				json_object_object_add(body, "thing_id",
						json_object_new_string("null"));

				json_object_object_add(fcm_s2security_request, "body", body);

				RequestResponseOBJ.SendFlag = true;
				RequestResponseOBJ.Thing_Response = std::string(
						json_object_to_json_string(fcm_s2security_request));

				DmPlatform::DmSendingResponse(RequestResponseOBJ);
				//json_object_put(fcm_s2security_request);
				//json_object_put(body);
			}
				break;

			case MODE_LEARN_MODE: {

				syslog(LOG_INFO,
						"[DM]: Zwave Framework Learn Mode response start");

				RequestResponse RequestResponseOBJ;

//				DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

				RequestResponseOBJ.Status_Code = DM_SUCCESS;

				RequestResponseOBJ.Message_Id = std::to_string(
						DMRespStructZWOBJ.MsgId);
				RequestResponseOBJ.Command = "LEARN_MODE";


				RequestResponseOBJ.Client =
						std::stol(
								std::string(
										std::to_string(DMRespStructZWOBJ.MsgId)).substr(
										0, 2));

				syslog(LOG_INFO, "[DM]: Zwave Framework key  response %s",
						DMRespStructZWOBJ.Key);

				DMRespStructZWOBJ.Key[5] = '\0';

				RequestResponseOBJ.queue_id = DMRespStructZWOBJ.Key;


				syslog(LOG_INFO, "[DM]: Zwave Framework key  response234  %s",
						RequestResponseOBJ.queue_id.c_str());

				bool LearnModeStatus = DmFrameworkHandle::StopReqHandleThread(
						std::string("ZW")
								+ std::to_string(DMRespStructZWOBJ.MsgId));

				if (LearnModeStatus == true) {

					syslog(LOG_DEBUG,
							"[DM]: Zwave Framework Learn mode response thing_id: [%s]",
							RequestResponseOBJ.Thing_Id.c_str());

					RequestResponseOBJ.Command = DM_LEARN_MODE;
					RequestResponseOBJ.Message = "Learn mode completed successfully";

					DmPlatform::DmSendingResponse(RequestResponseOBJ);

				} else {
					syslog(LOG_INFO,
							"[DM]: Zwave Framework Learn mode response message_id not found request map");
				}
				//	DmPlatform::DmSendingResponse(RequestResponseOBJ);

			}
				break;
			case MODE_EXCLUSION: {

				/*syslog(LOG_INFO,
						"[DM]: Zwave Framework exclusion response start");*/

				RequestResponse RequestResponseOBJ;

//				DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

				RequestResponseOBJ.Status_Code = DM_SUCCESS;
				RequestResponseOBJ.Message_Id = std::to_string(
						DMRespStructZWOBJ.MsgId);
				RequestResponseOBJ.Client =
						std::stol(
								std::string(
										std::to_string(DMRespStructZWOBJ.MsgId)).substr(
										0, 2));

				RequestResponseOBJ.Thing_Id = "ZW_"
						+ std::to_string(DMRespStructZWOBJ.HomeId) + "_"
						+ std::to_string(DMRespStructZWOBJ.NodeId);

				/*syslog(LOG_DEBUG,
						"[DM]: Zwave Framework remove response for thing_id: [%s]",
						RequestResponseOBJ.Thing_Id.c_str());*/

				bool ThingRemoveStatus = DmFrameworkHandle::StopReqHandleThread(
						std::string("ZW")
								+ std::to_string(DMRespStructZWOBJ.MsgId));

				if (ThingRemoveStatus == true) {

					syslog(LOG_DEBUG,
							"[DM]: Zwave Framework exclusion response thing_id: [%s]",
							RequestResponseOBJ.Thing_Id.c_str());

					RequestResponseOBJ.Command = DM_EXCLUSION;
					RequestResponseOBJ.Message = "Exclusion completed successfully";

					if (!RequestResponseOBJ.Thing_Id.empty()) {
						DmPlatform::DmSendingResponse(RequestResponseOBJ);

					} else {
						syslog(LOG_ERR,
								"[DM]: Zwave Framework exclusion response stopped processing empty data to TronX");
					}

				} else {
					syslog(LOG_INFO,
							"[DM]: Zwave Framework exclusion response message_id not found request map");
				}

			}
				break;

			case MODE_FORCE_REMOVE: {

				syslog(LOG_INFO,
						"[DM]: Zwave Framework force remove response start");

				RequestResponse RequestResponseOBJ;

				DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

				RequestResponseOBJ.Status_Code = DM_SUCCESS;
				RequestResponseOBJ.Message_Id = std::to_string(
						DMRespStructZWOBJ.MsgId);
				RequestResponseOBJ.Client =
						std::stol(
								std::string(
										std::to_string(DMRespStructZWOBJ.MsgId)).substr(
										0, 2));

				RequestResponseOBJ.Thing_Id = "ZW_"
						+ std::to_string(DMRespStructZWOBJ.HomeId) + "_"
						+ std::to_string(DMRespStructZWOBJ.NodeId);

				syslog(LOG_DEBUG,
						"[DM]: Zwave Framework remove response for thing_id: [%s]",
						RequestResponseOBJ.Thing_Id.c_str());

				bool ThingRemoveStatus = DmFrameworkHandle::StopReqHandleThread(
						std::string("ZW")
								+ std::to_string(DMRespStructZWOBJ.MsgId));

				if (ThingRemoveStatus == true) {

					std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
							DmThingMapObj->DmThingDataMap.find(
									RequestResponseOBJ.Thing_Id);
					if (DmThingMapObjItr
							!= DmThingMapObj->DmThingDataMap.end()) {

						RequestResponseOBJ.Command = DM_FORCE_REMOVE;
						RequestResponseOBJ.Message = "Remove completed successfully";

						if (!RequestResponseOBJ.Thing_Id.empty()) {
							DmPlatform::DmSendingResponse(RequestResponseOBJ);
						} else {
							syslog(LOG_ERR,
									"[DM]: Zwave Framework remove response stopped processing empty data to TronX");
						}

					}
				} else {
					syslog(LOG_INFO,
							"[DM]: Zwave Framework remove response message_id not found request map");
				}

			}
				break;

			default: {

			}
				break;
			}
			/*Zwave Framework SUCCESS END */
		} else {

			/* Zwave Framework ERROR START*/

			RequestResponse RequestResponseOBJ;

			syslog(LOG_DEBUG,
					"[DM]: Zwave Framework receive error response: [%d]",
					DMRespStructZWOBJ.ErrorCode);

			bool ThingRemoveStatus;
			/* bool ThingRemoveStatus = DmFrameworkHandle::StopReqHandleThread(
			 std::string("ZW")
			 + std::to_string(DMRespStructZWOBJ.MsgId));

			 if (ThingRemoveStatus == true) {

			 syslog(LOG_DEBUG,
			 "[DM]: Zwave Framework receive error response message_id found in request map: [%s]",
			 std::to_string(DMRespStructZWOBJ.MsgId).c_str());*/

			// changes made are first checking command and then checking the message id in ReqHandleMap

			RequestResponseOBJ.Message_Id = std::to_string(
					DMRespStructZWOBJ.MsgId);

			RequestResponseOBJ.Client = std::stol(
					RequestResponseOBJ.Message_Id.substr(0, 2));

			RequestResponseOBJ.Thing_Id = "ZW_"
					+ std::to_string(DMRespStructZWOBJ.HomeId) + "_"
					+ std::to_string(DMRespStructZWOBJ.NodeId);

			switch (DMRespStructZWOBJ.Operation) {

			case MODE_COMMISSIONING: {
				RequestResponseOBJ.Command = DM_DISCOVERY;
			}
				break;
			case MODE_CONTROL: {
				RequestResponseOBJ.Property_Name =
						DmFrameworkHandle::PropertyNameStopReqHandleThread(
								std::string("ZW")
										+ std::to_string(
												DMRespStructZWOBJ.MsgId));

				if (RequestResponseOBJ.Property_Name != "DM_DEFAULT") {

					RequestResponseOBJ.Command = DM_CONTROL;
					RequestResponseOBJ.Entity_Id =
							std::to_string(
									DMRespStructZWOBJ.EndPoint[0].EndPointId)
									+ "_"
									+ std::to_string(
											DMRespStructZWOBJ.EndPoint[0].GenericDeviceClass)
									+ "_"
									+ std::to_string(
											DMRespStructZWOBJ.EndPoint[0].SpecificDevice);

				} else
					RequestResponseOBJ.Command = "DM_DEFAULT";
			}

				break;
			case MODE_OBSERVE: {
				RequestResponseOBJ.Command = DM_OBSERVE;
				/* We are checking with the Error code 7 for battery */
				if (DMRespStructZWOBJ.ErrorCode == 7) {
					RequestResponseOBJ.Message_Id = std::to_string(
							DM_OBSERVE_MSG_ID);
					syslog(LOG_DEBUG,
							"[DM]: Zwave Framework OBSERVE Response for  BATTERY");
					DmThingMap* DmThingMapObj = DmThingMap::GetInstance();
					std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
							DmThingMapObj->DmThingDataMap.find(
									RequestResponseOBJ.Thing_Id);
					if (DmThingMapObjItr
							!= DmThingMapObj->DmThingDataMap.end()) {
						/* setting the battery status in map and DB*/
						DmThingMapObjItr->second->setbattery_status(
								std::to_string(
										DMRespStructZWOBJ.EndPoint[0].InterFace[0].PropertyValue.CharValue));
						DmPlatform::DMDeviceDetailsUpdate(
								RequestResponseOBJ.Thing_Id);
					}

				}
			}
				break;
			case MODE_REMOVE: {
				ThingRemoveStatus = DmFrameworkHandle::StopReqHandleThread(
						std::string("ZW")
								+ std::to_string(DMRespStructZWOBJ.MsgId));

				if (ThingRemoveStatus == true)
					RequestResponseOBJ.Command = DM_REMOVE;
				else
					RequestResponseOBJ.Command = "DM_DEFAULT";
			}
				break;
			case MODE_EXCLUSION: {
				ThingRemoveStatus = DmFrameworkHandle::StopReqHandleThread(
						std::string("ZW")
								+ std::to_string(DMRespStructZWOBJ.MsgId));

				if (ThingRemoveStatus == true)
					RequestResponseOBJ.Command = DM_EXCLUSION;
				else
					RequestResponseOBJ.Command = "DM_DEFAULT";

			}
				break;
			case MODE_FORCE_REMOVE: {
				ThingRemoveStatus = DmFrameworkHandle::StopReqHandleThread(
						std::string("ZW")
								+ std::to_string(DMRespStructZWOBJ.MsgId));

				if (ThingRemoveStatus == true)
					RequestResponseOBJ.Command = DM_FORCE_REMOVE;
				else
					RequestResponseOBJ.Command = "DM_DEFAULT";
			}
				break;
			case MODE_RESET: {
				RequestResponseOBJ.Command = DM_IOT_RESET;
			}
				break;
			case MODE_REFRESH: {
				RequestResponseOBJ.Command = DM_CONTROL;
			}
				break;

			case MODE_S2_SECURITY: {
				RequestResponseOBJ.Command = DM_S2_SECURITY;
			}
				break;
			case MODE_LEARN_MODE: {
				RequestResponseOBJ.Command = DM_LEARN_MODE;
			}
				break;

			default: {
				RequestResponseOBJ.Command = "DM_DEFAULT";
			}
				break;
			}

			RequestResponseOBJ.Client = std::stol(
					RequestResponseOBJ.Message_Id.substr(0, 2));

			RequestResponseOBJ.Message = ZwMappingDatabase::DMErrorTypeZW(
					DMRespStructZWOBJ.ErrorCode);

			RequestResponseOBJ.Status_Code = ZwMappingDatabase::DMErrorCodeZW(
					DMRespStructZWOBJ.ErrorCode);

			if (!RequestResponseOBJ.Thing_Id.empty() && RequestResponseOBJ.Command != DM_DISCOVERY) {
				DmPlatform::DmSendingResponse(RequestResponseOBJ);
			} else {
				syslog(LOG_ERR,
						"[DM]: Zwave Framework error code response stopped processing empty data to TronX");
			}

			/*} else {

			 syslog(LOG_DEBUG,
			 "[DM]: Zwave Framework receive error response message_id not found in request map: [%s]",
			 std::to_string(DMRespStructZWOBJ.MsgId).c_str());

			 if(DMRespStructZWOBJ.Operation == MODE_OBSERVE)
			 DMRespStructZWOBJ.MsgId = DM_OBSERVE_MSG_ID;

			 RequestResponseOBJ.Message_Id = std::to_string(
			 DMRespStructZWOBJ.MsgId);

			 RequestResponseOBJ.Client = std::stol(
			 RequestResponseOBJ.Message_Id.substr(0, 2));

			 RequestResponseOBJ.Thing_Id = "ZW_"
			 + std::to_string(DMRespStructZWOBJ.HomeId) + "_"
			 + std::to_string(DMRespStructZWOBJ.NodeId);

			 switch (DMRespStructZWOBJ.Operation) {

			 case MODE_COMMISSIONING: {
			 RequestResponseOBJ.Command = DM_DISCOVERY;
			 }
			 break;
			 case MODE_CONTROL: {
			 RequestResponseOBJ.Command = "DM_DEFAULT"; // Control Error response should not be sent in case of message id is not found
			 }
			 break;
			 case MODE_OBSERVE: {
			 RequestResponseOBJ.Command = DM_OBSERVE;
			 }
			 break;
			 case MODE_REMOVE: {
			 RequestResponseOBJ.Command = "DM_DEFAULT";
			 }
			 break;
			 case MODE_EXCLUSION: {
			 RequestResponseOBJ.Command = "DM_DEFAULT";
			 }
			 break;
			 case MODE_FORCE_REMOVE: {
			 RequestResponseOBJ.Command = "DM_DEFAULT";
			 }
			 break;
			 case MODE_RESET: {
			 RequestResponseOBJ.Command = DM_IOT_RESET;
			 }
			 break;
			 case MODE_REFRESH: {
			 RequestResponseOBJ.Command = DM_CONTROL;
			 }
			 break;

			 case MODE_S2_SECURITY: {
			 RequestResponseOBJ.Command = DM_S2_SECURITY;
			 }
			 break;

			 case MODE_LEARN_MODE: {
			 RequestResponseOBJ.Command = DM_LEARN_MODE;
			 }
			 break;

			 default: {
			 RequestResponseOBJ.Command = "DM_DEFAULT";
			 }
			 break;
			 }

			 RequestResponseOBJ.Client = std::stol(
			 RequestResponseOBJ.Message_Id.substr(0, 2));

			 RequestResponseOBJ.Message = ZwMappingDatabase::DMErrorTypeZW(
			 DMRespStructZWOBJ.ErrorCode);

			 RequestResponseOBJ.Status_Code =
			 ZwMappingDatabase::DMErrorCodeZW(
			 DMRespStructZWOBJ.ErrorCode);

			 DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

			 std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
			 DmThingMapObj->DmThingDataMap.find(
			 RequestResponseOBJ.Thing_Id);
			 if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {
			 switch (DMRespStructZWOBJ.ErrorCode) {
			 case 3: {

			 DmThingMapObjItr->second->setthing_status("online");
			 DmPlatform::DMDeviceDetailsUpdate(
			 RequestResponseOBJ.Thing_Id);
			 }
			 break;
			 case 4: {
			 DmThingMapObjItr->second->setthing_status("offline");
			 DmPlatform::DMDeviceDetailsUpdate(
			 RequestResponseOBJ.Thing_Id);
			 }
			 break;
			 default:
			 break;
			 }
			 }

			 if (!RequestResponseOBJ.Thing_Id.empty()) {
			 DmPlatform::DmSendingResponse(RequestResponseOBJ);
			 } else {
			 syslog(LOG_ERR,
			 "[DM]: Zwave Framework error code response stopped processing empty data to TronX");
			 }

			 }*/

		
		}

		/*Zwave Framework ERROR END*/
		//syslog(LOG_INFO, "[DM]: Zwave Framework end receive");
	}

}

void ZwFramework::stopdiscovery(void) {

	DMReqStructZW DMReqStructZWObjStopDisc;

	memset(&DMReqStructZWObjStopDisc, 0, sizeof(DMReqStructZWObjStopDisc));

	DMReqStructZWObjStopDisc.Mtype = DM_MQ_1;
	DMReqStructZWObjStopDisc.Operation = MODE_STOP_DISC;

	ZwMessageQueue::ZwaveMsgQueueReq(DMReqStructZWObjStopDisc);

	/*syslog(LOG_INFO,
			"[DM]: Zwave Framework request discovery stop request send to Zwave MQ");*/
}
