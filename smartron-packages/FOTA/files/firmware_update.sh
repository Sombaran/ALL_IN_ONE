#!/bin/sh
magic=$(strings  /root/downloads/fw/$1  | head -n1)

if [ "$magic" = "SMRTHUB" ]; then
        filesize=$(stat -c%s /root/downloads/fw/$1)
        offset=64
        sizetocopy=$(( ${filesize}-64-128))

        #recalculate the skip and size for bs=16
        offset=$((offset/16))
        sizetocopy=$((sizetocopy/16))

        #echo filesioe=$filesize, offset=$offset, sizetocopy=$sizetocopy
        dd if=/root/downloads/fw/$1 of=/tmp/firmware.img bs=16 skip=$offset count=$sizetocopy
else
        cp /root/downloads/fw/$1 /tmp/firmware.img 2>/dev/null
fi

if [ $? -ne 0 ]; then
	echo "not copied"
	exit 1
fi
echo "Copied"
killall dropbear uhttpd
/etc/init.d/bluetoothd stop
/etc/init.d/hwctrl_service stop
/etc/init.d/led-service stop
/etc/init.d/va_service stop
sleep 1

/sbin/sysupgrade -v /tmp/firmware.img
exit 0
#exit
