include $(TOPDIR)/rules.mk

PKG_NAME:=attrs-17.4.0
PKG_VERSION:=17.0.0b3
PKG_RELEASE:=1
PKG_SOURCE:=attrs-17.4.0-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/attrs-17.4.0-$(PKG_VERSION)
PKG_BUILD_DEPENDS:=python

include $(INCLUDE_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/attrs-17.4.0
  SUBMENU:=Shifu_Agents
  SECTION:=utils
  CATEGORY:=Smartron-Packages
  TITLE:=attrs-17.4.0
  URL:=http://pypi.python.org/pypi/attrs-17.4.0/
  DEPENDS:=+python +librt +libstdcpp
endef

define Package/attrs-17.4.0/description
  attrs-17.4.0 is the official Python binding for the ZeroMQ Messaging Library
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(call Build/Compile/PyMod,, install  --prefix="$(PKG_INSTALL_DIR)/usr")
endef

define Package/attrs-17.4.0/install
	$(INSTALL_DIR) $(1)$(PYTHON_PKG_DIR)
	$(CP) \
		$(PKG_INSTALL_DIR)$(PYTHON_PKG_DIR)/* \
		$(1)$(PYTHON_PKG_DIR)
endef

$(eval $(call BuildPackage,attrs-17.4.0))
