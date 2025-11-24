#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "uci_alsa_config.h"

/*get_fw_env method will greps the value from fw env variable into buf, and returns -1 if faild*/
int get_val_frm_conf(char *var, char *buf)
{
	int rc = 0;

	char cmd[CMD_MAC_CH];
	cmd[0] = '\0';

	/*cmd is a char array which will stores the cmd with variable*/
	sprintf(cmd, "uci get alsa.alsa.%s", var);

	/*we are opening a file pointer*/
	FILE *fp;

	/* Open the command for reading. */
	fp = popen(cmd, "r"); /*we are checking pipe opened or not opened then exit with -1*/
	if (fp == NULL) {
		syslog(LOG_ERR, "BTN-CTRL:failed in get_env %s", var);
	}

	/* Read the output a line at a time - output it. */
	while(fgets(buf, BUF_MAX, fp) != NULL);

	/* close */
	pclose(fp);

	if(!strlen(buf))
	{
		syslog(LOG_ERR, "BTN-CTRL:failed in get_env not defined %s", var);
		rc = -1;
	}
	return rc;
}

/*set_fw_env method will set the value into fw env variables, and returns -1 if faild*/
int set_val_to_conf(char *var, int val)
{
    int rc = 0;

    char cmd[CMD_MAC_CH];
	cmd[0] = '\0';

    sprintf(cmd, "uci set alsa.alsa.%s=%d && uci commit alsa", var, val );

	/*passing command to set fw env */
	if ( system(cmd) < 0 )  /*we are checking for success if not exit with -1*/
	{
		syslog(LOG_ERR, "BTN-CTRL:failed in %s", var);
		rc = -1;
	}
	return rc;
}

#if TEST
int main(int argc, char *argv[]) {

    char buffer[BUF_MAX];
    buffer[0] = '\0';

    if(argc < 2 || argc > 3) {
        printf("Invalid arguments\n");
        return 0;
    }

    if(argc == 3) {
        printf("DP-set:%s\n",argv[1]);
        if(set_val_to_conf(argv[1], atol(argv[2])) < 0) {
            printf("invalid\n");
            return 0;
        }
        else {
            printf("valid\n");
        }
    }

    if(argc == 2) {
        if(get_val_frm_conf(argv[1], buffer) < 0) {
            printf("invalid\n");
            return 0;
        }
        else {
            printf("%s = %d\n", argv[1], atol(buffer));
        }
   }

    return 0;
}
#endif
