#include "dm_rule_mapping.hpp"
#include "dm_thingmap.hpp"

#define ZERO_LENGTH 0
sqlite3* DMRuleDB;

std::map<std::string, Rules*> DMRuleMapData;

std::vector<std::string> RuleVecRet;

std::string Rules::get_rule_id() {
	return this->rule_id;
}

void Rules::set_rule_id(std::string rule_id) {
	this->rule_id = rule_id;
}

std::string Rules::get_rule_type() {
	return this->rule_type;
}

void Rules::set_rule_type(std::string rule_type) {
	this->rule_type = rule_type;
}

std::string Rules::get_time_flag() {
	return this->timeflag;
}

void Rules::set_time_flag(std::string timeflag) {
	this->timeflag = timeflag;
}

std::string Rules::get_sensor_id() {
	return this->sensor_id;
}

void Rules::set_sensor_id(std::string sensor_id) {
	this->sensor_id = sensor_id;
}

std::string Rules::get_entity_id() {
	return this->entity_id;
}

void Rules::set_entity_id(std::string entity_id) {
	this->entity_id = entity_id;
}

std::string Rules::get_property_name() {
	return this->property_name;
}

void Rules::set_property_name(std::string property_name) {
	this->property_name = property_name;
}

std::string Rules::get_property_value() {

	return this->property_value;
}

void Rules::set_property_value(std::string property_value) {
	this->property_value = property_value;
}

std::string Rules::get_sensor_condition() {
	return this->sensor_condition;
}

void Rules::set_sensor_condition(std::string sensor_condition) {
	this->sensor_condition = sensor_condition;
}

std::string Rules::get_time_condition() {
	return this->time_condition;
}

void Rules::set_time_condition(std::string time_condition) {
	this->time_condition = time_condition;
}

std::string Rules::get_time_start() {
	return this->time_start;
}

void Rules::set_time_start(std::string time_start) {
	this->time_start = time_start;
}

std::string Rules::get_time_end() {
	return this->time_end;
}

void Rules::set_time_end(std::string time_end) {
	this->time_end = time_end;
}

json_object *Rules::get_iot_action() {
	return this->iot_action;
}

void Rules::set_iot_action(json_object *iot_action) {
	this->iot_action = iot_action;
}

json_object *Rules::get_non_iot_action() {
	return this->non_iot_action;
}

void Rules::set_non_iot_action(json_object *non_iot_action) {
	this->non_iot_action = non_iot_action;
}

SecneRuleExecution Rules::execute_rule() {

	SecneRuleExecution SecneRuleExecutionObj;
	bool iot = false;
	bool non_iot = false;

	syslog(LOG_DEBUG,
			"[DM]: Dm_rulemapping executing rule with /'rule_ID/' : [%s]",
			this->rule_id.c_str());

	//---------------------------RULE START--------------------------------------------

	/*syslog(LOG_DEBUG, "[DM]: Rule execute start for iot : [%s]",
	 json_object_get_string(this->iot_action));

	 syslog(LOG_DEBUG, "[DM]: Rule execute start for non-iot : [%s]",
	 json_object_get_string(this->non_iot_action));*/

//--------------------------------------------------------------------------------------
	json_type jsontypeofATRCount1 = json_object_get_type(this->iot_action);

	if (jsontypeofATRCount1 == json_type_array) {
		syslog(LOG_DEBUG,
				"[DM]: Dm_rulemapping start execution /'rule/' for iot count: [%zu]",
				json_object_array_length(this->iot_action));

		if (json_object_array_length(this->iot_action) >= 1) {
//ACTION IOT DATA
			iot = true;
			/*syslog(LOG_INFO,
			 "[DM]: Rule execute start for scene_id iot success");*/
		} else {
//NON ACTION IOT DATA
			iot = false;
			/*syslog(LOG_INFO, "[DM]: Rule execute start for scene_id iot fail");*/
		}
	} else {
		syslog(LOG_DEBUG,
				"[DM]: Dm_rulemapping execution for /'rule/' has NULL array data");
	}
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
	json_type jsontypeofATRCount = json_object_get_type(this->non_iot_action);

	if (jsontypeofATRCount == json_type_array) {

		/*syslog(LOG_DEBUG, "[DM]: Rule execute start for non-iot count: [%ld]",
		 json_object_array_length(this->non_iot_action));

		 syslog(LOG_DEBUG,
		 "[DM]: Rule execute start for non-iot data all : [%s]",
		 json_object_get_string(this->non_iot_action));*/

		if (json_object_array_length(this->non_iot_action) >= 1) {

			/*syslog(LOG_INFO,
			 "[DM]: Rule execute start for scene_id non iot array not null");*/

			//handling non-iot actions
			for (size_t arrayLength = 0;
					arrayLength < json_object_array_length(this->non_iot_action);
					arrayLength++) {

				json_object* non_iot_json = json_object_array_get_idx(
						this->non_iot_action, arrayLength);

				/*syslog(LOG_DEBUG,
				 "[DM]: Rule execute start for non-iot data : [%s]",
				 json_object_get_string(non_iot_json));*/

				json_object* action_type = json_object_object_get(non_iot_json,
						"action_type");

				/*syslog(LOG_DEBUG,
				 "[DM]: Rule execute start for non-iot action_type : [%s]",
				 json_object_get_string(action_type));*/

				if (strcmp(json_object_get_string(action_type), "music") == 0
						|| strcmp(json_object_get_string(action_type),
								"weather") == 0
						|| strcmp(json_object_get_string(action_type), "news")
								== 0) {

					/*syslog(LOG_INFO,
					 "[DM]: Rule execute start for non iot actions");*/

					json_object* urls = json_object_object_get(non_iot_json,
							"urls");

					/*syslog(LOG_DEBUG,
					 "[DM]: Rule execute start for non-iot urls : [%s]",
					 json_object_get_string(urls));*/

					json_type urlstype = json_object_get_type(urls);
					if (urlstype == json_type_array) {
						if (json_object_array_length(urls) >= 1) {
							non_iot = true;
							/*syslog(LOG_INFO,
							 "[DM]: Rule execute start for scene_id non iot urls success");*/
							break;

						} else {
							non_iot = false;
							syslog(LOG_DEBUG,
									"[DM]: Dm_rulemapping execution for rule_id has NULL URL");
							//break;
						}
					}

				} else {
					syslog(LOG_DEBUG,
							"[DM]: Dm_rulemapping execution for rule_id has no /'NON-IOT/' data");
				}
			}
		} else {
			syslog(LOG_DEBUG,
					"[DM]: Dm_rulemapping execution for rule_id has /'NON-IOT/' array NULL");
		}
	} else {
		syslog(LOG_DEBUG,
				"[DM]: Dm_rulemapping execution for rule_id has /'NON-IOT/' array NULL");
	}
//--------------------------------------------------------------------------------------

	/*syslog(LOG_DEBUG,
	 "[DM]: Rule execute start for iot state : [%d] non_iot state : [%d]",
	 iot, non_iot);*/

	if (iot == true) {
		if (non_iot == true) {
//BoTH
//			/*syslog(LOG_DEBUG,
//			 "[DM]: Rule execute start for scene_id BOTH : [%s]",
//			 this->rule_id.c_str());*/
//
//			SecneRuleExecutionObj.iot_action = this->iot_action;
//
//			SecneRuleExecutionObj.non_iot_action = this->non_iot_action;
//			SecneRuleExecutionObj.RuleSceneID = this->rule_id;
//			SecneRuleExecutionObj.Sceneflag = 3;
//			SecneRuleExecutionObj.RuleSceneExecuteFlag = true;
//			this->DMSetRuleExecuteflag(true);
//			return SecneRuleExecutionObj;

		} else {
//IOT
			syslog(LOG_DEBUG,
					"[DM]: Dm_rulemapping  start execution of IOT data for /'rule_ID/' : [%s]",
					this->rule_id.c_str());

			SecneRuleExecutionObj.iot_action = this->iot_action;

			SecneRuleExecutionObj.non_iot_action = this->non_iot_action;
			SecneRuleExecutionObj.RuleSceneID = this->rule_id;
			SecneRuleExecutionObj.Sceneflag = 2;
			SecneRuleExecutionObj.RuleSceneExecuteFlag = true;
			this->DMSetRuleExecuteflag(true);
			return SecneRuleExecutionObj;
		}

	} else {
//NON IOT
//		/*syslog(LOG_DEBUG,
//		 "[DM]: Rule execute start for scene_id NON IOT : [%s]",
//		 this->rule_id.c_str());*/
//
//		if (non_iot == true) {
//
//			SecneRuleExecutionObj.iot_action = this->iot_action;
//
//			SecneRuleExecutionObj.non_iot_action = this->non_iot_action;
//			SecneRuleExecutionObj.RuleSceneID = this->rule_id;
//			SecneRuleExecutionObj.Sceneflag = 1;
//			SecneRuleExecutionObj.RuleSceneExecuteFlag = false;
//			this->DMSetRuleExecuteflag(false);
//
//			return SecneRuleExecutionObj;
//		}
	}

//---------------------------RULE END--------------------------------------------
	return SecneRuleExecutionObj;
}

void Rules::RuleDisplay() {

	syslog(LOG_INFO, "[DM]: Dm_rulemapping rule display start");
	syslog(LOG_DEBUG, "[DM]: Dm_rulemapping rule_id: [%s] ",
			this->rule_id.c_str());
	syslog(LOG_DEBUG, "[DM]: Dm_rulemapping rule_type: [%s] ",
			this->rule_type.c_str());
	syslog(LOG_DEBUG, "[DM]: Dm_rulemapping time flag: [%s] ",
			this->timeflag.c_str());
	syslog(LOG_DEBUG, "[DM]: Dm_rulemapping sensor_id: [%s] ",
			this->sensor_id.c_str());
	syslog(LOG_DEBUG, "[DM]: Dm_rulemapping entity_id: [%s] ",
			this->entity_id.c_str());
	syslog(LOG_DEBUG, "[DM]: Dm_rulemapping property_name: [%s] ",
			this->property_name.c_str());
	syslog(LOG_DEBUG, "[DM]: Dm_rulemapping property_value: [%s] ",
			this->property_value.c_str());
	syslog(LOG_DEBUG, "[DM]: Dm_rulemapping sensor_condition: [%s] ",
			this->sensor_condition.c_str());
	syslog(LOG_DEBUG, "[DM]: Dm_rulemapping time_start: [%s] ",
			this->time_start.c_str());
	syslog(LOG_DEBUG, "[DM]: Dm_rulemapping time_end: [%s] ",
			this->time_end.c_str());
	syslog(LOG_DEBUG, "[DM]: Dm_rulemapping iot_action: [%s] ",
			json_object_get_string(this->iot_action));
	syslog(LOG_DEBUG, "[DM]: Dm_rulemapping non_iot_action: [%s] ",
			json_object_get_string(this->non_iot_action));
}

bool Rules::modify_rule(std::string rule_id, std::string rule_type,
		std::string timeflag, std::string sensor_id, std::string entity_id,
		std::string property_name, std::string property_value,
		std::string sensor_condition, std::string time_condition,
		std::string time_start, std::string time_end, json_object * iot_action,
		json_object * non_iot_action) {

	this->set_rule_id(rule_id);
	this->set_rule_type(rule_type);
	this->set_time_flag(timeflag);
	this->set_sensor_id(sensor_id);
	this->set_entity_id(entity_id);
	this->set_property_name(property_name);
	this->set_property_value(property_value);
	this->set_sensor_condition(sensor_condition);
	this->set_time_condition(time_condition);
	this->set_time_start(time_start);
	this->set_time_end(time_end);
	this->set_iot_action(iot_action);
	this->set_non_iot_action(non_iot_action);

	this->RuleDisplay();

	return true;
}

bool Rules::DMCreateRule(std::string rule_id, std::string rule_type,
		std::string timeflag, std::string sensor_id, std::string entity_id,
		std::string property_name, std::string property_value,
		std::string sensor_condition, std::string time_condition,
		std::string time_start, std::string time_end, json_object* iot_action,
		json_object* non_iot_action) {

	syslog(LOG_INFO, "[DM]: Dm_rulemapping process to rule create");

	std::map<std::string, Rules*>::iterator DMRuleMapDataItr =
			DMRuleMapData.find(rule_id);
	if (DMRuleMapDataItr != DMRuleMapData.end()) {

	} else {
		/*syslog(LOG_DEBUG, "[DM]: Request queue process rule creating ");*/
		Rules* RulesObj = new Rules;
		RulesObj->set_rule_id(rule_id);
		RulesObj->set_rule_type(rule_type);
		RulesObj->set_time_flag(timeflag);
		RulesObj->set_sensor_id(sensor_id);
		RulesObj->set_entity_id(entity_id);
		RulesObj->set_property_name(property_name);
		RulesObj->set_property_value(property_value);
		RulesObj->set_sensor_condition(sensor_condition);
		RulesObj->set_time_condition(time_condition);
		RulesObj->set_time_start(time_start);
		RulesObj->set_time_end((time_end));
		RulesObj->set_iot_action(iot_action);
		RulesObj->set_non_iot_action(non_iot_action);
		DMRuleMapData[RulesObj->get_rule_id()] = RulesObj;
		syslog(LOG_DEBUG, "[DM]:Dm_rulemapping /'rule_mapped/' with : [%s]",
				(sensor_id + entity_id + property_name + property_value).c_str());
		RulesObj->RuleDisplay();
	}

	return true;
}

bool Rules::DMGetRuleExecuteflag() {

	/*syslog(LOG_DEBUG, "[DM]: Rule dm get rule executing flag value: [%d]",
	 this->RuleExecuteflag);*/

	return this->RuleExecuteflag;
}
void Rules::DMSetRuleExecuteflag(bool RuleExecuteflag) {

	/*syslog(LOG_DEBUG, "[DM]: Rule dm set rule executing flag value: [%d]",
	 RuleExecuteflag);*/

	this->RuleExecuteflag = RuleExecuteflag;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {

	RuleVecRet.clear();
	for (int i = 0; i < argc; i++) {
		std::string s(argv[i]);
		syslog(LOG_DEBUG,
				"[DM]: Dm_rulemapping prints database result rule [%s] : rule value [%s] ",
				azColName[i], argv[i]);
		RuleVecRet.push_back(s);
	}
	return 0;
}

bool DmRuleMapping::DMRuleDatabaseInit() {

	syslog(LOG_DEBUG, "[DM]: Dm_rulemapping open rule table: [%d]",
			DmRuleMapping::DMRuleDatabaseOpen());

	syslog(LOG_DEBUG, "[DM]: Dm_rulemapping create rule table: [%d]",
			DmRuleMapping::DMCreateRuleTable());

	return true;
}

static int callbackRuleDBtoMap(void *NotUsed, int argc, char **argv,
		char **azColName) {

	Rules* RulesObj = new Rules;
	RulesObj->set_rule_id(argv[0]);
	RulesObj->set_rule_type(argv[1]);
	RulesObj->set_time_flag(argv[2]);
	RulesObj->set_sensor_id(argv[3]);
	RulesObj->set_entity_id(argv[4]);
	RulesObj->set_property_name(argv[5]);
	RulesObj->set_property_value(argv[6]);
	RulesObj->set_sensor_condition(argv[7]);
	RulesObj->set_time_condition(argv[8]);
	RulesObj->set_time_start(argv[9]);
	RulesObj->set_time_end(argv[10]);
	RulesObj->set_iot_action(json_tokener_parse(argv[11]));
	RulesObj->set_non_iot_action(json_tokener_parse(argv[12]));

	/*syslog(LOG_DEBUG, "[DM]: Rule Rule map before adding size: [%ld]",
	 DMRuleMapData.size());*/

	RulesObj->RuleDisplay();

	DMRuleMapData[RulesObj->get_rule_id()] = RulesObj;

	/*syslog(LOG_DEBUG, "[DM]: Rule Rule map after adding size: [%ld]",
	 DMRuleMapData.size());*/

	return 0;
}

bool DmRuleMapping::DMRuleDatabaseClose() {

	int DMRuleQuerryRet;
//	char* DMRuleErrorRet;

	/* Close database */
	DMRuleQuerryRet = sqlite3_close(DMRuleDB);

	if (DMRuleQuerryRet) {
		syslog(LOG_DEBUG,
				"[DM]: Dm_rulemapping closing /'BACKUPDADABASE/' failed: [%s]",
				sqlite3_errmsg(DMRuleDB));
		return false;
	} else {
		fprintf(stdout,
				"[DM]: Dm_rulemapping closing /'BACKUPDADABASE/' success");
		return true;
	}
}

bool DmRuleMapping::DMRuleDatabaseOpen() {

	int DMRuleQuerryRet;
//	char* DMRuleErrorRet;

	/* Open database */
	DMRuleQuerryRet = sqlite3_open("DeviceManagerBackUpDB.db", &DMRuleDB);

	if (DMRuleQuerryRet) {
		syslog(LOG_DEBUG,
				"[DM]: Dm_rulemapping open /'BACKUPDADABASE/' failed: [%s]",
				sqlite3_errmsg(DMRuleDB));
		return false;
	} else {
		fprintf(stdout, "[DM]: Dm_rulemapping open /'BACKUPDADABASE/' SUCCESS");
		return true;
	}
	return false;
}

bool DmRuleMapping::DMCreateRuleTable() {

	int DMRuleQuerryRet;
	char* DMRuleErrorRet;

	std::stringstream sqlRULE;

	sqlRULE
			<< "CREATE TABLE RULES(  \
	         RULE_ID     TEXT    PRIMARY KEY    NOT NULL, \
	         RULE_TYPE          INT,\
			 TIMEFLAG 			INT,\
			 SENSOR_ID   		TEXT,\
			 ENTITY_ID 			TEXT,\
			 PROPERTY_NAME    	TEXT, \
	         PROPERTY_VALUE     TEXT, \
	         SENSOR_CONDITION   TEXT, \
			 TIME_CONDITION		TEXT, \
			 TIME_START 		TEXT,\
			 TIME_END			TEXT, \
			 IOT_ACTION				TEXT, \
			 NON_IOT_ACTION			TEXT);";

	/* Execute SQL statement */
	DMRuleQuerryRet = sqlite3_exec(DMRuleDB, (sqlRULE.str()).c_str(), callback,
			0, &DMRuleErrorRet);

	if (DMRuleQuerryRet != SQLITE_OK) {
		syslog(LOG_DEBUG, "[DM]: Dm_rulemapping to createRuleTable failed");
		sqlite3_free(DMRuleErrorRet);
		fprintf(stdout, "CreateRuleTable already exists\n");
		return false;
	} else {
		syslog(LOG_DEBUG, "[DM]: Dm_rulemapping to createRuleTable success");
		fprintf(stdout, "CreateRuleTable successfully\n");
		return true;
	}

	return false;
}

std::string DmRuleMapping::DMGetRuleTimeStart(std::string rule_id) {

	std::string ruleTimeStart;

	std::map<std::string, Rules*>::iterator DMRuleMapDataItr =
			DMRuleMapData.find(rule_id);
	if (DMRuleMapDataItr != DMRuleMapData.end()) {
		ruleTimeStart = DMRuleMapDataItr->second->get_time_start();
		return ruleTimeStart;
	} else {
		return ruleTimeStart;
	}
	return ruleTimeStart;
}

std::string DmRuleMapping::DMGetRuleTimeEnd(std::string rule_id) {
	std::string ruleTimeEnd;
	std::map<std::string, Rules*>::iterator DMRuleMapDataItr =
			DMRuleMapData.find(rule_id);
	if (DMRuleMapDataItr != DMRuleMapData.end()) {
		ruleTimeEnd = DMRuleMapDataItr->second->get_time_end();
		return ruleTimeEnd;
	} else {
		return ruleTimeEnd;
	}
	return ruleTimeEnd;
}

bool DmRuleMapping::DMSetTimeFlag(std::string rule_id, std::string timeflag) {

	std::map<std::string, Rules*>::iterator DMRuleMapDataItr =
			DMRuleMapData.find(rule_id);
	if (DMRuleMapDataItr != DMRuleMapData.end()) {
		DMRuleMapDataItr->second->set_time_flag(timeflag);
		return true;
	} else {
		return false;
	}
	return false;
}

int DmRuleMapping::DMGetRuleType(std::string rule_id) {

	int ruleType = 0;
	std::map<std::string, Rules*>::iterator DMRuleMapDataItr =
			DMRuleMapData.find(rule_id);
	if (DMRuleMapDataItr != DMRuleMapData.end()) {
		ruleType = std::stoi(DMRuleMapDataItr->second->get_rule_type());
		return ruleType;
	} else {
		return ruleType;
	}
	return ruleType;
}

bool DmRuleMapping::DMRuleCreateInDB(std::string rule_id, std::string rule_type,
		std::string timeflag, std::string sensor_id, std::string entity_id,
		std::string property_name, std::string property_value,
		std::string sensor_condition, std::string time_condition,
		std::string time_start, std::string time_end, std::string iot_action,
		std::string non_iot_action) {

	int DMRuleQuerryRet;
	char* DMRuleErrorRet;
//	const char* data = "RuleDBCallback function called";

	std::stringstream UPDATE_STRING;
	UPDATE_STRING << "INSERT INTO RULES VALUES (" << "\'" << rule_id << "\' ,"
			<< "\'" << rule_type << "\' ," << "\'" << timeflag << "\' ," << "\'"
			<< sensor_id << "\' ," << "\'" << entity_id << "\' ," << "\'"
			<< property_name << "\' ," << "\'" << property_value << "\' ,"
			<< "\'" << sensor_condition << "\' ," << "\'" << time_condition
			<< "\' ," << "\'" << time_start << "\' ," << "\'" << time_end
			<< "\' ," << "\'" << iot_action << "\' ," << "\'" << non_iot_action
			<< "\'" << ");";

	DMRuleQuerryRet = sqlite3_exec(DMRuleDB, UPDATE_STRING.str().c_str(),
			callback, 0, &DMRuleErrorRet);

	if (DMRuleQuerryRet != SQLITE_OK) {

		syslog(LOG_DEBUG,
				"[DM]: Dm_rulemapping to InsertRuleData failed: [%s] ",
				DMRuleErrorRet);

		sqlite3_free(DMRuleErrorRet);
		return false;
	} else {
		syslog(LOG_DEBUG, "[DM]: Dm_rulemapping to InsertRuleData success");
		return true;
	}
	return false;
}

bool DmRuleMapping::DMGetRuleExecuteStatus(std::string rule_id) {

	bool ruleStateFlag = false;
	/*syslog(LOG_DEBUG,
	 "[DM]: Request queue process rule execute status checking start for: [%s]",
	 rule_id.c_str())*/;

	std::map<std::string, Rules*>::iterator DMRuleMapDataItr =
			DMRuleMapData.find(rule_id);
	if (DMRuleMapDataItr != DMRuleMapData.end()) {
		syslog(LOG_DEBUG, "[DM]: Dm_rulemapping execute status found");
		ruleStateFlag = DMRuleMapDataItr->second->DMGetRuleExecuteflag();
		return ruleStateFlag;
	} else {
		syslog(LOG_DEBUG, "[DM]: Dm_rulemapping execute status not found");
		return ruleStateFlag;
	}
	return ruleStateFlag;
}

bool DmRuleMapping::DMSetRuleExecuteStatus(std::string rule_id) {

	bool ruleStateFlag = false;
	/*syslog(LOG_DEBUG,
	 "[DM]: Request queue process rule setting execute status start for: [%s]",
	 rule_id.c_str());*/

	std::map<std::string, Rules*>::iterator DMRuleMapDataItr =
			DMRuleMapData.find(rule_id);
	if (DMRuleMapDataItr != DMRuleMapData.end()) {
		syslog(LOG_DEBUG, "[DM]: Dm_rulemapping execute status found");
		DMRuleMapDataItr->second->DMSetRuleExecuteflag(false);
		ruleStateFlag = true;
		return ruleStateFlag;
	} else {
		syslog(LOG_DEBUG, "[DM]: Dm_rulemapping execute status not found");
		return ruleStateFlag;
	}
	return ruleStateFlag;
}

void DmRuleMapping::DMCallRuleCreate(int rule_type, int timeflag,
		json_object * rulesData) {

	syslog(LOG_INFO, "[DM]: Dm_rulemapping calling rule create");
	json_object * rule_id = json_object_object_get(rulesData, "rule_id");
	json_object * entity_id = json_object_object_get(rulesData, "entity_id");
	json_object * property_name = json_object_object_get(rulesData,
			"property_name");
	json_object * property_value = json_object_object_get(rulesData,
			"property_value");
	json_object * sensor_condition = json_object_object_get(rulesData,
			"sensor_condition");
	json_object * sensor_id = json_object_object_get(rulesData, "sensor_id");
	json_object * time_condition = json_object_object_get(rulesData,
			"time_condition");
	json_object * time_start = json_object_object_get(rulesData, "time_start");
	json_object * time_end = json_object_object_get(rulesData, "time_end");
	json_object * actions = json_object_object_get(rulesData, "actions");

	json_object* iot_action_ = json_object_new_object();
	json_object* non_iot_action_ = json_object_new_array();

	json_type jsontypeofactions = json_object_get_type(actions);
	size_t lengthofactions;
	/** Checking whether the json structure of actions is array or not */
	if (jsontypeofactions == json_type_array) {
		lengthofactions = json_object_array_length(actions);
	} else {
		lengthofactions = ZERO_LENGTH;
	}
	for (size_t messageSize = 0; messageSize < lengthofactions; messageSize++) {
		json_object* action_data = json_object_array_get_idx(actions,
				messageSize);

		json_object* action_type = json_object_object_get(
				json_object_array_get_idx(actions, messageSize), "action_type");
		if (strcmp(json_object_get_string(action_type), "iot") == 0) {
			iot_action_ = json_object_object_get(
					json_object_array_get_idx(actions, messageSize), "things");
		} else if (strcmp(json_object_get_string(action_type), "music") == 0) {
			json_object_array_add(non_iot_action_, action_data);
		} else if (strcmp(json_object_get_string(action_type), "news") == 0) {
			json_object_array_add(non_iot_action_, action_data);
		} else if (strcmp(json_object_get_string(action_type), "weather")
				== 0) {
			json_object_array_add(non_iot_action_, action_data);
		}
	}
	std::map<std::string, Rules*>::iterator DMRuleMapDataItr =
			DMRuleMapData.find(json_object_get_string(rule_id));
	if (DMRuleMapDataItr != DMRuleMapData.end()) {
		syslog(LOG_DEBUG, "[DM]: Dm_rulemappings creation failed");
	} else {
		Rules* RulesObj = new Rules;
		RulesObj->DMCreateRule(json_object_get_string(rule_id),
				std::to_string(rule_type), std::to_string(timeflag),
				json_object_get_string(sensor_id),
				json_object_get_string(entity_id),
				json_object_get_string(property_name),
				json_object_get_string(property_value),
				json_object_get_string(sensor_condition),
				json_object_get_string(time_condition),
				json_object_get_string(time_start),
				json_object_get_string(time_end), iot_action_, non_iot_action_);

		/*inserting json data into rule table*/
		DmRuleMapping::DMRuleCreateInDB(json_object_get_string(rule_id),
				std::to_string(rule_type), std::to_string(timeflag),
				json_object_get_string(sensor_id),
				json_object_get_string(entity_id),
				json_object_get_string(property_name),
				json_object_get_string(property_value),
				json_object_get_string(sensor_condition),
				json_object_get_string(time_condition),
				json_object_get_string(time_start),
				json_object_get_string(time_end),
				json_object_get_string(iot_action_),
				json_object_get_string(non_iot_action_));
		//delete RulesObj;
		return;
	}
}

bool DmRuleMapping::DMRuleModifyDB(std::string rule_id, std::string rule_type,
		std::string timeflag, std::string sensor_id, std::string entity_id,
		std::string property_name, std::string property_value,
		std::string sensor_condition, std::string time_condition,
		std::string time_start, std::string time_end, std::string iot_action,
		std::string non_iot_action) {

	int DMRuleQuerryRet;
	char* DMRuleErrorRet;
//	const char* data = "RuleDBCallback function called";
	std::stringstream MODIFY_RULE_STRING;
	MODIFY_RULE_STRING << "UPDATE RULES SET SENSOR_ID = " << "\'" << sensor_id
			<< "\'," << "RULE_TYPE=" << "\'" << rule_type << "\',TIMEFLAG="
			<< "\'" << timeflag << "\',ENTITY_ID=" << "\'" << entity_id
			<< "\',PROPERTY_NAME=" << "\'" << property_name
			<< "\',PROPERTY_VALUE=" << "\'" << property_value
			<< "\',SENSOR_CONDITION=" << "\'" << sensor_condition
			<< "\',TIME_CONDITION=" << "\'" << time_condition
			<< "\',TIME_START=" << "\'" << time_start << "\',TIME_END=" << "\'"
			<< time_end << "\',IOT_ACTION=" << "\'" << iot_action
			<< "\',NON_IOT_ACTION=" << "\'" << non_iot_action
			<< "\' WHERE RULE_ID =" << "\'" << rule_id << "\';";

	syslog(LOG_DEBUG, "[DM]: Dm_rulemapping UpdateRuleData: [%s]",
			(MODIFY_RULE_STRING.str()).c_str());

	DMRuleQuerryRet = sqlite3_exec(DMRuleDB, (MODIFY_RULE_STRING.str()).c_str(),
			callback, 0, &DMRuleErrorRet);
	if (DMRuleQuerryRet != SQLITE_OK) {
		syslog(LOG_DEBUG, "[DM]: Dm_rulemapping UpdateRuleData failed:[%s] ",
				DMRuleErrorRet);
		//syslog(LOG_DEBUG, "[DM]: Thing back up UpdateRuleData RULE FAIL");
		sqlite3_free(DMRuleErrorRet);
		return false;
	} else {
		syslog(LOG_DEBUG, "[DM]:Dm_rulemapping UpdateRuleData success");
		return true;
	}
	return false;
}

bool DmRuleMapping::DMCallRulesModify(int rule_type, int timeflag,
		json_object* update_rule) {

	json_object* rule_id = json_object_object_get(update_rule, "rule_id");
	json_object* sensor_condition = json_object_object_get(update_rule,
			"sensor_condition");
	json_object* sensor_id = json_object_object_get(update_rule, "sensor_id");
	json_object* entity_id = json_object_object_get(update_rule, "entity_id");
	json_object* property_value = json_object_object_get(update_rule,
			"property_value");
	json_object* property_name = json_object_object_get(update_rule,
			"property_name");
	json_object* time_condition = json_object_object_get(update_rule,
			"time_condition");
	json_object* time_start = json_object_object_get(update_rule, "time_start");
	json_object* time_end = json_object_object_get(update_rule, "time_end");
	json_object* actions = json_object_object_get(update_rule, "actions");
	json_object* iot_action_ = json_object_new_object();
	json_object* non_iot_action_ = json_object_new_array();
	json_type jsontypeofactions = json_object_get_type(actions);
	size_t lengthofactions;
	/** Checking whether the json structure of actions is array or not */
	if (jsontypeofactions == json_type_array) {
		lengthofactions = json_object_array_length(actions);
	} else {
		lengthofactions = ZERO_LENGTH;
	}

	for (size_t messageSize = 0; messageSize < lengthofactions; messageSize++) {
		json_object* action_data = json_object_array_get_idx(actions,
				messageSize);

		json_object* action_type = json_object_object_get(
				json_object_array_get_idx(actions, messageSize), "action_type");
		if (strcmp(json_object_get_string(action_type), "iot") == 0) {
			iot_action_ = json_object_object_get(
					json_object_array_get_idx(actions, messageSize), "things");
		} else if (strcmp(json_object_get_string(action_type), "music") == 0) {
			json_object_array_add(non_iot_action_, action_data);
		} else if (strcmp(json_object_get_string(action_type), "news") == 0) {
			json_object_array_add(non_iot_action_, action_data);
		} else if (strcmp(json_object_get_string(action_type), "weather")
				== 0) {
			json_object_array_add(non_iot_action_, action_data);
		}
	}

	std::map<std::string, Rules*>::iterator DMRuleMapDataItr =
			DMRuleMapData.find(json_object_get_string(rule_id));
	if (DMRuleMapDataItr != DMRuleMapData.end()) {

		DMRuleMapDataItr->second->modify_rule(json_object_get_string(rule_id),
				std::to_string(rule_type), std::to_string(timeflag),
				json_object_get_string(sensor_id),
				json_object_get_string(entity_id),
				json_object_get_string(property_name),
				json_object_get_string(property_value),
				json_object_get_string(sensor_condition),
				json_object_get_string(time_condition),
				json_object_get_string(time_start),
				json_object_get_string(time_end), iot_action_, non_iot_action_);

		bool RuleModifyStatus = DmRuleMapping::DMRuleModifyDB(
				json_object_get_string(rule_id), std::to_string(rule_type),
				std::to_string(timeflag), json_object_get_string(sensor_id),
				json_object_get_string(entity_id),
				json_object_get_string(property_name),
				json_object_get_string(property_value),
				json_object_get_string(sensor_condition),
				json_object_get_string(time_condition),
				json_object_get_string(time_start),
				json_object_get_string(time_end),
				json_object_get_string(iot_action_),
				json_object_get_string(non_iot_action_));

		if (RuleModifyStatus == true) {

			syslog(LOG_DEBUG, "[DM]:Dm_rulemapping modify success");
			return true;
		} else {
			syslog(LOG_DEBUG, "[DM]:Dm_rulemapping modify failed");
			return false;
		}
	}
	return false;
}

std::vector<std::string> DmRuleMapping::DMGetSensorCondition(
		std::string rules_id) {

	int DMRuleQuerryRet;
	char* DMRuleErrorRet;
//	const char* data = "RuleDBCallback function called";
	std::stringstream UPDATE_STRING;
	UPDATE_STRING
			<< "SELECT PROPERTY_VALUE,SENSOR_CONDITION FROM RULES WHERE RULE_ID = "
			<< "\'" << rules_id << "\'" << ";";

	DMRuleQuerryRet = sqlite3_exec(DMRuleDB, UPDATE_STRING.str().c_str(),
			callback, 0, &DMRuleErrorRet);
	if (DMRuleQuerryRet != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", DMRuleErrorRet);
		sqlite3_free(DMRuleErrorRet);
	}
	for (size_t i = 0; i < RuleVecRet.size(); i++) {
		syslog(LOG_DEBUG,
				"[DM]: Dm_rulemapping vector of all rule id associated: [%x]",
				RuleVecRet[i]);
	}
	return RuleVecRet;
}

/**
 * @REDMINE: 22201
 * */
void DmRuleMapping::CompareThingIdSensorIdMapDbAndDelete(
		std::string Input_thing_id, std::string sensor_id, std::string rule_id,
		std::string sensor_condition) {

	syslog(LOG_INFO,
			"[DM]: Dm_rulemapping to delete RULE only if /'rule_type 1,3,4/' sensor is deleted ");
	if (Input_thing_id == sensor_id) {
		if (int rule_type = DmRuleMapping::DMGetRuleType(rule_id) != DM_TIME_BASED_RULE) {
			if (sensor_condition != std::to_string(0)) {
//				DMRuleMapData.find(rule_id);
				DmRuleMapping::DMRulePrintMap();
				syslog(LOG_DEBUG,
						"[DM]: Dm_rulemapping modify with thing_id  [%s], sensor_id in /'IF_CASE/' [%s] "
								"rule_type : [%d] and sensor_condition [%s]",
						Input_thing_id.c_str(), sensor_id.c_str(), rule_type,
						sensor_condition.c_str());
				int ruleDeleteStatus = DmRuleMapping::DMCallRuleDelete(rule_id);
//				syslog(LOG_DEBUG,
//						"Dm_rulemapping @@@@@@@@@@@SOMSOMSOMSOM$$$$$$$$$$$$$$[%d]",
//						ruleDeleteStatus);
			}
		} else {
			rule_type = DM_TIME_BASED_RULE;
		}
	}
}

std::string DmRuleMapping::CompareThingIdFromRuleMap_AndDelete(
		json_object* things, std::string Input_thing_id) {

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
			syslog(LOG_DEBUG, "[DM]: Dm_rulemapping modify with thing_id: [%s]",
					Input_thing_id.c_str());
		} else {
			json_object_array_add(UpdateJson, TotalData);
			syslog(LOG_DEBUG, "[DM]: Dm_rulemapping modify with thing_id: [%s]",
					Input_thing_id.c_str());
		}
	}
	////json_object_put(UpdateJson);
	return json_object_get_string(UpdateJson);
}

void DmRuleMapping::DeleteThingIDFromRule(std::string thing_id) {

	for (std::map<std::string, Rules*>::iterator DMRuleMapDataItr =
			DMRuleMapData.begin(); DMRuleMapDataItr != DMRuleMapData.end();
			++DMRuleMapDataItr) {
		std::string IOT_Data =
				DmRuleMapping::CompareThingIdFromRuleMap_AndDelete(
						DMRuleMapDataItr->second->get_iot_action(), thing_id);
		DMRuleMapDataItr->second->set_iot_action(
				json_tokener_parse(IOT_Data.c_str()));

		DmRuleMapping::DMRuleModifyDB(DMRuleMapDataItr->second->get_rule_id(),
				DMRuleMapDataItr->second->get_rule_type(),
				DMRuleMapDataItr->second->get_time_flag(),
				DMRuleMapDataItr->second->get_sensor_id(),
				DMRuleMapDataItr->second->get_entity_id(),
				DMRuleMapDataItr->second->get_property_name(),
				DMRuleMapDataItr->second->get_property_value(),
				DMRuleMapDataItr->second->get_sensor_condition(),
				DMRuleMapDataItr->second->get_time_condition(),
				DMRuleMapDataItr->second->get_time_start(),
				DMRuleMapDataItr->second->get_time_end(),
				json_object_get_string(
						DMRuleMapDataItr->second->get_iot_action()),
				json_object_get_string(
						DMRuleMapDataItr->second->get_non_iot_action()));
		DmRuleMapping::CompareThingIdSensorIdMapDbAndDelete(thing_id,
				DMRuleMapDataItr->second->get_sensor_id(),
				DMRuleMapDataItr->second->get_rule_id(),
				DMRuleMapDataItr->second->get_sensor_condition());
//		syslog(LOG_INFO,
//				"Dm_rulemapping @@@@@@@@@@@SOMSOMSOMSOM$$$$$$$$$$$$$$===2");
	}
}

std::vector<std::string> DmRuleMapping::DMGetRuleId(std::string obs_sensor_id,
		std::string obs_entity_id, std::string obs_property_name,
		std::string obs_property_value) {

	/** Vector to store all rule_id associated with obs_sensor_id, obs_entity_id, obs_property_name */
	std::vector<std::string> VectorRuleIds;
	int cur_pro_val = std::stoi(obs_property_value);
	/** Iterating global gRules map */
	for (std::map<std::string, Rules*>::iterator DMRuleMapDataItr =
			DMRuleMapData.begin(); DMRuleMapDataItr != DMRuleMapData.end();
			++DMRuleMapDataItr) {

		syslog(LOG_DEBUG, " [DM]:Dm_rulemapping having Rule_id : [%s]",
				DMRuleMapDataItr->first.c_str());
		/** Checking for Sensor Id present in Map */
		if (DMRuleMapDataItr->second->get_sensor_id() == obs_sensor_id) {
			/** Checking for Sensor entity id present in map */
			if (DMRuleMapDataItr->second->get_entity_id() == obs_entity_id) {
				/** Checking for property name present in Map */
				if (DMRuleMapDataItr->second->get_property_name()
						== obs_property_name) {
					std::string sensor_condition =
							DMRuleMapDataItr->second->get_sensor_condition();
					int pro_val;
					try {
						pro_val = std::stoi(
								DMRuleMapDataItr->second->get_property_value());
					} catch (...) {
						syslog(LOG_CRIT, "[Dm]:Dm_rulemapping stoi error");
						return VectorRuleIds;
					}

					switch ((char) (sensor_condition.at(0))) {

					case '<': {
						if (cur_pro_val < pro_val) {
							syslog(LOG_DEBUG,
									"[DM]: Dm_rulemapping rule found for condition(<) : [%s] : [%s] : [%s]",
									obs_sensor_id.c_str(),
									obs_entity_id.c_str(),
									obs_property_name.c_str());
							VectorRuleIds.push_back(DMRuleMapDataItr->first); /** Pushing rule id in vector as all conditions are statisfied */
						} else
							syslog(LOG_DEBUG,
									"[DM]: Dm_rulemapping rule not found for condition(<): [%d], rule_val: [%d]",
									cur_pro_val, pro_val);
					}
						break;

					case '=': {
						if (cur_pro_val == pro_val) {
							syslog(LOG_DEBUG,
									"[DM]: Dm_rulemapping rule found for condition(=) : [%s] : [%s] : [%s]",
									obs_sensor_id.c_str(),
									obs_entity_id.c_str(),
									obs_property_name.c_str());

							VectorRuleIds.push_back(DMRuleMapDataItr->first); /** Pushing rule id in vector as all conditions are statisfied */
						} else
							syslog(LOG_DEBUG,
									"[DM]Dm_rulemapping rule not found for condition: [%d], rule_val: [%d]",
									cur_pro_val, pro_val);
					}
						break;

					case '>': {
						if (cur_pro_val > pro_val) {
							syslog(LOG_DEBUG,
									"[DM]:Dm_rulemapping rule found for condition(>) : [%s] : [%s] : [%s]",
									obs_sensor_id.c_str(),
									obs_entity_id.c_str(),
									obs_property_name.c_str());

							VectorRuleIds.push_back(DMRuleMapDataItr->first); /** Pushing rule id in vector as all conditions are statisfied */
						} else
							syslog(LOG_DEBUG,
									"[DM]: Dm_rulemapping rule found for condition: [%d], rule_val: [%d]",
									cur_pro_val, pro_val);
					}
						break;

					default:
						syslog(LOG_DEBUG,
								"[DM]:  Dm_rulemapping DMGetRuleId default");
						break;
					}
//				else {
//					syslog(LOG_DEBUG,
//							"[DM]: No Rule is associated with this sensor's property_value");
//				}
				} else {
					syslog(LOG_DEBUG,
							"[DM]: Dm_rulemapping no rule is associated with this sensor's property name");
				}
			} else {
				syslog(LOG_DEBUG,
						"[DM]:  Dm_rulemapping no rule is associated with this sensor's entity id");
			}
		} else {
			syslog(LOG_DEBUG,
					"[DM]:  Dm_rulemapping no rule is associated with this sensor");
		}
	}

	return VectorRuleIds;
}

SecneRuleExecution DmRuleMapping::DMCallRuleExecuteObs(std::string sensor_id,
		std::string entity_id, std::string property_name,
		std::string property_value) {

	SecneRuleExecution RuleExecutionObj;
	std::string rule_id;
	bool RuleExecuteStatus;

	std::vector<std::string> retRule_ids = DmRuleMapping::DMGetRuleId(sensor_id,
			entity_id, property_name, property_value);
	if (retRule_ids.size() != 0) {
		for (unsigned int rule_itr = 0; rule_itr < retRule_ids.size();
				rule_itr++) {

			std::map<std::string, Rules*>::iterator DMRuleMapDataItr =
					DMRuleMapData.find(retRule_ids[rule_itr]);
//			int cur_pro_val = std::stoi(
//					DMRuleMapDataItr->second->get_property_value());
			if (DMRuleMapDataItr != DMRuleMapData.end()) {
				/*syslog(LOG_DEBUG,
				 "[DM]: Rule mapping RESPONSE PRINT RULE_ID : [%s] ",
				 retRule_ids[rule_itr].c_str());*/
				RuleExecuteStatus = DmRuleMapping::DMGetRuleExecuteStatus(
						retRule_ids[rule_itr]);
				RuleExecutionObj.RuleSceneID = DMRuleMapDataItr->first;
				RuleExecutionObj.RuleSceneExecuteFlag = RuleExecuteStatus;
				if (RuleExecuteStatus == false) {

					syslog(LOG_DEBUG,
							"[DM]:  Dm_rulemapping executing for Rule: [%s]",
							DMRuleMapDataItr->first.c_str());

					RuleExecutionObj.RuleSceneID = DMRuleMapDataItr->first;

					syslog(LOG_DEBUG,
							"[DM]: Dm_rulemapping executing for RuleSceneID: [%s]",
							RuleExecutionObj.RuleSceneID.c_str());
					char sensor_condition =
							DMRuleMapDataItr->second->get_sensor_condition().at(
									0);
					switch (std::stoi(DMRuleMapDataItr->second->get_rule_type())) {

					case DM_SENSOR_BASED_RULE:

						syslog(LOG_DEBUG,
								"[DM]: Dm_rulemapping executing for Rule PROPERTY_VALUE : [%s]"
										" RUN PROPERTY VALUE : [%s]",
								DMRuleMapDataItr->second->get_property_value().c_str(),
								property_value.c_str());

						switch (sensor_condition) {

						case ('='): {
							rule_id = DMRuleMapDataItr->first;
							RuleExecutionObj =
									DMRuleMapDataItr->second->execute_rule();
							return RuleExecutionObj;
						}
							break;
						case ('<'):
							rule_id = DMRuleMapDataItr->first;
							RuleExecutionObj =
									DMRuleMapDataItr->second->execute_rule();
							return RuleExecutionObj;
							break;
						case ('>'):
							rule_id = DMRuleMapDataItr->first;
							RuleExecutionObj =
									DMRuleMapDataItr->second->execute_rule();
							return RuleExecutionObj;
							break;
						default:
							break;
						}
						break;

					case DM_SENSOR_TIME_BASED_RULE_END:
						/*
						 * Setting the time flag based on the current time,
						 * start time and end time of the Rule, when Cron job is triggered
						 * i.e if current time is in between start and end time then
						 * time flag is 1(allowing Sensor response and triggering rule) ,
						 * otherwise 0*/

						std::string time_start =
								DMRuleMapDataItr->second->get_time_start(); /* Getting the time start string from Rule Map Object and converting to integer by (hr*100 + min) */

						std::string Hr_start = time_start.substr(0, 2);
						std::string Min_start = time_start.substr(3, 2);

						int total_time_start = (std::stoi(Hr_start) * 100)
								+ std::stoi(Min_start);

						std::string time_end =
								DMRuleMapDataItr->second->get_time_end();/* Getting the time end string from Rule Map Object  and converting to integer by (hr*100 + min) */

						std::string Hr_end = time_end.substr(0, 2);
						std::string Min_end = time_end.substr(3, 2);

						int total_time_end = (std::stoi(Hr_end) * 100)
								+ std::stoi(Min_end);

						time_t theTime = time(NULL);
						struct tm *cTime = localtime(&theTime);

						int hour = cTime->tm_hour;
						int min = cTime->tm_min;

						int current_time = (hour * 100) + min;

						/* Setting the time flag based on the above mentioned logic */
						//REDMINE 22328 start: Accounting for the first minute in time and sensor based routine
						if (current_time <= total_time_end) {
							if (current_time >= total_time_start) {
								DMRuleMapDataItr->second->set_time_flag("1");
							} else if (total_time_start >= total_time_end) {
								DMRuleMapDataItr->second->set_time_flag("1");
							}
						//REDMINE 22328 end: Accounting for the first minute in time and sensor based routine
						} else if (current_time < total_time_start) {
							DMRuleMapDataItr->second->set_time_flag("0");
						} else {
							DMRuleMapDataItr->second->set_time_flag("0");
						}
						/** Checking Time activation flag from object */
						if (std::stoi(DMRuleMapDataItr->second->get_time_flag())
								== 1) {
							syslog(LOG_DEBUG,
									"[DM]: Dm_rulemapping active time status flag is true");
							switch (sensor_condition) {
							case ('='):
								rule_id = DMRuleMapDataItr->first;
								RuleExecutionObj =
										DMRuleMapDataItr->second->execute_rule();
								return RuleExecutionObj;
								break;
							case ('<'):
								rule_id = DMRuleMapDataItr->first;
								RuleExecutionObj =
										DMRuleMapDataItr->second->execute_rule();
								return RuleExecutionObj;
								break;
							case ('>'):
								rule_id = DMRuleMapDataItr->first;
								RuleExecutionObj =
										DMRuleMapDataItr->second->execute_rule();
								return RuleExecutionObj;
								break;
							default:
								break;
							}

						} else {
							syslog(LOG_DEBUG,
									"[DM]: Dm_rulemapping active time status flag is false");
							RuleExecutionObj.Sceneflag = 0; /* Setting Rule scene flag to zero after time end*/
						}
						break;
					default:
						break;
					}
				} else {

					syslog(LOG_DEBUG,
							"[DM]: Dm_rulemapping FAILED++++++++++++++++++++++++++++++++++++++++++++++");
					RuleExecutionObj.Sceneflag = 4;
					RuleExecutionObj.RuleSceneExecuteFlag = false;
					return RuleExecutionObj;
				}
			} else {
				syslog(LOG_DEBUG,
						"[DM]: Dm_rulemapping no Rules are associated with above condition");
			}
		}
	} else {
		syslog(LOG_DEBUG,
				"[DM]: Dm_rulemapping no Rules are associated with above condition");
	}
	retRule_ids.shrink_to_fit();
	return RuleExecutionObj;
}

SecneRuleExecution DmRuleMapping::DMCallRuleExecute(std::string rule_id) {

	SecneRuleExecution RuleExecutionRsultOBJ;
	bool RuleExecuteStatus = false;
	RuleExecuteStatus = DmRuleMapping::DMGetRuleExecuteStatus(rule_id);
	RuleExecutionRsultOBJ.RuleSceneExecuteFlag = RuleExecuteStatus;
	RuleExecutionRsultOBJ.RuleSceneID = rule_id;
	if (RuleExecuteStatus == false) {

		std::map<std::string, Rules*>::iterator DMRuleMapDataItr =
				DMRuleMapData.find(rule_id);
		if (DMRuleMapDataItr != DMRuleMapData.end()) {

			syslog(LOG_DEBUG,
					"Dm_rulemapping getting the type of the rule:[%s]",
					DMRuleMapDataItr->second->get_rule_type().c_str());

			switch (std::stoi(DMRuleMapDataItr->second->get_rule_type())) {

			case DM_TIME_BASED_RULE: {
				RuleExecutionRsultOBJ =
						DMRuleMapDataItr->second->execute_rule();
				return RuleExecutionRsultOBJ;
			}
				break;
			case DM_SENSOR_TIME_BASED_RULE_START: {

				DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

				std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
						DmThingMapObj->DmThingDataMap.find(
								DMRuleMapDataItr->second->get_sensor_id());
				if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {

					for (int EntityNo = 0;
							EntityNo
									< DmThingMapObjItr->second->getEntityListSize();
							EntityNo++) {

						if (DMRuleMapDataItr->second->get_entity_id()
								== DmThingMapObjItr->second->getEntityInfo(
										EntityNo)->getentity_id()) {

							for (int PropertyNo = 0;
									PropertyNo
											< DmThingMapObjItr->second->getEntityInfo(
													EntityNo)->getPropertySize();
									PropertyNo++) {

								if (DMRuleMapDataItr->second->get_property_name()
										== DmThingMapObjItr->second->getEntityInfo(
												EntityNo)->getPropertyInfo(
												PropertyNo)->getproperty_name()) {

									switch (DMRuleMapDataItr->second->get_sensor_condition().at(
											0)) {

									case ('='):
										if (DmThingMapObjItr->second->getEntityInfo(
												EntityNo)->getPropertyInfo(
												PropertyNo)->getproperty_value()
												== DMRuleMapDataItr->second->get_property_value()) {
											RuleExecutionRsultOBJ =
													DMRuleMapDataItr->second->execute_rule();
											return RuleExecutionRsultOBJ;
										}
										break;

									case ('>'):
										if (DmThingMapObjItr->second->getEntityInfo(
												EntityNo)->getPropertyInfo(
												PropertyNo)->getproperty_value()
												> DMRuleMapDataItr->second->get_property_value()) {
											RuleExecutionRsultOBJ =
													DMRuleMapDataItr->second->execute_rule();
											return RuleExecutionRsultOBJ;
										}
										break;

									case ('<'):
										if (DmThingMapObjItr->second->getEntityInfo(
												EntityNo)->getPropertyInfo(
												PropertyNo)->getproperty_value()
												< DMRuleMapDataItr->second->get_property_value()) {
											RuleExecutionRsultOBJ =
													DMRuleMapDataItr->second->execute_rule();
											return RuleExecutionRsultOBJ;
										}
										break;
									}
								} else {
								}
							}
						} else {
							syslog(LOG_ERR,
									"[DM]: Dm_rulemapping for sensor entity_id not found in map: [%s]",
									DMRuleMapDataItr->second->get_entity_id().c_str());
						}
					}

				} else {

				}
			}
				break;

//			case DM_SENSOR_TIME_BASED_RULE_END: {
//				/*
//				 * Setting the time flag based on the current time,
//				 * start time and end time of the Rule, when Cron job is triggered
//				 * i.e if current time is in between start and end time then
//				 * time flag is 1(allowing Sensor response and triggering rule) ,
//				 * otherwise 0*/
//				std::string time_start =
//						DMRuleMapDataItr->second->get_time_start(); /* Getting the time start string from Rule Map Object and converting to integer by (hr*100 + min) */
//
//				std::string Hr_start = time_start.substr(0, 2);
//				std::string Min_start = time_start.substr(3, 2);
//				int total_time_start = (std::stoi(Hr_start) * 100)
//						+ std::stoi(Min_start);
//
//				std::string time_end = DMRuleMapDataItr->second->get_time_end();/* Getting the time end string from Rule Map Object  and converting to integer by (hr*100 + min) */
//
//				std::string Hr_end = time_end.substr(0, 2);
//				std::string Min_end = time_end.substr(3, 2);
//
//				int total_time_end = (std::stoi(Hr_end) * 100)
//						+ std::stoi(Min_end);
//
//				time_t theTime = time(NULL);
//   			        struct tm *cTime = localtime(&theTime);
//
//				int hour = cTime->tm_hour;
//				int min = cTime->tm_min;
//
//				int current_time = (hour * 100) + min;
//
//				/* Setting the time flag based on the above mentioned logic */
//				if (current_time < total_time_end) {
//					if (current_time >= total_time_start) {
//						DMRuleMapDataItr->second->set_time_flag("1");
//					} else if (total_time_start > total_time_end) {
//						DMRuleMapDataItr->second->set_time_flag("1");
//					}
//				} else {
//					DMRuleMapDataItr->second->set_time_flag("0");
//				}
//				return RuleExecutionRsultOBJ;
//			}
//				break;
			default: {

			}
				break;
			}
		}
	}
	return RuleExecutionRsultOBJ;
}

bool DmRuleMapping::DMCallRuleDelete(std::string rule_id) {

	DmRuleMapping::DMRuleDeleteDB(rule_id);
	std::map<std::string, Rules*>::iterator DMRuleMapDataItr =
			DMRuleMapData.find(rule_id);
	if (DMRuleMapDataItr != DMRuleMapData.end()) {
		DMRuleMapData.erase(rule_id);
		syslog(LOG_DEBUG, "[DM]: Dm_rulemapping rule delete from map success");
		return true;
	} else {
		syslog(LOG_DEBUG, "[DM]: Dm_rulemapping rule delete from map failed");
		return false;
	}
	return false;
}

bool DmRuleMapping::DMRuleDeleteDB(std::string rule_id) {

	int DMRuleQuerryRet;
	char* DMRuleErrorRet;
	std::stringstream UPDATE_STRING;
	UPDATE_STRING << "DELETE FROM RULES WHERE RULE_ID =" << "\'" << rule_id
			<< "\'" << ";";

	DMRuleQuerryRet = sqlite3_exec(DMRuleDB, (UPDATE_STRING.str()).c_str(),
			callback, 0, &DMRuleErrorRet);

	if (DMRuleQuerryRet != SQLITE_OK) {
		syslog(LOG_DEBUG,
				"[DM]: Dm_rulemapping rule delete from database fail");
		sqlite3_free(DMRuleErrorRet);
		return false;
	} else {
		syslog(LOG_DEBUG,
				"[DM]: Dm_rulemapping rule delete from database success");
		return true;
	}
	return false;
}

bool DmRuleMapping::DMRecoverMapFromRuleDB() {

	syslog(LOG_INFO, "[DM]: Dm_rulemapping recover map from database start");
	int DMRuleQuerryRet;
	char* DMRuleErrorRet;
	std::stringstream UPDATE_STRING;
	DmRuleMapping::DMRulePrintDB();
	UPDATE_STRING << "SELECT * FROM RULES";

	DMRuleQuerryRet = sqlite3_exec(DMRuleDB, (UPDATE_STRING.str()).c_str(),
			callbackRuleDBtoMap, 0, &DMRuleErrorRet);
	if (DMRuleQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR, "[DM]: Dm_rulemapping SQL error: %s\n", DMRuleErrorRet);
		sqlite3_free(DMRuleErrorRet);
		syslog(LOG_DEBUG,
				"[DM]: Dm_rulemapping recover map from database failed");
		return false;
	} else {
		syslog(LOG_DEBUG,
				"[DM]: TDm_rulemapping recover map from database success");
		return true;
	}
	return false;
}

void DmRuleMapping::DMRulePrintMap() {

	for (std::map<std::string, Rules*>::iterator RulesMapDataItr =
			DMRuleMapData.begin(); RulesMapDataItr != DMRuleMapData.end();
			++RulesMapDataItr) {
		RulesMapDataItr->second->RuleDisplay();
	}
}

void DmRuleMapping::DMRulePrintDB() {

	int DMSceneQuerryRet;
	char* DMSceneErrorRet;
	std::stringstream UPDATE_STRING;
	UPDATE_STRING << "SELECT * from RULES ;";

	DMSceneQuerryRet = sqlite3_exec(DMRuleDB, (UPDATE_STRING.str()).c_str(),
			callback, 0, &DMSceneErrorRet);
	if (DMSceneQuerryRet != SQLITE_OK) {
		syslog(LOG_INFO, "[DM]: Dm_rulemapping printing rule database fail");
		sqlite3_free(DMSceneErrorRet);
	} else {
		syslog(LOG_INFO, "[DM]: Dm_rulemapping pinting rule database success");
	}
}

