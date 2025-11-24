import zmq
import sys
import time
import json
context = zmq.Context()

sock = context.socket(zmq.REQ)
sock.connect("tcp://localhost:7006")


while True:         
	json_data = {"status": "success", "status_code": 200, "message": "operation successfull", "message_id": "11600300"}
	sock.send(json.dumps(json_data))
	print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
	print json_data
	message = sock.recv()
	print "MOB1_RECEIVED_FROM_DEVICE_MANAGER_SERVER :"
	print message

