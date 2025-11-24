ExternalProject_Add(
 libpcap
 #URL http://www.tcpdump.org/release/libpcap-1.5.3.tar.gz
 DOWNLOAD_COMMAND cp -rf ${PROJECT_SOURCE_DIR}/udprelay/files/libpcap ${PROJECT_SOURCE_DIR}/udprelay/libpcap-prefix/src/
 CONFIGURE_COMMAND ./configure CC=${CMAKE_C_COMPILER} LDFLAGS=${CMAKE_C_FLAGS} CFLAGS=${CMAKE_C_FLAGS} --target=arm-openwrt-linux --host=arm-openwrt-linux --build=x86_64-linux-gnu --with-pcap=linux --enable-canusb=no --enable-dbus=no --without-flex --without-libnl --prefix=${CMAKE_BINARY_DIR}/stage
 BUILD_IN_SOURCE 1
)

add_definitions( -D_GNU_SOURCE )

include_directories( ${CMAKE_BINARY_DIR}/stage/include )
link_directories( ${CMAKE_BINARY_DIR}/stage/lib )

add_executable( udprelay udprelay.c)
target_link_libraries( udprelay libpcap.a )
