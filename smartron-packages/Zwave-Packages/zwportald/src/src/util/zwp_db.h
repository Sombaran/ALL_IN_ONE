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
 * @file    zwp_db.h
 *
 * @brief   Database management module.
 * @details APIs related to Database management.
 *
 * @author  Frank
 *
 * @version 1.0 - 2017-01-19
 * - Initial version
 */

#ifndef __ZWP_DB_H_
#define __ZWP_DB_H_

#include <stdint.h>

#include "zwp_status.h"

struct zwp_db_db_t_;
typedef struct zwp_db_db_t_ zwp_db_db;
typedef struct zwp_db_db_t_ *pzwp_db_db;

struct zwp_db_table_t_;
typedef struct zwp_db_table_t_ zwp_db_table;
typedef struct zwp_db_table_t_ *pzwp_db_table;

/* Info:
 * http://dev.mysql.com/doc/refman/5.7/en/integer-types.html
 * http://dev.mysql.com/doc/refman/5.7/en/numeric-type-overview.html
 * https://www.sqlite.org/datatype3.html
 */
typedef enum zwp_db_datatypes_
{
	ZWP_DB_DATATYPE_BLOB,
	ZWP_DB_DATATYPE_INT,
	ZWP_DB_DATATYPE_BIGINT,
	ZWP_DB_DATATYPE_BIGINT_UNSIGNED,
	ZWP_DB_DATATYPE_REAL,
	ZWP_DB_DATATYPE_TEXT,
	ZWP_DB_DATATYPE_QUERY,
	ZWP_DB_DATATYPE_NULL
} zwp_db_datatypes;

typedef enum zwp_db_sorting_order_
{
	ZWP_DB_SORTING_ASCENDING_ORDER,
	ZWP_DB_SORTING_DESCENDING_ORDER
} zwp_db_sorting_type;

typedef struct zwp_db_table_head_t_
{
	char				*pcColumnName;		//Name of the column, such as "vType", "vLevel", "zType", "zEvent" etc
	zwp_db_datatypes	eColumnType;		//an enum consist of all the types the database can support
	int					isize;				//Max size required for this column.
	int					iProperty;			//Bit mask property of the field
} zwp_db_table_head, *pzwp_db_table_head;

typedef struct zwp_db_table_value_t_
{
	char				*pcColumnName;		//Name of the column, such as "vType", "vLevel", "zType", "zEvent" etc
	zwp_db_datatypes	eType;				//type of the data
	int					isize;				//size of the data

	union
	{
		char			*valuestring;		//string value if the type is type_string with NULL terminated (ZWP_DB_DATATYPE_TEXT)
		int				valueint;			//integer value if the type is type_int, type_bool (ZWP_DB_DATATYPE_INT)
		int64_t			valueint64;			//64bit integer value if the type is type_int64 (ZWP_DB_DATATYPE_BIGINT)
		double			valuedouble;		//double/float/real value if type is type_real (ZWP_DB_DATATYPE_REAL)
		uint8_t			*valuebyte;			//Binary value if the type is type_binary (ZWP_DB_DATATYPE_BLOB)
	} data;
} zwp_db_table_value, *pzwp_db_table_value;

typedef struct zwp_db_sort_cond_t_
{
	char					*pcColumnName;		//Name of the column, such as "vType", "vLevel", "zType", "zEvent" etc
	zwp_db_sorting_type		eSortType;			//an enum consist of all the types the database can support
} zwp_db_sort_cond, *pzwp_db_sort_cond;


#define ZWP_DB_PROPERTY_PRIMARY_KEY		0x01	//BIT0
#define ZWP_DB_PROPERTY_NOT_NULL		0x02	//BIT1


/**
Opening a Home ID database. Create it if database does not exist.
@param[in]      db_dir              Database directory path
@param[in]      iHome_id            Home ID
@param[in,out]  ppzwp_db_dbhandler  zwp_db db handler. If failure, handler is NULL. If success handler is valid.
@return         Return error code if failure. If success will return ZERO.
@pre            NIL.
*/
zwp_status_t zwp_db_open_db(char *db_dir, int64_t iHome_id, zwp_db_db **ppzwp_db_dbhandler);


/**
Opening a zwp_db database by filename. Create it if database does not exist.
@param[in]      db_file             Database filename including full directory path
@return         Return zwp_db db handler. If failure, handler is NULL. If success handler is valid.
@pre            Please make sure directory path already exist.
*/
zwp_db_db *zwp_db_open_db_by_filename(char *db_file);


/**
Closing a Home ID database.
@param[in]      pzwp_db_dbhandler   zwp_db db handler. If success, handler reset to NULL.
@return         Return error code if failure. If success will return ZERO.
@pre            pzwp_db_dbhandler must be valid handler.
*/
zwp_status_t zwp_db_close_db(zwp_db_db *pzwp_db_dbhandler);


/**
Delete a Home ID database.
@param[in]      pzwp_db_dbhandler   zwp_db db handler. If success, database will be closed, handler reset to NULL.
@return         Return error code if failure. If success will return ZERO.
@pre            pzwp_db_dbhandler must be valid handler.
*/
zwp_status_t zwp_db_delete_db(zwp_db_db *pzwp_db_dbhandler);


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
                                 pzwp_db_table_head ptable_head_list, zwp_db_table **ppDbTable);


/**
Opening a table in a Home ID database if table already exist.
@param[in]      pzwp_db_dbhandler   zwp_db db handler.
@param[in]      pTableName          Table Name in database.
@param[out]     ppDbTable           Table handler pointer.
@return         Return error code if failure. If success will return ZERO.
@pre            pzwp_db_dbhandler must be valid handler.
*/
zwp_status_t zwp_db_open_table(zwp_db_db *pzwp_db_dbhandler, char *pTableName,
                               zwp_db_table **ppDbTable);


/**
Close a table handler in a Home ID database.
@param[in]      pDbTable            Table handler pointer.
@return         Return error code if failure. If success will return ZERO.
@pre            pDbTable must be valid handler.
*/
zwp_status_t zwp_db_close_table(zwp_db_table *pDbTable);


/**
Delete a table in a Home ID database if table already exist.
@param[in]      pDbTable            Table handler pointer.
@return         Return error code if failure. If success will return ZERO.
@pre            pDbTable must be valid handler.
*/
zwp_status_t zwp_db_drop_table(zwp_db_table *pDbTable);


/**
Get error text message from table handler.
@param[in]      pDbTable            Table handler pointer.
@return         Return error text message buffer pointer.
@pre            pDbTable must be valid handler.
*/
char *zwp_db_get_table_errortext(zwp_db_table *pDbTable);


/**
Inserting a record to a table in a Home ID database.
@param[in]      pDbTable            Table handler pointer.
@param[in]      iNumOfCol           Number of columns in this table_data.
@param[in]      table_data          Table data value specified using a value struct.
@return         Return error code if failure. If success will return ZERO.
@pre            pDbTable must be valid handler.
*/
zwp_status_t zwp_db_insert_record(zwp_db_table *pDbTable, int iNumOfCol,
                                  pzwp_db_table_value table_data);


/**
Deleting records in a table of Home ID database.
@param[in]      pDbTable            Table handler pointer.
@param[in]      iNumOfCol           Number of columns in this table_data.
@param[in]      table_data          Records which fulfill this table_data value will be deleted.
@return         Return error code if failure. If success will return ZERO.
@pre            pDbTable must be valid handler.
*/
zwp_status_t zwp_db_delete_record(zwp_db_table *pDbTable, int iNumOfCol,
                                  pzwp_db_table_value table_data);


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
                                 int *iNumColumnPerRecord, int *iNumRecordFound, zwp_db_table_value **table_data);


/**
Free the memory of records allocated by zwp_db_query_record.
@param[in]      iNumColumnPerRecord Number of column return by zwp_db_query_record.
@param[in]      iNumRecordFound     Number of records return by zwp_db_query_record.
@param[in]      table_data          Pointer to the contents of the records been SQL selected by zwp_db_query_record.
@return         Nil.
@pre            table_data must be valid pointer given by zwp_db_query_record.
*/
void zwp_db_query_record_free(int iNumColumnPerRecord, int iNumRecordFound,
                              zwp_db_table_value *table_data);


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
                                  zwp_db_table_value *table_data_toupdate, int *iNumRecordUpdated);

#endif
