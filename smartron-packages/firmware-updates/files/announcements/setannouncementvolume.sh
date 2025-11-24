#!/bin/bash
value="`uci get shifu.conf.AVSSPEAKERVOLUME`"
if [ $value '<' 0.900000 ]
then
	amixer -c0 sset DAC1 65
elif [ $value = 0.900000 ]
then
	amixer -c0 sset DAC1 68
else
	amixer -c0 sset DAC1 71
fi 
