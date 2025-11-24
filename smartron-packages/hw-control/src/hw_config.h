#include "uci_alsa_config.h"

#ifndef __HW_CONFIG_H__
#define __HW_CONFIG_H__

#define CHAR_LEN 4

typedef enum {
	UNMUTE,
	MUTE,
}voice_state;

typedef struct {
	int led_level;
	int speaker_volume_level;
	voice_state mic_state;
	voice_state speaker_state;
}hub_status_t;

void hw_init(void);
void mic_mute(void);
void mic_unmute(void);
void set_mic_mute_status(int state);
void send_mute_state(int mic_mute_status);
int get_mic_mute_status(void);

#endif //__VOLUME_API_H__
