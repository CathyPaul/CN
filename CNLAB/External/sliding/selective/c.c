// client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <time.h>
#include <arpa/inet.h>

#define PORT 7004
#define TOTAL_FRAMES 9

int is_corrupted() {
    return rand() % 4 == 0;  // 25% chance corruption
}

int main() {
    srand(time(NULL));
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    printf("Client - Selective Repeat\n\n");

    int received = 0;
    char buffer[100], reply[100];

    while (received < TOTAL_FRAMES) {
        memset(buffer, 0, sizeof(buffer));
        recv(sock, buffer, sizeof(buffer), 0);
        printf("Message received from server: %s\n", buffer);

        int msg_id = atoi(buffer + strlen(buffer) - 1);
        int corrupt = is_corrupted();
        printf("Corruption status: %d\n", corrupt);

        if (corrupt) {
            snprintf(reply, sizeof(reply), "Negative Acknowledgement: %d", msg_id);
        } else {
            snprintf(reply, sizeof(reply), "Acknowledgement of: %d", msg_id);
            received++;
        }

        send(sock, reply, strlen(reply) + 1, 0);
        usleep(200000);
    }

    close(sock);
    return 0;
}

