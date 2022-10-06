#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

// VVVV Log file to check whether server got the message
// tail ~cs257/public/serverLogs.txt

int main(int argc, char * argv[]) {
  const char *destination = "128.151.69.87"; // IP address of cycle3.csug.rochester.edu
  unsigned short src_port = 0;
  unsigned short dest_port = 4257;

  // Create socket
  int sock = socket(PF_INET, SOCK_DGRAM, 0);

  // Build address data structure
  struct sockaddr_in src_sockaddr;
  bzero((char *)&src_sockaddr, sizeof(src_sockaddr));
  src_sockaddr.sin_family = AF_INET;
  src_sockaddr.sin_port = htons(src_port);
  src_sockaddr.sin_addr.s_addr = INADDR_ANY;

  // Bind socket
  int bind_val = bind(sock, (const struct sockaddr*)&src_sockaddr, sizeof(src_sockaddr));
  if (bind_val != 0) {
    perror("Error binding socket!");
    exit(1);
  }

  // Connect socket to client address
  int connect_val = connect(sock, (struct sockaddr *)&src_sockaddr, sizeof(src_sockaddr));
  if (connect_val < 0) {
    perror("Error connecting the socket to the src sockaddr!");
    close(sock);
    exit(1);
  }

  // Create sockaddr for server so we can send UDP segments to it
  struct sockaddr_in dest_sockaddr;
  bzero((char*)&dest_sockaddr, sizeof(dest_sockaddr));
  dest_sockaddr.sin_family = AF_INET;
  dest_sockaddr.sin_port = htons(dest_port);

  // Convert string IP address into a network address data structure and store it as the addr in the destination sockaddr
  if (inet_aton(destination, &dest_sockaddr.sin_addr) != 1) {
    perror("Could not convert from dot notation address to network address format!");
    exit(1);
  }

  printf("Sending to: %s port %d\n", inet_ntoa(dest_sockaddr.sin_addr), ntohs(dest_sockaddr.sin_port));





  // TODO need to get input from user
  // use gets(buf+1, buflen-2) ???
  // Then set buf[0] = 0;
  int sendbuf_len = 256;
  char sendbuf[sendbuf_len];

  // Get message from user. Reserve first byte for opcode
  // TODO need to write a loop to:
  //    display new messages
  //    get input from the user to post to the chat board
  printf("Enter a command\n");
  fgets(sendbuf+1, sendbuf_len-2, stdin);
  sendbuf[0] = 1; // TODO determine opcode from input message
  sendbuf[sendbuf_len-1] = '\0';

  // TODO need to add opcode as first byte of message so we can get a response from the server
  if () {

  }


  int bytes_sent = sendto(sock, sendbuf, strlen(sendbuf+1)+1, 0, (const struct sockaddr*)&dest_sockaddr, sizeof(dest_sockaddr));
  printf("Sent %d bytes.\n", bytes_sent);


  int recvbuf_len = 478;
  char recvbuf[buf_len];
  recvbuf[buf_len-1] = '\0';

  struct sockaddr_in client;
  socklen_t client_len = sizeof(client);
  int bytes_received = recvfrom(sock, buf, buf_len, 0, (struct sockaddr*)&client, &client_len);
  printf("Receieved %d bytes from %s:%d\n", bytes_received, inet_ntoa(client.sin_addr), ntohs(client.sin_port));

  close(sock);
}
