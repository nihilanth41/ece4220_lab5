#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int sock, length=0;
	struct sockaddr_in server;
	
	// check input args for port #
	if(argc < 3) {
		printf("Usage is %s <host_ip> <port>\n", argv[0]);
		return EXIT_FAILURE;
	}
	//set server properties
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	// Memcpy, memset, or strcpy ?
	bcopy((char *)argv[1], (char *)&server.sin_addr, strlen(argv[1])+1);

	// Connectionless ipv4 socket (domain, type, protocol=0)
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
	{
		// Error creating socket
		return EXIT_FAILURE;
	}
	// get ip of board <that is running this program> ifocnfg or otherwise
	// bind socket to address of host + port # (command line)
	if( bind(sock, (struct sockaddr *)&server, length) < 0 )
	{
		// Error binding
		return EXIT_FAILURE;
	}


	// set socket perms to allow broadcast
	//setsockopt(

	return EXIT_SUCCESS;
}
