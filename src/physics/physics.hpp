#pragma once

#include "../asset/map.hpp"
#include "../config.hpp"
#include "../game/reg.hpp"
#include "body.hpp"
#include "common.hpp"
#include "util.hpp"
#include <spdlog/spdlog.h>

namespace physics {

class Physics {

private:
	std::shared_ptr<asset::Map> map;
	game::Reg &reg;
	Pos worldEnd;
	std::size_t serial = 0;
	std::unordered_map<entt::entity, Vec2> forSubStep = {};

public:
	Physics(std::shared_ptr<asset::Map> map_, game::Reg &reg_)
		: map{map_}, reg{reg_}, worldEnd{.x = static_cast<float>(map_->w + 2),
									.y = static_cast<float>(map_->h + 2)} {};

	~Physics() {};

	void step() {

		auto view = reg.view<Rect, Body, tag::PrevPos>();

		for (auto [e, rect, b, prevPos] : view.each()) {

			// 保存 PrevPos
			prevPos.x = rect.x;
			prevPos.y = rect.y;

			checkSpeed(b);

			Vec2 a = {.vx = b.vx, .vy = b.vy};
			Vec2 c = {};
			Vec2 n = {};
			if (limitSpeed(a, c, n)) {
				forSubStep[e] = n;
			}
			stepOne(b, rect, c.vx, c.vy);
		}

		while (!forSubStep.empty()) {
			subStep();
		}

		for (auto [e, rect, b, _] : view.each()) {
			checkReset(b, rect);
			CheckTouch(b, rect, map);
		}
	};

private:
	void subStep() {

		std::vector<entt::entity> toErase;

		for (auto &[e, s] : forSubStep) {
			Vec2 a = {.vx = s.vx, .vy = s.vy};
			Vec2 c = {};
			Vec2 n = {};

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

	// 掉落限速
	void checkSpeed(Body &b) {
		if (b.gravity && !b.touch.d) {
			b.vy -= (b.vy > 0.0f) ? config::gravityUp : config::gravity;
			b.vy = std::max(b.vy, config::downSpeedMax);
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
		float rb = CheckRollback(rect, map, Direction::Down);
		if (rb > 0.0f) {
			rect.y += rb;
			b.vy = 0.0f;
		} else if (rb == 0.0f) {
			rect.y += rb;
		}
	};
	void stepUp(Body &b, Rect &rect) {
		float rb = CheckRollback(rect, map, Direction::Up);
		if (rb > 0.0f) {
			rect.y -= rb;
			b.vy = 0.0f;
		} else if (rb == 0.0f) {
			rect.y -= rb;
		}
	};
	void stepLeft(Body &b, Rect &rect) {
		float rb = CheckRollback(rect, map, Direction::Left);
		if (rb > 0.0f) {
			rect.x += rb;
			b.vx = 0.0f;
		} else if (rb == 0.0f) {
			rect.x += rb;
		}
	};
	void stepRight(Body &b, Rect &rect) {
		float rb = CheckRollback(rect, map, Direction::Right);
		if (rb > 0.0f) {
			rect.x -= rb;
			b.vx = 0.0f;
		} else if (rb == 0.0f) {
			rect.x -= rb;
		}
	};

	void checkReset(Body &b, Rect &rect) {
		if (rect.x < -2.0f || rect.y < -2.0f || rect.x > worldEnd.x ||
			rect.y > worldEnd.y) {
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
};
}; // namespace physics
