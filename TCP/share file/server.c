#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int sockfd, newsockfd, portno;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    char buffer[256];
    int n;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) error("Error opening socket");

    // Prepare server address
    bzero((char*)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind
    if(bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        error("Binding failed");

    // Listen
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    printf("Server listening on port %d...\n", portno);

    // Accept connection
    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
    if(newsockfd < 0) error("Error on accept");

    FILE *fp = fopen("received.txt", "w");
    if(!fp) error("Cannot open file");

    printf("Receiving file...\n");

    // Receive data
    while((n = read(newsockfd, buffer, sizeof(buffer))) > 0){
        fwrite(buffer, 1, n, fp);
    }

    printf("File received successfully.\n");

    fclose(fp);
    close(newsockfd);
    close(sockfd);

    return 0;
}
