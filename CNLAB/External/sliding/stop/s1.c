// RECEIVER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

typedef struct packet {
    char data[1024];
} Packet;

typedef struct frame {
    int frame_kind; // ACK:0, SEQ:1, FIN:2
    int sq_no;
    int ack;
    Packet packet;
} Frame;

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(0);
    }

    srand(time(0)); // Seed random generator

    int port = atoi(argv[1]);
    int sockfd;
    struct sockaddr_in serverAddr, newAddr;
    socklen_t addr_size;
    Frame frame_recv, frame_send;
    int frame_id = 0;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    addr_size = sizeof(newAddr);

    while (1) {
        int f_recv_size = recvfrom(sockfd, &frame_recv, sizeof(Frame), 0, (struct sockaddr*)&newAddr, &addr_size);
        if (f_recv_size > 0 && frame_recv.frame_kind == 1 && frame_recv.sq_no == frame_id) {
            printf("[+] Frame Received: %s\n", frame_recv.packet.data);

            // Simulate ACK loss
            if (rand() % 3 == 0) {
                printf("[!] Simulating ACK Loss (not sending ack)\n");
                continue;
            }

            // Normal ACK
            frame_send.sq_no = 0;
            frame_send.frame_kind = 0;
            frame_send.ack = frame_recv.sq_no + 1;
            sendto(sockfd, &frame_send, sizeof(frame_send), 0, (struct sockaddr*)&newAddr, addr_size);
            printf("[+] Ack Sent for Frame %d\n", frame_recv.sq_no);
        } else {
            printf("[!] Frame Not Received or Unexpected Frame\n");
        }

        frame_id++;
    }

    close(sockfd);
    return 0;
}

