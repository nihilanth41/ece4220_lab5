#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> // gethostbyname
#include <string.h>
#include <strings.h> // bcopy
#include <errno.h>

int main(int argc, char **argv) {
	int sock = 0;
	struct sockaddr_in server;
	struct hostent *hp = NULL;
	unsigned int length = 0;
	unsigned int opt_val_bool = 0; // To pass to setsockopt()
	
	// Check input args for port #
	if(argc < 3) {
		printf("Usage is %s <hostname> <port>\n", argv[0]);
		return EXIT_FAILURE;
	}
	// Set server properties
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	hp = gethostbyname(argv[1]);
	if(0 == hp) {
		// Error
		fprintf(stderr, "Error: unknown host\n");
		return EXIT_FAILURE;
	}
	// IP Address
	bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
	// Port
	bcopy((char *)argv[1], (char *)&server.sin_addr, strlen(argv[1])+1);
	length = sizeof(server);

	// Connectionless ipv4 socket (domain, type, protocol=0)
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
	{
		// Error creating socket
		fprintf(stderr, "Error in socket(): %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	// get ip of board <that is running this program> ifconfig or otherwise
	// bind socket to address of host + port # (command line)
	if( bind(sock, (struct sockaddr *)&server, length) < 0 )
	{
		// Error binding
		fprintf(stderr, "Error in bind(): %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	// set socket perms to allow broadcast
	if( setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *)&opt_val_bool, sizeof(opt_val_bool)) < 0 )
	{
		// Error setting options
		fprintf(stderr, "Error in setsockopt(): %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
