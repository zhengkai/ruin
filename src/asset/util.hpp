#pragma once

#include "../util/pos.hpp"
#include "asset.hpp"
#include "map.hpp"
#include "pb/map.pb.h"

namespace asset {

MapGate convertPBTriggerGate(
	const uint32_t tid, const pb::MapTriggerGate &g, Zone &z) {

	int id = static_cast<int>(tid);

	return MapGate{
		.id = id,
		.rect = util::convertIDToRect(id, z.map),
		.target =
			{
				{g.x(), g.y()},
				g.zone(),
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
	Zone &z, const google::protobuf::RepeatedPtrField<pb::MapTrigger> &li) {
	for (const auto &t : li) {
		switch (t.trigger_case()) {
		case pb::MapTrigger::kGate: {
			z.gate.emplace_back(convertPBTriggerGate(t.id(), t.gate(), z));
			break;
		}
		case pb::MapTrigger::kExit: {
			z.exit.emplace_back(convertPBTriggerGate(t.id(), t.exit(), z));
			break;
		}
		case pb::MapTrigger::TRIGGER_NOT_SET:
		default:
			spdlog::info("unknown trigger");
			break;
		}
	}
};

void convertMapMob(Zone &z,
	Asset &dst,
	const google::protobuf::RepeatedPtrField<pb::MapMob> &li) {
	for (const auto &c : li) {
		auto &def = dst.mob.at(c.def());

		float scale =
			(def.scale ? def.scale : 1.0f) * (c.scale() ? c.scale() : 1.0f);
		float w = scale * def.sprite.physics.w;
		float h = scale * def.sprite.physics.h;

		z.mob.emplace_back(MapMob(c.x(), c.y(), w, h, def));
	}
};

}; // namespace asset
