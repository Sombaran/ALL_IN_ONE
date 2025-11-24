/*
#21134
File: LedInterface.h
* By default avs sdk will not provide any led interface.
* AVS needs to send the led pattern of current action to led server.
* SO writing below api's to send the led pattern to led server.
* we can use these api's as:
* Initilize the led client function: 
	led_client_init();
* send perticular pattern to led-server :
	alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_WAKEWORD, 1, 0);
*/

#include <iostream>

#define LED_NO_INTERNET		1
#define LED_INTERNET_LOST	2
#define LED_AVS_FAIL		3
#define LED_AUTH_FAIL		4

namespace alexaClientSDK {
namespace avsCommon {
namespace utils {
namespace led {

/* Opens a UDP socket to send data to led server */
void led_client_init();

/* The function sending data to led server in a udp socket */
void send_pattern_to_led_server(int pattern, int state, int client);

}
}
}
}
