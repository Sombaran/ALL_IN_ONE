#!/bin/bash

#export CALL_USRNAME=`whoami`
#export PHP_SSID=`env | grep -o -P 'PHPSESSID=.{0,32}' | cut -c11-42`
#PHP_SSID="796c950badca724ff2d14bc52e53f2af"

PHP_SSID=`env | grep -o -P 'PHPSESSID=.{0,26}' | cut -c11-36`

if [[ ${#PHP_SSID} -eq 26 ]]
then
	#printf "Content-type: text/html\n\n"
	#printf "Permission Denied\n"
	#exit
	name="$(echo $PHP_SSID | ../../php/bin/php getusr.php)"
	if [[ -z "$name" ]]
	then
        	#printf "Content-type: text/html\n\n"
        	#printf "You don't have permission to execute this command\n"
        	#exit
        	unset REMOTE_USER
	else
        	export REMOTE_USER="$name"
	fi
else
	unset REMOTE_USER

fi

export CONTEXT_PREFIX="/cgi/"

#printf "Content-type: text/plain\n\n"
#env

../cgi-bin/zcgi


