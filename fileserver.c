#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    FILE *fp;
    int sd, newsd, n, a, bd, port, clilen;
    char fileread[100], rcv[100];
    struct sockaddr_in servaddr, cliaddr;

    printf("Enter the port address: ");
    scanf("%d", &port);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        printf("Can't create socket\n");
        exit(1);
    } else {
        printf("Socket is created\n");
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    a = sizeof(servaddr);

    bd = bind(sd, (struct sockaddr *)&servaddr, a);
    if (bd < 0) {
        printf("Can't bind\n");
        close(sd);
        exit(1);
    } else {
        printf("Binded successfully\n");
    }

    listen(sd, 5);
    clilen = sizeof(cliaddr);
    newsd = accept(sd, (struct sockaddr *)&cliaddr, (socklen_t *)&clilen);
    if (newsd < 0) {
        printf("Can't accept connection\n");
        close(sd);
        exit(1);
    } else {
        printf("Client connected\n");
    }

    n = recv(newsd, rcv, sizeof(rcv) - 1, 0);
    if (n <= 0) {
        printf("Error receiving file name\n");
        close(newsd);
        close(sd);
        return 1;
    }

    rcv[n] = '\0';//ethinu space ondavan vendiyanu size(rcv)-1 cheythekane
    printf("Client requested file: %s\n", rcv);

    fp = fopen(rcv, "r");
    if (fp == NULL) {
        send(newsd, "error", strlen("error") + 1, 0);
        printf("Requested file not found\n");
        close(newsd);
    } else {
        while (fgets(fileread, sizeof(fileread), fp)) {
            if (send(newsd, fileread, sizeof(fileread), 0) < 0) {
                printf("Can't send file contents\n");
                break;
            }
            sleep(1);  // simulate delay
        }

        send(newsd, "completed", strlen("completed") + 1, 0);//null terminator ne account cheyan vendiya +1
        printf("File transfer completed\n");
        fclose(fp);
        close(newsd);
    }

    close(sd);
    return 0;
}
