/*
 * gcc <> -lzmq -o <>
 */
#include <stdio.h>
#include <zmq.h>
#include <unistd.h>
#include <stdint.h>
void va_to_btman(char *);

char status[40];
void *requester;
void *context;


int main()
{
	va_to_btman("connect");
	return 0;
}

void va_to_btman(char *va_to_btman_cmd)
{
	uint8_t length;
	for(length=0;*(va_to_btman_cmd+length)!='\0';length++);
	context = zmq_ctx_new();
	requester = zmq_socket(context, ZMQ_REQ);
	zmq_connect(requester, "tcp://localhost:4274");
	zmq_send (requester, va_to_btman_cmd ,length, 0);
	zmq_recv(requester, status, 40, 0);
	printf ("Reception %s\n", status);
    zmq_close(requester);
}
