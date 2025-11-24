mobile_agent Support APIs v1.4.1
==============================

Bluetooth APIs
==============
To use All the Blueooth APIs, Device should be registered first.
O.W. APIs will respond back with `401:Unauthorized request`

<BT-API-01>

## start_discovery
This API call helps to discover all the bluetooth devices nearby.
Device should be in discoverable mode to get the device in discovered devices list.

### GET /api/bluetooth/discovery/start

#### Request Headers
* `X-AccountToken`(*String*, **required**) -- the value received from tronX server
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* No request Parameters

#### Response Headers
* No Response Headers

#### Response Parameters
* `status`(*string*) -- success OR failure
* `device_list`(*json*) -- A list of all discovered devices with their names and mac addresses, If status is 'success'.

#### Response Codes
code        | status  | message                                          |
------------|:-------:|:-------------------------------------------------|
200         | Success | discovery successful                             |
400         | error   | Please include X-AccountToken in request headers |
401         | error   | Unauthorized request                             |
500         | error   | discovery failed                                 |
500         | error   | BTError: BT discovering is already in process    | 

<BT-API-02>

## connect_device
This API call helps to connect a BT device to shifu's BT.

### POST /api/bluetooth/device/connect

#### Request Headers
* `X-AccountToken`(*String*, **required**) -- the value received from tronX server
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* `mac-id`(*String*,**required**) -- Mac Id of the device which need to be paired.

#### Response Headers
* No Response Headers

#### Response Parameters
* `status`(*string*) -- success OR failure
* `message`(*string*) -- message from hub

#### Response Codes
code        | status  | message                                          |
------------|:-------:|:-------------------------------------------------|
200         | Success | device is paired                                 |
400         | error   | Please include X-AccountToken in request headers |
401         | error   | Unauthorized request                             |
500         | error   | device is not paired.                            |

<BT-API-03>

## disconnect_device
This API call helps to disconnect a device which is already connectd to Shifu's BT.

### POST /api/bluetooth/device/disconnect

#### Request Headers
* `X-AccountToken`(*String*, **required**) -- value received from tronX server
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* `mac-id`(*String*,**required**) -- Mac Id of the device which need to be paired.

#### Response Headers
* No Response Headers

#### Response Parameters
* `status`(*string*) -- success OR failure
* `message`(*string*) -- message from hub

#### Response Codes
code        | status  | message                                          |
------------|:-------:|:-------------------------------------------------|
200         | success | Disconnection successful                         |
400         | error   | Please include X-AccountToken in request headers |
401         | error   | Unauthorized request                             |
500         | error   | Disconnection failed                             |

<BT-API-04>

## forget_device
This API call helps to remove a device from paired devices list.

### POST /api/bluetooth/device/forget

#### Request Headers
* `X-AccountToken`(*String*, **required**) -- value received from tronX server.
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* `mac-id`(*String*,**required**) -- Mac Id of the device which need to be paired.

#### Response Headers
* No Response Headers

#### Response Parameters
* `status`(*string*) -- success OR failure
* `message`(*string*) -- message from hub

#### Response Codes
code        | status  | message                                          |
------------|:-------:|:-------------------------------------------------|
200         | success | Device forget successful                         |
400         | error   | Please include X-AccountToken in request headers |
401         | error   | Unauthorized request                             |
500         | error   | Device forget failed                             |

<BT-API-05>

## paired devices
This API call helps to get the all previously paired devices to Shifu's BT in a list.

### GET /api/bluetooth/devices/paired

#### Request Headers
* `X-AccountToken`(*String*, **required**) -- value received from tronX server.
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* No Request Parameters

#### Response Headers
* No Response Headers

#### Response Parameters
* `status`(*string*) -- success OR failure
* `message`(*string*) -- device_list on success
                         Error message on failure   

#### Response Codes
code        | status  | message                                          |
------------|:-------:|:-------------------------------------------------|
200         | success | device_list in dictionary format                 |
400         | error   | Please include X-AccountToken in request headers |
401         | error   | Unauthorized request                             |
500         | error   | Error message from BT stack                      |

<BT-API-06>

## Connected Devices
This API call helps to get the currently connected device to Shifu's BT.

### GET /api/bluetooth/devices/connected

#### Request Headers
* `X-AccountToken`(*String*, **required**) -- value received from tronX server.
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* No Request Parameters

#### Response Headers
* No Response Headers

#### Response Parameters
* `status`(*string*) -- success OR failure
* `message`(*string*) -- device_list on success
                         Error message on failure   

#### Response Codes
code        | status  | message                                          |
------------|:-------:|:-------------------------------------------------|
200         | success | device_list in dictionary format                 |
400         | error   | Please include X-AccountToken in request headers |
401         | error   | Unauthorized request                             |
500         | error   | Error message from BT stack                      |

<BT-API-07>

## Set BT discoverable Time
By default BT is discoverable for 60 seconds whenever BT is started.
This API call helps to keep the shifu again in a discoverable mode for a specific time.

### POST /api/bluetooth/discoverable

#### Request Headers
* `X-AccountToken`(*String*, **required**) -- value received from tronX server.
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* `timeout`(*String*,**Optional**) -- any value in seconds

#### Response Headers
* No Response Headers

#### Response Parameters
* `status`(*string*) -- success OR failure
* `message`(*string*) -- messages from BT Stack
#### Response Codes
code        | status  | message                                          |
------------|:-------:|:-------------------------------------------------|
200         | success | Bluetooth discoverable time is set               |
400         | error   | Please include X-AccountToken in request headers |
401         | error   | Unauthorized request                             |
500         | error   | Error message from BT stack                      |

<BT-API-08>

## Get Connected and Paired Devices List
This API call helps to get the combined list of all the devices connected and paired to Shifu's BT in a list.

### GET /api/bluetooth/devices/list

#### Request Headers
* `X-AccountToken`(*String*, **required**) -- value received from tronX server.
* `content-type`(*String*, **required**) -- application/json

#### Request Parameters
* No Request Parameters

#### Response Headers
* No Response Headers

#### Response Parameters
* `status`(*string*) -- success OR failure
* `message`(*string*) -- messages from BT Stack
* `list`(*json*) -- A list of all previously paired and connected devices with their names and mac addresses, If status is 'success'.
#### Response Codes
code        | status  | message                                          |
------------|:-------:|:-------------------------------------------------|
200         | success | list of devices                                  |
400         | error   | Please include X-AccountToken in request headers |
401         | error   | Unauthorized request                             |
500         | error   | Error message from BT stack

<BT-API-09>

## BT-On/Off
This API call helps to turn on and turn off of BT

### POST /api/bluetooth/switch

#### Request Headers
* `X-AccountToken`(*String*, **required**) -- the token received from tronX server
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* `switch`(*bool*) -- True/False

#### Response Headers
* No Response Headers

#### Response Parameters
* `status`(*string*) -- success OR failure
* `message`(*string*) -- Message from BT-Stack

#### Response Codes
code        | status  | message                                          |
------------|:-------:|:-------------------------------------------------|
200         | success | Bluetooth Turned On/Off                          |
400         | error   | Please include X-AccountToken in request headers |
400         | failure | Invalid BT switch value                          |
401         | error   | Unauthorized request                             |
500         | failure | Error message from BT-Stack                      |
500         | error   | Shifu Internal Error                             |

<BT-API-10>

## BT Status
This API call helps to know the status of bluetooth, whether it is turned On or turned Off

### GET /api/bluetooth/switch/status

#### Request Headers
* `X-AccountToken`(*String*, **required**) -- the value received from tronX server
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* No request Parameters

#### Response Headers
* No Response Headers
 
#### Response Parameters
* `status`(*string*) -- success OR failure
* `bt_status`(*bool*) -- values: True or False
* `message`(*string*)  -- Error message from BT stack (This will be as a part of response incase of failure)

#### Response Codes
code        | status  | message                                          |
------------|:-------:|:-------------------------------------------------|
200         | success | Instead of message bt_status will come           |
400         | error   | Please include X-AccountToken in request headers |
401         | error   | Unauthorized request                             |
500         | failure | Error message from BT-Stack                      |
500         | error   | Shifu Internal Error                             |


Onboarding APIs
==============
This API call helps to register a device.
To register a device, the device either should be in not_registered mode or in configuration mode.
This API won't allow to register a device twice, until the device goes to configuration mode.
 
<LOGIN-API-01> 
 
## Login

### GET /api/auth/login

#### Request Headers
* `X-HomeTempToken`(*String*, **required**) -- value received from tronX server for hub login
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* No request Parameters

#### Response Headers
* No Response Headers

#### Response Parameters
* `message`(*string*) -- message from tronx
* `status`(*string*) -- success OR failure
* `device_profile_data`(*json*) -- device profile

```json
{
        "alarm_settings": {
            "is_alarm_gradually_increased": false,
            "selected_alarm_tones": "",
            "tones": [],
            "volume": 12
        },
        "devices": {
            "about_device": {
                "device_software_version": "5.3.2.1",
                "mac_address": "00:0a:95:9d:68:16",
                "serial_number": "4CE0460D0G"
            },
            "device_id": "THOMETESTBJKxeUpiY",
            "device_location": [
                "17.3753",
                "79.4744"
            ],
            "device_name": "Shifu",
            "os": "Openwrt",
            "time_zone": "IST-5:30"
        },
        "measurement_units": {
            "distance": "feet",
            "temperature": "celcius",
            "weight": "grams"
        },
        "reminder_settings": {
            "selected_reminder_tones": "",
            "tones": [],
            "volume": 12
        },
        "wifi_settings": {
            "legacy_router": {
                "ssid": "ssid1"
            },
            "router_settings": {
                "ssid": "ssid2"
            }
        }
    }
```


#### Response Codes
code        | status  | message                                          |
------------|:-------:|:-------------------------------------------------|
200         | success | Logged In Successfully                           |
400         | failure | Not Allowed to login in current format           |
400         | failure | Require X-HomeTempToken in headers               |
400         | failure | Please check your HomeToken                      |
408         | failure | Request TimeOut                                  |
500         | failure | Shifu Internal Error               	             |
600         | failure | Problem with Internet connection                 |
700         | failure | Unexpected Error                                 |


Shifu Router Configuration APIs
===============================
These APIs are allowed on all registration states.
If device is registered once, only a valid user can configure the router settings.

<RT-API-1>

## change_credentials
This API call helps to changes the existing credentials of access-point on Shifu.

### POST /api/router/change/credentials

#### Request Headers
* `X-AccountToken`(*String*, **required/optional**) -- required when device is in registered mode 
    												-- optional when device is in not_registered/ configuration mode	
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* `ssid`(*string*,**required**) -- Any Value
* `password`(*String*,**required**) -- Any string but length should be greater than 8
* `encryption`(*String*, **required**) -- As per the tronx_router module documentation

#### Response Headers
* No Response Headers

#### Response Parameters
* `message`(*string*) -- message from tronx
* `status`(*string*) -- success OR failure

#### Response Codes
code        | status  | message                                              |
------------|:-------:|:-----------------------------------------------------|
200         | success | AP Credentials changed successfully.                 |
400         | error   | Please include X-AccountToken in request headers     |
400         | error   | Password length should not be less that 8 characters.|
400         | error   | Encryption is not supported.                         |
401         | error   | Unauthorized request                                 |
500         | error   | ap credentials are not changed. Internal Error       | 

<RT-API-2>

## connect
This API connects shifu to legacy router.

### POST /api/router/shifu/Legacy/connect

#### Request Headers
* `X-AccountToken`(*String*, **required/optional**) -- required when device is in registered mode
    												-- optional when device is in not_registered/ configuration mode	
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* `ssid`(*String*, **required**) -- Any Name
* `encryption`(*String*, **optional**) -- As per the tronx_router module documentation
* `password`(*String*,**password**) -- Any string but length should be greater than 8

#### Response Headers
* No Response Headers

#### Response Parameters
* `message`(*string*) -- message from tronx
* `status`(*string*) -- success OR failure

#### Response Codes
code        | status  | message                                              |
------------|:-------:|:-----------------------------------------------------|
200         | success | Shifu is connected to + `ssid`                       |
400         | error   | Password length should not be less than 8 characters |
400         | error   | ssid is not in string format                         |
400         | error   | Please include X-AccountToken in request headers     |
401         | error   | Unauthorized request                                 |
500         | error   | Shifu is not connected to + `ssid`. Internal Error.  |

<RT-API-3>

## Legacy_ping
This API call helps to check whether hub is connected to Legacy router or not

### GET /api/router/shifu/Legacy/ping

#### Request Headers
* `X-AccountToken`(*String*, **required/optional**) -- required when device is in registered mode
    												-- optional when device is in not_registered/ configuration mode	
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* No Request Parameters

#### Response Headers
* No Response Headers

#### Response Parameters
* `message`(*string*) -- message from tronx
* `status`(*string*) -- success OR failure

#### Response Codes
code        | status  | message                                              |
------------|:-------:|:-----------------------------------------------------|
200         | success | Network Active          							 |
400         | failure | Please include X-AccountToken in request headers     |
401         | error   | Unauthorized request                                 |
500         | error   | Router is not connected 						     | 

<RT-API-4>

## Connectivity check
This API call helps to check whether Internet is Active or not

### GET /api/router/check/connectivity

#### Request Headers
* `X-AccountToken`(*String*, **required/optional**) -- required when device is in registered mode
    												-- optional when device is in not_registered/ configuration mode	
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* No Request Parameters

#### Response Headers
* No Response Headers

#### Response Parameters
* `message`(*string*) -- message from tronx
* `status`(*string*) -- success OR failure

#### Response Codes
code        | status  | message                                              |
------------|:-------:|:-----------------------------------------------------|
200         | success | Network Active          							 |
400         | failure | Please include X-AccountToken in request headers     |
401         | error   | Unauthorized request                                 |
500         | error   | Network Error           						     | 

<RT-API-5>

## Check eth
This API call helps to check whether ethernet cable (RJ45) is connected or not

### GET /api/router/check/eth

#### Request Headers
* `X-AccountToken`(*String*, **required/optional**) -- required when device is in registered mode
    												-- optional when device is in not_registered/ configuration mode	
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* No Request Parameters

#### Response Headers
* No Response Headers

#### Response Parameters
* `message`(*string*) -- message from tronx
* `status`(*string*) -- success OR failure

#### Response Codes
code        | status  | message                                              |
------------|:-------:|:-----------------------------------------------------|
200         | success | WAN is connected          							 |
400         | failure | Please include X-AccountToken in request headers     |
401         | error   | Unauthorized request                                 |
500         | error   | WAN is not connected           					     | 

<RT-API-6>
## Scan access points
This API call gives all the 2.4 GHz supported wifi access points. 

### GET /api/router/scan/aps

#### Request Headers
* `X-AccountToken`(*String*, **required/optional**) -- required when device is in registered mode
    												-- optional when device is in not_registered/ configuration mode	
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* No Request Parameters

#### Response Headers
* No Response Headers

#### Response Parameters
* `message`(*string*) -- all supported access points in an array
* `status`(*string*) -- success OR failure

#### Response Codes
code        | status  | message                                              |
------------|:-------:|:-----------------------------------------------------|
200         | success | WAN is connected          							 |
400         | failure | Please include X-AccountToken in request headers     |
401         | error   | Unauthorized request                                 |
500         | error   | WAN is not connected                                 | 

<RT-API-7>

## EBLR-connect
This API connects shifu to Legacy router in Ethernet backhaul

### POST /api/router/shifu/Legacy/EBLR/connect

#### Request Headers
* `X-AccountToken`(*String*, **required/optional**) -- required when device is in registered mode
    												-- optional when device is in not_registered/ configuration mode	
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* `ssid`(*String*, **required**) -- Any Name
* `password`(*String*,**password**) -- Any string but length should be greater than 8

#### Response Headers
* No Response Headers

#### Response Parameters
* `message`(*string*) -- message from tronx
* `status`(*string*) -- success OR failure

#### Response Codes
code        | status  | message                                                   |
------------|:-------:|:----------------------------------------------------------|
200         | success | Shifu is connected to Legacy Router with Ethernet backhaul|
400         | error   | Password length should not be less than 8 characters      |
400         | error   | ssid is not in string format                              |
400         | error   | Please include X-AccountToken in request headers          |
401         | error   | Unauthorized request                                      |
500         | error   | Shifu is not connected to Legacy. Shifu Internal Error    |  

<RT-API-8>

## WBLR-connect
This API connects the Shifu to Legacy router with Wifi backhaul

### POST /api/router/shifu/Legacy/WBLR/connect

#### Request Headers
* `X-AccountToken`(*String*, **required/optional**) -- required when device is in registered mode
    												-- optional when device is in not_registered/ configuration mode	
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* `ssid`(*String*, **required**) -- Any Name
* `password`(*String*,**password**) -- Any string but length should be greater than 8

#### Response Headers
* No Response Headers

#### Response Parameters
* `message`(*string*) -- message from tronx
* `status`(*string*) -- success OR failure

#### Response Codes
code        | status  | message                                                   |
------------|:-------:|:----------------------------------------------------------|
200         | success | Shifu is connected to Legacy Router with Wifi backhaul|
400         | error   | Password length should not be less than 8 characters      |
400         | error   | ssid is not in string format                              |
400         | error   | Please include X-AccountToken in request headers          |
401         | error   | Unauthorized request                                      |
500         | error   | Shifu is not connected to Legacy. Shifu Internal Error    | 

<RT-API-9>

## Range-Extender
This API configures the Shifu as Range Extender.

### GET /api/router/set_re

#### Request Headers
* `X-AccountToken`(*String*, **required/optional**) -- required when device is in registered mode
    												-- optional when device is in not_registered/ configuration mode	
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* No Request Parameters

#### Response Headers
* No Response Headers

#### Response Parameters
* `message`(*string*) -- message from tronx
* `status`(*string*) -- success OR failure

#### Response Codes
code        | status  | message                                                   |
------------|:-------:|:----------------------------------------------------------|
200         | success | Shifu is configured as RE                                 |
400         | error   | Please include X-AccountToken in request headers          |
401         | error   | Unauthorized request                                      |
500         | error   | Shifu is not configured as RE                             | 


<RT-API-10>

## QWRAP mode
This API configures the shifu as a repeater. In this mode shifu can acts as a RE for non-SON routers also.

### POST /api/router/shifu/Legacy/repeater/connect

#### Request Headers
* `X-AccountToken`(*String*, **required/optional**) -- required when device is in registered mode
    												-- optional when device is in not_registered/ configuration mode	
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* `ssid`(*String*, **required**) -- Any Name
* `password`(*String*,**password**) -- Any string but length should be greater than 8

#### Response Headers
* No Response Headers

#### Response Parameters
* `message`(*string*) -- message from tronx
* `status`(*string*) -- success OR failure

#### Response Codes
code        | status  | message                                                   |
------------|:-------:|:----------------------------------------------------------|
200         | success | Shifu is connected to Legacy Router in Repeater mode      |
400         | error   | Password length should not be less than 8 characters      |
400         | error   | ssid is not in string format                              |
400         | error   | Please include X-AccountToken in request headers          |
401         | error   | Unauthorized request                                      |
500         | error   | Shifu is not connected to Legacy. Shifu Internal Error    |

<RT-API-11>

## Legacy_status
This API call helps to check whether hub is connected to Legacy router or not

### GET /api/router/shifu/Legacy/status

#### Request Headers
* `X-AccountToken`(*String*, **required/optional**) -- required when device is in registered mode
    												-- optional when device is in not_registered/ configuration mode	
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* No Request Parameters

#### Response Headers
* No Response Headers

#### Response Parameters
* `message`(*string*) -- message from tronx
* `status`(*string*) -- success OR failure

#### Response Codes
code        | status  | message                                              |
------------|:-------:|:-----------------------------------------------------|
200         | success | Network Active          							 |
400         | failure | Please include X-AccountToken in request headers     |
401         | error   | Unauthorized request                                 |
500         | error   | Shifu Internal Error/ Network Error                  | 

<RT-API-12>

## Legacy_network 
This API disables the shifu's DHCP server.

### GET /api/router/shifu/Legacy/network

#### Request Headers
* `X-AccountToken`(*String*, **required/optional**) -- required when device is in registered mode
    												-- optional when device is in not_registered/ configuration mode	
* `content-type`(*String*,**required**) -- application/json

#### Request Parameters
* No Request Parameters

#### Response Headers
* No Response Headers

#### Response Parameters
* `message`(*string*) -- message from tronx
* `status`(*string*) -- success OR failure

#### Response Codes
code        | status  | message                                              |
------------|:-------:|:-----------------------------------------------------|
200         | success | Successful network reload          		     		 |
400         | failure | Please include X-AccountToken in request headers     |
401         | error   | Unauthorized request                                 |
500         | error   | Shifu internal error                                 |


Offline things control APIs
===========================

<O-API-1>

## Get thing list

Get list of all things from Shifu in local network with updated status

### GET /api/thing/list

#### Request Headers

- `X-AccountToken`(*String*, **required**) 
- `content-type`(*String*,**required**) -- application/json

#### Request Parameters

- No Request Parameters

#### Response Headers

- No Response Headers

#### Response Parameters

- `message`(*json*) -- message as string
- `status`(*string*) -- success OR failure
- `things`(*json*) -- Json array of things list

#### Response Codes

| code | status  | message                                          |
| ---- | ------- | ------------------------------------------------ | 
| 200  | success | Json array of things                             |
| 400  | failure | Please include X-AccountToken in request headers |
| 401  | error   | Unauthorized request                             |
| 500  | error   | DM is not in listening mode                      |
| 503  | error   | DM is not responding within timeout              |
| 503  | error   | Invalid data sent from DM                        |

<O-API-2>

## Control things

This API call will control things using Offline mechanishm 

### POST /api/thing/control

#### Request Headers

- `X-AccountToken`(*String*, **required**) -- required when device is in registered mode
- `content-type`(*String*,**required**) -- application/json

#### Request Parameters

- `things`(*json*, **required**) -- required for controlling things and their properties (Array size should be one only)
- `content-type`(*String*,**required**) -- application/json

```json
{
  "things": [
    {
      "thing_id": "ZW_4206534619_9",
      "entity_id": "0_17_1",
      "property_value": "0",
      "property_type": "SWITCH",
      "property_name": "on_off"
    }
  ]
}
```


#### Response Headers

- No Response Headers

#### Response Parameters

- `message`(*string*/json) -- message for things control status
- `status`(*string*) -- success OR failure

#### Response Codes

| code | status  | message                                          |
| ---- | ------- | ------------------------------------------------ | 
| 200  | success | Things updated detail (*json*)                   |
| 400  | failure | Please include X-AccountToken in request headers |
| 401  | error   | Unauthorized request                             |
| 500  | error   | DM is not in listening mode                      |
| 503  | error   | DM is not responding within timeout              |
| 503  | error   | Invalid data sent from DM                        |
| 503  | error   | Control event Failed to execute                  |


<O-API-3>

## Exclude z-wave device

This API call will control things using Offline mechanishm 

### GET /api/thing/zwave/exclusion

#### Request Headers

- `X-AccountToken`(*String*, **required**) -- required when device is in registered mode
- `content-type`(*String*,**required**) -- application/json

#### Request Parameters

- No Request Headers

#### Response Headers

- No Response Headers

#### Response Parameters

- `message`(*string*/json) -- message for things control status
- `status`(*string*) -- success OR failure
- `thing_id`(*string*) -- thingd_id of z-wave device excluded

#### Response Codes

| code | status  | message                                          |
| ---- | ------- | ------------------------------------------------ | 
| 200  | success | THING EXCULDED SUCCESSFULLY                      |
| 400  | failure | Please include X-AccountToken in request headers |
| 401  | error   | Unauthorized request                             |
| 500  | error   | DM is not in listening mode                      |
| 503  | error   | DM is not responding within timeout              |
| 503  | error   | Invalid data sent from DM                        |
| 503  | error   | TIMEOUT DEVICE NOT RESPONDING                    |


## CHANGELOG
### Version 1.4.3 [12-06-2018]

#### Added
- /api/thing/zwave/exclusion is added

####Changed
- /api/thing/control and /api/thing/list is changed

####Deleted

### Version 1.4.2 [06-06-2018]

#### Added
- /api/bluetooth/switch is added
- /api/bluetooth/switch/status is added

####Changed
- Bug #16704 and #15705 is fixed.

####Deleted

### Version 1.4.1 [28-05-2018]

#### Added
- /api/bluetooth/devices/list is added

####Changed
- Bug #16241 is fixed.

####Deleted

### Version 1.4 [23-05-2018]

#### Added
- POST /api/router/shifu/Legacy/repeater/connect API is added : this configures Shifu in QWRAP mdoe.
- GET /api/router/shifu/Legacy/status API is added 
- GET /api/router/shifu/Legacy/network API is added
- Shifu as CAP (Wi-Fi SON Mode - Ethernet Backhaul in between Shifu and Legacy Rotuer) : In the presence of DHCP server and Pass-through from legacy router 
   POST /api/router/shifu/Legacy/EBLR/connect
- Shifu as CAP (Wi-Fi SON Mode - Wi-Fi Backhaul in between Shifu and Legacy Rotuer) : In the presence of DHCP server and Pass-through from legacy router 
   POST /api/router/shifu/Legacy/WBLR/connect
- Shifu as RE (Wi-Fi SON Mode - Wi-Fi Backhaul  in between Shifu CAP and Shifu RE) 
   GET /api/router/set_re

####Changed
- Now when GET /api/bluetooth/discovery/start api is called, it also sets shifu into discoverable mode.

####Deleted

### Version 1.3.1 [17-05-2018]

#### Added

####Changed

- Traceback logging implemented in python modules in Openwrt.
- Changing device profile format and uploading to tronx.

####Deleted

### Version 1.3 [07-05-2018]

#### Added

####Changed

- Changed socket messge format from string to json in things_control.py.
- Changed script for Legacy router connect in router_config.py.
- HMCONTROLTIMEOUT variable added in config.py.
- Changing the response format for BT discovery such that it now shows which BT devices are connected, paired or not_paired.
- Home manager is restarted every time on-boarding process is done.

####Deleted

### Version 1.2 [03-04-2018]

#### Added


- GET /api/thing/list
- POST /api/thing/control
- GET /api/scan/aps (scans only 2.4 GHz access points)

### Changed 
- scripts are changed for /legacy/connect API, now Shifu will connect to only router's 2.4 GHz interface.

### Version 1.1 [16-02-2018]

####Added
- POST /api/router/change/credentials is added
- GET /api/router/check/connectivity is added
- GET /api/router/check/eth is added
- GET /api/router/shifu/Legacy/ping is added

####Changed
- POST /api/bluetooth/request/pairing is changed to POST /api/bluetooth/device/connect
- After successful login login API returns the device profile

####Deleted
- from all api response parameters httpcode field is removed
- GET /api/router/shifu/legacy/init is deleted
- POST /api/router/change/ssid is deleted
- POST /api/router/change/password is deleted
- POST /api/router/change/encryption is deleted
- GET /api/bluetooth/discovery/stop is deleted

### Version 1.0

####Added
####Changed
####Deleted
