#include "dm_things_backup_db.hpp"
#include "dm_thingmap.hpp"
#include "dm_thing.hpp"
#include "zw_framework.hpp"
#include "zb_framework.hpp"

static int EntityCountDB;
static int PropertyCountDB;

DmThingsBackupDb * DmThingsBackupDb::Instance = 0;
std::vector<std::string> ListThingIDDB;
std::vector<std::string> ListRemoveSyncThingIDDB;

/*DmThingsBackupDb::DmThingsBackupDb() {
 }
 DmThingsBackupDb::~DmThingsBackupDb() {
 }*/

DmThingsBackupDb * DmThingsBackupDb::GetInstance() {
	if (Instance == 0) {
		Instance = new DmThingsBackupDb();
	}
	return Instance;
}

bool DmThingsBackupDb::DMBackUpDatabaseOpen() {

	int DMBackUpQuerryRet;

	DMBackUpQuerryRet = sqlite3_open("DeviceManagerBackUpDB.db", &DMBackUpDB);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database opening failed: [%s] error: [%d]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);
		return false;
	} else {
		syslog(LOG_INFO, "[DM]: Things backup database opening success");
		return true;
	}
	return false;
}

bool DmThingsBackupDb::DMBackUpDatabaseClose() {

	int DMBackUpQuerryRet;

	DMBackUpQuerryRet = sqlite3_close(DMBackUpDB);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database closing failed: [%s] error: [%d]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);
		return false;
	} else {
		syslog(LOG_INFO, "[DM]: Things backup database closing success");
		return true;
	}
	return false;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i < argc; i++) {

		syslog(LOG_DEBUG,
				"[DM]: Things backup printing database result thing [%s]: thing value [%s] ",
				azColName[i], argv[i]);
	}
	return 0;
}

bool DmThingsBackupDb::DMUpdateNodeIdTable(std::string THING_ID,
		std::string Update_Values) {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;

	std::stringstream UPDATE_STRING;

	UPDATE_STRING << "UPDATE THING SET " << Update_Values << " WHERE THING_ID="
			<< "\'" << THING_ID << "\';";

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (UPDATE_STRING.str()).c_str(),
			callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Property table records updated failed: [%s] error: [%d]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);

		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO, "[DM]: Property table records updated successfully");
		return true;
	}
	return false;
}

bool DmThingsBackupDb::CreateBackUpDB() {

	//syslog(LOG_INFO, "[DM]: Thing backup in create backup database");

	DMBackUpDatabaseOpen();
	CreateBackUpThingTable();
	CreateBackUpEntityTable();
	CreateBackUpPropertyTable();
	CreateTronxSyncTable();
	CreateFirmwareDB(); /* Creating a table to store the Device details  (Read access to FOTA module for Firmware update) */

	//syslog(LOG_INFO, "[DM]: Thing backup out create backup database");

	return true;
}

//-----------------------------------------------------------------------------------------------------------------------

//Firmware Table Create
bool DmThingsBackupDb::CreateFirmwareDB() {

	int DMBackUpQuerryRet; /* Var to store the return value of the error code in the SQL command to check the creation of DB*/
	char* DMBackUpErrorRet; /* Var to store the exact error code when accessing the DB*/
	std::stringstream FirmwareTableCreate;
	FirmwareTableCreate /* Creating the new table in the Db for EDFU with Thing ID, Firmware ver, hardware ver, manufacturer name, model ID */
			<< "CREATE TABLE FIRMWARE(  \
         THING_ID    TEXT  PRIMARY KEY   NOT NULL, \
         FIRMWARE_VERSION    TEXT, \
         HARDWARE_VERSION     INT,  \
         MANUFACTURER_NAME   TEXT, \
         MODEL_ID  INT);";

	/*syslog(LOG_DEBUG,
	 "[DM]: Things backup database create thing table query:[%s]",
	 ThingQuerry.str().c_str());*/

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB,
			(FirmwareTableCreate.str()).c_str(), callback, 0,
			&DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) { /* Check if the DB is created Successfully*/
		syslog(LOG_ERR,
				"[DM]: Things backup database create thing table failed: [%s] error: [%d]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);

		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database create thing table success");
		return true;
	}
	return false;

}

/*Function Name : InsertFirmwareDB
 *Description  : FUnction fill in the detail of the newly discovered device in to the DB
 */
bool DmThingsBackupDb::InsertFirmwareDB(FirmwareDB FirmwareDBobj) {

	int DMBackUpQuerryRet; /* Var to store the return value of the error code in the SQL command to check the creation of DB*/
	char* DMBackUpErrorRet; /* Var to store the exact error code when accessing the DB*/

	std::stringstream Firmware_Insert; /* Inserting the device details in to the DB table. */

	Firmware_Insert << "INSERT INTO FIRMWARE VALUES (" << "\'"
			<< FirmwareDBobj.THING_ID << "\' , \'"
			<< FirmwareDBobj.FIRMWARE_VERSION << "\' , "
			<< FirmwareDBobj.HARDWARE_VERSION << " , \'"
			<< FirmwareDBobj.MANUFACTURER_NAME.c_str() << "\' , "
			<< FirmwareDBobj.MODEL_ID << ");";

	/*syslog(LOG_DEBUG,
	 "[DM]: Things backup database insert thing table querry:[%s]",
	 INSERT_THING.str().c_str());*/

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB,
			(Firmware_Insert.str()).c_str(), callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) { /*Check if the DB write is successful */
		syslog(LOG_ERR,
				"[DM]: Things backup database insert Firmware table failed: [%s] error: [%d]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);

		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database insert Firmware table success");
		return true;
	}
	return false;

}

//Select query on firmware table
int DmThingsBackupDb::SelectFirmwareVersion(std::string thing_id) {

	char* FwSqlQuerry;
	sqlite3_stmt *SqliteStructFW;
	int result = 0;

	asprintf(&FwSqlQuerry,
			"SELECT FIRMWARE_VERSION FROM FIRMWARE WHERE THING_ID=%s;",
			thing_id);
	syslog(LOG_DEBUG, "[DM]: Firmware database mapping OUT FW version: [%s]",
			FwSqlQuerry);

	DmThingsBackupDb* DmThingsBackupDbobj = DmThingsBackupDb::GetInstance();
	sqlite3_prepare_v2(DmThingsBackupDbobj->DMBackUpDB, FwSqlQuerry,
			strlen(FwSqlQuerry), &SqliteStructFW,
			NULL);

	while (sqlite3_step(SqliteStructFW) == SQLITE_ROW) {
		result = sqlite3_column_int(SqliteStructFW, 0);
		break;
	}
	sqlite3_finalize(SqliteStructFW);
	syslog(LOG_DEBUG, "[DM]: Firmware database mapping OUT FW version: [%d]",
			result);
	free(FwSqlQuerry);
	return result;
}

//Updating version after firmware-update of end-device.
bool DmThingsBackupDb::UpdateEndDeviceFirmwareDB(RequestResponse FotaObj) {

	int DMBackUpQuerryRet;
	int DMBackUpQuerryRetThing;
	char* DMBackUpErrorRet;

	std::stringstream update_firmwareV;
	std::stringstream update_thingV;

	update_firmwareV << "UPDATE FIRMWARE set FIRMWARE_VERSION = " << "\'"
			<< FotaObj.Firmware_version << "\' where THING_ID = " << "\'"
			<< FotaObj.Thing_Id << "\';";

	syslog(LOG_DEBUG,
			"[DM]: Things backup database insert thing table querry:[%s]",
			update_firmwareV.str().c_str());

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB,
			(update_firmwareV.str()).c_str(), callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Firmware_version in update FIRMWARE table failed: [%s] error: [%d]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpErrorRet);

		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Firmware_version update in FIRMWARE table success");
	}

	update_thingV << "UPDATE THING set FIRMWARE_VERSION = " << "\'"
			<< FotaObj.Firmware_version << "\' where THING_ID = " << "\'"
			<< FotaObj.Thing_Id << "\';";

	DMBackUpQuerryRetThing = sqlite3_exec(DMBackUpDB,
			(update_thingV.str()).c_str(), callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRetThing != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Firmware_version update in THING table failed: [%s] error: [%d]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpErrorRet);

		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Firmware_version update in THING table success");
		return true;
	}
}

int DmThingsBackupDb::ENTITY_COUNTcallback(void *NotUsed, int argc, char **argv,
		char **azColName) {
	std::stringstream temp(argv[0]);
	temp >> EntityCountDB;
	return 0;
}

int DmThingsBackupDb::PROPERTY_COUNTcallback(void *NotUsed, int argc,
		char **argv, char **azColName) {
	std::stringstream temp(argv[0]);
	temp >> PropertyCountDB;
	return 0;
}

int DmThingMapOBJFill(void *NotUsed, int argc, char **argv, char **azColName) {

	DmThingMap* DmThingMapOBJ = DmThingMap::GetInstance();

	std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
			DmThingMapOBJ->DmThingDataMap.find(argv[0]);
	if (DmThingMapObjItr != DmThingMapOBJ->DmThingDataMap.end()) {

		int EnityCount = 0;
		for (EnityCount = 0;
				EnityCount < DmThingMapObjItr->second->getEntityListSize();
				EnityCount++) {
			if (DmThingMapObjItr->second->getEntityInfo(EnityCount)->getentity_id()
					== argv[13]) {
				break;
			}
		}

		if (EnityCount != DmThingMapObjItr->second->getEntityListSize()) {

			DmProperty * propertyDummy = new DmProperty;

			propertyDummy->setproperty_type(argv[19]);
			propertyDummy->setproperty_range(argv[21]);
			propertyDummy->setproperty_name(argv[20]);
			propertyDummy->setproperty_value(argv[18]);

			DmThingMapObjItr->second->getEntityInfo(EnityCount)->setpropertyInfo(
					propertyDummy);
		} else {

			DmEntity* entityDummy = new DmEntity;

			entityDummy->setentity_id(argv[13]);
			entityDummy->setentity_name(argv[14]);
			entityDummy->setentity_type(argv[15]);
			entityDummy->setsecurity_level(std::stoi(argv[16]));

			DmProperty* propertyDummy = new DmProperty;

			propertyDummy->setproperty_type(argv[19]);
			propertyDummy->setproperty_range(argv[21]);
			propertyDummy->setproperty_name(argv[20]);
			propertyDummy->setproperty_value(argv[18]);

			entityDummy->setpropertyInfo(propertyDummy);

			DmThingMapObjItr->second->setentityInfo(entityDummy);

		}

	} else {

		DmThing* ThingDummy = new DmThing;

		ThingDummy->setthing_id(argv[0]);
		ThingDummy->setfirmwareVersion(argv[1]);
		ThingDummy->setframeworkName(argv[2]);
		ThingDummy->sethardwareVersion(argv[3]);
		ThingDummy->setmanufacturerName(argv[4]);
		ThingDummy->setmanufacturerUrl(argv[5]);
		ThingDummy->setthing_type(argv[6]);
		ThingDummy->setthing_name(argv[7]);
		ThingDummy->setthing_status(argv[8]);
		ThingDummy->setbattery_status(argv[9]);
		ThingDummy->setzone_type(argv[10]);
		ThingDummy->sethome_node_id(std::string(argv[11]));

		if (strcmp(ThingDummy->getframeworkName().c_str(), "zwave") == 0) {
			DmFramework* ZWObj = new ZwFramework;
			ThingDummy->setFramework(ZWObj);
		} else if (strcmp(ThingDummy->getframeworkName().c_str(), "zigbee")
				== 0) {
			DmFramework* ZBObj = new ZwFramework;
			ThingDummy->setFramework(ZBObj);
		}

		DmEntity* entityDummy = new DmEntity;

		entityDummy->setentity_id(argv[13]);
		entityDummy->setentity_name(argv[14]);
		entityDummy->setentity_type(argv[15]);
		entityDummy->setsecurity_level(std::stoi(argv[16]));

		DmProperty* propertyDummy = new DmProperty;

		propertyDummy->setproperty_type(argv[19]);
		propertyDummy->setproperty_range(argv[21]);
		propertyDummy->setproperty_name(argv[20]);
		propertyDummy->setproperty_value(argv[18]);

		entityDummy->setpropertyInfo(propertyDummy);

		ThingDummy->setentityInfo(entityDummy);

		DmThingMapOBJ->DmThingDataMap[argv[0]] = ThingDummy;
	}

	return 0;
}

//----------------------------------------------CREATING THING TABLE------------------------------------------------
bool DmThingsBackupDb::CreateBackUpThingTable() {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;
	std::stringstream ThingQuerry;
	ThingQuerry
			<< "CREATE TABLE THING(  \
         THING_ID    TEXT  PRIMARY KEY   NOT NULL, \
         FIRMWARE_VERSION    TEXT, \
         FRAMEWORK   TEXT, \
         HARDWARE_VERSION     TEXT,  \
         MANUFACTURER_NAME   TEXT, \
         MANUFACTURER_URL  TEXT, \
         THING_TYPE   TEXT, \
         THING_NAME   TEXT, \
         THING_STATUS   TEXT, \
         BATTERY_STATUS    TEXT, \
         ZONE_TYPE   TEXT, \
         NODE_ID  TEXT);";

	/*syslog(LOG_DEBUG,
	 "[DM]: Things backup database create thing table querry:[%s]",
	 ThingQuerry.str().c_str());*/

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (ThingQuerry.str()).c_str(),
			callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database create thing table failed: [%s] error: [%d]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);

		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database create thing table success");
		return true;
	}
	return false;
}

//-------------------------------INSERTING VALUES IN THING TABLE-------------------------------------------

bool DmThingsBackupDb::InsertBackUpDeviceDetails(DMThingDB DMThingDBOBJ) {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;

	std::stringstream INSERT_THING;

	INSERT_THING << "UPDATE THING set THING_STATUS = " << "\'"
			<< DMThingDBOBJ.THING_STATUS << "\'" << " where THING_ID = " << "\'"
			<< DMThingDBOBJ.THING_ID << "\'";

	/*syslog(LOG_DEBUG,
	 "[DM]: Things backup database insert thing table querry:[%s]",
	 INSERT_THING.str().c_str());*/

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (INSERT_THING.str()).c_str(),
			callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database insert things table failed: [%s] error: [%d]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);

		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database insert things table success");
		return true;
	}
	return false;
}

bool DmThingsBackupDb::InsertBackUpThingsTable(DMThingDB DMThingDBOBJ) {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;

	std::stringstream INSERT_THING;

	INSERT_THING << "INSERT INTO THING VALUES (" << "\'"
			<< DMThingDBOBJ.THING_ID << "\' , \'"
			<< DMThingDBOBJ.FIRMWARE_VERSION << "\' , \'"
			<< DMThingDBOBJ.FRAMEWORK << "\' , \'"
			<< DMThingDBOBJ.HARDWARE_VERSION << "\' , \'"
			<< DMThingDBOBJ.MANUFACTURER_NAME.c_str() << "\' , \'"
			<< DMThingDBOBJ.MANUFACTURER_URL << "\' , \'"
			<< DMThingDBOBJ.THING_TYPE << "\' , \'" << DMThingDBOBJ.THING_NAME
			<< "\' , \'" << DMThingDBOBJ.THING_STATUS << "\' , \'"
			<< DMThingDBOBJ.BATTERY_STATUS << "\' , \'"
			<< DMThingDBOBJ.ZONE_TYPE << "\' , \'" << DMThingDBOBJ.HOME_NODE_ID
			<< "\' );";

	/*syslog(LOG_DEBUG,
	 "[DM]: Things backup database insert thing table querry:[%s]",
	 INSERT_THING.str().c_str());*/

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (INSERT_THING.str()).c_str(),
			callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database insert things table failed: [%s] error: [%d]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);

		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database insert things table success");
		return true;
	}
	return false;
}

//----------------------------------------------CREATING TRONXSYNC TABLE------------------------------------------------
bool DmThingsBackupDb::CreateTronxSyncTable() {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;
	std::stringstream ThingQuerry;
	ThingQuerry
			<< "CREATE TABLE TRONXSYNC(  \
         REMOVE_NOT_SENT_TO_TRONX    TEXT  PRIMARY KEY   NOT NULL );";

	/*syslog(LOG_DEBUG,
	 "[DM]: Things backup database create TronXSync table querry:[%s]",
	 ThingQuerry.str().c_str());*/

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (ThingQuerry.str()).c_str(),
			callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database create TronXSync table failed: [%s] error: [%d]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);

		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database create TronXSync table success");
		return true;
	}
	return false;
}

//-------------------------------INSERTING VALUES IN TRONXSYNC TABLE-------------------------------------------

bool DmThingsBackupDb::InsertTronxSyncTable(std::string ThingID) {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;

	std::stringstream INSERT_THING;

	INSERT_THING << "INSERT INTO TRONXSYNC VALUES( " << "\'" << ThingID
			<< "\');";

	/*syslog(LOG_DEBUG,
	 "[DM]: Things backup database insert TronXSync table querry:[%s]",
	 INSERT_THING.str().c_str());*/

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (INSERT_THING.str()).c_str(),
			callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database insert things TronXSync failed: [%s] error: [%d]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);

		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database insert TronXSync table success");
		return true;
	}
	return false;
}

//-------------------------------DELETING VALUES FROM TRONXSYNC TABLE-------------------------------------------

bool DmThingsBackupDb::DeleteTronxSyncTable(std::string ThingID) {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;

	std::stringstream DELETE_THING;

	DELETE_THING << "DELETE FROM TRONXSYNC WHERE REMOVE_NOT_SENT_TO_TRONX = "
			<< "\'" << ThingID << "\';";

	/*syslog(LOG_DEBUG,
	 "[DM]: Things backup database delete TronXSync table querry:[%s]",
	 DELETE_THING.str().c_str());*/

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (DELETE_THING.str()).c_str(),
			callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database delete things TronXSync failed: [%s] error: [%d]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);

		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database delete TronXSync table success");
		return true;
	}
	return false;
}

//----------------------------------------------CREATING ENTITY TABLE------------------------------------------------

bool DmThingsBackupDb::CreateBackUpEntityTable() {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;
	std::stringstream EntityQuerry;
	EntityQuerry
			<< "CREATE TABLE ENTITY(  \
         CUSTOM_ENTITY_ID    TEXT  PRIMARY KEY   NOT NULL,  \
         ENTITY_ID   TEXT,  \
         ENTITY_NAME   TEXT,  \
         ENTITY_TYPE   TEXT,  \
         SECURITY_LEVEL   TEXT,  \
         THING_ID    TEXT    NOT NULL,  \
         FOREIGN KEY (THING_ID) REFERENCES THING(THING_ID));";

	/*syslog(LOG_DEBUG,
	 "[DM]: Things backup database create entity table querry:[%s]",
	 EntityQuerry.str().c_str());*/

	/* Execute SQL statement */
	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (EntityQuerry.str()).c_str(),
			callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {

		syslog(LOG_ERR,
				"[DM]: Things backup database create entity table failed: [%s] error: [%d]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);

		EntityQuerry.str("");
		EntityQuerry << "SELECT SECURITY_LEVEL FROM ENTITY;";

		DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB,
				(EntityQuerry.str()).c_str(), callback, 0, &DMBackUpErrorRet);
		if (DMBackUpQuerryRet != SQLITE_OK) {
			syslog(LOG_DEBUG,
					"[DM] : ENTITY Backup Table Schema error mesg:[%s]:::error::[%d]",
					sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);
			EntityQuerry.str("");
			EntityQuerry
					<< "ALTER TABLE ENTITY ADD SECURITY_LEVEL TEXT DEFAULT 0;\
						UPDATE ENTITY SET SECURITY_LEVEL = 2 WHERE ENTITY_TYPE IN (\'Contact Sensor\',\'Occupancy Sensor\');\
						UPDATE ENTITY SET SECURITY_LEVEL = 1 WHERE ENTITY_TYPE IN (\'Gas Sensor\',\'Smoke Sensor\',\'Smoke Detector\');";

			DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB,
					(EntityQuerry.str()).c_str(), callback, 0,
					&DMBackUpErrorRet);
			if (DMBackUpQuerryRet != SQLITE_OK) {
				syslog(LOG_DEBUG,
						"[DM] : UPDATE ENTITY table schema ::fail:::[%s]:::error::[%d]",
						sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);
			}

		}
		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database create entity table success");
		return true;
	}
	return false;
}

//-------------------------------INSERTING VALUES IN ENTITY TABLE-------------------------------------------

bool DmThingsBackupDb::InsertBackUpEntityTable(DMEntityDB DMEntityDBOBJ) {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;
	std::stringstream INSERT_ENTITY, CUSTOM_ENTITY_ID;

	CUSTOM_ENTITY_ID << DMEntityDBOBJ.THING_ID << DMEntityDBOBJ.ENTITY_ID;

	INSERT_ENTITY << "INSERT INTO ENTITY VALUES (" << "\'"
			<< CUSTOM_ENTITY_ID.str() << "\' , \'" << DMEntityDBOBJ.ENTITY_ID
			<< "\' , \'" << DMEntityDBOBJ.ENTITY_NAME << "\' , \'"
			<< DMEntityDBOBJ.ENTITY_TYPE << "\' , \'"
			<< DMEntityDBOBJ.SECURITY_LEVEL << "\' , \'"
			<< DMEntityDBOBJ.THING_ID << "\');";

	/*syslog(LOG_DEBUG,
	 "[DM]: Things backup database insert entity table querry:[%s]",
	 INSERT_ENTITY.str().c_str());*/

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (INSERT_ENTITY.str()).c_str(),
			callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database insert entity table failed: [%s] error: [%d]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);

		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database insert entity table success");
		return true;
	}
	return false;
}

static int callback2(void *NotUsed, int argc, char **argv, char **azColName) {
	//updating security_level in thing map according to database
	DmThingMap* DmThingMapOBJ = DmThingMap::GetInstance();

	std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
			DmThingMapOBJ->DmThingDataMap.find(argv[0]);
	if (DmThingMapObjItr != DmThingMapOBJ->DmThingDataMap.end()) {
		for (int EntityCount = 0;
				EntityCount < DmThingMapObjItr->second->getEntityListSize();
				EntityCount++) {
			if (DmThingMapObjItr->second->getEntityInfo(EntityCount)->getentity_id()
					== argv[1]) {
				DmThingMapObjItr->second->getEntityInfo(EntityCount)->setsecurity_level(
						std::stoi(argv[2]));
				break;
			}
		}
	}
	return 0;
}

bool DmThingsBackupDb::UpdateEntityTable(std::string custom_id, int security,
		int mode) {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;
	std::stringstream UPDATE_ENTITY;
	std::stringstream SELECT_ENTITY;

	if (mode == 2) {
		//dis-arm all mode

		/*syslog(LOG_DEBUG,
		 "[DM]: update entity table:::dis arm all mode");*/

		UPDATE_ENTITY
				<< "UPDATE ENTITY SET SECURITY_LEVEL = 2 WHERE SECURITY_LEVEL = 3";

	} else if (mode == 1 || mode == 3) {
		//arm-all/arm-away/arm-stay mode/single area arm mode

		/*syslog(LOG_DEBUG,
		 "[DM] : update entity table:::arm all/arm stay/arm away mode/single area arm mode");*/

		UPDATE_ENTITY << "UPDATE ENTITY SET SECURITY_LEVEL = " << security
				<< " WHERE CUSTOM_ENTITY_ID IN (" << custom_id << "\);";
	}

	syslog(LOG_DEBUG, "[DM] : Update Entity Table query is::[%s]",
			UPDATE_ENTITY.str().c_str());

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (UPDATE_ENTITY.str()).c_str(),
			callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database update entity table failed: [%s] error: [%d]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);

		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		if (mode == 1) {

			UPDATE_ENTITY.str("");
			//arm-all/arm-stay/arm-away mode disarming remaining devices in data base.

			UPDATE_ENTITY
					<< "UPDATE ENTITY SET SECURITY_LEVEL = 2 WHERE CUSTOM_ENTITY_ID NOT IN ("
					<< custom_id << ") and SECURITY_LEVEL = 3;";

			syslog(LOG_DEBUG,
					"[DM] : Update Entity Table query 2 for arm away /aram stay is ::[%s]",
					UPDATE_ENTITY.str().c_str());

			DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB,
					(UPDATE_ENTITY.str()).c_str(), callback, 0,
					&DMBackUpErrorRet);

			if (DMBackUpQuerryRet != SQLITE_OK) {
				syslog(LOG_ERR,
						"[DM]: Things backup database update entity table failed: [%s] error: [%d]",
						sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);

				sqlite3_free(DMBackUpErrorRet);
				return false;
			}
		}
		syslog(LOG_INFO,
				"[DM]: Things backup database update entity table success");

		//sync thing map and database.

		SELECT_ENTITY
				<< "SELECT THING_ID,ENTITY_ID,SECURITY_LEVEL ,ENTITY_TYPE FROM ENTITY where ENTITY_TYPE IN (\"Contact Sensor\",\"Occupancy Sensor\");";

		syslog(LOG_DEBUG, "[DM] : select entity table selected query is::[%s]",
				SELECT_ENTITY.str().c_str());

		DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB,
				(SELECT_ENTITY.str()).c_str(), callback2, 0, &DMBackUpErrorRet);

		if (DMBackUpQuerryRet != SQLITE_OK) {
			syslog(LOG_DEBUG, "[DM] :data change failed");
		} else
			syslog(LOG_DEBUG, "[DM] :data successfully changed");

		return true;
	}
}

//----------------------------------------------CREATING PROPERTY TABLE------------------------------------------------

bool DmThingsBackupDb::CreateBackUpPropertyTable() {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;
	std::stringstream PropertyQuerry;

	PropertyQuerry
			<< "CREATE TABLE PROPERTY(  \
         CUSTOM_PROPERTY_ID     TEXT    PRIMARY KEY    NOT NULL, \
         PROPERTY_VALUE    TEXT, \
         PROPERTY_TYPE     TEXT, \
         PROPERTY_NAME     TEXT, \
         PROPERTY_RANGE    TEXT, \
         CUSTOM_ENTITY_ID   TEXT     NOT NULL, \
         FOREIGN KEY (CUSTOM_ENTITY_ID) REFERENCES ENTITY(CUSTOM_ENTITY_ID));";

	/*syslog(LOG_DEBUG,
	 "[DM]: Things backup database create property table querry:[%s]",
	 PropertyQuerry.str().c_str());*/

	/* Execute SQL statement */
	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (PropertyQuerry.str()).c_str(),
			callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database create property table failed: [%s] error: [%d]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);

		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database create property table success");
		return true;
	}
	return false;
}

//-------------------------------INSERTING VALUES IN PROPERTY TABLE-------------------------------------------

bool DmThingsBackupDb::InsertBackUpPropertyTable(DMPropertyDB DMPropertyDBOBJ) {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;

	std::stringstream INSERT_PROPERTY, CUSTOM_PROPERTY_ID;

	CUSTOM_PROPERTY_ID << DMPropertyDBOBJ.THING_ID << DMPropertyDBOBJ.ENTITY_ID
			<< DMPropertyDBOBJ.PROPERTY_NAME;

	INSERT_PROPERTY << "INSERT INTO PROPERTY VALUES (" << "\'"
			<< CUSTOM_PROPERTY_ID.str() << "\' , \'"
			<< DMPropertyDBOBJ.PROPERTY_VALUE << "\' , \'"
			<< DMPropertyDBOBJ.PROPERTY_TYPE << "\' , \'"
			<< DMPropertyDBOBJ.PROPERTY_NAME << "\' , \'"
			<< DMPropertyDBOBJ.PROPERTY_RANGE << "\' , \'"
			<< DMPropertyDBOBJ.THING_ID << DMPropertyDBOBJ.ENTITY_ID << "\');";

	/*syslog(LOG_DEBUG,
	 "[DM]: Things backup database insert property table querry:[%s]",
	 INSERT_PROPERTY.str().c_str());*/

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB,
			(INSERT_PROPERTY.str()).c_str(), callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database insert property table failed: [%s] error: [%d]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);

		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database insert property table success");
		return true;
	}
	return false;
}

//------------------------------------------------QUERY-----------------------------------

int DmThingsBackupDb::GetEntityCount(std::string THING_ID) {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;

	std::stringstream COUNT_STRING;

	COUNT_STRING
			<< "SELECT COUNT(ENTITY_ID) FROM ENTITY WHERE ENTITY.THING_ID=\'"
			<< THING_ID << "\';";

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (COUNT_STRING.str()).c_str(),
			ENTITY_COUNTcallback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database get entity count failed: [%s] error: [%d]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpQuerryRet);

		sqlite3_free(DMBackUpErrorRet);
		return EntityCountDB;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database get entity count success");
		return EntityCountDB;
	}
	return EntityCountDB;
}

bool DmThingsBackupDb::GetPropertyCount(std::string THING_ID) {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;

	DMBackUpQuerryRet = sqlite3_open("DeviceManagerBackUpDB.db", &DMBackUpDB);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database get property open database failed: [%s] error: [%s]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpErrorRet);
	} else {
		syslog(LOG_ERR,
				"[DM]: Things backup database get property open database success");
	}

	std::stringstream COUNT_STRING;

	COUNT_STRING
			<< "SELECT COUNT(PROPERTY_NAME) FROM PROPERTY WHERE EXISTS (SELECT * FROM ENTITY WHERE ENTITY.CUSTOM_ENTITY_ID=PROPERTY.CUSTOM_ENTITY_ID AND ENTITY.THING_ID=\'"
			<< THING_ID << "\');";

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (COUNT_STRING.str()).c_str(),
			PROPERTY_COUNTcallback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database get property count failed: [%s] error: [%s]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpErrorRet);
		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database get property count success");
		return true;
	}
	return false;
}

bool DmThingsBackupDb::GetBackUpThingData(std::string THING_ID) {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;

	GetEntityCount(THING_ID);
	GetPropertyCount(THING_ID);

	std::stringstream QUERY_STRING;

	if (THING_ID == "NULL") {
		QUERY_STRING
				<< "SELECT * FROM (SELECT * FROM THING NATURAL JOIN ENTITY) NATURAL JOIN PROPERTY;";
	} else {
		QUERY_STRING
				<< "SELECT * FROM (SELECT * FROM THING NATURAL JOIN ENTITY WHERE ENTITY.THING_ID="
				<< "\'" << THING_ID << "\'" << ") NATURAL JOIN PROPERTY;";
	}

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (QUERY_STRING.str()).c_str(),
			DmThingMapOBJFill, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database get backup thing data failed: [%s] error: [%s]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpErrorRet);
		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database get backup thing data success");
		return true;
	}
	return false;
}

bool DmThingsBackupDb::RestoreThingBackupDB() {

	ListThingIDDB = ListOfThingId();

	/*syslog(LOG_DEBUG,
	 "[DM]: Things backup database restore thing backup database size:[%d]",
	 ListThingIDDB.size());*/

	if (ListThingIDDB.size() > 0) {

		for (size_t ListThingsIdcount = 0;
				ListThingsIdcount < ListThingIDDB.size(); ListThingsIdcount++) {

			GetBackUpThingData(ListThingIDDB[ListThingsIdcount]);

			DmThingMap* DmThingMapOBJ = DmThingMap::GetInstance();

			std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
					DmThingMapOBJ->DmThingDataMap.find(
							ListThingIDDB[ListThingsIdcount]);
			if (DmThingMapObjItr != DmThingMapOBJ->DmThingDataMap.end()) {

				DmThingMapObjItr->second->Display();
			}
		}
		return true;
	} else {
		syslog(LOG_DEBUG,
				"[DM]: Things backup database restore thing backup database null size:[%zu]",
				ListThingIDDB.size());
		return false;
	}
	return false;
}

bool DmThingsBackupDb::UpdateThingTable(std::string THING_ID,
		std::string Update_Values) {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;

	std::stringstream UPDATE_STRING;

	UPDATE_STRING << "UPDATE THING SET " << Update_Values << " WHERE THING_ID="
			<< "\'" << THING_ID << "\';";

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (UPDATE_STRING.str()).c_str(),
			callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_DEBUG,
				"[DM]: Things backup database update thing table fail:[%s]",
				DMBackUpErrorRet);
		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_DEBUG,
				"[DM]: Things backup database update thing table success:[%s]",
				DMBackUpErrorRet);
		return true;
	}
	return false;
}

bool DmThingsBackupDb::UpdatePropertyTable(DMPropertyDB DMPropertyDBOBJ) {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;

	std::stringstream UPDATE_STRING;

	UPDATE_STRING << "UPDATE PROPERTY SET " << DMPropertyDBOBJ.PROPERTY_VALUE
			<< " WHERE PROPERTY_NAME=" << "\'" << DMPropertyDBOBJ.PROPERTY_NAME
			<< "\' AND CUSTOM_ENTITY_ID=" << "\'" << DMPropertyDBOBJ.THING_ID
			<< DMPropertyDBOBJ.ENTITY_ID << "\';";

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (UPDATE_STRING.str()).c_str(),
			callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database update property table failed: [%s] error: [%s]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpErrorRet);
		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database update property table success");
		return true;
	}
	return false;
}

bool DmThingsBackupDb::DeleteDataBaseThing(std::string THING_ID) {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;

	std::stringstream Delete_Query;

	Delete_Query
			<< "DELETE FROM PROPERTY WHERE EXISTS (SELECT * FROM ENTITY WHERE ENTITY.CUSTOM_ENTITY_ID=PROPERTY.CUSTOM_ENTITY_ID AND ENTITY.THING_ID=\'"
			<< THING_ID << "\');";
	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (Delete_Query.str()).c_str(),
			callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database delete property table failed: [%s] error: [%s]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpErrorRet);
		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database delete property table success");
	}

	Delete_Query.str("");

	Delete_Query << "DELETE FROM ENTITY WHERE THING_ID=\'" << THING_ID << "\';";
	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (Delete_Query.str()).c_str(),
			callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database delete entity table failed: [%s] error: [%s]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpErrorRet);
		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database delete entity table success");
	}

	Delete_Query.str("");

	Delete_Query << "DELETE FROM THING WHERE THING_ID=\'" << THING_ID << "\';";
	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (Delete_Query.str()).c_str(),
			callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database delete thing table failed: [%s] error: [%s]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpErrorRet);
		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database delete thing table success");
	}

	std::stringstream Del_Query;

	Del_Query << "DELETE FROM FIRMWARE WHERE THING_ID=\'" << THING_ID << "\';";
	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (Del_Query.str()).c_str(),
			callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Firmware database delete thing failed: [%s] error: [%s]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpErrorRet);
		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO, "[DM]: Firmware database delete thing success");
		return true;
	}
	return false;
}

static int GetListOfThingId(void *NotUsed, int argc, char **argv,
		char **azColName) {

	syslog(LOG_DEBUG,
			"[DM]: Things backup database get list of thing id thing table success:[%s]",
			argv[0]);

	ListThingIDDB.push_back(argv[0]);

	return 0;
}

std::vector<std::string> DmThingsBackupDb::ListOfThingId() {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;

	ListThingIDDB.clear();

	std::stringstream COUNT_STRING;

	COUNT_STRING << "SELECT THING_ID FROM THING;";

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (COUNT_STRING.str()).c_str(),
			GetListOfThingId, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database list thing table failed: [%s] error: [%s]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpErrorRet);
		sqlite3_free(DMBackUpErrorRet);
		return ListThingIDDB;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database list thing table success");
		return ListThingIDDB;
	}

	return ListThingIDDB;
}
//-----------------------------------------------------------------------------

static int GetListOfRemoveSyncThingId(void *NotUsed, int argc, char **argv,
		char **azColName) {

	syslog(LOG_DEBUG,
			"[DM]: Things backup database get list of thing id TronxSync table success:[%s]",
			argv[0]);

	ListRemoveSyncThingIDDB.push_back(argv[0]);

	return 0;
}

std::vector<std::string> DmThingsBackupDb::ListOfRemoveSyncThingId() {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;

	ListRemoveSyncThingIDDB.clear();

	std::stringstream COUNT_STRING;

	COUNT_STRING << "SELECT REMOVE_NOT_SENT_TO_TRONX FROM TRONXSYNC;";

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (COUNT_STRING.str()).c_str(),
			GetListOfRemoveSyncThingId, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database list TronxSync table failed: [%s] error: [%s]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpErrorRet);
		sqlite3_free(DMBackUpErrorRet);
		return ListRemoveSyncThingIDDB;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database list TronxSync table success");
		return ListRemoveSyncThingIDDB;
	}

	return ListRemoveSyncThingIDDB;
}

//-----------------------------------------------------------------------------

bool DmThingsBackupDb::ClearBackUpDatabase() {

	int DMBackUpQuerryRet;
	char* DMBackUpErrorRet;

	std::stringstream THING_CHECK, THING_CLEAR, ENTITY_CLEAR, PROPERTY_CLEAR,
			SCENE_CLEAR, RULE_CLEAR, FIRMWARE_CLEAR;

	THING_CHECK << "SELECT * FROM THING;";

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (THING_CLEAR.str()).c_str(),
			callback, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database check thing table failed: [%s] error: [%s]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpErrorRet);
		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database check thing table success");
	}

	THING_CLEAR << "DELETE FROM THING;";

	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (THING_CLEAR.str()).c_str(),
			GetListOfThingId, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database clear thing table failed: [%s] error: [%s]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpErrorRet);
		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database clear thing table success");
	}

	ENTITY_CLEAR << "DELETE FROM ENTITY;";
	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (ENTITY_CLEAR.str()).c_str(),
			GetListOfThingId, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database clear entity table failed: [%s] error: [%s]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpErrorRet);
		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database clear entity table success");
	}

	PROPERTY_CLEAR << "DELETE FROM PROPERTY;";
	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (PROPERTY_CLEAR.str()).c_str(),
			GetListOfThingId, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database clear property table failed: [%s] error: [%s]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpErrorRet);
		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database clear property table success");
	}
	/**
	 * @REDMINE: 22368
	 * Start
	 * */

//	SCENE_CLEAR << "DELETE FROM SCENE;";
//	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (SCENE_CLEAR.str()).c_str(),
//			GetListOfThingId, 0, &DMBackUpErrorRet);
//
//	if (DMBackUpQuerryRet != SQLITE_OK) {
//		syslog(LOG_ERR,
//				"[DM]: Things backup database clear scene table failed: [%s] error: [%s]",
//				sqlite3_errmsg(DMBackUpDB), DMBackUpErrorRet);
//		sqlite3_free(DMBackUpErrorRet);
//		return false;
//	} else {
//		syslog(LOG_INFO,
//				"[DM]: Things backup database clear scene table success");
//	}

	/**
	 * @REDMINE: 22368
	 * End
	 * */

	RULE_CLEAR << "DELETE FROM RULES;";
	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (RULE_CLEAR.str()).c_str(),
			GetListOfThingId, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database clear rules table failed: [%s] error: [%s]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpErrorRet);
		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database clear rules table success");
	}

	//To clear device details in FIRMWARE table

	FIRMWARE_CLEAR << "DELETE FROM FIRMWARE;";
	DMBackUpQuerryRet = sqlite3_exec(DMBackUpDB, (FIRMWARE_CLEAR.str()).c_str(),
			GetListOfThingId, 0, &DMBackUpErrorRet);

	if (DMBackUpQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Things backup database clear firmware table failed: [%s] error: [%s]",
				sqlite3_errmsg(DMBackUpDB), DMBackUpErrorRet);
		sqlite3_free(DMBackUpErrorRet);
		return false;
	} else {
		syslog(LOG_INFO,
				"[DM]: Things backup database clear firmware table success");
		return true;
	}
	return false;
}

bool DmThingsBackupDb::DeviceManagerBackUp(std::string Thing_id) {

	//syslog(LOG_INFO, "[DM]: thing backup storing into backup start");

	DmThingsBackupDb* DmThingsBackupDbOBJ = DmThingsBackupDb::GetInstance();

	DmThingMap* DmThingMapObj = DmThingMap::GetInstance();

	std::map<std::string, DmThing*>::iterator DmThingMapObjItr =
			DmThingMapObj->DmThingDataMap.find(Thing_id);
	if (DmThingMapObjItr != DmThingMapObj->DmThingDataMap.end()) {

		DMThingDB DMThingDBOBJ;

		DMThingDBOBJ.THING_ID = DmThingMapObjItr->second->getthing_id();
		DMThingDBOBJ.FIRMWARE_VERSION =
				DmThingMapObjItr->second->getfirmwareVersion();
		DMThingDBOBJ.FRAMEWORK = DmThingMapObjItr->second->getframeworkName();
		DMThingDBOBJ.HARDWARE_VERSION =
				DmThingMapObjItr->second->gethardwareVersion();
		DMThingDBOBJ.MANUFACTURER_NAME =
				DmThingMapObjItr->second->getmanufacturerName();
		DMThingDBOBJ.MANUFACTURER_URL =
				DmThingMapObjItr->second->getmanufacturerUrl();
		DMThingDBOBJ.THING_TYPE = DmThingMapObjItr->second->getthing_type();
		DMThingDBOBJ.THING_NAME = DmThingMapObjItr->second->getthing_name();
		DMThingDBOBJ.THING_STATUS = DmThingMapObjItr->second->getthing_status();
		DMThingDBOBJ.BATTERY_STATUS =
				DmThingMapObjItr->second->getbattery_status();
		DMThingDBOBJ.ZONE_TYPE = DmThingMapObjItr->second->getzone_type();
		DMThingDBOBJ.HOME_NODE_ID = DmThingMapObjItr->second->gethome_node_id();

		DmThingsBackupDbOBJ->InsertBackUpThingsTable(DMThingDBOBJ);

		FirmwareDB FirmwareDBobj;
		FirmwareDBobj.THING_ID = DmThingMapObjItr->second->getthing_id();
		FirmwareDBobj.FIRMWARE_VERSION =
				DmThingMapObjItr->second->getfirmwareVersion();
		FirmwareDBobj.HARDWARE_VERSION = std::stoi(
				DmThingMapObjItr->second->gethardwareVersion());
		FirmwareDBobj.MANUFACTURER_NAME =
				DmThingMapObjItr->second->getmanufacturerName();
		FirmwareDBobj.MODEL_ID = DmThingMapObjItr->second->getEntityListSize();

		DmThingsBackupDbOBJ->InsertFirmwareDB(FirmwareDBobj);

		for (int EntityNo = 0;
				EntityNo < DmThingMapObjItr->second->getEntityListSize();
				EntityNo++) {

			DMEntityDB DMEntityDBOBJ;

			DMEntityDBOBJ.ENTITY_ID = DmThingMapObjItr->second->getEntityInfo(
					EntityNo)->getentity_id();
			DMEntityDBOBJ.ENTITY_NAME = DmThingMapObjItr->second->getEntityInfo(
					EntityNo)->getentity_name();
			DMEntityDBOBJ.ENTITY_TYPE = DmThingMapObjItr->second->getEntityInfo(
					EntityNo)->getentity_type();
			DMEntityDBOBJ.SECURITY_LEVEL =
					DmThingMapObjItr->second->getEntityInfo(EntityNo)->getsecurity_level();
			DMEntityDBOBJ.THING_ID = DmThingMapObjItr->second->getthing_id();

			DmThingsBackupDbOBJ->InsertBackUpEntityTable(DMEntityDBOBJ);

			for (int PropertyNo = 0;
					PropertyNo
							< DmThingMapObjItr->second->getEntityInfo(EntityNo)->getPropertySize();
					PropertyNo++) {

				DMPropertyDB DMPropertyDBOBJ;

				DMPropertyDBOBJ.PROPERTY_VALUE =
						DmThingMapObjItr->second->getEntityInfo(EntityNo)->getPropertyInfo(
								PropertyNo)->getproperty_value();
				DMPropertyDBOBJ.PROPERTY_TYPE =
						DmThingMapObjItr->second->getEntityInfo(EntityNo)->getPropertyInfo(
								PropertyNo)->getproperty_type();
				DMPropertyDBOBJ.PROPERTY_NAME =
						DmThingMapObjItr->second->getEntityInfo(EntityNo)->getPropertyInfo(
								PropertyNo)->getproperty_name();
				DMPropertyDBOBJ.PROPERTY_RANGE =
						DmThingMapObjItr->second->getEntityInfo(EntityNo)->getPropertyInfo(
								PropertyNo)->getproperty_range();
				DMPropertyDBOBJ.ENTITY_ID =
						DmThingMapObjItr->second->getEntityInfo(EntityNo)->getentity_id();
				DMPropertyDBOBJ.THING_ID =
						DmThingMapObjItr->second->getthing_id();

				DmThingsBackupDbOBJ->InsertBackUpPropertyTable(DMPropertyDBOBJ);

			}

		}
		//syslog(LOG_INFO, "[DM]: thing backup  storing into backup end");

		return true;
	} else {
		syslog(LOG_INFO,
				"[DM]: thing backup storing thing not found into backup: [%s]",
				Thing_id.c_str());
		return false;
	}
	return false;
}

/**
 * Function to get the property value from Backup Database
 * Passing parameters as Custom property id(Thing Id + entity id+Property name),
 * Property name, Property range and Custom entity id(Thing Id + entity id)
 * */

std::string DmThingsBackupDb::Get_Property_Value(DMPropertyDB DMPropertyDBOBJ) {

	char* FwSqlQuerry;
	sqlite3_stmt *SqliteStructFW;
	std::string result = "DM_DEFAULT";
	asprintf(&FwSqlQuerry,
			"SELECT PROPERTY_VALUE FROM PROPERTY WHERE CUSTOM_PROPERTY_ID='%s'  AND PROPERTY_NAME='%s' AND CUSTOM_ENTITY_ID='%s';",
			(DMPropertyDBOBJ.THING_ID + DMPropertyDBOBJ.ENTITY_ID
					+ DMPropertyDBOBJ.PROPERTY_NAME).c_str(),
			DMPropertyDBOBJ.PROPERTY_NAME.c_str(),
			(DMPropertyDBOBJ.THING_ID + DMPropertyDBOBJ.ENTITY_ID).c_str());
	syslog(LOG_DEBUG,
			"[DM]: Thing Backup database mapping Getting Property Value: [%s]",
			FwSqlQuerry);
	DmThingsBackupDb* DmThingsBackupDbobj = DmThingsBackupDb::GetInstance();
	sqlite3_prepare_v2(DmThingsBackupDbobj->DMBackUpDB, FwSqlQuerry,
			strlen(FwSqlQuerry), &SqliteStructFW,
			NULL);
	while (sqlite3_step(SqliteStructFW) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructFW, 0));
		break;
	}
	sqlite3_finalize(SqliteStructFW);
	syslog(LOG_DEBUG,
			"[DM]: Thing Backup database mapping Getting Property Value result [%s]",
			result.c_str());
	free(FwSqlQuerry);
	return result;
}

