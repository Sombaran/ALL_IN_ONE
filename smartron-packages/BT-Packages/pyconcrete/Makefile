include $(TOPDIR)/rules.mk

PKG_NAME:=pyconcrete
PKG_VERSION:=0.11.0
PKG_RELEASE:=1
PKG_SOURCE:=pyconcrete-$(PKG_VERSION).tar.gz
PKG_BUILD_DIR:=$(BUILD_DIR)/pyconcrete-$(PKG_VERSION)
PKG_BUILD_DEPENDS:=python
PKG_FIXUP:=autoreconf

include $(INCLUDE_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/pyconcrete
  SUBMENU:=Python
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=pyconcrete
  URL:=http://pypi.python.org/pypi/pyconcrete/
  DEPENDS:=+python +librt +libstdcpp
endef

define Package/pyconcrete/description
  pyconcrete is the official Python binding for the ZeroMQ Messaging Library
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(call Build/Compile/PyMod,, install  --prefix="$(PKG_INSTALL_DIR)/usr" )
endef

define Package/pyconcrete/install
	$(INSTALL_DIR) $(1)$(PYTHON_PKG_DIR)
	$(CP) \
		$(PKG_INSTALL_DIR)/* \
		$(1)
endef

$(eval $(call BuildPackage,pyconcrete))
