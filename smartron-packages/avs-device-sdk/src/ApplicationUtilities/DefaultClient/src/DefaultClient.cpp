/*
 * Copyright 2017-2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *     http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include "DefaultClient/DefaultClient.h"
#include <ADSL/MessageInterpreter.h>
#include <AVSCommon/AVS/Attachment/AttachmentManager.h>
#include <AVSCommon/AVS/ExceptionEncounteredSender.h>
#include <AVSCommon/Utils/Bluetooth/BluetoothEventBus.h>
#include "AVSCommon/Utils/UCIInterface.h"

/**
 * Bug #21123 : Start
 * Description : Device is not indicating the error state, after disconnecting Ethernet
 * Changed by Smartron India Pvt. Ltd.
 * */
#include <curl/curl.h>
#include "shifu-led.h"
#include "AVSCommon/Utils/LedInterface.h"
/**
 * Bug #21123 : End
 * */

#ifdef ENABLE_OPUS
#include <SpeechEncoder/OpusEncoderContext.h>
#endif

#ifdef ENABLE_COMMS
#include <CallManager/CallManager.h>
#include <CallManager/SipUserAgent.h>
#endif

#ifdef ENABLE_PCC
#include <AVSCommon/SDKInterfaces/Phone/PhoneCallerInterface.h>
#include <PhoneCallController/PhoneCallController.h>
#endif

#ifdef ENABLE_MRM
#include <MRMHandler/MRMHandler.h>
#endif

#include <Settings/SettingsUpdatedEventSender.h>
#include <System/EndpointHandler.h>
#include <System/SystemCapabilityProvider.h>
#include <System/UserInactivityMonitor.h>

#ifdef BLUETOOTH_BLUEZ
#include <Bluetooth/BluetoothAVRCPTransformer.h>
#include <BlueZ/BlueZBluetoothDeviceManager.h>
#endif

// Redmine 21542: start: After completion of Registration,First wakeword is not detecting playing announcement as "device is not registered"
extern bool isDeviceRegisteredWithAVS;
// Redmine 21542: End.

// #22267 Start: Synchronization between LED server & AVS after Booting of Shifu
extern bool isAVSinInitFirstTime;
extern std::mutex avs_config;
// #22267 End.

/* #21139:variable to know about the DND state.*/
extern int Dnd_state;
#define BUFFER_MAX    2
namespace alexaClientSDK {
namespace defaultClient {

using namespace alexaClientSDK::avsCommon::sdkInterfaces;

/// String identifier for 'Alexa Stop' return by wake word engine
static const std::string ALEXA_STOP_KEYWORD = "STOP";

/// String to identify log entries originating from this file.
static const std::string TAG("DefaultClient");
/**
 * Bug #21123 : Start
 * Description : Device is not indicating the error state, after disconnecting Ethernet
 * Changed by Smartron India Pvt. Ltd.
 * */
/// Interval to check internet connectivity with curl
static const std::chrono::seconds DEFAULT_CLIENT_CHECK_NETWORK_INTERVAL(1);
/**
 * Bug #21123 : End
 * */

/**
 * Create a LogEntry using this file's TAG and the specified event string.
 *
 * @param The event string for this @c LogEntry.
 */
#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)
int get_val_frm_conf(int *val);

int DefaultClient::m_isNetworkConnected = UNKNOWN;

std::unique_ptr<DefaultClient> DefaultClient::create(
    std::shared_ptr<avsCommon::utils::DeviceInfo> deviceInfo,
    std::shared_ptr<registrationManager::CustomerDataManager> customerDataManager,
    const std::unordered_map<std::string, std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerInterface>>&
        externalMusicProviderMediaPlayers,
    const std::unordered_map<std::string, std::shared_ptr<avsCommon::sdkInterfaces::SpeakerInterface>>&
        externalMusicProviderSpeakers,
    const capabilityAgents::externalMediaPlayer::ExternalMediaPlayer::AdapterCreationMap& adapterCreationMap,
    std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerInterface> speakMediaPlayer,
    std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerInterface> audioMediaPlayer,
    std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerInterface> alertsMediaPlayer,
    std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerInterface> notificationsMediaPlayer,
    std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerInterface> bluetoothMediaPlayer,
    std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerInterface> ringtoneMediaPlayer,
    std::shared_ptr<avsCommon::sdkInterfaces::SpeakerInterface> speakSpeaker,
    std::shared_ptr<avsCommon::sdkInterfaces::SpeakerInterface> audioSpeaker,
    std::shared_ptr<avsCommon::sdkInterfaces::SpeakerInterface> alertsSpeaker,
    std::shared_ptr<avsCommon::sdkInterfaces::SpeakerInterface> notificationsSpeaker,
    std::shared_ptr<avsCommon::sdkInterfaces::SpeakerInterface> bluetoothSpeaker,
    std::shared_ptr<avsCommon::sdkInterfaces::SpeakerInterface> ringtoneSpeaker,
    const std::vector<std::shared_ptr<avsCommon::sdkInterfaces::SpeakerInterface>>& additionalSpeakers,
#ifdef ENABLE_PCC
    std::shared_ptr<avsCommon::sdkInterfaces::SpeakerInterface> phoneSpeaker,
    std::shared_ptr<avsCommon::sdkInterfaces::phone::PhoneCallerInterface> phoneCaller,
#endif
    std::shared_ptr<EqualizerRuntimeSetup> equalizerRuntimeSetup,
    std::shared_ptr<avsCommon::sdkInterfaces::audio::AudioFactoryInterface> audioFactory,
    std::shared_ptr<avsCommon::sdkInterfaces::AuthDelegateInterface> authDelegate,
    std::shared_ptr<capabilityAgents::alerts::storage::AlertStorageInterface> alertStorage,
    std::shared_ptr<certifiedSender::MessageStorageInterface> messageStorage,
    std::shared_ptr<capabilityAgents::notifications::NotificationsStorageInterface> notificationsStorage,
    std::shared_ptr<capabilityAgents::settings::SettingsStorageInterface> settingsStorage,
    std::unique_ptr<settings::storage::DeviceSettingStorageInterface> deviceSettingStorage,
    std::shared_ptr<capabilityAgents::bluetooth::BluetoothStorageInterface> bluetoothStorage,
    std::unordered_set<std::shared_ptr<avsCommon::sdkInterfaces::DialogUXStateObserverInterface>>
        alexaDialogStateObservers,
    std::unordered_set<std::shared_ptr<avsCommon::sdkInterfaces::ConnectionStatusObserverInterface>>
        connectionObservers,
    std::shared_ptr<avsCommon::utils::network::InternetConnectionMonitor> internetConnectionMonitor,
    bool isGuiSupported,
    std::shared_ptr<avsCommon::sdkInterfaces::CapabilitiesDelegateInterface> capabilitiesDelegate,
    std::shared_ptr<avsCommon::sdkInterfaces::ContextManagerInterface> contextManager,
    std::shared_ptr<alexaClientSDK::acl::TransportFactoryInterface> transportFactory,
    avsCommon::sdkInterfaces::softwareInfo::FirmwareVersion firmwareVersion,
    bool sendSoftwareInfoOnConnected,
    std::shared_ptr<avsCommon::sdkInterfaces::SoftwareInfoSenderObserverInterface> softwareInfoSenderObserver,
    std::unique_ptr<avsCommon::sdkInterfaces::bluetooth::BluetoothDeviceManagerInterface> bluetoothDeviceManager) {
    std::unique_ptr<DefaultClient> defaultClient(new DefaultClient());
    if (!defaultClient->initialize(
            deviceInfo,
            customerDataManager,
            externalMusicProviderMediaPlayers,
            externalMusicProviderSpeakers,
            adapterCreationMap,
            speakMediaPlayer,
            audioMediaPlayer,
            alertsMediaPlayer,
            notificationsMediaPlayer,
            bluetoothMediaPlayer,
            ringtoneMediaPlayer,
            speakSpeaker,
            audioSpeaker,
            alertsSpeaker,
            notificationsSpeaker,
            bluetoothSpeaker,
            ringtoneSpeaker,
            additionalSpeakers,
#ifdef ENABLE_PCC
            phoneSpeaker,
            phoneCaller,
#endif
            equalizerRuntimeSetup,
            audioFactory,
            authDelegate,
            alertStorage,
            messageStorage,
            notificationsStorage,
            settingsStorage,
            std::move(deviceSettingStorage),
            bluetoothStorage,
            alexaDialogStateObservers,
            connectionObservers,
            internetConnectionMonitor,
            isGuiSupported,
            capabilitiesDelegate,
            contextManager,
            transportFactory,
            firmwareVersion,
            sendSoftwareInfoOnConnected,
            softwareInfoSenderObserver,
            std::move(bluetoothDeviceManager))) {
        return nullptr;
    }

    return defaultClient;
}

bool DefaultClient::initialize(
    std::shared_ptr<avsCommon::utils::DeviceInfo> deviceInfo,
    std::shared_ptr<registrationManager::CustomerDataManager> customerDataManager,
    const std::unordered_map<std::string, std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerInterface>>&
        externalMusicProviderMediaPlayers,
    const std::unordered_map<std::string, std::shared_ptr<avsCommon::sdkInterfaces::SpeakerInterface>>&
        externalMusicProviderSpeakers,
    const capabilityAgents::externalMediaPlayer::ExternalMediaPlayer::AdapterCreationMap& adapterCreationMap,
    std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerInterface> speakMediaPlayer,
    std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerInterface> audioMediaPlayer,
    std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerInterface> alertsMediaPlayer,
    std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerInterface> notificationsMediaPlayer,
    std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerInterface> bluetoothMediaPlayer,
    std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerInterface> ringtoneMediaPlayer,
    std::shared_ptr<avsCommon::sdkInterfaces::SpeakerInterface> speakSpeaker,
    std::shared_ptr<avsCommon::sdkInterfaces::SpeakerInterface> audioSpeaker,
    std::shared_ptr<avsCommon::sdkInterfaces::SpeakerInterface> alertsSpeaker,
    std::shared_ptr<avsCommon::sdkInterfaces::SpeakerInterface> notificationsSpeaker,
    std::shared_ptr<avsCommon::sdkInterfaces::SpeakerInterface> bluetoothSpeaker,
    std::shared_ptr<avsCommon::sdkInterfaces::SpeakerInterface> ringtoneSpeaker,
    const std::vector<std::shared_ptr<avsCommon::sdkInterfaces::SpeakerInterface>>& additionalSpeakers,
#ifdef ENABLE_PCC
    std::shared_ptr<avsCommon::sdkInterfaces::SpeakerInterface> phoneSpeaker,
    std::shared_ptr<avsCommon::sdkInterfaces::phone::PhoneCallerInterface> phoneCaller,
#endif
    std::shared_ptr<EqualizerRuntimeSetup> equalizerRuntimeSetup,
    std::shared_ptr<avsCommon::sdkInterfaces::audio::AudioFactoryInterface> audioFactory,
    std::shared_ptr<avsCommon::sdkInterfaces::AuthDelegateInterface> authDelegate,
    std::shared_ptr<capabilityAgents::alerts::storage::AlertStorageInterface> alertStorage,
    std::shared_ptr<certifiedSender::MessageStorageInterface> messageStorage,
    std::shared_ptr<capabilityAgents::notifications::NotificationsStorageInterface> notificationsStorage,
    std::shared_ptr<capabilityAgents::settings::SettingsStorageInterface> settingsStorage,
    std::shared_ptr<settings::storage::DeviceSettingStorageInterface> deviceSettingStorage,
    std::shared_ptr<capabilityAgents::bluetooth::BluetoothStorageInterface> bluetoothStorage,
    std::unordered_set<std::shared_ptr<avsCommon::sdkInterfaces::DialogUXStateObserverInterface>>
        alexaDialogStateObservers,
    std::unordered_set<std::shared_ptr<avsCommon::sdkInterfaces::ConnectionStatusObserverInterface>>
        connectionObservers,
    std::shared_ptr<avsCommon::utils::network::InternetConnectionMonitor> internetConnectionMonitor,
    bool isGuiSupported,
    std::shared_ptr<avsCommon::sdkInterfaces::CapabilitiesDelegateInterface> capabilitiesDelegate,
    std::shared_ptr<avsCommon::sdkInterfaces::ContextManagerInterface> contextManager,
    std::shared_ptr<alexaClientSDK::acl::TransportFactoryInterface> transportFactory,
    avsCommon::sdkInterfaces::softwareInfo::FirmwareVersion firmwareVersion,
    bool sendSoftwareInfoOnConnected,
    std::shared_ptr<avsCommon::sdkInterfaces::SoftwareInfoSenderObserverInterface> softwareInfoSenderObserver,
    std::unique_ptr<avsCommon::sdkInterfaces::bluetooth::BluetoothDeviceManagerInterface> bluetoothDeviceManager) {
    if (!audioFactory) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "nullAudioFactory"));
        return false;
    }

    if (!speakMediaPlayer) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "nullSpeakMediaPlayer"));
        return false;
    }

    if (!audioMediaPlayer) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "nullAudioMediaPlayer"));
        return false;
    }

    if (!alertsMediaPlayer) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "nullAlertsMediaPlayer"));
        return false;
    }

    if (!notificationsMediaPlayer) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "nullNotificationsMediaPlayer"));
        return false;
    }

    if (!bluetoothMediaPlayer) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "nullBluetoothMediaPlayer"));
        return false;
    }

    if (!ringtoneMediaPlayer) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "nullRingtoneMediaPlayer"));
        return false;
    }

    if (!authDelegate) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "nullAuthDelegate"));
        return false;
    }

    if (!capabilitiesDelegate) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "nullCapabilitiesDelegate"));
        return false;
    }

    if (!deviceSettingStorage) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "nullDeviceSettingStorage"));
        return false;
    }
    m_deviceSettingStorage = deviceSettingStorage;
    if (!m_deviceSettingStorage->open()) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "deviceSettingStorageOpenFailed"));
        return false;
    }

    m_deviceSettingsManager = std::make_shared<settings::DeviceSettingsManager>();

    if (!m_deviceSettingsManager) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "createDeviceSettingsManagerFailed"));
        return false;
    }

    if (!contextManager) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "nullContextManager"));
        return false;
    }

    if (!transportFactory) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "nullTransportFactory"));
        return false;
    }

    m_dialogUXStateAggregator = std::make_shared<avsCommon::avs::DialogUXStateAggregator>();

    for (auto observer : alexaDialogStateObservers) {
        m_dialogUXStateAggregator->addObserver(observer);
    }

    /*
     * Creating the Attachment Manager - This component deals with managing attachments and allows for readers and
     * writers to be created to handle the attachment.
     */
    auto attachmentManager = std::make_shared<avsCommon::avs::attachment::AttachmentManager>(
        avsCommon::avs::attachment::AttachmentManager::AttachmentType::IN_PROCESS);

    /*
     * Creating the message router - This component actually maintains the connection to AVS over HTTP2. It is created
     * using the auth delegate, which provides authorization to connect to AVS, and the attachment manager, which helps
     * ACL write attachments received from AVS.
     */
    m_messageRouter = std::make_shared<acl::MessageRouter>(authDelegate, attachmentManager, transportFactory);

    /*
     * Creating the connection manager - This component is the overarching connection manager that glues together all
     * the other networking components into one easy-to-use component.
     */
    m_connectionManager =
        acl::AVSConnectionManager::create(m_messageRouter, false, connectionObservers, {m_dialogUXStateAggregator});
    if (!m_connectionManager) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateConnectionManager"));
        return false;
    }

    if (!internetConnectionMonitor) {
        ACSDK_CRITICAL(LX("initializeFailed").d("reason", "internetConnectionMonitor was nullptr"));
        return false;
    }
    m_internetConnectionMonitor = internetConnectionMonitor;

    /*
     * Creating our certified sender - this component guarantees that messages given to it (expected to be JSON
     * formatted AVS Events) will be sent to AVS.  This nicely decouples strict message sending from components which
     * require an Event be sent, even in conditions when there is no active AVS connection.
     */
    m_certifiedSender = certifiedSender::CertifiedSender::create(
        m_connectionManager, m_connectionManager, messageStorage, customerDataManager);
    if (!m_certifiedSender) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateCertifiedSender"));
        return false;
    }

    /*
     * Creating the Exception Sender - This component helps the SDK send exceptions when it is unable to handle a
     * directive sent by AVS. For that reason, the Directive Sequencer and each Capability Agent will need this
     * component.
     */
    m_exceptionSender = avsCommon::avs::ExceptionEncounteredSender::create(m_connectionManager);
    if (!m_exceptionSender) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateExceptionSender"));
        return false;
    }

    /*
     * Creating the Directive Sequencer - This is the component that deals with the sequencing and ordering of
     * directives sent from AVS and forwarding them along to the appropriate Capability Agent that deals with
     * directives in that Namespace/Name.
     */
    m_directiveSequencer = adsl::DirectiveSequencer::create(m_exceptionSender);
    if (!m_directiveSequencer) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateDirectiveSequencer"));
        return false;
    }

    /*
     * Creating the Message Interpreter - This component takes care of converting ACL messages to Directives for the
     * Directive Sequencer to process. This essentially "glues" together the ACL and ADSL.
     */
    auto messageInterpreter =
        std::make_shared<adsl::MessageInterpreter>(m_exceptionSender, m_directiveSequencer, attachmentManager);

    m_connectionManager->addMessageObserver(messageInterpreter);

    /*
     * Creating the Registration Manager - This component is responsible for implementing any customer registration
     * operation such as login and logout
     */
    m_registrationManager = std::make_shared<registrationManager::RegistrationManager>(
        m_directiveSequencer, m_connectionManager, customerDataManager);

    /*
     * Creating the Audio Activity Tracker - This component is responsibly for reporting the audio channel focus
     * information to AVS.
     */
    m_audioActivityTracker = afml::AudioActivityTracker::create(contextManager);

    /*
     * Creating the Focus Manager - This component deals with the management of layered audio focus across various
     * components. It handles granting access to Channels as well as pushing different "Channels" to foreground,
     * background, or no focus based on which other Channels are active and the priorities of those Channels. Each
     * Capability Agent will require the Focus Manager in order to request access to the Channel it wishes to play on.
     */
    m_audioFocusManager =
        std::make_shared<afml::FocusManager>(afml::FocusManager::getDefaultAudioChannels(), m_audioActivityTracker);

    /*
     * Creating the User Inactivity Monitor - This component is responsibly for updating AVS of user inactivity as
     * described in the System Interface of AVS.
     */
    m_userInactivityMonitor =
        capabilityAgents::system::UserInactivityMonitor::create(m_connectionManager, m_exceptionSender);
    if (!m_userInactivityMonitor) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateUserInactivityMonitor"));
        return false;
    }

/*
 * Creating the Audio Input Processor - This component is the Capability Agent that implements the
 * SpeechRecognizer interface of AVS.
 */
#ifdef ENABLE_OPUS
    m_audioInputProcessor = capabilityAgents::aip::AudioInputProcessor::create(
        m_directiveSequencer,
        m_connectionManager,
        contextManager,
        m_audioFocusManager,
        m_dialogUXStateAggregator,
        m_exceptionSender,
        m_userInactivityMonitor,
        std::make_shared<speechencoder::SpeechEncoder>(std::make_shared<speechencoder::OpusEncoderContext>()));
#else
    m_audioInputProcessor = capabilityAgents::aip::AudioInputProcessor::create(
        m_directiveSequencer,
        m_connectionManager,
        contextManager,
        m_audioFocusManager,
        m_dialogUXStateAggregator,
        m_exceptionSender,
        m_userInactivityMonitor);
#endif

    if (!m_audioInputProcessor) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateAudioInputProcessor"));
        return false;
    }

    m_audioInputProcessor->addObserver(m_dialogUXStateAggregator);

    /*
     * Creating the Speech Synthesizer - This component is the Capability Agent that implements the SpeechSynthesizer
     * interface of AVS.
     */
    m_speechSynthesizer = capabilityAgents::speechSynthesizer::SpeechSynthesizer::create(
        speakMediaPlayer,
        m_connectionManager,
        m_audioFocusManager,
        contextManager,
        m_exceptionSender,
        m_dialogUXStateAggregator);
    if (!m_speechSynthesizer) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateSpeechSynthesizer"));
        return false;
    }

    m_speechSynthesizer->addObserver(m_dialogUXStateAggregator);

    /*
     * Creating the PlaybackController Capability Agent - This component is the Capability Agent that implements the
     * PlaybackController interface of AVS.
     */
    m_playbackController =
        capabilityAgents::playbackController::PlaybackController::create(contextManager, m_connectionManager);
    if (!m_playbackController) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreatePlaybackController"));
        return false;
    }

    /*
     * Creating the PlaybackRouter - This component routes a playback button press to the active handler.
     * The default handler is @c PlaybackController.
     */
    m_playbackRouter = capabilityAgents::playbackController::PlaybackRouter::create(m_playbackController);
    if (!m_playbackRouter) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreatePlaybackRouter"));
        return false;
    }

    /*
     * Creating the Audio Player - This component is the Capability Agent that implements the AudioPlayer
     * interface of AVS.
     */
    m_audioPlayer = capabilityAgents::audioPlayer::AudioPlayer::create(
        audioMediaPlayer,
        m_connectionManager,
        m_audioFocusManager,
        contextManager,
        m_exceptionSender,
        m_playbackRouter);
    if (!m_audioPlayer) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateAudioPlayer"));
        return false;
    }

    std::vector<std::shared_ptr<avsCommon::sdkInterfaces::SpeakerInterface>> allSpeakers = {
        speakSpeaker, audioSpeaker, alertsSpeaker, notificationsSpeaker, bluetoothSpeaker, ringtoneSpeaker};

#ifdef ENABLE_PCC
    allSpeakers.push_back(phoneSpeaker);
#endif

    allSpeakers.insert(allSpeakers.end(), additionalSpeakers.begin(), additionalSpeakers.end());

    /*
     * Creating the SpeakerManager Capability Agent - This component is the Capability Agent that implements the
     * Speaker interface of AVS.
     */
    //#22273:Added focusmanager object to know about channel active or not.
    m_speakerManager = capabilityAgents::speakerManager::SpeakerManager::create(
        allSpeakers, contextManager, m_connectionManager, m_exceptionSender, m_audioFocusManager);
    if (!m_speakerManager) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateSpeakerManager"));
        return false;
    }

    /*
     * Creating the Alerts Capability Agent - This component is the Capability Agent that implements the Alerts
     * interface of AVS.
     */
    m_alertsCapabilityAgent = capabilityAgents::alerts::AlertsCapabilityAgent::create(
        m_connectionManager,
        m_connectionManager,
        m_certifiedSender,
        m_audioFocusManager,
        m_speakerManager,
        contextManager,
        m_exceptionSender,
        alertStorage,
        audioFactory->alerts(),
        capabilityAgents::alerts::renderer::Renderer::create(alertsMediaPlayer),
        customerDataManager);
    if (!m_alertsCapabilityAgent) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateAlertsCapabilityAgent"));
        return false;
    }

    addConnectionObserver(m_dialogUXStateAggregator);

    /*
     * Creating the Notifications Capability Agent - This component is the Capability Agent that implements the
     * Notifications interface of AVS.
     */
    m_notificationsCapabilityAgent = capabilityAgents::notifications::NotificationsCapabilityAgent::create(
        notificationsStorage,
        capabilityAgents::notifications::NotificationRenderer::create(notificationsMediaPlayer),
        contextManager,
        m_exceptionSender,
        audioFactory->notifications(),
        customerDataManager);
    if (!m_notificationsCapabilityAgent) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateNotificationsCapabilityAgent"));
        return false;
    }

    m_interactionCapabilityAgent = capabilityAgents::interactionModel::InteractionModelCapabilityAgent::create(
        m_directiveSequencer, m_exceptionSender);
    if (!m_interactionCapabilityAgent) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateInteractionModelCapabilityAgent"));
        return false;
    }

#ifdef ENABLE_PCC
    /*
     * Creating the PhoneCallController - This component is the Capability Agent that implements the
     * PhoneCallController interface of AVS
     */
    m_phoneCallControllerCapabilityAgent = capabilityAgents::phoneCallController::PhoneCallController::create(
        contextManager, m_connectionManager, phoneCaller, phoneSpeaker, m_audioFocusManager, m_exceptionSender);
    if (!m_phoneCallControllerCapabilityAgent) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreatePhoneCallControllerCapabilityAgent"));
    }
#endif

#ifdef ENABLE_COMMS
    if (!ringtoneMediaPlayer) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "nullRingtoneMediaPlayer"));
        return false;
    }

    auto sipUserAgent = std::make_shared<capabilityAgents::callManager::SipUserAgent>();

    if (!capabilityAgents::callManager::CallManager::create(
            sipUserAgent,
            ringtoneMediaPlayer,
            ringtoneSpeaker,
            m_connectionManager,
            contextManager,
            m_audioFocusManager,
            m_exceptionSender,
            audioFactory->communications())) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateCallManager"));
        return false;
    }

    m_callManager = capabilityAgents::callManager::CallManager::getInstance();
    addConnectionObserver(m_callManager);
#endif

    std::shared_ptr<capabilityAgents::settings::SettingsUpdatedEventSender> settingsUpdatedEventSender =
        alexaClientSDK::capabilityAgents::settings::SettingsUpdatedEventSender::create(m_certifiedSender);
    if (!settingsUpdatedEventSender) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateSettingsObserver"));
        return false;
    }

    /*
     * Creating the Setting object - This component implements the Setting interface of AVS.
     */
    m_settings = capabilityAgents::settings::Settings::create(
        settingsStorage, {settingsUpdatedEventSender}, customerDataManager);

    if (!m_settings) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateSettingsObject"));
        return false;
    }

    /*
     * Creating the ExternalMediaPlayer CA - This component is the Capability Agent that implements the
     * ExternalMediaPlayer interface of AVS.
     */
    m_externalMediaPlayer = capabilityAgents::externalMediaPlayer::ExternalMediaPlayer::create(
        externalMusicProviderMediaPlayers,
        externalMusicProviderSpeakers,
        adapterCreationMap,
        m_speakerManager,
        m_connectionManager,
        m_audioFocusManager,
        contextManager,
        m_exceptionSender,
        m_playbackRouter);
    if (!m_externalMediaPlayer) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateExternalMediaPlayer"));
        return false;
    }

    if (isGuiSupported) {
        /*
         * Creating the Visual Activity Tracker - This component is responsibly for reporting the visual channel focus
         * information to AVS.
         */
        m_visualActivityTracker = afml::VisualActivityTracker::create(contextManager);

        /*
         * Creating the Visual Focus Manager - This component deals with the management of visual focus across various
         * components. It handles granting access to Channels as well as pushing different "Channels" to foreground,
         * background, or no focus based on which other Channels are active and the priorities of those Channels. Each
         * Capability Agent will require the Focus Manager in order to request access to the Channel it wishes to play
         * on.
         */
        m_visualFocusManager = std::make_shared<afml::FocusManager>(
            afml::FocusManager::getDefaultVisualChannels(), m_visualActivityTracker);

        /*
         * Creating the TemplateRuntime Capability Agent - This component is the Capability Agent that implements the
         * TemplateRuntime interface of AVS.
         */
        m_templateRuntime = capabilityAgents::templateRuntime::TemplateRuntime::create(
            m_audioPlayer, m_visualFocusManager, m_exceptionSender);
        if (!m_templateRuntime) {
            ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateTemplateRuntimeCapabilityAgent"));
            return false;
        }
        m_dialogUXStateAggregator->addObserver(m_templateRuntime);
    }

#ifdef ENABLE_MRM
    /*
     * Creating the MRM (Multi-Room-Music) Capability Agent.
     */

    auto mrmHandler = capabilityAgents::mrm::mrmHandler::MRMHandler::create(
        m_connectionManager,
        m_connectionManager,
        m_directiveSequencer,
        m_userInactivityMonitor,
        contextManager,
        m_audioFocusManager,
        m_speakerManager,
        deviceInfo->getDeviceSerialNumber());

    if (!mrmHandler) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "Unable to create mrmHandler."));
        return false;
    }

    m_mrmCapabilityAgent = capabilityAgents::mrm::MRMCapabilityAgent::create(
        std::move(mrmHandler), m_speakerManager, m_userInactivityMonitor, m_exceptionSender);

    if (!m_mrmCapabilityAgent) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateMRMCapabilityAgent"));
        return false;
    }

#endif

    /*
     * Creating the DoNotDisturb Capability Agent
     */
    /* #21139: Adding FocusManager Interface object */
    m_dndCapabilityAgent = capabilityAgents::doNotDisturb::DoNotDisturbCapabilityAgent::create(
        customerDataManager, m_exceptionSender, m_connectionManager, m_deviceSettingsManager, m_deviceSettingStorage, m_audioFocusManager);

    if (!m_dndCapabilityAgent) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateDNDCapabilityAgent"));
        return false;
    }

    addConnectionObserver(m_dndCapabilityAgent);

    /*
     * Creating the Equalizer Capability Agent and related implementations if enabled
     */

    m_equalizerRuntimeSetup = equalizerRuntimeSetup;
    if (nullptr != m_equalizerRuntimeSetup) {
        auto equalizerController = equalizer::EqualizerController::create(
            equalizerRuntimeSetup->getModeController(),
            equalizerRuntimeSetup->getConfiguration(),
            equalizerRuntimeSetup->getStorage());

        if (!equalizerController) {
            ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateEqualizerController"));
            return false;
        }

        m_equalizerCapabilityAgent = capabilityAgents::equalizer::EqualizerCapabilityAgent::create(
            equalizerController,
            capabilitiesDelegate,
            equalizerRuntimeSetup->getStorage(),
            customerDataManager,
            m_exceptionSender,
            contextManager,
            m_connectionManager);
        if (!m_equalizerCapabilityAgent) {
            ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateEqualizerCapabilityAgent"));
            return false;
        }

        m_equalizerController = equalizerController;
        // Register equalizers
        for (auto& equalizer : m_equalizerRuntimeSetup->getAllEqualizers()) {
            equalizerController->registerEqualizer(equalizer);
        }

        // Add all equalizer controller listeners
        for (auto& listener : m_equalizerRuntimeSetup->getAllEqualizerControllerListeners()) {
            equalizerController->addListener(listener);
        }
    } else {
        ACSDK_DEBUG3(LX(__func__).m("Equalizer is disabled"));
    }

    /*
     * Creating the Endpoint Handler - This component is responsible for handling directives from AVS instructing the
     * client to change the endpoint to connect to.
     */
    auto endpointHandler = capabilityAgents::system::EndpointHandler::create(m_connectionManager, m_exceptionSender);
    if (!endpointHandler) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateEndpointHandler"));
        return false;
    }

    /*
     * Creating the SystemCapabilityProvider - This component is responsible for publishing information about the System
     * capability agent.
     */
    auto systemCapabilityProvider = capabilityAgents::system::SystemCapabilityProvider::create();
    if (!systemCapabilityProvider) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateSystemCapabilityProvider"));
        return false;
    }

#ifdef ENABLE_REVOKE_AUTH
    /*
     * Creating the RevokeAuthorizationHandler - This component is responsible for handling RevokeAuthorization
     * directives from AVS to notify the client to clear out authorization and re-enter the registration flow.
     */
    m_revokeAuthorizationHandler = capabilityAgents::system::RevokeAuthorizationHandler::create(m_exceptionSender);
    if (!m_revokeAuthorizationHandler) {
        ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateRevokeAuthorizationHandler"));
        return false;
    }
#endif

    if (avsCommon::sdkInterfaces::softwareInfo::isValidFirmwareVersion(firmwareVersion)) {
        auto tempSender = capabilityAgents::system::SoftwareInfoSender::create(
            firmwareVersion,
            sendSoftwareInfoOnConnected,
            softwareInfoSenderObserver,
            m_connectionManager,
            m_connectionManager,
            m_exceptionSender);
        if (tempSender) {
            std::lock_guard<std::mutex> lock(m_softwareInfoSenderMutex);
            m_softwareInfoSender = tempSender;
        } else {
            ACSDK_ERROR(LX("initializeFailed").d("reason", "unableToCreateSoftwareInfoSender"));
            return false;
        }
    }

    if (bluetoothDeviceManager) {
        ACSDK_DEBUG5(LX(__func__).m("Creating Bluetooth CA"));

        // Create a temporary pointer to the eventBus inside of bluetoothDeviceManager so that
        // the unique ptr for bluetoothDeviceManager can be moved.
        auto eventBus = bluetoothDeviceManager->getEventBus();

        auto bluetoothAVRCPTransformer =
            capabilityAgents::bluetooth::BluetoothAVRCPTransformer::create(eventBus, m_playbackRouter);

        /*
         * Creating the Bluetooth Capability Agent - This component is responsible for handling directives from AVS
         * regarding bluetooth functionality.
         */
        m_bluetooth = capabilityAgents::bluetooth::Bluetooth::create(
            contextManager,
            m_audioFocusManager,
            m_connectionManager,
            m_exceptionSender,
            std::move(bluetoothStorage),
            std::move(bluetoothDeviceManager),
            std::move(eventBus),
            bluetoothMediaPlayer,
            customerDataManager,
            bluetoothAVRCPTransformer);
    } else {
        ACSDK_DEBUG5(LX("bluetoothCapabilityAgentDisabled").d("reason", "nullBluetoothDeviceManager"));
    }

    /*
     * The following two statements show how to register capability agents to the directive sequencer.
     */
    if (!m_directiveSequencer->addDirectiveHandler(m_speechSynthesizer)) {
        ACSDK_ERROR(LX("initializeFailed")
                        .d("reason", "unableToRegisterDirectiveHandler")
                        .d("directiveHandler", "SpeechSynthesizer"));
        return false;
    }

    if (!m_directiveSequencer->addDirectiveHandler(m_audioPlayer)) {
        ACSDK_ERROR(LX("initializeFailed")
                        .d("reason", "unableToRegisterDirectiveHandler")
                        .d("directiveHandler", "AudioPlayer"));
        return false;
    }

    if (!m_directiveSequencer->addDirectiveHandler(m_externalMediaPlayer)) {
        ACSDK_ERROR(LX("initializeFailed")
                        .d("reason", "unableToRegisterDirectiveHandler")
                        .d("directiveHandler", "ExternalMediaPlayer"));
        return false;
    }

    if (!m_directiveSequencer->addDirectiveHandler(m_audioInputProcessor)) {
        ACSDK_ERROR(LX("initializeFailed")
                        .d("reason", "unableToRegisterDirectiveHandler")
                        .d("directiveHandler", "AudioInputProcessor"));
        return false;
    }

    if (!m_directiveSequencer->addDirectiveHandler(m_alertsCapabilityAgent)) {
        ACSDK_ERROR(LX("initializeFailed")
                        .d("reason", "unableToRegisterDirectiveHandler")
                        .d("directiveHandler", "AlertsCapabilityAgent"));
        return false;
    }

#ifdef ENABLE_PCC
    if (m_phoneCallControllerCapabilityAgent &&
        !m_directiveSequencer->addDirectiveHandler(m_phoneCallControllerCapabilityAgent)) {
        ACSDK_ERROR(LX("initializeFailed")
                        .d("reason", "unableToRegisterDirectiveHandler")
                        .d("directiveHandler", "PhoneCallControllerCapabilityAgent"));
        return false;
    }
#endif

    if (!m_directiveSequencer->addDirectiveHandler(endpointHandler)) {
        ACSDK_ERROR(LX("initializeFailed")
                        .d("reason", "unableToRegisterDirectiveHandler")
                        .d("directiveHandler", "EndpointHandler"));
        return false;
    }

#ifdef ENABLE_REVOKE_AUTH
    if (!m_directiveSequencer->addDirectiveHandler(m_revokeAuthorizationHandler)) {
        ACSDK_ERROR(LX("initializeFailed")
                        .d("reason", "unableToRegisterDirectiveHandler")
                        .d("directiveHandler", "RevokeAuthorizationHandler"));
        return false;
    }
#endif

    if (!m_directiveSequencer->addDirectiveHandler(m_userInactivityMonitor)) {
        ACSDK_ERROR(LX("initializeFailed")
                        .d("reason", "unableToRegisterDirectiveHandler")
                        .d("directiveHandler", "UserInactivityMonitor"));
        return false;
    }

    if (!m_directiveSequencer->addDirectiveHandler(m_speakerManager)) {
        ACSDK_ERROR(LX("initializeFailed")
                        .d("reason", "unableToRegisterDirectiveHandler")
                        .d("directiveHandler", "SpeakerManager"));
        return false;
    }

    if (isGuiSupported) {
        if (!m_directiveSequencer->addDirectiveHandler(m_templateRuntime)) {
            ACSDK_ERROR(LX("initializeFailed")
                            .d("reason", "unableToRegisterDirectiveHandler")
                            .d("directiveHandler", "TemplateRuntime"));
            return false;
        }
    }

    if (!m_directiveSequencer->addDirectiveHandler(m_notificationsCapabilityAgent)) {
        ACSDK_ERROR(LX("initializeFailed")
                        .d("reason", "unableToRegisterDirectiveHandler")
                        .d("directiveHandler", "NotificationsCapabilityAgent"));
        return false;
    }

    if (!m_directiveSequencer->addDirectiveHandler(m_interactionCapabilityAgent)) {
        ACSDK_ERROR(LX("initializeFailed")
                        .d("reason", "unableToRegisterDirectiveHandler")
                        .d("directiveHandler", "InteractionModelCapabilityAgent"));
        return false;
    }

    // The CallManager is an optional component, so it may be nullptr.
    if (m_callManager) {
        if (!m_directiveSequencer->addDirectiveHandler(m_callManager)) {
            ACSDK_ERROR(LX("initializeFailed")
                            .d("reason", "unableToRegisterDirectiveHandler")
                            .d("directiveHandler", "CallManager"));
            return false;
        }
    }

    if (m_bluetooth && !m_directiveSequencer->addDirectiveHandler(m_bluetooth)) {
        ACSDK_ERROR(
            LX("initializeFailed").d("reason", "unableToRegisterDirectiveHandler").d("directiveHandler", "Bluetooth"));
    }

    if (m_mrmCapabilityAgent) {
        if (!m_directiveSequencer->addDirectiveHandler(m_mrmCapabilityAgent)) {
            ACSDK_ERROR(LX("initializeFailed")
                            .d("reason", "unableToRegisterDirectiveHandler")
                            .d("directiveHandler", "MRMCapabilityAgent"));
            return false;
        }
    }

    if (nullptr != m_equalizerCapabilityAgent) {
        if (!m_directiveSequencer->addDirectiveHandler(m_equalizerCapabilityAgent)) {
            ACSDK_ERROR(LX("initializeFailed")
                            .d("reason", "unableToRegisterDirectiveHandler")
                            .d("directiveHandler", "Equalizer"));
            return false;
        }
    }

    if (!m_directiveSequencer->addDirectiveHandler(m_dndCapabilityAgent)) {
        ACSDK_ERROR(
            LX("initializeFailed").d("reason", "unableToRegisterDirectiveHandler").d("directiveHandler", "DND"));
        return false;
    }

    /*
     * Register capabilities for publishing to the Capabilities API.
     */
    if (!(capabilitiesDelegate->registerCapability(m_alertsCapabilityAgent))) {
        ACSDK_ERROR(
            LX("initializeFailed").d("reason", "unableToRegisterCapability").d("capabilitiesDelegate", "Alerts"));
        return false;
    }

    if (!(capabilitiesDelegate->registerCapability(m_audioActivityTracker))) {
        ACSDK_ERROR(LX("initializeFailed")
                        .d("reason", "unableToRegisterCapability")
                        .d("capabilitiesDelegate", "AudioActivityTracker"));
        return false;
    }

    if (!(capabilitiesDelegate->registerCapability(m_audioPlayer))) {
        ACSDK_ERROR(
            LX("initializeFailed").d("reason", "unableToRegisterCapability").d("capabilitiesDelegate", "AudioPlayer"));
        return false;
    }

    if (!(capabilitiesDelegate->registerCapability(m_externalMediaPlayer))) {
        ACSDK_ERROR(LX("initializeFailed")
                        .d("reason", "unableToRegisterCapability")
                        .d("capabilitiesDelegate", "ExternalMediaPlayer"));
        return false;
    }

    if (m_bluetooth && !(capabilitiesDelegate->registerCapability(m_bluetooth))) {
        ACSDK_ERROR(
            LX("initializeFailed").d("reason", "unableToRegisterCapability").d("capabilitiesDelegate", "Bluetooth"));
        return false;
    }

    if (!(capabilitiesDelegate->registerCapability(m_notificationsCapabilityAgent))) {
        ACSDK_ERROR(LX("initializeFailed")
                        .d("reason", "unableToRegisterCapability")
                        .d("capabilitiesDelegate", "Notifications"));
        return false;
    }

    if (!(capabilitiesDelegate->registerCapability(m_playbackController))) {
        ACSDK_ERROR(LX("initializeFailed")
                        .d("reason", "unableToRegisterCapability")
                        .d("capabilitiesDelegate", "PlaybackController"));
        return false;
    }

#ifdef ENABLE_PCC
    // PhoneCallController is an optional component, so it may be nullptr
    if (m_phoneCallControllerCapabilityAgent &&
        !(capabilitiesDelegate->registerCapability(m_phoneCallControllerCapabilityAgent))) {
        ACSDK_ERROR(LX("initializeFailed")
                        .d("reason", "unableToRegisterCapability")
                        .d("capabilitiesDelegate", "PhoneCallController"));
        return false;
    }
#endif

    if (!(capabilitiesDelegate->registerCapability(m_settings))) {
        ACSDK_ERROR(
            LX("initializeFailed").d("reason", "unableToRegisterCapability").d("capabilitiesDelegate", "Settings"));
        return false;
    }

    if (!(capabilitiesDelegate->registerCapability(m_speakerManager))) {
        ACSDK_ERROR(
            LX("initializeFailed").d("reason", "unableToRegisterCapability").d("capabilitiesDelegate", "Speaker"));
        return false;
    }

    if (!(capabilitiesDelegate->registerCapability(m_audioInputProcessor))) {
        ACSDK_ERROR(LX("initializeFailed")
                        .d("reason", "unableToRegisterCapability")
                        .d("capabilitiesDelegate", "SpeechRecognizer"));
        return false;
    }

    if (!(capabilitiesDelegate->registerCapability(m_speechSynthesizer))) {
        ACSDK_ERROR(LX("initializeFailed")
                        .d("reason", "unableToRegisterCapability")
                        .d("capabilitiesDelegate", "SpeechSynthesizer"));
        return false;
    }

    if (!(capabilitiesDelegate->registerCapability(m_interactionCapabilityAgent))) {
        ACSDK_ERROR(
            LX("initializeFailed").d("reason", "unableToRegisterCapability").d("capabilitiesDelegate", "Interaction"));
        return false;
    }

    if (!(capabilitiesDelegate->registerCapability(systemCapabilityProvider))) {
        ACSDK_ERROR(
            LX("initializeFailed").d("reason", "unableToRegisterCapability").d("capabilitiesDelegate", "System"));
        return false;
    }

    if (isGuiSupported) {
        if (!(capabilitiesDelegate->registerCapability(m_templateRuntime))) {
            ACSDK_ERROR(LX("initializeFailed")
                            .d("reason", "unableToRegisterCapability")
                            .d("capabilitiesDelegate", "TemplateRuntime"));
            return false;
        }

        if (!(capabilitiesDelegate->registerCapability(m_visualActivityTracker))) {
            ACSDK_ERROR(LX("initializeFailed")
                            .d("reason", "unableToRegisterCapability")
                            .d("capabilitiesDelegate", "VisualActivityTracker"));
            return false;
        }
    }

    if (m_mrmCapabilityAgent) {
        if (!(capabilitiesDelegate->registerCapability(m_mrmCapabilityAgent))) {
            ACSDK_ERROR(
                LX("initializeFailed").d("reason", "unableToRegisterCapability").d("capabilitiesDelegate", "MRM"));
            return false;
        }
    }

#ifdef ENABLE_COMMS
    auto callManager = capabilityAgents::callManager::CallManager::getInstance();
    if (callManager) {
        if (!(capabilitiesDelegate->registerCapability(callManager))) {
            ACSDK_ERROR(LX("initializeFailed")
                            .d("reason", "unableToRegisterCapability")
                            .d("capabilitiesDelegate", "CallManager"));
            return false;
        }
    }
#endif

    if (nullptr != m_equalizerCapabilityAgent) {
        if (!(capabilitiesDelegate->registerCapability(m_equalizerCapabilityAgent))) {
            ACSDK_ERROR(LX("initializeFailed")
                            .d("reason", "unableToRegisterCapability")
                            .d("capabilitiesDelegate", "Equalizer"));
            return false;
        }
    }

    if (!(capabilitiesDelegate->registerCapability(m_dndCapabilityAgent))) {
        ACSDK_ERROR(
            LX("initializeFailed").d("reason", "unableToRegisterCapability").d("capabilitiesDelegate", "DoNotDisturb"));
        return false;
    }

	/**
	 * Bug #21123 : Start
	 * Description : Device is not indicating the error state, after disconnecting Ethernet
	 * Changed by Smartron India Pvt. Ltd.
	 * */
	m_networkDetectionThread = std::thread(&DefaultClient::checkNetworkStatus, this);
	/**
	 * Bug #21123 : End
	 * */
	/**
    * BUG:21972
    * get default internet status connection from the device.
    */
    std::string StatusString;
    if(!alexaClientSDK::avsCommon::utils::uci::GetparamUCI("shifu.conf", "INTERNET_STATUS", StatusString))
    {
    	m_isNetworkConnected = atoi(StatusString.c_str());
    	ACSDK_DEBUG9(LX("GetparamUCI").d("m_isNetworkConnected", m_isNetworkConnected));
    }
    else
    	ACSDK_CRITICAL(LX("GetparamUCI read error").d("m_isNetworkConnected", m_isNetworkConnected));

    return true;
}

void DefaultClient::onCapabilitiesStateChange(
    CapabilitiesObserverInterface::State newState,
    CapabilitiesObserverInterface::Error newError) {
    if (CapabilitiesObserverInterface::State::SUCCESS == newState) {
        m_connectionManager->enable();
    }
}

void DefaultClient::connect(
    const std::shared_ptr<avsCommon::sdkInterfaces::CapabilitiesDelegateInterface>& capabilitiesDelegate,
    const std::string& avsEndpoint) {
    if (!avsEndpoint.empty()) {
        m_connectionManager->setAVSEndpoint(avsEndpoint);
    }
    capabilitiesDelegate->publishCapabilitiesAsyncWithRetries();
}

void DefaultClient::disconnect() {
    m_connectionManager->disable();
}

void DefaultClient::stopForegroundActivity() {
    m_audioFocusManager->stopForegroundActivity();
}

void DefaultClient::localStopActiveAlert() {
    m_alertsCapabilityAgent->onLocalStop();
}

/* #21219
* get the present active channel priority
*/
unsigned int DefaultClient::getActiveChannelPriority() {
	auto HPActiveChannel = m_audioFocusManager->getHighestPriorityActiveChannel();
	
	if (HPActiveChannel != nullptr) 
    	return HPActiveChannel->getPriority();
	return 0;
}

void DefaultClient::addAlexaDialogStateObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::DialogUXStateObserverInterface> observer) {
    m_dialogUXStateAggregator->addObserver(observer);
}

void DefaultClient::removeAlexaDialogStateObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::DialogUXStateObserverInterface> observer) {
    m_dialogUXStateAggregator->removeObserver(observer);
}

void DefaultClient::addMessageObserver(std::shared_ptr<avsCommon::sdkInterfaces::MessageObserverInterface> observer) {
    m_connectionManager->addMessageObserver(observer);
}

void DefaultClient::removeMessageObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::MessageObserverInterface> observer) {
    m_connectionManager->removeMessageObserver(observer);
}

void DefaultClient::addConnectionObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::ConnectionStatusObserverInterface> observer) {
    m_connectionManager->addConnectionStatusObserver(observer);
}

void DefaultClient::removeConnectionObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::ConnectionStatusObserverInterface> observer) {
    m_connectionManager->removeConnectionStatusObserver(observer);
}

void DefaultClient::addInternetConnectionObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::InternetConnectionObserverInterface> observer) {
    m_internetConnectionMonitor->addInternetConnectionObserver(observer);
}

void DefaultClient::removeInternetConnectionObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::InternetConnectionObserverInterface> observer) {
    m_internetConnectionMonitor->removeInternetConnectionObserver(observer);
}

void DefaultClient::addAlertsObserver(std::shared_ptr<capabilityAgents::alerts::AlertObserverInterface> observer) {
    m_alertsCapabilityAgent->addObserver(observer);
}

void DefaultClient::removeAlertsObserver(std::shared_ptr<capabilityAgents::alerts::AlertObserverInterface> observer) {
    m_alertsCapabilityAgent->removeObserver(observer);
}

void DefaultClient::addAudioPlayerObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::AudioPlayerObserverInterface> observer) {
    m_audioPlayer->addObserver(observer);
}

void DefaultClient::removeAudioPlayerObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::AudioPlayerObserverInterface> observer) {
    m_audioPlayer->removeObserver(observer);
}

void DefaultClient::addTemplateRuntimeObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::TemplateRuntimeObserverInterface> observer) {
    if (!m_templateRuntime) {
        ACSDK_ERROR(LX("addTemplateRuntimeObserverFailed").d("reason", "guiNotSupported"));
        return;
    }
    m_templateRuntime->addObserver(observer);
}

void DefaultClient::removeTemplateRuntimeObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::TemplateRuntimeObserverInterface> observer) {
    if (!m_templateRuntime) {
        ACSDK_ERROR(LX("removeTemplateRuntimeObserverFailed").d("reason", "guiNotSupported"));
        return;
    }
    m_templateRuntime->removeObserver(observer);
}

void DefaultClient::TemplateRuntimeDisplayCardCleared() {
    if (!m_templateRuntime) {
        ACSDK_ERROR(LX("TemplateRuntimeDisplayCardClearedFailed").d("reason", "guiNotSupported"));
        return;
    }
    m_templateRuntime->displayCardCleared();
}

void DefaultClient::addSettingObserver(
    const std::string& key,
    std::shared_ptr<avsCommon::sdkInterfaces::SingleSettingObserverInterface> observer) {
    m_settings->addSingleSettingObserver(key, observer);
}

void DefaultClient::removeSettingObserver(
    const std::string& key,
    std::shared_ptr<avsCommon::sdkInterfaces::SingleSettingObserverInterface> observer) {
    m_settings->removeSingleSettingObserver(key, observer);
}

void DefaultClient::addNotificationsObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::NotificationsObserverInterface> observer) {
    m_notificationsCapabilityAgent->addObserver(observer);
}

void DefaultClient::removeNotificationsObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::NotificationsObserverInterface> observer) {
    m_notificationsCapabilityAgent->removeObserver(observer);
}

void DefaultClient::addExternalMediaPlayerObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::externalMediaPlayer::ExternalMediaPlayerObserverInterface> observer) {
    m_externalMediaPlayer->addObserver(observer);
}

void DefaultClient::removeExternalMediaPlayerObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::externalMediaPlayer::ExternalMediaPlayerObserverInterface> observer) {
    m_externalMediaPlayer->removeObserver(observer);
}

void DefaultClient::addBluetoothDeviceObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::bluetooth::BluetoothDeviceObserverInterface> observer) {
    if (!m_bluetooth) {
        ACSDK_DEBUG5(LX(__func__).m("bluetooth is disabled, not adding observer"));
        return;
    }
    m_bluetooth->addObserver(observer);
}

void DefaultClient::removeBluetoothDeviceObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::bluetooth::BluetoothDeviceObserverInterface> observer) {
    if (!m_bluetooth) {
        return;
    }
    m_bluetooth->removeObserver(observer);
}

#ifdef ENABLE_REVOKE_AUTH
void DefaultClient::addRevokeAuthorizationObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::RevokeAuthorizationObserverInterface> observer) {
    if (!m_revokeAuthorizationHandler) {
        ACSDK_ERROR(LX("addRevokeAuthorizationObserver").d("reason", "revokeAuthorizationNotSupported"));
        return;
    }
    m_revokeAuthorizationHandler->addObserver(observer);
}

void DefaultClient::removeRevokeAuthorizationObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::RevokeAuthorizationObserverInterface> observer) {
    if (!m_revokeAuthorizationHandler) {
        ACSDK_ERROR(LX("removeRevokeAuthorizationObserver").d("reason", "revokeAuthorizationNotSupported"));
        return;
    }
    m_revokeAuthorizationHandler->removeObserver(observer);
}
#endif

void DefaultClient::changeSetting(const std::string& key, const std::string& value) {
    m_settings->changeSetting(key, value);
}

void DefaultClient::sendDefaultSettings() {
    m_settings->sendDefaultSettings();
}

std::shared_ptr<settings::DeviceSettingsManager> DefaultClient::getSettingsManager() {
    return m_deviceSettingsManager;
}

std::shared_ptr<avsCommon::sdkInterfaces::PlaybackRouterInterface> DefaultClient::getPlaybackRouter() const {
    return m_playbackRouter;
}

std::shared_ptr<registrationManager::RegistrationManager> DefaultClient::getRegistrationManager() {
    return m_registrationManager;
}

std::shared_ptr<equalizer::EqualizerController> DefaultClient::getEqualizerController() {
    return m_equalizerController;
}

void DefaultClient::addSpeakerManagerObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::SpeakerManagerObserverInterface> observer) {
    m_speakerManager->addSpeakerManagerObserver(observer);
}

void DefaultClient::removeSpeakerManagerObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::SpeakerManagerObserverInterface> observer) {
    m_speakerManager->removeSpeakerManagerObserver(observer);
}

std::shared_ptr<avsCommon::sdkInterfaces::SpeakerManagerInterface> DefaultClient::getSpeakerManager() {
    return m_speakerManager;
}

bool DefaultClient::setFirmwareVersion(avsCommon::sdkInterfaces::softwareInfo::FirmwareVersion firmwareVersion) {
    {
        std::lock_guard<std::mutex> lock(m_softwareInfoSenderMutex);

        if (!m_softwareInfoSender) {
            m_softwareInfoSender = capabilityAgents::system::SoftwareInfoSender::create(
                firmwareVersion, true, nullptr, m_connectionManager, m_connectionManager, m_exceptionSender);
            if (m_softwareInfoSender) {
                return true;
            }

            ACSDK_ERROR(LX("setFirmwareVersionFailed").d("reason", "unableToCreateSoftwareInfoSender"));
            return false;
        }
    }

    return m_softwareInfoSender->setFirmwareVersion(firmwareVersion);
}

std::future<bool> DefaultClient::notifyOfWakeWord(
    capabilityAgents::aip::AudioProvider wakeWordAudioProvider,
    avsCommon::avs::AudioInputStream::Index beginIndex,
    avsCommon::avs::AudioInputStream::Index endIndex,
    std::string keyword,
    std::chrono::steady_clock::time_point startOfSpeechTimestamp,
    const capabilityAgents::aip::ESPData espData,
    std::shared_ptr<const std::vector<char>> KWDMetadata) {

	/**
	 * Bug #21123 : Start
	 * Description : Device is not indicating the error state, after disconnecting Ethernet
	 * Changed by Smartron India Pvt. Ltd.
	 * */

	/* SMARTRON: Check connection status of network and AVS before starting AIP operation */
	std::promise<bool> ret;

	// #22267 Start: Synchronization between LED server & AVS after Booting of Shifu
    if (isAVSinInitFirstTime == false) {
        ACSDK_DEBUG9(LX("notifyOfWakeWord").d("action", "skipWakeword").d("reason", "Initially, AVS is not in ideal"));
        ret.set_value(true);
        return ret.get_future();
    }
    // #22267 End.

	// Redmine 21700: start: After Deregistration,if user press action button, no internet announcement observed
	int isNetworkConnected = isNetConnected();
	// Redmine 21700: End.
	bool isAVSConnected = m_connectionManager->isConnected();
	// Redmine 21700: start: After Deregistration,if user press action button, no internet announcement observed
	if(isNetworkConnected == CONNECTED){
	// Redmine 21700: End.
		// Redmine 21542: start: After completion of Registration,First wakeword is not detecting playing announcement as "device is not registered"
		if(isDeviceRegisteredWithAVS){
			if(isAVSConnected){
				return m_audioInputProcessor->recognize(
					wakeWordAudioProvider,
					capabilityAgents::aip::Initiator::WAKEWORD,
					startOfSpeechTimestamp,
					beginIndex,
					endIndex,
					keyword,
					espData,
					KWDMetadata);
			}else{
				ACSDK_ERROR(LX("FailedToStarted").d("reason", "Disconnected").d("isAVSConnected", isAVSConnected).d("isDeviceRegisteredWithAVS", isDeviceRegisteredWithAVS).d("Network", isNetworkConnected));
				ret.set_value(false);
				// Redmine 21676: start: Need announcement support from LED module when AVS is not connected
				alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_ANNOUNCEMENT, 1, LED_AVS_FAIL);
				// Redmine 21676: End.
				return ret.get_future();
			}
		}else{
			ACSDK_ERROR(LX("FailedToStarted").d("reason", "Disconnected").d("isAVSConnected", isAVSConnected).d("isDeviceRegisteredWithAVS", isDeviceRegisteredWithAVS).d("Network", isNetworkConnected));
			ret.set_value(false);
			alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_ANNOUNCEMENT, 1, LED_AUTH_FAIL);
			return ret.get_future();
		}
	// Redmine 21700: start: After Deregistration,if user press action button, no internet announcement observed
	}else if(isNetworkConnected == DISCONNECTED){
	// Redmine 21700: End.
		ACSDK_ERROR(LX("FailedToStarted").d("reason", "Disconnected").d("isAVSConnected", isAVSConnected).d("isDeviceRegisteredWithAVS", isDeviceRegisteredWithAVS).d("Network", isNetworkConnected));
		// Redmine 21542: End.
		ret.set_value(false);
		alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_ANNOUNCEMENT, 1, LED_NO_INTERNET);
		return ret.get_future();
	}
	// Redmine 21700: start: After Deregistration,if user press action button, no internet announcement observed
	else{
		ret.set_value(false);
		return ret.get_future();
	}
	// Redmine 21700: End.
	/**
	 * Bug #21123 : End
	 * */
}

std::future<bool> DefaultClient::notifyOfTapToTalk(
    capabilityAgents::aip::AudioProvider tapToTalkAudioProvider,
    avsCommon::avs::AudioInputStream::Index beginIndex,
    std::chrono::steady_clock::time_point startOfSpeechTimestamp) {
    return m_audioInputProcessor->recognize(
        tapToTalkAudioProvider, capabilityAgents::aip::Initiator::TAP, startOfSpeechTimestamp, beginIndex);
}

std::future<bool> DefaultClient::notifyOfHoldToTalkStart(
    capabilityAgents::aip::AudioProvider holdToTalkAudioProvider,
    std::chrono::steady_clock::time_point startOfSpeechTimestamp) {
    return m_audioInputProcessor->recognize(
        holdToTalkAudioProvider, capabilityAgents::aip::Initiator::PRESS_AND_HOLD, startOfSpeechTimestamp);
}

std::future<bool> DefaultClient::notifyOfHoldToTalkEnd() {
    return m_audioInputProcessor->stopCapture();
}

std::future<bool> DefaultClient::notifyOfTapToTalkEnd() {
    return m_audioInputProcessor->stopCapture();
}

void DefaultClient::addCallStateObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::CallStateObserverInterface> observer) {
    if (m_callManager) {
        m_callManager->addObserver(observer);
    }
}

void DefaultClient::removeCallStateObserver(
    std::shared_ptr<avsCommon::sdkInterfaces::CallStateObserverInterface> observer) {
    if (m_callManager) {
        m_callManager->removeObserver(observer);
    }
}

/**
 * Feature #21131 : Start
 * Description : [ALEXA][AVS]Function button support - wakeword detection and stop current activity
 * Changed by Smartron India Pvt. Ltd.
 * */
std::future<bool> DefaultClient::notifyOfActionButton(
    capabilityAgents::aip::AudioProvider AudioProvider,
    bool isMicOn,
    avsCommon::avs::AudioInputStream::Index beginIndex) {
   /*
    * SMARTRON: Base on amazon UX regulation, supported that cancel alert / recognize event
    *      through action button no matter it's triggered by WakeWord or tapToTalk.
    */
    ACSDK_DEBUG9(LX("onActionButtonCalled"));

    std::promise<bool> ret;
	// #22267 Start: Synchronization between LED server & AVS after Booting of Shifu
    if (isAVSinInitFirstTime == false) {
        ACSDK_DEBUG9(LX("onActionButtonCalled").d("action", "skipTap").d("reason", "Initially, AVS is not in ideal"));
        ret.set_value(true);
        return ret.get_future();
    }
    // #22267 End.
    auto HPActiveChannel = m_audioFocusManager->getHighestPriorityActiveChannel();
    if (HPActiveChannel != nullptr) {
        auto activeChannelPriority = HPActiveChannel->getPriority();
        ACSDK_DEBUG9(LX("onActionButtonCalled").d("activeChannelPriority", activeChannelPriority));
        switch(activeChannelPriority) {
            case 100: //Dialog
                if (m_speechSynthesizer->isSpeakingActive())
                    break;
				/**
				 * Feature #21440 : Start
				 * Description : [Performance-Bench-marking]:[AUDIO]:[SampleApp]: While tapping Action button for couple of times Sample is Crashed
				 * Changes: If AVS is in listening state then stop capturing voice for action button and return.
				 * Changed by Smartron India Pvt. Ltd.
				 * */
				ACSDK_DEBUG9(LX("onActionButtonCalled").d("action", "stopCapture"));
				m_audioInputProcessor->stopCapture();
				ret.set_value(true);
				return ret.get_future();
				/**
				 * Feature #21440 : End
				 * */
            case 200: //Alerts
                ACSDK_DEBUG9(LX("onActionButtonCalled").d("action", "stopForegroundActivity"));
                stopForegroundActivity();
                ret.set_value(true);
                if(Dnd_state == 1)
                {
                    ACSDK_DEBUG9(LX("DND led triggered"));
                    alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_DND, 1, 0);

                }
                return ret.get_future();
            case 300: //Content
            default:
                break;
        }
    }
    if (!isMicOn) {
        ACSDK_DEBUG9(LX("onActionButtonCalled").d("action", "skipTap").d("reason", "Mic is off now"));
        ret.set_value(true);
        return ret.get_future();
    }
	/**
	 * Bug #21123 : Start
	 * Description : Device is not indicating the error state, after disconnecting Ethernet
	 * Changed by Smartron India Pvt. Ltd.
	 * */
	// Redmine 21700: start: After Deregistration,if user press action button, no internet announcement observed
	int isNetworkConnected = isNetConnected();
	// Redmine 21700: End.
	bool isAVSConnected = m_connectionManager->isConnected();
	// Redmine 21700: start: After Deregistration,if user press action button, no internet announcement observed
	if(isNetworkConnected == CONNECTED){
	// Redmine 21700: end.
		// Redmine 21542: start: After completion of Registration,First wakeword is not detecting playing announcement as "device is not registered"
		if(isDeviceRegisteredWithAVS){
			if(isAVSConnected){
				ACSDK_DEBUG9(LX("onActionButtonCalled").d("action", "tapToTalk"));
				return notifyOfTapToTalk(AudioProvider, beginIndex);
			}else{
				ACSDK_ERROR(LX("FailedToStarted").d("reason", "Disconnected").d("isAVSConnected", isAVSConnected).d("isDeviceRegisteredWithAVS", isDeviceRegisteredWithAVS).d("Network", isNetworkConnected));
				ret.set_value(false);
				// Redmine 21676: start: Need announcement support from LED module when AVS is not connected
				alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_ANNOUNCEMENT, 1, LED_AVS_FAIL);
				// Redmine 21676: End.
				return ret.get_future();
			}
		}else{
			ACSDK_ERROR(LX("FailedToStarted").d("reason", "Disconnected").d("isAVSConnected", isAVSConnected).d("isDeviceRegisteredWithAVS", isDeviceRegisteredWithAVS).d("Network", isNetworkConnected));
			ret.set_value(false);
			alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_ANNOUNCEMENT, 1, LED_AUTH_FAIL);
			return ret.get_future();
		}
	// Redmine 21700: start: After Deregistration,if user press action button, no internet announcement observed
	}else if(isNetworkConnected == DISCONNECTED){
	// Redmine 21700: End.
		ACSDK_ERROR(LX("FailedToStarted").d("reason", "Disconnected").d("isAVSConnected", isAVSConnected).d("isDeviceRegisteredWithAVS", isDeviceRegisteredWithAVS).d("Network", isNetworkConnected));
		// Redmine 21542: End.
		ret.set_value(false);
		alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_ANNOUNCEMENT, 1, LED_NO_INTERNET);
		return ret.get_future();
	}
	// Redmine 21700: start: After Deregistration,if user press action button, no internet announcement observed
	else{
		ret.set_value(false);
		return ret.get_future();
	}
	// Redmine 21700: End.
	/**
	 * Feature #21123 : End
	 * */
}
/**
 * Feature #21131 : End
 * */

bool DefaultClient::isCommsEnabled() {
    return (m_callManager != nullptr);
}

void DefaultClient::acceptCommsCall() {
    if (m_callManager) {
        m_callManager->acceptCall();
    }
}

void DefaultClient::stopCommsCall() {
    if (m_callManager) {
        m_callManager->stopCall();
    }
}

DefaultClient::~DefaultClient() {
    if (m_directiveSequencer) {
        ACSDK_DEBUG5(LX("DirectiveSequencerShutdown"));
        m_directiveSequencer->shutdown();
    }
    if (m_speakerManager) {
        ACSDK_DEBUG5(LX("SpeakerManagerShutdown"));
        m_speakerManager->shutdown();
    }
    if (m_templateRuntime) {
        ACSDK_DEBUG5(LX("TemplateRuntimeShutdown"));
        m_templateRuntime->shutdown();
    }
    if (m_audioInputProcessor) {
        ACSDK_DEBUG5(LX("AIPShutdown"));
        m_audioInputProcessor->shutdown();
    }
    if (m_audioPlayer) {
        ACSDK_DEBUG5(LX("AudioPlayerShutdown"));
        m_audioPlayer->shutdown();
    }
    if (m_externalMediaPlayer) {
        ACSDK_DEBUG5(LX("ExternalMediaPlayerShutdown"));
        m_externalMediaPlayer->shutdown();
    }
    if (m_speechSynthesizer) {
        ACSDK_DEBUG5(LX("SpeechSynthesizerShutdown"));
        m_speechSynthesizer->shutdown();
    }
    if (m_alertsCapabilityAgent) {
        ACSDK_DEBUG5(LX("AlertsShutdown"));
        m_alertsCapabilityAgent->shutdown();
    }
    if (m_playbackController) {
        ACSDK_DEBUG5(LX("PlaybackControllerShutdown"));
        m_playbackController->shutdown();
    }
    if (m_softwareInfoSender) {
        ACSDK_DEBUG5(LX("SoftwareInfoShutdown"));
        m_softwareInfoSender->shutdown();
    }
    if (m_messageRouter) {
        ACSDK_DEBUG5(LX("MessageRouterShutdown."));
        m_messageRouter->shutdown();
    }
    if (m_connectionManager) {
        ACSDK_DEBUG5(LX("ConnectionManagerShutdown."));
        m_connectionManager->shutdown();
    }
    if (m_certifiedSender) {
        ACSDK_DEBUG5(LX("CertifiedSenderShutdown."));
        m_certifiedSender->shutdown();
    }
    if (m_audioActivityTracker) {
        ACSDK_DEBUG5(LX("AudioActivityTrackerShutdown."));
        m_audioActivityTracker->shutdown();
    }
    if (m_visualActivityTracker) {
        ACSDK_DEBUG5(LX("VisualActivityTrackerShutdown."));
        m_visualActivityTracker->shutdown();
    }
    if (m_playbackRouter) {
        ACSDK_DEBUG5(LX("PlaybackRouterShutdown."));
        m_playbackRouter->shutdown();
    }
    if (m_notificationsCapabilityAgent) {
        ACSDK_DEBUG5(LX("NotificationsShutdown."));
        m_notificationsCapabilityAgent->shutdown();
    }
    if (m_bluetooth) {
        ACSDK_DEBUG5(LX("BluetoothShutdown."));
        m_bluetooth->shutdown();
    }
    if (m_userInactivityMonitor) {
        ACSDK_DEBUG5(LX("UserInactivityMonitorShutdown."));
        m_userInactivityMonitor->shutdown();
    }
    if (m_callManager) {
        ACSDK_DEBUG5(LX("CallManagerShutdown."));
        m_callManager->shutdown();
    }
    if (m_mrmCapabilityAgent) {
        ACSDK_DEBUG5(LX("MRMCapabilityAgentShutdown"));
        m_mrmCapabilityAgent->shutdown();
    }
#ifdef ENABLE_PCC
    if (m_phoneCallControllerCapabilityAgent) {
        ACSDK_DEBUG5(LX("PhoneCallControllerCapabilityAgentShutdown"));
        m_phoneCallControllerCapabilityAgent->shutdown();
    }
#endif
    if (m_dndCapabilityAgent) {
        ACSDK_DEBUG5(LX("DNDCapabilityAgentShutdown"));
        removeConnectionObserver(m_dndCapabilityAgent);
        m_dndCapabilityAgent->shutdown();
    }
    if (nullptr != m_equalizerCapabilityAgent) {
        for (auto& equalizer : m_equalizerRuntimeSetup->getAllEqualizers()) {
            m_equalizerController->unregisterEqualizer(equalizer);
        }
        for (auto& listener : m_equalizerRuntimeSetup->getAllEqualizerControllerListeners()) {
            m_equalizerController->removeListener(listener);
        }
        ACSDK_DEBUG5(LX("EqualizerCapabilityAgentShutdown"));
        m_equalizerCapabilityAgent->shutdown();
    }

    if (m_deviceSettingStorage) {
        ACSDK_DEBUG5(LX("CloseSettingStorage"));
        m_deviceSettingStorage->close();
    }
}

/**
 * Bug #21123 : Start
 * Description : Device is not indicating the error state, after disconnecting Ethernet
 * Changed by Smartron India Pvt. Ltd.
 * */
/** Redmine 21700: start: After Deregistration,if user press action button, no internet announcement observed **/
int DefaultClient::isNetConnected(){
/** Redmine 21700: end.**/
	return m_isNetworkConnected;
}
	/**
    * BUG:21972
    * Below are the signal handlers
    * when internet is there  we will get SIGUSR1 signal from the External module(internet_check script).
    * when internet is not there  we will get SIGUSR2 signal from the External module(internet_check script).
    */



void  DefaultClient::network_connected(int signal_value){
	m_isNetworkConnected = CONNECTED;
}

void DefaultClient::network_disconnected(int signal_value){
	m_isNetworkConnected = DISCONNECTED;
}

	/**
    * BUG:21972
    *  connection status variables to alter the stop and start activity status of the device as when internet is connected and disconnected time.
    * 1. When Music is running: -Remove Internet: Music will be paused. -Connect Internet again: Next song will be played.
    * 2. When query voice response is running: -Remove Internet: Response will be stopped, Back to idle.. -Connect Internet again: No response(Alexa will be in idle state only).
    * 3. When query utterance is in progress: -Remove Internet: Query will be ignored, Back to idle.. -Connect Internet again: No response(Alexa will be in idle state only).
    * 4. When query is in process @ AVS: -Remove Internet: Back to idle. -Connect Internet again: Not sure, If AVS push the directive then It may play the response(SDK does not have any control).
    * 5. When Music is running & Query was asked and after that voice respons is playing: -Remove Internet: Voice response & Music both will be stopped. -Connect Internet again: No response. user need to ask again for music(Alexa will be in idle state only)..
    * 6. When Music is running & query utterance is in progress: -Remove Internet: Query will be ignored & Music will be stopped. -Connect Internet again: No response. user need to ask again for music(Alexa will be in idle state only)..
    * 7. When Music is running & query is in process @ AVS: -Remove Internet: Music will be stopped. -Connect Internet again: Not sure, If AVS push the directive then It may play the response(SDK does not have any control). user need to ask again for music.
    */
void DefaultClient::checkNetworkStatus(){

	while(1)
	{
		std::this_thread::sleep_for(DEFAULT_CLIENT_CHECK_NETWORK_INTERVAL);
		
		if(m_present_AVSNetworkConnected !=m_isNetworkConnected)
		{
		
			ACSDK_DEBUG9(LX("checkNetworkStatus").d("m_isNetworkConnected", m_isNetworkConnected));
			ACSDK_DEBUG9(LX("checkNetworkStatus").d("m_present_AVSNetworkConnected", m_present_AVSNetworkConnected));			
			m_present_AVSNetworkConnected = m_isNetworkConnected;
			if(m_present_AVSNetworkConnected == CONNECTED)
			{
				if(m_isMediaPaused)
				{
					m_audioPlayer->playMedia();
					m_isMediaPaused = false;
				}
			}
			else if(m_present_AVSNetworkConnected == DISCONNECTED)
			{
				// #22267 Start: Synchronization between LED server & AVS after Booting of Shifu
				avs_config.lock();
				if(isAVSinInitFirstTime == false){
					alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_AVS_CONFIG, 1, 0);
					ACSDK_DEBUG9(LX("checkNetworkStatus").d("info", "Sending AVS configured to LED module"));
					isAVSinInitFirstTime = true;
				}
				avs_config.unlock();
				// #22267 End.
				ACSDK_DEBUG9(LX("checkNetworkStatus").d("info", "Making sure that Internet does not exist"));
				auto HPActiveChannel = m_audioFocusManager->getHighestPriorityActiveChannel();
				if (HPActiveChannel != nullptr) {
					auto activeChannelPriority = HPActiveChannel->getPriority();
					if(activeChannelPriority == 100) {	//Dialog
						// Redmine 21874: start:During no internet if we press mic mute button,wakeword pattern triggered
						// End Session with AVS if Active
						m_audioInputProcessor->stopCapture();
						// Redmine 21874: End.

						if (m_speechSynthesizer->isSpeakingActive()){
							m_speechSynthesizer->stopSpeaking();
						}
						m_audioPlayer->stopMedia();
						// Redmine 22059: start: Requesting LED module to play announcement when device lost internet
						alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_ANNOUNCEMENT, 1, LED_INTERNET_LOST);
						// Redmine 22059: End.
					}
				}

				if((m_audioPlayer->isMediaPlayerActive()) && (!m_isMediaPaused)){
					m_audioPlayer->pauseMedia();
					m_isMediaPaused = true;
					// Redmine 22059: start: Requesting LED module to play announcement when device lost internet
					alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_ANNOUNCEMENT, 1, LED_INTERNET_LOST);
					// Redmine 22059: End.
				}
			}
		}
	}
}


}  // namespace defaultClient
}  // namespace alexaClientSDK
