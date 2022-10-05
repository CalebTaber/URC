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
  int buf_len = 255;
  char buf[buf_len];
  char* message = "Msg1 :D";
  int len;

  const char *destination = "128.151.69.87"; // IP address of cycle3.csug.rochester.edu
  unsigned short src_port = 0;
  unsigned short dest_port = 4257;

  // Create socket
  int fd = socket(PF_INET, SOCK_DGRAM, 0);

  // Build address data structure
  struct sockaddr_in sin;
  bzero((char *)&sin, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(src_port);
  sin.sin_addr.s_addr = INADDR_ANY;

  if (bind(fd, (const struct sockaddr*)&sin, sizeof(sin)) != 0) {
    perror("Bind error");
    exit(1);
  }

  if (connect(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
    perror("Connect error");
    close(fd);
    exit(1);
  }

  buf[buf_len-1] = '\0';
  len = strlen(buf) + 1;

  // Create datagram to send
  struct sockaddr_in dest;
  bzero((char*)&dest, sizeof(dest));
  dest.sin_family = AF_INET;
  dest.sin_port = htons(dest_port);

  if (inet_aton(destination, &dest.sin_addr) != 1)  perror("inet_aton");

  printf("Sending to: %s port %d\n", inet_ntoa(dest.sin_addr), ntohs(dest.sin_port));

  // TODO need to add opcode as first byte of message so we can get a response from the server

  int bytes_sent = sendto(fd, message, strlen(message), 0, (const struct sockaddr*)&dest, sizeof(dest));
  printf("Sent %d bytes.\n", bytes_sent);


  struct sockaddr_in client;
  socklen_t client_len = sizeof(client);
  int bytes_received = recvfrom(fd, buf, buf_len, 0, (struct sockaddr*)&client, &client_len);
  printf("Receieved %d bytes from %s:%d\n", bytes_received, inet_ntoa(client.sin_addr), ntohs(client.sin_port));

  close(fd);
}
