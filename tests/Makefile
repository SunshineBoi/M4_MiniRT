# Run from the project root:
#   make -C tests           build and run every test
#   make -C tests camera    run one test (vec, camera, ...)
#   make -C tests memcheck  run every test under valgrind
#   make -C tests clean     remove the test binaries
#
# Each test_*.c has its own main, so src/main.c is left out.
# New test_*.c files are picked up automatically.

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

ROOT = ..
SRC_DIR = $(ROOT)/src
HEADER = $(ROOT)/include/mrt.h
INCLUDES = -I$(ROOT)/include -I$(ROOT)/minilibx-linux -I.
BUILD_DIR = $(ROOT)/build/tests

TEST_SRCS := $(wildcard test_*.c)
TEST_NAMES := $(TEST_SRCS:test_%.c=%)
TEST_BINS := $(TEST_SRCS:%.c=$(BUILD_DIR)/%)
PROJECT_SRCS := $(filter-out $(SRC_DIR)/main.c, $(shell find $(SRC_DIR) -type f -name '*.c'))

all: $(TEST_BINS)
	@fail=0; for t in $(TEST_BINS); do $$t || fail=1; done; exit $$fail

$(TEST_NAMES): %: $(BUILD_DIR)/test_%
	@$<

memcheck: $(TEST_BINS)
	@fail=0; for t in $(TEST_BINS); do valgrind -q --leak-check=full --error-exitcode=1 $$t || fail=1; done; exit $$fail

$(BUILD_DIR)/%: %.c test.h $(PROJECT_SRCS) $(HEADER)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $< $(PROJECT_SRCS) -L$(ROOT)/minilibx-linux -lmlx -lXext -lX11 -lm -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all memcheck clean $(TEST_NAMES)
