/*
 * Copyright 2018 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

#include "KWDProvider/KeywordDetectorProvider.h"

#ifdef KWD_KITTAI
#include <KittAi/KittAiKeyWordDetector.h>
#elif KWD_SENSORY
#include <Sensory/SensoryKeywordDetector.h>
#elif KWD_AMAZONLITE
#include <AmazonLite/PryonLiteKeywordDetector.h>
#endif

#ifdef KWD_KITTAI
/// The sensitivity of the Kitt.ai engine.
static const double KITT_AI_SENSITIVITY = 0.6;

/// The audio amplifier level of the Kitt.ai engine.
static const float KITT_AI_AUDIO_GAIN = 2.0;

/// Whether Kitt.ai should apply front end audio processing.
static const bool KITT_AI_APPLY_FRONT_END_PROCESSING = true;
#endif

using namespace alexaClientSDK;
using namespace alexaClientSDK::kwd;

std::unique_ptr<kwd::AbstractKeywordDetector> KeywordDetectorProvider::create(
    std::shared_ptr<avsCommon::avs::AudioInputStream> stream,
    avsCommon::utils::AudioFormat audioFormat,
    std::unordered_set<std::shared_ptr<avsCommon::sdkInterfaces::KeyWordObserverInterface>> keyWordObservers,
    std::unordered_set<std::shared_ptr<avsCommon::sdkInterfaces::KeyWordDetectorStateObserverInterface>>
        keyWordDetectorStateObservers,
    const std::string& pathToInputFolder) {
#if defined(KWD_KITTAI)
    return alexaClientSDK::kwd::KittAiKeyWordDetector::create(
        stream,
        audioFormat,
        keyWordObservers,
        keyWordDetectorStateObservers,
        pathToInputFolder + "/common.res",
        {{pathToInputFolder + "/alexa.umdl", "ALEXA", KITT_AI_SENSITIVITY}},
        KITT_AI_AUDIO_GAIN,
        KITT_AI_APPLY_FRONT_END_PROCESSING);

#elif defined(KWD_SENSORY)
    return alexaClientSDK::kwd::SensoryKeywordDetector::create(
        stream,
        audioFormat,
        keyWordObservers,
        keyWordDetectorStateObservers,
        pathToInputFolder + "/spot-alexa-rpi-31000.snsr");
#elif defined(KWD_AMAZONLITE)
	/**
     * [22416][ALEXA][voiceassistant]Flag to configure threshold in avs-device-sdk
     * 
     * variable used is sm_threshold_config variable default 800.
     * 
     * we can configure in  AlexaClientSDKConfig.json file.
     * 
     * 
     * 
     * "sampleApp":{
        // To specify if the SampleApp supports display cards.
        "displayCardsSupported":false,
        "PryonLiteModelPath":"/etc/amazon_model/D.en-IN.alexa.bin",
        //[BUG][22040] if we need recording please set below flag to true,by default it is false
         "portAudioRecording":false,
         //[22416][ALEXA][voiceassistant]Flag to configure threshold in avs-device-sdk
         "PryonLiteThreshold":800,
        // The firmware version of the device to send in SoftwareInfo event.
        // Note: The firmware version should be a positive 32-bit integer in the range [1-2147483647].
		// This is Smartron Shifu t.hub firmware version which will be updated for every release
        "firmwareVersion": 6789,
        // The default endpoint to connect to.
        // See https://developer.amazon.com/docs/alexa-voice-service/api-overview.html#endpoints for regions and values
        "endpoint": "https://avs-alexa-eu.amazon.com"

        // Example of specifying suggested latency in seconds when openning PortAudio stream. By default,
        // when this paramater isn't specified, SampleApp calls Pa_OpenDefaultStream to use the default value.
        // See http://portaudio.com/docs/v19-doxydocs/structPaStreamParameters.html for further explanation
        // on this parameter.
        //"portAudio":{
        //    "suggestedLatency": 0.150
        //}
     },
     * 
     * 
     * 
     ***/

	int sm_threshold_config=0;
	alexaClientSDK::avsCommon::utils::configuration::ConfigurationNode::getRoot()[std::string("sampleApp")].getInt(
        "PryonLiteThreshold", &sm_threshold_config,800);
    std::string modelPath;
    alexaClientSDK::avsCommon::utils::configuration::ConfigurationNode::getRoot()[std::string("sampleApp")].getString(
        "PryonLiteModelPath", &modelPath);
    if (modelPath.empty()) {
          // Use the embedded model
          return alexaClientSDK::kwd::PryonLiteKeywordDetector::create(
               stream, audioFormat, keyWordObservers, keyWordDetectorStateObservers);
     } else {
          // Use a dynamic model provided on the modelPath
          return alexaClientSDK::kwd::PryonLiteKeywordDetector::create(
               stream, audioFormat, keyWordObservers, keyWordDetectorStateObservers, modelPath,sm_threshold_config);
     }
#else
    return nullptr;
#endif
}
