#include <unistd.h>
#include <libubox/blobmsg_json.h>
#include <libubus.h>
#include "event2/event.h"
#include <sys/syslog.h>
#include "fw-env.h"
#include <json-c/json.h>

#define SM_JSON_FILE_PATH	"/etc/sm_config.json"

static struct ubus_context *ctx;
static struct blob_buf b;

static int ath02 = 0, ath01 = 0, eth0 = 0;
static unsigned int id;

static char prev_IP[16] = "192.168.221.1";
static int prev_status = 0;
static int ip_timeout;
static int network_timeout;
static int notify_timeout;

enum {
	IP_ADDRESS, IP_MAX,
};

static void read_config() {
	json_object *local_obj = json_object_from_file(SM_JSON_FILE_PATH);
	json_object *obj;
	if (local_obj != NULL) {
		obj = json_object_object_get(local_obj, "chk_network_timeout");
		network_timeout = json_object_get_int(obj);
//		printf ("\n The json object network_timeout: %d \n",network_timeout);

		obj = json_object_object_get(local_obj, "notifier_timeout");
		notify_timeout = json_object_get_int(obj);
//		printf ("\n The json object notify_timeout: %d \n",notify_timeout);

		obj = json_object_object_get(local_obj, "ip_timeout");
		ip_timeout = json_object_get_int(obj);
//		printf ("\n The json object ip_timeout: %d \n",ip_timeout);

		obj = json_object_object_get(local_obj, "prev_IP");
		strcpy(prev_IP, json_object_get_string(obj));
//		printf ("\n The json object prev_ip: %s \n",prev_IP);
		syslog(LOG_INFO, "read_config success in sm_main\n");

	}

}

static const struct blobmsg_policy ip_policy[IP_MAX] = { [IP_ADDRESS] = {
		.name = "ipv4-address", .type = BLOBMSG_TYPE_ARRAY, }, };

static void get_ip_cb(struct ubus_request *req, int type, struct blob_attr *msg) {
	char command[50] = { 0 };
	int ret;

	if (!msg)
		return;

	struct blob_attr *tb[ARRAY_SIZE(ip_policy)];

	if (blobmsg_parse(ip_policy, ARRAY_SIZE(ip_policy), tb, blob_data(msg),
			blob_len(msg)) != 0) {
		syslog(LOG_ERR, "Parsing failed for IP msg\n");
		return;
	}

	if (tb[IP_ADDRESS]) {
//		fprintf(stderr, "IP: %s\n", (char *) blobmsg_name(tb[IP_ADDRESS]));
		static const struct blobmsg_policy add_pol[] = { { "address",
				BLOBMSG_TYPE_STRING }, { "mask", BLOBMSG_TYPE_INT8 }, };

		struct blob_attr *cur;
		struct blob_attr *tb1[2];
		int rem;

		blobmsg_for_each_attr(cur, tb[IP_ADDRESS], rem)
		{
			if (blobmsg_parse_array(add_pol, 2, tb1, blobmsg_data(cur),
					blobmsg_data_len(cur)) != 0) {
				syslog(LOG_ERR, "Parsing failed for IP msg\n");
				return;
			}
			syslog(LOG_INFO, "IP : %s  \n",
					(char *) blobmsg_get_string(tb1[0]));
			int retip = strcmp((char *) blobmsg_get_string(tb1[0]), prev_IP);
			syslog(LOG_INFO, "retip value in get ipo cb:%d....\n",
					retip);
			if (retip != 0) {
				strcpy(prev_IP, (char *) blobmsg_get_string(tb1[0]));
				syslog(LOG_CRIT, " Modified IP Address: %s",
						prev_IP);
				strcpy(command, "/etc/init.d/ws_service stop");
				ret = system(command);
				syslog(LOG_CRIT, "ret = %d for ws service stop\n",
						ret);
				strcpy(command, "/etc/init.d/ws_service start");
				ret = system(command);
				syslog(LOG_CRIT, "ret = %d for ws service start\n",
						ret);
			}
		}
	}

}

static void subscribers_cb(struct ubus_context *ctx, struct ubus_object *obj) {
	syslog(LOG_INFO, "Subscribers active: %d\n",
			obj->has_subscribers);
}

static struct ubus_method test_methods[] = { };

static struct ubus_object_type test_obj_type = UBUS_OBJECT_TYPE("sm_interface",
		test_methods);

static struct ubus_object test_object = { .name = "sm_interface", /* The name of the object */
.type = &test_obj_type, .subscribe_cb = subscribers_cb, };

static int notifier_main(void) {
	int ret;

	/* Register an object, client can subscribe to this object */
	ret = ubus_add_object(ctx, &test_object);
	if (ret) {
		syslog(LOG_ERR, "Failed to add object : %s\n",
				ubus_strerror(ret));
		return ret;
	}

	ret = ubus_lookup_id(ctx, "network.interface.lan", &id);

	if (ret != UBUS_STATUS_OK) {
		syslog(LOG_ERR, "lookup for network interface failed\n");
		return ret;
	}
	syslog(LOG_INFO, "lookup for network interface successful\n");
	return ret;
}

/* Send notify message to all clients when needed */

static void notify_cb(struct uloop_timeout *timeout) {
	int ret;
	FILE *fp;
	char str[5] = { 0 };
	char command[50] = { 0 };

	/*-----------------------eth0-----------------------------------------*/
	fp = popen("cat /sys/class/net/eth0/carrier", "r");
	if (fp == NULL) {
		syslog(LOG_ERR, "Failed to run command..\n");
		exit(1);
	}
	if (fgets(str, sizeof(str), fp) != NULL)
		;
	{
		eth0 = atoi(str);
//			printf("eth0 =%d.....%s \n",eth0,str);
		*str = NULL;
	}
	pclose(fp);

	/*-----------------------cmd_24Ghz-----------------------------------------*/
	fp = popen(
			"iwconfig ath01 | grep Bit |  cut -d: -f  2 | cut -c 1,2,3,4,5,6",
			"r");
	if (fp == NULL) {
// 			printf ("Failed to run command..\n");
		exit(1);
	}
	if (fgets(str, sizeof(str), fp) != NULL)
		;
	{

		ath01 = atoi(str);
//		printf("ath01 =%d......%s \n", ath01, str);
		*str = NULL;

	}
	pclose(fp);

	/*-----------------------cmd_5Ghz-----------------------------------------*/
	fp = popen(
			"iwconfig ath11 | grep Bit |  cut -d: -f  2 | cut -c 1,2,3,4,5,6",
			"r");
	if (fp == NULL) {
//		printf("Failed to run command..\n");
		exit(1);
	}
	if (fgets(str, sizeof(str), fp) != NULL)
		;
	{
		ath02 = atoi(str);
//			printf("ath02 =%d......%s \n",ath02,str);
		*str = NULL;
	}
	pclose(fp);

	int status = eth0 || ath01 || ath02;

	/*************comparing prev&present ip **********************************************/
	if (prev_status != status) {
		if (status)
			syslog(LOG_CRIT, " Network interface is UP");
		else
			syslog(LOG_CRIT, " Network interface is DOWN");

		if (!prev_status && status)
			ubus_invoke(ctx, id, "status", NULL, get_ip_cb, NULL,
					ip_timeout * 1000);
		prev_status = status;
	}
	/*************comparing prev&present ip **********************************************/

	blob_buf_init(&b, 0);
	blobmsg_add_u16(&b, "status", status);
	blobmsg_add_string(&b, "IP", prev_IP); //Adding ip value to blob

	ret = ubus_notify(ctx, &test_object, "status", b.head, notify_timeout);
	if (ret)
		syslog(LOG_INFO, "Notify failed: %s ret value:%d \n",
				ubus_strerror(ret), ret);

	uloop_timeout_set(timeout, network_timeout);
}

void cb(evutil_socket_t fd, short what, void *arg) {
	struct ubus_context *ctx = arg;
	ubus_handle_event(ctx);
}

static void signal_cb(evutil_socket_t fd, short event, void *arg) {
	struct event_base *evloop = arg;
	event_base_loopbreak(evloop);
}

static struct uloop_timeout notify_timer = { .cb = notify_cb, };



int main(int argc, char **argv) {
	const char *ubus_socket = NULL;

        /* To Get Module TAG name in syslog */
        openlog ("[SM]", NULL, LOG_USER);

	read_config();

	uloop_init();

	ctx = ubus_connect(ubus_socket);
	if (!ctx) {
		syslog(LOG_ERR, "Failed to connect to ubus\n");
		return -1;
	}
	ubus_add_uloop(ctx);
	int ret = notifier_main();
	if (!ret)
		notify_cb(&notify_timer);
	uloop_run();
	ubus_free(ctx);
	uloop_done();
	closelog();
	return 0;
}

