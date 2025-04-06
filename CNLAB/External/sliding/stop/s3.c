#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>//extra person
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define LOSS_PROBABILITY 30  // % chance to simulate ACK loss

typedef struct packet {
    char data[BUFFER_SIZE];
} Packet;

typedef struct frame {
    int frame_kind; // 0=ACK, 1=SEQ data, 2=FIN done communicating
    int sq_no;
    int ack;//next kittenda packet
    Packet packet;
} Frame;

int main(int argc, char** argv)//array of char* ie char**
{
    if (argc != 2) //number of count of arguments ./server 8080 has 2 arguments
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size = sizeof(clientAddr);
    int port = atoi(argv[1]);
    Frame frame_recv, frame_send;
    int expected_frame = 0;//remember

    srand(time(NULL));

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    while (1) {
        int recv_len = recvfrom(sockfd, &frame_recv, sizeof(Frame), 0, (struct sockaddr*)&clientAddr, &addr_size);
        if (recv_len <= 0) continue;

        if (frame_recv.frame_kind == 2) {
            printf("[+] FIN received. Exiting.\n");
            break;
        }

        if (frame_recv.frame_kind == 1 && frame_recv.sq_no == expected_frame) {
            printf("[+] Frame Received: %s\n", frame_recv.packet.data);

            // Simulate ACK loss
            int loss = rand() % 100;
            if (loss < LOSS_PROBABILITY) {
                printf("[!] Simulating ACK loss for frame %d\n\n", frame_recv.sq_no);
                continue;
            }

            frame_send.frame_kind = 0;
            frame_send.sq_no = 0;//no need for a sequence number for ack frame 
            frame_send.ack = frame_recv.sq_no + 1;//receive cheytha frame inte seqno+1 anu ack no this ack no is imp in case of server
            sendto(sockfd, &frame_send, sizeof(Frame), 0, (struct sockaddr*)&clientAddr, addr_size);
            printf("[+] ACK Sent for frame %d\n\n", frame_recv.sq_no);
            expected_frame++;
        }
    }

    close(sockfd);
    return 0;
}
/*
gcc s3.c -o s
tlog@ubuntuM1:~/Desktop/External/sliding/stop$ ./s 8080
[+] Frame Received: d1
[+] ACK Sent for frame 0

[+] Frame Received: d2
[+] ACK Sent for frame 1

[+] Frame Received: d3
[+] ACK Sent for frame 2

[+] Frame Received: d4
[+] ACK Sent for frame 3

[+] Frame Received: d5
[+] ACK Sent for frame 4

[+] Frame Received: d6
[+] ACK Sent for frame 5

[+] Frame Received: d7
[+] ACK Sent for frame 6

[+] Frame Received: d8
[+] ACK Sent for frame 7

[+] Frame Received: d9
[!] Simulating ACK loss for frame 8

[+] Frame Received: d9
[+] ACK Sent for frame 8

[+] Frame Received: d10
[!] Simulating ACK loss for frame 9

[+] Frame Received: d10
[!] Simulating ACK loss for frame 9

[+] Frame Received: d10
[+] ACK Sent for frame 9

[+] Frame Received: d11
[!] Simulating ACK loss for frame 10

[+] Frame Received: d11
[+] ACK Sent for frame 10

[+] Frame Received: d12
[+] ACK Sent for frame 11

[+] FIN received. Exiting
*/
