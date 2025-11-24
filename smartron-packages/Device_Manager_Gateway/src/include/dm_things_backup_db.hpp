/*
 * File_name:dm_things_backup_db.hpp
 * @Brief: Driver class to create backu into databasse for thing/entity/property details
 * Created on: 02-Aug-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef INCLUDE_DM_THINGS_BACKUP_DB_HPP_
#define INCLUDE_DM_THINGS_BACKUP_DB_HPP_

#include <iostream>
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

class DmThingsBackupDb {
private:

	static DmThingsBackupDb *Instance;

public:

	DmThingsBackupDb() {}
	sqlite3 *DMBackUpDB;
	static DmThingsBackupDb *GetInstance();
	bool CreateBackUpDB();
	bool DMBackUpDatabaseOpen();
	bool CreateBackUpThingTable();
	bool CreateTronxSyncTable();						/** To create table TronxSync which is used to sync things to be removed */
	bool InsertTronxSyncTable(std::string);				/** To insert values in table TronxSync which is used to sync things to be removed */
	bool DeleteTronxSyncTable(std::string);				/** To delete values from table TronxSync which is used to sync things to be removed */
	std::vector<std::string> ListOfRemoveSyncThingId();	/** To get all the things to be removed from TronxSync table*/
	bool CreateBackUpEntityTable();
	bool CreateBackUpPropertyTable();
	bool InsertBackUpThingsTable(DMThingDB);
	bool InsertBackUpDeviceDetails(DMThingDB);
	bool InsertBackUpEntityTable(DMEntityDB);
	bool InsertBackUpPropertyTable(DMPropertyDB);
	bool GetBackUpThingData(std::string);
	bool UpdatePropertyTable(DMPropertyDB);
	bool UpdateThingTable(std::string, std::string);
	bool UpdateEntityTable(std::string, int, int);
	static int ENTITY_COUNTcallback(void *NotUsed, int argc, char **argv,
			char **azColName);
	static int PROPERTY_COUNTcallback(void *NotUsed, int argc, char **argv,
			char **azColName);
	bool DeleteDataBaseThing(std::string);
	std::vector<std::string> ListOfThingId();
	bool GetPropertyCount(std::string);
	int GetEntityCount(std::string);
	bool ClearBackUpDatabase();
	bool DMBackUpDatabaseClose();
	bool DeviceManagerBackUp(std::string);
	bool RestoreThingBackupDB();
	bool DMUpdateNodeIdTable(std::string, std::string);
/**
 * Function to get the property value from Backup Database
 * Passing parameters as Custom property id(Thing Id + entity id+Property name), Property name, Property range and Custom entity id(Thing Id + entity id)*/
	std::string Get_Property_Value(DMPropertyDB );
	bool CreateFirmwareDB();
	bool InsertFirmwareDB(FirmwareDB);
	static int SelectFirmwareVersion(std::string);
	bool UpdateEndDeviceFirmwareDB(RequestResponse);
	 ~DmThingsBackupDb() {
		 ListOfThingId().shrink_to_fit();
		 ListOfRemoveSyncThingId().shrink_to_fit();
	 }
};

#endif /* INCLUDE_DM_THINGS_BACKUP_DB_HPP_ */
