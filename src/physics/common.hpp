#pragma once

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

}; // namespace physics
