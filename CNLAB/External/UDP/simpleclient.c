#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main()
{
	int clientfd;
	clientfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(5566);
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	char buffer[1024];
	printf("Enter meassge to be send\n");
	fgets(buffer, sizeof(buffer), stdin);
	socklen_t addrSize=sizeof(server);
	sendto(clientfd,buffer,sizeof(buffer),0,(struct sockaddr*)&server,addrSize);
	recvfrom(clientfd,buffer,sizeof(buffer),0,(struct sockaddr*)&server,&addrSize);
	printf("Message from server %s\n",buffer);
	close(clientfd);
	return 0;
}
