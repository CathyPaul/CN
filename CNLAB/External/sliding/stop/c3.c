#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>//extra person

#define BUFFER_SIZE 1024
#define TIMEOUT_SEC 2
#define MAX_RETRIES 5

typedef struct packet {
    char data[BUFFER_SIZE];
} Packet;

typedef struct frame {
    int frame_kind; // 0=ACK, 1=SEQ, 2=FIN
    int sq_no;
    int ack;
    Packet packet;
} Frame;

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int sockfd;
    struct sockaddr_in serverAddr;
    socklen_t addr_size = sizeof(serverAddr);
    int port = atoi(argv[1]);
    Frame frame_send, frame_recv;
    char input[BUFFER_SIZE];
    int frame_id = 0;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1) {
        printf("Enter Data: ");
        scanf("%s", input);

        if (strcmp(input, "exit") == 0) {
            frame_send.frame_kind = 2;  // FIN
            frame_send.sq_no = frame_id;
            frame_send.ack = 0;
            strcpy(frame_send.packet.data, input);
            sendto(sockfd, &frame_send, sizeof(Frame), 0, (struct sockaddr*)&serverAddr, addr_size);
            printf("[+] FIN Sent. Exiting.\n");
            break;
        }

        int retries = 0;
        int ack_received = 0;

        while (retries < MAX_RETRIES && !ack_received) {
            frame_send.frame_kind = 1;
            frame_send.sq_no = frame_id;
            frame_send.ack = 0;
            strcpy(frame_send.packet.data, input);
            sendto(sockfd, &frame_send, sizeof(Frame), 0, (struct sockaddr*)&serverAddr, addr_size);
            printf("[+] Frame Sent\n");

            // Set up timeout using select()
            fd_set readfds;
            struct timeval timeout;
            FD_ZERO(&readfds);//adyam readfds oru socket lum nokanilla
            FD_SET(sockfd, &readfds);//eppoo athu sockfd lu nokanondu
            timeout.tv_sec = TIMEOUT_SEC;
            timeout.tv_usec = 0;//microsec=0

            int activity = select(sockfd + 1, &readfds, NULL, NULL, &timeout);

            if (activity > 0) {
                int recv_len = recvfrom(sockfd, &frame_recv, sizeof(Frame), 0, (struct sockaddr*)&serverAddr, &addr_size);
                if (recv_len > 0 && frame_recv.frame_kind == 0 && frame_recv.ack == frame_id + 1) {
                    printf("[+] ACK Received\n\n");
                    ack_received = 1;
                    frame_id++;
                    break;
                }
            }

            printf("[!] Timeout - Resending frame %d...\n", frame_id);
            retries++;
        }

        if (!ack_received) {
            printf("[X] Failed to receive ACK after %d retries. Giving up.\n", MAX_RETRIES);
            break;
        }
    }

    close(sockfd);
    return 0;
}
/*
gcc c3.c -o c
tlog@ubuntuM1:~/Desktop/External/sliding/stop$ ./c 8080
Enter Data: d1
[+] Frame Sent
[+] ACK Received

Enter Data: d2
[+] Frame Sent
[+] ACK Received

Enter Data: d3
[+] Frame Sent
[+] ACK Received

Enter Data: d4
[+] Frame Sent
[+] ACK Received

Enter Data: d5
[+] Frame Sent
[+] ACK Received

Enter Data: d6
[+] Frame Sent
[+] ACK Received

Enter Data: d7
[+] Frame Sent
[+] ACK Received

Enter Data: d8
[+] Frame Sent
[+] ACK Received

Enter Data: d9
[+] Frame Sent
[!] Timeout - Resending frame 8...
[+] Frame Sent
[+] ACK Received

Enter Data: d10
[+] Frame Sent
[!] Timeout - Resending frame 9...
[+] Frame Sent
[!] Timeout - Resending frame 9...
[+] Frame Sent
[+] ACK Received

Enter Data: d11
[+] Frame Sent
[!] Timeout - Resending frame 10...
[+] Frame Sent
[+] ACK Received

Enter Data: d12
[+] Frame Sent
[+] ACK Received

Enter Data: exit
[+] FIN Sent. Exiting.
*/
