#include <json-c/json.h>

int shifu_va_db_init();
int shifu_va_setting_init();
int shifu_update_cron();
int cron_db_init();

#define CRON_DB_LOCATION "/root/va_data/ShifuCron.db"
#define VA_PUB_ZMQ_PORT_NUM "tcp://127.0.0.1:8800"
#define DM_ZMQ_PORT_NUM "tcp://localhost:7001"

#define MAX_RULE_COUNT 32


/**
 * Naming convention for columns in each tables of the Database
 */

// Table FOTA_INFO
/*-----column name------column index*/
#define FOTA_ID 			0
#define FOTA_CRONJOB 		1
#define FOTA_EXECUTABLE		2

// Table DM_RESTART_INFO
/*-----column name------column index*/
#define PA_RESTART_ID 		0
#define PA_RESTART_CRONJOB 	1
#define PA_RESTART_COMMAND	2

// Table RULE_INFO
/*-----column name------column index*/
#define RULE_ID 			0
#define RULE_CRONJOB 		1

// Table DM_RESTART_INFO
/*-----column name------column index*/
#define DM_RESTART_ID 		0
#define DM_RESTART_CRONJOB 	1
#define DM_RESTART_COMMAND	2


/**=============================================================================================
RETURN_STATUS
===============================================================================================*/
typedef enum
{
	NONE,
	SUCCESSFUL = 200,
	BAD_REQUEST	= 400,
	EXCEEDS_LIMIT = 429,
	FAILED_TO_PROCESS = 500
}RETURN_STATUS;

#define MAX_BUFSIZE_2      2
#define MAX_BUFSIZE_4      4
#define MAX_BUFSIZE_8      8
#define MAX_BUFSIZE_16     16
#define MAX_BUFSIZE_32     32
#define MAX_BUFSIZE_64     64
#define MAX_BUFSIZE_128    128
#define MAX_BUFSIZE_256    256
#define MAX_BUFSIZE_512    512
#define MAX_BUFSIZE_1024   1024
#define MAX_BUFSIZE_2048   2048
#define MAX_BUFSIZE_4096   4096
#define MAX_BUFSIZE_8192   8192
#define MAX_BUFSIZE_16384  16384
#define MAX_BUFSIZE_32768  32768
#define MAX_BUFSIZE_1MB    (1024*1024)
