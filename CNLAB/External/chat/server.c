#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    char ip[] = "127.0.0.1";
    int port = 5666;
    int server_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    int n;
    char buffer[1024];

    server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sock < 0)
    {
        perror("[-] Socket Error");
        exit(1);
    }
    printf("[+] UDP server socket created.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    n = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n < 0)
    {
        perror("[-] Bind Error");
        exit(1);
    }
    printf("[+] Bind to the port number: %d\n", port);

    printf("\nChat Server is running...\n");
    addr_size = sizeof(client_addr);
    while (1)
    {

        bzero(buffer, 1024);

        n = recvfrom(server_sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_size);
        if (n < 0)
        {
            perror("[-] Receive Error");
            exit(1);
        }
        printf("Received from client: %s", buffer);
        bzero(buffer, 1024);

        printf("Enter message for client: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (strstr(buffer, "$") != NULL)
        {
            printf("Chat ended by server.\n");
            break;
        }

        n = sendto(server_sock, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, addr_size);
        if (n < 0)
        {
            perror("[-] Send Error");
            exit(1);
        }
    }

    close(server_sock);
    printf("[+] Server closed.\n");

    return 0;
}

