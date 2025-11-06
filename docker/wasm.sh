#!/bin/bash

if [ "${HOSTNAME,,}" != 'doll' ]; then
	>&2 echo "run in Doll only"
	exit
fi

DIR="$(dirname "$(readlink -f "$0")")" && cd "$DIR" || exit 1

./build.sh || exit 1

DOCKER_IMAGE="ruin"

BUILD_WASM_DIR="${DIR}/build-wasm"
rm -rf "$BUILD_WASM_DIR"
mkdir -p "$BUILD_WASM_DIR"

set -x
sudo docker run \
	--name "ruin-cicd" \
	--mount "type=bind,source=${BUILD_WASM_DIR},target=/app/build-wasm" \
	--rm \
	"$DOCKER_IMAGE" \
	/app/wasm.sh || exit 1
