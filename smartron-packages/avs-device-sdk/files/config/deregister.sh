/etc/init.d/va_service stop
uci set shifu.conf.ALEXA_REGISTRATION_STATUS=not-registered && uci commit shifu
uci set shifu.conf.SKIP_ALEXA_REGISTRATION=false && uci commit shifu
kill -SIGUSR2 `ps | grep Mobile_Agent | grep -v grep | awk '{print $1}'`
rm -rf /root/va_data/alexa/*.db
/etc/init.d/va_service start
