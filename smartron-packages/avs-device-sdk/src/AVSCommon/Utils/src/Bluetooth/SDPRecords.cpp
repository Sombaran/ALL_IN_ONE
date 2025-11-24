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
#include "AVSCommon/SDKInterfaces/Bluetooth/Services/A2DPSinkInterface.h"
#include "AVSCommon/SDKInterfaces/Bluetooth/Services/A2DPSourceInterface.h"
#include "AVSCommon/SDKInterfaces/Bluetooth/Services/AVRCPControllerInterface.h"
#include "AVSCommon/SDKInterfaces/Bluetooth/Services/AVRCPTargetInterface.h"

#include "AVSCommon/Utils/Bluetooth/SDPRecords.h"
// Redmine 21156: Start: Declaration and Definition Wakeword detect variable   
bool WD_BT_trig=false;
// Redmine 21156: End

// Redmine 21823: Start: If wakeword triggered while streaming this variable will be triggered
bool WD_trig_internal = false;
// Redmine 21823: End

// Redmine 21230,21232: start: Global variables for shifu connection types and sink volume variables
uint8_t shifu_bt_device_connected=0;
bool bt_sync_alsa_mute=false;
uint8_t bt_sync_alsa_volume=10;
// Redmine 21230,21232: End

// Redmine 21298: Start: Variable will be triggered when Android phone Mediaplayer is initiated
bool external_play=false;
// Redmine 21298: End

// Redmine 21463: Start: MediaTransport Idle should be on only if cloud_play is not processed
bool cloud_play = false;

// After alarm is turned OFF, alarm_trig will indicate Alarm is disabled, BT Source can start Streaming.
// Redmine 22060: Start: Changing the type to check the Start of Alarm
uint8_t alarm_trig = 0;
// Redmine 22060: End

// Redmine 21463: End

namespace alexaClientSDK {
namespace avsCommon {
namespace utils {
namespace bluetooth {

using namespace avsCommon::sdkInterfaces::bluetooth::services;

SDPRecord::SDPRecord(const std::string& name, const std::string& uuid, const std::string& version) :
        m_name{name},
        m_uuid{uuid},
        m_version{version} {
}

std::string SDPRecord::getName() const {
    return m_name;
}

std::string SDPRecord::getUuid() const {
    return m_uuid;
}

std::string SDPRecord::getVersion() const {
    return m_version;
}

A2DPSourceRecord::A2DPSourceRecord(const std::string& version) :
        SDPRecord{A2DPSourceInterface::NAME, A2DPSourceInterface::UUID, version} {
}

A2DPSinkRecord::A2DPSinkRecord(const std::string& version) :
        SDPRecord{A2DPSinkInterface::NAME, A2DPSinkInterface::UUID, version} {
}

AVRCPTargetRecord::AVRCPTargetRecord(const std::string& version) :
        SDPRecord{AVRCPTargetInterface::NAME, AVRCPTargetInterface::UUID, version} {
}

AVRCPControllerRecord::AVRCPControllerRecord(const std::string& version) :
        SDPRecord{AVRCPControllerInterface::NAME, AVRCPControllerInterface::UUID, version} {
}

}  // namespace bluetooth
}  // namespace utils
}  // namespace avsCommon
}  // namespace alexaClientSDK


