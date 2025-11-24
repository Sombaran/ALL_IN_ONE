#include<stdio.h>
#include <stdlib.h>
#include <shifu-led.h>

int led_fd;	///< stores led-client socket fd

struct led_struct led_pattern_structure;	///< led pattern data

int main()
{
	led_fd = shifu_led_init(); 	///< led-client socket initializattion

	led_pattern_structure.pattern = LED_WPS_MODE; // led pattern VOLUME
	led_pattern_structure.state = 1;   // 1 is for pattern ON and 0 is OFF
	led_pattern_structure.value  = 0;	///< no of led's to glow
	shifu_led_pattern_send(led_fd, &led_pattern_structure);

	system("gst-launch-1.0 playbin uri=file:///announcements/WPS.mp3 audio-sink=alsasink");

	led_pattern_structure.pattern = LED_WPS_MODE; // led pattern VOLUME
	led_pattern_structure.state = 0;   // 1 is for pattern ON and 0 is OFF
	led_pattern_structure.value  = 0;	///< no of led's to glow
	shifu_led_pattern_send(led_fd, &led_pattern_structure);

	return 0;
}
