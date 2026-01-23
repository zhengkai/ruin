#pragma once

#include "../util/pos.hpp"
#include "asset.hpp"
#include "map.hpp"
#include "pb/map.pb.h"

namespace asset {

MapGate convertPBTriggerGate(
	const uint32_t tid, const pb::MapTriggerGate &g, Map &m) {

	int id = static_cast<int>(tid);

	return MapGate{
		.id = id,
		.rect = util::convertIDToRect(id, m),
		.target =
			{
				{g.x(), g.y()},
				g.map(),
			},
	};
};

void convertMapStaticTerrain(
	Map &m, const google::protobuf::RepeatedPtrField<pb::MapCell> &li) {

	std::unordered_map<std::size_t, const pb::MapCell &> tile = {};
	for (const auto &s : li) {
		tile.emplace(static_cast<std::size_t>(s.id()), s);
	}

	for (std::size_t y = 0; y < m.h; y++) {
		for (std::size_t x = 0; x < m.w; x++) {
			auto id = y * m.w + x;
			if (tile.contains(id)) {
				auto &t = tile.at(id);
				auto i = static_cast<int>(id);
				m.terrain.emplace_back(MapCell{
					.tileName = t.tile().name(),
					.tileID = static_cast<int>(t.tile().id()),
					.pos = util::convertIDToPos(i, m),
				});
			} else {
				m.terrain.emplace_back(MapCell{
					.tileName = pb::Tileset_Name_unknown,
				});
			}
		}
	}
};

void convertMapTrigger(
	Map &m, const google::protobuf::RepeatedPtrField<pb::MapTrigger> &li) {
	for (const auto &t : li) {
		switch (t.trigger_case()) {
		case pb::MapTrigger::kGate: {
			m.gate.emplace_back(convertPBTriggerGate(t.id(), t.gate(), m));
			break;
		}
		case pb::MapTrigger::kExit: {
			m.exit.emplace_back(convertPBTriggerGate(t.id(), t.exit(), m));
			break;
		}
		case pb::MapTrigger::TRIGGER_NOT_SET:
		default:
			spdlog::info("unknown trigger");
			break;
		}
	}
};

void convertMapMonster(Map &m,
	Asset &dst,
	const google::protobuf::RepeatedPtrField<pb::MapMonster> &li) {
	for (const auto &c : li) {
		auto &def = dst.monster.at(c.def());

		float scale =
			(def.scale ? def.scale : 1.0f) * (c.scale() ? c.scale() : 1.0f);
		float w = scale * def.sprite.physics.w;
		float h = scale * def.sprite.physics.h;

		m.monster.emplace_back(MapMonster(c.x(), c.y(), w, h, def));
	}
};

}; // namespace asset
