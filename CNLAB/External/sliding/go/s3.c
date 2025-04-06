// go_back_n_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define PORT 9009
#define TOTAL_MESSAGES 10
#define WINDOW_SIZE 3
#define TIMEOUT_SEC 2

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char base_msg[] = "server message :";
    char buffer[100];
    int base = 0, next_seq_num = 0;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 1);

    printf("Server started. Using Go-Back-N (n = %d) to send %d messages.\n\n", WINDOW_SIZE, TOTAL_MESSAGES);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);

    fd_set read_fds;
    struct timeval timeout;

    while (base < TOTAL_MESSAGES) {
        // Send window of messages
        while (next_seq_num < base + WINDOW_SIZE && next_seq_num < TOTAL_MESSAGES) {
            sprintf(buffer, "%s%d", base_msg, next_seq_num);
            printf("Sending: %s\n", buffer);
            write(client_fd, buffer, sizeof(buffer));
            usleep(1000);  // small delay
            next_seq_num++;
        }

        // Wait for ACKs with timeout
        FD_ZERO(&read_fds);
        FD_SET(client_fd, &read_fds);
        timeout.tv_sec = TIMEOUT_SEC;
        timeout.tv_usec = 0;

        int activity = select(client_fd + 1, &read_fds, NULL, NULL, &timeout);
        if (activity == 0) {
            printf("Timeout. Resending from message %d\n\n", base);
            next_seq_num = base;
            continue;
        }

        read(client_fd, buffer, sizeof(buffer));
        int ack_num = buffer[strlen(buffer) - 1] - '0'; // Last char is number
        printf("Received ACK: %s\n", buffer);

        if (ack_num >= base)
            base = ack_num + 1;
    }

    close(client_fd);
    close(server_fd);
    return 0;
}

