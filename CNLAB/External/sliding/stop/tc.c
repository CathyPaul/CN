#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define buffersize 1024
#define maxtries 5
#define timeout 2

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
int main(int argc,char** argv)
{
	if(argc!=2)
	{
		printf("Enter port \n");
		exit(1);
	}
}
