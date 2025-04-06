/*#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	int client;
	char buffer[1024];
	struct sockaddr_in servAddr;
	socklen_t addrSize;
	client=socket(AF_INET,SOCK_STREAM,0);
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(6255);
	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	connect(client,(struct sockaddr*)&servAddr,sizeof(servAddr));
	printf("Enter data to be sent to server\n");
	fgets(buffer,sizeof(buffer),stdin);
	buffer[(strcspn(buffer,"\n"))]='\0';
	send(client,buffer,1024,0);
	recv(client,buffer,1024,0);
	printf("server says %s\n",buffer);
	close(client);
	return 0;
}*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int client;
    char buffer[1024];
    struct sockaddr_in servAddr;

    // Create socket
    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Define server address
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(6255);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(client, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    // Send a single message
    printf("Enter message for server: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline character

    send(client, buffer, strlen(buffer), 0);

    // Receive response from server
    memset(buffer, 0, sizeof(buffer));  // Clear buffer before receiving
    recv(client, buffer, sizeof(buffer), 0);
    printf("Server: %s\n", buffer);

    // Close client after sending one message
    close(client);
    return 0;
}

