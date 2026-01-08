SHELL:=/bin/bash

PBJS=asset/node_modules/protobufjs-cli/bin/pbjs
PBTS=asset/node_modules/protobufjs-cli/bin/pbts

PROTO_DIR=../proto

asset-copy:
	mkdir -p ../build/Release
	./asset/copy.mjs

asset-pb:
	./asset/convert-pb.mjs

asset-init:
	cd asset && npm i --force

asset-pb-init:
	$(PBJS) -t static-module -w commonjs --force-number -o asset/pb.js $(PROTO_DIR)/*.proto
