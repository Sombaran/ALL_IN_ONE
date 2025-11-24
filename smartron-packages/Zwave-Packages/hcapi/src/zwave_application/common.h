#ifndef _COMMON_
#define _COMMON_


#include <sys/syslog.h>
#include "../include/zip_api.h"

#define MAX_DTLS_PSK 64				/* Maximum DTLS pre-shared key hex string length*/
#define LEN_GW_ADDR_STR 100			/*Length of gateway address string*/
#define HOST_PORT 0				/*Host port*/
#define LEN_PSK_STR 384				/*Length of PSK string*/
#define ZIP_GW_IP 16

#define RM_NODE_STS_UNKNOWN 0			/* Remove node status: unknown*/
#define RM_NODE_STS_PROGRESS 1			/* Remove node status: in progress*/
#define RM_NODE_STS_DONE 2			/* Remove node status: done*/

#define RESET_NW_STS_UNKNOWN 0			/* Reset network status: unknown*/
#define RESET_NW_STS_PROGRESS 1			/* Reset network status: in progress*/
#define RESET_NW_STS_DONE 2			/* Reset network status: done*/

#define ADD_NODE_STS_UNKNOWN 0			/* Add node status: unknown*/
#define ADD_NODE_STS_PROGRESS 1			/* Add node status: in progress*/
#define ADD_NODE_STS_DONE 2			/* Add node status: done*/

#define SEC2_ENTER_KEY_REQ 1			/* Bit-mask for allowing S2 key request callback*/
#define SEC2_ENTER_DSK 2			/* Bit-mask for allowing S2 DSK callback*/

#define NO_OF_ENDPOINTS 5			/* Number of endpoints*/
#define NO_OF_INTERFACES 5			/* Number of interfaces*/
#define NO_OF_COLOR_COMP 5			/* Color component*/
#define SIZE_OF_OPERATION 20			/* Operation Array size*/
#define SIZE_OF_FRAMEWORK 10			/* Framework array size*/

#define NW_INIT 7				/* Network initialization*/
#define COMMISSIONING 1				/* Commissioning - Discovery*/
#define CONSIGN 2				/* Consign - Set*/
#define REFRESH 3				/* Refresh - Get*/
#define REMOVE 4				/* Remove node*/
#define RESET 5					/* Reset node*/
#define NODE_STATUS 6				/* Status of node - Alive, Down.*/
#define CLEAR_REQUEST 10			/* CLearing request structure */ 
#define FORCE_REMOVE 12				/* Force removal of node.*/
#define S2_SECURITY 9				/* Security Request*/
#define NODE_SECURE 8				/* Node added securely*/
#define NODE_INSECURE 11			/* Node added insecurely*/
#define EXCLUSION 13			/* Exclusion request*/
#define INTIATE 14				/*Initial function request*/
#define UPDATE	15
#define ARR_TYPE 2				/* Integer Array Property*/
#define CHAR_TYPE 3				/* Char Property dataType*/

#define OP_SUCCESS 0
#define ERR_INVALID_HID 55
#define ERR_INVALID_REQ_OP 56
#define ERR_INVALID_CMD_CLS 57
#define ERR_HIGH_LEVEL_SYS 58
#define ERR_NODE_NOT_FOUND 59
#define ERR_INVALID_TYPE 60
#define ERR_BATTERY_STS  7

#define EP_INDEX 0				/* end point index for sending dm to zw*/
#define CC_INDEX 0				/* command class index for sending dm to zw*/


#define REQUEST_KEY 5555			/*Request Key, from Device Manager to Zwave*/
#define RESPONSE_KEY 4444			/*Response Key from Zwave to Device Managaer*/


#define NO_EVENTS 3				/*Considering 3 events for any sensor types*/
#define NO_TYPES 3				/*Considering 3 types for any sensors*/
#define GATEWAY_ATTEMPTS 5			/* gateway attempts  */


#define CONVERSION_VALUE 2.55			/*.Coversion to divide Lux value*/
#define TEMPERATURE 1				/*. Temperature Sensor Type*/
#define LUX 3					/*. LUminance Sensor type*/
#define LEFT_SHIFT_EIGHT_BITS 8			/*. LEFT SHIFT EIGHT BYTES */
#define LEFT_SHIFT_FIVE_BITS 5			/*. LEFT SHIFT FIVE BYTES */
#define PRECISION_ONE 1				/*. Sensor Data Precision is One  */
#define PRECISION_ZERO 0			/*.Sensor Data Precision is Zero*/
#define DIVIDE_BY_HUNDRED 100.0			/* Divide By Hundred*/
#define DIVIDE_BY_TEN 10.0			/*.Divide By Ten*/

typedef struct
{
	int use_ipv4;				/* Flag to indicate whether to use IPv4 or IPv6. 1=IPv4; 0=IPv6*/
	uint8_t *gw_addr_buf;			/* Gateway addresses buffer*/
	char **gw_name;				/*< Gateway names corresponding to entries in gw_addr_buf*/
	uint8_t gw_addr_cnt;			/* Number of gateway addresses in gw_addr_buf*/
	uint8_t zip_gw_ip[16];			/* Z/IP gateway address in IPv4 or IPv6*/
	volatile int init_status;		/* Network initialization status. 0=unknown; 1=done*/
	volatile int add_status;		/* Add node status.  0=unknown; 1=in progress; 2=done*/
	volatile int rm_status;			/* Remove node status.  0=unknown; 1=in progress; 2=done*/
	volatile int rst_status;		/* Reset network status.  0=unknown; 1=in progress; 2=done*/
	volatile unsigned sec2_cb_enter;	/* Control security 2 callback entry bitmask, see SEC2_ENTER_XXX. bit set = allowed callback, 0 = not allowed*/
	volatile int sec2_cb_exit;		/* Security 2 callback status. 1 = exited callback, 0 = waiting or still in the callback*/
	uint8_t sec2_add_node;			/* Flag to determine whether to use security 2 when adding node*/
	sec2_add_prm_t sec2_add_prm;		/* Add node with security 2 parameters*/
	zwnet_p zwnet;				/* Network handle*/

} hl_appl_ctx_t; 				/* High level Application context structure*/

hl_appl_ctx_t appl_ctx;




/**
bootup_scenario - Upon bootup and Upon Gateway discovery, this function updates the node details if any to the DeviceManager.
*/
void bootup_scenario();

/**
control_node - Controls a node in the  network using Command Class. (Set Operation)
*/
void control_node();

/**
 refresh_node - This function is used to perform GET operation on devices using Command Class. (Get Operation)
 */
void refresh_node();


uint8_t component_value[NO_OF_COLOR_COMP];
extern int dm_zw_comm_create(void);
#endif
