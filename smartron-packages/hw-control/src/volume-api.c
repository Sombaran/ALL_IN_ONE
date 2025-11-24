/*
 * File: volume-api.c
 * Copyright (C) 2018 Smartron India Private Limited - All Rights Reserved
 * AUTHOR :  Surya Teja
 *
 * Notes
 * -----
 * This application sets volume and led level according to the user
 * request for shifu, shifu connected as blutooth sets volume and controls led levels.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <sys/syslog.h>
#include "volume-api.h"

// Redmine ID: 21427 Changed the shifu alexa volume db levels 
/* stores Shifu volume levels from 1 to 10 array in db */
char volume_table_shifu[]  = {0, 44, 47, 50, 53, 56, 59, 62, 65, 68, 71};

/* stores Shifu blutooth volume levels from 1 to 10 array in db */
char volume_table_ext_BT[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};


/**
 * get_vol_db_with_index() - gives thechar volume_table_shifu[]  = {0,25,32,38,46,50,56,60,64,68,71}; present volume
 * @param[ ]
 * @return   present volume value
 */
int get_vol_db_with_index(int index) {
	return volume_table_shifu[index];
}

/**
 * get_vol_index_with_db() - gives thechar volume_table_shifu[]  = {0,25,32,38,46,50,56,60,64,68,71}; present volume
 * @param[ ]
 * @return   present volume value
 */
int get_vol_index_with_db(int db) {
	for(int index=0; index < TOTAL_LED_NUM; index++) {
		if(db == volume_table_shifu[index]) {
		    return index;
		}
	}
}

/**
 * get_volume() - gives thechar volume_table_shifu[]  = {0,25,32,38,46,50,56,60,64,68,71}; present volume
 * @param[ ]
 * @return   present volume value
 */
int get_volume(void) {
    int i = 0;
    FILE *fd = NULL;
    int volume = 0;
    char vol[4] = {0} ;

    if(BT_sink_connected == BT_SINK_CONNECTED) {
        fd = popen(CMD_GET_VOLUME_BT_SINK, "r");
    } else {
        fd = popen(CMD_GET_VOLUME_SHIFU, "r");
    }

    fgets(vol, 4, fd);
    pclose(fd);
    volume = atoi(vol);

    for(i=0; i < TOTAL_LED_NUM; i++) {
        if(BT_sink_connected == BT_SINK_CONNECTED) {
            if((volume == volume_table_ext_BT[i] || volume < volume_table_ext_BT[i+1])) {
                break;
            }
        } else {
            if((volume == volume_table_shifu[i] || volume < volume_table_shifu[i+1])) {
                break;
            }
        }
    }
    return i;
}

/**
 * set_volume_platform() - set the shifu/externel BT speaker volume based on "BT_sink_connected" status
 * @param[in] volume_table_index	index of the volume table to set perticular volume
 * @return     void
 */
void set_volume_platform(int volume_table_index) {
    char pactl_cmd[64] = {0};
    char amixer_cmd[32] = {0};

    if(BT_sink_connected == BT_SINK_CONNECTED) {
        snprintf(pactl_cmd, sizeof(pactl_cmd), CMD_SET_VOLUME_BT_SINK, BLUTOOTH_CARD, volume_table_ext_BT[volume_table_index]);
        system(pactl_cmd);
    } else {
        snprintf(amixer_cmd, sizeof(amixer_cmd), CMD_SET_VOLUME_SHIFU, volume_table_shifu[volume_table_index]);
        system(amixer_cmd);
    }
}

/**
 * set_volume() - controls the volume
 * @param[in] mode  represents mode of operation VOLUME_XXX
 * @param[in] level  volume level/index to volume table (0 to TOTAL_LED_NUM)
 * @return int  no of led's to glow for the current volume
 */
int set_volume(int mode, int level) {
    int present_volume = 0;
    int led_level = 0;
    int  i = 0;
    BT_sink_connected = get_shifu_bt_status();
    present_volume = get_volume();

    switch(mode)
    {
        case VOLUME_SET:
                       {
                           if(level <= 0) {
                               led_level = 0;
                               level = 0;
                               set_volume_platform(level);
                           } else if(level >= TOTAL_LED_NUM) {
                               led_level = TOTAL_LED_NUM;
                               level = TOTAL_LED_NUM;
                               set_volume_platform(level);
                           } else {
                               led_level = level;
                               set_volume_platform(level);
                           }
                           break;
                       }

         case VOLUME_UP:
                       {
                           level = present_volume + level;

                           if(level <= 0) {
                               led_level = present_volume;
                               level = present_volume;
                               set_volume_platform(present_volume);
                           } else if(level >= TOTAL_LED_NUM) {
                               led_level = TOTAL_LED_NUM;
                               level = TOTAL_LED_NUM;
                               set_volume_platform(level);
                           } else {
                               led_level = level;
                               set_volume_platform(level);
                           }
                           send_to_avs(VOLUME_UP);
                           break;
                       }

         case VOLUME_DOWN:
                         {
                             level = present_volume - level;

                             if(level <= 0) {
                                 led_level = 0;
    				             level = 0;
                                 set_volume_platform(level);
                             }
                             else if(level >= TOTAL_LED_NUM) {
                                 led_level = TOTAL_LED_NUM;
                                 level = TOTAL_LED_NUM;
                                 set_volume_platform(level);
                             } else {
                                 led_level = level;
                                 set_volume_platform(level);
                             }
                             send_to_avs(VOLUME_DOWN);
                             break;
                         }
         default:
                {
                    break;
                }
    }

    return led_level;
}


