#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../zwave_application/common.h"
void die(char *s) {
	perror(s);

}

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

int count_value;

void main() {
	int msqid;
	key_t key;
	response_t dm_response;
	int size;
	int i, j, k, l, count_value = 0, Number_bytes = 0;

	key = 4444;

	if ((msqid = msgget(key, 0)) < 0)
		die("msgget()");

	size = sizeof(dm_response);

	while (1) {
		memset(&dm_response,0, sizeof(dm_response));
		printf("ZW: size of structure is %d\n", size);

		/* if (msgrcv(msqid_req, &request, sizeof(request_t), 1, 0) < 0) */
		Number_bytes = msgrcv(msqid, &dm_response, sizeof(dm_response), 1, 0);
		printf("ZW: Number_bytes received: %d\n", Number_bytes);

		if (i >= 0) {

			printf(
					"ZW: **************** START-DM_Received********************\n");
			printf("ZW: msgid: %ld\n", dm_response.msgid);
			printf("ZW: operation %d\n", dm_response.operation);
			printf("ZW: homeid: %lu\n", dm_response.home_id);
			printf("ZW: nodeid: %u\n", dm_response.node_id);
			printf("ZW: vid: %u\n", dm_response.vid);
			printf("ZW: pid : %u\n", dm_response.pid);
			printf("ZW: ptype : %u\n", dm_response.vtype);
			printf("ZW: Device category : %u\n", dm_response.category);
			printf("ZW: Error code: %d\n", dm_response.err_code);
			printf("ZW: Number of end points: %d\n", dm_response.no_of_ep);

			for (char i = 0; i < dm_response.no_of_ep /*&& i<5*/; i++) {
				printf("ZW: epid : %d\n", dm_response.ep[i].ep_id);
				printf("ZW: generic device class : %d\n",
						dm_response.ep[i].generic_dc);
				printf("ZW: specific device class : %d\n",
						dm_response.ep[i].specific_dc);
				printf("ZW: number of interfaces : %d\n",
						dm_response.ep[i].no_of_intf);

				for (char j = 0; j < dm_response.ep[i].no_of_intf; j++) {
					printf("ZW: cls : %d\n", dm_response.ep[i].intf[j].cmd_cls);
					for (char k = 0; k < dm_response.ep[i].intf[j].no_of_types;
							k++) {
						printf("ZW: TYPE is :%d\n",
								dm_response.ep[i].intf[j].EVENT[k].type);

						for (char a = 0;
								a
										< dm_response.ep[i].intf[j].EVENT[k].no_of_events;
								a++)
							printf("ZW: \tEVENT[%d].evt[%d]:%d\n", k, a,
									dm_response.ep[i].intf[j].EVENT[k].evt[a]);
					}

					printf("ZW: property type : %d\n",
							dm_response.ep[i].intf[j].property_type);

					if (dm_response.ep[i].intf[j].property_type == 2) {
						for (char count_value = 0; count_value < 5;
								count_value++) {
							printf("ZW: property value arr %d:%d\n\n",
									count_value,
									dm_response.ep[i].intf[j].property_value.arr[count_value]);
						}
					} else if (dm_response.ep[i].intf[j].property_type == 3) {
						printf("ZW: value ch %d\n\n",
								dm_response.ep[i].intf[j].property_value.ch);

					}

					else {
						printf("ZW: else:Invalid property type  ***DM---RECEIVED FAIL***\n");
					}
				}
			}

			printf("ZW: **************** END-dm_s ********************\n");

		}
	}
}

