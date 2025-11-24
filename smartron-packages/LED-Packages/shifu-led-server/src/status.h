#include "uci_alsa_config.h"

#define CHAR_LEN 4
#define TOTAL_LED_NUM  10
#define CMD_SET_VOLUME_SHIFU    "amixer -c 0 sset 'DAC1' %d"
#define CMD_SET_VOLUME_BT_SINK  "pactl set-sink-volume %s %d%%"
#define MIC_MUTE_ON               "i2cset -y 0 0x18 0x51 0x01"
#define MIC_MUTE_OFF              "i2cset -y 0 0x18 0x51 0x00"


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

void mic_mute(void);
int mic_init(void);
void mic_unmute(void);
void volume_init(void);
