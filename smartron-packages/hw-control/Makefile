include $(TOPDIR)/rules.mk

PKG_NAME:=hw-control
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk


define Package/$(PKG_NAME)
	SECTION:=utils
	CATEGORY:=Smartron-Packages
	TITLE:=Hardware control for LED, Audio volume
	DEPENDS:=+librt +shifu-led-server +shifu-led-interface +fw-env
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/gpio-scripts
	$(INSTALL_BIN) ./files/45 $(1)/etc/gpio-scripts/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/hwctrl_service.init $(1)/etc/init.d/hwctrl_service
	$(INSTALL_BIN) ./files/Network_Reset.sh  $(1)/usr/bin/
	$(INSTALL_BIN) ./files/init_wps.sh  $(1)/usr/bin/
	$(INSTALL_BIN) ./files/FQC.py  $(1)/usr/bin/
	$(INSTALL_BIN) ./files/htm.py  $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/root/FQC_LOG
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
