# OS detection
UNAME_S := $(shell uname -s)

# Linux build flags
ifeq ($(UNAME_S),Linux)
	CC = g++
	CXXFLAGS = -O0 -g -Isrc -Itest/framework -Wall
	LDFLAGS =
	CLEANFLAGS = find test -type f -executable -delete
	RUNFLAGS = find test -type f -executable -print0 | xargs -0 -I % sh -c %
endif
# macOS build flags
ifeq ($(UNAME_S),Darwin)
	CC = clang++
	CXXFLAGS = -O0 -g -Isrc -Itest/framework -Wall --std=c++14
	LDFLAGS =
	CLEANFLAGS = find test -type f -perm +111 -delete
	RUNFLAGS = find test -type f -perm +111 -print0 | xargs -0 -I % sh -c %
endif


TEST_SRCS := $(shell find test -name '*.cpp')
TESTS := $(basename $(TEST_SRCS))


all: $(TESTS) run

run:
	@$(RUNFLAGS)

clean:
	@$(CLEANFLAGS)
	@rm -f log.xml
	@rm -f *.jsonl

.PHONY: clean run
