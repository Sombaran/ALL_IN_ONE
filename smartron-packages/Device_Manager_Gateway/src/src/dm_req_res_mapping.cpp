#include "dm_req_res_mapping.hpp"

DmReqResMapping *DmReqResMapping::Instance = 0;

DmReqResMapping * DmReqResMapping::GetInstance() {
	if (Instance == 0) {
		Instance = new DmReqResMapping;
	}
	return Instance;
}

/*DmReqResMapping::DmReqResMapping() {
 }
 DmReqResMapping::~DmReqResMapping() {
 }*/

void DmReqResMapping::AddToDmReqResMapping(std::string Notif_Id,
		std::string Message_Id, int RequestCount) {

	syslog(LOG_INFO,
			" Dm_requestresponsemapping  /'ADDING INFORMATIONS INTO MAP /'");
	syslog(LOG_INFO, "[DM]: Dm_requestresponsemapping adding notify_id: [%s] "
			"message_id: [%s] and request count: [%d] into /'CheckNotifMap/'/",
			Notif_Id.c_str(), Message_Id.c_str(), RequestCount);
	RequestResponse RequestResponseOBJ;
	RequestResponseOBJ.Message_Id = Message_Id;
	RequestResponseOBJ.Notif_Id = Notif_Id;
	RequestResponseOBJ.SendFlag = false;
	CheckNotifMap[Notif_Id].insert(
			std::make_pair(Message_Id, RequestResponseOBJ));
//----------------------------------------------------------------------------------------------------------------------
	std::map<std::string, RSARequestStruct>::iterator RSARequestMappingItr =
			RSARequestMapping.find(Notif_Id);
	if (RSARequestMappingItr != RSARequestMapping.end()) {

		syslog(LOG_DEBUG,
				"[DM]: Dm_requestresponsemapping /'AVAILABLE IN RSARequestStruct STUCTURE_MAP/' "
						"with notify_id : [%s] message_id: [%s] request count: [%d] response count:[%d]",
				Notif_Id.c_str(), Message_Id.c_str(),
				RSARequestMappingItr->second.RequestCount,
				RSARequestMappingItr->second.ResponseCount);
	} else {
		RSARequestStruct RSARequestStructOBJ;
		RSARequestStructOBJ.RequestCount = RequestCount;
		RSARequestStructOBJ.ResponseCount = 0;
		RSARequestMapping[Notif_Id] = RSARequestStructOBJ;
		syslog(LOG_DEBUG,
				"[DM]: Dm_requestresponsemapping /'NOT_AVAILABLE IN RSARequestStruct STUCTURE_MAP/' "
						"with notify_id: [%s] message_id: [%s] request count: [%d] response count:[%d]",
				Notif_Id.c_str(), Message_Id.c_str(),
				RSARequestStructOBJ.RequestCount,
				RSARequestStructOBJ.ResponseCount);
	}
}

RequestResponse DmReqResMapping::CheckToDmReqResMapping(
		RequestResponse RequestResponseOBJ) {

	syslog(LOG_INFO,
			" Dm_requestresponsemapping  /'CHECKING MAP INFORMATIONS/'");
	DmReqResMapping* DmReqResMappingOBJ = DmReqResMapping::GetInstance();
	syslog(LOG_DEBUG,
			"[DM]: Dm_requestresponsemapping checking message_id from /'REQUEST_RESPONSE STRUCTURE /': [%s], "
					" Notify-id from /'CheckNotifMap/' : [%zu] and Request Map from /'REQUEST_RESPONSE COUNT STRUCTURE/' : [%zu]",
			RequestResponseOBJ.Message_Id.c_str(),
			DmReqResMappingOBJ->CheckNotifMap.size(),
			DmReqResMappingOBJ->RSARequestMapping.size());
	for (std::map<std::string, std::map<std::string, RequestResponse>>::iterator CheckNotifMapItr =
			DmReqResMappingOBJ->CheckNotifMap.begin();
			CheckNotifMapItr != DmReqResMappingOBJ->CheckNotifMap.end();
			++CheckNotifMapItr) {
		syslog(LOG_DEBUG,
				"[DM]: Dm_requestresponsemapping checking notify_id from /'CheckNotifMap/' : [%ld]",
				DmReqResMappingOBJ->CheckNotifMap.size());
		if (DmReqResMappingOBJ->CheckNotifMap.size() != 0) {
//			syslog(LOG_DEBUG,
//			 "[DM]: Dm_requestresponsemapping checking request response search for notif_id: [%s]",
//			 CheckNotifMapItr->first.c_str());
			std::map<std::string, RequestResponse> CheckMessageMapTemp =
					CheckNotifMapItr->second;
			for (std::map<std::string, RequestResponse>::iterator CheckMessageMapItr =
					CheckMessageMapTemp.begin();
					CheckMessageMapItr != CheckMessageMapTemp.end();
					++CheckMessageMapItr) {
				syslog(LOG_DEBUG,
						"[DM]: Dm_requestresponsemapping to compare message_id from /'REQUEST_RESPONSE STRUCTURE /': [%s]",
						RequestResponseOBJ.Message_Id.c_str());
				if (CheckMessageMapItr->first
						== RequestResponseOBJ.Message_Id ||(RequestResponseOBJ.Message_Id == "123")) {
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestresponsemapping checking message_id : [%s]/'KEY/'from /'REQUEST_RESPONSE STRUCTURE /'"
									" and message_id from /'CheckMessageMapItr MAP/'",
							CheckMessageMapItr->first.c_str());
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestresponsemapping checking message_id : [%s] , "
									"notify_id: [%s], send_flag [%d] /' VALUES/' from /'CheckMessageMapItr MAP/' ",
							CheckMessageMapItr->second.Message_Id.c_str(),
							CheckMessageMapItr->second.Notif_Id.c_str(),
							CheckMessageMapItr->second.SendFlag);
//-----------------------------------------------------------------------------------------------------------------------
					std::map<std::string, RSARequestStruct>::iterator RSARequestMappingItr =
							DmReqResMappingOBJ->RSARequestMapping.find(
									CheckMessageMapItr->second.Notif_Id);
					if (RSARequestMappingItr
							!= DmReqResMappingOBJ->RSARequestMapping.end()) {
						RSARequestMappingItr->second.RequestCount;
						RSARequestMappingItr->second.ResponseCount =
								RSARequestMappingItr->second.ResponseCount + 1;
						RequestResponseOBJ.SendFlag = false;
						RequestResponseOBJ.Notif_Id =
								CheckMessageMapItr->second.Notif_Id;
						syslog(LOG_DEBUG,
								"[DM]: Dm_requestresponsemapping checking /'SEND_FLAG_FALSE/'notify_id: [%s] from /'CheckMessageMapItr MAP/'"
										"notif_id: [%s] from /'REQUEST_RESPONSE STRUCTURE /' , message_id: [%s] from /'REQUEST_RESPONSE STRUCTURE /,"
										" request count: [%zu], response count:[%zu] from /'REQUEST_RESPONSE COUNT STRUCTURE MAP/'",
								CheckMessageMapItr->second.Notif_Id.c_str(),
								RequestResponseOBJ.Notif_Id.c_str(),
								RequestResponseOBJ.Message_Id.c_str(),
								(size_t) RSARequestMappingItr->second.RequestCount,
								(size_t) RSARequestMappingItr->second.ResponseCount);
						if (RSARequestMappingItr->second.RequestCount
								== RSARequestMappingItr->second.ResponseCount) {
							RequestResponseOBJ.SendFlag = true;
							RequestResponseOBJ.Notif_Id =
									CheckMessageMapItr->second.Notif_Id;
							syslog(LOG_DEBUG,
									"[DM]: Dm_requestresponsemapping checking /'SEND_FLAG_TRUE/'notify_id: [%s] from /'CheckMessageMapItr MAP/'"
											"notif_id: [%s] from /'REQUEST_RESPONSE STRUCTURE /' , message_id: [%s] from /'REQUEST_RESPONSE STRUCTURE /,"
											" request count: [%zu], response count:[%zu] from /'REQUEST_RESPONSE COUNT STRUCTURE MAP/'",
									CheckMessageMapItr->second.Notif_Id.c_str(),
									RequestResponseOBJ.Notif_Id.c_str(),
									RequestResponseOBJ.Message_Id.c_str(),
									(size_t) RSARequestMappingItr->second.RequestCount,
									(size_t) RSARequestMappingItr->second.ResponseCount);
							syslog(LOG_DEBUG,
									"[DM]: Dm_requestresponsemapping checking /'BEFORE/' /'CheckNotifMap/'size: [%ld] and /'REQUEST_RESPONSE COUNT STRUCTURE MAP/' size: [%ld] ",
									DmReqResMappingOBJ->CheckNotifMap.size(),
									DmReqResMappingOBJ->RSARequestMapping.size());

							/**DELETE THE MAP FROM THE RSARequestMapping*/
							DmReqResMappingOBJ->RSARequestMapping.erase(
									RequestResponseOBJ.Notif_Id);

							/**DELETE THE MAP FROM THE CheckNotifMap AND CheckMessageMap*/
							DmReqResMappingOBJ->CheckNotifMap.erase(
									RequestResponseOBJ.Notif_Id);
							syslog(LOG_DEBUG,
									"[DM]: Dm_requestresponsemapping erasing the Notify_id: [%s] from /'REQUEST_RESPONSE STRUCTURE /' "
											"Notify_id /'CheckNotifMap/' Size after deletion : [%zu]",
									RequestResponseOBJ.Notif_Id.c_str(),
									DmReqResMappingOBJ->CheckNotifMap.size());
//							PrintDmReqResMapping(RequestResponseOBJ.Notif_id);
							syslog(LOG_DEBUG,
									"[DM]: Dm_requestresponsemapping checking /'AFTER/' /'CheckNotifMap/'size: [%ld] and /'REQUEST_RESPONSE COUNT STRUCTURE MAP/' size: [%ld] ",
									DmReqResMappingOBJ->CheckNotifMap.size(),
									DmReqResMappingOBJ->RSARequestMapping.size());
							return RequestResponseOBJ;
						}
					}
				}
			}
		}
	}
	RequestResponseOBJ.SendFlag = false;
	return RequestResponseOBJ;
}

void DmReqResMapping::PrintDmReqResMapping(std::string Notif_Id) {

	syslog(LOG_INFO,
			" Dm_requestresponsemapping  /'PRINTING MAP INFORMATIONS/'");
	DmReqResMapping* DmReqResMappingOBJ = DmReqResMapping::GetInstance();
	syslog(LOG_DEBUG,
			"[DM]: Dm_requestresponsemapping printing /'CheckNotifMap/' size [%zu] and "
					"/'RequestResponseStructure/': [%zu]", CheckNotifMap.size(),
			DmReqResMappingOBJ->RSARequestMapping.size());
	for (std::map<std::string, std::map<std::string, RequestResponse>>::iterator CheckNotifMapItr =
			DmReqResMappingOBJ->CheckNotifMap.begin();
			CheckNotifMapItr != DmReqResMappingOBJ->CheckNotifMap.end();
			++CheckNotifMapItr) {
		syslog(LOG_DEBUG,
				"[DM]: Dm_requestresponsemapping printing notify_id [%s] from /'CheckNotifMap/' and /'BY_USER/': [%s] ",
				CheckNotifMapItr->first.c_str(), Notif_Id.c_str());
		if (Notif_Id == CheckNotifMapItr->first) {
			std::map<std::string, RequestResponse> CheckMessageMapTemp =
					CheckNotifMapItr->second;
			for (std::map<std::string, RequestResponse>::iterator CheckMessageMapItr =
					CheckMessageMapTemp.begin();
					CheckMessageMapItr != CheckMessageMapTemp.end();
					++CheckMessageMapItr) {
				syslog(LOG_DEBUG,
						"[DM]: Dm_requestresponsemapping printing message_id: [%s], "
								"notify_id [%s], send_flag [%d]from /'CheckNotifMap/'",
						CheckMessageMapItr->second.Message_Id.c_str(),
						CheckMessageMapItr->second.Notif_Id.c_str(),
						CheckMessageMapItr->second.SendFlag);
//-----------------------------------------------------------------------------------------------------------------------
				std::map<std::string, RSARequestStruct>::iterator RSARequestMappingItr =
						DmReqResMappingOBJ->RSARequestMapping.find(Notif_Id);
				if (RSARequestMappingItr
						!= DmReqResMappingOBJ->RSARequestMapping.end()) {
					syslog(LOG_DEBUG,
							"[DM]: Dm_requestresponsemapping printing from /'CheckNotifMap/' notify_id: [%s] "
									"message_id: [%s] request count: [%d] response count:[%d]",
							Notif_Id.c_str(),
							CheckMessageMapItr->second.Message_Id.c_str(),
							RSARequestMappingItr->second.RequestCount,
							RSARequestMappingItr->second.ResponseCount);
				}
			}
		}
	}
}
