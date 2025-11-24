include $(TOPDIR)/rules.mk

PKG_NAME:=Z3GatewayHost
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk


define Package/$(PKG_NAME)
	SECTION:=utils
	CATEGORY:=Smartron-Packages
	TITLE:=zigbee gateway
	DEPENDS:= +libpthread +libreadline +libncurses +sqlite3 +librt +fw-env
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(CP) $(STAGING_DIR)/usr/lib/libjson-c* $(PKG_BUILD_DIR)/
	$(CP) $(STAGING_DIR)/usr/lib/libsqlite3.so* $(PKG_BUILD_DIR)/
	$(CP) $(STAGING_DIR)/usr/lib/libcurl.so.4* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I$(STAGING_DIR)/usr/include" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		LD="$(TARGET_CROSS)ld"
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/libjson-c.so.2 $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/libsqlite3.so.0 $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/libcurl.so.4 $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bin/Z3GatewayHost_2 $(1)/usr/sbin/Z3GatewayHost
	
	#deploying zigbee service
	$(INSTALL_DIR) $(1)/exe_dir
	$(CP) $(PKG_BUILD_DIR)/v6.0/app/builder/Z3GatewayHost_2/ZigbeeDatabase.db $(1)/exe_dir/ 
	$(INSTALL_BIN) ./files/zbee_init $(1)/exe_dir/

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/zb_service.init $(1)/etc/init.d/zb_service		
	
endef

$(eval $(call BuildPackage,$(PKG_NAME)))

