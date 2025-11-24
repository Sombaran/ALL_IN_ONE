#include "dm_zeromq.hpp"
#include "dm_platform.hpp"
#include "dm_thingmap.hpp"
#include "dm_request_queue.hpp"
#include "dm_req_res_mapping.hpp"
#include "dm_request_parse.hpp"

int ZMQ_TIMEOUT = 1000;
//char errnum;


DmZeroMq * DmZeroMq::Instance = 0;

/*DmZeroMq::DmZeroMq() {
}
DmZeroMq::~DmZeroMq() {
}*/

DmZeroMq * DmZeroMq::GetInstance() {
	if (Instance == 0) {
		Instance = new DmZeroMq();
	}
	return Instance;
}

bool DmZeroMq::zeroMqDmServer() {
	int ReturnValueServerDM = get_shifu_uci_param((char *)"DM_ZMQ_SERVER", DM_ZMQ_SERVER);
	if (ReturnValueServerDM == 0) {
		syslog(LOG_DEBUG,
				"[DM]: Dm_zeromq Reading Environment Variable for(DM_ZMQ_SERVER) :[%s] and return value is [%d]",
				DM_ZMQ_SERVER, ReturnValueServerDM);
	} else {
//		/*strcpy(DM_ZMQ_SERVER, "tcp://*:7001");check this */
		syslog(LOG_DEBUG,
				"[DM]: Dm_zeromq Reading Environment Variable for(DM_ZMQ_SERVER) :[%s] and return value is [%d]",
				DM_ZMQ_SERVER, ReturnValueServerDM);
	}
//	strcpy(DM_ZMQ_SERVER, "tcp://*:7001");
	syslog(LOG_DEBUG, "[DM]: ZeroMq server starting with: [%s]", DM_ZMQ_SERVER);
	ZmqCtxSCResp = zmq_ctx_new();
	ClientResponse = zmq_socket(ZmqCtxSCResp, ZMQ_REP);
	int calbak = zmq_bind(ClientResponse, DM_ZMQ_SERVER);
	if (calbak == 0) {
		syslog(LOG_DEBUG, "[DM]: Dm_zeromq server started successfully");
		return true;
	} else {
		syslog(LOG_ERR, "[DM]: Dm_zeromq server failed with response error number: [%d] and error statement: [%s]",
				errno, strerror(errno));
		return false;
	}
	return false;
}

bool DmZeroMq::DMZeroMQInit() {

	if (zeroMqDmServer() == true) {
		SocketServerResponse = (void *) ZeroMQDMClientsInit((char*)"DM_SS_ZMQ_CLIENT",
				DM_SS_ZMQ_CLIENT);
		CronHandlerResponse = (void *) ZeroMQDMClientsInit((char*)"DM_VA_ZMQ_CLIENT",
				 DM_VA_ZMQ_CLIENT);
		MobileAgentResponse = (void *) ZeroMQDMClientsInit((char*)"DM_MA_ZMQ_CLIENT",
				DM_MA_ZMQ_CLIENT);
//		FotaResponse = (void *) ZeroMQDMClientsInit("DM_FA_ZMQ_CLIENT",
//				DM_FA_ZMQ_CLIENT);
//		MqttResponse = (void *) ZeroMQDMClientsInit("DM_MQTT_ZMQ_CLIENT",
//		 DM_MQTT_ZMQ_CLIENT);
		if ((SocketServerResponse != 0) && (CronHandlerResponse!= 0)
				&& (MobileAgentResponse != 0) /*&& (MqttResponse != 0)*/) {
			syslog(LOG_DEBUG, "[DM]: Dm_zeromq socket client initialized successfully ");
			return true;
		} else {
			syslog(LOG_ERR, "Dm_zeromq socket client initializing failed ");
			return false;
		}
	} else {
		syslog(LOG_ERR, "[DM]: Dm_zeromq server listening failed");
		return false;
	}
}

void DmZeroMq::Get_NVM_Variables(char *NVM_variable, char *ClientName) {

	char localclient[100];
	int retval = get_shifu_uci_param(NVM_variable, localclient);
	if (retval == 0) {
		strcpy(ClientName, localclient); //check this
		syslog(LOG_DEBUG,
				"[DM]: Dm_zeromq Reading Environment Variable [%s] exist and string is [%s] ",
				NVM_variable, ClientName);
	} else {
		/*strcpy(ClientName, "tcp://localhost:7006"); //check this
		 syslog(LOG_DEBUG,
		 "[DM]: Zeromq Reading Environment Variable for(DM_SS_ZMQ_CLIENT) :[%s]",
		 ClientName);
		 */
		syslog(LOG_DEBUG,
				"[DM]: Dm_zeromq Environment Variable [%s] does not exist, return value :[%d]",
				NVM_variable, retval);
	}
}

void * DmZeroMq::ZeroMQDMClientsInit(char *NVM_Name, char *Global_Buffer) {

	DmZeroMq::Get_NVM_Variables(NVM_Name, Global_Buffer);
	ZmqCtxResp = zmq_ctx_new();
	zmq_socket_Response = zmq_socket(ZmqCtxResp, ZMQ_REQ);
	zmq_setsockopt(zmq_socket_Response, ZMQ_SNDTIMEO, &ZMQ_TIMEOUT,
			sizeof(int));
	int retval = zmq_connect(zmq_socket_Response, Global_Buffer);
	if (retval == 0) {
		syslog(LOG_DEBUG, "[DM]: Dm_zeromq  client started successfully ");
		return zmq_socket_Response;
	} else {
		syslog( LOG_ERR,
				"[DM]: Dm_zeromq sending resonse for generic client error number: [%d] and error statement: [%s]",
				errno, strerror(errno));
		return NULL;
	}

}

void DmZeroMq::ZeroMessageQueueRcvRequest() {

//	 Clearing the buffer before receiving requests from clients
//	memset(&DM_ZMQ_MSG_RCV, 0, sizeof(DM_ZMQ_MSG_RCV));
//	zmq_recv(ClientResponse, DM_ZMQ_MSG_RCV, DM_ZMQ_REQ_SIZE, 0);
//	syslog(LOG_DEBUG,
//			"[DM]: ZeroMq server receive message legth: [%ld] message: [%s]",
//			strlen(DM_ZMQ_MSG_RCV), DM_ZMQ_MSG_RCV);
//	ZeroMessageRequestParse(DM_ZMQ_MSG_RCV);
			/**dynamic buffer feature */
			int64_t more;
			size_t more_size = sizeof more;
			while (1) {
			char *Zstring = NULL;
			syslog(LOG_INFO, "[DM]: Dm_zeromq server /'RECEIVING/' thread started");
			/**Zero Message Queue buffer making dynamic */
			do {
			zmq_msg_t part;
			zmq_msg_init(&part);
			/**Create an empty ØMQ message to hold the message part
			 Block until a message is available to be received from socket*/
			zmq_msg_recv(&part, ClientResponse, 0);
			/** Determine if more message parts are to follow */
			zmq_getsockopt(ClientResponse, ZMQ_RCVMORE, &more, &more_size);
			if (zmq_msg_more(&part)) {
			int size = zmq_msg_size(&part);
			Zstring = (char *) malloc((size + 1) * sizeof(char));
			memcpy(Zstring, zmq_msg_data(&part), size);
			syslog( LOG_DEBUG, "[DM]: Dm_zeromq receiving request call [%s] and request length [%d]", Zstring, size );
			} else {
			syslog( LOG_DEBUG, "[DM]: Dm_zeromq receiving request end");
			break;
			}
			//zmq_msg_close(&part);
			} while (more);
			ZeroMessageRequestParse(Zstring);
			free(Zstring);
			}
}
