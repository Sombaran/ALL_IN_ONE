#include "dm_led_interface.hpp"

/*
 * 1 ARM
 * 2 LINUX
 * */

#define DM_LED_INTERFACE	1

extern int Led_Socket;

/*DmLedInterface::DmLedInterface() {  // TODO Auto-generated constructor stub
}
DmLedInterface::~DmLedInterface() {    // TODO Auto-generated destructor stu
}*/

#if DM_LED_INTERFACE == 2

int shifu_led_init() {
	return (0);
}

int shifu_led_pattern_send(int fd, struct led_struct *led) {
	return (0);
}

#endif

bool DmLedInterface::DiscoveryStart() {

	//syslog(LOG_INFO, "[DM]: LED Interface device discovery pattern start");

	struct led_struct led;

	led.pattern = LED_DEVICE_DISCOVERY;
	led.state = LED_STATE_1;
	led.value = LED_VALUE;
	int LedRet = shifu_led_pattern_send(Led_Socket, &led);

	/*syslog(LOG_DEBUG,
			"[DM]: LED Interface device discovery pattern return : [%d]",
			LedRet);*/

	if (LedRet == LED_SUCCESS) {
		syslog(LOG_INFO,
				"[DM]: LED Interface device discovery pattern success");
		return true;

	} else {
		syslog(LOG_INFO, "[DM]: LED Interface device discovery pattern failed");
		return false;
	}
	//syslog(LOG_INFO, "[DM]: LED Interface discovery pattern start");
	return false;
}

bool DmLedInterface::DiscoveryStop() {

	//syslog(LOG_INFO, "[DM]: LED Interface device discovery stop pattern start");
	struct led_struct led;
	led.pattern = LED_DEVICE_DISCOVERY;
	led.state = LED_STATE_0;
	led.value = LED_VALUE;
	int LedRet = shifu_led_pattern_send(Led_Socket, &led);

	/*syslog(LOG_DEBUG,
			"[DM]: LED Interface device discovery stop pattern return : [%d]",
			LedRet);*/

	if (LedRet == LED_SUCCESS) {
		syslog(LOG_INFO,
				"[DM]: LED Interface device discovery stop pattern success");
		return true;

	} else {
		syslog(LOG_INFO,
				"[DM]: LED Interface device discovery stop pattern failed");
		return false;
	}

	//syslog(LOG_INFO, "[DM]: LED Interface device discovery stop pattern end");
	return false;
}

bool DmLedInterface::DeviceConnected() {

	//syslog(LOG_INFO, "[DM]: LED Interface device connected pattern start");

	struct led_struct led;
	led.pattern = LED_DEVICE_CONNECTED;
	led.state = LED_STATE_1;
	led.value = LED_VALUE;
	int LedRet = shifu_led_pattern_send(Led_Socket, &led);

	/*syslog(LOG_DEBUG,
			"[DM]: LED Interface device connected pattern return : [%d]",
			LedRet);*/

	if (LedRet == LED_SUCCESS) {
		syslog(LOG_INFO,
				"[DM]: LED Interface device connected pattern success");
		return true;

	} else {
		syslog(LOG_INFO, "[DM]: LED Interface device connected pattern failed");
		return false;
	}
	return false;
}

bool DmLedInterface::DeviceDisConnected() {

	//syslog(LOG_INFO, "[DM]: LED Interface device disconnected pattern start");

	struct led_struct led;
	led.pattern = LED_DEVICE_CONNECTED;
	led.state = LED_STATE_1;
	led.value = LED_VALUE;
	int LedRet = shifu_led_pattern_send(Led_Socket, &led);

	/*syslog(LOG_DEBUG,
			"[DM]: LED Interface device disconnected pattern return : [%d]",
			LedRet);*/

	if (LedRet == LED_SUCCESS) {
		syslog(LOG_INFO,
				"[DM]: LED Interface device disconnected pattern success");
		return true;

	} else {
		syslog(LOG_INFO,
				"[DM]: LED Interface device disconnected pattern failed");
		return false;
	}

	//syslog(LOG_INFO, "[DM]: LED Interface device disconnected pattern end");
	return false;
}

