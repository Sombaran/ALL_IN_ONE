/*
 * File_name:dm_response_queue.hpp
 * @Brief: Driver class to receive responses from Zigbee/ Zwave layers and vector queue,
 * and then furthur process the response into another vector queue to before sending it to TronX
 * Created on: 26-Jul-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef INCLUDE_DM_RESPONSE_QUEUE_HPP_
#define INCLUDE_DM_RESPONSE_QUEUE_HPP_

#include <iostream>
#include <pthread.h>
#include <map>
#include <vector>
#include <sys/syslog.h>
#include <bits/stdc++.h>
#include "dm_platform.hpp"

class DmResponseQueue {
private:
	static DmResponseQueue *Instance;
public:
	DmResponseQueue() {}
	static DmResponseQueue *GetInstance();
	int RspQueueSizeflag = 0;
	pthread_mutex_t RspMutex3Cond = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t RspAddRcvMutx = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t RspAddRcvCond = PTHREAD_COND_INITIALIZER;
	bool ManyResponse = false;
	std::vector<RequestResponse> DMRspQueueDataTronx;
	std::vector<MessageIndex> DMRspQueueIndexTronx;
	bool DMResQueueInit();
	void DMResQueueheapify(std::vector<RequestResponse> &DMRspQueueDataTronx,int, int);
	void DMResQueueheapsort(std::vector<RequestResponse> &DMRspQueueDataTronx, int);
	void DMResQueuebuild_heap(std::vector<RequestResponse> &, int);
	void DMResQueuefilter(std::vector<RequestResponse> &,
			std::vector<MessageIndex> &, std::string, bool &,
			int &);
	void DMResQueueAddMessage(RequestResponse, std::string, std::string,
			std::string, int);
	void DMResQueueRemoveMessage();
	void DMResQueueReceiveMessage();
	 ~DmResponseQueue() {
	 DMRspQueueDataTronx.shrink_to_fit();
	 DMRspQueueIndexTronx.shrink_to_fit();
	 }
};

#endif /* INCLUDE_DM_RESPONSE_QUEUE_HPP_ */
