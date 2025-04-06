// udp_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/select.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define TIMEOUT 3
#define WINDOW_SIZE 4
#define TOTAL_PACKETS 10

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    socklen_t addr_len = sizeof(serv_addr);
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Client: Started (UDP)\n");

    int base = 1;
    int next_to_send = 1;
    int ack, packets_acked = 0;

    while (packets_acked < TOTAL_PACKETS) {
        // Send all packets in current window
        while (next_to_send < base + WINDOW_SIZE && next_to_send <= TOTAL_PACKETS) {
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "%d", next_to_send);
            printf("Client: Sending packet %d\n", next_to_send);
            sendto(sock, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&serv_addr, addr_len);
            next_to_send++;
        }

        // Wait for ACK with timeout
        fd_set readfds;
        struct timeval timeout;
        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);
        timeout.tv_sec = TIMEOUT;
        timeout.tv_usec = 0;

        int activity = select(sock + 1, &readfds, NULL, NULL, &timeout);

        if (activity > 0) {
            memset(buffer, 0, BUFFER_SIZE);
            int n = recvfrom(sock, buffer, BUFFER_SIZE, 0, NULL, NULL);
            if (n > 0) {
                ack = atoi(buffer);
                printf("Client: ACK received for packet %d\n", ack);
                if (ack >= base) {
                    base = ack + 1;
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

    printf("Client: All packets sent and acknowledged. Exiting.\n");
    close(sock);
    return 0;
}

