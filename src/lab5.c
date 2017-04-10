#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> // gethostbyname
#include <string.h>
#include <strings.h> // bcopy
#include <errno.h>
#include <time.h>

#define MSG_SIZE 40
#define IP_MAX 10
#define IP_MIN 1

int main(int argc, char **argv) {
  int sock, n;
  struct sockaddr_in server;
  struct sockaddr_in from;
  //struct sockaddr_in broadcast_addr = inet_addr("10.3.52.255");
  struct sockaddr_in broadcast_addr = inet_addr("127.0.0.1");
  struct hostent *hp = NULL;
  int length = sizeof(server);
  socklen_t fromlen = sizeof(struct sockaddr_in);
  unsigned int opt_val_bool = 1; // To pass to setsockopt()
  unsigned int isMaster = 0;
  char buf[MSG_SIZE];

  // Check input args for port #
  if(argc < 3) {
    fprintf(stderr, "Usage is %s <hostname> <port>\n", argv[0]);
    return EXIT_FAILURE;
  }

  // Random numbers seed
  srand(time(NULL));
  // Set server properties
  server.sin_family = AF_INET;
  // Get port # from input 
  server.sin_port = htons(atoi(argv[2]));
  // Get ip address by hostname
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

  // bind socket to address of host + port # (command line)
  if(bind(sock, (struct sockaddr *)&server, length) < 0 )
    {
      // Error binding
      fprintf(stderr, "Error in bind(): %s\n", strerror(errno));
      return EXIT_FAILURE;
    }

  // set socket perms to allow broadcast
  if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *)&opt_val_bool, sizeof(opt_val_bool)) < 0 )
    {
      // Error setting options
      fprintf(stderr, "Error in setsockopt(): %s\n", strerror(errno));
      return EXIT_FAILURE;
    }

  while(1)
    {
      // For sprintf() 
      str_buf[MSG_SIZE];
      // Wait for message from client.
       n = recvfrom(sock, buf, MSG_SIZE, 0, (struct sockaddr *)&from, &fromlen);
       if(n < 0) {
	   error("recvfrom");
	 }

       // DEBUG
       printf("Received datagram. It says: %s", buf);
       if(0 == strcmp(buf, "WHOIS")
	 {
	   if(isMaster)
	     {
	       sprintf(&str_buf, "Zach on board %s is the master", inet_ntoa(&server.sin_addr->s_addr));
	       // Write message to broadcast address.
	       n = sendto(sock, &str_buf, strlen(str_buf), 0, (struct sockaddr *)&broadcast_addr, fromlen);
	       if(n < 0) {
		 error("sendto");
	       }
	     }
	   else // Wait for another master to reply or a WHOIS
	     {
		n = recvfrom(sock, buf, MSG_SIZE, 0, (struct sockaddr *)&from, &fromlen);
		if(n < 0) {
		    error("recvfrom");
		    }
		printf("Received datagram. It says: %s", buf);
		if(0 == strcmp(buf, "VOTE"))
		  {
		    // No master reply
		    // Voting logic
		    // Get random number between 1-10 for the last octet
		    // Zeroes represent the min number
		    int r = rand() % (IP_MAX + 1 - IP_MIN) + IP_MIN;
		    sprintf(str_buf, "# 10.3.52.%d", r);
		    n = sendto(sock, &str_buf, strlen(str_buf), 0, (struct sockaddr *)&broadcast_addr, fromlen);
		    if(n < 0) {
			error("sendto");
		    }




		  }
		else
		  {
		    // Master reply
		    
		    
		    















    return EXIT_SUCCESS;
    }
