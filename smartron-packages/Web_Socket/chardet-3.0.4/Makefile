include $(TOPDIR)/rules.mk

PKG_NAME:=chardet-3.0.4
PKG_VERSION:=3.0.4
PKG_RELEASE:=1
PKG_SOURCE:=chardet-3.0.4-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/chardet-3.0.4-$(PKG_VERSION)
PKG_BUILD_DEPENDS:=python

include $(INCLUDE_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/chardet-3.0.4
  SUBMENU:=Shifu_Agents
  SECTION:=utils
  CATEGORY:=Smartron-Packages
  TITLE:=chardet-3.0.4
  URL:=http://pypi.python.org/pypi/chardet-3.0.4/
  DEPENDS:=+python +librt +libstdcpp +attrs-17.4.0
endef

define Package/chardet-3.0.4/description
  chardet-3.0.4 is the official Python binding for the ZeroMQ Messaging Library
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(call Build/Compile/PyMod,, install  --prefix="$(PKG_INSTALL_DIR)/usr")
endef

define Package/chardet-3.0.4/install
	$(INSTALL_DIR) $(1)$(PYTHON_PKG_DIR)
	$(CP) \
		$(PKG_INSTALL_DIR)$(PYTHON_PKG_DIR)/* \
		$(1)$(PYTHON_PKG_DIR)
endef

$(eval $(call BuildPackage,chardet-3.0.4))
