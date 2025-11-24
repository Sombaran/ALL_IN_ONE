#!/usr/bin/python

"""
Unit Test:

Switch on the Bluetooth Adapter power

"""

import Python_BT
status=Python_BT.bt_switch_status()
print status
status=Python_BT.bt_switch(True)
print status
status=Python_BT.bt_switch_status()
print status
