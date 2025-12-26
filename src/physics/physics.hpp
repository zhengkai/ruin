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

	void step() {
		spdlog::info("physics tick");

		IntersectResult re = {};
		for (auto &[_, b] : world.body) {
			bool hit = false;
			int i = 0;
			for (auto &[_, t] : world.tile) {
				i++;
				b.checkIntersects(t, re);
				if (re.overall != ContactType::None) {
					hit = true;
					break;
				}
			}
			if (!hit) {
				b.y -= 0.1f;
			}
			spdlog::info("body {} {} {}", b.serial, i, world.tile.size());
		};
	};

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

	Body &getBody(std::size_t serial) {
		return world.body.at(serial);
	}

private:
	std::size_t genSerial() {
		return ++serial;
	};
};

}; // namespace physics
