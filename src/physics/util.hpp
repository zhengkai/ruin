#pragma once

#include "../config.hpp"
#include "body.hpp"
#include "state.hpp"
#include "tile.hpp"
#include <unordered_map>
#include <vector>

namespace physics {

inline void CheckTouch(
	Body &b, const std::unordered_map<std::size_t, Tile> &tl) {

	b.touch = {};

	float l = b.x - b.w - config::physicsScanRange;
	float r = b.x + b.w + config::physicsScanRange;
	float u = b.y + b.h + config::physicsScanRange;
	float d = b.y - b.h - config::physicsScanRange;

	for (auto &[_, t] : tl) {
		if (!t.enable || t.x < l || t.x > r || t.y < d || t.y > u) {
			continue;
		}
		if (!b.touch.l && b.getRollback(t, Direction::Left) != -1.0f) {
			b.touch.l = true;
		}
		if (!b.touch.r && b.getRollback(t, Direction::Right) != -1.0f) {
			spdlog::info("touch right tile {}, {} {}, {} {}",
				t.serial,
				t.x,
				t.y,
				b.x,
				b.y);
			b.touch.r = true;
		}
		if (!b.touch.u && b.getRollback(t, Direction::Up) != -1.0f) {
			b.touch.u = true;
		}
		if (!b.touch.d && b.getRollback(t, Direction::Down) != -1.0f) {
			b.touch.d = true;
		}
		if (b.touch.l && b.touch.r && b.touch.u && b.touch.d) {
			break;
		}
	}
}

inline float CheckRollback(const Body &body,
	const std::unordered_map<std::size_t, Tile> &tl,
	Direction direction) {

	float l = direction == Direction::Right
		? body.x + body.w
		: body.x - body.w - config::physicsScanRange;
	float r = direction == Direction::Left
		? body.x - body.w
		: body.x + body.w + config::physicsScanRange;
	float u = direction == Direction::Down
		? body.y - body.h
		: body.y + body.h + config::physicsScanRange;
	float d = direction == Direction::Up
		? body.y + body.h
		: body.y - body.h - config::physicsScanRange;

	float rollback = -1.0f;
	int i = 0;
	int j = 0;
	int k = 0;
	for (auto &[_, t] : tl) {
		i++;
		if (!t.enable || t.x < l || t.x > r || t.y < d || t.y > u) {
			continue;
		}
		j++;
		float rb = body.getRollback(t, direction);
		if (rb >= 0.0f) {
			spdlog::debug(
				"roll {}, x: {}, y: {}, {}", t.serial, t.x, t.y, direction);
			k++;
		}
		rollback = std::max(rollback, rb);
	}

	spdlog::debug("total {}, check {}, effect {}", i, j, k);

	return rollback;
};

inline bool limitSpeed(Speed &origin, Speed &now, Speed &next) {

	bool overSpeed = false;

	if (origin.vx > config::halfFloat) {
		now.vx = config::halfFloat;
		next.vx = origin.vx - config::halfFloat;
		overSpeed = true;
	} else if (origin.vx < config::halfFloatNegative) {
		now.vx = config::halfFloatNegative;
		next.vx = origin.vx + config::halfFloat;
		overSpeed = true;
	} else {
		now.vx = origin.vx;
		next.vx = 0.0f;
	}

	if (origin.vy > config::halfFloat) {
		now.vy = config::halfFloat;
		next.vy = origin.vy - config::halfFloat;
		overSpeed = true;
	} else if (origin.vy < config::halfFloatNegative) {
		now.vy = config::halfFloatNegative;
		next.vy = origin.vy + config::halfFloat;
		overSpeed = true;
	} else {
		now.vy = origin.vy;
		next.vy = 0.0f;
	}

	return overSpeed;
};

inline IntersectEvent IntersectsOverall(const std::vector<IntersectEvent> &el) {
	IntersectEvent eo = {};
	for (auto &e : el) {
		eo.u = std::max(eo.u, e.u);
		eo.d = std::max(eo.d, e.d);
		eo.l = std::max(eo.l, e.l);
		eo.r = std::max(eo.r, e.r);
		if (e.hit) {
			eo.hit = true;
		}
		if (e.touch) {
			eo.touch = true;
		}
		if (e.overlap) {
			eo.overlap = true;
		}
	}
	return eo;
};
}; // namespace physics
