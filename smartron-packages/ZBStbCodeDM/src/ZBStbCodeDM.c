#include<stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<stdbool.h>
#include<stdint.h>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<stdbool.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>
/*#pragma pack(1);*/
#define ON 10
#define OFF 11
#define hue 12
#define MoveToLevel 13

#define ZIGBEE_CHAR_STRING_ATTRIBUTE_TYPE				0x42 // Character string-Defined in first octet

#define NULL_ATTRIBUTE_VALUE 0xFFFF
#define OPERATION_SIZE 15
#define IEEE_ADDRESS_LENGTH 8

#define NUMBER_OF_ENDPOINTS 	15
#define NUMBER_OF_CLUSTERS 		6
#define NUMBER_OF_ATTRIBUTES 	6
#define NUMBER_OF_COMMANDS 		5

#define MAX_COUNT 10

#define START_ATTR_ID       	0
#define MAX_ATTR_ID		15

#define MSQ_TX_KEY 1111        
#define MSQ_RX_KEY 2222
#define MSQ_RX_DISC_KEY 3333

#define ALARM_1				(1<<0)
#define ALARM_2				(1<<1)
#define TAMPER				(1<<2)
#define BATTERY				(1<<3)
#define SUPERVISION_REPORTS	(1<<4)
#define RESTORE_REPORTS		(1<<5)
#define TROUBLE_OR_FAILURE	(1<<6)
#define AC_MAINS_FAULT		(1<<7)
#define TEST_MODE			(1<<8)
#define BATTERY_DEFECT		(1<<9)

static int mq_id_send, mq_id_Discovery_recve;
static int mq_id_recve;
int sendresult, revresult, Discovery_revresult;

struct mq_attr attr;
uint32_t sent_time,received_time;

uint32_t print_time_ms(void)
{
  struct timeval tv;
  uint32_t now;

  gettimeofday(&tv, NULL);
  now = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
  return now;
}

struct tm *CurrentTime(void) {
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
}

int initmsgqueue_send() {
	key_t key_send = MSQ_TX_KEY;
	return msgget(key_send, IPC_CREAT | 0666);
}

int initmsgqueue_recve() {
	key_t key_recve = MSQ_RX_KEY;
	return msgget(key_recve, IPC_CREAT | 0666);
}

int initmsgqueue_Discovery_recve() {
	key_t key_recve = MSQ_RX_DISC_KEY;
	return msgget(key_recve, IPC_CREAT | 0666);
}

/*#pragma pack(1);*/

typedef struct {
	uint16_t attributeId;
	uint16_t argtype; //1=u 2=v
	union {
		int intargValue;
		char str[MAX_COUNT];
	} argValue;
} argValueStruct;

typedef struct {

	long mtype;
	uint32_t MsgId;
	uint8_t Mode;
	uint16_t timeout;
	uint16_t nodeId; // short id of specific device.
	uint64_t IEEE;
	uint8_t endpoint; // destination endpoint.
	uint16_t deviceId;
	uint16_t clusterId; // cluster id.
	uint16_t commandId; // command id .
	uint16_t argcount; // no of count of datatypes.
	argValueStruct argValue_info[3]; // value of each argument with some delimanator.
	uint16_t ErrorCode;
} SendToZB;

typedef struct {
    uint16_t attributeId;
    int attributeVal;
    uint8_t attributeType;
} Attributedata;

typedef struct {
	uint16_t clusterId;
    uint8_t cmdId[NUMBER_OF_COMMANDS];
    uint8_t cmdCount;
    uint8_t attributeCount;    
    Attributedata attributes[NUMBER_OF_ATTRIBUTES];
} Cluster;

typedef struct {
    uint8_t endpointId;
    uint16_t deviceId;
    uint8_t clusterCount;
    Cluster cluster[NUMBER_OF_CLUSTERS];
} DefinedEndpoint;

typedef struct{
    long mtype;
    uint8_t Mode;
    uint32_t MsgId;
    uint16_t ErrorCode;
    uint16_t node_id;
    uint64_t IEEE;
    uint8_t endpointCount;
	uint8_t ManFacturererName[32];
	uint8_t DoorLockPIN[32];
	DefinedEndpoint Endpoint[NUMBER_OF_ENDPOINTS];
}EndDevice;

SendToZB DataToZB, DataFromZB;
EndDevice DiscoveryResponse;

int fd_set_blocking(int fd, int blocking) {
	/* Save the current flags */
/*	int flags = fcntl(fd, F_GETFL, 0);*/
/*	if (flags == -1)*/
/*		return 0;*/

/*	if (blocking)*/
/*		flags &= ~O_NONBLOCK;*/
/*	else*/
/*		flags |= O_NONBLOCK;*/
/*	return fcntl(fd, F_SETFL, flags) != -1;*/
return 1;
}

void IAS_ZONE(uint16_t status) {
	switch (status) {

	case ALARM_1:
		printf("\nIAS_ZONE Status : ALARM_1\n");
		break;

	case ALARM_2:
		printf("\nIAS_ZONE Status : ALARM_2\n");
		break;

	case TAMPER:
		printf("\nIAS_ZONE Status : TAMPER\n");
		break;

	case BATTERY:
		printf("\nIAS_ZONE Status : BATTERY\n");
		break;

	case SUPERVISION_REPORTS:
		printf("\nIAS_ZONE Status : SUPERVISION_REPORTS\n");
		break;

	case RESTORE_REPORTS:
		printf("\nIAS_ZONE Status : RESTORE_REPORTS\n");
		break;

	case TROUBLE_OR_FAILURE:
		printf("\nIAS_ZONE Status : TROUBLE_OR_FAILURE\n");
		break;

	case AC_MAINS_FAULT:
		printf("\nIAS_ZONE Status : AC_MAINS_FAULT\n");
		break;

	case TEST_MODE:
		printf("\nIAS_ZONE Status : TEST_MODE");
		break;
	case BATTERY_DEFECT:
		printf("\nIAS_ZONE Status : BATTERY_DEFECT\n");
		break;
	}
}

int main() {
	char ch = 0;
	fd_set_blocking(0, 0);
	printf("welcome to message Queue\n");
	mq_id_send = initmsgqueue_send();
	if (mq_id_send != -1)
		perror("Opened Successfully for send");
	printf("mq_id_send:%d\n",mq_id_send);
	mq_id_recve = initmsgqueue_recve();
	if (mq_id_recve != -1)
		perror("Opened Successfully for recve");
	printf("mq_id_recve:%d\n",mq_id_recve);
	mq_id_Discovery_recve = initmsgqueue_Discovery_recve();
	if (mq_id_Discovery_recve != -1)
		perror("Opened Successfully for Discovery_recve");
	printf("mq_id_Discovery_recve:%d\n",mq_id_Discovery_recve);
	printf("\n");
	printf("Size Of Transceiver = %ld,Size Of Disc Structure = %ld\n",
			sizeof(SendToZB), sizeof(EndDevice));
	printf("%s", "Please Enter Mode : ");
	DataToZB.Mode = 0;

	while (true) {

		ch = read(0, &DataToZB.Mode, 1);
		if (Discovery_revresult > 0 || revresult > 0)
			printf("Please Enter Mode : ");

		if (ch > 0 && DataToZB.Mode != '\n') {
			DataToZB.Mode = DataToZB.Mode - 0x30;
			printf("\nEntered Mode is :%d\n", DataToZB.Mode);

			switch (DataToZB.Mode) {
			case 1:
				DataToZB.mtype = 1;
				DataToZB.Mode = 1;
				DataToZB.MsgId = 1;
				DataToZB.nodeId = 0;
				DataToZB.endpoint = 0;
				DataToZB.clusterId = 0;
				DataToZB.commandId = 0;

				break;
			case 2:
				DataToZB.mtype = 1;
				DataToZB.MsgId = 2;
				DataToZB.Mode = 2;
				break;
			case 3:
				DataToZB.mtype = 1;
				DataToZB.MsgId = 3;
				DataToZB.Mode = 3;
				break;
			case 4:
				DataToZB.mtype = 1;
				DataToZB.MsgId = 4;
				DataToZB.Mode = 4;
				DataToZB.timeout = 60;
				break;
			case 7:
				fd_set_blocking(0, 1);
				printf("please Enter the nodeId : ");
				scanf("%hu", &DataToZB.nodeId);
				printf("please Enter the IEEE: ");
				scanf("%ju",&DataToZB.IEEE);
				printf("please Enter the MsgId : ");
				scanf("%d", &DataToZB.MsgId);
				printf("Entered IEEE is %ju\n",DataToZB.IEEE);
				DataToZB.mtype = 1;
				DataToZB.Mode = 7; // For Handshaking
				DataToZB.timeout = 0;
				DataToZB.clusterId = 0;
				DataToZB.commandId = 0;
				DataToZB.argValue_info[0].attributeId = 0;
				DataToZB.argcount = 0;
				memset(&DataToZB.argValue_info, 0, 10);
				DataToZB.ErrorCode = 0;
				fd_set_blocking(0, 0);
				break;

			case 5:
				fd_set_blocking(0, 1);
				printf("you are Entered into SET Mode\n");

				DataToZB.mtype = 1;
				DataToZB.MsgId = 5;
				DataToZB.Mode = 5; // For Set W/R
				/*printf("please Enter the timeout : ");
				 scanf("%hu",&DataToZB.timeout);*/
				printf("please Enter the nodeId : ");
				scanf("%hu", &DataToZB.nodeId);
				printf("please Enter the IEEE: ");
				scanf("%ju",&DataToZB.IEEE);
				printf("Entered IEEE is %ju\n",DataToZB.IEEE);
				printf("please Enter the endpoint : ");
				scanf("%d", &DataToZB.endpoint);

				printf("please Enter the clusterId : ");
				scanf("%hu", &DataToZB.clusterId);

				printf("please Enter the commandId : ");
				scanf("%hu", &DataToZB.commandId);

				printf("please Enter the argcount : ");
				scanf("%hu", &DataToZB.argcount);
				for (char i = 0; i < DataToZB.argcount; i++) {
					printf("please Enter the argValue_info[%d].argtype : ", i);
					scanf("%d",
							&DataToZB.argValue_info[i].argtype);
							
					printf("please Enter the argValue_info[%d] : ", i);
					scanf("%d",
							&DataToZB.argValue_info[i].argValue.intargValue);
				}
				DataToZB.ErrorCode = 0;
				fd_set_blocking(0, 0);
				break;
			}

			sendresult = msgsnd(mq_id_send, &DataToZB, sizeof(DataToZB), 0);

			if (sendresult != -1) {
				sent_time = print_time_ms();
				printf("\n*****Message Sent : %s", asctime(CurrentTime()));
				printf("Mode: %d \n", DataToZB.Mode);
				printf("nwk_addr: %d \n", DataToZB.nodeId);
				printf("endpoint_id: %d \n", DataToZB.endpoint);
				printf("cluster_id: %d \n", DataToZB.clusterId);
				printf("commandId: %d \n", DataToZB.commandId);
				printf("argcount %d \n", DataToZB.argcount);
				printf("**********END**********\n");
			} else {
				perror("my message is not sent ");
			}
		}

		Discovery_revresult = msgrcv(mq_id_Discovery_recve, &DiscoveryResponse,
				sizeof(EndDevice), 1, IPC_NOWAIT);

		if (Discovery_revresult != -1) {

			printf("\n******************* START : %s", asctime(CurrentTime()));

			printf("NodeId:%d\n", DiscoveryResponse.node_id);
			printf("MsgId : %d\n", DiscoveryResponse.MsgId);
			printf("Mode : %d\n", DiscoveryResponse.Mode);
			printf("IEEE : %ju \n", DiscoveryResponse.IEEE);
			printf("EP_Count:%d\n", DiscoveryResponse.endpointCount);

			for (char i = 0; i < DiscoveryResponse.endpointCount; i++) {
				printf("EP ID[%d] : %d\nDevice_Id:0x%04X\n\n", i + 1,
						DiscoveryResponse.Endpoint[i].endpointId,
						DiscoveryResponse.Endpoint[i].deviceId);
				printf("PRINTING CLUSTER COUNT : %d\n",
						DiscoveryResponse.Endpoint[i].clusterCount);
				for (char j = 0; j < DiscoveryResponse.Endpoint[i].clusterCount;
						j++) {
					printf("cluster=0x%04X,attCount=%d\n",
							DiscoveryResponse.Endpoint[i].cluster[j].clusterId,
							DiscoveryResponse.Endpoint[i].cluster[j].attributeCount);
					for (char k = 0;
							k
									< DiscoveryResponse.Endpoint[i].cluster[j].attributeCount;
							k++) {
						if (ZIGBEE_CHAR_STRING_ATTRIBUTE_TYPE
								== DiscoveryResponse.Endpoint[i].cluster[j].attributes[k].attributeType)
							printf(
									"\tATTRIBUTE_TYPE = %d,ATTRIBUTE_ID = 0x%X,ATTRIBUTE_VAL = %s\n",
									DiscoveryResponse.Endpoint[i].cluster[j].attributes[k].attributeType,
									DiscoveryResponse.Endpoint[i].cluster[j].attributes[k].attributeId,
									DiscoveryResponse.ManFacturererName);
						else
							printf(
									"\tATTRIBUTE_TYPE = %d,ATTRIBUTE_ID = 0x%X,ATTRIBUTE_VAL = 0x%X\n",
									DiscoveryResponse.Endpoint[i].cluster[j].attributes[k].attributeType,
									DiscoveryResponse.Endpoint[i].cluster[j].attributes[k].attributeId,
									DiscoveryResponse.Endpoint[i].cluster[j].attributes[k].attributeVal);
					}
					for (char l = 0;
							l
									< DiscoveryResponse.Endpoint[i].cluster[j].cmdCount;
							l++) {
						printf("Command_id: %d\n",
								DiscoveryResponse.Endpoint[i].cluster[j].cmdId[l]);
					}
					printf("\n");

				}

			}
			printf("\n*******************  END *******************\n");
		}

		revresult = msgrcv(mq_id_recve, &DataFromZB, sizeof(DataFromZB), 1,
		IPC_NOWAIT);
		if (revresult != -1) {
			received_time = print_time_ms();
			printf("\n*****Message Received : %s", asctime(CurrentTime()));
			printf("Mode: %d \n", DataFromZB.Mode);
			printf("nwk_addr: %d \n", DataFromZB.nodeId);
			printf("endpoint_id: %d \n", DataFromZB.endpoint);
			printf("cluster_id: %d \n", DataFromZB.clusterId);
			printf("attributeId: %d \n", DataFromZB.argValue_info[0].attributeId);
			printf("commandId: %d \n", DataFromZB.commandId);
			printf("deviceId: %d \n", DataFromZB.deviceId);
			printf("argcount %d \n", DataFromZB.argcount);
			printf("ErrorCode %d \n", DataFromZB.ErrorCode);
			printf("IEEE : %ju \n", DataFromZB.IEEE);
			printf("MsgId : %d\n", DataFromZB.MsgId);
			for(char k=0;k<DataFromZB.argcount;k++)
			{
				printf("argValue_info[%d].attributeId : %d\n",k,
					DataFromZB.argValue_info[k].attributeId);
				printf("argValue_info[%d].value : %d\n",k,
					DataFromZB.argValue_info[k].argValue.intargValue);
			}
			printf("My message read succesfully\n");
			printf("\nTime taken = %d ms\n",received_time - sent_time);
			printf("\n**********END**********\n");
			memset(&DataFromZB,0,sizeof(DataFromZB));
		}

		fflush(stdout);
	}
}

