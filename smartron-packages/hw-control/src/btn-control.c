/*
 * File: btn-control.c
 * Copyright (C) 2018 Smartron India Private Limited - All Rights Reserved
 * AUTHOR :  Surya Teja
 *
 * Notes
 * -----
 * btn-control executes the shifu button action functionality pressed by the user.
 *
 * How it works:
 * When user 'tap's' or 'press and hold's', releases the shifu button's then MCU will
 * put the data on i2cbus. MCU is interfaced to shifu via gpio pin 45. Gpio-watch is
 * a tool which is used to moniter Gpio pin:45 on shifu. When user taps shifu buttons
 * interupt comes to gpio pin 45 then gpio-watch will send Signal to the hw-control
 * process. Once signal has recived to hw-control, it will read the i2c data on the bus
 * via i2cget() call. According to the data or values returned by the MCU for different
 * scenarios which are unique for the different operations, based on the values retruned
 * hw-control will perform operations according to the requirements.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/syslog.h>
#include <shifu-led.h>
#include "btn-control.h"
#include "volume-api.h"
#include "fw-env.h"
#include <sys/timerfd.h>
#include <poll.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "hw_config.h"

void die(char *s) {
    perror(s);
    exit(1);
}

struct msgbuf
{
    long    mtype;
    int     selectoption;
}sbuf;
struct msgbuf rbuf;
int button_factory = 0;
int ntemp;
shifu_button btn_act;
pthread_t thread_id;
bool mic_mute_status;
int action_state = 0;
int timer_handler_termination_flag = 0;
struct pollfd poll_list[MAX_POLL_FDS];

/* stores Shifu volume levels from 1 to 10  */
extern char volume_table_shifu[];

/* stores Shifu blutooth volume levels from 1 to 10 */
extern char volume_table_ext_BT[];

/* sturcture has following data variables which will send the date to led-server
 * int pattern;	LED_XXXX
 * int state;   PATTERN_STATE (0 for Off and 1 for On)
 * int value;   Only for hardware control module(for volume)
 */
struct led_struct led_pattern_structure;

/* stores led-client socket fd */
int led_fd;

    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key = 1234;
    size_t buflen;

void send_to_avs(int value) {

    //Message Type
	sbuf.selectoption = value;

    if (msgsnd(msqid, &sbuf, sizeof(struct msgbuf), IPC_NOWAIT) < 0) {
        syslog(LOG_DEBUG, "msqid = %d sbuf.mtype = %ld sbuf.selectoption = %d ", msqid, sbuf.mtype, sbuf.selectoption );
        die("msgsnd");
    }
}

/**
 * time_set() - initializes the timer for individual patterns
 * @param[in] - fd	        pattern fd's
 * @param[in] - itimerspec * time		timer sturcture
 * @param[in] - timeout	pattern-timeout
 */
static void time_set(int fd, unsigned long int timeout, unsigned long int nanotimeout) {
    struct itimerspec time;
    time.it_interval.tv_sec = 0;
    time.it_interval.tv_nsec = 0;
    time.it_value.tv_sec = timeout;
    time.it_value.tv_nsec = nanotimeout;

    if (timerfd_settime(fd, 0, &time, NULL) < 0) {
        syslog(LOG_ERR, "[LED]: timerfd_settime() failed: errno=%d", errno);
        close(fd);
        return;
    }
}

void button_up_volume_set_handler() {
    int volume_led_level;

    while(volume_up_termination_state) {
            /*volume_led_level = set_volume(VOLUME_UP, 1);
            led_pattern_structure.pattern = LED_VOLUME;
            led_pattern_structure.state = 1;
            led_pattern_structure.value = volume_led_level;
            shifu_led_pattern_send(led_fd, &led_pattern_structure);*/
            send_to_avs(VOLUME_UP);
            /*#22273:Volume buttons indication tone in AVS sdk */
            //system("aplay -f S16_LE -r 48000 /wav_files/volume.wav");
            usleep(1000 * 250);
    }
}

void button_down_volume_set_handler() {
    int volume_led_level;

    while(volume_down_termination_state) {
           /*volume_led_level = set_volume(VOLUME_DOWN, 1);
           led_pattern_structure.pattern = LED_VOLUME;
           led_pattern_structure.state = 1;
           led_pattern_structure.value  = volume_led_level;
           shifu_led_pattern_send(led_fd, &led_pattern_structure);*/
           send_to_avs(VOLUME_DOWN);
           /*#22273:Volume buttons indication tone in AVS sdk */
           //system("aplay -f S16_LE -r 48000 /wav_files/volume.wav");
           usleep(1000 * 250);
    }
}


void button_volumedown_micmute_handler() {
    int i = 0;
    action_state = 0;
    int temp = 1;
    int count = 0;
    int volume_tmp = 0;
    timer_handler_termination_flag = 1;

    while(action_state < RESET_TRIGGER_TIME) {
        action_state++;
        usleep(1000 * 1000);
    }

    timer_handler_termination_flag = 0;

    ntemp = 1;

    if(action_state == RESET_TRIGGER_TIME) {
        syslog(LOG_DEBUG,"RESET TO FACTORY SETTING EVENT DETECTED");
        system("/etc/init.d/va_service stop");
	volume_tmp = get_volume();
	/* #22366:Setting volume */
	//system("amixer -c0 sset DAC1 64");
	system("sh /announcements/setannouncementvolume.sh");
        system("gst-launch-1.0 playbin uri=file:///announcements/Reset_before_confirm.mp3 audio-sink=alsasink &");

            while(temp) {
                    if(ntemp == 3) {
                      temp = 0;
                    }
                    usleep(100 * 1000);
                    count++;
                    if(count == 150) {
                        ntemp = 0;
                        temp = 0;
                        set_volume_platform(volume_tmp);
                        syslog(LOG_DEBUG,"FACTORY RESET TRIGGER TIMED OUT");
                        system("/etc/init.d/va_service start");
                        return;
                    }
           }

        if(button_factory == 0x01) {
            syslog(LOG_DEBUG,"FACTORY RESET ACCEPTED");
            //sleep(5);
            system("killall gst-launch-1.0");
            system("firstboot && reboot -f &");
            return;
        }
        else {
        system("killall gst-launch-1.0");
	    set_volume_platform(volume_tmp);
            syslog(LOG_DEBUG,"FACTORY RESET REJECTED");
	    sleep(5);
	    while(msgrcv(msqid, &rbuf, sizeof(rbuf),0,IPC_NOWAIT) >0);
            system("/etc/init.d/va_service start");
            return;
        }
    }
}

void button_action_handler() {
    int i = 0;
    action_state = 0;

    timer_handler_termination_flag = 1;

    while(action_state < RESET_TRIGGER_TIME) {
        action_state++;
        if(action_state == BT_TRIGGER_TIME) {
            syslog(LOG_DEBUG,"[BTN-CTRL]: BT event detected!!");
            //22349
            //Turn ON BT discoverable mode on pressing and holding Action button for 10 seconds
            system("echo -e 'discoverable off' | bluetoothctl && echo -e 'discoverable on' | bluetoothctl &");
            system("gst-launch-1.0 playbin uri=file:///announcements/en_in_avs_system_prompt_bluetooth_ready_to_pair.mp3 audio-sink=alsasink &");
        }
        usleep(1000 * 1000);
    }

    timer_handler_termination_flag = 0;

    if(action_state == RESET_TRIGGER_TIME) {
        //SSH enabled after 30Sec of button press 
	//
	system("passwd -u root");
	system("uci set dropbear.@dropbear[0].enable='1' && uci commit dropbear");
	system("/etc/init.d/dropbear stop");
        system("/etc/init.d/dropbear start");
        syslog(LOG_DEBUG,"[BTN-CTRL]: Root disabled & SSH Access Enaled");
	system ("echo \"Root disabled & SSH Access Enaled\"");
    }
}

/**
 * myThreadFun() is a Thread function which is used for poll
 * the fd's with the LED pattern timeout handler
 * @param[in] - void
 */
void myThreadFun(void) {
    int i;
    int retval;
    uint64_t exp;
    int readBytes = 0;

    for(i = 0; i < MAX_POLL_FDS; i++) {
       if(poll_list[i].fd == -1) {
           syslog(LOG_ERR, "[LED]: Invalid Poll fd");
           return;
       }
    }

    while(1){
        retval = poll(poll_list, MAX_POLL_FDS, -1);
        if(retval <= 0){
            syslog(LOG_ERR, "[LED]: Error while polling");
            continue;
        }
        if((poll_list[VOLUMEUP_FD].revents & POLLIN) == POLLIN) {
            readBytes = read(poll_list[VOLUMEUP_FD].fd, &exp, sizeof(exp));
            button_up_volume_set_handler();
        }
        if((poll_list[FACTORYRESET_FD].revents & POLLIN) == POLLIN) {
            readBytes = read(poll_list[FACTORYRESET_FD].fd, &exp, sizeof(exp));
            button_volumedown_micmute_handler();
        }
        if((poll_list[BUTTON_ACTN_FD].revents & POLLIN) == POLLIN) {
            readBytes = read(poll_list[BUTTON_ACTN_FD].fd, &exp, sizeof(exp));
            button_action_handler();
        }
        if((poll_list[VOLUMEDOWN_FD].revents & POLLIN) == POLLIN) {
            readBytes = read(poll_list[VOLUMEDOWN_FD].fd, &exp, sizeof(exp));
            button_down_volume_set_handler();
        }
      }
}

/**
 * read_env_default_volume_values() - Reads the 10 volume levels for each button press
 */
void read_env_default_volume_values() {
    int env_ret;
    char env_var_val[8];
    /* Read default values from ENV */
    env_ret = get_shifu_uci_param("VA_BUTTON_VOL00", env_var_val);
    if(env_ret == 0) {
        volume_table_shifu[0] = atoi(env_var_val);
    }

    env_ret = get_shifu_uci_param("VA_BUTTON_VOL01", env_var_val);
    if(env_ret == 0) {
        volume_table_shifu[1] = atoi(env_var_val);
    }

    env_ret = get_shifu_uci_param("VA_BUTTON_VOL02", env_var_val);
    if(env_ret == 0) {
        volume_table_shifu[2] = atoi(env_var_val);
    }

    env_ret = get_shifu_uci_param("VA_BUTTON_VOL03", env_var_val);
    if(env_ret == 0) {
        volume_table_shifu[3] = atoi(env_var_val);
    }

    env_ret = get_shifu_uci_param("VA_BUTTON_VOL04", env_var_val);
    if(env_ret == 0) {
        volume_table_shifu[4] = atoi(env_var_val);
    }

    env_ret = get_shifu_uci_param("VA_BUTTON_VOL05", env_var_val);
    if(env_ret == 0) {
        volume_table_shifu[5] = atoi(env_var_val);
    }

    env_ret = get_shifu_uci_param("VA_BUTTON_VOL06", env_var_val);
    if(env_ret == 0) {
        volume_table_shifu[6] = atoi(env_var_val);
    }

    env_ret = get_shifu_uci_param("VA_BUTTON_VOL07", env_var_val);
    if(env_ret == 0) {
        volume_table_shifu[7] = atoi(env_var_val);
    }

    env_ret = get_shifu_uci_param("VA_BUTTON_VOL08", env_var_val);
    if(env_ret == 0) {
        volume_table_shifu[8] = atoi(env_var_val);
    }

    env_ret = get_shifu_uci_param("VA_BUTTON_VOL09", env_var_val);
    if(env_ret == 0) {
        volume_table_shifu[9] = atoi(env_var_val);
    }

    env_ret = get_shifu_uci_param("VA_BUTTON_VOL10", env_var_val);
    if(env_ret == 0) {
        volume_table_shifu[10] = atoi(env_var_val);
    }
}

/**
 * get_button_act() - reads the button triggered data from the mcu
 */
shifu_button get_button_act(void) {
    char btn_data[MAX_BUF_SIZE] = {0};
    char tmp_data[MAX_BUF_SIZE] = {0};
    shifu_button button_data;
    int i = 0;
    FILE *fd = NULL;
    system(CMD_GET_BTN_ACTION_DUMMY);
    memset(&btn_data, 0, sizeof(btn_data));
    memset(&tmp_data, 0, sizeof(tmp_data));
    memset(&button_data, 0, sizeof(button_data));

    fd = popen(CMD_GET_BTN_ACTION, "r");
    while((fgets(btn_data, MAX_BUF_SIZE, fd)) != NULL) {
        strcpy(tmp_data, &btn_data[2]);
        if(i == 0) {
            button_data.junk = (int)strtol(tmp_data, NULL, 16);
        } else if(i == 1) {
            button_data.keyStatus = (int)strtol(tmp_data, NULL, 16);
        }
        else if(i == 2) {
            button_data.keyEvent = (int)strtol(tmp_data, NULL, 16);
            break;
        }
        i++;
    }
    pclose(fd);
    return button_data;
}

/**
 * btn_ctrl_cb() - executes the button functionality accordingly
 *                 which user has triggered.
 */
void btn_ctrl_cb(void) {
    int rc;
    int signal_no;
    get_button_act();
    int volume_led_level = 0;

    sigset_t signals_to_catch;
    sigemptyset(&signals_to_catch);
    sigaddset(&signals_to_catch, SIGUSR1);

    sbuf.mtype = 1;
    /* Get the message queue ID for the given key */
    if ((msqid = msgget(key, msgflg )) < 0){
        die("msgget");
    }

    while(1) {

        sigwait(&signals_to_catch, &signal_no);
        memset(&btn_act, 0, sizeof(btn_act));
        btn_act = get_button_act();

        if(ntemp == 1 || ntemp == 2) {
            ntemp++;
            if(ntemp == 3) {
                button_factory = btn_act.keyStatus;
            }
        }
        syslog(LOG_DEBUG, "btn_act.keyStatus=%d btn_act.keyEvent=%d", btn_act.keyStatus, btn_act.keyEvent);

        volume_up_termination_state = 0;
        volume_down_termination_state = 0;

        time_set(volume_up_fd, 0, 0);
        time_set(volume_down_fd, 0, 0);
        time_set(factoryreset_fd, 0, 0);
        time_set(button_action_fd, 0, 0);

        if(timer_handler_termination_flag) {
            action_state = RESET_TRIGGER_TIME + 1;
            timer_handler_termination_flag = 0;
            ntemp = 0;
        }

        if( !(btn_act.keyStatus == VOLUMEDOWN_MICMUTE) && (btn_act.keyStatus & MICMUTE) ) {
            btn_act.keyStatus = MICMUTE;
        }
        else if(mic_mute_status) {
            system(MIC_MUTE_ON);
        }

        switch(btn_act.keyStatus) {

            /* Button Action */
            case BUTTONACTION:
                {
                    if(btn_act.keyEvent == BUTTONACTION_PRESS_INITIAL) {
                          time_set(button_action_fd, 1, 0);
                    } else if(btn_act.keyEvent == BUTTONACTION_PRESS_RELEASE1 || btn_act.keyEvent == BUTTONACTION_PRESS_RELEASE2) {
                          send_to_avs(WWD);
                    }
                }

                  /* Volume down */
            case VOLUMEDOWN:
                {
                    if(mic_mute_status) {
                        system(MIC_MUTE_ON);
                    }
                    if(btn_act.keyEvent == VOLUMEDOWN_PRESS) {
                        /*volume_led_level = set_volume(VOLUME_DOWN, 1);
                        led_pattern_structure.pattern = LED_VOLUME;
                        led_pattern_structure.state = 1;
                        led_pattern_structure.value  = volume_led_level;
                        //shifu_led_pattern_send(led_fd, &led_pattern_structure);*/
                        send_to_avs(VOLUME_DOWN);
                        /*#22273:Volume buttons indication tone in AVS sdk */
                        //system("aplay -f S16_LE -r 48000 /wav_files/volume.wav");
                        volume_down_termination_state = 1;
                        time_set(volume_down_fd, 1, 0);
                    } else if((btn_act.keyEvent == VOLUMEDOWN_PRESS_RELEASE)) {
                        /*volume_led_level = set_volume(VOLUME_DOWN, 1);
                        led_pattern_structure.pattern = LED_VOLUME;
                        led_pattern_structure.state = 1;
                        led_pattern_structure.value  = volume_led_level;
                        //shifu_led_pattern_send(led_fd, &led_pattern_structure);*/
                        send_to_avs(VOLUME_DOWN);
                        /*#22273:Volume buttons indication tone in AVS sdk */
                        //system("aplay -f S16_LE -r 48000 /wav_files/volume.wav");
                    }
                    break;
                }

                  /* Volume up */
            case VOLUMEUP:
                  {
                      if(btn_act.keyEvent == VOLUMEUP_PRESS) {
                          /*volume_led_level = set_volume(VOLUME_UP, 1);
                          led_pattern_structure.pattern = LED_VOLUME;
                          led_pattern_structure.state = 1;
                          led_pattern_structure.value = volume_led_level;
                          //shifu_led_pattern_send(led_fd, &led_pattern_structure);*/
                          send_to_avs(VOLUME_UP);
                          /*#22273:Volume buttons indication tone in AVS sdk */
                          //system("aplay -f S16_LE -r 48000 /wav_files/volume.wav");
                          volume_up_termination_state = 1;
                          time_set(volume_up_fd, 1, 0);
                      } else if((btn_act.keyEvent == VOLUMEUP_PRESS_RELEASE)) {
                          /*volume_led_level = set_volume(VOLUME_UP, 1);
                          led_pattern_structure.pattern = LED_VOLUME;
                          led_pattern_structure.state = 1;
                          led_pattern_structure.value = volume_led_level;
                          //shifu_led_pattern_send(led_fd, &led_pattern_structure);*/
                          send_to_avs(VOLUME_UP);
                          /*#22273:Volume buttons indication tone in AVS sdk */
                          //system("aplay -f S16_LE -r 48000 /wav_files/volume.wav");
                      }
                      break;
                  }

                 /* MIC-Mute button combination scenarios */
            case MICMUTE:
                   {
                       if( (btn_act.keyEvent == MICMUTE_0_SEC_PRESS) || (btn_act.keyEvent <= MICMUTE_2_SEC_PRESS) || (btn_act.keyEvent > MICMUTE_5_SEC_PRESS) ) {
                           mic_mute_status = !get_mic_mute_status();
                           send_mute_state(mic_mute_status);
                           set_mic_mute_status(mic_mute_status);
                       }
                       break;
                    }

                  /* + and Button-Action press */
            case VOLUMEUP_BUTTONACTION:
                  {
                      if(btn_act.keyEvent == VOLUMEUP_BUTTONACTION_0_SEC_PRESS) {
                          syslog(LOG_DEBUG,"+ and BA for one click");
                      } else if(btn_act.keyEvent == VOLUMEUP_BUTTONACTION_2_SEC_PRESS) {
                          syslog(LOG_DEBUG,"+ and BA for 2 seconds");
                      } else if(btn_act.keyEvent == VOLUMEUP_BUTTONACTION_5_SEC_PRESS) {
                          syslog(LOG_DEBUG,"+ and BA for 5 seconds");
                      }
                      break;
                  }

                  /* '-' and Button-Action press */
            case VOLUMEDOWN_BUTTONACTION:
                  {
                      if(btn_act.keyEvent == VOLUMEDOWN_BUTTONACTION_0_SEC_PRESS) {
                          syslog(LOG_DEBUG,"- and BA for one click");
                      } else if(btn_act.keyEvent == VOLUMEDOWN_BUTTONACTION_2_SEC_PRESS) {
                          syslog(LOG_DEBUG,"- and BA for 2 seconds");
                      } else if(btn_act.keyEvent == VOLUMEDOWN_BUTTONACTION_5_SEC_PRESS) {
                          syslog(LOG_DEBUG,"- and BA for 5 seconds");
                      }
                      break;
                  }

                 /* +,- and Button-Action press */
           case VOLUMEDOWN_MICMUTE:
                  {
                      if(!mic_mute_status) {
                        system(MIC_MUTE_OFF);
                      }
                      if(btn_act.keyEvent == VOLUMEDOWN_MICMUTE_PRESS_RELEASE) {
                          time_set(factoryreset_fd, 1, 0);
                      }
                      break;
                  }

           default:
                  {
                      syslog(LOG_ERR, "button error");
                  }
        }
   }
}

/**
 * get_shifu_bt_status() - finds BT SINK connected or not
 * @param[ ]
 * @return    BT SINK connection status
 */
int get_shifu_bt_status() {
    char vol[MAX_BT_CMD_BUF_SIZE] = {0};
    FILE *fd = NULL;
    fd = popen(GET_BT_SINK_CARD, "r");
    fgets(vol, MAX_BT_CMD_BUF_SIZE, fd);
    pclose(fd);
    if(strcmp(vol, BLUTOOTH_CARD) == 0) {
        syslog(LOG_DEBUG, "BT SINK card found %s", vol);
        return BT_SINK_CONNECTED;
    } else {
        syslog(LOG_DEBUG, "BT SINK card not found %s", vol);
        return BT_SINK_DISCONNECTED;
    }
}

void timer_fd_creation(int *fd,int val)
{
    *fd = timerfd_create(CLOCK_MONOTONIC, 0);

    if (*fd == -1) {
        syslog(LOG_ERR, "timerfd_create failed: errno=%d", errno);
        return;
	}

    syslog(LOG_DEBUG, "created timerfd %d", *fd);
	poll_list[val].fd = *fd;
    poll_list[val].events = POLLIN;
}

int main(int argc, char **argv) {

    int i;

    /* led-client socket initializattion */
    led_fd = shifu_led_init();

    sigset_t signals_to_block;
    sigemptyset(&signals_to_block);
    sigaddset(&signals_to_block, SIGUSR1);
    pthread_sigmask(SIG_BLOCK, &signals_to_block, NULL);

/*    while(access( "/tmp/volume.lock" , F_OK ) != -1) {
        usleep(100 * 1000);
    }
*/
    //hw_init();

    pthread_create(&btn_ctrl_cb_id, NULL, btn_ctrl_cb, NULL);

    //read_env_default_volume_values();
    syslog(LOG_DEBUG, "hw-control startup");

    /* Timer fd creation for the timeout*/
    for(i = 0; i < MAX_POLL_FDS; i++) {
        poll_list[i].fd = -1;
    }

    timer_fd_creation(&volume_up_fd, VOLUMEUP_FD);
    timer_fd_creation(&factoryreset_fd, FACTORYRESET_FD);
    timer_fd_creation(&button_action_fd, BUTTON_ACTN_FD);
    timer_fd_creation(&volume_down_fd, VOLUMEDOWN_FD);

    /* Creation of Thread function which is used for poll the Timer fd */
    pthread_create( &thread_id, NULL, (void * ) myThreadFun, NULL);

    pthread_join(btn_ctrl_cb_id, NULL);

    return 0;
}

