#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
int main()
{
	int client;
	int number;
	struct sockaddr_in servAddr;
	client=socket(AF_INET,SOCK_STREAM,0);
	socklen_t addrSize=sizeof(servAddr);
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(6655);
	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	connect(client,(struct sockaddr*)&servAddr,sizeof(servAddr));
	printf("enter number\n");
	scanf("%d",&number);
	send(client,&number,sizeof(number),0);
	//bzero(buffer,1024);
	recv(client,&number,sizeof(number),0);
	printf("The reverse gave string %d\n",number);
	close(client);
	return 0;
}
