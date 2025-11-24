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
