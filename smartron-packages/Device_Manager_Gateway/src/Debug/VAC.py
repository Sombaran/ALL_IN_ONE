import zmq
import sys
import time
import json

context = zmq.Context()

socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:7001")

while True:

	
	command = raw_input("ENTER COMMAND : DISCOVERY CONTROL : ")
	type(command)

	if command == "DISCOVERY":
		json_data = {}
		json_data['client'] = 11
		json_data['notif_id'] = "notif_id1111"
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
		thing_id1 = raw_input("ENTER COMMAND : thing_id : ")
		entity_id1 = raw_input("ENTER COMMAND : entity_id : ")
		property_type1 = raw_input("ENTER COMMAND : property_type // SWITCH : ")
		property_name1 = raw_input("ENTER COMMAND : property_name // OnOff : ")
		things_json1 = {"thing_id": thing_id1,
			"entity_id": entity_id1,
			"property_type" : property_type1,
			"property_name":property_name1,
			"property_value":"1"}
		things_json2 = {"thing_id": thing_id1,
			"entity_id": entity_id1,
			"property_type" : property_type1,
			"property_name":property_name1,
			"property_value":"0"}
		things_json3 = {"thing_id": thing_id1,
			"entity_id": entity_id1,
			"property_type" : property_type1,
			"property_name":property_name1,
			"property_value":"1"}
		things_json4 = {"thing_id": thing_id1,
			"entity_id": entity_id1,
			"property_type" : property_type1,
			"property_name":property_name1,
			"property_value":"0"}
		things_json5 = {"thing_id": thing_id1,
			"entity_id": entity_id1,
			"property_type" : property_type1,
			"property_name":property_name1,
			"property_value":"1"}
		things_json6 = {"thing_id": thing_id1,
			"entity_id": entity_id1,
			"property_type" : property_type1,
			"property_name":property_name1,
			"property_value":"0"}
			
		thing_id2 = raw_input("ENTER COMMAND : thing_id : ")
		entity_id2 = raw_input("ENTER COMMAND : entity_id : ")
		property_type2 = raw_input("ENTER COMMAND : property_type // SWITCH : ")
		property_name2 = raw_input("ENTER COMMAND : property_name // OnOff : ")
		things_json7 = {"thing_id": thing_id2,
			"entity_id": entity_id2,
			"property_type" : property_type2,
			"property_name":property_name2,
			"property_value":"1"}
		things_json8 = {"thing_id": thing_id2,
			"entity_id": entity_id2,
			"property_type" : property_type2,
			"property_name":property_name2,
			"property_value":"0"}
		things_json9 = {"thing_id": thing_id2,
			"entity_id": entity_id2,
			"property_type" : property_type2,
			"property_name":property_name2,
			"property_value":"1"}
		things_json10 = {"thing_id": thing_id2,
			"entity_id": entity_id2,
			"property_type" : property_type2,
			"property_name":property_name2,
			"property_value":"0"}
		things_json11 = {"thing_id": thing_id2,
			"entity_id": entity_id2,
			"property_type" : property_type2,
			"property_name":property_name2,
			"property_value":"1"}
		things_json12 = {"thing_id": thing_id2,
			"entity_id": entity_id2,
			"property_type" : property_type2,
			"property_name":property_name2,
			"property_value":"0"}


		things_array.append(things_json1)
		things_array.append(things_json2)
		things_array.append(things_json3)
		things_array.append(things_json4)
		things_array.append(things_json5)
		things_array.append(things_json6)
		things_array.append(things_json7)
		things_array.append(things_json8)
		things_array.append(things_json9)
		things_array.append(things_json10)
		things_array.append(things_json11)
		things_array.append(things_json12)

		things ={'client': 11, 'notif_id': "notif_id1111","command": "CONTROL", "things": things_array}
		print "Things "+str(things)
		json_data_req = json.dumps(things)
		socket.send(json_data_req)
		print "MOB1_SENT_TO_DEVICE_MANAGER_SERVER:"
		print json_data_req
		msg = socket.recv()
		print "DEVICE_MANAGER_SERVER_ACK:"
		print msg
g
		
time.sleep(100)

