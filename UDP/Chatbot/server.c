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
        fprintf(stderr,"Port number not provided. Program terminated.\n");
        exit(1);
    }

    int sockfd, n;
    int portno = atoi(argv[1]);
    char buffer[255];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0) error("Error opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("Binding failed");

    clilen = sizeof(cli_addr);

    while(1){
        bzero(buffer, 255);
        n = recvfrom(sockfd, buffer, 255, 0, (struct sockaddr *)&cli_addr, &clilen);
        if(n < 0) error("Error on receiving");
        printf("Client: %s", buffer);

        bzero(buffer, 255);
        fgets(buffer, 255, stdin);

        n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&cli_addr, clilen);
        if(n < 0) error("Error on sending");

        if(strncmp("Bye", buffer, 3) == 0) break;
    }

    close(sockfd);
    return 0;
}
