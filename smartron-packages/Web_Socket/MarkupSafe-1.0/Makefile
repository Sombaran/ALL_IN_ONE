include $(TOPDIR)/rules.mk

PKG_NAME:=MarkupSafe-1.0
PKG_VERSION:=1.0
PKG_RELEASE:=1
PKG_SOURCE:=MarkupSafe-1.0-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/MarkupSafe-1.0-$(PKG_VERSION)
PKG_BUILD_DEPENDS:=python

include $(INCLUDE_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/MarkupSafe-1.0
  SUBMENU:=Shifu_Agents
  SECTION:=utils
  CATEGORY:=Smartron-Packages
  TITLE:=MarkupSafe-1.0
  URL:=http://pypi.python.org/pypi/MarkupSafe-1.0/
  DEPENDS:=+python +librt +libstdcpp +attrs-17.4.0
endef

define Package/MarkupSafe-1.0/description
  MarkupSafe-1.0 is the official Python binding for the ZeroMQ Messaging Library
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(call Build/Compile/PyMod,, install  --prefix="$(PKG_INSTALL_DIR)/usr")
endef

define Package/MarkupSafe-1.0/install
	$(INSTALL_DIR) $(1)$(PYTHON_PKG_DIR)
	$(CP) \
		$(PKG_INSTALL_DIR)$(PYTHON_PKG_DIR)/* \
		$(1)$(PYTHON_PKG_DIR)
endef

$(eval $(call BuildPackage,MarkupSafe-1.0))
