
cmake_minimum_required(VERSION 2.8)
project(zipgateway C )
include( ExternalProject )
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wparentheses -Wno-array-bounds -std=gnu99 -D_DEFAULT_SOURCE")
# Add gcov support
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_C_OUTPUT_EXTENSION_REPLACE 1)

add_definitions ( -DZIPGW -DDEBUG_S2_INCL_FSM -DDEBUG_S2_FSM)

if( NOT APPLE )
  if( ${CMAKE_SYSTEM_PROCESSOR} MATCHES "i686" OR  ${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64"  )
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
    set(CMAKE_SYSTEM_PROCESSOR "i386")
    message("Building 32bit ")
  endif()
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})


if ( NOT LibUSB_LIBRARIES)
   find_package(LibUSB 1.0)
endif()

if (NOT OPENSSL_LIBRARIES)
   if( APPLE )
     find_package(OpenSSL 1.0.0)
       set(OPENSSL_INCLUDE_DIRS ${OPENSSL_INCLUDE_DIR} )
   else ()
     pkg_search_module(OPENSSL REQUIRED openssl>=1.0.1 )
   endif()
endif()

find_package(BISON 3.0.4 REQUIRED)
find_package(FLEX REQUIRED)

if (BISON_FOUND)
  include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    )
  BISON_TARGET(pvsparser src/pvs_cfg.y ${CMAKE_CURRENT_SOURCE_DIR}/src-gen/pvs_cfg.tab.c VERBOSE)
  if (FLEX_FOUND)
    FLEX_TARGET(pvslexer src/pvs_cfg.l ${CMAKE_CURRENT_SOURCE_DIR}/src-gen/lex.yy.c )
    ADD_FLEX_BISON_DEPENDENCY(pvslexer pvsparser)
  endif(FLEX_FOUND)
endif(BISON_FOUND)


set(ZWAVE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Z-Wave/include )
# Include lib S2
set(LIBS2 "./libs2" )
set(LIBS2_INCLUDE_DIRS "${LIBS2}/include")
set(S2_LIBRARIES s2_controller s2crypto aes)
set(TS2 "${LIBS2}/transport_service")
set(TS2_SRC ${TS2}/transport_service2.c ${TS2}/transport2_fsm.c)
add_subdirectory( ${LIBS2} libs2 )

enable_testing()
if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" AND NOT ${BUILD_TESTING} )
  add_subdirectory( udprelay )
endif()
add_subdirectory( convert_eeprom )

set(GW_SRC
src-gen/Secure_learn.c
src/Bridge.c
src/CC_FirmwareUpdate.c
src/CC_InclusionController.c
src/command_handler.c
src/CC_Gateway.c
src/CC_NetworkManagement.c
src/CC_Portal.c
src/CC_PowerLevel.c
src/CC_ManufacturerSpecific.c
src/CC_Version.c
src/CC_ZWavePlus.c
src/CC_ZIPNaming.c
src/CC_Supervision.c
src/CC_InstalltionAndMaintenance.c
src/ClassicZIPNode.c
src/DTLS_server.c
src/Mailbox.c
src/RD_DataStore.c
src/RD_internal.c
src/ResourceDirectory.c
src/ZIP_Router.c
src/ZWFirmwareUpdate.c
src/ZW_ZIPApplication.c
src/ZW_tcp_client.c
src/ZW_udp_server.c
src/crc32alg.c
src/dhcpc2.c
src/ipv46_nat.c
src/ipv4_interface.c
src/mDNSService.c
src/node_queue.c
src/serial_api_process.c
src/serialapi/Serialapi.c
src/serialapi/conhandle.c
src/smalloc.c
src/transport/Security_Scheme0.c
src/transport/ZW_PRNG.c
src/transport/ZW_SendDataAppl.c
src/transport/ZW_SendRequest.c
src/transport/rijndael-alg-fst.c
src/transport/security_layer.c
src/transport/S2_wrap.c
src/transport/s2_keystore.c
src/zwdb.c
src/zwpgrmr/crc32.c
src/zwpgrmr/linux_serial_interface.c
src/zwpgrmr/linux_usb_interface.c
src/zwpgrmr/zpg.c
src/zwpgrmr/zpgp.c
src/CommandAnalyzer.c
src-gen/get_list.c
src/provisioning_list.c
src/pvs_parse_help.c
src/CC_provisioning_list.c
)


add_subdirectory( test )
set(TS2_INCLUDE_DIRS "${LIBS2}/transport_service")
#add_subdirectory( ${TS2} transport_service ) 

include(GetGitRevisionDescription)

git_describe(VERSION --tags)
#MESSAGE(STATUS "VERSION: " ${VERSION})

if (VERSION MATCHES "^ver")
#    MESSAGE( STATUS "found git tag")
    #parse the version information into pieces.
    string(REGEX REPLACE "^ver([0-9]+)_.*" "\\1" VERSION_MAJOR "${VERSION}")
    string(REGEX REPLACE "^ver[0-9]+_([0-9]+).*" "\\1" VERSION_MINOR "${VERSION}")
    string(REGEX REPLACE "^ver[0-9]+_[0-9]+-(.*)" "\\1" VERSION_SHA1 "${VERSION}")
 #    MESSAGE( STATUS "Using pkgconfig.h CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})
    string(REGEX REPLACE "^ver[0-9]+_[0-9]+-([0-9]+)-.*" "\\1" VERSION_PATCH "${VERSION}")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pkgconfig.h.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/pkgconfig.h)

    add_custom_command( OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src-gen/get_list.c 
    COMMAND
    svn cat https://zensys14/svn/anicca/trunk/Libraries/ZWaveDll/XmlFiles/ZWave_custom_cmd_classes.xml |
     ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/utils/extract_get.py -
    > ${CMAKE_CURRENT_SOURCE_DIR}/src-gen/get_list.c )
else()
    execute_process(COMMAND grep PACKAGE_VERSION_MAJOR ../pkgconfig.h COMMAND awk "{print $3}" COMMAND tr -d "[[:space:]]" OUTPUT_VARIABLE VERSION_MAJOR)
    execute_process(COMMAND grep PACKAGE_VERSION_MINOR ../pkgconfig.h COMMAND awk "{print $3}" COMMAND tr -d "[[:space:]]" OUTPUT_VARIABLE VERSION_MINOR)
    set(VERSION_SHA1 "release" )
    set(VERSION_PATCH "0" )
endif()
MESSAGE(STATUS "VERSION_MAJOR: " ${VERSION_MAJOR})
MESSAGE(STATUS "VERSION_MINOR: " ${VERSION_MINOR})
MESSAGE(STATUS "VERSION_SHA1: " ${VERSION_SHA1})
MESSAGE(STATUS "VERSION_PATCH: " ${VERSION_PATCH})

set(VERSION_SHORT "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_SHA1}")
if( ${VERSION_SHA1} MATCHES "^ver" )
  set(VERSION_SHA1 "release" )
  set(VERSION_PATCH "0" )
endif()

option(DEBUG_ALLOW_NONSECURE "Allow for non-secure Z/IP connetions on port 4123" OFF)
option(NO_ZW_NVM "Do not use Z-Wave module NVM")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/doc/Doxyfile.in ${CMAKE_CURRENT_SOURCE_DIR}/src/doc/Doxyfile)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/doc/main.dox.in ${CMAKE_CURRENT_SOURCE_DIR}/src/doc/main.dox)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/files/zipgateway.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/files/zipgateway)


set(CONTIKI contiki )

include_directories( 
${OPENSSL_INCLUDE_DIRS}
${LibUSB_INCLUDE_DIRS}
${LIBS2_INCLUDE_DIRS}
${TS2_INCLUDE_DIRS}
Z-Wave/include 
${CONTIKI}/platform/linux 
${CONTIKI}/platform/linux/dev 
${CONTIKI}/cpu/native/. 
${CONTIKI}/cpu/native/net 
src/serialapi 
src-gen 
src/zwpgrmr 
src/transport
${CONTIKI}/core/dev 
${CONTIKI}/core/lib 
${CONTIKI}/core/net 
${CONTIKI}/core/sys 
${CONTIKI}/core 
src 
.
)


add_definitions( -DPROJECT_CONF_H=\"project-conf.h\" -DSERIAL_LOG -DSUPPORTS_MDNS 
-DSUPPORTS_MDNS -DSECURITY_SUPPORT -DCONTROLLER_CHANGE_SUPPORT -D__ROUTER_VERSION__
-DCONTIKI_TARGET_LINUX -DUIP_CONF_IPV6=1 -DAUTOSTART_ENABLE  -DPROVISIONING_LIST_SIZE=232)

set(CONTIKI_SRC
${CONTIKI}/core/dev/serial-line.c
${CONTIKI}/core/lib/crc16.c
${CONTIKI}/core/lib/list.c
${CONTIKI}/core/lib/memb.c
${CONTIKI}/core/lib/random.c
${CONTIKI}/core/lib/ringbuf.c
${CONTIKI}/core/net/resolv.c
${CONTIKI}/core/net/tcpip.c
${CONTIKI}/core/net/uip-debug.c
${CONTIKI}/core/net/uip-ds6.c
${CONTIKI}/core/net/uip-icmp6.c
${CONTIKI}/core/net/uip-nd6.c
${CONTIKI}/core/net/uip-packetqueue.c
${CONTIKI}/core/net/uip-split.c
${CONTIKI}/core/net/uip-udp-packet.c
${CONTIKI}/core/net/uip6.c
${CONTIKI}/core/net/uiplib.c
${CONTIKI}/core/sys/autostart.c
${CONTIKI}/core/sys/ctimer.c
${CONTIKI}/core/sys/etimer.c
${CONTIKI}/core/sys/process.c
${CONTIKI}/core/sys/procinit.c
${CONTIKI}/core/sys/stimer.c
${CONTIKI}/core/sys/timer.c
${CONTIKI}/cpu/native/net/tapdev-drv.c
${CONTIKI}/platform/linux/./clock.c
${CONTIKI}/platform/linux/./contiki-main.c
${CONTIKI}/platform/linux/./parse_config.c
${CONTIKI}/core/lib/assert.c
${CONTIKI}/core/net/tcpip_ipv4.c
${CONTIKI}/core/net/uip-ipv4-split.c
${CONTIKI}/core/net/uip-ipv4-udp-packet.c
${CONTIKI}/core/net/uip.c
${CONTIKI}/core/net/uip_arp.c
${CONTIKI}/cpu/native/linux-serial.c
${CONTIKI}/cpu/native/net/tapdev6.c
${CONTIKI}/platform/linux/File_eeprom.c



)

message (STATUS "openssl libraries: ${OPENSSL_LIBRARIES}")
message (STATUS "libsusb libraries: ${LibUSB_LIBRARIES}")
add_executable(zipgateway ${GW_SRC} ${CONTIKI_SRC} ${TS2_SRC} ${BISON_pvsparser_OUTPUTS} ${FLEX_pvslexer_OUTPUTS})
if( APPLE )
  target_link_libraries(zipgateway ${S2_LIBRARIES} ${OPENSSL_LIBRARIES} ${LibUSB_LIBRARIES})
else()
  target_link_libraries(zipgateway ${S2_LIBRARIES} rt ${OPENSSL_LIBRARIES} ${LibUSB_LIBRARIES})
endif()

add_custom_target(doc
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/doc
                  COMMAND doxygen
                  COMMAND roffit < zipgateway.3 > html/zip.html
                 )

add_custom_target(beaglebone
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/beaglebone/create_beaglebone_tarball.sh ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${VERSION_MAJOR} ${VERSION_MINOR}
                 )
install(TARGETS zipgateway
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/sbin
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/static)

install(FILES files/zipgateway DESTINATION /etc/init.d PERMISSIONS  OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
  GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(FILES src/doc/zipgateway.3 DESTINATION /usr/local/man/man3 PERMISSIONS  OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
  GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(FILES files/zipgateway.tun DESTINATION ${CMAKE_INSTALL_PREFIX}/etc PERMISSIONS  OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
  GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(FILES files/zipgateway_provisioning_list.cfg DESTINATION ${CMAKE_INSTALL_PREFIX}/etc PERMISSIONS  OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
  GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(FILES files/zipgateway.fin DESTINATION ${CMAKE_INSTALL_PREFIX}/etc PERMISSIONS  OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
  GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(FILES WRTpackage/files/Portal.ca_x509.pem DESTINATION ${CMAKE_INSTALL_PREFIX}/etc PERMISSIONS  OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
  GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(FILES WRTpackage/files/ZIPR.x509_1024.pem DESTINATION ${CMAKE_INSTALL_PREFIX}/etc PERMISSIONS  OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
  GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(FILES WRTpackage/files/ZIPR.key_1024.pem DESTINATION ${CMAKE_INSTALL_PREFIX}/etc PERMISSIONS  OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
  GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
if(NOT ${BUILD_TESTING})
install(FILES ${CMAKE_BINARY_DIR}/udprelay/udprelay DESTINATION ${CMAKE_INSTALL_PREFIX}/sbin PERMISSIONS  OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
  GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endif()
install(FILES ${CMAKE_BINARY_DIR}/convert_eeprom/zgw_convert_eeprom DESTINATION ${CMAKE_INSTALL_PREFIX}/sbin PERMISSIONS  OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
  GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)


install(FILES files/zipgateway.cfg DESTINATION ${CMAKE_INSTALL_PREFIX}/etc)
install(FILES beaglebone/radvd.conf DESTINATION /etc/)

install(CODE "execute_process(COMMAND sh -c \"echo ${CMAKE_INSTALL_PREFIX}/etc/zipgateway.cfg > ${CMAKE_CURRENT_SOURCE_DIR}/files/scripts/conffiles\")")

execute_process(COMMAND basename $ENV{PWD} COMMAND tr -d "[[:space:]]" OUTPUT_VARIABLE cwd)
#MESSAGE( STATUS " CMAKE_BINARY_DIR: " ${cwd})
SET(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_SOURCE_GENERATOR "TBZ2")
SET(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
SET(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}-${CMAKE_SYSTEM_NAME}-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Sigmadesigns") #required
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libusb-1.0-0,libssl1.0.0,radvd,parprouted,bridge-utils" )
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/files/scripts/postinst;${CMAKE_CURRENT_SOURCE_DIR}/files/scripts/config;${CMAKE_CURRENT_SOURCE_DIR}/files/scripts/postrm;${CMAKE_CURRENT_SOURCE_DIR}/files/scripts/templates;${CMAKE_CURRENT_SOURCE_DIR}/files/scripts/conffiles" )
SET(CPACK_SOURCE_IGNORE_FILES 
"/build.*/"
"/jni/"
".*/.git.*$"
"/utils/"
"/src/scxml/"
"/src/old/"
"/contiki/platform/ax110xx/"
"/contiki/platform/lm3s6964/"
"/src/SigmaContiki.vcproj"
"/autom4te.cache/" 
"/configure.ac"
"/.ctags"
"/.gitignore"
"/.project"
"/.cproject"
"/create_dist_source.sh"
"/README.create_dist_source"   
"/${cwd}"
"/pkgconfig.h.cmake.in"
)
INCLUDE(CPack)

