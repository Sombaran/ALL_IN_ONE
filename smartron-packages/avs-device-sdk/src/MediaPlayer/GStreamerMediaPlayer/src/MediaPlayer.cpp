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

#include <cmath>
#include <cstring>
#include <unordered_map>
#include <iostream> 
#include <string> 
// Redmine 21501: start
#include <syslog.h>
#include <unistd.h>
// Redmine 21501: End

#include <AVSCommon/AVS/Attachment/AttachmentReader.h>
#include <AVSCommon/AVS/SpeakerConstants/SpeakerConstants.h>
#include <AVSCommon/Utils/Logger/Logger.h>
#include <AVSCommon/Utils/Memory/Memory.h>
// Redmine 21230, 21232: Start
#include <AVSCommon/Utils/Bluetooth/SDPRecords.h>
// Redmine 21230, 21232: End
// Redmine 21470: Start
#include "BlueZ/BlueZBluetoothDevice.h"
// Redmine 21470: End

#include <PlaylistParser/PlaylistParser.h>
#include <PlaylistParser/UrlContentToAttachmentConverter.h>

#include "MediaPlayer/AttachmentReaderSource.h"
#include "MediaPlayer/ErrorTypeConversion.h"
#include "MediaPlayer/IStreamSource.h"
#include "MediaPlayer/Normalizer.h"

#include "MediaPlayer/MediaPlayer.h"

#include "shifu-led.h" 
#include "AVSCommon/Utils/LedInterface.h"
#include "AVSCommon/Utils/UCIInterface.h"

// Redmine 21230,21232 Start: Macros for shifu connection types and global variables calling with extern
#define SHIFU_SOURCE 2
#define SHIFU_SINK 1
#define SHIFU_NONE 0

extern uint8_t shifu_bt_device_connected;
extern bool bt_sync_alsa_mute;
extern uint8_t bt_sync_alsa_volume;
// Redmine 21230, 21232: End

// Redmine 21137: Start: Defining Player ID for Bluetooth Player differentiate 
static const uint8_t BLUETOOTH_PLAYER = 4;
// Redmine 21137: End

namespace alexaClientSDK {
namespace mediaPlayer {

using namespace avsCommon::avs::attachment;
using namespace avsCommon::avs::speakerConstants;
using namespace avsCommon::sdkInterfaces;
using namespace avsCommon::utils;
using namespace avsCommon::utils::mediaPlayer;
using namespace avsCommon::utils::memory;
using namespace avsCommon::utils::configuration;
using namespace alexaClientSDK::bluetoothImplementations::blueZ;

/* hardware volume values of shifu for 10 levels */
/**
 * Bug #21332: Start: [Shifu][Alexa] Audio volume levels - user experience
 **/
//int volume_table[] = {0, 25, 32, 38, 46, 50, 56, 60, 64, 68, 71};
int volume_table[] = {0, 44, 47, 50, 53, 56, 59, 62, 65, 68, 71};
/**
 * Bug #21332: End.
 **/

/* start #21219
* adding below variables to store the previous volume of corresponding player
*/
/// The volume to restore for alerts when exiting muted state.
gdouble m_AlertlastVolume;

/// The volume to restore for content/dialogue when exiting muted state.
gdouble m_SpeakerlastVolume; 
/* end #21329*/

/* #21812: Currently active player speaker type.
	If playing alerts, type will be AVS_ALERTS_VOLUME
	else type is AVS_SPEAKER_VOLUME 
*/
avsCommon::sdkInterfaces::SpeakerInterface::Activemedia activeSpeakerType;

/// String to identify log entries originating from this file.
static const std::string TAG("MediaPlayer");

static const std::string MEDIAPLAYER_CONFIGURATION_ROOT_KEY = "gstreamerMediaPlayer";
/// The key in our config file to set the audioSink.
static const std::string MEDIAPLAYER_AUDIO_SINK_KEY = "audioSink";
static const std::string MEDIAPLAYER_AUDIO_VOLUME_KEY = "volume";

/** fix for 21208 **/
static const std::string MEDIAPLAYER_BG_LOW_AUDIO_VOLUME_KEY = "bg_low_vol";

/// The key in our config file to find the output conversion type.
static const std::string MEDIAPLAYER_OUTPUT_CONVERSION_ROOT_KEY = "outputConversion";
/// The acceptable conversion keys to find in the config file
/// Key strings are mapped to gstreamer capabilities documented here:
/// https://gstreamer.freedesktop.org/documentation/design/mediatype-audio-raw.html
static const std::unordered_map<std::string, int> MEDIAPLAYER_ACCEPTED_KEYS = {{"rate", G_TYPE_INT},
                                                                               {"format", G_TYPE_STRING},
                                                                               {"channels", G_TYPE_INT}};

/// A counter used to increment the source id when a new source is set.
static MediaPlayer::SourceId g_id{0};

/// A link to @c MediaPlayerInterface::ERROR.
static const MediaPlayer::SourceId ERROR_SOURCE_ID = MediaPlayer::ERROR;

/// A value to indicate an unqueued callback. g_idle_add() only returns ids >= 0.
static const guint UNQUEUED_CALLBACK = guint(0);

/**
 * Create a LogEntry using this file's TAG and the specified event string.
 *
 * @param The event string for this @c LogEntry.
 */
#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

/// Timeout value for calls to @c gst_element_get_state() calls.
static const unsigned int TIMEOUT_ZERO_NANOSECONDS(0);

/// GStreamer Volume Element Minimum.
static const int8_t GST_SET_VOLUME_MIN = 0;

/// GStreamer Volume Element Maximum.
static const int8_t GST_SET_VOLUME_MAX = 1;

/// GStreamer Volume Adjust Minimum.
static const int8_t GST_ADJUST_VOLUME_MIN = -1;

/// GStreamer Volume Adjust Maximum.
static const int8_t GST_ADJUST_VOLUME_MAX = 1;
// Redmine 21230,21232: start: Volume table initialization for sink
// Redmine 22027: Start: Pulse-audio volume table is initialized with level '5' instead of '0' to take care of low volume scenarios
char pa_volume_table_shifu[] = {5,25,32,40,47,52,58,67,75,89,100};
// Redmine 22027: End
// Redmine 21230,21232: End

/// Represents the zero volume to avoid the actual 0.0 value. Used as a fix for GStreamer crashing on 0 volume for PCM.
static const gdouble VOLUME_ZERO = 0.0000001;

/// #21329: Variable for Volume level 1
static const gdouble HW_VOLUME_ZERO = 0.1;

/// #21329: mute volume
static const gdouble HW_VOLUME_MUTE = 0;

/// #21329: mute led level
static const gdouble HW_MUTE_LED = 0;

/// Mimimum level for equalizer bands
static const int MIN_EQUALIZER_LEVEL = -24;

/// Maximum level for equalizer bands
static const int MAX_EQUALIZER_LEVEL = 12;

/// The GStreamer property name for the frequency band 100 Hz.
static char GSTREAMER_BASS_BAND_NAME[] = "band0";

/// The GStreamer property name for the frequency band 1.1 kHz.
static char GSTREAMER_MIDRANGE_BAND_NAME[] = "band1";

/// The GStreamer property name for the frequency band 11 kHz.
static char GSTREAMER_TREBLE_BAND_NAME[] = "band2";

/**
 * Processes tags found in the tagList.
 * Called through gst_tag_list_foreach.
 *
 * @param tagList List of tags to iterate over.
 * @param tag A specific tag from the tag list.
 * @param pointerToMutableVectorOfTags Pointer to VectorOfTags. Use push_back to preserve order.
 *
 */
static void collectOneTag(const GstTagList* tagList, const gchar* tag, gpointer pointerToMutableVectorOfTags) {
    auto vectorOfTags = static_cast<VectorOfTags*>(pointerToMutableVectorOfTags);
    int num = gst_tag_list_get_tag_size(tagList, tag);
    for (int index = 0; index < num; ++index) {
        const GValue* val = gst_tag_list_get_value_index(tagList, tag, index);
        MediaPlayerObserverInterface::TagKeyValueType tagKeyValueType;
        tagKeyValueType.key = std::string(tag);
        if (G_VALUE_HOLDS_STRING(val)) {
            tagKeyValueType.value = std::string(g_value_get_string(val));
            tagKeyValueType.type = MediaPlayerObserverInterface::TagType::STRING;
        } else if (G_VALUE_HOLDS_UINT(val)) {
            tagKeyValueType.value = std::to_string(g_value_get_uint(val));
            tagKeyValueType.type = MediaPlayerObserverInterface::TagType::UINT;
        } else if (G_VALUE_HOLDS_INT(val)) {
            tagKeyValueType.value = std::to_string(g_value_get_int(val));
            tagKeyValueType.type = MediaPlayerObserverInterface::TagType::INT;
        } else if (G_VALUE_HOLDS_BOOLEAN(val)) {
            tagKeyValueType.value = std::string(g_value_get_boolean(val) ? "true" : "false");
            tagKeyValueType.type = MediaPlayerObserverInterface::TagType::BOOLEAN;
        } else if (GST_VALUE_HOLDS_DATE_TIME(val)) {
            GstDateTime* dt = static_cast<GstDateTime*>(g_value_get_boxed(val));
            gchar* dt_str = gst_date_time_to_iso8601_string(dt);
            if (!dt_str) {
                continue;
            }
            tagKeyValueType.value = std::string(dt_str);
            tagKeyValueType.type = MediaPlayerObserverInterface::TagType::STRING;
            g_free(dt_str);
        } else if (G_VALUE_HOLDS_DOUBLE(val)) {
            tagKeyValueType.value = std::to_string(g_value_get_double(val));
            tagKeyValueType.type = MediaPlayerObserverInterface::TagType::DOUBLE;
        } else {
            /*
             * Ignore GST_VALUE_HOLDS_BUFFER and other types.
             */
            continue;
        }
        vectorOfTags->push_back(tagKeyValueType);
    }
}

std::shared_ptr<MediaPlayer> MediaPlayer::create(
    std::shared_ptr<avsCommon::sdkInterfaces::HTTPContentFetcherInterfaceFactoryInterface> contentFetcherFactory,
    bool enableEqualizer,
    SpeakerInterface::Type type,
    std::string name) {
    ACSDK_DEBUG9(LX("createCalled"));
    std::shared_ptr<MediaPlayer> mediaPlayer(new MediaPlayer(contentFetcherFactory, enableEqualizer, type, name));
    if (mediaPlayer->init()) {
        return mediaPlayer;
    } else {
        return nullptr;
    }
};

MediaPlayer::~MediaPlayer() {
    ACSDK_DEBUG9(LX("~MediaPlayerCalled"));
    cleanUpSource();
    g_main_loop_quit(m_mainLoop);
    if (m_mainLoopThread.joinable()) {
        m_mainLoopThread.join();
    }
    gst_object_unref(m_pipeline.pipeline);
    resetPipeline();

    removeSource(m_busWatchId);
    g_main_loop_unref(m_mainLoop);

    g_main_context_unref(m_workerContext);
}

MediaPlayer::SourceId MediaPlayer::setSource(
    std::shared_ptr<avsCommon::avs::attachment::AttachmentReader> reader,
    const avsCommon::utils::AudioFormat* audioFormat) {
    ACSDK_DEBUG9(LX("setSourceCalled").d("sourceType", "AttachmentReader"));
    std::promise<MediaPlayer::SourceId> promise;
    auto future = promise.get_future();
    std::function<gboolean()> callback = [this, &reader, &promise, audioFormat]() {
        handleSetAttachmentReaderSource(std::move(reader), &promise, audioFormat);
        return false;
    };
    if (queueCallback(&callback) != UNQUEUED_CALLBACK) {
        return future.get();
    }
    return ERROR_SOURCE_ID;
}

MediaPlayer::SourceId MediaPlayer::setSource(std::shared_ptr<std::istream> stream, bool repeat) {
    ACSDK_DEBUG9(LX("setSourceCalled").d("sourceType", "istream"));
    std::promise<MediaPlayer::SourceId> promise;
    auto future = promise.get_future();
    std::function<gboolean()> callback = [this, &stream, repeat, &promise]() {
        handleSetIStreamSource(stream, repeat, &promise);
        return false;
    };
    if (queueCallback(&callback) != UNQUEUED_CALLBACK) {
        return future.get();
    }
    return ERROR_SOURCE_ID;
}

MediaPlayer::SourceId MediaPlayer::setSource(const std::string& url, std::chrono::milliseconds offset, bool repeat) {
    ACSDK_DEBUG9(LX("setSourceForUrlCalled").sensitive("url", url));
    std::promise<MediaPlayer::SourceId> promise;
    auto future = promise.get_future();
    std::function<gboolean()> callback = [this, url, offset, &promise, repeat]() {
        handleSetUrlSource(url, offset, &promise, repeat);
        return false;
    };
    if (queueCallback(&callback) != UNQUEUED_CALLBACK) {
        return future.get();
    }
    return ERROR_SOURCE_ID;
}

uint64_t MediaPlayer::getNumBytesBuffered() {
    ACSDK_DEBUG9(LX("getNumBytesBuffered"));
    guint64 bytesBuffered = 0;
    if (m_pipeline.appsrc) {
        bytesBuffered = gst_app_src_get_current_level_bytes(GST_APP_SRC(m_pipeline.appsrc));
    }
    if (m_pipeline.decodedQueue) {
        guint bytesInQueue = 0;
        g_object_get(m_pipeline.decodedQueue, "current-level-bytes", &bytesInQueue, NULL);
        bytesBuffered += bytesInQueue;
    }
    return bytesBuffered;
}

bool MediaPlayer::play(MediaPlayer::SourceId id) {
	double volume_le;
   
    ACSDK_DEBUG9(LX("playCalled"));
    if (!m_source) {
        ACSDK_ERROR(LX("playFailed").d("reason", "sourceNotSet"));
        return ERROR;
    }
    // Redmine 21501, 21956, 22119: start: Playing White_noise wav file with 'paplay'(at 3276=5% pa_volume level) based on player_type and sink_vol>1 when external_bt speaker is connected
    if(PlayerName == "SpeakMediaPlayer")
    {
        if((shifu_bt_device_connected == SHIFU_SINK) && (bt_sync_alsa_volume > 1))
        {
            syslog(LOG_DEBUG,"[SHIFU][AVS][BT]:%s: Playing white_noise", __func__);
            system("paplay -p /etc/bluetooth/white_noise.wav --device=bluez_sink --volume=3276 &");
            sleep(1);
        }
	    // Redmine 22240: strat: Pulse audio alsa_output is set to high if there is speakeMediaplayer when source(phone_bt) is connected.
        else if(shifu_bt_device_connected == SHIFU_SOURCE)
        {
            syslog(LOG_DEBUG,"[SHIFU][AVS][BT]:%s: Pa_volume max when speak mediaplayer", __func__);
            system("pactl set-sink-volume alsa_output.0 100%");
        }
	    // Redmine 22240: End
	}
    // Redmine 21501, 21956, 22119: End
    m_source->preprocess();

    // Redmine 21470: Start: If Other audio player is Playing, BT source disconnection process will be initiated.
    if((PlayerName == "AudioMediaPlayer") && (shifu_bt_device_connected == SHIFU_SOURCE))
    {
        if(!BlueZBluetoothDevice::shifu_last_bt_disconnect())
        {
            syslog(LOG_ERR,"[SHIFU][AVS][BT]:%s: Bluetooth Disconnection Failure", __func__);
        }
        //Redmine 22184: Start: Trigerring BT_Disconnection Led_Pattern for Cloud_Playback initialization
        else
        {
            syslog(LOG_DEBUG,"[SHIFU][AVS][BT]:%s: Bluetooth Disconnection sucess with cloud_playback Initialization", __func__);
            alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_BLUTOOTH_CONNECTED, 1, 0);
        }
        //Redmine 22184: End:
    }
    // Redmine 21470: End

	/* #22033: #21812:
		Activespeaker type will be set based on the playback player name.
		activeSpeaker type will be AVS_MEDIA_ALL, if bothe players(alerts and speaker) are active
		This will be used to set the player volume when both alerts player and speaker player are cative.
	*/
    if(PlayerName == "AlertsMediaPlayer")
		if(activeSpeakerType == SpeakerInterface::Activemedia::AVS_MEDIA_SPEAKER)
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_ALL;
		else
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_ALERTS;
	else
		if(activeSpeakerType == SpeakerInterface::Activemedia::AVS_MEDIA_ALERTS)
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_ALL;
		else
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_SPEAKER;
		  
		
	ACSDK_DEBUG8(LX("playCalled").d("active player type", activeSpeakerType)); 
    
    ///< #21219: set each player volume before starting
    ACSDK_DEBUG9(LX("playCalled").d("player name", PlayerName));
    SetPlayerVolume();
       
    std::string volume_string;
    ConfigurationNode::getRoot()[MEDIAPLAYER_CONFIGURATION_ROOT_KEY].getString(MEDIAPLAYER_AUDIO_VOLUME_KEY, &volume_string, "1.0");
        
    volume_le= std::stod(volume_string);
    g_object_set(m_pipeline.volume, "volume", volume_le, NULL);

    std::cout <<volume_le <<std::endl;
    std::promise<bool> promise;
    auto future = promise.get_future();
    std::function<gboolean()> callback = [this, id, &promise]() {
        handlePlay(id, &promise);
        return false;
    };

    if (queueCallback(&callback) != UNQUEUED_CALLBACK) {
        return future.get();
    }
    IsPlayerActive = true;
    return false;
}

bool MediaPlayer::stop(MediaPlayer::SourceId id) {
    ACSDK_DEBUG9(LX("stopCalled"));
    std::promise<bool> promise;
    auto future = promise.get_future();
    std::function<gboolean()> callback = [this, id, &promise]() {
        handleStop(id, &promise);
        return false;
    };
    if (queueCallback(&callback) != UNQUEUED_CALLBACK) {
        return future.get();
    }
    
    /* #21329
    * Make player state to false(not playing) when the player calls stop function
    */
    IsPlayerActive = false;
    
	/* #22033: #21812:
		Activespeaker type will be set based on the playback player name.
		activeSpeaker type will be AVS_MEDIA_ALL, if bothe players(alerts and speaker) are active
		This will be used to set the player volume when both alerts player and speaker player are cative.
	*/
    if(PlayerName == "AlertsMediaPlayer")
		if(activeSpeakerType == SpeakerInterface::Activemedia::AVS_MEDIA_ALL)
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_SPEAKER;
		else
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_NONE;
	else
		if(activeSpeakerType == SpeakerInterface::Activemedia::AVS_MEDIA_ALL)
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_ALERTS;
		else
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_NONE;
		
    		
    return false;
}

/** fix for 21208 **/
bool MediaPlayer::setLowVolume(MediaPlayer::SourceId id) {
    ACSDK_DEBUG9(LX("setLowVolumeCalled"));
    double volume_le;
    std::string volume_string;

    ConfigurationNode::getRoot()[MEDIAPLAYER_CONFIGURATION_ROOT_KEY].getString(MEDIAPLAYER_BG_LOW_AUDIO_VOLUME_KEY, &volume_string, "0.07");

    volume_le= std::stod(volume_string);
    g_object_set(m_pipeline.volume, "volume", volume_le, NULL);
    return true;
}

bool MediaPlayer::setActualVolume(MediaPlayer::SourceId id) {
    ACSDK_DEBUG9(LX("setActualVolumeCalled"));
    double volume_le;
    char amixer_cmd[32] = {0};  ///< volume set command
    double mute;    ///< to store the mute status of the player
    

    /* get the players mute status */
    mute = m_isMuted;
    
    /* #21329
    * The volume of song will be set to previous level when it comes from background to foreground.
    * increase the hardware volume only when mute is false 
    */
    if(!mute)
    {
        snprintf(amixer_cmd, sizeof(amixer_cmd), "amixer -c 0 sset 'DAC1' %d", get_volume_to_float_index(m_SpeakerlastVolume));
        system(amixer_cmd);
    }
    
    volume_le= std::stod("0.8");
    g_object_set(m_pipeline.volume, "volume", volume_le, NULL);
    return true;
}

bool MediaPlayer::pause(MediaPlayer::SourceId id) {
    ACSDK_DEBUG9(LX("pausedCalled"));
    std::promise<bool> promise;
    auto future = promise.get_future();
    IsPlayerActive = false;
    
	/* #22033: #21812:
		Activespeaker type will be set based on the playback player name.
		activeSpeaker type will be AVS_MEDIA_ALL, if bothe players(alerts and speaker) are active
		This will be used to set the player volume when both alerts player and speaker player are cative.
	*/
    if(PlayerName == "AlertsMediaPlayer")
		if(activeSpeakerType == SpeakerInterface::Activemedia::AVS_MEDIA_ALL)
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_SPEAKER;
		else
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_NONE;
	else
		if(activeSpeakerType == SpeakerInterface::Activemedia::AVS_MEDIA_ALL)
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_ALERTS;
		else
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_NONE;
    
    std::function<gboolean()> callback = [this, id, &promise]() {
        handlePause(id, &promise);
        return false;
    };
    if (queueCallback(&callback) != UNQUEUED_CALLBACK) {
        return future.get();
    }
    return false;
}

bool MediaPlayer::resume(MediaPlayer::SourceId id) {
    ACSDK_DEBUG9(LX("resumeCalled"));
    std::promise<bool> promise;
	///< #21219: set player volume before starting playback
	ACSDK_DEBUG8(LX("resume called").d("m_isMuted", m_isMuted).d("m_SpeakerlastVolume", m_SpeakerlastVolume));
	if(m_SpeakerlastVolume > 0)
	{
		if(!m_isMuted)
			SetPlayerVolume();
	}
	else
        system("amixer -c 0 sset 'DAC1' 0");		
		
    auto future = promise.get_future();
    std::function<gboolean()> callback = [this, id, &promise]() {
        handleResume(id, &promise);
        return false;
    };
    if (queueCallback(&callback) != UNQUEUED_CALLBACK) {
        return future.get();
    }
    return false;
}

/**
 * Bug #21360 : Start
 * Description : After removing the internet ,music is not playing in the low volume level if we interrupt the device
 * Changed by Smartron India Pvt. Ltd.
 * */

bool MediaPlayer::isMediaActive(MediaPlayer::SourceId id) {
    ACSDK_DEBUG9(LX("isMediaActiveCalled"));
    return IsPlayerActive;
}
/**
 * Bug #21360 : End
 * */

std::chrono::milliseconds MediaPlayer::getOffset(MediaPlayer::SourceId id) {
    ACSDK_DEBUG9(LX("getOffsetCalled"));
    std::promise<std::chrono::milliseconds> promise;
    auto future = promise.get_future();
    std::function<gboolean()> callback = [this, id, &promise]() {
        handleGetOffset(id, &promise);
        return false;
    };

    if (queueCallback(&callback) != UNQUEUED_CALLBACK) {
        return future.get();
    }
    return MEDIA_PLAYER_INVALID_OFFSET;
}

void MediaPlayer::setObserver(std::shared_ptr<MediaPlayerObserverInterface> observer) {
    ACSDK_DEBUG9(LX("setObserverCalled"));
    std::promise<void> promise;
    auto future = promise.get_future();
    std::function<gboolean()> callback = [this, &promise, &observer]() {
        handleSetObserver(&promise, observer);
        return false;
    };

    if (queueCallback(&callback) != UNQUEUED_CALLBACK) {
        future.wait();
    }
}

bool MediaPlayer::setVolume(int8_t volume) {
    ACSDK_DEBUG9(LX("setVolumeCalled"));
    std::promise<bool> promise;
    auto future = promise.get_future();
    std::function<gboolean()> callback = [this, &promise, volume]() {
        handleSetVolume(&promise, volume);
        return false;
    };
    if (queueCallback(&callback) != UNQUEUED_CALLBACK) {
        return future.get();
    }
    return false;
}
/** #21113: handleSetVolumeInternal definition is modified by smartron  **/
/**
 * handleSetVolumeInternal() - controls the volume based on speaker type ie, dialogue/alerts volume. 
 * @param[in] gstVolume value of volume to set.
 * @return  void
*/
void MediaPlayer::handleSetVolumeInternal(gdouble gstVolume) {
    
    ACSDK_DEBUG8(LX("handleSetVolumeInternal called").d("gstVolume", gstVolume).d("Speaker type", activeSpeakerType));
    
    avsCommon::sdkInterfaces::SpeakerInterface::Type speakerType;   ///< type of the speaker: AVS_SPEAKER_VOLUME/ AVS_ALERTS_VOLUME
    int volume;         ///< index of shifu volume array
    int volume_led = 0;     ///< level of the volume to send to led server
    char amixer_cmd[32] = {0};      ///< volume set command
    // Redmine 21230,21232 Start: Assiging the sink volume variable with '0'
    char pavolume_cmd[64] = {0};
    // Redmine 21230,21232 End
    gboolean mute;  ///< to store the mute status of the player
    
     /* #21329, #21219
    * set the hardware volume based on the mute status
    */
    mute = m_isMuted;
    
    /* #21329 #21219 update the mute status for +ve volume change ie, make it false if it is true, because we are changing the volume */
    if(mute && (gstVolume > 0))
    {
        m_isMuted = false;
    }
    
    /* get the current speaker type */
    speakerType = getSpeakerType();
    ACSDK_DEBUG8(LX("handleSetVolumeInternal").d("Speaker type", speakerType));
    
    volume = get_volume_to_float_index(gstVolume);
    volume_led = get_volume_led(volume);
    alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_VOLUME, 1, volume_led);   /// led indication    
    
    /* save the present volume based on the player type */
    if(speakerType == avsCommon::sdkInterfaces::SpeakerInterface::Type::AVS_ALERTS_VOLUME)
    {
        m_AlertlastVolume = gstVolume;

        /* #22115: writing the updated alerts volume to uci */
        alexaClientSDK::avsCommon::utils::uci::SetparamUCI("shifu.conf", "AVSALERTSVOLUME", std::to_string(m_AlertlastVolume));
        
        /* #21812: #22033: set alerts volume only when alerts player is in active
         *  otherwise indicate updated volume to user using led pattern and store the alerts volume
        */
		if(activeSpeakerType != SpeakerInterface::Activemedia::AVS_MEDIA_ALERTS)
			return;
    }
    else
    {
        m_SpeakerlastVolume = gstVolume;

        /* #22115: writing the updated speaker volume to uci */
        alexaClientSDK::avsCommon::utils::uci::SetparamUCI("shifu.conf", "AVSSPEAKERVOLUME", std::to_string(m_SpeakerlastVolume));

        /* #21812: #22033: Set the speaker volume when alert speaker is not in active. 
         * Indicate user that volume is updated using led pattern if alert is playing.
        */
		if(activeSpeakerType == SpeakerInterface::Activemedia::AVS_MEDIA_ALERTS)
			return;
    }    

	/* set hardware volume here */
    snprintf(amixer_cmd, sizeof(amixer_cmd), "amixer -c 0 sset 'DAC1' %d", volume); 
    system(amixer_cmd);
    
    // Redmine 21230,21232 Start: bt_sink volume is stored with hub_volume level
    bt_sync_alsa_volume = (uint8_t)volume_led;

    if(shifu_bt_device_connected == SHIFU_SINK)
    {
        snprintf(pavolume_cmd, sizeof(pavolume_cmd), "pactl set-sink-volume bluez_sink %d%%", pa_volume_table_shifu[bt_sync_alsa_volume]);
        system(pavolume_cmd);
    }
    // Redmine 21230,21232: End
    m_lastVolume = gstVolume;
}

void MediaPlayer::handleSetVolume(std::promise<bool>* promise, int8_t volume) {
    ACSDK_DEBUG9(LX("handleSetVolumeCalled"));
    auto toGstVolume =
        Normalizer::create(AVS_SET_VOLUME_MIN, AVS_SET_VOLUME_MAX, GST_SET_VOLUME_MIN, GST_SET_VOLUME_MAX);
    if (!toGstVolume) {
        ACSDK_ERROR(LX("handleSetVolumeFailed").d("reason", "createNormalizerFailed"));
        promise->set_value(false);
        return;
    }

    gdouble gstVolume;
    if (!m_pipeline.volume) {
        ACSDK_ERROR(LX("handleSetVolumeFailed").d("reason", "volumeElementNull"));
        promise->set_value(false);
        return;
    }

    if (!toGstVolume->normalize(volume, &gstVolume)) {
        ACSDK_ERROR(LX("handleSetVolumeFailed").d("reason", "normalizeVolumeFailed"));
        promise->set_value(false);
        return;
    }

    handleSetVolumeInternal(gstVolume);
    promise->set_value(true);
}

bool MediaPlayer::adjustVolume(int8_t delta) {
    ACSDK_DEBUG9(LX("adjustVolumeCalled"));
    std::promise<bool> promise;
    auto future = promise.get_future();
    std::function<gboolean()> callback = [this, &promise, delta]() {
        handleAdjustVolume(&promise, delta);
        return false;
    };
    if (queueCallback(&callback) != UNQUEUED_CALLBACK) {
        return future.get();
    }
    return false;
}

void MediaPlayer::handleAdjustVolume(std::promise<bool>* promise, int8_t delta) {
    ACSDK_DEBUG9(LX("handleAdjustVolumeCalled"));
    auto toGstDeltaVolume =
        Normalizer::create(AVS_ADJUST_VOLUME_MIN, AVS_ADJUST_VOLUME_MAX, GST_ADJUST_VOLUME_MIN, GST_ADJUST_VOLUME_MAX);

    if (!toGstDeltaVolume) {
        ACSDK_ERROR(LX("handleAdjustVolumeFailed").d("reason", "createNormalizerFailed"));
        promise->set_value(false);
        return;
    }

    if (!m_pipeline.volume) {
        ACSDK_ERROR(LX("adjustVolumeFailed").d("reason", "volumeElementNull"));
        promise->set_value(false);
        return;
    }

    gdouble gstVolume;
    g_object_get(m_pipeline.volume, "volume", &gstVolume, NULL);
    
    /*#21219 get hardware volume as we are not using gstreamer volume */
    /* #21843: Adjust the volume based on the player type. */
    if(getSpeakerType() == SpeakerInterface::Type::AVS_ALERTS_VOLUME)
    	gstVolume = m_AlertlastVolume;
    else
    	gstVolume = m_SpeakerlastVolume;
    

    gdouble gstDelta;
    if (!toGstDeltaVolume->normalize(delta, &gstDelta)) {
        ACSDK_ERROR(LX("adjustVolumeFailed").d("reason", "normalizeVolumeFailed"));
        promise->set_value(false);
        return;
    }

    gstVolume += gstDelta;

    // If adjustment exceeds bounds, cap at max/min.
    gstVolume = std::min(gstVolume, static_cast<gdouble>(GST_SET_VOLUME_MAX));
    gstVolume = std::max(gstVolume, static_cast<gdouble>(GST_SET_VOLUME_MIN));

	/*21570: some times while normalizing the volume, we are getting gstvolume > 0 and < 0.1, this is for volume level 0.
	So making any value < 0.1 to 0.
	*/
	if(gstVolume < 0.1)
		gstVolume = 0;
			
    handleSetVolumeInternal(gstVolume);
    promise->set_value(true);
}

bool MediaPlayer::setMute(bool mute) {
    ACSDK_DEBUG9(LX("setMuteCalled"));
    std::promise<bool> promise;
    auto future = promise.get_future();
    std::function<gboolean()> callback = [this, &promise, mute]() {
        handleSetMute(&promise, mute);
        return false;
    };
    if (queueCallback(&callback) != UNQUEUED_CALLBACK) {
        return future.get();
    }
    return false;
}

/* #21329
* definition changed by smartron to meet the alexa behaviour
*/
void MediaPlayer::handleSetMute(std::promise<bool>* promise, bool mute) {
    
    avsCommon::sdkInterfaces::SpeakerInterface::Type speakerType;   ///< type of the speaker: AVS_SPEAKER_VOLUME/ AVS_ALERTS_VOLUME
    int volume;         ///< index of shifu volume array
    int volume_led = 0;     ///< level of the volume to send to led server
    char amixer_cmd[32] = {0};      ///< volume set command
    // Redmine 21230,21232 Start: Sink volume level set to level '0' 
        char pavolume_cmd[64] = {0};
    // Redmine 21230,21232: End  
    ACSDK_DEBUG8(LX("handleSetMuteCalled").d("mute", mute));
    
    /* #21329:
    * Allow to execute this function when actual mute state and given mute state are different
    */
    if(m_isMuted == mute)
    {
        ACSDK_DEBUG8(LX("handleSetMuteCalled with same state").d("m_isMuted", m_isMuted));
        /* #21329 store the actual mute status here */
        promise->set_value(true);
        return;
    }
    
    if (!m_pipeline.volume) {
        ACSDK_ERROR(LX("setMuteFailed").d("reason", "volumeElementNull"));
        promise->set_value(false);
        return;
    }

    // A fix for GStreamer crashing for zero volume on PCM data
    // g_object_set(m_pipeline.volume, "volume", mute || m_lastVolume == 0 ? VOLUME_ZERO : m_lastVolume, NULL);
    
    /* #21329
    * get the player/speaker type.
    * save the different players current volumes in corresponding variables to use for unmute command
    */
    speakerType = getSpeakerType();
    
    ACSDK_DEBUG8(LX("handleSetMuteCalled").d("getSpeakerType", speakerType));
    if(mute)
    {
    /*#21329 save the volume while muting the device to use in unmute command */
        if(speakerType == SpeakerInterface::Type::AVS_ALERTS_VOLUME)
        {
            m_AlertlastVolume = get_volume_index();
            ACSDK_DEBUG8(LX("handleSetMuteCalled with mute").d("m_AlertlastVolume", m_AlertlastVolume));
        }
        else
        {
            /* #21329:
            * except ALERTS player remaning all use the same volume.
            * therefore save the first/speak player volume.
            */
            if(PlayerName == "SpeakMediaPlayer")
            {
                ACSDK_DEBUG9(LX("handleSetMuteCalled save the volume for SpeakMediaPlayer"));
                m_SpeakerlastVolume = get_volume_index();
            }
                
            ACSDK_DEBUG8(LX("handleSetMuteCalled with mute").d("m_SpeakerlastVolume", m_SpeakerlastVolume));
        }
        // Redmine 21230,21232 Start: variable is assigined with status as true if "mute" is success
                bt_sync_alsa_mute = true;
        // Redmine 21230,21232: End
        /* #21329: for mute led */
        alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_VOLUME, 1, HW_MUTE_LED);
        
        /* #21329: set volume to zero for mute command */
        snprintf(amixer_cmd, sizeof(amixer_cmd), "amixer -c 0 sset 'DAC1' %f", HW_VOLUME_MUTE);
        system(amixer_cmd);
        // Redmine 21230,21232 Start: Verifying the status of shifu_bt connected to sink device and volume command is issued
            if(shifu_bt_device_connected == SHIFU_SINK)
            {
                    snprintf(pavolume_cmd, sizeof(pavolume_cmd), "pactl set-sink-volume bluez_sink %d%%", pa_volume_table_shifu[0]);
                    system(pavolume_cmd);
            }
        // Redmine 21230,21232: End       
    }
    else
    {
        /* #21329: set the previous volume for unmute command based on player type */
        if(speakerType == SpeakerInterface::Type::AVS_ALERTS_VOLUME)
            volume = get_volume_to_float_index(m_AlertlastVolume);
        else
            volume = get_volume_to_float_index(m_SpeakerlastVolume);
            
        ACSDK_DEBUG8(LX("handleSetMuteCalled with unmute").d("volume", volume));
        
        /* #21329
        * if volume is zero level for unmute command, make it as level 1, ie 0.1
        */
        if(volume == 0)
            volume = HW_VOLUME_ZERO;
            
        volume_led = get_volume_led(volume);
        // Redmine 21230,21232 Start: variable is assigined with status as false if in case of "unmute"
                bt_sync_alsa_mute = false;
        //  Redmine 21230,21232: End    
        /* #21329: led for previous volume when user asks unmute */
        alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_VOLUME, 1, volume_led);
        snprintf(amixer_cmd, sizeof(amixer_cmd), "amixer -c 0 sset 'DAC1' %d", volume);
        system(amixer_cmd);
        // Redmine 21230,21232 Start: Verifying the status of shifu_bt connected to sink device and volume command issued 
            if(shifu_bt_device_connected == SHIFU_SINK)
            {
                    snprintf(pavolume_cmd, sizeof(pavolume_cmd), "pactl set-sink-volume bluez_sink %d%%", pa_volume_table_shifu[(int)volume_led]);
                    system(pavolume_cmd);
            }
        // Redmine 21230,21232: End 
    }
    
    /* store the actual mute status here */
    m_isMuted = mute;
    promise->set_value(true);
}

bool MediaPlayer::getSpeakerSettings(SpeakerInterface::SpeakerSettings* settings) {
    ACSDK_DEBUG9(LX("getSpeakerSettingsCalled"));
    std::promise<bool> promise;
    auto future = promise.get_future();
    std::function<gboolean()> callback = [this, &promise, settings]() {
        handleGetSpeakerSettings(&promise, settings);
        return false;
    };
    if (queueCallback(&callback) != UNQUEUED_CALLBACK) {
        return future.get();
    }
    return false;
}

void MediaPlayer::handleGetSpeakerSettings(
    std::promise<bool>* promise,
    avsCommon::sdkInterfaces::SpeakerInterface::SpeakerSettings* settings) {
    ACSDK_DEBUG9(LX("handleGetSpeakerSettingsCalled"));
    if (!settings) {
        ACSDK_ERROR(LX("getSpeakerSettingsFailed").d("reason", "nullSettings"));
        promise->set_value(false);
        return;
    } else if (!m_pipeline.volume) {
        ACSDK_ERROR(LX("getSpeakerSettingsFailed").d("reason", "volumeElementNull"));
        promise->set_value(false);
        return;
    }

    auto toAVSVolume =
        Normalizer::create(GST_SET_VOLUME_MIN, GST_SET_VOLUME_MAX, AVS_SET_VOLUME_MIN, AVS_SET_VOLUME_MAX);
    if (!toAVSVolume) {
        ACSDK_ERROR(LX("handleGetSpeakerSettingsFailed").d("reason", "createNormalizerFailed"));
        promise->set_value(false);
        return;
    }

    avsCommon::sdkInterfaces::SpeakerInterface::Type speakerType;   ///< type of the speaker: AVS_SPEAKER_VOLUME/ AVS_ALERTS_VOLUME
    gdouble avsVolume;
    gdouble gstVolume;
    gboolean mute;
    g_object_get(m_pipeline.volume, "volume", &gstVolume, "mute", &mute, NULL);
    
    /* get the current speaker type */
    speakerType = getSpeakerType();

    /// A part of GStreamer crash fix for zero volume on PCM data
    mute = m_isMuted;

    /* #21941 #21329 update the volume based on player type */
    if(speakerType == SpeakerInterface::Type::AVS_ALERTS_VOLUME)
        gstVolume = m_AlertlastVolume;
    else
        gstVolume = m_SpeakerlastVolume;


    if (!toAVSVolume->normalize(gstVolume, &avsVolume)) {
        ACSDK_ERROR(LX("handleGetSpeakerSettingsFailed").d("reason", "normalizeVolumeFailed"));
        promise->set_value(false);
        return;
    }

    // AVS Volume will be between 0 and 100.
    settings->volume = avsVolume;
    settings->mute = mute;

    promise->set_value(true);
}

SpeakerInterface::Type MediaPlayer::getSpeakerType() {
    ACSDK_DEBUG9(LX("getSpeakerTypeCalled"));
    return m_speakerType;
}

void MediaPlayer::setAppSrc(GstAppSrc* appSrc) {
    m_pipeline.appsrc = appSrc;
}

GstAppSrc* MediaPlayer::getAppSrc() const {
    return m_pipeline.appsrc;
}

void MediaPlayer::setDecoder(GstElement* decoder) {
    m_pipeline.decoder = decoder;
}

GstElement* MediaPlayer::getDecoder() const {
    return m_pipeline.decoder;
}

GstElement* MediaPlayer::getPipeline() const {
    return m_pipeline.pipeline;
}

MediaPlayer::MediaPlayer(
    std::shared_ptr<avsCommon::sdkInterfaces::HTTPContentFetcherInterfaceFactoryInterface> contentFetcherFactory,
    bool enableEqualizer,
    SpeakerInterface::Type type,
    std::string name) :
        RequiresShutdown{name},
        m_lastVolume{GST_SET_VOLUME_MAX},
        m_isMuted{false},
        m_contentFetcherFactory{contentFetcherFactory},
        m_equalizerEnabled{enableEqualizer},
        m_speakerType{type},
        m_playbackStartedSent{false},
        m_playbackFinishedSent{false},
        m_isPaused{false},
        m_isBufferUnderrun{false},
        m_playerObserver{nullptr},
        m_currentId{ERROR},
        m_playPending{false},
        m_pausePending{false},
        m_resumePending{false},
        m_pauseImmediately{false} {
        
        PlayerName = name;
        
        if(name == "BluetoothMediaPlayer")
            PlayerId = BLUETOOTH_PLAYER;
        else
            PlayerId = 0;    
        IsPlayerActive = false;
        
        /* #21812: #22033: By default the activespeaker type is AVS_MEDIA_NONE */
		activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_NONE;
        
		std::string alertvolume;
		std::string speakervolume;
	
		/* #22115: fetching last alert & speaker volumes from uci */  
		if(!alexaClientSDK::avsCommon::utils::uci::GetparamUCI("shifu.conf", "AVSALERTSVOLUME", alertvolume))
			m_AlertlastVolume = std::stof(alertvolume);
		else
			m_AlertlastVolume = 0.6;
		
		if(!alexaClientSDK::avsCommon::utils::uci::GetparamUCI("shifu.conf", "AVSSPEAKERVOLUME", speakervolume))
			m_SpeakerlastVolume = std::stof(speakervolume);
		else
			m_SpeakerlastVolume = 0.4;
		
		ACSDK_DEBUG8(LX("MediaPlayer").d("AVSALERTSVOLUME", m_AlertlastVolume).d("AVSSPEAKERVOLUME", m_SpeakerlastVolume));

}

void MediaPlayer::workerLoop() {
    g_main_context_push_thread_default(m_workerContext);

    // Add bus watch only after calling g_main_context_push_thread_default.
    GstBus* bus = gst_pipeline_get_bus(GST_PIPELINE(m_pipeline.pipeline));
    m_busWatchId = gst_bus_add_watch(bus, &MediaPlayer::onBusMessage, this);
    gst_object_unref(bus);

    g_main_loop_run(m_mainLoop);

    g_main_context_pop_thread_default(m_workerContext);
}

bool MediaPlayer::init() {
    m_workerContext = g_main_context_new();
    if (!m_workerContext) {
        ACSDK_ERROR(LX("initPlayerFailed").d("reason", "nullWorkerContext"));
        return false;
    }

    if (!(m_mainLoop = g_main_loop_new(m_workerContext, false))) {
        ACSDK_ERROR(LX("initPlayerFailed").d("reason", "gstMainLoopNewFailed"));
        return false;
    };

    if (false == gst_init_check(NULL, NULL, NULL)) {
        ACSDK_ERROR(LX("initPlayerFailed").d("reason", "gstInitCheckFailed"));
        return false;
    }

    if (!setupPipeline()) {
        ACSDK_ERROR(LX("initPlayerFailed").d("reason", "setupPipelineFailed"));
        return false;
    }

    m_mainLoopThread = std::thread(&MediaPlayer::workerLoop, this);

    return true;
}

bool MediaPlayer::setupPipeline() {
    m_pipeline.decodedQueue = gst_element_factory_make("queue2", "decodedQueue");
    if (!m_pipeline.decodedQueue) {
        ACSDK_ERROR(LX("setupPipelineFailed").d("reason", "createQueueElementFailed"));
        return false;
    }
    g_object_set(m_pipeline.decodedQueue, "use-buffering", TRUE, NULL);
    m_pipeline.converter = gst_element_factory_make("audioconvert", "converter");
    if (!m_pipeline.converter) {
        ACSDK_ERROR(LX("setupPipelineFailed").d("reason", "createConverterElementFailed"));
        return false;
    }

    m_pipeline.volume = gst_element_factory_make("volume", "volume");
    if (!m_pipeline.volume) {
        ACSDK_ERROR(LX("setupPipelineFailed").d("reason", "createVolumeElementFailed"));
        return false;
    }

    if (m_equalizerEnabled) {
        m_pipeline.equalizer = gst_element_factory_make("equalizer-3bands", "equalizer");
        if (!m_pipeline.equalizer) {
            ACSDK_ERROR(LX("setupPipelineFailed").d("reason", "createEqualizerElementFailed"));
            return false;
        }
    }

    std::string audioSinkElement;
    ConfigurationNode::getRoot()[MEDIAPLAYER_CONFIGURATION_ROOT_KEY].getString(
        MEDIAPLAYER_AUDIO_SINK_KEY, &audioSinkElement, "autoaudiosink");
    m_pipeline.audioSink = gst_element_factory_make(audioSinkElement.c_str(), "audio_sink");
    if (!m_pipeline.audioSink) {
        ACSDK_ERROR(LX("setupPipelineFailed")
                        .d("reason", "createAudioSinkElementFailed")
                        .d("audioSinkElement", audioSinkElement));
        return false;
    }

    GstCaps* caps = gst_caps_new_empty_simple("audio/x-raw");
    if (!caps) {
        ACSDK_ERROR(LX("setupPipelineFailed").d("reason", "createCapabilityStructFailed"));
        return false;
    }

    m_pipeline.resample = nullptr;
    m_pipeline.caps = nullptr;

    // Check to see if user has specified an output configuration
    auto configurationRoot =
        ConfigurationNode::getRoot()[MEDIAPLAYER_CONFIGURATION_ROOT_KEY][MEDIAPLAYER_OUTPUT_CONVERSION_ROOT_KEY];
    if (configurationRoot) {
        std::string value;

        // Search for output configuration keys
        for (auto& it : MEDIAPLAYER_ACCEPTED_KEYS) {
            if (!configurationRoot.getString(it.first, &value) || value.empty()) {
                continue;
            }

            // Found key, add it to capability struct
            switch (it.second) {
                case G_TYPE_INT:
                    gst_caps_set_simple(caps, it.first.c_str(), it.second, std::stoi(value), NULL);
                    break;
                case G_TYPE_STRING:
                    gst_caps_set_simple(caps, it.first.c_str(), it.second, value.c_str(), NULL);
                    break;
            }
        }

        // Add resample logic if configuration found
        if (!gst_caps_is_empty(caps)) {
            ACSDK_INFO(LX("outputConversion").d("string", gst_caps_to_string(caps)));

            m_pipeline.resample = gst_element_factory_make("audioresample", "resample");
            if (!m_pipeline.resample) {
                ACSDK_ERROR(LX("setupPipelineFailed").d("reason", "createResampleElementFailed"));
                return false;
            }

            m_pipeline.caps = gst_element_factory_make("capsfilter", "caps");
            if (!m_pipeline.caps) {
                ACSDK_ERROR(LX("setupPipelineFailed").d("reason", "createCapabilityElementFailed"));
                return false;
            }

            g_object_set(G_OBJECT(m_pipeline.caps), "caps", caps, NULL);
        } else {
            ACSDK_INFO(LX("invalidOutputConversion").d("string", gst_caps_to_string(caps)));
        }
    } else {
        ACSDK_DEBUG9(LX("noOutputConversion"));
    }

    // clean up caps object
    gst_caps_unref(caps);

    m_pipeline.pipeline = gst_pipeline_new("audio-pipeline");
    if (!m_pipeline.pipeline) {
        ACSDK_ERROR(LX("setupPipelineFailed").d("reason", "createPipelineElementFailed"));
        return false;
    }

    // Link only the queue, converter, volume, and sink here. Src will be linked in respective source files.
    gst_bin_add_many(
        GST_BIN(m_pipeline.pipeline),
        m_pipeline.decodedQueue,
        m_pipeline.converter,
        m_pipeline.volume,
        m_pipeline.audioSink,
        nullptr);

    GstElement* pipelineTailElement = m_pipeline.audioSink;

    if (m_equalizerEnabled) {
        // Add equalizer to a pipeline tail
        gst_bin_add(GST_BIN(m_pipeline.pipeline), m_pipeline.equalizer);
        pipelineTailElement = m_pipeline.equalizer;
        if (!gst_element_link(m_pipeline.equalizer, m_pipeline.audioSink)) {
            ACSDK_ERROR(LX("setupPipelineFailed").d("reason", "failed to linke equalizer to audiosink."));
            return false;
        }
    }

    if (m_pipeline.resample != nullptr && m_pipeline.caps != nullptr) {
        // Add resampler to the pipeline tail
        gst_bin_add_many(GST_BIN(m_pipeline.pipeline), m_pipeline.resample, m_pipeline.caps, nullptr);

        if (!gst_element_link_many(m_pipeline.resample, m_pipeline.caps, pipelineTailElement, nullptr)) {
            ACSDK_ERROR(LX("setupPipelineFailed").d("reason", "Failed to link converter."));
            return false;
        }
        pipelineTailElement = m_pipeline.resample;
    }

    // Complete the pipeline linking
    if (!gst_element_link_many(
            m_pipeline.decodedQueue, m_pipeline.converter, m_pipeline.volume, pipelineTailElement, nullptr)) {
        ACSDK_ERROR(LX("setupPipelineFailed").d("reason", "Failed to link pipeline."));
        return false;
    }

    return true;
}

void MediaPlayer::tearDownTransientPipelineElements(bool notifyStop) {
    ACSDK_DEBUG9(LX("tearDownTransientPipelineElements"));
    m_offsetBeforeTeardown = getCurrentStreamOffset();
    if (notifyStop) {
        sendPlaybackStopped();
    }
    m_currentId = ERROR_SOURCE_ID;
    cleanUpSource();
    m_offsetManager.clear();
    m_playPending = false;
    m_pausePending = false;
    m_resumePending = false;
    m_pauseImmediately = false;
    m_playbackStartedSent = false;
    m_playbackFinishedSent = false;
    m_isPaused = false;
    m_isBufferUnderrun = false;
    if (m_pipeline.audioSink) {
        // Set audioSink's sink option back to TRUE
        g_object_set(m_pipeline.audioSink, "sync", TRUE, NULL);
    }
}

void MediaPlayer::resetPipeline() {
    ACSDK_DEBUG9(LX("resetPipeline"));
    m_pipeline.pipeline = nullptr;
    m_pipeline.appsrc = nullptr;
    m_pipeline.decoder = nullptr;
    m_pipeline.decodedQueue = nullptr;
    m_pipeline.converter = nullptr;
    m_pipeline.volume = nullptr;
    m_pipeline.resample = nullptr;
    m_pipeline.caps = nullptr;
    m_pipeline.equalizer = nullptr;
    m_pipeline.audioSink = nullptr;
}

bool MediaPlayer::queryBufferPercent(gint* percent) {
    ACSDK_DEBUG5(LX("queryBufferPercent"));
    if (!percent) {
        ACSDK_ERROR(LX("queryBufferPercentFailed").d("reason", "nullPercent"));
        return false;
    }
    GstQuery* query;

    query = gst_query_new_buffering(GST_FORMAT_TIME);
    if (gst_element_query(m_pipeline.pipeline, query)) {
        gst_query_parse_buffering_percent(query, NULL, percent);
        gst_query_unref(query);
        return true;
    } else {
        ACSDK_ERROR(LX("queryBufferPercentFailed").d("reason", "bufferingQueryFailed"));
        gst_query_unref(query);
        return true;
    }
}

bool MediaPlayer::queryIsSeekable(bool* isSeekable) {
    GstState curState;
    auto stateChange = gst_element_get_state(m_pipeline.pipeline, &curState, NULL, TIMEOUT_ZERO_NANOSECONDS);
    if (stateChange == GST_STATE_CHANGE_FAILURE) {
        ACSDK_ERROR(LX("queryIsSeekableFailed").d("reason", "gstElementGetStateFailed"));
        return false;
    }
    if (stateChange == GST_STATE_CHANGE_ASYNC) {
        ACSDK_DEBUG(LX("pipelineNotSeekable").d("reason", "stateChangeAsync"));
        return false;
    }
    if ((GST_STATE_PLAYING != curState) && (GST_STATE_PAUSED != curState)) {
        ACSDK_DEBUG(LX("pipelineNotSeekable").d("reason", "notPlayingOrPaused"));
        *isSeekable = false;
        return true;
    }
    // TODO: ACSDK-1778: Investigate why gst_query_parse_seeking() is not working.
    // If it's usable again, use gst_query_parse_seeking() instead of gst_app_src_get_stream_type()

    *isSeekable = (GST_APP_STREAM_TYPE_SEEKABLE == gst_app_src_get_stream_type(m_pipeline.appsrc));
    return true;
}

bool MediaPlayer::seek() {
    bool seekSuccessful = true;
    ACSDK_DEBUG9(LX("seekCalled"));
    if (!m_offsetManager.isSeekable() || !m_offsetManager.isSeekPointSet()) {
        ACSDK_ERROR(LX("seekFailed")
                        .d("reason", "invalidState")
                        .d("isSeekable", m_offsetManager.isSeekable())
                        .d("seekPointSet", m_offsetManager.isSeekPointSet()));
        seekSuccessful = false;
    } else if (!gst_element_seek_simple(
                   m_pipeline.pipeline,
                   GST_FORMAT_TIME,  // ns
                   static_cast<GstSeekFlags>(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT),
                   std::chrono::duration_cast<std::chrono::nanoseconds>(m_offsetManager.getSeekPoint()).count())) {
        ACSDK_ERROR(LX("seekFailed").d("reason", "gstElementSeekSimpleFailed"));
        seekSuccessful = false;
    } else {
        ACSDK_DEBUG(LX("seekSuccessful").d("offsetInMs", m_offsetManager.getSeekPoint().count()));
    }

    m_offsetManager.clear();
    return seekSuccessful;
}

guint MediaPlayer::queueCallback(const std::function<gboolean()>* callback) {
    if (isShutdown()) {
        return UNQUEUED_CALLBACK;
    }
    auto source = g_idle_source_new();
    g_source_set_callback(
        source, reinterpret_cast<GSourceFunc>(&onCallback), const_cast<std::function<gboolean()>*>(callback), nullptr);
    auto sourceId = g_source_attach(source, m_workerContext);
    g_source_unref(source);
    return sourceId;
}

guint MediaPlayer::attachSource(GSource* source) {
    if (source) {
        return g_source_attach(source, m_workerContext);
    }
    return UNQUEUED_CALLBACK;
}

gboolean MediaPlayer::removeSource(guint tag) {
    auto source = g_main_context_find_source_by_id(m_workerContext, tag);
    if (source) {
        g_source_destroy(source);
    }
    return true;
}

void MediaPlayer::onError() {
    ACSDK_DEBUG9(LX("onError"));
    /*
     * Instead of calling the queueCallback, we are calling g_idle_add here directly here because we want this callback
     * to be non-blocking.  To do this, we are creating a static callback function with the this pointer passed in as
     * a parameter.char volume_table_shifu[] = {0,40,44,46,48,52,56,60,64,68,72};
     */
    auto source = g_idle_source_new();
    g_source_set_callback(source, reinterpret_cast<GSourceFunc>(&onErrorCallback), this, nullptr);
    g_source_attach(source, m_workerContext);
    g_source_unref(source);
}

void MediaPlayer::doShutdown() {
    if (m_urlConverter) {
        m_urlConverter->shutdown();
    }
    m_urlConverter.reset();
    m_playerObserver.reset();
}

gboolean MediaPlayer::onCallback(const std::function<gboolean()>* callback) {
    return (*callback)();
}

void MediaPlayer::onPadAdded(GstElement* decoder, GstPad* pad, gpointer pointer) {
    ACSDK_DEBUG9(LX("onPadAddedCalled"));
    auto mediaPlayer = static_cast<MediaPlayer*>(pointer);
    std::promise<void> promise;
    auto future = promise.get_future();
    std::function<gboolean()> callback = [mediaPlayer, &promise, decoder, pad]() {
        mediaPlayer->handlePadAdded(&promise, decoder, pad);
        return false;
    };
    if (mediaPlayer->queueCallback(&callback) != UNQUEUED_CALLBACK) {
        future.wait();
    }
}

void MediaPlayer::handlePadAdded(std::promise<void>* promise, GstElement* decoder, GstPad* pad) {
    ACSDK_DEBUG9(LX("handlePadAddedSignalCalled"));
    gst_element_link(decoder, m_pipeline.decodedQueue);
    promise->set_value();
}

gboolean MediaPlayer::onBusMessage(GstBus* bus, GstMessage* message, gpointer mediaPlayer) {
    return static_cast<MediaPlayer*>(mediaPlayer)->handleBusMessage(message);
}

gboolean MediaPlayer::handleBusMessage(GstMessage* message) {
    ACSDK_DEBUG9(
        LX("messageReceived").d("type", GST_MESSAGE_TYPE_NAME(message)).d("source", GST_MESSAGE_SRC_NAME(message)));
    switch (GST_MESSAGE_TYPE(message)) {
        case GST_MESSAGE_EOS:
            if (GST_MESSAGE_SRC(message) == GST_OBJECT_CAST(m_pipeline.pipeline)) {
                if (!m_source->handleEndOfStream()) {
                    const std::string errorMessage{"reason=sourceHandleEndOfStreamFailed"};
                    ACSDK_ERROR(LX("handleBusMessageFailed").m(errorMessage));
                    sendPlaybackError(ErrorType::MEDIA_ERROR_INTERNAL_DEVICE_ERROR, errorMessage);
                    break;
                }

                // Continue playback if there is additional data.
                if (m_source->hasAdditionalData()) {
                    if (GST_STATE_CHANGE_FAILURE == gst_element_set_state(m_pipeline.pipeline, GST_STATE_NULL)) {
                        const std::string errorMessage{"reason=setPipelineToNullFailed"};
                        ACSDK_ERROR(LX("continuingPlaybackFailed").m(errorMessage));
                        sendPlaybackError(ErrorType::MEDIA_ERROR_INTERNAL_DEVICE_ERROR, errorMessage);
                        break;
                    }

                    if (GST_STATE_CHANGE_FAILURE == gst_element_set_state(m_pipeline.pipeline, GST_STATE_PLAYING)) {
                        const std::string errorMessage{"reason=setPipelineToPlayingFailed"};
                        ACSDK_ERROR(LX("continuingPlaybackFailed").m(errorMessage));
                        sendPlaybackError(ErrorType::MEDIA_ERROR_INTERNAL_DEVICE_ERROR, errorMessage);
                        break;
                    }
                } else {
                    sendPlaybackFinished();
                }
            }
            break;

        case GST_MESSAGE_ERROR: {
            GError* error;
            gchar* debug;
            gst_message_parse_error(message, &error, &debug);

            std::string messageSrcName = GST_MESSAGE_SRC_NAME(message);
            ACSDK_ERROR(LX("handleBusMessageError")
                            .d("source", messageSrcName)
                            .d("error", error->message)
                            .d("debug", debug ? debug : "noInfo"));
            bool isPlaybackRemote = m_source ? m_source->isPlaybackRemote() : false;
            sendPlaybackError(gerrorToErrorType(error, isPlaybackRemote), error->message);
            g_error_free(error);
            g_free(debug);
            break;
        }
        case GST_MESSAGE_STATE_CHANGED: {
            // Check that the state change is for the pipeline.
            if (GST_MESSAGE_SRC(message) == GST_OBJECT_CAST(m_pipeline.pipeline)) {
                GstState oldState;
                GstState newState;
                GstState pendingState;
                gst_message_parse_state_changed(message, &oldState, &newState, &pendingState);
                ACSDK_DEBUG9(LX("State Change")
                                 .d("oldState", gst_element_state_get_name(oldState))
                                 .d("newState", gst_element_state_get_name(newState))
                                 .d("pendingState", gst_element_state_get_name(pendingState)));
                if (GST_STATE_PAUSED == newState) {
                    /*
                     * Pause occurred immediately after a play/resume, so it's possible that the play/resume
                     * was never enacted by MediaPlayer. If there are pending play/resume at the time of the pause,
                     * notify the observers that the calls were still received.
                     */
                    if (m_pauseImmediately) {
                        if (m_playPending) {
                            sendPlaybackStarted();
                        } else if (m_resumePending) {
                            sendPlaybackResumed();
                        }
                        sendPlaybackPaused();
                    } else if (GST_STATE_PLAYING == pendingState) {
                        // GStreamer seeks should be performed when the pipeline is in the PAUSED or PLAYING state only,
                        // so just before we make our first upwards state change from PAUSED to PLAYING, we perform the
                        // seek.
                        if (m_offsetManager.isSeekable() && m_offsetManager.isSeekPointSet()) {
                            if (!seek()) {
                                std::string error = "seekFailed";
                                ACSDK_ERROR(LX(error));
                                sendPlaybackError(ErrorType::MEDIA_ERROR_INTERNAL_DEVICE_ERROR, error);
                            };
                        }
                    } else if (GST_STATE_PLAYING == oldState && GST_STATE_VOID_PENDING == pendingState) {
                        // State change from PLAYING -> PAUSED.
                        if (m_isBufferUnderrun) {
                            sendBufferUnderrun();
                        } else if (!m_isPaused) {
                            sendPlaybackPaused();
                        }
                    }
                } else if (newState == GST_STATE_PLAYING) {
                    if (!m_playbackStartedSent) {
                        sendPlaybackStarted();
                    } else {
                        if (m_isBufferUnderrun) {
                            sendBufferRefilled();
                            m_isBufferUnderrun = false;
                        }
                        if (m_isPaused) {
                            sendPlaybackResumed();
                        }
                    }
                } else if (newState == GST_STATE_NULL && oldState == GST_STATE_READY) {
                    sendPlaybackStopped();
                }
            } else if (g_str_has_prefix(GST_MESSAGE_SRC_NAME(message), "tsdemux")) {
                /*
                 * tsdemux element can be used to determine if the music sources are MPEG-TS.
                 */
                GstState oldState;
                GstState newState;
                GstState pendingState;
                gst_message_parse_state_changed(message, &oldState, &newState, &pendingState);
                ACSDK_DEBUG5(LX("tsdemux State Change")
                                 .d("oldState", gst_element_state_get_name(oldState))
                                 .d("newState", gst_element_state_get_name(newState))
                                 .d("pendingState", gst_element_state_get_name(pendingState)));

                if (GST_STATE_READY == newState) {
                    /*
                     * Certain music sources, specifically Audible, were unable to play properly. With Audible, frames
                     * were getting dropped and the audio would play very choppily. For example, in a 10 second chunk,
                     * seconds 1-5 would play followed immediately by seconds 6.5-7.5, followed by 8.5-10. Setting this
                     * property to false prevents the sink from dropping frames because they arrive too late.
                     * TODO: (ACSDK-828) Investigate why frames are arriving late to the sink causing MPEG-TS files to
                     * play choppily
                     */
                    ACSDK_DEBUG5(LX("audioSink").m("Sync option set to false."));
                    g_object_set(m_pipeline.audioSink, "sync", FALSE, NULL);
                } else if (GST_STATE_NULL == newState) {
                    // Reset sync state back to true if tsdemux changes to NULL state
                    ACSDK_DEBUG5(LX("audioSink").m("Sync option set to true."));
                    g_object_set(m_pipeline.audioSink, "sync", TRUE, NULL);
                }
            }
            break;
        }

        case GST_MESSAGE_BUFFERING: {
            gint bufferPercent = 0;
            gst_message_parse_buffering(message, &bufferPercent);
            ACSDK_DEBUG9(LX("handleBusMessage").d("message", "GST_MESSAGE_BUFFERING").d("percent", bufferPercent));
            
            /* Redmine 21137: Start
            * Buffer will Start playing the Stream when 5% of the Buffer is Filled
            * If Buffer is going Below 5%, Underrun process will initiated to Fill the Buffer
            * We are disabling the Pause operation, which is creating the Glitches while underrun
            */
            if((PlayerId == BLUETOOTH_PLAYER) && (bufferPercent < 5)) {
                // if (GST_STATE_CHANGE_FAILURE == gst_element_set_state(m_pipeline.pipeline, GST_STATE_PAUSED)) {
                //     std::string error = "pausingOnBufferUnderrunFailed";
                //     ACSDK_ERROR(LX(error));
                //     sendPlaybackError(ErrorType::MEDIA_ERROR_INTERNAL_DEVICE_ERROR, error);
                //     break;
                // }
                // // Only enter bufferUnderrun after playback has started.
                // if (m_playbackStartedSent) {
                //     ACSDK_DEBUG9(LX("handleBusMessage").d("message", "bufferUnderrun"));
                m_isBufferUnderrun = true;
                // }
            }
            // For Non BluetoothMediaPlayer, Player will start Playing once the buffer is completely filled
            else if((PlayerId != BLUETOOTH_PLAYER) && (bufferPercent < 100)) {
                if (GST_STATE_CHANGE_FAILURE == gst_element_set_state(m_pipeline.pipeline, GST_STATE_PAUSED)) {
                    std::string error = "pausingOnBufferUnderrunFailed";
                    ACSDK_ERROR(LX(error));
                    sendPlaybackError(ErrorType::MEDIA_ERROR_INTERNAL_DEVICE_ERROR, error);
                    break;
                }
                // Only enter bufferUnderrun after playback has started.
                if (m_playbackStartedSent) {
                    ACSDK_DEBUG9(LX("handleBusMessage").d("message", "bufferUnderrun"));
                    m_isBufferUnderrun = true;
                }
            }
            // Redmine 21137: End
            else {
                if (m_pauseImmediately) {
                    // To avoid starting to play if a pause() was called immediately after calling a play()
                    break;
                }
                bool isSeekable = false;
                if (queryIsSeekable(&isSeekable)) {
                    m_offsetManager.setIsSeekable(isSeekable);
                }

                ACSDK_DEBUG9(LX("offsetState")
                                 .d("isSeekable", m_offsetManager.isSeekable())
                                 .d("isSeekPointSet", m_offsetManager.isSeekPointSet()));

                if (m_offsetManager.isSeekable() && m_offsetManager.isSeekPointSet()) {
                    seek();
                } else if (GST_STATE_CHANGE_FAILURE == gst_element_set_state(m_pipeline.pipeline, GST_STATE_PLAYING)) {
                    std::string error = "resumingOnBufferRefilledFailed";
                    ACSDK_ERROR(LX(error));
                    sendPlaybackError(ErrorType::MEDIA_ERROR_INTERNAL_DEVICE_ERROR, error);
                }
            }
            break;
        }
        case GST_MESSAGE_TAG: {
            auto vectorOfTags = collectTags(message);
            sendStreamTagsToObserver(std::move(vectorOfTags));
            break;
        }
        default:
            break;
    }
    return true;
}

std::unique_ptr<const VectorOfTags> MediaPlayer::collectTags(GstMessage* message) {
    VectorOfTags vectorOfTags;
    GstTagList* tags = NULL;
    gst_message_parse_tag(message, &tags);
    int num_of_tags = gst_tag_list_n_tags(tags);
    if (!num_of_tags) {
        gst_tag_list_unref(tags);
        return nullptr;
    }
    gst_tag_list_foreach(tags, &collectOneTag, &vectorOfTags);
    gst_tag_list_unref(tags);
    return make_unique<const VectorOfTags>(vectorOfTags);
}

void MediaPlayer::sendStreamTagsToObserver(std::unique_ptr<const VectorOfTags> vectorOfTags) {
    ACSDK_DEBUG(LX("callingOnTags"));
    if (m_playerObserver) {
        m_playerObserver->onTags(m_currentId, std::move(vectorOfTags));
    }
}

void MediaPlayer::handleSetAttachmentReaderSource(
    std::shared_ptr<AttachmentReader> reader,
    std::promise<MediaPlayer::SourceId>* promise,
    const avsCommon::utils::AudioFormat* audioFormat,
    bool repeat) {
    ACSDK_DEBUG(LX("handleSetAttachmentReaderSourceCalled"));

    tearDownTransientPipelineElements(true);

    std::shared_ptr<SourceInterface> source = AttachmentReaderSource::create(this, reader, audioFormat, repeat);

    if (!source) {
        ACSDK_ERROR(LX("handleSetAttachmentReaderSourceFailed").d("reason", "sourceIsNullptr"));
        promise->set_value(ERROR_SOURCE_ID);
        return;
    }

    /*
     * Once the source pad for the decoder has been added, the decoder emits the pad-added signal. Connect the signal
     * to the callback which performs the linking of the decoder source pad to decodedQueue sink pad.
     */
    if (!g_signal_connect(m_pipeline.decoder, "pad-added", G_CALLBACK(onPadAdded), this)) {
        ACSDK_ERROR(LX("handleSetAttachmentReaderSourceFailed").d("reason", "connectPadAddedSignalFailed"));
        promise->set_value(ERROR_SOURCE_ID);
        return;
    }

    m_source = source;
    m_currentId = ++g_id;
    m_offsetManager.setIsSeekable(true);
    promise->set_value(m_currentId);
}

void MediaPlayer::handleSetIStreamSource(
    std::shared_ptr<std::istream> stream,
    bool repeat,
    std::promise<MediaPlayer::SourceId>* promise) {
    ACSDK_DEBUG(LX("handleSetSourceCalled"));

    tearDownTransientPipelineElements(true);

    std::shared_ptr<SourceInterface> source = IStreamSource::create(this, stream, repeat);

    if (!source) {
        ACSDK_ERROR(LX("handleSetIStreamSourceFailed").d("reason", "sourceIsNullptr"));
        promise->set_value(ERROR_SOURCE_ID);
        return;
    }

    /*
     * Once the source pad for the decoder has been added, the decoder emits the pad-added signal. Connect the signal
     * to the callback which performs the linking of the decoder source pad to the decodedQueue sink pad.
     */
    if (!g_signal_connect(m_pipeline.decoder, "pad-added", G_CALLBACK(onPadAdded), this)) {
        ACSDK_ERROR(LX("handleSetIStreamSourceFailed").d("reason", "connectPadAddedSignalFailed"));
        promise->set_value(ERROR_SOURCE_ID);
        return;
    }

    m_source = source;
    m_currentId = ++g_id;
    promise->set_value(m_currentId);
}

void MediaPlayer::handleSetUrlSource(
    const std::string& url,
    std::chrono::milliseconds offset,
    std::promise<SourceId>* promise,
    bool repeat) {
    ACSDK_DEBUG(LX("handleSetSourceForUrlCalled"));

    tearDownTransientPipelineElements(true);

    m_urlConverter = alexaClientSDK::playlistParser::UrlContentToAttachmentConverter::create(
        m_contentFetcherFactory, url, shared_from_this(), offset);
    if (!m_urlConverter) {
        ACSDK_ERROR(LX("setSourceUrlFailed").d("reason", "badUrlConverter"));
        promise->set_value(ERROR_SOURCE_ID);
        return;
    }
    auto attachment = m_urlConverter->getAttachment();
    if (!attachment) {
        ACSDK_ERROR(LX("setSourceUrlFailed").d("reason", "badAttachmentReceived"));
        promise->set_value(ERROR_SOURCE_ID);
        return;
    }
    std::shared_ptr<avsCommon::avs::attachment::AttachmentReader> reader =
        attachment->createReader(sds::ReaderPolicy::NONBLOCKING);
    if (!reader) {
        ACSDK_ERROR(LX("setSourceUrlFailed").d("reason", "failedToCreateAttachmentReader"));
        promise->set_value(ERROR_SOURCE_ID);
        return;
    }
    handleSetAttachmentReaderSource(reader, promise, nullptr, repeat);
}

void MediaPlayer::handlePlay(SourceId id, std::promise<bool>* promise) {
    ACSDK_DEBUG(LX("handlePlayCalled").d("idPassed", id).d("currentId", (m_currentId)));
    if (!validateSourceAndId(id)) {
        ACSDK_ERROR(LX("handlePlayFailed"));
        promise->set_value(false);
        return;
    }

    GstState curState;
    auto stateChange = gst_element_get_state(m_pipeline.pipeline, &curState, NULL, TIMEOUT_ZERO_NANOSECONDS);
    if (stateChange == GST_STATE_CHANGE_FAILURE) {
        ACSDK_ERROR(LX("handlePlayFailed").d("reason", "gstElementGetStateFailed"));
        promise->set_value(false);
        return;
    }
    if (curState == GST_STATE_PLAYING) {
        ACSDK_DEBUG(LX("handlePlayFailed").d("reason", "alreadyPlaying"));
        promise->set_value(false);
        return;
    }
    if (m_playPending) {
        ACSDK_DEBUG(LX("handlePlayFailed").d("reason", "playCurrentlyPending"));
        promise->set_value(false);
        return;
    }

    m_playbackFinishedSent = false;
    m_playbackStartedSent = false;
    m_playPending = true;
    m_pauseImmediately = false;
    promise->set_value(true);

    /*
     * If the pipeline is completely buffered, then go straight to PLAY otherwise,
     * set pipeline to PAUSED state to attempt buffering.  The pipeline will be set to PLAY upon receiving buffer
     * percent = 100.
     */
    GstState startingState = GST_STATE_PAUSED;
    gint percent = 0;
    if ((GST_STATE_PAUSED == curState) && (queryBufferPercent(&percent))) {
        // Redmine 21137: Start: If any play is initiated, Play can be initiated at 5% of the buffering
        if((PlayerId == BLUETOOTH_PLAYER) && (5 <= percent)) {
            startingState = GST_STATE_PLAYING;
        }
        else if((PlayerId != BLUETOOTH_PLAYER) && (100 == percent)) {
        // Redmine 21137: End
            startingState = GST_STATE_PLAYING;
        }
    }

    stateChange = gst_element_set_state(m_pipeline.pipeline, startingState);
    ACSDK_DEBUG(LX("handlePlay")
                    .d("startingState", gst_element_state_get_name(startingState))
                    .d("stateReturn", gst_element_state_change_return_get_name(stateChange)));

    switch (stateChange) {
        case GST_STATE_CHANGE_FAILURE: {
            const std::string errorMessage{"reason=gstElementSetStateFailure"};
            ACSDK_ERROR(LX("handlePlayFailed").m(errorMessage));
            sendPlaybackError(ErrorType::MEDIA_ERROR_INTERNAL_DEVICE_ERROR, errorMessage);
        }
            return;
        default:
            if (m_urlConverter) {
                if (m_urlConverter->getDesiredStreamingPoint() == std::chrono::milliseconds::zero()) {
                    return;
                }
                m_offsetManager.setSeekPoint(
                    m_urlConverter->getDesiredStreamingPoint() - m_urlConverter->getStartStreamingPoint());
            }
            // Allow sending callbacks to be handled on the bus message
            return;
    }
}

void MediaPlayer::handleStop(MediaPlayer::SourceId id, std::promise<bool>* promise) {
    ACSDK_DEBUG(LX("handleStopCalled").d("idPassed", id).d("currentId", (m_currentId)));
    if (!validateSourceAndId(id)) {
        ACSDK_ERROR(LX("handleStopFailed"));
        promise->set_value(false);
        return;
    }

    GstState curState;
    GstState pending;
    auto stateChangeRet = gst_element_get_state(m_pipeline.pipeline, &curState, &pending, TIMEOUT_ZERO_NANOSECONDS);
    if (GST_STATE_CHANGE_FAILURE == stateChangeRet) {
        ACSDK_ERROR(LX("handleStopFailed").d("reason", "gstElementGetStateFailure"));
        promise->set_value(false);
        return;
    }

    // Only stop if currently not stopped.
    if (curState == GST_STATE_NULL) {
        ACSDK_ERROR(LX("handleStopFailed").d("reason", "alreadyStopped"));
        promise->set_value(false);
        return;
    }

    if (pending == GST_STATE_NULL) {
        ACSDK_ERROR(LX("handleStopFailed").d("reason", "alreadyStopping"));
        promise->set_value(false);
        return;
    }

    stateChangeRet = gst_element_set_state(m_pipeline.pipeline, GST_STATE_NULL);
    if (GST_STATE_CHANGE_FAILURE == stateChangeRet) {
        ACSDK_ERROR(LX("handleStopFailed").d("reason", "gstElementSetStateFailure"));
        promise->set_value(false);
    } else {
        /*
         * Based on GStreamer docs, a gst_element_set_state call to change the state to GST_STATE_NULL will never
         * return GST_STATE_CHANGE_ASYNC.
         */
        promise->set_value(true);
        if (m_playPending) {
            sendPlaybackStarted();
        } else if (m_resumePending) {
            sendPlaybackResumed();
        }
        sendPlaybackStopped();
    }
}

void MediaPlayer::handlePause(MediaPlayer::SourceId id, std::promise<bool>* promise) {
    ACSDK_DEBUG(LX("handlePauseCalled").d("idPassed", id).d("currentId", (m_currentId)));
    if (!validateSourceAndId(id)) {
        ACSDK_ERROR(LX("handlePauseFailed"));
        promise->set_value(false);
        return;
    }

    GstState curState;
    auto stateChangeRet = gst_element_get_state(m_pipeline.pipeline, &curState, NULL, TIMEOUT_ZERO_NANOSECONDS);
    if (GST_STATE_CHANGE_FAILURE == stateChangeRet) {
        ACSDK_ERROR(LX("handlePauseFailed").d("reason", "gstElementGetStateFailure"));
        promise->set_value(false);
        return;
    }

    /*
     * If a play() or resume() call is pending, we want to try pausing immediately to avoid blips in audio.
     */
    if (m_playPending || m_resumePending) {
        ACSDK_DEBUG9(LX("handlePauseCalled").d("info", "playOrResumePending"));
        if (m_pausePending) {
            ACSDK_DEBUG(LX("handlePauseFailed").d("reason", "pauseCurrentlyPending"));
            promise->set_value(false);
            return;
        }
        stateChangeRet = gst_element_set_state(m_pipeline.pipeline, GST_STATE_PAUSED);
        if (GST_STATE_CHANGE_FAILURE == stateChangeRet) {
            ACSDK_ERROR(LX("handlePauseFailed").d("reason", "gstElementSetStateFailure"));
            promise->set_value(false);
        } else {
            m_pauseImmediately = true;
            ACSDK_DEBUG(LX("handlePause").d("pauseImmediately", "true"));
            promise->set_value(true);
        }
        return;
    }

    if (curState != GST_STATE_PLAYING) {
        ACSDK_ERROR(LX("handlePauseFailed").d("reason", "noAudioPlaying"));
        promise->set_value(false);
        return;
    }
    if (m_pausePending) {
        ACSDK_DEBUG(LX("handlePauseFailed").d("reason", "pauseCurrentlyPending"));
        promise->set_value(false);
        return;
    }

    stateChangeRet = gst_element_set_state(m_pipeline.pipeline, GST_STATE_PAUSED);
    if (GST_STATE_CHANGE_FAILURE == stateChangeRet) {
        ACSDK_ERROR(LX("handlePauseFailed").d("reason", "gstElementSetStateFailure"));
        promise->set_value(false);
    } else {
        m_pausePending = true;
        promise->set_value(true);
    }
}

void MediaPlayer::handleResume(MediaPlayer::SourceId id, std::promise<bool>* promise) {
    ACSDK_DEBUG(LX("handleResumeCalled").d("idPassed", id).d("currentId", (m_currentId)));
    if (!validateSourceAndId(id)) {
        ACSDK_ERROR(LX("handleResumeFailed"));
        promise->set_value(false);
        return;
    }

    GstState curState;
    auto stateChangeRet = gst_element_get_state(m_pipeline.pipeline, &curState, NULL, TIMEOUT_ZERO_NANOSECONDS);

    if (GST_STATE_CHANGE_FAILURE == stateChangeRet) {
        ACSDK_ERROR(LX("handleResumeFailed").d("reason", "gstElementGetStateFailure"));
        promise->set_value(false);
        return;
    }

    if (GST_STATE_PLAYING == curState) {
        ACSDK_ERROR(LX("handleResumeFailed").d("reason", "alreadyPlaying"));
        promise->set_value(false);
        return;
    }

    // Only unpause if currently paused.
    if (GST_STATE_PAUSED != curState) {
        ACSDK_ERROR(LX("handleResumeFailed").d("reason", "notCurrentlyPaused"));
        promise->set_value(false);
        return;
    }

    if (m_resumePending) {
        ACSDK_DEBUG(LX("handleResumeFailed").d("reason", "resumeCurrentlyPending"));
        promise->set_value(false);
        return;
    }

    // TODO: ACSDK-1778: Verify if we need to check percent buffer here and decide whether to continue pausing to refill
    // buffer or go straight to play
    stateChangeRet = gst_element_set_state(m_pipeline.pipeline, GST_STATE_PLAYING);
    if (GST_STATE_CHANGE_FAILURE == stateChangeRet) {
        ACSDK_ERROR(LX("handleResumeFailed").d("reason", "gstElementSetStateFailure"));
        promise->set_value(false);
    } else {
        m_resumePending = true;
        m_pauseImmediately = false;
        promise->set_value(true);
    }
}

void MediaPlayer::handleGetOffset(SourceId id, std::promise<std::chrono::milliseconds>* promise) {
    ACSDK_DEBUG9(LX("handleGetOffsetCalled").d("idPassed", id).d("currentId", (m_currentId)));

    // Check if pipeline is set.
    if (!m_pipeline.pipeline) {
        ACSDK_INFO(LX("handleGetOffsetStopped").m("pipelineNotSet"));
        promise->set_value(MEDIA_PLAYER_INVALID_OFFSET);
        return;
    }

    if (!validateSourceAndId(id)) {
        promise->set_value(m_offsetBeforeTeardown);
        return;
    }

    promise->set_value(getCurrentStreamOffset());
}

std::chrono::milliseconds MediaPlayer::getCurrentStreamOffset() {
    ACSDK_DEBUG9(LX("getCurrentStreamOffsetCalled"));

    auto offsetInMilliseconds = MEDIA_PLAYER_INVALID_OFFSET;
    gint64 position = -1;
    GstState state = GST_STATE_NULL;
    auto stateChangeRet = gst_element_get_state(m_pipeline.pipeline, &state, NULL, TIMEOUT_ZERO_NANOSECONDS);

    if (GST_STATE_CHANGE_FAILURE == stateChangeRet) {
        // Getting the state failed.
        ACSDK_ERROR(LX("getCurrentStreamOffsetFailed").d("reason", "getElementGetStateFailure"));
    } else if (GST_STATE_CHANGE_SUCCESS != stateChangeRet) {
        // Getting the state was not successful (GST_STATE_CHANGE_ASYNC or GST_STATE_CHANGE_NO_PREROLL).
        ACSDK_WARN(LX("getCurrentStreamOffsetError")
                       .d("reason", "getElementGetStateUnsuccessful")
                       .d("stateChangeReturn", gst_element_state_change_return_get_name(stateChangeRet)));
    } else if (GST_STATE_PAUSED != state && GST_STATE_PLAYING != state) {
        // Invalid State.
        ACSDK_DEBUG9(LX("getCurrentStreamOffsetInvalid")
                         .d("reason", "invalidPipelineState")
                         .d("state", gst_element_state_get_name(state))
                         .d("expectedStates", "PAUSED/PLAYING"));
    } else if (!gst_element_query_position(m_pipeline.pipeline, GST_FORMAT_TIME, &position)) {
        // Query Failed.
        ACSDK_ERROR(LX("getCurrentStreamOffsetFailed").d("reason", "gstElementQueryPositionError"));
    } else {
        // Query succeeded.
        std::chrono::milliseconds startStreamingPoint = std::chrono::milliseconds::zero();
        if (m_urlConverter) {
            startStreamingPoint = m_urlConverter->getStartStreamingPoint();
        }
        offsetInMilliseconds =
            (startStreamingPoint +
             std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::nanoseconds(position)));

        ACSDK_DEBUG9(LX("getCurrentStreamOffset").d("offsetInMilliseconds", offsetInMilliseconds.count()));
    }

    return offsetInMilliseconds;
}

void MediaPlayer::handleSetObserver(
    std::promise<void>* promise,
    std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerObserverInterface> observer) {
    ACSDK_DEBUG(LX("handleSetObserverCalled"));
    m_playerObserver = observer;
    promise->set_value();
}

void MediaPlayer::sendPlaybackStarted() {
    if (!m_playbackStartedSent) {
        ACSDK_DEBUG(LX("callingOnPlaybackStarted").d("currentId", m_currentId));
        m_playbackStartedSent = true;
        m_playPending = false;
        if (m_playerObserver) {
            m_playerObserver->onPlaybackStarted(m_currentId);
        }
    }
}

void MediaPlayer::sendPlaybackFinished() {
    if (m_currentId == ERROR_SOURCE_ID) {
        return;
    }
    
    ACSDK_DEBUG(LX("callingOnsendPlaybackFinished").d("PlayerName", PlayerName));
    
    /* #21329
    * make player state to false(not playing) here except for music player.
    * Every player call stop function only when user stops.
    * But this function will be executed for every play back finished.
    * Hence we are changing the player state here.
    * But for music player, This function called for every time when music stream(some duration) completes.
    * Hence we are not changing the music player status here.
    */
    if(PlayerName != "AudioMediaPlayer")
        IsPlayerActive  = false;    
    
	/* #22033: #21812:
		Activespeaker type will be set based on the playback player name.
		activeSpeaker type will be AVS_MEDIA_ALL, if bothe players(alerts and speaker) are active
		This will be used to set the player volume when both alerts player and speaker player are cative.
	*/    
    if(PlayerName == "AlertsMediaPlayer")
		if(activeSpeakerType == SpeakerInterface::Activemedia::AVS_MEDIA_ALL)
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_SPEAKER;
		else
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_NONE;
	else
		if(activeSpeakerType == SpeakerInterface::Activemedia::AVS_MEDIA_ALL)
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_ALERTS;
		else
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_NONE;    
   
    m_isPaused = false;
    m_playbackStartedSent = false;
    if (!m_playbackFinishedSent) {
        m_playbackFinishedSent = true;
        ACSDK_DEBUG(LX("callingOnPlaybackFinished").d("currentId", m_currentId));
        if (m_playerObserver) {
            m_playerObserver->onPlaybackFinished(m_currentId);
        }
    }

    tearDownTransientPipelineElements(false);
    if (m_urlConverter) {
        m_urlConverter->shutdown();
    }
    m_urlConverter.reset();
}

void MediaPlayer::sendPlaybackPaused() {
    ACSDK_DEBUG(LX("callingOnPlaybackPaused").d("currentId", m_currentId));
    m_pausePending = false;
    m_isPaused = true;
    if (m_playerObserver) {
        m_playerObserver->onPlaybackPaused(m_currentId);
    }
}

void MediaPlayer::sendPlaybackResumed() {
    ACSDK_DEBUG(LX("callingOnPlaybackResumed").d("currentId", m_currentId));
    m_resumePending = false;
    m_isPaused = false;
    if (m_playerObserver) {
        m_playerObserver->onPlaybackResumed(m_currentId);
    }
}

void MediaPlayer::sendPlaybackStopped() {
    if (m_currentId == ERROR_SOURCE_ID) {
        return;
    }
    
    ACSDK_DEBUG(LX("callingOnsendPlaybackFinished").d("PlayerName", PlayerName));
    
    /* #21329
    * make player state to false(not playing) here except for music player.
    * Every player call stop function only when user stops.
    * But this function will be executed for every play back finished.
    * Hence we are changing the player state here.
    * But for music player, This function called for every time when music stream(some duration) completes.
    * Hence we are not changing the music player status here.
    */    
    if(PlayerName != "AudioMediaPlayer")
        IsPlayerActive  = false;    
    
	/* #22033: #21812:
		Activespeaker type will be set based on the playback player name.
		activeSpeaker type will be AVS_MEDIA_ALL, if bothe players(alerts and speaker) are active
		This will be used to set the player volume when both alerts player and speaker player are cative.
	*/   
    if(PlayerName == "AlertsMediaPlayer")
		if(activeSpeakerType == SpeakerInterface::Activemedia::AVS_MEDIA_ALL)
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_SPEAKER;
		else
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_NONE;
	else
		if(activeSpeakerType == SpeakerInterface::Activemedia::AVS_MEDIA_ALL)
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_ALERTS;
		else
			activeSpeakerType = SpeakerInterface::Activemedia::AVS_MEDIA_NONE;
    
    ACSDK_DEBUG(LX("callingOnPlaybackStopped").d("currentId", m_currentId));
    if (m_playerObserver && ERROR_SOURCE_ID != m_currentId) {
        m_playerObserver->onPlaybackStopped(m_currentId);
    }

    tearDownTransientPipelineElements(false);
    if (m_urlConverter) {
        m_urlConverter->shutdown();
    }
    m_urlConverter.reset();
}

void MediaPlayer::sendPlaybackError(const ErrorType& type, const std::string& error) {
    if (m_currentId == ERROR_SOURCE_ID) {
        return;
    }
    ACSDK_DEBUG(LX("callingOnPlaybackError").d("type", type).d("error", error).d("currentId", m_currentId));
    m_playPending = false;
    m_pausePending = false;
    m_resumePending = false;
    m_pauseImmediately = false;
    if (m_playerObserver) {
        m_playerObserver->onPlaybackError(m_currentId, type, error);
    }

    tearDownTransientPipelineElements(false);
    if (m_urlConverter) {
        m_urlConverter->shutdown();
    }
    m_urlConverter.reset();
}

void MediaPlayer::sendBufferUnderrun() {
    ACSDK_DEBUG(LX("callingOnBufferUnderrun").d("currentId", m_currentId));
    if (m_playerObserver) {
        m_playerObserver->onBufferUnderrun(m_currentId);
    }
}

void MediaPlayer::sendBufferRefilled() {
    ACSDK_DEBUG(LX("callingOnBufferRefilled").d("currentId", m_currentId));
    if (m_playerObserver) {
        m_playerObserver->onBufferRefilled(m_currentId);
    }
}

bool MediaPlayer::validateSourceAndId(SourceId id) {
    if (!m_source) {
        ACSDK_ERROR(LX("validateSourceAndIdFailed").d("reason", "sourceNotSet"));
        return false;
    }
    if (id != m_currentId) {
        ACSDK_ERROR(LX("validateSourceAndIdFailed").d("reason", "sourceIdMismatch"));
        return false;
    }
    return true;
}

gboolean MediaPlayer::onErrorCallback(gpointer pointer) {
    ACSDK_DEBUG9(LX("onErrorCallback"));
    auto mediaPlayer = static_cast<MediaPlayer*>(pointer);
    mediaPlayer->sendPlaybackError(ErrorType::MEDIA_ERROR_INTERNAL_DEVICE_ERROR, "streamingError");
    return false;
}

void MediaPlayer::cleanUpSource() {
    if (m_pipeline.pipeline) {
        gst_element_set_state(m_pipeline.pipeline, GST_STATE_NULL);
    }
    if (m_source) {
        m_source->shutdown();
    }
    m_source.reset();
}

int MediaPlayer::clampEqualizerLevel(int level) {
    return std::min(std::max(level, MIN_EQUALIZER_LEVEL), MAX_EQUALIZER_LEVEL);
}

void MediaPlayer::setEqualizerBandLevels(audio::EqualizerBandLevelMap bandLevelMap) {
    if (!m_equalizerEnabled) {
        return;
    }
    std::promise<void> promise;
    auto future = promise.get_future();
    std::function<gboolean()> callback = [this, &promise, bandLevelMap]() {
        auto it = bandLevelMap.find(audio::EqualizerBand::BASS);
        if (bandLevelMap.end() != it) {
            g_object_set(
                G_OBJECT(m_pipeline.equalizer),
                GSTREAMER_BASS_BAND_NAME,
                static_cast<gdouble>(clampEqualizerLevel(it->second)),
                NULL);
        }
        it = bandLevelMap.find(audio::EqualizerBand::MIDRANGE);
        if (bandLevelMap.end() != it) {
            g_object_set(
                G_OBJECT(m_pipeline.equalizer),
                GSTREAMER_MIDRANGE_BAND_NAME,
                static_cast<gdouble>(clampEqualizerLevel(it->second)),
                NULL);
        }
        it = bandLevelMap.find(audio::EqualizerBand::TREBLE);
        if (bandLevelMap.end() != it) {
            g_object_set(
                G_OBJECT(m_pipeline.equalizer),
                GSTREAMER_TREBLE_BAND_NAME,
                static_cast<gdouble>(clampEqualizerLevel(it->second)),
                NULL);
        }
        promise.set_value();
        return false;
    };
    if (queueCallback(&callback) != UNQUEUED_CALLBACK) {
        future.get();
    }
}

int MediaPlayer::getMinimumBandLevel() {
    return MIN_EQUALIZER_LEVEL;
}

int MediaPlayer::getMaximumBandLevel() {
    return MAX_EQUALIZER_LEVEL;
}

void MediaPlayer::SetPlayerVolume() {
    /* #21329, #21219
    * This definition is changed to meet the amazon echo behaviour for volume
    * The volume is different for different players as per alexa.
    * Alexa using gstreamer(stream) for volume control.
    * As per smartrons implementation we are going with hardware volume. It means using same volume interface for all the players.
    * Hence we need to set the volume for every player before starting of playback, to meet alexa behaviour.
    * get the speaker type.
    * check weather player is active or not. If the player is already active(playing something), do not distrub the volume.
    * If player is not in active state(not playing), set the previous volume of corresponding player before starting playback
    * if the player is muted, make it unmute(also trigger led), before starting the playback
    * If the volume is zero then make it level 1(also trigger led), before starting playback
    */
    ACSDK_DEBUG8(LX("SetPlayerVolume called").d("IsPlayerActive", IsPlayerActive));
    
    int volume;
    char amixer_cmd[32] = {0};
    // Redmine 22027: Start: Assiging the sink volume variable with '0'
    char pavolume_cmd[64] = {0};
    // Redmine 22027: End
    gboolean mute;
    int volume_led;
    avsCommon::sdkInterfaces::SpeakerInterface::Type speakerType;    
    
    mute = m_isMuted;
    speakerType = getSpeakerType();
	/* #22076 #22033: Set the player volume based on the existing playbacks status.
	* set the volume only when single media player is in active
	* Will not distrub the volume if both media players are active
	*/
	if(speakerType == SpeakerInterface::Type::AVS_ALERTS_VOLUME) 
		if((activeSpeakerType != SpeakerInterface::Activemedia::AVS_MEDIA_ALERTS)
			&& (activeSpeakerType != SpeakerInterface::Activemedia::AVS_MEDIA_NONE))
			return;

	if(!IsPlayerActive)
	{
	    ACSDK_DEBUG8(LX("SetPlayerVolume called").d("speakerType", speakerType));
	    
	    if(!mute)
	    {   
	    	/* #21935: Volume scenario should be identical to ECHO plus 2nd gen
	    	* When volume was set to zero through hardware buttons *
	    	*/            
	        if(speakerType == SpeakerInterface::Type::AVS_ALERTS_VOLUME)
	        {
	        	volume = get_volume_to_float_index(m_AlertlastVolume);            
	            snprintf(amixer_cmd, sizeof(amixer_cmd), "amixer -c 0 sset 'DAC1' %d", volume);
	            system(amixer_cmd);
	            ACSDK_DEBUG8(LX("SetPlayerVolume, mute is false").d("m_AlertlastVolume", m_AlertlastVolume));
	           
	            //#22027: Setting the bt sink volume before starting the playback
				if(shifu_bt_device_connected == SHIFU_SINK)
				{
					volume_led = get_volume_led(volume);
					bt_sync_alsa_volume = (uint8_t)volume_led;
					snprintf(pavolume_cmd, sizeof(pavolume_cmd), "pactl set-sink-volume bluez_sink %d%%", pa_volume_table_shifu[bt_sync_alsa_volume]);
					system(pavolume_cmd);
				}
	        }
	        else
	        {               
	            if(m_SpeakerlastVolume == 0)
	            {
	                m_SpeakerlastVolume = HW_VOLUME_ZERO;
	                volume = get_volume_to_float_index(m_SpeakerlastVolume);
					snprintf(amixer_cmd, sizeof(amixer_cmd), "amixer -c 0 sset 'DAC1' %d", volume);
					system(amixer_cmd);             
					volume_led = get_volume_led(volume);
					alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_VOLUME, 1, volume_led);
					
					//#22027: Setting the bt sink volume before starting the playback
					if(shifu_bt_device_connected == SHIFU_SINK)
					{
						bt_sync_alsa_volume = (uint8_t)volume_led;
						snprintf(pavolume_cmd, sizeof(pavolume_cmd), "pactl set-sink-volume bluez_sink %d%%", pa_volume_table_shifu[bt_sync_alsa_volume]);
						system(pavolume_cmd);
					}				    
				}
				else
				{
					volume = get_volume_to_float_index(m_SpeakerlastVolume);
					snprintf(amixer_cmd, sizeof(amixer_cmd), "amixer -c 0 sset 'DAC1' %d", volume);
					system(amixer_cmd);
					
					//#22027: Setting the bt sink volume before starting the playback
					if(shifu_bt_device_connected == SHIFU_SINK)
					{
						volume_led = get_volume_led(volume);
						bt_sync_alsa_volume = (uint8_t)volume_led;
						snprintf(pavolume_cmd, sizeof(pavolume_cmd), "pactl set-sink-volume bluez_sink %d%%", pa_volume_table_shifu[bt_sync_alsa_volume]);
						system(pavolume_cmd);
					}            
				}
	            ACSDK_DEBUG8(LX("SetPlayerVolume, mute is false").d("m_SpeakerlastVolume", m_SpeakerlastVolume));
	        }
	    }
	    else
	    {
	        ACSDK_DEBUG8(LX("SetPlayerVolume, mute is true"));
	        
	        m_isMuted = false;
	    
	        if(speakerType == SpeakerInterface::Type::AVS_ALERTS_VOLUME)
	        {
	            volume = get_volume_to_float_index(m_AlertlastVolume);
	            ACSDK_DEBUG9(LX("SetPlayerVolume, mute is true").d("m_AlertlastVolume", m_AlertlastVolume));
	        }
	        else
	        {
	            volume = get_volume_to_float_index(m_SpeakerlastVolume);
	            ACSDK_DEBUG8(LX("SetPlayerVolume, mute is true").d("m_SpeakerlastVolume", m_SpeakerlastVolume));
	        }
	        snprintf(amixer_cmd, sizeof(amixer_cmd), "amixer -c 0 sset 'DAC1' %d", volume);
	        system(amixer_cmd);         
	        volume_led = get_volume_led(volume);
	        alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_VOLUME, 1, volume_led);   

	        //#22027: Setting the bt sink volume before starting the playback
			if(shifu_bt_device_connected == SHIFU_SINK)
			{
				bt_sync_alsa_volume = (uint8_t)volume_led;
				snprintf(pavolume_cmd, sizeof(pavolume_cmd), "pactl set-sink-volume bluez_sink %d%%", pa_volume_table_shifu[bt_sync_alsa_volume]);
				system(pavolume_cmd);
			}
	    }
	}
}

int MediaPlayer::get_volume_to_float_index(gdouble gstVolume)
{   
/* #21812: Adding 0.05 to gstVolume to consider the second precision */
    int volume = int((gstVolume + 0.05) * 10);
    return volume_table[volume];
}

int MediaPlayer::get_volume_led(int volume)
{
    int i;
    
    for(i=0; i < 10; i++)
    {   
        if((volume == volume_table[i] || volume < volume_table[i+1]))
            break;
    }
    
    return i;   
}

double MediaPlayer::get_volume_index(void)
{
    int volume;
    int i = 0;
    double volume_index;
    char vol[4] = {0} ;
    FILE *fd    = NULL;
   
    fd = popen("amixer -c 0 cget numid=7 | grep \": values\" |  awk -F \",\"  '{ print $2 }'", "r");
        
    fgets(vol, 4, fd); 
    pclose(fd);
    
    volume = atoi(vol);
    
    for(i = 0; i < 10; i++)
    {   
        if((volume == volume_table[i] || volume < volume_table[i+1]))
            break;
    }
    
    volume_index = double(i);
    
    return double(volume_index/10);
}

}  // namespace mediaPlayer
}  // namespace alexaClientSDK

