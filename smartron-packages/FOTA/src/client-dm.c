#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <json-c/json.h>
#include <sys/syslog.h>

const int MAXLINE = 1024;
const int SERVER_PORT = 7771;
const int CLIENT_PORT = 7773;

// Driver code 
int main() { 
  int sockfd; 
  char buffer[MAXLINE]; 
  struct sockaddr_in	 servaddr,cliaddr; 
  
  
  // Creating socket file descriptor 
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
    //perror("socket creation failed"); 
    syslog(LOG_DEBUG, "[DM]: Bind failed");
    exit(EXIT_FAILURE); 
  } 
  
  memset(&servaddr, 0, sizeof(servaddr)); 
  memset(&cliaddr, 0, sizeof(cliaddr)); 
  // Filling server information 
  cliaddr.sin_family = AF_INET; 
  cliaddr.sin_port = htons(CLIENT_PORT); 
  cliaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  
  servaddr.sin_family = AF_INET; 
  servaddr.sin_port = htons(SERVER_PORT); 
  servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  
   if ( bind(sockfd, (const struct sockaddr *)&cliaddr,sizeof(cliaddr)) < 0 ) 
  { 
    syslog(LOG_DEBUG, "[DM]:bind failed"); 
    exit(EXIT_FAILURE); 
  }
  int n, len=sizeof(struct sockaddr_in); 
	syslog(LOG_DEBUG, "[DM]: FOTA client DM is connected");
  while(1)
  {
    syslog(LOG_DEBUG, "[DM]: Waiting for FOTA request\n");
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,NULL,NULL); 
    buffer[n] = '\0';
    syslog(LOG_DEBUG, "[DM]: The message from server is [%s]\n",buffer);
  
    json_object * fota_response =  json_object_new_object();
    json_object_object_add(fota_response, "client",json_object_new_int(14));
    json_object_object_add(fota_response, "command",json_object_new_string("FOTA_RES"));
    json_object_object_add(fota_response, "status",json_object_new_int(0));
    json_object_object_add(fota_response, "thing_id",json_object_new_string("ZB5526146540340410"));
    syslog(LOG_DEBUG, "[DM]: [FOTA] The object created is [%s]",json_object_to_json_string(fota_response));
    syslog(LOG_DEBUG,"[DM]: [FOTA] DM_FOTA_RESPONSE server address :[%s][%d] ",inet_ntoa(servaddr.sin_addr), ntohs(servaddr.sin_port));
    

    int res_fota = sendto(sockfd, json_object_to_json_string(fota_response),strlen(json_object_to_json_string(fota_response)),MSG_CONFIRM, (const struct sockaddr *) &servaddr,
						sizeof(servaddr));

  }
  close(sockfd); 
  return 0; 
} 
