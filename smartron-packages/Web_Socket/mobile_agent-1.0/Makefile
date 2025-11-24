include $(TOPDIR)/rules.mk

PKG_NAME:=mobile_agent-1.0
PKG_VERSION:=1.0
PKG_RELEASE:=1
PKG_SOURCE:=mobile_agent-1.0-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/mobile_agent-1.0-$(PKG_VERSION)
PKG_BUILD_DEPENDS:=python

include $(INCLUDE_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/mobile_agent-1.0
  SUBMENU:=Shifu_Agents
  SECTION:=utils
  CATEGORY:=Smartron-Packages
  TITLE:=mobile_agent-1.0
  URL:=http://pypi.python.org/pypi/mobile_agent-1.0/
  DEPENDS:=+python +librt +libstdcpp +attrs-17.4.0 +click-6.7 +Flask-0.12.2 +itsdangerous-0.24 +Jinja2-2.10 +MarkupSafe-1.0 +Werkzeug-0.14.1
endef

define Package/mobile_agent-1.0/description
  mobile_agent-1.0 is the official Python binding for the ZeroMQ Messaging Library
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(call Build/Compile/PyMod,, install  --prefix="$(PKG_INSTALL_DIR)/usr")
endef

define Package/mobile_agent-1.0/install
	$(INSTALL_DIR) $(1)$(PYTHON_PKG_DIR)
	$(CP) \
		$(PKG_INSTALL_DIR)$(PYTHON_PKG_DIR)/* \
		$(1)$(PYTHON_PKG_DIR)
		
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) ./files/Mobile_Agent.py $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) ./files/MA_boottime_update.py $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) ./files/WBLR.py $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/
	$(INSTALL_DATA) ./files/profile.json $(1)/etc/
	$(INSTALL_DIR) $(1)/etc/
	$(INSTALL_DATA) ./files/tokens.json $(1)/etc/

	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/ma_service.init $(1)/etc/init.d/ma_service
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/ma_bootup.init $(1)/etc/init.d/ma_bootup

endef

$(eval $(call BuildPackage,mobile_agent-1.0))
