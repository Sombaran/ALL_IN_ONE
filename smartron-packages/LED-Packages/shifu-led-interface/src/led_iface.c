#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "shifu-led.h"


/**
shifu_led_init - Creates an UDP socket
@return		return socket fd
*/
int shifu_led_init(){
	int clientSocket; 
	/*Create UDP socket*/
	clientSocket = socket(PF_INET, SOCK_DGRAM, 0);
	return clientSocket;
}

/**
shifu_led_pattern_send - send led_structure to led_server
@param[in] fd	socket fd returned in shifu_led_init()
@param[in] led	Address of led_structure
@return 	number of bytes sent
*/
int shifu_led_pattern_send(int fd, struct led_struct *led)
{
	struct sockaddr_in serverAddr;
	int rc;
	socklen_t addr_size;
	
	/*Configure settings in address struct*/
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
  
	/*Initialize size variable to be used later on*/
	addr_size = sizeof serverAddr;

	rc=sendto(fd,led,sizeof(*led),0,(struct sockaddr *)&serverAddr,addr_size);
	return rc;
}
