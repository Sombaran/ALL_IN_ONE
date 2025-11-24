#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "gtk_audio.h"
#include "gtk_i2c.h"
#include "gpio_num_tbl.h"
#include "gtk_gpio.h"
#include "gtk_popen.h"

#include "led.h"
#define TOTAL_RING_LED_NUM (12)

int g_interrupt = 0;

static void led_listen_cb() {
   // pthread_detach(pthread_self());
	char cmd[128];
	int angle = 0;
	int target_idx = 0;
	int bitmask = 0;
	unsigned int volume = 0;

	while(1) {

		while(!g_interrupt)
			usleep(100 * 1000);

		angle = (int)gtk_get_angle_of_arrival_4mic(&volume);

		if(angle < 0 || angle >= 360) //legal range is 0~359
			angle = 0;
        system("aplay -f S16_LE -r 48000 -c 1 /wav_files/ful_ui_wakesound.wav &");
		memset(cmd, 0x0, sizeof(cmd));
		snprintf(cmd, sizeof(cmd)-1, "i2cset -y 0 0x18 0x46 0xff 0xff 0x00 0x00 0x00 0x00 0x78 0x00 0xff 0x00 0x00 0x00 0x10 0x%02x 0x%02x i"
			, (angle >> 8) & 0xff, angle & 0xff);
		system(cmd);
        usleep(100 * 1000);

		printf("DS1: g_interrupt %d\n", g_interrupt);

        while(g_interrupt)
		{
		    //printf("DS2: g_interrupt %d\n", g_interrupt);
			angle = (int)gtk_get_angle_of_arrival_4mic(&volume);

			if(angle < 0 || angle >= 360) //legal range is 0~359
				angle = 0;
		    printf("Angle: %d\n", angle);
			target_idx = angle / (360 / TOTAL_RING_LED_NUM);

			/*the rule of DOA transfer to led_idx refer to trigger mode*/
			switch(target_idx)
			{
				case 0:
					bitmask = 0x0030;
					break;
				case 1:
					bitmask = 0x8010;
					break;
				default:
					bitmask = 0xC000 >> (target_idx - 2);
			}

		    //printf("DS3: g_interrupt %d\n", g_interrupt);
			memset(cmd, 0x0, sizeof(cmd));

			/* i2cset -y 0 0x18 [OP Code, 1 Byte]  [Brightness(B)-1, 2 Bytes] [Brightness(G)-1, 2 Bytes] [Brightness(R)-1, 2 Bytes]
				[Brightness(B)-2, 2 Bytes] [Brightness(G)-2, 2 Bytes] [Brightness(R)-2, 2 Bytes] [Bitmask, 2 Byte] i */
			snprintf(cmd, sizeof(cmd)-1, "i2cset -y 0 0x18 0x71 0xfd 0xe8 0x00 0x00 0x00 0x00 0x78 0x00 0xff 0x00 0x00 0x00 0x%02x 0x%02x i"
				, (bitmask >> 8) & 0xff, bitmask & 0xff);

			system(cmd);
			usleep(100 * 1000);
		   // printf("DS4: g_interrupt %d\n", g_interrupt);
		}

		angle = (int)gtk_get_angle_of_arrival_4mic(&volume);

		if(angle < 0 || angle >= 360) //legal range is 0~359
			angle = 0;
		memset(cmd, 0x0, sizeof(cmd));
		snprintf(cmd, sizeof(cmd)-1, "i2cset -y 0 0x18 0x4f 0xfd 0xe8 0x00 0x00 0x00 0x00 0x78 0x00 0xff 0x00 0x00 0x00 0x20 0x%02x 0x%02x i"
			, (angle >> 8) & 0xff, angle & 0xff);
		system(cmd);

	}
	return;
}

void alexa_finish() {
	char cmd[128];
	int angle = 0;
	unsigned int volume = 0;
		angle = (int)gtk_get_angle_of_arrival_4mic(&volume);

		if(angle < 0 || angle >= 360) //legal range is 0~359
			angle = 0;
		memset(cmd, 0x0, sizeof(cmd));
		snprintf(cmd, sizeof(cmd)-1, "i2cset -y 0 0x18 0x4f 0xfd 0xe8 0x00 0x00 0x00 0x00 0x78 0x00 0xff 0x00 0x00 0x00 0x20 0x%02x 0x%02x i"
			, (angle >> 8) & 0xff, angle & 0xff);
		system(cmd);
}

int doa_thrd_init() {
	g_interrupt = 0;
	pthread_create(&(led_listen_start), NULL, led_listen_cb, NULL);
}


