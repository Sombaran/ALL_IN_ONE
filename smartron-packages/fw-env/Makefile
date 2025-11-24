#====================================================================#
# Copyright (c) 2018 Smartron India Pvt. Ltd. All rights reserved.
# Version:0.2
# Last modified: janardhanachari.kell@smartron.com, 16/02/2018
#====================================================================#

include $(TOPDIR)/rules.mk

PKG_NAME:=fw-env
PKG_VERSION:0.1
PKG_RELEASE:=1
DEPENDS:=+libfw-env

include $(INCLUDE_DIR)/package.mk

define Package/fw-env
	SECTION:=utils
	CATEGORY:=Smartron-Packages
	SUBMENU:=Libraries
	TITLE:=Firmware environment variables library
endef

define Package/fw-env/description
	Firmware environment variables library
endef

define Build/Prepare
	mkdir -p  $(PKG_BUILD_DIR)
	echo xxx:$(CP)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/*.h $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libfw-env.so $(1)/usr/lib
endef

define Package/fw-env/install
    $(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libfw-env.so $(1)/usr/lib
endef

$(eval $(call BuildPackage,fw-env))
