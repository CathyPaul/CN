#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
int main()
{
	int client;
	char buffer[1024];
	struct sockaddr_in servAddr;
	client=socket(AF_INET,SOCK_STREAM,0);
	socklen_t addrSize=sizeof(servAddr);
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(6655);
	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	connect(client,(struct sockaddr*)&servAddr,sizeof(servAddr));
	printf("enter string\n");
	scanf("%s",buffer);
	send(client,buffer,strlen(buffer),0);
	bzero(buffer,1024);
	recv(client,buffer,sizeof(buffer),0);
	printf("The reverse gave string %s\n",buffer);
	close(client);
	return 0;
}
