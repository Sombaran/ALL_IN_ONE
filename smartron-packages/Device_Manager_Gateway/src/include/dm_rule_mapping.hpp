/*
 * File_name:dm_rule_mapping.hpp
 * @Brief: The file rule class and the inheritance the same for mapping it
 * Created on: 23-Aug-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef DM_RULE_MAPPING_HPP_
#define DM_RULE_MAPPING_HPP_

#include <iostream>
#include <map>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sqlite3.h>
#include <sys/syslog.h>
#include <json-c/json.h>
#include <string.h>
#include "dm_utility.hpp"

class Rules {
private:

	std::string rule_id;						//!< Unique identifier for Rule
	std::string rule_type;						//!< Category of rule of which class object belongs
	std::string timeflag;						//!< Status of time which tell whether Rule is active for particular Duration
	std::string sensor_id;						//!< Identifier of sensor on which Rule actions are associated
	std::string entity_id;						//!< Identifier for entity of sensor on which Rule actions are associated
	std::string property_name;					//!< Property name of sensor on which Rule actions are associated
	std::string property_value;					//!< Property Value of sensors property name on which Rule actions are associated
	std::string sensor_condition;				//!< Condition for sensor property value whether =, <, >
	std::string time_condition;					//!< Time condition if Rule belongs to TIME, SENSOR_TIME_START, SENSOR_TIME_START_END
	std::string time_start;						//!< Time when Rule is active
	std::string time_end;						//!< Time when Rule is in Deactive state
	json_object *iot_action;					//!< IoT actions associated with Rule
	json_object *non_iot_action;				//!< Non-IoT actions associated with Rule
	int no_of_things;
	bool RuleExecuteflag;

public:

	/**
	 * @Brief:Function to get rule ID of selected Rule Object
	 * @return:string of Rule Id
	 */
	std::string get_rule_id();

	/**
	 * @Brief:Function to set rule ID for Rule
	 * @param:rule_id
	 */
	void set_rule_id(std::string rule_id);

	/**
	 * @Brief:Function to get rule type of selected Rule Object
	 * @return:string of get rule type
	 */
	std::string get_rule_type();
	/**
	 * @Brief:Function to set get rule type for Rule
	 * @param:rule_type
	 */
	void set_rule_type(std::string rule_type);
	/**
	 * @Brief:Function to get time flag of selected Rule Object
	 * @return:string of get time flag
	 */
	std::string get_time_flag();
	/**
	 * @Brief:Function to set time flag for Rule
	 * @param:time flag
	 */
	void set_time_flag(std::string active_time_status);
	/**
	 * @Brief:Function to get sensor id of selected Rule Object
	 * @return:string of get sensor id
	 */
	std::string get_sensor_id();
	/**
	 * @Brief:Function to set sensor id for Rule
	 * @param: sensor id
	 */
	void set_sensor_id(std::string rule_id);
	/**
	 * @Brief:Function to get entity id of selected Rule Object
	 * @return:string of get entity id
	 */
	std::string get_entity_id();
	/**
	 * @Brief:Function to set entity id for Rule
	 * @param:entity id
	 */
	void set_entity_id(std::string rule_id);
	/**
	 * @Brief:Function to get property name of selected Rule Object
	 * @return:string of get property name
	 */
	std::string get_property_name();
	/**
	 * @Brief:Function to set property name for Rule
	 * @param:property_name
	 */
	void set_property_name(std::string property_name);
	/**
	 * @Brief:Function to get property value of selected Rule Object
	 * @return:string of get property value
	 */
	std::string get_property_value();
	/**
	 * @Brief:Function to set property value for Rule
	 * @param:property_value
	 */
	void set_property_value(std::string property_value);
	/**
	 * @Brief:Function to get sensor condition of selected Rule Object
	 * @return:string of get sensor condition
	 */
	std::string get_sensor_condition();
	/**
	 * @Brief:Function to set sensor condition for Rule
	 * @param:sensor_condition
	 */
	void set_sensor_condition(std::string sensor_condition);
	/**
	 * @Brief:Function to get time condition of selected Rule Object
	 * @return:string object of get time condition
	 */
	std::string get_time_condition();
	/**
	 * @Brief:Function to set time condition for Rule
	 * @param:time_condition
	 */
	void set_time_condition(std::string time_condition);
	/**
	 * @Brief:Function to get time start of selected Rule Object
	 * @return:string object of get time condition
	 */
	std::string get_time_start();
	/**
	 * @Brief:Function to set time start for Rule
	 * @param:time_start
	 */
	void set_time_start(std::string time_start);
	/**
	 * @Brief:Function to get time end of selected Rule Object
	 * @return:string object of get time end
	 */
	std::string get_time_end();
	/**
	 * @Brief:Function to set time end for Rule
	 * @param:time_end
	 */
	void set_time_end(std::string time_end);
	/**
	 * @Brief:Function to get iot action of selected Rule Object
	 * @return:json object of get iot action
	 */
	json_object *get_iot_action();
	/**
	 * @Brief:Function to set iot action for Rule
	 * @param:iot_action
	 */
	void set_iot_action(json_object *iot_action);
	/**
	 * @Brief:Function to get music action of selected Rule Object
	 * @return:json object of get music action
	 */
	json_object *get_non_iot_action();
	/**
	 * @Brief:Function to set music action for Rule
	 * @param:music/weather/news actions
	 */
	void set_non_iot_action(json_object *non_iot_action);

	/**
	 * @Brief:Execute the Rule Based on condition Occurred
	 * @return:stucture of rule/scene
	 */
	SecneRuleExecution execute_rule();

	/**
	 * @Brief:Modifies the Rule when particular Rule Object is called through Mobile App
	 * @param:active_time_status flag for Activation and De-activation for Rules base on time conditions
	 * @param:modified_jObj Modified Json Data to be added in Rules object and Database
	 * @return:status of modify rule on Success/failures
	 */
	bool modify_rule(std::string, std::string, std::string, std::string,
			std::string, std::string, std::string, std::string, std::string,
			std::string, std::string, json_object*, json_object*);
	/**
	 * @Brief:Function to create rule
	 * @param:rule_id,rule_type,timeflag,sensor_id,entity_id,property_name,
	 * @param:property_value,sensor_condition,time_condition,time_start
	 * @param:time_end, iot_action,non_iot_action
	 * @return:status of success/ failure
	 * */
	bool DMCreateRule(std::string, std::string, std::string, std::string,
			std::string, std::string, std::string, std::string, std::string,
			std::string, std::string, json_object*, json_object*);
	/**
	 * @Brief:Funciton to get rule executing flag value
	 * @return:status of success/ failure
	 * */
	bool DMGetRuleExecuteflag();
	/**
	 * @Brief:Funciton to set rule executing flag value
	 * @param: RuleExecuteflag
	 * */
	void DMSetRuleExecuteflag(bool);
	/**
	 * @Brief: Fucntion to display rule data attibutes
	 * */
	void RuleDisplay();
	/**
	 * @Brief: Vitual Destructor used to frer the allocated resourses
	 * */
	virtual ~ Rules() {
	}
};

/**
 * @Brief: Inheritance is public, base class Rules and derived class DmRuleMapping
 * */

class DmRuleMapping: public Rules {

public:

	bool ruleNewState;
	/**
	 * @Brief:Function to initilaize the rule database by opening and creatinf table
	 * @return:status of success/ failure
	 * */
	static bool DMRuleDatabaseInit();
	/**
	 * @Brief:Function to open the rule table
	 * @return:status of success/ failure
	 * */
	static bool DMRuleDatabaseOpen();
	/**
	 * @Brief:Function to close the rule table
	 * @return:status of success/ failure
	 * */
	static bool DMRuleDatabaseClose();
	/**
	 * @Brief:Function to create the rule table
	 * @return:status of success/ failure
	 * */
	static bool DMCreateRuleTable();
	/**
	 * @Brief:Function to get all rule ids based on below conditions in arguments
	 * @param:obs_sensor_id sensor id from observe request
	 * @param:obs_entity_id entity id from observe request
	 * @param:obs_property_name property name from observe request
	 * @return:vector of all rule id associated with above condition
	 */
	static std::vector<std::string> DMGetRuleId(std::string obs_sensor_id,
			std::string obs_entity_id, std::string obs_property_name,
			std::string);
	/**
	 * @Breif:Function to delete Rule from Rule map and Database
	 * @param:rule_id rule id to be deleted
	 */
	void rule_delete(std::string rule_id);
	/**
	 * @Brief:parsing Json data into database
	 * Inserting JSON data into RULES Database,
	 * Getting from TronX using Rules List API
	 * @param:rule_type, timeflag,ruleData
	 * */
	static void DMCallRuleCreate(int, int, json_object*);
	/**
	 * @Brief:Function to create copy in database
	 * @param:rule_id,rule_type,timeflag,sensor_id,entity_id,property_name,
	 * @param:property_value,sensor_condition,time_condition,time_start
	 * @param:time_end, iot_action,non_iot_action
	 * @return:status of success/ failure
	 */
	static bool DMRuleCreateInDB(std::string, std::string, std::string,
			std::string, std::string, std::string, std::string, std::string,
			std::string, std::string, std::string, std::string, std::string);
	/**
	 * @Brief: Function to modify the rule created
	 * @param:rule_type,timeflag,update_rule
	 * @return:status of success/ failure
	 * */
	static bool DMCallRulesModify(int, int, json_object*);
	/**
	 * @Brief:Function to modify rule data in database
	 * @param:rule_id,rule_type,timeflag,sensor_id,entity_id,property_name,
	 * @param:property_value,sensor_condition,time_condition,time_start
	 * @param:time_end, iot_action,non_iot_action
	 * @return:status of success/ failure
	 */
	static bool DMRuleModifyDB(std::string, std::string, std::string,
			std::string, std::string, std::string, std::string, std::string,
			std::string, std::string, std::string, std::string, std::string);
	/**
	 * @Brief:Funciton to get sensor conditon into a vector
	 * @param:rules_id
	 * @return:vector of all rule id associated with above condition
	 * */
	static std::vector<std::string> DMGetSensorCondition(std::string);
	/**
	 * @Brief:Funciton to drive the execution of rule_type /'1&4 /'
	 * @param:sensor_id,entity_id,property_name,property_value
	 * @return:stucture of rule/scene
	 * */
	static SecneRuleExecution DMCallRuleExecuteObs(std::string, std::string,
			std::string, std::string);
	/**
	 * @Brief:Funciton to drive the execution of rule_type /'2&3 /'
	 * @param:rule_id
	 * @return:stucture of rule/scene
	 * */
	static SecneRuleExecution DMCallRuleExecute(std::string);
	/**
	 * @Brief:Fucntion to compare thing and thingId in rule map
	 * @param:things,thing_id
	 * @return: string array of things
	 * */
	static std::string CompareThingIdFromRuleMap_AndDelete(json_object*,
			std::string);
	/**
	 * @Brief:Fucntion to compare thing and thingId in rule map
	 * @param:things,thing_id
	 * @return: string array of things
	 * */
	static void CompareThingIdSensorIdMapDbAndDelete(std::string,
			std::string, std::string, std::string);
	/**
	 * @Brief: Function to alter /delete thing_ID after rule modification
	 * @param:thing_id
	 * */
	static void DeleteThingIDFromRule(std::string);
	/**
	 * @Brief:Funtion to call delete the rule_ID from map/Database
	 * @param:rule_id
	 * @return:status of success/ failure
	 * */
	static bool DMCallRuleDelete(std::string);
	/**
	 * @Brief: Fucntion to delete rule from database
	 * @param:rule_id
	 * @return:status of success/ failure
	 * */
	static bool DMRuleDeleteDB(std::string);
	/**
	 * @Brief: Function to get the rule_type /'1,2,3 & 4/'
	 * @param:rule_id
	 * @return: type of rule
	 * 1: sensor based
	 * 2: time based
	 * 3: time &sensor based(time_start)
	 * 4: time & sensor(time_start, time_end)
	 * */
	static int DMGetRuleType(std::string);
	/**
	 * @Brief: Function to get the time start value for rule_type 4
	 * @param:rule_id
	 * @return: Value of time start
	 * */
	static std::string DMGetRuleTimeStart(std::string);
	/**
	 * @Brief: Function to get the time end value for rule_type 4
	 * @param:rule_id
	 * @return: Value of time end
	 * */
	static std::string DMGetRuleTimeEnd(std::string);
	/**
	 * @Breif: Function to set the time flag value
	 * @param:rule_id, time flag
	 * status of success/ failure
	 * */
	static bool DMSetTimeFlag(std::string, std::string);
	/**
	 * @Brief:Function to recover the map from copy database
	 * @return:status of success/ failure
	 * */
	static bool DMRecoverMapFromRuleDB();
	/**
	 * @Breif: Fucntion to get the rule execution status for a particular rule
	 * @param:rule_id
	 * @return:status of success/ failure
	 * */
	static bool DMGetRuleExecuteStatus(std::string);
	/**
	 * @Breif: Fucntion to set the rule execution status for a particular rule
	 * @param:rule_id
	 * @return:status of success/ failure
	 **/
	static bool DMSetRuleExecuteStatus(std::string);
	/**
	 * @Brief: Function to print the rule map via /'RuleDisplay/' function
	 * */
	static void DMRulePrintMap();
	/**
	 * @Brief: Function to print the rule table via /'RuleDisplay/' function
	 * */
	static void DMRulePrintDB();
	/**
	 * @Brief:estructor used to frer the allocated resourses
	 * */
	~DmRuleMapping() {
	}
};

#endif /* DM_RULE_MAPPING_HPP_ */
