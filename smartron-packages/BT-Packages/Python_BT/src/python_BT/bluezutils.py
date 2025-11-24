#!/usr/bin/python

"""Bluetooth_Manager: Supporting for A2DP & AVRCP profiles"""

__author__ = "Adarsh B U and Harish Gaddameedi"
__porting__ = "Adarsh B U and Praneet Kaur"
__copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
__credits__ = ["Adarsh B U, Praneet Kaur and Harish Gaddameedi"]
__maintainer__ = "adarsh B U"
__email__ = "adarsh.umesh@smartron.com"

import dbus
import logging
import logging.handlers
import config
import btlogger

class BTError(Exception):
  """Base class for BT_Exceptions"""
  pass

## initialising the logger from btlogger
log = btlogger.log_init()

## Getting D-Bus Managed Objects from the Bluez Object Manager
def get_managed_objects():
	bus = dbus.SystemBus()
	manager = dbus.Interface(bus.get_object("org.bluez", "/"),
				"org.freedesktop.DBus.ObjectManager")
	return manager.GetManagedObjects()

def find_adapter(pattern=None):
  """Function returns the adapter object paths from the Object Manager"""
  return find_adapter_in_objects(get_managed_objects(), pattern)

def find_adapter_in_objects(objects, pattern=None):
  """Function takes the Managed Objects, check for the Adapter Interfaces and return the Object path"""
  bus = dbus.SystemBus()
  for path, ifaces in objects.iteritems():
    adapter = ifaces.get(config.ADAPTER_INTERFACE)
    if adapter is None:
      continue
    if not pattern or pattern == adapter["Address"] or \
            path.endswith(pattern):
      obj = bus.get_object(config.SERVICE_NAME, path)
      return dbus.Interface(obj, config.ADAPTER_INTERFACE)
  log.critical('Bluetooth adapter not found')
  raise BTError("Bluetooth adapter not found")

def find_device(device_address, adapter_pattern=None):
  """Finding return the device object paths from the Adapter Interface in Object Manager"""
  return find_device_in_objects(get_managed_objects(), device_address,
								adapter_pattern)

def find_device_in_objects(objects, device_address, adapter_pattern=None):
  """Function takes the Managed Objects, check for the Device Interfaces and return the Device Object path"""
  bus = dbus.SystemBus()
  path_prefix = ""
  if adapter_pattern:
    adapter = find_adapter_in_objects(objects, adapter_pattern)
    path_prefix = adapter.object_path
  for path, ifaces in objects.iteritems():
    device = ifaces.get(config.DEVICE_INTERFACE)
    if device is None:
      continue
    if ((device["Address"] == device_address) and path.startswith(path_prefix)):
      obj = bus.get_object(config.SERVICE_NAME, path)
      return dbus.Interface(obj, config.DEVICE_INTERFACE)
  log.error('Bluetooth device not found')
  raise BTError("Bluetooth device not found")

def find_media(pattern=None):
  """Function returns the Media object paths from the Object Manager"""
  return find_media_in_objects(get_managed_objects(), pattern)

def find_media_in_objects(objects, pattern=None):
  """Function takes the Managed Objects, check for the Media Interfaces and return the Object path"""
  bus = dbus.SystemBus()
  for path, ifaces in objects.iteritems():
    adapter = ifaces.get(config.MEDIA_INTERFACE)
    if adapter is None:
      continue
    if not pattern or pattern == adapter["Address"] or \
        path.endswith(pattern):
      obj = bus.get_object(config.SERVICE_NAME, path)
      return dbus.Interface(obj, config.MEDIA_INTERFACE)
  log.error('BT Media Not found in objects')
  raise BTError("Bluetooth media not found")

def find_mediaplayer(device_address, adapter_pattern=None):
  """Finding return the device object paths from the Adapter Interface in Object Manager"""
  return find_mediaplayer_in_objects(get_managed_objects(), device_address, adapter_pattern)

def find_mediaplayer_in_objects(objects, device_address, adapter_pattern=None):
  """Function takes the Managed Objects, check for the Adapter Interfaces and return the Mediaplayer Object path"""
  bus = dbus.SystemBus()
  path_prefix = ""
  if adapter_pattern:
    adapter = find_adapter_in_objects(objects, adapter_pattern)
    path_prefix = adapter.object_path
  for path, ifaces in objects.iteritems():
    device = ifaces.get(config.MEDIAPLAYER_INTERFACE)
    if device is None:
      continue
    if(path.startswith(path_prefix)):
      obj = bus.get_object(config.SERVICE_NAME, path)
      return dbus.Interface(obj, config.MEDIAPLAYER_INTERFACE)
  log.error('Bluetooth Mediaplayer Not found')
  raise BTError("Bluetooth mediaplayer not found")
