import zmq
import sys
import time
import json

context = zmq.Context()

socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:7003")

while True:
	
	command = raw_input("ON_STATUS OFF_STATUS SINGLE_DEVICE MULTIPLE_DEVICE CONNECTED_STATUS PAIRED_STATUS INCOMPLETE_JSON NULL_CLIENT_NAME WRONG_CLIENT_NAME NULL_MESSAGE_ID : ")
	type(command)
	if command == "ON_STATUS":
		things ={'client': "BT", "message_id": 1234, "client_data": { "tag": "Status","tag_data":[{"Status":"ON"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg
	if command == "OFF_STATUS":
		things ={'client': "BT", "message_id": 1234, "client_data": { "tag": "Status","tag_data":[{"Status":"OFF"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg
	if command == "SINGLE_DEVICE":
		things ={'client': "BT", "message_id": 1234, "client_data": { "tag": "Device","tag_data":[{"mac_id": "12:31:f2:0a","device_name": "ABCD","status": "not_paired"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg
	if command == "MULTIPLE_DEVICE":
		things ={'client': "BT", "message_id": 1234, "client_data": { "tag": "Device","tag_data":[{"mac_id": "12:31:f2:0a","device_name": "ABCD","status": "not_paired"},{"mac_id": "32:31:fa:0a","device_name": "HONOR 10","status": "not_paired"},{"mac_id": "fa:3c:f2:1a","device_name": "ABCD","status": "not_paired"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg
	if command == "CONNECTED_STATUS":
		things ={'client': "BT", "message_id": 1234, "client_data": { "tag": "Device","tag_data":[{"mac_id": "12:31:f2:0a","device_name": "ABCD","status": "connected"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg
	if command == "PAIRED_STATUS":
		things ={'client': "BT", "message_id": 1234, "client_data": { "tag": "Device","tag_data":[{"mac_id": "12:31:f2:0a","device_name": "ABCD","status": "paired"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg
	if command == "INCOMPLETE_JSON":
		things ={'client': "BT", "message_id": 1234, "client_data": { "tag": "Device","tag_data":[{"mac_id": "12:31:f2:0a","device_name": "ABCD","status": "paired"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg
	if command == "NULL_CLIENT_NAME":
		things ={'client':"", "message_id": 1234, "client_data": { "tag": "Device","tag_data":[{"mac_id": "12:31:f2:0a","device_name": "ABCD","status": "paired"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg
	if command == "WRONG_CLIENT_NAME":
		things ={'client': "BT_WRONG", "message_id": 1234, "client_data": { "tag": "Device","tag_data":[{"mac_id": "12:31:f2:0a","device_name": "ABCD","status": "paired"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg
	if command == "NULL_MESSAGE_ID":
		things ={'client': "WI-Fi", "message_id":"", "client_data": { "tag": "Device","tag_data":[{"mac_id": "12:31:f2:0a","device_name": "ABCD","status": "paired"}]}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg
	if command == "NULL_CLIENT_ID":
		things ={'client': "BT", "message_id":"" , "client_data": {}}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg
		
time.sleep(100)

