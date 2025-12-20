SHELL:=/bin/bash

export RUIN_DIR := $(CURDIR)/static

default:
	./run.sh

run:
	# RUIN_DIR=$(RUIN_DIR) ./build/ruin -r 2,2
	# RUIN_DIR=$(RUIN_DIR) ./build/ruin -r 2,2,3,4,2,2 --fullscreen
	./build/ruin

debug:
	CMAKE_BUILD_TYPE=debug ./run.sh

trace:
	./build/ruin --verbose=trace

fullscreen:
	./build/ruin --fullscreen

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
	./build/ruin --version

tidy:
	find src -type f \( -name '*.h' -o -name '*.hpp' -o -name '*.cpp' \) | xargs clang-tidy -p build

clean:
	@rm -rf build || :
	./run.sh
