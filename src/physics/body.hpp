#pragma once

#include "common.hpp"
#include "rect.hpp"
#include <cstddef>

namespace physics {

struct Body {
	std::size_t serial;
	bool enable = true;
	Touch touch = {};
	IntersectEvent eventOverall = {};
	float vx = 0.0f;
	float vy = 0.0f;
	bool gravity = true;
};

}; // namespace physics
