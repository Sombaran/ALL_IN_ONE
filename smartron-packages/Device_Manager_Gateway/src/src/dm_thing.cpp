#include "dm_thing.hpp"
#include "dm_thingmap.hpp"
#include "dm_framework.hpp"
#include "dm_zeromq.hpp"
#include "dm_platform.hpp"

/*DM Property*/
DmProperty::DmProperty() {

	property_type = "DM_DEFAULT";
	property_name = "DM_DEFAULT";
	property_range = "DM_DEFAULT";
	property_value = "DM_DEFAULT";
}

void DmProperty::Display() {
	syslog(LOG_INFO, "[DM]: Thing property display");
	syslog(LOG_DEBUG, "[DM]: Property property_type: [%s]",
			this->property_type.c_str());
	syslog(LOG_DEBUG, "[DM]: Property property_name: [%s]",
			this->property_name.c_str());
	syslog(LOG_DEBUG, "[DM]: Property property_range: [%s]",
			this->property_range.c_str());
	syslog(LOG_DEBUG, "[DM]: Property property_value: [%s]",
			(this->property_value).c_str());
}

std::string DmProperty::getproperty_value() {
	return this->property_value;
}
std::string DmProperty::getproperty_type() {
	return this->property_type;
}
std::string DmProperty::getproperty_name() {
	return this->property_name;
}
std::string DmProperty::getproperty_range() {
	return this->property_range;
}

void DmProperty::setproperty_type(std::string property_type) {
	this->property_type = property_type;
}
void DmProperty::setproperty_name(std::string property_name) {
	this->property_name = property_name;
}
void DmProperty::setproperty_range(std::string property_range) {
	this->property_range = property_range;
}
void DmProperty::setproperty_value(std::string stringValue) {
	this->property_value = stringValue;
}

//=================================DmEntity==========================================

DmEntity::DmEntity() {
	entity_id = "DM_DEFAULT";
	entity_name = "DM_DEFAULT";
	security_level = 0;

	propertylist.clear();
}

void DmEntity::Display() {
	syslog(LOG_INFO, "[DM]: Thing entity display");
	syslog(LOG_DEBUG, "[DM]: Entity entity_id: [%s]", this->entity_id.c_str());
	syslog(LOG_DEBUG, "[DM]: Entity entity_name: [%s]",
			this->entity_name.c_str());
	syslog(LOG_DEBUG, "[DM]: Entity entity_type: [%s]",
			this->entity_type.c_str());
	syslog(LOG_DEBUG, "[DM]: Entity security_level: [%d]",
			this->security_level);

	for (int property_count = 0; property_count < this->getPropertySize();
			property_count++) {
		this->propertylist[property_count]->Display();
	}
}

DmProperty* DmEntity::getPropertyInfo(int property_count) {
	return this->propertylist[property_count];
}

int DmEntity::getPropertySize() {
	return this->propertylist.size();
}

std::string DmEntity::getentity_id() {
	return this->entity_id;
}
std::string DmEntity::getentity_type() {
	return this->entity_type;
}
std::string DmEntity::getentity_name() {
	return this->entity_name;
}

int DmEntity::getsecurity_level() {
	return this->security_level;
}

void DmEntity::setentity_id(std::string entity_id) {
	this->entity_id = entity_id;
}
void DmEntity::setentity_name(std::string entity_name) {
	this->entity_name = entity_name;
}
void DmEntity::setentity_type(std::string entity_type) {
	this->entity_type = entity_type;
}

void DmEntity::setsecurity_level(int security_level) {
	this->security_level = security_level;
}

void DmEntity::setpropertyInfo(DmProperty* propertyDummy) {
	this->propertylist.push_back(propertyDummy);
}

//=============================DmThing==============================================

DmThing::DmThing() {

	home_node_id = "DM_DEFAULT";
	zone_type = "DM_DEFAULT";

	frameworkName = "DM_DEFAULT";
	manufacturerName = "DM_DEFAULT";
	manufacturerUrl = "DM_DEFAULT";
	firmwareVersion = "DM_DEFAULT";
	hardwareVersion = "DM_DEFAULT";

	battery_status = "DM_DEFAULT";
	thing_type = "DM_DEFAULT";
	thing_status = "DM_DEFAULT";
	thing_name = "DM_DEFAULT";
	thing_id = "DM_DEFAULT";

	entitylist.clear();
}

void DmThing::ThingDiscovery(RequestResponse RequestResponseOBJ) {

	//syslog(LOG_INFO, "[DM]: Thing thing discovery start");

	DmFrameworkHandle::DiscoveryStart(RequestResponseOBJ);
}

bool DmThing::ThingControl(RequestResponse RequestResponseOBJ) {

	//syslog(LOG_INFO, "[DM]: Thing thing control start");

	return DmFrameworkHandle::ControlStart(RequestResponseOBJ);
}

bool DmThing::ThingRemove(RequestResponse RequestResponseOBJ) {

	//syslog(LOG_INFO, "[DM]: Thing thing remove start");

	return DmFrameworkHandle::RemoveStart(RequestResponseOBJ);

}

void DmThing::ThingExclusion(RequestResponse RequestResponseOBJ) {

	//syslog(LOG_INFO, "[DM]: Thing thing exclusion start");

	DmFrameworkHandle::ExclusionStart(RequestResponseOBJ);

}

void DmThing::ThingS2secutity(RequestResponse RequestResponseOBJ) {

	//syslog(LOG_INFO, "[DM]: Thing thing exclusion start");

	DmFrameworkHandle::S2securityStart(RequestResponseOBJ);

}

void DmThing::ThingForceRemove(RequestResponse RequestResponseOBJ) {

	syslog(LOG_INFO, "[DM]: Thing thing Force Remove start");

	DmFrameworkHandle::ForceRemoveStart(RequestResponseOBJ);

}

void DmThing::ZigbeeFota(RequestResponse RequestResponseOBJ){
	DmFrameworkHandle::ZigbeeFotaStart(RequestResponseOBJ);
}

void DmThing::ThingLearnMode(RequestResponse RequestResponseOBJ) {

	syslog(LOG_INFO, "[DM]: Thing thing Force Remove start");

	DmFrameworkHandle::LearnModeStart(RequestResponseOBJ);

}

void DmThing::Display() {
	syslog(LOG_INFO, "[DM]: Thing thing display");

	syslog(LOG_DEBUG, "[DM]: Thing node_id: [%s]", this->home_node_id.c_str());
	syslog(LOG_DEBUG, "[DM]: Thing zone_type: [%s]", this->zone_type.c_str());

	syslog(LOG_DEBUG, "[DM]: Thing framework_name: [%s]",
			this->frameworkName.c_str());
	syslog(LOG_DEBUG, "[DM]: Thing manufacturing_name: [%s]",
			this->manufacturerName.c_str());
	syslog(LOG_DEBUG, "[DM]: Thing manufacture_url: [%s]",
			this->manufacturerUrl.c_str());
	syslog(LOG_DEBUG, "[DM]: Thing firmware_version: [%s]",
			this->firmwareVersion.c_str());
	syslog(LOG_DEBUG, "[DM]: Thing hardware_version: [%s]",
			this->hardwareVersion.c_str());

	syslog(LOG_DEBUG, "[DM]: Thing battery_status: [%s]",
			this->battery_status.c_str());
	syslog(LOG_DEBUG, "[DM]: Thing thing_type: [%s]",
			this->thing_type.c_str());
	syslog(LOG_DEBUG, "[DM]: Thing thing_status: [%s]",
			this->thing_status.c_str());
	syslog(LOG_DEBUG, "[DM]: Thing thing_name: [%s]",
			this->thing_name.c_str());
	syslog(LOG_DEBUG, "[DM]: Thing thing_id: [%s]", this->thing_id.c_str());

	for (int EntityCount = 0; EntityCount < this->getEntityListSize();
			EntityCount++) {
		this->entitylist[EntityCount]->Display();
	}
}

/*get_thing_start*/
DmFramework* DmThing::getFramework() {
	return this->DmFrameworkObj;
}
std::string DmThing::gethome_node_id() {
	return this->home_node_id;
}
std::string DmThing::getzone_type() {
	return this->zone_type;
}
std::string DmThing::getframeworkName() {
	return this->frameworkName;
}
std::string DmThing::getmanufacturerName() {
	return this->manufacturerName;
}
std::string DmThing::getmanufacturerUrl() {
	return this->manufacturerUrl;
}
std::string DmThing::getfirmwareVersion() {
	return this->firmwareVersion;
}
std::string DmThing::gethardwareVersion() {
	if (this->hardwareVersion == "DM_DEFAULT"){
		this->hardwareVersion = "0";
	}
	return this->hardwareVersion;
}
std::string DmThing::getbattery_status() {
	return this->battery_status;
}
std::string DmThing::getthing_type() {
	return this->thing_type;
}
std::string DmThing::getthing_status() {
	return this->thing_status;
}
std::string DmThing::getthing_name() {
	return this->thing_name;
}
std::string DmThing::getthing_id() {
	return this->thing_id;
}

int DmThing::getEntityListSize() {
	return this->entitylist.size();
}

DmEntity* DmThing::getEntityInfo(int EntityCount) {
	return this->entitylist[EntityCount];
}

/*Set thing method start*/
void DmThing::setstatus_code(std::string status_code) {
	this->status_code = status_code;
}

void DmThing::setzone_type(std::string zone_type) {
	this->zone_type = zone_type;
}
void DmThing::sethome_node_id(std::string home_node_id) {
	this->home_node_id = home_node_id;
}
void DmThing::setframeworkName(std::string frameworkName) {
	this->frameworkName = frameworkName;
}
void DmThing::setmanufacturerName(std::string manufacturerName) {
	this->manufacturerName = manufacturerName;
}
void DmThing::setmanufacturerUrl(std::string manufacturerUrl) {
	this->manufacturerUrl = manufacturerUrl;
}
void DmThing::setfirmwareVersion(std::string firmwareVersion) {
	this->firmwareVersion = firmwareVersion;
}
void DmThing::sethardwareVersion(std::string hardwareVersion) {
	this->hardwareVersion = hardwareVersion;
}
void DmThing::setbattery_status(std::string battery_status) {
	this->battery_status = battery_status;
}
void DmThing::setthing_type(std::string thing_type) {
	this->thing_type = thing_type;
}
void DmThing::setthing_status(std::string thing_status) {
	this->thing_status = thing_status;
}
void DmThing::setthing_name(std::string thing_name) {
	this->thing_name = thing_name;
}
void DmThing::setthing_id(std::string thing_id) {
	this->thing_id = thing_id;
}
void DmThing::setentityInfo(DmEntity* entityDummy) {
	this->entitylist.push_back(entityDummy);
}

void DmThing::setFramework(DmFramework* Obj) {
	DmFrameworkObj = Obj;
}

DmThing::~DmThing() {
	entitylist.shrink_to_fit();
}

DmEntity::~DmEntity() {
	propertylist.shrink_to_fit();
}

DmProperty::~DmProperty() {
}
/*Set thing method end*/
