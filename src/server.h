#ifndef SERVER_H
#define SERVER_H

#define DEFAULT_PORT 8080
#define BACKLOG 10
#define BUFFER_SIZE 4096

int create_server_socket(int port);

#endif
