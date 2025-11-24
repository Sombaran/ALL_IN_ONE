# build Python_BT executable when user executes "make"
Python_BT: Python_BT.o
	$(CXX) $(LDFLAGS) Python_BT.o -o Python_BT

Python_BT.o: Python_BT.cpp
	$(CXX) $(CXXLAGS) -c Python_BT.cpp

# remove object files and executable when user executes "make clean"
clean:
	rm *.o Python_BT

