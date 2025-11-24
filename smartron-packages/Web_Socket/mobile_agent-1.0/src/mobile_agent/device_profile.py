#!/usr/bin/python
"""Module which provides the system information"""
# __author__ = "Praveen kumar Donga"
# __porting__ = "Praveen kumar Donga"
# __copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
# __credits__ = ["Praveen kumar Donga"]
# __maintainer__ = "Praveen kumar Donga"
# __email__ = "praveen.donga@smartron.com"

import subprocess
# Feature15670:Start [shifu][HM][MA] Changing device profile format
import config
# Feature15670:End
import logging.handlers
# Feature16704:Start
import ast
import sqlite3
import Python_BT as bt
import json
import router_config as rc

# Feature16704:End

def get_build():
    x = subprocess.check_output(['lsb_release', '-b'])
    x = x.strip()
    build_no = x.split('IND')[1]
    return build_no
build_no = "B"+get_build()
######## End of adding build no. Need to remove above this
# logger = logging.getLogger(__name__)
logger = logging.getLogger(build_no + ':' + __name__)
logger.setLevel(logging.DEBUG)
handler = logging.handlers.SysLogHandler(address='/dev/log')
formatter = logging.Formatter('%(name)s %(funcName)s %(lineno)d:%(message)s')
handler.setFormatter(formatter)
logger.addHandler(handler)

'''
logger = logging.getLogger(__name__)  # appends the module name to logger
logger.setLevel(logging.DEBUG)  # setting the logger level to debug
handler = logging.handlers.SysLogHandler(address='/dev/log')  # printing the logs in syslog
formatter = logging.Formatter('%(module)s.%(funcName)s:%(message)s')  # logging format
handler.setFormatter(formatter)
logger.addHandler(handler)
'''

# DEVICE_NAME, Device firmware version, mac-address, DeviceId will be fetched using this function.
# MACID, OS-Version, DEVICE_ID, DEVICE_NAME are the NV variables
def get_NVvalue(parm):
    """It fetches the parameter value from system NV variable"""
    logger.debug("executing the get_NVvalue function")
    logger.debug("Fetching NV variable: %s", parm)
    try:
        value = subprocess.check_output(["uci", "get", "shifu.conf." + parm]).strip()
    except Exception, e:
        logger.critical('Unexpected Error or System Variable %s is not present', parm, exc_info=True)
        value = ""
    return value


# Fetching the Time Zone
def get_TimeZone():
    """Fetching the timezone from /etc/zonename"""
    # Bug19771:Start Fix for Wrong Timezone issue.
    logger.debug("executing the get_TimeZone function")
    cmd = "source /usr/sbin/fread-write-protect /etc/zonename"
    try:
        timezone = subprocess.check_output(cmd, shell=True).strip()
    except IOError:
        logger.critical("/etc/zonename file is not found", exc_info=True)
    # Bug19771:End
        timezone = ""
    except Exception, e:
        logger.critical('Unexpected Error while fetching Time Zone', exc_info=True)
        timezone = ""
    return timezone


# def get_Location():
#     """It Fetches the device location from ISP"""
#     # Hard coding this part for now.
#     latitude = "17.3753"
#     longitude = "79.4744"
#     return latitude, longitude


# Feature16704:Start
def get_Alarm():
    """It fetches alarm related settings"""
    logger.debug("get_Alarm function is being executed.")
    try:
        conn = sqlite3.connect('/root/va_data/ShifuAlarm.db')
    except Exception, e:
        logger.critical("Cannot find Shifu Alarm/Reminder database.", exc_info=True)
        return []
    cur = conn.cursor()
    # Query getting data from ALERT_TONES table
    get_alarm_settings = """SELECT VolumeLevel, SelectedTone, ListOfTones, GraduallyIncrease FROM ALERT_TONES """
    try:
        cur.execute(get_alarm_settings)
    except Exception, e:
        logger.critical("Cannot extract Shifu Alarm/Reminder details from database.", exc_info=True)
        return []
    alarm = cur.fetchall()[0]
    return alarm


def get_Reminder():
    """It fetches reminder related settings"""
    logger.debug("get_Reminder function is being executed.")
    try:
        conn = sqlite3.connect('/root/va_data/ShifuAlarm.db')
    except Exception, e:
        logger.critical("Cannot find Shifu Alarm/Reminder database.", exc_info=True)
        return []
    cur = conn.cursor()
    # Query getting data from ALERT_TONES table
    get_reminder_settings = """SELECT VolumeLevel, SelectedTone, ListOfTones, GraduallyIncrease FROM ALERT_TONES """
    try:
        cur.execute(get_reminder_settings)
    except Exception, e:
        logger.critical("Cannot extract Shifu Alarm/Reminder details from database.", exc_info=True)
        return []
    reminder = cur.fetchall()[1]
    return reminder


# Feature16704:End

def get_interfaces():
    interface =[]
    try:
        eth0_mac_id = subprocess.check_output("cat /sys/class/net/eth0/address", shell=True).strip()
        eth1_mac_id = subprocess.check_output("cat /sys/class/net/eth1/address", shell=True).strip()
        ath0_mac_id = subprocess.check_output("cat /sys/class/net/ath0/address", shell=True).strip()
        ath1_mac_id = subprocess.check_output("cat /sys/class/net/ath1/address", shell=True).strip()
        interface.append(ath0_mac_id)
        interface.append(ath1_mac_id)
        interface.append(eth0_mac_id)
        interface.append(eth1_mac_id)
    except Exception, e:
        logger.critical("Unable to read the MAC address of physical interfaces", exc_info = True)
    return interface


def get_profile(latitude, longitude, firstname):
    """It fetches the device information and returns to mobile_agent"""
    # system parameters
    # device related parameters
    # Feature15670:Start [shifu][HM][MA] Changing device profile format
    logger.debug("Fetching Device profile.")
    devices = {}
    # Feature16704:Start
    try:
        devices["device_name"] = firstname + "'s " + config.CONFIG.DeviceName.lower()
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable DEVICE_NAME is not present', exc_info=True)
        return 'error'
    # Feature16704:End
    devices["device_location"] = latitude, longitude
    # Support #18640: Start
    # Bug19771:Start Fix for Wrong Timezone issue.
    devices["time_zone"] = get_TimeZone()
    # devices["time_zone"] = "GMT+5:30"
    # Bug19771:End
    # Support  # 18640: End
    devices["os"] = "Openwrt"
    # Feature16704:Start
    try:
        devices["device_id"] = config.CONFIG.DEVICE_ID
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable DEVICE_ID is not present', exc_info=True)
        return 'error'

    # Feature 22289:Start
    try:
        logger.debug("adding has_internet to profile.json")
        if(config.CONFIG.INTERNET_MOBI_NOTIFICATION=='1'):
            devices["has_internet"] = '1'
        elif(config.CONFIG.INTERNET_MOBI_NOTIFICATION=='0'):
            devices["has_internet"] = '0'
    except Exception as e:
        logger.critical('Unexpected Error or uci flag INTERNET_MOBI_NOTIFICATION is not present', exc_info=True)
        return 'error'
    # Feature 22289:End

    # Feature16704:End
    about_device = {}
    with open('/etc/shifu_version.json') as f:
        data = json.load(f)
    f.close()
    about_device["device_software_version"] = str(data['version'])
    about_device["min_android_app_version"] = str(data['min_android_app_version'])
    about_device["min_ios_app_version"] = str(data['min_ios_app_version'])
    with open('/etc/dnld_firmware_info.json') as f:
        data1 = json.load(f)
    about_device["last_updated"] = str(data1['last_updated'])
    about_device["serial_number"] = config.CONFIG.DEVICE_ID
    about_device["mac_address"] = subprocess.check_output("cat /sys/class/net/eth0/address",
                                                          shell=True).strip()
    about_device["interfaces"] = get_interfaces()
    #about_device["firstName"] = firstname

    if config.CONSTANT.alexa_registration_status == 'registered':
        about_device["alexa_reg_status"] = True
    else:
        about_device["alexa_reg_status"] = False

    if config.CONSTANT.skip_alexa_registration_status == 'true':
        about_device["skip_alexa_reg_status"] = True
    else:
        about_device["skip_alexa_reg_status"] = False
    devices["about_device"] = about_device
    # Feature16704:Start
    # alarm related parameters
    # alarm_settings = {}
    # get_alarm = get_Alarm()
    # try:
    #     alarm_settings["volume"] = get_alarm[0]
    # except Exception, e:
    #     logger.critical("Cannot fetch Alarm details", exc_info=True)
    #     alarm_settings["volume"] = None
    # try:
    #     alarm_settings["selected_alarm_tones"] = str(get_alarm[1])
    # except Exception, e:
    #     logger.critical("Cannot fetch Alarm details", exc_info=True)
    #     alarm_settings["selected_alarm_tones"] = ""
    # try:
    #     alarm_settings["tones"] = ast.literal_eval(get_alarm[2])
    # except Exception, e:
    #     logger.critical("Cannot fetch Alarm details", exc_info=True)
    #     alarm_settings["tones"] = []
    # try:
    #     alarm_settings["is_alarm_gradually_increased"] = bool(get_alarm[3])
    # except Exception, e:
    #     logger.critical("Cannot fetch Alarm details", exc_info=True)
    #     alarm_settings["is_alarm_gradually_increased"] = None
    # # reminder related parameters
    # reminder_settings = {}
    # get_reminder = get_Reminder()
    # try:
    #     reminder_settings["volume"] = get_reminder[0]
    # except Exception, e:
    #     logger.critical("Cannot fetch Reminder details", exc_info=True)
    #     reminder_settings["volume"] = None
    # try:
    #     reminder_settings["selected_reminder_tones"] = str(get_reminder[1])
    # except Exception, e:
    #     logger.critical("Cannot fetch Reminder details", exc_info=True)
    #     reminder_settings["selected_reminder_tones"] = ""
    # try:
    #     reminder_settings["tones"] = ast.literal_eval(get_reminder[2])
    # except Exception, e:
    #     logger.critical("Cannot fetch Reminder details", exc_info=True)
    #     reminder_settings["tones"] = []
    # Feature16704:End
    # measurement units
    measurement_units = {}
    # Bug18139: Start
    measurement_units["temperature"] = "celsius"
    # Bug18139: End
    measurement_units["distance"] = "feet"
    measurement_units["weight"] = "grams"
    # wifi related parameters
    wifi_settings = {}
    legacy_router = {}
    router_settings = {}
    # Feature16704:Start
    # try:
    #     legacy_router["ssid"] = ast.literal_eval(
    #         subprocess.check_output("iwconfig ath11 | grep ESSID | awk -F: '{print $2}'", shell=True).strip())
    # except Exception, e:
    #     legacy_router["ssid"] = ""
    legacy_router["ssid"] = rc.get_legacy()
    try:
        router_settings["ssid"] = ast.literal_eval(
            subprocess.check_output("iwconfig ath0 | grep ESSID | awk -F: '{print $2}'", shell=True).strip())
    except Exception, e:
        router_settings["ssid"] = ""
    # Feature16704:End
    wifi_settings["legacy_router"] = legacy_router
    wifi_settings["router_settings"] = router_settings
    wifi_settings["mode"] = rc.check_configuration()[1]
    #  Bluetooth profile
    # Feature #17128: Start
    bt_profile = bt.bt_adapter_profile()
    if 'Error' in bt_profile:
        logger.debug("Error While Fetching the BT log: " + bt_profile)
        bt_profile = {
            "device_name": '',
            "mac_address": '',
            "device_type": '',
            "devices_limit": config.CONFIG.BT_DEVICE_LIMIT
        }
    #  Feature17128: End
    # device profile
    iot_settings= {}
    try:
        iot_settings["scene_devices_limit"] = config.CONFIG.DM_SCENE_DEVICES_LIMIT  # Default vaue is 32
        iot_settings["scene_music_limit"] = config.CONFIG.DM_SCENE_SONGS_LIMIT # Defaut value is 16,
        iot_settings["rule_devices_limit"] = config.CONFIG.DM_RULE_DEVICES_LIMIT # Defaut vaue is 32,
        iot_settings["rule_music_limit"] = config.CONFIG.DM_RULE_SONGS_LIMIT # Default value is 16
        iot_settings["max_iot_end_devices"] = config.CONFIG.DM_MAX_IOT_END_DEVICES
        iot_settings["max_areas_limit"] = config.CONFIG.DM_MAX_AREAS_LIMIT
        iot_settings["max_routine_limit"] = config.CONFIG.DM_MAX_ROUTINE_LIMIT
    except Exception as e:
        logger.debug(" error while  creating device profile {}".format(e))

    if config.CONFIG.MA_SmartdeviceNotification == 'true':
        iot_settings["SmartdeviceNotification"] = True # Default value is false
    if config.CONFIG.MA_SmartdeviceNotification == 'false':
        iot_settings["SmartdeviceNotification"] = False # Default value is false


    profile = {}
    profile["devices"] = devices
    #profile["alarm_settings"] = alarm_settings
    #profile["reminder_settings"] = reminder_settings
    profile["measurement_units"] = measurement_units
    profile["wifi_settings"] = wifi_settings
    profile["bluetooth_profile"] = bt_profile
    profile["iot_settings"] = iot_settings
    with open('/etc/profile.json', 'w') as outfile:
        json.dump(profile, outfile)
    outfile.close()
    # Feature15670:End
    return profile
