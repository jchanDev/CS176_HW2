#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> // read(), write(), close()
#define MAX 80 
#define SA struct sockaddr 
  
// Function designed for chat between client and server. 
void func(int connfd) 
{ 
    char buff[MAX]; 
    int sum = 0;
    // infinite loop for chat 
    for (;;) { 
        bzero(buff, MAX);
        int bytesRead = read(connfd, buff, sizeof(buff) - 1);
        if (bytesRead <= 0) {
            printf("Client disconnected or read error.\n");
            break;
        } 
        buff[bytesRead] = '\0'; // Null-terminate the string
        printf("From client: %s\t", buff);
        // Remove any trailing newline
        buff[strcspn(buff, "\n")] = '\0';
        
        //int notSingleDigit = 1;
        while(1) {
            // sum up the numbers in the buffer
            sum = 0;
            for (int i = 0; i < strlen(buff); i++) {
                sum += buff[i] - '0';
            }
            //bzero(buff, MAX);
            printf("Sum: %d\n", sum);
            // Convert the sum back to a string
            snprintf(buff, MAX, "%d", sum);

            // Check if the buffer contains only one digit
            //notSingleDigit = strlen(buff) > 1;

            // Send the buffer to the client
            write(connfd, buff, strlen(buff));
            if (sum < 10) {
                break;
            }
            // // Add each digit in the variable sum to the buffer
            // int index = 0;
            // do {
            //     buff[index++] = (sum % 10) + '0'; // Add the last digit as a character
            //     sum /= 10; // Remove the last digit
            // } while (sum > 0);

            // buff[index] = '\0'; // Null-terminate the buffer

            // // Reverse the buffer since digits were added in reverse order
            // for (int i = 0; i < index / 2; i++) {
            //     char temp = buff[i];
            //     buff[i] = buff[index - i - 1];
            //     buff[index - i - 1] = temp;
            // }

            // // Check if the buffer contains only one digit
            // notSingleDigit = strlen(buff) > 1;
            
            // // and send that buffer to client 
            // write(connfd, buff, strlen(buff)); 
            //bzero(buff, MAX);
        }
        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("exit", buff, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        } 

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
    else
        printf("Socket successfully created..\n"); 
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
    else
        printf("Socket successfully binded..\n"); 
  
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
  
    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("server accept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server accept the client...\n"); 
  
    // Function for chatting between client and server 
    func(connfd); 
  
    // After chatting close the socket 
    close(sockfd); 
}