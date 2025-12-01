#pragma once

#include "../util/file.hpp"
#include "../util/path.hpp"
#include "../util/pose.hpp"
#include "../util/sprite.hpp"
#include "asset.hpp"
#include "pb/asset.pb.h"
#include "pb/map.pb.h"
#include <google/protobuf/util/json_util.h>

namespace asset {

inline bool mergeCharacter(SDL_Renderer *r,
	pb::AssetManifest &src,
	std::filesystem::path dir,
	Asset &dst) {

	for (const auto &sc : src.character()) {

		auto dc = std::make_shared<Character>();
		dc->name = sc.name();
		dst.character[dc->name] = dc;

		for (const auto &row : sc.sprite()) {

			auto sp = std::make_shared<Sprite>();
			sp->pose = row.pose();

			auto file = dir / row.path();
			sp->list = util::loadSpriteFrames(r, file);
			if (sp->list.empty()) {
				spdlog::warn("sprite character {} is empty: {}",
					dc->name,
					file.string());
				return false;
			}

			for (int s : row.step()) {
				sp->duration.push_back(s);
			}

			auto size = sp->list.size();
			if (size == 0 || size != sp->duration.size()) {
				spdlog::error(
					"sprite character {}.{} has invalid step size: {} vs {}",
					dc->name,
					util::poseName(row.pose()),
					size,
					sp->duration.size());
				return false;
			}

			dc->sprite.insert({sp->pose, sp});
		}
	}
	return true;
}

inline bool mergeTileset(SDL_Renderer *r,
	pb::AssetManifest &src,
	std::filesystem::path dir,
	Asset &dst) {

	for (const auto &st : src.tileset()) {

		auto t = std::make_shared<Tileset>();
		t->name = st.name();
		dst.tileset[t->name] = t;
		spdlog::info("Loading tileset: {}", pb::Tileset_Name_Name(t->name));

		auto file = dir / st.path();
		auto size = st.size();
		t->list = util::loadTileset(r, file, size.w(), size.h());
		if (t->list.empty()) {
			return false;
		}
	}
	return true;
}

inline bool mergeMap(pb::Map &src, Asset &dst) {

	int w = static_cast<int>(src.w());
	int h = static_cast<int>(src.h());

	for (const auto &s : src.list()) {

		auto t = s.tile();
		if (!t.id() || !t.name()) {
			continue;
		}

		int id = static_cast<int>(s.id());

		dst.map.cell.emplace_back(MapCell{
			.id = id,
			.tileName = t.name(),
			.tileID = static_cast<int>(t.id()),
			.x = static_cast<float>(id % w),
			.y = static_cast<float>(h - 1 - id / w),
		});
	}

	// for (const auto &c : dst.map.cell) {
	// spdlog::info("cell {} {} {}", c.id, c.x, c.y);
	// }

	dst.map.w = w;
	dst.map.h = h;

	return true;
}

inline bool Load(SDL_Renderer *r, std::filesystem::path dir, Asset &asset) {

	auto f = util::file(dir / "manifest.json");
	pb::AssetManifest manifest;
	if (!util::loadJSON(f, manifest)) {
		return false;
	}

	if (!mergeCharacter(r, manifest, dir, asset)) {
		return false;
	}

	if (!mergeTileset(r, manifest, dir, asset)) {
		return false;
	}

	auto fm = util::file(dir / "map.json");
	pb::Map map;
	if (!util::loadJSON(fm, map)) {
		return false;
	}

	if (!mergeMap(map, asset)) {
		return false;
	}

	spdlog::info("Loading asset from {}", f);
	return true;
};
}; // namespace asset
