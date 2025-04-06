#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
int main()
{
	int server;
	struct sockaddr_in servAddr,clientAddr;
	char buffer[1024];
	server=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(6655);
	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	socklen_t s=sizeof(clientAddr);
	bind(server,(struct sockaddr*)&servAddr,sizeof(servAddr));
	recvfrom(server,buffer,sizeof(buffer),0,(struct sockaddr*)&clientAddr,&s);
	printf("Client says %s \n",buffer);
	time_t current_time=time(NULL);
	char *str_time=ctime(&current_time);
	sendto(server,str_time,strlen(str_time),0,(struct sockaddr*)&clientAddr,s);
	close(server);
	return 0;
}
