#!/usr/bin/python

import os
import sys


def safely_append(filename):
    if filename not in sys.path:
        sys.path.append('/usr/lib/python2.7/site-packages/'+filename)


def fix_path():
    egg_path = '/usr/lib/python2.7/site-packages'
    file_list = os.listdir(egg_path)
    for filename in file_list:
        if filename.endswith('.egg'):
            safely_append(filename)
  
fix_path()

import web_socket
web_socket.start_service()
