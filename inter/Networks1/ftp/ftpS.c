#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main()
{
	int server,client;
	char buffer[1024];
	struct sockaddr_in servAddr;
	socklen_t s=sizeof(servAddr);
	server=socket(AF_INET,SOCK_STREAM,0);
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(6655);
	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	bind(server,(struct sockaddr*)&servAddr,s);
	if(listen(server,5)==0)
	{
		printf("Listening\n");
	}
	else
	{
		printf("Error\n");
		exit(1);
	}
	client=accept(server,(struct sockaddr*)&servAddr,&s);
	recv(client,buffer,sizeof(buffer),0);
	if(buffer!='\0')
	{
		FILE *fp=fopen(buffer,"r");
		if(fp!=NULL)
		{
			while(fgets(buffer,sizeof(buffer),fp)!=NULL)
			{
				send(client, buffer, sizeof(buffer), 0);
            			memset(buffer, 0,sizeof(buffer));
			}
			fclose(fp);
		}
	}
	send(client,buffer,sizeof(buffer),0);
	close(client);
	close(server);
	return 0;
}
