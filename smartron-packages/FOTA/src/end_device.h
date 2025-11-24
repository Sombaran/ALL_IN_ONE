/*
 * end_device.h
 *
 *  Created on: 15 Mar 2019
 *      Author: smartron
 */

#ifndef END_DEVICE_H_
#define END_DEVICE_H_

#endif /* END_DEVICE_H_ */

#include "fota.h"

#define FOTA_PORT   7771
#define MA_PORT     7772
#define DM_PORT     7773
#define MAXLINE 1024


#define MAX_END_DEVICE 5
#define END_DEVICE_DATABASE "/exe_dir/DeviceManagerBackUpDB.db"
#define END_DEVICE_VERSION_JSON_PATH     "/etc/shifu_version.json"
#define DNLD_END_DEVICE_FIRMWARE_JSON_PATH     "/etc/dnld_end_device_firmware_info.json"

#define CLIENT_MA 13
#define CLIENT_DM 14
#define CLIENT_FOTA 16
#define THING true
#define HUB false

struct sockaddr_in cliaddr, cli_addr, ma_cliaddr, dm_cliaddr;
char buffer[MAXLINE], buffer1[MAXLINE];

char fota_json_file_path[PATH_LEN];
char fota_end_device_json_cdn_url[URL_LEN];
int sockfd;
json_object *jthingid_list;
json_object *thingid_obj[MAX_END_DEVICE]; // = json_object_new_array();
int server_fd;

char Firmware_version[VERSION_LEN];
char Manufacturer_name[64];
char Hardware_version[VERSION_LEN];
char Model_id[64];
char response_flag;
char received_status;
static char fota_silent_flag = 1;
sigset_t set;
