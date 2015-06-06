#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <strings.h> // This is wrong
#include <string.h> // This will help
#include <unistd.h>
#include <stdio.h>


void printsin(struct sockaddr_in *s, char *str1, char *str2) {
  printf("%s\n", str1);
  printf("%s ", str2);
  printf("%s= %s, %s= %d", "ip", inet_ntoa(s->sin_addr), "port", s->sin_port);
  //-- port: sin->sin_port (host integer type) 
  //-- IP: sin->sin_addr (IP in dotted-decimal notation) 
  printf("\n");
}

int main(int argc, char *argv[])
{
  int socket_fd, cc, fsize;
  struct sockaddr_in  s_in, from;
  struct { char head; char body[100]; char tail;} msg;

  // Create a new socket of type ipv4 and DGRAM (UDP), without stating the protocol and returning its ptr
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);

  // set all values in the buffer to zero
  bzero((char *) &s_in, sizeof(s_in));

  s_in.sin_family = (short)AF_INET;
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */

  // convert the decimal 13107 to TCP/IP network byte order port value
  s_in.sin_port = htons((u_short)0x3333);

  printsin( &s_in, "RECV_UDP:", "Local socket is:"); 
  fflush(stdout);

  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));

  for(;;) {

    // Recieving a message
    fsize = sizeof(from);
    cc = recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize);
    printsin( &from, "RECV_UDP: ", "Packet from:");
    // printf("Got data ::%c%ld%c\n",msg.head,(long) ntohl(msg.body),msg.tail); 
    printf("Got data ::%c%s%c\n",msg.head, msg.body, msg.tail); 
    fflush(stdout);

    // Sending back our name
    strncpy(msg.body, argv[1],sizeof(argv[1]));

    sendto(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from, sizeof(from));
  }
  
  return 0;
}

