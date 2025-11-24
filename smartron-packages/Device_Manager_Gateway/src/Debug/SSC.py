import zmq
import sys
import time
import json

context = zmq.Context()

socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:7001")

while True:

	
	command = raw_input("ENTER COMMAND : REMOVE DISCOVERY CONTROL : ")
	type(command)

	if command == "DISCOVERY":
		json_data = {}
		json_data['client'] = 12
		json_data['command'] = "DISCOVERY"
		json_data_req = json.dumps(json_data)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg
		
	if command == "CONTROL":
		things_array = []
		thing_id = raw_input("ENTER COMMAND : thing_id  ZB_43356_252512003824122 // 43356: ")
		entity_id = raw_input("ENTER COMMAND : entity_id // 3 : ")
		property_type = raw_input("ENTER COMMAND : property_type // switch : ")
		property_name = raw_input("ENTER COMMAND : property_name // On_Off : ")
		property_value = raw_input("ENTER COMMAND : property_value // 0 : ")
		things_json = {"thing_id": thing_id,
			"entity_id": "1",
			"property_type" : property_type,
			"property_name":property_name,
			"property_value":"0"}
		things_json1 = {"thing_id": thing_id,
			"entity_id": "2",
			"property_type" : property_type,
			"property_name":property_name,
			"property_value":"0"}
		things_json2 = {"thing_id": thing_id,
			"entity_id": "3",
			"property_type" : property_type,
			"property_name":property_name,
			"property_value":"1"}
		things_json3 = {"thing_id": thing_id,
			"entity_id": "4",
			"property_type" : property_type,
			"property_name":property_name,
			"property_value":"1"}
		things_array.append(things_json)
		things_array.append(things_json1)
		things_array.append(things_json2)
		things_array.append(things_json3)
		things ={'client': 12, "command": "CONTROL", "things": things_array}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg

	if command == "REMOVE":
		json_data = {}
		json_data['client'] = 12
		json_data['command'] = "REMOVE"
		command = raw_input("ENTER COMMAND : thing_id : ")
		type(command)
		json_data['thing_id'] = command
		json_data_req = json.dumps(json_data)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg
		
time.sleep(100)

