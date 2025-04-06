#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main()
{
	int client;
	char buffer[1024];
	struct sockaddr_in servAddr;
	socklen_t s=sizeof(servAddr);
	client=socket(AF_INET,SOCK_STREAM,0);
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(6655);
	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	connect(client,(struct sockaddr*)&servAddr,s);
	printf("Enter file name\n");
	scanf("%s",buffer);
	send(client,buffer,sizeof(buffer),0);
	recv(client,buffer,sizeof(buffer),0);
	printf("%s\n",buffer);
	close(client);

	return 0;
}
