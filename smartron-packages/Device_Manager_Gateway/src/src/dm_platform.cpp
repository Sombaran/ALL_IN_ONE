#include "dm_platform.hpp"
#include "dm_framework.hpp"
#include "dm_thing.hpp"
#include "dm_thingmap.hpp"
#include "zb_framework.hpp"
#include "zw_framework.hpp"
#include "dm_zeromq.hpp"
#include "dm_led_interface.hpp"
#include "dm_request_queue.hpp"
#include "dm_response_queue.hpp"
#include "dm_response_queue.hpp"
#include "dm_things_backup_db.hpp"
#include "dm_send_data_tronx.hpp"
#include "dm_request_parse.hpp"
#include "dm_req_res_mapping.hpp"
#include "dm_secne_mapping.hpp"
#include "dm_rule_mapping.hpp"
#include "dm_utility.hpp"

/*
 * 1 ARM
 * 2 LINUX
 * */

#define DM_GET_ENV_PLATFROM  1
extern std::string removeThingID;
extern bool FlagDiscSend;
extern bool ExclusionFlag;
extern bool RemoveFlag;
extern bool ControlFlag;
extern bool FlagFota;
uint8_t ArmStatusFlag;
pthread_mutex_t ArmStatusLock;
//extern char errnum;
long long dm_Request, dm_Response;
extern uint8_t RuleStatusFlag;
extern pthread_mutex_t RuleStatusLock;

char DM_DISCOVERY_TIMEOUT[8] = "";
char DM_REMOVE_TIMEOUT[8] = "";
char DM_CONTROL_TIMEOUT[8] = "";
char DM_EXCLUSION_TIMEOUT[8] = "";
char DM_STACK_CHECK[8] = "";
char DM_KILL_SWITCH[32] = "";
char DM_ZBFOTA_TIMEOUT[8] = "420";

#if DEMO == 1
//For Demo
char entrance_door_sensor_id[32] = "";
char exit_door_sensor_id[32] = "";
char entrance_door_sensor_value[32] = "";
char exit_door_sensor_value[32] = "";
char kill_switch_device_id[32] = "";
#endif

#define  WAIT_TIME_SECONDS_DISCOVERY   		65
#define  WAIT_TIME_SECONDS_CONTROL   		7
#define  WAIT_TIME_SECONDS_REMOVE   		42
#define  WAIT_TIME_SECONDS_EXCLUSION   		65
#define  WAIT_TIME_MINUTES_ZIGBEEFOTA		420
const int FOTA_PORT = 7771; /* Sending port number of the Socket for end device FOTA */
const int DM_PORT = 7773; /* Receive port number of the Socket for end device FOTA */
const int MAXLINE = 1024; /* Max size of the incoming message from FOTA module */
int sockfd; /* Socket discriptor */
struct sockaddr_in fota_servaddr, dm_addr; /* Socket structure objects */
pthread_mutex_t LockMutexDisc = PTHREAD_MUTEX_INITIALIZER; //DISCOVERY MUTEX

pthread_mutex_t DM_Req_Mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t DM_Req_Discovery;
pthread_cond_t DM_Req_Control;
pthread_cond_t DM_Req_Remove;
pthread_cond_t DM_Req_Exclusion;
pthread_cond_t DM_Req_ForceRemove;
pthread_cond_t DM_Req_LearnMode;
pthread_cond_t DM_Req_ZigbeeFota;

extern void Audio_play();
extern void Audio_pause();

/*volume---for buzzer start*/
#define CMD_GET_VOLUME_SHIFU    "amixer -c 0 cget numid=7 | grep \": values\" |  awk -F \",\"  '\{ print $2 }'"
#define CMD_SET_VOLUME_SHIFU    "amixer -c 0 sset 'DAC1' %d"
#define VOLUME_MAX 72

int volume_present = 0;
int va_on = 1;
int get_volume(void) { /* function to get the present volume from shifu*/
//    int i = 0;
	FILE *fd = NULL;
	int volume = 0;
	char vol[4] = { 0 };

	fd = popen(CMD_GET_VOLUME_SHIFU, "r");
	fgets(vol, 4, fd);
	pclose(fd);
	volume = atoi(vol);
	return volume;
}

void set_volume(int volume_in_dB) { /* Set volume to high*/

	char amixer_cmd[32] = { 0 };
	snprintf(amixer_cmd, sizeof(amixer_cmd), CMD_SET_VOLUME_SHIFU,
			volume_in_dB);
	system(amixer_cmd);
}

/*volume---for buzzer end*/

/*
 DmPlatform::DmPlatform() {
 }
 DmPlatform::~DmPlatform() {
 }
 */

void DmPlatform::DmReceiveRequest() {

	syslog(LOG_INFO,
			"[DM]: Dm_platform start to receive data from /'SS/' /'MA/' /'CJ/' ");

	DmZeroMq *DmZeroMqOBJ = DmZeroMq::GetInstance();

	bool DMZMQStatus = DmZeroMqOBJ->DMZeroMQInit();

	while (DMZMQStatus) {

		DmZeroMqOBJ->ZeroMessageQueueRcvRequest();
	}
}

void DmPlatform::DmProcessIOTRequest() {

	int Operation; /* Var to store the command in the request object*/

	syslog(LOG_INFO,
			"[DM]: Dm_platform start processing request_queue thread Started ");

	DmRequestQueue *DmRequestQueueObj = DmRequestQueue::GetInstance();

	bool StartRequestQueue = DmRequestQueueObj->DMReqQueueInit();

	/*syslog(LOG_INFO, "[DM]: DM_platform request_queue processing status:[%d]",
	 StartRequestQueue);*/

	struct timespec DiscoveryTs;
	struct timespec ControlTs;
	struct timespec RemoveTs;
	struct timespec ExclusionTs;
	struct timespec ForceRemoveTs;
	struct timeval tp;
	struct timespec LearnModeTs;
	struct timespec ZigbeeFotaTs;

	while (StartRequestQueue) {

		/*syslog(LOG_DEBUG,
		 "[DM]: DM_platform request_queue processing in \"while(StartRequestQueue)\" ");*/

		DmRequestQueue *DmRequestQueueObj = DmRequestQueue::GetInstance();
		RequestResponse DmRequestQueueOBJ;
		DmRequestQueueOBJ = DmRequestQueueObj->DMReqQueueReceiveMessage(); /* Fetching the request object from the process vector Queue */
		Operation = ConvertStringToInt(DmRequestQueueOBJ.Command);
		syslog(LOG_DEBUG,
				"[DM]: Dm_platform request_queue for Operation :[%s] ",
				DmRequestQueueOBJ.Command.c_str());

		switch (Operation) {

		case DISCOVERY: {
			//--------------------------------2ndthread_DISCOVERY_STARTS-------------------------------//

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform request_queue for device discovery ");

			FlagDiscSend = true;

			DmThing::ThingDiscovery(DmRequestQueueOBJ);

			gettimeofday(&tp, NULL);
			DiscoveryTs.tv_sec = tp.tv_sec;
			DiscoveryTs.tv_sec += WAIT_TIME_SECONDS_DISCOVERY;

			dm_Request = 0;
			dm_Response = 0;

			dm_Request = CurrentZWReqTime();

			/*syslog(LOG_INFO,
			 "[DM]: DM_platform request_queue processing discovery request time: [%lld] ",
			 dm_Request);*/

			//syslog(LOG_INFO, "[DM]: Platform calling led-discovery pattern");
			DmLedInterface::DiscoveryStart();

			/*syslog(LOG_DEBUG,
			 "[DM]: DM_platform request_queue discovery is lock during \"mutex\" ");*/

			pthread_cond_timedwait(&DM_Req_Discovery, &DM_Req_Mutex,
					&DiscoveryTs);
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform /'discoveryFlag/' status: [%d]",
					FlagDiscSend);

		}
			break;
			//--------------------------------2ndthread_DISCOVERY_END-------------------------------//

		case CONTROL: {
			//--------------------------------2ndthread_CONTROL_STARTS-------------------------------//

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform request_queue for device control ");

			syslog(LOG_DEBUG,
					"[DM] : [AUTO] : Dm_platform REQUEST STATUS_CODE:[%d], THING_ID:[%s], ENTITY_ID:[%s],"
							" PROPERTY_NAME:[%s] PROPERTY_VALUE:[%s]",
					DmRequestQueueOBJ.Status_Code,
					DmRequestQueueOBJ.Thing_Id.c_str(),
					DmRequestQueueOBJ.Entity_Id.c_str(),
					DmRequestQueueOBJ.Property_Name.c_str(),
					DmRequestQueueOBJ.Property_Value.c_str());

			if (DmRequestQueueOBJ.Message_Id.substr(0, 2) == "88") {
				RuleStatusFlag = 1;
			} else if (DmRequestQueueOBJ.Message_Id.substr(0, 2) == "99") {
//				DmSceneMapping::DMSetSceneMapExecuteflag(true);
			} else {
				ControlFlag = true;
			}

			bool controlState = DmThing::ThingControl(DmRequestQueueOBJ);
			/*syslog(LOG_INFO,
			 "[DM]: DM_platform request_queue processing control request sent status : [%d] ",
			 controlState);*/

			gettimeofday(&tp, NULL);
			ControlTs.tv_sec = tp.tv_sec;
			ControlTs.tv_sec += WAIT_TIME_SECONDS_CONTROL;

			dm_Request = 0;
			dm_Response = 0;

			dm_Request = CurrentZWReqTime();

			/*syslog(LOG_INFO,
			 "[DM]: DM_platform request_queue processing control request time: [%lld] ",
			 dm_Request);*/

			/*syslog(LOG_DEBUG,
			 "[DM]: DM_platform request_queue control is lock during \"mutex\" ");*/

			/**if thread created in ThingControl for control then only calling conditional wait*/
			if (controlState == true)
				pthread_cond_timedwait(&DM_Req_Control, &DM_Req_Mutex,
						&ControlTs);

			ControlFlag = false;

			syslog(LOG_DEBUG, "[DM]: Dm_platform /'controlFlag/' status: [%d]",
					ControlFlag);

//			if ((DmRequestQueueOBJ.Message_Id.substr(0, 2) != "88")
//					&& (DmRequestQueueOBJ.Message_Id.substr(0, 2) != "99")
//					&& (DmRequestQueueOBJ.Message_Id.substr(0, 2) != "77")) {
//				ControlFlag = false;
//			}

		}
			break;
			//--------------------------------2ndthread_CONTROL_END-------------------------------//

		case REMOVE: {
			//--------------------------------2ndthread_REMOVE_STARTS-------------------------------//

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform request_queue for device remove ");

			RemoveFlag = true; //Remove process check flag

			bool remove = DmThing::ThingRemove(DmRequestQueueOBJ);

			gettimeofday(&tp, NULL);
			RemoveTs.tv_sec = tp.tv_sec;
			RemoveTs.tv_sec += WAIT_TIME_SECONDS_REMOVE;

			dm_Request = 0;
			dm_Response = 0;

			dm_Request = CurrentZWReqTime();

			/*syslog(LOG_INFO,
			 "[DM]: DM_platform request_queue processing remove request time: [%lld] ",
			 dm_Request);

			 syslog(LOG_DEBUG,
			 "[DM]: DM_platform request_queue remove is lock during \"mutex\" ");*/

			if (remove)
				pthread_cond_timedwait(&DM_Req_Remove, &DM_Req_Mutex,
						&RemoveTs);

			RemoveFlag = false;
			syslog(LOG_DEBUG, "[DM]: Dm_platform /'removeFlag/' status: [%d]",
					RemoveFlag);

		}
			break;
			//--------------------------------2ndthread_REMOVE_END-------------------------------//

		case EXCLUSION: {
			//--------------------------------2ndthread_EXCLUSION_STARTS-------------------------------//

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform request_queue for device exclusion ");

			ExclusionFlag = true; //Exclusion process check flag

			DmThing::ThingExclusion(DmRequestQueueOBJ);

			gettimeofday(&tp, NULL);
			ExclusionTs.tv_sec = tp.tv_sec;
			ExclusionTs.tv_sec += WAIT_TIME_SECONDS_EXCLUSION;

			dm_Request = 0;
			dm_Response = 0;

			dm_Request = CurrentZWReqTime();

			/*syslog(LOG_INFO,
			 "[DM]: DM_platform request_queue processing exclusion request time: [%lld] ",
			 dm_Request);

			 syslog(LOG_DEBUG,
			 "[DM]: DM_platform request_queue exclusion is lock during \"mutex\" ");*/

			pthread_cond_timedwait(&DM_Req_Exclusion, &DM_Req_Mutex,
					&ExclusionTs);

			ExclusionFlag = false;
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform /'exclusionFlag/' status: [%d]",
					ExclusionFlag);

		}
			break;
			//--------------------------------2ndthread_EXCLUSION_END-------------------------------//

		case FACTORY_RESET: {
			//--------------------------------2ndthread_FACTORY_RESET_STARTS-------------------------------//

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform request_queue for device factory reset ");
			system("firstboot && reboot &");

		}
			break;
			//--------------------------------2ndthread_FACTORY_RESET_END-------------------------------//

		case IOT_RESET: {
			//--------------------------------2ndthread_IOT_RESET_STARTS-------------------------------//

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform request_queue for device IOT reset ");

			DmThingsBackupDb *DmThingsBackupDbObj =
					DmThingsBackupDb::GetInstance();

			bool iotReset = DmThingsBackupDbObj->ClearBackUpDatabase();
			if (iotReset == true) {
				/*syslog(LOG_INFO,
				 "[DM]: DM_platform resetting \"IOT\" devices status: [%d]",
				 iot_reset);*/

				syslog(LOG_DEBUG,
						"[DM]: Dm_platform  calling iot_reset and reboot script");
				/* Stopping the Voice_Service and playing the Announcement with maximum volume*/
				system("/etc/init.d/va_service stop");
				/* Stopping the Led service*/
				system("/etc/init.d/led_server stop");
				system("i2cset -y 0 0x18 0x41");
				/* #22366:Setting volume */
				//system("amixer -c0 sset DAC1 64");
				system("sh /announcements/setannouncementvolume.sh");
				system(
						"gst-launch-1.0 playbin uri=file:///announcements/iot_reset.mp3 audio-sink=alsasink &");
				system("iot_reset && reboot &");

			} else {
				syslog(LOG_DEBUG,
						"[DM]: Dm_platform resetting \"IOT\" devices status: [%d]",
						iotReset);
			}
			syslog(LOG_DEBUG, "[DM]: Dm_platform /'IotReset/' status: [%d]",
					iotReset);

		}
			break;
			//--------------------------------2ndthread_IOT_RESET_END-------------------------------//

		case S2_SECURITY: {
			//--------------------------------2ndthread_S2_SECURITY_STARTS-------------------------------//

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform request_queue for device S2_SECURITY ");
			DmThing::ThingS2secutity(DmRequestQueueOBJ);
		}
			pthread_cond_timedwait(&DM_Req_Discovery, &DM_Req_Mutex,
					&DiscoveryTs);
			break;
			//--------------------------------2ndthread_S2_SECURITY_END-------------------------------//

		case FORCE_REMOVE: {

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform request_queue for device FORCE REMOVE");
			DmThing::ThingForceRemove(DmRequestQueueOBJ);
			gettimeofday(&tp, NULL);
			ForceRemoveTs.tv_sec = tp.tv_sec;
			ForceRemoveTs.tv_sec += WAIT_TIME_SECONDS_EXCLUSION;

			dm_Request = 0;
			dm_Response = 0;

			dm_Request = CurrentZWReqTime();

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform request queue processing exclusion request time: [%lld] ",
					dm_Request);
			pthread_cond_timedwait(&DM_Req_ForceRemove, &DM_Req_Mutex,
					&ForceRemoveTs);
		}

			break;

		case FOTA_REQ: { /* Case to send fota request to Zigbee*/

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform request_queue for FOTA -> Device manager| FOTA to Zigbee. ");
			FlagFota = true; /* Set the fota execution flag */
			std::string str = "cp " + DmRequestQueueOBJ.Firmware_path
					+ " /exe_dir/ota-files/"; /* Copying file to Zigbee default directory */
			system(str.c_str());

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform FOTA -> Firmware Path given to DM is: [%s]",
					DmRequestQueueOBJ.Firmware_path.c_str());
			DmThing::ZigbeeFota(DmRequestQueueOBJ); /* initiating firmware update request to ZB module*/
			gettimeofday(&tp, NULL);
			ZigbeeFotaTs.tv_sec = tp.tv_sec;
			ZigbeeFotaTs.tv_sec += WAIT_TIME_MINUTES_ZIGBEEFOTA;
			dm_Request = 0;
			dm_Response = 0;
			dm_Request = CurrentZWReqTime();
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform FOTA -> Platform request queue processing ZB FOTA time: [%lld] ",
					dm_Request);
//			syslog(LOG_INFO,
//					"[DM]: FOTA -> Platform request ZB FOTA lock dm_request_condition mutex");

			/*pthread_cond_timedwait(&DM_Req_ZigbeeFota, &DM_Req_Mutex,
			 &ZigbeeFotaTs);
			 FlagFota = false;*/
			//related to 21878
			/* Not waiting while FOTA is in process and allowing the next request in Queue, Making the Flag false while sending response to Client */

		}

			break;

		case LEARN_MODE: {
			//--------------------------------2ndthread_EXCLUSION_STARTS-------------------------------//
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform request_queue for device Learn_mode ");
			DmThing::ThingLearnMode(DmRequestQueueOBJ);
			gettimeofday(&tp, NULL);
			LearnModeTs.tv_sec = tp.tv_sec;
			LearnModeTs.tv_sec += WAIT_TIME_SECONDS_CONTROL;
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform request_queue for device Learn_mode %ld ",
					LearnModeTs.tv_sec);
			dm_Request = 0;
			dm_Response = 0;
			dm_Request = CurrentZWReqTime();
//			syslog(LOG_DEBUG,
//					"[DM]: DM_platform request_queue for device before  Learn_mode ");
			pthread_cond_timedwait(&DM_Req_LearnMode, &DM_Req_Mutex,
					&LearnModeTs);
//			syslog(LOG_DEBUG,
//					"[DM]: DM_platform request_queue for device after Learn_mode ");
		}
			break;
		default:
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform thread created for /'Learn_mode/' in default");
			break;

		}
	}
	/*	syslog(LOG_INFO,
	 * 	 "[DM]: DM_platform start processing request_queue thread Finished ");*/
}

void DmPlatform::DmProcessTronxResponse() {

	syslog(LOG_INFO,
			"[DM]: Dm_platform /'SEND RESPONSE TO TRONX VECTOR/' queue processing ");
	DmSendDataTronx* DmSendDataTronxOBJ = DmSendDataTronx::GetInstance();
	bool StartSendDataTronxQueue = DmSendDataTronxOBJ->DmSendDataTronxInit();
	/*syslog(LOG_DEBUG,
	 "[DM]: DM_platform send response to tronx queue processing status:[%d]",
	 StartSendDataTronxQueue);*/
	bool StartCurlStatus = DmSendDataTronxOBJ->CurlInit();
	syslog(LOG_DEBUG,
			"[DM]: Dm_platform send response to tronx queue curl init status:[%d]",
			StartCurlStatus);
	while (StartSendDataTronxQueue) {
		/*syslog(LOG_INFO,
		 "[DM]: DM_platform sendresponsetronx queue processing calling dm request queue listen");*/
		DmSendDataTronxOBJ->DmSendReceiveMessage();
	}
}

void DmPlatform::DmProcessIOTResponse() {

	syslog(LOG_INFO, "[DM]: Dm_platform /'RESPONSE QUEUE VECTOR/' processing ");
	DmResponseQueue *DmResponseQueueOBJ = DmResponseQueue::GetInstance();
	bool StartRequestQueue = DmResponseQueueOBJ->DMResQueueInit();
	/*syslog(LOG_DEBUG, "[DM]: Platform response queue processing status:[%d]",
	 StartRequestQueue);*/
	while (StartRequestQueue) {
		DmResponseQueue* DmResponseQueueOBJ = DmResponseQueue::GetInstance();
		/*syslog(LOG_INFO,
		 "[DM]: DM_platform response queue processing calling dm request queue listen");*/
		DmResponseQueueOBJ->DMResQueueReceiveMessage();
	}
}

void DmPlatform::DmZigbeeReceive() {

	syslog(LOG_INFO, "[DM]: Dm_platform /'ZIGBEE_IPC_MESSAGE_QUEUE/' receive");
	ZbFramework::ZbReceive();
}

void DmPlatform::DmZwaveReceive() {

	syslog(LOG_INFO, "[DM]: Dm_platform /'ZWAVE_IPC_MESSAGE_QUEUE/' receive ");
	ZwFramework::ZwReceive();
}

bool DmPlatform::DMDeviceDetailsUpdate(std::string Thing_id) {

	syslog(LOG_INFO,
			"[DM]: Dm_platform update device details in /'DMBACKUPDB/' ");
	DmThingMap* DmThingMapObj = DmThingMap::GetInstance();
	DmThingsBackupDb* DmThingsBackupDbOBJ = DmThingsBackupDb::GetInstance();
	std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
			DmThingMapObj->DmThingDataMap.find(Thing_id);
	if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {
		DMThingDB DMThingDBOBJ;
		DMThingDBOBJ.THING_ID = DmThingMapObjItr->second->getthing_id();
		DMThingDBOBJ.FIRMWARE_VERSION =
				DmThingMapObjItr->second->getfirmwareVersion();
		DMThingDBOBJ.FRAMEWORK = DmThingMapObjItr->second->getframeworkName();
		DMThingDBOBJ.HARDWARE_VERSION =
				DmThingMapObjItr->second->gethardwareVersion();
		DMThingDBOBJ.MANUFACTURER_NAME =
				DmThingMapObjItr->second->getmanufacturerName();
		DMThingDBOBJ.MANUFACTURER_URL =
				DmThingMapObjItr->second->getmanufacturerUrl();
		DMThingDBOBJ.THING_TYPE = DmThingMapObjItr->second->getthing_type();
		DMThingDBOBJ.THING_NAME = DmThingMapObjItr->second->getthing_name();
		DMThingDBOBJ.THING_STATUS = DmThingMapObjItr->second->getthing_status();
		DMThingDBOBJ.BATTERY_STATUS =
				DmThingMapObjItr->second->getbattery_status();
		DMThingDBOBJ.ZONE_TYPE = DmThingMapObjItr->second->getzone_type();
		DMThingDBOBJ.HOME_NODE_ID = DmThingMapObjItr->second->gethome_node_id();
		DmThingsBackupDbOBJ->InsertBackUpDeviceDetails(DMThingDBOBJ);
	}
	return true;
}

bool DmPlatform::DMNodeIdUpdateInDB(std::string Thing_id, std::string Node_id) {

	syslog(LOG_INFO,
			"[DM]: Dm_platform update NODE details for NON-DISCOVERY in /'DMBACKUPDB/' ");
	DmThingsBackupDb* DmThingsBackupDbOBJ = DmThingsBackupDb::GetInstance();
	DmThingsBackupDbOBJ->DMUpdateNodeIdTable(Thing_id,
			"NODE_ID=\'" + Node_id + "\'");
	return true;
}

#if DEMO == 1
//For Demo
void DemoCheck(std::string sensor_id, std::string proeprty_name,
		std::string entity_id, std::string stack_prp_value) {

	DmZeroMq* DmZeroMqOBJ = DmZeroMq::GetInstance();

	syslog(LOG_DEBUG,
			"[DM] : DM_platform DemoCheck IN sensor_id :[%s] entity_id :[%s] proeprty_name :[%s] stack_prp_value :[%s]",
			sensor_id.c_str(), entity_id.c_str(), proeprty_name.c_str(),
			stack_prp_value.c_str());

	syslog(LOG_DEBUG,
			"[DM] : DM_platform DemoCheck IN entrance_door_sensor_id :[%s] entrance_door_sensor_value :[%s]",
			std::string(entrance_door_sensor_id).c_str(),
			std::string(entrance_door_sensor_value).c_str());

	if (sensor_id == std::string(entrance_door_sensor_id)
			&& stack_prp_value == std::string(entrance_door_sensor_value)
			&& proeprty_name == "Contact Detect") {

		/*	syslog(LOG_DEBUG,
		 "[DM] : DM_platform DemoCheck IN entrance_door_sensor_id and entrance_door_sensor_value if case");*/

		json_object* va_response = json_object_new_object();
		json_object* client_data = json_object_new_object();
		std::string va_act_data = "welcome_home.mp3";
		json_object * action_type = json_object_new_string(va_act_data.c_str());

		json_object_object_add(client_data, "offline", action_type);
		json_object * led_obj = json_object_new_boolean(true);
		json_object_object_add(client_data, "led", led_obj);

		json_object * client_name = json_object_new_string("DM");
		json_object * client_action = json_object_new_string("VOICE");

		json_object_object_add(va_response, "client_name", client_name);
		json_object_object_add(va_response, "client_action", client_action);
		json_object_object_add(va_response, "client_data", client_data);

		std::string dm_va_response = json_object_to_json_string(va_response);
		/*syslog(LOG_DEBUG,
		 "[DM] : DM_platform DemoCheck Client data RESPONSE TO VoiceAssistant :[%s]",
		 json_object_to_json_string(client_data));*/

		/*syslog(LOG_DEBUG,
		 "[DM] : DM_platform DemoCheck RESPONSE TO VoiceAssistant :[%s]",
		 json_object_to_json_string(va_response));*/

		size_t msgSize = strlen(json_object_to_json_string(va_response));

		int ResultSend = zmq_send(DmZeroMqOBJ->CronHandlerResponse,
				json_object_to_json_string(va_response), msgSize, 0);

		syslog(LOG_DEBUG,
				"[DM] : DM_platform DemoCheck ZMQ SEND for VoiceAssistant :[%d]",
				ResultSend);

		if (ResultSend == -1) {
			syslog(LOG_INFO,
					"[DM] : DM_platform DemoCheck ZMQ SEND for VoiceAssistant: RESTART ZMQ");
//			int zmq_close(DmZeroMqOBJ->CronHandlerResponse);
//			zmqRpcSendTestVA();
		}
		char HM_ACK[128] = {0};
		int ResultRecv = zmq_recv(DmZeroMqOBJ->CronHandlerResponse, HM_ACK,
				128, 0);

		syslog(LOG_DEBUG,
				"[DM] : DM_platform DemoCheck ZMQ rcv from HM :[%d]",
				ResultRecv);

		/*syslog(LOG_DEBUG,
		 "[DM] : DM_platform DemoCheck THING JSON ACK RECEIVED FROM THE HOME MANAGER :[%s]",
		 HM_ACK);*/

		if (ResultRecv == -1) {
			syslog(LOG_INFO,
					"[DM] : DM_platform DemoCheck ZMQ rcv from HM: RESTART ZMQ");
//			int zmq_close(DmZeroMqOBJ->CronHandlerResponse);
//			zmqRpcSendTestVA();
		}
		////json_object_put(va_response);
	}

	if (sensor_id == std::string(exit_door_sensor_id)
			&& stack_prp_value == std::string(exit_door_sensor_value)
			&& proeprty_name == "Contact Detect") {

		syslog(LOG_DEBUG,
				"[DM] : DM_platform DemoCheck exit_door_sensor_id:[%s] exit_door_sensor_value:[%s]",
				std::string(exit_door_sensor_id).c_str(),
				std::string(exit_door_sensor_value).c_str());

		json_object* va_response = json_object_new_object();
		json_object* client_data = json_object_new_object();

		json_object * client_name = json_object_new_string("DM");
		json_object * client_action = json_object_new_string("BUZZER");

		json_object_object_add(client_data, "offline", client_action);
		json_object * led_obj = json_object_new_boolean(true);
		json_object_object_add(client_data, "led", led_obj);

		json_object_object_add(va_response, "client_name", client_name);
		json_object_object_add(va_response, "client_action", client_action);
		json_object_object_add(va_response, "client_data",
				json_object_new_string("PLAY"));

		std::string dm_va_response = json_object_to_json_string(va_response);

		syslog(LOG_DEBUG,
				"[DM] : DM_platform DemoCheck RESPONSE data TO VoiceAssistant :[%s]",
				json_object_to_json_string(va_response));

		size_t msgSize = strlen(json_object_to_json_string(va_response));

		int ResultSend = zmq_send(DmZeroMqOBJ->CronHandlerResponse,
				json_object_to_json_string(va_response), msgSize, 0);

		syslog(LOG_DEBUG,
				"[DM] : DM_platform DemoCheck RESPONSE sent to VA Status :[%d]",
				ResultSend);

		if (ResultSend == -1) {
			/*syslog(LOG_INFO,
			 "[DM] : DM_platform DemoCheck ZMQ send status MA: RESTART ZMQ MA");*/
//			int zmq_close(DmZeroMqOBJ->CronHandlerResponse);
//			zmqRpcSendTestVA();
		}
		char HM_ACK[128] = {0};
		int ResultRecv = zmq_recv(DmZeroMqOBJ->CronHandlerResponse, HM_ACK,
				128, 0);

		syslog(LOG_DEBUG,
				"[DM] : DM_platform DemoCheck ZMQ rcv from HM :[%d]",
				ResultRecv);

		/*syslog(LOG_DEBUG,
		 "[DM] : DM_platform DemoCheck ZMQ rcv from HM :[%s]",
		 HM_ACK);*/

		if (ResultRecv == -1) {
			/*	syslog(LOG_INFO,
			 "[DM] : DM_platform DemoCheck THING JSON ACK RECEIVED FROM THE MobileAgentResponse RESTART ZMQ MA");*/
//			int zmq_close(DmZeroMqOBJ->CronHandlerResponse);
//			zmqRpcSendTestVA();
		}
		////json_object_put(va_response);
	}

	syslog(LOG_DEBUG,
			"[DM] : DM_platform DemoCheck IN kill_switch_device_id :[%s]",
			std::string(kill_switch_device_id).c_str());
	if (sensor_id == std::string(kill_switch_device_id)
			&& proeprty_name == "OnOff") {

		json_object* va_response = json_object_new_object();
//		json_object* client_data = json_object_new_object();

		json_object * client_name = json_object_new_string("DM");
		json_object * client_action = json_object_new_string("BUZZER");

		json_object_object_add(va_response, "client_name", client_name);
		json_object_object_add(va_response, "client_action", client_action);
		json_object_object_add(va_response, "client_data",
				json_object_new_string("STOP"));

		std::string dm_va_response = json_object_to_json_string(va_response);

		syslog(LOG_DEBUG,
				"[DM] :  DM_platform DemoCheck RESPONSE TO VoiceAssistant :[%s]",
				json_object_to_json_string(va_response));

		size_t msgSize = strlen(json_object_to_json_string(va_response));

		int ResultSend = zmq_send(DmZeroMqOBJ->CronHandlerResponse,
				json_object_to_json_string(va_response), msgSize, 0);

		syslog(LOG_DEBUG,
				"[DM] :  DM_platform DemoCheck HOME MANAGER REQUEST RESPONSE ZMQ STATUS CronHandlerResponse :[%d]",
				ResultSend);

		if (ResultSend == -1) {
			/*syslog(LOG_INFO,
			 "[DM] : DM_platform DemoCheck JSON ACK RECEIVED FROM THE MobileAgentResponse RESTART ZMQ MA");*/
//			int zmq_close(DmZeroMqOBJ->CronHandlerResponse);
//			zmqRpcSendTestVA();
		}
		char HM_ACK[128] = {0};
		int ResultRecv = zmq_recv(DmZeroMqOBJ->CronHandlerResponse, HM_ACK,
				128, 0);

		syslog(LOG_DEBUG,
				"[DM] : DM_platform DemoCheck zmq_recv from HM :[%d]",
				ResultRecv);

		/*syslog(LOG_DEBUG,
		 "[DM] : DM_platform DemoCheck JSON ACK RECEIVED FROM THE HOME MANAGER :[%s]",
		 HM_ACK);*/

		if (ResultRecv == -1) {
			/*syslog(LOG_INFO,
			 "[DM] : DM_platform DemoCheck JSON ACK RECEIVED FROM THE MobileAgentResponse RESTART ZMQ MA");*/
//			int zmq_close(DmZeroMqOBJ->CronHandlerResponse);
//			zmqRpcSendTestVA();
		}
		////json_object_put(va_response);
	}
}
#endif

int getSecurityStatus(std::string thingid, std::string entity_id) {

	syslog(LOG_INFO, "[DM]: Dm_platform to get security Status ");
	DmThingMap* DmThingMapObj = DmThingMap::GetInstance();
	std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
			DmThingMapObj->DmThingDataMap.find(thingid);
	if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {
		int size = DmThingMapObjItr->second->getEntityListSize();
		for (int i = 0; i < size; i++) {
			if (DmThingMapObjItr->second->getEntityInfo(i)->getentity_id()
					== entity_id) {
				return DmThingMapObjItr->second->getEntityInfo(i)->getsecurity_level();
			}
		}
	}
	return 0;
}

void DmPlatform::DmSendingResponse(RequestResponse RequestResponseOBJ) {

	/*Printing whole data below while sending to TRONX*/
	syslog(LOG_INFO, "[DM]: Dm_platform /'SENDING-RESPONSE/' start ");
//	syslog(LOG_DEBUG,
//	 "[DM]: DM_platform sending response print = client: [%d] command: [%s] entity_id: [%s] message_id: [%s] message: [%s] notif_id: [%s] property_name: [%s] property_type: [%s] property_value: [%s] proeporty_range: [%s] rulescenedata: [%s] status_code: [%d] thing_id: [%s]",
//	 RequestResponseOBJ.Client, RequestResponseOBJ.Command.c_str(),
//	 RequestResponseOBJ.Entity_Id.c_str(),
//	 RequestResponseOBJ.Message_Id.c_str(),
//	 RequestResponseOBJ.Message.c_str(),
//	 RequestResponseOBJ.Notif_Id.c_str(),
//	 RequestResponseOBJ.Property_Name.c_str(),
//	 RequestResponseOBJ.Property_Type.c_str(),
//	 RequestResponseOBJ.Property_Value.c_str(),
//	 RequestResponseOBJ.Property_Range.c_str(),
//	 RequestResponseOBJ.RuleSceneData.c_str(),
//	 RequestResponseOBJ.Status_Code,
//	 RequestResponseOBJ.Thing_Id.c_str());

	DmThingMap* DmMsgNotifyMapObj = DmThingMap::GetInstance();
	if (DmMsgNotifyMapObj->DmMsgNotifyMap.empty()) {
		syslog(LOG_DEBUG, "[DM]: Dm_platform notify map is empty ");
	} else {
		std::map<std::string, std::string>::iterator DmMsgNotifyMapObjItr =
				DmMsgNotifyMapObj->DmMsgNotifyMap.find(
						RequestResponseOBJ.Message_Id);
		if (DmMsgNotifyMapObjItr != DmMsgNotifyMapObj->DmMsgNotifyMap.end()) {
			RequestResponseOBJ.Notif_Id = DmMsgNotifyMapObjItr->second;
			DmMsgNotifyMapObj->DmMsgNotifyMap.erase(
					RequestResponseOBJ.Message_Id);
			syslog(LOG_DEBUG, "[DM]: Dm_platform Notify_id : [%s] ",
					DmMsgNotifyMapObjItr->second.c_str());
		}
		syslog(LOG_DEBUG, "[DM]: Dm_platform size of Notify_id map : [%zu] ",
				DmMsgNotifyMapObj->DmMsgNotifyMap.size());
	}
	if (RequestResponseOBJ.Status_Code == DM_SUCCESS) {

//----------------------------------------- PLATFORM SUCCESS START ---------------------------------------------------

		syslog(LOG_DEBUG,
				"[DM]: Dm_platform sending Success for command : [%s] ",
				RequestResponseOBJ.Command.c_str());
		int successCommand = ConvertStringToInt(RequestResponseOBJ.Command);
		switch (successCommand) {
		case DISCOVERY: {

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform /'SENDING SUCCESS DISCOVERY RESPONSE/' start");
			json_object *MainRes = json_object_new_object();
			json_object *things = json_object_new_array();
			DmThingMap* DmThingMapObj = DmThingMap::GetInstance();
			std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
					DmThingMapObj->DmThingDataMap.find(
							RequestResponseOBJ.Thing_Id);
			if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {
				//+++++++++++++++++++++++++++++++++++
				DmThingMapObjItr->second->Display();
				//+++++++++++++++++++++++++++++++++++
				json_object *thingsinfo = json_object_new_object();
				json_object *entities = json_object_new_array();
				json_object *firmware_version = json_object_new_string(
						DmThingMapObjItr->second->getfirmwareVersion().c_str());
				json_object *framework = json_object_new_string(
						DmThingMapObjItr->second->getframeworkName().c_str());
				json_object *hardware_version = json_object_new_string(
						DmThingMapObjItr->second->gethardwareVersion().c_str());
				json_object *manufacturer_name =
						json_object_new_string(
								DmThingMapObjItr->second->getmanufacturerName().c_str());
				json_object *manufacturer_url = json_object_new_string(
						DmThingMapObjItr->second->getmanufacturerUrl().c_str());
				json_object *thing_id = json_object_new_string(
						DmThingMapObjItr->second->getthing_id().c_str());
				json_object *thing_type = json_object_new_string(
						DmThingMapObjItr->second->getthing_type().c_str());
				json_object *thing_name = json_object_new_string(
						DmThingMapObjItr->second->getthing_name().c_str());
				json_object *thing_status = json_object_new_string(
						DmThingMapObjItr->second->getthing_status().c_str());
				json_object *battery_status = json_object_new_string(
						DmThingMapObjItr->second->getbattery_status().c_str());
				json_object_object_add(thingsinfo, "firmware_version",
						firmware_version);
				json_object_object_add(thingsinfo, "framework", framework);
				json_object_object_add(thingsinfo, "hardware_version",
						hardware_version);
				json_object_object_add(thingsinfo, "manufacturer_name",
						manufacturer_name);
				json_object_object_add(thingsinfo, "manufacturer_url",
						manufacturer_url);
				json_object_object_add(thingsinfo, "thing_status",
						thing_status);
				json_object_object_add(thingsinfo, "battery_status",
						battery_status);
				json_object_object_add(thingsinfo, "thing_name", thing_name);
				json_object_object_add(thingsinfo, "thing_type", thing_type);
				json_object_object_add(thingsinfo, "thing_id", thing_id);
				for (int EntityNo = 0;
						EntityNo < DmThingMapObjItr->second->getEntityListSize();
						EntityNo++) {
					json_object *entityinfo = json_object_new_object();
					json_object *properties = json_object_new_array();
					json_object *entity_name =
							json_object_new_string(
									DmThingMapObjItr->second->getEntityInfo(
											EntityNo)->getentity_name().c_str());
					json_object *entity_id =
							json_object_new_string(
									DmThingMapObjItr->second->getEntityInfo(
											EntityNo)->getentity_id().c_str());
					json_object *entity_type =
							json_object_new_string(
									DmThingMapObjItr->second->getEntityInfo(
											EntityNo)->getentity_type().c_str());
					json_object *security_level =
							json_object_new_int(
									DmThingMapObjItr->second->getEntityInfo(
											EntityNo)->getsecurity_level());
					json_object_object_add(entityinfo, "entity_id", entity_id);
					json_object_object_add(entityinfo, "entity_name",
							entity_name);
					json_object_object_add(entityinfo, "entity_type",
							entity_type);
					json_object_object_add(entityinfo, "security_level",
							security_level);
					for (int PropertyNo = 0;
							PropertyNo
									< DmThingMapObjItr->second->getEntityInfo(
											EntityNo)->getPropertySize();
							PropertyNo++) {
						json_object *propertyinfo = json_object_new_object();
						json_object *property_type =
								json_object_new_string(
										DmThingMapObjItr->second->getEntityInfo(
												EntityNo)->getPropertyInfo(
												PropertyNo)->getproperty_type().c_str());
						json_object *property_name =
								json_object_new_string(
										DmThingMapObjItr->second->getEntityInfo(
												EntityNo)->getPropertyInfo(
												PropertyNo)->getproperty_name().c_str());
						json_object* property_value =
								json_object_new_string(
										DmThingMapObjItr->second->getEntityInfo(
												EntityNo)->getPropertyInfo(
												PropertyNo)->getproperty_value().c_str());
						json_object* property_range =
								json_tokener_parse(
										DmThingMapObjItr->second->getEntityInfo(
												EntityNo)->getPropertyInfo(
												PropertyNo)->getproperty_range().c_str());
						json_object_object_add(propertyinfo, "property_type",
								property_type);
						json_object_object_add(propertyinfo, "property_name",
								property_name);
						json_object_object_add(propertyinfo, "property_value",
								property_value);
						json_object_object_add(propertyinfo, "property_range",
								property_range);
						json_object_object_add(propertyinfo,
								"property_updated_at",
								json_object_new_string(
										std::to_string(time(NULL)).c_str()));
						json_object_array_add(properties, propertyinfo);
					}
					json_object_object_add(entityinfo, "properties",
							properties);
					json_object_array_add(entities, entityinfo);
				}
				json_object_object_add(thingsinfo, "entities", entities);
				json_object_array_add(things, thingsinfo);
				json_object_object_add(MainRes, "things", things);
				json_object_object_add(MainRes, "status_code",
						json_object_new_int(DM_SUCCESS));
				syslog(LOG_DEBUG,
						"[DM]: Dm_platform printing success discovery data before sending to TronX:[%s]",
						json_object_get_string(MainRes));
				DmLedInterface::DiscoveryStop();
				DmResponseQueue* DmResponseQueueOBJ =
						DmResponseQueue::GetInstance();
				DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ,
						"POST", "/hub/thing/discovered",
						json_object_get_string(MainRes), PRORITY_VALUE);
				DmSendingRequestResp(RequestResponseOBJ);
				FlagDiscSend = false;
			} else {
				syslog(LOG_CRIT,
						"[DM]: Dm_platform discovery thing data not found in thing map:[%s]",
						RequestResponseOBJ.Thing_Id.c_str());
			}
			////json_object_put(MainRes);
		}
			/*syslog(LOG_INFO, "[DM]: DM_platform sending discovery response end");
			 syslog(LOG_INFO,
			 "[DM]: DM_platform discovery unlock dm_request_condition mutex");*/
			pthread_cond_signal(&DM_Req_Discovery);
			break;

		case CONTROL: {

			/* Filling the json data for sending to tronx */

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform /'SENDING SUCCESS CONTROL RESPONSE/' start");
			json_object *entities = json_object_new_array();
			json_object *properties = json_object_new_array();
			json_object *thingsinfo = json_object_new_object();
			json_object *entityinfo = json_object_new_object();
			json_object *propertyinfo = json_object_new_object();
			json_object *thing_id = json_object_new_string(
					RequestResponseOBJ.Thing_Id.c_str());
			json_object *entity_id = json_object_new_string(
					RequestResponseOBJ.Entity_Id.c_str());
			json_object_object_add(entityinfo, "entity_id", entity_id);
			json_object *property_type = json_object_new_string(
					RequestResponseOBJ.Property_Type.c_str());
			json_object *property_name = json_object_new_string(
					RequestResponseOBJ.Property_Name.c_str());
			json_object *property_value = json_object_new_string(
					RequestResponseOBJ.Property_Value.c_str());
			json_object_object_add(propertyinfo, "property_type",
					property_type);
			json_object_object_add(propertyinfo, "property_name",
					property_name);
			json_object_object_add(propertyinfo, "property_value",
					property_value);
//----------------------------------------------------------------------------------------
			/* Added extra loop for adding the extra property information like
			 * onoff for brightness,dimmer,covering and vice versa  */

			DmThingMap* DmThingMapObj = DmThingMap::GetInstance();
			/* Going into Map based on the thing id for getting at the property levels to be changed*/
			std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
					DmThingMapObj->DmThingDataMap.find(
							RequestResponseOBJ.Thing_Id);
			if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {
				for (int EntityNo = 0;
						EntityNo < DmThingMapObjItr->second->getEntityListSize();
						EntityNo++) {
					if(EntityNo == std::stoi(RequestResponseOBJ.Entity_Id)) {
						
						if ((DmThingMapObjItr->second->getEntityInfo(EntityNo)->getentity_type()
								== std::string("Light"))
								|| (DmThingMapObjItr->second->getEntityInfo(
										EntityNo)->getentity_type()
										== std::string("Switchboard"))
								|| (DmThingMapObjItr->second->getEntityInfo(
										EntityNo)->getentity_type()
										== std::string("Energy Dimmer"))) {
							/* Checking Entity type for Light, Switchboard and Energy Dimmer  */
							if ((RequestResponseOBJ.Property_Name == BRIGHTNESS)
									|| (RequestResponseOBJ.Property_Name == FANSPEED)
									|| (RequestResponseOBJ.Property_Name == DIMMER)) {
								/* If property name is brightness, fanspeed and dimmer */
								/*syslog(LOG_INFO,
								 "**********  [DM]: Rcvd  Switchboard and FanSpeed*******************");*/
								for (int PropertyNo = 0;
										PropertyNo
												< DmThingMapObjItr->second->getEntityInfo(
														EntityNo)->getPropertySize();
										PropertyNo++) {
									if ((DmThingMapObjItr->second->getEntityInfo(
											EntityNo)->getPropertyInfo(PropertyNo)->getproperty_name()
											== BRIGHTNESS)
											|| (DmThingMapObjItr->second->getEntityInfo(
													EntityNo)->getPropertyInfo(
													PropertyNo)->getproperty_name()
													== FANSPEED)
											|| (DmThingMapObjItr->second->getEntityInfo(
													EntityNo)->getPropertyInfo(
													PropertyNo)->getproperty_name()
													== DIMMER)) {
										if (std::stoi(
												DmThingMapObjItr->second->getEntityInfo(
														EntityNo)->getPropertyInfo(
														PropertyNo)->getproperty_value())
												== 0) {
											/* filling the switch detilas into json data for sending into Tronx*/
											json_object *propertyinfo1 =
													json_object_new_object();
											json_object_object_add(propertyinfo1,
													"property_type",
													json_object_new_string(
															"SWITCH"));
											json_object_object_add(propertyinfo1,
													"property_name",
													json_object_new_string(
													ONOFF));
											json_object_object_add(propertyinfo1,
													"property_value",
													json_object_new_string("0"));
											json_object_array_add(properties,
													propertyinfo1);
											for (int PropertyNo1 = 0;
													PropertyNo1
															< DmThingMapObjItr->second->getEntityInfo(
																	EntityNo)->getPropertySize();
													PropertyNo1++) {
												if (DmThingMapObjItr->second->getEntityInfo(
														EntityNo)->getPropertyInfo(
														PropertyNo1)->getproperty_name() == ONOFF) {
													DmThingsBackupDb* DmThingsBackupDbOBJ =
															DmThingsBackupDb::GetInstance();
													DMPropertyDB DMPropertyDBOBJ;
													DMPropertyDBOBJ.THING_ID =
															RequestResponseOBJ.Thing_Id; /* Updating the Thing details in DMthings database*/
													DMPropertyDBOBJ.ENTITY_ID =
															RequestResponseOBJ.Entity_Id;
													DMPropertyDBOBJ.PROPERTY_NAME =
													ONOFF;
													DMPropertyDBOBJ.PROPERTY_VALUE =
															"PROPERTY_VALUE=\'0\'";
													DmThingsBackupDbOBJ->UpdatePropertyTable(
															DMPropertyDBOBJ);
													DmThingMapObjItr->second->getEntityInfo(
															EntityNo)->getPropertyInfo(
															PropertyNo1)->setproperty_value(
															"0");
												}
											}

										} else {
											/* filling the switch detilas into json data for sending into Tronx if it was in ON state*/
											json_object *propertyinfo1 =
													json_object_new_object();
											json_object_object_add(propertyinfo1,
													"property_type",
													json_object_new_string(
															"SWITCH"));
											json_object_object_add(propertyinfo1,
													"property_name",
													json_object_new_string(
													ONOFF));
											json_object_object_add(propertyinfo1,
													"property_value",
													json_object_new_string("1"));
											json_object_array_add(properties,
													propertyinfo1);
											for (int PropertyNo1 = 0;
													PropertyNo1
															< DmThingMapObjItr->second->getEntityInfo(
																	EntityNo)->getPropertySize();
													PropertyNo1++) {
												if (DmThingMapObjItr->second->getEntityInfo(
														EntityNo)->getPropertyInfo(
														PropertyNo1)->getproperty_name() == ONOFF) {
													DmThingsBackupDb* DmThingsBackupDbOBJ =
															DmThingsBackupDb::GetInstance();
													DMPropertyDB DMPropertyDBOBJ;
													DMPropertyDBOBJ.THING_ID =
															RequestResponseOBJ.Thing_Id; /* Updating the Thing details in DMthings database*/
													DMPropertyDBOBJ.ENTITY_ID =
															RequestResponseOBJ.Entity_Id;
													DMPropertyDBOBJ.PROPERTY_NAME =
													ONOFF;
													DMPropertyDBOBJ.PROPERTY_VALUE =
															"PROPERTY_VALUE=\'1\'";
													DmThingsBackupDbOBJ->UpdatePropertyTable(
															DMPropertyDBOBJ);
													DmThingMapObjItr->second->getEntityInfo(
															EntityNo)->getPropertyInfo(
															PropertyNo1)->setproperty_value(
															"1");
												}

											}
										}
									}
								}

							} else if (RequestResponseOBJ.Property_Name == ONOFF) {
								/*If property name is ON_OFF,then updating the Brightness,fanspeed and Dimmer */
								for (int PropertyNo = 0;
										PropertyNo
												< DmThingMapObjItr->second->getEntityInfo(
														EntityNo)->getPropertySize();
										PropertyNo++) {
									/* Checking whether Property name is brightness,fanspeed and dimmer*/
									if ((DmThingMapObjItr->second->getEntityInfo(
											EntityNo)->getPropertyInfo(PropertyNo)->getproperty_name()
											== BRIGHTNESS)
											|| (DmThingMapObjItr->second->getEntityInfo(
													EntityNo)->getPropertyInfo(
													PropertyNo)->getproperty_name()
													== FANSPEED)
											|| (DmThingMapObjItr->second->getEntityInfo(
													EntityNo)->getPropertyInfo(
													PropertyNo)->getproperty_name()
													== DIMMER)) {
										if (DmThingMapObjItr->second->getEntityInfo(
												EntityNo)->getPropertyInfo(
												PropertyNo)->getproperty_name()
												== BRIGHTNESS
												&& (RequestResponseOBJ.Property_Value
														== "1")) {
											/* Checking whether property is brightness or not*/
											json_object *propertyinfo1 =
													json_object_new_object();
											/*filling the  detilas into json data for sending into Tronx if it was in ON state*/
											json_object_object_add(propertyinfo1,
													"property_type",
													json_object_new_string(
															"RANGE"));
											json_object_object_add(propertyinfo1,
													"property_name",
													json_object_new_string(
															"Brightness"));
											json_object_object_add(propertyinfo1,
													"property_value",
													json_object_new_string(
															DmThingMapObjItr->second->getEntityInfo(
																	EntityNo)->getPropertyInfo(
																	PropertyNo)->getproperty_value().c_str()));
											json_object_array_add(properties,
													propertyinfo1);

										} else if ((DmThingMapObjItr->second->getEntityInfo(
												EntityNo)->getPropertyInfo(
												PropertyNo)->getproperty_name()
												== FANSPEED)
												&& (RequestResponseOBJ.Property_Value
														== "1")) {
											json_object *propertyinfo1 =
													json_object_new_object();
											json_object_object_add(propertyinfo1,
													"property_type",
													json_object_new_string(
															"RANGE"));
											json_object_object_add(propertyinfo1,
													"property_name",
													json_object_new_string(
															"Fan Speed"));
											if ((std::stoi)(
													DmThingMapObjItr->second->getEntityInfo(
															EntityNo)->getPropertyInfo(
															PropertyNo)->getproperty_value())
													>= 25) {
												json_object_object_add(
														propertyinfo1,
														"property_value",
														json_object_new_string(
																DmThingMapObjItr->second->getEntityInfo(
																		EntityNo)->getPropertyInfo(
																		PropertyNo)->getproperty_value().c_str()));
												syslog(LOG_DEBUG,
														"[DM]: Dm_platform switch board fan speed in /'IF_CASE/'");
											} else {
												json_object_object_add(
														propertyinfo1,
														"property_value",
														json_object_new_string(
																"25.0"));
												syslog(LOG_DEBUG,
														"[DM]: Dm_platform switch board fan speed in /'ELSE_CASE/'");
											}
											json_object_array_add(properties,
													propertyinfo1);

										} else if ((DmThingMapObjItr->second->getEntityInfo(
												EntityNo)->getPropertyInfo(
												PropertyNo)->getproperty_name()
												== DIMMER)
												&& (RequestResponseOBJ.Property_Value
														== "1")) {
											/* Updating the dimming level when it was in ON state*/
											json_object *propertyinfo1 =
													json_object_new_object();
											json_object_object_add(propertyinfo1,
													"property_type",
													json_object_new_string(
															"RANGE"));
											json_object_object_add(propertyinfo1,
													"property_name",
													json_object_new_string(
															"Dimmer"));
											json_object_object_add(propertyinfo1,
													"property_value",
													json_object_new_string(
															DmThingMapObjItr->second->getEntityInfo(
																	EntityNo)->getPropertyInfo(
																	PropertyNo)->getproperty_value().c_str()));
											json_object_array_add(properties,
													propertyinfo1);
										}
									}
								}

							}

						} else if (RequestResponseOBJ.Property_Name == COVERING) {
							/*syslog(LOG_DEBUG,
							 "[DM]: DM_platform Curtain CHECK");*/
							for (int PropertyNo = 0;
									PropertyNo
											< DmThingMapObjItr->second->getEntityInfo(
													EntityNo)->getPropertySize();
									PropertyNo++) {
								if (DmThingMapObjItr->second->getEntityInfo(
										EntityNo)->getPropertyInfo(PropertyNo)->getproperty_name() == COVERING_STATUS) {
									json_object *propertyinfo1 =
											json_object_new_object();
									json_object_object_add(propertyinfo1,
											"property_type",
											json_object_new_string("INFO"));
									json_object_object_add(propertyinfo1,
											"property_name", json_object_new_string(
											COVERING_STATUS));
									json_object_object_add(propertyinfo1,
											"property_value",
											json_object_new_string(
													DmThingMapObjItr->second->getEntityInfo(
															EntityNo)->getPropertyInfo(
															PropertyNo)->getproperty_value().c_str()));
									json_object_array_add(properties,
											propertyinfo1);
								}
							}
						}
					}
				}

			} else {
				syslog(LOG_INFO, "[DM]: Dm_platform thing not found");
			}
//-----------------------------------------------------------------------------------------
			json_object_array_add(properties, propertyinfo);
			json_object_object_add(entityinfo, "properties", properties);
			json_object_array_add(entities, entityinfo);
			json_object_object_add(thingsinfo, "thing_id", thing_id);
			json_object_object_add(thingsinfo, "entities", entities);
			json_object_object_add(thingsinfo, "status_code",
					json_object_new_int(DM_SUCCESS));
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform printing success control data before sending to TronX:[%s]",
					json_object_get_string(thingsinfo));
			DmResponseQueue* DmResponseQueueOBJ =
					DmResponseQueue::GetInstance();
			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ, "POST",
					"/hub/thing/update", json_object_get_string(thingsinfo),
					PRORITY_VALUE);
			DmSendingRequestResp(RequestResponseOBJ);
			dm_Response = CurrentZWReqTime();
			/*syslog(LOG_DEBUG,
			 "[DM]: DM_platform sending control response queue processing response time: [%lld] ",
			 dm_Response);
			 syslog(LOG_DEBUG,
			 "[DM]: DM_platform sending control response queue processing response total time: [%lld] millisecond ",
			 dm_Response - dm_Request);*/
			////json_object_put(thingsinfo);
		}
			//syslog(LOG_INFO, "[DM]: DM_platform sending control response end");

			/*syslog(LOG_INFO,
			 "[DM]: DM_platform sending control unlock dm_request_condition mutex");*/

//			pthread_cond_signal(&DM_Req_Control);
			break;

		case OBSERVE: {

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform /'SENDING SUCCESS OBSERVE RESPONSE/' start");
			json_object *entities = json_object_new_array();
			json_object *properties = json_object_new_array();
			json_object *thingsinfo;
			json_object *entityinfo;
			entities = json_object_new_array();
			properties = json_object_new_array();
			thingsinfo = json_object_new_object();
//			json_object *thing_status = json_object_new_object();
			json_object *battery_status = NULL; /*Battery status we are initlization with Null*/
			json_object *thing_id = json_object_new_string(
					RequestResponseOBJ.Thing_Id.c_str());
			entityinfo = json_object_new_object();
			json_object *entity_id = json_object_new_string(
					RequestResponseOBJ.Entity_Id.c_str());
			json_object_object_add(entityinfo, "entity_id", entity_id);
			json_object *propertyinfo = json_object_new_object();
			json_object *property_type = json_object_new_string(
					RequestResponseOBJ.Property_Type.c_str());
			json_object *property_name = json_object_new_string(
					RequestResponseOBJ.Property_Name.c_str());
			json_object *property_value = json_object_new_string(
					RequestResponseOBJ.Property_Value.c_str());
			json_object_object_add(propertyinfo, "property_type",
					property_type);
			json_object_object_add(propertyinfo, "property_name",
					property_name);
			json_object_object_add(propertyinfo, "property_value",
					property_value);
			json_object *security_breach, *kill_switch_status;

#if DEMO == 1
			//For Demo

			/*syslog(LOG_INFO,
			 "[DM]: DM_platform observe calling Demo Check function start");*/

			DemoCheck(RequestResponseOBJ.Thing_Id,
					RequestResponseOBJ.Property_Name,
					RequestResponseOBJ.Entity_Id,
					RequestResponseOBJ.Property_Value);

			/*syslog(LOG_INFO,
			 "[DM]: DM_platform observe calling Demo Check function end");*/
#endif
//-----------------------------------------------------------------------------------------

			DmThingMap* DmThingMapObj = DmThingMap::GetInstance();
			std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
					DmThingMapObj->DmThingDataMap.find(
							RequestResponseOBJ.Thing_Id);
			if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {
				/*json_object * thing_status = json_object_new_string(
				 DmThingMapObjItr->second->getthing_status().c_str());*/
				battery_status = json_object_new_string(
						DmThingMapObjItr->second->getbattery_status().c_str()); /* Getting the battery status from the thing Map*/
				for (int EntityNo = 0;
						EntityNo < DmThingMapObjItr->second->getEntityListSize();
						EntityNo++) {
					/* Checking whether the Entity type is light or Not*/
					if (DmThingMapObjItr->second->getEntityInfo(EntityNo)->getentity_type()
							== std::string("Light")) {
						/* Checking the Property name is Brightness or not*/
						if (RequestResponseOBJ.Property_Name == BRIGHTNESS) {
							for (int PropertyNo = 0;
									PropertyNo
											< DmThingMapObjItr->second->getEntityInfo(
													EntityNo)->getPropertySize();
									PropertyNo++) {
								/* Checking from the ThingMap based on the property_name*/
								if (DmThingMapObjItr->second->getEntityInfo(
										EntityNo)->getPropertyInfo(PropertyNo)->getproperty_name() == BRIGHTNESS) {
									if (std::stoi(
											DmThingMapObjItr->second->getEntityInfo(
													EntityNo)->getPropertyInfo(
													PropertyNo)->getproperty_value())
											== 0) {
										json_object *propertyinfo1 =
												json_object_new_object();
										json_object_object_add(propertyinfo1,
												"property_type",
												json_object_new_string(
														"SWITCH"));
										json_object_object_add(propertyinfo1,
												"property_name",
												json_object_new_string(
												ONOFF));
										json_object_object_add(propertyinfo1,
												"property_value",
												json_object_new_string("0"));
										json_object_array_add(properties,
												propertyinfo1);
										for (int PropertyNo1 = 0;
												PropertyNo1
														< DmThingMapObjItr->second->getEntityInfo(
																EntityNo)->getPropertySize();
												PropertyNo1++) {
											if (DmThingMapObjItr->second->getEntityInfo(
													EntityNo)->getPropertyInfo(
													PropertyNo1)->getproperty_name() == ONOFF) {
												DmThingsBackupDb *DmThingsBackupDbOBJ =
														DmThingsBackupDb::GetInstance();
												DMPropertyDB DMPropertyDBOBJ;
												DMPropertyDBOBJ.THING_ID =
														RequestResponseOBJ.Thing_Id;
												DMPropertyDBOBJ.ENTITY_ID =
														RequestResponseOBJ.Entity_Id;
												DMPropertyDBOBJ.PROPERTY_NAME =
												ONOFF;
												DMPropertyDBOBJ.PROPERTY_VALUE =
														"PROPERTY_VALUE=\'0\'";
												DmThingsBackupDbOBJ->UpdatePropertyTable(
														DMPropertyDBOBJ);
												DmThingMapObjItr->second->getEntityInfo(
														EntityNo)->getPropertyInfo(
														PropertyNo1)->setproperty_value(
														"0");
											}
										}
									}
									/* If the brightness is greater than zero setting the ON_OFF PROPERTY VALUE To 1*/
									else {

										json_object *propertyinfo1 =
												json_object_new_object();
										json_object_object_add(propertyinfo1,
												"property_type",
												json_object_new_string(
														"SWITCH"));
										json_object_object_add(propertyinfo1,
												"property_name",
												json_object_new_string(
												ONOFF));
										json_object_object_add(propertyinfo1,
												"property_value",
												json_object_new_string("1"));
										json_object_array_add(properties,
												propertyinfo1);
										for (int PropertyNo1 = 0;
												PropertyNo1
														< DmThingMapObjItr->second->getEntityInfo(
																EntityNo)->getPropertySize();
												PropertyNo1++) {
											if (DmThingMapObjItr->second->getEntityInfo(
													EntityNo)->getPropertyInfo(
													PropertyNo1)->getproperty_name() == ONOFF) {
												DmThingsBackupDb *DmThingsBackupDbOBJ =
														DmThingsBackupDb::GetInstance();
												DMPropertyDB DMPropertyDBOBJ;
												DMPropertyDBOBJ.THING_ID =
														RequestResponseOBJ.Thing_Id;
												DMPropertyDBOBJ.ENTITY_ID =
														RequestResponseOBJ.Entity_Id;
												DMPropertyDBOBJ.PROPERTY_NAME =
												ONOFF;
												DMPropertyDBOBJ.PROPERTY_VALUE =
														"PROPERTY_VALUE=\'1\'";
												DmThingsBackupDbOBJ->UpdatePropertyTable(
														DMPropertyDBOBJ);
												DmThingMapObjItr->second->getEntityInfo(
														EntityNo)->getPropertyInfo(
														PropertyNo1)->setproperty_value(
														"1");
											}
										}
									}
								}
							}
						}
						/* If the Property name is ON_OFF*/
						else if (RequestResponseOBJ.Property_Name == ONOFF) {
							for (int PropertyNo = 0;
									PropertyNo
											< DmThingMapObjItr->second->getEntityInfo(
													EntityNo)->getPropertySize();
									PropertyNo++) {
								// Checking whether property name is Brightness or not*/
								if (DmThingMapObjItr->second->getEntityInfo(
										EntityNo)->getPropertyInfo(PropertyNo)->getproperty_name() == BRIGHTNESS) {
									//Check whether the Range is zero, if the range is zero update the RANGE Property in in  Things Info
									if (std::stoi(
											DmThingMapObjItr->second->getEntityInfo(
													EntityNo)->getPropertyInfo(
													PropertyNo)->getproperty_value())
											== 0) {
										syslog(LOG_INFO,
												"[DM]: Dm_PLATFORM CHECKS IF THE RANGE VALUE IS ZERO");
										// Filling the data in json structure for sending Tronx
										json_object *propertyinfo1 =
												json_object_new_object();
										json_object_object_add(propertyinfo1,
												"property_type",
												json_object_new_string(
														"RANGE"));
										json_object_object_add(propertyinfo1,
												"property_name",
												json_object_new_string(
														"Brightness"));
										json_object_object_add(propertyinfo1,
												"property_value",
												json_object_new_string(
														DmThingMapObjItr->second->getEntityInfo(
																EntityNo)->getPropertyInfo(
																PropertyNo)->getproperty_value().c_str()));
										json_object_array_add(properties,
												propertyinfo1);
									}

								}
							}
						}
					}

					/* Checking whether the Entity type is Dimmer or Not*/
					else if (DmThingMapObjItr->second->getEntityInfo(EntityNo)->getentity_type()
							== std::string("Energy Dimmer")) {
						/* Checking the Property name is Dimmers or not*/
						if (RequestResponseOBJ.Property_Name == DIMMER) {
							syslog(LOG_INFO,
									"[DM]: Dm_platform sending observe response start456");
							for (int PropertyNo = 0;
									PropertyNo
											< DmThingMapObjItr->second->getEntityInfo(
													EntityNo)->getPropertySize();
									PropertyNo++) {
								/* Checking from the ThingMap based on the property_name*/
								if (DmThingMapObjItr->second->getEntityInfo(
										EntityNo)->getPropertyInfo(PropertyNo)->getproperty_name() == DIMMER) {
									syslog(LOG_INFO,
											"[DM]: Dm_platform sending observe responseFOR DIMMER 1-99");
									if (std::stoi(
											DmThingMapObjItr->second->getEntityInfo(
													EntityNo)->getPropertyInfo(
													PropertyNo)->getproperty_value())
											== 0) {
										syslog(LOG_INFO,
												"[DM]: Dm_platform sending observe response To DIMMER 1-99");
										json_object *propertyinfo1 =
												json_object_new_object();
										json_object_object_add(propertyinfo1,
												"property_type",
												json_object_new_string(
														"SWITCH"));
										json_object_object_add(propertyinfo1,
												"property_name",
												json_object_new_string(
												ONOFF));
										json_object_object_add(propertyinfo1,
												"property_value",
												json_object_new_string("0"));
										json_object_array_add(properties,
												propertyinfo1);
										DmThingsBackupDb *DmThingsBackupDbOBJ =
												DmThingsBackupDb::GetInstance();
										DMPropertyDB DMPropertyDBOBJ;
										DMPropertyDBOBJ.THING_ID =
												RequestResponseOBJ.Thing_Id;
										DMPropertyDBOBJ.ENTITY_ID =
												RequestResponseOBJ.Entity_Id;
										DMPropertyDBOBJ.PROPERTY_NAME =
										ONOFF;
										DMPropertyDBOBJ.PROPERTY_VALUE =
												"PROPERTY_VALUE=\'0\'";
										DmThingsBackupDbOBJ->UpdatePropertyTable(
												DMPropertyDBOBJ);
										DmThingMapObjItr->second->getEntityInfo(
												EntityNo)->getPropertyInfo(
												PropertyNo)->setproperty_value(
												"0");
									}

									/*/* If the Range is greater than zero setting the ON_OFF PROPERTY VALUE To 1* */
									else {
										json_object *propertyinfo1 =
												json_object_new_object();
										json_object_object_add(propertyinfo1,
												"property_type",
												json_object_new_string(
														"SWITCH"));
										json_object_object_add(propertyinfo1,
												"property_name",
												json_object_new_string(
												ONOFF));
										json_object_object_add(propertyinfo1,
												"property_value",
												json_object_new_string("1"));
										json_object_array_add(properties,
												propertyinfo1);
										for (int PropertyNo1 = 0;
												PropertyNo1
														< DmThingMapObjItr->second->getEntityInfo(
																EntityNo)->getPropertySize();
												PropertyNo1++) {
											if (DmThingMapObjItr->second->getEntityInfo(
													EntityNo)->getPropertyInfo(
													PropertyNo1)->getproperty_name() == ONOFF) {
												DmThingsBackupDb *DmThingsBackupDbOBJ =
														DmThingsBackupDb::GetInstance();
												DMPropertyDB DMPropertyDBOBJ;
												DMPropertyDBOBJ.THING_ID =
														RequestResponseOBJ.Thing_Id;
												DMPropertyDBOBJ.ENTITY_ID =
														RequestResponseOBJ.Entity_Id;
												DMPropertyDBOBJ.PROPERTY_NAME =
												ONOFF;
												DMPropertyDBOBJ.PROPERTY_VALUE =
														"PROPERTY_VALUE=\'1\'";
												DmThingsBackupDbOBJ->UpdatePropertyTable(
														DMPropertyDBOBJ);
												DmThingMapObjItr->second->getEntityInfo(
														EntityNo)->getPropertyInfo(
														PropertyNo1)->setproperty_value(
														"1");
											}
										}
									}
								}
							}
						}
						/* If the Property name is ON_OFF*/
						else if (RequestResponseOBJ.Property_Name == ONOFF) {
							for (int PropertyNo = 0;
									PropertyNo
											< DmThingMapObjItr->second->getEntityInfo(
													EntityNo)->getPropertySize();
									PropertyNo++) {
								// Checking whether property name is Dimmer
								if (DmThingMapObjItr->second->getEntityInfo(
										EntityNo)->getPropertyInfo(PropertyNo)->getproperty_name() == DIMMER) {
									/**Check whether the Range is zero, if the range is zero update
									 the RANGE Property in in  Things Info*/

									if (std::stoi(
											DmThingMapObjItr->second->getEntityInfo(
													EntityNo)->getPropertyInfo(
													PropertyNo)->getproperty_value())
											== 0) {
										// Filling the data in json structutr for sending Tronx
										json_object *propertyinfo1 =
												json_object_new_object();
										json_object_object_add(propertyinfo1,
												"property_type",
												json_object_new_string(
														"RANGE"));
										json_object_object_add(propertyinfo1,
												"property_name",
												json_object_new_string(
														"Dimmer"));
										json_object_object_add(propertyinfo1,
												"property_value",
												json_object_new_string(
														DmThingMapObjItr->second->getEntityInfo(
																EntityNo)->getPropertyInfo(
																PropertyNo)->getproperty_value().c_str()));
										json_object_array_add(properties,
												propertyinfo1);
									}

								}
							}
						}
					}
				}
			} else {
				syslog(LOG_INFO, "[DM]: Dm_platform thing not found");
			}
			//-----------------------------------------------------------------------------------------
			//checking for security breach
			bool sendFCM = false;
			if (strlen(DM_KILL_SWITCH) > 0) {
				if (RequestResponseOBJ.Thing_Type == "Sensor"
						|| RequestResponseOBJ.Thing_Type
								== "Multilevel Sensor") {
					//checking security breach of sensor
					int security = getSecurityStatus(
							RequestResponseOBJ.Thing_Id,
							RequestResponseOBJ.Entity_Id);
					if (security == 1 || security == 3) {
						if (RequestResponseOBJ.Property_Name == "Occupancy") {
							if (RequestResponseOBJ.Property_Value == "1") {
								//security breach detected
								security_breach = json_object_new_boolean(true);
								kill_switch_status = json_object_new_boolean(
										false);
								pthread_mutex_lock(&ArmStatusLock);
								ArmStatusFlag = 1;
								pthread_mutex_unlock(&ArmStatusLock);
								sendFCM = true;
							}
						} else if (RequestResponseOBJ.Property_Name
								== "Contact Detect") {
							if (RequestResponseOBJ.Property_Value == "0") {
								//security breach detected
								security_breach = json_object_new_boolean(true);
								kill_switch_status = json_object_new_boolean(
										false);
								pthread_mutex_lock(&ArmStatusLock);
								ArmStatusFlag = 1;
								pthread_mutex_unlock(&ArmStatusLock);
								sendFCM = true;
							}// Redmine 22384 start: Including temper alarm
						} else if (RequestResponseOBJ.Property_Name
								== "tamper") {
							if (RequestResponseOBJ.Property_Value == "1") {

							syslog(LOG_INFO,"[DM]:tamper detected");
								//security breach detected
								security_breach = json_object_new_boolean(true);
								kill_switch_status = json_object_new_boolean(
										false);
								pthread_mutex_lock(&ArmStatusLock);
								ArmStatusFlag = 1;
								pthread_mutex_unlock(&ArmStatusLock);
								sendFCM = true;
							} // Redmine 22384 end: Including temper alarm
						} else if (RequestResponseOBJ.Property_Name
								== "Smoke Detect"
								|| RequestResponseOBJ.Property_Name
										== "Gas Detect") {
							if (RequestResponseOBJ.Property_Value == "1") {
								//security breach detected
								security_breach = json_object_new_boolean(true);
								kill_switch_status = json_object_new_boolean(
										false);
								pthread_mutex_lock(&ArmStatusLock);
								ArmStatusFlag = 1;
								pthread_mutex_unlock(&ArmStatusLock);
								sendFCM = true;
							}
						}
					}
				} else {
					std::string custom_id = RequestResponseOBJ.Thing_Id
							+ RequestResponseOBJ.Entity_Id;
					if ((bool) ArmStatusFlag && DM_KILL_SWITCH == custom_id) {
						pthread_mutex_lock(&ArmStatusLock);
						ArmStatusFlag = 0;
						pthread_mutex_unlock(&ArmStatusLock);
						security_breach = json_object_new_boolean(false);
						kill_switch_status = json_object_new_boolean(true);
						sendFCM = true;
					}
				}
			} else {
				//kill switch not configured
			}
//-----------------------------------------------------------------------------------------

			//	syslog(LOG_INFO, "[DM]: DM_platform observe calling rule executing");
			json_object_object_add(propertyinfo, "property_updated_at",
					json_object_new_string(std::to_string(time(NULL)).c_str()));
			json_object_array_add(properties, propertyinfo);
			json_object_object_add(entityinfo, "properties", properties);
			json_object_array_add(entities, entityinfo);
			json_object_object_add(thingsinfo, "thing_id", thing_id);
			json_object_object_add(thingsinfo, "entities", entities);
			json_object_object_add(thingsinfo, "status_code",
					json_object_new_int(DM_SUCCESS));
			//json_object_object_add(thingsinfo, "thing_status", thing_status);
			json_object_object_add(thingsinfo, "battery_status",
					battery_status); /* Adding in to things_info object*/
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform printing success observe response before sending to TronX:[%s]",
					json_object_get_string(thingsinfo));
			DmResponseQueue* DmResponseQueueOBJ =
					DmResponseQueue::GetInstance();
			if (sendFCM) {
				//sending security breach notification to application via fcm notification
				json_object *fcm_request = json_object_new_object();
				json_object *body = json_object_new_object();
				json_object_object_add(fcm_request, "verb",
						json_object_new_string("NOTIFY"));
				json_object_object_add(fcm_request, "domain",
						json_object_new_string("SHIFU"));
				json_object_object_add(fcm_request, "subdomain",
						json_object_new_string("SECURITY"));
				json_object_object_add(body, "thing_info", thingsinfo);
				json_object_object_add(body, "security_breach",
						security_breach);
				json_object_object_add(body, "kill_switch_status",
						kill_switch_status);
				json_object_object_add(fcm_request, "body", body);
				syslog(LOG_DEBUG,
						"[DM] :Dm_platform sending security breach notification: [%s]",
						json_object_get_string(fcm_request));
				if (json_object_get_boolean(kill_switch_status) == false) {
					//start playing buzzer
					syslog(LOG_DEBUG, "[DM]: Dm_platform /'VA_ON/' [%d]",
							va_on);
					if (va_on == 1) /* checking the flag to start the buzzer*/
					{
						system("/etc/init.d/va_service stop");
						volume_present = get_volume(); /* Returning the present volume of shifu*/
						syslog(LOG_DEBUG,
								"[DM] :Dm_platform volume is present before play pause [%d]",
								volume_present);
						set_volume(VOLUME_MAX); /* Setting the volume to high*/
						Audio_play();
						va_on = 0; /* After that we are setting to 0*/
						set_shifu_uci_param((char *) "DM_VA_START_CHECK", (char *)"0");
					}
				} else {
					//stop playing buzzer
					Audio_pause();
					set_volume(volume_present);
					syslog(LOG_DEBUG,
							"[DM] :Dm_platform volume is set after pause [%d]",
							volume_present);
					system("/etc/init.d/va_service start");
					syslog(LOG_DEBUG,
							"[DM]: Dm_platform /'VA_ON/' PLATFORM BEFORE UPDATING THE FLAG [%d]",
							va_on);
					va_on = 1; /* after stopping the buzzer we are setting the flag to 1*/
					syslog(LOG_DEBUG,
							"[DM]: Dm_platform /'VA_ON/' PLATFORM AFTER UPDATING THE FLAG  [%d]",
							va_on);
					set_shifu_uci_param((char *) "DM_VA_START_CHECK", (char *)"1");
				}
				/**
				 * @REDMINE: 22118
				 * */
//				DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ,
//						"POST", "/hub/fcm/push",
//						json_object_get_string(fcm_request),
//					PRORITY_VALUE);
			}
			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ, "POST",
					"/hub/thing/update", json_object_get_string(thingsinfo),
					PRORITY_VALUE);
			DmSendingRequestResp(RequestResponseOBJ);
			////json_object_put(thingsinfo);
		}
			//		syslog(LOG_INFO, "[DM]: DM_platform sending observe response end");
			break;

		case REMOVE: {

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform /'SENDING SUCCESS REMOVE RESPONSE/' start");
			std::string dm_kill_switch(DM_KILL_SWITCH);
			if (dm_kill_switch.find(RequestResponseOBJ.Thing_Id)
					!= std::string::npos) {
				strcpy(DM_KILL_SWITCH, "");
				set_shifu_uci_param((char *) "DM_KILL_SWITCH", DM_KILL_SWITCH);
			}
			json_object *thingsinfo = json_object_new_object();
			json_object *thing_Id = json_object_new_string(
					RequestResponseOBJ.Thing_Id.c_str());
			json_object_object_add(thingsinfo, "thing_id", thing_Id);
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform printing success remove response before sending to TronX:[%s]",
					json_object_get_string(thingsinfo));
			DmResponseQueue* DmResponseQueueOBJ =
					DmResponseQueue::GetInstance();
			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ,
					"DELETE", "/hub/thing/remove",
					json_object_get_string(thingsinfo),
					PRORITY_VALUE);
			DmSendingRequestResp(RequestResponseOBJ);
			dm_Response = CurrentZWReqTime();

			/*syslog(LOG_DEBUG,
			 "[DM]: DM_platform sending remove response queue processing response time: [%lld] ",
			 dm_Response);
			 syslog(LOG_DEBUG,
			 "[DM]: DM_platform sending remove response queue processing response total time: [%lld] millisecond ",
			 dm_Response - dm_Request);*/
			////json_object_put(thingsinfo);
		}
			/*syslog(LOG_INFO, "[DM]: DM_platform sending remove response end");
			 syslog(LOG_INFO,
			 "[DM]: DM_platform sending remove unlock dm_request_condition mutex");*/
			pthread_cond_signal(&DM_Req_Remove);
			break;

		case FORCE_REMOVE: {

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform /'SENDING SUCCESS FORCE_REMOVE RESPONSE/' start");
			json_object *thingsinfo = json_object_new_object();
			json_object *thing_Id = json_object_new_string(
					RequestResponseOBJ.Thing_Id.c_str());
			json_object_object_add(thingsinfo, "thing_id", thing_Id);
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform printing success force_remove response before sending to TronX:[%s]",
					json_object_get_string(thingsinfo));
			DmResponseQueue* DmResponseQueueOBJ =
					DmResponseQueue::GetInstance();
			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ,
					"DELETE", "/hub/thing/remove",
					json_object_get_string(thingsinfo),
					PRORITY_VALUE);
			DmSendingRequestResp(RequestResponseOBJ);
			dm_Response = CurrentZWReqTime();

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform sending remove response queue processing response time: [%lld] ",
					dm_Response);
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform sending remove response queue processing response total time: [%lld] millisecond ",
					dm_Response - dm_Request);
			////json_object_put(thingsinfo);
		}
//			syslog(LOG_INFO, "[DM]: Platform sending remove response end");
//			syslog(LOG_INFO,
//					"[DM]: Platform sending remove unlock dm_request_condition mutex");
			pthread_cond_signal(&DM_Req_ForceRemove);
			break;

		case EXCLUSION: {

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform /'SENDING SUCCESS EXCLUSION RESPONSE/' start");
			json_object *thingsinfo = json_object_new_object();
			json_object *thing_Id = json_object_new_string(
					RequestResponseOBJ.Thing_Id.c_str());
			json_object_object_add(thingsinfo, "thing_id", thing_Id);
			json_object_object_add(thingsinfo, "status_code",
					json_object_new_int(DM_SUCCESS));
			syslog(LOG_DEBUG,
					"[DM]: DM_platform printing success exclusion response before sending to TronX:[%s]",
					json_object_get_string(thingsinfo));
			DmResponseQueue* DmResponseQueueOBJ =
					DmResponseQueue::GetInstance();
			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ,
					"DELETE", "/hub/thing/remove",
					json_object_get_string(thingsinfo),
					PRORITY_VALUE);
			DmSendingRequestResp(RequestResponseOBJ);
			////json_object_put(thingsinfo);
		}
			/*syslog(LOG_INFO, "[DM]: DM_platform sending exclusion response end");
			 syslog(LOG_INFO,
			 "[DM]: DM_platform sending exclusion unlock dm_request_condition mutex");*/
			pthread_cond_signal(&DM_Req_Exclusion);
			break;

		case FOTA_REQ: { /*Updating dB and TronX with new firmware-version*/

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform SUCCESS FOTA -> DM_platform Updating FirmwareDB");
			json_object* thingsinfo = json_object_new_object();
			json_object_object_add(thingsinfo, "thing_id",
					json_object_new_string(
							RequestResponseOBJ.Thing_Id.c_str()));
			json_object_object_add(thingsinfo, "firmware_version",
					json_object_new_string(
							RequestResponseOBJ.Firmware_version.c_str()));
			DmThingsBackupDb* DmFotaResponseOBJ =
					DmThingsBackupDb::GetInstance();
			DmFotaResponseOBJ->UpdateEndDeviceFirmwareDB(RequestResponseOBJ); /* Updating the DB incase of successful upgrade of device */
			syslog(LOG_DEBUG,
					"[DM]: DM_platform printing success fota request data before sending to TronX:[%s]",
					json_object_get_string(thingsinfo));
			DmResponseQueue* DmResponseQueueOBJ =
					DmResponseQueue::GetInstance();
			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ, "POST",
					"/hub/thing/update", json_object_get_string(thingsinfo),
					PRORITY_VALUE); /* Updating TronX with updated firmware-version incase of successful upgrade of device */
			DmSendingRequestResp(RequestResponseOBJ);
			////json_object_put(thingsinfo);
		}
			FlagFota = false; /* Making the FOTA flag false while sending success response to client*/
			//pthread_cond_signal(&DM_Req_ZigbeeFota); //As we are not waiting so not calling for signal
			break;

		case LEARN_MODE: {

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform /'SENDING SUCCESS LEARN_MODE RESPONSE/' start");
//			DmResponseQueue* DmResponseQueueOBJ =
//					DmResponseQueue::GetInstance();
			DmSendingRequestResp(RequestResponseOBJ);
		}
			pthread_cond_signal(&DM_Req_LearnMode);
			break;

		case GETRULE: {

			syslog(LOG_DEBUG,
					"[DM]: DM_platform /'SENDING SUCCESS GETRULE RESPONSE/' start");
			DmResponseQueue* DmResponseQueueOBJ =
					DmResponseQueue::GetInstance();
			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ, "GET",
					"/hub/rule/list", "DM_DEFAULT", PRORITY_VALUE);
		}
			break;

		case GETSCENE: {

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform /'SENDING SUCCESS GETSCENE RESPONSE/' START");
			DmResponseQueue* DmResponseQueueOBJ =
					DmResponseQueue::GetInstance();
			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ, "GET",
					"/hub/scene/listall", "DM_DEFAULT", PRORITY_VALUE);
		}
			break;

			/** Added Extra case for Scene request is initiated from Voice */
			//case VOICE_SCENE:
		case SCENE: {

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform /'SENDING SUCCESS SCENE RESPONSE/'start");
			DmSendingRequestResp(RequestResponseOBJ);
		}
			break;

		case RULE: {

			syslog(LOG_DEBUG,
					"[DM]: DM_platform /'SENDING SUCCESS RULE RESPONSE/' start");
			DmSendingRequestResp(RequestResponseOBJ);
		}

			break;

		case S2_SECURITY: {

			syslog(LOG_DEBUG,
					"[DM]: DM_platform /'SENDING S2_SECURITY RESPONSE/' start ");
			DmResponseQueue* DmResponseQueueOBJ =
					DmResponseQueue::GetInstance();
			RequestResponseOBJ.Client = 11;
			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ, "POST",
					"/hub/fcm/push", RequestResponseOBJ.Thing_Response,
					PRORITY_VALUE);
			DmSendingRequestResp(RequestResponseOBJ);
		}
			break;

		case SECURITY: {

			syslog(LOG_DEBUG,
					"[DM]: DM_platform /'SENDING SECURITY RESPONSE/' start ");
			DmSendingRequestResp(RequestResponseOBJ);
		}
			break;

		default:
			break;
		}

//----------------------------------------- PLATFORM SUCCESS END ---------------------------------------------------
	} else {

//----------------------------------------- PLATFORM ERROR START ---------------------------------------------------

		syslog(LOG_DEBUG,
				"[DM]: Dm_platform sending error response for command : [%s]",
				RequestResponseOBJ.Command.c_str());
		DmResponseQueue* DmResponseQueueOBJ = DmResponseQueue::GetInstance();
		int errorCommand = ConvertStringToInt(RequestResponseOBJ.Command);
		switch (errorCommand) {
		case DISCOVERY: {

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform /'SENDING ERROR DISCOVERY RESPONSE/' start");
			json_object *thingsinfo = json_object_new_object();
			json_object *thing_Id = json_object_new_string(
					RequestResponseOBJ.Thing_Id.c_str());
			json_object *message = json_object_new_string(
					RequestResponseOBJ.Message.c_str());
			json_object *status_code = json_object_new_int(
					RequestResponseOBJ.Status_Code);
			json_object *notif_id = json_object_new_string(
					RequestResponseOBJ.Notif_Id.c_str());
			json_object_object_add(thingsinfo, "status_code", status_code);
			json_object_object_add(thingsinfo, "message", message);
			json_object_object_add(thingsinfo, "thing_id", thing_Id);
			json_object_object_add(thingsinfo, "notif_id", notif_id);
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform printing failed discovery data before sending to TronX:[%s]",
					json_object_get_string(thingsinfo));
			DmSendingRequestResp(RequestResponseOBJ);
			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ,
					"DM_DEFAILT", "DM_DEFAILT",
					json_object_get_string(thingsinfo), PRORITY_VALUE);
			/*syslog(LOG_INFO,
			 "[DM]: DM_platform discovery error unlock dm_request_condition mutex");*/
			pthread_cond_signal(&DM_Req_Discovery);
			////json_object_put(thingsinfo);
		}
			/*syslog(LOG_INFO,
			 "[DM]: DM_platform sending error discovery response end ");*/
			break;

		case CONTROL: {

			syslog(LOG_INFO,
						 "[DM]: Dm_platform /'SENDING ERROR CONTROL RESPONSE/' start ");
						//Sending response to tronx even in Failed case
						DmThingsBackupDb* DmThingsBackupDbOBJ =
								DmThingsBackupDb::GetInstance();
						DMPropertyDB DMPropertyDBOBJ;
						DMPropertyDBOBJ.THING_ID = RequestResponseOBJ.Thing_Id;
						DMPropertyDBOBJ.ENTITY_ID = RequestResponseOBJ.Entity_Id;
						DMPropertyDBOBJ.PROPERTY_NAME = RequestResponseOBJ.Property_Name;
						DMPropertyDBOBJ.PROPERTY_RANGE = RequestResponseOBJ.Property_Range;
						//getting property value from DB and updating to Map
						RequestResponseOBJ.Property_Value =
								DmThingsBackupDbOBJ->Get_Property_Value(DMPropertyDBOBJ);
						DmThingMap* DmThingMapObj = DmThingMap::GetInstance();
						std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
								DmThingMapObj->DmThingDataMap.find(
										RequestResponseOBJ.Thing_Id);


						if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {
									for (int entitysize = 0;
											entitysize
													< DmThingMapObjItr->second->getEntityListSize();
											entitysize++) {
										if (strcmp(RequestResponseOBJ.Entity_Id.c_str(),
												DmThingMapObjItr->second->getEntityInfo(entitysize)->getentity_id().c_str())
												== 0) {
											for (int propertysize = 0;
													propertysize
															< DmThingMapObjItr->second->getEntityInfo(
																	entitysize)->getPropertySize();
													propertysize++) {
												if (strcmp(RequestResponseOBJ.Property_Name.c_str(),
														DmThingMapObjItr->second->getEntityInfo(
																entitysize)->getPropertyInfo(
																propertysize)->getproperty_name().c_str())
														== 0) {
													DmThingMapObjItr->second->getEntityInfo(
															entitysize)->getPropertyInfo(
															propertysize)->setproperty_value(
															RequestResponseOBJ.Property_Value);
													RequestResponseOBJ.Property_Type =
															DmThingMapObjItr->second->getEntityInfo(
																	entitysize)->getPropertyInfo(
																	propertysize)->getproperty_type();
													break;
												}
											}
											break;
										}
									}
									json_object *thingsinfo = json_object_new_object();
									json_object *propertyinfo = json_object_new_object();
									json_object *entityinfo = json_object_new_object();
									json_object *properties = json_object_new_array();
									json_object *entities = json_object_new_array();
									json_object *thing_Id = json_object_new_string(
											RequestResponseOBJ.Thing_Id.c_str());
									json_object *message = json_object_new_string(
											RequestResponseOBJ.Message.c_str());
									json_object *status_code = json_object_new_int(
											RequestResponseOBJ.Status_Code);
									json_object *notif_id = json_object_new_string(
											RequestResponseOBJ.Notif_Id.c_str());
									json_object_object_add(propertyinfo, "property_type",
											json_object_new_string(
													RequestResponseOBJ.Property_Type.c_str()));
									json_object_object_add(propertyinfo, "property_name",
											json_object_new_string(
													RequestResponseOBJ.Property_Name.c_str()));
									json_object_object_add(propertyinfo, "property_value",
											json_object_new_string(
													RequestResponseOBJ.Property_Value.c_str()));
									json_object_array_add(properties, propertyinfo);
									json_object_object_add(entityinfo, "properties", properties);
									json_object_object_add(entityinfo, "entity_id",
											json_object_new_string(
													RequestResponseOBJ.Entity_Id.c_str()));
									json_object_array_add(entities, entityinfo);
									json_object_object_add(thingsinfo, "entities", entities);
									json_object_object_add(thingsinfo, "status_code", status_code);
									json_object_object_add(thingsinfo, "message", message);
									json_object_object_add(thingsinfo, "thing_id", thing_Id);
									json_object_object_add(thingsinfo, "notif_id", notif_id);
									syslog(LOG_DEBUG,
											"[DM]: Dm_platform printing failed control data before sending to TronX:[%s]",
											json_object_get_string(thingsinfo));
									DmSendingRequestResp(RequestResponseOBJ);
									DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ, "POST",
											"/hub/thing/update", json_object_get_string(thingsinfo),
											PRORITY_VALUE);
								}
		 else {

			 	 json_object *thingsinfo = json_object_new_object();
				RequestResponseOBJ.Thing_Id=removeThingID;
				syslog(LOG_DEBUG, "[DM]: Dm_platform remove thing_ID [%s]",
						removeThingID.c_str());
				json_object *thing_Id = json_object_new_string(
						removeThingID.c_str());
				json_object_object_add(thingsinfo, "thing_id", thing_Id);
//				json_object_object_add(thingsinfo, "status_code", status_code);
//				json_object_object_add(thingsinfo, "message", message);
				DmSendingRequestResp(RequestResponseOBJ);
				DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ,
						"DELETE", "/hub/thing/remove",
						json_object_get_string(thingsinfo),
						PRORITY_VALUE);
			}

//

//			json_object *notif_id = json_object_new_string(
//					RequestResponseOBJ.Notif_Id.c_str());
//			json_object_object_add(propertyinfo, "property_type",
//					json_object_new_string(
//							RequestResponseOBJ.Property_Type.c_str()));
//			json_object_object_add(propertyinfo, "property_name",
//					json_object_new_string(
//							RequestResponseOBJ.Property_Name.c_str()));
//			json_object_object_add(propertyinfo, "property_value",
//					json_object_new_string(
//							RequestResponseOBJ.Property_Value.c_str()));
//			json_object_array_add(properties, propertyinfo);
//			json_object_object_add(entityinfo, "properties", properties);
//			json_object_object_add(entityinfo, "entity_id",
//					json_object_new_string(
//							RequestResponseOBJ.Entity_Id.c_str()));
//			json_object_array_add(entities, entityinfo);
//			json_object_object_add(thingsinfo, "entities", entities);

////			json_object_object_add(thingsinfo, "thing_id", thing_Id);
//			json_object_object_add(thingsinfo, "notif_id", notif_id);
//			syslog(LOG_DEBUG,
//					"[DM]: Dm_platform printing failed control data before sending to TronX:[%s]",
//					json_object_get_string(thingsinfo));
//			DmSendingRequestResp(RequestResponseOBJ);
//			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ, "POST",
//					"/hub/thing/update", json_object_get_string(thingsinfo),
//					PRORITY_VALUE);
			/*syslog(LOG_INFO,
			 "[DM]: DM_platform control error unlock dm_request_condition mutex");*/
//			pthread_cond_signal(&DM_Req_Control);
			////json_object_put(thingsinfo);*/


		}
			break;

		case OBSERVE: {

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform /'SENDING ERROR OBSERVE RESPONSE/' start ");
			json_object *thingsinfo = json_object_new_object();
//			json_object *thing_status = NULL; /** To update the device status offline/online */
			json_object *battery_status = NULL; /* Creating json object for battery*/
			DmThingMap* DmThingMapObj = DmThingMap::GetInstance();
			std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
					DmThingMapObj->DmThingDataMap.find(
							RequestResponseOBJ.Thing_Id);
			if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {
//				thing_status = json_object_new_string(
//						DmThingMapObjItr->second->getthing_status().c_str());
				battery_status = json_object_new_string(
						DmThingMapObjItr->second->getbattery_status().c_str()); /* Getting the battery status from Thing map*/
			}
			json_object *thing_Id = json_object_new_string(
					RequestResponseOBJ.Thing_Id.c_str());
			json_object *message = json_object_new_string(
					RequestResponseOBJ.Message.c_str());
			json_object *status_code = json_object_new_int(400);
//			json_object *notif_id = json_object_new_string(
//					RequestResponseOBJ.Notif_Id.c_str());
			json_object_object_add(thingsinfo, "status_code", status_code);
			json_object_object_add(thingsinfo, "message", message);
			json_object_object_add(thingsinfo, "thing_id", thing_Id);
			//json_object_object_add(thingsinfo, "notif_id", notif_id);
			//json_object_object_add(thingsinfo, "thing_status", thing_status);
			json_object_object_add(thingsinfo, "battery_status",
					battery_status); /* we are adding the battery status in json structure for sending to Tronx*/
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform printing failed observe data before sending to TronX:[%s]",
					json_object_get_string(thingsinfo));
			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ, "POST",
					"/hub/thing/update", json_object_get_string(thingsinfo),
					PRORITY_VALUE);
			/*syslog(LOG_INFO,
			 "[DM]: DM_platform control error unlock dm_request_condition mutex");*/

//			pthread_cond_signal(&DM_Req_Control);
			////json_object_put(thingsinfo);
		}
			break;

		case REMOVE: {

			syslog(LOG_DEBUG,
					"[DM]: DM_platform /'SENDING ERROR REMOVE RESPONSE/' start ");
			json_object *thingsinfo = json_object_new_object();
			json_object *thing_Id = json_object_new_string(
					RequestResponseOBJ.Thing_Id.c_str());
			json_object *message = json_object_new_string(
					RequestResponseOBJ.Message.c_str());
			json_object *status_code = json_object_new_int(
					RequestResponseOBJ.Status_Code);
			json_object *notif_id = json_object_new_string(
					RequestResponseOBJ.Notif_Id.c_str());
			json_object_object_add(thingsinfo, "status_code", status_code);
			json_object_object_add(thingsinfo, "message", message);
			json_object_object_add(thingsinfo, "thing_id", thing_Id);
			json_object_object_add(thingsinfo, "notif_id", notif_id);
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform printing failed remove data before sending to TronX:[%s]",
					json_object_get_string(thingsinfo));
			DmSendingRequestResp(RequestResponseOBJ);
			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ,
					"DM_DEFAULT", "DM_DEFAULT",
					json_object_get_string(thingsinfo), PRORITY_VALUE);
			/*syslog(LOG_INFO,
			 "[DM]: DM_platform remove error unlock dm_request_condition mutex");*/
			pthread_cond_signal(&DM_Req_Remove);
			////json_object_put(thingsinfo);
		}
			break;

		case EXCLUSION: {

			syslog(LOG_DEBUG,
					"[DM]: DM_platform /'SENDING ERROR EXCLUSION RESPONSE/' start ");
			json_object *thingsinfo = json_object_new_object();
			json_object *thing_Id = json_object_new_string(
					RequestResponseOBJ.Thing_Id.c_str());
			json_object *message = json_object_new_string(
					RequestResponseOBJ.Message.c_str());
			json_object *status_code = json_object_new_int(
					RequestResponseOBJ.Status_Code);
			json_object *notif_id = json_object_new_string(
					RequestResponseOBJ.Notif_Id.c_str());
			json_object_object_add(thingsinfo, "status_code", status_code);
			json_object_object_add(thingsinfo, "message", message);
			json_object_object_add(thingsinfo, "thing_id", thing_Id);
			json_object_object_add(thingsinfo, "notif_id", notif_id);
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform printing failed exclusion data before sending to TronX:[%s]",
					json_object_get_string(thingsinfo));
			DmSendingRequestResp(RequestResponseOBJ);
			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ,
					"DM_DEFAULT", "DM_DEFAULT",
					json_object_get_string(thingsinfo), PRORITY_VALUE);
			/*syslog(LOG_INFO,
			 "[DM]: DM_platform exclusion error unlock dm_request_condition mutex");*/
			pthread_cond_signal(&DM_Req_Exclusion);
			////json_object_put(thingsinfo);
		}

			break;

		case LEARN_MODE: {

			syslog(LOG_DEBUG,
					"[DM]: DM_platform /'SENDING ERROR LEARN_MODE RESPONSE/' start ");
			DmSendingRequestResp(RequestResponseOBJ);
//			syslog(LOG_INFO,
//					"[DM]: DM_platform Learn mode error unlock dm_request_condition mutex");
			pthread_cond_signal(&DM_Req_LearnMode);
		}
			break;

		case FOTA_REQ: {

			syslog(LOG_DEBUG,
					"[DM]: DM_platform /'SENDING ERROR FOTA_REQ RESPONSE/' start ");
			DmSendingRequestResp(RequestResponseOBJ);
			FlagFota = false; /* Making the FOTA flag false while sending error response to client*/
//			syslog(LOG_INFO,
//					"[DM]: DM_platform FOTA error dm_request_condition mutex");
			//pthread_cond_signal(&DM_Req_ZigbeeFota); // As we are not waiting so not calling for signal
		}
			break;

			//case VOICE_SCENE:
		case SCENE: {

			syslog(LOG_DEBUG,
					"[DM]: DM_platform /'SENDING ERROR SCENE RESPONSE/' start ");
			json_object *thingsinfo = json_object_new_object();
			json_object *thing_Id = json_object_new_string(
					RequestResponseOBJ.Thing_Id.c_str());
			json_object *message = json_object_new_string(
					RequestResponseOBJ.Message.c_str());
			json_object *status_code = json_object_new_int(
					RequestResponseOBJ.Status_Code);
			json_object *notif_id = json_object_new_string(
					RequestResponseOBJ.Notif_Id.c_str());
			json_object_object_add(thingsinfo, "status_code", status_code);
			json_object_object_add(thingsinfo, "message", message);
			json_object_object_add(thingsinfo, "thing_id", thing_Id);
			json_object_object_add(thingsinfo, "notif_id", notif_id);
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform printing failed scene data before sending to TronX:[%s]",
					json_object_get_string(thingsinfo));
			DmSendingRequestResp(RequestResponseOBJ);
			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ,
					"DM_DEFAILT", "DM_DEFAILT",
					json_object_get_string(thingsinfo), PRORITY_VALUE);
			/*syslog(LOG_INFO,
			 "[DM]: DM_platform scene error unlock dm_request_condition mutex");*/
//			pthread_cond_signal(&DM_Req_Control);
			////json_object_put(thingsinfo);
		}
			break;

		case RULE: {

			syslog(LOG_DEBUG,
					"[DM]: DM_platform /'SENDING ERROR RULE RESPONSE/' start ");
			json_object *thingsinfo = json_object_new_object();
			json_object *thing_Id = json_object_new_string(
					RequestResponseOBJ.Thing_Id.c_str());
			json_object *message = json_object_new_string(
					RequestResponseOBJ.Message.c_str());
			json_object *status_code = json_object_new_int(
					RequestResponseOBJ.Status_Code);
			json_object *notif_id = json_object_new_string(
					RequestResponseOBJ.Notif_Id.c_str());
			json_object_object_add(thingsinfo, "status_code", status_code);
			json_object_object_add(thingsinfo, "message", message);
			json_object_object_add(thingsinfo, "thing_id", thing_Id);
			json_object_object_add(thingsinfo, "notif_id", notif_id);
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform printing failed rule data before sending to TronX:[%s]",
					json_object_get_string(thingsinfo));
			DmSendingRequestResp(RequestResponseOBJ);
			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ,
					"DM_DEFAILT", "DM_DEFAILT",
					json_object_get_string(thingsinfo), PRORITY_VALUE);
			/*syslog(LOG_INFO,
			 "[DM]: DM_platform rule error unlock dm_request_condition mutex");*/
//			pthread_cond_signal(&DM_Req_Control);
			////json_object_put(thingsinfo);
		}
			break;

		case S2_SECURITY: {

			syslog(LOG_DEBUG,
					"[DM]: DM_platform /'SENDING ERROR S2_SECURITY RESPONSE/' start ");
			json_object *thingsinfo = json_object_new_object();
			json_object *thing_Id = json_object_new_string(
					RequestResponseOBJ.Thing_Id.c_str());
			json_object *message = json_object_new_string(
					RequestResponseOBJ.Message.c_str());
			json_object *status_code = json_object_new_int(
					RequestResponseOBJ.Status_Code);
			json_object *notif_id = json_object_new_string(
					RequestResponseOBJ.Notif_Id.c_str());
			json_object_object_add(thingsinfo, "status_code", status_code);
			json_object_object_add(thingsinfo, "message", message);
			json_object_object_add(thingsinfo, "thing_id", thing_Id);
			json_object_object_add(thingsinfo, "notif_id", notif_id);
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform printing failed s2_security data before sending to TronX:[%s]",
					json_object_get_string(thingsinfo));
			DmSendingRequestResp(RequestResponseOBJ);
			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ,
					"DM_DEFAILT", "DM_DEFAILT",
					json_object_get_string(thingsinfo), PRORITY_VALUE);
			/*syslog(LOG_INFO,
			 "[DM]: DM_platform rule error unlock dm_request_condition mutex");*/

//			pthread_cond_signal(&DM_Req_Control);
			////json_object_put(thingsinfo);
		}
			break;

		case SECURITY: {

			syslog(LOG_DEBUG,
					"[DM]: DM_platform /'SENDING ERROR SECURITY RESPONSE/' start ");
			json_object *thingsinfo = json_object_new_object();
			json_object *thing_Id = json_object_new_string(
					RequestResponseOBJ.Thing_Id.c_str());
			json_object *message = json_object_new_string(
					RequestResponseOBJ.Message.c_str());
			json_object *status_code = json_object_new_int(
					RequestResponseOBJ.Status_Code);
			json_object *notif_id = json_object_new_string(
					RequestResponseOBJ.Notif_Id.c_str());
			json_object_object_add(thingsinfo, "status_code", status_code);
			json_object_object_add(thingsinfo, "message", message);
			json_object_object_add(thingsinfo, "thing_id", thing_Id);
			json_object_object_add(thingsinfo, "notif_id", notif_id);
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform printing failed security data before sending to TronX:[%s]",
					json_object_get_string(thingsinfo));
			DmSendingRequestResp(RequestResponseOBJ);
			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ,
					"DM_DEFAILT", "DM_DEFAILT",
					json_object_get_string(thingsinfo), PRORITY_VALUE);
			/*syslog(LOG_INFO,
			 "[DM]: DM_platform rule error unlock dm_request_condition mutex");*/
			pthread_cond_signal(&DM_Req_Control);
			////json_object_put(thingsinfo);
		}
		default:
			break;
		}
//----------------------------------------- PLATFORM ERROR END ---------------------------------------------------
	}

	//syslog(LOG_INFO, "[DM]: DM_platform sending response end");
}

void DmPlatform::DmSendingRequestResp(RequestResponse RequestResponseOBJ) {

	DmZeroMq* DmZeroMqOBJ = DmZeroMq::GetInstance();

	syslog(LOG_INFO, "[DM]: Dm_platform /'SENDING REQUEST-RESPONSE/' start ");
	syslog(LOG_DEBUG,
			"[DM] : [AUTO] : Dm_platform RESPONSE STATUS_CODE:[%d], "
					"THING_ID:[%s], ENTITY_ID:[%s], PROPERTY_NAME:[%s], PROPERTY_VALUE:[%s]",
			RequestResponseOBJ.Status_Code, RequestResponseOBJ.Thing_Id.c_str(),
			RequestResponseOBJ.Entity_Id.c_str(),
			RequestResponseOBJ.Property_Name.c_str(),
			RequestResponseOBJ.Property_Value.c_str());

	syslog(LOG_DEBUG,
			"[DM]: Dm_platform sending request-response client: [%d] command: [%s] "
					"entity_id: [%s] message_id: [%s] message: [%s] notif_id: [%s] "
					"property_name: [%s] property_type: [%s] property_value: [%s] "
					"proeporty_range: [%s] rulescenedata: [%s] status_code: [%d] thing_id: [%s]",
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

	/** Checking Message_Id length */
	if (!(RequestResponseOBJ.Message_Id.size() < 4)) {
		switch (std::stol(RequestResponseOBJ.Message_Id.substr(2, 2))) {
		case AREA_CONTROL:
		case SCENE_CONTROL:
		case RULE_COLTROL:
			RequestResponseOBJ.Client = DM_FAIL;
			syslog(LOG_DEBUG,
					"Dm_platform sending AREA/SCENE/RULE control: [%s]",
					RequestResponseOBJ.Message_Id.c_str());
			break;
		default:
			syslog(LOG_DEBUG,
					"Dm_platform sending request-response print client other than "
							"area_control scene_control rule_coltrol in /'DEFAULT_CASE/': [%s]",
					RequestResponseOBJ.Message_Id.c_str());
			break;
		}
	}

	int toSendClient = RequestResponseOBJ.Client;
	switch (toSendClient) {
	case RULE_COLTROL: {

		if (RequestResponseOBJ.Command == DM_RULE_COLTROL) {
			if (RequestResponseOBJ.RuleSceneData == "SUCCESS") {
				DmRuleMapping::DMSetRuleExecuteStatus(
						RequestResponseOBJ.Notif_Id);
				pthread_mutex_lock(&RuleStatusLock);
				RuleStatusFlag = 0;
				syslog(LOG_DEBUG, "[DM]: Dm_platform /'RuleStatusFlag/' in /'SUCCESS_CASE/'[%d] ",
						RuleStatusFlag);
				pthread_mutex_unlock(&RuleStatusLock);

			} else if (RequestResponseOBJ.RuleSceneData == "FAIL") {

				DmRuleMapping::DMSetRuleExecuteStatus(
						RequestResponseOBJ.Notif_Id);
				pthread_mutex_lock(&RuleStatusLock);
				RuleStatusFlag = 0;
				syslog(LOG_DEBUG, "[DM]: Dm_platform /'RuleStatusFlag/' in /'FAILURE_CASE/'[%d] ",
						RuleStatusFlag);
				pthread_mutex_unlock(&RuleStatusLock);

			} else if (RequestResponseOBJ.RuleSceneData == "PARTIAL") {

				syslog(LOG_DEBUG,
						"[DM]: Dm_platform rule execute complete for rule_id: [%s]",
						RequestResponseOBJ.Notif_Id.c_str());
				DmRuleMapping::DMSetRuleExecuteStatus(
						RequestResponseOBJ.Notif_Id);
				pthread_mutex_lock(&RuleStatusLock);
				RuleStatusFlag = 0;
				syslog(LOG_DEBUG, "[DM]: Dm_platform /'RuleStatusFlag/' in /'PARTIAL_CASE/'[%d] ",
						RuleStatusFlag);
				pthread_mutex_unlock(&RuleStatusLock);

			} else {

				RuleStatusFlag = 0;
				syslog(LOG_DEBUG,
						"[DM]: Dm_platform sending Rule execution status RULE_COLTROL  operation");
			}
		}
	}

		break;

	case SCENE_CONTROL: {

//		if (RequestResponseOBJ.Command == DM_SCENE_CONTROL) {
//			DmSceneMapping DmSceneMappingobj;
//			DmSceneMapping::DMSetSceneMapExecuteflag(false);
//			DmSceneMapping::DMSetSceneExecuteStatus(
//					RequestResponseOBJ.Notif_Id);
//			syslog(LOG_DEBUG, "[DM]: Dm_platform /'SceneStatusFlag/' cleared");
//		}
	}
		break;

	case CRON_HANDLER: {

		syslog(LOG_DEBUG,
				"[DM]: Dm_platform sending request response to /'CJ/' start ");
		json_object* va_response = json_object_new_object();
		json_object* client_data = json_tokener_parse(
				RequestResponseOBJ.RuleSceneData.c_str());
		//json_object *va_response = json_object_object_get(client_data, "client_data");
		//json_object* va_response = json_tokener_parse(RequestResponseOBJ.Command.c_str());
		syslog(LOG_DEBUG, "[DM]: Dm_platform cron_1 [%s]",
				json_object_get_string(
						json_object_object_get(client_data, "verb")));
		syslog(LOG_DEBUG, "[DM]: Dm_platform cron_2 [%s]",
				json_object_get_string(
						json_object_object_get(client_data, "cron_pattern")));
		syslog(LOG_DEBUG, "[DM]: Dm_platform cron_3 [%s]",
				json_object_get_string(
						json_object_object_get(client_data, "rule_id")));
		syslog(LOG_DEBUG, "[DM]: Dm_platform cron_4 [%s]",
				json_object_get_string(
						json_object_object_get(client_data, "actions")));
		if (RequestResponseOBJ.Command == DM_RULE_COLTROL) {
			if (RequestResponseOBJ.RuleSceneData == "SUCCESS") {
				DmRuleMapping::DMSetRuleExecuteStatus(
						RequestResponseOBJ.Notif_Id);
				pthread_mutex_lock(&RuleStatusLock);
				RuleStatusFlag = 0;
				syslog(LOG_DEBUG, "[DM]: Dm_platform /'RuleStatusFlag/' [%d] ",
						RuleStatusFlag);
				pthread_mutex_unlock(&RuleStatusLock);

			} else if (RequestResponseOBJ.RuleSceneData == "FAIL") {

				DmRuleMapping::DMSetRuleExecuteStatus(
						RequestResponseOBJ.Notif_Id);
				pthread_mutex_lock(&RuleStatusLock);
				RuleStatusFlag = 0;
				syslog(LOG_DEBUG, "[DM]: Dm_platform /'RuleStatusFlag/' [%d] ",
						RuleStatusFlag);
				pthread_mutex_unlock(&RuleStatusLock);

			} else if (RequestResponseOBJ.RuleSceneData == "PARTIAL") {

				syslog(LOG_INFO,
						"[DM]: Dm_platform rule execute complete for rule_id: [%s]",
						RequestResponseOBJ.Notif_Id.c_str());
				DmRuleMapping::DMSetRuleExecuteStatus(
						RequestResponseOBJ.Notif_Id);
				pthread_mutex_lock(&RuleStatusLock);
				RuleStatusFlag = 0;
				syslog(LOG_DEBUG, "[DM]: Dm_platform /'RuleStatusFlag/' [%d] ",
						RuleStatusFlag);
				pthread_mutex_unlock(&RuleStatusLock);

			} else {

				RuleStatusFlag = 0;
				syslog(LOG_INFO,
						"[DM]: Dm_PLARFORM sending Rule execution status CRON_HANDLER operation");
			}

			json_object *client_action = json_object_new_string(
					RequestResponseOBJ.Command.c_str());
			json_object_object_add(va_response, "client_name",
					json_object_new_string("DM"));
			json_object_object_add(va_response, "client_action", client_action);
			json_object_object_add(va_response, "client_data", client_data);
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform sending response data to \"CJ\" :(%s)",
					json_object_to_json_string(va_response));
			int ResultSend = zmq_send(DmZeroMqOBJ->CronHandlerResponse,
					json_object_to_json_string(va_response),
					strlen(json_object_to_json_string(va_response)), 0);
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform sending response to \"CJ\" with status code :[%d]",
					ResultSend);
			if (ResultSend == -1) {
				syslog(LOG_CRIT,
						"[DM]: DM_platform sending failed response to \"CJ\" : [%d], [%s]",
						errno, strerror(errno));
				zmq_close(DmZeroMqOBJ->CronHandlerResponse);
				void *CronHandlerResponse =
						(void *) DmZeroMqOBJ->ZeroMQDMClientsInit(
								(char*) "DM_VA_ZMQ_CLIENT",
								DmZeroMqOBJ->DM_VA_ZMQ_CLIENT);
				if (CronHandlerResponse != 0) {
					syslog(LOG_DEBUG,
							"[DM]: Dm_platform /'CJ/' Socket client Init Success");
				} else {
					syslog(LOG_ERR,
							"[DM]: Dm_platform /'CJ/' Socket client Init Failure");
				}

			} else {

				/** Clearing the buffer before receiving the response from VA */
				memset(&DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER, 0,
						sizeof(DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER));
				int ResultRecv = zmq_recv(DmZeroMqOBJ->CronHandlerResponse,
						DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER, BUF_MAX_128, 0);
				syslog(LOG_DEBUG,
						"[DM]: DM_platform receiving response data from \"CJ\" :[%s]",
						DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER);
				syslog(LOG_DEBUG,
						"[DM]: DM_platform receiving response from \"CJ\" with status code :[%d]",
						ResultRecv);
				if (ResultRecv == -1) {
					syslog(LOG_CRIT,
							"[DM]: DM_platform receiving failed response from \"CJ\" : [%d], [%s]",
							errno, strerror(errno));
					zmq_close(DmZeroMqOBJ->CronHandlerResponse);
					void *CronHandlerResponse =
							(void *) DmZeroMqOBJ->ZeroMQDMClientsInit(
									(char*) "DM_VA_ZMQ_CLIENT",
									DmZeroMqOBJ->DM_VA_ZMQ_CLIENT);
					if (CronHandlerResponse != 0) {
						syslog(LOG_DEBUG,
								"[DM]: Dm_platform /'CJ/' client Init Success");
					} else {
						syslog(LOG_ERR,
								"[DM]: Dm_platform /'CJ/' Socket client Init Failure");
					}
				}
			}

		}

	}
		break;
//	case AREA_CONTROL:
//	case RULE_COLTROL:
//	case SCENE_CONTROL:
//	case VOICE_ASSISTANCE: {
//
//		/*syslog(LOG_INFO,
//				"[DM]: DM_platform sending request response to VA start ");*/
//
//		std::string va_act_data = "DM_DEFAULT";
//		json_object* va_response = json_object_new_object();
//		json_object* client_data = json_object_new_object();
//		json_object* client_action = json_object_new_object();
//
//		json_object * led_obj = json_object_new_boolean(false);
//
//		if (RequestResponseOBJ.Status_Code == DM_SUCCESS) {
//
//			/*syslog(LOG_INFO,
//					"[DM]: DM_platform sending request response to va success start ");*/
//
//			//---------------------------------SUCCESS START--------------------------------
//
//			syslog(LOG_INFO,
//								"[DM]: DM_platform sending SUCCESS to VA for command : %s ",RequestResponseOBJ.Command);
//			if (RequestResponseOBJ.Command == DM_CONTROL
//					|| RequestResponseOBJ.Command == DM_REMOVE) {
//
//			/*	syslog(LOG_INFO,
//						"[DM]: DM_platform sending request-response to VA CONTROL/REMOVE start ");*/
//
//				va_act_data = "ok.mp3";
//				client_action = json_object_new_string("VOICE");
//				json_object * action_type = json_object_new_string(
//						va_act_data.c_str());
//
//				json_object_object_add(client_data, "offline", action_type);
//
//			} else if (RequestResponseOBJ.Command == DM_DISCOVERY) {
//
//				va_act_data = "Discovered 1 " + RequestResponseOBJ.RuleSceneData
//						+ ", Go to your Shifu App to configure";
//
//				client_action = json_object_new_string("VOICE");
//				json_object * action_type = json_object_new_string(
//						va_act_data.c_str());
//
//				json_object_object_add(client_data, "online", action_type);
//				json_object_object_add(client_data, "led", led_obj);
//
//			} else if (RequestResponseOBJ.Command == DM_SCENE_CONTROL
//					|| RequestResponseOBJ.Command == DM_VOICE_SCENE_CONTROL) {
//				/** Sending succesfully executed announcement to VA, if scene request initiated by both voice or mobile  */
//				if (RequestResponseOBJ.RuleSceneData == "SUCCESS") {
//
//					va_act_data = "scene execution success";
//					json_object_object_add(client_data, "led", led_obj);
//
//					client_action = json_object_new_string("VOICE");
//					json_object * action_type = json_object_new_string(
//							va_act_data.c_str());
//
//					json_object_object_add(client_data, "online", action_type);
//					syslog(LOG_INFO,
//							"[DM]: DM_platform SCENE EXECUTE COMPLETE FOR SCENE_ID: [%s]",
//							RequestResponseOBJ.Notif_Id.c_str());
//
//					DmSceneMapping DmSceneMappingobj;
//
//					DmSceneMapping::DMSetSceneMapExecuteflag(false);
//
//					DmSceneMapping::DMSetSceneExecuteStatus(
//							RequestResponseOBJ.Notif_Id);
//
//				} else if (RequestResponseOBJ.RuleSceneData == "FAIL") {
//
//					va_act_data = "scene execution failed";
//
//					client_action = json_object_new_string("VOICE");
//					json_object * action_type = json_object_new_string(
//							va_act_data.c_str());
//
//					json_object_object_add(client_data, "online", action_type);
//					json_object_object_add(client_data, "led", led_obj);
//					syslog(LOG_INFO,
//							"[DM]: DM_platform SCENE EXECUTE COMPLETE FOR SCENE_ID: [%s]",
//							RequestResponseOBJ.Notif_Id.c_str());
//
//					DmSceneMapping::DMSetSceneMapExecuteflag(false);
//
//					DmSceneMapping::DMSetSceneExecuteStatus(
//							RequestResponseOBJ.Notif_Id);
//
//				} else if (RequestResponseOBJ.RuleSceneData == "PARTIAL") {
//
//					va_act_data = "scene completed partially";
//
//					client_action = json_object_new_string("VOICE");
//					json_object * action_type = json_object_new_string(
//							va_act_data.c_str());
//
//					json_object_object_add(client_data, "online", action_type);
//					json_object_object_add(client_data, "led", led_obj);
//					syslog(LOG_INFO,
//							"[DM]: DM_platform SCENE EXECUTE COMPLETE FOR SCENE_ID: [%s]",
//							RequestResponseOBJ.Notif_Id.c_str());
//
//					DmSceneMapping::DMSetSceneMapExecuteflag(false);
//
//					DmSceneMapping::DMSetSceneExecuteStatus(
//							RequestResponseOBJ.Notif_Id);
//
//				} else {
//
//					json_object_object_add(client_data, "actions",
//							json_tokener_parse(
//									RequestResponseOBJ.RuleSceneData.c_str()));
//
//					/**
//					 *  Seperating the command based on the scene request while sending to VA
//					 */
//
//					if (RequestResponseOBJ.Command == "VOICE_SCENE") {
//						client_action = json_object_new_string("VOICE_SCENE");
//					} else {
//						client_action = json_object_new_string("SCENE");
//					}
//
//				}
//			} else if (RequestResponseOBJ.Command == DM_RULE_COLTROL) {
//
//				if (RequestResponseOBJ.RuleSceneData == "SUCCESS") {
//
////					va_act_data = "rule execution success";
////
////					client_action = json_object_new_string("VOICE");
////					json_object * action_type = json_object_new_string(
////							va_act_data.c_str());
////
////					json_object_object_add(client_data, "online", action_type);
////					json_object_object_add(client_data, "led", led_obj);
////
////					syslog(LOG_INFO,
////							"[DM]: Platform rule execute complete for rule_id: [%s]",
////							RequestResponseOBJ.Notif_Id.c_str());
//
//					DmRuleMapping::DMSetRuleExecuteStatus(
//							RequestResponseOBJ.Notif_Id);
//					pthread_mutex_lock(&RuleStatusLock);
//					RuleStatusFlag--;
//					syslog(LOG_INFO, "[DM]: RuleStatusFlag [%d] ",RuleStatusFlag);
//					pthread_mutex_unlock(&RuleStatusLock);
//
//				} else if (RequestResponseOBJ.RuleSceneData == "FAIL") {
//
//					va_act_data = "rule execution failed";
//
//					client_action = json_object_new_string("VOICE");
//					json_object * action_type = json_object_new_string(
//							va_act_data.c_str());
//
//					json_object_object_add(client_data, "online", action_type);
//					json_object_object_add(client_data, "led", led_obj);
//
//					syslog(LOG_INFO,
//							"[DM]: Platform rule execute complete for rule_id: [%s]",
//							RequestResponseOBJ.Notif_Id.c_str());
//
//					DmRuleMapping::DMSetRuleExecuteStatus(
//							RequestResponseOBJ.Notif_Id);
//					pthread_mutex_lock(&RuleStatusLock);
//					RuleStatusFlag--;
//					syslog(LOG_INFO, "[DM]: RuleStatusFlag [%d] ",RuleStatusFlag);
//					pthread_mutex_unlock(&RuleStatusLock);
//
//				} else if (RequestResponseOBJ.RuleSceneData == "PARTIAL") {
//
//					va_act_data = "rule completed partially";
//
//					client_action = json_object_new_string("VOICE");
//					json_object * action_type = json_object_new_string(
//							va_act_data.c_str());
//
//					json_object_object_add(client_data, "online", action_type);
//					json_object_object_add(client_data, "led", led_obj);
//
//					syslog(LOG_INFO,
//							"[DM]: Platform rule execute complete for rule_id: [%s]",
//							RequestResponseOBJ.Notif_Id.c_str());
//
//					DmRuleMapping::DMSetRuleExecuteStatus(
//							RequestResponseOBJ.Notif_Id);
//					pthread_mutex_lock(&RuleStatusLock);
//					RuleStatusFlag--;
//					syslog(LOG_INFO, "[DM]: RuleStatusFlag [%d] ",RuleStatusFlag);
//					pthread_mutex_unlock(&RuleStatusLock);
//
//				} else {
//
//					syslog(LOG_INFO,
//							"[DM]: DM_platform sending Rule execution status operation to VA");
//
//					client_data = json_tokener_parse(
//							RequestResponseOBJ.RuleSceneData.c_str());
//					client_action = json_object_new_string("RULE");
//				}
//
//			} else if (RequestResponseOBJ.Command == DM_AREA_CONTROL) {
//
//				//va_act_data = "area controlled successfully";
//				va_act_data = "ok.mp3";
//				client_action = json_object_new_string("VOICE");
//				json_object * action_type = json_object_new_string(
//						va_act_data.c_str());
//
//				json_object_object_add(client_data, "offline", action_type);
//			} else if (RequestResponseOBJ.Command == "S2_SECURITY") {
//
//
//				va_act_data =
//						"This device supports S two Security, Go to your shifu app to enter secure key";
//				client_action = json_object_new_string("VOICE");
//				json_object * action_type = json_object_new_string(
//						va_act_data.c_str());
//
//				json_object_object_add(client_data, "online", action_type);
//
//			}
//
//			//---------------------------------SUCCESS Scenarios Completed--------------------------------
//
//		} else {
//
//			//---------------------------------Error Scenarios started-------------------------------
//
//			syslog(LOG_INFO,
//					"[DM]: DM_platform sending ERROR response to VA for command: %s ",RequestResponseOBJ.Command);
//
//			if ((RequestResponseOBJ.Command == DM_DISCOVERY)
//					&& (RequestResponseOBJ.RuleSceneData == "DM_DEFAULT")) {
//
//				va_act_data = RequestResponseOBJ.Message;
//
//				client_action = json_object_new_string("VOICE");
//				json_object * action_type = json_object_new_string(
//						va_act_data.c_str());
//
//				json_object_object_add(client_data, "online", action_type);
//				led_obj = json_object_new_boolean(true);
//				json_object_object_add(client_data, "led", led_obj);
//
//			} else if ((RequestResponseOBJ.Command == DM_DISCOVERY)
//					&& (RequestResponseOBJ.RuleSceneData == "NULL")) {
//
//				va_act_data = "no device found";
//
//				client_action = json_object_new_string("VOICE");
//				json_object * action_type = json_object_new_string(
//						va_act_data.c_str());
//
//				json_object_object_add(client_data, "online", action_type);
//				json_object_object_add(client_data, "led", led_obj);
//
//			} else if (RequestResponseOBJ.Command == DM_CONTROL
//					|| RequestResponseOBJ.Command == "AREA") {
//				if (RequestResponseOBJ.RuleSceneData == "DM_DEFAULT") {
//					va_act_data = RequestResponseOBJ.Message;
//				} else {
//					va_act_data =
//							"device is not reachable, check device connected to power supply";
//				}
//				client_action = json_object_new_string("VOICE");
//				json_object * action_type = json_object_new_string(
//						va_act_data.c_str());
//
//				json_object_object_add(client_data, "online", action_type);
//				json_object_object_add(client_data, "led", led_obj);
//
//			} else if (RequestResponseOBJ.Command == DM_VOICE_SCENE_CONTROL) {
//
//				//va_act_data = "scene is already in progress";
//
//				//va_act_data = RequestResponseOBJ.Message;
//
//				//client_action = json_object_new_string("VOICE");
//				//json_object * action_type = json_object_new_string(
//						//va_act_data.c_str());
//
//				//json_object_object_add(client_data, "online", action_type);
//				//json_object_object_add(client_data, "led", led_obj);
//
//			} else if (RequestResponseOBJ.Command == DM_RULE_COLTROL) {
//
//				if (RequestResponseOBJ.Status_Code
//						== DISCOVERY_ALREADY_IN_PROCESS
//						|| RequestResponseOBJ.Status_Code
//								== SCENE_ALREADY_PROCESSING)
//					va_act_data = RequestResponseOBJ.Message;
//				else
//					va_act_data = "rule is already in progress";
//
//				client_action = json_object_new_string("VOICE");
//				json_object * action_type = json_object_new_string(
//						va_act_data.c_str());
//
//				json_object_object_add(client_data, "online", action_type);
//				json_object_object_add(client_data, "led", led_obj);
//
//			} else if (RequestResponseOBJ.Command == DM_AREA_CONTROL) {
//
//				va_act_data = "area control is already in progress";
//
//				client_action = json_object_new_string("VOICE");
//				json_object * action_type = json_object_new_string(
//						va_act_data.c_str());
//
//				json_object_object_add(client_data, "online", action_type);
//				json_object_object_add(va_response, "led", led_obj);
//			}
//
//			else if (RequestResponseOBJ.Command == DM_S2_SECURITY) {
//
//				syslog(LOG_INFO,
//						"[DM]: DM_platform DM_S2_SECURITY device is added non secure ");
//
//				va_act_data = "device is added non secure";
//
//				client_action = json_object_new_string("VOICE");
//				json_object * action_type = json_object_new_string(
//						va_act_data.c_str());
//
//				json_object_object_add(client_data, "online", action_type);
//			}
//
//			//---------------------------------ERROR END--------------------------------
//		}
//
//		//-------------------------------------------ZMQ_VA_STARTS-----------------------------------------------//
//
//		      json_object_object_add(va_response, "client_name",
//		          json_object_new_string("DM"));
//		      json_object_object_add(va_response, "client_action", client_action);
//		      json_object_object_add(va_response, "client_data", client_data);
//
//		      syslog(LOG_DEBUG, "[DM]: DM_platform sending response data to \"VA\" :[%s]",
//		            json_object_to_json_string(va_response));
//		    //  syslog(LOG_DEBUG, "[DM]: DM_platform sending response data length :[%d]",
//		    //        strlen(json_object_to_json_string(va_response)));
//		      int ResultSend = zmq_send(DmZeroMqOBJ->CronHandlerResponse,
//		          json_object_to_json_string(va_response),
//		          strlen(json_object_to_json_string(va_response)), 0);
//
//		      syslog(LOG_DEBUG, "[DM]: DM_platform sending response to \"VA\" with status code :[%d]",
//		          ResultSend);
//
//		      if (ResultSend == -1) {
//
//		        syslog(LOG_CRIT, "[DM]: DM_platform sending failed response to \"VA\" : [%d], [%s]",
//		        errno, strerror(errnum));
//		      }
//
//
//
//
//		      /** Clearing the buffer before receiving the response from VA */
//
//		      memset(&DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER, 0,
//		          sizeof(DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER));
//
//		      int ResultRecv = zmq_recv(DmZeroMqOBJ->CronHandlerResponse,
//		          DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER, BUF_MAX_128, 0);
//
//		      syslog(LOG_DEBUG, "[DM]: DM_platform receiving response data from \"VA\" :[%s]",
//		          DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER);
//		     /* syslog(LOG_DEBUG, "[DM]: DM_platform receiving response data length :[%d]",
//		              strlen(DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER));*/
//
//		      syslog(LOG_DEBUG, "[DM]: DM_platform receiving response from \"VA\" with status code :[%d]",
//		          ResultRecv);
//
//		      if (ResultRecv == -1) {
//		        syslog(LOG_CRIT, "[DM]: DM_platform receiving failed response from \"VA\" : [%d], [%s]",
//		              errno, strerror(errnum));
//		      }
//			//json_object_put(va_response);
//		    }
//		      break;
		//-------------------------------------------ZMQ_VA_ENDS-----------------------------------------------//

	case SOCKET_SERVICE: {

		syslog(LOG_INFO,
				"[DM]: Dm_platform sending request response to /'SS/'");
		if ((RequestResponseOBJ.Command == "REMOVE")
				|| (RequestResponseOBJ.Command == "EXCLUSION")
				|| (RequestResponseOBJ.Command == "FORCE_REMOVE")
				|| (RequestResponseOBJ.Command == "SECURITY")) { /**  force remove, remove, arm mode change and exclusion only response should be sent to Socket service  */
			json_object *ss_response = json_object_new_object();
			json_object *message_Id = json_object_new_string(
					RequestResponseOBJ.Message_Id.c_str());
			json_object *message = json_object_new_string(
					RequestResponseOBJ.Message.c_str());
			json_object *status_Code = json_object_new_int(
					RequestResponseOBJ.Status_Code);
			json_object *command = json_object_new_string(
					RequestResponseOBJ.Command.c_str());
			json_object *notif_id = json_object_new_string(
					RequestResponseOBJ.Notif_Id.c_str());
			if (RequestResponseOBJ.Command == "EXCLUSION") {
				json_object *thingsexc = json_object_new_array();
				json_object *thingsinfoexc = json_object_new_object();
				json_object *thing_id_exc = json_object_new_string(
						RequestResponseOBJ.Thing_Id.c_str());
				json_object_object_add(thingsinfoexc, "thing_id", thing_id_exc);
				json_object_array_add(thingsexc, thingsinfoexc);
				json_object_object_add(ss_response, "things", thingsexc);
			}
			//-------------------------------------------ZMQ_SS_STARTS-----------------------------------------------//

			json_object_object_add(ss_response, "client",
					json_object_new_int(RequestResponseOBJ.Client));
			json_object_object_add(ss_response, "notif_id", notif_id);
			json_object_object_add(ss_response, "command", command);
			json_object_object_add(ss_response, "message", message);
			json_object_object_add(ss_response, "message_id", message_Id);
			json_object_object_add(ss_response, "status_code", status_Code);
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform sending response data to \"SS\" :[%s]",
					json_object_to_json_string(ss_response));
			int ResultSend = zmq_send(DmZeroMqOBJ->SocketServerResponse,
					json_object_to_json_string(ss_response),
					strlen(json_object_to_json_string(ss_response)), 0);
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform sending response to \"SS\" with status code :[%d]",
					ResultSend);
			if (ResultSend == -1) {
				syslog(LOG_CRIT,
						"[DM]: Dm_platform sending failed response to \"SS\" : [%d], [%s]",
						errno, strerror(errno));
				zmq_close(DmZeroMqOBJ->SocketServerResponse);
				void *SocketServerResponse =
						(void *) DmZeroMqOBJ->ZeroMQDMClientsInit(
								(char*) "DM_SS_ZMQ_CLIENT",
								DmZeroMqOBJ->DM_SS_ZMQ_CLIENT);
				if (SocketServerResponse != 0) {
					syslog(LOG_DEBUG,
							"[DM]: Dm_platform /'SS/' Socket client Init Success");
				} else {
					syslog(LOG_ERR,
							"[DM]: Dm_platform /'SS/' Socket client Init Failure");
				}

			} else {
				/** Clearing the buffer before receiving the response from MA */
				memset(&DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER, 0,
						sizeof(DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER));
				int ResultRecv = zmq_recv(DmZeroMqOBJ->SocketServerResponse,
						DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER, BUF_MAX_128, 0);
				syslog(LOG_DEBUG,
						"[DM]: Dm_platform receiving response data from \"SS\" :[%s]",
						DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER);

				syslog(LOG_DEBUG,
						"[DM]: Dm_platform receiving response from \"SS\" with status code :[%d]",
						ResultRecv);
				if (ResultRecv == -1) {
					syslog(LOG_CRIT,
							"[DM]: Dm_platform receiving failed response from \"SS\" : [%d], [%s]",
							errno, strerror(errno));
					zmq_close(DmZeroMqOBJ->SocketServerResponse);
					void *SocketServerResponse =
							(void *) DmZeroMqOBJ->ZeroMQDMClientsInit(
									(char*) "DM_SS_ZMQ_CLIENT",
									DmZeroMqOBJ->DM_SS_ZMQ_CLIENT);
					if (SocketServerResponse != 0) {
						syslog(LOG_DEBUG,
								"[DM]: Dm_platform /'SS/' Socket client Init Success");
					} else {
						syslog(LOG_ERR,
								"[DM]: Dm_platform /'SS/' Socket client Init Failure");
					}
				}
			}
		}
	}
		break;
		//-------------------------------------------ZMQ_SS_ENDS-----------------------------------------------//

	case MOBILE_AGENT: {

		syslog(LOG_DEBUG,
				"[DM]: DM_platform sending request response to /'MA/' start");
		if ((RequestResponseOBJ.Command == "CONTROL")
				|| (RequestResponseOBJ.Command == "REMOVE")
				|| (RequestResponseOBJ.Command == "EXCLUSION")
				|| (RequestResponseOBJ.Command == "FORCE_REMOVE"
						|| RequestResponseOBJ.Command == "LEARN_MODE")) {
			/** Control, force remove, remove and exclusion only response should be sent to Mobile Agent  */

			json_object* ma_response = json_object_new_object();
			json_object *message_Id = json_object_new_string(
					RequestResponseOBJ.Message_Id.c_str());
			json_object *message = json_object_new_string(
					RequestResponseOBJ.Message.c_str());
			json_object *status_Code = json_object_new_int(
					RequestResponseOBJ.Status_Code);
			json_object *command = json_object_new_string(
					RequestResponseOBJ.Command.c_str());
			json_object *notif_id = json_object_new_string(
					RequestResponseOBJ.Notif_Id.c_str());
			if (RequestResponseOBJ.Command == "EXCLUSION") {
				json_object *thingsexc = json_object_new_array();
				json_object *thingsinfoexc = json_object_new_object();
				json_object *thing_id_exc = json_object_new_string(
						RequestResponseOBJ.Thing_Id.c_str());
				json_object_object_add(thingsinfoexc, "thing_id", thing_id_exc);
				json_object_array_add(thingsexc, thingsinfoexc);
				json_object_object_add(ma_response, "things", thingsexc);
			}

			else if (RequestResponseOBJ.Command == "LEARN_MODE") {

				json_object *key_info = json_object_new_array();
				json_object *key_obj = json_object_new_object();
				json_object *key_id = json_object_new_string(
						RequestResponseOBJ.queue_id.c_str());
				json_object_object_add(key_obj, "sec_key", key_id);
				json_object_array_add(key_info, key_obj);
				json_object_object_add(ma_response, "learn_key", key_info);
			}
			//-------------------------------------------ZMQ_MA_STARTS-----------------------------------------------//
			json_object_object_add(ma_response, "client",
					json_object_new_int(RequestResponseOBJ.Client));
			json_object_object_add(ma_response, "notif_id", notif_id);
			json_object_object_add(ma_response, "command", command);
			json_object_object_add(ma_response, "message", message);
			json_object_object_add(ma_response, "message_id", message_Id);
			json_object_object_add(ma_response, "status_code", status_Code);

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform sending response data to \"MA\" :[%s]",
					json_object_to_json_string(ma_response));
			int ResultSend = zmq_send(DmZeroMqOBJ->MobileAgentResponse,
					json_object_to_json_string(ma_response),
					strlen(json_object_to_json_string(ma_response)), 0);
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform sending response to \"MA\" with status code :[%d]",
					ResultSend);
			if (ResultSend == -1) {
				syslog(LOG_CRIT,
						"[DM]: Dm_platform sending failed response to \"MA\" : [%d], [%s]",
						errno, strerror(errno));
				zmq_close(DmZeroMqOBJ->MobileAgentResponse);
				void *MobileAgentResponse =
						(void *) DmZeroMqOBJ->ZeroMQDMClientsInit(
								(char*) "DM_MA_ZMQ_CLIENT",
								DmZeroMqOBJ->DM_MA_ZMQ_CLIENT);
				if (MobileAgentResponse != 0) {
					syslog(LOG_DEBUG,
							"[DM]: Dm_platform /'MA/' Socket client Init Success");
				} else {
					syslog(LOG_ERR,
							"[DM]: Dm_platform /'MA/' Socket client Init Failure");
				}

			} else {

				/** Clearing the buffer before receiving the response from MA */
				memset(&DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER, 0,
						sizeof(DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER));
				int ResultRecv = zmq_recv(DmZeroMqOBJ->MobileAgentResponse,
						DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER, BUF_MAX_128, 0);

				syslog(LOG_DEBUG,
						"[DM]: Dm_platform receiving response data from \"MA\" :[%s]",
						DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER);
				syslog(LOG_DEBUG,
						"[DM]: Dm_platform receiving response from \"MA\" with status code :[%d]",
						ResultRecv);
				if (ResultRecv == -1) {
					syslog(LOG_CRIT,
							"[DM]: Dm_platform receiving failed response from \"MA\" : [%d], [%s]",
							errno, strerror(errno));
					zmq_close(DmZeroMqOBJ->MobileAgentResponse);
					void *MobileAgentResponse =
							(void *) DmZeroMqOBJ->ZeroMQDMClientsInit(
									(char*) "DM_MA_ZMQ_CLIENT",
									DmZeroMqOBJ->DM_MA_ZMQ_CLIENT);
					if (MobileAgentResponse != 0) {
						syslog(LOG_DEBUG,
								"[DM]: Dm_platform /'MA/' socket client Init Success");
					} else {
						syslog(LOG_ERR,
								"[DM]: Dm_platform /'MA/' socket client Init Failure");
					}
				}
			}
		}
	}
		break;

	case FOTA_SERVICE: { /* Sending response to FOTA module via socket */
		//	DmThingMap* DmMsgNotifyMapObj = DmThingMap::GetInstance();
		syslog(LOG_DEBUG,
				"[DM]: DM_platform sending request response to /'FOTA_SERVICE/' start");
		json_object * fota_response = json_object_new_object();
		json_object_object_add(fota_response, "client",
				json_object_new_int(14));
		json_object_object_add(fota_response, "command",
				json_object_new_string("FOTA_RES"));
		json_object_object_add(fota_response, "thing_id",
				json_object_new_string(RequestResponseOBJ.Thing_Id.c_str()));
		if (strcmp(RequestResponseOBJ.Message.c_str(),
				"DM_FOTA_UPDATE_COMPLETE") == 0)
			json_object_object_add(fota_response, "status",
					json_object_new_int(0));
		else
			json_object_object_add(fota_response, "status",
					json_object_new_int(RequestResponseOBJ.Status_Code));
		json_object_object_add(fota_response, "version",
				json_object_new_string(
						RequestResponseOBJ.Firmware_version.c_str()));
		syslog(LOG_DEBUG, "[DM]: Dm_platform FOTA -> DM response object: [%s]",
				json_object_to_json_string(fota_response));
		int res_fota = sendto(sockfd, json_object_to_json_string(fota_response),
				strlen(json_object_to_json_string(fota_response)),
				MSG_CONFIRM, (const struct sockaddr *) &fota_servaddr,
				sizeof(fota_servaddr));
		syslog(LOG_DEBUG,
				"[DM]: Dm_platform FOTA -> DM response object status:[%d]",
				res_fota);
		system("rm -rf /exe_dir/ota-files/*"); /*Deleting all files from directory*/
		json_object_put(fota_response);

	}
		//-------------------------------------------ZMQ_MA_ENDS-----------------------------------------------//

		/*case MQTT_SERVICE: {

		 syslog(LOG_INFO,
		 "[DM]: Dmplatform sending request response to mqtt start");

		 json_object* mqtt_response = json_object_new_object();

		 json_object *message_Id = json_object_new_string(
		 RequestResponseOBJ.Message_Id.c_str());
		 json_object *message = json_object_new_string(
		 RequestResponseOBJ.Message.c_str());
		 json_object *status_Code = json_object_new_int(
		 RequestResponseOBJ.Status_Code);
		 json_object *command = json_object_new_string(
		 RequestResponseOBJ.Command.c_str());

		 json_object_object_add(mqtt_response, "command", command);
		 json_object_object_add(mqtt_response, "message", message);
		 json_object_object_add(mqtt_response, "message_id", message_Id);
		 json_object_object_add(mqtt_response, "status_code", status_Code);
		 //------------------------------------------------------------------------------------------
		 int ResultSend = zmq_send(DmZeroMqOBJ->MqttResponse,
		 json_object_to_json_string(mqtt_response),
		 strlen(json_object_to_json_string(mqtt_response)), 0);

		 syslog(LOG_DEBUG, "[DM]: Dmplatform sending response mqtt status:[%d]",
		 ResultSend);

		 syslog(LOG_DEBUG, "[DM]: Dmplatform sending response ack mqtt:[%s]",
		 json_object_to_json_string(mqtt_response));

		 if (ResultSend == -1) {
		 syslog(LOG_INFO, "[DM]: Dmplatform sending response mqtt reset");

		 syslog( LOG_ERR,
		 "[DM]: Dmplatform sending response mqtt errno: [%d]",
		 errno);
		 syslog( LOG_ERR,
		 "[DM]:  Dmplatform sending response mqtt errnum: [%d]",
		 strerror(errno));

		 //			zmq_close(DmZeroMqOBJ->CronHandlerResponse);
		 //			DmZeroMqOBJ->zeroMqDmClientMQTT();
		 }

		 * Clearing the buffer before recieving the response from MQTT
		 memset(&DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER, 0,
		 sizeof(DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER));

		 int ResultRecv = zmq_recv(DmZeroMqOBJ->MqttResponse,
		 DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER, BUF_MAX_128, 0);

		 syslog(LOG_DEBUG,
		 "[DM]: Dmplatform sending response ack mqtt status:[%d]",
		 ResultRecv);

		 syslog(LOG_DEBUG, "[DM]: Dmplatform sending response ack mqtt:[%s]",
		 DmZeroMqOBJ->DM_ZMQ_RSP_BUFFER);

		 if (ResultRecv == -1) {
		 syslog(LOG_INFO,
		 "[DM]: Dmplatform sending response ack mqtt reset");

		 syslog( LOG_ERR,
		 "[DM]: Dmplatform sending response ack mqtt errno: [%d]",
		 errno);
		 syslog( LOG_ERR,
		 "[DM]:  Dmplatform sending response ack mqtt errnum: [%d]",
		 strerror(errno));

		 //			zmq_close(DmZeroMqOBJ->MqttResponse);
		 //			DmZeroMqOBJ->zeroMqDmClientMQTT();
		 }
		 //------------------------------------------------------------------------------------------
		 }*/

		break;

	default: {

		syslog(LOG_INFO, "[DM]: DM_platform sending response default case");
	}
		break;
	}

}

void DmPlatform::DmSendingRequestAck(RequestResponse RequestResponseOBJ) {

	syslog(LOG_INFO,
			"[DM]: Dm_platform /'SENDING REQUEST-ACKNOWLEDGE/' start ");
	DmZeroMq *DmZeroMqObj = DmZeroMq::GetInstance();
	json_object *thingsinfo = json_object_new_object();
	json_object *notif_id = json_object_new_string(
			RequestResponseOBJ.Notif_Id.c_str());
	json_object *message = json_object_new_string(
			RequestResponseOBJ.Message.c_str());
	json_object *status_code = json_object_new_int(
			RequestResponseOBJ.Status_Code);
	json_object_object_add(thingsinfo, "message", message);
	json_object_object_add(thingsinfo, "notif_id", notif_id);
	json_object_object_add(thingsinfo, "status_code", status_code);
	syslog(LOG_DEBUG,
			"[DM]: Dm_platform sending ack response to requestor: [%s]",
			json_object_get_string(thingsinfo));
	size_t msgSize = strlen(json_object_to_json_string(thingsinfo));
	int ResultSend = zmq_send(DmZeroMqObj->ClientResponse,
			json_object_to_json_string(thingsinfo), msgSize, 0);
	syslog(LOG_DEBUG,
			"[DM]: Dm_platform sending ack response to requestor status: [%d]",
			ResultSend);
	////json_object_put(thingsinfo);

}

void DmPlatform::DeviceManagerFetchScenesRules() {

	syslog(LOG_INFO,
			"[DM]: Dm_platform fetch Rules and Scenes data by slef trigger using /'DM_SERVICE/'");
	json_object *client = json_object_new_int(15);
	json_object *ruledata = json_object_new_object();
	json_object *rulecommand = json_object_new_string("GETRULE");
	json_object_object_add(ruledata, "client", client);
	json_object_object_add(ruledata, "command", rulecommand);
	ZeroMessageRequestParse(json_object_get_string(ruledata));
	////json_object_put(ruledata);
	json_object *scenedata = json_object_new_object();
	json_object *commandscene = json_object_new_string("GETSCENE");
	json_object_object_add(scenedata, "client", client);
	json_object_object_add(scenedata, "command", commandscene);
	ZeroMessageRequestParse(json_object_get_string(scenedata));
	////json_object_put(scenedata);

}

bool DmPlatform::DeviceManagerDatabase() {

	syslog(LOG_INFO, "[DM]: Dm_platform device manager database ");
//	DmSceneMapping::DMSceneDatabaseInit(); /* initializing scene table to store the device data to be triggered self */
//	DmSceneMapping::DMRecoverMapFromSceneDB(); /* Recovering the Scene Table*/
	DmRuleMapping::DMRuleDatabaseInit(); /* Creating rule table to store the device data to be triggered self */
	DmRuleMapping::DMRecoverMapFromRuleDB(); /* Recovering the Routine table */
	DmThingsBackupDb * DmThingsBackupDbOBJ = DmThingsBackupDb::GetInstance();
	DmThingsBackupDbOBJ->CreateBackUpDB(); /* Creating DB to store the devices data based on thing ID */
	DmThingsBackupDbOBJ->RestoreThingBackupDB();
	return true;

}

void DmPlatform::DmFotaclient() {
	int num_of_bytes; /* number of bytes in message received */
	char buffer[MAXLINE]; /* Variable to store the size of incoming FOTA request*/
	// Creating socket file descriptor
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) { /* Check for the successful creation of the socket */
		syslog(LOG_CRIT, "[DM]: Dm_platform socket creation failed for FOTA");
	}
	memset(&fota_servaddr, 0, sizeof(fota_servaddr));
	fota_servaddr.sin_family = PF_INET; /* Filling socket client information for sending response*/
	fota_servaddr.sin_port = htons(FOTA_PORT);
	fota_servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	memset(&dm_addr, 0, sizeof(dm_addr));
	dm_addr.sin_family = PF_INET; /* Filling socket server information for receive request port*/
	dm_addr.sin_port = htons(DM_PORT);
	dm_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	// Bind the socket with the server address
	if (bind(sockfd, (const struct sockaddr *) &dm_addr, sizeof(dm_addr)) < 0) { /*Bind the socket with the server address*/
		syslog(LOG_CRIT, "[DM]: Dm_platform socket bind failed for FOTA");
	}
	while (1) {
		num_of_bytes = recvfrom(sockfd, (char *) buffer, MAXLINE, MSG_WAITALL,
		NULL, NULL); /*wait to receive the request from the FOTA for end device firmware update*/
		buffer[num_of_bytes] = '\0';
		syslog(LOG_INFO, "[DM]: FOTA -> Request received from FOTA is: %s",
				buffer);
		json_object* req = json_tokener_parse(buffer);
//		json_type type = json_object_get_type(req);
		json_object* command = json_object_object_get(req, "command"); /* get the command in the request json and check for its validity*/
		if ((strcmp(json_object_get_string(command), "FOTA_REQ") == 0)) {
			syslog(LOG_DEBUG,
					"[DM]: Dm_platform FOTA -> Socket initialization successful!");
			ZeroMessageRequestParse(buffer); /* Parsing the received json request*/
		}
	}
}

bool DmPlatform::DeviceManagerInitialize() {

	syslog(LOG_INFO,
			"[DM]: Dm_platform /'STARTS HERE FOR ARM BASED PROCESSOR /'");

#if DM_GET_ENV_PLATFROM == 1

//=====================================================================DM_DISCOVERY_TIMEOUT

	int ReturnValueDiscovery = get_shifu_uci_param(
			(char *) "DM_DISCOVERY_TIMEOUT", DM_DISCOVERY_TIMEOUT);
	if (ReturnValueDiscovery == 0) {
		syslog(LOG_DEBUG,
				"[DM]: DM_platform_discovery_timeout status:[%s] DM_DISCOVERY_TIMEOUT:[%d]",
				DM_DISCOVERY_TIMEOUT, ReturnValueDiscovery);
	} else {
		strcpy(DM_DISCOVERY_TIMEOUT, "63");
		syslog(LOG_DEBUG,
				"[DM]: DM_platform_discovery_timeout status:[%s] DM_DISCOVERY_TIMEOUT:[%d]",
				DM_DISCOVERY_TIMEOUT, ReturnValueDiscovery);

	}

//===============Checking DM_VA_START_CHECK ENV variable and starting the VA service if not running

	char DM_VA_START_CHECK[8];
	int ReturnValueVA_Start = get_shifu_uci_param((char *) "DM_VA_START_CHECK",
			DM_VA_START_CHECK);
	if (ReturnValueVA_Start == 0) {
		syslog(LOG_DEBUG, "[DM]: DM_platform_va start :[%s] DM_VA_START_CHECK ",
				DM_VA_START_CHECK);
		if (strcmp(DM_VA_START_CHECK, "0") == 0) {
			syslog(LOG_DEBUG, "[DM]: DM_platform_restarting VA service");
			system("/etc/init.d/va_service start");
			set_shifu_uci_param((char *) "DM_VA_START_CHECK", "1");
		}
	}

//=================================================================DM_CONTROL_TIMEOUT

	int ReturnValueControl = get_shifu_uci_param((char *) "DM_CONTROL_TIMEOUT",
			DM_CONTROL_TIMEOUT);
	if (ReturnValueControl == 0) {
		syslog(LOG_DEBUG,
				"[DM]: DM_platform_control_timeout status:[%s] DM_CONTROL_TIMEOUT IS:[%d]",
				DM_CONTROL_TIMEOUT, ReturnValueControl);

	} else {
		strcpy(DM_CONTROL_TIMEOUT, "6");
		syslog(LOG_DEBUG,
				"[DM]: DM_platform_control_timeout status:[%s] DM_CONTROL_TIMEOUT IS:[%d]",
				DM_CONTROL_TIMEOUT, ReturnValueControl);
	}

//====================================================================DM_REMOVE_TIMEOUT

	int ReturnValueRemove = get_shifu_uci_param((char *) "DM_REMOVE_TIMEOUT",
			DM_REMOVE_TIMEOUT);
	if (ReturnValueRemove == 0) {
		syslog(LOG_DEBUG,
				"[DM]: DM_platform remove_timeout status:[%s] DM_REMOVE_TIMEOUT IS:[%d]",
				DM_REMOVE_TIMEOUT, ReturnValueRemove);

	} else {
		strcpy(DM_REMOVE_TIMEOUT, "40");
		syslog(LOG_DEBUG,
				"[DM]: DM_platform_remove_timeout status:[%s] DM_REMOVE_TIMEOUT IS:[%d]",
				DM_REMOVE_TIMEOUT, ReturnValueRemove);

	}

//====================================================================DM_EXCLUSION_TIMEOUT

	int ReturnValueExclusion = get_shifu_uci_param((char *) "DM_REMOVE_TIMEOUT",
			DM_EXCLUSION_TIMEOUT);
	if (ReturnValueExclusion == 0) {
		syslog(LOG_DEBUG,
				"[DM]: Platform dm_exclusion_timeout status:[%s] DM_EXCLUSION_TIMEOUT IS:[%d]",
				DM_EXCLUSION_TIMEOUT, ReturnValueExclusion);

	} else {
		strcpy(DM_EXCLUSION_TIMEOUT, "63");
		syslog(LOG_DEBUG,
				"[DM]: Platform dm_exclusion_timeout status:[%s] DM_EXCLUSION_TIMEOUT IS:[%d]",
				DM_EXCLUSION_TIMEOUT, ReturnValueExclusion);

	}

//========================================================================DM_STACK_CHECK

	int ReturnValueStackCheck = get_shifu_uci_param((char *) "DM_STACK_CHECK",
			DM_STACK_CHECK);
	if (ReturnValueStackCheck == 0) {
		syslog(LOG_DEBUG,
				"[DM]: Platform check stack status:[%s]  DM_STACK_CHECK IS:[%d]",
				DM_STACK_CHECK, ReturnValueStackCheck);
	} else {
		strcpy(DM_STACK_CHECK, "33");
		syslog(LOG_DEBUG,
				"[DM]: Platform check stack status:[%s]  DM_STACK_CHECK IS:[%d]",
				DM_STACK_CHECK, ReturnValueStackCheck);

	}

//========================================================================DM_STACK_CHECK

	int ReturnValueKillSwitch = get_shifu_uci_param((char *) "DM_KILL_SWITCH",
			DM_KILL_SWITCH);
	if (ReturnValueKillSwitch == 0) {
		syslog(LOG_DEBUG,
				"[DM]: Platform kill_switch status:[%s]  DM_KILL_SWITCH IS:[%d]",
				DM_KILL_SWITCH, ReturnValueKillSwitch);
	} else {
		strcpy(DM_KILL_SWITCH, "");
		syslog(LOG_DEBUG,
				"[DM]: Platform kill_switch status:[%s]  DM_STACK_CHECK IS:[%d]",
				DM_KILL_SWITCH, ReturnValueKillSwitch);

	}

#if DEMO == 1
	//For Demo
	char Env_Variable[128] = {0};
	int ReturnValueEntranceDoorSens = get_shifu_uci_param((char *)"ENTRANCE_DOOR_SENSOR_ID",
			entrance_door_sensor_id);
	if (ReturnValueEntranceDoorSens == 0) {
		syslog(LOG_DEBUG, "[DM]: DM_platform check ENTRANCE_DOOR_SENSOR_ID :[%s] return value: [%d]",
				entrance_door_sensor_id,ReturnValueEntranceDoorSens);
		/*syslog(LOG_DEBUG,
		 "[DM]: DM_platform return value of the ENTRANCE_DOOR_SENSOR_ID IS:[%d]",
		 ReturnValueEntranceDoorSens);*/
	} else {
		strcpy(entrance_door_sensor_id, "");
		syslog(LOG_DEBUG, "[DM]: DM_platform check ENTRANCE_DOOR_SENSOR_ID:[%s] return value: [%d]",
				entrance_door_sensor_id,ReturnValueEntranceDoorSens);
		/*syslog(LOG_DEBUG,
		 "[DM]: DM_platform return value of the ENTRANCE_DOOR_SENSOR_ID IS:[%d]",
		 ReturnValueEntranceDoorSens);*/
	}
	//========================================================================ENTRANCE_DOOR_SENSOR_ID

	int ReturnValueExitDoorSens = get_shifu_uci_param((char *)"EXIT_DOOR_SENSOR_ID",
			exit_door_sensor_id);
	if (ReturnValueExitDoorSens == 0) {
		syslog(LOG_DEBUG, "[DM]: DM_platform check EXIT_DOOR_SENSOR_ID :[%s]  return value: [%d]",
				exit_door_sensor_id,ReturnValueExitDoorSens);
		/*syslog(LOG_DEBUG,
		 "[DM]: DM_platform return value of the EXIT_DOOR_SENSOR_ID IS:[%d]",
		 ReturnValueExitDoorSens);*/
	} else {
		strcpy(exit_door_sensor_id, "");
		syslog(LOG_DEBUG, "[DM]: DM_platform check EXIT_DOOR_SENSOR_ID:[%s]  return value: [%d]",
				exit_door_sensor_id,ReturnValueExitDoorSens);
		/*syslog(LOG_DEBUG,
		 "[DM]: DM_platform return value of the EXIT_DOOR_SENSOR_ID IS:[%d]",
		 ReturnValueExitDoorSens);*/
	}
	//========================================================================EXIT_DOOR_SENSOR_ID

	int ReturnValueEntranceDoorSensValue = get_shifu_uci_param(
			(char *)"ENTRANCE_DOOR_SENSOR_VALUE", entrance_door_sensor_value);
	if (ReturnValueEntranceDoorSensValue == 0) {
		syslog(LOG_DEBUG,
				"[DM]: DM_platform check ENTRANCE_DOOR_SENSOR_VALUE :[%s]  return value: [%d]",
				entrance_door_sensor_value,ReturnValueEntranceDoorSensValue);
		/*syslog(LOG_DEBUG,
		 "[DM]: DM_platform return value of the ENTRANCE_DOOR_SENSOR_VALUE IS:[%d]",
		 ReturnValueEntranceDoorSensValue);*/
	} else {
		strcpy(entrance_door_sensor_value, "");
		syslog(LOG_DEBUG,
				"[DM]: DM_platform check ENTRANCE_DOOR_SENSOR_VALUE:[%s]  return value: [%d]",
				entrance_door_sensor_value,ReturnValueEntranceDoorSensValue);
		/*syslog(LOG_DEBUG,
		 "[DM]: DM_platform return value of the ENTRANCE_DOOR_SENSOR_VALUE IS:[%d]",
		 ReturnValueEntranceDoorSensValue);*/
	}
	//========================================================================ENTRANCE_DOOR_SENSOR_VALUE

	int ReturnValueExitDoorSensValue = get_shifu_uci_param((char *)"EXIT_DOOR_SENSOR_VALUE",
			exit_door_sensor_value);
	if (ReturnValueExitDoorSensValue == 0) {
		syslog(LOG_DEBUG, "[DM]: DM_platform check EXIT_DOOR_SENSOR_VALUE :[%s]  return value: [%d]",
				exit_door_sensor_value,ReturnValueExitDoorSensValue);
		/*syslog(LOG_DEBUG,
		 "[DM]: DM_platform return value of the EXIT_DOOR_SENSOR_VALUE IS:[%d]",
		 ReturnValueExitDoorSensValue);*/
	} else {
		strcpy(exit_door_sensor_value, "");
		syslog(LOG_DEBUG, "[DM]: DM_platform check EXIT_DOOR_SENSOR_VALUE:[%s]  return value: [%d]",
				Env_Variable,ReturnValueExitDoorSensValue);
		/*syslog(LOG_DEBUG,
		 "[DM]: DM_platform return value of the EXIT_DOOR_SENSOR_VALUE IS:[%d]",
		 ReturnValueExitDoorSensValue);*/
	}
	//========================================================================EXIT_DOOR_SENSOR_VALUE

	int ReturnValueKillSwitchId = get_shifu_uci_param((char *)"KILL_SWITCH_ID",
			kill_switch_device_id);
	if (ReturnValueKillSwitchId == 0) {
		syslog(LOG_DEBUG, "[DM]: DM_platform check KILL_SWITCH_ID :[%s]  return value: [%d]",
				kill_switch_device_id,ReturnValueKillSwitchId);
		/*syslog(LOG_DEBUG,
		 "[DM]: DM_platform return value of the KILL_SWITCH_ID IS:[%d]",
		 ReturnValueKillSwitchId);*/
	} else {
		strcpy(kill_switch_device_id, "");
		syslog(LOG_DEBUG, "[DM]: DM_platform check KILL_SWITCH_ID:[%s]",
				kill_switch_device_id);
		/*syslog(LOG_DEBUG,
		 "[DM]: DM_platform return value of the KILL_SWITCH_ID IS:[%d]",
		 ReturnValueKillSwitchId);*/
	}
	//========================================================================KILL_SWITCH_ID

#endif

#else

	strcpy(DM_DISCOVERY_TIMEOUT, "63");
	strcpy(DM_REMOVE_TIMEOUT, "63");
	strcpy(DM_CONTROL_TIMEOUT, "6");
	strcpy(DM_EXCLUSION_TIMEOUT, "63");
	strcpy(DM_STACK_CHECK, "33");

#endif
	return true;
}

//#if DM_GET_ENV_PLATFROM == 1
//
//#ifdef __cplusplus
//extern "C" {
//#endif
//#include <stdio.h>
//
//#include "sm_iface.h"
//
//#ifdef __cplusplus
//}
//#endif
//
//#define SM_INTERFACE  "sm_interface"
//
//extern int sm_socket_init();
//void sm_event_call();
//int sm_register_obj(char *);
//
//#endif

bool DmPlatform::DMNetworkStatusSM() {

	syslog(LOG_INFO,
			"[DM]: Dm_platform /'STARTS HERE FOR NETWORK STATUS CHECK/' ");

//#if DM_GET_ENV_PLATFROM == 1

//	syslog(LOG_DEBUG,
//			"[DM]: DM_platform Main service manager network status: [%s] service manager return object [%d]",
//			stat_net, intf_obj);
//
//	if (strcmp(stat_net, "null") != 0) {
//
//		json_object* SM_Status = json_tokener_parse(stat_net);
//
//		json_object* status = json_object_object_get(SM_Status, "status");
//
//		if (json_object_get_int(status) == 1) {
//
//			syslog(LOG_INFO,
//					"[DM]: DM_platform Main syslog service manager network status success end");
//
//			return true;
//
//		} else if (json_object_get_int(status) == 0) {
//
//			syslog(LOG_INFO,
//					"[DM]: DM_platform Main syslog service manager network status fail end");
//
//			return false;
//		}
//		////json_object_put(SM_Status);
//	} else {
//		return false;
//	}

	bool networkflag;
	if (system("ping -c 1 8.8.8.8")) {
		networkflag = true;
		std::cout << "flag if " << networkflag << "\n";
		syslog(LOG_INFO, "[DM]: Dm_platform Internet flag if case [%d]",
				networkflag);
	} else {
		networkflag = false;
		std::cout << "flag else " << networkflag << "\n";
		syslog(LOG_INFO, "[DM]: Dm_platform Internet flag else case[%d]",
				networkflag);
	}

	if (networkflag == false) {
		syslog(LOG_INFO, "[DM]: Dm_platform Internet is there");

		return true;

	} else {
		syslog(LOG_ERR, "[DM]: Dm_platform Internet is not there");
		return false;
	}

//#endif
}

//void DmPlatform::ServiceManagerInit() {
//
//	syslog(LOG_INFO,
//			"[DM]: DM_platform Main syslog service manager init start");
//
////#if DM_GET_ENV_PLATFROM == 1
//
////	sm_socket_init();
////
////	sm_register_obj((char *)SM_INTERFACE);
////
////	sm_event_call();
////
////	uloop_run();
////
////	ubus_free (ctx);
////
////	uloop_done();
//
////#endif
//
//}

/** TronxRemoveThingSync used to sync/remove thing from TronX which are not updated */
void DmPlatform::TronxRemoveThingSync() {

	syslog(LOG_INFO, "[DM]: Dm_platform TronxRemoveThingSync checking");
	DmThingsBackupDb * DmThingsBackupDbOBJ = DmThingsBackupDb::GetInstance();
	std::vector<std::string> ListtoRemovefromTronx =
			DmThingsBackupDbOBJ->ListOfRemoveSyncThingId(); /** Getting the list of thing_id to be updated to TronX */
	if (ListtoRemovefromTronx.size() > 0) {
		for (unsigned int size_of_list = 0;
				size_of_list < ListtoRemovefromTronx.size(); size_of_list++) { /** Iterating the list of thing_id to be updated to TronX */

			syslog(LOG_DEBUG,
					"[DM]: Dm_platform TronxRemoveThingSync Thing_ID to be removed from TronX : [%s] ",
					ListtoRemovefromTronx[size_of_list].c_str());
			DmReqResMapping* DmReqResMappingOBJ =
					DmReqResMapping::GetInstance();
			std::string notifid_dummy = std::to_string(rand() % 900);
			std::string msgid_dummy = std::to_string(DM_SERVICE)
					+ std::string("00") + std::to_string(rand() % 900);
			DmReqResMappingOBJ->AddToDmReqResMapping(notifid_dummy, msgid_dummy,
					1); /** Adding notif id and message id into request response count map*/
			RequestResponse RequestResponseOBJ;
			RequestResponseOBJ.Notif_Id = notifid_dummy;
			RequestResponseOBJ.Message_Id = msgid_dummy;
			RequestResponseOBJ.Status_Code = DM_SUCCESS;
			RequestResponseOBJ.Command = DM_REMOVE;
			RequestResponseOBJ.Thing_Id = ListtoRemovefromTronx[size_of_list];
			json_object *thingsinfo = json_object_new_object();
			json_object *thing_Id = json_object_new_string(
					RequestResponseOBJ.Thing_Id.c_str());
			json_object_object_add(thingsinfo, "thing_id", thing_Id);
			DmResponseQueue* DmResponseQueueOBJ =
					DmResponseQueue::GetInstance();
			DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ,
					"DELETE", "/hub/thing/remove",
					json_object_get_string(thingsinfo),
					PRORITY_VALUE);
			////json_object_put(thingsinfo);
		}

	}
	syslog(LOG_DEBUG,
			"[DM] Dm_platform vector size [%zu] and its capacity  BEFORE: [%zu]=======PT",
			ListtoRemovefromTronx.size(), ListtoRemovefromTronx.capacity());
	ListtoRemovefromTronx.shrink_to_fit();
	syslog(LOG_DEBUG,
			"[DM] Dm_platform vector size [%zu] and its capacity  AFTER: [%zu]=======PT",
			ListtoRemovefromTronx.size(), ListtoRemovefromTronx.capacity());
}

