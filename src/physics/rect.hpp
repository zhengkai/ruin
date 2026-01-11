#pragma once

#include "common.hpp"
#include "pos.hpp"
#include "state.hpp"
#include <SDL3/SDL_rect.h>
#include <spdlog/spdlog.h>

namespace physics {

struct Rect : Pos {
	float w;
	float h;

	Rect(Pos pos, float w, float h) : Pos{pos}, w(w), h(h) {};

	Rect(Pos pos, float size) : Pos{pos}, w(size), h(size) {};

	Rect(Pos pos) : Pos{pos}, w(0.5f), h(0.5f) {};

	Rect(float x, float y, float w, float h) : Pos{x, y}, w(w), h(h) {};

	Rect(float x, float y, float size) : Pos{x, y}, w(size), h(size) {
		spdlog::info("Rect({}, {}, {}, {})", x, y, w, h);
	};

	Rect() : Pos{0.0f, 0.0f}, w(0.5f), h(0.5f) {};

	void updatePos(const Pos &p) {
		Pos::x = p.x;
		Pos::y = p.y;
	};

	void updateRect(const Rect &r) {
		*this = r;
	};

	SDL_FRect getRect() const {
		return SDL_FRect{
			.x = Pos::x, .y = Pos::y, .w = w * 2.0f, .h = h * 2.0f};
	};

	bool isOverlap(const Rect &other) const {

		if (x > other.x) {
			if (x - w > other.x + other.w) {
				return false;
			}
		} else if (x < other.x) {
			if (other.x - other.w > x + w) {
				return false;
			}
		}

		if (y > other.y) {
			if (y - h > other.y + other.h) {
				return false;
			}
		} else if (y < other.y) {
			if (other.y - other.h > y + h) {
				return false;
			}
		}

		return true;
	}

	void checkIntersects(const Rect &b, IntersectEvent &re) const {

		float l = -1.0f;
		float r = -1.0f;

		if (x < b.x) {
			float diff = x + w + b.w;
			if (diff < b.x) {
				re.Reset();
				return;
			};
			r = diff - b.x;
		} else if (x > b.x) {
			float diff = b.x + b.w + w;
			if (diff < x) {
				re.Reset();
				return;
			}
			l = diff - x;
		} else {
			l = b.w + w;
			r = l;
		}

		if (y < b.y) {
			float diff = y + h + b.h;
			if (diff < b.y) {
				re.Reset();
				return;
			}
			re.u = diff - b.y;
		} else if (y > b.y) {
			float diff = b.y + b.h + h;
			if (diff < y) {
				re.Reset();
				return;
			}
			re.d = diff - y;
		} else {
			re.u = b.h + h;
			re.d = re.u;
		}

		if (re.u > 0.0f || re.d > 0.0f) {
			re.l = l;
			re.r = r;
		}

		re.hit = true;
		re.touch = re.d == 0.0f || re.u == 0.0f || re.l == 0.0f || re.r == 0.0f;
		re.overlap = re.d > 0.0f || re.u > 0.0f || re.l > 0.0f || re.r > 0.0f;
	};

	float getRollback(const Rect &b, Direction d) const {

		float rb;
		switch (d) {

		case Direction::Down:
			if (!overlapX(b)) {
				return -1.0f;
			}
			rb = (b.y + b.h) - (y - h);
			if (rb < 0.0f) {
				return -1.0f;
			}
			if (rb >= (b.h + h)) {
				return -1.0f;
			}
			break;

		case Direction::Up:
			if (!overlapX(b)) {
				return -1.0f;
			}
			rb = (y + h) - (b.y - b.h);
			if (rb < 0.0f) {
				return -1.0f;
			}
			if (rb >= (b.h + h)) {
				return -1.0f;
			}
			break;

		case Direction::Left:
			if (!overlapY(b)) {
				return -1.0f;
			}
			rb = (b.x + b.w) - (x - w);
			if (rb < 0.0f) {
				return -1.0f;
			}
			if (rb > (b.w + w)) {
				return -1.0f;
			}
			break;

		case Direction::Right:
			if (!overlapY(b)) {
				return -1.0f;
			}
			rb = (x + w) - (b.x - b.w);
			if (rb < 0.0f) {
				return -1.0f;
			}
			if (rb >= (b.w + w)) {
				return -1.0f;
			}
			break;

		default:
			return -1.0f;
			break;
		}

		return rb;
	};

	bool overlapX(const Rect &b) const {
		if (x < b.x) {
			if (x + w <= b.x - b.w) {
				return false;
			}
		} else if (x > b.x) {
			if (b.x + b.w <= x - w) {
				return false;
			}
		}
		return true;
	};

	bool overlapY(const Rect &b) const {
		if (y < b.y) {
			if (y + h <= b.y - b.h) {
				return false;
			}
		} else if (y > b.y) {
			if (b.y + b.h <= y - h) {
				return false;
			}
		}
		return true;
	};
};

}; // namespace physics
