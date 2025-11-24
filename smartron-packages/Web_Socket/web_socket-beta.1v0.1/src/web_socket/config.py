import logging.handlers
import platform
import subprocess


##### Adding build no. remove once all are set before release
def get_build():
    x = subprocess.check_output(['lsb_release', '-b'])
    x = x.strip()
    build_no = x.split('IND')[1]
    return build_no
build_no = "B"+get_build()
######## End of adding build no. Need to remove above this

if platform.system() == 'Linux':
    #logger = logging.getLogger(__name__)
    logger = logging.getLogger(build_no)
    logger.setLevel(logging.DEBUG)
    handler = logging.handlers.SysLogHandler(address='/dev/log')
    formatter = logging.Formatter('%(name)s %(module)s.%(funcName)s %(lineno)d:%(message)s')
    handler.setFormatter(formatter)
    logger.addHandler(handler)
else:
    raise ValueError('System Not Supported')


class CONSTANT(object):
    HOST_NAME = subprocess.check_output(["uci", "get", "shifu.conf.TRONX_URL"]).strip()
    LOCAL_PORT_NUMBER = ''
    WS_CLIENT_SENT_PORT = 7001
    WS_CLIENT_RECEIVE_PORT = 7006
    SOCKET_IO_RESOURCE = "/hub/socket.io"
    HOME_TOKEN = subprocess.check_output(["uci", "get", "shifu.conf.HOME_TOKEN"]).strip()
    LOGGER = logger

    def __setattr__(self, *_):
        raise AttributeError("These Value are constant and can not changed")


CONFIG = CONSTANT()

