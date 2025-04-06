// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/select.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define TIMEOUT 3            // Timeout in seconds
#define WINDOW_SIZE 4        // Sliding window size
#define TOTAL_PACKETS 10     // Number of packets to send

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    /*if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);}*/
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Client: Connected to server.\n");

    int base = 1;
    int next_to_send = 1;
    int ack, packets_acked = 0;

    while (packets_acked < TOTAL_PACKETS) {
        // Send packets in the window
        while (next_to_send < base + WINDOW_SIZE && next_to_send <= TOTAL_PACKETS) {
            memset(buffer, 0, BUFFER_SIZE);
            printf("Client: Sending packet %d\n", next_to_send);
            sprintf(buffer, "%d", next_to_send);
            send(sock, buffer, strlen(buffer) + 1, 0);
            next_to_send++;
        }

        // Wait for ACK using select
        fd_set readfds;
        struct timeval timeout;
        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);
        timeout.tv_sec = TIMEOUT;
        timeout.tv_usec = 0;

	/*
	base: the smallest unacknowledged packet number.

	next_to_send: the next packet number you plan to send.

	So you're sending packets from base up to base + WINDOW_SIZE - 1
	*/



        int activity = select(sock + 1, &readfds, NULL, NULL, &timeout);

        if (activity > 0 )// //&& FD_ISSET(sock, &readfds) venel include akkam Only execute this block if `sock` is ready for reading (i.e., ACK arrived)
        {
            memset(buffer, 0, BUFFER_SIZE);
            int valread = read(sock, buffer, BUFFER_SIZE);
            if (valread > 0) {
                ack = atoi(buffer);
                printf("Client: ACK received for packet %d\n", ack);
                if (ack >= base) {
                    base = ack + 1;//base is smallest unack packet so ack packt +1
                    packets_acked = ack;
                }
            }
        } else if (activity == 0) {
            // Timeout
            printf("Client: Timeout! Retransmitting from packet %d...\n", base);
            next_to_send = base;
        } else {
            perror("select error");
            break;
        }
    }

    printf("Client: All packets sent and acknowledged. Closing connection.\n");
    close(sock);
    return 0;
}
/*

./c
Client: Connected to server.
Client: Sending packet 1
Client: Sending packet 2
Client: Sending packet 3
Client: Sending packet 4
Client: Timeout! Retransmitting from packet 1...
Client: Sending packet 1
Client: Sending packet 2
Client: Sending packet 3
Client: Sending packet 4
Client: Timeout! Retransmitting from packet 1...
Client: Sending packet 1
Client: Sending packet 2
Client: Sending packet 3
Client: Sending packet 4
Client: ACK received for packet 1
Client: Sending packet 5
Client: Timeout! Retransmitting from packet 2...
Client: Sending packet 2
Client: Sending packet 3
Client: Sending packet 4
Client: Sending packet 5
Client: Timeout! Retransmitting from packet 2...
Client: Sending packet 2
Client: Sending packet 3
Client: Sending packet 4
Client: Sending packet 5
Client: ACK received for packet 2
Client: Sending packet 6
Client: ACK received for packet 6
Client: Sending packet 7
Client: Sending packet 8
Client: Sending packet 9
Client: Sending packet 10
Client: ACK received for packet 7
Client: Timeout! Retransmitting from packet 8...
Client: Sending packet 8
Client: Sending packet 9
Client: Sending packet 10
Client: ACK received for packet 8
Client: ACK received for packet 9
Client: Timeout! Retransmitting from packet 10...
Client: Sending packet 10
Client: ACK received for packet 10
Client: All packets sent and acknowledged. Closing connection.
*/
