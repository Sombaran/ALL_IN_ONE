##############################################
# OpenWrt Makefile for mqtt-manager
##############################################

include $(TOPDIR)/rules.mk

PKG_NAME:=mqtt-manager
PKG_VERSION:=1.0.0
PKG_RELEASE:=1
PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	SECTION:=utils
	CATEGORY:=Smartron-Packages
	TITLE:=mqtt-manager -- A client application for publishing BT data to Mobile App
	DEPENDS:=+libstdcpp  +libpthread +libmqtt +libpaho-mqtt +libblobmsg-json +libjson-c +libzeromq +fw-env

endef

define Package/$(PKG_NAME)/description
 This package helps in communicating with Mobile APP from other modules like BT, VA and DM.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/Debug/mqtt-manager $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/mqtt_service.init $(1)/etc/init.d/mqtt_service
	$(CP) $(PKG_BUILD_DIR)/test/*.py $(1)/usr/sbin/
	

endef

$(eval $(call BuildPackage,$(PKG_NAME)))

