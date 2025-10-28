#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg){
    perror(msg);
    exit(1);
}

int sockfd;
struct sockaddr_in serv_addr;
socklen_t servlen;

void *receive_messages(void *arg){
    char buffer[255];
    int n;
    while(1){
        bzero(buffer, 255);
        n = recvfrom(sockfd, buffer, 255, 0, NULL, NULL); // server address optional
        if(n < 0) error("Error receiving message");
        printf("Server: %s", buffer);
    }
}

void *send_messages(void *arg){
    char buffer[255];
    int n;
    while(1){
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);
        n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, servlen);
        if(n < 0) error("Error sending message");
        if(strncmp("Bye", buffer, 3) == 0){
            printf("Client exiting...\n");
            exit(0);
        }
    }
}

int main(int argc, char *argv[]){
    if(argc < 3){
        fprintf(stderr,"Usage: %s hostname port\n", argv[0]);
        exit(1);
    }

    int portno;
    struct hostent *server;

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0) error("Error opening socket");

    server = gethostbyname(argv[1]);
    if(server == NULL){
        fprintf(stderr,"No such host\n");
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    servlen = sizeof(serv_addr);

    pthread_t recv_thread, send_thread;
    pthread_create(&recv_thread, NULL, receive_messages, NULL);
    pthread_create(&send_thread, NULL, send_messages, NULL);

    pthread_join(recv_thread, NULL);
    pthread_join(send_thread, NULL);

    close(sockfd);
    return 0;
}
