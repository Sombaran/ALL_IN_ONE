all: libsm_app.so sm_test_client

libsm_app.so : sm_iface.c
	$(CC) -c -Wall -Werror -fpic sm_iface.c -lubus -levent -lpthread -levent_pthreads -lblobmsg_json -lubox -ljson-c -shared -o libsm_app.so

sm_test_client : sm_test_client.c
	$(CC) -L. sm_test_client.c -o sm_test_client -lfw-env -lubus -levent -lpthread -levent_pthreads -lblobmsg_json -lubox -lsm_app -ljson-c

clean:
	rm -rf *.so
	rm -rf sm_test_client
