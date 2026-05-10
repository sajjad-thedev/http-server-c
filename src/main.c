#include "server.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  // Create socket file descriptor
  int sockfd = create_server_socket(DEFAULT_PORT);
  if (sockfd == -1) {
    printf("Failed to create server socket\n");
    return 1;
  }
  printf("Server socket created successfully on port %d (fd: %d)\n",
         DEFAULT_PORT, sockfd);

  // handle SIGPIPE signal to prevent crash on broken connections
  signal(SIGPIPE, SIG_IGN);

  // Accept infinite loop
  while (1) {
    // Create client_addr to store client info and initiate everything to 0
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    memset(&client_addr, 0, client_len);
    // Create clientfd for accept
    int clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
    // Error handling
    if (clientfd == -1) {
      if (errno == EINTR)
        continue;
      perror("accept");
      break;
    }
    // Covert client_ip from network-byte to string
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    // Covert client_port from network-byte to real numbers
    int client_port = ntohs(client_addr.sin_port);
    // Print client ip and port
    printf("Connection from : %s:%d\n", client_ip, client_port);

    // Store the response from client
    char buffer[BUFFER_SIZE];
    ssize_t bytesReceived = recv(clientfd, buffer, sizeof(buffer) - 1, 0);
    // buffer error handling
    if (bytesReceived == -1) {
      perror("recv");
      close(clientfd);
      continue;
    } else if (bytesReceived == 0) {
      close(clientfd);
      continue;
    } else {
      // Terminate buffer with null terminator
      buffer[bytesReceived] = '\0';
      printf("Received %zd bytes:\n%s\n", bytesReceived, buffer);
    }

    // Send hardcoded response back to client
    const char *response =
        "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: "
        "13\r\n\r\nHello, World!";
    ssize_t totalSent = 0;
    ssize_t responseLength = strlen(response);
    while (totalSent < responseLength) {
      ssize_t sent =
          send(clientfd, response + totalSent, responseLength - totalSent, 0);
      if (sent == -1) {
        perror("send");
        break;
      }
      totalSent += sent;
    }

    // Close clientfd
    close(clientfd);
  }
  return 1;
}
