#pragma once

#include "../name/zone.hpp"
#include "../util/file.hpp"
#include "asset.hpp"
#include "convert-sprite.hpp"
#include "pb/manifest.pb.h"
#include "util.hpp"
#include "zone.hpp"
#include <SDL3/SDL_render.h>

namespace asset {

class Loader {
private:
	pb::AssetManifest src;
	Asset &dst;
	SDL_Renderer *r;
	std::filesystem::path dir;
	bool ok = true;

public:
	Loader(Asset &dst, SDL_Renderer *r, std::filesystem::path dir)
		: dst{dst}, r{r}, dir{dir} {};

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
		convertSprite(src.character(), dst, r, dir);
		convertSprite(src.animal(), dst, r, dir);
	};

	void mergeTileset() {

		if (src.tileset().empty()) {
			spdlog::warn("no tileset defined");
			return;
		}

		for (const auto &st : src.tileset()) {

			auto name = st.name();

			dst.tileset.emplace(name, name);
			auto &t = dst.tileset.at(name);

			auto file = dir / st.path();
			auto size = st.size();
			t.list = util::loadTileset(r, file, size.w(), size.h());
			if (t.list.empty()) {
				ok = false;
				return;
			}
		}
	}

	void mergeMob() {

		if (src.mob().empty()) {
			spdlog::warn("no mobs defined");
			return;
		}

		for (const auto &sm : src.mob()) {

			auto &name = sm.name();

			if (dst.mob.contains(name)) {
				spdlog::warn("duplicate mob name: {}", sm.name());
				ok = false;
				break;
			}

			auto spriteName = name::Sprite{sm.sprite()};

			if (!dst.sprite.contains(spriteName)) {
				spdlog::warn("missing mob sprite {}.{}", sm.name(), spriteName);
				ok = false;
				break;
			}

			dst.mob.emplace(name,
				Mob{
					.sprite = dst.getSprite(spriteName),
					// .type = sm.type(),
				});
		}
	};

	void mergeMap() {

		if (src.map().empty()) {
			spdlog::warn("no maps defined");
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
			return;
		}

		for (auto pz : src.zone()) {
			name::Zone name = {pz.name()};
			dst.zone.try_emplace(name, name, dst.map.at(pz.map()));
			auto &cz = dst.zone.at(name);
			convertZone(pz, dst, cz);
		}
	};
};
}; // namespace asset
