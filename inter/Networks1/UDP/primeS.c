#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
	int server;
	int buffer;
	struct sockaddr_in servAddr,clientAddr;
	socklen_t sizeAddr=sizeof(clientAddr);
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(6655);
	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	server=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	bind(server,(struct sockaddr*)&servAddr,sizeof(servAddr));
	if(server<0)
	{
		printf("Error1\n");
		exit(1);
	}
	recvfrom(server,&buffer,sizeof(buffer),0,(struct sockaddr*)&clientAddr,&sizeAddr);
	printf("Received from client %d",buffer);
	buffer=buffer*10;
	sendto(server,&buffer,sizeof(buffer),0,(struct sockaddr*)&clientAddr,sizeAddr);
	close(server);
	return 0;
}
