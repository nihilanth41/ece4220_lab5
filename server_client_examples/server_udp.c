/* 	Name       : 	server_udp.c
	Author     : 	Luis A. Rivera
	Description: 	ECE 4220/7220 lab, Fall 2012
					Lab 5. Simple server (UDP) for lecture purposes			*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MSG_SIZE 40			// message size

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
   int sock, length, n;
   int boolval = 1;		// for a socket option
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[MSG_SIZE];

   if (argc < 2)
   {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
   }

   sock = socket(AF_INET, SOCK_DGRAM, 0); // Creates socket. Connectionless.
   if (sock < 0)
	   error("Opening socket");

   length = sizeof(server);			// length of structure
   bzero(&server,length);			// sets all values to zero. memset() could be used
   server.sin_family = AF_INET;		// symbol constant for Internet domain
   server.sin_addr.s_addr = INADDR_ANY;		// IP address of the machine on which
											// the server is running
   server.sin_port = htons(atoi(argv[1]));	// port number

   // binds the socket to the address of the host and the port number
   if (bind(sock, (struct sockaddr *)&server, length) < 0)
       error("binding");

   // set broadcast option
   if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &boolval, sizeof(boolval)) < 0)
   	{
   		printf("error setting socket options\n");
   		exit(-1);
   	}

   fromlen = sizeof(struct sockaddr_in);	// size of structure

   while (1)
   {
	   // receive from client
       n = recvfrom(sock, buf, MSG_SIZE, 0, (struct sockaddr *)&from, &fromlen);
       if (n < 0)
    	   error("recvfrom");

       write(1,"Received a datagram: ",21);	// one could use printf...
       write(1, buf, n);	// one could use printf...

       // send message to client
       n = sendto(sock, "Got your message\n", 17, 0, (struct sockaddr *)&from, fromlen);
       if (n  < 0)
    	   error("sendto");
   }

   return 0;
 }

