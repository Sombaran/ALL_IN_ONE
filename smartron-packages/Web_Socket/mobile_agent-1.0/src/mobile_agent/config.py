#!/usr/bin/python
"""doc string will be added"""
import logging.handlers
import platform
import subprocess

# TODO Change stream here to log file for shifu

##### Adding build no. remove once all are set before release
def get_build():
    x = subprocess.check_output(['lsb_release', '-b'])
    x = x.strip()
    build_no = x.split('IND')[1]
    return build_no
build_no = "B"+get_build()
######## End of adding build no. Need to remove above this
if platform.system() == 'Linux':
    # logger = logging.getLogger(__name__)
    logger = logging.getLogger(build_no + ':'+__name__)
    logger.setLevel(logging.DEBUG)
    handler = logging.handlers.SysLogHandler(address='/dev/log')
    formatter = logging.Formatter('%(name)s %(funcName)s %(lineno)d:%(message)s')
    handler.setFormatter(formatter)
    logger.addHandler(handler)
else:
    raise ValueError('System Not Supported')

HEADERS = {
    'Content-Type': "application/json",
    'X-DeviceId': subprocess.check_output(["fw_printenv", "DEVICE_ID"]).strip().split('=')[1],
    'X-RequestId': 'Test',
    'Cache-Control': "no-cache"
}


class CONSTANT(object):
    # Bug14612:Start This is a part of code-review
    # Bug14748:Start MA_CONTROL_TIMEOUT from NV variables should be changed to <type int>
    # Feature16704:Start
    try:
        MA_CONTROL_TIMEOUT = int(subprocess.check_output(["uci", "get", "shifu.conf.MA_CONTROL_TIMEOUT"]).strip())
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable MA_CONTROL_TIMEOUT is not present', exc_info=True)
    # Bug14748:End
    # Bug14612:End
    try:
        HOME_TOKEN = subprocess.check_output(["uci", "get", "shifu.conf.HOME_TOKEN"]).strip()
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable HOME_TOKEN is not present', exc_info=True)
    # Feature15670:Start [shifu][HM][MA] Changing device profile format
    try:
        DeviceName = subprocess.check_output(["uci", "get", "shifu.conf.DEVICE_NAME"]).strip()
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable DEVICE_NAME is not present', exc_info=True)
    try:
        DEVICE_ID = subprocess.check_output(["fw_printenv", "DEVICE_ID"]).strip().split('=')[1]
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable DEVICE_ID is not present', exc_info=True)
    # Feature16849:Start
    try:
        exclusion_timeout = int(subprocess.check_output(["uci", "get", "shifu.conf.MA_EXCLUSION_TIMEOUT"]).strip())
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable MA_EXCLUSION_TIMEOUT is not present', exc_info=True)
    #  Feature-17218:Start
    try:
        qwrap_connection_timeout = int(
            subprocess.check_output(["uci", "get", "shifu.conf.MA_QWRAP_CONNECT_TIMEOUT"]).strip())
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable MA_QWRAP_CONNECT_TIMEOUT is not present', exc_info=True)
    try:
        qwrap_reset_timeout = int(subprocess.check_output(["uci", "get", "shifu.conf.MA_QWRAP_RESET_TIMEOUT"]).strip())
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable MA_QWRAP_RESET_TIMEOUT is not present', exc_info=True)
    try:
        device_registration_status = subprocess.check_output(["uci", "get", "shifu.conf.DEVICE_REGISTRATION_STATUS"]).strip()
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable DEVICE_REGISTRATION_STATUS is not present', exc_info=True)
    try:
        alexa_registration_status = subprocess.check_output(["uci", "get", "shifu.conf.ALEXA_REGISTRATION_STATUS"]).strip()
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable ALEXA_REGISTRATION_STATUS is not present', exc_info=True)
    try:
        skip_alexa_registration_status = \
            subprocess.check_output(["uci", "get", "shifu.conf.SKIP_ALEXA_REGISTRATION"]).strip()
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable SKIP_ALEXA_REGISTRATION is not present', exc_info=True)
    try:
        # logger.debug("checking the MA_FACTORY_RESET_TOKEN")
        reset_token = subprocess.check_output(["uci", "get", "shifu.conf.MA_FACTORY_RESET_TOKEN"]).strip()
    except Exception, e:
        logger.critical('NV variable MA_FACTORY_RESET_TOKEN is not present', exc_info=True)
    try:
        # logger.debug("checking the MA_EBLR_FLAG")
        eblr_flag = subprocess.check_output(["uci", "get", "shifu.conf.MA_EBLR_FLAG"]).strip()
    except Exception, e:
        logger.critical('NV variable MA_EBLR_FLAG is not present', exc_info=True)
    try:
        # logger.debug("checking the MA_ROUTER_FLAG")
        router_flag = subprocess.check_output(["uci", "get", "shifu.conf.MA_ROUTER_FLAG"]).strip()
    except Exception, e:
        logger.critical('NV variable MA_ROUTER_FLAG is not present', exc_info=True)
    try:
        fota_timeout = int(subprocess.check_output(["uci", "get", "shifu.conf.MA_FOTA_TIMEOUT"]).strip())
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable MA_FOTA_TIMEOUT is not present', exc_info=True)
        fota_timeout = 28
    # Redmine 19607: Start
    try:
        ACCOUNT_TOKEN = subprocess.check_output(["uci", "get", "shifu.conf.ACCOUNT_TOKEN"]).strip()
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable ACCOUNT_TOKEN is not present', exc_info=True)
    try:
        MA_TRONX_TIMEOUT = subprocess.check_output(["uci", "get", "shifu.conf.MA_TRONX_TIMEOUT"]).strip()
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable MA_TRONX_TIMEOUT is not present', exc_info=True)
    try:
        TRONX_URL = subprocess.check_output(["uci", "get", "shifu.conf.TRONX_URL"]).strip()
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable TRONX_URL is not present', exc_info=True)
    # Redmine 19607: End
    #  Feature-17218:End
    # Feature16849: End
    # Feature16704:End
    # FIX ME: SWVersion = subprocess.check_output(["uci", "get", "shifu.conf.SWVersion "]).strip()
    # FIX ME: Serial_Number = subprocess.check_output(["uci", "get", "shifu.conf.Serial_Number "]).strip()
    # FIX ME: MACID = subprocess.check_output(["uci", "get", "shifu.conf.MACID"]).strip()
    # Feature15670:End
    # Bug:20611- Start
    try:
        BT_DEVICE_LIMIT = subprocess.check_output(["uci", "get", "shifu.conf.BT_DEVICE_LIMIT"]).strip()
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable BT_DEVICE_LIMIT is not present', exc_info=True)
    try:
        DM_SCENE_DEVICES_LIMIT = subprocess.check_output(["uci", "get", "shifu.conf.DM_SCENE_DEVICES_LIMIT"]).strip()
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable DM_SCENE_DEVICES_LIMIT is not present', exc_info=True)
    try:
        DM_SCENE_SONGS_LIMIT = subprocess.check_output(["uci", "get", "shifu.conf.DM_SCENE_SONGS_LIMIT"]).strip()
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable DM_SCENE_SONGS_LIMIT is not present', exc_info=True)
    try:
        DM_RULE_DEVICES_LIMIT = subprocess.check_output(["uci", "get", "shifu.conf.DM_RULE_DEVICES_LIMIT"]).strip()
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable DM_RULE_DEVICES_LIMIT is not present', exc_info=True)
    try:
        DM_RULE_SONGS_LIMIT = subprocess.check_output(["uci", "get", "shifu.conf.DM_RULE_SONGS_LIMIT"]).strip()
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable DM_RULE_SONGS_LIMIT is not present', exc_info=True)

    try:
        MA_SmartdeviceNotification = subprocess.check_output(["uci", "get", "shifu.conf.MA_SmartdeviceNotification"]).strip()
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable MA_SmartdeviceNotification is not present', exc_info=True)

    try:
        DM_MAX_IOT_END_DEVICES = subprocess.check_output(["uci", "get", "shifu.conf.DM_MAX_IOT_END_DEVICES"]).strip()
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable DM_MAX_IOT_END_DEVICES is not present', exc_info=True)

    try:
        DM_MAX_AREAS_LIMIT = subprocess.check_output(["uci", "get", "shifu.conf.DM_MAX_AREAS_LIMIT"]).strip()
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable DM_MAX_AREAS_LIMIT is not present', exc_info=True)

    try:
        DM_MAX_ROUTINE_LIMIT = subprocess.check_output(["uci", "get", "shifu.conf.DM_MAX_ROUTINE_LIMIT"]).strip()
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable DM_MAX_ROUTINE_LIMIT is not present', exc_info=True)

    # Feature 22289:Start
    try:
        INTERNET_MOBI_NOTIFICATION = subprocess.check_output(["uci", "get", "shifu.conf.INTERNET_MOBI_NOTIFICATION"]).strip()
    except Exception, e:
        logger.critical('Unexpected Error or NV Variable INTERNET_MOBI_NOTIFICATION is not present', exc_info=True)
    # Feature 22289:End

    # Bug:20611- End
    LOGGER = logger

    def __setattr__(self, *_):
        raise AttributeError("These Value are constant and can not changed")


CONFIG = CONSTANT()
