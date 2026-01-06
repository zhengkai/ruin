#!/usr/bin/env node

"use strict";

import fs from "fs";
import path from "path";
import { dirname } from "node:path";
import { fileURLToPath } from "node:url";

const __filename = fileURLToPath(import.meta.url);
const jsonCache = {};

const SOURCE = "asset";
const MANIFEST = "manifest.json";

(() => {

	// 确定目录

	const rootDir = dirname(__filename);
	let tmpDir = rootDir;
	while (true) {
		if (fs.existsSync(path.join(tmpDir, SOURCE, MANIFEST))) {
			break;
		}
		tmpDir = dirname(tmpDir);
		if (!(tmpDir?.length > 1)) {
			console.error(`no asset dir found at ${rootDir}`);
			return;
		}
	}
	const srcDir = path.join(tmpDir, SOURCE);

	tmpDir = path.join(tmpDir, "build", "Release");
	if (!fs.existsSync(tmpDir)) {
		console.error(`no target dir found: ${tmpDir}`);
		return;
	}
	const dstDir = path.join(tmpDir, SOURCE);

	console.info();
	console.info("srcDir:", srcDir);
	console.info("dstDir:", dstDir);
	console.info();

	// 开始处理

	const copy = (p) => {
		const src = path.join(srcDir, p);
		if (!fs.existsSync(src)) {
			console.error("src file not found:", src);
			return;
		}

		const dst = path.join(dstDir, p);
		fs.mkdirSync(path.dirname(dst), { recursive: true });
		fs.copyFileSync(src, dst);

		console.log("copied", src, "=>", dst);
	}


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

		if (Array.isArray(o)) {
			o = o.map(v => walk(v));
			return o;
		}

		if (typeof o === "object") {
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
				o[k] = walk(v);
			}
		}
		return o;
	}

	const manifest = walk(readJSON(MANIFEST));
	fs.writeFileSync(
		path.join(dstDir, MANIFEST),
		JSON.stringify(manifest, null, 2),
		"utf8"
	);
})();
