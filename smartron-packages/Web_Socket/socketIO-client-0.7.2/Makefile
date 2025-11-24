include $(TOPDIR)/rules.mk

PKG_NAME:=socketIO-client-0.7.2
PKG_VERSION:=0.7.2
PKG_RELEASE:=1
PKG_SOURCE:=socketIO-client-0.7.2-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/socketIO-client-0.7.2-$(PKG_VERSION)
PKG_BUILD_DEPENDS:=python

include $(INCLUDE_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/socketIO-client-0.7.2
  SUBMENU:=Shifu_Agents
  SECTION:=utils
  CATEGORY:=Smartron-Packages
  TITLE:=socketIO-client-0.7.2
  URL:=http://pypi.python.org/pypi/socketIO-client-0.7.2/
  DEPENDS:=+python +librt +libstdcpp +attrs-17.4.0 +requests-2.7.0  +six-1.11.0 +backports.ssl-match-hostname
endef

define Package/socketIO-client-0.7.2/description
  socketIO-client-0.7.2 is the official Python binding for the ZeroMQ Messaging Library
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(call Build/Compile/PyMod,, install  --prefix="$(PKG_INSTALL_DIR)/usr")
endef

define Package/socketIO-client-0.7.2/install
	$(INSTALL_DIR) $(1)$(PYTHON_PKG_DIR)
	$(CP) \
		$(PKG_INSTALL_DIR)$(PYTHON_PKG_DIR)/* \
		$(1)$(PYTHON_PKG_DIR)
endef

$(eval $(call BuildPackage,socketIO-client-0.7.2))
