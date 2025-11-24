------------------------------
Readme for Bt python libraries
-------------------------------
1. bluezutils.py
		
		BlueZ Utilities is the Library used to manage the BlueZ 
		Objects and find the Objects path via D-Bus Interface.

		BlueZ Utilities Functionalities-

			- Get the Managed Objects of Bluez Object Manager
			- Finds the availability of Adapter and Object Path
			- Finds the availability of Device and Object Path
			- Finds the availability of Media and Object Path on 
			  the Bluetooth Adapter
			- Finds the Mediaplayer and Object path of the Bluetooth 
			  Adapter

2. btagent.py
		
		Module is used to Access the API calls

		Module Functionalities -

			- Setting adapter properties for discoverable mode and adapter custom name
			- checking the power of BT adapter
			- checks for the adapter Discovering mode, starts and
			  stops the discovering process
			- Control BT Adapter Power
			- Listing Connected, Paired and Discovered Devices
			- Performs the AVRCP commands on Source Media Player

3. btlogger.py
		
		Initialisation for BT Module Logger Configurations

		BT Logger Functionality -

			- Setting Logger level, Format and Logging Handler

4. __init__.py
		
		library for initialization of API from btagent.py

5. config.py
		
		Global configurations of D-bus Interfaces path for BT Module
