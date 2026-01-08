#!/usr/bin/env node

"use strict";

import fs from "fs";
import path from "path";
import { getAssetDir, MANIFEST } from "./local.mjs";
import pb from "./pb.js";

const am = pb.pb.AssetManifest;

(() => {

	let dir = getAssetDir();
	if (!dir) {
		console.error("getAssetDir fail");
		return;
	}
	const { dstDir } = dir;

	const jsonFile = path.join(dstDir, MANIFEST);
	console.log("read json:", jsonFile);

	const data = fs.readFileSync(jsonFile, "utf8");
	if (!data?.length) {
		console.error("read json fail");
		return;
	}

	if (data?.length < 1000) {
		console.error("read json fail, size:", data?.length);
		return;
	}

	console.log("read json done, size:", data?.length);

	const obj = JSON.parse(data);
	const out = am.encode(am.fromObject(obj)).finish();
	if (out?.length < 1000) {
		console.error("pb encode fail, size:", out?.length);
		return;
	}

	const pbFile = jsonFile.replace(/\.json$/, '.pb');
	fs.writeFileSync(pbFile, out);

	console.log('done', pbFile, out.length);
})();
