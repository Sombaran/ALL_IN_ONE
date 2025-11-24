# Make file for shifu-logger package
# $Id: $

include $(TOPDIR)/rules.mk

PKG_NAME:=logfiles_size_monitor
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk


define Package/logfiles_size_monitor
	SECTION:=utils
	CATEGORY:=Smartron-Packages
	TITLE:=logfiles_size_monitor
endef

define Build/Prepare
endef

define Build/Compile
endef


define Package/logfiles_size_monitor/install
	#Logger
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/logfiles_size_monitor.init $(1)/etc/init.d/logfiles_size_monitor
	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/all_logfiles_size_monitor $(1)/usr/sbin/

	#Timezone
	$(INSTALL_DIR) $(1)/etc/data/
	$(INSTALL_BIN) ./files/tz.dat $(1)/etc/data/
	$(INSTALL_BIN) ./files/check_tz $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/fread-write-protect $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/cpu_mem_usage $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/tz_check.init $(1)/etc/init.d/tz_check
endef

$(eval $(call BuildPackage,logfiles_size_monitor))
