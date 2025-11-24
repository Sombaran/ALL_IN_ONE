#
# Copyright (C) 2014 Gemtek
#
# $Id: $

include $(TOPDIR)/rules.mk

PKG_NAME:=libzwscene
PKG_VERSION:=0.42
PKG_RELEASE:=1
PKG_FIXUP:=autoreconf
PKG_INSTALL:=1


include $(INCLUDE_DIR)/package.mk


define Package/libzwscene
	SECTION:=lib
	CATEGORY:=Smartron-Packages
	SUBMENU:=Libraries
	TITLE:=libzwscene (zware v0.42)
	DEPENDS:=+glib2 +hcapi +libesmtp +udunits
endef

CONFIGURE_ARGS += \
	--with-glib="$(STAGING_DIR)/usr" \
	--with-hcapi="$(STAGING_DIR)/usr" \
	--with-libesmtp="$(STAGING_DIR)/usr" \
	--with-udunits="$(STAGING_DIR)/usr"


define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/InstallDev
	$(CP) $(PKG_INSTALL_DIR)/* $(1)/
endef

define Package/libzwscene/install
	$(INSTALL_DIR) $(1)/usr/{lib,bin} $(1)/etc
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libzwscene.so* $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/zwscene_config $(1)/usr/bin/
	$(CP) $(PKG_INSTALL_DIR)/etc/zwscenes.conf $(1)/etc/
endef

$(eval $(call BuildPackage,libzwscene))
