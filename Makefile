CC = g++
CXXFLAGS = -O0 -g -Ivendor -Ispec/framework -Wall
LDFLAGS =

TEST_SRCS := $(shell find spec -name '*.cpp')
TESTS := $(basename $(TEST_SRCS))


all: $(TESTS) run

run:
	@find spec -type f -executable -print0 | xargs -0 -I % sh -c %

clean:
	@find spec -type f -executable -delete
	@rm -f log.xml
	@rm -f test_summary.jsonl

.PHONY: clean run
