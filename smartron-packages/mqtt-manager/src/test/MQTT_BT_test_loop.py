import zmq
import sys
import time
import json

context = zmq.Context()

socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:7003")

while True:
	

		things ={'client': "BT", "message_id": 1234, "client_data": { "tag": "Status","tag_data":[{"Status":"ON"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg

		time.sleep(10)
		things ={'client': "BT", "message_id": 1234, "client_data": { "tag": "Status","tag_data":[{"Status":"OFF"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg

		time.sleep(10)
		things ={'client': "BT", "message_id": 1234, "client_data": { "tag": "Device","tag_data":[{"mac_id": "12:31:f2:0a","device_name": "ABCD","status": "not_paired"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg

		time.sleep(10)
		things ={'client': "BT", "message_id": 1234, "client_data": { "tag": "Device","tag_data":[{"mac_id": "12:31:f2:0a","device_name": "ABCD","status": "not_paired"},{"mac_id": "32:31:fa:0a","device_name": "HONOR 10","status": "not_paired"},{"mac_id": "fa:3c:f2:1a","device_name": "ABCD","status": "not_paired"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg

		time.sleep(10)
		things ={'client': "BT", "message_id": 1234, "client_data": { "tag": "Device","tag_data":[{"mac_id": "12:31:f2:0a","device_name": "ABCD","status": "connected"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg

		things ={'client': "BT", "message_id": 1234, "client_data": { "tag": "Device","tag_data":[{"mac_id": "12:31:f2:0a","device_name": "ABCD","status": "paired"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg

		time.sleep(10)
		things ={'client': "BT", "message_id": 1234, "client_data": { "tag": "Device","tag_data":[{"mac_id": "12:31:f2:0a","device_name": "ABCD","status": "paired"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg

		time.sleep(10)
		things ={'client':"", "message_id": 1234, "client_data": { "tag": "Device","tag_data":[{"mac_id": "12:31:f2:0a","device_name": "ABCD","status": "paired"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg

		time.sleep(10)
		things ={'client': "BT_WRONG", "message_id": 1234, "client_data": { "tag": "Device","tag_data":[{"mac_id": "12:31:f2:0a","device_name": "ABCD","status": "paired"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg

		time.sleep(10)
		things ={'client': "BT", "message_id":"", "client_data": { "tag": "Device","tag_data":[{"mac_id": "12:31:f2:0a","device_name": "ABCD","status": "paired"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg
		
		time.sleep(10)
		things ={'client': "BT", "message_id":"" , "client_data": {}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg
		
time.sleep(10)

