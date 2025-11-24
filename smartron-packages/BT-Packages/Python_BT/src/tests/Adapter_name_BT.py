#!/usr/bin/python

"""
Unit Test:

Rename of changes the name of the BT adapter with the custom name

"""

#__author__ = "Adarsh B U and Harish Gaddameedi"
#__porting__ = "Adarsh B U and Praneet Kaur"
#__copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
#__credits__ = ["Adarsh BU, Praneet Kaur and Harish Gaddameedi"]
#__maintainer__ = "adarsh BU"
#__email__ = "adarsh.umesh@smartron.com"

import Python_BT
import config
msg=Python_BT.bt_adapter_name(config.PINAME)
print msg
