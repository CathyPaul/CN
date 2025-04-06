// udp_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define LOSS_PROBABILITY 30
#define TOTAL_PACKETS 10

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};
    int ack;
    int packets_received = 0;

    srand(time(0));  // Seed random number generator

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server: Listening on UDP port %d...\n", PORT);

    while (packets_received < TOTAL_PACKETS) {
        memset(buffer, 0, BUFFER_SIZE);
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (n <= 0) continue;

        ack = atoi(buffer);
        printf("Server: Received packet %d\n", ack);

        if (rand() % 100 < LOSS_PROBABILITY) {
            printf("Server: ACK for packet %d lost!\n\n", ack);
        } else {
            sleep(1);  // Simulate delay
            printf("Server: ACK sent for packet %d\n\n", ack);
            sprintf(buffer, "%d", ack);
            sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&client_addr, addr_len);
            packets_received++;
        }
    }

    printf("Server: All packets processed. Exiting.\n");
    close(sockfd);
    return 0;
}

