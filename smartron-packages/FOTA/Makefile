##############################################
# OpenWrt Makefile for FOTA program
#
#
# Most of the variables used here are defined in
# the include directives below. We just need to
# specify a basic description of the package,
# where to build our program, where to find
# the source files, and where to install the
# compiled program on the router.
#
# Be very careful of spacing in this file.
# Indents should be tabs, not spaces, and
# there should be no trailing whitespace in
# lines that are not commented.
#
##############################################

include $(TOPDIR)/rules.mk

# Name and release number of this package
PKG_NAME:=FOTA
PKG_VERSION:=3.0.4
PKG_RELEASE:=1

# This specifies the directory where we're going to build the program.
# The root build directory, $(BUILD_DIR), is by default the build_mipsel
# directory in your OpenWrt SDK directory
PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

# Specify package information for this program.
# The variables defined here should be self explanatory.
define Package/$(PKG_NAME)
	SUBMENU:=FOTA
	SECTION:=utils
	CATEGORY:=Smartron-Packages
	TITLE:=FOTA -- FOTA service in Native C to download firmware updates and do system upgrade.
	DEPENDS:=+fw-env +libcurl +libjson-c +libpthread +libsqlite3 +libssl +libcrypto
endef

define Package/$(PKG_NAME)/description
	FOTA service in Native C to download firmware updates and do system upgrade.
endef

# Specify what needs to be done to prepare for building the package.
# In our case, we need to copy the source files to the build directory.
# This is NOT the default.  The default uses the PKG_SOURCE_URL and the
# PKG_SOURCE which is not defined here to download the source from the web.
# In order to just build a simple program that we have just written, it is
# much easier to do it this way.
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

# We do not need to define Build/Configure or Build/Compile directives
# The defaults are appropriate for compiling a simple program such as this one\
# Specify where and how to install the program. Since we only have one file,
# the Device_Manager_Gateway executable, install it by copying it to the /bin directory on
# the router. The $(1) variable represents the root directory on the router running
# OpenWrt. The $(INSTALL_DIR) variable contains a command to prepare the install
# directory if it does not already exist.  Likewise $(INSTALL_BIN) contains the
# command to copy the binary file from its current location (in our case the build
# directory) to the install directory.
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/fota_update $(1)/usr/bin
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/end_device_upgrade $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/fota_client_dm $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/fota_client_ma $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/*.init $(1)/etc/init.d/fota_service
	$(INSTALL_DIR) $(1)/etc/
	$(INSTALL_DATA) ./files/dnld_firmware_info.json $(1)/etc/dnld_firmware_info.json
	$(INSTALL_DIR) $(1)/etc/
	$(INSTALL_DATA) ./files/shifu_version.json $(1)/etc/shifu_version.json
	$(INSTALL_DIR) $(1)/etc/
	$(INSTALL_DATA) ./files/fota_status.json $(1)/etc/fota_status.json
	$(INSTALL_DIR) $(1)/etc/fw
	$(INSTALL_BIN) ./files/firmware_update.sh $(1)/etc/fw/firmware_update
	$(INSTALL_DATA) ./files/*.pem $(1)/etc/fw/fota-public.pem
	$(INSTALL_DIR) $(1)/root/downloads/fw/
	$(INSTALL_DIR) $(1)/etc/ota/
	$(INSTALL_DATA) ./files/dnld_SMARTRON*.json $(1)/etc/ota
	$(INSTALL_DIR) $(1)/root/downloads/fw/enddevota/
endef

# This line executes the necessary commands to compile our program.
# The above define directives specify all the information needed, but this
# line calls BuildPackage which in turn actually uses this information to
# build a package.
$(eval $(call BuildPackage,FOTA))
