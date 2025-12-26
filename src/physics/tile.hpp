#pragma once

#include "common.hpp"
#include <cstddef>

namespace physics {

struct Tile : Box {
	std::size_t serial;
	bool enable = true;
};

inline Tile newTile(std::size_t serial, float x, float y) {
	return Tile{Box{x, y, 0.5f, 0.5f}, serial};
};

}; // namespace physics
