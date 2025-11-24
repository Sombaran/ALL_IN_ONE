/*
 * File_name: dm_request_parse.cpp
 * @Brief: This recieves the data from MA,WS, CJ and parses it,
 * sends the data to II nd Thread to process the data received
 * Created on: 22-Aug-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */


#ifndef DM_REQUEST_PARSE_HPP_
#define DM_REQUEST_PARSE_HPP_

#include <iostream>
#include <zmq.h>
#include <thread>
#include <sys/syslog.h>
#include <string.h>
#include "fw-env.h"
#include <json-c/json.h>
#include "dm_zeromq.hpp"


void AddDMSendResponse(json_object*, json_object*, json_object*, std::string,
		json_object*, int, json_object*);
void ZeroMessageRequestParse(const char *);
void DMSceneRuleParseVA(int, std::string, std::string, std::string,
		json_object*);
void requestfailurefromclient(json_object*, json_object*, json_object*,
		json_object*, const char *,int);
bool JsonDataCheck(json_object*);
void Thing_Check_AddingQueue(json_object*, int, int);
void MakeDMReqQueueAddMessage(json_object*, json_object*, json_object*,
		json_object*, json_object*, json_object*);
/*Function Overloading for FOTA */
void MakeDMReqQueueAddMessage(json_object*, json_object*, json_object*,
		json_object*, json_object*, json_object*, json_object*, json_object*);
#endif /* DM_REQUEST_PARSE_HPP_ */
