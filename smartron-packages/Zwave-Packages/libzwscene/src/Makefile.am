#
# Automake input file for ZWScene library
#
# Author: Arif

lib_LTLIBRARIES = libzwscene.la
bin_PROGRAMS = zwscene_config
ACLOCAL_AMFLAGS = -I m4

libzwscene_la_SOURCES = \
	src/zwscn_device_report_cache.c \
	src/zwscn_notification_handler.c \
	src/zwscn_notification_processor.c \
	src/zwscn_persistence.c \
	src/zwscn_persistence_format.c \
	src/zwscn_persistence_security_scene.c \
	src/zwscn_scene_api.c \
	src/zwscn_scene.c \
	src/zwscn_scene_events.c \
	src/zwscn_scene_execute.c \
	src/zwscn_scene_schedule.c \
	src/zwscn_scene_status_update.c \
	src/zwscn_setup_op.c \
	src/zwscn_statelog.c \
	src/zwscn_util.c \
	src/zwscn_util_config.c \
	src/zwscn_util_config_file.c \
	src/zwscn_util_email.c \
	src/zwscn_util_filesys.c \
	src/zwscn_util_hash.c \
	src/zwscn_util_list.c \
	src/zwscn_util_mem.c \
	src/zwscn_util_message_log.c \
	src/zwscn_util_mqueue.c \
	src/zwscn_util_mqueue_message.c \
	src/zwscn_util_mutex.c \
	src/zwscn_util_rwlock.c \
	src/zwscn_util_thread.c \
	src/zwscn_util_time.c \
	src/zwscn_util_timer.c \
	src/zwscn_util_unit.c \
	src/zwscn_util_zwave.c \
	src/zwscn_util_zwave_eval.c \
	src/zwscn_zwpoll.c \
	src/zwscn_security_scene.c \
	src/zwscn_security_scene_api.c \
	src/zwscn_security_scene_events.c \
	src/zwscn_security_scene_notifications.c \
	src/zwscn_notification_processor_security_scene.c \
	src/zwscn_xml.c \
	src/zwscn_serializer_functions.c \
	src/zwscn_deserializer_functions.c
	
zwscene_config_SOURCES = \
	util/zwscene_config.c


AM_CFLAGS     			= $(GLIB_CFLAGS) @HCAPI_CPPFLAGS@ @HCAPI_CFLAGS@ @LIBESMTP_CFLAGS@ @UDUNITS_CFLAFS@ -I$(top_srcdir)/include -I$(top_srcdir)/include/private -Wall
libzwscene_LIBS 		= $(GLIB_LIBS) @LIBESMTP_LIBS@ @UDUNITS_LIBS@
#libzwscene_la_LIBADD 	= -lz
libzwscene_la_LDFLAGS	= -version-info 6:0:2
zwscene_config_LDADD 	= $(GLIB_LIBS)


include_HEADERS = \
	include/zwscn_device_args.h \
	include/zwscn_device_reports.h \
	include/zwscn_scene_api.h \
	include/zwscn_security_scene_api.h
	
SYSCONFIGDIR = etc
sysconf_DATA = $(SYSCONFIGDIR)/zwscenes.conf
	
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libzwscene.pc

