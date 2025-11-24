/*
 * File_name:dm_send_data_tronx.hpp
 * @Brief: Driver class which receives the data from response queue vector,
 * to send the data to TronX by adding and processing into another vector queue
 * Created on: 01-Aug-2018
 * Author: JOTIRLING SWAMI
 *Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef INCLUDE_DM_SEND_DATA_TRONX_HPP_
#define INCLUDE_DM_SEND_DATA_TRONX_HPP_

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <json-c/json.h>
#include <unistd.h>
#include <sys/syslog.h>
#include <thread>
#include <vector>
#include "fw-env.h"
#include <sys/utsname.h>
#include "dm_utility.hpp"

#define 	MAX_BUFSIZE_6144  		6144
#define 	MAX_BUFSIZE_256 		256
#define 	MAX_BUFSIZE_128 		128
#define 	MAX_BUFSIZE_32 		32
#define 	MAX_BUFSIZE_8 		8

struct MemoryStruct {
	char *memory;
	size_t size;
};

class DmSendDataTronx {

private:

	static DmSendDataTronx *Instance;

public:

	DmSendDataTronx() {}
	char X_REQUEST_ID[MAX_BUFSIZE_128];
	char X_HOME_TOKEN[MAX_BUFSIZE_128];
	char X_HOME_TOKEN_VALUE[MAX_BUFSIZE_128];
	char X_DEVICE_ID[MAX_BUFSIZE_32];
	char X_DEVICE_ID_VALUE[MAX_BUFSIZE_32];
	char X_BASE_URL_VALUE[256];
	char CONTENT_TYPE[MAX_BUFSIZE_32];
	char CONTENT_TYPE_VALUE[MAX_BUFSIZE_32] = "chunked";
	char CLIENT_NAME_VALUE[MAX_BUFSIZE_8] = "DM";
	char C_TIME_VALUE[MAX_BUFSIZE_32];
	char URL_NAME[MAX_BUFSIZE_256];
	CURL *curl;
	CURLcode res;

	static DmSendDataTronx *GetInstance();

//------------------------------------------------------------------------------------

	std::string updateTronxDatabase(ReqMessageData);
	static size_t ReadDataCallback(void *, size_t , size_t ,void *);
	static size_t WriteMemoryCallback(void *, size_t , size_t ,void *);
	bool CurlInit();
	bool DmSendDataTronxInit();
	void DmSendheapify(std::vector<ReqMessageData> &DMSendDataTronx, int,int);
	void DmSendheapsort(std::vector<ReqMessageData> &DMSendDataTronx, int);
	void DmSendbuild_heap(std::vector<ReqMessageData> &DMSendDataTronx, int);
	void DmSendfilter(std::vector<ReqMessageData> &DMSendDataTronx,
			std::vector<MessageIndex> &DMSendsIndexTronx, std::string,
			bool &, int &);
	void DmSendAddMessage(ReqMessageData);
	void DmSendRemoveMessage();
	void DmSendReceiveMessage();
	int RspQueueSizeflag = 0;
	bool ManyRequest = false;
	std::vector<ReqMessageData> DMSendDataTronx;
	std::vector<MessageIndex> DMSendsIndexTronx;
//------------------------------------------------------------------------------------
	 ~DmSendDataTronx() {
	 DMSendDataTronx.shrink_to_fit();
	 DMSendsIndexTronx.shrink_to_fit();
	 }
};

#endif /* INCLUDE_DM_SEND_DATA_TRONX_HPP_ */
