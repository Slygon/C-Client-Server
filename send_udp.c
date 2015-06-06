#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <strings.h> // This is wrong
#include <string.h> // This will help
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int socket_fd;
  struct sockaddr_in  dest;
  struct hostent *hostptr;

  // We assume the name is not longer than 100 chars
  struct { char head; char body[100]; char tail; } msgbuf;

  // Create a new socket of type ipv4 and DGRAM (UDP), without stating the protocol and returning its ptr
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);

  // set all values in the buffer to zero
  bzero((char *) &dest, sizeof(dest));
  
  // resolve the dest hostname recieved as an argument
  hostptr = gethostbyname(argv[1]);
  dest.sin_family = (short) AF_INET;

  // copy the dest address to our hostent struct
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);

  // convert the decimal 13107 to TCP/IP network byte order port value
  dest.sin_port = htons((u_short)0x3333);

  msgbuf.head = '<';
  //  msgbuf.body = htonl(getpid()); /* IMPORTANT! */
  strncpy(msgbuf.body, argv[2],sizeof(argv[2]));
  msgbuf.tail = '>';

  // Sending a message
  sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,
                  sizeof(dest));

  // Recieving a message
  int from = sizeof(dest);
  recvfrom(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,&from);
  printf("Got data ::%c%s%c\n",msgbuf.head, msgbuf.body, msgbuf.tail); 
  fflush(stdout);

  return 0;
}
