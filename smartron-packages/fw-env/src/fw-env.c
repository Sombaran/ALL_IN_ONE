/************************************************************************************************************
* FILENAME :       fw-env.c
*
* DESCRIPTION :
*			This header file is contain, API's to get or set fw_env variables using C program. 
*			these functions are like string functions. we should not to pass any null values.
*			
*			int get_fw_env(char *var,char *buf);
*			Return value is zero if successfully executed. var is a char pointer we have to 
*			pass a string of which variable value we need to get. buf will gets the val into it.
*			internal buf MAX is 1024.
*			
*			int set_fw_env(char *var,char *val);
*			Return value is zero if successfully executed. var is a char pointer we have to 
*			pass a string of which variable value we need to set and as well as pass the val.
*			
*			Note: set_fw_env function won't	remove variables. what ever the variables you want 
*			to use you need provid us before. Passing variable should not exceed 60 characters.
*
* AUTHOR :  Janardhanachari.kella
*
* VERSION:  0.1
*
* Lastmodified: janardhanachari.kella@smartron.com, 12/02/2018
*
* Copyright (C) 2018 Smartron India Private Limited - All Rights Reserved 
*
**********************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "fw-env.h"

#define BUF_MAX 1024
#define CMD_MAC_CH 128

void init_log()
{
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("fw-env",LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
}

void remove_char_from_string(char c, char *str)
{
    int i=0;
    int len = strlen(str)+1;

    for(i=0; i<len; i++)
    {
        if(str[i] == c)
        {
            // Move all the char following the char "c" by one to the left.
            strncpy(&str[i],&str[i+1],len-i);
        }
    }
}

/*get_fw_env method will greps the value from fw env variable into buf, and returns -1 if faild*/
int __get_fw_env(char *_var,char *_buf)
{
	/*cmd is a char array which will stores the cmd with variable*/ 
	char cmd[CMD_MAC_CH]="fw_printenv |grep \""; /* fw_printenv DevicID */
	strcat(cmd, _var);
	strcat(cmd, "\"|awk -F \"=\" '{print $2}'");
	
	/*we are opening a file pointer*/
	FILE *fp;
	
	/* Open the command for reading. */
	fp = popen(cmd, "r"); /*we are checking pipe opened or not opened then exit with -1*/
	if (fp == NULL) {
		init_log();
		syslog(LOG_NOTICE, "FEV:failed in get_fw_env");
		closelog();
		exit(-1);
	}

	/* Read the output a line at a time - output it. */
	while(fgets(_buf, BUF_MAX, fp) != NULL);
	
	/* close */
	pclose(fp);
	
	if(!strlen(_buf))
	{
		syslog(LOG_NOTICE, "FEV:NVM Value is not defined");
		return -1;
	}
	else
	{
		return 0;
	}
}

/* Read Shifu specific configuration from UCI system */
int get_shifu_uci_param(char *_var, char *_buf)
{
	/*
	 * The applications only passes part of variable name. Append the configuration
	 * module name 'shifu' to it and read the value from the UCI system
	 */
	char cmd[CMD_MAC_CH] = "uci get shifu.conf.";
	strcat(cmd, _var);
	
	FILE *fp = NULL;
	
	fp = popen(cmd, "r");
	if (fp == NULL) {
		init_log();
		syslog(LOG_NOTICE, "Failed to read configuration from Shifu UCI");
		closelog();
		exit(-1);
	}

	/* Read the output one line at a time. */
	while(fgets(_buf, BUF_MAX, fp) != NULL);
	
	/* close */
	pclose(fp);
	
	if(!strlen(_buf))
	{
		syslog(LOG_NOTICE, "The Shifu UCI Value is not defined");
		return -1;
	}
	else
	{
		remove_char_from_string('\n',_buf);
		return 0;
	}
}


int get_fw_env(char *var,char *buf) {
	int rc=__get_fw_env(var,buf);
	remove_char_from_string('\n',buf);
	return rc;
}

/*set_fw_env method will set the value into fw env variables, and returns -1 if faild*/
int set_fw_env(char *var,char *val)
{
	/*cmd is a char array which will stores the cmd with variable and value*/ 
	char cmd[CMD_MAC_CH]="fw_setenv ";	 /* fw_setenv DevicID SHIFU-THOME000001*/		
	strcat(cmd, var);	/*we concating cmd variable and value*/
	strcat(cmd, " ");
	strcat(cmd,val);
	
	/*passing command to set fw env */
	if ( system(cmd) < 0 )  /*we are checking for success if not exit with -1*/
	{
		init_log();
		syslog(LOG_NOTICE, "FEV:failed in set_fw_env");
		closelog();
		exit(-1);
	}
	return 0;
}

/* Set Shifu specific configuration to UCI system */
int set_shifu_uci_param(char *var, char *val)
{
	char cmd[CMD_MAC_CH]="uci set shifu.conf.";
	strcat(cmd, var);
	strcat(cmd, "=");
	strcat(cmd, val);
	
	if (system(cmd) < 0 )
	{
		init_log();
		syslog(LOG_NOTICE, "Failed to configure Shifu UCI parameter");
		closelog();
		exit(-1);
	}
	system("uci commit shifu");
	return 0;
}