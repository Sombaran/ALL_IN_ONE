#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "shifu-led.h"

int main(int argc, char *argv[]){
  
	int rc;
	
	rc=shifu_led_init();
	
	struct led_struct led;
	
	while(1) {
		printf("Enter values..");
		printf("Pattern:");
		scanf("%d",&led.pattern);
		printf("state:");
		scanf("%d",&led.state);
		printf("value:");
		scanf("%d",&led.value);
		shifu_led_pattern_send(rc,&led);
	}
}
