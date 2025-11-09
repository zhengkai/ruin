#!/bin/bash -ex

BUILD_TYPE="${1:-Debug}"

PWD="$(dirname "$(readlink -f "$0")")" && cd "$PWD" || exit 1

mkdir -p static/tmp
if [ -z "$RUIN_DIR" ]; then
	RUIN_DIR="$(pwd)/static"
	export RUIN_DIR
fi

export ASAN_OPTIONS=detect_leaks=0
# export LSAN_OPTIONS="fast_unwind_on_malloc=0:verbosity=1:ignorelist=${PWD}/misc/lsan-ignore.txt"

export CC=clang
export CXX=clang++

cmake \
	-B build \
	-DCMAKE_TOOLCHAIN_FILE="${PWD}/tool/vcpkg/scripts/buildsystems/vcpkg.cmake" \
	-DCMAKE_BUILD_TYPE="$BUILD_TYPE"

cmake --build build --config "$BUILD_TYPE" -j"$(nproc)"

./ldd/run.sh

if [ -e ./build/src/build.h ]; then
	echo cp ./build/src/build.h ./src/
fi

./build/ruin --version
