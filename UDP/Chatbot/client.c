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
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[255];
    socklen_t servlen;

    if(argc < 3){
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(1);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
    if(sockfd < 0) error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if(server == NULL){
        fprintf(stderr,"ERROR, no such host\n");
        exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    servlen = sizeof(serv_addr);

    while(1){
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);

        n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, servlen);
        if(n < 0) error("Error on sending");

        bzero(buffer, 255);
        n = recvfrom(sockfd, buffer, 255, 0, (struct sockaddr *)&serv_addr, &servlen);
        if(n < 0) error("Error on receiving");

        printf("Server: %s", buffer);

        if(strncmp("Bye", buffer, 3) == 0) break;
    }

    close(sockfd);
    return 0;
}
