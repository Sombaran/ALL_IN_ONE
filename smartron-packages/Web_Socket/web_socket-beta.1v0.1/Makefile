# Makefile to build Python Package
include $(TOPDIR)/rules.mk

PKG_NAME:=web_socket
PKG_VERSION:=0.0.3
PKG_RELEASE:=1
PKG_SOURCE:=web_socket-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/web_socket-$(PKG_VERSION)
PKG_BUILD_DEPENDS:=python

include $(INCLUDE_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/$(PKG_NAME)
  SUBMENU:=Shifu_Agents
  SECTION:=utils
  CATEGORY:=Smartron-Packages
  TITLE:=Web Socket
  DEPENDS:=+python +librt +libstdcpp +libzeromq +pyzmq +socketIO-client-0.7.2 +requests-2.7.0 +certifi_2017_11_5 +chardet-3.0.4 +idna-2.6 +pluggy-0.6.0 +py-1.5.2 +setproctitle-1.1.10 +six-1.11.0 +urllib3-1.22 +websocket_client +click-6.7 +Flask-0.12.2 +itsdangerous-0.24 +Jinja2-2.10 +MarkupSafe-1.0 +Werkzeug-0.14.1 +mobile_agent-1.0 +backports.ssl-match-hostname 
endef

define Package/$(PKG_NAME)/description
  Web Socket service is a package through which Shifu Hub communicates with Tronx. 
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(call Build/Compile/PyMod,, install  --prefix="$(PKG_INSTALL_DIR)/usr")
endef

define Package/$(PKG_NAME)/install
        $(INSTALL_DIR) $(1)$(PYTHON_PKG_DIR)
	$(INSTALL_DIR) $(1)/exe_dir
	$(CP) \
		$(PKG_INSTALL_DIR)$(PYTHON_PKG_DIR)/* \
		$(1)$(PYTHON_PKG_DIR) 
	$(CP) $(PKG_BUILD_DIR)/easy-install.pth $(1)$(PYTHON_PKG_DIR)
	
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) ./files/Web_Socket.py $(1)/usr/bin/
	
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/ws_service.init $(1)/etc/init.d/ws_service
endef

$(eval $(call BuildPackage,web_socket))
