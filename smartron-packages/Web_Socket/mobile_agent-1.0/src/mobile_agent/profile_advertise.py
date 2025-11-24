#!/usr/bin/python
"""Module provides a function for device reset."""
# __author__ = "Praveen kumar Donga"
# __porting__ = "Praveen kumar Donga"
# __copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
# __credits__ = ["Praveen"]
# __maintainer__ = "Praveen kumar, Anudeep T"
# __email__ = "praveen.donga@smartron.com"

# Feature16870:Start MA with Device reset capability
import os
import time
import json
import requests
import logging.handlers
import config
import subprocess
from requests.exceptions import ConnectTimeout, ReadTimeout, ConnectionError
# import device_profile as dp

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

def profile_update(home_token,profile_data):
    """ This function is to reset of device from Hub
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
    logger.debug("Profile update API's request_id: " + request_id)
    # calling TronX API to reset device from Hub
    url = TRONX_URL + "/hub/device/profile/partial"
    logger.debug(url)
    headers = {
        'X-DeviceId': home_id,
        'X-HomeToken': home_token,
        'Content-Type': 'application/json',
        'X-RequestId': request_id
    }
    #data = dp.get_profile('17.422725', '78.337106')
    # need to the partial update only
    # with open("/etc/profile.json", "r") as f:
    #     data = json.load(f)
    #     f.close()
    logger.debug(profile_data)
    # if data == 'error':
    #     return 'failure', 500, 'Shifu Internal Error'
    # else:
    #     logger.info("Device profile successfully generated")
    try:
        response = requests.request("POST", url, headers=headers, data=json.dumps(profile_data), timeout=int(timeout))
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
        message = response.json()
        logger.info(message)
        return 'success'
    message = response.json()
    logger.info(message)
    return 'failure'
# Feature16870:End
