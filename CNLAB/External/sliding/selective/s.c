// server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#define PORT 7004
#define TOTAL_FRAMES 9
#define WINDOW_SIZE 3
#define TIMEOUT_SEC 2

void resend_frame(int frame, int client_sock) {
    char message[100];
    snprintf(message, sizeof(message), "Retransmission message: %d", frame);
    printf("Resending Message to client: %s\n", message);
    send(client_sock, message, strlen(message) + 1, 0);
    usleep(500000);
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr, client_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(sockfd, 5);
    socklen_t addr_size = sizeof(client_addr);
    int client_sock = accept(sockfd, (struct sockaddr*)&client_addr, &addr_size);

    printf("Selective Repeat - Server\n\n");

    int base = 0;
    int acked[TOTAL_FRAMES] = {0};
    char buffer[100];

    fd_set readfds;
    struct timeval timeout;

    while (base < TOTAL_FRAMES) {
        // Send all unacknowledged frames in window
        for (int i = base; i < base + WINDOW_SIZE && i < TOTAL_FRAMES; ++i) {
            if (!acked[i]) {
                char msg[100];
                snprintf(msg, sizeof(msg), "Server message: %d", i);
                printf("Message sent to client: %s\n", msg);
                send(client_sock, msg, strlen(msg) + 1, 0);
                usleep(500000);
            }
        }

        // Check acknowledgments
        for (int i = base; i < base + WINDOW_SIZE && i < TOTAL_FRAMES; ++i) {
            if (acked[i]) continue;

            FD_ZERO(&readfds);
            FD_SET(client_sock, &readfds);
            timeout.tv_sec = TIMEOUT_SEC;
            timeout.tv_usec = 0;

            int ready = select(client_sock + 1, &readfds, NULL, NULL, &timeout);
            if (ready <= 0) {
                printf("Timeout for message: %d\n", i);
                resend_frame(i, client_sock);
                --i;
                continue;
            }

            memset(buffer, 0, sizeof(buffer));
            int bytes = recv(client_sock, buffer, sizeof(buffer), 0);

            if (bytes <= 0 || strlen(buffer) == 0) {
                printf("Empty or invalid response. Retrying message: %d\n", i);
                resend_frame(i, client_sock);
                --i;
                continue;
            }

            printf("Message from client: %s\n", buffer);

            if (strncmp(buffer, "Acknowledgement of:", 19) == 0) {
                int id = atoi(buffer + 20);
                if (id >= 0 && id < TOTAL_FRAMES) {
                    acked[id] = 1;
                }
            } else if (strncmp(buffer, "Negative Acknowledgement:", 25) == 0) {
                int id = atoi(buffer + 26);
                printf("Corrupted message acknowledgement (msg %d)\n", id);
                resend_frame(id, client_sock);
                --i;
            } else {
                printf("Unrecognized response. Retrying message: %d\n", i);
                resend_frame(i, client_sock);
                --i;
            }
        }

        // Slide the window
        while (acked[base] && base < TOTAL_FRAMES) {
            base++;
        }
    }

    printf("\nAll messages acknowledged. Transmission complete.\n");
    close(client_sock);
    close(sockfd);
    return 0;
}

