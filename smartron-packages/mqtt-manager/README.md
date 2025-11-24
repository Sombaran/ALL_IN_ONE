# mqtt-manager

This application will establish communication based on MQTT protocol using mqtt paho libraries

### Prerequisites

Compactible with openWRT build environment.

### Installing

This package will be automatically selected during project building

If User want to execute mannually,
In openWRT Build Environment go to qsdk/ folder perform below operation.

To build project:
make package/mqtt-manager/compile V=s

To clean project:
make package/mqtt-manager/clean V=s

## Running the tests

All test scripts are placed in openWRT's /usr/sbin folder during building the project.

Run python MQTT_BT_test.py
This test program is to evaluate individual commands with error scenarios from clients on user intervention

Run python MQTT_BT_test_loop.py
This test program is to evaluate individual commands with error scenarios from clients which will run for indefinite loop




