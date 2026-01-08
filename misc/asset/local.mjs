"use strict";

import fs from "fs";
import path from "path";
import { dirname } from "node:path";
import { fileURLToPath } from "node:url";

const SOURCE = "asset";
const __filename = fileURLToPath(import.meta.url);

export const MANIFEST = "manifest.json";
export function getAssetDir() {
	const rootDir = dirname(__filename);
	let tmpDir = rootDir;
	while (true) {
		if (fs.existsSync(path.join(tmpDir, SOURCE, MANIFEST))) {
			break;
		}
		tmpDir = dirname(tmpDir);
		if (!(tmpDir?.length > 1)) {
			console.error(`no asset dir found at ${rootDir}`);
			return false;
		}
	}
	const srcDir = path.join(tmpDir, SOURCE);

	tmpDir = path.join(tmpDir, "build", "Release");
	if (!fs.existsSync(tmpDir)) {
		console.error(`no target dir found: ${tmpDir}`);
		return false;
	}
	const dstDir = path.join(tmpDir, SOURCE);

	console.info();
	console.info("srcDir:", srcDir);
	console.info("dstDir:", dstDir);
	console.info();

	return { srcDir, dstDir };
}
