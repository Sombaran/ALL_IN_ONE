
#include "dm_response_queue.hpp"
#include "dm_send_data_tronx.hpp"
#include "dm_things_backup_db.hpp"
#include "dm_thingmap.hpp"
#include "dm_things_backup_db.hpp"
#include "dm_req_res_mapping.hpp"
#include "dm_platform.hpp"
#include "dm_secne_mapping.hpp"
#include "dm_rule_mapping.hpp"
#include "dm_utility.hpp"
#include "dm_request_parse.hpp"

#define ZERO_LENGTH 0
#define DM_SEND_MULTIPLE_RESP 1

/*DmResponseQueue::DmResponseQueue() {	// TODO Auto-generated constructor stub
}
DmResponseQueue::~DmResponseQueue() {   // TODO Auto-generated destructor stub
}*/

DmResponseQueue * DmResponseQueue::Instance = 0;

DmResponseQueue * DmResponseQueue::GetInstance() {
	if (Instance == 0) {
		Instance = new DmResponseQueue();
	}
	return Instance;
}

bool DmResponseQueue::DMResQueueInit() {

	RequestResponse init;
	init.Thing_Id = "0";
	init.Thing_Response = "Initial message, ignore this.";
	init.priority = 0;

	DMRspQueueDataTronx.push_back(init);

	return true;
}

void DmResponseQueue::DMResQueueheapify(
		std::vector<RequestResponse> &DMRspQueueDataTronx, int i, int n) {
	RequestResponse temp;
	temp = DMRspQueueDataTronx[i];

	int j = 2 * i;
	while (j <= n) {
		if (j < n
				&& DMRspQueueDataTronx[j + 1].priority
						< DMRspQueueDataTronx[j].priority) {
			j = j + 1;
		}

		if (temp.priority <= DMRspQueueDataTronx[j].priority) {
			break;
		} else if (temp.priority > DMRspQueueDataTronx[j].priority) {
			DMRspQueueDataTronx[j / 2] = DMRspQueueDataTronx[j];
			j = 2 * j;
		}
	}

	DMRspQueueDataTronx[j / 2] = temp;
//	DMRspQueueDataTronx.shrink_to_fit();
}

void DmResponseQueue::DMResQueueheapsort(
		std::vector<RequestResponse> &DMRspQueueDataTronx, int n) {
	int i;
	RequestResponse temp;

	for (i = n; i >= 2; i--) {
		temp = DMRspQueueDataTronx[i];

		if (DMRspQueueDataTronx[1].priority
				!= DMRspQueueDataTronx[i].priority) {
			DMRspQueueDataTronx[i] = DMRspQueueDataTronx[1];

			DMRspQueueDataTronx[1] = temp;

			DMResQueueheapify(DMRspQueueDataTronx, 1, i - 1);
		}
	}
//	DMRspQueueDataTronx.shrink_to_fit();
}

void DmResponseQueue::DMResQueuebuild_heap(
		std::vector<RequestResponse> &DMRspQueueDataTronx, int n) {
	for (int i = n / 2; i >= 1; i--) {
		DMResQueueheapify(DMRspQueueDataTronx, i, n);
	}
}

void DmResponseQueue::DMResQueuefilter(
		std::vector<RequestResponse> &DMRspQueueDataTronx,
		std::vector<MessageIndex> &DMRspQueueIndexTronx, std::string id,
		bool &update, int &position) {

	update = true;
	size_t counter;

	for (size_t i = 0; i < DMRspQueueIndexTronx.size(); i++) {
		if (id == DMRspQueueIndexTronx[i].Thing_Id) {
			if (DMRspQueueIndexTronx[i].repeatAllowedStatus == 0
					&& DMRspQueueIndexTronx[i].count == 1) {
				update = false;
				counter = 1;
				while (counter < DMRspQueueDataTronx.size()) {
					if (id == DMRspQueueDataTronx[counter].queue_id) {

						syslog(LOG_DEBUG,
								"[DM]: Response queue filtering messages : [%s]",
								DMRspQueueDataTronx[counter].queue_id.c_str());

						DMRspQueueDataTronx.erase(
								DMRspQueueDataTronx.begin() + counter);
						break;
					}
					counter++;
				}
			}
			position = i;
			break;
		}
	}
/*	syslog(LOG_DEBUG,
			"[DM] : The vector size [%d] and its capacity BEFORE: [%d]=======RES",
			DMRspQueueDataTronx.size(), DMRspQueueDataTronx.capacity());
	DMRspQueueDataTronx.shrink_to_fit();
	syslog(LOG_DEBUG,
			"[DM] : The vector size [%d] and its capacity AFTER: [%d]=======RES",
			DMRspQueueDataTronx.size(), DMRspQueueDataTronx.capacity());
	syslog(LOG_DEBUG,
			"[DM] : The vector size [%d] and its capacity BEFORE: [%d]=======RES",
			DMRspQueueIndexTronx.size(), DMRspQueueIndexTronx.capacity());
	DMRspQueueIndexTronx.shrink_to_fit();
	syslog(LOG_DEBUG,
			"[DM] : The vector size [%d] and its capacity AFTER: [%d]=======RES",
			DMRspQueueIndexTronx.size(), DMRspQueueIndexTronx.capacity());*/
}

void DmResponseQueue::DMResQueueAddMessage(RequestResponse RequestResponseOBJ,
		std::string Req_Type, std::string Req_URI, std::string Thing_Response,
		int Priority) {

	pthread_mutex_lock(&RspAddRcvMutx);
	/*syslog(LOG_INFO,
			"[DM]: Response  queue process start addinng message from queue lock rspaddrcvmutx");*/
	bool updateStatus = false;
	int position = 0;
	bool ItemFlag = false;

	for (size_t i = 0; i < DMRspQueueIndexTronx.size(); i++) {

		if (RequestResponseOBJ.Thing_Id == DMRspQueueIndexTronx[i].Thing_Id) {
			ItemFlag = true;
		}
	}

	if (ItemFlag == false) {
		MessageIndex temp;
		temp.Thing_Id = RequestResponseOBJ.Thing_Id
				+ RequestResponseOBJ.Entity_Id
				+ RequestResponseOBJ.Property_Name;
		temp.repeatAllowedStatus = ManyResponse;
		temp.count = 0;
		DMRspQueueIndexTronx.push_back(temp);
	}

	DMResQueuefilter(DMRspQueueDataTronx, DMRspQueueIndexTronx,
			RequestResponseOBJ.Thing_Id + RequestResponseOBJ.Entity_Id
					+ RequestResponseOBJ.Property_Name, updateStatus, position);

	RequestResponse MessageDataObj;
	MessageDataObj.Thing_Type=RequestResponseOBJ.Thing_Type;
	MessageDataObj.Status_Code = RequestResponseOBJ.Status_Code;
	MessageDataObj.Client = RequestResponseOBJ.Client;
	MessageDataObj.Command = RequestResponseOBJ.Command;
	MessageDataObj.Thing_Id = RequestResponseOBJ.Thing_Id;
	MessageDataObj.Message_Id = RequestResponseOBJ.Message_Id;
	MessageDataObj.Notif_Id = RequestResponseOBJ.Notif_Id;
	MessageDataObj.Entity_Id = RequestResponseOBJ.Entity_Id;
	MessageDataObj.Property_Name = RequestResponseOBJ.Property_Name;
	MessageDataObj.Property_Type = RequestResponseOBJ.Property_Type;
	MessageDataObj.Property_Value = RequestResponseOBJ.Property_Value;
	MessageDataObj.queue_id = RequestResponseOBJ.Thing_Id
			+ RequestResponseOBJ.Entity_Id + RequestResponseOBJ.Property_Name;
	MessageDataObj.Thing_Response = Thing_Response;
	MessageDataObj.Req_URI = Req_URI;
	MessageDataObj.Req_Type = Req_Type;
	MessageDataObj.priority = Priority;

/*	syslog(LOG_DEBUG,
			"[DM]: Response  queue process response count before adding into queue: [%ld]",
			DMRspQueueDataTronx.size());

	syslog(LOG_DEBUG,
			"[DM]: Response  queue process response adding into response message: [%s]",
			Thing_Response.c_str());*/

	syslog(LOG_DEBUG,
			"[DM]: Response queue adding notif_id: [%s] message_id: [%s]",
			MessageDataObj.Notif_Id.c_str(), MessageDataObj.Message_Id.c_str());

	DMRspQueueDataTronx.push_back(MessageDataObj);
	syslog (LOG_DEBUG,"[DM]: Dm_Responsequeue_capacity [%zu]", DMRspQueueDataTronx.capacity());
	syslog (LOG_DEBUG,"[DM]: DmResponsequeue_size [%zu]", DMRspQueueDataTronx.size());
	/*syslog(LOG_DEBUG,
			"[DM]: Response  queue process response count after adding into queue: [%ld]",
			DMRspQueueDataTronx.size());*/

	int size = (DMRspQueueDataTronx.size() - 1);
	DMResQueuebuild_heap(DMRspQueueDataTronx, size);
	DMResQueueheapsort(DMRspQueueDataTronx, size);

	if (updateStatus) {
		DMRspQueueIndexTronx[position].count++;
	}
	if (RspQueueSizeflag == 0) {
		pthread_cond_signal(&RspAddRcvCond);
		RspQueueSizeflag = 1;
	}

	pthread_mutex_unlock(&RspAddRcvMutx);

	/*syslog(LOG_INFO,
			"[DM]: Response  queue process end addinng message from queue unlock rspaddrcvmutx");*/
}

void DmResponseQueue::DMResQueueRemoveMessage() {

	for (size_t i = 0; i < DMRspQueueIndexTronx.size(); i++) {
		if (DMRspQueueIndexTronx[i].Thing_Id
				== DMRspQueueDataTronx[1].Thing_Id) {
			DMRspQueueIndexTronx[i].count--;
			break;
		}
	}
	DMRspQueueDataTronx.erase(DMRspQueueDataTronx.begin() + 1);
}

void DmResponseQueue::DMResQueueReceiveMessage() {

	RequestResponse MessageDataOBJ;

	if (DMRspQueueDataTronx.size() == 1) {

		syslog(LOG_INFO,
				"[DM]: DM RESPONSE QUEUE PROCESS IN REMOVE QUEUE IS EMPTY");
		RspQueueSizeflag = 0;
		pthread_cond_wait(&RspAddRcvCond, &RspMutex3Cond);

		//SWAMI usleep(300);
	}

	pthread_mutex_lock(&RspAddRcvMutx);

	/*syslog(LOG_INFO,
			"[DM]: DM RESPONSE QUEUE PROCESS START RECEIVING MESSAGE FROM QUEUE LOCK RspAddRcvMutx");*/

	MessageDataOBJ = DMRspQueueDataTronx[1];

	syslog(LOG_DEBUG,
			"[DM]: DM RESPONSE QUEUE PROCESS RESPONSE MESSAGE RECEIVE NOTIF_ID : [%s] MESSAGE_ID : [%s] ",
			MessageDataOBJ.Notif_Id.c_str(), MessageDataOBJ.Message_Id.c_str());

	DMResQueueRemoveMessage();

	pthread_mutex_unlock(&RspAddRcvMutx);

	/*syslog(LOG_INFO,
			"[DM]: DM RESPONSE QUEUE PROCESS START RECEIVING MESSAGE FROM QUEUE UNLOCK RspAddRcvMutx");*/

	syslog(LOG_DEBUG,
			"[DM]: DM RESPONSE QUEUE PROCESS RESPONSE THING SENDING TO TRONX : [%s]",
			MessageDataOBJ.Thing_Response.c_str());

//###########################################################################################################
	if(MessageDataOBJ.Command == "OBSERVE"){
	syslog(LOG_DEBUG,
			"[DM]: Platform Checking-rule associated for thig_id : [%s]",
			(MessageDataOBJ.Thing_Id + MessageDataOBJ.Entity_Id
					+ MessageDataOBJ.Property_Name
					+ MessageDataOBJ.Property_Value).c_str());

	std::vector<std::string> retRule_ids;
	std::string temp="Sensor";
 	/*syslog(LOG_DEBUG,"[DM]: DM req type : [%s] ",MessageDataOBJ.Thing_Type.c_str());*/

	if(MessageDataOBJ.Thing_Type.find(temp)!= std::string::npos){
		/*syslog(LOG_INFO, "[DM]: PLATFROM RULE CHECKING FOR Sensor ");*/
		retRule_ids = DmRuleMapping::DMGetRuleId(MessageDataOBJ.Thing_Id,
						MessageDataOBJ.Entity_Id, MessageDataOBJ.Property_Name,
						MessageDataOBJ.Property_Value);
	}else  syslog(LOG_INFO, "[DM]: PLATFROM No rule checking FOR Actuator ");


	/*syslog(LOG_DEBUG,
			"[DM]: Platform executing-rule printing rule vector size : [%ld]",
			retRule_ids.size());

	syslog(LOG_INFO, "[DM]: PLATFROM RULE CHECKING FOR ");*/

	if (retRule_ids.size() != 0) {

		/*syslog(LOG_DEBUG,
				"[DM]: Platform executing-rule associated for thig_id : [%s]",
				MessageDataOBJ.Thing_Id.c_str());*/

		SecneRuleExecution RuleExecutionRsultOBJ;

		RuleExecutionRsultOBJ = DmRuleMapping::DMCallRuleExecuteObs(
				MessageDataOBJ.Thing_Id, MessageDataOBJ.Entity_Id,
				MessageDataOBJ.Property_Name, MessageDataOBJ.Property_Value);

		syslog(LOG_DEBUG,
				"[DM]: DM_RESPONSE_QUEUE Platform rule id: [%s] rule execute status: [%d] execute Sceneflag: [%d]",
				RuleExecutionRsultOBJ.RuleSceneID.c_str(),
				RuleExecutionRsultOBJ.RuleSceneExecuteFlag,
				RuleExecutionRsultOBJ.Sceneflag);

		switch (RuleExecutionRsultOBJ.Sceneflag) {
		case 1:
		{
//
//			syslog(LOG_INFO, "[DM]: Platform rule executing for 1: [%s]",
//					RuleExecutionRsultOBJ.RuleSceneID.c_str());
//
//			if (RuleExecutionRsultOBJ.RuleSceneExecuteFlag == false) {
//
//				if (json_object_get_type(RuleExecutionRsultOBJ.non_iot_action)
//						!= json_type_string) {
//
//					RequestResponse RequestResponseOBJ;
//
//					RequestResponseOBJ.Client = CRON_HANDLER;
//					RequestResponseOBJ.Command = DM_RULE_COLTROL;
//					RequestResponseOBJ.Message =
//							"Sent Rule execute request";
//					RequestResponseOBJ.Message_Id = std::string("1100");
//					RequestResponseOBJ.Notif_Id =
//							RuleExecutionRsultOBJ.RuleSceneID;
//					RequestResponseOBJ.Status_Code = DM_SUCCESS;
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
//					//------------------------------------------------
//
//					RequestResponseOBJ.RuleSceneData = std::string(
//							json_object_get_string(client_data));
//
//					DmPlatform::DmSendingResponse(RequestResponseOBJ);
//					//json_object_put(client_data);
//
//				} else {
//
//				}
//			}
////			else {
//
////			}
		}
			break;
		case 2: {
			syslog(LOG_INFO, "[DM]: Dm_responsequeue executing rule /'IOT_ONLY/' [%s]",
					RuleExecutionRsultOBJ.RuleSceneID.c_str());
			if (json_object_get_type(RuleExecutionRsultOBJ.iot_action)
					!= json_type_string) {

				DMSceneRuleParseVA(DM_SERVICE, "RULE",
						RuleExecutionRsultOBJ.RuleSceneID, "DM_DEFAULT",
						RuleExecutionRsultOBJ.iot_action);

			} else {

			}
		}
			break;
		case 3:
		{
//			syslog(LOG_INFO, "[DM]: Platform rule executing for 3: [%s]",
//					RuleExecutionRsultOBJ.RuleSceneID.c_str());
//
//			if (json_object_get_type(RuleExecutionRsultOBJ.iot_action)
//					!= json_type_string
//					|| json_object_get_type(
//							RuleExecutionRsultOBJ.non_iot_action)
//							!= json_type_string) {
//
//				DMSceneRuleParseVA(DM_SERVICE, "RULE",
//						RuleExecutionRsultOBJ.RuleSceneID, "DM_DEFAULT",
//						RuleExecutionRsultOBJ.iot_action);
//
//				RequestResponse RequestResponseOBJ;
//
//				RequestResponseOBJ.Client = CRON_HANDLER;
//				RequestResponseOBJ.Command = DM_RULE_COLTROL;
//				RequestResponseOBJ.Message =
//						"Sent Rule execute request";
//				RequestResponseOBJ.Message_Id = std::string("1100");
//				RequestResponseOBJ.Notif_Id = RuleExecutionRsultOBJ.RuleSceneID;
//				RequestResponseOBJ.Status_Code = DM_SUCCESS;
//
//				//------------------------------------------------
//				json_object* client_data = json_object_new_object();
//				json_object_object_add(client_data, "verb",
//						json_object_new_string("EXECUTE"));
//				json_object_object_add(client_data, "cron_pattern",
//						json_object_new_string("null"));
//				json_object_object_add(client_data, "rule_id",
//						json_object_new_string(
//								RuleExecutionRsultOBJ.RuleSceneID.c_str()));
//				json_object_object_add(client_data, "actions",
//						RuleExecutionRsultOBJ.non_iot_action);
//				//------------------------------------------------
//
//				RequestResponseOBJ.RuleSceneData = std::string(
//						json_object_get_string(client_data));
//
//				DmPlatform::DmSendingResponse(RequestResponseOBJ);
//				//json_object_put(client_data);
//			} else {
//			}
		}
			break;
		case 4: {

			syslog(LOG_INFO, "[DM]: Dm_responsequeue executing for Rule is already in proces: [%s]",
					RuleExecutionRsultOBJ.RuleSceneID.c_str());
			RequestResponse RequestResponseOBJ;
			RequestResponseOBJ.Client = DM_SERVICE;
			RequestResponseOBJ.Command = "RULE";
			RequestResponseOBJ.Message = "Rule is already in process";
			RequestResponseOBJ.Message_Id = std::string("1100");
			RequestResponseOBJ.Notif_Id = RuleExecutionRsultOBJ.RuleSceneID;
			RequestResponseOBJ.Status_Code = DM_FAIL;
			RequestResponseOBJ.RuleSceneData = "DM_DEFAULT";
			DmPlatform::DmSendingResponse(RequestResponseOBJ);
		}
			break;
		default:
			break;
		}
		/*
		 //------------------------------------------------------------------------------
		 if (RuleExecutionRsultOBJ.RuleSceneExecuteFlag == true) {

		 syslog(LOG_INFO, "[DM]: PLATFROM RULE EXECUTING FOR : [%s]",
		 RuleExecutionRsultOBJ.RuleSceneID.c_str());

		 if (json_object_get_type(RuleExecutionRsultOBJ.iot_action)
		 != json_type_string
		 || json_object_get_type(
		 RuleExecutionRsultOBJ.non_iot_action)
		 != json_type_string) {

		 DMSceneRuleParseVA(DM_SERVICE, "RULE",
		 RuleExecutionRsultOBJ.RuleSceneID, "DM_DEFAULT",
		 RuleExecutionRsultOBJ.iot_action);

		 RequestResponse RequestResponseOBJ;

		 RequestResponseOBJ.Client = CRON_HANDLER;
		 RequestResponseOBJ.Command = DM_RULE_COLTROL;
		 RequestResponseOBJ.Message =
		 "Sent Rule execute request";
		 RequestResponseOBJ.Message_Id = std::string("1100");
		 RequestResponseOBJ.Notif_Id =
		 RuleExecutionRsultOBJ.RuleSceneID;
		 RequestResponseOBJ.Status_Code = DM_SUCCESS;

		 //------------------------------------------------
		 json_object* client_data = json_object_new_object();
		 json_object_object_add(client_data, "verb",
		 json_object_new_string("EXECUTE"));
		 json_object_object_add(client_data, "cron_pattern",
		 json_object_new_string("null"));
		 json_object_object_add(client_data, "rule_id",
		 json_object_new_string(
		 RuleExecutionRsultOBJ.RuleSceneID.c_str()));
		 json_object_object_add(client_data, "actions",
		 RuleExecutionRsultOBJ.non_iot_action);
		 //------------------------------------------------

		 RequestResponseOBJ.RuleSceneData = std::string(
		 json_object_get_string(client_data));

		 DmPlatform::DmSendingResponse(RequestResponseOBJ);

		 }
		 } else {

		 syslog(LOG_INFO,
		 "[DM]: Platform rule skipping for rule_id: [%s] rule already executing",
		 RuleExecutionRsultOBJ.RuleSceneID.c_str());
		 }
		 //-----------------------------------------------------------------------------------------
		 */
	} else {
		syslog(LOG_DEBUG, "[DM]: Platform no-rule associated for THING ID: [%s]",
				MessageDataOBJ.Thing_Id.c_str());
	}
	retRule_ids.shrink_to_fit();
	}
//#########################################################################################################
	syslog(LOG_DEBUG,
					"[DM]: Response queue calling [%s] from main map",MessageDataOBJ.Command.c_str());

	if(MessageDataOBJ.Status_Code == DM_SUCCESS){
	switch (ConvertStringToInt(MessageDataOBJ.Command)) {
	case CONTROL:
	case OBSERVE: {
		/*syslog(LOG_INFO,
				"[DM]: Response  queue process observe calling update property table");*/

		DmThingsBackupDb* DmThingsBackupDbOBJ = DmThingsBackupDb::GetInstance();

		DMPropertyDB DMPropertyDBOBJ;

		DMPropertyDBOBJ.THING_ID = MessageDataOBJ.Thing_Id;
		DMPropertyDBOBJ.ENTITY_ID = MessageDataOBJ.Entity_Id;
		DMPropertyDBOBJ.PROPERTY_NAME = MessageDataOBJ.Property_Name;
		DMPropertyDBOBJ.PROPERTY_VALUE = "PROPERTY_VALUE=\'"
				+ MessageDataOBJ.Property_Value + "\'";

		DmThingsBackupDbOBJ->UpdatePropertyTable(DMPropertyDBOBJ);
	}
		break;

	case FORCE_REMOVE:
	case REMOVE:
	case EXCLUSION: {

		DmThingMap* DmThingMapOBJ = DmThingMap::GetInstance();

		/*syslog(LOG_DEBUG,
				"[DM]: Response  queue process remove/exclusion thing from main map");*/

		DmThingMapOBJ->RemoveToThingMap(MessageDataOBJ.Thing_Id);
//		DmSceneMapping::DeleteThingIDFromScenes(MessageDataOBJ.Thing_Id);
		DmRuleMapping::DeleteThingIDFromRule(MessageDataOBJ.Thing_Id);
	}
		break;

	case S2_SECURITY: {

//		DmThingMap* DmThingMapOBJ = DmThingMap::GetInstance();

		/*syslog(LOG_DEBUG,
				"[DM]: Response  queue process s2 security thing from main map");*/
	}
		break;

	default: {
		syslog(LOG_DEBUG, "[DM]: Response  queue process dafault case");
	}
		break;
	}
	}
	//---------------------------------------------------------------------------------------------------------------------

	DmReqResMapping* DmReqResMappingOBJ = DmReqResMapping::GetInstance();

	if (!MessageDataOBJ.Message_Id.empty()
			&& MessageDataOBJ.Message_Id != "0") {

		syslog(LOG_INFO, "[DM]: Platform MessageDataOBJ.Command : [%s]",
				MessageDataOBJ.Command.c_str());

		if ((MessageDataOBJ.Command.compare("OBSERVE") != 0)
				&& (MessageDataOBJ.Client != DM_SERVICE)) {

		

			if( MessageDataOBJ.Command.compare("S2_SECURITY") != 0)
			{

			/*syslog(LOG_DEBUG, "[DM]: Response  DM_S2_SECURITY new");*/

			MessageDataOBJ = DmReqResMappingOBJ->CheckToDmReqResMapping(
					MessageDataOBJ);

			//-------------------------------------------------------------------------------------------------------------
			//ADDING ALL MESSAGAES INTO THE FINAL RESPONSE MAP START

			switch (std::stol(MessageDataOBJ.Message_Id.substr(2, 2))) {

			case AREA_CONTROL:

				/*syslog(LOG_INFO,
						"[DM]: Response  queue process received for area_control with message_id : [%s] notif_id: [%s] thing_respose : [%s]",
						MessageDataOBJ.Message_Id.c_str(),
						MessageDataOBJ.Notif_Id.c_str(),
						MessageDataOBJ.Thing_Response.c_str());*/

			case SCENE_CONTROL:

				/*syslog(LOG_INFO,
						"[DM]: Response  queue process received for scene_control with message_id : [%s] notif_id: [%s] thing_respose : [%s]",
						MessageDataOBJ.Message_Id.c_str(),
						MessageDataOBJ.Notif_Id.c_str(),
						MessageDataOBJ.Thing_Response.c_str());*/

			case RULE_COLTROL: {

				syslog(LOG_INFO,
						"[DM]: Response queue for AREA_CONTROL/SCENE_CONTROL/RULE_CONTROL with message_id: [%s] notif_id: [%s] thing_respose: [%s]",
						MessageDataOBJ.Message_Id.c_str(),
						MessageDataOBJ.Notif_Id.c_str(),
						MessageDataOBJ.Thing_Response.c_str());

#if DM_SEND_MULTIPLE_RESP == 1

				/*syslog(LOG_INFO,
						"[DM]: ***********SINGLE RESPONSE TO TRONX : [%s]",
						MessageDataOBJ.Thing_Response.c_str());*/

				DmSendDataTronx* DmSendDataTronxObj =
						DmSendDataTronx::GetInstance();

				ReqMessageData ReqMessageDataOBJ;

				ReqMessageDataOBJ.ReponseData = MessageDataOBJ.Thing_Response;
				ReqMessageDataOBJ.ResponseMethod = MessageDataOBJ.Req_Type;
				ReqMessageDataOBJ.ResponseURI = MessageDataOBJ.Req_URI;
				ReqMessageDataOBJ.Thing_ID = MessageDataOBJ.Thing_Id;
				ReqMessageDataOBJ.Command = MessageDataOBJ.Command;
				ReqMessageDataOBJ.priority = PRORITY_VALUE;

				ReqMessageDataOBJ.queue_id = MessageDataOBJ.Thing_Id
						+ MessageDataOBJ.Entity_Id
						+ MessageDataOBJ.Property_Name;

				DmSendDataTronxObj->DmSendAddMessage(ReqMessageDataOBJ);

#endif
//-----------------------------------------------------------------------------------------------------------

				DmReqResMapping* DmReqResMappingOBJ =
						DmReqResMapping::GetInstance();

				std::map<std::string, DMFinalResMessage>::iterator DMFinalResponseItr =
						DmReqResMappingOBJ->DMFinalResponse.find(
								MessageDataOBJ.Notif_Id);
				if (DMFinalResponseItr
						!= DmReqResMappingOBJ->DMFinalResponse.end()) {

					json_object_array_add(
							DMFinalResponseItr->second.allResponse,
							json_tokener_parse(
									MessageDataOBJ.Thing_Response.c_str()));

					DMFinalResponseItr->second.DMFinalResponseData =
							MessageDataOBJ.Thing_Response;

					DmReqResMappingOBJ->DMFinalResponse[MessageDataOBJ.Notif_Id] =
							DMFinalResponseItr->second;

				} else {

					DMFinalResMessage DMFinalResMessageOBJ;

					DMFinalResMessageOBJ.allResponse = json_object_new_array();
					DMFinalResMessageOBJ.DMFinalResponseData =
							MessageDataOBJ.Thing_Response;

					json_object_array_add(DMFinalResMessageOBJ.allResponse,
							json_tokener_parse(
									MessageDataOBJ.Thing_Response.c_str()));

					DmReqResMappingOBJ->DMFinalResponse[MessageDataOBJ.Notif_Id] =
							DMFinalResMessageOBJ;

				}
//-----------------------------------------------------------------------------------------------------------------
			}
				break;

			default: {

				/*syslog(LOG_INFO,
						"[DM]: Response  queue process received for other message_id: [%s]",
						MessageDataOBJ.Message_Id.c_str());*/

				DMFinalResMessage DMFinalResMessageOBJ;

				DMFinalResMessageOBJ.allResponse = json_object_new_array();
				DMFinalResMessageOBJ.DMFinalResponseData =
						MessageDataOBJ.Thing_Response;

				json_object_array_add(DMFinalResMessageOBJ.allResponse,
						json_tokener_parse(
								MessageDataOBJ.Thing_Response.c_str()));

				DmReqResMappingOBJ->DMFinalResponse[MessageDataOBJ.Notif_Id] =
						DMFinalResMessageOBJ;

				/*syslog(LOG_DEBUG,
						"[DM]: DM response queue skipping notif_id: [%s] thing response: [%s] map count: [%ld]",
						MessageDataOBJ.Notif_Id.c_str(),
						MessageDataOBJ.Thing_Response.c_str(),
						DmReqResMappingOBJ->DMFinalResponse.size());*/
			}
				break;
			}

			//ADDING ALL MESSAGAES INTO THE FINAL RESPONSE MAP END
			//-------------------------------------------------------------------------------------------------------------

			if (MessageDataOBJ.SendFlag == true) {

				std::string RULE_SCENE_AREA_STATUS = "DM_DEFAULT";

				DmReqResMapping* DmReqResMappingOBJ =
						DmReqResMapping::GetInstance();

				std::map<std::string, DMFinalResMessage>::iterator DMFinalResponseItr =
						DmReqResMappingOBJ->DMFinalResponse.find(
								MessageDataOBJ.Notif_Id);
				if (DMFinalResponseItr
						!= DmReqResMappingOBJ->DMFinalResponse.end()) {

					syslog(LOG_INFO,
							"[DM]: Response  queue process multiple response command: [%s] notif_id: [%s] response: [%s]",
							MessageDataOBJ.Command.c_str(),
							MessageDataOBJ.Notif_Id.c_str(),
							json_object_get_string(
									DMFinalResponseItr->second.allResponse));

					if (json_object_get_type(
							DMFinalResponseItr->second.allResponse)
							== json_type_array) {

						DMFinalResponseItr->second.response_success_count = 0;
						DMFinalResponseItr->second.response_fail_count = 0;

						json_type jsontypeofDMFinalResponseItr =
								json_object_get_type(
										DMFinalResponseItr->second.allResponse);

						/** Checking whether the json structure of Response of DMFinalResponseItr is array or not */
						if (jsontypeofDMFinalResponseItr == json_type_array) {
							DMFinalResponseItr->second.total_count =
									json_object_array_length(
											DMFinalResponseItr->second.allResponse);
						} else {
							DMFinalResponseItr->second.total_count =
							ZERO_LENGTH;
						}

						/*syslog(LOG_INFO,
								"[DM]: Response  queue process count before success [%d] fail [%d] all [%d]",
								DMFinalResponseItr->second.response_success_count,
								DMFinalResponseItr->second.response_fail_count,
								DMFinalResponseItr->second.total_count);*/

						for (int messageSize = 0;
								messageSize
										< DMFinalResponseItr->second.total_count;
								messageSize++) {

							json_object* TotalData = json_object_array_get_idx(
									DMFinalResponseItr->second.allResponse,
									messageSize);

							json_object* status_code = json_object_object_get(
									TotalData, "status_code");

							if (json_object_get_int(status_code) == DM_SUCCESS) {

								DMFinalResponseItr->second.response_success_count =
										DMFinalResponseItr->second.response_success_count
												+ 1;

								/*syslog(LOG_INFO,
										"[DM]: Response queue process count check success [%d] fail [%d] all [%d]",
										DMFinalResponseItr->second.response_success_count,
										DMFinalResponseItr->second.response_fail_count,
										DMFinalResponseItr->second.total_count);*/

							} else {

								DMFinalResponseItr->second.response_fail_count =
										DMFinalResponseItr->second.response_fail_count
												+ 1;

								/*syslog(LOG_INFO,
										"[DM]: Response queue process count check success [%d] fail [%d] all [%d]",
										DMFinalResponseItr->second.response_success_count,
										DMFinalResponseItr->second.response_fail_count,
										DMFinalResponseItr->second.total_count);*/

							}
						}

						/*syslog(LOG_INFO,
								"[DM]: Response queue process count after success [%d] fail [%d] all [%d]",
								DMFinalResponseItr->second.response_success_count,
								DMFinalResponseItr->second.response_fail_count,
								DMFinalResponseItr->second.total_count);*/

						if (DMFinalResponseItr->second.response_success_count
								== DMFinalResponseItr->second.total_count) {
							/*syslog(LOG_INFO,
									"[DM]: Response queue process control successfully");*/
							RULE_SCENE_AREA_STATUS = "SUCCESS";

						} else if (DMFinalResponseItr->second.response_fail_count
								== DMFinalResponseItr->second.total_count
								&& DMFinalResponseItr->second.response_success_count
										!= DMFinalResponseItr->second.total_count) {

							/*syslog(LOG_INFO,
									"[DM]: Response queue process control failed");*/
							RULE_SCENE_AREA_STATUS = "FAIL";

						} else {
							/*syslog(LOG_INFO,
									"[DM]: Response queue process control partially failed");*/
							RULE_SCENE_AREA_STATUS = "PARTIAL";
						}

						syslog(LOG_INFO,
								"[DM]: Response queue process after success [%d] fail [%d] all [%d] RULE/SCENE/AREA STATUS :%s",
								DMFinalResponseItr->second.response_success_count,
								DMFinalResponseItr->second.response_fail_count,
								DMFinalResponseItr->second.total_count,RULE_SCENE_AREA_STATUS.c_str());

					} else {

						syslog(LOG_ERR,
								"[DM]: Response queue process Response Object is not array");
					}

					switch (std::stol(MessageDataOBJ.Message_Id.substr(2, 2))) {

					case AREA_CONTROL: {

						/*syslog(LOG_INFO,
								"[DM]: Response  queue process response sending request-response print = client area_control: [%s]",
								MessageDataOBJ.Message_Id.c_str());*/

						MessageDataOBJ.Command = DM_AREA_CONTROL;
						MessageDataOBJ.Client = AREA_CONTROL;
						MessageDataOBJ.Status_Code = DM_SUCCESS;
						MessageDataOBJ.RuleSceneData = RULE_SCENE_AREA_STATUS;
						MessageDataOBJ.Message_Id = std::to_string(
						DM_OBSERVE_MSG_ID);

						DmPlatform::DmSendingRequestResp(MessageDataOBJ);
					}
						break;

					case SCENE_CONTROL: {

						/*syslog(LOG_INFO,
								"[DM]: Response  queue process response sending request-response print = client scene_control: [%s]",
								MessageDataOBJ.Message_Id.c_str());*/

//						MessageDataOBJ.Command = DM_SCENE_CONTROL;
//						MessageDataOBJ.Client = SCENE_CONTROL;
//						MessageDataOBJ.Status_Code = DM_SUCCESS;
//						MessageDataOBJ.RuleSceneData = RULE_SCENE_AREA_STATUS;
//						MessageDataOBJ.Message_Id = std::to_string(
//						DM_OBSERVE_MSG_ID);
//
//						DmPlatform::DmSendingRequestResp(MessageDataOBJ);
					}
						break;

					case RULE_COLTROL: {

						/*syslog(LOG_INFO,
								"[DM]: Response  queue process response sending request-response print = client rule_coltrol: [%s]",
								MessageDataOBJ.Message_Id.c_str());*/

						MessageDataOBJ.Command = DM_RULE_COLTROL;
						MessageDataOBJ.Client = RULE_COLTROL;
						MessageDataOBJ.Status_Code = DM_SUCCESS;
						MessageDataOBJ.RuleSceneData = RULE_SCENE_AREA_STATUS;
						MessageDataOBJ.Message_Id = std::to_string(
						DM_OBSERVE_MSG_ID);

						DmPlatform::DmSendingRequestResp(MessageDataOBJ);
					}
						break;

					default: {
						syslog(LOG_INFO,
								"[DM]: Response  queue process response sending request-response print = client default: [%s]",
								MessageDataOBJ.Message_Id.c_str());
					}
						break;

					}

#if DM_SEND_MULTIPLE_RESP == 1

					syslog(LOG_INFO,
							"[DM]: Response  queue process response send data to tronx: [%s]",
							MessageDataOBJ.Thing_Response.c_str());

					DmSendDataTronx* DmSendDataTronxObj =
							DmSendDataTronx::GetInstance();

					ReqMessageData ReqMessageDataOBJ;

//					ReqMessageDataOBJ.ReponseData = json_object_get_string(
//							DMFinalResponseItr->second.allResponse);

					ReqMessageDataOBJ.ReponseData =
							MessageDataOBJ.Thing_Response;

					ReqMessageDataOBJ.ResponseMethod = MessageDataOBJ.Req_Type;
					ReqMessageDataOBJ.ResponseURI = MessageDataOBJ.Req_URI;
					ReqMessageDataOBJ.Thing_ID = MessageDataOBJ.Thing_Id;
					ReqMessageDataOBJ.Command = MessageDataOBJ.Command;
					ReqMessageDataOBJ.priority = PRORITY_VALUE;

					ReqMessageDataOBJ.queue_id = MessageDataOBJ.Thing_Id
							+ MessageDataOBJ.Entity_Id
							+ MessageDataOBJ.Property_Name;

					DmSendDataTronxObj->DmSendAddMessage(ReqMessageDataOBJ);
#endif

					DmReqResMappingOBJ->DMFinalResponse.erase(
							MessageDataOBJ.Notif_Id);
				}

			} else {

				syslog(LOG_DEBUG,
						"[DM]: Response  queue process response waiting to TronX with notif_id: [%s] message_id: [%s] send flag: [%d]",
						MessageDataOBJ.Notif_Id.c_str(),
						MessageDataOBJ.Message_Id.c_str(),
						MessageDataOBJ.SendFlag);
			}
			}
			else {

						//SENDING OBSERVE RESPONSE
						DmSendDataTronx* DmSendDataTronxObj =
								DmSendDataTronx::GetInstance();
						ReqMessageData ReqMessageDataOBJ;

						ReqMessageDataOBJ.ReponseData = MessageDataOBJ.Thing_Response;
						ReqMessageDataOBJ.ResponseMethod = MessageDataOBJ.Req_Type;
						ReqMessageDataOBJ.ResponseURI = MessageDataOBJ.Req_URI;
						ReqMessageDataOBJ.Thing_ID = MessageDataOBJ.Thing_Id;
						ReqMessageDataOBJ.priority = PRORITY_VALUE;

						ReqMessageDataOBJ.queue_id = MessageDataOBJ.Thing_Id
								+ MessageDataOBJ.Entity_Id + MessageDataOBJ.Property_Name;

						syslog(LOG_INFO,
								"[DM]: Response  queue process observe response sending to tronx : [%s]",
								MessageDataOBJ.Thing_Response.c_str());

						DmSendDataTronxObj->DmSendAddMessage(ReqMessageDataOBJ);
					}
		} else {

			//SENDING OBSERVE RESPONSE
			DmSendDataTronx* DmSendDataTronxObj =
					DmSendDataTronx::GetInstance();
			ReqMessageData ReqMessageDataOBJ;

			ReqMessageDataOBJ.ReponseData = MessageDataOBJ.Thing_Response;
			ReqMessageDataOBJ.ResponseMethod = MessageDataOBJ.Req_Type;
			ReqMessageDataOBJ.ResponseURI = MessageDataOBJ.Req_URI;
			ReqMessageDataOBJ.Thing_ID = MessageDataOBJ.Thing_Id;
			ReqMessageDataOBJ.priority = PRORITY_VALUE;

			ReqMessageDataOBJ.queue_id = MessageDataOBJ.Thing_Id
					+ MessageDataOBJ.Entity_Id + MessageDataOBJ.Property_Name;

			syslog(LOG_INFO,
					"[DM]: Response  queue process observe response sending to tronx : [%s]",
					MessageDataOBJ.Thing_Response.c_str());

			DmSendDataTronxObj->DmSendAddMessage(ReqMessageDataOBJ);
		}
	} else {
//EMPTY MESSAGE ID
	}

}

