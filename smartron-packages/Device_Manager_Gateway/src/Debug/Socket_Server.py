#!/usr/bin/python

__author__ = "Hemant Jiwani"
__copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
__credits__ = ["Hemant Jiwani"]
__maintainer__ = "Hemant Jiwani"
__email__ = "hemant.jiwani@smartron.com"
__status__ = "Testing"


import json
import sys
import ast
import Queue
import thread
import subprocess
from requests.exceptions import ConnectionError

from socketIO_client import SocketIO, namespaces, BaseNamespace

class CONSTANT(object):
    HOST_NAME = subprocess.check_output(["uci", "get", "shifu.conf.base_url"]).strip()
    LOCAL_PORT_NUMBER = ''
    HM_CLIENT_SENT_PORT = 7001
    HM_CLIENT_RECEIVE_PORT = 7006
    SOCKET_IO_RESOURCE = "/hub/socket.io"
    HOME_TOKEN = subprocess.check_output(["uci", "get", "shifu.conf.HOME_TOKEN"]).strip()

    def __setattr__(self, *_):
        raise AttributeError("These Value are constant and can not changed")

CONFIG = CONSTANT()

try:
    import zmq
except ImportError:
    print('zmq Python package not found in system application closing')
    sys.exit()
class HMClientTask:
    """
    Home Manager Client Class
    """

    def __init__(self):
        """
        """
        self.messageId = None
        self.context = zmq.Context()

    def sent_response(self, port, msg, timeout):
        """
        :param port: port number on which home manager server is running
        :param msg: message or payload from tronX socket io server
        :return: ack message from home manager server
        """
        socket = self.context.socket(zmq.REQ)
        socket.setsockopt(zmq.LINGER, 0)
        socket.connect("tcp://127.0.0.1:{}".format(port))
        socket.send(msg)
        poller = zmq.Poller()
        poller.register(socket, zmq.POLLIN)
        if poller.poll(timeout):  # 1s timeout in per milliseconds
            msg = socket.recv()
            msg = ast.literal_eval(msg)
            if msg['status_code'] != 200:
                return 'error', msg['message']
            else:
                return 'success', 'controlled successfully done'
        else:
            socket.close()
            return 'error', 'TimeoutError'

    def receive_response(self, port, timeout):
        socket = self.context.socket(zmq.REP)
        socket.setsockopt(zmq.LINGER, 0)
        socket.bind("tcp://127.0.0.1:{}".format(port))
        poller = zmq.Poller()
        poller.register(socket, zmq.POLLIN)
        if poller.poll(timeout):  # 5s timeout in milliseconds
            msg = socket.recv()
            socket.send('received')
            socket.close()
            print(msg)
            msg = ast.literal_eval(msg)
            if msg['status_code'] != 200:
                return 'error', msg['message']
            else:
                return 'success', 'controlled successfully done'
        else:
            socket.close()
            return 'error', 'TimeoutError'


queue = Queue.Queue(maxsize=100)
command_status = False


class ShifuNamespace(BaseNamespace):
    """
    shifu Namespace class
    """

    def handle_command(self):
        print("handle command started")
        command_status = True
        while queue._qsize() > 0:
            payload = queue.get()
            if payload['verb'] == "things_discover":
                print("call to discover API to discover things.")
                discovery = {
                    'client': 12,
                    'command': 'DISCOVERY'
                }
                client_task_response = HMClientTask().sent_response(CONFIG.HM_CLIENT_SENT_PORT,
                                                                    json.dumps(discovery), 1000)
                print('Response from Client task {}'.format(client_task_response))
                if 'success' in client_task_response:
                    self.emit('ack', {'status': 'success', 'message': 'Operation performed.',
                                      'notif_id': payload["notif_id"]}, payload)
                else:
                    self.emit('ack', {'status': 'error', 'message': 'Operation failed',
                                      'notif_id': payload["notif_id"]}, payload)
            elif payload['verb'] == "control":
		control_single = {
			'client': 12,
			'command': 'CONTROL',
			'things': payload["things"]
			}
                notification_id = payload["notif_id"]
                client_task_response = HMClientTask().sent_response(CONFIG.HM_CLIENT_SENT_PORT,
                                                                    json.dumps(control_single), 1000)
                print('Response from Client task {}'.format(client_task_response))
                if 'success' in client_task_response:
                    recv_status = HMClientTask().receive_response(CONFIG.HM_CLIENT_RECEIVE_PORT, 10*1000)
                    print('Response from Client task on control operation {}'.format(recv_status))
                    if 'error' in recv_status:
                        self.emit('ack', {'status': 'error', 'message': 'Operation failed',
                                          'notif_id': notification_id})
                    else:
                        self.emit('ack', {'status': 'success', 'message': 'Operation performed.',
                                          'notif_id': notification_id})
                else:
                    self.emit('ack', {'status': 'error', 'message': 'Operation failed',
                                      'notif_id': notification_id})
            elif payload['verb'] == 'remove':
                remove_single = {
                        'client': 12,
                        'command': 'REMOVE',
                        'thing_id': payload["thing_id"]
                }
                notification_id = payload["notif_id"]
                client_task_response = HMClientTask().sent_response(CONFIG.HM_CLIENT_SENT_PORT,
                                                                    json.dumps(remove_single), 1000)
                print('Response from Client task {}'.format(client_task_response))
                if 'success' in client_task_response:
                    recv_status = HMClientTask().receive_response(CONFIG.HM_CLIENT_RECEIVE_PORT, 30*000)
                    print('Response from Client task on control operation {}'.format(recv_status))
                    if 'error' in recv_status:
                        self.emit('ack', {'status': 'error', 'message': 'Operation failed',
                                          'notif_id': notification_id})
                    else:
                        self.emit('ack', {'status': 'success', 'message': 'Operation performed.',
                                          'notif_id': notification_id})
                else:
                    self.emit('ack', {'status': 'error', 'message': 'Operation failed',
                                      'notif_id': notification_id})
            elif payload['verb'] == "get_group":
                # FIXME: Perform task to get group
                self.emit('ack',
                          {"status": "success", "message": "message dropped from HUB", "notif_id": payload["notif_id"]})
            elif payload['verb'] == "rename_group":
                # FIXME: Perform task to rename group
                self.emit('ack',
                          {"status": "success", "message": "message dropped from HUB", "notif_id": payload["notif_id"]})
            elif payload['verb'] == "group_create":
                # FIXME: Perform task to create group
                self.emit('ack', {"status": "success", "message": "message dropped from HUB",
                                  "notif_id": payload["notif_id"]})
            elif payload['subdomain'] == "alarm":
                msg = "HM ALARM {}".format(json.dumps(payload))
                client_task_response = HMClientTask().sent_response(8802, msg, 2*1000)
                print('Response from Client task {}'.format(client_task_response))
                if 'success' in client_task_response:
                    self.emit('ack', {'status': 'success', 'message': 'Operation performed.',
                                      'notif_id': payload["notif_id"]}, payload)
                else:
                    self.emit('ack', {'status': 'error', 'message': 'Operation failed',
                                      'notif_id': payload["notif_id"]}, payload)
            elif payload['subdomain'] == "reminder":
                msg = "HM REMINDER {}".format(json.dumps(payload))
                client_task_response = HMClientTask().sent_response(8802, msg, 2*1000)
                print('Response from Client task {}'.format(client_task_response))
                if 'success' in client_task_response:
                    self.emit('ack', {'status': 'success', 'message': 'Operation performed.',
                                      'notif_id': payload["notif_id"]}, payload)
                else:
                    self.emit('ack', {'status': 'error', 'message': 'Operation failed',
                                      'notif_id': payload["notif_id"]}, payload)
            else:
                self.emit('ack', {"status": "success", "message": "message dropped from HUB",
                                  "notif_id": payload["notif_id"]})

        print('Send an acknowledgement to tronx of received notification successfully')
        command_status = False
        print("Handle Command went to sleep")

    def on_connect(self):
        """
        :return: socket io client on establishing connection with tronX socket io server
        """
        print('Socket io client is connected with tronX socket io server.')

    def on_disconnect(self):
        """
        :return: socket io client on disconnected with tronX socket io server
        """
        print('Socket io client is disconnected with tronX socket io server.')

    def on_reconnect(self):
        """
        :return: In case disconnection due to such as internet failure in after successfull connection or
        disconnection with tronX socket io server,
        then after reconnection with tronX socket io server.
        """
        print('Socket io client is re-connected to tronX socket io server due to some kind of interruption\
                     in communication or connection.')

    def on_notification(self, payload):
        """
        :param payload: data from tronX socket io server
        :return: Adding payload in queue with some priority as 10.
        Acknowledgement to the tronX socket server of received data as payload and notification-id.
        """
        print('New Socket io job from tronX {}'.format(payload))
        print("payload putting into Queue")
        queue.put_nowait(payload)
        if not command_status:
            thread.start_new_thread(self.handle_command, ())



class SocketIOClient:
    """
    Socket io client class
    """

    def __init__(self):
        self.HOME_TOKEN = None

    def socket_io(self):
        """
        call to tronX socket io server for connection with socket io client
        :return:
        """
        print("Getting home token from shifu device environment")
        self.HOME_TOKEN = CONFIG.HOME_TOKEN
        if self.HOME_TOKEN:
            self.HOME_TOKEN = CONFIG.HOME_TOKEN
            print("Home token from shifu device environment is {}".format(self.HOME_TOKEN))
        else:
            logger.error("No Home token in shifu device environment")
            sys.exit()
        print("calling to tronX socket io server for connection with socket io client")
        socket_io_client = SocketIO(
            CONFIG.HOST_NAME,  # host name
            CONFIG.LOCAL_PORT_NUMBER,  # local leasoning port number
            namespaces.SocketIONamespace,
            True,  # Block until connection is established
            ("websocket",),
            CONFIG.SOCKET_IO_RESOURCE.strip('/'),  # socket.io resource
            params={'access_token': self.HOME_TOKEN},
            # verify=False,
        )
        print("Connected with tronX socket io server {}".format(socket_io_client))
        print("Call to tronX socket io server /shifu")
        socket_io_client.define(ShifuNamespace, '/shifu')
        print("tronX socket io server connection uses {} transport ".format(socket_io_client.transport_name))
        try:
            print("Calling to socket io client to wait")
            socket_io_client.wait()
            print("socket io client connection is closed from tronX socket io server.")
        except ConnectionError:
            print('Not able to connect to socket server. Retrying connection.')
            socket_io_client = SocketIOClient()
            socket_io_client.socket_io()
        except Exception as e:
            print("Error to keep connection alive closing connection with tronx server")
            print("Retrying connection")
            socket_io_client = SocketIOClient()
            socket_io_client.socket_io()


def main():
    socket_io_client = SocketIOClient()
    socket_io_client.socket_io()
   


if __name__ == '__main__':
	main()



