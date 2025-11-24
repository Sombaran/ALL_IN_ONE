#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <stdbool.h>
#define MAXLINE 4096

void xmlReadKeyVal(char *key,int len_val,char *key_val,char *buf)
{
	char *uid = NULL;
	char *loc = NULL;

	loc = strstr(buf,key);
	loc += strlen(key);
	loc[len_val]=0;
	puts(loc);
	if(key_val != NULL)
		strcpy(key_val,loc);
}

bool SendCGICommandToFoscam(char *Ip,int internal_port, char *Cmd, char *UserName, char *Password, char *uid)
{
    int sockfd, n;
    char recvline[MAXLINE];

/*char *buff="GET http://192.168.221.143:88/cgi-bin/CGIProxy.fcgi?cmd=getDDNSConfig&usr=Smartron&pwd=IN@Smart123 \*/
/*HTTP/1.1\r\n\r\n";*/
    char buff[512]="";
    sprintf(buff,"GET http://%s:%d/cgi-bin/CGIProxy.fcgi?cmd=%s&usr=%s&pwd=%s \
HTTP/1.1\r\n\r\n",Ip,internal_port,Cmd,UserName,Password);

    struct sockaddr_in servaddr;
    
    if( (sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1 ){
        printf("socket: error");
        return 0;
    }
    
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(internal_port);  //connect to http server
    
    if(inet_pton(AF_INET,Ip,&servaddr.sin_addr) < 0){
        printf("port: assigned invalid");
        return 0;
    }
    if( connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1){
        printf("connect: error");
        return 0;
    }
    puts(buff);
    if(write(sockfd,buff,strlen(buff)+1) == -1)
        return 0;
    while ((n =read(sockfd,recvline,sizeof(recvline))) > 0){
        recvline[n] = 0;
        if(fputs(recvline,stdout) == EOF)
        {
            printf("Got EOF from socket\n");
            return 1;
        }
    }

    if(uid != NULL)
    {
    	xmlReadKeyVal("<uid>",24,uid,recvline);
    }
    return (EXIT_SUCCESS);
 }
