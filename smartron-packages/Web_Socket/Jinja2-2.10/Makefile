include $(TOPDIR)/rules.mk

PKG_NAME:=Jinja2-2.10
PKG_VERSION:=2.10
PKG_RELEASE:=1
PKG_SOURCE:=Jinja2-2.10-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/Jinja2-2.10-$(PKG_VERSION)
PKG_BUILD_DEPENDS:=python

include $(INCLUDE_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/Jinja2-2.10
  SUBMENU:=Shifu_Agents
  SECTION:=utils
  CATEGORY:=Smartron-Packages
  TITLE:=Jinja2-2.10
  URL:=http://pypi.python.org/pypi/Jinja2-2.10/
  DEPENDS:=+python +librt +libstdcpp +attrs-17.4.0
endef

define Package/Jinja2-2.10/description
  Jinja2-2.10 is the official Python binding for the ZeroMQ Messaging Library
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(call Build/Compile/PyMod,, install  --prefix="$(PKG_INSTALL_DIR)/usr")
endef

define Package/Jinja2-2.10/install
	$(INSTALL_DIR) $(1)$(PYTHON_PKG_DIR)
	$(CP) \
		$(PKG_INSTALL_DIR)$(PYTHON_PKG_DIR)/* \
		$(1)$(PYTHON_PKG_DIR)
endef

$(eval $(call BuildPackage,Jinja2-2.10))
