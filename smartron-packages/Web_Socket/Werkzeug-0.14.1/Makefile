include $(TOPDIR)/rules.mk

PKG_NAME:=Werkzeug-0.14.1
PKG_VERSION:=0.14.1
PKG_RELEASE:=1
PKG_SOURCE:=Werkzeug-0.14.1-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/Werkzeug-0.14.1-$(PKG_VERSION)
PKG_BUILD_DEPENDS:=python

include $(INCLUDE_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/Werkzeug-0.14.1
  SUBMENU:=Shifu_Agents
  SECTION:=utils
  CATEGORY:=Smartron-Packages
  TITLE:=Werkzeug-0.14.1
  URL:=http://pypi.python.org/pypi/Werkzeug-0.14.1/
  DEPENDS:=+python +librt +libstdcpp +attrs-17.4.0
endef

define Package/Werkzeug-0.14.1/description
  Werkzeug-0.14.1 is the official Python binding for the ZeroMQ Messaging Library
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(call Build/Compile/PyMod,, install  --prefix="$(PKG_INSTALL_DIR)/usr")
endef

define Package/Werkzeug-0.14.1/install
	$(INSTALL_DIR) $(1)$(PYTHON_PKG_DIR)
	$(CP) \
		$(PKG_INSTALL_DIR)$(PYTHON_PKG_DIR)/* \
		$(1)$(PYTHON_PKG_DIR)
endef

$(eval $(call BuildPackage,Werkzeug-0.14.1))
