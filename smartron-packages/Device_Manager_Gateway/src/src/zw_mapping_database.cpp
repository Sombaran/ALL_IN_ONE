#include "zw_mapping_database.hpp"

static sqlite3 *DMZwaveDB;

/*ZwMappingDatabase::ZwMappingDatabase() {   // TODO Auto-generated constructor stub
}
ZwMappingDatabase::~ZwMappingDatabase() {   // TODO Auto-generated destructor stub
}*/

bool ZwMappingDatabase::ZwaveDatabaseOpen() {

	int DMZwaveQuerryRet = sqlite3_open("ZwaveDatabaseMap.db", &DMZwaveDB);

	syslog(LOG_DEBUG, "[DM]: Zwave database open status:[%d]",
			DMZwaveQuerryRet);
	if (DMZwaveQuerryRet) {
		syslog(LOG_DEBUG, "[DM]: Zwave database open status fail");
		return false;
	} else {
		syslog(LOG_DEBUG, "[DM]: Zwave database open status success");
		return true;
	}
}

bool ZwMappingDatabase::ZwaveDatabaseClose() {

	int DMZwaveQuerryRet = sqlite3_close(DMZwaveDB);

	syslog(LOG_DEBUG, "[DM]: Zwave database close status: [%d]",
			DMZwaveQuerryRet);
	if (DMZwaveQuerryRet) {
		syslog(LOG_INFO, "[DM]: Zwave database close status fail");
		return false;
	} else {
		syslog(LOG_INFO, "[DM]: Zwave database close status success");
		return true;
	}
}

int ZwMappingDatabase::DMCommandIDCheck(int GENERIC_DEVICE_CLASS,
		int SPECIFIC_DEVICE_CLASS, int COMMAND_CLASS_ID) {

	int result = 0;
	sqlite3_stmt *SqliteStructZW;
	char* ZwSqlQuerry;

	asprintf(&ZwSqlQuerry,
			"SELECT COMMAND_CLASS_ID FROM CLASS WHERE GENERIC_DEVICE_CLASS = %d AND SPECIFIC_DEVICE_CLASS = %d AND COMMAND_CLASS_ID = %d;",
			GENERIC_DEVICE_CLASS, SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID);
	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping Query DM Sensor Event Type ZW : [%s]",
			ZwSqlQuerry);

	sqlite3_prepare_v2(DMZwaveDB, ZwSqlQuerry, strlen(ZwSqlQuerry),
			&SqliteStructZW,
			NULL);

	while (sqlite3_step(SqliteStructZW) == SQLITE_ROW) {
		result = sqlite3_column_int(SqliteStructZW, 0);
		break;
	}
	sqlite3_finalize(SqliteStructZW);
	free(ZwSqlQuerry);
	return result;

}

int ZwMappingDatabase::DMSensorEventTypeZW(int GENERIC_DEVICE_CLASS,
		int SPECIFIC_DEVICE_CLASS, int COMMAND_CLASS_ID, int SENSOR_TYPE,
		int EVENT_TYPE) {

	int result = 0;
	sqlite3_stmt *SqliteStructZW;
	char* ZwSqlQuerry;

	asprintf(&ZwSqlQuerry,
			"SELECT EVENT_TYPE FROM CLASS WHERE GENERIC_DEVICE_CLASS = %d AND SPECIFIC_DEVICE_CLASS = %d AND COMMAND_CLASS_ID = %d AND SENSOR_TYPE = %d AND EVENT_TYPE = %d;",
			GENERIC_DEVICE_CLASS, SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID,
			SENSOR_TYPE, EVENT_TYPE);
	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping DM Sensor Event Type ZW Query: [%s]",
			ZwSqlQuerry);

	sqlite3_prepare_v2(DMZwaveDB, ZwSqlQuerry, strlen(ZwSqlQuerry),
			&SqliteStructZW,
			NULL);

	while (sqlite3_step(SqliteStructZW) == SQLITE_ROW) {
		result = sqlite3_column_int(SqliteStructZW, 0);
		break;
	}
	sqlite3_finalize(SqliteStructZW);
	free(ZwSqlQuerry);
	return result;

}

std::string ZwMappingDatabase::DMThingNameZW(int GENERIC_DEVICE_CLASS,
		int SPECIFIC_DEVICE_CLASS, int COMMAND_CLASS_ID, int SENSOR_TYPE) {

	char* ZwSqlQuerry;
	sqlite3_stmt *SqliteStructZW;
	std::string result = "DM_DEFAULT";

	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping in dm thing name zw generic_device_class: [%d] specific_device_class : [%d] command_class_id : [%d] sensor_type : [%d]",
			GENERIC_DEVICE_CLASS,SPECIFIC_DEVICE_CLASS,COMMAND_CLASS_ID,SENSOR_TYPE);

	asprintf(&ZwSqlQuerry,
			"SELECT THING_NAME FROM CLASS WHERE GENERIC_DEVICE_CLASS = %d AND SPECIFIC_DEVICE_CLASS = %d AND COMMAND_CLASS_ID = %d AND SENSOR_TYPE = %d;",
			GENERIC_DEVICE_CLASS, SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID,
			SENSOR_TYPE);
	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping OUT ZW Manufacture Name: [%s]",
			ZwSqlQuerry);

	sqlite3_prepare_v2(DMZwaveDB, ZwSqlQuerry, strlen(ZwSqlQuerry),
			&SqliteStructZW,
			NULL);

	while (sqlite3_step(SqliteStructZW) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructZW, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZW);
	free(ZwSqlQuerry);
	return result;
}

std::string ZwMappingDatabase::DMPropertyNameEvent(int SPECIFIC_DEVICE_CLASS,
		int COMMAND_CLASS_ID, int PROPERTY_TYPE, int SENSOR_TYPE,
		int EVENT_TYPE) {

	sqlite3_stmt *SqliteStructZW;
	char* ZwSqlQuerry;
	std::string result = "DM_DEFAULT";

	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping IN DMPropertyNameEvent SPECIFIC_DEVICE_CLASS : [%d] COMMAND_CLASS_ID : [%d] PROPERTY_TYPE : [%d] SENSOR_TYPE : [%d] EVENT_TYPE : [%d]",
			SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID, PROPERTY_TYPE, SENSOR_TYPE,
			EVENT_TYPE);

	asprintf(&ZwSqlQuerry,
			"SELECT PROPERTY_NAME FROM CLASS WHERE SPECIFIC_DEVICE_CLASS = %d AND COMMAND_CLASS_ID = %d AND PROPERTY_TYPE = %d AND SENSOR_TYPE = %d AND EVENT_TYPE = %d;",
			SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID, PROPERTY_TYPE, SENSOR_TYPE,
			EVENT_TYPE);
	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping Query DM Property Event ZW: [%s]",
			ZwSqlQuerry);

	sqlite3_prepare_v2(DMZwaveDB, ZwSqlQuerry, strlen(ZwSqlQuerry),
			&SqliteStructZW,
			NULL);

	while (sqlite3_step(SqliteStructZW) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructZW, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZW);
	free(ZwSqlQuerry);
	return result;

}

std::string ZwMappingDatabase::DMThingTypeZW(int GENERIC_DEVICE_CLASS,
		int SPECIFIC_DEVICE_CLASS) {

	char* ZwSqlQuerry;
	sqlite3_stmt *SqliteStructZW;
	std::string result = "DM_DEFAULT";

	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping IN DM Thing Type ZW generic_device_class: [%d] SPECIFIC_DEVICE_CLASS : [%d]",
			GENERIC_DEVICE_CLASS, SPECIFIC_DEVICE_CLASS);

	asprintf(&ZwSqlQuerry,
			"SELECT THING_TYPE FROM CLASS WHERE GENERIC_DEVICE_CLASS = %d AND SPECIFIC_DEVICE_CLASS = %d ;",
			GENERIC_DEVICE_CLASS, SPECIFIC_DEVICE_CLASS);

	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping out ZW Manufacture Name: [%s]",
			ZwSqlQuerry);
	sqlite3_prepare_v2(DMZwaveDB, ZwSqlQuerry, strlen(ZwSqlQuerry),
			&SqliteStructZW,
			NULL);

	while (sqlite3_step(SqliteStructZW) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructZW, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZW);
	free(ZwSqlQuerry);
	return result;

}

std::string ZwMappingDatabase::DMEntityTypeZW(int GENERIC_DEVICE_CLASS,
		int SPECIFIC_DEVICE_CLASS, int COMMAND_CLASS_ID, int SENSOR_TYPE,
		int EVENT_TYPE) {

	sqlite3_stmt *SqliteStructZW;
	std::string result = "DM_DEFAULT";
	char* ZwSqlQuerry;

	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping IN DMEntityTypeZW GENERIC_DEVICE_CLASS : [%d] SPECIFIC_DEVICE_CLASS : [%d] COMMAND_CLASS_ID : [%d] SENSOR_TYPE : [%d]",
			GENERIC_DEVICE_CLASS, SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID,
			SENSOR_TYPE);

	asprintf(&ZwSqlQuerry,
			"SELECT ENTITY_TYPE FROM CLASS WHERE GENERIC_DEVICE_CLASS = %d AND SPECIFIC_DEVICE_CLASS = %d AND COMMAND_CLASS_ID = %d  AND SENSOR_TYPE = %d AND EVENT_TYPE = %d;",
			GENERIC_DEVICE_CLASS, SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID,
			SENSOR_TYPE, EVENT_TYPE);
	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping Query DM Entity Type ZW: [%s]",
			ZwSqlQuerry);

	sqlite3_prepare_v2(DMZwaveDB, ZwSqlQuerry, strlen(ZwSqlQuerry),
			&SqliteStructZW,
			NULL);

	while (sqlite3_step(SqliteStructZW) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructZW, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZW);
	free(ZwSqlQuerry);
	return result;

}

std::string ZwMappingDatabase::DMEntityNameZW(int GENERIC_DEVICE_CLASS,
		int SPECIFIC_DEVICE_CLASS, int COMMAND_CLASS_ID, int SENSOR_TYPE,
		int EVENT_TYPE) {

	sqlite3_stmt *SqliteStructZW;
	std::string result = "DM_DEFAULT";
	char* ZwSqlQuerry;

	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping in dm entity name zw generic_device_class : [%d] specific_device_class : [%d] command_class_id : [%d] sensor_type : [%d]",
			GENERIC_DEVICE_CLASS, SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID,
			SENSOR_TYPE);

	asprintf(&ZwSqlQuerry,
			"SELECT ENTITY_NAME FROM CLASS WHERE GENERIC_DEVICE_CLASS = %d AND SPECIFIC_DEVICE_CLASS = %d AND COMMAND_CLASS_ID = %d AND SENSOR_TYPE = %d AND EVENT_TYPE = %d;",
			GENERIC_DEVICE_CLASS, SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID,
			SENSOR_TYPE, EVENT_TYPE);

	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping Query DM Entity Name ZW: [%s]",
			ZwSqlQuerry);

	sqlite3_prepare_v2(DMZwaveDB, ZwSqlQuerry, strlen(ZwSqlQuerry),
			&SqliteStructZW,
			NULL);

	while (sqlite3_step(SqliteStructZW) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructZW, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZW);
	free(ZwSqlQuerry);
	return result;

}

int ZwMappingDatabase::DMEntitySecurityLevelZW(int GENERIC_DEVICE_CLASS,
		int SPECIFIC_DEVICE_CLASS, int COMMAND_CLASS_ID, int SENSOR_TYPE,
		int EVENT_TYPE) {

	sqlite3_stmt *SqliteStructZW;
	int result = 0;
	char* ZwSqlQuerry;

	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping IN DMEntitySecurityZW GENERIC_DEVICE_CLASS : [%d] SPECIFIC_DEVICE_CLASS : [%d] COMMAND_CLASS_ID : [%d] SENSOR_TYPE : [%d]",
			GENERIC_DEVICE_CLASS, SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID,
			SENSOR_TYPE);

	asprintf(&ZwSqlQuerry,
			"SELECT SECURITY_LEVEL FROM CLASS WHERE GENERIC_DEVICE_CLASS = %d AND SPECIFIC_DEVICE_CLASS = %d AND COMMAND_CLASS_ID = %d  AND SENSOR_TYPE = %d AND EVENT_TYPE = %d;",
			GENERIC_DEVICE_CLASS, SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID,
			SENSOR_TYPE, EVENT_TYPE);

	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping Query DM Entity Security ZW: [%s]",
			ZwSqlQuerry);

	sqlite3_prepare_v2(DMZwaveDB, ZwSqlQuerry, strlen(ZwSqlQuerry),
			&SqliteStructZW,
			NULL);

	while (sqlite3_step(SqliteStructZW) == SQLITE_ROW) {
		result = std::stoi(
				(const char*) sqlite3_column_text(SqliteStructZW, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZW);
	free(ZwSqlQuerry);
	return result;
}


int ZwMappingDatabase::DMPropertyTypeZW(int SPECIFIC_DEVICE_CLASS,
		int COMMAND_CLASS_ID, int SENSOR_TYPE) {

	int result = 0;
	sqlite3_stmt *SqliteStructZW;
	char* ZwSqlQuerry;

	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping in dm property type w specific_device_class : [%d] command_class_id : [%d] sensor_type : [%d]",
			SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID, SENSOR_TYPE);

	asprintf(&ZwSqlQuerry,
			"SELECT PROPERTY_TYPE FROM CLASS WHERE SPECIFIC_DEVICE_CLASS = %d AND COMMAND_CLASS_ID = %d AND SENSOR_TYPE = %d;",
			SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID, SENSOR_TYPE);

	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping OUT DM Property Type ZW: [%s]",
			ZwSqlQuerry);

	sqlite3_prepare_v2(DMZwaveDB, ZwSqlQuerry, strlen(ZwSqlQuerry),
			&SqliteStructZW,
			NULL);

	while (sqlite3_step(SqliteStructZW) == SQLITE_ROW) {
		result = sqlite3_column_int(SqliteStructZW, 0);
		break;
	}
	sqlite3_finalize(SqliteStructZW);
	free(ZwSqlQuerry);
	return result;

}

std :: string  ZwMappingDatabase::DMPropertyNameSensorZW(int SPECIFIC_DEVICE_CLASS,
		int COMMAND_CLASS_ID, int PROPERTY_TYPE, int SENSOR_TYPE) {

	std::string result = "DM_DEFAULT";
	sqlite3_stmt *SqliteStructZW;
	char* ZwSqlQuerry;

	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping in dm property type w specific_device_class : [%d] command_class_id : [%d] sensor_type : [%d]",
			SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID, SENSOR_TYPE);

	asprintf(&ZwSqlQuerry,
			"SELECT PROPERTY_NAME FROM CLASS WHERE SPECIFIC_DEVICE_CLASS = %d AND COMMAND_CLASS_ID = %d AND PROPERTY_TYPE = %d AND SENSOR_TYPE = %d;",
			SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID, PROPERTY_TYPE,SENSOR_TYPE);

	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping OUT DM Property Type ZW: [%s]",
			ZwSqlQuerry);

	sqlite3_prepare_v2(DMZwaveDB, ZwSqlQuerry, strlen(ZwSqlQuerry),
			&SqliteStructZW,
			NULL);

	while (sqlite3_step(SqliteStructZW) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructZW, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZW);
	free(ZwSqlQuerry);
	return result;




}

std::string ZwMappingDatabase::DMDOPropertyTypeZW(int SPECIFIC_DEVICE_CLASS,
		int COMMAND_CLASS_ID, int SENSOR_TYPE) {

	char* ZwSqlQuerry;
	sqlite3_stmt *SqliteStructZW;
	std::string result = "DM_DEFAULT";

	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping in dm property type zw specific_device_class : [%d] command_class_id : [%d] sensor_type : [%d]",
			SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID, SENSOR_TYPE);

	asprintf(&ZwSqlQuerry,
			"SELECT DISC_OBS_PROPERTY_TYPE FROM CLASS WHERE SPECIFIC_DEVICE_CLASS = %d AND COMMAND_CLASS_ID = %d AND SENSOR_TYPE = %d;",
			SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID, SENSOR_TYPE);
	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping Query DM Property Type ZW: [%s]",
			ZwSqlQuerry);

	sqlite3_prepare_v2(DMZwaveDB, ZwSqlQuerry, strlen(ZwSqlQuerry),
			&SqliteStructZW,
			NULL);

	while (sqlite3_step(SqliteStructZW) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructZW, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZW);
	free(ZwSqlQuerry);
	return result;

}

std::string ZwMappingDatabase::DMPropertyNameZW(int SPECIFIC_DEVICE_CLASS,
		int COMMAND_CLASS_ID, int PROPERTY_TYPE, int SENSOR_TYPE,
		int EVENT_TYPE) {

	sqlite3_stmt *SqliteStructZW;
	char* ZwSqlQuerry;
	std::string result = "DM_DEFAULT";

	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping IN DMPropertyNameZW SPECIFIC_DEVICE_CLASS : [%d] COMMAND_CLASS_ID : [%d] PROPERTY_TYPE : [%d] SENSOR_TYPE : [%d]",
			SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID, PROPERTY_TYPE,
			SENSOR_TYPE);

	asprintf(&ZwSqlQuerry,
			"SELECT PROPERTY_NAME FROM CLASS WHERE SPECIFIC_DEVICE_CLASS = %d AND COMMAND_CLASS_ID = %d AND PROPERTY_TYPE = %d AND SENSOR_TYPE = %d AND EVENT_TYPE = %d;",
			SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID, PROPERTY_TYPE, SENSOR_TYPE,
			EVENT_TYPE);
	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping Query DM Property Name ZW: [%s]",
			ZwSqlQuerry);

	sqlite3_prepare_v2(DMZwaveDB, ZwSqlQuerry, strlen(ZwSqlQuerry),
			&SqliteStructZW,
			NULL);

	while (sqlite3_step(SqliteStructZW) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructZW, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZW);
	free(ZwSqlQuerry);
	return result;

}

std::string ZwMappingDatabase::DMManufactureNameZW(int VERSION_ID, int PID,
		int CATEGORY, int PTYPE) {

	char* ZwSqlQuerry;
	sqlite3_stmt *SqliteStructZW;
	std::string result = "NO_DATABASE";

	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping in dm manufacture name zw version_id : [%d] pid : [%d] category : [%d] ptype : [%d]",
			VERSION_ID, PID, CATEGORY, PTYPE);

	asprintf(&ZwSqlQuerry,
			"SELECT MANUFACTURE_NAME FROM ZWAVEMANUFACTURENAME WHERE VERSION_ID = %d AND PID = %d AND CATEGORY = %d AND PTYPE = %d;",
			VERSION_ID, PID, CATEGORY, PTYPE);
	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping Query ZW Manufacture Name: [%s]",
			ZwSqlQuerry);

	sqlite3_prepare_v2(DMZwaveDB, ZwSqlQuerry, strlen(ZwSqlQuerry),
			&SqliteStructZW,
			NULL);

	while (sqlite3_step(SqliteStructZW) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructZW, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZW);
	free(ZwSqlQuerry);
	return result;
}

int ZwMappingDatabase::DMSensorTypeZW(int GENERIC_DEVICE_CLASS,
		int SPECIFIC_DEVICE_CLASS, int COMMAND_CLASS_ID, int SENSOR_TYPE) {

	int result = 0;
	sqlite3_stmt *SqliteStructZW;
	char* ZwSqlQuerry;
	asprintf(&ZwSqlQuerry,
			"SELECT SENSOR_TYPE FROM CLASS WHERE GENERIC_DEVICE_CLASS = %d AND SPECIFIC_DEVICE_CLASS = %d AND COMMAND_CLASS_ID = %d AND SENSOR_TYPE = %d;",
			GENERIC_DEVICE_CLASS, SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID,
			SENSOR_TYPE);
	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping Query DM Sensor Type ZW: [%s]",
			ZwSqlQuerry);

	sqlite3_prepare_v2(DMZwaveDB, ZwSqlQuerry, strlen(ZwSqlQuerry),
			&SqliteStructZW,
			NULL);

	while (sqlite3_step(SqliteStructZW) == SQLITE_ROW) {
		result = sqlite3_column_int(SqliteStructZW, 0);
		break;
	}
	sqlite3_finalize(SqliteStructZW);
	free(ZwSqlQuerry);
	return result;

}

std::string ZwMappingDatabase::DMPropertyRangeZW(int SPECIFIC_DEVICE_CLASS,
		int COMMAND_CLASS_ID, int PROPERTY_TYPE,  int SENSOR_TYPE) {

	char* ZwSqlQuerry;
	std::string result = "DM_DEFAULT";
	sqlite3_stmt *SqliteStructZW;

	asprintf(&ZwSqlQuerry,
			"SELECT PROPERTY_RANGE FROM CLASS WHERE SPECIFIC_DEVICE_CLASS = %d AND COMMAND_CLASS_ID = %d AND PROPERTY_TYPE = %d AND  SENSOR_TYPE = %d;",
			SPECIFIC_DEVICE_CLASS, COMMAND_CLASS_ID, PROPERTY_TYPE,SENSOR_TYPE);
	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping Query DM Property Range ZW: [%s]",
			ZwSqlQuerry);

	sqlite3_prepare_v2(DMZwaveDB, ZwSqlQuerry, strlen(ZwSqlQuerry),
			&SqliteStructZW,
			NULL);

	while (sqlite3_step(SqliteStructZW) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructZW, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZW);
	free(ZwSqlQuerry);
	return result;

}

int ZwMappingDatabase::DMCommandClassIDZW(int Device_class_id,
		int specific_class_id, std::string PROPERTY_NAME) {

	int result = 0;
	sqlite3_stmt *SqliteStructZW;
	char* ZwSqlQuerry;

	asprintf(&ZwSqlQuerry,
			"SELECT COMMAND_CLASS_ID FROM CLASS WHERE GENERIC_DEVICE_CLASS = %d AND SPECIFIC_DEVICE_CLASS = %d AND PROPERTY_NAME = '%s';",
			Device_class_id, specific_class_id, PROPERTY_NAME.c_str());
	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping Query DM Command Class ID ZW: [%s]",
			ZwSqlQuerry);

	sqlite3_prepare_v2(DMZwaveDB, ZwSqlQuerry, strlen(ZwSqlQuerry),
			&SqliteStructZW,
			NULL);

	while (sqlite3_step(SqliteStructZW) == SQLITE_ROW) {
		result = sqlite3_column_int(SqliteStructZW, 0);
		break;
	}
	sqlite3_finalize(SqliteStructZW);
	free(ZwSqlQuerry);
	return result;
}

int ZwMappingDatabase::DMAttributeTypeZW(std::string PROPERTY_NAME) {

	char* ZwSqlQuerry;
	int result = 0;
	sqlite3_stmt *SqliteStructZW;

	asprintf(&ZwSqlQuerry,
			"SELECT ATTRIBUTE_TYPE FROM CLASS WHERE  PROPERTY_NAME = '%s' ;",
			PROPERTY_NAME.c_str());
	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping Query DM attribute type ZW: [%s]",
			ZwSqlQuerry);

	sqlite3_prepare_v2(DMZwaveDB, ZwSqlQuerry, strlen(ZwSqlQuerry),
			&SqliteStructZW,
			NULL);

	while (sqlite3_step(SqliteStructZW) == SQLITE_ROW) {
		result = sqlite3_column_int(SqliteStructZW, 0);
		break;
	}
	sqlite3_finalize(SqliteStructZW);
	free(ZwSqlQuerry);
	return result;
}

int ZwMappingDatabase::DMErrorCodeZW(int zw_error_code) {

	char* ZwSqlQuerry;
	int result = 0;
	sqlite3_stmt *SqliteStructZW;

	asprintf(&ZwSqlQuerry,
			"SELECT DMERRORCODE FROM DMZWAVEERRORCODE WHERE ZWERRORCODE = %d ;",
			zw_error_code);
	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping Query DM errorCode ZW: [%s]",
			ZwSqlQuerry);

	sqlite3_prepare_v2(DMZwaveDB, ZwSqlQuerry, strlen(ZwSqlQuerry),
			&SqliteStructZW,
			NULL);

	while (sqlite3_step(SqliteStructZW) == SQLITE_ROW) {
		result = sqlite3_column_int(SqliteStructZW, 0);
		break;
	}
	sqlite3_finalize(SqliteStructZW);
	free(ZwSqlQuerry);
	return result;
}
std::string ZwMappingDatabase::DMErrorTypeZW(int zw_error_code) {

	char* ZwSqlQuerry;
	std::string result = "DM_DEFAULT";
	sqlite3_stmt *SqliteStructZW;

	asprintf(&ZwSqlQuerry,
			"SELECT DMERRORTYPE FROM DMZWAVEERRORCODE WHERE ZWERRORCODE = %d ;",
			zw_error_code);

	syslog(LOG_DEBUG,
			"[DM]: Zwave database mapping Query DM errorType  ZW: [%s]",
			ZwSqlQuerry);

	sqlite3_prepare_v2(DMZwaveDB, ZwSqlQuerry, strlen(ZwSqlQuerry),
			&SqliteStructZW,
			NULL);

	while (sqlite3_step(SqliteStructZW) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructZW, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZW);
	free(ZwSqlQuerry);
	return result;

}

