#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void main()
{
	int client;
	char buffer[1024];
	struct sockaddr_in servAddr;
	client=socket(AF_INET,SOCK_STREAM,0);
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(6265);
	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	connect(client,(struct sockaddr*)&servAddr,sizeof(servAddr));
	socklen_t sizeAddr;
	strcpy(buffer,"hey server this is client\n");
	send(client,buffer,1024,0);
	recv(client,buffer,1024,0);
	printf("Received grom server %s\n",buffer);
	close(client);
}
