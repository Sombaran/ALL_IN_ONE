#!/usr/bin/python

__author__ = "Hemant Jiwani"
__copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
__credits__ = ["Hemant Jiwani"]
__maintainer__ = "Hemant Jiwani"
__email__ = "hemant.jiwani@smartron.com"
__status__ = "Testing"

import threading
import json
import sys
# Bug-19702:Start
import os
# Bug-19702:Start
import config
import ast
import Queue
import thread
import time
import subprocess
import re
from requests.exceptions import ConnectionError
import socket
from socketIO_client import SocketIO, namespaces, BaseNamespace
from mobile_agent import led_client as LED


logger = config.CONFIG.LOGGER
logger.debug ("finished importing")
try:
    import zmq
except ImportError:
    logger.critical('zmq Python package not found in system application closing')
    sys.exit()


class WSClientTask:
    """
    Web Socket Client Class
    """

    def __init__(self):
        """
        """
        self.messageId = None
        self.context = zmq.Context()

    def send_request(self, port, msg, timeout, notif_id):
        """
        :param port: port number on which web socket server is running
        :param msg: message or payload from tronX socket io server
        :return: ack message from web socket server
        """
        logger.debug("Sending msg {} to Device manager/VA".format(msg))
        socket = self.context.socket(zmq.REQ)
        socket.setsockopt(zmq.LINGER, 0)
        socket.connect("tcp://127.0.0.1:{}".format(port))
        logger.debug("Msg size before sending to Device manager is {}".format(sys.getsizeof(msg)))
        #ret_val=socket.send(msg)
        socket.send_multipart([msg, b''])
        poller = zmq.Poller()
        poller.register(socket, zmq.POLLIN)
        if poller.poll(timeout):  # 1s timeout in per milliseconds
            msg = socket.recv()
            msg = ast.literal_eval(msg)
            socket.close()
            logger.debug("Acknowledgement from DM/VA for sent request at 7001/8800 port is {}".format(msg))
            # 20848-Start
            if msg['notif_id'] != notif_id:
                 logger.critical("SYNC_ERROR: notif_id s are not matching in send_request.")
                 return 'error', "INTERNAL HUB ERROR", 500
            # 208408 - End
            if msg['status_code'] == 200:
                return 'success', msg['message'], msg['status_code']
            else:
                return 'error', msg['message'], msg['status_code']
        else:
            socket.close()
            return 'error', 'TimeOut Error no response from server', 504

    def receive_response(self, port, timeout, notif_id):
        socket = self.context.socket(zmq.REP)
        socket.setsockopt(zmq.LINGER, 0)
        socket.bind("tcp://127.0.0.1:{}".format(port))
        poller = zmq.Poller()
        poller.register(socket, zmq.POLLIN)
        if poller.poll(timeout):  # 5s timeout in milliseconds
            msg=''
            logger.debug("data size before getting response from Device manager is {}".format(sys.getsizeof(msg)))
            msg = socket.recv()
            logger.debug("data size after getting response from Device manager is {}".format(sys.getsizeof(msg)))
            socket.send('received')
            socket.close()
            msg = ast.literal_eval(msg)
            logger.debug("Response from DM for controlled operation at 7006 port is {}".format(msg))
            if msg['notif_id'] != notif_id:
                 logger.critical("SYNC_ERROR: notif_id s are not matching in receive_response.")
                 return 'error', "INTERNAL HUB ERROR", 500
            if msg['status_code'] == 200:
                return 'success', msg['message'], msg['status_code']
            elif msg['status_code'] == 102:
                return 'error', msg['message'], 400
            else:
                return 'error', msg['message'], msg['status_code']
        else:
            socket.close()
            return 'error', 'Timeout Error no response from client', 504

## Fota server Socket start
def fota_socket(fota_msg, timeout, port=7771):
    """
    - Sending FOTA request to fota server
    """
    logger.debug("fota_socket function is called")
    BUFSIZE = 1024
    try:
        clientSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        #clientSock.bind(('127.0.0.1', 7772))
        clientSock.sendto(str(fota_msg),('127.0.0.1', 7771))
        clientSock.settimeout(timeout)
        try:
            responseData = clientSock.recv(BUFSIZE)  # try to receive 100 bytes
            data = responseData.strip('\0')
            logger.debug("data from fota server is: {},,,,{}".format(type(data),data))
            #response = ast.literal_eval(data)
            response = json.loads(data)
            return response
        except socket.timeout:  # fail after 30 second of no activity
            response = {'status':'error','body': {'message':'Timeout.Please try after sometime'},'status_code':504 }
            return response
    except Exception as e:
        logger.debug('Exception in fota socket is : {}'.format(e))
        response = {'status': 'error', 'body': {'message':'Please try after sometime'}, 'status_code': 504}
        return response
queue = Queue.Queue(maxsize=100)
command_status = False

# VA socket connection
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
            logger.error("WS Error: ZMQ reception failed")
            socket.close()
            return response
        socket.close()
        return response
    except Exception as e:
        logger.error('Exception in va_socket is: {}'.format(e))
        return response

class ShifuNamespace(BaseNamespace):
    """
    shifu Namespace class
    """

    # Feature17524: start
    def ordered(self, obj):
        if isinstance(obj, dict):
            return sorted((k, self.ordered(v)) for k, v in obj.items())
        if isinstance(obj, list):
            return sorted(self.ordered(x) for x in obj)
        else:
            return obj

    # Feature17524: End

    def handle_command(self):
        logger.debug("handle command started")
        command_status = True
        while queue._qsize() > 0:
            payload = queue.get()
            verb = payload['verb']
            notification_id = payload['notif_id']
            logger.debug("WS is processing :" + notification_id)
            # THING DISCOVERY
            if verb == "things_discover":
                discovery = {
                    'client': 12,
                    'command': 'DISCOVERY',
                    'notif_id': notification_id
                }
                client_task_response = WSClientTask().send_request(config.CONFIG.WS_CLIENT_SENT_PORT,
                                                                    json.dumps(discovery), 1000, notification_id)
                logger.debug('Response from Client task {}'.format(client_task_response))
                self.emit('ack', {'status': client_task_response[0], 'message': client_task_response[1],
                                  'notif_id': notification_id, 'status_code': client_task_response[2]})
                logger.debug("response code " + str(client_task_response[2]) + " is sent to tronX for " + notification_id)
            # THING CONTROL
            elif verb == "control":
                control_single = {
                    'client': 12,
                    'command': 'CONTROL',
                    'things': payload["things"],
                    'notif_id': notification_id
                }
                # Sending only request to Device manager
                # & Not waiting for result for actual action performed
                client_task_response = WSClientTask().send_request(config.CONFIG.WS_CLIENT_SENT_PORT,
                                                                    json.dumps(control_single), 1000, notification_id)
                logger.debug('Response from Device manager for sent request: {}'.format(client_task_response))
                # if 'success' in client_task_response:
                # if client_task_response[2] == 200:
                #     # Bug:19688-Start
                #     # recv_status = WSClientTask().receive_response(config.CONFIG.WS_CLIENT_RECEIVE_PORT, 7000)
                #     recv_status = WSClientTask().receive_response(config.CONFIG.WS_CLIENT_RECEIVE_PORT, 8000,
                #                                                   notification_id)
                #     # Bug:19688-End
                #     logger.debug('Response from Client task on control operation {}'.format(recv_status))
                #     self.emit('ack', {'status': recv_status[0], 'message': recv_status[1],
                #                       'notif_id': notification_id, 'status_code': recv_status[2]})
                #     logger.debug("response code " + str(recv_status[2]) + " is sent to tronX for " + notification_id)
                # else:
                self.emit('ack', {'status': client_task_response[0], 'message': client_task_response[1],
                                  'notif_id': notification_id, 'status_code': client_task_response[2]})
                logger.debug(
                    "Response code " + str(client_task_response[2]) + " is sent to tronX for " + notification_id)
            # REMOVE:
            elif verb == 'remove' or verb == 'exclusion':
                # this change has to be verified from DM side
                if verb == 'remove':
                    if payload["force_remove"] == True:
                        command = "FORCE_REMOVE"
                    else:
                        command = "REMOVE"
                    # This change has to be verified from DM side.
                    remove_single = {
                        'client': 12,
                        'command': command,
                        'thing_id': payload["thing_id"],
                        'notif_id': notification_id
                    }
                else:
                    remove_single = {
                        'client': 12,
                        'command': "EXCLUSION",
                        'notif_id': notification_id
                    }
                client_task_response = WSClientTask().send_request(config.CONFIG.WS_CLIENT_SENT_PORT,
                                                                    json.dumps(remove_single), 1000,notification_id)
                logger.debug('Response from Client task {}'.format(client_task_response))
                # if 'success' in client_task_response:
                if client_task_response[2] == 200:
                    # Bug:19688-Start
                    # recv_status = WSClientTask().receive_response(config.CONFIG.WS_CLIENT_RECEIVE_PORT, 30 * 1000)
                    recv_status = WSClientTask().receive_response(config.CONFIG.WS_CLIENT_RECEIVE_PORT, 45 * 1000, notification_id)
                    # Bug:19688-End
                    logger.debug('Response from Client task on remove operation {}'.format(recv_status))
                    self.emit('ack', {'status': recv_status[0], 'message': recv_status[1],
                                      'notif_id': notification_id, 'status_code': recv_status[2]})
                    logger.debug(
                        "response code " + str(recv_status[2]) + " is sent to tronX for " + notification_id)
                else:
                    self.emit('ack', {'status': client_task_response[0], 'message': client_task_response[1],
                                      'notif_id': notification_id, 'status_code': client_task_response[2]})
                    logger.debug(
                        "response code " + str(client_task_response[2]) + " is sent to tronX for " + notification_id)
            # REMOVE DISCOVERED THING
            # Feature:20285-Start
            elif verb == 'remove_discovered_thing':
                remove_single = {
                    'client': 12,
                    'command': 'REMOVE',
                    'thing_id': payload["thing_id"],
                    'notif_id': notification_id
                }
                client_task_response = WSClientTask().send_request(config.CONFIG.WS_CLIENT_SENT_PORT,
                                                                     json.dumps(remove_single), 1000, notification_id)
                logger.debug('Response from Client task {}'.format(client_task_response))
                # if 'success' in client_task_response:
                if client_task_response[2] == 200:
                    # Bug:19688-Start
                    # recv_status = WSClientTask().receive_response(config.CONFIG.WS_CLIENT_RECEIVE_PORT, 30 * 1000)
                    recv_status = WSClientTask().receive_response(config.CONFIG.WS_CLIENT_RECEIVE_PORT, 45 * 1000,notification_id)
                    # Bug:19688-End
                    logger.debug('Response from Client task on remove operation {}'.format(recv_status))
                    self.emit('ack', {'status': recv_status[0], 'message': recv_status[1],
                                      'notif_id': notification_id, 'status_code': recv_status[2]})
                    logger.debug("sent response code " + str(recv_status[2]) + " to tronX for " + notification_id)
                else:
                    self.emit('ack', {'status': client_task_response[0], 'message': client_task_response[1],
                                      'notif_id': notification_id, 'status_code': client_task_response[2]})
                    logger.debug(
                        "response code " + str(client_task_response[2]) + " is sent to tronX for " + notification_id)
            # RESET: IOT & FACTORY
            # Feature:20285-End
            elif verb == 'reset':
                if payload['subdomain'] == 'device':
                    reset = {
                        'client': '12',
                        'command': 'FACTORY_RESET',
                        'notif_id': notification_id
                    }
                    client_task_response = WSClientTask().send_request(config.CONFIG.WS_CLIENT_SENT_PORT,
                                                                        json.dumps(reset), 1000, notification_id)
                    logger.debug('Response from Client task {}'.format(client_task_response))
                    self.emit('ack', {'status': client_task_response[0], 'message': client_task_response[1],
                                      'notif_id': notification_id, 'status_code': client_task_response[2]})
                    logger.debug("sent response code " + str(client_task_response[2]) + " to tronX fpr " + notification_id)
                elif payload['subdomain'] == 'iot':
                    reset = {
                        'client': '12',
                        'command': 'IOT_RESET',
                        'notif_id': notification_id
                    }
                    client_task_response = WSClientTask().send_request(config.CONFIG.WS_CLIENT_SENT_PORT,
                                                                        json.dumps(reset), 1000, notification_id)
                    logger.debug('Response from Client task {}'.format(client_task_response))
                    logger.debug("sending " + str(client_task_response[2]) + " to tronX")
                    self.emit('ack', {'status': client_task_response[0], 'message': client_task_response[1],
                                      'notif_id': notification_id, 'status_code': client_task_response[2]})
                    logger.debug(
                        "sent response code " + str(client_task_response[2]) + " to tronX fpr " + notification_id)
                else:
                    logger.debug("sent response code 400 to tronX for " + notification_id)
                    self.emit('ack', {'status': 'error', 'message': 'This reset is not supported by the Hub',
                                      'notif_id': notification_id, 'status_code': 400})
            # Feature-19706 Start
            # To support the S2SECURITY supported ZWAVE devices.
            elif verb == 'S2Security':
                data = {
                    'client': '12',
                    'command': 'S2_SECURITY',
                    'data': payload["body"],
                    'notif_id': notification_id
                }
                client_task_response = WSClientTask().send_request(config.CONFIG.WS_CLIENT_SENT_PORT,
                                                                    json.dumps(data), 1000, notification_id)
                logger.debug('Response from Client task {}'.format(client_task_response))
                self.emit('ack', {'status': client_task_response[0], 'message': client_task_response[1],
                                  'notif_id': notification_id, 'status_code': client_task_response[2]})
                logger.debug("sent response code " + str(client_task_response[2]) + " to tronX for " + notification_id)
            # SCENE
            # Feature-19706: End
            elif payload['subdomain'] == 'scene' or payload['subdomain'] == 'voice_scene':
                # SCENE-EDIT
                if verb == 'modify':
                    scene_modify = {
                        'client': '12',
                        'command': payload['subdomain'].upper(),
                        'verb': 'MODIFY',
                        'notif_id': notification_id,
                        'scene_data': payload["scene_data"]
                    }
                    client_task_response = WSClientTask().send_request(config.CONFIG.WS_CLIENT_SENT_PORT,
                                                                        json.dumps(scene_modify), 1000, notification_id)
                    logger.debug('Response from Client task {}'.format(client_task_response))
                    self.emit('ack', {'status': client_task_response[0], 'message': client_task_response[1],
                                      'notif_id': notification_id, 'status_code': client_task_response[2]})
                    logger.debug("sent response code " + str(client_task_response[2]) + " to tronX for " + notification_id)
                # SCENE - EXECUTE
                elif verb == 'run':
                    scene_run = {
                        'client': '12',
                        'command': payload['subdomain'].upper(),
                        'verb': 'RUN',
                        'notif_id': payload["notif_id"],
                        'scene_data': payload["scene_data"]
                    }
                    client_task_response = WSClientTask().send_request(config.CONFIG.WS_CLIENT_SENT_PORT,
                                                                        json.dumps(scene_run), 1000, notification_id)
                    logger.debug('Response from Client task {}'.format(client_task_response))
                    self.emit('ack', {'status': client_task_response[0], 'message': client_task_response[1],
                                      'notif_id': notification_id, 'status_code': client_task_response[2]})
                    logger.debug("sent response code " + str(client_task_response[2]) + " to tronX for " + notification_id)
                # SCENE - DELETE
                elif verb == 'delete':
                    scene_delete = {
                        'client': '12',
                        'command': payload['subdomain'].upper(),
                        'verb': 'DELETE',
                        'notif_id': payload["notif_id"],
                        'scene_data': payload["scene_data"]
                    }
                    client_task_response = WSClientTask().send_request(config.CONFIG.WS_CLIENT_SENT_PORT,
                                                                        json.dumps(scene_delete), 1000, notification_id)
                    logger.debug('Response from Client task {}'.format(client_task_response))
                    self.emit('ack', {'status': client_task_response[0], 'message': client_task_response[1],
                                      'notif_id': notification_id, 'status_code': client_task_response[2]})
                    logger.debug(
                        "sent response code " + str(client_task_response[2]) + " to tronX for " + notification_id)
                else:
                    logger.debug("sent response code 400 to tronX for " + notification_id)
                    self.emit('ack', {'status': 'error', 'message': 'This scene mode is not supported by the Hub',
                                      'notif_id': notification_id, 'status_code': 400})
            # RULES
            elif payload['subdomain'] == 'rules':
                # RULE-CREATE
                if verb == 'create':
                    rules_create = {
                        'client': '12',
                        'command': 'RULE',
                        'verb': 'CREATE',
                        'notif_id': payload["notif_id"],
                        'rules_data': payload["rules_data"]
                    }
                    client_task_response = WSClientTask().send_request(config.CONFIG.WS_CLIENT_SENT_PORT,
                                                                        json.dumps(rules_create), 1000, notification_id)
                    logger.debug('Response from Client task {}'.format(client_task_response))
                    self.emit('ack', {'status': client_task_response[0], 'message': client_task_response[1],
                                      'notif_id': notification_id, 'status_code': client_task_response[2]})
                    logger.debug(
                        "sent response code " + str(client_task_response[2]) + " to tronX for " + notification_id)
                # RULE - EDIT
                elif verb == 'modify':
                    rules_modify = {
                        'client': '12',
                        'command': 'RULE',
                        'verb': 'MODIFY',
                        'notif_id': payload["notif_id"],
                        'rules_data': payload["rules_data"]
                    }
                    client_task_response = WSClientTask().send_request(config.CONFIG.WS_CLIENT_SENT_PORT,
                                                                        json.dumps(rules_modify), 1000, notification_id)
                    logger.debug('Response from Client task {}'.format(client_task_response))
                    self.emit('ack', {'status': client_task_response[0], 'message': client_task_response[1],
                                      'notif_id': notification_id, 'status_code': client_task_response[2]})
                    logger.debug(
                        "sent response code " + str(client_task_response[2]) + " to tronX for " + notification_id)
                # RULE -DELETE
                elif verb == 'delete':
                    rules_delete = {
                        'client': '12',
                        'command': 'RULE',
                        'verb': 'DELETE',
                        'notif_id': payload["notif_id"],
                        'rules_data': payload["rules_data"]
                    }
                    client_task_response = WSClientTask().send_request(config.CONFIG.WS_CLIENT_SENT_PORT,
                                                                        json.dumps(rules_delete), 1000, notification_id)
                    logger.debug('Response from Client task {}'.format(client_task_response))
                    self.emit('ack', {'status': client_task_response[0], 'message': client_task_response[1],
                                      'notif_id': notification_id, 'status_code': client_task_response[2]})
                    logger.debug(
                        "sent response code " + str(client_task_response[2]) + " to tronX for " + notification_id)
                else:
                    logger.debug("sent response code 400 to tronX for " + notification_id)
                    self.emit('ack', {'status': 'error', 'message': 'This rule mode is not supported by the Hub',
                                      'notif_id': notification_id, 'status_code': 400})
            elif verb == "get_group":
                self.emit('ack',
                          {"status": "success", "message": "message dropped from HUB", "notif_id": payload["notif_id"]})
            elif verb == "rename_group":
                self.emit('ack',
                          {"status": "success", "message": "message dropped from HUB", "notif_id": payload["notif_id"]})
            elif verb == "group_create":
                self.emit('ack', {"status": "success", "message": "message dropped from HUB",
                                  "notif_id": payload["notif_id"]})
            # ALARMS
            elif payload['subdomain'] == "alarm":
                alarm_payload = {
                    "client_name": "WS",
                    "client_action": "ALARM",
                    "client_data": payload
                }
                client_task_response = WSClientTask().send_request(8800, json.dumps(alarm_payload), 5 * 1000, notification_id)
                logger.debug('Response from Client task {}'.format(client_task_response))
                self.emit('ack', {'status': client_task_response[0], 'message': client_task_response[1],
                                  'notif_id': notification_id, 'status_code': client_task_response[2]})
                logger.debug("sent response code " + str(client_task_response[2]) + " to tronX for " + notification_id)
            # REMINDERS
            elif payload['subdomain'] == "reminder":
                reminder_payload = {
                    "client_name": "WS",
                    "client_action": "REMINDER",
                    "client_data": payload
                }
                client_task_response = WSClientTask().send_request(8800, json.dumps(reminder_payload), 5 * 1000, notification_id)
                logger.debug('Response from Client task {}'.format(client_task_response))
                self.emit('ack', {'status': client_task_response[0], 'message': client_task_response[1],
                                  'notif_id': notification_id, 'status_code': client_task_response[2]})
                logger.debug("sent response code " + str(client_task_response[2]) + " to tronX for " + notification_id)

        # #FOTA CHECK
        #     elif payload["subdomain"]== "fota" and verb == "check":
        #         logger.debug('fota check API is called')
        #         msg = {
        #                "client":12,
        #                "body":{
        #                   "domain":"SHIFU",
        #                   "subdomain":"FOTA",
        #                   "verb":"check"
        #                }
        #             }
        #         response = fota_socket(msg,45)
        #         logger.debug('Response from Fota server is {}'.format(response))
        #         if response['body']['message'] == 'Timeout.Please try after sometime':
        #             response = {
        #                 "status": "success",
        #                 "body": {"update": False,"release_version": "","release_date": "","message": "Update is not available","change_log": ""},                    "notif_id": notification_id,
        #                 "status_code": 200
        #             }
        #
        #         logger.debug('Response to tronx is:{}'.format(response))
        #         self.emit('ack', {'status': response['status'], 'message': str(response['body']),
        #                               'notif_id': notification_id, 'status_code': response['status_code']})
        #         logger.debug("sent response code " + str(response['status_code']) + " to tronX for " + notification_id)
        #
        #
        #     #FOTA UPDATE
        #     elif payload["subdomain"] == "fota" and verb == "update":
        #         logger.debug('fota update API is called')
        #         msg = {
        #             "client": 12,
        #             "body": {
        #                 "domain": "SHIFU",
        #                 "subdomain": "FOTA",
        #                 "verb": "update"
        #             }
        #         }
        #         response = fota_socket(msg,15)
        #         logger.debug('Response from Fota server is {}'.format(response))
        #         self.emit('ack', {'status': response['status'], 'message': str(response['body']),
        #                           'notif_id': notification_id, 'status_code': response['status_code']})
        #         logger.debug("sent response code " + str(response['status_code']) + " to tronX for " + notification_id)

            #FOTA CHECK
            elif payload["subdomain"] == "fota" and verb == "check":
                logger.debug('fota check API is called')
                msg = {
                    "client": 12,
                    "body": {
                        "domain": "SHIFU",
                        "subdomain": "FOTA",
                        "verb": "check"
                    }
                }
                response = fota_socket(msg, 5)
                logger.debug('Response from Fota server is {}'.format(response))
                if response['status'] == "success":
                    self.emit('ack', {'status': response['status'], 'message': json.dumps(response['body']),
                                      'notif_id': notification_id, 'status_code': response['status_code']})
                else:
                    self.emit('ack', {'status': response['status'], 'message': str(response['body']['message']),
                                      'notif_id': notification_id, 'status_code': response['status_code']})
                logger.debug('Response to tronx is:{}'.format(response))
                logger.debug("sent response code " + str(response['status_code']) + " to tronX for " + notification_id)

            # FOTA UPDATE
            elif payload["subdomain"] == "fota" and verb == "update":
                logger.debug('fota update API is called')
                logger.debug("Received update request for version: {}".format(payload["payload"]["version"]))
                msg = {
                    "client": 12,
                    "body": {
                        "domain": "SHIFU",
                        "subdomain": "FOTA",
                        "verb": "update",
                        "version": str(payload["payload"]["version"])
                    }
                }
                response = fota_socket(msg, 10)
                logger.debug('Response from Fota server is {}'.format(response))
                if response['status'] == "success":
                    self.emit('ack', {'status': response['status'], 'message': json.dumps(response['body']),
                                      'notif_id': notification_id, 'status_code': response['status_code']})
                else:
                    self.emit('ack', {'status': response['status'], 'message': str(response['body']['message']),
                                      'notif_id': notification_id, 'status_code': response['status_code']})
                logger.debug('Response to tronx is:{}'.format(response))
                logger.debug("sent response code " + str(response['status_code']) + " to tronX for " + notification_id)



        # ARM DISARM Feature
            elif verb == 'update' and payload['subdomain'] in ['configuration' , 'multiple' ,'single']:
                arm_disarm = {
                    'client': 12,
                    'command': 'SECURITY',
                    'verb': 'UPDATE',
                    'notif_id': notification_id,
                    'payload': payload
                }
                client_task_response = WSClientTask().send_request(config.CONFIG.WS_CLIENT_SENT_PORT,
                                                                   json.dumps(arm_disarm), 1000, notification_id)
                logger.debug('Response from Client task {}'.format(client_task_response))
                # if 'success' in client_task_response:
                if client_task_response[2] == 200:
                    # Bug:19688-Start
                    # recv_status = WSClientTask().receive_response(config.CONFIG.WS_CLIENT_RECEIVE_PORT, 7000)
                    recv_status = WSClientTask().receive_response(config.CONFIG.WS_CLIENT_RECEIVE_PORT, 8000,
                                                                  notification_id)
                    # Bug:19688-End
                    logger.debug('Response from Client task on control operation {}'.format(recv_status))
                    self.emit('ack', {'status': recv_status[0], 'message': recv_status[1],
                                      'notif_id': notification_id, 'status_code': recv_status[2]})
                    logger.debug("response code " + str(recv_status[2]) + " is sent to tronX for " + notification_id)
                else:
                    self.emit('ack', {'status': client_task_response[0], 'message': client_task_response[1],
                                      'notif_id': notification_id, 'status_code': client_task_response[2]})
                    logger.debug(
                        "response code " + str(client_task_response[2]) + " is sent to tronX for " + notification_id)

            # PROFILE UPDATES
            elif verb == "update":
                tx_data = payload
                logger.debug("updating the local device_profile")
                # logger.debug("tx_data is {}".format(tx_data))
                # Feature17524: Start
                try:
                    with open("/etc/profile.json", mode='r+') as f:
                        data = json.load(f)
                        # logger.debug("content in profile.json file is {}".format(data))
                        device_name = tx_data['message']['devices']['device_name']
                        smartdevicenotification = tx_data['message']['iot_settings']['SmartdeviceNotification']

                        # changing device name/SmartdeviceNotification in profile.json file
                        data['devices']['device_name'] = tx_data['message']['devices']['device_name']
                        data['devices']['has_internet'] = tx_data['message']['devices']['has_internet']
                        data['iot_settings']['SmartdeviceNotification']  = tx_data['message']['iot_settings']['SmartdeviceNotification']
                        f.seek(0)  # rewind
                        json.dump(data, f)
                        f.truncate()
                        f.close()
                    logger.debug("profile changes are written to profile.json file successfully")
                    # Feature 22289:Start
                    try:
                        user_pref = tx_data['message']['devices']['has_internet']
                        logger.debug("User preference for enabling LED is: {}".format(user_pref))
                        if user_pref=='0':
                            web_socket_led.send_msg("LED_NO_INTERNET_CHECK_NOTI", 0)
                            os.system("uci set shifu.conf.INTERNET_MOBI_NOTIFICATION='0' && uci commit shifu")
                        elif user_pref=='1':
                            web_socket_led.send_msg("LED_NO_INTERNET_CHECK_NOTI", 1)
                            os.system("uci set shifu.conf.INTERNET_MOBI_NOTIFICATION='1' && uci commit shifu")
                    except Exception as e:
                        logger.debug("Unexpected error while sending message to LED server: {}".format(e))
                    # Feature 22289:End
                    # updating DEVICE_NAME flag in uci shifu file
                    os.system("uci set shifu.conf.DEVICE_NAME='" + device_name + "' && uci commit shifu")
                    os.system("uci set shifu.conf.MA_SmartdeviceNotification='" + str(smartdevicenotification).lower() + "' && uci commit shifu")
                    self.emit('ack', {'status': 'success', 'message': 'Profile is updated',
                                      'notif_id': notification_id, 'status_code': 200})
                    logger.debug("sent response code 200 to tronX for " + notification_id)
                except Exception as e:
                    logger.debug("Error is: {}".format(e))
                    self.emit('ack', {'status': 'error', 'message': 'Operation failed',
                                      'notif_id': notification_id, 'status_code': 500})
                    logger.debug("sent response code 500 to tronX for " + notification_id)
                # Feature17524: End

        # LWA HANDLING
            elif verb == "lwa_login":
                notif_id = 12345
                tx_data = payload
                logger.debug(payload)
                logger.debug("lwa_login is called")

                # Reading ALEXA_REGISTRATION_STATUS env
                # try:
                #     alexa_status_value = config.CONSTANT.alexa_registration_status
                #     logger.debug("Alexa registration status is %s", alexa_status_value)
                # except Exception, e:
                #     logger.critical(
                #         'Unexpected Error or not able to access the ALEXA_REGISTRATION_STATUS system variable', exc_info=True)
                #     return jsonify({'status': 1, 'message': "Shifu Interneal Error"}), 500
                try:
                    alexa_status_value = subprocess.check_output(["uci", "get", "shifu.conf.ALEXA_REGISTRATION_STATUS"]).strip()
                except Exception as e:
                    logger.critical('Unexpected Error or NV Variable ALEXA_REGISTRATION_STATUS is not present', exc_info=True)
                    self.emit('ack', {'status': 'error', 'message': 'Shifu Internal Error','notif_id': notification_id, 'status_code': 500})
                    logger.debug("sent response code 500 to tronX for " + notification_id)

                # if registered sending fail to app
                if alexa_status_value == 'registered':
                    logger.debug(" Shifu is already registered with Alexa")
                    logger.debug("responding back for lwa_login request")
                    self.emit('ack',
                              {'status': 'error', 'message': 't.hub is already registered with Alexa', 'notif_id': notification_id,
                               'status_code': 400})
                    logger.debug("sent response code 400 to tronX for " + notification_id)
                else:
                    grant_type = str(tx_data["payload"]['grant_type'])
                    client_id = str(tx_data["payload"]['client_id'])
                    redirect_uri = str(tx_data["payload"]['redirect_uri'])
                    code = str(tx_data["payload"]['code'])
                    code_verifier = str(tx_data["payload"]['code_verifier'])
                    if client_id == '' or redirect_uri == '' or code == '' or code_verifier == '' or grant_type == '':
                        logger.debug("Parameters are missing")
                        #return jsonify({'status': 'failure', 'message': "Parameters are required to Login"}), 400
                        self.emit('ack',
                                  {'status': 'error', 'message': 'Parameters required for Login are missing',
                                   'notif_id': notification_id,
                                   'status_code': 400})
                        logger.debug("sent response code 400 to tronX for " + notification_id)
                    va_msg = {"client_name": "MA",
                              "client_action": "REGISTRATION",
                              "notif_id": notif_id,
                              "client_data":
                                  {"amz_client_id": client_id,
                                   "amz_auth_code": code,
                                   "amz_redirect_uri": redirect_uri,
                                   "amz_code_verifier": code_verifier}
                              }
                    logger.debug('va_msg is {}'.format(va_msg))
                    response = va_socket(str(va_msg), 4640, 50)  # port=4640 and timeout = 50s for lwa
                    logger.debug('Response from va_socket is: {}'.format(response))
                    # response = 0
                    if response == 1:
                        logger.debug("responding back for lwa_login request : LWA login failed")
                        web_socket_led.send_msg("LED_ERROR", 1)  # Need to be implemented
                        #return jsonify({'status': response, 'message': "Failure"}), 400
                        self.emit('ack',
                                  {'status':'failure', 'message': 'Failure','notif_id': notification_id,
                                   'status_code': 400})
                        logger.debug("sent response code 400 to tronX for " + notification_id)
                    else:  # va response success
                        # updating env
                        try:
                            os.system("uci set shifu.conf.ALEXA_REGISTRATION_STATUS=registered && uci commit shifu")
                            os.system("uci set shifu.conf.SKIP_ALEXA_REGISTRATION=false && uci commit shifu")
                        except Exception, e:
                            logger.critical('Not able to set the NV variable: alexa_registration_stauts', exc_info=True)
                            #return jsonify({'status': 1,'message': "Unable to store Alexa registration env. Please try after some time"}), 400
                            self.emit('ack',{'status': 1, 'message': 'Unable to store Alexa registration env. Please try after some time', 'notif_id': notification_id,'status_code': 400})
                            logger.debug("sent response code 400 to tronX for " + notification_id)
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
                            #return jsonify({'status': 1,'message': "Unable to store Alexa registration status locally in hub"}), 400
                            self.emit('ack', {'status': 'error',
                                              'message': 'Unable to store Alexa registration status locally in hub',
                                              'notif_id': notification_id, 'status_code': 400})
                        logger.debug('Alexa reg status is changed successfully')

                        #reload(config)

                        # os.system('/etc/init.d/ma_bootup start')
                        os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
                        os.system("kill -SIGUSR1 `ps | grep Mobile_Agent | grep -v grep | awk '{print $1}'`")
                        logger.debug("responding back for lwa_login API call")
                        web_socket_led.send_msg("LED_SETUP_MODE", 0)
                        #return jsonify({'status': response, 'message': "Success"}), 200
                        self.emit('ack', {'status': 'success',
                                          'message': 'Success',
                                          'notif_id': notification_id, 'status_code': 200})
                        logger.debug("responding back to tronx for successfull LWA login")

            elif verb == "skip_lwa_login":

                notif_id = 12345
                logger.debug(" skip_lwa_login  is called")

                # Reading SKIP_ALEXA_REGISTRATION env
                try:
                    #skip_alexa_status_value = config.CONSTANT.skip_alexa_registration_status
                    skip_alexa_status_value = subprocess.check_output(["uci", "get", "shifu.conf.SKIP_ALEXA_REGISTRATION"]).strip()
                    logger.debug("Skip Alexa registration status is %s", skip_alexa_status_value)
                except Exception as e:
                    logger.critical(
                        'Unexpected Error or not able to access the SKIP_ALEXA_REGISTRATION system variable',
                        exc_info=True)
                    #return jsonify({'status': 1, 'message': "Shifu Interneal Error"}), 500
                    time.sleep(2)
                    self.emit('ack', {'status': 'error',
                                      'message': 'Shifu Internal Error',
                                      'notif_id': notification_id, 'status_code': 500})
                # if registered sending fail to app
                if skip_alexa_status_value == 'true':
                    logger.debug("Skip alexa registration is already true")
                    logger.debug("/auth/skip_lwa_login API is responding back")
                    #return jsonify({'status': 1, 'message': "lwa login is already skipped"}), 400
                    time.sleep(2)
                    self.emit('ack', {'status': 'error',
                                      'message': 'lwa login is already skipped',
                                      'notif_id': notification_id, 'status_code': 400})
                else:
                    # updating env
                    try:
                        os.system("uci set shifu.conf.SKIP_ALEXA_REGISTRATION=true && uci commit shifu")
                    except Exception as e:
                        logger.critical('Not able to set the NV variable: skip alexa_registration', exc_info=True)
                        #return jsonify({'status': 1,
                        #                'message': "Unable to store skip Alexa registration env. Please try after some time"}), 400
                        time.sleep(2)
                        self.emit('ack', {'status': 'error',
                                          'message': 'Unable to store skip Alexa registration env. Please try after some time',
                                          'notif_id': notification_id, 'status_code': 400})
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
                        #return jsonify({'status': 1,
                        #                'message': "Unable to store skip Alexa registration status locally in hub"}), 400
                        time.sleep(2)
                        self.emit('ack', {'status': 'error',
                                          'message': 'Unable to store skip Alexa registration status locally in hub',
                                          'notif_id': notification_id, 'status_code': 400})

                    logger.debug('Skip Alexa reg status is changed successfully')
                    #reload(config)
                    # os.system('/etc/init.d/ma_bootup start')
                    os.system("uci set shifu.conf.MA_PENDING_FLAG=1 && uci commit shifu")
                    os.system("kill -SIGUSR1 `ps | grep Mobile_Agent | grep -v grep | awk '{print $1}'`")
                    logger.debug("/auth/skip_lwa_login API is responding back")
                    web_socket_led.send_msg("LED_SETUP_MODE", 0)
                    logger.debug('stopping led after skip alexa')
                    #return jsonify({'status': 0, 'message': "Success"}), 200
                    time.sleep(2)
                    self.emit('ack', {'status': 'success',
                                      'message': 'Success',
                                      'notif_id': notification_id, 'status_code': 200})
            else:
                logger.debug(notification_id + " is dropped from Hub")
                time.sleep(2)
                self.emit('ack', {"status": "success", "message": "message dropped from HUB",
                                  "notif_id": notification_id, 'status_code':510})
                logger.debug("sent response code 510 to tronX for " + notification_id)
        command_status = False
        logger.debug("Handle Command went to sleep")

    def on_connect(self):
        """
        :return: socket io client on establishing connection with tronX socket io server
        """
        logger.debug('Socket io client is connected with tronX socket io server.')

    def on_disconnect(self):
        """
        :return: socket io client on disconnected with tronX socket io server
        """
        logger.debug('Socket io client is disconnected with tronX socket io server.')

    def on_reconnect(self):
        """
        :return: In case disconnection due to such as internet failure in after successfull connection or
        disconnection with tronX socket io server,
        then after reconnection with tronX socket io server.
        """
        logger.debug('Socket io client is re-connected to tronX socket io server due to some kind of interruption\
        in communication or connection.')
        # Bug-19702:Start
        try:
            os.system("kill -SIGUSR1 `ps | grep Mobile_Agent | grep -v grep | awk '{print $1}'`")
        except Exception as e:
            logger.debug("Error {}".format(e))
            logger.error("notable to trigger the nework signal")
        # Bug-19702:End

    def on_notification(self, payload):
        """
        :param payload: data from tronX socket io server
        :return: Adding payload in queue with some priority as 10.
        Acknowledgement to the tronX socket server of received data as payload and notification-id.
        """
        logger.debug('New Socket io job from tronX {}'.format(payload))
        logger.debug('Size of Payload is {}'.format(sys.getsizeof(payload)))
        logger.debug("payload putting into Queue")
        queue.put_nowait(payload)
        if not command_status:
            thread.start_new_thread(self.handle_command, ())


class SocketIOClient:
    """
    Socket io client class
    """

    def __init__(self):
        self.HOME_TOKEN = None

    def socket_io(self, ):
        """
        call to tronX socket io server for connection with socket io client
        :return:
        """
        logger.debug("Base Url to connect with tronX {}".format(config.CONFIG.HOST_NAME))
        #self.HOME_TOKEN = config.CONFIG.HOME_TOKEN
        self.HOME_TOKEN = subprocess.check_output(["uci", "get", "shifu.conf.HOME_TOKEN"]).strip()
        logger.debug("Home token from shifu device environment is {}".format(self.HOME_TOKEN))
        if self.HOME_TOKEN != "NULL":
            try:
                logger.debug("calling to tronX socket io server for connection with socket io client")
                socket_io_client = SocketIO(
                    config.CONFIG.HOST_NAME,  # host name
                    config.CONFIG.LOCAL_PORT_NUMBER,  # local listening port number
                    namespaces.SocketIONamespace,
                    False,  # Block until connection is established
                    ("websocket",),
                    config.CONFIG.SOCKET_IO_RESOURCE.strip('/'),  # socket.io resource
                    params={'access_token': self.HOME_TOKEN},
                    verify=False,
                )
                logger.critical("Connected with tronX socket io server {}".format(socket_io_client))
                socket_io_client.define(ShifuNamespace, '/shifu')
                logger.debug(
                    "tronX socket io server connection uses {} transport ".format(socket_io_client.transport_name))
                logger.debug("Calling to socket io client to wait")
                socket_io_client.wait()
                logger.debug("socket io client connection is closed from tronX socket io server.")
            except ConnectionError:
                time.sleep(2)
                logger.debug("Connection Error")
                logger.debug('The server is down. Retrying connection.')
                socket_io_client = SocketIOClient()
                socket_io_client.socket_io()
            except Exception as e:
                time.sleep(2)
                logger.critical("Error to keep connection alive closing connection with tronx server")
                logger.error("Error {}".format(e))
                while True:
                    Internet_status = subprocess.check_output(["uci", "get", "shifu.conf.INTERNET_STATUS"]).strip()
                    if Internet_status == '1' :
                        break
                    else:
                        logger.debug("no internet")
                        time.sleep(1)
                        continue
                logger.debug("Retrying connection")
                socket_io_client = SocketIOClient()
                socket_io_client.socket_io()
        else:
            logger.debug("waiting for 2 sec and then check")
            time.sleep(2)
            while True:
                HOME_TOKEN_temp = subprocess.check_output(["uci", "get", "shifu.conf.HOME_TOKEN"]).strip()
                logger.debug("Home token from shifu device environment is {}".format(HOME_TOKEN_temp))
                Internet_status_temp = subprocess.check_output(["uci", "get", "shifu.conf.INTERNET_STATUS"]).strip()
                if (HOME_TOKEN_temp != "NULL" and Internet_status_temp == "1" ):
                    #logger.debug("web socket is connecting to Tronx")
                    break
                else:
                    #logger.debug("web socket is not connecting as either internet is not there or token missing" )
                    time.sleep(2)
                    continue
            socket_io_client = SocketIOClient()
            socket_io_client.socket_io()
            pass
            #sys.exit(0)

class SocketIoClientThread(threading.Thread):
    """
    socket io client thread class
    """

    def __init__(self):
        """
        """
        threading.Thread.__init__(self)

    def run(self):
        """
        running SocketIO-Client thread to connect with tronX socket io server
        :return:
        """
        logger.debug("Running socket io client thread.")
        socket_io_client_thread = SocketIOClient()
        socket_io_client_thread.socket_io()
        logger.debug("socket io client thread closed")

# API to inform whether internet connection is there or not
def ws_ping_test():
    """
    web socket ping test
    """
    net_connection_prv_state = True
    net_connection_cur_state = True
    while(1):
        time.sleep(60)
        out = subprocess.Popen("ping 8.8.8.8 -c4", stdout=subprocess.PIPE, shell=True)
        Output = (out.communicate())
        Output = str(Output)
        Output1 = Output.replace('\n', " ")
        try:
            if (re.search("[1-9] packets received", Output1, re.IGNORECASE)):
                net_connection_cur_state = True
                logger.debug("Internet is CONNECTED")
            else:
                net_connection_prv_state = False
                net_connection_cur_state = False
                logger.debug("Internet is DISCONNECTED")
            # logger.debug("net_connection previous state is {}".format(net_connection_prv_state))
            # logger.debug("net_connection current state is {}".format(net_connection_cur_state))
            if ((not net_connection_prv_state) and (net_connection_cur_state)):
                logger.debug("restarting WS_SERVICE")
                os.system("/etc/init.d/ws_service restart")
            else:
                pass
                # logger.debug("Not restarting WS_SERVICE")
        except Exception as e:
            logger.debug("IN exception ws ping test {}".format(e))

def start_service():
    logger.info('Socket service is starting')
    global web_socket_led
    web_socket_led = LED.LedControl(7891)
    web_socket_led.connect()
    ss = SocketIoClientThread()
    ss.start()
    # t = threading.Thread(name = "ws_ping_test",target=ws_ping_test)
    # t.start()