#ifndef _INTEGRATION_
#define _INTEGRATION_

#include "common.h"
#include <stdbool.h>

typedef struct
{
	uint8_t type;
	uint8_t no_of_events;
	uint8_t evt[NO_EVENTS];
}event;

typedef struct
{
	uint16_t cmd_cls;				/* Command Class*/
	event EVENT[NO_TYPES];				/* event_type array*/
	uint8_t no_of_types;				/* Types count*/
	uint8_t property_type;				/* Data type of property*/
	union property_state_res {
	uint8_t arr[NO_OF_COLOR_COMP];			/* Integer array property value*/
	uint8_t ch;					/* Char property value*/
	}property_value;				/* Union for the property values*/
}interface;						/* Interface Structure*/

typedef struct
{
	uint8_t ep_id;					/* End point id*/
	uint8_t generic_dc;				/* Generic Device Class*/
	uint8_t specific_dc;				/* Specific Device Class*/
	uint8_t no_of_intf;
	interface intf[NO_OF_INTERFACES];		/* Interface structure array*/
}endpoint;						/* Endpoint structure*/

typedef struct
{
	long mtype;					/* Message queue type*/
	uint32_t msgid;
	uint8_t operation;				/* Operation (Add/Get/Set)*/
	uint32_t home_id;				/* Home ID*/
	uint8_t node_id;				/* Node ID*/
	uint16_t vid;					/* Vendor ID*/
	uint16_t pid;					/* Product ID*/
	uint16_t vtype;					/* Vendor Product Type*/
	uint16_t category;				/* Device Category*/
	char Key[6];  					/* KEY*/
	int err_code;					/* 0= success, error code = failure*/
	uint8_t no_of_ep;
	endpoint ep[NO_OF_ENDPOINTS];			/* array of endpoint structures*/
}response_t;						/* Response Structure*/

typedef struct
{
	long mtype;					/* Message Queue type*/
	uint32_t msgid;					/* Message Queue ID*/
	uint8_t operation;				/* Operation (Add/Get/Set)*/
	uint32_t home_id;				/* Home ID*/
	uint8_t node_id;				/* Node ID*/
	uint8_t ep_id;					/* Endpoint ID*/
	uint8_t generic_dc;				/* Generic Device Class*/
	uint8_t specific_dc;				/* Specific Device Class*/
	uint16_t cmd_cls;				/* Command Class*/
	uint8_t type;					/* Sensor type*/
	char property_type;				/* Data type of Property*/
	union property_state {
		uint8_t arr[NO_OF_COLOR_COMP];		/* Integer array property value*/
		uint8_t ch;				/* Character value*/
	} property_value;				/* Union for storing property value*/
	char dsk[6];					/* Device specific key */
}request_t;						/* Request structure*/


/**
response_DM - This function will send response to the device manager by constructing the response structure using MESSAGE QUEUE.
*/
extern void response_DM(response_t dm_response);

#endif

