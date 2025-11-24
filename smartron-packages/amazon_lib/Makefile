#
# Copyright (C) 2008 Lionic Corporation
#

include $(TOPDIR)/rules.mk

PKG_NAME:=amazon_lib
PKG_VERSION:1.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/amazon_lib
	SECTION:=utils
	DEFAULT:=y
	CATEGORY:=Smartron-Packages
	SUBMENU:=Libraries
	TITLE:=Sensory TrulyHandsFree SDK
	DEPENDS:=+libstdcpp
endef

define Package/amazon_lib/description
	amazon library
endef

define Build/Prepare
	mkdir -p  $(PKG_BUILD_DIR)
	echo xxx:$(CP)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	
endef

#define Build/InstallDev
#	$(INSTALL_DIR) $(1)/usr/include/
#	$(CP) $(PKG_BUILD_DIR)/src/pryon_lite.h $(1)/usr/include
#	$(INSTALL_DIR) $(1)/usr/lib
#	$(CP) $(PKG_BUILD_DIR)/lib/libpyton_lite.so $(1)/usr/lib
	
#endef

define Package/amazon_lib/install

	#$(INSTALL_DIR) $(1)/usr/lib
	#$(CP) $(PKG_BUILD_DIR)/lib/libpryon_lite.a $(1)/usr/lib
	
	$(INSTALL_DIR) $(1)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/include/pryon_lite.h $(1)/usr/include
	
	#$(INSTALL_DIR) $(1)/usr/lib
	#$(CP) $(PKG_BUILD_DIR)/lib/libpryon_lite.so $(1)/usr/lib
	
	#$(INSTALL_DIR) $(1)/usr/lib
	#$(CP) $(PKG_BUILD_DIR)/lib/libpryon_lite.so.1.11 $(1)/usr/lib
	
	
	$(INSTALL_DIR) $(1)/etc/amazon_model
	$(CP) $(PKG_BUILD_DIR)/arm_neon/D.en-IN.alexa.bin $(1)/etc/amazon_model
#	$(CP) $(PKG_BUILD_DIR)/arm_neon/D.en-IN.alexa.cpp  $(1)/etc/amazon_model
	
	
	#$(INSTALL_DIR) $(1)/etc/amazon_model/common
	#$(CP) $(PKG_BUILD_DIR)/common/D.en-IN.alexa.bin $(1)/etc/amazon_model/common
	#$(CP) $(PKG_BUILD_DIR)/common/D.en-IN.alexa.cpp  $(1)/etc/amazon_model/common

#	$(INSTALL_DIR) $(1)/usr/bin
#	$(INSTALL_BIN)  $(PKG_BUILD_DIR)/bin/spot-eval $(1)/usr/bin/
#	$(INSTALL_BIN)  $(PKG_BUILD_DIR)/bin/live-segment $(1)/usr/bin/
#	$(INSTALL_BIN)  $(PKG_BUILD_DIR)/bin/live-enroll $(1)/usr/bin/

#	$(INSTALL_DIR) $(1)/etc/sensory
#	$(CP) files/sensory_x_hbg_enUS_sfs14_delivery01_search_4.snsr $(1)/etc/sensory
#	$(CP) files/vad-energy-1.0.0.snsr $(1)/etc/sensory
endef

$(eval $(call BuildPackage,amazon_lib))
