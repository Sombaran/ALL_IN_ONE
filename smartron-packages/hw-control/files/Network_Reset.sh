#!/bin/sh

#[smartron][DVT][Tracker-ID:21064][Module-Name:WiFi]:Network and Throughput optimization for 2.4Ghz
uci set shifu.conf.NETWORK_RESET_INITIATED=1
uci commit shifu

network_reset() {
    rm -f /etc/config/network /etc/config/wireless

    wifi detect > /etc/config/wireless

    cp /rom/etc/config/repacd /etc/config/

    uci set dhcp.lan.ignore=0
    uci commit dhcp

    uci set firewall.@zone[1].input='REJECT'
    uci commit firewall

    # 21547: network file for first time flash start
    if [ -f /rom/etc/uci-defaults/network ];
    then
	    logger -p user.crit  "Executing /rom/etc/uci-defaults/network"
	    /rom/etc/uci-defaults/network
    else
	    logger -p user.crit  "Executing /rom/rom/etc/uci-defaults/network"
	    /rom/rom/etc/uci-defaults/network
    fi

    # Redmine:20336 : Clear flag status during network reset to maintain Shifu in default mode
    uci set shifu.conf.NETWORK_MODE='DEFAULT'
    uci set shifu.conf.MA_EBLR_FLAG='0'
    uci set shifu.conf.MA_ROUTER_FLAG='0'
    uci set shifu.conf.MA_ROUTER_SSID='NULL'
    uci set shifu.conf.MA_ROUTER_PASSWORD='NULL'
    uci set shifu.conf.MA_LEGACY_ROUTER_SSID='NULL'
    uci set shifu.conf.MA_LEGACY_ROUTER_PASSWORD='NULL'
    uci commit shifu
}

logger -p user.notice "[NETWORK-RESET]: Network reset initilized.."

/etc/init.d/hwctrl_service stop
/etc/init.d/va_service stop
/etc/init.d/led-service stop
/etc/init.d/ma_service stop

i2cset -y 0 0x18 0x47 0xFD 0xE8 0x00 0x00 0x00 0x00 0x78 0x00 0xff 0x00 0x00 0x00 0x50 0x20 i &

# #22366:Setting volume
#amixer -c0 sset DAC1 64
sh /announcements/setannouncementvolume.sh

gst-launch-1.0 playbin uri=file:///announcements/Network_reset.mp3 audio-sink=alsasink

#Use below code for network reset from Mobile agent
network_reset

# Reset router configuration Stop */
logger -p user.notice "[NETWORK-RESET]: Network reset completed! shifu is rebooting.."

i2cset -y 0 0x18 0x47 0xFD 0xE8 0x00 0x00 0x00 0x00 0x78 0x00 0xff 0x00 0x00 0x00 0x50 0x20 i
# #22366:Setting volume
#amixer -c0 sset DAC1 64
sh /announcements/setannouncementvolume.sh
gst-launch-1.0 playbin uri=file:///announcements/reboot.mp3 audio-sink=alsasink

uci set shifu.conf.NETWORK_RESET_INITIATED=0
uci commit shifu

reboot -f
