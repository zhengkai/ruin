#pragma once

#include "../util/file.hpp"
#include "asset.hpp"
#include "convert-sprite.hpp"
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

		for (auto pm : src.map()) {
			Map cm = {};
			convertMap(pm, cm);
			dst.map.emplace(pm.name(), cm);
		}
		return ok;
	};

private:
	void mergeConfig() {
		auto &sc = src.config();
		auto &dc = dst.config;

		dc.playerSprite = sc.playersprite();

		auto &z = sc.zonestart();
		dc.zoneStart = {
			{z.x(), z.y()},
			z.map(),
		};
	};

	void mergeSprite() {
		convertSprite(src.character(), dst, r, dir);
		convertSprite(src.animal(), dst, r, dir);
	};

	void mergeTileset() {

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
		for (const auto &sm : src.mob()) {

			auto &name = sm.name();

			if (dst.mob.contains(name)) {
				spdlog::warn("duplicate mob name: {}", sm.name());
				ok = false;
				break;
			}
			if (!dst.sprite.contains(sm.sprite())) {
				spdlog::warn(
					"missing mob sprite {}.{}", sm.name(), sm.sprite());
				ok = false;
				break;
			}

			dst.mob.emplace(name,
				Mob{
					.sprite = dst.sprite.at(sm.sprite()),
					.type = sm.type(),
				});
		}
	}

	void convertMap(const pb::Map &pm, Map &m) {

		m.w = static_cast<std::size_t>(pm.w());
		m.h = static_cast<std::size_t>(pm.h());

		convertMapStaticTerrain(m, pm.terrain());

		convertMapTrigger(m, pm.trigger());
		convertMapMob(m, dst, pm.mob());
	};
};
}; // namespace asset
