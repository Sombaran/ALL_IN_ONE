include $(TOPDIR)/rules.mk

PKG_NAME:=urllib3-1.22
PKG_VERSION:=1.22
PKG_RELEASE:=1
PKG_SOURCE:=urllib3-1.22-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/urllib3-1.22-$(PKG_VERSION)
PKG_BUILD_DEPENDS:=python

include $(INCLUDE_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/urllib3-1.22
  SUBMENU:=Shifu_Agents
  SECTION:=utils
  CATEGORY:=Smartron-Packages
  TITLE:=urllib3-1.22
  URL:=http://pypi.python.org/pypi/urllib3-1.22/
  DEPENDS:=+python +librt +libstdcpp +attrs-17.4.0
endef

define Package/urllib3-1.22/description
  urllib3-1.22 is the official Python binding for the ZeroMQ Messaging Library
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(call Build/Compile/PyMod,, install  --prefix="$(PKG_INSTALL_DIR)/usr")
endef

define Package/urllib3-1.22/install
	$(INSTALL_DIR) $(1)$(PYTHON_PKG_DIR)
	$(CP) \
		$(PKG_INSTALL_DIR)$(PYTHON_PKG_DIR)/* \
		$(1)$(PYTHON_PKG_DIR)
endef

$(eval $(call BuildPackage,urllib3-1.22))
