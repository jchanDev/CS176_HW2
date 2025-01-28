#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <ctype.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define PORT 8080
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
            printf("Enter the string: ");
            n = 0;
            while ((buff[n++] = getchar()) != '\n')
                ;
            //check if everytrhing in buffer is a number
            for (int i = 0; i < strlen(buff) - 1; i++) {
                if (!isdigit(buff[i])) {
                    printf("Must be a number:\n");
                    valid = 0;
                }
            }
        }

        valid = 0;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        int notSingleDigit = 1;
        while (1) {
            int bytesRead = read(sockfd, buff, sizeof(buff) - 1);
            if (bytesRead <= 0) {
                printf("Server disconnected or read error.\n");
                break;
            }
            buff[bytesRead] = '\0'; // Null-terminate the string
            printf("From Server: %s\n", buff);
            notSingleDigit = strlen(buff) > 1;
            if (!notSingleDigit) {
                break;
            }
        }
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
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
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    printf("argv[1]: %s\n", argv[1]);
    printf("type of argv[1]: %d\n", inet_addr(argv[1]));
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(atoi(argv[2]));
    printf("argv[2]: %s\n", argv[2]);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // function for chat
    func(sockfd);

    // close the socket
    close(sockfd);
}