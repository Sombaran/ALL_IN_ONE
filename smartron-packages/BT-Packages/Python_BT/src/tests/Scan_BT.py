#!/usr/bin/python

"""
Unit Test:

Starts Scan to discovery surrounding bluetooth devices

"""

#__author__ = "Adarsh B U and Harish Gaddameedi"
#__porting__ = "Adarsh B U and Praneet Kaur"
#__copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
#__credits__ = ["Adarsh BU, Praneet Kaur and Harish Gaddameedi"]
#__maintainer__ = "adarsh BU"
#__email__ = "adarsh.umesh@smartron.com"

import Python_BT
import sys
import time
#try:
msg=Python_BT.bt_start_discovery()
if msg is "Success":
  for i in range(1,27):
    time.sleep(1)
    a=Python_BT.print_discovery()
    print a  # (key: string, values: utf-8) of this dictionary are in utf-8 format
else:
  print msg
#except:
#  error_type, error_instance, traceback = sys.exc_info()
#  raise error_instance
