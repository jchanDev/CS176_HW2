CC = gcc
ARGS = -Wall

# Compiling all the dependencies
all: server_c_udp client_c_udp server_c_tcp client_c_tcp

tcp: server_c_tcp client_c_tcp
udp: server_c_udp client_c_udp

# Server and client programs
server_c_udp: server_c_udp.c
	$(CC) $(ARGS) -o server_c_udp server_c_udp.c

client_c_udp: client_c_udp.c
	$(CC) $(ARGS) -o client_c_udp client_c_udp.c

server_c_tcp: server_c_tcp.c
	$(CC) $(ARGS) -o server_c_tcp server_c_tcp.c

client_c_tcp: client_c_tcp.c
	$(CC) $(ARGS) -o client_c_tcp client_c_tcp.c

# Clean up object and temporary files
clean:
	rm -f *.o server_c_udp *~
	rm -f *.o client_c_udp *~
	rm -f *.o server_c_tcp *~
	rm -f *.o client_c_tcp *~