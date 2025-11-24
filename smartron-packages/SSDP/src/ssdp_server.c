#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>     // select
#include <sys/time.h>   // gettimeofday
#include "lssdp.h"
// Bug16158:Start SSDP service getting killed during eth0 interface down
#include <poll.h>
#include "fw-env.h"     // to get NVM variables
// Bug16158:End

// Log callback function
void log_callback(const char * file, const char * tag, int level, int line,
		const char * func, const char * message) {
	char * level_name = "DEBUG";
	if (level == LSSDP_LOG_INFO)
		level_name = "INFO";
	if (level == LSSDP_LOG_WARN)
		level_name = "WARN";
	if (level == LSSDP_LOG_ERROR)
		level_name = "ERROR";

	printf("[%-5s][%s] %s", level_name, tag, message);
}

long long get_current_time() {
	struct timeval time = { };
	if (gettimeofday(&time, NULL) == -1) {
		printf("gettimeofday failed, errno = %s (%d)\n", strerror(errno),
				errno);
		return -1;
	}
	return (long long) time.tv_sec * 1000 + (long long) time.tv_usec / 1000;
}

// Shows neighbour list. When *NOTIFY* or *RESPONSE* packet is received, neighbor list will be updated.
int show_neighbor_list(lssdp_ctx * lssdp) {
	int i = 0;
	lssdp_nbr * nbr;
	puts("\nSSDP List:");
	for (nbr = lssdp->neighbor_list; nbr != NULL; nbr = nbr->next) {
		printf(
				"%d. id = %-9s, ip = %-20s, name = %-12s, device_mode = %-8s (%lld)\n",
				++i, nbr->device_id, nbr->location, nbr->usn, nbr->device_mode,
				nbr->update_time);
	}
	printf("%s\n", i == 0 ? "Empty" : "");
	return 0;
}

int show_interface_list_and_rebind_socket(lssdp_ctx * lssdp) {
	// 1. show interface list
	printf("\nNetwork Interface List (%zu):\n", lssdp->interface_num);
	size_t i;
	for (i = 0; i < lssdp->interface_num; i++) {
		printf("%zu. %-6s: %s\n", i + 1, lssdp->interface[i].name,
				lssdp->interface[i].ip);
	}
	printf("%s\n", i == 0 ? "Empty" : "");

	// 2. re-bind SSDP socket
	if (lssdp_socket_create(lssdp) != 0) {
		puts("SSDP create socket failed");
		return -1;
	}
	return 0;
}

int main() {
	// Setup SSDP log callback.
	lssdp_set_log_callback(log_callback);

	// Bug16158:Start SSDP service getting killed during eth0 interface down
	// Feature16858:Start
	char fw_env_val_ma[128];
	char fw_env_val_id[128];
	char ma_core_port[128] = ":";
	char device_id[128];
	char device_mode[128];
	memset(fw_env_val_ma, '\0', sizeof(fw_env_val_ma));
	memset(fw_env_val_id, '\0', sizeof(fw_env_val_id));
	memset(device_id, '\0', sizeof(device_id));
	memset(device_mode, '\0', sizeof(device_mode));

	lssdp_ctx lssdp = {
			//.debug = true,           		     				// debug
			.port = 1900,										// SSDP port (0x0000 ~ 0xFFFF)
			.neighbor_timeout = 15000,  						// 15 seconds
			.header = { 
					.search_target = "ST_P2P",					// Search Targets
					.unique_service_name = "f835dd000001",		// Unique Service Name: MAC or User Name    //FIX ME: Yet to decide what parmater goes here
					.device_id = NULL,			    			// Shifu Hub DeviceID
					.device_mode = NULL,						// Device Mode: "Q-WRAP" or "DEFAULT"
					.location.suffix = NULL						// URI or Port: "/index.html" or ":80"
			},
			// callback
			.neighbor_list_changed_callback = show_neighbor_list,
			.network_interface_changed_callback =
					show_interface_list_and_rebind_socket, };
	int fw_env_ret_ma = get_shifu_uci_param("MA_LISTEN_PORT", fw_env_val_ma);
	int fw_env_ret_id = get_fw_env("DEVICE_ID", fw_env_val_id);

	if ((fw_env_ret_ma == 0) && (fw_env_ret_id == 0)) {
		strcat(ma_core_port, fw_env_val_ma);
		strcpy(device_id, fw_env_val_id);
		//printf("MACorePort: %s\n", ma_core_port);      // debug
		//printf("DeviceID: %s\n", device_id);           // debug
		strcpy(lssdp.header.location.suffix, ma_core_port);
		strcpy(lssdp.header.device_id, device_id);
	} else if (fw_env_ret_ma != 0) {
		printf("MACOREPORT NV fetch failed");
		strcpy(device_id, fw_env_val_id);
		strcpy(lssdp.header.device_id, device_id);
	} else if (fw_env_ret_id != 0) {
		printf("DEVICE_ID NV fetch failed");
		strcat(ma_core_port, fw_env_val_ma);
		strcpy(lssdp.header.location.suffix, ma_core_port);
	} else if ((fw_env_ret_ma != 0) && (fw_env_ret_id != 0)) {
		printf("MACOREPORT and DEVICE_ID NV fetch failed");
	}
	//printf("%s\n", lssdp.header.location.suffix);      // debug
	//printf("%s\n", lssdp.header.device_id);      	     // debug
	// Feature16858:End
	// Bug16158:End

	/*
	 * Get network interface at first time, network_interface_changed_callback will be invoke
	 * SSDP socket will be created in callback function
	 */
	lssdp_network_interface_update(&lssdp);

	long long last_time = get_current_time();
	if (last_time < 0) {
		printf("got invalid timestamp %lld\n", last_time);
		return EXIT_SUCCESS;
	}

	// Main Loop
	for (;;) {

		// Feature18157:Start Device wifi Mode should be Advertised to clients through SSDP
		int rc;
		rc = system("uci show network.lan.proto | grep dhcp");
		//printf("Return code: %d\n", rc);							 // debug
		if (rc == 0) {
			//printf("DHCP is not present\n");						 // debug
			strcpy(lssdp.header.device_mode, "Q-WRAP");
			//printf("%s\n", lssdp.header.device_mode);     	     // debug
		} else {
			//printf("DHCP is present\n");   						 // debug
			strcpy(lssdp.header.device_mode, "DEFAULT");
			//printf("%s\n", lssdp.header.device_mode);      	     // debug
		}
		// Feature18157:End

		// Bug16158:Start SSDP service getting killed during eth0 interface down
		struct pollfd fds[10];
		int nfds = 1;
		// initialize the pollfd structure
		memset(fds, 0, sizeof(fds));
		// set up the initial listening socket
		fds[0].fd = lssdp.sock;
		fds[0].events = POLLIN;
		int timeout = 0.5 * 1000;   // 0.5 s
		// call poll() and wait 0.5 second for it to complete.
		int ret = poll(fds, nfds, timeout);
		// check to see if the poll call failed.
		if (ret < 0) {
			printf("select error, ret = %d\n", ret);
			break;
		}
		// read data from listening socket
		if (ret > 0) {
			lssdp_socket_read(&lssdp);
			printf("read data from socket\n");
		}
		// Bug16158:End

		// get current time
		long long current_time = get_current_time();
		if (current_time < 0) {
			printf("got invalid timestamp %lld\n", current_time);
			break;
		}
		/*
		 * 1. Create SSDP socket with port 1900
		 * 2. Select SSDP socket with timeout 0.5 seconds
		 *    - when select return value > 0, invoke lssdp_socket_read
		 * 3. Every 5 seconds do:
		 *    - update network interface
		 *    - send M-SEARCH and NOTIFY
		 *    - check neighbor timeout
		 * 4. When neighbor list is changed
		 *    - show neighbor list
		 * 5. When network interface is changed
		 *    - show interface list
		 *    - re-bind the socket
		 */

		// doing task per 1 seconds
		if (current_time - last_time >= 500) {
			lssdp_network_interface_update(&lssdp); // 1. update network interface
			//lssdp_send_msearch(&lssdp);           	// 2. send M-SEARCH
			lssdp_send_notify(&lssdp);              	// 3. send NOTIFY
			//lssdp_neighbor_check_timeout(&lssdp); 	// 4. check neighbor timeout
			last_time = current_time;               	// update last_time
		}
	}

	return EXIT_SUCCESS;
}
