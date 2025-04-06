#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void main()
{
	int server;
	struct sockaddr_in servAddr,clientAddr;
	char serverMsg[1024],clientMsg[1024];
	server=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);	
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(6265);
	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	bind(server,(struct sockaddr*)&servAddr,sizeof(servAddr));
	socklen_t cllen=sizeof(clientAddr);
	recvfrom(server,clientMsg,sizeof(clientMsg),0,(struct sockaddr*)&clientAddr,&cllen);
	printf("Received %s from client\n",clientMsg);
	strcpy(serverMsg,"hey this is server i got you\n");
	sendto(server,serverMsg,sizeof(serverMsg),0,(struct sockaddr*)&clientAddr,cllen);
	close(server);
}
