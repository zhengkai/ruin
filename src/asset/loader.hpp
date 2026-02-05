#pragma once

#include "../name/mob.hpp"
#include "../name/zone.hpp"
#include "../util/file.hpp"
#include "../util/path.hpp"
#include "asset.hpp"
#include "file-loader.hpp"
#include "load-sprite.hpp"
#include "load-tileset.hpp"
#include "pb/manifest.pb.h"
#include "util.hpp"
#include <SDL3/SDL_render.h>

namespace asset {

class Loader {
private:
	pb::AssetManifest src;
	Asset &dst;
	SDL_Renderer *r;
	std::filesystem::path dir;
	FileLoader fileLoader;
	bool ok = true;

public:
	Loader(Asset &dst_, SDL_Renderer *r_, std::filesystem::path dir_)
		: dst{dst_}, r{r_}, dir{dir_}, fileLoader{r_, dir_} {};

	bool load() {
		auto f = util::file(dir / "manifest.json");
		spdlog::info("Loading asset from {}", f);

		if (!util::loadJSON(f, src)) {
			return false;
		}

		mergeConfig();
		mergeSprite();
		mergeTileset();
		mergeMob();

		mergeMap();
		mergeZone();

		return ok;
	};

private:
	void mergeConfig() {
		auto &sc = src.config();
		auto &dc = dst.config;

		// dc.playerSprite = sc.playersprite();

		auto &z = sc.zonestart();
		dc.zoneStart = {
			{z.x(), z.y()},
			z.zone(),
		};
	};

	void mergeSprite() {
		convertSprite(src.character(), dst, fileLoader);
		convertSprite(src.animal(), dst, fileLoader);
	};

	void mergeTileset() {

		if (src.tileset().empty()) {
			spdlog::warn("no tileset defined");
			ok = false;
			return;
		}

		for (const auto &st : src.tileset()) {

			auto name = st.name();

			dst.tileset.emplace(name, name);
			auto &ts = dst.tileset.at(name);

			auto file = dir / st.path();
			auto size = st.size();

			auto t = Texture{};
			if (!fileLoader.load(st.path(), t)) {
				return;
			}

			ts.list = loadTileset(r, t, size);
			if (ts.list.empty()) {
				spdlog::warn("load tileset fail: {}", st.path());
				ok = false;
				return;
			}
		}
	}

	void mergeMob() {

		if (src.mob().empty()) {
			spdlog::warn("no mobs defined");
			ok = false;
			return;
		}

		int i = -1;
		for (const auto &sm : src.mob()) {
			i++;

			auto type = name::MobType{sm.type()};
			if (!type) {
				spdlog::warn("invalid mob type: {} {}", i, type);
				ok = false;
				return;
			}

			if (dst.mob.contains(type)) {
				spdlog::warn("duplicate mob type: {}", type);
				ok = false;
				break;
			}

			auto spriteName = name::Sprite{sm.sprite()};

			if (!dst.sprite.contains(spriteName)) {
				spdlog::warn("missing mob sprite {}.{}", type, spriteName);
				ok = false;
				break;
			}

			dst.mob.emplace(type,
				Mob{
					.type = type,
					.sprite = dst.sprite.at(spriteName),
				});
		}
	};

	void mergeMap() {

		if (src.map().empty()) {
			spdlog::warn("no maps defined");
			ok = false;
			return;
		}

		for (auto pm : src.map()) {
			auto name = pm.name();
			dst.map.emplace(name, name);
			auto &cm = dst.map.at(name);
			convertMap(pm, cm);
		}
	};

	void mergeZone() {

		if (src.zone().empty()) {
			spdlog::warn("no maps defined");
			ok = false;
			return;
		}

		for (auto pz : src.zone()) {
			name::Zone name = {pz.name()};
			dst.zone.try_emplace(name, name, dst.map.at(pz.map()));
			auto &cz = dst.zone.at(name);
			if (!convertZone(pz, fileLoader, dst, cz)) {
				ok = false;
				break;
			}
		}
	};
};
}; // namespace asset
