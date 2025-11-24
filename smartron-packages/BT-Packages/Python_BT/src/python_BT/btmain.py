#!/usr/bin/python

"""Bluetooth_Manager: Supporting Shifu-thub with A2DP & AVRCP profiles"""

__author__ = "Adarsh B U and Harish Gaddameedi"
__porting__ = "Adarsh B U and Praneet Kaur"
__copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
__credits__ = ["Adarsh B U, Praneet Kaur and Harish Gaddameedi"]
__maintainer__ = "adarsh B U"
__email__ = "adarsh.umesh@smartron.com"

import threading, bluezutils, time, sys, dbus, dbus.mainloop.glib, logging, logging.handlers,config
import btlogger, socket, struct
import zmq,subprocess,json,signal
import ast

try:
    from gi.repository import GObject
except ImportError:
    import gobject as GObject


class BTError(Exception):
    """Base class for BT_Exceptions"""
    pass

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

    def send_msg(self, event, state):
        """
        Send message for LED event using class variables
        :param event: type event for LED
        :param state: Switch ON / OFF
        :type state: int {1,0}
        """
        values = (PATTERN[event], state, chr(0))
        packer = struct.Struct('I ? c')
        if not self.socket:
            return "socket is not initialized"
        else:
            self.socket.sendto(packer.pack(*values), (self.host, self.port))
            self.socket.close()

PATTERN = dict(
    LED_OFF                 =    0,             # LED OFF  
    LED_VOLUME              =    (1 << 0),      # Volume , (volume up and down same priority) 
    LED_ACTION              =    (1 << 1),      # Action button
    LED_ERROR               =    (1 << 2),      # Error
    LED_WAKEWORD            =    (1 << 3),      # Wake word
    LED_ALARM               =    (1 << 4),      # Alarm
    LED_QUERY_PROCESSING    =    (1 << 5),      # Query processing
    LED_QUERY_RESPONSE      =    (1 << 6),      # Query Response
    LED_DEVICE_CONNECTED    =    (1 << 7),      # Device Discovery
    LED_DEVICE_DISCOVERY    =    (1 << 8),      # Device Control
    LED_CONNECTED_AS_ROUTER =    (1 << 9),      # Connected as Router
    LED_MIC_MUTE            =    (1 << 10),     # Mic mute
    LED_SETUP_MODE          =    (1 << 11),     # device registration mode
    LED_VOICE_VOLUME        =    (1 << 12),     # Volume change through voice
    LED_MICMUTE_OFF         =    (1 << 13),     # Mic UnMute after Mute
    LED_BLUTOOTH_DISCOVERY  =    (1 << 14),     # Blutooth Discovery  
    LED_BLUTOOTH_CONNECTED  =    (1 << 15),     # Blutooth Connected
    LED_NETWORK_RESET       =    (1 << 16),     # Network Reset
    LED_WPS_MODE            =    (1 << 17)      # WPS Mode
)

# Redmine 19739 Start: Class to Store the Last Five Connected Device
class five_paired_object(object):
    def open(self):
        self.filep = open("/etc/bluetooth/five_pair_list","r+")
        try:
            self.five_pair_list = ast.literal_eval(self.filep.read())
        except:
            self.five_pair_list = []

    def add(self, mac_addr):
        try:
            self.open()
            if mac_addr not in self.five_pair_list:
                if(len(self.five_pair_list) < 5):
                    self.five_pair_list.append(mac_addr)
                else:
                    self.five_pair_list.append(mac_addr)
                    del(self.five_pair_list[0])
            else:
                del(self.five_pair_list[self.five_pair_list.index(mac_addr)])
                self.five_pair_list.append(mac_addr)
            self.close()
            return True                                                                                                        
        except:
            return False

    def read(self):
        self.open()
        self.filep.close()
        return self.five_pair_list

    def remove(self, mac_addr):
        try:
            self.open()
            try:
                del(self.five_pair_list[self.five_pair_list.index(mac_addr)])
            except:
                log.debug("BT: device is not in five_pair_list")
            self.close()
            return True
        except:
            return False
        
    def close(self):
        self.filep.seek(0)
        self.filep.truncate()
        self.filep.write(str(self.five_pair_list))
        self.filep.close()

## To store the list of Last 5 Paired devices
five_pair_list = five_paired_object()

# Redmine 19739 Stop

## To store the list of external BT scanned devices
discovered_bt = {} 

## To initialize the logger using btlogger module
log = btlogger.log_init()

va_port = "8800"
dm_port = "7003"
LED_port = 7891
DISCOVERABLE_TIMEOUT=60 
DISCOVERABLE_INFINITE=0 # zero defines always ON
SCAN_LIST_INTERVAL=3
# Redmine 20283 Start: Setting connection source volume maximum to 99
MAX_SOURCE_VOLUME = 99
# Redmine 20283 Stop

# Redmine 19936 Start: Added Common format for Mobile Agent and MQTT service
def dm_ma_list_format(msg, device_status):
    tag_data=[]
    for key,value in msg.iteritems():
        tag_data_object={}
        tag_data_object["Device_macId"] = key
        tag_data_object["Device_name"] = value
        tag_data_object["Device_status"] = device_status
        tag_data_object["type"]=get_device_property(key,"Icon")
        tag_data.append(tag_data_object)
    return tag_data
# Redmine 19936 Stop

def dm_socket(mqtt_msg,tag,status):
    context = zmq.Context()
    socket = context.socket(zmq.REQ)
    socket.setsockopt(zmq.LINGER,0)
    socket.connect("tcp://localhost:"+dm_port)
    tag_data=[]
    client_data={}
    if(tag=="Device"):
        tag_data = dm_ma_list_format(mqtt_msg, status)
    elif(tag=="Status"):
        tag_data_object={}
        tag_data_object["status"]=status
        tag_data.append(tag_data_object)
    client_data["tag"]=tag
    client_data["tag_data"]=tag_data
    mqtt_msg_json={}
    mqtt_msg_json["client"]="BT"
    mqtt_msg_json["message_id"]="1234"
    mqtt_msg_json["client_data"]=client_data
    socket.send(json.dumps(mqtt_msg_json))
    poller = zmq.Poller()
    poller.register(socket, zmq.POLLIN)
    if poller.poll(1000): # 5s timeout in milliseconds
        socket_status=socket.recv()
    else:
        log.error("BTError: DM reception failed")
    socket.close()

## Voice Assistant Socket
def va_socket(va_msg, led=True):
    """ 
    - Passing Audio string when Bluetooth Connected, Disconnected and Error Case
    - Passing External Bluetooth device Connection state
    """
    va_msg_json={}
    context = zmq.Context()
    socket = context.socket(zmq.REQ)
    socket.setsockopt(zmq.LINGER,0)
    socket.connect("tcp://localhost:"+va_port)
    va_msg_array=va_msg.split(" ")
    va_msg_json['client_name']=va_msg_array[0]
    va_msg_json['client_action']=va_msg_array[1]
    if(va_msg_json['client_action']=="VOICE"):
        va_msg_json['client_data']={}
        #va_msg_json['client_data']['online']=" ".join(va_msg_array[2:])
        #Redmine 20049 start
        va_msg_json['client_data']['led']= led
        #Redmine 20049 end
        if(va_msg_array[2]=="Connected"):
            va_msg_json['client_data']['offline']="bt_connect.mp3"
        elif(va_msg_array[2]=="Bluetooth"):
            va_msg_json['client_data']['offline']="bt_disconnect.mp3"
        elif(va_msg_array[2]=='Searching'):
            va_msg_json['client_data']['offline']="bt_turned_on.mp3"
        #Feature: 19673 Start
        elif(va_msg_array[3]=='turned' and va_msg_array[4]=='off'):
            va_msg_json['client_data']['online']=" ".join(va_msg_array[2:])
        #Feature: 19673 End
    else:
        va_msg_json["client_data"]=" ".join(va_msg_array[2:])
    log.debug("BT: VA: "+json.dumps(va_msg_json))
    socket.send(json.dumps(va_msg_json))
    poller = zmq.Poller()
    poller.register(socket, zmq.POLLIN)
    if poller.poll(5*1000): # 5s timeout in milliseconds
        socket_status=socket.recv()
    else:
        log.error("BTError: VA reception failed")
    socket.close()

## BT Source Mediaplayer Status
def get_mediaplayer_property(mac_addr,property_keyword):
    try:
        bus = dbus.SystemBus()
        device = bluezutils.find_mediaplayer(mac_addr,).object_path
        props = dbus.Interface(bus.get_object(config.SERVICE_NAME,device),"org.freedesktop.DBus.Properties")
        property_param = props.Get(config.MEDIAPLAYER_INTERFACE,property_keyword)
        return property_param
    except:
        return "BTError: Getting mediaplayer property failed"

## External Bluetooth Device Properties
def get_device_property(mac_addr,property_keyword):
    """ Checking the properties of the Connected device. """
    try:
        bus = dbus.SystemBus()
        device = bluezutils.find_device(mac_addr,)
        path = device.object_path
        props = dbus.Interface(bus.get_object(config.SERVICE_NAME,path),"org.freedesktop.DBus.Properties")
        property_param = props.Get(config.DEVICE_INTERFACE,property_keyword)
        if property_keyword=="UUIDs":
            source=False
            sink=False
            for uuid in property_param:
                if uuid=="0000110a-0000-1000-8000-00805f9b34fb":
                    source=True
                elif uuid=="0000110b-0000-1000-8000-00805f9b34fb":
                    sink = True 
            if sink:
                uuid_string="BT STATUS SINK"
            elif source:
                uuid_string="BT STATUS SOURCE"
            else:
                uuid_string="BT STATUS NONE"
            return uuid_string
        # Redmine 19936 Start: Getting a Icon type from Device properties
        elif property_keyword=="Icon":
            return str(property_param)
        # Redmine 19936 Stop
        #Redmine 19742 start
        elif property_keyword=="Connected":
            return str(property_param)
        #Redmine 19742 end
        name = property_param.encode('utf-8')
        return name
    # Redmine 20053 Start: Added exceptional handler for device property
    except:
        # Redmine 19936 Start: If type not found status will be sent as none
        if(property_keyword=="Icon"):
            return str("none")
        else:
            error_type, error_instance, traceback = sys.exc_info()
            log.error(error_instance)
            raise error_type,error_instance,traceback
        # Redmine 19936 Stop
    # Redmine 20053 Stop

## BT module Configuration
#
# To configure Shifu-BT name and discoverable mode (To perform Shifu as visible to other BT devices). 
# Once after the time-out, Shifu-BT won't be visible to external BT devices. 
#
# property_keyword: String, Possible values: ["discoverable","name"]
# 
# Setting discoverable time:
# Property_keyword = "discoverable", value_keyword : int
#
# Setting Name of adapter:
# Property Keyword = "name", value_keyword : utf-8
def set_adapter(property_keyword,value_keyword):
    """
    Used to configure Shifu-BT name and the Discoverable period. 
    """
    try:
        log.debug('BT: Setting Adapter')
        bus = dbus.SystemBus()
        adapter_path = bluezutils.find_adapter().object_path
        adapter = dbus.Interface(bus.get_object(config.SERVICE_NAME, adapter_path), "org.freedesktop.DBus.Properties")
        if property_keyword is "discoverable":
            if type(value_keyword) is bool:
                adapter.Set(config.ADAPTER_INTERFACE, "Discoverable", dbus.Boolean(value_keyword))
            else:
                raise TypeError('value_keyword is unknown type')
        elif property_keyword is "discoverabletime":
            if type(value_keyword) is int:
                adapter.Set(config.ADAPTER_INTERFACE, "DiscoverableTimeout", dbus.UInt32(value_keyword))
            else:
                raise TypeError('value_keyword is unknown type')
        elif property_keyword is "power":
            if type(value_keyword) is bool:
                adapter.Set(config.ADAPTER_INTERFACE,"Powered",dbus.Boolean(value_keyword))
            else:
                raise TypeError('value_keyword is unknown type')
        elif property_keyword is "name":
            name_unicode=value_keyword.decode('utf-8')
            adapter.Set(config.ADAPTER_INTERFACE, "Alias", dbus.String(name_unicode))
        else:
            raise ValueError('property_keyword Undefined')
        log.debug('BT: Adapter Settings Done')
        return "Success"
    except dbus.exceptions.DBusException:
        log.error('BT: Setting Adapter Unsuccessful')
        raise BTError('Setting Adapter Unsuccessful')
    except OSError:
        log.error('BT: OS Functionality Error')
        raise OSError('OS Functionality Error')
    except SystemError:
        log.error('BT: Internal System Issues')
        raise SystemError('Internal System issues')
    except RuntimeError:
        error_type, error_instance, traceback = sys.exc_info()
        log.error(error_instance)
        raise error_type,error_instance,traceback 
    except:
        error_type, error_instance, traceback = sys.exc_info()
        log.error(error_instance)
        raise error_type,error_instance,traceback 

## Shifu-BT validating the status of power and Scan mode:
# check_keyword : String, possible values= ["power", "scan","discoverable"]
def check_adapter(check_keyword):
    """Verifies the BT Adapter power status before starting any operations 
       and also verifies BT scanning  status"""
    try:
        log.debug('Checking '+check_keyword)
        # setting interfaces path and fetching the object path from dbus
        bus = dbus.SystemBus()
        adapter_path = bluezutils.find_adapter().object_path
        adapter = dbus.Interface(bus.get_object(config.SERVICE_NAME, adapter_path),"org.freedesktop.DBus.Properties")
        if(check_keyword=="discoverable"):
            status=adapter.Get(config.ADAPTER_INTERFACE, "Discoverable") # Setting BT Adapter power to on or off
            return status
        elif(check_keyword=="power"):
            status=adapter.Get(config.ADAPTER_INTERFACE, "Powered") # Setting BT Adapter power to on or off
            return status
        elif(check_keyword=="name"):
            status=adapter.Get(config.ADAPTER_INTERFACE, "Alias")
            return status.encode('utf-8')
        elif(check_keyword=="macaddr"):
            status=adapter.Get(config.ADAPTER_INTERFACE, "Address")
            return str(status)
        elif(check_keyword=="scan"):
            status=adapter.Get(config.ADAPTER_INTERFACE, "Discovering")
            return status       
        log.debug('Checked '+check_keyword)
    except dbus.exceptions.DBusException:
        raise BTError('Getting Adapter Status Failed')
    except OSError:
        log.error('BT: OS Functionality Error')
        return "OSError: OS Functionality Error"
    except SystemError:
        log.error('BT: Internal System Issues')
        return "SystemError: Internal System issues"

## API CALL FOR BLUETOOTH ADAPTER INFO
def bt_adapter_profile():
    try:
        adapter_info={}
        adapter_info["device_name"]=check_adapter("name")
        adapter_info["mac_address"]=check_adapter("macaddr")
        adapter_info["device_type"]="Hub"
        return adapter_info
    except:
        error_type, error_instance, traceback = sys.exc_info()
        log.error(error_instance)
        return error_type.__name__+": "+str(error_instance)

## API CALL FOR BLUETOOTH ADAPTER POWER STATUS
def bt_switch_status():
    try:
        return check_adapter("power")
    except:
        error_type, error_instance, traceback = sys.exc_info()
        log.error(error_instance)
        return error_type.__name__+": "+str(error_instance)

## BLUETOOTH MODULE POWER API CALL:
# switch_cmd : Boolean, possible values= [True, False]
def bt_switch(switch_cmd):
    """Used to switch bluetooth power ON & OFF"""
    try:
        if(switch_cmd==True):
            set_adapter("power",True)
            set_adapter("discoverabletime",DISCOVERABLE_INFINITE)
            set_adapter("discoverable",True)
            #dm_socket("NA","Status","ON")
            log.debug('[BT]:[AUTO]: SWITCH ON '+str(DISCOVERABLE_INFINITE))
        elif(switch_cmd==False):
            set_adapter("discoverable",False)
            set_adapter("power",False)
            led = LedControl(LED_port)
            led.connect()
            led.send_msg("LED_BLUTOOTH_DISCOVERY", 0)
            log.debug('[BT]:[AUTO]: SWITCH OFF')
        status="Success"
        return status
    except dbus.exceptions.DBusException:
        if switch_cmd:
            log.critical('BT: switch on BT Device unsuccessful')
            return "BTError: switchon unsuccessful"
        elif not switch_cmd:
            log.critical('BT: switch off BT Device Unsuccesful')
            return "BTError: switchoff unsuccessful"
    except OSError:
        log.error('BT: OS Functionality Error')
        return "OSError: OS Functionality Error"
    except SystemError:
        log.error('BT: Internal System Issues')
        return "SystemError: Internal System issues"
    except RuntimeError:
        error_type, error_instance, traceback = sys.exc_info()
        log.error(error_instance)
        return error_type.__name__+": "+str(error_instance)
    except:
        error_type, error_instance, traceback = sys.exc_info()
        log.error(error_instance)
        return error_type.__name__+": "+str(error_instance)      

## BT SCAN PROCESSING FUNCTION:
# cmd : String, possible values= ["on", "off"]
def scan_func(cmd):
    """Responsible to start & Stop the BT scan process"""
    try:
        log.debug('BT: scan '+cmd+' processing...')
        objects = bluezutils.get_managed_objects()
        adapter = bluezutils.find_adapter()
        # if cmd is "on" will start the scan process
        if (cmd == "on"):
            adapter.StartDiscovery()
            log.debug('[BT]:[AUTO]: SCAN STARTED')
        # if cmd is "off" stops the scan process 
        elif (cmd == "off"):
            adapter.StopDiscovery()
            led = LedControl(LED_port)
            led.connect()
            led.send_msg("LED_BLUTOOTH_DISCOVERY", 0)
            log.debug('[BT]:[AUTO]: SCAN STOPPED')
        return "Success"
    except dbus.exceptions.DBusException:
        log.error('BT: scan '+cmd+' Process Failed')
        raise BTError('scan unsuccessful')
    except OSError:
        log.error('BT: OS Functionality Error')
        raise OSError('OS Functionality Error')
    except SystemError:
        log.error('BT: Internal System Issues')
        raise SystemError('Internal System issues')
    except MemoryError:
        log.error('BT: Memory issues detected')
        raise MemoryError('Memory issues detected')
    except RuntimeError:
        error_type, error_instance, traceback = sys.exc_info()
        log.error(error_instance)
        raise error_type,error_instance,traceback
    except:
        error_type, error_instance, traceback = sys.exc_info()
        log.error(error_instance)
        raise error_type,error_instance,traceback   

## API CALL TO STOP the SCAN process
def bt_stop_discovery():
    """API call to stop the scan process"""
    try:
        global scan_status
        global discovered_bt
        discovered_bt=list_devices("RSSI")
        scan_status=scan_func("off")
        return scan_status
    except:
        error_type, error_instance, traceback = sys.exc_info()
        return error_type.__name__+": "+str(error_instance)

## LIST OF ALL DEVICES  
# list_keyword: String type, Possible values: ["Paired","Connected","RSSI","RSSIprev"]
def list_devices(list_keyword):
    """Provides the list of Scanned, Paired and Connected device
       and also used to clear the list of previously scanned devices"""
    try:
        listed_dev={} 
        sorting=0 # Sorting is integer to find the Name and Mac-address int the properties
        sv_link=False # sv_link is boolean, used for find RSSI in scan
        sv_link_prev=False # sv_link_prev is boolean, used ro remove previous scan list, neglecting connected and paired list.
        sv_check='0' # sv_check is string, used to check the paired and connected process
        objects = bluezutils.get_managed_objects()
        all_devices = (str(path) for path, interfaces in objects.iteritems() if config.DEVICE_INTERFACE in interfaces.keys())
        for path, interfaces in objects.iteritems():
            # checking for dbus BT adapter
            if config.ADAPTER_INTERFACE not in interfaces.keys():
                continue
            device_list = [d for d in all_devices if d.startswith(path + "/")]
            for dev_path in device_list:
                # checking for the device list from the BT device manager
                dev = objects[dev_path]
                properties = dev[config.DEVICE_INTERFACE]
                # checking for the properties dictionary of all the devices
                for key in properties.keys(): 
                    value=properties[key]
                    # If key is name, stores the name of the Remote BT device into the dictionary
                    if (key == "Name"):
                        sv_dev_name=value.encode('utf-8')
                        sorting=sorting+1
                    # If paired then stored the device paired status 
                    elif ((key == "Paired") and (list_keyword=="Paired")):
                        sv_check = str(value)
                    # If Connected then stored the device connected status 
                    elif ((key == "Connected") and (list_keyword=="Connected")):
                        sv_check = str(value)
                    # If remote device scanned RSSI value is detected, enables the detected status for the mac-address
                    elif ((key== "RSSI") and (list_keyword=="RSSI")):
                        sv_link = True
                    # If the devices connected or paired, enables the sv_link_prev
                    elif (((key=="Connected") or (key=="Paired")) and ((list_keyword=="RSSIprev") or (list_keyword=="RSSI"))):
                        if(str(value)=='1'):
                            sv_link_prev=True
                    # if key is "Address", stores the mac-address in the variable
                    elif (key == "Address"):
                        sv_mac_addr=str(value)
                        sorting=sorting+1
                # if list_keyword is paired, stores the list of paired devices in the dictionary
                # if list_keyword is connected, stores the list of connected devices in the dictionary
                if((sorting==2) and (sv_check == '1') and ((list_keyword=="Paired") or (list_keyword=="Connected"))):
                    listed_dev[sv_mac_addr]=sv_dev_name
                # stores the discovered devices in the dictionary
                elif ((sorting==2) and (sv_link is True) and (sv_link_prev is False) and (list_keyword=="RSSI")):
                    listed_dev[sv_mac_addr]=sv_dev_name
                # removes the previously scanned devices from the device list
                elif ((sorting==2) and (sv_link_prev is False) and (list_keyword=="RSSIprev")):
                    bt_process(sv_mac_addr,"unpair")
                sorting=0
                sv_link=False
                sv_link_prev=False
        return listed_dev
    except dbus.exceptions.DBusException:
        log.error('BT: Listing Devices failed')
        raise BTError('Listing Devices Failed')
    except BTError:
        error_type, error_instance, traceback = sys.exc_info()
        log.error(error_instance)
        raise error_type,error_instance,traceback    
    except OSError:
        log.error('BT: OS Functionality Error')
        raise OSError('OS Functionality Error')
    except SystemError:
        log.error('BT: Internal System issues')
        raise SystemError('Internal System issues')
    except MemoryError:
        log.error('BT: Memory issues detected')
        raise MemoryError('Memory issues detected')
    except RuntimeError:
        error_type, error_instance, traceback = sys.exc_info()
        log.error(error_instance)
        raise error_instance
    except:
        error_type, error_instance, traceback = sys.exc_info()
        log.error(error_instance)
        raise error_type,error_instance,traceback   

## API CALL FOR SCANNED DEVICE LIST
def print_discovery():
    """API call to list all the external scanned BT devices"""
    try:
        global discovered_bt
        local_device_list={}
        local_device_list=list_devices("RSSI")
        if bool(local_device_list):
            discovered_bt=local_device_list
        return discovered_bt
    except:
        error_type, error_instance, traceback = sys.exc_info()
        return error_type.__name__+": "+str(error_instance)

def list_discovery():
    scan_list_count=0
    sent_list={}
    try:
        while(scan_list_count<(25/SCAN_LIST_INTERVAL)):
            time.sleep(SCAN_LIST_INTERVAL)
            scan_list=print_discovery()
            sendable_list={}
            for scan_list_key,scan_list_value in scan_list.iteritems():
                if scan_list_key not in sent_list:
                    sendable_list[scan_list_key]=scan_list_value
                    sent_list[scan_list_key]=scan_list_value
            if bool(sendable_list):
                dm_socket(sendable_list,"Device","not_paired")
            status=check_adapter("scan")
            scan_list_count=scan_list_count+1
        if(scan_list_count>=(25/SCAN_LIST_INTERVAL)):
            bt_stop_discovery()
        scan_list_count=0
    except:
        error_type, error_instance, traceback = sys.exc_info()
        return error_type.__name__+": "+str(error_instance)

## API Call FOR START SCANNING PROCESS
def bt_start_discovery(DISCOVERY_TIME=25):
    """API call to initialise the BT scan process for the defined period of time"""
    global timer # thread_timer of stop scanning
    global scan_status # String variable for response status of scanning process 
    try:
        
        status = check_adapter("scan")
        if(status==True):
            log.error("BT: BT scan is already in process")
            return -1,"Bluetooth scan already in process"
        discovered_bt.clear()
        led = LedControl(LED_port)
        led.connect()
        led.send_msg("LED_BLUTOOTH_DISCOVERY", 1)
        list_devices("RSSIprev") # Removes previously discovered device list
        scan_func("on")
        scan_list_thread=threading.Thread(target=list_discovery)
        scan_list_thread.start()
        log.debug("BT: thread started")
        return 0,
    except BTError:
        error_type, error_instance, traceback = sys.exc_info()
        log.error(error_instance)
        return -2,error_type.__name__+": "+str(error_instance)
    except:
        scan_status="Error"
        error_type, error_instance, traceback = sys.exc_info()
        log.error(error_instance)
        log.error('BT: Error in scan process') 
        return -2,error_type.__name__+": "+str(error_instance)

## API CALL FOR SCANNING MODE STATUS 
def bt_discovery_status():
    """API to know the BT scanning process"""
    return scan_status

## API CALL FOR DISCOVERABLE MODE :
# discoverable_time = uint32_t (in secs)
def bt_discoverable(discoverable_time=DISCOVERABLE_TIMEOUT):
    """ 
    API call to keep Shifu-BT in discoverable mode based on the requested time in secs.
    If discoverable_time is set to 0, Shifu-BT will be always in discoverable mode to other devices.
    """
    try:
        set_adapter("discoverabletime",discoverable_time)
        status=set_adapter("discoverable",True)
        dm_socket("NA","Status","ON")
        log.debug('[BT]:[AUTO]: SWITCH ON '+str(discoverable_time))
        return status
    except:
        error_type, error_instance, traceback = sys.exc_info()
        return error_type.__name__+": "+str(error_instance)

## API CALL TO MODIFY Shifu-BT NAME :
# name_utf8: utf-8 / String.
def bt_adapter_name(name_utf8):
    """API call to modify the Shifu-BT name. utf-8 format data will be converted to Unicode """
    try:
        status=set_adapter("name",name_utf8)
        log.debug('[BT]:[AUTO]: RENAMED '+str(name_utf8))
        return status
    except:
        error_type, error_instance, traceback = sys.exc_info()
        return error_type.__name__+": "+str(error_instance)

## BLUETOOTH ACTION API CALL :
# mac_addr : String type, format= "XX:XX:XX:XX:XX:XX"
# process_cmd : String type, possible values: ["unpair", "connect", "disconnect"]
def bt_process(mac_addr,process_cmd):
    """Function is responsible to handle pairing & connection, disconnection and unpairing the external BT device"""
    try:
        log.debug('Setting BT process')
        bus=dbus.SystemBus()
        managed_objects=bluezutils.get_managed_objects()
        device=bluezutils.find_device(mac_addr, )
        card_mac='_'.join(mac_addr.split(':'))
        # Find the BT adapter and remove the device from the device list
        if (process_cmd=="unpair"):
            local_device_list=list_devices("Connected")
            if bool(local_device_list):
                prev_macaddr=local_device_list.keys()[0]
                if(prev_macaddr==mac_addr):
                    bt_process(mac_addr,"disconnect")
            adapter = bluezutils.find_adapter_in_objects(managed_objects, )
            try:
                dev = bluezutils.find_device_in_objects(managed_objects, mac_addr, )
                path = dev.object_path
            except:
                path = mac_addr
            adapter.RemoveDevice(path)
            # Redmine 19739 Start: After Forget the device will be removed from Last 5 Connected devices
            five_pair_list.remove(mac_addr)
            # Redmine 19739 Stop
            log.debug('[BT]:[AUTO]: FORGOT '+str(mac_addr))
        # Device pair, connection and trusting a remote is process
        elif (process_cmd=="connect"):
            local_device_list=list_devices("Connected")
            if bool(local_device_list):
                prev_macaddr=local_device_list.keys()[0]
                if(prev_macaddr!=mac_addr):
                    status=bt_process(prev_macaddr,"disconnect")
                    if(status=="Success"):
                        dm_msg={}
                        dm_msg[prev_macaddr]=get_device_property(prev_macaddr,"Name")
                        dm_socket(dm_msg,"Device","paired")
                # Added Error case, If the Device is already connected for reconnection
                else:
                    return "BTError: Already Connected"
            device.Connect()       
            uuid_string = get_device_property(mac_addr,"UUIDs")
            if(uuid_string=="BT STATUS SINK"):
                log.debug('BT: Setting sink profile')
                time.sleep(1)
                try:
                    pactl_out=subprocess.check_output('pactl set-card-profile bluez_card.'+card_mac+' a2dp_sink',shell=True)
                    log.debug("BT: a2dp_sink profile is set")
                except:
                    pactl_out=subprocess.check_output('pactl set-card-profile bluez_card.'+card_mac+' headset_head_unit',shell=True)
                    log.debug("BT: headset_head_unit sink profile is set")
            elif(uuid_string=="BT STATUS SOURCE"):
                log.debug('BT: setting source profile')
                log.debug("BT: Source profile")
                try:
                    time.sleep(1)
                    log.debug("BT: setting card profile")
                    pactl_out=subprocess.check_output('pactl set-card-profile bluez_card.'+card_mac+' a2dp_source',shell=True)
                    log.debug(pactl_out)
                    # Redmine 20283 Start: Updated the Max Volume to 99 After connection
                    pactl=subprocess.check_output("pactl set-source-volume bluez_source "+str(MAX_SOURCE_VOLUME)+"%",shell=True)
                    # Redmine 20283 Stop 
                except:
                    log.error('BT: Source volume setting failed')
                status=get_mediaplayer_property(mac_addr,"Status")
                if(status=="playing"):
                    va_socket("BT COMMAND STOP",)
            else:
                log.debug('BT: setting other profile')
            file=open("/etc/bluetooth/lastconnect_macaddr","w+")
            file.write(mac_addr+str(" 1"))                                    
            file.close()
            # Redmine 19739 Start: Connected device will be added to Last 5 Connected List
            five_pair_list.add(mac_addr)
            # Redmine 19739 Stop
            va_socket(uuid_string,)
            name = get_device_property(mac_addr,"Name")
            va_socket("BT VOICE Connected to "+str(name)+" Bluetooth",)
            led = LedControl(LED_port)
            led.connect()
            led.send_msg("LED_BLUTOOTH_CONNECTED", 1)
            #set_adapter("discoverable",False)
            log.debug('[BT]:[AUTO]: CONNECTED '+str(mac_addr))
        # Device Disconnection is processed
        elif (process_cmd=="disconnect"):
            local_device_list=list_devices("Connected")
            if bool(local_device_list):
                prev_macaddr=local_device_list.keys()[0]
                if(prev_macaddr==mac_addr):
                    name = get_device_property(mac_addr,"Name")
                    device.Disconnect()
                    file=open("/etc/bluetooth/lastconnect_macaddr","w+")
                    file.write(mac_addr+str(" 0"))                                    
                    file.close()
                    va_socket("BT STATUS NONE",)
                    va_socket("BT VOICE Bluetooth device disconnected from "+str(name),)
                    led = LedControl(LED_port)
                    led.connect()
                    led.send_msg("LED_BLUTOOTH_CONNECTED", 1)
                    #set_adapter("discoverable",True)
                    #dm_socket("NA","Status","ON")
                    log.debug('[BT]:[AUTO]: DISCONNECTED '+str(mac_addr))
                else:
                    log.error('BT: Macaddr is not in the connected list')
                    raise BTError('Macaddr is not in the connected list')
            else:
                #va_socket("BT VOICE No device to Disconnect")
                log.error('BT: No device to Disconnect')
                raise BTError('No device to Disconnect')
        status = "Success"
        log.debug('BT Processing Finished')
        return status
    except dbus.exceptions.DBusException:
        if (process_cmd=="connect"):
            #va_socket("BT VOICE Connection Failed")
            log.critical('BT: connection Unsuccessful')
            return "BTError: Connection Unsuccessful"
        elif (process_cmd=="disconnect"):
            #va_socket("BT VOICE Disconnection Failed")
            log.critical('BT: Disconnection Unsuccessful')
            return "BTError: Disconnection Unsuccesful"
        elif (process_cmd=="unpair"):
            log.critical('BT: Unpairing Device Failed')
            return "BTError: Unpairing Device Failed"
    except OSError:
        log.error('BT: OS Functionality Error')
        return "OSError: OS Functionality Error"
    except SystemError:
        log.error('BT: Internal System issues')
        return 'SystemError: Internal System issues'
    except MemoryError:
        log.error('BT: Memory issues detected')
        return "MemoryError: Memory issues detected"
    except RuntimeError:
        error_type, error_instance, traceback = sys.exc_info()
        log.error(error_instance)
        return error_type.__name__+": "+str(error_instance)
    # Redmine 20053 Start: Unpair request with Device not found Error is a Success case
    except:
        error_type, error_instance, traceback = sys.exc_info()
        log.error(str(error_instance))
        if process_cmd == "unpair" and "device not found" in str(error_instance):
            log.debug("device_not_found_success")
            return "Success"
        return error_type.__name__+": "+str(error_instance)   
    # Redmine 20053 Stop 

## API CALL FOR PAIRED DEVICES LIST
def bt_list_paired_dev():
    """API for Paired BT Devices list"""
    try:
        paired_dev = list_devices("Paired")
        # Redmine 19936 Start: Paired device list Format with Icon Type
        paired_dev = dm_ma_list_format(paired_dev,"paired")
        # Redmine 19936 Stop
        return paired_dev
    except:
        error_type, error_instance, traceback = sys.exc_info()
        return error_type.__name__+": "+str(error_instance)

## API CALL FOR CONNECTED DEVICE NAME & MAC ADDR
def bt_list_connected_dev():
    """API for Connected BT Devices list"""
    try:
        connected_dev={}
        connecting_dev=list_devices("Connected")
        fil=open("/etc/bluetooth/lastconnect_macaddr","r")
        file_read=fil.read()
        fil.close()
        if len(file_read)<1:
            return connected_dev
        else:
            connected_dev_status=file_read.split(" ")
        if (bool(connecting_dev)==False) and connected_dev_status[1]=='1':
            try:
                # Redmine 19742 Start: Added force disconnection 
                bus=dbus.SystemBus()
                managed_objects=bluezutils.get_managed_objects()
                device=bluezutils.find_device(connected_dev_status[0], )
                device.Disconnect()
                # Redmine 19742 Stop
                fil=open("/etc/bluetooth/lastconnect_macaddr","w+")
                fil.write(connected_dev_status[0]+str(" 0"))                                    
                fil.close()
                # Redmine 20053 Start: If the device is already in forget state, Name will be displayed as Mac_addr
                try:
                    name = get_device_property(connected_dev_status[0],"Name")
                    #Redmine 19742 start
                    log.debug("BT: SA: Disconnected" + str(get_device_property(connected_dev_status[0],"Connected")))
                    #Redmine 19742 end
                except:
                    name = connected_dev_status[0]
                    log.error("BT: device property not found")
                # Redmine 20053 Stop
                dm_msg={}
                dm_msg[connected_dev_status[0]]=name
                dm_socket(dm_msg,"Device","paired")
                log.debug('[BT]:[AUTO]: DISCONNECTED '+str(connected_dev_status[0]))
                # set_adapter("discoverable",True)
                # dm_socket("NA","Status","ON")
            except:
                log.error("BT: SA: Disconnection process failure")
        elif(connected_dev_status[1]=='1'):
            if connected_dev_status[0] in connecting_dev:
                # Redmine 19936 Start: Type Identification format Added
                connected_dev["Device_macId"] = connected_dev_status[0]
                connected_dev["Device_name"] = connecting_dev[connected_dev_status[0]]
                connected_dev["Device_status"] = "connected"
                connected_dev["type"] = get_device_property(connected_dev_status[0],"Icon")
                # Redmine 19936 Stop
        return connected_dev
    except:
        error_type, error_instance, traceback = sys.exc_info()
        return error_type.__name__+": "+str(error_instance)

## API CALL FOR LISTING PAIRED AND CONNECTED DEVICES
def bt_list_devices():
    paired_list = bt_list_paired_dev()
    if type(paired_list) is str:
        return paired_list
    connected_list = bt_list_connected_dev()
    if bool(connected_list) and type(connected_list) is dict:
        for check_list in paired_list:
            if (connected_list["Device_macId"] == check_list["Device_macId"]):
                paired_list[paired_list.index(check_list)]["Device_status"] = "connected"
                break
        return paired_list
    else:
        return paired_list

## AVRCP API CALL:
# mac_addr : String, format= "XX:XX:XX:XX:XX:XX"
# process_cmd : String, possible values: ["play", "pause", "stop", "next", "previous", "restart"]
def bt_avrcp_command(mac_addr,player_cmd):
    """API call will be used for AVRCP commands: play, pause, stop, next, previous and restart the track"""
    try:
        log.debug('BT: AVRCP '+player_cmd+' Commands Processing')
        media=bluezutils.find_mediaplayer(mac_addr,) # check whether the media player is connected to the media
        # media player play command is processed
        if (player_cmd == "play"):
            player_status=get_mediaplayer_property(mac_addr,"Status")
            if(player_status!="playing"):
                media.Play()
        # media player pause command is processed
        elif (player_cmd == "pause"):
            media.Pause()
        # media player stop command is processed
        elif (player_cmd == "stop"):
            media.Stop()
        # media player next command is processed
        elif (player_cmd == "next"):
            media.Next()
        # media player previous command is processed 2 times to go back to the previous track
        elif (player_cmd == "previous"):
            media.Previous()
            time.sleep(0.2)
            media.Previous()
        # media player previous command is processed for 1 time to process the restart track
        elif (player_cmd == "restart"):
            media.Previous()
        log.debug('[BT]:[AUTO]: AVRCP '+player_cmd.upper()+' '+str(mac_addr))
        return "Success"
    except dbus.exceptions.DBusException:
        log.error('BT: BT AVRCP mediaplayer Error')
        return "BTError: BT avrcp mediaplayer Error"
    except BTError:
        error_type, error_instance, traceback = sys.exc_info()
        log.error(error_instance)
        return error_type.__name__+": "+str(error_instance)    
    except OSError:
        log.error('BT: OS Functionality Error')
        return "OSError: OS Functionality Error"
    except SystemError:
        log.error('BT: Internal System Error')
        return "SystemError: Internal System issues"
    except MemoryError:
        log.error('BT: Memory issues detected')
        return "MemoryError: Memory issues detected"
    except RuntimeError:
        error_type, error_instance, traceback = sys.exc_info()
        log.error(error_instance)
        return error_type.__name__+": "+str(error_instance)
    except:
        error_type, error_instance, traceback = sys.exc_info()
        log.error(error_instance)
        return error_type.__name__+": "+str(error_instance)