#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void main()
{
	int client;
	struct sockaddr_in servAddr;
	char serverMsg[1024],clientMsg[1024];
	client=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);	
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(6265);
	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	socklen_t cllen=sizeof(servAddr);
	
	strcpy(clientMsg,"hey this is client\n");
	sendto(client,clientMsg,sizeof(clientMsg),0,(struct sockaddr*)&servAddr,cllen);
	recvfrom(client,serverMsg,sizeof(serverMsg),0,(struct sockaddr*)&servAddr,&cllen);
	printf("Received %s from server\n",serverMsg);
	close(client);
}
