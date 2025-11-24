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
 * @file    zwp_hcapi.c
 *
 * @brief   HCAPI helpers
 * @details Helper functions for HCAPI calls.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-06-12
 * - Initial version
 */

#include <stdio.h>
#include <string.h>

#include "zwp_hcapi.h"

/** HCAPI error code - Error description mapping */
static struct {
	int code;
	char *string;
} g_zwp_hcapi_error_code[] =
	{
		{ZW_ERR_CACHE_AVAIL, "Data is available in cache"},
		{ZW_ERR_QUEUED, "The command is queued"},
		{ZW_ERR_NONE, "Operation succeeded"},
		{ZW_ERR_FAILED, "Operation failed"},
		{ZW_ERR_WRONG_IF, "Wrong interface"},
		{ZW_ERR_NO_RESP, "No response from controller"},
		{ZW_ERR_MEMORY, "Out of memory"},
		{ZW_ERR_NODE_NOT_FOUND, "Node not found"},
		{ZW_ERR_CLASS_NOT_FOUND, "Command Class not found"},
		{ZW_ERR_INTF_NOT_FOUND, "Interface to a class not found"},
		{ZW_ERR_INTF_NO_REP_HDLR, "Interface report handler not found"},
		{ZW_ERR_LAST_OP_NOT_DONE, "Last operation not completed yet, try again"},
		{ZW_ERR_NOT_IN_LIST, "Node not in protocol layer failed node ID list"},
		{ZW_ERR_OP_FAILED, "The requested operation failed"},
		{ZW_ERR_EP_NOT_FOUND, "Endpoint not found"},
		{ZW_ERR_RPT_NOT_FOUND, "The report command of an interface not found"},
		{ZW_ERR_NET_NOT_FOUND, "Network not found"},
		{ZW_ERR_CMD_VERSION, "Incorrect command class version"},
		{ZW_ERR_PENDING, "Operation pending, it cannot be canceled now"},
		{ZW_ERR_VALUE, "The value is invalid"},
		{ZW_ERR_QUEUE_FULL, "The queue is full"},
		{ZW_ERR_UNSUPPORTED, "The requested function is unsupported for this node"},
		{ZW_ERR_FILE_OPEN, "Open file error"},
		{ZW_ERR_FILE_WRITE, "Write file error"},
		{ZW_ERR_FILE_EOF, "The end-of-file was reached"},
		{ZW_ERR_FILE, "File is corrupted"},
		{ZW_ERR_FILE_HOME_ID, "File home id does not match"},
		{ZW_ERR_EXPIRED, "Expired"},
		{ZW_ERR_NO_RES, "No resource for mutex, semaphore, timer, etc"},
		{ZW_ERR_EVENT, "Event is not processed"},
		{ZW_ERR_TOO_LARGE, "Data size is too large"},
		{ZW_ERR_TOO_SMALL, "Data size is too small"},
		{ZW_ERR_TIMEOUT, "Timeout"},
		{ZW_ERR_TRANSMIT, "Transmission failed"},
		{ZW_ERR_NONCE_NOT_FOUND, "Security nonce not found"},
		{ZW_ERR_AUTH, "Authentication error"},
		{ZW_ERR_SEQ_NUMBER, "Incorrect sequence number"},
		{ZW_ERR_BUSY, "Busy, try again later"},
		{ZW_ERR_SEC_SCHEME, "Security scheme unsupported"},
		{ZW_ERR_TRANSPORT_INI, "Initialization error on transport layer"},
		{ZW_ERR_FRAME_INI, "Initialization error on frame layer"},
		{ZW_ERR_SESSION_INI, "Initialization error on session layer"},
		{ZW_ERR_APPL_INI, "Initialization error on application layer"},
		{ZW_ERR_UNEXPECTED, "The error was unexpected under normal circumstances"},
		{ZW_ERR_NETWORK_IF, "Network interface not configured properly"},
		{ZW_ERR_IP_ADDR, "The IP address is invalid"},
		{ZW_ERR_VERSION, "Wrong version number"},
		{ZW_ERR_INTF_NO_DATA, "Interface data is missing"},
		{ZW_ERR_FILE_READ, "Read file error"},
		{ZW_ERR_PARSE_FILE, "Parsing file failed"},
		{ZW_ERR_MISSING_ENTRY, "Missing mandatory entry"},
		{ZW_ERR_DEVCFG_NOT_FOUND, "Device specific configuration record not found"},
		{ZW_ERR_DISALLOWED, "The operation is disallowed under certain circumstances"}
	};

/** Z-Wave network operation code - Network operation description mapping */
static struct {
	uint8_t operation;
	char *string;
} g_zwp_hcapi_net_operation[] =
	{
		{ZWNET_OP_NONE, "None"},
		{ZWNET_OP_INITIALIZE, "Initialize"},
		{ZWNET_OP_ADD_NODE, "Add node"},
		{ZWNET_OP_RM_NODE, "Remove node"},
		{ZWNET_OP_RP_NODE, "Replace failed node"},
		{ZWNET_OP_RM_FAILED_ID, "Remove failed node"},
		{ZWNET_OP_INITIATE, "Initiate"},
		{ZWNET_OP_UPDATE, "Update network"},
		{ZWNET_OP_RESET, "Reset network"},
		{ZWNET_OP_MIGRATE_SUC, "Create primary controller by a SUC"},
		{ZWNET_OP_MIGRATE, "Migrate primary controller"}, 
		{ZWNET_OP_ASSIGN, "Assign role"},
		//{ZWNET_OP_LOAD_NW_INFO, "Load network and node information"},
		{ZWNET_OP_NODE_UPDATE, "Update node"},
		{ZWNET_OP_SEND_NIF, "Send node information frame"},
		{ ZWNET_OP_NW_CHANGED, "Network change detection" },
		{ ZWNET_OP_NODE_CACHE_UPT, "Update node cache info" },
		{ ZWNET_OP_SAVE_NW, "Save network and node information to persistent storage" },
		{ ZWNET_OP_SLEEP_NODE_UPT, "Update sleeping detailed node information when it is awake" },
		{ ZWNET_OP_FW_UPDT, "Firmware update" }
	};

/** Z-Wave network operation status code - Network operation status description mapping */
static struct {
	uint16_t status;
	char *string;
} g_zwp_hcapi_net_operation_status[] =
	{
		{OP_DONE, "Done"},
		{OP_FAILED, "Failed"}
	};

/** Z-Wave network operation intermediate status code - Network operation intermediate status description mapping */
static struct {
	uint8_t operation;
	struct {
		uint16_t status;
		char *string;
	} intermediate_status[5];
} g_zwp_hcapi_net_operation_intermediate_status[] =
	{
		{ZWNET_OP_ADD_NODE, {
				{OP_ADD_NODE_PROTOCOL_START, "Protocol part of adding node started" },
				{OP_ADD_NODE_PROTOCOL_DONE, "Protocol part of adding node done"},
				{OP_ADD_NODE_SEC_REQ_KEY_READY, "Security 2 inclusion - request key ready"},
				{OP_ADD_NODE_SEC_DSK_READY, "Security 2 inclusion - dsk ready"},
				{OP_ADD_NODE_GET_NODE_INFO, "Getting node detailed information"}
		}},
		{ZWNET_OP_RP_NODE, {
				{OP_RP_NODE_PROTOCOL_DONE, "Protocol part of replacing node done"},
				{OP_RP_NODE_GET_NODE_INFO, "Getting node detailed information"}
		}},
		{ZWNET_OP_INITIATE, {
				{OP_INI_PROTOCOL_DONE, "Protocol part of initiating done"},
				{OP_INI_SEC_OWN_DSK_READY, "Security 2 initiate - self dsk ready"},
				{OP_INI_GET_NODE_INFO, "Getting node detailed information"}
		}},
		{ZWNET_OP_UPDATE, {
				{OP_NU_TOPOLOGY, "Network topology update started"},
				{OP_NU_NEIGHBOR, "Node neighbor update started"},
				{OP_NU_GET_NODE_INFO, "Node information update started"}
		}},
	};

/** Z-Wave network node status code - Network node status description mapping */
static struct {
	int status;
	char *string;
} g_zwp_hcapi_node_status[] =
	{
		{ZWNET_NODE_ADDED, "Added"},
		{ZWNET_NODE_REMOVED, "Removed"},
		{ZWNET_NODE_UPDATED, "Updated"},
		{ZWNET_NODE_STATUS_ALIVE, "Alive"},
		{ZWNET_NODE_STATUS_DOWN, "Down"},
		{ZWNET_NODE_STATUS_SLEEP, "Sleeping" }
	};

/** Z-Wave network transmit status code - Network transmit status description mapping */
static struct {
	int status;
	char *string;
} g_zwp_hcapi_net_transmit_status[] =
	{
		{ZWNET_TX_OK, "OK"},
		{ZWNET_TX_NO_ACK, "Send frame timeout due to no ACK received"},
		{ZWNET_TX_SYS_ERR, "System error, the program should exit"},
		{ZWNET_TX_DEST_BUSY, "Message has not timed out yet.The destination host may have a long response time. (e.g. sleeping node)"},
		{ZWNET_TX_NOROUTE, "Frame failed to reach destination host"}
	};

/** Z-Wave controller role code - Controller role description mapping */
static struct {
	uint8_t role;
	char *string;
} g_zwp_hcapi_controller_role[] =
	{
		{ZWNET_CTLR_ROLE_PROXY, ZWP_HCAPI_CONTROLLER_ROLE_STRING_PROXY},
		{ZWNET_CTLR_ROLE_INCL, ZWP_HCAPI_CONTROLLER_ROLE_STRING_INCLUSION},
		{ZWNET_CTLR_ROLE_PRI, ZWP_HCAPI_CONTROLLER_ROLE_STRING_PRIMARY}
	};

/** Firmware update restart status code - Firmware update restart status description mapping */
static struct {
	uint8_t status;
	char *string;
} g_zwp_hcapi_firmware_update_target_restart_status[] =
	{
		{ZW_FW_UPDT_RESTART_OK, "Restart OK"},
		{ZW_FW_UPDT_RESTART_FAILED, "Restart failed due to no response from the target"}
	};

char *zwp_hcapi_error_code_to_string(int hcapi_code)
{
	int count;
	int i;
	char *string = NULL;

	count = sizeof(g_zwp_hcapi_error_code) / sizeof(g_zwp_hcapi_error_code[0]);

	string = "Unknown";
	for (i = 0; i < count; i++)
	{
		if (g_zwp_hcapi_error_code[i].code == hcapi_code)
		{
			string = g_zwp_hcapi_error_code[i].string;
			break;
		}
	}

	return string;
}

char *zwp_hcapi_net_operation_to_string(uint8_t operation)
{
	int count;
	int i;
	char *string = NULL;

	count = sizeof(g_zwp_hcapi_net_operation) / sizeof(g_zwp_hcapi_net_operation[0]);

	string = "Unknown";
	for (i = 0; i < count; i++)
	{
		if (g_zwp_hcapi_net_operation[i].operation == operation)
		{
			string = g_zwp_hcapi_net_operation[i].string;
			break;
		}
	}

	return string;
}

char *zwp_hcapi_net_operation_status_to_string(uint16_t status)
{
	int count;
	int i;
	char *string = NULL;

	count = sizeof(g_zwp_hcapi_net_operation_status) / sizeof(g_zwp_hcapi_net_operation_status[0]);

	string = "Unknown";
	for (i = 0; i < count; i++)
	{
		if (g_zwp_hcapi_net_operation_status[i].status == status)
		{
			string = g_zwp_hcapi_net_operation_status[i].string;
			break;
		}
	}

	return string;
}

char *zwp_hcapi_net_operation_intermediate_status_to_string(uint8_t operation, uint8_t status)
{
	int operation_count;
	int status_count;
	int i;
	int j;
	char *string = NULL;

	operation_count = sizeof(g_zwp_hcapi_net_operation_intermediate_status) / sizeof(g_zwp_hcapi_net_operation_intermediate_status[0]);

	status_count = sizeof(g_zwp_hcapi_net_operation_intermediate_status[0].intermediate_status) / sizeof(*g_zwp_hcapi_net_operation_intermediate_status[0].intermediate_status);

	string = "Unknown";
	for (i = 0; i < operation_count; i++)
	{
		if (g_zwp_hcapi_net_operation_intermediate_status[i].operation == operation)
		{
			j = 0;

			while ((j < status_count) && (g_zwp_hcapi_net_operation_intermediate_status[i].intermediate_status[j].status != 0))
			{
				if (g_zwp_hcapi_net_operation_intermediate_status[i].intermediate_status[j].status == status)
				{
					string = g_zwp_hcapi_net_operation_intermediate_status[i].intermediate_status[j].string;
					break;
				}
				j++;
			}
			break;
		}
	}

	return string;
}

char *zwp_hcapi_net_node_status(int status)
{
	int count;
	int i;
	char *string = NULL;

	count = sizeof(g_zwp_hcapi_node_status) / sizeof(g_zwp_hcapi_node_status[0]);

	string = "Unknown";
	for (i = 0; i < count; i++)
	{
		if (g_zwp_hcapi_node_status[i].status == status)
		{
			string = g_zwp_hcapi_node_status[i].string;
			break;
		}
	}

	return string;
}

char *zwp_hcapi_net_transmit_status_to_string(int8_t status)
{
	int count;
	int i;
	char *string = NULL;

	count = sizeof(g_zwp_hcapi_net_transmit_status) / sizeof(g_zwp_hcapi_net_transmit_status[0]);

	string = "Unknown";
	for (i = 0; i < count; i++)
	{
		if (g_zwp_hcapi_net_transmit_status[i].status == status)
		{
			string = g_zwp_hcapi_net_transmit_status[i].string;
			break;
		}
	}

	return string;
}

void zwp_hcapi_controller_role_to_string(int8_t role_bitmap, char *buffer, size_t buffer_size)
{
	int count;
	size_t remain;
	size_t bytes;
	char delimiter[] = ",";
	int i;

	count = sizeof(g_zwp_hcapi_controller_role) / sizeof(g_zwp_hcapi_controller_role[0]);

	if (buffer_size > 0)
	{
		buffer[0] = '\0';
	}

	remain = buffer_size;
	for (i = 0; i < count; i++)
	{
		if (g_zwp_hcapi_controller_role[i].role & role_bitmap)
		{
			if (buffer[0])
			{
				bytes = strlen(delimiter);
				if (remain > bytes)
				{
					strncat(buffer, delimiter, remain);
					remain -= bytes;
				}
			}

			bytes = strlen(g_zwp_hcapi_controller_role[i].string);
			if (remain >= bytes)
			{
				remain -= bytes;
			}
		}
	}

	return;
}


char *zwp_hcapi_firmware_update_target_restart_status_to_string(int8_t status)
{
	int count;
	int i;
	char *string = NULL;

	count = sizeof(g_zwp_hcapi_firmware_update_target_restart_status) / sizeof(g_zwp_hcapi_firmware_update_target_restart_status[0]);

	string = "Unknown";
	for (i = 0; i < count; i++)
	{
		if (g_zwp_hcapi_firmware_update_target_restart_status[i].status == status)
		{
			string = g_zwp_hcapi_firmware_update_target_restart_status[i].string;
			break;
		}
	}

	return string;
}

