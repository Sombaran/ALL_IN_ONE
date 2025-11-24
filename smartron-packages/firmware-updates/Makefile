#***********************************************************************************************************************************************************#
# FILENAME:		Makefile
#
# AUTHOR :  janardhanachari.kell@smartron.com, 26/02/2018
#
# VERSION:  0.1
#
# Copyright (C) 2018 Smartron India Private Limited - All Rights Reserved
#*********************************************************************************************************************************************************#

include $(TOPDIR)/rules.mk

PKG_NAME:=firmware-updates
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk


define Package/firmware-updates
  SECTION:=utils
  CATEGORY:=Smartron-Packages
  TITLE:=firmware-updates
  DEPENDS:=+libcrypto +libpthread +librt +libopenssl +libusb-1.0
endef

define Build/Prepare
endef

define Build/Compile
endef


define Package/firmware-updates/install

	$(INSTALL_DIR) $(1)/etc/fw/uboot-env
	$(CP) ./files/etc/fw/uboot-env/uboot-env.conf $(1)/etc/fw/uboot-env/

	$(INSTALL_DIR) $(1)/announcements
	$(CP) ./files/announcements/*.mp3 $(1)/announcements/
	$(CP) ./files/announcements/setannouncementvolume.sh $(1)/announcements/

	$(INSTALL_DIR) $(1)/etc
	$(CP) ./files/version/openwrt_release.conf $(1)/etc/openwrt_release
	$(CP) ./files/version/openwrt_version.conf $(1)/etc/openwrt_version
	$(CP) ./files/version/banner $(1)/etc/banner

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/peripheral_updater.init $(1)/etc/init.d/peripheral_updater

	$(INSTALL_DIR) $(1)/bin/
	$(INSTALL_BIN) ./files/version/lsb_release $(1)/bin/

	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/mcu_upgrade $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/dsp_upgrade $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/zwave_upgrade $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/zigbee_upgrade $(1)/usr/sbin/

	$(INSTALL_DIR) $(1)/etc/fw/mcu
	$(INSTALL_BIN) ./files/mcu/* $(1)/etc/fw/mcu/

	$(INSTALL_DIR) $(1)/etc/fw/dsp
	$(INSTALL_BIN) ./files/dsp/* $(1)/etc/fw/dsp/

	$(INSTALL_DIR) $(1)/etc/fw/zwave
	$(INSTALL_BIN) ./files/zwave/* $(1)/etc/fw/zwave/
	$(INSTALL_BIN) ./files/tools/* $(1)/usr/sbin/

	$(INSTALL_DIR) $(1)/etc/fw/zigbee
	$(INSTALL_BIN) ./files/zigbee/* $(1)/etc/fw/zigbee/
endef

$(eval $(call BuildPackage,firmware-updates))
