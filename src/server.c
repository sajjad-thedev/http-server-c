#include "server.h"

#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int create_server_socket(int port) {
  // Create socket file descriptor
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("socket");
    return -1;
  }
  // Set SO_REUSEADDR
  int opt = 1;
  if ((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) == -1) {
    perror("setsockopt");
    close(sockfd);
    return -1;
  }
  // Create sockaddr_in
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  // Bind socket to port
  if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("bind");
    close(sockfd);
    return -1;
  }
  // Mark socket on Listen mode
  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    close(sockfd);
    return -1;
  }

  return sockfd;
}
