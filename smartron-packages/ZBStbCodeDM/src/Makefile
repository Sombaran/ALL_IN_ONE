# build ZBStbCodeDM executable when user executes "make"
ZBStbCodeDM: ZBStbCodeDM.o
	$(CC) $(LDFLAGS) ZBStbCodeDM.o -o ZBStbCodeDM 

ZBStbCodeDM.o: ZBStbCodeDM.c
	$(CC) $(CFLAGS) -c ZBStbCodeDM.c  

# remove object files and executable when user executes "make clean"
clean:
	rm *.o ZBStbCodeDM

