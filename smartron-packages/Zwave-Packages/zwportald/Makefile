#
# Copyright (C) 2014 Gemtek
#
# $Id: $

include $(TOPDIR)/rules.mk

PKG_NAME:=zwportald
PKG_VERSION:=0.50
PKG_RELEASE:=1
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk


define Package/zwportald
	SECTION:=utils
	CATEGORY:=Smartron-Packages
	SUBMENU:=Zwave-Packages
	TITLE:=zwportald (zware v0.50)
	DEPENDS:=+libopenssl +hcapi +libconfig +libapr +libaprutil +avro-c \
	  +libffi +zlib +glib2 +libexpat +udunits +libesmtp +libzwscene +unixodbc \
	  +libcurl
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

CONFIGURE_VARS += \
	ac_cv_lib_zwscene_main=yes


CONFIGURE_VARS += \
	LIBS="-ljansson -lodbc -lcrypto" \
	CFLAGS+="-DSHOW_HCAPI_MSG"

CONFIGURE_ARGS += \
	--with-zware-type=local \
	--with-hcapi-security=openssl \
	--with-openssl="$(STAGING_DIR)/usr" \
	--with-hcapi="$(STAGING_DIR)/usr" \
	--with-libconfig="$(STAGING_DIR)/usr" \
	--with-apr="$(STAGING_DIR)/usr" \
	--with-apr-util="$(STAGING_DIR)/usr" \
	--with-avro="$(STAGING_DIR)/usr" \
	--with-libffi="$(STAGING_DIR)/usr" \
	--with-zlib="$(STAGING_DIR)/usr" \
	--with-glib="$(STAGING_DIR)/usr" \
	--with-expat="$(STAGING_DIR)/usr" \
	--with-udunits="$(STAGING_DIR)/usr" \
	--with-libesmtp="$(STAGING_DIR)/usr" \
	--with-libzwscene="$(STAGING_DIR)/usr"


define Build/Configure
	( cd $(PKG_BUILD_DIR); ./auto-gen.sh )
	$(call Build/Configure/Default)
endef


define Package/zwportald/install
	$(INSTALL_DIR) $(1)/usr/bin $(1)/etc $(1)/etc/init.d
### Create the following dirs in runtime.
#	$(INSTALL_DIR) $(1)/var/{firmwares,log,networks}

	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/etc/device_rec_constants.txt $(1)/etc/device_rec_constants.txt
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/etc/zwave.avsc $(1)/etc/zwave.avsc
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/etc/zwave_device_rec.txt $(1)/etc/zwave_device_rec.txt
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/etc/zwportald.conf $(1)/etc/zwportald.conf
	$(INSTALL_BIN)  $(PKG_INSTALL_DIR)/usr/bin/zwportald $(1)/usr/bin/zwportald
	$(INSTALL_BIN)  ./files/zwportald.init $(1)/etc/init.d/zwportald
endef

$(eval $(call BuildPackage,zwportald))
