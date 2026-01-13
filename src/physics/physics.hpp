#pragma once

#include "../asset/map.hpp"
#include "../config.hpp"
#include "../game/reg.hpp"
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
	game::Reg &reg;
	std::size_t serial = 0;
	std::unordered_map<entt::entity, Speed> forSubStep = {};

public:
	Physics(std::shared_ptr<asset::Map> map, game::Reg &reg_) : reg(reg_) {

		world.Reset(map->w, map->h);

		auto &cl = map->terrain;
		if (!cl.size()) {
			spdlog::warn("No map cells to init tiles.");
		}
		for (const auto &c : cl) {
			addTile(c.pos);
		}
	};
	~Physics() {};

	void step() {

		checkSpeed();

		auto view = reg.view<Rect, Body>();

		for (auto [e, rect, b] : view.each()) {
			Speed a = {.vx = b.vx, .vy = b.vy};
			Speed c = {};
			Speed n = {};
			if (limitSpeed(a, c, n)) {
				forSubStep[e] = n;
			}
			stepOne(b, rect, c.vx, c.vy);
		}

		while (!forSubStep.empty()) {
			subStep();
		}

		for (auto [e, rect, b] : view.each()) {
			checkReset(b, rect);
			CheckTouch(b, rect, world.tile);
		}
	};

	void dump() {
		spdlog::info("Physics tile count: {}", world.tile.size());
	};

private:
	void subStep() {

		std::vector<entt::entity> toErase;

		for (auto &[e, s] : forSubStep) {
			Speed a = {.vx = s.vx, .vy = s.vy};
			Speed c = {};
			Speed n = {};

			if (limitSpeed(a, c, n)) {
				forSubStep[e] = n;
			} else {
				toErase.push_back(e);
			}

			auto &b = reg.get<Body>(e);
			auto &rect = reg.get<Rect>(e);
			stepOneSub(b, rect, c.vx, c.vy);
		}

		for (auto id : toErase) {
			forSubStep.erase(id);
		}
	};

	void checkSpeed() {

		auto view = reg.view<Body>();

		for (auto [_, b] : view.each()) {
			if (b.gravity && !b.touch.d) {
				b.vy -= (b.vy > 0.0f) ? config::gravityUp : config::gravity;
				b.vy = std::max(b.vy, config::downSpeedMax);
			}
		}
	};

	void stepOne(Body &b, Rect &rect, float vx, float vy) {

		if (vx < 0.0f) {
			if (!b.touch.l) {
				rect.x += vx;
				stepLeft(b, rect);
			}
		} else if (vx > 0.0f) {
			if (!b.touch.r) {
				rect.x += vx;
				stepRight(b, rect);
			}
		}

		if (vy < 0.0f) {
			if (!b.touch.d) {
				rect.y += vy;
				stepDown(b, rect);
			}
		} else if (vy > 0.0f) {
			if (!b.touch.u) {
				rect.y += vy;
				stepUp(b, rect);
			}
		}
	};

	void stepOneSub(Body &b, Rect rect, float vx, float vy) {

		if (vx < 0.0f) {
			rect.x += vx;
			stepLeft(b, rect);
		} else if (vx > 0.0f) {
			rect.x += vx;
			stepRight(b, rect);
		}

		if (vy < 0.0f) {
			rect.y += vy;
			stepDown(b, rect);
		} else if (vy > 0.0f) {
			rect.y += vy;
			stepUp(b, rect);
		}
	};

	void stepDown(Body &b, Rect &rect) {
		float rb = CheckRollback(rect, world.tile, Direction::Down);
		if (rb > 0.0f) {
			rect.y += rb;
			b.vy = 0.0f;
		} else if (rb == 0.0f) {
			rect.y += rb;
		}
	};
	void stepUp(Body &b, Rect &rect) {
		float rb = CheckRollback(rect, world.tile, Direction::Up);
		if (rb > 0.0f) {
			rect.y -= rb;
			b.vy = 0.0f;
		} else if (rb == 0.0f) {
			rect.y -= rb;
		}
	};
	void stepLeft(Body &b, Rect &rect) {
		float rb = CheckRollback(rect, world.tile, Direction::Left);
		if (rb > 0.0f) {
			rect.x += rb;
			b.vx = 0.0f;
		} else if (rb == 0.0f) {
			rect.x += rb;
		}
	};
	void stepRight(Body &b, Rect &rect) {
		float rb = CheckRollback(rect, world.tile, Direction::Right);
		if (rb > 0.0f) {
			rect.x -= rb;
			b.vx = 0.0f;
		} else if (rb == 0.0f) {
			rect.x -= rb;
		}
	};

	void checkReset(Body &b, Rect &rect) {
		if (rect.x < -2.0f || rect.y < -2.0f || rect.x > world.w ||
			rect.y > world.h) {
			rect.x = config::posResetX;
			rect.y = config::posResetY;
			b.vx = 0.0f;
			b.vy = 0.0f;
		}
	};

	void clean() {};

private:
	std::size_t genSerial() {
		return ++serial;
	};

	int addTile(const Pos &pos) {
		auto serial = genSerial();
		auto t = newTile(serial, pos);
		world.tile[serial] = t;
		return serial;
	};
};
}; // namespace physics
