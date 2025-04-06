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
	int buffer;
	struct sockaddr_in servAddr;
	socklen_t sizeAddr=sizeof(servAddr);
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(6655);
	client=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)
;	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	printf("Enter number\n");
	scanf("%d",&buffer);
	sendto(client,&buffer,sizeof(buffer),0,(struct sockaddr*)&servAddr,sizeAddr);
	recvfrom(client,&buffer,sizeof(buffer),0,(struct sockaddr*)&servAddr,&sizeAddr);
	printf("Received from server %d",buffer);
	close(client);
	return 0;
}
