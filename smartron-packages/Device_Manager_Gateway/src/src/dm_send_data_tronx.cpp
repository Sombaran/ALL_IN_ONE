#include "dm_send_data_tronx.hpp"
#include "dm_platform.hpp"
#include "dm_thingmap.hpp"
#include "dm_things_backup_db.hpp"
#include "dm_secne_mapping.hpp"

pthread_mutex_t SendRespQueueRcvMutx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t SendRespMutex3Cond = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t SendRespRcvCond = PTHREAD_COND_INITIALIZER;
std::string Post2TronxStr;

DmSendDataTronx * DmSendDataTronx::Instance = 0;

/*
 * 1 ARM
 * 2 LINUX
 * */

#define DM_ARM_SENDRESP 1

int Post2tronx_sent;

char datatopost[MAX_BUFSIZE_6144];

/*DmSendDataTronx::DmSendDataTronx() {   // TODO Auto-generated destructor stub
}
DmSendDataTronx::~DmSendDataTronx() {	// TODO Auto-generated destructor stub
}*/

DmSendDataTronx * DmSendDataTronx::GetInstance() {
	if (Instance == 0) {
		Instance = new DmSendDataTronx();
	}
	return Instance;
}

bool DmSendDataTronx::CurlInit() {

#if DM_ARM_SENDRESP == 1

	get_shifu_uci_param((char *)"HOME_TOKEN", X_HOME_TOKEN_VALUE);
	get_fw_env((char *)"DEVICE_ID", X_DEVICE_ID_VALUE);
	get_shifu_uci_param((char *)"TRONX_URL", X_BASE_URL_VALUE);

#elif DM_ARM_SENDRESP == 2

	strcpy(X_HOME_TOKEN_VALUE,
			"shifu_access_cc962442-f966-43a7-b7f4-3619f5a0600d");

	strcpy(X_DEVICE_ID_VALUE, "S119300161");

	strcpy(X_BASE_URL_VALUE, "https://tronxapi-staging.smartron.com/v2");

#endif

	syslog(LOG_DEBUG, "[DM]: MAIN ENV X-HOMETOKEN:[%s]", X_HOME_TOKEN_VALUE);
	syslog(LOG_DEBUG, "[DM]: MAIN ENV X-BASEURL :[%s]", X_BASE_URL_VALUE);
	syslog(LOG_DEBUG, "[DM]: MAIN ENV X-DEVICE_ID :[%s]", X_DEVICE_ID_VALUE);

	sprintf(X_HOME_TOKEN, "X-HomeToken: %s", X_HOME_TOKEN_VALUE);

	sprintf(X_DEVICE_ID, "X-DeviceId: %s", X_DEVICE_ID_VALUE);

	sprintf(CONTENT_TYPE, "Transfer-Encoding: %s", CONTENT_TYPE_VALUE);

	/** get a curl handle */
	curl = curl_easy_init();
	if (curl == NULL) {
		syslog(LOG_ERR, "[DM]: CURL init failed");
		return false;
	} else {
		syslog(LOG_ERR, "[DM]: CURL init success");
		return true;
	}
}

size_t DmSendDataTronx::WriteMemoryCallback(void *contents, size_t size,
		size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *) userp;

	mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL) {
		/* out of memory! */
		syslog(LOG_INFO,
				"[DM]: TRONX API not enough memory to store received cloud data");
		return 0;
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

size_t DmSendDataTronx::ReadDataCallback(void *dest, size_t size,
		size_t nmemb, void *userp) {

	char *temp_ptr = (char *) dest;

	if (Post2tronx_sent == 1) {
		Post2tronx_sent = 0;
		return 0;
	} else {
		memcpy(temp_ptr, datatopost, strlen(datatopost));
		Post2tronx_sent = 1;
		return strlen(datatopost);
	}
}

std::string DmSendDataTronx::updateTronxDatabase(
		ReqMessageData ReqMessageDataOBJ) {

	syslog(LOG_INFO, "[DM]: Dm_sendtotronX  /'SENDING TO TRONX VIA GET, PUT, POST, DELETE/' start");

	syslog(LOG_DEBUG,
			"[DM]:Dm_sendtotronX sending response to cloud of length: [%d]  and data [%s]",
			(int) strlen(ReqMessageDataOBJ.ReponseData.c_str()), ReqMessageDataOBJ.ReponseData.c_str());

	struct curl_slist *header_data = NULL;
	long response_code;
	struct MemoryStruct Post2tronx_chunk;
	Post2tronx_chunk.memory = (char *)malloc(sizeof(char*));
	Post2tronx_chunk.size = 0; /* no data at this point */
	time_t current_time = time(NULL);

	sprintf(C_TIME_VALUE, "%ld", current_time);

	sprintf(X_REQUEST_ID, "X-RequestId: %s%s%s", X_DEVICE_ID_VALUE,
			CLIENT_NAME_VALUE, C_TIME_VALUE);

	sprintf(URL_NAME, "%s%s", X_BASE_URL_VALUE,
			ReqMessageDataOBJ.ResponseURI.c_str());

	curl_easy_setopt(curl, CURLOPT_URL, URL_NAME);
	memset(datatopost, '\0', sizeof(datatopost));
	strcpy(datatopost, ReqMessageDataOBJ.ReponseData.c_str());
	if (strcmp(ReqMessageDataOBJ.ResponseMethod.c_str(), "POST") == 0) {

		syslog(LOG_INFO, "[DM]: Dm_sendtotronX Sending response via /'POST/'");
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadDataCallback);
		header_data = curl_slist_append(header_data,
				"Content-Type: application/json");

	} else if (strcmp(ReqMessageDataOBJ.ResponseMethod.c_str(), "PUT") == 0) {

		syslog(LOG_INFO, "[DM]: Dm_sendtotronX Sending response via /'PUT/'");
		curl_easy_setopt(curl, CURLOPT_PUT, 1L);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadDataCallback);
		curl_easy_setopt(curl, CURLOPT_READDATA, datatopost); 
		header_data = curl_slist_append(header_data,
				"Content-Type: application/json");

	} else if (strcmp(ReqMessageDataOBJ.ResponseMethod.c_str(), "DELETE")
			== 0) {

		syslog(LOG_INFO, "[DM]: Dm_sendtotronX Sending response via /'DELETE/'");
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, datatopost);
		header_data = curl_slist_append(header_data,
				"Content-Type: application/json");

	} else if (strcmp(ReqMessageDataOBJ.ResponseMethod.c_str(), "GET") == 0) {

		syslog(LOG_INFO, "[DM]: Dm_sendtotronX Sending response via /'GET/'");
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
	}
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void * ) &Post2tronx_chunk);
	header_data = curl_slist_append(header_data, X_REQUEST_ID);
	header_data = curl_slist_append(header_data, X_DEVICE_ID);
	header_data = curl_slist_append(header_data, X_HOME_TOKEN);
	header_data = curl_slist_append(header_data, CONTENT_TYPE);
	res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_data);
	if (res != CURLE_OK) {

		syslog(LOG_ERR,
				"[DM]: Dm_sendtotronX response TronX tts headers appending failed");
		return "DM_DEFAULT";
	}

	res = curl_easy_perform(curl);
	curl_slist_free_all(header_data); /* free the list again */
	syslog(LOG_DEBUG,
			"[DM]: Dm_sendtotronX rceived bytes from cloud: [%ld]",
			Post2tronx_chunk.size);
	if (res != CURLE_OK) {
		syslog(LOG_CRIT,
				"[DM]: Dm_sendtotronX curl perform failed: with message [%s] and error number [%d] ",
				curl_easy_strerror(res), res);

		if (res == CURLE_OPERATION_TIMEDOUT) {

			syslog(LOG_CRIT, "[DM]: Dm_sendtotronX timeout reached");
			return Post2TronxStr = "TIMEOUT";

		} else if (res == CURLE_COULDNT_CONNECT
				|| res == CURLE_COULDNT_RESOLVE_HOST) {

			syslog(LOG_CRIT,
					"[DM]: Dm_sendtotronX curl could not connect");
			return Post2TronxStr = "HOST_NOT_FOUND";
		}

	} else {

		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
		syslog(LOG_DEBUG, "[DM]: Dm_sendtotronX response code: [%ld]",
				response_code);

		Post2TronxStr = std::string(Post2tronx_chunk.memory);
	}
	free(Post2tronx_chunk.memory);	
	curl_easy_reset(curl);
	//syslog(LOG_INFO, "[DM]: Sending response TronX curl reset done");
	return Post2TronxStr;
}

//----------------------------------------------------------------------------------------------------------------------------------------

bool DmSendDataTronx::DmSendDataTronxInit() {

	ReqMessageData init;
	init.ResponseMethod = "Initial message, ignore this.";
	init.ResponseURI = "Initial message, ignore this.";
	init.priority = 0;
	init.ReponseData = "Initial message, ignore this.";
	init.Thing_ID = "Initial message, ignore this.";
	init.Command = "Initial message, ignore this.";

	DMSendDataTronx.push_back(init);

	/*syslog(LOG_DEBUG, "[DM]: DMSENDDATATRONX INIT MESSAG QUEUE SIZE : [%ld]",
			DMSendDataTronx.size());*/

	return true;
}

void DmSendDataTronx::DmSendheapify(
		std::vector<ReqMessageData> &DMSendDataTronx, int i, int n) {
	ReqMessageData temp;
	temp = DMSendDataTronx[i];

	int j = 2 * i;
	while (j <= n) {
		if (j < n
				&& DMSendDataTronx[j + 1].priority
						< DMSendDataTronx[j].priority) {
			j = j + 1;
		}

		if (temp.priority <= DMSendDataTronx[j].priority) {
			break;
		} else if (temp.priority > DMSendDataTronx[j].priority) {
			DMSendDataTronx[j / 2] = DMSendDataTronx[j];
			j = 2 * j;
		}
	}

	DMSendDataTronx[j / 2] = temp;
//	DMSendDataTronx.shrink_to_fit();
}

void DmSendDataTronx::DmSendheapsort(
		std::vector<ReqMessageData> &DMSendDataTronx, int n) {
	int i;
	ReqMessageData temp;

	for (i = n; i >= 2; i--) {
		temp = DMSendDataTronx[i];

		if (DMSendDataTronx[1].priority != DMSendDataTronx[i].priority) {
			DMSendDataTronx[i] = DMSendDataTronx[1];

			DMSendDataTronx[1] = temp;

			DmSendheapify(DMSendDataTronx, 1, i - 1);
		}
	}
//	DMSendDataTronx.shrink_to_fit();
}

void DmSendDataTronx::DmSendbuild_heap(
		std::vector<ReqMessageData> &DMSendDataTronx, int n) {
	for (int i = n / 2; i >= 1; i--) {
		DmSendheapify(DMSendDataTronx, i, n);
	}
//	DMSendDataTronx.shrink_to_fit();
}

void DmSendDataTronx::DmSendfilter(std::vector<ReqMessageData> &DMSendDataTronx,
		std::vector<MessageIndex> &DMSendsIndexTronx, std::string id,
		bool &update, int &position) {

	update = true;
	size_t counter;

	for (size_t i = 0; i < DMSendsIndexTronx.size(); i++) {
		if (id == DMSendsIndexTronx[i].queue_id) {
			if (DMSendsIndexTronx[i].repeatAllowedStatus == 0
					&& DMSendsIndexTronx[i].count == 1) {
				update = false;
				counter = 1;
				while (counter < DMSendDataTronx.size()) {
					if (id == DMSendDataTronx[counter].queue_id) {

						syslog(LOG_DEBUG,
								"[DM]: DMSENDDATATRONX RESPONSE FILTER FOR THE SAME : [%s]",
								DMSendDataTronx[counter].queue_id.c_str());

						DMSendDataTronx.erase(
								DMSendDataTronx.begin() + counter);
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
			"[DM] : The vector size [%d] and its capacity BEFORE: [%d]=======TRX",
			DMSendDataTronx.size(), DMSendDataTronx.capacity());
	DMSendDataTronx.shrink_to_fit();
	syslog(LOG_DEBUG,
			"[DM] : The vector size [%d] and its capacity AFTER: [%d]=======TRX",
			DMSendDataTronx.size(), DMSendDataTronx.capacity());
	syslog(LOG_DEBUG,
			"[DM] : The vector size [%d] and its capacity BEFORE: [%d]=======TRX",
			DMSendsIndexTronx.size(), DMSendsIndexTronx.capacity());
	DMSendsIndexTronx.shrink_to_fit();
	syslog(LOG_DEBUG,
			"[DM] : The vector size [%d] and its capacity AFTER: [%d]=======TRX",
			DMSendsIndexTronx.size(), DMSendsIndexTronx.capacity());*/
}

void DmSendDataTronx::DmSendAddMessage(ReqMessageData ReqMessageDataOBJ) {

	/*syslog(LOG_INFO,
			"[DM]: Send data TronX start adding message into queue start");*/

	pthread_mutex_lock(&SendRespQueueRcvMutx);

	/*syslog(LOG_INFO,
			"[DM]: Send data TronX start adding message from queue lock sendrespqueuercvmutx");*/

	bool updateStatus = false;
	int position = 0;
	bool ItemFlag = false;

	syslog(LOG_DEBUG,
			"[DM]: Send data TronX before adding message queue size: [%ld]",
			DMSendDataTronx.size());

	if (strcmp(ReqMessageDataOBJ.ResponseURI.c_str(), "/hub/rule/list") == 0) {
		ReqMessageDataOBJ.queue_id = "RULE_LIST";
	} else if (strcmp(ReqMessageDataOBJ.ResponseURI.c_str(),
			"/hub/scene/listall") == 0) {
		ReqMessageDataOBJ.queue_id = "SCENE_LIST";
	}

	for (size_t i = 0; i < DMSendsIndexTronx.size(); i++) {
		if (ReqMessageDataOBJ.queue_id == DMSendsIndexTronx[i].queue_id) {
			ItemFlag = true;
		}
	}

	if (ItemFlag == false) {
		MessageIndex temp;
		temp.queue_id = ReqMessageDataOBJ.queue_id;
		temp.repeatAllowedStatus = ManyRequest;
		temp.count = 0;
		DMSendsIndexTronx.push_back(temp);
	}

	DmSendfilter(DMSendDataTronx, DMSendsIndexTronx, ReqMessageDataOBJ.queue_id,
			updateStatus, position);

	syslog(LOG_DEBUG,
			"[DM]: Send data TronX adding into message queue response method : [%s] "
			"response uri : [%s] response data : [%s] command : [%s] thing_id : [%s]",
			ReqMessageDataOBJ.ResponseMethod.c_str(),
			ReqMessageDataOBJ.ResponseURI.c_str(),
			ReqMessageDataOBJ.ReponseData.c_str(),
			ReqMessageDataOBJ.Command.c_str(),
			ReqMessageDataOBJ.Thing_ID.c_str());

	DMSendDataTronx.push_back(ReqMessageDataOBJ);
	syslog (LOG_DEBUG,"[DM] Dm_SendtotronX queue_capacity [%zu]", DMSendDataTronx.capacity());
	syslog (LOG_DEBUG,"[DM] Dm_SendtotronX TronX queue_size [%zu]", DMSendDataTronx.size());

	/*syslog(LOG_DEBUG,
			"[DM]: Send data TronX after adding message queue size: [%ld]",
			DMSendDataTronx.size());*/

	int size = (DMSendDataTronx.size() - 1);
	DmSendbuild_heap(DMSendDataTronx, size);
	DmSendheapsort(DMSendDataTronx, size);

	if (updateStatus) {
		DMSendsIndexTronx[position].count++;
	}
	if (RspQueueSizeflag == 0) {
		pthread_cond_signal(&SendRespRcvCond);
		RspQueueSizeflag = 1;
	}

	pthread_mutex_unlock(&SendRespQueueRcvMutx);

	/*syslog(LOG_INFO,
			"[DM]: Send data TronX start receiving message from queue unlock sendrespqueuercvmutx");

	syslog(LOG_INFO,
			"[DM]: DMSENDDATATRONX start adding message into queue end");*/
}

void DmSendDataTronx::DmSendRemoveMessage() {

	for (size_t i = 0; i < DMSendsIndexTronx.size(); i++) {
		if (DMSendsIndexTronx[i].queue_id == DMSendDataTronx[1].queue_id) {

			syslog(LOG_DEBUG,
					"[DM]: Send data TronX request remove from request queue: [%s]",
					DMSendDataTronx[1].queue_id.c_str());

			DMSendsIndexTronx[i].count--;
			break;
		}
	}
	DMSendDataTronx.erase(DMSendDataTronx.begin() + 1);
}

void DmSendDataTronx::DmSendReceiveMessage() {

	ReqMessageData ReqMessageDataOBJ;
	std::string TronxSendRet;

	if (DMSendDataTronx.size() == 1) {

		/*syslog(LOG_INFO,
				"[DM]: Send data TronX receiving queue is empty waiting to add message");*/
		RspQueueSizeflag = 0;
		pthread_cond_wait(&SendRespRcvCond, &SendRespMutex3Cond);

		//SWAMI usleep(300);
	}

	pthread_mutex_lock(&SendRespQueueRcvMutx);

	/*syslog(LOG_INFO,
			"[DM]: Send data TronX start receiving message from queue lock sendrespqueuercvmutx");

	syslog(LOG_DEBUG,
			"[DM]: Send data TronX before receiving message queue size: [%ld]",
			DMSendDataTronx.size());*/

	ReqMessageDataOBJ = DMSendDataTronx[1];

	syslog(LOG_DEBUG,
			"[DM]: Dm_SendtotronX receiving from message queue response method : [%s] "
			"responseu ri : [%s] response data : [%s] command : [%s] thing_id : [%s]",
			ReqMessageDataOBJ.ResponseMethod.c_str(),
			ReqMessageDataOBJ.ResponseURI.c_str(),
			ReqMessageDataOBJ.ReponseData.c_str(),
			ReqMessageDataOBJ.Command.c_str(),
			ReqMessageDataOBJ.Thing_ID.c_str());

	DmSendRemoveMessage();

	/*syslog(LOG_DEBUG,
			"[DM]: Send data TronX after receiving message queue size: [%ld]",
			DMSendDataTronx.size());*/

	pthread_mutex_unlock(&SendRespQueueRcvMutx);

	/*syslog(LOG_INFO,
			"[DM]: Send data TronX start receiving message from queue unlock sendrespqueuercvmutx");*/

	time_t NetWait = time(NULL);

	while (!DmPlatform::DMNetworkStatusSM()) {

		syslog(LOG_DEBUG,
				"[DM]: Response queue process waiting for network come alive ...:[%d]",
				(int) (time(NULL) - NetWait));

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));


		if (DmPlatform::DMNetworkStatusSM() == true) {
			sleep(20);// Sleep required to initialize the CURL and send response
			break;
		}
	}

	TronxSendRet = updateTronxDatabase(ReqMessageDataOBJ);

	/*syslog(LOG_INFO,
			"[DM]: Response queue process response from TronX for operation: [%s]",
			ReqMessageDataOBJ.Command.c_str());*/

	syslog(LOG_DEBUG, "[DM]: Response queue process response from TronX: [%s]",
			TronxSendRet.c_str());

	/*syslog(LOG_DEBUG,
			"[DM]: Response queue process response size receiving from TronX: [%ld]",
			TronxSendRet.size());*/

	std::cout << "[DM]: Response queue process RESPONSE FROM TRONX :"
			<< std::endl << TronxSendRet << std::endl;

	json_object *jobj = json_tokener_parse(TronxSendRet.c_str());

    /*Bug Fix: 21424
     When network is down couldn't connect to server and DM returns a string "HOST NOT FOUND", if we do json parse its json type is 0
     But it is expecting JSON array data, this check point verify whether incoming data is valid or not*/
     
	if(json_object_get_type(jobj) != json_type_null)
	{


	if (json_object_get_type(jobj) == json_type_array) {
		syslog(LOG_INFO,
				"[DM]: Response queue process response received discovery start");

		if (ReqMessageDataOBJ.Command == "DISCOVERY") {

			for (size_t RespObjCount = 0;
					RespObjCount
							< json_object_array_length(
									json_tokener_parse(TronxSendRet.c_str()));
					RespObjCount++) {

				json_object* thing_id = json_object_object_get(
						json_object_array_get_idx(
								json_tokener_parse(TronxSendRet.c_str()),
								RespObjCount), "thing_id");
				json_object* status = json_object_object_get(
						json_object_array_get_idx(
								json_tokener_parse(TronxSendRet.c_str()),
								RespObjCount), "status");
				json_object* message = json_object_object_get(
						json_object_array_get_idx(
								json_tokener_parse(TronxSendRet.c_str()),
								RespObjCount), "message");

				if (strcmp(json_object_get_string(status), "success") == 0) {

					/*syslog(LOG_DEBUG,
							"[DM]: Response queue process response thing successfully added in TronX - DM: [%s]",
							json_object_get_string(thing_id));*/

					syslog(LOG_DEBUG,
							"[DM]: Response queue process response thing successfully added in TronX - DM message: [%s]",
							json_object_get_string(message));

					DmThingsBackupDb* DmThingsBackupDbOBJ =
							DmThingsBackupDb::GetInstance();

					/*syslog(LOG_DEBUG,
							"[DM]: Response queue process calling database for thing_id: [%s]",
							json_object_get_string(thing_id));*/

					DmThingsBackupDbOBJ->DeviceManagerBackUp(
							json_object_get_string(thing_id));

				} else {

					syslog(LOG_INFO,
							"[DM]: Response queue process response error message from TronX ");

//					DmThingMap* DmThingMapOBJ = DmThingMap::GetInstance();
//
//					DmThingMapOBJ->RemoveToThingMap(
//							json_object_get_string(thing_id));
//
//					syslog(LOG_DEBUG,
//							"[DM]: Response queue process response thing removed from DM: [%s]",
//							json_object_get_string(thing_id));
				}
                                //json_object_put(thing_id);
                                //json_object_put(status);
                                //json_object_put(message);
			}
			//json_object_put(jobj);
		}
		/*syslog(LOG_INFO,
				"[DM]: Response queue process response received discovery end");*/

	} else {
		/*syslog(LOG_INFO,
				"[DM]: Response queue process response received non discovery start");*/

		if (json_object_object_get(json_tokener_parse(TronxSendRet.c_str()),
				"status") != NULL) {

			syslog(LOG_DEBUG,
					"[DM]: Response queue process response received status: [%s]",
					json_object_get_string(
							json_object_object_get(
									json_tokener_parse(TronxSendRet.c_str()),
									"status")));

			if (json_object_object_get(json_tokener_parse(TronxSendRet.c_str()),
					"scenes") != NULL) {

				/*syslog(LOG_INFO,
						"[DM]: Response queue process response received scene start");*/

//				DmSceneMapping::DMCallSceneCreate(
//						json_tokener_parse(TronxSendRet.c_str()));

				/*syslog(LOG_INFO,
						"[DM]: Response queue process response received scene end");*/

			} else if (json_object_object_get(
					json_tokener_parse(TronxSendRet.c_str()), "rules") != NULL) {

				/*syslog(LOG_INFO,
						"[DM]: Response queue process response received rule start");*/

			}

			if ((ReqMessageDataOBJ.Command == "REMOVE")
					|| (ReqMessageDataOBJ.Command == "EXCLUSION")) {

				DmThingsBackupDb * DmThingsBackupDbOBJ =
						DmThingsBackupDb::GetInstance();
				DmThingsBackupDbOBJ->DeleteTronxSyncTable(
						ReqMessageDataOBJ.Thing_ID); /** Removing the thing_id from TronxSync table in database */
			}
		}

		/*syslog(LOG_INFO,
				"[DM]: Response queue process response received non discovery end");*/

	}
}else{
    syslog(LOG_ERR, "[DM]: Not able to post data to TronX, Host is not reachable");
}

}
