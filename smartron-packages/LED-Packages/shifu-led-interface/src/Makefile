# fw-env Confidential
# Copyright (C)2018 fw-env, Inc. http://smartron.com/
#
# fw-env GNU make build script

all: liblediface.so led-test-client
	
liblediface.so : led_iface.c 
	# compiling .c file to .so file
	$(CC) -c -Wall -Werror -fpic -O3 -I./include led_iface.c -shared -o liblediface.so
	#gcc -c -Wall -Werror -fpic -I./include led_iface.c -shared -o liblediface.so
	
led-test-client:led-test-client.c
	$(CC) -I./include -L. led-test-client.c -o led-test-client -llediface
	#gcc -I./include -L. led-client.c -o led-client -llediface
	
clean:
	rm -rf *.so
	rm -rf led-test-client
