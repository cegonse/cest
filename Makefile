all: test

test: build-cest
	cd build && cmake .. && make
	./build/cest-runner

build-cest:
	mkdir -p build
	quom src/main.hpp build/cest
	python scripts/remove_duplicated_headers.py build/cest
	python scripts/remove_duplicated_once_blocks.py build/cest

clean:
	@rm -rf build

.PHONY: clean run build-cest test
