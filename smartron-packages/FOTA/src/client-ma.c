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
const int CLIENT_PORT = 7772;

// Driver code 
int main() { 
  int sockfd; 
  char buffer[MAXLINE]; 
  char *hello = "Hello from client"; 
  struct sockaddr_in	 servaddr,cliaddr; 
  
  json_object *jobj = json_object_new_object();
  json_object *jclient = json_object_new_int(13);
  json_object *jrequest = json_object_new_string("FOTA_REQ");
  json_object *jthing_id=json_object_new_array();
  json_object_object_add(jobj,"client",jclient);
  json_object_object_add(jobj,"command",jrequest); 
  json_object_array_add(jthing_id,json_object_new_string("ZB5526146540340410"));
  json_object_object_add(jobj,"thing_id",jthing_id); 

  syslog(LOG_DEBUG, "[MA]: The object created is [%s]",json_object_to_json_string(jobj));
  
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
    syslog(LOG_DEBUG, "[MA]:bind failed"); 
    exit(EXIT_FAILURE); 
  }
  int n, len=sizeof(struct sockaddr_in); 
  while(1)
  {
    syslog(LOG_DEBUG,"[MA]: [FOTA] MA_FOTA_REQUEST server address :[%s][%d] ",inet_ntoa(servaddr.sin_addr), ntohs(servaddr.sin_port));
    
    int res_fota = sendto(sockfd, json_object_to_json_string(jobj),strlen(json_object_to_json_string(jobj)),MSG_CONFIRM, (const struct sockaddr *) &servaddr,
						sizeof(servaddr));
    syslog(LOG_DEBUG, "[MA]: [FOTA] waiting for FOTA ACK\n");
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,NULL,NULL); 
    buffer[n] = '\0';
    syslog(LOG_DEBUG, "[MA]: [FOTA] The message from server is [%s]\n",buffer);
    
    getchar();
  }
  close(sockfd); 
  return 0; 
} 
