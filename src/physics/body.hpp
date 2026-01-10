#pragma once

#include "common.hpp"
#include "rect.hpp"
#include <cstddef>

namespace physics {

struct Body : Speed {
	std::size_t serial;
	bool enable = true;
	Rect &rect;
	Touch touch = {};
	IntersectEvent eventOverall = {};
	float vx = 0.0f;
	float vy = 0.0f;
	bool gravity = true;

	Body(std::size_t serial_, Rect &rect_) : serial(serial_), rect(rect_) {};
};

}; // namespace physics
