bin_PROGRAMS = zwportald

SYSCONFIGDIR = ../etc

AVRO_SCHEMA_DIR = ../../avro/schemas

APR_LIB=`@APR_CONFIG@ --prefix`/lib
APR_UTIL_LIB=`@APR_UTIL_CONFIG@ --prefix`/lib

localstate_DATA =

install-data-local:
	@$(NORMAL_INSTALL)
	$(MKDIR_P) "$(DESTDIR)$(localstatedir)/networks"
	$(MKDIR_P) "$(DESTDIR)$(localstatedir)/log"
	$(MKDIR_P) "$(DESTDIR)$(localstatedir)/firmwares"
	$(MKDIR_P) "$(DESTDIR)$(localstatedir)/ifttt"

sysconf_DATA = \
	$(SYSCONFIGDIR)/zwportald.conf \
	$(SYSCONFIGDIR)/zwave_device_rec.txt \
	$(SYSCONFIGDIR)/device_rec_constants.txt \
	$(SYSCONFIGDIR)/zwave.avsc

zwportald_CFLAGS = \
	-Wall \
	-DOS_LINUX \
	-D_LARGEFILE64_SOURCE \
	-DLDAP_DEPRECATED=1 \
	-I./util/ \
	-I./avi/ \
	-I./service/ \
	-I./ifttt/ \
	-I./messages/scenes/ \
	-I./messages/ifttt/ \
	-I@HCAPI_INC@ \
	-I@LIBCONFIG_INC@ \
	@EXPAT_CFLAGS@ \
	@UDUNITS_CFLAGS@ \
	@LIBESMTP_CFLAGS@ \
	@LIBZWSCENE_CFLAGS@ \
	@ZLIB_CFLAGS@ \
	@LIBFFI_CFLAGS@ \
	-I@SIMCLIST_INC@ \
	-I@LDAP_INC@ \
	-I@AVRO_PATH@/include/ \
	`@APR_CONFIG@ --includes` \
	`@APR_UTIL_CONFIG@ --includes` \
	-DZWP_SYSCONFDIR='"$(sysconfdir)"'

zwportald_LDFLAGS = \
	-pthread \
	-L$(APR_LIB) -Wl,-rpath -Wl,$(APR_LIB) \
	-L$(APR_UTIL_LIB) -Wl,-rpath -Wl,$(APR_UTIL_LIB) \
	@EXPAT_LIBS@ \
	@UDUNITS_LIBS@ \
	@LIBESMTP_LIBS@ \
	@LIBZWSCENE_LIBS@ \
	@ZLIB_LIBS@ \
	@LIBFFI_LIBS@


zwportald_LDADD = \
	@HCAPI_LIB_HCAPI@ \
	@HCAPI_LIB_ZWAPI@ \
	@LIBCONFIG_LIB@/libconfig.a \
	@AVRO_PATH@/lib/libavro.a \
	-lrt \
	-lm \
	-lapr-1 \
	-laprutil-1 \
	-lzwscene \
	-lglib-2.0 \
	-lz \
	-lffi \
	-ludunits2 \
	-lesmtp \
	-lexpat

zwportald_SOURCES = \
	zwportald.c \
	zwp_portal.c \
	zwp_network.c \
	zwp_interface.c \
	zwp_node_state.c \
	zwp_interface_state.c \
	zwp_descriptor.c \
	zwp_config.c \
	zwp_hcapi.c \
	zwp_hcapi_aal.c \
	zwp_hcapi_zwave.c \
	zwp_server.c \
	zwp_client.c \
	service/zwp_avi_service.c \
	service/zwp_avi_service_networks.c \
	service/zwp_avi_service_nodes.c \
	service/zwp_avi_service_endpoints.c \
	service/zwp_avi_service_interfaces.c \
	avi/zw_avro.c \
	avi/zwp_avi.c \
	avi/zwp_avi_message.c \
	avi/zwp_avi_message_networks.c \
	avi/zwp_avi_message_nodes.c \
	avi/zwp_avi_message_endpoints.c \
	avi/zwp_avi_message_interfaces.c \
	messages/networks/zwp_avi_networks_exit.c \
	messages/networks/zwp_avi_networks_info_get.c \
	messages/networks/zwp_avi_networks_node_list.c \
	messages/networks/zwp_avi_networks_node_endpoint_list.c \
	messages/networks/zwp_avi_networks_operations.c \
	messages/networks/zwp_avi_networks_operation_state.c \
	messages/networks/zwp_avi_networks_summary.c \
	messages/networks/zwp_avi_networks_version.c \
	messages/networks/zwp_avi_networks_whitelist.c \
	messages/nodes/zwp_avi_nodes_command_queue.c \
	messages/nodes/zwp_avi_nodes_endpoint_list.c \
	messages/endpoints/zwp_avi_endpoints_interface_list.c \
	messages/endpoints/zwp_avi_endpoints_node_naming.c \
	messages/interfaces/zwp_avi_interfaces_alarm.c \
	messages/interfaces/zwp_avi_interfaces_association_grp_info.c \
	messages/interfaces/zwp_avi_interfaces_association.c \
	messages/interfaces/zwp_avi_interfaces_basic.c \
	messages/interfaces/zwp_avi_interfaces_battery.c \
	messages/interfaces/zwp_avi_interfaces_central_scene.c \
	messages/interfaces/zwp_avi_interfaces_configuration.c \
	messages/interfaces/zwp_avi_interfaces_door_lock.c \
	messages/interfaces/zwp_avi_interfaces_firmware_update_md.c \
	messages/interfaces/zwp_avi_interfaces_meter.c \
	messages/interfaces/zwp_avi_interfaces_sensor_binary.c \
	messages/interfaces/zwp_avi_interfaces_sensor_multilevel.c \
	messages/interfaces/zwp_avi_interfaces_switch_binary.c \
	messages/interfaces/zwp_avi_interfaces_switch_multilevel.c \
	messages/interfaces/zwp_avi_interfaces_switch_color.c \
	messages/interfaces/zwp_avi_interfaces_thermostat_fan_mode.c \
	messages/interfaces/zwp_avi_interfaces_thermostat_fan_state.c\
	messages/interfaces/zwp_avi_interfaces_thermostat_mode.c \
	messages/interfaces/zwp_avi_interfaces_thermostat_operating_state.c\
	messages/interfaces/zwp_avi_interfaces_thermostat_setpoint.c\
	messages/interfaces/zwp_avi_interfaces_user_code.c \
	messages/interfaces/zwp_avi_interfaces_barrier_operator.c \
	messages/interfaces/zwp_avi_interfaces_wake_up.c \
	messages/interfaces/zwp_avi_interfaces_zip_gateway.c \
	messages/interfaces/zwp_avi_interfaces_zip_portal.c \
	messages/scenes/zwp_avi_scene_common.c \
	messages/scenes/zwp_avi_scene.c \
	messages/scenes/zwp_avi_scene_save.c \
	messages/scenes/zwp_avi_scene_list.c \
	messages/scenes/zwp_avi_scene_get_sup.c \
	messages/scenes/zwp_avi_security_scene_common.c \
	messages/scenes/zwp_avi_security_scene_list.c \
	messages/scenes/zwp_avi_security_scene_save.c \
	messages/scenes/zwp_avi_security_scene_get.c \
	messages/scenes/zwp_avi_security_scene_del.c \
	messages/scenes/zwp_avi_security_scene_set_state.c \
	messages/scenes/zwp_avi_security_scene_get_sup.c \
	util/zwp_io.c \
	util/zwp_log.c \
	util/zwp_apr.c \
	util/zwp_thread.c \
	util/zwp_thread_pool.c \
	util/zwp_queue.c \
	util/zwp_hash.c \
	util/zwp_thread_mutex.c \
	util/zwp_thread_cond.c \
	util/zwp_util_config.c \
	util/zwu_number.c \
	util/zwu_checksum.c \
	util/zwu_array_list.c \
	util/zwu_time.c \
	util/zwp_inet.c 
	
#
# Z-Ware portal version only
#
if ZWARE_PORTAL

sysconf_DATA += \
	$(SYSCONFIGDIR)/ZIPR.ca_x509.cer \
	$(SYSCONFIGDIR)/ZIPR.ca_x509.pem \
	$(SYSCONFIGDIR)/Portal.x509_1024.cer \
	$(SYSCONFIGDIR)/Portal.key_1024

zwportald_CFLAGS += \
	-DTCP_PORTAL \
	@HCAPI_SECURITY_INC_FLAGS@ \
	-I@SQLITE_INC@ \
	-I@CJSON64_INC@ \
	-I@CURL_INC@ \
	-I./messages/interfaces/
	
zwportald_LDFLAGS += \
	@HCAPI_SECURITY_LDFLAGS@  \
	-L@SQLITE_LIB@ \
	-L@LDAP_LIB@ -Wl,-rpath -Wl,@LDAP_LIB@ \
	-L@CURL_LIB@ -Wl,-rpath -Wl,@CURL_LIB@

zwportald_LDADD += \
	@HCAPI_SECURITY_LIBS@ \
	@CJSON64_LIB@/cJSON64.a \
	-lsqlite3 \
	-lcurl \
	-lldap_r

zwportald_SOURCES += \
	zwp_directory_service.c \
	zwp_ifttt.c \
	messages/networks/zwp_avi_networks_gateway_status.c \
	messages/root/zwp_avi_root_network_list.c \
	messages/root/zwp_avi_root_network_refresh.c \
	ifttt/zwp_avi_message_ifttt.c \
	util/zwp_db.c \
	util/zwu_http_client.c \
	util/zwu_http_response_header.c \
	util/zwu_http_client_option.c \
	util/zwu_http_client_manager.c \
	util/zwp_command_class_type_define.c \
	util/zwu_scale.c \
	messages/ifttt/zwp_avi_ifttt_common.c \
	messages/ifttt/zwp_avi_ifttt_db_common.c \
	messages/ifttt/zwp_avi_ifttt_test.c \
	messages/ifttt/zwp_avi_ifttt_bsensor.c \
	messages/ifttt/zwp_avi_ifttt_bswitch.c \
	messages/ifttt/zwp_avi_ifttt_dlock.c \
	messages/ifttt/zwp_avi_ifttt_mswitch.c \
	messages/ifttt/zwp_avi_ifttt_thrmomode.c \
	messages/ifttt/zwp_avi_ifttt_alarm.c \
	messages/ifttt/zwp_avi_ifttt_msensor.c
endif



#
# Z-Ware local version only
#
if ZWARE_LOCAL

zwportald_CFLAGS += \
	-DLOCAL_CLIENT_ENABLE \
	@HCAPI_SECURITY_INC_FLAGS@
	
zwportald_LDFLAGS += \
	@HCAPI_SECURITY_LDFLAGS@

zwportald_LDADD += \
	@HCAPI_SECURITY_LIBS@

zwportald_SOURCES +=  \
	zwp_net_discovery.c \
	zwp_zwave_client.c\
	messages/root/zwp_avi_root_gateway_initialize.c
endif

