#include <stdio.h>
#include <zmq.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <thread>
#include <stdlib.h>
#include <thread>
#include <sys/syslog.h>
#include <json-c/json.h>
#include "fw-env.h"
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <time.h>
#include <iostream>

#include "MQTTAsync.h"
#include "mqtt-manager.hpp"

void *MqttServer;									//! MQTT ZMQ Context
void *mqtt_zmq_response_ctx;								//! ZMQ Context which binds with IP Addr
char mqtt_zmq_server_addr[MAX_IPV4_ADDR_SIZE];						//! Environmental variable to get MQTT_ZMQ Address
char x_account_token[ACCOUNT_TOKEN_MAX_SIZE];						//! Account token Env variable
std::string bt_topic_name;								//! Bluetooth Topic name
const char* supported_client_name[MAX_CLIENT_SUPPORTED] = { "BT", "FOTA", "Wi-Fi" };	//! Array of Supported client
int zmq_msg_timeout = ZMQ_MSG_TIMEOUT;
char zmq_buffer[ZMQ_BUFFER_SIZE];
bool is_client_valid;									//! returns true if client is supported else false

/*MQTT Variable Declaration*/
MQTTAsync clientLocal;									//! MQTT Async Client
MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;		//! MQTTAsync_connectOptions defines several settings that control the way the 												client connects to an MQTT server
MQTTAsync_message pubmsg = MQTTAsync_message_initializer;				//!A structure representing the payload and attributes of an MQTT message
MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
volatile MQTTAsync_token deliveredtoken; 						//! Delivery token structure

/** Function to get system IPV4 or the HUB IPv4 address */
const char* GetIPv4AddressFrom() {
	int fd;
	struct ifreq ifr;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
	memcpy(ifr.ifr_name, "br-lan", IFNAMSIZ - 1);
	ioctl(fd, SIOCGIFADDR, &ifr);
	close(fd);
	syslog(LOG_DEBUG, "[MQTT] : Mqtt Mananger Connected to IPv4 On Interface (br-lan) : [%s]",
			(char*) inet_ntoa(
					((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr));
	return ((char*) inet_ntoa(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr));
}

/** This is a callback function called by the client library after the client application has published a message to the server */
void MessageDelivered(void *context, MQTTAsync_token dt) {
	syslog( LOG_INFO, "[MQTT]: Message With Token Value Delivery Confirmed  : [%d]", dt);
	deliveredtoken = dt;
}

/** Callback invoked when MQTT publisher losts connection with broker */
void ConnectionLost(void *context, char *cause) {
	int rc;
	syslog( LOG_INFO, "[MQTT]:Connection Loss, Wait for Reconnection, Reconnecting... : [%s]", cause);
	conn_opts.automaticReconnect = MQTT_ATTEMPT_RECONNECT;
	if ((rc = MQTTAsync_reconnect(clientLocal)) != MQTTASYNC_SUCCESS) {
		syslog( LOG_ERR, "[MQTT] : Failed to Connect Broker, Return Code [%d]", rc);
	} else {
		syslog( LOG_INFO, "[MQTT] : Successfully Connected To Broker");
	}
}

/** Establishing Connection with MQTT Local Mosquitto Broker */
bool InitMqttPublisher(std::string local_client_id) {
	int rc;
	/** Creates conection with defined MQTT client handler with mentioned Client_ID, System IP, Session flag */
	MQTTAsync_create(&clientLocal, GetIPv4AddressFrom(), local_client_id.c_str(),
	MQTTCLIENT_PERSISTENCE_NONE, NULL);

	/** Configuring MQTT callbacks for async operation */
	MQTTAsync_setCallbacks(clientLocal, NULL, ConnectionLost, NULL, MessageDelivered);
	conn_opts.keepAliveInterval = MQTT_KEEP_ALIVE_INTERVAL;
	conn_opts.cleansession = MQTT_CLEAN_SESSION;
	conn_opts.automaticReconnect = MQTT_ATTEMPT_RECONNECT;
	if ((rc = MQTTAsync_connect(clientLocal, &conn_opts)) != MQTTASYNC_SUCCESS) {
		syslog( LOG_ERR, "[MQTT] : Failed to Connect Broker,with Return Code [%d]", rc);
		return false;
	} else {
		syslog( LOG_INFO, "[MQTT] : Successfully Connected To Broker");
		return true;
	}
}

/** Message publishing to requested topic based on client name */
void PublishDataToBroker(std::string client_name, const char* client_data) {

	syslog( LOG_DEBUG, "[MQTT]: ZMQ Parsed Message Sending to Publisher  : [%s]", client_data);
	if (client_name == "BT") {
		pubmsg.payload = (char*) client_data;
		pubmsg.payloadlen = strlen(client_data);
		pubmsg.qos = MQTT_QOS;
		pubmsg.retained = MQTT_RETAINED_MESSAGE;
		deliveredtoken = MQTT_DELIVERY_TOKEN;
		MQTTAsync_sendMessage(clientLocal, bt_topic_name.c_str(), &pubmsg,
				&opts);
	} else {
		syslog( LOG_ERR, "[MQTT] : Not Valid Client");
	}
}

/** Function to Bind ZMQ TCP port With Socket Options */
bool InitClientConnection() {
	int calbak;
	bool status;

	get_shifu_uci_param("MQTT_ZMQ_SERVER_ADDR", mqtt_zmq_server_addr);
	syslog(LOG_DEBUG, "[MQTT] : Reading Environment  Variable for MQTT_ZMQ_SERVER_ADDR :[%s]",
			mqtt_zmq_server_addr);
	MqttServer = zmq_ctx_new();
	mqtt_zmq_response_ctx = zmq_socket(MqttServer, ZMQ_REP);

	/** Configuring ZMQ context for timeouts while message send*/
	zmq_setsockopt(mqtt_zmq_response_ctx, ZMQ_SNDTIMEO, &zmq_msg_timeout,
			sizeof(int));
	calbak = zmq_bind(mqtt_zmq_response_ctx, mqtt_zmq_server_addr);

	if (calbak == 0) {
		syslog( LOG_INFO, "[MQTT]: Successfully Bound To PORT 7003: [%d]", calbak);
		status = true;
	} else {
		syslog( LOG_INFO, "[MQTT]: Failed To Bound to PORT 7003: [%d]", calbak);
		status = false;
	}
	return status;
}

/** Acknowledge to Client Once Request received on ZMQ port */
void ClientAckSend(std::string client_name, int status_code, int message_id,
		std::string message) {
	int ResultSend;
	size_t msgSize;
	json_object *ack = json_object_new_object();

	json_object *ack_message_id = json_object_new_int(message_id);
	json_object *ack_message = json_object_new_string(message.c_str());
	json_object *status_Code = json_object_new_int(status_code);

	json_object_object_add(ack, "message", ack_message);
	json_object_object_add(ack, "message_id", ack_message_id);
	json_object_object_add(ack, "status_code", status_Code);

	syslog(LOG_DEBUG, "[MQTT] : Sending Response to Client [%s]", json_object_get_string(ack));

	msgSize = strlen(json_object_to_json_string(ack));

	ResultSend = zmq_send(mqtt_zmq_response_ctx,
			json_object_to_json_string(ack), msgSize, 0);

	syslog(LOG_DEBUG, "[MQTT] : Response Code Received From Client :[%d]", ResultSend);
	
	/** FIX ME: Need to confirm action if ZMQ Sends Negative response */
	if (ResultSend == -1) {
		syslog(LOG_ERR, "[MQTT] : Error Response From Client, Resetting Client");
		zmq_close(mqtt_zmq_response_ctx);
		InitClientConnection();
	}

	/** Deleting JSON Object pointer */
	json_object_put(ack);
}

/** Checking for valid MQTT supported client */
bool ClientCheck(const char* client_name) {
	syslog( LOG_DEBUG, "[MQTT]: Client Name : [%s]", client_name);
	unsigned int client_name_itr;
	bool status;
	for (client_name_itr = 0;
			client_name_itr < MAX_CLIENT_SUPPORTED; client_name_itr++) {

		if (strcmp(client_name, supported_client_name[client_name_itr]) == 0) {
			status = true;
			break;
		} else {
			status = false;
		}
	}
	return status;
}

/** Parsing Messages received from various clients like BT, WiFi, DM */
void ParseClientMessage(const char* received_json_string) {
	try {
		/** Converting received sting data into JSON */
		json_object* received_json_object = json_tokener_parse(
				received_json_string);

		/** Extracting client_name, message_id, client_data JSON object from received JSON string */
		json_object* client_name = json_object_object_get(received_json_object,
				"client");
			json_object_object_get_ex(received_json_object,"client", &client_name);
		json_object* message_id = json_object_object_get(received_json_object,
				"message_id");
		json_object* client_data = json_object_object_get(received_json_object,
				"client_data");
		ClientAckSend(std::string(json_object_get_string(client_name)), 200,
				json_object_get_int(message_id),
				"REQUEST RECEIVED SUCCESSFULLY");

		is_client_valid = ClientCheck(
				json_object_get_string(client_name));
		if (is_client_valid == true) {

			PublishDataToBroker(
					std::string(json_object_get_string(client_name)),
					json_object_get_string(client_data));
		} else {

			syslog( LOG_ERR, "[MQTT] : Not Valid Client");
		}
		json_object_put(received_json_object);
	} catch (...) {
		syslog( LOG_CRIT, "[MQTT]: Exception Occurred During JSON Conversion");
	}
}

/** Establsh ZMQ Server Connection which is called in main */
bool InitZmqServer() {
	bool mqtt_zmq_connect_status = InitClientConnection();
	if (mqtt_zmq_connect_status == true) {
		return true;
	} else {
		return false;
	}
}

/** This Function is invoked in main, with separate thread for continuously listen for ZMQ Request */
void ListenForClientRequests() {
	while (true) {
		syslog( LOG_INFO, "[MQTT] : MQTT client  Listen ZMQ Request - Request ");

		/** Message Received through ZMQ */
		zmq_recv(mqtt_zmq_response_ctx, zmq_buffer,
		ZMQ_BUFFER_SIZE, 0);

		syslog( LOG_DEBUG, "[MQTT] : Received Message From Client Before Parsing [%s]", zmq_buffer);

		/** Parsing received ZMQ data from client */
		ParseClientMessage(zmq_buffer);
	}
}

int main(void) {

	syslog( LOG_INFO, "[MQTT] : Initializing MQTT Manager");

	bool is_publisher_initialized, is_zmq_server_initialized;

	/** Setting Environment Variable Initially */
	get_shifu_uci_param("ACCOUNT_TOKEN", x_account_token);
	syslog( LOG_DEBUG, "[MQTT] : MQTT Manager Account_token :[%s]",
			x_account_token);

	/** Based on Account token Created Topic_name and client_id */
	bt_topic_name = "BT/" + std::string(x_account_token) + "/ack";
	std::string local_client_id = std::string(x_account_token) + "/#" + std::to_string(rand() % 900);
	syslog( LOG_DEBUG, 
			 "[MQTT] : MQTT Client  Home_token :[%s]",
			x_account_token);

	is_publisher_initialized = InitMqttPublisher(local_client_id);
	if (is_publisher_initialized == true) {
		syslog( LOG_INFO, "[MQTT] : MQTT Manager Successfully Connected To Local Broker");

		is_zmq_server_initialized = InitZmqServer();
		if (is_zmq_server_initialized == true) {
			syslog( LOG_INFO, "[MQTT] : MQTT Manager Successfully bound to port");

			std::thread ListenForClientRequestsThread(ListenForClientRequests);
			ListenForClientRequestsThread.join();
		} else {
			syslog( LOG_CRIT, "[MQTT]: MQTT Manager Failed to bound to port");
		}

	} else {
		syslog( LOG_CRIT, "[MQTT]: MQTT Manager Failed to Connect to Local Broker ");
	}
	return (0);
}

