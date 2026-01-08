#pragma once

#include "../config.hpp"
#include "../context/scene.hpp"
#include "body.hpp"
#include "common.hpp"
#include "tile.hpp"
#include "util.hpp"
#include "world.hpp"
#include <spdlog/spdlog.h>

namespace physics {

class Physics {

public:
	World world;

private:
	context::Scene &scene;
	std::size_t serial = 0;
	std::unordered_map<std::size_t, Speed> forSubStep = {};
	int mapIdx = -1;

public:
	Physics(context::Scene &cs) : scene(cs) {
		scene.player.x = config::posResetX;
		scene.player.y = config::posResetY;
		updateMap();
	};
	~Physics() {};

	void step() {

		updateMap();

		checkSpeed();

		for (auto &[id, b] : world.body) {
			Speed a = {.vx = b.vx, .vy = b.vy};
			Speed c = {};
			Speed n = {};
			if (limitSpeed(a, c, n)) {
				forSubStep[id] = n;
			}
			stepOne(b, c.vx, c.vy);
		}

		while (!forSubStep.empty()) {
			subStep();
		}

		for (auto &[_, b] : world.body) {
			checkReset(b);
			CheckTouch(b, world.tile);
		}
	};

	int addBody(const physics::Rect &rect) {
		auto serial = genSerial();
		auto b = newBody(serial, rect.x, rect.y, rect.w, rect.h);
		world.body[serial] = b;
		return serial;
	};

	Body &getBody(std::size_t serial) {
		return world.body.at(serial);
	}

	void dump() {
		spdlog::info("Physics tile count: {}", world.tile.size());
	};

private:
	void subStep() {

		std::vector<std::size_t> toErase;

		for (auto &[id, s] : forSubStep) {
			Speed a = {.vx = s.vx, .vy = s.vy};
			Speed c = {};
			Speed n = {};

			if (limitSpeed(a, c, n)) {
				forSubStep[id] = n;
			} else {
				toErase.push_back(id);
			}

			auto &b = world.body.at(id);
			stepOneSub(b, c.vx, c.vy);
		}

		for (auto id : toErase) {
			forSubStep.erase(id);
		}
	};

	void checkSpeed() {
		for (auto &[_, b] : world.body) {
			if (!b.enable) {
				continue;
			}
			if (b.gravity && !b.touch.d) {
				b.vy -= (b.vy > 0.0f) ? config::gravityUp : config::gravity;
				b.vy = std::max(b.vy, config::downSpeedMax);
			}
		}
	};

	void stepOne(Body &b, float vx, float vy) {

		if (vx < 0.0f) {
			if (!b.touch.l) {
				b.x += vx;
				stepLeft(b);
			}
		} else if (vx > 0.0f) {
			if (!b.touch.r) {
				b.x += vx;
				stepRight(b);
			}
		}

		if (vy < 0.0f) {
			if (!b.touch.d) {
				b.y += vy;
				stepDown(b);
			}
		} else if (vy > 0.0f) {
			if (!b.touch.u) {
				b.y += vy;
				stepUp(b);
			}
		}
	};

	void stepOneSub(Body &b, float vx, float vy) {

		if (vx < 0.0f) {
			b.x += vx;
			stepLeft(b);
		} else if (vx > 0.0f) {
			b.x += vx;
			stepRight(b);
		}

		if (vy < 0.0f) {
			b.y += vy;
			stepDown(b);
		} else if (vy > 0.0f) {
			b.y += vy;
			stepUp(b);
		}
	};

	void stepDown(Body &b) {
		float rb = CheckRollback(b, world.tile, Direction::Down);
		if (rb > 0.0f) {
			b.y += rb;
			b.vy = 0.0f;
		} else if (rb == 0.0f) {
			b.y += rb;
		}
	};
	void stepUp(Body &b) {
		float rb = CheckRollback(b, world.tile, Direction::Up);
		if (rb > 0.0f) {
			b.y -= rb;
			b.vy = 0.0f;
		} else if (rb == 0.0f) {
			b.y -= rb;
		}
	};
	void stepLeft(Body &b) {
		float rb = CheckRollback(b, world.tile, Direction::Left);
		if (rb > 0.0f) {
			b.x += rb;
			b.vx = 0.0f;
		} else if (rb == 0.0f) {
			b.x += rb;
		}
	};
	void stepRight(Body &b) {
		float rb = CheckRollback(b, world.tile, Direction::Right);
		if (rb > 0.0f) {
			b.x -= rb;
			b.vx = 0.0f;
		} else if (rb == 0.0f) {
			b.x -= rb;
		}
	};

	void checkReset(Body &b) {
		if (b.x < -2.0f || b.y < -2.0f || b.x > world.w || b.y > world.h) {
			b.x = config::posResetX;
			b.y = config::posResetY;
			b.vx = 0.0f;
			b.vy = 0.0f;
		}
	};

	void clean() {};

private:
	std::size_t genSerial() {
		return ++serial;
	};

	void updateMap() {
		auto &m = scene.map;
		if (mapIdx == m->idx) {
			return;
		}

		world.Reset(m->w, m->h);

		initPlayer();
		initTile();

		mapIdx = m->idx;
	}

	void initPlayer() {
		auto &sp = scene.player;
		sp.physicsSerial = addBody(sp);
	};

	void initTile() {
		auto &cl = scene.map->terrain;
		if (!cl.size()) {
			spdlog::warn("No map cells to init tiles.");
			return;
		}
		for (const auto &c : cl) {
			addTile(c.pos);
		}
	};

	int addTile(const Pos &pos) {
		auto serial = genSerial();
		auto t = newTile(serial, pos);
		world.tile[serial] = t;
		return serial;
	};
};
}; // namespace physics
