#pragma once

#include "tile.hpp"
#include <unordered_map>

namespace physics {

struct World {
	std::unordered_map<std::size_t, Tile> tiles;
};

}; // namespace physics
