#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
int main()
{
	int serverfd,clientfd;
	struct sockaddr_in server,client;
	
	serverfd=socket(AF_INET,SOCK_STREAM,0);
	server.sin_family=AF_INET;
	server.sin_port=htons(5566);
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	socklen_t servAddrLen=sizeof(server);
	int s=bind(serverfd,(struct sockaddr*)&server,servAddrLen);
	/*if(s<0)
	{
		printf("Error1");
		exit(1);
	}*/
	if(listen(serverfd,5)==0)
	{
		printf("Listening\n");
		
	}		
	else
	{
		printf("Error2");
		exit(1);
	}
	socklen_t cl=sizeof(client);
	clientfd=accept(serverfd,(struct sockaddr*)&client,&cl);
	if(clientfd<0)
	{
		printf("Error3");
		exit(1);
	}
	char buffer[1024];
	recv(clientfd,buffer,sizeof(buffer),0);
	int n=atoi(buffer);
	if(n%2==0)
	{
		strcpy(buffer,"Even");
	}
	else
	{
		strcpy(buffer,"Odd");
	}
	send(clientfd,buffer,sizeof(buffer),0);
	close(serverfd);
	close(clientfd);
	return 0;
}
