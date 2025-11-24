#include "dm_thing.hpp"
#include "dm_thingmap.hpp"
#include "zb_framework.hpp"
#include "dm_framework.hpp"
#include "zb_message_queue.hpp"
#include "zb_mapping_database.hpp"
#include "dm_thingmap.hpp"
#include "dm_led_interface.hpp"
#include "dm_utility.hpp"
#include "dm_things_backup_db.hpp"


#define CONATCT_OR_ALARM_POSITION 1
#define ZERO_LENGTH 0
extern bool FlagDiscSend;
extern bool FlagFota;
extern std::string FotaThingID, FotaMsgID;
extern int sockfd;
extern sockaddr_in fota_servaddr;

extern pthread_mutex_t LockMutexDisc;
extern pthread_cond_t DM_Req_Control;

extern char DM_DISCOVERY_TIMEOUT[8];
extern char DM_REMOVE_TIMEOUT[8];
extern char DM_CONTROL_TIMEOUT[8];
extern char DM_ZBFOTA_TIMEOUT[8];

/*
ZbFramework::ZbFramework() {
}
ZbFramework::~ZbFramework() {
}
*/

bool ZbFramework::DMMessageQueueDiscInitZB() {

	if (ZbMessageQueue::ZbMsgReadDiscTest() == true) {

		//syslog(LOG_INFO, "[DM]: Zigbee framework discovery message queue init success");

		return true;

	} else {

		//syslog(LOG_INFO, "[DM]: Zigbee framework discovery message queue init fail");

		return false;
	}
}

bool ZbFramework::DMMessageQueueInitZB() {

	if ((ZbMessageQueue::ZbMsgSendTest() == true)
			&& (ZbMessageQueue::ZbMsgReadTest() == true)
			&& (ZbMappingDatabase::ZigbeeDatabaseOpen() == true)) {

		//syslog(LOG_INFO, "[DM]: Zigbee framework message queue init success");

		return true;

	} else {

		//syslog(LOG_INFO, "[DM]: Zigbee framework message queue init fail");

		return false;
	}
}

void ZbFramework::discovery(RequestResponse RequestResponseOBJ) {

//	syslog(LOG_INFO, "[DM]: Zigbee framework discovery start");

	DMReqStructZB DMReqStructZBOBJ;
	memset(&DMReqStructZBOBJ, 0, sizeof(DMReqStructZBOBJ));
	DMReqStructZBOBJ.mtype = 1;
	DMReqStructZBOBJ.timeout = std::stol(DM_DISCOVERY_TIMEOUT) - 2;
	DMReqStructZBOBJ.MsgId = std::stol(RequestResponseOBJ.Message_Id);
	DMReqStructZBOBJ.Mode = MODE_ID_COMMISINING;

	bool DiscSend = ZbMessageQueue::ZigbeeMsgQueueReq(DMReqStructZBOBJ);

	syslog(LOG_INFO, "[DM]: Zigbee framework discovery start status: [%d]",
			DiscSend);
//--------------------------------------------START DISCOVERY TIMER THERAD------------------------------------------------//
	if (DiscSend == true) {

		time_t seconds = time(NULL);

		while (stopRequested() == false
				&& (seconds + std::stol(DM_DISCOVERY_TIMEOUT)) != time(NULL)) {

		/*	syslog(LOG_DEBUG,
					"[DM]: Zigbee discovery thread started waiting for .. [%ld - %s]",
					time(NULL) - seconds,
					(std::string("ZB") + RequestResponseOBJ.Message_Id).c_str());*/

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

		if (stopRequested() == false
				&& (seconds + std::stol(DM_DISCOVERY_TIMEOUT)) <= time(NULL)) {

			pthread_mutex_lock(&LockMutexDisc);

			if (FlagDiscSend == true) {

				FlagDiscSend = false;

				bool RemoveStatusDiscReqZB =
						DmFrameworkHandle::RemoveReqHandleMapId(
								std::string("ZB")
										+ RequestResponseOBJ.Message_Id);

				if (RemoveStatusDiscReqZB == true) {
					syslog(LOG_DEBUG,
							"[DM]: Zigbee discovery thread removed for id:[%s]",
							(std::string("ZB") + RequestResponseOBJ.Message_Id).c_str());
				} else {
					syslog(LOG_DEBUG,
							"[DM]: Zigbee discovery thread not removed for id:[%s]",
							(std::string("ZB") + RequestResponseOBJ.Message_Id).c_str());
				}

				bool RemoveStatusDiscReqZW =
						DmFrameworkHandle::RemoveReqHandleMapId(
								std::string("ZW")
										+ RequestResponseOBJ.Message_Id);

				if (RemoveStatusDiscReqZW == true) {
					syslog(LOG_DEBUG,
							"[DM]: Zigbee discovery thread removed for id:[%s]",
							(std::string("ZW") + RequestResponseOBJ.Message_Id).c_str());
				} else {
					syslog(LOG_DEBUG,
							"[DM]: Zigbee discovery thread not removed for id:[%s]",
							(std::string("ZW") + RequestResponseOBJ.Message_Id).c_str());
				}

				syslog(LOG_INFO,
						"[DM]: Zigbee framework request discovery zwave response timeout ");

				/*Sending timeout response platform*/
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
				std::string("ZB") + RequestResponseOBJ.Message_Id);
		syslog(LOG_INFO,
				"[DM]: Zigbee framework request commissioning not send from zb");
	}
	/*End discovery timer therad*/
}

void ZbFramework::control(RequestResponse RequestResponseOBJ) {
	//syslog(LOG_INFO, "[DM]: Zigbee framework control start");

	DMReqStructZB DMReqStructZBOBJ;
	memset(&DMReqStructZBOBJ, 0, sizeof(DMReqStructZBOBJ));
	DMReqStructZBOBJ.mtype = 1;

//	syslog(LOG_INFO, "[DM]: Zigbee framework request control start");

	DMReqStructZBOBJ.MsgId = std::stol(RequestResponseOBJ.Message_Id);

	DMReqStructZBOBJ.timeout = std::stol(DM_CONTROL_TIMEOUT) - 1;

	DMReqStructZBOBJ.Mode = MODE_ID_CONTROL;

	DMReqStructZBOBJ.endpoint = std::stoi(RequestResponseOBJ.Entity_Id);

	std::stringstream IEEE_ID(
			std::string(RequestResponseOBJ.Thing_Id).erase(0, 2));
	IEEE_ID >> DMReqStructZBOBJ.IEEE;

	DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

	DmThing* DmThingOBJ = new DmThing;

	std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
			DmThingMapObj->DmThingDataMap.find(RequestResponseOBJ.Thing_Id);
	if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {

		DmThingOBJ = DmThingMapObjItr->second;

		std::stringstream NODE_ID(DmThingOBJ->gethome_node_id());
		NODE_ID >> DMReqStructZBOBJ.nodeId;

		DMReqStructZBOBJ.deviceId = ZbMappingDatabase::DMDeviceIDZB(
				DmThingOBJ->getthing_name());

		for (int EntityNo = 0; EntityNo < DmThingOBJ->getEntityListSize();
				EntityNo++) {

			if (DmThingOBJ->getEntityInfo(EntityNo)->getentity_id()
					== std::string(RequestResponseOBJ.Entity_Id)) {

				for (int PropertyNo = 0;
						PropertyNo
								< DmThingOBJ->getEntityInfo(EntityNo)->getPropertySize();
						PropertyNo++) {

					if (DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
							PropertyNo)->getproperty_type()
							== std::string(RequestResponseOBJ.Property_Type)) {

						syslog(LOG_INFO,
										"[DM]: Zigbee framework request control starts for :%s  ",
										("DM "+ RequestResponseOBJ.Property_Type).c_str());

						switch (ConvertStringToInt(
								std::string("DM_")
										+ RequestResponseOBJ.Property_Type)) {

						case DM_SWITCH: {

						//	syslog(LOG_INFO,
							//		"[DM]: Zigbee framework request control temp map switch start  ");

							DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
									PropertyNo)->setproperty_value(
									RequestResponseOBJ.Property_Value);

							/*syslog(LOG_INFO,
									"[DM]: Zigbee framework request control for ZB switch");*/

							DMReqStructZBOBJ.commandId = (uint16_t) std::stol(
									RequestResponseOBJ.Property_Value);

							DMReqStructZBOBJ.clusterId =
									ZbMappingDatabase::DMClusterIDZB(
											RequestResponseOBJ.Property_Name);

							json_object* ATRCount =
									json_tokener_parse(
											ZbMappingDatabase::DMAtrTypeIDZB(
													RequestResponseOBJ.Property_Name).c_str());

							json_type jsontypeofATRCount = json_object_get_type(
									ATRCount);

							/** Checking whether the json structure of ATRCount is array or not */

							if (jsontypeofATRCount == json_type_array) {
								DMReqStructZBOBJ.argcount =
										json_object_array_length(ATRCount);
							} else {
								DMReqStructZBOBJ.argcount = ZERO_LENGTH;
							}

							for (size_t ArgCount = 0;
									ArgCount < DMReqStructZBOBJ.argcount;
									ArgCount++) {

								json_object* DataType =
										json_object_array_get_idx(ATRCount,
												ArgCount);

								DMReqStructZBOBJ.argValue_info[ArgCount].argtype =
										json_object_get_int(DataType);

								if (RequestResponseOBJ.Property_Name ==
								DOOR_STATE) {

								 //	syslog(LOG_INFO,
									//		"[DM]: Zigbee framework request inside door lock");

									switch (std::stoi(
											RequestResponseOBJ.Property_Value)) {

									case 0: {

										syslog(LOG_INFO,
												"[DM]: Zigbee framework request unlock DOOR ");

										DMReqStructZBOBJ.argValue_info[ArgCount].argValue.intargValue =
												1;
										DMReqStructZBOBJ.commandId = 1;
									}
										break;

									case 1: {

										syslog(LOG_INFO,
												"[DM]: Zigbee framework request lock DOOR ");

										DMReqStructZBOBJ.argValue_info[ArgCount].argValue.intargValue =
												0;
										DMReqStructZBOBJ.commandId = 0;
									}
										break;

									default: {
										syslog(LOG_INFO,
												"[DM]: Zigbee framework request lock else");
									}
										break;

									}
								}
							}

							if (DMReqStructZBOBJ.commandId == 1) {

								for (int property_no = 0;
										property_no
												< DmThingOBJ->getEntityInfo(
														EntityNo)->getPropertySize();
										property_no++) {

									if (DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
											property_no)->getproperty_name() == BRIGHTNESS) {

										std::string prv_val =
												DmThingOBJ->getEntityInfo(
														EntityNo)->getPropertyInfo(
														property_no)->getproperty_value();

										if (std::stoi(prv_val) == 0) {
											DMReqStructZBOBJ.commandId =
													ZbMappingDatabase::DMommandIDZB(
													BRIGHTNESS);

											DMReqStructZBOBJ.clusterId =
													ZbMappingDatabase::DMClusterIDZB(
													BRIGHTNESS);
											/**
											 * Retriving Attribute details from Zigbee Database
											 */
											json_object* ATRCount =
													json_tokener_parse(
															ZbMappingDatabase::DMAtrTypeIDZB(
															BRIGHTNESS).c_str());
											json_type jsontypeofATRCount =
													json_object_get_type(
															ATRCount);
											/** Checking whether the json structure of ATRCount is array or not */

											if (jsontypeofATRCount
													== json_type_array) {
												DMReqStructZBOBJ.argcount =
														json_object_array_length(
																ATRCount);

											} else {
												DMReqStructZBOBJ.argcount =
												ZERO_LENGTH;
											}

											for (size_t ArgCount = 0;
													ArgCount
															< DMReqStructZBOBJ.argcount;
													ArgCount++) {
												json_object* DataType =
														json_object_array_get_idx(
																ATRCount,
																ArgCount);
												DMReqStructZBOBJ.argValue_info[ArgCount].argtype =
														json_object_get_int(
																DataType);

												switch (ArgCount) {
												case 0: {
													DMReqStructZBOBJ.argValue_info[0].argValue.intargValue =
															255;
													DmThingOBJ->getEntityInfo(
															EntityNo)->getPropertyInfo(
															property_no)->setproperty_value(
															"100");

												}
													break;
												case 2: {
													DMReqStructZBOBJ.argValue_info[ArgCount].argValue.intargValue =
													TRANSMISSION_VALUE;
												}
													break;
												default:
													syslog(LOG_INFO,
															"[DM]: Zigbee framework request control range value default");
													break;
												}
											}
											//json_object_put(ATRCount);
											break;
										}
									}
								}
							}

							/**
							 *  End for Brightness check loop
							 */

						//	syslog(LOG_INFO,
							//		"[DM]: Zigbee framework request control switch zb mq structure end");
							//json_object_put(ATRCount);	
						}
							break;

						case DM_RANGE: {

							/*syslog(LOG_INFO,
									"[DM]: Zigbee framework request control RANGE start  ");*/

							std::string pro_name =
									DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
											PropertyNo)->getproperty_name();
							//	syslog(LOG_INFO,
							//		"[DM]: Zigbee framework request control range ZB mq structure start");

							DMReqStructZBOBJ.commandId =
									ZbMappingDatabase::DMommandIDZB(
											RequestResponseOBJ.Property_Name);
							DMReqStructZBOBJ.clusterId =
									ZbMappingDatabase::DMClusterIDZB(
											RequestResponseOBJ.Property_Name);

							if (RequestResponseOBJ.Property_Name ==
							BRIGHTNESS
									&& RequestResponseOBJ.Property_Name
											== pro_name) {
								DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
										PropertyNo)->setproperty_value(
										RequestResponseOBJ.Property_Value);
								json_object* ATRCount =
										json_tokener_parse(
												ZbMappingDatabase::DMAtrTypeIDZB(
														RequestResponseOBJ.Property_Name).c_str());

								json_type jsontypeofATRCount =
										json_object_get_type(ATRCount);
								/** Checking whether the json structure of ATRCount is array or not */

								if (jsontypeofATRCount == json_type_array) {
									DMReqStructZBOBJ.argcount =
											json_object_array_length(ATRCount);
								} else {
									DMReqStructZBOBJ.argcount = ZERO_LENGTH;
								}

								for (size_t ArgCount = 0;
										ArgCount < DMReqStructZBOBJ.argcount;
										ArgCount++) {

									json_object* DataType =
											json_object_array_get_idx(ATRCount,
													ArgCount);

									DMReqStructZBOBJ.argValue_info[ArgCount].argtype =
											json_object_get_int(DataType);

									switch (ArgCount) {
									case 0: {

										DMReqStructZBOBJ.argValue_info[ArgCount].argValue.intargValue =
												std::stoi(
														RequestResponseOBJ.Property_Value)
														* 255 / 100;
									}
										break;
									case 1: {

										DMReqStructZBOBJ.argValue_info[ArgCount].argValue.intargValue =
										TRANSMISSION_VALUE;

									}
										break;
									default:
										syslog(LOG_INFO,
												"[DM]: Zigbee framework request control range value default");
										break;
									}
								}
								//json_object_put(ATRCount);
							} else if (RequestResponseOBJ.Property_Name
									== COLOR_TEMPERATURE
									&& RequestResponseOBJ.Property_Name
											== pro_name) {
								DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
										PropertyNo)->setproperty_value(
										RequestResponseOBJ.Property_Value);
								json_object* ATRCount =
										json_tokener_parse(
												ZbMappingDatabase::DMAtrTypeIDZB(
														RequestResponseOBJ.Property_Name).c_str());
								json_type jsontypeofATRCount =
										json_object_get_type(ATRCount);
								/** Checking whether the json structure of ATRCount is array or not */

								if (jsontypeofATRCount == json_type_array) {
									DMReqStructZBOBJ.argcount =
											json_object_array_length(ATRCount);
								} else {
									DMReqStructZBOBJ.argcount = ZERO_LENGTH;
								}

								for (size_t ArgCount = 0;
										ArgCount < DMReqStructZBOBJ.argcount;
										ArgCount++) {

									json_object* DataType =
											json_object_array_get_idx(ATRCount,
													ArgCount);

									DMReqStructZBOBJ.argValue_info[ArgCount].argtype =
											json_object_get_int(DataType);

									switch (ArgCount) {
									case 0: {
										DMReqStructZBOBJ.argValue_info[ArgCount].argValue.intargValue =
												(153
														+ (int) ((368 - 153)
																* (std::stoi(
																		RequestResponseOBJ.Property_Value)
																		/ (double) 100)));
									}
										break;

									case 1: {

										DMReqStructZBOBJ.argValue_info[ArgCount].argValue.intargValue =
										TRANSMISSION_VALUE;
									}
										break;
									default:
										syslog(LOG_INFO,
												"[DM]: Zigbee framework request control range value default");
										break;
									}
								}
									//json_object_put(ATRCount);
							} else if (RequestResponseOBJ.Property_Name ==
							FANSPEED
									&& RequestResponseOBJ.Property_Name
											== pro_name) {
								DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
										PropertyNo)->setproperty_value(
										RequestResponseOBJ.Property_Value);
								json_object* ATRCount =
										json_tokener_parse(
												ZbMappingDatabase::DMAtrTypeIDZB(
														RequestResponseOBJ.Property_Name).c_str());
								json_type jsontypeofATRCount =
										json_object_get_type(ATRCount);
								/** Checking whether the json structure of ATRCount is array or not */

								if (jsontypeofATRCount == json_type_array) {
									DMReqStructZBOBJ.argcount =
											json_object_array_length(ATRCount);
								} else {
									DMReqStructZBOBJ.argcount = ZERO_LENGTH;
								}

								for (size_t ArgCount = 0;
										ArgCount < DMReqStructZBOBJ.argcount;
										ArgCount++) {
									json_object* DataType =
											json_object_array_get_idx(ATRCount,
													ArgCount);
									DMReqStructZBOBJ.argValue_info[ArgCount].argtype =
											json_object_get_int(DataType);

									switch (ArgCount) {
									case 0: {
										DMReqStructZBOBJ.argValue_info[0].argValue.intargValue =
												std::stoi(
														RequestResponseOBJ.Property_Value)
														* 255 / 100;
									}
										break;
									case 1: {
										DMReqStructZBOBJ.argValue_info[ArgCount].argValue.intargValue =
										TRANSMISSION_VALUE;
									}
										break;
									default:
										syslog(LOG_INFO,
												"[DM]: Zigbee framework request control range value default");
										break;
									}
								}

							}

							//syslog(LOG_INFO,
								//	"[DM]: Zigbee framework request control range zb mq structure end");

						}
							break;

						case DM_COLOR: {

							/*syslog(LOG_INFO,
									"[DM]: Zigbee framework request control COLOR start  ");*/

							DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
									PropertyNo)->setproperty_value(
									RequestResponseOBJ.Property_Value);

					//		syslog(LOG_INFO,
						//			"[DM]: Zigbee framework request control color zb mq structure start");

							DMReqStructZBOBJ.commandId =
									ZbMappingDatabase::DMommandIDZB(
											RequestResponseOBJ.Property_Name);

							DMReqStructZBOBJ.clusterId =
									ZbMappingDatabase::DMClusterIDZB(
											RequestResponseOBJ.Property_Name);

							RGBVALUE RGBVALUEObj;
							XYVALUE XYVALUEObj;

							RGBVALUEObj = convertHEXAtoRGB(
									RequestResponseOBJ.Property_Value);

							XYVALUEObj = convertRGBtoXY(RGBVALUEObj.Red,
									RGBVALUEObj.Green, RGBVALUEObj.Blue);

							json_object* ATRCount =
									json_tokener_parse(
											ZbMappingDatabase::DMAtrTypeIDZB(
													RequestResponseOBJ.Property_Name).c_str());
							json_type jsontypeofATRCount = json_object_get_type(
									ATRCount);
							/** Checking whether the json structure of ATRCount is array or not */

							if (jsontypeofATRCount == json_type_array) {
								DMReqStructZBOBJ.argcount =
										json_object_array_length(ATRCount);
							} else {
								DMReqStructZBOBJ.argcount = ZERO_LENGTH;
							}

							json_object* DataType;

							for (size_t ArgCount = 0;
									ArgCount < DMReqStructZBOBJ.argcount;
									ArgCount++) {

								switch (ArgCount) {

								case 0:
									DataType = json_object_array_get_idx(
											ATRCount, ArgCount);
									DMReqStructZBOBJ.argValue_info[ArgCount].argtype =
											json_object_get_int(DataType);

									DMReqStructZBOBJ.argValue_info[ArgCount].argValue.intargValue =
											XYVALUEObj.X;
									break;
								case 1:
									DataType = json_object_array_get_idx(
											ATRCount, ArgCount);
									DMReqStructZBOBJ.argValue_info[ArgCount].argtype =
											json_object_get_int(DataType);
									DMReqStructZBOBJ.argValue_info[ArgCount].argValue.intargValue =
											XYVALUEObj.Y;
									break;
								case 2:
									DataType = json_object_array_get_idx(
											ATRCount, ArgCount);
									DMReqStructZBOBJ.argValue_info[ArgCount].argtype =
											json_object_get_int(DataType);
									DMReqStructZBOBJ.argValue_info[ArgCount].argValue.intargValue =
									TRANSMISSION_VALUE;
									break;
								}
							}
							//syslog(LOG_INFO,
								//	"[DM]: Zigbee framework request control color zb mq structure end");
								//json_object_put(ATRCount);							
							}
							break;

						case DM_LIST: {
							if (RequestResponseOBJ.Property_Name ==
							COVERING) {

								DMReqStructZBOBJ.clusterId =
										ZbMappingDatabase::DMClusterIDZB(
												RequestResponseOBJ.Property_Name);

							//	syslog(LOG_INFO,
								//		"[DM]: Zigbee framework request control list covering case start");

								if (RequestResponseOBJ.Property_Value
										== "OPEN") {

									syslog(LOG_INFO,
											"[DM]: Zigbee framework CURTAIN OPEN");

									DMReqStructZBOBJ.commandId = 0;
									DMReqStructZBOBJ.argValue_info[0].argValue.intargValue =
											0;
									DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
											PropertyNo)->setproperty_value(
													RequestResponseOBJ.Property_Value);
								} else if (RequestResponseOBJ.Property_Value
										== "CLOSE") {

									syslog(LOG_INFO,
											"[DM]: Zigbee framework CURTAIN CLOSE");
									DMReqStructZBOBJ.commandId = 1;
									DMReqStructZBOBJ.argValue_info[0].argValue.intargValue =
											1;
									DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
											PropertyNo)->setproperty_value(
													RequestResponseOBJ.Property_Value);

								} else {
									syslog(LOG_INFO,
											"[DM]: Zigbee framework CURTAIN STOP");
									DMReqStructZBOBJ.commandId = 2;
									DMReqStructZBOBJ.argValue_info[0].argValue.intargValue =
											2;
									DmThingOBJ->getEntityInfo(EntityNo)->getPropertyInfo(
											PropertyNo)->setproperty_value(
													RequestResponseOBJ.Property_Value);
								}

								json_object* ATRCount =
										json_tokener_parse(
												ZbMappingDatabase::DMAtrTypeIDZB(
														RequestResponseOBJ.Property_Name).c_str());
								json_type jsontypeofATRCount =
										json_object_get_type(ATRCount);
								/** Checking whether the json structure of ATRCount is array or not */

								if (jsontypeofATRCount == json_type_array) {
									DMReqStructZBOBJ.argcount =
											json_object_array_length(ATRCount);
								} else {
									DMReqStructZBOBJ.argcount = ZERO_LENGTH;
								}

								for (size_t ArgCount = 0;
										ArgCount < DMReqStructZBOBJ.argcount;
										ArgCount++) {
									json_object* DataType =
											json_object_array_get_idx(ATRCount,
													ArgCount);
									DMReqStructZBOBJ.argValue_info[ArgCount].argtype =
											json_object_get_int(DataType);
								}
								//json_object_put(ATRCount);
								//syslog(LOG_INFO,
							//			"[DM]: Zigbee framework request control list covering case end");
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
	} else {
		syslog(LOG_DEBUG,
				"[DM]: Zigbee framework request control failed to find in temp map for id:[%s]",
				RequestResponseOBJ.Thing_Id.c_str());

		/*sending timeout response platform*/
		RequestResponseOBJ.Client = std::stol(
				RequestResponseOBJ.Message_Id.substr(0, 2));
		RequestResponseOBJ.Command = DM_CONTROL;
		RequestResponseOBJ.Message = "Device does not exists";

		RequestResponseOBJ.Status_Code = THING_NOT_PRESENT_IN_DM;

		DmPlatform::DmSendingResponse(RequestResponseOBJ);
	}

	bool ControlSend = ZbMessageQueue::ZigbeeMsgQueueReq(DMReqStructZBOBJ);
	syslog(LOG_INFO, "[DM]: Zigbee framework control request status: [%d]",
			ControlSend);
	/*Start control timer therad*/
	if (ControlSend == true) {

	//	syslog(LOG_INFO,
		//		"[DM]: Zigbee framework control setting control thing object in thing control map for message_id: [%s]",
			//	RequestResponseOBJ.Message_Id.c_str());

		DmThingMapObj->SetDmThingMapCtrlObj(RequestResponseOBJ.Message_Id,
				DmThingOBJ);

		time_t seconds = time(NULL);

		while (stopRequested() == false
				&& (seconds + std::stol(DM_CONTROL_TIMEOUT)) != time(NULL)) {

			syslog(LOG_DEBUG,
					"[DM]: Zigbee control thread started waiting for .. [%ld - %s]",
					time(NULL) - seconds,
					(std::string("ZB") + RequestResponseOBJ.Message_Id).c_str());

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

		if (stopRequested() == false
				&& (seconds + std::stol(DM_CONTROL_TIMEOUT)) <= time(NULL)) {

		//	syslog(LOG_INFO, "[DM]: Zigbee control thread removed for id:[%s]",
			//		(std::string("ZB") + RequestResponseOBJ.Message_Id).c_str());

			DmFrameworkHandle::RemoveReqHandleMapId(
					std::string("ZB") + RequestResponseOBJ.Message_Id);

			syslog(LOG_INFO,
					"[DM]: Zigbee framework control response timeout ");

			/*sending timeout response platform*/
			RequestResponseOBJ.Client = std::stol(
					RequestResponseOBJ.Message_Id.substr(0, 2));
			RequestResponseOBJ.Command = DM_CONTROL;
			RequestResponseOBJ.Message = "Device Unavailable";

			RequestResponseOBJ.Status_Code = CONTROL_TIME_OUT;

			DmPlatform::DmSendingResponse(RequestResponseOBJ);

			DmFrameworkHandle::RemoveReqHandleMapId(
					std::string("ZB") + RequestResponseOBJ.Message_Id);
	//		syslog(LOG_INFO,
		//			"[DM]: Zigbee framework request control not send from zb");

			DmThingMapObj->RemoveDmThingMapCtrlObj(
					RequestResponseOBJ.Message_Id);

			DMReqStructZB DMReqStructZBOBJTimeOut;
			memset(&DMReqStructZBOBJTimeOut, 0,
					sizeof(DMReqStructZBOBJTimeOut));
			DMReqStructZBOBJTimeOut.mtype = 1;

			DMReqStructZBOBJTimeOut.MsgId = std::stol(
					RequestResponseOBJ.Message_Id);
		//	syslog(LOG_INFO,
			//		"[DM]: Zigbee framework request control operation timeout for message id %u",
				//	DMReqStructZBOBJTimeOut.MsgId);
			DMReqStructZBOBJTimeOut.Mode = MODE_ID_TIMEOUT;

			ZbMessageQueue::ZigbeeMsgQueueReq(DMReqStructZBOBJTimeOut);
		}
	} else {

		DmFrameworkHandle::RemoveReqHandleMapId(
				std::string("ZB") + RequestResponseOBJ.Message_Id);
		syslog(LOG_INFO,
				"[DM]: Zigbee framework request control not send from zb");
	}
	/*End control timer therad*/
}
void ZbFramework::remove(RequestResponse RequestResponseOBJ) {
//	syslog(LOG_INFO, "[DM]: Zigbee framework remove start");

	DMReqStructZB DMReqStructZBOBJ;
	memset(&DMReqStructZBOBJ, 0, sizeof(DMReqStructZBOBJ));
	DMReqStructZBOBJ.mtype = 1;

//	syslog(LOG_INFO, "[DM]: Zigbee framework request remove start");

	DMReqStructZBOBJ.MsgId = std::stol(RequestResponseOBJ.Message_Id);

	DMReqStructZBOBJ.timeout = std::stol(DM_REMOVE_TIMEOUT) - 1;

	DMReqStructZBOBJ.Mode = MODE_ID_REMOVE;

	std::stringstream IEEE_ID(
			std::string(RequestResponseOBJ.Thing_Id).erase(0, 2));
	IEEE_ID >> DMReqStructZBOBJ.IEEE;

	DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

	std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
			DmThingMapObj->DmThingDataMap.find(RequestResponseOBJ.Thing_Id);
	if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {

		syslog(LOG_DEBUG,
				"[DM]: Zigbee framework remove request: Thing id:[%s] FOUND",
				DmThingMapObjItr->first.c_str());

		std::stringstream NODE_ID(DmThingMapObjItr->second->gethome_node_id());
		NODE_ID >> DMReqStructZBOBJ.nodeId;

	} else {
		syslog(LOG_DEBUG,
				"[DM]: Zigbee framework request remove  Thing id:[%s] NOT FOUND",
				DmThingMapObjItr->first.c_str());
	}

	bool RemoveSend = ZbMessageQueue::ZigbeeMsgQueueReq(DMReqStructZBOBJ);
	syslog(LOG_INFO, "[DM]: Zigbee framework discovery request status: [%d]",
			RemoveSend);

	/*Start remove timer thread*/
	if (RemoveSend == true) {

		time_t seconds = time(NULL);

		while (stopRequested() == false
				&& (seconds + std::stol(DM_REMOVE_TIMEOUT)) != time(NULL)) {

			syslog(LOG_DEBUG,
					"[DM]: Zigbee remove thread started waiting for .. [%ld - %s]",
					time(NULL) - seconds,
					(std::string("ZB") + RequestResponseOBJ.Message_Id).c_str());

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

		if (stopRequested() == false
				&& (seconds + std::stol(DM_REMOVE_TIMEOUT)) <= time(NULL)) {

		//	syslog(LOG_INFO, "[DM]: Zigbee remove thread removed for id:[%s]",
			//		(std::string("ZB") + RequestResponseOBJ.Message_Id).c_str());

			DmFrameworkHandle::RemoveReqHandleMapId(
					std::string("ZB") + RequestResponseOBJ.Message_Id);

			syslog(LOG_INFO,
					"[DM]: Zigbee framework remove response timeout for id :[%s]",(std::string("ZB") + RequestResponseOBJ.Message_Id).c_str());

			/*sending timeout response platform*/
			RequestResponseOBJ.Client = std::stol(
					RequestResponseOBJ.Message_Id.substr(0, 2));
			RequestResponseOBJ.Command = DM_REMOVE;
			RequestResponseOBJ.Message = "Remove operation failed";

			RequestResponseOBJ.Status_Code = DM_FAIL;

			DmPlatform::DmSendingResponse(RequestResponseOBJ);
		}
	} else {

		DmFrameworkHandle::RemoveReqHandleMapId(
				std::string("ZB") + RequestResponseOBJ.Message_Id);
		syslog(LOG_INFO,
				"[DM]: Zigbee framework sending REMOVE request FAILED");
	}
	/*End remove timer thread*/
}

void ZbFramework::exclusion(RequestResponse RequestResponseOBJ) {
	//syslog(LOG_INFO, "[DM]: Zigbee framework exclusion start");

	/*start exclusion timer thread*/
	/*
	 time_t seconds = time(NULL);

	 while (stopRequested() == false
	 && (seconds + EXCLUSION_TIMEOUT) != time(NULL)) {

	 syslog(LOG_DEBUG,
	 "[DM]: ZIGBEE EXCLUSION THREAD STARTED WAITING FOR .. [%ld - %s]",
	 time(NULL) - seconds,
	 (std::string("ZB") + RequestResponseOBJ.Message_Id).c_str());

	 std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	 }

	 if (stopRequested() == false
	 && (seconds + EXCLUSION_TIMEOUT) <= time(NULL)) {

	 syslog(LOG_INFO,
	 "[DM]: ZIGBEE EXCLUSION THREAD REMOVED FOR ID:[%s]",
	 (std::string("ZB") + RequestResponseOBJ.Message_Id).c_str());

	 RemoveReqHandleMapId(std::string("ZB") + RequestResponseOBJ.Message_Id);
	 }
	 */
	/*end exclusion timer therad*/
}

void ZbFramework::learnmode(RequestResponse RequestResponseOBJ) {
	//syslog(LOG_INFO, "[DM]: Zigbee framework exclusion start");

	/*start exclusion timer thread*/
	/*
	 time_t seconds = time(NULL);

	 while (stopRequested() == false
	 && (seconds + EXCLUSION_TIMEOUT) != time(NULL)) {

	 syslog(LOG_DEBUG,
	 "[DM]: ZIGBEE EXCLUSION THREAD STARTED WAITING FOR .. [%ld - %s]",
	 time(NULL) - seconds,
	 (std::string("ZB") + RequestResponseOBJ.Message_Id).c_str());

	 std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	 }

	 if (stopRequested() == false
	 && (seconds + EXCLUSION_TIMEOUT) <= time(NULL)) {

	 syslog(LOG_INFO,
	 "[DM]: ZIGBEE EXCLUSION THREAD REMOVED FOR ID:[%s]",
	 (std::string("ZB") + RequestResponseOBJ.Message_Id).c_str());

	 RemoveReqHandleMapId(std::string("ZB") + RequestResponseOBJ.Message_Id);
	 }
	 */
	/*end exclusion timer therad*/
}


void ZbFramework::s2security(RequestResponse RequestResponseOBJ) {
	//syslog(LOG_INFO, "[DM]: Zigbee framework exclusion start");
}

void ZbFramework::forceremove(RequestResponse RequestResponseOBJ) {
	syslog(LOG_INFO, "[DM]: Zigbee framework exclusion start");
}

/* Function name : zigbeefota
 * Description   : Sends FOTA request to Zigbee with corresponding message format and runs thread for specified time.
 */
void ZbFramework::zigbeefota(RequestResponse RequestResponseOBJ){

	syslog(LOG_INFO, "[DM]: Zigbee framework FOTA start");


	DMReqStructZB DMReqStructZBOBJ;
	memset(&DMReqStructZBOBJ, 0, sizeof(DMReqStructZBOBJ));
	DMReqStructZBOBJ.mtype = 1;
	DMReqStructZBOBJ.timeout = std::stol(DM_ZBFOTA_TIMEOUT) - 2;
	DMReqStructZBOBJ.MsgId = std::stol(RequestResponseOBJ.Message_Id);
	DMReqStructZBOBJ.Mode = MODE_ID_ZBFOTA;
	//DMReqStructZBOBJ.endpoint = std::stoi(RequestResponseOBJ.Entity_Id);
	//DMReqStructZBOBJ.argValue_info[0].argValue.intargValue = DmThingsBackupDb::SelectFirmwareVersion(std::string(RequestResponseOBJ.Thing_Id));

	DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

	DmThing* DmThingOBJ = new DmThing;

	std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
			DmThingMapObj->DmThingDataMap.find(RequestResponseOBJ.Thing_Id);
	if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {

		DmThingOBJ = DmThingMapObjItr->second;
		DMReqStructZBOBJ.endpoint = std::stoi(DmThingOBJ->getEntityInfo(0)->getentity_id());

		std::stringstream NODE_ID(DmThingOBJ->gethome_node_id());
		NODE_ID >> DMReqStructZBOBJ.nodeId;					/* Fetching node_id from DM thing map */
	}

	bool FotaSend = ZbMessageQueue::ZigbeeMsgQueueReq(DMReqStructZBOBJ);		/*Send function for zigbee*/

	syslog(LOG_INFO, "[DM]: FOTA -> Zigbee framework discovery start status: [%d]",
			FotaSend);

	/*Start fota timer thread*/
	if (FotaSend == true) {

		time_t seconds = time(NULL);

		while (stopRequested() == false
				&& (seconds + std::stol(DM_ZBFOTA_TIMEOUT)) != time(NULL)) {		/* Handle for running the timer for specified time */

			syslog(LOG_DEBUG,
					"[DM]: Zigbee fota update thread started waiting for .. [%ld - %s]",
					time(NULL) - seconds,
					(std::string("ZB") + RequestResponseOBJ.Message_Id).c_str());

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

		if (stopRequested() == false
				&& (seconds + std::stol(DM_ZBFOTA_TIMEOUT)) <= time(NULL)) {		/* Error handle for timeout scenario */

		//	syslog(LOG_INFO, "[DM]: Zigbee remove thread removed for id:[%s]",
			//		(std::string("ZB") + RequestResponseOBJ.Message_Id).c_str());

			DmFrameworkHandle::RemoveReqHandleMapId(
					std::string("ZB") + RequestResponseOBJ.Message_Id);

			syslog(LOG_INFO,
					"[DM]: Zigbee framework fota update response timeout for id :[%s]",(std::string("ZB") + RequestResponseOBJ.Message_Id).c_str());

			/*sending timeout response platform*/
			RequestResponseOBJ.Client = std::stol(
					RequestResponseOBJ.Message_Id.substr(0, 2));
			RequestResponseOBJ.Command = "FOTA_REQ";
			RequestResponseOBJ.Message = "FOTA UPDATE TIMEOUT";

			//Error code for Timeout
			RequestResponseOBJ.Status_Code = 404;


			DmPlatform::DmSendingResponse(RequestResponseOBJ);
		}
	} else {

		/*bool RemoveStatusDiscReq = DmFrameworkHandle::RemoveReqHandleMapId(
				std::string("ZB") + RequestResponseOBJ.Message_Id);*/
		syslog(LOG_INFO,
				"[DM]: Zigbee framework sending fota update request FAILED");
	}
	/*End fota timer thread*/


}

void ZBDiscoveryResponse() {

	/*syslog(LOG_INFO,
			"[DM]: Zigbee framework Discovery Response ");*/

ZbFramework::DMMessageQueueDiscInitZB();

	while (true) {

		DMRespStructZB ZB_MsgQue_ResObj;
		memset(&ZB_MsgQue_ResObj, 0, sizeof(ZB_MsgQue_ResObj));
		ZB_MsgQue_ResObj.mtype = 1;

	//	syslog(LOG_DEBUG,
		//		"[DM]: Zigbee message queue discovery response size: [%ld]",
			//	sizeof(ZB_MsgQue_ResObj));

		ZB_MsgQue_ResObj = ZbMessageQueue::ZigbeeDiscoveryRcvResp(
				ZB_MsgQue_ResObj);

	//	syslog(LOG_INFO, "[DM]: Zigbee framework discovery response received ");

		if (ZB_MsgQue_ResObj.ErrorCode == DM_NULL) {

			bool RemoveStatusDiscRes =
					DmFrameworkHandle::StopDiscReqHandleThread(
							std::to_string(ZB_MsgQue_ResObj.MsgId), "ZB");

			if (RemoveStatusDiscRes == true) {

				DmThingMap *DmThingMapOBJ = DmThingMap::GetInstance();

				RequestResponse RequestResponseOBJ;
				RequestResponseOBJ.Thing_Id = "ZB"
						+ std::to_string(ZB_MsgQue_ResObj.IEEE);

				bool ThingPresent = DmThingMapOBJ->CheckToThingMap(
						RequestResponseOBJ.Thing_Id);

				//LED DEVICE CONNECTED
				DmLedInterface::DeviceConnected();
				FlagDiscSend = false;

				if (ThingPresent == true) {

					syslog(LOG_DEBUG,
							"[DM]: Zigbee framework discovery response already created for thing_id:[%s] with message_id:[%s]",
							RequestResponseOBJ.Thing_Id.c_str(),
							std::to_string(ZB_MsgQue_ResObj.MsgId).c_str());
				} else {

					syslog(LOG_DEBUG,
							"[DM]: Zigbee framework discovery response newly created for thing_id:[%s] with message_id:[%s]",
							RequestResponseOBJ.Thing_Id.c_str(),
							std::to_string(ZB_MsgQue_ResObj.MsgId).c_str());

					DmThing* ZBThingObj = new DmThing;

					/*syslog(LOG_INFO,
							"[DM]: Zigbee framework thing creation start ");*/

					std::string ManufacturerName(
							ZB_MsgQue_ResObj.ManFacturererName + 1,
							ZB_MsgQue_ResObj.ManFacturererName
									+ sizeof(ZB_MsgQue_ResObj.ManFacturererName));

					ZBThingObj->setmanufacturerName(ManufacturerName);

					ZBThingObj->setframeworkName("zigbee");
					ZBThingObj->setthing_id(
							std::string("ZB")
									+ std::to_string(ZB_MsgQue_ResObj.IEEE));
					ZBThingObj->setthing_status("online");
					ZBThingObj->sethome_node_id(
							std::to_string(ZB_MsgQue_ResObj.node_id));

					syslog(LOG_DEBUG,
							"[DM]: Zigbee framework end point count: %d",
							ZB_MsgQue_ResObj.endpointCount);

					for (int EndPointNo = 0;
							EndPointNo < ZB_MsgQue_ResObj.endpointCount;
							EndPointNo++) {

						if (ZbMappingDatabase::DMThingTypeZB(
								ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
								ZERO_ZONE_ID) == "Sensor") {

							for (int ClusterNo = 0;
									ClusterNo
											< ZB_MsgQue_ResObj.Endpoint[EndPointNo].clusterCount;
									ClusterNo++) {

								syslog(LOG_DEBUG,
										"[DM]: Zigbee framework cluster_id: %d cluster_id: %d",
										ClusterNo,
										ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId);

								switch (ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId) {

								case BASIC_INFO: {

							//		syslog(LOG_INFO,
								//			"[DM]: Zigbee framework in basic_info cluster ");

									/*syslog(LOG_DEBUG,
											"[DM]: Zigbee framework BASIC CLUSTER attribute count:%d",
											ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributeCount);*/

									for (int AttributeNo = 0;
											AttributeNo
													< ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributeCount;
											AttributeNo++) {

										syslog(LOG_DEBUG,
												"[DM]: Zigbee framework BASIC CLUSTER attribute id: %d",
												ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeId);

										switch (ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeId) {

										case 1:
											ZBThingObj->setfirmwareVersion(
													std::to_string(
															ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal));
											break;

										case 3:
											ZBThingObj->sethardwareVersion(
													std::to_string(
															ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal));
											break;
										case 4:
											break;
										case 7: {
											switch (ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal) {
											case 1:
												ZBThingObj->setbattery_status(
														"MainSupply");
												break;
											case 3:
												ZBThingObj->setbattery_status(
														"Battery");
												break;
											case 4:
												ZBThingObj->setbattery_status(
														"DM_DEFAULT");
												break;
											default:
												break;
											}
										}
											break;

										default:
											syslog(LOG_INFO,
													"[DM]: Zigbee framework in basic_info default case ");
											break;
										}
									}

								}
									break;
								case POWER_CONFG: {
									syslog(LOG_INFO,
											"[DM]: Zigbee framework in power_confg cluster ");
									for (int AttributeNo = 0;
											AttributeNo
													< ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributeCount;
											AttributeNo++) {
										if (ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeId
												== 33) {
											std::stringstream BATTERY_STATUS;
											BATTERY_STATUS
													<< ((ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal)
															/ 2);
											ZBThingObj->setbattery_status(
													BATTERY_STATUS.str());
										}
									}
								}
									break;

								case IAS_ZONE: {

									//create Entity
									DmEntity* ZBEntityObj = new DmEntity;

									RequestResponseOBJ.Entity_Id =
											std::to_string(
													(int) ZB_MsgQue_ResObj.Endpoint[EndPointNo].endpointId)
													+ "_"
													+ std::to_string(
															(int) ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId);

									ZBEntityObj->setentity_id(
											RequestResponseOBJ.Entity_Id);

					//				syslog(LOG_INFO,
						//					"[DM]: Zigbee framework property started extra property creating start ");

									for (int CommandIDNo = 0; CommandIDNo <= 0;
											CommandIDNo++) {

										syslog(LOG_DEBUG,
												"[DM]: Zigbee framework IAS_ZONE attribute count ias_zone: %d",
												ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributeCount);

										for (int AttributeNo = 0;
												AttributeNo
														< ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributeCount;
												AttributeNo++) {

											syslog(LOG_DEBUG,
													"[DM]: Zigbee framework IAS_ZONE attribute_id ias_zone: %d",
													ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeId);

											if (ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeId
													== ZONE_STATUS_ATTRIBUTE) {

												for (int SensorProp = 0;
														SensorProp < 10;
														SensorProp++) {

													DmProperty* ZBPropertyObj =
															new DmProperty;

													switch (SensorProp) {

													case ZBALARM: {

						//								syslog(LOG_INFO,
							//									"[DM]: Zigbee framework property started alarm creating start ");

														ZBPropertyObj->setproperty_type(
														DM_INFO_DM);

														ZBPropertyObj->setproperty_name(
																"Alarm");

														ZBPropertyObj->setproperty_range(
																"{\"max\":1,\"step\":1,\"list\":[],\"min\":0,\"unit\":\"%\"}");

														ZBPropertyObj->setproperty_value(
																std::to_string(
																		ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal
																				>> (SensorProp) & 1));
														ZBEntityObj->setpropertyInfo(
																ZBPropertyObj);

					//									syslog(LOG_INFO,
						//										"[DM]: Zigbee framework property started alarm creating end ");
													}
														break;

													case ZBTAMPER: {
						//								syslog(LOG_INFO,
								//								"[DM]: Zigbee framework property started tamper creating start ");

														ZBPropertyObj->setproperty_type(
														DM_INFO_DM);

														ZBPropertyObj->setproperty_name(
																"Tamper");

														ZBPropertyObj->setproperty_range(
																"{\"max\":1,\"step\":1,\"list\":[],\"min\":0,\"unit\":\"%\"}");

														ZBPropertyObj->setproperty_value(
																std::to_string(
																		ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal
																				>> (SensorProp) & 1));

														ZBEntityObj->setpropertyInfo(
																ZBPropertyObj);

														/*syslog(LOG_INFO,
																"[DM]: Zigbee framework property started tamper creating end ");*/
													}
														break;
													case ZBBATTERY: {
														syslog(LOG_INFO,
																"[DM]: Zigbee framework property started battery creating start ");

														//Break if the battery status is DM_DEFAULT
														if (ZBThingObj->getbattery_status()
																== "DM_DEFAULT")
															break;
														
														ZBPropertyObj->setproperty_type(
														DM_INFO_DM);

														ZBPropertyObj->setproperty_name(
																"Battery");

														ZBPropertyObj->setproperty_range(
																"{\"max\":1,\"step\":1,\"list\":[],\"min\":0,\"unit\":\"%\"}");

														ZBPropertyObj->setproperty_value(
																std::to_string(
																		ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal
																				>> (SensorProp) & 1));

														if(ZBPropertyObj->getproperty_value()=="0")
															ZBThingObj->setbattery_status("75");
														else
															ZBThingObj->setbattery_status("25");

														ZBEntityObj->setpropertyInfo(
																ZBPropertyObj);

														/*syslog(LOG_INFO,
																"[DM]: Zigbee framework property started battery creating end ");*/
													}
														break;

													case ZBBATTERYDEFECT: {
														syslog(LOG_INFO,
																"[DM]: Zigbee framework property started battery defect defect creating start ");

														//Break if the battery status is DM_DEFAULT
														if (ZBThingObj->getbattery_status()
																== "DM_DEFAULT")
															break;
														
														ZBPropertyObj->setproperty_type(
														DM_INFO_DM);

														ZBPropertyObj->setproperty_name(
																"Battery Defect");

														ZBPropertyObj->setproperty_range(
																"{\"max\":1,\"step\":1,\"list\":[],\"min\":0,\"unit\":\"%\"}");

														ZBPropertyObj->setproperty_value(
																std::to_string(
																		ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal
																				>> (SensorProp) & 1));

														ZBEntityObj->setpropertyInfo(
																ZBPropertyObj);

														/*syslog(LOG_INFO,
																"[DM]: Zigbee framework property started battery defect defect creating end ");*/
													}
														break;

													default: {
														syslog(
														LOG_DEBUG,
																"[DM]: Zigbee framework property started default case creating for: [%d]",
																SensorProp);
													}
														break;
													}
												}
											} else if (ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeId
													== ZONE_TYPE_ATTRIBUTE) {

												DmProperty* ZBPropertyObj =
														new DmProperty;

												ZBThingObj->setzone_type(
														std::to_string(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal));

												ZBThingObj->setthing_name(
														ZbMappingDatabase::DMThingNameZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal));

												ZBThingObj->setthing_type(
														ZbMappingDatabase::DMThingTypeZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal));

												ZBEntityObj->setentity_name(
														ZbMappingDatabase::DMEntityNameZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId));

												ZBEntityObj->setentity_type(
														ZbMappingDatabase::DMEntityTypeZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId));

												ZBEntityObj->setsecurity_level(
														ZbMappingDatabase::DMEntitySecurityLevelZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId));


												syslog(LOG_INFO,
														"[DM]: Zigbee framework property started IAS ZONE sensor creating start");

												ZBPropertyObj->setproperty_type(
												DM_INFO_DM);

												ZBPropertyObj->setproperty_name(
														ZbMappingDatabase::DMPropertyNameZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo],
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId));

												ZBPropertyObj->setproperty_range(
														ZbMappingDatabase::DMPropertyRangeZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo],
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId));

												ZBPropertyObj->setproperty_value(
														std::to_string(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal));

												ZBEntityObj->setpropertyInfo(
														ZBPropertyObj);

												/*syslog(LOG_INFO,
														"[DM]: Zigbee framework property started  IAS ZONE sensor creating end ");*/

											}
										}
										//Fix For Bug: 21413
										ZBEntityObj->getPropertyInfo(0)->setproperty_value(
												ZBEntityObj->getPropertyInfo(1)->getproperty_value());
									}

									ZBThingObj->setentityInfo(ZBEntityObj);

								}
									break;

								default: {

									DmEntity* ZBEntityObj = new DmEntity;

									RequestResponseOBJ.Entity_Id =
											std::to_string(
													(int) ZB_MsgQue_ResObj.Endpoint[EndPointNo].endpointId)
													+ "_"
													+ std::to_string(
															(int) ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId);

									ZBEntityObj->setentity_id(
											RequestResponseOBJ.Entity_Id);

									/*syslog(LOG_INFO,
											"[DM]: Zigbee framework in other cluster ");

									syslog(LOG_DEBUG,
											"[DM]: Zigbee framework command count: %d",
											ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdCount);*/

									for (int CommandIDNo = 0;
											(CommandIDNo
													< ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdCount)
													|| (CommandIDNo == 0);
											CommandIDNo++) {

										syslog(LOG_DEBUG,
												"[DM]: Zigbee framework cluster_id: %d",
												ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId);
										syslog(LOG_DEBUG,
												"[DM]: Zigbee framework command_id: %d",
												ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo]);

										if (ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo]
												== ZbMappingDatabase::DMCommandIDCheckZB(
														ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
														ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId,
														ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo])) {

											/*syslog(LOG_INFO,
													"[DM]: Zigbee framework found zero zone_id");

											syslog(LOG_INFO,
													"[DM]: Zigbee framework property started creating start ");*/

											DmProperty* ZBPropertyObj =
													new DmProperty;

											syslog(LOG_DEBUG,
													"[DM]: Zigbee framework attribute count: %d",
													ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributeCount);

											for (int AttributeNo = 0;
													AttributeNo
															< ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributeCount;
													AttributeNo++) {

//												int ZERO_ZONE_ID_ = 0;

/*												if (ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId
														== 0x0500&& ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeId
														== ZONE_STATUS_ATTRIBUTE) {

													ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal;
												}
*/

												syslog(LOG_DEBUG,
														"[DM]: Zigbee framework attribute_id: %d",
														ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeId);

												ZBThingObj->setzone_type(
														std::string(
																std::to_string(
																ZERO_ZONE_ID)));

												ZBThingObj->setthing_name(
														ZbMappingDatabase::DMThingNameZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZERO_ZONE_ID));

												ZBThingObj->setthing_type(
														ZbMappingDatabase::DMThingTypeZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZERO_ZONE_ID));

												ZBEntityObj->setentity_name(
														ZbMappingDatabase::DMEntityNameZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZERO_ZONE_ID,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId));

												ZBEntityObj->setentity_type(
														ZbMappingDatabase::DMEntityTypeZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZERO_ZONE_ID,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId));

												ZBEntityObj->setsecurity_level(
														ZbMappingDatabase::DMEntitySecurityLevelZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZERO_ZONE_ID,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId));

												ZBPropertyObj->setproperty_name(
														ZbMappingDatabase::DMPropertyNameZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo],
																ZERO_ZONE_ID,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId));

												ZBPropertyObj->setproperty_range(
														ZbMappingDatabase::DMPropertyRangeZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo],
																ZERO_ZONE_ID,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId));

												//Only Info type will come here
												/*syslog(
												LOG_INFO,
														"[DM]: Zigbee framework commissioning info start");*/
												ZBPropertyObj->setproperty_type(
												DM_INFO_DM);
												if (ZbMappingDatabase::DMPropertyNameZB(
														ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
														ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo],
														ZERO_ZONE_ID,
														ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId) == TEMPERATURE) {
													ZBPropertyObj->setproperty_value(
															std::to_string(
																	(ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal)
																			/ 100));
												} else if (ZbMappingDatabase::DMPropertyNameZB(
														ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
														ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo],
														ZERO_ZONE_ID,
														ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId) == HUMIDITY) {
													ZBPropertyObj->setproperty_value(
															std::to_string(
																	(ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal)
																			/ 100));
												}
												ZBEntityObj->setpropertyInfo(
														ZBPropertyObj);
												/*syslog(
												LOG_INFO,
														"[DM]: Zigbee framework commissioning info end");*/
											}
										}
									}
 									 if (ZBEntityObj->getPropertySize() != 0) {
                                                  			      ZBThingObj->setentityInfo(ZBEntityObj);
                                              				  }
								}
									break;

								}
							}
						} else {

							DmEntity* ZBEntityObj = new DmEntity;

							RequestResponseOBJ.Entity_Id =
									std::to_string(
											(int) ZB_MsgQue_ResObj.Endpoint[EndPointNo].endpointId);

							ZBEntityObj->setentity_id(
									RequestResponseOBJ.Entity_Id);

							for (int ClusterNo = 0;
									ClusterNo
											< ZB_MsgQue_ResObj.Endpoint[EndPointNo].clusterCount;
									ClusterNo++) {

								syslog(LOG_DEBUG,
										"[DM]: Zigbee framework cluster_id: %d cluster_id: %d",
										ClusterNo,
										ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId);

								switch (ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId) {

								case BASIC_INFO: {

									/*syslog(LOG_INFO,
											"[DM]: Zigbee framework in basic_info cluster ");*/

									syslog(LOG_DEBUG,
											"[DM]: Zigbee framework attribute count:%d",
											ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributeCount);

									for (int AttributeNo = 0;
											AttributeNo
													< ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributeCount;
											AttributeNo++) {

										syslog(LOG_DEBUG,
												"[DM]: Zigbee framework attribute id: %d",
												ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeId);

										switch (ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeId) {

										case 1:
											ZBThingObj->setfirmwareVersion(
													std::to_string(
															ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal));
											break;

										case 3:
											ZBThingObj->sethardwareVersion(
													std::to_string(
															ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal));
											break;
										case 4:
											break;
										case 7: {
											switch (ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal) {
											case 1:
												ZBThingObj->setbattery_status(
														"MainSupply");
												break;
											case 3:
												ZBThingObj->setbattery_status(
														"Battery");
												break;
											case 4:
												ZBThingObj->setbattery_status(
														"DCSupply");
												break;
											default:
												break;
											}
										}
											break;

										default:
											syslog(LOG_INFO,
													"[DM]: Zigbee framework in basic_info default case ");
											break;
										}
									}

								}
									break;
								case POWER_CONFG: {
									syslog(LOG_INFO,
											"[DM]: Zigbee framework in power_confg cluster ");
									for (int AttributeNo = 0;
											AttributeNo
													< ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributeCount;
											AttributeNo++) {
										if (ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeId
												== 33) {
											std::stringstream BATTERY_STATUS;
											BATTERY_STATUS
													<< ((ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal)
															/ 2);
											ZBThingObj->setbattery_status(
													BATTERY_STATUS.str());
										}
									}
								}
									break;

								default: {
									/*syslog(LOG_INFO,
											"[DM]: Zigbee framework in other cluster ");*/

									syslog(LOG_DEBUG,
											"[DM]: Zigbee framework in DEFAULT case command count: %d",
											ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdCount);

									for (int CommandIDNo = 0;
											(CommandIDNo
													< ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdCount)
													|| (CommandIDNo == 0);
											CommandIDNo++) {

										syslog(LOG_DEBUG,
												"[DM]: Zigbee framework cluster_id: %d ,command_id: %d",
												ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId,
												ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo]);

										if (ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo]
												== ZbMappingDatabase::DMCommandIDCheckZB(
														ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
														ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId,
														ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo])) {

											/*syslog(LOG_INFO,
													"[DM]: Zigbee framework found zero zone_id");

											syslog(LOG_INFO,
													"[DM]: Zigbee framework property started creating start ");*/

											DmProperty* ZBPropertyObj =
													new DmProperty;

											syslog(LOG_DEBUG,
													"[DM]: Zigbee framework attribute count: %d",
													ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributeCount);

											for (int AttributeNo = 0;
													AttributeNo
															< ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributeCount;
													AttributeNo++) {

												syslog(LOG_DEBUG,
														"[DM]: Zigbee framework attribute_id: %d",
														ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeId);

												ZBThingObj->setzone_type(
														std::string(
																std::to_string(
																ZERO_ZONE_ID)));

												ZBThingObj->setthing_name(
														ZbMappingDatabase::DMThingNameZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZERO_ZONE_ID));

												ZBThingObj->setthing_type(
														ZbMappingDatabase::DMThingTypeZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZERO_ZONE_ID));

												ZBEntityObj->setentity_name(
														ZbMappingDatabase::DMEntityNameZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZERO_ZONE_ID,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId));

												ZBEntityObj->setentity_type(
														ZbMappingDatabase::DMEntityTypeZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZERO_ZONE_ID,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId));

												ZBEntityObj->setsecurity_level(
														ZbMappingDatabase::DMEntitySecurityLevelZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZERO_ZONE_ID,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId));

												ZBPropertyObj->setproperty_name(
														ZbMappingDatabase::DMPropertyNameZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo],
																ZERO_ZONE_ID,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId));

												ZBPropertyObj->setproperty_range(
														ZbMappingDatabase::DMPropertyRangeZB(
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo],
																ZERO_ZONE_ID,
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId));

												switch (ZbMappingDatabase::DMPropertyTypeZB(
														ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
														ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo],
														ZERO_ZONE_ID,
														ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId)) {

												case SWITCH: {

													/*syslog(
													LOG_INFO,
															"[DM]: Zigbee framework commissioning switch start");*/

													ZBPropertyObj->setproperty_type(
													DM_SWITCH_DM);
													if (ZbMappingDatabase::DMPropertyNameZB(
															ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
															ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo],
															ZERO_ZONE_ID,
															ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId) == ON_OFF) {

														ZBPropertyObj->setproperty_value(
																std::to_string(
																		ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal));

														syslog(LOG_DEBUG,
																"[DM]: Zigbee framework commissioning switch on off value int:[%d]",
																ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal);

														/*syslog(LOG_DEBUG,
																"[DM]: Zigbee framework commissioning switch on off value string:[%s]",
																ZBPropertyObj->getproperty_value().c_str());*/

													} else if (ZbMappingDatabase::DMPropertyNameZB(
															ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
															ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo],
															ZERO_ZONE_ID,
															ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId) == DOOR_STATE) {

														if (ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal
																== 1) {
															ZBPropertyObj->setproperty_value(
																	std::to_string(
																			0));
														} else {
															ZBPropertyObj->setproperty_value(
																	std::to_string(
																			1));
														}
													}
												}
													/*syslog(
													LOG_INFO,
															"[DM]: Zigbee framework commissioning switch end");*/
													break;
												case RANGE: {
													/*syslog(
													LOG_INFO,
															"[DM]: Zigbee framework commissioning range start");*/

													ZBPropertyObj->setproperty_type(
													DM_RANGE_DM);

													RequestResponseOBJ.Property_Name =
															ZbMappingDatabase::DMPropertyNameZB(
																	ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																	ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo],
																	ZERO_ZONE_ID,
																	ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId);

													if (RequestResponseOBJ.Property_Name
															== BRIGHTNESS) {

														ZBThingObj->setthing_name(
																"Dimmable Light");
														ZBPropertyObj->setproperty_value(
																std::to_string(
																		((ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[0].attributeVal)
																				* 100)
																				/ 255));
													} else if (RequestResponseOBJ.Property_Name
															== FANSPEED) {
														ZBPropertyObj->setproperty_value(
																std::to_string(
																		((ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[0].attributeVal)
																				* 100)
																				/ 255));
													} else if (RequestResponseOBJ.Property_Name
															== COLOR_TEMPERATURE) {
														ZBPropertyObj->setproperty_value(
																std::to_string(
																		((ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal)
																				* 100)
																				/ 255));
													}
												}
													/*syslog(
													LOG_INFO,
															"[DM]: Zigbee framework commissioning range end");*/
													break;

												case COLOR: {
													/*syslog(
													LOG_INFO,
															"[DM]: Zigbee framework commissioning color picker start");*/

													if (ZBThingObj->getthing_type()
															== "Light") {
														ZBThingObj->setthing_name(
																"Color Dimmable Light");
													}
													ZBPropertyObj->setproperty_type(
													DM_COLOR_DM);
													float x_value, y_value;

													if (ZbMappingDatabase::DMPropertyNameZB(
															ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
															ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo],
															ZERO_ZONE_ID,
															ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId) == COLORS) {
														if (AttributeNo == 0) {
															x_value =
																	(ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal);
														} else if (AttributeNo
																== 1) {
															y_value =
																	(ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].attributes[AttributeNo].attributeVal);
														}
													}

													RGBVALUE RGBVALUEObj;
													RGBVALUEObj =
															convertXYtoRGB(
																	x_value,
																	y_value);
													ZBPropertyObj->setproperty_value(
															(convertRGBtoHEXA(
																	RGBVALUEObj.Red,
																	RGBVALUEObj.Green,
																	RGBVALUEObj.Blue)).c_str());
												}
													/*syslog(
													LOG_INFO,
															"[DM]: Zigbee framework commissioning colorpicker end");*/
													break;
												case LIST: {
													/*syslog(
													LOG_INFO,
															"[DM]: Zigbee framework commissioning list start");*/
													ZBPropertyObj->setproperty_type(
													DM_LIST_DM);
													ZBPropertyObj->setproperty_name(
															ZbMappingDatabase::DMPropertyNameZB(
																	ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																	ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo],
																	ZERO_ZONE_ID,
																	ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId));
													ZBPropertyObj->setproperty_range(
															ZbMappingDatabase::DMPropertyRangeZB(
																	ZB_MsgQue_ResObj.Endpoint[EndPointNo].deviceId,
																	ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].cmdId[CommandIDNo],
																	ZERO_ZONE_ID,
																	ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId));
													ZBPropertyObj->setproperty_value(
															"STOP");
													/*syslog(
													LOG_INFO,
															"[DM]: Zigbee framework commissioning list end");*/
												}
													break;

												}
											}
												//ZBEntityObj->setpropertyInfo(ZBPropertyObj);
												//To exclude cluster 25 ( FOTA )
												if (ZB_MsgQue_ResObj.Endpoint[EndPointNo].cluster[ClusterNo].clusterId != 25) {
													ZBEntityObj->setpropertyInfo(
													ZBPropertyObj);
												}

										}

									}
									/*syslog(LOG_INFO,
											"[DM]: Zigbee framework property started creating end ");*/
								}

									break;
								}

							}
							 if (ZBEntityObj->getPropertySize() != 0) {
                                                  			      ZBThingObj->setentityInfo(ZBEntityObj);
                                              				  }
						}
					}

					syslog(LOG_DEBUG,
							"[DM]: Zigbee framework discovery response creating thing in map: [%s]",
							(RequestResponseOBJ.Thing_Id).c_str());

					DmThingMap *ThingMapInstance = DmThingMap::GetInstance();

					ThingMapInstance->AddToThingMap(ZBThingObj->getthing_id(),
							ZBThingObj);

					RequestResponseOBJ.Client = std::stol(
							std::to_string(ZB_MsgQue_ResObj.MsgId).substr(0,
									2));
					RequestResponseOBJ.Command = DM_DISCOVERY;
					RequestResponseOBJ.Status_Code = DM_SUCCESS;
					RequestResponseOBJ.Message = "Discovery completed successfully";
					RequestResponseOBJ.RuleSceneData =
							ZBThingObj->getthing_name();
					RequestResponseOBJ.Message_Id = std::to_string(
							ZB_MsgQue_ResObj.MsgId);
					RequestResponseOBJ.Thing_Id = ZBThingObj->getthing_id();

					DmPlatform::DmSendingResponse(RequestResponseOBJ);
				}
			}

		} else {

			RequestResponse RequestResponseOBJ;

			syslog(LOG_DEBUG,
					"[DM]: Zigbee framework  discovery error code: %d",
					ZB_MsgQue_ResObj.ErrorCode);

			RequestResponseOBJ.Command = DM_DISCOVERY;

			RequestResponseOBJ.Message_Id = std::to_string(
					ZB_MsgQue_ResObj.MsgId);

			RequestResponseOBJ.Client = std::stol(
					RequestResponseOBJ.Message_Id.substr(0, 2));

			RequestResponseOBJ.Message = ZbMappingDatabase::DMErrorTypeZB(
					ZB_MsgQue_ResObj.ErrorCode);

			RequestResponseOBJ.Status_Code = ZbMappingDatabase::DMErrorCodeZB(
					ZB_MsgQue_ResObj.ErrorCode);

			bool ThingRemoveStatus = DmFrameworkHandle::StopReqHandleThread(
					std::string("ZB") + RequestResponseOBJ.Message_Id);

			if (ThingRemoveStatus == true) {

				/*syslog(LOG_DEBUG,
						"[DM]: Zigbee Framework receive error response message_id found in request map: [%s]",
						(std::string("ZB") + RequestResponseOBJ.Message_Id).c_str());*/

				DMReqStructZB DMReqStructZBObjRemove;
				DMReqStructZBObjRemove.Mode = MODE_ID_REMOVE;
				DMReqStructZBObjRemove.IEEE = ZB_MsgQue_ResObj.IEEE;
				DMReqStructZBObjRemove.nodeId = ZB_MsgQue_ResObj.node_id;
				DMReqStructZBObjRemove.MsgId = 0;

				bool ControlSend = ZbMessageQueue::ZigbeeMsgQueueReq(
						DMReqStructZBObjRemove);
				syslog(LOG_INFO,
						"[DM]: Zigbee framework discovery request status remove: [%d]",
						ControlSend);
				if (RequestResponseOBJ.Command != DM_DISCOVERY) {
					DmPlatform::DmSendingResponse(RequestResponseOBJ);
				}
			}
		}
	}
}

/* Function name : ZBNonDiscoveryResponse
 * Description   : Receive all non-discovery response from Zigbee
 */
void ZBNonDiscoveryResponse() {
	/*syslog(LOG_INFO,
			"[DM]: Zigbee framework start mq receive zb non discovery main thread ");

	syslog(LOG_INFO, "[DM]: Zigbee framework in ZB non discovery response");*/

	ZbFramework::DMMessageQueueInitZB();

	while (true) {

		DMReqStructZB DMReqStructZBObj;
		memset(&DMReqStructZBObj, 0, sizeof(DMReqStructZBObj));
		DMReqStructZBObj.mtype = 1;

		/*syslog(LOG_DEBUG,
				"[DM]: Zigbee message queue nondiscovery response size: [%ld]",
				sizeof(DMReqStructZBObj));*/

		DMReqStructZBObj = ZbMessageQueue::ZigbeeNonDiscoveryRcvResp(
				DMReqStructZBObj);

		RequestResponse RequestResponseOBJ;

		RequestResponseOBJ.Message_Id = std::to_string(DMReqStructZBObj.MsgId);

		DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

		if (DMReqStructZBObj.ErrorCode == DM_NULL) {

			syslog(LOG_DEBUG,
							"[DM]: Zigbee framework control response for MODE: [%d]",
							DMReqStructZBObj.Mode);
			switch (DMReqStructZBObj.Mode) {

			case MODE_ID_CONTROL: {

				/*syslog(LOG_INFO,
						"[DM]: Zigbee framework control response start");*/

				RequestResponseOBJ.Property_Name =
						DmFrameworkHandle::PropertyNameStopReqHandleThread(
								std::string("ZB")
										+ std::to_string(
												DMReqStructZBObj.MsgId));

				syslog(LOG_DEBUG,
						"[DM]: Zigbee framework control response for property_name: [%s]",
						RequestResponseOBJ.Property_Name.c_str());

				if (RequestResponseOBJ.Property_Name
						!= std::string("DM_DEFAULT")) {

					DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

					RequestResponseOBJ.Thing_Id = std::string("ZB")
							+ std::to_string(DMReqStructZBObj.IEEE);

					/*started the control request*/

					std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
							DmThingMapObj->DmThingDataCtrlObjMap.find(
								std::to_string(DMReqStructZBObj.MsgId));
					if (DmThingMapObjItr
							!= DmThingMapObj->DmThingDataCtrlObjMap.end()) { 

						syslog(LOG_DEBUG,
								"[DM]: Zigbee framework control response thing id found in temp map: [%s]",
								(RequestResponseOBJ.Thing_Id).c_str());

						for (int EndPointNo = 0;
								EndPointNo
										< DmThingMapObjItr->second->getEntityListSize();
								EndPointNo++) {

							RequestResponseOBJ.Entity_Id = std::to_string(
									DMReqStructZBObj.endpoint);

							DmThingMapObjItr->second->setstatus_code(
									std::to_string(DM_SUCCESS));

							/*syslog(LOG_DEBUG,
									"[DM]: Zigbee framework control response entity_id found in in main map: [%s] response: [%s]",
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
												"[DM]: Zigbee framework control response printing main thing map size: [%ld]",
												DmThingMapObj->DmThingDataMap.size());

										syslog(LOG_DEBUG,
												"[DM]: Zigbee framework control response printing main control map size: [%ld]",
												DmThingMapObj->DmThingDataCtrlObjMap.size());

										syslog(LOG_DEBUG,
												"[DM]: Zigbee framework control response checking for message_id: [%s]",
												RequestResponseOBJ.Message_Id.c_str());*/

										DmThingMapObj->DmThingDataMap.erase(
												DmThingMapObjItr->first);

										/*syslog(LOG_DEBUG,
												"[DM]: Zigbee framework control response removing main thing map size: [%ld]",
												DmThingMapObj->DmThingDataMap.size());*/

										DmThingMapObj->DmThingDataMap[RequestResponseOBJ.Thing_Id] =
												DmThingMapObj->GetDmThingMapCtrlObj(
														RequestResponseOBJ.Message_Id);

										/*syslog(LOG_DEBUG,
												"[DM]: Zigbee framework control response updated main map size: [%ld]",
												DmThingMapObj->DmThingDataMap.size());*/

										DmThingMapObj->RemoveDmThingMapCtrlObj(
												RequestResponseOBJ.Message_Id);

//										DmThingMapObj->DmThingDataCtrlObjMap.erase(
//												RequestResponseOBJ.Message_Id);

										/*syslog(LOG_DEBUG,
												"[DM]: Zigbee framework control response removing main control map size: [%ld]",
												DmThingMapObj->DmThingDataCtrlObjMap.size());*/

										syslog(LOG_DEBUG,
												"[DM]: Zigbee framework control response updated main map: [%s]",
												RequestResponseOBJ.Thing_Id.c_str());
										break;
									}
								}
								break;
							}
						}

						RequestResponseOBJ.Status_Code = DM_SUCCESS;
						RequestResponseOBJ.Client = std::stol(
								std::to_string(DMReqStructZBObj.MsgId).substr(0,
										2));
						RequestResponseOBJ.Command = DM_CONTROL;
						RequestResponseOBJ.Message = "Control completed successfully";
						RequestResponseOBJ.Message_Id = std::to_string(
								DMReqStructZBObj.MsgId);

						if (!RequestResponseOBJ.Thing_Id.empty()
								&& !RequestResponseOBJ.Entity_Id.empty()
								&& !RequestResponseOBJ.Property_Name.empty()
								&& !RequestResponseOBJ.Property_Type.empty()
								&& !RequestResponseOBJ.Property_Range.empty()
								&& !RequestResponseOBJ.Property_Value.empty()) {
							DmPlatform::DmSendingResponse(RequestResponseOBJ);
						} else {
							syslog(LOG_ERR,
									"[DM]: Zigbee Framework control response stopped processing empty data to TronX");
						}

					} else {
						syslog(LOG_INFO,
								"[DM]: Zigbee framework control response thing_id not found in temp map");
					}
				} else {
					syslog(LOG_INFO,
							"[DM]: Zigbee framework control response message_id not found in request map");
				}
			}
				//syslog(LOG_INFO, "[DM]: Zigbee framework control response end");

			pthread_cond_signal(&DM_Req_Control);

				break;

			case MODE_ID_OBSERVE: {
//----------------------------------- STARTED THE OBSERVE REQUEST ------------------------------------
				/*syslog(LOG_INFO,
						"[DM]: Zigbee framework OBSERVE RESPONSE START ");*/

				DMReqStructZBObj.MsgId = DM_OBSERVE_MSG_ID;
				DMReqStructZBObj.argcount = DMReqStructZBObj.argcount - 1;
//------------------------------------------------------------------------------------------
				RequestResponseOBJ.Thing_Id = std::string("ZB")
						+ std::to_string(DMReqStructZBObj.IEEE);

				std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
						DmThingMapObj->DmThingDataMap.find(
								RequestResponseOBJ.Thing_Id);
				if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {

					/*syslog(LOG_DEBUG,
							"[DM]: Zigbee framework observe response found for  thing_id: [%s] not found in main map",
							RequestResponseOBJ.Thing_Id.c_str());*/

						RequestResponseOBJ.Entity_Id = std::to_string(
								DMReqStructZBObj.endpoint);

						for (int EndPointNo = 0;
								EndPointNo
										< DmThingMapObjItr->second->getEntityListSize();
								EndPointNo++) {

	RequestResponseOBJ.Thing_Type=DmThingMapObjItr->second->getthing_type();


/*syslog(LOG_DEBUG,
                                                        "[DM]: Zigbee framework observe response  verifying**: [%s] ",
                                                        RequestResponseOBJ.Thing_Type.c_str());


syslog(LOG_DEBUG,
							"[DM]: Zigbee framework observe response found for thingtype**: [%s] ",
							DmThingMapObjItr->second->getthing_type().c_str());*/

					if (DmThingMapObjItr->second->getthing_type() == "Sensor") {

/*syslog(LOG_DEBUG,
										"[DM]: DMReqStructZBObj.clusterId %d\n",
										DMReqStructZBObj.clusterId);*/

								RequestResponseOBJ.Entity_Id = std::to_string(
										DMReqStructZBObj.endpoint) + "_"
										+ std::to_string(
												DMReqStructZBObj.clusterId);


							if (DmThingMapObjItr->second->getEntityInfo(
									EndPointNo)->getentity_id()
									== RequestResponseOBJ.Entity_Id) {

								if (DMReqStructZBObj.clusterId == IAS_ZONE) {

									RequestResponseOBJ.Property_Name =
											ZbMappingDatabase::DMPropertyNameZB(
													DMReqStructZBObj.deviceId,
													DMReqStructZBObj.commandId,
													std::stoi(
															DmThingMapObjItr->second->getzone_type()),
													DMReqStructZBObj.clusterId).c_str();

if(RequestResponseOBJ.Property_Name == CONTACT_DETECT)
									{
											syslog(LOG_DEBUG,
													"[DM]: Zigbee MSG queue IPC response IAS ZONE status: ZBConatct: %d",
													DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue
															 & CONATCT_OR_ALARM_POSITION);
											if ((DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue
													 & CONATCT_OR_ALARM_POSITION) == 0) {
												RequestResponseOBJ.Property_Value =
												DM_VALUE_1;    //CLOSED
											} else {
												RequestResponseOBJ.Property_Value =
												DM_VALUE_0;  //OPENED
											}
									}
									else {
											syslog(LOG_DEBUG,
													"[DM]: Zigbee MSG queue IPC response IAS ZONE status: ZBALARM TYPE : %d",
													DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue
															 & CONATCT_OR_ALARM_POSITION);
											if ((DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue
													 & CONATCT_OR_ALARM_POSITION) == 0) {
												RequestResponseOBJ.Property_Value =
														DM_VALUE_0;   //ALARM NOT DETECTED
											} else {
												RequestResponseOBJ.Property_Value =
														DM_VALUE_1;   //ALARM DETECTED
											}


										}

									/*for (char i = 0; i < 1; i++) {

										switch (i) {

										case ZBALARM: {
											syslog(LOG_DEBUG,
													"[DM]: Zigbee MSG queue IPC response IAS ZONE status: ZBALARM: %d",
													DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue
															>> i & 1);
											RequestResponseOBJ.Property_Name
													== ALARM;

											if ((DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue
													>> i & 1) == 0) {
												RequestResponseOBJ.Property_Value =
												DM_VALUE_1;
											} else {
												RequestResponseOBJ.Property_Value =
												DM_VALUE_0;
											}

										}
											break;

										case ZBTAMPER: {
											syslog(LOG_DEBUG,
													"[DM]: Zigbee MSG queue IPC response IAS ZONE status: ZBTAMPER: %d",
													DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue
															>> i & 1);
											RequestResponseOBJ.Property_Name
													== TAMPER;
											if ((DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue
													>> i & 1) == 0) {
												RequestResponseOBJ.Property_Value =
												DM_VALUE_1;
											} else {
												RequestResponseOBJ.Property_Value =
												DM_VALUE_0;
											}
										}
											break;

										case ZBBATTERY: {
											syslog(LOG_DEBUG,
													"[DM]: Zigbee MSG queue IPC response IAS ZONE status: ZBBATTERY: %d",
													DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue
															>> i & 1);
											RequestResponseOBJ.Property_Name
													== BATTERY;
											if ((DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue
													>> i & 1) == 0) {
												RequestResponseOBJ.Property_Value =
												DM_VALUE_1;
											} else {
												RequestResponseOBJ.Property_Value =
												DM_VALUE_0;
											}
										}
											break;

										case ZBBATTERYDEFECT: {
											syslog(LOG_DEBUG,
													"[DM]: Zigbee MSG queue IPC response IAS ZONE status: ZB battery defect: %d",
													DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue
															>> i & 1);
											RequestResponseOBJ.Property_Name
													== BATTERY_DEFECT;
											if ((DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue
													>> i & 1) == 0) {
												RequestResponseOBJ.Property_Value =
												DM_VALUE_1;
											} else {
												RequestResponseOBJ.Property_Value =
												DM_VALUE_0;
											}
										}
											break;
										default: {
											syslog(LOG_INFO,
													"[DM]: Zigbee framework observe started contact_detect default case ");
										}
											break;
										}
									}*/

									RequestResponseOBJ.Property_Type =
									DM_INFO_DM;

									//SMOKE_DETECT OCCUPANCY TEMPERATURE ON_OFF
									/*if (RequestResponseOBJ.Property_Name
											== ALARM) {
										RequestResponseOBJ.Property_Name
												== CONTACT_DETECT;
									}*/

									for (int ClusterNoIAS = 0;
											ClusterNoIAS
													< DmThingMapObjItr->second->getEntityInfo(
															EndPointNo)->getPropertySize();
											ClusterNoIAS++) {

										/*syslog(LOG_DEBUG,
												"[DM]: Zigbee framework observe started for [%s] searching for [%s]",
												DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertyInfo(
														ClusterNoIAS)->getproperty_name().c_str(),
												RequestResponseOBJ.Property_Name.c_str());*/

										if (DmThingMapObjItr->second->getEntityInfo(
												EndPointNo)->getPropertyInfo(
												ClusterNoIAS)->getproperty_name()
												== RequestResponseOBJ.Property_Name) {

											DmThingMapObjItr->second->getEntityInfo(
													EndPointNo)->getPropertyInfo(
													ClusterNoIAS)->setproperty_value(
													RequestResponseOBJ.Property_Value);

											syslog(LOG_DEBUG,
													"[DM]: Zigbee framework observe started for [%s] ",
													RequestResponseOBJ.Property_Name.c_str());
											break;
										}
									}

								}
								else {



										for (int ClusterNo = 0;
										ClusterNo
												< DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertySize();
										ClusterNo++) {

											RequestResponseOBJ.Property_Type =
											DM_INFO_DM;

											/*syslog(LOG_INFO,
													"[DM]: Zigbee framework observe response info start ");*/
											
											RequestResponseOBJ.Property_Name =
											ZbMappingDatabase::DMPropertyNameZB(
													DMReqStructZBObj.deviceId,
													DMReqStructZBObj.commandId,
													std::stoi(
															DmThingMapObjItr->second->getzone_type()),
													DMReqStructZBObj.clusterId).c_str();


											if (RequestResponseOBJ.Property_Name
													== TEMPERATURE) {

												DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertyInfo(
														ClusterNo)->setproperty_value(
														std::to_string(
																(DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue)
																		/ 100));

												RequestResponseOBJ.Property_Value =
														std::to_string(
																(DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue)
																		/ 100);

											} else if (RequestResponseOBJ.Property_Name
													== HUMIDITY) {
												DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertyInfo(
														ClusterNo)->setproperty_value(
														std::to_string(
																(DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue)
																		/ 100));
												RequestResponseOBJ.Property_Value =
														std::to_string(
																(DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue)
																		/ 100);

											} else if (RequestResponseOBJ.Property_Name
													== GAS_DETECT) {
												DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertyInfo(
														ClusterNo)->setproperty_value(
														std::to_string(
																(DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue)
																		/ 100));
												RequestResponseOBJ.Property_Value =
														std::to_string(
																(DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue)
																		/ 100);

											} else if (RequestResponseOBJ.Property_Name
													== SMOKE_DETECT) {
												DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertyInfo(
														ClusterNo)->setproperty_value(
														std::to_string(
																(DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue)
																		/ 100));
												RequestResponseOBJ.Property_Value =
														std::to_string(
																(DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue)
																		/ 100);

											}

										}
											/*syslog(LOG_INFO,
													"[DM]: Zigbee framework observe response info_end ");*/

}
							}


					} else {

						/*syslog(LOG_DEBUG,
								"[DM]: Zigbee framework observe came for endpoint: [%d]",
								DMReqStructZBObj.endpoint);

						syslog(LOG_DEBUG,
									"[DM]: Zigbee framework observe in map for endpoint: [%d]",
									DmThingMapObjItr->second->getEntityInfo(
											EndPointNo)->getentity_id().c_str());*/

							if (DmThingMapObjItr->second->getEntityInfo(
									EndPointNo)->getentity_id()
									== std::to_string(
											DMReqStructZBObj.endpoint)) {

								syslog(LOG_DEBUG,
										"[DM]: Zigbee framework observe in found for endpoint: [%s]",
										DmThingMapObjItr->second->getEntityInfo(
												EndPointNo)->getentity_id().c_str());

								
								for (int ClusterNo = 0;
										ClusterNo
												< DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertySize();
										ClusterNo++) {

									if (DMReqStructZBObj.clusterId
											== ONOFF_CLUSTER
											|| DMReqStructZBObj.clusterId
													== RANGE_CLUSTER) {

										if (DMReqStructZBObj.clusterId
												== ONOFF_CLUSTER) {

											RequestResponseOBJ.Property_Name =
													ZbMappingDatabase::DMPropertyNameZB(
															DMReqStructZBObj.deviceId,
															std::stoi(
																	DmThingMapObjItr->second->getzone_type()),
															std::stoi(
																	DmThingMapObjItr->second->getzone_type()),
															DMReqStructZBObj.clusterId);
										} else {
											RequestResponseOBJ.Property_Name =
													ZbMappingDatabase::DMPropertyNameZB(
															DMReqStructZBObj.deviceId,
															RANGE_COMMAND,
															std::stoi(
																	DmThingMapObjItr->second->getzone_type()),
															DMReqStructZBObj.clusterId);
										}
									} else {
										RequestResponseOBJ.Property_Name =
												ZbMappingDatabase::DMPropertyNameZB(
														DMReqStructZBObj.deviceId,
														DMReqStructZBObj.commandId,
														std::stoi(
																DmThingMapObjItr->second->getzone_type()),
														DMReqStructZBObj.clusterId);
									}
									if (DmThingMapObjItr->second->getEntityInfo(
											EndPointNo)->getPropertyInfo(
											ClusterNo)->getproperty_name()
											== RequestResponseOBJ.Property_Name) {

										int PRO_type = 0;

										if (DMReqStructZBObj.clusterId
												== ONOFF_CLUSTER
												|| DMReqStructZBObj.clusterId
														== RANGE_CLUSTER) {

											if (DMReqStructZBObj.clusterId
													== ONOFF_CLUSTER) {
												PRO_type =
														ZbMappingDatabase::DMPropertyTypeZB(
																DMReqStructZBObj.deviceId,
																ZERO_ZONE_ID,
																std::stoi(
																		DmThingMapObjItr->second->getzone_type()),
																DMReqStructZBObj.clusterId);
											} else {
												PRO_type =
														ZbMappingDatabase::DMPropertyTypeZB(
																DMReqStructZBObj.deviceId,
																RANGE_COMMAND,
																std::stoi(
																		DmThingMapObjItr->second->getzone_type()),
																DMReqStructZBObj.clusterId);
											}

										} else {
											PRO_type =
													ZbMappingDatabase::DMPropertyTypeZB(
															DMReqStructZBObj.deviceId,
															DMReqStructZBObj.commandId,
															std::stoi(
																	DmThingMapObjItr->second->getzone_type()),
															DMReqStructZBObj.clusterId);
										}

										syslog(LOG_INFO,
														"[DM]: Zigbee framework observe property type : [ %d ] Prop Nmae: %s ",PRO_type,RequestResponseOBJ.Property_Name.c_str());

										switch (PRO_type) {

										

										case RANGE: {
											RequestResponseOBJ.Property_Type =
											DM_RANGE_DM;
											syslog(LOG_INFO,
													"[DM]: Zigbee framework observe response range_start ");

											if (RequestResponseOBJ.Property_Name
													== BRIGHTNESS) {

												syslog(LOG_INFO,
														"[DM]: Zigbee framework observe response range_brightness start ");

												DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertyInfo(
														ClusterNo)->setproperty_value(
														std::to_string(
																((DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue)
																		* 100)
																		/ 255));
												RequestResponseOBJ.Property_Value =
														std::to_string(
																((DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue)
																		* 100)
																		/ 255);

											} else if (RequestResponseOBJ.Property_Name
													== FANSPEED) {

												syslog(LOG_INFO,
														"[DM]: Zigbee framework observe response range_dimmer start ");

												DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertyInfo(
														ClusterNo)->setproperty_value(
														std::to_string(
																((DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue)
																		* 100)
																		/ 255));
												RequestResponseOBJ.Property_Value =
														std::to_string(
																((DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue)
																		* 100)
																		/ 255);
											} else if (RequestResponseOBJ.Property_Name
													== COVERING) {

												syslog(LOG_INFO,
														"[DM]: Zigbee framework observe response range_covering start ");

												DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertyInfo(
														ClusterNo)->setproperty_value(
														std::to_string(
																((DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue)
																		* 100)
																		/ 255));

												RequestResponseOBJ.Property_Value =
														std::to_string(
																((DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue)
																		* 100)
																		/ 255);
											}
										}
											syslog(LOG_INFO,
													"[DM]: Zigbee framework RANGE_END ");
											break;

										case SWITCH: {
											RequestResponseOBJ.Property_Type =
											DM_SWITCH_DM;
											syslog(LOG_INFO,
													"[DM]: Zigbee framework observe response switch_start ");

											if (RequestResponseOBJ.Property_Name
													== ON_OFF) {

												DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertyInfo(
														ClusterNo)->setproperty_value(
														std::to_string(
																DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue));

												RequestResponseOBJ.Property_Value =
														std::to_string(
																DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue);
											} else if (RequestResponseOBJ.Property_Name
													== STATE) {

												switch (DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue) {

												case 2: { 						// device_in state_open
													syslog(LOG_INFO,
															"[DM]: Zigbee framework observe response device_in state_open ");

													DmThingMapObjItr->second->getEntityInfo(
															EndPointNo)->getPropertyInfo(
															ClusterNo)->setproperty_value(
															std::to_string(1));
													RequestResponseOBJ.Property_Value =
															std::to_string(1);
												}
													break;
												case 0: {						// device_in state_close
													syslog(LOG_INFO,
															"[DM]: Zigbee framework observe response device_in state_close ");

													DmThingMapObjItr->second->getEntityInfo(
															EndPointNo)->getPropertyInfo(
															ClusterNo)->setproperty_value(
															std::to_string(0));
													RequestResponseOBJ.Property_Value =
															std::to_string(0);
												}
													break;
												default: {
													syslog(LOG_INFO,
															"[DM]: Zigbee framework observe response  DEFAULT case ");
												}
												}
											} else if (strcmp(
													RequestResponseOBJ.Property_Name.c_str(),
													DOOR_STATE) == 0) {

												switch (DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue) {

												case 1: {					// door in close state
													/*syslog(LOG_INFO,
															"[DM]: Zigbee framework observe response device_in state_close ");*/

													DmThingMapObjItr->second->getEntityInfo(
															EndPointNo)->getPropertyInfo(
															ClusterNo)->setproperty_value(
															std::to_string(1));
													RequestResponseOBJ.Property_Value =
															std::to_string(1);
												}
													break;
												case 2: {					// door in open state
													/*syslog(LOG_INFO,
															"[DM]: Zigbee framework observe response device_in state_open ");*/

													DmThingMapObjItr->second->getEntityInfo(
															EndPointNo)->getPropertyInfo(
															ClusterNo)->setproperty_value(
															std::to_string(0));
													RequestResponseOBJ.Property_Value =
															std::to_string(0);
												}
													break;
												default: {
													syslog(LOG_INFO,
															"[DM]: Zigbee framework observe response  invalid case ");
												}
												}
											}
										}
											/*syslog(LOG_INFO,
													"[DM]: Zigbee framework observe response  switch_end ");*/
											break;

										case COLOR: {

											RequestResponseOBJ.Property_Type =
											DM_COLOR_DM;
											/*syslog(LOG_INFO,
													"[DM]: Zigbee framework observe response  color_start ");
											syslog(LOG_INFO,
													"[DM]: Zigbee framework observe response  ZB device_in color change state ");*/
										}
											/*syslog(LOG_INFO,
													"[DM]: Zigbee framework observe response  color_end ");*/
											break;
										}
									}
								}
								break;
							} else {
								syslog(LOG_INFO,
										"[DM]: Zigbee framework observe response wrong end point ");
							}
						}


					}

					RequestResponseOBJ.Command = DM_OBSERVE;
					RequestResponseOBJ.Message = "Received observe response";
					RequestResponseOBJ.Status_Code = DM_SUCCESS;
					RequestResponseOBJ.Message_Id = std::to_string(
							DMReqStructZBObj.MsgId);
					RequestResponseOBJ.Client =
							std::stol(
									std::string(
											std::to_string(
													DMReqStructZBObj.MsgId)).substr(
											0, 2));

					if (!RequestResponseOBJ.Thing_Id.empty()
							&& !RequestResponseOBJ.Entity_Id.empty()
							&& !RequestResponseOBJ.Property_Name.empty()
							&& !RequestResponseOBJ.Property_Type.empty()
							&& !RequestResponseOBJ.Property_Value.empty()) {
						DmPlatform::DmSendingResponse(RequestResponseOBJ);
					} else {
						syslog(LOG_ERR,
								"[DM]: Zigbee Framework observe response stopped processing empty data to TronX");
					}

				} else {
					syslog(LOG_DEBUG,
							"[DM]: Zigbee framework observe response removing thing_id: [%s] not found in main map",
							RequestResponseOBJ.Thing_Id.c_str());
				}
			}

				//syslog(LOG_INFO, "[DM]: Zigbee framework observe response end");
				break;

			case MODE_ID_REMOVE: {

				/*syslog(LOG_INFO,
						"[DM]: Zigbee framework remove response start ");*/

				RequestResponseOBJ.Status_Code = DM_SUCCESS;
				RequestResponseOBJ.Message_Id = std::to_string(
						DMReqStructZBObj.MsgId);
				RequestResponseOBJ.Client =
						std::stol(
								std::string(
										std::to_string(DMReqStructZBObj.MsgId)).substr(
										0, 2));

				RequestResponseOBJ.Thing_Id = std::string("ZB")
						+ std::to_string(DMReqStructZBObj.IEEE);

				syslog(LOG_DEBUG,
						"[DM]: Zigbee framework remove response for thing_id: [%s]",
						RequestResponseOBJ.Thing_Id.c_str());

				bool ThingRemoveStatus = DmFrameworkHandle::StopReqHandleThread(
						std::string("ZB")
								+ std::to_string(DMReqStructZBObj.MsgId));

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
									"[DM]: Zigbee Framework remove response stopped processing empty data to TronX");
						}

					}
				}
			}
				//syslog(LOG_INFO, "[DM]: Zigbee framework remove response end");

				break;

			case MODE_ID_ZBFOTA: {					/*Response handle from Zigbee for end-device FOTA response*/

				/*syslog(LOG_INFO,
				 "[DM]: Zigbee framework fota update response start ");*/

				RequestResponseOBJ.Status_Code = DM_SUCCESS;
				RequestResponseOBJ.Message_Id = std::to_string(
						DMReqStructZBObj.MsgId);
				RequestResponseOBJ.Client =
						std::stol(
								std::string(
										std::to_string(DMReqStructZBObj.MsgId)).substr(
										0, 2));

				RequestResponseOBJ.Thing_Id = std::string("ZB")
						+ std::to_string(DMReqStructZBObj.IEEE);
				
				syslog(LOG_DEBUG,
						"[DM]: Zigbee framework fota response for thing_id: [%s]",
						RequestResponseOBJ.Thing_Id.c_str());

				bool ThingfotaStatus = DmFrameworkHandle::StopReqHandleThread(
						std::string("ZB")
								+ std::to_string(DMReqStructZBObj.MsgId));			/* Stopping the thread for that MSG_Id after receiving response */

				if (ThingfotaStatus == true) {

					std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
							DmThingMapObj->DmThingDataMap.find(
									RequestResponseOBJ.Thing_Id);
					if (DmThingMapObjItr
							!= DmThingMapObj->DmThingDataMap.end()) {

						RequestResponseOBJ.Command = "FOTA_REQ";
						RequestResponseOBJ.Message = "DM_FOTA_UPDATE_COMPLETE";

						if (!RequestResponseOBJ.Thing_Id.empty()) {
							DmPlatform::DmSendingResponse(RequestResponseOBJ);
						} else {
							syslog(LOG_ERR,
									"[DM]: Zigbee Framework fota update response stopped processing empty data to TronX");
						}

					}
				}
			}
				//syslog(LOG_INFO, "[DM]: Zigbee framework fota update response end");

				break;

			case MODE_ID_LEFT: {

				/*syslog(LOG_INFO,
						"[DM]: Zigbee framework mode_id_left response start");*/
				/*started the left request*/

				RequestResponseOBJ.Thing_Id = std::string("ZB")
						+ std::to_string(DMReqStructZBObj.IEEE);

				std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
						DmThingMapObj->DmThingDataMap.find(
								RequestResponseOBJ.Thing_Id);
				if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {

					syslog(LOG_DEBUG,
							"[DM]: Zigbee framework MODE_ID_LEFT RESPONSE THINGID FOUND IN TEMPMAP: [%s]",
							RequestResponseOBJ.Thing_Id.c_str());

					DMReqStructZBObj.MsgId = DM_OBSERVE_MSG_ID;

					RequestResponseOBJ.Command = DM_REMOVE;
					RequestResponseOBJ.Message = "Device Left";
					RequestResponseOBJ.Status_Code = DM_SUCCESS;
					
				//To stop the FOTA-thread when device leaves network and send error response to FOTA
				if( FlagFota== true && RequestResponseOBJ.Thing_Id == FotaThingID)
					{
						bool ThingfotaStatus = DmFrameworkHandle::StopReqHandleThread(std::string ("ZB") + FotaMsgID);
						json_object * fota_response =  json_object_new_object();
						json_object_object_add(fota_response, "client",json_object_new_int(14));
						json_object_object_add(fota_response, "command",json_object_new_string("FOTA_RES"));
						json_object_object_add(fota_response, "thing_id",json_object_new_string(RequestResponseOBJ.Thing_Id.c_str()));

						if(strcmp(RequestResponseOBJ.Message.c_str(),"DM_FOTA_UPDATE_COMPLETE")==0)
							json_object_object_add(fota_response, "status",json_object_new_int(0));
						else
							json_object_object_add(fota_response, "status",json_object_new_int(504));
						json_object_object_add(fota_response, "version",json_object_new_string("0"));

						syslog(LOG_DEBUG, "[DM]: FOTA -> DM response object: [%s]",
								json_object_to_json_string(fota_response));
						int res_fota = sendto(sockfd, json_object_to_json_string(fota_response),
									strlen(json_object_to_json_string(fota_response)),
									MSG_CONFIRM, (const struct sockaddr *) &fota_servaddr,
									sizeof(fota_servaddr));
						syslog(LOG_DEBUG,
								"[DM]: FOTA -> DM response object status:[%d]",
								res_fota);
						system("rm -rf /exe_dir/ota-files/*");
						
						FlagFota=false; /* Reseting flag after response */
						json_object_put(fota_response);

					}
					
					RequestResponseOBJ.Message_Id = std::to_string(
							DMReqStructZBObj.MsgId);
					RequestResponseOBJ.Client =
							std::stol(
									std::string(
											std::to_string(
													DMReqStructZBObj.MsgId)).substr(
											0, 2));

					if (!RequestResponseOBJ.Thing_Id.empty()) {
						DmPlatform::DmSendingResponse(RequestResponseOBJ);
					} else {
						syslog(LOG_ERR,
								"[DM]: Zigbee Framework mode left response stopped processing empty data to TronX");
					}
				} else {
					syslog(LOG_DEBUG,
							"[DM]: Zigbee framework mode_id_left response thing id not found in temp map: [%s]",
							std::to_string(DMReqStructZBObj.IEEE).c_str());
				}
			}
				/*syslog(LOG_INFO,
						"[DM]: Zigbee framework mode_id_left response end");*/

				break;
			case MODE_ID_NODEID_CHANGE: {

				/*syslog(LOG_INFO,
						"[DM]: Zigbee framework mode_id_nodeid_change response start");

				syslog(LOG_DEBUG,
						"[DM]: Zigbee framework node id updating response node_id found in main map: [%d]",
						DMReqStructZBObj.nodeId);*/

				RequestResponseOBJ.Thing_Id = std::string("ZB")
						+ std::to_string(DMReqStructZBObj.IEEE);

				std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
						DmThingMapObj->DmThingDataMap.find(
								RequestResponseOBJ.Thing_Id);
				if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {

					/*syslog(LOG_DEBUG,
							"[DM]: Zigbee framework node id change response thing id found in main map: [%s]",
							std::to_string(DMReqStructZBObj.IEEE).c_str());*/

					syslog(LOG_DEBUG,
							"[DM]: Zigbee framework node id in map: [%s] updating response node_id found in main map: [%d]",
							DmThingMapObjItr->second->gethome_node_id().c_str(),
							DMReqStructZBObj.nodeId);

					if (DmThingMapObjItr->second->gethome_node_id()
							!= std::to_string(DMReqStructZBObj.nodeId)) {

						/*syslog(LOG_DEBUG,
								"[DM]: Zigbee framework node id in map: [%s] updating response node_id found in main map: [%s]",
								DmThingMapObjItr->second->gethome_node_id(),
								std::to_string(DMReqStructZBObj.nodeId));*/

						DmThingMapObjItr->second->sethome_node_id(
								std::to_string(DMReqStructZBObj.nodeId));

						/*syslog(LOG_DEBUG,
								"[DM]: Zigbee framework node id in map: [%s] updating response node_id found in main map: [%s]",
								DmThingMapObjItr->second->gethome_node_id(),
								std::to_string(DMReqStructZBObj.nodeId));*/

						DmPlatform::DMNodeIdUpdateInDB(
								RequestResponseOBJ.Thing_Id,
								DmThingMapObjItr->second->gethome_node_id());

					} else {
						syslog(LOG_DEBUG,
								"[DM]: Zigbee framework node id not changed: [%s]",
								std::to_string(DMReqStructZBObj.IEEE).c_str());
					}
				} else {
					syslog(LOG_DEBUG,
							"[DM]: Zigbee framework node id change response thing id not found in main map: [%s]",
							std::to_string(DMReqStructZBObj.IEEE).c_str());
				}
			}
				case MODE_ID_ZB_DEVICE_INFO:{

					//----------------------------------- STARTED THE OBSERVE REQUEST ------------------------------------
				/*syslog(LOG_INFO,
						"[DM]: Zigbee framework MODE_ID_ZB_DEVICE_INFO ( 11 ) START ");*/

				DMReqStructZBObj.MsgId = DM_OBSERVE_MSG_ID;
				DMReqStructZBObj.argcount = DMReqStructZBObj.argcount - 1;
//------------------------------------------------------------------------------------------
				RequestResponseOBJ.Thing_Id = std::string("ZB")
						+ std::to_string(DMReqStructZBObj.IEEE);

				std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
						DmThingMapObj->DmThingDataMap.find(
								RequestResponseOBJ.Thing_Id);
				if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {

					/*syslog(LOG_DEBUG,
							"[DM]: Zigbee framework observe response found for  thing_id: [%s] found in main map",
							RequestResponseOBJ.Thing_Id.c_str());*/
 {

						/*syslog(LOG_DEBUG,
								"[DM]: Zigbee framework observe came for endpoint: [%d]",
								DMReqStructZBObj.endpoint);*/

						for (int EndPointNo = 0;EndPointNo<DmThingMapObjItr->second->getEntityListSize();EndPointNo++) {

							syslog(LOG_DEBUG,"[DM]: Zigbee framework observe in map for endpoint: [%s]",
									DmThingMapObjItr->second->getEntityInfo(
											EndPointNo)->getentity_id().c_str());

						RequestResponseOBJ.Thing_Type=DmThingMapObjItr->second->getthing_type();

							if (DmThingMapObjItr->second->getEntityInfo(
									EndPointNo)->getentity_id()
									== std::to_string(
											DMReqStructZBObj.endpoint)) {

								/*syslog(LOG_DEBUG,
										"[DM]: Zigbee framework observe in found for endpoint: [%d]",
										DmThingMapObjItr->second->getEntityInfo(
												EndPointNo)->getentity_id().c_str());*/

								RequestResponseOBJ.Entity_Id = std::to_string(
										DMReqStructZBObj.endpoint);

								for (int ClusterNo = 0;
										ClusterNo< DmThingMapObjItr->second->getEntityInfo(EndPointNo)->getPropertySize();ClusterNo++) {
									// property name in this if
								if (DMReqStructZBObj.clusterId== ONOFF_CLUSTER|| DMReqStructZBObj.clusterId== RANGE_CLUSTER|| DMReqStructZBObj.clusterId==COLOR_CLUSTER) {

										if (DMReqStructZBObj.clusterId
												== ONOFF_CLUSTER) {

											RequestResponseOBJ.Property_Name =
													ZbMappingDatabase::DMPropertyNameZB(
															DMReqStructZBObj.deviceId,
															std::stoi(
																	DmThingMapObjItr->second->getzone_type()),
															std::stoi(
																	DmThingMapObjItr->second->getzone_type()),
															DMReqStructZBObj.clusterId);
										} else if (DMReqStructZBObj.clusterId
												== RANGE_CLUSTER) {
											RequestResponseOBJ.Property_Name =
													ZbMappingDatabase::DMPropertyNameZB(
															DMReqStructZBObj.deviceId,
															RANGE_COMMAND,
															std::stoi(
																	DmThingMapObjItr->second->getzone_type()),
															DMReqStructZBObj.clusterId);
										}else {
											RequestResponseOBJ.Property_Name =
													ZbMappingDatabase::DMPropertyNameZB(
															DMReqStructZBObj.deviceId,
															COLOR_COMMAND,
															std::stoi(
																	DmThingMapObjItr->second->getzone_type()),
															DMReqStructZBObj.clusterId);							
										}
									} else {
										RequestResponseOBJ.Property_Name =
												ZbMappingDatabase::DMPropertyNameZB(
														DMReqStructZBObj.deviceId,
														DMReqStructZBObj.commandId,
														std::stoi(
																DmThingMapObjItr->second->getzone_type()),
														DMReqStructZBObj.clusterId);
									}
									syslog(LOG_INFO,"[DM]: RequestResponseOBJ.Property_Name :%s ",RequestResponseOBJ.Property_Name.c_str());
									if (DmThingMapObjItr->second->getEntityInfo(
											EndPointNo)->getPropertyInfo(
											ClusterNo)->getproperty_name()
											== RequestResponseOBJ.Property_Name) {

										int PRO_type = 0;

										if (DMReqStructZBObj.clusterId
														== ONOFF_CLUSTER
												|| DMReqStructZBObj.clusterId
														== RANGE_CLUSTER 
												|| DMReqStructZBObj.clusterId
														== COLOR_CLUSTER) {

											if (DMReqStructZBObj.clusterId
													== ONOFF_CLUSTER) {
												PRO_type =
														ZbMappingDatabase::DMPropertyTypeZB(
																DMReqStructZBObj.deviceId,
																ZERO_ZONE_ID,
																std::stoi(
																		DmThingMapObjItr->second->getzone_type()),
																DMReqStructZBObj.clusterId);
											} else if (DMReqStructZBObj.clusterId
													== RANGE_CLUSTER){
												PRO_type =
														ZbMappingDatabase::DMPropertyTypeZB(
																DMReqStructZBObj.deviceId,
																RANGE_COMMAND,
																std::stoi(
																		DmThingMapObjItr->second->getzone_type()),
																DMReqStructZBObj.clusterId);
											}else if (DMReqStructZBObj.clusterId
													== COLOR_CLUSTER &&(DMReqStructZBObj.deviceId!=DIMMABLE_BULB_DEVICE_ID)){
												PRO_type =
														ZbMappingDatabase::DMPropertyTypeZB(
																DMReqStructZBObj.deviceId,
																COLOR_COMMAND,
																std::stoi(
																		DmThingMapObjItr->second->getzone_type()),
																DMReqStructZBObj.clusterId);
											}

										} else {
											PRO_type =
													ZbMappingDatabase::DMPropertyTypeZB(
															DMReqStructZBObj.deviceId,
															DMReqStructZBObj.commandId,
															std::stoi(
																	DmThingMapObjItr->second->getzone_type()),
															DMReqStructZBObj.clusterId);
										}


										syslog(LOG_INFO,"[DM]: RequestResponseOBJ.Property_Type :%s ",RequestResponseOBJ.Property_Type.c_str());

								//	if(DMReqStructZBObj.deviceId!=DIMMABLE_BULB_DEVICE_ID){		
									
										switch (PRO_type) {

										case RANGE: {
											RequestResponseOBJ.Property_Type =
											DM_RANGE_DM;
											syslog(LOG_INFO,
													"[DM]: RequestResponseOBJ.Property_Type :%s ",RequestResponseOBJ.Property_Type.c_str());

											if (RequestResponseOBJ.Property_Name
													== BRIGHTNESS) {

												//syslog(LOG_INFO,
												//		"[DM]: Zigbee framework observe response range_brightness start ");

												DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertyInfo(
														ClusterNo)->setproperty_value(
														std::to_string(
																((DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue)
																		* 100)
																		/ 255));
												RequestResponseOBJ.Property_Value =
														std::to_string(
																((DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue)
																		* 100)
																		/ 255);

											}
										}
											//syslog(LOG_INFO,
											//		"[DM]: Zigbee framework RANGE_END ");
											break;

										case SWITCH: {
											RequestResponseOBJ.Property_Type =
											DM_SWITCH_DM;
											syslog(LOG_INFO,
													"[DM]: RequestResponseOBJ.Property_Type :%s ",RequestResponseOBJ.Property_Type.c_str());

											if (RequestResponseOBJ.Property_Name
													== ON_OFF) {

												DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertyInfo(
														ClusterNo)->setproperty_value(
														std::to_string(
																DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue));

												RequestResponseOBJ.Property_Value =
														std::to_string(
																DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue);
											} 
										}
											//syslog(LOG_INFO,
												//	"[DM]: Zigbee framework observe response  switch_end ");
											break;

										case COLOR: {
																							
											RequestResponseOBJ.Property_Type =DM_COLOR_DM;
											syslog(LOG_INFO,
													"[DM]: RequestResponseOBJ.Property_Type :%s ",RequestResponseOBJ.Property_Type.c_str());


											if (RequestResponseOBJ.Property_Name== "Color") {
												float x_value, y_value; //DMReqStructZBObj
												x_value=(float)DMReqStructZBObj.argValue_info[0].argValue.intargValue;
												y_value=(float)DMReqStructZBObj.argValue_info[1].argValue.intargValue;
												RGBVALUE RGBVALUEObj;
												RGBVALUEObj = convertXYtoRGB(
														x_value, y_value);
												DmThingMapObjItr->second->getEntityInfo(
														EndPointNo)->getPropertyInfo(
														ClusterNo)->setproperty_value(
														(convertRGBtoHEXA(
																RGBVALUEObj.Red,
																RGBVALUEObj.Green,
																RGBVALUEObj.Blue)).c_str());




												RequestResponseOBJ.Property_Value =
														((convertRGBtoHEXA(
																RGBVALUEObj.Red,
																RGBVALUEObj.Green,
																RGBVALUEObj.Blue)).c_str());
											} 
										}
											//syslog(LOG_INFO,
													//"[DM]: Zigbee framework observe response  color_end ");
											break;
										}
									}
								}
								break;
							} else {
								syslog(LOG_INFO,
										"[DM]: Zigbee framework observe response wrong end point ");
							}
						}
					}

					RequestResponseOBJ.Command = DM_OBSERVE;
					RequestResponseOBJ.Message = "Received observe response";
					RequestResponseOBJ.Status_Code = DM_SUCCESS;
					RequestResponseOBJ.Message_Id = std::to_string(
							DMReqStructZBObj.MsgId);
					RequestResponseOBJ.Client =
							std::stol(
									std::string(
											std::to_string(
													DMReqStructZBObj.MsgId)).substr(
											0, 2));

					/*syslog(LOG_INFO,"[DM]: Th :%s E_ID :%s  name : %s  type :%s  val: %s ",
RequestResponseOBJ.Thing_Id.c_str(),RequestResponseOBJ.Entity_Id.c_str(),RequestResponseOBJ.Property_Name.c_str(),RequestResponseOBJ.Property_Type.c_str(),RequestResponseOBJ.Property_Value.c_str());*/
					if (!RequestResponseOBJ.Thing_Id.empty()
							&& !RequestResponseOBJ.Entity_Id.empty()
							&& !RequestResponseOBJ.Property_Name.empty()
							&& !RequestResponseOBJ.Property_Type.empty()
							&& !RequestResponseOBJ.Property_Value.empty()) {
						DmPlatform::DmSendingResponse(RequestResponseOBJ);
					} else {
						syslog(LOG_ERR,
								"[DM]: Zigbee Framework observe response stopped processing empty data to TronX");
					}

				} else {
					syslog(LOG_DEBUG,
							"[DM]: Zigbee framework observe response removing thing_id: [%s] not found in main map",
							RequestResponseOBJ.Thing_Id.c_str());
				}
			

				//syslog(LOG_INFO, "[DM]: Zigbee framework observe response end");
					}
				break;

				/*syslog(LOG_INFO,
						"[DM]: Zigbee framework mode_id_nodeid_change response end");*/
			default: {
				syslog(LOG_INFO,
						"[DM]: Zigbee framework default response case");
			}
				break;
			}

		} else {

			syslog(LOG_DEBUG,
					"[DM]: Zigbee framework non discovery error code: [%d]",
					DMReqStructZBObj.ErrorCode);

			RequestResponseOBJ.Property_Name =
					DmFrameworkHandle::PropertyNameStopReqHandleThread(
							std::string("ZB")
									+ std::to_string(DMReqStructZBObj.MsgId));

			RequestResponseOBJ.Thing_Id = std::string("ZB")
										+ std::to_string(DMReqStructZBObj.IEEE);

			syslog(LOG_DEBUG, "[DM]: Zigbee framework non discovery Mode: [%d]",
					DMReqStructZBObj.Mode);

			switch (DMReqStructZBObj.Mode) {

			case MODE_ID_COMMISINING: {
				RequestResponseOBJ.Command = DM_DISCOVERY;
			}
				break;
			case MODE_ID_CONTROL: {
				RequestResponseOBJ.Command = DM_CONTROL;
				RequestResponseOBJ.Entity_Id = std::to_string((int) DMReqStructZBObj.endpoint);
			}
				break;
			case MODE_ID_OBSERVE: {
				RequestResponseOBJ.Command = DM_OBSERVE;

				if (DMReqStructZBObj.ErrorCode == 7) {
					RequestResponseOBJ.Message_Id = std::to_string(
					DM_OBSERVE_MSG_ID);

					DmThingMap* DmThingMapObj = DmThingMap::GetInstance();
					std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
							DmThingMapObj->DmThingDataMap.find(
									RequestResponseOBJ.Thing_Id);
					if (DmThingMapObjItr
							!= DmThingMapObj->DmThingDataMap.end()) {
						/* setting the battery status in map and DB*/
						
						syslog(LOG_DEBUG,
								"[DM]: Zwave Framework OBSERVE Response for  BATTERY STATUS [%d]",DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue);
						DmThingMapObjItr->second->setbattery_status(
								std::to_string(
										(DMReqStructZBObj.argValue_info[DMReqStructZBObj.argcount].argValue.intargValue)
																		/ 2));
																		
						syslog(LOG_DEBUG,
								"[DM]: Zwave Framework OBSERVE Response for  SET BATTERY STATUS [%s]",DmThingMapObjItr->second->getbattery_status().c_str());
						DmPlatform::DMDeviceDetailsUpdate(
								RequestResponseOBJ.Thing_Id);
					}

				}


			}
				break;
			case MODE_ID_REMOVE: {
				RequestResponseOBJ.Command = DM_REMOVE;
			}
				break;
			case MODE_ID_LEFT: {
				RequestResponseOBJ.Command = DM_REMOVE;
			}
				break;
			case MODE_ID_ZBFOTA: {
				RequestResponseOBJ.Command = "FOTA_REQ";
			}
				break;


			default: {
				RequestResponseOBJ.Command = "DM_DEFAULT";
			}
				break;
			}

			RequestResponseOBJ.Client = std::stoi(
					RequestResponseOBJ.Message_Id.substr(0, 2));

			RequestResponseOBJ.Message = ZbMappingDatabase::DMErrorTypeZB(
					(int) DMReqStructZBObj.ErrorCode);

			RequestResponseOBJ.Status_Code = ZbMappingDatabase::DMErrorCodeZB(
					(int) DMReqStructZBObj.ErrorCode);

			if (!RequestResponseOBJ.Property_Name.empty()) {
				DmPlatform::DmSendingResponse(RequestResponseOBJ);
			} else {
				syslog(LOG_ERR,
						"[DM]: Zigbee Framework error code response stopped processing empty data to TronX");
			}

		}
	}

}

void ZbFramework::ZbReceive() {

	syslog(LOG_INFO, "[DM]: Zigbee framework start mq receive main thread ");

	std::thread ZigBeeDiscTh(ZBDiscoveryResponse);
	std::thread ZigBeeNonDiscTh(ZBNonDiscoveryResponse);

	ZigBeeDiscTh.join();
	ZigBeeNonDiscTh.join();

}

void ZbFramework::stopdiscovery(void) {

	DMReqStructZB DMReqStructZBOBJStopDisc;

	memset(&DMReqStructZBOBJStopDisc, 0, sizeof(DMReqStructZBOBJStopDisc));

	DMReqStructZBOBJStopDisc.mtype = 1;
	DMReqStructZBOBJStopDisc.Mode = MODE_ID_STOP_COMMISINING;

	ZbMessageQueue::ZigbeeMsgQueueReq(DMReqStructZBOBJStopDisc);

	/*syslog(LOG_INFO,
			"[DM]: Zigbee framework request stop discovery send to Zigbee MQ");*/
}

