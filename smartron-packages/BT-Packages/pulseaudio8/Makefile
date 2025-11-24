#
# Copyright (C) 2011-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=pulseaudio
PKG_VERSION:=8.0
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=http://freedesktop.org/software/pulseaudio/releases/
PKG_MD5SUM:=8678442ba0bb4b4c33ac6f62542962df
PKG_LICENSE:=LGPL-2.1+
PKG_LICENSE_FILES:=GPL LICENSE

PKG_BUILD_DEPENDS:=intltool/host

PKG_FIXUP:=autoreconf
PKG_USE_MIPS16:=0

PKG_INSTALL = 1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/pulseaudio8/Default
  SECTION:=utils
  CATEGORY:=Smartron-Packages
  SUBMENU:=BT-Packages
  DEPENDS:=+libspeexdsp +libsndfile +libltdl +libpthread \
	+librt +alsa-lib +libopenssl +libwrap +libcap $(ICONV_DEPENDS) $(INTL_DEPENDS) \
        +sbc +libdbus +liblzma +bluez5-daemon
  TITLE:=Network sound server
  MAINTAINER:=Peter Wagner <tripolar@gmx.at>
  URL:=http://www.pulseaudio.org
  PROVIDES:=pulseaudio8
  USERID:=pulse=51:pulse=51
endef

define Package/pulseaudio8-daemon
  $(call Package/pulseaudio8/Default)
  DEPENDS+= +libjson-c 
endef

define Package/pulseaudio8/Default/description
  PulseAudio (formerly Polypaudio) is a cross-platform, networked sound server.
endef

define Package/pulseaudio8-daemon/description
  $(call Package/pulseaudio8/Default/description)
endef

define Package/pulseaudio8-daemon/conffiles
/etc/pulse/client.conf
/etc/pulse/daemon.conf
/etc/pulse/default.pa
/etc/pulse/system.pa
endef

define Package/pulseaudio8-tools
  SECTION:=utils
  CATEGORY:=Smartron-Packages
  SUBMENU:=BT-Packages
  DEPENDS:=+libsndfile +pulseaudio8
  TITLE:=Tools for Pulseaudio
  URL:=http://www.pulseaudio.org
endef

define Package/pulseaudio8-profiles
  SECTION:=utils
  CATEGORY:=Smartron-Packages
  SUBMENU:=BT-Packages
  DEPENDS:=pulseaudio8
  TITLE:=Profiles for Pulseaudio
  URL:=http://www.pulseaudio.org
endef

CONFIGURE_ARGS += \
	--with-system-user=pulse \
	--with-system-group=pulse \
	--with-access-group=audio \
	--with-database=simple \
	--enable-alsa \
	--disable-hal \
	--disable-gconf \
	--disable-nls \
	--disable-manpages \
	--enable-oss-output \
	--disable-oss-wrapper \
	--disable-samplerate \
	--disable-per-user-esound-socket \
	--disable-solaris \
	--disable-gtk2 \
	--disable-glib2 \
	--disable-jack \
	--disable-asyncns \
	--disable-lirc \
	--enable-bluez5 \
	--disable-udev \
	--without-fftw \
	--disable-avahi \
	--enable-dbus \
	--enable-bluez5-native-headset

CONFIGURE_VARS += \
	PKG_CONFIG_LIBDIR="$(STAGING_DIR)/usr/lib/pkgconfig"

#SUPP_LIBS:=-L$(INTL_PREFIX)/lib -L$(ICONV_PREFIX)/lib
TARGET_CFLAGS += -std=gnu99
TARGET_CFLAGS += -mfloat-abi=softfp -mfpu=neon
define Build/InstallDev
	$(INSTALL_DIR) \
		$(1)/usr/lib/pkgconfig \
		$(1)/usr/include/pulse \
		$(1)/usr/lib \
		$(1)/usr/lib/pulseaudio
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/include/pulse/* \
		$(1)/usr/include/pulse
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc \
		$(1)/usr/lib/pkgconfig
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/*.so* \
		$(1)/usr/lib/
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/pulseaudio/* \
		$(1)/usr/lib/
endef

define Package/pulseaudio8-daemon/install
	$(INSTALL_DIR) \
		$(1)/root/.pulse \
		$(1)/etc/pulse \
		$(1)/etc/init.d \
		$(1)/usr/bin \
		$(1)/usr/lib \
		$(1)/usr/lib/pulseaudio \
		$(1)/usr/lib/pulse-$(PKG_VERSION)/modules

	$(INSTALL_BIN) \
		$(PKG_INSTALL_DIR)/usr/bin/pulseaudio \
		$(1)/usr/bin/pulseaudio

	$(INSTALL_BIN) \
		./files/pulseaudio.init \
		$(1)/etc/init.d/pulseaudio

	$(INSTALL_CONF) \
		$(PKG_INSTALL_DIR)/etc/pulse/* \
		$(1)/etc/pulse

	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/*.so* \
		$(1)/usr/lib/

	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/pulseaudio/* \
		$(1)/usr/lib/

	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/pulse-$(PKG_VERSION)/modules/lib*.so \
		$(1)/usr/lib/

	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/pulse-$(PKG_VERSION)/modules/module*.so \
		$(1)/usr/lib/pulse-$(PKG_VERSION)/modules/

		$(INSTALL_CONF) ./files/default.pa $(1)/etc/pulse/default.pa		
		$(INSTALL_CONF) ./files/daemon.conf $(1)/etc/pulse/daemon.conf		
		$(INSTALL_CONF) ./files/asound.conf $(1)/etc/asound.conf	

	chmod -R 0644 $(1)/etc/pulse/*
endef

define Package/pulseaudio8-tools/install
	$(INSTALL_DIR) \
		$(1)/usr/bin

	$(INSTALL_BIN) \
		$(PKG_INSTALL_DIR)/usr/bin/pa* \
		$(1)/usr/bin/
endef

define Package/pulseaudio8-profiles/install
	$(INSTALL_DIR) \
		$(1)/usr/share/pulseaudio/alsa-mixer/paths \
		$(1)/usr/share/pulseaudio/alsa-mixer/profile-sets

	$(INSTALL_CONF) \
		$(PKG_INSTALL_DIR)/usr/share/pulseaudio/alsa-mixer/paths/* \
		$(1)/usr/share/pulseaudio/alsa-mixer/paths

	$(INSTALL_CONF) \
		$(PKG_INSTALL_DIR)/usr/share/pulseaudio/alsa-mixer/profile-sets/* \
		$(1)/usr/share/pulseaudio/alsa-mixer/profile-sets
endef

$(eval $(call BuildPackage,pulseaudio8-daemon))
$(eval $(call BuildPackage,pulseaudio8-tools))
$(eval $(call BuildPackage,pulseaudio8-profiles))

