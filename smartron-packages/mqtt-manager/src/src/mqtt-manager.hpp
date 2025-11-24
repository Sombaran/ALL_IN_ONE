/*
 * mqtt-manager.hpp
 *
 *  Created on: 18-Sep-2018
 *      Author: som
 */

#ifndef MQTT_MANAGER_HPP_
#define MQTT_MANAGER_HPP_

#define MQTT_QOS        		  	1		//! MQTT QOS level 0 -Fire and forget - the message may not be delivered
								//! 1 -At least once - the message will be delivered, but may be delivered more than once  	  	  	     							//! 2 -Once and one only - the message will be delivered exactly once.
#define ZMQ_MSG_TIMEOUT    		  	1000		//! Timeout for each ZMQ send request to be avaible in Queue in milli-seconds
#define ZMQ_BUFFER_SIZE 			5000		//! Maximum Buffer size which is to be allocated for ZMQ receive request FIX ME: Define max size
#define MAX_CLIENT_SUPPORTED			3		//! Maximum Number of Clients Supported which will asynchronously communicating through MQTT
#define ACCOUNT_TOKEN_MAX_SIZE			128		//! Maximum Allocated Size of Account token to b efetched from Env Variable
#define MQTT_KEEP_ALIVE_INTERVAL		60		//! The "keep alive" interval, measured in seconds, defines the maximum time that should pass without communication between the client and the server
#define MQTT_CLEAN_SESSION                      1		//! controls the behaviour of both the client and the server at connection and disconnection time
#define MQTT_ATTEMPT_RECONNECT 			4		//! Maximum reconnection tries to be performed by MQTT on disconnection with broker   
#define MQTT_RETAINED_MESSAGE                   0		//! The retained flag serves two purposes depending on whether the message it is associated with is being published or received
#define MQTT_DELIVERY_TOKEN			0 		//! Initialisation of delivry token
#define MAX_IPV4_ADDR_SIZE			16		//! Maximum IPV4 Address size


/**
 * A function to get system IPV4 or the HUB IPv4 address
 * @return IPv4 address of the HUB
 */
const char* GetIPv4AddressFrom();

/**
 * A callback function called by the client library after the client application has published a message to the server
 * @param context The the context pointer is passed to each of the callback functions to provide access to the context information in the callback
 * @param dt  delivery tokens returned from calls
 */
void MessageDelivered(void *context, MQTTAsync_token dt);

/**
 * Callback invoked when MQTT publisher losts connection with broker.
 * This function to enable asynchronous notification of the loss of connection to the server.
 * This function will retry 4 times for re-establishing connection
 * @param context The the context pointer is passed to each of the callback functions to provide access to the context information in the callback
 * @param cause The reason for the disconnection. Currently, cause is always set to NULL
 */
void ConnectionLost(void *context, char *cause);

/**
 * Function to connect local broker with specified client id
 * @param local_client_id client id to be registered with broker
 * @return on success true, on failure false
 */
bool InitMqttPublisher(std::string local_client_id);

/**
 * Handler to publish client data based on Clients requested
 * @param client_name Client name who wants to publish data
 * @param client_data data which clients want to publish
 */
void PublishDataToBroker(std::string client_name, const char* client_data);

/**
 * Function to initialize zmq server 
 * @return true/ false on success / failure
 */
bool InitClientConnection();

/**
 * Handler to Send Acknowledgment to requested client
 * @param client_name Client name who wants to publish data
 * @param status_code code to indicate failure or success
 * @param message_id Client requested message ID
 * @param message Message needs to send
 */
void ClientAckSend(std::string client_name, int status_code, int message_id,
		std::string message);

/**
 * Function to validate supported client
 * @param client_name client name from requested json object.
 * @return if supports true, else false
 */
bool ClientCheck(const char* client_name);

/**
 * Function to parse the payload received from various clients like BT, WiFi, DM
 * @param received_json_string is the payload received
 */
void ParseClientMessage(const char* received_json_string);

/**
 * Function to initialize zmq server connection port
 * @return true/ false on success / failure
 */
bool InitZmqServer();

/**
 * Function to Continuously listen client requests such as BT, WiFi, DM
 */
void ListenForClientRequests();

#endif /* MQTT_MANAGER_HPP_ */
