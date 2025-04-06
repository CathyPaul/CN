#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main()
{
	int server,newSock;
	char buffer[1024];
	server=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in servAddr;
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(6255);
	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	socklen_t addrSize;
	bind(server,(struct sockaddr*)&servAddr,sizeof(servAddr));
	if(listen(server,5)==0)
	{
		printf("Listening\n");
	}
	else
	{
		printf("Error\n");
		exit(1);
	}
	newSock=accept(server,(struct sockaddr*)&servAddr,&addrSize);
	recv(newSock,buffer,1024,0);
	printf("Client says %s\n",buffer);
	for(int i=0;i<strlen(buffer)/2;i++)
	{
		char temp=buffer[i];
		buffer[i]=buffer[strlen(buffer)-i-1];
		buffer[strlen(buffer)-i-1]=temp;
	}
	send(newSock,buffer,1024,0);
	close(newSock);
	close(server);
	return 0;
}
