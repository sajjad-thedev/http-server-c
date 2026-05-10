#include "http_parser.h"
#include <string.h>

parse_result_t parse_http_request(const char *raw, http_request_t *req) {
  // Check for NULL parameters for safety
  if (!raw || !req) {
    return PARSE_ERROR_MALFORMED;
  }
  // Parsing Logic
  const char *ptr = raw;
  const char *line_end = strstr(ptr, "\r\n");
  if (!line_end) {
    return PARSE_ERROR_INCOMPLETE;
  }
  // method parsing
  const char *space = strchr(ptr, ' ');
  if (!space) {
    return PARSE_ERROR_MALFORMED;
  }
  if (space > line_end) {
    return PARSE_ERROR_MALFORMED;
  }
  int method_len = space - ptr;
  if (method_len > MAX_METHOD_LEN) {
    return PARSE_ERROR_TOO_LARGE;
  }
  strncpy(req->method, ptr, method_len);
  req->method[method_len] = '\0';
  // Advance ptr to next word
  ptr = space + 1;
  // Double space error handling
  if (*ptr == ' ') {
    return PARSE_ERROR_MALFORMED;
  }
  space = strchr(ptr, ' ');
  if (!space) {
    return PARSE_ERROR_MALFORMED;
  }
  if (space > line_end) {
    return PARSE_ERROR_MALFORMED;
  }
  int path_len = space - ptr;
  if (path_len > MAX_PATH_LEN) {
    return PARSE_ERROR_TOO_LARGE;
  }
  strncpy(req->path, ptr, path_len);
  req->path[path_len] = '\0';
  // Advance ptr to next word
  ptr = space + 1;
  // Double space error handling
  if (*ptr == ' ') {
    return PARSE_ERROR_MALFORMED;
  }
  // Version parsing
  if (line_end == ptr) {
    return PARSE_ERROR_MALFORMED;
  }
  int version_len = line_end - ptr;
  if (version_len > MAX_VERSION_LEN) {
    return PARSE_ERROR_TOO_LARGE;
  }
  strncpy(req->http_version, ptr, version_len);
  req->http_version[version_len] = '\0';

  return PARSE_OK;
}
