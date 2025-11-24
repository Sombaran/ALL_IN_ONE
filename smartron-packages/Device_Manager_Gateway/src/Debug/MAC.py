import zmq
import sys
import time
import json

context = zmq.Context()

socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:7001")

while True:

	
	command = raw_input("ENTER COMMAND : THINGSLIST CONTROL : ")
	type(command)

	if command == "THINGSLIST":
		json_data = {}
		json_data['client'] = 13
		json_data['command'] = "THINGSLIST"
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
			"entity_id": entity_id,
			"property_type" : property_type,
			"property_name":property_name,
			"property_value":property_value}
		things_array.append(things_json)
		things ={'client': 13, "command": "CONTROL", "things": things_array}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg

		
time.sleep(100)

