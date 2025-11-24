#====================================================================#
# Copyright (c) 2018 Smartron India Pvt. Ltd. All rights reserved.
# Version:0.2
# Last modified: harendragautam@smartron.com, 24/01/2019
#====================================================================#
# make file for standalone voice cron_handler in openWRT
# inputs : source code to be compiled as openWRT executable
# outputs: Generates executable file in build directory.

include $(TOPDIR)/rules.mk

PKG_NAME:=cron_handler
PKG_VERSION:=0.1
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/cron_handler
	SECTION:=utils
	DEFAULT:=y
	CATEGORY:=Smartron-Packages
	TITLE:=cron_handler -- prints a snarky message
	DEPENDS:=+libstdcpp +libjson-c +libzeromq +fw-env +lsqlite3
endef

define Package/cron_handler/description
 This package prints cron_handler
endef


define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(CP) $(STAGING_DIR)/usr/lib/libsqlite3.so* $(PKG_BUILD_DIR)/
endef

define Package/cron_handler/install

	#deploying cron_handler & shifu_cron_event into /usr/bin
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cron_handler $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/shifu_cron_event $(1)/usr/bin/
	$(CP) $(STAGING_DIR)/usr/lib/libsqlite3.so.0 $(1)/usr/bin/

	#deploying cron_handler service
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/cron_service.init $(1)/etc/init.d/cron_service

endef

$(eval $(call BuildPackage,cron_handler))

