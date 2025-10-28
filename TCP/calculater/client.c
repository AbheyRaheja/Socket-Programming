#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void error(const char * msg){
    perror(msg);
    exit(1);
}

int main(int argc, char * argv[]){
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[255];

    if(argc < 3){
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if(server == NULL){
        fprintf(stderr, "ERROR, no such host\n");
        exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("Connection Failed");
    }

    int num1, num2, choice, result;

S:  bzero(buffer, 255);
    n = read(sockfd, buffer, 255);
    if(n < 0){
        error("Error in reading.");
    }
    printf("Server : %s\n", buffer);
    scanf("%d", &num1);
    n = write(sockfd, &num1, sizeof(int));
    if(n < 0){
        error("Error in writing.");
    }

    bzero(buffer, 255);
    n = read(sockfd, buffer, 255);
    if(n < 0){
        error("Error in reading.");
    }
    printf("Server : %s\n", buffer);
    scanf("%d", &num2);
    n = write(sockfd, &num2, sizeof(int));
    if(n < 0){
        error("Error in writing.");
    }

    bzero(buffer, 255);
    n = read(sockfd, buffer, 255);
    if(n < 0){
        error("Error in reading.");
    }
    printf("Server : %s\n", buffer);
    scanf("%d", &choice);
    n = write(sockfd, &choice, sizeof(int));
    if(n < 0){
        error("Error in writing.");
    }

    n = read(sockfd, &result, sizeof(int));
    if(n < 0){
        error("Error in reading.");
    }
    printf("Server : Answer --> %d\n", result);

    if(choice == 5){
        goto Q;
    }
    goto S;

Q:  printf("You have selected to exit");
    close(sockfd);
    return 0;
}
