#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/types.h>
#include <json-c/json.h>
#include <time.h>
#include "foscam_sqlite3.h"
#include <sys/syslog.h>
/*#include "fw-env.h"*/
#include <errno.h>

#define BUFFER_SIZE  255
#define BROADCAST_IP  "192.168.221.255"
#define BROADCAST_PORT  10000	
#define LENGTH_NAME_CAMERA  20
#define MAX_NETWORK_NUM_CAMERAS 1
#define MAC_LENGTH 12	//camera mac address length
#define OFFSET_NAME 36
#define OFFSET_MAC 23
#define OFFSET_PORT 75

bool RemoveFirewallEntry(char *ip);

bool SendCGICommandToFoscam(char *Ip,int internal_port, char *Cmd, char *UserName, char *Password, char *uid);

int GetRandomPort(int Divisor) {
   time_t t;
   int number = 0;
   /* Intializes random number generator */
   srand((unsigned) time(&t));

   for(;;) {
      number = rand()%Divisor;
      printf("%d\n", number);
      if(number != 8080 && CheckPortNumberExistOrNot(number))
      	return number;
   }
   
   return 0;
}

//Once received response from camera, the data will be saved in followig staructure.
typedef struct {
	char name[20];
	uint8_t IP[16];
	char mac[25];
	uint8_t port;
	int external_port;
	char uid[25];
} Camera;
Camera fc;
/*Finds next available entry for the the static entry.
Takes camera mac as argument and returns 255 if that mac already exists in static ip list*/
char FindNextAvailableHost(char *mac) {
	char i = 0;
	char buf[100] = { 0 };
	char MAC[17];
	FILE *p = NULL;
	printf("FindNextAvailableHost mac: %s\n", mac);

	sprintf(MAC, "%c%c:%c%c:%c%c:%c%c:%c%c:%c%c", mac[0], mac[1], mac[2],
			mac[3], mac[4], mac[5], mac[6], mac[7], mac[8], mac[9], mac[10],
			mac[11]);
	for (i = 0; i < 4; i++) {
		//sprintf(buf,"uci show dhcp.@host[%d]",i);
		sprintf(buf, "uci show dhcp.@host[%d].mac", i);
		p = popen(buf, "r");

		if (p) {
			fgets(buf, sizeof(buf), p);
			printf("buf=%s\n", buf);
			
			//checking whether this mac has alredy allocated static ip
			if (buf[0] == 'd' && strstr(buf, MAC) != NULL && MacIDPresence(mac)) {
				printf("This MAC already exist!\n");
				syslog(LOG_DEBUG, "IC: FindNextAvailableHost This MAC already exist");
				return 255;
			} else if (buf[0] == 'u') {//if we got response like uci error... means there is no entry with this index i
				printf("available host found at %d\n", i);
				syslog(LOG_DEBUG, "IC: FindNextAvailableHost available host found at %d",i);
				pclose(p);
				return i;
			}
			/*else {
			 printf("available host found at %d\n", i);
			 pclose(p);
			 return i;
			 }*/
		}

		pclose(p);
	}
	printf("No available hosts found, Exiting....\n");
	return 0;
}

/*Finds next available index for firewall entry.
Takes camra ip as argument and returns index if no firewall entry exist with that ip else return 255*/
char FindNextAvailableFirewallEntry(char *ip)
{
	char i = 0;
	char buf[100] = { 0 };
	FILE *p = NULL;
	printf("FindNextAvailableFirewallEntry ip: %s\n", ip);
	
	for (i = 0; i < 4; i++) {
		sprintf(buf, "uci show firewall.@redirect[%d].dest_ip", i);
		p = popen(buf, "r");

		if (p) {
			fgets(buf, sizeof(buf), p);
			printf("buf=%s\n", buf);
			
			//checking whether this ip has already allocated firewall entry.
			if (buf[0] == 'f' && strstr(buf, ip) != NULL) {
				printf("This ip already exist in firewall!\n");
				syslog(LOG_DEBUG, "IC: This ip already exist in firewall");
				return 255;
			} else if (buf[0] == 'u') {//if we got response like uci error... means there is no entry with this index i
				printf("available index found for firewall at %d\n", i);
				pclose(p);
				return i;
			}
		}

		pclose(p);
	}
	printf("No available index found for firewall, Exiting....\n");
	return 0;
}

/*It adds new firewall entry for port forwarding if the ip is not already exist in firewall entry.
These are adding as firewall entry for port forwarding.
	option target 'DNAT'
	option src 'wan'
	option dest 'lan'
	option proto 'tcp udp'
	option src_dport '8080'
	option dest_ip '192.168.221.233'
	option dest_port '88'
	option name 'cam1'*/
bool AddNewFirewallEntry(Camera cam)
{
	char const available_entry = FindNextAvailableFirewallEntry(cam.IP);
	printf("called firewall");
	if (available_entry == 255)
		return false;
	char buf[100] = { 0 };
	char mac[25] = { 0 };
	FILE *p = NULL;

	printf("\nAvailable host=%d\n", available_entry);

	sprintf(buf, "cat /tmp/dhcp.leases | grep %s | awk '{print $2}'", cam.IP);
	p = popen(buf, "r");
	fgets(mac, sizeof(mac), p);
	mac[17] = '\0';
	printf("\nDHCP Lease MAC:%s\n", mac);
	
	if (!system("uci add firewall redirect"))
		printf("uci add firewall redirect");
	
	sprintf(buf, "uci set firewall.@redirect[%d].target='DNAT'", available_entry);
	if (!system(buf))
		printf(buf);
	
	sprintf(buf, "uci set firewall.@redirect[%d].src=wan", available_entry);
	if (!system(buf))
		printf(buf);
	
	sprintf(buf, "uci set firewall.@redirect[%d].dest=lan", available_entry);
	if (!system(buf))
		printf(buf);
		
	sprintf(buf, "uci set firewall.@redirect[%d].proto='tcp udp'", available_entry);
	if (!system(buf))
		printf(buf);
	
	sprintf(buf, "uci set firewall.@redirect[%d].src_dport=%d", available_entry,fc.external_port);
	if (!system(buf))
		printf(buf);
	
	sprintf(buf, "uci set firewall.@redirect[%d].dest_ip=%s", available_entry,cam.IP);
	if (!system(buf))
		printf(buf);
	
	sprintf(buf, "uci set firewall.@redirect[%d].dest_port=%d", available_entry,cam.port);
	if (!system(buf))
		printf(buf);
	
	sprintf(buf, "uci set firewall.@redirect[%d].name=%s", available_entry,cam.name);
	if (!system(buf))
		printf(buf);
	
	if (!system("uci commit firewall"))
		printf("uci commit firewall");

	return true;
}

/*Deallocates Static Ip based on mac*/
/*bool RemoveStaticIP(char *mac) */
bool RemoveFoscam(const char *mac) {
	char i = 0;
	char buf[100] = { 0 };
	char MAC[17];
	FILE *p = NULL;
	printf("RemoveStaticIP MAC: %s\n", mac);

	sprintf(MAC, "%c%c:%c%c:%c%c:%c%c:%c%c:%c%c", mac[0], mac[1], mac[2],
			mac[3], mac[4], mac[5], mac[6], mac[7], mac[8], mac[9], mac[10],
			mac[11]);
	for (i = 0; i < 4; i++) {
		//sprintf(buf,"uci show dhcp.@host[%d]",i);
		sprintf(buf, "uci show dhcp.@host[%d].mac", i);
		p = popen(buf, "r");

		if (p) {
			fgets(buf, sizeof(buf), p);
			printf("buf=%s\n", buf);
			
			if (buf[0] == 'd' && strstr(buf, MAC) != NULL) {
				sprintf(buf,"uci delete dhcp.@host[%d] && uci commit",i);
				system(buf);
				printf("Removed static IP");
				RemoveFirewallEntry(GetIpAddress(mac));
				DeleteRecord(mac);
/*				json_object *remove = json_object_new_object();*/
/*				json_object_object_add(remove, "msg_id", json_object_new_string("12345"));*/
/*				json_object_object_add(remove, "thing_id", json_object_new_string(mac));*/
/*				json_object_object_add(remove, "property_name", json_object_new_string("REMOVE"));*/
/*				json_object_object_add(remove, "Error_code", json_object_new_string("Success"));*/
				return true;
			}
		pclose(p);
	}
}
	printf("No camera found with MAC: %s\n",MAC);
	return false;
}

/*Removes firewall entry based on ip*/
bool RemoveFirewallEntry(char *ip) {
	char i = 0;
	char buf[100] = { 0 };
	char MAC[17];
	FILE *p = NULL;
	printf("\nRemoveFirewallEntry IP: %s\n", ip);

	for (i = 0; i < 4; i++) {
		sprintf(buf, "uci show firewall.@redirect[%d].dest_ip", i);
		p = popen(buf, "r");

		if (p) {
			fgets(buf, sizeof(buf), p);
			printf("buf=%s\n", buf);

			if (buf[0] == 'f' && strstr(buf, ip) != NULL) {
				sprintf(buf,"uci delete firewall.@redirect[%d] && uci commit",i);
				system(buf);
				printf("Removed Firewall Entry");
				pclose(p);
				return true;
			}
	}
}
	printf("No camera found with IP: %s\n", ip);
	return false;
}

/*Makes camera ip as static ip*/
bool AddNewStaticIpEntry(Camera cam) {
	char const available_entry = FindNextAvailableHost(cam.mac);
	if (available_entry == 255)
		return false;
	char buf[100] = { 0 };
	char mac[25] = { 0 };
	FILE *p = NULL;

	printf("\nAvailable host=%d\n", available_entry);

	sprintf(buf, "cat /tmp/dhcp.leases | grep %s | awk '{print $2}'", cam.IP);
	p = popen(buf, "r");
	fgets(mac, sizeof(mac), p);
	mac[17] = '\0';
	printf("\nDHCP Lease MAC:%s\n", mac);
	if (!system("uci add dhcp host"))
		printf("uci add dhcp host");
	sprintf(buf, "uci set dhcp.@host[%d].ip=%s", available_entry, cam.IP);
	printf("%s\n", buf);
	if (!system(buf))
		printf(buf);
	sprintf(buf,
			"uci set dhcp.@host[%d].mac='%s %c%c:%c%c:%c%c:%c%c:%c%c:%c%c'",
			available_entry, mac, cam.mac[0], cam.mac[1], cam.mac[2],
			cam.mac[3], cam.mac[4], cam.mac[5], cam.mac[6], cam.mac[7],
			cam.mac[8], cam.mac[9], cam.mac[10], cam.mac[11]);
	printf("%s\n", buf);
	if (!system(buf))
		printf(buf);
	sprintf(buf, "uci set dhcp.@host[%d].name=%s", available_entry, cam.name);
	printf("%s\n", buf);
	if (!system(buf))
		printf(buf);
	if (!system("uci commit dhcp"))
		printf("uci commit dhcp");

	return AddNewFirewallEntry(cam);
}

json_object *CreateJsonProperty(char *property_name, char *property_type, char *property_value) {
	json_object *property = json_object_new_object();
	json_object *property_range = json_object_new_object();
	
	json_object_object_add(property, "property_name", json_object_new_string(property_name));
	json_object_object_add(property, "property_type", json_object_new_string(property_type));
	json_object_object_add(property, "property_value", json_object_new_string(property_value));
	
	json_object_object_add(property_range, "max", json_object_new_int(0));
	json_object_object_add(property_range, "min", json_object_new_int(0));
	
	json_object_object_add(property_range,"list",json_object_new_array());
	json_object_object_add(property_range, "step", json_object_new_int(0));
	json_object_object_add(property_range, "unit", json_object_new_int(0));
	
	json_object_object_add(property, "property_range", property_range);
	
	json_object *jarray = json_object_new_array();
	json_object_array_put_idx(jarray,0, property);
	printf("JSON_OBJECT(CreateJsonProperty): %s\n",
			json_object_to_json_string(jarray));
	
	return property;
}

json_object *FormatJsonData(char *p, char *IP) {
	json_object *camera = json_object_new_object();
	char buf[50] = { 'F','C' };

	memcpy(buf+2, p + OFFSET_MAC, 12);
	buf[14] = '\0';

	json_object_object_add(camera, "thing_id", json_object_new_string(buf));
	

	json_object_object_add(camera, "thing_name", json_object_new_string("Foscam"));
	json_object_object_add(camera, "thing_type", json_object_new_string("IP Camera"));
	json_object_object_add(camera, "firmware_version", json_object_new_string("1.0"));
	json_object_object_add(camera, "framework", json_object_new_string("WiFi"));
	json_object_object_add(camera, "hardware_version", json_object_new_string("2"));
	json_object_object_add(camera, "manufacturer_name", json_object_new_string("Foscam"));
	json_object_object_add(camera, "manufacturer_url", json_object_new_string("www.Foscam.com"));
	json_object_object_add(camera, "thing_status", json_object_new_string("online"));
	json_object_object_add(camera, "battery_status", json_object_new_string("null"));
	
	json_object *entities = json_object_new_array();
	json_object *jobj = json_object_new_object();
	
	json_object_object_add(jobj, "entity_id", json_object_new_string("1"));
	
	memcpy(buf, p + OFFSET_NAME, LENGTH_NAME_CAMERA);
	buf[LENGTH_NAME_CAMERA] = '\0';

	json_object_object_add(jobj, "entity_name", json_object_new_string(buf));
	json_object_object_add(jobj, "entity_type", json_object_new_string("IP Camera"));
	
	json_object *properties = json_object_new_array();
	
	json_object_array_put_idx(properties,0, CreateJsonProperty("USER_ID","CREDENTIAL",fc.uid));
	json_object_array_put_idx(properties,1, CreateJsonProperty("PASSWORD","CREDENTIAL","IN@Smart123"));
	
	memcpy(buf, IP, strlen(IP) + 1);

	json_object_array_put_idx(properties,2, CreateJsonProperty("IP","ADDRESS",buf));
	sprintf(buf,"%d",fc.external_port);
	json_object_array_put_idx(properties,3, CreateJsonProperty("EXTERNAL_PORT","PORT",buf));
	json_object_array_put_idx(properties,4, CreateJsonProperty("INTERNAL_PORT","PORT","88"));
	json_object_array_put_idx(properties,5, CreateJsonProperty("USER_NAME","CREDENTIAL_USER_NAME","Smartron"));
	
	json_object_object_add(jobj, "properties", properties);
	
	json_object_array_put_idx(entities,0, jobj);
	json_object_object_add(camera, "entities", entities);
	
	printf("JSON_OBJECT(FormatJsonData): \n\n%s\n",
			json_object_to_json_string(camera));

	return camera;
}

/*Broadcasts and waits for 1 sec for response from the camera. 
And repeats same 10 times, if it got response meanwhile it skips sending broadcast and returns discovery data.*/
json_object* FoscamSearch(int discTimeout) {
	uint8_t msg1[27] = { 0x4d, 0x4f, 0x5f, 0x49, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };
	uint8_t msg2[5] = { 0xb4, 0x9a, 0x70, 0x4d, 0x00 };
	int s = -1;
	char buf[512] = { 0 };
	char broadcastip[16] = { 0 }, i = 0;
	FILE *p = NULL;
	json_object *json_main = json_object_new_object();
	
	discTimeout -= 5;
	
	syslog(LOG_DEBUG, "IC: FoscamSearch called");
	
	//Getting broadcast ip from br-lan interface
	p = popen("echo $(ifconfig br-lan|grep 'inet addr')|cut -b 11-24", "r");

	fgets(broadcastip, sizeof(broadcastip), p);
/*	if (broadcastip[0] == '\0') {*/
/*		printf("br-lan \n");*/
/*		exit(1);*/
/*	}*/
	//Constructing broadcast ip by appending 255 as 4th octet
	for (char j = 0; j < 3; j++)
		while (broadcastip[i++] != '.')
			;
	strcpy(broadcastip + i, "255");	//copying 255 as 4th octet
	puts("BroadcastIP:");
	puts(broadcastip);

	struct sockaddr_in broadcastAddr;

	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		perror("Can't open UDP socket");
		return false;
	}
	int broadcastPermission = 1;
	if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission,
			sizeof(broadcastPermission)) < 0) {
		perror("setsockopt error");
		return false;
	}
	/* Construct local address structure */
	memset(&broadcastAddr, 0, sizeof(broadcastAddr));
	broadcastAddr.sin_family = AF_INET;
	broadcastAddr.sin_addr.s_addr = inet_addr(broadcastip);
	broadcastAddr.sin_port = htons(BROADCAST_PORT);

	char name_camera[LENGTH_NAME_CAMERA + 1];
	int len = sizeof(broadcastAddr);
	int ret = 0;
	int num_cameras_detected = 0;
	fd_set fdMaskRead;
	fd_set fdMaskError;
	struct timeval timeout;
	// If in 1 seconds nothing is received, no Foscam cameras in network.
	/*	timeout.tv_sec = 1;*/
	/*	timeout.tv_usec = 0;*/
	//Broadcasting on network untill finding new camera in the network and maximum 10 times
	for (char loop = 0;
			loop < discTimeout && num_cameras_detected < MAX_NETWORK_NUM_CAMERAS;
			loop++) {
		//while (num_cameras_detected < MAX_NETWORK_NUM_CAMERAS) {
		/*********** Wait for reply ***********/
		if (sendto(s, msg1, sizeof(msg1), 0, (struct sockaddr *) &broadcastAddr,
				sizeof(broadcastAddr)) == -1) {
			perror("Error sending message 1");
			return false;
		}

		//send the message 2
		if (sendto(s, msg2, sizeof(msg2), 0, (struct sockaddr *) &broadcastAddr,
				sizeof(broadcastAddr)) == -1) {
			perror("Error sending message 2");
			return false;
		}
		printf("Req sent, Waiting for reply..\n");
/*		syslog(LOG_DEBUG, "IC: Req sent, Waiting for reply");*/
		memset(buf, 0x00, BUFFER_SIZE);
		FD_ZERO(&fdMaskRead);
		FD_ZERO(&fdMaskError);
		FD_SET(s, &fdMaskRead);
		FD_SET(s, &fdMaskError);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		
		ret = select(s + 1, &fdMaskRead, NULL, &fdMaskError, &timeout);

		if (ret < 0) {
			syslog(LOG_DEBUG, "IC: Error to call method select: %s",strerror(errno));
			perror("Error to call method select: ");
			continue;//return false;
		} else if (ret == 0) {
			// Timeout done
			printf("Timeout done for %d iteration\n", loop);
			//syslog(LOG_DEBUG, "IC: Timeout done for %d iteration",loop);
			//break;
		} else {
			if (FD_ISSET(s, &fdMaskRead)) {
				// Change value
				ret = recvfrom(s, buf, BUFFER_SIZE, 0,
						(struct sockaddr *) &broadcastAddr, (socklen_t*) &len);

				if (ret == 0) {
					// No data
					syslog(LOG_DEBUG, "IC: No data rxd");
					continue;
				}

				if (ret == -1) {
					syslog(LOG_DEBUG, "IC: Error receiving data");
					perror("Error receiving data: ");
					return NULL;
				}

				if (ret != 129) {
					syslog(LOG_DEBUG, "IC: Unknown response. Not Foscam response");
					printf("Unknown response. Not Foscam response.\n");
					return NULL;
				}

				/*memcpy(name_camera, buf + offset_name, LENGTH_NAME_CAMERA);
				 name_camera[LENGTH_NAME_CAMERA] = '\0';
				 port_camera = buf[offset_port];*/

				memcpy(fc.name, buf + OFFSET_NAME, LENGTH_NAME_CAMERA);
				fc.name[LENGTH_NAME_CAMERA] = '\0';

				memcpy(fc.mac, buf + OFFSET_MAC, 12);
				fc.mac[12] = '\0';
				fc.port = buf[OFFSET_PORT];
				char *p = inet_ntoa(broadcastAddr.sin_addr);
				memcpy(fc.IP, p, strlen(p) + 1);
				fc.external_port = GetRandomPort(10000);
				printf("name: %s\nIP: %s\nPort: %d\next_port: %d\nMac: %s\n", fc.name, fc.IP,
						fc.port, fc.external_port, fc.mac);
				CreateTable();
				if(MacIDPresence(fc.mac) == false)
				{
					if (AddNewStaticIpEntry(fc)) {
						syslog(LOG_DEBUG, "IC: New camera discovered");
						InsertTable(fc.mac, fc.IP, fc.port,fc.external_port);
					
						FormatJsonData(buf,
								inet_ntoa(broadcastAddr.sin_addr));
							
						//Set Default UserId and Password
						SendCGICommandToFoscam(fc.IP,fc.port, "changeUserNameAndPwdTogether&usrName=admin&newUsrName=Smartron&oldPwd=&newPwd=IN@Smart123&usr=admin&pwd=", "", "",NULL);
						SendCGICommandToFoscam(fc.IP,fc.port, "getP2PInfo", "Smartron", "IN@Smart123",fc.uid);
						printf("UID: %s",fc.uid);
						
						num_cameras_detected++;
						json_object *jobj = json_object_new_object();
						jobj = FormatJsonData(buf,
								inet_ntoa(broadcastAddr.sin_addr));

						json_object_object_add(json_main, "Cam", jobj);
						printf("JSON_OBJECT(main): %s\n",
								json_object_to_json_string(json_main));
					
						json_object *jarray_discovery = json_object_new_array();
						json_object *things = json_object_new_object();
					
						json_object_array_put_idx(jarray_discovery,0,jobj);
						json_object_object_add(things, "things", jarray_discovery);
						return things;
					}
				}
				else
				{
/*					syslog(LOG_DEBUG, "IC: This mac already exist in DB");*/
					usleep(1000*500);
				}
				bzero(buf, sizeof(buf));
				bzero(&fc, sizeof(fc));
			}
		}
	}
	close(s);
	return NULL;
}

