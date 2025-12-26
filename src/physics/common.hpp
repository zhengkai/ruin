#pragma once

#include <cmath>
#include <cstdint>

namespace physics {

enum class ContactType : uint8_t {
	None,	// 完全不相交
	Touch,	// 贴边
	Overlap // 穿透
};

struct IntersectResult {
	ContactType overall = ContactType::None;
	ContactType u = ContactType::None;
	ContactType d = ContactType::None;
	ContactType l = ContactType::None;
	ContactType r = ContactType::None;

	void Reset() {
		*this = {};
	}
};

struct Box {
	float x;
	float y;
	float w; // half witdth
	float h; // half height

	void checkIntersects(const Box &b, IntersectResult &re) const {

		ContactType l = ContactType::None;
		ContactType r = ContactType::None;

		if (x < b.x) {
			float diff = x + w + b.w;
			if (diff < b.x) {
				re.Reset();
				return;
			} else if (diff == b.x) {
				r = ContactType::Touch;
			} else {
				r = ContactType::Overlap;
			}
		} else if (x > b.x) {
			float diff = b.x + b.w + w;
			if (diff < x) {
				re.Reset();
				return;
			} else if (diff == x) {
				l = ContactType::Touch;
			} else {
				l = ContactType::Overlap;
			}
		} else {
			l = ContactType::Overlap;
			r = ContactType::Overlap;
		}

		if (y < b.y) {
			float diff = y + h + b.h;
			if (diff < b.y) {
				re.Reset();
				return;
			} else if (diff == b.y) {
				re.u = ContactType::Touch;
			} else {
				re.u = ContactType::Overlap;
			}
		} else if (y > b.y) {
			float diff = b.y + b.h + h;
			if (diff < y) {
				re.Reset();
				return;
			} else if (diff == y) {
				re.d = ContactType::Touch;
			} else {
				re.d = ContactType::Overlap;
			}
		} else {
			re.u = ContactType::Overlap;
			re.d = ContactType::Overlap;
		}

		re.l = l;
		re.r = r;
		re.overall = ContactType::Overlap;
	};
};

}; // namespace physics
