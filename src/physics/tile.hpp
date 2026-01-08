#pragma once

#include "rect.hpp"
#include <cstddef>

namespace physics {

struct Tile : Rect {
	std::size_t serial;
	bool enable = true;
};

inline Tile newTile(std::size_t serial, Pos pos) {
	return Tile{Rect{pos, 0.5f}, serial};
};

}; // namespace physics
