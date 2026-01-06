#pragma once

#include "common.hpp"
#include <cstddef>

namespace physics {

struct Tile : Box {
	std::size_t serial;
	bool enable = true;
};

inline Tile newTile(std::size_t serial, Pos pos) {
	return Tile{Box{pos, 0.5f, 0.5f}, serial};
};

}; // namespace physics
