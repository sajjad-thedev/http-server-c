# http-server-c

HTTP/1.1 server built from scratch in C — TCP sockets, request parsing, static file serving, and POSIX threads.

---

## Overview

A production-quality HTTP/1.1 server implemented in pure C with no external networking libraries. Built as a
systems programming exercise covering the full stack from raw TCP socket management to concurrent client
handling.

This is not a framework wrapper. Every layer — socket lifecycle, HTTP parsing, response building, file serving,
and thread management — is written by hand.

---

## Features

### Implemented

- [ ] TCP socket server with `SO_REUSEADDR` and `SIGPIPE` handling
- [ ] HTTP/1.1 request line and header parsing
- [ ] Response engine with correct status codes and headers
- [ ] Static file serving from configurable document root
- [ ] POSIX thread-per-connection concurrency model
- [ ] Path traversal protection via `realpath()`
- [ ] Request timeouts and size limits
- [ ] Per-IP rate limiting
- [ ] Graceful shutdown on `SIGINT`/`SIGTERM`
- [ ] Thread-safe Apache Combined Format access log
- [ ] Key-value configuration file
- [ ] Automated tests and GitHub Actions CI pipeline

> Checkboxes will be filled as phases complete.

---

## Build

**Requirements:** GCC, Make, Linux (uses `sendfile()` and POSIX threads)

```bash
# Clone
git clone https://github.com/sajjad-thedev/http-server-c.git
cd http-server-c

# Build
make

# Run
make run

# Debug build (AddressSanitizer + UBSan)
make debug

# Run tests
make test

# Valgrind memory check
make memcheck

# Clean
make clean
```

---

## Usage

```bash
# Start with default config
./http-server

# Start with a custom config file
./http-server -c /path/to/http-server.conf
```

Default port: `8080`
Default document root: `./www`

Configuration file format (`http-server.conf`):

```
port            = 8080
document_root   = ./www
log_file        = ./logs/access.log
max_connections = 100
request_timeout = 30
max_request_size = 8192
```

---

## Project Structure

```
http-server-c/
├── src/
│   ├── main.c          # Entry point, accept loop
│   ├── server.c/h      # TCP socket lifecycle
│   ├── http_parser.c/h # HTTP/1.1 request parser
│   ├── response.c/h    # Response builder and router
│   ├── file_handler.c/h# Static file serving
│   ├── logger.c/h      # Thread-safe access and error logs
│   └── config.c/h      # Config file parser
├── www/                # Static files served by default
│   ├── index.html
│   └── 404.html
├── tests/              # Unit and integration tests
├── docs/               # Architecture documentation
├── Makefile
├── LICENSE
└── README.md
```

---

## Testing

```bash
# Unit tests (HTTP parser)
make test

# Manual test with curl
curl -v http://localhost:8080/
curl -v http://localhost:8080/index.html

# Test 404 handling
curl -v http://localhost:8080/nonexistent

# Test path traversal protection
curl -v http://localhost:8080/../../../etc/passwd
```

---

## Roadmap

| Phase | Description                | Status      |
| ----- | -------------------------- | ----------- |
| 0     | Repo setup and scaffolding | In progress |
| 1     | Core TCP socket server     | Planned     |
| 2     | HTTP request parser        | Planned     |
| 3     | Response engine            | Planned     |
| 4     | Static file serving        | Planned     |
| 5     | Concurrent client handling | Planned     |
| 6     | Robustness and security    | Planned     |
| 7     | Logging and configuration  | Planned     |
| 8     | Testing and CI/CD          | Planned     |
| 9     | Polish and portfolio       | Planned     |

---

## License

MIT — see [LICENSE](LICENSE)
