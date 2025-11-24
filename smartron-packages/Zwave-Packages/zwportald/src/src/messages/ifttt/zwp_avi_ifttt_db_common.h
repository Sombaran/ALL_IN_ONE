
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
 * @file    zwp_avi_ifttt_db_common.h
 *
 * @brief   Common header file for IFTTT database utility APIs
 * @details Common header file for IFTTT database utility APIs.
 *
 * @author  Angela
 *
 * @version 1.0 - 2017-01-13
 * - Initial version
 */
#ifndef ZWP_AVI_IFTTT_DB_COMMON_H_
#define ZWP_AVI_IFTTT_DB_COMMON_H_

#include <stdint.h>

#include "zwp_status.h"
#include "zip_api.h"
#include "zwp_db.h"
#include "zwp_avi_ifttt_common.h"
#include "zwp_avi_interfaces_sensor_multilevel.h"
//#include "zw_avro.h"
//#include "zwp_network_define.h"
//#include "zwu_array_list.h"
//#include "zwp_network.h"

/**IFTTT database generic trigger table name */
#define IFTTT_DB_TABLE_NAME_GENERIC_TRIGGER				"GenericTrigger"

/**IFTTT Generic trigger database table column name defines */
#define TABLE_COL_NAME_DEVICE				"Device"
#define TABLE_COL_NAME_CC					"Command_class"
#define TABLE_COL_NAME_STATE				"State_data"
#define TABLE_COL_NAME_PARAM_SIZE			"Param_size"
#define TABLE_COL_NAME_PARAM				"Param"
#define TABLE_COL_NAME_TRIGGERID			"TriggerID"

/**IFTTT CC database table column name defines */
#define TABLE_COL_NAME_TIMESTAMP			"Timestamp"

/**IFTTT Alarm database table column name defines */
#define TABLE_COL_NAME_ZTYPE				"Ztype"
#define TABLE_COL_NAME_EVENT				"Event"

/**IFTTT multilevel sensor database table column name defines */
#define TABLE_COL_NAME_TYPE					"Type"
#define TABLE_COL_NAME_VALUE				"Value"
#define TABLE_COL_NAME_UNIT					"Unit"

#define MAX_STATE_DATA_LENGTH				200
#define MAX_TRIGGER_ID_LENGTH				300
#define MAX_PARAM_LENGTH					250

/**IFTTT Generic trigger database table column index. This must match arIFTTT_Alarm_generic_table_default */
typedef enum _ifttt_generic_trigger_table_index
{
	GENERIC_TABLE_INDEX_DEVICE = 0,
	GENERIC_TABLE_INDEX_CC,
	GENERIC_TABLE_INDEX_STATE,
	GENERIC_TABLE_INDEX_PARAM_SIZE,
	GENERIC_TABLE_INDEX_PARAM,
	GENERIC_TABLE_INDEX_TRIGGERID

}ifttt_generic_trigger_table_index;


/**IFTTT Alarm database table column index. This must match arIFTTT_alarm_table_head */
typedef enum _ifttt_alarm_table_index
{
	ALARM_TABLE_INDEX_DEVICE = 0,
	ALARM_TABLE_INDEX_ZTYPE,
	ALARM_TABLE_INDEX_EVENT,
	ALARM_TABLE_INDEX_PARAM_SIZE,
	ALARM_TABLE_INDEX_PARAM,
	ALARM_TABLE_INDEX_TIMESTAMP,
	ALARM_TABLE_INDEX_TRIGGERID

}ifttt_alarm_table_index;

/**IFTTT MSensor database table column index. This must match arIFTTT_msensor_table_head */
typedef enum _ifttt_msensor_table_index
{
	MSENSOR_TABLE_INDEX_DEVICE = 0,
	MSENSOR_TABLE_INDEX_TYPE,
	MSENSOR_TABLE_INDEX_VALUE,
	MSENSOR_TABLE_INDEX_UNIT,
	MSENSOR_TABLE_INDEX_TIMESTAMP,
	MSENSOR_TABLE_INDEX_TRIGGERID

}ifttt_msensor_table_index;

#define ZWP_STR(s) #s
#define ZWP_XSTR(s) ZWP_STR(s)


#define IFTTT_DB_TABLE_NAME(name, command_class)		ZWP_STR(name)"_"ZWP_XSTR(command_class)

#define IFTTT_DB_TABLE_NAME_ALARM				IFTTT_DB_TABLE_NAME(alarm, COMMAND_CLASS_ALARM)		//name would be "alarm_113"
#define IFTTT_DB_TABLE_NAME_BSENSOR				IFTTT_DB_TABLE_NAME(bsensor, COMMAND_CLASS_SENSOR_BINARY)		//name would be "bsensor_48"
#define IFTTT_DB_TABLE_NAME_MSENSOR				IFTTT_DB_TABLE_NAME(msensor, COMMAND_CLASS_SENSOR_MULTILEVEL)		//name would be "msensor_49"


/** Combo ID macros, copy from zweb */

/** Node ID bitmask */
#define ZWS_CGI_NODE_ID_MASK             0x000000FF
/** Node ID shift */
#define ZWS_CGI_NODE_ID_SHIFT            0
/** Endpoint ID bitmask */
#define ZWS_CGI_ENDPOINT_ID_MASK         0x0000FF00
/** Endpoint ID shift */
#define ZWS_CGI_ENDPOINT_ID_SHIFT        8
/** Interface ID bitmask */
#define ZWS_CGI_INTERFACE_ID_MASK        0xFFFF0000
/** Interface ID shift */
#define ZWS_CGI_INTERFACE_ID_SHIFT       16

/** Combo ID not specified */
#define ZWS_CGI_NET_COMBO_ID_NONE        (-1L)
/** Node ID not specified */
#define ZWS_CGI_NODE_ID_NONE             (-1)
/** Endpoint ID not specified */
#define ZWS_CGI_ENDPOINT_ID_NONE         (-1)
/** Interface ID not specified */
#define ZWS_CGI_INTERFACE_ID_NONE        (-1)

/*
* @note Node descriptor id for node 3 is 3. Endpoint descriptor id for node 3 endpoint 0 is also 3.
* So to guarantee uniqueness of all descriptor ids endpoint id is incremented/decremented by 1.
*/

/** Get node ID from combo ID */
#define ZWS_CGI_GET_NODE_ID(desc)            (((desc) & ZWS_CGI_NODE_ID_MASK) >> ZWS_CGI_NODE_ID_SHIFT)
/** Get node ID component of combo ID */
#define ZWS_CGI_GET_NODE_DESC_ID(node_id)    (((node_id) << ZWS_CGI_NODE_ID_SHIFT) & ZWS_CGI_NODE_ID_MASK)

/** Get endpoint ID from combo ID */
#define ZWS_CGI_GET_ENDPOINT_ID(desc)        ((((desc) & ZWS_CGI_ENDPOINT_ID_MASK) >> ZWS_CGI_ENDPOINT_ID_SHIFT) - 1)
/** Get endpoint ID component of combo ID */
#define ZWS_CGI_GET_ENDPOINT_DESC_ID(combo_id, endpoint_id)  \
                                             (combo_id) | \
                                             (((endpoint_id + 1) << ZWS_CGI_ENDPOINT_ID_SHIFT) & ZWS_CGI_ENDPOINT_ID_MASK)

/** Get interface ID from combo ID */
#define ZWS_CGI_GET_INTERFACE_ID(desc)       (((desc) & ZWS_CGI_INTERFACE_ID_MASK) >> ZWS_CGI_INTERFACE_ID_SHIFT)
/** Get interface ID component of combo ID */
#define ZWS_CGI_GET_INTERFACE_DESC_ID(combo_id, interface_id)  \
                                             (combo_id) | \
                                             (((interface_id) << ZWS_CGI_INTERFACE_ID_SHIFT) & ZWS_CGI_INTERFACE_ID_MASK)

/** Generate combo ID from interface descriptor (zwifd_t *) */
#define ZWS_CGI_GET_COMBO_ID_FROM_INTFD(combo_id, desc_interface)						\
{																						\
	combo_id = 0;																		\
	combo_id = ZWS_CGI_GET_NODE_DESC_ID((uint8_t)desc_interface->nodeid);				\
	combo_id = ZWS_CGI_GET_ENDPOINT_DESC_ID(combo_id, (uint8_t)desc_interface->epid);	\
	combo_id = ZWS_CGI_GET_INTERFACE_DESC_ID(combo_id, (uint16_t)desc_interface->cls);	\
}


typedef struct _ifttt_generic_table_device_data
{
	char sState[MAX_STATE_DATA_LENGTH];
	int iParam_size;
	uint8_t  *pParam;
}ifttt_generic_table_device_data;



/**
* @brief Free the table value resources
* @param[in]     iNofCol    Number of columns in table_value struct
* @param[in]     pTableValueList    A list of tableValues
*/
void zwp_ifttt_free_table_value(int iNofCol, pzwp_db_table_value pTableValueList);

/**
* @brief Free the multilevel sensor get list array
* @param[in]     msensor_get_list   Array of the multilevel sensor get list
*/
void zwp_ifttt_free_msensor_get_list(zwu_array_list_t msensor_get_list );

/**
* @brief Generate DB table column value for column "Device"
* @param[in]     pTableValue    A table value pointer
* @param[in]     combo_id    device id/combo ID for the column
*/
void zwp_ifttt_generate_column_value_device(pzwp_db_table_value pTableValue, int64_t combo_id);

/**
* @brief Generate DB table column value for column "Command_class"
* @param[in]     pTableValue    A table value pointer
* @param[in]     sCC_name		Command class table name
*/
int zwp_ifttt_generate_column_value_command_class(pzwp_db_table_value pTableValue, char* sCC_name);


/**
* @brief Generate DB table column value for column "State/Data"
* @param[in]     pTableValue    A table value pointer
* @param[in]     sState    State string
*/
int zwp_ifttt_generate_column_value_state(pzwp_db_table_value pTableValue, char* sState);

/**
* @brief Generate DB table column value for column "timestamp"
* @param[in]     pTableValue    A table value pointer
* @param[in]     utime    timestamp
*/
void zwp_ifttt_generate_column_value_timestamp(pzwp_db_table_value pTableValue, zwu_time_t utime);


/**
* @brief Generate DB table column value for column type 'int'
* @param[in]     pTableValue    A table value pointer
* @param[in]     sColumnName    Column name
* @param[in]     iValue    integer value
*/
void zwp_ifttt_generate_column_value_type_int(pzwp_db_table_value pTableValue, char* sColumnName, int iValue);

/**
* @brief Generate DB table column value for column type 'TEXT'
* @param[in]     pTableValue    A table value pointer
* @param[in]     sColumnName    Column name
* @param[in]     sValue			string value
* @return 1 - success 0 - fail
*/
int zwp_ifttt_generate_column_value_type_string(pzwp_db_table_value pTableValue, char* sColumnName, char *sValue);

/**
* @brief Generate DB table column value for column "param_size" and "param"
* @param[in]     pTableValue    A table value pointer for "param_size"
* @param[in]     pTableValue    A table value pointer for "param"
* @param[in]     iParam_size    Param size
* @param[in]     pParam			Param
* @return 1- success 0 - fail
*/
int zwp_ifttt_generate_column_value_param_size_param(pzwp_db_table_value pTableValueParamSize,
	pzwp_db_table_value pTableValueParam,
	int iParam_size,
	uint8_t *pParam);

/**
* @brief Generate DB table column value for column "TriggerID"
* @param[in]     pTableValue    A table value pointer
* @param[in]     triggerID    Trigger ID string
*/
int zwp_ifttt_generate_column_value_triggerid(pzwp_db_table_value pTableValue, char* triggerID);

/**
* @brief Get the DB table pointer for IFTTT generic trigger table
* @param[in]     desc_interface    Interface descriptor
* @return Pointer to IFTTT generic trigger table
*/
pzwp_db_table zwp_avi_ifttt_generic_trigger_table_get(zwifd_p desc_interface);

/**
* @brief Check whether the specific trigger info already exists in IFTTT generic trigger table
* @param[in]     pgeneric_table    Table pointer to generic table
* @param[in]     desc_interface    Interface descriptor
* @param[in]     stQueryCond		Query condition from device report 
* @param[out]	 sTriggerID  Trigger ID found
* @return zwp_status_error means error or not found
*/
zwp_status_t zwp_avi_ifttt_generic_table_report_exist(pzwp_db_table pgeneric_table,
											 zwifd_p desc_interface, 
											 ifttt_generic_table_device_data stQueryCond,
											 char *sTriggerID);

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
									zwu_array_list_t ResultList);

/**
* @brief Free multilevel sensor get struct array list
* @param[in]	  genericTableMSensorList  multilevel sensor get struct array list
* @note see zwp_avi_ifttt_generic_table_report_msensor_query
*/
void zwp_avi_interfaces_msensor_free_msensor_get_list(zwu_array_list_t genericTableMSensorList);

/**
* @brief Generate query record for generic table based on device report
* @param[in]     iNofQueryCond     Number of query conditions
* @param[in]     desc_interface    Interface descriptor
* @param[in]     stQueryCond		Query condition from device report
* @param[out]    ppQueryCond   Query conditions generated
* @return 1 - success 0 - fail
* @note the query condition does not include trigger ID
* Caller of this function much call zwp_ifttt_free_table_value() free the resource
*/
int zwp_ifttt_generate_query_record_generic_table_for_device_report(
	int iNofQueryCond,
	zwifd_p desc_interface,
	ifttt_generic_table_device_data *pstQueryCond,
	pzwp_db_table_value *ppQueryCond);

/**
* @brief Generate query record for generic table based on command class arguments
* @param[in]     desc_interface    Interface descriptor
* @param[in]     device_data    device data
* @param[in]     command_class    command class ID
* @param[out]    piNofCol   Number of column of query conditions
* @param[out]    ppQueryCond   Query conditions generated
* @return 1 - success 0 - fail
* @note the query condition does not include trigger ID
* Caller of this function much call zwp_ifttt_free_table_value() free the resource
*/
int zwp_ifttt_generate_query_record_generic_table(
	zwifd_p desc_interface,
	ifttt_generic_table_device_data stQueryCond,
	int *piNofCol,
	pzwp_db_table_value *ppQueryCond);


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
	char* pcCommand_class_name);

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
	pzwp_db_table_value *ppTableRecord);

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
	int *piRecordUpdated);


/**
* @brief Get the DB table pointer for IFTTT cc specific trigger table
* @param[in]     desc_interface    Interface descriptor
* @param[in]     command_class	   Command class
* @return Pointer to IFTTT cc specific table
*/
pzwp_db_table zwp_avi_ifttt_cc_table_get(zwifd_p desc_interface, uint16_t command_class);


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
	zwp_db_table_value **pptable_data);

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
	char *sTriggerID);

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
	pzwp_db_table_value *ppTableRecord);

/**
* @brief Parse multilevel sensor state string to a IFTTT multilevel sensor get struct
* @param[in]     sState			State string
* @param[out]    ppSingleResult   Address of pointer to a IFTTT multilevel sensor get struct
* @return 1 - success 0 - fail
* @note 
* Caller of this function much free the multilevel sensor get pointer that returned by this function
*/
int zwp_avi_ifttt_parse_msensor_state_string(char *sState, zwp_avi_ifttt_sensor_multilevel_get_t **ppSingleResult);

/**
* @brief Assign IFTTT multilevel sensor get data based on key/value pair
* @param[in]     key			key
* @param[in]     value			value
* @param[in, out]    pSingleResult   Pointer to a IFTTT multilevel sensor get struct
* @return 1 - success 0 - fail
* @note
* Caller of this function much free the multilevel sensor get pointer that returned by this function
*/
int zwp_avi_ifttt_string_to_msensor_get_data(char *key, char* value, zwp_avi_ifttt_sensor_multilevel_get_t *pSingleResult);

#endif /* ZWP_AVI_IFTTT_DB_COMMON_H_ */
