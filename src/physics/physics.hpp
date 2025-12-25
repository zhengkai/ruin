#pragma once

#include "../asset/asset.hpp"
#include "../context/scene.hpp"
#include "body.hpp"
#include "tile.hpp"
#include "world.hpp"
#include <spdlog/spdlog.h>

namespace physics {

struct Dep {
	context::Scene &scene;
	const asset::Asset &asset;
};

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
		spdlog::info("Physics tile count: {}", world.tile.size());
	};

	int addBody(float x, float y, float w, float h) {
		auto serial = genSerial();
		auto b = newBody(serial, x, y, w, h);
		world.body[serial] = b;
		return serial;
	};

	int addTile(float x, float y) {
		auto serial = genSerial();
		auto t = newTile(serial, x, y);
		world.tile[serial] = t;
		return serial;
	};

private:
	std::size_t genSerial() {
		return ++serial;
	};
};

}; // namespace physics
