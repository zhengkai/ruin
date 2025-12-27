#pragma once

#include <cstdint>

namespace physics {

enum class ContactType : uint8_t {
	None,	// 完全不相交
	Touch,	// 贴边
	Overlap // 穿透
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

struct Box {
	float x;
	float y;
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

		re.l = l;
		re.r = r;

		re.hit = true;
		re.touch = re.d == 0.0f || re.u == 0.0f || re.l == 0.0f || re.r == 0.0f;
		re.overlap = re.d > 0.0f || re.u > 0.0f || re.l > 0.0f || re.r > 0.0f;
	};
};

}; // namespace physics
