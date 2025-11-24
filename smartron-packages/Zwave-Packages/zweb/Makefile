#
# Copyright (C) 2014 Gemtek
#
# $Id: $

include $(TOPDIR)/rules.mk

PKG_NAME:=zweb
PKG_VERSION:=0.50
PKG_RELEASE:=1
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk


define Package/zweb
	SECTION:=utils
	CATEGORY:=Smartron-Packages
	SUBMENU:=Zwave-Packages
	TITLE:=zweb (zware v0.50)
	DEPENDS:=+jansson +apache +avro-c +php5
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef



CONFIGURE_VARS += \
	LIBS="-ljansson -lm"


CONFIGURE_ARGS += \
	--with-zware-type=local \
	--with-fcgi="$(STAGING_DIR)/usr" \
	--with-avro="$(STAGING_DIR)/usr"


define Build/Configure
	( cd $(PKG_BUILD_DIR); ./autogen.sh )
	$(call Build/Configure/Default)
endef


define Package/zweb/install
	$(INSTALL_DIR) \
		$(1)/usr/share/zweb/fcgi-bin

	$(INSTALL_BIN)  $(PKG_INSTALL_DIR)/usr/bin/zcgi $(1)/usr/share/zweb/fcgi-bin/zcgi
	$(CP) ./files/* $(1)/
endef

$(eval $(call BuildPackage,zweb))
