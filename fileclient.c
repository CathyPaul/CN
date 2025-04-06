#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main() {
    FILE *fp;
    int csd, n, s, cport;
    char name[100], rcvmsg[100], rcvg[100], fname[100];
    struct sockaddr_in servaddr;

    printf("Enter the port: ");
    scanf("%d", &cport);

    csd = socket(AF_INET, SOCK_STREAM, 0);
    if (csd < 0) {
        printf("Error creating socket\n");
        exit(0);
    } else {
        printf("Socket is created\n");
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(cport);

    if (connect(csd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        printf("Error in connection\n");
        exit(0);
    } else {
        printf("Connected to server\n");
    }

    printf("Enter the existing file name on server: ");
    scanf("%s", name);

    printf("Enter the new file name to save as: ");
    scanf("%s", fname);

    fp = fopen(fname, "w");
    if (fp == NULL) {
        perror("File creation failed");
        close(csd);
        return 1;
    }

    send(csd, name, sizeof(name), 0);

    while (1) {
        bzero(rcvg, sizeof(rcvg));
        s = recv(csd, rcvg, sizeof(rcvg) - 1, 0);
        if (s <= 0) {
            printf("Connection closed or error\n");
            break;
        }

        rcvg[s] = '\0';

        if (strcmp(rcvg, "error") == 0) {
            printf("File is not available on the server\n");
            break;
        }

        if (strcmp(rcvg, "completed") == 0) {
            printf("File is transferred successfully\n");
            break;
        }

        fputs(rcvg, stdout);
        fprintf(fp, "%s", rcvg);
    }

    fclose(fp);
    close(csd);
    return 0;
}
