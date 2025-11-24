#!/usr/bin/python
"""Need to be added"""

__author__ = "Hemant Jiwani"
__copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
__credits__ = ["Hemant Jiwani"]
__maintainer__ = "Hemant Jiwani"
__email__ = "hemant.jiwani@smartron.com"
__status__ = "Testing"

import os
import sqlite3
from sqlite3 import Error
import config
import shifu_db_create

logger = config.logger


class SqLiteDatabaseConnection:
    """SqliteDatabaseConnection
    """

    def __init__(self):
        """
        Directory of the script being run
        """

        # self.directory = os.path.abspath(os.path.join(os.path.dirname(__file__), '../shifu_things'))
        self.database = "/exe_dir/shifu.db"

    def create_connection(self):
        """
        create a database connection to the SQLite database
        :return: connection
        """

        if os.path.isfile(self.database):
            try:
                conn = sqlite3.connect(self.database, isolation_level=None)
                logger.debug("Established Connection with Sqlite database {}".format(self.database))
                return conn
            except Error:
                logger.error("Cannot Established connection.")
                conn = sqlite3.connect(self.database, isolation_level=None)
                conn.execute(shifu_db_create.things_query)
                conn.execute(shifu_db_create.entity_query)
                conn.execute(shifu_db_create.properties_query)
                conn.execute(shifu_db_create.group_query)
                conn.execute(shifu_db_create.group_thing_mapping_query)
                return conn
        else:
            logger.error("Cannot Established connection.")
            conn = sqlite3.connect(self.database, isolation_level=None)
            conn.execute(shifu_db_create.things_query)
            conn.execute(shifu_db_create.entity_query)
            conn.execute(shifu_db_create.properties_query)
            conn.execute(shifu_db_create.group_query)
            conn.execute(shifu_db_create.group_thing_mapping_query)
            return conn
