#include "common.h"
#include "integration.h"
#include <sys/ipc.h>

hl_appl_ctx_t appl_ctx;
extern uint32_t homeid;
extern int msqid_req;
extern request_t request;
extern request_t requestdummy;

/**
 prompt_str - prompt for a string from user
 @param[in] disp_str   The prompt string to display
 @param[in] out_buf_sz The size of out_str buffer
 @param[out] out_str   The buffer where the user input string to be stored
 @return          The out_str if successful; else NULL.
 */
static char *prompt_str(hl_appl_ctx_t *hl_appl, const char *disp_str,
		int out_buf_sz, char *out_str) {
	int retry;
	puts(disp_str);
	retry = 3;
	while (retry-- > 0) {
		if (fgets(out_str, out_buf_sz, stdin) && (*out_str)
				&& ((*out_str) != '\n')) {
			char *newline;
			/*Remove newline character*/
			newline = strchr(out_str, '\n');
			if (newline) {
				*newline = '\0';
			}
			return out_str;
		}
	}
	return NULL;
}

/**
 prompt_char - prompt for a character input from user
 @param[in] str   The prompt string to display
 @return          The character that user has input. Null character on error.
 */
static char prompt_char(hl_appl_ctx_t *hl_appl, char *str) {
	char ret[80];
	if (prompt_str(hl_appl, str, 80, ret)) {
		return ret[0];
	}
	return 0;
}

/**
 prompt_hex - prompt for an hexadecimal unsigned integer input from user
 @param[in] str   The prompt string to display
 @return          The unsigned integer that user has input
 */
static unsigned prompt_hex(hl_appl_ctx_t *hl_appl, char *str) {
	char user_input_str[36];
	unsigned ret;

	if (prompt_str(hl_appl, str, 36, user_input_str)) {
#ifdef USE_SAFE_VERSION
		if (sscanf_s(user_input_str, "%x", &ret) == 1)
		{
			return ret;
		}
#else
		if (sscanf(user_input_str, "%x", &ret) == 1) {
			return ret;
		}
#endif
	}
	return 0;
}

/**
 prompt_yes - prompt for yes or no from user
 @param[in] str   The prompt string to display
 @return          1 = user has input yes, 0 =  user has input no
 */
int prompt_yes(hl_appl_ctx_t *hl_appl, char *str) {
	char c;
	c = prompt_char(hl_appl, str);
	/*    if (c == 'y' || c == 'Y')
	 {
	 return 1;
	 }*/
	return 0;
}

/**
 hl_add_node_s2_cb - callback for add node with security 2 operation to report Device Specific Key (DSK)
 @param[in]	usr_param  user supplied parameter when calling zwnet_add
 @param[in]	cb_param   DSK related callback parameters
 */
void hl_add_node_s2_cb(void *usr_param, sec2_add_cb_prm_t *cb_param) {

	syslog(LOG_INFO, "ZW: In hl_add_node_s2_cb");
	int timeout = 25;
	response_t response = { 0 };

	hl_appl_ctx_t *hl_appl = (hl_appl_ctx_t *) usr_param;
	int res;

	if (cb_param->cb_type == S2_CB_TYPE_REQ_KEY) {
		uint8_t granted_key;
		uint8_t grant_csa;

		if (hl_appl->sec2_cb_enter & SEC2_ENTER_KEY_REQ) { //Requested keys callback is allowed
			hl_appl->sec2_cb_enter &= ~SEC2_ENTER_KEY_REQ;
		} else {
			syslog(LOG_DEBUG,
					"ZW: Not allowed to processed Security 2 requested keys callback!");
			return;
		}
		syslog(LOG_DEBUG, "ZW: Device requested keys bit-mask: %02Xh",
				cb_param->cb_prm.req_key.req_keys);

		syslog(LOG_INFO, "ZW: Key (bit-mask in hex) :");
		syslog(LOG_INFO, "ZW: Security 2 key 0 (01)");
		syslog(LOG_INFO, "ZW: Security 2 key 1 (02)");
		syslog(LOG_INFO, "ZW: Security 2 key 2 (04)");
		syslog(LOG_INFO, "ZW: Security 0       (80)");

		//granted_key = prompt_hex(hl_appl, "Grant keys bit-mask (hex):");

		granted_key = cb_param->cb_prm.req_key.req_keys;
		syslog(LOG_DEBUG, "ZW: Granted key is: %x", granted_key);
		grant_csa = 0;

		if (cb_param->cb_prm.req_key.req_csa) {
			syslog(LOG_DEBUG,
					"ZW: Device requested for client-side authentication (CSA)");

			if (prompt_yes(hl_appl, "Grant CSA (y/n)?:")) {
				grant_csa = 1;
				syslog(LOG_DEBUG,
						"ZW: Please enter this 10-digit CSA Pin into the joining device: %s",
						cb_param->cb_prm.req_key.csa_pin);
			}
			//No DSK callback when in CSA mode
			hl_appl->sec2_cb_enter &= ~SEC2_ENTER_DSK;
		}

		res = zwnet_add_sec2_grant_key(hl_appl->zwnet, granted_key, grant_csa);

		if (res != 0) {
			syslog(LOG_ERR, "ZW: zwnet_add_sec2_grant_key with error: %d", res);
		}

		//Check whether if there is DSK callback pending
		if (!(hl_appl->sec2_cb_enter)) { //No callback pending
			hl_appl->sec2_cb_exit = 1;
		}
	} else {
		sec2_dsk_cb_prm_t *dsk_prm;
		int accept;
		char dsk_str[200];

		if (hl_appl->sec2_cb_enter & SEC2_ENTER_DSK) { //DSK callback is allowed
			hl_appl->sec2_cb_enter &= ~SEC2_ENTER_DSK;
		} else {
			syslog(LOG_DEBUG,
					"ZW: Not allowed to processed Security 2 DSK callback!");
			return;
		}

		dsk_prm = &cb_param->cb_prm.dsk;
		syslog(LOG_DEBUG, "ZW: Required pin is: %x", dsk_prm->pin_required);
		syslog(LOG_DEBUG, "ZW: Received DSK: %s", dsk_prm->dsk);

		if (dsk_prm->pin_required) {
			syslog(LOG_DEBUG, "ZW: Received DSK in IF: %s", dsk_prm->dsk);

		} else {
			syslog(LOG_DEBUG, "ZW: Received DSK: %s", dsk_prm->dsk);
		}
		//accept = prompt_yes(hl_appl, "Do you accept this device to be added securely (y/n)?:");

		accept = 1;				// Always Add Device Securely = 1
		syslog(LOG_DEBUG, "ZW: You %s the device.",
				(accept) ? "accepted" : "rejected");

		if (accept && dsk_prm->pin_required) {

			response.operation = S2_SECURITY;
			response.home_id = 0;
			response.node_id = 0;
			response.vid = 0;
			response.pid = 0;
			response.vtype = 0;
			response.category = 0;
			response.vtype = 0;
			response.no_of_ep = 1;
			response.ep[0].ep_id = 0;
			response.ep[0].generic_dc = 0;
			response.ep[0].specific_dc = 0;
			response.ep[0].no_of_intf = 0;
			syslog(LOG_DEBUG, "ZW: Security Response operation is: %d",
					response.operation);
			response_DM(response);

			while (timeout) {
				if (msgrcv(msqid_req, &request, sizeof(request_t), 1,
				IPC_NOWAIT) < 0) {
					syslog(LOG_ERR,
							"ZW: msgrcv error: Failed to receive Message");

				} else {
					syslog(LOG_DEBUG, "ZW: Message id: %d", request.msgid);
					syslog(LOG_DEBUG, "ZW: operation: %d", request.operation);
					syslog(LOG_DEBUG, "ZW: homeid: %lu", request.home_id);
					syslog(LOG_DEBUG, "ZW: nodeid: %u", request.node_id);
					syslog(LOG_DEBUG, "ZW: epid: %u", request.ep_id);
					syslog(LOG_DEBUG, "ZW: cls: %u", request.cmd_cls);
					syslog(LOG_DEBUG, "ZW: generic device class: %u",
							request.generic_dc);
					syslog(LOG_DEBUG, "ZW: specific device class: %u",
							request.specific_dc);
					syslog(LOG_DEBUG, "ZW: property_type: %d",
							request.property_type);
					syslog(LOG_DEBUG, "ZW: dsk: %c%c%c%c%c", request.dsk[0],
							request.dsk[1], request.dsk[2], request.dsk[3],
							request.dsk[4]);
					if (request.property_type == ARR_TYPE) {
						int count_value;
						for (count_value = 0; count_value < NO_OF_COLOR_COMP;
								count_value++) {
							syslog(LOG_DEBUG, "ZW: value %d:%d", count_value,
									request.property_value.arr[count_value]);
						}
					} else if (request.property_type == CHAR_TYPE) {
						syslog(LOG_DEBUG, "ZW: value %d",
								request.property_value.ch);
					} else {
						syslog(LOG_DEBUG, "ZW: Wrong property value");
					}
					break;// To Break the while loop after getting DSK key from Device Manager
				}
				sleep(1);// Waiting for 25 Seconds to Enter DSk key, So we are using sleep(1)
				timeout--;
				syslog(LOG_DEBUG, "ZW: Timeout: %d", timeout);
			}
			strcpy(dsk_str, request.dsk);
			strcat(dsk_str, dsk_prm->dsk);
			syslog(LOG_DEBUG, "ZW: The DSK after concatenation is: %s",
					dsk_str);
		}

		res = zwnet_add_sec2_accept(hl_appl->zwnet, accept,
				(dsk_prm->pin_required) ? dsk_str : dsk_prm->dsk);
		syslog(LOG_DEBUG, "ZW: The result of zwnet_add_sec2_accept is %d:",
				res);
		hl_appl->sec2_cb_exit = 1;
	}
}

/**
 hl_add_node - Add node
 @param[in]	hl_appl  Application context
 @return zero if successful; else negative error number
 */
static int hl_add_node(hl_appl_ctx_t *hl_appl) {
	int res;
	char dsk_str[200];
	zwnetd_p netdesc;

	netdesc = zwnet_get_desc(hl_appl->zwnet);

	if (netdesc->ctl_cap & ZWNET_CTLR_CAP_S2) {
		syslog(LOG_DEBUG, "ZW: Controller supports security 2");
		hl_appl->sec2_add_node = 1;
	} else {
		hl_appl->sec2_add_node = 0;
	}

	if (hl_appl->sec2_add_node) {
		hl_appl->sec2_add_prm.dsk = NULL;

		/*  if (prompt_yes(hl_appl, "Pre-enter Device Specific Key (DSK) (y/n)?:"))
		 {
		 if (prompt_str(hl_appl, "DSK:", 200, dsk_str))
		 {
		 hl_appl->sec2_add_prm.dsk = dsk_str;
		 }
		 }*/

		hl_appl->sec2_add_prm.usr_param = hl_appl;
		hl_appl->sec2_add_prm.cb = hl_add_node_s2_cb;
	}

	res = zwnet_add(hl_appl->zwnet, 1,
			(hl_appl->sec2_add_node) ? &hl_appl->sec2_add_prm : NULL, 0);

	if (res == 0) {
		if (hl_appl->sec2_add_node) {
			int wait_count;

			hl_appl->sec2_cb_enter = SEC2_ENTER_KEY_REQ;

			if (!hl_appl->sec2_add_prm.dsk) { //No pre-entered DSK, requires DSK callback
				hl_appl->sec2_cb_enter |= SEC2_ENTER_DSK;
			}

			hl_appl->sec2_cb_exit = 0;
			syslog(LOG_DEBUG,
					"ZW: Waiting for Requested keys and/or DSK callback ...");

//Wait for S2 callback to exit
			wait_count = 500; //Wait for 50 seconds
			while (wait_count-- > 0) {
				if (hl_appl->sec2_cb_exit == 1)
					break;
				plt_sleep(100);
			}
		}
	}

	return res;
}

/**
 add_node - Adds a node to the  network.
 */
void add_node() {
	syslog(LOG_DEBUG, "ZW: In add_node");
	int result;
	response_t response = { 0 };
	/*	response_t response = {0};*/
	if ((appl_ctx.add_status == ADD_NODE_STS_UNKNOWN)
			|| (appl_ctx.add_status == ADD_NODE_STS_DONE)) {
		result = hl_add_node(&appl_ctx);
		/*		result = zwnet_add(appl_ctx.zwnet, 1, (appl_ctx.sec2_add_node)? &appl_ctx.sec2_add_prm : NULL);*/
		if (!appl_ctx.sec2_add_node) {
			if (result == 0) {
				syslog(LOG_DEBUG,
						"ZW: Add node in progress, please wait for status ...");
				appl_ctx.add_status = ADD_NODE_STS_PROGRESS;
			} else {
				syslog(LOG_ERR, "ZW: Add node with error : %d", result);
				response.home_id = homeid;
				response.msgid = request.msgid;
				response.operation = COMMISSIONING;
				response.err_code = result;
				response_DM(response);
				return;
			}
		}
	} else /*ADD_NODE_STS_PROGRESS*/
	{
		syslog(LOG_DEBUG, "ZW: ADD_NODE_STS_PROGRESS");
	}
}

