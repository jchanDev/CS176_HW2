#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <ctype.h>
#include <unistd.h> // read(), write(), close()
#define MAX 129 
#define SA struct sockaddr
  
// Function designed for chat between client and server. 
void func(int sockfd)
{ 
    char buff[MAX]; 
    int sum = 0;
    int check = 0;
    struct sockaddr_in cli;
    socklen_t len = sizeof(cli);

    // infinite loop for chat 
    for (;;) { 
        // printf("Before recvfrom()\n");
        // int bytesRead = recvfrom(sockfd, buff, sizeof(buff) - 1, 0, (SA*)&cli, &len);
        // printf("After recvfrom(), bytesRead = %d\n", bytesRead);
        int bytesRead = recvfrom(sockfd, buff, sizeof(buff) - 1, 0, (SA*)&cli, &len);
        if (bytesRead <= 0) {
            printf("Client disconnected or read error.\n");
            break;
        }

        buff[bytesRead] = '\0'; // Null-terminate the string

        // sum up the numbers in the buffer
        sum = 0;
        for (int i = 0; i < strlen(buff); i++) {
            if (buff[i] == '\n') {
                continue;  // Skip the newline character
            }
            sum += buff[i] - '0';
            check = 1;
        }

        if (check) {
            printf("Sum: %d\n", sum);
            // Convert the sum back to a string
            snprintf(buff, MAX, "%d", sum);

            // Send the buffer to the client
            sendto(sockfd, buff, strlen(buff), 0, (SA*)&cli, len);
            bzero(buff, sizeof(buff));
        }

        check = 0;
    } 
} 
  
// Driver function 
int main(int argc, char *argv[]) 
{ 
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
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
    // else
    //     printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(atoi(argv[1])); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    // else
    //     printf("Socket successfully binded..\n"); 
  
    // Function for chatting between client and server 
    func(sockfd); 
  
    // After chatting close the socket 
    close(sockfd); 
}