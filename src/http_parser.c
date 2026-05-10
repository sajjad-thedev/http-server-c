#include "http_parser.h"
#include <string.h>

parse_result_t parse_http_request(const char *raw, http_request_t *req) {
  // Check for NULL parameters for safety
  if (!raw || !req) {
    return PARSE_ERROR_MALFORMED;
  }
  // Initialize req to 0 to deal with garbage values
  memset(req, 0, sizeof(http_request_t));
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

  // Headers parsing
  const char *header_start = line_end + 2;
  const char *headers_end = strstr(header_start, "\r\n\r\n");
  if (!headers_end) {
    return PARSE_ERROR_INCOMPLETE;
  }
  ptr = header_start;
  req->header_count = 0;
  for (int i = 0; i < MAX_HEADERS; i++) {
    // MAX_HEADERS is reached but there are remaining headers to parse
    if (i == MAX_HEADERS - 1 && ptr < headers_end) {
      return PARSE_ERROR_TOO_LARGE;
    }
    // End of Headers check
    if (ptr >= headers_end) {
      break;
    }
    const char *current_line_end = strstr(ptr, "\r\n");
    if (!current_line_end) {
      return PARSE_ERROR_MALFORMED;
    }
    const char *header_split = strstr(ptr, ": ");
    if (!header_split) {
      return PARSE_ERROR_MALFORMED;
    }
    if (header_split > current_line_end) {
      return PARSE_ERROR_MALFORMED;
    }
    // Header name parsing
    int header_name_len = header_split - ptr;
    if (header_name_len > MAX_HEADER_NAME) {
      return PARSE_ERROR_MALFORMED;
    }
    strncpy(req->headers[i].name, ptr, header_name_len);
    req->headers[i].name[header_name_len] = '\0';
    // Header value parsing
    ptr = header_split + 2;
    if (ptr == current_line_end) {
      return PARSE_ERROR_MALFORMED;
    }
    int header_value_len = current_line_end - ptr;
    if (header_value_len > MAX_HEADER_VALUE) {
      return PARSE_ERROR_MALFORMED;
    }
    strncpy(req->headers[i].value, ptr, header_value_len);
    req->headers[i].value[header_value_len] = '\0';
    // Increment Headers count
    req->header_count++;
    // Advance ptr for the next iteration
    ptr = current_line_end + 2;
  }

  return PARSE_OK;
}
