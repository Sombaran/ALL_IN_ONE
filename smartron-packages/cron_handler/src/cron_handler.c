/*************************************************************************************************************************************
* FILENAME    :        cron_handler.c
*
* DESCRIPTION :
*		
*
*
* AUTHOR :  HARENDRA GAUTAM
*
* VERSION:  0.1
*
* Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
************************************************************************************************************************************/

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <json-c/json.h>
#include <unistd.h>
#include <zmq.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <sys/types.h>
#include <sys/syslog.h>
#include <stdbool.h>
#include "cron_handler.h"
#include "fw-env.h"

void *zmq_context_cron;
void *zmq_requester_cron;

int max_rule_count = MAX_RULE_COUNT;

RETURN_STATUS return_status;

void va_pub_socket_ack(RETURN_STATUS status_code, char *NotifId)
{
	int DataLen = 0;
	json_object *JsonSocRespobj;

	JsonSocRespobj = json_object_new_object();
	json_object_object_add(JsonSocRespobj, "status_code", json_object_new_int(status_code));
	if( status_code == SUCCESSFUL )
		json_object_object_add(JsonSocRespobj, "message", json_object_new_string("REQUEST RECEIVED SUCCESSFULLY"));
	else if( status_code == BAD_REQUEST )
		json_object_object_add(JsonSocRespobj, "message", json_object_new_string("RECEIVED BAD REQUEST"));
	else if( status_code == EXCEEDS_LIMIT )
		json_object_object_add(JsonSocRespobj, "message", json_object_new_string("EXCEEDING LIMIT"));
	else
		json_object_object_add(JsonSocRespobj, "message", json_object_new_string("FAILED TO PROCESS"));

	if(NotifId == NULL)
		sprintf(NotifId,"NONE");
	json_object_object_add(JsonSocRespobj, "notif_id", json_object_new_string(NotifId));

	DataLen = (int)strlen(json_object_get_string(JsonSocRespobj));
	zmq_send (zmq_requester_cron, json_object_get_string(JsonSocRespobj), DataLen, 0);
	syslog(LOG_DEBUG, "VA:PSOC Status codeL %d, RESPONSE TO CLIENT: %s",status_code, json_object_get_string(JsonSocRespobj));
	json_object_put(JsonSocRespobj);
}


int pub_soc_rule_db_handler(json_object *ClientDataObj, const char *RuleAction)
{
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char* query = NULL;
	int rc, RuleCount = 0;
	bool RuleAlreadyExisted = false, UpdateRuleDB =false;
    json_object *CronPatternObj, *RuleId;
    
    //syslog(LOG_INFO, "CRON:  Entering pub_soc_rule_db_handler()");

    json_object_object_get_ex(ClientDataObj, "rule_id", &RuleId);
    if(RuleId == NULL) {
		syslog(LOG_ERR, "CRON: RuleId is NULL");
		return FAILED_TO_PROCESS;
	}

	/************************************************************************/
	/*
	 * open SQLite database CRON_DB_LOCATION
	 */
	rc = sqlite3_open(CRON_DB_LOCATION, &db);
	if (rc != SQLITE_OK) {
		syslog(LOG_ERR, "CRON: ERROR opening SQLite DB in memory: %s", sqlite3_errmsg(db));
		return FAILED_TO_PROCESS;
	}

	if(strcmp(RuleAction,"CREATE") == 0) {
		
		asprintf(&query, "SELECT * FROM RULES WHERE RuleId = '%s';",json_object_get_string(RuleId));
		sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			RuleAlreadyExisted = true;
		}
		sqlite3_finalize(stmt);
		
			asprintf(&query, "SELECT * FROM RULES;");
			sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				RuleCount++;
			}
			sqlite3_finalize(stmt);
			
			if (RuleCount < max_rule_count) {
				//syslog(LOG_INFO, "CRON: : Rule Count : %d",RuleCount);
				json_object_object_get_ex(ClientDataObj, "cron_pattern", &CronPatternObj);
				if(CronPatternObj != NULL) {
					//syslog(LOG_INFO, "CRON:  Inserting Rule in VA DB");
					asprintf(&query, "INSERT INTO RULES (RuleId, RuleCronJob) VALUES ('%s','%s');", json_object_get_string(RuleId), json_object_get_string(CronPatternObj));
					UpdateRuleDB = true;
				}
				else {
					syslog(LOG_ERR, "CRON: CronPatternObj for Rule is NULL");
					return_status = FAILED_TO_PROCESS;
				}
			}
			else {
				syslog(LOG_ERR, "CRON: Rule Count Exceeded: %d : Can't create Rule",RuleCount);
				return_status = EXCEEDS_LIMIT;
			}

	}

	else if(strcmp(RuleAction,"MODIFY") == 0) {
		json_object_object_get_ex(ClientDataObj, "cron_pattern", &CronPatternObj);
		if(CronPatternObj != NULL) {
			//syslog(LOG_INFO, "CRON: Updating Rule in VA DB");
			asprintf(&query, "UPDATE RULES SET RuleCronJob = '%s' WHERE RuleId = '%s';", json_object_get_string(CronPatternObj), json_object_get_string(RuleId));
			UpdateRuleDB = true;
		}
		else {
			syslog(LOG_ERR, "CRON:  CronPatternObj for Rule is NULL");
			return_status = FAILED_TO_PROCESS;
		}
	}

	else if(strcmp(RuleAction,"DELETE") == 0) {
		//syslog(LOG_INFO, "CRON: Deleting Rule in VA DB");
		asprintf(&query, "DELETE FROM RULES WHERE RuleId = '%s';",json_object_get_string(RuleId));
		UpdateRuleDB = true;
	}


	if ( UpdateRuleDB == true)	{
		sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
		rc = sqlite3_step(stmt);
		if (rc != SQLITE_DONE) {
			return_status = FAILED_TO_PROCESS;
			syslog(LOG_ERR, "CRON: ERROR Failed to INSERT/UPDATE/DELETE in DB for RULES: %s", sqlite3_errmsg(db));
		}
		else {
			return_status = shifu_update_cron();
		}
		sqlite3_finalize(stmt);
		free(query);
	}


	/*
	 * close SQLite database
	 */
	rc = sqlite3_close(db);
	//syslog(LOG_INFO, "CRON: Rule changes are performed: sqlite3_close: %d",rc);

	/************************************************************************/
	//syslog(LOG_INFO, "CRON: Leaving pub_soc_rule_db_handler()");
    return return_status;
}



int pub_soc_rule_handler(json_object *ClientDataObj)
{
    json_object *VerbObj;
    json_object *NotifIdObj;
    //syslog(LOG_INFO, "CRON: Entering pub_soc_rule_handler()");

    json_object_object_get_ex(ClientDataObj, "verb", &VerbObj);
	if (VerbObj!=NULL) {
		if(strcmp(json_object_get_string(VerbObj), "CREATE") == 0) {
			return_status = pub_soc_rule_db_handler(ClientDataObj, "CREATE");
		}
		else if(strcmp(json_object_get_string(VerbObj), "MODIFY") == 0) {
			return_status = pub_soc_rule_db_handler(ClientDataObj, "MODIFY");
		}
		else if(strcmp(json_object_get_string(VerbObj), "DELETE") == 0) {
			return_status = pub_soc_rule_db_handler(ClientDataObj, "DELETE");
		}
		else {
			syslog(LOG_ERR, "CRON: UNKNOWN VERB FOR RULES in pub_soc_rule_handler");
			return_status = FAILED_TO_PROCESS;
		}
	}
	else {
		syslog(LOG_ERR, "CRON: VerbObj is NULL in pub_soc_rule_handler");
		return_status = FAILED_TO_PROCESS;
	}
	//syslog(LOG_INFO, "CRON: Leaving pub_soc_rule_handler()");

	return return_status;
}



int pub_soc_fota_update_handler(json_object *ClientDataObj)
{
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char* query = NULL;
    json_object *CronPatternObj;
    json_object *ExecutableObj;
    int rc;
    bool FotaId_existed = false;
    json_object_object_get_ex(ClientDataObj, "cron_pattern", &CronPatternObj);
    json_object_object_get_ex(ClientDataObj, "executable_string", &ExecutableObj);
    
    if((CronPatternObj!=NULL) && (ExecutableObj!=NULL)) {
		/************************************************************************/
		/*
		 * open SQLite database CRON_DB_LOCATION
		 */
		rc = sqlite3_open(CRON_DB_LOCATION, &db);
		if (rc != SQLITE_OK) {
			syslog(LOG_ERR, "CRON: ERROR opening SQLite DB in memory: %s", sqlite3_errmsg(db));
			return FAILED_TO_PROCESS;
		}

		asprintf(&query, "SELECT * FROM FOTA_INFO WHERE FotaId = 1;");
		sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			FotaId_existed = true;
			//syslog(LOG_INFO, "CRON: FotaId existed already existed");
		}
		sqlite3_finalize(stmt);
		

		if(FotaId_existed == false)	{
			syslog(LOG_DEBUG, "CRON: Fota Job Not existed, Inserting");		
			asprintf(&query, "INSERT INTO FOTA_INFO (FotaId,FotaCronJob, FotaExecutable)	\
										VALUES ('%d','%s', '%s');", 1,json_object_get_string(CronPatternObj), json_object_get_string(ExecutableObj));			
		}
		else if (FotaId_existed == true) {
			syslog(LOG_DEBUG, "CRON: FotaId existed already existed, Updating");
			asprintf(&query, "UPDATE FOTA_INFO SET FotaCronJob = '%s' , FotaExecutable = '%s' \
								WHERE FotaId = '%d';", json_object_get_string(CronPatternObj), json_object_get_string(ExecutableObj),1);			
		}
		sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
		rc = sqlite3_step(stmt);
		if (rc != SQLITE_DONE) {
			return_status = FAILED_TO_PROCESS;
			syslog(LOG_ERR, "CRON: ERROR while Inserting in Alert Tones Table for Reminder: %s", sqlite3_errmsg(db));
		}
		else
			return_status = SUCCESSFUL;
		sqlite3_finalize(stmt);	
			
		free(query);

		/*
		 * close SQLite database
		 */
		rc = sqlite3_close(db);
		//syslog(LOG_INFO, "CRON: Fota Configuration updated: sqlite3_close: %d",rc);
		
		if(return_status == SUCCESSFUL)
			return_status = shifu_update_cron();
		
		/************************************************************************/
	}
	else {
		return_status = FAILED_TO_PROCESS;
	}
	return return_status;	
}





int pub_soc_dm_handler(json_object *JsonSocobj, char *NotifId)
{
    json_object *ClientActionObj;
    json_object *ClientDataObj;
    json_object *NotifIdObj;
    json_object_object_get_ex(JsonSocobj, "client_action", &ClientActionObj);
    json_object_object_get_ex(JsonSocobj, "client_data", &ClientDataObj);
	if (ClientDataObj!=NULL) {
		json_object_object_get_ex(ClientDataObj, "notif_id", &NotifIdObj);
		if (NotifIdObj!=NULL)
			sprintf(NotifId,"%s",json_object_get_string(NotifIdObj));
		else
			sprintf(NotifId,"NONE");
	}
	else {
		syslog(LOG_ERR, "CRON: ClientDataObj is NULL in pub_soc_dm_handler");
		sprintf(NotifId,"NONE");
		return FAILED_TO_PROCESS;
	}
	if (ClientActionObj != NULL) {
		if(strcmp(json_object_get_string(ClientActionObj), "RULE") == 0) {
			if (ClientDataObj!=NULL) {
				return_status = pub_soc_rule_handler(ClientDataObj);
			}
		}
		else {
			syslog(LOG_ERR, "CRON: UNKNOWN CLIENT ACTION in pub_soc_dm_handler");
			return_status = FAILED_TO_PROCESS;
		}
	}
	else {
		syslog(LOG_ERR, "CRON: ClientActionObj is NULL in pub_soc_dm_handler");
		return_status = FAILED_TO_PROCESS;
	}
	return return_status;
}



int pub_soc_ma_handler(json_object *JsonSocobj, char *NotifId)
{
    json_object *ClientActionObj;
    json_object *ClientDataObj;
    json_object *NotifIdObj;

    json_object_object_get_ex(JsonSocobj, "client_action", &ClientActionObj);
    json_object_object_get_ex(JsonSocobj, "client_data", &ClientDataObj);
	if (ClientDataObj!=NULL) {
		json_object_object_get_ex(ClientDataObj, "notif_id", &NotifIdObj);
		if (NotifIdObj!=NULL)
			sprintf(NotifId,"%s",json_object_get_string(NotifIdObj));
		else
			sprintf(NotifId,"NONE");
	}
	else {
		syslog(LOG_ERR, "CRON: ClientDataObj is NULL in pub_soc_ma_handler");
		sprintf(NotifId,"NONE");
		return FAILED_TO_PROCESS;
	}

	if (ClientActionObj != NULL) {
		if(strcmp(json_object_get_string(ClientActionObj), "FOTA") == 0) {
			if (ClientDataObj!=NULL) {			
				return_status = pub_soc_fota_update_handler(ClientDataObj);
			}
		}
		else {
			syslog(LOG_ERR, "CRON: UNKNOWN CLIENT ACTION in pub_soc_ma_handler");
			return_status = FAILED_TO_PROCESS;
		}
	}
	else {
		syslog(LOG_ERR, "CRON: ClientActionObj is NULL in pub_soc_ma_handler");
		return_status = FAILED_TO_PROCESS;
	}
	return return_status;
}


int shifu_update_cron()
{
	FILE *fptr;
    sqlite3 *db;        // database connection
    int rc, i = 0, CronCount = 0, FotaJobCount = 0, RuleCount = 0, PARestartCount = 0, DMRestartCount = 0;
    sqlite3_stmt *stmt;
    char *query = NULL;
    char cron_entry[MAX_BUFSIZE_256];
    char cron_list[100][MAX_BUFSIZE_256];


    //syslog(LOG_INFO, "CRON: ENTERING UpdateCron()");
    /**
     * open SQLite database file ShifuAlarm.db
     */
    rc = sqlite3_open(CRON_DB_LOCATION, &db);
    if (rc != SQLITE_OK) {
        syslog(LOG_ERR, "CRON: ERROR opening SQLite DB in memory: %s", sqlite3_errmsg(db));
        return FAILED_TO_PROCESS;
    }


    asprintf(&query, "SELECT * FROM FOTA_INFO WHERE FotaId = 1;");
    sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
		FotaJobCount++;
        sprintf(cron_entry, "%s %s\n",sqlite3_column_text(stmt, FOTA_CRONJOB), sqlite3_column_text(stmt, FOTA_EXECUTABLE));
        strcpy(cron_list[CronCount],cron_entry);
        CronCount++;
    }
    sqlite3_finalize(stmt);

    asprintf(&query, "SELECT * FROM PA_RESTART WHERE PARestartId = 1;");
    sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
		PARestartCount++;
        sprintf(cron_entry, "%s %s\n",sqlite3_column_text(stmt, PA_RESTART_CRONJOB), sqlite3_column_text(stmt, PA_RESTART_COMMAND));
        strcpy(cron_list[CronCount],cron_entry);
        CronCount++;
    }
    sqlite3_finalize(stmt);

    /*
     * getting data for device manager restart from Database and processing to cronlist
     */
    asprintf(&query, "SELECT * FROM DM_RESTART WHERE DMRestartId = 1;");
    sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
		DMRestartCount++;
        sprintf(cron_entry, "%s %s\n",sqlite3_column_text(stmt, DM_RESTART_CRONJOB), sqlite3_column_text(stmt, DM_RESTART_COMMAND));
        strcpy(cron_list[CronCount],cron_entry);
        CronCount++;
    }
    sqlite3_finalize(stmt);

    asprintf(&query, "SELECT * FROM RULES;");
    sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        RuleCount++;
        sprintf(cron_entry, "%s shifu_cron_event \"RULE\" \"%s\"\n",sqlite3_column_text(stmt, RULE_CRONJOB), sqlite3_column_text(stmt, RULE_ID));
        strcpy(cron_list[CronCount],cron_entry);
        CronCount++;       
    }
    sqlite3_finalize(stmt);    

    /*
     * close SQLite database
     */
    rc = sqlite3_close(db);

    if((FotaJobCount != 0) || (RuleCount != 0) || (PARestartCount != 0) || (DMRestartCount != 0)) {
		fptr = fopen ("/etc/crontab","w");
		if(fptr == NULL) {
			//If not able to open file Exit Safely
			syslog(LOG_ERR, "CRON: CAN'T OPEN /etc/crontab FILE");
			return FAILED_TO_PROCESS;
		}

		for(i=0; i<CronCount; i++)
			fwrite(cron_list[i] , 1 , strlen(cron_list[i]) , fptr);
		return_status = SUCCESSFUL;
		fclose (fptr);
	}
    else {
		return_status = SUCCESSFUL;
		fptr = fopen ("/etc/crontab","w");
		fclose (fptr);
		syslog(LOG_DEBUG, "CRON: NO RECORDS IN CRON DATABASE");
    }

	/**Restart the cronjob*/
    system("/etc/init.d/cron restart");
    //syslog(LOG_INFO, "CRON: : LEAVING UpdateCron(): sqlite3_close: %d",rc);
    return return_status;
}


int cron_db_init()
{
    sqlite3 *db;        // database connection
    int rc;             // return code
    sqlite3_stmt *stmt;
    //syslog(LOG_INFO, "CRON: : ENTERING CreateAlarmDB()");
    char *query = NULL;
    /*
     * open SQLite database CRON_DB_LOCATION
     */
    rc = sqlite3_open(CRON_DB_LOCATION, &db);
    if (rc != SQLITE_OK) {
        syslog(LOG_ERR, "CRON: ERROR opening SQLite DB in memory: %s", sqlite3_errmsg(db));
        return FAILED_TO_PROCESS;
    }
    
    /**Table for Fota*/
    sqlite3_prepare_v2(db, "CREATE TABLE IF NOT EXISTS FOTA_INFO (FotaId INTEGER, FotaCronJob TEXT, FotaExecutable TEXT	);", -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        syslog(LOG_ERR, "CRON: ERROR while Creating FOTA_INFO Table: %s", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    
    /**Table for Pulse Audio restart*/
    sqlite3_prepare_v2(db, "CREATE TABLE IF NOT EXISTS PA_RESTART (PARestartId INTEGER PRIMARY KEY, PARestartCronJob TEXT, PARestartCommand TEXT);", -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        syslog(LOG_ERR, "CRON: ERROR while Creating PA_RESTART Table: %s", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);

    /**Table for Device Manager restart*/
    sqlite3_prepare_v2(db, "CREATE TABLE IF NOT EXISTS DM_RESTART (DMRestartId INTEGER PRIMARY KEY, DMRestartCronJob TEXT, DMRestartCommand TEXT);", -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        syslog(LOG_ERR, "CRON: ERROR while Creating DM_RESTART Table: %s", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    
    /**Table for Rules*/
    sqlite3_prepare_v2(db, "CREATE TABLE IF NOT EXISTS RULES (RuleId INTEGER, RuleCronJob TEXT );", -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        syslog(LOG_ERR, "CRON: ERROR while Creating RULES Table: %s", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    
    /** Inserting Pulse Audio restart Job */
	asprintf(&query, "INSERT OR IGNORE INTO PA_RESTART (PARestartId, PARestartCronJob, PARestartCommand )	\
									VALUES ('%d', '%s', '%s');", 1, "0 */5 * * *", "pactl unload-module module-alsa-sink && pactl load-module module-alsa-sink sink_name=alsa_output.0 device=dmixer");
	sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);


    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        syslog(LOG_ERR, "CRON: ERROR while Inserting in PA_RESTART table: %s", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);

    free(query);

    /** Inserting Device Manager restart Job */
	asprintf(&query, "INSERT OR IGNORE INTO DM_RESTART (DMRestartId, DMRestartCronJob, DMRestartCommand )	\
									VALUES ('%d', '%s', '%s');", 1, "0 1 * * *", "/etc/init.d/dm_service stop && /etc/init.d/dm_service start");
	sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);


    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        syslog(LOG_ERR, "CRON: ERROR while Inserting in DM_RESTART table: %s", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);

    free(query);

    /*
     * close SQLite database
     */
    rc = sqlite3_close(db);
    system("touch /etc/crontabs/root");
    system("ln -sf /etc/crontabs/root /etc/crontab");
    system("/etc/init.d/cron start");
    system("/etc/init.d/cron enable");
    shifu_update_cron();
    //syslog(LOG_INFO, "CRON: : LEAVING CreateAlarmDB(): sqlite3_close: %d",rc);
    return 0;
}

/**----------------------------------------------------------------------------------------------
 function: main()
 purpose:
	Starts the threads - audio record, voice_assistant_be,gst_core_action_handler,gst_play_prim,gst_play_sec and shifu_gst_cmd_handler.
----------------------------------------------------------------------------------------------*/
int main()
{
    int count=0;
	int env_ret;
	char env_var_val[MAX_BUFSIZE_256];
    char RequestedData[MAX_BUFSIZE_4096],RequestId[MAX_BUFSIZE_128], NotifId[MAX_BUFSIZE_128];
    json_object *JsonSocobj;
    json_object *ClientNameObj;
    json_object *ClientDataObj;
    json_object *MessageIdObj;

	env_ret = get_shifu_uci_param("VA_RULE_COUNT", env_var_val);
	if(env_ret == 0)
		max_rule_count = atoi(env_var_val);

	cron_db_init();

    syslog(LOG_DEBUG, "CRON: CRON PUBLIC INTERFACE INIT");
    
    zmq_context_cron = (void *)zmq_ctx_new();
    zmq_requester_cron = (void *)zmq_socket(zmq_context_cron, ZMQ_REP);
    zmq_bind(zmq_requester_cron, VA_PUB_ZMQ_PORT_NUM);

    while(true)
    {
		return_status = NONE;
		memset(RequestedData,'\0',sizeof(RequestedData));
		syslog(LOG_DEBUG, "CRON: WAITING FOR REQUEST FROM CLIENT");

		zmq_recv(zmq_requester_cron, RequestedData, MAX_BUFSIZE_4096, 0);

		count++;
		syslog(LOG_DEBUG, "CRON: ########################################");
		syslog(LOG_DEBUG, "CRON: NEW DATA FORMAT ON PUBLIC INTERFACE :: %s", RequestedData);
		syslog(LOG_DEBUG, "CRON: NEW DATA FORMAT REQUEST COUNT :: %d", count);
		syslog(LOG_DEBUG, "CRON: ########################################");
		
		if(RequestedData != NULL) {
			JsonSocobj = json_tokener_parse(RequestedData);
			if ( JsonSocobj != NULL ) {
				json_object_object_get_ex(JsonSocobj, "client_name", &ClientNameObj);
				if ( ClientNameObj != NULL ) {
					if(strcmp(json_object_get_string(ClientNameObj), "DM") == 0) {
						/** Device Manager request handler*/
						return_status = pub_soc_dm_handler(JsonSocobj, NotifId);		/** No need for return value, DM handler function will send ACK in function */
					}
					else if(strcmp(json_object_get_string(ClientNameObj), "MA") == 0) {
						/** Mobile Agent request handler*/
						return_status = pub_soc_ma_handler(JsonSocobj, NotifId);
					}
					else {
						syslog(LOG_ERR, "CRON: UNKNOWN CLIENT NAME");
						return_status = FAILED_TO_PROCESS;
					}
				}
				else{
					return_status = BAD_REQUEST;
				}
				json_object_put(JsonSocobj);			
			}
			else {
				return_status = BAD_REQUEST;
			}
		}
		else {
			return_status = BAD_REQUEST;
		}
		va_pub_socket_ack(return_status, NotifId);		/** Bad Request Response */
    }//END OF WHILE
    syslog(LOG_DEBUG, "CRON: MAIN Voice Assistant exiting");
}
