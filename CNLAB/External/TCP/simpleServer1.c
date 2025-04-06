#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
int main()
{
	int serverfd,newSock;
	serverfd=socket(AF_INET,SOCK_STREAM,0);
	
	struct sockaddr_in server,client;
	server.sin_family=AF_INET;
	server.sin_port=htons(5566);
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	socklen_t servAddrSize=sizeof(server);
	int s=bind(serverfd,(struct sockaddr *)&server,servAddrSize);
	
	if(s<0)
	{
		printf("Connection failed \n");
		exit(1);
	}
	
	if(listen(serverfd,5)==0)
	{
		printf("Listening\n");
	}
	else
	{
		printf("Error\n");
		exit(1);
	}
	
	socklen_t clientAddrSize=sizeof(client);
	newSock=accept(serverfd,(struct sockaddr *)&client,&clientAddrSize);
	if(newSock<0)
	{
		printf("Connection failed \n");
	
	}
	char buffer[1024];
	recv(newSock,buffer,sizeof(buffer),0);
	printf("Received from client %s\n",buffer);
	strcpy(buffer,"Hello client this is server:)");
	send(newSock,buffer,1024,0);
	close(serverfd);
	return 0;
}
