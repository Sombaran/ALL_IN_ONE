/*
 * Copyright 2017-2018 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

#include <cstring>
#include <string>

#include <rapidjson/document.h>

#include <AVSCommon/Utils/Configuration/ConfigurationNode.h>
#include <AVSCommon/Utils/Logger/Logger.h>
#include "SampleApp/PortAudioMicrophoneWrapper.h"
#include "SampleApp/ConsolePrinter.h"

namespace alexaClientSDK {
namespace sampleApp {

using avsCommon::avs::AudioInputStream;
FILE *mDebugFilePtr;
int mSampleWrittenToDebugfile;

static const int NUM_INPUT_CHANNELS = 2;
static const int NUM_OUTPUT_CHANNELS = 0;
static const double SAMPLE_RATE = 16000;
static const unsigned long PREFERRED_SAMPLES_PER_CALLBACK = paFramesPerBufferUnspecified;

static const int DEBUG_FILE_DURATION = 120;		/** 2 minutes or 120 seconds*/
static const int NUM_OF_SAMPLES_TO_DEBUG = SAMPLE_RATE * 120;
static const char* DEBUG_FILENAME = "/root/va_data/debug/audio_debug_file.raw";
static const std::string PORTAUDIORECORDING("portAudioRecording");

static const std::string SAMPLE_APP_CONFIG_ROOT_KEY("sampleApp");
static const std::string PORTAUDIO_CONFIG_ROOT_KEY("portAudio");
static const std::string PORTAUDIO_CONFIG_SUGGESTED_LATENCY_KEY("suggestedLatency");

/// String to identify log entries originating from this file.
static const std::string TAG("PortAudioMicrophoneWrapper");

/**
 * Create a LogEntry using this file's TAG and the specified event string.
 *
 * @param The event string for this @c LogEntry.
 */
#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

std::unique_ptr<PortAudioMicrophoneWrapper> PortAudioMicrophoneWrapper::create(
    std::shared_ptr<AudioInputStream> stream) {
    if (!stream) {
        ACSDK_CRITICAL(LX("Invalid stream passed to PortAudioMicrophoneWrapper"));
        return nullptr;
    }
    std::unique_ptr<PortAudioMicrophoneWrapper> portAudioMicrophoneWrapper(new PortAudioMicrophoneWrapper(stream));
    if (!portAudioMicrophoneWrapper->initialize()) {
        ACSDK_CRITICAL(LX("Failed to initialize PortAudioMicrophoneWrapper"));
        return nullptr;
    }
    return portAudioMicrophoneWrapper;
}

PortAudioMicrophoneWrapper::PortAudioMicrophoneWrapper(std::shared_ptr<AudioInputStream> stream) :
        m_audioInputStream{stream},
        m_paStream{nullptr} {
}

PortAudioMicrophoneWrapper::~PortAudioMicrophoneWrapper() {
    Pa_StopStream(m_paStream);
    Pa_CloseStream(m_paStream);
    Pa_Terminate();
}

bool PortAudioMicrophoneWrapper::initialize() {
    m_writer = m_audioInputStream->createWriter(AudioInputStream::Writer::Policy::NONBLOCKABLE);
    if (!m_writer) {
        ACSDK_CRITICAL(LX("Failed to create stream writer"));
        return false;
    }
    PaError err;
    err = Pa_Initialize();
    if (err != paNoError) {
        ACSDK_CRITICAL(LX("Failed to initialize PortAudio").d("errorCode", err));
        return false;
    }

    PaTime suggestedLatency;
    bool latencyInConfig = getConfigSuggestedLatency(suggestedLatency);

    if (!latencyInConfig) {
        err = Pa_OpenDefaultStream(
            &m_paStream,
            NUM_INPUT_CHANNELS,
            NUM_OUTPUT_CHANNELS,
            paInt16,
            SAMPLE_RATE,
            PREFERRED_SAMPLES_PER_CALLBACK,
            PortAudioCallback,
            this);
    } else {
        ACSDK_INFO(
            LX("PortAudio suggestedLatency has been configured to ").d("Seconds", std::to_string(suggestedLatency)));

        PaStreamParameters inputParameters;
        std::memset(&inputParameters, 0, sizeof(inputParameters));
        inputParameters.device = Pa_GetDefaultInputDevice();
        inputParameters.channelCount = NUM_INPUT_CHANNELS;
        inputParameters.sampleFormat = paInt16;
        inputParameters.suggestedLatency = suggestedLatency;
        inputParameters.hostApiSpecificStreamInfo = nullptr;

        err = Pa_OpenStream(
            &m_paStream,
            &inputParameters,
            nullptr,
            SAMPLE_RATE,
            PREFERRED_SAMPLES_PER_CALLBACK,
            paNoFlag,
            PortAudioCallback,
            this);
    }
	if (err != paNoError) {
      ACSDK_CRITICAL(LX("Failed to open PortAudio default stream").d("errorCode", err));
      return false;
    }
    /**
	 *[BUG][22040]
	 *  Reading the value from the Main Alexa configuration file.
	 ***/
    
    
    auto config = avsCommon::utils::configuration::ConfigurationNode::getRoot()[SAMPLE_APP_CONFIG_ROOT_KEY];
    if(config)
    {
		config.getBool(PORTAUDIORECORDING, &portAudioRecording, false);
	}
    return true;
}

bool PortAudioMicrophoneWrapper::startStreamingMicrophoneData() {
    std::lock_guard<std::mutex> lock{m_mutex};
    PaError err = Pa_StartStream(m_paStream);
    if (err != paNoError) {
        ACSDK_CRITICAL(LX("Failed to start PortAudio stream"));
        return false;
    }
    return true;
}

bool PortAudioMicrophoneWrapper::stopStreamingMicrophoneData() {
    std::lock_guard<std::mutex> lock{m_mutex};
    PaError err = Pa_StopStream(m_paStream);
    if (err != paNoError) {
        ACSDK_CRITICAL(LX("Failed to stop PortAudio stream"));
        return false;
    }
    return true;
}

int PortAudioMicrophoneWrapper::PortAudioCallback(
	const void* inputBuffer,
	void* outputBuffer,
	unsigned long numSamples,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void* userData) {
	PortAudioMicrophoneWrapper* wrapper = static_cast<PortAudioMicrophoneWrapper*>(userData);
	/* smartron changes START */
	short int* input_left_right_channel;
	short int* left_channel;
	unsigned long i;
	input_left_right_channel =(short int *)inputBuffer;
	left_channel =(short int *)malloc(numSamples*2);//think about size of the byte not channels ---
	//std::memcpy(input_left_right_channel,inputBuffer,numSamples/2);

	for(i=0;i<numSamples;i++){
		left_channel[i]=input_left_right_channel[2*i];
	}
	/**
	 *[BUG][22040]
	 *  portAudioRecording is true from configuration file then we will open the file and will write in to a file.
	 *  if it false we will not open a file.
	 * 
	 * the recording time is 2 minutes.* 
	 ***/
	if(portAudioRecording == true)
	{
		if(mSampleWrittenToDebugfile > NUM_OF_SAMPLES_TO_DEBUG){
			fclose(mDebugFilePtr);
			mSampleWrittenToDebugfile = 0;
		}
		if(mSampleWrittenToDebugfile == 0){
			mDebugFilePtr = fopen(DEBUG_FILENAME,"wb");
			if(mDebugFilePtr == NULL){
				ACSDK_CRITICAL(LX("Failed to write in debug."));
				return paAbort;
			}
		}
		fwrite(left_channel, 2, numSamples, mDebugFilePtr);
		fflush(mDebugFilePtr);
		mSampleWrittenToDebugfile += numSamples;
	}
	ssize_t returnCode = wrapper->m_writer->write((void *)left_channel, numSamples);
	free(left_channel);
	if (returnCode <= 0) {
	ACSDK_CRITICAL(LX("Failed to write to stream."));
	return paAbort;
	}

	return paContinue;
	/* smartron changes END*/
}
	/**
	 *[BUG][22040]  
	 * 
	 *  Default initilization for recording  is false.
	 ***/
bool PortAudioMicrophoneWrapper::portAudioRecording=false;

bool PortAudioMicrophoneWrapper::getConfigSuggestedLatency(PaTime& suggestedLatency) {
    bool latencyInConfig = false;
    auto config = avsCommon::utils::configuration::ConfigurationNode::getRoot()[SAMPLE_APP_CONFIG_ROOT_KEY]
                                                                               [PORTAUDIO_CONFIG_ROOT_KEY];
    if (config) {
        latencyInConfig = config.getValue(
            PORTAUDIO_CONFIG_SUGGESTED_LATENCY_KEY,
            &suggestedLatency,
            suggestedLatency,
            &rapidjson::Value::IsDouble,
            &rapidjson::Value::GetDouble);
    }

    return latencyInConfig;
}

}  // namespace sampleApp
}  // namespace alexaClientSDK
