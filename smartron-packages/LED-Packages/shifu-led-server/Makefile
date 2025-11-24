include $(TOPDIR)/rules.mk

PKG_NAME:=shifu-led-server
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk


define Package/$(PKG_NAME)
	SECTION:=utils
	CATEGORY:=Smartron-Packages
	SUBMENU:=LED-Packages
	TITLE:=client for  for LED, Audio volume
	DEPENDS:=+libpthread +librt +libgtk +libusb-1.0 +fw-env +shifu-led-interface +libjson-c
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	mkdir -p $(PKG_BUILD_DIR)/include/
	$(CP) -r ../include $(PKG_BUILD_DIR)/
endef

define Package/$(PKG_NAME)/install
	#installing led-server to shfu
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/led-server $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wps_trigger $(1)/usr/bin/
	$(INSTALL_BIN) ./files/host_demo $(1)/usr/bin/
	$(INSTALL_BIN) ./files/WakeWord_start $(1)/usr/bin/

	#installing startup for led-server to shfu
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/led-service.init $(1)/etc/init.d/led-service

	#moving .wav files to wav_files folder
	$(INSTALL_DIR) $(1)/wav_files
	$(CP) ./files/wav_files/*.wav $(1)/wav_files/

	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libgtk.so $(1)/usr/lib
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
