# CS176_HW2

Derek Dela Cruz, Julia Chan

The code submitted is a client-server implementation in C where the client sends a number up to 128 digits to the server. There is first a check to ensure the input is only digits. If it isn't, the server doesn't proceed. If it is, the server sums the digits, sends it to the client, and repeats the process until the sum is only one digit.

There are four code files in total (TCP server, TCP client, UDP server, UDP client). There two connection implementations: one that utilizes TCP and one that utilizes UDP.

An example output would be:  
csil-machine2> ./server_c_tcp 32000  
csil-machine2> ./client_c_tcp 128.111.49.44 32000  
Enter string: 123456789101234567891012345678910  
From server: 138  
From server: 12  
From server: 3  
csil-machine2>  

The socket and buffer logic for TCP and UDP were sourced from these 2 sources:  
1) https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/ 
2) https://www.geeksforgeeks.org/udp-client-server-using-connect-c-implementation/ 

The logic of summing the digits, checking if the input is all digits, and adding checks for one digit responses were implemented by us.

We also modified the Makefile to add 2 new targets: tcp and udp, which compile only the TCP and UDP code respectively.
