#!/bin/bash -ex

cd "$(dirname "$(readlink -f "$0")")" || exit 1

mkdir -p pb

cd ../proto || exit 1

protoc --cpp_out=../misc/pb ./*.proto
