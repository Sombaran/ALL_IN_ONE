#include "dm_utility.hpp"
#include "dm_secne_mapping.hpp"

#define ZERO_LENGTH 0
std::map<std::string, Scenes*> DMSceneMapData;
static bool sceneExecutionStatus;

sqlite3* DMSceneDB;

//----------------------------------------------------------------------------------------------------------------

void Scenes::DMSetSeceneId(std::string scene_id) {
	this->scene_id = scene_id;
}
void Scenes::DMSetSeceneName(std::string scene_name) {
	this->scene_name = scene_name;
}
void Scenes::DMSetSecenePhrase(std::string scene_phrase) {
	this->scene_phrase = scene_phrase;
}
void Scenes::DMSetSeceneMute(std::string scene_mute) {
	this->scene_mute = scene_mute;
}
void Scenes::DMSetSeceneIOTAction(json_object* iot_action) {
	this->iot_action = iot_action;
}
void Scenes::DMSetSeceneNonIOTAction(json_object* non_iot_action) {
	this->non_iot_action = non_iot_action;
}

std::string Scenes::DMGetSeceneId() {
	return this->scene_id;
}
std::string Scenes::DMGetSeceneName() {
	return this->scene_name;
}
std::string Scenes::DMGetSecenePhrase() {
	return this->scene_phrase;
}
std::string Scenes::DMGetSeceneMute() {
	return this->scene_mute;
}
json_object* Scenes::DMGetSeceneIOTAction() {
	return this->iot_action;
}
json_object* Scenes::DMGetSeceneNonIOTAction() {
	return this->non_iot_action;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {

	for (int i = 0; i < argc; i++) {

		syslog(LOG_DEBUG,
				"[DM]: Scene mapping printing database result scene [%s]: scene value [%s] ",
				azColName[i], argv[i]);
	}
	return 0;
}

SecneRuleExecution Scenes::DMSceneExecute(std::string scene_id) {

	SecneRuleExecution SceneExecuteData;

	bool iot = false;
//	bool non_iot = false;

	/*syslog(LOG_DEBUG, "[DM]: Scene execute start for scene_id : [%s]",
			scene_id.c_str());*/

	std::map<std::string, Scenes*>::iterator DMSceneMapDataItr =
			DMSceneMapData.find(scene_id);
	if (DMSceneMapDataItr != DMSceneMapData.end()) {

		/*syslog(LOG_DEBUG, "[DM]: Scene execute start for iot : [%s]",
				json_object_get_string(
						DMSceneMapDataItr->second->DMGetSeceneIOTAction()));*/

		/*syslog(LOG_DEBUG, "[DM]: Scene execute start for non-iot : [%s]",
				json_object_get_string(
						DMSceneMapDataItr->second->DMGetSeceneNonIOTAction()));*/

//--------------------------------------------------------------------------------------
		json_type jsontypeofATRCount1 = json_object_get_type(
				DMSceneMapDataItr->second->DMGetSeceneIOTAction());

		if (jsontypeofATRCount1 == json_type_array) {
			/*syslog(LOG_DEBUG, "[DM]: Scene execute start for iot count: [%d]",
					json_object_array_length(
							DMSceneMapDataItr->second->DMGetSeceneIOTAction()));*/

			if (json_object_array_length(
					DMSceneMapDataItr->second->DMGetSeceneIOTAction()) >= 1) {
//ACTION IOT DATA
				iot = true;
				/*syslog(LOG_INFO,
						"[DM]: Scene execute start for scene_id iot success");*/
			} else {
//NON ACTION IOT DATA
				iot = false;
				syslog(LOG_INFO,
						"[DM]: Scene execute start for scene_id iot fail");
			}
		} else {
			syslog(LOG_INFO, "[DM]: Scene execute start for scene_id iot null");
		}
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//		json_type jsontypeofATRCount = json_object_get_type(
//				DMSceneMapDataItr->second->DMGetSeceneNonIOTAction());
//
//		if (jsontypeofATRCount == json_type_array) {
//
//			/*syslog(LOG_DEBUG,
//					"[DM]: Scene execute start for non-iot count: [%ld]",
//					json_object_array_length(
//							DMSceneMapDataItr->second->DMGetSeceneNonIOTAction()));
//
//			syslog(LOG_DEBUG,
//					"[DM]: Scene execute start for non-iot data all : [%s]",
//					json_object_get_string(
//							DMSceneMapDataItr->second->DMGetSeceneNonIOTAction()));*/
//
//			if (json_object_array_length(
//					DMSceneMapDataItr->second->DMGetSeceneNonIOTAction())
//					>= 1) {
//
//				/*syslog(LOG_INFO,
//						"[DM]: Scene execute start for scene_id non iot array not null");*/
//
//				//Non iot actions of scenes handling
//				for (size_t arrayLength = 0;
//						arrayLength
//								< json_object_array_length(
//										DMSceneMapDataItr->second->DMGetSeceneNonIOTAction());
//						arrayLength++) {
//
//					json_object* non_iot_json =
//							json_object_array_get_idx(
//									DMSceneMapDataItr->second->DMGetSeceneNonIOTAction(),
//									arrayLength);
//
//					/*syslog(LOG_DEBUG,
//							"[DM]: Scene execute start for non-iot data : [%s]",
//							json_object_get_string(non_iot_json));*/
//
//					json_object* action_type = json_object_object_get(
//							non_iot_json, "action_type");
//
//					/*syslog(LOG_DEBUG,
//							"[DM]: Scene execute start for non-iot action_type : [%s]",
//							json_object_get_string(action_type));*/
//
//					//Scenes Music action handling
//					if (strcasecmp(json_object_get_string(action_type), "music")
//							== 0
//							|| strcasecmp(json_object_get_string(action_type),
//									"weather") == 0
//							|| strcasecmp(json_object_get_string(action_type),
//									"news") == 0) {
//
//						/*syslog(LOG_INFO,
//								"[DM]: Scene execute start for non iot action music or weather or news");*/
//
//						json_object* urls = json_object_object_get(non_iot_json,
//								"urls");
//
//						/*syslog(LOG_DEBUG,
//								"[DM]: Scene execute start for non-iot urls : [%s]",
//								json_object_get_string(urls));*/
//
//						json_type urlstype = json_object_get_type(urls);
//						if (urlstype == json_type_array) {
//							if (json_object_array_length(urls) >= 1) {
//								non_iot = true;
//								syslog(LOG_INFO,
//										"[DM]: Scene execute start for scene: non iot urls present");
//								break;
//
//							} else {
//								non_iot = false;
//								syslog(LOG_INFO,
//										"[DM]: Scene execute start for scene: non iot urls not present");
//								//break;
//							}
//						}
//
//					} else {
//						syslog(LOG_INFO,
//								"[DM]: Scene execute start for scene_id non iot action is not there");
//					}
//				}
//			} else {
//				syslog(LOG_INFO,
//						"[DM]: Scene execute start for scene_id non iot array null");
//			}
//		} else {
//			syslog(LOG_INFO,
//					"[DM]: Scene execute start for scene_id non-iot null");
//		}
//--------------------------------------------------------------------------------------

		/*syslog(LOG_DEBUG,
				"[DM]: Scene execute start for iot state : [%d] non_iot state : [%d]",
				iot, non_iot);*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//		if (iot == true) {
//			if (non_iot == true) {
////BoTH
//				/*syslog(LOG_DEBUG,
//						"[DM]: Scene execute start for scene_id BOTH : [%s]",
//						scene_id.c_str());*/
//				SceneExecuteData.Sceneflag = 3;
//
//			} else {
////IOT
//				/*syslog(LOG_DEBUG,
//						"[DM]: Scene execute start for scene_id IOT : [%s]",
//						scene_id.c_str());*/
//				SceneExecuteData.Sceneflag = 2;
//			}
//
//		} else {
////NON IOT
//			/*syslog(LOG_DEBUG,
//					"[DM]: Scene execute start for scene_id NON IOT : [%s]",
//					scene_id.c_str());*/
//			if (non_iot == true) {
//				SceneExecuteData.Sceneflag = 1;
//			}
//		}

       if (iot == true) {
			SceneExecuteData.Sceneflag = 2;
			syslog(LOG_INFO, "[DM]: Scene iot flag set");
		}
	
		SceneExecuteData.iot_action =
				DMSceneMapDataItr->second->DMGetSeceneIOTAction();

		SceneExecuteData.non_iot_action =
				DMSceneMapDataItr->second->DMGetSeceneNonIOTAction();
		SceneExecuteData.RuleSceneID =
				DMSceneMapDataItr->second->DMGetSeceneId();

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	}

	/*syslog(LOG_INFO, "[DM]: Scene execute end");*/

	return SceneExecuteData;
}

bool Scenes::DMSceneModify(std::string scene_id, std::string scene_name,
		std::string scene_phrase, std::string scene_mute,
		json_object* iot_action, json_object* non_iot_action) {

	/*syslog(LOG_INFO, "[DM]: Scene modify start");*/

	this->DMSetSeceneId(scene_id);
	this->DMSetSeceneName(scene_name);
	this->DMSetSecenePhrase(scene_phrase);
	this->DMSetSeceneMute(scene_mute);
	this->DMSetSeceneIOTAction(iot_action);
	this->DMSetSeceneNonIOTAction(non_iot_action);

	/*syslog(LOG_INFO, "[DM]: Scene modify end");*/

	return true;
}

bool Scenes::DMGetSceneExecuteflag() {

	/*syslog(LOG_DEBUG, "[DM]: SCENE executing VALUE : [%d]",
			this->SceneExecuteflag);*/

	return this->SceneExecuteflag;
}
void Scenes::DMSetSceneExecuteflag(bool RuleExecuteflag) {

	/*syslog(LOG_DEBUG, "[DM]: Scene executing FLAG VALUE : [%d]",
			RuleExecuteflag);*/

	this->SceneExecuteflag = RuleExecuteflag;
}

void Scenes::SceneDisplay() {

	syslog(LOG_INFO, "[DM]: Scene display start");

	syslog(LOG_DEBUG, "[DM]: Scene display scene_id: [%s]",
			this->scene_id.c_str());
	syslog(LOG_DEBUG, "[DM]: Scene display scene_name: [%s]",
			this->scene_name.c_str());
	syslog(LOG_DEBUG, "[DM]: Scene display scene_phrase: [%s]",
			this->scene_phrase.c_str());
	syslog(LOG_DEBUG, "[DM]: Scene display scene_mute: [%s]",
			this->scene_mute.c_str());
	syslog(LOG_DEBUG, "[DM]: Scene display scene_iot_action: [%s]",
			json_object_get_string(this->iot_action));
	syslog(LOG_DEBUG, "[DM]: Scene display scene_non_iot_action: [%s]",
			json_object_get_string(this->non_iot_action));

	syslog(LOG_INFO, "[DM]: Scene display end");
}

//----------------------------------------------------------------------------------------------------------------

bool DmSceneMapping::DMGetSceneMapExecuteflag() {
	return sceneExecutionStatus;
}

void DmSceneMapping::DMSetSceneMapExecuteflag(bool sceneExecution) {
	sceneExecutionStatus = sceneExecution;
}

bool DmSceneMapping::DMSceneDatabaseInit() {

	syslog(LOG_DEBUG, "[DM]: Scene open scene table: [%d]",
			DmSceneMapping::DMSceneDatabaseOpen());

	syslog(LOG_DEBUG, "[DM]: Scene create scene table: [%d]",
			DmSceneMapping::DMCreateSceneTable());

	return true;
}

static int callbackBackupDB(void *printData, int argc, char **argv,
		char **azColName) {

	Scenes *ScenesObj = new Scenes();

	ScenesObj->DMSetSeceneId(std::string(argv[0]));
	ScenesObj->DMSetSeceneName(std::string(argv[1]));
	ScenesObj->DMSetSecenePhrase(std::string(argv[2]));
	ScenesObj->DMSetSeceneMute(std::string(argv[3]));
	ScenesObj->DMSetSeceneIOTAction(json_tokener_parse((const char*) argv[4]));
	ScenesObj->DMSetSeceneNonIOTAction(
			json_tokener_parse((const char*) argv[5]));

	/*syslog(LOG_DEBUG, "[DM]: Scene scene map before adding size: [%ld]",
			DMSceneMapData.size());*/

	ScenesObj->SceneDisplay();

	DMSceneMapData[argv[0]] = ScenesObj;

	/*syslog(LOG_DEBUG, "[DM]: Scene scene map after adding size: [%ld]",
			DMSceneMapData.size());*/

	return 0;
}

bool DmSceneMapping::DMSceneDatabaseOpen() {

	int DMSceneQuerryRet;
//	char* DMSceneErrorRet;

	/* Open database */
	DMSceneQuerryRet = sqlite3_open("DeviceManagerBackUpDB.db", (&DMSceneDB));

	if (DMSceneQuerryRet != SQLITE_OK) {
		syslog(LOG_DEBUG, "[DM]: Scene insert scene data fail: [%s]",
				sqlite3_errmsg(DMSceneDB));
		return false;
	} else {
		fprintf(stdout, "[DM]: Scene rule data base open success ");
		return true;
	}
	return false;
}

bool DmSceneMapping::DMSceneDatabaseClose() {

	int DMSceneQuerryRet;
//	char* DMSceneErrorRet;

	/* Close database */
	DMSceneQuerryRet = sqlite3_close(DMSceneDB);

	if (DMSceneQuerryRet != SQLITE_OK) {
		syslog(LOG_DEBUG,
				"[DM]: Scene mapping close backup database fail: [%s]",
				sqlite3_errmsg(DMSceneDB));
		return false;
	} else {
		fprintf(stdout, "[DM]: Scene mapping close backup database success ");
		return true;
	}
}

bool DmSceneMapping::DMCreateSceneTable() {

	int DMSceneQuerryRet;
	char* DMSceneErrorRet;

	/* Create SQL statement */
	std::stringstream INSERT_SCENE;
	INSERT_SCENE
			<< "CREATE TABLE SCENE ( SCENE_ID	       TEXT  PRIMARY KEY NOT NULL, SCENE_NAME        TEXT, SCENE_PHRASE      TEXT, SCENE_MUTE        TEXT, IOT_ACTION        TEXT, NON_IOT_ACTION    TEXT);";

	DMSceneQuerryRet = sqlite3_exec(DMSceneDB, INSERT_SCENE.str().c_str(),
			callback, 0, &DMSceneErrorRet);

	if (DMSceneQuerryRet != SQLITE_OK) {
		fprintf(stderr,
				"[DM]: Scene mapping create scene table backup database fail: %s\n",
				DMSceneErrorRet);
		sqlite3_free(DMSceneErrorRet);
		return false;
	} else {
		fprintf(stdout,
				"[DM]: Scene mapping create scene table backup database success\n");
		return true;
	}
	return false;
}

bool DmSceneMapping::DMSceneModifyDB(std::string scene_id,
		std::string scene_name, std::string scene_phrase,
		std::string scene_mute, std::string iot_action,
		std::string non_iot_action) {

	//syslog(LOG_INFO, "[DM]: Scene scene modify start");

	int DMSceneQuerryRet;
	char* DMSceneErrorRet;

	std::stringstream UPDATE_STRING;
	UPDATE_STRING << "UPDATE SCENE SET SCENE_NAME= " << "\""

	<< scene_name << "\"" << ", " << "SCENE_PHRASE= " << "\"" << scene_phrase
			<< "\"" << ", " << "SCENE_MUTE= " << "\'" << scene_mute << "\'"
			<< ", " << "IOT_ACTION= " << "\'" << iot_action << "\'" << ", "
			<< "NON_IOT_ACTION= " << "\'" << non_iot_action << "\'"
			<< " WHERE SCENE_ID=" << "\'" << scene_id << "\';";

	DMSceneQuerryRet = sqlite3_exec(DMSceneDB, (UPDATE_STRING.str()).c_str(),
			callback, 0, &DMSceneErrorRet);

	if (DMSceneQuerryRet != SQLITE_OK) {
		syslog(LOG_DEBUG, "[DM]: scene scene modify fail status: [%s]",
				DMSceneErrorRet);
		sqlite3_free(DMSceneErrorRet);
		return false;
	} else {
		syslog(LOG_INFO, "[DM]: scene scene modify success");
		return true;
	}

	//syslog(LOG_INFO, "[DM]: Scene scene modify end");

	return true;
}

bool DmSceneMapping::DMSceneIdCheck(std::string SCENE_ID) {

	std::map<std::string, Scenes*>::iterator DMSceneMapDataItr =
			DMSceneMapData.find(SCENE_ID);
	if (DMSceneMapDataItr != DMSceneMapData.end()) {
		return true;
	} else {
		return false;
	}
	return false;
}

bool DmSceneMapping::DMSceneCreate(std::string scene_id, std::string scene_name,
		std::string scene_phrase, std::string scene_mute,
		std::string iot_action, std::string non_iot_action) {

	int DMSceneQuerryRet;
	char* DMSceneErrorRet;

	std::stringstream INSERT_SCENE;
	INSERT_SCENE << "INSERT INTO SCENE VALUES(" << "\'"

	<< scene_id << "\'" << "," << "\"" << scene_name << "\"" << ", " << "\""
			<< scene_phrase << "\"" << ", " << "\'" << scene_mute << "\'"
			<< ", " << "\'" << iot_action << "\'" << "," << "\'"
			<< non_iot_action << "\'" << ");";

	DMSceneQuerryRet = sqlite3_exec(DMSceneDB, (INSERT_SCENE.str()).c_str(),
			callback, 0, &DMSceneErrorRet);

	if (DMSceneQuerryRet != SQLITE_OK) {
		syslog(LOG_INFO, "[DM]: SCENE DMSceneCreate FAIL");
		sqlite3_free(DMSceneErrorRet);
		return false;

	} else {
		syslog(LOG_INFO, "[DM]: SCENE DMSceneCreate SUCCESS");
		return true;
	}
}

bool DmSceneMapping::DMGetSceneExecuteStatus(std::string scene_id) {

	bool ruleStateFlag = false;
	/*syslog(LOG_DEBUG,
			"[DM]: Request queue process scene execute status checking start for: [%s]",
			scene_id.c_str());*/

	std::map<std::string, Scenes*>::iterator DMSceneMapDataItr =
			DMSceneMapData.find(scene_id);
	if (DMSceneMapDataItr != DMSceneMapData.end()) {
		/*syslog(LOG_DEBUG,
				"[DM]: Request queue process scene execute status found");*/

		ruleStateFlag = DMSceneMapDataItr->second->DMGetSceneExecuteflag();

		//syslog(LOG_DEBUG, "[DM]: Request queue process scene success ");
		return ruleStateFlag;
	} else {
		syslog(LOG_DEBUG,
				"[DM]: Request queue process scene execute status not found");
		return ruleStateFlag;
	}
	/*syslog(LOG_DEBUG,
			"[DM]: Request queue process scene execute status checking start");*/
	return ruleStateFlag;

}
bool DmSceneMapping::DMSetSceneExecuteStatus(std::string scene_id) {

//SETTING FLAG TO SET SCENE STATUE TO FALSE
	bool ruleStateFlag = false;

	/*syslog(LOG_DEBUG,
			"[DM]: Request queue process scene setting execute status start for: [%s]",
			scene_id.c_str());*/

	std::map<std::string, Scenes*>::iterator DMSceneMapDataItr =
			DMSceneMapData.find(scene_id);
	if (DMSceneMapDataItr != DMSceneMapData.end()) {

		/*syslog(LOG_DEBUG,
				"[DM]: Request queue process scene setting execute status found");*/

		DMSceneMapDataItr->second->DMSetSceneExecuteflag(false);

		ruleStateFlag = true;
		//syslog(LOG_DEBUG, "[DM]: Request queue process scene setting success ");

		return ruleStateFlag;
	} else {
		syslog(LOG_DEBUG,
				"[DM]: Request queue process scene setting execute status not found");
		return ruleStateFlag;
	}
	/*syslog(LOG_DEBUG,
			"[DM]: Request queue process scene setting execute status checking start");*/
	return ruleStateFlag;

}

bool DmSceneMapping::DMCallSceneCreate(json_object* scene_data) {

	std::cout << "SCENE_CREATE_DATA : " << json_object_get_string(scene_data)
			<< std::endl;

	json_object* scenes = json_object_object_get(scene_data, "scenes");

	json_type jsontypeofscenes = json_object_get_type(scenes);
	size_t lengthofscenes;

	/** Checking whether the json structure of scenes is array or not */
	if (jsontypeofscenes == json_type_array) {
		lengthofscenes = json_object_array_length(scenes);
	} else {
		lengthofscenes = ZERO_LENGTH;
	}

	for (size_t messageSize = 0; messageSize < lengthofscenes; messageSize++) {

		json_object* scene_id = json_object_object_get(
				json_object_array_get_idx(scenes, messageSize), "scene_id");

		json_object* scene_name = json_object_object_get(
				json_object_array_get_idx(scenes, messageSize), "scene_name");

		json_object* scene_phrase = json_object_object_get(
				json_object_array_get_idx(scenes, messageSize), "scene_phrase");

		json_object* scene_mute = json_object_object_get(
				json_object_array_get_idx(scenes, messageSize), "isMuted");

		json_object* actions = json_object_object_get(
				json_object_array_get_idx(scenes, messageSize), "actions");

		json_object* iot_action = json_object_new_object();
		json_object* non_iot_action = json_object_new_array();

		/*syslog(LOG_DEBUG,
				"[DM]: Request queue process scene create action: [%s]",
				json_object_get_string(actions));*/

		json_type jsontypeofactions = json_object_get_type(actions);
		size_t lengthofactions;
		/** Checking whether the json structure of actions is array or not */
		if (jsontypeofactions == json_type_array) {
			lengthofactions = json_object_array_length(actions);
		} else {
			lengthofactions = ZERO_LENGTH;
		}

		for (size_t messageSize = 0; messageSize < lengthofactions;
				messageSize++) {

			/*syslog(LOG_DEBUG,
					"[DM]: Request queue process scene create action: [%s]",
					json_object_get_string(
							json_object_array_get_idx(actions, messageSize)));*/

			json_object* action_data = json_object_array_get_idx(actions,
					messageSize);

			json_object* action_type = json_object_object_get(
					json_object_array_get_idx(actions, messageSize),
					"action_type");

			/*syslog(LOG_DEBUG,
					"[DM]: Request queue process scene create action_type: [%s]",
					json_object_get_string(action_type));*/

			if (strcasecmp(json_object_get_string(action_type), "iot") == 0) {

				iot_action = json_object_object_get(
						json_object_array_get_idx(actions, messageSize),
						"things");

				/*syslog(LOG_DEBUG,
						"[DM]: Request queue process scene create iot_action: [%s]",
						json_object_get_string(iot_action));*/

			} else if (strcasecmp(json_object_get_string(action_type), "music")
					== 0) {

				json_object_array_add(non_iot_action, action_data);

				/*syslog(LOG_DEBUG,
						"[DM]: Request queue process scene create music_action: [%s]",
						json_object_get_string(non_iot_action));*/

			} else if (strcasecmp(json_object_get_string(action_type), "news")
					== 0) {

				json_object_array_add(non_iot_action, action_data);

				/*syslog(LOG_DEBUG,
						"[DM]: Request queue process scene create news_action: [%s]",
						json_object_get_string(non_iot_action));*/

			} else if (strcasecmp(json_object_get_string(action_type), "weather")
					== 0) {

				json_object_array_add(non_iot_action, action_data);

				/*syslog(LOG_DEBUG,
						"[DM]: Request queue process scene create weather_action: [%s]",
						json_object_get_string(non_iot_action));*/
			}
		}

		bool ScencIDCheck = DMSceneIdCheck(json_object_get_string(scene_id));

		/*syslog(LOG_DEBUG,
				"[DM]: Request queue process create scene id check status: [%d]",
				ScencIDCheck);*/

		if (ScencIDCheck == true) {

			/*syslog(LOG_DEBUG,
					"[DM]: Request queue process scene create for scene_id time create: [%s] time updated: [%s]",
					json_object_get_string(
							json_object_object_get(
									json_object_array_get_idx(scenes,
											messageSize), "createdAt")),
					json_object_get_string(
							json_object_object_get(
									json_object_array_get_idx(scenes,
											messageSize), "updatedAt")));*/

			if (strcmp(
					json_object_get_string(
							json_object_object_get(
									json_object_array_get_idx(scenes,
											messageSize), "createdAt")),
					json_object_get_string(
							json_object_object_get(
									json_object_array_get_idx(scenes,
											messageSize), "updatedAt"))) != 0) {

				/*syslog(LOG_DEBUG,
						"[DM]: Request queue process scene create for scene_id: [%s]",
						json_object_get_string(scene_id));*/

				std::map<std::string, Scenes*>::iterator DMSceneMapDataItr =
						DMSceneMapData.find(
								std::string(json_object_get_string(scene_id)));
				if (DMSceneMapDataItr != DMSceneMapData.end()) {

					syslog(LOG_DEBUG,
							"[DM]: Request queue process scene create calling for scene_id: [%s]",
							json_object_get_string(scene_id));

					DMSceneMapDataItr->second->DMSceneModify(
							json_object_get_string(scene_id),
							json_object_get_string(scene_name),
							json_object_get_string(scene_phrase),
							json_object_get_string(scene_mute), iot_action,
							non_iot_action);
				} else {

				}

			} else {

				syslog(LOG_DEBUG,
						"[DM]: Request queue process cene create for scene_id skipped: [%s]",
						json_object_get_string(scene_id));
			}

		} else {

			/*syslog(LOG_DEBUG,
					"[DM]: Request queue proces sscene create for scene_id: [%s]",
					json_object_get_string(scene_id));*/

			Scenes* SceneObj = new Scenes();

			SceneObj->DMSetSeceneId(json_object_get_string(scene_id));
			/*syslog(LOG_DEBUG,
					"[DM]: Request queue process scene create for scene_id: [%s]",
					json_object_get_string(scene_id));*/
			SceneObj->DMSetSeceneName(json_object_get_string(scene_name));
			/*syslog(LOG_DEBUG,
					"[DM]: Request queue process scene create for scene_id: [%s]",
					json_object_get_string(scene_name));*/
			SceneObj->DMSetSecenePhrase(json_object_get_string(scene_phrase));
			/*syslog(LOG_DEBUG,
					"[DM]: Request queue process scene create for scene_id: [%s]",
					json_object_get_string(scene_phrase));*/
			SceneObj->DMSetSeceneMute(json_object_get_string(scene_mute));
			/*syslog(LOG_DEBUG,
					"[DM]: Request queue process scene create for scene_id: [%s]",
					json_object_get_string(scene_mute));*/
			SceneObj->DMSetSeceneIOTAction(iot_action);
			/*syslog(LOG_DEBUG,
					"[DM]: Request queue process scene create for scene_id: [%s]",
					json_object_get_string(iot_action));*/
			SceneObj->DMSetSeceneNonIOTAction(non_iot_action);
			/*syslog(LOG_DEBUG,
					"[DM]: Request queue process scene create for scene_id: [%s]",
					json_object_get_string(non_iot_action));*/

			/*syslog(LOG_DEBUG, "[DM]: Scene map before adding size: [%ld]",
					DMSceneMapData.size());*/

			DMSceneMapData[SceneObj->DMGetSeceneId()] = SceneObj;

			/*syslog(LOG_DEBUG, "[DM]: Scene map after adding size: [%ld]",
					DMSceneMapData.size());*/

			syslog(LOG_DEBUG,
								"[DM]: Request queue process scene create for scene_id: [%s]  scene_name:[%s] scene_phrase:[%s] scene_mute:[%s] iot_action:[%s]  non_iot_action:[%s]",
								json_object_get_string(scene_id),json_object_get_string(scene_name),json_object_get_string(scene_phrase),json_object_get_string(scene_mute),json_object_get_string(iot_action),json_object_get_string(non_iot_action));

			DmSceneMapping::DMSceneCreate(json_object_get_string(scene_id),
					json_object_get_string(scene_name),
					json_object_get_string(scene_phrase),
					json_object_get_string(scene_mute),
					json_object_get_string(iot_action),
					json_object_get_string(non_iot_action));
//			delete SceneObj;
		}
	}

	return true;
}

void DmSceneMapping::DeleteThingIDFromScenes(std::string thing_id) {

	//syslog(LOG_INFO, "[DM]: Call scene modify delete start");

	for (std::map<std::string, Scenes*>::iterator DMSceneMapDataItr =
			DMSceneMapData.begin(); DMSceneMapDataItr != DMSceneMapData.end();
			++DMSceneMapDataItr) {

		std::string IOT_Data =
				DmSceneMapping::CompareThingIdFromSceneMap_AndDelete(
						DMSceneMapDataItr->second->DMGetSeceneIOTAction(),
						thing_id);

		DMSceneMapDataItr->second->DMSetSeceneIOTAction(
				json_tokener_parse(IOT_Data.c_str()));

		DmSceneMapping::DMSceneModifyDB(
				DMSceneMapDataItr->second->DMGetSeceneId(),
				DMSceneMapDataItr->second->DMGetSeceneName(),
				DMSceneMapDataItr->second->DMGetSecenePhrase(),
				DMSceneMapDataItr->second->DMGetSeceneMute(),
				json_object_get_string(
						DMSceneMapDataItr->second->DMGetSeceneIOTAction()),
				json_object_get_string(
						DMSceneMapDataItr->second->DMGetSeceneNonIOTAction()));
	}

}

std::string DmSceneMapping::CompareThingIdFromSceneMap_AndDelete(
		json_object* things, std::string Input_thing_id) {

	/*syslog(LOG_INFO, "[DM]: Call scene modify start for thing_id : [%s]",
			Input_thing_id.c_str());*/

	json_object *UpdateJson = json_object_new_array();

	json_type jsontypeofthings = json_object_get_type(things);
	size_t lengthofthings;

	/** Checking whether the json structure of things is array or not */
	if (jsontypeofthings == json_type_array) {
		lengthofthings = json_object_array_length(things);
	} else {
		lengthofthings = ZERO_LENGTH;
	}

	for (size_t messageSize = 0; messageSize < lengthofthings; messageSize++) {

		json_object* TotalData = json_object_array_get_idx(things, messageSize);

		json_object* thing_id = json_object_object_get(TotalData, "thing_id");

		if ((std::string(json_object_get_string(thing_id)).compare(
				Input_thing_id)) == 0) {
			syslog(LOG_INFO,
					"[DM]: Call scene modify for skipping thing_id : [%s]",
					Input_thing_id.c_str());
		} else {
			json_object_array_add(UpdateJson, TotalData);
			syslog(LOG_INFO, "[DM]: Call scene modify for thing_id : [%s]",
					Input_thing_id.c_str());
		}
	}
	////json_object_put(UpdateJson);
	return json_object_get_string(UpdateJson);
}

bool DmSceneMapping::DMCallSceneModify(json_object* scene_data) {

	json_object* iot_action = json_object_new_object();
	json_object* non_iot_action = json_object_new_array();

	//syslog(LOG_INFO, "[DM]: Call scene modify start");

	std::cout << "SCENE_DATA : " << json_object_get_string(scene_data)
			<< std::endl;

	json_type jsontypeofscenedata = json_object_get_type(scene_data);
	size_t lengthofscenedata;
	/** Checking whether the json structure of scene_data is array or not */
	if (jsontypeofscenedata == json_type_array) {
		lengthofscenedata = json_object_array_length(scene_data);
	} else {
		lengthofscenedata = ZERO_LENGTH;
	}

	for (size_t messageSize = 0; messageSize < lengthofscenedata;
			messageSize++) {

		json_object* scene_id = json_object_object_get(
				json_object_array_get_idx(scene_data, messageSize), "scene_id");

		json_object* scene_name = json_object_object_get(
				json_object_array_get_idx(scene_data, messageSize),
				"scene_name");

		json_object* scene_phrase = json_object_object_get(
				json_object_array_get_idx(scene_data, messageSize),
				"scene_phrase");

		json_object* scene_mute = json_object_object_get(
				json_object_array_get_idx(scene_data, messageSize), "isMuted");

		json_object* actions = json_object_object_get(
				json_object_array_get_idx(scene_data, messageSize), "actions");

		/*syslog(LOG_DEBUG,
				"[DM]: Request queue process scene modify action: [%s]",
				json_object_get_string(actions));*/

		json_type jsontypeofactions = json_object_get_type(actions);
		size_t lengthofactions;

		/** Checking whether the json structure of actions is array or not */
		if (jsontypeofactions == json_type_array) {
			lengthofactions = json_object_array_length(actions);
		} else {
			lengthofactions = ZERO_LENGTH;
		}

		for (size_t messageSize = 0; messageSize < lengthofactions;
				messageSize++) {

			syslog(LOG_DEBUG,
					"[DM]: Request queue process scene modify action: [%s]",
					json_object_get_string(
							json_object_array_get_idx(actions, messageSize)));

			json_object* action_data = json_object_array_get_idx(actions,
					messageSize);

			json_object* action_type = json_object_object_get(
					json_object_array_get_idx(actions, messageSize),
					"action_type");

			/*syslog(LOG_DEBUG,
					"[DM]: Request queue process scene modify action_type: [%s]",
					json_object_get_string(action_type));*/

			if (strcasecmp(json_object_get_string(action_type), "iot") == 0) {

				iot_action = json_object_object_get(
						json_object_array_get_idx(actions, messageSize),
						"things");

				/*syslog(LOG_DEBUG,
						"[DM]: Request queue process scene modify iot_action: [%s]",
						json_object_get_string(iot_action));*/

			} else if (strcasecmp(json_object_get_string(action_type), "music")
					== 0) {

				json_object_array_add(non_iot_action, action_data);

				/*syslog(LOG_DEBUG,
						"[DM]: Request queue process scene modify music_action: [%s]",
						json_object_get_string(non_iot_action));*/

			} else if (strcasecmp(json_object_get_string(action_type), "news")
					== 0) {

				json_object_array_add(non_iot_action, action_data);

				/*syslog(LOG_DEBUG,
						"[DM]: Request queue process scene modify news_action: [%s]",
						json_object_get_string(non_iot_action));*/

			} else if (strcasecmp(json_object_get_string(action_type), "weather")
					== 0) {

				json_object_array_add(non_iot_action, action_data);

				/*syslog(LOG_DEBUG,
						"[DM]: Request queue process scene modify weather_action: [%s]",
						json_object_get_string(non_iot_action));*/
			}
		}

		/*syslog(LOG_DEBUG,
				"[DM]: Request queue process scenemodify for scene_id: [%s]",
				json_object_get_string(scene_id));*/

		std::map<std::string, Scenes*>::iterator DMSceneMapDataItr =
				DMSceneMapData.find(
						std::string(json_object_get_string(scene_id)));
		if (DMSceneMapDataItr != DMSceneMapData.end()) {

			/*syslog(LOG_DEBUG,
					"[DM]: Request queue process scene modify calling for scene_id: [%s]",
					json_object_get_string(scene_id));*/

			DMSceneMapDataItr->second->DMSceneModify(
					json_object_get_string(scene_id),
					json_object_get_string(scene_name),
					json_object_get_string(scene_phrase),
					json_object_get_string(scene_mute), iot_action,
					non_iot_action);

			bool SceneModifyStatus = DmSceneMapping::DMSceneModifyDB(
					json_object_get_string(scene_id),
					json_object_get_string(scene_name),
					json_object_get_string(scene_phrase),
					json_object_get_string(scene_mute),
					json_object_get_string(iot_action),
					json_object_get_string(non_iot_action));

			if (SceneModifyStatus == true) {
				syslog(LOG_INFO, "[DM]: Call scene modify success ");
				return true;
			} else {
				syslog(LOG_INFO, "[DM]: Call scene modify fail ");

				return false;
			}

		} else {

		}

	}
	////json_object_put(iot_action);
	////json_object_put(non_iot_action);
	return false;
}

SecneRuleExecution DmSceneMapping::DMCallSceneExecute(
		json_object * scene_data) {

	SecneRuleExecution SecneExecutionData;

	json_type jsontypeofscenedata = json_object_get_type(scene_data);
	size_t lengthofscenedata;
	/** Checking whether the json structure of scene_data is array or not */
	if (jsontypeofscenedata == json_type_array) {
		lengthofscenedata = json_object_array_length(scene_data);
	} else {
		lengthofscenedata = ZERO_LENGTH;
	}

	for (size_t messageSize = 0; messageSize < lengthofscenedata;
			messageSize++) {

		syslog(LOG_DEBUG,
				"[DM]: Request queue process scene run scene_data: [%s]",
				json_object_get_string(
						json_object_array_get_idx(scene_data, messageSize)));

		json_object* scene_id = json_object_object_get(
				json_object_array_get_idx(scene_data, messageSize), "scene_id");

		/*syslog(LOG_DEBUG,
				"[DM]: Request queue process scene run scene_id: [%s]",
				json_object_get_string(scene_id));*/

		std::map<std::string, Scenes*>::iterator DMSceneMapDataItr =
				DMSceneMapData.find(json_object_get_string(scene_id));
		if (DMSceneMapDataItr != DMSceneMapData.end()) {

			DMSceneMapDataItr->second->SceneDisplay();

			SecneExecutionData = DMSceneMapDataItr->second->DMSceneExecute(
					json_object_get_string(scene_id));

			if (SecneExecutionData.Sceneflag == 2
					/*|| SecneExecutionData.Sceneflag == 3*/) {

				DmSceneMapping::DMSetSceneMapExecuteflag(true);

			} else {
				DmSceneMapping::DMSetSceneMapExecuteflag(false);

			}
			SecneExecutionData.RuleSceneID = json_object_get_string(scene_id);

			return SecneExecutionData;
		}
	}

	return SecneExecutionData;
}

void DmSceneMapping::DMScenePrintMap() {

	//syslog(LOG_INFO, "[DM]: Sceneprinting start");

	for (std::map<std::string, Scenes*>::iterator DMSceneMapDataItr =
			DMSceneMapData.begin(); DMSceneMapDataItr != DMSceneMapData.end();
			++DMSceneMapDataItr) {

		DMSceneMapDataItr->second->SceneDisplay();
	}

	//syslog(LOG_INFO, "[DM]: Scene printing end");
}

void DmSceneMapping::DMScenePrintDB() {

	//syslog(LOG_INFO, "[DM]: Scene scene print database start");

	int DMSceneQuerryRet;
	char* DMSceneErrorRet;

	std::stringstream UPDATE_STRING;
	UPDATE_STRING << "SELECT * from SCENE ;";

	DMSceneQuerryRet = sqlite3_exec(DMSceneDB, (UPDATE_STRING.str()).c_str(),
			callback, 0, &DMSceneErrorRet);

	if (DMSceneQuerryRet != SQLITE_OK) {
		syslog(LOG_INFO, "[DM]: Print scene database fail");
		sqlite3_free(DMSceneErrorRet);
	} else {
		syslog(LOG_INFO, "[DM]: Scene database success");
	}

	//syslog(LOG_INFO, "[DM]: Scene scene print database end");
}

bool DmSceneMapping::DMRecoverMapFromSceneDB() {

	//syslog(LOG_INFO, "[DM]: Scene recover map from scene database start");

	DmSceneMapping::DMScenePrintDB();

	int DMSceneQuerryRet;
	char* DMSceneErrorRet;

	std::stringstream UPDATE_STRING;
	UPDATE_STRING << "SELECT * FROM SCENE ;";

	DMSceneQuerryRet = sqlite3_exec(DMSceneDB, (UPDATE_STRING.str()).c_str(),
			callbackBackupDB, 0, &DMSceneErrorRet);

	if (DMSceneQuerryRet != SQLITE_OK) {
		sqlite3_free(DMSceneErrorRet);
		syslog(LOG_INFO,
				"[DM]: Scene recover map from scene database fail end");
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Scene recover map from scene database success end");
		return true;
	}
	return false;
}

