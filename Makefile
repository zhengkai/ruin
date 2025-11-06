SHELL:=/bin/bash

RUIN_DIR := $(CURDIR)/static

default:
	./run.sh

run:
	# RUIN_DIR=$(RUIN_DIR) ./build/ruin -r 2,2
	# RUIN_DIR=$(RUIN_DIR) ./build/ruin -r 2,2,3,4,2,2 --fullscreen
	RUIN_DIR=$(RUIN_DIR) ./build/ruin -r 2,2,3,4,2,2

v2:
	RUIN_DIR=$(RUIN_DIR) ./build/ruin -W 38 -H 18 -r 2,3,3

classic:
	RUIN_DIR=$(RUIN_DIR) ./build/ruin -W 25 -H 26 -r 2 -c

debug:
	RUIN_DIR=$(RUIN_DIR) CMAKE_BUILD_TYPE=debug ./run.sh

trace:
	RUIN_DIR=$(RUIN_DIR) ./build/ruin --verbose=trace

fullscreen:
	RUIN_DIR=$(RUIN_DIR) ./build/ruin -r 2,2,3,4,2,2 --fullscreen

help:
	./build/ruin --help

wasm:
	./wasm.sh

wasm-clean:
	@rm -rf build-wasm || :
	./wasm.sh

ver:
	./build/ruin --version

ruin:
	./build/ruin --ruin

small:
	./build/ruin --grid-w=20 --grid-h=16

gdb:
	gdb ./build/ruin

vc:
	vcpkg install

version:
	RUIN_DIR=$(RUIN_DIR) ./build/ruin --version

tidy:
	find src -type f \( -name '*.h' -o -name '*.hpp' -o -name '*.cpp' \) | xargs clang-tidy -p build

clean:
	@rm -rf build || :
	./run.sh
