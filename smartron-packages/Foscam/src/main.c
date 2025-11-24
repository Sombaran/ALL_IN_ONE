#include <stdio.h>
#include "Foscam.h"
#include "foscam_sqlite3.h"

bool SendCGICommandToFoscam(char *Ip,int internal_port, char *Cmd, char *UserName, char *Password,char *);

int main(void) {
	int input = 0;
	char uid[25];
	while (1) {
		printf("\n1. Discovery\n2. Remove\n3. CGI Execution\n4. Get uid\n");
		scanf("%d", &input);

		switch (input) {
		case 1: printf("JSON_OBJECT(Discovery): %s\n",
							json_object_to_json_string(FoscamSearch(55)));
		break;
		case 2:
			{
				printf("Enter camera MAC:");
				char buf[20]={0};
				scanf("%s",buf);
				if(MacIDPresence(buf))
				{
					printf("RemoveFoscam: %d\n",
								(RemoveFoscam(buf)));
/*					printf("Enter IP for removing Firewall Entry: ");*/
	/*				scanf("%s",buf);*/
/*					RemoveFirewallEntry(GetIpAddress(buf));*/
/*					DeleteRecord(buf);*/
				}
				else
					printf("No data exist with Mac: %s\n",buf);
			}
		break;
		case 3: SendCGICommandToFoscam("192.168.221.143",88,"getDDNSConfig","admin","","");
		break;
		
		case 4: SendCGICommandToFoscam("192.168.221.143",88,"getP2PInfo","Smartron","IN@Smart123",uid);
		printf("main: %s\n",uid);
		break;
			
		}
	}

}
