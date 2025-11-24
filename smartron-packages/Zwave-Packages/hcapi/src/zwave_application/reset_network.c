#include "common.h"
#include "integration.h"

/*#define RESET_NW_STS_UNKNOWN 0  ///<Reset network status: unknown
 #define RESET_NW_STS_PROGRESS 1 ///<Reset network status: in progress
 #define RESET_NW_STS_DONE 2     ///<Reset network status: done*/

extern hl_appl_ctx_t appl_ctx;
extern uint32_t homeid;
extern request_t request;
/**
 reset_network - This function resets the zwave network.
 */
void reset_network() {
	response_t response = { 0 };
	int result;
	char choice;

	if (appl_ctx.rst_status == RESET_NW_STS_UNKNOWN
			|| appl_ctx.rst_status == RESET_NW_STS_DONE) {
		result = zwnet_reset(appl_ctx.zwnet);
		if (result == 0) {
			syslog(LOG_DEBUG,
					"ZW: Reset network in progress, please wait for status");
			appl_ctx.rst_status = RESET_NW_STS_PROGRESS;
		} else {
			syslog(LOG_ERR, "ZW: Reset network with error : %d", result);
			response.home_id = homeid;
			response.msgid = request.msgid;
			response.operation = RESET;
			response.err_code = result;
			response_DM(response);
			return;
		}
	} else //RESET_NW_STS_PROGRESS
	{
		syslog(LOG_DEBUG, "ZW: RESET_NODE_STS_PROGRESS");
		response.home_id = homeid;
		response.operation = RESET;
		response.err_code = RESET_NW_STS_PROGRESS;
		response_DM(response);
	}
}

