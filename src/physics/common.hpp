#pragma once

#include "pos.hpp"
#include "state.hpp"
#include <cstdint>
#include <spdlog/spdlog.h>

namespace physics {

enum class ContactType : uint8_t {
	None,	// 完全不相交
	Touch,	// 贴边
	Overlap // 穿透
};

struct Touch {
	bool u = false;
	bool d = false;
	bool l = false;
	bool r = false;

	void Reset() {
		*this = {};
	}
};

struct IntersectEvent {
	bool hit = false;
	bool touch = false;
	bool overlap = false;
	float u = -1.0f;
	float d = -1.0f;
	float l = -1.0f;
	float r = -1.0f;

	void Reset() {
		*this = {};
	}
};

struct Speed {
	float vx = 0.0f;
	float vy = 0.0f;
};

struct Box : Pos {
	float w; // half witdth
	float h; // half height

	void checkIntersects(const Box &b, IntersectEvent &re) const {

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

	float getRollback(const Box &b, Direction d) const {

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

	bool overlapX(const Box &b) const {
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

	bool overlapY(const Box &b) const {
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
