#!/usr/bin/python

"""
Unit Test:

changed the discoverable time and sets the discoverable time on for specified time

"""

#__author__ = "Adarsh B U and Harish Gaddameedi"
#__porting__ = "Adarsh B U and Praneet Kaur"
#__copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
#__credits__ = ["Adarsh BU, Praneet Kaur and Harish Gaddameedi"]
#__maintainer__ = "adarsh BU"
#__email__ = "adarsh.umesh@smartron.com"

import Python_BT
status=Python_BT.bt_discoverable(30)
print status
