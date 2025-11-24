#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <libubus.h>
#include <libubox/blobmsg_json.h>
#include "event2/event.h"
#include <pthread.h>
#include <event2/thread.h>
#include <sys/syslog.h>
#include <libubox/uloop.h>

extern const char *stat_net;//extern
extern bool intf_obj;//extern

struct ubus_context *ctx;
struct ubus_subscriber network_event;	//static

uint32_t obj_id;			//static

int sm_socket_init();			//void
void sm_event_call();
int sm_register_obj(char *);
