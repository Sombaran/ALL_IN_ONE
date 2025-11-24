/** @copyright
© 2014 SIGMA DESIGNS, INC. THIS IS AN UNPUBLISHED WORK PROTECTED BY SIGMA
DESIGNS, INC. AS A TRADE SECRET, AND IS NOT TO BE USED OR DISCLOSED EXCEPT AS
PROVIDED Z-WAVE CONTROLLER DEVELOPMENT KIT LIMITED LICENSE AGREEMENT. ALL
RIGHTS RESERVED.
@par
NOTICE: ALL INFORMATION CONTAINED HEREIN IS CONFIDENTIAL AND/OR PROPRIETARY TO
SIGMA DESIGNS AND MAY BE COVERED BY U.S. AND FOREIGN PATENTS, PATENTS IN
PROCESS, AND ARE PROTECTED BY TRADE SECRET OR COPYRIGHT LAW. DISSEMINATION OR
REPRODUCTION OF THE SOURCE CODE CONTAINED HEREIN IS EXPRESSLY FORBIDDEN TO
ANYONE EXCEPT LICENSEES OF SIGMA DESIGNS  WHO HAVE EXECUTED A SIGMA DESIGNS'
Z-WAVE CONTROLLER DEVELOPMENT KIT LIMITED LICENSE AGREEMENT. THE COPYRIGHT
NOTICE ABOVE IS NOT EVIDENCE OF ANY ACTUAL OR INTENDED PUBLICATION OF THE
SOURCE CODE. THE RECEIPT OR POSSESSION OF  THIS SOURCE CODE AND/OR RELATED
INFORMATION DOES NOT CONVEY OR IMPLY ANY RIGHTS  TO REPRODUCE, DISCLOSE OR
DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL A PRODUCT THAT IT  MAY
DESCRIBE.
@par
THE SIGMA PROGRAM AND ANY RELATED DOCUMENTATION OR TOOLS IS PROVIDED TO COMPANY
"AS IS" AND "WITH ALL FAULTS", WITHOUT WARRANTY OF ANY KIND FROM SIGMA. COMPANY
ASSUMES ALL RISKS THAT LICENSED MATERIALS ARE SUITABLE OR ACCURATE FOR
COMPANY'S NEEDS AND COMPANY'S USE OF THE SIGMA PROGRAM IS AT COMPANY'S
OWN DISCRETION AND RISK. SIGMA DOES NOT GUARANTEE THAT THE USE OF THE SIGMA
PROGRAM IN A THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES ENVIRONMENT WILL
BE: (A) PERFORMED ERROR-FREE OR UNINTERRUPTED; (B) THAT SIGMA WILL CORRECT ANY
THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT ERRORS; (C) THE
THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT WILL OPERATE IN
COMBINATION WITH COMPANY'S CONTENT OR COMPANY APPLICATIONS THAT UTILIZE THE
SIGMA PROGRAM; (D) OR WITH ANY OTHER HARDWARE, SOFTWARE, SYSTEMS, SERVICES OR
DATA NOT PROVIDED BY SIGMA. COMPANY ACKNOWLEDGES THAT SIGMA DOES NOT CONTROL
THE TRANSFER OF DATA OVER COMMUNICATIONS FACILITIES, INCLUDING THE INTERNET,
AND THAT THE SERVICES MAY BE SUBJECT TO LIMITATIONS, DELAYS, AND OTHER PROBLEMS
INHERENT IN THE USE OF SUCH COMMUNICATIONS FACILITIES. SIGMA IS NOT RESPONSIBLE
FOR ANY DELAYS, DELIVERY FAILURES, OR OTHER DAMAGE RESULTING FROM SUCH ISSUES.
SIGMA IS NOT RESPONSIBLE FOR ANY ISSUES RELATED TO THE PERFORMANCE, OPERATION
OR SECURITY OF THE THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES
ENVIRONMENT THAT ARISE FROM COMPANY CONTENT, COMPANY APPLICATIONS OR THIRD
PARTY CONTENT. SIGMA DOES NOT MAKE ANY REPRESENTATION OR WARRANTY REGARDING THE
RELIABILITY, ACCURACY, COMPLETENESS, CORRECTNESS, OR USEFULNESS OF THIRD PARTY
CONTENT OR SERVICE OR THE SIGMA PROGRAM, AND DISCLAIMS ALL LIABILITIES ARISING
FROM OR RELATED TO THE SIGMA PROGRAM OR THIRD PARTY CONTENT OR SERVICES. TO THE
EXTENT NOT PROHIBITED BY LAW, THESE WARRANTIES ARE EXCLUSIVE. SIGMA OFFERS NO
WARRANTY OF NON-INFRINGEMENT, TITLE, OR QUIET ENJOYMENT. NEITHER SIGMA NOR ITS
SUPPLIERS OR LICENSORS SHALL BE LIABLE FOR ANY INDIRECT, SPECIAL, INCIDENTAL OR
CONSEQUENTIAL DAMAGES OR LOSS (INCLUDING DAMAGES FOR LOSS OF BUSINESS, LOSS OF
PROFITS, OR THE LIKE), ARISING OUT OF THIS AGREEMENT WHETHER BASED ON BREACH OF
CONTRACT, INTELLECTUAL PROPERTY INFRINGEMENT, TORT (INCLUDING NEGLIGENCE),
STRICT LIABILITY, PRODUCT LIABILITY OR OTHERWISE, EVEN IF SIGMA OR ITS
REPRESENTATIVES HAVE BEEN ADVISED OF OR OTHERWISE SHOULD KNOW ABOUT THE
POSSIBILITY OF SUCH DAMAGES. THERE ARE NO OTHER EXPRESS OR IMPLIED WARRANTIES
OR CONDITIONS INCLUDING FOR SOFTWARE, HARDWARE, SYSTEMS, NETWORKS OR
ENVIRONMENTS OR FOR MERCHANTABILITY, NONINFRINGEMENT, SATISFACTORY QUALITY AND
FITNESS FOR A PARTICULAR PURPOSE.
@par
The Sigma Program  is not fault-tolerant and is not designed, manufactured or
intended for use or resale as on-line control equipment in hazardous
environments requiring fail-safe performance, such as in the operation of
nuclear facilities, aircraft navigation or communication systems, air traffic
control, direct life support machines, or weapons systems, in which the failure
of the Sigma Program, or Company Applications created using the Sigma Program,
could lead directly to death, personal injury, or severe physical or
environmental damage ("High Risk Activities").  Sigma and its suppliers
specifically disclaim any express or implied warranty of fitness for High Risk
Activities.Without limiting Sigma's obligation of confidentiality as further
described in the Z-Wave Controller Development Kit Limited License Agreement,
Sigma has no obligation to establish and maintain a data privacy and
information security program with regard to Company's use of any Third Party
Service Environment or Cloud Service Environment. For the avoidance of doubt,
Sigma shall not be responsible for physical, technical, security,
administrative, and/or organizational safeguards that are designed to ensure
the security and confidentiality of the Company Content or Company Application
in any Third Party Service Environment or Cloud Service Environment that
Company chooses to utilize.
*/
/**
 * @file    zwp_db.c
 *
 * @brief   Database management module.
 * @details APIs related to Database management.
 *
 * @author  Frank
 *
 * @version 1.0 - 2017-01-19
 * - Initial version
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <libgen.h>
#include "zwp_db_internal.h"
#include "zwp_db.h"
#include "zwp_memory.h"
#include "zwp_log.h"


const char *ZWP_DB_DATATYPE_STR_BLOB			= "BLOB";
const char *ZWP_DB_DATATYPE_STR_INT				= "INT";
const char *ZWP_DB_DATATYPE_STR_BIGINT			= "BIGINT";
const char *ZWP_DB_DATATYPE_STR_BIGINT_UNSIGNED = "BIGINT UNSIGNED";
const char *ZWP_DB_DATATYPE_STR_REAL			= "REAL";
const char *ZWP_DB_DATATYPE_STR_VARCHAR			= "VARCHAR";
#define     ZWP_DB_DATATYPE_STR_MAXLEN			16		/* Length of the longest ZWP_DB_DATATYPE_STR_XXXXXXX */

const char *ZWP_DB_DATA_EMPTY_BUFFER_INT		= "";

const char *ZWP_DB_ERRORTEXT_EMPTY				= "";
const char *ZWP_DB_ERRORTEXT_DB_OPEN			= "sqlite3_open() not ok.";
const char *ZWP_DB_ERRORTEXT_DB_EXEC			= "sqlite3_exec() not ok.";
const char *ZWP_DB_ERRORTEXT_DB_PREPARE			= "sqlite3_prepare_v2() not ok.";
const char *ZWP_DB_ERRORTEXT_DB_STEP			= "sqlite3_step() not ok.";
const char *ZWP_DB_ERRORTEXT_DB_FINALIZE		= "sqlite3_finalize() not ok.";


/**
Opening a Home ID database. Create it if database does not exist.
@param[in]      db_dir              Database directory path
@param[in]      iHome_id            Home ID
@param[in,out]  ppzwp_db_dbhandler  zwp_db db handler. If failure, handler is NULL. If success handler is valid.
@return         Return error code if failure. If success will return ZERO.
@pre            NIL.
*/
zwp_status_t zwp_db_open_db(char *db_dir, int64_t iHome_id, zwp_db_db **ppzwp_db_dbhandler)
{
	sqlite3 *sqldb_handler = NULL;
	zwp_db_db *pzwp_db_dbhandler;
	int RetCode;

	pzwp_db_dbhandler = ZWP_CALLOC(1, (sizeof(struct zwp_db_db_t_)));
	if(pzwp_db_dbhandler != NULL)
	{
		snprintf(pzwp_db_dbhandler->db_path_filename, ZWP_DB_DB_FILENAME_MAX, "%s/%" PRIX64 "-notif.db",
		         db_dir, iHome_id);
		if(sqlite3_open(pzwp_db_dbhandler->db_path_filename, &sqldb_handler) == SQLITE_OK)
		{
			if(sqldb_handler != NULL)
			{
				/* Successfully Open */
				pzwp_db_dbhandler->sqldb_handler = sqldb_handler;
				*ppzwp_db_dbhandler = pzwp_db_dbhandler;
				RetCode = ZWP_DB_OK;
			}
			else
			{
				/* Failed to Open with valid sqldb_handler, NULL it and Free it */
				ZWP_LOG(ZWP_LOG_DEBUG, "%s %" PRIX64 "-notif.db", ZWP_DB_ERRORTEXT_DB_OPEN, iHome_id);
				pzwp_db_dbhandler->sqldb_handler = NULL;
				pzwp_db_dbhandler->db_path_filename[0] = '\0';
				ZWP_FREE(pzwp_db_dbhandler);
				*ppzwp_db_dbhandler = NULL;
				RetCode = ZWP_DB_ERROR_UNKNOWN;
			}
		}
		else
		{
			/* Failed to Open, NULL it and Free it */
			ZWP_LOG(ZWP_LOG_DEBUG, "%s %" PRIX64 "-notif.db", ZWP_DB_ERRORTEXT_DB_OPEN, iHome_id);
			sqlite3_close(sqldb_handler);
			pzwp_db_dbhandler->sqldb_handler = NULL;
			pzwp_db_dbhandler->db_path_filename[0] = '\0';
			ZWP_FREE(pzwp_db_dbhandler);
			*ppzwp_db_dbhandler = NULL;
			RetCode = ZWP_DB_ERROR_DB_CONNECT;
		}
	}
	else
	{
		*ppzwp_db_dbhandler = NULL;
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Unable to ZWP_CALLOC");
		RetCode = ZWP_DB_ERROR;
	}

	return RetCode;
}


/**
Opening a zwp_db database by filename. Create it if database does not exist.
@param[in]      db_file             Database filename including full directory path
@return         Return zwp_db db handler. If failure, handler is NULL. If success handler is valid.
@pre            Please make sure directory path already exist.
*/
zwp_db_db *zwp_db_open_db_by_filename(char *db_file)
{
	sqlite3 *sqldb_handler = NULL;
	zwp_db_db *pzwp_db_dbhandler;

	pzwp_db_dbhandler = ZWP_CALLOC(1, (sizeof(struct zwp_db_db_t_)));
	if(pzwp_db_dbhandler != NULL)
	{
		snprintf(pzwp_db_dbhandler->db_path_filename, ZWP_DB_DB_FILENAME_MAX, "%s", db_file);
		if(sqlite3_open(pzwp_db_dbhandler->db_path_filename, &sqldb_handler) == SQLITE_OK)
		{
			if(sqldb_handler != NULL)
			{
				/* Successfully Open, return valid pzwp_db_dbhandler */
				pzwp_db_dbhandler->sqldb_handler = sqldb_handler;
			}
			else
			{
				/* Failed to Open with valid sqldb_handler, NULL it and Free it */
				ZWP_LOG(ZWP_LOG_DEBUG, "%s, %s", ZWP_DB_ERRORTEXT_DB_OPEN, "sqlite3_open failed");
				pzwp_db_dbhandler->sqldb_handler = NULL;
				pzwp_db_dbhandler->db_path_filename[0] = '\0';
				ZWP_FREE(pzwp_db_dbhandler);
				pzwp_db_dbhandler = NULL;
			}
		}
		else
		{
			/* Failed to Open, NULL it and Free it */
			ZWP_LOG(ZWP_LOG_DEBUG, "%s", ZWP_DB_ERRORTEXT_DB_OPEN);
			sqlite3_close(sqldb_handler);
			pzwp_db_dbhandler->sqldb_handler = NULL;
			pzwp_db_dbhandler->db_path_filename[0] = '\0';
			ZWP_FREE(pzwp_db_dbhandler);
			pzwp_db_dbhandler = NULL;
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Unable to ZWP_CALLOC");
	}

	return pzwp_db_dbhandler;
}


/**
Closing a Home ID database.
@param[in]      pzwp_db_dbhandler   zwp_db db handler. If success, handler reset to NULL.
@return         Return error code if failure. If success will return ZERO.
@pre            pzwp_db_dbhandler must be valid handler.
*/
zwp_status_t zwp_db_close_db(zwp_db_db *pzwp_db_dbhandler)
{
	sqlite3 *sqldb_handler;
	int RetCode = ZWP_DB_OK;

	if(pzwp_db_dbhandler != NULL)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Closing DB file: %s", pzwp_db_dbhandler->db_path_filename);

		sqldb_handler = pzwp_db_dbhandler->sqldb_handler;
		if(sqlite3_close(sqldb_handler) == SQLITE_OK)
		{
			pzwp_db_dbhandler->sqldb_handler = NULL;
			pzwp_db_dbhandler->db_path_filename[0] = '\0';
			ZWP_FREE(pzwp_db_dbhandler);
		}
		else
		{
			RetCode = ZWP_DB_ERROR;
		}
	}
	

	return RetCode;
}


/**
Delete a Home ID database.
@param[in]      pzwp_db_dbhandler   zwp_db db handler. If success, database will be closed, handler reset to NULL.
@return         Return error code if failure. If success will return ZERO.
@pre            pzwp_db_dbhandler must be valid handler.
*/
zwp_status_t zwp_db_delete_db(zwp_db_db *pzwp_db_dbhandler)
{
	int RetCode;
	char rm_db_path_filename[ZWP_DB_DB_FILENAME_MAX];

	if(pzwp_db_dbhandler != NULL)
	{
		RetCode = ZWP_DB_ERROR;
		rm_db_path_filename[0] = '\0';
		/* Close Database if required */
		if(pzwp_db_dbhandler->sqldb_handler != NULL)
		{
			snprintf(rm_db_path_filename, ZWP_DB_DB_FILENAME_MAX, "%s", pzwp_db_dbhandler->db_path_filename);
			zwp_db_close_db(pzwp_db_dbhandler);
			if(strlen(rm_db_path_filename) > 0)
			{
				if((remove(rm_db_path_filename)) == 0)
				{
					RetCode = ZWP_DB_OK;
				}
			}
		}
	}
	else
	{
		RetCode = ZWP_DB_ERROR_DB_CLOSE;
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pzwp_db_dbhandler pointer is NULL");
	}

	return RetCode;
}


/**
Creating and Opening a table in a Home ID database.
@param[in]      pzwp_db_dbhandler   zwp_db db handler.
@param[in]      pTableName          Table Name in database.
@param[in]      iNumOfCol           Number of columns in this new table header.
@param[in]      ptable_head_list    New table header declaration structure.
@param[out]     ppDbTable           Table handler pointer.
@return         Return error code if failure. If success will return ZERO.
@pre            pzwp_db_dbhandler must be valid handler.
*/
zwp_status_t zwp_db_create_table(zwp_db_db *pzwp_db_dbhandler, char *pTableName, int iNumOfCol,
                                 pzwp_db_table_head ptable_head_list, zwp_db_table **ppDbTable)
{
	sqlite3 *sqldb_handler;
	char sql_stmt[ZWP_DB_SQL_STMT_BUFSIZE];
	char sql_stmt_field[ZWP_DB_SQL_STMT_BUFSIZE];
	char sql_stmt_primarykey[ZWP_DB_SQL_STMT_BUFSIZE];
	int  sql_stmt_field_length;
	int  sql_stmt_primarykey_length;
	zwp_db_table *pDbTable;
	char *dbbasename;
	char *zErrMsg = 0;
	int  i;
	int  rc;

	if(pzwp_db_dbhandler != NULL)
	{
		if((strlen(pTableName) < 1) || (iNumOfCol < 1))
		{
			ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pTableName or iNumOfCol is invalid");
			return ZWP_DB_ERROR_DB_CREATE;
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pzwp_db_dbhandler pointer is NULL");
		return ZWP_DB_ERROR_DB_CREATE;
	}

	pDbTable = ZWP_CALLOC(1, (sizeof(struct zwp_db_table_t_)));
	if(pDbTable == NULL)
	{
		/* Error allocate memory */
		*ppDbTable = NULL;
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pDbTable pointer is NULL. Unable to ZWP_CALLOC");
		return ZWP_DB_ERROR_DB_CREATE;
	}

	sqldb_handler = pzwp_db_dbhandler->sqldb_handler;
	pDbTable->errortext[0] = '\0';
	sql_stmt_field_length = 0;
	sql_stmt_primarykey_length = 0;
	pDbTable->max_blob_size = 0;
	pDbTable->max_text_size = 0;
	for(i=0; i<iNumOfCol; i++)
	{
		switch(ptable_head_list[i].eColumnType)
		{
			case ZWP_DB_DATATYPE_BLOB :
				snprintf((sql_stmt_field + sql_stmt_field_length),
				         (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_field_length),
				         "%s %s", ptable_head_list[i].pcColumnName, ZWP_DB_DATATYPE_STR_BLOB);
				if(ptable_head_list[i].isize > 0)
				{
					pDbTable->max_blob_size += (ptable_head_list[i].isize);
				}
				break;

			case ZWP_DB_DATATYPE_INT :
				snprintf((sql_stmt_field + sql_stmt_field_length),
				         (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_field_length),
				         "%s %s", ptable_head_list[i].pcColumnName, ZWP_DB_DATATYPE_STR_INT);
				break;

			case ZWP_DB_DATATYPE_BIGINT :
				snprintf((sql_stmt_field + sql_stmt_field_length),
				         (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_field_length),
				         "%s %s", ptable_head_list[i].pcColumnName, ZWP_DB_DATATYPE_STR_BIGINT);
				break;

			case ZWP_DB_DATATYPE_BIGINT_UNSIGNED :
				snprintf((sql_stmt_field + sql_stmt_field_length),
				         (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_field_length),
				         "%s %s", ptable_head_list[i].pcColumnName, ZWP_DB_DATATYPE_STR_BIGINT_UNSIGNED);
				break;

			case ZWP_DB_DATATYPE_REAL :
				snprintf((sql_stmt_field + sql_stmt_field_length),
				         (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_field_length),
				         "%s %s", ptable_head_list[i].pcColumnName, ZWP_DB_DATATYPE_STR_REAL);
				break;

			case ZWP_DB_DATATYPE_TEXT :
				if(ptable_head_list[i].isize > 0)
				{
					snprintf((sql_stmt_field + sql_stmt_field_length),
					         (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_field_length),
					         "%s %s(%d)", ptable_head_list[i].pcColumnName, ZWP_DB_DATATYPE_STR_VARCHAR,
					         ptable_head_list[i].isize);
				}
				else
				{
					snprintf((sql_stmt_field + sql_stmt_field_length),
					         (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_field_length),
					         "%s %s(%d)", ptable_head_list[i].pcColumnName, ZWP_DB_DATATYPE_STR_VARCHAR,
					         ZWP_DB_SQL_STMT_VARCHAR_MAX);
				}
				pDbTable->max_text_size += (ZWP_DB_SQL_STMT_VARCHAR_MAX + 1);
				break;

			default :
				snprintf((sql_stmt_field + sql_stmt_field_length),
				         (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_field_length),
				         "%s %s(%d)", ptable_head_list[i].pcColumnName, ZWP_DB_DATATYPE_STR_VARCHAR,
				         ZWP_DB_SQL_STMT_VARCHAR_MAX);
				pDbTable->max_text_size += (ZWP_DB_SQL_STMT_VARCHAR_MAX + 1);
		}
		sql_stmt_field_length = strlen(sql_stmt_field);

		/* Check iProperty for NOT_NULL is set? */
		if(RegisterChk(ptable_head_list[i].iProperty, ZWP_DB_PROPERTY_NOT_NULL))
		{
			snprintf((sql_stmt_field + sql_stmt_field_length),
			         (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_field_length),
			         " NOT NULL");
			sql_stmt_field_length += 9;
		}

		/* Check iProperty for PRIMARY KEY is set? */
		if(RegisterChk(ptable_head_list[i].iProperty, ZWP_DB_PROPERTY_PRIMARY_KEY))
		{
			/* Construct CONSTRAINT PRIMARY KEY field */
			if(sql_stmt_primarykey_length == 0)
			{
				snprintf((sql_stmt_primarykey + sql_stmt_primarykey_length),
				         (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_primarykey_length),
				         "%s", ptable_head_list[i].pcColumnName);
				sql_stmt_primarykey_length = strlen(ptable_head_list[i].pcColumnName);
			}
			else
			{
				snprintf((sql_stmt_primarykey + sql_stmt_primarykey_length),
				         (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_primarykey_length),
				         ", %s", ptable_head_list[i].pcColumnName);
				sql_stmt_primarykey_length += strlen(ptable_head_list[i].pcColumnName) + 2;
			}
		}

		/* Adding commas for each column field */
		if(i < (iNumOfCol-1))
		{
			snprintf((sql_stmt_field + sql_stmt_field_length),
			         (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_field_length),
			         ", ");
			sql_stmt_field_length += 2;
		}
	}

	if(sql_stmt_primarykey_length > 0)
	{
		/* With CONSTRAINT PRIMARY KEY field */
		snprintf(sql_stmt, ZWP_DB_SQL_STMT_BUFSIZE,
		         "CREATE TABLE IF NOT EXISTS '%s' ( %s, CONSTRAINT zwp_pk_constraint PRIMARY KEY ( %s ));",
		         pTableName, sql_stmt_field, sql_stmt_primarykey);
	}
	else
	{
		/* Without CONSTRAINT PRIMARY KEY field */
		snprintf(sql_stmt, ZWP_DB_SQL_STMT_BUFSIZE,
		         "CREATE TABLE IF NOT EXISTS '%s' ( %s );", pTableName, sql_stmt_field);
	}

	if((rc = sqlite3_exec(sqldb_handler, sql_stmt, 0, 0, &zErrMsg)) == SQLITE_OK)
	{
		pDbTable->dbhandler = pzwp_db_dbhandler->sqldb_handler;
		snprintf(pDbTable->table_name, ZWP_DB_SQL_TABLENAME_MAX, "%s", pTableName);
		pDbTable->table_ptr = pDbTable;
		pDbTable->table_NumOfColumnField = iNumOfCol;
		pDbTable->errortext[0] = '\0';
		*ppDbTable = pDbTable;
		return ZWP_DB_OK;
	}
	else
	{
		dbbasename = basename(pzwp_db_dbhandler->db_path_filename);
		ZWP_LOG(ZWP_LOG_DEBUG, "%s %s, rc=%d, %s", ZWP_DB_ERRORTEXT_DB_EXEC,
		        sqlite3_errmsg(sqldb_handler), rc, dbbasename);
		pDbTable->dbhandler = NULL;
		ZWP_FREE(pDbTable);
		*ppDbTable = NULL;
		if(zErrMsg != NULL)
		{
			sqlite3_free(zErrMsg);
		}
	}

	return ZWP_DB_ERROR_DB_CREATE;
}


/**
Opening a table in a Home ID database if table already exist.
@param[in]      pzwp_db_dbhandler   zwp_db db handler.
@param[in]      pTableName          Table Name in database.
@param[out]     ppDbTable           Table handler pointer.
@return         Return error code if failure. If success will return ZERO.
@pre            pzwp_db_dbhandler must be valid handler.
*/
zwp_status_t zwp_db_open_table(zwp_db_db *pzwp_db_dbhandler, char *pTableName,
                               zwp_db_table **ppDbTable)
{
	sqlite3 *sqldb_handler;
	sqlite3_stmt *pStmt;
	char sql_stmt[ZWP_DB_SQL_STMT_BUFSIZE];
	int  RetCode;
	int  retval;
	int  iNumOfCols;
	int  iCol;
	char *dbbasename;
	zwp_db_table *pDbTable;
	int  rc;

	if(pzwp_db_dbhandler != NULL)
	{
		if(strlen(pTableName) < 1)
		{
			ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pTableName is invalid");
			return ZWP_DB_ERROR_DB_CONNECT;
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pzwp_db_dbhandler pointer is NULL");
		return ZWP_DB_ERROR_DB_CONNECT;
	}

	pDbTable = ZWP_CALLOC(1, (sizeof(struct zwp_db_table_t_)));
	if(pDbTable == NULL)
	{
		/* Error allocate memory */
		*ppDbTable = NULL;
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pDbTable pointer is NULL. Unable to ZWP_CALLOC");
		return ZWP_DB_ERROR_DB_CONNECT;
	}

	sqldb_handler = pzwp_db_dbhandler->sqldb_handler;
	pDbTable->errortext[0] = '\0';
	dbbasename = basename(pzwp_db_dbhandler->db_path_filename);

	/*-------------------------------------*/
	/* Step 1: Search for table_name first */
	/*-------------------------------------*/
	snprintf(sql_stmt, ZWP_DB_SQL_STMT_BUFSIZE,
	         "SELECT tbl_name FROM sqlite_master UNION SELECT tbl_name FROM sqlite_temp_master;");
	if((rc = sqlite3_prepare_v2(sqldb_handler, sql_stmt, -1, &pStmt, 0)) != SQLITE_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s %s, rc=%d, %s", ZWP_DB_ERRORTEXT_DB_PREPARE,
		        sqlite3_errmsg(sqldb_handler), rc, dbbasename);
		ZWP_FREE(pDbTable);
		*ppDbTable = NULL;
		return ZWP_DB_ERROR_DB_CONNECT;
	}

	/* Read the number of rows fetched */
	iNumOfCols = sqlite3_column_count(pStmt);
	RetCode = ZWP_DB_ERROR_DB_SEARCH_NOT_FOUND;
	while(1)
	{
		/* Check fetch a row's status */
		retval = sqlite3_step(pStmt);
		if(retval == SQLITE_ROW)
		{
			/* SQLITE_ROW means continue fetching a row */
			for(iCol=0; iCol<iNumOfCols; iCol++)
			{
				const char *val = (const char*)sqlite3_column_text(pStmt, iCol);
				if(strncmp(val, pTableName, ZWP_DB_SQL_TABLENAME_MAX) == 0)
				{
					pDbTable->dbhandler = pzwp_db_dbhandler->sqldb_handler;;
					snprintf(pDbTable->table_name, ZWP_DB_SQL_TABLENAME_MAX, "%s", pTableName);
					pDbTable->table_ptr = pDbTable;
					pDbTable->errortext[0] = '\0';
					*ppDbTable = pDbTable;
					RetCode = ZWP_DB_OK;
				}
			}
		}
		else if(retval == SQLITE_DONE)
		{
			/* All rows fetched, break loop now */
			break;
		}
		else
		{
			/* Unknown error encountered */
			ZWP_LOG(ZWP_LOG_DEBUG, "%s %s, %s", ZWP_DB_ERRORTEXT_DB_STEP,
			        sqlite3_errmsg(sqldb_handler), dbbasename);
			RetCode = ZWP_DB_ERROR_DB_DUMP;
			break;
		}
	}

	if(sqlite3_finalize(pStmt) != SQLITE_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s, %s", ZWP_DB_ERRORTEXT_DB_FINALIZE, dbbasename);
	}

	/*------------------------------------------------------------*/
	/* Step 2: Search for table structure, count number of column */
	/*------------------------------------------------------------*/
	if(RetCode == ZWP_DB_OK)
	{
		/* Search for total column then save it */
		snprintf(sql_stmt, ZWP_DB_SQL_STMT_BUFSIZE,
		         "SELECT * FROM '%s' LIMIT 1;", pTableName);
		if((rc = sqlite3_prepare_v2(sqldb_handler, sql_stmt, -1, &pStmt, 0)) != SQLITE_OK)
		{
			ZWP_LOG(ZWP_LOG_DEBUG, "%s %s, rc=%d, %s", ZWP_DB_ERRORTEXT_DB_PREPARE,
			        sqlite3_errmsg(sqldb_handler), rc, dbbasename);
			ZWP_FREE(pDbTable);
			*ppDbTable = NULL;
			return ZWP_DB_ERROR_DB_CONNECT;
		}
		iNumOfCols = sqlite3_column_count(pStmt);
		if(iNumOfCols > 0)
		{
			pDbTable->table_NumOfColumnField = iNumOfCols;
			*ppDbTable = pDbTable;
			RetCode = ZWP_DB_OK;
		}
		else
		{
			pDbTable->table_NumOfColumnField = 0;
			ZWP_LOG(ZWP_LOG_ERR, "Parse table structure failed. Num of column is 0: %s %s",
			        sqlite3_errmsg(sqldb_handler), dbbasename);
			ZWP_FREE(pDbTable);
			*ppDbTable = NULL;
			RetCode = ZWP_DB_ERROR_DB_CONNECT;
		}
		if(sqlite3_finalize(pStmt) != SQLITE_OK)
		{
			ZWP_LOG(ZWP_LOG_DEBUG, "%s, %s", ZWP_DB_ERRORTEXT_DB_FINALIZE, dbbasename);
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s table not exist. %s, %s", pTableName,
		        sqlite3_errmsg(sqldb_handler), dbbasename);
		ZWP_FREE(pDbTable);
		*ppDbTable = NULL;
	}

	return RetCode;
}


/**
Close a table handler in a Home ID database.
@param[in]      pDbTable            Table handler pointer.
@return         Return error code if failure. If success will return ZERO.
@pre            pDbTable must be valid handler.
*/
zwp_status_t zwp_db_close_table(zwp_db_table *pDbTable)
{
	if(pDbTable == NULL)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pDbTable pointer is NULL");
		return ZWP_DB_ERROR_DB_SELECT;
	}

	pDbTable->dbhandler = NULL;
	pDbTable->table_ptr = NULL;
	pDbTable->table_NumOfColumnField = 0;
	pDbTable->table_name[0] = '\0';
	pDbTable->errortext[0] = '\0';
	ZWP_FREE(pDbTable);

	return ZWP_DB_OK;
}


/**
Delete a table in a Home ID database if table already exist.
@param[in]      pDbTable            Table handler pointer.
@return         Return error code if failure. If success will return ZERO.
@pre            pDbTable must be valid handler.
*/
zwp_status_t zwp_db_drop_table(zwp_db_table *pDbTable)
{
	sqlite3 *sqldb_handler;
	sqlite3_stmt *pStmt;
	char sql_stmt[ZWP_DB_SQL_STMT_BUFSIZE];
	int RetCode;
	int retval;

	if(pDbTable == NULL)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pDbTable pointer is NULL");
		return ZWP_DB_ERROR_DB_SELECT;
	}

	if(pDbTable->dbhandler == NULL)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pDbTable->dbhandler pointer is NULL");
		return ZWP_DB_ERROR_DB_SELECT;
	}

	if(strlen(pDbTable->table_name) < 1)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pDbTable->table_name is invalid");
		return ZWP_DB_ERROR_DB_SELECT;
	}

	sqldb_handler = (sqlite3 *) pDbTable->dbhandler;
	pDbTable->errortext[0] = '\0';
	snprintf(sql_stmt, ZWP_DB_SQL_STMT_BUFSIZE,
	         "DROP TABLE IF EXISTS '%s'; COMMIT;", pDbTable->table_name);
	if(sqlite3_prepare_v2(sqldb_handler, sql_stmt, -1, &pStmt, 0) != SQLITE_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s %s", ZWP_DB_ERRORTEXT_DB_PREPARE,
		        sqlite3_errmsg(sqldb_handler));
		snprintf(pDbTable->errortext, ZWP_DB_SQL_ERRORTEXT_MAX, "ERROR: %s: Prepare failed: %s",
		         __func__, sqlite3_errmsg(sqldb_handler));
		return ZWP_DB_ERROR_DB_CONNECT;
	}

	if((retval = sqlite3_step(pStmt)) == SQLITE_DONE)
	{
		zwp_db_close_table(pDbTable);
		RetCode = ZWP_DB_OK;
	}
	else
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s %s", ZWP_DB_ERRORTEXT_DB_STEP,
		        sqlite3_errmsg(sqldb_handler));
		snprintf(pDbTable->errortext, ZWP_DB_SQL_ERRORTEXT_MAX, "ERROR: %s: Drop table failed: %s",
		         __func__, sqlite3_errmsg(sqldb_handler));
		RetCode = ZWP_DB_ERROR_DB_CONNECT;
	}

	if(sqlite3_finalize(pStmt) != SQLITE_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", ZWP_DB_ERRORTEXT_DB_FINALIZE);
	}

	return RetCode;
}


/**
Get error text message from table handler.
@param[in]      pDbTable            Table handler pointer.
@return         Return error text message buffer pointer.
@pre            pDbTable must be valid handler.
*/
char *zwp_db_get_table_errortext(zwp_db_table *pDbTable)
{
	char *pErrText;

	if(pDbTable != NULL)
	{
		pErrText = pDbTable->errortext;
	}
	else
	{
		pErrText = (char *) ZWP_DB_ERRORTEXT_EMPTY;
	}

	return pErrText;
}


/**
Get table column header declaration information from table handler.
@param[in]      pDbTable            Table handler pointer.
@param[in]      iNumOfColumnField   Number of columns in this table header.
@param[in,out]  pTableHead_Vector   This table header declaration structure.
@return         Return error code if failure. If success will return ZERO.
@pre            pDbTable must be valid handler.
*/
zwp_status_t zwp_db_pragma_table_info(zwp_db_table *pDbTable, int iNumOfColumnField,
                                      pzwp_db_table_head pTableHead_Vector)
{
	sqlite3 *sqldb_handler;
	sqlite3_stmt *pStmt;
	char sql_stmt[ZWP_DB_SQL_STMT_BUFSIZE];
	int  retval;
	int  iNumOfCols;
	int  iCol;
	int  iRow;

	if(pDbTable == NULL)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pDbTable pointer is NULL");
		return ZWP_DB_ERROR;
	}

	if(pDbTable->dbhandler != NULL)
	{
		sqldb_handler = (sqlite3 *) pDbTable->dbhandler;
		pDbTable->errortext[0] = '\0';
	}
	else
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pDbTable->dbhandler pointer is NULL");
		return ZWP_DB_ERROR;
	}

	/* PRAGMA is specific to SQLite only. Main purpose is to get table field_name 'name' list and it's 'type' */
	snprintf(sql_stmt, ZWP_DB_SQL_STMT_BUFSIZE,
	         "PRAGMA TABLE_INFO(%s);", pDbTable->table_name);

	if(sqlite3_prepare_v2(sqldb_handler, sql_stmt, -1, &pStmt, 0) != SQLITE_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s %s", ZWP_DB_ERRORTEXT_DB_PREPARE,
		        sqlite3_errmsg(sqldb_handler));
		snprintf(pDbTable->errortext, ZWP_DB_SQL_ERRORTEXT_MAX, "ERROR: %s: Prepare failed: %s",
		         __func__, sqlite3_errmsg(sqldb_handler));
		return ZWP_DB_ERROR;
	}
	iNumOfCols = sqlite3_column_count(pStmt);
	iRow = 0;
	while(1)
	{
		/* Check fetch a row's status */
		retval = sqlite3_step(pStmt);
		if(retval == SQLITE_ROW)
		{
			/* SQLITE_ROW means continue fetching a row */
			if(iRow < iNumOfColumnField)
			{
				for(iCol=0; iCol<iNumOfCols; iCol++)
				{
					const char *val;

					if(strncmp(sqlite3_column_name(pStmt, iCol), "name", 4) == 0)
					{
						val = (const char*)sqlite3_column_text(pStmt, iCol);
						//printf("%d: %s=%s\n", iRow, sqlite3_column_name(pStmt, iCol), val);
					}
					else if(strncmp(sqlite3_column_name(pStmt, iCol), "type", 4) == 0)
					{
						val = (const char*)sqlite3_column_text(pStmt, iCol);
						//printf("%d: %s=%s\n", iRow, sqlite3_column_name(pStmt, iCol), val);
						if(strncmp(val, ZWP_DB_DATATYPE_STR_BLOB, ZWP_DB_DATATYPE_STR_MAXLEN) == 0)
						{
							pTableHead_Vector[iRow].eColumnType = ZWP_DB_DATATYPE_BLOB;
						}
						else if(strncmp(val, ZWP_DB_DATATYPE_STR_INT, ZWP_DB_DATATYPE_STR_MAXLEN) == 0)
						{
							pTableHead_Vector[iRow].eColumnType = ZWP_DB_DATATYPE_INT;
						}
						else if(strncmp(val, ZWP_DB_DATATYPE_STR_BIGINT, ZWP_DB_DATATYPE_STR_MAXLEN) == 0)
						{
							pTableHead_Vector[iRow].eColumnType = ZWP_DB_DATATYPE_BIGINT;
						}
						else if(strncmp(val, ZWP_DB_DATATYPE_STR_BIGINT_UNSIGNED, ZWP_DB_DATATYPE_STR_MAXLEN) == 0)
						{
							pTableHead_Vector[iRow].eColumnType = ZWP_DB_DATATYPE_BIGINT_UNSIGNED;
						}
						else if(strncmp(val, ZWP_DB_DATATYPE_STR_REAL, ZWP_DB_DATATYPE_STR_MAXLEN) == 0)
						{
							pTableHead_Vector[iRow].eColumnType = ZWP_DB_DATATYPE_REAL;
						}
						else if(strncmp(val, ZWP_DB_DATATYPE_STR_VARCHAR, ZWP_DB_DATATYPE_STR_MAXLEN) == 0)
						{
							pTableHead_Vector[iRow].eColumnType = ZWP_DB_DATATYPE_TEXT;
						}
						else
						{
							pTableHead_Vector[iRow].eColumnType = ZWP_DB_DATATYPE_NULL;
						}
					}
				}
			}
			iRow++;
		}
		else if(retval == SQLITE_DONE)
		{
			/* All rows fetched, break loop now */
			break;
		}
		else
		{
			/* Unknown error encountered */
			ZWP_LOG(ZWP_LOG_DEBUG, "%s %s", ZWP_DB_ERRORTEXT_DB_STEP,
			        sqlite3_errmsg(sqldb_handler));
			snprintf(pDbTable->errortext, ZWP_DB_SQL_ERRORTEXT_MAX, "ERROR: %s: Table_info failed: %s",
			         __func__, sqlite3_errmsg(sqldb_handler));
			break;
		}
	}

	if(sqlite3_finalize(pStmt) != SQLITE_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", ZWP_DB_ERRORTEXT_DB_FINALIZE);
	}

	return ZWP_DB_OK;
}


/**
Malloc memory for struct zwp_db_table_head_t_. Each table column is 1 header struct.
@param[in]      sz                  Number of header struct required.
@return         Return table header struct pointer.
@pre            Nil.
*/
zwp_db_table_head *zwp_db_table_head_malloc(size_t sz)
{
	struct zwp_db_table_head_t_ *pTableHead_Vector;

	/* Request with zero-initialized memory block */
	pTableHead_Vector = calloc(1, (sizeof(struct zwp_db_table_head_t_) * sz));
	if(pTableHead_Vector == NULL)
	{
		return NULL;
	}

	return pTableHead_Vector;
}


/**
Free memory for struct zwp_db_table_head_t_ allocated by zwp_db_table_head_malloc.
@param[in]      pTableHead_Vector   Header struct pointer.
@return         Nil.
@pre            Nil.
*/
void zwp_db_table_head_free(struct zwp_db_table_head_t_ *pTableHead_Vector)
{
	if(pTableHead_Vector != NULL)
	{
		ZWP_FREE(pTableHead_Vector);
	}
}


/**
Malloc memory for struct zwp_db_table_value_t_. Each column value is 1 value struct.
@param[in]      sz                  Number of value struct required.
@return         Return table value struct pointer.
@pre            Nil.
*/
zwp_db_table_value *zwp_db_table_value_malloc(size_t sz)
{
	struct zwp_db_table_value_t_ *pTableValue_Vector;

	/* Request with zero-initialized memory block */
	pTableValue_Vector = ZWP_CALLOC(1, (sizeof(struct zwp_db_table_value_t_) * sz));
	if(pTableValue_Vector == NULL)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Unable to ZWP_CALLOC");
		return NULL;
	}

	return pTableValue_Vector;
}


/**
Free memory for struct zwp_db_table_value_t_ allocated by zwp_db_table_value_malloc.
@param[in]      iNumOfCol            Number of column per record.
@param[in]      iNumOfRow            Number of row of records.
@param[in]      pTableValue_Vector   Table value struct pointer.
@return         Return error code if failure. If success will return ZERO.
@pre            pDbTable must be valid handler.
*/
int zwp_db_table_value_free(int iNumOfCol, int iNumOfRow, zwp_db_table_value *pTableValue_Vector)
{
	int  RetCode;
	int  i, j, k;
	int  iRowIndex;

	RetCode = ZWP_DB_ERROR;
	if(pTableValue_Vector != NULL)
	{
		for(j=0; j<iNumOfRow; j++)
		{
			iRowIndex = (j * iNumOfCol);
			for(i=0; i<iNumOfCol; i++)
			{
				k = (iRowIndex + i);
				if(j == 0)
				{
					/* Free memory for Column Name */
					/* Only free the 1st row, as subsequence row is point to 1st row malloc pointer. */
					if(pTableValue_Vector[k].pcColumnName != NULL)
					{
						*pTableValue_Vector[k].pcColumnName = '\0';
						ZWP_FREE(pTableValue_Vector[k].pcColumnName);
					}
				}
				pTableValue_Vector[k].pcColumnName = NULL;

				/* Free memory for Column Data Pointer */
				if(pTableValue_Vector[k].eType == ZWP_DB_DATATYPE_TEXT)
				{
					/* Free memory for valuestring TEXT */
					if(pTableValue_Vector[k].data.valuestring != NULL)
					{
						ZWP_FREE(pTableValue_Vector[k].data.valuestring);
						pTableValue_Vector[k].data.valuestring = NULL;
					}
				}

				if(pTableValue_Vector[k].eType == ZWP_DB_DATATYPE_BLOB)
				{
					/* Free memory for valuestring BLOB */
					if(pTableValue_Vector[k].data.valuebyte != NULL)
					{
						ZWP_FREE(pTableValue_Vector[k].data.valuebyte);
						pTableValue_Vector[k].data.valuebyte = NULL;
					}
				}
			}
		}

		ZWP_FREE(pTableValue_Vector);
		RetCode = ZWP_DB_OK;
	}

	return RetCode;
}


/**
Malloc memory buffer then memcopy source address to the new memory. Duplicate memory.
@param[in]      sz                   Number of bytes to malloc.
@param[in]      sql_column_data_src  Source memory pointer to copy from.
@return         Return a new memory pointer.
@pre            Nil.
*/
void *zwp_db_table_column_data_malloc_copy(size_t sz, void *sql_column_data_src)
{
	uint8_t *ptable_column_data;

	if(sz > 0)
	{
		ptable_column_data = ZWP_MALLOC(sizeof(uint8_t) * (sz + 1));
		//printf("zwp_db_table_column_data_malloc_copy() %p\n", ptable_column_data);
		if(ptable_column_data != NULL)
		{
			memcpy(ptable_column_data, sql_column_data_src, sz);
			*((ptable_column_data) + sz) = '\0';
		}
	}
	else
	{
		ptable_column_data = NULL;
	}

	return ptable_column_data;
}


/**
Inserting a record to a table in a Home ID database.
@param[in]      pDbTable            Table handler pointer.
@param[in]      iNumOfCol           Number of columns in this table_data.
@param[in]      table_data          Table data value specified using a value struct.
@return         Return error code if failure. If success will return ZERO.
@pre            pDbTable must be valid handler.
*/
zwp_status_t zwp_db_insert_record(zwp_db_table *pDbTable, int iNumOfCol,
                                  pzwp_db_table_value table_data)
{
	sqlite3 *sqldb_handler;
	sqlite3_stmt *pStmt;
	char sql_stmt[ZWP_DB_SQL_STMT_BUFSIZE];
	int  sql_stmt_length;
	int  i, k;
	int  RetCode;
	int  retval;
	char *pExpanded_sql_pStmt_Mem;

	if(pDbTable == NULL)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pDbTable pointer is NULL");
		return ZWP_DB_ERROR_DB_INSERT;
	}

	if(pDbTable->dbhandler != NULL)
	{
		sqldb_handler = (sqlite3 *) pDbTable->dbhandler;
		pDbTable->errortext[0] = '\0';
		if((iNumOfCol < 1) || (iNumOfCol > pDbTable->table_NumOfColumnField))
		{
			ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: iNumOfCol value is invalid");
			return ZWP_DB_ERROR_DB_INSERT;
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pDbTable->dbhandler pointer is NULL");
		return ZWP_DB_ERROR_DB_INSERT;
	}

	snprintf(sql_stmt, ZWP_DB_SQL_STMT_BUFSIZE,
	         "INSERT INTO '%s' ( ", pDbTable->table_name);
	sql_stmt_length = strlen(sql_stmt);

	/* Fill up the column field name */
	for(i=0; i<iNumOfCol; i++)
	{
		if(i == 0)
		{
			snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
			         "%s", table_data[i].pcColumnName);
			sql_stmt_length += strlen(table_data[i].pcColumnName);
		}
		else
		{
			snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
			         ", %s", table_data[i].pcColumnName);
			sql_stmt_length += (strlen(table_data[i].pcColumnName) + 2);
		}
	}

	for(i=0; i<iNumOfCol; i++)
	{
		if(i == 0)
		{
			snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
			         ") VALUES ( ?");
			sql_stmt_length += 12;
		}
		else
		{
			snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
			         ", ?");
			sql_stmt_length += 3;
		}
	}
	snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
	         " ); COMMIT;");

	if(sqlite3_prepare_v2(sqldb_handler, sql_stmt, -1, &pStmt, 0) != SQLITE_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s %s", ZWP_DB_ERRORTEXT_DB_PREPARE,
		        sqlite3_errmsg(sqldb_handler));
		snprintf(pDbTable->errortext, ZWP_DB_SQL_ERRORTEXT_MAX, "ERROR: %s: Prepare failed: %s",
		         __func__, sqlite3_errmsg(sqldb_handler));
		return ZWP_DB_ERROR_DB_INSERT;
	}

	/* Fill up the column field data content. Now replace ? with actual data */
	for(i=0, k=1; i<iNumOfCol; i++, k++)
	{
		switch(table_data[i].eType)
		{
			case ZWP_DB_DATATYPE_BLOB :
				if(table_data[i].data.valuebyte != NULL)
				{
					sqlite3_bind_blob(pStmt, k, table_data[i].data.valuebyte, table_data[i].isize, SQLITE_STATIC);
				}
				else
				{
					/* For TEXT & BLOB, sqlite expect a valid ptr. For BLOB: SQlite will replace ? with x'' */
					/* See https://www.sqlite.org/nulls.html. Nulls are distinct in a UNIQUE column */
					sqlite3_bind_blob(pStmt, k, (const void *)ZWP_DB_DATA_EMPTY_BUFFER_INT, table_data[i].isize,
					                  SQLITE_STATIC);
				}
				break;

			case ZWP_DB_DATATYPE_INT :
				sqlite3_bind_int(pStmt, k, table_data[i].data.valueint);
				break;

			case ZWP_DB_DATATYPE_BIGINT :
				sqlite3_bind_int64(pStmt, k, table_data[i].data.valueint64);
				break;

			case ZWP_DB_DATATYPE_BIGINT_UNSIGNED :
				sqlite3_bind_int64(pStmt, k, table_data[i].data.valueint64);
				break;

			case ZWP_DB_DATATYPE_REAL :
				sqlite3_bind_double(pStmt, k, table_data[i].data.valuedouble);
				break;

			case ZWP_DB_DATATYPE_TEXT :
				if(table_data[i].data.valuestring != NULL)
				{
					sqlite3_bind_text(pStmt, k, table_data[i].data.valuestring, table_data[i].isize, SQLITE_STATIC);
				}
				else
				{
					/* For TEXT & BLOB, sqlite expect a valid ptr. For TEXT: SQlite will replace ? with '' */
					/* See https://www.sqlite.org/nulls.html. Nulls are distinct in a UNIQUE column */
					sqlite3_bind_text(pStmt, k, (const char *)ZWP_DB_DATA_EMPTY_BUFFER_INT, table_data[i].isize,
					                  SQLITE_STATIC);
				}
				break;

			case ZWP_DB_DATATYPE_NULL :
				sqlite3_bind_null(pStmt, k);
				break;

			default :
				sqlite3_bind_null(pStmt, k);
		}
	}
	if((retval = sqlite3_step(pStmt)) != SQLITE_DONE)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s %s", ZWP_DB_ERRORTEXT_DB_STEP,
		        sqlite3_errmsg(sqldb_handler));
		snprintf(pDbTable->errortext, ZWP_DB_SQL_ERRORTEXT_MAX, "ERROR: %s: Insert record failed: %s",
		         __func__, sqlite3_errmsg(sqldb_handler));
		if(retval == SQLITE_CONSTRAINT)
		{
			RetCode = ZWP_DB_ERROR_DB_CONSTRAINT;
		}
		else
		{
			RetCode = ZWP_DB_ERROR_DB_INSERT;
		}
	}
	else
	{
		RetCode = ZWP_DB_OK;
	}

	pExpanded_sql_pStmt_Mem = sqlite3_expanded_sql(pStmt);
	ZWP_LOG(ZWP_LOG_DEBUG, "SQL:%s", pExpanded_sql_pStmt_Mem);
	sqlite3_free(pExpanded_sql_pStmt_Mem);

	if(sqlite3_finalize(pStmt) != SQLITE_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", ZWP_DB_ERRORTEXT_DB_FINALIZE);
	}

	return RetCode;
}


/**
Deleting records in a table of Home ID database.
@param[in]      pDbTable            Table handler pointer.
@param[in]      iNumOfCol           Number of columns in this table_data.
@param[in]      table_data          Records which fulfill this table_data value will be deleted.
@return         Return error code if failure. If success will return ZERO.
@pre            pDbTable must be valid handler.
*/
zwp_status_t zwp_db_delete_record(zwp_db_table *pDbTable, int iNumOfCol,
                                  pzwp_db_table_value table_data)
{
	sqlite3 *sqldb_handler;
	sqlite3_stmt *pStmt;
	char sql_stmt[ZWP_DB_SQL_STMT_BUFSIZE];
	int  sql_stmt_length;
	int  i, k;
	int  RetCode;
	int  retval;
	char *pExpanded_sql_pStmt_Mem;

	if(pDbTable == NULL)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pDbTable pointer is NULL");
		return ZWP_DB_ERROR_DB_SELECT;
	}

	if(pDbTable->dbhandler != NULL)
	{
		sqldb_handler = (sqlite3 *) pDbTable->dbhandler;
		pDbTable->errortext[0] = '\0';
	}
	else
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pDbTable->dbhandler pointer is NULL");
		return ZWP_DB_ERROR_DB_SELECT;
	}

	if(strlen(pDbTable->table_name) < 1)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pDbTable->table_name is invalid");
		return ZWP_DB_ERROR_DB_SELECT;
	}

	if(iNumOfCol == 0)
	{
		snprintf(sql_stmt, ZWP_DB_SQL_STMT_BUFSIZE,
		         "DELETE FROM '%s'; COMMIT;", pDbTable->table_name);
	}
	else
	{
		snprintf(sql_stmt, ZWP_DB_SQL_STMT_BUFSIZE,
		         "DELETE FROM '%s' WHERE ( ", pDbTable->table_name);
		sql_stmt_length = strlen(sql_stmt);

		for(i=0; i<iNumOfCol; i++)
		{
			if(i == 0)
			{
				if(table_data[i].eType != ZWP_DB_DATATYPE_NULL)
				{
					snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
					         "%s = ? ", table_data[i].pcColumnName);
					sql_stmt_length += (strlen(table_data[i].pcColumnName)+5);
				}
				else
				{
					snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
					         "%s IS ? ", table_data[i].pcColumnName);
					sql_stmt_length += (strlen(table_data[i].pcColumnName)+6);
				}
			}
			else
			{
				if(table_data[i].eType != ZWP_DB_DATATYPE_NULL)
				{
					snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
					         "AND %s = ? ", table_data[i].pcColumnName);
					sql_stmt_length += (strlen(table_data[i].pcColumnName)+9);
				}
				else
				{
					snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
					         "AND %s IS ? ", table_data[i].pcColumnName);
					sql_stmt_length += (strlen(table_data[i].pcColumnName)+10);
				}
			}
		}
		snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
		         "); COMMIT;");
	}

	if(sqlite3_prepare_v2(sqldb_handler, sql_stmt, -1, &pStmt, 0) != SQLITE_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s %s", ZWP_DB_ERRORTEXT_DB_PREPARE,
		        sqlite3_errmsg(sqldb_handler));
		snprintf(pDbTable->errortext, ZWP_DB_SQL_ERRORTEXT_MAX, "ERROR: %s: Prepare failed: %s",
		         __func__, sqlite3_errmsg(sqldb_handler));
		return ZWP_DB_ERROR_DB_DELETE;
	}

	/* Fill up ? with actual value */
	for(i=0, k=1; i<iNumOfCol; i++, k++)
	{
		switch(table_data[i].eType)
		{
			case ZWP_DB_DATATYPE_BLOB :
				if(table_data[i].data.valuebyte != NULL)
				{
					sqlite3_bind_blob(pStmt, k, table_data[i].data.valuebyte, table_data[i].isize, SQLITE_STATIC);
				}
				else
				{
					sqlite3_bind_blob(pStmt, k, (const void *)ZWP_DB_DATA_EMPTY_BUFFER_INT, table_data[i].isize,
					                  SQLITE_STATIC);
				}
				break;

			case ZWP_DB_DATATYPE_INT :
				sqlite3_bind_int(pStmt, k, table_data[i].data.valueint);
				break;

			case ZWP_DB_DATATYPE_BIGINT :
				sqlite3_bind_int64(pStmt, k, table_data[i].data.valueint64);
				break;

			case ZWP_DB_DATATYPE_BIGINT_UNSIGNED :
				sqlite3_bind_int64(pStmt, k, table_data[i].data.valueint64);
				break;

			case ZWP_DB_DATATYPE_REAL :
				sqlite3_bind_double(pStmt, k, table_data[i].data.valuedouble);
				break;

			case ZWP_DB_DATATYPE_TEXT :
				if(table_data[i].data.valuestring != NULL)
				{
					sqlite3_bind_text(pStmt, k, table_data[i].data.valuestring, table_data[i].isize, SQLITE_STATIC);
				}
				else
				{
					/* For TEXT & BLOB, sqlite expect a valid ptr. For TEXT: SQlite will replace ? with '' */
					sqlite3_bind_text(pStmt, k, (const char *)ZWP_DB_DATA_EMPTY_BUFFER_INT, table_data[i].isize,
					                  SQLITE_STATIC);
				}
				break;

			case ZWP_DB_DATATYPE_NULL :
				sqlite3_bind_null(pStmt, k);
				break;

			default :
				sqlite3_bind_null(pStmt, k);
		}
	}

	if((retval = sqlite3_step(pStmt)) != SQLITE_DONE)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s %s", ZWP_DB_ERRORTEXT_DB_STEP,
		        sqlite3_errmsg(sqldb_handler));
		snprintf(pDbTable->errortext, ZWP_DB_SQL_ERRORTEXT_MAX, "ERROR: %s: Delete record failed: %s",
		         __func__, sqlite3_errmsg(sqldb_handler));
		RetCode = ZWP_DB_ERROR_DB_DELETE;
	}
	else
	{
		RetCode = ZWP_DB_OK;
	}

	pExpanded_sql_pStmt_Mem = sqlite3_expanded_sql(pStmt);
	ZWP_LOG(ZWP_LOG_DEBUG, "SQL:%s", pExpanded_sql_pStmt_Mem);
	sqlite3_free(pExpanded_sql_pStmt_Mem);

	if(sqlite3_finalize(pStmt) != SQLITE_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", ZWP_DB_ERRORTEXT_DB_FINALIZE);
	}

	return RetCode;
}


/**
Query with SQL SELECT command and return number of records selected.
@param[in]      pDbTable            Table handler pointer.
@param[in]      sql_query_stmt      SQL SELECT command string.
@return         Return number of records fulfill the SQL SELECT command.
@pre            pDbTable must be valid handler.
*/
int zwp_db_query_record_count(zwp_db_table *pDbTable, char *sql_query_stmt)
{
	sqlite3 *sqldb_handler;
	sqlite3_stmt *pStmt;
	char sql_stmt[ZWP_DB_SQL_STMT_BUFSIZE];
	int  sql_stmt_length;
	int  RecordCount;
	int  retval;
	int  iNumOfCols;
	int  iCol;
	int  i;

	if(pDbTable == NULL)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pDbTable pointer is NULL");
		return ZWP_DB_ERROR;
	}

	if(pDbTable->dbhandler != NULL)
	{
		sqldb_handler = (sqlite3 *) pDbTable->dbhandler;
		pDbTable->errortext[0] = '\0';
	}
	else
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pDbTable->dbhandler pointer is NULL");
		return ZWP_DB_ERROR;
	}

	snprintf(sql_stmt, ZWP_DB_SQL_STMT_BUFSIZE,
	         "SELECT COUNT(*) FROM (%s", sql_query_stmt);
	sql_stmt_length = strlen(sql_stmt);

	for(i=(sql_stmt_length-1); i>0; i--)
	{
		if(sql_stmt[i] == ';')
		{
			sql_stmt[i] = '\0';
			break;
		}
	}
	sql_stmt_length = strlen(sql_stmt);
	snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
	         ");");

	if(sqlite3_prepare_v2(sqldb_handler, sql_stmt, -1, &pStmt, 0) != SQLITE_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s %s", ZWP_DB_ERRORTEXT_DB_PREPARE,
		        sqlite3_errmsg(sqldb_handler));
		snprintf(pDbTable->errortext, ZWP_DB_SQL_ERRORTEXT_MAX, "ERROR: %s: Prepare failed: %s",
		         __func__, sqlite3_errmsg(sqldb_handler));
		return ZWP_DB_ERROR;
	}
	iNumOfCols = sqlite3_column_count(pStmt);
	RecordCount = ZWP_DB_ERROR;
	while(1)
	{
		/* Check fetch a row's status */
		retval = sqlite3_step(pStmt);
		if(retval == SQLITE_ROW)
		{
			/* SQLITE_ROW means continue fetching a row */
			for(iCol=0; iCol<iNumOfCols; iCol++)
			{
				if(strncmp(sqlite3_column_name(pStmt, iCol), "COUNT(*)", 8) == 0)
				{
					RecordCount = sqlite3_column_int(pStmt, iCol);
				}
			}
		}
		else if(retval == SQLITE_DONE)
		{
			/* All rows fetched, break loop now */
			break;
		}
		else
		{
			/* Unknown error encountered */
			ZWP_LOG(ZWP_LOG_DEBUG, "%s %s", ZWP_DB_ERRORTEXT_DB_STEP,
			        sqlite3_errmsg(sqldb_handler));
			snprintf(pDbTable->errortext, ZWP_DB_SQL_ERRORTEXT_MAX, "ERROR: %s: Query record count failed: %s",
			         __func__, sqlite3_errmsg(sqldb_handler));
			RecordCount = ZWP_DB_ERROR;
			break;
		}
	}

	if(sqlite3_finalize(pStmt) != SQLITE_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", ZWP_DB_ERRORTEXT_DB_FINALIZE);
	}

	return RecordCount;
}


/**
Query for records base on Query condition with sort condition. Memory will be allocated for contents of the records selected.
@param[in]      pDbTable            Table handler pointer.
@param[in]      iNumQueryCond       Number of columns in this QueryCond table value struct.
@param[in]      QueryCond           QueryCond table value struct.
@param[in]      iReqTotalRow        Limit the number of records to SELECT. Set to -1 if all records to be selected.
@param[in]      iNumSortCond        Number of items for SortCond struct.
@param[in]      SortCond            Specified the SortCond struct.
@param[out]     iNumColumnPerRecord Number of column in each record been SQL selected.
@param[out]     iNumRecordFound     Number of records been SQL selected.
@param[out]     table_data          Return the pointer to the contents of the records been SQL selected which fulfill the query condition.
@return         Return error code if failure. If success will return ZERO.
@pre            pDbTable must be valid handler.
*/
zwp_status_t zwp_db_query_record(zwp_db_table *pDbTable, int iNumQueryCond,
                                 pzwp_db_table_value QueryCond, int iReqTotalRow,
                                 int iNumSortCond, pzwp_db_sort_cond SortCond,
                                 int *iNumColumnPerRecord, int *iNumRecordFound, zwp_db_table_value **table_data)
{
	sqlite3 *sqldb_handler;
	sqlite3_stmt *pStmt;
	char sql_stmt[ZWP_DB_SQL_STMT_BUFSIZE];
	int  sql_stmt_length;
	int  RetCode;
	int  retval;
	int  iRow, iRowIndex;
	int  iNumOfRows;
	int  iCol;
	int  iNumOfCol;
	int  i, k;
	char *pExpanded_sql_pStmt_Mem;
	zwp_db_table_head  *pTableHead_MetaVector;
	zwp_db_table_value *pTableValue_MetaVector;

	if(pDbTable == NULL)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pDbTable pointer is NULL");
		return ZWP_DB_ERROR_DB_SELECT;
	}

	if(pDbTable->dbhandler != NULL)
	{
		sqldb_handler = (sqlite3 *) pDbTable->dbhandler;
		pDbTable->errortext[0] = '\0';
	}
	else
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pDbTable->dbhandler pointer is NULL");
		return ZWP_DB_ERROR_DB_SELECT;
	}

	if(iNumQueryCond == 0)
	{
		snprintf(sql_stmt, ZWP_DB_SQL_STMT_BUFSIZE,
		         "SELECT * FROM '%s'", pDbTable->table_name);
		sql_stmt_length = strlen(sql_stmt);
	}
	else
	{
		/* Fill up WHERE */
		snprintf(sql_stmt, ZWP_DB_SQL_STMT_BUFSIZE,
		         "SELECT * FROM '%s' WHERE ( ", pDbTable->table_name);
		sql_stmt_length = strlen(sql_stmt);

		if(iNumQueryCond > 0)
		{
			if(QueryCond[0].eType != ZWP_DB_DATATYPE_NULL)
			{
				snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
				         "%s = ? ", QueryCond[0].pcColumnName);
				sql_stmt_length += (strlen(QueryCond[0].pcColumnName)+5);
			}
			else
			{
				snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
				         "%s IS ? ", QueryCond[0].pcColumnName);
				sql_stmt_length += (strlen(QueryCond[0].pcColumnName)+6);
			}
		}

		for(i=1; i<iNumQueryCond; i++)
		{
			if(QueryCond[i].eType != ZWP_DB_DATATYPE_NULL)
			{
				snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
				         "AND %s = ? ", QueryCond[i].pcColumnName);
				sql_stmt_length += (strlen(QueryCond[i].pcColumnName)+9);
			}
			else
			{
				snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
				         "AND %s IS ? ", QueryCond[i].pcColumnName);
				sql_stmt_length += (strlen(QueryCond[i].pcColumnName)+10);
			}
		}
		snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
		         ")");
		sql_stmt_length = strlen(sql_stmt);
	}

	if((iNumSortCond > 0) && (SortCond != NULL))
	{
		/* Fill up ORDER BY, ascending, descending */
		if(SortCond[0].eSortType == ZWP_DB_SORTING_ASCENDING_ORDER)
		{
			snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
			         " ORDER BY %s ASC", SortCond[0].pcColumnName);
			sql_stmt_length += (strlen(SortCond[0].pcColumnName)+14);
		}
		else
		{
			snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
			         " ORDER BY %s DESC", SortCond[0].pcColumnName);
			sql_stmt_length += (strlen(SortCond[0].pcColumnName)+15);
		}

		for(i=1; i<iNumSortCond; i++)
		{
			if(SortCond[i].eSortType == ZWP_DB_SORTING_ASCENDING_ORDER)
			{
				snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
				         ", %s ASC", SortCond[i].pcColumnName);
				sql_stmt_length += (strlen(SortCond[i].pcColumnName)+6);
			}
			else
			{
				snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
				         ", %s DESC", SortCond[i].pcColumnName);
				sql_stmt_length += (strlen(SortCond[i].pcColumnName)+7);
			}
		}
	}

	if(iReqTotalRow >= 0)
	{
		/* Fill up LIMIT if iReqTotalRow >= 0 */
		snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
		         " LIMIT %d;", iReqTotalRow);
	}
	else
	{
		/* Else dump everything */
		snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
		         ";");
	}

	if(sqlite3_prepare_v2(sqldb_handler, sql_stmt, -1, &pStmt, 0) != SQLITE_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s %s", ZWP_DB_ERRORTEXT_DB_PREPARE,
		        sqlite3_errmsg(sqldb_handler));
		snprintf(pDbTable->errortext, ZWP_DB_SQL_ERRORTEXT_MAX, "ERROR: %s: Prepare failed: %s",
		         __func__, sqlite3_errmsg(sqldb_handler));
		return ZWP_DB_ERROR_DB_CONNECT;
	}
	iNumOfCol = sqlite3_column_count(pStmt);
	RetCode = ZWP_DB_ERROR;

	/* Fill up ? with actual value */
	for(i=0, k=1; i<iNumQueryCond; i++, k++)
	{
		switch(QueryCond[i].eType)
		{
			case ZWP_DB_DATATYPE_BLOB :
				if(QueryCond[i].data.valuebyte != NULL)
				{
					sqlite3_bind_blob(pStmt, k, QueryCond[i].data.valuebyte, QueryCond[i].isize, SQLITE_STATIC);
				}
				else
				{
					/* See https://www.sqlite.org/nulls.html. Nulls are distinct in a UNIQUE column */
					sqlite3_bind_blob(pStmt, k, (const void *)ZWP_DB_DATA_EMPTY_BUFFER_INT, QueryCond[i].isize,
					                  SQLITE_STATIC);
				}
				break;

			case ZWP_DB_DATATYPE_INT :
				sqlite3_bind_int(pStmt, k, QueryCond[i].data.valueint);
				break;

			case ZWP_DB_DATATYPE_BIGINT :
				sqlite3_bind_int64(pStmt, k, QueryCond[i].data.valueint64);
				break;

			case ZWP_DB_DATATYPE_BIGINT_UNSIGNED :
				sqlite3_bind_int64(pStmt, k, QueryCond[i].data.valueint64);
				break;

			case ZWP_DB_DATATYPE_REAL :
				sqlite3_bind_double(pStmt, k, QueryCond[i].data.valuedouble);
				break;

			case ZWP_DB_DATATYPE_TEXT :
				if(QueryCond[i].data.valuestring != NULL)
				{
					sqlite3_bind_text(pStmt, k, QueryCond[i].data.valuestring, QueryCond[i].isize, SQLITE_STATIC);
				}
				else
				{
					/* For TEXT & BLOB, sqlite expect a valid ptr. For TEXT: SQlite will replace ? with '' */
					sqlite3_bind_text(pStmt, k, (const char *)ZWP_DB_DATA_EMPTY_BUFFER_INT, QueryCond[i].isize,
					                  SQLITE_STATIC);
				}
				break;

			case ZWP_DB_DATATYPE_NULL :
				sqlite3_bind_null(pStmt, k);
				break;

			default :
				sqlite3_bind_null(pStmt, k);
		}
	}

	pExpanded_sql_pStmt_Mem = sqlite3_expanded_sql(pStmt);
	ZWP_LOG(ZWP_LOG_DEBUG, "SQL:%s", pExpanded_sql_pStmt_Mem);
	iNumOfRows = zwp_db_query_record_count(pDbTable, pExpanded_sql_pStmt_Mem);
	ZWP_LOG(ZWP_LOG_INFO, "Number of Records found: %d", iNumOfRows);
	sqlite3_free(pExpanded_sql_pStmt_Mem);

	if(iNumOfRows > 0)
	{
		/* Some number of rows of record found. Allocate Header & Value memory */
		pTableHead_MetaVector = zwp_db_table_head_malloc(pDbTable->table_NumOfColumnField);
		pTableValue_MetaVector = zwp_db_table_value_malloc((iNumOfCol * iNumOfRows));
		zwp_db_pragma_table_info(pDbTable, pDbTable->table_NumOfColumnField, pTableHead_MetaVector);
	}
	else
	{
		/* No record found. */
		pTableHead_MetaVector = NULL;
		pTableValue_MetaVector = NULL;
		RetCode = ZWP_DB_OK;	/* Indicate NO error, just found nothing */
	}
	/* Return Query Result */
	*table_data = pTableValue_MetaVector;
	*iNumRecordFound = 0;
	*iNumColumnPerRecord = 0;
	iRow = 0;
	if((iNumOfRows > 0) && (iNumOfCol > 0) && (pTableHead_MetaVector != NULL)
	        && (pTableValue_MetaVector != NULL))
	{
		*iNumRecordFound = iNumOfRows;
		*iNumColumnPerRecord = iNumOfCol;
		//ZWP_LOG(ZWP_LOG_INFO, "Num Of Cols: %d, Num Of Records: %d", iNumOfCol, iNumOfRows);
		while(iNumOfRows > 0)
		{
			/* Check fetch a row's status */
			retval = sqlite3_step(pStmt);
			if(retval == SQLITE_ROW)
			{
				/* SQLITE_ROW means continue fetching a row */
				for(iCol=0; iCol<iNumOfCol; iCol++)
				{
					void *ptable_column_name_malloc;
					void *ptable_column_data_malloc;
					char *pCol_FieldName;
					int  Col_FieldName_Len;

					RetCode = ZWP_DB_OK;
					iRowIndex = (iRow * iNumOfCol);
					if(iRow == 0)
					{
						/* Duplicate and malloc column name only for 1st row */
						pCol_FieldName = (char *)sqlite3_column_name(pStmt, iCol);
						Col_FieldName_Len = strlen(pCol_FieldName);
						ptable_column_name_malloc = zwp_db_table_column_data_malloc_copy(Col_FieldName_Len, pCol_FieldName);
						//printf("%p %s\n", ptable_column_name_malloc, (char *) ptable_column_name_malloc);
						pTableValue_MetaVector[(iRowIndex + iCol)].pcColumnName = (char *) ptable_column_name_malloc;
					}
					else
					{
						/* Re-use column name from 1st row for subsequence row */
						pTableValue_MetaVector[(iRowIndex + iCol)].pcColumnName = pTableValue_MetaVector[iCol].pcColumnName;
					}
					switch(sqlite3_column_type(pStmt, iCol))
					{
						case SQLITE_INTEGER :
							pTableValue_MetaVector[(iRowIndex + iCol)].eType = pTableHead_MetaVector[iCol].eColumnType;
							if(pTableHead_MetaVector[iCol].eColumnType == ZWP_DB_DATATYPE_INT)
							{
								pTableValue_MetaVector[(iRowIndex + iCol)].data.valueint = sqlite3_column_int(pStmt, iCol);
								//ZWP_LOG(ZWP_LOG_DEBUG, "SQLITE_INTEGER(INT): %s, %d",
								//        pTableValue_MetaVector[(iRowIndex + iCol)].pcColumnName,
								//        pTableValue_MetaVector[(iRowIndex + iCol)].data.valueint);
							}
							else if(pTableHead_MetaVector[iCol].eColumnType == ZWP_DB_DATATYPE_BIGINT)
							{
								pTableValue_MetaVector[(iRowIndex + iCol)].data.valueint64 = sqlite3_column_int64(pStmt, iCol);
								//ZWP_LOG(ZWP_LOG_DEBUG, "SQLITE_INTEGER(BIGINT): %s, %" PRId64,
								//        pTableValue_MetaVector[(iRowIndex + iCol)].pcColumnName,
								//        pTableValue_MetaVector[(iRowIndex + iCol)].data.valueint64);
							}
							else if(pTableHead_MetaVector[iCol].eColumnType == ZWP_DB_DATATYPE_BIGINT_UNSIGNED)
							{
								pTableValue_MetaVector[(iRowIndex + iCol)].data.valueint64 = sqlite3_column_int64(pStmt, iCol);
								//ZWP_LOG(ZWP_LOG_DEBUG, "SQLITE_INTEGER(BIGINT_UNSIGNED): %s, %" PRIu64,
								//        pTableValue_MetaVector[(iRowIndex + iCol)].pcColumnName,
								//        pTableValue_MetaVector[(iRowIndex + iCol)].data.valueint64);
							}
							break;

						case SQLITE_FLOAT :
							pTableValue_MetaVector[(iRowIndex + iCol)].data.valuedouble = sqlite3_column_double(pStmt, iCol);
							pTableValue_MetaVector[(iRowIndex + iCol)].eType = ZWP_DB_DATATYPE_REAL;
							//ZWP_LOG(ZWP_LOG_DEBUG, "SQLITE_FLOAT: %s, %f",
							//        pTableValue_MetaVector[(iRowIndex + iCol)].pcColumnName,
							//        pTableValue_MetaVector[(iRowIndex + iCol)].data.valuedouble);
							break;

						case SQLITE3_TEXT :
							pTableValue_MetaVector[(iRowIndex + iCol)].isize = sqlite3_column_bytes(pStmt, iCol);
							pTableValue_MetaVector[(iRowIndex + iCol)].eType = ZWP_DB_DATATYPE_TEXT;
							ptable_column_data_malloc = zwp_db_table_column_data_malloc_copy(
							                                pTableValue_MetaVector[(iRowIndex + iCol)].isize,
							                                (void *) sqlite3_column_text(pStmt, iCol));
							pTableValue_MetaVector[(iRowIndex + iCol)].data.valuestring = ptable_column_data_malloc;
							//ZWP_LOG(ZWP_LOG_DEBUG, "SQLITE_TEXT: %s, %p, %s",
							//        pTableValue_MetaVector[(iRowIndex + iCol)].pcColumnName,
							//        pTableValue_MetaVector[(iRowIndex + iCol)].data.valuestring,
							//        pTableValue_MetaVector[(iRowIndex + iCol)].data.valuestring);
							break;

						case SQLITE_BLOB :
							pTableValue_MetaVector[(iRowIndex + iCol)].isize = sqlite3_column_bytes(pStmt, iCol);
							pTableValue_MetaVector[(iRowIndex + iCol)].eType = ZWP_DB_DATATYPE_BLOB;
							ptable_column_data_malloc = zwp_db_table_column_data_malloc_copy(
							                                pTableValue_MetaVector[(iRowIndex + iCol)].isize,
							                                (void *) sqlite3_column_blob(pStmt, iCol));
							pTableValue_MetaVector[(iRowIndex + iCol)].data.valuebyte = ptable_column_data_malloc;
							//ZWP_LOG(ZWP_LOG_DEBUG, "SQLITE_BLOB: %s, %p",
							//        pTableValue_MetaVector[(iRowIndex + iCol)].pcColumnName,
							//        pTableValue_MetaVector[(iRowIndex + iCol)].data.valuebyte);
							break;

						case SQLITE_NULL :
							pTableValue_MetaVector[(iRowIndex + iCol)].isize = 0;
							pTableValue_MetaVector[(iRowIndex + iCol)].eType = ZWP_DB_DATATYPE_NULL;
							//ZWP_LOG(ZWP_LOG_DEBUG, "SQLITE_NULL: %s",
							//        pTableValue_MetaVector[(iRowIndex + iCol)].pcColumnName);
							break;
					}
				}
				iRow++;
			}
			else if(retval == SQLITE_DONE)
			{
				/* All rows fetched, break loop now */
				break;
			}
			else
			{
				/* Unknown error encountered */
				ZWP_LOG(ZWP_LOG_DEBUG, "%s %s", ZWP_DB_ERRORTEXT_DB_STEP,
				        sqlite3_errmsg(sqldb_handler));
				snprintf(pDbTable->errortext, ZWP_DB_SQL_ERRORTEXT_MAX, "ERROR: %s: Query record failed: %s",
				         __func__, sqlite3_errmsg(sqldb_handler));
				RetCode = ZWP_DB_ERROR_DB_DUMP;
				break;
			}
		}
	}

	zwp_db_table_head_free(pTableHead_MetaVector);
	if(sqlite3_finalize(pStmt) != SQLITE_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", ZWP_DB_ERRORTEXT_DB_FINALIZE);
	}

	return RetCode;
}


/**
Free the memory of records allocated by zwp_db_query_record.
@param[in]      iNumColumnPerRecord Number of column return by zwp_db_query_record.
@param[in]      iNumRecordFound     Number of records return by zwp_db_query_record.
@param[in]      table_data          Pointer to the contents of the records been SQL selected by zwp_db_query_record.
@return         Nil.
@pre            table_data must be valid pointer given by zwp_db_query_record.
*/
void zwp_db_query_record_free(int iNumColumnPerRecord, int iNumRecordFound,
                              zwp_db_table_value *table_data)
{
	if(table_data != NULL)
	{
		zwp_db_table_value_free(iNumColumnPerRecord, iNumRecordFound, table_data);
	}
}


/**
Updating of records base on query condition.
@param[in]      pDbTable            Table handler pointer.
@param[in]      iNumQueryCond       Number of columns in this QueryCond table value struct.
@param[in]      QueryCond           QueryCond table value struct.
@param[in]      iNumOfCol_toupdate  Number of column in table_data_toupdate to be updated.
@param[in]      table_data_toupdate Column data content to be update in query selected records.
@param[out]     iNumRecordUpdated   Number of records updated.
@return         Return error code if failure. If success will return ZERO.
@pre            pDbTable must be valid handler.
*/
zwp_status_t zwp_db_update_record(zwp_db_table *pDbTable, int iNumQueryCond,
                                  pzwp_db_table_value QueryCond, int iNumOfCol_toupdate,
                                  zwp_db_table_value *table_data_toupdate, int *iNumRecordUpdated)
{
	sqlite3 *sqldb_handler;
	sqlite3_stmt *pStmt;
	char sql_stmt[ZWP_DB_SQL_STMT_BUFSIZE];
	int  sql_stmt_length;
	int  RetCode;
	int  retval;
	int  i, k;
	char *pExpanded_sql_pStmt_Mem;

	if(pDbTable == NULL)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pDbTable pointer is NULL");
		return ZWP_DB_ERROR_DB_SELECT;
	}

	if(pDbTable->dbhandler != NULL)
	{
		sqldb_handler = (sqlite3 *) pDbTable->dbhandler;
		pDbTable->errortext[0] = '\0';
	}
	else
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", "Param: pDbTable->dbhandler pointer is NULL");
		return ZWP_DB_ERROR_DB_SELECT;
	}

	snprintf(sql_stmt, ZWP_DB_SQL_STMT_BUFSIZE,
	         "UPDATE '%s' SET ", pDbTable->table_name);
	sql_stmt_length = strlen(sql_stmt);

	for(i=0; i<iNumOfCol_toupdate; i++)
	{
		if(i == 0)
		{
			snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
			         "%s = ?", table_data_toupdate[i].pcColumnName);
			sql_stmt_length += (strlen(table_data_toupdate[i].pcColumnName) + 4);
		}
		else
		{
			snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
			         ", %s = ?", table_data_toupdate[i].pcColumnName);
			sql_stmt_length += (strlen(table_data_toupdate[i].pcColumnName) + 6);
		}
	}

	if(iNumQueryCond > 0)
	{
		snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
		         " WHERE ( ");
		sql_stmt_length += 9;

		if(QueryCond[0].eType == ZWP_DB_DATATYPE_NULL)
		{
			snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
			         "%s IS ? ", QueryCond[0].pcColumnName);
			sql_stmt_length += (strlen(QueryCond[0].pcColumnName)+6);
		}
		else if(QueryCond[0].eType == ZWP_DB_DATATYPE_QUERY)
		{
			snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
			         "%s IN %s ", QueryCond[0].pcColumnName, QueryCond[0].data.valuestring);
			sql_stmt_length += (strlen(QueryCond[0].pcColumnName)+strlen(QueryCond[0].data.valuestring)+5);
		}
		else
		{
			snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
			         "%s = ? ", QueryCond[0].pcColumnName);
			sql_stmt_length += (strlen(QueryCond[0].pcColumnName)+5);
		}
	}

	for(i=1; i<iNumQueryCond; i++)
	{
		if(QueryCond[i].eType == ZWP_DB_DATATYPE_NULL)
		{
			snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
			         "AND %s IS ? ", QueryCond[i].pcColumnName);
			sql_stmt_length += (strlen(QueryCond[i].pcColumnName)+10);
		}
		else if(QueryCond[i].eType == ZWP_DB_DATATYPE_QUERY)
		{
			snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
			         "AND %s IN %s ", QueryCond[i].pcColumnName, QueryCond[i].data.valuestring);
			sql_stmt_length += (strlen(QueryCond[i].pcColumnName)+strlen(QueryCond[i].data.valuestring)+9);
		}
		else
		{
			snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
			         "AND %s = ? ", QueryCond[i].pcColumnName);
			sql_stmt_length += (strlen(QueryCond[i].pcColumnName)+9);
		}
	}
	snprintf((sql_stmt + sql_stmt_length), (ZWP_DB_SQL_STMT_BUFSIZE - sql_stmt_length),
	         ");");
	sql_stmt_length = strlen(sql_stmt);

	if(sqlite3_prepare_v2(sqldb_handler, sql_stmt, -1, &pStmt, 0) != SQLITE_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s %s", ZWP_DB_ERRORTEXT_DB_PREPARE,
		        sqlite3_errmsg(sqldb_handler));
		snprintf(pDbTable->errortext, ZWP_DB_SQL_ERRORTEXT_MAX, "ERROR: %s: Prepare failed: %s",
		         __func__, sqlite3_errmsg(sqldb_handler));
		return ZWP_DB_ERROR_DB_CONNECT;
	}

	/* Fill up ? with actual value for SET */
	for(i=0, k=1; i<iNumOfCol_toupdate; i++, k++)
	{
		switch(table_data_toupdate[i].eType)
		{
			case ZWP_DB_DATATYPE_BLOB :
				if(table_data_toupdate[i].data.valuebyte != NULL)
				{
					sqlite3_bind_blob(pStmt, k, table_data_toupdate[i].data.valuebyte,
					                  table_data_toupdate[i].isize, SQLITE_STATIC);
				}
				else
				{
					/* See https://www.sqlite.org/nulls.html. Nulls are distinct in a UNIQUE column */
					sqlite3_bind_blob(pStmt, k, (const void *)ZWP_DB_DATA_EMPTY_BUFFER_INT,
					                  table_data_toupdate[i].isize, SQLITE_STATIC);
				}
				break;

			case ZWP_DB_DATATYPE_INT :
				sqlite3_bind_int(pStmt, k, table_data_toupdate[i].data.valueint);
				break;

			case ZWP_DB_DATATYPE_BIGINT :
				sqlite3_bind_int64(pStmt, k, table_data_toupdate[i].data.valueint64);
				break;

			case ZWP_DB_DATATYPE_BIGINT_UNSIGNED :
				sqlite3_bind_int64(pStmt, k, table_data_toupdate[i].data.valueint64);
				break;

			case ZWP_DB_DATATYPE_REAL :
				sqlite3_bind_double(pStmt, k, table_data_toupdate[i].data.valuedouble);
				break;

			case ZWP_DB_DATATYPE_TEXT :
				if(table_data_toupdate[i].data.valuestring != NULL)
				{
					sqlite3_bind_text(pStmt, k, table_data_toupdate[i].data.valuestring,
					                  table_data_toupdate[i].isize, SQLITE_STATIC);
				}
				else
				{
					/* For TEXT & BLOB, sqlite expect a valid ptr. For TEXT: SQlite will replace ? with '' */
					sqlite3_bind_text(pStmt, k, (const char *)ZWP_DB_DATA_EMPTY_BUFFER_INT,
					                  table_data_toupdate[i].isize, SQLITE_STATIC);
				}
				break;

			case ZWP_DB_DATATYPE_NULL :
				sqlite3_bind_null(pStmt, k);
				break;

			default :
				sqlite3_bind_null(pStmt, k);
		}
	}

	/* Fill up ? with actual value for WHERE */
	for(i=0, k=(iNumOfCol_toupdate+1); i<iNumQueryCond; i++, k++)
	{
		switch(QueryCond[i].eType)
		{
			case ZWP_DB_DATATYPE_BLOB :
				if(QueryCond[i].data.valuebyte != NULL)
				{
					sqlite3_bind_blob(pStmt, k, QueryCond[i].data.valuebyte, QueryCond[i].isize, SQLITE_STATIC);
				}
				else
				{
					/* See https://www.sqlite.org/nulls.html. Nulls are distinct in a UNIQUE column */
					sqlite3_bind_blob(pStmt, k, (const void *)ZWP_DB_DATA_EMPTY_BUFFER_INT, QueryCond[i].isize,
					                  SQLITE_STATIC);
				}
				break;

			case ZWP_DB_DATATYPE_INT :
				sqlite3_bind_int(pStmt, k, QueryCond[i].data.valueint);
				break;

			case ZWP_DB_DATATYPE_BIGINT :
				sqlite3_bind_int64(pStmt, k, QueryCond[i].data.valueint64);
				break;

			case ZWP_DB_DATATYPE_BIGINT_UNSIGNED :
				sqlite3_bind_int64(pStmt, k, QueryCond[i].data.valueint64);
				break;

			case ZWP_DB_DATATYPE_REAL :
				sqlite3_bind_double(pStmt, k, QueryCond[i].data.valuedouble);
				break;

			case ZWP_DB_DATATYPE_TEXT :
				if(QueryCond[i].data.valuestring != NULL)
				{
					sqlite3_bind_text(pStmt, k, QueryCond[i].data.valuestring, QueryCond[i].isize, SQLITE_STATIC);
				}
				else
				{
					/* See https://www.sqlite.org/nulls.html. Nulls are distinct in a UNIQUE column */
					sqlite3_bind_text(pStmt, k, (const char *)ZWP_DB_DATA_EMPTY_BUFFER_INT, QueryCond[i].isize,
					                  SQLITE_STATIC);
				}
				break;

			case ZWP_DB_DATATYPE_QUERY :
				k--;	/* There is no ? in ZWP_DB_DATATYPE_QUERY, so no increment */
				break;

			case ZWP_DB_DATATYPE_NULL :
				sqlite3_bind_null(pStmt, k);
				break;

			default :
				sqlite3_bind_null(pStmt, k);
		}
	}

	pExpanded_sql_pStmt_Mem = sqlite3_expanded_sql(pStmt);
	ZWP_LOG(ZWP_LOG_DEBUG, "SQL:%s", pExpanded_sql_pStmt_Mem);
	sqlite3_free(pExpanded_sql_pStmt_Mem);

	if((retval = sqlite3_step(pStmt)) == SQLITE_DONE)
	{
		RetCode = ZWP_DB_OK;
	}
	else
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s %s", ZWP_DB_ERRORTEXT_DB_STEP,
		        sqlite3_errmsg(sqldb_handler));
		snprintf(pDbTable->errortext, ZWP_DB_SQL_ERRORTEXT_MAX, "ERROR: %s: Update record failed: %s",
		         __func__, sqlite3_errmsg(sqldb_handler));
		if(retval == SQLITE_CONSTRAINT)
		{
			RetCode = ZWP_DB_ERROR_DB_CONSTRAINT;
		}
		else
		{
			RetCode = ZWP_DB_ERROR_DB_UPDATE;
		}
	}

	*iNumRecordUpdated = sqlite3_changes(sqldb_handler);
	if(sqlite3_finalize(pStmt) != SQLITE_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "%s", ZWP_DB_ERRORTEXT_DB_FINALIZE);
		*iNumRecordUpdated = 0;
	}

	return RetCode;
}
