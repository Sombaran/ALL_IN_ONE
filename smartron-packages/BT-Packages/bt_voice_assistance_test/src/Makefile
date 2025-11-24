# build bt_voice_assistance_testcpp executable when user executes "make"
avrcp_test: avrcp_test.o
	$(CC) avrcp_test.o -o avrcp_test -lzmq
	$(CC) va_bt_connect_test.o -o va_bt_connect_test -lzmq
	$(CC) va_bt_disconnect_test.o -o va_bt_disconnect_test -lzmq
	$(CC) va_bt_status.o -o va_bt_status -lzmq
	$(CC) va_bt_mediastatus.o -o va_bt_mediastatus -lzmq

avrcp_test.o: avrcp_test.c
	$(CC) -c avrcp_test.c 
	$(CC) -c va_bt_connect_test.c
	$(CC) -c va_bt_disconnect_test.c
	$(CC) -c va_bt_status.c
	$(CC) -c va_bt_mediastatus.c

# remove object files and executable when user executes "make clean"
clean:
	rm *.o avrcp_test
	rm *.o va_bt_connect_test
	rm *.o va_bt_disconnect_test
	rm *.o va_bt_status
	rm *.o va_bt_mediastatus


