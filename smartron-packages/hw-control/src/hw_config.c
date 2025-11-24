/*
 * File: hw_config.c
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
#include "btn-control.h"
#include "hw_config.h"

hub_status_t hub_status;

extern int led_fd;

struct led_struct led_structure;


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

    memset(&led_structure, 0, sizeof(struct led_struct));

    led_structure.pattern = LED_MIC_MUTE;
    led_structure.state = 1;
    led_structure.value = 0;

    if(led_fd >= 0)
        shifu_led_pattern_send(led_fd, &led_structure);
}

/**
 * mic_unmute() - Unmutes the hub mic
 * @param[in] - void
 */
void mic_unmute() {

    system(MIC_MUTE_OFF);

    memset(&led_structure, 0, sizeof(struct led_struct));

    led_structure.pattern = LED_MIC_MUTE;
    led_structure.state = 0;
    led_structure.value = 0;
    if(led_fd >= 0)
        shifu_led_pattern_send(led_fd, &led_structure);
}

/**
 * get_mic_mute_status() - Loads mute state form reserved values to hub status object
 * @param[in] - void
 */
int get_mic_mute_status() {
    char buf[CHAR_LEN];
    memset(buf, 0, sizeof(buf));

    if(get_val_frm_conf(OPT_KEY_ALSA_ALSA_MICMUTE, buf) < 0) {
        syslog(LOG_ERR, "[BTN_CTRL]:mic mute Config not available.");
        return;
    }

    hub_status.mic_state = atoi(buf);
    return hub_status.mic_state;
}

/**
 * set_mic_mute_status() - Loads mute state form hub status object to alsa config for reserve value
 * @param[in] - state		toggled state of mute button
 */
void set_mic_mute_status(int state) {

    hub_status.mic_state = state;

    if(set_val_to_conf(OPT_KEY_ALSA_ALSA_MICMUTE, hub_status.mic_state) < 0) {
        syslog(LOG_ERR, "[BTN_CTRL]:speaker volume level Configuration faild");
        return;
    }
    syslog(LOG_DEBUG, "[BTN-CTRL]: setting HUB Mute to %d", hub_status.mic_state);
}

/**
 * send_mute_state() - toggles state of mic UX, when button pressed
 * @param[in] - state		toggled state of mute button
 */
void send_mute_state(int mic_mute_status) {
    if(mic_mute_status) {
        mic_mute();
        send_to_avs(MIC_MUTE);
        system("aplay -f S16_LE -r 48000 /wav_files/ful_state_privacy_mode_on.wav");
    }
    else {
        mic_unmute();
        send_to_avs(MIC_UNMUTE);
        system("aplay -f S16_LE -r 48000 /wav_files/ful_state_privacy_mode_off.wav");
    }
}

/**
 * hw_init() - Restores volume, and mic mute statuses
 * @param[in] - fd	        pattern fd's
 * @param[in] - itimerspec * time		timer sturcture
 * @param[in] - timeout	pattern-timeout
 */
void hw_init() {

    hub_state_to_default();

    syslog(LOG_DEBUG, "[BTN-CTRL]: Default Volume: %d Mute: %d", hub_status.speaker_volume_level, hub_status.mic_state);

    get_hub_configuration();

    syslog(LOG_DEBUG, "[BTN-CTRL]: Restoring HUB Status back, Volume: %d Mute: %d", hub_status.speaker_volume_level, hub_status.mic_state);

    set_volume_platform(get_vol_index_with_db(hub_status.speaker_volume_level));

    if(hub_status.mic_state) {
        mic_mute();
    }
    else {
        mic_unmute();
    }
}
