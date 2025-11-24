main_server:
	$(CC) $(LDFLAGS) main_server.c -o sm_app -O0 -g -lubus -lubox -lblobmsg_json -levent -lfw-env -ljson-c

clean:
	rm -rf *.so
	rm -rf sm_app
