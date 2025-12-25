#pragma once

#include "common.hpp"

namespace physics {

struct Tile {
	std::size_t serial;
	bool enable = true;
	Position pos;
	AABB box{0.5f, 0.5f};
};

inline Tile newTile(std::size_t serial, float x, float y) {
	return Tile{.serial = serial, .pos = {.x = x, .y = y}};
};

}; // namespace physics
