/*
 * File_name:zw_mapping_database.hpp
 * @Breief: This file is regarding the Zwave-database function decllartions
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef INCLUDE_ZW_MAPPING_DATABASE_HPP_
#define INCLUDE_ZW_MAPPING_DATABASE_HPP_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <sys/syslog.h>

class ZwMappingDatabase {
public:

	/*
	 *@Brief:  Default constructor
	 * */
	ZwMappingDatabase() {}

	/*
	 *@Brief: Opening the Zwave Database
	 *@return: If it was succcesfully Opened , it returns true else it will return false
	 * */
	static bool ZwaveDatabaseOpen();

	/*
	 *@Brief: Closing the Zwave Database
	 *@return: If it was succcesfully Closed ,it returns true else it will return false
	 * */
	static bool ZwaveDatabaseClose();

	/*
	 *@Brief: Fetching the Thingtype from Z-wave database
	 *@param: passing Generic and Specific device class
	 *@return: If it was succcesfully, it returns Thingtype of the device
	 * */
	static std::string DMThingTypeZW(int, int);

	/*
	 *@Brief: Fetching the ThingName from Z-wave database
	 *@param: passing Generic,Specific device class,Command class and Sensor Type
	 *@return: If it was succcesfully, it returns ThingName of the device
	 * */
	static std::string DMThingNameZW(int, int, int, int);

	/*
	 *@Brief: Fetching the ManufactureName from Z-wave database
	 *@param: passing Version Id,Product Id,Category and Product Type
	 *@return: If it was succcesfully, it returns ManufactureName of the device
	 * */
	static std::string DMManufactureNameZW(int, int, int, int);

	/*
	 *@Brief: Fetching the EntityType from Z-wave database
	 *@param: passing Generic,Specific device class,Command class, Sensor Type and Events of that sensor
	 *@return: If it was succcesfully, it returns EntityType of the device like Bulb,Smartplug etc.
	 * */
	static std::string DMEntityTypeZW(int, int, int, int, int);

    /*
	 *@Brief: Fetching the EntityName from Z-wave database
	 *@param: passing Generic,Specific device class,Command class, Sensor Type and Events of that sensor
	 *@return: If it was succcesfully, it returns EntityName of the device like Color dimmable light etc.
	 * */
	static std::string DMEntityNameZW(int, int, int, int, int);

	/*
	 *@Brief: Fetching the EntitySecurityLevel from Z-wave database
	 *@param: passing Generic,Specific device class,Command class, Sensor Type and Events of that sensor
	 *@return: At present we are Manually entered the security levels
	 * */
    static int DMEntitySecurityLevelZW(int, int, int, int, int);

	/*
	 *@Brief: Fetching the PropertyTypeZW from Z-wave database
	 *@param: passing Generic,Specific device class,Command class and  Sensor Type
	 *@return: It it was succesfully it returns text like [1,2]
	 * */
	static std::string DMDOPropertyTypeZW(int, int, int);

	/*
	 *@Brief: Fetching the PropertyNameZW from Z-wave database
	 *@param: passing Specific device class,Command class,Property type,Sensor type and Event type
	 *@return: It it was succesfully it returns like property name based on Event
	 * */
	static std::string DMPropertyNameEvent(int, int, int, int, int);

	/*
	 *@Brief: Fetching the PropertyTypeZW from Z-wave database
	 *@param: passing Specific device class,Command class and Sensor type
	 *@return: It it was succesfully it returns like 1,2 etc.
	 * */
	static int DMPropertyTypeZW(int, int, int);

	/*
	 *@Brief: Fetching the PropertyNameSensor from Z-wave database
	 *@param: passing Specific device class,Command class ,Propertype and Sensor type
	 *@return: It it was succesfully it returns like Contact  Detect,Occupancy etc.
	 * */
	static std :: string  DMPropertyNameSensorZW(int,int,int,int);

	/*
	 *@Brief: Fetching the PropertyNameSensor from Z-wave database
	 *@param: passing Specific device class,Command class ,Propertype and Sensor type
	 *@return: It it was succesfully it returns like On_OFF,Brightness
	 * */
	static std::string DMPropertyNameZW(int, int, int, int, int);

	/*
	 *@Brief: Fetching the PropertyRange from Z-wave database
	 *@param: passing Specific device class,Command class and Propertype
	 *@return: It it was succesfully it returns Property range like 1 and 0, 1-99 for Brightness
	 * */
	static std::string DMPropertyRangeZW(int, int, int, int);

    /*
	 *@Brief: Fetching the Sensor type from Z-wave database
	 *@param: passing Specific device class,Command class and Propertype
	 *@return: It it was succesfully it returns sensor type like 1 for temperature,3 for lux etc
	 * */
	static int DMSensorTypeZW(int, int, int, int);
	
	/*
	 *@Brief: Fetching the Commnad class ID from Z-wave database
	 *@param: passing Generic,Specific device class and Command class
	 *@return: It it was succesfully it returns Command class like 38,51,37 etc
	 * */
	static int DMCommandIDCheck(int, int, int);

	/*
	 *@Brief: Fetching the Event Type from Z-wave database
	 *@param: passing Generic,Specific device class,Command class,Property type,Sensor type and Event type
	 *@return: It it was succesfully it returns Sensor events like 22 for door open etc
	 * */
	static int DMSensorEventTypeZW(int, int, int, int, int);

	/*
	 *@Brief: Fetching the Commnad class ID from Z-wave database
	 *@param: passing Generic,Specific device class and  Property name
	 *@return: It it was succesfully it returns Command class like 38,51,37 etc
	 * */
	static int DMCommandClassIDZW(int, int, std::string);

	/*
	 *@Brief: Fetching the AttributeType from Z-wave database
	 *@param: passing  Property name
	 *@return: It it was succesfully it returns the type like 3 and 2 for color
	 **/
	static int DMAttributeTypeZW(std::string);

	/*
	 *@Brief: Fetching the ErrorCode from Z-wave database
	 *@param: passing  the error code
	 *@return: It it was succesfully it returns the integer value
	 **/
	static int DMErrorCodeZW(int);

	/*
	 *@Brief: Fetching the ErrorType from Z-wave database
	 *@param: passing  the error code
	 *@return: It it was succesfully it returns the string
	 **/
	static std::string DMErrorTypeZW(int);

	/*
	 *@Brief: Destructors are used to free the resources
	 **/
	 ~ZwMappingDatabase() {}

};

#endif /* INCLUDE_ZW_MAPPING_DATABASE_HPP_ */
