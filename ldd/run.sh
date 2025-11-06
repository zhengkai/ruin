#!/bin/bash

cd "$(dirname "$(readlink -f "$0")")" || exit 1

export LD_PRELOAD=

ldd ../build/ruin | awk '{print $1}' | sort | tee result.txt
