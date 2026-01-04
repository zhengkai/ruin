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

	void Reset(int w, int h) {
		this->w = static_cast<float>(w + 2);
		this->h = static_cast<float>(h + 20);
		tile.clear();
		body.clear();
	}
};

}; // namespace physics
