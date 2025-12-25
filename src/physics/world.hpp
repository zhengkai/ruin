#pragma once

#include "body.hpp"
#include "tile.hpp"
#include <unordered_map>

namespace physics {

struct World {
	std::unordered_map<std::size_t, Tile> tile;
	std::unordered_map<std::size_t, Body> body;
};

}; // namespace physics
