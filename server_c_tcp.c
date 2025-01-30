#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <ctype.h>
#include <unistd.h> // read(), write(), close()
#define MAX 256
#define SA struct sockaddr 
  
// Function designed for chat between client and server. 
void func(int connfd) 
{ 
    char buff[MAX]; 
    int sum = 0;
    int check = 0;
    // infinite loop for chat 
    for (;;) { 
        int bytesRead = read(connfd, buff, sizeof(buff) - 1);
        if (bytesRead <= 0) {
            printf("Client disconnected or read error.\n");
            break;
        }

        buff[bytesRead] = '\0'; // Null-terminate the string
        buff[strcspn(buff, "\n")] = '\0'; // Remove any trailing newline

        // sum up the numbers in the buffer
        sum = 0;
        for (int i = 0; i < strlen(buff); i++) {
            sum += buff[i] - '0';
            check = 1;
        }

        if (check) {
            printf("Sum: %d\n", sum);
            snprintf(buff, MAX, "%d", sum);
            write(connfd, buff, strlen(buff)); // Send the buffer to the client
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
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
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
  
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    // else
    //     printf("Server listening..\n"); 
    len = sizeof(cli); 

    // Continuously accept new clients
    while (1) {
        // Accept the data packet from client and verification 
        connfd = accept(sockfd, (SA*)&cli, &len); 
        if (connfd < 0) { 
            printf("server accept failed...\n"); 
            exit(0); 
        } 
        // else
        //     printf("server accept the client...\n"); 

        // Handle the client request
        func(connfd); 

        // After handling the client, close the connection and wait for the next client
        close(connfd); 
    }

    // After the loop ends, close the server socket
    close(sockfd); 
}