all: cron_handler shifu_cron_event

cron_handler: cron_handler.o
	$(CC) $(LDFLAGS) -g -I $(STAGING_DIR)/usr/include/ -L ../lib cron_handler.o -lzmq -ljson-c -lsqlite3 -lfw-env -o cron_handler

cron_handler.o: cron_handler.c
	$(CC) $(LDFLAGS) -g -c cron_handler.c

shifu_cron_event: shifu_cron_event.o
	$(CC) $(LDFLAGS) -g -I $(STAGING_DIR)/usr/include/ -L ../lib shifu_cron_event.o -lzmq -ljson-c -o shifu_cron_event

shifu_cron_event.o: shifu_cron_event.c
	$(CC) $(LDFLAGS)  -g -c shifu_cron_event.c

# remove object files and executable when user executes "make clean"
clean:
	rm *.o cron_handler shifu_cron_event


