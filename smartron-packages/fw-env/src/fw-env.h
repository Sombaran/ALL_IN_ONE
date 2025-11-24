#include "uci_alsa_config.h"

#ifndef __FW_ENV_H
#define __FW_ENV_H
/************************************************************************************************************
* FILENAME :       fw-env.h
*
* DESCRIPTION :
*			This header file is contain, API's to get or set fw_env variables using C program.
*			these functions are like string functions. we should not to pass any null values.
*
*			int get_fw_env(char *var,char *buf);
*			Return value is zero on successfully, if not successfully -1. var is a char pointer we have to
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
*			eg; to compile appilication use as below. for reference, compile fw-env-app2 using make menuconfig.
*				$(CC) test-env.c $(LDFLAGS) -o test-env -lfw-env
*
*
* AUTHOR :  Janardhanachari.kella
*
* VERSION:  0.2
*
* Lastmodified: janardhanachari.kella@smartron.com, 31/05/2018
*
* Copyright (C) 2018 Smartron India Private Limited - All Rights Reserved
*
**********************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

void remove_char_from_string(char c, char *str);

int get_fw_env(char *var,char *buf);

int set_fw_env(char *var,char *val);
int get_shifu_uci_param(char *_var, char *_buf);
int set_shifu_uci_param(char *var, char *val);

#ifdef __cplusplus
}
#endif
#endif
