
#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>    
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define SIM_LENGTH 10 
//#define IP_ADDRESS "127.0.0.1" 
#define PORT 80 
#define MAXBUF  1024

int main(int argc, char* argv[])
{ 
  struct sockaddr_in cli_name; 
  int sock, bytes_read;
  int value; 
  char hostname[100];
  char path[100];
  char* url;
  char* prefix="http://";
  struct addrinfo* res;
  struct sockaddr_in* saddr;
  char buffer[MAXBUF];
int port;
  
  if (argc != 2) {
    perror("Usage: hostnamelookup <hostname>\n");
    exit(1);
  }

  url = argv[1];

  // check if url has a "http://" in the beggining
  if (strncmp(url, prefix, 7) != 0) {
    printf("\nno http:// prefix found\n");
    exit(1);
  }

  // cut the http:// from the hostname
 sscanf(url, "http://%99[^/]/%99[^\n]", hostname, path);

  // check if its a valid hostname
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) {
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);
    exit(1);
  }

  printf("Client is alive and establishing socket connection.\n");
  
  
  // Create a new socket of type ipv4 and TCP, without stating the protocol and returning its ptr
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    { perror ("Error opening channel");
      close(sock);
      exit(1);
    }

  saddr = (struct sockaddr_in*)res->ai_addr;

  // set all values in the buffer to zero
  bzero(&cli_name, sizeof(cli_name)); 
  cli_name.sin_family = AF_INET; 
  //cli_name.sin_addr.s_addr = inet_addr(IP_ADDRESS); 
  cli_name.sin_addr.s_addr = inet_addr(inet_ntoa(saddr->sin_addr)); 
  cli_name.sin_port = htons(PORT);

    // ---Connect to server---
    if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0)
    { perror ("Error establishing communications");
      close(sock);
      exit(1);
    }
    
    sprintf(buffer, "GET %s%s HTTP/1.0\n\n", hostname, path);
    send(sock, buffer, strlen(buffer), 0);

    do
    {
        bzero(buffer, sizeof(buffer));
        bytes_read = recv(sock, buffer, sizeof(buffer), 0);
        if ( bytes_read > 0 )
            printf("%s", buffer);
    }
    while ( bytes_read > 0 );

  printf("Exiting now.\n");

  close(sock); 
  exit(0);

} 
