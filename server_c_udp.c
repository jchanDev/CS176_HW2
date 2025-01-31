// Code based on https://www.geeksforgeeks.org/udp-client-server-using-connect-c-implementation/
// website last updated April 17, 2018
#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#define MAX 256
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
        int bytesRead = recvfrom(sockfd, buff, sizeof(buff) - 1, 0, (SA*)&cli, &len);

        //int bytesRead = recvfrom(sockfd, buff, sizeof(buff) - 1, 0, (SA*)&cli, &len);
        if (bytesRead <= 0) {
            printf("Client disconnected or read error.\n");
            break;
        }

        buff[bytesRead] = '\0'; // Null-terminate the string
        // Clear buffer for response
        char response[MAX] = "";
        
        // Compute repeated digit sums until single digit
        while (1) {
            sum = 0;
            for (int i = 0; i < strlen(buff); i++) {
                if (isdigit(buff[i])) {  
                    sum += buff[i] - '0';
                }
            }

            // Append intermediate result to response
            char temp[MAX];
            snprintf(temp, MAX, "From server: %d\n", sum);
            strncat(response, temp, MAX - strlen(response) - 1);

            // Stop if sum is a single-digit number
            if (sum < 10) {
                break;
            }

            // Convert sum back to string for next iteration
            snprintf(buff, MAX, "%d", sum);
        }

        // Send final result to the client
        sendto(sockfd, response, strlen(response), 0, (SA*)&cli, len);
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
  
    // Function for communication between client and server 
    func(sockfd); 
  
    // After chatting close the socket 
    close(sockfd); 
}