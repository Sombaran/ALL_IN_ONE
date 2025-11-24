.PHONY: toolchain emberlib 

all: toolchain emberlib 


export TOP_PWD = $(shell pwd)
export BUILD_VITA=1 

export COMPILER = $(CC) 
export LINK     = $(CC)
export ARCHIVE  = ar
export USE_DEVICE_PHILIO=1
export XONXOFF=1
export BUILD_VITA_OS=1

export DEVICE_PHILIO=1

export NO_READLINE=1

export COMPILER_DEFINES=-DGEMTEK_CALLBACK -fPIC

emberlib:
	$(MAKE) -C v6.0/app/builder/Z3GatewayHost_2/
ifdef GENERATE_LIBRARY
	test -d lib || mkdir -p lib
	cp v6.0/app/builder/Z3GatewayHost_2/build/exe/Z3GatewayHost_2.a lib/libZ3GatewayHost_2.a
else
	test -d bin || mkdir -p bin
	cp v6.0/app/builder/Z3GatewayHost_2/build/exe/Z3GatewayHost_2 bin/Z3GatewayHost_2
	chmod 755 bin/Z3GatewayHost_2
endif


