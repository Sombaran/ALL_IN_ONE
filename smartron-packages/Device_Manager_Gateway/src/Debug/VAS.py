import time
import zmq
import json

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:8800")


messages = []

while True:
	messages=socket.recv()
	print messages
	socket.send("DONE")
	print "DONE"
