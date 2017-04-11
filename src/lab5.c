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
#define VOTE_READ_TIMEOUT_USEC 500000

int main(int argc, char **argv) {
  int sock, n;
  struct sockaddr_in server;
  struct sockaddr_in from;
  struct hostent *hp = NULL;
  int length = sizeof(server);
  socklen_t fromlen = sizeof(struct sockaddr_in);
  unsigned int opt_val_bool = 1; // To pass to setsockopt()
  unsigned int isMaster = 0;
  char buf[MSG_SIZE]; // for receive messages
  char str_buf[MSG_SIZE]; // for sprintf
  char tok_buf[MSG_SIZE]; // for strtok
  struct timeval read_timeout = { .tv_sec = 0, .tv_usec = VOTE_READ_TIMEOUT_USEC };
  int VOTES[IP_MAX + 1] = { 0 };


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
      
      // Wait for message from client.
       n = recvfrom(sock, buf, MSG_SIZE, 0, (struct sockaddr *)&from, &fromlen);
       if(n < 0) {
	   perror("recvfrom");
	 }
       // DEBUG
       printf("Received datagram. It says: %s, %d\n", buf, n);
       if(0 == strcmp(buf, "WHOIS\n"))
	 {
	   bzero(buf, MSG_SIZE);
	   printf("WHOIS ack.\n");
	   if(isMaster)
	     {
	       sprintf(&str_buf, "Zach on board %s is the master", hp->h_addr);
	       // Write message to broadcast address.
	       n = sendto(sock, &str_buf, strlen(str_buf), 0, (struct sockaddr *)&server, fromlen);
	       if(n < 0) {
		 perror("sendto");
	       }
	     }
	   else // Wait for another master to reply or a VOTE
	     {
	       n = recvfrom(sock, buf, MSG_SIZE, 0, (struct sockaddr *)&from, &fromlen);
	       if(n < 0) {
		 perror("recvfrom");
	       }
	       printf("Received datagram. It says: %s %d", buf, n);
	       if(0 == strcmp(buf, "VOTE\n"))
		 {
		   printf("Voting initiated.\n");
		   // No master reply =>
		   // Send and receive votes

		   // Get random number between 1-10 for the last octet
		   // Zeroes represent the min number
		   int r = rand() % (IP_MAX + 1 - IP_MIN) + IP_MIN;
		   sprintf(str_buf, "# 10.3.52.%d", r);
		   // Send vote
		   n = sendto(sock, &str_buf, MSG_SIZE, 0, (struct sockaddr *)&server, fromlen);
		   if(n < 0) {
		     perror("sendto");
		   }
		   // Set timeout
		 }
		else
		  {
		    // Master replied
		  }
	     }
	 } // endifwhois
    } // end while

return EXIT_SUCCESS;

}

