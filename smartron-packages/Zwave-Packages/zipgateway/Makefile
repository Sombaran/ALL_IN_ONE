#
# Copyright (C) 2014 Gemtek
#
# $Id: $

include $(TOPDIR)/rules.mk

PKG_NAME:=zipgateway
PKG_VERSION:=2.81
PKG_RELEASE:=1

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk


define Package/$(PKG_NAME)
	SECTION:=utils
	CATEGORY:=Smartron-Packages
	SUBMENU:=Zwave-Packages
	TITLE:=ZIP Router for Z-Wave Devices
#	DEPENDS:=+kmod-tun +kmod-ipv6 +ip +libopenssl +libusb-1.0
	DEPENDS:=+kmod-tun  +ip +libopenssl +libusb-1.0
endef

define Package/$(PKG_NAME)/description
	ZIP Router for Z-Wave Devices. Make a possible to Z-Wave Devices to reach internet.
endef


define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef


define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) \
		$(1)/usr/bin \
		$(1)/usr/local/etc \
		$(1)/usr/share/z-wave \
		$(1)/usr/local/var/    \
    	$(1)/usr/local/var/lib/ \
		$(1)/usr/local/var/lib/zipgateway \
		$(1)/etc/init.d \

	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/zipgateway $(1)/usr/bin/zipgateway

	$(INSTALL_BIN)  ./files/zipgateway.init $(1)/etc/init.d/zipgateway
	$(INSTALL_BIN)  ./files/zipgateway.tun $(1)/usr/local/etc/zipgateway.tun
	$(INSTALL_BIN)  ./files/zipgateway.fin $(1)/usr/local/etc/zipgateway.fin
	$(INSTALL_CONF) ./files/zipgateway.cfg $(1)/usr/local/etc/zipgateway.cfg
	$(INSTALL_CONF) ./files/zipgateway_provisioning_list.cfg $(1)/usr/local/etc/zipgateway_provisioning_list.cfg
#	$(INSTALL_CONF) ./files/ZIPR.x509_1024.pem $(1)/usr/local/etc/ZIPR.x509_1024.pem
#	$(INSTALL_CONF) ./files/ZIPR.key_1024.pem $(1)/usr/local/etc/ZIPR.key_1024.pem
#	$(INSTALL_CONF) ./files/Portal.ca_x509.pem $(1)/usr/local/etc/Portal.ca_x509.pem

endef

$(eval $(call BuildPackage,$(PKG_NAME)))
