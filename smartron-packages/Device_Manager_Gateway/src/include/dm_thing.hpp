/*
 * File_name:dm_thing.hpp
 * @Brief: Driver class to get/set entity/ properrty and things details
 * Created on: 26-Jul-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef INCLUDE_DM_THING_HPP_
#define INCLUDE_DM_THING_HPP_

#include <iostream>
#include <map>
#include <vector>
#include <sys/syslog.h>
#include "dm_framework.hpp"
#include <json-c/json.h>
#include <string>
#include <thread>

/**
 * @brief 	Property class will give device property information which are able to control.
 */
class DmProperty {
private:
	std::string property_type; //!< property type (SWITCH, RANGE, COLOR, LIST, INFO etc.)
	std::string property_name;			//!< name of property to be controlled.
	std::string property_range;							//!< limits of property
	std::string property_value;

public:

	/**
	 * Constructor for property
	 */
	DmProperty();

	/**
	 * Function to display property info in entity
	 */
	void Display();

	/**
	 * Function to get property type
	 * @return property type
	 */
	std::string getproperty_type();

	/**
	 * Function to get property name
	 * @return property name
	 */
	std::string getproperty_name();

	/**
	 * Function to get property range
	 * @return property range
	 */
	std::string getproperty_range();

	/**
	 * Function to get string type value
	 * @return string value
	 */
	std::string getproperty_value();

	/**
	 * Function to set property type
	 * @param property type string
	 */
	void setproperty_type(std::string);

	/**
	 * Function to set property name
	 * @param property name
	 */
	void setproperty_name(std::string);

	/**
	 * Function to set property range
	 * @param property range
	 */
	void setproperty_range(std::string);

	/**
	 * Function to set string property
	 * @param string property
	 */
	void setproperty_value(std::string);

	 ~DmProperty();
};

/**
 * @brief 	Entity class stores no of entities available in physical device.
 * 			It also accumulates no. of properties available for control for particular
 * 			Entity.
 */
class DmEntity {
private:
	std::vector<DmProperty*> propertylist;		//!< holds list of properties
	std::string entity_id;							//!< identifier of entity
	std::string entity_name;						//!< name of entity
	std::string entity_type;						//!< name of entity
	int security_level;                        //security level of entity 0=for NA, 1=ALWAYS ARM , 2= DIS ARM, 3= ARM

public:
	/**
	 * Constructor for entity.
	 */
	DmEntity();

	/**
	 * Function to display entities in selected thing
	 */
	void Display();

	/**
	 * Function to get number of properties in things.
	 * @return no. of properties in entity
	 */
	int getPropertySize();

	/**
	 * Function to get entity id
	 * @return entity identifier
	 */
	std::string getentity_id();

	/**
	 * Function to get entity id
	 * @return entity identifier
	 */
	std::string getentity_type();

	/**
	 * Function to get entity name
	 * @return entity name
	 */
	std::string getentity_name();

	/**
	 * Function to select property
	 * @param m : property at selected position
	 * @return property object at position m
	 */
	DmProperty* getPropertyInfo(int);

	/**
	 * Function to get entity security level
	 * @return entity security level
	 */
	int getsecurity_level();

	/**
	 * Function to set entity identifier
	 * @param entity_id : entity identifier string
	 */
	void setentity_id(std::string);

	/**
	 * Function to set entity name
	 * @param entity_name : entity name string
	 */
	void setentity_name(std::string);

	/**
	 * Function to set entity name
	 * @param entity_name : entity name string
	 */
	void setentity_type(std::string);

	/**
	 * Function to set property.
	 * @param property object
	 */
	void setpropertyInfo(DmProperty*);

	/**
	 * Function to set security level
	 * @param security_level : entity security level int
	 */
	void setsecurity_level(int);

	 ~DmEntity();
};

/**
 * @brief	Thing Class is a digital representation of physical end device available in network.
 * 			It Things information like device name, hardware, software version, thing unique identifier
 * 			It holds number of entities that thing holds.
 * 			e.g In Case of Switchboard, SwitchBoard is a thing and individual switches (switch 1, switch 2 etc. are entities)
 */

class DmThing {

private:

	std::string status_code;
	std::string home_node_id;
	std::string zone_type;

	std::string frameworkName;//!< states device belongs to which framework (zigbee/zwave)
	std::string manufacturerName;	//!< name of manufacturer
	std::string manufacturerUrl;	//!< manufactures support web link
	std::string firmwareVersion;	//!< device firmware version
	std::string hardwareVersion;	//!< device hardware version

	std::string battery_status;		//!< Battery strength of device
	std::string thing_type;			//!< type of thing which category it belongs
	std::string thing_status;		//!< states device status online/offline
	std::string thing_name;			//!< end device name
	std::string thing_id;			//!< unique identifier for end device

	std::vector<DmEntity*> entitylist;	//!< list of entities thing can control

	DmFramework *DmFrameworkObj;

public:

	/**
	 * Constructor for Thing class
	 */
	DmThing();
	/**
	 * Function to display thing data
	 */
	void Display();

	static void ThingDiscovery(RequestResponse);
	static bool ThingControl(RequestResponse);
	static bool ThingRemove(RequestResponse);
	static void ThingExclusion(RequestResponse);
	static void ThingS2secutity(RequestResponse);
	static void ThingForceRemove(RequestResponse);
	static void ThingLearnMode(RequestResponse);
	static void ZigbeeFota(RequestResponse);
	void setFramework(DmFramework*);

	DmFramework* getFramework();

	/**
	 *	Function to get node identifier
	 * @return node id in zigbee / zwave
	 */
	std::string gethome_node_id();

	/**
	 * Function to get zone type
	 * @return zone type
	 */
	std::string getzone_type();

	/**
	 * Function to get framework (Zigbee / Zwave)
	 * @return framework
	 */
	std::string getframeworkName();

	/**
	 * Function to get manufacturer name
	 * @return manufacturerName
	 */
	std::string getmanufacturerName();

	/**
	 * Function to get manufacturer url
	 * @return manufacturerUrl
	 */
	std::string getmanufacturerUrl();

	/**
	 * Function to get firmware version
	 * @return firmwareVersion
	 */
	std::string getfirmwareVersion();

	/**
	 * Function to get hardware version
	 * @return hardwareVersion
	 */
	std::string gethardwareVersion();

	/**
	 * Function to get battery status
	 * @return battery_status of end devices
	 */
	std::string getbattery_status();

	/**
	 * Function to get thing type
	 * @return thing type of end device
	 */
	std::string getthing_type();

	/**
	 * Function to get thing status (online / offline)
	 * @return thing status
	 */
	std::string getthing_status();

	/**
	 * Function to get thing name
	 * @return thing name
	 */
	std::string getthing_name();

	/**
	 * Function to get thing identifier
	 * @return thing_id
	 */
	std::string getthing_id();

	/**
	 * Function to get no. of entities in thing
	 * @return number of entities
	 */
	int getEntityListSize();

	/**
	 * Function to get entity pointer
	 * @param m :position of entity in thing
	 * @return entity object
	 */
	DmEntity* getEntityInfo(int);

	/**
	 * Function to set node identifier
	 * @param node_id : node identifier placeholder in case of zigbee (node_id)/ zwave (home_id)
	 */
	void sethome_node_id(std::string);

	/**
	 * Function to set node identifier
	 * @param zone type : node identifier placeholder in case of zigbee (node_id)/ zwave (home_id)
	 */
	void setzone_type(std::string);

	/**
	 * Function to set status code (error cases)
	 * @param status_code : error code within response
	 */
	void setstatus_code(std::string);

	/**
	 * Function to set framework name
	 * @param framework_name : framework name (zigbee / zwave)
	 */
	void setframeworkName(std::string);

	/**
	 * Function to set manufacturer name
	 * @param manufacturer_name : name of manufacturer
	 */
	void setmanufacturerName(std::string);

	/**
	 * Function to set manufacturer url
	 * @param manufacturer_url : web link of manufacturer for support
	 */
	void setmanufacturerUrl(std::string);

	/**
	 * Function to set firmware version of end device
	 * @param firmware_version : gives firmware version
	 */
	void setfirmwareVersion(std::string);

	/**
	 * Function to set hardware version
	 * @param hardware_version : hardware version of end device
	 */
	void sethardwareVersion(std::string);

	/**
	 * Function to set battery strength of end device
	 * @param battery_status : battery strength
	 */
	void setbattery_status(std::string);

	/**
	 * Function to set thing category light, smart_plug etc.
	 * @param thing_type : string type attribute
	 */
	void setthing_type(std::string);

	/**
	 * Function to set thing status whether it is online / offline
	 * @param thing_status : online / offline
	 */
	void setthing_status(std::string);

	/**
	 * Function to set thing name
	 * @param thing_name : name of end device
	 */
	void setthing_name(std::string);

	/**
	 * Function to set thing identifier
	 * @param thing_id : string type thing identifier
	 */
	void setthing_id(std::string);

	/**
	 * Function to set entity object
	 * @param entity_obj : entity object thing contains
	 */
	void setentityInfo(DmEntity*);

	virtual ~DmThing();
};

#endif /* INCLUDE_DM_THING_HPP_ */
