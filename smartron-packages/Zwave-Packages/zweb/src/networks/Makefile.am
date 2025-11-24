bin_PROGRAMS = zcgi

AVRO_SCHEMA_DIR = ../avro/schemas

sysconf_DATA = \
	${AVRO_SCHEMA_DIR}/zwave.avsc

zcgi_CFLAGS = \
	-Wall \
	-I./util/ \
	-I./avi/ \
	-I./service/ \
	-I./messages/scenes/ \
	-I./messages/ifttt/ \
	-I./ifttt/ \
	@FCGI_CFLAGS@ \
	-I@AVRO_PATH@/include/ \
	-DZW_AVRO_SCHEMA_DIR='"$(sysconfdir)"'

zcgi_LDADD = \
	@FCGI_LIBS@ \
	@AVRO_PATH@/lib/libavro.a

zcgi_SOURCES = \
	zcgi.c \
	zws_portal.c \
	zws_memcache.c \
	zws_session.c \
	zws_file.c \
	service/zws_cgi.c \
	service/zws_cgi_service.c \
	service/zws_cgi_service_networks.c \
	service/zws_cgi_service_nodes.c \
	service/zws_cgi_service_endpoints.c \
	service/zws_cgi_service_interfaces.c \
	service/zws_cgi_common.c \
	service/zws_cgi_common_networks.c \
	ifttt/zws_cgi_service_ifttt.c\
	avi/zw_avro.c \
	avi/zws_avi.c \
	avi/zws_avi_message.c \
	avi/zws_avi_message_networks.c \
	avi/zws_avi_message_nodes.c \
	avi/zws_avi_message_endpoints.c \
	avi/zws_avi_message_interfaces.c \
	messages/networks/zws_avi_networks_exit.c \
	messages/networks/zws_avi_networks_info_get.c \
	messages/networks/zws_avi_networks_node_list.c \
	messages/networks/zws_avi_networks_node_endpoint_list.c \
	messages/networks/zws_avi_networks_operations.c \
	messages/networks/zws_avi_networks_operation_state.c \
	messages/networks/zws_avi_networks_summary.c \
	messages/networks/zws_avi_networks_version.c \
	messages/networks/zws_avi_networks_whitelist.c \
	messages/nodes/zws_avi_nodes_command_queue.c \
	messages/nodes/zws_avi_nodes_endpoint_list.c \
	messages/endpoints/zws_avi_endpoints_interface_list.c \
	messages/endpoints/zws_avi_endpoints_node_naming.c \
	messages/interfaces/zws_avi_interfaces_alarm.c \
	messages/interfaces/zws_avi_interfaces_association_grp_info.c \
	messages/interfaces/zws_avi_interfaces_association.c \
	messages/interfaces/zws_avi_interfaces_basic.c \
	messages/interfaces/zws_avi_interfaces_battery.c \
	messages/interfaces/zws_avi_interfaces_central_scene.c \
	messages/interfaces/zws_avi_interfaces_configuration.c \
	messages/interfaces/zws_avi_interfaces_door_lock.c \
	messages/interfaces/zws_avi_interfaces_firmware_update_md.c \
	messages/interfaces/zws_avi_interfaces_meter.c \
	messages/interfaces/zws_avi_interfaces_sensor_binary.c \
	messages/interfaces/zws_avi_interfaces_sensor_multilevel.c \
	messages/interfaces/zws_avi_interfaces_switch_binary.c \
	messages/interfaces/zws_avi_interfaces_switch_multilevel.c \
	messages/interfaces/zws_avi_interfaces_switch_color.c \
	messages/interfaces/zws_avi_interfaces_thermostat_fan_mode.c \
	messages/interfaces/zws_avi_interfaces_thermostat_fan_state.c\
	messages/interfaces/zws_avi_interfaces_thermostat_mode.c \
	messages/interfaces/zws_avi_interfaces_thermostat_operating_state.c\
	messages/interfaces/zws_avi_interfaces_thermostat_setpoint.c\
	messages/interfaces/zws_avi_interfaces_user_code.c \
	messages/interfaces/zws_avi_interfaces_barrier_operator.c \
	messages/interfaces/zws_avi_interfaces_wake_up.c \
	messages/interfaces/zws_avi_interfaces_zip_gateway.c \
	messages/interfaces/zws_avi_interfaces_zip_portal.c \
	messages/scenes/zws_avi_scene.c \
	messages/scenes/zws_avi_scene_save.c \
	messages/scenes/zws_avi_scene_list.c \
	messages/scenes/zws_avi_scene_get_sup.c \
	messages/scenes/zws_avi_security_scene_common.c \
	messages/scenes/zws_avi_security_scene_list.c \
	messages/scenes/zws_avi_security_scene_save.c \
	messages/scenes/zws_avi_security_scene_get.c \
	messages/scenes/zws_avi_security_scene_del.c \
	messages/scenes/zws_avi_security_scene_set_state.c \
	messages/scenes/zws_avi_security_scene_get_sup.c \
	util/zws_io.c \
	util/zws_log.c \
	util/zwu_number.c \
	util/zwu_time.c \
	util/zwu_array_list.c \
	util/zwu_string_buffer.c \
	util/zwu_encoding.c \
	util/zwu_inet.c \
	util/zws_string.c \
	util/zws_exec_php_script.c

#
# Z-Ware portal version only
#
if ZWARE_PORTAL
zcgi_CFLAGS += \
		@LIBMEMCACHED_CFLAGS@ \
		-I@CJSON_INC@
		
zcgi_LDADD += \
		@LIBMEMCACHED_LIBS@ \
		@CJSON_LIB@/libcjson.a

zcgi_SOURCES += \
	messages/networks/zws_avi_networks_gateway_status.c \
	messages/root/zws_avi_root_network_list.c \
	messages/root/zws_avi_root_network_refresh.c \
	ifttt/zws_avi_message_ifttt.c\
	messages/ifttt/zws_avi_ifttt_common.c \
	messages/ifttt/zws_avi_ifttt_status.c \
	messages/ifttt/zws_avi_ifttt_user_info.c \
	messages/ifttt/zws_avi_ifttt_test.c \
	messages/ifttt/zws_avi_ifttt_bsensor.c \
	messages/ifttt/zws_avi_ifttt_bswitch.c \
	messages/ifttt/zws_avi_ifttt_dlock.c \
	messages/ifttt/zws_avi_ifttt_mswitch.c \
	messages/ifttt/zws_avi_ifttt_thrmomode.c \
	messages/ifttt/zws_avi_ifttt_alarm.c \
	messages/ifttt/zws_avi_ifttt_msensor.c \
	util/zwu_json.c \
	util/zws_ifttt_response.c
endif

#
# Z-Ware local version only
#
if ZWARE_LOCAL

zcgi_CFLAGS +=  \
	-DLOCAL_CLIENT_ENABLE

zcgi_LDADD +=

zcgi_SOURCES += \
	messages/root/zws_avi_root_gateway_initialize.c

endif
