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

#include "BlueZ/PairingAgent.h"

#include <iostream>
#include <string>
#include <stdbool.h>
// Redmine 21384: Start
#include "AVSCommon/Utils/LedInterface.h"
#include "shifu-led.h"
// Redmine 21384: End
#include <AVSCommon/Utils/Logger/Logger.h>
// Redmine 21230,21232: Start
#include <AVSCommon/Utils/Bluetooth/SDPRecords.h>
// Redmine 21230,21232: End
#include <BlueZ/BlueZConstants.h>
#include <BlueZ/BlueZDeviceManager.h>
#include "BlueZ/BlueZBluetoothDevice.h"
// Redmine 21389: Start: Defining Macro for A2DP_UUID
#define A2DP_UUID "0000110d-0000-1000-8000-00805f9b34fb"
// Redmine 21389: End
// Redmine 21230,21232: start: Defining macros for shifu_bt connection types, variables and sink volume table initialization
#define SHIFU_SOURCE 2
#define SHIFU_SINK 1
#define SHIFU_NONE 0

extern uint8_t shifu_bt_device_connected;
extern bool bt_sync_alsa_mute;
extern uint8_t bt_sync_alsa_volume;
extern bool external_play;
// Redmine 22027: Start: Pulse-audio volume table is initialized with level '5' instead of '0' to take care of low volume scenarios
static char pa_volume_table_shifu[] = {5,25,32,40,47,52,58,67,75,89,100};
// Redmine 22027: End
// Redmine 21230,21232: End
namespace alexaClientSDK {
namespace bluetoothImplementations {
namespace blueZ {

// Redmine 22007: Start: Varible to store status when shifu connected to computer
extern bool shifu_connected_computer;
// Redmine 22007: End

// // Redmine 22146: Start: Updating connection status directly, no conditional wait case is required
// extern std::mutex sa_sync_connect_mutex;
// extern std::condition_variable sa_sync_connect_cv;
// extern bool sa_connected;
// // Redmine 22146: End

/// String to identify log entries originating from this file.
static const std::string TAG{"PairingAgent"};

/**
 * Create a LogEntry using this file's TAG and the specified event string.
 *
 * @param The event string for this @c LogEntry.
 */
#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

/// The path we register the PairingAgent object under.
static const std::string AGENT_OBJECT_PATH = "/ACSDK/Bluetooth/Agent";

/**
 * The io_capability of the pairing agent.
 *
 * This is used for devices without a screen or display,
 * and tells BlueZ to use Simple Secure Pairing.
 */
static const std::string CAPABILITY = "NoInputNoOutput";

/// org.bluez.Agent1.Release method name.
static const std::string RELEASE = "Release";

/// org.bluez.Agent1.RequestPinCode method name.
static const std::string REQUEST_PIN_CODE = "RequestPinCode";

/// org.bluez.Agent1.DisplayPinCode method name.
static const std::string DISPLAY_PIN_CODE = "DisplayPinCode";

/// org.bluez.Agent1.RequestPasskey method name.
static const std::string REQUEST_PASSKEY = "RequestPasskey";

/// org.bluez.Agent1.DisplayPasskey method name.
static const std::string DISPLAY_PASSKEY = "DisplayPasskey";

/// org.bluez.Agent1.RequestConfirmation method name.
static const std::string REQUEST_CONFIRMATION = "RequestConfirmation";

/// org.bluez.Agent1.RequestAuthorization method name.
static const std::string REQUEST_AUTHORIZATION = "RequestAuthorization";

/// org.bluez.Agent1.AuthorizeService method name.
static const std::string AUTHORIZE_SERVICE = "AuthorizeService";

/// org.bluez.Agent1.Cancel method name.
static const std::string CANCEL = "Cancel";

/// BlueZ Identifier.
static const std::string BLUEZ_OBJECT_PATH = "/org/bluez";

/// Default Passkey.
const uint32_t DEFAULT_PASSKEY = 0;

/// Default PinCode.
const char* DEFAULT_PIN_CODE = "0000";

/// Redmine 22146: Fetching the Bluetooth devices from deviceManager
std::map<std::string, std::shared_ptr<BlueZBluetoothDevice>> shifu_bt_device_list;

// The introspect XML we ue to create the DBus object.
// clang-format off
const char INTROSPECT_XML[] = R"(
<!DOCTYPE node PUBLIC -//freedesktop//DTD D-BUS Object Introspection 1.0//EN
    "http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd";>
<node>
<interface name="org.bluez.Agent1">
    <method name="Release">
    </method>
    <method name="RequestPinCode">
        <arg type="s" direction="out"/>
        <arg name="device" type="o" direction="in"/>
        </method>
    <method name="DisplayPinCode">
        <arg type="s" direction="out"/>
        <arg name="device" type="o" direction="in"/>
        <arg name="pincode" type="s" direction="in"/>
    </method>
    <method name="RequestPasskey">
        <arg type="u" direction="out"/>
        <arg name="device" type="o" direction="in"/>
    </method>
    <method name="DisplayPasskey">
        <arg name="device" type="o" direction="in"/>
        <arg name="passkey" type="u" direction="in"/>
        <arg name="entered" type="q" direction="in"/>
    </method>
    <method name="RequestConfirmation">
        <arg name="device" type="o" direction="in"/>
        <arg name="passkey" type="u" direction="in"/>
    </method>
    <method name="RequestAuthorization">
        <arg name="device" type="o" direction="in"/>
    </method>
    <method name="AuthorizeService">
        <arg name="device" type="o" direction="in"/>
        <arg name="uuid" type="s" direction="in"/>
    </method>
    <method name="Cancel">
    </method>
</interface>
</node>)";
// clang-format on

std::unique_ptr<PairingAgent> PairingAgent::create(std::shared_ptr<DBusConnection> connection) {
    if (!connection) {
        ACSDK_ERROR(LX(__func__).d("reason", "nullConnection"));
        return nullptr;
    }

    auto pairingAgent = std::unique_ptr<PairingAgent>(new PairingAgent(connection));
    if (!pairingAgent->init()) {
        ACSDK_ERROR(LX(__func__).d("reason", "initFailed"));
        return nullptr;
    }

    return pairingAgent;
}

PairingAgent::PairingAgent(std::shared_ptr<DBusConnection> connection) :
        DBusObject(
            connection,
            INTROSPECT_XML,
            AGENT_OBJECT_PATH,
            {
                {RELEASE, &PairingAgent::release},
                {REQUEST_PIN_CODE, &PairingAgent::requestPinCode},
                {DISPLAY_PIN_CODE, &PairingAgent::displayPinCode},
                {REQUEST_PASSKEY, &PairingAgent::requestPasskey},
                {DISPLAY_PASSKEY, &PairingAgent::displayPasskey},
                {REQUEST_CONFIRMATION, &PairingAgent::requestConfirmation},
                {REQUEST_AUTHORIZATION, &PairingAgent::requestAuthorization},
                {AUTHORIZE_SERVICE, &PairingAgent::authorizeService},
                {CANCEL, &PairingAgent::cancel},
            }) {
}

bool PairingAgent::init() {
    if (!registerWithDBus()) {
        return false;
    }

    m_agentManager = DBusProxy::create(BlueZConstants::BLUEZ_AGENTMANAGER_INTERFACE, BLUEZ_OBJECT_PATH);
    if (!m_agentManager) {
        ACSDK_ERROR(LX(__func__).d("reason", "nullAgentManager"));
        return false;
    }

    return registerAgent() && requestDefaultAgent();
}

PairingAgent::~PairingAgent() {
    ACSDK_DEBUG5(LX(__func__));
    unregisterAgent();
}

void PairingAgent::release(GVariant* arguments, GDBusMethodInvocation* invocation) {
    ACSDK_DEBUG5(LX(__func__));
    g_dbus_method_invocation_return_value(invocation, nullptr);
}

void PairingAgent::requestPinCode(GVariant* arguments, GDBusMethodInvocation* invocation) {
    ACSDK_DEBUG5(LX(__func__));
    ACSDK_INFO(LX(__func__).d("pinCode", DEFAULT_PIN_CODE));

    // STRING type.
    auto parameters = g_variant_new("(s)", DEFAULT_PIN_CODE);
    g_dbus_method_invocation_return_value(invocation, parameters);
}

void PairingAgent::displayPinCode(GVariant* arguments, GDBusMethodInvocation* invocation) {
    ACSDK_DEBUG5(LX(__func__));
    g_dbus_method_invocation_return_value(invocation, nullptr);
}

void PairingAgent::requestPasskey(GVariant* arguments, GDBusMethodInvocation* invocation) {
    ACSDK_DEBUG5(LX(__func__));
    ACSDK_INFO(LX(__func__).d("passKey", DEFAULT_PASSKEY));

    // UINT32 type.
    auto parameters = g_variant_new("(u)", DEFAULT_PASSKEY);
    g_dbus_method_invocation_return_value(invocation, parameters);
}

void PairingAgent::displayPasskey(GVariant* arguments, GDBusMethodInvocation* invocation) {
    ACSDK_DEBUG5(LX(__func__));
    g_dbus_method_invocation_return_value(invocation, nullptr);
}

void PairingAgent::requestConfirmation(GVariant* arguments, GDBusMethodInvocation* invocation) {
    ACSDK_DEBUG5(LX(__func__));
    g_dbus_method_invocation_return_value(invocation, nullptr);
}

void PairingAgent::requestAuthorization(GVariant* arguments, GDBusMethodInvocation* invocation) {
    ACSDK_DEBUG5(LX(__func__));
    g_dbus_method_invocation_return_value(invocation, nullptr);
}

// Redmine 22146: Start: Getting requested device shared_pointer from device_list of DeviceManager
std::shared_ptr<BlueZBluetoothDevice> PairingAgent::getDeviceByPath(const std::string& path) {
    {
        std::lock_guard<std::mutex> guard(m_devicesMutex);
        auto iter = shifu_bt_device_list.find(path);
        if (iter != shifu_bt_device_list.end()) {
            return iter->second;
        }
    }

    ACSDK_ERROR(LX("getDeviceByPathFailed").d("reason", "deviceNotFound").d("path", path));

    return nullptr;
}
// Redmine 22146: End

void PairingAgent::authorizeService(GVariant* arguments, GDBusMethodInvocation* invocation) {
    ACSDK_DEBUG5(LX(__func__));
    // Redmine 21119: Start: Pulseaudio Dual connection Control
    char previous_mac[SIZE_32];
    // Redmine 21230,21232: Start: Defining volume_cmnd variable with level '0'
    char pavolume_cmd[64] = {0};
    // Redmine 21230,21232: End
    char previous_status;
    const gchar *uuid;
    const gchar *object_path;
    const gchar *address;
    // Redmine 21136: Start: Variable for getting the card name with Mac_addr
    std::string card_mac;
    std::string cmd;
    // Redmine 21336: End
    int rc;
    const char *result;

    g_variant_get(arguments, "(&os)", &object_path, &uuid);

    // Redmine 21136: Changing the ObjectPath to String type
    std::string shifu_bt_objectpath(object_path);

    syslog(LOG_DEBUG, "[SHIFU][BT][SA]:%s: address=%s, uuid=%s", __func__, object_path, uuid);
    rc = bluez_device_call_method(object_path, "Get", "Address", &address);
    if(rc)
    {
        syslog(LOG_ERR, "[SHIFU][BT][SA]: Get Address Failed");
        return;
    }

    // Redmine 22146: Start: Removed conditional blocker as connection status updating directly
    // std::unique_lock<std::mutex> lock_sync(sa_sync_connect_mutex);
    // sa_connected=true;
    // sa_sync_connect_cv.notify_one();
    // lock_sync.unlock();
    // Redmine 22146: End
    // Redmine 21389: End
    connected_list_read(previous_mac, &previous_status);
    // Redmine 21389: Start: Comparing the mac_address of connections and writing previous_mac to '0'
    if(strcmp(previous_mac,address))
    {
        // Redmine 21598: Start: Disconnecting Previous device whenever new device connected
        BlueZBluetoothDevice::shifu_last_bt_disconnect();
        syslog(LOG_DEBUG, "[SHIFU][BT][SA]:%s: last BT Disconnected",__func__);
        // Redmine 21598: End
        connected_list_write(previous_mac, (char*)"0");
        // Redmine 21425: Start: If external BT source Disconnected while Streaming this will turn OFF Volume control for the next device.
        external_play = false;
        // Redmine 21425: End
    }

    // Redmine 21389: Start: Invokes the dbus and returns the type of BT_Profile connected.
    g_dbus_method_invocation_return_value(invocation, nullptr);

    connected_list_write((char *)address, (char*)"1");

    // Redmine 22146: Start: Fetching the Requested device shared_pointer and updating the Connection status to AVS cloud
    auto deviceOnPath=getDeviceByPath(shifu_bt_objectpath);
    if (!deviceOnPath) {
        ACSDK_ERROR(LX(__func__).d("reason", "deviceDoesNotExist").d("path", deviceOnPath));
    }
    else
    {
        deviceOnPath->ConnectionTranistionStateUpdate();
    }
    // Redmine 22146: End

    // Redmine 21389: End

    if(strcmp(uuid, A2DP_UUID) == 0)
    {
            // Redmine 21384: Start: Trigger connect led pattern when BT is connected from external_bt settings
            alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_BLUTOOTH_CONNECTED, 1, 0);
            // Redmine 21384: End
            rc = bluez_device_call_method(object_path, "Get", "Icon", &result);
            if(rc)
            {
                syslog(LOG_ERR, "[SHIFU][BT][SA]: Get Icon Failed");
                return;
            }
            syslog(LOG_DEBUG, "[SHIFU][BT][SA]:%s: Get Icon Success",__func__);
            if(strcmp(result,"audio-card")==0)
            {
            	// Redmine 21230,21232: Start: Updating the status as sink connected
                shifu_bt_device_connected = SHIFU_SINK;
                // Redmine 21230,21232: End
                syslog(LOG_DEBUG, "[SHIFU][BT][SA]:%s: Do your operation here",__func__);
                
                // Redmine 21136: Start: After Connection for some speakers, pulseaudio card is taking 1 sec delay to appear
                sleep(1);
                // Some speakers are pulseaudio profile to OFF state instead of connecting to A2DP sink
                for(int i=20;i<=36;i++)
                    card_mac.push_back(shifu_bt_objectpath.at(i));
                cmd = "pactl set-card-profile " BLUEZ_CARD + card_mac + " a2dp_sink";
                system(cmd.c_str());
                // Redmine 21136: End

                system("pactl set-default-sink " BLUEZ_SINK);
                // Redmine 21230,21232: Start: Verifying the sync mute is true/false 
                if(bt_sync_alsa_mute)
                    snprintf(pavolume_cmd, sizeof(pavolume_cmd), "pactl set-sink-volume bluez_sink %d%%", pa_volume_table_shifu[0]);
                else
                    snprintf(pavolume_cmd, sizeof(pavolume_cmd), "pactl set-sink-volume bluez_sink %d%%", pa_volume_table_shifu[(int)bt_sync_alsa_volume]);
                system(pavolume_cmd);
                // Redmine 21230,21232: End
                syslog(LOG_DEBUG, "[SHIFU][BT][SA]:%s: Routing to External BT speaker",__func__);
                execute_speaker_route();
            }
            // Redmine 21470: Start: Updating the Global variable with BT source connection status.
            else if(strcmp(result,"phone")==0)
            {
                shifu_bt_device_connected = SHIFU_SOURCE;
                // Redmine 22007: Start: status should be false if shifu connected to phone
                shifu_connected_computer = false;
                // Redmine 22007: End
                syslog(LOG_DEBUG, "[SHIFU][BT][SA]:%s: Connected to phone",__func__);
            }
            // Redmine 21470: End
            // Redmine 22007: Start: If shifu connected to computer triggerring variable to true and updating shifu connection type is source
            else if(strcmp(result,"computer")==0)
            {
                shifu_connected_computer = true;
                shifu_bt_device_connected = SHIFU_SOURCE;
                syslog(LOG_DEBUG,"[SHIFU][BT][SA]:%s: shifu_connected_computer = %d\n",__func__,shifu_connected_computer);
            }
            // Redmine 22007: End
            else
            {
               // Redmine 21230,21232: start: Shifu_bt status is updated as none if no device connected
               shifu_bt_device_connected = SHIFU_NONE;
               // Redmine 21230,21232: End
               syslog(LOG_DEBUG, "[SHIFU][BT][SA]:%s: Not BT speaker",__func__);
            }

    }
    // Redmine 21119: End
}

void PairingAgent::cancel(GVariant* arguments, GDBusMethodInvocation* invocation) {
    ACSDK_DEBUG5(LX(__func__));
    g_dbus_method_invocation_return_value(invocation, nullptr);
}

bool PairingAgent::requestDefaultAgent() {
    ACSDK_DEBUG5(LX(__func__));
    ManagedGError error;

    // OBJECT_PATH type.
    auto parameters = g_variant_new("(o)", AGENT_OBJECT_PATH.c_str());
    m_agentManager->callMethod("RequestDefaultAgent", parameters, error.toOutputParameter());

    if (error.hasError()) {
        ACSDK_ERROR(LX(__func__).d("reason", "requestDefaultAgentFailed").d("error", error.getMessage()));
        return false;
    }

    ACSDK_DEBUG5(LX(__func__).m("requestDefaultAgentSuccessful"));
    return true;
}

bool PairingAgent::registerAgent() {
    ACSDK_DEBUG5(LX(__func__));
    ManagedGError error;

    // OBJECT_PATH & STRING type.
    auto parameters = g_variant_new("(os)", AGENT_OBJECT_PATH.c_str(), CAPABILITY.c_str());
    m_agentManager->callMethod("RegisterAgent", parameters, error.toOutputParameter());

    if (error.hasError()) {
        ACSDK_ERROR(LX(__func__).d("reason", "registerAgentFailed").d("error", error.getMessage()));
        return false;
    }

    ACSDK_DEBUG5(LX(__func__).m("registerAgentDone"));
    return true;
}

bool PairingAgent::unregisterAgent() {
    ACSDK_DEBUG5(LX(__func__));
    ManagedGError error;

    // OBJECT_PATH type.
    auto parameters = g_variant_new("(o)", AGENT_OBJECT_PATH.c_str());
    m_agentManager->callMethod("UnregisterAgent", parameters, error.toOutputParameter());

    if (error.hasError()) {
        ACSDK_ERROR(LX(__func__).d("reason", "unregisterAgentFailed").d("error", error.getMessage()));
        return false;
    }

    ACSDK_DEBUG5(LX(__func__).m("unregisterAgentDone"));
    return true;
}

}  // namespace blueZ
}  // namespace bluetoothImplementations
}  // namespace alexaClientSDK

