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

#include <cctype>

#include <sys/syslog.h>
#include <unistd.h>

#include "AVSCommon/Utils/UCIInterface.h"
#include <AVSCommon/SDKInterfaces/SpeakerInterface.h>
#include <AVSCommon/Utils/Logger/Logger.h>
#include <AVSCommon/Utils/String/StringUtils.h>
#include "SampleApp/UserInputManager.h"
#include "SampleApp/ConsolePrinter.h"
#include <AVSCommon/SDKInterfaces/FocusManagerInterface.h>

#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

// #22267 Start: Synchronization between LED server & AVS after Booting of Shifu
extern bool isAVSinInitFirstTime;
// #22267 End.

namespace alexaClientSDK {
namespace sampleApp {

using namespace avsCommon::sdkInterfaces;
using namespace avsCommon::sdkInterfaces::softwareInfo;

static const char HOLD = 'h';
static const char TAP = 't';
static const char QUIT = 'q';
static const char INFO = 'i';
static const char MIC_TOGGLE = 'm';
static const char STOP = 's';
static const char PLAY = '1';
static const char PAUSE = '2';
static const char NEXT = '3';
static const char PREVIOUS = '4';
static const char SKIP_FORWARD = '5';
static const char SKIP_BACKWARD = '6';
static const char SHUFFLE = '7';
static const char LOOP = '8';
static const char REPEAT = '9';
static const char THUMBS_UP = '+';
static const char THUMBS_DOWN = '-';
static const char SETTINGS = 'c';
static const char SPEAKER_CONTROL = 'p';
static const char FIRMWARE_VERSION = 'f';
static const char ESP_CONTROL = 'e';
static const char RESET = 'k';
static const char REAUTHORIZE = 'z';
#ifdef ENABLE_COMMS
static const char COMMS_CONTROL = 'd';
#endif

//////////message_queue structure////

struct msgbuf
{
    long    mtype;
    int     selectoption;
};
#ifdef ENABLE_PCC
static const char PHONE_CONTROL = 'a';
#endif
static constexpr char ENABLE = 'E';
static constexpr char DISABLE = 'D';

enum class SettingsValues : char { LOCALE = '1', DO_NOT_DISTURB = '2' };

static const std::unordered_map<char, std::string> LOCALE_VALUES({{'1', "en-US"},
                                                                  {'2', "en-GB"},
                                                                  {'3', "de-DE"},
                                                                  {'4', "en-IN"},
                                                                  {'5', "en-CA"},
                                                                  {'6', "ja-JP"},
                                                                  {'7', "en-AU"},
                                                                  {'8', "fr-FR"},
                                                                  {'9', "it-IT"},
                                                                  {'a', "es-ES"},
                                                                  {'b', "es-MX"},
                                                                  {'c', "fr-CA"}});

static const std::unordered_map<char, SpeakerInterface::Type> SPEAKER_TYPES(
    {{'1', SpeakerInterface::Type::AVS_SPEAKER_VOLUME}, {'2', SpeakerInterface::Type::AVS_ALERTS_VOLUME}});

static const int8_t INCREASE_VOLUME = 10;

static const int8_t DECREASE_VOLUME = -10;

/// Time to wait for console input
static const auto READ_CONSOLE_TIMEOUT = std::chrono::milliseconds(100);

/// String to identify log entries originating from this file.
static const std::string TAG("UserInputManager");

/**
 * Create a LogEntry using this file's TAG and the specified event string.
 *
 * @param The event string for this @c LogEntry.
 */
#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

std::unique_ptr<UserInputManager> UserInputManager::create(
    std::shared_ptr<InteractionManager> interactionManager,
    std::shared_ptr<ConsoleReader> consoleReader) {
    if (!interactionManager) {
        ACSDK_CRITICAL(LX("Invalid InteractionManager passed to UserInputManager"));
        return nullptr;
    }

    if (!consoleReader) {
        ACSDK_CRITICAL(LX("Invalid ConsoleReader passed to UserInputManager"));
        return nullptr;
    }

    return std::unique_ptr<UserInputManager>(new UserInputManager(interactionManager, consoleReader));
}

UserInputManager::UserInputManager(
    std::shared_ptr<InteractionManager> interactionManager,
    std::shared_ptr<ConsoleReader> consoleReader) :
        m_interactionManager{interactionManager},
        m_consoleReader{consoleReader},
        m_limitedInteraction{false},
        m_restart{false} {
        
    /* #21936: Reading the mic mute state from uic config on bootup
    */
    std::string micmute;
    if(!alexaClientSDK::avsCommon::utils::uci::GetparamUCI("alsa.alsa", "mic_mute", micmute))
    {
		if(atoi(micmute.c_str()) == 0)
			m_interactionManager->microphoneOn();
		else
			m_interactionManager->microphoneOff();
    }
}

bool UserInputManager::readConsoleInput(char* input) {
    while (input && !m_restart) {
        if (m_consoleReader->read(READ_CONSOLE_TIMEOUT, input)) {
            return true;
        }
    }
    return false;
}

SampleAppReturnCode UserInputManager::run(bool ui) {
    bool userTriggeredLogout = false;
	unsigned int ActiveChannelPriority;
	    
    m_interactionManager->begin();
    syslog(LOG_DEBUG, "ALEXA: Disabled Input option");
    
    
    //messsage queue inits///
    int msqid;
    key_t key;
    struct msgbuf rcvbuffer;

    key = 1234;

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
    {
      syslog(LOG_ERR, "ALEXA: msgget() key generation error");
      exit(0);
    }
    
    while(!(msgrcv(msqid, &rcvbuffer, sizeof(struct msgbuf), 0, IPC_NOWAIT) < 0))
		ACSDK_CRITICAL(LX("Clearing Old meaasges present in Message Queue"));
    
    if(ui)
    {
		while (true) {
			char x;
			if (!readConsoleInput(&x)) {
				break;
			}
			x = ::tolower(x);
			if (x == QUIT) {
				break;
			} else if (x == RESET) {
				if (confirmReset()) {
					userTriggeredLogout = true;
				}
			} else if (x == REAUTHORIZE) {
				confirmReauthorizeDevice();
			} else if (x == MIC_TOGGLE) {
				m_interactionManager->microphoneToggle();
			} else if (x == STOP) {
				m_interactionManager->stopForegroundActivity();
			} else if (x == SPEAKER_CONTROL) {
				controlSpeaker();
			} else if (x == INFO) {
				if (m_limitedInteraction) {
					m_interactionManager->limitedHelp();
				} else {
					m_interactionManager->help();
				}
			} else if (m_limitedInteraction) {
				m_interactionManager->errorValue();
				// ----- Add a new interaction bellow if the action is available only in 'unlimited interaction mode'.
			} else if (x == HOLD) {
				m_interactionManager->holdToggled();
			} else if (x == TAP) {
				m_interactionManager->tap();
			} else if (x == PLAY) {
				m_interactionManager->playbackPlay();
			} else if (x == PAUSE) {
				m_interactionManager->playbackPause();
			} else if (x == NEXT) {
				m_interactionManager->playbackNext();
			} else if (x == PREVIOUS) {
				m_interactionManager->playbackPrevious();
			} else if (x == SKIP_FORWARD) {
				m_interactionManager->playbackSkipForward();
			} else if (x == SKIP_BACKWARD) {
				m_interactionManager->playbackSkipBackward();
			} else if (x == SHUFFLE) {
				m_interactionManager->playbackShuffle();
			} else if (x == LOOP) {
				m_interactionManager->playbackLoop();
			} else if (x == REPEAT) {
				m_interactionManager->playbackRepeat();
			} else if (x == THUMBS_UP) {
				m_interactionManager->playbackThumbsUp();
			} else if (x == THUMBS_DOWN) {
				m_interactionManager->playbackThumbsDown();
			} else if (x == SETTINGS) {
				m_interactionManager->settings();
				char y;
				std::cin >> y;
				// Check the Setting which has to be changed.
				switch (y) {
					case (char)SettingsValues::LOCALE: {
						char localeValue;
						m_interactionManager->locale();
						std::cin >> localeValue;
						auto searchLocale = LOCALE_VALUES.find(localeValue);
						if (searchLocale != LOCALE_VALUES.end()) {
							m_interactionManager->changeSetting("locale", searchLocale->second);
						} else {
							m_interactionManager->errorValue();
						}
						break;
					}
						m_interactionManager->help();
				}
			} else if (x == FIRMWARE_VERSION) {
				std::string text;
				std::getline(std::cin, text);
				// If text is empty the user entered newline right after the command key.
				// Prompt for the version number and get the version from the next line.
				if (text.empty()) {
					m_interactionManager->firmwareVersionControl();
					std::getline(std::cin, text);
				}
				int version;
				const long long int maxValue = MAX_FIRMWARE_VERSION;
				if (avsCommon::utils::string::stringToInt(text, &version) && version > 0 && version <= maxValue) {
					m_interactionManager->setFirmwareVersion(static_cast<FirmwareVersion>(version));
				} else {
					m_interactionManager->errorValue();
				}
			} else if (x == ESP_CONTROL) {
				m_interactionManager->espControl();
				char espChoice;
				std::string tempString;
				bool continueWhileLoop = true;
				while (continueWhileLoop) {
					std::cin >> espChoice;
					switch (espChoice) {
						case '1':
							m_interactionManager->toggleESPSupport();
							m_interactionManager->espControl();
							break;
						case '2':
							std::cin >> tempString;
							m_interactionManager->setESPVoiceEnergy(tempString);
							m_interactionManager->espControl();
							break;
						case '3':
							std::cin >> tempString;
							m_interactionManager->setESPAmbientEnergy(tempString);
							m_interactionManager->espControl();
							break;
						case 'q':
							m_interactionManager->help();
							continueWhileLoop = false;
							break;
						default:
							m_interactionManager->errorValue();
							m_interactionManager->espControl();
							break;
					}
				}
	#ifdef ENABLE_COMMS
			} else if (x == COMMS_CONTROL) {
				m_interactionManager->commsControl();
				char commsChoice;
				bool continueWhileLoop = true;
				while (continueWhileLoop) {
					std::cin >> commsChoice;
					switch (commsChoice) {
						case 'a':
						case 'A':
							m_interactionManager->acceptCall();
							break;
						case 's':
						case 'S':
							m_interactionManager->stopCall();
							break;
						case 'q':
							m_interactionManager->help();
							continueWhileLoop = false;
							break;
						default:
							m_interactionManager->errorValue();
							continueWhileLoop = false;
							break;
					}
				}
	#endif
			} else {
				m_interactionManager->errorValue();
			}
		}
	}
	else
	{
		
		while(true)
		{
			    /**
				* BUG:21972
				*  Initialing to the default value.
				*/
				rcvbuffer.selectoption =0;
				//Receive an answer of message type 1.
				if (msgrcv(msqid, &rcvbuffer, sizeof(struct msgbuf), 1, 0) < 0)
				{
					syslog(LOG_ERR, "ALEXA: message receiving eror");
					continue;
				}	

				syslog(LOG_DEBUG, "ALEXA: received buffer in MSG Q: %d", rcvbuffer.selectoption);

				// #22267 Start: Synchronization between LED server & AVS after Booting of Shifu
				if (isAVSinInitFirstTime == false) {
					syslog(LOG_DEBUG, "ALEXA: skip recv buff in MSG Q :reason: Initially, AVS is not in ideal");
					continue;
				}
				// #22267 End.

				switch (rcvbuffer.selectoption) {
				case 1:
					/* #21219;
					* get the active channel and change the volume based on the channel priority.
					*/
					ActiveChannelPriority = m_interactionManager->getActiveChannelPriority();
					if(ActiveChannelPriority == FocusManagerInterface::ALERTS_CHANNEL_PRIORITY)
						m_interactionManager->adjustVolume(SpeakerInterface::Type::AVS_ALERTS_VOLUME, INCREASE_VOLUME);
					else
						m_interactionManager->adjustVolume(SpeakerInterface::Type::AVS_SPEAKER_VOLUME, INCREASE_VOLUME);
					break;
				case 2:
					/* #21219;
					* get the active channel and change the volume based on the channel priority.
					*/                
					ActiveChannelPriority = m_interactionManager->getActiveChannelPriority();
					if(ActiveChannelPriority == FocusManagerInterface::ALERTS_CHANNEL_PRIORITY)
						m_interactionManager->adjustVolume(SpeakerInterface::Type::AVS_ALERTS_VOLUME, DECREASE_VOLUME);
					else
						m_interactionManager->adjustVolume(SpeakerInterface::Type::AVS_SPEAKER_VOLUME, DECREASE_VOLUME);                  
					break;
				case 3:
					/**
					 * Feature #21131 : Start
					 * Description : [ALEXA][AVS]Function button support - wakeword detection and stop current activity
					 * Added/Changed by Smartron India Pvt. Ltd.
					 * */
					m_interactionManager->buttonActive();
					/**
					 * Feature #21131 : End
					 * */
					break;
				case 4:
					/* #21158: got microphone off message from hw-control, hence invoking microphone off api */
				   m_interactionManager->microphoneOff();
				   
					break; 
				 case 5:
				 	/* #21158: got microphone on message from hw-control, hence invoking microphone on api */
					m_interactionManager->microphoneOn();
						
					break;
								     
				default:
					m_interactionManager->errorValue();
					break;
				}
		}	
	}		
   	
    if (!userTriggeredLogout && m_restart) {
        return SampleAppReturnCode::RESTART;
    }
    return SampleAppReturnCode::OK;
}

void UserInputManager::onLogout() {
    m_restart = true;
}

void UserInputManager::onAuthStateChange(AuthObserverInterface::State newState, AuthObserverInterface::Error newError) {
    m_limitedInteraction = m_limitedInteraction || (newState == AuthObserverInterface::State::UNRECOVERABLE_ERROR);
}

void UserInputManager::onCapabilitiesStateChange(
    CapabilitiesObserverInterface::State newState,
    CapabilitiesObserverInterface::Error newError) {
    m_limitedInteraction = m_limitedInteraction || (newState == CapabilitiesObserverInterface::State::FATAL_ERROR);
}

void UserInputManager::controlSpeaker() {
    m_interactionManager->speakerControl();
    char speakerChoice;
    std::cin >> speakerChoice;
    if (SPEAKER_TYPES.count(speakerChoice) == 0) {
        m_interactionManager->errorValue();
    } else {
        m_interactionManager->volumeControl();
        SpeakerInterface::Type speaker = SPEAKER_TYPES.at(speakerChoice);
        char volume;
        while (std::cin >> volume && volume != 'q') {
            switch (volume) {
                case '1':
                    m_interactionManager->adjustVolume(speaker, INCREASE_VOLUME);
                    break;
                case '2':
                    m_interactionManager->adjustVolume(speaker, DECREASE_VOLUME);
                    break;
                case '3':
                    m_interactionManager->setMute(speaker, true);
                    break;
                case '4':
                    m_interactionManager->setMute(speaker, false);
                    break;
                case 'i':
                    m_interactionManager->volumeControl();
                    break;
                default:
                    m_interactionManager->errorValue();
                    break;
            }
        }
    }
}

#ifdef ENABLE_PCC
void UserInputManager::controlPhone() {
    std::string callerId;

    m_interactionManager->callId();
    std::string callId;
    std::cin >> callId;
    m_interactionManager->phoneControl();
    char phoneChoice;
    std::cin >> phoneChoice;
    switch (phoneChoice) {
        case '1':
            m_interactionManager->sendCallActivated(callId);
            break;
        case '2':
            m_interactionManager->sendCallTerminated(callId);
            break;
        case '3':
            m_interactionManager->sendCallFailed(callId);
            break;
        case '4':
            m_interactionManager->callerId();
            std::cin >> callerId;
            m_interactionManager->sendCallReceived(callId, callerId);
            break;
        case '5':
            m_interactionManager->callerId();
            std::cin >> callerId;
            m_interactionManager->sendCallerIdReceived(callId, callerId);
            break;
        case '6':
            m_interactionManager->sendInboundRingingStarted(callId);
            break;
        case '7':
            m_interactionManager->sendOutboundCallRequested(callId);
            break;
        case '8':
            m_interactionManager->sendOutboundRingingStarted(callId);
            break;
        case '9':
            m_interactionManager->sendSendDtmfSucceeded(callId);
            break;
        case '0':
            m_interactionManager->sendSendDtmfFailed(callId);
            break;
        case 'q':
            m_interactionManager->help();
            break;
        case 'i':
            m_interactionManager->phoneControl();
            break;
        default:
            m_interactionManager->errorValue();
            break;
    }
}

#endif

bool UserInputManager::confirmReset() {
    m_interactionManager->confirmResetDevice();
    char y;
    do {
        std::cin >> y;
        // Check the Setting which has to be changed.
        switch (y) {
            case 'Y':
            case 'y':
                m_interactionManager->resetDevice();
                return true;
            case 'N':
            case 'n':
                return false;
            default:
                m_interactionManager->errorValue();
                m_interactionManager->confirmResetDevice();
                break;
        }
    } while (true);

    return false;
}

bool UserInputManager::confirmReauthorizeDevice() {
    m_interactionManager->confirmReauthorizeDevice();
    char y;
    do {
        std::cin >> y;
        // Check the Setting which has to be changed.
        switch (y) {
            case 'Y':
            case 'y':
                m_interactionManager->resetDevice();
                return true;
            case 'N':
            case 'n':
                return false;
            default:
                m_interactionManager->errorValue();
                m_interactionManager->confirmReauthorizeDevice();
                break;
        }
    } while (true);

    return false;
}

void boolSettingMenu(std::function<void(bool)> setFunction) {
    char y;
    std::cin >> y;
    switch (y) {
        case ENABLE:
            setFunction(true);
            return;
        case DISABLE:
            setFunction(false);
            return;
    }
}

void UserInputManager::settingsMenu() {
    m_interactionManager->settings();
    char y;
    std::cin >> y;
    // Check the Setting which has to be changed.
    switch (y) {
        case (char)SettingsValues::LOCALE: {
            char localeValue;
            m_interactionManager->locale();
            std::cin >> localeValue;
            auto searchLocale = LOCALE_VALUES.find(localeValue);
            if (searchLocale != LOCALE_VALUES.end()) {
                m_interactionManager->changeSetting("locale", searchLocale->second);
            } else {
                m_interactionManager->errorValue();
            }
            return;
        }
        case (char)SettingsValues::DO_NOT_DISTURB: {
            m_interactionManager->doNotDisturb();
            boolSettingMenu([this](bool enable) { m_interactionManager->setDoNotDisturbMode(enable); });
            return;
        }
    }
    m_interactionManager->help();
}

}  // namespace sampleApp
}  // namespace alexaClientSDK
