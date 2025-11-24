#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "../zwave_application/common.h"

void die(char *s) {
	perror(s);

}

struct request {
	long mtype; /* Message Queue type */
	uint32_t msgid; /* Message Queue ID */
	uint8_t operation; /* Operation (Add/Get/Set) */
	uint32_t home_id; /* Home ID */
	uint8_t node_id; /* Node ID */
	uint8_t ep_id; /* Endpoint ID */
	uint8_t generic_dc; /* Generic Device Class */
	uint8_t specific_dc; /* Specific Device Class */
	uint16_t cmd_cls; /* Command Class */
	uint8_t type; /* Sensor type */
	char property_type; /* Data type of Property */
	union property_state {
		uint8_t arr[NO_OF_COLOR_COMP]; /* Integer array property value */
		uint8_t ch; /* Character value */
	} property_value; /* Union for storing prop erty value */
	char dsk[6]; /* Device specific key */
};

void main() {

	int loop;
	int msqid;
	int msgflg = IPC_CREAT | 0666;
	key_t key;
	struct request sbuf = { 0 };
	size_t buflen;

	key = 5555;

	if ((msqid = msgget(key, msgflg)) < 0)
		die("msgget");

	sbuf.mtype = 1;

	while (1) {
		printf(
				"Enter the operation to be performed: \n1-add:\n2-control:\n3-refresh:\n4-remove\n5-reset:\n12-force remove: \n9-S2 Security:\n14-Intiate: \n15-UPdate:");
		scanf(" %u", &sbuf.operation);

		if ((sbuf.operation) == 1) {
			printf("In add operation-with msgid=123: \n");
			sbuf.msgid = 123;
			sbuf.operation = 1;
		} else if ((sbuf.operation) == 4) {
			printf("In remove operation-with msgid=321: \n");
			sbuf.msgid = 321;
			sbuf.operation = 4;
		} else if ((sbuf.operation) == 14) {
			printf("In remove operation-with msgid=325: \n");
			sbuf.msgid = 325;
			sbuf.operation = 14;
		}
		else if ((sbuf.operation) == 15) {
			printf("In remove operation-with msgid=325: \n");
			sbuf.msgid = 325;
			sbuf.operation = 15;
		} else if ((sbuf.operation) == 5) {
			printf("In reset operation: \n");
			sbuf.msgid = 321;
			sbuf.operation = 5;
		} else if ((sbuf.operation) == 2 || (sbuf.operation) == 3
				|| (sbuf.operation) == 10 || (sbuf.operation) == 12
				|| (sbuf.operation) == 9) {
			printf("In control/refresh operation: \n");

			printf("Enter a msgid : ");
			scanf(" %lu", &sbuf.msgid);

			printf("Enter a operation : ");
			scanf(" %u", &sbuf.operation);

			printf("Enter a homeid : ");
			scanf(" %lu", &sbuf.home_id);

			fflush(stdin);
			printf("Enter a nodeid : ");
			scanf(" %d", &sbuf.node_id);

			printf("Enter a epid : ");
			scanf(" %d", &sbuf.ep_id);

			printf("Enter a generic device class : ");
			scanf(" %d", &sbuf.generic_dc);

			printf("Enter a specific device class : ");
			scanf(" %d", &sbuf.specific_dc);

			printf("Enter a cmd_cls : ");
			scanf(" %d", &sbuf.cmd_cls);

			printf("Enter a dsk: ");
			scanf(" %s", &sbuf.dsk);

			printf("Enter a sensor type : ");
			scanf(" %d", &sbuf.type);

			printf("Enter a property_type : ");
			scanf(" %d", &sbuf.property_type);

			if (sbuf.property_type == 2) {
				int count_value;
				for (count_value = 0; count_value < 5; count_value++) {
					printf("Enter a attribute_value : ");
					scanf(" %d", &sbuf.property_value.arr[count_value]);
				}
			} else if (sbuf.property_type == 3) {
				printf("Enter a attribute_value : ");
				scanf(" %d", &sbuf.property_value.ch);
			}

		}

		else
			printf("Wrong choice\n");

		buflen = sizeof(struct request);

		if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0) {
			printf("%d\n, %d\n, %lu\n, %d\n, %c\n, %d\n, %d\n", msqid,
					sbuf.mtype, sbuf.home_id, sbuf.node_id, sbuf.ep_id,
					sbuf.cmd_cls, sbuf.property_type, sbuf.property_value.ch,
					buflen);
			die("msgsnd");
		}

		else
			printf("Message Sent\n");
	}
}

