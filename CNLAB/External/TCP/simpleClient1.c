#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
int main()
{
	int clientfd;
	struct sockaddr_in client,server;
	socklen_t serverAddrSize=sizeof(server);
	
	clientfd=socket(AF_INET,SOCK_STREAM,0);
	if(clientfd<0)
	{
		printf("Error1\n");
		exit(1);
	}
	server.sin_family=AF_INET;
	server.sin_port=htons(5566);
	server.sin_addr.s_addr=inet_addr("127.0.0.1");


	connect(clientfd,(struct sockaddr*)&server,serverAddrSize);
	char buffer[1024];
	strcpy(buffer,"client message\n");
	send(clientfd,buffer,sizeof(buffer),0);
	recv(clientfd,buffer,sizeof(buffer),0);
	printf("Server says %s\n",buffer);
	close(clientfd);
	return 0;
}
