/*
 * File_name:zb_mapping_database.hpp
 * @Brief:Driver classs to create database for Zigbee data
 * Created on: 26-Jul-2018
 * Author: JOTIRLING SWAMI
 * Copyright (C) 2019 Smartron India Private Limited - All Rights Reserved
 */

#ifndef INCLUDE_ZB_MAPPING_DATABASE_HPP_
#define INCLUDE_ZB_MAPPING_DATABASE_HPP_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <sys/syslog.h>

class ZbMappingDatabase {
public:
	ZbMappingDatabase() {}
	static bool ZigbeeDatabseCreate();
	static bool ZigbeeDatabseConfig();
	static bool ZigbeeDatabaseOpen();
	static bool ZigbeeDatabaseClose();
	static std::string DMThingTypeZB(int, int);
	static std::string DMThingNameZB(int, int);
	static std::string DMEntityTypeZB(int, int, int);
	static std::string DMEntityNameZB(int, int, int);
	static int DMEntitySecurityLevelZB(int, int, int);
	static std::string DMPropertyNameZB(int, int, int, int);
	static std::string DMPropertyRangeZB(int, int, int, int);
	static int DMCommandIDCheckZB(int, int, int);
	static int DMPropertyTypeZB(int, int, int, int);
	static int DMDeviceIDZB(std::string);
	static int DMClusterIDZB(std::string);
	static std::string DMAtrTypeIDZB(std::string);
	static int DMommandIDZB(std::string);
	static int DMErrorCodeZB(int);
	static std::string DMErrorTypeZB(int);
	 ~ZbMappingDatabase() {}

};

#endif /* INCLUDE_ZB_MAPPING_DATABASE_HPP_ */
