#!/usr/bin/python

"""
Unit Test:

Connects the specified mac-address with bluetooth adapter

"""

#__author__ = "Adarsh B U and Harish Gaddameedi"
#__porting__ = "Adarsh B U and Praneet Kaur"
#__copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
#__credits__ = ["Adarsh BU, Praneet Kaur and Harish Gaddameedi"]
#__maintainer__ = "adarsh BU"
#__email__ = "adarsh.umesh@smartron.com"

import config
import Python_BT
import sys
try:
  status=Python_BT.bt_process(config.mac_addr,"connect")
  print status
except Exception:
  error_type,error_instance,traceback=sys.exc_info()
  print error_instance
