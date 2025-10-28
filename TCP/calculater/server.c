#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int solve(int a, int b, int choice){
    if(choice == 1)
        return a+b;
    if(choice == 2)
        return a-b;
    if(choice == 3)
        return a*b;
    if(choice == 4)
        return a/b;
}

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

    int num1, num2, choice;

S:  n = write(newsockfd, "Enter number 1 : ", strlen("Enter number 1"));
    if (n < 0){
        error("Error writing on socket");
    }
    read(newsockfd, &num1, sizeof(int));
    printf("Client : %d\n", num1);

    n = write(newsockfd, "Enter number 2 : ", strlen("Enter number 2"));
    if (n < 0){
        error("Error writing on socket");
    }
    read(newsockfd, &num2, sizeof(int));
    printf("Client : %d\n", num2);

    n = write(newsockfd, "Enter choice \n1. +\n2. -\n3. *\n4. /\n5.Exit\n", strlen("Enter choice \n1. +\n2. -\n3. *\n4. /\n5.Exit\n"));
    if (n < 0){
        error("Error writing on socket");
    }
    read(newsockfd, &choice, sizeof(int));
    printf("Client : %d\n", choice);

    if(choice != 5){
        int result = solve(num1, num2, choice);
        n = write(newsockfd, &result, sizeof(int));
        if (n < 0){
            error("Error writing on socket");
        }
        goto S;
    }


Q:  close(newsockfd);
    close(sockfd);

    return 0;

}