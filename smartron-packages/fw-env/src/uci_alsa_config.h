#ifndef __UCI_CONFIG_H__
#define __UCI_CONFIG_H__

/* UCI alsa key define */
#define OPT_KEY_ALSA_ALSA_SPKVOL  	"speaker_volume"
#define OPT_KEY_ALSA_ALSA_SPKMUTE 	"speaker_mute"
#define OPT_KEY_ALSA_ALSA_MICVOL  	"mic_volume"
#define OPT_KEY_ALSA_ALSA_MICMUTE 	"mic_mute"
#define OPT_KEY_ALSA_ALSA_LED     	"led"

/* UCI alsa key default value */
#define DEFAULT_ALSA_ALSA_SPKVOL  64
#define DEFAULT_ALSA_ALSA_SPKMUTE 0
#define DEFAULT_ALSA_ALSA_MICVOL  50
#define DEFAULT_ALSA_ALSA_MICMUTE 0
#define DEFAULT_ALSA_ALSA_LED     8

#define BUF_MAX 1024
#define CMD_MAC_CH 128

/*Below get_val_frm_conf and set_val_to_conf are used to access the uci alsa.alsa.x section*/
int get_val_frm_conf(char *var, char *buf);
int set_val_to_conf(char *var, int val);
#endif //__UCI_CONFIG_H__
