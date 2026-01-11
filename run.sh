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

export CC="clang"
export CXX="clang++"
export CCACHE_NOCOMPRESS=true
export CCACHE_DIRECT=1
export CCACHE_MAXSIZE=50G
export CMAKE_GENERATOR=Ninja

cmake \
	-S . \
	-B build \
	-G Ninja \
	-DCMAKE_C_COMPILER=clang \
	-DCMAKE_CXX_COMPILER=clang++ \
	-DCMAKE_C_COMPILER_LAUNCHER=ccache \
	-DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
	-DCMAKE_TOOLCHAIN_FILE="${PWD}/tool/vcpkg/scripts/buildsystems/vcpkg.cmake" \
	-DCMAKE_BUILD_TYPE="$BUILD_TYPE"

cmake --build build -j"$(nproc)"

./ldd/run.sh

if [ -e ./build/src/build.h ]; then
	echo cp ./build/src/build.h ./src/
fi

./build/ruin --version
