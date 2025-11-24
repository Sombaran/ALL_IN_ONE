#!/usr/bin/python
"""Mobile Agent module provides all the APIs required for mobile application"""
# __author__ = "Praveen kumar Donga"
# __porting__ = "Praveen kumar Donga"
# __copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
# __credits__ = ["Praveen kumar Donga"]
# __maintainer__ = "Praveen kumar Donga, Anudeep T"
# __email__ = "praveen.donga@smartron.com"

#from Crypto.Cipher import PKCS1_OAEP
#from Crypto.PublicKey import RSA
#from Crypto import Random
#import base64
import datetime


from functools import wraps
import subprocess
import time
import os
# Feature14820:Start For converting dictionary to json format
import json
# Feature14820:End
import logging.handlers
from flask import Flask, request, jsonify
import Python_BT as bt
import tronx_registration as tr
import router_config as rc
import shifu_things
import things_control as tc
import led_client as LED
import device_profile as dp
import zmq
import ast
import config
import shlex
# Feature16870:Start MA with Device reset capability
import reset_advertise as ra
import profile_advertise as pa
# Bug:19702-Start
import signal
import threading
# Bug:19702-End

# Feature16870:End
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
logger = logging.getLogger(__name__)  # appends the module name to logger
logger.setLevel(logging.DEBUG)  # setting the logger level to debug
handler = logging.handlers.SysLogHandler(address='/dev/log')  # printing the logs in syslog
formatter = logging.Formatter('%(module)s.%(funcName)s:%(message)s')  # logging format
handler.setFormatter(formatter)
logger.addHandler(handler)
'''
application = Flask(__name__)

curl_503 = [6, 7]
curl_504 = [28]
curl_525 = [51, 90, 91]
curl_500 = [-3, -4, -5, -6, -7, -8, 1, 2, 3, 4, 5, 8, 9, 10,
            11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
            21, 22, 23, 24, 25, 26, 27, 29, 30,
            31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
            52, 53, 54, 55, 56, 57, 58, 59, 60,
            61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
            71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
            81, 82, 83, 84, 85, 86, 87, 88, 89,
            92, 93]


# ***************************************#
# supporting functions:
# ***************************************#

def check_auth(token, api_type):
    """check_auth function is called to check if a username and password combination is valid.

    This function fetches the Account Token from the system variable and matches with the argument of this function and
    returns a boolean.
    """
    logger.debug("executing the check_auth function")
    # Fetching the DEVICE_REGISTRATION_STATUS value from NV variable
    try:
        # Redmine 19607: Start
        status_value = config.CONSTANT.device_registration_status
        # Redmine 19607: End
    except Exception, e:
        logger.critical('NV variable DEVICE_REGISTRATION_STATUS is not present', exc_info=True)
        return False
    # All router APIs will be ignored for authentication check if device is not registered or in configuration mode.
    if status_value != "registered":
        logger.debug("device status is %s", status_value)
        if api_type == 'R':  # R for Router APIs
            logger.debug("Router API is bypassed ")
            return True
        logger.info("APIs other than Router configuration are not allowed in not_registered mode")
        return False
    # Fetching the ACCOUNT_TOKEN value from NV variable
    try:
        # Redmine 19607: Start
        AcToken_value = config.CONSTANT.ACCOUNT_TOKEN
        # Redmine 19607: End
    except Exception, e:
        logger.critical('NV variable ACCOUNT_TOKEN is not present', exc_info=True)
        return False
    # Validating the ACCOUNT_TOKEN
    return token == AcToken_value


def requires_Auth(api_type):
    """requites_auth function.

    This is a wrapper that takes the function as an argument and modifies it.
    If we use the decorator before a function call, then that needs to be authenticated first.
    """

    def requires_auth(f):
        """It checks whether request is authorized or not."""

        @wraps(f)
        def decorated(*args, **kwargs):
            """Python Decorator"""
            # Fetching the ACCOUNT_TOKEN from the request headers
            auth = request.headers.get('X-AccountToken')
            # If there is no AccountToken in the headers returns Error
            if not auth:
                logger.debug("400: X-AccountToken is missing in the headers")
                return jsonify(
                    {'status': 'failure', 'message': "Please include X-AccountToken in request headers"}), 400
            # If request is not authenticated, it returns Error
            if not check_auth(auth, api_type):
                logger.debug("401: Unauthorized request")
                return jsonify({'status': 'failure', 'message': "Unauthorized request"}), 401
            return f(*args, **kwargs)

        return decorated

    return requires_auth


# Feature 16562: Start
## Voice Assistant Socket
def va_socket(va_msg, port=8800, timeout = 5):
    """
    - Passing router connection state to voice assistant
    """
    logger.debug("va_socket function is called")
    response = 1
    try:
        context = zmq.Context()
        socket = context.socket(zmq.REQ)
        socket.setsockopt(zmq.LINGER, 0)
        connection = "tcp://localhost:"+str(port) #port 4640 is used for LWA purpose
        socket.connect(connection)
        socket.send(va_msg)
        poller = zmq.Poller()
        poller.register(socket, zmq.POLLIN)
        if poller.poll(timeout * 1000):  # 5s timeout in milliseconds
            logger.debug("waiting in socket receive ************")
            socket_status = socket.recv()
            json_data = ast.literal_eval(socket_status)
            if port == 4640:    #checking status code in LWA login return value from va socket
                if json_data["status_code"] == 200:
                    response = 0
            logger.info('Received LWA login status from va_socket is: {}'.format(socket_status))
        else:
            logger.error("MA Error: ZMQ reception failed")
            socket.close()
            return response
        socket.close()
        return response
    except Exception as e:
        logger.error('Exception in va_socket is: {}'.format(e))
        return response



# Feature 16562: End

############################
# Bluetooth APIs:
############################
# Bluetooth API-01:
@application.route('/api/bluetooth/discovery/start', methods=['GET'])
@requires_Auth('B')  # tagging this API as Bluetooth API(B)
def start_discovery():
    """A function to discover all the bluetooth devices nearby"""

    logger.debug('Bluetooth start_discovery API is called')
    # calling start discovery api from BT stack
    discovery = bt.bt_start_discovery()
    # Redmine 20752: Start
    if discovery[0] == -1:
        logger.debug('Bluetooth Discovery is failed.')
        return jsonify({'status': 'failure', 'message': discovery[1]}), 500
    elif discovery[0] == -2:
        logger.debug('Bluetooth Discovery Internal Error')
        return jsonify({'status': discovery[1], 'message': 'Bluetooth internal error'}), 500
    # Redmine 20752: End
    logger.debug('Bluetooth discovery is started.')
    return jsonify({'status': 'success', 'message': 'Bluetooth discovery initiated Successfully'}), 200


# Bluetooth API-02:
@application.route('/api/bluetooth/discovery/stop', methods=['GET'])
@requires_Auth('B')  # tagging this API as Bluetooth API(B)
def stop_discovery():
    """It stops the start discovery call"""
    logger.debug('Bluetooth stop discovery API is called')
    # calling the stop discovery API from BT stack
    discovery = bt.bt_stop_discovery()
    if discovery != 'Success':
        logger.debug('Bluetooth stop_discovery is failed')
        return jsonify({'status': discovery, 'message': 'BT stop discovery is failed'}), 500
    logger.debug('Bluetooth stop_discovery is done')
    return jsonify({'status': discovery, 'message': 'BT stop discovery is successful'}), 200


# Bluetooth API-03:
@application.route('/api/bluetooth/device/connect', methods=['POST'])
@requires_Auth('B')  # tagging this API as Bluetooth API(B)
def device_connect():
    """It connect a device to Shifu's BT"""
    logger.debug('Bluetooth request pairing API is called')
    # Fetching the mac-id from request headers
    mac_id = str(request.json['mac-id'])
    logger.debug('connecting ..... %s', mac_id)
    # calling the bt_process from BT stack to connect the device to Shifu BT
    response = bt.bt_process(mac_id, 'connect')
    if response == 'Success':
        http_code = 200
        message = 'device is connected'
        logger.debug(message)
    else:
        http_code = 500
        message = 'device is not connected'
        logger.debug(message)
    return jsonify({'status': response, 'message': message}), http_code


# Bluetooth API-04:
@application.route('/api/bluetooth/device/disconnect', methods=['POST'])
@requires_Auth('B')  # tagging this API as Bluetooth API(B)
def disconnect_device():
    """It disconnects the connected device from Shifu's BT"""
    logger.debug('Bluetooth disconnect device API is called')
    # Fetching the mac-id from the request headers
    mac_id = str(request.json['mac-id'])
    logger.debug('disconnecting..... %s', mac_id)
    # calling the bt_process function from BT stack to disconnect the device from Shifu BT
    response = bt.bt_process(mac_id, 'disconnect')
    if response == 'Success':
        http_code = 200
        message = 'device is disconnected'
        logger.debug(message)
    else:
        http_code = 500
        message = 'device is not disconnected'
        logger.debug(message)
    return jsonify({'status': response, 'message': message}), http_code


# Bluetooth API-05:
@application.route('/api/bluetooth/device/forget', methods=['POST'])
@requires_Auth('B')  # tagging this API as Bluetooth API(B)
def forget_device():
    """It removes the device from paired list"""
    logger.debug('Bluetooth forget_device API is called')
    # Fetching the mac-id from the request headers
    mac_id = str(request.json['mac-id'])
    logger.debug('forgetting ... %s', mac_id)
    # calling the bt_process function from BT stack to unpair the device from Shifu BT
    response = bt.bt_process(mac_id, 'unpair')
    if response == 'Success':
        http_code = 200
        message = 'forget device is done'
        logger.debug(message)
    else:
        http_code = 500
        message = 'forget device is not done'
        logger.debug(message)
    return jsonify({'status': response, 'message': message}), http_code


# Bluetooth API-06:
@application.route('/api/bluetooth/devices/paired', methods=['GET'])
@requires_Auth('B')  # tagging this API as Bluetooth API(B)
def paired_devices():
    """It gives all the previously paired devices in a list"""
    logger.debug('Bluetooth paired_devices API is called')
    # calling the bt_list_paired_dev function from BT stack to list paired devices of Shifu BT
    response = bt.bt_list_paired_dev()
    # checking whether the response is dictionary or not
    if isinstance(response, dict):
        logger.debug('successfully got the paired devices list')
        return jsonify({'status': 'success', 'message': response}), 200
    logger.debug(response)
    return jsonify({'status': 'failure', 'message': response}), 500


# Bluetooth API-07:
@application.route('/api/bluetooth/devices/connected', methods=['GET'])
@requires_Auth('B')  # tagging this API as Bluetooth API(B)
def connected_devices():
    """It gives all the connected devices list"""
    logger.debug('Bluetooth connected_devices API is called')
    # calling the bt_list_connected_dev function from BT stack to list connected devices of Shifu BT
    response = bt.bt_list_connected_dev()
    # checking whether the response is dictionary or not
    if isinstance(response, dict):
        logger.debug('successfully got the connected devices list')
        return jsonify({'status': 'success', 'message': response}), 200
    logger.debug(response)
    return jsonify({'status': 'failure', 'message': response}), 500


# Bluetooth API-08:
@application.route('/api/bluetooth/discoverable', methods=['POST'])
@requires_Auth('B')  # tagging this API as Bluetooth API(B)
def discoverable():
    """It sets the discoverable time of Shifu's BT"""
    logger.debug('Bluetooth discoverable API is called')
    # Fetching the timeout from the request headers
    timeout = str(request.json['timeout'])
    logger.debug('Bluetooth discoverable timeout is %s', timeout)
    # if timeout is not sent, we are calling the bt_discoverable function with default timeout value
    if not timeout:
        response = bt.bt_discoverable()
    else:
        timeout = int(timeout)
        response = bt.bt_discoverable(timeout)
    if response == 'Success':
        logger.debug('Bluetooth discoverable time is set')
        return jsonify({'status': 'success', 'message': 'Bluetooth discoverable time is set'}), 200
    logger.debug(response)
    return jsonify({'status': 'failure', 'message': response}), 500


# Feature16456:Start
# Bluetooth API-09:
@application.route('/api/bluetooth/devices/list', methods=['GET'])
@requires_Auth('B')  # tagging this API as Bluetooth API(B)
def device_list():
    """It gives all the paired and connected devices in a single list"""
    logger.debug('Bluetooth device_list API is called')
    # Feature19936:Start External BT device type identification
    # bt_list_devices funciton gives the list of paired and connected devices list
    device_list = bt.bt_list_devices()

    if not isinstance(device_list, list):
        logger.debug(device_list)
        return jsonify({'status': 'failure', 'message': device_list}), 500

    logger.debug('Bluetooth device list is fetched.')
    return jsonify({'status': "success", 'list': device_list}), 200
    # Feature19936:End
# Feature16456:End


# Feature15705:Start Implementation of ON and OFF feature to Bluetooth
# Bluetooth API-10:
@application.route('/api/bluetooth/switch', methods=['POST'])
@requires_Auth('B')  # tagging this API as Bluetooth API(B)
def bt_switch():
    """It turns ON/OFF BT"""
    logger.debug('Bluetooth turn ON/OFF API called')
    # Fetching the value to which BT status needs to be changed to from request headers
    switch = request.json['switch']
    if not isinstance(switch, bool):
        logger.debug(switch)
        return jsonify({'status': 'failure', 'message': 'Invalid BT switch value'}), 400
    # Changing BT status
    response = bt.bt_switch(switch)
    # Returning failure message if the response is not Success
    if response != 'Success':
        logger.debug(response)
        return jsonify({'status': 'failure', 'message': response}), 500
    logger.debug('Succesfully changed status of BT')
    return jsonify({'status': 'success', 'message': 'Bluetooth Turned On/Off'}), 200


# Bluetooth API-11:
@application.route('/api/bluetooth/switch/status', methods=['GET'])
@requires_Auth('B')  # tagging this API as Bluetooth API(B)
def bt_switch_status():
    """It returns BT status"""
    logger.debug('Bluetooth status API called')
    # Fetching BT status
    response = bt.bt_switch_status()
    # returning failure message if the response is not a boolean
    if isinstance(response, str):
        logger.debug(response)
        return jsonify({'status': 'failure', 'message': response}), 500
    logger.debug('Succesfully changed status of BT')
    return jsonify({'status': 'success', 'bt_status': bool(response)}), 200
# Feature15705:End


# Feature17344: Start
# Bluetooth API-11:
@application.route('/api/bluetooth/change/name', methods=['POST'])
@requires_Auth('B')  # tagging this API as Bluetooth API(B)
def bt_change_name():
    """It changes the BT name"""
    logger.debug('Bluetooth name change API is called')
    # Fetching the Bt name from request headers
    bt_name = request.json['name']
    if bt_name == "":
        logger.debug("bt_name field is ")
        return jsonify({'status': 'failure', 'message': 'bluetooth new name can not be empty'}), 400
    logger.debug("BT name will be changed to " + str(bt_name))
    #  Changing the BT name
    logger.debug("Calling BT Stack's API to change the name")
    response = bt.bt_adapter_name(bt_name)
    # returning failure message if the response is not a boolean
    if response != 'Success':
        logger.debug(response)
        return jsonify({'status': 'failure', 'message': response}), 500
    try:
        with open("/etc/profile.json", "r+") as f:
            data = json.load(f)
            data["bluetooth_profile"]["device_name"] = bt_name
            # bt_profile= {"bluetooth_profile":data["bluetooth_profile"]}
            f.seek(0)  # rewind
            json.dump(data, f)
            f.truncate()
            f.close()
    except IOError:
        logger.error("profile.json file is not found")
    logger.debug('BT name is changed successfully')
    # os.system('/etc/init.d/ma_bootup start')
    # os.system("kill -SIGUSR1 `ps | grep Mobile_Agent | grep -v grep | awk '{print $1}'`")
    os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
    ma_profile_updates()
    return jsonify({'status': 'success', 'message': 'Bluetooth name is changed successfully'}), 200
# Feature 17344: End


# ***************************************#
# On-boarding APIs
# ***************************************#
# OnBoarding API-01:
@application.route('/api/auth/login', methods=['POST'])
def login():
    """The API takes care of the logging process.

    If device is not_registered or in configuration state login will be allowed and on successful login HOME_TOKEN and
    ACCOUNT_TOKEN will be stored into system variable.
    """
    logger.debug("/auth/login API is called")
    latitude = str(request.json['latitude'])
    longitude = str(request.json['longitude'])
    mode = str(request.json['mode'])
    #mode = "EBLR"

    try:
        firstname = str(request.json['firstName'])
    except Exception as e:
        firstname = "blank"
    if latitude == '' or longitude == '' or firstname == '':
        logger.debug("Device location is null")
        return jsonify({'status': 'failure', 'message': "Current device location is required to Login"}), 400
    try:
        status_value = config.CONSTANT.device_registration_status
        logger.debug("Device registration status is %s", status_value)
    except Exception, e:
        logger.critical('Unexpected Error or not able to access the DEVICE_REGISTRATION_STATUS system variable',
                        exc_info=True)
        return jsonify({'status': 'failure', 'message': "Shifu Interneal Error"}), 500

    # If request mode is "EBLR" then check the current mode.
    # If current mode is WBLR and device is not registered then return error message to obile app and start network reset
    if mode == "EBLR" and status_value != "registered":
        output = rc.check_configuration()
        current_mode = output[1]
        if current_mode == "WBLR":
            logger.critical("t.hub is already in WBLR mode, Before setting to EBLR, we need to do NW reset")
            nw_reset = rc.network_reset()
            # Feature:20136-End
            if nw_reset[1] == '5134':
                logger.debug("another router configuration is in progress")
                return jsonify({
                    "status": 'error',
                    "message": 'another t.hub network configuration is in progress'
                }), 400
            logger.debug("Initiated Network reset and replying back to Mobile App")
            return jsonify({'status': 'failure',
                            'message': "t.hub network setting are in bad state. t.hub is resetting Network."}), 601



        ###############set mode
    os.system("uci set wireless.@wifi-iface[0].configuration={} && uci commit wireless".format(mode))
    os.system("uci set wireless.@wifi-iface[1].configuration={} && uci commit wireless".format(mode))

    ################################
    # Fetching the value stored in system variable DEVICE_REGISTRATION_STATUS

    # checking whether device is in configuration state or in not_registered state
    if status_value != "registered":
        if not request.headers.get('X-HomeTempToken'):
            logger.debug("412:Require X-HomeTempToken is required")
            return jsonify({'status': 'failure', 'message': "Require X-HomeTempToken in headers"}), 400
        # extracting the HomeTempToken from the request API header
        token = str(request.headers.get('X-HomeTempToken'))
        logger.debug("calling the registration API")
        # sending message to led socket to play the setup mode pattern
        response, response_code, response_text = tr.device_registration(token, latitude, longitude , firstname)
        # if registration is unsuccessful playing the Error pattern and stopping the set_up mode pattern
        if response_code != 200:
            logger.debug("received not 200 code from TronX")
            led.send_msg("LED_ERROR", 1)
            return jsonify({'status': response, 'message': response_text}), response_code
        # getting the system information and sending it as a part of response
        # profile = dp.get_profile(latitude, longitude)
        with open("/etc/profile.json", "r") as f:
            profile = json.load(f)
            f.close()

        logger.debug("Turning off the LED pattern for setup")
        #led.send_msg("LED_SETUP_MODE", 0)
        logger.debug("/auth/login API is responding back in success case")
        # Redmine 19607: Start
        reload(config)
        time.sleep(2)
        # Redmine 19607: End
        # Feature15866:Start Changing device profile format
        return jsonify(
            {'status': response, 'message': 'Logged in successfully', 'device_profile_data': profile}), response_code
    # Feature15866:End
    #
    else:
        if request.headers.get('X-AccountToken') == config.CONSTANT.ACCOUNT_TOKEN:
            with open("/etc/profile.json", "r") as f:
                profile = json.load(f)
                f.close()
            return jsonify({'status':'success' , 'message': 'Logged in successfully', 'device_profile_data': profile}), 200
        else:
            logger.debug("/auth/login API is responding back in failure case")
            return jsonify({'status': 'failure', 'message': "t.hub already registered with some other user. Please contact customer care for more information"}), 400

#_______________________________________________________________________________________________________________________________________________________--
@application.route('/api/auth/lwa_login', methods=['POST'])
@requires_Auth('O')
def lwa_login():
    """The API takes care of the lwa login process.
    """
    notif_id = 12345
    logger.debug("/auth/lwa_login API is called")

    # Reading ALEXA_REGISTRATION_STATUS env
    try:
        alexa_status_value = config.CONSTANT.alexa_registration_status
        logger.debug("Alexa registration status is %s", alexa_status_value)
    except Exception, e:
        logger.critical('Unexpected Error or not able to access the ALEXA_REGISTRATION_STATUS system variable',exc_info=True)
        return jsonify({'status': 1, 'message': "Shifu Interneal Error"}), 500

    # if registered sending fail to app
    if alexa_status_value == 'registered':
        logger.debug(" Shifu is already registered with Alexa")
        logger.debug("/auth/lwa_login API is responding back")
        return jsonify({'status': 1, 'message': "Failure"}), 400
    else:
        grant_type = str(request.json['grant_type'])
        client_id = str(request.json['client_id'])
        redirect_uri = str(request.json['redirect_uri'])
        code = str(request.json['code'])
        code_verifier = str(request.json['code_verifier'])
        if client_id == '' or  redirect_uri == '' or code == '' or code_verifier== '' or grant_type== '' :
            logger.debug("Parameters are missing")
            return jsonify({'status': 'failure', 'message': "Parameters are required to Login"}), 400
        va_msg = {"client_name" : "MA",
                    "client_action" : "REGISTRATION",
                    "notif_id" : notif_id,
                    "client_data" :
                              {"amz_client_id" :client_id ,
                               "amz_auth_code" : code,
                               "amz_redirect_uri" :redirect_uri ,
                               "amz_code_verifier" : code_verifier }
                    }
        logger.debug('va_msg is {}'.format(va_msg))
        response = va_socket(str(va_msg), 4640, 50) #port=4640 and timeout = 50s for lwa
        logger.debug('Response from va_socket is: {}'.format(response))
        #response = 0
        if response == 1 :
            logger.debug("/auth/lwa_login API is responding back")
            led.send_msg("LED_ERROR", 1)
            return jsonify({'status': response, 'message': "Failure"}), 400
        else: # va response success
            # updating env
            try:
                os.system("uci set shifu.conf.ALEXA_REGISTRATION_STATUS=registered && uci commit shifu")
                os.system("uci set shifu.conf.SKIP_ALEXA_REGISTRATION=false && uci commit shifu")
            except Exception, e:
                logger.critical('Not able to set the NV variable: alexa_registration_stauts', exc_info=True)
                return jsonify({'status': 1,'message': "Unable to store Alexa registration env. Please try after some time"}), 400

            try:
                with open("/etc/profile.json", "r+") as f:
                    data = json.load(f)
                    data["devices"]["about_device"]["alexa_reg_status"] = True
                    data["devices"]["about_device"]["skip_alexa_reg_status"] = False
                    # bt_profile= {"bluetooth_profile":data["bluetooth_profile"]}
                    f.seek(0)  # rewind
                    json.dump(data, f)
                    f.truncate()
                    f.close()
            except IOError:
                logger.error("profile.json file is not found for updating alexa reg status")
                return jsonify({'status': 1, 'message': "Unable to store Alexa registration status locally in hub"}), 400
            logger.debug('Alexa reg status is changed successfully')
            reload(config)

            # os.system('/etc/init.d/ma_bootup start')
            # os.system("kill -SIGUSR1 `ps | grep Mobile_Agent | grep -v grep | awk '{print $1}'`")
            os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
            ma_profile_updates()
            logger.debug("/auth/lwa_login API is responding back")
            led.send_msg("LED_SETUP_MODE", 0)
            return jsonify({'status': response, 'message': "Success"}), 200

@application.route('/api/auth/skip_lwa_login', methods=['GET'])
@requires_Auth('O')
def skip_lwa_login():
    """The API takes care of the skip lwa login process.
    """
    notif_id = 12345
    logger.debug("/auth/skip_lwa_login API is called")

    # Reading SKIP_ALEXA_REGISTRATION env
    try:
        skip_alexa_status_value = config.CONSTANT.skip_alexa_registration_status
        logger.debug("Skip Alexa registration status is %s", skip_alexa_status_value)
    except Exception, e:
        logger.critical('Unexpected Error or not able to access the SKIP_ALEXA_REGISTRATION system variable',exc_info=True)
        return jsonify({'status': 1, 'message': "Shifu Interneal Error"}), 500

    # if registered sending fail to app
    if skip_alexa_status_value == 'true':
        logger.debug("Skip alexa registration is already true")
        logger.debug("/auth/skip_lwa_login API is responding back")
        return jsonify({'status': 1, 'message': "lwa login is already skipped"}), 400
    else:
        # updating env
        try:
            os.system("uci set shifu.conf.SKIP_ALEXA_REGISTRATION=true && uci commit shifu")
        except Exception, e:
            logger.critical('Not able to set the NV variable: skip alexa_registration', exc_info=True)
            return jsonify({'status': 1,'message': "Unable to store skip Alexa registration env. Please try after some time"}), 400
        try:
            with open("/etc/profile.json", "r+") as f:
                data = json.load(f)
                data["devices"]["about_device"]["skip_alexa_reg_status"] = True
                # bt_profile= {"bluetooth_profile":data["bluetooth_profile"]}
                f.seek(0)  # rewind
                json.dump(data, f)
                f.truncate()
                f.close()
        except IOError:
            logger.error("profile.json file is not found for updating skip alexa reg status")
            return jsonify({'status': 1, 'message': "Unable to store skip Alexa registration status locally in hub"}), 400
        logger.debug('Skip Alexa reg status is changed successfully')
        reload(config)
        # os.system('/etc/init.d/ma_bootup start')
        # os.system("kill -SIGUSR1 `ps | grep Mobile_Agent | grep -v grep | awk '{print $1}'`")
        os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
        ma_profile_updates()
        logger.debug("/auth/skip_lwa_login API is responding back")
        led.send_msg("LED_SETUP_MODE", 0)
        logger.debug('stopping led after skip alexa')
        return jsonify({'status': 0, 'message': "Success"}), 200
# ***************************************#
# Router APIs:
# ***************************************#
# Router API-01:
@application.route('/api/router/change/credentials', methods=['POST'])
@requires_Auth('R')  # tagging this API as router configuration API(R)
def change_credentials():
    """An API call to make changes to AP credentials."""
    logger.debug("change_credentials API is called")
    # extracting the arguments from API header
    SSID = str(request.json['ssid'])
    # ENCRYPTION = str(request.json['encryption'])
    # if not ENCRYPTION:
    #     ENCRYPTION = "psk2"
    ENCRYPTION = "psk2+ccmp"
    PASSWORD = str(request.json['password'])
    # calling the function from router configuration module
    result = rc.change_apcredentials(SSID, ENCRYPTION, PASSWORD)
    # result[1]=6000 on success /5XXX on failure
    if result[1] == '6000':
        logger.debug("AP credentials are changed")
        return jsonify({
            "status": 'success',
            "message": "AP credentials are changed"
        }), 200
    elif result[1] == '5102':
        logger.debug("Password length should not be less than 8 characters and not more than 64 characters")
        return jsonify({
            "status": 'failure',
            "message": "Password length should not be less than 8 characters and not more than 64 characters"
        }), 400
    elif result[1] == '5105':
        logger.debug("AP credentials are not changed. Encryption is not supported")
        return jsonify({
            "status": 'failure',
            "message": "Encryption is not supported"
        }), 400
    elif result[1] == '5134':
        return jsonify({
            "status": 'failure',
            "message": "Another router configuration is in process. Please try after sometime"
        }), 400
    logger.debug("AP credentials are not changed. Shifu Internal Error")
    return jsonify({
        "status": 'failure',
        "message": 'AP credentials are not changed. Shifu Internal Error'
    }), 500


# Router API-01:
# Feature:20136-Start
# @application.route('/api/router/change/credentials/proceed', methods=['GET'])
@application.route('/api/router/EBLR/proceed', methods=['GET'])
# Feature:20136-End
@requires_Auth('R')  # tagging this API as router configuration API(R)
def change_credentials_proceed():
    """An API call to make changes to AP credentials."""
    logger.debug("change_credentials_proceed API is called")
    # calling the function from router configuration module
    result = rc.shifu_connect_EBLR()
    # result[1]=6000 on success /5XXX on failure
    if result[1] == '6000':
        logger.debug("AP credentials are changed")
        return jsonify({
            "status": 'success',
            "message": "AP credentials are changed"
        }), 200
    elif result[1] == '5134':
        logger.debug("another router configuration is in progress")
        return jsonify({
            "status": 'failure',
            "message": "another router configuration is in progress. please try after some time"
        }), 400
    elif result[1] == '52XX':
        logger.debug("AP credentials are not stored")
        return jsonify({
            "status": 'failure',
            "message": "AP credentials are not stored"
        }), 400
    logger.debug("AP credentials are not changed. Shifu Internal Error")
    return jsonify({
        "status": 'failure',
        "message": 'AP credentials are not changed. Shifu Internal Error'
    }), 500


# Router API-01:
# Feature:20136-Start
# @application.route('/api/router/change/credentials/set', methods=['POST'])
@application.route('/api/router/EBLR/set', methods=['POST'])
# Feature:20136-End
@requires_Auth('R')  # tagging this API as router configuration API(R)
def change_credentials_set():
    """An API call to make changes to AP credentials."""
    logger.debug("change_credentials_set API is called")
    # extracting the arguments from API header
    SSID = str(request.json['ssid'])
    ENCRYPTION = str(request.json['encryption'])
    if not ENCRYPTION:
        ENCRYPTION = "psk-mixed"
    PASSWORD = str(request.json['password'])
    # calling the function from router configuration module
    result = rc.store_credentials(SSID, PASSWORD, ENCRYPTION)
    # result[1]=6000 on success /5XXX on failure
    if result[1] == '6000':
        logger.debug("AP credentials are stored")
        return jsonify({
            "status": 'success',
            "message": "AP credentials are stored"
        }), 200
    elif result[1] == '5102':
        logger.debug("Password length should not be less than 8 characters and not more than 64 characters")
        return jsonify({
            "status": 'failure',
            "message": "Password length should not be less than 8 characters and not more than 64 characters"
        }), 400
    elif result[1] == '5105':
        logger.debug("AP credentials are not stored. Encryption is not supported")
        return jsonify({
            "status": 'failure',
            "message": "Encryption is not supported"
        }), 400
    elif result[1] == '5107':
        logger.debug("AP credentials are not stored. SSID is not a valid string")
        return jsonify({
            "status": 'failure',
            "message": "SSID is not a valid string"
        }), 400
    elif result[1] == '5117':
        logger.debug("AP credentials are not stored. SSID should not be empty")
        return jsonify({
            "status": 'failure',
            "message": "SSID should not be empty"
        }), 400
    elif result[1] == '5133':
        logger.debug("AP credentials are not stored. ssid with all spaces is not allowed")
        return jsonify({
            "status": 'failure',
            "message": "SSID with all spaces is not allowed"
        }), 400
    logger.debug("AP credentials are not stored. Shifu Internal Error")
    return jsonify({
        "status": 'failure',
        "message": 'AP credentials are not stored. Shifu Internal Error'
    }), 500
@application.route('/api/router/WBLR/proceed', methods=['POST'])
# Feature:20136-End
@requires_Auth('R')  # tagging this API as router configuration API(R)
def shifu_connect_WBLR():
    """An API which provides user access to connect Shifu to the external legacy router with wifi backhaul"""
    logger.debug("shifu_connect WBLR API is called")
    if rc.eth0_check():
        return jsonify({
            "status": 'failure',
            "message": "Remove Ethernet cable from t.hub WAN port"
        }), 400
    # extracting the arguments from API header
    ssid = str(request.json['ssid'])
    password = str(request.json['password'])
    result = rc.shifu_connect_WBLR(ssid, password)

    if result[1] == '6000':
        logger.debug("WBLR configuration is initialized")
        return jsonify({
            "status": 'success',
            "message": "WBLR configuration is initialized",
            "hub_ssid": "t.hub-{}".format(str(ssid))
        }), 200
    elif result[1] == '5134':
        logger.debug("Router configuration is already in Progress")
        return jsonify({
            "status": 'failure',
            "message": "Router configuration is already in Progress"
        }), 400
    elif result[1] == '5102':
        logger.debug("Password length should not be less than 8 characters and not more than 64 characters")
        return jsonify({
                "status": 'failure',
                "message": "Password length should not be less than 8 characters and not more than 64 characters"
            }), 400
    elif result[1] == '5107':
        logger.debug("ssid is not in string format. Internal Error")
        return jsonify({
                "status": 'failure',
                "message": "ssid is not in string format"
            }), 400
    logger.debug("Shifu Internal Error")
    return jsonify({
        "status": 'failure',
        "message": 'Internal Server Error'
    }), 500

# Router API-X2
# Feature:20136-Start
# @application.route('/api/router/shifu/WBLR/connect', methods=['POST'])
@application.route('/api/router/WBLR/Front/proceed', methods=['POST'])
# Feature:20136-End
@requires_Auth('R')  # tagging this API as router configuration API(R)
def shifu_connect_WBLR_fronthaul():
    """An API which provides user access to set hub ssid and password"""
    logger.debug("shifu_connect WBLR fronthaul API is called")
    if rc.eth0_check():
        return jsonify({
            "status": 'failure',
            "message": "Remove Ethernet cable from t.hub WAN port"
        }), 400

    hub_ssid = str(request.json['hub_ssid'])
    hub_password = str(request.json['hub_password'])
    result = rc.shifu_connect_WBLR_fronthaul(hub_ssid,hub_password)
    if result[1] == '6000':
        logger.debug("WBLR fronthaul configuration is initialized")
        return jsonify({
            "status": 'success',
            "message": "WBLR front configuration is initialized",
            #"hub_ssid": "t.hub-{}".format(str(ssid))
        }), 200
    elif result[1] == '5134':
        logger.debug("Router configuration is already in Progress")
        return jsonify({
            "status": 'failure',
            "message": "Router configuration is already in Progress"
        }), 400
    elif result[1] == '5102':
        logger.debug("Password length should not be less than 8 characters and not more than 64 characters")
        return jsonify({
                "status": 'failure',
                "message": "Password length should not be less than 8 characters and not more than 64 characters"
            }), 400
    elif result[1] == '5107':
        logger.debug("ssid is not in string format. Internal Error")
        return jsonify({
                "status": 'failure',
                "message": "ssid is not in string format"
            }), 400
    logger.debug("Shifu Internal Error")
    return jsonify({
        "status": 'failure',
        "message": 'Internal Server Error'
    }), 500


@application.route('/api/router/WBLR/Back/proceed', methods=['POST'])
# Feature:20136-End
@requires_Auth('R')  # tagging this API as router configuration API(R)
def shifu_connect_WBLR_backhaul():
    """An API which provides user access to connect Shifu to the external legacy router with wifi backhaul"""
    logger.debug("shifu_connect WBLR backhaul API is called")
    if rc.eth0_check():
        return jsonify({
            "status": 'failure',
            "message": "Remove Ethernet cable from t.hub WAN port"
        }), 400
    legacy_ssid = str(request.json['legacy_ssid'])
    legacy_password = str(request.json['legacy_password'])
    global temp_ssid
    temp_ssid=legacy_ssid
    result = rc.shifu_connect_WBLR_backhaul(legacy_ssid,legacy_password)

    if result[1] == '6000':
        logger.debug("WBLR backhaul configuration is initialized")
        return jsonify({
            "status": 'success',
            "message": "WBLR configuration is initialized",
            #"hub_ssid": "t.hub-{}".format(str(ssid))
        }), 200
    elif result[1] == '5134':
        logger.debug("Router configuration is already in Progress")
        return jsonify({
            "status": 'failure',
            "message": "Router configuration is already in Progress"
        }), 400
    elif result[1] == '5102':
        logger.debug("Password length should not be less than 8 characters and not more than 64 characters")
        return jsonify({
                "status": 'failure',
                "message": "Password length should not be less than 8 characters and not more than 64 characters"
            }), 400
    elif result[1] == '5107':
        logger.debug("ssid is not in string format. Internal Error")
        return jsonify({
                "status": 'failure',
                "message": "ssid is not in string format"
            }), 400
    logger.debug("Shifu Internal Error")
    return jsonify({
        "status": 'failure',
        "message": 'Internal Server Error'
    }), 500


# Router API-X3
# Feature:20136-Start
# @application.route('/api/router/set_re', methods=['GET'])
@application.route('/api/router/RE/proceed', methods=['GET'])
# Feature:20136-End
@requires_Auth('R')  # tagging this API as router configuration API(R)
def shifu_connect_re():
    """An API which provides user access to configure shifu as RE"""
    logger.debug("shifu_connect RE API is called")
    # extracting the arguments from API header
    # calling shifu connect function from router_configuration module
    result = rc.set_re()
    # result[1]=6000 on success/5XXX on failure
    if result[1] == '6000':
        logger.debug("Shifu RE configuration is initiated")
        return jsonify({
            "status": 'success',
            "message": "Shifu RE configuration is initiated"
        }), 200
    logger.debug("Shifu is not configured as RE")
    if result[1] == '5134':
        logger.debug("another router configuration is in progress")
        return jsonify({
            "status": 'error',
            "message": 'another router configuration is in progress'
        }), 400
    return jsonify({
        "status": 'failure',
        "message": 'Shifu is not configured as RE'
    }), 500


#  WBLR API-02
# Feature:20136-Start
# @application.route('/api/router/shifu/WBLR/status', methods=['GET'])
@application.route('/api/router/WBLR/status', methods=['GET'])
# Feature:20136-End
@requires_Auth('R')  # tagging this API as router configuration API(R)
def check_wblr_status():
    """A function used to check Legacy router connection in WBLR mode"""
    # Feature:20136-Start
    # output = rc.check_legacy_connection5()
    logger.debug("executing  WBLR status api call")
    output = rc.check_legacy_connection()
    try:
        logger.debug("received response from router  file is : {}".format(output))
        logger.debug("output-1st is {}".format(output[0]))
        logger.debug("output-2nd is {}".format(output[1]))
    except Exception as e:
        logger.debug("Exception is WBLR status :{}".format(e))

    # Featue:20136-End
    if output[0] == 'error':
        logger.debug("entered into error case")
        if output[1] == 500:
            logger.debug("entered into uci parameter issue case")
            return jsonify({
                "status": 'error',
                "message": 'Please reboot t.hub and try again'
            }), 500
        elif output[1] == 601:  # Legacy router not available case
            logger.debug("entered into router not available case")
            return jsonify({
                "status": 'error',
                "message": 'Please check '+ temp_ssid +' Wi-Fi Connectivity and try again'
            }), 601
        elif output[1] == 602:  # wrong password case
            logger.debug("entered into wrong password case")
            return jsonify({
                "status": 'error',
                "message": 'Please enter a valid ' + temp_ssid + ' Wi-Fi password'
            }), 602
    else:
        logger.debug("entered into success case")
        return jsonify({
            "status": 'success',
            "message": 'Shifu is now in WBLR mode'
        }), 200

#  WBLR API-02
# Feature:20136-Start
@application.route('/api/router/EBLR/status', methods=['GET'])
@requires_Auth('R')  # tagging this API as router configuration API(R)
def check_eblr_status():
    """A function used to check Legacy router connection in EBLR mode"""
    output = rc.check_eblr()
    if output == 1:
        return jsonify({
            "status": 'error',
            "message": 'Shifu is not in EBLR mode'
        }), 500
    else:
        return jsonify({
            "status": 'success',
            "message": 'Shifu is in EBLR mode'
        }), 200
# Feature:20136-End

#  WBLR API-03
# Feature:20136-Start
# @application.route('/api/router/shifu/WBLR/reset', methods=['GET'])
@application.route('/api/router/network/reset', methods=['GET'])
# Feature:20136-End
@requires_Auth('R')  # tagging this API as router configuration API(R)
def wblr_reset():
    """A function used to reset the network configuration"""
    logger.debug("Executing /network/reset API")
    # Feature:20136-Start
    # output = rc.qwrap_reset()
    output = rc.network_reset()
    # Feature:20136-End
    if output[1] == '5134':
        logger.debug("another router configuration is in progress")
        return jsonify({
            "status": 'error',
            "message": 'another router configuration is in progress'
        }), 400
    else:
        time.sleep(config.CONSTANT.qwrap_reset_timeout)  # default value is 30 seconds
        return jsonify({
            "status": 'error',
            "message": 'Shifu reset back to Normal mode'
        }), 200


#  Feature-17218:End
# Router API-04
@application.route('/api/router/check/connectivity', methods=['GET'])
@requires_Auth('R')  # tagging this API as router configuration API(R)
def ping_google():
    """A function used to check internet connectivity"""
    logger.debug("/check/Connectivity API is called")
    Internet_status = subprocess.check_output(["uci", "get", "shifu.conf.INTERNET_STATUS"]).strip()
    if Internet_status == '1':
        return jsonify({
            "status": 'success',
            "message": "Network Active"
        }), 200
    logger.debug("Internet status is {}".format(Internet_status))
    return jsonify({
        "status": 'error',
        "message": "Network Error"
    }), 500


# Router API-05
@application.route('/api/router/check/eth', methods=['GET'])
@requires_Auth('R')  # tagging this API as router configuration API(R)
def check_eth():
    """A function which checks whether RJ45 cable is connected or not."""
    logger.debug("/router/check/eth api is called")
    if rc.eth0_check():
        return jsonify({
            "status": 'success',
            "message": "WAN is connected"
        }), 200
    logger.debug("RJ45 cable is not connected.")
    return jsonify({
        "status": 'error',
        "message": "WAN is not connected"
    }), 500


# Router API-06
@application.route('/api/router/scan/aps', methods=['GET'])
@requires_Auth('R')  # tagging this API as router configuration API(R)
def get_aps():
    """A function call to scan for all existing access points"""
    logger.debug("scan/aps API is called")
    output = rc.get_ssids()
    if output == 0:
        logger.debug("/scan/aps API is returning Error")
        return jsonify({
            "status": 'error',
            "message": "Shifu Internal Error"
        }), 500
    logger.debug("/scan/aps API is returning all the aps")
    return jsonify({
        "status": 'success',
        "list": output
    }), 200


# Feature19113:Start APIs to enable and disable Internet on Shifu
# Router API for Automation testing
@application.route('/api/wifi/switch', methods=['POST'])
@requires_Auth('R')  # tagging this API as Router API(R)
def wifi_switch():
    """It turns ON/OFF Ethernet"""
    logger.debug('Ethernet turn ON/OFF API is called')
    # Fetching the switch value from body.
    switch = request.json['switch']
    if not isinstance(switch, bool):
        logger.debug("Invalid Ethernet switch value")
        return jsonify({'status': 'failure', 'message': 'Invalid Ethernet switch value'}), 400
    # Changing Wifi status
    response = rc.wifi_switch(switch)
    # Returning failure message if the response is not Success
    if response != 0:
        logger.debug(response)
        return jsonify({'status': 'failure', 'message': 'Shifu Internal Error'}), 500
    logger.debug('Succesfully changed status of Ethernet')
    # Support19362:Start Log changes to cover WAN disable/enable functionality for automation
    if switch:
        logger.debug("[Ethernet]:[AUTO]:eth1 Turned On")
        return jsonify({'status': 'success', 'message': 'Ethernet Turned On'}), 200
    logger.debug("[Ethernet]:[AUTO]:eth1 Turned Off")
    return jsonify({'status': 'success', 'message': 'Ethernet Turned Off'}), 200
    # Support19362:End
# Feature19113:End

# Feature: 20112-Start
# Router Configuration switching APIs
@application.route('/api/router/network/configuration/check', methods=['GET'])
@requires_Auth('R')  # tagging this API as router configuration API(R)
def check_configuration():
    """A function used to check the Shifu's network configuration mode"""
    output = rc.check_configuration()
    if output[0] == 1:
        return jsonify({
            "status": 'error',
            "mode" : None,
            "message": 'Internal server Error'
        }), 500
    else:
        return jsonify({
            "status": 'success',
            "mode" : output[1],
            "message": 'Shifu is in '+ output[1] + ' mode'
        }), 200


@application.route('/api/router/WBLR/to/EBLR/proceed', methods=['GET'])
@requires_Auth('R')  # tagging this API as router configuration API(R)
def Wblr_to_eblr():
    """An API call to switch Shifu from WBLR to EBLR."""
    logger.debug("WBLR to EBLR API is called")
    # calling the function from router configuration module
    current_mode = rc.check_configuration()
    if current_mode[1] == "EBLR":
        return jsonify({
            "status": 'failure',
            "message": "Hub is already in EBLR mode."
        }), 400
    if not rc.eth0_check():
        return jsonify({
            "status": 'failure',
            "message": "Ethernet cable is not connected to Hub"
        }), 400
    result = rc.switch_WBLR_to_EBLR(current_mode[1])
    # result[1]=6000 on success /5XXX on failure
    if result[1] == '6000':
        logger.debug("Switching from WBLR to EBLR")
        return jsonify({
            "status": 'success',
            "message": "Switching from WBLR to EBLR is initiated"
        }), 200
    elif result[1] == '5134':
        logger.debug("Router configuration is already in Progress")
        return jsonify({
            "status": 'failure',
            "message": "Router configuration is already in Progress"
        }), 400
    logger.debug("Shifu Internal Error")
    return jsonify({
        "status": 'failure',
        "message": 'Internal Server Error'
    }), 500

@application.route('/api/router/EBLR/to/WBLR/proceed', methods=['POST'])
@requires_Auth('R')  # tagging this API as router configuration API(R)
def eblr_to_wblr():
    """An API call to switch Shifu from EBLR to WBLR."""
    logger.debug("EBLR to WBLR API is called")
    # calling the function from router configuration module
    current_mode = rc.check_configuration()
    if current_mode[1] == "WBLR":
        return jsonify({
            "status": 'failure',
            "message": "Hub is already in WBLR mode."
        }), 400
    if rc.eth0_check():
        return jsonify({
            "status": 'failure',
            "message": "Remove Ethernet cable from t.hub WAN port"
        }), 400
    # extracting the arguments from API header
    ssid = str(request.json['ssid'])
    password = str(request.json['password'])
    if ssid=="" or password == "":
        return jsonify({
            "status": 'failure',
            "message": "ssid and password should not be empty"
        }), 400
    result = rc.switch_EBLR_to_WBLR(ssid,password)
    # result[1]=6000 on success /5XXX on failure
    if result[1] == '6000':
        logger.debug("Switching from EBLR to WBLR")
        return jsonify({

            "status": 'success',
            "message": "Switching from EBLR to WBLR is initiated"
            #"hub_ssid": "t.hub-{}".format(str(ssid))
        }), 200
    elif result[1] == '5134':
        logger.debug("Router configuration is already in Progress")
        return jsonify({
            "status": 'failure',
            "message": "Router configuration is already in Progress"
        }), 400
    logger.debug("Shifu Internal Error")
    return jsonify({
        "status": 'failure',
        "message": 'Internal Server Error'
    }), 500

# Feature: 20112-End

#WBLR TO WBLR
@application.route('/api/router/WBLR/to/WBLR/proceed', methods=['POST'])
@requires_Auth('R')  # tagging this API as router configuration API(R)
def wblr_to_wblr():
    """An API call to switch Shifu from WBLR to WBLR."""
    logger.debug("WBLR to WBLR API is called")
    # calling the function from router configuration module
    current_mode = rc.check_configuration()
    if current_mode[1] != "WBLR":
        return jsonify({
            "status": 'failure',
            "message": "Hub is already not in WBLR mode."
        }), 400
    if rc.eth0_check():
        return jsonify({
            "status": 'failure',
            "message": "Remove Ethernet cable from t.hub WAN port"
        }), 400
    # extracting the arguments from API header
    ssid = str(request.json['ssid'])
    password = str(request.json['password'])
    if ssid=="" or password == "":
        return jsonify({
            "status": 'failure',
            "message": "ssid and password should not be empty"
        }), 400
    result = rc.switch_WBLR_to_WBLR(ssid,password)
    # result[1]=6000 on success /5XXX on failure
    if result[1] == '6000':
        logger.debug("Switching from WBLR to WBLR")
        return jsonify({

            "status": 'success',
            "message": "Switching from WBLR to WBLR is initiated"
            #"hub_ssid": "t.hub-{}".format(str(ssid))
        }), 200
    elif result[1] == '5134':
        logger.debug("Router configuration is already in Progress")
        return jsonify({
            "status": 'failure',
            "message": "Router configuration is already in Progress"
        }), 400
    logger.debug("Shifu Internal Error")
    return jsonify({
        "status": 'failure',
        "message": 'Internal Server Error'
    }), 500
# ***************************************#
# Offline Contorl APIs:
# ***************************************#
# Offline Control API-01:
@application.route('/api/thing/list', methods=['GET'])
@requires_Auth('O')
def list_things():
    """Get List of all things and updated details

    :return: Json List of things
    """
    # formatting message
    logger.debug("/thing/list API is called")
    req_id = request.headers.get('X-RequestId')
    logger.debug("RequestID from mobile app is: {}" .format(req_id))
    if(req_id):
        request_id = req_id
    else:
        request_id = 'MA' + str(time.time())
    logger.debug("RequestID: " + request_id)
    msg = {
        'client': 13,
        'command': 'THINGSLIST',
        'notif_id' : request_id
    }
    # initialize things class for further operation
    things = tc.ThingsControl()
    # Send control message to Web Socket
    logger.debug(msg)
    send_status = things.send_control_request(7001, msg)
    if send_status['notif_id'] != request_id:
         logger.critical("SYNC_ERROR: notif_id s are not matching send_status.")
         return jsonify({'status': 'error', 'message': 'INTERNAL HUB ERROR'}), 500
    if send_status['status_code'] == 102:
        return jsonify({'status': 'error', 'message': send_status['message']}), 400
    if send_status['status_code'] != 200:
        return jsonify({'status': 'error', 'message': send_status['message']}), send_status['status_code']
    recv_status = things.receive_control_response(7005, config.CONFIG.MA_CONTROL_TIMEOUT, request_id)
    logger.debug(recv_status)
    if recv_status['notif_id'] != request_id:
         logger.critical("SYNC_ERROR: notif_id s are not matching recv_status.")
         return jsonify({'status': 'error', 'message': 'INTERNAL HUB ERROR'}), 500
    if recv_status['status_code'] == 102:
        return jsonify({'status': 'error', 'message': recv_status['message']}), 400
    if recv_status['status_code'] != 200:
        return jsonify({'status': 'error', 'message': recv_status['message']}), recv_status['status_code']
    return jsonify({'status': 'success', 'message': recv_status['message'], 'things': recv_status['things']}), 200


# Offline Control API-02:
@application.route('/api/thing/control', methods=['POST'])
@requires_Auth('O')
def control_things():
    """An API to control things even when Shifu does not have internet access and mobile app is connected to Shifu.

    This is using Zmq socket for communication with Web Socket with timeout 1 Second
    """
    # Fetching request json data
    logger.debug("/thing/control API is called")
    content = request.json
    req_id = request.headers.get('X-RequestId')
    logger.debug("RequestID from mobile app is: {}".format(req_id))
    if(req_id):
        request_id = req_id
    else:
        request_id = 'MA' + str(time.time())
    logger.debug("RequestID: " + request_id)
    # formatting message
    msg = {
        'client': 13,
        'command': 'CONTROL',
        'things': content['things'],
        'notif_id': request_id
    }
    things = tc.ThingsControl()
    logger.debug(msg)
    send_status = things.send_control_request(7001, msg)
    if send_status['notif_id'] != request_id:
         logger.critical("SYNC_ERROR: notification ids are not matching in send_status.")
         return jsonify({'status': 'error', 'message': 'INTERNAL HUB ERROR'}), 500
    if send_status['status_code'] == 102:
        return jsonify({'status': 'error', 'message': send_status['message']}), 400
    if send_status['status_code'] != 200:
        return jsonify({'status': 'error', 'message': send_status['message']}), send_status['status_code']
    recv_status = things.receive_control_response(7005, config.CONFIG.MA_CONTROL_TIMEOUT, request_id)
    logger.debug(recv_status)
    if recv_status['notif_id'] != request_id:
         logger.critical("SYNC_ERROR: notification ids are not matching in recv_status.")
         return jsonify({'status': 'error', 'message': 'INTERNAL HUB ERROR'}), 500
    if recv_status['status_code'] == 102:
        return jsonify({'status': 'error', 'message': recv_status['message']}), 400
    if recv_status['status_code'] != 200:
        return jsonify({'status': 'error', 'message': recv_status['message']}), recv_status['status_code']
    return jsonify({'status': 'success', 'message': recv_status['message']}), 200

# Certification API:
@application.route('/api/thing/learn_mode', methods=['GET'])
@requires_Auth('O')
def sec_key():
    """

    This is using Zmq socket for communication with Web Socket with timeout 1 Second
    """
    # Fetching request json data
    logger.debug("/thing/learn_mode API is called")
    content = request.json
    req_id = request.headers.get('X-RequestId')
    logger.debug("RequestID from mobile app is: {}".format(req_id))
    if(req_id):
        request_id = req_id
    else:
        request_id = 'MA' + str(time.time())
    logger.debug("RequestID: " + request_id)
    # formatting message
    msg = {
        'client': 13,
        'command': 'LEARN_MODE',
        'notif_id': request_id
    }
    things = tc.ThingsControl()
    logger.debug(msg)
    send_status = things.send_control_request(7001, msg)
    if send_status['notif_id'] != request_id:
         logger.critical("SYNC_ERROR: notification ids are not matching in send_status.")
         return jsonify({'status': 'error', 'message': 'INTERNAL HUB ERROR'}), 500
    if send_status['status_code'] == 102:
        return jsonify({'status': 'error', 'message': send_status['message']}), 400
    if send_status['status_code'] != 200:
        return jsonify({'status': 'error', 'message': send_status['message']}), send_status['status_code']
    recv_status = things.receive_control_response(7005, config.CONFIG.MA_CONTROL_TIMEOUT, request_id)
    logger.debug(recv_status)
    if recv_status['notif_id'] != request_id:
         logger.critical("SYNC_ERROR: notification ids are not matching in recv_status.")
         return jsonify({'status': 'error', 'message': 'INTERNAL HUB ERROR'}), 500
    if recv_status['status_code'] == 102:
        return jsonify({'status': 'error', 'message': recv_status['message']}), 400
    if recv_status['status_code'] != 200:
        return jsonify({'status': 'error', 'message': recv_status['message']}), recv_status['status_code']
    return jsonify({'status': 'success', 'message': recv_status['message'],'sec_key': recv_status['learn_key'][0]['sec_key']}), 200
# ***********************END OF Certification API-02:

# Feature16849:Start
# Offline Control API-03:
@application.route('/api/thing/zwave/exclusion', methods=['GET'])
@requires_Auth('O')
def control_things_exclusion():
    """To keep the device in exclusion mode.
    """
    # formatting message
    logger.debug("/thing/exclusion API is called")
    req_id = request.headers.get('X-RequestId')
    logger.debug("RequestID from mobile app is: {}" .format(req_id))
    if(req_id):
        request_id = req_id
    else:
        request_id = 'MA' + str(time.time())
    logger.debug("RequestID: " + request_id)
    msg = {
        'client': 13,
        'command': 'EXCLUSION',
        'notif_id': request_id
    }
    # initialize things class for further operation
    things = tc.ThingsControl()
    send_status = things.send_control_request(7001, msg)
    if send_status['notif_id'] != request_id:
         logger.critical("SYNC_ERROR: notif_id s are not matching in send_status.")
         return jsonify({'status': 'error', 'message': 'INTERNAL HUB ERROR'}), 500
    if send_status['status_code'] == 102:
        return jsonify({'status': 'error', 'message': send_status['message']}), 400
    if send_status['status_code'] != 200:
        return jsonify({'status': 'error', 'message': send_status['message']}), send_status['status_code']
    recv_status = things.receive_control_response(7005, config.CONFIG.exclusion_timeout, request_id)
    logger.debug(recv_status)
    if recv_status['notif_id'] != request_id:
         logger.critical("SYNC_ERROR: notif_id s are not matching in recv_status.")
         return jsonify({'status': 'error', 'message': 'INTERNAL HUB ERROR'}), 500
    if recv_status['status_code'] == 102:
        return jsonify({'status': 'error', 'message': recv_status['message']}), 400
    if recv_status['status_code'] != 200:
        return jsonify({'status': 'error', 'message': recv_status['message']}), recv_status['status_code']
    return jsonify({'status': 'success', 'message': recv_status[1], 'thing_id': recv_status['things'][0]['thing_id']}), 200


# Feature16849:End

# Feature:20900-Start
# Offline Control API-04:
@application.route('/api/scene/execute', methods=['POST'])
@requires_Auth('O')
def scene_execute():
    """To run the scene offline
    """
    # formatting message
    logger.debug("/api/scene/execute API is being executed")
    scene_id = request.json['scene_id']
    if scene_id == "":
        return jsonify({'status': 'error', 'message': 'SceneId should not be Empty'}), 400
    req_id = request.headers.get('X-RequestId')
    logger.debug("RequestID from mobile app is: {}" .format(req_id))
    if(req_id):
        request_id = req_id
    else:
        request_id = 'MA' + str(time.time())
    logger.debug("RequestID: " + request_id)
    msg = {
        'client': '13',
        'command': 'SCENE',
        'verb': 'RUN',
        'notif_id': request_id,
        'scene_data': [{'scene_id':scene_id}]
    }
    # initialize things class for further operation
    things = tc.ThingsControl()
    # Send control message to ZMQ Socket
    send_status = things.send_control_request(7001, msg)
    # print send_status
    #logger.debug(str(json.dumps(send_status)))
    if send_status['notif_id'] != request_id:
         logger.critical("SYNC_ERROR: notif_id s are not matching in send_status.")
         return jsonify({'status': 'error', 'message': 'INTERNAL HUB ERROR'}), 500
    if send_status['status_code'] == 102:
        return jsonify({'status': 'error', 'message': send_status['message']}), 400
    if send_status['status_code'] != 200:
        return jsonify({'status': 'error', 'message': send_status['message']}), send_status['status_code']
    return jsonify({'status': 'success', 'message': send_status['message']}), 200
# Feature:20900-End

# ***************************************#
# FOTA APIs:
# ***************************************#
# FOTA API - 01:
@application.route('/api/fota/update/proceed', methods=['GET'])
@requires_Auth('F')
def fota_proceed():
    """ This API triggers the FOTA upgradation process on user permission.
    """
    logger.debug('/fota/update/proceed api is called')
    run_cmd = '/usr/bin/fota_update U'
    # Bug19369:Start FOTA upgrade triggered on Luci and App at same instant should be handled.
    with open('/etc/fota_status.json') as f:
        data = json.load(f)
    # upgrade_status = "1" : Upgrade process initiated.
    # upgrade_status will change back to its default "0" value once flashing of image is completed.
    if data["upgrade_status"] == '1':
        return jsonify({'status': 'error', 'message': 'Shifu upgrade already in process'}), 429  ##FIXME: check return code (need to change).
    with open('/etc/fota_status.json') as f:
        data = json.load(f)
    # Bug19369:End
    #Bug19123:Start Race condition handling btw App and cronjob for FOTA update
    # Race condition is handled using FLAG download_status. If download_status='1' it means download is already in progress. Now we throw an error back to Mobile App.
    if data["download_status"] == '1':
        return jsonify({'status': 'error', 'message': 'New update check is in progress'}), 405
    #Bug19123:End
    args = shlex.split(run_cmd)
    try:
        subprocess.Popen(args)
    except OSError:
        logger.critical("Fota service file not found")
        return jsonify({'status': 'error', 'message': 'Shifu Internal Error'}), 500
    except Exception, e:
        logger.critical('service not started properly', exc_info=True)
        return jsonify({'status': 'error', 'message': 'Shifu Internal Error'}), 500
    return jsonify({'status': 'success', 'message': 'Fota upgrade is initialized'}), 200


# FOTA API - 02:
@application.route('/api/fota/update/check', methods=['GET'])
@requires_Auth('F')
def fota_check():
    """ This API checks for the updates from TronX.
    """
    logger.debug("[MA]:/fota/update/check API is called")
    #Bug20244:Start Fota update check process to be continued in the background even after mobile agent timer exhaustion.
    with open('/etc/fota_status.json') as f:
        data = json.load(f)
    # Race condition is handled using FLAG download_status. If download_status = '1', it means download is already in progress. Now we throw an error back to Mobile App.
    if data["download_status"] == '1':
        return jsonify({'status': 'error', 'message': 'New update check is in progress'}), 405
    #Bug20244:End
    run_cmd = "/usr/bin/fota_update D"
    args = shlex.split(run_cmd)
    fota_codes = [0, 1, 2, -1]
    try:
        process = subprocess.Popen(args, stdout=subprocess.PIPE)
    except OSError:
        logger.critical("Fota service file not found")
        return jsonify({'status': 'error', 'message': 'Shifu Internal Error'}), 500
    except Exception, e:
        logger.critical('service not started properly', exc_info=True)
        return jsonify({'status': 'error', 'message': 'Shifu Internal Error'}), 500
    time.sleep(config.CONFIG.fota_timeout)
    exit_code = process.poll()
    if exit_code == 0:
        logger.debug("FOTA application is ended with out any Error")
        for file in os.listdir("/root/downloads/fw/"):
            if file.endswith(".md"):
                changelog_file_path = os.path.join("/root/downloads/fw/", file)
        # Fetching the ChangeLog information
        try:
            with open(changelog_file_path, 'r') as f:
                changelog = f.read()
        except IOError:
            logger.error("changelog file is not found")
            return jsonify({'status': 'failure', 'message': 'Shifu Internal Error'}), 500
        except Exception, e:
            logger.critical('Error case is not handled in /fota/update/check API', exc_info=True)
            return jsonify({'status': 'failure', 'message': 'Shifu Internal Error'}), 500
        # Fetching the release_date and update_version information from /etc/dnld_firmware_info.json file.
        try:
            with open("/etc/dnld_firmware_info.json", "r") as f:
                data = json.load(f)
                update_version = data["downloaded_version"]
                release_date = data["uploaded_at"]
                f.close()
        except Exception, e:
            logger.critical('Error while accessing the file /etc/dnld_firmware_info.json', exc_info=True)
            update_version = None
            release_date = None
        # Giving the response to MobileAPP
        return jsonify({
            "status": "success",
            "update": True,
            "release_version": update_version,
            "release_date": release_date,
            "message": "Fota update is available",
            "change_log": changelog
        }), 200
    elif exit_code in curl_503:
        logger.debug("Couldn't connect to CDN server")
        return jsonify({
            "status": "failure",
            "message": "Couldn't connect to CDN server",
        }), 503
    elif exit_code in curl_504:
        logger.debug("Operation timeout")
        return jsonify({
            "status": "failure",
            "message": "Operation timeout",
        }), 504
    elif exit_code in curl_525:
        logger.debug("SSL handshake failed with CDN")
        return jsonify({
            "status": "failure",
            "message": "SSL handshake failed with CDN",
        }), 525
    elif exit_code in curl_500:
        logger.debug("Shifu Internal Error")
        return jsonify({
            "status": "failure",
            "message": "Shifu internal Error",
        }), 500
    else:
        logger.debug('negative response is received from FOTA')
        return jsonify({
            "status": "Success",
            "update": False,
            "release_version": None,
            "release_date": None,
            "message": "Fota update is not available",
            "change_log": None
        }), 200


# Feature #18574: Start
# FOTA API - 03:
@application.route('/api/fota/downgrade', methods=['GET'])
@requires_Auth('F')
def fota_downgrade():
    """ This API triggers the system downgrading process.
    """
    logger.debug('/fota/downgrade api is called')
    try:
        with open("/etc/shifu_version.json", "r+") as f:
            data = json.load(f)
            data["version"] = "1.0.1"
            f.seek(0)  # rewind
            json.dump(data, f)
            f.truncate()
            f.close()
    except IOError:
        logger.error("shifu_version.json file is not found")
        return jsonify({'status': 'failure', 'message': 'Shifu Internal Error'}), 500
    except Exception, e:
        logger.critical('Error case is not handled in /fota/update/check API', exc_info=True)
        return jsonify({'status': 'failure', 'message': 'Shifu Internal Error'}), 500
    try:
        with open("/etc/dnld_firmware_info.json", "r+") as f:
            data = json.load(f)
            data["downloaded_version"] = "1.0.0"
            data["validity_status"] = "null"
            f.seek(0)  # rewind
            json.dump(data, f)
            f.truncate()
            f.close()
    except IOError:
        logger.error("dnld_firmware_info.json file is not found")
        return jsonify({'status': 'failure', 'message': 'Shifu Internal Error'}), 500
    except Exception, e:
        logger.critical('Error case is not handled in /fota/update/check API', exc_info=True)
        return jsonify({'status': 'failure', 'message': 'Shifu Internal Error'}), 500
    try:
        # os.system('/etc/init.d/ma_bootup start')
        # os.system("kill -SIGUSR1 `ps | grep Mobile_Agent | grep -v grep | awk '{print $1}'`")
        os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
        ma_profile_updates()
    except Exception, e:
        logger.critical('Exception occured while executing ma_bootup service', exc_info=True)
    return jsonify({'status': 'success', 'message': 'Shifu is Downgraded'}), 200


# FOTA API - 04:
@application.route('/api/fota/cronjob/reschedule', methods=['POST'])
@requires_Auth('F')
def fota_cronjob():
    """ This API changes the cron job time for FOTA
    """
    logger.debug('/fota/update/cronjob/update api is called')
    pattern = request.json['cron-pattern']
    logger.debug("Sending fota cron job to VA")
    fota_cron_job = {
        "client_name": "MA",
        "client_action": "FOTA",
        "client_data": {
            "cron_pattern": pattern,
            "executable_string": "/usr/bin/fota_update D"
        }
    }
    logger.debug(json.dumps(fota_cron_job))
    status = va_socket(json.dumps(fota_cron_job))
    if status == 1:
        return jsonify({'status': 'failure', 'message': 'Timeout from VA socket'}), 504
    return jsonify({'status': 'success', 'message': 'FOTA cron job is reshceduled'}), 200


# Feature #18574: End


# ***************************************************#
# Profile_update APIs
# ***************************************************#
# Profile_update API-01:
@application.route('/api/profile/location/change', methods=['POST'])
@requires_Auth('P')  # tagging this API as Bluetooth API(B)
def change_location():
    """It changes the BT name"""
    logger.debug('Location change API is called')
    # Fetching the Bt name from request headers
    latitude = request.json['latitude']
    longitude = request.json['longitude']
    if latitude == "" or longitude == "":
        logger.debug("Location is Empty ")
        return jsonify({'status': 'failure', 'message': 'Empty Location is not Allowed'}), 400
    logger.debug("Location will be changed to " + str(latitude) + str(longitude))
    try:
        with open("/etc/profile.json", "r+") as f:
            data = json.load(f)
            data["devices"]["device_location"] = latitude, longitude
            f.seek(0)  # rewind
            json.dump(data, f)
            f.truncate()
            f.close()
    except IOError:
        logger.critical("profile.json file is not found")
        return jsonify({'status': 'failure', 'message': 'Shifu Internal Error'}), 500
    logger.debug('Location is changed successfully')
    # os.system('/etc/init.d/ma_bootup start')
    # os.system("kill -SIGUSR1 `ps | grep Mobile_Agent | grep -v grep | awk '{print $1}'`")
    os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
    ma_profile_updates()
    return jsonify({'status': 'success', 'message': 'Location is changed successfully'}), 200


# ***************************************#
# Bug:19702-Start
def network_signal(signum, frame):
    """ Starting the Ma_boottime_update based on the signal received from service manger
        pt-thread: pendingtasks thread
    """
    logger.debug("[MA:] network signal received")
    time.sleep(5)
    # pt_thread=threading.Thread(target=MA_boottime_update,name="pt_thread")
    logger.debug(str(threading.enumerate()))
    # if len(threading.enumerate()) >= 2:
    thread_list=[]
    for t in threading.enumerate():
        thread_list.append(t.name)
    if "pt_thread" in thread_list:
        logger.critical("profile update thread is already running")
    else:
        profile_update_pending_flag = subprocess.check_output(["uci", "get", "shifu.conf.MA_PENDING_FLAG"]).strip()
        logger.debug("profile_update_pending_flag is {}".format(profile_update_pending_flag))
        if profile_update_pending_flag == '1':
            logger.debug("Starting profile_updates in a thread")
            pt_thread = threading.Thread(target=ma_profile_updates, name="pt_thread")
            pt_thread.start()
            logger.debug("MA running threads list:" + str(threading.enumerate()))
        else:
            logger.debug("no profile update pending")
# Bug:19702-End

def deregister_alexa(signum,frame):
    logger.debug("Initiated DE-REGISTER")
    logger.debug("Received the signal {} from VA".format(signum))
    try:
        os.system("uci set shifu.conf.ALEXA_REGISTRATION_STATUS=not-registered && uci commit shifu")
        os.system("uci set shifu.conf.SKIP_ALEXA_REGISTRATION=false && uci commit shifu")
        with open("/etc/profile.json", "r+") as f:
            data = json.load(f)
            data["devices"]["about_device"]["alexa_reg_status"] = False
            data["devices"]["about_device"]["skip_alexa_reg_status"] = False
            f.seek(0)  # rewind
            json.dump(data, f)
            f.truncate()
            f.close()
        logger.debug('Alexa de-registered')
    except Exception as e:
        logger.debug("Error is: {}".format(e))
        return jsonify({'status': 1, 'message': "Unable to store Alexa registration status locally in hub"}), 400
    reload(config)
    # os.system("kill -SIGUSR1 `ps | grep Mobile_Agent | grep -v grep | awk '{print $1}'`")
    os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
    ma_profile_updates()
    logger.debug('de-registered and updated to tronx')

def MobileAgent_start():
    """Main function which starts the flask server"""
    global led
    led = LED.LedControl(7891)
    led.connect()
    try:
        ma_port = subprocess.check_output(["uci", "get", "shifu.conf.MA_LISTEN_PORT"]).strip()
    except Exception, e:
        logger.critical('NV variable  MA_LISTEN_PORT is not present', exc_info=True)
        raise
    # Bug:19702-Start
    signal.signal(signal.SIGUSR1, network_signal)
    logger.critical("[MA:] setting signal SIGUSR1 ")
    signal.signal(signal.SIGUSR2, deregister_alexa)
    logger.critical("[MA:] setting signal SIGUSR2")
    # Bug:19702-End
    application.run(host='0.0.0.0', port=int(ma_port),
                    debug=False)  # running the Application on port 3000 and in debug mode.


def MA_boottime_update():
    """This will run as service once and it will be closed automatically"""

    # Feature16870:Start MA with Device reset capability
    logger.debug("MA_boottime_update is executing all the pending tasks")
    logger.debug("Checking Reset status")
    reset_token = config.CONFIG.reset_token
    if reset_token != 'NULL':
        recv_status = ra.device_reset(reset_token)
        if recv_status == 'success':
            try:
                os.system("uci set shifu.conf.MA_FACTORY_RESET_TOKEN=NULL && uci commit shifu")
            except Exception, e:
                logger.critical('Not able to set the NV variable: MA_FACTORY_RESET_TOKEN', exc_info=True)
    logger.debug("Checking EBLR Status")
    if config.CONFIG.eblr_flag == '1':
        logger.debug("executing pending router configuration: EBLR")
        rc.shifu_connect_EBLR()
    # Feature16870:End

    logger.debug("Checking WBLR Status")
    if config.CONFIG.router_flag == '2':
        logger.debug("executing pending router configuration: WBLR")
        ssid = subprocess.check_output(["uci", "get", "shifu.conf.MA_LEGACY_ROUTER_SSID"]).strip()
        password = subprocess.check_output(["uci", "get", "shifu.conf.MA_LEGACY_ROUTER_PASSWORD"]).strip()
        rc.shifu_connect_WBLR_backhaul(ssid, password)

    #generate_keys()
    # Bug-19702:Start
    os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
    ma_profile_updates()
    # Bug-19702:End


# function to handle regular profile update, when received signal to update or at every boot
def regular_profile_update():
    logger.debug("Trying regular profile update")
    # Fetching Existing profile
    try:
        with open("/etc/profile.json", "r") as f:
            data = json.load(f)
            profile_data = data
            # profile_data = {"measurement_units": data["measurement_units"],
            #                 "bluetooth_profile": data["bluetooth_profile"],
            #                 "devices": data["devices"],
            #                 "wifi_settings": data["wifi_settings"],
            #                 "iot_settings": data["iot_settings"]}
            f.close()
    except IOError:
        logger.error("profile.json file is not found")
    except Exception as e:
        logger.debug("exception while reading or copying profile {}".format(e))
        # Updating profile:Start
        # ######Wifi_profile
    try:
        alexa_registration_status = subprocess.check_output(
            ["uci", "get", "shifu.conf.ALEXA_REGISTRATION_STATUS"]).strip()
        if alexa_registration_status == 'registered':
            alexa_registration_status = True
        else:
            alexa_registration_status = False
        profile_data["devices"]["about_device"][
            "alexa_reg_status"] = alexa_registration_status  # data["devices"]["about_device"]["alexa_reg_status"]
    except Exception as e:
        logger.critical('Exception in reading alexa reg status from env:{}'.format(e))

    try:
        skip_alexa_registration_status = subprocess.check_output(
            ["uci", "get", "shifu.conf.SKIP_ALEXA_REGISTRATION"]).strip()
        profile_data["devices"]["about_device"][
            "skip_alexa_reg_status"] = skip_alexa_registration_status  # data["devices"]["about_device"]["skip_alexa_reg_status"]
    except Exception as e:
        logger.critical('Exception in reading skip alexa reg status from profile.json:{}'.format(e))


    # try:
    #     profile_data["wifi_settings"]["legacy_router"]["ssid"] = ast.literal_eval(
    #         subprocess.check_output("iwconfig ath11 | grep ESSID | awk -F: '{print $2}'", shell=True).strip())
    # except Exception, e:
    #     logger.debug("error in getting legacy router: {}".format(e))
    #     profile_data["wifi_settings"]["legacy_router"]["ssid"] = ""

    profile_data["wifi_settings"]["legacy_router"]["ssid"] = rc.get_legacy()  # get legacy router details

    try:
        profile_data["wifi_settings"]["router_settings"]["ssid"] = ast.literal_eval(
            subprocess.check_output("iwconfig ath0 | grep ESSID | awk -F: '{print $2}'", shell=True).strip())
        logger.debug("try router seetings")
    except Exception, e:
        logger.debug("error in getting router setting: {}".format(e))
        profile_data["wifi_settings"]["router_settings"]["ssid"] = ""
    profile_data["wifi_settings"]["mode"] = rc.check_configuration()[1]

    # ######BT_profile
    # ######device_profile
    with open('/etc/shifu_version.json') as f:
        data = json.load(f)
    f.close()
    profile_data["devices"]["about_device"]["device_software_version"] = str(data['version'])
    profile_data["devices"]["about_device"]["min_android_app_version"] = str(data['min_android_app_version'])
    profile_data["devices"]["about_device"]["min_ios_app_version"] = str(data['min_ios_app_version'])

    with open('/etc/dnld_firmware_info.json') as f:
        data1 = json.load(f)
    profile_data["devices"]["about_device"]["last_updated"] = str(data1['last_updated'])
    # updating profile:End
    try:
        with open("/etc/profile.json", "r+") as f:
            f.seek(0)  # rewind
            json.dump(profile_data, f)
            f.truncate()
            f.close()
        logger.debug("Updated profile.json file")
    except Exception as e:
        logger.debug("Unable to open profile.json file: {}".format(e))

    return profile_data

# function to handle profile update, only when there is FOTA update to acomodate new parameters
def fota_profile_update():
    logger.debug("Recreating Device profile due to Fota upgrade.")
    try:
        with open("/etc/profile.json", "r") as f:
            data = json.load(f)
            #profile_data = data
            profile_data = {"bluetooth_profile": data["bluetooth_profile"],
                            "devices": data["devices"],
                            }
            f.close()
    except IOError:
        logger.error("profile.json file is not found")
    except Exception as e:
        logger.debug("exception while reading or copying profile {}".format(e))
        return 'error'

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
    about_device["mac_address"] = subprocess.check_output("cat /sys/class/net/eth0/address",shell=True).strip()
    # about_device["firstName"] = firstname

    if config.CONSTANT.alexa_registration_status == 'registered':
        about_device["alexa_reg_status"] = True
    else:
        about_device["alexa_reg_status"] = False

    if config.CONSTANT.skip_alexa_registration_status == 'true':
        about_device["skip_alexa_reg_status"] = True
    else:
        about_device["skip_alexa_reg_status"] = False

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
    legacy_router["ssid"] = rc.get_legacy()  # get legacy router details

    # try:
    #     legacy_router["ssid"] = ast.literal_eval(
    #         subprocess.check_output("iwconfig ath11 | grep ESSID | awk -F: '{print $2}'", shell=True).strip())
    # except Exception, e:
    #     legacy_router["ssid"] = ""
    try:
        router_settings["ssid"] = ast.literal_eval(
            subprocess.check_output("iwconfig ath0 | grep ESSID | awk -F: '{print $2}'", shell=True).strip())
    except Exception, e:
        router_settings["ssid"] = ""
    # Feature16704:End
    wifi_settings["legacy_router"] = legacy_router
    wifi_settings["router_settings"] = router_settings
    wifi_settings["mode"] = rc.check_configuration()[1]

    # device profile
    iot_settings = {}
    try:
        iot_settings["scene_devices_limit"] = config.CONFIG.DM_SCENE_DEVICES_LIMIT  # Default vaue is 32
        iot_settings["scene_music_limit"] = config.CONFIG.DM_SCENE_SONGS_LIMIT  # Defaut value is 16,
        iot_settings["rule_devices_limit"] = config.CONFIG.DM_RULE_DEVICES_LIMIT  # Defaut vaue is 32,
        iot_settings["rule_music_limit"] = config.CONFIG.DM_RULE_SONGS_LIMIT  # Default value is 16
        iot_settings["max_iot_end_devices"] = config.CONFIG.DM_MAX_IOT_END_DEVICES
        iot_settings["max_areas_limit"] = config.CONFIG.DM_MAX_AREAS_LIMIT
        iot_settings["max_routine_limit"] = config.CONFIG.DM_MAX_ROUTINE_LIMIT
    except Exception as e:
        logger.debug(" error while  creating IOT settings in device profile {}".format(e))

    if config.CONFIG.MA_SmartdeviceNotification == 'true':
        iot_settings["SmartdeviceNotification"] = True  # Default value is false
    if config.CONFIG.MA_SmartdeviceNotification == 'false':
        iot_settings["SmartdeviceNotification"] = False  # Default value is false

    #profile = {}
    profile_data['devices']["about_device"] = about_device
    #profile_data["devices"] = devices
    # profile["alarm_settings"] = alarm_settings
    # profile["reminder_settings"] = reminder_settings
    profile_data["measurement_units"] = measurement_units
    profile_data["wifi_settings"] = wifi_settings
    #profile_data["bluetooth_profile"] = bt_profile
    profile_data["iot_settings"] = iot_settings
    with open('/etc/profile.json', 'w') as outfile:
        json.dump(profile_data, outfile)
    outfile.close()
    # Feature15670:End
    return profile_data

def ma_profile_updates():
    """
    This function tries to update the device profile on interrupt in internet connectivity.
    :return: None
    """
    logger.debug("Trying to Update the device_profile with new patch")
    logger.debug("Checking Reset status")
    reset_token = config.CONFIG.reset_token
    logger.debug("Reset token is {}".format(reset_token))
    if (config.CONSTANT.device_registration_status == 'registered' ):
        # Fetching Existing profile
        try:
            x = time.strftime("%H:%M:%S", time.localtime())
            y = x + '_prfile.json'
            cmd = 'cp /etc/profile.json' + ' ' + '/etc/' + y
            os.system(cmd)
            with open("/etc/profile.json", "r") as f:
                data = json.load(f)
                current_version = data["devices"]["about_device"]["device_software_version"]
            f.close()
            with open('/etc/shifu_version.json') as f:
                data = json.load(f)
                updated_version = str(data['version'])
            f.close()
        except Exception as e:
            logger.debug("problem reading software versions {}".format(e))

        if (current_version == updated_version):
            profile_data = regular_profile_update()
        else:
            profile_data = fota_profile_update()
        try:
            logger.debug("profile data to be update is {}".format(profile_data))
        except Exception as e:
            logger.debug( "Exception printing profile data {}".format(e))

        recv_status = pa.profile_update(config.CONFIG.HOME_TOKEN, profile_data)
        if recv_status == 'success':
            os.system("uci set shifu.conf.MA_PENDING_FLAG=0 && uci commit shifu")
            logger.debug("device profile is updated to Tronx after last reboot/bt_profile update/network interrupt / fota update")
        else:
            logger.debug("Error while updating the device_profile to TronX after the last reboot")
    elif reset_token != 'NULL':
            os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
            recv_status = ra.device_reset(reset_token)
            if recv_status == 'success':
                try:
                    os.system("uci set shifu.conf.MA_FACTORY_RESET_TOKEN=NULL && uci commit shifu")
                    os.system("uci set shifu.conf.MA_PENDING_FLAG=0 && uci commit shifu")
                except Exception, e:
                    logger.critical('Not able to set the NV variable: MA_FACTORY_RESET_TOKEN', exc_info=True)
    else:
        logger.debug("Device is not registered to update the profile to TronX")

#**********************************************************************************
#***********************************************************************************
#******************************************************************************************
#----------------------SECURITY---------------------
def generate_keys():
    #RSA modulus length must be a multiple of 256 and >= 1024
    modulus_length = 256*6
    privatekey = RSA.generate(modulus_length, Random.new().read)
    publickey = privatekey.publickey()
    with open('private_key.txt', 'wb') as f1:
        f1.write(privatekey.exportKey())
    with open('public_key.txt', 'wb') as f2:
        f2.write(publickey.exportKey())
    return privatekey, publickey

def encrypt_message(a_message):
    # encrypted_msg = publickey.encrypt(a_message, 32)[0]
    # encoded_encrypted_msg = base64.b64encode(encrypted_msg) # base64 encoded strings are database friendly
    # return encoded_encrypted_msg
    with open('public_key.txt', 'r') as f2:
        k = f2.read()
        publickey = RSA.importKey(k)
    cipher = PKCS1_OAEP.new(publickey)
    encrypted_message = cipher.encrypt(a_message)
    encoded_encrypted_msg = base64.b64encode(encrypted_message)  # base64 encoded strings are database friendly
    logger.debug('encrypted msg is {}'.format(encoded_encrypted_msg))
    return encoded_encrypted_msg

def decrypt_message(encoded_encrypted_msg):
    # decoded_encrypted_msg = base64.b64decode(encoded_encrypted_msg)
    # decoded_decrypted_msg = privatekey.decrypt(decoded_encrypted_msg)
    # return decoded_decrypted_msg
    with open('private_key.txt', 'r') as f1:
        k = f1.read()
        privatekey = RSA.importKey(k)
    cipher = PKCS1_OAEP.new(privatekey)
    decoded_encrypted_msg = base64.b64decode(encoded_encrypted_msg)
    decoded_decrypted_msg = cipher.decrypt(decoded_encrypted_msg)
    logger.debug('decrypted msg is {}'.format(decoded_decrypted_msg))
    return decoded_decrypted_msg


@application.route('/api/key/exchange', methods=['GET'])
def key_exchange():
    #p1,p2 = generate_keys()
    try:
        key =  (open('public_key.txt').read())
        key2 = key.lstrip("-----BEGIN PUBLIC KEY-----\n")
        key2 = key2.rstrip("\n-----END PUBLIC KEY-----")
        return jsonify({'status': 'success', 'public_key1': key,'public_key2': key2}), 200
    except Exception as e:
        logger.debug("Exception occurred in reading key:{}".format(e))
        return jsonify({'status': 'Failure', 'message': e}), 400

@application.route('/api/security/test', methods=['POST'])
def test():
    """Security testing API"""
    Encry_Acc_tok = request.headers.get('X-AccountToken')
    logger.debug('TIme before decrytpying act tok is')
    logger.debug(datetime.datetime.now())
    Decry_Acc_tok = decrypt_message(Encry_Acc_tok)
    logger.debug('TIme After decrytpying act tok is')
    logger.debug(datetime.datetime.now())
    logger.debug('test yes API is called')
    try:
        response = request.get_data()
        logger.debug("Received data in encrypted format is {}".format(response))
        logger.debug('TIme before decrytpying msg is')
        logger.debug(datetime.datetime.now())
        response = decrypt_message(response)
        logger.debug('TIme After decrytpying msg is')
        logger.debug(datetime.datetime.now())
        logger.debug("Received data after decrypting is {}".format(response))
        #response = "Decrypted_"+str(response)
        response = json.loads(response)
        msg = 'Decrypted!!!!!!!!!!'
    except Exception as e:
        print(e)
        response={}
        msg = 'Sorry could not decrypt :( !!!!!!'
        return jsonify({'status': response, 'message': msg}), 400

    return jsonify({'status': response, 'message': msg, 'Decrypted Acc_Tok':Decry_Acc_tok}), 200

@application.route('/api/router/interface', methods=['GET'])
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
        return jsonify({'status': "error", 'message': "could not get interface information"}), 400
    return jsonify({'status':"success",'interfaces':interface}),200
    #return interface
