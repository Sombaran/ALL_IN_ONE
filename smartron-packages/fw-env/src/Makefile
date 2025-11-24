#====================================================================#
# Copyright (c) 2018 Smartron India Pvt. Ltd. All rights reserved.
# fw-env GNU make build script
# Version:0.2
# Last modified: janardhanachari.kell@smartron.com, 07/02/2018
#====================================================================#

all: libfw-env.so

libfw-env.so : fw-env.c uci_alsa_config.c
	# compiling .c file to .so file
	$(CC) -Wall -Werror -fpic fw-env.c uci_alsa_config.c -shared -o libfw-env.so

clean:
	rm -rf *.o *.so
