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

#include <AVSCommon/Utils/Bluetooth/SDPRecords.h>
#include <iostream>
#include <syslog.h>
#include <AVSCommon/Utils/Logger/Logger.h>
// Redmine 21129: Start
#include "AVSCommon/Utils/LedInterface.h"
#include "shifu-led.h"
// Redmine 21129: End
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mutex>
#include "BlueZ/BlueZA2DPSink.h"
#include "BlueZ/BlueZA2DPSource.h"
#include "BlueZ/BlueZAVRCPController.h"
#include "BlueZ/BlueZAVRCPTarget.h"
#include "BlueZ/BlueZConstants.h"
#include "BlueZ/BlueZDeviceManager.h"

#include "BlueZ/BlueZBluetoothDevice.h"

// Redmine 21136: Audio sink A2DP profile ID
#define AUDIO_SINK_UUID "0000110b-0000-1000-8000-00805f9b34fb"

#define LAST_CONNECT_MACADDR "/etc/bluetooth/lastconnect_macaddr"
// Redmine 21230,21232: start: Defining macros for shifu_bt connection types, variables and sink volume table initialization
#define SHIFU_SOURCE 2
#define SHIFU_SINK 1
#define SHIFU_NONE 0

extern uint8_t shifu_bt_device_connected;
extern bool bt_sync_alsa_mute;
extern uint8_t bt_sync_alsa_volume;
// Redmine 21603: start: Variable to record External Play operations from phone
extern bool external_play;
// Redmine 21603: End
// Redmine 22027: Start: Pulse-audio volume table is initialized with level '5' instead of '0' to take care of low volume scenarios
static char pa_volume_table_shifu[] = {5,25,32,40,47,52,58,67,75,89,100};
// Remine 22027: End
// Redmine 21230,21232: End

// Redmine 21701,21729: Start: Defining variables for shifu connection status
static const char SHIFU_CONNECTED = '1';
static const char SHIFU_DISCONNECTED = '0';
// Redmine 21701,21729: End

namespace alexaClientSDK {
namespace bluetoothImplementations {
namespace blueZ {

using namespace avsCommon::utils;
using namespace avsCommon::utils::bluetooth;
using namespace avsCommon::sdkInterfaces::bluetooth;
using namespace avsCommon::sdkInterfaces::bluetooth::services;

// Redmine 22007: Start: Boolean variable to use when shifu connected with PC
bool shifu_connected_computer = false;
// Redmine 22007: End

// // Redmine 22146: Start: Updating connection status directly, no conditional wait case is required
// std::mutex sa_sync_connect_mutex;
// std::condition_variable sa_sync_connect_cv;
// bool sa_connected = false;
// // Redmine 22146: End

/// String to identify log entries originating from this file.
static const std::string TAG{"BlueZBluetoothDevice"};

/**
 * Create a LogEntry using this file's TAG and the specified event string.
 *
 * @param The event string for this @c LogEntry.
 */
#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

/// The Name property that BlueZ uses.
static const std::string BLUEZ_DEVICE_PROPERTY_ALIAS = "Alias";

/// The UUID property that BlueZ uses.
static const std::string BLUEZ_DEVICE_PROPERTY_UUIDS = "UUIDs";

/// An BlueZ error indicating when an object no longer exists.
static const std::string BLUEZ_ERROR_NOTFOUND = "org.bluez.Error.DoesNotExist";

/// A BlueZ connect error indicating authentication was rejected.
static const std::string BLUEZ_ERROR_RESOURCE_UNAVAILABLE = "org.bluez.Error.Failed: Resource temporarily unavailable";

/// BlueZ org.bluez.Device1 method to pair.
static const std::string BLUEZ_DEVICE_METHOD_PAIR = "Pair";

/// BlueZ org.bluez.Device1 method to connect.
static const std::string BLUEZ_DEVICE_METHOD_CONNECT = "Connect";

/// Redmine ID:21136: BlueZ org.bluez.Device1 method to connect specific UUID profile
static const std::string BLUEZ_DEVICE_METHOD_CONNECT_PROFILE = "ConnectProfile";

/// BlueZ org.bluez.Device1 method to disconnect.
static const std::string BLUEZ_DEVICE_METHOD_DISCONNECT = "Disconnect";

/// BlueZ org.bluez.Device1 paired property.
static const std::string BLUEZ_DEVICE_PROPERTY_PAIRED = "Paired";

/// BlueZ org.bluez.Device1 connected property.
static const std::string BLUEZ_DEVICE_PROPERTY_CONNECTED = "Connected";

/// BlueZ org.bluez.Adapter1 method to remove device.
static const std::string BLUEZ_ADAPTER_REMOVE_DEVICE = "RemoveDevice";

/// The Media Control interface on the DBus object.
static const std::string MEDIA_CONTROL_INTERFACE = "org.bluez.MediaControl1";

std::shared_ptr<BlueZBluetoothDevice> BlueZBluetoothDevice::create(
    const std::string& mac,
    const std::string& objectPath,
    std::shared_ptr<BlueZDeviceManager> deviceManager) {
    ACSDK_DEBUG5(LX(__func__));

    if (!g_variant_is_object_path(objectPath.c_str())) {
        ACSDK_ERROR(LX(__func__).d("reason", "invalidObjectPath").d("objPath", objectPath));
        return nullptr;
    }

    auto device = std::shared_ptr<BlueZBluetoothDevice>(new BlueZBluetoothDevice(mac, objectPath, deviceManager));

    if (!device->init()) {
        ACSDK_ERROR(LX(__func__).d("reason", "initFailed"));
        return nullptr;
    }

    return device;
}

BlueZBluetoothDevice::BlueZBluetoothDevice(
    const std::string& mac,
    const std::string& objectPath,
    std::shared_ptr<BlueZDeviceManager> deviceManager) :
        m_mac{mac},
        m_objectPath{objectPath},
        m_deviceState{BlueZDeviceState::FOUND},
        m_deviceManager{deviceManager} {
}

std::string BlueZBluetoothDevice::getMac() const {
    ACSDK_DEBUG5(LX(__func__));

    return m_mac;
}

std::string BlueZBluetoothDevice::getFriendlyName() const {
    ACSDK_DEBUG5(LX(__func__));

    return m_friendlyName;
}

bool BlueZBluetoothDevice::updateFriendlyName() {
    ACSDK_DEBUG5(LX(__func__));

    if (!m_propertiesProxy->getStringProperty(
            BlueZConstants::BLUEZ_DEVICE_INTERFACE, BLUEZ_DEVICE_PROPERTY_ALIAS, &m_friendlyName)) {
        ACSDK_ERROR(LX(__func__).d("reason", "getNameFailed"));
        return false;
    }

    return true;
}

BlueZBluetoothDevice::~BlueZBluetoothDevice() {
    ACSDK_DEBUG5(LX(__func__).d("mac", getMac()));
    m_executor.shutdown();

    {
        std::lock_guard<std::mutex> lock(m_servicesMapMutex);

        for (auto& entry : m_servicesMap) {
            entry.second->cleanup();
        }
        m_servicesMap.clear();
    }
}

bool BlueZBluetoothDevice::init() {
    ACSDK_DEBUG5(LX(__func__).d("path", m_objectPath));

    m_deviceProxy = DBusProxy::create(BlueZConstants::BLUEZ_DEVICE_INTERFACE, m_objectPath);
    if (!m_deviceProxy) {
        ACSDK_ERROR(LX(__func__).d("reason", "createDeviceProxyFailed"));
        return false;
    }

    m_propertiesProxy = DBusPropertiesProxy::create(m_objectPath);
    if (!m_propertiesProxy) {
        ACSDK_ERROR(LX(__func__).d("reason", "createPropertyProxyFailed"));
        return false;
    }

    updateFriendlyName();

    bool isPaired = false;
    if (queryDeviceProperty(BLUEZ_DEVICE_PROPERTY_PAIRED, &isPaired) && isPaired) {
        ACSDK_DEBUG5(LX(__func__).m("deviceIsPaired"));
        m_deviceState = BlueZDeviceState::IDLE;
    }

    // Parse UUIDs and find versions.
    if (!initializeServices(getServiceUuids())) {
        ACSDK_ERROR(LX(__func__).d("reason", "initializeServicesFailed"));
        return false;
    }

    return true;
}

bool BlueZBluetoothDevice::initializeServices(const std::unordered_set<std::string>& uuids) {
    ACSDK_DEBUG5(LX(__func__));

    for (const auto& uuid : uuids) {
        ACSDK_DEBUG9(LX(__func__).d("supportedUUID", uuid));

        // BlueZ does not provide the version of the service.
        if (A2DPSourceInterface::UUID == uuid && !serviceExists(uuid)) {
            ACSDK_DEBUG5(LX(__func__).d("supports", A2DPSourceInterface::NAME));
            auto a2dpSource = BlueZA2DPSource::create(m_deviceManager);
            if (!a2dpSource) {
                ACSDK_ERROR(LX(__func__).d("reason", "createA2DPFailed"));
                return false;
            } else {
                a2dpSource->setup();
                insertService(a2dpSource);
            }
        } else if (AVRCPTargetInterface::UUID == uuid && !serviceExists(uuid)) {
            ACSDK_DEBUG5(LX(__func__).d("supports", AVRCPTargetInterface::NAME));
            auto mediaControlProxy = DBusProxy::create(MEDIA_CONTROL_INTERFACE, m_objectPath);
            if (!mediaControlProxy) {
                ACSDK_ERROR(LX(__func__).d("reason", "nullMediaControlProxy"));
                return false;
            }

            auto avrcpTarget = BlueZAVRCPTarget::create(mediaControlProxy);
            if (!avrcpTarget) {
                ACSDK_ERROR(LX(__func__).d("reason", "createAVRCPTargetFailed"));
                return false;
            } else {
                avrcpTarget->setup();
                insertService(avrcpTarget);
            }
        } else if (A2DPSinkInterface::UUID == uuid && !serviceExists(uuid)) {
            ACSDK_DEBUG5(LX(__func__).d("supports", A2DPSinkInterface::NAME));
            auto a2dpSink = BlueZA2DPSink::create();
            if (!a2dpSink) {
                ACSDK_ERROR(LX(__func__).d("reason", "createA2DPSinkFailed"));
                return false;
            } else {
                a2dpSink->setup();
                insertService(a2dpSink);
            }
        } else if (AVRCPControllerInterface::UUID == uuid && !serviceExists(uuid)) {
            ACSDK_DEBUG5(LX(__func__).d("supports", AVRCPControllerInterface::NAME));
            auto avrcpController = BlueZAVRCPController::create();
            if (!avrcpController) {
                ACSDK_ERROR(LX(__func__).d("reason", "createAVRCPControllerFailed"));
                return false;
            } else {
                avrcpController->setup();
                insertService(avrcpController);
            }
        }
    }

    return true;
}

bool BlueZBluetoothDevice::isPaired() {
    ACSDK_DEBUG5(LX(__func__));

    auto future = m_executor.submit([this] { return executeIsPaired(); });

    if (future.valid()) {
        return future.get();
    } else {
        ACSDK_ERROR(LX(__func__).d("reason", "invalidFuture").d("action", "defaultingFalse"));
        return false;
    }
}

bool BlueZBluetoothDevice::executeIsPaired() {
    ACSDK_DEBUG5(LX(__func__));

    return BlueZDeviceState::UNPAIRED != m_deviceState && BlueZDeviceState::FOUND != m_deviceState;
}

std::future<bool> BlueZBluetoothDevice::pair() {
    ACSDK_DEBUG5(LX(__func__));

    return m_executor.submit([this] { return executePair(); });
}

bool BlueZBluetoothDevice::executePair() {
    ACSDK_DEBUG5(LX(__func__));

    ManagedGError error;
    m_deviceProxy->callMethod(BLUEZ_DEVICE_METHOD_PAIR, nullptr, error.toOutputParameter());

    if (error.hasError()) {
        ACSDK_ERROR(LX(__func__).d("error", error.getMessage()));
        return false;
    }
    return true;
}

std::future<bool> BlueZBluetoothDevice::unpair() {
    ACSDK_DEBUG5(LX(__func__));

    return m_executor.submit([this] { return executeUnpair(); });
}

bool BlueZBluetoothDevice::executeUnpair() {
    ACSDK_DEBUG5(LX(__func__));
    // Redmine 21402,21405: Start: Variables Declaration and reading connection status and mac_addr from the connected_devices list
    char previous_mac[SIZE_32];
    char previous_status;
    std::string mac_addr;
    mac_addr = getMac();
    connected_list_read(previous_mac,&previous_status);
    // Redmine 21402,21405: End
    ManagedGError error;

    auto adapterProxy = DBusProxy::create(BlueZConstants::BLUEZ_ADAPTER_INTERFACE, m_deviceManager->getAdapterPath());

    if (!adapterProxy) {
        ACSDK_ERROR(LX(__func__).d("error", "createAdapterProxyFailed"));
        return false;
    }
    // Redmine 21402,21405: Start: Verification of connected status based on mac_addr and calling disconnect function before unpairing
    if(previous_status == '1' && !strcmp(previous_mac,mac_addr.c_str()))
         executeDisconnect();
    // Redmine 21402,21405: End
    adapterProxy->callMethod(
        BLUEZ_ADAPTER_REMOVE_DEVICE, g_variant_new("(o)", m_objectPath.c_str()), error.toOutputParameter());

    if (error.hasError()) {
        std::string errorMsg = error.getMessage();
        // Treat as success if you can't find the device anymore.
        if (std::string::npos != errorMsg.find(BLUEZ_ERROR_NOTFOUND)) {
            return true;
        }
        ACSDK_ERROR(LX(__func__).d("error", errorMsg));
        return false;
    }

    return true;
}

std::string BlueZBluetoothDevice::getObjectPath() const {
    ACSDK_DEBUG5(LX(__func__));

    return m_objectPath;
}

std::unordered_set<std::string> BlueZBluetoothDevice::getServiceUuids(GVariant* array) {
    ACSDK_DEBUG5(LX(__func__));

    std::unordered_set<std::string> uuids;

    if (!array) {
        ACSDK_ERROR(LX(__func__).d("reason", "nullArray"));
        return uuids;
    } else if (!g_variant_is_of_type(array, G_VARIANT_TYPE_ARRAY)) {
        ACSDK_ERROR(LX(__func__).d("reason", "invalidType").d("type", g_variant_get_type_string(array)));
        return uuids;
    }

    GVariantTupleReader arrayReader(array);
    arrayReader.forEach([&uuids](GVariant* variant) {
        if (!variant) {
            ACSDK_ERROR(LX("iteratingArrayFailed").d("reason", "nullVariant"));
            return false;
        }
        // Do not free, this is not allocated.
        const gchar* temp = g_variant_get_string(variant, NULL);
        std::string uuid(temp);
        ACSDK_DEBUG5(LX(__func__).d("uuid", uuid));
        uuids.insert(uuid);
        return true;
    });

    return uuids;
}

std::unordered_set<std::string> BlueZBluetoothDevice::getServiceUuids() {
    ACSDK_DEBUG5(LX(__func__));

    // DBus returns this as (a{v},). We have to drill into the tuple to retrieve the array.
    ManagedGVariant uuidsTuple;
    if (!m_propertiesProxy->getVariantProperty(
            BlueZConstants::BLUEZ_DEVICE_INTERFACE, BLUEZ_DEVICE_PROPERTY_UUIDS, &uuidsTuple)) {
        ACSDK_ERROR(LX(__func__).d("reason", "getVariantPropertyFailed"));
        return std::unordered_set<std::string>();
    }

    GVariantTupleReader tupleReader(uuidsTuple);
    ManagedGVariant array = tupleReader.getVariant(0).unbox();

    if (!array.hasValue()) {
        // The format isn't what we were expecting. Print the original tuple for debugging.
        ACSDK_ERROR(LX(__func__).d("reason", "unexpectedVariantFormat").d("variant", uuidsTuple.dumpToString(false)));
        return std::unordered_set<std::string>();
    }

    return getServiceUuids(array.get());
}

bool BlueZBluetoothDevice::isConnected() {
    ACSDK_DEBUG5(LX(__func__));

    auto future = m_executor.submit([this] { return executeIsConnected(); });

    if (future.valid()) {
        return future.get();
    } else {
        ACSDK_ERROR(LX(__func__).d("reason", "invalidFuture").d("action", "defaultingFalse"));
        return false;
    }
}

bool BlueZBluetoothDevice::executeIsConnected() {
    ACSDK_DEBUG5(LX(__func__));

    return BlueZDeviceState::CONNECTED == m_deviceState;
}

std::future<bool> BlueZBluetoothDevice::connect() {
    ACSDK_DEBUG5(LX(__func__));

    return m_executor.submit([this] { return executeConnect(); });
}

std::string convert(const char *data)
{
    return std::string(data, data+strlen(data));
}

std::string mac_to_object_path(const char *mac_addr)
{
    std::string object_path_str;
    uint8_t i;
    object_path_str = convert(mac_addr);
    // cout << object_path_str << "\n";
    for(i=0; object_path_str[i]!='\0'; i++)
        if(object_path_str[i]==':')
            object_path_str[i]='_';
    
    object_path_str = "/org/bluez/hci0/dev_" + object_path_str;
    return object_path_str;
}

// Redmine 21119: Start: Reading Last Connected Mac Address and Status of Connection
void connected_list_read (char *mac_addr, char *status)
{
    // printf("%d\n", mac_addr);
    FILE *fd=NULL;
    char previous_mac[SIZE_32];
    char previous_status;
    memset(previous_mac, 0, SIZE_32);
    fd=fopen(LAST_CONNECT_MACADDR,"r+");
    (void)fread((void *) previous_mac, SIZE_OF_MAC, 1, fd);
    if(strlen(previous_mac)==0)
    {
        fclose(fd);
        strcpy(mac_addr,"XX:XX:XX:XX:XX:XX");
        *status = '0';
        return;
    }
    fseek(fd,18,SEEK_SET);
    (void)fread((void *)&previous_status, 1, 1, fd);
    fclose(fd);
    strcpy(mac_addr,previous_mac);
    printf("%s\n", mac_addr);
    printf("%c\n", previous_status);
    *status = previous_status;
}


// Writing Last Connected Mac Address and Status of Connection 
void connected_list_write(const char *mac_addr_char, char *status_char)
{
    FILE *fd=NULL;
    std::string set_mac;
    char previous_mac[SIZE_32];
    char previous_status;
    std::string mac_addr;
    std::string status;
    mac_addr = convert(mac_addr_char);
    status = convert(status_char);
    std::cout << mac_addr << "\n";
    // printf("previous = %s previous_status = %c", previous_mac, previous_status);    
    connected_list_read(previous_mac,&previous_status);
    // printf("after read previous = %s\n after read previous_status = %c\n", previous_mac, previous_status);
 
    if(!strcmp(previous_mac,mac_addr.c_str()))
    {
        goto WRITE_UPDATE;
    }
    else
    {
        if( (previous_status=='0') || ((previous_status == '1') && (status.at(0)=='1')) )
        {
WRITE_UPDATE:
            fd=fopen(LAST_CONNECT_MACADDR,"w+");
            set_mac = mac_addr + " " + status;
            (void)fwrite(set_mac.c_str(), set_mac.size(), 1, fd);
            fclose(fd);
        }
    }
}
// Redmine 21119: End

//
// Call back to Routing Audio to External Speaker
//
static int shifu_bt_speaker_audio_route(char * token)
{
    FILE *fd=NULL;
    char concat_buf[SIZE_64];

    sprintf(concat_buf, "pactl move-sink-input %s " BLUEZ_SINK,token);
    printf("%s\n", concat_buf);
    fd=popen(concat_buf,"r");
    if(fd==NULL)
    {
        return -1;
    }
    pclose(fd);
    return 0;
}

//
// Checking Active Streams to be routed to External Bluetooth Speaker
//
char *execute_speaker_route()
{
    FILE *fd=NULL;
    char concat_buf[SIZE_64];
    char *token;
    static char *free_ptr;
    free(free_ptr);
    char *result_string=(char *)malloc(SIZE_128);
    free_ptr=result_string;
    memset(result_string, 0, SIZE_128*sizeof(char));
    
    sprintf(concat_buf,"pactl list | grep \"Input #\" | awk \'{print $3}\'");
    fd=popen(concat_buf,"r");
    if(fd==NULL)
    {
        return NULL;
    }

    fread((void *)result_string,SIZE_128,1,fd);
    pclose(fd);
    if(strlen(result_string) == 0)
    {
        printf("Not able to access the NV variable:");
        return NULL;
    }
    else
    {
        printf("result_string = %s",result_string);
        token = strtok(result_string, "\n#"); 
    
        // Keep printing tokens while one of the 
        // delimiters present in str[]. 
        while (token != NULL) 
        { 
            shifu_bt_speaker_audio_route(token);
            token = strtok(NULL, "\n#"); 
        }
        return NULL;
    }
}

//
// Callback to check property of the External Bluetooth Device
//

int bluez_device_call_method(const gchar *object_path, const gchar *method, const gchar *prop, const char **output)
{
    GDBusConnection *con;
    con = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);
    GVariant *result;
    GError *error = NULL;

    result = g_dbus_connection_call_sync(con, 
                                        "org.bluez", 
                                        object_path,
                                        "org.freedesktop.DBus.Properties",
                                        method,
                                        g_variant_new("(ss)", "org.bluez.Device1",prop),
                                        G_VARIANT_TYPE("(v)"),
                                        G_DBUS_CALL_FLAGS_NONE,
                                        -1,
                                        NULL,
                                        &error);
    if(result) {
        result = g_variant_get_child_value(result, 0);
        GVariant *prop_val;
        g_variant_get(result, "v", &prop_val);
        *output = g_variant_get_string(prop_val,NULL);
        syslog(LOG_DEBUG, "[SHIFU][BT]:%s: output = %s\n",__func__,*output);        
        syslog(LOG_DEBUG, "[SHIFU][BT]:%s: Connected = %s\n", __func__, g_variant_get_string(prop_val, NULL));
        g_variant_unref(prop_val);
    }

    if(error != NULL)
        return 1;

    g_variant_unref(result);
    g_object_unref(con);
    return 0;
}

// Redmine 21119: Start: BT External Device Operation Call back
static int bluez_device_process_method(const gchar *method, const gchar *object_path)
{
    GDBusConnection *con;
    GVariant *result;
    GError *error = NULL;

    con = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);
    result = g_dbus_connection_call_sync(con, 
                                        "org.bluez", 
                                        object_path,
                                        "org.bluez.Device1",
                                        method,
                                        NULL,
                                        NULL,
                                        G_DBUS_CALL_FLAGS_NONE,
                                        -1,
                                        NULL,
                                        &error);

    if(error != NULL)
        return 1;

    g_variant_unref(result);
    g_object_unref(con);
    return 0;
}
// Redmine 21119: End

// Redmine 21470: Start: Explicit Last Connected Device Disconnection Callback function

bool BlueZBluetoothDevice::shifu_last_bt_disconnect()
{
    char previous_mac[SIZE_32];
    char previous_status;
    int rc;
    std::string previous_mac_op;
    std::string mac_addr;

    // Redmine 21119: Start: Explicit Disconnection of Previous Device Before Connection
    connected_list_read(previous_mac,&previous_status);
    // Disconnecting Previous device
    previous_mac_op = mac_to_object_path(previous_mac);
    rc = bluez_device_process_method("Disconnect", previous_mac_op.c_str());
    if(rc)
    {
        syslog(LOG_ERR, "[SHIFU][BT]:%s: Disconnection Failed", __func__);
        return false;
    }
    syslog(LOG_DEBUG, "[SHIFU][BT]:%s: Previous Device Disconnecting", __func__);
    connected_list_write(previous_mac,(char *)"0");
    // Redmine 21230,21232: start: Shifu_bt connection is updated the status as None if disconnected
    shifu_bt_device_connected = SHIFU_NONE;
    // Redmine 21230,21232: End
    return true;
}

// Redmine 21470: End

bool BlueZBluetoothDevice::executeConnect() {
    ACSDK_DEBUG5(LX(__func__));

    // If already connected, don't attempt to connect again.
    // On 5.37, a generic "Failed" error is thrown.

    // Redmine 21172: Start: Disabling due to double check of connection status.
   
    // if (executeIsConnected()) {
    //    syslog(LOG_ERR,"[SHIFU][BT]:%s: Alexa already Connected", __func__);
    //    return true;
    // }

    // Redmine 21172: End

    char previous_mac[SIZE_32];
    char previous_status;
    int rc;
    std::string previous_mac_op;
    std::string mac_addr;
    // Redmine 21136: Variable to store objectpath of requested device
    std::string shifu_bt_objectpath;

    // Redmine 21119: Start: Explicit Disconnection of Previous Device Before Connection
    mac_addr = getMac();
    // Redmine 21136: Connection requested device ObjectPath
    shifu_bt_objectpath = getObjectPath();
    connected_list_read(previous_mac,&previous_status);
    if(previous_status == '1')
    {
        if(strcmp(previous_mac, mac_addr.c_str())==0)
        {
            syslog(LOG_ERR,"[SHIFU][BT]:%s: Modified already Connected", __func__);
            return true;
        }
        else
        {
            // Redmine 21470: Start: Added function call to use from multiple Places
            if(!shifu_last_bt_disconnect())
            {
                syslog(LOG_ERR,"[SHIFU][BT]:%s: Previous device Disconnection Failure", __func__);
                return false;
            }
            // // Disconnecting Previous device
            // previous_mac_op = mac_to_object_path(previous_mac);
            // rc = bluez_device_process_method("Disconnect", previous_mac_op.c_str());
            // if(rc)
            // {
            //     syslog(LOG_ERR, "[SHIFU][BT]:%s: Disconnection Failed", __func__);
            //     return false;
            // }
            // syslog(LOG_ERR, "[SHIFU][BT]:%s: Previous Device Disconnecting", __func__);
            // connected_list_write(previous_mac,(char *)"0");
            // // Redmine 21230,21232: start: Shifu_bt connection is updated the status as None if disconnected
            // shifu_bt_device_connected = SHIFU_NONE;
            // // Redmine 21230,21232: End
            // Redmine 21470: End
        }
    }
    // Redmine 21119: End

    ManagedGError error;
    // Redmine 21136: Start: Connecting to Audio Sink A2DP profile when Connect to Speaker
    const char *result;
    rc = bluez_device_call_method(shifu_bt_objectpath.c_str(), "Get", "Icon", &result);
    if(rc)
    {
        syslog(LOG_ERR, "[SHIFU][BT]: Get Icon Failed");
        m_deviceProxy->callMethod(BLUEZ_DEVICE_METHOD_CONNECT, nullptr, error.toOutputParameter());
    }
    else
    {
        syslog(LOG_DEBUG, "[SHIFU][BT]:%s: Get Icon Success",__func__);
        if(!strcmp(result,"audio-card"))
            m_deviceProxy->callMethod(BLUEZ_DEVICE_METHOD_CONNECT_PROFILE, g_variant_new("(s)", AUDIO_SINK_UUID), error.toOutputParameter());
        else
            m_deviceProxy->callMethod(BLUEZ_DEVICE_METHOD_CONNECT, nullptr, error.toOutputParameter());
    }
    // Redmine 21136: End
    if (error.hasError()) {
        std::string errStr = error.getMessage() ? error.getMessage() : "";
        ACSDK_ERROR(LX(__func__).d("error", errStr));

        // This indicates an issue with authentication, likely the other device has unpaired.
        if (std::string::npos != errStr.find(BLUEZ_ERROR_RESOURCE_UNAVAILABLE)) {
            // Redmine 22146: Start: Event is set to True, to send FAILURE the event to AVS cloud
            transitionToState(BlueZDeviceState::CONNECTION_FAILED, true);
            // Redmine 22146: End
        }
        return false;
    }

    /*
     * If the current state is CONNECTION_FAILED, another Connected = true property changed signal may not appear.
     * We'll transition to the CONNECTED state directly here. If that signal does come, we simply
     * ignore it because there's no transition when you're already CONNECTED and you see a Connected = true.
     */
    // // Redmine 22146: Start: Connection success event will always update from here, and will not be updated from Device manager
    // if (BlueZDeviceState::CONNECTION_FAILED == m_deviceState) {
        transitionToState(BlueZDeviceState::CONNECTED, true);
    // }
    // // Redmine 22146: End

    // Redmine 21129: Start
    alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_BLUTOOTH_CONNECTED, 1, 0);
    // Redmine 21129: End

    std::string card_mac;
    std::string cmd;
    // Redmine 21230,21232: start: sink volume cmnd variable is assigned with volume level '0'
    char pavolume_cmd[64] = {0};
    // Redmine 21230,21232: End  

    // Redmine 21119: Start: Updating Last connected Device After successful connection
    connected_list_write(mac_addr.c_str(),(char *)"1");
    // Redmine 21119: End
    syslog(LOG_DEBUG, "[SHIFU][BT]: shifu_bt_objectpath = %s",shifu_bt_objectpath.c_str());

    // Redmine 21136: If there is no Property of Bluetooth Class, returning status as connected
    if(rc)
    {
        syslog(LOG_ERR, "[SHIFU][BT]: Get Icon Failed, Setting Status to NONE");
        return true;
    }
    // Checking for connected device and routing the Active Audio stream.
    if(strcmp(result,"audio-card")==0)
    {
        // Redmine 21230,21232: start: if audio-card is '0' then bt-sink is connected and variable is updated with status
        shifu_bt_device_connected = SHIFU_SINK;
        // Redmine 21230,21232: End        
        syslog(LOG_DEBUG, "[SHIFU][BT]:%s: Do your operation here",__func__);
        for(int i=20;i<=36;i++)
            card_mac.push_back(shifu_bt_objectpath.at(i));
        cmd = "pactl set-card-profile " BLUEZ_CARD + card_mac + " a2dp_sink";
        system(cmd.c_str());
        system("pactl set-default-sink " BLUEZ_SINK);
        // Redmine 21230,21232: start: If hub is muted then sink also set to '0' are previous volume
        if(bt_sync_alsa_mute)
            snprintf(pavolume_cmd, sizeof(pavolume_cmd), "pactl set-sink-volume bluez_sink %d%%", pa_volume_table_shifu[0]);
        else
            snprintf(pavolume_cmd, sizeof(pavolume_cmd), "pactl set-sink-volume bluez_sink %d%%", pa_volume_table_shifu[(int)bt_sync_alsa_volume]);
        system(pavolume_cmd);
        // Redmine 21230,21232: End      
        execute_speaker_route();
    }
    // Redmine 21470: Start: Updating Source Connection Status
    else if(strcmp(result,"phone")==0)
    {
        shifu_bt_device_connected = SHIFU_SOURCE;
	// Redmine 22007: Start: status should be false if shifu connected to phone
        shifu_connected_computer = false;
	// Redmine 22007: End
        syslog(LOG_DEBUG, "[SHIFU][BT]:%s: Connected to phone",__func__);
    }
    // Redmine 21470: End
    // Redmine 22007: Start: Assigning the connection_type based on bluez-card when shifu connected to computer
    else if(strcmp(result,"computer")==0)
    {
        shifu_connected_computer = true;
        if(!system("pactl list sinks | grep bluez_sink"))
        {
            shifu_bt_device_connected = SHIFU_SINK;
            syslog(LOG_DEBUG,"[SHIFU][BT]:%s: shifu_connected_computer_Sink = %d\n",__func__,shifu_connected_computer);
        }
        else
        {
            shifu_bt_device_connected = SHIFU_SOURCE;
            syslog(LOG_DEBUG,"[SHIFU][BT]:%s: shifu_connected_computer = %d\n",__func__,shifu_connected_computer);
        }
    }
    // Redmine 22007: End
    else
    {
        // Redmine 21230,21232: start: Shifu_bt status is updated as none if no device connected
        shifu_bt_device_connected = SHIFU_NONE;
        // Redmine 21230,21232: End
        syslog(LOG_DEBUG, "[SHIFU][BT]:%s: Not BT speaker",__func__);
    }
    return true;
}

std::future<bool> BlueZBluetoothDevice::disconnect() {
    ACSDK_DEBUG5(LX(__func__));
    syslog(LOG_DEBUG, "[SHIFU][BT]:%s: BT Disconnecting called",__func__);

    return m_executor.submit([this] { return executeDisconnect(); });
}

bool BlueZBluetoothDevice::executeDisconnect() {
    ACSDK_DEBUG5(LX(__func__));
    ManagedGError error;
    std::string mac_addr;
    mac_addr = getMac();
    // Redmine 21701,21729: Start: Checking the previous connection status when disconnect function is called again, to avoid mismatch of type of shifu connection status.
    char previous_mac[SIZE_32];
    char previous_status;

    connected_list_read(previous_mac,&previous_status);
    if(previous_status == SHIFU_CONNECTED && (strcmp(previous_mac, mac_addr.c_str())))
    {
        return true;
    }
    // Redmine 21701,21729: End

    m_deviceProxy->callMethod(BLUEZ_DEVICE_METHOD_DISCONNECT, nullptr, error.toOutputParameter());

    if (error.hasError()) {
        ACSDK_ERROR(LX(__func__).d("error", error.getMessage()));
        return false;
    }
    
    // Redmine 21603: Start: Triggering led for device disconnect and updating the external_play variable to false
    alexaClientSDK::avsCommon::utils::led::send_pattern_to_led_server(LED_BLUTOOTH_CONNECTED, 1, 0);
    external_play = false;
    // Redmine 21603: End

    syslog(LOG_DEBUG, "[SHIFU][BT]:%s: BT Disconnecting", __func__);
    connected_list_write(mac_addr.c_str(),(char *)"0");
    // Redmine 21230,21232: start: Shifu_bt status is updated as none if no device connected
    shifu_bt_device_connected = SHIFU_NONE;
    // Redmine 21230,21232: End
    int rc;
    const char *result;
    
    std::string shifu_bt_objectpath;
    shifu_bt_objectpath = getObjectPath();
    syslog(LOG_DEBUG, "[SHIFU][BT]: shifu_bt_objectpath = %s",shifu_bt_objectpath.c_str());

    rc = bluez_device_call_method(shifu_bt_objectpath.c_str(), "Get", "Icon", &result);
    if(rc)
    {
        syslog(LOG_ERR, "[SHIFU][BT]: Get Icon Failed");
        return true;
    }
    syslog(LOG_DEBUG, "[SHIFU][BT]:%s: Get Icon Success",__func__);
    if(strcmp(result,"audio-card")==0)
    {
        syslog(LOG_DEBUG, "[SHIFU][BT]:%s: Do your operation here",__func__);
        system("pactl set-default-sink " ALSA_SINK);
    }
    else
        syslog(LOG_DEBUG, "[SHIFU][BT]:%s: Not BT speaker",__func__);

    return true;
}

std::vector<std::shared_ptr<SDPRecordInterface>> BlueZBluetoothDevice::getSupportedServices() {
    ACSDK_DEBUG5(LX(__func__));

    std::vector<std::shared_ptr<SDPRecordInterface>> services;

    {
        std::lock_guard<std::mutex> lock(m_servicesMapMutex);

        for (auto& it : m_servicesMap) {
            services.push_back(it.second->getRecord());
        }
    }

    return services;
}

bool BlueZBluetoothDevice::serviceExists(const std::string& uuid) {
    std::lock_guard<std::mutex> lock(m_servicesMapMutex);
    return m_servicesMap.count(uuid) != 0;
}

bool BlueZBluetoothDevice::insertService(std::shared_ptr<BluetoothServiceInterface> service) {
    ACSDK_DEBUG5(LX(__func__));

    if (!service) {
        ACSDK_ERROR(LX(__func__).d("reason", "nullService"));
        return false;
    }

    std::shared_ptr<SDPRecordInterface> record = service->getRecord();

    if (!record) {
        ACSDK_ERROR(LX(__func__).d("reason", "nullRecord"));
        return false;
    }

    ACSDK_DEBUG5(LX(__func__).d("serviceUuid", record->getUuid()));

    bool success = false;
    {
        std::lock_guard<std::mutex> lock(m_servicesMapMutex);
        success = m_servicesMap.insert({record->getUuid(), service}).second;
    }

    if (!success) {
        ACSDK_ERROR(LX(__func__).d("reason", "serviceAlreadyExists"));
    }

    return success;
}

template <typename ServiceType>
std::shared_ptr<ServiceType> BlueZBluetoothDevice::getService() {
    ACSDK_DEBUG5(LX(__func__).d("uuid", ServiceType::UUID));

    std::shared_ptr<ServiceType> service = nullptr;
    {
        std::lock_guard<std::mutex> lock(m_servicesMapMutex);
        auto it = m_servicesMap.find(ServiceType::UUID);
        if (it == m_servicesMap.end()) {
            ACSDK_DEBUG(LX(__func__).d("reason", "serviceNotFound"));
        } else {
            // We completely control the types these are going to be, so avoid the overhead of dynamic_pointer_cast.
            service = std::static_pointer_cast<ServiceType>(it->second);
        }
    }

    return service;
}

std::shared_ptr<A2DPSinkInterface> BlueZBluetoothDevice::getA2DPSink() {
    ACSDK_DEBUG5(LX(__func__));

    return getService<A2DPSinkInterface>();
}

std::shared_ptr<A2DPSourceInterface> BlueZBluetoothDevice::getA2DPSource() {
    ACSDK_DEBUG5(LX(__func__));
    return getService<A2DPSourceInterface>();
}

std::shared_ptr<AVRCPTargetInterface> BlueZBluetoothDevice::getAVRCPTarget() {
    ACSDK_DEBUG5(LX(__func__));

    return getService<AVRCPTargetInterface>();
}

std::shared_ptr<AVRCPControllerInterface> BlueZBluetoothDevice::getAVRCPController() {
    ACSDK_DEBUG5(LX(__func__));

    return getService<AVRCPControllerInterface>();
}

DeviceState BlueZBluetoothDevice::getDeviceState() {
    ACSDK_DEBUG5(LX(__func__));

    return m_executor.submit([this] { return convertToDeviceState(m_deviceState); }).get();
}

avsCommon::sdkInterfaces::bluetooth::DeviceState BlueZBluetoothDevice::convertToDeviceState(
    BlueZDeviceState bluezDeviceState) {
    switch (bluezDeviceState) {
        case BlueZDeviceState::FOUND:
            return DeviceState::FOUND;
        case BlueZDeviceState::UNPAIRED:
            return DeviceState::UNPAIRED;
        case BlueZDeviceState::PAIRED:
            return DeviceState::PAIRED;
        case BlueZDeviceState::CONNECTION_FAILED:
        case BlueZDeviceState::IDLE:
            return DeviceState::IDLE;
        case BlueZDeviceState::DISCONNECTED:
            return DeviceState::DISCONNECTED;
        case BlueZDeviceState::CONNECTED:
            return DeviceState::CONNECTED;
    }

    ACSDK_ERROR(LX(__func__)
                    .d("reason", "noConversionFound")
                    .d("bluezDeviceState", bluezDeviceState)
                    .d("defaulting", DeviceState::FOUND));

    return DeviceState::FOUND;
}

bool BlueZBluetoothDevice::queryDeviceProperty(const std::string& name, bool* value) {
    ACSDK_DEBUG5(LX(__func__).d("name", name));

    if (!value) {
        ACSDK_ERROR(LX(__func__).d("reason", "nullValue"));
        return false;
    } else if (!m_propertiesProxy) {
        ACSDK_ERROR(LX(__func__).d("reason", "nullPropertiesProxy"));
        return false;
    }

    return m_propertiesProxy->getBooleanProperty(BlueZConstants::BLUEZ_DEVICE_INTERFACE, name.c_str(), value);
}

void BlueZBluetoothDevice::transitionToState(BlueZDeviceState newState, bool sendEvent) {
    ACSDK_DEBUG5(LX(__func__).d("oldState", m_deviceState).d("newState", newState).d("sendEvent", sendEvent));

    m_deviceState = newState;
    if (!m_deviceManager) {
        ACSDK_ERROR(LX(__func__).d("reason", "nullDeviceManager"));
        return;
    } else if (!m_deviceManager->getEventBus()) {
        ACSDK_ERROR(LX(__func__).d("reason", "nullEventBus"));
        return;
    }

    if (sendEvent) {
        m_deviceManager->getEventBus()->sendEvent(
            DeviceStateChangedEvent(shared_from_this(), convertToDeviceState(newState)));
    }
}

// Redmine 22146: Start: Updating the Connection status of Pairing Agent based on device
void BlueZBluetoothDevice::ConnectionTranistionStateUpdate() {
    ACSDK_DEBUG9(LX(__func__));
    transitionToState(BlueZDeviceState::CONNECTED, true);
}
// Redmine 22146: End

// TODO ACSDK-1398: Refactor this with a proper state machine.
void BlueZBluetoothDevice::onPropertyChanged(const GVariantMapReader& changesMap) {
    ACSDK_DEBUG5(LX(__func__).d("values", g_variant_print(changesMap.get(), true)));

    gboolean paired = false;
    bool pairedChanged = changesMap.getBoolean(BLUEZ_DEVICE_PROPERTY_PAIRED.c_str(), &paired);

    gboolean connected = false;
    bool connectedChanged = changesMap.getBoolean(BLUEZ_DEVICE_PROPERTY_CONNECTED.c_str(), &connected);

    // Changes to the friendlyName on the device will be saved on a new connect.
    char* alias = nullptr;
    bool aliasChanged = changesMap.getCString(BLUEZ_DEVICE_PROPERTY_ALIAS.c_str(), &alias);
    std::string aliasStr;

    if (aliasChanged) {
        // This should never happen. If it does, don't update.
        if (!alias) {
            ACSDK_ERROR(LX(__func__).d("reason", "nullAlias"));
            aliasChanged = false;
        } else {
            aliasStr = alias;
        }
    }

    // This is used for checking connectedness.
    bool a2dpSourceAvailable = false;
    bool a2dpSinkAvailable = false;

    /*
     * It's not guaranteed all services will be available at construction time.
     * If any become available at a later time, initialize them.
     */
    ManagedGVariant uuidsVariant = changesMap.getVariant(BLUEZ_DEVICE_PROPERTY_UUIDS.c_str());
    std::unordered_set<std::string> uuids;

    if (uuidsVariant.hasValue()) {
        auto uuids = getServiceUuids(uuidsVariant.get());
        initializeServices(uuids);

        a2dpSourceAvailable = (uuids.count(A2DPSourceInterface::UUID) > 0);
        a2dpSinkAvailable = (uuids.count(A2DPSinkInterface::UUID) > 0);
    }

    m_executor.submit([this,
                       pairedChanged,
                       paired,
                       connectedChanged,
                       connected,
                       a2dpSourceAvailable,
                       a2dpSinkAvailable,
                       aliasChanged,
                       aliasStr] {

        if (aliasChanged) {
            ACSDK_DEBUG5(LX("nameChanged").d("oldName", m_friendlyName).d("newName", aliasStr));
            m_friendlyName = aliasStr;
        }

        switch (m_deviceState) {
            case BlueZDeviceState::FOUND: {
                if (pairedChanged && paired) {
                    transitionToState(BlueZDeviceState::PAIRED, true);
                    transitionToState(BlueZDeviceState::IDLE, true);

                    /*
                     * A connect signal doesn't always mean a device is connected by the BluetoothDeviceInterface
                     * definition. This sequence has been observed:
                     *
                     * 1) Pairing (BlueZ sends Connect = true).
                     * 2) Pair Successful.
                     * 3) Connect multimedia services.
                     * 4) Connect multimedia services successful (BlueZ sends Paired = true, UUIDs = [array of
                     * uuids]).
                     *
                     * Thus we will use the combination of Connect, Paired, and the availability of certain UUIDs to
                     * determine connectedness.
                     */
                    bool isConnected = false;
                    if (queryDeviceProperty(BLUEZ_DEVICE_PROPERTY_CONNECTED, &isConnected) && isConnected &&
                        (a2dpSourceAvailable || a2dpSinkAvailable)) {
                        transitionToState(BlueZDeviceState::CONNECTED, true);
                    }
                }
                break;
            }
            case BlueZDeviceState::IDLE: {
                if (pairedChanged && !paired) {
                    transitionToState(BlueZDeviceState::UNPAIRED, true);
                    transitionToState(BlueZDeviceState::FOUND, true);
                }
                break;
            }
            case BlueZDeviceState::CONNECTED: {
                if (pairedChanged && !paired) {
                    transitionToState(BlueZDeviceState::UNPAIRED, true);
                    transitionToState(BlueZDeviceState::FOUND, true);
                } else if (connectedChanged && !connected) {
                    transitionToState(BlueZDeviceState::DISCONNECTED, true);
                    transitionToState(BlueZDeviceState::IDLE, true);
                }
                break;
            }
            case BlueZDeviceState::UNPAIRED:
            case BlueZDeviceState::PAIRED:
            case BlueZDeviceState::DISCONNECTED: {
                ACSDK_ERROR(LX("onPropertyChanged").d("reason", "invalidState").d("state", m_deviceState));
                break;
            }
            case BlueZDeviceState::CONNECTION_FAILED: {
                if (pairedChanged && !paired) {
                    transitionToState(BlueZDeviceState::UNPAIRED, true);
                    transitionToState(BlueZDeviceState::FOUND, true);
                }
                // Redmine 21603: start: Updating the DeviceState even in case of any failure of connection.
                else if(connectedChanged && !connected) {
                    syslog(LOG_DEBUG,"[SHIFU][BT]:%s: CONNECTION_FAILED: BluezDeviceState update for any connection failure", __func__);
                    transitionToState(BlueZDeviceState::DISCONNECTED, true);
                    transitionToState(BlueZDeviceState::IDLE, true);
                }
                // Redmine 21603: End
                // // Redmine 22146: Start: Connection states are directly handled from connection function callbacks
                // else if(connectedChanged && connected) {
                //    // Wait until main() sends data
                //    sa_connected = false;
                //    std::unique_lock<std::mutex> lock_sync(sa_sync_connect_mutex);
                //    if(sa_sync_connect_cv.wait_for(lock_sync, std::chrono::seconds(2), [](){return sa_connected;}))
                //    {
                //        syslog(LOG_DEBUG,"[SHIFU][BT]:%s: CONNECTED_AFTER_FAILURE", __func__);
                //        transitionToState(BlueZDeviceState::CONNECTED, true);
                //    }
                //    else
                //        syslog(LOG_ERR,"[SHIFU][BT]:%s: NOT_CONNECTED", __func__);
                //    lock_sync.unlock();
                // }
                // // Redmine 22146: End
            }
        }
    });
}

}  // namespace blueZ
}  // namespace bluetoothImplementations
}  // namespace alexaClientSDK

