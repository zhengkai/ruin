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
		mergeMonster();

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

	void mergeMonster() {
		for (const auto &sm : src.monster()) {

			auto &name = sm.name();

			if (dst.monster.contains(name)) {
				spdlog::warn("duplicate monster name: {}", sm.name());
				ok = false;
				break;
			}
			if (!dst.sprite.contains(sm.sprite())) {
				spdlog::warn(
					"missing monster sprite {}.{}", sm.name(), sm.sprite());
				ok = false;
				break;
			}

			dst.monster.emplace(name,
				Monster{
					.sprite = dst.sprite[sm.sprite()],
					.type = sm.type(),
				});

			spdlog::info("monster {}", name);
		}
	}

	std::shared_ptr<Map> convertMap(const pb::Map &pm, int idx) {

		auto m = std::make_shared<Map>();

		m->idx = idx;
		m->w = static_cast<int>(pm.w());
		m->h = static_cast<int>(pm.h());

		convertMapTerrain(m, pm.terrain());
		convertMapTrigger(m, pm.trigger());
		convertMapMonster(m, pm.monster());

		return m;
	};

	void convertMapTerrain(std::shared_ptr<Map> m,
		const google::protobuf::RepeatedPtrField<::pb::MapCell> &li) {
		for (const auto &s : li) {

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
	};

	void convertMapTrigger(std::shared_ptr<Map> m,
		const google::protobuf::RepeatedPtrField<::pb::MapTrigger> &li) {
		for (const auto &t : li) {
			switch (t.trigger_case()) {
			case pb::MapTrigger::kGate: {
				m->gate.emplace_back(convertPBTriggerGate(t.id(), t.gate(), m));
				break;
			}
			case pb::MapTrigger::kExit: {
				m->exit.emplace_back(convertPBTriggerGate(t.id(), t.exit(), m));
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
	}

	void convertMapMonster(std::shared_ptr<Map> m,
		const google::protobuf::RepeatedPtrField<::pb::MapMonster> &li) {
		for (const auto &c : li) {
			auto &def = dst.monster.at(c.def());

			float scale =
				(def.scale ? def.scale : 1.0f) * (c.scale() ? c.scale() : 1.0f);
			float w = scale * def.sprite->physics.w;
			float h = scale * def.sprite->physics.h;

			m->monster.emplace_back(
				MapMonster(c.x(), c.y(), w / 2.0f, h / 2.0f, def));
		}
	}
};

}; // namespace asset
