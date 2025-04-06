#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
int main()
{
	int serverfd;
	serverfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	struct sockaddr_in server,client;
	server.sin_family=AF_INET;
	server.sin_port=htons(5566);
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	socklen_t addrSize=sizeof(server);
	int f=bind(serverfd,(struct sockaddr *)&server,addrSize);
	if(f<0)
	{
		printf("Error1\n");
		exit(1);
	}
	char buffer[1024];
	socklen_t cl=sizeof(client);
	recvfrom(serverfd,buffer,sizeof(buffer),0,(struct sockaddr*)&client,&cl);
	printf("Message from client %s\n",buffer);
	strcpy(buffer,"hey from server:)");
	sendto(serverfd,buffer,sizeof(buffer),0,(struct sockaddr*)&client,cl);
	close(serverfd);
	return 0;
}

