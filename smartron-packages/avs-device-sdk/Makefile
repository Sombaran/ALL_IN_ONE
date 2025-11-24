#
# Copyright (C) 2008 Lionic Corporation
#

include $(TOPDIR)/rules.mk

PKG_NAME:=avs-device-sdk
PKG_VERSION:=1.0
PKG_RELEASE:=1

CMAKE_INSTALL:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

CMAKE_OPTIONS += -DCMAKE_CXX_FLAGS="-g -std=c++11 -Wall" 
CMAKE_OPTIONS += -Dgtest_build_samples=OFF
CMAKE_OPTIONS += -Dgtest_build_tests=OFF

TARGET_CFLAGS += -mfloat-abi=softfp -mfpu=neon

#CMAKE_OPTIONS += -DSENSORY_KEY_WORD_DETECTOR=ON
#CMAKE_OPTIONS += -DSENSORY_KEY_WORD_DETECTOR_LIB_PATH=$(BUILD_DIR)/sensory-thf/lib/libsnsr.a
#CMAKE_OPTIONS += -DSENSORY_KEY_WORD_DETECTOR_INCLUDE_DIR=$(BUILD_DIR)/sensory-thf/include/ 

CMAKE_OPTIONS += -DAMAZONLITE_KEY_WORD_DETECTOR=ON
CMAKE_OPTIONS += -DAMAZONLITE_KEY_WORD_DETECTOR_LIB_PATH=$(BUILD_DIR)/amazon_lib/lib/libpryon_lite.a
CMAKE_OPTIONS += -DAMAZONLITE_KEY_WORD_DETECTOR_INCLUDE_DIR=$(BUILD_DIR)/amazon_lib/include/
CMAKE_OPTIONS += -DAMAZONLITE_KEY_WORD_DETECTOR_DYNAMIC_MODEL_LOADING=ON
CMAKE_OPTIONS += -DACSDK_AUTO_TEST_SUPPORTED=ON
#CMAKE_OPTIONS += -DAMAZONLITE_KEY_WORD_DETECTOR_MODEL_CPP_PATH=$(BUILD_DIR)/amazon_lib/arm_neon/D.en-IN.alexa.cpp



CMAKE_OPTIONS += -DGSTREAMER_MEDIA_PLAYER=ON 
CMAKE_OPTIONS += -DCMAKE_PREFIX_PATH=$(BUILD_DIR)/gstreamer-1.12.1/

CMAKE_OPTIONS += -DPORTAUDIO=ON
CMAKE_OPTIONS += -DPORTAUDIO_LIB_PATH=$(BUILD_DIR)/portaudio/ipkg-install/usr/lib/libportaudio.a 
CMAKE_OPTIONS += -DPORTAUDIO_INCLUDE_DIR=$(BUILD_DIR)/portaudio/ipkg-install/usr/include
CMAKE_OPTIONS += -DBLUETOOTH_BLUEZ=ON
CMAKE_OPTIONS += -DBLUETOOTH_BLUEZ_PULSEAUDIO_OVERRIDE_ENDPOINTS=ON
CMAKE_OPTIONS += -DCMAKE_BUILD_TYPE=DEBUG

CMAKE_OPTIONS += -DZMQ_LIB_PATH=$(BUILD_DIR)/libzeromq/ipkg-install/usr/lib/libzmq.a
CMAKE_OPTIONS += -DZMQ_INCLUDE_DIR=$(BUILD_DIR)/libzeromq/ipkg-install/usr/include

CMAKE_OPTIONS += -DJSONC_LIB_PATH=$(BUILD_DIR)/json-c-0.12/ipkg-install/usr/lib/libjson-c.a
CMAKE_OPTIONS += -DJSONC_INCLUDE_DIR=$(BUILD_DIR)/json-c-0.12/ipkg-install/usr/include/json-c

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk
	

define Package/avs-device-sdk
	SECTION:=utils
	DEFAULT:=y
	CATEGORY:=Smartron-Packages
	SUBMENU:=avs-device-sdk
	TITLE:= Alexa sdk on shifu
	DEPENDS:=+libpthread +libcurl +glib2 +libnghttp2 +liboil +libgst1app +libgstbase +libgstreamer +libstdcpp +llediface +shifu-led-interface +portaudio +alsa-lib +libzeromq +libjson-c +lsqlite3 +sbc  +amazon_lib +pulseaudio8-daemon +libpulse
endef

define Package/avs-device-sdk/description
	alexa device sdk on shifu
endef

define Build/Prepare
	mkdir -p  $(PKG_BUILD_DIR)
	echo xxx:$(CP)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(CP) $(STAGING_DIR)/usr/lib/libsqlite3.so* $(PKG_BUILD_DIR)/
endef


define Build/Compile
    $(MAKE) -C $(PKG_BUILD_DIR) 
endef

define Package/avs-device-sdk/install
	
	#deploying libraries
	
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ACL/src/libACL.so $(1)/usr/lib		
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ADSL/src/libADSL.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/AFML/src/libAFML.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/AVSCommon/libAVSCommon.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ApplicationUtilities/DefaultClient/src/libDefaultClient.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ApplicationUtilities/Resources/Audio/src/libAudioResources.so $(1)/usr/lib	
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/BluetoothImplementations/BlueZ/src/libBluetoothImplementationsBlueZ.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/CapabilityAgents/AIP/src/libAIP.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/CapabilityAgents/AudioPlayer/src/libAudioPlayer.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/CapabilityAgents/Bluetooth/src/libBluetooth.so $(1)/usr/lib    			
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/CapabilityAgents/System/src/libAVSSystem.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/CapabilityAgents/Alerts/src/libAlerts.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/CapabilityAgents/Equalizer/src/libEqualizer.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/CapabilityAgents/ExternalMediaPlayer/src/libExternalMediaPlayer.so $(1)/usr/lib 				
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/CapabilityAgents/SpeechSynthesizer/src/libSpeechSynthesizer.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/CapabilityAgents/Notifications/src/libNotifications.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/CapabilityAgents/PlaybackController/src/libPlaybackController.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/CapabilityAgents/Settings/src/libSettings.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/CapabilityAgents/SpeakerManager/src/libSpeakerManager.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/CapabilityAgents/TemplateRuntime/src/libTemplateRuntime.so $(1)/usr/lib	
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/CapabilityAgents/InteractionModel/src/libInteractionModel.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/CapabilityAgents/DoNotDisturb/src/libDoNotDisturbCA.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/CapabilitiesDelegate/src/libCapabilitiesDelegate.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/CertifiedSender/src/libCertifiedSender.so $(1)/usr/lib	
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ContextManager/src/libContextManager.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/EqualizerImplementations/src/libEqualizerImplementations.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ESP/src/libESP.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/KWD/src/libKWD.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/KWD/KWDProvider/src/libKeywordDetectorProvider.so $(1)/usr/lib	
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/KWD/AmazonLite/src/libAMAZONLITE.so $(1)/usr/lib
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/KWD/Sensory/src/libSENSORY.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/MediaPlayer/GStreamerMediaPlayer/src/libMediaPlayer.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/PlaylistParser/src/libPlaylistParser.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/RegistrationManager/src/libRegistrationManager.so $(1)/usr/lib		
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/KWD/Sensory/src/libSENSORY.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/SampleApp/Authorization/CBLAuthDelegate/src/libCBLAuthDelegate.so $(1)/usr/lib	
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/Storage/SQLiteStorage/src/libSQLiteStorage.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/Settings/src/libDeviceSettings.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/SpeechEncoder/src/libSpeechEncoder.so $(1)/usr/lib
	
	
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/../openssl-1.0.2l/libcrypto.so.1.0.0 $(1)/usr/lib
	
	
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/SampleApp/src/SampleApp $(1)/usr/bin/
	
	
	$(CP) $(STAGING_DIR)/usr/lib/libsqlite3.so.0 $(1)/usr/bin/
	
	
	$(INSTALL_DIR) $(1)/root/va_data
	$(CP) ./files/va_data/* $(1)/root/va_data/

	
	$(INSTALL_DIR) $(1)/root/va_data/alexa
	$(CP) ./files/config/* $(1)/root/va_data/alexa/
	
		
	#deploying voice assistant service
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/va_service.init $(1)/etc/init.d/va_service
		
endef

$(eval $(call BuildPackage,$(PKG_NAME)))

