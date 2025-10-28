#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 4000
#define BUFFER_SIZE 255

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    printf("[+]Client socket created.\n");

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost

    // Connect to server
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }
    printf("[+]Connected to server.\n");

    // Chat loop
    while (1) {
        printf("[You]: ");
        bzero(buffer, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);
        send(client_fd, buffer, strlen(buffer), 0);

        if (strncmp(buffer, "Bye", 3) == 0)
            break;

        bzero(buffer, BUFFER_SIZE);
        int n = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (n <= 0)
            break;

        printf("[Server]: %s", buffer);

        if (strncmp(buffer, "Bye", 3) == 0)
            break;
    }

    close(client_fd);
    printf("[+]Disconnected from server.\n");
    return 0;
}
