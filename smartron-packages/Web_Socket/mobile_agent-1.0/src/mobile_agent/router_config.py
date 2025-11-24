#!/usr/bin/python
"""Router configuration management.

This module helps to create CAP, RE, STA.
It changes the SSID, password, encryption, deleting a particular user, checks the password, starts the wifi
"""
# __author__ = "Praveen kuamr Donga & Gopinath"
# __copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
# __credits__ = ["Praveenkumar Donga", "Gopinath"]
# __maintainer__ = "Praveenkumar Donga, Anudeep T"
# __email__ = "praveen.donga@smartron.com"
# __status__ = "Testing"

# 16165: [shifu][wifi] Comments for each line in Router configuration scripts;   Praveen kumar Donga     30/05/2018
## Comments are added for WBLR, EBLR, QWRAP and RE scripts.

import subprocess
import shlex
import logging.handlers
import os, json
import zmq
import ast
# Feature:20112-Start
import threading
import time
import led_client as LED
# import mobile_agent as ma
# Feature:20112-End

def get_build():
    x = subprocess.check_output(['lsb_release', '-b'])
    x = x.strip()
    build_no = x.split('IND')[1]
    return build_no
build_no = "B"+get_build()
######## End of adding build no. Need to remove above this

logger = logging.getLogger(build_no + ':' + __name__)
logger.setLevel(logging.DEBUG)
handler = logging.handlers.SysLogHandler(address='/dev/log')
formatter = logging.Formatter('%(name)s %(funcName)s %(lineno)d:%(message)s')
handler.setFormatter(formatter)
logger.addHandler(handler)

'''
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)  # setting the log level to debug mode
handler = logging.handlers.SysLogHandler(
    address='/dev/log')  # directidng the logs to /dev/log folder usr readlog command to read the logs
formatter = logging.Formatter('%(module)s.%(funcName)s:%(message)s')  # Logs record format
handler.setFormatter(formatter)
logger.addHandler(handler)
'''

# encryption modes : psk2, psk-mixed ,none ,wep-open ,wep-shared ,psk,
# enc_types = ['psk2', 'psk-mixed', 'none', 'wep-open', 'wep-shared', 'psk', 'psk2', 'psk2-256', 'psk2+gcmp', \
#             'psk2+gcmp-256', 'tkip', 'wep', 'wep+shared', 'wapi-psk']  # possible encryption types.
enc_types = ['psk-mixed', 'psk2', 'psk', 'wep', 'psk2+ccmp']  # possible encryption types.
radio = ['0', '1']  # possible radio numbers in Dakota board.


def run_command(command):
    """The function executes all the commands. This is internal function."""
    #args = shlex.split(command)
    try:
        # subprocess.call(args)
        output = subprocess.check_output(command, shell= True).strip()
    except subprocess.CalledProcessError as e:
        #start of Bug #21048
        if command == "/rom/etc/uci-defaults/network":
            logger.error ("network file not present in default location. looking at /rom/rom/etc..")
            command = "/rom/rom/etc/uci-defaults/network"
            try:
                output = subprocess.check_output(command, shell=True).strip()
                logger.debug("network file executed from /rom/rom/etc..")
            except:
                output = 1
                logger.error('Subprocess call error')
                logger.error("%s : execution failed", command, exc_info=True)
        #end of #21048
        elif command in ["uci delete wireless.@wifi-iface[2] 2>/dev/null" , "uci delete wireless.qcawifi 2>/dev/null"]:
            output = 0
            logger.debug("uci delete failed")
        else:
            output = 1
            logger.error('Subprocess call error')
            logger.error("%s : execution failed", command, exc_info=True)
    except OSError, e:
        output = 1
        logger.error('System Error')
        logger.error("%s : execution failed", command, exc_info=True)
    except Exception, e:
        output = 1
        logger.error('Unexpected Error', exc_info=True)
    if output == 1:
        return 1
    logger.debug("%s : executed successfully", command)
    return output


def va_socket(va_msg):
    """
    - Passing router connection state to voice assistant
    """
    logger.debug("va_socket funtion is called")
    context = zmq.Context()
    socket = context.socket(zmq.REQ)
    socket.setsockopt(zmq.LINGER, 0)
    socket.connect("tcp://localhost:8800")
    socket.send(va_msg)
    poller = zmq.Poller()
    poller.register(socket, zmq.POLLIN)
    if poller.poll(5 * 1000):  # 5s timeout in milliseconds
        socket_status = socket.recv()
    else:
        logger.error("MA Error: ZMQ reception failed")
    socket.close()


#####################
# router_config APIs
#####################
# 26/02/2018: change_ssid function changes both ssids of interface 0 and interface 1
# def change_ssid(num,ssid):
def change_ssid(ssid):
    # 26/02/2018
    """It takes the input attribute and sets the ssid to the desired input value by calling this function.

    Once the ssid is set , we have to commit UCI and then restart wifi.
    """
    logger.debug("change_ssid function is called")
    t2 = ssid_check(ssid)
    if not (t2[0]):
        logger.error("5108:Input parameters are not Valid")
        return 1, t2[1]

    # generating the command with the argument.
    cmd_list = [
        #  "uci set wireless.@wifi-iface[" +num+ "].ssid="+ssid,
        "uci set wireless.@wifi-iface[0].ssid='" + ssid + "'",
        "uci set wireless.@wifi-iface[1].ssid='" + ssid + "'",
        "uci commit wireless",
        "wifi"
    ]
    # Commands-Execution: Start
    for cmd in cmd_list:
        t = run_command(cmd)
        if t == 1:
            logger.error("5109:ssid change unsuccessfull")
            return 1, '5109'
    # Commands-Execution: End
    logger.info("ssid changed successfully")
    return 0, '6000'


def change_password(password):
    """It takes the input attribute and sets the password to the desired input value by calling this function.

    Once the password is set , we have to commit UCI and then restart wifi.
    """
    logger.debug("change_password function is called")
    # validating the argument type:
    t2 = pass_check(password)
    if not (t2[0]):
        logger.error("5108:Input parameters are not Valid")
        return 1, t2[1]
    # generating the command with the arguments.
    cmd_list = [
        "uci set wireless.@wifi-iface[0].key='" + password + "'",
        "uci set wireless.@wifi-iface[1].key='" + password + "'",
        "uci commit wireless",
        "wifi"
    ]
    # command execution-Start
    for cmd in cmd_list:
        t = run_command(cmd)
        if t == 1:
            logger.error("5110:password change unsuccessfull")
            return 1, '5110'
    # command execution-End
    logger.info("password changed successfully")
    return 0, '6000'


def set_encryption(enc):
    """It takes the input attribute and sets the encryption to the desired input value by calling the function.

    Once the encryption is set , we have to commit UCI and then restart wifi.
    """
    logger.debug("set_encryption function is called")
    # validating the argument type:
    t2 = enc_check(enc)
    if not (t2[0]):
        logger.error("5108:Input parameters are not Valid")
        return 1, t2[1]
    # generating the commands with the argument.
    cmd_list = [
        "uci set wireless.@wifi-iface[0].encryption=" + enc,
        "uci set wireless.@wifi-iface[1].encryption=" + enc,
        "uci commit wireless",
        "wifi"
    ]
    # current encryprtion checking
    crt_enc = getEncryption()
    if crt_enc == enc:
        logger.info("Encryption already set to %s", enc)
        return 0, '6001'
    # command Execution
    for cmd in cmd_list:
        t = run_command(cmd)
        if t == 1:
            logger.error("5111:encryption change unsuccessfull")
            return 1, '5111'
    # Command Execution-End
    logger.info("encryption changed successfully.")
    return 0, '6000'


def delete_user(macId):
    """It takes the macID of the user as attribute and deletes the user from the list by calling the function"""
    logger.debug("delete_user function is called")
    cmd_list = [
        "iwpriv ath0 kickmac " + macId
    ]
    # command execution-Start
    for cmd in cmd_list:
        t = run_command(cmd)
        if t == 1:
            logger.error("5112:user not deleted")
            return 1, '5112'
    # command execution-End
    logger.info("user deleted")
    return 0, '6000'


def hpb():
    """It switches the hard push button which is required for 2 dakota boards to operate in re mode."""
    logger.debug("hpb function is called")
    cmd_list = [
        "env -i ACTION=\"pressed\" BUTTON=\"wps\" /sbin/hotplug-call button; sleep 1; env -i "
        + "ACTION=\"released\" BUTTON=\"wps\"  /sbin/hotplug-call button"
    ]
    logger.debug(cmd_list[0])
    # command execution-Start
    for cmd in cmd_list:
        t = run_command(cmd)
        if t == 1:
            logger.error("5113:Hard push button is not activated")
            return 1, '5113'
    # command execution-End
    logger.info("Hard push button activated")
    return 0, '6000'

def set_re():
    """It sets the radio to RE mode (range extender) by calling this function.

    Once the RE mode is set , we have to commit UCI and then restart wifi.29 uci commands to be executed.
    """

    logger.debug("set_re function is called")
    logger.debug(str(threading.enumerate()))
    thread_list = []
    for t in threading.enumerate():
        thread_list.append(t.name)
    if "rc_thread" in thread_list:
        logger.critical("rc_thread is already running")
        return 1, '5134'
    else:
        logger.debug("configuring the hub into RE mode")
        rc_thread = threading.Thread(target=set_re_task, name="rc_thread")
        rc_thread.start()
        logger.debug("MA running threads list:" + str(threading.enumerate()))
    return 0, '6000'


def set_re_task():
    logger.debug("set_re function is called")
    cmd_list = [
        "uci set dhcp.lan.ignore=1",  # configure DHCP to not respond
        "uci commit dhcp",  # save DHCP settings
        "uci set network.lan.proto=dhcp",  # disable NAT- Dynamic IP address
        "uci delete network.wan",  # configure device as a pure bridge
        "uci commit network",  # save network settings
        "/etc/init.d/network restart",  # restart network service
        "/etc/init.d/dnsmasq restart",  # restart dnsmasq service
        "uci set wireless.@wifi-iface[0].wnm=1",  # set WNM capability for radio 0
        "uci set wireless.@wifi-iface[1].wnm=1",  # set WNM capability for radio 1
        # 21301-Assigning default country code as India and removing throughput patch start.
        "uci set wireless.@wifi-device[0].country=0x138E", # Setting country code for Radio-0 to IN(India)
        "uci set wireless.@wifi-device[1].country=0x138E", # Setting country code for Radio-1 to IN(India)
	# 21301-Assigning default country code as India and removing throughput patch end.
	# 21064: Throughput improvement start
	"uci set wireless.wifi0.htmode=HT40", # Setting bandwidth (2*20MHz channels) for 2.4GHz radio
	"uci set wireless.wifi1.htmode=VHT80", # Setting bandwidth (2*40MHz channels) for 5GHz radio
	"uci set wireless.wifi0.hwmode=11n", # Selects 802.11n protocol for 2.4GHz radio 
	"uci set wireless.wifi1.hwmode=11ac", # Selects 802.11ac protocol for 5GHz radio
	"uci set wireless.@wifi-iface[0].wds=1", # Allows connecting OpenWrt based station devices to Shifu seamlessly
	"uci set wireless.@wifi-iface[1].wds=1", # Allows connecting OpenWrt based station devices to Shifu seamlessly
	# 21064: Throughput improvement end
        "uci commit wireless",  # save wireless settings
        "uci set hyd.Topology.ENABLE_NOTIFICATION_UNICAST=1",  # set AP steering parameter
        "uci set hyd.Topology.PERIODIC_QUERY_INTERVAL=15",  # set AP steering parameter
        "uci set hyd.@Vlanid[0].vid=2",  # set vlan id for radio 0
        "uci set hyd.@Vlanid[1].vid=1",  # set vlan id for radio 1
        "uci commit hyd",  # save hyd settings
        "uci set lbd.StaDB.MarkAdvClientAsDualBand=1",  # set band steering parameter
        "uci set lbd.SteerExec_Adv.StartInBTMActiveState=1",  # set band steering parameter
        "uci commit lbd",  # save lbd settings
        "uci set repacd.WiFiLink.ManageVAPInd=1",  # enabling VAP for repeater placement
        "uci set repacd.WiFiLink.5GBackhaulEvalTimeShort='180'",
        # minimum evaluation time (in sec) for bring back up forcibly down 5 GHz backhaul
        "uci set repacd.WiFiLink.5GBackhaulEvalTimeLong='300'",
        # maximum evaluation time (in sec) for bring back up forcibly down 5 GHz backhaul
        "uci set repacd.WiFiLink.2GBackhaulEvalTime='180'",
        # Evaluation time (in sec) for bring back up forcibly down 2.4 GHz backhaul
        "uci set repacd.WiFiLink.BSSIDAssociationTimeout='75'",
        # timeout (in sec) for which if the RE is not associated with configured BSSID
        "uci set repacd.WiFiLink.RateNumMeasurements=10",
        # number of measurements to take to arrive at an average Rate to compare against the Min/Max thresholds
        "uci set repacd.WiFiLink.MinAssocCheckPostBSSIDConfig=10",
        # number of times the association must be deemed up after a BSSID is configured before it is considered stable enough before a rate measurement can begin
        "uci set repacd.WiFiLink.DaisyChain=1",
        # Daisy chain feature which allows a RE to connect to another RE or the Root AP
        "uci set repacd.repacd.ConfigREMode=son",  # setting the mode of range extension to SON
        "uci set repacd.repacd.Enable=1",  # enable repacd
        "uci commit repacd",  # save repacd setting
        "wifi",  # restart wifi
        "/etc/init.d/repacd start",  # start repacd daemon
        "iwpriv ath0 wps 1",  # enable wps functionality on radio 0
        "iwpriv ath1 wps 1",  # enable wps functionality on radio 1
        "uci set shifu.conf.DM_STACK_CHECK=00 && uci commit shifu",  # set NV variable for IoT
        "/etc/init.d/dm_service stop",  # stop Device_Manager_Gateway
        "/etc/init.d/dm_service start"  # start Device_Manager_Gateway
    ]
    # command execution-Start
    for cmd in cmd_list:
        t = run_command(cmd)
        if t == 1:
            logger.error("5115:set to RE mode unsuccessfull")
            return 1, '5115'
    # command execution-End
    logger.info("please turn on WPS hard push button on the CAP first and then on RE within 10 sec")
    return 0, '6000'


def create_ap(num, ssid, enc, password):
    """It takes the input attributes and creates an Access point from the radio by calling this function.

    Once the access point is created , we have to commit UCI and then restart wifi.
    """
    logger.debug("create_ap function is called")
    # validating the argument type:
    t1 = num_check(num)
    t2 = ssid_check(ssid)
    t3 = enc_check(enc)
    t4 = pass_check(password)
    if not (t1[0]):
        logger.error("5108:Input parameters are not Valid")
        return 1, t1[1]
    elif not (t2[0]):
        logger.error("5108:Input parameters are not Valid")
        return 1, t2[1]
    elif not (t3[0]):
        logger.error("5108:Input parameters are not Valid")
        return 1, t3[1]
    elif not (t4[0]):
        logger.error("5108:Input parameters are not Valid")
        return 1, t4[1]
    # generating the commands with the argument.
    cmd_list = [
        "uci set dhcp.lan.ignore=1",
        "uci commit dhcp",
        "uci set network.lan.proto=dhcp",
        "uci delete network.wan",
        "uci commit network",
        "/etc/init.d/network restart",
        "/etc/init.d/dnsmasq restart",
        "uci set wireless.@wifi-iface[" + num + "].ssid='" + ssid + "'",
        "uci set wireless.@wifi-iface[" + num + "].encryption=" + enc,
        "uci set wireless.@wifi-iface[" + num + "].key='" + password + "'",
        "uci commit wireless",
        "wifi"
    ]
    # command execution-Start
    for cmd in cmd_list:
        t = run_command(cmd)
        if t == 1:
            logger.error("5117:AP is not created")
            return 1, '5117'
    # command execution-End
    logger.info("AP created successfully")
    return 0, '6000'


def init_wifi(num):
    """It initializes the wifi before changing the ssid,password and encryption."""
    logger.debug("init_wifi function is called")
    # validating the argument type:
    t1 = num_check(num)
    if not (t1[0]):
        logger.error("5108:Input parameters are not Valid")
        return 1, t1[1]
    # generating the command with the argument.
    cmd_list = [
        "uci set wireless.@wifi-device[" + num + "].disabled=0",
        "uci commit wireless"
    ]
    # command execution-Start
    for cmd in cmd_list:
        t = run_command(cmd)
        if t == 1:
            logger.error("5118:Wifi[%s] is not Initialized", num)
            return 1, '5118'
    # command execution-End
    logger.info("Wifi[%s] is Initialized", num)
    return 0, '6000'


def start_wifi():
    """The function starts the wifi"""
    logger.debug("start_wifi function is called")
    cmd = "wifi"
    t = run_command(cmd)
    if t == 1:
        logger.error("5119:Wifi is not Started/restarted")
        return 1, '5119'
    logger.info("Wifi is restarted")
    return 0, '6000'


def getSSID(num):
    """Gets the ssid of the required radio"""
    logger.debug("getSSID function is called")
    cmd_ssid = "uci get wireless.@wifi-iface[" + num + "].ssid"
    # command Execution-Start
    t = run_command(cmd_ssid)
    res = str(t)
    if t == 1:
        logger.error("5120:SSID[%s] is not Fetched", num)
        return 1, '5120'
    # command Execution-End
    logger.info("SSID[%s] is %s", num, res)
    return res, '6000'


def getEncryption():
    """Gets the encryption of the required radio"""
    logger.debug("getEncryption function is called")
    cmd_encryption = "uci get wireless.@wifi-iface[0].encryption"
    # command Execution-Start
    t = run_command(cmd_encryption)
    res = str(t)
    if t == 1:
        logger.error("5121:Encryption[0] is not Fetched")
        return 1, '5121'
    # command Execution-End
    logger.info("Encryption[0] %s", res)
    return res, '6000'


def getMode(num):
    """Gets the mode of the required radio"""
    logger.debug("getMode function is called")
    cmd_mode = "uci get wireless.@wifi-iface[" + num + "].mode"
    # command Execution-Start
    t = run_command(cmd_mode)
    res = str(t)
    if t == 1:
        logger.error("5122:Mode[%s] is not Fetched", num)
        return 1, '5122'
    # command Execution-End
    logger.info("Mode[%s] is %s", num, res)
    return res, '6000'


def getNetwork(num):
    """Gets the network of the required radio"""
    logger.debug("getNetwork function is called")
    cmd_network = "uci get wireless.@wifi-iface[" + num + "].network"
    # command Execution-Start
    t = run_command(cmd_network)
    res = str(t)
    if t == 1:
        logger.error("5123:Network[%s] is not Fetched", num)
        return 1, '5123'
    # command Execution-End
    logger.info("Network[%s] is %s", num, res)
    return res, '6000'


def getDevice(num):
    """Gets the device of the desired radio"""
    logger.debug("getDevice function is called")
    cmd_device = "uci get wireless.@wifi-iface[" + num + "].device"
    # command Execution-Start
    t = run_command(cmd_device)
    res = str(t)
    if t == 1:
        logger.error("5124:Device[%s] is not Fetched", num)
        return 1, '5124'
    # command Execution-End
    logger.info("Device[%s] is %s", num, res)
    return res, '6000'


def getMacaddr(num):
    """Gets the mac address of the radio"""
    logger.debug("getMacaddr function is called")
    cmd_mac = "uci get wireless.wifi" + num + ".macaddr"
    # command Execution-Start
    t = run_command(cmd_mac)
    res = str(t)
    if t == 1:
        logger.error("5125:macaddr[%s] is not Fetched", num)
        return 1, '5125'
    # command Execution-End
    logger.info("macaddr[%s] is %s", num, res)
    return res, '6000'


def getTxPower(num):
    """Gets the transmission power of the radio"""
    logger.debug("getTxpower function is called")
    cmd_tx = "uci get wireless.wifi" + num + ".txpower"
    # command Execution-Start
    t = run_command(cmd_tx)
    res = str(t)
    if t == 1:
        logger.error("5126:TxPower[%s] is not Fetched", num)
        return 1, '5126'
    # command Execution-End
    logger.info("TxPower[%s] is %s", num, res)
    return res, '6000'


def gettype(num):
    """Gets the type of the desired radio"""
    logger.debug("gettype function is called")
    cmd_type = "uci get wireless.wifi" + num + ".type"
    # command Execution-Start
    t = run_command(cmd_type)
    res = str(t)
    if t == 1:
        logger.error("5127:type[%s] is not Fetched", num)
        return 1, '5127'
    # command Execution-End
    logger.info("type[%s] is %s", num, res)
    return res, '6000'


def getChannel(num):
    """Gets the Channel of the Desired radio"""
    logger.debug("getChannel function is called")
    cmd_channel = "uci get wireless.wifi" + num + ".channel"
    # command Execution-Start
    t = run_command(cmd_channel)
    res = str(t)
    if t == 1:
        logger.error("5128:channel[%s] is not Fetched", num)
        return 1, '5128'
    # command Execution-End
    logger.info("channel[%s] is %s", num, res)
    return res, '6000'


def getHwmode(num):
    """Gets the Hwmode of the desired radio"""
    logger.debug("getHwmode function is called")
    cmd_hwmode = "uci get wireless.wifi" + num + ".hwmode"
    # command Execution-Start
    t = run_command(cmd_hwmode)
    res = str(t)
    if t == 1:
        logger.error("5129:Hwmode[%s] is not Fetched", num)
        return 1, '5129'
    # command Execution-End
    logger.info("Hwmode[%s] is %s", num, res)
    return res, '6000'


def getHtmode(num):
    """Gets the Htmode of the desired radio"""
    logger.debug("getHtmode function is called")
    cmd_htmode = "uci get wireless.wifi" + num + ".htmode"
    # command Execution-Start
    t = run_command(cmd_htmode)
    res = str(t)
    if t == 1:
        logger.error("5130:Htmode[%s] is not Fetched", num)
        return 1, '5130'
    # command Execution-End
    logger.info("Htmode[%s] is %s", num, res)
    return res, '6000'


def conso(att, num):
    """Gives the information based on the attribute and the desired radio"""
    logger.debug("conso function is called")
    # validating the argument type:
    t1 = num_check(num)
    if not (t1[0]):
        logger.error("5108:Input parameters are not Valid")
        return 1, t1[1]
    return {
        'ssid': getSSID(num),
        'encryption': getEncryption(),
        'macaddr': getMacaddr(num),
        'txpower': getTxPower(num),
        'type': gettype(num),
        'channel': getChannel(num),
        'hwmode': getHwmode(num),
        'htmode': getHtmode(num),
        'mode': getMode(num),
        'device': getDevice(num),
        'network': getNetwork(num),
        'all': conso_all(num)
    }.get(att, conso_error)


def conso_all(num):
    """Function is for internal calling"""
    logger.debug("conso_all function is called")
    res = [getSSID(num)]
    res.append(getEncryption())
    res.append(getMacaddr(num))
    res.append(getTxPower(num))
    res.append(gettype(num))
    res.append(getChannel(num))
    res.append(getHwmode(num))
    res.append(getHtmode(num))
    res.append(getMode(num))
    res.append(getDevice(num))
    res.append(getNetwork(num))
    return res


def conso_error():
    """Function is for internal calling"""
    logger.debug("conso_error function is called")
    logger.error("5103:No attribute found")
    return 1, '5103'


# validations on attributes:
def pass_check(str1):
    """Checks the password as per the specified format(format to be decided)"""
    logger.debug("pass_check function is called")
    if not isinstance(str1, str):
        logger.error("5106:password should be in string format")
        return 0, '5106'
    if (len(str1) > 7) and (len(str1) < 65):
        logger.debug("pass_check is done successfully")
        return 1, '6000'
    logger.error("5102:Password length should not be less than 8 characters and not more than 64 characters")
    return 0, '5102'


def num_check(num):
    """Checks the radio numbers"""
    logger.debug("num_check function is called")
    if not isinstance(num, str):
        logger.error("5104:Interface number is not in string format")
        return 0, '5104'
    # validating the radio number
    if num in radio:
        logger.debug("num_check is done successfully")
        return 1, '6000'
    logger.error("5101:The interface is not present")
    return 0, '5101'


def enc_check(enc):
    """Checks the encryption is proper or not"""
    logger.debug("enc_check function is called")
    if enc in enc_types:
        return 1, '6000'
    logger.error("5105:Encryption is not supported")
    return 0, '5105'


def ssid_check(ssid):
    """Checks the ssid type
    """
    logger.debug("ssid_check function is called")
    logger.debug("Checking whether ssid is NULL or not")
    if ssid == "":
        logger.debug("ssid is empty")
        return 0, '5117'
    logger.debug("Checking whether ssid is in string format or not")
    if isinstance(ssid, str):
        #Bug:19734-Start
        if ssid.isspace():
            logger.debug("all spaces in ssid are not allowed")
            return 0, '5133'
        #Bug:19734-End
        logger.debug("ssid_check is done successfully")
        return 1, '6000'
    logger.error("5107:ssid is not in string fromat")
    return 0, '5107'

def eth0_check():
    """Checking whether rj45 cable is connected or not"""
    try:
        output = os.system("ethtool eth0 | grep  'Link detected: yes'")
    except Exception, e:
        logger.critical('Unexpected Error', exc_info=True)
        output = '256'
    if not output:
        return True
    return False


def change_apcredentials(ssid, enc, password):
    """Function to change the ssid,password and encryption of the access point on both the radios."""
    logger.debug("change_apcredentials function is called")
    # validating the argument type:
    # t1 = ssid_check(ssid)
    # t2 = enc_check(enc)
    # t3 = pass_check(password)
    # if not (t1[0]):
    #     logger.error("5108:Input parameters are not Valid")
    #     return 1, t1[1]
    # elif not (t2[0]):
    #     logger.error("5108:Input parameters are not Valid")
    #     return 1, t2[1]
    # elif not (t3[0]):
    #     logger.error("5108:Input parameters are not Valid")
    #     return 1, t3[1]
    logger.debug(str(threading.enumerate()))
    thread_list = []
    for t in threading.enumerate():
        thread_list.append(t.name)
    if "rc_thread" in thread_list:
        logger.critical("rc_thread is already running")
        return 1, '5134'
    else:
        logger.debug("changing the credentials")
        rc_thread = threading.Thread(target=change_apcredentials_task, args=(ssid, password, enc), name="rc_thread")
        rc_thread.start()
        logger.debug("MA running threads list:" + str(threading.enumerate()))
    return 0, '6000'

def change_apcredentials_task(ssid, password, enc):
    logger.debug("running change credentials script in a thread")
    cmd_list = [
        "uci set wireless.@wifi-iface[0].ssid='" + ssid + "'",
        "uci set wireless.@wifi-iface[0].encryption=" + enc,
        "uci set wireless.@wifi-iface[0].key='" + password + "'",
        "uci set wireless.@wifi-iface[1].ssid='" + ssid + "'",
        "uci set wireless.@wifi-iface[1].encryption=" + enc,
        "uci set wireless.@wifi-iface[1].key='" + password + "'",
        "uci set shifu.conf.MA_ROUTER_SSID='" + ssid + "' && uci commit shifu",
	    "uci set shifu.conf.MA_ROUTER_PASSWORD='" + password + "' && uci commit shifu",
        "uci commit wireless",
        # "/etc/init.d/network restart",
        # "/etc/init.d/dropbear start"
        "wifi"
    ]
    # command execution-Start
    for cmd in cmd_list:
        t = run_command(cmd)
        if t == 1:
            logger.error("5132:Credentials are not changed")
            return 1, '5132'
    # command execution-End
    logger.info("Credentials are changed successfully")
    os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
    # time.sleep(10)
    # #ma.ma_profile_updates()
    # logger.debug("Sending signal after change credentials for profile updates")
    # os.system("kill -SIGUSR1 `ps | grep Mobile_Agent | grep -v grep | awk '{print $1}'`")



def get_wanaddress():
    """Getting the wan address from shifu"""
    logger.debug("get_wanaddress function is called")
    try:
        address = os.popen(". /lib/functions/network.sh; network_get_ipaddr ip wan; echo $ip").read().strip()
    except Exception, e:
        logger.error('Not able to get the wan address', exc_info=True)
        address = ''
    if not address:
        return "Error"
    address = str(address)
    logger.debug(address)
    octets = address.split('.')
    # changing the last octet of the wan address to 1: Assumption is always router keeps the x.x.x.1 as it's ip address
    address = octets[0] + '.' + octets[1] + '.' + octets[2] + '.' + '1'
    logger.debug("Legacy router address is %s", address)
    return address


def get_ssids():
    """Gives the ssids and signal strengths of all the existing accesspoints for 2.4GHz and 5GHz

    :return:array of ssids
    """
    logger.debug("get_ssids function is called")

    # Feature20116:Start WiFi Signal Strength Indication
    PdBm_max = 0        # max signal strength in dBm
    PdBm_min = -99      # min signal strength in dBm
    aps_list = []

    cmd_24Ghz = "iwlist ath0 scan | grep -e ESSID -e Signal"
    cmd_5Ghz = "iwlist ath1 scan | grep -e ESSID -e Signal"

    output_24GHz = os.popen(cmd_24Ghz).read().strip().split('\n')
    output_24GHz = map(str.strip, output_24GHz)

    for i in range(0, len(output_24GHz)):
        access_point_info = {}
        if('ESSID:' in output_24GHz[i]):
            access_point_info["ssid"] = output_24GHz[i][7:-1]
            i = i+1
            if('Signal level' in output_24GHz[i]):
                PdBm = int(output_24GHz[i].split('=')[2].split(' ')[0])
                ## converting dBm value into percentage eg: 0dBm = 101% and -99dBm = 1
                percent = (100 * (1- (PdBm_min - PdBm))) / (PdBm_max - PdBm_min)
                access_point_info["strength"] = percent
            else:
                access_point_info["strength"] = 0
            access_point_info["band"] = '2.4GHz'
            aps_list.append(access_point_info)
    logger.debug("2.4 list")
    # logger.debug(aps_list)
    # logger.debug("Len: %d", len(aps_list))

    ######
    output_5GHz = os.popen(cmd_5Ghz).read().strip().split('\n')
    output_5GHz = map(str.strip, output_5GHz)

    for i in range(0, len(output_5GHz)):
        access_point_info = {}
        if('ESSID:' in output_5GHz[i]):
            access_point_info["ssid"] = output_5GHz[i][7:-1]
            i = i+1
            if('Signal level' in output_5GHz[i]):
                PdBm = int(output_5GHz[i].split('=')[2].split(' ')[0])
                ## converting dBm value into percentage eg: 0dBm = 101% and -99dBm = 1
                percent = (100 * (1- (PdBm_min - PdBm))) / (PdBm_max - PdBm_min)
                access_point_info["strength"] = percent
            else:
                access_point_info["strength"] = 0
            access_point_info["band"] = '5GHz'
            aps_list.append(access_point_info)
    logger.debug("2.4 + 5.0 list")
        # logger.debug(aps_list)
        # logger.debug("Len: %d", len(aps_list))

    return aps_list
    # Feature20116:End

# Feature15958:Start
def shifu_connect_EBLR():
    """It connects the shifu to the Legacy Router,
     Wi-Fi-SON-CAP Configuration (Ethernet(E)-Backhaul(B) - from Legacy(L) Router(R))"""
    try:
        logger.debug("checking the MA_ROUTER_FLAG")
        eblr_flag = subprocess.check_output(["uci", "get", "shifu.conf.MA_ROUTER_FLAG"]).strip()
    except Exception, e:
        logger.critical('NV variable MA_ROUTER_FLAG is not present', exc_info=True)
    if eblr_flag == '0':
        return 1, '52XX'
    try:
        ssid = subprocess.check_output(["uci", "get", "shifu.conf.MA_ROUTER_SSID"]).strip()
    except Exception, e:
        logger.critical('NV variable MA_ROUTER_SSID is not present', exc_info=True)
        return 1, '5XXX'
    try:
        password = subprocess.check_output(["uci", "get", "shifu.conf.MA_ROUTER_PASSWORD"]).strip()
    except Exception, e:
        logger.critical('NV variable MA_ROUTER_PASSWORD is not present', exc_info=True)
        return 1, '5XXX'
    try:
        encryption = subprocess.check_output(["uci", "get", "shifu.conf.MA_ROUTER_ENCRYPTION"]).strip()
    except Exception, e:
        logger.critical('NV variable MA_ROUTER_ENCRYPTION is not present', exc_info=True)
        return 1, '5XXX'
    logger.debug(str(threading.enumerate()))
    thread_list = []
    for t in threading.enumerate():
        thread_list.append(t.name)
    if "rc_thread" in thread_list:
        logger.critical("rc_thread is already running")
        return 1, '5134'
    else:
        logger.debug("Running EBLR")
        rc_thread = threading.Thread(target=shifu_connect_EBLR_task, args=(ssid, password, encryption), name="rc_thread")
        rc_thread.start()
        logger.debug("MA running threads list:" + str(threading.enumerate()))
    return 0, '6000'

def shifu_connect_EBLR_task(ssid,password, encryption):
    # generating the command with the argument.
    logger.debug("running EBLR script in a thread")
    cmd_list = [
        "uci set wireless.@wifi-iface[0].ssid='" + ssid + "'",  # set SSID for AP interface on radio 0
        "uci set wireless.@wifi-iface[0].encryption=" + encryption,  # set encryption for AP interface on radio 0
        "uci set wireless.@wifi-iface[0].key='" + password + "'",  # set password for AP interface on radio 0
        "uci set wireless.@wifi-iface[0].wnm=1",  # set Wireless Network Management capability for radio 0
        "uci set wireless.@wifi-iface[0].configuration=EBLR",
        "uci set wireless.wifi0.repacd_auto_create_vaps=0",
        # disabling the auto-creation of Virtual Access Points on radio 0
        "uci set wireless.@wifi-iface[1].ssid='" + ssid + "'",  # set SSID for AP interface on radio 1
        "uci set wireless.@wifi-iface[1].encryption=" + encryption,  # set encryption for AP interface on radio 1
        "uci set wireless.@wifi-iface[1].key='" + password + "'",  # set password for AP interface on radio 1
        "uci set wireless.@wifi-iface[1].wnm=1",  # set WNM(Wireless network managements) capability for radio 1
        "uci set wireless.@wifi-iface[1].configuration=EBLR",
        "uci set wireless.wifi1.repacd_auto_create_vaps=0",  # disabling the auto-creation of VAPs on radio 1
	# 21301-Assigning default country code as India and removing throughput patch start.
        "uci set wireless.@wifi-device[0].country=0x138E",  # Setting country code for Radio-0 to IN(India)
        "uci set wireless.@wifi-device[1].country=0x138E",  # Setting country code for Radio-1 to IN(India)
	# 21301-Assigning default country code as India and removing throughput patch end.
	# 21064: Throughput improvement start
	"uci set wireless.wifi0.htmode=HT40", # Setting bandwidth (2*20MHz channels) for 2.4GHz radio
	"uci set wireless.wifi1.htmode=VHT80", # Setting bandwidth (2*40MHz channels) for 5GHz radio
	"uci set wireless.wifi0.hwmode=11n", # Selects 802.11n protocol for 2.4GHz radio 
	"uci set wireless.wifi1.hwmode=11ac", # Selects 802.11ac protocol for 5GHz radio
	"uci set wireless.@wifi-iface[0].wds=1", # Allows connecting OpenWrt based station devices to Shifu seamlessly
	"uci set wireless.@wifi-iface[1].wds=1", # Allows connecting OpenWrt based station devices to Shifu seamlessly
	# 21064: Throughput improvement end
        "uci commit wireless",  # save wireless settings
        "uci set firewall.@zone[1].input=ACCEPT",  # disable firewall interface under zone 1
        "uci commit firewall",  # save firewall settings
        "/etc/init.d/firewall reload",  # reload the firewall
        "uci set lbd.StaDB.MarkAdvClientAsDualBand=1",  # set band steering parameter
        "uci set lbd.SteerExec_Adv.StartInBTMActiveState=1",  # set band steering parameter
        "uci commit lbd",  # save Load Balancing Deamon service settings
        "uci set hyd.Topology.ENABLE_NOTIFICATION_UNICAST=1",  # set AP steering parameter
        "uci set hyd.Topology.PERIODIC_QUERY_INTERVAL=15",  # set AP steering parameter
        "uci set hyd.@Vlanid[0].vid=2",  # setting the number of VLAN line for radio 0
        "uci set hyd.@Vlanid[1].vid=1",  # setting the number of VLAN line for radio 1
        "uci commit hyd",  # save hyd settings
        "uci set repacd.repacd.Enable=1",  # enable repacd (Repeater placement deamon)
        "uci set repacd.WiFiLink.DaisyChain=1",  # enable daisy chain feature
        "uci commit repacd",  # save repacd settings
        "wifi",  # reload wifi daemons
        "/etc/init.d/repacd start",  # start repacd daemon
        "iwpriv ath0 wps 1",  # enable wps functionality on radio 0
        "iwpriv ath1 wps 1"  # enable wps functionality on radio 1
    ]
    # Commands-Execution: Start
    for cmd in cmd_list:
        t = run_command(cmd)
        if t == 1:
            logger.error("5109:Shifu-Legacy EBLR connection failed ")
            return 1, '5109'
    # Commands-Execution: End
    try:
        os.system("uci set shifu.conf.MA_ROUTER_FLAG=0 && uci commit shifu")
    except Exception, e:
        logger.critical('Not able to access the NV variable: MA_ROUTER_FLAG', exc_info=True)
        return 1, '5XXX'
    logger.info("Shifu-Legacy EBLR connected successfully")
    os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
    # ma.ma_profile_updates()
    logger.debug("Sending signal after EBLR for profile updates")
    os.system("kill -SIGUSR1 `ps | grep Mobile_Agent | grep -v grep | awk '{print $1}'`")



def store_credentials(ssid, password, encryption):
    """This function validates and stores the credentials into NV variables"""
    logger.debug("store_credentials function is called")
    t1 = pass_check(password)
    if not (t1[0]):
        logger.error("5108:Input parameters are not Valid")
        return 1, t1[1]
    t2 = ssid_check(ssid)
    if not (t2[0]):
        logger.error("5108:Input parameters are not Valid")
        return 1, t2[1]
    t3 = enc_check(encryption)
    if not (t3[0]):
        logger.error("5105: Encryption is not supported")
        return 1, t3[1]
    try:
        os.system("uci set shifu.conf.MA_ROUTER_SSID='" + ssid + "' && uci commit shifu")
    except Exception, e:
        logger.critical('Not able to access the NV variable: MA_ROUTER_SSID', exc_info=True)
        return 1, '5XXX'
    try:
        os.system("uci set shifu.conf.MA_ROUTER_PASSWORD='" + password + "' && uci commit shifu")
    except Exception, e:
        logger.critical('Not able to access the NV variable: MA_ROUTER_PASSWORD', exc_info=True)
        return 1, '5XXX'
    try:
        os.system("uci set shifu.conf.MA_ROUTER_ENCRYPTION=%s && uci commit shifu" % (encryption,))
    except Exception, e:
        logger.critical('Not able to access the NV variable: MA_ROUTER_ENCRYPTION', exc_info=True)
        return 1, '5XXX'
    try:
        os.system("uci set shifu.conf.MA_ROUTER_FLAG=1 && uci commit shifu")
    except Exception, e:
        logger.critical('Not able to access the NV variable: MA_ROUTER_FLAG', exc_info=True)
        return 1, '5XXX'
    logger.info("EBLR credentials are stored successfully")
    return 0, '6000'

def shifu_connect_WBLR(ssid, password):
    """It connects the shifu to the Legacy Router,
     Wi-Fi-SON-CAP Configuration (Wifi(W)-Backhaul(B) - from Legacy(L) Router(R))"""
    logger.debug("connect function is called")
    # t1 = pass_check(password)
    # if not (t1[0]):
    #     logger.error("5108:Input parameters are not Valid")
    #     return 1, t1[1]
    # t2 = ssid_check(ssid)
    # if not (t2[0]):
    #     logger.error("5108:Input parameters are not Valid")
    #     return 1, t2[1]
    os.system("uci set shifu.conf.MA_ROUTER_SSID='t.hub-" + ssid + "' && uci commit shifu")
    os.system("uci set shifu.conf.MA_ROUTER_PASSWORD='" + password + "' && uci commit shifu")
    os.system("uci set shifu.conf.MA_LEGACY_ROUTER_SSID='" + ssid + "' && uci commit shifu")
    os.system("uci set shifu.conf.MA_LEGACY_ROUTER_PASSWORD='" + password + "' && uci commit shifu")
    logger.debug("ssid and password are copied into NV")
    logger.debug(str(threading.enumerate()))
    thread_list = []
    for t in threading.enumerate():
        thread_list.append(t.name)
    if "rc_thread" in thread_list:
        logger.critical("rc_thread is already running")
        return 1, '5134'
    else:
        logger.debug("Running WBLR")
        rc_thread = threading.Thread(target=shifu_connect_WBLR_task, args=(ssid,password), name="rc_thread")
        rc_thread.start()
        logger.debug("MA running threads list:" + str(threading.enumerate()))
    return 0, '6000'


def shifu_connect_WBLR_task(ssid,password):
    logger.debug("running WBLR script in a thread")
    logger.debug("setting the MA_ROUTER_FALG to 2")
    os.system("uci set shifu.conf.MA_ROUTER_FLAG=2 && uci commit shifu")
    logger.debug("check whether SSID is in 5GHz or in 2.4GHz")
    output = os.popen("iwlist ath1 scan | grep ESSID").read().strip().split('\n')
    output = map(str.strip, output)
    output = [x[7:-1] for x in output]
    if ssid in output:
        logger.debug("SSID is found in 5GHz list")
        interface = "1"
    else:
        logger.debug("SSID is found in 2.4GHz list")
        interface = "0"
    # generating the command with the argument.
    cmd_list = [
        "uci set wireless.@wifi-iface[0].ssid='" + "t.hub-" + ssid + "'",   # set SSID for AP interface on radio 0
        "uci set wireless.@wifi-iface[0].encryption=psk2+ccmp",  # set encryption for AP interface on radio 0
        "uci set wireless.@wifi-iface[0].key='" + password + "'",  # set password for AP interface on radio 0
        "uci set wireless.@wifi-iface[0].configuration=WBLR",  # mode of configuration
        "uci set wireless.@wifi-iface[1].ssid='" + "t.hub-" + ssid + "'",   # set SSID for AP interface on radio 1
        "uci set wireless.@wifi-iface[1].encryption=psk2+ccmp",  # set encryption for AP interface on radio 1
        "uci set wireless.@wifi-iface[1].key='" + password + "'",  # set password for AP interface on radio 1
        "uci set wireless.@wifi-iface[1].configuration=WBLR",  # mode of configuration
        "uci set wireless.@wifi-iface[0].wnm=1",  # set WNM(Wireless network managements) capability for radio 0
        "uci set wireless.wifi0.repacd_auto_create_vaps=0",
        # disabling the auto-creation of Virtual Access Points on radio 0
        "uci set wireless.@wifi-iface[1].wnm=1",  # set WNM(Wireless network managements) capability for radio 1
        "uci set wireless.wifi1.repacd_auto_create_vaps=0",
        # disabling the auto-creation of Virtual Access Points on radio 1
        "uci set wireless.qcawifi=qcawifi",
        "uci add wireless wifi-iface",
        "uci set wireless.@wifi-iface[2].network=wan",  # Create a interface for WAN port for backhaul creation
        "uci set wireless.@wifi-iface[2].device=wifi" + interface,  # wifi1 for 5Ghz backhaul only
        "uci set wireless.@wifi-iface[2].mode=sta",  # Defining mode of interface
        "uci set wireless.@wifi-iface[2].extap=1",  # The extap logic introduces a smart MAC address masking
        "uci set wireless.@wifi-iface[2].encryption=psk2",  # set encryption for AP interface on backhaul interface
        "uci set wireless.@wifi-iface[2].ssid='" + ssid + "'",  # set SSID for AP interface on backhaul radio
        "uci set wireless.@wifi-iface[2].key='" + password + "'",  # set password for AP interface on backhaul
	# 21301-Assigning default country code as India and removing throughput patch start.
        "uci set wireless.@wifi-device[0].country=0x164",  # Setting country code for Radio-0 to IN(India)
        "uci set wireless.@wifi-device[1].country=0x164",  # Setting country code for Radio-1 to IN(India)
	# 21301-Assigning default country code as India and removing throughput patch end.
	# 21064: Throughput improvement start
	"uci set wireless.wifi0.htmode=HT40", # Setting bandwidth (2*20MHz channels) for 2.4GHz radio
	"uci set wireless.wifi1.htmode=VHT80", # Setting bandwidth (2*40MHz channels) for 5GHz radio
	"uci set wireless.wifi0.hwmode=11n", # Selects 802.11n protocol for 2.4GHz radio
	"uci set wireless.wifi1.hwmode=11ac", # Selects 802.11ac protocol for 5GHz radio
	"uci set wireless.@wifi-iface[0].wds=1", # Allows connecting OpenWrt based station devices to Shifu seamlessly
	"uci set wireless.@wifi-iface[1].wds=1", # Allows connecting OpenWrt based station devices to Shifu seamlessly
	"uci set wireless.@wifi-iface[2].wds=1", # Allows connecting OpenWrt based station devices to Shifu seamlessly
	# 21064: Throughput improvement end
        "uci commit wireless",  # save wireless Balancing Deamon service settings
        "uci set lbd.StaDB.MarkAdvClientAsDualBand=1",  # set band steering parameter
        "uci set lbd.SteerExec_Adv.StartInBTMActiveState=1",  # set band steering parameter
        "uci commit lbd",  # reload the load balancing deamon
        "uci set hyd.Topology.ENABLE_NOTIFICATION_UNICAST=1",  # set AP steering parameter
        "uci set hyd.Topology.PERIODIC_QUERY_INTERVAL=15",  # set AP steering parameter
        "uci set hyd.@Vlanid[0].vid=2",  # setting the number of VLAN line for radio 0
        "uci set hyd.@Vlanid[1].vid=1",  # setting the number of VLAN line for radio 1
        "uci commit hyd",  # reload the hyd deamon
        "uci set firewall.@zone[1].input=ACCEPT",  # disable firewall interface under zone 1
        "uci commit firewall",  # save firewall settings
        "/etc/init.d/firewall reload",  # reload the firewall
        # 21075-auto credentials confiuration-start
        # REPACD is responsible for same SSID maintainace in between 2.4G and 5G.
        "uci set repacd.repacd.Enable=1",
        "uci set repacd.WiFiLink.DaisyChain=1",
        "uci commit repacd",
        "uci set network.wan._orig_bridge=true",  # Enable bridge interface for WAN port
        "uci set network.wan.type=bridge",  # Enable bridge interface for WAN port
        "uci commit network",  # save network settings
        "wifi",  # reload wifi daemons
        "/etc/init.d/repacd start",
        # 21075-Enabled REPACD start command in WBLR mode configuration.
        "iwpriv ath0 wps 1",  # enable wps functionality on radio 0
        "iwpriv ath1 wps 1"  # enable wps functionality on radio 1
    ]
    # Commands-Execution: Start
    #os.system("uci set shifu.conf.MA_ROUTER_FLAG=2 && uci commit shifu")
    for cmd in cmd_list:
        t = run_command(cmd)
        if t == 1:
            logger.error("5109:Shifu-Legacy WBLR configuration failed ")
            return 1, '5109'
    # Commands-Execution: End
    os.system("uci set shifu.conf.MA_ROUTER_FLAG=0 && uci commit shifu")
    connection_status = check_legacy_connection()
    if connection_status == 'success':
        logger.info("Shifu-Legacy WBLR connected successfully")
        TTS = {
            "client_name": "MA",
            "client_action": "VOICE",
            "client_data": {
                "online": "I am now connected to " + ssid + " WiFi",
                "offline": None,
                "led": False
            }
        }
        logger.debug("sending voice response format to VA Socket")
        logger.debug(json.dumps(TTS))
        va_socket(json.dumps(TTS))

def shifu_connect_WBLR_fronthaul(hub_ssid,hub_password):
    """sets ssid and password of thub fronthaul
    """
    logger.debug("WBLR fronthaul connect function is called")
    t1 = pass_check(hub_password)
    if not (t1[0]):
        logger.error("5108:Input parameters are not Valid")
        return 1, t1[1]
    t2 = ssid_check(hub_ssid)
    if not (t2[0]):
        logger.error("5108:Input parameters are not Valid")
        return 1, t2[1]
    os.system("uci set shifu.conf.MA_ROUTER_SSID='" + hub_ssid + "' && uci commit shifu")
    os.system("uci set shifu.conf.MA_ROUTER_PASSWORD='" + hub_password + "' && uci commit shifu")
    logger.debug(str(threading.enumerate()))
    thread_list = []
    for t in threading.enumerate():
        thread_list.append(t.name)
    if "rc_thread" in thread_list:
        logger.critical("rc_thread is already running")
        return 1, '5134'
    else:
        logger.debug("Running WBLR")
        rc_thread = threading.Thread(target=shifu_connect_WBLR_fronthaul_task, args=(hub_ssid,hub_password), name="rc_thread")
        rc_thread.start()
        logger.debug("MA running threads list:" + str(threading.enumerate()))
    return 0, '6000'

def shifu_connect_WBLR_backhaul(legacy_ssid,legacy_password):
    """It connects the shifu to the Legacy Router,
     Wi-Fi-SON-CAP Configuration (Wifi(W)-Backhaul(B) - from Legacy(L) Router(R))"""
    logger.debug("WBLR backhaul connect function is called")
    # t3 = pass_check(legacy_password)
    # if not (t3[0]):
    #     logger.error("5108:Input parameters are not Valid")
    #     return 1, t3[1]
    # t4 = ssid_check(legacy_ssid)
    # if not (t4[0]):
    #     logger.error("5108:Input parameters are not Valid")
    #     return 1, t4[1]
    os.system("uci set shifu.conf.MA_LEGACY_ROUTER_SSID='" + legacy_ssid + "' && uci commit shifu")
    os.system("uci set shifu.conf.MA_LEGACY_ROUTER_PASSWORD='" + legacy_password + "' && uci commit shifu")
    os.system("uci set shifu.conf.WPA_AUTH_STATUS='NULL' && uci commit shifu")

    logger.debug(str(threading.enumerate()))
    thread_list = []
    for t in threading.enumerate():
        thread_list.append(t.name)
    if "rc_thread" in thread_list:
        logger.critical("rc_thread is already running")
        return 1, '5134'
    else:
        logger.debug("Running WBLR")
        rc_thread = threading.Thread(target=shifu_connect_WBLR_backhaul_task, args=(legacy_ssid,legacy_password), name="rc_thread")
        rc_thread.start()
        logger.debug("MA running threads list:" + str(threading.enumerate()))
    return 0, '6000'

def shifu_connect_WBLR_fronthaul_task(hub_ssid,hub_password):
    logger.debug("running WBLR fronthaul script in a thread")
    logger.debug("setting the MA_ROUTER_FLAG to 2")
    os.system("uci set shifu.conf.MA_ROUTER_FLAG=2 && uci commit shifu")
    # generating the command with the argument.
    cmd_list = [
        "uci set wireless.@wifi-iface[0].ssid='" + hub_ssid + "'",   # set SSID for AP interface on radio 0
        "uci set wireless.@wifi-iface[0].encryption=psk2+ccmp",  # set encryption for AP interface on radio 0
        "uci set wireless.@wifi-iface[0].key='" + hub_password + "'",  # set password for AP interface on radio 0
        #"uci set wireless.@wifi-iface[0].configuration=WBLR",  # mode of configuration
        "uci set wireless.@wifi-iface[1].ssid='" + hub_ssid + "'",   # set SSID for AP interface on radio 1
        "uci set wireless.@wifi-iface[1].encryption=psk2+ccmp",  # set encryption for AP interface on radio 1
        "uci set wireless.@wifi-iface[1].key='" + hub_password + "'",  # set password for AP interface on radio 1
        #"uci set wireless.@wifi-iface[1].configuration=WBLR",  # mode of configuration
        "uci set wireless.@wifi-iface[0].wnm=1",  # set WNM(Wireless network managements) capability for radio 0
        "uci set wireless.wifi0.repacd_auto_create_vaps=0",
        # disabling the auto-creation of Virtual Access Points on radio 0
        "uci set wireless.@wifi-iface[1].wnm=1",  # set WNM(Wireless network managements) capability for radio 1
      "uci set wireless.wifi1.repacd_auto_create_vaps=0",
        # disabling the auto-creation of Virtual Access Points on radio 1
       # "uci set wireless.qcawifi=qcawifi",
       # "uci add wireless wifi-iface",
     #    "uci set wireless.@wifi-iface[2].network=wan",  # Create a interface for WAN port for backhaul creation
     #    "uci set wireless.@wifi-iface[2].device=wifi" + interface,  # wifi1 for 5Ghz backhaul only
     #    "uci set wireless.@wifi-iface[2].mode=sta",  # Defining mode of interface
     #    "uci set wireless.@wifi-iface[2].extap=1",  # The extap logic introduces a smart MAC address masking
     #    "uci set wireless.@wifi-iface[2].encryption=psk2",  # set encryption for AP interface on backhaul interface
     #    "uci set wireless.@wifi-iface[2].ssid='" + legacy_ssid + "'",  # set SSID for AP interface on backhaul radio
     #    "uci set wireless.@wifi-iface[2].key='" + legacy_password + "'",  # set password for AP interface on backhaul
	# 21301-Assigning default country code as India and removing throughput patch start.
        "uci set wireless.@wifi-device[0].country=0x138E",  # Setting country code for Radio-0 to IN(India)
        "uci set wireless.@wifi-device[1].country=0x138E",  # Setting country code for Radio-1 to IN(India)
	# 21301-Assigning default country code as India and removing throughput patch end.
	# 21064: Throughput improvement start
	"uci set wireless.wifi0.htmode=HT40", # Setting bandwidth (2*20MHz channels) for 2.4GHz radio
	"uci set wireless.wifi1.htmode=VHT80", # Setting bandwidth (2*40MHz channels) for 5GHz radio
	"uci set wireless.wifi0.hwmode=11n", # Selects 802.11n protocol for 2.4GHz radio 
	"uci set wireless.wifi1.hwmode=11ac", # Selects 802.11ac protocol for 5GHz radio
	"uci set wireless.@wifi-iface[0].wds=1", # Allows connecting OpenWrt based station devices to Shifu seamlessly
	"uci set wireless.@wifi-iface[1].wds=1", # Allows connecting OpenWrt based station devices to Shifu seamlessly
	#"uci set wireless.@wifi-iface[2].wds=1", # Allows connecting OpenWrt based station devices to Shifu seamlessly
	# 21064: Throughput improvement end
        "uci commit wireless",  # save wireless Balancing Deamon service settings
        "uci set lbd.StaDB.MarkAdvClientAsDualBand=1",  # set band steering parameter
        "uci set lbd.SteerExec_Adv.StartInBTMActiveState=1",  # set band steering parameter
        "uci commit lbd",  # reload the load balancing deamon
        "uci set hyd.Topology.ENABLE_NOTIFICATION_UNICAST=1",  # set AP steering parameter
        "uci set hyd.Topology.PERIODIC_QUERY_INTERVAL=15",  # set AP steering parameter
        "uci set hyd.@Vlanid[0].vid=2",  # setting the number of VLAN line for radio 0
        "uci set hyd.@Vlanid[1].vid=1",  # setting the number of VLAN line for radio 1
        "uci commit hyd",  # reload the hyd deamon
        "uci set firewall.@zone[1].input=ACCEPT",  # disable firewall interface under zone 1
        "uci commit firewall",  # save firewall settings
        "/etc/init.d/firewall reload",  # reload the firewall
        # 21075-auto credentials confiuration-start
        # REPACD is responsible for same SSID maintainace in between 2.4G and 5G.
        "uci set repacd.repacd.Enable=1",
        "uci set repacd.WiFiLink.DaisyChain=1",
        "uci commit repacd",
        # "uci set network.wan._orig_bridge=true",  # Enable bridge interface for WAN port
        # "uci set network.wan.type=bridge",  # Enable bridge interface for WAN port
        #"uci commit network",  # save network settings
        "wifi",  # reload wifi daemons
        "/etc/init.d/repacd start",
        # 21075-Enabled REPACD start command in WBLR mode configuration.
        "iwpriv ath0 wps 1",  # enable wps functionality on radio 0
        "iwpriv ath1 wps 1"  # enable wps functionality on radio 1
    ]
    # Commands-Execution: Start
    #os.system("uci set shifu.conf.MA_ROUTER_FLAG=2 && uci commit shifu")
    for cmd in cmd_list:
        t = run_command(cmd)
        if t == 1:
            logger.error("5109:Shifu-Legacy WBLR configuration failed ")
            return 1, '5109'
    # Commands-Execution: End
    os.system("uci set shifu.conf.MA_ROUTER_FLAG=0 && uci commit shifu")
    # connection_status = check_legacy_connection()
    # if connection_status == 'success':
    #     logger.info("Shifu-Legacy WBLR connected successfully")
    #     TTS = {
    #         "client_name": "MA",
    #         "client_action": "VOICE",
    #         "client_data": {
    #             "online": "I am now connected to " + legacy_ssid + " WiFi",
    #             "offline": None,
    #             "led": False
    #         }
    #     }
    #     logger.debug("sending voice response format to VA Socket")
    #     logger.debug(json.dumps(TTS))
    #     va_socket(json.dumps(TTS))

def shifu_connect_WBLR_backhaul_task(legacy_ssid, legacy_password):
    logger.debug("running WBLR backhaul script in a thread")
    logger.debug("setting WBLR backhaul MA_ROUTER_FLAG to 2")
    os.system("uci set shifu.conf.MA_ROUTER_FLAG=2 && uci commit shifu")
    logger.debug("check whether SSID is in 5GHz or in 2.4GHz")
    output = os.popen("iwlist ath1 scan | grep ESSID").read().strip().split('\n')
    output = map(str.strip, output)
    output = [x[7:-1] for x in output]
    if legacy_ssid in output:
        logger.debug("SSID is found in 5GHz list")
        interface = "1"
    else:
        logger.debug("SSID is found in 2.4GHz list")
        interface = "0"
    # generating the command with the argument.
    cmd_list = [
            # "uci set dhcp.lan.ignore=0",
            # "uci commit dhcp",
         #new commands
            "rm -f /etc/config/network /etc/config/wireless",
            "wifi detect > /etc/config/wireless",
            "/rom/etc/uci-defaults/network",


            # "uci set network.lan.proto=static",   # Not required any more
            # "/etc/init.d/dnsmasq restart",     # Not required any more

            "uci set wireless.wifi0.repacd_auto_create_vaps=0",
            "uci set wireless.wifi1.repacd_auto_create_vaps=0",
            "uci set wireless.qcawifi=qcawifi",
            "uci add wireless wifi-iface",
            "uci set wireless.@wifi-iface[0].configuration=WBLR",
            "uci set wireless.@wifi-iface[1].configuration=WBLR",
             "uci set wireless.@wifi-iface[2].network=wan",  # Create a interface for WAN port for backhaul creation
             "uci set wireless.@wifi-iface[2].device=wifi" + interface,  # wifi1 for 5Ghz backhaul only
            "uci set wireless.@wifi-iface[2].mode=sta",  # Defining mode of interface
            "uci set wireless.@wifi-iface[2].encryption=psk2+ccmp",  # set encryption for AP interface on backhaul interface
            "uci set wireless.@wifi-iface[2].ssid='" + legacy_ssid + "'",  # set SSID for AP interface on backhaul radio
            "uci set wireless.@wifi-iface[2].key='" + legacy_password + "'",  # set password for AP interface on backhaul
            "uci set wireless.@wifi-iface[2].extap=1",  # The extap logic introduces a smart MAC address masking
            "uci set wireless.@wifi-iface[2].wds=1", # Allows connecting OpenWrt based station devices to Shifu seamlessly
            "uci commit wireless",
            "uci set network.wan._orig_bridge=true",  # Enable bridge interface for WAN port
            "uci set network.wan.type=bridge",  # Enable bridge interface for WAN port
            "uci commit network",  # save network settings
            "wifi"  # reload wifi daemons
            #"/etc/init.d/network restart",
            #"/etc/init.d/dropbear start"

    ]
    # Commands-Execution: Start
    # os.system("uci set shifu.conf.MA_ROUTER_FLAG=2 && uci commit shifu")
    for cmd in cmd_list:
        t = run_command(cmd)
        if t == 1:
            logger.error("5109:Shifu-Legacy WBLR backhaul configuration failed ")
            return 1, '5109'
    # Commands-Execution: End
    os.system("uci set shifu.conf.MA_ROUTER_FLAG=0 && uci commit shifu")

# Feature:20136-Start
# def qwrap_reset():
def network_reset():
# Feature:20136-End
    """Resets the network configuration"""
    logger.debug("network_reset function is called")
    logger.debug(str(threading.enumerate()))
    thread_list = []
    for t in threading.enumerate():
        thread_list.append(t.name)
    if "rc_thread" in thread_list:
        logger.critical("rc_thread is already running")
        return 1, '5134'
    else:
        logger.debug("network is getting reset back to Default")
        rc_thread = threading.Thread(target=network_reset_task, name="rc_thread")
        rc_thread.start()
        logger.debug("MA running threads list:" + str(threading.enumerate()))
    return 0, '6000'

# OLD network reset. new one will be done below this.
'''
def network_reset_task():
    logger.debug("network_reset function is being executed in a thread")
    cmd_list = [
        "uci set dhcp.lan.ignore=0",
        "uci commit dhcp",
        "uci set firewall.@zone[1].input='REJECT'",
        "uci commit firewall",
        "/etc/init.d/firewall reload",
        "/etc/init.d/dnsmasq restart",
        "rm -f /etc/config/network /etc/config/wireless",
        "wifi detect > /etc/config/wireless",
        "/rom/etc/uci-defaults/network",
        "uci set lbd.StaDB.MarkAdvClientAsDualBand=0",
        "uci set lbd.SteerExec_Adv.StartInBTMActiveState=0",
        "uci commit lbd",
        "uci set hyd.Topology.ENABLE_NOTIFICATION_UNICAST=0",
        "uci set hyd.Topology.PERIODIC_QUERY_INTERVAL=60",
        "uci set hyd.@Vlanid[0].vid=1",
        "uci set hyd.@Vlanid[1].vid=2",
        "uci commit hyd",
        "uci set repacd.repacd.Enable=0",
        "uci set repacd.WiFiLink.DaisyChain=1",
        "uci commit repacd",
        "uci set wireless.@wifi-iface[0].configuration=DEFAULT",
        "uci set wireless.@wifi-iface[1].configuration=DEFAULT",
        "uci commit wireless",
        "wifi",
        # Redmine:20336: Start Clear flag status during network reset to maintain Shifu in default mode
        "uci set shifu.conf.MA_EBLR_FLAG=0 && uci commit shifu", #No need to set it anymore as now only MA_ROUTER_FLAG is used
        "uci set shifu.conf.MA_ROUTER_FLAG=0 && uci commit shifu",
        # Redmine:20336 :End
        "iwpriv ath0 wps 1",
        "iwpriv ath1 wps 1"
    ]
    for cmd in cmd_list:
        #start of Bug #21048
        if cmd == "/rom/etc/uci-defaults/network":
            s = os.system(cmd)
            if s == 0:
                s = 0
            else:
                cmd = "/rom/rom/etc/uci-defaults/network"
                logger.debug("network file could not be found on:/rom/etc/uci-defaults/. Trying other location ")
                s = os.system(cmd)
        #End of Bug #21048
        else:
            s = os.system(cmd)
        if s != 0:
            logger.debug(cmd + "Execution failed")
            logger.debug("Could not reset the network configuration")
            return 'failure'
        logger.debug(cmd + " Executed successfully")
    os.system("uci set shifu.conf.MA_ROUTER_SSID='NULL" + "' && uci commit shifu")
    os.system("uci set shifu.conf.MA_ROUTER_PASSWORD='NULL" + "' && uci commit shifu")
    os.system("uci set shifu.conf.MA_LEGACY_ROUTER_SSID='NULL" +  "' && uci commit shifu")
    os.system("uci set shifu.conf.MA_LEGACY_ROUTER_PASSWORD='NULL" +  "' && uci commit shifu")
    os.system("uci set shifu.conf.WPA_AUTH_STATUS='NULL' && uci commit shifu")

    logger.debug('network configuration reset is done')
    os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
    # ma.ma_profile_updates()
    logger.debug("Sending signal after network reset for profile updates")
    os.system("kill -SIGUSR1 `ps | grep Mobile_Agent | grep -v grep | awk '{print $1}'`")
'''

# NEW network reset task based on script given by Chari
def network_reset_task():
    """
    This method will be called by Mobile Agent when user wants to do a network reset. or
    during onboarding process, when configurations are not correct. it will be called interanally.
    :return: None
    """
    logger.debug("network_reset function is being executed in a thread")
    cmd_list = [
        "rm -f /etc/config/network /etc/config/wireless",
        "wifi detect > /etc/config/wireless",
        "cp /rom/etc/config/repacd /etc/config/",
        "uci set dhcp.lan.ignore=0",
        "uci commit dhcp",
        "uci set firewall.@zone[1].input='REJECT'",
        "uci commit firewall",
        "/rom/etc/uci-defaults/network",
        "uci set shifu.conf.NETWORK_MODE='DEFAULT'",
        "uci set shifu.conf.MA_EBLR_FLAG='0'",
        "uci set shifu.conf.MA_ROUTER_FLAG='0'",
        "uci set shifu.conf.MA_ROUTER_SSID='NULL'",
        "uci set shifu.conf.MA_ROUTER_PASSWORD='NULL'",
        "uci set shifu.conf.MA_LEGACY_ROUTER_SSID='NULL'",
        "uci set shifu.conf.MA_LEGACY_ROUTER_PASSWORD='NULL'",
        "uci commit shifu",
        "/etc/init.d/firewall restart",
        "/etc/init.d/repacd start",
        "/etc/init.d/dnsmasq restart",
        ]
    for cmd in cmd_list:
        t = run_command(cmd)
        if t == 1:
            logger.error("5109:network reset on t.hub failed ")
            return 1, '5109'

    # Commands-Execution: End








#  checking the internet connectivity and DHCP connection.
# Feature:20136-Start
# def check_legacy_connection5():
def check_legacy_connection():
# Feature:20136-End
    """To check Legacy router connection by observing bit rate values of 2.4GHz and 5.0GHz wifi interfaces"""
    # Feature:20136-start
    # logger.debug("Executing he check_legacy_connection5 function")
    logger.debug("Executing the check_legacy_connection function")    # Feature:20136-End

    try:
        flag = subprocess.check_output(["uci", "get", "shifu.conf.WPA_AUTH_STATUS"]).strip()
    except Exception as e:
        logger.debug("could not read WPA Auth status : {}".format(e))
        return  ('error',500)

    if flag == 'SUCCESS':
        logger.debug("Connected to Legacy router")
        return ('success',200)
    elif flag == 'NULL':
        logger.debug("Unable to connect. Legacy router may be switched off or not in range")
        return ('error',601)
    else:
        logger.debug("Unable to connect legacy router, may be wrong password")
        return ('error',602)


# Start: (Feature: 19113)
def wifi_switch(switch):
    """disables the Shifu Backhaul connection"""

    # commands to do disbale the Shifu Backhaul configuration
    logger.debug("wifi_switch function is being executed")
    if switch == True:
        cmd_list = [
            "uci set dhcp.lan.ignore=0",
            "uci set network.@switch_vlan[1].ports='0t 5'",
            "uci commit network",
            "/etc/init.d/network restart"
        ]
    else:
        cmd_list = [
            "uci set dhcp.lan.ignore=0",
            "uci set network.@switch_vlan[1].ports='0t'",
            "uci commit network",
            "/etc/init.d/network restart"
        ]
    for cmd in cmd_list:
        s = os.system(cmd)
        if s != 0:
            logger.debug(cmd + "Execution failed")
            logger.debug("not able to switch the backhaul configuration")
            return 1
        logger.debug(cmd + " Executed successfully")
    logger.debug('Shifu backhaul configuration is switched')
    return 0
# End

def get_legacy():
    """
    function to get the legacy router name
    :return: legacy router name
    """
    legacy_router_ssid = None
    try:
        legacy_router_ssid = ast.literal_eval(
            subprocess.check_output("iwconfig ath11 | grep ESSID | awk -F: '{print $2}'", shell=True).strip())
    except Exception as e:
        pass

    if legacy_router_ssid:  # legacy router connected is 5GHz
        logger.debug("hub connected to: {} on 5 GHz band".format(legacy_router_ssid))
        return legacy_router_ssid
    else:    # legacy router connected is 2.4GHz
        try:

            legacy_router_ssid = ast.literal_eval(
            subprocess.check_output("iwconfig ath01 | grep ESSID | awk -F: '{print $2}'", shell=True).strip())
        except Exception as e:
            pass
        if legacy_router_ssid:
            logger.debug("hub connected to: {} on 2.4 GHz band".format(legacy_router_ssid))
            return legacy_router_ssid
        else:
            legacy_router_ssid = ""
            logger.debug("hub is not connected to any legacy router")
            return legacy_router_ssid

def check_eblr():
    logger.debug("Executing check_eblr function")
    try:
        output1 = subprocess.check_output(["uci","show", "wireless.@wifi-iface[1].configuration"]).strip().split('=')[1]
        output2 = subprocess.check_output(["uci", "show", "wireless.@wifi-iface[0].configuration"]).strip().split('=')[1]
    except Exception,e:
        logger.debug(e)
        logger.critical("Error in checking EBLR status script",exc_info=True)
        return 1
    if output1 == "'EBLR'" and output2 == "'EBLR'":
        logger.debug("Shifu is in EBLR mode")
        return 0
    logger.debug("Shifu is in EBLR mode")
    return 1

# Feature:20112-Start
def check_configuration():
    logger.debug("Executing check_confuguration function")
    try:
        output = subprocess.check_output(["uci", "show", "wireless.@wifi-iface[0].configuration"]).strip().split('=')[1]
    except Exception, e:
        logger.debug(e)
        logger.critical("Error in checking EBLR status script", exc_info=True)
        return 1, "Error"
    logger.debug("Shifu is in "+ output +" mode")
    return 0, output.strip('\'')

def switch_WBLR_to_EBLR(mode):
    logger.debug("executing Shifu configuration switching from {} to EBLR".format(mode))
    logger.debug(str(threading.enumerate()))
    thread_list = []
    for t in threading.enumerate():
        thread_list.append(t.name)
    if "rc_thread" in thread_list:
        logger.critical("rc_thread is already running")
        return 1, '5134'
    else:
        if mode != "WBLR":
            logger.debug("router switching is getting initiated")
            rc_thread = threading.Thread(target=switch2, args=(), name="rc_thread")
            rc_thread.start()
            logger.debug("MA running threads list:" + str(threading.enumerate()))
        elif mode == "WBLR":
            logger.debug("router switching is getting initiated")
            rc_thread = threading.Thread(target=switch3, args=(), name="rc_thread")
            rc_thread.start()
            logger.debug("MA running threads list:" + str(threading.enumerate()))
    return 0, '6000'

def switch_EBLR_to_WBLR(ssid,password):
    logger.debug("executing Shifu configuration switching from EBLR to WBLR")
    # t1 = pass_check(password)
    # if not (t1[0]):
    #     logger.error("5108:Input parameters are not Valid")
    #     return 1, t1[1]
    # t2 = ssid_check(ssid)
    # if not (t2[0]):
    #     logger.error("5108:Input parameters are not Valid")
    #     return 1, t2[1]
    try:
        #os.system("uci set shifu.conf.MA_ROUTER_SSID='t.hub-" + ssid + "' && uci commit shifu")
        #os.system("uci set shifu.conf.MA_ROUTER_PASSWORD='" + password + "' && uci commit shifu")
        os.system("uci set shifu.conf.MA_LEGACY_ROUTER_SSID='" + ssid + "' && uci commit shifu")
        os.system("uci set shifu.conf.MA_LEGACY_ROUTER_PASSWORD='" + password + "' && uci commit shifu")
        os.system("uci set shifu.conf.WPA_AUTH_STATUS='NULL' && uci commit shifu")

    except Exception, e:
        logger.error(e)
        return 1, '5XXX'
    # rc_thread = threading.Thread(target=switch1, args=(ssid, password))
    # rc_thread.start()
    logger.debug(str(threading.enumerate()))
    thread_list = []
    for t in threading.enumerate():
        thread_list.append(t.name)
    if "rc_thread" in thread_list:
        logger.critical("rc_thread is already running")
        return 1, '5134'
    else:
        logger.debug("router switching is getting initiated")
        rc_thread = threading.Thread(target=switch1, args=(ssid, password), name="rc_thread")
        rc_thread.start()
        logger.debug("MA running threads list:" + str(threading.enumerate()))
    return 0, '6000'
def switch_WBLR_to_WBLR(ssid,password):
    logger.debug("executing Shifu configuration switching from WBLR to WBLR")
    # t1 = pass_check(password)
    # if not (t1[0]):
    #     logger.error("5108:Input parameters are not Valid")
    #     return 1, t1[1]
    # t2 = ssid_check(ssid)
    # if not (t2[0]):
    #     logger.error("5108:Input parameters are not Valid")
    #     return 1, t2[1]
    try:
        #os.system("uci set shifu.conf.MA_ROUTER_SSID='t.hub-" + ssid + "' && uci commit shifu")
        #os.system("uci set shifu.conf.MA_ROUTER_PASSWORD='" + password + "' && uci commit shifu")
        os.system("uci set shifu.conf.MA_LEGACY_ROUTER_SSID='" + ssid + "' && uci commit shifu")
        os.system("uci set shifu.conf.MA_LEGACY_ROUTER_PASSWORD='" + password + "' && uci commit shifu")
        os.system("uci set shifu.conf.WPA_AUTH_STATUS='NULL' && uci commit shifu")
    except Exception, e:
        logger.error(e)
        return 1, '5XXX'
    logger.debug(str(threading.enumerate()))
    thread_list = []
    for t in threading.enumerate():
        thread_list.append(t.name)
    if "rc_thread" in thread_list:
        logger.critical("rc_thread is already running")
        return 1, '5134'
    else:
        logger.debug("router switching is getting initiated")
        rc_thread = threading.Thread(target=switch4, args=(ssid, password), name="rc_thread")
        rc_thread.start()
        logger.debug("MA running threads list:" + str(threading.enumerate()))
    return 0, '6000'
def switch1(ssid, password):
    '''
    Both eblr to wblr or default to wblr
    :param ssid:
    :param password:
    :return:
    '''
    logger.debug("Running Router configuration script in thread.")
    led = LED.LedControl(7891)
    led.connect()
    led.send_msg("LED_NETWORK_RESET", 1)
    # Checking ssid interface number
    logger.debug("check whether SSID is in 5GHz or in 2.4GHz")
    output = os.popen("iwlist ath1 scan | grep ESSID").read().strip().split('\n')
    output = map(str.strip, output)
    output = [x[7:-1] for x in output]
    if ssid in output:
        logger.debug("SSID is found in 5GHz list")
        interface = "1"
    else:
        logger.debug("SSID is found in 2.4GHz list")
        interface = "0"
    # Creating the UCI script
    cmd_list = [
                "rm -f /etc/config/network",
                "/rom/etc/uci-defaults/network",
		        "uci delete wireless.@wifi-iface[2] 2>/dev/null",
                "uci delete wireless.@wifi-iface[2] 2>/dev/null",
                "uci delete wireless.qcawifi 2>/dev/null",
                "uci set wireless.wifi0.repacd_auto_create_vaps=0",
                "uci set wireless.wifi1.repacd_auto_create_vaps=0",
                "uci set wireless.qcawifi=qcawifi",
                "uci add wireless wifi-iface",
                "uci set wireless.@wifi-iface[0].configuration=WBLR",
                "uci set wireless.@wifi-iface[1].configuration=WBLR",
                "uci set wireless.@wifi-iface[2].network=wan",  # Create a interface for WAN port for backhaul creation
                "uci set wireless.@wifi-iface[2].device=wifi" + interface,  # wifi1 for 5Ghz backhaul only
                "uci set wireless.@wifi-iface[2].mode=sta",  # Defining mode of interface
                "uci set wireless.@wifi-iface[2].encryption=psk2+ccmp",  # set encryption for AP interface on backhaul interface
                "uci set wireless.@wifi-iface[2].ssid='" + ssid + "'",  # set SSID for AP interface on backhaul radio
                "uci set wireless.@wifi-iface[2].key='" + password + "'",  # set password for AP interface on backhaul
                "uci set wireless.@wifi-iface[2].extap=1",  # The extap logic introduces a smart MAC address masking
                "uci set wireless.@wifi-iface[2].wds=1", # Allows connecting OpenWrt based station devices to Shifu seamlessly
                "uci commit wireless",
                "uci set network.wan._orig_bridge=true",  # Enable bridge interface for WAN port
                "uci set network.wan.type=bridge",  # Enable bridge interface for WAN port
                "uci commit network",  # save network settings
                # "/etc/init.d/network restart",
                # "/etc/init.d/dropbear start"
                "wifi"
    ]
    # Executing the script
    for cmd in cmd_list:
        t = run_command(cmd)
        if t == 1:
            logger.error("5109: EBLR to WBLR switching is failed ")
            led.send_msg("LED_NETWORK_RESET", 0)
            led.send_msg("LED_ERROR", 1)
            return 1, '5109'
    #End
    os.system("uci set shifu.conf.MA_ROUTER_FLAG=0 && uci commit shifu")
    os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
    led.send_msg("LED_NETWORK_RESET", 0)
    #logger.debug("Checking the connectivity with the Legacy router in switching")
    # for count in range(10):
    #     time.sleep(10)
    #     if check_legacy_connection()=="success":
    #         break
    # led.send_msg("LED_NETWORK_RESET", 0)
    # # time.sleep(60)
    # os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
    # # ma.ma_profile_updates()
    # logger.debug("Sending signal after switching for profile updates")
    # os.system("kill -SIGUSR1 `ps | grep Mobile_Agent | grep -v grep | awk '{print $1}'`")

# def switch2(ssid, password):
def switch2():
    # from default to eblr

    logger.debug("Running Router configuration script in thread.")
    led = LED.LedControl(7891)
    led.connect()
    led.send_msg("LED_NETWORK_RESET", 1)
    cmd_list  = [
                "uci set wireless.@wifi-iface[0].configuration='EBLR'",
                "uci set wireless.@wifi-iface[1].configuration='EBLR'",
                "uci commit wireless"
    ]

    # Executing the script
    for cmd in cmd_list:
        t = run_command(cmd)
        if t == 1:
            logger.error("5109: DEFAULT to EBLR switching is failed ")
            led.send_msg("LED_NETWORK_RESET", 0)
            led.send_msg("LED_ERROR", 1)
            return 1, '5109'
    #End
    os.system("uci set shifu.conf.MA_ROUTER_FLAG=0 && uci commit shifu")
    time.sleep(5)
    led.send_msg("LED_NETWORK_RESET", 0)
    os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
    logger.debug("Sending signal after switching for profile updates")
    os.system("kill -SIGUSR1 `ps | grep Mobile_Agent | grep -v grep | awk '{print $1}'`")
# Feature:20112-End
def switch3():
# from wblr to eblr
    logger.debug("Running Router configuration script in thread.")
    led = LED.LedControl(7891)
    led.connect()
    led.send_msg("LED_NETWORK_RESET", 1)
    cmd_list  = [
                "rm -f /etc/config/network",
                "/rom/etc/uci-defaults/network",
                "uci delete wireless.@wifi-iface[2] 2>/dev/null",
                "uci delete wireless.@wifi-iface[2] 2>/dev/null",
                "uci delete wireless.qcawifi 2>/dev/null",
                "uci set wireless.@wifi-iface[0].configuration='EBLR'",
                "uci set wireless.@wifi-iface[1].configuration='EBLR'",
                "uci set wireless.wifi0.repacd_auto_create_vaps='1'",
                "uci set wireless.wifi1.repacd_auto_create_vaps='1'",
                "uci commit wireless",
                "/etc/init.d/repacd restart"
                ## "/etc/init.d/dropbear start"

    ]

    # Executing the script
    for cmd in cmd_list:
        t = run_command(cmd)
        if t == 1:
            logger.error("5109: WBLR to EBLR switching is failed ")
            led.send_msg("LED_NETWORK_RESET", 0)
            led.send_msg("LED_ERROR", 1)
            return 1, '5109'
    #End
    os.system("uci set shifu.conf.MA_ROUTER_FLAG=0 && uci commit shifu")
    os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
    led.send_msg("LED_NETWORK_RESET", 0)
    # os.system("uci set shifu.conf.MA_ROUTER_FLAG=0 && uci commit shifu")
    # time.sleep(30)
    # led.send_msg("LED_NETWORK_RESET", 0)
    # os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
    # # ma.ma_profile_updates()
    time.sleep(10)
    logger.debug("Sending signal after switching for profile updates")
    os.system("kill -SIGUSR1 `ps | grep Mobile_Agent | grep -v grep | awk '{print $1}'`")
# Feature:20112-End
def switch4(ssid, password):
    '''
    wblr to wblr
    :param ssid:
    :param password:
    :return:
    '''
    logger.debug("Running Router configuration script in thread.")
    led = LED.LedControl(7891)
    led.connect()
    led.send_msg("LED_NETWORK_RESET", 1)
    # Checking ssid interface number
    logger.debug("check whether SSID is in 5GHz or in 2.4GHz")
    output = os.popen("iwlist ath1 scan | grep ESSID").read().strip().split('\n')
    output = map(str.strip, output)
    output = [x[7:-1] for x in output]
    if ssid in output:
        logger.debug("SSID is found in 5GHz list")
        interface = "1"
    else:
        logger.debug("SSID is found in 2.4GHz list")
        interface = "0"
    # Creating the UCI script
    cmd_list = [
                "uci set wireless.@wifi-iface[2].encryption=psk2+ccmp",  # set encryption for AP interface on backhaul interface
                "uci set wireless.@wifi-iface[2].ssid='" + ssid + "'",  # set SSID for AP interface on backhaul radio
                "uci set wireless.@wifi-iface[2].key='" + password + "'",  # set password for AP interface on backhaul
                "uci set wireless.@wifi-iface[2].device=wifi" + interface,  # wifi1 for 5Ghz backhaul only
                "uci commit wireless",
                "wifi"
           ]
    # Executing the script
    for cmd in cmd_list:
        t = run_command(cmd)
        if t == 1:
            logger.error("5109: EBLR to WBLR switching is failed ")
            led.send_msg("LED_NETWORK_RESET", 0)
            led.send_msg("LED_ERROR", 1)
            return 1, '5109'
    #End
    os.system("uci set shifu.conf.MA_ROUTER_FLAG=0 && uci commit shifu")
    os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
    led.send_msg("LED_NETWORK_RESET", 0)
    # os.system("uci set shifu.conf.MA_ROUTER_FLAG=0 && uci commit shifu")
    # logger.debug("Checking the connectivity with the Legacy router in switching")
    # for count in range(10):
    #     time.sleep(10)
    #     if check_legacy_connection()=="success":
    #         break
    # led.send_msg("LED_NETWORK_RESET", 0)
    # # time.sleep(60)
    # os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
    # # ma.ma_profile_updates()
    # logger.debug("Sending signal after switching for profile updates")
    # os.system("kill -SIGUSR1 `ps | grep Mobile_Agent | grep -v grep | awk '{print $1}'`")
