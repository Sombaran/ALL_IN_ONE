#include "zb_mapping_database.hpp"

static sqlite3 *DMZigbeeDB;

/*ZbMappingDatabase::ZbMappingDatabase() {
}
ZbMappingDatabase::~ZbMappingDatabase() {
}*/

bool ZbMappingDatabase::ZigbeeDatabaseOpen() {

	int DMZigbeeQuerryRet;
	DMZigbeeQuerryRet = sqlite3_open("ZigbeeDatabaseMap.db", &DMZigbeeDB);

	if (DMZigbeeQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Zigbee database mapping opening failed: [%s] error: [%d]",
				sqlite3_errmsg(DMZigbeeDB), DMZigbeeQuerryRet);
		return false;
	} else {
		syslog(LOG_INFO, "[DM]: Things backup database opening success");
		return true;
	}
	return false;
}

bool ZbMappingDatabase::ZigbeeDatabaseClose() {

	int DMZigbeeQuerryRet;
	DMZigbeeQuerryRet = sqlite3_close(DMZigbeeDB);
	if (DMZigbeeQuerryRet != SQLITE_OK) {
		syslog(LOG_ERR,
				"[DM]: Zigbee database mapping closing failed: [%s] error: [%d]",
				sqlite3_errmsg(DMZigbeeDB), DMZigbeeQuerryRet);
		return false;
	} else {
		syslog(LOG_INFO, "[DM]: Things backup database closing success");
		return true;
	}
	return false;
}

std::string ZbMappingDatabase::DMThingTypeZB(int DEVICE_ID, int ZONE_ID) {

	sqlite3_stmt *SqliteStructZB;
	char* ZbSqlQuerry;
	std::string result = "DM_DEFAULT";

	syslog(LOG_DEBUG,
			"[DM]: Zigbee database mapping thing type device_id: [%d] zone_id: [%d]",
			DEVICE_ID, ZONE_ID);

	asprintf(&ZbSqlQuerry,
			"SELECT THING_TYPE FROM HADATABASE WHERE DEVICE_ID = %d AND ZONE_ID = %d;",
			DEVICE_ID, ZONE_ID);

	sqlite3_prepare_v2(DMZigbeeDB, ZbSqlQuerry, strlen(ZbSqlQuerry),
			&SqliteStructZB,
			NULL);

	while (sqlite3_step(SqliteStructZB) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructZB, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZB);
	free(ZbSqlQuerry);
	return result;
}

std::string ZbMappingDatabase::DMThingNameZB(int DEVICE_ID, int ZONE_ID) {

	sqlite3_stmt *SqliteStructZB;
	char* ZbSqlQuerry;
	std::string result = "DM_DEFAULT";

	syslog(LOG_DEBUG,
			"[DM]: Zigbee database mapping thing name device_id: [%d] zone_id: [%d]",
			DEVICE_ID, ZONE_ID);

	asprintf(&ZbSqlQuerry,
			"SELECT THING_NAME FROM HADATABASE WHERE DEVICE_ID = %d AND ZONE_ID = %d;",
			DEVICE_ID, ZONE_ID);

	sqlite3_prepare_v2(DMZigbeeDB, ZbSqlQuerry, strlen(ZbSqlQuerry),
			&SqliteStructZB,
			NULL);

	while (sqlite3_step(SqliteStructZB) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructZB, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZB);
	free(ZbSqlQuerry);
	return result;
}

std::string ZbMappingDatabase::DMEntityTypeZB(int DEVICE_ID, int ZONE_ID,
		int CLUSTER_ID) {

	sqlite3_stmt *SqliteStructZB;
	char* ZbSqlQuerry;
	std::string result = "DM_DEFAULT";

	syslog(LOG_DEBUG,
			"[DM]: Zigbee database mapping entity type device_id: [%d] zone_id: [%d] cluster_id [%d]",
			DEVICE_ID, ZONE_ID, CLUSTER_ID);

	asprintf(&ZbSqlQuerry,
			"SELECT ENTITY_TYPE FROM HADATABASE WHERE DEVICE_ID = %d AND ZONE_ID = %d AND CLUSTER_ID = %d;",
			DEVICE_ID, ZONE_ID, CLUSTER_ID);

	sqlite3_prepare_v2(DMZigbeeDB, ZbSqlQuerry, strlen(ZbSqlQuerry),
			&SqliteStructZB,
			NULL);

	while (sqlite3_step(SqliteStructZB) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructZB, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZB);
	free(ZbSqlQuerry);
	return result;
}

int ZbMappingDatabase::DMEntitySecurityLevelZB(int DEVICE_ID, int ZONE_ID,
		int CLUSTER_ID) {
	sqlite3_stmt *SqliteStructZB;
	char* ZbSqlQuerry;
	int result = 0;
	syslog(LOG_DEBUG,
			"[DM]: Zigbee database mapping entity security level device_id: [%d] zone_id: [%d] cluster_id [%d]",
			DEVICE_ID, ZONE_ID, CLUSTER_ID);

	asprintf(&ZbSqlQuerry,
			"SELECT SECURITY_LEVEL FROM HADATABASE WHERE DEVICE_ID = %d AND ZONE_ID = %d AND CLUSTER_ID = %d;",
			DEVICE_ID, ZONE_ID, CLUSTER_ID);

	sqlite3_prepare_v2(DMZigbeeDB, ZbSqlQuerry, strlen(ZbSqlQuerry),
			&SqliteStructZB,
			NULL);

	while (sqlite3_step(SqliteStructZB) == SQLITE_ROW) {
		result = std::stoi(
				(const char*) sqlite3_column_text(SqliteStructZB, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZB);
	free(ZbSqlQuerry);
	return result;
}

std::string ZbMappingDatabase::DMEntityNameZB(int DEVICE_ID, int ZONE_ID,
		int CLUSTER_ID) {

	sqlite3_stmt *SqliteStructZB;
	char* ZbSqlQuerry;
	std::string result = "DM_DEFAULT";

	syslog(LOG_DEBUG,
			"[DM]: Zigbee database mapping entity name device_id: [%d] zone_id: [%d], cluster_id [%d]",
			DEVICE_ID, ZONE_ID, CLUSTER_ID);

	asprintf(&ZbSqlQuerry,
			"SELECT ENTITY_NAME FROM HADATABASE WHERE DEVICE_ID = %d AND ZONE_ID = %d AND CLUSTER_ID = %d;",
			DEVICE_ID, ZONE_ID, CLUSTER_ID);

	sqlite3_prepare_v2(DMZigbeeDB, ZbSqlQuerry, strlen(ZbSqlQuerry),
			&SqliteStructZB,
			NULL);

	while (sqlite3_step(SqliteStructZB) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructZB, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZB);
	free(ZbSqlQuerry);
	return result;
}

std::string ZbMappingDatabase::DMPropertyNameZB(int DEVICE_ID, int COMMAND_ID,
		int ZONE_ID, int CLUSTER_ID) {

	sqlite3_stmt *SqliteStructZB;
	std::string result = "DM_DEFAULT";
	char* ZbSqlQuerry;

	syslog(LOG_DEBUG,
			"[DM]: Zigbee database mapping property name device_id: [%d] command_id: [%d] zone_id: [%d] cluster_id: [%d]",
			DEVICE_ID, COMMAND_ID, ZONE_ID, CLUSTER_ID);

	result = "DM_DEFAULT";

	asprintf(&ZbSqlQuerry,
			"SELECT PROPERTY_NAME FROM HADATABASE WHERE DEVICE_ID = %d AND COMMAND_ID = %d AND ZONE_ID = %d AND CLUSTER_ID = %d;",
			DEVICE_ID, COMMAND_ID, ZONE_ID, CLUSTER_ID);

	sqlite3_prepare_v2(DMZigbeeDB, ZbSqlQuerry, strlen(ZbSqlQuerry),
			&SqliteStructZB,
			NULL);

	while (sqlite3_step(SqliteStructZB) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructZB, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZB);
	free(ZbSqlQuerry);
	return result;
}

std::string ZbMappingDatabase::DMPropertyRangeZB(int DEVICE_ID, int COMMAND_ID,
		int ZONE_ID, int CLUSTER_ID) {

	sqlite3_stmt *SqliteStructZB;
	std::string result = "DM_DEFAULT";
	char* ZbSqlQuerry;

	result = "DM_DEFAULT";

	syslog(LOG_DEBUG,
			"[DM]: Zigbee database mapping property range device_id: [%d] command_id: [%d] zone_id: [%d] cluster_id: [%d]",
			DEVICE_ID, COMMAND_ID, ZONE_ID, CLUSTER_ID);

	asprintf(&ZbSqlQuerry,
			"SELECT PROPERTY_RANGE FROM HADATABASE WHERE DEVICE_ID = %d AND COMMAND_ID = %d AND ZONE_ID = %d AND CLUSTER_ID = %d;",
			DEVICE_ID, COMMAND_ID, ZONE_ID, CLUSTER_ID);

	sqlite3_prepare_v2(DMZigbeeDB, ZbSqlQuerry, strlen(ZbSqlQuerry),
			&SqliteStructZB,
			NULL);

	while (sqlite3_step(SqliteStructZB) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructZB, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZB);
	free(ZbSqlQuerry);
	return result;
}

//----------------------

int ZbMappingDatabase::DMCommandIDCheckZB(int DEVICE_ID, int CLUSTER_ID,
		int COMMAND_ID) {

	sqlite3_stmt *SqliteStructZB;

	char* ZbSqlQuerry;
	int result = 0;

	syslog(LOG_DEBUG,
			"[DM]: Zigbee database mapping command id check command_id: [%d] device_id: [%d] cluster_id: [%d]",
			COMMAND_ID, DEVICE_ID, CLUSTER_ID);

	asprintf(&ZbSqlQuerry,
			"SELECT COMMAND_ID FROM HADATABASE WHERE DEVICE_ID = %d AND CLUSTER_ID = %d AND COMMAND_ID = %d;",
			DEVICE_ID, CLUSTER_ID, COMMAND_ID);

	sqlite3_prepare_v2(DMZigbeeDB, ZbSqlQuerry, strlen(ZbSqlQuerry),
			&SqliteStructZB,
			NULL);

	while (sqlite3_step(SqliteStructZB) == SQLITE_ROW) {
		result = sqlite3_column_int(SqliteStructZB, 0);
		break;
	}
	sqlite3_finalize(SqliteStructZB);
	free(ZbSqlQuerry);
	return result;
}

int ZbMappingDatabase::DMPropertyTypeZB(int DEVICE_ID, int COMMAND_ID,
		int ZONE_ID, int CLUSTER_ID) {

	sqlite3_stmt *SqliteStructZB;
	char* ZbSqlQuerry;
	int result = 0;

	syslog(LOG_DEBUG,
			"[DM]: Zigbee database mapping command id check device_id: [%d] command_id: [%d] zone_id: [%d] cluster_id: [%d]",
			DEVICE_ID, COMMAND_ID, ZONE_ID, CLUSTER_ID);

	asprintf(&ZbSqlQuerry,
			"SELECT PROPERTY_TYPE FROM HADATABASE WHERE DEVICE_ID = %d AND COMMAND_ID = %d AND ZONE_ID = %d AND CLUSTER_ID = %d;",
			DEVICE_ID, COMMAND_ID, ZONE_ID, CLUSTER_ID);

	sqlite3_prepare_v2(DMZigbeeDB, ZbSqlQuerry, strlen(ZbSqlQuerry),
			&SqliteStructZB,
			NULL);

	while (sqlite3_step(SqliteStructZB) == SQLITE_ROW) {
		result = sqlite3_column_int(SqliteStructZB, 0);
		break;
	}
	sqlite3_finalize(SqliteStructZB);
	free(ZbSqlQuerry);
	return result;
}

int ZbMappingDatabase::DMDeviceIDZB(std::string THING_NAME) {

	sqlite3_stmt *SqliteStructZB;
	char* ZbSqlQuerry;
	int result = 0;

	syslog(LOG_DEBUG, "[DM]: Zigbee database mapping thing name: [%s]",
			THING_NAME.c_str());

	asprintf(&ZbSqlQuerry,
			"SELECT DEVICE_ID FROM HADATABASE WHERE THING_NAME = '%s';",
			THING_NAME.c_str());

	sqlite3_prepare_v2(DMZigbeeDB, ZbSqlQuerry, strlen(ZbSqlQuerry),
			&SqliteStructZB,
			NULL);

	while (sqlite3_step(SqliteStructZB) == SQLITE_ROW) {
		result = sqlite3_column_int(SqliteStructZB, 0);
		break;
	}
	sqlite3_finalize(SqliteStructZB);
	free(ZbSqlQuerry);
	return result;
}

int ZbMappingDatabase::DMClusterIDZB(std::string PROPERTY_NAME) {

	sqlite3_stmt *SqliteStructZB;
	char* ZbSqlQuerry;
	int result = 0;

	syslog(LOG_DEBUG, "[DM]: Zigbee database mapping property name: [%s]",
			PROPERTY_NAME.c_str());

	asprintf(&ZbSqlQuerry,
			"SELECT CLUSTER_ID FROM HADATABASE WHERE PROPERTY_NAME = '%s';",
			PROPERTY_NAME.c_str());

	sqlite3_prepare_v2(DMZigbeeDB, ZbSqlQuerry, strlen(ZbSqlQuerry),
			&SqliteStructZB,
			NULL);

	while (sqlite3_step(SqliteStructZB) == SQLITE_ROW) {
		result = sqlite3_column_int(SqliteStructZB, 0);
		break;
	}
	sqlite3_finalize(SqliteStructZB);
	free(ZbSqlQuerry);
	return result;
}

std::string ZbMappingDatabase::DMAtrTypeIDZB(std::string PROPERTY_NAME) {
	syslog(LOG_INFO, "[DM]: Zigbee database mapping IN DMAtrTypeIDZB");
	sqlite3_stmt *SqliteStructZB;
	std::string result = "NO_DATABASE";

	char* ZbSqlQuerry;

	syslog(LOG_DEBUG, "[DM]: Zigbee database mapping property name: [%s]",
			PROPERTY_NAME.c_str());

	asprintf(&ZbSqlQuerry,
			"SELECT ATTRIBUTE_TYPE FROM HADATABASE WHERE PROPERTY_NAME = '%s';",
			PROPERTY_NAME.c_str());

	sqlite3_prepare_v2(DMZigbeeDB, ZbSqlQuerry, strlen(ZbSqlQuerry),
			&SqliteStructZB,
			NULL);

	while (sqlite3_step(SqliteStructZB) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructZB, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZB);
	free(ZbSqlQuerry);
	return result;
}

int ZbMappingDatabase::DMommandIDZB(std::string PROPERTY_NAME) {

	sqlite3_stmt *SqliteStructZB;
	char* ZbSqlQuerry;
	int result = 0;

	syslog(LOG_DEBUG, "[DM]: Zigbee database mapping property name: [%s]",
			PROPERTY_NAME.c_str());

	asprintf(&ZbSqlQuerry,
			"SELECT COMMAND_ID FROM HADATABASE WHERE PROPERTY_NAME = '%s';",
			PROPERTY_NAME.c_str());

	sqlite3_prepare_v2(DMZigbeeDB, ZbSqlQuerry, strlen(ZbSqlQuerry),
			&SqliteStructZB,
			NULL);

	while (sqlite3_step(SqliteStructZB) == SQLITE_ROW) {
		result = sqlite3_column_int(SqliteStructZB, 0);
		break;
	}
	sqlite3_finalize(SqliteStructZB);
	free(ZbSqlQuerry);
	return result;
}

int ZbMappingDatabase::DMErrorCodeZB(int zb_error_code) {

	sqlite3_stmt *SqliteStructZB;
	char* ZbSqlQuerry;
	int result = 0;

	syslog(LOG_DEBUG, "[DM]: Zigbee database mapping error code: [%d]",
			zb_error_code);

	asprintf(&ZbSqlQuerry,
			"SELECT DMERRORCODE FROM ZIGBEEDMERRORCODE WHERE ZIGBEEERRORCODE = %d;",
			zb_error_code);

	syslog(LOG_DEBUG, "[DM]: ZIGBEE DM ERROR CODE mapping Query: [%s]",
			ZbSqlQuerry);

	sqlite3_prepare_v2(DMZigbeeDB, ZbSqlQuerry, strlen(ZbSqlQuerry),
			&SqliteStructZB,
			NULL);

	while (sqlite3_step(SqliteStructZB) == SQLITE_ROW) {
		result = sqlite3_column_int(SqliteStructZB, 0);
		break;
	}

	sqlite3_finalize(SqliteStructZB);
	free(ZbSqlQuerry);
	return result;
}

std::string ZbMappingDatabase::DMErrorTypeZB(int zb_error_code) {

	sqlite3_stmt *SqliteStructZB;
	char* ZbSqlQuerry;
	std::string result = "DM_DEFAULT";

	syslog(LOG_DEBUG, "[DM]: Zigbee database mapping error code: [%d]",
			zb_error_code);

	asprintf(&ZbSqlQuerry,
			"SELECT ZBErrorMessage FROM ZIGBEEDMERRORCODE WHERE ZIGBEEERRORCODE = %d;",
			zb_error_code);

	syslog(LOG_DEBUG, "[DM]: ZIGBEE DM ERROR CODE mapping Query: [%s]",
			ZbSqlQuerry);

	sqlite3_prepare_v2(DMZigbeeDB, ZbSqlQuerry, strlen(ZbSqlQuerry),
			&SqliteStructZB,
			NULL);

	while (sqlite3_step(SqliteStructZB) == SQLITE_ROW) {
		result = std::string(
				(const char*) sqlite3_column_text(SqliteStructZB, 0));
		break;
	}
	sqlite3_finalize(SqliteStructZB);
	free(ZbSqlQuerry);
	return result;
}

