#include <unistd.h>
#include <sys/syslog.h>
#include "fw-env.h"
#include "sm_iface.h"
#include <libubox/uloop.h>
#include <json-c/json.h>

char *obj_name = "sm_interface";
extern int sm_socket_init();			//void
void sm_event_call();
void internet_status();
int sm_register_obj(char *);
//extern pthread_t tid_sm_soc;
pthread_t tid_inet_status, tid_sm_soc;

void internet_status() {
	while (1) {
		syslog(LOG_INFO, " network interface status %s \n",stat_net);
		sleep(1);
	}
}

void main() {
	int ret;

	/* To Get Module TAG name in syslog */
	openlog ("SM",NULL,LOG_USER); 

	syslog(LOG_INFO, "TEST CLIENT called\n");
	ret = sm_socket_init();
	ret = sm_register_obj(obj_name);

	sm_event_call();
	pthread_create(&tid_inet_status, NULL, internet_status, NULL);

	uloop_run();
	ubus_free(ctx);
	uloop_done();
	closelog();
}
