# build ssdp_server executable when user executes "make"
daemon: 
	$(CC) $(LDFLAGS) ssdp_server.c lssdp.c -lfw-env -o ssdp_server
 
# remove object files and executable when user executes "make clean" 
clean:
	 rm *.o ssdp_server
