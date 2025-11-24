#
# Copyright (C) 2017 Smartron India Pvt Ltd.
#
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=Python_BT
PKG_VERSION:=0.1.0
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

PKG_BUILD_DEPENDS:=python

include $(INCLUDE_DIR)/package.mk

define Package/Python_BT
  SECTION:=utils
  CATEGORY:=Smartron-Packages
  SUBMENU:=BT-Packages
  TITLE:=Python_BT
  DEPENDS:=+pulseaudio8-daemon +bluez5-daemon +python +libdbus-glib +dbus-python +python-gobject
endef

define Package/Python_BT/description
  Python-BT is a package for custom Bluetooth applications using Bluez5
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/Python_BT/install
	$(INSTALL_DIR) $(1)/usr/sbin/Bluetooth/tests
	$(CP) $(PKG_BUILD_DIR)/tests/* $(1)/usr/sbin/Bluetooth/tests
	$(INSTALL_DIR) $(1)/usr/lib/python2.7/site-packages/Python_BT
	$(CP) $(PKG_BUILD_DIR)/python_BT/*.pye $(1)/usr/lib/python2.7/site-packages/Python_BT
	$(INSTALL_DIR) $(1)/usr/sbin/Bluetooth
	$(CP) $(PKG_BUILD_DIR)/setup.py $(1)/usr/sbin/Bluetooth
	$(CP) $(PKG_BUILD_DIR)/requirements.txt $(1)/usr/sbin/Bluetooth
endef

$(eval $(call BuildPackage,Python_BT))
