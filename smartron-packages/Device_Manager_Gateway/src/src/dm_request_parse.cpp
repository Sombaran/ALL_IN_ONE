#include "dm_request_parse.hpp"
#include "dm_platform.hpp"
#include "dm_request_queue.hpp"
#include "dm_zeromq.hpp"
#include "dm_thingmap.hpp"
#include "dm_req_res_mapping.hpp"
#include "dm_secne_mapping.hpp"
#include "dm_rule_mapping.hpp"
#include "dm_utility.hpp"
#include "dm_things_backup_db.hpp"
#include "dm_response_queue.hpp"


#define ZERO_LENGTH 0
extern pthread_cond_t DM_Req_Discovery;
extern char DM_KILL_SWITCH[32];
std::string removeThingID;
extern int va_on;
extern int volume_present;

bool FlagDiscSend = false;
bool FlagFota = false;
bool s2securityflag;
bool ExclusionFlag = false;
bool RemoveFlag = false;
bool ControlFlag = false;
bool QueueSizeLimit = false;
uint8_t RuleStatusFlag = 0;
pthread_mutex_t RuleStatusLock;
extern int sockfd;
extern sockaddr_in fota_servaddr;
extern uint8_t ArmStatusFlag;
extern pthread_mutex_t ArmStatusLock;
extern void Audio_pause();
extern void set_volume(int);
std::string FotaThingID = "\0"; /*var to store the thing ID of the end device undergoing firmware update  */
std::string FotaMsgID = "\0"; /*var to store the firmware update message_Id */

void AddDMSendResponse(json_object* client, json_object* notif_id,
		json_object* message_id, std::string message, json_object* command,
		int status_code, json_object* RuleSecenedata) {

	RequestResponse RequestResponseOBJ;

	RequestResponseOBJ.Client = std::stol(json_object_get_string(client));
	RequestResponseOBJ.Command = json_object_get_string(command);
	RequestResponseOBJ.Message = message; //"PROPERTY_VALUE IS WRONG";
	RequestResponseOBJ.Message_Id = json_object_get_string(message_id);
	RequestResponseOBJ.Notif_Id = json_object_get_string(notif_id);
	RequestResponseOBJ.RuleSceneData = json_object_get_string(RuleSecenedata);
	RequestResponseOBJ.Status_Code = status_code; //DM_FAIL;

	DmPlatform::DmSendingResponse(RequestResponseOBJ);
}

void MakeDMReqQueueAddMessage(json_object* client, json_object* notif_id,
		json_object* message_id, json_object* thing_id, json_object* command,
		json_object*pass_phrase) {
	DmRequestQueue *DmRequestQueueOBJ = DmRequestQueue::GetInstance();

	RequestResponse ReqMessageDataOBJ;

	ReqMessageDataOBJ.Client = std::stol(json_object_get_string(client));
	ReqMessageDataOBJ.Message_Id = json_object_get_string(message_id);
	ReqMessageDataOBJ.Command = json_object_get_string(command);
	ReqMessageDataOBJ.Notif_Id = json_object_get_string(notif_id);

	ReqMessageDataOBJ.RequestCount = 1;

	ReqMessageDataOBJ.priority = PRORITY_VALUE;

	ReqMessageDataOBJ.Thing_Id = json_object_get_string(thing_id); // "DM_DEFAULT";

	ReqMessageDataOBJ.Entity_Id = "DM_DEFAULT";

	ReqMessageDataOBJ.Property_Type = "DM_DEFAULT";
	ReqMessageDataOBJ.Property_Name = "DM_DEFAULT";
	ReqMessageDataOBJ.Property_Range = "DM_DEFAULT";
	ReqMessageDataOBJ.Property_Value = "DM_DEFAULT";

	ReqMessageDataOBJ.RuleSceneData = "DM_DEFAULT";

	ReqMessageDataOBJ.Thing_Response = std::string(
			json_object_get_string(pass_phrase));

	int Operation = ConvertStringToInt(
			std::string(json_object_get_string(command)));

	switch (Operation) {

	case CONTROL:
		ReqMessageDataOBJ.priority = PRIORITY_CONTROL;
		break;

	case REMOVE:
		ReqMessageDataOBJ.priority = PRIORITY_REMOVE;
		break;

	case DISCOVERY:
		ReqMessageDataOBJ.priority = PRIORITY_DISCOVERY;
		break;

	case EXCLUSION:
		ReqMessageDataOBJ.priority = PRIORITY_EXCLUSION;
		break;

	case S2_SECURITY:
		ReqMessageDataOBJ.priority = PRIORITY_S2SECURITY;
		break;
	}

	syslog(LOG_DEBUG, "DM: MakeDMReqQueueAddMessage- Cmd:%s,Pri:%d",
			ReqMessageDataOBJ.Command.c_str(), ReqMessageDataOBJ.priority);
	/*syslog(LOG_INFO,
	 "[DM]: Request queue process request IN SECURITY2 RECIEVE ************");*/

	DmRequestQueueOBJ->DMReqQueueAddMessage(ReqMessageDataOBJ);

}

void MakeDMReqQueueAddMessage(json_object* client, json_object* notif_id,
		json_object* message_id, json_object* thing_id, json_object* command,
		json_object*pass_phrase, json_object* firmware_path,
		json_object* firmware_version) {

	DmRequestQueue *DmRequestQueueOBJ = DmRequestQueue::GetInstance();

	RequestResponse ReqMessageDataOBJ;

	ReqMessageDataOBJ.Client = json_object_get_int(client);
	ReqMessageDataOBJ.Message_Id = json_object_get_string(message_id);
	ReqMessageDataOBJ.Command = json_object_get_string(command);

	ReqMessageDataOBJ.Firmware_version = json_object_get_string(
			firmware_version);
	ReqMessageDataOBJ.Firmware_path = json_object_get_string(firmware_path);
	ReqMessageDataOBJ.Notif_Id = json_object_get_string(notif_id);

	ReqMessageDataOBJ.RequestCount = 1;

	ReqMessageDataOBJ.priority = PRORITY_VALUE;

	ReqMessageDataOBJ.Thing_Id = json_object_get_string(thing_id); // "DM_DEFAULT";

	ReqMessageDataOBJ.Entity_Id = "DM_DEFAULT";

	ReqMessageDataOBJ.Property_Type = "DM_DEFAULT";
	ReqMessageDataOBJ.Property_Name = "DM_DEFAULT";
	ReqMessageDataOBJ.Property_Range = "DM_DEFAULT";
	ReqMessageDataOBJ.Property_Value = "DM_DEFAULT";

	ReqMessageDataOBJ.RuleSceneData = "DM_DEFAULT";

	ReqMessageDataOBJ.Thing_Response = std::string(
			json_object_get_string(pass_phrase));

	ReqMessageDataOBJ.priority = PRIORITY_FOTA; /* states the prority level of the firmware update request among other request*/

	syslog(LOG_DEBUG, "DM: FOTA -> MakeDMReqQueueAddMessage- Cmd:%s,Pri:%d",
			ReqMessageDataOBJ.Command.c_str(), ReqMessageDataOBJ.priority);

	DmRequestQueueOBJ->DMReqQueueAddMessage(ReqMessageDataOBJ);

}

void Thing_Check_AddingQueue(json_object* ThingData, int RequestCount,
		int priority) {

	syslog(LOG_INFO,
			"[DM]: ZERO MSG RPC in request thing_check_adding to queue: [%s]",
			json_object_get_string(ThingData));

	json_type type = json_object_get_type(ThingData);
	if (type != json_type_null) {
		//--------------------------------------------------------------------------------
		json_object* client = json_object_object_get(ThingData, "client");
		json_object* command = json_object_object_get(ThingData, "command");
		json_object* message_id = json_object_object_get(ThingData,
				"message_id");
		json_object* notif_id = json_object_object_get(ThingData, "notif_id");
		json_object* thing_id = json_object_object_get(ThingData, "thing_id");
		json_object* entity_id = json_object_object_get(ThingData, "entity_id");
		json_object* property_type = json_object_object_get(ThingData,
				"property_type");
		json_object* property_name = json_object_object_get(ThingData,
				"property_name");
		json_object* property_range = json_object_object_get(ThingData,
				"property_range");
		json_object* property_value = json_object_object_get(ThingData,
				"property_value");
		//--------------------------------------------------------------------------------

		DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

		int EntityNo = 0;
		int PropertyNo = 0;

		/*bool Entity_id_flag = false;
		 bool Property_type_flag = false;
		 bool Property_name_flag = false;
		 bool Property_value_flag = false;*/

		uint8_t Thing_Data_Status = 0;

		std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
				DmThingMapObj->DmThingDataMap.find(
						json_object_get_string(thing_id));
		if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {

			for (EntityNo = 0;
					EntityNo < DmThingMapObjItr->second->getEntityListSize();
					EntityNo++) {

				if (DmThingMapObjItr->second->getEntityInfo(EntityNo)->getentity_id()
						== std::string(json_object_get_string(entity_id))) {

					//Entity_id_flag = true;
					Thing_Data_Status = Requested_Thing_Data_Good;
					syslog(LOG_INFO,
							"[DM]: ZERO MSG RPC in request entity_id present in main map");
					for (PropertyNo = 0;
							PropertyNo
									< DmThingMapObjItr->second->getEntityInfo(
											EntityNo)->getPropertySize();
							PropertyNo++) {
						if (DmThingMapObjItr->second->getEntityInfo(EntityNo)->getPropertyInfo(
								PropertyNo)->getproperty_type()
								== std::string(
										json_object_get_string(
												property_type))) {

							//syslog(LOG_INFO,
							//		"[DM]: ZERO MSG RPC in request property_type present in main map");
							//Property_type_flag = true;
							Thing_Data_Status = Requested_Thing_Data_Good;

							break;

						} else {
							Thing_Data_Status = property_type_wrong;
						}
					}

					for (PropertyNo = 0;
							PropertyNo
									< DmThingMapObjItr->second->getEntityInfo(
											EntityNo)->getPropertySize();
							PropertyNo++) {

						if (DmThingMapObjItr->second->getEntityInfo(EntityNo)->getPropertyInfo(
								PropertyNo)->getproperty_name()
								== std::string(
										json_object_get_string(
												property_name))) {

							//syslog(LOG_INFO,
							//		"[DM]: ZERO MS GRPC in request property_name present in main map");
							//Property_name_flag = true;
							Thing_Data_Status = Requested_Thing_Data_Good;

							if (strcmp(json_object_get_string(property_type),
									"SWITCH") == 0) {

								if (!(std::stol(
										json_object_get_string(property_value))
										== 0
										|| std::stol(
												json_object_get_string(
														property_value)) == 1)) {
									//syslog(LOG_INFO,
									//		"[DM]: ZERO MSG RPC in request property_value present in main map");
									Thing_Data_Status =
									property_value_wrong;
									//Property_value_flag = true;
									break;
								}
							} else if (strcmp(
									json_object_get_string(property_type),
									"RANGE") == 0) {
								if (!(std::stol(
										json_object_get_string(property_value))
										>= 0
										|| std::stol(
												json_object_get_string(
														property_value)) <= 100)) {
									Thing_Data_Status =
									property_value_wrong;
									//Property_value_flag = true;
									break;
								}

							} else if (strcmp(
									json_object_get_string(property_type),
									"COLOR") == 0) {

								//Property_value_flag = true;

							} else if (strcmp(
									json_object_get_string(property_type),
									"LIST") == 0) {
								Thing_Data_Status = Requested_Thing_Data_Good;
								//Property_value_flag = true;

							} else if (strcmp(
									json_object_get_string(property_type),
									"INFO") == 0) {
								Thing_Data_Status = Requested_Thing_Data_Good; // we have to verify this
								//Property_value_flag = true;

							} else {
								syslog(LOG_INFO,
										"[DM]: ZERO MSG RPC in request property_value checking");
							}
							break;
						} else {

							Thing_Data_Status = property_name_wrong;
						}
					}
					break;
				} else {
					Thing_Data_Status = Entity_id_wrong;
				}
			}
			syslog(LOG_INFO, "[DM]: ZERO MSG RPC in request property_val %d \n",
					Thing_Data_Status);

			switch (Thing_Data_Status) {
			case Requested_Thing_Data_Good: {
				DmRequestQueue *DmRequestQueueOBJ =
						DmRequestQueue::GetInstance();
				RequestResponse ReqMessageDataOBJ;

				ReqMessageDataOBJ.Client = std::stol(
						std::string(json_object_get_string(message_id)).substr(
								0, 2));
				ReqMessageDataOBJ.Thing_Id = json_object_get_string(thing_id);
				ReqMessageDataOBJ.Notif_Id = json_object_get_string(notif_id);
				ReqMessageDataOBJ.RequestCount = RequestCount;
				ReqMessageDataOBJ.Command = json_object_get_string(command);
				ReqMessageDataOBJ.priority = priority;
				ReqMessageDataOBJ.Message_Id = json_object_get_string(
						message_id);
				ReqMessageDataOBJ.Entity_Id = json_object_get_string(entity_id);
				ReqMessageDataOBJ.Property_Type = json_object_get_string(
						property_type);
				ReqMessageDataOBJ.Property_Name = json_object_get_string(
						property_name);
				ReqMessageDataOBJ.Property_Range = json_object_get_string(
						property_range);
				ReqMessageDataOBJ.Property_Value = json_object_get_string(
						property_value);

				DmRequestQueueOBJ->DMReqQueueAddMessage(ReqMessageDataOBJ); //Not optimized because memory constraint

				syslog(LOG_INFO,
						"[DM]: ZERO MSG RPC in request adding into request queue");
			}
				break;

			case Entity_id_wrong: {
				syslog(LOG_INFO,
						"[DM]: ZERO MSG RPC in request entity_id not present in main map");

				DmReqResMapping* DmReqResMappingOBJ =
						DmReqResMapping::GetInstance();

				DmReqResMappingOBJ->AddToDmReqResMapping(
						json_object_get_string(notif_id),
						json_object_get_string(message_id), RequestCount);

				json_object* Default_RulesceneData = json_object_new_string(
						"DM_DEFAULT");
				AddDMSendResponse(client, notif_id, message_id,
						"Invalid entity data", command, DM_FAIL,
						Default_RulesceneData);
			}
				break;

			case property_type_wrong: {
				syslog(LOG_INFO,
						"[DM]: ZERO MSG RPC in request property_type not present in main map");

				DmReqResMapping* DmReqResMappingOBJ =
						DmReqResMapping::GetInstance();

				DmReqResMappingOBJ->AddToDmReqResMapping(
						json_object_get_string(notif_id),
						json_object_get_string(message_id), RequestCount);

				json_object* Default_RulesceneData = json_object_new_string(
						"DM_DEFAULT");
				AddDMSendResponse(client, notif_id, message_id,
						"Invalid property data", command, DM_FAIL,
						Default_RulesceneData);
				////json_object_put(Default_RulesceneData);
			}
				break;

			case property_name_wrong: {
				syslog(LOG_INFO,
						"[DM]: ZERO MSG RPC in request property_name not present in main map");

				DmReqResMapping* DmReqResMappingOBJ =
						DmReqResMapping::GetInstance();

				DmReqResMappingOBJ->AddToDmReqResMapping(
						json_object_get_string(notif_id),
						json_object_get_string(message_id), RequestCount);

				json_object* Default_RulesceneData = json_object_new_string(
						"DM_DEFAULT");
				AddDMSendResponse(client, notif_id, message_id,
						"Invalid property data", command, DM_FAIL,
						Default_RulesceneData);
				////json_object_put(Default_RulesceneData);
			}
				break;

			case property_value_wrong: {
				syslog(LOG_INFO,
						"[DM]: ZERO MSG RPC in request property_value not present in main map");

				DmReqResMapping* DmReqResMappingOBJ =
						DmReqResMapping::GetInstance();

				DmReqResMappingOBJ->AddToDmReqResMapping(
						json_object_get_string(notif_id),
						json_object_get_string(message_id), RequestCount);

				json_object* Default_RulesceneData = json_object_new_string(
						"DM_DEFAULT");
				AddDMSendResponse(client, notif_id, message_id,
						"Invalid property data", command, DM_FAIL,
						Default_RulesceneData);
				////json_object_put(Default_RulesceneData);
			}
				break;

			default: {
				//Print some Critical logs
				syslog(LOG_INFO,
						"[DM]: ZERO MSG RPC in request in Default case %d \n",
						Thing_Data_Status);
			}
				break;
			}
		} else {

			syslog(LOG_INFO,
					"[DM]: &&&&&& THING_ID NOT PRESENT IN MAIN MAP BUT IS THERE [%s]",
					json_object_get_string(thing_id));
			//////////////////////////////////////////////////////////////////
//			json_object *thingsinfo = json_object_new_object();
//			json_object *thing_Id = json_object_new_string(json_object_get_string(thing_id));
			removeThingID = json_object_get_string(thing_id);
			syslog(LOG_DEBUG,
						 "[DM]:Dm_requestparse rrmove thingID [%s]",removeThingID.c_str());
//			DmResponseQueue* DmResponseQueueOBJ = DmResponseQueue::GetInstance();
//			RequestResponse RequestResponseOBJ;
//			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ,
//					"DELETE", "/hub/thing/remove",
//					json_object_get_string(thing_id),
//					PRORITY_VALUE);
		/////////////////////////////////////////////////////////////////////////////
			DmReqResMapping* DmReqResMappingOBJ =
					DmReqResMapping::GetInstance();

			DmReqResMappingOBJ->AddToDmReqResMapping(
					json_object_get_string(notif_id),
					json_object_get_string(message_id), RequestCount);

			//SWAMI	usleep(300);

			json_object* Default_RulesceneData = json_object_new_string(
					"DM_DEFAULT");

			AddDMSendResponse(client, notif_id, message_id,
					"Device does not exists", command, THING_NOT_PRESENT_IN_DM,
					Default_RulesceneData);

		}

	} else {
		syslog(LOG_ERR, "[DM]: Received Thing Data data is not Json data");
	}
}

bool JsonDataCheck(json_object* reqJson) {

	json_object* things = json_object_object_get(reqJson, "things");

	if (things == NULL || json_object_get_type(things) != json_type_array)
		return false;

	for (size_t messageSize = 0; messageSize < json_object_array_length(things);
			messageSize++) {

		json_object* ThingData1 = json_tokener_parse(
				json_object_get_string(
						json_object_array_get_idx(things, messageSize)));

		json_object* thing_id = json_object_object_get(ThingData1, "thing_id");
		if (thing_id == NULL)
			return false;
		json_object* entity_id = json_object_object_get(ThingData1,
				"entity_id");
		if (entity_id == NULL)
			return false;
		json_object* property_type = json_object_object_get(ThingData1,
				"property_type");
		if (property_type == NULL)
			return false;
		json_object* property_name = json_object_object_get(ThingData1,
				"property_name");
		if (property_name == NULL)
			return false;
		json_object* property_value = json_object_object_get(ThingData1,
				"property_value");
		if (property_value == NULL)
			return false;
	}

	return true;
}

void DMSceneRuleParseVA(int Client, std::string Command, std::string Notif_id,
		std::string Message_id, json_object* things) {

//	bool AddReqInQueue = false;

	syslog(LOG_DEBUG,
			"[DM]: zero msg rpc dmscene_rule_parse client : [%d] command : [%s] notif_id : [%s] message_id : [%s] thingrequest : [%s]",
			Client, Command.c_str(), Notif_id.c_str(), Message_id.c_str(),
			json_object_get_string(things));

	json_object* thing_id = json_object_new_object();
	json_object* entity_id = json_object_new_object();

	json_type jsontypeofthings = json_object_get_type(things);
	json_type jsontypeofproperties;
	size_t lengthofthings;

	/** Checking whether the json structure of things is array or not */
	if (jsontypeofthings == json_type_array) {
		lengthofthings = json_object_array_length(things);
	} else {
		lengthofthings = ZERO_LENGTH;
	}

	int RequestCount = 0;
	for (size_t messageSize = 0; messageSize < lengthofthings; messageSize++) {
		json_object* TotalData = json_object_array_get_idx(things, messageSize);
		json_object* properties = json_object_object_get(TotalData,
				"properties");

		jsontypeofproperties = json_object_get_type(properties);
		size_t lengthofproperties;
		/** Checking whether the json structure of properties is array or not */
		if (jsontypeofproperties == json_type_array) {
			lengthofproperties = json_object_array_length(properties);
		} else {
			lengthofproperties = ZERO_LENGTH;
		}

		RequestCount = RequestCount + lengthofproperties;
	}

	syslog(LOG_DEBUG, "[DM]: Request queue parse thing control: [%ld]",
			lengthofthings);

	/*
	 if ((Command == "RULE") && (RequestCount > 0)) {
	 pthread_mutex_lock(&RuleStatusLock);
	 RuleStatusFlag = RuleStatusFlag + 1;
	 syslog(LOG_INFO, "[DM]: RuleStatusFlag [%d] ", RuleStatusFlag);
	 pthread_mutex_unlock(&RuleStatusLock);
	 }
	 */

	for (size_t messageSize = 0; messageSize < lengthofthings; messageSize++) {

		json_object* TotalData = json_object_array_get_idx(things, messageSize);

		thing_id = json_object_object_get(TotalData, "thing_id");

		if (strcmp(FotaThingID.c_str(), json_object_get_string(thing_id))
				!= 0) {

			entity_id = json_object_object_get(TotalData, "entity_id");

			json_object* properties = json_object_object_get(TotalData,
					"properties");

			jsontypeofproperties = json_object_get_type(properties);
			size_t lengthofproperties;
			/** Checking whether the json structure of properties is array or not */
			if (jsontypeofproperties == json_type_array) {
				lengthofproperties = json_object_array_length(properties);
			} else {
				lengthofproperties = ZERO_LENGTH;
			}

			for (size_t messageSizeP = 0; messageSizeP < lengthofproperties;
					messageSizeP++) {

				json_object* TotalDataP = json_object_array_get_idx(properties,
						messageSizeP);

				//			json_object* property_type = json_object_object_get(TotalDataP,
				//					"property_type");
				//			json_object* property_name = json_object_object_get(TotalDataP,
				//					"property_name");
				//			json_object* property_value = json_object_object_get(TotalDataP,
				//					"property_value");

				//			json_object *client = json_object_new_int(Client);

				//			json_object *notif_id = json_object_new_string(Notif_id.c_str());

				json_object* message_id;
				json_object* command;

				/** Parsing the all the scene(voice or mobile initiated) requests into control requests */
				if (Command == "SCENE" /*|| Command == "VOICE_SCENE"*/) {

					message_id = json_object_new_string(
							(std::to_string(SCENE_CONTROL).insert(2,
							DM_SCENE) + std::to_string(rand() % 900)).c_str());

					command = json_object_new_string("CONTROL");

				} else if (Command == "RULE") {

					message_id = json_object_new_string(
							(std::to_string(RULE_COLTROL).insert(2,
							DM_RULE) + std::to_string(rand() % 900)).c_str());

					command = json_object_new_string("CONTROL");

				}

				json_object* SingleReq = json_object_new_object();

				json_object_object_add(SingleReq, "client",
						json_object_new_int(Client));
				json_object_object_add(SingleReq, "command", command);
				json_object_object_add(SingleReq, "message_id", message_id);
				json_object_object_add(SingleReq, "notif_id",
						json_object_new_string(Notif_id.c_str()));

				json_object_object_add(SingleReq, "thing_id", thing_id);
				json_object_object_add(SingleReq, "entity_id", entity_id);

				json_object_object_add(SingleReq, "property_range",
						json_object_new_string("DM_DEFAULT"));
				json_object_object_add(SingleReq, "property_name",
						json_object_object_get(TotalDataP, "property_name"));
				json_object_object_add(SingleReq, "property_type",
						json_object_object_get(TotalDataP, "property_type"));
				json_object_object_add(SingleReq, "property_value",
						json_object_object_get(TotalDataP, "property_value"));

				if (Command == "SCENE" || Command == "VOICE_SCENE") {

					Thing_Check_AddingQueue(SingleReq, RequestCount,
					PRIORITY_SCENE);
				} else {

					Thing_Check_AddingQueue(SingleReq, RequestCount,
					PRIORITY_RULE);
				}

				syslog(LOG_INFO,
						"[DM]: Request queue parse adding into request queue success");
			}
		}
	}
	syslog(LOG_INFO, "[DM]: Request queue parse out parse things");
//	////json_object_put(thing_id);
//	////json_object_put(entity_id);
}

void requestfailurefromclient(int Client, std::string Command,
		json_object* notif_id, json_object* message_id, const char *message,
		int status_code) {

	RequestResponse ReqMessageDataOBJ;
	ReqMessageDataOBJ.Client = long(Client);
	ReqMessageDataOBJ.Message_Id = json_object_get_string(message_id);
	ReqMessageDataOBJ.Message = message;
	ReqMessageDataOBJ.Command = Command.c_str();
	ReqMessageDataOBJ.Notif_Id = json_object_get_string(notif_id);
	ReqMessageDataOBJ.RequestCount = 1;
	ReqMessageDataOBJ.priority = PRORITY_VALUE;
	ReqMessageDataOBJ.Thing_Id = "DM_DEFAULT";
	ReqMessageDataOBJ.Entity_Id = "DM_DEFAULT";
	ReqMessageDataOBJ.Property_Type = "DM_DEFAULT";
	ReqMessageDataOBJ.Property_Name = "DM_DEFAULT";
	ReqMessageDataOBJ.Property_Range = "DM_DEFAULT";
	ReqMessageDataOBJ.Property_Value = "DM_DEFAULT";
	ReqMessageDataOBJ.RuleSceneData = "DM_DEFAULT";
	ReqMessageDataOBJ.Status_Code = status_code;

	DmPlatform::DmSendingRequestAck(ReqMessageDataOBJ);

	//DmPlatform::DmSendingResponse(ReqMessageDataOBJ);
}

void ZeroMessageRequestParse(const char* DM_ZMQ_MSG_RCV) {

	int incomingClient; /* Variable to store the incoming client ID of the request*/
	std::string incomingCommand; /* Variable to store the incoming command*/

	//syslog(LOG_INFO, "[DM]: Request queue parse request start");
	std::string Message_ID_Client = "DM_DEFAULT";
	//bool Scene_Execute_state;
	json_object* reqJson = json_tokener_parse(DM_ZMQ_MSG_RCV); /* Assigning a pointer to the Received request buffer */
	json_type type = json_object_get_type(reqJson);
	//syslog(LOG_INFO, "[DM]: Request queue parse request type:[%d]", type);
	if (type != json_type_null) {

		json_object* client = json_object_object_get(reqJson, "client");
		json_object* command = json_object_object_get(reqJson, "command");
		incomingClient = json_object_get_int(client); /* extracting the Client and command ID to differentiate the request  */
		incomingCommand = json_object_get_string(command);
		json_object* notif_id = json_object_new_string(
				std::to_string(rand() % 900).c_str()); /* Default notification Id generated to track the request*/

		json_object* message_id = json_object_new_object();
		json_object* verb = json_object_object_get(reqJson, "verb");

//--------------------------------------------------------------------------------------------------

		syslog(LOG_DEBUG,
				"[DM]: Dm_requestparse from Client : [%d] Command :[%s]",
				incomingClient, incomingCommand.c_str());

		if (incomingClient == SOCKET_SERVICE || incomingClient == MOBILE_AGENT) {
			notif_id = json_object_object_get(reqJson, "notif_id"); /*Notification Id sent by clients (MA / WS) */
		}

		switch (incomingClient) {
		case CRON_HANDLER: //!To ensure that we got request from proper Client
		case SOCKET_SERVICE:
		case MOBILE_AGENT:
		case MQTT_SERVICE: {
			syslog(LOG_DEBUG,
					"[DM]: Dm_requestparse from Valid Client : [%d] Command :[%s]",
					incomingClient, incomingCommand.c_str());
			/*std::to_string(CRON_HANDLER)*/
			Message_ID_Client = std::to_string(incomingClient)
					+ std::string("00") + std::to_string(rand() % 900);
			json_object_object_add(reqJson, "message_id",
					json_object_new_string(Message_ID_Client.c_str()));
			message_id = json_object_object_get(reqJson, "message_id");

			int Request_Status = REQUEST_SUCCESS;

			DmRequestQueue* DmRequestQueueObj = DmRequestQueue::GetInstance();
			if ((strcmp(incomingCommand.c_str(), "RULE") != 0) // Only allowing Rule request to add into queue even overflow happens to 64
			&& (DmRequestQueueObj->DMReqQueueDataTronx.size() >= 64)) {	// Checking the request queue size if reached to 64, and stopping that request.
				QueueSizeLimit = true;
			}

			pthread_mutex_lock(&RuleStatusLock);
			syslog(LOG_DEBUG, "[DM]: Dm_requestparse /'RuleStatusFlag/':[%d],/'SceneStatusFlag/':[%d]",
					RuleStatusFlag, DmSceneMapping::DMGetSceneMapExecuteflag());
			Request_Status = ((int) FlagDiscSend << 1)
					| ((int) ExclusionFlag << 2)
					| ((int) DmSceneMapping::DMGetSceneMapExecuteflag() << 3
							| (int) ((bool) RuleStatusFlag) << 4
							| ((int) RemoveFlag << 5) | ((int) ControlFlag << 6)
							| ((int) QueueSizeLimit << 7)
							| ((int) FlagFota << 8));
			pthread_mutex_unlock(&RuleStatusLock);
			syslog(LOG_DEBUG, "[DM]: Dm_requestparse requestflag_status = [%d]",
					Request_Status);
			switch (Request_Status) {
			case REQUEST_SUCCESS: { /* Currently no request is under processing */

				syslog(LOG_DEBUG,
						"[DM]: Dm_requestparse message_id [%s],notif_id [%s]",
						json_object_get_string(message_id),
						json_object_get_string(notif_id));
				if (strcmp(incomingCommand.c_str(), "SCENE") == 0)
					requestfailurefromclient(incomingClient,
							incomingCommand.c_str(), notif_id, message_id,
							(char *) "Scene initiated successfully",
							DM_SUCCESS);
				else
					requestfailurefromclient(incomingClient,
							incomingCommand.c_str(), notif_id, message_id,
							(char *) "Request processing successfully",
							DM_SUCCESS);
			}
				break;

			case DISCOVERY_ALREADY_PROCESS: {

				syslog(LOG_DEBUG,
						"[DM]: Dm_requestparse from valid client, but discovery already in progress");
				requestfailurefromclient(incomingClient,
						incomingCommand.c_str(), notif_id, message_id,
						(char *) "Discovery in progress",
						DISCOVERY_ALREADY_IN_PROCESS);
				if ((((strcmp(incomingCommand.c_str(), "RULE") == 0)
						&& (strcmp(json_object_get_string(verb), "RUN") == 0))
						|| (strcmp(incomingCommand.c_str(), "S2_SECURITY") == 0))) {
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse to allow /'RULE TO RUN & S2_SECURITY/' during /'DISCOVERY/' ");
				}
				//	else if (strcmp(json_object_get_string(command), "CONTROL")
				//	== 0) {
				//	syslog(LOG_DEBUG,
				//	"[DM]: Request for CONTROL,But scene already in process, so Adding to Queue");
				//
				//	requestfailurefromclient(incomingClient, command, notif_id,
				//	message_id, "DM_CONTROL_COMPLETE",
				//	DM_SUCCESS);
				//
				//	}
				else {
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse not to allow other operations other than "
							"/'RULE TO RUN & S2_SECURITY/' during /'DISCOVERY/'");
					return;
				}
			}
				break;

			case EXECLUSION_ALREADY_PROCESS: {

				syslog(LOG_DEBUG,
						"[DM]: Dm_requestparse from valid client, but exclusion already in progress");
				if ((strcmp(incomingCommand.c_str(), "RULE") == 0)
						&& (strcmp(json_object_get_string(verb), "RUN") == 0)) {
					requestfailurefromclient(incomingClient,
							incomingCommand.c_str(), notif_id, message_id,
							(char *) "Exclusion in progress, so queuing request",
							DM_SUCCESS);
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse to allow /'RULE TO RUN /' during /'EXCLUSION/' ");
				} else {
					requestfailurefromclient(incomingClient,
							incomingCommand.c_str(), notif_id, message_id,
							(char *) "Exclusion in progress",
							SCENE_ALREADY_PROCESSING);
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse not to allow other operations other than "
							"/'RULE TO RUN /' during /'EXCLUSION/'");
					return;
				}
			}

				break;

			case REMOVE_ALREADY_PROCESS: {

				syslog(LOG_DEBUG,
						"[DM]: Dm_requestparse from valid client, but remove already in progress");
				if ((strcmp(incomingCommand.c_str(), "RULE") == 0)
						&& (strcmp(json_object_get_string(verb), "RUN") == 0)) {
					requestfailurefromclient(incomingClient,
							incomingCommand.c_str(), notif_id, message_id,
							(char *) "Remove in progress, so queuing request",
							DM_SUCCESS);
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse to allow /'RULE TO RUN /' during /'REMOVE/' ");
				} else {
					requestfailurefromclient(incomingClient,
							incomingCommand.c_str(), notif_id, message_id,
							(char *) "Remove in progress",
							SCENE_ALREADY_PROCESSING);
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse not to allow other operations other than "
							"/'RULE TO RUN /' during /'REMOVE/'");
					return;
				}
			}

				break;

			case CONTROL_ALREADY_PROCESS: {

				syslog(LOG_DEBUG,
						"[DM]: Dm_requestparse from valid client, but control already in progress");
				if ((strcmp(incomingCommand.c_str(), "REMOVE") == 0)
						|| (strcmp(incomingCommand.c_str(), "EXCLUSION") == 0)
						|| (strcmp(incomingCommand.c_str(), "FORCE_REMOVE") == 0)) {
					requestfailurefromclient(incomingClient,
							incomingCommand.c_str(), notif_id, message_id,
							(char *) "Control in progress",
							SCENE_ALREADY_PROCESSING);
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse not to allow ther operations other than"
							" /'REMOVE,EXCLUSION & FORCE_REMOVE /' during /'CONTROL/' ");
					return;
				} else {
					requestfailurefromclient(incomingClient,
							incomingCommand.c_str(), notif_id, message_id,
							("Control in progress, so queuing the request "
									+ std::string(incomingCommand.c_str())).c_str(),
							DM_SUCCESS);
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse to allow /'REMOVE,EXCLUSION & FORCE_REMOVE /' during /'CONTROL/' ");
				}
			}
				break;

			case SCENE_ALREADY_PROCESS: {

				syslog(LOG_DEBUG,
						"[DM]: Dm_requestparse from valid client, but scene already in progress");

//				int Operation = ConvertStringToInt(
//						std::string(incomingCommand.c_str()));
//				switch (Operation) {
//				case SCENE: {
//					//incomingClient = json_object_new_string(
//					//std::to_string(SOCKET_SERVICE).c_str());
//					syslog(LOG_CRIT, "[DM]: Requested scene not from VA");
//				}
//					break;
//				case VOICE_SCENE: {
//					incomingClient = json_object_new_string(
//							std::to_string(VOICE_ASSISTANCE).c_str());
//				}
//					break;
//				}

				if ((strcmp(incomingCommand.c_str(), "RULE") == 0)
						&& (strcmp(json_object_get_string(verb), "RUN") == 0)) {
					requestfailurefromclient(incomingClient,
							incomingCommand.c_str(), notif_id, message_id,
							(char *) "Scene in progress, so queuing request",
							DM_SUCCESS);
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse to allow /'RULE TO RUN /' during /'SCENE/' ");
				} else if (strcmp(incomingCommand.c_str(), "CONTROL") == 0) {
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse to allow /'CONTROL /' during /'SCENE/' ");
					requestfailurefromclient(incomingClient,
							incomingCommand.c_str(), notif_id, message_id,
							(char *) "Scene in progress, so queuing request",
							DM_SUCCESS);
				} else {
					requestfailurefromclient(incomingClient,
							incomingCommand.c_str(), notif_id, message_id,
							(char *) "Scene in progress",
							SCENE_ALREADY_PROCESSING);
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse not to allow ther operations other than"
							" /'RULE TO RUN & CONTROL /' during /'SCENE/' ");
					return;
				}
			}
				break;

			case RULE_ALREADY_PROCESS: {

				syslog(LOG_DEBUG,
						"[DM]: Dm_requestparse from valid client, but rule already in progress");
				if ((strcmp(incomingCommand.c_str(), "RULE") == 0)
						&& (strcmp(json_object_get_string(verb), "RUN") == 0)) {
					requestfailurefromclient(incomingClient,
							incomingCommand.c_str(), notif_id, message_id,
							(char *) "Rule in progress,so queuing request",
							DM_SUCCESS);
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse to allow /'RULE TO RUN /' during /'RULE/' ");
				} else if (strcmp(incomingCommand.c_str(), "CONTROL") == 0) {
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse to allow /'CONTROL /' during /'RULE/' ");
					requestfailurefromclient(incomingClient,
							incomingCommand.c_str(), notif_id, message_id,
							(char *) "Rule in progress,so queuing request",
							DM_SUCCESS);
				}
				/*else if ((strcmp(incomingCommand.c_str(), "SCENE")
				 && (strcmp(json_object_get_string(verb), "RUN") == 0))
				 == 0) {
				 syslog(LOG_DEBUG,
				 "[DM]: Request for SCENE,But rule already in process, so Adding to Queue");

				 requestfailurefromclient(client, command, notif_id,
				 message_id,
				 "Rule in process,so queuing scene request",
				 DM_SUCCESS);
				 }
				 */
				else {
					requestfailurefromclient(incomingClient,
							incomingCommand.c_str(), notif_id, message_id,
							(char *) "Rule in progress",
							SCENE_ALREADY_PROCESSING);
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse not to allow ther operations other than"
							" /'RULE TO RUN & CONTROL /' during /'RULE/' ");
					return;
				}
			}
				break;

			case REQUEST_QUEUE_SIZE_OVERFLOW: {

				syslog(LOG_DEBUG,
						"[DM]: Dm_requestparse request queue size reached to maximum limit of /'64/ '");
				requestfailurefromclient(incomingClient,
						incomingCommand.c_str(), notif_id, message_id,
						(char *) "Maximum limit reached",
						SCENE_ALREADY_PROCESSING);
				return;

			}

				break;
			case FOTA_IN_PROGRESS: { /*Handling other requests when end device fota update is in progress*/

				syslog(LOG_DEBUG,
								"[DM]: Dm_requestparse from valid client, but fota already in progress");
				if ((strcmp(incomingCommand.c_str(), "CONTROL") == 0)) {
					std::string ThingID = std::string(
							json_object_get_string(
									json_object_object_get(
											json_object_array_get_idx(
													json_object_object_get(
															reqJson, "things"),
													0), "thing_id")));
					if (ThingID != FotaThingID) { /*Handle for control operations during FOTA*/
						requestfailurefromclient(incomingClient,
								incomingCommand.c_str(), notif_id, message_id,
								("Fota in progress, so queuing the request"
										+ std::string(incomingCommand.c_str())).c_str(),
								DM_SUCCESS);
						syslog(LOG_DEBUG,
								"[DM]: Dm_requestparse to allow /'CONTROL /' during /'FOTA/' ");
					} else {
						requestfailurefromclient(incomingClient,
								incomingCommand.c_str(), notif_id, message_id,
								(char *) "Fota in progress",
								SCENE_ALREADY_PROCESSING);
						return;
					}
				} else if ((strcmp(incomingCommand.c_str(), "RULE") == 0)
						&& (strcmp(json_object_get_string(verb), "RUN") == 0)) { /*Handle for rule operations during FOTA*/

					requestfailurefromclient(incomingClient,
							incomingCommand.c_str(), notif_id, message_id,
							(char *) "FOTA in progress, so queuing the request",
							DM_SUCCESS);
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse to allow /'RULE TO RUN /' during /'FOTA/' ");
				} else {
					requestfailurefromclient(incomingClient,
							incomingCommand.c_str(), notif_id, message_id,
							(char *) "Fota in progress",
							SCENE_ALREADY_PROCESSING);
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse not to allow ther operations other than"
							" /'RULE TO RUN & CONTROL /' during /'FOTA/' ");
					return;
				}

			}
				break;

			default: {

				if (QueueSizeLimit) {
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse request queue size reached to maximum limit of /'64/ '");
					requestfailurefromclient(incomingClient,
							incomingCommand.c_str(), notif_id, message_id,
							(char *) "Maximum limit reached",
							SCENE_ALREADY_PROCESSING);
				} else {
					syslog(LOG_CRIT, "[DM]: Dm_requestparse request is /'INVALID/'");
					requestfailurefromclient(incomingClient,
							incomingCommand.c_str(), notif_id, message_id,
							(char *) "Request is invalid",
							DM_FAIL);
				}
				return;
			}
				break;
			}

		}
			break;

		case DM_SERVICE: {
			syslog(LOG_DEBUG,
					"[DM]: Dm_requestparse from Valid Client for self trigger: [%d] Command :[%s]",
					incomingClient, incomingCommand.c_str());
			Message_ID_Client = std::to_string(DM_SERVICE) + std::string("00")
					+ std::to_string(rand() % 900);
			json_object_object_add(reqJson, "message_id",
					json_object_new_string(Message_ID_Client.c_str()));
			message_id = json_object_object_get(reqJson, "message_id");
		}
			break;

		case FOTA_SERVICE: {

			int Request_Status; // Variable to grab the on going request
			Request_Status = ((int) FlagDiscSend << 1)
					| ((int) ExclusionFlag << 2)
					| ((int) DmSceneMapping::DMGetSceneMapExecuteflag() << 3
							| (int) ((bool) RuleStatusFlag) << 4
							| ((int) RemoveFlag << 5) | ((int) ControlFlag << 6)
							| ((int) QueueSizeLimit << 7)
							| ((int) FlagFota << 8)); /*check for the ongoing operation in DM to perform end device firmware update */
			if ((Request_Status != REQUEST_SUCCESS)) {
				json_object * fota_response = json_object_new_object();
				json_object_object_add(fota_response, "client",
						json_object_new_int(14));
				json_object_object_add(fota_response, "command",
						json_object_new_string("FOTA_RES"));
				json_object_object_add(fota_response, "thing_id",
						json_object_object_get(reqJson, "thing_id"));

				json_object_object_add(fota_response, "status",
						json_object_new_int(104));

				json_object_object_add(fota_response, "version",
						json_object_new_string("0"));

				syslog(LOG_DEBUG, "[DM]: Dm_requestparse FOTA -> DM response object: [%s]",
						json_object_to_json_string(fota_response));
				int res_fota = sendto(sockfd,
						json_object_to_json_string(fota_response),
						strlen(json_object_to_json_string(fota_response)),
						MSG_CONFIRM, (const struct sockaddr *) &fota_servaddr,
						sizeof(fota_servaddr));
				syslog(LOG_DEBUG,
						"[DM]:Dm_requestparse  FOTA -> DM response object status:[%d]",
						res_fota);
				system("rm -rf /exe_dir/ota-files/*");
				json_object_put(fota_response);
				return;
			}
//			syslog(LOG_DEBUG, "[DM]: Dm_requestparseFOTA_SERVICE Request");
			Message_ID_Client = json_object_get_string(client)
					+ std::string("00") + std::to_string(rand() % 900); /* Creating a message ID to execute fota request*/
			json_object_object_add(reqJson, "message_id",
					json_object_new_string(Message_ID_Client.c_str()));
			message_id = json_object_object_get(reqJson, "message_id");
		}
			break;

		default:  //!if Client is unknown

			syslog(LOG_DEBUG, "[DM]: Dm_requestparse  from Invalid Client");
			requestfailurefromclient(incomingClient, incomingCommand.c_str(),
					notif_id, message_id,
					(char *) "Request from invalid client",
					DM_FAIL);
			return;

			break;
		}

//--------------------------------------------------------------------------------------------------
		/*Rule is allowed during discovery, rule run command will be added in Queue. 
		 /Once Discovery completes rule will be executed*/

		DmThingMap* DmMsgNotifyMapObj = DmThingMap::GetInstance();
		int Operation = ConvertStringToInt(
				std::string(incomingCommand.c_str()));
		switch (Operation) {
		/**
		 * case for Arm_DisArm feature
		 */
		case SECURITY: {
			syslog(LOG_DEBUG, "[DM] : Dm_requestparse security [%s]",
					json_object_get_string(reqJson));
			json_object* security_data = json_object_object_get(reqJson,
					"payload");
			json_object* verb = json_object_object_get(reqJson, "verb");
			json_object* sub_domain = json_object_object_get(security_data,
					"subdomain");
			std::string message;
			bool status = true;
			if (strcmp(json_object_get_string(verb), "UPDATE") == 0) {
				json_object* payload = json_object_object_get(security_data,
						"payload");
				if (strcmp(json_object_get_string(sub_domain), "configuration")
						== 0) {
					/**
					 * case for Arm_DisArm feature::kill_switch configuration  //dm_kill_switch=(thing_id+entitiy_id)
					 */
					json_object* temp_kill_switch = json_object_object_get(
							payload, "kill_switch");

					if (temp_kill_switch != NULL) {
						json_object* thing_id = json_object_object_get(
								temp_kill_switch, "thing_id");
						json_object* entity_id = json_object_object_get(
								temp_kill_switch, "entity_id");
						if (thing_id != NULL && entity_id != NULL) {
							std::string temp_id = std::string(
									json_object_get_string(thing_id))
									+ std::string(
											json_object_get_string(entity_id));
							if (DM_KILL_SWITCH != temp_id) {

								strcpy(DM_KILL_SWITCH, temp_id.c_str());
								set_shifu_uci_param((char *) "DM_KILL_SWITCH",
										DM_KILL_SWITCH);

							}
							message = "Kill Switch configuration successful";

						} else {
							syslog(LOG_DEBUG,
									"[DM] :Dm_requestparse  kill switch data not provided");
							message = "Invalid Data";
						}
					} else {
						syslog(LOG_DEBUG,
								"[DM] :Dm_requestparse kill switch data not provided");
						message = "Invalid Data";
					}
				} else if (strcmp(json_object_get_string(sub_domain),
						"multiple") == 0) {
					/**
					 * case for Arm_DisArm feature::Arming requested devices & disarming all other sensors in thing map and database.
					 */
					if (strlen(DM_KILL_SWITCH) > 0) {
						/**
						 * case for Arm_DisArm feature::only allows if kill_switch is configured.
						 */
						json_object* thingsData = json_object_object_get(
								payload, "arm_devices");
						if (thingsData != NULL) {
							if (json_object_get_type(thingsData)
									== json_type_array) {
								DmThingsBackupDb* DmThingsBackupDbOBJ =
										DmThingsBackupDb::GetInstance();
								int security, mode;
								std::stringstream device_ids;
								if (json_object_array_length(thingsData) > 0) {

									security = 3;
									mode = 1;  //arm-all/arm-stay/arm-away mode
									std::string thing_id;

									for (size_t thingNo = 0;
											thingNo
													< json_object_array_length(
															thingsData);
											thingNo++) {

										if (thingNo != 0)
											device_ids << ",";
										thing_id =
												json_object_get_string(
														json_object_object_get(
																json_object_array_get_idx(
																		thingsData,
																		thingNo),
																"thing_id"));

										json_object* entities =
												json_object_object_get(
														json_object_array_get_idx(
																thingsData,
																thingNo),
														"entity_ids");
										if (json_object_get_type(entities)
												== json_type_array
												&& json_object_array_length(
														entities) > 0) {
											for (size_t entityNo = 0;
													entityNo
															< json_object_array_length(
																	entities);
													entityNo++) {

												if (entityNo != 0)
													device_ids << ",";

												device_ids << "'" << thing_id
														<< json_object_get_string(
																json_object_array_get_idx(
																		entities,
																		entityNo))
														<< "'";
											}

										} else {
											status = false;
											message = "Invalid Data";
											break;
										}
										if (status == false)
											break;
									}
								} else {
									security = 2;
									mode = 2;  //dis-arm all mode
									device_ids << "DM_DEFAULT";

								}
								if (status) {
									/**
									 * case for Arm_DisArm feature::updating security level in database and thing map based on request.
									 */
									bool modify =
											DmThingsBackupDbOBJ->UpdateEntityTable(
													device_ids.str(), security,
													mode);
									if (modify)
										message = "Update Successful";
									else
										message = "Update Failed";
								}

							} else {
								//bad data request
								message = "Invalid Data";
							}
						} else {
							//bad data request
							message = "Invalid Data";
						}
					} else {
						//kill switch not configured or removed
						message = "Kill switch not configured";
					}

				} else if (strcmp(json_object_get_string(sub_domain), "single")
						== 0) {
					/**
					 * case for Arm_DisArm feature::changing requested devices security level based on requested arm_status.
					 */
					if (strlen(DM_KILL_SWITCH) > 0) {
						/**
						 * case for Arm_DisArm feature::only allows if kill_switch is configured.
						 */
						json_object* thingsData = json_object_object_get(
								payload, "arm_devices");
						if (thingsData != NULL) {
							if (json_object_get_type(thingsData)
									== json_type_array) {

								DmThingsBackupDb* DmThingsBackupDbOBJ =
										DmThingsBackupDb::GetInstance();
								int security = 3, mode = 3;  //arm single area
								std::stringstream device_ids;
								std::string thing_id;
								for (size_t thingNo = 0;
										thingNo
												< json_object_array_length(
														thingsData);
										thingNo++) {

									if (thingNo != 0)
										device_ids << ",";

									thing_id = json_object_get_string(
											json_object_object_get(
													json_object_array_get_idx(
															thingsData,
															thingNo),
													"thing_id"));
									json_object* entities =
											json_object_object_get(
													json_object_array_get_idx(
															thingsData,
															thingNo),
													"entity_ids");
									if (json_object_get_type(entities)
											== json_type_array
											&& json_object_array_length(
													entities) > 0) {
										for (size_t entityNo = 0;
												entityNo
														< json_object_array_length(
																entities);
												entityNo++) {
											if (entityNo != 0)
												device_ids << ",";
											device_ids << "'" << thing_id
													<< json_object_get_string(
															json_object_array_get_idx(
																	entities,
																	entityNo))
													<< "'";
										}
									} else {
										//Bad data
										message = "Invalid Data";
										status = false;
										break;
									}
									if (status == false)
										break;

								}
								if (status) {
									security = std::stoi(
											json_object_get_string(
													json_object_object_get(
															payload,
															"arm_status")));
									/**
									 * case for Arm_DisArm feature::updating security level in database and thing map based on request.
									 */
									bool modify =
											DmThingsBackupDbOBJ->UpdateEntityTable(
													device_ids.str(), security,
													mode);
									if (modify)
										message = "Updated Successful";
									else {
										message = "Update Failed";
									}
								}

							} else {
								//Bad data
								message = "Invalid Data";
							}

						} else {
							//Bad data
							message = "Invalid Data";

						}
					} else {
						//kill switch not configured or removed
						message = "Kill switch not configured";
					}

				} else {
					//Bad Request
					message = "Invalid Data";

				}
			} else {
				//Bad Request
				message = "Invalid Data";
			}

			json_object *client_data = json_object_new_object();
			json_object_object_add(client_data, "verb",
					json_object_new_string("UPDATE"));
			json_object* Default_client = json_object_new_string(
					(char *) std::to_string(
					SOCKET_SERVICE).c_str());
			AddDMSendResponse(Default_client, notif_id, message_id, message,
					command, DM_SUCCESS, client_data);
			json_object_put(client_data);
			json_object_put(Default_client);
		}
			break;

		case DISCOVERY: {

			//			syslog(LOG_INFO,
			//				"[DM]: Request queue process request discovery ");

			json_object* Default_pass_phrase = json_object_new_string(
					"DM_DEFAULT");
			json_object* Default_thing_id = json_object_new_string(
					"DM_DEFAULT");
			MakeDMReqQueueAddMessage(client, notif_id, message_id,
					Default_thing_id, command, Default_pass_phrase);
			json_object_put(Default_pass_phrase);
			json_object_put(Default_thing_id);

		}
			break;

		case FOTA_REQ: { /* performing the FOTA operation */

			json_object* Default_pass_phrase = json_object_new_string(
					"DM_DEFAULT");
			json_object* Default_thing_id = json_object_object_get(reqJson,
					"thing_id");
			FotaThingID = json_object_get_string(Default_thing_id); /* Stopping incoming control requests for this thing_Id during FOTA */
			FotaMsgID = json_object_get_string(
					json_object_object_get(reqJson, "message_id")); /* To stop FOTA-thread incase of error-response*/

			json_object* firmware_path = json_object_object_get(reqJson,
					"path"); /* Firmware file path given by FOTA module */
			syslog(LOG_DEBUG, "[DM]: FOTA -> Firmware path given is: [%s]",
					json_object_get_string(firmware_path));

			json_object* firmware_version = json_object_object_get(reqJson,
					"version"); /* Firmware version given by FOTA module */

			MakeDMReqQueueAddMessage(client, notif_id, message_id,
					Default_thing_id, command, Default_pass_phrase,
					firmware_path, firmware_version); /*Adding message into process queue vector*/
			json_object_put(Default_pass_phrase);
		}
			break;

		case CONTROL: {

			//		syslog(LOG_INFO,
			//				"[DM]: Request queue process request control ");
			if ((JsonDataCheck(reqJson) == true)) { /*Fix for 20583*/
				json_object* things = json_object_object_get(reqJson, "things");
				json_object* dummy_request_array = json_object_new_array(); /* Fix for 19764 */
				//----------------------------------

				//----------------------------------------------------------
				//checking for kill-switch triggered or not

				if (json_object_array_length(things) == 1) {

					std::string custom_id = std::string(
							json_object_get_string(
									json_object_object_get(
											json_object_array_get_idx(things,
													0), "thing_id")))
							+ std::string(
									json_object_get_string(
											json_object_object_get(
													json_object_array_get_idx(
															things, 0),
													"entity_id")));

					std::string prop_type = json_object_get_string(
							json_object_object_get(
									json_object_array_get_idx(things, 0),
									"property_type"));

					if (DM_KILL_SWITCH == custom_id
							&& "KILL_SWITCH" == prop_type) {
						//kill switch triggered so stopping buzzer and discarding control request
						json_object *client_data = json_object_new_object();
						json_object_object_add(client_data, "verb",
								json_object_new_string("KILL_SWITCH"));
						Audio_pause();
						system("/etc/init.d/va_service start");
						set_shifu_uci_param((char *) "DM_VA_START_CHECK", (char *)"1");
						va_on = 1; /* Updating the flag*/
						syslog(LOG_DEBUG, "[DM]: VA_ON in REQUEST_PARSE %d",
								va_on);
						set_volume(volume_present);
						syslog(LOG_DEBUG, "[DM] :Volume is set after pause %d",
								volume_present);
						pthread_mutex_lock(&ArmStatusLock);
						ArmStatusFlag = 0;
						pthread_mutex_unlock(&ArmStatusLock);
						AddDMSendResponse(client, notif_id, message_id,
								(char *) "KILL_SWITCH_TRIGGERED SUCCESSFULLY",
								command, DM_SUCCESS, client_data);
						json_object_put(client_data);
						break;
					}
				}

				//----------------------------------------------------------
				for (size_t messageSize = 0;
						messageSize < json_object_array_length(things);
						messageSize++) {

					json_object* ThingData1 = json_tokener_parse(
							json_object_get_string(
									json_object_array_get_idx(things,
											messageSize)));

//check the request for color control

					if (incomingClient == SOCKET_SERVICE) {

						json_object* property_name = json_object_object_get(
								ThingData1, "property_name");

						if (strcmp(json_object_get_string(property_name),
								"Color") == 0) {

							json_object* PropertY2 = json_object_new_object();

							json_object_object_add(PropertY2, "thing_id",
									json_object_object_get(ThingData1,
											"thing_id"));

							json_object_object_add(PropertY2, "entity_id",
									json_object_object_get(ThingData1,
											"entity_id"));

							json_object_object_add(PropertY2, "property_type",
									json_object_new_string("SWITCH"));

							json_object_object_add(PropertY2, "property_name",
									json_object_new_string("OnOff"));

							json_object_object_add(PropertY2, "property_value",
									json_object_new_string("1"));

							json_object_array_add(dummy_request_array,
									PropertY2);
							json_object_array_add(dummy_request_array,
									ThingData1);

						} else {
							dummy_request_array = things;
						}
					} else { /* This else case captures the input from all the ZMQ clients like WS and MA other than VA*/
						dummy_request_array = things;
					}
				}

				things = dummy_request_array;

//----------------------------------

				for (size_t messageSize = 0;
						messageSize < json_object_array_length(things);
						messageSize++) {

					if (json_object_array_length(things) == 1) {

						json_object* ThingData = json_tokener_parse(
								json_object_get_string(
										json_object_array_get_idx(things,
												messageSize)));

						json_object_object_add(ThingData, "client", client);
						json_object_object_add(ThingData, "notif_id", notif_id);
						json_object_object_add(ThingData, "command", command);
						json_object_object_add(ThingData, "message_id",
								message_id);

						//DM ADDED
						json_object_object_add(ThingData, "property_range",
								json_object_new_string(
										"PROPERTY_RANGE_DM_DEFAULT"));

						/*							syslog(LOG_INFO,
						 "[DM]: Platform control adding into queue: [%s]",
						 json_object_get_string(ThingData));*/

						DmMsgNotifyMapObj->DmMsgNotifyMap[json_object_get_string(
								message_id)] = json_object_get_string(notif_id);

						for (std::map<std::string, std::string>::iterator itr =
								DmMsgNotifyMapObj->DmMsgNotifyMap.begin();
								itr != DmMsgNotifyMapObj->DmMsgNotifyMap.end();
								++itr) {
							syslog(LOG_DEBUG,
									"[DM]: The notify MAP has Message_Id: [%s], and Notification_Id: [%s]",
									itr->first.c_str(), itr->second.c_str());
						}

						Thing_Check_AddingQueue(ThingData,
								json_object_array_length(things),
								PRIORITY_CONTROL);

					} else if (json_object_array_length(things) > 1) {

						json_object* ThingData = json_tokener_parse(
								json_object_get_string(
										json_object_array_get_idx(things,
												messageSize)));

						message_id =
								json_object_new_string(
										(std::to_string(incomingClient).insert(
												2,
												DM_AREA)
												+ std::to_string(rand() % 900)).c_str());

						json_object_object_add(ThingData, "client", client);
						json_object_object_add(ThingData, "notif_id", notif_id);
						json_object_object_add(ThingData, "command", command);
						json_object_object_add(ThingData, "message_id",
								message_id);
						json_object_object_add(ThingData, "property_range",
								json_object_new_string(
										"PROPERTY_RANGE_DM_DEFAULT"));

						/*syslog(LOG_INFO,
						 "[DM]: Platform control area adding into queue thing_id : [%s] message_id: [%s]",
						 json_object_get_string(ThingData),
						 json_object_get_string(message_id));*/

						Thing_Check_AddingQueue(ThingData,
								json_object_array_length(things),
								PRIORITY_CONTROL);

					}
				}
				json_object_put(dummy_request_array);
			} else {
				syslog(LOG_CRIT, "[DM]: Data from  Clent is not Valid");
			}
		}
			break;

			/** Added Extra case for Scene request is initiated from Voice */
			//case VOICE_SCENE:
		case SCENE: {
//
//			//		syslog(LOG_INFO, "[DM]: Request queue process request scene ");
//
//			switch (incomingClient) {
//
//			case SOCKET_SERVICE: {
//
//				/*		syslog(LOG_INFO,
//				 "[DM]: Request queue process request scene socket_service start");*/
//
//				json_object* verb = json_object_object_get(reqJson, "verb");
//
//				if (strcmp(json_object_get_string(verb), "MODIFY") == 0) {
//
//					/*			syslog(LOG_INFO,
//					 "[DM]: Request queue process request scene socket_service modify start");*/
//
//					json_object* scene_data = json_object_object_get(reqJson,
//							"scene_data");
//
//					bool SceneModifytatus = DmSceneMapping::DMCallSceneModify(
//							scene_data);
//
//					syslog(LOG_INFO,
//							"[DM]: Request queue process request scene socket_service modify status: [%d]",
//							SceneModifytatus);
//
//					/*	syslog(LOG_INFO,
//					 "[DM]: Request queue process request scene socket_service modify end");*/
//
//				} else if (strcmp(json_object_get_string(verb), "RUN") == 0) {
//
//					/*		syslog(LOG_INFO,
//					 "[DM]: Request queue process request scene socket_service run start");*/
//
//					json_object* scene_data = json_object_object_get(reqJson,
//							"scene_data");
//
//					/*			syslog(LOG_INFO,
//					 "[DM]: Request queue process calling dm call scene execute start");*/
//
//					SecneRuleExecution RuleExecutionRsultOBJ;
//					DmSceneMapping DmSceneMappingObj;
//
//					if (DmSceneMapping::DMGetSceneMapExecuteflag() == false) {
//
//						RuleExecutionRsultOBJ =
//								DmSceneMapping::DMCallSceneExecute(scene_data);
//
//						syslog(LOG_DEBUG,
//								"[DM]: Request queue process Scene ID : [%s] SCENEEXECUTE STATUS: [%d]",
//								RuleExecutionRsultOBJ.RuleSceneID.c_str(),
//								RuleExecutionRsultOBJ.RuleSceneExecuteFlag);
//
//						/*			syslog(LOG_INFO,
//						 "[DM]: Platform scene iot_acton: [%s]",
//						 json_object_get_string(
//						 RuleExecutionRsultOBJ.iot_action));*/
//
//						/*		syslog(LOG_INFO,
//						 "[DM]: Platform scene non_iot_acton: [%s]",
//						 json_object_get_string(
//						 RuleExecutionRsultOBJ.non_iot_action));*/
//						syslog(LOG_INFO, "[DM]:Scene ID : [%s]",
//								RuleExecutionRsultOBJ.RuleSceneID.c_str());
//
//						syslog(LOG_INFO,
//								"[DM]: Platform scene executing for 2: [%s]",
//								RuleExecutionRsultOBJ.RuleSceneID.c_str());
//
//						DMSceneRuleParseVA(incomingClient,
//								incomingCommand.c_str(),
//								RuleExecutionRsultOBJ.RuleSceneID,
//								json_object_get_string(message_id),
//								RuleExecutionRsultOBJ.iot_action);
//
//						syslog(LOG_INFO,
//								"[DM]: Request queue process REQUEST SCENE SOCKET_SERVICE RUN END");
//
////						switch (RuleExecutionRsultOBJ.Sceneflag) {
////
////						case 1: {
////
////							syslog(LOG_INFO,
////									"[DM]: Platform scene executing for 1: [%s]",
////									RuleExecutionRsultOBJ.RuleSceneID.c_str());
////
////							json_object* Default_Client =
////									json_object_new_string(
////											(char *) std::to_string(
////											CRON_HANDLER).c_str());
////							json_object* Default_Notif_ID =
////									json_object_new_string(
////											(char *) RuleExecutionRsultOBJ.RuleSceneID.c_str());
////
////							AddDMSendResponse(Default_Client, notif_id,
////									message_id,
////									"SENDING RULE EXECUTE REQUEST SUCCESSFULLY",
////									command, DM_SUCCESS,
////									RuleExecutionRsultOBJ.non_iot_action);
////
////							//	syslog(LOG_INFO,
////							//			"[DM]: Request queue process REQUEST SCENE SOCKET_SERVICE RUN END");
////						}
////							break;
////
////						case 2: {
////
////							//DmSceneMapping::SceneStatusFlag = 4;
////
////							syslog(LOG_INFO,
////									"[DM]: Platform scene executing for 2: [%s]",
////									RuleExecutionRsultOBJ.RuleSceneID.c_str());
////
////							DMSceneRuleParseVA(json_object_get_int(client),
////									json_object_get_string(command),
////									RuleExecutionRsultOBJ.RuleSceneID,
////									json_object_get_string(message_id),
////									RuleExecutionRsultOBJ.iot_action);
////
////							syslog(LOG_INFO,
////									"[DM]: Request queue process REQUEST SCENE SOCKET_SERVICE RUN END");
////
////						}
////							break;
////
////						case 3: {
////
////							//DmSceneMapping::SceneStatusFlag = 4;
////
////							syslog(LOG_INFO,
////									"[DM]: Platform scene executing for 3: [%s]",
////									RuleExecutionRsultOBJ.RuleSceneID.c_str());
////
////							DMSceneRuleParseVA(json_object_get_int(client),
////									json_object_get_string(command),
////									RuleExecutionRsultOBJ.RuleSceneID,
////									json_object_get_string(message_id),
////									RuleExecutionRsultOBJ.iot_action);
////
////							json_object* Default_Client =
////									json_object_new_string(
////											(char *) std::to_string(
////											CRON_HANDLER).c_str());
////							json_object* Default_Notif_ID =
////									json_object_new_string(
////											(char *) RuleExecutionRsultOBJ.RuleSceneID.c_str());
////
////							AddDMSendResponse(Default_Client, notif_id,
////									message_id,
////									"SENDING RULE EXECUTE REQUEST SUCCESSFULLY",
////									command, DM_SUCCESS,
////									RuleExecutionRsultOBJ.non_iot_action);
////
////							//		syslog(LOG_INFO,
////							//				"[DM]: Request queue process REQUEST SCENE SOCKET_SERVICE RUN END");
////
////						}
////							break;
////
////						default: {
////							syslog(LOG_CRIT,
////									"[DM]: Request queue process Wrong Scene Data coming from tronX");
////
////							syslog(LOG_INFO,
////									"[DM]: Platform scene executing for default: [%s]",
////									RuleExecutionRsultOBJ.RuleSceneID.c_str());
////
////							for (size_t messageSize = 0;
////									messageSize
////											< json_object_array_length(
////													scene_data);
////									messageSize++) {
////
////								/*	syslog(LOG_DEBUG,
////								 "[DM]: Request queue process scene run scene_data: [%s]",
////								 json_object_get_string(
////								 json_object_array_get_idx(
////								 scene_data,
////								 messageSize)));*/
////
////								json_object* scene_id = json_object_object_get(
////										json_object_array_get_idx(scene_data,
////												messageSize), "scene_id");
////
////								json_object* Default_Notif_ID = scene_id;//json_object_new_string((char *)std::to_string(json_object_get_int(scene_id)));
////								json_object* Default_command =
////										json_object_new_string("SCENE");
////								json_object* Default_RulesceneData =
////										json_object_new_string("SCENE");
////
////								AddDMSendResponse(client, notif_id, message_id,
////										"SCENE IS ALREADY PROCESSING", command,
////										DM_FAIL, Default_RulesceneData);
////
////							}
////
////						}
////							break;
////						}
//
//					} else {
//
//						syslog(LOG_INFO,
//								"[DM]: Platform scene executing for default: [%s]",
//								RuleExecutionRsultOBJ.RuleSceneID.c_str());
//
//						for (size_t messageSize = 0;
//								messageSize
//										< json_object_array_length(scene_data);
//								messageSize++) {
//
//							syslog(LOG_DEBUG,
//									"[DM]: Request queue process scene run scene_data: [%s]",
//									json_object_get_string(
//											json_object_array_get_idx(
//													scene_data, messageSize)));
//
//						/*	json_object* scene_id = json_object_object_get(
//									json_object_array_get_idx(scene_data,
//											messageSize), "scene_id");*/
//
//						/*	json_object* Default_Notif_ID = scene_id;//json_object_new_string((char *)std::to_string(json_object_get_int(scene_id)));
//							json_object* Default_command =
//									json_object_new_string("SCENE");*/
//							json_object* Default_RulesceneData =
//									json_object_new_string("SCENE");
//
//							AddDMSendResponse(client, notif_id, message_id,
//									"Scene already in progress", command,
//									DM_FAIL, Default_RulesceneData);
//
//							syslog(LOG_INFO,
//									"[DM]: Request queue process REQUEST SCENE SOCKET_SERVICE RUN END");
//						}
//
//					}
//
//				}
//			}
//
//				break;
//
//				// Offline Scene Execution
//			case MOBILE_AGENT: {
//
//				//JSON response received from MA
//				/*{
//				 'client': '13',
//				 'command': 'SCENE',
//				 'verb': 'RUN',
//				 'notif_id': <id>,
//				 'scene_data': [
//				 {scene_id}
//				 ]
//
//				 }*/
//
//				//	syslog(LOG_INFO,
//				//			"[DM]: Request queue process of Scene via Mobile Agent ");
//				json_object* verb = json_object_object_get(reqJson, "verb");
//				json_object* scene_data = json_object_object_get(reqJson,
//						"scene_data");
//
//				for (size_t messageSize = 0;
//						messageSize < json_object_array_length(scene_data);
//						messageSize++) {
//
//					/*	syslog(LOG_DEBUG,
//					 "[DM]: Scene run scene_data in Offline from MA: [%s]",
//					 json_object_get_string(
//					 json_object_array_get_idx(scene_data,
//					 messageSize)));*/
///*
//					json_object* scene_id = json_object_object_get(
//							json_object_array_get_idx(scene_data, messageSize),
//							"scene_id");*/
//
//					if (strcasecmp(json_object_get_string(verb), "RUN") == 0) {
//
//						/*syslog(LOG_INFO,
//						 "[DM]: Request queue process of Scene via Mobile Agent execution start");*/
//
//						SecneRuleExecution RuleExecutionRsultOBJ;
//						DmSceneMapping DmSceneMappingObj;
//
//						// Scene execution Success case
////
//						RuleExecutionRsultOBJ =
//								DmSceneMapping::DMCallSceneExecute(scene_data);
//
//						/*syslog(LOG_INFO,
//						 "[DM]: Platform scene executing for MA: [%s]",
//						 RuleExecutionRsultOBJ.RuleSceneID.c_str());*/
//
//						DMSceneRuleParseVA(incomingClient,
//								incomingCommand.c_str(),
//								RuleExecutionRsultOBJ.RuleSceneID,
//								json_object_get_string(message_id),
//								RuleExecutionRsultOBJ.iot_action);
//
//					/*	json_object* Default_Notif_ID =
//								json_object_new_string(
//										(char *) RuleExecutionRsultOBJ.RuleSceneID.c_str());
//						json_object* Default_RulesceneData =
//								json_object_new_string("DM_DEFAULT"); //we need confirm once*/
//
//						/*AddDMSendResponse(client, notif_id,
//						 message_id,
//						 "Scene Initiated Successfully",
//						 command, DM_SUCCESS,
//						 Default_RulesceneData);*/
//
//						/*syslog(LOG_INFO,
//						 "[DM]: Request queue process REQUEST SCENE SOCKET_SERVICE RUN END");
//
//						 syslog(LOG_INFO, "[DM]: Request queue process of Scene via Mobile Agent execution End");*/
//
//					}
//				}
//
//			}
//				break;
//
//			default: {
//
//				syslog(LOG_INFO,
//						"[DM]: Request queue process request scene default case");
//			}
//				break;
//			}

		}
			break;

		case RULE: {

			json_object* rules_data = json_object_object_get(reqJson,
					"rules_data");
			json_object* verb = json_object_object_get(reqJson, "verb");

			if (strcmp(json_object_get_string(verb), "CREATE") == 0) {

				int rule_type;
				int timeflag = 0;

				for (size_t messageSize = 0;
						messageSize < json_object_array_length(rules_data);
						messageSize++) {

//--------------------------------------------TIME BASED RULE START------------------------------------------------//
//					json_object* rule_id = json_object_object_get(
//							json_object_array_get_idx(rules_data, messageSize),
//							"rule_id");

					json_object* sensor_condition = json_object_object_get(
							json_object_array_get_idx(rules_data, messageSize),
							"sensor_condition");
					json_object* time_condition = json_object_object_get(
							json_object_array_get_idx(rules_data, messageSize),
							"time_condition");
					json_object* time_end = json_object_object_get(
							json_object_array_get_idx(rules_data, messageSize),
							"time_end");
					json_object* time_start = json_object_object_get(
							json_object_array_get_idx(rules_data, messageSize),
							"time_start");
					if (strcmp(json_object_get_string(sensor_condition), "none")
							!= 0) {
						if (strcmp(json_object_get_string(time_condition),
								"none") != 0) {
							if (strcmp(json_object_get_string(time_end), "none")
									!= 0) {
								rule_type =
								DM_SENSOR_TIME_BASED_RULE_END;
								syslog(LOG_DEBUG,
										"[DM]: Dm_requestparse found dm_sensor_time_based_rule_end");
							} else {
								rule_type =
								DM_SENSOR_TIME_BASED_RULE_START;
							}
						} else {
							rule_type = DM_SENSOR_BASED_RULE;
						}
					} else {
						rule_type = DM_TIME_BASED_RULE;
					}

					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse has rule_type: [%d]",
							rule_type);

					switch (rule_type) {

					case DM_TIME_BASED_RULE:

					case DM_SENSOR_TIME_BASED_RULE_START: {

						/*syslog(LOG_DEBUG,
						 "[DM]: Request queue process rule create for dm_sensor_time_based_rule_start");*/

						json_object* rule_id = json_object_object_get(
								json_object_array_get_idx(rules_data,
										messageSize), "rule_id");

						json_object* time_start = json_object_object_get(
								json_object_array_get_idx(rules_data,
										messageSize), "time_start");

						std::string time_start_voice = std::string(
								json_object_get_string(time_start));

						std::string Hr_start = time_start_voice.substr(0, 2);
						std::string Min_start = time_start_voice.substr(3, 2);

						std::stringstream time_start_str;

						time_start_str << Min_start << " " << Hr_start << " "
								<< "* " << "* " << "*";
						std::string cron_pattern_start = time_start_str.str();

						json_object *client_data = json_object_new_object();
						json_object *actions = json_object_new_array();
						json_object_object_add(client_data, "verb",
								json_object_new_string("CREATE"));
						json_object_object_add(client_data, "cron_pattern",
								json_object_new_string(
										cron_pattern_start.c_str()));
						json_object_object_add(client_data, "rule_id", rule_id);
						json_object_object_add(client_data, "actions", actions);

						json_object* Default_command = json_object_new_string(
						DM_RULE_COLTROL);
						json_object* Default_client = json_object_new_string(
								(char *) std::to_string(
								CRON_HANDLER).c_str());
						json_object* Default_message_ID =
								json_object_new_string("1100");

						AddDMSendResponse(Default_client, rule_id,
								Default_message_ID,
								"Sent request to create Cronjob",
								Default_command, DM_SUCCESS, client_data);
					}
						break;

					case DM_SENSOR_TIME_BASED_RULE_END: {

//						json_object *client_data = json_object_new_object();
//						json_object *actions = json_object_new_array();
						std::string time_start_voice = std::string(
								json_object_get_string(time_start));
						std::string Hr_start = time_start_voice.substr(0, 2);
						std::string Min_start = time_start_voice.substr(3, 2);
						int total_time_start = (std::stoi(Hr_start) * 100)
								+ std::stoi(Min_start);
						std::stringstream time_start_str;
						time_start_str << Min_start << " " << Hr_start << " "
								<< "* " << "* " << "*";
						std::string cron_pattern_start = time_start_str.str();

//						json_object_object_add(client_data, "verb",
//								json_object_new_string("CREATE"));
//						json_object_object_add(client_data, "cron_pattern",
//								json_object_new_string(
//										cron_pattern_start.c_str()));
//						json_object_object_add(client_data, "rule_id", rule_id);
//						json_object_object_add(client_data, "actions", actions);
//						json_object* Default_client = json_object_new_string(
//								(char *) std::to_string(
//								CRON_HANDLER).c_str()); /* creating extra parameter of client,command and message id for sending cron job to Cron Handler*/
//						json_object* Default_command = json_object_new_string(
//						DM_RULE_COLTROL);
//						json_object* Default_message_ID =
//								json_object_new_string("1100");
//
//						//SENDING CRON PATTERNT SET START TO CLIENT
//
//						AddDMSendResponse(Default_client, notif_id, Default_message_ID,
//								"Sent request to start Cronjob", Default_command,
//								DM_SUCCESS, client_data);

//						json_object *client_data_end = json_object_new_object();
//						json_object *actions_end = json_object_new_array();

						std::string time_end_voice = std::string(
								json_object_get_string(time_end));
						std::string Hr_end = time_end_voice.substr(0, 2);
						std::string Min_end = time_end_voice.substr(3, 2);
						int total_time_end = (std::stoi(Hr_end) * 100)
								+ std::stoi(Min_end);
						std::stringstream time_end_str;
						time_end_str << Min_end << " " << Hr_end << " " << "* "
								<< "* " << "*";
						std::string cron_pattern_end = time_end_str.str();
//						json_object_object_add(client_data_end, "verb",
//								json_object_new_string("CREATE"));
//						json_object_object_add(client_data_end, "cron_pattern",
//								json_object_new_string(
//										cron_pattern_end.c_str()));
//						json_object_object_add(client_data_end, "rule_id",
//								rule_id);
//						json_object_object_add(client_data_end, "actions",
//								actions_end);
//
//						//SENDING CRON PATTERN SET END TO CLIENT
//
//						AddDMSendResponse(Default_client, notif_id, Default_message_ID,
//								"Sent request to end Cronjob", Default_command,
//								DM_SUCCESS, client_data_end);

						time_t theTime = time(NULL);
						struct tm *aTime = localtime(&theTime);
						int hour = aTime->tm_hour;
						int min = aTime->tm_min;
						int current_time = (hour * 100) + min;
						//REDMINE 22328 start: Accounting for the first minute in time and sensor based routine
						if (current_time <= total_time_end) {
							if (current_time >= total_time_start) {
								timeflag = 1;
							} else if (total_time_start >= total_time_end) {
								timeflag = 1;
							}
						//REDMINE 22328 end: Accounting for the first minute in time and sensor based routine
						} else if (current_time < total_time_start) {
							timeflag = 0;
						} else {
							timeflag = 0;
						}

					}
						break;
					}
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse calling rule create ");

					DmRuleMapping::DMCallRuleCreate(rule_type, timeflag,
							json_object_array_get_idx(rules_data, messageSize));
				}

			} else if (strcmp(json_object_get_string(verb), "MODIFY") == 0) {

				int rule_type;
				int timeflag = 0;

				json_object *client_data = json_object_new_object();
				json_object *actions = json_object_new_array(); //TODO---check if this can also be cleared

				for (size_t messageSize = 0;
						messageSize < json_object_array_length(rules_data);
						messageSize++) {
					json_object* rule_id = json_object_object_get(
							json_object_array_get_idx(rules_data, messageSize),
							"rule_id");

					rule_type = DmRuleMapping::DMGetRuleType(
							json_object_get_string(rule_id));

					switch (rule_type) {

					case DM_TIME_BASED_RULE:

					case DM_SENSOR_TIME_BASED_RULE_START: {

						json_object* time_start = json_object_object_get(
								json_object_array_get_idx(rules_data,
										messageSize), "time_start");

						std::string time_start_voice = std::string(
								json_object_get_string(time_start));

						std::string cron_time_start =
								DmRuleMapping::DMGetRuleTimeStart(
										json_object_get_string(rule_id));

						if (cron_time_start != time_start_voice) {

							std::string Hr = time_start_voice.substr(0, 2);
							std::string Min = time_start_voice.substr(3, 2);
							std::stringstream ss;
							ss << Min << " " << Hr << " " << "* " << "* "
									<< "*";
							std::string cron_pattern = ss.str();

							json_object_object_add(client_data, "verb",
									json_object_new_string("MODIFY"));
							json_object_object_add(client_data, "cron_pattern",
									json_object_new_string(
											cron_pattern.c_str()));
							json_object_object_add(client_data, "rule_id",
									rule_id);
							json_object_object_add(client_data, "actions",
									actions);

							json_object* Default_command =
									json_object_new_string(
									DM_RULE_COLTROL);
							json_object* Default_client =
									json_object_new_string(
											(char *) std::to_string(
											CRON_HANDLER).c_str());
							json_object* Default_message_ID =
									json_object_new_string("1100");
							AddDMSendResponse(Default_client, rule_id,
									Default_message_ID,
									"Sent request to modify Cronjob",
									Default_command, DM_SUCCESS, client_data);
							break;
						} else {
							syslog(LOG_DEBUG,
									"[DM]: Dm_requestparse rule modify has no cron job");
						}
					}
						break;

					case DM_SENSOR_TIME_BASED_RULE_END: {

						json_object* time_start = json_object_object_get(
								json_object_array_get_idx(rules_data,
										messageSize), "time_start");
						std::string time_start_voice = std::string(
								json_object_get_string(time_start));
						std::string cron_time_start =
								DmRuleMapping::DMGetRuleTimeStart(
										json_object_get_string(rule_id));
						std::string Hr_start = time_start_voice.substr(0, 2);
						std::string Min_start = time_start_voice.substr(3, 2);
						int total_time_start = (std::stoi(Hr_start) * 100)
								+ std::stoi(Min_start);
						if (cron_time_start != time_start_voice) {

							std::string Hr = time_start_voice.substr(0, 2);
							std::string Min = time_start_voice.substr(3, 2);
							std::stringstream ss;
							ss << Min << " " << Hr << " " << "* " << "* "
									<< "*";
							std::string cron_pattern = ss.str();
//							json_object_object_add(client_data, "verb",
//									json_object_new_string("MODIFY"));
//							json_object_object_add(client_data, "cron_pattern",
//									json_object_new_string(
//											cron_pattern.c_str()));
//							json_object_object_add(client_data, "rule_id",
//									rule_id);
//							json_object_object_add(client_data, "actions",
//									actions);
//							//------------------------------------------------
//
//							json_object* Default_command =
//									json_object_new_string(
//									DM_RULE_COLTROL);
//							json_object* Default_client =
//									json_object_new_string(
//											(char *) std::to_string(
//											CRON_HANDLER).c_str());
//							json_object* Default_message_ID =
//									json_object_new_string("1100");
//
//							AddDMSendResponse(Default_client, rule_id,
//									Default_message_ID,
//									"Sent request to modify Cronjob",
//									Default_command, DM_SUCCESS, client_data);

							break;

						} else {

							syslog(LOG_DEBUG,
									"[DM]: Dm_requestparse rule modify has no cron job");
						}

						json_object* time_end = json_object_object_get(
								json_object_array_get_idx(rules_data,
										messageSize), "time_end");

						std::string time_end_voice = std::string(
								json_object_get_string(time_end));

						std::string cron_time_end =
								DmRuleMapping::DMGetRuleTimeEnd(
										std::string(
												json_object_get_string(
														rule_id)));

						std::string Hr_end = time_end_voice.substr(0, 2);
						std::string Min_end = time_end_voice.substr(3, 2);

						int total_time_end = (std::stoi(Hr_end) * 100)
								+ std::stoi(Min_end);

						if (cron_time_end != time_end_voice) {

							std::string Hr = time_end_voice.substr(0, 2);
							std::string Min = time_end_voice.substr(3, 2);

							std::stringstream ss;

							ss << Min << " " << Hr << " " << "* " << "* "
									<< "*";
							std::string cron_pattern = ss.str();

//							json_object_object_add(client_data, "verb",
//									json_object_new_string("MODIFY"));
//							json_object_object_add(client_data, "cron_pattern",
//									json_object_new_string(
//											cron_pattern.c_str()));
//							json_object_object_add(client_data, "rule_id",
//									rule_id);
//							json_object_object_add(client_data, "actions",
//									actions);
//
//							json_object* Default_command =
//									json_object_new_string(
//									DM_RULE_COLTROL);
//							json_object* Default_client =
//									json_object_new_string(
//											(char *) std::to_string(
//											CRON_HANDLER).c_str());
//							json_object* Default_message_ID =
//									json_object_new_string("1100");
//							AddDMSendResponse(Default_client, rule_id,
//									Default_message_ID,
//									"Sent request to modify Cronjob",
//									Default_command, DM_SUCCESS, client_data);
							break;
						} else {

							syslog(LOG_DEBUG,
									"[DM]: Dm_requestparse rule modify has no change in cron job");
						}

						time_t tHeTime = time(NULL);
						struct tm *cTime = localtime(&tHeTime);

						int hour = cTime->tm_hour;
						int min = cTime->tm_min;

						int current_time = (hour * 100) + min;
						//REDMINE 22328 start: Accounting for the first minute in time and sensor based routine
						if (current_time <= total_time_end) {
							if (current_time >= total_time_start) {
								timeflag = 1;
							} else if (total_time_start >= total_time_end) {
								timeflag = 1;
							}
						//REDMINE 22328 end: Accounting for the first minute in time and sensor based routine
						} else if (current_time < total_time_start) {
							timeflag = 0;
						} else {
							timeflag = 0;
						}
					}
						break;
					default: {

					}
						break;
					}
					bool RuleModify = DmRuleMapping::DMCallRulesModify(
							rule_type, timeflag,
							json_object_array_get_idx(rules_data, messageSize));

					int status = DM_FAIL;
					if (RuleModify == true) {
						status = DM_SUCCESS;
					} else {
						status = DM_FAIL;
					}
//					AddDMSendResponse(client, notif_id, message_id,
//							"Sent request to modify Cronjob", command, staus,
//							client_data);

				}
				json_object_put(client_data);
			} else if (strcmp(json_object_get_string(verb), "DELETE") == 0) {

//				json_object* rules_data = json_object_object_get(reqJson,
//						"rules_data");
				json_object *client_data = json_object_new_object();
				json_object *actions = json_object_new_array();	//TODO---check if this can also be cleared

				for (size_t messageSize = 0;
						messageSize < json_object_array_length(rules_data);
						messageSize++) {
					json_object* rules_id = json_object_object_get(
							json_object_array_get_idx(rules_data, messageSize),
							"rule_id");
					DmRuleMapping::DMCallRuleDelete(
							json_object_get_string(rules_id));

					int rule_type = DmRuleMapping::DMGetRuleType(
							json_object_get_string(rules_id));

//					json_object *client_data = json_object_new_object();
//					json_object *actions = json_object_new_array();
					if (!(rule_type == 1)) {

						json_object_object_add(client_data, "verb",
								json_object_new_string("DELETE"));
						json_object_object_add(client_data, "cron_pattern",
								json_object_new_string("null"));
						json_object_object_add(client_data, "rule_id",
								rules_id);
						json_object_object_add(client_data, "actions", actions);

						json_object* Default_command = json_object_new_string(
						DM_RULE_COLTROL);
						json_object* Default_client = json_object_new_string(
								(char *) std::to_string(
								CRON_HANDLER).c_str());
						json_object* Default_message_ID =
								json_object_new_string("1100");
						AddDMSendResponse(Default_client, rules_id,
								Default_message_ID,
								"Sending delete cron pattern rule create request successfully",
								Default_command, DM_SUCCESS, client_data);
					}
					AddDMSendResponse(client, notif_id, message_id,
							"Sent request to delete Cronjob", command,
							DM_SUCCESS, client_data);

				}
				json_object_put(client_data);
			} else if (strcmp(json_object_get_string(verb), "RUN") == 0) {

				/*	json_object *client_data = json_object_new_object();
				 json_object *actions = json_object_new_array();*/

				json_object* rule_id = json_object_object_get(reqJson,
						"rule_id");
				SecneRuleExecution RuleExecutionRsultOBJ;
				RuleExecutionRsultOBJ = DmRuleMapping::DMCallRuleExecute(
						json_object_get_string(rule_id));
				switch (RuleExecutionRsultOBJ.Sceneflag) {

				case 1: {
//
//					syslog(LOG_INFO,
//					 "[DM]: Platform rule executing for 1: [%s]",
//					 RuleExecutionRsultOBJ.RuleSceneID.c_str());
//
//					//------------------------------------------------
//					json_object* client_data = json_object_new_object();
//					json_object_object_add(client_data, "verb",
//							json_object_new_string("EXECUTE"));
//					json_object_object_add(client_data, "cron_pattern",
//							json_object_new_string("null"));
//					json_object_object_add(client_data, "rule_id",
//							json_object_new_string(
//									RuleExecutionRsultOBJ.RuleSceneID.c_str()));
//					json_object_object_add(client_data, "actions",
//							RuleExecutionRsultOBJ.non_iot_action);
//
//					//RequestResponseOBJ.RuleSceneData = std::string(json_object_get_string(client_data));
//					//------------------------------------------------
//
//					json_object* Default_client = json_object_new_string(
//							(char *) std::to_string(
//							CRON_HANDLER).c_str());
//					json_object* Default_Notif_ID = json_object_new_string(
//							(char *) RuleExecutionRsultOBJ.RuleSceneID.c_str()); //json_object_new_string((char *)RuleExecutionRsultOBJ.RuleSceneID.c_str());
//
//					AddDMSendResponse(Default_client, Default_Notif_ID,
//							message_id,
//							"Rule execution request sending successfully",
//							command, DM_SUCCESS, client_data);
//					json_object_put(client_data);
//					syslog(LOG_INFO,
//							"[DM]: Request queue process REQUEST rule SOCKET_SERVICE RUN END");
				}
					break;

				case 2: {

					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse executing rule for /'IOT_ONLY/' [%s]",
							RuleExecutionRsultOBJ.RuleSceneID.c_str());
					DMSceneRuleParseVA(incomingClient, incomingCommand.c_str(),
							RuleExecutionRsultOBJ.RuleSceneID,
							json_object_get_string(message_id),
							RuleExecutionRsultOBJ.iot_action);
				}
					break;

				case 3: {
//					syslog(LOG_INFO,
//					 "[DM]: Platform rule executing for 3: [%s]",
//					 RuleExecutionRsultOBJ.RuleSceneID.c_str());
//
//					DMSceneRuleParseVA(incomingClient, incomingCommand.c_str(),
//							RuleExecutionRsultOBJ.RuleSceneID,
//							json_object_get_string(message_id),
//							RuleExecutionRsultOBJ.iot_action);
//
//					//------------------------------------------------
//					json_object* client_data = json_object_new_object();
//					json_object_object_add(client_data, "verb",
//							json_object_new_string("EXECUTE"));
//					json_object_object_add(client_data, "cron_pattern",
//							json_object_new_string("null"));
//					json_object_object_add(client_data, "rule_id",
//							json_object_new_string(
//									RuleExecutionRsultOBJ.RuleSceneID.c_str()));
//					json_object_object_add(client_data, "actions",
//							RuleExecutionRsultOBJ.non_iot_action);
//
//					json_object* Default_client = json_object_new_string(
//							(char *) std::to_string(
//							CRON_HANDLER).c_str());
//					json_object* Default_Notif_ID = json_object_new_string(
//							(char *) RuleExecutionRsultOBJ.RuleSceneID.c_str());
//
//					AddDMSendResponse(Default_client, Default_Notif_ID,
//							message_id,
//							"Rule execution request sending successfully",
//							command, DM_SUCCESS, client_data);
//					json_object_put(client_data);
				}
					break;

				case 4: {

					syslog(LOG_DEBUG,
							"[DM]: Dm_requestparse executing for default: [%s]",
							RuleExecutionRsultOBJ.RuleSceneID.c_str());
					json_object* Default_command = json_object_new_string(
							"SCENE");
					json_object* Default_RulesceneData = json_object_new_string(
							"DM_DEFAULT");
					AddDMSendResponse(client, rule_id, message_id,
							"Rule is already in progress", Default_command,
							DM_FAIL, Default_RulesceneData);
					json_object_put(Default_command);
					json_object_put(Default_RulesceneData);

				}
					break;
				default:
					break;
				}
			} else {
				syslog(LOG_DEBUG,
						"[DM]: Dm_requestparse no support for rule execution");

			}

		}
			break;
		case REMOVE: {

			DmMsgNotifyMapObj->DmMsgNotifyMap[json_object_get_string(message_id)] =
					json_object_get_string(notif_id);

			for (std::map<std::string, std::string>::iterator itr =
					DmMsgNotifyMapObj->DmMsgNotifyMap.begin();
					itr != DmMsgNotifyMapObj->DmMsgNotifyMap.end(); ++itr) {
				syslog(LOG_DEBUG,
						"[DM]: The notify MAP has Message_Id: [%s], and Notification_Id: [%s]",
						itr->first.c_str(), itr->second.c_str());
			}

			json_object* thing_id = json_object_object_get(reqJson, "thing_id");

			DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

			std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
					DmThingMapObj->DmThingDataMap.find(
							json_object_get_string(thing_id));
			if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()
					|| strncmp(json_object_get_string(thing_id), "FC", 2)
							== 0) {

				json_object* Default_pass_phrase = json_object_new_string(
						"DM_DEFAULT");
				MakeDMReqQueueAddMessage(client, notif_id, message_id, thing_id,
						command, Default_pass_phrase);
				json_object_put(Default_pass_phrase);
			} else {

				json_object* Default_RulesceneData = json_object_new_string(
						"DM_DEFAULT");

				AddDMSendResponse(client, notif_id, message_id,
						"Device does not exists", command,
						THING_NOT_PRESENT_IN_DM, Default_RulesceneData);
				json_object_put(Default_RulesceneData);
			}
		}
			break;
		case EXCLUSION: {

			DmMsgNotifyMapObj->DmMsgNotifyMap[json_object_get_string(message_id)] =
					json_object_get_string(notif_id);

			for (std::map<std::string, std::string>::iterator itr =
					DmMsgNotifyMapObj->DmMsgNotifyMap.begin();
					itr != DmMsgNotifyMapObj->DmMsgNotifyMap.end(); ++itr) {
				syslog(LOG_DEBUG,
						"[DM]: The notify MAP has Message_Id: [%s], and Notification_Id: [%s]",
						itr->first.c_str(), itr->second.c_str());
			}

			json_object* Default_command = json_object_new_string("EXCLUSION");
			json_object* Default_pass_phrase = json_object_new_string(
					"DM_DEFAULT");
			json_object* Default_thing_id = json_object_new_string(
					"DM_DEFAULT");
			MakeDMReqQueueAddMessage(client, notif_id, message_id,
					Default_thing_id, Default_command, Default_pass_phrase);
			json_object_put(Default_command);
			json_object_put(Default_pass_phrase);
			json_object_put(Default_thing_id);
		}
			break;

		case LEARN_MODE: {

			DmMsgNotifyMapObj->DmMsgNotifyMap[json_object_get_string(message_id)] =
					json_object_get_string(notif_id);

			/*for (std::map<std::string, std::string>::iterator itr =
			 DmMsgNotifyMapObj->DmMsgNotifyMap.begin();
			 itr != DmMsgNotifyMapObj->DmMsgNotifyMap.end(); ++itr) {
			 syslog(LOG_DEBUG,
			 "[DM]: The notify MAP has Message_Id: [%s], and Notification_Id: [%s]",
			 itr->first.c_str(), itr->second.c_str());
			 }*/

			json_object* Default_command = json_object_new_string("LEARN_MODE");
			json_object* Default_pass_phrase = json_object_new_string(
					"DM_DEFAULT");
			json_object* Default_thing_id = json_object_new_string(
					"DM_DEFAULT");
			MakeDMReqQueueAddMessage(client, notif_id, message_id,
					Default_thing_id, Default_command, Default_pass_phrase);
			json_object_put(Default_command);
			json_object_put(Default_pass_phrase);
			json_object_put(Default_thing_id);
		}
			break;
		case FACTORY_RESET: {
			json_object* Default_pass_phrase = json_object_new_string(
					"DM_DEFAULT");
			json_object* Default_thing_id = json_object_new_string(
					"DM_DEFAULT");
			MakeDMReqQueueAddMessage(client, notif_id, message_id,
					Default_thing_id, command, Default_pass_phrase);
			json_object_put(Default_pass_phrase);
			json_object_put(Default_thing_id);
		}
			break;
		case IOT_RESET: {

			json_object* Default_pass_phrase = json_object_new_string(
					"DM_DEFAULT");
			json_object* Default_thing_id = json_object_new_string(
					"DM_DEFAULT");
			MakeDMReqQueueAddMessage(client, notif_id, message_id,
					Default_thing_id, command, Default_pass_phrase);
			strcpy(DM_KILL_SWITCH, "");
			set_shifu_uci_param((char *) "DM_KILL_SWITCH", DM_KILL_SWITCH);	//! Setting the DM_KILL_SWITCH variable to Default state
			json_object_put(Default_pass_phrase);
			json_object_put(Default_thing_id);
		}
			break;

		case GETRULE: {

			DmReqResMapping* DmReqResMappingOBJ =
					DmReqResMapping::GetInstance();

			DmReqResMappingOBJ->AddToDmReqResMapping(
					json_object_get_string(notif_id),
					json_object_get_string(message_id), 1);

			json_object* Default_RulesceneData = json_object_new_string(
					"DM_DEFAULT");

			AddDMSendResponse(client, notif_id, message_id,
					"Rule fetch successful", command, DM_SUCCESS,
					Default_RulesceneData);
			json_object_put(Default_RulesceneData);
		}
			break;
		case GETSCENE: {

			DmReqResMapping* DmReqResMappingOBJ =
					DmReqResMapping::GetInstance();

			DmReqResMappingOBJ->AddToDmReqResMapping(
					json_object_get_string(notif_id),
					json_object_get_string(message_id), 1);

			json_object* Default_RulesceneData = json_object_new_string(
					"DM_DEFAULT");

			AddDMSendResponse(client, notif_id, message_id,
					"Scene fetch successful", command,
					DM_SUCCESS, Default_RulesceneData);
			json_object_put(Default_RulesceneData);
		}
			break;

		case S2_SECURITY: {

			//	syslog(LOG_INFO, "[DM]: IN SECURITY2 RECIEVE");

			json_object* ThingData1 = json_tokener_parse(
					json_object_get_string(
							json_object_object_get(reqJson, "data")));

			json_object* pass_phrase = json_object_object_get(ThingData1,
					"passphrase");
			json_object* thing_id = json_object_object_get(ThingData1,
					"thing_id");
			/*	json_object* request_id = json_object_object_get(ThingData1,
			 "request_id");*/

			syslog(LOG_DEBUG, "[DM]: Request received pass_phrase [%s]",
					json_object_get_string(pass_phrase));
			syslog(LOG_DEBUG, "[DM]: Request received thing_id [%s]",
					json_object_get_string(thing_id));

			//	syslog(LOG_DEBUG, "[DM]: Request received request_id [%s]",
			//			json_object_get_string(request_id));
			json_object* Default_thing_id = json_object_new_string(
					"DM_DEFAULT");
			MakeDMReqQueueAddMessage(client, notif_id, message_id,
					Default_thing_id, command, pass_phrase);

			pthread_cond_signal(&DM_Req_Discovery);

		}
			break;
		case FORCE_REMOVE: {

			DmMsgNotifyMapObj->DmMsgNotifyMap[json_object_get_string(message_id)] =
					json_object_get_string(notif_id);

			for (std::map<std::string, std::string>::iterator itr =
					DmMsgNotifyMapObj->DmMsgNotifyMap.begin();
					itr != DmMsgNotifyMapObj->DmMsgNotifyMap.end(); ++itr) {
				syslog(LOG_DEBUG,
						"[DM]: The notify MAP has Message_Id: [%s], and Notification_Id: [%s]",
						itr->first.c_str(), itr->second.c_str());
			}

			syslog(LOG_INFO, "[DM]: Platform calling force remove mode");

			json_object* thing_id = json_object_object_get(reqJson, "thing_id");

			DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

			std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
					DmThingMapObj->DmThingDataMap.find(
							json_object_get_string(thing_id));
			if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {

				json_object* Default_pass_phrase = json_object_new_string(
						"DM_DEFAULT");
				MakeDMReqQueueAddMessage(client, notif_id, message_id, thing_id,
						command, Default_pass_phrase);
				json_object_put(Default_pass_phrase);
			} else {

				json_object* Default_RulesceneData = json_object_new_string(
						"DM_DEFAULT");

				AddDMSendResponse(client, notif_id, message_id,
						"Device does not exists", command,
						THING_NOT_PRESENT_IN_DM, Default_RulesceneData);
				json_object_put(Default_RulesceneData);
			}

		}
			break;
		default: {
			json_object* Default_RulesceneData = json_object_new_string(
					"DM_DEFAULT");
			AddDMSendResponse(client, notif_id, message_id, "Command is wrong",
					command, DM_FAIL, Default_RulesceneData);
			json_object_put(Default_RulesceneData);
		}
			break;
		}
		json_object_put(message_id);
	} else {
		syslog(LOG_CRIT, "[DM]: Bad data received");

		RequestResponse ReqMessageDataOBJ;

		ReqMessageDataOBJ.Message = "Bad Data received from Client ";
		ReqMessageDataOBJ.Notif_Id = "DM_DEFAULT";
		ReqMessageDataOBJ.Status_Code = DM_FAIL;

		DmPlatform::DmSendingRequestAck(ReqMessageDataOBJ);

	}
}
