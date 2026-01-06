#pragma once

#include "../asset/map.hpp"
#include "../physics/pos.hpp"

namespace util {

inline physics::Pos convertIDToPos(int &id, int &w, int &h) {
	return physics::Pos{
		.x = static_cast<float>(id % w),
		.y = static_cast<float>(h - 1 - id / w),
	};
};

inline physics::Pos convertIDToPos(int &id, std::shared_ptr<asset::Map> map) {
	return convertIDToPos(id, map->w, map->h);
};

}; // namespace util
