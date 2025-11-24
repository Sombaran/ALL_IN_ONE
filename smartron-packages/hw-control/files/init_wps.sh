#!/bin/sh

if [ -f /tmp/wps.lock ]
then
    logger -p user.notice  "[WPS]:already in progress.."
    return
else
    touch /tmp/wps.lock

    wps_trigger

    hostapd_cli -p /var/run/hostapd-wifi0 wps_pbc

    hostapd_cli -p /var/run/hostapd-wifi1 wps_pbc

    rm /tmp/wps.lock
fi
