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
 * @file    zwp_db_internal.h
 *
 * @brief   Database management module.
 * @details APIs related to Database management.
 *
 * @author  Frank
 *
 * @version 1.0 - 2017-01-19
 * - Initial version
 */

#ifndef __ZWP_DB_INTERNAL_H_
#define __ZWP_DB_INTERNAL_H_

#include <sqlite3.h>
#include "zwp_status.h"
#include "zwp_db.h"

#define ZWP_DB_DB_FILENAME_MAX			256
#define ZWP_DB_SQL_STMT_BUFSIZE			1024
#define ZWP_DB_SQL_STMT_VARCHAR_MAX		256
#define ZWP_DB_SQL_TABLENAME_MAX		32
#define ZWP_DB_SQL_ERRORTEXT_MAX		200

typedef enum zwp_db_error_types_
{
	ZWP_DB_ERROR                      = -1,
	ZWP_DB_OK                         = 0,
	ZWP_DB_ERROR_DB_UNKNOWN_DB_NAME   = 1,
	ZWP_DB_ERROR_DB_CREATE            = 2,
	ZWP_DB_ERROR_DB_CONNECT           = 3,
	ZWP_DB_ERROR_DB_SELECT            = 4,
	ZWP_DB_ERROR_DB_SEARCH_NOT_FOUND  = 5,
	ZWP_DB_ERROR_DB_DUMP              = 6,
	ZWP_DB_ERROR_DB_UPDATE            = 7,
	ZWP_DB_ERROR_DB_INSERT            = 8,
	ZWP_DB_ERROR_DB_DELETE            = 9,
	ZWP_DB_ERROR_DB_FINALIZE          = 10,
	ZWP_DB_ERROR_DB_CLOSE             = 11,
	ZWP_DB_ERROR_DB_CONSTRAINT        = 12,
	ZWP_DB_ERROR_SQL_MSG              = 13,
	ZWP_DB_ERROR_UNKNOWN              = 14
} zwp_db_error_types;

typedef struct zwp_db_db_t_
{
	sqlite3  *sqldb_handler;
	char     db_path_filename[ZWP_DB_DB_FILENAME_MAX];
} zwp_db_db;

typedef struct zwp_db_table_t_
{
	sqlite3  *dbhandler;
	void     *table_ptr;
	int      table_NumOfColumnField;
	char     table_name[ZWP_DB_SQL_TABLENAME_MAX];
	int      max_blob_size;
	int      max_text_size;
	char     errortext[ZWP_DB_SQL_ERRORTEXT_MAX];
} zwp_db_table;

/* ---------- Bit Values ---------- */
#define BIT0		0x01
#define BIT1		0x02
#define BIT2		0x04
#define BIT3		0x08
#define BIT4		0x10
#define BIT5		0x20
#define BIT6		0x40
#define BIT7		0x80

/* ---------- Register Macro ---------- */
#define RegisterSet(reg, mask)        \
	(reg) |= (mask)

#define RegisterUnSet(reg, mask)      \
	(reg) &= ~(mask)

#define RegisterRd(reg, mask, val)    \
	(val) = (reg) & (mask)

#define RegisterWr(reg, mask, val)    \
	{                               \
		int _TempReg;               \
		_TempReg = (reg);           \
		_TempReg &= ~(mask);        \
		_TempReg |= (val) & (mask); \
		(reg) = _TempReg;           \
	}

#define RegisterChk(reg, mask)        \
	((reg)&(mask))


/**
Get table column header declaration information from table handler.
@param[in]      pDbTable            Table handler pointer.
@param[in]      iNumOfColumnField   Number of columns in this table header.
@param[in,out]  pTableHead_Vector   This table header declaration structure.
@return         Return error code if failure. If success will return ZERO.
@pre            pDbTable must be valid handler.
*/
zwp_status_t zwp_db_pragma_table_info(zwp_db_table *pDbTable, int iNumOfColumnField,
                                      pzwp_db_table_head pTableHead_Vector);


/**
Malloc memory for struct zwp_db_table_head_t_. Each table column is 1 header struct.
@param[in]      sz                  Number of header struct required.
@return         Return table header struct pointer.
@pre            Nil.
*/
zwp_db_table_head *zwp_db_table_head_malloc(size_t sz);


/**
Free memory for struct zwp_db_table_head_t_ allocated by zwp_db_table_head_malloc.
@param[in]      pTableHead_Vector   Header struct pointer.
@return         Nil.
@pre            Nil.
*/
void zwp_db_table_head_free(struct zwp_db_table_head_t_ *pTableHead_Vector);


/**
Malloc memory for struct zwp_db_table_value_t_. Each column value is 1 value struct.
@param[in]      sz                  Number of value struct required.
@return         Return table value struct pointer.
@pre            Nil.
*/
zwp_db_table_value *zwp_db_table_value_malloc(size_t sz);


/**
Free memory for struct zwp_db_table_value_t_ allocated by zwp_db_table_value_malloc.
@param[in]      iNumOfCol            Number of column per record.
@param[in]      iNumOfRow            Number of row of records.
@param[in]      pTableValue_Vector   Table value struct pointer.
@return         Return error code if failure. If success will return ZERO.
@pre            pDbTable must be valid handler.
*/
int zwp_db_table_value_free(int iNumOfCol, int iNumOfRow, zwp_db_table_value *pTableValue_Vector);


/**
Malloc memory buffer then memcopy source address to the new memory. Duplicate memory.
@param[in]      sz                   Number of bytes to malloc.
@param[in]      sql_column_data_src  Source memory pointer to copy from.
@return         Return a new memory pointer.
@pre            Nil.
*/
void *zwp_db_table_column_data_malloc_copy(size_t sz, void *sql_column_data_src);


/**
Query with SQL SELECT command and return number of records selected.
@param[in]      pDbTable            Table handler pointer.
@param[in]      sql_query_stmt      SQL SELECT command string.
@return         Return number of records fulfill the SQL SELECT command.
@pre            pDbTable must be valid handler.
*/
int zwp_db_query_record_count(zwp_db_table *pDbTable, char *sql_query_stmt);

#endif
