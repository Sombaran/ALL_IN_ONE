#ifndef FOTA_H                  /* to prevent the header file from being included multiple times */
#define FOTA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <fcntl.h>
#include <json-c/json.h>
#include <stdbool.h>
#include <syslog.h>             /* send messages to the system logger */
#include <time.h>
#include <curl/curl.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sqlite3.h>
#include <pthread.h> 
#include "fw-env.h"             /* to get NVM variables */
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>


#define CLIENT_WS                   12
#define CLIENT_MA                   13
#define CLIENT_DM                   14
#define CLIENT_FOTA                 16
#define CLIENT_LUCI                 17

#define FOTA_PORT                   7771
#define MA_PORT                     7772
#define DM_PORT                     7773
#define LUCI_PORT                   7774

/* MACROs used to determine length of variables*/
#define DNLD_REMOTE_FNAME_LEN       256
#define URL_LEN                     512
#define PATH_LEN                    256
#define ENV_LEN                     64
#define VERSION_LEN                 16
#define CHANGELOG_STR_LEN           4096
#define CREATED_AT_LEN              16
#define FILENAME_LEN                64
#define HEADER_FIELD_LEN            128
#define COMMAND_LEN                 512
#define DEFAULT_VALUE               0
#define EXTRA_SPACE_TMP             32
#define EXTRA_SPACE_OVERLAY         128
#define MAXLINE                     1024
// Bug #19709:Start Error handling unwanted and unexpected deletion of FOTA related files from the system.
#define FLAG_LEN                    16
#define LEN_256                     256
#define LEN_128                     128
// Bug #19709:End
#define MAX_END_DEVICE              5
#define MAX_THINGS                  10

/* MACRO to determine no of curl retries*/
#define CURL_NUM_RETRIES_ON_ERR     1

/* Macros for paths */
#define DNLD_FIRMWARE_JSON_PATH     "/etc/dnld_firmware_info.json"  /* path to get json with present firmware details */
#define SHIFU_VERSION_JSON_PATH     "/etc/shifu_version.json"
#define FOTA_STATUS_JSON_PATH       "/etc/fota_status.json"
#define FCM_PUSH_URL_PATH           "/hub/fcm/push"
#define THUB_DNLD_FILES_PATH        "/root/downloads/fw/"
#define ED_DNLD_FILES_PATH          "/root/downloads/fw/enddevota/"
#define ED_CONFIG_FILE_PATH         "/etc/ota/"
#define END_DEVICE_DATABASE         "/exe_dir/DeviceManagerBackUpDB.db"
#define PUBLIC_KEY                  "/etc/fw/fota-public.pem"


//char fota_json_file_path[PATH_LEN];
char fota_json_cdn_url[URL_LEN];

/* MACRO to enable Verbose */
#define VERBOSE                     0L          /* 0L: Disable  1L: Enable */

#define ENUM_MIN                    -100

/**
 * @brief enum containing return codes used in FOTA module
 */
typedef enum {
    UPGRADE_IN_PROGRESS = ENUM_MIN,
    CHECK_IN_PROGRESS,
    JSON_FILE_READ_ERR,
    JSON_FILE_WRITE_ERR,
    MD5_FETCH_ERR,
    MD5_COMP_FAIL,
    ENV_ERR,
    HTTP_ERR,  ////
    UPGRADE_ERR,
    UP_TO_DATE,
    INVALID_DATA,
    FAILURE,
    SUCCESS=0
} return_code;

/**
 * @brief Data structure which describes the file to be downloaded.
 */
typedef struct {
    char dnld_remote_fname[DNLD_REMOTE_FNAME_LEN];
    char dnld_url[URL_LEN];
    FILE *dnld_stream;
    FILE *dbg_stream;
    uint64_t dnld_file_sz;
} dnld_params_type;

/**
 * @brief Data structure which describes url response data.
 */
typedef struct {
    size_t size;
    char *data;
} url_data_type;

 /* @brief Data structure which describes security header version 1.
 */
typedef struct _sec_hdr_v1 {
   char magic[8];
   int  hdr_version;
   int  img_offset;
   int  res1;
   int  res2;
   int  sig_offset;
   int  res3;
   char fw_ver_str[16];
   char hw_model_str[16];
} sec_hdr_v1;

struct sockaddr_in cliaddr, cli_addr, ma_cliaddr, dm_cliaddr, luci_cliaddr,req_cliaddr;
char buffer[MAXLINE];

int sockfd;
json_object *jthingid_list;
json_object *jthingid_obj[MAX_END_DEVICE];
int server_fd;

char response_flag;
char received_status;
static char fota_silent_flag = 1;
sigset_t set;

void generate_request_id(char *request_id, char *device_id);

#endif                          /* FOTA_H */


