#!/usr/bin/python

"""
Unit Test:

Runs the specified AVRCP commands with the specified Mac-Address

"""

#__author__ = "Adarsh B U and Harish Gaddameedi"
#__porting__ = "Adarsh B U and Praneet Kaur"
#__copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
#__credits__ = ["Adarsh BU, Praneet Kaur and Harish Gaddameedi"]
#__maintainer__ = "adarsh BU"
#__email__ = "adarsh.umesh@smartron.com"

import config
import Python_BT
import time

status=Python_BT.bt_avrcp_command(config.mac_addr,"play")
print "play"
print status
time.sleep(15)

status=Python_BT.bt_avrcp_command(config.mac_addr,"pause")
print "pause"
print status
time.sleep(15)

status=Python_BT.bt_avrcp_command(config.mac_addr,"play")
print "play"
print status
time.sleep(15)

status=Python_BT.bt_avrcp_command(config.mac_addr,"restart")
print "restart"
print status
time.sleep(15)

status=Python_BT.bt_avrcp_command(config.mac_addr,"next")
print "next"
print status
time.sleep(15)

status=Python_BT.bt_avrcp_command(config.mac_addr,"previous")
print "previous"
print status
time.sleep(15)

status=Python_BT.bt_avrcp_command(config.mac_addr,"stop")
print "stopped"
print status
