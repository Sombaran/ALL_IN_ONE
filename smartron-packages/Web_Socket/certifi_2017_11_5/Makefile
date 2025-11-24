include $(TOPDIR)/rules.mk

PKG_NAME:=certifi_2017_11_5
PKG_VERSION:=17.0.0b3
PKG_RELEASE:=1
PKG_SOURCE:=certifi_2017_11_5-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/certifi_2017_11_5-$(PKG_VERSION)
PKG_BUILD_DEPENDS:=python

include $(INCLUDE_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/certifi_2017_11_5
  SUBMENU:=Shifu_Agents
  SECTION:=utils
  CATEGORY:=Smartron-Packages
  TITLE:=certifi_2017_11_5
  URL:=http://pypi.python.org/pypi/certifi_2017_11_5/
  DEPENDS:=+python +librt +libstdcpp +attrs-17.4.0
endef

define Package/certifi_2017_11_5/description
  certifi_2017_11_5 is the official Python binding for the ZeroMQ Messaging Library
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(call Build/Compile/PyMod,, install  --prefix="$(PKG_INSTALL_DIR)/usr")
endef

define Package/certifi_2017_11_5/install
	$(INSTALL_DIR) $(1)$(PYTHON_PKG_DIR)
	$(CP) \
		$(PKG_INSTALL_DIR)$(PYTHON_PKG_DIR)/* \
		$(1)$(PYTHON_PKG_DIR)
endef

$(eval $(call BuildPackage,certifi_2017_11_5))
