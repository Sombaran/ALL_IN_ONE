#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "common.h"

extern int msqid_resp;
extern int msqid_req;

int dm_comm_create(void);
/* Communication between Device Manager and Zwave Creation  via system V message Queues */
 
int dm_zw_comm_create(void) {
	int msgflg = IPC_CREAT | 0666;

	if ((msqid_req = msgget(REQUEST_KEY, msgflg)) < 0) {
		syslog(LOG_CRIT, "ZW: Failed to create Request Message queue");
		return -1;
	}

	if ((msqid_resp = msgget(RESPONSE_KEY, msgflg)) < 0) {
		syslog(LOG_CRIT, "ZW: Failed to create Response Message queue");
		return -1;
	}
	return 0;
}

