// go_back_n_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 9009
#define SERVER_IP "127.0.0.1"
#define TOTAL_MESSAGES 10

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[100];
    char ack_base[] = "acknowledgementof-";
    int i;
    int ack_lost_once = 0;  // Simulate ACK loss only once

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Client connected. Ready to receive and acknowledge messages.\n\n");

    for (i = 0; i < TOTAL_MESSAGES; i++) {
        bzero(buffer, sizeof(buffer));
        read(sock, buffer, sizeof(buffer));
        printf("Received: %s\n", buffer);

        int msg_num = buffer[strlen(buffer) - 1] - '0';
        if (msg_num != i) {
            printf("Out-of-order. Discarding.\n\n");
            i--;  // Expecting same msg again
            continue;
        }

        // Simulate ACK loss only once for message 8
        if (msg_num == 8 && !ack_lost_once) {
            printf("[Simulated ACK loss for message %d]\n\n", msg_num);
            ack_lost_once = 1;
            i--;  // Don't increment so same msg will be expected again
            continue;
        }

        char ack[100];
        sprintf(ack, "%s%d", ack_base, msg_num);
        write(sock, ack, sizeof(ack));
    }

    close(sock);
    return 0;
}

