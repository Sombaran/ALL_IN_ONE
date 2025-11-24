#!/usr/bin/python
"""Module provides a function for registration."""
# __author__ = "Shiva verma"
# __porting__ = "Praveen kumar Donga"
# __copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
# __credits__ = ["Shiva Verma"]
# __maintainer__ = "Praveen kumar"
# __email__ = "praveen.donga@smartron.com"

import os
import time
import json
import requests
# Feature15670:Start [shifu][HM][MA] Changing device profile format
import device_profile as dp
# Feature15670:End
import config
import logging.handlers
from requests.exceptions import ConnectTimeout, ReadTimeout, ConnectionError
import zmq
import ast
import random
import subprocess
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

# Bug18413: Start
## Voice Assistant Socket
def zmq_socket(va_msg, port, timeout):
    """
    - Passing router connection state to voice assistant
    """
    logger.debug("zmq_socket funtion is called")
    context = zmq.Context()
    socket = context.socket(zmq.REQ)
    socket.setsockopt(zmq.LINGER, 0)
    socket.connect("tcp://localhost:" + str(port))
    socket.send(va_msg)
    poller = zmq.Poller()
    poller.register(socket, zmq.POLLIN)
    if poller.poll(timeout):  # 5s timeout in milliseconds
        socket_status = socket.recv()
        socket_status = ast.literal_eval(socket_status)
        logger.debug("Response from DM/VA at 7001/8800 port is {}".format(socket_status))
    else:
        logger.error("MA Error: ZMQ reception failed")
    socket.close()


# Bug18413:End


def device_registration(temp_token, latitude, longitude, firstname):
    """:param temp_token: Received from the user

    This function register the device to cloud. And saves HOME_TOKEN and ACCOUNT_TOKEN
    to system env. And sets the value of 'DEVICE_REGISTRATION_STATUS' to 'registered'

    :return: Returns the status code and corresponding message received from the cloud.
    """
    # Fetching DEVICE_ID from NV variable
    try:
        # Redmine 19607: Start
        home_id = config.CONSTANT.DEVICE_ID
        # Redmine 19607: End
        logger.debug(str(home_id))
    except Exception, e:
        logger.critical("Not able to access the NV variable: DEVICE_ID", exc_info=True)
        return 'failure', 500, 'Shifu Internal Error'
    # creating request_id: TronX, Shifu team is agreed upon this format
    request_id = home_id + 'MA' + str(time.time())
    # Fetching MA_TRONX_TIMEOUT from NV variable
    try:
        # Redmine 19607: Start
        timeout = config.CONSTANT.MA_TRONX_TIMEOUT
        # Redmine 19607: End
        logger.debug(str(timeout))
    except Exception, e:
        logger.critical("Not able to access the NV variable: MA_TRONX_TIMEOUT", exc_info=True)
        return 'failure', 500, 'Shifu Internal Error'
    # Fetching TRONX_URL from NV variable
    try:
        # Redmine 19607: Start
        TRONX_URL = config.CONSTANT.TRONX_URL
        # Redmine 19607: End
        logger.debug(str(TRONX_URL))
    except Exception, e:
        logger.critical("Not able to access the NV variable: TRONX_URL", exc_info=True)
        return 'failure', 500, 'Shifu Internal Error'
    # calling TronX API to verify the HomeTempToken
    url = TRONX_URL + "/hub/auth/token/verify"
    logger.debug(url)
    # Feature15866:Start Included Content-Type missing header field
    headers = {
        'X-DeviceId': home_id,
        'X-HomeTempToken': temp_token,
        'Content-Type': 'application/json',
        'X-RequestId': request_id
    }
    # Feature15866:End
    # Feature15670:Start Changing device profile format
    data = dp.get_profile(latitude, longitude, firstname)
    logger.debug(data)
    # Feature16704:Start
    if data == 'error':
        return 'failure', 500, 'Shifu Internal Error'
    else:
        logger.info("Device profile successfully generated")
    # Feature16704:End
    try:
        # Feature15866:Start Converting data into a json string
        response = requests.request("POST", url, headers=headers, data=json.dumps(data), timeout=int(timeout))
    # Feature15866:End
    # Feature15670:End
    except ConnectTimeout:
        logger.debug("The request timed out while trying to connect to the remote server")
        return 'failure', 408, 'Request Timeout'
    except ReadTimeout:
        logger.debug("The server did not send any data in the allotted amount of time")
        return 'failure', 504, 'Gateway Timeout'
    except ConnectionError:
        logger.debug("600:Check Internet Connection")
        return 'failure', 600, 'Problem with Internet Connection'
    except Exception, e:
        logger.error("700: Unexpected Error", exc_info=True)
        logger.error(e)
        return 'failure', 700, 'Unexpected Error'
    # if registration is successful
    if response.status_code == 200:
        home_t = response.headers['X-HomeToken']
        acc = response.headers['X-AccountToken']
        # saving HOME_TOKEN, ACCOUNT_TOKEN in system env
        try:
            with open("/etc/tokens.json", "r+") as f:
                data = json.load(f)
                data["HomeToken"] = str(home_t)
                data["AccountToken"] = str(acc)
                data["ResetToken"] = "NULL"
                f.seek(0)  # rewind
                json.dump(data, f)
                f.truncate()
                f.close()
        except IOError:
            logger.error("tokens.json file is not found")
        try:
            os.system("uci set shifu.conf.HOME_TOKEN=%s && uci commit shifu" % (home_t))
        except Exception, e:
            logger.critical('Not able to access the NV variable: HOME_TOKEN', exc_info=True)
            return 'failure', 500, 'Shifu Internal Error'
        try:
            os.system("uci set shifu.conf.ACCOUNT_TOKEN=%s && uci commit shifu" % (acc,))
        except Exception, e:
            logger.critical('Not able to access the NV variable: Account Token', exc_info=True)
            return 'failure', 500, 'Shifu Internal Error'
        try:
            os.system("uci set shifu.conf.DEVICE_REGISTRATION_STATUS=registered && uci commit shifu")
        except Exception, e:
            logger.critical('Not able to access the NV variable: device_registration_stauts', exc_info=True)
            return 'failure', 500, 'Shifu Internal Error'
        try:
            os.system("uci set shifu.conf.MA_FACTORY_RESET_TOKEN=NULL && uci commit shifu")
        except Exception, e:
            logger.critical('Not able to access the NV variable: MA_FACTORY_RESET_TOKEN', exc_info=True)
            return 'failure', 500, 'Shifu Internal Error'
        logger.debug("home-token and acc-token are saved into system variables")
        logger.info("User Login is successful")
        # Bug18413:Start
        logger.debug("giving voice response")
        TTS = {
            "client_name": "MA",
            "client_action": "VOICE",
            "client_data": {
                "online": None,
                "offline": "ShifuConfigured.mp3",
                "led": False
            }
        }
        logger.debug(json.dumps(TTS))
        # va_socket(json.dumps(TTS),8800,5000)
        zmq_socket(json.dumps(TTS), 8800, 5000)
        logger.debug("Sending HomeToken to VA")
        Token = {
            "client_name": "MA",
            "client_action": "HOMETOKEN",
            "client_data": home_t
        }
        logger.debug(json.dumps(Token))
        # va_socket(json.dumps(Token))
        zmq_socket(json.dumps(Token), 8800, 5000)
        # Feature 17490: Start
        logger.debug("Sending fota cron job to VA")
        fota_cron_job = {
            "client_name": "MA",
            "client_action": "FOTA",
            "client_data": {
                # Generating the random cron schedule between 1:00 AM to 4:59 AM everyday.
                "cron_pattern": "{m} {h} * * *".format(m=random.randint(0,59), h=random.randint(1,4)),
                "executable_string": "kill -SIGUSR2 `pidof fota_update`"
            }
        }
        logger.debug(json.dumps(fota_cron_job))
        # va_socket(json.dumps(fota_cron_job))
        zmq_socket(json.dumps(fota_cron_job), 8800, 5000)
        # Feature 17490: End
        # Bug18413:End
        # Feature14873:Start Web Socket and Device Manager and Voice Assistant is restarted every time on-boarding process is done


        # for test purpose commenting start & stop of websocket. from MA,. WS should do it automatically based on token
        '''
        try:
            os.system('/etc/init.d/ws_service stop')
        except Exception, e:
            logger.critical("Unable to stop WS service", exc_info=True)
            return 'failure', 500, 'Could not stop WS service'
        try:
            os.system('/etc/init.d/ws_service start')
        except Exception, e:
            logger.critical("Unable to start WS service", exc_info=True)
            return 'failure', 500, 'Could not start WS service'
        logger.debug("WS service successfully stopped and started again")
        '''
        logger.debug("WebSocket should automatically connect back to Server")
        try:
            os.system('/etc/init.d/dm_service stop')
        except Exception, e:
            logger.critical("Unable to stop DM service", exc_info=True)
            return 'failure', 500, 'Could not stop DM service'
        try:
            os.system('/etc/init.d/dm_service start')
        except Exception, e:
            logger.critical("Unable to start DM service", exc_info=True)
            return 'failure', 500, 'Could not start DM service'
        logger.debug("DM service successfully stopped and started again")
        # MQTT server re-start
        logger.debug("Sending AccountToken to MQTT")
        Token = {
            "client": "MA",
            "message_id": "MA1234",
            "client_data": {
                "tag": "Device_details",
                "tag_data": [
                    {
                        "account_token": acc,
                        "home_token": home_t
                    }
                ]
            }
        }
        logger.debug(json.dumps(Token))
        # va_socket(json.dumps(Token))
        zmq_socket(json.dumps(Token), 7003, 1000)
        # try:
        #     os.system('/etc/init.d/mqtt_service stop')
        # except Exception, e:
        #     logger.critical("Unable to stop MQTT service", exc_info=True)
        #     return 'failure', 500, 'Could not stop MQTT service'
        # try:
        #     os.system('/etc/init.d/mqtt_service start')
        # except Exception, e:
        #     logger.critical("Unable to start MQTT service", exc_info=True)
        #     return 'failure', 500, 'Could not start MQTT service'
        # logger.debug("MQTT service successfully stopped and started again")
        # MQTT server re-start
        # Bug18413:Start
        # # Bug16955:Start VA restart is necessary whenever device registration happens to fetch changed tokens
        # try:
        #     os.system('/etc/init.d/va_service stop')
        # except Exception, e:
        #     logger.critical("Unable to stop VA service", exc_info=True)
        #     return 'failure', 500, 'Could not stop VA service'
        # try:
        #     os.system('/etc/init.d/va_service start')
        # except Exception, e:
        #     logger.critical("Unable to start VA service", exc_info=True)
        #     return 'failure', 500, 'Could not start VA service'
        # logger.debug("VA service successfully stopped and started again")
        # # Bug16955:End
        # Bug18413: End
        # Feature14873:End
        message = response.json()
        message = message["status"]
        return 'success', response.status_code, message
    message = response.json()
    message = message["error"]
    return 'failure', response.status_code, message
