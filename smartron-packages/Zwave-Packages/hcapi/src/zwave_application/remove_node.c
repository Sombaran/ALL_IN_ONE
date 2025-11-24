#include "common.h"
#include "integration.h"
/*
 #define RM_NODE_STS_UNKNOWN 0  ///<Remove node status: unknown
 #define RM_NODE_STS_PROGRESS 1 ///<Remove node status: in progress
 #define RM_NODE_STS_DONE 2     ///<Remove node status: done
 */
extern hl_appl_ctx_t appl_ctx;
extern uint32_t homeid;
extern request_t request;
/**
 remove_node - Removes a node from the  network.
 */
void remove_node() {
	int result;
	char choice;
	response_t response = { 0 };
	if ((appl_ctx.rm_status == RM_NODE_STS_UNKNOWN)
			|| (appl_ctx.rm_status == RM_NODE_STS_DONE)) {
		result = zwnet_add(appl_ctx.zwnet, 0, NULL, 0);
		if (result == 0) {
			syslog(LOG_DEBUG,
					"ZW: Remove node in progress, please wait for status ...");
			appl_ctx.rm_status = RM_NODE_STS_PROGRESS;
		} else {
			syslog(LOG_ERR, "ZW: Remove node with error : %d", result);
			response.home_id = homeid;
			response.msgid = request.msgid;
			response.operation = request.operation;
			response.err_code = result;
			response_DM(response);
		}
	} else //RM_NODE_STS_PROGRESS
	{
		/*result = zwnet_abort(appl_ctx.zwnet);
		 if (result == 0)
		 {
		 syslog(LOG_ERR, "ZW: Remove node operation aborted \n");
		 appl_ctx.rm_status = RM_NODE_STS_UNKNOWN;
		 }
		 else
		 {
		 syslog(LOG_ERR, "ZW: Remove node operation can't be aborted, error:%d \n", result);
		 }*/
		response.home_id = homeid;
		response.operation = REMOVE;
		response.err_code = RM_NODE_STS_PROGRESS;
		response_DM(response);
	}
}

