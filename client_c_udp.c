// Code based on https://www.geeksforgeeks.org/udp-client-server-using-connect-c-implementation/
// website last updated April 17, 2018
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#define MAX 256
#define SA struct sockaddr

void func(int sockfd, struct sockaddr_in servaddr)
{
    char buff[MAX];
    int n;
    int valid = 0;
    socklen_t len = sizeof(servaddr); // Length of the server address

    for (;;) {
        while (!valid) {
            valid = 1;
            bzero(buff, sizeof(buff));
            printf("Enter string: ");
            n = 0;
            while ((buff[n++] = getchar()) != '\n');
            if (buff[n-1] == '\n') {
                buff[n-1] = '\0';  // Remove the newline character
            }
            if (strlen(buff) == 0) {
                printf("From server: Sorry, cannot compute!\n");
                valid = 0;
                continue;   // Prompt for input again
             }

            //check if everything in buffer is a number
            for (int i = 0; i < strlen(buff) - 1; i++) {
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
        sendto(sockfd, buff, sizeof(buff), 0, (SA*)&servaddr, len);
        bzero(buff, sizeof(buff));
        
        bool notSingleDigit = true;
        while (1) {
            int bytesRead = recvfrom(sockfd, buff, sizeof(buff) - 1, 0, (SA*)&servaddr, &len);
            if (bytesRead <= 0) {
                printf("Server disconnected or read error.\n");
                break;
            }

            buff[bytesRead] = '\0';

            printf(buff);
            exit(0);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int sockfd;
    struct sockaddr_in servaddr;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(atoi(argv[2]));

    // function for sums
    func(sockfd, servaddr);

    // close the socket
    close(sockfd);
}