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
ANYONE EXCEPT LICENSEES OF SIGMA DESIGNS  WHO HAVE EXECUTED A SIGMA DESIGNS’ 
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
COMPANY’S NEEDS AND COMPANY’S USE OF THE SIGMA PROGRAM IS AT COMPANY’S 
OWN DISCRETION AND RISK. SIGMA DOES NOT GUARANTEE THAT THE USE OF THE SIGMA 
PROGRAM IN A THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES ENVIRONMENT WILL 
BE: (A) PERFORMED ERROR-FREE OR UNINTERRUPTED; (B) THAT SIGMA WILL CORRECT ANY 
THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT ERRORS; (C) THE 
THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT WILL OPERATE IN 
COMBINATION WITH COMPANY’S CONTENT OR COMPANY APPLICATIONS THAT UTILIZE THE 
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
Activities.Without limiting Sigma’s obligation of confidentiality as further 
described in the Z-Wave Controller Development Kit Limited License Agreement, 
Sigma has no obligation to establish and maintain a data privacy and 
information security program with regard to Company’s use of any Third Party 
Service Environment or Cloud Service Environment. For the avoidance of doubt, 
Sigma shall not be responsible for physical, technical, security, 
administrative, and/or organizational safeguards that are designed to ensure 
the security and confidentiality of the Company Content or Company Application 
in any Third Party Service Environment or Cloud Service Environment that 
Company chooses to utilize.
*/
/**
 * @file    zwp_avi_ifttt_db_common.c
 *
 * @brief   Common header file for IFTTT database utility APIs
 * @details Common header file for IFTTT database utility APIs
 *
 * @author  Angela
 *
 * @version 1.0 - 2017-01-13
 * - Initial version
 */

#include "zwp_avi_ifttt_db_common.h"
#include "zwp_network_define.h"
#include "zwp_descriptor.h"
#include "zwp_status.h"
#include "zwp_log.h"
#include "zip_api.h"
#include "zwp_ifttt.h"
#include "zwp_avi_interfaces_alarm.h"

/**IFTTT Generic trigger database table head */
/**This much match ifttt_generic_trigger_table_index  */
static zwp_db_table_head	arIFTTT_generic_trigger_table_head[] =
{
	//pcColumnName					//eColumnType						//isize					//iProperty
	{ TABLE_COL_NAME_DEVICE,		ZWP_DB_DATATYPE_BIGINT_UNSIGNED,	sizeof(int64_t),		ZWP_DB_PROPERTY_PRIMARY_KEY },
	{ TABLE_COL_NAME_CC,			ZWP_DB_DATATYPE_TEXT,				100,					0 },
	{ TABLE_COL_NAME_STATE,			ZWP_DB_DATATYPE_TEXT,				MAX_STATE_DATA_LENGTH,	ZWP_DB_PROPERTY_PRIMARY_KEY },
	{ TABLE_COL_NAME_PARAM_SIZE,	ZWP_DB_DATATYPE_INT,				sizeof(int),			ZWP_DB_PROPERTY_PRIMARY_KEY },
	{ TABLE_COL_NAME_PARAM,			ZWP_DB_DATATYPE_BLOB,				MAX_PARAM_LENGTH,		ZWP_DB_PROPERTY_PRIMARY_KEY },
	{ TABLE_COL_NAME_TRIGGERID,		ZWP_DB_DATATYPE_TEXT,				MAX_TRIGGER_ID_LENGTH,	0 }
};

//static zwp_db_table_value arIFTTT_Alarm_generic_table_init[] =
//{
//	//pcColumnName					//eType								//isize				//data
//	{ TABLE_COL_NAME_DEVICE,		ZWP_DB_DATATYPE_BIGINT_UNSIGNED,	sizeof(int64_t),	{ 0 } },
//	{ TABLE_COL_NAME_CC,			ZWP_DB_DATATYPE_TEXT,				100,				{ IFTTT_DB_TABLE_NAME_ALARM } },
//	{ TABLE_COL_NAME_STATE,			ZWP_DB_DATATYPE_TEXT,				MAX_STATE_DATA_LENGTH,{ "" } },
//	{ TABLE_COL_NAME_PARAM_SIZE,	ZWP_DB_DATATYPE_INT,				sizeof(int),		{ 0 } },
//	{ TABLE_COL_NAME_PARAM,			ZWP_DB_DATATYPE_BLOB,				MAX_PARAM_LENGTH,				{ 0 } },
//	{ TABLE_COL_NAME_TRIGGERID,		ZWP_DB_DATATYPE_TEXT,				MAX_TRIGGER_ID_LENGTH,{ "" } }
//};

/**IFTTT Alarm database table head */
/**This much match _ifttt_alarm_table_index  */
static zwp_db_table_head	arIFTTT_alarm_table_head[] =
{
	//pcColumnName					//eColumnType						//isize					//iProperty
	{ TABLE_COL_NAME_DEVICE,		ZWP_DB_DATATYPE_BIGINT_UNSIGNED,	sizeof(int64_t),		ZWP_DB_PROPERTY_PRIMARY_KEY },
	{ TABLE_COL_NAME_ZTYPE,			ZWP_DB_DATATYPE_INT,				sizeof(int),			ZWP_DB_PROPERTY_PRIMARY_KEY },
	{ TABLE_COL_NAME_EVENT,			ZWP_DB_DATATYPE_INT,				sizeof(int),			ZWP_DB_PROPERTY_PRIMARY_KEY },
	{ TABLE_COL_NAME_PARAM_SIZE,	ZWP_DB_DATATYPE_INT,				sizeof(int),			ZWP_DB_PROPERTY_PRIMARY_KEY },
	{ TABLE_COL_NAME_PARAM,			ZWP_DB_DATATYPE_BLOB,				MAX_PARAM_LENGTH,		ZWP_DB_PROPERTY_PRIMARY_KEY },
	{ TABLE_COL_NAME_TIMESTAMP,		ZWP_DB_DATATYPE_BIGINT_UNSIGNED,	sizeof(int64_t),		ZWP_DB_PROPERTY_PRIMARY_KEY },
	{ TABLE_COL_NAME_TRIGGERID,		ZWP_DB_DATATYPE_TEXT,				MAX_TRIGGER_ID_LENGTH,	0 }
};

/**IFTTT MSensor database table head */
/**This much match _ifttt_msensor_table_index  */
static zwp_db_table_head	arIFTTT_msensor_table_head[] =
{
	//pcColumnName					//eColumnType						//isize					//iProperty
	{ TABLE_COL_NAME_DEVICE,		ZWP_DB_DATATYPE_BIGINT_UNSIGNED,	sizeof(int64_t),		ZWP_DB_PROPERTY_PRIMARY_KEY },
	{ TABLE_COL_NAME_TYPE,			ZWP_DB_DATATYPE_INT,				sizeof(int),			ZWP_DB_PROPERTY_PRIMARY_KEY },
	{ TABLE_COL_NAME_VALUE,			ZWP_DB_DATATYPE_TEXT,				50,						ZWP_DB_PROPERTY_PRIMARY_KEY },
	{ TABLE_COL_NAME_UNIT,			ZWP_DB_DATATYPE_INT,				sizeof(int),			ZWP_DB_PROPERTY_PRIMARY_KEY },
	{ TABLE_COL_NAME_TIMESTAMP,		ZWP_DB_DATATYPE_BIGINT_UNSIGNED,	sizeof(int64_t),		ZWP_DB_PROPERTY_PRIMARY_KEY },
	{ TABLE_COL_NAME_TRIGGERID,		ZWP_DB_DATATYPE_TEXT,				MAX_TRIGGER_ID_LENGTH,	ZWP_DB_PROPERTY_PRIMARY_KEY }
	//Trigger ID is also part of the primary key because the same report may fulfill 1 or more trigger conditions
};

/**
* @brief Get the DB table pointer for IFTTT generic trigger table
* @param[in]     desc_interface    Interface descriptor
* @return Pointer to IFTTT generic trigger table
*/
pzwp_db_table zwp_avi_ifttt_generic_trigger_table_get(zwifd_p desc_interface)
{
	pzwp_db_table pTableGeneric = NULL;
	pzwp_db_db pDB = NULL;
	zwp_net_t	net = NULL;
	int col_count = 0;

	if ((net = zwp_desc_interface_get_net(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Fail to get net pointer from interface descriptor %p", desc_interface);
		goto l_err_get_net;
	}

	pTableGeneric = zwp_net_get_ifttt_table_trigger_generic(net);

	if (pTableGeneric == NULL)
	{
		//Create generic Table
		pDB = zwp_net_get_ifttt_db(net);

		if(!pDB)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Fail to get ifttt DB in network object %p", net);
			goto l_err_get_db;
		}

		if (zwp_db_open_table(pDB, IFTTT_DB_TABLE_NAME_GENERIC_TRIGGER, &pTableGeneric) != ZWP_STATUS_OK)
		{
			//Generic table not exist, create it
			col_count = sizeof(arIFTTT_generic_trigger_table_head) / sizeof(zwp_db_table_head);

			if (zwp_db_create_table(pDB, IFTTT_DB_TABLE_NAME_GENERIC_TRIGGER, 
									col_count, arIFTTT_generic_trigger_table_head, 
									&pTableGeneric) != ZWP_STATUS_OK)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Fail to create generic trigger table for net %p", net);
				goto l_err_create_generic_table;
			}
		}

		zwp_net_set_ifttt_table_trigger_generic(net, pTableGeneric);
	}

l_err_create_generic_table:
l_err_get_db:
l_err_get_net:
	return pTableGeneric;
}

/**
* @brief Check whether the specific trigger info already exists in IFTTT generic trigger table
* @param[in]     pgeneric_table    Table pointer to generic table
* @param[in]     desc_interface    Interface descriptor
* @param[in]     stQueryCond		Query condition for generic table
* @param[in]     command_class    command class ID
* @param[out]	 sTriggerID  Trigger ID found
* @return ZWP_STATUS_xx  ZWP_STATUS_ENOENT means not found
*/
zwp_status_t zwp_avi_ifttt_generic_table_report_exist(pzwp_db_table pgeneric_table,
	zwifd_p desc_interface,
	ifttt_generic_table_device_data stQueryCond,
	//uint16_t command_class,
	char *sTriggerID)
{
	zwp_status_t res = ZWP_STATUS_ERROR;
	int iNofCol = 0, iResNofCol = 0, iResRecord = 0;
	int i, imatch = 0, temp;
	pzwp_db_table_value pQueryCond = NULL;
	pzwp_db_table_value pResRecords = NULL;

	iNofCol = 2;

	if (zwp_ifttt_generate_query_record_generic_table_for_device_report(iNofCol, desc_interface, &stQueryCond, &pQueryCond))
	{		
		if (zwp_db_query_record(pgeneric_table, iNofCol, pQueryCond,
			-1, //Query all the records that matches device ID and state/data
			0, //No sorting condition
			NULL, //No sorting condition
			&iResNofCol,
			&iResRecord,
			&pResRecords)
			!= ZWP_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Query generic table fail. error: %s", zwp_db_get_table_errortext(pgeneric_table));
			goto l_err_get_query_record;
		}

		ZWP_LOG(ZWP_LOG_DEBUG, "Query generic record with device and state. %d matched record found.", iResRecord);

		if (iResRecord == 0)
		{
			//Means no match record found
			ZWP_LOG(ZWP_LOG_DEBUG, "No match record found. Return.");
			res = ZWP_STATUS_ENOENT;
		}
		else
		{
			//For every matched record, check param_size and param
			for (i = GENERIC_TABLE_INDEX_PARAM_SIZE; i < iResNofCol * iResRecord; i += iResNofCol)
			{
				if (!strcmp(pResRecords[i].pcColumnName, TABLE_COL_NAME_PARAM_SIZE))
				{
					if (pResRecords[i].data.valueint == -1)	//If param size ignore, always match
					{
						imatch = 1;
					}
					else if (pResRecords[i].data.valueint == stQueryCond.iParam_size) //If param size >= 0 always check with param
					{
						temp = GENERIC_TABLE_INDEX_PARAM - GENERIC_TABLE_INDEX_PARAM_SIZE;

						if (stQueryCond.iParam_size > 0)
						{
							ZWP_LOG(ZWP_LOG_DEBUG, "*pResRecords[i+temp].data.valuebyte = %d, \
								*stQueryCond.pParam = %d",
								*(pResRecords[i + temp].data.valuebyte),
								*(stQueryCond.pParam));
						}

						if(!memcmp(pResRecords[i+temp].data.valuebyte, stQueryCond.pParam, stQueryCond.iParam_size))
							imatch = 1;
					}

					if(imatch == 1)
					{
						//Match found
						ZWP_LOG(ZWP_LOG_DEBUG, "Matched!! pResRecords[i].pcColumnName = %s, pResRecords[i].data.valueint = %d, stQueryCond.iParam_size = %d",
							pResRecords[i].pcColumnName, pResRecords[i].data.valueint, stQueryCond.iParam_size);
						temp = GENERIC_TABLE_INDEX_TRIGGERID - GENERIC_TABLE_INDEX_PARAM_SIZE;

						memcpy(sTriggerID, pResRecords[i + temp].data.valuestring, pResRecords[i + temp].isize);

						ZWP_LOG(ZWP_LOG_INFO, "Matched record found in generic table with trigger ID %s", sTriggerID);
						res = ZWP_STATUS_OK;
						break;
					}
					else
					{
						ZWP_LOG(ZWP_LOG_DEBUG, "Not matched!! pResRecords[i].pcColumnName = %s, pResRecords[i].data.valueint = %d, stQueryCond.iParam_size = %d",
							pResRecords[i].pcColumnName, pResRecords[i].data.valueint, stQueryCond.iParam_size);

					}
				}
			}
		}

		zwp_db_query_record_free(iResNofCol, iResRecord, pResRecords);
		zwp_ifttt_free_table_value(iNofCol, pQueryCond);
	}
	else
	{
		goto l_err_get_generate_query_record;
	}

	return res;

	zwp_db_query_record_free(iResNofCol, iResRecord, pResRecords);
l_err_get_query_record:
	zwp_ifttt_free_table_value(iNofCol, pQueryCond);
l_err_get_generate_query_record:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Query IFTTT generic trigger table multilevel sensor records based on sensor type and unit
* @param[in]     pgeneric_table    Table pointer to generic table
* @param[in]     desc_interface    Interface descriptor
* @param[in]     type			   Type
* @param[in]     unit			   Unit
* @param[out]	 pResultList	   Array of matched result list. Array element zwp_avi_ifttt_sensor_multilevel_get_t*
* @return ZWP_STATUS_ERROR ZWP_STATUS_ENOENT ZWP_STATUS_OK
*/
zwp_status_t zwp_avi_ifttt_generic_table_report_msensor_query(pzwp_db_table pgeneric_table,
	zwifd_p desc_interface,
	int	type,
	int unit,
	zwu_array_list_t ResultList)
	//zwp_avi_ifttt_sensor_multilevel_get_t *pResultList)
{
	zwp_status_t res = ZWP_STATUS_ERROR;
	int iNofCol = 0, iResNofCol = 0, iResRecord = 0;
	int i, temp;
	pzwp_db_table_value pQueryCond = NULL;
	pzwp_db_table_value pResRecords = NULL;
	zwp_avi_ifttt_sensor_multilevel_get_t *pSingleResult = NULL;
	char sState[200] = { 0 };

	iNofCol = 1;

	if (zwp_ifttt_generate_query_record_generic_table_for_device_report(iNofCol, desc_interface, NULL, &pQueryCond) == 0)
	{
		goto l_err_get_generate_query_record;
	}

	if (zwp_db_query_record(pgeneric_table, iNofCol, pQueryCond,
							-1, //Query all the records that matches device ID and state/data
							0, //No sorting condition
							NULL, //No sorting condition
							&iResNofCol,
							&iResRecord,
							&pResRecords)
							!= ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Query generic table fail. error: %s", zwp_db_get_table_errortext(pgeneric_table));
		goto l_err_get_query_record;
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Query generic table for matched multilevel sensor device id. %d matched record found.", iResRecord);

	if (iResRecord == 0)
	{
		//Means no match record found
		ZWP_LOG(ZWP_LOG_DEBUG, "No match record found. Return.");
		res = ZWP_STATUS_ENOENT;
	}
	else
	{
		//For every matched record, parse the State string into zwp_avi_ifttt_sensor_multilevel_get_t structs
		for (i = GENERIC_TABLE_INDEX_STATE; i < iResNofCol * iResRecord; i += iResNofCol)
		{
			if (!strcmp(pResRecords[i].pcColumnName, TABLE_COL_NAME_STATE))
			{
				memset(sState, 0, 200);
				strcpy(sState, pResRecords[i].data.valuestring);

				//Parse state string
				if (!zwp_avi_ifttt_parse_msensor_state_string(sState, &pSingleResult))
				{
					ZWP_LOG(ZWP_LOG_DEBUG, "Fail to parse state string %s.", sState);
					goto l_err_parse_state_string;
				}

				//Check for type and unit
				if (pSingleResult->type != type)
				{
					ZWP_LOG(ZWP_LOG_DEBUG, "Different sensor type from generic table %d and new report %d. Look for next record.", 
						pSingleResult->type, type);
					ZWP_FREE(pSingleResult);
					
					continue;
				}

				if (type == ZW_SENSOR_TYPE_LUM || type == ZW_SENSOR_TYPE_HUMD)
				{
					if (pSingleResult->thld_unit != unit)
					{
						ZWP_LOG(ZWP_LOG_DEBUG, "Different sensor unit from generic table %d and new report %d. Look for next record.",
							pSingleResult->thld_unit, unit);
						ZWP_FREE(pSingleResult);

						continue;
					}
				}

				temp = GENERIC_TABLE_INDEX_TRIGGERID - GENERIC_TABLE_INDEX_STATE;

				ZWP_LOG(ZWP_LOG_DEBUG, "pResRecords[i + temp] column name = %s isize = %d", pResRecords[i + temp].pcColumnName, pResRecords[i + temp].isize);

				strncpy(pSingleResult->trigger_id_data.striggerID,
					pResRecords[i + temp].data.valuestring,
					min(MAX_TRIGGER_ID_LENGTH, pResRecords[i + temp].isize));

				if (zwu_array_list_add(ResultList, pSingleResult) != ZWP_STATUS_OK)
				{
					ZWP_FREE(pSingleResult);
					goto l_err_result_list_add;
				}
			}
		}
	}

	zwp_db_query_record_free(iResNofCol, iResRecord, pResRecords);
	zwp_ifttt_free_table_value(iNofCol, pQueryCond);

	return ZWP_STATUS_OK;

l_err_result_list_add:
l_err_parse_state_string:
	zwp_ifttt_free_msensor_get_list(ResultList);
	zwp_db_query_record_free(iResNofCol, iResRecord, pResRecords);
l_err_get_query_record:
	zwp_ifttt_free_table_value(iNofCol, pQueryCond);
l_err_get_generate_query_record:
	return res;
}

/**
* @brief Free multilevel sensor get struct array list
* @param[in]	  genericTableMSensorList  multilevel sensor get struct array list
* @note see zwp_avi_ifttt_generic_table_report_msensor_query
*/
void zwp_avi_interfaces_msensor_free_msensor_get_list(zwu_array_list_t genericTableMSensorList)
{
	zwp_avi_ifttt_sensor_multilevel_get_t *pSingleResult;
	int count, i;

	count = zwu_array_list_size(genericTableMSensorList);

	for (i = 0; i < count; i++)
	{
		pSingleResult = zwu_array_list_get(genericTableMSensorList, i);

		ZWP_FREE(pSingleResult);
	}
	zwu_array_list_remove_all(genericTableMSensorList);

	zwu_array_list_destroy(genericTableMSensorList);
}

/**
* @brief Generate query record for generic table based on device report
* @param[in]     desc_interface    Interface descriptor
* @param[in]     iNofQueryCond     Number of query conditions
* @param[in]     stQueryCond		Query condition from device report
* @param[out]    ppQueryCond		Query conditions generated
* @return 1 - success 0 - fail
* @note the query condition does not include trigger ID
* Caller of this function much call zwp_ifttt_free_table_value() free the resource
*/
int zwp_ifttt_generate_query_record_generic_table_for_device_report(
	int iNofQueryCond, 
	zwifd_p desc_interface,
	ifttt_generic_table_device_data *pstQueryCond,
	pzwp_db_table_value *ppQueryCond)
{
	int iRes = 0;
	pzwp_db_table_value pQueryCond = NULL;
	int64_t combo_id = 0;

	ZWS_CGI_GET_COMBO_ID_FROM_INTFD(combo_id, desc_interface);

	if ((pQueryCond = (pzwp_db_table_value)ZWP_CALLOC(iNofQueryCond, sizeof(zwp_db_table_value))) == NULL)
	{
		goto l_err_malloc_failure;
	}

	if (iNofQueryCond >= 1)
	{
		zwp_ifttt_generate_column_value_device(&pQueryCond[0], combo_id);
	}

	if ((iNofQueryCond >= 2) && pstQueryCond)
	{
		zwp_ifttt_generate_column_value_state(&pQueryCond[1], pstQueryCond->sState);
	}

	iRes = 1;

	*ppQueryCond = pQueryCond;

	return iRes;


	//ZWP_FREE(pQueryCond);
l_err_malloc_failure:
	return iRes;
}

/**
* @brief Generate query record for generic table based on command class arguments
* @param[in]     desc_interface    Interface descriptor
* @param[in]     device_data    device data
* @param[out]    piNofCol   Number of column of query conditions
* @param[out]    ppQueryCond   Query conditions generated
* @return ZWP_STATUS_ERROR - fail
*			ZWP_STATUS_OK - generate a record
* @note the query condition does not include trigger ID
*/
zwp_status_t zwp_ifttt_generate_query_record_generic_table(
	zwifd_p desc_interface,
	ifttt_generic_table_device_data device_data,
	int *piNofCol,
	pzwp_db_table_value *ppQueryCond)
{
	int iNofCol = 0;
	pzwp_db_table_value pQueryCond = NULL;
	int64_t combo_id = 0;

	ZWS_CGI_GET_COMBO_ID_FROM_INTFD(combo_id, desc_interface);

	iNofCol = 4;

	if ((pQueryCond = (pzwp_db_table_value)ZWP_CALLOC(iNofCol, sizeof(zwp_db_table_value))) == NULL)
	{
		goto l_err_malloc_failure;
	}

	zwp_ifttt_generate_column_value_device(&pQueryCond[0], combo_id);
	zwp_ifttt_generate_column_value_state(&pQueryCond[1], device_data.sState);
	zwp_ifttt_generate_column_value_param_size_param(&pQueryCond[2], &pQueryCond[3],
		device_data.iParam_size, device_data.pParam);

	*piNofCol = iNofCol;
	*ppQueryCond = pQueryCond;

	return ZWP_STATUS_OK;


	//ZWP_FREE(pQueryCond);
l_err_malloc_failure:
	return ZWP_STATUS_ERROR;
}


/**
* @brief Free the table value resources
* @param[in]     iNofCol    Number of columns in table_value struct
* @param[in]     pTableValueList    A list of tableValues
*/
void zwp_ifttt_free_table_value(int iNofCol, pzwp_db_table_value pTableValueList)
{
	int i;

	if (pTableValueList)
	{
		for (i = 0; i < iNofCol; i++)
		{
			if ((pTableValueList[i].eType == ZWP_DB_DATATYPE_BLOB) &&
				(pTableValueList[i].isize != 0))
				ZWP_FREE(pTableValueList[i].data.valuebyte);
			if (pTableValueList[i].eType == ZWP_DB_DATATYPE_TEXT)
				ZWP_FREE(pTableValueList[i].data.valuestring);
		}

		ZWP_FREE(pTableValueList);
	}
}

/**
* @brief Free the multilevel sensor get list array
* @param[in]     msensor_get_list   Array of the multilevel sensor get list
*/
void zwp_ifttt_free_msensor_get_list(zwu_array_list_t msensor_get_list)
{
	int count, i;
	zwp_avi_ifttt_sensor_multilevel_get_t *pSingle;

	count = zwu_array_list_size(msensor_get_list);

	for (i = 0; i < count; i++)
	{
		pSingle = zwu_array_list_get(msensor_get_list, i);

		ZWP_FREE(pSingle);
	}

	zwu_array_list_destroy(msensor_get_list);
}

/**
* @brief Generate DB table column value for column "Device"
* @param[in]     pTableValue    A table value pointer
* @param[in]     combo_id    device id/combo ID for the column
*/
void zwp_ifttt_generate_column_value_device(pzwp_db_table_value pTableValue, int64_t combo_id)
{

	pTableValue->pcColumnName = TABLE_COL_NAME_DEVICE;
	pTableValue->eType = ZWP_DB_DATATYPE_BIGINT_UNSIGNED;
	pTableValue->isize = sizeof(int64_t);
	pTableValue->data.valueint64 = combo_id;
}

/**
* @brief Generate DB table column value for column "Command_class"
* @param[in]     pTableValue    A table value pointer
* @param[in]     sCC_name		Command class table name
*/
int zwp_ifttt_generate_column_value_command_class(pzwp_db_table_value pTableValue, char* sCC_name)
{
	pTableValue->pcColumnName = TABLE_COL_NAME_CC;
	pTableValue->eType = ZWP_DB_DATATYPE_TEXT;
	pTableValue->isize = strlen(sCC_name);
	if ((pTableValue->data.valuestring = ZWP_STRDUP(sCC_name)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		return 0;
	}

	return 1;
}

/**
* @brief Generate DB table column value for column "State/Data"
* @param[in]     pTableValue    A table value pointer
* @param[in]     sState    State string
*/
int zwp_ifttt_generate_column_value_state(pzwp_db_table_value pTableValue, char* sState)
{
	pTableValue->pcColumnName = TABLE_COL_NAME_STATE;
	pTableValue->eType = ZWP_DB_DATATYPE_TEXT;
	pTableValue->isize = strlen(sState);
	if ((pTableValue->data.valuestring = ZWP_STRDUP(sState)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		return 0;
	}

	return 1;
}

/**
* @brief Generate DB table column value for column "timestamp"
* @param[in]     pTableValue    A table value pointer
* @param[in]     utime    timestamp
*/
void zwp_ifttt_generate_column_value_timestamp(pzwp_db_table_value pTableValue, zwu_time_t utime)
{
	pTableValue->pcColumnName = TABLE_COL_NAME_TIMESTAMP;
	pTableValue->eType = ZWP_DB_DATATYPE_BIGINT_UNSIGNED;
	pTableValue->isize = sizeof(int64_t);
	pTableValue->data.valueint64 = utime;
}

/**
* @brief Generate DB table column value for column type 'int'
* @param[in]     pTableValue    A table value pointer
* @param[in]     sColumnName    Column name
* @param[in]     iValue    integer value
*/
void zwp_ifttt_generate_column_value_type_int(pzwp_db_table_value pTableValue, char* sColumnName, int iValue)
{
	pTableValue->pcColumnName = sColumnName;
	pTableValue->eType = ZWP_DB_DATATYPE_INT;
	pTableValue->isize = sizeof(int);
	pTableValue->data.valueint = iValue;
}

/**
* @brief Generate DB table column value for column type 'TEXT'
* @param[in]     pTableValue    A table value pointer
* @param[in]     sColumnName    Column name
* @param[in]     sValue			string value
*/
int zwp_ifttt_generate_column_value_type_string(pzwp_db_table_value pTableValue, char* sColumnName, char *sValue)
{
	pTableValue->pcColumnName = sColumnName;
	pTableValue->eType = ZWP_DB_DATATYPE_TEXT;
	pTableValue->isize = strlen(sValue);
	if ((pTableValue->data.valuestring = ZWP_STRDUP(sValue)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		return 0;
	}

	return 1;
}



/**
* @brief Generate DB table column value for column "param_size" and "param"
* @param[in]     pTableValue    A table value pointer for "param_size"
* @param[in]     pTableValue    A table value pointer for "param"
* @param[in]     iParam_size    Param size
* @param[in]     pParam			Param
*/
int zwp_ifttt_generate_column_value_param_size_param(pzwp_db_table_value pTableValueParamSize,
	pzwp_db_table_value pTableValueParam,
	int iParam_size,
	uint8_t *pParam)
{
	pTableValueParamSize->pcColumnName = TABLE_COL_NAME_PARAM_SIZE;
	pTableValueParamSize->eType = ZWP_DB_DATATYPE_INT;
	pTableValueParamSize->isize = sizeof(int);
	pTableValueParamSize->data.valueint = iParam_size;

	pTableValueParam->pcColumnName = TABLE_COL_NAME_PARAM;
	pTableValueParam->eType = ZWP_DB_DATATYPE_BLOB;

	if ((iParam_size == 0) || (iParam_size == -1))
	{
		pTableValueParam->isize = 0;
		pTableValueParam->data.valuebyte = NULL;
	}
	else
	{
		pTableValueParam->isize = iParam_size;
		if ((pTableValueParam->data.valuebyte = ZWP_MALLOC(iParam_size + 1)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, "MALLOC failed.");
			return 0;
		}

		memcpy(pTableValueParam->data.valuebyte, pParam, iParam_size);
	}

	return 1;
}

/**
* @brief Generate DB table column value for column "TriggerID"
* @param[in]     pTableValue    A table value pointer
* @param[in]     triggerID    Trigger ID string
*/
int zwp_ifttt_generate_column_value_triggerid(pzwp_db_table_value pTableValue, char* triggerID)
{
	pTableValue->pcColumnName = TABLE_COL_NAME_TRIGGERID;
	pTableValue->eType = ZWP_DB_DATATYPE_TEXT;
	pTableValue->isize = strlen(triggerID);
	if ((pTableValue->data.valuestring = ZWP_STRDUP(triggerID)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		return 0;
	}

	return 1;
}

/**
* @brief Insert command class data and trigger info into IFTTT generic trigger table
* @param[in]     pgeneric_table    Table pointer to generic table
* @param[in]     triggerIDdata     Trigger ID data
* @param[in]     desc_interface    Interface descriptor
* @param[in]     device_data	   IFTTT Trigger device data argument
* @param[in]     pcCommand_class_name    command class field
* @returnzwp_status_xx
*/
zwp_status_t zwp_avi_ifttt_generic_table_insert(pzwp_db_table pgeneric_table,
	zwp_avi_ifttt_trigger_get_t triggerIDdata,
	zwifd_p desc_interface,
	ifttt_generic_table_device_data device_data,
	char* pcCommand_class_name)
{
	int iNofCol = 0;
	pzwp_db_table_value pTableRecord = NULL;


	if (zwp_ifttt_generate_record_generic_table(desc_interface, triggerIDdata, device_data, pcCommand_class_name, &iNofCol, &pTableRecord) != ZWP_STATUS_OK)

	{
		ZWP_LOG(ZWP_LOG_ERR, "Fail to generate record for %s to insert to generic table.", pcCommand_class_name);
		goto l_err_generate_record;
	}

	if (zwp_db_insert_record(pgeneric_table, iNofCol, pTableRecord) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Fail to insert record to generic table. Error: %s", zwp_db_get_table_errortext(pgeneric_table));
		goto l_err_insert_record;
	}

	zwp_ifttt_free_table_value(iNofCol, pTableRecord);
	

	return ZWP_STATUS_OK;

l_err_insert_record:
	zwp_ifttt_free_table_value(iNofCol, pTableRecord);
l_err_generate_record:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Generate record (to insert) for generic table based on command class arguments
* @param[in]     desc_interface    Interface descriptor
* @param[in]     triggerIDdata      IFTTT Trigger ID data
* @param[in]     device_data       IFTTT Trigger device data arguments
* @param[in]     pcCommand_class_name    command class field
* @param[out]    piNofCol   Number of column of query conditions
* @param[out]    ppTableRecord   Query conditions generated
* @return ZWP_STATUS_ERROR - fail
*			ZWP_STATUS_OK - generate a record
* Caller of this function much call zwp_ifttt_free_table_value() free the resource
*/
zwp_status_t zwp_ifttt_generate_record_generic_table(
	zwifd_p desc_interface,
	zwp_avi_ifttt_trigger_get_t triggerIDdata,
	ifttt_generic_table_device_data device_data,
	char* pcCommand_class_name,
	int *piNofCol,
	pzwp_db_table_value *ppTableRecord)
{
	int iNofCol = 0;
	pzwp_db_table_value pTableRecord = NULL;
	int64_t combo_id = 0;

	ZWS_CGI_GET_COMBO_ID_FROM_INTFD(combo_id, desc_interface);

	iNofCol = sizeof(arIFTTT_generic_trigger_table_head) / sizeof(zwp_db_table_head);

	if ((pTableRecord = (pzwp_db_table_value)ZWP_CALLOC(iNofCol, sizeof(zwp_db_table_value))) == NULL)
	{
		goto l_err_malloc_failure;
	}

	zwp_ifttt_generate_column_value_device(&pTableRecord[GENERIC_TABLE_INDEX_DEVICE], combo_id);

	zwp_ifttt_generate_column_value_command_class(&pTableRecord[GENERIC_TABLE_INDEX_CC], pcCommand_class_name);

	zwp_ifttt_generate_column_value_state(&pTableRecord[GENERIC_TABLE_INDEX_STATE], device_data.sState);

	zwp_ifttt_generate_column_value_param_size_param(&pTableRecord[GENERIC_TABLE_INDEX_PARAM_SIZE], &pTableRecord[GENERIC_TABLE_INDEX_PARAM],
		device_data.iParam_size, device_data.pParam);

	zwp_ifttt_generate_column_value_triggerid(&pTableRecord[GENERIC_TABLE_INDEX_TRIGGERID], triggerIDdata.striggerID);
	

	*piNofCol = iNofCol;
	*ppTableRecord = pTableRecord;

	return ZWP_STATUS_OK;

	ZWP_FREE(pTableRecord);
l_err_malloc_failure:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Get the DB table pointer for IFTTT cc specific trigger table
* @param[in]     desc_interface    Interface descriptor
* @param[in]     command_class	   Command class
* @return Pointer to IFTTT cc specific table
*/
pzwp_db_table zwp_avi_ifttt_cc_table_get(zwifd_p desc_interface, uint16_t command_class)
{
	pzwp_db_table pTableCC = NULL;
	pzwp_db_db pDB = NULL;
	zwp_net_t	net = NULL;
	int col_count = 0;

	//Open CC specfic table
	//If not exists, create the table
	if ((net = zwp_desc_interface_get_net(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Fail to get net pointer from interface descriptor %p", desc_interface);
		goto l_err_get_net;
	}
	
	if((pDB = zwp_net_get_ifttt_db(net)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Fail to get ifttt DB in network object %p", net);
		goto l_err_get_db;
	}

	if (command_class == COMMAND_CLASS_ALARM)
	{
		if (zwp_db_open_table(pDB, IFTTT_DB_TABLE_NAME_ALARM, &pTableCC) != ZWP_STATUS_OK)
		{
			//CC table not exist, create it
			col_count = sizeof(arIFTTT_alarm_table_head) / sizeof(zwp_db_table_head);

			if (zwp_db_create_table(pDB, IFTTT_DB_TABLE_NAME_ALARM,
				col_count, arIFTTT_alarm_table_head,
				&pTableCC) != ZWP_STATUS_OK)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Fail to create alarm trigger table for net %p", net);
				goto l_err_create_cc_table;
			}
		}	
	}
	else if (command_class == COMMAND_CLASS_SENSOR_MULTILEVEL)
	{
		if (zwp_db_open_table(pDB, IFTTT_DB_TABLE_NAME_MSENSOR, &pTableCC) != ZWP_STATUS_OK)
		{
			//CC table not exist, create it
			col_count = sizeof(arIFTTT_msensor_table_head) / sizeof(zwp_db_table_head);

			if (zwp_db_create_table(pDB, IFTTT_DB_TABLE_NAME_MSENSOR,
				col_count, arIFTTT_msensor_table_head,
				&pTableCC) != ZWP_STATUS_OK)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Fail to create msensor trigger table for net %p", net);
				goto l_err_create_cc_table;
			}
		}
	}

l_err_create_cc_table:
l_err_get_db:
l_err_get_net:
	return pTableCC;
}

/**
* @brief Update the specific trigger info with the latest trigger ID in IFTTT generic trigger table
* @param[in]     pgeneric_table    Table pointer to generic table
* @param[in]     triggerIDdata     Trigger ID data
* @param[in]     desc_interface    Interface descriptor
* @param[in]     device_data	   Device data for the specific CC
* @param[out]	 piRecordUpdated  Number of matched records being updated
* @returnzwp_status_xx
*/
zwp_status_t zwp_avi_ifttt_generic_table_update_trigger_id(pzwp_db_table pgeneric_table,
	zwp_avi_ifttt_trigger_get_t triggerIDdata,
	zwifd_p desc_interface,
	ifttt_generic_table_device_data device_data,
	int *piRecordUpdated)
{
	int iNofCol = 0, iNofRecordsUpdated = 0;
	pzwp_db_table_value pQueryCond = NULL;
	zwp_db_table_value stTableValueToUpdate = { 0 };
	zwp_status_t dbRes;

	if (zwp_ifttt_generate_query_record_generic_table(desc_interface, device_data, &iNofCol, &pQueryCond) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Generate generic table query records fail.");
		goto l_err_get_query_record;
	}

	zwp_ifttt_generate_column_value_triggerid(&stTableValueToUpdate, triggerIDdata.striggerID);

	dbRes = zwp_db_update_record(pgeneric_table, iNofCol, pQueryCond, 1, &stTableValueToUpdate, &iNofRecordsUpdated);
	if (dbRes != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Fail to update record to generic table. dbRes = %d Error: %s", dbRes, zwp_db_get_table_errortext(pgeneric_table));
		ZWP_LOG(ZWP_LOG_ERR, "pgeneric_table = %p, iNofCol = %d, pQueryCond = %p, iNofRecordsUpdated = %d",
			pgeneric_table, iNofCol, pQueryCond, iNofRecordsUpdated);
		goto l_err_update_record;
	}

	ZWP_FREE(stTableValueToUpdate.data.valuestring);

	zwp_ifttt_free_table_value(iNofCol, pQueryCond);

	*piRecordUpdated = iNofRecordsUpdated;

	return ZWP_STATUS_OK;

l_err_update_record:
	zwp_ifttt_free_table_value(iNofCol, pQueryCond);
l_err_get_query_record:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Query records from cc table based on trigger ID and sort by timestamp
* @param[in]     pcc_table		Table pointer to cc table
* @param[in]     triggerIDdata     Trigger ID data
* @param[out]     iNumRecordFound    Number of record
* @param[out]     iNumColPerRecord   Number of Columns per record
* @param[out]     pptable_data    Matched records
* @returnzwp_status_xx
*/
zwp_status_t zwp_avi_ifttt_query_trigger_id_from_cc_table(
	pzwp_db_table pcc_table, 
	zwp_avi_ifttt_trigger_get_t triggerIDdata, 
	int *piNumRecordFound, 
	int *piNumColPerRecord,
	zwp_db_table_value **pptable_data)
{
	zwp_status_t res;
	zwp_db_table_value QueryCond = { 0 };
	zwp_db_sort_cond SortCond = { 0 };

	zwp_ifttt_generate_column_value_triggerid(&QueryCond, triggerIDdata.striggerID);

	SortCond.eSortType = ZWP_DB_SORTING_DESCENDING_ORDER;
	SortCond.pcColumnName = TABLE_COL_NAME_TIMESTAMP;

	res = zwp_db_query_record(pcc_table,
		1, //Number of query condition
		&QueryCond,
		triggerIDdata.iLimit,
		1, //Number of Sorting condition
		&SortCond,
		piNumColPerRecord,
		piNumRecordFound,
		pptable_data
	);

	ZWP_FREE(QueryCond.data.valuestring);

	return res;
}

zwp_status_t zwp_avi_ifttt_realtime_notification_add(zwifd_p desc_interface, char *sTriggerID)
{
	zwp_net_t net;
	zwp_ifttt_t ifttt;

	if ((net = zwp_desc_interface_get_net(desc_interface)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Fail to get net pointer from interface descriptor %p", desc_interface);
		goto l_err_get_net;
	}

	ifttt = zwp_portal_get_ifttt(zwp_net_get_portal(net));

	if (zwp_ifttt_realtime_notification_trigger_add(ifttt, sTriggerID) != ZWP_STATUS_OK)
	{
		goto l_err_ifttt_notification_trigger_add;
	}

	return ZWP_STATUS_OK;

l_err_ifttt_notification_trigger_add:
l_err_get_net:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Insert command class data and trigger info into IFTTT cc table
* @param[in]     pcc_table    Table pointer to CC table
* @param[in]     desc_interface    Interface descriptor
* @param[in]     args    IFTTT Trigger arguments
* @param[in]     command_class    command class ID
* @param[in]     utime    timestamp
* @param[in]     sTriggerID     Trigger ID
* @returnzwp_status_xx
*/
zwp_status_t zwp_avi_ifttt_cc_table_insert(pzwp_db_table pcc_table,
	zwifd_p desc_interface,
	void *args,
	uint16_t command_class,
	zwu_time_t utime,
	char *sTriggerID)
{
	int iNofCol = 0;
	pzwp_db_table_value pTableRecord = NULL;

	if (strlen(sTriggerID) <= 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "No valid Trigger ID. Fail to insert.");
		goto l_err_trigger_id;
	}

	if (zwp_ifttt_generate_record_cc_table(desc_interface, args, command_class, utime, sTriggerID, &iNofCol, &pTableRecord) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Fail to generate record to insert to cc table. command_class", command_class);
		goto l_err_generate_record;
	}

	if (zwp_db_insert_record(pcc_table, iNofCol, pTableRecord) != ZWP_STATUS_OK)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Fail to insert record to cc table. Error: %s", zwp_db_get_table_errortext(pcc_table));
		ZWP_LOG(ZWP_LOG_DEBUG, "It is possible that this is a cached value and the same data has been recorded into DB previously. Ignore.");
		goto l_err_insert_record;
	}

	zwp_avi_ifttt_realtime_notification_add(desc_interface, sTriggerID);


l_err_insert_record:
	zwp_ifttt_free_table_value(iNofCol, pTableRecord);

	return ZWP_STATUS_OK;

	zwp_ifttt_free_table_value(iNofCol, pTableRecord);
l_err_generate_record:
l_err_trigger_id:
	return ZWP_STATUS_ERROR;
}

/**
* @brief Generate record (to insert) for cc table based on command class arguments
* @param[in]     desc_interface    Interface descriptor
* @param[in]     args    IFTTT Trigger arguments
* @param[in]     command_class    command class ID
* @param[in]     utime    timestamp
* @param[in]     sTriggerID     Trigger ID
* @param[out]    piNofCol   Number of column of query conditions
* @param[out]    ppTableRecord   Query conditions generated
* @return ZWP_STATUS_ERROR - fail
*			ZWP_STATUS_OK - generate a record
* Caller of this function much call zwp_ifttt_free_table_value() free the resource
* @note Only 1 record will be generated.
*/
zwp_status_t zwp_ifttt_generate_record_cc_table(
	zwifd_p desc_interface,
	void *args,
	uint16_t command_class,
	zwu_time_t utime,
	char *sTriggerID,
	int *piNofCol,
	pzwp_db_table_value *ppTableRecord)
{
	int iNofCol = 0;
	pzwp_db_table_value pTableRecord = NULL;
	int64_t combo_id = 0;

	ZWS_CGI_GET_COMBO_ID_FROM_INTFD(combo_id, desc_interface);


	if (command_class == COMMAND_CLASS_ALARM)
	{
		zwalrm_p argument = args;

		iNofCol = sizeof(arIFTTT_alarm_table_head) / sizeof(zwp_db_table_head);

		if ((pTableRecord = (pzwp_db_table_value)ZWP_CALLOC(iNofCol, sizeof(zwp_db_table_value))) == NULL)
		{
			goto l_err_malloc_failure;
		}

		//(pTableRecord, arIFTTT_Alarm_generic_table_init, sizeof(arIFTTT_Alarm_generic_table_init));

		zwp_ifttt_generate_column_value_device(&pTableRecord[ALARM_TABLE_INDEX_DEVICE], combo_id);
		zwp_ifttt_generate_column_value_type_int(&pTableRecord[ALARM_TABLE_INDEX_ZTYPE], TABLE_COL_NAME_ZTYPE, argument->ex_type);
		zwp_ifttt_generate_column_value_type_int(&pTableRecord[ALARM_TABLE_INDEX_EVENT], TABLE_COL_NAME_EVENT, argument->ex_event);

		zwp_ifttt_generate_column_value_param_size_param(
			&pTableRecord[ALARM_TABLE_INDEX_PARAM_SIZE], 
			&pTableRecord[ALARM_TABLE_INDEX_PARAM],
			argument->ex_evt_len, 
			(uint8_t *)&argument->ex_evt_prm[0]);

		zwp_ifttt_generate_column_value_timestamp(&pTableRecord[ALARM_TABLE_INDEX_TIMESTAMP], utime);

		zwp_ifttt_generate_column_value_triggerid(&pTableRecord[ALARM_TABLE_INDEX_TRIGGERID], sTriggerID);
	}
	else if (command_class == COMMAND_CLASS_SENSOR_MULTILEVEL)
	{
		zwsensor_p argument = args;
		char sValue[80] = { 0 };
		int32_t  snsr_value = 0;

		iNofCol = sizeof(arIFTTT_msensor_table_head) / sizeof(zwp_db_table_head);

		if ((pTableRecord = (pzwp_db_table_value)ZWP_CALLOC(iNofCol, sizeof(zwp_db_table_value))) == NULL)
		{
			goto l_err_malloc_failure;
		}

		zwp_ifttt_generate_column_value_device(&pTableRecord[MSENSOR_TABLE_INDEX_DEVICE], combo_id);
		zwp_ifttt_generate_column_value_type_int(&pTableRecord[MSENSOR_TABLE_INDEX_TYPE], TABLE_COL_NAME_TYPE, argument->type);

		if (!zwu_byte_stream_to_int(argument->data, argument->size, &snsr_value))
		{   
			ZWP_LOG(ZWP_LOG_ERR, "Fail to convert sensor value to integer, argument->size = %d", argument->size);
			goto l_err_int_convert;
		}

		zwu_integer_precision_to_string(snsr_value, argument->precision, 80, sValue);

		zwp_ifttt_generate_column_value_type_string(&pTableRecord[MSENSOR_TABLE_INDEX_VALUE], TABLE_COL_NAME_VALUE, sValue);

		zwp_ifttt_generate_column_value_type_int(&pTableRecord[MSENSOR_TABLE_INDEX_UNIT], TABLE_COL_NAME_UNIT, argument->unit);

		zwp_ifttt_generate_column_value_timestamp(&pTableRecord[MSENSOR_TABLE_INDEX_TIMESTAMP], utime);

		zwp_ifttt_generate_column_value_triggerid(&pTableRecord[MSENSOR_TABLE_INDEX_TRIGGERID], sTriggerID);
		
	}

	*piNofCol = iNofCol;
	*ppTableRecord = pTableRecord;

	return ZWP_STATUS_OK;

l_err_int_convert:
	ZWP_FREE(pTableRecord);
l_err_malloc_failure:
	return ZWP_STATUS_ERROR;


}

/**
* @brief Parse multilevel sensor state string to a IFTTT multilevel sensor get struct
* @param[in]     sState			State string
* @param[out]    ppSingleResult   Pointer to a IFTTT multilevel sensor get struct
* @return 1 - success 0 - fail
* @note
* Caller of this function much free the multilevel sensor get pointer that returned by this function
*/
int zwp_avi_ifttt_parse_msensor_state_string(char *sState, zwp_avi_ifttt_sensor_multilevel_get_t **ppSingleResult)
{
	char *entry, *next_entry;
	char *key, *value;
	zwp_avi_ifttt_sensor_multilevel_get_t *pSingleResult;

	entry = sState;

	if ((pSingleResult = (zwp_avi_ifttt_sensor_multilevel_get_t *)ZWP_CALLOC(1, sizeof(zwp_avi_ifttt_sensor_multilevel_get_t))) == NULL)
	{
		goto l_err_malloc_failure;
	}

	while (entry && entry[0])
	{
		if ((next_entry = strstr(entry, "&")))
		{
			*next_entry = '\0';
			next_entry++;
		}

		key = entry;

		if (!(value = strstr(key, "=")))
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to find key-value delimiter in state string entry: %s", entry);
			goto l_err_key_value;
		}

		*value = '\0';
		value++;

		if (!key[0])
		{
			ZWP_LOG(ZWP_LOG_ERR, "Empty key with value: '%s'", value);
			goto l_err_empty_key;
		}

		if (!zwp_avi_ifttt_string_to_msensor_get_data(key, value, pSingleResult))
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to assign key: %s, value %s to msensor get data", key, value);
			goto l_err_key_value;
		}

		entry = next_entry;
	}

	*ppSingleResult = pSingleResult;

	return 1;

l_err_empty_key:
l_err_key_value:
	ZWP_FREE(pSingleResult);
l_err_malloc_failure:
	return 0;
}

/**
* @brief Assign IFTTT multilevel sensor get data based on key/value pair
* @param[in]     key			key
* @param[in]     value			value
* @param[in, out]    pSingleResult   Pointer to a IFTTT multilevel sensor get struct
* @return 1 - success 0 - fail
* @note
* Caller of this function much free the multilevel sensor get pointer that returned by this function
*/
int zwp_avi_ifttt_string_to_msensor_get_data(char *key, char* value, zwp_avi_ifttt_sensor_multilevel_get_t *pSingleResult)
{
	long lint;

	if (!strcmp(key, "type"))
	{
		if (zwu_parse_long(value, &lint) != ZWP_STATUS_OK)
		{
			goto l_err_parse_type;
		}

		pSingleResult->type = (int)lint;
	}
	else if (!strcmp(key, "comp_command"))
	{
		if (zwu_parse_long(value, &lint) != ZWP_STATUS_OK)
		{
			goto l_err_parse_comp_command;
		}

		pSingleResult->comp_command = (int)lint;
		
		if((pSingleResult->comp_command != ZW_SENSOR_COMPARISON_COMMAND_LT) &&
			(pSingleResult->comp_command != ZW_SENSOR_COMPARISON_COMMAND_GT))
			goto l_err_check_comp_command;
	}
	else if (!strcmp(key, "thld_value"))
	{
		strncpy(pSingleResult->cArrThld_value, value, min(50, strlen(value)));
	}
	else if (!strcmp(key, "thld_unit"))
	{
		if (zwu_parse_long(value, &lint) != ZWP_STATUS_OK)
		{
			goto l_err_parse_unit;
		}

		pSingleResult->thld_unit = (int)lint;
	}


	return 1;

l_err_parse_unit:
l_err_check_comp_command:
l_err_parse_comp_command:
l_err_parse_type:

	return 0;
}
