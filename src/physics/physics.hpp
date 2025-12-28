#pragma once

#include "../asset/asset.hpp"
#include "../config.hpp"
#include "../context/scene.hpp"
#include "body.hpp"
#include "tile.hpp"
#include "util.hpp"
#include "world.hpp"
#include <spdlog/spdlog.h>

namespace physics {

struct Dep {
	context::Scene &scene;
	const asset::Asset &asset;
};

class Physics {

public:
	World world;

private:
	std::size_t serial = 0;

public:
	Physics() {};
	~Physics() {};

	void step() {
		spdlog::info("physics tick");
		for (auto &[_, b] : world.body) {
			stepOne(b);
			if (b.x < 0.0f || b.y < 0.0f) {
				b.x = 10.0f;
				b.y = 13.0f;
			}
		}
	};

	void stepOne(Body &b) {

		if (b.gravity && !b.touch.d) {
			b.vy -= b.vy > 0.0f ? config::gravityUp : config::gravity;
		}

		if (b.vy < 0.0f) {
			b.y += b.vy;
			stepDown(b);
		} else if (b.vy > 0.0f) {
			b.y += b.vy;
			stepUp(b);
		}

		if (b.vx < 0.0f) {
			b.x += b.vx;
			stepLeft(b);
		} else if (b.vx > 0.0f) {
			b.x += b.vx;
			stepRight(b);
		}
	};

	void stepDown(Body &b) {
		float rb = CheckRollback(b, world.tile, Direction::Down);
		if (rb > 0.0f) {
			b.y += rb;
			b.vy = 0.0f;
			b.touch.d = true;
		} else if (rb == 0.0f) {
			b.y += rb;
			b.touch.d = true;
		} else {
			b.touch.d = false;
		}
	};
	void stepUp(Body &b) {
		float rb = CheckRollback(b, world.tile, Direction::Up);
		if (rb < 0.0f) {
			b.y -= rb;
			b.vy = 0.0f;
			b.touch.u = true;
		} else if (rb == 0.0f) {
			b.y -= rb;
			b.touch.u = true;
		} else {
			b.touch.u = false;
		}
	};
	void stepLeft(Body &b) {
		float rb = CheckRollback(b, world.tile, Direction::Left);
		if (rb > 0.0f) {
			b.x += rb;
			b.vx = 0.0f;
			b.touch.l = true;
		} else if (rb == 0.0f) {
			b.x += rb;
			b.touch.l = true;
		} else {
			b.touch.l = false;
		}
	};
	void stepRight(Body &b) {
		float rb = CheckRollback(b, world.tile, Direction::Right);
		if (rb < 0.0f) {
			b.x -= rb;
			b.vx = 0.0f;
			b.touch.r = true;
		} else if (rb == 0.0f) {
			b.x -= rb;
			b.touch.r = true;
		} else {
			b.touch.r = false;
		}
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
