#!/usr/bin/python
"""Need to be added"""

__author__ = "Hemant Jiwani"
__copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
__credits__ = ["Hemant Jiwani"]
__maintainer__ = "Hemant Jiwani, Anudeep T"
__email__ = "hemant.jiwani@smartron.com"
__status__ = "Testing"

import zmq
import json
import ast
import config
import ast

context = zmq.Context()
logger = config.logger


class ThingsControl:
    """Things control class to manage control event for things oin offline mode
    """

    def __init__(self):
        """
        parameters are for information purpose and BUG tracking
        """
        self.thingsId = None
        self.messageId = None

    def send_control_request(self, port, message):
        """
        General function to send request to ZMQ server running on port
        (Used here for sending message to Web Socket)
        :param port: Port which Web Socket is accepting Control request
        :param message: Control message to be send
        :return: Weather successfully accepted request by Web Socket or not
        """
        # using global context to save extra resources (Fully safe and advisable by ZMQ)
        global context
        # Creating a REQ socket
        socket = context.socket(zmq.REQ)
        # Setting linger to 0 to prevent infinite wait and to use poller
        socket.setsockopt(zmq.LINGER, 0)
        # Connecting socket to given port
        socket.connect("tcp://127.0.0.1:{}".format(port))
        # Bug14650:Start
        # Sending message as string
        # socket.send_json(message)
        logger.debug("Sending control request")
        message = json.dumps(message)
        socket.send_multipart([message, b''])
	# Bug14650: End
        # initialize poller for polling event
        poller = zmq.Poller()
        poller.register(socket, zmq.POLLIN)
        # polling for 1 Second to send message status for Web Socket
        if poller.poll(1000):  # 1s timeout in milliseconds
            # Receive ACK message from Web Socket within polling time
            msg = socket.recv()
            msg = ast.literal_eval(msg)
            #logger.debug(msg)
            # Since this will be used very often by Mobile socket should be closed
            socket.close()
            # Change class value for Debugging
            # self.messageId = msg
            #msg=json.loads(msg)
            return msg
        socket.close()
        # return timeout if control not accepted by Web Socket within 1 Second
        msg = {
            "status_code" : 504,
            "message" : "Gateway timeOut",
            "notif_id": message['notif_id']
        }
        return msg

    # Feature16849:Start Adding timeout as a new parameter
    def receive_control_response(self, port, timeout, requst_id):
        """
        Open a Zmq socket to accept Control event success request
        :param port: Port which , it want to listen update
        :param timeout
        :return: Status
        """
        # using global context to save extra resources (Fully safe and advisable by ZMQ)
        global context
        # Creating a REP socket
        socket = context.socket(zmq.REP)
        socket.setsockopt(zmq.LINGER, 0)
        # Binding to given port
        socket.bind("tcp://127.0.0.1:{}".format(port))
        # initialize poller for polling event
        poller = zmq.Poller()
        poller.register(socket, zmq.POLLIN)
        # waiting for 5 Second to get status about control event
        # Bug14612:Start timeout is changed from 5000 to 5100
        #  if poller.poll(config.CONFIG.MA_CONTROL_TIMEOUT):  # 1s timeout in milliseconds #Fix_me
        if poller.poll(timeout):  # 1s timeout in milliseconds #Fix_me
            # Bug14162: End
            # Receive status message
            logger.debug("Receiving control request")
            msg = socket.recv()
            logger.debug(msg)
            msg = ast.literal_eval(msg)
            # Send ACK for received message
            socket.send('received')
            socket.close()
            # self.messageId = msg
            ##Bug14612:Start ast is used because json.loads is not working on board
            # msg = json.loads(msg)
            # logger.debug(msg)
            return msg
            # msg = ast.literal_eval(msg)
            # logger.debug() #Fix me
            # if msg['status_code'] == 200 and msg['command'] == 'THINGSLIST':
            #     return 'success', msg['message'], json.loads(json.dumps(msg['things']))
            # elif msg['status_code'] == 200 and msg['command'] == 'CONTROL':
            #     return 'success', msg['message'],msg['status_code']
            # elif msg['status_code'] == 200 and msg['command'] == 'SCENE':
            #     return 'success', msg['message'], msg['status_code']
            # elif msg['status_code'] == 200 and msg['command'] == 'EXCLUSION':
            #     try:
            #         thing_id = msg['things'][0]['thing_id']
            #     except Exception, e:
            #         logger.debug("not able to fetch the thing_id", exc_info=True)
            #         thing_id = 'NULL'
            #     return 'success', msg['message'], json.loads(json.dumps(thing_id))
            # elif msg['status_code'] != 200:
            #     return 'error', msg['message']
            # else:
            #     return 'error', 'Invalid data sent from DM'
        else:
            socket.close()
            msg = {
                "status_code" : 504,
                "message" : "DM is not responding within timeout",
                "notif_id": requst_id
            }
            return msg
