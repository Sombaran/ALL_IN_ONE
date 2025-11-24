""" BT Logger Configuration """

import logging.handlers

## Function call for Logger Initialisation
def log_init():
    """ 
    - Initialising the Logger
    - Setting the Logging Level
      Possible Logging Levels = logging.DEBUG, logging.INFO, logging.ERROR, logging.NOTSET
    - Setting Logger Format
    
    """
    log = logging.getLogger(__name__)
    if not getattr(log,'handler_set',None):
        log.setLevel(logging.DEBUG)
        handler = logging.handlers.SysLogHandler(address='/dev/log')
        formatter = logging.Formatter('%(module)s: %(message)s')
        handler.setFormatter(formatter)
        log.addHandler(handler)
        log.handler_set=True
    return log