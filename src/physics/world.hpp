#pragma once

#include "body.hpp"
#include "tile.hpp"
#include <unordered_map>

namespace physics {

struct World {
	float h = 100.0f;
	float w = 100.0f;
	std::unordered_map<std::size_t, Tile> tile = {};
	std::unordered_map<std::size_t, Body> body = {};
};

}; // namespace physics
