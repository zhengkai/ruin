#pragma once

#include "../util/pos.hpp"
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

}; // namespace asset
