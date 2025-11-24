# build fota_update executable when user executes "make"
ifeq ($(CONFIG_BUILD_TYPE_RELEASE),y)
	FOTA_CFLAGS = -DBUILD_TYPE_RELEASE
endif
fota_update:
	$(CC)  $(FOTA_CFLAGS) -I $(STAGING_DIR)/usr/include/ -L ../lib  main.c fota.c -o fota_update -lcurl -ljson-c -lfw-env -lsqlite3 -lpthread -lssl -lcrypto
#	$(CC) -I $(STAGING_DIR)/usr/include/ -L ../lib  end_device.c fota.c -lcurl -ljson-c -lsqlite3 -lpthread -lfw-env -o end_device_upgrade
	$(CC) -I $(STAGING_DIR)/usr/include/ -L ../lib  client-dm.c -ljson-c -o fota_client_dm
	$(CC) -I $(STAGING_DIR)/usr/include/ -L ../lib  client-ma.c -ljson-c -o fota_client_ma

# remove object files and executable when user executes "make clean" 
clean:
	rm -f $(OBJS) $(PROC_OBJS)
	rm -f fota_update
	rm -f end_device_upgrade
	rm -f fota_client_dm
	rm -f fota_client_ma
