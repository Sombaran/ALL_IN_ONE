
/*
 * File: btn-control.h
 * Copyright (C) 2018 Smartron India Private Limited - All Rights Reserved
 * AUTHOR :  Surya Teja
 *
 * Notes
 * -----
 * "This file declares macros, global variables, structures and functions".
 *
 * "This file is particularly used in btn-control.c.
 * so btn-control.c can only access these header file."
 */

#ifndef __BTN_CTRL_H__
#define __BTN_CTRL_H__

#define MIC_MUTE_ON               "i2cset -y 0 0x18 0x51 0x01"
#define MIC_MUTE_OFF              "i2cset -y 0 0x18 0x51 0x00"
#define CMD_GET_BTN_ACTION        "i2cset -y 0 0x18 0x50 0xff i && i2cget -y 0 0x18 0x50 && i2cget -y 0 0x18 0x50 && i2cget -y 0 0x18 0x50"
#define CMD_GET_BTN_ACTION_DUMMY  "i2cget -y 0 0x18 0x50 > /dev/null"

#define ANNOUNCE_ALEXA_RESET "gst-launch-1.0 playbin uri=file:/announcements/reset_without_IOT.mp3 audio-sink=alsasink volume=2"

#define BUTTONACTION 4
#define BUTTONACTION_PRESS_INITIAL  112
#define BUTTONACTION_PRESS_RELEASE1 117
#define BUTTONACTION_PRESS_RELEASE2 5


#define VOLUMEDOWN 2
#define VOLUMEDOWN_PRESS 96
#define VOLUMEDOWN_PRESS_RELEASE 101

#define VOLUMEUP 1
#define VOLUMEUP_PRESS 80
#define VOLUMEUP_PRESS_RELEASE 85

#define MICMUTE 8
#define MICMUTE_0_SEC_PRESS 0
#define MICMUTE_2_SEC_PRESS 100
#define MICMUTE_5_SEC_PRESS 100

#define VOLUMEUP_VOLUMEDOWN 3
#define VOLUMEUP_VOLUMEDOWN_PRESS_RELEASE 112

#define VOLUMEDOWN_MICMUTE 10
#define VOLUMEDOWN_MICMUTE_PRESS_RELEASE 240

#define VOLUMEUP_BUTTONACTION 5
#define VOLUMEDOWN_BUTTONACTION 6

#define VOLUMEUP_BUTTONACTION 5
#define VOLUMEUP_BUTTONACTION_0_SEC_PRESS 0
#define VOLUMEUP_BUTTONACTION_2_SEC_PRESS 17
#define VOLUMEUP_BUTTONACTION_5_SEC_PRESS 34

#define VOLUMEDOWN_BUTTONACTION 6
#define VOLUMEDOWN_BUTTONACTION_0_SEC_PRESS 0
#define VOLUMEDOWN_BUTTONACTION_2_SEC_PRESS 20
#define VOLUMEDOWN_BUTTONACTION_5_SEC_PRESS 40

#define RESET_TRIGGER_TIME 30
#define BT_TRIGGER_TIME 10

#define MAX_BUF_SIZE 255
#define MAX_BT_CMD_BUF_SIZE 11

/* structure used toe store the returned value
 * from the mcu when the button has been triggered.
 */
typedef struct button {
    int junk;
    int keyStatus;
    int keyEvent;
}shifu_button;

pthread_t btn_ctrl_cb_id;


typedef enum {
   VOLUME_UP = 1,
   VOLUME_DOWN,
   WWD,
   MIC_MUTE,
   MIC_UNMUTE,
   ALEXA_RESET
}snd_keyto_avs;

typedef enum {
    VOLUMEUP_FD = 0,
    FACTORYRESET_FD,
    BUTTON_ACTN_FD,
    VOLUMEDOWN_FD,
    MAX_POLL_FDS
};

/* Creation of fd's and timer variables,
 * polling list variables for all the timeout patters
 */
static int volume_up_fd = -1;
static int volume_down_fd = -1;
static int factoryreset_fd = -1;
static int button_action_fd = -1;

int volume_status;
int volume_revert_flag;
int volume_up_down_state;
int volume_up_termination_state;
int volume_down_termination_state;
int network_reset_led_status;

void button_select_handler();
void button_volume_set_handler();
void button_volumeup_volumedown_handler();
void button_volumeup_volumedown_action_handler();
void send_to_avs(int value);

#endif //__BTN_CTRL_H__
