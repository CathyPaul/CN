// server.c
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>


#define WINDOW_SIZE 3
#define TOTAL_FRAMES 9

void rsendd(int ch, int c_sock){
	char buff2[60];
	bzero(buff2,sizeof(buff2));
	strcpy(buff2,"Retransmission message: ");
	buff2[strlen(buff2)]=(ch)+'0';
	buff2[strlen(buff2)]='\0';
	printf("Resending Message to client: %s\n", buff2);
	write(c_sock, buff2, sizeof(buff2));
	usleep(1000);
}

int main(){
	int s_sock, c_sock;
	s_sock = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in server, other;
	memset(&server, 0, sizeof(server));
	memset(&other, 0, sizeof(other));
	server.sin_family = AF_INET;
	server.sin_port = htons(7004);
	server.sin_addr.s_addr = INADDR_ANY;
	socklen_t add;
	
	if (bind(s_sock,(struct sockaddr*)&server, sizeof(server))== -1){
		printf("Binding failed\n");
		return 0;
	}
	printf("\nSelective Repeat - Server\n\n");
	listen(s_sock,10);
	add = sizeof(other);
	c_sock = accept(s_sock, (struct sockaddr*)&other, &add);
	
	char msg[50] = "server message: ";
	char buff[50];
	int tot = 0;
	
	fd_set set1;
	struct timeval timeout1;
	int rv1;

	while(tot < TOTAL_FRAMES){
		int toti = tot;
		//sending part
		for (int j = toti; j < toti + WINDOW_SIZE && j < TOTAL_FRAMES; j++){
			bzero(buff,sizeof(buff));
			char buff2[60];
			bzero(buff2, sizeof(buff2));
			strcpy(buff2, "Server message: ");
			buff2[strlen(buff2)] = (j) + '0';
			buff2[strlen(buff2)] = '\0';
			printf("Message sent to client: %s\n",buff2);
			write(c_sock, buff2, sizeof(buff2));
			usleep(1000);
		}
		//toti is the smallest unack frame
		for (int k = toti; k < toti + WINDOW_SIZE && k < TOTAL_FRAMES; k++){
		qq:
			FD_ZERO(&set1);
			FD_SET(c_sock,&set1);
			timeout1.tv_sec = 2;
			timeout1.tv_usec = 0;
			rv1 = select(c_sock + 1, &set1, NULL, NULL, &timeout1);//wait for timeout seconds to check whether ack is received or not either nack or ack
			if (rv1 == -1)
				perror("Select error");
			else if (rv1 == 0){
				printf("Timeout for message: %d\n", k);
				rsendd(k, c_sock);
				goto qq;
			}
			else {
				read(c_sock, buff, sizeof(buff));
				printf("Message from client: %s\n",buff);
				if (buff[0] == 'N'){
					printf("Corrupted message acknowledgement (msg %d)\n", buff[strlen(buff)-1] - '0');
					rsendd(buff[strlen(buff)-1] - '0', c_sock);
					goto qq;
				} else {
					tot++;
				}
			}
		}
	}
	close(c_sock);
	close(s_sock);
	return 0;
}
/*

./s

Selective Repeat - Server

Message sent to client: Server message: 0
Message sent to client: Server message: 1
Message sent to client: Server message: 2
Message from client: Negative Acknowledgement: 0
Corrupted message acknowledgement (msg 0)
Resending Message to client: Retransmission message: 0
Message from client: Negative Acknowledgement: 1
Corrupted message acknowledgement (msg 1)
Resending Message to client: Retransmission message: 1
Message from client: Negative Acknowledgement: 2
Corrupted message acknowledgement (msg 2)
Resending Message to client: Retransmission message: 2
Message from client: Acknowledgement of: 0
Message from client: Acknowledgement of: 1
Message from client: Acknowledgement of: 2
Message sent to client: Server message: 3
Message sent to client: Server message: 4
Message sent to client: Server message: 5
Message from client: Negative Acknowledgement: 3
Corrupted message acknowledgement (msg 3)
Resending Message to client: Retransmission message: 3
Message from client: Acknowledgement of: 4
Message from client: Negative Acknowledgement: 5
Corrupted message acknowledgement (msg 5)
Resending Message to client: Retransmission message: 5
Message from client: Acknowledgement of: 3
Message from client: Acknowledgement of: 5
Message sent to client: Server message: 6
Message sent to client: Server message: 7
Message sent to client: Server message: 8
Message from client: Acknowledgement of: 6
Message from client: Acknowledgement of: 7
Message from client: Negative Acknowledgement: 8
Corrupted message acknowledgement (msg 8)
Resending Message to client: Retransmission message: 8
Message from client: Negative Acknowledgement: 8
Corrupted message acknowledgement (msg 8)
Resending Message to client: Retransmission message: 8
Message from client: Negative Acknowledgement: 8
Corrupted message acknowledgement (msg 8)
Resending Message to client: Retransmission message: 8
Message from client: Acknowledgement of: 8
*/
