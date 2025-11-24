#include "ip_camera_framework.hpp"
#include "dm_framework.hpp"
#include "dm_led_interface.hpp"
#include "dm_thingmap.hpp"
#include "dm_response_queue.hpp"

/*IpCameraFramework::IpCameraFramework() {
}
IpCameraFramework::~IpCameraFramework() {
}*/
void IpCameraFramework::discovery(RequestResponse RequestResponseOBJ) {
	syslog(LOG_DEBUG, "[DM] : IPCAMERA Discovery");

	extern char DM_DISCOVERY_TIMEOUT[8];

	json_object* WifiDevice = FoscamSearch(std::stol(DM_DISCOVERY_TIMEOUT) - 2);

	std::cout << json_object_get_string(WifiDevice) << std::endl;

	if (json_object_get_string(WifiDevice) != NULL) {
		syslog(LOG_DEBUG, "[DM] : IPCAMERA Discovery response received");
		DmLedInterface::DiscoveryStop();

		extern bool FlagDiscSend;

		FlagDiscSend = false;

//		json_object* things = json_object_object_get(WifiDevice, "things");

		DmFrameworkHandle::StopDiscReqHandleThread(
				RequestResponseOBJ.Message_Id, "IPCamera");

//		json_object* ThingId = json_object_object_get(
//				json_object_array_get_idx(things, 0), "thing_id");

		syslog(LOG_DEBUG,
				"[DM] : PRINTING THE DISCOVERY WIFI DEVICE DATA SENDING TO TRONX  :[%s]",
				json_object_get_string(WifiDevice));

		/*
		 DmThingMap *ThingMapInstance = DmThingMap::GetInstance();

		 ThingMapInstance->AddToThingMap(ZBThingObj->getthing_id(), ZBThingObj);
		 */
		extern pthread_cond_t DM_Req_Discovery;

		pthread_cond_signal(&DM_Req_Discovery);

		DmResponseQueue* DmResponseQueueOBJ = DmResponseQueue::GetInstance();

		DmResponseQueueOBJ->DMResQueueAddMessage(RequestResponseOBJ, "POST",
				"/hub/thing/discovered", json_object_get_string(WifiDevice),
				PRORITY_VALUE);
	} else {
		syslog(LOG_DEBUG, "[DM]: No IpCamera discovered");
	}

}

void IpCameraFramework::control(RequestResponse obj) {

}

void IpCameraFramework::exclusion(RequestResponse obj) {
}

void IpCameraFramework::remove(RequestResponse obj) {
}

void IpCameraFramework::s2security(RequestResponse obj) {
}

void IpCameraFramework::learnmode(RequestResponse obj) {
}

void IpCameraFramework::forceremove(RequestResponse obj) {
}

void IpCameraFramework::zigbeefota(RequestResponse RequestResponseOBJ) {
	syslog(LOG_INFO, "[DM]: IpCamera framework fota start");
}
