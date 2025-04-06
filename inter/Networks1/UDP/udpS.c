#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
	int server,client;
	struct sockaddr_in servAddr,clientAddr;
	socklen_t sizeA=sizeof(clientAddr);
	char buffer[1024]={0};
	server=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(server<0)
	{
		printf("error");
		exit(1);
	}
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(6655);
	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	if(bind(server,(struct sockaddr*)&servAddr,sizeof(servAddr))<0)
	{
		printf("E\n");
		exit(1);
	}
	recvfrom(server,buffer,sizeof(buffer),0,(struct sockaddr*)&clientAddr,&sizeA);
	printf("Received from client %s\n",buffer);
	strcpy(buffer,"got it\n");
	sendto(server,buffer,sizeof(buffer),0,(struct sockaddr*)&clientAddr,sizeA);
	close(client);
	close(server);
	return 0;
}
