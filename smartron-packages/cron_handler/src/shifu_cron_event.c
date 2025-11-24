#include <stdio.h>
#include <string.h>
#include <sys/syslog.h>
#include <zmq.h>
#include <json-c/json.h>
#include "cron_handler.h"


char *ModuleData, *ModuleName;
char RespBuf[32];
int zmq_timeout = 1000;
void *pub_requester;
void *pub_context;
int ret =0;

int main(int argc,char **argv)
{
    syslog(LOG_INFO, "VA: CRONEVENT: CRON EVENT OCCURED");    
    ModuleName = argv[1];
    ModuleData = argv[2];
    int DataLen = 0;
    int VAClientIdForDM = 11; //client id of VA to DM
    json_object *JsonObj;

	JsonObj = json_object_new_object();

    pub_context = (void *)zmq_ctx_new();
    pub_requester = (void *)zmq_socket(pub_context, ZMQ_REQ);
    zmq_setsockopt(pub_requester, ZMQ_RCVTIMEO, &zmq_timeout, sizeof (int));
    
	if ((strcmp(ModuleName,"RULE") == 0))
	{
		zmq_connect(pub_requester, DM_ZMQ_PORT_NUM);
		json_object_object_add(JsonObj, "client", json_object_new_int(VAClientIdForDM));
		json_object_object_add(JsonObj, "command", json_object_new_string(ModuleName));
		json_object_object_add(JsonObj, "rule_id", json_object_new_string(ModuleData));
		json_object_object_add(JsonObj, "verb", json_object_new_string("RUN"));
		DataLen = (int)strlen(json_object_get_string(JsonObj));		
	}
	else
	{
		DataLen = 0;
		syslog(LOG_INFO, "VA: CRONEVENT: UNKNOWN CRON EVENT");
	}	
	
	if ( DataLen != 0)
	{
	    zmq_msg_t msg;
	    int rc = zmq_msg_init_size (&msg, DataLen);
	    strcpy(zmq_msg_data (&msg),json_object_get_string(JsonObj));

	    rc = zmq_msg_send (&msg, pub_requester, ZMQ_SNDMORE);
	
	    rc = zmq_msg_send (&msg, pub_requester, 0);
	    
		//zmq_msg_send (pub_requester, json_object_get_string(JsonObj), DataLen, 0);
		syslog(LOG_INFO, "VA: CRONEVENT: CRON EVENT DATA TO SEND: %s",json_object_get_string(JsonObj));
		json_object_put(JsonObj);
		
		zmq_recv(pub_requester, RespBuf, 512, 0);    
		syslog(LOG_INFO, "VA: CRONEVENT: RespBuf: %s", RespBuf);
	}
    syslog(LOG_INFO, "VA: CRONEVENT: CRON EVENT EXECUTED");    
    return 0;
}
