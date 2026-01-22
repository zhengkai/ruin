#pragma once

#include "../asset/map.hpp"
#include "../physics/pos.hpp"

namespace util {

inline physics::Pos convertIDToPos(
	const int &id, const std::size_t &w, const std::size_t &h) {
	return physics::Pos{
		.x = static_cast<float>(id % w),
		.y = static_cast<float>(id / w),
	};
};

inline physics::Pos convertIDToPos(const int &id, asset::Map &map) {
	return convertIDToPos(id, map.w, map.h);
};

inline physics::Rect convertIDToRect(
	const int &id, asset::Map &map, float size = 0.5f) {
	return {
		convertIDToPos(id, map.w, map.h),
		size,
		size,
	};
};

}; // namespace util
