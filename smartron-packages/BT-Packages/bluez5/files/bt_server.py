# first of all import the socket library
import socket
import Python_BT
import subprocess
import thread
import time

bt_port = 4274 

log = Python_BT.log_init()

AVRCP_commands = ("play","pause","next","previous","restart")
AVCTP_commands = ("PLAY","PAUSE","FORWARD","BACKWARD")
GST_CONTROL = ("PLAY","PAUSE","NEXT","PREV")

try:
    status_value = subprocess.check_output(["fw_printenv", "DEVICE_REGISTRATION_STATUS"]).strip().split('=')[1]
    if status_value == "registered":
        status = Python_BT.bt_discoverable(1)
    elif status_value != "registered":
        Python_BT.bt_discoverable(86400) 
except:
    print ("BTError: Discoverable mode set to off failed")

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)         
s.bind(('localhost', bt_port))        
s.listen(5)     

connect_process_status = False

# Redmine 19739: Client handler for multiple request handling
def client_handler_func(client, addr):
    global connect_process_status
    message=client.recv(16)
    log.debug("BT: SERVER: Voice command: "+str(message))
    # Redmine 20518 Start: White noise will be played query response when connected to External BT speaker
    if (message=="silence"):
        subprocess.Popen("paplay -p /etc/bluetooth/white_noise.wav --volume=4096",shell=True)
        time.sleep(0.5)
        client.send("silence-success")
    # Redmine 20518 Stop

    elif (message=="discoverable"):
        client.send("bton-Received-Success")
        if not bool(Python_BT.check_adapter("power")):
            Python_BT.bt_switch(True) # Turn ON power and discoverable mode
            Python_BT.dm_socket("NA","Status","ON")
        Python_BT.va_socket("BT VOICE Searching for bluetooth devices, Go ahead and connect to available device on your T Home app",)
        Python_BT.bt_start_discovery()

    elif (message=="undiscoverable"):
        client.send("btoff-Received-Success")
        if bool(Python_BT.check_adapter("power")):
            Python_BT.bt_switch(False)
            Python_BT.dm_socket("NA","Status","OFF")
            #Feature: 19673 Start
            Python_BT.va_socket("BT VOICE Sure, turned off bluetooth", False)
            #Feature: 19673 End

    elif (message=="connect"):
        client.send("Connect-Received-Success")
        if not bool(Python_BT.check_adapter("power")):
            status = Python_BT.bt_switch(True) # Turn ON in discoverable mode
            if(status=='Success'):
                Python_BT.dm_socket("NA","Status","ON")
        if not bool(Python_BT.bt_list_paired_dev()):
            Python_BT.va_socket("BT VOICE Searching for bluetooth devices, Go ahead and connect to available device on your T Home app",)
            Python_BT.bt_start_discovery()
        else:
            # Redmine 19739 Start: Voice command will try connecting to Last 5 connected devices
            if not connect_process_status:
                five_list = Python_BT.five_pair_list.read()
                if bool(five_list):
                    connect_process_status = True
                    five_list.reverse()
                    for mac_addr in five_list:
                        log.debug("BT: SERVER: "+str(mac_addr)+" Connecting")
                        status=Python_BT.bt_process(mac_addr,"connect")
                        if(status=="Success"):
                            dm_msg={}
                            dm_msg[mac_addr]=Python_BT.get_device_property(mac_addr,"Name")
                            Python_BT.dm_socket(dm_msg,"Device","connected")
                            connect_process_status = False
                            break
                    connect_process_status = False
            # Redmine 19739 Stop

    elif (message=="disconnect"):
        client.send("Disconnect-Recvd-Success")
        file=open("/etc/bluetooth/lastconnect_macaddr","r")
        file_read=file.read()
        file.close()
        if(len(file_read)>1):
            mac_addr=file_read.split(" ")
            status=Python_BT.bt_process(mac_addr[0],"disconnect")
            if(status=="Success"):
                dm_msg={}
                dm_msg[mac_addr[0]]=Python_BT.get_device_property(mac_addr[0],"Name")
                Python_BT.dm_socket(dm_msg,"Device","paired")

    elif (message=="mediastatus"):
        file=open("/etc/bluetooth/lastconnect_macaddr","r")
        mac_addr=file.read().split(" ")
        file.close()
        status=Python_BT.get_mediaplayer_property(mac_addr[0],"Status")      
        client.send(str(status))

    elif message in AVRCP_commands:
        client.send("AVRCP-Received-Success")
        connected_dev=Python_BT.bt_list_connected_dev()
        if bool(connected_dev):
            Python_BT.bt_avrcp_command(connected_dev["Device_macId"],message)

    elif message in AVCTP_commands:
        file=open("/etc/bluetooth/lastconnect_macaddr","r")
        mac_addr=file.read().split(" ")
        file.close()
        for i in range (1,4):
            if(message==AVCTP_commands[i]):
                print(GST_CONTROL[i])
                Python_BT.va_socket("BT COMMAND "+GST_CONTROL[i],)
                log.debug('[BT]:[AUTO]: AVCTP '+str(GST_CONTROL[i])+' '+str(mac_addr[0]))
                print("done")
        if (message=="PLAY"):
            pactl=subprocess.check_output('pactl list sinks short | grep bluez_sink | awk -v x=7 \'{print $x}\'',shell=True).rstrip('\n')
            print(pactl)
            if(pactl=="RUNNING"):
                Python_BT.va_socket("BT COMMAND "+GST_CONTROL[1],)
                log.debug('[BT]:[AUTO]: AVCTP '+str(GST_CONTROL[1])+' '+str(mac_addr[0]))
                print(GST_CONTROL[1])
            elif (pactl=="IDLE"):
                Python_BT.va_socket("BT COMMAND "+GST_CONTROL[0],)
                log.debug('[BT]:[AUTO]: AVCTP '+str(GST_CONTROL[0])+' '+str(mac_addr[0]))
                print(GST_CONTROL[0])

    else:
        client.send("Invalid-Command         ")
    client.close()

while True:
    client, addr = s.accept()
    thread.start_new_thread(client_handler_func,(client,addr))
