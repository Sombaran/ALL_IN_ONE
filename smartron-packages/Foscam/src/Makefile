# build Foscam executable when user executes "make"
all: libFoscamForDM Foscam

libFoscamForDM:
	$(CC) $(LDFLAGS) -shared -o libFoscamForDM.so -fPIC Foscam.c foscam_sqlite3.c Foscam_Socket.c -ljson-c -lsqlite3 -lfw-env

Foscam: 
	$(CC) main.c -o Foscam  -ljson-c  -L . libFoscamForDM.so


# remove object files and executable when user executes "make clean"
clean:
	rm *.o Foscam

