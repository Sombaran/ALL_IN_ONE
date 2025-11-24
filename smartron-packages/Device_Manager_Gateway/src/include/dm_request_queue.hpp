/*
 * File_name:dm_request_queue.hpp
 * @Brief: Driver class to process the parsed request to sort into vector queue
 * Created on: 26-Jul-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef INCLUDE_DM_REQUEST_QUEUE_HPP_
#define INCLUDE_DM_REQUEST_QUEUE_HPP_

#include <iostream>
#include <vector>
#include <map>
#include <pthread.h>
#include <sys/syslog.h>
#include <json-c/json.h>
#include <bits/stdc++.h>
#include "dm_platform.hpp"

class DmRequestQueue {

private:
	static DmRequestQueue *Instance;

public:
	DmRequestQueue() {}
	static DmRequestQueue *GetInstance();
	int RspQueueSizeflag = 0;
	pthread_mutex_t ReqAddQueueRcvMutx = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t ReqMutex3Cond = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t ReqAddRcvCond = PTHREAD_COND_INITIALIZER;
	bool ManyRequest = true;
	std::vector<RequestResponse> DMReqQueueDataTronx;
	std::vector<MessageIndex> DMReqQueueIndexTronx;
	bool DMReqQueueInit();
	void DMReqQueueheapify(std::vector<RequestResponse> &DMReqQueueDataTronx, int, int);
	void DMReqQueueheapsort(std::vector<RequestResponse> &DMReqQueueTronx,int);
	void DMReqQueuebuild_heap(std::vector<RequestResponse> &DMReqQueueTronx, int);
	void DMReqQueuefilter(std::vector<RequestResponse> &DMReqQueueTronx,
			std::vector<MessageIndex> &DMReqInfoTronx, std::string ,
			bool &, int &);
	void DMReqQueueAddMessage(RequestResponse);		/* Function for adding request objects into process queue vector */
	void DMReqQueueRemoveMessage();
	RequestResponse DMReqQueueReceiveMessage();
	 ~DmRequestQueue() {
	 DMReqQueueDataTronx.shrink_to_fit();
	 DMReqQueueIndexTronx.shrink_to_fit();
	 }
};

#endif /* INCLUDE_DM_REQUEST_QUEUE_HPP_ */
