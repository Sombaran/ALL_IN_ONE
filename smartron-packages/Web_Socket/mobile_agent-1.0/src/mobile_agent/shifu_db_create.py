#!/usr/bin/python
"""This are SQL commands for curd operations on db
"""

__author__ = "Hemant Jiwani"
__copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
__credits__ = ["Hemant Jiwani"]
__maintainer__ = "Hemant Jiwani"
__email__ = "hemant.jiwani@smartron.com"
__status__ = "Testing"

""" creating things table. """
things_query = """CREATE TABLE IF NOT EXISTS 
			   things (thing_id CHAR(20) PRIMARY KEY, thing_name CHAR(30), thing_type CHAR(30),
			   firmware_version CHAR(16), framework CHAR(30), manufacturer_name CHAR(30), 
			   manufacturer_url CHAR(50), hardware_version CHAR(15), battery_status CHAR(16),
			   thing_status CHAR(15), status CHAR(8));"""

""" creating entity table. """
entity_query = """CREATE TABLE IF NOT EXISTS 
			   entity (id INTEGER PRIMARY KEY AUTOINCREMENT, entity_id CHAR(8), 
			   entity_name CHAR(30), thing_id CHAR(20), 
			   FOREIGN KEY(thing_id) REFERENCES things(thing_id));"""

""" creating properties table. """
properties_query = """CREATE TABLE IF NOT EXISTS 
				   properties (id INTEGER PRIMARY KEY AUTOINCREMENT, property_name CHAR(30), 
				   property_value CHAR(30), property_type CHAR(16), property_range_min INTEGER, 
				   property_range_max INTEGER, property_range_step INTEGER, 
				   property_range_list CHAR(255), property_range_unit CHAR(15), 
				   entity_id INTEGER, FOREIGN KEY(entity_id) REFERENCES entity(id));"""

""" creating groups table. """
group_query = """CREATE TABLE IF NOT EXISTS 
			  groups (group_id INTEGER PRIMARY KEY, group_name CHAR(30), status CHAR(8));"""

""" creating mapping table. """
group_thing_mapping_query = """CREATE TABLE IF NOT EXISTS 
							group_thing_mapping (id INTEGER PRIMARY KEY AUTOINCREMENT, 
							group_id INTEGER, thing_id CHAR(20), 
							FOREIGN KEY(thing_id) REFERENCES things(thing_id), 
							FOREIGN KEY(group_id) REFERENCES groups(group_id));"""

# conn.execute(things_query)
# conn.execute(entity_query)
# conn.execute(properties_query)
# conn.execute(group_query)
# conn.execute(group_thing_mapping_query)
