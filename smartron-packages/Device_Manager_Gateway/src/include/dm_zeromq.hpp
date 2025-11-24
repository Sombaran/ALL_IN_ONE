/*
 * File_name:dm_zeromq.hpp
 * @Brief: Driver class to receive data from SS,MA,CJ and to initialize client and server for ZMQ
 * Created on: 31-Jul-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef SRC_DM_ZEROMQ_HPP_
#define SRC_DM_ZEROMQ_HPP_

#include <iostream>
#include <zmq.h>
#include <thread>
#include <sys/syslog.h>
#include <string.h>
#include "fw-env.h"
#include <json-c/json.h>
#include <errno.h>

#define BUF_MAX_128 			128
//#define BUF_MAX_1024 			1024

//#define DM_ZMQ_REQ_SIZE			5000

class DmZeroMq {

private:

	static DmZeroMq *Instance;

public:
	DmZeroMq() {}
	//char *DM_ZMQ_MSG_RCV;
	char DM_ZMQ_RSP_BUFFER[BUF_MAX_128] = "";
	char DM_ZMQ_SERVER[BUF_MAX_128] = "";
	char DM_SS_ZMQ_CLIENT[BUF_MAX_128] = "";
	char DM_MA_ZMQ_CLIENT[BUF_MAX_128] = "";
	char DM_VA_ZMQ_CLIENT[BUF_MAX_128] = "";
//	char DM_FA_ZMQ_CLIENT[BUF_MAX_128];
//	char DM_MQTT_ZMQ_CLIENT[BUF_MAX_128];
	void *ClientResponse;
	void *ZmqCtxSCResp;
	void *zmq_socket_Response; //void *SocketServerResponse;
	void *ZmqCtxResp; //void *ZmqCtxSSResp
	void *ZmqCtxSSResp;
	void *SocketServerResponse;
	void *MobileAgentResponse;
	void *CronHandlerResponse;
//	void *FotaResponse;
//	void *MqttResponse;
//	void *ZmqCtxMQTTResp;
	bool zeroMqDmServer();
	bool zeroMqDmClientSS();
	bool zeroMqDmClientVA();
	bool zeroMqDmClientMA();
//	bool zeroMqDmClientMQTT();
	void Get_NVM_Variables(char *,char *);
	void *ZeroMQDMClientsInit(char *,char *);
	void *ZeroMQDMServerInit(char *,char *);
	void ZeroMessageQueueRcvRequest();
	bool DMZeroMQInit();
	static DmZeroMq *GetInstance();
	 ~DmZeroMq() {}
};

#endif /* SRC_DM_ZEROMQ_HPP_ */
