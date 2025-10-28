#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

void error(const char * msg){
    perror(msg);
    exit(1);
}

int main(int agrc, char * argv[]){ // argc --> Total no of parameters that will be passed || argv --> Contains the original parameters -> Port No.
    if(agrc < 2){
        fprintf(stderr, "Port No not proided. Program Terminated \n");
        exit(1);
    }
    int sockfd, newsockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr, cli_addr; // sockaddr_in -> provides the internet access address
    socklen_t clilen; // --> 32 bit datatype 

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0){
        error("Error openning Socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr)); // Clears any data/text in whatever is referenced to
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; 
    serv_addr.sin_port = htons(portno); // host to network short

    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("Binding Failed");
    }

    listen(sockfd, 5); // 5 is the max no of clients that the server can be connected to at a time
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if(newsockfd < 0){
        error("Error on Accept");
    }

    int data[10];
    printf("Please input 4 bits of data\n");
    scanf("%d", &data[0]);
    scanf("%d", &data[1]);
    scanf("%d", &data[2]);
    scanf("%d", &data[4]);

    data[6] = data[4] ^ data[2] ^ data[0];
    data[5] = data[4] ^ data[1] ^ data[0];
    data[3] = data[2] ^ data[1] ^ data[0];

    for(int i = 0;i<7;i++){
        n = write(newsockfd, &data[i], sizeof(int));
        if(n < 0){
            error("Error while transmitting data");
        }
    }

    printf("The data sent is -->\n");
    for(int i = 0;i<7;i++){
        printf("%d", data[i]);
    }
    printf("\n");

    close(newsockfd);
    close(sockfd);

    return 0;

}