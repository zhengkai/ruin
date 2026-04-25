#!/bin/bash

cd "$(dirname "$(readlink -f "$0")")" || exit 1

export SDL_VIDEO_DRIVER=wayland

ldd ../build/ruin | awk '{print $1}' | sort | tee result.txt
