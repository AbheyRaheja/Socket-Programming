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
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[BUFFER_SIZE];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    printf("[+]Server socket created.\n");

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }
    printf("[+]Bind successful.\n");

    // Listen for client connections
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        exit(1);
    }
    printf("[+]Listening on port %d...\n", PORT);

    addr_size = sizeof(client_addr);
    new_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
    if (new_socket < 0) {
        perror("Accept failed");
        exit(1);
    }
    printf("[+]Client connected.\n");

    // Chat loop
    while (1) {
        bzero(buffer, BUFFER_SIZE);
        int n = recv(new_socket, buffer, BUFFER_SIZE, 0);
        if (n <= 0) break;

        printf("[Client]: %s", buffer);

        if (strncmp(buffer, "Bye", 3) == 0)
            break;

        printf("[You]: ");
        bzero(buffer, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);
        send(new_socket, buffer, strlen(buffer), 0);

        if (strncmp(buffer, "Bye", 3) == 0)
            break;
    }

    close(new_socket);
    close(server_fd);
    printf("[+]Connection closed.\n");
    return 0;
}
