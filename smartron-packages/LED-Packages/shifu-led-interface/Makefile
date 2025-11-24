#
# Copyright (C) 2018 Smartron India Pvt. Ltd.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=shifu-led-interface
PKG_VERSION:0.1
PKG_RELEASE:=1
DEPENDS:=+liblediface

include $(INCLUDE_DIR)/package.mk

define Package/shifu-led-interface
	SECTION:=utils
	CATEGORY:=Smartron-Packages
	TITLE:=shifu-led-interface library
endef

define Package/shifu-led-interface/description
	shifu-led-interface library
endef

define Build/Prepare
	mkdir -p  $(PKG_BUILD_DIR)
	echo xxx:$(CP)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	mkdir -p $(PKG_BUILD_DIR)/include/
	$(CP) -r ../include $(PKG_BUILD_DIR)/
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/include/*.h $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/*.so $(1)/usr/lib
endef

define Package/shifu-led-interface/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/led-test-client $(1)/usr/bin/
endef

$(eval $(call BuildPackage,shifu-led-interface))
