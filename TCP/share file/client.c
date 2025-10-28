#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
    if(argc < 4){
        fprintf(stderr, "Usage: %s <hostname> <port> <filename>\n", argv[0]);
        exit(1);
    }

    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) error("Error opening socket");

    server = gethostbyname(argv[1]);
    if(server == NULL){
        fprintf(stderr, "ERROR, no such host\n");
        exit(1);
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        error("Connection failed");

    FILE *fp = fopen(argv[3], "r");
    if(!fp) error("Cannot open file");

    printf("Sending file...\n");

    size_t n;
    while((n = fread(buffer, 1, sizeof(buffer), fp)) > 0){
        if(write(sockfd, buffer, n) < 0) error("Error writing to socket");
    }

    printf("File sent successfully.\n");

    fclose(fp);
    close(sockfd);

    return 0;
}
