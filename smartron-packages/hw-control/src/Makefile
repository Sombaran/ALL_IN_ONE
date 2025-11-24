all: hw-control

hw-control:
	$(CC) $(LDFLAGS) btn-control.c volume-api.c hw_config.c -lpthread -llediface -lfw-env -o hw-control

clean:
	rm -f $(OBJS) $(PROC_OBJS)
	rm -f hw-control
