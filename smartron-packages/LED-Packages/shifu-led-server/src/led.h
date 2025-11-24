/*
 * File: led.h
 * Copyright (C) 2018 Smartron India Private Limited - All Rights Reserved
 * AUTHOR :  Surya Teja
 *
 * Notes
 * -----
 * "This file declares macros, global variables, structures and functions".
 */

#ifndef __LED_H__
#define __LED_H__

#include <poll.h>
#include "shifu-led.h"

#define DEVICE_REGISTERED	"registered"
#define VOLUME_MUTE_PATTERN	"i2cset -y 0 0x18 0x49 0xb4 0xff 0xff 0xff 0xff 0xff 0x15 0x10 0x10 0x00 0x3 i"
#define MAX_REQUEST_PATTERNS	26
#define MAX_RUNNING_PATTERNS	26
#define MAX_POLL_FDS            1
#define MAX_PATTERNS            22
#define ALARM_TIMEOUT           3600
#define AUTH_FAIL_TIMEOUT       5
#define NO_INTERNET_TIMEOUT     5
#define NO_INTERNET     1
#define REBOOT_PAT_TIMEOUT		20		//timeout for reboot LED pattern
#define AVS_PID_BUF_SIZE		16		//Max buf Size of the AVS PID
/*
 * This is to indicate that the user has requested a new pattern while other pattern
 * is already being played.
 */
#define LED_MIXED		(1 << 31)

/* LED pattern Timeout variables. All values in seconds. */
static short volume_timeout  = 1;
static short micmute_timeout = 3;
static short error_timeout   = 3;
static short alarm_timeout   = 600;
static short default_timeout = 30;
static short micmute_off_timeout  = 1;
static short voice_volume_timeout = 1;
static short setup_mode_timeout   = 30;
static short bt_connected_timeout = 3;
static short bt_discovery_timeout = 35;
static short device_connected_timeout = 3;
static short device_discovery_timeout = 65;
static short query_response_timeout   = 600;
static short query_processing_timeout = 30;

/* Creation of fd's and timer variables,
 * polling list variables for all the timeout patters
 */
static int timer_fd;

struct pollfd poll_list[MAX_POLL_FDS];
pthread_t thread_id;

pthread_t led_listen_start;

/* status of the device registration (not registered, registered) */
static char device_registration_status[15];

/* status of the alexa registration (not registered, registered) */
static char alexa_registration_status[15];

/* status of the skip alexa registration (true, false) */
static char skip_alexa_registration[15];

/* pattern sent to queue when client request for a led-pattern which
 * is low priority than the running led pattern
 */
static unsigned int queued_led_patterns = LED_OFF;

/* Current running led pattern */
static unsigned int led_pattern_running = LED_OFF;

/* When client requests for the mixed pattern then the
 * running pattern will be set to background and requested
 * pattern will the forground pattern
 */
static unsigned int led_pattern_backend = LED_OFF;

/* When client requests for the mixed pattern then the
 * running pattern will be set to background and requested
 * pattern will the forground pattern
 */
static unsigned int led_pattern_frontend = LED_OFF;

/* Used to print the mixed pattern state for loging */
static unsigned int printmix_pattern = 0;

/* running pattern is set in the pattern_stop variable when ever new
 * pattern has started for timeout purpose
 */
static unsigned int pattern_stop = LED_OFF;

/* present volume of the device */
static unsigned int volume_level = 0;

/*
 * 0 = mixed pattern not running
 * 1 = mixed pattern running
 */
static int is_mixed_pattern = 0;

/* Enum vaiables for AVS configuration states */
typedef enum {
    WAIT_FOR_CONFIG,
    CONFIG_DONE
} AVS_CONFIG_STATE;

/* creation of enum vaiarble avs_config_state with the type AVS_CONFIG_STATE */
AVS_CONFIG_STATE avs_config_state = WAIT_FOR_CONFIG;

/* Enum vaiables for led-pattern states */
typedef enum {
    PI,
    PAR,
    PAT,
    DIS,
    INVALID,
    MIX
} LED_PATTERN_STATE;

/* creation of enum vaiarble pattern_state with the type LED_PATTERN_STATE */
static LED_PATTERN_STATE pattern_state = DIS;

/*
 *	PI     Play Immediate      Play the Requested Pattern Immediately
 *	PAT    Play After Timeout  Continue Current Pattern till the Timeout and Play
 *                                 Requested Pattern After that
 *	PAR    Play And Resume	   Play Requested Pattern and Resume Previous Pattern
 *	DIS    Discard             Discard the Requested Pattern
 *	MIX    Mixed pattern       Play with the running pattern especially for
 *                                 Hw-volume and voice based volume
 */
static LED_PATTERN_STATE get_requested_pattern_state[MAX_REQUEST_PATTERNS][MAX_RUNNING_PATTERNS] = {
							/* Running pattern */

/* LO    VO    MM    IC    ER    WW    AL    QP    QR    DC    DD    WM    SM    MI    MO    BD    BC    NR    VV    WO    DN    NT   LA    */


                                                                                                                                                  /* Rquested Pattern */

{  PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,  PI },        /* LED_OFF  (LO) */

{  PI,   PI,   MIX,  PAR,  MIX,  PAR,  MIX,  MIX,  MIX,  MIX,  MIX,  MIX,  MIX,  PAR,  PI,   PAR,  PAR,  PAR,  PI,   MIX,  MIX,  MIX, PI },        /* VOLUME   (VO) */

{  PI,   PI,   PI,   PAR,  PAR,  PAT,  MIX,  PAT,  PAT,  PAR,  PAT,  PAR,  PAT,  MIX,  PI,   PAR,  PAR,  PAR,  MIX,  PI,   PI,   PAT, PI },        /* MIC_MUTE (MM) */

{  PI,   PAT,  PAR,  PI,   PAT,  PAT,  PAT,  DIS,  DIS,  PAT,  PAT,  PAT,  DIS,  DIS,  PI,   PAT,  PAT,  PAT,  PAR,  PAT,  PAT,  PAT, PI },       /* INTERNET_CHECK */

{  PI,   MIX,  PAR,  PAT,  PI,   PAR,  PAR,  PAR,  PAR,  PAR,  PAR,  PAR,  PAR,  MIX,  PAT,  PAR,  PAR,  PAT,  PAT,  PAR,  PAT,  PAT, PI },        /* ERROR    (ER) */

{  PI,   PI,   PAR,  DIS,  PAR,  PI,   PAR,  PAR,  PAR,  PAR,  PAR,  PAR,  PAR,  MIX,  PI,   PAR,  PAR,  PAR,  PI,   PI,   PI,   PAR, PI },        /* WAKEWORD (WW) */

{  PI,   PI,   MIX,  PAR,  PI,   PAT,  PI,   PAR,  PAT,  PAR,  PAT,  PAR,  PAR,  MIX,  PI,   PAR,  PAR,  PAT,  PI,   PAT,  PAT,  MIX, PI },        /* ALARM    (AL) */

{  PI,   MIX,  PAR,  DIS,  PAT,  PAT,  DIS,  PI,   PAT,  PAR,  PAR,  PAR,  PAR,  MIX,  PAT,  PAR,  PAR,  PAR,  PAT,  PAT,  PAT,  PAR, PI },        /*QUERY_PROCC(QP)*/

{  PI,   MIX,  PAR,  DIS,  PAT,  PAT,  DIS,  PI,   PI,   PAR,  PAR,  PAR,  PAR,  MIX,  PAT,  PAR,  PAR,  PAR,  PAT,  PI,   PAT,  PAR, PI },         /*QUERY_RESP(QR)*/

{  PI,   PAR,  PAR,  DIS,  DIS,  PAT,  PAR,  DIS,  DIS,  PI,   PI,   PI,   PAR,  MIX,  PAT,  PAR,  PAR,  PAT,  DIS,  PAT,  PAT,  PAR, PI },        /*DEVICE_CONN(DC)*/

{  PI,   PAR,  PAR,  PAT,  PAT,  PAT,  PAR,  PAT,  PAT,  PI,   PI,   PI,   PAR,  MIX,  PAT,  PAR,  PAT,  PAT,  PAT,  PAT,  PAR,  PAR, PI },        /*DEVICE_DISC(DD)*/

{  PI,   PAT,  PAR,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PI,   PAR,  MIX,  PAT,  PAR,  PAT,  PAT,  PAT,  PAT,  PAR,  PAR, PI },           /*WPS_MODE(WM)*/

{  PI,   PAT,  PAR,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PI,   MIX,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PAR,  PAR, PI },         /*SETUP_MODE(SM)*/

{  PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   PI,   MIX,  PI,   PI,   PI,   PAT,  PI,   PI,   PI,   PI,  PI },         /*MIXED(MI) */

{  PI,   PI,   PI,   DIS,  DIS,  DIS,  PAR,  PAR,  PAR,  DIS,  DIS,  DIS,  DIS,  PI,   PI,   DIS,  DIS,  PAT,  DIS,  DIS,  PAR,  PAR, DIS},       /*MIC_MUTE_OFF(MO)*/

{  PI,   PAR,  PAR,  PAR,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PAR,  PAR,  MIX,  PAT,  PI,   PAT,  PAT,  PAR,  PAT,  PAR,  PAR, PI },     /*BLUTOOTH_DISCO(BD)*/

{  PI,   PAR,  PAR,  PAR,  PAT,  PAT,  PAR,  DIS,  DIS,  PAR,  PAR,  PAR,  PAR,  MIX,  PAT,  PAR,  PI,   PAT,  DIS,  PI,   PAR,  PAR, PI },    /*BLUTOOTH_CONNECTED(BC)*/

{  PI,   PAR,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PAR,  MIX,  PAT,  PAT,  PAT,  PI,   PAT,  PAT,  PAR,  PAR, PI },      /*NETWORK_RESET(NR)*/

{  PI,   PI,   PAT,  PAR,  PAR,  PAT,  PAR,  PAT,  PAT,  PAR,  PAR,  PAR,  PAR,  MIX,  PAT,  PAR,  PAR,  PAT,  PI,   PAT,  MIX,  MIX, PI },      /*VOICE-VOLUME(VV)*/

{  PI,   PI,   DIS,  DIS,  PAR,  PAT,  DIS,  PAR,  PAT,  PAR,  PAR,  PAR,  PAR,  MIX,  PI,   PAR,  PAR,  PAR,  PI,   PI,   PAT,  PAR, PI },   /*LED_WAKEWORD_OFF(WW)*/

{  PI,   MIX,  DIS,  DIS,  PAR,  PAT,  PAR,  PAR,  PAR,  PAR,  PAR,  PAR,  PAR,  MIX,  PI,   PAR,  PAR,  PAR,  PI,   PI,   PAT,  PAR, PI },       /*LED_DND(DN)*/

{  PI,   PAT,  PAR,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PI,   MIX,  PAT,  PAT,  PAT,  PAT,  PAT,  PAT,  PAR,  PI,  PI },       /*LED_NOTIFY(LN)*/

{  PI,   PI,   DIS,  PI,   PI,   PI,   PAR,  PAR,  PAR,  PAR,  PAR,  DIS,  PAR,  MIX,  PAT,  PAR,  PAR,  DIS,  PAT,  PAR,  PAT,  PAR, PI },      /*LED_ANNOUNCEMENT(LA) */

};


/* Function Declarations */
void alexa_finish();
static void led_listen();
void led_listen_thread();
static void *led_listen_cb();
void* no_Internet_Check_Monitor(void *var);
void* wait_for_avs_config(void *var);
void avs_config_done();
static char* printstate(int value);
static int timer_fd_creation(int val);
static void shifu_led_resume_pattern();
static void time_set(int fd, int timeout);
int func_To_Get_from_UCI(char *cmd, int *val);
static void read_env_default_timeout_values();
static char* printpattern(int value, int state);
static int shifu_led_default_timeout_handler(void);
static int get_state_value_for_pattern(PATTERN pattern);
static void shifu_led_start_pattern(int pattern, int value);
static void set_volume_pattern(int volume_led_level, int is_voice);
static void shifu_led_pattern_processing(unsigned int led_pattern, int led_start_stop, int value);
static void shifu_led_process_mixed_pattern(unsigned int led_pattern, int led_start_stop, int value);

#endif // __LED_H__


