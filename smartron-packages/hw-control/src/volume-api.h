/*
 * File: btn-control.h
 * Copyright (C) 2018 Smartron India Private Limited - All Rights Reserved
 * AUTHOR :  Surya Teja
 *
 * Notes
 * -----
 * "This file declares macros, global variables, structures and functions".
 *
 * "This file is particularly used in volume-api.c.
 * so volume-api.c can only access these header file."
 */

#ifndef __VOLUME_API_H__
#define __VOLUME_API_H__

#define TOTAL_LED_NUM  10

#define VOLUME_UP     1
#define VOLUME_SET    0
#define VOLUME_DOWN   2

#define BT_SINK_CONNECTED    1
#define BT_SINK_DISCONNECTED 0

#define BLUTOOTH_CARD      "bluez_sink"
#define GET_BT_SINK_CARD   "pactl list sinks short | grep bluez_sink | awk 'NR==1 {print $2}'"

#define CMD_SET_VOLUME_SHIFU    "amixer -c 0 sset 'DAC1' %d"
#define CMD_SET_VOLUME_BT_SINK  "pactl set-sink-volume %s %d%%"

#define CMD_GET_VOLUME_SHIFU    "amixer -c 0 cget numid=7 | grep \": values\" |  awk -F \",\"  '\{ print $2 \}'"
#define CMD_GET_VOLUME_BT_SINK  "pactl list sinks | grep -A 10 bluez_sink | grep Volume | awk '{print $5}' | head -n 1"

/* BT_sink_connected = 1 --> Blutooth Connected,
 * BT_sink_connected = 0 --> Blutooth Disconnected
 */
static char BT_sink_connected = 0;

int get_volume(void);
int set_volume(int mode, int level);
void set_volume_platform(int volume_table_index);
int get_vol_db_with_index(int index);
int get_vol_index_with_db(int db);

#endif //__VOLUME_API_H__
