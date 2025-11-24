import config

# import socketservice


logger = config.CONFIG.LOGGER

# def start_service():
#     logger.info('Web Socket starting')
#     ss = socketservice.SocketIoClientThread()
#     logger.debug('SocketIO client Process starting')
#     ss.start()
#     logger.debug('Socket Client Started')

from socketservice import start_service
