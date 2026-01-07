#pragma once

#include "../util/file.hpp"
#include "../util/path.hpp"
#include "../util/pos.hpp"
#include "../util/pose.hpp"
#include "../util/sprite.hpp"
#include "asset.hpp"
#include "pb/manifest.pb.h"
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

inline std::shared_ptr<Map> convertMap(const pb::Map &src, int idx) {

	auto m = std::make_shared<Map>();

	m->idx = idx;

	m->w = static_cast<int>(src.w());
	m->h = static_cast<int>(src.h());

	for (const auto &s : src.terrain()) {

		auto t = s.tile();
		if (!t.id() || !t.name()) {
			continue;
		}
		int id = static_cast<int>(s.id());
		m->terrain.emplace_back(MapCell{
			.id = id,
			.tileName = t.name(),
			.tileID = static_cast<int>(t.id()),
			.pos = util::convertIDToPos(id, m),
		});
	}

	for (const auto &t : src.trigger()) {
		switch (t.trigger_case()) {
		case pb::MapTrigger::kGate: {
			auto &g = t.gate();
			int id = static_cast<int>(t.id());
			m->gate.emplace_back(MapGate{
				.id = id,
				.rect = util::convertIDToRect(id, m),
				.target =
					{
						{g.x(), g.y()},
						g.map(),
					},
			});
			break;
		}
		case pb::MapTrigger::kExit: {
			auto e = t.exit();
			int id = static_cast<int>(t.id());
			m->exit.emplace_back(MapGate{
				.id = id,
				.rect = util::convertIDToRect(id, m),
				.target =
					{
						{e.x(), e.y()},
						e.map(),
					},
			});
			break;
		}
		case pb::MapTrigger::TRIGGER_NOT_SET:
		default:
			spdlog::info("unknown trigger");
			break;
		}
	}

	for (auto &g : m->gate) {
		spdlog::info("  gate id={} target={}@({},{})",
			g.id,
			g.target.name,
			g.target.x,
			g.target.y);
	}

	return m;
}

inline bool Load(SDL_Renderer *r, std::filesystem::path dir, Asset &asset) {

	auto f = util::file(dir / "manifest.json");

	spdlog::info("Loading asset from {}", f);

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

	int idx = 0;
	for (auto m : manifest.map()) {
		asset.map[m.name()] = convertMap(m, idx);
		idx++;
	}
	return true;
};
}; // namespace asset
