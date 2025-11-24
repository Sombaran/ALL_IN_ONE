/*
 * File_name:dm_secne_mapping.hpp
 * @Brief:Driver class to enable scene and then inherit into database mapping
 * Created on: 23-Aug-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef DM_SECNE_MAPPING_HPP_
#define DM_SECNE_MAPPING_HPP_

#include <iostream>
#include <sqlite3.h>
#include <json-c/json.h>
#include <map>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sys/syslog.h>
#include <string.h>
#include <unistd.h>

class Scenes {

private:

	std::string scene_id;
	std::string scene_name;
	std::string scene_phrase;
	std::string scene_mute;
	json_object *iot_action;
	json_object *non_iot_action;
	bool SceneExecuteflag;

public:

	void DMSetSeceneId(std::string);
	void DMSetSeceneName(std::string);
	void DMSetSecenePhrase(std::string);
	void DMSetSeceneMute(std::string);
	void DMSetSeceneIOTAction(json_object*);
	void DMSetSeceneNonIOTAction(json_object*);
	bool DMGetSceneExecuteflag();
	void DMSetSceneExecuteflag(bool);
	std::string DMGetSeceneId();
	std::string DMGetSeceneName();
	std::string DMGetSecenePhrase();
	std::string DMGetSeceneMute();
	json_object* DMGetSeceneIOTAction();
	json_object* DMGetSeceneNonIOTAction();
	bool DMSceneModify(std::string, std::string, std::string, std::string,
			json_object*, json_object*);
	SecneRuleExecution DMSceneExecute(std::string);
	void SceneDisplay();
	virtual ~Scenes() {}
};

class DmSceneMapping: public Scenes {

public:

	static bool DMGetSceneMapExecuteflag();
	static void DMSetSceneMapExecuteflag(bool);
	static bool DMSceneDatabaseInit();
	static bool DMSceneDatabaseOpen();
	static bool DMSceneDatabaseClose();
	static bool DMCreateSceneTable();
	static void DMScenePrintMap();
	static void DMScenePrintDB();
	static void DeleteThingIDFromScenes(std::string);
	static std::string CompareThingIdFromSceneMap_AndDelete(json_object*,
			std::string);
	static bool DMSceneCreate(std::string, std::string, std::string,
			std::string, std::string, std::string);
	static bool DMRecoverMapFromSceneDB();
	static bool DMSceneModifyDB(std::string, std::string, std::string,
			std::string, std::string, std::string);
	static bool DMSceneIdCheck(std::string);
	static bool DMCallSceneModify(json_object*);
	static bool DMCallSceneCreate(json_object*);
	static bool DMGetSceneExecuteStatus(std::string);
	static bool DMSetSceneExecuteStatus(std::string);
	static SecneRuleExecution DMCallSceneExecute(json_object*);
	~DmSceneMapping() {}
};

#endif /* DM_SECNE_MAPPING_HPP_ */
