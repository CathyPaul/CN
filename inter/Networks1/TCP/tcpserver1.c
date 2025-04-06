#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void main()
{
	int server,newSock;
	char buffer[1024];
	struct sockaddr_in servAddr;
	server=socket(AF_INET,SOCK_STREAM,0);
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(6265);
	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	bind(server,(struct sockaddr*)&servAddr,sizeof(servAddr));
	socklen_t sizeAddr;
	if(listen(server,5)==0)
	{
		printf("Listening\n");
	}
	else
	{
		printf("Error\n");
		exit(1);
	}
	newSock=accept(server,(struct sockaddr*)&servAddr,(socklen_t *)&sizeAddr);
	recv(newSock,buffer,1024,0);
	printf("Received grom client %s\n",buffer);
	strcpy(buffer,"Got It \n");
	send(newSock,buffer,1024,0);
	close(newSock);
	close(server);
}
