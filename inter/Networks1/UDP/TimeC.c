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
	int client;
	struct sockaddr_in servAddr;
	char buffer[1024];
	client=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(6655);
	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	socklen_t s=sizeof(servAddr);
	strcpy(buffer,"time_request\n");
	sendto(client,buffer,sizeof(buffer),0,(struct sockaddr*)&servAddr,s);
	recvfrom(client,buffer,sizeof(buffer),0,(struct sockaddr*)&servAddr,&s);
	printf("Server says %s \n",buffer);
	close(client);
	return 0;
}
