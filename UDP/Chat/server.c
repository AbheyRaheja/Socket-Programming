#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg){
    perror(msg);
    exit(1);
}

int sockfd;
struct sockaddr_in cli_addr;
socklen_t clilen;

void *receive_messages(void *arg){
    char buffer[255];
    int n;
    while(1){
        bzero(buffer, 255);
        n = recvfrom(sockfd, buffer, 255, 0, (struct sockaddr *)&cli_addr, &clilen);
        if(n < 0) error("Error receiving message");
        printf("Client: %s", buffer);
    }
}

void *send_messages(void *arg){
    char buffer[255];
    int n;
    while(1){
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);
        n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&cli_addr, clilen);
        if(n < 0) error("Error sending message");
        if(strncmp("Bye", buffer, 3) == 0){
            printf("Server exiting...\n");
            exit(0);
        }
    }
}

int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr,"Port not provided.\n");
        exit(1);
    }

    int portno;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0) error("Error opening socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Binding failed");

    clilen = sizeof(cli_addr);

    pthread_t recv_thread, send_thread;
    pthread_create(&recv_thread, NULL, receive_messages, NULL);
    pthread_create(&send_thread, NULL, send_messages, NULL);

    pthread_join(recv_thread, NULL);
    pthread_join(send_thread, NULL);

    close(sockfd);
    return 0;
}
