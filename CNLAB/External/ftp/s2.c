// file_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    char filename[100], buffer[BUFFER_SIZE];
    pid_t pid;//pid_t pid:)

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen
    listen(server_fd, 5);
    printf("Server running on port %d...\n", PORT);

//Loop infinetly till exit 
    while (1) {
        addr_len = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }
//accept success anel child create cheyya
        pid = fork();
        if (pid == 0) {
            // Child process: handle the client
            close(server_fd);//child process thread nu server process thread um ayitu bendham onuulla
            memset(filename, 0, sizeof(filename));
            read(client_fd, filename, sizeof(filename));
            /*
            What does strcspn(filename, "\n") do?
		strcspn() returns the index of the first \n in the string.

		So, for "file.txt\n", it returns 8.

		Then, filename[8] = 0; → replaces \n with the null terminator (\0), effectively chopping off the newline.
            */
            filename[strcspn(filename, "\n")] = 0; // remove newline

            if (strcmp(filename, "exit") == 0) {
                char msg[] = "Server exiting...\n";
                write(client_fd, msg, strlen(msg));
                close(client_fd);
                exit(99);  // Special code to indicate exit
            }

            printf("Client requested: %s\n", filename);
            int fd = open(filename, O_RDONLY);
            if (fd < 0) {
                snprintf(buffer, sizeof(buffer), "File not found\n[PID: %d]", getpid());
                write(client_fd, buffer, strlen(buffer));
            } else {
                int n;
                snprintf(buffer, sizeof(buffer), "File contents from [PID: %d]:\n", getpid());
                write(client_fd, buffer, strlen(buffer));
                while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
                    write(client_fd, buffer, n);
                }
                close(fd);
            }

            close(client_fd);
            exit(0);  // Normal termination
        } else {
            // Parent process
            close(client_fd);
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 99) {
                printf("Exit command received from client. Server shutting down.\n");
                break;
            }
        }
    }

    close(server_fd);
    return 0;
}

