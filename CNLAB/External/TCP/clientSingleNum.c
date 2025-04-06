#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main()
{
	int clientfd;
	struct sockaddr_in server;
	clientfd=socket(AF_INET,SOCK_STREAM,0);
	server.sin_family=AF_INET;
	server.sin_port=htons(5566);
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	socklen_t addrLen=sizeof(server);
	connect(clientfd,(struct sockaddr*)&server,addrLen);
	int num=25;
	send(clientfd,&num,sizeof(num),0);
	char buffer[1024];
	recv(clientfd,buffer,sizeof(buffer),0);
	printf("Server says %s\n",buffer);
	close(clientfd);
	return 0;
}
