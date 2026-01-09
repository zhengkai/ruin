#pragma once

#include "../util/file.hpp"
#include "../util/pos.hpp"
#include "asset.hpp"
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

		mergeCharacter();
		mergeTileset();

		int idx = 0;
		for (auto m : src.map()) {
			dst.map[m.name()] = convertMap(m, idx);
			idx++;
		}
		return ok;
	};

private:
	void mergeCharacter() {
		for (const auto &sc : src.character()) {
			auto dc = std::make_shared<SpriteBox>();
			if (!dc->import(sc, r, dir)) {
				continue;
			}
			dst.addSprite(dc);
		}
		for (const auto &sc : src.animal()) {
			auto dc = std::make_shared<SpriteBox>();
			if (!dc->import(sc, r, dir)) {
				continue;
			}
			dst.addSprite(dc);
		}
	};

	void mergeTileset() {

		for (const auto &st : src.tileset()) {

			auto t = std::make_shared<Tileset>();
			t->name = st.name();
			dst.tileset[t->name] = t;
			spdlog::info("Loading tileset: {}", pb::Tileset_Name_Name(t->name));

			auto file = dir / st.path();
			auto size = st.size();
			t->list = util::loadTileset(r, file, size.w(), size.h());
			if (t->list.empty()) {
				ok = false;
				return;
			}
		}
	}

	std::shared_ptr<Map> convertMap(const pb::Map &pm, int idx) {

		auto m = std::make_shared<Map>();

		m->idx = idx;

		m->w = static_cast<int>(pm.w());
		m->h = static_cast<int>(pm.h());

		for (const auto &s : pm.terrain()) {

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

		for (const auto &t : pm.trigger()) {
			switch (t.trigger_case()) {
			case pb::MapTrigger::kGate: {
				m->gate.emplace_back(convertPBTriggerGate(t.id(), t.gate(), m));
				break;
			}
			case pb::MapTrigger::kExit: {
				m->gate.emplace_back(convertPBTriggerGate(t.id(), t.exit(), m));
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
};

}; // namespace asset
