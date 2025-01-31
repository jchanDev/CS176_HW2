// Code based on https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
// website last updated January 10, 2025
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#define MAX 256
#define SA struct sockaddr

void func(int sockfd)
{
    char buff[MAX];
    int n;
    int valid = 0;
    for (;;) {
        while (!valid) {
            valid = 1;
            bzero(buff, sizeof(buff));
            printf("Enter string: ");
            n = 0;
            while ((buff[n++] = getchar()) != '\n')
                ;
            //check if everything in buffer is a number
            for (int i = 0; i < strlen(buff) - 1; i++) {
                if (buff[i] == '\n') continue;
                if (!isdigit(buff[i])) {
                    printf("From server: Sorry, cannot compute!\n");
                    valid = 0;
                    close(sockfd);
                    exit(0);
                    break;
                }
            }
        }

        valid = 0;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        
        bool notSingleDigit = true;
        while (1) {
            int bytesRead = read(sockfd, buff, sizeof(buff) - 1);
            if (bytesRead <= 0) {
                printf("Server disconnected or read error.\n");
                break;
            }

            buff[bytesRead] = '\0'; // Null-terminate the string

            notSingleDigit = strlen(buff) > 1;
            if (!notSingleDigit) {
                printf("From server: %s\n", buff);
                bzero(buff, sizeof(buff));
                notSingleDigit = false;
                close(sockfd);
                exit(0);
                break;
            }
            else {
                printf("From server: %s\n", buff);
                write(sockfd, buff, sizeof(buff));
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(atoi(argv[2]));

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }

    // function for sums
    func(sockfd);

    // close the socket
    close(sockfd);
}
