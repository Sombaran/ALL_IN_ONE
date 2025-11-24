/*
 * File: status.c
 * Copyright (C) 2018 Smartron India Private Limited - All Rights Reserved
 * AUTHOR :  Janardhanachari Kella
 * Notes
 * -----
 * This file containing APIs to initalizes the hardware at hw-contrl module startup.
 */

#include <stdio.h>
#include <shifu-led.h>
#include <sys/syslog.h>
#include <fw-env.h>
#include "status.h"

hub_status_t hub_status;

// Redmine ID: 21427 Changed the shifu alexa volume db levels 
/* stores Shifu volume levels from 1 to 10 array in db */
char volume_table_shifu[]  = {0, 44, 47, 50, 53, 56, 59, 62, 65, 68, 71};

/**
 * hub_state_to_default() - initializes the hub volume, mute, led configurations to default. if it found the config parameters are missed, loads default value.
 * @param[in] - void
 */
static void hub_state_to_default() {

    char buf[CHAR_LEN];

    hub_status.led_level = DEFAULT_ALSA_ALSA_LED;
    hub_status.speaker_volume_level = DEFAULT_ALSA_ALSA_SPKVOL;
    hub_status.mic_state = DEFAULT_ALSA_ALSA_MICMUTE;
    hub_status.speaker_state = DEFAULT_ALSA_ALSA_SPKMUTE;

    if(get_val_frm_conf(OPT_KEY_ALSA_ALSA_LED, buf) < 0) {
        syslog(LOG_ERR, "[BTN_CTRL]:led_level Config not available, setting as default!");
        if(set_val_to_conf(OPT_KEY_ALSA_ALSA_LED, hub_status.led_level) < 0)
            syslog(LOG_ERR, "[BTN_CTRL]:led_level Configuration faild");
    }

    if(get_val_frm_conf(OPT_KEY_ALSA_ALSA_SPKVOL, buf) < 0) {
        syslog(LOG_ERR, "[BTN_CTRL]:speaker volume level Config not available, setting as default!");
        if(set_val_to_conf(OPT_KEY_ALSA_ALSA_SPKVOL, hub_status.speaker_volume_level) < 0)
            syslog(LOG_ERR, "[BTN_CTRL]:speaker volume level Configuration faild");
    }

    if(get_val_frm_conf(OPT_KEY_ALSA_ALSA_MICMUTE, buf) < 0) {
        syslog(LOG_ERR, "[BTN_CTRL]:mic mute Config not available, setting as default!");
        if(set_val_to_conf(OPT_KEY_ALSA_ALSA_MICMUTE, hub_status.mic_state) < 0)
            syslog(LOG_ERR, "[BTN_CTRL]:speaker volume level Configuration faild");
    }

    if(get_val_frm_conf(OPT_KEY_ALSA_ALSA_SPKMUTE, buf) < 0) {
        syslog(LOG_ERR, "[BTN_CTRL]:speaker mute Config not available, setting as default!");
        if(set_val_to_conf(OPT_KEY_ALSA_ALSA_SPKMUTE, hub_status.speaker_state) < 0)
            syslog(LOG_ERR, "[BTN_CTRL]:speaker volume level Configuration faild");
    }
}

/**
 * get_hub_configuration() - Loads hub status object with the reserved values
 * @param[in] - void
 */
static void get_hub_configuration() {
    char buf[CHAR_LEN];

    if(get_val_frm_conf(OPT_KEY_ALSA_ALSA_LED, buf) < 0) {
        syslog(LOG_ERR, "[BTN_CTRL]:led_level Config not available.");
    }
    hub_status.led_level = atoi(buf);
    memset(buf, 0, sizeof(buf));

    if(get_val_frm_conf(OPT_KEY_ALSA_ALSA_SPKVOL, buf) < 0) {
        syslog(LOG_ERR, "[BTN_CTRL]:speaker volume level Config not available.");
    }
    hub_status.speaker_volume_level = atoi(buf);
    memset(buf, 0, sizeof(buf));

    if(get_val_frm_conf(OPT_KEY_ALSA_ALSA_MICMUTE, buf) < 0) {
        syslog(LOG_ERR, "[BTN_CTRL]:mic mute Config not available.");
    }
    hub_status.mic_state = atoi(buf);
    memset(buf, 0, sizeof(buf));

    if(get_val_frm_conf(OPT_KEY_ALSA_ALSA_SPKMUTE, buf) < 0) {
        syslog(LOG_ERR, "[BTN_CTRL]:speaker mute Config not available.");
    }
    hub_status.speaker_state = atoi(buf);
}

/**
 * mic_mute() - Mutes the hub mic
 * @param[in] - void
 */
void mic_mute() {

    system(MIC_MUTE_ON);

}

/**
 * mic_unmute() - Unmutes the hub mic
 * @param[in] - void
 */
void mic_unmute() {

    system(MIC_MUTE_OFF);
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
 * set_volume_platform() - set the shifu volumelevel
 * @param[in] volume_table_index	index of the volume table to set perticular volume
 * @return     void
 */
void set_volume_platform(int volume_table_index) {
    char amixer_cmd[32] = {0};
    snprintf(amixer_cmd, sizeof(amixer_cmd), CMD_SET_VOLUME_SHIFU, volume_table_shifu[volume_table_index]);
    system(amixer_cmd);
}

/**
 * hw_init() - Restores volume, and mic mute statuses
 * @param[in] - fd	        pattern fd's
 * @param[in] - itimerspec * time		timer sturcture
 * @param[in] - timeout	pattern-timeout
 */
void volume_init() {

    hub_state_to_default();

    syslog(LOG_DEBUG, "[LED]: Default Volume: %d Mute: %d", hub_status.speaker_volume_level, hub_status.mic_state);

    get_hub_configuration();

    syslog(LOG_DEBUG, "[LED]: Restoring HUB Status back, Volume: %d Mute: %d", hub_status.speaker_volume_level, hub_status.mic_state);

    set_volume_platform(get_vol_index_with_db(hub_status.speaker_volume_level));

}


int mic_init() {
    if(hub_status.mic_state) {
        mic_mute();
        return 1;
    }
    else {
        mic_unmute();
        return 0;
    }
}

