/*
 * FOSCAM_DBSqlite3.c
 *
 *  Created on: 07-Jun-2018
 *      Author: Kandula Madhukar
 */

#ifdef UNIX
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <json-c/json.h>

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdint.h>
#include <stdbool.h>

sqlite3 *db;
char *zErrMsg = 0;
int rc;
const char *CreateDevice;
char *CreateCluster;
char *CreateAttribute;
char* result;
char sql[100];
char resultRet[50];

static int callback(void *data, int argc, char **argv, char **azColName) {
	int i;

	for (i = 0; i < argc; i++) {
		strcpy(resultRet, argv[i]);
		result = resultRet;
	}
	printf("\n*******\nRESULT DATABASE(callback) : %s\n******\n", result);
	return 0;
}

bool DeleteRecord(char * Mac) {
	const char* data = "Callback function called";
	rc = sqlite3_open("/exe_dir/FoscamDataBase.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return false;
	}
	sprintf(sql, "DELETE FROM FOSCAM WHERE MACID = \"%s\";", Mac);
	rc = sqlite3_exec(db, sql, callback, (void*) data, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return false;
	}
	sqlite3_close(db);

	return true;
}

char *GetIpAddress(char * Mac) {
	result = NULL;
	const char* data = "Callback function called";
	rc = sqlite3_open("/exe_dir/FoscamDataBase.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return " ";
	}
	sprintf(sql, "SELECT LOCALIP FROM FOSCAM WHERE MACID = \"%s\";", Mac);
	rc = sqlite3_exec(db, sql, callback, (void*) data, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);

	return result;
}

bool CheckPortNumberExistOrNot(int port) {
	result = NULL;
	const char* data = "Callback function called";
	rc = sqlite3_open("/exe_dir/FoscamDataBase.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return false;
	}
	
	sprintf(sql, "SELECT EXTERNALPORT FROM FOSCAM WHERE EXTERNALPORT = \"%d\";", port);
	rc = sqlite3_exec(db, sql, callback, (void*) data, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);
	printf("CheckPortNumberExistOrNot: %s\n",result);
	
	if(result == NULL)
		return true;//Not found external port
	return false;
/*	char buf[5]="";*/
/*	sprintf(buf,"%d",port);*/
/*	*/
/*	return !strcmp(buf, result);;*/
}

bool MacIDPresence(char *Mac) {
	result = "8585858";
	const char* data = "Callback function called";
	rc = sqlite3_open("/exe_dir/FoscamDataBase.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return false;
	}
	sprintf(sql, "SELECT MACID FROM FOSCAM WHERE MACID = \"%s\";", Mac);
	rc = sqlite3_exec(db, sql, callback, (void*) data, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);
	return !strcmp(Mac, result);
}

/*static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
 int i;
 for (i = 0; i < argc; i++) {
 printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
 }
 printf("\n");
 return 0;
 }
 */
int CreateTable(void) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;

	/* Open database */
	rc = sqlite3_open("/exe_dir/FoscamDataBase.db", &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return (0);
	} else {
		fprintf(stdout, "Opened database successfully\n");
	}

	/* Create SQL statement */
	sql = "CREATE TABLE FOSCAM("
			"MACID         TEXT    NOT NULL,"
			"LOCALIP        TEXT    NOT NULL,"
			"INTERNALPORT        INT     NOT NULL,"
			"EXTERNALPORT        INT     NOT NULL);";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Table created successfully\n");
	}
	sqlite3_close(db);
	return 0;
}

int InsertTable(const char *macID, const char *localIP, int internal_port,
		int external_port) {

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char sql[255] = "";
	if(MacIDPresence(macID))
	{
		printf("Record already exist with this MAC: %s\n",macID);
		return false;
	}
	/* Open database */
	rc = sqlite3_open("/exe_dir/FoscamDataBase.db", &db);

	if (rc) {
		fprintf(stderr, "Can't open FoscamDataBase: %s\n", sqlite3_errmsg(db));
		return (0);
	} else {
		fprintf(stderr,
				"Opened FoscamDataBase successfully for inserting json data\n");
	}

	/* Create SQL statement */

	sprintf(sql,
			"INSERT INTO FOSCAM(MACID, LOCALIP, INTERNALPORT, EXTERNALPORT)    VALUES ('%s','%s','%d','%d');",
			macID, localIP, internal_port, external_port);

	printf("%s\n", sql);

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Records created successfully\n");
	}
	sqlite3_close(db);
	return 0;
}

