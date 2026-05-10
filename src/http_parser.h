#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#define MAX_HEADERS 32
#define MAX_PATH_LEN 1024
#define MAX_METHOD_LEN 16
#define MAX_VERSION_LEN 16
#define MAX_HEADER_NAME 64
#define MAX_HEADER_VALUE 256
#define MAX_BODY_LEN 4096

// Http header struct with name and value pair
typedef struct {
  char name[MAX_HEADER_NAME];
  char value[MAX_HEADER_VALUE];
} http_header_t;

// http request struct storing data about http request
typedef struct {
  char method[MAX_METHOD_LEN];
  char path[MAX_PATH_LEN];
  char http_version[MAX_VERSION_LEN];
  http_header_t headers[MAX_HEADERS];
  int header_count;
  char body[MAX_BODY_LEN];
  int body_length;
} http_request_t;

// http parser enum for result of http parsing function
typedef enum {
  PARSE_OK,
  PARSE_ERROR_INCOMPLETE,
  PARSE_ERROR_MALFORMED,
  PARSE_ERROR_TOO_LARGE,
} parse_result_t;

// functions prototypes
parse_result_t parse_http_request(const char *raw, http_request_t *req);

#endif
