all: led-server wps_trigger

led-server: led.c status.c
	$(CC) led.c status.c $(LDFLAGS) -L./ -I./include -o led-server -lpthread -lfw-env -lpthread

wps_trigger: wps_trigger.c
	$(CC) wps_trigger.c $(LDFLAGS) -o wps_trigger -llediface

clean:
	rm -rf led-server
	rm -rf wps_trigger
