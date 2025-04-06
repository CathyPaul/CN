// client.c
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/time.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define TOTAL_FRAMES 9

int isfaulty(){
	int d = rand() % 4;
	return (d > 2); // 25% chance of error
}

int main(){
	srand(time(0));
	int c_sock;
	c_sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in client;
	memset(&client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(7004);
	client.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if (connect(c_sock, (struct sockaddr*)&client, sizeof(client)) == -1){
		printf("Connection failed\n");
		return 0;
	}
	printf("\nClient - with individual acknowledgement scheme\n\n");
	
	char msg1[50] = "Acknowledgement of: ";
	char msg3[50] = "Negative Acknowledgement: ";
	char msg2[50], buff[100];
	int count = -1, flag = 1;
	
	while (count < TOTAL_FRAMES - 1){
		bzero(buff, sizeof(buff));
		bzero(msg2, sizeof(msg2));
		/*if (count == TOTAL_FRAMES - 2 && flag == 1){
			flag = 0;
			read(c_sock, buff, sizeof(buff));
			continue;
		}*/
		int n = read(c_sock, buff, sizeof(buff));
		char i = buff[strlen(buff) - 1];
		printf("Message received from server: %s\n", buff);
		
		int isfault = isfaulty();
		printf("Corruption status: %d\n", isfault);
		printf("Acknowledgement sent for message.\n");
		
		if (isfault)
			strcpy(msg2, msg3);
		else {
			strcpy(msg2, msg1);
			count++;	
		}
		msg2[strlen(msg2)] = i;
		write(c_sock, msg2, sizeof(msg2));
	}
	close(c_sock);
	return 0;
}
/*

$ ./c


Client - with individual acknowledgement scheme

Message received from server: Server message: 0
Corruption status: 1
Acknowledgement sent for message.
Message received from server: Server message: 1
Corruption status: 1
Acknowledgement sent for message.
Message received from server: Server message: 2
Corruption status: 1
Acknowledgement sent for message.
Message received from server: Retransmission message: 0
Corruption status: 0
Acknowledgement sent for message.
Message received from server: Retransmission message: 1
Corruption status: 0
Acknowledgement sent for message.
Message received from server: Retransmission message: 2
Corruption status: 0
Acknowledgement sent for message.
Message received from server: Server message: 3
Corruption status: 1
Acknowledgement sent for message.
Message received from server: Server message: 4
Corruption status: 0
Acknowledgement sent for message.
Message received from server: Server message: 5
Corruption status: 1
Acknowledgement sent for message.
Message received from server: Retransmission message: 3
Corruption status: 0
Acknowledgement sent for message.
Message received from server: Retransmission message: 5
Corruption status: 0
Acknowledgement sent for message.
Message received from server: Server message: 6
Corruption status: 0
Acknowledgement sent for message.
Message received from server: Server message: 7
Corruption status: 0
Acknowledgement sent for message.
Message received from server: Server message: 8
Corruption status: 1
Acknowledgement sent for message.
Message received from server: Retransmission message: 8
Corruption status: 1
Acknowledgement sent for message.
Message received from server: Retransmission message: 8
Corruption status: 1
Acknowledgement sent for message.
Message received from server: Retransmission message: 8
Corruption status: 0
Acknowledgement sent for message.

*/




