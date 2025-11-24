#!/usr/bin/python
"""led_client script provides the socket connection for led module to mobile_agent"""
# __author__ = "Subhash kumar"
# __porting__ = "Praveen kumar Donga"
# __copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
# __credits__ = ["Subhash kumar, Praveen kumar Donga"]
# __maintainer__ = "Praveen kumar Donga"
# __email__ = "praveen.donga@smartron.com"

import socket
import struct

PATTERN = dict(
    LED_OFF              	=    0,	     # < LED OFF	
    LED_VOLUME           	=    (1 << 0),     # < Volume , (volume up and down same priority) 
    LED_NO_INTERNET_CHECK_NOTI           =    (1 << 1),   # LED_NO_INTERNET_CHECK_NOTIFY
    LED_ERROR			=    (1 << 2),     # < Error
    LED_WAKEWORD         	=    (1 << 3),     # < Wake word
    LED_ALARM		 	=    (1 << 4),     # < Alarm
    LED_QUERY_PROCESSING 	=    (1 << 5),     # < Query processing
    LED_QUERY_RESPONSE   	=    (1 << 6),     # < Query Response
    LED_DEVICE_CONNECTED 	=    (1 << 7),     # < Device Discovery
    LED_DEVICE_DISCOVERY 	=    (1 << 8),     # < Device Control
    LED_CONNECTED_AS_ROUTER	=    (1 << 9),     # < Connected as Router
    LED_MIC_MUTE         	=    (1 << 10),    # < Mic mute
    LED_SETUP_MODE		=    (1 << 11),    # < device registration mode
    LED_VOICE_VOLUME        =    (1 << 12),    # < Volume change through voice
    LED_MICMUTE_OFF         =    (1 << 13),    # < Mic UnMute after Mute
    LED_BLUTOOTH_DISCOVERY  =    (1 << 14),    # < Blutooth Discovery  
    LED_BLUTOOTH_CONNECTED  =    (1 << 15),    # < Blutooth Connected
    LED_NETWORK_RESET       =    (1 << 16),    # < Network Reset
    LED_WPS_MODE            =    (1 << 17)     # < WPS Mode
)


class LedControl:
    """Led Control class for initialization to Led Socket Server and to send message

    :param: port : port which LED server is listening
    :type: port <4 digit int or string>
    """

    def __init__(self, port):
        """Initialize Led socket client with given ports

        :param port:
        """
        self.host = 'localhost'
        self.port = port
        self.socket = None

    def connect(self):
        """Will connect to LED socket server through UDP Datagram packet, if it failed then return error"""
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        except socket.error:
            return 'Failed to create socket'

    def send_msg(self, event, state):
        """Send message for LED event using class variables

        :param event: type event for LED
        :param state: Switch ON / OFF
        :type state: int {0,1}
        """
        values = (PATTERN[event], state, 0)
        packer = struct.Struct('I I I')
        if not self.socket:
            return "socket is not initialized"
        else:
            self.socket.sendto(packer.pack(*values), (self.host, self.port))
