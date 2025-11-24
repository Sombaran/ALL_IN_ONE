==========
Change Log
==========

This document records all notable changes to `SocketIo Client Service and tronX SocketIo Server`.
This project adheres to `Semantic Versioning <http://semver.org/>`_.

:Authors:
        Hemant Jiwani <hemant.jiwani@smartron.com>

        Developer (Shifu-Hub Apps)
:Version: `1.9.0-dev` of 2018/09/29 

`1.10.0-dev` (unreleased)
-------------------------

* NA

Modification
^^^^^^^^^^^^

* Name changed from Home Manger(HM) to Web Socket(WS).

`0.0.2-dev` (released)
-------------------------

* SocketIoClientQueue class defined
* run function to recieve data from ShifuNamespace on notification and put it in a queue.
* worker function to sent data from queue to HMClient task.
* HMClientTask class defined.
* run function to second data to HMServer.

Modification
^^^^^^^^^^^^

* Change in config module logger.config
* Change in getting HomeToken from system enviornment.
* change in getting HostName from system enviornment


`0.0.1-dev` (released)
------------------------

Added
^^^^^
* SocketIO Client Class defined
* socket_io function to connect with tronX socketIo server
* ShifuNamespace Class defined
* on_connect function on connection, on_disconnect function on disconnection, on_reconnect function on reconnection and
  on_notification function on data from `/shifu` method call
* SocketIoClientThread class defined
* run function to start socketIo Client as daemon process.

Modification
^^^^^^^^^^^^
* Change in module from SocketIO nexus client to SocketIo client.
* Change in handshake timming from 3 second to 10 second with tronX socketIo server


