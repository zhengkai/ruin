"use strict";

import fs from "fs";
import path from "path";
import { dirname } from "node:path";
import { fileURLToPath } from "node:url";

const rootDir = dirname(dirname(fileURLToPath(import.meta.url)));

const SOURCE = "asset";
const MANIFEST = "manifest.json";

const dstDir = path.join(rootDir, "build", "Release");

function walk(obj, cb) {
	if (Array.isArray(obj)) {
		return obj.forEach(i => walk(i, cb));
	}
	if (obj && typeof obj === "object") {
		if (obj.path) {
			cb(obj.path);
		}
		for (const k in obj) {
			walk(obj[k], cb);
		}
	}
}

(() => {

	const srcDir = path.join(rootDir, SOURCE);

	if (!fs.existsSync(srcDir)) {
		console.error("asset dir not found:", srcDir);
		return;
	}


	if (!fs.existsSync(dstDir)) {
		console.error("target dir not found:", dstDir);
		return;
	}

	const manifestFile = path.join(srcDir, MANIFEST);
	if (!fs.existsSync(manifestFile)) {
		console.error("Manifest file not found:", manifestFile);
		return;
	}

	let manifest;
	try {
		manifest = JSON.parse(fs.readFileSync(manifestFile, "utf8"));
	} catch (e) {
		console.error(`Manifest file ${manifestFile} read fail: ${e.message}`);
	}

	const copy = (p) => {
		const src = path.join(srcDir, p);
		const dst = path.join(dstDir, SOURCE, p);

		fs.mkdirSync(path.dirname(dst), { recursive: true });
		fs.copyFileSync(src, dst);

		console.log("copied", src, "=>", dst);
	}

	walk(manifest, copy);
	copy(MANIFEST);
	copy("map.json")
	copy("font/JetBrainsMono-Regular.otf");

})();
