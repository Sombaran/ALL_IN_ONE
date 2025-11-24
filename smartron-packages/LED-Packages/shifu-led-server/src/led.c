
/*
 * File: led.c
 * Copyright (C) 2018 Smartron India Private Limited - All Rights Reserved
 * AUTHOR :  Surya Teja
 *
 * Notes
 * -----
 * ->led server program
 * Socket address: SERVER_ADDRESS	"127.0.0.1"
 * Socket port: SERVER_PORT	 7891
 *
 * -> The led-server has below led-patters with bit mask values.
 * LED_OFF                    =     0,	         //< LED OFF
 * LED_VOLUME                 =    (1 << 0),     //< Volume , (volume up and down same priority)
 * LED_ERROR		      =    (1 << 2),     //< Error
 * LED_WAKEWORD               =    (1 << 3),     //< Wake word
 * LED_ALARM		      =    (1 << 4),     //< Alarm
 * LED_QUERY_PROCESSING       =    (1 << 5),     //< Query processing
 * LED_QUERY_RESPONSE         =    (1 << 6),     //< Query Response
 * LED_DEVICE_CONNECTED       =    (1 << 7),     //< Device Discovery
 * LED_DEVICE_DISCOVERY       =    (1 << 8),     //< Device Control
 * LED_MIC_MUTE               =    (1 << 10),    //< Mic mute
 * LED_SETUP_MODE	      =    (1 << 11),    //< device registration mode
 * LED_VOICE_VOLUME           =    (1 << 12),    //< Volume change through voice
 * LED_MICMUTE_OFF            =    (1 << 13),    //< Mic UnMute after Mute
 * LED_BLUTOOTH_DISCOVERY     =    (1 << 14),    //< Blutooth Discovery
 * LED_BLUTOOTH_CONNECTED     =    (1 << 15),    //< Blutooth Connected
 * LED_NETWORK_RESET          =    (1 << 16),    //< Network Reset
 * LED_WPS_MODE               =    (1 << 17)     //< WPS Mode
 *
 * How other applications can interact with led server :
 * If any client application/program want to interact with "led server" should
 * add the liblediface.so library in their Make file Dependency section as +shifu-led-interface
 *
 * liblediface.so:
 * Consists of two api's and pattern structure (data) which are listed below.
 * i. shifu_led_init(); .........................//< led-client socket initializattion
 * ii.Client should add data to the structure:
 * struct led_struct led_pattern_structure;	///< led pattern data, which consists of the follwing data,
 * struct led_struct{
 * int pattern;	///< LED_XXXX
 * int state;	///< SHIFU_LED_PATTERN_XXX (0 for OFF and 1 for ON)
 * int value;	///< The data is set only for setting the led volume level(allowed values 0 10)
 * };
 * iii. shifu_led_pattern_send(int fd,struct led_struct *);	///< API to send pattern data to led-server
 *
 * How it works:
 * Client will request for the particular pattern whether to ON/OFF through sendto() socket with
 * the server address and server port as mentioned above,
 * then led-server reads the data from the client through the recvfrom() socket and led-server will
 * process the data and play the particular pattern depending upon the priority and also maintins
 * the pattern in queue formate and play's one after the other if exits.
 */

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <fw-env.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/timerfd.h>
#include <sys/wait.h>
#include "led.h"

#define TOTAL_RING_LED_NUM (12)

#define ALARM_MIC_ON  1
#define ALARM_MIC_OFF 0
#define DEVICE_DISCOVERY_ON  2
#define DEVICE_DISCOVERY_OFF 0

#define SET_BIT(value, position) (value | position)
#define CLR_BIT(value, position) (value & (~position))

#define BUFFER_MAX 2
#define INTERNET_UCI_CMD  "uci get shifu.conf.INTERNET_STATUS"
#define MOBILE_NOTI_CMD   "uci get shifu.conf.INTERNET_MOBI_NOTIFICATION"

/****
 * [21961]
 * 
 * mutex and condition  variables for mobile notification,set up pattern  to send status  to the no_Internet_Check_Monitor thread
 * 
 * mobile_notif = 0(0 means no need to notify,1 means need to notify) default=0;
 * 
 *	internet_state = -1 (0 means internet not there, 1 means internet available -1 means i didn't get any status from ping module;
 *  set_up_pattern_off = 0; (0 means setup pattern running,1 means setup pattern off
 * 
 * 
 * ***/


pthread_mutex_t internet_Lock;
pthread_mutex_t decision_lock;
pthread_mutex_t avs_config_lock;
pthread_cond_t internet_cond;
pthread_t led_internet_check;
pthread_t avs_config;
int mobile_notif = 0;
int internet_state = -1;
int set_up_pattern_off = 0;
void led_priority_decision(struct led_struct req_obj);
/****  end******/
int g_interrupt;
int alarm_mic_mix_state = ALARM_MIC_OFF; //Bug:21487

// Redmine ID: 21427 Changed the shifu alexa volume db levels
/* stores Shifu volume levels from 1 to 10 array in db */
char volume_table_shifu_alexa[]  = {0, 44, 47, 50, 53, 56, 59, 62, 65, 68, 71};

/****
 * [21961]
 *
 * Function for getting uci parameters from the system
 *    "uci get shifu.conf.INTERNET_STATUS"
 * 		1=internet available
 *      0=internet not available
 *
 *
 *    "uci get shifu.conf.INTERNET_MOBI_NOTIFICATION"
 *  	1=mobile notification on
 * 		0= mobile notification off
 *
 * ***/



int func_To_Get_from_UCI(char *cmd, int *val) {

    int rc = 0;
    /*we are opening a file pointer*/
    FILE *fp = NULL;
    char buf[BUFFER_MAX];

    /* Open the command for reading. */
    fp = popen(cmd, "r"); /*we are checking pipe opened or not opened then exit with -1*/
    if (fp == NULL) {
        syslog(LOG_ERR, "LED_SERVER:failed to open uci pipe");
        rc =-1;
    } else {
        if(fgets(buf, BUFFER_MAX, fp) == NULL) {
            syslog(LOG_ERR, "LED_SERVER:failed to read uci pipe");
        } else {
            *val = atoi(buf);
            rc = 1;
        }
        /* close */
        pclose(fp);
   }
   return rc;
}

/****
 * [21961]
 *
 * Function for checking internet is available or not for every 10 seconds.
 *
 * If internet and mobile notification on are  present we will send LED_NO_INTERNET_CHECK_NOTI=off to led server.
 *
 * mobile notification is off  we will not glow any led patterns.
 *
 * mobile notification=on and skip registration/Alexa registation done then
 *
 *
 * internet on we will send LED_NO_INTERNET_CHECK_NOTI=off to led server.
 *  internet off we will send LED_NO_INTERNET_CHECK_NOTI=on to led server
 *
 *
 * ***/


void* no_Internet_Check_Monitor(void *var) {
    int rc = -1;
    /*********************************************/
    	///< stores led-client socket fd
	struct led_struct led_pattern_structure;	///< led pattern data

    led_pattern_structure.pattern = LED_NO_INTERNET_CHECK_NOTI; // LED_NO_INTERNET_CHECK_NOTI


    /*************************************************/
    sleep(6);//Time required to get initialize ping module at start of the system//
    //system("uci set shifu.conf.INTERNET_MOBI_NOTIFICATION=1 && uci commit shifu");
    rc = func_To_Get_from_UCI(MOBILE_NOTI_CMD, &mobile_notif);
    if(rc == -1) {
        //fail we need to check;
        return NULL;
    } else {
        while(1) {
            if(mobile_notif != 1 && set_up_pattern_off ==0) {
                pthread_mutex_lock(&internet_Lock);
                pthread_cond_wait(&internet_cond, &internet_Lock);
                pthread_mutex_unlock(&internet_Lock);
            }
            rc = func_To_Get_from_UCI(INTERNET_UCI_CMD, &internet_state);
            if(rc ==-1) {
                continue;
            } else {
                if(internet_state == 0 && mobile_notif == 1 && set_up_pattern_off == 1) {
                    //send no_internet led on
                    led_pattern_structure.state = 1;   // 1 is for pattern ON and 0 is OFF
					led_pattern_structure.value  = 1;	///< no of led's to glow
					led_priority_decision(led_pattern_structure);
                } else if(internet_state == 1) {
                    //shifu_led_pattern_processing(LED_NO_INTERNET_CHECK_NOTI, 0, 0);
                    //send no_internet led off
                    led_pattern_structure.state = 0;   // 1 is for pattern ON and 0 is OFF
					led_pattern_structure.value  = 1;	///< no of led's to glow
					//led_priority_decision(led_pattern_structure);
                }
            }
            sleep(10);//time required to repeat no internet led pattern for every 10 sec
       }
    }
    return NULL;
}

/**
 * get_vol_db_with_index() - gives thechar volume_table_shifu_alexa[]  = {0,25,32,38,46,50,56,60,64,68,71}; present volume
 * @param[ ]
 * @return   present volume value
 */
int get_vol_db_with_index(int index) {
	return volume_table_shifu_alexa[index];
}

float gtk_get_angle_of_arrival_4mic(unsigned int *p_volume)
{
    FILE *fp;
    char buf[16] = {'\0'};

    fp = popen("host_demo --4micDOA", "r");
    if (fp) {
        fgets(buf, sizeof(buf), fp);
        pclose(fp);
    }

    return atof(buf);
}

static void led_listen()
{
	syslog(LOG_INFO, "%s\n", __func__);
	g_interrupt = 0;
	led_listen_thread();
	return;
}

void led_listen_thread()
{
	pthread_t led_listen_start;
	pthread_create(&(led_listen_start), NULL, led_listen_cb, NULL);
}

static void *led_listen_cb() {
    pthread_detach(pthread_self());
	char cmd[128];
	int angle = 0;
	int target_idx = 0;
	int bitmask = 0;
	unsigned int volume = 0;
    int tmp = 0;

    angle = (int)gtk_get_angle_of_arrival_4mic(&volume);
    tmp = angle;
    if(angle < 0 || angle >= 360) //legal range is 0~359
        angle = 0;
    memset(cmd, 0x0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd)-1, "i2cset -y 0 0x18 0x46 0xfd 0xe8 0x00 0x00 0x00 0x00 0x78 0x00 0xff 0x00 0x00 0x00 0x15 0x%02x 0x%02x i"
			, (angle >> 8) & 0xff, angle & 0xff);
    system(cmd);

    usleep(1000 * 1000);

	while(!g_interrupt) {

        angle = (int)gtk_get_angle_of_arrival_4mic(&volume);

		if(angle < 0 || angle >= 360) //legal range is 0~359
			angle = 0;

		target_idx = angle / (360 / TOTAL_RING_LED_NUM);

		syslog(LOG_DEBUG, "[%s] angle=%d, target_idx=%d\n", __func__, angle, target_idx);

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

		memset(cmd, 0x0, sizeof(cmd));
        if(tmp == angle) {

        } else {
		/* i2cset -y 0 0x18 [OP Code, 1 Byte]  [Brightness(B)-1, 2 Bytes] [Brightness(G)-1, 2 Bytes] [Brightness(R)-1, 2 Bytes]
			[Brightness(B)-2, 2 Bytes] [Brightness(G)-2, 2 Bytes] [Brightness(R)-2, 2 Bytes] [Bitmask, 2 Byte] i */
		snprintf(cmd, sizeof(cmd)-1, "i2cset -y 0 0x18 0x71 0xfd 0xe8 0x00 0x00 0x00 0x00 0x78 0x00 0xff 0x00 0x00 0x00 0x%02x 0x%02x i"
			, (bitmask >> 8) & 0xff, bitmask & 0xff);
		system(cmd);
        }
		usleep(100*1000);
	}

}

void alexa_finish() {
	char cmd[128];
	int angle = 0;
	unsigned int volume = 0;
    angle = (int)gtk_get_angle_of_arrival_4mic(&volume);
    system("i2cset -y 0 0x18 0x41");
    system("i2cset -y 0 0x18 0x77 0x0 i");
    queued_led_patterns = CLR_BIT(queued_led_patterns, LED_VOLUME);
    shifu_led_process_mixed_pattern(LED_VOLUME, 0, 0);
    if(angle < 0 || angle >= 360) //legal range is 0~359
        angle = 0;
    memset(cmd, 0x0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd)-1, "i2cset -y 0 0x18 0x4f 0xfd 0xe8 0x00 0x00 0x00 0x00 0x78 0x00 0xff 0x00 0x00 0x00 0x30 0x%02x 0x%02x i"
			, (angle >> 8) & 0xff, angle & 0xff);
    system(cmd);
}

/**
 * time_set() - initializes the timer for individual patterns
 * @param[in] - fd	        pattern fd's
 * @param[in] - itimerspec * time		timer sturcture
 * @param[in] - timeout	pattern-timeout
 */
static void time_set(int fd, int timeout) {
    struct itimerspec time;
    time.it_interval.tv_sec = 0;
    time.it_interval.tv_nsec = 0;
    time.it_value.tv_sec = timeout;
    time.it_value.tv_nsec = 0;

    if (timerfd_settime(fd, 0, &time, NULL) < 0) {
        syslog(LOG_ERR, "[LED]: timerfd_settime() failed: errno=%d", errno);
        close(fd);
        return;
    }
}

/**
 * myThreadFun() is a Thread function which is used for poll
 * the fd's with the LED pattern timeout handler
 * @param[in] - void
 */
void myThreadFun(void) {
    int retval;
    uint64_t exp;
    int readBytes = 0;

    while(1){
        retval = poll(poll_list, MAX_POLL_FDS, -1);
        if(retval < 0){
            perror("[LED]: Error while polling::");
            syslog(LOG_ERR, "[LED]: Error while polling::");
        }
        if((poll_list[0].revents & POLLIN) == POLLIN) {
            readBytes = read(poll_list[0].fd, &exp, sizeof(exp));
            shifu_led_default_timeout_handler();
        }
    }
  }

/**
 * printpattern() function used to print the pattern state for logging, which takes enum values
 * @param[in] - value	pattern value
 * @param[in] - state	on/off
 */
static char *printpattern(int value, int state) {
    switch(value) {

    case LED_OFF:
        {
            if (state) {
              return "LED_OFF";
            } else {
              return "LED_OFF_OFF";
            }
            break;
        }

    case LED_VOLUME:
        {
            if (state) {
              return "LED_VOLUME";
            } else {
              return "LED_VOLUME_OFF";
            }
            break;
        }

    case LED_NO_INTERNET_CHECK_NOTI:
        {
            if (state) {
                return "LED_NO_INTERNET_CHECK_NOTI";
            } else {
                return "LED_NO_INTERNET_CHECK_NOTI_OFF";
            }
            break;
        }

    case LED_ERROR:
        {
            if (state) {
                return "LED_ERROR";
            } else {
                return "LED_ERROR_OFF";
            }
            break;
        }

    case LED_WAKEWORD:
        {
            if (state) {
                return "LED_WAKEWORD";
            } else {
                return "LED_WAKEWORD_END";
            }
            break;
        }

    case LED_ALARM:
        {
            if (state) {
                return "LED_ALARM";
            } else {
                return "LED_ALARM_OFF";
            }
            break;
        }

    case LED_QUERY_PROCESSING:
        {
            if (state) {
                return "LED_QUERY_PROCESSING";
            } else {
                return "LED_QUERY_PROCESSING_OFF";
            }
            break;
        }

    case LED_QUERY_RESPONSE:
        {
            if (state) {
                return "LED_QUERY_RESPONSE";
            } else {
                return "LED_QUERY_RESPONSE_OFF";
            }
            break;
        }

    case LED_DEVICE_CONNECTED:
        {
            if (state) {
                return "LED_DEVICE_CONNECTED";
            } else {
                return "LED_DEVICE_CONNECTED_OFF";
            }
            break;
        }

    case LED_DEVICE_DISCOVERY:
        {
            if (state) {
                return "LED_DEVICE_DISCOVERY";
            } else {
                return "LED_DEVICE_DISCOVERY_OFF";
            }
            break;
        }

    case 512:
        {
            if (state) {
                return "LED_CONNECTED_AS_ROUTER";
            } else {
                return "LED_CONNECTED_AS_ROUTER_OFF";
            }
            break;
        }

    case LED_MIC_MUTE:
        {
            if (state) {
                return "LED_MIC_MUTE";
            } else {
                return "LED_MIC_MUTE_OFF";
            }
            break;
        }

    case LED_SETUP_MODE:
        {
            if (state) {
                return "LED_SETUP_MODE";
            } else {
                return "LED_SETUP_MODE_OFF";
            }
            break;
        }
    case LED_VOICE_VOLUME:
        {
            if (state) {
                return "LED_VOICE_VOLUME";
            } else {
                return "LED_VOICE_VOLUME_OFF";
            }
            break;
        }
    case LED_MICMUTE_OFF:
        {
            if (state) {
                return "LED_MICMUTE_OFF";
            } else {
                return "LED_MICMUTE_OFF_OFF";
            }
            break;
        }
    case LED_BLUTOOTH_DISCOVERY:
        {
            if (state) {
                return "LED_BLUTOOTH_DISCOVERY";
            } else {
                return "LED_BLUTOOTH_DISCOVERY_OFF";
            }
            break;
        }
    case LED_BLUTOOTH_CONNECTED:
        {
            if (state) {
                return "LED_BLUTOOTH_CONNECTED";
            } else {
                return "LED_BLUTOOTH_CONNECTED_OFF";
            }
            break;
        }
    case LED_NETWORK_RESET:
        {
            if (state) {
                return "LED_NETWORK_RESET";
            } else {
                return "LED_NETWORK_RESET_OFF";
            }
            break;
        }
    case LED_WPS_MODE:
        {
            if (state) {
                return "LED_WPS_MODE";
            } else {
                return "LED_WPS_MODE_OFF";
            }
            break;
        }
    case LED_WAKEWORD_OFF:
        {
            if (state) {
                return "LED_WAKEWORD_OFF";
            } else {
                return "LED_WAKEWORD_OFF_OFF";
            }
            break;
        }
    case LED_DND:
        {
            if (state) {
                return "LED_DND";
            } else {
                return "LED_DND_OFF";
            }
            break;
        }
    case LED_NOTIFY:
        {
            if (state) {
                return "LED_NOTIFY";
            } else {
                return "LED_NOTIFY_OFF";
            }
            break;
        }
    case LED_ANNOUNCEMENT:
        {
            if (state) {
                return "LED_ANNOUNCEMENT";
            } else {
                return "LED_ANNOUNCEMENT_OFF";
            }
            break;
        }
    default:
        {
            return "Mixed";
            break;
        }
    }

}

/**
 * printstate() -  used to print the pattern state for logging, which takes enum values
 * @param[in] - value	1/O--> On/Off
 */
static char *printstate(int value) {
    switch (value) {
    case 0:
        {
            return "OFF";
            break;
        }
    case 1:
        {
            return "ON";
            break;
        }
    default:
        {
            return "INVALID";
            break;
        }
    }
}

/**
 * Voice based LED pattern
 * set_volume_pattern() - generates the command for number of leds to glow for current volume
 * @param[in] volume_led_level	no of leds for present volume
 * @return 	void
 */
static void set_volume_pattern(int volume_led_level, int is_voice) {

    char cmd[128];
    cmd[0] = '\0';

    if(is_voice == 1)
    {
        if (volume_led_level <= 0) {
            system(VOLUME_MUTE_PATTERN);
        } else {
            if (volume_led_level >= 10) {
                volume_led_level = 10;
            }
            switch (volume_led_level) {
            case 1:
                {
                    system("i2cset -y 0 0x18 0x4e 0xff 0x00 0xff 0x00 0xff 0x00 0x20 0x10 0x2 0x2 i");
                    break;
                }
            case 2:
                {
                    system("i2cset -y 0 0x18 0x4e 0xff 0x00 0xff 0x00 0xff 0x00 0x20 0x10 0x2 0x3 i");
                    break;
                }
            case 3:
                {
                    system("i2cset -y 0 0x18 0x4e 0xff 0x00 0xff 0x00 0xff 0x00 0x20 0x10 0x2 0x4 i");
                    break;
                }
            case 4:
                {
                    system("i2cset -y 0 0x18 0x4e 0xff 0x00 0xff 0x00 0xff 0x00 0x20 0x10 0x2 0x5 i");
                    break;
                }
            case 5:
                {
                    system("i2cset -y 0 0x18 0x4e 0xff 0x00 0xff 0x00 0xff 0x00 0x20 0x10 0x2 0x6 i");
                    break;
                }
            case 6:
                {
                    system("i2cset -y 0 0x18 0x4e 0xff 0x00 0xff 0x00 0xff 0x00 0x20 0x10 0x2 0x7 i");
                    break;
                }
            case 7:
                {
                    system("i2cset -y 0 0x18 0x4e 0xff 0x00 0xff 0x00 0xff 0x00 0x20 0x10 0x2 0x8 i");
                    break;
                }
            case 8:
                {
                    system("i2cset -y 0 0x18 0x4e 0xff 0x00 0xff 0x00 0xff 0x00 0x20 0x10 0x2 0x9 i");
                    break;
                }
            case 9:
                {
                    system("i2cset -y 0 0x18 0x4e 0xff 0x00 0xff 0x00 0xff 0x00 0x20 0x10 0x2 0xa i");
                    break;
                }
            case 10:
                {
                    system("i2cset -y 0 0x18 0x4e 0xff 0x00 0xff 0x00 0xff 0x00 0x20 0x10 0x2 0xc i");
                    break;
                }
            }
        }

    } else {
        if(volume_led_level <= 0) {
            system(VOLUME_MUTE_PATTERN);
        } else {
            if (volume_led_level >= 10) {
                volume_led_level = 10;
            }
            switch (volume_led_level) {

            case 1:
                {
                    system("i2cset -y 0 0x18 0x42 0xb4 0xff 0xff 0xff 0xff 0xff 0x18 0x00 i");
                    break;
                }
            case 2:
                {
                    system("i2cset -y 0 0x18 0x42 0xb4 0xff 0xff 0xff 0xff 0xff 0x1c 0x00 i");
                    break;
                }
            case 3:
                {
                    system("i2cset -y 0 0x18 0x42 0xb4 0xff 0xff 0xff 0xff 0xff 0x1e 0x00 i");
                    break;
                }
            case 4:
                {
                    system("i2cset -y 0 0x18 0x42 0xb4 0xff 0xff 0xff 0xff 0xff 0x1f 0x00 i");
                    break;
                }
            case 5:
                {
                    system("i2cset -y 0 0x18 0x42 0xb4 0xff 0xff 0xff 0xff 0xff 0x1f 0x80 i");
                    break;
                }
            case 6:
                {
                    system("i2cset -y 0 0x18 0x42 0xb4 0xff 0xff 0xff 0xff 0xff 0x1f 0xc0 i");
                    break;
                }
            case 7:
                {
                    system("i2cset -y 0 0x18 0x42 0xb4 0xff 0xff 0xff 0xff 0xff 0x1f 0xe0 i");
                    break;
                }
            case 8:
                {
                    system("i2cset -y 0 0x18 0x42 0xb4 0xff 0xff 0xff 0xff 0xff 0x1f 0xf0 i");
                    break;
                }
            case 9:
                {
                    system("i2cset -y 0 0x18 0x42 0xb4 0xff 0xff 0xff 0xff 0xff 0x9f 0xf0 i");
                    break;
                }
            case 10:
                {
                    system("i2cset -y 0 0x18 0x42 0xb4 0xff 0xff 0xff 0xff 0xff 0xff 0xf0 i");
                    break;
                }
            }
        }
    }

    memset(cmd, 0x0, sizeof(cmd));
    syslog(LOG_DEBUG, "[LED]: setting HUB Volume to %d\n", get_vol_db_with_index(volume_led_level));

    if(set_val_to_conf(OPT_KEY_ALSA_ALSA_LED, volume_led_level) < 0)
            syslog(LOG_ERR, "[BTN_CTRL]:led_level Configuration faild");

    if(set_val_to_conf(OPT_KEY_ALSA_ALSA_SPKVOL, get_vol_db_with_index(volume_led_level)) < 0)
        syslog(LOG_ERR, "[BTN_CTRL]:speaker volume level Configuration faild");
}

/* #22366: setting volume to system level */
/**
 * set_volume_pattern() - gets the volume from uci and then set that volume
 * @return 	void
 */
void SetPlayerVolume()
{
	char temp_buf[100] = {0};
	int volumeIndex = 0;
	
    if(get_shifu_uci_param("AVSSPEAKERVOLUME", temp_buf) > -1){
        volumeIndex = (strtof(temp_buf, NULL) + 0.05) * 10;
    }
    else
    	volumeIndex = 8;
       
    sprintf(temp_buf, "amixer -c0 sset DAC1 %d", get_vol_db_with_index(volumeIndex));
    syslog(LOG_INFO, "[LED]: volume command %s", temp_buf);
    system(temp_buf);
}

/**
 * shifu_led_start_pattern() - start playing requested led pattern
 * @param[in] pattern	the pattern to trigger
 * @param[in] value	No of leds to glow(used for volume control)
 * @return
 */
static void shifu_led_start_pattern(int pattern, int value) {
    int shifu_internet = 0;
    switch (pattern) {
    case LED_OFF:
        {
            time_set(timer_fd, 0);
            system("i2cset -y 0 0x18 0x41");
            syslog(LOG_INFO, "[LED]: Excecuting:LEDOFF");
            break;
        }
    case LED_VOLUME:
        {
            time_set(timer_fd, 0);
            time_set(timer_fd, volume_timeout);
            set_volume_pattern(volume_level, 0);
            pattern_stop = LED_VOLUME;
            syslog(LOG_INFO, "[LED]: Executing:LED_VOLUME");
            break;
        }
    case LED_NO_INTERNET_CHECK_NOTI:
        {
            time_set(timer_fd, 0);
            time_set(timer_fd, 3);
            system("i2cset -y 0 0x18 0x45 0x00 0x00 0x00 0x00 0xff 0x00 0x10 0x20 0xFF 0xF0 0x3 i");
            pattern_stop = LED_NO_INTERNET_CHECK_NOTI;
            syslog(LOG_INFO, "[LED]: Executing:LED_NO_INTERNET_CHECK_NOTI");
            break;
        }
    case LED_ERROR:
        {
            time_set(timer_fd, 0);
            time_set(timer_fd, error_timeout);
            system("i2cset -y 0 0x18 0x45 0x00 0x00 0x00 0x00 0xff 0x00 0x10 0x20 0xFF 0xF0 0x3 i");
            pattern_stop = LED_ERROR;
            syslog(LOG_INFO, "[LED]: Executing:LED_ERROR");
            break;
        }
    case LED_ANNOUNCEMENT:
        {
        	/* #22366: setting volume to system level */
        	SetPlayerVolume();
            switch(value)
            {
                case 1:
                {
                    if(access( "/tmp/led_announcement.lock" , F_OK ) == -1) { //LED_NO_INTERNET
                        shifu_internet = NO_INTERNET;
                        system("touch /tmp/led_announcement.lock && gst-launch-1.0 playbin uri=file:///announcements/en_in_avs_system_prompt_error_offline_not_connected_to_internet.mp3 audio-sink=alsasink && rm /tmp/led_announcement.lock &");
                    }
                    break;
                }
                case 2:
                {
                    if(access( "/tmp/led_announcement.lock" , F_OK ) == -1) { //LED_INTERNET_LOST
                        system("touch /tmp/led_announcement.lock && gst-launch-1.0 playbin uri=file:///announcements/en_in_avs_system_prompt_error_offline_lost_connection.mp3 audio-sink=alsasink && rm /tmp/led_announcement.lock &");
                    }
                    break;
                }
                case 3:
                {
                    if(access( "/tmp/led_announcement.lock" , F_OK ) == -1) { //LED_AVS_FAIL
                        system("touch /tmp/led_announcement.lock && gst-launch-1.0 playbin uri=file:///announcements/en_in_avs_system_prompt_error_offline_not_connected_to_service_else.mp3 audio-sink=alsasink && rm /tmp/led_announcement.lock &");
                    }
                    break;
                }
                case 4:
                {
                    if(access( "/tmp/led_announcement.lock" , F_OK ) == -1) { //LED_AUTH_FAIL
                        system("touch /tmp/led_announcement.lock && gst-launch-1.0 playbin uri=file:///announcements/en_in_avs_system_prompt_error_offline_not_registered.mp3 audio-sink=alsasink && rm /tmp/led_announcement.lock &");
                    }
                    break;
                }
            }
            queued_led_patterns = CLR_BIT(queued_led_patterns, LED_ANNOUNCEMENT);
            pattern_stop = LED_ANNOUNCEMENT;
            syslog(LOG_INFO, "[LED]: Executing:LED_ANNOUNCEMENT");
            shifu_led_default_timeout_handler();
            if(shifu_internet == NO_INTERNET) {
                if(!(queued_led_patterns == LED_SETUP_MODE)) {
                    // #22267 Start: Synchronization between LED server & AVS after Booting of Shifu
                    led_pattern_running = LED_ERROR;
                    queued_led_patterns = SET_BIT(queued_led_patterns, LED_ERROR);
                    time_set(timer_fd, 0);
                    time_set(timer_fd, error_timeout);
                    system("i2cset -y 0 0x18 0x45 0x00 0x00 0x00 0x00 0xff 0x00 0x10 0x20 0xFF 0xF0 0x3 i");
                    pattern_stop = LED_ERROR;
                    syslog(LOG_INFO, "[LED]: Executing:LED_ERROR");
                    // #22267 End.
                }
            }
            break;
        }

    case LED_WAKEWORD:
        {
            time_set(timer_fd, 0);
            time_set(timer_fd, default_timeout);
            queued_led_patterns = CLR_BIT(queued_led_patterns, LED_ANNOUNCEMENT);
            led_listen();
            pattern_stop = LED_WAKEWORD;
            syslog(LOG_INFO, "[LED]: Executing:LED_WAKEWORD");
            break;
        }
    case LED_WAKEWORD_OFF:
        {
            system("i2cset -y 0 0x18 0x41");
            g_interrupt = 1;
            queued_led_patterns = CLR_BIT(queued_led_patterns, LED_WAKEWORD);
            alexa_finish();
            syslog(LOG_INFO, "[LED]: Executing:LED_WAKEWORD_OFF");
            pattern_stop = LED_WAKEWORD_OFF;
            usleep(200 * 1000);
            shifu_led_default_timeout_handler();
            break;
        }
    case LED_DND:
        {
            time_set(timer_fd, 0);
            time_set(timer_fd, 1);
            system("i2cset -y 0 0x18 0x45 0xff 0x00 0x00 0x00 0x99 0x00 0x10 0x30 0xFF 0xF0 0x1 i");
            pattern_stop = LED_DND;
            syslog(LOG_INFO, "[LED]: Executing:LED_DND");
            break;
        }
    case LED_NOTIFY:
        {
              time_set(timer_fd, 0);
          //  time_set(timer_fd, 1);
              system("i2cset -y 0 0x18 0x45 0x00 0x00 0xff 0x00 0xff 0x00 0x20 0x35 0xFF 0xF0 0x0 i");
              pattern_stop = LED_NOTIFY;
              syslog(LOG_INFO, "[LED]: Executing:LED_NOTIFY");
            break;
        }
    case LED_ALARM:
        {
            g_interrupt = 1;
            queued_led_patterns = CLR_BIT(queued_led_patterns, LED_WAKEWORD);
            time_set(timer_fd, 0);
            time_set(timer_fd, ALARM_TIMEOUT);
            system("i2cset -y 0 0x18 0x78 0x23 i");
            pattern_stop = LED_ALARM;
            syslog(LOG_INFO, "[LED]: Executing:LED_ALARM");
            break;
        }
    case LED_QUERY_PROCESSING:
        {
            system("i2cset -y 0 0x18 0x41");
            g_interrupt = 1;
            queued_led_patterns = CLR_BIT(queued_led_patterns, LED_WAKEWORD);
            time_set(timer_fd, 0);
            time_set(timer_fd, query_processing_timeout);
            system("i2cset -y 0 0x18 0x47 0xFD 0xE8 0x00 0x00 0x00 0x00 0x78 0x00 0xff 0x00 0x00 0x00 0x60 0x04 i");
            pattern_stop = LED_QUERY_PROCESSING;
            syslog(LOG_INFO, "[LED]: Executing:LED_QUERY_PROCESSING");
            break;
        }
    case LED_QUERY_RESPONSE:
        {
            time_set(timer_fd, 0);
            time_set(timer_fd, query_response_timeout);
            system("i2cset -y 0 0x18 0x70 0xFD 0xE8 0x00 0x00 0x00 0x00 0x78 0x00 0xff 0x00 0x00 0x00 0x08 0x46 i");
            pattern_stop = LED_QUERY_RESPONSE;
            syslog(LOG_INFO, "[LED]: Executing:LED_QUERY_RESPONSE");
            break;
        }
    case LED_DEVICE_CONNECTED:
        {
            time_set(timer_fd, 0);
            time_set(timer_fd, device_connected_timeout);
            system("i2cset -y 0 0x18 0x46 0xff 0x00 0x00 0x00 0x00 0x00 0xff 0x00 0x00 0x00 0x00 0x00 0x50 0x01 0x0e i");
            pattern_stop = LED_DEVICE_CONNECTED;
            syslog(LOG_INFO, "[LED]: Executing:LED_DEVICE_CONNECTED");
            break;
        }
    case LED_DEVICE_DISCOVERY:
        {
            time_set(timer_fd, 0);
            time_set(timer_fd, device_discovery_timeout);
            system("i2cset -y 0 0x18 0x47 0x00 0x00 0x00 0x00 0x00 0x00 0xFD 0xE8 0x00 0x00 0x00 0x00 0x60 0x20 i");
            pattern_stop = LED_DEVICE_DISCOVERY;
            syslog(LOG_INFO, "[LED]: Executing:LED_DEVICE_DISCOVERY");
            break;
        }
    case LED_WPS_MODE:
        {
        	/* #22366: setting volume to system level */
        	SetPlayerVolume();
            time_set(timer_fd, 0);
            time_set(timer_fd, default_timeout);
            system("i2cset -y 0 0x18 0x47 0x00 0x00 0x00 0x00 0x00 0x00 0xFD 0xE8 0x00 0x00 0x00 0x00 0x60 0x20 i");
            pattern_stop = LED_WPS_MODE;
            syslog(LOG_INFO, "[LED]: Excecuting:LED_WPS_MODE");
            break;
        }
    case LED_MIC_MUTE:
        {
            time_set(timer_fd, 0);
            queued_led_patterns = CLR_BIT(queued_led_patterns, LED_WAKEWORD_OFF);
            system("i2cset -y 0 0x18 0x42 0x00 0x00 0x00 0x00 0xff 0xff 0xFF 0xF0 i");
            syslog(LOG_INFO, "[LED]: Executing:LED_MIC_MUTE");
            break;
        }
    case LED_SETUP_MODE:
        {
            time_set(timer_fd, 0);
            system("i2cset -y 0 0x18 0x47 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x30 0x00 0xff 0x00 0x60 0x22 i");
            pattern_stop = LED_SETUP_MODE;
            syslog(LOG_INFO, "[LED]: Executing:LED_SETUP_MODE");
            break;
        }
    case LED_MICMUTE_OFF:
        {
            time_set(timer_fd, 0);
            time_set(timer_fd, micmute_off_timeout);
            system("i2cset -y 0 0x18 0x77 0x0 i");
            system("i2cset -y 0 0x18 0x4b 0x00 0x00 0x00 0x00 0xff 0x00 0x20 0x10 0x3 i");
            queued_led_patterns = CLR_BIT(queued_led_patterns, LED_MIC_MUTE);
            pattern_stop = LED_MICMUTE_OFF;
            syslog(LOG_INFO, "[LED]: Executing:LED_MICMUTE_OFF");
            break;
        }
    case LED_BLUTOOTH_DISCOVERY:
        {
            time_set(timer_fd, 0);
            time_set(timer_fd, bt_discovery_timeout);
            system("i2cset -y 0 0x18 0x45 0xff 0x00 0x00 0x00 0x00 0x00 0xff 0x00 0xff 0x00 0xff 0x00 0x20 0x10 0x82 i");
            pattern_stop = LED_BLUTOOTH_DISCOVERY;
            syslog(LOG_INFO, "[LED]: Executing:LED_BLUTOOTH_DISCOVERY");
            break;
        }
    case LED_BLUTOOTH_CONNECTED:
        {
            if((queued_led_patterns & LED_MIC_MUTE) || (queued_led_patterns & LED_ALARM)) {
                time_set(timer_fd, 0);
                time_set(timer_fd, 1);
                queued_led_patterns = CLR_BIT(queued_led_patterns, LED_WAKEWORD_OFF);
                system("i2cset -y 0 0x18 0x45 0xff 0xff 0x00 0x00 0x00 0x00 0x05 0x20 0xFF 0xF0 0x3 i");
                pattern_stop = LED_BLUTOOTH_CONNECTED;
                syslog(LOG_INFO, "[LED]: Executing:LED_BLUTOOTH_CONNECTED");
            } else {
                time_set(timer_fd, 0);
                time_set(timer_fd, bt_connected_timeout);
                queued_led_patterns = CLR_BIT(queued_led_patterns, LED_WAKEWORD_OFF);
                system("i2cset -y 0 0x18 0x45 0xff 0xff 0x00 0x00 0x00 0x00 0x05 0x20 0xFF 0xF0 0x3 i");
                pattern_stop = LED_BLUTOOTH_CONNECTED;
                syslog(LOG_INFO, "[LED]: Executing:LED_BLUTOOTH_CONNECTED");
            }
            break;
        }
    case LED_VOICE_VOLUME:
        {
            time_set(timer_fd, 0);
            time_set(timer_fd, voice_volume_timeout);
            set_volume_pattern(volume_level, 1);
            pattern_stop = LED_VOICE_VOLUME;
            syslog(LOG_INFO, "[LED]: Executing:LED_VOICE_VOLUME");
            break;
        }
    case LED_NETWORK_RESET:
        {
            time_set(timer_fd, 0);
            time_set(timer_fd, default_timeout);
            system("i2cset -y 0 0x18 0x44 0x78 0x00 0xff 0x00 0x00 0x00 0x20 0x10 0x6 i");
            pattern_stop = LED_NETWORK_RESET;
            syslog(LOG_INFO, "[LED]: Executing:LED_NETWORK_RESET");
            break;
        }
    default:
        {
            syslog(LOG_INFO, "[LED]: Invalid pattern request");
            break;
        }
    }
}

/**
 * get_state_value_for_pattern() - returns corresponding integer value to the paerticular pattern, to use that to get state in array
 * @param[in] pattern	led pattern value
 * @return	   int      value for corresponding pattern
 */
static int get_state_value_for_pattern(PATTERN pattern) {
    if (pattern == LED_OFF)
        return 0;
    else if (pattern == LED_VOLUME)
        return 1;
    else if (pattern == LED_MIC_MUTE)
        return 2;
    else if (pattern == LED_NO_INTERNET_CHECK_NOTI)
        return 3;
    else if (pattern == LED_ERROR)
        return 4;
    else if (pattern == LED_WAKEWORD)
        return 5;
    else if (pattern == LED_ALARM)
        return 6;
    else if (pattern == LED_QUERY_PROCESSING)
        return 7;
    else if (pattern == LED_QUERY_RESPONSE)
        return 8;
    else if (pattern == LED_DEVICE_CONNECTED)
        return 9;
    else if (pattern == LED_DEVICE_DISCOVERY)
        return 10;
    else if (pattern == LED_WPS_MODE)
        return 11;
    else if (pattern == LED_SETUP_MODE)
        return 12;
    else if (pattern == LED_MIXED)
        return 13;
    else if (pattern == LED_MICMUTE_OFF)
        return 14;
    else if (pattern == LED_BLUTOOTH_DISCOVERY)
        return 15;
    else if (pattern == LED_BLUTOOTH_CONNECTED)
        return 16;
    else if (pattern == LED_NETWORK_RESET)
        return 17;
    else if (pattern == LED_VOICE_VOLUME)
        return 18;
    else if (pattern == LED_WAKEWORD_OFF)
        return 19;
    else if (pattern == LED_DND)
        return 20;
    else if (pattern == LED_NOTIFY)
        return 21;
    else if (pattern == LED_ANNOUNCEMENT)
        return 22;
    else
        syslog(LOG_INFO, "[LED]: Invalid pattern request", pattern);
        return 0;
}

/**
 * shifu_led_resume_pattern() - plays the patterns present in queue list
 * @param[ ]
 * @return		void
 */
static void shifu_led_resume_pattern() {
    int i = 0;
    /* checking for any pattern is present in queue and start pattern if present */
    if (queued_led_patterns) {
        for (i = 0; i <=MAX_PATTERNS; i++) {
            if (queued_led_patterns & (1 << i)) {
                if((queued_led_patterns & LED_SETUP_MODE) && (queued_led_patterns & LED_MIC_MUTE)) {
                    shifu_led_start_pattern(LED_SETUP_MODE, 1);
                    syslog(LOG_DEBUG, "[LED]: started next pattern present in queue: %s", printpattern((1 << i), 1));
                    syslog(LOG_DEBUG, "[LED]: Start pattern: %s", printpattern((1 << i), 1));
                } else {
                    syslog(LOG_DEBUG, "[LED]: started next pattern present in queue: %s", printpattern((1 << i), 1));
                    syslog(LOG_DEBUG, "[LED]: Start pattern: %s", printpattern((1 << i), 1));
                    led_pattern_running = 1 << i;
                    shifu_led_start_pattern(1 << i, 1);
                }
                break;
            }
        }
    /* if no pattern present in queue turn on the running led pattern */
    } else {
        system("i2cset -y 0 0x18 0x77 0x0 i");
        led_pattern_running = LED_OFF;
        shifu_led_start_pattern(LED_OFF, 1);
    }
}

/**
 * shifu_led_default_timeout_handler() - call back fuction for default timeout patterns
 * stops the pattern after the default timeout
 * @return 	0
 */
static int shifu_led_default_timeout_handler(void) {
   if (led_pattern_running == LED_WAKEWORD) {
        queued_led_patterns = CLR_BIT(queued_led_patterns, pattern_stop);
		g_interrupt = 1;
        usleep(100 * 1000);
        syslog(LOG_DEBUG, "[LED]: [AUTO]:Default time out reached, pattern: %s\t stopped", printpattern(pattern_stop, 1));
        syslog(LOG_DEBUG, "[LED]: [AUTO]:Stop pattern:%s", printpattern(pattern_stop, 1));
        shifu_led_resume_pattern();
    } else if (led_pattern_running == LED_VOLUME) { //Bug:21487
        if(alarm_mic_mix_state == DEVICE_DISCOVERY_ON) {
            system("i2cset -y 0 0x18 0x77 0x0 i");
            system("i2cset -y 0 0x18 0x41");
            queued_led_patterns = CLR_BIT(queued_led_patterns, LED_VOLUME);
            system("i2cset -y 0 0x18 0x47 0x00 0x00 0x00 0x00 0x00 0x00 0xFD 0xE8 0x00 0x00 0x00 0x00 0x60 0x20 i");
            alarm_mic_mix_state = DEVICE_DISCOVERY_OFF;
            led_pattern_running = LED_MIXED;
        } else if(alarm_mic_mix_state == ALARM_MIC_ON) {
            system("i2cset -y 0 0x18 0x77 0x0 i");
            queued_led_patterns = CLR_BIT(queued_led_patterns, LED_VOLUME);
            //usleep(150 * 1000);
            /* Triggering MicMute Pattern */
            system("i2cset -y 0 0x18 0x42 0x00 0x00 0x00 0x00 0xff 0xff 0xFF 0xF0 i");
            //usleep(150 * 1000);
            system("i2cset -y 0 0x18 0x77 0x1 i");
            //usleep(150 * 1000);
            /* Triggering Alarm Pattern */
            system("i2cset -y 0 0x18 0x78 0x23 i");
            alarm_mic_mix_state = ALARM_MIC_OFF;
            led_pattern_running = LED_MIXED;
        } else {
            system("i2cset -y 0 0x18 0x77 0x0 i");
            queued_led_patterns = CLR_BIT(queued_led_patterns, LED_VOLUME);
            shifu_led_process_mixed_pattern(pattern_stop, 0, 0);
            shifu_led_resume_pattern();
            syslog(LOG_DEBUG, "[LED]: [AUTO]:Default time out reached, pattern: %s\t stopped", printpattern(pattern_stop, 1));
            syslog(LOG_DEBUG, "[LED]: [AUTO]:Stop pattern:%s", printpattern(pattern_stop, 1));
        }
    } else if (led_pattern_running == pattern_stop) {
        queued_led_patterns = CLR_BIT(queued_led_patterns, pattern_stop);
        syslog(LOG_DEBUG, "[LED]: [AUTO]:Default time out reached, pattern: %s\t stopped", printpattern(pattern_stop, 1));
        syslog(LOG_DEBUG, "[LED]: [AUTO]:Stop pattern:%s", printpattern(pattern_stop, 1));
        shifu_led_resume_pattern();
    } else if (led_pattern_running == LED_MIXED) {
        shifu_led_process_mixed_pattern(pattern_stop, 0, 0);
        syslog(LOG_DEBUG, "[LED]: [AUTO]:Default time out reached, Forground pattern: %s\t stopped", printpattern(printmix_pattern, 1));
        syslog(LOG_DEBUG, "[LED]: [AUTO]:Stop pattern:%s", printpattern(printmix_pattern, 1));
    } else {
        syslog(LOG_DEBUG, "[LED]: Pattern stopped by client or high priority pattern running");
    }
    return 0;
}

/**
 * shifu_led_pattern_processing() - process the received pattern based on priority
 * @param[in] led_pattern	variable in which the bit has to reset
 * @param[in] led_start_stop	1=start led_pattern, 0=stop led_pattern
 * @param[in] value	No of leds to glow for volume led level
 * @return
 */
static void shifu_led_pattern_processing(unsigned int led_pattern, int led_start_stop, int value) {

    if (led_start_stop) {
        syslog(LOG_DEBUG, "[LED]: Starting requested pattern %s", printpattern(led_pattern, 1));
        led_pattern_running = led_pattern;
        queued_led_patterns = SET_BIT(queued_led_patterns, led_pattern);
        shifu_led_start_pattern(led_pattern, value);
    } else {
        system("i2cset -y 0 0x18 0x77 0x0 i");
        queued_led_patterns = CLR_BIT(queued_led_patterns, led_pattern);
        syslog(LOG_DEBUG, "[LED]: pattern:%s\t state:%s  stopped by client", printpattern(led_pattern, 1), printstate(led_start_stop));

        if (led_pattern_running == led_pattern)
            shifu_led_resume_pattern();
    }
}

/**
 * shifu_led_process_mixed_pattern() - Process the Mixed pattern requests
 * @param[in] led_pattern	variable in which the bit has to reset
 * @param[in] led_start_stop	1=start led_pattern, 0=stop led_pattern
 * @param[in] value	No of leds to glow for volume led level
 */
static void shifu_led_process_mixed_pattern(unsigned int led_pattern, int led_start_stop, int value) {

    if (led_start_stop) {
        if (led_pattern_running != LED_MIXED) {
            is_mixed_pattern = 1;
            if (led_pattern_running <= led_pattern) {
                led_pattern_backend = led_pattern;
                led_pattern_frontend = led_pattern_running;
            } else {
                led_pattern_backend = led_pattern_running;
                led_pattern_frontend = led_pattern;
            }
            led_pattern_running = LED_MIXED;
        }

        if (led_pattern_frontend < led_pattern) {
            led_pattern_backend = led_pattern;
            queued_led_patterns = SET_BIT(queued_led_patterns, led_pattern);
            system("i2cset -y 0 0x18 0x77 0x0 i");
            shifu_led_start_pattern(led_pattern_backend, value);
            system("i2cset -y 0 0x18 0x77 0x1 i");
            shifu_led_start_pattern(led_pattern_frontend, value);
        } else if (led_pattern_frontend == led_pattern) {
            if (is_mixed_pattern == 1) {
                is_mixed_pattern = 0;
                system("i2cset -y 0 0x18 0x77 0x0 i");
                system("i2cset -y 0 0x18 0x77 0x1 i");
            }
            shifu_led_start_pattern(led_pattern_frontend, value);
        } else {
            led_pattern_backend = led_pattern_frontend;
            led_pattern_frontend = led_pattern;
            queued_led_patterns = SET_BIT(queued_led_patterns, led_pattern);
            system("i2cset -y 0 0x18 0x77 0x0 i");
            shifu_led_start_pattern(led_pattern_backend, value);
            system("i2cset -y 0 0x18 0x77 0x1 i");
            shifu_led_start_pattern(led_pattern_frontend, value);
        }
    } else {
        if (led_pattern_frontend == led_pattern) {
            system("i2cset -y 0 0x18 0x77 0x0 i");
            led_pattern_running = led_pattern_backend;
            led_pattern_backend = 0;
            led_pattern_frontend = 0;
            queued_led_patterns = CLR_BIT(queued_led_patterns, led_pattern);
        } else if (led_pattern_backend == led_pattern) {
            system("i2cset -y 0 0x18 0x77 0x0 i");
            shifu_led_start_pattern(led_pattern_frontend, value);
            led_pattern_running = led_pattern_frontend;
            led_pattern_backend = 0;
            led_pattern_frontend = 0;
            queued_led_patterns = CLR_BIT(queued_led_patterns, led_pattern);
        } else {
            queued_led_patterns = CLR_BIT(queued_led_patterns, led_pattern);
        }
    }
}

/**
 * read_env_default_timeout_values() - reads the all env timeout variables
 * @param[ ]
 * @return	void
 */
static void read_env_default_timeout_values() {
    char temp_buf[5] = { 0 };

    if(get_shifu_uci_param("LED_VOLUME_TIMEOUT", temp_buf) > -1){
        volume_timeout = atoi(temp_buf);
    }

    if(get_shifu_uci_param("LED_VOICE_VOLUME_TIMEOUT", temp_buf) > -1){
        voice_volume_timeout = atoi(temp_buf);
    }

    if(get_shifu_uci_param("LED_MIC_MUTE_TIMEOUT", temp_buf) > -1){
        micmute_timeout = atoi(temp_buf);
    }

    if(get_shifu_uci_param("LED_ERROR_TIMEOUT", temp_buf) > -1){
        error_timeout = atoi(temp_buf);
    }

    if(get_shifu_uci_param("LED_ALARM_TIMEOUT", temp_buf) > -1){
        alarm_timeout = atoi(temp_buf);
    }

    if(get_shifu_uci_param("LED_DEVICE_CONNECTED_TIMEOUT", temp_buf) > -1){
        device_connected_timeout = atoi(temp_buf);
    }

    if(get_shifu_uci_param("LED_DEVICE_DISCOVERY_TIMEOUT", temp_buf) > -1){
        device_discovery_timeout = atoi(temp_buf);
    }

    if(get_shifu_uci_param("LED_QUERY_RESPONSE_TIMEOUT", temp_buf) > -1){
        query_response_timeout = atoi(temp_buf);
    }

    if(get_shifu_uci_param("LED_QUERY_PROCESSING_TIMEOUT", temp_buf) > -1){
        query_processing_timeout = atoi(temp_buf);
    }

    if(get_shifu_uci_param("LED_SETUP_MODE_TIMEOUT", temp_buf) > -1){
        setup_mode_timeout = atoi(temp_buf);
    }

    if(get_shifu_uci_param("LED_DEAFAULT_TIMEOUT", temp_buf) > -1){
        default_timeout = atoi(temp_buf);
    }

    if(get_shifu_uci_param("LED_BT_CONNECTED_TIMEOUT", temp_buf) > -1){
        bt_connected_timeout = atoi(temp_buf);
    }

    if(get_shifu_uci_param("LED_BT_DISCOVERY_TIMEOUT", temp_buf) > -1){
        bt_discovery_timeout = atoi(temp_buf);
    }

    if(get_shifu_uci_param("LED_MICMUTEOFF_TIMEOUT", temp_buf) > -1){
        micmute_off_timeout = atoi(temp_buf);
    }
}

void led_priority_decision(struct led_struct req_obj)
{

        pthread_mutex_lock(&decision_lock);

        /* Getting the Running and Requested pattern State */
        pattern_state = get_requested_pattern_state[get_state_value_for_pattern(req_obj.pattern)][get_state_value_for_pattern(led_pattern_running)];

        if (req_obj.pattern == LED_NO_INTERNET_CHECK_NOTI) {
            if(req_obj.state == 1) {
				pthread_mutex_lock(&internet_Lock);

				if(req_obj.value !=1)
				{
					mobile_notif = 1;

					/**
					*
					* [21961]
					* Broadcasting the signal to the thread to inform mobile notification on for internet status monitoring
					*
					*
					* */
					pthread_cond_broadcast(&internet_cond);
					/**
					* [BUG#22377]LED Pattern is observed When we turn ON/OFF Notifications button in Device settings
					*
					* check internet status when triggered from MA**/

					int rc;
					rc = func_To_Get_from_UCI(INTERNET_UCI_CMD, &internet_state);
					if(rc ==-1) {
					syslog(LOG_DEBUG, "[LED]: [AUTO]:reading from uci failed");
					}
					if(internet_state ==1){
					pattern_state = DIS;
					}
					else if(internet_state ==0){

					}
			   }
				pthread_mutex_unlock(&internet_Lock);
            } else {
				if(req_obj.value !=1)
				{
					mobile_notif = 0;
				}
            }
        }
		/**
		 * when Setup mode is on requested pattern LED_NO_INTERNET_CHECK_NOTIFY we will discard this pattern
		 *
		 * */

        if( (queued_led_patterns & LED_SETUP_MODE) && (req_obj.pattern == LED_NO_INTERNET_CHECK_NOTI) ) {
            pattern_state = DIS;
        }

        if( (queued_led_patterns & LED_ALARM) && (queued_led_patterns & LED_MIC_MUTE) && (req_obj.pattern == LED_NO_INTERNET_CHECK_NOTI) ) {
            pattern_state = DIS;
        }

        if (req_obj.pattern == LED_WAKEWORD && req_obj.state == 1) {
            queued_led_patterns = SET_BIT(queued_led_patterns, req_obj.pattern);
            system("aplay -f S16_LE -r 48000 -c 1 /wav_files/ful_ui_wakesound.wav &");
        }

        if (req_obj.pattern == LED_WAKEWORD && req_obj.state == 0) {
	        g_interrupt = 1;
            queued_led_patterns = CLR_BIT(queued_led_patterns, LED_WAKEWORD);
	        usleep(100 * 1000);
        }

        if (req_obj.pattern == LED_ALARM && req_obj.state == 1) {
            queued_led_patterns = SET_BIT(queued_led_patterns, req_obj.pattern);
        }

        if (req_obj.pattern == LED_MIC_MUTE && req_obj.state == 1) {
            queued_led_patterns = SET_BIT(queued_led_patterns, req_obj.pattern);
        }

        if (req_obj.pattern == LED_MIC_MUTE && req_obj.state == 0) {
            queued_led_patterns = CLR_BIT(queued_led_patterns, LED_BLUTOOTH_CONNECTED);
        }

        if (req_obj.pattern == LED_VOLUME) {
            volume_level = req_obj.value;
        }

        if (req_obj.pattern == LED_VOLUME && req_obj.state == 1) {
            queued_led_patterns = SET_BIT(queued_led_patterns, req_obj.pattern);
        }

        if (req_obj.pattern == LED_VOICE_VOLUME) {
            volume_level = req_obj.value;
        }

        if((queued_led_patterns & LED_ALARM) && (queued_led_patterns & LED_VOLUME)) {
              if((req_obj.pattern == LED_WAKEWORD)) {
                  shifu_led_process_mixed_pattern(LED_VOLUME, 0, 0);
                  queued_led_patterns = CLR_BIT(queued_led_patterns, LED_VOLUME);
                  led_pattern_running = LED_WAKEWORD;
              }
        }

        if( (led_pattern_running == LED_MIXED) && (queued_led_patterns & LED_ALARM) && (queued_led_patterns & LED_MIC_MUTE)) {
              if((req_obj.pattern == LED_QUERY_PROCESSING) || (req_obj.pattern == LED_QUERY_RESPONSE) || (req_obj.pattern == LED_WAKEWORD_OFF)) {
                  pattern_state = DIS;
              }
        }

        if((led_pattern_running == LED_MIXED) && (queued_led_patterns & LED_ALARM)) {
              if((req_obj.pattern == LED_QUERY_PROCESSING) || (req_obj.pattern == LED_QUERY_RESPONSE) || (req_obj.pattern == LED_WAKEWORD_OFF)) {
                  pattern_state = DIS;
              }
        }

        if( (queued_led_patterns & LED_WAKEWORD) && (queued_led_patterns & LED_MIC_MUTE) && (req_obj.pattern == LED_DND) && (req_obj.state == 1)) { 
              pattern_state = DIS;
        }

        if( (queued_led_patterns & LED_DEVICE_DISCOVERY) && (queued_led_patterns & LED_ALARM) && (req_obj.pattern == LED_MIC_MUTE) && (req_obj.state == 0)) { 
                queued_led_patterns = CLR_BIT(queued_led_patterns, LED_MIC_MUTE);
                queued_led_patterns = CLR_BIT(queued_led_patterns, LED_VOLUME);
                system("i2cset -y 0 0x18 0x41");
                system("i2cset -y 0 0x18 0x47 0x00 0x00 0x00 0x00 0x00 0x00 0xFD 0xE8 0x00 0x00 0x00 0x00 0x60 0x20 i");
                queued_led_patterns = SET_BIT(queued_led_patterns, LED_ALARM);
                queued_led_patterns = SET_BIT(queued_led_patterns, LED_DEVICE_DISCOVERY);
                led_pattern_running = LED_DEVICE_DISCOVERY;
                pattern_state = DIS;
        }

        if( (queued_led_patterns & LED_ALARM) && (queued_led_patterns & LED_MIC_MUTE)) { //Bug:21487

           if((req_obj.pattern == LED_DND)) {
               if(req_obj.state == 1) {
                    pattern_state = DIS;
               }
           }

           if((req_obj.pattern == LED_VOLUME)) {
                if(queued_led_patterns & LED_DEVICE_DISCOVERY) {
                    alarm_mic_mix_state = DEVICE_DISCOVERY_ON;
                    pattern_state = PAR;
                } else {
                    system("i2cset -y 0 0x18 0x77 0x0 i");
                    system("i2cset -y 0 0x18 0x41");
                    alarm_mic_mix_state = ALARM_MIC_ON;
                    pattern_state = PAR;
                }
           }

           if((req_obj.pattern == LED_DEVICE_DISCOVERY)) {
            if(req_obj.state) {
                system("i2cset -y 0 0x18 0x77 0x0 i");
                system("i2cset -y 0 0x18 0x41");
                system("i2cset -y 0 0x18 0x47 0x00 0x00 0x00 0x00 0x00 0x00 0xFD 0xE8 0x00 0x00 0x00 0x00 0x60 0x20 i");
                queued_led_patterns = SET_BIT(queued_led_patterns, LED_DEVICE_DISCOVERY);
                pattern_state = DIS;
            } else {
                system("i2cset -y 0 0x18 0x41");
                system("i2cset -y 0 0x18 0x77 0x0 i");
                queued_led_patterns = CLR_BIT(queued_led_patterns, LED_DEVICE_DISCOVERY);
                /* Triggering MicMute Pattern */
                system("i2cset -y 0 0x18 0x42 0x00 0x00 0x00 0x00 0xff 0xff 0xFF 0xF0 i");
                system("i2cset -y 0 0x18 0x77 0x1 i");
                /* Triggering Alarm Pattern */
                system("i2cset -y 0 0x18 0x78 0x23 i");
                pattern_state = DIS;
            }
         }

           if((req_obj.pattern == LED_BLUTOOTH_CONNECTED)) {
            if(req_obj.state) {
                system("i2cset -y 0 0x18 0x77 0x0 i");
                system("i2cset -y 0 0x18 0x41");
                system("i2cset -y 0 0x18 0x45 0xff 0xff 0x00 0x00 0x00 0x00 0x05 0x20 0xFF 0xF0 0x3 i");
                usleep(1000 * 500);
                system("i2cset -y 0 0x18 0x77 0x0 i");
                /* Triggering MicMute Pattern */
                system("i2cset -y 0 0x18 0x42 0x00 0x00 0x00 0x00 0xff 0xff 0xFF 0xF0 i");
                system("i2cset -y 0 0x18 0x77 0x1 i");
                /* Triggering Alarm Pattern */
                system("i2cset -y 0 0x18 0x78 0x23 i");
                pattern_state = DIS;
            }
          }

        if( (queued_led_patterns & LED_ALARM) && (queued_led_patterns & LED_WAKEWORD) && (req_obj.pattern == LED_MIC_MUTE)) { //Bug:22050
              queued_led_patterns = CLR_BIT(queued_led_patterns, LED_WAKEWORD);
              system("i2cset -y 0 0x18 0x77 0x0 i");
              system("i2cset -y 0 0x18 0x41");
              system("i2cset -y 0 0x18 0x78 0x23 i");
              led_pattern_running = LED_ALARM;
              shifu_led_process_mixed_pattern(LED_MIC_MUTE, 0, 0);
              pattern_state = MIX;
        }

           if((req_obj.pattern == LED_WAKEWORD)) {
            if(req_obj.state) {
                system("i2cset -y 0 0x18 0x77 0x0 i");
                system("i2cset -y 0 0x18 0x41");
                //system("i2cset -y 0 0x18 0x47 0x00 0x00 0x00 0x00 0x00 0x00 0xFD 0xE8 0x00 0x00 0x00 0x00 0x60 0x20 i");
                queued_led_patterns = SET_BIT(queued_led_patterns, LED_WAKEWORD);
                pattern_state = DIS;
            } else {
                system("i2cset -y 0 0x18 0x41");
                system("i2cset -y 0 0x18 0x77 0x0 i");
                queued_led_patterns = CLR_BIT(queued_led_patterns, LED_WAKEWORD);
                /* Triggering MicMute Pattern */
                system("i2cset -y 0 0x18 0x42 0x00 0x00 0x00 0x00 0xff 0xff 0xFF 0xF0 i");
                system("i2cset -y 0 0x18 0x77 0x1 i");
                /* Triggering Alarm Pattern */
                system("i2cset -y 0 0x18 0x78 0x23 i");
                pattern_state = DIS;
            }
         }

           if((req_obj.pattern == LED_QUERY_PROCESSING)) {
            if(req_obj.state) {
                system("i2cset -y 0 0x18 0x77 0x0 i");
                system("i2cset -y 0 0x18 0x41");
            system("i2cset -y 0 0x18 0x47 0xFD 0xE8 0x00 0x00 0x00 0x00 0x78 0x00 0xff 0x00 0x00 0x00 0x60 0x04 i");
                queued_led_patterns = SET_BIT(queued_led_patterns, LED_QUERY_PROCESSING);
                pattern_state = DIS;
            } else {
                system("i2cset -y 0 0x18 0x41");
                system("i2cset -y 0 0x18 0x77 0x0 i");
                queued_led_patterns = CLR_BIT(queued_led_patterns, LED_QUERY_PROCESSING);
                /* Triggering MicMute Pattern */
                system("i2cset -y 0 0x18 0x42 0x00 0x00 0x00 0x00 0xff 0xff 0xFF 0xF0 i");
                system("i2cset -y 0 0x18 0x77 0x1 i");
                /* Triggering Alarm Pattern */
                system("i2cset -y 0 0x18 0x78 0x23 i");
                pattern_state = DIS;
            }
         }

           if((req_obj.pattern == LED_QUERY_RESPONSE)) {
            if(req_obj.state) {
                system("i2cset -y 0 0x18 0x77 0x0 i");
                system("i2cset -y 0 0x18 0x41");
                system("i2cset -y 0 0x18 0x70 0xFD 0xE8 0x00 0x00 0x00 0x00 0x78 0x00 0xff 0x00 0x00 0x00 0x08 0x46 i");
                queued_led_patterns = SET_BIT(queued_led_patterns, LED_QUERY_RESPONSE);
                pattern_state = DIS;
            } else {
                system("i2cset -y 0 0x18 0x41");
                system("i2cset -y 0 0x18 0x77 0x0 i");
                queued_led_patterns = CLR_BIT(queued_led_patterns, LED_QUERY_RESPONSE);
                /* Triggering MicMute Pattern */
                system("i2cset -y 0 0x18 0x42 0x00 0x00 0x00 0x00 0xff 0xff 0xFF 0xF0 i");
                system("i2cset -y 0 0x18 0x77 0x1 i");
                /* Triggering Alarm Pattern */
                system("i2cset -y 0 0x18 0x78 0x23 i");
                pattern_state = DIS;
            }
         }
      }

        if( (queued_led_patterns & LED_ALARM) && (queued_led_patterns & LED_VOLUME) && (req_obj.pattern == LED_MIC_MUTE) ) { //Bug:22035

           if((req_obj.state == 1)) {
              shifu_led_process_mixed_pattern(LED_VOLUME, 0, 0);
              queued_led_patterns = SET_BIT(queued_led_patterns, LED_MIC_MUTE);
              led_pattern_running = LED_ALARM;
              pattern_state = get_requested_pattern_state[get_state_value_for_pattern(req_obj.pattern)][get_state_value_for_pattern(led_pattern_running)];
           } else {
              queued_led_patterns = CLR_BIT(queued_led_patterns, LED_VOLUME);
              queued_led_patterns = CLR_BIT(queued_led_patterns, LED_MIC_MUTE);
              shifu_led_process_mixed_pattern(LED_MIC_MUTE, 0, 0);
              system("i2cset -y 0 0x18 0x77 0x0 i");
              system("i2cset -y 0 0x18 0x41");
              system("i2cset -y 0 0x18 0x78 0x23 i");
              led_pattern_running = LED_ALARM;
              alarm_mic_mix_state = ALARM_MIC_OFF;
              pattern_state = DIS;
           }

        }

        if( (queued_led_patterns & LED_ALARM) && (queued_led_patterns & LED_MIC_MUTE) && (req_obj.pattern == LED_ALARM) && (req_obj.state == 0)) { //Bug #22290
            if((queued_led_patterns & LED_DEVICE_DISCOVERY)) {
               alarm_mic_mix_state = ALARM_MIC_OFF;
               queued_led_patterns = CLR_BIT(queued_led_patterns, LED_VOLUME);
               queued_led_patterns = CLR_BIT(queued_led_patterns, LED_ALARM);
               queued_led_patterns = SET_BIT(queued_led_patterns, LED_MIC_MUTE);
               shifu_led_process_mixed_pattern(LED_VOLUME, 0, 0);
               system("i2cset -y 0 0x18 0x77 0x0 i");
               system("i2cset -y 0 0x18 0x41");
               shifu_led_pattern_processing(LED_MIC_MUTE, 1, 0);
               shifu_led_pattern_processing(LED_DEVICE_DISCOVERY, 1, 0);
               led_pattern_running = LED_MIC_MUTE;
               pattern_state = DIS;
          } else {
               time_set(timer_fd, 0);
               alarm_mic_mix_state = ALARM_MIC_OFF;
               queued_led_patterns = CLR_BIT(queued_led_patterns, LED_VOLUME);
               queued_led_patterns = CLR_BIT(queued_led_patterns, LED_ALARM);
               queued_led_patterns = SET_BIT(queued_led_patterns, LED_MIC_MUTE);
               shifu_led_process_mixed_pattern(LED_VOLUME, 0, 0);
               system("i2cset -y 0 0x18 0x77 0x0 i");
               system("i2cset -y 0 0x18 0x41");
               shifu_led_pattern_processing(LED_MIC_MUTE, 1, 0);
               led_pattern_running = LED_MIC_MUTE;
               pattern_state = DIS;
          } 
        }

        if (led_pattern_running == LED_WAKEWORD && req_obj.pattern == LED_VOLUME) {
            g_interrupt = 1;
            usleep(100 * 1000);
        }
        if (led_pattern_running == LED_SETUP_MODE && req_obj.pattern == LED_MIC_MUTE && req_obj.value == 0 ) {
            queued_led_patterns = CLR_BIT(queued_led_patterns, req_obj.pattern);
        }

        if ((queued_led_patterns & LED_SETUP_MODE) && (queued_led_patterns & LED_MIC_MUTE)) {
            if(req_obj.pattern == LED_VOLUME) {
                pattern_state = MIX;
            }
        }

        if ((queued_led_patterns & LED_SETUP_MODE) && (queued_led_patterns & LED_VOLUME)) { //Bug ID: 22068
            if(req_obj.pattern == LED_MIC_MUTE) {
                pattern_state = DIS;
                if(req_obj.state)
                {
                    queued_led_patterns = SET_BIT(queued_led_patterns, req_obj.pattern);
                }
                else
                {
                    queued_led_patterns = CLR_BIT(queued_led_patterns, req_obj.pattern);
                }
            }

            if(req_obj.pattern == LED_ANNOUNCEMENT) {
                time_set(timer_fd, 0);
                queued_led_patterns = CLR_BIT(queued_led_patterns, LED_VOLUME);
                shifu_led_process_mixed_pattern(LED_VOLUME, 0, 0);
                system("i2cset -y 0 0x18 0x77 0x0 i");
                system("i2cset -y 0 0x18 0x41");
                shifu_led_pattern_processing(LED_SETUP_MODE, 1, 0);
                led_pattern_running = LED_SETUP_MODE;
                pattern_state = PAR;
            }

        }

        if (req_obj.pattern == LED_SETUP_MODE && req_obj.state == 0) {
            if (!get_shifu_uci_param("DEVICE_REGISTRATION_STATUS", device_registration_status) && !get_shifu_uci_param("ALEXA_REGISTRATION_STATUS", alexa_registration_status) && !get_shifu_uci_param("SKIP_ALEXA_REGISTRATION", skip_alexa_registration)) {
                if ((!strcmp(alexa_registration_status, DEVICE_REGISTERED) || !strcmp(skip_alexa_registration, "true")) && !strcmp(device_registration_status, DEVICE_REGISTERED)) {
                    if(!strcmp(skip_alexa_registration, "true")) {
                    	/* #22366: setting volume to system level */
                    	SetPlayerVolume();
                        system("i2cset -y 0 0x18 0x41 && gst-launch-1.0 playbin uri=file:///announcements/skip_alexa.mp3 audio-sink=alsasink &");
                    } else if(!strcmp(alexa_registration_status, DEVICE_REGISTERED)) {
                    	/* #22366: setting volume to system level */
                    	SetPlayerVolume();
                        system("i2cset -y 0 0x18 0x41 && gst-launch-1.0 playbin uri=file:///announcements/en_in_avs_system_prompt_your_alexa_device_is_ready.mp3 audio-sink=alsasink &");
                    }
                    queued_led_patterns = CLR_BIT(queued_led_patterns, req_obj.pattern);
                    pthread_mutex_lock(&internet_Lock);
                    /**
					*
					* [21961]
					* Broadcasting the setup pattern off status to internet monitoring thread.
					* Alexa registation/skip will come here to make setup pattern off this we are using.			*
					*
					* */

                    set_up_pattern_off = 1;
                    pthread_cond_broadcast(&internet_cond);
					pthread_mutex_unlock(&internet_Lock);
                    shifu_led_resume_pattern();
                }
            }

        } else {
         switch(pattern_state)
         {
           case DIS:
           {
                syslog(LOG_DEBUG, "[LED]: [AUTO]:Running pattern: %s, Requested pattern: %s State:[DIS: DISCARD]", printpattern(led_pattern_running, 1), printpattern(req_obj.pattern, 1));
                syslog(LOG_DEBUG, "[LED]: [AUTO]:Discarding the Requested pattern:%s ", printpattern(req_obj.pattern, 1));
                break;
           }
           case PAT:
           {
                syslog(LOG_DEBUG, "[LED]: [AUTO]:Running pattern: %s,  Requested pattern: %s State:[PAT: Play After Timeout]", printpattern(led_pattern_running, 1), printpattern(req_obj.pattern, 1));
                syslog(LOG_DEBUG, "[LED]: [AUTO]:Start pattern:%s Queue pattern:%s", printpattern(led_pattern_running, 1), printpattern(req_obj.pattern, 1));
                if (req_obj.state) {
                    syslog(LOG_DEBUG, "[LED]: Pattern: %s sent to queue", printpattern(req_obj.pattern, 1));
                    queued_led_patterns = SET_BIT(queued_led_patterns, req_obj.pattern);
                } else {
                    syslog(LOG_DEBUG, "[LED]: Pattern: %s cleared in queue, if exist", printpattern(req_obj.pattern, 1));
                    syslog(LOG_DEBUG, "[LED]: [AUTO]: Stop Pattern: %s", printpattern(req_obj.pattern, req_obj.state));
                    queued_led_patterns = CLR_BIT(queued_led_patterns, req_obj.pattern);
                }
                break;
           }
           case PI:
           {
               if (led_pattern_running == req_obj.pattern) {
                   syslog(LOG_DEBUG, "[LED]: [AUTO]: Running pattern: %s,  Requested pattern: %s  State:[PI: PLAY IMMEDIATELY]", printpattern(led_pattern_running, 1), printpattern(req_obj.pattern, req_obj.state));
               } else {
                   syslog(LOG_DEBUG, "[LED]: [AUTO]: Running pattern: %s,  Requested pattern: %s  State:[PI: PLAY IMMEDIATELY]", printpattern(led_pattern_running, 1), printpattern(req_obj.pattern, 1));
               }
                if (led_pattern_running == req_obj.pattern) {
                    syslog(LOG_DEBUG, "[LED]: [AUTO]:Start pattern:%s  Stop pattern:%s", printpattern(req_obj.pattern, req_obj.state), printpattern(led_pattern_running, 1));
                } else {
                    syslog(LOG_DEBUG, "[LED]: [AUTO]:Start pattern:%s  Stop pattern:%s", printpattern(req_obj.pattern, 1), printpattern(led_pattern_running, 1));
                }
                queued_led_patterns = CLR_BIT(queued_led_patterns, led_pattern_running);
                shifu_led_pattern_processing(req_obj.pattern, req_obj.state, req_obj.value);
                break;
           }
           case PAR:
           {
                syslog(LOG_DEBUG, "[LED]: [AUTO]: Running pattern: %s,  Requested pattern: %s  State:[PAR: Play and Ressume]", printpattern(led_pattern_running, 1), printpattern(req_obj.pattern, req_obj.state));
                syslog(LOG_DEBUG, "[LED]: [AUTO]: Start pattern:%s  Queue pattern:%s", printpattern(req_obj.pattern, req_obj.state), printpattern(led_pattern_running, 1));
               	shifu_led_pattern_processing(req_obj.pattern, req_obj.state, req_obj.value);
                break;
           }
           case MIX:
           {
                syslog(LOG_DEBUG, "[LED]: [AUTO]: Running pattern: %s,  Requested pattern: %s  state:[MIX of patterns]", printpattern(led_pattern_running, 1), printpattern(req_obj.pattern, req_obj.state));
                syslog(LOG_DEBUG, "[LED]: Requested pattern: %s will overlap on Running pattern: %s  state  is MIX", printpattern(req_obj.pattern, 1), printpattern(led_pattern_running, 1));
                syslog(LOG_DEBUG, "[LED]: [AUTO]: Forground pattern: %s Backgroud pattern: %s  State:MIX", printpattern(req_obj.pattern, 1), printpattern(led_pattern_running, 1));
                printmix_pattern = req_obj.pattern;
                shifu_led_process_mixed_pattern(req_obj.pattern, req_obj.state, req_obj.value);
                break;
           }
           default:
           {
                syslog(LOG_DEBUG, "[LED]: Received pattern:%s\tstate:%d\tvalue:%d  is INVALID", printpattern(req_obj.pattern, 1), printstate(req_obj.state), req_obj.value);
                break;
           }
         }
      }
      pthread_mutex_unlock(&decision_lock);
}

// #22267 Start: Synchronization between LED server & AVS after Booting of Shifu
/**
 * avs_config_done() - This function will turn off Boot pattern and glow SET_MODE & MUTE LED pattern if required
 * @param[ ]
 * @return	void
 */

void avs_config_done() {

    int ret;
	int mic_mute_on = 0;
	syslog(LOG_INFO, "[LED]: AVS Request for Configuration Completion");

	// #22374 Start: Multi-voice output is observed after successful flashing of firmware
	pthread_mutex_lock(&avs_config_lock);

	if(avs_config_state == CONFIG_DONE){
		pthread_mutex_unlock(&avs_config_lock);
		syslog(LOG_INFO, "[LED]: AVS Configuration Done Already");
		return;
	}
	// #22374 End.

	ret = get_shifu_uci_param("DEVICE_REGISTRATION_STATUS", device_registration_status);
	ret &= get_shifu_uci_param("ALEXA_REGISTRATION_STATUS", alexa_registration_status);
	ret &= get_shifu_uci_param("SKIP_ALEXA_REGISTRATION", skip_alexa_registration);

	if (!ret) {
		if (strcmp(device_registration_status, DEVICE_REGISTERED)) {
			syslog(LOG_DEBUG, "[LED]: Device not registered, entering into Setup mode");
			//system("touch /tmp/volume.lock && amixer -c0 sset DAC1 65");
			//system("sh /announcements/setannouncementvolume.sh");
			/* #22366: setting volume to system level */
        	SetPlayerVolume();
			system("gst-launch-1.0 playbin uri=file:///announcements/en_in_avs_system_oobe_setup.mp3 audio-sink=alsasink && rm /tmp/volume.lock &");
			shifu_led_pattern_processing(LED_SETUP_MODE, 1, 0);
			set_up_pattern_off =0;
		} else {
			syslog(LOG_DEBUG, "[LED]: Device registered");
			/**
			* [21961]
			* Broadcasting the setup pattern off status to internet monitoring thread.
			* if already we registered we never start setup pattern thats why default we are making set_up_pattern_off=1.			*
			* */
			pthread_mutex_lock(&internet_Lock);
			set_up_pattern_off = 1;
			pthread_cond_broadcast(&internet_cond);
			pthread_mutex_unlock(&internet_Lock);
			system("i2cset -y 0 0x18 0x41");

		}
	} else {
		syslog(LOG_DEBUG, "[LED]: get_shifu_uci_param() function failed to get registration_status");
	}
	mic_mute_on = mic_init();
	if(mic_mute_on == 1) {
		syslog(LOG_DEBUG, "[LED]: MICMUTE STATUS TRUE");
		shifu_led_pattern_processing(LED_MIC_MUTE, 1, 0);
		syslog(LOG_INFO, "[LED]: Executing:LED_MIC_MUTE after AVS configured");
	}
	avs_config_state = CONFIG_DONE;
	// #22374 Start: Multi-voice output is observed after successful flashing of firmware
	pthread_mutex_unlock(&avs_config_lock);
	// #22374 End.
	syslog(LOG_INFO, "[LED]: AVS Configuration Done");
}


/**
 * wait_for_avs_config() - This thread will wait for REBOOT_PAT_TIMEOUT if AVS not configured
 * @param[ ]
 * @return	void
 */
void* wait_for_avs_config(void *var) {
    int rebootPatternTimeout = REBOOT_PAT_TIMEOUT;
    /**
    int SampleAppPID = -1;
    FILE *fp = NULL;
    char buf[AVS_PID_BUF_SIZE] = {'\0'};
	*/
	pthread_detach(pthread_self());
	/**
    fp = popen("ps | grep 'SampleApp' | grep -v 'grep' | awk '{ printf $1 }'","r");
    if (fp) {
		if(fgets(buf,AVS_PID_BUF_SIZE,fp)!=NULL)
            SampleAppPID = atoi(buf);
        pclose(fp);
        if(SampleAppPID > 0);
            //rebootPatternTimeout = 0;
    }
    */

	while(rebootPatternTimeout != 0){
		syslog(LOG_DEBUG, "[LED]: rebootPatternTimeout :%d",rebootPatternTimeout);
		if(avs_config_state == CONFIG_DONE){
			syslog(LOG_DEBUG, "[LED]: Reset rebootPatternTimeout counter");
			break;
		}
		else
			sleep(1);
		rebootPatternTimeout--;
	}

	if(rebootPatternTimeout == 0)
		avs_config_done();

}
// #22267 End.

/**
 * main()- Program starts from here
 * @param[ ]
 * @return returns an integer
 */
int main() {
    int ret;
    int udpSocket;
    int nBytes;
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    socklen_t client_addr_size;
    struct led_struct req_obj;

    /* Timer fd creation for the timeout a pattern */
    timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);
    if ( timer_fd == -1) {
        syslog(LOG_ERR, "[LED]: timerfd_create() failed: errno=%d", errno);
    }
    syslog(LOG_INFO, "[LED]: created timer_fd %d", timer_fd);
    poll_list[0].fd = timer_fd;
    poll_list[0].events = POLLIN;

    /* Creation of Thread function which is used for poll the Timer fd */
    pthread_create( &thread_id, NULL, (void * ) myThreadFun, NULL);

    /* Create UDP socket */
    udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
    bind(udpSocket, (struct sockaddr *) & serverAddr, sizeof(serverAddr));
    addr_size = sizeof(serverStorage);


    volume_init();
    syslog(LOG_DEBUG, "[LED]: Reading ENV variables");


    /* reading NV default timeout varibles */
    read_env_default_timeout_values();

    if(pthread_create(&(avs_config), NULL, wait_for_avs_config, NULL)!=0)
    {
		syslog(LOG_ERR, "[LED]: pthread_create wait_for_avs_config failed ");
		exit(0);
	}

    /**
     * 21961:creating thread for internet check monitor continuously.
     * **/
    if(pthread_create(&(led_internet_check), NULL, no_Internet_Check_Monitor, NULL)!=0)
    {
		syslog(LOG_ERR, "[LED]: pthread_create internet check failed ");
		exit(0);
	}

    while (1) {
        memset( &req_obj, 0, sizeof(req_obj));
        syslog(LOG_DEBUG, "[LED]: Waiting for request from clients\n");

        /* Wait for the led-pattern request */
        nBytes = recvfrom(udpSocket, &req_obj, sizeof(req_obj), 0, (struct sockaddr * ) & serverStorage, & addr_size);

        if ( nBytes == -1 ) {
            syslog(LOG_ERR, "[LED]: Socket recieve from failed");
            continue;
        }

        syslog(LOG_DEBUG, "[LED]: [AUTO]:Received pattern:%s\tstate:%s\tvalue:%d", printpattern(req_obj.pattern, req_obj.state), printstate(req_obj.state), req_obj.value);

        // #22267 Start: Synchronization between LED server & AVS after Booting of Shifu

        if ( (req_obj.pattern != LED_AVS_CONFIG) && (avs_config_state == CONFIG_DONE) ){
            led_priority_decision(req_obj);
        }
        else if( ((req_obj.pattern == LED_AVS_CONFIG)) && (avs_config_state == WAIT_FOR_CONFIG) ){
            avs_config_done();
        }
        // #22267 End.
    }
    pthread_join(thread_id, NULL);
    pthread_join(led_listen_start,NULL);
    pthread_join(led_internet_check,NULL);
    return 0;
}




