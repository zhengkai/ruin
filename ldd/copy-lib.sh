#!/bin/bash

cd "$(dirname "$(readlink -f "$0")")" || exit 1

TARGET_DIR="lib"
mkdir -p "$TARGET_DIR"

ldd ../build/ruin | grep "=> /" | awk '{print $3}' | while read -r lib; do
    cp "$lib" "$TARGET_DIR"
done
chmod -x "${TARGET_DIR}"/*
