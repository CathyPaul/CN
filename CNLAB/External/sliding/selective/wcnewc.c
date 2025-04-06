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
	return (d > 2); // 25% chance of fault
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
	int count = 0;

	while (count < TOTAL_FRAMES){
		bzero(buff, sizeof(buff));
		bzero(msg2, sizeof(msg2));

		int simulateLoss = isfaulty(); // 25% chance to simulate loss
		if (simulateLoss) {
			printf("Simulating loss for message %d (not reading from server)\n", count);
			sleep(3); // Give enough time for server to timeout
			continue;
		}

		int n = read(c_sock, buff, sizeof(buff));
		if (n <= 0) continue; // avoid processing empty reads

		char i = buff[strlen(buff) - 1];
		printf("Message received from server: %s\n", buff);

		int isCorrupt = isfaulty(); // 25% chance of corruption
		printf("Corruption status: %d\n", isCorrupt);
		
		if (isCorrupt)
			strcpy(msg2, msg3);
		else {
			strcpy(msg2, msg1);
			count++;
		}

		msg2[strlen(msg2)] = i;
		msg2[strlen(msg2) + 1] = '\0';
		printf("Sending to server: %s\n", msg2);
		write(c_sock, msg2, sizeof(msg2));
	}

	close(c_sock);
	return 0;
}

