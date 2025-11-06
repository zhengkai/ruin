#!/bin/bash

if [ "${HOSTNAME,,}" != 'doll' ]; then
	>&2 echo "run in Doll only"
	exit
fi

DIR="$(dirname "$(readlink -f "$0")")" && cd "$DIR" || exit 1

./build.sh || exit 1

DOCKER_IMAGE="ruin"

BUILD_DIR="${DIR}/build"
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

set -x
sudo docker run \
	--name "ruin-cicd" \
	--mount "type=bind,source=${BUILD_DIR},target=/app/build" \
	--rm \
	"$DOCKER_IMAGE" \
	/app/run.sh || exit 1
