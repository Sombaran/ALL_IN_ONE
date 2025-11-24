#
# Copyright (C) 2014 Gemtek
#
# $Id: $

include $(TOPDIR)/rules.mk

PKG_NAME:=hcapi
PKG_VERSION:=8.12
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk


define Package/hcapi
	SECTION:=lib
	CATEGORY:=Smartron-Packages
	SUBMENU:=Libraries
	TITLE:=Z/IP library
	DEPENDS:= +libopenssl +libapr +libaprutil \
	  +zlib +glib2 +libexpat +udunits  +unixodbc +fw-env
	  

endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(PKG_BUILD_DIR)" \
		CC="$(TARGET_CC)" \
		AR="$(TARGET_CROSS)ar" \
		SINGLE_CLIENT=1 \
		TARGET_PLATFORM=LINUX_ZIP2 \
		INCLUDE_PATH="-I$(STAGING_DIR)/usr/include -I$(STAGING_DIR)/usr/include/openssl"
endef

define Build/InstallDev
	$(INSTALL_DIR) \
		$(1)/usr/include \
		$(1)/usr/include/zwave \
		$(1)/usr/lib 

	$(INSTALL_DATA) $(PKG_BUILD_DIR)/include/*.h $(1)/usr/include/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/include/zwave/*.h $(1)/usr/include/zwave/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/lib/libzip_api.a $(1)/usr/lib/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/libzip_ctl.a $(1)/usr/lib/

endef

define Package/hcapi/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/demos/gw_discovery/gw_discovery $(1)/usr/bin/gw_discovery
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/zwave_application/zwave_app $(1)/usr/bin/zwave_app
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dm_req/dm_req $(1)/usr/bin/dm_req
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dm_res/dm_res $(1)/usr/bin/dm_res

	#deploying zwave service

	$(INSTALL_DIR) $(1)/exe_dir
	$(INSTALL_BIN) ./files/zwave_init $(1)/exe_dir/

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/zw_service.init $(1)/etc/init.d/zw_service
	$(INSTALL_BIN) ./files/zw_reset_service.init $(1)/etc/init.d/zw_reset_service
	$(INSTALL_BIN) ./files/zwave_reset $(1)/usr/bin
	$(INSTALL_DIR) $(1)/root/zwave/
	$(CP) $(PKG_BUILD_DIR)/zwave_application/zwave_device_rec.txt $(1)/exe_dir/

endef

$(eval $(call BuildPackage,hcapi))
