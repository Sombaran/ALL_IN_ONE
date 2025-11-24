include $(TOPDIR)/rules.mk

PKG_NAME:=backports.ssl_match_hostname
PKG_VERSION:=3.5.0.1
PKG_RELEASE:=1
PKG_SOURCE:=backports.ssl_match_hostname-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/backports.ssl_match_hostname-$(PKG_VERSION)
PKG_BUILD_DEPENDS:=python

include $(INCLUDE_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/backports.ssl_match_hostname
  SUBMENU:=Shifu_Agents
  SECTION:=utils
  CATEGORY:=Smartron-Packages
  TITLE:=backports.ssl_match_hostname
  URL:=http://pypi.python.org/pypi/backports.ssl_match_hostname/
  DEPENDS:=+python +librt +libstdcpp +attrs-17.4.0
endef

define Package/backports.ssl_match_hostname/description
  backports.ssl_match_hostname is the official Python binding for the ZeroMQ Messaging Library
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(call Build/Compile/PyMod,, install  --prefix="$(PKG_INSTALL_DIR)/usr")
endef

define Package/backports.ssl_match_hostname/install
	$(INSTALL_DIR) $(1)$(PYTHON_PKG_DIR)
	$(CP) \
		$(PKG_INSTALL_DIR)$(PYTHON_PKG_DIR)/* \
		$(1)$(PYTHON_PKG_DIR)
endef

$(eval $(call BuildPackage,backports.ssl_match_hostname))
