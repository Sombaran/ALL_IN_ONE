#
# Copyright (C) 2018 Smartron India Pvt. Ltd.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=SM_iface
PKG_VERSION:0.1
PKG_RELEASE:=1
PKG_BUILD_DIR:= $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	SECTION:=utils
	CATEGORY:=Smartron-Packages
	SUBMENU:=Service Manager
	TITLE:=Service Manager libraries
	DEPENDS:=+libubus +libubox +libblobmsg-json +libevent2 +libpthread +libevent2-pthreads +fw-env +libjson-c +libsm_app
endef

define Package/$(PKG_NAME)/description
	This library has client related APIs.
endef

define Build/Prepare
	mkdir -p  $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
#	mkdir -p $(PKG_BUILD_DIR)/include/
#	$(CP) -r ./include $(PKG_BUILD_DIR)/

endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/*.h $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/*.so $(1)/usr/lib/
	chmod +x $(1)/usr/lib/libsm_app.so
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/*.so $(1)/usr/lib/
	chmod +x $(1)/usr/lib/libsm_app.so
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sm_test_client $(1)/usr/bin/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
