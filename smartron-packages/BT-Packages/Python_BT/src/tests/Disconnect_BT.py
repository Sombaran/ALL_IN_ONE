#!/usr/bin/python

"""
Unit Test:

To disconnect the specified Bluetooth device
Device mac_addr need to mentioned in config.py

"""

#__author__ = "Adarsh B U and Harish Gaddameedi"
#__porting__ = "Adarsh B U and Praneet Kaur"
#__copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
#__credits__ = ["Adarsh BU, Praneet Kaur and Harish Gaddameedi"]
#__maintainer__ = "adarsh BU"
#__email__ = "adarsh.umesh@smartron.com"

import config
import Python_BT
status=Python_BT.bt_process(config.mac_addr,"disconnect")
print status
