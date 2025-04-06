#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
int main()
{
	int client;
	struct sockaddr_in servAddr;
	socklen_t addrSize=sizeof(servAddr);
	char buffer[1024];
	client=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(6655);
	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	strcpy(buffer,"Hello from client\n");
	sendto(client,buffer,sizeof(buffer),0,(struct sockaddr*)&servAddr,addrSize);
	recvfrom(client,buffer,sizeof(buffer),0,(struct sockaddr*)&servAddr,&addrSize);
	printf("Server says %s\n",buffer);
	close(client);
	return 0;
}
