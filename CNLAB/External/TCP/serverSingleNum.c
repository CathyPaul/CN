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
	if(s<0)
	{
		printf("Error1");
		exit(1);
	}
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
	int num=0;
	recv(clientfd,&num,sizeof(num),0);
	char buffer[1024];
	sprintf(buffer, "The number is %d", num);   // Convert number to string
	send(clientfd,buffer,sizeof(buffer),0);
	close(serverfd);
	close(clientfd);
	return 0;
}
