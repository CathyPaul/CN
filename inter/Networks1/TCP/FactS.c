#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
int main()
{
	int server,client;
	int number=0;
	struct sockaddr_in servAddr;
	server=socket(AF_INET,SOCK_STREAM,0);
	socklen_t addrSize=sizeof(servAddr);
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(6655);
	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
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
	client=accept(server,(struct sockaddr*)&servAddr,&addrSize);
	if((client<0))
	{
		printf("Connection faild\n");
		exit(1);
	}
	//memset(buffer, 0, sizeof(buffer));
	recv(client,&number,sizeof(number),0);
	//int bytesReceived = recv(client, buffer, sizeof(buffer) - 1, 0);
	//buffer[bytesReceived]='\0';
	printf("The client gave string %d\n",number);
	number=number*5;
	
	send(client,&number,sizeof(number),0);
	close(server);
	close(client);
	return 0;
}
