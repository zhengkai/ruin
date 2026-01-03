"use strict";

import fs from "fs";
import path from "path";
import { dirname } from "node:path";
import { fileURLToPath } from "node:url";

const rootDir = dirname(dirname(fileURLToPath(import.meta.url)));

const SOURCE = "asset";
const MANIFEST = "manifest.json";

const srcDir = path.join(rootDir, SOURCE);
const dstDir = path.join(rootDir, "build", "Release");


(() => {

	if (!fs.existsSync(srcDir)) {
		console.error("asset dir not found:", srcDir);
		return;
	}


	if (!fs.existsSync(dstDir)) {
		console.error("target dir not found:", dstDir);
		return;
	}

	const copy = (p) => {
		const src = path.join(srcDir, p);
		if (!fs.existsSync(src)) {
			console.error("src file not found:", src);
			return;
		}

		const dst = path.join(dstDir, SOURCE, p);
		fs.mkdirSync(path.dirname(dst), { recursive: true });
		fs.copyFileSync(src, dst);

		console.log("copied", src, "=>", dst);
	}



	const jsonCache = {};

	const readJSON = (p) => {

		if (jsonCache[p]) {
			return jsonCache[p];
		}

		const fullPath = path.join(srcDir, p);
		if (!fs.existsSync(fullPath)) {
			console.error(`file ${fullPath} not exists`);
			jsonCache[p] = null;
			return null;
		}

		let obj;
		try {
			obj = JSON.parse(fs.readFileSync(fullPath, "utf8"));
			console.error(`read JSON file ${p}`);
		} catch (e) {
			console.error(`JSON file ${p} read fail: ${e.message}`);
		}
		jsonCache[p] = obj || null;
		return obj;
	};

	const walk = (o) => {
		const ks = Object.keys(o);
		for (const k of ks) {
			const v = o[k];
			if (k === "path") {
				if (ks.length === 1 && v.endsWith(".json")) {
					o = walk(readJSON(v));
					continue;
				}
				copy(v);
				continue;
			}
			if (Array.isArray(v)) {
				o[k] = v.map(i => walk(i));
				continue;
			}
			if (typeof o === "object") {
				o[k] = walk(v);
			}
		}
		return o;
	}

	const manifest = walk(readJSON(MANIFEST));
	fs.writeFileSync(
		path.join(dstDir, SOURCE, MANIFEST),
		JSON.stringify(manifest, null, 2),
		"utf8"
	);

	// console.log(manifest);
	console.log(path.join(dstDir, SOURCE, MANIFEST));
})();
