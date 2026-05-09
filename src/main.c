#include "server.h"
#include <stdio.h>

int main(void) {
  int sockfd = create_server_socket(DEFAULT_PORT);
  if (sockfd == -1) {
    printf("Failed to create server socket\n");
    return 1;
  }
  printf("Server socket created successfully on port %d (fd: %d)\n",
         DEFAULT_PORT, sockfd);
  return 0;
}
