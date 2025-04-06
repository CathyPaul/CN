// file_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char filename[100], buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Enter filename to request: ");
    scanf("%s", filename);
    write(sockfd, filename, strlen(filename) + 1);

    printf("\n--- Server Response ---\n");
    int n;
    while ((n = read(sockfd, buffer, sizeof(buffer)-1)) > 0) {
        buffer[n] = '\0';
        printf("%s", buffer);
    }

    printf("\n-----------------------\n");

    close(sockfd);
    return 0;
}

