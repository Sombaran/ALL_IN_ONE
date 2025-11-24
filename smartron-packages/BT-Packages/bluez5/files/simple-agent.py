#!/usr/bin/python

from __future__ import absolute_import, print_function, unicode_literals

from optparse import OptionParser
import sys
import dbus
import dbus.service
import dbus.mainloop.glib
import Python_BT
import socket
import struct
import zmq,time
import subprocess
import json
import signal

try:
  from gi.repository import GObject
except ImportError:
  import gobject as GObject
import bluezutils

BUS_NAME = 'org.bluez'
AGENT_INTERFACE = 'org.bluez.Agent1'
AGENT_PATH = "/test/agent"
# Redmine 20283 Start: setting AVRCP Source Volume Maximum value to 99
MAX_SOURCE_VOLUME = 99
# Redmine 20283 Stop

bus = None
device_obj = None
dev_path = None

connected=False
list_devices={}

log = Python_BT.log_init()

# va_port="8800"
LED_port=7891

class LedControl:
    """
    Led Control class for initialization to Led Socket Server and to send message
    :param: port : port which LED server is listening
    :type: port <4 digit int or string>
    """

    def __init__(self, port):
        """
        initialize Led socket client with given port
        :param port:
        """
        self.host = 'localhost'
        self.port = port
        self.socket = None

    def connect(self):
        """
        Will connect to LED socket server through UDP Datagram packet, if it failed then return error
        """
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        except socket.error:
            return 'Failed to create socket'

    def send_msg(self, event, state, value):
        """
        Send message for LED event using class variables
        :param event: type event for LED
        :param state: Switch ON / OFF
        :type state: int {1,0}
        """
        values = (PATTERN[event], state, value)
        packer = struct.Struct('I ? I')
        if not self.socket:
            return "socket is not initialized"
        else:
            self.socket.sendto(packer.pack(*values), (self.host, self.port))
            self.socket.close()

PATTERN = dict(
	LED_OFF                 =    0,            # LED OFF  
	LED_VOLUME              =    (1 << 0),     # Volume , (volume up and down same priority) 
	LED_ACTION              =    (1 << 1),     # Action button
	LED_ERROR               =    (1 << 2),     # Error
	LED_WAKEWORD            =    (1 << 3),     # Wake word
	LED_ALARM               =    (1 << 4),     # Alarm
	LED_QUERY_PROCESSING    =    (1 << 5),     # Query processing
	LED_QUERY_RESPONSE      =    (1 << 6),     # Query Response
	LED_DEVICE_CONNECTED    =    (1 << 7),     # Device Discovery
	LED_DEVICE_DISCOVERY    =    (1 << 8),     # Device Control
	LED_CONNECTED_AS_ROUTER =    (1 << 9),     # Connected as Router
	LED_MIC_MUTE            =    (1 << 10),    # Mic mute
	LED_SETUP_MODE          =    (1 << 11),    # device registration mode
	LED_VOICE_VOLUME        =    (1 << 12),    # Volume change through voice
	LED_MICMUTE_OFF         =    (1 << 13),    # Mic UnMute after Mute
	LED_BLUTOOTH_DISCOVERY  =    (1 << 14),    # Blutooth Discovery  
	LED_BLUTOOTH_CONNECTED  =    (1 << 15),    # Blutooth Connected
	LED_NETWORK_RESET       =    (1 << 16),    # Network Reset
	LED_WPS_MODE            =    (1 << 17)     # WPS Mode
)

def connect_TTS_func(signum, frame):
	global connected_mac
	dm_msg={}
	dm_msg[connected_mac]=btname
	Python_BT.dm_socket(dm_msg,"Device","connected")
	# Redmine 20658 start: BT connected announcement plays with BT connected pattern
	led = LedControl(LED_port)
	led.connect()
	led.send_msg("LED_BLUTOOTH_CONNECTED", 1, 0)
	# Redmine 20658 end
	Python_BT.va_socket(str("BT VOICE Connected to ")+str(btname)+str(" Bluetooth"),)
	# Redmine 20283 Start: Test Case of RID_20283 After connection from Simple-agent setting volume to 99
	if "SOURCE" in Python_BT.get_device_property(connected_mac,"UUIDs"):
		subprocess.check_output("pactl set-source-volume bluez_source "+str(MAX_SOURCE_VOLUME)+"%",shell=True)
	# Redmine 20283 Stop

def property_changed(interface, changed, invalidated, path):
	global connected
	global btname
	iface = interface[interface.rfind(".") + 1:]
	list_devices=Python_BT.bt_list_connected_dev()
	# if iface=="Adapter1":
	# 	for name,value in changed.iteritems():
	# 		if name=="Discoverable" and value==0:
	# 			Python_BT.dm_socket("NA","Status","OFF")
	if bool(list_devices):
		connected = True
		# Redmine 19936: Modifications based on list_devices data format
		key = list_devices["Device_macId"]
		# Redmine 20053 Start: If the device is already in forget state, Name will be displayed as Mac_addr
		try:
			btname = Python_BT.get_device_property(key,"Name")
		except:
			btname = key
		# Redmine 20053 Stop
		status=Python_BT.get_mediaplayer_property(key,"Status")
		if iface=="MediaPlayer1" and status=="playing":
			for name, value in changed.iteritems():
				if name=="Status":
					Python_BT.va_socket(str("BT COMMAND STOP"),)
		if iface=="MediaTransport1":
			for name, value in changed.iteritems():
				if name=="Volume":
					led = LedControl(LED_port)
					led.connect()
					if(value<MAX_SOURCE_VOLUME):
						led.send_msg("LED_VOLUME", 1, value/10)
					else:
						led.send_msg("LED_VOLUME", 1, 10)
						value = MAX_SOURCE_VOLUME
					subprocess.check_output("pactl set-source-volume bluez_source "+str(value)+"%",shell=True)
	elif (not bool(list_devices)) and connected:
		Python_BT.va_socket(str("BT STATUS NONE"),)
		connected=False
		log.debug(str("BT: SA: Disconnected from ")+str(btname))

def ask(prompt):
	try:
		return raw_input(prompt)
	except:
		return input(prompt)

def set_trusted(path):
	props = dbus.Interface(bus.get_object("org.bluez", path),
					"org.freedesktop.DBus.Properties")
	props.Set("org.bluez.Device1", "Trusted", dbus.Boolean(1))

def dev_connect(path):
	dev = dbus.Interface(bus.get_object("org.bluez", path),
							"org.bluez.Device1")
	dev.Connect()

class Rejected(dbus.DBusException):
	_dbus_error_name = "org.bluez.Error.Rejected"

class Agent(dbus.service.Object):
	exit_on_release = True

	def set_exit_on_release(self, exit_on_release):
		self.exit_on_release = exit_on_release

	@dbus.service.method(AGENT_INTERFACE,
					in_signature="", out_signature="")
	def Release(self):
		print("Release")
		if self.exit_on_release:
			mainloop.quit()

	@dbus.service.method(AGENT_INTERFACE,
					in_signature="os", out_signature="")
	def AuthorizeService(self, device, uuid):
		print("AuthorizeService (%s, %s)" % (device, uuid))
		#authorize = ask("Authorize connection (yes/no): ")
		#if (authorize == "yes"):
		global connected_mac
		global btname
		list_devices=Python_BT.bt_list_connected_dev()
		key=":".join(device[20:37].split("_"))
		# Redmine 19936: Modifications of list_devices due to the data format
		if not bool(list_devices):
			log.debug("BT: SA: First profile Connecting with "+key)
			btname = Python_BT.get_device_property(key,"Name")
			uuid_string = Python_BT.get_device_property(key,"UUIDs")
			Python_BT.va_socket(uuid_string,)
			file=open("/etc/bluetooth/lastconnect_macaddr","w+")
			file.write(str(key)+str(" 1"))                                     
			file.close()
			# Redmine 19739 Start: Connected device will be added to Last 5 Connected List
			Python_BT.five_pair_list.add(str(key))
			# Redmine 19739 Stop
			#Python_BT.bt_switch(False)
			log.debug(str("[BT]:[AUTO]: SA CONNECTED ")+str(key))
			connected_mac=key
			signal.signal(signal.SIGALRM, connect_TTS_func)
			signal.alarm(2)
			return
		elif(key == list_devices["Device_macId"]):
			uuid_string=Python_BT.get_device_property(key,"UUIDs")
			Python_BT.va_socket(uuid_string,)
			log.debug("BT: SA: Connected to "+uuid+" of "+key)
			return
		dm_msg={}
		dm_msg[key]=Python_BT.get_device_property(key,"Name")
		Python_BT.dm_socket(dm_msg,"Device","paired")
		raise Rejected("Connection rejected")

	@dbus.service.method(AGENT_INTERFACE,
					in_signature="o", out_signature="s")
	def RequestPinCode(self, device):
		print("RequestPinCode (%s)" % (device))
		set_trusted(device)
		#return ask("Enter PIN Code: ")
		return "0000"

	@dbus.service.method(AGENT_INTERFACE,
					in_signature="o", out_signature="u")
	def RequestPasskey(self, device):
		print("RequestPasskey (%s)" % (device))
		set_trusted(device)
		passkey = ask("Enter passkey: ")
		return dbus.UInt32(passkey)

	@dbus.service.method(AGENT_INTERFACE,
					in_signature="ouq", out_signature="")
	def DisplayPasskey(self, device, passkey, entered):
		print("DisplayPasskey (%s, %06u entered %u)" %
						(device, passkey, entered))

	@dbus.service.method(AGENT_INTERFACE,
					in_signature="os", out_signature="")
	def DisplayPinCode(self, device, pincode):
		print("DisplayPinCode (%s, %s)" % (device, pincode))

	@dbus.service.method(AGENT_INTERFACE,
					in_signature="ou", out_signature="")
	def RequestConfirmation(self, device, passkey):
		print("RequestConfirmation (%s, %06d)" % (device, passkey))
		confirm = ask("Confirm passkey (yes/no): ")
		if (confirm == "yes"):
			set_trusted(device)
			return
		raise Rejected("Passkey doesn't match")

	@dbus.service.method(AGENT_INTERFACE,
					in_signature="o", out_signature="")
	def RequestAuthorization(self, device):
		print("RequestAuthorization (%s)" % (device))
		auth = ask("Authorize? (yes/no): ")
		if (auth == "yes"):
			return
		raise Rejected("Pairing rejected")

	@dbus.service.method(AGENT_INTERFACE,
					in_signature="", out_signature="")
	def Cancel(self):
		print("Cancel")

def pair_reply():
	print("Device paired")
	set_trusted(dev_path)
	dev_connect(dev_path)
	mainloop.quit()

def pair_error(error):
	err_name = error.get_dbus_name()
	if err_name == "org.freedesktop.DBus.Error.NoReply" and device_obj:
		print("Timed out. Cancelling pairing")
		device_obj.CancelPairing()
	else:
		print("Creating device failed: %s" % (error))


	mainloop.quit()

if __name__ == '__main__':
	dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)

	bus = dbus.SystemBus()

	bus.add_signal_receiver(property_changed, bus_name="org.bluez",
			dbus_interface="org.freedesktop.DBus.Properties",
			signal_name="PropertiesChanged",
			path_keyword="path")

	#capability = "KeyboardDisplay"
	capability = "NoInputNoOutput"
	parser = OptionParser()
	parser.add_option("-i", "--adapter", action="store",
					type="string",
					dest="adapter_pattern",
					default=None)
	parser.add_option("-c", "--capability", action="store",
					type="string", dest="capability")
	parser.add_option("-t", "--timeout", action="store",
					type="int", dest="timeout",
					default=60000)
	(options, args) = parser.parse_args()
	if options.capability:
		capability  = options.capability

	path = "/test/agent"
	agent = Agent(bus, path)

	mainloop = GObject.MainLoop()

	obj = bus.get_object(BUS_NAME, "/org/bluez");
	manager = dbus.Interface(obj, "org.bluez.AgentManager1")
	manager.RegisterAgent(path, capability)

	print("Agent registered")

	# Fix-up old style invocation (BlueZ 4)
	if len(args) > 0 and args[0].startswith("hci"):
		options.adapter_pattern = args[0]
		del args[:1]

	if len(args) > 0:
		device = bluezutils.find_device(args[0],
						options.adapter_pattern)
		dev_path = device.object_path
		agent.set_exit_on_release(False)
		device.Pair(reply_handler=pair_reply, error_handler=pair_error,
								timeout=60000)
		device_obj = device
	else:
		manager.RequestDefaultAgent(path)

	mainloop.run()

	#adapter.UnregisterAgent(path)
	#print("Agent unregistered")