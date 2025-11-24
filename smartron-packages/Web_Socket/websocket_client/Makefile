include $(TOPDIR)/rules.mk

PKG_NAME:=websocket_client
PKG_VERSION:=0.47.0
PKG_RELEASE:=1
PKG_SOURCE:=websocket_client-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/websocket_client-$(PKG_VERSION)
PKG_BUILD_DEPENDS:=python

include $(INCLUDE_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/websocket_client
  SUBMENU:=Shifu_Agents
  SECTION:=utils
  CATEGORY:=Smartron-Packages
  TITLE:=websocket_client
  URL:=http://pypi.python.org/pypi/websocket_client/
  DEPENDS:=+backports.ssl_match_hostname +python +librt +libstdcpp +attrs-17.4.0 +six-1.11.0 
endef

define Package/websocket_client/description
  websocket_client is the official Python binding for the ZeroMQ Messaging Library
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(call Build/Compile/PyMod,, install  --prefix="$(PKG_INSTALL_DIR)/usr")
endef

define Package/websocket_client/install
	$(INSTALL_DIR) $(1)$(PYTHON_PKG_DIR)
	$(CP) \
		$(PKG_INSTALL_DIR)$(PYTHON_PKG_DIR)/* \
		$(1)$(PYTHON_PKG_DIR)
endef

$(eval $(call BuildPackage,websocket_client))
