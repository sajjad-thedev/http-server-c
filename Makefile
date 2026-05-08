# ==============================================================================
# http-server-c — Makefile
# ==============================================================================

# --- Compiler & Flags ---------------------------------------------------------

CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -std=c11 -pedantic
LDFLAGS := -lpthread

# Debug build: symbols + AddressSanitizer
DEBUG_CFLAGS := -g -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer

# --- Paths --------------------------------------------------------------------

SRC_DIR   := src
TEST_DIR  := tests
BUILD_DIR := build
LOG_DIR   := logs

TARGET    := http-server

# --- Sources & Objects --------------------------------------------------------

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# ==============================================================================
# Targets
# ==============================================================================

# Default: build the server
.PHONY: all
all: $(BUILD_DIR) $(TARGET)

# Link final binary
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "✓ Build complete → ./$(TARGET)"

# Compile each .c to .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# --- Run ----------------------------------------------------------------------

.PHONY: run
run: all
	./$(TARGET)

# --- Debug build (AddressSanitizer + symbols) ---------------------------------

.PHONY: debug
debug: CFLAGS += $(DEBUG_CFLAGS)
debug: $(BUILD_DIR) $(TARGET)
	@echo "✓ Debug build complete (ASan + UBSan enabled)"

# --- Memcheck (Valgrind) ------------------------------------------------------

.PHONY: memcheck
memcheck: all
	valgrind \
		--leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--verbose \
		./$(TARGET)

# --- Tests --------------------------------------------------------------------

TEST_SRCS   := $(wildcard $(TEST_DIR)/*.c)
TEST_BINS   := $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%, $(TEST_SRCS))

.PHONY: test
test: all $(TEST_BINS)
	@echo "--- Running unit tests ---"
	@for t in $(TEST_BINS); do \
		echo "  Running $$t ..."; \
		$$t && echo "  PASS" || echo "  FAIL"; \
	done
	@if [ -f $(TEST_DIR)/test_server.sh ]; then \
		echo "--- Running integration tests ---"; \
		bash $(TEST_DIR)/test_server.sh; \
	fi

# Compile each test binary (links against all src objects except main.o)
$(BUILD_DIR)/%: $(TEST_DIR)/%.c $(filter-out $(BUILD_DIR)/main.o, $(OBJS))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# --- Setup (create required directories) -------------------------------------

.PHONY: setup
setup:
	@mkdir -p $(SRC_DIR) $(TEST_DIR) $(BUILD_DIR) $(LOG_DIR) www docs
	@echo "✓ Project directories created"

# --- Clean --------------------------------------------------------------------

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	@echo "✓ Cleaned build artifacts"

# --- Help ---------------------------------------------------------------------

.PHONY: help
help:
	@echo ""
	@echo "  http-server-c — available targets"
	@echo ""
	@echo "  make          Build the server (release mode)"
	@echo "  make run      Build and run the server"
	@echo "  make debug    Build with AddressSanitizer + UBSan + debug symbols"
	@echo "  make test     Run unit tests and integration tests"
	@echo "  make memcheck Run server under Valgrind (leak check)"
	@echo "  make setup    Create project directory structure"
	@echo "  make clean    Remove build artifacts and binary"
	@echo "  make help     Show this message"
	@echo ""
