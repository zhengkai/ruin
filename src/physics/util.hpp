#pragma once

#include "../config.hpp"
#include "body.hpp"
#include "tile.hpp"
#include <unordered_map>
#include <vector>

namespace physics {

inline std::vector<IntersectEvent> CheckIntersects(
	const Body body, const std::unordered_map<std::size_t, Tile> &tl) {
	IntersectEvent e = {};
	auto el = std::vector<IntersectEvent>{};

	float l = body.x - body.w - config::physicsScanRange;
	float r = body.x + body.w + config::physicsScanRange;
	float u = body.y - body.h - config::physicsScanRange;
	float d = body.y + body.h + config::physicsScanRange;
	for (auto &[_, t] : tl) {
		if (!t.enable || t.x < l || t.x > r || t.y < u || t.y > d) {
			continue;
		}
		body.checkIntersects(t, e);
		if (!e.hit) {
			continue;
		}
		el.push_back(e);
	}
	return el;
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
