#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define buffersize 1024
#define loss_probabilty 30

typedef struct packet
{
	char data[buffersize];
}Packet;
typedef struct frame
{
	int frame_kind;
	int seqno;
	int ack;
	Packet packet;
}Frame;
int main(int argc, char** argv)
{
	if(argc!=2)
	{
		printf("Specify port as well\n");
		exit(1);
	}
	srand(time(NULL));
	int sockfd;
	int port=atoi(argv[1]);
	struct sockaddr_in serverAddr,clientAddr;
	socklen_t addr_len=sizeof(clientAddr);
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	memset(&serverAddr,0,sizeof(serverAddr));
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons(port);
	serverAddr.sin_addr.s_addr=INADDR_ANY;
	bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
	Frame recv_frame,send_frame;
	int expected_frame=0;
	while(1)
	{
		int recv_len=recvfrom(sockfd,&recv_frame,sizeof(recv_frame),0,(struct sockaddr*)&clientAddr,&addr_len);
		if(recv_len<=0) continue;
		else
		{
			if(recv_frame.frame_kind==2)
			{
				printf("Exiting\n");
				break;
			}
			else if(recv_frame.frame_kind==1 && recv_frame.seqno==expected_frame)
			{
				printf("[+] Frame Received: %s\n",recv_frame.packet.data);
				int l=rand()%100;
				if(l<loss_probabilty)
				{
					printf("acknowledgement lost for frame %d\n",expected_frame);
					continue;
				}
				send_frame.frame_kind=0;
				send_frame.seqno=0;
				send_frame.ack=recv_frame.seqno+1;
				sprintf(send_frame.packet.data,"Acknowledgment for the frame %d\n",expected_frame);
				sendto(sockfd,&send_frame,sizeof(send_frame),0,(struct sockaddr*)&clientAddr,sizeof(clientAddr));
			}
			
		}
		
	}
	close(sockfd);
	return 0;

}
