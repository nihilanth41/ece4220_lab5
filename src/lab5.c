#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h> // gethostbyname
#include <string.h>
#include <strings.h> // bcopy
#include <errno.h>
#include <time.h>

#define MSG_SIZE 40
#define IP_MAX 10
#define IP_MIN 1
#define VOTE_TIMEOUT_SEC 5
//#define BROADCAST_ADDR 10.3.52.255
#define BROADCAST_ADDR 192.168.0.255

int VOTES[IP_MAX + 1] = { 0 };
int last_octet; // To store the identifier for this instance
// Tally votes. Return 1 if this program is the new master, 0 otherwise;
int newMaster(void) {
  int i = 0;
  int max_votes = 0;
  int new_master = 0;
  for(i=1; i<=IP_MAX; i++)
    {
      printf("[%d]: %d\n", i, VOTES[i]);
      if(VOTES[i] > max_votes)
	{
	  new_master = i;
	  max_votes = VOTES[i];
	}
      else if(max_votes == VOTES[i])
	{
	  if(i > new_master)
	    {
	      new_master = i;
	    }
	}
    }
  if(new_master == last_octet)
    {
      return 1;
    }
  return 0;
}

void socket_transciever(int sockfd, struct sockaddr_in *addr, int len) {
  char tokbuf[MSG_SIZE];
  char bcast_ip[MSG_SIZE];
  char octets[4][4];
  int ip_fourth_octet;

  struct ifreq ifr;
  ifr.ifr_addr.sa_family = AF_INET;
  strncpy((char*)&ifr.ifr_name, "eth0", IFNAMSIZ-1);
  ioctl(sockfd, SIOCGIFADDR, &ifr);
  char *my_ip  = inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr);
  memcpy(tokbuf, my_ip, strlen(my_ip));
  char *tok = strtok(my_ip, ". ");
  int count = 5;
  while(tok != NULL)
    {
      count--;
      if(1 == count)
	{
	  ip_fourth_octet = atoi(tok);
	}
      tok = strtok(NULL, ". ");
    }

  // Get broadcast
  memcpy(tokbuf, my_ip, strlen(my_ip));
  tok = strtok(tokbuf, ". "); // re-parse
  count = 0;
  while(tok != NULL)
    {
      memcpy(octets[count], tok, strlen(my_ip));
      count++;
      tok = strtok(NULL, ". ");
    }
  bzero(&bcast_ip, MSG_SIZE);
  int i;
  for(i=0; i<3; i++)
    {
      strcat(bcast_ip, octets[i]);
      strcat(bcast_ip, ".");
    }
  strcat(bcast_ip, "255");

  // Receieve
  while(1)
    {
      struct sockaddr_in from;
      socklen_t fromlen = sizeof(from);
      int n;
      char buf2[MSG_SIZE];
      bzero(&buf, MSG_SIZE);
      bzero(&buf2, MSG_SIZE);

      n = recvfrom(sockfd, buf, MSG_SIZE, 0, (struct sockaddr *)&from, fromlen);
      if(n < 0) {
	perror("Error receiving data\n"); }
      else {
	printf("Received message\n");
      ("\n" == buf[strlen(buf)-1]) ? memcpy(buf2, buf, strlen(buf)-1) : memcpy(buf2, buf, strlen(buf));

      // voting is occuring 
      if('#' == buf2[0])
	{
	  tok = strtok(buf2, ". ");
	  count = 6;
	    while(tok != NULL)
	      {
		count--;
		(1 == count
	  
											 
      
    

}

      

int main(int argc, char **argv) {
  //  int sock, n;
  //  struct sockaddr_in server;
  //  struct sockaddr_in from;
  //  struct hostent *hp = NULL;
  //  unsigned long broadcast_addr = inet_addr("BROADCAST_ADDR");
  //  unsigned long server_addr; 
  //  int length = sizeof(server);
  //  socklen_t fromlen = sizeof(struct sockaddr_in);
  //  unsigned int opt_val_bool = 1; // To pass to setsockopt()
  //  unsigned int isMaster = 0;
  //  char buf[MSG_SIZE]; // for receive messages
  //  char str_buf[MSG_SIZE]; // for sprintf
  //  char ip_str[INET_ADDRSTRLEN];
  //  
  //  struct timeval read_timeout = { .tv_sec = VOTE_TIMEOUT_SEC, .tv_usec = 0};
  //  
  //
  //  // Check input args for port #
  //  if(argc < 3) {
  //    fprintf(stderr, "Usage is %s <hostname> <port>\n", argv[0]);
  //    return EXIT_FAILURE;
  //  }
  //
  //  // Random numbers seed
  //  srand(time(NULL));
  //  // Set server properties
  //  server.sin_family = AF_INET;
  //  // Get port # from input 
  //  server.sin_port = htons(atoi(argv[2]));
  //  // Get ip address by hostname
  //  hp = gethostbyname(argv[1]);
  //  if(0 == hp) {
  //    // Error
  //    fprintf(stderr, "Error: unknown host\n");
  //    return EXIT_FAILURE;
  //  }
  //  // IP Address
  //  bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
  //  length = sizeof(server);
  //
  //  // Get ip address as string
  //  struct sockaddr_in *ipv4Addr = (struct sockaddr_in*)&server;
  //  struct in_addr ipAddr = ipv4Addr->sin_addr;
  //  inet_ntop(AF_INET, &ipAddr, ip_str, INET_ADDRSTRLEN);
  //  strncpy(&tok_buf, &ip_str, strlen(ip_str));
  //  char *my_id = strtok(&ip_str, ".");
  //  int i;
  //  for(i=0; i<3; i++)
  //    {
  //      my_id = strtok(NULL, ".");
  //    }
  //  last_octet = atoi(my_id);
  //  printf("My identifier: %d\n", last_octet);
  //
  //  // Connectionless ipv4 socket (domain, type, protocol=0)
  //  sock = socket(AF_INET, SOCK_DGRAM, 0);
  //  if(sock < 0)
  //    {
  //      // Error creating socket
  //      fprintf(stderr, "Error in socket(): %s\n", strerror(errno));
  //      return EXIT_FAILURE;
  //    }
  //
  //  // bind socket to address of host + port # (command line)
  //  if(bind(sock, (struct sockaddr *)&server, length) < 0 )
  //    {
  //      // Error binding
  //      fprintf(stderr, "Error in bind(): %s\n", strerror(errno));
  //      return EXIT_FAILURE;
  //    }
  //
  //  // set socket perms to allow broadcast
  //  if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *)&opt_val_bool, sizeof(opt_val_bool)) < 0 )
  //    {
  //      // Error setting options
  //      fprintf(stderr, "Error in setsockopt(): %s\n", strerror(errno));
  //      return EXIT_FAILURE;
  //    }
  //
  //  while(1)
  //    {
  //      // Wait for message from client.
  //       n = recvfrom(sock, buf, MSG_SIZE, 0, (struct sockaddr *)&from, &fromlen);
  //       if(n < 0) {
  //	   perror("recvfrom");
  //	 }
  //       // DEBUG
  //       printf("Received datagram. It says: %s, %d\n", buf, n);
  //       if(0 == strcmp(buf, "WHOIS\n"))
  //	 {
  //	   bzero(buf, MSG_SIZE);
  //	   printf("WHOIS ack.\n");
  //	   if(isMaster)
  //	     {
  //	       server_addr = server.sin_addr.s_addr;
  //	       server.sin_addr.s_addr = broadcast_addr;
  //	       sprintf(&str_buf, "Zach on board %s is the master", ip_str);
  //	       printf("Zach on board is master");
  //	       // Write message to broadcast address.
  //	       // TODO change to broadcast
  //	       n = sendto(sock, &str_buf, strlen(str_buf), 0, (struct sockaddr *)&server, fromlen);
  //	       if(n < 0) {
  //		 perror("sendto");
  //	       }
  //	       server.sin_addr.s_addr = server_addr;
  //	     }
  //	   else // Wait for another master to reply or a VOTE
  //	     {
  //	       n = recvfrom(sock, buf, MSG_SIZE, 0, (struct sockaddr *)&from, &fromlen);
  //	       if(n < 0) {
  //		 perror("recvfrom");
  //	       }
  //	       printf("Received datagram. It says: %s %d", buf, n);
  //	       if(0 == strcmp(buf, "VOTE\n"))
  //		 {
  //		   // Clear votes
  //		   memset(VOTES, 0, (IP_MAX + 1)*sizeof(int));
  //		   bzero(tok_buf, MSG_SIZE);
  //		   printf("Voting initiated.\n");
  //		   // No master reply =>
  //		   // Send and receive votes
  //		   // Get random number between 1-10 for the last octet
  //		   // Zeroes represent the min number
  //		   int r = rand() % (IP_MAX + 1 - IP_MIN) + IP_MIN;
  //		   //VOTES[r]++;
  //		   sprintf(str_buf, "# 10.3.52.%d", r);
  //		   // Send vote to broadcast
  //		   server.sin_addr.s_addr = broadcast_addr;
  //		   n = sendto(sock, &str_buf, MSG_SIZE, 0, (struct sockaddr *)&server, fromlen);
  //		   if(n < 0) {
  //		     perror("sendto");
  //		   }
  //		   
  //		   // Set timeout and read votes
  //		   read_timeout.tv_sec = VOTE_TIMEOUT_SEC;
  //		   if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout)) < 0) {
  //		     perror("Error setting socket timeout");
  //		   }
  //		   do {
  //		     n = recvfrom(sock, buf, MSG_SIZE, 0, (struct sockaddr *)&from, &fromlen);
  //		     // Check buf for leading '#' and parse ip
  //		     if('#' == buf[0]) {
  //		       printf("Vote received\n%s", buf);
  //		       strncpy(&tok_buf, &buf, strlen(buf));
  //		       char *vote_num = strtok(&tok_buf, ".");
  //		       int i;
  //		       for(i=0; i<3; i++)
  //			 {
  //			   vote_num = strtok(NULL, ".");
  //			 }
  //		       int vote_index = atoi(vote_num);
  //		       VOTES[vote_index]++;
  //		       printf("Vote: %d\n", vote_index);
  //		     }
  //		     bzero(buf, MSG_SIZE);
  //		     bzero(tok_buf, MSG_SIZE);
  //
  //		     // Add vote
  //		   } while(n > 0);
  //
  //		   // Disable timeout
  //		   read_timeout.tv_sec = 0;
  //		    if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(struct timeval)) < 0) {
  //		     perror("Error un-setting socket timeout");
  //		   }
  //		    // Change address back from broadcast
  //		   server.sin_addr.s_addr = server_addr;
  //		   // Tally votes
  //		   if(newMaster())
  //		     {
  //		       printf("I am the new master\n");
  //		       isMaster = 1;
  //		     }
  //		 } // Voting not initiated
  //		else
  //		  {
  //		    // Master replied
  //		  }
  //	     }
  //	 } // endifwhois
  //    } // end while

return EXIT_SUCCESS;

}

