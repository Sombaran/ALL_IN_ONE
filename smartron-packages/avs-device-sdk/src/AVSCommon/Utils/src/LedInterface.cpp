#include <iostream>
#include <unistd.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "AVSCommon/Utils/LedInterface.h"

/* led pattern and structure realated header file defines by led-server */
#include "shifu-led.h"

namespace alexaClientSDK {
namespace avsCommon {
namespace utils {
namespace led {

/* static variable used to hold udp socket fd, to interface with led-server */  
static int LedSock_fd;

/**
 * led_client_init() - Function to opens a UDP socket
 */
void led_client_init()
{
	alexaClientSDK::avsCommon::utils::led::LedSock_fd = socket(PF_INET, SOCK_DGRAM, 0);
}

/**
 * send_pattern_to_led_server() - function used to send perticular led pattern and state(on/off) of pattern to led-server
 * @param[in] - LED pattern
 * @param[in] - state	on/off
 * @param[in] - Led client number
 */
void send_pattern_to_led_server(int pattern, int state, int client)
{
	/* data will be send in this structure format and is defined in shifu-led.h file. */
	struct led_struct led;
	
	led.pattern = pattern;
	led.state = state;
	led.value = client;	
	
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	
	/*Configure settings in address struct*/
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
  
	/*Initialize size variable to be used later on*/
	addr_size = sizeof serverAddr;	

	/* sending data to led-server */
	sendto(alexaClientSDK::avsCommon::utils::led::LedSock_fd, &led, sizeof(led), 0, (struct sockaddr *)&serverAddr, addr_size);
}

}
}
}
}
