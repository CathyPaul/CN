#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define buffersize 1024
#define lossprobability 30

typedef struct packet
{
	char data[buffersize];
}Packet;
typedef struct frame
{
	int frame_kind;
	int ack;
	int seqno;
	Packet packet;
}Frame;
int main(int argc, char** argv)
{
	if(argc!=2)
	{
		printf("Enter port number \n");
		exit(1);
	}
	srand(time(NULL));
	int expectedFrame=0;
	Frame recvFrame,sendFrame;
	int port=atoi(argv[1]);
	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in servAddr,clientAddr;
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(port);
	servAddr.sin_addr.s_addr=INADDR_ANY;
	bind(sockfd,(struct sockaddr*)&servAddr,sizeof(servAddr));
	socklen_t addrsize=sizeof(clientAddr);
	while(1)
	{
		int recvlen=recvfrom(sockfd,&recvFrame,sizeof(recvFrame),0,(struct sockaddr*)&clientAddr,&addrsize);
		if(recvlen<=0)
		{
			continue;
		}
		if(recvFrame.frame_kind==2)
		{
			printf("Exiting\n");
			break;
		}
		if(recvFrame.frame_kind==1 && recvFrame.seqno==expectedFrame &&recvlen>0)
		{
			printf("Received frame %d\n",recvFrame.seqno);
			
			int l=rand()%100;
			if(l<lossprobability)
			{
				printf("acknowledgement loss for frame %d\n",expectedFrame);
				continue;
			}
			sendFrame.frame_kind=0;
			sendFrame.ack=recvFrame.seqno+1;
			sendFrame.seqno=0;
			sendto(sockfd,&sendFrame,sizeof(sendFrame),0,(struct sockaddr*)&clientAddr,addrsize);
			expectedFrame++;
		}
	}
	close(sockfd);
	return 0;
}
