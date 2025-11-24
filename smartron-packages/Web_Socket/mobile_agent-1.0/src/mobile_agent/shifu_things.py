#!/usr/bin/python

__author__ = "Hemant Jiwani"
__copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
__credits__ = ["Hemant Jiwani"]
__maintainer__ = "Hemant Jiwani"
__email__ = "hemant.jiwani@smartron.com"
__status__ = "Testing"

import json
from sqlite_connection import SqLiteDatabaseConnection
from sqlite3 import Error
import config

logger = config.logger


class Things:
    """
    Things class
    """

    def __init__(self):

        self.status_active = "active"
        self.status_close = "close"
        self.conn = None

    """ getting things from things, entity and properties schema"""

    def get_things(self):
        """
        getting all things from things, entity and properties schema
        :return: array of thing object.
        """

        """ Query getting data from things schema """

        get_thing_query = """SELECT thing_id, thing_name, thing_type, manufacturer_name, manufacturer_url,
		hardware_version, firmware_version, framework, battery_status, thing_status
		FROM things 
		WHERE status = ?"""

        """ Query getting data from entity schema """

        get_entity_query = """SELECT id, entity_id, 
		entity_name, thing_id 
		FROM entity 
		WHERE thing_id = ?"""

        """ Query getting data from properties schema """

        get_property_query = """SELECT property_name, 
		property_value, property_type, property_range_min, 
		property_range_max, property_range_step, 
		property_range_list, property_range_unit, entity_id 
		FROM properties 
		WHERE entity_id = ?"""

        """ Initialising things list to add thing json object """

        thing_data = []
        thing_json = {}
        try:
            logger.debug('Getting all the things from things schema')
            """
            Creating connection with Shifu database.
            """
            self.conn = SqLiteDatabaseConnection().create_connection()
            cur = self.conn.cursor()
            cur.execute(get_thing_query, (self.status_active,))
            thing_rows = cur.fetchall()
            logger.debug('Fetched things data from things schema {}'.format(thing_rows))
            if not thing_rows:
                """
                In case, no things data found
                """
                logger.debug('No Things data available in sqlite database')
                # Bug14650: Start: TronX is giving the things list under things key, same change is implemented here.
                return 200, {"status": "success", "message": "List things successfully.", "things": thing_data}
            # Bug14650:End
            else:
                for thing_row in thing_rows:
                    logger.debug('Fetched thing data {} '.format(thing_row))
                    """
                    Creating thing Json with thing data.
                    """
                    thing_json = {'thing_id': thing_row[0],
                                  'thing_name': thing_row[1],
                                  'thing_type': thing_row[2],
                                  'manufacturer_name': thing_row[3],
                                  'manufacturer_url': thing_row[4],
                                  'hardware_version': thing_row[5],
                                  'firmware_version': thing_row[6],
                                  'framework': thing_row[7],
                                  'battery_status': thing_row[8],
                                  'thing_status': thing_row[9]
                                  }
                    logger.debug('Created Thing Json with Thing data {}'.format(thing_json))
                    try:
                        entities = []
                        entity_json = {}
                        thing_id = thing_row[0]
                        entity_values = (thing_id,)
                        cur = self.conn.cursor()
                        logger.debug('Getting all the entities from entity schema having thing-id {}'.format(thing_id))
                        cur.execute(get_entity_query, entity_values)
                        entity_rows = cur.fetchall()
                        logger.debug('Fetched entity data from entity schema {}'.format(entity_rows))
                        if not entity_rows:
                            """
                            In case, no entity data found
                            """
                            logger.debug('No Entity data found with thing-id {}'.format(thing_id))
                            thing_json['entities'] = entities
                        else:
                            for entity in entity_rows:
                                get_entity_id = entity[0]
                                entity_json = {'entity_id': entity[1], 'entity_name': entity[2]}
                                """
                                Creating entity Json with entity data.
                                """
                                logger.debug('Created entity Json with entity data {} '.format(entity_json))
                                """
                                Appending enity Json to entities list.
                                """
                                entities.append(entity_json)
                                logger.debug('Appended entity json in entities list {}'.format(entities))
                                property_values = (get_entity_id,)
                                try:
                                    property_list = []
                                    cur = self.conn.cursor()
                                    logger.debug(
                                        'Getting all the property from properties schema having entity-id {}'.format(
                                            get_entity_id))
                                    cur.execute(get_property_query, property_values)
                                    property_rows = cur.fetchall()
                                    logger.debug(
                                        'Fetched properties data from properties schema {}'.format(property_rows))
                                    if not property_rows:
                                        """
                                        In case, no property data found
                                        """
                                        logger.debug(
                                            'No property data with enity-id {}'.format(get_entity_id))
                                        entity_json['properties'] = property_list
                                    else:
                                        for property_data in property_rows:
                                            """
                                            Creating pipe separated string of property range list. 
                                            """
                                            property_range_list = [] if property_data[6] == "" else property_data[
                                                6].split('|')
                                            property_range_json = {'min': None if property_data[3] == "" else int(
                                                property_data[3]),
                                                                   'max': None if property_data[4] == "" else int(
                                                                       property_data[4]),
                                                                   'step': None if property_data[5] == "" else int(
                                                                       property_data[5]),
                                                                   'list': property_range_list,
                                                                   'unit': None if property_data[7] == "" else
                                                                   property_data[7]}
                                            """
                                            Creating an property range json.
                                            """
                                            logger.debug(
                                                "Created property range Json with property range data {}".format(
                                                    property_range_json))
                                            """ 
                                            Creating Property Json with property data.
                                            """
                                            propertites_json = {'property_name': property_data[0],
                                                                'property_value': property_data[1],
                                                                'property_type': property_data[2],
                                                                'property_range': property_range_json}
                                            logger.debug('Created property Json with propertity data {} '.format(
                                                propertites_json))
                                            """
                                            Appending property json into property list. 
                                            """
                                            property_list.append(propertites_json)
                                            logger.debug(
                                                'Appended property json in properties list {}'.format(property_list))
                                        """
                                        Adding properties list to entity Json.
                                        """
                                        entity_json['properties'] = property_list
                                        logger.debug('Added entity json with properties list {} '.format(entity_json))
                                except Error as e:
                                    """
                                    In case properties query execution or data fetching fails, returning error. 
                                    """
                                    logger.error(e, exc_info=True)
                                    return 500, {"status": "error", "message": e.message}
                            """
                            Adding entities list to things Json. 
                            """
                            thing_json['entities'] = entities
                            logger.debug('Added entity json with entites lsit {} '.format(thing_json))
                    except Error as e:
                        """
                        In case entity query execution or data fetching fails, returning error. 
                        """
                        logger.error(e, exc_info=True)
                        return 500, {"status": "error", "message": e.message}
                    """
                    Adding thing Json to Thing list to get all things.
                    """
                    # Bug14650: Start
                    thing_data.append(thing_json)
                    # Bug14650: End
                    logger.debug('Added thing json to things list {}'.format(thing_data))
            logger.debug('Things in th SQLite schema')
            logger.debug('Things available in sqlite database {}'.format(thing_data))
            # Bug14650:Start: TronX is giving the things list under things key, same change is implemented here.
            return 200, {"status": "success", "message": "List things successfully.", "things": thing_data}
        # Bug14650:End
        except AttributeError as e:
            """
            In case cannot established connection with database. 
            """
            logger.error("Cannot established connection.")
            logger.error('Error message:- {}'.format(e.message))
            return 503, {"status": "error", "message": "Cannot established connection."}
        except Error as e:
            """
            In case things query execution or data fetching fails, returning error. 
            """
            logger.error(e, exc_info=True)
            return 500, {"status": "error", "message": e.message}
        finally:
            """
            closing connection with shifu database.
            """
            if self.conn:
                self.conn.close()
                logger.debug('Closed connection with shifu database.')
