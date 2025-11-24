#
# Copyright (C) 2018 Smartron India Pvt. Ltd.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=SM_app
PKG_VERSION:0.2
PKG_RELEASE:=1
PKG_BUILD_DIR:= $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	SECTION:=utils
	CATEGORY:=Smartron-Packages
	SUBMENU:=Service Manager
	TITLE:=Service Manager application
	DEPENDS:=+libubus +libubox +libblobmsg-json +libevent2 +fw-env +libjson-c
endef

define Package/$(PKG_NAME)/description
	This application broadcasts network status.
endef

define Build/Prepare
	mkdir -p  $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sm_app $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/
	$(INSTALL_DATA) ./files/sm_config.json $(1)/etc/sm_config.json
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/sm_service.init $(1)/etc/init.d/sm_service
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
