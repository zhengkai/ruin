#pragma once

#include "tile.hpp"
#include "world.hpp"
#include <spdlog/spdlog.h>

namespace physics {

class Physics {

private:
	std::size_t serial = 0;
	World world;

public:
	Physics() {};
	~Physics() {};

	void step() {};

	void clean() {};

	void dump() {
		spdlog::info("Physics tile count: {}", world.tiles.size());
	};

	int addTile(float x, float y) {
		auto serial = genSerial();
		auto t = Tile{.serial = serial, .x = x, .y = y};
		world.tiles[serial] = t;
		return serial;
	}

private:
	std::size_t genSerial() {
		return ++serial;
	};
};

}; // namespace physics
