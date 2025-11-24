################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/device_manager_gateway.cpp \
../src/dm_framework.cpp \
../src/dm_led_interface.cpp \
../src/dm_platform.cpp \
../src/dm_req_res_mapping.cpp \
../src/dm_request_parse.cpp \
../src/dm_request_queue.cpp \
../src/dm_response_queue.cpp \
../src/dm_rule_mapping.cpp \
../src/dm_secne_mapping.cpp \
../src/dm_send_data_tronx.cpp \
../src/dm_thing.cpp \
../src/dm_thingmap.cpp \
../src/dm_things_backup_db.cpp \
../src/dm_utility.cpp \
../src/dm_zeromq.cpp \
../src/ip_camera_framework.cpp \
../src/zb_framework.cpp \
../src/zb_mapping_database.cpp \
../src/zb_message_queue.cpp \
../src/zw_framework.cpp \
../src/zw_mapping_database.cpp \
../src/zw_message_queue.cpp \
../src/dm_buzzerplayer.cpp

OBJS += \
./src/device_manager_gateway.o \
./src/dm_framework.o \
./src/dm_led_interface.o \
./src/dm_platform.o \
./src/dm_req_res_mapping.o \
./src/dm_request_parse.o \
./src/dm_request_queue.o \
./src/dm_response_queue.o \
./src/dm_rule_mapping.o \
./src/dm_secne_mapping.o \
./src/dm_send_data_tronx.o \
./src/dm_thing.o \
./src/dm_thingmap.o \
./src/dm_things_backup_db.o \
./src/dm_utility.o \
./src/dm_zeromq.o \
./src/ip_camera_framework.o \
./src/zb_framework.o \
./src/zb_mapping_database.o \
./src/zb_message_queue.o \
./src/zw_framework.o \
./src/zw_mapping_database.o \
./src/zw_message_queue.o \
./src/dm_buzzerplayer.o

CPP_DEPS += \
./src/device_manager_gateway.d \
./src/dm_framework.d \
./src/dm_led_interface.d \
./src/dm_platform.d \
./src/dm_req_res_mapping.d \
./src/dm_request_parse.d \
./src/dm_request_queue.d \
./src/dm_response_queue.d \
./src/dm_rule_mapping.d \
./src/dm_secne_mapping.d \
./src/dm_send_data_tronx.d \
./src/dm_thing.d \
./src/dm_thingmap.d \
./src/dm_things_backup_db.d \
./src/dm_utility.d \
./src/dm_zeromq.d \
./src/ip_camera_framework.d \
./src/zb_framework.d \
./src/zb_mapping_database.d \
./src/zb_message_queue.d \
./src/zw_framework.d \
./src/zw_mapping_database.d \
./src/zw_message_queue.d \
./src/dm_buzzerplayer.d

# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	$(CXX) -std=c++0x -fpermissive -I../include  -I$(STAGING_DIR)/usr/include/ -lgstreamer-1.0  `pkg-config --cflags --libs glib-2.0 gstreamer-1.0` -O0 -g3 -Wall -c $(DEFINES) -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


